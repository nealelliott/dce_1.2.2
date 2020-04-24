/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_service_global_set.c,v $
 * Revision 1.1.6.2  1996/02/17  23:35:33  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:46  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:13:47  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/11  12:33 UTC  pare
 * 	Merge changes
 * 
 * 	HP revision /main/HPDCE02/pare_MOTHRA/1  1995/09/08  12:59 UTC  pare
 * 	Free profileName string -- CHFts16224
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/27  13:46 UTC  pare
 * 	Create initial branch
 * 
 * 	HP revision /main/pare_mothra/1  1995/06/26  14:00 UTC  pare
 * 	Fix for CHFts15592
 * 	[1995/12/08  17:25:50  root]
 * 
 * Revision 1.1.4.7  1994/08/23  20:32:26  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:57:02  cbrooks]
 * 
 * Revision 1.1.4.6  1994/08/05  20:47:40  cbrooks
 * 	CR11240 - add dced config functionality
 * 	[1994/08/05  20:23:39  cbrooks]
 * 
 * Revision 1.1.4.5  1994/06/30  19:45:23  cbrooks
 * 	CR11112 - new SAMS messages
 * 	[1994/06/28  20:13:27  cbrooks]
 * 
 * 	CR11112 - new serviceability messages required
 * 	[1994/06/28  17:48:35  cbrooks]
 * 
 * Revision 1.1.4.4  1994/05/16  18:51:48  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:20  rhw]
 * 
 * Revision 1.1.4.3  1994/04/28  21:08:47  cbrooks
 * 	DTS Code Cleanup
 * 	[1994/04/28  17:35:12  cbrooks]
 * 
 * Revision 1.1.4.2  1994/04/21  22:06:03  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:59:47  cbrooks]
 * 
 * Revision 1.1.4.1  1994/01/25  17:16:37  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/25  17:15:38  rsalz]
 * 
 * Revision 1.1.2.2  1992/12/30  16:38:39  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:20:58  htf]
 * 
 * Revision 1.1  1992/01/19  15:34:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module DTSS_SERVICE_GLOBAL_SET.C
 *
 * Copyright (c) 1989 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:    Interface to the DNS clerk for reading and writing
 *              the GLobal set of time servers.
 *
 * ENVIRONMENT:	portable
 *
 * AUTHORS:
 *		Wick Nichols
 *		Distributed Systems Services (DSS)
 *
 * DATE:	November 1989
 *
 */

#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>
#include <dtss_attr_ranges.h>
#include <dtsd_macros.h>
#include <dce/dce_cf.h>
#include <dce/dce.h>
#include <dce/secidmap.h>
#include <dcedtssvc.h>
#include <dcedtsmsg.h>
#include <dcedtsmac.h>

/*
 * external variables
 */
extern Que portQhd;		/* Port Queue header */

extern ShmBuf *shmPtr;		/* Ptr to shared memory */
                                /* All modules have shmPtr defined */

extern Sdb sdb;			/* Synchronization database. */

extern PortDsc *rpcPortPtr;
extern RpcBuf  *rpcBufPtr;

/*
 * Global Variables
 */

globalref gbl_time_service_v1_0_epv_t gbl_time_service_epv;

/*
 * Local typedefs
 */

typedef enum
{
    CRlocal = 1,
    CRglobal
    } ConfigSet;

typedef enum
{
    CTnone = 0,
    CT802,
    CTdecnet,
    CTrpc
    } ConfigTransport;

typedef struct ConfigRec
{
    ConfigSet set;
    ConfigTransport transport;
    PortDsc *portPtr;
    char nodeName[511];
} ConfigRec;



    /*
     * Local prototypes
     */

static void PopulateCacheFromRPCprofile( ConfigSet , ServerSet *, ServerSet *);
StatusBlk InitGlobalSet(void);

