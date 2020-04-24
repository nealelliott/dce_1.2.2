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
 * Revision 1.1.4.2  1996/02/17  23:15:07  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:09  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:25:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:13:49  root]
 * 
 * Revision 1.1.2.3  1993/08/27  20:37:57  tom
 * 	Bug 8520 - Fix rios compile errors: add prototypes (depending on
 * 	IDL_PROTOTYPES) for each routine.
 * 	[1993/08/27  20:19:11  tom]
 * 
 * Revision 1.1.2.2  1993/08/11  16:28:30  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:16:07  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <n_e_union.h>
#include <test_common.h>

#ifdef IDL_PROTOTYPES
static void s_param_by_value(
	handle_t h,
	n_e_union_t *p_neu,
	idl_boolean b,
	idl_long_int *res
)
#else
static void s_param_by_value(h, p_neu, b, res)
handle_t h;
n_e_union_t *p_neu;
idl_boolean b;
idl_long_int *res;
#endif
{
    *res =p_neu->l * 16;
}

#ifdef IDL_PROTOTYPES
static void s_param_by_ref(
	handle_t h,
	n_e_union_t *p_neu,
	idl_boolean *p_b
)
#else
static void s_param_by_ref(h, p_neu, p_b)
handle_t h;
n_e_union_t *p_neu;
idl_boolean *p_b;
#endif
{
    idl_long_int work;

    work = p_neu->l;
    *p_b = ndr_true;
    p_neu->p_l = (idl_long_int *)rpc_ss_allocate(sizeof(idl_long_int));
    *(p_neu->p_l) = work / 16;
}

#ifdef IDL_PROTOTYPES
static void s_in_struct(
	handle_t h,
	cont_neu_t *p_cneu
)
#else
static void s_in_struct(h, p_cneu)
handle_t h;
cont_neu_t *p_cneu;
#endif
{
    p_cneu->b = ndr_false;
    p_cneu->neu.l = *(p_cneu->neu.p_l);
}

#ifdef IDL_PROTOTYPES
static void s_ptr_union_val_discrim(
	handle_t h,
	n_e_union_t *p_neu,
	idl_boolean b,
	idl_long_int *res
)
#else
static void s_ptr_union_val_discrim(h, p_neu, b, res)
handle_t h;
n_e_union_t *p_neu;
idl_boolean b;
idl_long_int *res;
#endif
{
    *res = p_neu->l * 16;
}

#ifdef IDL_PROTOTYPES
static void s_ptr_union_ref_discrim(
	handle_t h,
	n_e_union_t *p_neu,
	idl_boolean *p_b
)
#else
static void s_ptr_union_ref_discrim(h, p_neu, p_b)
handle_t h;
n_e_union_t *p_neu;
idl_boolean *p_b;
#endif
{
    idl_long_int work;

    work = p_neu->l;
    *p_b = ndr_true;
    p_neu->p_l = (idl_long_int *)rpc_ss_allocate(sizeof(idl_long_int));
    *(p_neu->p_l) = work / 16;
}

static int call_cnt = 0;
static idl_long_int save_val;
#ifdef IDL_PROTOTYPES
static void s_union_unique_ref_discrim(
	handle_t h,
	n_e_union_unique_t *p_neu,
	idl_boolean *p_b
)
#else
static void s_union_unique_ref_discrim(h, p_neu, p_b)
handle_t h;
n_e_union_unique_t *p_neu;
idl_boolean *p_b;
#endif
{
    /*
     * Can't do discriminator change to/from unique arm, but can change the
     * pointer non-NULL->NULL, NULL->non-NULL, or non-NULL->non-NULL.  In the
     * last case, the pointer change is not reflected on the caller but the
     * pointee change is.
     */
    switch (call_cnt++ % 3)
    {
    case 0:
        save_val = *p_neu->p_l;
        p_neu->p_l = NULL;
        break;
    case 1:
        p_neu->p_l = (idl_long_int *)rpc_ss_allocate(sizeof(idl_long_int));
        *(p_neu->p_l) = save_val * 16;
        break;
    case 2:
        save_val = *p_neu->p_l;
        p_neu->p_l = (idl_long_int *)rpc_ss_allocate(sizeof(idl_long_int));
        *(p_neu->p_l) = save_val / 16;
        save_val = 0;
    }
}

#ifdef IDL_PROTOTYPES
static void s_in_multi(
	handle_t h,
	multi_neu_t *p_mneu
)
#else
static void s_in_multi( h, p_mneu )
handle_t h;
multi_neu_t *p_mneu;
#endif
{
    idl_long_int work;

    p_mneu->b = false;
    p_mneu->neu.l = (*(p_mneu->neu.p_l) / 2);
    p_mneu->c = true;
    work = p_mneu->p_neu->l;
    p_mneu->p_neu->p_l = (idl_long_int *)rpc_ss_allocate(sizeof(idl_long_int));
    *(p_mneu->p_neu->p_l) = work * 4;
}

#ifdef IDL_PROTOTYPES
static void s_array_op(
	handle_t h,
	s1 a1[ARRAY_SIZE]
)
#else
static void s_array_op( h, a1 )
handle_t h;
s1 a1[ARRAY_SIZE];
#endif
{
    u2 work_u2;
    enum_t work_e;
    u5 work_u5;
    int work_l;

    work_u2 = a1[0].f1;
    a1[0].f1 = a1[1].f1;
    a1[1].f1 = a1[2].f1;
    a1[2].f1 = work_u2;
    work_e = a1[0].e;
    a1[0].e = a1[1].e;
    a1[1].e = a1[2].e;
    a1[2].e = work_e;
    work_u5 = a1[0].f3;
    a1[0].f3 = a1[2].f3;
    a1[2].f3 = a1[1].f3;
    a1[1].f3 = work_u5;
    work_l = a1[0].l;
    a1[0].l = a1[2].l;
    a1[2].l = a1[1].l;
    a1[1].l = work_l;
}

/*globaldef*/ n_e_union_v0_0_epv_t n_e_union_v0_0_m_epv =
{
    s_param_by_value,
    s_param_by_ref,
    s_in_struct,
    s_ptr_union_val_discrim,
    s_ptr_union_ref_discrim,
    s_union_unique_ref_discrim,
    s_in_multi,
    s_array_op
};


