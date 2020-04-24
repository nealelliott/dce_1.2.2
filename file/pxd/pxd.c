/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pxd.c,v $
 * Revision 1.1.198.1  1996/10/02  18:13:50  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:45:46  damon]
 *
 * $EndLog$
 */
/*
 *      Copyright (C) 1991, 1995 Transarc Corporation
 *      All rights reserved.
 */

 /*
  * fxd
  *
  * Description:
  *	The Exporter startup process.  It is responsible for starting up the 
  *     kernel processes required by the Exporter module.
  *
  * Recognized flags are:
  *     -adminGroup group name.
  *	-mainprocs  Number of processes handling incoming rpc calls.
  *	-tokenprocs Number of processes handling the token revocation.
  *	-hostlife   Default client lifetime guarantee
  *	-hostrpc    Default client rpc life guarantee (>= hostlife)
  *	-pollinterval	Default poll interval for clients when server dies
  *     -maxlife    Maximum negotiated vallue for hostlife
  *	-maxrpc     Maximum vallue for hostrpc
  *	-notsr      No Token State Recovery Period for clients' requests.
  *	-verbose    Be chatty.
  *     -help
  */

#include <dcedfs/param.h>
#include <dcedfs/cmdAux.h>
#include <dcedfs/osi.h>
#include <dcedfs/syscall.h>
#include <dcedfs/ioctl.h>
#include <dcedfs/lock.h>
#include <dcedfs/nubik.h>
#include <dcedfs/common_data.h>
#include <dcedfs/compat.h>
#include <ctype.h>
#include <errno.h>
#include <dcedfs/fshs.h>
#include <dcedfs/px.h>
#include <dce/dnsmessage.h>
#include <dce/pgo.h>
#include <dce/secidmap.h>
#include <pthread.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsfxdmac.h>
#include <dfsfxdsvc.h>
#include <dfsfxdmsg.h>

dce_svc_handle_t   fxd_svc_handle;

void initialize_svc()
{
  error_status_t st;

  fxd_svc_handle = dce_svc_register(fxd_svc_table,  (idl_char *)"fxd",  &st);
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
  dfs_define_fxd_message_tables();
}

/*
 * Global configuration variables.
 */
char ExpLogFile[1024];			/* Full pathname of EXPLOGFILE */
int mainprocs = 4;			/* Default minimum possible */
int tokenprocs = 2;			/* Default minimum possible */
sec_rgy_name_t adminGroupName;		/* name of Sys Admin Group */
afsUUID adminGroupID;			/* UUID for administrator group */
char principalName[1024];		/* DCE host's principal name */
int fxd_verbose = 0;			/* Are we being chatty? */
int fxd_postRecovery = 1;		/* Wait for the TSR work */
int fxd_admingroup = 0;			/* Is the admingroup provided ? */

/*
 * set up structure for TSR recovery parameters:
 */
struct recovery_params recoveryParams = {
    2*60,	/* lifeGuarantee */
    2*60,	/* RPC Guarantee */
    3*60,	/* deadServer */
    1,		/* post-crash Recovery. TRUE by default */
    2*60,	/* maxLife */
    2*60	/* maxRPC */
};

static struct fshs_security_advice secAdvice = {
    FSHS_SEC_ADVICE_FORMAT_1,
    {
	rpc_c_protect_level_default,	/* local.minProtectLevel */
	rpc_c_protect_level_default	/* local.maxProtectLevel */
    },
    {
	rpc_c_protect_level_default,	/* nonLocal.minProtectLevel */
	rpc_c_protect_level_default	/* nonLocal.maxProtectLevel */
    },
};

static void CloseAllFileDescriptorsAfter(arg)
    void * arg;
{
    register int i;
    int nfiles;
    int after = (int)arg;

    /* amazingly enough, this triggers a Solaris 2 bug; close hangs! */
#ifndef AFS_SUNOS5_ENV
    nfiles = osi_GetMaxNumberFiles();
    for(i = after+1; i < nfiles; i++)
	(void) close(i);
#endif /* not AFS_SUNOS5_ENV */
}

/*
 * This routine is called to register the FX server's rpc binding with RPCD.
 * The complete string binding is actually provided by the KNCS runtime.
 *
 * Note: RPCD is running on all platforms that support DCE.
 */
