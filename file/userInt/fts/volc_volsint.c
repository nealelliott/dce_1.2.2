/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $EndLog$
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <fcntl.h>
#include <dcedfs/sysincludes.h>
#include <dce/rpc.h>
#include <dcedfs/osi_dfserrors.h>
#include <dcedfs/ftserver_proc.h>
#include <dcedfs/queue.h> 
#include <dcedfs/volume.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/flserver.h>
#include <dcedfs/ftserver.h>
#include <dcedfs/ftserver_trans.h>
#include <dcedfs/cmd.h>
#include <dcedfs/aggr.h>
#include <dcedfs/compat.h>
#include <dcedfs/ftutil.h>
#include <dcedfs/vol_errs.h>
#include <dcedfs/sock_pipe.h>
#include <volc.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftimac.h>
#include <dfsftisvc.h>
#include <dfsftimsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/userInt/fts/RCS/volc_volsint.c,v 4.99 1996/10/31 20:16:42 gait Exp $")

static int doneDeCodeInit = 0;
static pthread_once_t decodeInitBlock = pthread_once_init;

static void doDeCodeInit() {
    osi_initDecodeTable();
    doneDeCodeInit = 1;
}

void fts_InitDecoder()
{
    if (doneDeCodeInit == 0) {
	/* Initialize the table used for decoding the error returned by DFS calls */
	if (pthread_once(&decodeInitBlock, &doDeCodeInit) != 0) {
	    VOLSERLOG(0, "pthread_once failed: %d\n", errno);
	}
    }
}

extern int verbose;
/*
 * This section includes all the RPC interface calls to the FILESET SERVER
 * (FTSERVER).  We have this additional layer mainly so that we can isolate
 * debugging/logging messages in a single place.
 */

static int ConditionBinding(connp, code)
    register rpc_binding_handle_t connp;
    int code;
{/* Recondition the RPC binding on appropriate errors. */
    error_status_t st;

    /* Reset the binding on any RPC error. */
    if ((code >= rpc_s_mod) && (code <= (rpc_s_mod+4096))) {
	VOLSERLOG(1, "[Reconditioning server binding due to RPC error]\n");
	st = 0;
	rpc_binding_reset(connp, &st);
	if (st != error_status_ok)
	    code = st;
    }
    return code;
}

static int canReTry(code)
long code;
{
    /* Determine whether it's worth retrying this condition. */
    /* Retry only if it's going to be busy for at most a short time. */
    /* Don't know about VOLERR_TRANS_REPFORWARD, since it applies to RWs as
     * well as ROs.
     */
    if (code == VOLERR_TRANS_LIST
	|| code == VOLERR_TRANS_GETSTATUS
	|| code == VOLERR_TRANS_SETQUOTA
	|| code == VOLERR_TRANS_FILESETEXISTS
	|| code == VOLERR_TRANS_SETFLAGS
	|| code == VOLERR_TRANS_SETSTATUS
	|| code == VOLERR_TRANS_REPGETSTATUS
	|| code == VOLERR_TRANS_REPSETSTATUS
	|| code == VOLERR_TRANS_REPSETFILESETVERSION
	|| code == VOLERR_TRANS_REPEDITSTATUS
	|| code == VOLERR_TRANS_REPCLEARSTATUS
	|| code == VOLERR_TRANS_REPGETFILESETCHANGES)
	return 1;
    return 0;
}
/* How many times we'll try an operation */
#define MAX_OPS_RETRIES	5

vols_CreateTrans(connp, volIdp, aggrId, attachFlag, transIdp, quiet)
    register rpc_binding_handle_t connp;
    afs_hyper_t *volIdp; 
    unsigned long aggrId;
    unsigned long attachFlag;
    long *transIdp;
    int quiet;
{
    register int code;
    int counter, retry;

    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+2);/* 2.5 minutes */
    fts_InitDecoder();
    for (counter = 0; counter < MAX_OPS_RETRIES; ++counter) {
	code = FTSERVER_CreateTrans(connp, volIdp, aggrId, attachFlag,
					(unsigned long *)transIdp);
	code = osi_errDecode(code);
	if (code == 0)
	    return 0;
	retry = canReTry(code);
	if ((quiet == 0) && (verbose || (retry && counter == 1))) {
	    /* Always print a ``retrying'' message after the second failure */
	    if (retry && counter < (MAX_OPS_RETRIES-1)) {
		VOLSERLOG(0, "FTSERVER_CreateTrans of vol=%lu,,%lu, aggrId=%d, flag=%#lx failed: Retrying\n",
			  AFS_HGETBOTH(*volIdp), aggrId, attachFlag);
	    } else {
		dce_svc_printf(FTS_S_VOLSINT_FTSERVER_CREATETRANS_MSG,
			       AFS_HGETBOTH(*volIdp), aggrId, attachFlag);
	    }
	    PrintError(0, code);
	}
	code = ConditionBinding(connp, code);
	if (!retry)
	    break;
	if (counter < (MAX_OPS_RETRIES-1))
	    sleep(15);
    }
    return code;
}


vols_AbortTrans(connp, transId)
     register rpc_binding_handle_t connp;
     long transId;
  {
    register int code;

    fts_InitDecoder();
    code = FTSERVER_AbortTrans(connp, transId);
    code = osi_errDecode(code);
    if (code)
	VOLSERLOG(1, "FTSERVER_AbortTrans failed to End trans %lu\n", transId); 
    code = ConditionBinding(connp, code);
    return code;
}


vols_DeleteTrans(connp, transId)
     register rpc_binding_handle_t connp;
     long transId;
  {
    register int code;

    fts_InitDecoder();
    code = FTSERVER_DeleteTrans(connp, transId);
    code = osi_errDecode(code);
    if (code)
	VOLSERLOG(1, "FTSERVER_DeleteTrans failed to End trans %lu\n", transId); 
    code = ConditionBinding(connp, code);
    return code;
}


vols_CreateVolume(connp, aggrId, volNamep, volType, flags, parIdp, volIdp, transIdp)
    register rpc_binding_handle_t connp;
    unsigned long aggrId;
    char *volNamep;
    long volType;
    unsigned long flags;
    afs_hyper_t *parIdp; 
    afs_hyper_t *volIdp; 
    long *transIdp;
{
    afs_hyper_t parVol;
    register int code;
    volNamep_string VN;

    if (!parIdp)
	AFS_hzero(parVol);
    else	
	parVol = *parIdp;
    strncpy((char *)VN, volNamep, sizeof(VN));
    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+1);/* 1.2 minutes */
    code = FTSERVER_CreateVolume(connp, aggrId, &VN, volType, flags, &parVol,
				     volIdp, (unsigned long *)transIdp);
    code = osi_errDecode(code);
    if (code) {
      if (verbose) {
	dce_svc_printf(FTS_S_VOLSINT_FTSERVER_CREATEVOLUME_MSG,
		       volNamep, AFS_HGETBOTH(*volIdp));
	PrintError(0, code);
      }
    }
    code = ConditionBinding(connp, code);
    return code;
}


