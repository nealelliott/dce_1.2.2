
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
 * $Log: binding_create.c,v $
 * Revision 1.1.4.2  1996/02/18  00:29:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:16:15  marty]
 *
 * Revision 1.1.4.1  1995/12/11  15:17:54  root
 * 	Submit
 * 	[1995/12/11  14:32:59  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  23:18:10  root]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:33  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:15  melman]
 * 
 * Revision 1.1.2.3  1994/07/20  19:57:37  ganni
 * 	introduce the tests for srvrconf and srvrexec services
 * 	[1994/07/20  19:33:30  ganni]
 * 
 * Revision 1.1.2.2  1994/06/17  18:44:42  devsrc
 * 	     cr10872 - fix copyright
 * 	     [1994/06/17  18:32:10  devsrc]
 * 	Revision 1.1.2.1  1994/05/18  21:21:11  ganni
 * 	Initial Version
 * 	[1994/05/18  21:11:12  ganni]
 * 
 * $EndLog$
 */

/*
** Module: 	binding_create.c
**
** FACILITY: 	DCE Host Daemon API
**
** ABSTRACT:	This module implements the tests for dced api 
**		dced_binding_create 
**
*/

#include <dtapi_impl.h>

/*
 *  Global Declarations.
 */
dced_binding_handle_t   binding_handle;

void dced_binding_create_1(void);		/* Test purposes 1 */
void dced_binding_create_2(void);		/* Test purposes 2 */
void dced_binding_create_3(void);		/* Test purposes 3 */
void dced_binding_create_4(void);		/* Test purposes 4 */
void dced_binding_create_5(void);		/* Test purposes 5 */
void dced_binding_create_6(void);		/* Test purposes 6 */
void dced_binding_create_7(void);		/* Test purposes 7 */
void dced_binding_create_8(void);		/* Test purposes 8 */
void dced_binding_create_9(void);		/* Test purposes 9 */
void dced_binding_create_10(void);		/* Test purposes 10 */
void dced_binding_create_11(void);		/* Test purposes 11 */
void dced_binding_create_12(void);		/* Test purposes 12 */
void dced_binding_create_13(void);		/* Test purposes 13 */
void dced_binding_create_14(void);		/* Test purposes 14 */
void dced_binding_create_15(void);		/* Test purposes 15 */
void dced_binding_create_16(void);		/* Test purposes 16 */
void dced_binding_create_17(void);		/* Test purposes 17 */
void dced_binding_create_18(void);		/* Test purposes 18 */
void dced_binding_create_19(void);		/* Test purposes 19 */
void dced_binding_create_20(void);		/* Test purposes 20 */

