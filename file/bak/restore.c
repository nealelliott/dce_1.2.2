/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: restore.c,v $
 * Revision 1.1.98.1  1996/10/02  16:58:52  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:57  damon]
 *
 * $EndLog$
 */
/*
 * restore.c -- Fileset restore functions
 */ 

#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <sys/types.h>
#include <dcedfs/cmd.h>
#include <sys/time.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/butc.h>
#include <com_err.h>
#include <bc.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bak/restore.c,v 1.1.98.1 1996/10/02 16:58:52 damon Exp $")

static int BackupName();
extern int nonInteractiveBak;
extern struct bc_config *bc_globalConfig;
extern struct bc_dumpTask bc_dumpTasks[BC_MAXSIMDUMPS];
extern char *whoami;
extern char *hostutil_GetNameByINet();
extern rpc_binding_handle_t bc_GetConn();
#ifndef FOOBAR_MODE
#define	BC_MAXLEVELS	    20
#endif /* FOOBAR_MODE */
#define	MAXTAPESATONCE	    10

/* local structure to keep track of filesets and the dumps 
   from which they should be restored */
struct bc_dumpList {
    struct bc_dumpList *next;
    char *volumeName;	    /* restore this fileset from this dump */
    long dumpID;
    long level;
    afsNetAddr server; 
    long partition; 
};

/* local structure used to keep track of a tape, including a list of things to restore from that tape */
struct bc_tapeList {
    struct bc_tapeList *next;		    /* next guy in list */
    char *tapeName;			    /* name of the tape */
    long dumpID;			    /* dump located on this tape */
    long tapeNumber;			    /* which tape in the dump */
    struct bc_tapeItem *restoreList;	    /* filesets to restore from this tape */
};

/* each tape has a bunch of things to do; hangs off of a struct bc_tapeList.  Kept sorted so that we can read the tape once to do everything we need to do.  */
struct bc_tapeItem {
    struct bc_tapeItem *next;
    char *volumeName;			    /* 1st: fileset to restore */
    long position;			    /* file slot on this tape */
    long startPos, endPos;		    /* dump positions */
    afs_hyper_t oid;			    /* id of the fileset */
    long flags;				    /* flags describing tape item */
    long level;				    /* level # of the containing dump (0 == top level) */
    afsNetAddr hostaddr;                       /* Host to restore to */
    long partition; 
    struct bc_tapeList *back;		    /* back ptr */
};

/* strip .backup from the end of a name */
static StripBackup(aname)
  register char *aname; {
    register int i;
    
    if (!BackupName(aname)) return -1;	/* doesn't end properly */
    i = strlen(aname);
    /* don't need to check length; previous function checked it */
    aname[i-7] = 0;	/* blam! */
    return 0;
}

static BackupName(aname)
char *aname; {
    register int j;

    j = strlen(aname);
    if (j < 8 || strcmp(".backup", aname+j-7) != 0)
	return 0;
    else return 1;
}

static SortDump(avname, server, partition, adumpDescr, adumps, alevel)
char *avname;
afsNetAddr *server; 
long partition; 
register int alevel;
struct bc_dumpList *adumps[];
struct budb_dumpEntry *adumpDescr; {
    register struct bc_dumpList *adl;

    /* see if there's already an entry in the right list with the same 
       fileset name and	dump id.  If there is, we're done, otherwise 
       add this guy in. */

    for(adl=adumps[alevel]; adl; adl=adl->next) {
	if (strcmp(adl->volumeName, avname) == 0 && adumpDescr->id == adl->dumpID)
 	    return 0;	/* already in list */
    }
    adl = (struct bc_dumpList *) osi_Alloc(sizeof(struct bc_dumpList));
    bzero((char *)adl, sizeof(*adl));
    adl->next = adumps[alevel];
    adumps[alevel] = adl;
    adl->dumpID = adumpDescr->id;
    adl->level = alevel;
    adl->partition = partition;
    adl->server = *server;
    adl->volumeName = (char *) osi_Alloc(strlen(avname)+1);
    strcpy(adl->volumeName, avname);
    return 0;
}

/* SortTape
 *
 * entry:
 *      tdl - pointer to dumpList entry 
 *	aentry - fileset entry for the above. Why does it take the two
 *		previous parameters? Is that information not in the
 *		fileset entry?
 *	alist - tape list to which the new information should be added
 *	alevel - level of the dump
 */