vols_DeleteVolume(connp, transId)
    register rpc_binding_handle_t connp;
    long transId;
{
    register int code;
    int counter, retry;

    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+2);/* 2.5 minutes */
    for (counter = 0; counter < MAX_OPS_RETRIES; ++counter) {
	code = FTSERVER_DeleteVolume(connp, transId);
	code = osi_errDecode(code);
	if (code == 0)
	    return 0;
	retry = canReTry(code);
	if (verbose || (retry && counter == 1)) {
	    if (retry && counter < (MAX_OPS_RETRIES-1)) {
		VOLSERLOG(0, "FTSERVER_DeleteVolume failed for fileset associated with trans %d; retrying.\n",
			  transId);
	    } else {
		dce_svc_printf(FTS_S_VOLSINT_FTSERVER_DELETEVOLUME_MSG, transId); 
	    }
	    PrintError(0, code);
	}
	code = ConditionBinding(connp, code);
	if (!retry)
	    break;
	if (counter < (MAX_OPS_RETRIES-1))
	    sleep(15);
    }
    return code;
}

#if !defined(OSF_NO_SOCKET_DUMP)
/*
 * establish an out of band sock connection to the specified server
 * and return a handle for the connection
 */
static vols_ConnectSock(connp, transId, pipeP, clntFlag, sockFlagP, clntSockPP)
    rpc_binding_handle_t connp;
    unsigned32 transId;
    pipe_t *pipeP;
    unsigned clntFlag;
    unsigned *sockFlagP;
    clntSock_t **clntSockPP;
{
    long code, code2;
    ftserver_addr_t clntAddr;
    ftserver_addr_t srvrAddr;
    unsigned32 srvrInetAddr, clntInetAddr;
    struct sockaddr_in inAddr;

    if (!pipeP || !sockFlagP || !clntSockPP)
	return EINVAL;

    *clntSockPP = NULL;

    /*
     * get the server's address from the binding handle
     */
    code = rpcx_binding_to_sockaddr(connp, &inAddr);
    if (code)
	goto exit;

    /*
     * get a local address to use for connecting with the server.
     * We only support IPv4 for now.
     */
    if (inAddr.sin_family == AF_INET) {
        srvrInetAddr = ntohl(inAddr.sin_addr.s_addr);
        code = getLocalInetAddr(srvrInetAddr, &clntInetAddr);
        if (code || !clntInetAddr)
	    goto exit;
	if (clntInetAddr != srvrInetAddr)
	    *sockFlagP &= ~PIPE_UNIX;
    } else {
	*sockFlagP = PIPE_NONE;
	goto exit;
    }

    /*
     * if the PIPE_UNIX flag is still set, then the server is local.
     * Use a Unix Domain socket for the connection to the server.
     */
    if (*sockFlagP & PIPE_UNIX) {
	*sockFlagP = PIPE_UNIX;
	clntAddr.protocol = FTSERVER_UNIX;
	code = lstnUnixSock(clntFlag, pipeP,
			    (char *)clntAddr.address.unix_addr,
			    clntSockPP);
	if (code) {
	    code = osi_errEncode(code);
	    goto exit;
	}
	code = FTSERVER_ConnectSock(connp, transId, &clntAddr, &srvrAddr);
	if (code) {
	    termClntSock(*clntSockPP);
	    goto exit;
 	}
	code = acceptUnixSock(*clntSockPP,
			      (char *)srvrAddr.address.unix_addr);
	if (code) {
	    code = osi_errEncode(code);
	    code2 = FTSERVER_DisconnectSock(connp, transId);
	    if (code2) code = code2;
	    goto exit;
	}
    } else if (*sockFlagP & PIPE_TCP) {
	*sockFlagP = PIPE_TCP;
	clntAddr.protocol = FTSERVER_TCP;
	code = lstnTcpSock(clntFlag, pipeP,
			   &clntAddr.address.tcp_addr.tcpPort,
			   &clntAddr.address.tcp_addr.inetAddr,
			   clntSockPP);
	if (code) {
	    code = osi_errEncode(code);
	    goto exit;
	}
	code = FTSERVER_ConnectSock(connp, transId, &clntAddr, &srvrAddr);
	if (code) {
	    termClntSock(*clntSockPP);
	    goto exit;
 	}
	code = acceptTcpSock(*clntSockPP,
			     srvrAddr.address.tcp_addr.tcpPort,
			     srvrAddr.address.tcp_addr.inetAddr);
	if (code) {
	    code = osi_errEncode(code);
	    code2 = FTSERVER_DisconnectSock(connp, transId);
	    if (code2) code = code2;
	    goto exit;
	}
    } else {
	*sockFlagP = PIPE_NONE;
    }

exit:
    /* Use a RPC Pipe for the transfer if the server is down level */
    if (code == rpc_s_op_rng_error || code == FTSERVER_ERR_BAD_PROTOCOL) {
	*sockFlagP = PIPE_NONE;
	code = 0;
    }
    return code;
}

/* 
 * allocate a buffer for the null pipe passed for socket transfers.
 */

static void nullAlloc(state,bsize,buf,bcount)
    rpc_ss_pipe_state_t state;
    idl_ulong_int bsize;
    idl_char **buf;
    idl_ulong_int *bcount;
{
    static char nullBuf[8192];
    *buf = (idl_char *)(&nullBuf[0]);
    *bcount = 8192;
}

/*
 * push routine for the null pipe passed for socket transfers. Record
 * an error if any data is received on the pipe
 */

static void nullPush(state, buf, bsize)
    rpc_ss_pipe_state_t state;
    idl_char *buf;
    idl_ulong_int bsize;
{
    if (bsize != 0) {
	*((long *)state) = FTS_ERR_PIPE;
    }
}

/*
 * pull routine for the null pipe passed for socket transfers.
 */

static void nullPull(state, bsize, buf, nbytes)
    rpc_ss_pipe_state_t state;
    idl_ulong_int bsize;
    idl_byte **buf;
    idl_ulong_int *nbytes;
{
    *nbytes = 0;
}
#endif /* OSF_NO_SOCKET_DUMP */
vols_Dump(connp, transId, dumpDatep, pipeP, sockFlag)
    register rpc_binding_handle_t connp;
    long transId;
    ftserver_Date *dumpDatep;
    pipe_t *pipeP;
    unsigned sockFlag ;
{
    long code = 0;
    long code2 = 0;
#if !defined(OSF_NO_SOCKET_DUMP)
    clntSock_t *clntSockP;
#endif /* OSF_NO_SOCKET_DUMP */
    pipe_t nullPipe;
    int mySockFlag = sockFlag;

    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+2);/* 2.5 minutes */

