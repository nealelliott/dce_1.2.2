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
 * Revision 1.1.25.1  1996/10/02  17:03:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:31  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * server.c -- bakserver startup
 */
#include <dcedfs/stds.h>  /* include appropriate standards file */
#include <stdlib.h>

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <pthread.h>
#include <dce/rpc.h>

#include <dcedfs/cmd.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/dfsauth.h>
#include <dcedfs/lock.h>
#include <dcedfs/icl.h>

#include <signal.h>
#include <error_macros.h>
#include <budb_errs.h>
#include <database.h>
#include <globals.h>
#include <com_err.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bakserver/server.c,v 1.1.25.1 1996/10/02 17:03:01 damon Exp $")

/* The following #define is for testing purposes and should not be
 * used under normal circumstances.
 */
/*
#define allow_multiple_bakservers
*/
#define BUDB_MAXCALLS 32

extern long ubik_lastYesTime;
extern long ubik_nBuffers;
/* some messy problems if ubikdisk_proc.h and ubikvote_proc.h are included */
extern rpc_if_handle_t UBIKDISK_v4_0_s_ifspec;
extern rpc_if_handle_t UBIKVOTE_v4_0_s_ifspec;

static char *nsGroupP;
static char *nsSuffix=(char *)NULL;
static char *adminFile=(char *)NULL;

/* server's global configuration information. This is exported to other
 * files/routines
 */
buServerConfT	globalConf;
buServerConfP	globalConfPtr = &globalConf;

/* Debugging control */
int verbose=0;

static void AbortDumpHandler(sig)
int sig;
{
    (void) fprintf(stderr, "AbortDumpHandler called\n");
    icl_DumpUser();
    abort();
}

/* define event set structure for tracing and setup function */
struct icl_set *bak_iclSetp = 0;
static void SetupICL()
{
    struct icl_log *logp;
    long code;
    static int Initted = 0;

    if (Initted)
	return;
    Initted = 1;

    code = icl_ExportIF("bakserver");
    if (code)
      (void) fprintf(stderr, "could not export ICL interface, code=%d\n",code);

    code = icl_StartCatcher(0);
    if (code < 0)
      (void) fprintf(stderr,
		     "could not start ICL signal watcher, code=%d, errno=%d\n",
		     code, errno);
    else
    {
      code = icl_CreateLog("common", 30*1024, &logp);
      if (code == 0)
	code = icl_CreateSet("bakserver",
			     logp, (struct icl_log *) 0,
			     (struct icl_set **)&bak_iclSetp);
	if (code)
	  fprintf(stderr,
	  "bakserver: warning: can't initialize ICL tracing, code %d\n", code);
	else
	{
#ifdef AFS_SIGACTION_ENV
#define SIGFUNC sigaction
	    struct sigaction	sv;

	    /* set up sigaction stuff now since CMA won't let us catch these */
	    bzero((char *)&sv, sizeof(sv));
	    sv.sa_handler = AbortDumpHandler;
#else
#define SIGFUNC sigvec
	    struct sigvec	sv;

	    /* set up sigaction stuff now since CMA won't let us catch these */
	    bzero((char *)&sv, sizeof(sv));
	    sv.sv_handler = AbortDumpHandler;
#if 0
	    sv.sv_flags = SA_OLDSTYLE;
#endif /* 0 */
#endif /* AFS_SIGACTION_ENV */

	    if (SIGFUNC(SIGBUS, &sv, NULL) < 0)
	    {
		(void) fprintf(stderr,
			       "unable to catch SIGBUS, errno = %d\n", errno);
	    }
	    if (SIGFUNC(SIGSEGV, &sv, NULL) < 0)
	    {
		(void) fprintf(stderr,
			       "unable to catch SIGSEGV, errno = %d\n", errno);
	    }
	    if (SIGFUNC(SIGILL, &sv, NULL) < 0)
	    {
		(void) fprintf(stderr,
			       "unable to catch SIGILL, errno = %d\n", errno);
	    }
	}
    }
}

/* initializeCommands
 *	initialize all the supported commands and their arguments
 */

initializeArgHandler()
{
    struct cmd_syndesc *cptr;

    int argHandler();

    cptr = cmd_CreateSyntax((char *) 0, argHandler, (char *)0,
                            "Backup database server");

    cmd_AddParm(cptr, "-adminlist", CMD_SINGLE, CMD_OPTIONAL,
                "filename");
    cmd_AddParm(cptr, "-verbose", CMD_FLAG, CMD_OPTIONAL, "debug mode");

#ifdef allow_multiple_bakservers
    cmd_AddParm(cptr, "-bakgroup", CMD_SINGLE, CMD_OPTIONAL,
                "server_group");
#endif /* allow_multiple_bakservers */
}