/*
 *++
 *  InitGlobalSet
 *
 *  Functional Description:
 *
 *    This routine is called once at startup to initialize global opaque
 *    full names used by the global set routines.
 *
 *    DNATowersOpq is intialized to 'DNA$TOWERS'
 *    DTSSClassOpqName is initialized to K_TS_DNS_CLASS
 *
 *  Inputs:
 *	none
 *
 *  Implicit Inputs:
 *
 *  Outputs:
 * 	none
 *
 *  Implicit Outputs:
 *	none
 *
 *  Value Returned:
 *	StatusBlk
 *
 *  Side Effects:
 *      Globals are inited.
 *--
 */

StatusBlk InitGlobalSet(void)
{

    StatusBlk statusBlk;

    statusBlk.status = DTSS_SUCCESS;
    return(statusBlk);
}



/*
 *++
 *  PopulateGlobalCache ()
 *  PopulateLocalCache ()
 *
 *  Functional Description:
 *
 *      Enumerate the global/local set, either from the name service
 *      and/or from a local config file, depending on the environment:
 *      if the name service is available, use it. If there is a config
 *      file, use it.
 *
 *  Inputs:
 *	none
 *
 *  Implicit Inputs:
 *	The global/local cache queue
 *
 *  Outputs:
 * 	none
 *
 *  Implicit Outputs:
 *	none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *	populates the global/local cache
 *--
 */

void PopulateGlobalCache (void)
{

    PopulateCacheFromRPCprofile (CRglobal,
				 (ServerSet *)&sdb.globalSet,
				 (ServerSet *)&sdb.localSet);
}

void PopulateLocalCache (void)
{

    PopulateCacheFromRPCprofile (CRlocal,
				 (ServerSet *)&sdb.localSet,
				 (ServerSet *)&sdb.globalSet);
}


/*
 *  PopulateCacheFromRPCprofile ()
 *
 *  Functional Description:
 *
 *
 *  Inputs:
 *	none
 *
 *  Implicit Inputs:
 *	The global/local cache queue
 *
 *  Outputs:
 * 	none
 *
 *  Implicit Outputs:
 *	none
 *
 *  Value Returned:
 *	none
 *
 *  Side Effects:
 *	populates the global/local cache
 *
 */

static void PopulateCacheFromRPCprofile (
    ConfigSet configSet,
    ServerSet *setPtr,
    ServerSet *otherSet
)
{
    char                   moreBindings;
    CacheElement           *sp, *newServer;
    NodeAddr               zeroAddr, nodeAddr;
    unsigned32             status, rpcStatus, secStatus;
    unsigned_char_p_t      profileName,  serverName;
    char                   *cellName;
    char                   *serverEntryName;

#if CONFIG_SECURITY
    unsigned_char_p_t      serverPrincName;  /* Server global name */
    sec_rgy_name_t         serverRelName;    /* Server cell-relative name */
#endif

    rpc_binding_handle_t   importBinding_h;
    rpc_if_handle_t        TSinterface;
    rpc_ns_handle_t        importContext;
    uuid_t                 newServUUID;
    unsigned32             expirationAge;

    /*
     * Initialize variables.
     */
    moreBindings = FALSE;
    serverName      = (unsigned_char_p_t)NULL;
#ifndef HPDCE_FIX_CHFts16224
    profileName      = (unsigned_char_p_t)NULL;
#endif
#if CONFIG_SECURITY
    serverPrincName = (unsigned_char_p_t)NULL;

#endif

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
	  "-> Populating %s set cache\n",
	  configSet == CRglobal ? "global" : "local"));

    /*
     * If security is turned on, the name of the cell was obtained during
     * initialization.  Otherwise, obtain the cell name from the configuration
     * file.
     */

#if CONFIG_SECURITY
    cellName = (char *)rpcBufPtr->my_cell.name;
