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
 * Revision 1.1.4.2  1996/02/17  23:14:53  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:03  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:08:49  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/29  21:13 UTC  dat
 * 	Remove fabs function
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/19  22:03 UTC  hopkins
 * 	merging hidesyms work
 * 
 * 	HP revision /main/hopkins_hidesyms/1  1994/12/18  04:56 UTC  hopkins
 * 	tmp fix for missing fabs()
 * 	[1995/12/11  19:32:43  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:27:52  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:15:38  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <math.h>
#include <mtsaconf.h>
#include <test_common.h>

/*
static double fabs( double x );
*/

#define CDIM1 (UDIM1 - LDIM1 + 1)
#define CDIM2 (UDIM2 - LDIM2 + 1)
#define CDIM3 (UDIM3 - LDIM3 + 1)

int client_aux(h,passes)
    int passes;      handle_t h;
{
    int i,j,k;
    idl_long_int fa3[CDIM1][CDIM2][CDIM3];
    idl_small_int *a1;
    idl_long_int s1;
    idl_short_int f1,f2,f3,l1,l2,l3;
    int sl2, sl3;
    three_smalls *a2, *a2ptr;
    unsigned int wrong_at_server;
    multiconf_t *p_mc;
    idl_short_float *fl_a, *flptr1, *flptr2;
    multiopen_t *p_mo;
    idl_long_float *db_a, *dbptr1, *dbptr2;
    int failures = 0;
    strptr_t strptr_a1[PMAXI-PMINI+1];
    strptr_t strptr_a2[PMAXO-PMINO+1];



    for (i=0; i<CDIM1; i++)
    {
        for (j=0; j<CDIM2; j++)
        {
            for (k=0; k<CDIM3; k++)
            {
                fa3[i][j][k] = 65536*i + 256*j + k;
            }
        }
    }
    op0( h, fa3 );
    for (i=0; i<CDIM1; i++)
    {
        for (j=0; j<CDIM2; j++)
        {
            for (k=0; k<CDIM3; k++)
            {
                if( fa3[i][j][k] != -(65536*i + 256*j + k) )
                    failures++;
            }
        }
    }
    printf("op0 completed - %d failures\n", failures);

    s1 = 9;
    a1 = (idl_small_int *)malloc(s1 * sizeof(idl_small_int));
    f1 = 6;
    for (i=0; i<s1; i++)
    {
        a1[i] = i + LDIM1;
    }
    op1( h, a1, s1, &f1);
    if (f1 != 5)
        failures++;
    for (i=f1-LDIM1; i<s1; i++)
    {
        if(a1[i] != -(i + LDIM1))
            failures++;
    }
    free(a1);
    printf("op1 completed - %d failures\n", failures);

    sl3 = UDIM3 - LDIM3 + 1;
    sl2 = (UDIM2 - LDIM2 + 1) * sl3;
    a2 = (three_smalls *)malloc( (UDIM1 - LDIM1 + 1) * sl2 * sl3
                                        * sizeof(three_smalls) );
    a2ptr = a2;
    for (i = LDIM1; i <= UDIM1; i++)
    {
        for (j = LDIM2; j <= UDIM2; j++)
        {
            for (k = LDIM3; k <= UDIM3; k++)
            {
                a2ptr->x = i;
                a2ptr->y = j;
                a2ptr->z = k;
                a2ptr++;
            }
        }
    }
    op2(h, (idl_long_int)LDIM1, (idl_long_int)UDIM2,
        a2, (idl_long_int)LDIM3, (idl_long_int)UDIM3, &wrong_at_server);
    if (wrong_at_server != 0)
    {
        printf("%d wrong values at server\n", wrong_at_server);
        failures += wrong_at_server;
    }
    a2ptr = a2;
    for (i = LDIM1; i <= UDIM1; i++)
    {
        for (j = LDIM2; j <= UDIM2; j++)
        {
            for (k = LDIM3; k <= UDIM3; k++)
            {
                if (a2ptr->x != -i)
                    failures++;
                if (a2ptr->y != -j)
                    failures++;
                if (a2ptr->z != -k)
                    failures++;
                a2ptr++;
            }
        }
    }
    printf("op2 completed - %d failures\n", failures);

    f1 = LDIM1 + 2;
    l1 = f1 + 1;
    f2 = LDIM2 + 2;
    l2 = f2 + 1;
    f3 = LDIM3 + 2;
    l3 = f3 + 1;
    op3(h, (idl_long_int)LDIM1, (idl_long_int)UDIM2,
         (idl_long_int)LDIM3, (idl_long_int)UDIM3, a2, &f1, &f2, &f3,
         &l1, &l2, &l3, &wrong_at_server);
    if (wrong_at_server != 0)
    {
        printf("%d wrong values at server\n", wrong_at_server);
        failures += wrong_at_server;
    }
    if (f1 != LDIM1 + 1)
    {
        printf("f1 = %d\n", f1);
        failures++;
    }
    if (l1 != f1 + 1)
    {
        printf("l1 = %d\n", l1);
        failures++;
    }
    if (f2 != LDIM2 + 1)
    {
        printf("f2 = %d\n", f2);
        failures++;
    }
    if (l2 != f2 + 1)
    {
        printf("l2 = %d\n", l2);
        failures++;
    }
    if (f3 != LDIM3 + 1)
    {
        printf("f3 = %d\n", f3);
        failures++;
    }
    if (l3 != f3 + 1)
    {
        printf("l3 = %d\n", l3);
        failures++;
    }
    for(i = f1; i <= l1; i++)
    {
        for (j = f2; j <= l2; j++)
        {
            for (k = f3; k <= l3; k++)
            {
                a2ptr = a2 + (i-(LDIM1))*sl2 + (j-(LDIM2))*sl3 + (k-(LDIM3));
                if (a2ptr->x != -(i+1))
                    failures++;
                if (a2ptr->y != -(j+1))
                    failures++;
                if (a2ptr->z != -(k+1))
                    failures++;
            }
        }
    }
    free(a2);
    printf("op3 completed - %d failures\n", failures);

    p_mc = (multiconf_t *)malloc( sizeof(multiconf_t)
            + ((UDIM1-(LDIM1)+1)*(UDIM2-(LDIM2)+1)-1)*sizeof(idl_short_float) );
    p_mc->min1 = LDIM1;
    p_mc->max1 = UDIM1;
    p_mc->min2 = LDIM2;
    p_mc->max2 = UDIM2;
    flptr1 = &(p_mc->a);
    for (i = LDIM1; i <= UDIM1; i++)
    {
        for (j = LDIM2; j <= UDIM2; j++)
        {
            *flptr1 = (float)((10.0*i) + (0.1*j));
            flptr1++;
        }
    }
    fl_a = (idl_short_float *)malloc(
                (UDIM1-(LDIM1)+1)*(UDIM2-(LDIM2)+1)*sizeof(idl_short_float) );
    op4( h, p_mc, LDIM1, UDIM1, LDIM2, UDIM2, fl_a );
    flptr1 = &(p_mc->a);
    flptr2 = fl_a;
    for (i = LDIM1; i <= UDIM1; i++)
    {
        for (j = LDIM2; j <= UDIM2; j++)
        {
            if (*flptr1 != *flptr2)
            {
                printf("fl_a[%d,%d] = %f - expected %f\n",
                    i, j, *flptr2, *flptr1);
                failures++;
            }
            flptr1++;
            flptr2++;
        }
    }
    free(p_mc);
    free(fl_a);
    printf("op4 completed - %d failures\n", failures);

    p_mo = (multiopen_t *)malloc( sizeof(multiopen_t)
            + ((UDIM1-(LDIM1)+1)*(UDIM2-(LDIM2)+1)-1)*sizeof(idl_long_float) );
    p_mo->min1 = LDIM1;
    p_mo->max1 = UDIM1;
    p_mo->min2 = LDIM2;
    p_mo->max2 = UDIM2;
    p_mo->first1 = LDIM1 + 2;
    p_mo->last1 = UDIM1 - 1;
    p_mo->first2 = LDIM2 + 2;
    p_mo->last2 = UDIM2 - 1;
    dbptr1 = &(p_mo->d);
    for (i = LDIM1; i <= UDIM1; i++)
    {
        for (j = LDIM2; j <= UDIM2; j++)
        {
            if ( (i>=p_mo->first1) && (i<=p_mo->last1)
                && (j>=p_mo->first2) && (j<=p_mo->last2) )
            {
                *dbptr1 = (10.0*i) + (0.1*j);
            }
            dbptr1++;
        }
    }
    db_a = (idl_long_float *)malloc(
                (UDIM1-(LDIM1)+1)*(UDIM2-(LDIM2)+1)*sizeof(idl_long_float) );
    op5( h, p_mo, db_a, LDIM1, UDIM1, LDIM2, UDIM2, &f1, &f2, &l1, &l2 );
    if (f1 != LDIM1 + 1)
    {
        printf("%d is incorrect for f1\n", f1);
        failures++;
    }
    if (l1 != UDIM1 - 2)
    {
        printf("%d is incorrect for l1\n", l1);
        failures++;
    }
    if (f2 != LDIM2 + 1)
    {
        printf("%d is incorrect for f2\n", f2);
        failures++;
    }
    if (l2 != UDIM2 - 2)
    {
        printf("%d is incorrect for l2\n", l2);
        failures++;
    }
    dbptr2 = db_a;
    for (i = LDIM1; i <= UDIM1; i++)
    {
        for (j = LDIM2; j <= UDIM2; j++)
        {
            if ( (i>=f1) && (i<=l1) && (j>=f2) && (j<=l2) )
            {
                if ( fabs(*dbptr2 - ((10.0*(i+1)) + (0.1*(j+1)))) > 1E-9 )
                {
                    printf("Error - db_a[%d.%d] = %f\n", i, j, *dbptr2);
                }
            }
            dbptr2++;
        }
    }
    free(p_mo);
    free(db_a);
    printf("op5 completed - %d failures\n", failures);

    strptr_a1[0] = (idl_char *)"A partridge in a pear tree";
    strptr_a1[1] = (idl_char *)"Two turtle doves";
    strptr_a1[2] = (idl_char *)"Three French hens";
    strptr_a1[3] = (idl_char *)"Four calling birds";
    strptr_a1[4] = (idl_char *)"Five gold rings";
    strptr_a1[5] = (idl_char *)"Six geese a-laying";
    strptr_a1[6] = (idl_char *)"Seven swans a-swimming";
    strptr_a1[7] = (idl_char *)"Eight maids a-milking";
    strptr_a1[8] = (idl_char *)"Nine ladies dancing";
    strptr_a1[9] = (idl_char *)"Ten lords a-leaping";
    op6( h, strptr_a1, PMINI, PMAXI, strptr_a2, PMINO, PMAXO,
                    &f1, &l1 );
    if (f1 != PMINI+PMINO)
    {
        printf("op6 - f1 = %d\n", f1);
        failures++;
    }
    if (l1 != PMAXI+PMINO)
    {
        printf("op6 - l1 = %d\n", l1);
        failures++;
    }
    for ( i=f1; i<=l1; i++ )
    {
        if (strcmp(strptr_a2[i-PMINO], strptr_a1[i-f1]) != 0)
        {
            printf("ar_str_ptr_op - strptr_a2[%d] = %s\n",
                     i, strptr_a2[i-PMINO]);
            failures++;
        }
        free(strptr_a2[i-PMINO]);
    }
    printf("op6 completed - %d failures\n", failures);

    return failures;
}

/*
static double
fabs( double x )
{
	return( x >= 0 ? x : -x );
}
*/
