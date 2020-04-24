/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: strzm.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:20  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:11:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:31  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:28:51  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:45:23  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:41:36  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:14:57  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:25  devrcs
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



#include <scal.h>
#include <strz.h>

/*
 * s t r i n g 0 t e s t _ o p 1
 */
void string0test_op1(h, i, o)
handle_t h;
string0test_t1 i, o;
{
    ndr_long_int indx;

    for (indx=0; i[indx] != '\000'; indx++)
        scalartest_op13(h, i[indx], &o[indx]);
    o[indx] = '\000';
}
