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
 * $Log: tape_hosts.c,v $
 * Revision 1.1.90.1  1996/10/02  16:58:58  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:00  damon]
 *
 * $EndLog$
 */
/*
 * tape_hosts.c -- Manage tape hosts
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <dce/dce_cf.h>
#include <dcedfs/cmd.h>
#include <com_err.h>
#include <dcedfs/compat.h>
#include <dcedfs/butc.h>
#include <dcedfs/budb_errs.h>
#include <error_macros.h>
#include <bc.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bak/tape_hosts.c,v 1.1.90.1 1996/10/02 16:58:58 damon Exp $")

extern struct bc_config *bc_globalConfig;
extern struct udbHandleS udbHandle;
extern char *whoami;
long bc_UpdateHosts();
extern struct hostent *hostutil_GetHostByName();
extern char *hostutil_GetHostByINet();

/* ------------------------------------
 * command level routines
 * ------------------------------------
 */
/* bc_AddHostCmd
 *	Add a host to the tape hosts
 */

bc_AddHostCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    struct cmd_item *ti;
    udbClientTextP ctPtr;
    long port = 0;
    register long code = 0;
    char *validHostName;

    ctPtr = &bc_globalConfig->configText[TB_TAPEHOSTS];
    code = bc_LockText(ctPtr);
    if ( code == BUDB_LOCKED ) {
      com_err(whoami, code, 
          "; Unable to lock tape host info in database, try again later");
      ERROR(code);
    }
    else if (code) {
      com_err(whoami, code, 
	      "; Unable to add tape coordinator host to backup database");
      ERROR(code);
    }

    code = bc_UpdateHosts(ctPtr);
    if (code) {
      com_err(whoami, code, 
	      "; Unable to sync host info in local cache and backup database");
      ERROR(code);
    }

    ti = as->parms[0].items;
    if(ti)
    {
	if(as->parms[1].items)
	    port = bc_SafeATOI(as->parms[1].items->data);
	else
	    port = 0;
	
	if (port < 0)
	{
	    com_err(whoami,0,"Bad port '%s'", as->parms[1].items->data);
	    ERROR(BC_BADARG);
	}
	if(port > BC_MAXPORTOFFSET)
	{
	    com_err(whoami,0,"%u exceeds max port offset %u", 
		    port, BC_MAXPORTOFFSET);
	    ERROR(BC_BADARG);
	}
	/*
	 * Let us do some intelligent parsing of the host name before we
	 * go ahead and add it to the database.
	 * 1. If host name is /.:/hosts/<hostname>, we expand it.
	 * 2. Validate the host name; If DCE host name, lookup in CDS.
	 *    If IP address of Internet style host name, do a gethostbyname.
	 *    Reject entries that fail to get through validation. 
	 * 3. If IP address is added, try to convert it into Internet style 
	 *    host name before adding it.
	 */
	code = bc_validateTapeHostName(ti->data, &validHostName);
	if (code) {
	  com_err(whoami, code, 
		  "Entered an invalid tape coordinator host name");
	  ERROR(code);
	}
	dce_printf(bak_s_adding_host_to_list, validHostName, port);
	code = bc_AddTapeHost(bc_globalConfig, validHostName, port);
	if (code)
	{
	    dce_svc_printf(BAK_S_FILED_MSG);
	    com_err(whoami, code, "; Unable to add tape host");
	    ERROR(code);
	}

	code = bc_SaveHosts();
	if ( code )
	{
	    dce_svc_printf(BAK_S_FAILED_MSG);
	    com_err(whoami, code,
		    "; Unable to add tape host to database");
	    com_err(whoami, 0,"Changes are temporary - for this session only");
	    ERROR(code);
	}
	(void)osi_Free(validHostName, strlen(validHostName)+1);
    }

    /* done */
    dce_svc_printf(BAK_S_DONE_MSG);

error_exit:
    if ( ctPtr->lockHandle != 0 )
        bc_UnlockText(ctPtr);
    return(code);
}
    
