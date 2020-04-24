/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1991, 1995, 1996 Transarc Corporation
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: ubik_db_if.c,v $
 * Revision 1.1.19.1  1996/10/02  16:59:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:01  damon]
 *
 * $EndLog$
 */
/*
 * ubik_db_if.c -- Interface and supporting routines for the bak system's
 *                 Ubik database
 */
#include <dcedfs/stds.h>  /* include appropriate standards file */
#include <dcedfs/param.h>
#include <sys/types.h>
#include <sys/param.h>                  /* for hostnames etc */
#include <com_err.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/budb_errs.h>
#include <bc.h>
#include <error_macros.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

/* -------------------------------------
 * Globals
 * -------------------------------------
 */

struct udbHandleS udbHandle;
extern long budbInitialize();

/* -------------------------------------
 *  supporting routines
 * -------------------------------------
 */

/*
 * -------------------------------------
 * interface routines (alphabetic)
 * -------------------------------------
 */

bcdb_AddVolume(veptr)
    struct budb_volumeEntry *veptr;
{
    long code;
    code = bcdb_AddVolumes(1,veptr);
    return(code);
}

bcdb_AddVolumes(vcount,varray)
     long vcount;
     struct budb_volumeEntry *varray;
{
    long code;
    long index;

    code = ubik_PVCall( BUDB_AddVolumes,  udbHandle.uh_client, UF_SYNC_SITE,
			 2, /* provider version number */
			 vcount, varray);
    if ( code == UDOWNLVL )
    {
	for ( index = 0 ; index < vcount ; index++ )
	{
	    code = ubik_Call( BUDB_AddVolume, udbHandle.uh_client, UF_SYNC_SITE,
			      &varray[index]);
	if ( code ) break;
	}
    }
    return(code);
}



bcdb_CreateDump(deptr)
     register struct budb_dumpEntry *deptr;
{
    long code;
    code = ubik_Call( BUDB_CreateDump,  udbHandle.uh_client, UF_SYNC_SITE,
		     deptr);
    return(code);
}

bcdb_deleteDumpById(dumpID)
{
    return( ubik_Call(BUDB_DeleteDump,  udbHandle.uh_client, UF_SYNC_SITE,
		      dumpID) );
}


bcdb_DeleteVDP(dumpSetName, dumpPath, dumpID)
     char *dumpSetName;
     char *dumpPath;
     long dumpID;
{
    long code;
    bakNameString_t ndumpSetName, ndumpPath;

    strcpy((char *)ndumpSetName, dumpSetName);
    strcpy((char *)ndumpPath, dumpPath);
    code = ubik_Call( BUDB_DeleteVDP,  udbHandle.uh_client, UF_SYNC_SITE,
		     &ndumpSetName, &ndumpPath, dumpID);
    return(code);
}

/* bcdb_FindClone
 *      Returns the clone time of a volume by going up the parent chain.
 *      If no clone time is found, a clone time of 0 is returned, forcing
 *      a full dump.
 * entry:
 *      dumpID - of the first dump to examine.
 *      volName - name of the volume for whom a clone time is required
 *      clonetime - ptr to vbl for returning result
 * exit:
 *      0 - clonetime set appropriately
 */

long
bcdb_FindClone(dumpID, volName, clonetimep)
     long dumpID;
     char *volName;
     Date *clonetimep;
{
    long code;
    unsigned long cloneSpare;
    bakNameString_t nvolName;

    strcpy((char *)nvolName, volName);
    code = ubik_Call( BUDB_FindClone,  udbHandle.uh_client, 0,
		     dumpID, &nvolName, clonetimep, &cloneSpare);
    return(code);
}

/* bcdb_FindDump
 *      scan entire database for latest volume dump before adate.  Optimize
 *      further by reading only the first line of the dump and if it is older
 *      than the oldest acceptable dump we've found so far, we don't bother
 *      scanning the dump file we've just opened
 *
 *      Used by restore code when restoring a user requested volume(s)
 * entry:
 *      volumeName - name of volume to match on
 *      beforeDate - look for dumps older than this date
 * exit:
 *      deptr - desciptor of most recent dump
 * notes:
 *      should be able to implement this in a single call rather than
 *      the current multiple bcdb_ call algorithm.
 */

