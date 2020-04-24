/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: vol_sets.c,v $
 * Revision 1.1.90.1  1996/10/02  16:59:04  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:03  damon]
 *
 * Revision 1.1.84.4  1994/08/25  17:34:48  mckeen
 * 	Added s12y calls
 * 	[1994/08/25  17:23:21  mckeen]
 * 
 * Revision 1.1.84.3  1994/07/13  22:27:25  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:20:49  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:53:20  mbs]
 * 
 * Revision 1.1.84.2  1994/06/09  13:50:47  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:24:07  annie]
 * 
 * Revision 1.1.84.1  1994/02/04  20:04:52  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:03  devsrc]
 * 
 * Revision 1.1.82.1  1993/12/07  17:11:52  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:46:18  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/18  19:52:01  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:26:50  cjd]
 * 
 * Revision 1.1.5.3  1993/01/13  15:24:39  shl
 * 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
 * 	  Selected comments:
 * 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
 * 	    add casts for calls to bzero.
 * 	    Remove the function dacl_SetOwnerControl.  It is not used.
 * 	Transarc delta: vijay-ot1104-bak-exit-status-not-set-correctly 1.1
 * 	  Selected comments:
 * 
 * 	    bak lsftfam was not setting the exit status correctly on failure. I made a
 * 	    pass through the bak command functions to make sure exit status was set
 * 	    properly for other commands. There were a couple of other error paths where
 * 	    the exit status was not set correctly. Two other minor things fixed as part
 * 	    of this delta
 * 	    1. error messages now don't print file/line numbers of errors. Less clutter.
 * 	    2. bak <subcommand> -h takes us into the bak interactive mode. It should
 * 	    just print usage for this subcommand and exit.
 * 	Transarc delta: vijay-ot6381-correct-handling-of-device-queue-lock 1.4
 * 	  Selected comments:
 * 
 * 	    The aim of this delta is to get bak restore to succeed. The problems in restore
 * 	    were some spurious thread exceptions and assertions that made bak dump core.
 * 	    Upon investigation it turned out that there were some locking problems in
 * 	    backup and butc. The delta cleans up locking in backup and butc.
 * 	    The delta is not ready for export. Much more needs to be done to get all the
 * 	    locking issues right. This is just a checkpoint.
 * 	    Second set of changes to get locking issues in order. This time the changes
 * 	    are mostly in bakserver. The changes introduced are
 * 	    1. Establish a global lock so that two RPCs do not interfere with each other.
 * 	    This was introduced because there are some global data structures in
 * 	    backup that might get affected. These global structures now have
 * 	    individual locks to serialize changes. The global lock in temporary, and
 * 	    will go away once we have serialized all accesses to all global data
 * 	    structures.
 * 	    2. Disable cancels in bakserver RPCs. Care is taken to not disable cancels in
 * 	    operations that can be cancelled. There is some more work that needs to be
 * 	    done in this area.
 * 	    3. Accesses to the database are controlled by memoryDB.lock. The accesses are
 * 	    by means of dbread and dbwrite. The hash tables too are protected by this
 * 	    mechanism.
 * 	    4. Changes to the backup database dump code to simplify the dump operation. In
 * 	    particular, the Unix pipe mechanism is used to synchronize reader and
 * 	    writer without need for condition variables.
 * 	    5. Get rid of any pthread_mutex and pthread_cond operations. Use afslk and
 * 	    afsos routines instead.
 * 	    6. Continue the work described by the previous revision of the delta in bak
 * 	    and butc. This should be it for changes in bak and butc.
 * 	    Fix compilation problems on the RIOS.
 * 	    This is hopefully the final revision of this delta. The fixes here are
 * 	    1. Changes to the bakserver and butc RPC interfaces.
 * 	    The RPCs that handle variable size arrays now use conformant arrays.
 * 	    This avoids allocating lots of memory on the client thread stack which
 * 	    was the cause of the exceptions in restoreft. The server allocates
 * 	    memory which is freed by the server stub on the way out. The client thread
 * 	    frees the memory allocated by the client stub.
 * 	    2. get database dump and restore to handle status correctly.
 * 	    3. Get the locking hierarchy right in bakserver, bak and butc.
 * 	    4. Other minor nits.
 * 	    There is still a problem with scantape -dbadd that has to be addressed. With
 * 	    this delta, all backup commands should work when used in a straightforward
 * 	    manner. Some error cases are still not handled properly though. Subsequent
 * 	    deltas would fix those.
 * 	[1993/01/12  17:41:30  shl]
 * 
 * Revision 1.1.5.2  1992/11/24  15:38:21  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:05:55  bolinger]
 * 
 * Revision 1.1.3.5  1992/05/05  01:38:07  mason
 * 	delta vijay-butc-compile-error 1.2
 * 	[1992/04/24  21:17:35  mason]
 * 
 * Revision 1.1.3.4  1992/01/25  20:43:50  zeliff
 * 	dfs6.3 merge, part2
 * 	[1992/01/25  19:39:34  zeliff]
 * 
 * $EndLog$
 */
