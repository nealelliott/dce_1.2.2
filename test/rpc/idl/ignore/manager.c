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
 * Revision 1.1.4.2  1996/02/17  23:14:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:02  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:08:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:40  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:58:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:34:50  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <ignore.h>
#include <test_common.h>

/*
 * Version of binopwk to test [ignore] attribute.
 */
static void binopwk_add(h, sp)
handle_t h;
struct_t *sp;
{
    /* Put values in filler fields - stub code should not attempt access. */
    sp->filler1 = (ndr_small_int *)0xDD;
    sp->filler2 = (ndr_short_int *)0xCCCC;
    sp->filler3 = (ndr_long_int *) 0xBBBBBBBB;
    sp->filler4 = (ndr_hyper_int *)0xAAAAAAAA;

    sp->c = sp->a + sp->b;
}

globaldef ignore_v0_0_epv_t ignore_v0_0_m_epv = {binopwk_add};
