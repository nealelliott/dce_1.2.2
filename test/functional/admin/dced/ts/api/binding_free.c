
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
 * $Log: binding_free.c,v $
 * Revision 1.1.4.2  1996/02/18  00:30:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:19  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:17:59  root
 * 	Submit
 * 	[1995/12/11  14:33:01  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  23:18:13  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:36  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:18  melman]
 * 
 * Revision 1.1.2.3  1994/07/20  19:57:40  ganni
 * 	introduce the tests for srvrconf and srvrexec services
 * 	[1994/07/20  19:35:42  ganni]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:44  devsrc
 * 	     cr10872 - fix copyright
 * 	     [1994/06/17  18:32:19  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:17  ganni
 * 	Initial Version
 * 	[1994/05/18  21:11:19  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	binding_free.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_binding_free 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;

void dced_binding_free_1(void);		/* Test purposes 1 */
void dced_binding_free_2(void);		/* Test purposes 2 */
void dced_binding_free_3(void);		/* Test purposes 3 */
void dced_binding_free_4(void);		/* Test purposes 4 */
void dced_binding_free_5(void);		/* Test purposes 5 */

void (*tet_startup)(void) = NULL;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = NULL;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_binding_free_1,  1 },
    { dced_binding_free_2,  2 },
    { dced_binding_free_3,  3 },
    { dced_binding_free_4,  4 },
    { dced_binding_free_5,  5 },
    { NULL, 0 }
};

/*
** ROUTINE NAME:
**		dced_binding_free_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_binding_free(). 
**
*/
void 
dced_binding_free_1()
{
        error_status_t          st;
        unsigned32              flags = 0;

	tet_infoline("dced_binding_free_1, Assertion 01(A):");
	tet_infoline(
	"When a dced binding handle for srvrconf service is supplied");
	tet_infoline(
	"to dced_binding_free, the binding is released and");
	tet_infoline(
	"the status is set to error_status_ok.");

        dced_binding_create( SRVRCONF_SVC, flags, &binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
        }

        dced_binding_free( binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		return;
        }

	tet_result(TET_PASS);
	return;

}

/*
** ROUTINE NAME:
**		dced_binding_free_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_binding_free(). 
**
*/
void 
dced_binding_free_2()
{
        error_status_t          st;
        unsigned32              flags = 0;

	tet_infoline("dced_binding_free_2, Assertion 02(A):");
	tet_infoline(
	"When a dced binding handle for srvrexec service is supplied");
	tet_infoline(
	"to dced_binding_free, the binding is released and");
	tet_infoline(
	"the status is set to error_status_ok.");

        dced_binding_create( SRVREXEC_SVC, flags, &binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
        }

        dced_binding_free( binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		return;
        }

	tet_result(TET_PASS);
	return;

}

/*
** ROUTINE NAME:
**		dced_binding_free_3()
**
** DESCRIPTION:
**		This routine verifies Assertion 03(A) for the function
**		dced_binding_free(). 
**
*/
void 
dced_binding_free_3()
{
        error_status_t          st;
        unsigned32              flags = 0;

	tet_infoline("dced_binding_free_3, Assertion 03(A):");
	tet_infoline(
	"When a dced binding handle for hostdata service is supplied");
	tet_infoline(
	"to dced_binding_free, the binding is released and");
	tet_infoline(
	"the status is set to error_status_ok.");

        dced_binding_create( HOST_SVC, flags, &binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
        }

        dced_binding_free( binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		return;
        }

	tet_result(TET_PASS);
	return;

}

/*
** ROUTINE NAME:
**		dced_binding_free_4()
**
** DESCRIPTION:
**		This routine verifies Assertion 04(A) for the function
**		dced_binding_free(). 
**
*/
void 
dced_binding_free_4()
{
        error_status_t          st;
        unsigned32              flags = 0;

	tet_infoline("dced_binding_free_4, Assertion 04(A):");
	tet_infoline(
	"When a dced binding handle for keytab service is supplied");
	tet_infoline(
	"to dced_binding_free, the binding is released and");
	tet_infoline(
	"the status is set to error_status_ok.");

        dced_binding_create( KEYTAB_SVC, flags, &binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
        }

        dced_binding_free( binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		return;
        }

	tet_result(TET_PASS);
	return;

}

/*
** ROUTINE NAME:
**		dced_binding_free_5()
**
** DESCRIPTION:
**		This routine verifies Assertion 05(A) for the function
**		dced_binding_free(). 
**
*/
void 
dced_binding_free_5()
{
        error_status_t          st;
        unsigned32              flags = 0;

	tet_infoline("dced_binding_free_5, Assertion 05(A):");
	tet_infoline(
	"When a dced binding handle for secval service is supplied");
	tet_infoline(
	"to dced_binding_free, the binding is released and");
	tet_infoline(
	"the status is set to error_status_ok.");

        dced_binding_create( SECVAL_SVC, flags, &binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_UNRESOLVED);
		return;
        }

        dced_binding_free( binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_free failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
		return;
        }

	tet_result(TET_PASS);
	return;

}

