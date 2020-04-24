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
 * Revision 1.1.8.2  1996/02/17  23:14:12  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:43  marty]
 *
 * Revision 1.1.8.1  1995/12/11  20:05:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:27  root]
 * 
 * Revision 1.1.6.1  1994/06/29  21:00:00  ohara
 * 	add SNI_SVR4 to the DIV_BY_ZERO define
 * 	[1994/05/12  20:58:25  ohara]
 * 
 * Revision 1.1.4.1  1993/10/12  20:27:10  ganni
 * 	OT 9087 - define DIV_BY_ZERO properly for hp/ux 486/osf
 * 	[1993/10/12  20:25:10  ganni]
 * 
 * Revision 1.1.2.2  1993/01/07  19:56:52  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:33:42  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:24:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <test_common.h>
#include <faultstat.h>

#if defined(mips) || defined(__mips) || defined(_AIX)|| defined(__hpux) || defined(__OSF__) || defined(SNI_SVR4)
#define DIV_BY_ZERO RAISE(exc_e_intdiv)
#endif

#ifndef DIV_BY_ZERO
#define DIV_BY_ZERO b = a/zero
#endif

int a = 1;
int zero = 0;
int b;

error_status_t s_setup(handle_t h)
{
    return error_status_ok;
}

error_status_t s_fr_cr_op(/* [in] */handle_t bad_h)
{}

error_status_t s_fp_cr_op (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *st
)
{}

error_status_t s_fep_cr_op (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *fest
)
{
    *fest = error_status_ok;
    return error_status_ok;
}

error_status_t s_fr_cp_op (
    /* [in] */ handle_t bad_h,
    /* [out] */error_status_t *st
)
{}

void s_fdp_cp_op (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *cst,
    /* [out] */error_status_t *fst
)
{}

void s_fsp_cp_op (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *st
)
{
}

void s_fep_cp_op (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *st,
    /* [out] */error_status_t *fest
)
{
}

error_status_t s_fr_cep_op (
    /* [in] */handle_t bad_h,
    /* [out] */ error_status_t *st
)
{
    *st = error_status_ok;
    return error_status_ok;
}

void s_fp_cep_op (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *fst,
    /* [out] */error_status_t *cest
)
{}

void s_fsp_cep_op (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *fst
)
{
}

void s_fdp_cep_op (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *fst,
    /* [out] */error_status_t *cest
)
{
}

error_status_t s_fr_cr_dz(/* [in] */handle_t bad_h)
{
    DIV_BY_ZERO;
    return error_status_ok;
}

error_status_t s_fp_cr_dz (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *st
)
{
    DIV_BY_ZERO;
    return error_status_ok;
}

error_status_t s_fep_cr_dz (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *st
)
{
    DIV_BY_ZERO;
}

error_status_t s_fr_cp_dz (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *st
)
{
    DIV_BY_ZERO;
    return error_status_ok;
}

void s_fdp_cp_dz (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *cst,
    /* [out] */error_status_t *fst
)
{
    DIV_BY_ZERO;
}

void s_fsp_cp_dz (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *st
)
{
    DIV_BY_ZERO;
}

void s_fep_cp_dz (
    /* [in] */handle_t bad_h,
    /* [out] */ error_status_t *cst,
    /* [out] */ error_status_t *fest
)
{
    DIV_BY_ZERO;
}

error_status_t s_fr_cep_dz (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *st
)
{
    DIV_BY_ZERO;
    return error_status_ok;
}

void s_fp_cep_dz (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *fst,
    /* [out] */error_status_t *cest
)
{
    DIV_BY_ZERO;
}

void s_fsp_cep_dz (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *st
)
{
    DIV_BY_ZERO;
}

void s_fdp_cep_dz (
    /* [in] */handle_t bad_h,
    /* [out] */error_status_t *fst,
    /* [out] */error_status_t *cest
)
{
    DIV_BY_ZERO;
}

globaldef faultstat_v0_0_epv_t faultstat_v0_0_m_epv = {
    s_setup,
    s_fr_cr_op, s_fp_cr_op, s_fep_cr_op,
    s_fr_cp_op, s_fdp_cp_op, s_fsp_cp_op, s_fep_cp_op,
    s_fr_cep_op, s_fp_cep_op, s_fsp_cep_op, s_fdp_cep_op,
    s_fr_cr_dz, s_fp_cr_dz, s_fep_cr_dz,
    s_fr_cp_dz, s_fdp_cp_dz, s_fsp_cp_dz, s_fep_cp_dz,
    s_fr_cep_dz, s_fp_cep_dz, s_fsp_cep_dz, s_fdp_cep_dz
};
