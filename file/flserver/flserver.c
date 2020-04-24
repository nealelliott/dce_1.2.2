/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: flserver.c,v $
 * Revision 1.1.94.1  1996/10/02  17:46:16  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:37:07  damon]
 *
 * $EndLog$
*/

/* Copyright (C) 1996, 1991 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/stds.h>
#include <dcedfs/basicincludes.h>
#include <dcedfs/common_data.h>
#include <dcedfs/cmdAux.h>

#include <dce/rpc.h>
#include <dce/dnsmessage.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <signal.h>
#include <fldb_proc.h>

#include <flserver.h>
#include <fl_opcodes.h>
#include "flinternal.h"

#include <time.h>

#define DEFAULT_ADMIN_FILENAME	"admin.fl"

#include <dcedfs/icl.h>
#include <fl_trace.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfslsvmac.h>
#include <dfslsvsvc.h>
#include <dfslsvmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/flserver/flserver.c,v 1.1.94.1 1996/10/02 17:46:16 damon Exp $")

dce_svc_handle_t   lsv_svc_handle;

void initialize_svc()
{
  error_status_t st;

  lsv_svc_handle = dce_svc_register(lsv_svc_table,  (idl_char *)"lsv",  &st);
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

  dfs_define_lsv_message_tables();

}



#if 0
SHARED struct afsconf_dir *vldb_confdir = 0;	/* vldb configuration dir */
#endif /* 0 */
SHARED struct vlconf_cell LocalCellInfo;
SHARED afs_hyper_t trialCellID;
PRIVATE char *groupName = NULL;
SHARED int flserver_verbose = 0;
SHARED int LocalCellNames;

SHARED vldstats dynamic_statistics;
SHARED struct ubik_dbase *VL_dbase;
SHARED struct siteBlock FirstSiteBlock;
extern long ubik_lastYesTime;
extern long ubik_nBuffers;
/* some messy problems if ubikdisk_proc.h and ubikvote_proc.h are included */
extern rpc_if_handle_t UBIKDISK_v4_0_s_ifspec;
extern rpc_if_handle_t UBIKVOTE_v4_0_s_ifspec;

extern osi_dlock_t globalLock;
extern osi_dlock_t statLock;

    error_status_t st;
    unsigned char   *cmd_explanation;



globaldef VL__v4_0_epv_t VL__v4_0_epv = {
    VL_GetEntryByID,
    VL_GetEntryByName,
    VL_Probe,
    VL_GetCellInfo,
    VL_GetNextServersByID,
    VL_GetNextServersByName,
    VL_GetSiteInfo,
    VL_GetCEntryByID,
    VL_GetCEntryByName,
    VL_GetCNextServersByID,
    VL_GetCNextServersByName,
    VL_ExpandSiteCookie,
    VL_GetServerInterfaces,
    /* end of prefix */
    VL_CreateEntry,
    VL_DeleteEntry,
    VL_GetNewVolumeId,
    VL_ReplaceEntry,
    VL_SetLock,
    VL_ReleaseLock,
    VL_ListEntry,
    VL_ListByAttributes,
    VL_GetStats,
    VL_AddAddress,
    VL_RemoveAddress,
    VL_ChangeAddress,
    VL_GenerateSites,
    VL_GetNewVolumeIds,
    VL_CreateServer,
    VL_AlterServer
};
globaldef uuid_t uuid_nil;

static void AbortDumpHandler(sig)
int sig;
{
    (void) dce_fprintf(stderr, lsv_s_dumphdlr_called);
    icl_DumpUser();
    abort();
}

