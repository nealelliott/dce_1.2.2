/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth.p.h,v $
 * Revision 1.1.704.1  1996/10/02  20:58:43  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:14  damon]
 *
 * $EndLog$
*/
/*
 *	dfsauth.h -- header file for use with the DFS authentication package
 *
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All rights reserved.
 */

#ifndef TRANSARC_DFSAUTH_H
#define TRANSARC_DFSAUTH_H 1

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dce/rpc.h>

#include <dcedfs/dacl.h>

/* the following type is modelled after the bosserver's keyinfo description */
typedef struct dfsauth_keyinfo {
  long int mod_sec;
  long int mod_usec;
  unsigned long int keyCheckSum;
  unsigned long int spare1;
  unsigned long int spare2;
  unsigned long int spare3;
  unsigned long int spare4;
} dfsauth_keyinfo_t;

/*
 *   For all the dfsauth routines, a zero (0) return code indicates that the requested
 * operation(s) succeeded.  Non-zero return codes indicate an error.  All potential
 * dfsauth error codes are listed, with explanatory text, in the source file
 * security/dfsauth/daut_errs.et and are defined as constants in the automatically
 * produced header file, afs/dfsauth.h.
 */
/*************************  general purpose routines *************************/
#if !defined(KERNEL)
IMPORT boolean32 dfsauth_rpc_mgmt_authz_check _TAKES((
				      IN  rpc_binding_handle_t	client_binding,
				      IN  unsigned32		requested_mgmt_operation,
				      OUT unsigned32 *		statusP
						    ));
#endif /* !defined(KERNEL) */

/*
 * The key management covers.
 * Nobody other than the dfsauth package and the bosserver should be using these routines
 * or definitions.
 */
#if defined(sec_key_mgmt_v0_0_included)
/*
 * We believe that the sec_key_mgmt routines should be changed to allow the authn service
 * parameter to be rpc_c_authn_default, allowing export concerns to be concentrated
 * in the HP/Apollo code.  However, for DCE 1.0, only rpc_c_authn_dce_private
 * is allowed.  Should default be allowed in the future, this definition should be
 * changed to that.  If not, this definition should be changed to be rpc_c_authn_dce_dummy
 * (I think) for exportable versions.
 */
#if defined(SEC_KEY_MGMT_ALLOWS_DEFAULT_AUTH_SERIVCE)
#define dfsauth_sec_key_mgmt_authn_service	rpc_c_authn_dce_default
#else /* defined(SEC_KEY_MGMT_ALLOWS_DEFAULT_AUTH_SERIVCE) */
#if defined(DFSAUTH_FOR_EXPORT)
#define dfsauth_sec_key_mgmt_authn_service	rpc_c_authn_dce_dummy
#else /* defined(DFSAUTH_FOR_EXPORT) */
#define dfsauth_sec_key_mgmt_authn_service	rpc_c_authn_dce_private
#endif /* defined(DFSAUTH_FOR_EXPORT) */
#endif /* defined(SEC_KEY_MGMT_ALLOWS_DEFAULT_AUTH_SERIVCE) */
  
/*
 * The default key version definitely should be changed (to sec_passwd_plain)
 * for exportable code.
 */
#if defined(DFSAUTH_FOR_EXPORT)
#define dfsauth_sec_key_mgmt_default_keytype	sec_passwd_plain
#else /* defined(DFSAUTH_FOR_EXPORT) */
#define dfsauth_sec_key_mgmt_default_keytype	sec_passwd_des
#endif /* defined(DFSAUTH_FOR_EXPORT) */

IMPORT void dfsauth_sec_key_mgmt_free_key _TAKES((
						  void *	voidKeydataP
						));

IMPORT void dfsauth_sec_key_mgmt_keyinfo _TAKES((
						 void *			voidKeydataP,
						 dfsauth_keyinfo_t *	keyinfoP,
						 char *			principalNameP
					       ));
#endif /* defined(sec_key_mgmt_v0_0_included) */

/*************************  server routines *************************/

