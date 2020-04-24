/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fxConnect.c -- 
 *    C functions for implementing the fileset connection data type.
 */
/*
 * HISTORY
 * $Log: itl_fxConnect.c,v $
 * Revision 1.1.315.1  1996/10/17  18:26:17  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:14  damon]
 *
 * Revision 1.1.310.1  1994/02/04  20:44:17  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:00  devsrc]
 * 
 * Revision 1.1.308.2  1994/01/20  18:44:08  annie
 * 	added copyright header
 * 	[1994/01/20  18:40:25  annie]
 * 
 * Revision 1.1.308.1  1993/12/07  17:44:01  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:17:40  jaffe]
 * 
 * $EndLog$
 */

#include <assert.h>
#include <pthread.h>
#include <netinet/in.h>
#include <dce/rpc.h>
#include <dce/sec_login.h>
#include <dcedfs/lock.h>
#include <dcedfs/afs4int.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_exp.h>
#include <utils/itl/itl_fxDCEError.h>
#include <utils/itl/itl_fxCommon.h>
#include <utils/itl/itl_fxToken.h>
#include <utils/itl/itl_fxPrincipal.h>
#include <utils/itl/itl_fxServer.h>
#include <utils/itl/itl_fxFileset.h>
#include <utils/itl/itl_fxConnect.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxConnect.c,v 1.1.315.1 1996/10/17 18:26:17 damon Exp $")

/*
 *  ITL_FX_CONNECT_BUILD_PRINCIPAL() --
 *    Builds a principal name for a server.
 *
 *  Assumptions:
 *    Standard DCE configuration, i.e. hostnames are in
 *    /.../<cellname>/<hostname>
 *
 *  Arguments:
 *    _cName - cell name
 *    _sName - server name
 *    _strP  - char pointer into which the string will be placed.  This 
 *             string is malloc'ed and must be free'd by the caller.
 *
 *  Note: The constant 18, accounts for the embedded strings, and the
 *        end of string character.
 */
#define ITL_FX_CONNECT_BUILD_PRINCIPAL(_cName, _sName, _strP)            \
        MACRO_BEGIN                                                      \
          ITL_UTIL_ALLOC((_strP), char *, (strlen((_cName)) +            \
					   strlen((_sName)) + 18));      \
          sprintf((_strP), "/.../%s/%s/dfs-server", (_cName), (_sName)); \
        MACRO_END
/*
 *  NOTE:
 *    The value 6 given to ITL_FX_HANDLE_STR_LEN is the maximum length
 *    for a string value of a handle_t.  This is
 *    assuming the maximum value of a short, which is what a handle_t
 *    is currently defined to be.  Ideally a print function should be
 *    defined for the data type.
 */

#define ITL_FX_HANDLE_STR_LEN  6


PRIVATE int itlFX_connectDataType;

PRIVATE itlValue_varTypeInfo_t fxConnectVarType = {
    "fs_fxHandle",
    itlFX_ConnectCopy,
    itlFX_ConnectNew,
    itlFX_ConnectFree,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    itlFX_ConnectDisplay,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};


PRIVATE boolean_t MakeConnection _TAKES((
					 IN itlFX_server_t *serverP,
					 IN itlFX_principal_t *principalP,
					 IN unsigned long interface,
					 INOUT handle_t *handleP
					 ));

/*
 *  ConnectInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void ConnectInit()
{
    /*
     *  We can safely set connectHashType because this function is called
     *  via pthread_once.
     */
    itlFX_connectDataType = itlValue_RegisterVarType(&fxConnectVarType);
}

/*
 * itlFX_ConnectInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlFX_ConnectInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    /*
     *  First ensure the hash table module is initialized.
     */
    itlu_NameHashInit();

    if (pthread_once(&hashOnce, (pthread_initroutine_t) ConnectInit))
	itl_Error("pthread_once failed in ConnectInit\n");
}

