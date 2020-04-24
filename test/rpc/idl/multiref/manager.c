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
 * Revision 1.1.6.2  1996/02/17  23:15:03  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:08  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:25:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:13:46  root]
 * 
 * Revision 1.1.4.1  1994/02/01  22:00:27  cbrooks
 * 	OT 9858 - add dce.h to manager.c files
 * 	[1994/02/01  21:59:17  cbrooks]
 * 
 * Revision 1.1.2.2  1993/01/07  19:59:07  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:35:03  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <dce/dce.h>
#include <stdio.h>
#include <multiref.h>
#include <test_common.h>

void his_op0
#ifdef _DCE_PROTO_
(
    /* [in] */handle_t h,
   /*     [in]*/ ndr_short_int multiply_by,
  /*      [in,out]*/ thing_t *p_thing
)
#else
( h, multiply_by, p_thing )
    /* [in] */handle_t h;
   /*     [in]*/ ndr_short_int multiply_by;
  /*      [in,out]*/ thing_t *p_thing;
#endif
{
    p_thing->s1 *= multiply_by;
    ***(p_thing->rprprp) *= multiply_by;
    p_thing->s2 *= multiply_by;
}

globaldef multiref_v0_0_epv_t multiref_v0_0_m_epv = {
    his_op0
};