bcdb_FindDump(volumeName, beforeDate, deptr)
     char *volumeName;
     unsigned long beforeDate;
     struct budb_dumpEntry *deptr;
{
    long code;
    unsigned long dateSpare;
    bakNameString_t nvolumeName;

    strcpy((char *)nvolumeName, volumeName);
    code = ubik_Call( BUDB_FindDump,  udbHandle.uh_client, 0,
		     &nvolumeName, beforeDate, dateSpare, deptr);
    return(code);
}

/* bcdb_FindDumpByID
 *	find a dump by id. Currently insists on a single return value.
 * entry:
 *	dumpID - id to lookup
 * exit:
 */
bcdb_FindDumpByID(dumpID, deptr)
     long dumpID;
     register struct budb_dumpEntry *deptr;
{
    register long code;
    long	nextindex;
    long	dbTime;
    budb_dumpList *dl;
    bakNameString_t emptyName;

    strcpy((char *)emptyName, "");
    /* outline algorithm */
    code = ubik_Call (BUDB_GetDumps,  udbHandle.uh_client, 0,
		      CUR_BUDB_VERSION,
		      BUDB_OP_DUMPID,
		      &emptyName,		/* no name */
		      dumpID,			/* start */
		      0,			/* end */
		      0,			/* index */
		      &nextindex,
		      &dbTime,
		      &dl);

    if ( (code != 0)
    ||   (dl->budb_dumpList_len != 1)		/* single retn val expected */
       )  {
      if ( code == 0 ) {
	code = 1;			/* multiple id's */
	(void)free(dl);
      }
      goto error;
    }

    *deptr = dl->budb_dumpList_val[0];

exit:
    /* Free the storage allocated by the rpc runtime before leaving */
    (void)free(dl);
    return(code);

error:
    bzero((char *)deptr, sizeof(budb_dumpEntry));
    return(code);
}

/* bcdb_FindLastVolClone
 *      Returns the clone time, from the most recent dump of volName, when
 *      dumped in the volume set volSetName, with dump schedule dumpName.
 *      The clone time can be used to check if the volume has been correctly
 *      re-cloned, and also is used as the time from which to do the current
 *      incremental dump.
 * entry:
 *      volSetName - name of volume set
 *      dumpName - full path of dump node
 *      volName - name of volume for whom a clone time is required
 *      clonetime - ptr to vbl for result
 * exit:
 *      0 - clonetime set appropriately
 * notes:
 *      used only for warning generation. Suggest that this be omitted.
 */

long
bcdb_FindLastVolClone(volSetName, dumpName, volName, clonetime)
     char *volSetName;
     char *dumpName;
     char *volName;
     afsTimeval *clonetime;
{
    /* server notes
       search by dumpName
       match on volumeset and dump path
       search for the volume name
     */
    return(0);
}

/* bcdb_FindLatestDump
 *      find the latest dump with volume set component avname and the
 *      specified dump pathname. Used to find a dump, relative to which an
 *      incremental dump can be done. Defines the parent <-> child relations
 *      for restores.
 * entry:
 *      avname: volume set name
 *      dumpPath: full path of dump node
 * exit:
 *      0:  adentry: dump entry structure filled in.
 * Notes for 4.0:
 *      Need to store volumeset name in dump in order to implement this.
 *      Need new routine since params are two strings
 */

bcdb_FindLatestDump(volSetName, dumpPath, deptr)
     char *volSetName;
     char *dumpPath;
     struct budb_dumpEntry *deptr;
{
    long code;
    bakNameString_t nvolSetName, ndumpPath;

    strcpy((char *)nvolSetName, volSetName);
    strcpy((char *)ndumpPath, dumpPath);
    code = ubik_Call( BUDB_FindLatestDump,  udbHandle.uh_client, 0,
		     &nvolSetName, &ndumpPath, deptr);
    return(code);
}


