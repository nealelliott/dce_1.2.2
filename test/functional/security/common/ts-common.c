/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ts-common.c,v $
 * Revision 1.1.8.2  1996/07/09  15:00:08  arvind
 * 	Initialize some variables.
 * 	[1996/06/17  19:42 UTC  jrr  /main/DCE_1.2.2/jrr_122_1/1]
 *
 * Revision 1.1.8.1  1996/06/04  22:06:21  arvind
 * 	TETize KRB5 tests for DCE 1.2.2.
 * 	[1996/04/05  21:58 UTC  psn  /main/DCE_1.2/psn_dce1.2_krb5/1]
 * 
 * Revision 1.1.6.3  1996/02/17  23:11:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:12  marty]
 * 
 * Revision 1.1.6.2  1995/12/11  19:52:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:26:39  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:18:02  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:31:14  agd
 * 	expand copyright
 * 	[1994/10/06  14:28:31  agd]
 * 
 * Revision 1.1.2.1  1994/08/04  16:15:11  mdf
 * 	Initial checkin
 * 	[1994/08/03  19:00:52  mdf]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <varargs.h>

#include <dce/dce_error.h>
#include <dce/nbase.h>
#include <dce/rpcsts.h>

#include <ts-common.h>

/* Global variables */
unsigned char           unexpected[dce_c_error_string_len];
unsigned char           expected[dce_c_error_string_len];
int                     dce_status;
error_status_t          rgy_st;
int                     test_no;
int                     test_replica;
unsigned_char_t         *sec_replica_site_name = NULL;
unsigned_char_t         *sec_master_site_name = NULL;
char                    *sec_test_replication = NULL;
char                    *sec_test_migration = NULL;
int sec_migrate_vno = 9999; /* initial value must be greater than all other values */

boolean32 sec_migrate_check_exp_result(
        sec_api_name_t	api_name,
        error_status_t  *returned_st_p
);

/*
 * prints information to stream, and to the tet journal, this is needed
 * to meet the HP TET requirements as well as the OSF TET requirements.
 */
fmtet_infoline(stream, va_alist)
FILE *stream;
va_dcl
{
        va_list vargs;
        char msg[1024];                 /* buffer for info lines */
        char *fmt;

        va_start(vargs);
        fmt = va_arg(vargs, char *);
        (void) vsprintf(msg, fmt, vargs);
        va_end(vargs);
        fprintf (stream, msg);
        tet_infoline (msg);
}

/*
 * prints information to stdout, and to the tet journal, this is needed
 * to meet the HP TET requirements as well as the OSF TET requirements.
 */
mtet_infoline(va_alist)
va_dcl
{
        va_list vargs;
        char msg[1024];                 /* buffer for info lines */
        char *fmt;

        va_start(vargs);
        fmt = va_arg(vargs, char *);
        (void) vsprintf(msg, fmt, vargs);
        va_end(vargs);
        printf (msg);
        tet_infoline (msg);
}

boolean32 sec_migrate_check_exp_result(
	sec_api_name_t	api_name, 
        error_status_t 	*returned_st_p
)
{
    /* get version number from TET config file */
    if (!sec_test_migration) {
        sec_test_migration = (char *) tet_getvar("SEC_TEST_MIGRATION");
        sec_migrate_vno = atoi(sec_test_migration);
    }

    switch (sec_migrate_vno) {
        default:
        case 0: /* implies all APIs are supported */
	    return true;
            break;
        case 1: 
	    if (api_name < Msec_11_first) { /* these apis are supported */
                return true;
	    }
            break;
    }

    if ((*returned_st_p == ept_s_not_registered) ||
        (*returned_st_p == rpc_s_unknown_if) ||
        (*returned_st_p == sec_s_v1_1_no_support))
        return true;

    return false;
}