#else
    dce_cf_get_cell_name (&cellName, &confStatus);
    if (confStatus != dce_cf_st_ok)
    {
	dce_error_string_t tempstring;	
	dce_svc_printf(DTS_S_GET_CELL_NAME_MSG, 
		       dce_dts_error_text( rpcStatus, tempstring));
    }
    
#endif

    /*
     * Obtain the name of the profile, depending on the set type (Local
     * or Global) we want to import servers for.  First, free up memory
     * if it is already allocated.  GetProfileName will return us a
     * 'fresh' pointer to a the new profile name.
     */

    if (configSet == CRlocal)
    {
	GetProfileName (lan_profile_v1_0_c_ifspec,
			&rpcBufPtr->lanProfile,
			&rpcStatus);

	if (rpcStatus != rpc_s_ok)
	{
	    /*
	     * Profile loop detected, profile lacking a default is
	     * found or profile found but default entry does not refer to
	     * another profile.
	     *
	     */
	    dce_error_string_t tempstring;

	    LogEventNoLANProfile();
/*	    OLD_LOG_EVENT (rpcStatus, "Failure in GetProfileName: ");*/
	    dce_svc_printf(DTS_S_GET_PROFILE_NAME_MSG,
		       dce_dts_error_text( rpcStatus, tempstring));
	    return;
	}
        TSinterface = time_service_v1_0_c_ifspec;
	profileName = rpcBufPtr->lanProfile;

    }
    else
    {
	/*
	 * Cell profile is a cell relative well-known name, so we don't
	 * need to search for it.
	 * rpcBufPtr->cellProfile was obtained at startup time (from name
	 * constant).
	 */

	TSinterface = gbl_time_service_v1_0_c_ifspec;
	profileName = rpcBufPtr->cellProfile;
    }

    /*
     * If the Profile Name was successfully found, start importing
     * the time servers.
     */

    /*
     * Set context to import binding handles from the either the
     * LAN services or Cell services profile.
     */

    rpc_ns_binding_import_begin (rpc_c_ns_syntax_dce,
				 profileName,
				 TSinterface,
				 &rpcBufPtr->TServiceObjUUID,
				 &importContext,
				 &status);
    if (status != rpc_s_ok)
    {
	
	dce_error_string_t tempstring;
	dce_svc_printf(DTS_S_BINDING_IMPORT_MSG, 
		       dce_dts_error_text( status, tempstring ));
    }
    
	moreBindings = TRUE;

    /*
     * Disable CDS caching. By setting the expiration age to 0 on the
     * CDS import handle, we force the local name-service data to be
     * updated from the CDS server.  This guarantees that if new servers
     * (local or global) were exported to the name service, this client
     * knows about them.
     */
    expirationAge = 0;
    rpc_ns_mgmt_handle_set_exp_age (importContext,
				    expirationAge,
				    &status);
    if (status != rpc_s_ok)
    {
	
	dce_error_string_t tempstring;
	dce_svc_printf( DTS_S_SET_EXP_AGE_MSG, 
		       dce_dts_error_text( status, tempstring ));
    }
    
    while ( moreBindings )
    {
	importBinding_h = (rpc_binding_handle_t)NULL;

	rpc_ns_binding_import_next (importContext,
				    &importBinding_h,
				    &status);
	if (status == rpc_s_ok)
	{
	    SHOW_BINDING(dts_s_threads, "Partially bound handle",
		     importBinding_h);

	    rpc_ns_binding_inq_entry_name (importBinding_h,
					   rpc_c_ns_syntax_dce,
					   &serverName,
					   &status);
	    if (status != rpc_s_ok){
		dce_error_string_t tempstring;
                dce_svc_printf( DTS_S_BINDING_INQ_NAME_MSG, 
			       dce_dts_error_text( status, tempstring));
		goto DoNotAdd;
	    }

	    /*
	     * If running as a server, and the server imported is this entity,
	     * do not add it to the cache.
	     */
	    if (IsServer())
	    {
		unsigned_char_p_t servNamePtr;

		/*
		 * Do some 'editing' to get rid of the cell name.
		 */
		servNamePtr = serverName;
		servNamePtr += strlen(cellName)+1;

		if ( strcmp((char *)shmPtr->mgtParams.servEntryName,
                            (char *)servNamePtr) == 0)
		    goto DoNotAdd;
	    }

#if CONFIG_SECURITY

	    /*
	     * Obtain the server principal name by invoking a management
	     * routine, which queries the remote server, whose binding
	     * was obtained.  The binding has to be either fully-bound or
	     * have a UUID associated with it.
	     */

	    rpc_mgmt_inq_server_princ_name (importBinding_h,
					    rpc_c_authn_dce_private,
					    &serverPrincName,
					    &rpcStatus);
	    if (rpcStatus != rpc_s_ok)
	    {
		dce_error_string_t tempstring;
		dce_svc_printf(DTS_S_INQ_SERVER_PRINC_MSG, 
			       dce_dts_error_text( rpcStatus, tempstring));
		goto DoNotAdd;
	    }

	    /*
	     * Parse the Server principal name, which is returned by CDS
	     * as a global name, and obtain its cell-relative name and
	     * the UUID corresponding to this principal.
	     *
	     * NOTE: The current version of security (auth5a) does not
	     *       restricts names to be cell-relative. The next version,
	     *       auth5b, should lift this restriction.
	     */

	    memset((VOID *)serverRelName, 0, sec_rgy_name_t_size);

	    sec_id_parse_name (rpcBufPtr->secContext,
			       (idl_char *)serverPrincName,
			       (idl_char *)NULL,
			       (uuid_t *)NULL,
			       (idl_char *)serverRelName,
			       &newServUUID,
			       &secStatus);

	    if (secStatus != error_status_ok)
	    {
		dce_error_string_t tempstring;
		dce_svc_printf( DTS_S_SEC_ID_PARSE_MSG, 
			       dce_dts_error_text( secStatus, tempstring));
		goto DoNotAdd;
	    }

	    if (! serverInSecurityGroup (serverRelName))
	    {
		int hostServer = 0; /* not the time server on this host */
 		LogEventServerNotInGroup((char *)serverPrincName,
					 (char *)shmPtr->mgtParams.groupName,
					 hostServer);
		DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		      "Server -> %s is not a member of Time Servers group\n",
		       serverRelName));
		goto DoNotAdd;
	    }

	    /*
	     * Set authentication and autorization information
	     * into this binding handle.
	     */

	    rpc_binding_set_auth_info (importBinding_h,
				       serverRelName,
				       rpc_c_authn_level_default,
				       rpc_c_authn_dce_private,
				       (rpc_auth_identity_handle_t)
				              rpcBufPtr->loginContext_h,
				       rpc_c_authz_dce,
				       &secStatus);
	    if (secStatus != rpc_s_ok)
	    {
		dce_error_string_t tempstring;
		dce_svc_printf( DTS_S_SET_AUTH_INFO_MSG, serverRelName,
			       dce_dts_error_text( secStatus, tempstring));
	       goto DoNotAdd;
	    }