bc_DeleteHostCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock; 
{
    struct cmd_item *ti;
    long port = 0;
    udbClientTextP ctPtr;
    register long code = 0;

    ctPtr = &bc_globalConfig->configText[TB_TAPEHOSTS];
    code = bc_LockText(ctPtr);
    if ( code == BUDB_LOCKED ) {
      com_err(whoami, code, 
          "; Unable to lock tape host info in database, try again later");
      ERROR(code);
    }
    else if (code) {
      com_err(whoami, code, 
	"; Unable to delete tape coordinator host entry from backup database");
      ERROR(code);
    }
    code = bc_UpdateHosts(ctPtr);
    if (code) {
      com_err(whoami, code, 
	      "; Unable to sync host info in local cache and backup database");
      ERROR(code);
    }

    if(as->parms[0].items)
	port = bc_SafeATOI(as->parms[0].items->data);
    else
	port = 0;
    if (port < 0) 
    {
	com_err(whoami,0,"Bad port '%s'", as->parms[0].items->data);
	ERROR(BC_BADARG);
    }
    if(port > BC_MAXPORTOFFSET)
    {
	com_err(whoami,0,"%u exceeds max port offset %u",
		port,BC_MAXPORTOFFSET);
	ERROR(BC_BADARG);
    }
    dce_printf(bak_s_deleteing_from_tape_list, port);
    code = bc_DeleteTapeHost(bc_globalConfig, port);
    if (code) {
      dce_svc_printf(BAK_S_FAILED_MSG);
      com_err(whoami, code, 
	      "; Unable to delete tape host");
      ERROR(code);
    }
    
    code = bc_SaveHosts();
    if ( code )
    {
      dce_svc_printf(BAK_S_FAILED_MSG);
      com_err(whoami, code, 
	      "; Unable to delete tape host from database");
      com_err(whoami, 0,"Changes are temporary - for this session only");
      ERROR(code);
    }
    dce_svc_printf(BAK_S_DONE_MSG);
error_exit:
    if ( ctPtr->lockHandle != 0 )
        bc_UnlockText(ctPtr);
    return(code);
}


/* bc_ListHostsCmd
 *	list all tape hosts (from internally built tables)
 * parameters:
 *	ignored
 */

bc_ListHostsCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock; 
{
    register struct bc_hostEntry *tentry;
    register int first;
    udbClientTextP ctPtr;
    register long code;

    /* lock schedules and check validity */
    ctPtr = &bc_globalConfig->configText[TB_TAPEHOSTS];
    code = bc_LockText(ctPtr);
    if ( code )
      com_err(whoami, code, 
	   "; Unable to fetch tape hosts from bakserver; Using info in cache");
    else
      (void)bc_UpdateHosts(ctPtr);
    lock_ObtainRead(&(bc_globalConfig->hostLock));
    for ( first=1, tentry = bc_globalConfig->tapeHosts; tentry; 
	  first=0, tentry = tentry->next )
    {
      if (first)
	dce_svc_printf(BAK_S_TAPE_HOSTS_MSG);
      dce_svc_printf(BAK_S_HOST_PORT_PAIT_MSG, tentry->name, tentry->portOffset);
    }
    lock_ReleaseRead(&(bc_globalConfig->hostLock));
    if ( ctPtr->lockHandle )
	bc_UnlockText(ctPtr);
    return 0;
}

/* ------------------------------------
 * support routines
 * ------------------------------------
 */