/* define event set structure for tracing and setup function */
struct icl_set *fl_iclSetp = 0;
PRIVATE void SetupICL()
{
    struct icl_log *logp;
    long code;
    static int Initted = 0;

    if (Initted)
	return;
    Initted = 1;

    code = icl_ExportIF("flserver");
    if (code)
	(void) dce_fprintf(stderr, lsv_s_ICL_if_not_exported, code);

    code = icl_StartCatcher(0);
    if (code < 0)
	(void) dce_fprintf(stderr, lsv_s_ICL_signal_watcher_not_started, code, errno);
    else
    {
	code = icl_CreateLog("common", 30*1024, &logp);
	if (code == 0)
	    code = icl_CreateSet("flserver", logp, (struct icl_log *) 0, &fl_iclSetp);
	if (code)
	    dce_fprintf(stderr, lsv_s_ICL_trace_not_inited, code);
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
		(void) dce_fprintf(stderr, lsv_s_cant_catch_SIGBUS, errno);
	    }
	    if (SIGFUNC(SIGSEGV, &sv, NULL) < 0)
	    {
		(void) dce_fprintf(stderr, lsv_s_cant_catch_SIGSEGV, errno);
	    }
	    if (SIGFUNC(SIGILL, &sv, NULL) < 0)
	    {
		(void) dce_fprintf(stderr, lsv_s_cant_catch_SIGILL, errno);
	    }
	}
    }
}

/* Initialize the stats for the opcodes */
PRIVATE void initialize_dstats ()
{   int i;

    dynamic_statistics.start_time = (unsigned long) time(0);
    bzero((char *)&dynamic_statistics.requests[0], sizeof(dynamic_statistics.requests));
    bzero((char *)&dynamic_statistics.aborts[0], sizeof(dynamic_statistics.aborts));
}

PRIVATE FILE *logIO = NULL;
PRIVATE char dcelocal_Path[] = DCELOCAL_PATH;

extern void Log();	/* forward declaration */

PRIVATE void initFlLog(myName)
char *myName;
{/* Initialize the log service. */
    char LOGFILE[500], OLOGFILE[500];
    strcpy(LOGFILE, dcelocal_Path);
    strcat(LOGFILE, "/var/dfs/adm/FlLog");
    strcpy(OLOGFILE, LOGFILE);
    strcat(OLOGFILE, ".old");
    rename(LOGFILE, OLOGFILE);	/* don't check error code */
    logIO = fopen(LOGFILE, "w");
    if (!logIO) {
	dce_fprintf(stderr, lsv_s_write_open_failed, LOGFILE);
	exit(1);
    }
    initialize_ubk_error_table();
    initialize_vls_error_table();

    cmd_explanation = dce_msg_get_msg(lsv_s_log_inited_to_myname, &st);
    Log( (char *) cmd_explanation, myName, LOGFILE);
    free(cmd_explanation);

}

#define VLSERVER_MAXNCSCALLS 10

