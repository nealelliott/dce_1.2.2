/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftserver_main.c,v $
 * $EndLog$
 */
/* Copyright (C) 1996, 1995, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <fcntl.h>
#include <dcedfs/osi_net.h>

#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/cmdAux.h>
#include <dcedfs/osi_dfserrors.h>

#include <ftserver_proc.h>
#include <ftserver_data.h>
#include <dcedfs/queue.h>

#include <dcedfs/volume.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/ftutil.h>

#include <ftserver.h>

#include <time.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfstsvmac.h>
#include <dfstsvsvc.h>
#include <dfstsvmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/ftserver/RCS/ftserver_main.c,v 4.81 1996/10/12 18:10:29 madhuc Exp $")

osi_dlock_t globalLock;

/*
 * Local Macros
 */
#define	GCINTERVAL		30
#define	N_SECURITY_OBJECTS	3
#define	RPCPACKETS		100
#define	RPCDEADTIME		150
#define	FT_MAXCALLS		30

#if defined(AFS_OSF_ENV)
#define FTSERVER_RPC_STACK_SIZE	128000
#endif /* AFS_OSF_ENV */

#define DEFAULT_ADMIN_FILENAME	"admin.ft"

#ifndef FNAME_SIZ
#define FNAME_SIZ 1024
#endif /* FNAME_SIZ */

int verbose = 0;
FILE *logIO;
struct icl_set *iclSetP = 0;
struct icl_log *iclLogP = 0;

globaldef uuid_t uuid_nil;

#if !defined(OSF_NO_SOCKET_DUMP)
error_status_t
    SFTSERVER_CreateTrans(),
    SFTSERVER_AbortTrans(),
    SFTSERVER_DeleteTrans(),
    SFTSERVER_CreateVolume(),
    SFTSERVER_DeleteVolume(),
    SFTSERVER_Dump(),
    SFTSERVER_Restore(),
    SFTSERVER_Forward(),
    SFTSERVER_Clone(),
    SFTSERVER_ReClone(),
    SFTSERVER_GetFlags(),
    SFTSERVER_SetFlags(),
    SFTSERVER_GetStatus(),
    SFTSERVER_SetStatus(),
    SFTSERVER_ListVolumes(),
    SFTSERVER_ListAggregates(),
    SFTSERVER_AggregateInfo(),
    SFTSERVER_Monitor(),
    SFTSERVER_GetOneVolStatus(),
    SFTSERVER_GetServerInterfaces(),
    SFTSERVER_SwapIDs(),
    SFTSERVER_ConnectSock(),
    SFTSERVER_DisconnectSock(),
    SFTSERVER_ListenSock(),
    SFTSERVER_AcceptSock();

globaldef FTSERVER_v4_0_epv_t FTSERVER_v4_0_epv = {
    SFTSERVER_CreateTrans,
    SFTSERVER_AbortTrans,
    SFTSERVER_DeleteTrans,
    SFTSERVER_CreateVolume,
    SFTSERVER_DeleteVolume,
    SFTSERVER_Dump,
    SFTSERVER_Restore,
    SFTSERVER_Forward,
    SFTSERVER_Clone,
    SFTSERVER_ReClone,
    SFTSERVER_GetFlags,
    SFTSERVER_SetFlags,
    SFTSERVER_GetStatus,
    SFTSERVER_SetStatus,
    SFTSERVER_ListVolumes,
    SFTSERVER_ListAggregates,
    SFTSERVER_AggregateInfo,
    SFTSERVER_Monitor,
    SFTSERVER_GetOneVolStatus,
    SFTSERVER_GetServerInterfaces,
    SFTSERVER_SwapIDs,
    SFTSERVER_ConnectSock,
    SFTSERVER_DisconnectSock,
    SFTSERVER_ListenSock,
    SFTSERVER_AcceptSock
};

#else /* OSF_NO_SOCKET_DUMP */