#if !defined(OSF_NO_SOCKET_DUMP)

    if (mySockFlag != PIPE_NONE) {
	/* start the socket pipe thread */
	code = vols_ConnectSock(connp, transId, pipeP, SOCK_CLNT_RECV,
				&mySockFlag, &clntSockP);
    }
    if (!code) {
 	if (mySockFlag == PIPE_NONE) {
            code = FTSERVER_Dump(connp, transId, dumpDatep, pipeP);
	} else {
	    nullPipe.state = (rpc_ss_pipe_state_t)&code2;
	    nullPipe.push = (void (*)())nullPush;
	    nullPipe.pull = 0;
	    nullPipe.alloc = (void (*)())nullAlloc;
	    code = FTSERVER_Dump(connp, transId, dumpDatep, &nullPipe);
	    if (!code && code2)
	        code = code2;
	    code2 = termClntSock(clntSockP);
	    if (!code && code2)
	        code = osi_errEncode(code2);
	}
    }
#else /* OSF_NO_SOCKET_DUMP */
    code = FTSERVER_Dump(connp, transId, dumpDatep, pipeP);
#endif /* OSF_NO_SOCKET_DUMP */
    code = osi_errDecode(code);
    if (code && verbose) {
	dce_svc_printf(FTS_S_VOLSINT_FTSERVER_DUMP_MSG);
	PrintError(0, code);
    }
    code = ConditionBinding(connp, code);
    return code;
}


vols_Restore(connp, toTransId, flags, pipeP, sockFlag)
    register rpc_binding_handle_t connp;
    long toTransId;
    unsigned long flags;
    pipe_t *pipeP;
    unsigned sockFlag;
{
    long code = 0;
    long code2 = 0;
#if !defined(OSF_NO_SOCKET_DUMP)
    clntSock_t *clntSockP;
#endif /* OSF_NO_SOCKET_DUMP */
    pipe_t nullPipe;
    int mySockFlag = sockFlag;

    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+2);/* 2.5 minutes */
#if !defined(OSF_NO_SOCKET_DUMP)
    if (mySockFlag != PIPE_NONE) {
	/* start the socket pipe thread */
	code = vols_ConnectSock(connp, toTransId, pipeP, SOCK_CLNT_XMIT,
				&mySockFlag, &clntSockP);
    }
    if (!code) {
        if (mySockFlag == PIPE_NONE) {
            code = FTSERVER_Restore(connp, toTransId, flags, pipeP);
	} else {
	    nullPipe.state = (rpc_ss_pipe_state_t)&code2;
	    nullPipe.push = 0;
	    nullPipe.pull = (void (*)())nullPull;
	    nullPipe.alloc = (void (*)())nullAlloc;
	    code = FTSERVER_Restore(connp, toTransId, flags, &nullPipe);
	    if (!code && code2)
	        code = code2;
	    code2 = termClntSock(clntSockP);
	    if (!code && code2)
	        code = osi_errEncode(code2);
        }
    }
#else /* OSF_NO_SOCKET_DUMP */
    code = FTSERVER_Restore(connp, toTransId, flags, pipeP);
#endif /* OSF_NO_SOCKET_DUMP */
    code = osi_errDecode(code);
    if (code != 0 && code != FTU_E_RESTORED_INCONSISTENT_FSET && verbose) {
	dce_svc_printf(FTS_S_VOLSINT_FTSERVER_RESTORE_MSG);
	PrintError(0, code);
    }
    code = ConditionBinding(connp, code);
    return code;
}


vols_Forward(fconnp, tconnp, fromtransId, dumpDatep, destp, totransId, sockFlag)
    register rpc_binding_handle_t fconnp;
    register rpc_binding_handle_t tconnp;
    long fromtransId;
    struct ftserver_Date *dumpDatep;
    struct ftserver_dest *destp;
    long totransId;
    int sockFlag;
{
    long code = 0;
    long fcode = 0;
    long tcode = 0;
#if !defined(OSF_NO_SOCKET_DUMP)
    ftserver_addr_t fromAddr;
    ftserver_addr_t toAddr;
#endif /* OSF_NO_SOCKET_DUMP */

    fts_InitDecoder();
    fts_UseThisTimeout(fconnp, rpc_c_binding_default_timeout+2);/*2.5 minutes*/
#if !defined(OSF_NO_SOCKET_DUMP)
    /* establish a third party connection between the servers */
    if (sockFlag) {
	/* put the destination server in listen mode */
	tcode = FTSERVER_ListenSock(tconnp, totransId, &toAddr);
	if (!tcode) {
	    /* connect the source server to the destination server */
	    fcode = FTSERVER_ConnectSock(fconnp, fromtransId,
					 &toAddr, &fromAddr);
	    if (!fcode) {
		/* complete the connection between the servers */
		tcode = FTSERVER_AcceptSock(tconnp, totransId, &fromAddr);
		if (tcode) {
		    /* connection refused indicates interference */
		    if (tcode == ECONNREFUSED) {
			tcode = 0;
		    }
		    /* accept failed, use RPC pipes for data transfer */
		    fcode = FTSERVER_DisconnectSock(fconnp, fromtransId);
		}
	    } else {
		/* RPC range or protocol error indicates down level server */
		if (fcode == rpc_s_op_rng_error ||
		    fcode == FTSERVER_ERR_BAD_PROTOCOL) {
		    fcode = 0;
		}
		/* connect failed, use RPC pipes for data transfer */
		tcode = FTSERVER_DisconnectSock(tconnp, totransId);
	    }
	/* RPC range error indicates down level server */
	}  else if (tcode == rpc_s_op_rng_error) {
	    tcode = 0;
	}
    }
#endif /* OSF_NO_SOCKET_DUMP */
    if (!fcode && !tcode) {
        fcode = FTSERVER_Forward(fconnp, fromtransId, dumpDatep, destp,
				 0, totransId);
    }
    fcode = osi_errDecode(fcode);
    tcode = osi_errDecode(tcode);
    if (fcode) {
	code = fcode;
    } else {
	code = tcode;
    }
    if (code != 0 && code != FTU_E_RESTORED_INCONSISTENT_FSET && verbose) {
	dce_svc_printf(FTS_S_VOLSINT_FTSERVER_FORWARD_MSG, fromtransId,
		       totransId, MapSockAddr(&destp->address));
	PrintError(0, code);
    }
    fcode = ConditionBinding(fconnp, fcode);
    tcode = ConditionBinding(tconnp, tcode);
    if (fcode) {
	return fcode;
    } else {
	return tcode;
    }
}


