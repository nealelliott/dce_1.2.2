/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: server.c,v $
 * Revision 1.1.897.1  1996/10/02  21:05:48  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:00  damon]
 *
 * $EndLog$
 */
/*
 *      Copyright (C) 1991, 1995 Transarc Corporation
 *      All rights reserved.
 */

/*
 *       UPDATE Server (AFS 4.0) Server Application
 */


#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/compat.h>
#include <dcedfs/dfsauth.h>
#include <dcedfs/cmd.h>
#include <dirent.h>

#include <update.h>
#include <upcommon.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsudtmac.h>
#include <dfsudtsvc.h>
#include <dfsudtmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/update/server.c,v 1.1.897.1 1996/10/02 21:05:48 damon Exp $")

#define UPDATE_MAXCALLS 5
#define DEFAULT_ADMIN_FILENAME	"admin.up"

char *whoami = "upserver";


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



/* synchronization objects */
pthread_mutex_t updateMutex;

/* These variables should be protected by a lock when they get to be used
 * again
 */
int   nDirs;
char  *dirName[10];
int   dirLevel[10];
upitemP itemListHeadP = 0;	/* list of objects the server can export */
char    adminFilename[BUFSIZ];
upLogP  logP = 0;		/* upserver's handle on it's log file */

void
updatePthreadInit ()
{
    pthread_mutexattr_t updateMutexAttr;

    if (pthread_mutexattr_create (&updateMutexAttr) != 0)
    {
	Log(udt_s_mutex_attr_create_error, errno);
	exit(1);
    }

    if (pthread_mutexattr_setkind_np(&updateMutexAttr,
				     MUTEX_RECURSIVE_NP) != 0)
    {
	Log(udt_s_mutex_attr_setkind_failed, errno);
	exit(1);
    }

    if (pthread_mutex_init(&updateMutex, updateMutexAttr) != 0)
    {
	Log(udt_s_init_server_mutex_error, errno);
	exit(1);
    }
    if (pthread_mutexattr_delete(&updateMutexAttr) != 0)
    {
	Log(udt_s_delete_mutex_attr_error, errno);
	exit(1);
    }
}

/* isCompatibleObject
 *	Is the named item compatible with previously specified items.
 *	Rules:
 *	1) Can't specify subsets or supersets of previous items
 * entry:
 *	itemP - linked list of items. Object compatibility is checked against
 *		this.
 *	nameP - pathname of object to check
 * exit:
 *	1 - yes
 *	0 - no. Reason logged in logfile.
 */

long
isCompatibleObject(itemP, nameP)
     upitemP itemP;
     char *nameP;
{
    long itemLength, nameLength, minLength;
    char *cptr;

    while ( itemP )
    {
	if ( compatibleObjects(nameP, itemP->name) == 0 )
	{
	    Log(udt_s_incompat_obj_ignored2, nameP, itemP->name);
	    return(0);
	}
	itemP = itemP->next;
    }
    return(1);
}

/* addServerItem
 *	Add an item to the server's exportable object list. Items may be
 *	files or directories.
 * entry:
 *	nameP - pathname of object
 */