/* bcdb_FindTape
 *	find a tape
 * entry:
 *	dumpid: dump id to which tape belongs
 *	tapeName: name of tape
 */

bcdb_FindTape(dumpid, tapeName, teptr)
     long dumpid;
     char *tapeName;
     struct budb_tapeEntry *teptr;
{
    budb_tapeList *tl;
    long next;
    long dbTime;
    long code = 0;
    bakNameString_t ntapeName;

    bzero((char *)teptr, sizeof(*teptr));
    strcpy((char *)ntapeName, tapeName);
    code = ubik_Call( BUDB_GetTapes, udbHandle.uh_client, 0,
		     CUR_BUDB_VERSION,
		     BUDB_OP_TAPENAME|BUDB_OP_DUMPID,
		     &ntapeName, dumpid, 0, 0, &next, &dbTime, &tl);

    if ( code ) {
      (void)free(tl);
      ERROR(code);
    }
    if ( tl->budb_tapeList_len != 1 )
    {
	/* expecting a single descriptor */
	ERROR(BC_NOTUNIQUE);
    }

    *teptr = tl->budb_tapeList_val[0];

    /* Free the storage allocated by the rpc runtime before leaving */
    (void)free(tl);

error_exit:
    return(code);
}

/* bcdb_FindVolumes
 * notes:
 *	- this is part of dblookup. The existing semantics will not work since
 *	they do lookups based on dump id.
 *	- in the restore code, it uses this to extract information about
 *	the volume. Need current semantics. Could filter the output, selecting
 *	on the dumpid.
 *	- Suggest that the lookup be based on volume name only, with optional
 *	match on backup, and readonly volumes.
 *	- Further, need to check if the volume structure returns enough
 *	information
 */

long
bcdb_FindVolumes(dumpID, volumeName, last, next, vl)
     long dumpID;
     char *volumeName;
     long last;
     long *next;
     budb_volumeList **vl;
{
    long dbTime;
    bakNameString_t nvolumeName;

    strcpy((char *)nvolumeName, volumeName);
    return(ubik_Call (BUDB_GetVolumes,  udbHandle.uh_client, 0,
                      CUR_BUDB_VERSION,
                      BUDB_OP_VOLUMENAME|BUDB_OP_DUMPID,
                      &nvolumeName,               /* name */
                      dumpID,			/* start */
                      0,                        /* end */
		      last,			/* index */
                      next,			/* nextindex */
                      &dbTime,
                      vl));
}


bcdb_FinishDump(deptr)
     register struct budb_dumpEntry *deptr;
{
    long code;
    code = ubik_Call (BUDB_FinishDump,  udbHandle.uh_client, UF_SYNC_SITE,
		     deptr);
    return(code);
}

bcdb_FinishTape(teptr)
     register struct budb_tapeEntry *teptr;
{
    long code;
    code = ubik_Call (BUDB_FinishTape,  udbHandle.uh_client, UF_SYNC_SITE,
		     teptr);
    return(code);

}

/* bcdb_LookupVolumes
 */

long
bcdb_LookupVolume(volumeName, last, next, vl)
     char *volumeName;
     long last;
     long *next;
     budb_volumeList **vl;
{
    long dbTime;
    bakNameString_t nvolumeName;

    strcpy((char *)nvolumeName, volumeName);
    return(ubik_Call (BUDB_GetVolumes,  udbHandle.uh_client, 0,
                      CUR_BUDB_VERSION,
                      BUDB_OP_VOLUMENAME,
                      &nvolumeName,             /* name */
                      0,			/* start */
                      0,                        /* end */
		      last,			/* index */
                      next,			/* nextindex */
                      &dbTime,
                      vl));
}


bcdb_UseTape(teptr, newFlag)
     register struct budb_tapeEntry *teptr;
     long *newFlag;
{
    long code;
    code = ubik_Call (BUDB_UseTape,  udbHandle.uh_client, UF_SYNC_SITE,
		     teptr, newFlag);
    return(code);
}