bc_AddTapeHost(aconfig, aname,aport)
struct bc_config *aconfig;
long aport;
char *aname; {
    register struct bc_hostEntry **tlast, *tentry;
    uuid_t uuid;
    unsigned_char_t *uuidStr;
    rpc_binding_handle_t binding;
    long code = 0;

    if (aconfig == (struct bc_config *)NULL)
      return BC_INTERNALERROR;
    if (aport < 0 || aport > BC_MAXPORTOFFSET)
      return BC_BADTCID;

    /* check if this entry is already in the list */
    lock_ObtainShared(&(aconfig->hostLock));
    for(tentry = aconfig->tapeHosts; tentry; tentry=tentry->next) {
	/* entries are uniquely defined by their port offset (TCID) */
	if (tentry->portOffset == aport) {
	  lock_ReleaseShared(&(aconfig->hostLock));
	  return BC_TCIDEXISTS;
	}
    }
    /* add this entry to the end of the list */

    for(tlast = (struct bc_hostEntry **)&(aconfig->tapeHosts),tentry = *tlast; 
	tentry; tlast = &tentry->next, tentry = *tlast);
    lock_UpgradeSToW(&(aconfig->hostLock));
    /* tlast now points to the next pointer (or head pointer) we should 
       overwrite */
    tentry = (struct bc_hostEntry *) osi_Alloc(sizeof(struct bc_hostEntry));
    bzero((char *)tentry, sizeof(*tentry));
    tentry->name = (char *) osi_Alloc(strlen(aname)+1);
    strcpy((char *)tentry->name, aname);
    *tlast = tentry;
    tentry->next = (struct bc_hostEntry *) 0;

    uuid_create(&uuid,(unsigned32 *) &code);
    if (code != uuid_s_ok) {
      lock_ReleaseWrite(&(aconfig->hostLock));
      return code;
    }
    uuid_to_string(&uuid, &uuidStr, (unsigned32 *)&code);
    if (code != uuid_s_ok) {
      lock_ReleaseWrite(&(aconfig->hostLock));
      return code;
    }
    tentry->uuid = (char *)osi_Alloc(strlen((const char *)uuidStr)+1);
    if (!tentry->uuid) {
      lock_ReleaseWrite(&(aconfig->hostLock));
      return ENOMEM;
    }
    strcpy(tentry->uuid, (char *)uuidStr);
    rpc_string_free(&uuidStr, (unsigned32 *)&code);
    dfs_locate_rpc_server((unsigned char *)aname, &binding, &uuid, 0, 0,
			  (error_status_t *)&code);
    if ( code != rpc_s_ok && code != COMPAT_ERROR_SHORTNAME_NOAUTH) {
      lock_ReleaseWrite(&(aconfig->hostLock));
      return code;
    }
    rpc_binding_copy (binding, &tentry->binding, (unsigned32 *)&code);
    tentry->portOffset = aport;
    lock_ReleaseWrite(&(aconfig->hostLock));
    return 0;
}

bc_DeleteTapeHost(aconfig, aport)
struct bc_config *aconfig;
long aport; {
    register struct bc_hostEntry **tlast, *tentry;
    long code = 0;
    if (aconfig == (struct bc_config *)NULL)
      return BC_INTERNALERROR;
    if (aport < 0 || aport > BC_MAXPORTOFFSET)
      return BC_BADTCID;

    /* find entry to delete */
    lock_ObtainShared(&(aconfig->hostLock));
    tlast = (struct bc_hostEntry **)&(aconfig->tapeHosts);
    for(tentry = *tlast; tentry; tlast = &(tentry->next), tentry = *tlast)
	/* entries are uniquely defined by their port offset (TCID) */
	if (aport == tentry->portOffset)
	    break;
    if (!tentry) {
      lock_ReleaseShared(&(aconfig->hostLock));
      return BC_NOTCID;	/* failed to find it */
    }
    /* otherwise delete the entry from the list and free appropriate
       structures */
    lock_UpgradeSToW(&(aconfig->hostLock));
    *tlast = tentry->next;
    (void)osi_Free(tentry->name, strlen(tentry->name)+1);
    rpc_binding_free(&tentry->binding, (unsigned32 *)&code);
    (void)osi_Free(tentry->uuid, strlen(tentry->uuid)+1);
    (void)osi_Free(tentry, sizeof(struct bc_hostEntry));
    lock_ReleaseWrite(&(aconfig->hostLock));
    return 0;
}

/*-----------------------------------------------------------------------------
 * bc_hostNameToAddr
 *
 * Description:
 *	Convert a host name in IP address format or Internet domain host name
 *	format or DCE style host name format to a sockaddr structure.
 *
 * Arguments:
 *	name - host name in one of above formats
 *	sockaddr - returns a sockaddr for the address of server 
 * Returns:
 *	0 if host not found
 *	1 if host found
 *
 * Environment:
 *	Nothing interesting.
 *
 * Side Effects:
 *	As advertised.
 *-----------------------------------------------------------------------------
 */

int bc_hostNameToAddr(servName, servAddrp)
     char *servName;
     struct sockaddr *servAddrp;
{
  /*
   * When handling server name wildcards, we should be able to find out if
   * the server name passed in is a pattern or a name. If it is a pattern
   * we should not try calling GetServer. If it is a name, we should go ahead
   * and get an address. For now, we only handle .* wildcard pattern or a 
   * server name. This should change once server patterns are implemented.
   */
  if (!strcmp(servName, ".*")) {
    bzero((char *)servAddrp, sizeof(struct sockaddr));
    return 1;
  }
  /* wrapper routine to the fts GetServer routine */
  return(GetServer(servName, servAddrp));
}

