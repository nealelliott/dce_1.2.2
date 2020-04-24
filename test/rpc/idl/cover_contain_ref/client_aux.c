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
 * Revision 1.1.4.2  1996/02/17  23:13:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:21  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:01:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:02  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:52:38  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:31:40  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <math.h>
#include <test_common.h>
#include <cover_contain_ref.h>

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int f, failures = 0;
    int i;
    /*
     * Pointed at data.
     */
    ndr_boolean         bo, BO;
    ndr_char            ch, CH;
    ndr_byte            by, BY;
    ndr_short_float     fl, FL;
    ndr_long_float      db, DB;
    ndr_small_int       sm, SM;
    ndr_short_int       sh, SH;
    ndr_long_int        lo, LO;
    ndr_hyper_int       hy, HY;
    ndr_usmall_int      usm, USM;
    ndr_ushort_int      ush, USH;
    ndr_ulong_int       ulo, ULO;
    ndr_uhyper_int      uhy, UHY;
    u_t                 u, U;
    fs_t                fs, FS;
    vs_t                vs, VS;
    vs_t                cs, CS;     /* hack - assumes layout of vs_t and cs_t same */
    vs_t                cvs, CVS;   /* hack - assumes layout of vs_t and cvs_t same */

    /*
     * Data structures containing pointers to data.
     */
    in_t    in_s;
    out_t   out_s;
    byp_t   byp;
    cvsp_t  cvsp;
    flp_t   flp;
    hyp_t   hyp;
    io_t    io_s;

    /*
     * Initialize data.
     */
    bo = BO = rand() % 2;
         CH = rand() % 256;
    by = BY = rand() % 256;
    fl = FL = rand() % 10000000;
    db = DB = rand() % 10000000;
    sm = SM = rand() % 256 - 128;
    sh = SH = rand() % 65536 - 32768;
         LO = rand();
    hy.low = HY.low = rand();
    hy.high = HY.high = rand();
    usm = USM = rand() % 256;
    ush = USH = rand() % 65536;
    ulo = ULO = rand();
          UHY.low = rand();
          UHY.high = rand();

    u.sw = U.sw = rand() % 2;
    switch (u.sw)
    {
    case 0: u.tagged_union.ush = U.tagged_union.ush = rand() % 65536; break;
    case 1: u.tagged_union.ulo = U.tagged_union.ulo = rand();
    }

    vs.l = VS.l = rand() % ARRAY_SIZE;
    cs.s = CS.s = rand() % ARRAY_SIZE;
    cvs.s = CVS.s = rand() % ARRAY_SIZE;
    cvs.l = CVS.l = rand() % cvs.s;

    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        fs.a[i] = FS.a[i] = rand();
        vs.a[i] = VS.a[i] = rand();
        cs.a[i] = CS.a[i] = rand();
        cvs.a[i] = CVS.a[i] = rand();
    }

    /*
     * Call remote operation with all the expected values.
     */

    expected_vals(h, BO, CH, BY, &FL, DB, SM, SH, LO, HY, USM, USH, ULO, UHY,
                  &U, &FS, &VS, (cs_t *)&CS, (cvs_t *)&CVS);

    /*
     * Set pointers to data.
     */
    in_s.bop = &bo;
    in_s.smp = &sm;
    in_s.ulop = &ulo;
    in_s.vsp = &vs;

    out_s.chp = &ch;
    out_s.lop = &lo;
    out_s.uhyp = &uhy;

    byp = &by;
    cvsp = (cvsp_t)&cvs;
    flp = &fl;
    hyp = &hy;

    io_s.dbp = &db;
    io_s.usmp = &usm;
    io_s.up = &u;

    /*
     * Call remote operation.
     */
    failures += contained_refs(h, &in_s, &out_s, &byp, &cvsp, &flp, &hyp, &io_s);
    if (failures != 0)
        fprintf(stderr, "%d errors in manager operation _contained_refs()\n",
                failures);

    /*
     * Verify in_s parameter.
     */
    if (in_s.bop != &bo
        ||  in_s.smp != &sm
        ||  in_s.ulop != &ulo
        ||  in_s.vsp != &vs)
    {
        fprintf(stderr, "A [ref] pointer in in_s changed value!\n");
        failures++;
    }

    if (bo != BO
        ||  sm != SM
        ||  ulo != ULO
        ||  vs.l != VS.l)
    {
        fprintf(stderr, "Target of [ref] pointer in in_s changed value!\n");
        failures++;
    }

    for (i = 0 ; i < ARRAY_SIZE ; i++)
        if (vs.a[i] != VS.a[i])
        {
            fprintf(stderr, "Target of [ref] pointer in in_s changed value!\n");
            failures++;
            break;
        }

    /*
     * Verify out_s parameter.
     */
    if (out_s.chp != &ch
        ||  out_s.lop != &lo
        ||  out_s.uhyp != &uhy)
    {
        fprintf(stderr, "A [ref] pointer in out_s changed value!\n");
        failures++;
    }

    if (ch != CH
        ||  lo != LO
        ||  uhy.low != UHY.low || uhy.high != UHY.high)
    {
        fprintf(stderr, "Target of [ref] pointer in out_s changed value!\n");
        failures++;
    }

    /*
     * Verify byp parameter.
     */
    if (byp != &by)
    {
        fprintf(stderr, "[ref] pointer byp changed value!\n");
        failures++;
    }

    if (by != BY/2)
    {
        fprintf(stderr, "Target of [ref] pointer byp has wrong value!\n");
        failures++;
    }

    /*
     * Verify cvsp parameter.
     */
    if (cvsp != (cvsp_t)&cvs)
    {
        fprintf(stderr, "[ref] pointer cvsp changed value!\n");
        failures++;
    }

    if (cvs.s != CVS.s / 2
        ||  cvs.l != CVS.l / 2)
    {
        fprintf(stderr, "Target of [ref] pointer cvsp has wrong value!\n");
        failures++;
    }

    for (i = 0 ; i < cvs.l ; i++)
        if (cvs.a[i] != CVS.a[i] / 2)
        {
            fprintf(stderr, "Target of [ref] pointer cvsp has wrong value!\n");
            failures++;
            break;
        }

    for (i =  cvs.l ; i < ARRAY_SIZE ; i++)
        if (cvs.a[i] != CVS.a[i])
        {
            fprintf(stderr, "Target of [ref] pointer cvsp has wrong value!\n");
            failures++;
            break;
        }

    /*
     * Verify flp parameter.
     */
    if (flp != &fl)
    {
        fprintf(stderr, "[ref] pointer flp changed value!\n");
        failures++;
    }

    if (fl != FL)
    {
        fprintf(stderr, "Target of [ref] pointer flp changed value!\n");
        failures++;
    }

    /*
     * Verify hyp parameter.
     */
    if (hyp != &hy)
    {
        fprintf(stderr, "[ref] pointer hyp changed value!\n");
        failures++;
    }

    if (hy.low != HY.low || hy.high != HY.high)
    {
        fprintf(stderr, "Target of [ref] pointer hyp changed value!\n");
        failures++;
    }

    /*
     * Verify io_s parameter.
     */
    if (io_s.dbp != &db
        ||  io_s.usmp != &usm
        ||  io_s.up != &u)
    {
        fprintf(stderr, "A [ref] pointer in io_s changed value!\n");
        failures++;
    }

    if (db != DB/2
        ||  usm != USM/2)
    {
        fprintf(stderr, "Target of [ref] pointer in io_s has wrong value!\n");
        failures++;
    }

    if (u.sw != U.sw)
    {
        fprintf(stderr, "Union discriminator in io_s changed value!\n");
        failures++;
    }

    if ((u.sw == 0 && u.tagged_union.ush != U.tagged_union.ush/2)
        ||  (u.sw == 1 && u.tagged_union.ulo != U.tagged_union.ulo/2))
    {
        fprintf(stderr, "Union arm in io_s has wrong value!\n");
        failures++;
    }

    return failures;
}
