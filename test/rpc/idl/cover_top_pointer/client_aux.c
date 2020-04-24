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
 * Revision 1.1.4.2  1996/02/17  23:13:45  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:28  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:02:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:23  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:54:28  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:32:31  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:24:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <math.h>
#include <test_common.h>
#include <cover_top_pointer.h>

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int f, failures = 0;
    ndr_char            ch      = 0x01;
    ndr_char            vch;
    ndr_char *          ptrch   = &vch;
    ndr_short_float     fl      = 0x020202;
    ndr_short_float     vfl;
    ndr_short_float *   ptrfl   = &vfl;
    ndr_small_int       sm      = 0x03;
    ndr_small_int       vsm;
    ndr_small_int *     ptrsm   = &vsm;
    ndr_ulong_int       ulo     = 0x04040404;
    ndr_ulong_int       vulo;
    ndr_ulong_int *     ptrulo  = &vulo;
    ndr_usmall_int      usm     = 0x05;
    ndr_usmall_int      vusm;
    ndr_usmall_int *    ptrusm  = &vusm;
    ndr_long_int        lo      = 0x06060606;
    ndr_long_int        vlo;
    ndr_long_int *      ptrlo   = &vlo;
    ndr_boolean         bo      = ndr_true;
    ndr_boolean         vbo;
    ndr_boolean *       ptrbo   = &vbo;
    ndr_uhyper_int      uhy;
    ndr_uhyper_int      vuhy;
    ndr_uhyper_int *    ptruhy  = &vuhy;
    ndr_long_float      db      = 0x090909;
    ndr_long_float      vdb;
    ndr_long_float *    ptrdb   = &vdb;
    ndr_byte            by      = 0x10;
    ndr_byte            vby;
    ndr_byte *          ptrby   = &vby;
    ndr_ushort_int      ush     = 0x1111;
    ndr_ushort_int      vush;
    ndr_ushort_int *    ptrush  = &vush;
    ndr_hyper_int       hy;
    ndr_hyper_int       vhy;
    ndr_hyper_int *     ptrhy   = &vhy;

    uhy.low = 0x07070707;
    uhy.high= 0x08080808;
    hy.low  = 0x12121212;
    hy.high = 0x13131313;

    /*
     * The following parameter pairs have an [in,ref] value and an [in,out,ptr]
     * value equal to NULL.
     */
    ptrch = NULL;
    ptrfl = NULL;

    /*
     * The following parameter pairs have an [in,ref] value and an [in,out,ptr]
     * value that will be point to storage equal to the [in] value both ways.
     */
    *ptrsm = sm;
    *ptrulo = ulo;

    /*
     * The following parameter pairs have an [in,ref] value and an [in,out,ptr]
     * value that will be point to storage equal to the [in] value on the way
     * on the way in and equal to a multiple of the [in] value on the way out.
     */
    *ptrusm = usm;
    *ptrlo = lo;

    /*
     * The following parameter pairs have a NULL [in] value and an
     * [in,out] value that doesn't change across the call.
     */
    ptrbo = NULL;
    ptruhy = NULL;
    ptrdb = NULL;

    /*
     * The following parameter pairs have a non-NULL [in] value and an
     * [in,out] value that changes across the call.
     */
    *ptrby = by;
    *ptrush = ush;
    ptrhy->low = hy.low;
    ptrhy->high = hy.high;

    /*
     * Remote call.
     */
    failures += scalars(h,
                    &ch, ptrch, &fl, ptrfl,
                    &sm, ptrsm, &ulo, ptrulo,
                    &usm, ptrusm, &lo, ptrlo,
                    ptrbo, &bo, ptruhy, &uhy, ptrdb, &db,
                    ptrby, &by, ptrush, &ush, ptrhy, &hy);
    if (failures != 0)
        fprintf(stderr, "%d errors in manager operation _scalars()\n",
                failures);

    f = failures;

    /*
     * The following parameter pairs have an [in,ref] value and an [in,out,ptr]
     * value equal to NULL.
     */
    if (ch != 0x01)
        failures++;
    if (ptrch != NULL)
        failures++;
    if (fl != 0x020202)
        failures++;
    if (ptrfl != NULL)
        failures++;

    /*
     * The following parameter pairs have an [in,ref] value and an [in,out,ptr]
     * value that will be point to storage equal to the [in] value both ways.
     */
    if (sm != 0x03)
        failures++;
    if (ptrsm == NULL || *ptrsm != sm)
        failures++;
    if (ulo != 0x04040404)
        failures++;
    if (ptrulo == NULL || *ptrulo != ulo)
        failures++;

    /*
     * The following parameter pairs have an [in,ref] value and an [in,out,ptr]
     * value that will be point to storage equal to the [in] value on the way
     * on the way in and equal to a multiple of the [in] value on the way out.
     */
    if (usm != 0x05)
        failures++;
    if (ptrusm == NULL || *ptrusm != 2 * usm)
        failures++;
    if (lo != 0x06060606)
        failures++;
    if (ptrlo == NULL || *ptrlo != 2 * lo)
        failures++;

    /*
     * The following parameter pairs have a NULL [in] value and an
     * [in,out] value that doesn't change across the call.
     */
    if (ptrbo != NULL)
        failures++;
    if (bo != ndr_true)
        failures++;
    if (ptruhy != NULL)
        failures++;
    if (uhy.low != 0x07070707 || uhy.high != 0x08080808)
        failures++;
    if (ptrdb != NULL)
        failures++;
    if (db != 0x090909)
        failures++;

    /*
     * The following parameter pairs have a non-NULL [in] value and an
     * [in,out] value that changes across the call.
     */
    if (ptrby == NULL || *ptrby != 0x10)
        failures++;
    if (ptrby == NULL || by != 2 * *ptrby)
        failures++;
    if (ptrush == NULL || *ptrush != 0x1111)
        failures++;
    if (ptrush == NULL || ush != 2 * *ptrush)
        failures++;
    if (ptrhy == NULL || ptrhy->low != 0x12121212 || ptrhy->high != 0x13131313)
        failures++;
    if (ptrhy == NULL || hy.low != 2*ptrhy->low || hy.high != 2*ptrhy->high)
        failures++;

    if (failures > f)
        fprintf(stderr, "%d errors in return values from operation scalars()\n",
                failures-f);

    return failures;
}
