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
 * $Log: main.c,v $
 * Revision 1.1.97.1  1996/10/02  16:58:47  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:54  damon]
 *
 * $EndLog$
 */
/*
 * main.c -- bak startup
 */ 

#include <dcedfs/stds.h>  /* include appropriate standards file */
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/cmd.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dce/secsts.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <com_err.h>
#include <dcedfs/flserver.h>
#include <dcedfs/flclient.h>
#include <dcedfs/volc.h>
#include <dcedfs/tcdata.h>
#include <error_macros.h>
#include <bc.h>	    
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

/*
 * The following externs should be replaced by C prototypes
 */
extern struct udbHandleS udbHandle;
extern int BC_ExecuteRequest();
extern int bc_AddDumpCmd();
extern int bc_AddHostCmd();
extern int bc_AddVolEntryCmd();
extern int bc_AddVolSetCmd();
extern int bc_DeleteDumpCmd();
extern int bc_DeleteHostCmd();
extern int bc_DeleteVolEntryCmd();
extern int bc_DeleteVolSetCmd();
extern int bc_DiskRestoreCmd();
extern int bc_DumpCmd();
extern int bc_GetTapeStatusCmd();
extern int bc_JobsCmd();
extern int bc_KillCmd();
extern int bc_LabelTapeCmd();
extern int bc_ListDumpScheduleCmd();
extern int bc_ListHostsCmd();
extern int bc_ListVolSetCmd();
extern int bc_QuitCmd();
extern int bc_ReadLabelCmd();
extern int bc_ScanDumpsCmd();
extern int bc_SetExpCmd();
extern int bc_VolRestoreCmd();
extern int bc_VolSetRestoreCmd();
extern int bc_dblookupCmd();

extern int bc_dbVerifyCmd();
extern int bc_deleteDumpCmd();
extern int bc_dumpInfoCmd();
extern int bc_restoreDbCmd();
extern int bc_saveDbCmd();

/*
 * Global configuration information for the Bak Coordinator.
 */

/* The following two #defines are for testing purposes and should not be
 * used under normal circumstances.
 */
/*
#define allow_multiple_bakservers
#define BAK_BAKSERVER_GROUP_OFFSET		15
*/
#define BAK_RESTOREDISK_NEWSERVER_OFFSET	8

struct bc_config *bc_globalConfig;	/*Ptr to global BC configuration info*/
struct ubik_client *cstruct;		/*Ptr to Ubik client structure*/
char *whoami = "bak";
 
int nonInteractiveBak;			/*Is bak in non-interactive mode?*/

DCE_SVC_DEFINE_HANDLE(bak_svc_handle, bak_svc_table, "bak")