error_status_t
    SFTSERVER_CreateTrans(),
    SFTSERVER_AbortTrans(),
    SFTSERVER_DeleteTrans(),
    SFTSERVER_CreateVolume(),
    SFTSERVER_DeleteVolume(),
    SFTSERVER_Dump(),
    SFTSERVER_Restore(),
    SFTSERVER_Forward(),
    SFTSERVER_Clone(),
    SFTSERVER_ReClone(),
    SFTSERVER_GetFlags(),
    SFTSERVER_SetFlags(),
    SFTSERVER_GetStatus(),
    SFTSERVER_SetStatus(),
    SFTSERVER_ListVolumes(),
    SFTSERVER_ListAggregates(),
    SFTSERVER_AggregateInfo(),
    SFTSERVER_Monitor(),
    SFTSERVER_GetOneVolStatus(),
    SFTSERVER_GetServerInterfaces(),
    SFTSERVER_SwapIDs();

globaldef FTSERVER_v4_0_epv_t FTSERVER_v4_0_epv = {
    SFTSERVER_CreateTrans,
    SFTSERVER_AbortTrans,
    SFTSERVER_DeleteTrans,
    SFTSERVER_CreateVolume,
    SFTSERVER_DeleteVolume,
    SFTSERVER_Dump,
    SFTSERVER_Restore,
    SFTSERVER_Forward,
    SFTSERVER_Clone,
    SFTSERVER_ReClone,
    SFTSERVER_GetFlags,
    SFTSERVER_SetFlags,
    SFTSERVER_GetStatus,
    SFTSERVER_SetStatus,
    SFTSERVER_ListVolumes,
    SFTSERVER_ListAggregates,
    SFTSERVER_AggregateInfo,
    SFTSERVER_Monitor,
    SFTSERVER_GetOneVolStatus,
    SFTSERVER_GetServerInterfaces,
    SFTSERVER_SwapIDs
};

#endif /* OSF_NO_SOCKET_DUMP */

dce_svc_handle_t   tsv_svc_handle;