#else
	    /*
	     * If dts is not running with security, get the UUID corresponding
	     * to this server from the namespace.
	     */

	    rpc_ns_entry_object_inq_begin (rpc_c_ns_syntax_dce,
					   serverName,
					   &objInqContext,
					   &rpcStatus);
	    if (rpcStatus == rpc_s_ok)
	    {

		rpc_ns_entry_object_inq_next (objInqContext,
					      &newServUUID,
					      &rpcStatus);

		rpc_ns_entry_object_inq_done (&objInqContext,
					      &status);
		if (rpcStatus != rpc_s_ok)
		{
		    dce_error_string_t tempstring;
		    dce_svc_printf( DTS_S_ENTRY_OBJECT_INQ_NEXT_MSG, serverName, 
				   dce_dts_error_text( rpcStatus, tempstring));
		 goto DoNotAdd;
		}
	    }
	    else
	    {
		dce_error_string_t tempstring;
		dce_svc_printf( DTS_S_ENTRY_OBJECT_INQ_BEGIN_MSG, serverName,
			       dce_dts_error_text( rpcStatus, tempstring));
		goto DoNotAdd;
	    }

#endif /* CONFIG_SECURITY */

	    /*
	     * If this host has been configured as a Time Server,
	     * check its UUID against the imported server's UUID to
	     * make sure we don't add ourselves in the cache.
	     */

	    if ( IsServer() )
	    {
		if (uuid_equal (&rpcBufPtr->TServicePrincUUID,
				&newServUUID, &status))
		    goto DoNotAdd;
	    }

	    /*
	     * See if we already have this server in the cache.
	     * We don't want to add it again.
	     */

	    QloopBegin (&setPtr->cacheQ, &sp)
	    {
		if (uuid_equal (&sp->serverUUID, &newServUUID, &status) &&
		    (sp->portPtr->socketType == rpcPortPtr->socketType))
		    goto DoNotAdd;
	    }
	    QloopEnd;

	    /*
	     * Check the local or global cache, depending on which one is
	     * being populated, to make sure that a server that is in our LAN
	     * does not get added to the global cache.  Adding a server to both
	     * caches will have the side effect of a client sending
	     * time request to the same server twice.  This is especially true
	     * if a courier on
	     */

	    if (configSet == CRglobal)
	    {
		/*
		 * Populating the GLOBAL set. If this server is already in
		 * the local set, don't add it to the global cache.
		 * Note that in this case, otherSet is the local cache.
		 */

		QloopBegin (&otherSet->cacheQ, &sp)
		{
		    if (uuid_equal (&sp->serverUUID, &newServUUID, &status) &&
			(sp->portPtr->socketType == rpcPortPtr->socketType))
			goto DoNotAdd;
		}
		QloopEnd;
	    }
	    else  /* local set */
	    {
		/*
		 * Populating the LOCAL set. If this server is already in
		 * the global set, remove it from the global cache and
		 * continue. It will be added to the local cache.
		 * Note that in this case, otherSet is the global cache.
		 */

		QloopBegin (&otherSet->cacheQ, &sp)
		{
		    if (uuid_equal (&sp->serverUUID, &newServUUID, &status) &&
			(sp->portPtr->socketType == rpcPortPtr->socketType))
		    {
			RemoveServer (otherSet, sp);
			break;
		    }
		}
		QloopEnd;
	    }

	    /*
	     * Add this new server to the corresponding local or global
	     * cache.
	     */

