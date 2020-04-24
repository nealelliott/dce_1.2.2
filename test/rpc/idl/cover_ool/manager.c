/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:13:38  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:23  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:02:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:12  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:53:28  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:32:03  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:32:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <cover_ool.h>
#include <test_common.h>
#include <util_macros.h>

/*
 * 1a) [out_of_line] conformant struct as sub-structure of [in_line] struct.
 */
ndr_long_int _op1a
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ il_struct_t *p
)
#else
(h, p)
      /* [in] */ handle_t h;
      /* [in, out] */ il_struct_t *p;
#endif

{
    MODIFY_CFMT_MEM(p, p->struc.size, p->struc.a, *2);
    return 0;
}

/*
 * 1b) [out_of_line] conformant struct as sub-structure of [out_of_line] struct.
 */
ndr_long_int _op1b
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ ool_struct_t *p
)
#else
(h, p)
      /* [in] */ handle_t h;
      /* [in, out] */ ool_struct_t *p;
#endif

{
    MODIFY_CFMT_MEM(p, p->struc.size, p->struc.a, *3);
    return 0;
}

/*
 * 2) [out_of_line] struct containing pointers as sub-structure of [in_line]
 *    structure containing other pointers.
 */
ndr_long_int _op2
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ il_ptr_struct_t *p
)
#else
(h, p)
      /* [in] */ handle_t h;
      /* [in, out] */ il_ptr_struct_t *p;
#endif

{
    long temp = *p->struc.full_ptr;
    /*
     * Assume full pointers are non-NULL.  For one of them, just change the
     * value; for the other, allocate new storage.
     */
    RPC_SS_ALLOCATE((ndr_long_int *), p->struc.full_ptr, sizeof(ndr_long_int));
    *p->ref_ptr         *= 2;
    *p->full_ptr        *= 3;
    *p->struc.ref_ptr   *= 4;
    *p->struc.full_ptr  = temp * 5;
    return 0;
}

/*
 * 5) [out_of_line] typedef [ptr] <array_definition>.
 */
ndr_long_int _op5
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in] */ ndr_long_int s,
      /* [in, out] */ ptr_array_t a
)
#else
(h, s, a)
      /* [in] */ handle_t h;
      /* [in] */ ndr_long_int s;
      /* [in, out] */ ptr_array_t a;
#endif

{
    MODIFY_CFMT_MEM(a, s, a, *2);
    return 0;
}

/*
 * 8) [in_line] array of [out_of_line, v1_string, v1_array] arrays.
 */
ndr_long_int _op8
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ array_v1_string_t a
)
#else
(h, a)
      /* [in] */ handle_t h;
      /* [in, out] */ array_v1_string_t a;
#endif

{
    int i, j, k;

    /* Upcase each string in the array (assume alphabetic). */
    for (i = 0 ; i < ARRAY_SIZE ; i++)
        for (j = 0 ; j < STRING_SIZE ; j++)
            a[i][j] -= ('a' - 'A');

    return 0;
}

/*
 * 11b) Array of out_of_line transmit_as type.
 */
ndr_long_int _op11b
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ array_ool_xmitas_t a
)
#else
(h, a)
      /* [in] */ handle_t h;
      /* [in, out] */ array_ool_xmitas_t a;
#endif

{
    int i;
    for (i = 0 ; i < ARRAY_SIZE ; i++)
        a[i].l *= 2;
    return 0;
}

/*
 * 12a) [out_of_line] struct as sub-structure of [transmit_as] struct.
 */
ndr_long_int _op12a
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ struct_xmitas_t *p
)
#else
(h, p)
      /* [in] */ handle_t h;
      /* [in, out] */ struct_xmitas_t *p;
#endif
{
    p->sh       *= 2;
    p->struc.l  *= 2;
    p->struc.f  *= 2;
    p->d        *= 2;
    return 0;
}

/*
 * 12b) [transmit_as] struct as sub-structure of [out_of_line] struct.
 */
ndr_long_int _op12b
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ complex_ool_struct_t *p
)
#else
(h, p)
      /* [in] */ handle_t h;
      /* [in, out] */ complex_ool_struct_t *p;
#endif
{
    p->sh       *= 2;
    p->struc.l  *= 2;
    p->struc.f  *= 2;
    p->d        *= 2;
    return 0;
}

globaldef cover_ool_v0_0_epv_t cover_ool_v0_0_m_epv = {
    _op1a,
    _op1b,
    _op2,
    _op5,
    _op8,
    _op11b,
    _op12a,
    _op12b
};