/*
 * vol_sets.c -- Manage Fileset families
 */ 

#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <dcedfs/compat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dcedfs/cmd.h>
#include <com_err.h>
#include <pthread.h>
#include <dcedfs/budb_errs.h>
#include <bc.h>
#include <error_macros.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

extern struct bc_config *bc_globalConfig;
extern struct udbHandleS udbHandle;
extern char *whoami;
extern int bc_FindVolumeSet(struct bc_config *,
			    char *,
			    struct bc_volumeSet **);
int volmatch();
static long ListVolSet();
long bc_UpdateVolumeSet();

/* ------------------------------------
 * command level routines
 * ------------------------------------
 */

/* bc_AddVolEntryCmd
 * 	add a volume (or volume expression) to a volume set
 * params:
 *	parm 0 is vol set name.
 *	parm 1 is server name
 *	parm 2 is partition name
 *	parm 3 is volume regexp
 */

bc_AddVolEntryCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
  register long code;
  char *volSetName, *serverName, *partitionName, *volRegExp;
  udbClientTextP ctPtr;
  struct bc_volumeSet *tset;
  register struct bc_volumeEntry *tentry;
  extern int bc_FindVolumeSet();
    
  ctPtr = &bc_globalConfig->configText[TB_VOLUMESET];

  code = bc_LockText(ctPtr);
  if ( code == BUDB_LOCKED ) {
    com_err(whoami, code, 
          "; Unable to lock fileset family info in database, try again later");
    ERROR(code);
  }
  else if (code) {
    com_err(whoami, code, 
	    "; Unable to add fileset family entry to backup database");
    ERROR(code);
  }

  code = bc_UpdateVolumeSet(ctPtr);
  if (code) {
    com_err(whoami, code, 
	    "; Unable to sync fileset family info in local cache and backup database");
    ERROR(code);
  }
    
  volSetName = as->parms[0].items->data;
  serverName = as->parms[1].items->data;
  partitionName = as->parms[2].items->data;
  volRegExp = as->parms[3].items->data;

  /* Check if this is a duplicate */

  lock_ObtainRead(&(bc_globalConfig->vsetLock));
  code = bc_FindVolumeSet(bc_globalConfig, volSetName, &tset);
  if (code) tset = (struct bc_volumeSet *)NULL;
  if (tset) 
    for(tentry=tset->ventries; tentry; tentry=tentry->next)
      if ((strncmp(serverName, tentry->serverName, strlen(serverName)) == 0)  
	  && (strcmp(partitionName, tentry->partname) == 0) 
	  && (volmatch(volRegExp, tentry->name) == 0)) {
	dce_svc_printf(BAK_S_DUP_ENTRY_IN_FAMILY_MSG, volSetName);
	lock_ReleaseRead(&(bc_globalConfig->vsetLock));
	goto error_exit;
      }
    lock_ReleaseRead(&(bc_globalConfig->vsetLock));
    code = bc_AddVolumeItem(bc_globalConfig, volSetName, serverName,
			    partitionName, volRegExp);
    if (code)
	com_err(whoami, code, "; Unable to add fileset family entry");
    else
    {
      code = bc_SaveVolumeSet();
      if (code)
	{
	    com_err(whoami, code,
		    "; Unable to add fileset family entry to database");
	    com_err(whoami,0,"Changes are temporary - for this session only");
	}
    }

 error_exit:
  if ( ctPtr->lockHandle != 0 )
    bc_UnlockText(ctPtr);
  return(code);
}



/* bc_AddVolSetCmd
 *	create a new volume set, writing out the new volumeset
 *	file in a safe manner
 * params:
 *	name of new volume set
 */