static
SortTape(tdl, aentry, alist, alevel)
     struct bc_dumpList *tdl;
     struct budb_volumeEntry *aentry;
     struct bc_tapeList **alist;
     long alevel;
{
    register struct bc_tapeList *tlist;
    struct bc_tapeList **plist;
    register struct bc_tapeItem *titem;
    struct budb_tapeEntry tapeEntry;
    long code;
    int found;
    struct bc_tapeItem *litem;

    code = bcdb_FindTape(tdl->dumpID, aentry->tape, &tapeEntry);
    if (code) {
	com_err(whoami,code,
		"Restore: failed to find tape %s", aentry->tape);
	return(code);
    }

    /* get plist to the end of the tape list */
    found = 0;
    for(plist=alist, tlist = *plist; tlist; plist = &tlist->next, tlist = *plist) {
	/* if we find a later tape, stop now with plist pointing to guy to zap */
	if (tlist->dumpID > tdl->dumpID) break;
	if (tlist->dumpID == tdl->dumpID) {
	    if (tlist->tapeNumber == tapeEntry.seq) {
		found = 1;
		break;
	    }
	    if (tlist->tapeNumber > tapeEntry.seq)
		break;
	}
    }

    /* now, we may have found the right entry already */
    if (!found) {
	tlist = (struct bc_tapeList *) osi_Alloc(sizeof(struct bc_tapeList));
	if (tlist == (struct bc_tapeList *)NULL)
	  return(BC_NOMEM);
	bzero((char *)tlist, sizeof(struct bc_tapeList));
	tlist->tapeName = (char *) osi_Alloc(strlen((const char *)aentry->tape)+1);
	strcpy(tlist->tapeName, (char *)aentry->tape);
	tlist->dumpID = tdl->dumpID;
	tlist->tapeNumber = tapeEntry.seq;
	tlist->next = *plist;
	*plist = tlist;
    }

    /* now sort the particular entry into a slot field by position on tape */
    litem = (struct bc_tapeItem *) 0;
    for(titem = tlist->restoreList; titem; litem = titem, titem=titem->next) {
	if(aentry->position == titem->position) {
	    /* duplicate */
	    return 0;
	}
	if (aentry->position < titem->position)
	    break;
    }
    /* if here and titem, we found someone before which we must go.  Otherwise
	we end up going at the end.  The vbl litem, if it is non-null, tells us before whom we go */
    titem = (struct bc_tapeItem *) osi_Alloc(sizeof(struct bc_tapeItem));
    if (titem == (struct bc_tapeItem *)NULL)
      return(BC_NOMEM);
    bzero((char *)titem, sizeof(*titem));
    if (litem) {
	titem->next = litem->next;
	litem->next = titem;
    }
    else {
	titem->next = tlist->restoreList;
	tlist->restoreList = titem;
    }
    titem->volumeName = (char *) osi_Alloc(strlen(tdl->volumeName)+1);
    strcpy(titem->volumeName, tdl->volumeName);
    titem->position = aentry->position;
    titem->startPos = aentry->startByte;
    titem->endPos = titem->startPos + aentry->nBytes;
    titem->level = alevel;
    titem->hostaddr = tdl->server;
    titem->partition = tdl->partition;
    titem->oid = aentry->id;
    titem->back = tlist;
    return 0;
}

/* 
 * bc_DoRestore
 *	routine calls BUTC_PerformRestore with an array of restore descriptors,
 *     	each descriptor holding information about a fileset to be restored.
 *	This is called by both restoreft and restoredisk.
 */

bc_DoRestore(tconn, tdump)
  rpc_binding_handle_t tconn;
  struct bc_dumpTask *tdump;
{
    struct bc_dumpList *dumps[BC_MAXLEVELS], *tdumps[BC_MAXLEVELS], *tdl;
    budb_dumpEntry *dumpDescr, tdumpDescr;
    budb_volumeEntry *tapeEntries;
    budb_volumeList *tapeList;
    struct bc_dumpList *td, *ndump;
    struct bc_volumeDump *tvol;
    long code;
    long st = 0;
    struct tc_restoreArray *rpcArray=(struct tc_restoreArray *)NULL;	
    struct bc_tapeList *tapes;
    struct bc_tapeList *ttape, *ntape;
    register long i;
    struct bc_tapeItem *titem, *nitem;
    char *tname;
    long j, last, next, nentries;
    butcNameString_t dsName;

    /* tname string holds the fileset name for sorting purposes. Lets allocate
     * sufficient memory for holding the largest possible fileset names and
     * their extensions.
     */
    tname = (char *)osi_Alloc(BU_MAXNAMELEN+1);
    if (tname == (char *)NULL)
      return (BC_NOMEM);
      
