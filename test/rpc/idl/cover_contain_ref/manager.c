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
 * Revision 1.1.4.2  1996/02/17  23:13:35  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:22  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:02:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:05  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:52:52  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:31:47  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <cover_contain_ref.h>
#include <test_common.h>

idl_boolean         BO;
idl_char            CH;
idl_byte            BY;
idl_short_float     FL;
idl_long_float      DB;
idl_small_int       SM;
idl_short_int       SH;
idl_long_int        LO;
idl_hyper_int       HY;
idl_usmall_int      USM;
idl_ushort_int      USH;
idl_ulong_int       ULO;
idl_uhyper_int      UHY;
u_t                 U;
fs_t                FS;
vs_t                VS;
vs_t                CS;     /* hack - assumes layout of vs_t and cs_t same */
vs_t                CVS;    /* hack - assumes layout of vs_t and cvs_t same */

void _expected_vals(
    handle_t            h,
    idl_boolean         bo,
    idl_char            ch,
    idl_byte            by,
    idl_short_float     *fl,
    idl_long_float      db,
    idl_small_int       sm,
    idl_short_int       sh,
    idl_long_int        lo,
    idl_hyper_int       hy,
    idl_usmall_int      usm,
    idl_ushort_int      ush,
    idl_ulong_int       ulo,
    idl_uhyper_int      uhy,
    u_t                 *up,
    fs_t                *fsp,
    vs_t                *vsp,
    cs_t                *csp,   /* hack - see above */
    cvs_t               *cvsp  /* hack - see above */
)
{
    int i;

    BO = bo;
    CH = ch;
    BY = by;
    FL = *fl;
    DB = db;
    SM = sm;
    SH = sh;
    LO = lo;
    HY = hy;
    USM = usm;
    USH = ush;
    ULO = ulo;
    UHY = uhy;
    U = *up;

    for (i = 0 ; i < ARRAY_SIZE ; i++)
        FS.a[i] = fsp->a[i];

    VS.l = vsp->l;
    for (i = 0 ; i < vsp->l ; i++)
        VS.a[i] = vsp->a[i];

    CS.s = csp->s;
    for (i = 0 ; i < csp->s ; i++)
        CS.a[i] = csp->a[i];

    CVS.s = cvsp->s;
    CVS.l = cvsp->l;
    for (i = 0 ; i < cvsp->l ; i++)
        CVS.a[i] = cvsp->a[i];
}

idl_long_int _contained_refs(
    handle_t    h,
    in_t        *in_sp,
    out_t       *out_sp,
    byp_t       *bypp,
    cvsp_t      *cvspp,
    flp_t       *flpp,
    hyp_t       *hypp,
    io_t        *io_sp
)
{
    idl_long_int failures = 0;
    int i;

    /*
     * Verify in_sp parameter.
     */
    if (*in_sp->bop != BO
        ||  *in_sp->smp != SM
        ||  *in_sp->ulop != ULO
        ||  (*in_sp->vsp).l != VS.l)
    {
        fprintf(stderr, "Target of [ref] pointer in in_sp has wrong value!\n");
        failures++;
    }

    for (i = 0 ; i < VS.l ; i++)
        if ((*in_sp->vsp).a[i] != VS.a[i])
        {
            fprintf(stderr, "Target of [ref] pointer in in_sp has wrong value!\n");
            failures++;
            break;
        }

    /*
     * Set up return values in out_sp parameter.
     */
    *out_sp->chp = CH;
    *out_sp->lop = LO;
    (*out_sp->uhyp).low = UHY.low;
    (*out_sp->uhyp).high = UHY.high;

    /*
     * Halve all the relevant values in the bypp and cvspp parameters.
     */
    **bypp /= 2;
    (**cvspp).s /= 2;
    (**cvspp).l /= 2;
    for (i = 0 ; i < (**cvspp).l ; i++)
        (**cvspp).a[i] /= 2;

    /*
     * Verify flpp and hypp parameters.
     */
    if (**flpp != FL)
    {
        fprintf(stderr, "Target of [ref] pointer in flpp has wrong value!\n");
        failures++;
    }

    if ((**hypp).low != HY.low || (**hypp).high != HY.high)
    {
        fprintf(stderr, "Target of [ref] pointer in hypp has wrong value!\n");
        failures++;
    }

    /*
     * Halve all the relevant values in the io_sp parameter.
     */
    *io_sp->dbp /= 2;
    *io_sp->usmp /= 2;

    switch ((*io_sp->up).sw)
    {
    case 0: (*io_sp->up).tagged_union.ush /= 2; break;
    case 1: (*io_sp->up).tagged_union.ulo /= 2;
    }

    return failures;
}

globaldef cover_contain_ref_v0_0_epv_t cover_contain_ref_v0_0_m_epv = {
    _expected_vals,
    _contained_refs
};