vols_Clone(connp, transId, newType, newNamep, newVolIdp)
    register rpc_binding_handle_t connp;
    long transId;
    long newType;
    char *newNamep;
    afs_hyper_t *newVolIdp; 
{
    register int code;
    volNamep_string VN;
    int counter, retry;

    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+2);/* 2.5 minutes */
    for (counter = 0; counter < MAX_OPS_RETRIES; ++counter) {
	strncpy((char *)VN, newNamep, sizeof(VN));
	code = FTSERVER_Clone(connp, transId, newType, &VN, newVolIdp);
	code = osi_errDecode(code);
	if (code == 0)
	    return 0;
	retry = canReTry(code);
	if (verbose || (retry && counter == 1)) {
	    if (retry && counter < (MAX_OPS_RETRIES-1)) {
		VOLSERLOG(0, "FTSERVER_Clone failed to clone fileset (trans=%lu, name=%s); retrying.\n", 
			  transId, newNamep);
	    } else {
		dce_svc_printf(FTS_S_VOLSINT_FTSERVER_CLONE_MSG,
			       transId, newNamep); 
	    }
	    PrintError(0, code);
	}
	code = ConditionBinding(connp, code);
	if (!retry)
	    break;
	if (counter < (MAX_OPS_RETRIES-1))
	    sleep(15);
    }
    return code;
}


vols_ReClone(connp, transId, newVolIdp)
    register rpc_binding_handle_t connp;
    long transId;
    afs_hyper_t *newVolIdp; 
{
    register int code;
    int counter, retry;

    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+2);/* 2.5 minutes */
    for (counter = 0; counter < MAX_OPS_RETRIES; ++counter) {
	code = FTSERVER_ReClone(connp, transId, newVolIdp);
	code = osi_errDecode(code);
	if (code == 0)
	    return 0;
	retry = canReTry(code);
	if (verbose || (retry && counter == 1)) {
	    if (retry && counter < (MAX_OPS_RETRIES-1)) {
		VOLSERLOG(0, "FTSERVER_Reclone failed to Reclone vol (trans=%lu, newvol=%lu,,%lu); retrying.\n",
			  transId, AFS_HGETBOTH(*newVolIdp));
	    } else {
		dce_svc_printf(FTS_S_VOLSINT_FTSERVER_RECLONE_MSG,
			       transId, AFS_HGETBOTH(*newVolIdp)); 
	    }
	    PrintError(0, code);
	}
	code = ConditionBinding(connp, code);
	if (!retry)
	    break;
	if (counter < (MAX_OPS_RETRIES-1))
	    sleep(15);
    }
    return code;
}


vols_GetFlags(connp, transId, outFlagp)
    register rpc_binding_handle_t connp;
    long transId;
    unsigned long *outFlagp;
{
    register int code;

    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+1);/* 1.2 minutes */
    code = FTSERVER_GetFlags(connp, transId, outFlagp);
    code = osi_errDecode(code);
    if (code && verbose) {
	dce_svc_printf(FTS_S_VOLSINT_FTSERVER_GETFLAGS_MSG, transId);
	PrintError(0, code);
    }
    code = ConditionBinding(connp, code);
    return code;
}


vols_SetFlags(connp, transId, flags)
    register rpc_binding_handle_t connp;
    long transId;
    unsigned long flags;
{
    register int code;

    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+2);/* 2.5 minutes */
    code = FTSERVER_SetFlags(connp, transId, flags);
    code = osi_errDecode(code);
    if (code && verbose) {
	dce_svc_printf(FTS_S_VOLSINT_FTSERVER_SETFLAGS_MSG, flags, transId);
	PrintError(0, code);
    }
    code = ConditionBinding(connp, code);
    return code;
}


vols_GetStatus(connp, transId, statusp)
    register rpc_binding_handle_t connp;
    long transId;
    struct ftserver_status *statusp;
{
    register int code;

    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+1);/* 1.2 minutes */
    code = FTSERVER_GetStatus(connp, transId, statusp);
    code = osi_errDecode(code);
    if (code && verbose) {
	dce_svc_printf(FTS_S_VOLSINT_FTSERVER_GETSTATUS_MSG, transId);
	PrintError(0, code);
    }
    code = ConditionBinding(connp, code);
    return code;
}


vols_SetStatus(connp, transId, mask, statusp)
    register rpc_binding_handle_t connp;
    long transId;
    unsigned long mask;
    struct ftserver_status *statusp;
{
    register int code;

    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+2);/* 2.5 minutes */
    code = FTSERVER_SetStatus(connp, transId, mask, statusp, 0);
    code = osi_errDecode(code);
    if (code && verbose) {
	dce_svc_printf(FTS_S_VOLSINT_FTSERVER_SETSTATUS_MSG, transId, mask);
	PrintError(0, code);
    }
    code = ConditionBinding(connp, code);
    return code;
}


vols_ListVolumes(connp, aggrId, maxEntries, inCookie, outCookie, statEntriesp)
    register rpc_binding_handle_t connp;
    unsigned long aggrId;
    long maxEntries;
    long inCookie;
    long *outCookie;
    struct ftserver_statEntries *statEntriesp;
{
    register int code;
    ftserver_iterator inC, outC;

    inC.index = inCookie;

    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+1);/* 1.2 minutes */
    code = FTSERVER_ListVolumes(connp, aggrId, &inC, &outC, statEntriesp);
    code = osi_errDecode(code);
    if (code) {
	if (code != VOL_ERR_EOF) {
	    VOLSERLOG(1, "ListVolumes for aggrId %d, maxEnts %d, inCookie %d failed: %d\n",
		      aggrId, maxEntries, inCookie, code);
	}
    }
    *outCookie = outC.index;
    code = ConditionBinding(connp, code);
    return code;
}


vols_ListAggregates(connp, servAddrp, inCookieP, outCookieP, entriesp)
    register rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;			/* XXX Not USED XXX */
    ftserver_iterator *inCookieP, *outCookieP;
    struct ftserver_aggrEntries *entriesp;
{
    register int code;

    fts_InitDecoder();
    entriesp->ftserver_aggrList_len = 0;
    code = FTSERVER_ListAggregates(connp, inCookieP, outCookieP, entriesp);
    code = osi_errDecode(code);
    if (code) {
      if (verbose) {
	dce_svc_printf(FTS_S_VOLSINT_FTSERVER_LISTAGGREGATES_MSG);
	PrintError(0, code);
      }
    }
    code = ConditionBinding(connp, code);
    return code;
}


