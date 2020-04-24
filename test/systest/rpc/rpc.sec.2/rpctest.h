/* 
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
*/
/*
 * HISTORY
 * $Log: rpctest.h,v $
 * Revision 1.1.36.2  1996/02/18  23:07:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:49  marty]
 *
 * Revision 1.1.36.1  1995/12/11  22:44:19  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/04  15:31 UTC  bissen
 * 	merge from HPDCE01
 * 	[1995/12/11  22:20:54  root]
 * 
 * Revision 1.1.34.3  1993/10/14  17:37:13  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:28  cmckeen]
 * 
 * Revision 1.1.34.2  1993/09/30  14:40:58  cmckeen
 * 	Changed TIMEOFDAY define
 * 	[1993/09/30  14:40:40  cmckeen]
 * 
 * Revision 1.1.34.1  1993/09/09  19:13:11  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/09  19:12:06  cmckeen]
 * 
 * Revision 1.1.2.3  1993/08/19  15:36:45  bissen
 * 	Removed typedef of uchar: multiple definitions of uchar in test rpc.cds.3
 * 	[1993/08/19  15:35:50  bissen]
 * 
 * Revision 1.1.2.2  1993/08/16  14:46:50  eheller
 * 	TIMEOFDAY fix.
 * 	[1993/07/23  18:37:13  eheller]
 * 
 * $EndLog$
 */
/* 
 *   The purpose of this file is to define types and constants
 *   which are used by many files in the rpc/rpc.'et_al' directory.
 *   and which are not defined in any system header file.
 *
 *   The code here follows the conventions noted in the RFC-17.0
 *   "DCE 1.1 CODE CLEAN-UP REQUIREMENTS" dated December 1992.
 */

#ifndef _RPCTEST_H
#define _RPCTEST_H

#include <dce/nbase.h>

/* typedef idl_byte uchar ;*/


#endif  /* _RPCTEST_H */

#if !defined(TIMEOFDAY)
#    define TIMEOFDAY		1	/* identifier for timer of the day  */
					/* clock type                       */
#endif /* _TIMEOFDAY_T_ */
