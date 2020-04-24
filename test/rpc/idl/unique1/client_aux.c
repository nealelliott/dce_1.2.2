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
 * Revision 1.1.4.2  1996/02/17  23:18:14  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:13  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:35  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:29:59  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:17:17  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <unique1.h>
#include <test_common.h>

int client_aux(h,passes)
    int passes;rpc_binding_handle_t h;
{
    int failures = 0;
    u_a_ptr uap[ARRAY_SIZE];
    idl_long_int i,f,l;
    u_struct us[UA_SIZE];
    r_u_ptr rup[ARRAY_SIZE];
    char *getenvres;


    for (i=0; i<ARRAY_SIZE; i++)
    {
        if ( (i % 2) == 0 )
            uap[i] = NULL;
        else
        {
            uap[i] = (a_struct *)malloc(sizeof(a_struct));
            uap[i]->f1 = i * (i+1);
            uap[i]->f2 = (-(uap[i]->f1));
        }
    }
    f = 5;
    l = 15;
    op0( h, &f, &l, uap);
    for( i = f; i <= l; i++ )
    {
        if ( (i % 2) == 1 )
        {
            if ( uap[i] != NULL )
            {
                failures++;
                printf("op0 - uap[%d] not NULL\n", i);
            }
        }
        else
        {
            if (uap[i]->f1 != i * (i-1))
            {
                failures++;
                printf("op0 - uap[%d]->f1 = %d\n", i, uap[i]->f1);
            }
            if ( uap[i]->f2 != (-(uap[i]->f1)) )
            {
                failures++;
                printf("op0 - uap[%d]->f2 = %d\n", i, uap[i]->f2);
            }
        }
    }

    op1( h, ARRAY_SIZE, &f, &l, uap);
    for( i = f; i <= l; i++ )
    {
        if ( (i % 2) == 0 )
        {
            if ( uap[i] != NULL )
            {
                failures++;
                printf("op1 - uap[%d] not NULL\n", i);
            }
        }
        else
        {
            if (uap[i]->f1 != i * (i+1))
            {
                failures++;
                printf("op1 - uap[%d]->f1 = %d\n", i, uap[i]->f1);
            }
            if ( uap[i]->f2 != (-(uap[i]->f1)) )
            {
                failures++;
                printf("op1 - uap[%d]->f2 = %d\n", i, uap[i]->f2);
            }
        }
    }

    for ( i = 0; i < UA_SIZE; i++ )
    {
        us[i].u1 = NULL;
        us[i].u2 = (a_struct *)malloc(sizeof(a_struct));
        us[i].u2->f1 = i * (i+1);
        us[i].u2->f2 = (-(us[i].u2->f1));
    }
    f = 3;
    l = 6;
    op2( h, &f, &l, us);
    for ( i = f; i <= l; i++ )
    {
        if ( us[i].u1->f1 != (i * i) )
        {
            failures++;
            printf("op2 - us[%d].u1->f1 = %d\n", i, us[i].u1->f1);
        }
        if ( us[i].u1->f2 != -(i * i) )
        {
            failures++;
            printf("op2 - us[%d].u1->f2 = %d\n", i, us[i].u1->f2);
        }
        if ( (i == f) || (i == l) )
        {
            if ( us[i].u2 != NULL )
            {
                failures++;
                printf("op2 - us[%d].u2 not NULL\n", i);
            }
        }
        else
        {
            if (us[i].u2->f1 != i * (i+1))
            {
                failures++;
                printf("op2 - us[%d].u2->f1 = %d\n", i, us[i].u2->f1);
            }
            if ( us[i].u2->f2 != (-(us[i].u2->f1)) )
            {
                failures++;
                printf("op2 - us[%d].u2->f2 = %d\n", i, us[i].u2->f2);
            }
        }
    }

    for ( i = 0; i < UA_SIZE; i++ )
    {
        us[i].u1 = NULL;
    }
    us[f].u2 = (a_struct *)malloc(sizeof(a_struct));
    us[f].u2->f1 = f * (f+1);
    us[f].u2->f2 = (-(us[f].u2->f1));
    us[l].u2 = (a_struct *)malloc(sizeof(a_struct));
    us[l].u2->f1 = l * (l+1);
    us[l].u2->f2 = (-(us[l].u2->f1));
    op3( h, UA_SIZE, &f, &l, us);
    for ( i = f; i <= l; i++ )
    {
        if ( us[i].u1->f1 != (i * i) )
        {
            failures++;
            printf("op3 - us[%d].u1->f1 = %d\n", i, us[i].u1->f1);
        }
        if ( us[i].u1->f2 != -(i * i) )
        {
            failures++;
            printf("op3 - us[%d].u1->f2 = %d\n", i, us[i].u1->f2);
        }
        if ( (i == f) || (i == l) )
        {
            if ( us[i].u2 != NULL )
            {
                failures++;
                printf("op3 - us[%d].u2 not NULL\n", i);
            }
        }
        else
        {
            if (us[i].u2->f1 != i * (i+1))
            {
                failures++;
                printf("op3 - us[%d].u2->f1 = %d\n", i, us[i].u2->f1);
            }
            if ( us[i].u2->f2 != (-(us[i].u2->f1)) )
            {
                failures++;
                printf("op3 - us[%d].u2->f2 = %d\n", i, us[i].u2->f2);
            }
        }
    }

    for ( i = 0; i < ARRAY_SIZE; i++ )
    {
        rup[i] = (u_struct *)malloc(sizeof(u_struct));
    }

    for ( i = 0; i < ARRAY_SIZE; i++ )
    {
        rup[i]->u1 = NULL;
        rup[i]->u2 = (a_struct *)malloc(sizeof(a_struct));
        rup[i]->u2->f1 = i * (i+1);
        rup[i]->u2->f2 = (-(rup[i]->u2->f1));
    }
    f = 8;
    l = 14;
    op4( h, &f, &l, rup);
    for ( i = f; i <= l; i++ )
    {
        if ( rup[i]->u1->f1 != (i * i) )
        {
            failures++;
            printf("op4 - rup[%d]->u1->f1 = %d\n", i, rup[i]->u1->f1);
        }
        if ( rup[i]->u1->f2 != -(i * i) )
        {
            failures++;
            printf("op4 - rup[%d]->u1->f2 = %d\n", i, rup[i]->u1->f2);
        }
        if ( (i == f) || (i == l) )
        {
            if ( rup[i]->u2 != NULL )
            {
                failures++;
                printf("op4 - rup[%d]->u2 not NULL\n", i);
            }
        }
        else
        {
            if (rup[i]->u2->f1 != i * (i+1))
            {
                failures++;
                printf("op4 - rup[%d]->u2->f1 = %d\n", i, rup[i]->u2->f1);
            }
            if ( rup[i]->u2->f2 != (-(rup[i]->u2->f1)) )
            {
                failures++;
                printf("op4 - rup[%d]->u2->f2 = %d\n", i, rup[i]->u2->f2);
            }
        }
    }

    for ( i = 0; i < ARRAY_SIZE; i++ )
    {
        rup[i]->u1 = NULL;
    }
    rup[f]->u2 = (a_struct *)malloc(sizeof(a_struct));
    rup[f]->u2->f1 = f * (f+1);
    rup[f]->u2->f2 = (-(rup[f]->u2->f1));
    rup[l]->u2 = (a_struct *)malloc(sizeof(a_struct));
    rup[l]->u2->f1 = l * (l+1);
    rup[l]->u2->f2 = (-(rup[l]->u2->f1));
    op5( h, ARRAY_SIZE, &f, &l, rup);
    for ( i = f; i <= l; i++ )
    {
        if ( rup[i]->u1->f1 != (i * i) )
        {
            failures++;
            printf("op5 - rup[%d]->u1->f1 = %d\n", i, rup[i]->u1->f1);
        }
        if ( rup[i]->u1->f2 != -(i * i) )
        {
            failures++;
            printf("op5 - rup[%d]->u1->f2 = %d\n", i, rup[i]->u1->f2);
        }
        if ( (i == f) || (i == l) )
        {
            if ( rup[i]->u2 != NULL )
            {
                failures++;
                printf("op5 - rup[%d]->u2 not NULL\n", i);
            }
        }
        else
        {
            if (rup[i]->u2->f1 != i * (i+1))
            {
                failures++;
                printf("op5 - rup[%d]->u2->f1 = %d\n", i, rup[i]->u2->f1);
            }
            if ( rup[i]->u2->f2 != (-(rup[i]->u2->f1)) )
            {
                failures++;
                printf("op5 - rup[%d]->u2->f2 = %d\n", i, rup[i]->u2->f2);
            }
        }
    }

    return(failures);
}
