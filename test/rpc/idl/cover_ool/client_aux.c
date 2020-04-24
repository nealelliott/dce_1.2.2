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
 * Revision 1.1.4.2  1996/02/17  23:13:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:22  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:02:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:08  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:53:08  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:31:53  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:32:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <math.h>
#include <test_common.h>
#include <cover_ool.h>
#include <util_macros.h>

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int                 i, j, k, failures = 0, mgr_failures;
    il_struct_t         *il_struct_p, *bmk_il_struct_p;
    ool_struct_t        *ool_struct_p, *bmk_ool_struct_p;
    il_ptr_struct_t     il_ptr_struct;
    ptr_array_t         *ptr_array_p, *bmk_ptr_array_p;
    array_v1_string_t   array_v1_string;    /* Array of strings */
    array_ool_xmitas_t  array_ool_xmitas;   /* Array of xmitas types */
    struct_xmitas_t     struct_xmitas;
    complex_ool_struct_t complex_ool_struct;
    long                long_ref_1, long_ref_2, long_ref_out_1, long_ref_out_2;
    long                long_ptr_1, long_ptr_2, long_ptr_out_1, long_ptr_out_2;
    char                guard[4];
    ndr_small_int       size;
    char                *alphabet = "abcdefghijklmnopqrstuvwxyz";
    char                *ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    long_ref_1 = random();
    long_ref_2 = random();
    long_ptr_1 = random();
    long_ptr_2 = random();

    for (i = 0 ; i < sizeof(guard) ; i++)
        guard[i] = random();

/*
 * 1a) [out_of_line] conformant struct as sub-structure of [in_line] struct.
 */
    size = random() % 128;
    ALLOC_CFMT_MEM(op1a, il_struct_p, bmk_il_struct_p, il_struct_t, size,
        ndr_long_int, il_struct_p->struc.a, bmk_il_struct_p->struc.a, guard);
    il_struct_p->sh = random();
    il_struct_p->struc.size = size;
    CALL(op1a, (h, il_struct_p), mgr_failures, failures);
    CHECK_CFMT_MEM(op1a, il_struct_p, bmk_il_struct_p, il_struct_t, size,
        ndr_long_int, il_struct_p->struc.a, bmk_il_struct_p->struc.a, guard,
        *2, failures);

/*
 * 1b) [out_of_line] conformant struct as sub-structure of [out_of_line] struct.
 */
    size = random() % 128;
    ALLOC_CFMT_MEM(op1b, ool_struct_p, bmk_ool_struct_p, ool_struct_t, size,
        ndr_long_int, ool_struct_p->struc.a, bmk_ool_struct_p->struc.a, guard);
    ool_struct_p->sh = random();
    ool_struct_p->struc.size = size;
    CALL(op1b, (h, ool_struct_p), mgr_failures, failures);
    CHECK_CFMT_MEM(op1b, ool_struct_p, bmk_ool_struct_p, ool_struct_t, size,
        ndr_long_int, ool_struct_p->struc.a, bmk_ool_struct_p->struc.a, guard,
        *3, failures);

/*
 * 2) [out_of_line] struct containing pointers as sub-structure of [in_line]
 *    structure containing other pointers.
 */
    long_ref_out_1 = 2 * long_ref_1;
    long_ptr_out_1 = 3 * long_ptr_1;
    long_ref_out_2 = 4 * long_ref_2;
    long_ptr_out_2 = 5 * long_ptr_2;
    il_ptr_struct.ref_ptr       = &long_ref_1;
    il_ptr_struct.full_ptr      = &long_ptr_1;
    il_ptr_struct.struc.ref_ptr = &long_ref_2;
    il_ptr_struct.struc.full_ptr= &long_ptr_2;
    CALL(op2, (h, &il_ptr_struct), mgr_failures, failures);
    if (    *il_ptr_struct.ref_ptr          != long_ref_out_1
        ||  *il_ptr_struct.full_ptr         != long_ptr_out_1
        ||  *il_ptr_struct.struc.ref_ptr    != long_ref_out_2
        ||  *il_ptr_struct.struc.full_ptr   != long_ptr_out_2)
        FAIL("Return values from op2 are incorrect\n", failures);

