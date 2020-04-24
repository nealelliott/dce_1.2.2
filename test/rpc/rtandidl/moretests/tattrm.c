/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tattrm.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:38  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:24  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:11:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:38  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:29:06  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:45:44  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:41:52  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:15:16  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:33  devrcs
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
#include <tattr.h>
#include <nidltest.h>


#include <tattrx.c>

/*
 * op1
 */
 
void tattrtest_op1 (h, i, o)
ndr_long_int h, i, *o;
{
    *o = i + h;
}

/*
 * op2
 */
 
void tattrtest_op2 (h)
handle_t h;
{
    value_into_to_xr = 57;
    xmit_value_from_to_xr = NULL;
    xmit_value_into_free_xr = NULL;
    xmit_value_into_from_xr = 57;
    value_into_free_xr = 57;
    value_into_free = 57;

    from_xr_called = false;
    to_xr_called = false;
    free_called = false;
    free_xr_called = false;
}

/*
 * op3
 */
 
ndr_boolean tattrtest_op3 (h, msg)
handle_t h;
astring *msg;
{
    if (value_into_to_xr != 3 + 1) 
    {
        sprintf ((char *)msg,
                 "value_into_to_xr (%d) != 3 + 1\n",
                 value_into_to_xr);
        return(true);
    }

    if (xmit_value_from_to_xr != xmit_value_into_free_xr) 
    {
        sprintf ((char *)msg,
               "xmit_value_from_to_xr (%8lx) != xmit_value_into_free_xr (%8lx)",
                 xmit_value_from_to_xr, xmit_value_into_free_xr);
        return(true);
    }

    if (value_into_to_xr != value_into_free_xr) 
    {
        sprintf ((char *)msg,
                 "value_into_to_xr (%8lx) != value_into_free_xr (%8lx)",
                 value_into_to_xr, value_into_free_xr);
        return(true);
    }

    if (xmit_value_into_from_xr != 3) 
    {
        sprintf ((char *)msg,
                 "xmit_value_into_from_xr (%d) != 3",
                 xmit_value_into_from_xr);
        return(true);
    }

    if (value_into_free != xmit_value_into_from_xr) 
    {
        sprintf ((char *)msg,
                 "value_into_free (%d) != xmit_value_into_from_xr (%d)",
                 xmit_value_into_from_xr, xmit_value_into_from_xr);
        return(true);
    }

    if ( !(from_xr_called && to_xr_called && free_xr_called && free_called)) 
    {
        sprintf ((char *)msg,
"(!(from_xr_called (%c) && to_xr_called (%c) && free_xr_called (%c) && free_called (%c)",
        from_xr_called?'T':'F', to_xr_called?'T':'F', free_xr_called?'T':'F', free_called?'T':'F' );
        return(true);
    }

    return (false);
}