void initialize_svc()
{
  error_status_t st;

  bak_svc_handle = dce_svc_register(bak_svc_table,  (idl_char *)"bak",  &st);
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

  dce_msg_define_msg_table(bak__table,
        sizeof bak__table / sizeof bak__table[0], &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Warning Cannot register bak incore message table 0x%lx\
n", st);
}


/* Return true if line is all whitespace */
static LineIsBlank(aline)
    register char *aline;

{ /*LineIsBlank*/

    register int tc;

    while (tc = *aline++)
	if (tc != ' ' && tc != '\t' && tc != '\n')
	    return(0);
    return(1);

} /*LineIsBlank*/

static FWait(afile)
    FILE *afile;

{ /*FWait*/

    register long code;
    fd_set in;

    FD_ZERO(&in); 
    FD_SET(fileno(afile), &in);

    while (1) {
	if (afile->_cnt > 0)
	    break;
	code = select ((int)1, &in, (fd_set *)0, (fd_set *)0, 
		       (struct timeval *) 0);
	if (code > 0)
	    break;

    }

    return(0);

} /*FWait*/

/* bc_InitTextConfig
 *	initialize configuration information that is stored as text blocks
 */

long
bc_InitTextConfig()
{
    udbClientTextP ctPtr;
    int i;
    long code = 0;

    extern struct bc_config *bc_globalConfig;
    /* initialize the client text structures */
    ctPtr = &bc_globalConfig->configText[0];
    for ( i = 0; i < TB_NUM; i++ )  {
	bzero((char *)ctPtr, sizeof(*ctPtr));
	ctPtr->textType = i;
	ctPtr++;
    }
    code = bc_GetDumpSchedule();
    if ( code )
	return(code);
    code = bc_GetVolumeSet();
    if ( code )
	return(code);
    code = bc_GetHosts();
    return(code);
}

extern statusP createStatusNode();

#define minimum(x,y) (((x) < (y))? (x):(y))
StatusNodesInit() {
  register struct bc_hostEntry *tentry;
  register int i;
  volatile register int count, index;
  unsigned long taskId;
  volatile unsigned long flags, code;
  rpc_binding_handle_t * volatile bindingArray;
  long * volatile portArray;
  error_status_t st;
  volatile tciStatusS status;
  volatile statusP statusPtr;

  lock_ObtainRead(&(bc_globalConfig->hostLock));
  TRY {
    for (count=0, tentry = bc_globalConfig->tapeHosts; tentry; 
	 count++,tentry = tentry->next );
  } 	
  FINALLY {
    lock_ReleaseRead(&(bc_globalConfig->hostLock));
  } ENDTRY
  if (!count) {
    return 0;
  }
  bindingArray = (rpc_binding_handle_t *)osi_Alloc(count * \
					       sizeof(rpc_binding_handle_t));
  if (bindingArray == (rpc_binding_handle_t *)NULL) {
    return BC_NOMEM;
  }
  portArray = (long *)osi_Alloc(count * \
				sizeof(long));
  if (portArray == (long *)NULL)
    return BC_NOMEM;
  lock_ObtainWrite(&(bc_globalConfig->hostLock));
  TRY {
    for (index = 0, tentry = bc_globalConfig->tapeHosts; tentry, index< count; 
	 index++, tentry = tentry->next ) {
      rpc_binding_copy(tentry->binding,
		(rpc_binding_handle_t *)&(bindingArray[index]), &st);
      portArray[index] = tentry->portOffset;
      rpc_binding_reset(bindingArray[index], &st);
#ifdef DEBUG
      compat_print_binding("\nStatusNodesInit binding: ", bindingArray[index]);
      fflush(stdout); fflush(stderr);
#endif /* DEBUG */
    }
  } FINALLY {
    lock_ReleaseWrite(&(bc_globalConfig->hostLock));
  } ENDTRY

  for(i=0;i<index;i++) {
    flags = TSK_STAT_FIRST;
    taskId = 0;
    while ( (flags & TSK_STAT_END) == 0 ) {
      TRY {
	code = BUTC_ScanStatus(bindingArray[i], &taskId,
			(tciStatusS *) &status, (unsigned long *) &flags);
      } CATCH_ALL {
	code = BC_EXCEPTION;
      } ENDTRY
      if ( IS_COMM_ERROR(code) ) {
	if ( code == TC_NOTASKS )
	  break;
	break;
      }
      /* Check if task is already done, and if so, do not add to 
	 status nodes */ 
      if (status.flags & TASK_DONE) {
	flags &= ~TSK_STAT_FIRST; /* turn off flag */
	continue; 
      }
      statusPtr = createStatusNode();
      if (statusPtr == (statusP)NULL)
	return ENOMEM;
      lock_ObtainWrite(&statusQueueLock);
      TRY {
	statusPtr->taskId = status.taskId;
	statusPtr->port = portArray[i];
	/* Only 1 listener, so reduce refcount to 1 */
	statusPtr->refCount = 1;
	statusPtr->flags = status.flags & ~STARTING;
	statusPtr->jobNumber =  bc_jobNumber();
	strncpy(statusPtr->taskName, (const char *)status.taskName,
		minimum(64, TC_MAXNAMELEN));
	statusPtr->opType = status.info.opType;
	switch(statusPtr->opType) {
	case TCOP_DUMP:
	case TCOP_RESTORE:
	  strncpy(statusPtr->volumeName,
		  (const char *)status.info.u.vol.volumeName, 
		  minimum(TC_MAXNAMELEN, BU_MAXNAMELEN));
	  statusPtr->nKBytes = status.info.u.vol.nKBytes;
	  statusPtr->volsFailed = status.info.u.vol.volsFailed;
	  break;
	case TCOP_READLABEL:
	case TCOP_LABELTAPE:
	  strncpy((char *)statusPtr->tapeName, 
		  (char *)status.info.u.label.tapeLabel.name, 
		  minimum(TC_MAXTAPELEN, BU_MAXTAPELEN));
	  statusPtr->tapeSize = status.info.u.label.tapeLabel.size;
	  statusPtr->tapeSize_ext = status.info.u.label.tapeLabel.size_ext;
	}
      } FINALLY {
	lock_ReleaseWrite(&statusQueueLock);
      } ENDTRY
      flags &= ~TSK_STAT_FIRST; /* turn off flag */
    }
  }
  for(i=0;i<index;i++)
    bc_FreeConn(bc_globalConfig, bindingArray[i]);
  osi_Free(bindingArray, count*sizeof(rpc_binding_handle_t));
  return 0;
}

/*
 * budbInitialize
 *	Setup ubik client structures to bakserver
 */

static long
budbInitialize (cellName)
     unsigned char *cellName;
{
    char *budbNsGroupP=0;
    long code = 0;
    char sfxBuf[200];
    int useNoAuth, useLocalAuth;
    unsigned long st, oldAuth;
    char *rn = "budbInitialize";

    useNoAuth = useLocalAuth = 0;
    dfs_GetBakJunctionName ((char *)cellName, (char *)sfxBuf, 
			    (int)sizeof(sfxBuf), (unsigned32 *)&st);
    if (st != 0) {
      if ( st == sec_login_s_no_current_context || st == rpc_s_auth_tkt_expired || st == sec_rgy_server_unavailable ) {
	dce_svc_printf(BAK_S_UNAUTH_CONN_MSG, whoami, cellName, dfs_dceErrTxt(st));
	useNoAuth = 1;
	rpc_ns_set_authn(0, &oldAuth, &st); /* turn off authentication */
	if (st != rpc_s_ok) {
	  dce_svc_printf(BAK_S_UNABLE_UNAUTH_MSG, whoami, dfs_dceErrTxt(st));
	  return st;
	}
      }
      dce_svc_printf(BAK_S_NO_JUNCTION_MSG, whoami, cellName, dfs_dceErrTxt(st));
      return(st);
    }

    budbNsGroupP = (char *)osi_Alloc((strlen((const char *)cellName)+strlen(sfxBuf)+1)*
				   sizeof(char));
    if (!budbNsGroupP)
    {
	dce_svc_printf(BAK_S_ERR_MEM_ALLOC_MSG, whoami, rn);
	return(BC_NOMEM);
    }
    sprintf(budbNsGroupP, "%s%s", cellName, sfxBuf);
    /* For now, lets get ubik_ClientInit to do the auth initialization. Later
       on, we may have to do the initialization much before (to catch 
       unauthenticated calls) before we get to ubik_ClientInit. Then we may
       have to change the initAuth parameter */
    code = ubik_ClientInit(NULL,
                           (unsigned_char_t *)budbNsGroupP, 
			   (unsigned_char_t *)"subsys/dce/dfs-bak-servers",
			   useNoAuth, useLocalAuth, 1, 
			   (struct ubik_client **)&udbHandle.uh_client);
    if (code)
	goto error_exit;

  error_exit:
    if (budbNsGroupP) 
      osi_Free(budbNsGroupP, (strlen((const char *)cellName)+strlen(sfxBuf)+1));
    return (code);
}

#ifdef allow_multiple_bakservers
static long
multi_budbInitialize (cellName, bakGroup)
     unsigned char *cellName;
     char *bakGroup;
{
    char *budbNsGroupP=0;
    long code = 0;
    char sfxBuf[200];
    int useNoAuth, useLocalAuth;
    unsigned long st, oldAuth;
    char *rn = "budbInitialize";

    useNoAuth = useLocalAuth = 0;
    /* For now, lets get ubik_ClientInit to do the auth initialization. Later
       on, we may have to do the initialization much before (to catch 
       unauthenticated calls) before we get to ubik_ClientInit. Then we may
       have to change the initAuth parameter */
    code = ubik_ClientInit(NULL,
                           (unsigned_char_t *)bakGroup, 
			   (unsigned_char_t *)"subsys/dce/dfs-bak-servers",
			   useNoAuth, useLocalAuth, 1, 
			   (struct ubik_client **)&udbHandle.uh_client);
    if (code)
      return (code);
}
#endif /* allow_multiple_bakservers */

/*
 * fldbInitialize
 *	Setup ubik client structures to flserver
 */
static long
fldbInitialize (cellName)
     unsigned char *cellName;

{
    char *fldbNsGroupP=0;
    long code = 0;
    unsigned long st, oldAuth;
    int clen;
    char sfxBuf[200];
    int useNoAuth, useLocalAuth;
    char *rn = "fldbInitialize";

    useNoAuth = useLocalAuth = 0;
    dfs_GetJunctionName((char *)cellName, 0, sfxBuf, sizeof(sfxBuf), &st);
    if (st != 0) {
      if ( st == sec_login_s_no_current_context || st == rpc_s_auth_tkt_expired || st == sec_rgy_server_unavailable ) {
	dce_svc_printf(BAK_S_UNAUTH_CONN_FLDB_MSG, whoami, cellName, dfs_dceErrTxt(st));
	useNoAuth = 1;
	rpc_ns_set_authn(0, &oldAuth, &st); /* turn off authentication */
	if (st != rpc_s_ok) {
	  dce_svc_printf(BAK_S_UNABLE_UNAUTH_MSG, whoami, dfs_dceErrTxt(st));
	  return st;
	}
      }
      dce_svc_printf(BAK_S_NO_JUNCTION_MSG, whoami, cellName, dfs_dceErrTxt(st));
      return(st);
    }
    fldbNsGroupP = (char *)osi_Alloc((strlen((const char *)cellName)+strlen(sfxBuf)+1)*
				   sizeof(char));
    if (!fldbNsGroupP)
    {
	dce_svc_printf(BAK_S_ERR_MEM_ALLOC_MSG, whoami, rn);
	return(BC_NOMEM);
    }
    sprintf(fldbNsGroupP, "%s%s", cellName, sfxBuf);
    /* For now, lets get ubik_ClientInit to do the auth initialization. Later
       on, we may have to do the initialization much before (to catch 
       unauthenticated calls) before we get to ubik_ClientInit. Then we may
       have to change the initAuth parameter */
    code = ubik_ClientInit( NULL,
                            (unsigned_char_t *)fldbNsGroupP, 
			    (unsigned_char_t *)"subsys/dce/dfs-fs-servers",
			    useNoAuth, useLocalAuth, 1, 
			   (struct ubik_client **)&cstruct);
    if (code)
	goto error_exit;

  error_exit:
    if (fldbNsGroupP) 
      osi_Free(fldbNsGroupP, (strlen((const char *)cellName)+strlen(sfxBuf)+1));
    return (code);
}

/*
 * bak_ClientInit
 *	setup connections to bakserver and flserver
 */
long bak_ClientInit(bakGroup)
  char *bakGroup;
{ 
    static char	rn[] = "bak_ClientInit";   /*Routine name*/
    char *cellName;
    long code;				    /*Error/return code*/
    extern statusWatcher();
    pthread_t watcher, statusIniter;
    pthread_attr_t threadAttr;

    initialize_cmd_error_table();
    dce_cf_get_cell_name( &cellName, &code);
    if (code)
	return (code);
    cstruct = (struct ubik_client *)0;

    /*
     * Initialize ubik client structures to the flserver and bakserver. Its
     * unfortunate that we have to initialize our ubik client structures to
     * the flserver twice. This is because we call VL RPCs directly as well
     * as use some of the vldb_* routines provided by fts. fts wants its own
     * ubik client structures setup, and we can't use that structure directly.
     * We should change our scheme to use the vldb_* routines instead of VL
     * RPCs, but until then we have both vldb_InitUbikHandle and fldbInitialize
     */
    code = fldbInitialize(cellName);
    if (code)
	return(code);
#ifdef allow_multiple_bakservers
    if (bakGroup)
      code = multi_budbInitialize(cellName, bakGroup);
    else
#endif /* allow_multiple_bakservers */
      code = budbInitialize(cellName);
    if (code)
	return(code);
    code = vldb_InitUbikHandle(cellName);
    if (code)
	return(code);
    (void)free(cellName);

    code = ubik_Call(BUDB_GetInstanceId,  udbHandle.uh_client, UF_SYNC_SITE,
                     &udbHandle.uh_instanceId);
    if ( code )
	return(code);

    code = bc_InitTextConfig();
    if ( code )
	return(code);
    code = pthread_attr_create(&threadAttr);
    if (code != 0) {
      dce_svc_printf(BAK_S_CANT_CREATE_INITER_MSG, whoami, errno);
      return(BC_THREAD);
    }
    code = pthread_attr_setstacksize(&threadAttr, 
				     BC_DEFAULT_STACK_SIZE);
    if (code != 0) {
      dce_svc_printf(BAK_S_CANT_CHANGE_INITER_MSG, whoami, errno);
      return(BC_THREAD);
    }
    if (!nonInteractiveBak) {
      code = pthread_create(&statusIniter, threadAttr,
			    (pthread_startroutine_t)StatusNodesInit,
			    (pthread_addr_t)0);
      if (IS_PTHR_ERROR(code)) {
	dce_svc_printf(BAK_S_CANT_CREATE_INITER_TASK_MSG, whoami, errno);
	return(BC_THREAD);
      }
    }

    code = pthread_create(&watcher, threadAttr,
			  (pthread_startroutine_t)statusWatcher,
			  (pthread_addr_t)0);
    if (IS_PTHR_ERROR(code))
    {
	dce_svc_printf(BAK_S_CANT_CREATE_WATCHER_MSG, whoami, errno);
	return(BC_THREAD);
    }
    return(0);
} 

/*
 * bak_CallBeforeProc
 *	This procedure called on every cmd_Dispatch before calling the actual
 *	bak routine servicing the command.
 *
 *	arock is 1 if non-interactive, else 0
 */
int bak_CallBeforeProc(as, arock)
  register struct cmd_syndesc *as;
  char *arock;
{
    if(nonInteractiveBak&& strncmp(as->name, "help", strlen(as->name)) != 0 && 
       strncmp(as->name, "apropos", strlen(as->name)) != 0 &&
       strncmp(as->name, "-help", strlen(as->name)) != 0 &&
       as->parms[CMD_HELPPARM].items == 0) {
#ifdef allow_multiple_bakservers
      if (as->parms[BAK_BAKSERVER_GROUP_OFFSET].items && 
	  as->parms[BAK_BAKSERVER_GROUP_OFFSET].items->data)
	return (bak_Init(as->parms[BAK_BAKSERVER_GROUP_OFFSET].items->data));
      else
	return (bak_Init(getenv("DFS_BAKSERVER_GROUP_NAME")));
#else
    return (bak_Init((char *)NULL));
#endif /* allow_multiple_bakservers */
    }
    return 0;
}

/*
 * bak_Init
 *	Make sure we mark down when we need to exit the Bak Coordinator.
 *	Specifically, we don't want to continue when our help and apropos
 *	routines are called from the command line.
 */
int bak_Init(bakGroup) 
  char *bakGroup;
{
    long code;
    bc_globalConfig=(struct bc_config *)osi_Alloc(sizeof(struct bc_config));
    if (bc_globalConfig == (struct bc_config *)NULL) {
	dce_svc_printf(BAK_S_CANT_INIT_BAK_MSG, whoami, code);
	return (BC_NOMEM);
    }
    bzero((char *)bc_globalConfig, sizeof(struct bc_config));
    lock_Init(&(bc_globalConfig->hostLock));
    lock_Init(&(bc_globalConfig->vsetLock));
    lock_Init(&(bc_globalConfig->dschedLock));
    code = bak_ClientInit(bakGroup);
    if(code){
      com_err(whoami, code, 
	      "; Unable to communicate with the bakserver");
      return(code);
    }
    return 0;
}

void bak_addDefaultParams(ts)
  register struct cmd_syndesc *ts;
{
#ifdef allow_multiple_bakservers
  /* check for the bakgroup environment variable */
  cmd_Seek(ts, BAK_BAKSERVER_GROUP_OFFSET);

  error_status_t st;


  cmd_explanation = dce_msg_get_msg(bak_s_bak_group_name, &st);
  cmd_AddParm(ts, "-bakgroup", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation );
  free(cmd_explanation);

#endif /* allow_multiple_bakservers */
}

#define	MAXV	100			    /* max number of cmd line args */
main(argc, argv)
    int    argc;
    char **argv;

{ /*main*/

    register int i;
    static char	rn[] = "main";		    /*Routine name*/
    char *targv[MAXV];			    /*Ptr to parsed argv stuff*/
    long targc;				    /*Num parsed arguments*/
    long code;				    /*Return code*/
    char lineBuffer[1024];		    /*Line typed in by user*/
    char *tp;				    /*Result of gets()*/
    register struct cmd_syndesc	*ts;	    /*Ptr to parsed command line*/
    char *grpName;			    /*alternate bakserver group name*/
    error_status_t st;
    unsigned char *cmd_explanation;


    osi_setlocale(LC_ALL, "");    
    initialize_svc();
    cmd_SetBeforeProc(bak_CallBeforeProc, (char *)0);


  cmd_explanation = dce_msg_get_msg(bak_s_start_dump, &st);
    ts = cmd_CreateSyntax("dump", bc_DumpCmd, 0, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_fileset_family_name, &st);
    cmd_AddParm(ts, "-family", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_dump_level, &st);
    cmd_AddParm(ts, "-level", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tcid, &st);
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_noaction, &st);
    cmd_AddParm(ts, "-noaction", CMD_FLAG, CMD_OPTIONAL, 
  (char *) cmd_explanation );
  free(cmd_explanation);
    bak_addDefaultParams(ts);

    ts = cmd_CreateSyntax("restoreftfamily", bc_VolSetRestoreCmd, 0,
			  "restore a dump set");
    cmd_AddParm(ts, "-family", CMD_SINGLE, CMD_REQUIRED,
		"fileset family name");
    cmd_AddParm(ts, "-file", CMD_SINGLE, CMD_REQUIRED, "file name");
    cmd_MakeAlternate(ts, "-family", "-file");
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL, "TC number");
    cmd_AddParm(ts, "-noaction", CMD_FLAG, CMD_OPTIONAL,
		"don't really execute it");
    bak_addDefaultParams(ts);
    cmd_CreateAlias(ts, "familyrestore");


  cmd_explanation = dce_msg_get_msg(bak_s_restoreft, &st);
    ts = cmd_CreateSyntax("restoreft", bc_VolRestoreCmd, 0, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_destination_server, &st);
    cmd_AddParm(ts,"-server", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_dest_aggregate, &st);
    cmd_AddParm(ts, "-aggregate", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_fileset_restore, &st);
    cmd_AddParm(ts, "-fileset", CMD_LIST, CMD_REQUIRED, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_extension, &st);
    cmd_AddParm(ts, "-extension", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_date, &st);
    cmd_AddParm(ts, "-date", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tcid, &st);
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_noaction, &st);
    cmd_AddParm(ts, "-noaction", CMD_FLAG, CMD_OPTIONAL, 
  (char *) cmd_explanation );
  free(cmd_explanation);

    bak_addDefaultParams(ts);
    cmd_CreateAlias (ts, "ftrestore");
    

  cmd_explanation = dce_msg_get_msg(bak_s_restoredisk, &st);
    ts = cmd_CreateSyntax("restoredisk", bc_DiskRestoreCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_restore_server, &st);
    cmd_AddParm(ts, "-server", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_rest_aggregate, &st);
    cmd_AddParm(ts, "-aggregate", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tcid, &st);
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    cmd_Seek(ts, BAK_RESTOREDISK_NEWSERVER_OFFSET);

  cmd_explanation = dce_msg_get_msg(bak_s_newserver, &st);
    cmd_AddParm(ts, "-newserver", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_newaggregate, &st);
    cmd_AddParm(ts, "-newaggregate", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_noaction, &st);
    cmd_AddParm(ts, "-noaction", CMD_FLAG, CMD_OPTIONAL, 
  (char *) cmd_explanation );
  free(cmd_explanation);

    bak_addDefaultParams(ts);
    cmd_CreateAlias (ts, "dkrestore");


  cmd_explanation = dce_msg_get_msg(bak_s_quit, &st);
    ts = cmd_CreateSyntax("quit", bc_QuitCmd, 0, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_addhost, &st);
    ts = cmd_CreateSyntax("addhost", bc_AddHostCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tapehost, &st);
    cmd_AddParm(ts, "-tapehost", CMD_SINGLE, CMD_REQUIRED,
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tcid, &st);
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_rmhost, &st);
    ts = cmd_CreateSyntax("rmhost", bc_DeleteHostCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tcid, &st);
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_lshosts, &st);
    ts = cmd_CreateSyntax("lshosts", bc_ListHostsCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);

    bak_addDefaultParams(ts);

  cmd_explanation = dce_msg_get_msg(bak_s_jobs, &st);
    ts = cmd_CreateSyntax("jobs", bc_JobsCmd, 0, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_kill, &st);
    ts = cmd_CreateSyntax("kill", bc_KillCmd, 0, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_job, &st);
    cmd_AddParm(ts, "-job",CMD_SINGLE,CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);



  cmd_explanation = dce_msg_get_msg(bak_s_lsftfamilies, &st);
    ts = cmd_CreateSyntax("lsftfamilies", bc_ListVolSetCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_family, &st);
    cmd_AddParm(ts, "-family", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_lsdumps, &st);
    ts = cmd_CreateSyntax("lsdumps", bc_ListDumpScheduleCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);

    bak_addDefaultParams(ts);

  cmd_explanation = dce_msg_get_msg(bak_s_addfamily, &st);
    ts = cmd_CreateSyntax("addftfamily", bc_AddVolSetCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_family, &st);
    cmd_AddParm(ts, "-family", CMD_SINGLE, CMD_REQUIRED,
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_status, &st);
    ts = cmd_CreateSyntax("status", bc_GetTapeStatusCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tcid, &st);
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_rmftfamily, &st);
    ts = cmd_CreateSyntax("rmftfamily", bc_DeleteVolSetCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_family, &st);
    cmd_AddParm(ts, "-family", CMD_LIST, CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_addftentry, &st);
    ts = cmd_CreateSyntax("addftentry", bc_AddVolEntryCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_family, &st);
    cmd_AddParm(ts, "-family", CMD_SINGLE, CMD_REQUIRED,
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_server_name, &st);
    cmd_AddParm(ts, "-server", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_aggregate_name, &st);
    cmd_AddParm(ts, "-aggregate", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_fileset_addftentry, &st);
    cmd_AddParm(ts, "-fileset", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation );
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_rmftentry, &st);
    ts = cmd_CreateSyntax("rmftentry", bc_DeleteVolEntryCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_family, &st);
    cmd_AddParm(ts, "-family", CMD_SINGLE, CMD_REQUIRED,
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_entry, &st);
    cmd_AddParm(ts, "-entry", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_adddump, &st);
    ts = cmd_CreateSyntax("adddump", bc_AddDumpCmd, 0, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_level, &st);
    cmd_AddParm(ts, "-level", CMD_LIST, CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_expires, &st);
    cmd_AddParm(ts, "-expires", CMD_LIST, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_rmdump, &st);
    ts = cmd_CreateSyntax("rmdump", bc_DeleteDumpCmd,0,
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_level, &st);
    cmd_AddParm(ts, "-level", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_labeltape, &st);
    ts = cmd_CreateSyntax("labeltape", bc_LabelTapeCmd, 0, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tape_desc, &st);
    cmd_AddParm(ts, "-tape", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_size, &st);
    cmd_AddParm(ts, "-size", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tcid, &st);
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_readlabel, &st);
    ts = cmd_CreateSyntax("readlabel", bc_ReadLabelCmd,0,
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tcid, &st);
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_scantape, &st);
    ts = cmd_CreateSyntax("scantape", bc_ScanDumpsCmd, 0,
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_dbadd, &st);
    cmd_AddParm(ts, "-dbadd", CMD_FLAG, CMD_OPTIONAL,
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tcid, &st);
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_ftinfo, &st);
    ts = cmd_CreateSyntax("ftinfo", bc_dblookupCmd, 0,
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_fileset_ftinfo, &st);
    cmd_AddParm(ts, "-fileset", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_setexp, &st);
    ts = cmd_CreateSyntax("setexp", bc_SetExpCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_level, &st);
    cmd_AddParm(ts, "-level", CMD_LIST, CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_expires, &st);
    cmd_AddParm(ts, "-expires", CMD_LIST, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_savedb, &st);
    ts = cmd_CreateSyntax("savedb", bc_saveDbCmd, 0,
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tcid, &st);
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_restoredb, &st);
    ts = cmd_CreateSyntax("restoredb", bc_restoreDbCmd, 0,
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_tcid, &st);
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);
    cmd_CreateAlias(ts, "dbrestore");


  cmd_explanation = dce_msg_get_msg(bak_s_dumpinfo, &st);
    ts = cmd_CreateSyntax("dumpinfo", bc_dumpInfoCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_ndumps, &st);
    cmd_AddParm(ts, "-ndumps", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_id, &st);
    cmd_AddParm(ts, "-id", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    /* ndumps and id are mutually exclusive */
    (void)cmd_MakeAlternate(ts, "-ndumps", "-id");

  cmd_explanation = dce_msg_get_msg(bak_s_verbose, &st);
    cmd_AddParm(ts, "-verbose", CMD_FLAG, CMD_OPTIONAL,
  (char *) cmd_explanation );
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_verifydb, &st);
    ts = cmd_CreateSyntax("verifydb", bc_dbVerifyCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_verbose_2, &st);
    cmd_AddParm(ts, "-verbose", CMD_FLAG, CMD_OPTIONAL, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);


  cmd_explanation = dce_msg_get_msg(bak_s_deletedump, &st);
    ts = cmd_CreateSyntax("deletedump", bc_deleteDumpCmd, 0, 
  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_id, &st);
    cmd_AddParm(ts, "-id", CMD_SINGLE, CMD_REQUIRED, 
  (char *) cmd_explanation);
  free(cmd_explanation);

    bak_addDefaultParams(ts);

    /*
     * status node management initialization
     */
    (void)initStatus();			/* setup status structures */
    (void)bc_InitDumpSlots();		/* zero out the dump slots */

    /*
     * Determine if we are interactive or non-interactive.   
     */
    
    if (argc > 1) {
#ifdef allow_multiple_bakservers
      if (argc == 3 && strncmp(argv[1], "-bakgroup", strlen(argv[1])) == 0 &&
	  argv[2] != (char *)NULL) {
	if (code=bak_Init(argv[2]))/* argv[2] contains the bakserver group */
	  exit(1);
      }
      else {
#endif /* allow_multiple_bakservers */
	nonInteractiveBak=1;
	code = cmd_Dispatch(argc, argv);
	if (code)
	    exit(1);
	exit(bc_WaitForJob()); /*wait, we're non-interactive*/
#ifdef allow_multiple_bakservers
      }
#endif /* allow_multiple_bakservers */
    }
#ifdef allow_multiple_bakservers
    else if (code = bak_Init(getenv("DFS_BAKSERVER_GROUP_NAME")))
#else
    else if (code = bak_Init((char *)NULL))
#endif /* allow_multiple_bakservers */
      /* default bakserver group */
	exit(1);
    while(1) {
	dce_printf(bak_s_prompt);
	fflush(stdout);
	/*Now, wait for characters on stdin*/
	FWait(stdin);
	tp = gets(lineBuffer);
	if (!tp) {
	    exit(0);	/* Found an EOF */
	}
	if (!LineIsBlank(lineBuffer)) {
	    code = cmd_ParseLine(lineBuffer, targv, &targc, MAXV);
	    if (code) {
		dce_svc_printf(BAK_S_PARSE_ERR_MSG,
			       whoami, dfs_dceErrTxt(code));
	    }
	    else {
		/*
		 * Line was parsed; act on it, then free things up.
		 */
		cmd_Dispatch(targc, targv);	
		cmd_FreeArgv(targv);
	    }
	}
    }
} /*main*/