bc_AddVolSetCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock; 
{
    /* parm 0 is vol set name */
    register long code;
    register struct cmd_item *ti;
    udbClientTextP ctPtr;

    /* lock schedules and check validity */
    ctPtr = &bc_globalConfig->configText[TB_VOLUMESET];

    code = bc_LockText(ctPtr);
    if ( code == BUDB_LOCKED ) {
      com_err(whoami, code, 
	  "; Unable to lock fileset family info in database, try again later");
      ERROR(code);
    }
    else if (code) {
      com_err(whoami, code, 
       "; Unable to add fileset family to backup database");
      ERROR(code);
    }
    code = bc_UpdateVolumeSet(ctPtr);
    if (code) {
      com_err(whoami, code, 
	      "; Unable to sync fileset family info in local cache and backup database");
      ERROR(code);
    }

    code = bc_CreateVolumeSet(bc_globalConfig, (ti=as->parms[0].items)->data);
    if (code) {
      com_err(whoami,code,"; Failed to create fileset family '%s'",
	      as->parms[0].items->data);
    }
    if (code == 0) {
	code = bc_SaveVolumeSet();
	if (code)
	{
	    com_err(whoami, code,
		    "; Failed to add fileset family to database");
	    com_err(whoami, 0,
		    "; Changes are temporary - for this session only");
	}
    }

error_exit:
    if ( ctPtr->lockHandle != 0 )
        bc_UnlockText(ctPtr);
    return(code);
}


/* bc_DeleteVolEntryCmd
 *	delete a volume specification from a volume set
 * params:
 *	parm 0 is vol set name
 *	parm 1 is entry # (integer, 1 based)
 */
     
bc_DeleteVolEntryCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    register long code;
    long entry;
    char *vsname;
    udbClientTextP ctPtr;

    /* lock schedules and check validity */
    ctPtr = &bc_globalConfig->configText[TB_VOLUMESET];

    code = bc_LockText(ctPtr);
    if ( code == BUDB_LOCKED ) {
      com_err(whoami, code, 
	  "; Unable to lock fileset family info in database, try again later");
      ERROR(code);
    }
    else if (code) {
      com_err(whoami, code, 
       "; Unable to delete fileset family entry from backup database");
      ERROR(code);
    }
    code = bc_UpdateVolumeSet(ctPtr);
    if (code) {
      com_err(whoami, code, 
	      "; Unable to sync fileset family info in local cache and backup database");
      ERROR(code);
    }

    vsname = as->parms[0].items->data;
    entry = bc_SafeATOI(as->parms[1].items->data);
    if (entry < 0) {
	com_err(whoami,0,"Can't parse entry number '%s' as decimal integer",
	       as->parms[1].items->data);
	return(BC_BADARG);
    }

    code = bc_DeleteVolumeItem(bc_globalConfig, vsname, entry);
    if (code) {
      com_err(whoami, code, 
	      "; Unable to delete entry '%d' from fileset family '%s'",
	      entry, vsname);
    }
    else 
    {
	code = bc_SaveVolumeSet();
	if (code == 0)
	    dce_svc_printf(BAK_S_DEL_FILSET_FROM_FAMILY_MSG, entry, vsname);
	else
	{
	    com_err(whoami, code,
		    "; Unable to add fileset family entry to database");
	    com_err(whoami, 0,
		    "; Deletion is temporary - for this session only");
	}
    }

error_exit:
    if ( ctPtr->lockHandle != 0 )
        bc_UnlockText(ctPtr);
    return(code);
}




/* bc_DeleteVolSetCmd
 *	delete a volume set, writing out a new configuration file when
 *	completed
 * params:
 *	name of volumeset to delete
 */