/*
 * bc_validateTapeHostName
 *	Host names could be in one of three formats - DCE, Internet domain or
 *	IP address. This routine handles names in these three formats. It
 *	expands DCE host names to the canonical one, converts IP addresses
 *	into Internet domain host names, and makes sure these entries do exist.
 *	-- The caller should call osi_Free on *validatedHost
 */
bc_validateTapeHostName(rawHost, validatedHost)
     char *rawHost;
     char **validatedHost;
{
  error_status_t st;
  struct hostent *he;
  struct sockaddr_in sa;
  char *cellName;
  struct in_addr sin;

  if (!bc_hostNameToAddr(rawHost, &sa))
    return BC_NOHOST;
  dce_cf_get_cell_name(&cellName, &st);
  if (st != dce_cf_st_ok)
    return st;
  if (strncmp(rawHost, "/.:", 3) == 0) {
    *validatedHost = (char *)osi_Alloc(strlen(rawHost)+strlen(cellName)-2);
    if (*validatedHost == (char *)NULL)
      return BC_NOMEM;
    strcpy(*validatedHost, cellName);
    strcat(*validatedHost, (char *)(rawHost+3));
  }
  else if (strncmp(rawHost, "/...", 4) == 0) {
    *validatedHost = (char *)osi_Alloc(strlen(rawHost)+1);
    if (*validatedHost == (char *)NULL)
      return BC_NOMEM;
    strcpy(*validatedHost, rawHost);
  }
  else { /* name could be a.b.c.d or internet host name */
    he = (struct hostent *)hostutil_GetHostByName(rawHost);
    if (he) {
      sin = *((struct in_addr *) he->h_addr_list[0]);
      rawHost = (char *)hostutil_GetNameByINet(sin);
      if (rawHost) {
	*validatedHost = (char *)osi_Alloc(strlen(rawHost)+1);
	if (*validatedHost == (char *)NULL)
	  return BC_NOMEM;
	strcpy(*validatedHost, rawHost);
      }
      else {
	*validatedHost = (char *)osi_Alloc(strlen(rawHost)+1);
	if (*validatedHost == (char *)NULL)
	  return BC_NOMEM;
	strcpy(*validatedHost, rawHost);
      }	
    }
    else {
      *validatedHost = (char *)osi_Alloc(strlen(rawHost)+1);
      if (*validatedHost == (char *)NULL)
	return BC_NOMEM;
      strcpy(*validatedHost, rawHost);
    }
  }
  return 0;
}

bc_ClearHosts()
{
    register struct bc_hostEntry *tentry, *temp;
    long code;

    lock_ObtainWrite(&(bc_globalConfig->hostLock));
    tentry = bc_globalConfig->tapeHosts;
    while ( tentry != 0 )
    {
	temp = tentry->next;
	(void)osi_Free(tentry->name, strlen(tentry->name)+1);
	rpc_binding_free (&tentry->binding,(unsigned32 *) &code);
	(void)osi_Free(tentry->uuid, strlen(tentry->uuid)+1);
	(void)osi_Free(tentry, sizeof(struct bc_hostEntry));
	tentry = temp;
    }

    bc_globalConfig->tapeHosts = (struct bc_hostEntry *) 0;
    lock_ReleaseWrite(&(bc_globalConfig->hostLock));
    return(0);
}

bc_GetHosts()
{
    extern struct bc_config *bc_globalConfig;
    udbClientTextP ctPtr;
    struct udbHandleS *uhptr = &udbHandle;
    long code;

    /* setup the right ptr */
    ctPtr = &bc_globalConfig->configText[TB_TAPEHOSTS];
    
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
    code = bc_ParseHosts();
    if ( code )
    	ERROR(code);

error_exit:
    if ( ctPtr->lockHandle )
	code = bc_UnlockText(ctPtr);
    return(code);

}

bc_GetHostText ()
{
    extern struct bc_config *bc_globalConfig;
    udbClientTextP ctPtr;
    struct udbHandleS *uhptr = &udbHandle;
    long code;

    /* setup the right ptr */
    ctPtr = &bc_globalConfig->configText[TB_TAPEHOSTS];
    
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

error_exit:
    if ( ctPtr->lockHandle )
	code = bc_UnlockText(ctPtr);
    return(code);

}