extern rpc_if_handle_t AFS4Int_v4_0_s_ifspec;

static long RegisterWithRpcd(stringBinding) 
    char *stringBinding;
{
    rpc_binding_vector_t rpcbv;
    unsigned32 st = 0, xst;
    rpc_ep_inq_handle_t     inq_context;
    rpc_if_id_t             if_id, fx_if_id;
    unsigned32               error_status;
    rpc_binding_handle_t     binding;

    rpcbv.count = 1;
    rpc_binding_from_string_binding((unsigned char *)stringBinding,
				     &rpcbv.binding_h[0], &st);
    if (st != error_status_ok) { 
	return st;
    }

    /*
     * Delete all entries from the endpoint map database with the
     * same ifid. Note that rpc_ep_register() will NOT delete all
     * entries for a machine with multiple interfaces, so this step
     * is necessary. Also, there is no need to check if the old ser-
     * ver is still running, because if it had been, the registration
     * in the kernel rpc runtime would have failed. Therefore any
     * entry in the ep  has to be bogus and can be removed. 
     */

    /* Get my id */
    rpc_if_inq_id(AFS4Int_v4_0_s_ifspec, &fx_if_id, &xst);

    if (st != error_status_ok) {
	rpc_binding_free(&rpcbv.binding_h[0], &xst);
	return st;
    }

    rpc_mgmt_ep_elt_inq_begin (
			       NULL, /* same host */
			       rpc_c_ep_match_by_if,
			       &fx_if_id,
			       rpc_c_vers_all,
			       NULL, 
			       &inq_context,
			       &st);

    if (st != error_status_ok) {
	rpc_binding_free(&rpcbv.binding_h[0], &xst);
	return st;
    }

    while (1) {

      rpc_mgmt_ep_elt_inq_next(inq_context, &if_id, &binding,
			       NULL, NULL,  &st);

      if (st == rpc_s_no_more_elements) {
	break;
      }
      if (st != error_status_ok) {
	rpc_binding_free(&rpcbv.binding_h[0], &xst);
	return st;
      }

      /* 
       *  Check if interface uuid's are same. Isn't really necessary, since the 
       *  rpc_mgmt_fn should take care of this
       */
      
      if (!uuid_equal(&if_id.uuid, &fx_if_id.uuid, &error_status)
		    || (error_status != uuid_s_ok)) {
	rpc_binding_free(&rpcbv.binding_h[0], &xst);
	rpc_mgmt_ep_elt_inq_done (&inq_context, &xst);
	rpc_binding_free(&binding, &xst);
	return error_status;
      }

      /* found entry, deleted same */

      rpc_mgmt_ep_unregister(NULL, &if_id, binding, NULL, &st);

      if (st != error_status_ok) {
	rpc_binding_free(&rpcbv.binding_h[0], &xst);
	rpc_mgmt_ep_elt_inq_done (&inq_context, &xst);
	rpc_binding_free(&binding, &xst);
	return st;
      }
      /* free rpc binding */
      rpc_binding_free(&binding, &xst);
    }
	
    /* Now clean up */

    rpc_mgmt_ep_elt_inq_done (&inq_context, &xst);

    /*
     * Register the FX server interface with the local endpoint map database 
     */
    rpc_ep_register(AFS4Int_v4_0_s_ifspec, &rpcbv, NULL, 
		    (unsigned char *)"DFS file exporter", &st);
    if (st != error_status_ok) {
	rpc_binding_free(&rpcbv.binding_h[0], &xst);
	return st;
    }
    return 0;
}

