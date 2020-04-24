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
 * itl_fxPrincipal.c -- 
 *    C functions for implementing the principal (user) hash table.
 * 
 */
/*
 * HISTORY
 * $Log: itl_fxPrincipal.c,v $
 * Revision 1.1.11.1  1996/10/17  18:26:37  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:23  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:44:41  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:12  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:30  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:49  annie]
 * 
 * Revision 1.1.2.2  1993/01/13  21:13:18  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	[1993/01/13  14:33:50  shl]
 * 
 * $EndLog$
 */

#include <pthread.h>
#include <dce/sec_login.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_utilsNameHash.h>
#include <utils/itl/itl_fxDCEError.h>
#include <utils/itl/itl_fxPrincipal.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxPrincipal.c,v 1.1.11.1 1996/10/17 18:26:37 damon Exp $")

PRIVATE unsigned int principalHashType;


/*
 *  PrincipalInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void PrincipalInit()
{
    /*
     *  We can safely set principalHashType because this function is called
     *  via pthread_once.
     */
    principalHashType = itlu_NameHashGetType();
}

/*
 * itlFX_PrincipalInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlFX_PrincipalInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    /*
     *  First ensure the hash table module is initialized.
     */
    itlu_NameHashInit();

    if (pthread_once(&hashOnce, (pthread_initroutine_t) PrincipalInit))
	itl_Error("pthread_once failed in PrincipalInit\n");
}

/*
 * itlFX_PrincipalAddName() --
 *    Adds a principal name and login context to the table.
 *
 * RETURN CODES: 
 *    FALSE - Successful addition.
 *    TRUE  - Unsuccessful addition, principal name exists or is invalid.
 */
SHARED boolean_t itlFX_PrincipalAddName(nameP, passwordP)
  IN char *nameP;       /* principal's name */
  IN char *passwordP;   /* principal's password */
{
    boolean_t retVal = TRUE;
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
		    itlFX_principal_t *entryP;

		    /*
		     *  Allocate memory to hold data, and initialize it.
		     */
		    ITL_UTIL_NEW(entryP, itlFX_principal_t);
		    entryP->loginContext = loginContext;

		    if (retVal = itlu_NameHashAdd(nameP, principalHashType,
						  (void *) entryP))
		    {
			ITL_UTIL_FREE(entryP);
		    }
		}
		else
		    ITL_FX_DCE_ERROR(errorStatus,
				     "Could NOT set login context");
	    }
	    else
		itl_Error("Could NOT obtain network credentials.\n");
	}
	else
	    ITL_FX_DCE_ERROR(errorStatus, 
			     "Login context validation error");
    }
    else
	ITL_FX_DCE_ERROR(errorStatus, "Network identity setup error");
    
    if (retVal)
	sec_login_purge_context(&loginContext, &errorStatus);

    return(retVal);
}

/*
 * itlFX_PrincipalRemoveName() --
 *    Delete a principal name from the table.
 *
 * RETURN CODES: void
 */
SHARED void itlFX_PrincipalRemoveName(nameP)
  IN char *nameP;   /* name of principal to remove */
{
    itlFX_principal_t *entryP;

    if ((entryP = (itlFX_principal_t *) itlu_NameHashDelete(nameP, 
							    principalHashType))
	       != (itlFX_principal_t *) 0)
	ITL_FREE(entryP);
    else 
	itl_Message("\nWARNING: Principal, %s, NOT in table.\n\n", nameP);
}

/*
 * itlFX_PrincipalLookup() -- 
 *    Given a principal name, return a pointer to the itlFX_principal_t.
 *
 * RETURN CODES: 
 *    NULL (0) - if no entry is found.
 *    Otherwise success.
 */
SHARED itlFX_principal_t *itlFX_PrincipalLookup(nameP)
  IN char *nameP;                /* the principal name to compare with */
{
    itlu_NameHashTable_t *entryP;
    itlFX_principal_t *retVal = (itlFX_principal_t *) 0;

    if ((entryP = itlu_NameHashLookup(nameP, principalHashType))
	       != ITLU_NAME_HASH_NULL)
    {
	retVal = ITLU_NAME_HASH_GET_DATA(entryP, itlFX_principal_t);
	itlu_NameHashRelease(entryP);
    }

    return(retVal);
}



