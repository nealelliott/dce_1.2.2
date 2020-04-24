/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: perfg.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:19  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:15:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:48:05  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:57:17  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:25:14  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:41:35  rsalz
 * 	 22-jul-92 mishkin   eliminate use of globaldef
 * 	[1992/05/01  01:22:41  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:41:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      rpcp.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Server manager routines for performance and system execiser
**  auxiliary interface. This interface is generic.
**
**
*/

#include <perf_c.h>
#include <perf_p.h>

/***************************************************************************/

void foo_perfg_op1 (h, n, x)

handle_t                h;
unsigned long           n;
unsigned long           *x;

{
    *x = 2 * n;
}


/***************************************************************************/

void foo_perfg_op2 (h, n, x)

handle_t                h;
unsigned long           n;
unsigned long           *x;

{
    *x = 3 * n;
}


/***************************************************************************/

perfg_v1_0_epv_t foo_perfg_epv =
{
    foo_perfg_op1,
    foo_perfg_op2
};

/***************************************************************************/

void bar_perfg_op1 (h, n, x)

handle_t                h;
unsigned long           n;
unsigned long           *x;

{
    *x = 4 * n;
}


/***************************************************************************/

void bar_perfg_op2 (h, n, x)

handle_t                h;
unsigned long           n;
unsigned long           *x;

{
    *x = 5 * n;
}


/***************************************************************************/

perfg_v1_0_epv_t bar_perfg_epv =
{
    bar_perfg_op1,
    bar_perfg_op2
};