/*
 * dfsauth_server_InitAuthentication:
 *   main initialization routine for a server - This routine MUST be called before
 * any other dfsauth_server_* routines are called.
 *
 *  This routine sets up all the local context, including context required to
 * make authenticated out-bound RPC calls, for a DFS server.  The local identity
 * of the server is established with the DCE security sub-system, and the name of
 * the file to be consulted in the making of server authorization decisions (the
 * server's UserList) is saved for later use.
 *
 *
 * parameters:
 *	principalNameP: the principal name under which the server will run.  This
 * parameter is REQUIRED in kernel space, but can be defaulted to the dfs principal name
 * for the local host in user-space
 *	listFilenameP: the name of the file containing the admin list against which
 * authorization checks will be made.  This parameter is REQUIRED in user-space,
 * and must be NULL in kernel-space.
 *	serverExtensionP: this parameter is only used when principalNameP is NULL and
 * the server's principal is obtained from a local file.  If the extension is non-NULL,
 * it is expected to be the string representation of a uuid representing one of the
 * pre-defined types of servers (see the files ./dfsauth_localprincipal.c and
 * dfsauth_uuidmap.h).  The uuid map will be used to determine a suffix for the
 * principal to be used by the server.
 *	This feature is currently unused by DFS code; should the decision 
 * be made to use it, the uuid maps will be extended.  Until then, NULL should
 * be passed for this parameter.
 */
IMPORT long dfsauth_server_InitAuthentication _TAKES((
						      IN char *	principalNameP,
						      IN char *	listFileNameP,
						      IN char *	serverExtensionP
						    ));

/*
 * dfsauth_server_CheckAuthorization:
 *   routine to be called when a server requires an authorization check for a received RPC
 *
 *   This routine extracts authentication/authorization information from the RPC
 * binding handle passed to the server by the RPC runtime system.  The routine
 * checks that information and the desired access permissions, permsetP, against
 * the ACL found in in the appropriate UserList file, either the one given during
 * server intialization, or the one specified by the listFileNameOverrideP parameter,
 * to determine whether or not the user is authorized to have perform any operation
 * that requires the desired permissions.  This routine will, also, check whether or
 * the authentication information in the binding handle indicates that the caller is
 * a member of the (optionally) specified group, authGroupUuidP.  If the principal
 * represented by the authentication information in the binding handle is appropriately
 * authorized by the UserList, zero (0) is returned from this routine.  If the user is
 * not authorized, DAUTH_ERROR_ACCESS_DENIED is returned.  Other return values indicate
 * some other type of error (see daut_errs.et).
 *
 * parameters:
 *	rpcBindingP: pointer to the RPC binding handle that the RPC runtime system
 * intialized for the server when it dispatched the RPC routine.  This parameter
 * cannot be NULL.
 *	permsetP: the permissions that are to be checked for the caller.  Currently,
 * the permset, rwc, is considered to be the DFS permset, and is the set used if a NULL
 * is passed for the permsetP pointer.  This parameter is here, mainly, for future
 * expansion.
 *	listFileNameOverrideP:  If this parameter is non-NULL, the filename given
 * is used to specify the UserList/ACL file against which the rpcBindingP and permsetP
 * authentication/authorization information is to be checked.  If this parameter is
 * NULL (the much more common case), the filename * specified when
 * dfsauth_server_InitAuthentication was called is used. This parameter is here, mainly,
 * for future expansion.
 *	authGroupUuidP:  If this parameter is non-NULL, the principal will be considered
 * authorized (and zero (0) returned) if it is a member of the given group.
 *
 */
IMPORT long dfsauth_server_CheckAuthorization _TAKES((
					      IN handle_t 	*rpcBindingP,
					      IN dacl_permset_t *	permsetP,
					      IN char *		listFileNameOverrideP,
					      IN uuid_t *		authGroupUuidP
					    ));

