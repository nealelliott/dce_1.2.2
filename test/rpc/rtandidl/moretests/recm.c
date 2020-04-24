/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: recm.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:57  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:09:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:39  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:27:19  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:43:27  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:39:13  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:12:25  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:36:39  devrcs
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



#include <rec.h>


/*
 * O P 1
 */
void
recordtest_op1(h, i, o)
handle_t h;
recordtest_t1 i, *o;
{
    o->f1 = i.f1 + 1;
}


/*
 * O P 2
 */
void
recordtest_op2(h, io)
handle_t h;
recordtest_t2 *io;
{
    io->f2 = io->f1 + 1;
}


/*
 * O P 3
 */
void
recordtest_op3(h, i, o)
handle_t h;
recordtest_t3 i;
recordtest_t3 *o;
{
    int indx;

    for (indx = 0; indx < recordtest_c1; indx++)
        (o->f1)[indx] = i.f1[indx] + 1;
}


/*
 * O P 4
 */
void
recordtest_op4(h, i, o)
handle_t h;
recordtest_t4 *i;
recordtest_t4 *o;
{
    int indx;

    o->f1 = i->f1;
    for (indx = 0; indx <= i->f1; indx++)
        (o->f3)[indx] = i->f3[indx] + 1;
}


/*
 * O P 5
 */
void
recordtest_op5(h, i, o)
handle_t h;
recordtest_t5 i;
recordtest_t5 *o;
{
    int indx;

    recordtest_op1 (h, i.f1, &(o->f1));
    recordtest_op3 (h, i.f2, &(o->f2));
}


