/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tattrx.c,v $
 * Revision 1.1.6.2  1996/02/17  23:22:40  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:25  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:11:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:39  root]
 * 
 * Revision 1.1.4.3  1993/01/25  19:29:08  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:45:49  bbelch]
 * 
 * Revision 1.1.4.2  1993/01/11  23:41:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:15:20  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/08  20:58:32  rsalz
 * 	Close 3241 by removing declaration of malloc.
 * 	[1992/05/08  20:29:53  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:37:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/


tattrtest_t2 xmit_value_into_from_xr;
ndr_boolean from_xr_called;
ndr_boolean to_xr_called;
ndr_boolean free_called;
ndr_boolean free_xr_called;

void tattrtest_t1_from_xmit(xmit_value, value)
tattrtest_t2 *xmit_value;
tattrtest_t1 *value;
{
    from_xr_called = true;
    *value = xmit_value_into_from_xr = *xmit_value;
}


tattrtest_t1 value_into_to_xr;
tattrtest_t2 *xmit_value_from_to_xr;

void tattrtest_t1_to_xmit(value, xmit_value)
tattrtest_t1 *value;
tattrtest_t2 **xmit_value;
{
    to_xr_called = true;
    *xmit_value =
    xmit_value_from_to_xr =
        (tattrtest_t2 *) malloc (sizeof (tattrtest_t2));
    **xmit_value = value_into_to_xr = *value;
}


tattrtest_t1 value_into_free;

void tattrtest_t1_free_inst(value)
tattrtest_t1 *value;
{
    free_called = true;
    value_into_free = *value;
}


tattrtest_t2 value_into_free_xr;
tattrtest_t2 *xmit_value_into_free_xr;

void tattrtest_t1_free_xmit(xmit_value)
tattrtest_t2 *xmit_value;
{
    free_xr_called = true;
    value_into_free_xr = *xmit_value;
    xmit_value_into_free_xr = xmit_value;
    free (xmit_value);
}