void (*tet_startup)(void) = NULL;	/* Load the start routine into TET */
void (*tet_cleanup)(void) = NULL;	/* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
    { dced_binding_create_1,  1 },
    { dced_binding_create_2,  2 },
    { dced_binding_create_3,  3 },
    { dced_binding_create_4,  4 },
    { dced_binding_create_5,  5 },
    { dced_binding_create_6,  6 },
    { dced_binding_create_7,  7 },
    { dced_binding_create_8,  8 },
    { dced_binding_create_9,  9 },
    { dced_binding_create_10, 10 },
    { dced_binding_create_11, 11 },
    { dced_binding_create_12, 12 },
    { dced_binding_create_13, 13 },
    { dced_binding_create_14, 14 },
    { dced_binding_create_15, 15 },
    { dced_binding_create_16, 16 },
    { dced_binding_create_17, 17 },
    { dced_binding_create_18, 18 },
    { dced_binding_create_19, 19 },
    { dced_binding_create_20, 20 },
    { NULL, 0 }
};


/*
** ROUTINE NAME:
**		dced_binding_create_1()
**
** DESCRIPTION:
**		This routine verifies Assertion 01(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_1()
{
	error_status_t	st;
	unsigned32	flags = 0;

	tet_infoline("dced_binding_create_1, Assertion 01(A):");
	tet_infoline(
	"When srvrconf service name (format: service) is supplied");
	tet_infoline(
	"to dced_binding_create, a dced binding handle for");
	tet_infoline(
	"dced srvrconf service in the local host is returned.");

        dced_binding_create( SRVRCONF_SVC, flags, &binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_2()
**
** DESCRIPTION:
**		This routine verifies Assertion 02(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_2()
{
	error_status_t	st;
	unsigned32	flags = 0;
	char		*service_name;

	tet_infoline("dced_binding_create_2, Assertion 02(A):");
	tet_infoline(
	"When srvrconf service name [format: service@host] is supplied");
	tet_infoline(
	"to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"srvrconf service at specified host in local cell is returned.");


	test_format_2_name( SRVRCONF_SVC, &service_name);
        dced_binding_create(	(dced_string_t) service_name, 
				flags, 
				&binding_handle, &st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_3()
**
** DESCRIPTION:
**		This routine verifies Assertion 03(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_3()
{
	error_status_t	st;
	unsigned32	flags = 0;
	char		*service_name;

	tet_infoline("dced_binding_create_3, Assertion 03(A):");
	tet_infoline(
	"When srvrconf service name [format: /.:/host/config/service]");
	tet_infoline(
	"is supplied to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"srvrconf service at specified host in local cell is returned.");


	test_format_3_name( SRVRCONF_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
                                &binding_handle,
				&st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_4()
**
** DESCRIPTION:
**		This routine verifies Assertion 04(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_4()
{
	error_status_t	st;
	unsigned32	flags = 0;
	char		*service_name;

	tet_infoline("dced_binding_create_4, Assertion 04(A):");
	tet_infoline(
	"When srvrconf service [format: /.../cellname/host/config/service]");
	tet_infoline(
	"is supplied to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"srvrconf service at specified host in specified cell is returned.");


	test_format_4_name( SRVRCONF_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
				&binding_handle, 
				&st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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


/*
** ROUTINE NAME:
**		dced_binding_create_5()
**
** DESCRIPTION:
**		This routine verifies Assertion 05(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_5()
{
        error_status_t          st;
        unsigned32              flags = 0;

	tet_infoline("dced_binding_create_5, Assertion 05(A):");
	tet_infoline(
	"When srvrexec service name (format: service) is supplied");
	tet_infoline(
	"to dced_binding_create, a dced binding handle for");
	tet_infoline(
	"dced srvrexec service in the local host is returned.");

        dced_binding_create( SRVREXEC_SVC, flags, &binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_6()
**
** DESCRIPTION:
**		This routine verifies Assertion 06(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_6()
{
        error_status_t          st;
        unsigned32              flags = 0;
        char                    *service_name;

	tet_infoline("dced_binding_create_6, Assertion 06(A):");
	tet_infoline(
	"When srvrexec service name [format: service@host] is supplied");
	tet_infoline(
	"to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"srvrexec service at specified host in local cell is returned.");


	test_format_2_name( SRVREXEC_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
                                &binding_handle,
                                &st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_7()
**
** DESCRIPTION:
**		This routine verifies Assertion 07(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_7()
{

        error_status_t          st;
        unsigned32              flags = 0;
        char                    *service_name;

	tet_infoline("dced_binding_create_7, Assertion 07(A):");
	tet_infoline(
	"When srvrexec service name [format: /.:/host/config/service]");
	tet_infoline(
	"is supplied to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"srvrexec service at specified host in local cell is returned.");



	test_format_3_name( SRVREXEC_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
                                &binding_handle,
                                &st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_8()
**
** DESCRIPTION:
**		This routine verifies Assertion 08(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_8()
{
        error_status_t          st;
        unsigned32              flags = 0;
        char                    *service_name;

	tet_infoline("dced_binding_create_8, Assertion 08(A):");
	tet_infoline(
	"When srvrexec service [format: /.../cellname/host/config/service]");
	tet_infoline(
	"is supplied to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"srvrexec service at specified host in specified cell is returned.");


	test_format_4_name( SRVREXEC_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
                                &binding_handle,
                                &st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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


/*
** ROUTINE NAME:
**		dced_binding_create_9()
**
** DESCRIPTION:
**		This routine verifies Assertion 09(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_9()
{
        error_status_t          st;
        unsigned32              flags = 0;

	tet_infoline("dced_binding_create_9, Assertion 09(A):");
	tet_infoline(
	"When hostdata service name (format: service) is supplied");
	tet_infoline(
	"to dced_binding_create, a dced binding handle for");
	tet_infoline(
	"dced hostdata service in the local host is returned.");

        dced_binding_create( HOST_SVC, flags, &binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_10()
**
** DESCRIPTION:
**		This routine verifies Assertion 10(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_10()
{
        error_status_t          st;
        unsigned32              flags = 0;
	char 			*service_name;

	tet_infoline("dced_binding_create_10, Assertion 10(A):");
	tet_infoline(
	"When hostdata service name [format: service@host] is supplied");
	tet_infoline(
	"to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"hostdata service at specified host in local cell is returned.");

	test_format_2_name( HOST_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
                                &binding_handle,
                                &st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_11()
**
** DESCRIPTION:
**		This routine verifies Assertion 11(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_11()
{
        error_status_t          st;
        unsigned32              flags = 0;
        char                    *service_name;

	tet_infoline("dced_binding_create_11, Assertion 11(A):");
	tet_infoline(
	"When hostdata service name [format: /.:/host/config/service]");
	tet_infoline(
	"is supplied to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"hostdata service at specified host in local cell is returned.");

	test_format_3_name( HOST_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
                                &binding_handle,
                                &st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_12()
**
** DESCRIPTION:
**		This routine verifies Assertion 12(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_12()
{
        error_status_t          st;
        unsigned32              flags = 0;
        char                    *service_name;

	tet_infoline("dced_binding_create_12, Assertion 12(A):");
	tet_infoline(
	"When hostdata service [format: /.../cellname/host/config/service]");
	tet_infoline(
	"is supplied to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"hostdata service at specified host in specified cell is returned.");

	test_format_4_name( HOST_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
                                &binding_handle,
                                &st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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


/*
** ROUTINE NAME:
**		dced_binding_create_13()
**
** DESCRIPTION:
**		This routine verifies Assertion 13(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_13()
{
        error_status_t          st;
        unsigned32              flags = 0;

	tet_infoline("dced_binding_create_13, Assertion 13(A):");
	tet_infoline(
	"When keytab service name (format: service) is supplied");
	tet_infoline(
	"to dced_binding_create, a dced binding handle for");
	tet_infoline(
	"dced keytab service in the local host is returned.");

        dced_binding_create( KEYTAB_SVC, flags, &binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_14()
**
** DESCRIPTION:
**		This routine verifies Assertion 14(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_14()
{
        error_status_t          st;
        unsigned32              flags = 0;
        char                    *service_name;

	tet_infoline("dced_binding_create_14, Assertion 14(A):");
	tet_infoline(
	"When keytab service name [format: service@host] is supplied");
	tet_infoline(
	"to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"keytab service at specified host in local cell is returned.");

	test_format_2_name( KEYTAB_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
                                &binding_handle,
                                &st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_15()
**
** DESCRIPTION:
**		This routine verifies Assertion 15(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_15()
{
        error_status_t          st;
        unsigned32              flags = 0;
        char                    *service_name;

	tet_infoline("dced_binding_create_15, Assertion 15(A):");
	tet_infoline(
	"When keytab service name [format: /.:/host/config/service]");
	tet_infoline(
	"is supplied to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"keytab service at specified host in local cell is returned.");

	test_format_3_name( KEYTAB_SVC, &service_name);
        dced_binding_create( 	(dced_string_t) service_name,
				flags, 
				&binding_handle, 
				&st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_16()
**
** DESCRIPTION:
**		This routine verifies Assertion 16(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_16()
{
        error_status_t          st;
        unsigned32              flags = 0;
        char                    *service_name;

	tet_infoline("dced_binding_create_16, Assertion 16(A):");
	tet_infoline(
	"When keytab service [format: /.../cellname/host/config/service]");
	tet_infoline(
	"is supplied to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"keytab service at specified host in specified cell is returned.");

	test_format_4_name( KEYTAB_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
                                &binding_handle,
                                &st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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


/*
** ROUTINE NAME:
**		dced_binding_create_17()
**
** DESCRIPTION:
**		This routine verifies Assertion 17(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_17()
{
        error_status_t          st;
        unsigned32              flags = 0;

	tet_infoline("dced_binding_create_17, Assertion 17(A):");
	tet_infoline(
	"When secval service name (format: service) is supplied");
	tet_infoline(
	"to dced_binding_create, a dced binding handle for");
	tet_infoline(
	"dced secval service in the local host is returned.");

        dced_binding_create( SECVAL_SVC, flags, &binding_handle, &st);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_18()
**
** DESCRIPTION:
**		This routine verifies Assertion 18(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_18()
{
        error_status_t          st;
        unsigned32              flags = 0;
        char                    *service_name;

	tet_infoline("dced_binding_create_18, Assertion 18(A):");
	tet_infoline(
	"When secval service name [format: service@host] is supplied");
	tet_infoline(
	"to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"secval service at specified host in local cell is returned.");

	test_format_2_name( SECVAL_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
                                &binding_handle,
                                &st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_19()
**
** DESCRIPTION:
**		This routine verifies Assertion 19(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_19()
{
        error_status_t          st;
        unsigned32              flags = 0;
        char                    *service_name;

	tet_infoline("dced_binding_create_19, Assertion 19(A):");
	tet_infoline(
	"When secval service name [format: /.:/host/config/service]");
	tet_infoline(
	"is supplied to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"secval service at specified host in local cell is returned.");

	test_format_3_name( SECVAL_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
                                &binding_handle,
                                &st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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

/*
** ROUTINE NAME:
**		dced_binding_create_20()
**
** DESCRIPTION:
**		This routine verifies Assertion 20(A) for the function
**		dced_binding_create(). 
**
*/
void 
dced_binding_create_20()
{
        error_status_t          st;
        unsigned32              flags = 0;
        char                    *service_name;

	tet_infoline("dced_binding_create_20, Assertion 20(A):");
	tet_infoline(
	"When secval service [format: /.../cellname/host/config/service]");
	tet_infoline(
	"is supplied to dced_binding_create, a dced binding handle for dced");
	tet_infoline(
	"secval service at specified host in specified cell is returned.");

	test_format_4_name( SECVAL_SVC, &service_name);
        dced_binding_create(    (dced_string_t) service_name,
                                flags, 
                                &binding_handle,
                                &st);
	free(service_name);
        if (st != error_status_ok)
        {
#ifdef DEBUG
                tet_infoline("dced_binding_create failed:");
                dced_util_printerr(st);
#endif
		tet_result(TET_FAIL);
                return;
        }

        dced_binding_free( binding_handle, &st);
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


