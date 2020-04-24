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
 * Revision 1.1.4.2  1996/02/17  23:12:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:35  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:55:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:02  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:03:06  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:25:24  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <array2.h>
#include <test_common.h>

static ndr_usmall_int mop0
#ifdef NIDL_PROTOTYPES
(
    /* [in] */handle_t h,
    /* [in] */ref_t *p,
    /* [in] */ndr_long_int s,
    /* [in] */ndr_long_int *checksum
)
#else
(h, p, s, checksum)
    /* [in] */handle_t h;
    /* [in] */ref_t *p;
    /* [in] */ndr_long_int s;
    /* [in] */ndr_long_int *checksum;
#endif
{
    int i, j, k, failures = 0;
    ndr_long_int check2;

    check2 = 0;
    for (i = 0; i < s; i++)
        for (j = 0; j < p[i].s; j++)
            if (p[i].array[j].array_is_null)
            {
                if (p[i].array[j].array)
                {
                    printf("Expected NULL array, got non-null, i=%d, j=%d\n",
                            i,j);
                    failures++;
                }
            }
            else
            {
                if (!p[i].array[j].array)
                {
                    printf("Got unexpected NULL array, i=%d, j=%d\n",
                            i,j);
                    failures++;
                }
                for (k = 0; k < p[i].array[j].l; k++)
                    check2 += p[i].array[j].array[k];
            }

    if (check2 != *checksum)
    {
        printf("Checksums don't match (%d != %d)!\n", check2, checksum);
        failures++;
    }

    check2 = 0;
    for (i = 0; i < s; i++)
        for (j = 0; j < p[i].s; j++)
            if (!p[i].array[j].array_is_null)
            {
                p[i].array[j].l = p[i].array[j].s;
                for (k = 0; k < p[i].array[j].s; k++)
                    check2 += p[i].array[j].array[k] = (random()&0x7fff);
            }

    *checksum = check2;

    return failures;
}

globaldef array2_v0_0_epv_t array2_v0_0_m_epv = {mop0};
