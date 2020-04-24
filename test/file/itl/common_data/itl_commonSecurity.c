/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1993 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_commonSecurity.c -- 
 *    C functions for implementing the DCE security context.
 *
 */
/*
 * HISTORY
 * $Log: itl_commonSecurity.c,v $
 * Revision 1.1.117.1  1996/10/17  18:25:27  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:48  damon]
 *
 * Revision 1.1.112.1  1994/02/04  20:43:22  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:38  devsrc]
 * 
 * Revision 1.1.110.2  1994/01/20  18:43:59  annie
 * 	added copyright header
 * 	[1994/01/20  18:40:17  annie]
 * 
 * Revision 1.1.110.1  1993/12/07  17:43:24  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  22:00:07  jaffe]
 * 
 * $EndLog$
 */

#include <pthread.h>
#include <dce/sec_login.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_commonSecurity.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/common_data/itl_commonSecurity.c,v 1.1.117.1 1996/10/17 18:25:27 damon Exp $")

PRIVATE sec_login_handle_t currentLoginContext = (sec_login_handle_t) 0;

EXPORT void itlCommon_SecurityInit()
{
    /* do nothing for now */
}

/*
 * itlCommon_SecuritySetContext() --
 *    Sets the current login context.
 *
 * RETURN CODES: 
 *    FALSE - Successful addition.
 *    TRUE  - Unsuccessful addition, principal name exists or is invalid.
 */
EXPORT boolean_t itlCommon_SecuritySetContext(nameP, passwordP)
  IN char *nameP;       /* principal's name */
  IN char *passwordP;   /* principal's password */
{
    boolean_t retVal = FALSE;
    sec_login_handle_t loginContext;
    boolean32 resetPassword;
    sec_login_auth_src_t authSource;
    sec_passwd_rec_t passwordData;
    error_status_t errorStatus;

    if (sec_login_setup_identity((unsigned_char_p_t) nameP,
				 sec_login_no_flags, &loginContext,
				 &errorStatus)) 
    {
	passwordData.key.key_type = sec_passwd_plain;
	passwordData.key.tagged_union.plain = (unsigned char *) passwordP;
	passwordData.pepper = NULL;
	passwordData.version_number = sec_passwd_c_version_none;

	if (sec_login_valid_and_cert_ident(loginContext, &passwordData,
					   &resetPassword, &authSource,
					   &errorStatus))
	{
	    if (authSource == sec_login_auth_src_local)
		itl_AdviseUser("Using local credentials.\n");
	    if (authSource != sec_login_auth_src_overridden) 
	    {
		sec_login_set_context(loginContext, &errorStatus);
		if (errorStatus == error_status_ok)
		{
		    sec_login_purge_context(&currentLoginContext, &
					    errorStatus);
		    currentLoginContext = loginContext;
		    retVal = TRUE;
		}
		else
		    itlu_DCEError(errorStatus, "Could NOT set login context");
	    }
	    else
		itl_Error("Could NOT obtain network credentials.\n");
	}
	else
	    itlu_DCEError(errorStatus, "Login context validation error");

	if (!retVal)
	    sec_login_purge_context(&loginContext, &errorStatus);
    }
    else
	itlu_DCEError(errorStatus, "Network identity setup error");
    
    return(retVal);
}

EXPORT boolean_t itlCommon_SecurityGetContext(contextP)
  OUT sec_login_handle_t *contextP;
{
    error_status_t errorCode;
    boolean_t retVal = TRUE;

    sec_login_get_current_context(contextP, &errorCode);
    if (errorCode != 0)
	retVal = FALSE;

    return(retVal);
}


