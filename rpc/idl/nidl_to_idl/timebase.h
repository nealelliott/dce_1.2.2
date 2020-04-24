/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: timebase.h,v $
 * Revision 1.1.4.2  1996/02/18  23:46:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:57  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:37:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:18:39  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:34:05  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:48:46  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:33:59  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:14:26  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:00:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef time_base_included
#define time_base_included
#include "idl_base.h"
typedef ndr_$ulong_int time_$clockh_t;
typedef struct time_$clock_t time_$clock_t;
struct time_$clock_t {
time_$clockh_t high;
ndr_$ushort_int low;
};
#endif
