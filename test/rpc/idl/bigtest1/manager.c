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
 * Revision 1.1.4.2  1996/02/17  23:12:45  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:50  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:57:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:41  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:08:11  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:27:50  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <bigtest1.h>
#include <test_common.h>
 
void transform (p_x)
x_union_t *p_x;
{
    unsigned long array_size;
    long *p_array_elt;
    int i;
    long array_sum;

    if (p_x->discrim == ptr_k)
    {
        array_size = p_x->ub.ptr->s;
        p_array_elt = p_x->ub.ptr->a;
        array_sum = 0;
        for (i=0; i<array_size; i++)
        {
            array_sum += (*p_array_elt);
            p_array_elt++;
        }
        p_x->discrim = sum_k;
        p_x->ub.sum = array_sum;
    }
    else if (p_x->discrim == count_k)
    {
        array_size = p_x->ub.count;
        p_x->discrim = ptr_k;
        p_x->ub.ptr = (confstruct_t *) rpc_ss_allocate
              ( sizeof(confstruct_t) + (array_size-1) * sizeof(ndr_long_int) );
        p_x->ub.ptr->s = array_size;
        p_array_elt = p_x->ub.ptr->a;
        for (i=0; i<array_size; i++)
        {
            *p_array_elt = 100 * i;
            p_array_elt++;
        }
    }
}

void changevars(p_varstruct)
varstruct_t *p_varstruct;
{
    int i;

    for ( i=p_varstruct->f; i<=p_varstruct->l; i++ )
    {
        transform(&p_varstruct->x[i]);
    }
}

void his_op0
( h, ptrtosum, counttoptr )
    /* [in] */handle_t h;
    /* [in, out] */varstruct_t *ptrtosum[5][5];
    /* [in, out] */varstruct_t *counttoptr[5][5];
{
    int i,j;
    for (i=0; i<5; i++)
    {
        for (j=0; j<5; j++)
        {
            changevars(ptrtosum[i][j]);
            changevars(counttoptr[i][j]);
        }
    }

}

globaldef bigtest1_v0_0_epv_t bigtest1_v0_0_m_epv = {his_op0};