/* ---- text configuration handling routines ----
 *
 * notes:
 *	use of a filename is temporary, for debug/test. The final version
 *	should pass in/out a fid for an unlinked, open file to prevent
 *	tampering with the files contents
 */

/* bcdb_GetTextFile
 *	extract the specified textType and put it in a temporary, local
 *	file.
 * entry:
 *	ctPtr - ptr to client structure with all the required information
 */

bcdb_GetTextFile(ctPtr)
     register udbClientTextP ctPtr;
{
    char filename[128];
    long bufferSize;
    long offset, nextOffset, chunkSize;
    charListT *charList;
    long code = 0;

    extern struct udbHandleS udbHandle;
    struct udbHandleS *uhptr = &udbHandle;

    /* allocate a buffer */
    bufferSize = BUDB_MAX_CHAR_LIST;
    charList = (charListT *)osi_Alloc(sizeof(charListT));
    if (charList == (charListT *)NULL)
      return (BC_NOMEM);
    /* check params and cleanup any previous state */
    if ( ctPtr->lockHandle == 0 )
	ERROR(BUDB_INTERNALERROR);
    if ( ctPtr->textStream != NULL )
	fclose(ctPtr->textStream);

    strcpy(filename, "/tmp/bu_XXXXXX");
    mktemp(filename);
    ctPtr->textStream = fopen(filename, "w+");
    if ( ctPtr->textStream == NULL )
	ERROR(BUDB_INTERNALERROR);
    /* make the file invisible to others */
    code = unlink(filename);
    if ( code )
	ERROR(errno);

    offset = 0;
    nextOffset = 0;
    ctPtr->textSize = 0;
    while ( nextOffset != -1 )
    {
	offset = nextOffset;
	charList->charListT_len=0; /* optimization */
	code = ubik_Call(BUDB_GetText,  uhptr->uh_client, 0,
			 ctPtr->lockHandle, ctPtr->textType, bufferSize,
			 offset, &nextOffset, charList);
	if ( code )
	  ERROR(code);

	code = fwrite((void *)charList->charListT_val, sizeof(char),
		      charList->charListT_len, ctPtr->textStream);
	if ( ferror(ctPtr->textStream) )
	  ERROR(BUDB_INTERNALERROR);

        ctPtr->textSize += charList->charListT_len;
    }

    /* get text version */
    code = ubik_Call(BUDB_GetTextVersion, uhptr->uh_client, 0,
		     ctPtr->textType, &ctPtr->textVersion);
    if ( code )
	ERROR(code);

normal_exit:
    fflush(ctPtr->textStream);				/* debug */
    osi_Free(charList, sizeof(charListT));
    /* exit, leaving the configuration text file open */
    return(code);

error_exit:
    if ( ctPtr->textStream != NULL )
    {
	fclose(ctPtr->textStream);
	ctPtr->textStream = NULL;
    }
    goto normal_exit;
}


/* bcdb_SaveTextFile
 *	save the text file in ubik database
 * entry:
 *	textType - identifies type of configuration file
 *	filename - where to get the text from
 */

