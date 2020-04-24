/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: debug.h,v $
 * Revision 1.1.2.1  1996/09/25  21:00:45  arvind
 * 	OSF DCE 1.2.2 - missing files
 * 	[1996/09/25  20:59:44  arvind]
 *
 * $EndLog$
 */
#ifndef __DEBUG_H__
#define __DEBUG_H__

/*
   assert utility macro for test programs:
   If the predicate (pred) is true, then
   OK: <message> is printed.  Otherwise,
   ERROR: <message> is printed.

   message should be a printf format string.
*/

#include <stdio.h>

#define assert(pred,message)\
  if(pred) printf("OK: ");\
  else { printf("ERROR: "); error_count++; }\
  printf(message);

#endif
