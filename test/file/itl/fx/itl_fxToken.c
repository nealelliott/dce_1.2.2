/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1996, 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fxToken.c -- 
 *    Code to initialize a token revocation server (thread), and to handle
 *    RPC's related to token management that the test program will get
 *    from the file server.
 *
 */
/*
 * HISTORY
 * $Log: itl_fxToken.c,v $
 * Revision 1.1.323.1  1996/10/17  18:26:49  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:28  damon]
 *
 * $EndLog$
 */
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dcedfs/lock.h>
#include <dcedfs/common_data.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/tkn4int.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_fxDCEError.h>
#include <utils/itl/itl_fxCommon.h>
#include <utils/itl/itl_fxToken.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxToken.c,v 1.1.323.1 1996/10/17 18:26:49 damon Exp $")

#define ITL_FX_TOKEN_SERVER_NAME   "dfst"
#define ITL_FX_NUM_KERNEL_THREADS  4

struct afsNetData tokenServerAddress;  /* address of token server */
long epochStartTime;                   /* when this program started */

PRIVATE void InitServerAddress _TAKES((
				       void
				       ));

PRIVATE int TokenService _TAKES((
				 IN pthread_addr_t argAddress
				 ));

/*
 *  The following routines of the name, STKN_*, implement the RPC
 *  interfaces for the token manager.  They basically always return
 *  success.  The test program does not manage tokens.
 */

/*
 * Probe client Token interface call.
 */
PRIVATE error_status_t STKN_Probe(h)
  IN handle_t h;
{
    return(error_status_ok);
}


/*
 * Initialize all the token state associated with the server
 */
PRIVATE error_status_t STKN_InitTokenState(
  handle_t h,
  unsigned32 Flags,
  unsigned32 hostLifeGuarantee,
  unsigned32 hostRPCGuarantee,
  unsigned32 deadServerTimeout,
  unsigned32 serverRestartEpoch,
  unsigned32 serverSizesAttrs,
  unsigned32 spare2,
  unsigned32 spare3,
  unsigned32 *spare4,
  unsigned32 *spare5,
  unsigned32 *spare6)
{
    return(error_status_ok);
}


/*
 * Revoke token RPC call.
 */

PRIVATE error_status_t STKN_TokenRevoke(h, descp)
  IN handle_t h;
  INOUT afsRevokes *descp; 
{
    return(error_status_ok);
}


PRIVATE error_status_t STKN_GetCellName(h, cellp, cellNameP)
  IN handle_t h;
  IN afs_hyper_t *cellp;
  OUT unsigned char *cellNameP;
{
    *cellNameP = '\0';
    return(EINVAL);
}


/*
 * These are debugging only RPC calls and might be removed without any warning
 */
PRIVATE error_status_t STKN_GetLock (h, index, resultp)
  IN handle_t  h;
  IN unsigned32 index;
  OUT afsDBLock *resultp; 
{
    return(EINVAL);
}

PRIVATE error_status_t STKN_GetCE(h, index, resultp)
  IN handle_t h;
  IN unsigned32 index;
  OUT afsDBCacheEntry *resultp; 
{
    return(EINVAL);
}

PRIVATE error_status_t STKN_GetServerInterfaces(h, serverInterfacesP)
  IN handle_t  h;
  INOUT dfs_interfaceList *serverInterfacesP;
{
    return(EINVAL);
}

PRIVATE error_status_t STKN_SetParams(h, Flags, paramsP)
  IN handle_t h;
  IN unsigned32 Flags;
  INOUT afsConnParams *paramsP;
{
    return(EINVAL);
}

PRIVATE error_status_t STKN_AsyncGrant(h, grantedFileIdP, grantedTokenP,
				       grantedRequestID)
  IN handle_t h;
  IN afsFid *grantedFileIdP;
  IN afs_token_t *grantedTokenP;
  IN signed32 grantedRequestID;
{
    return(EINVAL);
}


PRIVATE TKN4Int_v4_0_epv_t TKN4Int_v4_0_manager_epv = {
	STKN_Probe,
	STKN_InitTokenState,
	STKN_TokenRevoke,
	STKN_GetCellName,
	STKN_GetLock,
	STKN_GetCE,
	STKN_GetServerInterfaces,
	STKN_SetParams,
	STKN_AsyncGrant
};


/*
 *  TokenService() --
 *    The function executed to respond to token revocation request from the
 *    file server.  The argAddress is ignored.
 */
PRIVATE int TokenService(argAddress)
  IN pthread_addr_t argAddress;
{
    unsigned32 statusCode;
    int retVal = 0;

    rpc_server_listen(ITL_FX_NUM_KERNEL_THREADS, &statusCode);
    if ((statusCode != error_status_ok) && 
	(statusCode != rpc_s_already_listening))
    {
	ITL_FX_DCE_ERROR(statusCode, "rpc_server_listen returned");
	retVal = 1;
    }
    pthread_exit((pthread_addr_t *) &retVal);
}

/*
 *  InitServerAddress() --
 *    This function setups the tokenServerAddress data, for later connections
 *    to a file server. 
 */