vols_AggregateInfo(connp, servAddrp, aggrId, raggrInfop, quietp)
    register rpc_binding_handle_t connp;
    struct sockaddr *servAddrp;			/* XXX Not USED XXX */
    unsigned long aggrId;
    struct ftserver_aggrInfo *raggrInfop;
    int quietp;
{
    register int code;

    fts_InitDecoder();
    code = FTSERVER_AggregateInfo(connp, aggrId, raggrInfop);
    code = osi_errDecode(code);
    if (code) {
	if ((quietp == 0) && verbose) {
	    dce_svc_printf(FTS_S_VOLSINT_FTSERVER_AGGREGATEINFO_MSG, aggrId);
	    PrintError(0, code);
	}
    }
    code = ConditionBinding(connp, code);
    return code;
}


vols_Monitor(connp, transEntriesp)
    register rpc_binding_handle_t connp;
    struct ftserver_transEntries *transEntriesp;
{
    register int code;

    fts_InitDecoder();
    code = FTSERVER_Monitor(connp, transEntriesp);
    code = osi_errDecode(code);
    if (code && verbose) {
	dce_svc_printf(FTS_S_VOLSINT_FTSERVER_MONITOR_MSG);
	PrintError(0, code);
    }
    code = ConditionBinding(connp, code);
    return code;
}

vols_GetOneVolStatus(connp, volIdp, aggrId, statusp, quietp)
    register rpc_binding_handle_t connp;
    afs_hyper_t *volIdp; 
    unsigned long aggrId;
    struct ftserver_status *statusp;
    int quietp;
{
    register long code;
    int counter, retry;

    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+1);/* 1.2 minutes */
    for (counter = 0; counter < MAX_OPS_RETRIES; ++counter) {
	code = FTSERVER_GetOneVolStatus(connp, volIdp, aggrId, 0, statusp);
	code = osi_errDecode(code);
	if (code == 0)
	    return 0;
	retry = canReTry(code);
	if ((quietp == 0) && (verbose || (retry && counter == 1))) {
	    if (retry && counter < (MAX_OPS_RETRIES-1)) {
		VOLSERLOG(0, "FTSERVER_GetOneVolStatus of ft=%lu,,%lu, aggrId=%d failed; retrying.\n",
			  AFS_HGETBOTH(*volIdp), aggrId);
	    } else {
		dce_svc_printf(FTS_S_VOLSINT_FTSERVER_GETONEVOLSTATUS_MSG,
			       AFS_HGETBOTH(*volIdp), aggrId);
	    }
	    PrintError(0, code);
	}
	code = ConditionBinding(connp, code);
	if (!retry)
	    break;
	if (counter < (MAX_OPS_RETRIES-1))
	    sleep(15);
    }
    return code;
}


vols_SwapIDs(connp, transIdA, transIdB)
    register rpc_binding_handle_t connp;
    long transIdA, transIdB;
{
    register int code;
    unsigned long spare2;
    int counter, retry;

    fts_InitDecoder();
    fts_UseThisTimeout(connp, rpc_c_binding_default_timeout+2);/* 2.5 minutes */
    for (counter = 0; counter < MAX_OPS_RETRIES; ++counter) {
	code = FTSERVER_SwapIDs(connp, transIdA, transIdB, 0, &spare2);
	code = osi_errDecode(code);
	if (code == 0)
	    return 0;
	retry = canReTry(code);
	if (retry && counter < (MAX_OPS_RETRIES-1)) {
	    VOLSERLOG(0, "FTSERVER_SwapIDs failed to swap IDs of filesets (transA=%lu, transB=%lu); retrying.\n", 
		      transIdA, transIdB);
	} else {
	    dce_svc_printf(FTS_S_VOLSINT_FTSERVER_SWAPIDS_MSG,
			   transIdA, transIdB); 
	}
	PrintError(0, code);
	code = ConditionBinding(connp, code);
	if (!retry)
	    break;
	if (counter < (MAX_OPS_RETRIES-1))
	    sleep(15);
    }
    return code;
}

static int CompareAggID(p1,p2)
    const void *p1, *p2;
{
    ftserver_aggrList *arg1, *arg2;

    arg1 = (ftserver_aggrList *)p1;
    arg2 = (ftserver_aggrList *)p2;
    if (arg1->Id > arg2->Id) return 1;
    else if (arg1->Id < arg2->Id) return -1;
    else return 0;
}

long fts_GetAllAggrs(connp, servAddrp, numAggrsP, aggrDescsP)
register rpc_binding_handle_t connp;
struct sockaddr *servAddrp;			/* XXX Not USED XXX */
long *numAggrsP;
ftserver_aggrList **aggrDescsP;
{
    register ftserver_aggrList *aggsp;
    register long numAggrs, maxAggrs;
    ftserver_iterator inCook, outCook;
    ftserver_aggrEntries aggEnts;
    long code;

    *numAggrsP = 0;
    *aggrDescsP = NULL;
    bzero((char *)&inCook, sizeof(inCook));
    aggsp = NULL;
    maxAggrs = 0;	/* amount allocated */
    numAggrs = 0;	/* amount in use */
    fts_InitDecoder();
    for (;;) {
	code = vols_ListAggregates(connp, servAddrp, &inCook, &outCook, &aggEnts);
	if (code) {
	    if (aggsp)
		free((opaque) aggsp);
	    return code;
	}
	if ((numAggrs + aggEnts.ftserver_aggrList_len) > maxAggrs) {
	    /* make it 5 larger than 3/2 of the needed space */
	    maxAggrs = 5 + (((numAggrs + aggEnts.ftserver_aggrList_len)*3)>>1);
	    if (aggsp == NULL) {
		aggsp = (ftserver_aggrList *)
		  malloc(maxAggrs * sizeof(ftserver_aggrList));
	    } else {
		aggsp = (ftserver_aggrList *)
		  realloc(aggsp, maxAggrs * sizeof(ftserver_aggrList));
	    }
	    if (aggsp == NULL)
		return ENOMEM;
	}
	if (aggEnts.ftserver_aggrList_len != 0) {
	    /* there are some new descriptors to copy in */
	    bcopy((char *)&aggEnts.ftserver_aggrEntries_val[0], (char *)&aggsp[numAggrs],
		  aggEnts.ftserver_aggrList_len * sizeof(ftserver_aggrList));
	    numAggrs += aggEnts.ftserver_aggrList_len;
	}
	/* Check for loop termination */
	if (aggEnts.ftserver_aggrList_len != FTSERVER_MAXAGGR
	    || inCook.index == outCook.index)
	    break;
	inCook = outCook;	/* prepare for next round */
    }
    if (aggsp != NULL && numAggrs != maxAggrs) {
	/* Cut back on required space */
	aggsp = (ftserver_aggrList *) realloc(aggsp,
					      numAggrs * sizeof(ftserver_aggrList));
	if (aggsp == NULL) /* should NEVER happen */
	    return ENOMEM;
    }
    if (numAggrs > 1) {
	qsort((void *)aggsp, (size_t)numAggrs, sizeof(ftserver_aggrList), CompareAggID);
    }
    *numAggrsP = numAggrs;
    *aggrDescsP = aggsp;
    return 0;
}

