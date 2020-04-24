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
 * Revision 1.1.4.2  1996/02/17  23:13:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:28  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:03:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:26  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:54:47  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:32:38  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:24:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <cover_top_pointer.h>
#include <test_common.h>

ndr_long_int _scalars(h,
                    refch, ptrch, reffl, ptrfl,
                    refsm, ptrsm, refulo, ptrulo,
                    refusm, ptrusm, reflo, ptrlo,
                    ptrbo, refbo, ptruhy, refuhy, ptrdb, refdb,
                    ptrby, refby, ptrush, refush, ptrhy, refhy)

    handle_t h;
    ndr_char *          refch;
    ndr_char *          ptrch;
    ndr_short_float *   reffl;
    ndr_short_float *   ptrfl;
    ndr_small_int *     refsm;
    ndr_small_int *     ptrsm;
    ndr_ulong_int *     refulo;
    ndr_ulong_int *     ptrulo;
    ndr_usmall_int      *refusm;
    ndr_usmall_int      *ptrusm;
    ndr_long_int *      reflo;
    ndr_long_int *      ptrlo;
    ndr_boolean *       ptrbo;
    ndr_boolean *       refbo;
    ndr_uhyper_int      *ptruhy;
    ndr_uhyper_int      *refuhy;
    ndr_long_float *    ptrdb;
    ndr_long_float *    refdb;
    ndr_byte *          ptrby;
    ndr_byte *          refby;
    ndr_ushort_int      *ptrush;
    ndr_ushort_int      *refush;
    ndr_hyper_int *     ptrhy;
    ndr_hyper_int *     refhy;
{
    int failures = 0;

    /*
     * The following parameter pairs have an [in,ref] value and an [in,out,ptr]
     * value equal to NULL.
     */
    if (*refch != 0x01)
        failures++;
    if (ptrch != NULL)
        failures++;
    if (*reffl != 0x020202)
        failures++;
    if (ptrfl != NULL)
        failures++;

    /*
     * The following parameter pairs have an [in,ref] value and an [in,out,ptr]
     * value that will be point to storage equal to the [in] value both ways.
     */
    if (*refsm != 0x03)
        failures++;
    if (ptrsm == NULL || *ptrsm != *refsm)
        failures++;
    if (*refulo != 0x04040404)
        failures++;
    if (ptrulo == NULL || *ptrulo != *refulo)
        failures++;

    /*
     * The following parameter pairs have an [in,ref] value and an [in,out,ptr]
     * value that will be point to storage equal to the [in] value on the way
     * on the way in and equal to a multiple of the [in] value on the way out.
     */
    if (*refusm != 0x05)
        failures++;
    if (ptrusm == NULL || *ptrusm != *refusm)
        failures++;
    else
        *ptrusm *= 2;
    if (*reflo != 0x06060606)
        failures++;
    if (ptrlo == NULL || *ptrlo != *reflo)
        failures++;
    else
        *ptrlo *= 2;

    /*
     * The following parameter pairs have a NULL [in] value and an
     * [in,out] value that doesn't change across the call.
     */
    if (ptrbo != NULL)
        failures++;
    if (*refbo != ndr_true)
        failures++;
    if (ptruhy != NULL)
        failures++;
    if (refuhy->low != 0x07070707 || refuhy->high != 0x08080808)
        failures++;
    if (ptrdb != NULL)
        failures++;
    if (*refdb != 0x090909)
        failures++;

    /*
     * The following parameter pairs have a non-NULL [in] value and an
     * [in,out] value that changes across the call.
     */
    if (*refby != 0x10)
        failures++;
    if (ptrby == NULL || *ptrby != *refby)
        failures++;
    else
        *refby *= 2;
    if (*refush != 0x1111)
        failures++;
    if (ptrush == NULL || *ptrush != *refush)
        failures++;
    else
        *refush *= 2;
    if (refhy->low != 0x12121212 || refhy->high != 0x13131313)
        failures++;
    if (ptrhy == NULL || ptrhy->low != refhy->low || ptrhy->high != refhy->high)
        failures++;
    else
    {
        refhy->low *= 2;
        refhy->high *= 2;
    }

    return failures;
}

globaldef cover_top_pointer_v0_0_epv_t cover_top_pointer_v0_0_m_epv = {
    _scalars
};