/* bc_ParseHosts
 *	Open up the volume set configuration file as specified in our argument,
 *	then parse the file to set up our internal representation.
 * exit:
 *	0 on successful processing,
 */

int bc_ParseHosts()
{
    char tbuffer[512];
    char hostName[256];
    long port = 0;
    struct bc_hostEntry *tfirst, *tlast, *the;
    char *tp;
    struct hostent *th;
    rpc_binding_handle_t binding;
    long code = 0;
    udbClientTextP ctPtr;
    FILE *stream;
    char butcUuidStr[256];
    uuid_t butcUuid;

#ifdef DEBUG
    unsigned_char_t *uuidStr;
    rpc_if_id_t if_id;
#endif

    /* initialize locally used variables */
    ctPtr = &bc_globalConfig->configText[TB_TAPEHOSTS];
    stream = ctPtr->textStream;

    if ( ctPtr->textSize == 0 )                 /* nothing defined yet */
	return(0);

    if ( stream == NULL )
        return(BC_INTERNALERROR);

    rewind(stream);

    /* now read the lines and build the structure list */
    tfirst = tlast = (struct bc_hostEntry *) 0;

    while(1) 
    {
	tp = fgets(tbuffer, sizeof(tbuffer), stream);
	if (!tp)
	    break;	    /* end of file */

	sscanf(tbuffer, "%s %s %u", hostName, butcUuidStr, &port);
	/* get the bindings to the host */
	uuid_from_string ((unsigned char *)butcUuidStr, &butcUuid, (unsigned32 *)&code);
	if (code != uuid_s_ok)
	{
	    dce_svc_printf(BAK_S_UUID_FROM_STRING_FAILED_MSG, dfs_dceErrTxt(code));
	    continue;
	}
#ifdef DEBUG
	rpc_if_inq_id (BUTC_v4_0_c_ifspec, &if_id, &code);
	uuid_to_string(&if_id.uuid, &uuidStr, (unsigned32 *)&code);
	dce_svc_printf(BAK_S_VERSION_NUMBER_MSG, uuidStr, if_id.vers_major, if_id.vers_minor);
	rpc_string_free(&uuidStr, &code);
#endif /* DEBUG */

	dfs_locate_rpc_server ((unsigned char *)hostName, &binding, &butcUuid, 
			       0, 0, (error_status_t *)&code);
	if ( code != rpc_s_ok && code != COMPAT_ERROR_SHORTNAME_NOAUTH )
	{
	    dce_svc_printf(BAK_S_LOCATE_DFS_SERVER_FAILED_MSG, "bc_ParseHosts",
                           dfs_dceErrTxt(code));
	    continue;
	}
#ifdef DEBUG
	compat_print_binding("\nbinding: ", binding);
	fflush(stdout); fflush(stderr);
#endif /* DEBUG */
	the = (struct bc_hostEntry *) osi_Alloc(sizeof(struct bc_hostEntry));
	if (tlast) 
	{
	    tlast->next = the;
	    tlast = the;
	}
	else 
	{
	    tfirst = tlast = the;
	}
	the->next = (struct bc_hostEntry *) 0;
	the->name = (char *)osi_Alloc(strlen(hostName)+1);
	strcpy(the->name, hostName);
	the->uuid = (char *)osi_Alloc(strlen(butcUuidStr)+1);
	strcpy (the->uuid, butcUuidStr);
	the->portOffset = port;
	rpc_binding_copy (binding, &the->binding, (unsigned32 *) &code);
    }
    lock_ObtainWrite(&(bc_globalConfig->hostLock));
    bc_globalConfig->tapeHosts = tfirst;
    lock_ReleaseWrite(&(bc_globalConfig->hostLock));
    return (0);
}

/* bc_SaveHosts
 *	really two parts
 *	1) save the current host information to disk
 *	2) transmit to ubik server
 */

