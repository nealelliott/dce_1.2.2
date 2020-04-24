/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: utc_gettime_noshm.c,v $
 * Revision 1.1.2.2  1996/03/09  20:52:49  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:30:19  marty]
 *
 * Revision 1.1.2.1  1995/12/11  23:05:43  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/15  14:22 UTC  pare
 * 	Merge files
 * 
 * 	HP revision /main/pare_mothra/1  1995/06/15  12:32 UTC  pare
 * 	Create new test for utc_gettime_noshm() function
 * 	[1995/12/11  22:37:12  root]
 * 
 * $EndLog$
 */
#include <dce/utc.h>
#include <stdlib.h>
#include <tet_api.h>

void (*tet_startup)() = NULL;
void (*tet_cleanup)() = NULL;
void test_utc_gettime_noshm();

struct tet_testlist tet_testlist[] =
   {{test_utc_gettime_noshm, 1},
    {NULL, NULL}};

/*
 * This routine compares the time field from two utc_t's and print an
 * error if they are different
 *
 * Input:
 *	utc1	1st utc_t
 *	utc2	2nd utc_t
 *	tag	string indicating which test failed
 *
 */
int compare_time( utc1, utc2, tag )
utc_t *utc1;
utc_t *utc2;
char  *tag;
{
    timespec_t t1, t2;
    char tmpstr[100];

    if (utc_bintime(&t1, (timespec_t *)0, (long *)0, utc1) < 0) {
	sprintf(tmpstr,"1st utc_bintime returned error in %s time compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}

    if (utc_bintime(&t2, (timespec_t *)0, (long *)0, utc2) < 0) {
	sprintf(tmpstr,"2nd utc_bintime returned error in %s time compare\n", tag);
	tet_infoline(tmpstr);
	return(0);
	}
    return(1);
}

void test_utc_gettime_noshm()
{
    utc_t	utc, utc_noshm;
    int		i, ok;

    ok = 1;

    for (i = 0; i < 250; i++)
    {
        if (utc_gettime(&utc) < 0) {
	    tet_infoline("utc_gettime returned error\n");
            ok = 0;
        }

        if (utc_gettime_noshm(&utc_noshm) < 0) {
	    tet_infoline("utc_gettime_noshm returned error\n");
            ok = 0;
        }

        if (compare_time(&utc, &utc_noshm, "utc_gettime, utc_gettime_noshm") == 0)
            ok = 0;
    }

    if (ok) {
        printf("PASSED, Test 1 'TSET_UTC_GETTIME_NOSHM' : test ran successfully\n");
	tet_result(TET_PASS);
    }
    else {
        printf("FAILED, Test 1 'TEST_UTC_GETTIME_NOSHM' : test ran unsuccessfully\n");
	tet_result(TET_FAIL);
    }
}