    /* don't have enough thread room to get away with this otherwise */
    dumpDescr = (struct budb_dumpEntry *) 
      osi_Alloc(BC_MAXLEVELS *	sizeof(struct budb_dumpEntry));
    if (dumpDescr == (struct budb_dumpEntry *)NULL)
      return (BC_NOMEM);
    
    for(i=0;i<BC_MAXLEVELS;i++)	{   /* clear out dump arrays */
	dumps[i] = (struct bc_dumpList *) 0;
    }
    for (tvol = tdump->volumes; tvol; tvol = tvol->next) {
	/* for each dump, contact dump dbase and find the best dump to use for
	 * this fileset */
	for(i=0;i<BC_MAXLEVELS;i++)	{   /* clear out tdump arrays */
	    tdumps[i] = (struct bc_dumpList *) 0;
	}
	i = 0;
	/* we will try looking for foo and foo.backup, and will use the best
	 * dump we've got (in case local facilities group switched from
	 * one dump scheme to another
	 */

	code = bcdb_FindDump(tvol->name, tdump->fromDate, &dumpDescr[i]);	/* look for fileset */
	if (!BackupName(tvol->name)) {	/* look for backup, too */
	    /* fileset doesn't end ".backup", try adding it */
	    strcpy(tname, tvol->name);
	    strcat(tname, ".backup");
	    j = bcdb_FindDump(tname, tdump->fromDate, &tdumpDescr);
	}
	else j = -1;   /* already backup? pretend that can't find other dump */
	if (code == 0 && j == 0) {
	    /* found two dumps, use latest one by faking other dude's return
	     * code to look as if it failed
	     */
	    if (tdumpDescr.created > dumpDescr[i].created)
		code = -1;	/* use .backup: make normal dump look bad */
	    else
		j = -1;		/* use original name */
	}
	if (j == 0) {
	    /* restore .backup fileset */
	    dumpDescr[i] = tdumpDescr;
	}
	else if (code == 0) {
	    /* original is fine, move its name to tname */
	    strcpy(tname, tvol->name);
	}
	else {
	    /* neither fileset dump exists */
	    com_err(whoami, code,
	            "Can't find any dump for fileset %s", tvol->name);
	    continue;
	}
	/* here, fileset name is in tname, and dump info is in dumpDescr[i] */

	/* next, sort this dump into our work list, and go up the dump tree */
	i++;
	while(dumpDescr[i-1].parent) {
	    code = bcdb_FindDumpByID(dumpDescr[i-1].parent, &dumpDescr[i]);
	    if (code) {
		com_err(whoami, code,
	                "Could not find parent dump for fileset %s", tvol->name);
		break;
	    }
	    i++;
	}
	/* i is first free slot; i-j is level # of dump in j'th slot */
	for(j=0;j<i;j++) {
	    SortDump(tname, &(tvol->server), tvol->partition, &dumpDescr[j], dumps, i-j-1);
	}
    }

    /* We could free tname here, but we choose to free it at the end of the 
       function, in case someone thinks of using it later */

    /* at this point we know which dumps we need, and which filesets from those
     * dumps.  We next retrieve all of the dump information for each fileset
     * dump and sort it onto our tape lists.
     * We do this starting at the level-0 dumps and progressing onwards, so
     * that we overwrite old data with new. 
     */
    tapes = (struct bc_tapeList *) 0;
    for(i=0;i<BC_MAXLEVELS;i++) {
	for(tdl = dumps[i]; tdl; tdl = tdl->next) {
	    /* look at each dump, getting the tape sections we need */
	    last = 0;
	    while(1) {
		code = bcdb_FindVolumes(tdl->dumpID, tdl->volumeName,
					last, &next,
					&tapeList);
		if (code) {
		    /* can't find the tape, print warning and continue */
		    com_err(whoami, code,
			    "Can't find tapes for fileset %s (in dump %d)",
			    tdl->volumeName, tdl->dumpID);
		    code = -1;
		    tapeList = (budb_volumeList *)NULL;
		    goto done;
		}
		nentries = tapeList->budb_volumeList_len;
		tapeEntries = tapeList->budb_volumeList_val;
		/* iterate over all returned tape items, sorting them in tape
		 * position order.  For now, also notice if we have two dumps
		 * that overlap, since that'd be a problem with the current 
		 * volserver
		 */
		for(j=0;j<nentries;j++)
		  (void)SortTape(tdl, &tapeEntries[j],
			   &tapes, i);
		last = next;
		(void)free((char *)tapeList);
		if (last < 0) 
		  break;	/* if < 0, we're done */
	    } /*w*/
	}
    }