/*
 * dfsauth_server_CheckAuthorizationGroup:
 *  routine to check whether or not the principal represented by the authentication
 * information in the RPC binding handle is a member of the given group.
 *
 * parameters:
 *	rpcBindingP: RPC binding handle containing authentication information about
 * the principal making a call to an RPC server.  This parameter cannot be NULL.
 *	authGroupUuidP: authentication group for which the principal represented by
 * the authentication information in rpcBindingP is to be tested for membership.
 * This parameter may not be NULL.
 */
EXPORT long dfsauth_server_CheckAuthorizationGroup _TAKES((
					  IN rpc_binding_handle_t *	rpcBindingP,
					  IN uuid_t *			authGroupUuidP
							));

/*****
 ***** the following routines should only be called directly by the bosserver
 *****/
/*
 * main initialization routine for the bosserver
 *
 * parameters:
 *	principalNameP: the principal name under which the server will run.  This
 * parameter is REQUIRED in kernel space, but can be defaulted to the dfs principal name
 * for the local host in user-space
 *	listFilenameP: the name of the file containing the admin list against which
 * authorization checks will be made.  This parameter is REQUIRED in user-space,
 * and must be NULL in kernel-space.
 *	serverExtensionP: this parameter is only used when principalNameP is NULL and
 * the server's principal is obtained from a local file.  If the extension is non-NULL,
 * it is expected to be the string representation of a uuid representing one of the
 * pre-defined types of servers (see the files ./dfsauth_localprincipal.c and
 * dfsauth_uuidmap.h).  The uuid map will be used to determine a suffix for the
 * principal to be used by the server.
 *	This feature is currently unused by DFS code; should the decision 
 * be made to use it, the uuid maps will be extended.  Until then, NULL should
 * be passed for this parameter.
 *	noAuth:	an integer indicating whether or not the server is to run without
 * authorization checking.  If the value of noAuth is non-zero, the server will refrain
 * from doing any of the rpc_server initialization calls.
 */
IMPORT long dfsauth_server_InitAuth _TAKES((
					    IN char *	principalNameP,
					    IN char *	listFileNameP,
					    IN char *	serverExtensionP,
					    IN int	noAuth
					  ));

/*
 * routines for setting/getting NoAuth status of entire set of DFS servers
 *
 *  The NoAuth status of all the DFS servers on a single machine is controlled by
 * the presence/absence of a single file: <dcelocal>/var/dfs/NoAuth on the local
 * disk.  dfsauth_server_SetNoAuthStatus ensures that that file is present if its
 * parameter, newStatus is non-zero.  If newStatus is zero, dfsauth_server_SetNoAuthStatus
 * ensures that the file is not present.
 *  dfsauth_server_GetNoAuthStatus returns a non-zero value in *oldStatusP if the NoAuth
 * file is present, and zero (0) in *oldStatusP is the NoAuth file is absent.  The
 * oldStatusP parameter may not be NULL.
 */
IMPORT long dfsauth_server_SetNoAuthStatus _TAKES((IN int newStatus));

IMPORT long dfsauth_server_GetNoAuthStatus _TAKES((INOUT int * oldStatusP));

/* call to make sure server is running as 'self' */
IMPORT void dfsauth_server_RunAsSelf _TAKES((void));

/*
 * A server calls this function to get the ID of the principal that issued
 * an RPC call. The input to this routine is an RPC binding handle passed
 * down from the server stub. The UUIDs for the callers realm and principal
 * are retrieved from the callers credentials. The routine will fail if
 * the client is not authenticated through DCE.
 */
IMPORT long dfsauth_server_GetClientIdentity _TAKES((
	IN  rpc_binding_handle_t bindingHndl,
	OUT uuid_t 		 *realmUuidP,
	OUT uuid_t		 *princUuidP
	));

/*************************  client routines *************************/
/*
 * dfsauth_client_IsClientAuthenticated
 *  routine takes no parameters and returns non-zero if the client is currently 
 * authenticated (i.e., has a default login context) and 0 if not
 */
IMPORT long dfsauth_client_IsClientAuthenticated _TAKES((void));