argHandler(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    char *cellName, *bakGroup, *curptr;
    error_status_t code=0;
    char sfxBuf[200];

    /* admin list file */
    if ( as->parms[0].items && as->parms[0].items->data )
    {
	adminFile = (char *)osi_Alloc((strlen(as->parms[0].items->data)+1)*
				   sizeof(char));
	if ( adminFile == (char *)NULL ) {
	    LogError(BUDB_NOMEM, "argHandler: Memory Allocation Failure\n");
	    return (BUDB_NOMEM);
	}
	strcpy (adminFile, as->parms[0].items->data);
    }
    else
    {
	adminFile = (char *)osi_Alloc((strlen(DEFAULT_ADMIN_FILENAME)+1)*
				   sizeof(char));
	if ( adminFile == (char *)NULL ) {
	    LogError(BUDB_NOMEM, "argHandler: Memory Allocation Failure\n");
	    return(BUDB_NOMEM);
	}
	strcpy (adminFile, DEFAULT_ADMIN_FILENAME);
    }
    /* verbose */
    if ( as->parms[1].items ) {
      verbose = 1;	/* scale of 1 to 10; print with full verbosity */
      Log("bakserver starting in verbose mode; printing messages to stderr\n");
      LogDebug(1,
	  "bakserver starting in verbose mode; printing messages to stderr\n");
    }
    else
      verbose = 0;

#ifdef allow_multiple_bakservers
    /* server group name */
    if (as->parms[2].items && as->parms[2].items->data)
      bakGroup = as->parms[2].items->data;
    else if(bakGroup = getenv("DFS_BAKSERVER_GROUP_NAME"))
      ;
    else
      bakGroup = (char *)NULL;

    if (bakGroup)
    {
	dce_cf_get_cell_name(&cellName, &code);
	if (!code) {
	  if(((strncmp(bakGroup,cellName,strlen(cellName))==0
	    &&(curptr=(char *)&(bakGroup[strlen(cellName)])))
	    || (strncmp(bakGroup, "/.:", strlen("/.:"))==0
	    && (curptr = (char *)&(bakGroup[strlen("/.:")]))))
	    && strncmp(curptr,"/subsys/dce/dfs/",strlen("/subsys/dce/dfs/"))==0
	    && strchr((char *)&(curptr[strlen("/subsys/dce/dfs/")]),
		      '/') == (char *)NULL) {
	    nsGroupP = (char *) osi_Alloc((strlen(bakGroup)+1)*
					  sizeof(char));
	    if ( nsGroupP == (char *)NULL ) {
	      LogError(BUDB_NOMEM, "argHandler: Memory Allocation Failure\n");
	      return(BUDB_NOMEM);
	    }
	    strcpy(nsGroupP, bakGroup);
	    dfs_GetBakJunctionName ((char *)cellName, (char *)sfxBuf,
				    (int)sizeof(sfxBuf),
				    (unsigned32 *)&code);
	    if ((unsigned32)code != 0) {
	      LogError(code,
		       "argHandler: Unable to determine bak junction name for\
 cell %s; %s\n",
		       cellName, dfs_dceErrTxt(code));
	      return(code);
	    }
	    if (strcmp(curptr, sfxBuf) != 0) {
	      nsSuffix = (char *)osi_Alloc(strlen(bakGroup)+1);
	      if ( nsSuffix == (char *)NULL ) {
		LogError(BUDB_NOMEM,"argHandler: Memory Allocation Failure\n");
		return(BUDB_NOMEM);
	      }
	      strcpy(nsSuffix,
		     (char *)&(curptr[strlen("/subsys/dce/dfs/")]));
	    }
	    else
	      nsSuffix = (char *)NULL;
	  }
	  else {
	    LogError(0, "argHandler: bakserver group name not adhering to \
convention /.../<cell name>/subsys/dce/dfs/<group name>\n",
		     bakGroup);
	    fprintf(stderr,"bakserver group name %s not adhering to convention\
 /.../<cell name>/subsys/dce/dfs/<group name>\n",
		    bakGroup);
	    return(BUDB_BADGROUP);
	  }
	}
	else {
	  LogError(code, "argHandler: failed to obtain cell name\n");
	  return(code);
	}
    }
    else
    {
#endif /* allow_multiple_bakservers */
      dce_cf_get_cell_name(&cellName, &code);
      if (!code) {
	dfs_GetBakJunctionName ((char *)cellName, (char *)sfxBuf,
				(int)sizeof(sfxBuf), (unsigned32 *)&code);
	if (!code && strcmp(sfxBuf, "") != 0) {
	  nsGroupP = (char *) osi_Alloc((strlen(cellName)+strlen(sfxBuf)+1)*
					sizeof(char));
	  if (nsGroupP)
	    sprintf(nsGroupP, "%s%s", cellName, sfxBuf);
	  else {
	    LogError(BUDB_NOMEM, "argHandler: Memory Allocation Failure\n");
	    return(BUDB_NOMEM);
	  }
	}
	else {
	  LogError(code,"argHandler: Could not get bakserver junction name \
for %s, %s\n",
		   cellName, dfs_dceErrTxt(code));
	  return(code);
	}
	(void)free(cellName);/*we didn't allocate this, don't call osi_Free*/
      }
      else {
	LogError(code,"argHandler: Cell Name could not be determined, %s\n",
		 dfs_dceErrTxt(code));
	return(code);
      }
#ifdef allow_multiple_bakservers
    }
#endif /* allow_multiple_bakservers */

    return 0;
}

