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
 * Revision 1.1.4.2  1996/02/17  23:12:28  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:40  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:56:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:15  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:04:19  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:26:06  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <arrayp.h>
#include <stdio.h>

static ndr_short_int mop0
#ifdef NIDL_PROTOTYPES
(
    /* [in] */handle_t h,
    /* [in] */ndr_long_int size,
    /* [in] */ndr_long_int check1,
    /* [in] */ndr_long_int check2,
    /* [in] */cp_t carray[],
    /* [in] */long_t *larray[]
)
#else
(h, size, check1, check2, carray, larray)
    /* [in] */handle_t h;
    /* [in] */ndr_long_int size;
    /* [in] */ndr_long_int check1;
    /* [in] */ndr_long_int check2;
    /* [in] */cp_t carray[];
    /* [in] */long_t *larray[];
#endif
{
    int i;
    ndr_long_int sum1, sum2;
    ndr_short_int failures = 0;

    if (carray[0].two != NULL)
    {
        printf("op0: Element zero of carray is not NULL!\n");
        failures++;
    }

    if (larray[0] != NULL)
    {
        printf("op0: Element zero of larray is not NULL!\n");
        failures++;
    }

    sum1 = sum2 = 0;
    for (i = 1; i < size; i++)
    {
        sum1 += carray[i].one;
        sum1 += carray[i].two->c;
        sum1 += carray[i].three;
        sum2 += larray[i]->l;
    }

    if (sum1 != check1)
    {
        printf("sum1 != check1 (%d != %d)\n", sum1, check1);
        failures++;
    }

    if (sum2 != check2)
    {
        printf("sum2 != check2 (%d != %d)\n", sum2, check2);
        failures++;
    }

    carray[0] = carray[size-1];
    larray[0] = larray[size-1];
    carray[size-1].two = NULL;
    larray[size-1] = NULL;

    return failures;
}

static ndr_short_int mop1
#ifdef NIDL_PROTOTYPES
(
    /* [in] */handle_t h,
    /* [in] */ndr_long_int size,
    /* [in] */s_t sarray[]
)
#else
(h, size, sarray)
    /* [in] */handle_t h;
    /* [in] */ndr_long_int size;
    /* [in] */s_t sarray[];
#endif
{
    int i, k, failures = 0;
    ndr_short_int namelen;
    ndr_long_int checksum;


    for (i = 0; i < size; i++)
    {
        namelen = sarray[i].namelen;
        checksum = 0;
        switch (sarray[i].body.i)
        {
            case 0:
                for (k = 0; k < namelen; k++) 
                    checksum += sarray[i].body.tag.name[k];
                if (checksum != sarray[i].checksum)
                {
                    printf("op1: Case 0 (i = %d): %d != %d\n", i, checksum, 
                           sarray[i].checksum);
                    failures++;
                }
                break;
            case 1:
                for (k = 0; k < MAXNAME; k++)
                    if (k < namelen) 
                        checksum += *(sarray[i].body.tag.namep[k]);
                    else if (sarray[i].body.tag.namep[k] != NULL)
                    {
                        printf("op1: Case 1 (i = %d) : namep[%d] != NULL\n",
                               i, k);
                        failures++;
                    }
                if (checksum != sarray[i].checksum)
                {
                    printf("op1: Case 1 (i = %d): %d != %d\n", i, checksum, 
                           sarray[i].checksum);
                    failures++;
                }
                break;
            case 2:
                break;
            default:
                printf("op1: Bogus tag (i = %d): %d\n", sarray[i].body.i);
				failures++;
                break;
        }
    }
    return failures;
}

globaldef arrayp_v0_0_epv_t arrayp_v0_0_m_epv = {mop0, mop1};
