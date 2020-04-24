/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: tet_code.h,v $
 * Revision 1.1.6.2  1996/02/18  23:07:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:43  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:43:35  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/01  14:18 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  18:27  giza
 * 	merge kk & hpdce01
 * 	[1995/12/11  22:35:33  root]
 * 
 * Revision 1.1.2.2  1993/08/24  19:04:16  giza
 * 	Initial version.
 * 	[1993/08/24  19:03:51  giza]
 * 
 * $EndLog$
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* $EndLog$
 */

/*
 * TET reserved codes
 */ 
#define TET_PASS            0
#define TET_FAIL            1
#define TET_UNRESOLVED      2
#define TET_NOTINUSE        3
#define TET_UNSUPPORTED     4
#define TET_UNTESTED        5
#define TET_UNINITIATED     6
#define TET_NORESULT        7
#define TET_SLEEP           8

/*
 * Test suite additional codes
 */
#define TET_DCE_FAIL        40
#define TET_DCE_ERROR       41
#define TET_NET_FAIL        50
#define TET_NET_ERROR       51
