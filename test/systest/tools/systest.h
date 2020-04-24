/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: systest.h,v $
 * Revision 1.1.8.2  1996/03/09  20:52:43  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:30:15  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:51:33  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/05  19:31 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  19:03  bissen
 * 	merge kk and hpdce01
 * 	[1995/12/11  22:38:20  root]
 * 
 * Revision 1.1.4.2  1993/07/09  16:34:39  root
 * 	Initial King Kong branch
 * 	[1993/07/09  16:34:14  root]
 * 
 * Revision 1.1.2.2  1993/04/23  16:26:55  sekhar
 * 	Original coding
 * 	[1993/04/16  14:51:11  sekhar]
 * 
 * $EndLog$
 */

/* 
 *   The purpose of this file is to define types and constants
 *   which are used by many files in the test/systest directory.
 *   and which are not defined in any system header file.
 *
 *   The code here follows the conventions noted in the RFC-17.0
 *   "DCE 1.1 CODE CLEAN-UP REQUIREMENTS" dated December 1992.
 */

#ifndef _SYSTEST_H
#define _SYSTEST_H

#include <dce/nbase.h>

#ifndef _AIX
typedef idl_byte uchar ;
#endif

#ifndef TRUE
#    define TRUE 1
#endif

#ifndef FALSE
#    define FALSE 0
#endif

#endif  /* _SYSTEST_H */

