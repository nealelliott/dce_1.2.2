/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: specm.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:17  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:12  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:10:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:12  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:28:16  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:44:40  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:40:44  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:14:15  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:10  devrcs
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
#include <spec.h>
#include <dce/pthread_exc.h>
#include <specexc.h>


ndr_boolean op1_called_since_op2 = false;

/*
 * O P 1
 */

void specialtest_op1(h)
handle_t h;
{
    op1_called_since_op2= true;
}

/*
 * O P 2
 */

void specialtest_op2(h)
handle_t h;
{
    op1_called_since_op2= false;
}

/*
 * O P 3
 */

ndr_boolean specialtest_op3(h)
handle_t h;
{
    return (op1_called_since_op2);
}


/*
 * O P 4
 */

void specialtest_op4(h, i1, i2, i3, l, m)
handle_t h;
specialtest_t1 i1[], i2[], i3[];
ndr_long_int *l, m;
{  
    EXCEPTION_INIT(my_specexc);
    RAISE(my_specexc);
}
