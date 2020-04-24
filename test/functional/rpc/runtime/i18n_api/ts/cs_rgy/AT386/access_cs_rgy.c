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
 * $Log: access_cs_rgy.c,v $
 * Revision 1.1.4.2  1996/02/18  00:33:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:19:10  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:51:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:26:25  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:17:48  root]
 * 
 * Revision 1.1.2.1  1994/06/22  19:59:25  mori_m
 * 	CR 10043:  Separated access_cs_rgy.c into machine dependent directory.
 * 	[1994/06/22  19:55:35  mori_m]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <dce/rpc.h>
#include <dce/nbase.h>
#include <csrc.h>
#include <unistd.h>
#include <fcntl.h>
#include <dce/rpcsts.h>
#include <tet_api.h>

#include "check_status.h"

static void cs_access1(), cs_access2(), cs_access3(), cs_access4(), cs_access5(), cs_access6();

/*
 * Initialize TCM data structures
 */

void (*tet_startup)() = NULL;
void (*tet_cleanup)() = NULL;
struct tet_testlist tet_testlist[] = {
	{cs_access1, 1},	/*  dce_cs_loc_to_rgy:  Normal case */
	{cs_access2, 2},	/*  dce_cs_rgy_to_loc:  Normal case */
	{cs_access3, 3},	/*  dce_cs_loc_to_rgy:  Normal case */
	{cs_access4, 4},	/*  dce_cs_rgy_to_loc:  Normal case */
	{cs_access5, 5},	/*  dce_cs_rgy_to_loc:  Not Found case */
	{cs_access6, 6},	/*  dce_cs_rgy_to_loc:  Unknown case */
	{NULL, 0}
};


static void
cs_access1()
{
	unsigned32	code_set;
	unsigned16	char_num;
	unsigned16	*char_sets;
	idl_char	*code_name;
	int		i;
	unsigned32	status;

	tet_infoline("test case (cs_access1)");

	dce_cs_loc_to_rgy ((idl_char *)"AJEC", &code_set, &char_num, &char_sets, &status);

	CHECK_STATUS(TRUE, "dce_cs_loc_to_rgy", status, dce_cs_c_ok);

	mtet_infoline("AJEC is searched.   code_set = 0x%lX\n", code_set);
	mtet_infoline("		   char_num = %d\n", char_num);

	for (i=0; i < char_num; i++)
	{
		mtet_infoline("		   charset[%d] = 0x%X\n", i, *char_sets++);
	}

	tet_result(TET_PASS);
	return;
}


static void
cs_access2()
{
	unsigned32	code_set;
	unsigned16	char_num;
	unsigned16	*char_sets;
	idl_char	*code_name;
	int		i;
	unsigned32	status;

	tet_infoline("test case (cs_access2)");

	dce_cs_rgy_to_loc (0xFFFCE9F1, &code_name, &char_num, &char_sets, &status);

	CHECK_STATUS(TRUE, "dce_cs_rgy_to_loc", status, dce_cs_c_ok);

	mtet_infoline("0xFFFCE9F1 is searched.  code_name = %s\n", code_name);
	mtet_infoline("		   char_num = %d\n", char_num);

	for (i=0; i < char_num; i++)
	{
		mtet_infoline("		   charset[%d] = 0x%X\n", i, *char_sets++);
	}

	tet_result(TET_PASS);
	return;
}


static void
cs_access3()
{
	unsigned32	code_set;
	unsigned16	char_num;
	unsigned16	*char_sets;
	idl_char	*code_name;
	int		i;
	unsigned32	status;

	tet_infoline("test case (cs_access3)");

	dce_cs_loc_to_rgy ((idl_char *)"ISO8859-1", &code_set, &char_num, &char_sets, &status);

	CHECK_STATUS(TRUE, "dce_cs_loc_to_rgy", status, dce_cs_c_ok);

	mtet_infoline("ISO8859-1 is searched.  code_set = 0x%lX\n", code_set);
	mtet_infoline("		   char_num = %d\n", char_num);

	for (i=0; i < char_num; i++)
	{
		mtet_infoline("		   charset[%d] = 0x%X\n", i, *char_sets++);
	}

	tet_result(TET_PASS);
	return;
}


static void
cs_access4()
{
	unsigned32	code_set;
	unsigned16	char_num;
	unsigned16	*char_sets;
	idl_char	*code_name;
	int		i;
	unsigned32	status;

	tet_infoline("test case (cs_access4)");

	dce_cs_rgy_to_loc (0x10001, &code_name, &char_num, &char_sets, &status);

	CHECK_STATUS(TRUE, "dce_cs_rgy_to_loc", status, dce_cs_c_ok);

	mtet_infoline("0x10001 is searched.  code_name = %s\n", code_name);
	mtet_infoline("		   char_num = %d\n", char_num);

	for (i=0; i < char_num; i++)
	{
		mtet_infoline("		   charset[%d] = 0x%X\n", i, *char_sets++);
	}

	tet_result(TET_PASS);
	return;
}

static void
cs_access5()
{
	unsigned32	code_set;
	unsigned16	char_num;
	unsigned16	*char_sets;
	idl_char	*code_name;
	int		i;
	unsigned32	status;

	tet_infoline("test case (cs_access5)");

	dce_cs_rgy_to_loc (0x10010801, &code_name, &char_num, &char_sets, &status);

	CHECK_STATUS(TRUE, "dce_cs_rgy_to_loc", status, dce_cs_c_notfound);

	tet_result(TET_PASS);
	return;
}

static void
cs_access6()
{
	unsigned32	code_set;
	unsigned16	char_num;
	unsigned16	*char_sets;
	idl_char	*code_name;
	int		i;
	unsigned32	status;

	tet_infoline("test case (cs_access6)");

	dce_cs_rgy_to_loc (0x97439, &code_name, &char_num, &char_sets, &status);
	CHECK_STATUS(TRUE, "dce_cs_rgy_to_loc", status, dce_cs_c_unknown);
	tet_result(TET_PASS);
	return;
}