bcdb_SaveTextFile(ctPtr)
     udbClientTextP ctPtr;
{
    long bufferSize;
    long offset, chunkSize, fileSize;
    charListT *charList;
    long code;

    extern struct udbHandleS udbHandle;
    struct udbHandleS *uhptr = &udbHandle;

    long filesize();

    /* allocate a buffer */
    bufferSize = BUDB_MAX_CHAR_LIST;
    charList = (charListT *)osi_Alloc(sizeof(charListT));
    if (charList == (charListT *)NULL)
      return (BC_NOMEM);
    if ( ctPtr->textStream == NULL )
	ERROR(BUDB_INTERNALERROR);
    rewind(ctPtr->textStream);
    fileSize = (long) filesize(ctPtr->textStream);
    rewind(ctPtr->textStream);

    /* special case empty files */
    if ( fileSize == 0 ) {
	charList->charListT_len = 0;
	code = ubik_Call(BUDB_SaveText,  uhptr->uh_client, UF_SYNC_SITE,
			 ctPtr->lockHandle, ctPtr->textType, 0,
			 BUDB_TEXT_COMPLETE, charList);
	goto error_exit;
    }

    offset = 0;
    while ( fileSize != 0 )
    {
	chunkSize = MIN(fileSize, bufferSize);
	code = fread((void *)charList->charListT_val, sizeof(char), chunkSize,
		     ctPtr->textStream);
	if ( code != chunkSize )
		dce_svc_printf(BAK_S_CODE_EQUALS_MSG, code );
	if ( ferror(ctPtr->textStream ) )
		ERROR(BUDB_INTERNALERROR);

	charList->charListT_len = chunkSize;
	code = ubik_Call(BUDB_SaveText,  uhptr->uh_client, UF_SYNC_SITE,
			 ctPtr->lockHandle, ctPtr->textType, offset,
			 (chunkSize == fileSize) ? BUDB_TEXT_COMPLETE: 0,
			 charList);
	if ( code )
		ERROR(code);

	fileSize -= chunkSize;
	offset += chunkSize;
    }

error_exit:
    osi_Free(charList, sizeof(charListT));
    return(code);
}

/* -------------------------------------
 * misc. support routines
 * -------------------------------------
 */

long
filesize(stream)
     FILE *stream;
{
    long offset;
    long size;

    offset = ftell(stream);
    fseek(stream, (long) 0, 2);		/* end of file */
    size = ftell(stream);
    fseek(stream, offset, 0);
    return(size);
}


/* ------------------------------------
 * misc. support routines - general text management
 * ------------------------------------
 */


/* bc_LockText
 *	locks the text described by the ctPtr
 * entry:
 *	ctptr - client text ptr
 * exit:
 *	0 - success
 *	n - fail
 */

bc_LockText(ctPtr)
     register udbClientTextP ctPtr;
{
    struct udbHandleS *uhptr = &udbHandle;
    long code;
    long timeout, quitTime;
    struct timeval curTime;
    struct timespec delayTime;

    if ( ctPtr->lockHandle != 0 )
      return BC_LOCKED;

    if ( ctPtr->textSize == 0 )
	timeout = 30;				/* be safe */
    else
	timeout = (ctPtr->textSize/50000) + 10;	/* estimate time */

    osi_gettimeofday(&curTime, 0);
    quitTime = curTime.tv_sec + timeout;	/* time we should quit */
    while (1) {
      code = ubik_Call(BUDB_GetLock,  uhptr->uh_client, UF_SYNC_SITE,
		       uhptr->uh_instanceId, ctPtr->textType, timeout,
		       &ctPtr->lockHandle);
      if ( code != BUDB_LOCKED )
	break;
      osi_gettimeofday(&curTime, 0);
      if (curTime.tv_sec > quitTime)
	break;
      delayTime.tv_sec = 5;
      delayTime.tv_nsec = 0;
      pthread_delay_np(&delayTime);
    }
    if (code == 0)
      return(0);
    else {
      /* cleanup */
      ctPtr->lockHandle = 0;
      return(code);
    }
}

/* bc_UnlockText
 *	unlocks the text described by the ctPtr
 * entry:
 *	ctptr - client text ptr
 * exit:
 *	0 - success
 *	n - fail
 */

bc_UnlockText(ctPtr)
     register udbClientTextP ctPtr;
{
    struct udbHandleS *uhptr = &udbHandle;
    long code;

    if ( ctPtr->lockHandle == 0 )
	return(0);

    code = ubik_Call(BUDB_FreeLock,  uhptr->uh_client, UF_SYNC_SITE,
		     ctPtr->lockHandle);
    ctPtr->lockHandle = 0;

    /* Don't try to analyse the error. Let the lock timeout */
    return(code);
}

/* bc_CheckTextVersion
 * exit:
 *	0 - version # ok
 *	n - out of date or error
 */

