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
 * Revision 1.1.4.2  1996/02/17  23:15:42  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:21  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:17  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:34:49  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:36:57  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <orecord.h>
#include <test_common.h>

static idl_long_int op1 (
  /* [in] */handle_t h,
  /* [in] */idl_byte c,
  /* [in] */orecord_t *o,
  /* [in] */idl_long_int d
)
{
    idl_long_int checksum = 0;
    int i;

    checksum += c + d;

    for (i = 0; i < o->namelen; i++) checksum += o->name[i];

    return checksum;
}

globaldef orecord_v0_0_epv_t orecord_v0_0_m_epv = {op1};