bc_SaveHosts()
{
    register long code;

    udbClientTextP ctPtr;
    register FILE *stream;
    struct bc_hostEntry *hePtr;

    extern struct bc_config *bc_globalConfig;

    ctPtr = &bc_globalConfig->configText[TB_TAPEHOSTS];
    stream = ctPtr->textStream;

    /* must be locked */
    if ( ctPtr->lockHandle == 0 )
        return(BC_INTERNALERROR);

    /* truncate the file */
    code = ftruncate(fileno(stream), 0);
    if ( code )
        ERROR(errno);

    rewind(stream);

    lock_ObtainRead(&(bc_globalConfig->hostLock));
    hePtr = bc_globalConfig->tapeHosts;

    while ( hePtr != 0 )
    {
	fprintf(stream, "%s %s %u\n", hePtr->name, hePtr->uuid, hePtr->portOffset);
	hePtr = hePtr->next;   
    }
    lock_ReleaseRead(&(bc_globalConfig->hostLock));

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

long bc_UpdateHosts(ctPtr)  
  udbClientTextP ctPtr;
{
    struct udbHandleS *uhptr = &udbHandle;
    long code;

    code = bc_CheckTextVersion(ctPtr);
    if ( code == 0 )
    {
	/* version matches, nothing to do */
	goto error_exit;
    }
    else
    if ( code != BC_VERSIONMISMATCH )
    {
	/* some other error */
        com_err(whoami, code, 
	       "; Unable to contact bakserver; Using tape host info in cache");
	ERROR(code);
    }

    /* must update the dump schedules */
    dce_svc_printf(BAK_S_OBSOLETE_HOSTS_INFO_MSG );
    bc_ClearHosts();

    /* now get a fresh set of information from the database */
    code = bcdb_GetTextFile(ctPtr);
    if ( code )
        ERROR(code);

     /* fetch the version number */
    code = ubik_Call(BUDB_GetTextVersion,  uhptr->uh_client, 0,
                     ctPtr->textType, &ctPtr->textVersion);
    if ( code )
        ERROR(code);

    /* parse the file */
    code = bc_ParseHosts();
    if ( code )
    	ERROR(code);

error_exit:
    return(code);
}

/*
 * bc_getUuidFromBinding
 *	search for the UUID corresponding to the host name and port. We do
 *	something more than a simple string compare. We try comparisons 
 *	based on DCE style host name, Internet style host name, and Internet
 *	address, in that order. This would handle all kinds of host name
 *	formats used by the administrator.
 */
long bc_getUuidFromBinding(dceHostName, inHostName, inPort, inUuid)
  char *dceHostName, *inHostName;
  long inPort;
  unsigned_char_t  **inUuid;

{
    char hostName[BU_MAXNAMELEN], ipAddr[24], tbuffer[256];
    char localHostName[BU_MAXNAMELEN];
    static char uuidStr[128];
    long port;
    udbClientTextP ctPtr;
    FILE *stream;
    char *tp;
    int i;
    struct hostent *he;
    long code = 0;
    struct in_addr haddr;
    extern struct bc_config *bc_globalConfig;
    extern struct hostent *hostutil_GetHostByName();

    /* First construct a IP address from inHostName */
    he = (struct hostent *)hostutil_GetHostByName(inHostName);
    if (he) {
      haddr = *((struct in_addr *) he->h_addr);
      strcpy(ipAddr, (char *)inet_ntoa(haddr));
    }
    /* construct the short host name (no domain) from the inHostName */
    strcpy(localHostName, inHostName);
    tp = strchr(localHostName, '.');
    if (tp) {
	*tp = '\0';
    }
    bzero((char *)hostName, BU_MAXNAMELEN);
    bzero((char *)uuidStr, 128);
    *inUuid = (unsigned_char_t *)NULL;
    ctPtr = &bc_globalConfig->configText[TB_TAPEHOSTS];
    stream = ctPtr->textStream;
    if (ctPtr->textSize == 0)
	return(0);
    if (stream == NULL)
	return (BC_INTERNALERROR);
    rewind(stream);

    while(1)
    {
	tp = fgets(tbuffer, sizeof(tbuffer), stream);
	if (!tp)
	    break;
	sscanf(tbuffer, "%s %s %u", hostName, uuidStr, &port);
	if ((!strcmp(dceHostName, hostName) || !strcmp(inHostName, hostName) ||
	     !strcmp(ipAddr, hostName) || !strcmp(localHostName, hostName)) &&
	     inPort == port)
	{
	    if (!strcmp((char *)uuidStr, (char *)""))
		return(BC_NOHOST);
	    *inUuid = (unsigned_char_t *)&uuidStr[0];
	    return(0);
	}
    }
    return(BC_NOHOST);
}