bc_CheckTextVersion(ctPtr)
     register udbClientTextP ctPtr;
{
    struct udbHandleS *uhptr = &udbHandle;
    long code;
    u_long tversion;

    /* must be holding the lock */
    if ( ctPtr->lockHandle == 0 )
	return(BC_NOTLOCKED);

    code = ubik_Call(BUDB_GetTextVersion,  uhptr->uh_client, 0,
		     ctPtr->textType, &tversion);
    if ( code )
	return(code);
    if ( tversion != ctPtr->textVersion )
	return(BC_VERSIONMISMATCH);
    return(0);
}

/* -------------------------------------
 * initialization routines
 * -------------------------------------
 */
#ifdef notdef
/* udbClientInit
 *	initialize a client for the bak systems ubik database.
 */
long
udbClientInit(cellName)
     char *cellName;
{
    long serverList[UBIK_MAXSERVERS];
    char hostname[MAXHOSTNAMELEN];
    /* char *cellName = 0; */
    int noauth = 0;				/* authenticate */
    int i;
    long code = 0;

    char *whoami="udbClientInit";

    extern struct udbHandleS udbHandle;
    struct udbHandleS *uhptr = &udbHandle;
    switch ( uhptr->uh_scIndex )
    {
      case 0:
	break;

      case 2:
	break;

      case 1:
      default:
	com_err(whoami, 0, "Unsupported authentication type %d",
		uhptr->uh_scIndex );
	exit(-1);
    }
    /* establish connections to the servers. Check for failed connections? */
    uhptr->uh_serverConn[i] = 0;
    code = ubik_ClientInit(uhptr->uh_serverConn, &uhptr->uh_client);
    if ( code )
	ERROR(-1);
    code = ubik_Call(BUDB_GetInstanceId,  udbHandle.uh_client, UF_SYNC_SITE,
                     &udbHandle.uh_instanceId);
    if ( code )
    {
        com_err(whoami, code, " : can't establish instance Id");
        ERROR(-1);
    }

error_exit:
    return(code);
}
#endif /* notdef */

/* -------------------------------------
 * specialized ubik support
 * -------------------------------------
 */

/* notes
 *	1) first call with SINGLESERVER set, record the server to be used.
 *	2) subsequent calls use that server. If a failure is encountered,
 *	   the state is cleaned up and the error returned back to the caller.
 *	3) upon completion, the user must make a dummy call with
 *	   END_SINGLESERVER set, to clean up state.
 *	4) if the vanilla ubik_Call is ever modified, the END_SINGLESERVER
 *	   flag can be discarded. The first call without SINGLESERVER set
 *	   can clean up the state.
 */

struct ubikCallState
{
    long ucs_flags;			/* state flags */
    long ucs_selectedServer;		/* which server selected */
};

static struct ubikCallState uServer;

/* ubik_Call_SingleServer
 *	variant of ubik_Call. This is used by the bak system to initiate
 *	a series of calls to a single ubik server. The first call sets up
 *	process state (not recorded in ubik) that requires all further calls
 *	of that group to be made to the same server process.
 *
 *	call this instead of stub and we'll guarantee to find a host that's up.
 *	in the future, we should also put in a protocol to find the sync site
 */