static void initBakLog(myName)
char *myName;
{/* Initialize the log service. */
    char *logFile, *ologFile;

    /* set default configuration values */
    if (nsSuffix)
      logFile = (char *)osi_Alloc ((strlen(DCELOCAL_PATH)+
				    strlen(LOGPATH)+
				    strlen(DEFAULT_LOGNAME)+
				    strlen(nsSuffix)+1+
				    1)*sizeof(char));
    else
      logFile = (char *)osi_Alloc ((strlen(DCELOCAL_PATH)+
				    strlen(LOGPATH)+
				    strlen(DEFAULT_LOGNAME)+
				    1)*sizeof(char));

    if (!logFile)  {
      fprintf(stderr,
	      "%s: Can't open log file %s, memory allocation failure\n",
	      DEFAULT_LOGNAME);
      exit(1);
    }
    if (nsSuffix)
      ologFile = (char *)osi_Alloc ((strlen(DCELOCAL_PATH)+
				     strlen(LOGPATH)+
				     strlen(DEFAULT_LOGNAME)+
				     strlen(nsSuffix)+1+
				     5)*sizeof(char));
    else
      ologFile = (char *)osi_Alloc ((strlen(DCELOCAL_PATH)+
				     strlen(LOGPATH)+
				     strlen(DEFAULT_LOGNAME)+
				     5)*sizeof(char));
    if (!ologFile)  {
      fprintf(stderr,
	      "%s: Can't open log file %s, memory allocation failure\n",
	      DEFAULT_LOGNAME);
      exit(1);
    }
    strcpy(logFile, DCELOCAL_PATH);
    strcat(logFile, LOGPATH);
    strcat(logFile, DEFAULT_LOGNAME);
    if (nsSuffix) {
      strcat(logFile, ".");
      strcat(logFile, nsSuffix);
    }
    strcpy(ologFile, logFile);
    strcat(ologFile, ".old");
    rename(logFile, ologFile);	/* don't check error code */
    globalConfPtr->log = fopen(logFile, "w");
    if (!(globalConfPtr->log)) {
	fprintf(stderr, "%s: open-to-write failed on %s\n",
		myName, logFile);
	exit(1);
    }
    Log("%s: log initialized to %s\n", myName, logFile);
    if (nsSuffix)
      (void)osi_Free(logFile,
	   strlen(DCELOCAL_PATH)+strlen(LOGPATH)+strlen(DEFAULT_LOGNAME)+\
		     strlen(nsSuffix)+1+1);
    else
      (void)osi_Free(logFile,
	   strlen(DCELOCAL_PATH)+strlen(LOGPATH)+strlen(DEFAULT_LOGNAME)+1);
    if (nsSuffix)
      (void)osi_Free(ologFile,
	   strlen(DCELOCAL_PATH)+strlen(LOGPATH)+strlen(DEFAULT_LOGNAME)+\
		     strlen(nsSuffix)+1+5);
    else
      (void)osi_Free(ologFile,
	   strlen(DCELOCAL_PATH)+strlen(LOGPATH)+strlen(DEFAULT_LOGNAME)+5);
}