PRIVATE int flserverDriver(as, arock)
  IN struct cmd_syndesc *as;
  IN char *arock;
{

    register long   code;
    long	    serverList[UBIK_MAXSERVERS];
    long	    myHost;
    struct sockaddr_in newserverList[UBIK_MAXSERVERS];
    struct sockaddr_in newmyHost;
    struct hostent *th;
    char hostname[MAXFTNAMELEN];
    register int index, i,j;
    uuid_t objUUID;
    char *cellNameP, *excNameP;
    register char *realCellP;
    long serverCount, NowTime, TargetTime, Retries;
    struct timeval tv;
    struct sockaddr_in *sinp;
    struct ubik_binding_vector_t *bindingVectorP;
    rpc_binding_handle_t *selectedBinding;
    error_status_t st;
    char adminPath[BUFSIZ];
    char *vl_dbaseName;
    static char dcelocal_Suffix[] = "/var/dfs/fldb";
    extern char * ExceptionToString();
    uuid_t object_uuid;
    /* cds */
    char nsGroup[BUFSIZ];
    /* security */
    static char secGroup[] = "subsys/dce/dfs-fs-servers";

    strncpy(adminPath, DEFAULT_ADMIN_FILENAME, sizeof(adminPath) - 1);
    nsGroup[0] = '\0';

    /* '-adminlist' at offset 0 */
    /* '-verbose' at offset 1 */
    if (as->parms[0].items)
      (void) strncpy(adminPath, as->parms[0].items->data,
		     sizeof(adminPath) - 1);
    if (as->parms[1].items)
      flserver_verbose = 1;
    else
      flserver_verbose = 0;

    initFlLog("flserver");

    dfs_installInterfaceDescription(VL__v4_0_s_ifspec, VL__v4_0_s_ifspec,
				     1 /* provider_version number */,
	         (unsigned_char_t *)"Transarc FL Server Interface", &st);
    if (st != 0) {

	cmd_explanation = dce_msg_get_msg(lsv_s_intfc_descr_not_installed, &st);
	Log( (char *) cmd_explanation, dfs_dceErrTxt(st));
	free(cmd_explanation);

	dce_fprintf(stderr, lsv_s_cant_install_infc_descr, dfs_dceErrTxt(st));
	exit(1);
    }

    TRY {
	/* Start setting up LocalCellInfo. */
	bzero((char *)&LocalCellInfo, sizeof(LocalCellInfo));
	cellNameP = NULL;
	dce_cf_get_cell_name(&cellNameP, &st);
	if ((st != 0) || (cellNameP == 0) || (cellNameP[0] != '/')) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_local_cell_name_not_determined, &st);
	    Log( (char *) cmd_explanation, dfs_dceErrTxt(st));
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_cant_determine_local_cell_name, dfs_dceErrTxt(st));
	    exit(1);
	}
	/* Turn DCE version of cell name into our style, for LocalCellInfo. */
	realCellP = cellNameP;
	if (strncmp(realCellP, "/.../", 5) == 0) realCellP += 5;
	else if (realCellP[0] == '/') ++realCellP;
	strncpy((char *)LocalCellInfo.name, realCellP, sizeof(LocalCellInfo.name) - 1);
	strncpy(nsGroup, cellNameP, sizeof(nsGroup)-10);
	i = strlen(nsGroup);

	icl_Trace1(fl_iclSetp, FL_TRACE_GET_JUNCT, ICL_TYPE_STRING, cellNameP);
	dfs_GetJunctionName(cellNameP, 0, &nsGroup[i], sizeof(nsGroup) - i - 1, &st);
	if (st != 0) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_DFS_cellname_jct_not_determined, &st);
	    Log( (char *) cmd_explanation, cellNameP, dfs_dceErrTxt(st));
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_cant_determine_cellname_jct, cellNameP, dfs_dceErrTxt(st));
	    exit(1);
	}

	cmd_explanation = dce_msg_get_msg(lsv_s_default_nsGroup_used, &st);
	Log( (char *) cmd_explanation, nsGroup);
	free(cmd_explanation);

#ifdef notdef
	strncpy(secGroup, cellNameP, sizeof(secGroup) - sizeof(secGroupSuffix));
	strcat(secGroup, secGroupSuffix);