long
ubik_Call_SingleServer(aproc, aclient, aflags, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16)
    register struct ubik_client *aclient;
    int (*aproc)();
    long aflags;
    long p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16;
{
    long code;
    long someCode, newHost, thisHost;
    register long i;
    register long count;
    int chaseCount;
    int pass;
    char *bindingStr, *hostAddr;
    rpc_binding_handle_t tc;

    if ( (aflags & (UF_SINGLESERVER | UF_END_SINGLESERVER)) != 0 )
    {
	if ( ((aflags & UF_SINGLESERVER) != 0 )
	&&   ((uServer.ucs_flags & UF_SINGLESERVER ) != 0)
	   )
	{

	    /* have a selected server */
	    tc = aclient->conns[uServer.ucs_selectedServer][0];

	    code = (*aproc)(tc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11,
			    p12, p13, p14, p15, p16);
	    if ( code )
	    {
		/* error. Clean up single server state */
		bzero((char *)&uServer, sizeof(uServer));
	    }
	    return(code);
	}
	else
	if ( (aflags & UF_END_SINGLESERVER) != 0 )
	{
	    bzero((char *)&uServer, sizeof(uServer));
	    return(0);
	}
    }

    someCode = UNOSERVERS;
    chaseCount = 0;
    pass = 0;
    count = 0;
    while(1)
    { /*w*/

	/* tc is the next conn to try */
	tc = aclient->conns[count][0];
	if ( tc == 0 )
	{
	    if (pass ==	0) {
		pass = 1;	/* in pass 1, we look at down hosts, too */
		count =	0;
		continue;
	    }
	    else break;			/* nothing left to try */
	}
	if (pass == 0 && (aclient->states[count] & CFLastFailed)) {
	    count++;
	    continue;	/* this guy's down, try someone else first */
	}

	code = (*aproc)(tc, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);

	/* note that getting a UNOTSYNC error code back does *not* guarantee
	 * that there is a sync site yet elected.  However, if there is a
	 * sync site out there somewhere, and you're trying an operation that
	 * requires a sync site, ubik will return UNOTSYNC, indicating the
	 * operation won't work until you find a sync site
	 */
	if (code == UNOTSYNC)
	{ /*ns*/
	    /* means that this requires a sync site to work */
	    someCode = code;	/* remember an error, if this fails */

	    /* now see if we can find the sync site host */
	    code = UBIKVOTE_GetSyncSite(tc, &newHost);
	    if (code == 0 && newHost != 0) {
		newHost = htonl(newHost); /* convert back to network order */

		/* position count at the appropriate slot in the client
		 * structure and retry. If we can't find in slot, we'll just
		 * continue through the whole list
		 */
		for(i=0;i<UBIK_MAXSERVERS;i++)
		{ /*f*/
		    rpc_binding_to_string_binding(aclient->conns[i][0],
						  (unsigned char **)&bindingStr,
						  (unsigned32 *)&code);
		    if (code != rpc_s_ok)
			return (code);
		    rpc_string_binding_parse((unsigned char *)bindingStr, NULL,
					     NULL, (unsigned char **)&hostAddr,
					     NULL, NULL, (unsigned32 *)&code);
		    if (code != rpc_s_ok)
			return(code);
		    thisHost = inet_addr(hostAddr);
		    rpc_string_free((unsigned char **)&hostAddr, (unsigned32 *)&code);
                    rpc_string_free((unsigned char **)&bindingStr, (unsigned32 *)&code);

		    if (thisHost == -1) {
			count++;
			break;
		    }

		    if (thisHost == newHost)
		    {
			/* avoid asking in a loop */
			if (chaseCount++ > 2)
			    break;
			count =	i;  /* we were told to use this one */
			break;
		    }
		} /*f*/
	    }
	    else count++;   /* not directed, keep looking for a sync site */
	    continue;
	} /*ns*/
	else if	(code == UNOQUORUM) {	/* this guy is still recovering */
	    someCode = code;
	    count++;
	    continue;
	}
	else if	(code <	0) {	/* network errors */
	    someCode = code;
	    aclient->states[count] |= CFLastFailed;
	    count++;
	    continue;
	}
	else
	{
	    /* ok, operation worked */
	    aclient->states[count] &= ~CFLastFailed;
	    /* either misc ubik code, or misc application code (incl success)
	     */

	    /* if the call succeeded, setup connection state for subsequent
	     * calls
	     */
	    if ( (code == 0)
	    &&   (aflags & UF_SINGLESERVER != 0)
	       )
	    {
		/* need to save state */
		uServer.ucs_flags = UF_SINGLESERVER;
		uServer.ucs_selectedServer = count;
	    }

	    return code;
	}
    } /*w*/
    return someCode;
}

