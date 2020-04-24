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
 * $Log: check_status.h,v $
 * Revision 1.1.4.2  1996/02/18  23:05:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:37  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:47:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:25:11  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:16:28  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:42:34  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:14:56  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/04  21:53:13  mori_m
 * 	CR 9701: RPC runtime I18N extension functional test drop.
 * 	[1994/05/04  21:37:57  mori_m]
 * 
 * $EndLog$
 */

#include <dce/rpc.h>
#include <dce/dce_error.h>

/*
 * General TET printf routine
 */
extern void mtet_infoline();

/* 
 * Test suite additional codes 
 */
#undef TET_DCE_FAIL
#define TET_DCE_FAIL	40
#undef TET_DCE_ERROR
#define TET_DCE_ERROR	41


#define CHECK_STATUS(t, func, returned_st, expected_st) \
{ \
	if (returned_st == expected_st) { \
		if ((t)) { \
			pthread_lock_global_np();\
				mtet_infoline("PASSED %s()\n", func); \
			pthread_unlock_global_np();\
		} \
	} else { \
		dce_error_inq_text(returned_st, (unsigned char *)unexpected, &dce_status); \
		dce_error_inq_text(expected_st, (unsigned char *)expected, &dce_status); \
		pthread_lock_global_np();\
			mtet_infoline("FAILED %s()\nresult:   %s\nexpected:   %s\n\n", \
				func, unexpected, expected); \
			tet_result(TET_DCE_FAIL);\
		pthread_unlock_global_np();\
	} \
} \

static unsigned char	unexpected[dce_c_error_string_len];
static unsigned char	expected[dce_c_error_string_len];
static int		dce_status;