PRIVATE void InitServerAddress()
{
    char *stringBinding, *protseqString; 
    char *stringNetAddr, *stringEndpoint;
    rpc_binding_vector_p_t bindingVector;
    unsigned32 statusCode; 
    boolean_t setAddress = FALSE;

    /*
     * Verify whether the specified protocol sequence is supported by
     * both the RPC runtime and the underlying operating system.
     */
    if (rpc_network_is_protseq_valid((unsigned_char_p_t) 
				     ITL_FX_DFS_PROT_SEQ, &statusCode))
    {
	/*
	 * Tell the RPC runtime to listen for the remote procedure calls using 
	 * the specified protocol sequence.
	 */
	rpc_server_use_protseq((unsigned_char_p_t) ITL_FX_DFS_PROT_SEQ, 
			       ITL_FX_NUM_KERNEL_THREADS, &statusCode);
	if (statusCode == error_status_ok)
	{
	    /*
	     * Register the token server interface with the RPC runtime.
	     */
	    rpc_server_register_if(TKN4Int_v4_0_s_ifspec, (uuid_t *) 0,
				   (rpc_mgr_epv_t) &TKN4Int_v4_0_manager_epv,
				   &statusCode);

	    if (statusCode == error_status_ok)
	    {
		int i;

		/*
		 *  Inquire about all protocol sequences supported by both 
		 *  the RPC runtime and the underlying operating system. We
		 *  really want to return to the user the desired rpc 
		 *  binding that uses the "ncadg_ip_udp" protocol sequence.
		 */
		rpc_server_inq_bindings(&bindingVector, &statusCode);
		if (statusCode == error_status_ok)
		{
		    for (i = 0; 
			 (i < bindingVector->count && setAddress == FALSE);
			 i++)  {
			rpc_binding_to_string_binding(bindingVector->binding_h[i], 
						      (unsigned_char_p_t *)
						      &stringBinding, 
						      &statusCode);
			if (statusCode != error_status_ok)
			{
			    ITL_FX_DCE_ERROR(statusCode, 
					     "Could NOT get string binding");
			    break;
			}
			rpc_string_binding_parse((unsigned_char_p_t) 
						 stringBinding, 
						 (unsigned_char_p_t *) NULL, 
						 (unsigned_char_p_t *)
						 &protseqString,
						 (unsigned_char_p_t *)
						 &stringNetAddr, 
						 (unsigned_char_p_t *)
						 &stringEndpoint, 
						 (unsigned_char_p_t *) NULL, 
						 &statusCode);
			if (statusCode != error_status_ok) 
			{
			    ITL_FX_DCE_ERROR(statusCode, 
					     "Could NOT parse binding");
			    rpc_string_free((unsigned_char_p_t *)
					    &stringBinding, &statusCode);
			    break;
			}

			if (strcmp(protseqString, ITL_FX_DFS_PROT_SEQ) == 0 )
			{
			    struct sockaddr_in ipAddress;

			    /* 
			     *  Found first desired binding that matches 
			     *  "ncadg_ip_udp"  
			     */
			    setAddress = TRUE;

			    (void) memset((void *) &ipAddress, 
					  sizeof(struct sockaddr_in), 0); 
			    ipAddress.sin_family = AF_INET;
			    ipAddress.sin_addr.s_addr = 
				inet_addr(stringNetAddr);
			    ipAddress.sin_port = (unsigned short) htons((unsigned short) atoi(stringEndpoint));
			    (void) memcpy((void *) &tokenServerAddress.sockAddr,
					  (void *) &ipAddress,
					  sizeof(struct afsNetAddr));
			    tokenServerAddress.principalName[0] = '\0';
			}
			rpc_string_free((unsigned_char_p_t *) &protseqString,
					&statusCode);
			rpc_string_free((unsigned_char_p_t *) &stringBinding,
					&statusCode);
			rpc_string_free((unsigned_char_p_t *) &stringNetAddr,
					&statusCode);
			rpc_string_free((unsigned_char_p_t *) &stringEndpoint,
					&statusCode);
		    }
		    rpc_binding_vector_free(&bindingVector, &statusCode);

		    if (!setAddress)
			itl_Error("Unable to find protocol sequence match\n.");
		}
		else
		    ITL_FX_DCE_ERROR(statusCode, 
				     "Could NOT inquire about bindings");
	    }
	    else
		ITL_FX_DCE_ERROR(statusCode, 
				 "Could NOT register  token server");
	}
	else
	    ITL_FX_DCE_ERROR(statusCode, "Could NOT register protocol sequence");
    }
    else
	ITL_FX_DCE_ERROR(statusCode, "Invalid protocol sequence");

    if (!setAddress)
    {
	itl_Error("Could NOT start token revocation server, exiting.\n");
	exit(-1);
    }
}

EXPORT void itlFX_TokenInit()
{
    pthread_t listenThread;

    /*
     *  Set our start time.
     */
    epochStartTime = (long) time((time_t *) 0);

    InitServerAddress();

    /*
     *  Start the thread that will listen for RPC's.
     */
    if (pthread_create(&listenThread, pthread_attr_default,
		       (pthread_startroutine_t) TokenService,
		       (pthread_addr_t) 0))
    {
	itl_Error("Could NOT start token service listener thread, errno = %d\n",
		  errno);
	exit(-1);
    }
}