/*
 * This section includes routines that display VOLSER interface related data
 * structures
 */
static void DisplayFtType(stsp)
ftserver_status *stsp;
{/* Print (on stdout) some representation of the fileset type. */
    register unsigned long Tp;
    register unsigned long Sts;
    register char *cp;

    Tp = stsp->vss.type;
    Sts = stsp->vss.states;
    cp = NULL;
    switch (Sts & VOL_TYPEFIELD) {
	case VOL_TYPE_RW:
	    cp = "RW"; break;
	case VOL_TYPE_RO:
	    cp = "RO"; break;
	case VOL_TYPE_BK:
	    cp = "BK"; break;
	case VOL_TYPE_TEMP:
	    cp = "TEMP"; break;
	case 0:
	    break;	/* fall through */
	default:
	    /* Oddball: note it and leave. */
	    dce_printf(fts_s_volsint_tf_default, Sts & VOL_TYPEFIELD);
	    return;
    }
    if (cp == NULL) {
	switch (Tp) {
	    case VOL_RW:
		cp = "RW"; break;
	    case VOL_READONLY:
		cp = "RO/BK"; break;
	}
    }
    if (cp != NULL) {
	fputs(cp, stdout);
    } else {
	dce_printf(fts_s_volsint_type, Tp);
    }
}

/* Helper routine for DisplayFormat */
static void DisplayOnLine(stsp, totalOKp, totalNotOKp, totalBusyP)
register ftserver_status *stsp;
long *totalOKp, *totalNotOKp, *totalBusyP;
{
    register unsigned long states = stsp->vss.states;
    char *msg;
    long localerror;

    if (states & (VOL_ZAPME | VOL_CLONEINPROG | VOL_OFFLINE | VOL_DELONSALVAGE
		   | VOL_DEADMEAT | VOL_OUTOFSERVICE
		   | VOL_MOVE_TARGET | VOL_MOVE_SOURCE)) {
	*totalNotOKp += 1;
	if (states & VOL_DEADMEAT) {
	    msg = " (deleted)";
	} else if (states & VOL_ZAPME) {
	    msg = " (delete now)";
	} else if (states & VOL_CLONEINPROG) {
	    msg = " (unfinished clone exists)";
	} else if (states & VOL_DELONSALVAGE) {
	    msg = " (inconsistent--being updated)";
	} else if (states & VOL_MOVE_SOURCE) {
	    msg = " (being moved from here)";
	} else if (states & VOL_MOVE_TARGET) {
	    msg = " (being moved to here)";
	} else if (states & VOL_OFFLINE) {
	    msg = "";
	} else {
	    msg = " (out of service)";
	}
	dce_printf(fts_s_volsint_off_line, msg);
	if (stsp->vss.accError) {
	    localerror = osi_errDecode(stsp->vss.accError);
	    dce_printf(fts_s_volsint_vss_accerror, 
		       dfs_dceErrTxt(localerror));
	}
    } else {
	*totalOKp += 1;
	dce_printf(fts_s_volsint_on_line);
    }
}


