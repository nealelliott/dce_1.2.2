/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: client.c,v $
 * Revision 1.1.195.1  1996/10/02  21:02:30  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:52  damon]
 *
 * $EndLog$
 */
/*
 *      Copyright (C) 1996, 1991 Transarc Corporation
 *      All rights reserved.
 */

/*
 *       UPDATE Server (AFS 4.0) Client Operation
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dirent.h>
#include <pthread.h>
#include <dce/rpc.h>
#include <dcedfs/compat.h>
#include <dcedfs/cmd.h>
#include <update.h>
#include <upcommon.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsudtmac.h>
#include <dfsudtsvc.h>
#include <dfsudtmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/update/client.c,v 1.1.195.1 1996/10/02 21:02:30 damon Exp $")

/* flag to check if UPDATE interface accessible */
unsigned int UPDATE_INTERFACE = 0;
unsigned int UPDATE_CHECK_PROVIDER_OR_RANGE = 0;

/* List of interfaces supported */
#define DFS_UPDATE		1
#define UPDATE_PROVIDER         1

/* should be NIDL_PIPE_BUFF_SIZE defined in stubbase.h */
#define PREFER_BUFFSIZE  	2048
#define UPDATE_MAXBINDINGS 	32
#define MAX_RPC_FAILURES	10
#define UPDATE_RETRY_TIME	60
#define UPDATE_MAX_CANCEL_ITER	20

/* macros to refresh ubik connections upon ticket expiration */
unsigned int DFS_RPC_WRAP_ERROR;
#define DFS_RPC_PREWRAP { int rpc_attempts, retry_rpc, dfsauthRtn; \
			  rpc_attempts = dfsauthRtn = 0; \
			  retry_rpc = 1; \
                          while (retry_rpc) { \
                            retry_rpc = 0; \
			    rpc_attempts++;

#define DFS_RPC_POSTWRAP(error_status) \
                      if (error_status == rpc_s_auth_tkt_expired) { \
                        if (rpc_attempts == 1) { \
                         dfsauthRtn =dfsauth_client_RefreshLocalAuthContext();\
                         if (!dfsauthRtn) retry_rpc = 1; \
                         else {\
		           retry_rpc = 0; \
			   DFS_RPC_WRAP_ERROR = 1; \
			}}}}}


char       *whoami = "upclient";


dce_svc_handle_t   udt_svc_handle;

