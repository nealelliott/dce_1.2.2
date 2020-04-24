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
 * Revision 1.1.4.2  1996/02/17  23:14:56  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:04  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:08:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:44  root]
 * 
 * Revision 1.1.2.3  1993/08/27  20:37:55  tom
 * 	Bug 8519 - Fix compile problems in rios:
 * 	 Use idl types instead of int/short.
 * 	 Add a few casts where needed.
 * 	[1993/08/27  20:19:07  tom]
 * 
 * Revision 1.1.2.2  1993/08/11  16:27:56  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:15:41  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <mtsaconf.h>
#include <test_common.h>


#define CDIM1 (UDIM1 - LDIM1 + 1)
#define CDIM2 (UDIM2 - LDIM2 + 1)
#define CDIM3 (UDIM3 - LDIM3 + 1)

static void s_op0(h, fa3)
handle_t h;
idl_long_int fa3[CDIM1][CDIM2][CDIM3];
{
    int i,j,k;

    for (i=0; i<CDIM1; i++)
    {
        for (j=0; j<CDIM2; j++)
        {
            for (k=0; k<CDIM3; k++)
            {
                fa3[i][j][k] = -fa3[i][j][k];
            }
        }
    }
}

static void s_op1(h, a1, s1, p_f1)
handle_t h;
idl_small_int *a1;
idl_long_int s1;
idl_short_int *p_f1;
{
    int i;

    for (i = *p_f1-LDIM1; i<s1; i++)
        a1[i] = -a1[i];

    (*p_f1)--;
    a1[*p_f1-LDIM1] = -(*p_f1);
}

static void s_op2(h, min1, max2, a2, min3, max3, p_wrong_at_server)
handle_t h;
idl_long_int min1;
idl_long_int max2;
three_smalls *a2;
idl_long_int min3;
idl_long_int max3;
idl_ulong_int *p_wrong_at_server;
{
    int i,j,k;
    three_smalls *a2ptr;

    *p_wrong_at_server = 0;
    a2ptr = a2;
    for (i = min1; i <= UDIM1; i++)
    {
        for (j = LDIM2; j <= max2; j++)
        {
            for (k = min3; k <= max3; k++)
            {
                if (a2ptr->x != i)
                    (*p_wrong_at_server)++;
                a2ptr->x = -i;
                if (a2ptr->y != j)
                    (*p_wrong_at_server)++;
                a2ptr->y = -j;
                if (a2ptr->z != k)
                    (*p_wrong_at_server)++;
                a2ptr->z = -k;
                a2ptr++;
            }
        }
    }
}

static void s_op3(h, min1, max2, min3, max3, a2, p_f1, p_f2 ,p_f3,
 p_l1, p_l2, p_l3, p_wrong_at_server)
handle_t h;
idl_long_int min1;
idl_long_int max2;
idl_long_int min3;
idl_long_int max3;
three_smalls *a2;
idl_short_int *p_f1;
idl_short_int *p_f2;
idl_short_int *p_f3;
idl_short_int *p_l1;
idl_short_int *p_l2;
idl_short_int *p_l3;
idl_ulong_int *p_wrong_at_server;
{
    int i,j,k;
    three_smalls *a2srcptr, *a2destptr;
    int sl2, sl3;

    *p_wrong_at_server = 0;
    sl3 = max3 - min3 + 1;
    sl2 = (max2 - LDIM2 + 1) * sl3;
    for(i = *p_f1; i <= *p_l1; i++)
    {
        for (j = *p_f2; j <= *p_l2; j++)
        {
            for (k = *p_f3; k <= *p_l3; k++)
            {
                a2srcptr = a2 + (i-min1)*sl2 + (j-(LDIM2))*sl3 + (k-min3);
                if (a2srcptr->x != -i)
                    (*p_wrong_at_server)++;
                if (a2srcptr->y != -j)
                    (*p_wrong_at_server)++;
                if (a2srcptr->z != -k)
                    (*p_wrong_at_server)++;
                a2destptr =
                         a2 + (i-min1-1)*sl2 + (j-(LDIM2)-1)*sl3 + (k-min3-1);
                *a2destptr = *a2srcptr;
            }
        }
    }
    (*p_f1)--;
    (*p_f2)--;
    (*p_f3)--;
    (*p_l1)--;
    (*p_l2)--;
    (*p_l3)--;
}

static void s_op4( h, p_mc, min1, max1, min2, max2, fl_a )
handle_t h;
multiconf_t *p_mc;
idl_long_int min1;
idl_long_int max1;
idl_long_int min2;
idl_long_int max2;
idl_short_float *fl_a;
{
    int i,j;
    idl_short_float *flptr1, *flptr2;

    flptr1 = (idl_short_float *)&(p_mc->a);
    flptr2 = fl_a;
    for (i=min1; i<=max1; i++)
    {
        for (j=min2; j<=max2; j++)
        {
            *flptr2 = *flptr1;
            flptr1++;
            flptr2++;
        }
    }
}

static void s_op5( h, p_mo, db_a, min1, max1, min2, max2, p_f1, p_f2, p_l1, p_l2 )
handle_t h;
multiopen_t *p_mo;
idl_long_float *db_a;
idl_long_int min1;
idl_long_int max1;
idl_long_int min2;
idl_long_int max2;
idl_short_int *p_f1;
idl_short_int *p_f2;
idl_short_int *p_l1;
idl_short_int *p_l2;
{
    int i,j;
    idl_long_float *dbrowi, *dbcoli, *dbrowo, *dbcolo;

    *p_f1 = p_mo->first1 - 1;
    *p_l1 = p_mo->last1 - 1;
    *p_f2 = p_mo->first2 - 1;
    *p_l2 = p_mo->last2 - 1;
    for (i=p_mo->first1; i<=p_mo->last1; i++)
    {
        dbrowi = (idl_long_float *)&(p_mo->d);
        dbrowi += (i - p_mo->min1) * (p_mo->max2 - p_mo->min2 + 1);
        dbrowo = db_a;
        dbrowo += (i - p_mo->min1 - 1) * (p_mo->max2 - p_mo->min2 + 1);
        for (j=p_mo->first2; j<=p_mo->last2; j++)
        {
            dbcoli = dbrowi + j - p_mo->min2;
            dbcolo = dbrowo + j - p_mo->min2 - 1;
            *dbcolo = *dbcoli;
        }
    }
}

void s_op6(h, strptr_a1, min1, max1, strptr_a2, min2, max2, p_f1, p_l1)
    handle_t h;
    strptr_t *strptr_a1;
    idl_long_int min1;
    idl_long_int max1;
    strptr_t *strptr_a2;
    idl_long_int min2;
    idl_long_int max2;
    idl_short_int *p_f1;
    idl_short_int *p_l1;
{
    int i;

    *p_f1 = min2 + min1;
    *p_l1 = min2 + max1;
    for ( i=0; i<=max1-min1; i++)
    {
        strptr_a2[i+min1] = (strptr_t)rpc_ss_allocate(strlen(strptr_a1[i]+1));
        strcpy(strptr_a2[i+min1], strptr_a1[i]);
    }
}

/*globaldef*/ mtsaconf_v0_0_epv_t mtsaconf_v0_0_m_epv = 
        {s_op0,
         s_op1,
         s_op2,
         s_op3,
         s_op4,
         s_op5,
         s_op6};