#if CONFIG_SECURITY
	    serverEntryName = (char *)serverPrincName;
#else
	    serverEntryName = (char *)serverName;
#endif
	    newServer = AddServer (setPtr,
				   &zeroAddr,     /* No known node id */
				   &nodeAddr,     /* Node address or zero */
				   serverEntryName, /* Node name ptr */
				   (VOID *)NULL,
				   strlen(serverEntryName),
				   K_ROLE_NONCOURIER,
				   rpcPortPtr);

	    memcpy ((VOID *)&newServer->serverUUID,
		    (VOID *)&newServUUID, sizeof(uuid_t));

	    if (rpcPortPtr->socketType == K_SOCKET_DTS_RPC)
		newServer->bindPtr = CreateServerBinding ((char *)NULL,
							  (size_t)0,
							  importBinding_h );

	}

	else if (status == rpc_s_no_more_bindings)
	{
	    moreBindings = FALSE;
	    rpc_ns_binding_import_done (&importContext, &status);
	}
	else
	{
	    dce_error_string_t tempstring;

	    moreBindings = FALSE;
	    dce_svc_printf( DTS_S_BINDING_IMPORT_NEXT_MSG,
			   dce_dts_error_text( status, tempstring)); 
#ifndef __hpux_CHFts15592
	    rpc_ns_binding_import_done (&importContext, &status);
#endif
	}