/*
 * dfsauth_client_InitAuthentication:
 *  main initialization routine for a client - this routine, or
 * dfsauth_client_InitBindingAuth, must be called before any other dfsauth_client_*
 * routines are called.
 *
 *  This routine will perform dfsauth client system initialization, if the initialization
 * has not yet been performed, and it will attach appropriate authentication information
 * to the RPC binding handle passed to it.  If both useNoAuth nor useLocalAuth
 * are zero (0), the authentication information attached to the binding handle will
 * identify the user of the binding handle as network principal associated with
 * the calling process (i.e., if dce_login, or klog, or some equivalent operation
 * has been performed before running the client program (and the network credentials
 * are unexpired), the network identity established by that operation will be associated
 * with the binding handle).
 *
 * parameters:
 *	serverBindingHandleP: This parameter should point to the RPC binding handle
 * with which authentication information is to be associated.  Any RPCs made using
 * this binding handle, after calling dfsauth_client_InitBindingAuth will pass the called
 * server information identifying the caller, as described above.  This parameter
 * may be NULL.
 *	serverLocationP: This parameter should point to a character string containing
 * the CDS pathname of the DFS server for which the given binding handle will be used.
 * The dfsauth code will construct a principal name from the CDS pathname, and
 * associate that principal name with the given binding handle.  This parameter
 * may be NULL.  If it is NULL, the code will assume that the caller wants to use
 * the local host as the server.
 *	useNoAuth:  If this value is non-zero, the binding handle will not have
 * any authentication associated with it.
 *	useLocalAuth:  If this value is non-zero, the binding handle will have
 * the identity of the local DFS servers associated with it, rather than the
 * identity of the calling process.  In order for this to succeed, the uid of the
 * calling process must be root (0), and the local machine must have a DFS server
 * identity (some client-only machines may not be configured to have a local DFS
 * server identity).  Distinguished errors are returned in case either of these conditions
 * is not fulfilled (DAUT_ERROR_UID_NOT_LOCAL_ROOT and DAUT_ERROR_NO_LOCAL_IDENTITY,
 * repectively).  (The uid test is administered first.)
 *	NB: If both useNoAuth and useLocalAuth are non-zero, the client will be
 * initialized as if useNoAuth were non-zero, but useLocalAuth were zero (i.e,
 * in noAuth mode).
 */
IMPORT long dfsauth_client_InitAuthentication _TAKES((
				      rpc_binding_handle_t *	serverBindingHandleP,
				      char *			serverLocationP,
				      int			useNoAuth,
				      int			useLocalAuth
						    ));

/*
 * dfsauth_client_InitBindingAuth:
 *  secondary initialization routine for a client - this routine, or
 * dfsauth_client_InitAuthentication, must be called before any other dfsauth_client_*
 * routines are called.
 *
 *  This routine will perform dfsauth client system initialization, if the initialization
 * has not yet been performed, and it will attach appropriate authentication information
 * to the RPC binding handle passed to it.  If both useNoAuth nor useLocalAuth
 * are zero (0), the authentication information attached to the binding handle will
 * identify the user of the binding handle as network principal associated with
 * the calling process (i.e., if dce_login, or klog, or some equivalent operation
 * has been performed before running the client program (and the network credentials
 * are unexpired), the network identity established by that operation will be associated
 * with the binding handle).
 *
 * parameters:
 *	serverBindingHandleP: This parameter must point to the RPC binding handle
 * with which authentication information is to be associated.  Any RPCs made using
 * this binding handle, after calling dfsauth_client_InitBindingAuth will pass the called
 * server information identifying the caller, as described above. This parameter may
 * not be NULL.
 *	useNoAuth:  (see description of useNoAuth parameter to
 * dfsauth_client_InitAuthentication and usage note, above)
 *	useLocalAuth:  (see description of useNoAuth parameter to
 * dfsauth_client_InitAuthentication and usage note, above)
 *	serverPrincipalNameP: The parameter must point to a character string containing
 * the principal name of the server for which the given server binding handle will
 * be used. This parameter may not be NULL.
 */
IMPORT long dfsauth_client_InitBindingAuth _TAKES((
				   rpc_binding_handle_t *	serverBindingHandleP,
				   int				useNoAuth,
				   int				useLocalAuth,
				   char *			serverPrincipalNameP
						 ));

