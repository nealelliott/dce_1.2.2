/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tstdriver.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:37  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:05  root]
 * 
 * Revision 1.1.2.2  1993/01/13  19:49:49  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:16:41  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  16-oct-90 woodbury  moved global definitions vars to main module
*/

#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

#define LINESZ   200       /* size of input line buffer */

struct cmd {
char   *c_name;            /* Name of test command */
int    (*c_handler)();     /* Test function to execute */
};

