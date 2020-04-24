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
 * Revision 1.1.4.2  1996/02/17  23:14:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:53  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:07:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:14  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:26:31  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:14:36  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <ichar2.h>
#include <test_common.h>

/*
** Implementation of Remote Procedures for ichar2
*/

void s_op0
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in] */ /* Type must appear in user header or IDL */ ltype x_char,
    /* [in] */ /* Type must appear in user header or IDL */ ltype *p_char,
    /* [in] */ tbs_t *p_tbs,
    /* [in] */ idl_long_int sz,
    /* [out] */ /* Type must appear in user header or IDL */ ltype c_arr[]
)
#else
(h, x_char, p_char, p_tbs, sz, c_arr)
#endif

#ifndef IDL_PROTOTYPES
    handle_t h;
    /* Type must appear in user header or IDL */ ltype x_char;
    /* Type must appear in user header or IDL */ ltype *p_char;
    tbs_t *p_tbs;
    idl_long_int sz;
    /* Type must appear in user header or IDL */ ltype c_arr[];
#endif
{
    c_arr[0] = p_tbs->v;
    c_arr[1] = p_tbs->w;
    c_arr[2] = x_char;
    c_arr[3] = *p_char;
}

void s_op1
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in] */ a_t (*p_a),
    /* [in, out] */ u_t *p_u
)
#else
(h, p_a, p_u)
#endif

#ifndef IDL_PROTOTYPES
    handle_t h;
    a_t (*p_a);
    u_t *p_u;
#endif
{
    mtype work;
    int i;

    if (p_u->is_array)
        RAISE(rpc_x_invalid_tag);
    work.row = p_u->arms.scalar.column;
    work.column = p_u->arms.scalar.row;
    p_u->is_array = idl_true;
    p_u->arms.array[OASIZE-1] = work;
    for (i=0; i<IASIZE; i++)
    {
        work.row = ((mtype *)p_a)[i].column;
        work.column = ((mtype *)p_a)[i].row;
        p_u->arms.array[i] = work;
    }
}

void s_op2
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in] */ is2_t *is2,
    /* [out] */ os2_t *os2
)
#else
(h, is2, os2)
#endif

#ifndef IDL_PROTOTYPES
    handle_t h;
    is2_t *is2;
    os2_t *os2;
#endif
{
    int i;
    ltype *p_iu;
    ltype *p_ou;
    ltype *p_ir;
    ltype *p_or;
    ltype *p_oc;

    os2->p_sc = is2->p_sc;
    p_iu = (ltype *)is2->p_ua;
    p_ou = (ltype *)os2->p_ua;
    p_ir = (ltype *)is2->p_ra;
    p_or = (ltype *)os2->p_ra;
    p_oc = (ltype *)os2->p_ca;
    for (i=0; i<A2SIZE; i++)
    {
        p_ou[i] = p_iu[i];
        p_or[i] = p_ir[i];
        p_oc[i] = is2->ca[i];
    }
}

void s_op3
#ifdef IDL_PROTOTYPES
(
    /* [in] */ handle_t h,
    /* [in] */ nc_arptr_t ara[5],
    /* [in, out] */ nc_uptr_t a2u[5][5]
)
#else
(h, ara, a2u)
#endif
#ifndef IDL_PROTOTYPES
    handle_t h;
    nc_arptr_t ara[5];
    nc_uptr_t a2u[5][5];
#endif
{
    int i,j;

    for(i=0; i<IASIZE; i++)
    {
        for(j=0; j<IASIZE; j++)
        {
            a2u[i][j] = (nc_uptr_t)rpc_ss_allocate(sizeof(ntype));
            *(a2u[i][j]) = (*ara[i])[j];
        }
    }
}


/*globaldef*/ ichar2_v0_0_epv_t ichar2_v0_0_m_epv = {
    s_op0,
    s_op1,
    s_op2,
    s_op3
};