static void DisplayFormat(statusp, servAddrp, aggrId, totalOKp, totalNotOKp, totalBusyP, 
			   fast, longlist)
    ftserver_status *statusp;
    struct sockaddr *servAddrp;
    long aggrId;
    long *totalOKp, *totalNotOKp, *totalBusyP;
    long fast, longlist;
{
    unsigned long aggrType;
    long code, localerror;
    long bogusSignedTime;
    afs_hyper_t hDum;

    if (fast) {
	dce_printf(fts_s_volsint_format, AFS_HGETBOTH(statusp->vss.volId));
    } else if (longlist) {
	if (statusp->vss.accError == 0){
	    dce_printf(fts_s_volsint_another_format, statusp->vss.volName,
			AFS_HGETBOTH(statusp->vss.volId));
	    DisplayFtType(statusp);
	    code = fts_GetAggrType(servAddrp, aggrId, &aggrType, 0);
	    if (code == 0) {
		dce_printf(fts_s_volsint_aggrtye, AG_TYPE_TO_STR(aggrType));
	    }
	    switch (statusp->vss.states & VOL_REPFIELD) {
		case VOL_REP_RELEASE:
		    dce_printf(fts_s_volsint_release); break;
		case VOL_REP_LAZY:
		    dce_printf(fts_s_volsint_scheduled); break;
		case VOL_REP_NONE:
		    dce_printf(fts_s_volsint_blanks);
		    break;
		default:	/* oddball */ /* Jeff K. likes this */
		    dce_printf(fts_s_volsint_repl_vss_states, statusp->vss.states & VOL_REPFIELD);
		    break;
	    }
	    dce_printf(fts_s_volsint_states_vss_states, statusp->vss.states);
	    /* printf(" accStatus %#lx  ", statusp->vss.accStatus); */
	    DisplayOnLine(statusp, totalOKp, totalNotOKp, totalBusyP);
	    dce_printf(fts_s_volsint_aggregate_servaddrp, MapSockAddr(servAddrp),
		   getAggrName(servAddrp, aggrId, 0), aggrId);
	    dce_printf(fts_s_volsint_parent_clone,
		       AFS_HGETBOTH(statusp->vss.parentId),
		       AFS_HGETBOTH(statusp->vsd.cloneId),
		       AFS_HGETBOTH(statusp->vsd.backupId));
	    dce_printf(fts_s_volsint_back_version,
		       AFS_HGETBOTH(statusp->vsd.llBackId),
		       AFS_HGETBOTH(statusp->vsd.llFwdId),
		       AFS_HGETBOTH(statusp->vsd.volversion));
	    if (AFS_hsame(ftu_unlimitedQuota, statusp->vsd.allocLimit)) {
		printf("        Infinite alloc limit");
	    } else {
		hDum = statusp->vsd.allocLimit;
		AFS_hrightshift(hDum, 10);	/* cvt bytes to K */
		dce_printf(fts_s_volsint_alloc_limit, AFS_hgetlo(hDum));
	    }
	    hDum = statusp->vsd.allocUsage;
	    AFS_hrightshift(hDum, 10);	/* cvt bytes to K */
	    dce_printf(fts_s_volsint_alloc_usage, AFS_hgetlo(hDum));
	    hDum = statusp->vsd.visQuotaLimit;
	    AFS_hrightshift(hDum, 10);	/* cvt bytes to K */
	    dce_printf(fts_s_volsint_quota_limit, AFS_hgetlo(hDum));
	    hDum = statusp->vsd.visQuotaUsage;
	    AFS_hrightshift(hDum, 10);	/* cvt bytes to K */
	    dce_printf(fts_s_volsint_quota_usage, AFS_hgetlo(hDum));
	    if (statusp->vsd.minQuota != 0) {
		dce_printf(fts_s_volsint_min_quota, statusp->vsd.minQuota);
	    }
	    printf("\n");
	    if (verbose) {
		if (statusp->vss.states & VOL_HAS_TOKEN) {
		    bogusSignedTime = (long) statusp->vss.tokenTimeout;
		    dce_printf(fts_s_volsint_token_expires, ctime(&bogusSignedTime));
		} else if (statusp->vss.vvCurrentTime.sec != 0) {
		    bogusSignedTime = (long) statusp->vss.vvCurrentTime.sec;
		    dce_printf(fts_s_volsint_vv_was_current, ctime(&bogusSignedTime));
		}
#ifdef notdef
		/* got rid of the VOL_KNOWDALLY flag */
		if (statusp->vss.states & VOL_KNOWDALLY) {
		    dce_printf(fts_s_volsint_reclaim_wait, fts_ivToString(statusp->vss.reclaimDally));
		}
#endif /* notdef */
		if (statusp->vsd.nodeMax != 0) {
		    dce_printf(fts_s_volsint_max_node_index, statusp->vsd.nodeMax);
		}
	    }
	    if (statusp->vsd.creationDate.sec != 0) {
		bogusSignedTime = (long) statusp->vsd.creationDate.sec;
		dce_printf(fts_s_volsint_creation_time, ctime(&bogusSignedTime));
	    }
	    if (statusp->vsd.updateDate.sec != 0) {
		bogusSignedTime = (long) statusp->vsd.updateDate.sec;
		if (statusp->vsd.creationDate.sec != 0
		    && statusp->vsd.creationDate.sec > statusp->vsd.updateDate.sec)
		    bogusSignedTime = (long) statusp->vsd.creationDate.sec;
		dce_printf(fts_s_volsint_last_update, ctime(&bogusSignedTime));
	    }
	    if (verbose) {
		if (statusp->vsd.accessDate.sec != 0) {
		    bogusSignedTime = (long) statusp->vsd.accessDate.sec;
		    dce_printf(fts_s_volsint_last_access, ctime(&bogusSignedTime));
		}
		if (statusp->vss.cloneTime.sec != 0) {
		    bogusSignedTime = (long) statusp->vss.cloneTime.sec;
		    dce_printf(fts_s_volsint_clone_made, ctime(&bogusSignedTime));
		}
		if (statusp->vsd.backupDate.sec != 0) {
		    bogusSignedTime = (long) statusp->vsd.backupDate.sec;
		    dce_printf(fts_s_volsint_backup_made, ctime(&bogusSignedTime));
		}
		if (statusp->vsd.copyDate.sec != 0) {
		    bogusSignedTime = (long) statusp->vsd.copyDate.sec;
		    dce_printf(fts_s_volsint_copy_made, ctime(&bogusSignedTime));
		}
	    }
	} else {
	    *totalBusyP += 1;
	    localerror = osi_errDecode(statusp->vss.accError);
	    dce_printf(fts_s_volsint_fileset_busy,
		       AFS_HGETBOTH(statusp->vss.volId),
		       dfs_dceErrTxt(localerror));
	}
	printf("\n");
    } else {
	if (statusp->vss.accError == 0) {
	    dce_printf(fts_s_volsint_format_3, statusp->vss.volName,
		       AFS_HGETBOTH(statusp->vss.volId));
	    DisplayFtType(statusp);
	    hDum = statusp->vsd.allocUsage;
	    AFS_hrightshift(hDum, 10);	/* cvt bytes to K */
	    dce_printf(fts_s_volsint_k_alloc, AFS_hgetlo(hDum));
	    hDum = statusp->vsd.visQuotaUsage;
	    AFS_hrightshift(hDum, 10);	/* cvt bytes to K */
	    dce_printf(fts_s_volsint_k_quota, AFS_hgetlo(hDum));
	    DisplayOnLine(statusp, totalOKp, totalNotOKp, totalBusyP);
	    printf("\n");
	} else {
	    *totalBusyP += 1;
	    localerror = osi_errDecode(statusp->vss.accError);
	    dce_printf(fts_s_volsint_fileset_busy,
		       AFS_HGETBOTH(statusp->vss.volId),
		       dfs_dceErrTxt(localerror));
	}
    }
}


void DisplayVolumes(servAddrp, aggrId, statusp, count, longlist, fast)
    struct sockaddr *servAddrp;
    unsigned long aggrId;
    ftserver_status *statusp;
    long count, longlist, fast;
{
    long totalOK = 0, totalNotOK = 0, totalBusy = 0, i;
    register ftserver_status *statp = statusp;

    for (i = 0; i < count; ++i, ++statp)
	DisplayFormat(statp, servAddrp, aggrId, &totalOK, &totalNotOK,
		      &totalBusy, fast, longlist);
    if (!fast) {
	dce_svc_printf(FTS_S_VOLSINT_TOTAL_FILESETS_ON_LINE_MSG, totalOK, totalNotOK, totalBusy);
    }
}


void VolumeStats(statusp, entryp, servAddrp, aggrId, voltype)
    ftserver_status *statusp;
    struct vldbentry *entryp;
    struct sockaddr *servAddrp;
    unsigned long aggrId;
    long voltype;
{
    long totalOK, totalNotOK, totalBusy;
    long code;
    int RWix, anyIx;
    register long k;
    char dateBuf[30];
    long bogusSignedTime;

    dce_svc_printf(FTS_S_VOLSINT_DASHES_MSG);
    DisplayFormat(statusp, servAddrp, aggrId, &totalOK, &totalNotOK,
		  &totalBusy, 0, 1);
    RWix = anyIx = -1;
    for (k = 0; k < MAXVOLTYPES && !AFS_hiszero(entryp->VolIDs[k]); ++k) {
	anyIx = k;
	if (entryp->VolTypes[k] == VOLTIX_TO_VOLTYPE(RWVOL)) {
	    RWix = k;
	    break;
	}
    }
    k = -1;
    if (RWix >= 0) {anyIx = RWix; k = RWVOL;}
    if (anyIx < 0) {
	dce_svc_printf(FTS_S_VOLSINT_NO_VOLID_MSG, entryp->VolTypes[0], AFS_HGETBOTH(entryp->VolIDs[0]));
    } else {
	code = vldb_SetLock(&entryp->VolIDs[anyIx], k, 0 /* special no-complaint code */);
	if (!code) {
	    (void) vldb_ReleaseLock(&entryp->VolIDs[anyIx], k, LOCKREL_ALL);
	} else if (code == VL_ENTRYLOCKED) {
	    vldb_PrintLocker(entryp, -1, "LOCKED");
	}
    }
    vldb_EnumerateEntry(entryp, " ", 0);
    dce_svc_printf(FTS_S_VOLSINT_MORE_DASHES_MSG);
}