void initialize_svc()
{
  error_status_t st;

  tsv_svc_handle = dce_svc_register(tsv_svc_table,  (idl_char *)"tsv",  &st);
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

  dce_msg_define_msg_table(tsv__table,
        sizeof tsv__table / sizeof tsv__table[0], &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Warning Cannot register tsv incore message table 0x%lx\n", st);
}

void Log(s, a, b, c, d, e, f, g, h, i, j)
unsigned32          s;
{
    ftu_svc_LogPrintf(s, a, b, c, d, e, f, g, h, i, j);
}


static void LogInit()
{
    char LOGFILE[500], OLOGFILE[500];
    strcpy(LOGFILE, DCELOCAL_PATH);
    strcat(LOGFILE, "/var/dfs/adm/FtLog");
    strcpy(OLOGFILE, LOGFILE);
    strcat(OLOGFILE, ".old");
    rename(LOGFILE, OLOGFILE);			/* XXX check error? XXX */
    if (!(logIO = fopen(LOGFILE, "w"))) {
	dce_svc_printf(TSV_S_MAIN_LOG_OPEN_MSG, LOGFILE);
	exit(1);
    }
    setbuf(logIO, NULL);
    ftu_SetLogStream(logIO);
    Log(tsv_s_main_log_initialized, LOGFILE);
}

/*
 * Background daemon for Garbage Collections
 */
static pthread_addr_t GCDaemon(ignoreMe)
pthread_addr_t ignoreMe;
{
    struct timeval tv;

    while (1) {
	tv.tv_sec = GCINTERVAL;
	tv.tv_usec = 0;
	(void) select(0, 0, 0, 0, &tv);
	ftserver_GCTrans();
    }
}

extern FTSERVER_ExecuteRequest();

char localCellName[256];

ftserverDriver(as, arock)
    struct cmd_syndesc  *as;
    char *arock;
{
    register long	code;
    error_status_t	st;
    pthread_t		GCDaemonPid;
    pthread_attr_t	createAttr;
    char		adminFilename[FNAME_SIZ];
    char excBuf[50];
    char *excNameP;
    char *nameP;
    uuid_t nil_uuid;
    extern char * ExceptionToString();

    lock_Init(&globalLock);

    verbose = 0;
    (void)strncpy(adminFilename, DEFAULT_ADMIN_FILENAME,
		   sizeof(adminFilename) - 1);
    /* '-adminlist' at offset 0 */
    /* '-verbose' at offset 1 */
    if (as->parms[0].items && as->parms[0].items->data)
      (void) strncpy(adminFilename, as->parms[0].items->data,
		     sizeof(adminFilename) - 1);
    if (as->parms[1].items) {
      verbose = 1;
      dce_svc_printf(TSV_S_MAIN_START_DEBUG_MSG, verbose);
    }
    LogInit();
    ftutil_ImagePrep();

    dfs_installInterfaceDescription(FTSERVER_v4_0_s_ifspec, FTSERVER_v4_0_s_ifspec,
				     1 /* provider_version number */,
				     (unsigned_char_t *)"Transarc FT Server Interface", &st);
    if (st != 0) {
	Log(tsv_s_main_cant_install_interface, dfs_dceErrTxt(st));
	dce_svc_printf(TSV_S_MAIN_INSTALL_INTERFACE_FAILED_MSG, dfs_dceErrTxt(st));
	exit(1);
    }

    /* We'll be encoding the RPC results. */
    osi_initEncodeTable();

    nameP = NULL;
    dce_cf_get_cell_name(&nameP, &st);
    if ((st != 0) || (nameP == 0) || (nameP[0] != '/')) {
	Log(tsv_s_main_log_local_cell_name, dfs_dceErrTxt(st));
	dce_svc_printf(TSV_S_MAIN_LOCAL_CELL_NAME_MSG, dfs_dceErrTxt(st));
	exit(1);
    }
    strncpy(localCellName, nameP, sizeof(localCellName));
    if (nameP) free(nameP);
#ifdef use_shutdown_duplicates
    code = compat_ShutdownDuplicateServer (FTSERVER_v4_0_s_ifspec,
					   (uuid_t *)NULL, 0);
    if ( code) {
      Log(tsv_s_main_log_duplicate_ftserver, dfs_dceErrTxt(code));
      dce_svc_printf(TSV_S_MAIN_DUPLICATE_FTSERVER_MSG, dfs_dceErrTxt(code));
      exit(1);
    }
#endif /* use_shutdown_duplicates */
    uuid_create_nil(&nil_uuid, (unsigned32 *)&st);
    if ( st != uuid_s_ok ) {
      Log(tsv_s_main_log_uuid_create_failed, (long)st);
      dce_svc_printf(TSV_S_MAIN_UUID_CREATE_FAILED_MSG, (long)st);
      exit(1);
    }

    dfs_register_rpc_server(FTSERVER_v4_0_s_ifspec,
			    (rpc_mgr_epv_t)&FTSERVER_v4_0_epv,
			    &nil_uuid, &nil_uuid,
			    FT_MAXCALLS,
			    adminFilename,		/* admin file path */
			    "DFS ftserver",
			    (error_status_t *)&st);
    if ( st != error_status_ok ) {
	Log(tsv_s_main_log_rpc_register_failed, (long)st);
	dce_svc_printf(TSV_S_MAIN_RPC_REGISTER_FAILED_MSG, (long)st);
	exit(1);
    }

#if defined(AFS_OSF_ENV)
    {
      unsigned32 status;

      rpc_mgmt_set_server_stack_size(FTSERVER_RPC_STACK_SIZE, &status);
      if (status != rpc_s_ok){
	Log(tsv_s_main_log_rpc_mgmt_set_server_stack_size, FTSERVER_RPC_STACK_SIZE, status);
      }
    }
#endif /* AFS_OSF_ENV */

    TRY {
	pthread_attr_create(&createAttr);
	pthread_attr_setstacksize(&createAttr, 40000);
	code = pthread_create(&GCDaemonPid, createAttr, GCDaemon, (pthread_addr_t)0);
	if (code) {
	    Log(tsv_s_main_log_pthread_create, code);
	    dce_svc_printf(TSV_S_MAIN_PTHREAD_CREATE_MSG, code);
	    exit(1);
	}

	Log(tsv_s_main_log_starting);
	rpc_mgmt_set_server_com_timeout(rpc_c_binding_default_timeout+2, &st);
	rpc_server_listen(FT_MAXCALLS, &st);
	if (st != error_status_ok) {
	    Log(tsv_s_main_log_rpc_listen, (long)st);
	    dce_svc_printf(TSV_S_MAIN_RPC_LISTEN_MSG, (long)st);
	    exit(1);
	}
	/*
	 * if there is a duplicate server, rpc_server_listen should return
	 * error_status_ok
	 */
	code = compat_UnregisterServer (FTSERVER_v4_0_s_ifspec,
					(uuid_t *)NULL);
	if (code) {
	    Log(tsv_s_main_log_compat_UnregisterServer, dfs_dceErrTxt(code));
	    dce_svc_printf(TSV_S_MAIN_COMPAT_UNREGISTERSERVER_MSG, dfs_dceErrTxt(code));
	    exit(1);
	  }
      }
    CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	    sprintf(excBuf, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excBuf;
	}

	dce_svc_printf(TSV_S_MAIN_UNEXPECTED_EXCEPTION_RAISED_MSG,
		       excNameP,
		       strerror(errno));

 	Log(tsv_s_main_log_unexpected_exception_raised, excNameP, errno);
	exit(1);
    }
    ENDTRY
}

