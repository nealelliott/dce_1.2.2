
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
 * $Log: secval_stop.c,v $
 * Revision 1.1.4.2  1996/02/18  00:31:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:02  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:19:35  root
 * 	Submit
 * 	[1995/12/11  14:33:27  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:48:15  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:53  melman]
 * 
 * Revision 1.1.2.3  1994/09/09  19:06:53  ganni
 * 	verify if secval is activated
 * 	[1994/09/09  19:06:18  ganni]
 * 
 * Revision 1.1.2.2  1994/06/17  18:45:17  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:34:11  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/18  21:22:08  ganni
 * 	Initial Version
 * 	[1994/05/18  21:12:37  ganni]
 * 
 * $EndLog$
 */

/*
** Module:	secval_stop.c
**
** FACILITY:	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_secval_stop 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
void dced_secval_stop_1(void);          /* Test purposes 1 */
void dced_secval_stop_2(void);          /* Test purposes 2 */

void (*tet_startup)(void) = NULL;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = NULL;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_secval_stop_1,  1 },
    { dced_secval_stop_2,  2 },
    { NULL, 0 }
};

/*
** ROUTINE NAME:
**		dced_secval_stop_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_secval_stop().
**
*/
void 
dced_secval_stop_1()
{
	dced_binding_handle_t	binding_handle;
	error_status_t		st, sec_st;
        boolean32               sv_active= FALSE;

	tet_infoline("dced_secval_stop_1, Assertion 01(A):");
	tet_infoline(
	"When secval dced binding handle is supplied to dced_secval_stop,");
	tet_infoline(
	"the dced_secval service will be stopped and the status is ");
	tet_infoline(
	"set to error_status_ok.");

	dced_util_binding_create( SECVAL_SVC, &binding_handle, &st);
	if (st != error_status_ok)
	{
                tet_result(TET_UNRESOLVED);
		return;
	}
        dced_secval_status( binding_handle, &sv_active, &sec_st);
        if (!sv_active)
	{
                dced_secval_start( binding_handle, &sec_st);
		if (sec_st != error_status_ok)
		{
#ifdef DEBUG
		    tet_infoline("dced_secval_start failed:");
        	    dced_util_printerr(sec_st);
#endif
		    dced_binding_free( binding_handle, &st);
		    tet_result(TET_UNRESOLVED);
		    return;
		}
	}

	dced_secval_stop( binding_handle, &sec_st);
	if (sec_st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_secval_stop failed:");
       		dced_util_printerr(sec_st);
#endif
		tet_result(TET_FAIL);
	}
	else
		tet_result(TET_PASS);

	if (sv_active)
		dced_secval_start( binding_handle, &st);
	dced_binding_free( binding_handle, &st);
	return;

}


/*
** ROUTINE NAME:
**		dced_secval_stop_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_secval_stop().
**
*/
void 
dced_secval_stop_2()
{
	dced_binding_handle_t	binding_handle;
	error_status_t		st, sec_st;

	tet_infoline("dced_secval_stop_2, Assertion 02(A):");
	tet_infoline(
	"When hostdata dced binding handle is supplied to dced_secval_stop,");
	tet_infoline(
	"the status dced_s_bad_binding is returned.");

	dced_util_binding_create( HOST_SVC, &binding_handle, &st);
	if (st != error_status_ok)
	{
                tet_result(TET_UNRESOLVED);
		return;
	}

	dced_secval_stop( binding_handle, &sec_st);
	if (sec_st != dced_s_bad_binding)
	{
#ifdef DEBUG
		tet_infoline("dced_secval_stop failed:");
        	dced_util_printerr(sec_st);
#endif
		tet_result(TET_FAIL);
	}

	dced_binding_free( binding_handle, &st);

	if (sec_st != dced_s_bad_binding)
		return;

	if (st != error_status_ok)
	{
#ifdef DEBUG
		tet_infoline("dced_binding_free failed:");
        	dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
	}

	tet_result(TET_PASS);
	return;
}