void initialize_svc()
{
  error_status_t st;

  udt_svc_handle = dce_svc_register(udt_svc_table,  (idl_char *)"udt",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dfs_define_udt_message_tables();

}



/* Forward declaration for ANSI C */
long isCompatibleObject();

/* filestr - client's list of items to import */

struct filestr {
    struct filestr *next;
    int type;
    char *name;
};

/* client globals */
upLogP  logP = 0;               /* upclient's handle on it's log file */

void pipeAlloc();
void ReceiveFile();
int  AddToList();
int  ZapList();
int  IsCompatible();
int  NotOnHost();

/* Check to see if client has access to desired interface. Can associate
 * priorities with interfaces in the case of servers exporting multiple
 * interfaces.
 */
static error_status_t
dfs_selectInterface (h)
  rpc_binding_handle_t h;

{
    rpc_if_id_t if_id;
    static dfs_interfaceList serverInterfaces;
    unsigned int i, j, result;
    error_status_t code;

    serverInterfaces.dfs_interfaceList_len = 0;
    bzero((char *)serverInterfaces.dfs_interfaceList_val,
	  MAXINTERFACESPERVERSION * sizeof(dfs_interfaceDescription));

    DFS_RPC_PREWRAP
    code = UPDATE_GetServerInterfaces (h, &serverInterfaces);
    DFS_RPC_POSTWRAP(code)

    if 	( IS_COMM_ERR(code) )
	return code;

#ifdef DEBUG
	dfs_printInterfaceDescription (&serverInterfaces, &code);
#endif

    /* Check to see if you have access to desired interface. The
       following are the different classes from most desired to
       least.

       Good:  UUID and major version numbers of the client and server
              interfaces are identical.

	      1. client and server interfaces are identical -- the minor
	         version numbers and the provider_version numbers are equal.
              2. Server minor version is greater than client's.
	      3. Server provider_version is greater than client's.

       Medium: UUID and major version numbers of client and server are
               identical, and the minor version of client is less than
	       or equal to server's. In addition the provider_version
	       number of client is greater than server's.

       Bad:    The UUID or the major version number of the client is different
               from the server's. Client-Server binding in this case is
	       impossible and hence this case is least desirable.
     */

    if (( result =  dfs_sameInterface (UPDATE_v4_0_c_ifspec,
				       UPDATE_PROVIDER,
				       &serverInterfaces)) == MATCH_GOOD ||
	                               result == MATCH_MEDIUM) {

	    /* check for new interfaces derived from the original interface
	       should be in here, if multiple versions of a DFS interface is
	       supported. Here we only have one version of the interface.
	     */
	    UPDATE_INTERFACE = DFS_UPDATE; /* original interface */

	    if (result == MATCH_MEDIUM)
		UPDATE_CHECK_PROVIDER_OR_RANGE = 1;
	}
    if (!UPDATE_INTERFACE)
      return UP_BAD_INTERFACE;
    else return 0;
}

/* RPC Wrappers */

error_status_t
update_FetchFile (h, filename, pipeP)
  rpc_binding_handle_t h;
  NameString_t filename;
  pipe_t *pipeP;
{
    error_status_t code;
    static unsigned long FUNCTION_PROVIDER = 1; /* provider_version #
						   this RPC is supported
						 */
    static unsigned long FUNCTION_NOOP = 0;	/* Is this function supp-
						   orted by server or not? */

    if (FUNCTION_NOOP) /* this RPC not supported */
	return (rpc_s_op_rng_error);

    /* Check to see if the desired interface provider is supported. If not
       we may try alternate interfaces. Also check if this RPC is supported
       by the server interface. This is done by comparing provider_versions
    */

    if (UPDATE_INTERFACE == DFS_UPDATE &&
	UPDATE_PROVIDER >= FUNCTION_PROVIDER) {

#ifdef UPDATE_CANCEL_TEST
	  Log(udt_s_call_upd_fetchfile);
#endif /* UPDATE_CANCEL_TEST */
	DFS_RPC_PREWRAP
	code = UPDATE_FetchFile (h, filename, pipeP);
	DFS_RPC_POSTWRAP(code)

#ifdef UPDATE_CANCEL_TEST
	  Log(udt_s_cmpltd_upd_fetchfile);
#endif /* UPDATE_CANCEL_TEST */

	if ( IS_COMM_ERR(code) ) {
	    if (UPDATE_CHECK_PROVIDER_OR_RANGE && code == rpc_s_op_rng_error){
	        code = UP_BAD_INTERFACE;
		FUNCTION_NOOP = 1;
	    }
	}
	return code;
    }
    else
      return(UP_BAD_INTERFACE);
}

error_status_t
update_FetchInfo (h, name, pipeP)
  rpc_binding_handle_t h;
  NameString_t name;
  pipe_t *pipeP;
{
    error_status_t code;
    static unsigned long FUNCTION_PROVIDER = 1; /* provider_version #
						   this RPC is supported
						 */
    static unsigned long FUNCTION_NOOP = 0;	/* Is this function supp-
						   orted by server or not? */

    if (FUNCTION_NOOP) /* this RPC not supported */
	return (rpc_s_op_rng_error);

    if (UPDATE_INTERFACE == DFS_UPDATE &&
	UPDATE_PROVIDER >= FUNCTION_PROVIDER) {

#ifdef UPDATE_CANCEL_TEST
	  Log(udt_s_call_upd_fetchinfo);
#endif /* UPDATE_CANCEL_TEST */
	DFS_RPC_PREWRAP
	code = UPDATE_FetchInfo (h, name, pipeP);
	DFS_RPC_POSTWRAP(code)

#ifdef UPDATE_CANCEL_TEST
	  Log(udt_s_cmpltd_upd_fetchinfo);
#endif /* UPDATE_CANCEL_TEST */

	if ( IS_COMM_ERR(code) ) {
	    if (UPDATE_CHECK_PROVIDER_OR_RANGE && code == rpc_s_op_rng_error){
	      code = UP_BAD_INTERFACE;
		FUNCTION_NOOP = 1;
	    }
	}
	return code;
    }
    else
      return(UP_BAD_INTERFACE);
}

error_status_t
update_FetchObjectInfo (h, name, buf)
  rpc_binding_handle_t h;
  char *name;
  updateFileStatT *buf;
{
    error_status_t code;
    static unsigned long FUNCTION_PROVIDER = 1; /* provider_version #
						   this RPC is supported
						 */
    static unsigned long FUNCTION_NOOP = 0;	/* Is this function supp-
						   orted by server or not? */
    NameString_t object;	/* For use in UPDATE_FetchObjectInfo call */

    if (FUNCTION_NOOP) /* this RPC not supported */
	return (rpc_s_op_rng_error);

    if (UPDATE_INTERFACE == DFS_UPDATE &&
	UPDATE_PROVIDER >= FUNCTION_PROVIDER) {

	strcpy((char *)object, name);

#ifdef UPDATE_CANCEL_TEST
	  Log(udt_s_call_upd_fobjinfo);
#endif /* UPDATE_CANCEL_TEST */

	DFS_RPC_PREWRAP
	code = UPDATE_FetchObjectInfo (h, object, buf);
	DFS_RPC_POSTWRAP(code)

#ifdef UPDATE_CANCEL_TEST
	  Log(udt_s_cmpltd_upd_fobjinfo);
#endif /* UPDATE_CANCEL_TEST */

	if (IS_COMM_ERR(code)) {
	    if (UPDATE_CHECK_PROVIDER_OR_RANGE && code == rpc_s_op_rng_error){
		code = UP_BAD_INTERFACE;
		FUNCTION_NOOP = 1;
	    }
	}
	return code;
    }
    else
      return(UP_BAD_INTERFACE);
}

/* addClientItem
 *	Add an item to the client's list of objects to import.
 * entry:
 *	nameP - pathname of object
 */

long
addClientItem(objectListPP, nameP)
     struct filestr **objectListPP;
     char *nameP;
{
    char *pathNameP;
    struct filestr *itemP = 0;

    if ( nameP == 0 )				/* must have a valid path */
    {
	Log(udt_s_invalid_arg);
	return(0);
    }

    /* now check if this object is compatible with previously specified
     * objects.
     */
    if ( isCompatibleObject(*objectListPP, nameP) == 0 )
	return(0);

    /* allocate the item descriptor */
    itemP = (struct filestr *) malloc(sizeof(*itemP));
    if ( itemP == 0 )
	goto error;

    pathNameP = (char *) malloc(strlen(nameP)+1);
    if ( pathNameP == 0 )
	goto error;

    strcpy(pathNameP, nameP);

    /* add it to the object chain */
    itemP->next = *objectListPP;
    *objectListPP = itemP;

    itemP->name = pathNameP;			/* attach pathname */
    return(0);

error:
    LogError(udt_s_bad_malloc, ENOMEM);
    if ( itemP )
	free(itemP);
    return(ENOMEM);
}

/* getObjectType
 *	determine type of an object
 * exit:
 *	0 - invalid type
 *	FSTR_FILE - regular file
 *	FSTR_DIRECTORY - directory
 */

getObjectType(mode)
     u_short mode;
{
    if ( mode & S_IFDIR )
	return(FSTR_DIRECTORY);
    if ( mode & S_IFREG )
	return(FSTR_FILE);

    return(0);
}

/* globals - shared between uc_argHandler and main */
char 	*hostEntryP = 0;			/* cds host name */
int 	verbose = 0;				/* flag */

unsigned int interval;
struct filestr *objectListP;

/* defines for argument handling/ordering */
#define ARG_SERVER	0
#define	ARG_PATH	1
#define	ARG_TIME	2
#define	ARG_LOGFILE	3
#define	ARG_VERBOSE	4

/* uc_argHandler
 *	upclient arg handler. Sets global variables based on the
 *	user supplied arguments
 */

int
uc_argHandler(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    struct cmd_item *cmdItemPtr;
    long code = 0;
    char *oldLogfile;

    /* cds host name - must be present */
    hostEntryP = (char *) malloc(strlen(as->parms[ARG_SERVER].items->data)+1);
    if ( hostEntryP == (char *)0 ) {
      LogError(udt_s_bad_malloc_hostep, ENOMEM);
      return(ENOMEM);
    }
    strcpy(hostEntryP, as->parms[ARG_SERVER].items->data);

    /* logfile. Process this before the path list, so that errors may
     * be logged
     */
    if ( as->parms[ARG_LOGFILE].items )
    {
	char *cptr;

	/* allocate the log structure and clear it */
        logP = (upLogP) malloc(sizeof(upLogT));
        if ( logP == 0 )
        {
	  LogError(udt_s_bad_malloc_logp, ENOMEM);
	  return(ENOMEM);
        }
        bzero((char *)logP, sizeof(*logP));

        cptr = as->parms[ARG_LOGFILE].items->data;
        if ( *cptr == '/' )
        {
            /* this is an absolute path - use it as is for the location of the
             * logfile
             */
            logP->logfile = (char *) malloc(strlen(cptr)+1);
            if ( logP->logfile == 0 )
            {
	      LogError(udt_s_bad_malloc_logfile, ENOMEM);
	      return(ENOMEM);
            }
            strcpy(logP->logfile, cptr);
        }
        else
        {
            int length;

            /* relative filename. Prepend the default path to the filename */
            length = strlen(DCELOCAL_PATH) + strlen(LOGPATH) + strlen(cptr)
			+ 1;
            logP->logfile = (char *) malloc(length);
            if ( logP->logfile == 0 )
            {
	      LogError(udt_s_bad2_malloc_logfile, ENOMEM);
	      return(ENOMEM);
            }
            strcpy(logP->logfile, DCELOCAL_PATH);
            strcat(logP->logfile, LOGPATH);
            strcat(logP->logfile, cptr);
        }

	/* rename logfile to logfile.old */
	oldLogfile = malloc(strlen(logP->logfile) + sizeof(".old"));
	if ( oldLogfile )
	{
	    strcpy(oldLogfile, logP->logfile);
	    strcat(oldLogfile, ".old");
	    rename(logP->logfile, oldLogfile);
	    free(oldLogfile);
	}
        /* open the logfile */
	logP->stream = fopen(logP->logfile, "w");
        if ( logP->stream == NULL )
        {
            LogError(udt_s_no_open_logfile, errno, logP->logfile);
	    return(errno);
        }
    }

    /* -path, save the list of objects */
    if ( as->parms[ARG_PATH].items )
    {
	/* remember each of the files/directories */
	cmdItemPtr = as->parms[ARG_PATH].items;
	while ( cmdItemPtr )
	{
	    stripTrailingSlash(cmdItemPtr->data);
	    code = addClientItem(&objectListP, cmdItemPtr->data);
	    if (code)
	      return(code);
	    cmdItemPtr = cmdItemPtr->next;
	}
    }

    /* -time, optional */
    if ( as->parms[ARG_TIME].items )
    {
	interval = atol(as->parms[ARG_TIME].items->data);
	if ( interval <= 0 )
	    interval = TIMEOUT;
    }

    /* third param - verbose flag, optional */
    if ( as->parms[ARG_VERBOSE].items )
	verbose++;

exit:
    return(code);
}

int	rpcFailures = 0;
int     commTry, numTry;

/* fetchFile
 *	fetch a specific file from the upserver
 * entry:
 *	df - describes file to be fetched (incl. path of file)
 *	pipeP - pipe to use for data transfer
 *	binding - rpc binding for upserver
 */

fetchFile(df, pipeP, binding)
     struct filestr *df;
     pipe_t *pipeP;
     rpc_binding_handle_t binding;
{
    updateFileStatT rpcStatBuf;
    struct   timeval tvp[2];
    NameString_t filebuf;
    error_status_t escode;
    long code = 0;

    /* get the file's status */
    if ( verbose )
	dce_svc_printf(UDT_S_CHECKING_FILE_MSG, df->name);

    escode = update_FetchObjectInfo(binding, df->name, &rpcStatBuf);
    if ( escode )
	return(escode);

    /* stat the local file */
    if ( IsCompatible(df->name, rpcStatBuf.mtime, &rpcStatBuf.fileLength) )
	return(0);

    strcpy((char *)filebuf, df->name);
    strcat((char *)filebuf, ".NEW");
    pipeP->state = fopen((char *)filebuf, "w");
    if ( (FILE *) pipeP->state == NULL )
    {
	Log(udt_s_create_filebuf_failed, filebuf);
	return(UP_WRITEFAIL);
    }

    escode = update_FetchFile(binding, df->name, pipeP);
    fclose((FILE *) pipeP->state);
    if ( escode )
    {
	LogError(udt_s_escode_error, escode, filebuf);
	rpcFailures++;
	code = (long) escode;
	goto error_exit;
    }

    rpcFailures = 0;
    code = chmod((char *)filebuf, rpcStatBuf.mode);
    if ( code ) {
	Log(udt_s_chg_protect_failed, filebuf, rpcStatBuf.mode, errno);
	goto error_exit;
    }

    code = chown((char *)filebuf, rpcStatBuf.uid, rpcStatBuf.gid);
    if(code) {
	Log(udt_s_set_uid_failed, rpcStatBuf.uid, rpcStatBuf.gid, filebuf, errno);
    }

    tvp[0].tv_sec = rpcStatBuf.atime;
    tvp[0].tv_usec = 0;
    tvp[1].tv_sec = rpcStatBuf.mtime;
    tvp[1].tv_usec = 0;
    code = utimes((char *)filebuf,tvp);
    if(code){
	Log(udt_s_access_times_both_failed, filebuf,tvp[0],tvp[1], errno);
	goto error_exit;
    }

    /* rename it */
    code = rename((char *)filebuf, df->name);
    if (code)
    {
	Log(udt_s_rename_file_failed, filebuf, df->name, errno);
	goto error_exit;
    }

error_exit:
    unlink((char *)filebuf);
    return(code);
}

/* fetchDirectory
 *	fetch all the files in the specified directory. Will create the
 *	directory if it doesn't exist.
 * entry:
 *	df - includes the pathname of the directory
 *	pipeP - used for data transfer
 *	binding - rpc binding for upserver
 */

long
fetchDirectory(df, pipeP, binding)
     struct filestr *df;
     pipe_t *pipeP;
     rpc_binding_handle_t binding;
{

    struct dirent *dp;
    DIR *dirp;
    FILE *stream = NULL;
    struct   stat tstat;
    struct filestr *ModFiles, *okhostfiles;
    struct filestr *tf;
    NameString_t dirbuf, filename, filebuf;
    char     newname[MAX_NAME_SIZE];
    long time, atime;
    u_long len;
    afs_hyper_t length;
    struct   timeval tvp[2];
    short    uid, gid;
    u_long un_uid, un_gid;		/*Unsigned long versions of the above*/
    u_long mode;
    char     errorMessage[BUFSIZ];

    long              errcode;
    error_status_t scode;
    long retval = 0;

    ModFiles =  (struct filestr *) 0;
    okhostfiles = (struct filestr *) 0;

    if (verbose)
	dce_svc_printf(UDT_S_CHECKING_DIR_MSG, df->name);
    /* For each directory do */
    ZapList(&ModFiles);
    ZapList(&okhostfiles);
    if (stat(df->name, &tstat) < 0)
	mkdir(df->name,0700);	/* try to make the dir */
    /*
     * Scratch pad file and initialize the "state" of the pipe stream
     */

    {
    char *dcesptr;
    dcesptr = (char *)dce_sprintf(udt_s_tmpfile2_getpid, getpid());
    strcpy((char *)dirbuf , dcesptr);
    free(dcesptr);
    }
    pipeP->state = fopen((char *)dirbuf, "w");
    if (pipeP->state == NULL) {
	Log(udt_s_dirbuf_create_failed, dirbuf);
	retval = UP_OPENFAIL;
	goto fail_exit;
    }
    /*
     * Start a RPC call to get the names and relevant info about
     * all the files in the directory df->name into file dirbuf.
     */
    scode = update_FetchInfo(binding, df->name, pipeP);
    fclose((FILE*)pipeP->state);
    if (scode) {
	if (scode == rpc_s_comm_failure)
	    commTry++;
	LogError(udt_s_object_update_fetchinfo_failed, scode, df->name);
	rpcFailures++;
	retval = scode;
	goto fail_exit;
    }
    else {
	rpcFailures = 0;	/* reset failure state if we succeed */
    }

    stream = fopen((char *)dirbuf, "r");
    if (stream == NULL) {
	Log(udt_s_fopen_failed, dirbuf, ++numTry, errno);
	retval = UP_OPENFAIL;
	goto fail_exit;
    }
    umask(00);
    /*
     * While there is more info about files in file dirbuf
     */
    while(fscanf(stream,"%s %u %u %u %u %u %u\n",
		 filename, &time, &len, &mode, &un_uid, &un_gid,
		 &atime) != EOF){

	uid = (short)un_uid;
	gid = (short)un_gid;
	AFS_hset64(length, 0, len);
	AddToList(&okhostfiles, filename, FSTR_FILE);
	/*
	 * Record all the file names which exist on the remote
	 * sync site, to enable purging of redundant files
	 */
	if (verbose)
	    dce_svc_printf(UDT_S_CHKING_FILENAME_MSG, filename);
	if(!IsCompatible(filename, time, &length)){
	    /*
	     *If the file info has changed , record all the
	     * changed files in the ModFiles array.
	     */
	    if (verbose)
		dce_svc_printf(UDT_S_GETTING_FILENAME_MSG, filename);
	    AddToList(&ModFiles, filename, FSTR_FILE);
	    strcpy((char *)filebuf, (char *)filename);
	    strcat((char *)filebuf, ".NEW");
	    /*
	     * Start a RPC call to fetch the entire file, filename,
	     * from the host and write it into filebuf, since the
	     * current file is outdated. The new versions of changed
	     * files is stored as filename.new
	     */
	    pipeP->state = fopen((char *)filebuf, "w");
	    if ((FILE*)pipeP->state == NULL) {
		Log(udt_s_fetch_entire_create_failed, filebuf);
		retval = UP_OPENFAIL;
		goto fail_exit;
	    }
	    scode = update_FetchFile(binding, filename, pipeP);
	    fclose((FILE *)pipeP->state);
	    if(scode) {
		if (scode == rpc_s_comm_failure)
		    commTry++;

		Log(udt_s_scode_failure_on_upd_obj, scode, filename);
		rpcFailures++;
		retval = scode;
		goto fail_exit;
	    }
	    else {
		rpcFailures = 0;	/* reset failure state if we succeed */
	    }

	    /* now set the rest of the file status */
	    errcode = chmod((char *)filebuf,mode);
	    if(errcode){
		Log(udt_s_bad_chmod_for_fstatus, filebuf, mode, errno);
		retval = errno;
		goto fail_exit;
	    }
	    errcode = chown((char *)filebuf, uid, gid);
	    if(errcode){
		Log(udt_s_chg_uid_and_gid_failed, filebuf, gid, uid);
		Log(udt_s_errno_for_bad_chg, errno, whoami);
	    }
	    tvp[0].tv_sec = atime;
	    tvp[0].tv_usec = 0;
	    tvp[1].tv_sec = time;
	    tvp[1].tv_usec = 0;
	    errcode = utimes((char *)filebuf,tvp);
	    if(errcode){
		Log(udt_s_chg_acc_and_times_warning, filebuf,tvp[0],tvp[1]);
		Log(udt_s_errno_for_chg_warning, errno, whoami);
		retval = errno;
		goto fail_exit;
	    }
	}
    }
    fclose(stream);
    stream = NULL;
    unlink((char *)dirbuf);

    /*
     * Delete all the redundant files on the client.
     */

    dirp = opendir(df->name);
    if (dirp == 0) {
	Log(udt_s_open_local_dir_failed, whoami, df->name, errno);
	retval = UP_OPENFAIL;
	goto fail_exit;
    }

    while(dp = readdir(dirp)) {
	/* for all the files in the directory df->name do*/
	strcpy((char *)filename,df->name);
	strcat((char *)filename,"/");
	strcat((char *)filename,dp->d_name);
	/* the file filename is redundant, delete it */
	if (NotOnHost(filename, okhostfiles)) {
	    if (verbose) dce_svc_printf(UDT_S_FLUSHING_FILE_MSG, filename);
	    errcode = unlink((char *)filename);
	    if(errcode)
		Log(udt_s_delete_file_failed,filename, errno);
	}
    }
    closedir(dirp);

    for(tf = ModFiles; tf; tf=tf->next) {
	strcpy(newname, tf->name);
	strcat(newname, ".NEW");
	if (verbose) dce_svc_printf(UDT_S_RENAMING_FILE_MSG, newname);
	errcode = rename(newname, tf->name);
	if (errcode)
	    Log(udt_s_rename_file_failed2, newname, tf->name,
 errno);
    }
    return(0);

fail_exit:
    if ( stream != NULL )
	fclose(stream);
    unlink((char *)dirbuf);
    return(retval);
}

void keepUpToDate (BindingHandle)
  rpc_binding_handle_t *BindingHandle;
{
    struct	timeval 	timeOut;
    struct 	filestr  	*objPtr;
    updateFileStatT 		remoteStatBuf;
    struct 	stat 		localStatBuf;
    int 			remoteObjectType, localObjectType;
    error_status_t    		scode, code;
    unsigned32        		status;
    int      			retrytime;
    pipe_t      *pipeStreamPtr = (pipe_t *)malloc(sizeof(struct pipe_t));
    int		savedCancelState;

#ifdef UPDATE_CANCEL_TEST
    int cancelIter = 0;
#endif /* UPDATE_CANCEL_TEST */

    retrytime = 60;
    if(interval < retrytime)
        retrytime  = interval;

    /*
     * Initialize the "PIPE" data structure
     */
    pipeStreamPtr->alloc = pipeAlloc;
    pipeStreamPtr->push  = ReceiveFile;

    numTry  = 0;     /* retryn due to invalid file name ? */
    commTry = 0;     /* retry due to communication failure. */

    while(rpcFailures < MAX_RPC_FAILURES) {  /* Keep doing it */

#ifdef UPDATE_CANCEL_TEST
      /*
       * While testing for cancels, kill this thread after
       * UPDATE_MAX_CANCEL_ITER iterations
       */
      if (cancelIter++ >= UPDATE_MAX_CANCEL_ITER) {
	Log(udt_s_upclient_iterations_run, UPDATE_MAX_CANCEL_ITER);
	pthread_testcancel();
      }
#endif /* UPDATE_CANCEL_TEST */

#ifdef DEBUG
	compat_print_binding("Attempted to connect to:", *BindingHandle);
#endif
	for(objPtr = objectListP; objPtr; objPtr=objPtr->next)
	{
	    /* ask server about type of object */
	    scode = update_FetchObjectInfo(*BindingHandle, objPtr->name,
					  &remoteStatBuf);
	    if ( IS_COMM_ERR(scode) )
	    {
		LogError(udt_s_indeterminate_type_obj, scode, objPtr->name);

		/*
		 * If error because server is not exporting this, go to
		 * next object on list.
		 */
		if ((scode == UP_NOT_EXPORTABLE) || (scode == UP_SOFT_ERROR))
		    continue;

		goto retry;
	    }
	    /* now stat local and remote objects for type checking */
	    remoteObjectType = getObjectType(remoteStatBuf.mode);
	    if ( remoteObjectType == 0 )
	    {
		Log(udt_s_bad_remote_obj_type, objPtr->name);
		continue;
	    }

	    /* determine type of local object (if any) */
	    code = stat(objPtr->name, &localStatBuf);
	    if ( code )
	    {
		if ( errno == ENOENT )
		{
		    /* no local object exists - ok */
		    localObjectType = remoteObjectType;	/* for type checks */
		}
		else
		{
		    Log(udt_s_stat_failed_ignoring, objPtr->name, errno);
		    continue;
		}
	    }
	    else
		localObjectType = getObjectType(localStatBuf.st_mode);

	    switch ( remoteObjectType )
	    {
	      case FSTR_DIRECTORY:
		/* fetch/synchronize a directory. First check if local
		 * item is also a directory
		 */
		if ( localObjectType != FSTR_DIRECTORY )
		{
		    Log(udt_s_ignoring_remote_dir_invalid_type, objPtr->name);
		    continue;
		}

		code = fetchDirectory(objPtr, pipeStreamPtr, *BindingHandle);
		break;

	      case FSTR_FILE:
		/* fetch/synchronize a single file */

		if ( localObjectType != FSTR_FILE )
		{
                    Log(udt_s_ignor_remote_file_invalid_type, objPtr->name);
                    continue;
                }

		code = fetchFile(objPtr, pipeStreamPtr, *BindingHandle);
		break;

	      default:
		Log(udt_s_ignoring_object, objPtr->name);
		continue;
	    }

	    if ( code )
	    {
		switch ( code )
		{
		  case UP_SOFT_ERROR:
		  case UP_OPENFAIL:
		  case UP_WRITEFAIL:
		    continue;

		  case rpc_s_comm_failure:
		  default:
		    /* Abandon this pass over the list of items to be fetched.
		     */
		    goto retry;
		    break;
		}
	    }
	} /* End for each dir loop */

	/*
	 * Delete the file with info on files in directory df->name
	 */
      /* Have to disable this sleep while testing cancels because, pthread_
       * delay_np() is a cancellation point. Disabling this would let the
       * client thread not be affected by cancels from the keepCancelling
       * thread.
       */
#ifdef UPDATE_CANCEL_TEST
      DFS_DISABLE_CANCEL(&savedCancelState, code);
#endif /* UPDATE_CANCEL_TEST */
      timeOut.tv_sec = interval;
      timeOut.tv_usec = 0;
      pthread_delay_np((struct timespec *)&timeOut);
#ifdef UPDATE_CANCEL_TEST
      DFS_ENABLE_CANCEL(savedCancelState, code);
#endif /* UPDATE_CANCEL_TEST */
      continue;

retry:
      rpc_binding_reset(*BindingHandle, &status);
      if(IS_COMM_ERR(status))
	{
	  LogError(udt_s_terminating_becuz_binding_fail, status);
	  dce_svc_printf(UDT_S_UPCLIENT_RESET_BINDING_FAILED_MSG, dfs_dceErrTxt(status));
	  exit(5);
	}
#ifdef UPDATE_CANCEL_TEST
      DFS_DISABLE_CANCEL(&savedCancelState, code);
#endif /* UPDATE_CANCEL_TEST */
      timeOut.tv_sec = retrytime;
      timeOut.tv_usec = 0;
      pthread_delay_np((struct timespec *)&timeOut);
#ifdef UPDATE_CANCEL_TEST
      DFS_ENABLE_CANCEL(savedCancelState, code);
#endif /* UPDATE_CANCEL_TEST */
      /* start the cycle again */
    }
}

#ifdef UPDATE_CANCEL_TEST
int keepCancelling(threadP)
pthread_t *threadP;
{
  long code;

  while(1) {
    code = pthread_cancel(*threadP);
    if (code)
      LogError(udt_s_thread_cancel_failed, errno,
	       *threadP);
    pthread_testcancel();
  }
}
#endif /* UPDATE_CANCEL_TEST */

int main(argc,argv)
     int   argc;
     char  **argv;
{
    struct cmd_syndesc *ts;
    error_status_t       status;
    long code, i;
    rpc_binding_handle_t   BindingHandle;
    struct timeval tv;
    struct filestr *ModFiles, *okhostfiles;
    pthread_t thread1, thread2;
    pthread_addr_t thrStatus;
    uuid_t nil_uuid;

#define IsArg(a) (strncmp (arg,a, arglen) == 0)

    osi_setlocale(LC_ALL, "");
    objectListP = (struct filestr *) 0;
    ModFiles =  (struct filestr *) 0;
    okhostfiles = (struct filestr *) 0;
    verbose = 0;
    interval = TIMEOUT;


    initialize_svc();


    ts = cmd_CreateSyntax((char *) 0, uc_argHandler, (char *) 0, "upclient");
    cmd_AddParm(ts, "-server", CMD_SINGLE, CMD_REQUIRED, "machine");
    cmd_AddParm(ts, "-path", CMD_LIST, CMD_REQUIRED,
		"filename | directory_name");
    cmd_AddParm(ts, "-time", CMD_SINGLE, CMD_OPTIONAL, "frequency");
    cmd_AddParm(ts, "-file", CMD_SINGLE, CMD_OPTIONAL,"log_file");
    cmd_AddParm(ts, "-verbose", CMD_FLAG, CMD_OPTIONAL, "verbose");

    initialize_upd_error_table();

    code = cmd_Dispatch(argc, argv);
    if ( code ) {
      LogError(udt_s_cmd_line_parse_failed, code);
      dce_svc_printf(UDT_S_FAILED_TO_PARSE_CMD_MSG, dfs_dceErrTxt(code));
      goto exit;
    }

    /* check if help was requested, and if so, quit cleanly */
    for(i=0;i<argc;i++) {
      if (strncmp(argv[i], "-help", strlen(argv[i])) == 0 ||
	  strncmp(argv[i], "help", strlen(argv[i])) == 0)
	  exit(0);
    }

    if ( (objectListP == 0)
     ||   (hostEntryP == 0)
       )
    {
      LogError(udt_s_invalid_name_or_path, 0);
      dce_svc_printf(UDT_S_SERVER_NAME_OR_PATH_BAD_MSG);
      goto exit;
    }

    uuid_create_nil(&nil_uuid, (unsigned32 *)&status);
    if (status != uuid_s_ok) {
      LogError(udt_s_nil_UUID_create, status);
      dce_svc_printf(UDT_S_CREATE_NIL_UUID_FAILED_MSG, dfs_dceErrTxt(status));
      code = status;
      goto exit;
    }
    dfs_locate_rpc_server((unsigned_char_t *)hostEntryP, &BindingHandle,
			  &nil_uuid, 1, 0, &status);
    if(IS_COMM_ERR(status))
    {
	LogError(udt_s_locate_server_failed, status);
	dce_svc_printf(UDT_S_LOCATE_UPSERVER_FAILED_MSG, dfs_dceErrTxt(status));
	code = status;
	goto exit;
    }

    bzero((char *)&tv, sizeof(tv));
    tv.tv_sec = UPDATE_RETRY_TIME;
    tv.tv_usec = 0;
    while(1) {
      /* Check to see if client has access to desired interface */
      status = dfs_selectInterface (BindingHandle);
      if (status != rpc_s_comm_failure)
	break;
      LogError(udt_s_intf_access_failed, status);
      (void)select(0, 0, 0, 0, &tv);
    }

    if (IS_COMM_ERR(status)) {
      LogError(udt_s_select_rpc_intfc_failed, status);
      dce_svc_printf(UDT_S_RPC_INTFC_SELECT_FAILED_MSG, dfs_dceErrTxt(status));
      code = dfsauth_client_CleanupAuthentication();
      code = status;
      goto exit;
    }

    Log(udt_s_upclient_startec);

    /* The following code under #ifdef is to test whether cancellations by the
     * client thread affect the server thread. Two threads are created, one
     * running the update task, and the other trying to cancel the first
     * thread. The server does not get affected by these cancels even if the
     * client thread does.
     */
#ifdef UPDATE_CANCEL_TEST
    code = pthread_create(&thread1, pthread_attr_default,
			  (pthread_startroutine_t)keepUpToDate,
			  (pthread_addr_t)&BindingHandle);
    if (code) {
      LogError(udt_s_thread_spawn_failed, errno);
      dce_svc_printf(UDT_S_SPAWN_THREAD_FAILED_MSG);
      goto exit;
    }
    code = pthread_create(&thread2, pthread_attr_default,
			  (pthread_startroutine_t)keepCancelling,
			  (pthread_addr_t)&thread1);
    if (code) {
      LogError(udt_s_keep_cancelling_thread_spawn_failed, errno);
      dce_svc_printf(UDT_S_SPAWN_CANCELLING_THR_FAILED_MSG);
      goto exit;
    }

    code = pthread_join(thread1, (pthread_addr_t *)&thrStatus);
    if (code) {
      LogError(udt_s_thread_join_failed, errno);
      dce_svc_printf(UDT_S_UPTODATE_JOIN_THR_FAILED_MSG);
      goto exit;
    }
    if ((int)thrStatus != 0) { /* Things didn't go right */
      Log(udt_s_main_thread_terminating);
      dce_svc_printf(UDT_S_TERMINATING_MAIN_THREAD_MSG);
    }
    code = pthread_cancel(thread2);
    if (code) {
      LogError(udt_s_thread_cancel_failed2, errno);
      dce_svc_printf(UDT_S_CANCEL_THREAD_FAILED_MSG);
      goto exit;
    }
#else
    keepUpToDate(&BindingHandle);

#endif /* UPDATE_CANCEL_TEST */

  exit:
    exit(2);
}  /* End of Main */

/* IsCompatible
 *	Is the local file compatible (identical) with the remote file?
 * exit:
 *	1 - files compatible. Don't update local file
 *	0 - incompatible. Fetch a new copy of the file
 */

int
IsCompatible(filename, time, lengthp)
     char *filename;
     long time;
     afs_hyper_t *lengthp;
{
    struct stat status;
    long error;

    error = stat(filename,&status);

    /*
     * A non-existent file, has to be fetched fresh.
     */
    if (error == -1)
        return 0;

    /* why return true if it is a directory? No one uses this feature PA */
    if((status.st_mode & S_IFMT) == S_IFDIR ||
       ((status.st_mtime == time) &&
	AFS_hcmp64(*lengthp, 0, status.st_size) == 0))
        return(1);
    else
        return 0;
}

/* NotOnHost
 *	checks client file, filename, against list of files obtained from
 *	server, okhostfiles.
 * exit:
 *	1 - delete file on client. Not present on server.
 *	0 - both client and server have this file.
 * Notes:
 * 1) Why does it return true for the directory case?
 */

int NotOnHost(filename, okhostfiles)
     char *filename;
     struct filestr *okhostfiles;
{
    int i;
    struct stat status;
    struct filestr *tf;

    stat(filename,&status);

    if((status.st_mode & S_IFMT) == S_IFDIR)
	return 0;
    i = strlen(filename);
    if(!strcmp(&filename[i-4],".NEW"))
	return 0;

    for(tf=okhostfiles; tf; tf=tf->next) {
	if (!strcmp(tf->name, filename))
	    return 0;
    }
    return 1;
}


int
AddToList(ah, aname, type)
     struct filestr **ah;
     char *aname;
     int type;
{
    register struct filestr *tf;
    tf = (struct filestr *) malloc(sizeof(struct filestr));
    tf->next = *ah;
    *ah = tf;
    tf->name = (char *) malloc(strlen(aname)+1);
    strcpy(tf->name, aname);
    tf->type = type;
    return 0;
}

/* isCompatibleObject
 *      Is the named item compatible with previously specified items.
 *      Rules:
 *      1) Can't specify subsets or supersets of previous items
 * entry:
 *      itemP - linked list of items. Object compatibility is checked against
 *              this.
 *      nameP - pathname of object to check
 * exit:
 *      1 - yes
 *      0 - no. Reason logged in logfile.
 */

long
isCompatibleObject(itemP, nameP)
     struct filestr *itemP;
     char *nameP;
{

    while ( itemP )
    {
	if ( compatibleObjects(nameP, itemP->name) == 0 )
	{
	    Log(udt_s_incompat_obj_ignored, nameP, itemP->name);
	    return(0);
        }
        itemP = itemP->next;
    }
    return(1);
}

int ZapList(ah)
     struct filestr **ah;
{
    register struct filestr *tf, *nf;
    for(tf = *ah; tf; tf=nf) {
	nf = tf->next;	    /* save before freeing */
	free(tf->name);
	free(tf);
    }
    *ah = (struct filestr *) 0;
    return 0;
}

/*
 *
 *           Pipe Stream Definition
 *
 */

void pipeAlloc(state, bsize, buf, bcount)
  /*in */  void *state;
  /*in */  ndr_ulong_int bsize;
  /*out*/  ndr_byte **buf;
  /*out*/  ndr_ulong_int *bcount;
{
    static char buffer[PREFER_BUFFSIZE];
    /*
     * NOTE: PREFER_BUFFSIZE is currently defined by NIDL_PIPE_BUFF_SIZE (which
     *       is alleged to be optimal for transmit)
     *
     *       But, it may be a best size for performing the I/Os. ??
     */
    *buf = (ndr_byte *)buffer;
    *bcount = PREFER_BUFFSIZE;
}


void ReceiveFile(state, buf, ecount)
  /*in*/   void *state;            /* the file stream as the target file */
  /*in*/   ndr_byte *buf;          /* pointer to the buffer containing data */
  /*in*/   ndr_ulong_int ecount;   /* number of bytes to be written */
{
    long written;
    written = fwrite((char *)buf, sizeof(ndr_byte), ecount, (FILE *)state);
    if ( written != sizeof(ndr_byte)*ecount)
	Log(udt_s_short_write, written);
}



