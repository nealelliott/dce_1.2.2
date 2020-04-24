/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: enumm.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:27  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:11  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:04:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:09  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:04:13  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:35:38  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  22:13:33  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:05:38  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:34:58  devrcs
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



#include <enum.h>


/*
 * e n u m t e s t _ o p 1
 */
void
enumtest_op1(h, i, o)
handle_t h;
enumtest_t1 i, *o;
{

    switch ((int) i) {
        case enumtest_t1_id1:
            *o = enumtest_t1_id2;
            break;
        case enumtest_t1_id2:
            *o = enumtest_t1_id3;
            break;
        case enumtest_t1_id3:
            *o = enumtest_t1_id0;
            break;
        case enumtest_t1_id0:
            *o = enumtest_t1_id1;
    }
}


/*
 * e n u m t e s t _ o p 2
 */
void 
enumtest_op2(h, i, o)
handle_t h;
enumtest_t2 i, *o;
{

    switch ((int) i) {
        case enumtest_t2_id1:
            *o = enumtest_t2_id2;
            break;
        case enumtest_t2_id2:
            *o = enumtest_t2_id3;
            break;
        case enumtest_t2_id3:
            *o = enumtest_t2_id0;
            break;
        case enumtest_t2_id0:
            *o = enumtest_t2_id1;
    }
}