    /* make restore requests */
    nentries = 0;
    for(ttape = tapes; ttape; ttape=ttape->next) {
	for(titem = ttape->restoreList; titem; titem=titem->next)
	    nentries++;
    }
    /* now we know how many to restore */
    rpcArray = (struct tc_restoreArray *)osi_Alloc(sizeof(struct tc_restoreArray) + ((nentries-1)*sizeof(struct tc_restoreDesc)));
    if (rpcArray == (struct tc_restoreArray *)NULL) {
      code = BC_NOMEM;
      goto done;
    }
    rpcArray->tc_restoreArray_len=0;
    bzero((char *)rpcArray->tc_restoreArray_val,
	  nentries*sizeof(struct tc_restoreDesc));
    i=0;
    for(ttape=tapes; ttape; ttape=ttape->next) {
      for(titem = ttape->restoreList; titem; titem=titem->next) {
	rpcArray->tc_restoreArray_val[i].origVid = titem->oid;
	AFS_hzero(rpcArray->tc_restoreArray_val[i].vid);
	rpcArray->tc_restoreArray_val[i].partition = titem->partition;
	rpcArray->tc_restoreArray_val[i].frag = 1;
	rpcArray->tc_restoreArray_val[i].position = titem->position;
	rpcArray->tc_restoreArray_val[i].realDumpId = ttape->dumpID;
	rpcArray->tc_restoreArray_val[i].hostAddr = titem->hostaddr;
	strcpy((char *)rpcArray->tc_restoreArray_val[i].tapeName, ttape->tapeName);
	strcpy((char *)rpcArray->tc_restoreArray_val[i].oldName, titem->volumeName);
	strcpy((char *)rpcArray->tc_restoreArray_val[i].newName, titem->volumeName);
	if (BackupName(titem->volumeName)) {
	  /* if we're restoring a backup fileset, modify the new fileset's
	     name to excise ".backup" */
	  StripBackup(rpcArray->tc_restoreArray_val[i].newName);
	}
	if (tdump->newExt)
	  strcat((char *)rpcArray->tc_restoreArray_val[i].newName,
		 tdump->newExt);
	if (titem->level > 0) {
	  /* link up the restore nodes which correspond to the same
	   * fileset
	   */
	  rpcArray->tc_restoreArray_val[i].flags = TC_RESTORE_INCR;
	  /* incr restore node follows */
	}
	else    /* full dump */
	  rpcArray->tc_restoreArray_val[i].flags = TC_RESTORE_CREATE;
	if(tdump->dontExecute) {

	  if (tdump->dontExecute == 2) { /* "restoreftfa" */
	    char *hostname;
	    struct sockaddr_in haddr;
	    u_long did;

	    haddr = *((struct sockaddr_in *)
		      &rpcArray->tc_restoreArray_val[i].hostAddr);

	    hostname = hostutil_GetNameByINet(haddr.sin_addr);
	    if (hostname) 
	      printf("%s", hostname);
	    else
	      printf("<Host Not Found>");

	    printf(" %s", getAggrName(rpcArray->tc_restoreArray_val[i].hostAddr, 
				      rpcArray->tc_restoreArray_val[i].partition,
				      0));
	    printf(" %s # as %s; tape %s; pos %ld;", 
		   rpcArray->tc_restoreArray_val[i].oldName, 
		   rpcArray->tc_restoreArray_val[i].newName, 
		   rpcArray->tc_restoreArray_val[i].tapeName, 
		   (rpcArray->tc_restoreArray_val[i].position==0)?0:
		   (rpcArray->tc_restoreArray_val[i].position-1));
	    printf(" %s\n", ctime((time_t *) &rpcArray->tc_restoreArray_val[i].realDumpId));

	  } else {
	      dce_printf(bak_s_restore_fileset,
			 rpcArray->tc_restoreArray_val[i].oldName,
			 AFS_hgetlo(rpcArray->tc_restoreArray_val[i].origVid),
			 rpcArray->tc_restoreArray_val[i].tapeName, 
			 rpcArray->tc_restoreArray_val[i].realDumpId, 
			 (rpcArray->tc_restoreArray_val[i].position==0)?0:
			 (rpcArray->tc_restoreArray_val[i].position-1));
	      if (strcmp((char *)rpcArray->tc_restoreArray_val[i].oldName, 
			 (char *)rpcArray->tc_restoreArray_val[i].newName) != 0)
		  dce_printf(bak_s_as_newname,
			     rpcArray->tc_restoreArray_val[i].newName);
	      dce_printf(bak_s_dot_newline);
	  }
	}
	rpcArray->tc_restoreArray_len++; i++;
      }
    }
    if(tdump->dontExecute){
	code = 0;
	goto done;
    }
    strcpy((char *)dsName, "DumpSetName");
    /* amazingly enough, we're now ready to make the call */
    code = BUTC_PerformRestore(tconn, &dsName,
			       rpcArray, &(tdump->dumpID));
    if (code)
	com_err(whoami, code, "Failed to start restore");