PRIVATE boolean_t MakeConnection(serverP, principalP, interface, handleP)
  IN itlFX_server_t *serverP;
  IN itlFX_principal_t *principalP;
  IN unsigned long interface;
  INOUT handle_t *handleP;
{

    unsigned char *stringBinding;
    char addressString[64];     /* 64 should be large enough */
    char endPointString[16];    /* 16 should be large enough */
    unsigned32 ipAddress, ipPort;
    struct sockaddr_in serverAddress;
    handle_t binding = 0;
    unsigned32 statusCode;
    boolean_t success = FALSE;

    /*
     * NOTE: Assuming the protocol used  here is UPD/IP. Otherwise, the
     *       code should be rewritten. 
     */


    /*
     *  Convert the server port address to a string and get an rpc
     *  binding.
     */
    serverAddress = ITL_FX_SERVER_ADDRESS(serverP);
    ipAddress = ntohl(serverAddress.sin_addr.s_addr);
    ipPort = ntohs(serverAddress.sin_port);

    sprintf(addressString, "%ld.%ld.%ld.%ld", (ipAddress >> 24),
	                                      ((ipAddress >> 16) & 0xff),
	                                      ((ipAddress >> 8) & 0xff),
	                                      (ipAddress & 0xff));
    sprintf(endPointString, "%d", ipPort);


    rpc_string_binding_compose((unsigned_char_p_t) NULL,   /* uuid */
			       (unsigned_char_p_t) ITL_FX_DFS_PROT_SEQ,  /* protocol sequence */
			       (unsigned_char_p_t) addressString,      /* network addr */
			       (unsigned_char_p_t) endPointString,     /* endpoint */
			       (unsigned_char_p_t) NULL,    /* NULL options */
			       &stringBinding,
			       &statusCode);

    if (statusCode == error_status_ok)
    {
	rpc_binding_from_string_binding(stringBinding, &binding, &statusCode);
	if (statusCode == error_status_ok)
	{
	    char *serverName;
	    rpc_auth_identity_handle_t rpcAuthIdentity;
	    unsigned32 authnService, authzService;

	    /*
	     *  Set up authentication info.
	     */
	    ITL_FX_CONNECT_BUILD_PRINCIPAL(ITL_FX_CELL_NAME, 
					   ITL_FX_SERVER_NAME(serverP),
					   serverName);

	    /*
	     *  If principalP is NULL, then we are establishing an
	     *  un-authenticated binding.
	     */
	    if (principalP)   /* authenticated binding */
	    {
		rpcAuthIdentity = (rpc_auth_identity_handle_t) 
		                  ITL_FX_PRINCIPAL_GET_LOGIN(principalP);
		authnService = rpc_c_authn_dce_secret;
		authzService = rpc_c_authz_dce;
	    }
	    else             /* un-authenticated binding */
	    {
		rpcAuthIdentity = (rpc_auth_identity_handle_t) 0;
		authnService = rpc_c_authn_none;
		authzService = rpc_c_authz_none;
	    }

	    rpc_binding_set_auth_info(binding, (unsigned_char_p_t) serverName,
				      rpc_c_authn_level_pkt, authnService,
				      rpcAuthIdentity, authzService,
				      &statusCode);

	    if (statusCode == error_status_ok) 
	    {
		afsUUID interfaceUUID;

		interfaceUUID = (interface == ITL_FX_PRIMARY_IF) ?
		                 ITL_FX_SERVER_PRIMARY(serverP)  :
				 ITL_FX_SERVER_SECONDARY(serverP);

		rpc_binding_set_object(binding, &interfaceUUID, &statusCode);
		if (statusCode == error_status_ok) 
		{
		    success = TRUE;
		    *handleP = binding;
		}
		else
		{
		    ITL_FX_DCE_ERROR(statusCode, 
				     "Could NOT set object binding");
		}
	    }
	    else
	    {
		ITL_FX_DCE_ERROR(statusCode, "Could NOT set rpc authorization");
	    }
	    ITL_FREE(serverName);
	    if (!success)
		rpc_binding_free(&binding, &statusCode);
	}
	else
	{
	    ITL_FX_DCE_ERROR(statusCode, "Could NOT rpc binding from string");
	}
	rpc_string_free(&stringBinding, &statusCode);
    }
    else 
    {
	ITL_FX_DCE_ERROR(statusCode, "Could NOT compose string binding");
    }

    return(success);
}

/*
 * itlFX_ConnectCreate() --
 *    Create a connection and store the resulting rpc binding in a 
 *    variable.
 *
 * RETURN CODES: 
 *    FALSE - Successful creation.
 *    TRUE  - Unsuccessful creation.
 */
