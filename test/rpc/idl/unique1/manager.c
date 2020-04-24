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
 * Revision 1.1.4.2  1996/02/17  23:18:16  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:14  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:37  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:30:04  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:17:20  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <unique1.h>
#include <test_common.h>

void s_op0 ( h, f, l, uap )
handle_t h;
idl_long_int *f;
idl_long_int *l;
u_a_ptr uap[ARRAY_SIZE];
{
    idl_long_int i;

    (*l)++;
    (*f)++;
    for (i = *l; i >= *f; i--)
        uap[i] = uap[i - 1];
}

void s_op1 ( h, s, f, l, uap )
handle_t h;
idl_long_int s;
idl_long_int *f;
idl_long_int *l;
u_a_ptr uap[];
{
    idl_long_int i;

    (*l)--;
    (*f)--;
    for (i = *f; i <= *l; i++)
        uap[i] = uap[i + 1];
}

void s_op2 ( h, f, l, us )
handle_t h;
idl_long_int *f;
idl_long_int *l;
u_struct us[UA_SIZE];
{
    idl_long_int i;

    (*f)--;
    (*l)++;
    for ( i = *f; i <= *l; i++ )
    {
        us[i].u1 = (a_struct *)malloc(sizeof(a_struct));
        us[i].u1->f1 = i * i;
        us[i].u1->f2 = (-(i * i));
    }
}

void s_op3 ( h, s, f, l, us )
handle_t h;
idl_long_int s;
idl_long_int *f;
idl_long_int *l;
u_struct us[];
{
    idl_long_int i;

    (*f)--;
    (*l)++;
    for ( i = *f; i <= *l; i++ )
    {
        us[i].u1 = (a_struct *)malloc(sizeof(a_struct));
        us[i].u1->f1 = i * i;
        us[i].u1->f2 = (-(i * i));
    }
}

void s_op4 ( h, f, l, rup )
handle_t h;
idl_long_int *f;
idl_long_int *l;
r_u_ptr rup[ARRAY_SIZE];
{
    idl_long_int i;

    (*f)--;
    (*l)++;
    for ( i = *f; i <= *l; i++ )
    {
        rup[i]->u1 = (a_struct *)malloc(sizeof(a_struct));
        rup[i]->u1->f1 = i * i;
        rup[i]->u1->f2 = (-(i * i));
    }
}

void s_op5 ( h, s, f, l, rup )
handle_t h;
idl_long_int s;
idl_long_int *f;
idl_long_int *l;
r_u_ptr rup[];
{
    idl_long_int i;

    (*f)--;
    (*l)++;
    for ( i = *f; i <= *l; i++ )
    {
        rup[i]->u1 = (a_struct *)malloc(sizeof(a_struct));
        rup[i]->u1->f1 = i * i;
        rup[i]->u1->f2 = (-(i * i));
    }
}

/*globaldef*/ unique1_v0_0_epv_t unique1_v0_0_m_epv = {
    s_op0,
    s_op1,
    s_op2,
    s_op3,
    s_op4,
    s_op5
};