/*
 * 5) [out_of_line] typedef [ptr] <array_definition>.
 */
    ALLOC_CFMT_MEM(op5, ptr_array_p, bmk_ptr_array_p, ptr_array_t, ARRAY_SIZE,
        ndr_long_int, (*ptr_array_p), (*bmk_ptr_array_p), guard);
    CALL(op5, (h, ARRAY_SIZE, *ptr_array_p), mgr_failures, failures);
    CHECK_CFMT_MEM(op5, ptr_array_p, bmk_ptr_array_p, ptr_array_t, ARRAY_SIZE,
        ndr_long_int, (*ptr_array_p), (*bmk_ptr_array_p), guard, *2, failures);

/*
 * 8) [in_line] array of [out_of_line, v1_string, v1_array] arrays.
 */
    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        /* Put a variation of the alphabet into this string. */
        for (j = 0, k = i%STRING_SIZE ; j < STRING_SIZE ; j++, k++)
            array_v1_string[i][j] = alphabet[k%STRING_SIZE];
        array_v1_string[i][STRING_SIZE] = '\0';
    }
    CALL(op8, (h, array_v1_string), mgr_failures, failures);
    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        /* Each return string should be the original in upper case. */
        for (j = 0, k = i%STRING_SIZE ; j < STRING_SIZE ; j++, k++)
            if (array_v1_string[i][j] != ALPHABET[k%STRING_SIZE])
                FAIL_BREAK("Return values from op8 are incorrect\n", failures);
        if (array_v1_string[i][STRING_SIZE] != '\0')
            FAIL_BREAK("Return string from op8 does not have null terminator\n",
                failures);
    }

/*
 * 11b) Array of out_of_line transmit_as type.
 */
    for (i = 0 ; i < ARRAY_SIZE ; i++)
        array_ool_xmitas[i].l = i;
    CALL(op11b, (h, array_ool_xmitas), mgr_failures, failures);
    for (i = 0 ; i < ARRAY_SIZE ; i++)
        if (array_ool_xmitas[i].l != 2*i)
            FAIL_BREAK("Return values from op11b are incorrect\n", failures);

/*
 * 12a) [out_of_line] struct as sub-structure of [transmit_as] struct.
 */
    struct_xmitas.sh        = 111;
    struct_xmitas.struc.l   = 222222;
    struct_xmitas.struc.f   = 333333.0;
    struct_xmitas.d         = 444444.0;
    CALL(op12a, (h, &struct_xmitas), mgr_failures, failures);
    if (    struct_xmitas.sh        != 2*111
        ||  struct_xmitas.struc.l   != 2*222222
        ||  struct_xmitas.struc.f   != 2*333333.0
        ||  struct_xmitas.d         != 2*444444.0)
        FAIL("Return values from op12a are incorrect\n", failures);

/*
 * 12b) [transmit_as] struct as sub-structure of [out_of_line] struct.
 */
    complex_ool_struct.sh       = 111;
    complex_ool_struct.struc.l  = 222222;
    complex_ool_struct.struc.f  = 333333.0;
    complex_ool_struct.d        = 444444.0;
    CALL(op12b, (h, &complex_ool_struct), mgr_failures, failures);
    if (    complex_ool_struct.sh       != 2*111
        ||  complex_ool_struct.struc.l  != 2*222222
        ||  complex_ool_struct.struc.f  != 2*333333.0
        ||  complex_ool_struct.d        != 2*444444.0)
        FAIL("Return values from op12b are incorrect\n", failures);

    if (failures != 0)
        fprintf(stderr, "%d failures.\n", failures);

    return failures;
}