SHARED boolean_t itlFX_ConnectCreate(filesetName, interface, userName, varName)
  IN char *filesetName;  
  IN unsigned int interface;       /* server interface to connect to */
  IN char *userName, *varName;
{
    boolean_t retVal = TRUE;
    itlFX_server_t *serverP;
    itlFX_fileset_t *filesetP;
    itlFX_principal_t *principalP = (itlFX_principal_t *) 0;

    /*
     *  First verify the fileset name is valid.
     */
    if ((filesetP = itlFX_FilesetLookup(filesetName))
    	         != (itlFX_fileset_t *) 0)
    {
	serverP = ITL_FX_FILESET_SERVER(filesetP);

	/*
	 *  If the userName is the empty string, then we use a NULL principal
	 *  to indicate this in an un-authenticated connection.
	 */
	if ((!strcmp(userName, "")) ||
	    ((principalP = itlFX_PrincipalLookup(userName))
	                != (itlFX_principal_t *) 0))
	{
	    handle_t newHandle;

	    /*
	     *  Now actually, establish a connection.
	     */
	    if (MakeConnection(serverP, principalP, interface, &newHandle))
	    {
		var_val_t *newVarP;
		itlFX_connect_t *newConnectP;

		ITL_NEW(newConnectP, itlFX_connect_t);
		newConnectP->handle = newHandle;
		newConnectP->interface = interface;
		newConnectP->filesetP = filesetP;
		newVarP = itlValue_New((pointer_t) newConnectP, 
				       itlFX_connectDataType);
		if (itl_SetVariable(varName, newVarP, var_local))
		    retVal = FALSE;
		else
		    ITL_FREE(newConnectP);

		ITL_FREE(newVarP);
	    }
	}
	else
	{
	    itl_Error("Invalid user name given, %s\n", userName);
	}
    }
    else
    {
	itl_Error("Invalid fileset name given, %s\n", filesetName);
    }

    return(retVal);
}

/*
 *  itlFX_ConnectGet() --
 *    Will prompt for and obtain the value of a fs_fxConnect variable.
 */
SHARED itlFX_connect_t *itlFX_ConnectGet()
{
    itlFX_connect_t *connectInfoP;
    var_val_t *connectVarP;

    itl_PromptUser("Fileset Handle Variable (fs_fxHandle): ");
    connectVarP = itl_GetVarVal();
    assert(connectVarP);
    if (VALUE_TYPE(connectVarP) == itlFX_connectDataType)
	connectInfoP = (itlFX_connect_t *) VALUE_VAL(connectVarP);
    else 
	connectInfoP = (itlFX_connect_t *) 0;

    return(connectInfoP);
}

/**
 **  fs_fxConnect variable rouines.
 **/

SHARED var_val_t *itlFX_ConnectNew(newConnectP, newType)
  IN pointer_t newConnectP;
  IN int newType;
{
    var_val_t *newVarP;
    itlFX_connect_t *connectP = (itlFX_connect_t *) newConnectP;

    ITL_UTIL_NEW(newVarP, var_val_t);
    VALUE_TYPE(newVarP) = newType;
    VALUE_VAL(newVarP) = newConnectP;
    VALUE_SIZE(newVarP) = (int) sizeof(itlFX_connect_t);
    return(newVarP);
}

SHARED boolean_t itlFX_ConnectFree(freeConnectP)
  IN var_val_t *freeConnectP;
{
    if (VALUE_VAL(freeConnectP))
	ITL_UTIL_FREE(VALUE_VAL(freeConnectP));
    return(FALSE);
}

SHARED var_val_t *itlFX_ConnectCopy(copyConnectP)
  IN var_val_t *copyConnectP;
{
    var_val_t *newConnectP;
    itlFX_connect_t *connectP;

    newConnectP = itl_NewVarVal();
    ITL_UTIL_ALLOC(connectP, itlFX_connect_t *, VALUE_SIZE(copyConnectP));
    VALUE_SIZE(newConnectP) = VALUE_SIZE(copyConnectP);
    VALUE_TYPE(newConnectP) = VALUE_TYPE(copyConnectP);
    VALUE_VAL(newConnectP) = connectP;
    ITL_UTIL_MEMCOPY(VALUE_VAL(copyConnectP), VALUE_VAL(newConnectP), 
		     VALUE_SIZE(copyConnectP));
    return(newConnectP);
}

SHARED char *itlFX_ConnectDisplay(displayConnectP, lengthP)
  IN var_val_t *displayConnectP;
  OUT int *lengthP;
{
    itlFX_connect_t *connectP;
    char *returnStr;
    unsigned int strLength;

    connectP = (itlFX_connect_t *) VALUE_VAL(displayConnectP);

    /*
     *  Lengths below include the field names inserted in the sprintf
     *  that follows.
     */
    strLength = ITL_FX_HANDLE_STR_LEN + 9 +   /* handle plus a space */
	        7 +                           /* interface plus space */
		20 + 10;                      /* fileset, space, and pad */

    ITL_UTIL_ALLOC(returnStr, char *, strLength);
    sprintf(returnStr, "handle: %d, i/f: %d, fileset: %x", 
	    (int) connectP->handle, connectP->interface, connectP->filesetP);

    if (lengthP)
	*lengthP = strLength;

    return(returnStr);
}