bc_DeleteVolSetCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock; 
{
    /* parm 0 is vol set name */
    register long code;
    register struct cmd_item *ti;
    udbClientTextP ctPtr;
    long exitStatus=0;

   /* lock schedules and check validity */
    ctPtr = &bc_globalConfig->configText[TB_VOLUMESET];

    code = bc_LockText(ctPtr);
    if ( code == BUDB_LOCKED ) {
      com_err(whoami, code, 
	  "; Unable to lock fileset family info in database, try again later");
      ERROR(code);
    }
    else if (code) {
      com_err(whoami, code, 
       "; Unable to delete fileset family from backup database");
      ERROR(code);
    }
    code = bc_UpdateVolumeSet(ctPtr);
    if (code) {
      com_err(whoami, code, 
	      "; Unable to sync fileset family info in local cache and backup database");
      ERROR(code);
    }

    for(ti=as->parms[0].items; ti; ti=ti->next)
    {
	code = bc_DeleteVolumeSet(bc_globalConfig, ti->data);
	if (code) {
	  exitStatus=1;
	  com_err(whoami, code,
		  "; Unable to delete fileset family '%s'",
		  ti->data);
	}
	else
	  dce_svc_printf(BAK_S_UNABLE_TO_DELETE_MSG, ti->data);
    }
	
    /* now write out the file */
    if (code == 0)
    {
	code = bc_SaveVolumeSet();
	if (code)
	{
	    com_err(whoami, code,
		    "; Unable to remove fileset family from database");
	    com_err(whoami,0,"Deletion effective for this session only");
	}

    }

error_exit:
    if ( ctPtr->lockHandle != 0 )
        bc_UnlockText(ctPtr);
    if (code)
      return(code);
    else
      return(exitStatus);
}


 /* bc_ListVolSetCmd
 *	list out all the information (?) about a volumeset or about all
 *	volumesets
 * entry:
 *	optional parameter specifies a volumeset name
 */

bc_ListVolSetCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock; 
{
    /* parm 0 is optional volume set to display */
    register long code, exitStatus=0;
    struct bc_volumeSet *tset;
    register struct cmd_item *ti;
    udbClientTextP ctPtr;

    /* lock schedules and check validity */
    ctPtr = &bc_globalConfig->configText[TB_VOLUMESET];

    code = bc_LockText(ctPtr);
    if ( code )
      com_err(whoami, code, "; Unable to fetch fileset families from \
bakserver; Using info in cache");
    else
      (void)bc_UpdateVolumeSet(ctPtr);
    /* figure out volume set to list */
    if (ti=as->parms[0].items) {
      for(;ti;ti=ti->next) {
	/* for each volume set in the command item list */
	lock_ObtainRead(&(bc_globalConfig->vsetLock));
	code = bc_FindVolumeSet(bc_globalConfig, ti->data, &tset);
	if (!code && tset) {
	  (void)ListVolSet(tset);
	  dce_printf(bak_s_newline);
	}
	else {
	  com_err(whoami,code,
		  "; Unable to find fileset family '%s'", ti->data);
	  exitStatus=1;
	}
	lock_ReleaseRead(&(bc_globalConfig->vsetLock));
      }
    }
    else {
	/* no command parameters specified, show entire list */
	for(tset = bc_globalConfig->vset; tset; tset=tset->next) {
	    (void)ListVolSet(tset);
	    dce_printf(bak_s_newline);
	}
    }

    if ( ctPtr->lockHandle )
	bc_UnlockText(ctPtr);
    return exitStatus;
}



/* ------------------------------------
 * support routines
 * ------------------------------------
 */

bc_ClearVolumeSets()
{
    udbClientTextP ctPtr;
    struct udbHandleS *uhptr = &udbHandle;
    struct bc_volumeSet *vsPtr, *vsNextPtr;
    struct bc_volumeEntry *vePtr, *veNextPtr;
    long code;

    extern struct bc_config *bc_globalConfig;

    lock_ObtainWrite(&(bc_globalConfig->vsetLock));
    vsPtr = bc_globalConfig->vset;
    while ( vsPtr != 0 ) {
      vsNextPtr = vsPtr->next;
      /* free all the volume entries */
      vePtr = vsPtr->ventries;
      while ( vePtr != 0 ) {
	veNextPtr = vePtr->next;
	(void)osi_Free(vePtr->name, strlen(vePtr->name)+1);
	(void)osi_Free(vePtr->partname, strlen(vePtr->partname)+1);
	(void)osi_Free(vePtr->serverName, strlen(vePtr->serverName)+1);
	(void)osi_Free(vePtr, sizeof(struct bc_volumeEntry));
	vePtr = veNextPtr;
      }
      (void)osi_Free(vsPtr->name, strlen(vsPtr->name)+1);
      (void)osi_Free(vsPtr, sizeof(struct bc_volumeSet));
      vsPtr = vsNextPtr;
    }
    bc_globalConfig->vset = (struct bc_volumeSet *) 0;
    lock_ReleaseWrite(&(bc_globalConfig->vsetLock));
    return(0);
}

