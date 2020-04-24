/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:28  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:07  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:29:56  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:46:57  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:50:01  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:16:52  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:59  devrcs
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
#ifdef MSDOS
#  include <dos.h>
#  include <stdlib.h>
#  include <malloc.h>
#endif

/*
 * Tests for closeness of two floating values
 */

ndr_boolean close_sf (f1,f2)
ndr_short_float f1,f2;
{
    ndr_short_float ratio;

    if (-0.0001 < f2 && 0.0001 > f2)
        return (-0.0001 < f1 && 0.0001 > f1);
    else {
        ratio = f1 / f2;
        return (ratio > 0.9999 && ratio < 1.0001);
    }
}

ndr_boolean close_lf (f1,f2)
ndr_long_float f1,f2;
{
    ndr_long_float ratio;

    if (-0.0001 < f2 && 0.0001 > f2)
        return (-0.0001 < f1 && 0.0001 > f1);
    else {
        ratio = f1 / f2;
        return (ratio > 0.9999 && ratio < 1.0001);
    }
}


