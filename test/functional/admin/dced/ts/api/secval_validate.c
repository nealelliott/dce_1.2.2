
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: secval_validate.c,v $
 * Revision 1.1.4.2  1996/02/18  00:31:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:04  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:19:39  root
 * 	Submit
 * 	[1995/12/11  14:33:28  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:48:16  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:54  melman]
 * 
 * Revision 1.1.2.4  1994/09/09  19:06:55  ganni
 * 	verify if secval is activated
 * 	[1994/09/09  19:06:19  ganni]
 * 
 * Revision 1.1.2.3  1994/06/17  18:45:19  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:34:15  devsrc]
 * 
 * Revision 1.1.2.2  1994/06/03  14:05:39  ganni
 * 	create the principal needed for the tests through scripts
 * 	[1994/06/03  14:03:19  ganni]
 * 
 * Revision 1.1.2.1  1994/05/18  21:22:10  ganni
 * 	Initial Version
 * 	[1994/05/18  21:12:42  ganni]
 * 
 * $EndLog$
 */

/*
** Module:	secval_validate.c
**
** FACILITY:	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_secval_validate 
**
*/

#include <dtapi_impl.h>
#include <dce/sec_login.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;
boolean32		sv_active=FALSE;

void setup(void);
void cleanup(void);
void test_free( /* [out] */  error_status_t *st);
void dced_secval_validate_1(void);          /* Test purposes 1 */

void (*tet_startup)(void) = setup;
void (*tet_cleanup)(void) = cleanup;

struct tet_testlist tet_testlist[] =
{
    { dced_secval_validate_1,  1 },
    { NULL, 0 }
};

void
setup()
{
	char		add_princ_cmd[250];
	error_status_t	st;

        sprintf( add_princ_cmd, 
		 "dce_login %s %s -exec ./secval_setup.sh %s", 
		 CELL_ADMIN, CELL_ADMIN_PWD, CELL_ADMIN_PWD);
	st = system ( add_princ_cmd );
	tet_infoline ( "add_principals: Done " );

	dced_util_binding_create( SECVAL_SVC, &binding_handle, &st);
}

/*
** ROUTINE NAME:
**                    cleanup()
**
** DESCRIPTION:
**                    This routine performs necessary cleanup for all tests.
**
*/
void cleanup()
{
        error_status_t          st;

	dced_binding_free( binding_handle, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_binding_free failed:");
        	dced_util_printerr(st);
#endif
		return;
	}
}

/*
** ROUTINE NAME:
**                    test_init()
**
** DESCRIPTION:
**                    This routine performs api specific initialization.
**
*/
static void test_init(
        /* [out] */     error_status_t          *st
)
{

	unsigned_char_p_t     dced_test_princ;
	sec_login_handle_t    context;
	sec_login_flags_t     flags;
	sec_passwd_rec_t      passwd;
	boolean32             reset_passwd;
	sec_login_auth_src_t  auth_src;
	error_status_t	      tmp_st;
        unsigned char         *raw_pw;
	int 		      len;

        dced_secval_status( binding_handle, &sv_active, st);
        if (!sv_active)
	{
                dced_secval_start( binding_handle, st);
        	if (*st != error_status_ok)
        	{
#ifdef DEBUG
                    tet_infoline("dced_secval_start failed:");
                    dced_util_printerr(*st);
#endif
                    return;
        	}
	}

	flags = sec_login_no_flags;
	dced_test_princ = (unsigned_char_t *) DCED_TEST_PRINC;

	passwd.key.key_type = sec_passwd_plain;
	passwd.pepper = NULL;
	passwd.version_number = sec_passwd_c_version_none;

	len = strlen((char *)DCED_TEST_PASSWD)+1;
	raw_pw=(unsigned char *)malloc(len);
	memcpy (raw_pw, DCED_TEST_PASSWD, len);
	raw_pw[len] = '\0';
	passwd.key.tagged_union.plain= raw_pw;

	if (!(sec_login_setup_identity(dced_test_princ, flags, &context, st)))
        {
#ifdef DEBUG
                tet_infoline("sec_login_setup_identity failed:");
                dced_util_printerr(*st);
#endif
		dced_secval_stop( binding_handle, &tmp_st);
		return;
        }

	if (!( sec_login_validate_identity(context,
                                &passwd, 
                                &reset_passwd,
                                &auth_src,
                                st )))
        {
#ifdef DEBUG
                tet_infoline("sec_login_validate_identity failed:");
                dced_util_printerr(*st);
#endif
		dced_secval_stop( binding_handle, &tmp_st);
		return;
        }

	return;
}

/*
** ROUTINE NAME:
**                    test_free()
**
** DESCRIPTION:
**                    This routine performs necessary cleanup for all tests.
**
*/
void test_free(
        /* [out] */     error_status_t          *st
)
{

	if (!sv_active)
	{
		dced_secval_stop( binding_handle, st);
		if (*st != error_status_ok)
		{
#ifdef DEBUG
		    tet_infoline("dced_secval_stop failed:");
       		    dced_util_printerr(*st);
#endif
		}
	}
}

/*
** ROUTINE NAME:
**		dced_secval_validate_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_secval_validate(). 
**
*/
void 
dced_secval_validate_1()
{
	error_status_t		st;

	tet_infoline("dced_secval_validate_1, Assertion 01(A):");
	tet_infoline(
	"After the login context is setup and validated ");
	tet_infoline(
	"by a security server, decd_secval_validate API");
	tet_infoline(
	"returns with the status error_status_ok,");
	tet_infoline(
	"upon supplying a secval dced binding handle.");

	test_init(&st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("test_init failed:");
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

	dced_secval_validate( binding_handle, &st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_secval_validate failed:");
       		dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		test_free(&st);
		return;
	}

	test_free(&st);
	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("test_free failed:");
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

	tet_result(TET_PASS);
	return;

}