static void AbortDumpHandler(sig)
int sig;
{
    (void)dce_svc_printf(TSV_S_MAIN_ABORTDUMPHANDLER_MSG, sig);
    icl_DumpUser();
    abort();
}

/* Main server module */
main (argc, argv)
int     argc;
char    **argv;
{
  struct cmd_syndesc *ts;
  char               *cmd_explanation;
  int code;
  sigset_t           signalBlockSet;
  error_status_t     st;

  osi_setlocale(LC_ALL, "");

  initialize_svc();

  cmd_explanation = (char *) dce_msg_get_msg(tsv_s_main_cmd_ftserver, &st);
  ts = cmd_CreateSyntax((char *)0, ftserverDriver, (char *)0, cmd_explanation);
  free(cmd_explanation);

  cmd_explanation = (char *) dce_msg_get_msg(tsv_s_main_cmd_filename, &st);
  cmd_AddParm(ts, "-adminlist", CMD_SINGLE, CMD_OPTIONAL, cmd_explanation);
  free(cmd_explanation);

  cmd_explanation = (char *) dce_msg_get_msg(tsv_s_main_cmd_verbose, &st);
  cmd_AddParm(ts, "-verbose", CMD_FLAG, CMD_OPTIONAL, cmd_explanation);
  free(cmd_explanation);

  /*
   * block SIGUSR1 for all threads. The ICL thread that wiil be
   * waiting for it will clear its mask by using sigwait()
   */
  (void)sigemptyset(&signalBlockSet);
  (void)sigaddset(&signalBlockSet, SIGUSR1);
  (void)sigprocmask(SIG_BLOCK, &signalBlockSet, (sigset_t *)NULL);

  /* Initialize ICL tracing */
  if (code = icl_ExportIF("ftserver"))
      dce_svc_printf(TSV_S_MAIN_EXPORT_RPC_FAILED_MSG, code);

  if (code = icl_CreateLog("common", 0, &iclLogP))
      dce_svc_printf(TSV_S_MAIN_ICL_LOG_FAILED_MSG, code);
  else if (code = icl_CreateSet("ftserver", iclLogP, 0, &iclSetP))
      dce_svc_printf(TSV_S_MAIN_ICL_SET_FAILED_MSG, code);
  else if (code = ftu_CreateIclSet(iclLogP))
      dce_svc_printf(TSV_S_MAIN_ICL_EVENT_SET_MSG, code);
  else if (code = icl_StartCatcher(0))
      dce_svc_printf(TSV_S_MAIN_ICL_CATCHER_MSG, code);
#if defined(AFS_DEBUG) && defined(AFS_AIX_ENV)
  else {
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
	  (void) dce_svc_printf(TSV_S_MAIN_SIGBUS_MSG, errno);
      }
      if (SIGFUNC(SIGSEGV, &sv, NULL) < 0)
      {
	  (void) dce_svc_printf(TSV_S_MAIN_SIGSEGV_MSG, errno);
      }
      if (SIGFUNC(SIGILL, &sv, NULL) < 0)
      {
	  (void) dce_svc_printf(TSV_S_MAIN_SIGKILL_MSG, errno);
      }
  }
#endif	/* AFS_DEBUG && AFS_AIX_ENV */

  /*
   * as a workaround to the DG private sockets/pipe argument problem
   * (transarc DB 6731), we disallow private DG sockets.  Do it here,
   * after we know the DCE RPC is initialized, as the last thing we do
   * before starting real work.
   */
#ifndef AFS_AIX_ENV
    rpc__dg_disa_priv_sockets();
#endif /* AFS_AIX_ENV */


  return cmd_Dispatch(argc, argv);
}