static void SortHosts(hostp)
     register struct sockaddr *hostp;
{
/* 
 * Sort the hosts into some random order. 
 * Note, 16777213 is prime; Knuth volume 2 page 15 random generator  
 */
#define	ranstage(x)	(x)=((0xa0a0a0*(x)+314159) % 16777213)
    unsigned long rands[MAXHOSTSPERCELL];
    register int numHosts, changed, ix;
    unsigned long ranState;
    struct sockaddr hostTemp;

    ranState = osi_Time() ^ getpid();
    for (ix = 0; ix < 30; ++ix) ranstage(ranState);
    for (numHosts = 0; numHosts < MAXHOSTSPERCELL; ++numHosts) {
	if (hostp[numHosts].sa_family == 0) break;
	if (hostp[numHosts].sa_family == ((unsigned short) -1)) break;
    }
    /* either numHosts == MAXHOSTSPERCELL or hostp[numHosts] == 0 */
    for (ix = 0; ix < numHosts; ++ix) {
	ranstage(ranState);
	rands[ix] = ranState;
    }
    for (changed = 1; changed != 0; ) {
	changed = 0;
	for (ix = 1; ix < numHosts; ++ix) {
	    if (rands[ix-1] > rands[ix]) {
		hostTemp = hostp[ix-1];
		hostp[ix-1] = hostp[ix];
		hostp[ix] = hostTemp;
		ranState = rands[ix-1];
		rands[ix-1] = rands[ix];
		rands[ix] = ranState;
		changed = 1;
	    }
	}
    }
}

static long GetFlserverHandle(tcp)
struct fsop_cell *tcp;
{
    long i, bound, reTry = 1;
    unsigned long code = 0;
    char cellWithFS[450];
    char *cellName;
    char *cellNameP = 0;
    char *hostName;
    struct ubik_client *cstruct;
    struct afsNetAddr Socks[MAXHOSTSPERCELL];
    extern uuid_t uuid_nil;

    bzero((char *)tcp, sizeof(*tcp));
    dce_cf_get_cell_name(&cellNameP, &code);
    if (code) {
	dce_svc_printf(FXD_S_NO_LOCAL_CELL_NAME_MSG,
		       dfs_dceErrTxt(code));
	return code;
    }
    strncpy(tcp->cellName, cellNameP, sizeof(tcp->cellName));
    free(cellNameP);
    strncpy(cellWithFS, tcp->cellName, sizeof(cellWithFS));
    i = strlen(cellWithFS);
    dfs_GetJunctionName(tcp->cellName, 0, &cellWithFS[i],
			 sizeof(cellWithFS) - i, &code);
    if (code) {
       dce_svc_printf(FXD_S_NO_JUNCTION_NAME_MSG,
		      tcp->cellName, dfs_dceErrTxt(code));
       return code;
    }

    /*
     * In case CDS is not up yet, try ubik_ClientInit several times 
     * with patience. 
     */
    while (reTry) {
        code = ubik_ClientInit(NULL, 
			       (unsigned_char_t *)cellWithFS, 
			       (unsigned_char_t *)"subsys/dce/dfs-fs-servers", 
			       0, 1, 1, 
			       (struct ubik_client **)&cstruct);
	if (code == 0)
	    goto next;
	else 
	if (code != rpc_s_entry_not_found && code != DNS_SOCKET) 
	    break;
	else /* CDS may not be up ? */ {
	    /* 
	     * Let's put the entire process (fxd) to sleep for 10 secs
	     */
	    dce_svc_printf(FXD_S_TRYING_CONNECT_MSG, reTry);
	    sleep(10);
	    if (reTry++ > 10)  /* already try 100 secs */
	        break;
	}
    }
    dce_svc_printf(FXD_S_UBIK_CLIENT_INIT_FAILED_MSG,
		   cellWithFS, dfs_dceErrTxt(code));
    return code;

next:
    compat_get_group_object_uuid((unsigned_char_t *)cellWithFS,
				 &tcp->fsServerGp, &code);
    if ( code ) {
	dce_svc_printf(FXD_S_NO_OBJ_UUID_MSG, cellWithFS, dfs_dceErrTxt(code)); 
	dce_svc_printf(FXD_S_USING_NULL_UUID_MSG);
	tcp->fsServerGp = uuid_nil;
	code = 0;
    }
    bound = UBIK_MAXSERVERS;
    if (bound > MAXHOSTSPERCELL) bound = MAXHOSTSPERCELL;
    bzero((char *)Socks, sizeof(Socks));
    for (i = 0; i < bound; ++i) {
	if (cstruct->conns[i][0] == 0) break;
	/* There could be multiple IP addresses for a multi-homed machine.
	   These should be handled in the right way. For now we just consider
	   the first address
	 */
	code = rpcx_binding_to_sockaddr(cstruct->conns[i][0],
					(struct sockaddr_in *)&Socks[i]);
	if (code != rpc_s_ok) {
	    dce_svc_printf(FXD_S_BAD_BINDING_MSG,
			   i, cellWithFS, dfs_dceErrTxt(code));
	    break;
	}
    }
    bound = i;
    if (bound > MAXVLHOSTSPERCELL) bound = MAXVLHOSTSPERCELL;
    SortHosts(&Socks[0]);
    for (i = 0; i < bound; ++i)
	tcp->hosts[i] = Socks[i];
    /* Sigh--
     * Just discard the storage that cstruct points to; never refer to it again
     */

