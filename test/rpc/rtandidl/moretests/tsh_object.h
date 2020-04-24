/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_object.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:28  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:58  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:29:47  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:46:43  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:49:24  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:16:10  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:53  devrcs
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
# Revision 1.1  91/09/11  16:15:15  marco
# Initial revision
# 
*/



#include <tsh_common.h>


/* %*START*% Start the body of code that will get translated into the FILE.tshrc */

#define Valid_UUID            0
#define Invalid_UUID          1
#define NULL_UUID             2
#define NIL_UUID              3

#define REGISTER              0
#define REPEAT_REGISTER       1 
#define MAXIMUM_REGISTER      2
#define MAXIMUM_UNREGISTER    3
#define UNREGISTER_WITH_NULL  4
#define UNREGISTER_WITH_NIL   5
#define REPEAT_UNREGISTER     6

#define UNREGISTERED		10
#define REG_BY_RUNTIME		11
#define REG_BY_APPLICATION	12
#define SET_INQ_FN		13


/* %*STOP*%  Stop translating for FILE.tshrc  */


uuid_t tsh_c_valid_UUID = 
    {
    32345678,
    1234,
    1234,
    12,
    34,
    1234567890
    };

uuid_t tsh_c_invalid_UUID =
    {
    0x00000000,
    0xFFFF,
    0xFFFF,
    0xFF,
    0xFF,
    0xFFFFFFFF
    };

uuid_t tsh_c_nil_UUID =
    {
    00000000,
    0000,
    0000,
    00,
    00,
    000000000000
    };