bc_GetVolumeSet()
{
    extern struct bc_config *bc_globalConfig;
    udbClientTextP ctPtr;
    struct udbHandleS *uhptr = &udbHandle;
    long code;

    /* setup the right ptr */
    ctPtr = &bc_globalConfig->configText[TB_VOLUMESET];
    
    /* lock the text */
    code = bc_LockText(ctPtr);
    if ( code )
    	ERROR(code);

    /* fetch the file */
    code = bcdb_GetTextFile(ctPtr);
    if ( code )
    	ERROR(code);

     /* fetch the version number */
    code = ubik_Call(BUDB_GetTextVersion,  uhptr->uh_client, 0,
                     ctPtr->textType, &ctPtr->textVersion);
    if ( code )
        ERROR(code);

    /* parse the file */
    code = bc_ParseVolumeSet();
    if ( code )
    	ERROR(code);

error_exit:
    if ( ctPtr->lockHandle )
	code = bc_UnlockText(ctPtr);
    return(code);

}

/*
 * called with vsetLock held for read
 */
static long ListVolSet(aset)
register struct bc_volumeSet *aset; {
    register struct bc_volumeEntry *tentry;
    register int i;

    dce_printf(bak_s_fileset_family, aset->name);
    dce_printf(bak_s_newline);
    i = 1;
    for(tentry=aset->ventries; tentry; tentry=tentry->next, i++) {
	dce_printf(bak_s_fileset_family_item, i, tentry->serverName, tentry->partname, tentry->name);
	dce_printf(bak_s_newline);
    }
    return 0;
}


/* bc_ParseVolumeSet
 *	Open up the volume set configuration file as specified in our argument,
 *	then parse the file to set up our internal representation.
 * exit:
 *	0 on successful processing,
 */