    /*
     * Get the DCE principal name for this host. Its format is like
     *       /.../<CellName>/<hostName>/dfs-server
     */
    dce_cf_get_host_name(&hostName, &code);
    if (code) {
	dce_svc_printf(FXD_S_NO_LOCAL_HOST_NAME_MSG, dfs_dceErrTxt(code));
	return code;
    }
    if (strlen(tcp->cellName) + strlen("//dfs-server") + 
	strlen(hostName) >= sizeof(principalName)) {
	dce_svc_printf(FXD_S_BUFFER_TOO_SMALL_MSG);
	code = -1;
	goto out;
    }

    (void) sprintf(principalName, "%s/%s/dfs-server", 
		   tcp->cellName, hostName);
out:
    free(hostName);
    return code;
}

/* Define the argument indices. */
#define FXDCMD_ADMINGROUP	0
#define FXDCMD_MAINPROCS		1
#define FXDCMD_TOKENPROCS	2
#define FXDCMD_HOSTLIFE		3
#define FXDCMD_HOSTRPC		4
#define FXDCMD_POLLINTERVAL	5
#define FXDCMD_MAXLIFE		6
#define FXDCMD_MAXRPC		7
#define FXDCMD_NOTSR			8
#define FXDCMD_MINLCLAUTHN	9
#define FXDCMD_MAXLCLAUTHN	10
#define FXDCMD_MINRMTAUTHN	11
#define FXDCMD_MAXRMTAUTHN	12
#define FXDCMD_VERBOSE		20