/* thread to process SIGINT and SIGTERM */
void KillHandler(arg)
void *arg;
{
    sigset_t signalWaitSet;
    int caught;
    
    (void)sigemptyset(&signalWaitSet);
    (void)sigaddset(&signalWaitSet, SIGINT);
    (void)sigaddset(&signalWaitSet, SIGTERM);
    caught = sigwait(&signalWaitSet);
    exit(0);
}

main(argc, argv)
     int   argc;
     char *argv[];

{
    char *whoami = argv[0];
    char *dbNamePtr = 0;
    long  currentTime;
    long  code = 0;
    uuid_t object_uuid;

    struct sockaddr_in newMyHost;
    struct hostent *th;
    int i;
    char hostname[MAXHOSTNAMELEN];
    struct stat statbuf;
    pthread_attr_t threadAttr;
    pthread_t serverInitThr;
    pthread_t killHandlerThr;
    sigset_t signalBlockSet;

    extern BUDB_v4_0_epv_t BUDB_v4_0_manager_epv;
    extern dumpSyncP dumpSyncPtr;
    extern long InitProcs();

    osi_setlocale(LC_ALL, "");
    /*
     * block SIGUSR1 for all threads. The ICL thread that wiil be
     * waiting for it will clear its mask by using sigwait().
     * block SIGINT and SIGTERM for all threads, the kill handler
     * will clear their masks using sigwait().
     */
    (void)sigemptyset((sigset_t *)&signalBlockSet);
    (void)sigaddset((sigset_t *)&signalBlockSet, SIGUSR1);
    (void)sigaddset((sigset_t *)&signalBlockSet, SIGINT);
    (void)sigaddset((sigset_t *)&signalBlockSet, SIGTERM);
    (void)sigprocmask(SIG_BLOCK, (sigset_t *)&signalBlockSet,
		      (sigset_t *)NULL);
    SetupICL();

    initialize_cmd_error_table();
    initializeArgHandler();
    /*
     * do the dispatch first, so we can service help requests early and exit
     * without having to setup all the overheads. This should also avoid the
     * problem where bakserver -help was done on a machine that isn't
     * configured to run the DCE
     */
    code = cmd_Dispatch (argc, argv);
    if ( code )
      ERROR(code);
    /* if a help was issued, then give help and get out */
    for (i=0; i<argc; i++) {
      if (strncmp (argv[i], "-help", strlen(argv[i])) == 0)
	exit(0);
    }

    bzero((char *)globalConfPtr, sizeof(*globalConfPtr));
    /* initialize the bakserver log file */
    initBakLog(whoami);

    globalConfPtr->databaseDirectory=(char *)osi_Alloc ((strlen(DCELOCAL_PATH)+
							strlen(DEFAULT_DBPATH)+
							 1)*sizeof(char));
    if (globalConfPtr->databaseDirectory == (char *)NULL) {
      printf("Can't open log file %s, memory allocation failure - aborting\n",
	     DEFAULT_LOGNAME);
      ERROR(BUDB_NOMEM);
    }
    strcpy(globalConfPtr->databaseDirectory, DCELOCAL_PATH);
    strcat(globalConfPtr->databaseDirectory, DEFAULT_DBPATH);

    /* Now check if this directory exists. If not, try to create  it */

    if ((stat(globalConfPtr->databaseDirectory, &statbuf) < 0) ||
	(statbuf.st_mode & S_IFMT) != S_IFDIR){
          /* if there is trouble finding the entry, or it is not a directory */

      (void) unlink(globalConfPtr->databaseDirectory);

      (void) mkdir(globalConfPtr->databaseDirectory, 777);
      /* if the mkdir failes, then the ubik server init code will catch
	 the error later on */

    }

    if (nsSuffix) {
     globalConfPtr->databaseName=(char *)osi_Alloc(strlen(DEFAULT_DBPREFIX)+\
						   strlen(nsSuffix)+1+1);
     if (globalConfPtr->databaseName == (char *)NULL) {
      printf("Can't open log file %s, memory allocation failure - aborting\n",
	     DEFAULT_LOGNAME);
      ERROR(BUDB_NOMEM);
     }
     strcpy(globalConfPtr->databaseName, DEFAULT_DBPREFIX);
     strcat(globalConfPtr->databaseName, ".");
     strcat(globalConfPtr->databaseName, nsSuffix);
    }
    else {
      /*
       *  Undocumented and unsupported means to change the name of the
       *  database name prefix.
       *  stdlib.h is included above for declaration of getenv()
       */
      if((globalConfPtr->databaseName = getenv("_DFS_BAK_DB_PREFIX_"))==NULL) {
	globalConfPtr->databaseName=(char *)osi_Alloc(
						 strlen(DEFAULT_DBPREFIX)+1);
	if (globalConfPtr->databaseName == (char *)NULL) {
	  printf("Can't open log file %s, \
memory allocation failure - aborting\n",
		 DEFAULT_LOGNAME);
	  ERROR(BUDB_NOMEM);
	}
	strcpy(globalConfPtr->databaseName, DEFAULT_DBPREFIX);
      }
    }
    gethostname(hostname, sizeof(hostname));
    th = gethostbyname(hostname);
    if (!th)
    {
      code = errno;
      fprintf(stderr, "bakserver: couldn't get address of this host (%s)\n",
	     hostname);
      ERROR(code);
    }
    rpcx_sockaddr_from_ipaddr(th->h_addr, &newMyHost);
    ubik_nBuffers = BUDB_UBIK_N_BUFFERS;

    dbNamePtr = (char *) osi_Alloc(strlen(globalConfPtr->databaseDirectory) +
                           strlen(globalConfPtr->databaseName) + 1);
    if ( dbNamePtr == 0 )
        ERROR(BUDB_NOMEM);

    /* construct the database name */
    strcpy(dbNamePtr, globalConfPtr->databaseDirectory);
    strcat(dbNamePtr, globalConfPtr->databaseName);	/* name prefix */

    code = (long)dfsauth_server_InitAuthentication ((char *)NULL,
					      adminFile,
					      (char *)NULL);
    if (code) {
	fprintf(stderr, "bakserver: failed to init authentication subsystem; %s\n",
		dfs_dceErrTxt(code));
	LogError(code, "Can't init authentication: code %lu %s\n",
		 code, dfs_dceErrTxt(code));
	ERROR(code);
    }

    if (adminFile)
      (void)osi_Free(adminFile, (strlen(DEFAULT_ADMIN_FILENAME)+1)*sizeof(char));

    dfs_installInterfaceDescription (BUDB_v4_0_s_ifspec, BUDB_v4_0_s_ifspec,
				     BUDB_PROVIDER_VERSION,
				     (unsigned_char_t *)"Transarc Bak Server Interface",
				     (error_status_t *)&code);
    if (code) {
	LogError(code, "Can't install interface description: %lu %s\n",
		 code, dfs_dceErrTxt(code));
	fprintf(stderr, "bakserver: Can't install interface description (%s); \
Exiting.\n", dfs_dceErrTxt(code));
	ERROR(code);
    }

    code = ubik_ServerInit(&newMyHost, BUDB_v4_0_s_ifspec,
			   (rpc_mgr_epv_t)&BUDB_v4_0_manager_epv,
			   (unsigned_char_t *)nsGroupP,
			   (unsigned_char_t *)NULL,
			   (unsigned_char_t *)"subsys/dce/dfs-bak-servers",
			   (uuid_t *)&Budb_UbikObjectTypeUuid,
			   (uuid_t *)&object_uuid,
			   BUDB_MAXCALLS, dbNamePtr, "DFS Bakserver",
			   (struct ubik_dbase **)&(globalConfPtr->database));
    if (code)
    {
	fprintf(stderr, "bakserver: Ubik init failed with code %d\n", code);
	LogError(code, "Can't init Ubik: code %lu  %s\n",
		 code, dfs_dceErrTxt(code));
	ERROR(code);
    }
    if (nsGroupP)
     (void)osi_Free(nsGroupP,strlen(nsGroupP)+1);

    /* Initialize the dump synchronization lock. Can't do this in BUDB_DumpDB
     * because it is called repetetively by BUTC
     */
    bzero((char *)dumpSyncPtr, sizeof(dumpSyncT));
    lock_Init(&(dumpSyncPtr->ds_lock));

    /*
     * Our server initialization has to be done in parallel with listening for
     * incoming RPCs. This is crucial to get the ubik synchronization process
     * started up as soon as possible. Incoming RPCs for bakserver interface
     * have to call the InitRPC routine that returns UNOQUORUM until the
     * server init has completed. The indication that the server has still not
     * completed initialization is done using the startupLock and the
     * procsInited variable.
     */
    /* create & init attr values needed by the threads */
    if (pthread_attr_create(&threadAttr) != 0) {
      LogError(BUDB_THREAD, "failed to initialize database\n");
      ERROR(BUDB_THREAD);
    }
    code = pthread_create(&serverInitThr, threadAttr,
			  (pthread_startroutine_t)InitProcs,
			  (pthread_addr_t)NULL);
    if(code) {
      LogError(BUDB_THREAD, "failed to initialize database\n");
      ERROR(BUDB_THREAD);
    }

    /* start a thread to handle SIGINT and SIGTERM interrupts. */
    code = pthread_create(&killHandlerThr, threadAttr,
			  (pthread_startroutine_t)KillHandler,
			  (pthread_addr_t)NULL);
    if(code) {
      LogError(BUDB_THREAD, "failed to start kill handler\n");
      ERROR(BUDB_THREAD);
    }

    currentTime = time(0);
    Log("Ready to process requests at %s\n", ctime(&currentTime));

    rpc_mgmt_set_server_com_timeout(rpc_c_binding_default_timeout+2,
				    (error_status_t *) &code);
    rpc_server_listen (rpc_c_listen_max_calls_default,(error_status_t *)&code);
    if (IS_COMM_ERROR(code)) {
        LogError(code, "Bakserver listen failed, %s\n",
		dfs_dceErrTxt(code));
	ERROR(code);
    }
    /*
     * if there is a duplicate server, rpc_server_listen should return
     * error_status_ok
     */
    code = compat_UnregisterServer ((rpc_if_handle_t)BUDB_v4_0_s_ifspec,
				    (uuid_t *)&object_uuid);
    if (code) {
	LogError(code, "Bakserver failed to unregister itself, %s\n",
		 dfs_dceErrTxt(code));
	ERROR(code);
    }

    /* Have to unregister the two ubik interfaces also */
    code = compat_UnregisterServer (UBIKDISK_v4_0_s_ifspec,
				    (uuid_t *)&object_uuid);
    if (code) {
	LogError(code, "Bakserver failed to unregister itself, %s\n",
		 dfs_dceErrTxt(code));
	ERROR(code);
    }

    code = compat_UnregisterServer (UBIKVOTE_v4_0_s_ifspec,
				    (uuid_t *)&object_uuid);
    if (code) {
	LogError(code, "Bakserver failed to unregister itself, %s\n",
		 dfs_dceErrTxt(code));
	ERROR(code);
    }
    exit(0);

error_exit:
    fprintf(stderr, "Bakserver: failed to start; %s\n",
	    dfs_dceErrTxt(code));
    exit(1);
}