int
bc_ParseVolumeSet()
{
    static char	rn[] = "bc_ParseVolumeSet";  /*Routine name*/
    char tbuffer[1024];			    /*Buffer for reading config file*/
    char vsname[256];			    /*Volume set name*/
    char serverName[256];		    /*Server name*/
    char partName[256];			    /*Partition name*/
    register struct bc_volumeEntry *tve;    /*Ptr to generated volume spec struct*/
    register struct bc_volumeSet *tvs;	    /*Ptr to volume set struct*/
    struct bc_volumeEntry **ppve, *pve;
    struct bc_volumeSet **ppvs, *pvs;
    register long code;			    /*Generalized return code*/
    char *tp;				    /*Result of fgets(), osi_Alloc */
    int	readHeader;			    /*Is next thing to read a volume set hdr?*/

    udbClientTextP ctPtr;
    register FILE *stream;
    struct bc_config *configPtr;

    extern struct bc_config *bc_globalConfig;

    ctPtr = &bc_globalConfig->configText[TB_VOLUMESET];
    stream = ctPtr->textStream;
    configPtr = bc_globalConfig;

    /*
     * Open up the volume set configuration file, fail if it can't be done.
     */
    lock_ObtainWrite(&(configPtr->vsetLock));
    /* default is an empty volume set */
    configPtr->vset = (struct bc_volumeSet *) 0;

    if ( ctPtr->textSize == 0 ) {			/* empty is ok */
      lock_ReleaseWrite(&(configPtr->vsetLock));
      return(0);
    }
    /* stream checks and initialization */
    if ( stream == NULL ) {
      lock_ReleaseWrite(&(configPtr->vsetLock));
      return(BC_INTERNALERROR);
    }
    rewind(stream);

    readHeader = 1;
    while (1)
    {
	/* Read in and process the next line of the volume set description
	 * file.
	 */
	tp = fgets(tbuffer, sizeof(tbuffer), stream);
	if (!tp)
	    break;
	if (readHeader)
	{ /*r*/
	    /*
	     * Scan a header entry.
	     */
	    readHeader = 0;
	    code = sscanf(tbuffer, "%s %s", serverName, vsname);
	    if ( (code != 2)
	    ||   (strcmp(serverName, "volumeset") != 0)
	       )
	    {
		com_err(whoami, BC_INTERNALERROR, 
			"; Bad fileset family header '%s'",
			tbuffer);
		lock_ReleaseWrite(&(configPtr->vsetLock));
		return BC_INTERNALERROR;
	    }

	    /* Create and fill in the volume set descriptor structure from
	     * the info just read placing it at the head of its queue in the
	     * global configuration structure.
	     */
	    tvs=(struct bc_volumeSet *)osi_Alloc(sizeof(struct bc_volumeSet));
	    bzero((char *)tvs, sizeof(*tvs));
	    tvs->name = (char *)osi_Alloc(strlen(vsname)+1);
	    strcpy(tvs->name, vsname);

	    /* append to the end */
	    for(ppvs = &bc_globalConfig->vset, pvs = *ppvs; pvs; ppvs = &pvs->next, pvs = *ppvs);
	    *ppvs = tvs;
	    tvs->next = (struct bc_volumeSet *) 0;
	} /*r*/
	else
	{ /*e*/
	    /* Scan a volume name entry, which contains the server name, 
	     * partition pattern, and volume pattern.
	     */
	    code = sscanf(tbuffer, "%s %s %s", serverName, partName, vsname);
	    if (code == 1 && strcmp(serverName, "end") == 0)
	    {
		/* This was really a line delimiting the current volume set.
		 * Next time around, we should try to read a header.
		 */
		readHeader = 1;
		continue;
	    }

	    /* The line just read in is a volume spec.  Create a new volume 
	     * spec record, then get the rest of the information regarding 
	     * the host, and stuff everything into place.
	     */
	    tve = (struct bc_volumeEntry *) 
	    			osi_Alloc(sizeof(struct bc_volumeEntry));
	    if (!tve)
	    {
	        code = BC_NOMEM;
		com_err(whoami, code,
			"; Unable to allocate a new fileset entry");
		lock_ReleaseWrite(&(configPtr->vsetLock));
		return code;
	    }
	    bzero((char *)tve, sizeof(*tve));
	    if (!bc_hostNameToAddr(serverName, 
				    (struct sockaddr_in *)&(tve->server)))
	    {
	        code = BC_NOHOST;
		com_err(whoami, code,
			"; Unable to get info on host '%s'",
			serverName);
		lock_ReleaseWrite(&(configPtr->vsetLock));
		return code;
	    }

	    /* The above code has filled in the server sockaddr, now fill in
	     * the rest of the fields.
	     */
	    tve->serverName = (char *)osi_Alloc(strlen(serverName)+1);
	    if (!tve->serverName)
	    {
	      code = BC_NOMEM;
	      com_err(whoami, code,
		      "; Unable to allocate a new fileset entry");
	      lock_ReleaseWrite(&(configPtr->vsetLock));
	      return code;
	    }
	    strcpy(tve->serverName, serverName);
	    tve->partname = (char *)osi_Alloc(strlen(partName)+1);
	    if (!tve->partname)
	    {
	      code = BC_NOMEM;
	      com_err(whoami, code,
		      "; Unable to allocate a new fileset entry");
	      lock_ReleaseWrite(&(configPtr->vsetLock));
	      return code;
	    }
	    strcpy(tve->partname, partName);
	    code = bc_GetPartitionID(partName, serverName, 
				     &tve->partition);
	    if (code)
	    {
		com_err(whoami, code,
			";\nUnable to add fileset entry for fileset family '%s' : server '%s' aggregate '%s'",
			 tvs->name, serverName, partName);
		lock_ReleaseWrite(&(configPtr->vsetLock));
		return code;
	    }
	    tp = (char *)osi_Alloc(strlen(vsname)+1);
	    if (!tp) 
	    {
	      code = BC_NOMEM;
	      com_err(whoami, code, 
		      "Unable to allocate a new fileset entry");
	      lock_ReleaseWrite(&(configPtr->vsetLock));
	      return code;
	    }
	    strcpy(tp, vsname);
	    tve->name =	tp;

	    /* Now, thread it onto the list of other fileset spec entries for
	     * the current fileset family.
	     */

	    for(ppve = &tvs->ventries, pve = *ppve; pve; ppve = &pve->next, pve = *ppve);
	    *ppve = tve;
	    tve->next = (struct bc_volumeEntry *) 0;
	}
    } /*forever loop*/

    /* If we hit an EOF in the middle of a volume set record, we bitch and 
     * moan.
     */
    if (!readHeader) {
      code = BC_INTERNALERROR;
      com_err(whoami, code, 
	      "; Bad fileset family record in the backup database");
      lock_ReleaseWrite(&(configPtr->vsetLock));
      return code;
    }
    lock_ReleaseWrite(&(configPtr->vsetLock));
    /*
     * Well, we did it.  Return successfully.
     */
    return(0);

} /*bc_ParseVolumeSet*/