long
addServerItem(nameP)
     char *nameP;
{
    struct stat statbuf;
    upitemP itemP = 0;
    char *pathNameP = 0;
    long fileType;
    long code = 0;

    if ( nameP == 0 )				/* must have a valid path */
    {
	Log(udt_s_null_pathname);
	return(0);
    }

    /* stat the object */
    code = stat(nameP, &statbuf);
    if ( code )
    {
	Log(udt_s_export_item_failed, nameP);
	return(0);
    }

    /* now check if the object has a reasonable type */
    if ( statbuf.st_mode & S_IFREG )
	fileType = FSTR_FILE;
    else
    if ( statbuf.st_mode & S_IFDIR )
	fileType = FSTR_DIRECTORY;
    else
    {
	/* object type is not acceptable */
	Log(udt_s_obj_not_file_or_dir, nameP);
	return(0);
    }

    /* now check if this object is compatible with previously specified
     * objects.
     */
    if ( isCompatibleObject(itemListHeadP, nameP) == 0 )
	return(0);

    /* allocate the item descriptor */
    itemP = (upitemP) malloc(sizeof(*itemP));
    if ( itemP == 0 )
	goto error;

    pathNameP = (char *) malloc(strlen(nameP)+1);
    if ( pathNameP == 0 )
	goto error;

    strcpy(pathNameP, nameP);

    /* add it to the object chain */
    itemP->next = itemListHeadP;
    itemListHeadP = itemP;

    itemP->name = pathNameP;			/* attach pathname */
    itemP->level = 0;				/* TBD */
    itemP->type = fileType;
    return(0);

error:
    Log(udt_s_add_server_malloc_failed);
    if ( itemP )
	free(itemP);
    exit(1);
}

/* canExportObject
 *	check if a file or directory can be exported by the upserver
 * exit:
 *	1 - yes, allowed to export
 *	0 - no
 */

canExportObject(nameP)
     char *nameP;
{
    upitemP ptr;
    char *cptr;
    unsigned int pathLength;

    ptr = itemListHeadP;		/* list of exportable items */
    while ( ptr != 0 )
    {
	/* exact match - dir or file exportable */
	if ( strcmp(ptr->name, nameP) == 0 )
	    return(1);

	pathLength = strlen(ptr->name);

	/* if path is shorter than pathname of stored object, continue
	 * checking against next item on list
	 */
	if ( strlen(nameP) < pathLength )
	{
	    ptr = ptr->next;
	    continue;
	}

	if ( strncmp(nameP, ptr->name, pathLength) == 0 )
	{
	    /* special case for the root */
	    if ( strcmp(ptr->name, "/") == 0 )
		return(1);

	    /* nameP is a child of stored item. Ensure that nameP
	     * is actually a child and not just a longer filename
	     */
	    cptr = nameP + pathLength;
	    if ( *cptr == '/' )
		return(1);
	}
	ptr = ptr->next;
    }
    return(0);
}

/* us_argHandler
 *	update server argument handler
 */

int
us_argHandler(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    struct cmd_item *cmdItemPtr;
    char *cptr;
    long code = 0;

    /* -path option */
    if (as->parms[0].items)
    {
	/* list of objects that the server may export */
	cmdItemPtr = as->parms[0].items;
	while ( cmdItemPtr )
	{
	    stripTrailingSlash(cmdItemPtr->data);
	    addServerItem(cmdItemPtr->data);
	    cmdItemPtr = cmdItemPtr->next;
	}
    }

    /* -adminfile */
    if (as->parms[1].items)
	(void) strcpy(adminFilename, as->parms[1].items->data);
    else
	(void) strcpy(adminFilename, DEFAULT_ADMIN_FILENAME);

    return(0);
}

/* us_argSetup
 *	initialize the command line arguments to the upserver
 */

us_argSetup()
{
    /* for command handling */
    struct cmd_syndesc *ts;

    ts = cmd_CreateSyntax((char *) 0, us_argHandler, 0, "upserver");
    cmd_AddParm(ts, "-path", CMD_LIST, CMD_REQUIRED,
		"filename | directory_name");
    cmd_AddParm(ts, "-adminlist", CMD_SINGLE, CMD_OPTIONAL,
		"filename");
}

/* initUpserverLog
 *	initialize the log structure and open the upserver's log file.
 *	Logging will work upon exit.
 */

