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
 * Revision 1.1.6.2  1996/02/17  23:13:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:03  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:59:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:07  root]
 * 
 * Revision 1.1.4.1  1994/02/01  22:00:25  cbrooks
 * 	OT 9858 - add dce.h to manager.c files
 * 	[1994/02/01  21:59:13  cbrooks]
 * 
 * Revision 1.1.2.2  1993/01/07  19:10:24  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:29:01  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:31:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <dce/dce.h>

#ifdef _DCE_PROTO_
int after_dce = 1 ;
#else
int after_dce = 0 ;
#endif
#include <bug_3.h>

static ndr_boolean verbose = false;


static ndr_boolean
op0
#ifdef _DCE_PROTO_
(
    /*in*/  handle_t        h,
    /*in*/  ndr_boolean     b
)
#else
(h, b)
    handle_t    h;
    ndr_boolean b;
#endif
{
    if (verbose)
        printf("In bug_3_op0().\n");

    return ! b;
}

static ndr_boolean
op1
#ifdef _DCE_PROTO_
(
    /*in*/      handle_t        h,
    /*in,out*/  ndr_boolean     *b
)
#else
(h, b)
    handle_t    h;
    ndr_boolean *b;
#endif
{
    ndr_boolean btoo;

    if (verbose)
        printf("In bug_3_op1().\n");

    btoo = *b;
    *b = !btoo;
    return btoo;
}

globaldef bug_3_v0_0_epv_t bug_3_v0_0_m_epv = {
    op0,
    op1
};