/* bc_SaveVolumeSet
 *	save the current volume set information to disk
 */

bc_SaveVolumeSet()
{
    register long code;
    register struct bc_volumeSet *tset;
    register struct bc_volumeEntry *tentry;

    udbClientTextP ctPtr;
    register FILE *stream;
    struct bc_config *configPtr;

    extern struct bc_config *bc_globalConfig;

    ctPtr = &bc_globalConfig->configText[TB_VOLUMESET];
    stream = ctPtr->textStream;
    configPtr = bc_globalConfig;

    /* must be locked */
    if ( ctPtr->lockHandle == 0 )
        return(BC_INTERNALERROR);

    /* truncate the file */
    code = ftruncate(fileno(stream), 0);
    if ( code )
        ERROR(errno);

    rewind(stream);

    /* now write the volumeset information */
    lock_ObtainRead(&(configPtr->vsetLock));
    for( tset = bc_globalConfig->vset; tset != 0; tset=tset->next )
    {
	fprintf(stream, "volumeset %s\n", tset->name);
	for(tentry=tset->ventries; tentry; tentry=tentry->next)
	{
	    fprintf(stream, "%s %s %s\n",
		    tentry->serverName, tentry->partname, tentry->name);
	}
	fprintf(stream, "end\n");
    }
    lock_ReleaseRead(&(configPtr->vsetLock));
    if (ferror(stream))
        return(BC_INTERNALERROR);

    /* send to server */
    code = bcdb_SaveTextFile(ctPtr);
    if ( code )
        ERROR(code);

    /* do this on bcdb_SaveTextFile */
    /* increment local version number */
    ctPtr->textVersion++;

    /* update locally stored file size */
    ctPtr->textSize = filesize(ctPtr->textStream);

error_exit:
    return(code);
}

long bc_UpdateVolumeSet(ctPtr)
  udbClientTextP ctPtr;
{
    struct bc_dumpSchedule *dumpPtr, *nextDumpPtr;
    struct udbHandleS *uhptr = &udbHandle;
    long code;

    code = bc_CheckTextVersion(ctPtr);
    if ( code == 0 ) {
      /* version matches, nothing to do */
      goto error_exit;
    }
    else if ( code != BC_VERSIONMISMATCH ) {
      com_err(whoami, code, 
	  "; Unable to contact bakserver; Using fileset family info in cache");
      ERROR(code);
    }
    /* must update the dump schedules */
    dce_svc_printf(BAK_S_OBSOLETE_INFO_IN_CACHE_MSG );
    
    bc_ClearVolumeSets();

    /* now get a fresh set of information from the database */
    code = bcdb_GetTextFile(ctPtr);
    if ( code )
        ERROR(code);

     /* fetch the version number */
    code = ubik_Call(BUDB_GetTextVersion,  uhptr->uh_client, 0,
                     ctPtr->textType, &ctPtr->textVersion);
    if ( code )
        return(code);

    /* parse the file */
    code = bc_ParseVolumeSet();
    if ( code )
    	ERROR(code);

error_exit:
    return(code);
}

/* Check if the two volumes are identical or one is a 
 * backup or readonly extension of the other. If so,
 * return 0, otherwise return 1.
 */

int volmatch (vol1, vol2)
char *vol1, *vol2;
{
  int len1, len2,len;
  char  tstr[BU_MAXNAMELEN];

  len1 = strlen(vol1);
  len2 = strlen(vol2);

  len = (len1 < len2) ? len1 : len2;

  if (strncmp(vol1, vol2, len))
    return (1); /* No match */

  if (len1 == len2) /* match found */
    return (0);

      /* One string is a substring of the other, now to find out if one is a backup
       * or readonly extension of the other
       */
  
  if (len1 < len2) {
    strcpy(tstr, vol1);
    strcat(tstr, ".backup");
    if (strcmp(tstr, vol2) == 0)
      return (0); /* found match */
      

    strcpy(tstr, vol1);
    strcat(tstr, ".readonly");

    if (strcmp(tstr, vol2) == 0)
      return (0); /* found match */

  } else {

    strcpy(tstr, vol2);
    strcat(tstr, ".backup");
    if (strcmp(tstr, vol1) == 0)
      return (0); /* found match */

    strcpy(tstr, vol2);
    strcat(tstr, ".readonly");

    if (strcmp(tstr, vol1) == 0)
      return (0); /* found match */
  }

/* No match */
return (1); 
}