DoNotAdd: ;

	/*
	 * Free up memory allocated by rpc
	 */

	if (importBinding_h != NULL)
	    rpc_binding_free (&importBinding_h, &status);

	if (serverName != NULL)
	    rpc_string_free (&serverName, &status);

#ifndef HPDCE_FIX_CHFts16224
	if (profileName != NULL)
	    rpc_string_free (&profileName, &status);
#endif

#if CONFIG_SECURITY
	if (serverPrincName != NULL)
	    rpc_string_free (&serverPrincName, &status);
#endif

    } /* end while ()  */
} /* end of PopulateCacheFromRPCprofile */


/*
 *  ExportGblTServiceToNS ()
 *
 *  Functional Description:
 *
 *
 *  Inputs:
 *	statusBlkPtr - return value.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 * 	none
 *
 *  Implicit Outputs:
 *	none
 *
 *  Value Returned:
 *	NCL return code
 *
 *  Side Effects:
 */

int ExportGblTServiceToNS (
    StatusBlk *statusBlk
)
{
    unsigned32    status;
    rpc_if_id_t        interfaceID;

    statusBlk->status = DTSS_SUCCESS;

    /*
     * Add server address information to the endpoint mapper database.
     */

    rpc_ep_register (gbl_time_service_v1_0_s_ifspec,
		     rpcBufPtr->bindingVector,
		     &rpcBufPtr->objUUIDvector,
		     (unsigned_char_p_t)"Global Time Service",
		     &status);

    if (status != rpc_s_ok)
    {
	dce_error_string_t tempstring;

	dce_svc_printf(DTS_S_EP_REGISTER_TP_MSG, 
		       dce_dts_error_text( status, tempstring));
	statusBlk->status    = K_DCE_ERROR;
	statusBlk->sysStatus = status;
	return( K_NCL_RSP_SYSTEM_ERROR );
    }

    /*
     * First, attempt to remove from the nameservice database
     * all binding handles corresponding to the time service
     * interface UUID.
     */
    rpc_ns_binding_unexport (rpc_c_ns_syntax_dce,
			     rpcBufPtr->TServiceEntryName,
			     gbl_time_service_v1_0_s_ifspec,
			     &rpcBufPtr->objUUIDvector,
			     &status);

    /*
     * Export the server's bindings to the node entry.
     */
    rpc_ns_binding_export (rpc_c_ns_syntax_dce,
			   rpcBufPtr->TServiceEntryName,
			   gbl_time_service_v1_0_s_ifspec,
			   rpcBufPtr->bindingVector,
			   &rpcBufPtr->objUUIDvector,
			   &status);

    if (status != rpc_s_ok)
    {
	dce_error_string_t tempstring;

	dce_svc_printf( DTS_S_EXPORT_GTS_MSG, 
		       dce_dts_error_text( status, tempstring ));
	statusBlk->status    = K_DCE_ERROR;
	statusBlk->sysStatus = status;
	return( K_NCL_RSP_SYSTEM_ERROR );
    }
    /*
     * Create entry in the CELL profile for this server.
     */

    rpc_if_inq_id (gbl_time_service_v1_0_s_ifspec,
		   &interfaceID,
		   &status);

    rpc_ns_profile_elt_add (rpc_c_ns_syntax_dce,
			    rpcBufPtr->cellProfile,
			    &interfaceID,
			    rpc_c_ns_syntax_dce,
			    rpcBufPtr->TServiceEntryName,
			    0,
			    (unsigned_char_p_t)"Global Time Server entry",
			    &status);
    if (status != rpc_s_ok &&
	status != rpc_s_profile_element_replaced)
    {
	dce_error_string_t tempstring;

	dce_svc_printf(DTS_S_ADD_PROFILE_MSG, 
		       dce_dts_error_text( status, tempstring));
	statusBlk->status    = K_DCE_ERROR;
	statusBlk->sysStatus = status;
	return( K_NCL_RSP_SYSTEM_ERROR );
    }

    /*
     * Set the server type to Global.
     */

    shmPtr->mgtParams.servType = K_SERVER_GLOBAL;

    /*
     * Register the Time Request interface for global servers with the
     * RPC run-time library, thus making available to clients.
     */

    rpc_server_register_if (gbl_time_service_v1_0_s_ifspec,
			    (uuid_p_t)NULL,
			    (rpc_mgr_epv_t) &gbl_time_service_epv,
			    &status);
    if (status != rpc_s_ok)
    {
	statusBlk->status    = K_DCE_ERROR;
	statusBlk->sysStatus = status;
	return( K_NCL_RSP_SYSTEM_ERROR );
    }

    return ( K_NCL_RSP_SUCCESS );
} /* end ExportGblTServiceToNS ()  */