consistencyCheckDb()
{
    /* do consistency checks on structure sizes */
    if ( (sizeof(struct htBlock) > BLOCKSIZE)
    ||	 (sizeof(struct vfBlock) > BLOCKSIZE)
    ||   (sizeof(struct viBlock) > BLOCKSIZE)
    ||   (sizeof(struct dBlock) > BLOCKSIZE)
    ||   (sizeof(struct tBlock) > BLOCKSIZE)
       )
    {
	fprintf (stderr, "Block layout error!\n");
	exit (99);
    }
}

/*VARARGS*/
LogDebug(level, a,b,c,d,e,f,g,h,i)
     int level;
{
  if ( verbose >= level)
    fprintf(stderr,(char *) a, b, c, d, e, f, g, h, i);
}

/*VARARGS*/
Log(a,b,c,d,e,f,g,h,i)
{
  time_t tm;
    LogHeader(0);
    if ( globalConfPtr->log != NULL )
    {
      tm = time(0);
      fprintf(globalConfPtr->log, "%s: ", (char *)ctime((time_t *)&tm));
      fprintf(globalConfPtr->log,(char *) a, b, c, d, e, f, g, h, i);
      fflush(globalConfPtr->log);
    }
}

/*VARARGS*/
LogError(code, a,b,c,d,e,f,g,h,i)
{
  time_t tm;
    if ( globalConfPtr->log != NULL )
    {
      tm=time(0);
      fprintf(globalConfPtr->log, "%s: ", (char *)ctime((time_t *)&tm));
      if ( code )
	fprintf(globalConfPtr->log, "%s: %s\n", error_table_name(code),
		dfs_dceErrTxt(code));
      fprintf(globalConfPtr->log,(char *) a, b, c, d, e, f, g, h, i);
      fflush(globalConfPtr->log);
    }
}


/*  ----------------
 * debug
 * ----------------
 */


LogNetDump(dumpPtr)
     struct dump *dumpPtr;
{
    struct dump hostDump;
    extern buServerConfP globalConfPtr;

    dump_ntoh(dumpPtr, &hostDump);
    if ( globalConfPtr->log != NULL )
    {
	printDump(globalConfPtr->log, &hostDump);
    }
}