static void PrintVolserOps(ops, sep)
unsigned32 ops;
char *sep;
{/* Print a description of the operations being tried */
    static struct opDesc {unsigned32 bitval; char *bitname;} bitNames[] = {
	{FTSERVER_OP_DELETE, "Delete"},
	{FTSERVER_OP_DUMP, "Dump"},
	{FTSERVER_OP_RESTORE, "Restore"},
	{FTSERVER_OP_CLONE, "Clone"},
	{FTSERVER_OP_RECLONE, "ReClone"},
	{FTSERVER_OP_GETFLAGS, "GetFlags"},
	{FTSERVER_OP_SETFLAGS, "SetFlags"},
	{FTSERVER_OP_GETSTATUS, "GetStatus"},
	{FTSERVER_OP_SETSTATUS, "SetStatus"},
	{FTSERVER_OP_SETSTATUS_ID, "SetStatus-ID"},
	{FTSERVER_OP_SWAPIDS, "SwapIDs"}
    };
    register int first, ctr;

    for (first = 0, ctr = 0; ctr < (sizeof(bitNames)/sizeof(bitNames[0])); ++ctr) {
	if (ops & bitNames[ctr].bitval) {
	    if (first == 0) {
		first = 1;
	    } else {
		fputs(sep, stdout);
	    }
	    fputs(bitNames[ctr].bitname, stdout);
	}
    }
}

void PrintVolserStatus(statusp)
    ftserver_transStatus *statusp;
{
    long bogusSignedTime;
    register unsigned long sts;
    register int printed;
    long localerror;

    dce_printf(fts_s_volsint_again_dashes);
    bogusSignedTime = statusp->creationTime.sec;
    dce_printf(fts_s_volsint_trans_created, statusp->transId, ctime(&bogusSignedTime));
    if (statusp->callerAddr.type != 0) {
	dce_printf(fts_s_volsint_from_addr, MapSockAddr(&statusp->callerAddr));
    }
    bogusSignedTime = statusp->lastTime;
    dce_printf(fts_s_volsint_ref_count, statusp->vDesc, statusp->refCount, ctime(&bogusSignedTime));
    if (statusp->returnCode)
	dce_printf(fts_s_volsint_returncode, statusp->returnCode,
	       dfs_dceErrTxt(statusp->returnCode));
    dce_printf(fts_s_volsint_fileset_aggrtype,
	       AFS_HGETBOTH(statusp->volId), statusp->aggrId,
	       AFS_HGETBOTH(statusp->baseType));
    localerror = osi_errDecode(statusp->accError);
    dce_printf(fts_s_volsint_action_ops, dfs_dceErrTxt(localerror));
    PrintVolserOps(statusp->accStatus, ", ");
    printf("\n");
    sts = statusp->states;
    if (sts) {
	dce_printf(fts_s_volsint_fileset_status, sts);
	switch (sts & (VOL_RW | VOL_READONLY)) {
	    case VOL_RW:
		dce_printf(fts_s_volsint_r_w); break;
	    case VOL_READONLY:
		dce_printf(fts_s_volsint_r_o); break;
	    case 0:
		dce_printf(fts_s_volsint_no_rw_ro); break;
	    case (VOL_RW | VOL_READONLY):
		dce_printf(fts_s_volsint_both_rw_ro); break;
	}
	if (sts & VOL_BUSY) dce_printf(fts_s_volsint_busy);
	if (sts & VOL_OFFLINE) dce_printf(fts_s_volsint_offline);
	if (sts & VOL_DELONSALVAGE) dce_printf(fts_s_volsint_delonsalvage);
	if (sts & VOL_OUTOFSERVICE) dce_printf(fts_s_volsint_outofservice);
	if (sts & VOL_DEADMEAT) dce_printf(fts_s_volsint_deadmeat);
	if (sts & VOL_LCLMOUNT) dce_printf(fts_s_volsint_lclmount);
	if (sts & VOL_IS_COMPLETE) dce_printf(fts_s_volsint_complete);
	if (sts & VOL_HAS_TOKEN) dce_printf(fts_s_volsint_has_token);
#ifdef notdef
	/* no more KNOWDALLY flag */
	if (sts & VOL_KNOWDALLY) dce_printf(fts_s_volsint_know_dally);
#endif /* notdef */
	if (sts & VOL_NOEXPORT) dce_printf(fts_s_volsint_no_export);
	if (sts & VOL_GRABWAITING) dce_printf(fts_s_volsint_grab_waiting);
	if (sts & VOL_LOOKUPWAITING) dce_printf(fts_s_volsint_lookupb_waiting);
	if (sts & VOL_REPSERVER_MGD) dce_printf(fts_s_volsint_repserver_mgd);
	if (sts & VOL_MOVE_TARGET) dce_printf(fts_s_volsint_move_target);
	if (sts & VOL_MOVE_SOURCE) dce_printf(fts_s_volsint_move_source);
	if (sts & VOL_ZAPME) dce_printf(fts_s_volsint_zap_me);
	if (sts & VOL_CLONEINPROG) dce_printf(fts_s_volsint_clone_in_progress);
	switch (sts & VOL_REPFIELD) {
	    case VOL_REP_NONE:
		break;
	    case VOL_REP_RELEASE:
		dce_printf(fts_s_volsint_release_replicated); break;
	    case VOL_REP_LAZY:
		dce_printf(fts_s_volsint_sched_replicated); break;
	    default:
		dce_printf(fts_s_volsint_x_replicated, sts&VOL_REPFIELD); break;
	}
	switch (sts & VOL_TYPEFIELD) {
	    case 0:
		break;
	    case VOL_TYPE_RW:
		dce_printf(fts_s_volsint_type_r_w); break;
	    case VOL_TYPE_RO:
		dce_printf(fts_s_volsint_type_rd); break;
	    case VOL_TYPE_BK:
		dce_printf(fts_s_volsint_type_bk); break;
	    case VOL_TYPE_TEMP:
		dce_printf(fts_s_volsint_type_temp); break;
	    default:
		dce_printf(fts_s_volsint_type_number, sts&VOL_TYPEFIELD); break;
	}
	if (statusp->flags & FTSERVER_FLAGS_DELETED) dce_printf(fts_s_volsint_transdeleted);
	printf("\n");
    }
    dce_printf(fts_s_volsint_even_more);
}