  done:
    /* free dump structures */
    (void)osi_Free(tname, BU_MAXNAMELEN+1);
    if (rpcArray)
      (void)osi_Free(rpcArray, 
		     sizeof(struct tc_restoreArray) + ((nentries-1)*sizeof(struct tc_restoreDesc)));

    for(i=0;i<BC_MAXLEVELS;i++) {
	for(td = dumps[i]; td; td=ndump) {
	    ndump = td->next;
	    osi_Free(td->volumeName, strlen(td->volumeName)+1);
	    osi_Free(td, sizeof(struct bc_dumpList));
	}
    }
    /* free tape lists and items */
    for(ttape = tapes; ttape; ttape=ntape) {
	ntape = ttape->next;
	for(titem = ttape->restoreList; titem; titem = nitem) {
	    nitem = titem->next;
	    osi_Free(titem->volumeName, strlen(titem->volumeName)+1);
	    osi_Free(titem, sizeof(struct bc_tapeItem));
	}
	osi_Free(ttape->tapeName, strlen(ttape->tapeName)+1);
	osi_Free(ttape, sizeof(struct bc_tapeList));
    }
    /* free local-like things we alloacted to save stack space */
    osi_Free(dumpDescr, BC_MAXLEVELS * sizeof(budb_dumpEntry));
    return code;
}

bctc_RestoreVolumes() {
    dce_svc_printf(BAK_S_IN_BUTC_RESTOREVOLUMES_MSG);
    return 0;
}

/*
 * bc_Restorer
 *	handle restores by calling bc_DoRestore, and prepare a status
 *	node for it.
 */
bc_Restorer(ai)
pthread_addr_t ai; 

{
    volatile rpc_binding_handle_t tconn = NULL;
    volatile register long code;
    struct bc_dumpTask * volatile tdump;
    volatile statusP statusPtr;
    unsigned long aindex = (unsigned long) ai;
    extern statusP createStatusNode();
    
    /* setup status monitor node */
    statusPtr = createStatusNode(BC_REFCOUNT);
    tdump = &bc_dumpTasks[aindex];

    if (!tdump->dontExecute) {
	 tconn = bc_GetConn(tdump->config,tdump->portOffset);
	 if ( tconn == (rpc_binding_handle_t)NULL )  {
	      code = BC_NOCONN;
	      com_err(whoami, code, 
		      "; Unable to connect to tape coordinator at port %d",
		      tdump->portOffset);
	      goto finish;
	 }

	 code = CheckTCVersion(tconn);
	 if ( code ) {
	      com_err(whoami, code, 
		      "; Unable to contact tape coordinator at port %d",
		      tdump->portOffset);
	      goto finish;
	 }
    }

    /* make call to start restore */
    code = bc_DoRestore(tconn, tdump);
    if (code==0 && !tdump->dontExecute) {
	lock_ObtainWrite(&statusQueueLock);
	TRY {
	  statusPtr->taskId = tdump->dumpID;
	  statusPtr->port = tdump->portOffset;
	  statusPtr->jobNumber =  bc_jobNumber();
	  statusPtr->opType = TCOP_RESTORE;
	  sprintf(statusPtr->taskName, "Restore");
	} FINALLY {
	  lock_ReleaseWrite(&statusQueueLock);
	} ENDTRY
	clearStatus(tdump->dumpID, STARTING);		/* ok to examine */
	lock_ObtainWrite(&statusQueueLock);
	TRY {
	  deleteStatusNode(statusPtr);			/* drop the refCount */
	} FINALLY {
	  lock_ReleaseWrite(&statusQueueLock);
	} ENDTRY
    }
finish:
    /* now free up resources used */
    tdump->dumpID = 0;
    if (tdump->dumpName) 
      (void)osi_Free(tdump->dumpName, strlen(tdump->dumpName)+1);
    if (tdump->newExt) 
      (void)osi_Free(tdump->newExt, strlen(tdump->newExt)+1);
    if (tdump->volSetName) 
      (void)osi_Free(tdump->volSetName, strlen(tdump->volSetName)+1);
    if (tconn)
	 bc_FreeConn(tdump->config, tconn);
    tdump->flags &= ~BC_DI_INUSE;
    if (code || tdump->dontExecute) {
	zapStatusNode(statusPtr);
	tdump->flags |= BC_DI_ERROR;
    }
    return 0;
}