initUpserverLog()
{
    int logPathLength;
    char *oldLogfile;

    extern upLogP logP;

    /* allocate the log structure and clear it */
    logP = (upLogP) malloc(sizeof(upLogT));
    if ( logP == 0 )
    {
	dce_svc_printf(UDT_S_INIT_LOGFILE_FAILED_MSG);
	exit(1);
    }
    bzero((char *)logP, sizeof(*logP));

    logPathLength = strlen(DCELOCAL_PATH)+strlen(LOGPATH)+strlen(LOGFILE)+1;

    /* want to save the path for the log file */
    logP->logfile = (char *) malloc(logPathLength);
    if ( logP->logfile == 0 )
    {
	dce_svc_printf(UDT_S_INIT_LOGFILE_FAILED2_MSG);
	exit(1);
    }

    strcpy(logP->logfile, DCELOCAL_PATH);
    strcat(logP->logfile, LOGPATH);
    strcat(logP->logfile, LOGFILE);

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
	dce_svc_printf(UDT_S_OPEN_LOGFILE_FAILED_MSG, logP->logfile);
	exit(1);
    }
}

int
main(argc, argv)
   int   argc;
   char  *argv[];
{

/* Declarations needed by the Security Stuff */
    long		dfsauthRtnVal;
    error_status_t	st;
    static char		routineName[] = "main";
    long		code = 0;
    int			i;
    uuid_t		nil_uuid;

    extern char *dfs_dceErrTxt();
    extern UPDATE_v4_0_epv_t UPDATE_v4_0_manager_epv;

    osi_setlocale(LC_ALL, "");


    initialize_svc();


    initialize_upd_error_table();

    us_argSetup();
    code = cmd_Dispatch(argc, argv);
    if ( code )
      exit(1);

    /* check if this was simply a help request */
    for (i=0;i<argc;i++) {
      if (strncmp(argv[i], "-help", strlen(argv[i])) == 0)
	exit(0);
    }

    initUpserverLog();
    updatePthreadInit();

    code = compat_ShutdownDuplicateServer ((rpc_if_handle_t)UPDATE_v4_0_s_ifspec,
					   (uuid_t *)NULL,
					   0);
    if ( code) {
	LogError(udt_s_dup_server_shutdown_failed, code);
	goto exit;
    }

    /* recording the above registered interface */
    dfs_installInterfaceDescription ((rpc_if_handle_t)UPDATE_v4_0_s_ifspec,
				     (rpc_if_handle_t)UPDATE_v4_0_s_ifspec,
				     1, /* provider_version number */
				     (unsigned_char_t *)"Transarc Update Server Interface",
				     &st);
    if (IS_COMM_ERR(st))
    {
	LogError(udt_s_install_reg_intfc_failed, st);
	code = st;
	goto exit;
    }

    uuid_create_nil(&nil_uuid, &st);
    if (st != uuid_s_ok) {
      LogError(udt_s_nil_UUID_create_failed, st);
      goto exit;
    }

    dfs_register_rpc_server ((rpc_if_handle_t)UPDATE_v4_0_s_ifspec,
			     (rpc_mgr_epv_t)&UPDATE_v4_0_manager_epv,
			     (uuid_t *)&nil_uuid, (uuid_t *)&nil_uuid,
			     UPDATE_MAXCALLS, adminFilename,
			     "DFS upserver", &st);
    if (IS_COMM_ERR(st))
    {
	LogError(udt_s_servr_regist_failed, st);
	code = st;
	goto exit;
    }

    Log(udt_s_upsrvr_started);

    rpc_mgmt_set_server_com_timeout(rpc_c_binding_default_timeout+2, &st);
    rpc_server_listen (UPDATE_MAXCALLS, &st);
    if (IS_COMM_ERR(st))
    {
	LogError(udt_s_srvr_listen_failed, st);
	code = st;
	goto exit;
    }

    code = compat_UnregisterServer ((rpc_if_handle_t)UPDATE_v4_0_s_ifspec,
				    (uuid_t *)NULL);
    if (code) {
      LogError(udt_s_unregister_intfc_failed, code);
      goto exit;
    }
    else exit(0);

exit:
    exit(2);
}
