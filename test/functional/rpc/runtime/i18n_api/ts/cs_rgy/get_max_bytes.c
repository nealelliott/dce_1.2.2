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
 * $Log: get_max_bytes.c,v $
 * Revision 1.1.4.2  1996/02/18  00:33:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:19:15  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:51:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:26:31  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:17:55  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:44:22  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:11  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/04  21:54:46  mori_m
 * 	CR 9701: RPC runtime I18N extension functional test drop.
 * 	[1994/05/04  21:21:34  mori_m]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <dce/rpc.h>
#include <dce/nbase.h>
#include <unistd.h>
#include <fcntl.h>
#include <csrc.h>
#include <dce/rpcsts.h>
#include <locale.h>
#include <tet_api.h>

#include "check_status.h"

static void get_max1(), get_max2(), get_max3(), get_max4(), get_max5();
static void startup();

/*
 * Initialize TCM data structures
 */

void (*tet_startup)() = NULL;
void (*tet_cleanup)() = NULL;
struct tet_testlist tet_testlist[] = {
        {get_max1, 1},	/*  rpc_rgy_get_max_bytes:  Normal case */
        {get_max2, 2},	/*  rpc_rgy_get_max_bytes:  Normal case */
        {get_max3, 3},	/*  rpc_rgy_get_max_bytes:  Not Found case */
        {get_max4, 4},	/*  rpc_rgy_get_max_bytes:  Unknown case */
        {get_max5, 5},	/*  rpc_rgy_get_codesets:   Normal case */
        {NULL, 0}
};

static void
get_max1()
{
	unsigned16		max_bytes;
	unsigned32		status;

	tet_infoline("test case (get_max1)");

	rpc_rgy_get_max_bytes (0xFFFCE9F1, &max_bytes, &status);

	CHECK_STATUS(TRUE, "rpc_rgy_get_max_bytes", status, rpc_s_ok);

	mtet_infoline("0xFFFCE9F1 is searched.  max_bytes = %d\n", max_bytes);

	tet_result(TET_PASS);
	return;
}

static void
get_max2()
{
	unsigned16              max_bytes;
	unsigned32              status;

	tet_infoline("test case (get_max2)");

	rpc_rgy_get_max_bytes (0x30011, &max_bytes, &status);

	CHECK_STATUS(TRUE, "rpc_rgy_get_max_bytes", status, rpc_s_ok);

	mtet_infoline("0x30011 is searched.  max_bytes = %d\n", max_bytes);

	tet_result(TET_PASS);
	return;
}

static void
get_max3()
{
	unsigned16              max_bytes;
	unsigned32              status;

	tet_infoline("test case (get_max3)");

	rpc_rgy_get_max_bytes (0x10020007, &max_bytes, &status);

	CHECK_STATUS(TRUE, "rpc_rgy_get_max_bytes", status, dce_cs_c_notfound);
	tet_result(TET_PASS);
	return;
}

static void
get_max4()
{
        unsigned16              max_bytes;
        unsigned32              status;

	tet_infoline("test case (get_max4)");

	rpc_rgy_get_max_bytes (0x10820307, &max_bytes, &status);

	CHECK_STATUS(TRUE, "rpc_rgy_get_max_bytes", status, dce_cs_c_unknown);
	tet_result(TET_PASS);
	return;
}

static void
get_max5()
{
	unsigned16              max_bytes;
	rpc_codeset_mgmt_p_t    codesets_p;
	int                     i;
	unsigned32              status;

	setlocale(LC_ALL, "");

	tet_infoline("test case (get_max5)");

	rpc_rgy_get_codesets(&codesets_p, &status);

	CHECK_STATUS(TRUE, "rpc_rgy_get_codesets", status, rpc_s_ok);

	mtet_infoline("\ncount = %d\n", codesets_p->count);

	for (i = 0; i < codesets_p->count; i++)
	{
	    mtet_infoline("codeset[%d].c_set = 0x%lX\n", i, codesets_p->codesets[i].c_set);
	    mtet_infoline("           .c_max_bytes = %d\n", codesets_p->codesets[i].c_max_bytes);
	}

	tet_result(TET_PASS);
	return;
}
