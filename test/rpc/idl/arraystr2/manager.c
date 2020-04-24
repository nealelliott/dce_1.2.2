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
 * Revision 1.1.4.2  1996/02/17  23:12:35  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:45  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:56:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:29  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:05:48  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:26:49  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:31:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <arraystr2.h>
#include <test_common.h>
#include <util_macros.h>

/*
 * op1 -  array of [in_line] struct containing string.
 */
ndr_long_int _op1
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ array_il_string_t a
)
#else
(h, a)
      /* [in] */ handle_t h;
      /* [in, out] */ array_il_string_t a;
#endif

{
    int i, j, k;

    /* Upcase each string in the array (assume alphabetic). */
    for (i = 0 ; i < ARRAY_SIZE ; i++)
        for (j = 0 ; j < STRING_SIZE ; j++)
            a[i].s[j] -= ('a' - 'A');

    return 0;
}

/*
 * op2 -  array of [in_line] struct containing string.
 */
ndr_long_int _op2
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ array_ool_string_t a
)
#else
(h, a)
      /* [in] */ handle_t h;
      /* [in, out] */ array_ool_string_t a;
#endif

{
    int i, j, k;

    /* Upcase each string in the array (assume alphabetic). */
    for (i = 0 ; i < ARRAY_SIZE ; i++)
        for (j = 0 ; j < STRING_SIZE ; j++)
            a[i].s[j] -= ('a' - 'A');

    return 0;
}

globaldef arraystr2_v0_0_epv_t arraystr2_v0_0_m_epv = {
    _op1,
    _op2
};