#endif
	free(cellNameP);
	/* get this host */
	gethostname(hostname, sizeof(hostname));
	th = gethostbyname(hostname);
	if (!th) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_address_ofthis_host_not_avail, &st);
	    Log( (char *) cmd_explanation, hostname);
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_cant_get_thishost_address, hostname);
	    exit(1);
	}
	myHost = *((long *) th->h_addr);
	rpcx_sockaddr_from_ipaddr(th->h_addr, &newmyHost);

	bzero((char *)newserverList, sizeof(newserverList));

	ubik_nBuffers = 256;

	icl_Trace0(fl_iclSetp, FL_TRACE_INIT_AUTH);
	code = dfsauth_server_InitAuthentication((char *)NULL, adminPath, (char *)NULL);
	if (code) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_authenticate_not_init, &st);
	    Log( (char *) cmd_explanation, code, dfs_dceErrTxt(code));
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_cant_init_authentication, dfs_dceErrTxt(code));
	    exit(2);
	}

	code = strlen(dcelocal_Path) + strlen(dcelocal_Suffix) + 1;
	vl_dbaseName = (char *) malloc(code);
	if (vl_dbaseName == NULL) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_not_enuf_memory, &st);
	    Log( (char *) cmd_explanation, code);
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_no_flserver_memory, code);
	    exit(2);
	}
	strcpy(vl_dbaseName, dcelocal_Path);
	strcat(vl_dbaseName, dcelocal_Suffix);
	NowTime = osi_Time();
	TargetTime = NowTime + 600;
	Retries = 0;
	/* Keep trying to init Ubik until CDS comes up. */
	for (;;) {
	    icl_Trace0(fl_iclSetp, FL_TRACE_INIT_UBIK);
	    code = ubik_ServerInit(&newmyHost, VL__v4_0_s_ifspec,
				   (rpc_mgr_epv_t)&VL__v4_0_epv,
				   (unsigned_char_t *)nsGroup,
				   (unsigned_char_t *)NULL,
				   (unsigned_char_t *)secGroup,
				   (uuid_t *)&Fldb_UbikObjectTypeUuid,
				   (uuid_t *)&object_uuid,
				   VLSERVER_MAXNCSCALLS, vl_dbaseName,
				   "DFS Flserver",
				   (struct ubik_dbase **)&VL_dbase);
	    if (code != rpc_s_entry_not_found
		&& code != DNS_SOCKET) break;	/* some other problem */
	    NowTime = osi_Time();
	    if (NowTime >= TargetTime) break;		/* timed out */
	    bzero((char *)&tv, sizeof(tv));
	    tv.tv_sec = TargetTime - NowTime;

	    cmd_explanation = dce_msg_get_msg(lsv_s_ubik_serverinit_failed, &st);
	    Log( (char *) cmd_explanation, nsGroup, secGroup, code, dfs_dceErrTxt(code), tv.tv_sec);
	    free(cmd_explanation);

	    if (tv.tv_sec > 4) tv.tv_sec = 4;
	    (void) select(0, 0, 0, 0, &tv);
	    ++Retries;
	}
	free(vl_dbaseName);
	if (code) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_ubik_serverinit_faailed_totally, &st);
	    Log( (char *) cmd_explanation, nsGroup, secGroup, code, dfs_dceErrTxt(code));
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_ubik_init_failed_with_code, nsGroup, code, dfs_dceErrTxt(code));
	    exit(2);
	}

	/* Get a cell ID that we could use if we're called upon to write
	   a fresh FLDB header. */
	compat_get_group_object_uuid((unsigned char *)nsGroup,
					    &objUUID, &st);
	if ( st ) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_no_objuuid_will_try_creating_one, &st);
	    Log( (char *) cmd_explanation, nsGroup, dfs_dceErrTxt(code));
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_cant_get_objuuid, nsGroup, dfs_dceErrTxt(code));
	    uuid_create(&objUUID, &st);
	    if (st != uuid_s_ok) {

		cmd_explanation = dce_msg_get_msg(lsv_s_cant_create_frestuuid_exiting, &st);
		Log( (char *) cmd_explanation, dfs_dceErrTxt(st));
		free(cmd_explanation);

		dce_fprintf(stderr, lsv_s_cant_even_create_freshuuid, dfs_dceErrTxt(st));
		exit(1);
	    }
	} else {
	    unsigned long hi, lo;
	    hi = lo = 0;
	    for (i = 0; i < 4; ++i) {
		hi <<= 8;
		hi |= objUUID.node[i];
	    }
	    for (i = 4; i < 6; ++i) {
		lo <<= 8;
		lo |= objUUID.node[i];
	    }
	    /* This doesn't really have to address network/host order problems,
		  but it would be nice if it did and it worked. */
	    lo <<= 16;
	    lo |= htons(objUUID.time_mid);
	    AFS_hset64(trialCellID, hi, lo);
	}
	/* Finish setting up LocalCellInfo from the bindings. */
	icl_Trace0(fl_iclSetp, FL_TRACE_GET_SERVER_LIST);
	ubik_GetServerList(nsGroup, VL__v4_0_s_ifspec,
			   0, /* we need all server bindings including ours */
			   &bindingVectorP, &st);
	if ( st != rpc_s_ok ) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_ukib_getserverlist_failure, &st);
	    Log( (char *) cmd_explanation, nsGroup, dfs_dceErrTxt(st));
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_ubik_getserverlist_failed, nsGroup, dfs_dceErrTxt(st));
	    exit(1);
	}

	LocalCellInfo.numServers = 0;
	for (i = 0; i < bindingVectorP->count; ++i) {
	  /* We'll take the first address for each server now, but we may have
	   * to handle the case of multi-homed hosts soon
	   */
	  selectedBinding = &(bindingVectorP->s[i].binding_h[0]);
	  st = rpcx_binding_to_sockaddr((rpc_binding_handle_t)*selectedBinding,
			    (struct sockaddr_in *)&LocalCellInfo.hostAddr[i]);
	    if (st != rpc_s_ok)
		break;
	    LocalCellInfo.numServers = i + 1;
	}
	if (st != rpc_s_ok && st != rpc_s_no_more_bindings) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_error_enumerating_bindings, &st);
	    Log( (char *) cmd_explanation, nsGroup, dfs_dceErrTxt(st));
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_cant_enumerate_bindings, nsGroup, dfs_dceErrTxt(st));
	    exit(1);
	}
	icl_Trace1(fl_iclSetp, FL_TRACE_NUM_SERVERS,
		   ICL_TYPE_LONG, LocalCellInfo.numServers);

	/* Now free the binding vector */
	for(i=0;i<bindingVectorP->count;i++)
	  for(j=0;j<bindingVectorP->s[i].count;j++)
	    rpc_binding_free(&(bindingVectorP->s[i].binding_h[j]), &st);
	(void)free(bindingVectorP->s);
	(void)free(bindingVectorP);

	if (flserver_verbose)
	    dce_fprintf(stderr, lsv_s_cell_trialid_and_numservers,
		   LocalCellInfo.name, AFS_HGETBOTH(trialCellID),
		   LocalCellInfo.numServers);

	cmd_explanation = dce_msg_get_msg(lsv_s_log_cell_trialid_and_numservers, &st);
	Log( (char *) cmd_explanation, LocalCellInfo.name,
	    AFS_HGETBOTH(trialCellID), LocalCellInfo.numServers);
	free(cmd_explanation);

	LocalCellNames = 0;
	for (i = 0; i < LocalCellInfo.numServers; ++i) {
	    LocalCellInfo.hostName[i].hstnam[0] = '\0';
	}
	/* We put the principal name into hstnam, later, at the first call. */

	bzero((char *)&FirstSiteBlock, sizeof(FirstSiteBlock));
	FirstSiteBlock.Tag = htonl(SITEBLOCKTAG);
	FirstSiteBlock.AllocHere = SITESINBLOCK;
	initialize_dstats();
	lock_Init(&globalLock);
	lock_Init(&statLock);
	vlserver_Init();

	/* NCS code that starts the server */
	rpc_mgmt_set_server_stack_size(60000, &st);
	if (st != error_status_ok) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_set_server_stacksize_failed, &st);
	    Log( (char *) cmd_explanation, dfs_dceErrTxt(st));
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_setsvr_stacksize_failed, dfs_dceErrTxt(st));
	    exit(1);
	}

	if (flserver_verbose)
	    dce_fprintf(stderr, lsv_s_flsvr_ready_for_business);

	cmd_explanation = dce_msg_get_msg(lsv_s_flserver_ready_for_service, &st);
	Log( (char *) cmd_explanation);
	free(cmd_explanation);

	icl_Trace0(fl_iclSetp, FL_TRACE_READY);

	rpc_mgmt_set_server_com_timeout(rpc_c_binding_default_timeout+2, &st);
	rpc_server_listen(VLSERVER_MAXNCSCALLS , &st);
	if (st != error_status_ok){

	    cmd_explanation = dce_msg_get_msg(lsv_s_rpc_listen_failed, &st);
	    Log( (char *) cmd_explanation, dfs_dceErrTxt(st));
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_server_listen_failed, dfs_dceErrTxt(st));
	    exit(1);
	}
	/*
	 * if there is a duplicate server, rpc_server_listen should return
	 * error_status_ok
	 */
	code = compat_UnregisterServer (VL__v4_0_s_ifspec, &object_uuid);
	if (code) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_unregister_server_failed, &st);
	    Log( (char *) cmd_explanation, dfs_dceErrTxt(code));
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_compat_unregister_server_failed, dfs_dceErrTxt(code));
	    exit(1);
	}
	icl_Trace0(fl_iclSetp, FL_TRACE_UNREG);

	/* Have to unregister the two ubik interfaces also */
	code = compat_UnregisterServer (UBIKDISK_v4_0_s_ifspec, &object_uuid);
	if (code) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_log_unregister_server_failed, &st);
	    Log( (char *) cmd_explanation, dfs_dceErrTxt(code));
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_compat_unreg_server_failed, dfs_dceErrTxt(code));
	    exit(1);
	}

	code = compat_UnregisterServer (UBIKVOTE_v4_0_s_ifspec, &object_uuid);
	if (code) {

	    cmd_explanation = dce_msg_get_msg(lsv_s_log_unregister_svr_failed_again, &st);
	    Log( (char *) cmd_explanation, dfs_dceErrTxt(code));
	    free(cmd_explanation);

	    dce_fprintf(stderr, lsv_s_unregister_svr_failed_again, dfs_dceErrTxt(code));
	    exit(1);
	}
    }
    CATCH_ALL {
	char *dcesptr;

	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	    dcesptr = (char *)dce_sprintf(lsv_s_unknown_prefix, THIS_CATCH);
	    strcpy(secGroup , dcesptr);
	    free(dcesptr);
	    excNameP = secGroup;
	}
	dcesptr = (char *)dce_sprintf(lsv_s_unex_excptn_raised, excNameP);
	strcpy(adminPath , dcesptr);
	free(dcesptr);
	dce_fprintf(stderr, lsv_s_unexpected_excptn_perror , excNameP, errno);
	cmd_explanation = dce_msg_get_msg(lsv_s_log_unex_exception, &st);
	Log( (char *) cmd_explanation, excNameP, errno);
	free(cmd_explanation);

	exit(1);
    }
    ENDTRY
}

