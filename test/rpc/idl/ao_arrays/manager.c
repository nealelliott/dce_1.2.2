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
 * Revision 1.1.4.2  1996/02/17  23:12:14  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:32  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:55:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:27:54  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:01:49  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:24:54  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <ao_arrays.h>
#include <test_common.h>

static ndr_long_int process_array
#ifdef NIDL_PROTOTYPES
(
      /* [in, out] */ array_v1_string_t a
)
#else
(a)
      /* [in, out] */ array_v1_string_t a;
#endif

{
    int i, j, k;
    char                *alphabet = "abcdefghijklmnopqrstuvwxyz";

    /* Upcase each string in the array (assume alphabetic). */
    for (i = 0 ; i < ARRAY_SIZE ; i++)
        for (j = 0, k= i%STRING_SIZE ; j < STRING_SIZE ; j++, k++)
        {
            if (a[i][j] != alphabet[k%STRING_SIZE])
            {
                printf("Manager got bad value[%d,%d]='%c', expected '%c'\n",
                    i,j,a[i][j], alphabet[k%STRING_SIZE]);
                return 1;
            }
            else
                a[i][j] -= ('a' - 'A');
        }
    return 0;
}



ndr_long_int _op8
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ array_v1_string_t a[AO_ARRAY_SIZE]
)
#else
(h, a)
      /* [in] */ handle_t h;
      /* [in, out] */ array_v1_string_t a[AO_ARRAY_SIZE];
#endif

{
    int i;

    /* Upcase each string in the array (assume alphabetic). */
    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        process_array(a[i]);

    return 0;
}

globaldef ao_arrays_v0_0_epv_t ao_arrays_v0_0_m_epv = {
    _op8,
    _op8,
    _op8,
    _op8,
    _op8,
    _op8,
    _op8
};