mainproc(as, arock)
     register struct cmd_syndesc *as;
     char *arock;
{
    register long code;			/*Result of fork()*/
    struct fsop_cell CellInfo;		/* arg block for PXOP_INITHOST */
    afsUUID localCellID;		/* This cell uuid */
    sec_rgy_name_t globalName;
    unsigned long st;
    long longArg;
    int syntaxError = 0;
    int lifeSet = 0;			/* -hostlife specified? */
    int rpcSet = 0;			/* -hostrpc specified? */
    int maxRpcSet = 0;			/* -maxrpc specified? */
    int printedError = 0;

    long error;

    /*
     * Parse the arguments 
     */
    if (as->parms[FXDCMD_ADMINGROUP].items) { /* CMD_REQUIRED */
        strcpy((char *)adminGroupName,
	       as->parms[FXDCMD_ADMINGROUP].items->data);
    }
    if (as->parms[FXDCMD_MAINPROCS].items) {
	mainprocs = atoi(as->parms[FXDCMD_MAINPROCS].items->data);
	if (mainprocs <= 0) {
	    (void) dce_svc_printf(FXD_S_BAD_MAINPROCS_MSG);
	    syntaxError++;
	}
    }
    if (as->parms[FXDCMD_TOKENPROCS].items) {
	tokenprocs = atoi(as->parms[FXDCMD_TOKENPROCS].items->data);
	if (tokenprocs <= 0) {
	    (void) dce_svc_printf(FXD_S_BAD_TOKENPROCS_MSG);
	    syntaxError++;
	}
    }

    if (as->parms[FXDCMD_HOSTRPC].items) {
	longArg=atoi(as->parms[FXDCMD_HOSTRPC].items->data);
	rpcSet++;
	if (longArg <= 0) {
	    (void) dce_svc_printf(FXD_S_BAD_HOSTRPC_MSG);
	    syntaxError++;
	}
	else
	    recoveryParams.RPCGuarantee = (unsigned long)longArg; ;
    }

    if (as->parms[FXDCMD_HOSTLIFE].items) {
	longArg = atoi(as->parms[FXDCMD_HOSTLIFE].items->data);
	lifeSet++;
	if (longArg <= 0) {
	    (void) dce_svc_printf(FXD_S_BAD_HOSTLIFE_MSG);
	    syntaxError++;
	}
	else {
	    recoveryParams.lifeGuarantee = (unsigned long)longArg;
	    if ((recoveryParams.lifeGuarantee > recoveryParams.RPCGuarantee) && !rpcSet) {
		(void) dce_svc_printf(FXD_S_HOSTLIFE_WARNING_MSG,
				      (as->parms[FXDCMD_MAXRPC].items ||
				       as->parms[FXDCMD_MAXLIFE].items) ? 
				      "hostrpc" : "hostrpc and maxrpc",
				      recoveryParams.lifeGuarantee);
		recoveryParams.RPCGuarantee = recoveryParams.lifeGuarantee;
		recoveryParams.maxRPC = recoveryParams.lifeGuarantee;
		rpcSet++;
	    }
	}
    }

    if (as->parms[FXDCMD_POLLINTERVAL].items) {
        longArg = atoi(as->parms[FXDCMD_POLLINTERVAL].items->data);
        if (longArg <= 0) {
	    (void) dce_svc_printf(FXD_S_BAD_POLLINTERVAL_MSG);
	    syntaxError++;
	}
	else
	    recoveryParams.deadServer = (unsigned long)longArg;
    }

    if (as->parms[FXDCMD_MAXRPC].items) {
	/* enforce max being >= rpc life, decreasing rpc as necessary */
	longArg = atoi(as->parms[FXDCMD_MAXRPC].items->data);
	if (longArg <= 0) {
	    (void) dce_svc_printf(FXD_S_BAD_MAXRPC_MSG);
	    syntaxError++;
	}
	else {
	    recoveryParams.maxRPC = (unsigned long)longArg;
	    if (recoveryParams.RPCGuarantee > recoveryParams.maxRPC) {
		if (rpcSet) {
		    dce_svc_printf(FXD_S_BAD_HOSTRPC2_MSG,
				   recoveryParams.RPCGuarantee,
				   recoveryParams.maxRPC);
		    syntaxError++;
		}
		else {
		    recoveryParams.RPCGuarantee = recoveryParams.maxRPC;
		    (void) dce_svc_printf(FXD_S_MAXRPC_WARNING_MSG,
					  recoveryParams.RPCGuarantee);
		}
		rpcSet++;		/* RPC implicitly set */
	    }
	    maxRpcSet++;
	}
    }
    else {
	/* RPClife <= maxRPC */
	if (recoveryParams.RPCGuarantee > recoveryParams.maxRPC) {
	    /* if rpc > max, make max match */
	    recoveryParams.maxRPC = recoveryParams.RPCGuarantee;
	    (void) dce_svc_printf(FXD_S_HOSTRPC_WARNING_MSG,
				  recoveryParams.maxRPC);
	    maxRpcSet++;
	}
    }	

    if (as->parms[FXDCMD_MAXLIFE].items) {
	/* enforce max being >= life, decreasing life as necessary */
	longArg = atoi(as->parms[FXDCMD_MAXLIFE].items->data);
	if (longArg <= 0) {
	    (void) dce_svc_printf(FXD_S_BAD_MAXLIFE_MSG);
	    syntaxError++;
	}
	else {
	    recoveryParams.maxLife = (unsigned long)longArg;
	    if (recoveryParams.lifeGuarantee > recoveryParams.maxLife) {
		if (lifeSet) {
		    dce_svc_printf(FXD_S_BAD_HOSTLIFE2_MSG,
				   recoveryParams.lifeGuarantee,
				   recoveryParams.maxLife);
		    syntaxError++;
		}
		else {
		    recoveryParams.lifeGuarantee = recoveryParams.maxLife;
		    (void) dce_svc_printf(FXD_S_MAXLIFE_WARNING_MSG,
					  recoveryParams.lifeGuarantee);
		}
		lifeSet++;		/* life implicitly set */
	    }
	    /* if maxlife is set, but maxrpc is not, may need to bump up maxrpc */
	    if ((recoveryParams.maxLife > recoveryParams.maxRPC) && !maxRpcSet) {
		(void) dce_svc_printf(FXD_S_MAXLIFE_WARNING2_MSG,
				      recoveryParams.maxLife);
		recoveryParams.maxRPC = recoveryParams.maxLife;
	    }
	}
    }
    else {
	/* lifetime <= maxLife */
	if (recoveryParams.lifeGuarantee > recoveryParams.maxLife) {
	    /* if life > max, make max match */
	    recoveryParams.maxLife = recoveryParams.lifeGuarantee;
	    (void) dce_svc_printf(FXD_S_HOSTLIFE_WARNING2_MSG,
				  recoveryParams.maxLife);
	}
    }

    if (as->parms[FXDCMD_MINLCLAUTHN].items) {
	if (compat_nameToAuthnLevel(
				    as->parms[FXDCMD_MINLCLAUTHN].items->data,
				    &secAdvice.local.minProtectLevel)) {
	    (void) dce_svc_printf(FXD_S_BAD_MINLCLAUTHN_MSG,
				  as->parms[FXDCMD_MINLCLAUTHN].items->data);
	    syntaxError++;
	}
    }
    if (as->parms[FXDCMD_MAXLCLAUTHN].items) {
	if (compat_nameToAuthnLevel(
				    as->parms[FXDCMD_MAXLCLAUTHN].items->data,
				    &secAdvice.local.maxProtectLevel)) {
	    (void) dce_svc_printf(FXD_S_BAD_MAXLCLAUTHN_MSG,
				  as->parms[FXDCMD_MAXLCLAUTHN].items->data);
	    syntaxError++;
	}
    }
    if (as->parms[FXDCMD_MINRMTAUTHN].items) {
	if (compat_nameToAuthnLevel(
				    as->parms[FXDCMD_MINRMTAUTHN].items->data,
				    &secAdvice.nonLocal.minProtectLevel)) {
	    (void) dce_svc_printf(FXD_S_BAD_MINRMTAUTHN_MSG,
				  as->parms[FXDCMD_MINRMTAUTHN].items->data);
	    syntaxError++;
	}
    }
    if (as->parms[FXDCMD_MAXRMTAUTHN].items) {
	if (compat_nameToAuthnLevel(
				    as->parms[FXDCMD_MAXRMTAUTHN].items->data,
				    &secAdvice.nonLocal.maxProtectLevel)) {
	    (void) dce_svc_printf(FXD_S_BAD_MAXRMTAUTHN_MSG,
				  as->parms[FXDCMD_MAXRMTAUTHN].items->data);
	    syntaxError++;
	}
    }
    if (secAdvice.local.minProtectLevel != rpc_c_protect_level_default
	&& secAdvice.local.maxProtectLevel != rpc_c_protect_level_default
	&& secAdvice.local.minProtectLevel > secAdvice.local.maxProtectLevel) {
	(void) dce_svc_printf(FXD_S_OVERLAPLCL_MSG,
			      secAdvice.local.minProtectLevel,
			      secAdvice.local.maxProtectLevel);
	    syntaxError++;
    }
    if (secAdvice.nonLocal.minProtectLevel != rpc_c_protect_level_default
	&& secAdvice.nonLocal.maxProtectLevel != rpc_c_protect_level_default
	&& secAdvice.nonLocal.minProtectLevel > secAdvice.nonLocal.maxProtectLevel) {
	    (void) dce_svc_printf(FXD_S_OVERLAPRMT_MSG,
			      secAdvice.nonLocal.minProtectLevel,
			      secAdvice.nonLocal.maxProtectLevel);
	    syntaxError++;
    }

    if (as->parms[FXDCMD_NOTSR].items) {
        fxd_postRecovery = 0;
    }
	
    if (as->parms[FXDCMD_VERBOSE].items) {
        /* -verbose */
        fxd_verbose = 1;
    }
    
    /* do some sanity checking */
	
    if (fxd_postRecovery) 
        recoveryParams.postRecovery = 1;
    else
        recoveryParams.postRecovery = 0;

    /* lifetime <= RPC Life */
    if (recoveryParams.lifeGuarantee > recoveryParams.RPCGuarantee) {
	dce_svc_printf(FXD_S_BAD_HOSTLIFE3_MSG, recoveryParams.lifeGuarantee,
		       recoveryParams.RPCGuarantee);
	syntaxError++;
	printedError++;
    }
    /* max lifetime <= max RPC Life */
    if ((recoveryParams.maxLife > recoveryParams.maxRPC) && !printedError) {
	dce_svc_printf(FXD_S_BAD_MAXLIFE2_MSG, recoveryParams.maxLife,
		       recoveryParams.maxRPC);
	syntaxError++;
    }

    if (syntaxError)
	exit(1);

    code = GetFlserverHandle(&CellInfo);
    if (code)
       exit(1);
    bzero((char *)&adminGroupID, sizeof(afsUUID));
    bzero((char *)&localCellID, sizeof(afsUUID));

    sec_rgy_pgo_name_to_id(sec_rgy_default_handle,
			   sec_rgy_domain_group,
			   adminGroupName,
			   &adminGroupID,
			   &st);
    if (st != rpc_s_ok) {
	dce_svc_printf(FXD_S_NO_SYSADMIN_GID_MSG, dfs_dceErrTxt(st));
	return st;
    }
    sprintf((char *)globalName, "%s/root", CellInfo.cellName);
    sec_id_parse_name(sec_rgy_default_handle,
		      globalName, 
		      NULL, 
		      &localCellID,
		      NULL,
		      NULL,
		      &st);
    if (st != rpc_s_ok) {
	dce_svc_printf(FXD_S_NO_LOCAL_CELL_UUID_MSG, dfs_dceErrTxt(st));
	return st;
    }
    
    if (fxd_verbose)
	dce_svc_printf(FXD_S_CALLING_PXOP_PUTKEYS_MSG);
    code = afs_syscall(AFSCALL_PX, PXOP_PUTKEYS,
		       (int)&adminGroupID, (int)&localCellID, 0, 0);
    if (code != 0) {
	dce_svc_printf(FXD_S_PXOP_PUTKEYS_FAILED_MSG, code, errno);
	exit(1);
    }

    if (fxd_verbose) {
	dce_svc_printf(FXD_S_CALLING_PXOP_INITHOST_MSG, CellInfo.cellName);
    }
    code = afs_syscall(AFSCALL_PX, PXOP_INITHOST,
		       (int)&CellInfo, (int)&recoveryParams, (int)&secAdvice, 0);
    if (code != 0) {
	dce_svc_printf(FXD_S_PXOP_INITHOST_FAILED_MSG, code, errno);
	exit(1);
    }

    if (fxd_verbose) 
	dce_svc_printf(FXD_S_INVOKE_SERVICES_MSG, mainprocs, tokenprocs);
    /*
     * Set up all the kernel processes needed for the Protocol Exporter.
     *
     * 1) Issue a system call to invoke and register PX server(s) in kernel.
     * 2) This call returns the rpc binding pertaining to the PX Interface.
     * 3) Register this binding with the local rpcd.
     */
    {  /* Perform this in this addr space rather than via a "fork" */

	char rpcString[256];
	struct afs_ioctl ioBuf;
	int Flags; 

	errno = 0;
	ioBuf.in = &principalName[0];
	ioBuf.in_size = strlen(principalName);
	ioBuf.out = &rpcString[0];
	ioBuf.out_size = 256;

	Flags = 0; 
	/*
	 * Invoke the PX server and cread a secondary thread pool for TKN 
	 */
	error = afs_syscall(AFSCALL_PX, PXOP_RPCDAEMON, Flags, mainprocs,
			    tokenprocs, (int)&ioBuf);
	if (error < 0) {
	    dce_svc_printf(FXD_S_INVOKE_SERVER_FAILED_2_MSG, error, errno);
	    exit(1);
	}
	code = RegisterWithRpcd(ioBuf.out);
	if (code) {
	    dce_svc_printf(FXD_S_REGISTER_BINDINGS_FAILED_2_MSG,
			   ioBuf.out, code);
	    exit(1);
	}
	if (fxd_verbose)
	    dce_svc_printf(FXD_S_SERVER_REGISTERED_MSG);
    }   /* PXOP_RPCDAEMON just for PX */

    /* close all file descriptors in children after forking */
    atfork((void *)2, NULL, NULL, CloseAllFileDescriptorsAfter);
    code = fork();
    if (code < 0) {
	dce_svc_printf(FXD_S_FORK_ZERODAEMON_FAILED_MSG, code, errno);
	exit(1);
    }
    if (code == 0)  { /* Child  */
       int Flags;
       
       errno = 0;     
       Flags = 1; /* Create a multi-threads pool for PX servers */
       error = afs_syscall(AFSCALL_PX, PXOP_RPCDAEMON, Flags, mainprocs, 
			   tokenprocs, 0);
       if (error<0) {
	   dce_svc_printf(FXD_S_NO_MULTI_THREAD_POOL_MSG);
	   exit(1);
       }
       exit(0);

    }  /* Child for PXOP_RPCDAEMON */

    if (fxd_verbose)
	dce_svc_printf(FXD_S_FORKING_HOST_GCDAEMON_MSG);
    code = fork();
    if (code < 0) {
	dce_svc_printf(FXD_S_FORK_CHECKHOST_FAILED_MSG, code, errno);
	exit(1);
    }
    if (code == 0) {
	/* Child */
	code = afs_syscall(AFSCALL_PX, PXOP_CHECKHOST, 0, 0, 0, 0);
	if (code < 0) {
	    dce_svc_printf(FXD_S_CHECKHOST_FAILED_MSG, code, errno);
	}
	exit(1);
    }

    if (fxd_verbose) {
	dce_svc_printf(FXD_S_CHILD_CREATED_MSG, code);
    }

    /*
     * Exit successfully.
     */
    exit(0);
}

