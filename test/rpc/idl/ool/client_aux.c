/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.4.2  1996/02/17  23:15:18  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:13  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:25:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:13:59  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:31:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:35:45  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <math.h>
#include <ool.h>
#include <test_common.h>

int client_aux(h, passes)
    handle_t h;
    int passes;
{
    ptr_t   in_ptr, out_ptr;
    ool_t   in_ool, out_ool;
    ndr_long_int    ia, ib, ic;
    int     failures = 0;

    /*
     * Test operations with only an [in] or [out] parameter with full pointers.
     */
    in_ptr.a = &ia;
    in_ptr.b = &ib;
    in_ptr.c = &ic;

    ia = rand();
    ib = rand();
    ic = ia + ib;

    has_in_ptr(h, &in_ptr);

    has_out_ptr(h, &out_ptr);

    if (*in_ptr.a != *out_ptr.a
        ||  *in_ptr.b != *out_ptr.b
        ||  *in_ptr.c != *out_ptr.c)
    {
        fprintf(stderr, "has_in_ptr/has_out_ptr failed.\n");
        failures++;
    }


    /*
     * Test operations with only an [in] or [out] out_of_line parameter.
     */
    in_ool.a = rand();
    in_ool.b = rand();
    in_ool.c = in_ool.a + in_ool.b;

    has_in_ool(h, &in_ool);

    has_out_ool(h, &out_ool);

    if (in_ool.a != out_ool.a
        ||  in_ool.b != out_ool.b
        ||  in_ool.c != out_ool.c)
    {
        fprintf(stderr, "has_in_ool/has_out_ool failed.\n");
        failures++;
    }

    return failures;
}
