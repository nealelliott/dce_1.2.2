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
 * Revision 1.1.4.2  1996/02/17  23:15:53  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:25  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:27  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:37:26  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:37:56  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <math.h>
#include <test_common.h>
#include <packed_byte_array.h>

#define COOKIE      0x08812685
#define JUNK        0x11223344
#define FLOAT_JUNK  1234567

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    fixed_union_t   fixed_union;
    i_v_union_t     i_v_union;
    o_v_union_t     o_v_union;
    io_v_union_t    io_v_union;
    int             i, j, k, f;
    int             failures = 0, mgr_failures;
    ndr_byte        *alphabet = (ndr_byte *)"abcdefghijklmnopqrstuvwxyz";
    ndr_byte        *junk     = (ndr_byte *)"!@#$%^&*()!@#$%^&*()!@#$%^";

    /*
     * Test fixed union:
     *  [in,out] union with union arm.
     */
    fixed_union.sw = GREEN;
    fixed_union.tagged_union.u.sw = 'l';
    fixed_union.tagged_union.u.tagged_union.l = COOKIE;

    mgr_failures = fixed_op(h, &fixed_union);
    if (mgr_failures != 0)
    {
        fprintf(stderr, "Failure in fixed_op manager routine\n");
        failures += mgr_failures;
    }

    if (fixed_union.sw != GREEN
        ||  fixed_union.tagged_union.u.sw != 'c'
        ||  fixed_union.tagged_union.u.tagged_union.c != 'c')
    {
        fprintf(stderr, "Failure: Bad data returned from fixed_op()\n");
        failures++;
    }

    /*
     * Test varying unions:
     *  [in] union with struct arm ct. varying array of union
     *  [in,out] union with struct arms ct. varying arrays of array and struct
     *  [out] union with struct arms ct. varying arrays of long and pointer
     */
    i_v_union.sw = ndr_true;
    i_v_union.tagged_union.svu.l = HALF_ELEM;

    io_v_union.sw = 1;
    io_v_union.tagged_union.sva.l = NUM_ELEM;
 
    for (i = 0 ; i < HALF_ELEM ; i++)
    {
        switch (i % 3)
        {
        case 0:
            i_v_union.tagged_union.svu.au[i].sw = 'l';
            i_v_union.tagged_union.svu.au[i].tagged_union.l = 3000 * i;
            break;

        case 1:
            i_v_union.tagged_union.svu.au[i].sw = 's';
            i_v_union.tagged_union.svu.au[i].tagged_union.s = -20 * i;
            break;

        case 2: 
            i_v_union.tagged_union.svu.au[i].sw = 'c';
            i_v_union.tagged_union.svu.au[i].tagged_union.c = i % 256;
            break;
       }
    }

    for (i = HALF_ELEM ; i < NUM_ELEM ; i++)
    {
        i_v_union.tagged_union.svu.au[i].sw = JUNK % 256;
        i_v_union.tagged_union.svu.au[i].tagged_union.l = JUNK;
    }

    o_v_union.sw = JUNK % 65536;
    o_v_union.tagged_union.svl.l = JUNK;

    for (i = 0 ; i < NUM_ELEM ; i++)
    {
        o_v_union.tagged_union.svl.al[i] = JUNK;

        switch (i % 2)
        {
        case 0:
            j = i % NUM_LETTERS;
            k = NUM_LETTERS - j;
            strncpy(io_v_union.tagged_union.sva.aa[i], &alphabet[j], k);
            strncpy(&io_v_union.tagged_union.sva.aa[i][k], alphabet, j);
            break;

        case 1:
            strncpy(io_v_union.tagged_union.sva.aa[i], junk, NUM_LETTERS);
            break;
        }
    }

    /*
     * First call to vary_op.
     */
    mgr_failures = vary_op(h, &i_v_union, &io_v_union, &o_v_union);
    if (mgr_failures != 0)
    {
        fprintf(stderr, "Failure in vary_op manager routine\n");
        failures += mgr_failures;
    }

    if (i_v_union.sw != ndr_true
        ||  i_v_union.tagged_union.svu.l != HALF_ELEM
        ||  io_v_union.sw != 1
        ||  io_v_union.tagged_union.sva.l != HALF_ELEM
        ||  o_v_union.sw != 1
        ||  o_v_union.tagged_union.svl.l != HALF_ELEM)
    {
        fprintf(stderr, "Failure: Bad union switch or array length in vary_op()\n");
        failures++;
    }

    /*
     * i_v_union:  Unchanged
     * o_v_union:  Original data from i_v_union
     */
    f = failures;
    for (i = 0 ; i < HALF_ELEM ; i++)
    {
        switch (i % 3)
        {
        case 0:
            if (i_v_union.tagged_union.svu.au[i].sw != 'l'
                ||  i_v_union.tagged_union.svu.au[i].tagged_union.l != 3000 * i
                ||  o_v_union.tagged_union.svl.al[i] != 3000 * i)
            {
                fprintf(stderr, "Failure: Bad long data in case 1 of i_v_union or o_v_union\n");
                failures++;
                fprintf(stderr, "  Expected: %d\n", 3000 * i);
                fprintf(stderr, "  Actual:   %d\n",
                        o_v_union.tagged_union.svl.al[i]);
            }
            break;

        case 1:
            if (i_v_union.tagged_union.svu.au[i].sw != 's'
                ||  i_v_union.tagged_union.svu.au[i].tagged_union.s != -20 * i
                ||  o_v_union.tagged_union.svl.al[i] != -20 * i)
            {
                fprintf(stderr, "Failure: Bad short data in case 1 of i_v_union or o_v_union\n");
                fprintf(stderr, "  Expected: %d\n", -20 * i);
                fprintf(stderr, "  Actual:   %d\n",
                        o_v_union.tagged_union.svl.al[i]);
                failures++;
            }
            break;

        case 2: 
            if (i_v_union.tagged_union.svu.au[i].sw != 'c'
                ||  i_v_union.tagged_union.svu.au[i].tagged_union.c != i % 256
                ||  o_v_union.tagged_union.svl.al[i] != i % 256)
            {
                fprintf(stderr, "Failure: Bad byte data in case 1 of i_v_union or o_v_union\n");
                fprintf(stderr, "  Expected: %d\n", i % 256);
                fprintf(stderr, "  Actual:   %d\n",
                        o_v_union.tagged_union.svl.al[i]);
                failures++;
            }
            break;
        }
        if (failures > f)
            break;
    }

    /*
     * io_v_union: Original "sparse" data compressed to lower half of array
     */
    for (i = 0 ; i < HALF_ELEM ; i++)
    {
        j = (i * 2) % NUM_LETTERS;
        k = NUM_LETTERS - j;
        if (strncmp(io_v_union.tagged_union.sva.aa[i], &alphabet[j], k) != 0
            || strncmp(&io_v_union.tagged_union.sva.aa[i][k], alphabet, j) != 0)
        {
            fprintf(stderr, "Failure: Bad data in io_v_union\n");
            failures++;
            break;
        }
    }

    /*
     * Make sure upper halves of i_v_union and io_v_union unchanged.
     */
    f = failures;
    for (i = HALF_ELEM ; i < NUM_ELEM ; i++)
    {
        if (i_v_union.tagged_union.svu.au[i].sw != JUNK % 256
            ||  i_v_union.tagged_union.svu.au[i].tagged_union.l != JUNK)
        {
            fprintf(stderr, "Failure: Data in i_v_union overwritten\n");
            failures++;
        }

        switch (i % 2)
        {
        case 0:
            j = i % NUM_LETTERS;
            k = NUM_LETTERS - j;
            if (strncmp(io_v_union.tagged_union.sva.aa[i], &alphabet[j], k) != 0
                || strncmp(&io_v_union.tagged_union.sva.aa[i][k], alphabet, j) != 0)
            {
                fprintf(stderr, "Failure: Original data in upper half of io_v_union overwritten\n");
                failures++;
                break;
            }
            break;

        case 1:
            if (strncmp(io_v_union.tagged_union.sva.aa[i], junk, NUM_LETTERS) != 0)
            {
                fprintf(stderr, "Failure: Original junk in upper half of io_v_union overwritten\n");
                failures++;
                break;
            }
            break;
        }
        if (failures > f)
            break;
    }

    /*
     * Data setup for second call to vary_op.
     */

    /* i_v_union remains the same as last call. */

    io_v_union.sw = JUNK;
    io_v_union.tagged_union.svs.l = NUM_ELEM;
 
    o_v_union.sw = JUNK % 65536;
    o_v_union.tagged_union.svp.l = JUNK;

    for (i = 0 ; i < NUM_ELEM ; i++)
    {
        o_v_union.tagged_union.svp.apl[i] = (ndr_long_int *)JUNK;

        switch (i % 2)
        {
        case 0:
            io_v_union.tagged_union.svs.as[i].f = -200 * i;
            io_v_union.tagged_union.svs.as[i].l = 3000 * i;
            break;

        case 1:
            io_v_union.tagged_union.svs.as[i].f = FLOAT_JUNK;
            io_v_union.tagged_union.svs.as[i].l = JUNK;
            break;
        }
    }

    /*
     * Second call to vary_op.
     */
    mgr_failures = vary_op(h, &i_v_union, &io_v_union, &o_v_union);
    if (mgr_failures != 0)
    {
        fprintf(stderr, "Failure in vary_op manager routine\n");
        failures += mgr_failures;
    }

    if (i_v_union.sw != ndr_true
        ||  i_v_union.tagged_union.svu.l != HALF_ELEM
        ||  io_v_union.sw != JUNK
        ||  io_v_union.tagged_union.svs.l != HALF_ELEM
        ||  o_v_union.sw != JUNK % 65536
        ||  o_v_union.tagged_union.svp.l != HALF_ELEM)
    {
        fprintf(stderr, "Failure: Bad union switch or array length in vary_op()\n");
        failures++;
    }

    /*
     * i_v_union:  Unchanged
     * o_v_union:  Original data from i_v_union
     */
    f = failures;
    for (i = 0 ; i < HALF_ELEM ; i++)
    {
        switch (i % 3)
        {
        case 0:
            if (i_v_union.tagged_union.svu.au[i].sw != 'l'
                ||  i_v_union.tagged_union.svu.au[i].tagged_union.l != 3000 * i
                ||  o_v_union.tagged_union.svp.apl[i] == (ndr_long_int *)JUNK
                ||  *(o_v_union.tagged_union.svp.apl[i]) != 3000 * i)
            {
                fprintf(stderr, "Failure: Bad long data in case 1 of i_v_union or o_v_union\n");
                failures++;
                fprintf(stderr, "  Expected: %d\n", 3000 * i);
                fprintf(stderr, "  Actual:   %d\n",
                        *(o_v_union.tagged_union.svp.apl[i]));
            }
            break;

        case 1:
            if (i_v_union.tagged_union.svu.au[i].sw != 's'
                ||  i_v_union.tagged_union.svu.au[i].tagged_union.s != -20 * i
                ||  o_v_union.tagged_union.svp.apl[i] == (ndr_long_int *)JUNK
                ||  *(o_v_union.tagged_union.svp.apl[i]) != -20 * i)
            {
                fprintf(stderr, "Failure: Bad short data in case 1 of i_v_union or o_v_union\n");
                fprintf(stderr, "  Expected: %d\n", -20 * i);
                fprintf(stderr, "  Actual:   %d\n",
                        *(o_v_union.tagged_union.svp.apl[i]));
                failures++;
            }
            break;

        case 2: 
            if (i_v_union.tagged_union.svu.au[i].sw != 'c'
                ||  i_v_union.tagged_union.svu.au[i].tagged_union.c != i % 256
                ||  o_v_union.tagged_union.svp.apl[i] == (ndr_long_int *)JUNK
                ||  *(o_v_union.tagged_union.svp.apl[i]) != i % 256)
            {
                fprintf(stderr, "Failure: Bad byte data in case 1 of i_v_union or o_v_union\n");
                fprintf(stderr, "  Expected: %d\n", i % 256);
                fprintf(stderr, "  Actual:   %d\n",
                        *(o_v_union.tagged_union.svp.apl[i]));
                failures++;
            }
            break;
        }
        if (failures > f)
            break;
    }

    /*
     * io_v_union: Original "sparse" data compressed to lower half of array
     */
    for (i = 0 ; i < HALF_ELEM ; i++)
    {
        if (io_v_union.tagged_union.svs.as[i].f != -200 * i * 2
            ||  io_v_union.tagged_union.svs.as[i].l != 3000 * i * 2)

        {
            fprintf(stderr, "Failure: Bad data in io_v_union\n");
            failures++;
            break;
        }
    }

    /*
     * Make sure upper halves of i_v_union and io_v_union unchanged.
     */
    f = failures;
    for (i = HALF_ELEM ; i < NUM_ELEM ; i++)
    {
        if (i_v_union.tagged_union.svu.au[i].sw != JUNK % 256
            ||  i_v_union.tagged_union.svu.au[i].tagged_union.l != JUNK)
        {
            fprintf(stderr, "Failure: Data in i_v_union overwritten\n");
            failures++;
        }

        switch (i % 2)
        {
        case 0:
            if (io_v_union.tagged_union.svs.as[i].f != -200 * i
                ||  io_v_union.tagged_union.svs.as[i].l != 3000 * i)
            {
                fprintf(stderr, "Failure: Original data in upper half of io_v_union overwritten\n");
                failures++;
                break;
            }
            break;

        case 1:
            if (io_v_union.tagged_union.svs.as[i].f != FLOAT_JUNK
                ||  io_v_union.tagged_union.svs.as[i].l != JUNK)
            {
                fprintf(stderr, "Failure: Original junk in upper half of io_v_union overwritten\n");
                failures++;
                break;
            }
            break;
        }
        if (failures > f)
            break;
    }

    return failures;
}
