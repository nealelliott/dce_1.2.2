/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rattrm.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:55  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:09:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:34  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:27:08  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:43:09  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:38:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:12:04  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:36:32  devrcs
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



#include <nidltest.h>
#include <rattr.h>


static ndr_long_int op1_in = 0;

/*
 * r a t t r t e s t _ o p 1
 */
void rattrtest_op1(h, i)
handle_t h;
ndr_long_int i;
{
    op1_in = i;
}

/*
 * r a t t r t e s t _ o p 2
 */
ndr_long_int rattrtest_op2(h)
handle_t h;
{
    ndr_long_int i;

    i = op1_in;
    if (i != 0)
        op1_in = 0;
    return (i);
}


    
ndr_ulong_int op3_calls_since_op4_called = 0;

/*
 * r a t t r t e s t _ o p 3
 */
ndr_ulong_int rattrtest_op3(h)
handle_t h;
{
    return (++op3_calls_since_op4_called);
}


/*
 * r a t t r t e s t _ o p 4
 */
void rattrtest_op4(h)
handle_t h;
{
    op3_calls_since_op4_called = 0;
}


ndr_long_int op5_in;

/*
 * r a t t r t e s t _ o p 5
 */
void rattrtest_op5(h, i)
handle_t h;
ndr_long_int i;
{
    op5_in = i;
}

/*
 * r a t t r t e s t _ o p 6
 */
ndr_long_int rattrtest_op6(h)
handle_t h;
{
    return (op5_in);
}