/* Main server module */
main (argc, argv)
int	argc;
char	**argv;
{
  struct cmd_syndesc *ts;
  sigset_t		signalBlockSet;

  osi_setlocale(LC_ALL, "");


    initialize_svc();


  /*
   * block SIGUSR1 for all threads. The ICL thread that wiil be
   * waiting for it will clear its mask by using sigwait()
   */
  (void)sigemptyset(&signalBlockSet);
  (void)sigaddset(&signalBlockSet, SIGUSR1);
  (void)sigprocmask(SIG_BLOCK, &signalBlockSet, (sigset_t *)NULL);

  SetupICL();

cmd_explanation = dce_msg_get_msg(lsv_s_create_flsvr_driver, &st);
  ts = cmd_CreateSyntax((char *)0, flserverDriver, (char *)0,
(char *) cmd_explanation);
 free(cmd_explanation);


cmd_explanation = dce_msg_get_msg(lsv_s_add_admin_filename, &st);
  cmd_AddParm(ts, "-adminlist", CMD_SINGLE, CMD_OPTIONAL,
(char *) cmd_explanation);
free(cmd_explanation);


cmd_explanation = dce_msg_get_msg(lsv_s_add_boring_stuff, &st);
  cmd_AddParm(ts, "-verbose", CMD_FLAG, CMD_OPTIONAL,
(char *) cmd_explanation);
free(cmd_explanation);


  return cmd_Dispatch(argc, argv);
}

void Log(s, a, b, c, d, e, f, g, h, i, j)
char *s;
{
    unsigned long NOW;
    struct tm *NowP;
    static char *months[12] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    NOW = osi_Time();
    NowP = (struct tm *) localtime((time_t *)&NOW);
    fprintf(logIO, "%02d-%s-%02d %02d:%02d:%02d ",
	     (NowP->tm_year) % 100,
	     months[NowP->tm_mon],
	     NowP->tm_mday,
	     NowP->tm_hour, NowP->tm_min, NowP->tm_sec);
    fprintf(logIO, s, a, b, c, d, e, f, g, h, i, j);
    putc('\n', logIO);
    fflush(logIO);
}