main(argc, argv)
    int    argc;
    char **argv;
{
    register struct cmd_syndesc *ts;

    osi_setlocale(LC_ALL, "");
    initialize_svc();
    /* release any pre-existing context */
    dfsauth_server_RunAsSelf();

    ts = cmd_CreateSyntax((char *) 0, mainproc, (char *) 0, "start PXD");

    cmd_IndexedAddParm(ts, "-admingroup", CMD_SINGLE, CMD_REQUIRED, "group",
		       FXDCMD_ADMINGROUP);
    cmd_IndexedAddParm(ts, "-mainprocs", CMD_SINGLE, CMD_OPTIONAL,
		       "number_of_rpc_daemons", FXDCMD_MAINPROCS);
    cmd_IndexedAddParm(ts, "-tokenprocs", CMD_SINGLE, CMD_OPTIONAL,
		       "number_of_token_daemons", FXDCMD_TOKENPROCS);
    cmd_IndexedAddParm(ts, "-hostlife", CMD_SINGLE, CMD_OPTIONAL,
		       "client_timeout", FXDCMD_HOSTLIFE);
    cmd_IndexedAddParm(ts, "-hostrpc", CMD_SINGLE, CMD_OPTIONAL,
		       "client_rpc_timeout", FXDCMD_HOSTRPC);
    cmd_IndexedAddParm(ts, "-pollinterval", CMD_SINGLE, CMD_OPTIONAL,
		       "server_poll_period", FXDCMD_POLLINTERVAL);
    cmd_IndexedAddParm(ts, "-maxlife", CMD_SINGLE, CMD_OPTIONAL,
		       "max_hostlife", FXDCMD_MAXLIFE);
    cmd_IndexedAddParm(ts, "-maxrpc", CMD_SINGLE, CMD_OPTIONAL,
		       "max_hostrpc", FXDCMD_MAXRPC);
    cmd_IndexedAddParm(ts, "-notsr", CMD_FLAG, CMD_OPTIONAL,
		       "no_recovery_period", FXDCMD_NOTSR);
    cmd_IndexedAddParm(ts, "-minlocalprotectlevel", CMD_SINGLE, CMD_OPTIONAL,
		       "level", FXDCMD_MINLCLAUTHN);
    cmd_IndexedAddParm(ts, "-maxlocalprotectlevel", CMD_SINGLE, CMD_OPTIONAL,
		       "level", FXDCMD_MAXLCLAUTHN);
    cmd_IndexedAddParm(ts, "-minremoteprotectlevel", CMD_SINGLE, CMD_OPTIONAL,
		       "level", FXDCMD_MINRMTAUTHN);
    cmd_IndexedAddParm(ts, "-maxremoteprotectlevel", CMD_SINGLE, CMD_OPTIONAL,
		       "level", FXDCMD_MAXRMTAUTHN);
    cmd_IndexedAddParm(ts, "-verbose", CMD_FLAG, CMD_OPTIONAL, "",
		       FXDCMD_VERBOSE);

    return (cmd_Dispatch(argc, argv));
}
