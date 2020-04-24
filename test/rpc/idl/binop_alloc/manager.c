/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:12:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:58  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:58:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:54  root]
 * 
 * Revision 1.1.2.3  1993/02/26  21:26:43  weisman
 * 	Removed binop_alloc_fail test.
 * 	[1993/02/26  21:10:41  weisman]
 * 
 * Revision 1.1.2.2  1993/01/07  19:08:42  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:28:07  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <binop_alloc.h>
#include <test_common.h>

/* Check that [enable_allocate] correctly enables rpc_ss_allocate */
static void binopwk_add(h, a, b, c)
handle_t h;
ndr_long_int a, b, *c;
{
    int i;
    *c = a + b;

    /* Allocate lots of memory */
    for (i = 0; i < 100; i++) rpc_ss_allocate(1000);
}

/* Check that rpc_ss_enable_allocate correctly enables rpc_ss_allocate */
static void binopwk_add_enable(h, a, b, c)
handle_t h;
ndr_long_int a, b, *c;
{
    int i;
    *c = a + b;

    /* Enable allocations */
    rpc_ss_enable_allocate();

    /* Allocate lots of memory */
    for (i = 0; i < 100; i++) rpc_ss_allocate(1000);

    /* Free all allocations */
    rpc_ss_disable_allocate();
}

globaldef binop_alloc_v0_0_epv_t binop_alloc_v0_0_m_epv = {
      binopwk_add,
      binopwk_add_enable
      };