/*
 *  UnexportGblTServiceFromNS ()
 *
 *  Functional Description:
 *
 *
 *  Inputs:
 *	statusBlkPtr - return value.
 *
 *  Implicit Inputs:
 *	none
 *
 *  Outputs:
 * 	none
 *
 *  Implicit Outputs:
 *	none
 *
 *  Value Returned:
 *	NCL return code
 *
 *  Side Effects:
 */

int UnexportGblTServiceFromNS (
    StatusBlk *statusBlk
)
{
    unsigned32    status;
    rpc_if_id_t   interfaceID;

    statusBlk->status = DTSS_SUCCESS;

    /*
     * Make sure that this server is a global server and has already
     * been advertised.
     */

    if (shmPtr->mgtParams.servType != K_SERVER_GLOBAL)
    {
	statusBlk->status    = K_DCE_ERROR;
	statusBlk->sysStatus = dts_s_not_global_server;
        return( K_NCL_RSP_SYSTEM_ERROR );
    }

    rpc_server_unregister_if (gbl_time_service_v1_0_s_ifspec,
			      NULL,
			      &status);
    if (status != rpc_s_ok)
    {
	statusBlk->status    = K_DCE_ERROR;
	statusBlk->sysStatus = status;
    }

    /*
     * Unregister this server from the endpoint mapper.
     */
    rpc_ep_unregister (gbl_time_service_v1_0_s_ifspec,
		       rpcBufPtr->bindingVector,
		       &rpcBufPtr->objUUIDvector,
		       &status);

    if (status != rpc_s_ok)
    {
	statusBlk->status    = K_DCE_ERROR;
	statusBlk->sysStatus = status;
        return( K_NCL_RSP_SYSTEM_ERROR );
    }

    /*
     * Unexport bindings for this server.
     */

    rpc_ns_binding_unexport (rpc_c_ns_syntax_dce,
			     rpcBufPtr->TServiceEntryName,
			     gbl_time_service_v1_0_s_ifspec,
			     NULL,
			     &status);

    /*
     * Remove this server from the CELL services profile.
     */

    rpc_if_inq_id (gbl_time_service_v1_0_s_ifspec,
		   &interfaceID,
		   &status);

    rpc_ns_profile_elt_remove (rpc_c_ns_syntax_dce,
			       rpcBufPtr->cellProfile,
			       &interfaceID,
			       rpc_c_ns_syntax_dce,
			       rpcBufPtr->TServiceEntryName,
			       &status);
    if (status != rpc_s_ok)
    {
	statusBlk->status = K_DCE_ERROR;
	statusBlk->status = status;
	return( K_NCL_RSP_SYSTEM_ERROR );
    }

    /*
     * Set the server type back to Local Server.
     */

    shmPtr->mgtParams.servType = K_SERVER_LOCAL;

    return ( K_NCL_RSP_SUCCESS );
} /* end UnexportGblTServiceFromNS ()  */



