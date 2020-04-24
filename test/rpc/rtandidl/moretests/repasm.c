/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: repasm.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:59  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:09:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:44  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:27:35  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:43:43  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:39:25  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:12:41  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:36:45  devrcs
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



#include <stdio.h>
#include <repas.h>
#include <nidltest.h>


#include <repasx.c>

/*
 * op1
 */
 
void repastest_op1 (h, i, o)
handle_t h;
repastest_t1 i, *o;
{
    o->f1 = i.f1  * 2;
}