/*
 * dfsauth_client_CleanupAuthentication:
 *  routine cleans up and frees localAuth credentials that were established by
 * the client at initialization and performs any other cleanup that may be required.
 * This routine MUST be called if the client id running in localAuth mode (i.e., if
 * the value of the useLocalAuth parameter to the dfsauth_client initialization
 * routine was non-zero).  It use is, currently, optional when not running in localAuth
 * mode, but calling will never hurt in those cases, and may ease future expansion.
 */
IMPORT long dfsauth_client_CleanupAuthentication _TAKES((void));

/*
 * dfsauth_client_RefreshLocalAuthContext:
 *  routine refreshes a security system context in which the client is identified
 * as the principal representing the DFS servers on the local machine.  This routine
 * should be used by DFS servers that act as clients by making out-bound RPCs to other
 * DFS servers when the client-server's ticket-granting ticket (TGT) expires.  This
 * condition can be detected by a return code of rpc_s_auth_tkt_expired from an RPC
 * attempt (eventually, right now, they just raise an rpc_x_comm_failure exception,
 * though they tell me that this will change).  If a code (exception) is returned
 * (raised), the client should call dfsauth_client_RefreshLocalAuthContext, then
 * (if the refresh was successful) retry the RPC.
 */
IMPORT long dfsauth_client_RefreshLocalAuthContext _TAKES((void));

/**********
 * The only known use for the following routines is the bos command, when
 * sending key information back and forth between itself and the bosserver.  Full
 * encryption imposes a large performance penalty, and should not be used indiscriminantly.
 **********/
  
/*
 * dfsauth_client_SetFullEncryption:
 *  routine to set binding client's handle to a state in which further RPCs made on
 * the binding handle will use the highest level of privacy (usually forcing
 * encryption of RPC routine codes and in and out parameters) available.  The highest
 * level, encrypting everything, may not be available in versions for export.  The
 * serverBindingHandleP parameter may not be NULL.  This routine saves the authentication
 * level of the binding handle when the routine is first called, for later restoration
 * by the dfsauth_client_ResetDefaultEncryption routine.
 */
IMPORT long dfsauth_client_SetFullEncryption _TAKES ((
				      rpc_binding_handle_t *	serverBindingHandleP
						    ));

/*
 * dfsauth_client_ResetDefaultEncyption:
 *  routine restores a lower level of protection of routine codes and parameter
 * values for the given binding handle.  If dfsauth_client_SetFullEncryption is called
 * before dfsauth_client_ResetDefaultEncryption, the privacy (authentication) level
 * for the binding handle is restored to its initial value at the start of the
 * dfsauth_client_SetFullEncryption call.  If dfsauth_client_SetFullEncryption is not
 * called first, the privacy (authentication) level for the binding is set to the default
 * value for the RPC/security runtime system. The serverBindingHandleP parameter may
 * not be NULL.
 */
IMPORT long dfsauth_client_ResetDefaultEncryption _TAKES((
				  rpc_binding_handle_t *	serverBindingHandleP
							));

#if !defined(KERNEL)
/*
 * The following three functions provide an interface to check to see if a 
 * principal (identified by the realm and principal uuid pair) exists in 
 * an admin list.  The first function takes the name of an admin list.  If
 * this name is NULL, the default admin list for the server is used.  It returns
 * a cookie that can be used by the subsequent calls.
 *
 * The second function does the actual check.  If takes the cookie and the two
 * uuids.  It returns 0 if the principal is NOT in the admin list; non-zero
 * otherwise.
 *
 * The third function cleans up.
 */
IMPORT void * dfsauth_server_CheckAdminListAccessBegin _TAKES((char *));

IMPORT int dfsauth_server_CheckAdminListAccessNext _TAKES((void *, uuid_t *, uuid_t *));

IMPORT void dfsauth_server_CheckAdminListAccessEnd _TAKES((void *));

#endif /* !KERNEL */
#endif /* TRANSARC_DFSAUTH_H */
