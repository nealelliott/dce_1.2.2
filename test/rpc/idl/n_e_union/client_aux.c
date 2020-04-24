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
 * Revision 1.1.4.2  1996/02/17  23:15:05  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:08  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:25:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:13:48  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:28:26  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:16:04  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <n_e_union.h>
#include <test_common.h>

#define COOKIE 0x01234567

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    int failures = 0;
    n_e_union_t neu;
    idl_boolean b;
    cont_neu_t cneu;
    multi_neu_t mneu;
    idl_long_int res;
    idl_long_int *unique_ptr;
    s1 a1[ARRAY_SIZE];
    char *getenvres;

    
    neu.l = COOKIE;
    param_by_value( h, &neu, ndr_false, &res );
    if (res != COOKIE * 16)
    {
        printf("param_by_value error - res = %08x\n", neu.l);
        failures++;
    }
    neu.l = res;

    b = ndr_false;
    param_by_ref( h, &neu, &b );
    if ( ! b )
    {
        printf("param_by_ref - b not true\n");
        failures++;
    }
    else if (*(neu.p_l) != COOKIE)
    {
        printf("param_by_ref error - *neu.p_l = %08x\n", *(neu.p_l));
        failures++;
    }

    cneu.neu.p_l = (idl_long_int *)malloc(sizeof(idl_long_int));
    *(cneu.neu.p_l) = COOKIE;
    cneu.b = ndr_true;
    in_struct( h, &cneu );
    if ( cneu.b )
    {
        printf("in_struct error - cneu.b is true\n");
        failures++;
    }
    else if (cneu.neu.l != COOKIE)
    {
        printf("in_struct error - cneu.neu.l = %08x\n", cneu.neu.l);
        failures++;
    }

    
    neu.l = COOKIE;
    ptr_union_val_discrim( h, &neu, ndr_false, &res );
    if (res != COOKIE * 16)
    {
        printf("ptr_union_val_discrim error - res = %08x\n", neu.l);
        failures++;
    }
    neu.l = res;

    b = ndr_false;
    ptr_union_ref_discrim( h, &neu, &b );
    if ( ! b )
    {
        printf("ptr_union_ref_discrim - b not true\n");
        failures++;
    }
    else if (*(neu.p_l) != COOKIE)
    {
        printf("ptr_union_ref_discrim error - *neu.p_l = %08x\n", *(neu.p_l));
        failures++;
    }


    /* Call 1: non-NULL -> NULL */
    b = ndr_true;
    union_unique_ref_discrim( h, &neu, &b );
    if ( ! b )
    {
        printf("union_unique_ref_discrim(1) - b not true\n");
        failures++;
    }
    else
    {
        if (neu.p_l != NULL)
        {
            printf("union_unique_ref_discrim(1) error - unique pointer non-NULL after call\n");
            failures++;
        }
    }
    /* Call 2: NULL -> non-NULL */
    b = ndr_true;
    neu.p_l = NULL;
    union_unique_ref_discrim( h, &neu, &b );
    if ( ! b )
    {
        printf("union_unique_ref_discrim(2) - b not true\n");
        failures++;
    }
    else
    {
        if (neu.p_l == NULL)
        {
            printf("union_unique_ref_discrim(2) error - unique pointer NULL after call\n");
            failures++;
        }
        else if (*(neu.p_l) != COOKIE * 16)
        {
            printf("union_unique_ref_discrim(2) error - *neu.p_l = %08x\n", *(neu.p_l));
            failures++;
        }
    }
    /* Call 3: non-NULL -> non-NULL */
    b = ndr_true;
    unique_ptr = neu.p_l;
    union_unique_ref_discrim( h, &neu, &b );
    if ( ! b )
    {
        printf("union_unique_ref_discrim(3) - b not true\n");
        failures++;
    }
    else
    {
#if 0   /* We added the restriction that unions cannot contain unique pointers, so this check is no longer applicable */
        if (neu.p_l != unique_ptr)
        {
            printf("union_unique_ref_discrim(3) error - unique pointer changed\n");
            failures++;
        }
#endif 
        if (*(neu.p_l) != COOKIE)
        {
            printf("union_unique_ref_discrim(3) error - *neu.p_l = %08x\n", *(neu.p_l));
            failures++;
        }
    }

    mneu.neu.p_l = (idl_long_int *)malloc(sizeof(idl_long_int));
    *(mneu.neu.p_l) = COOKIE * 2;
    mneu.p_neu = (n_e_union_t *)malloc(sizeof(n_e_union_t));
    mneu.p_neu->l = COOKIE * 4;
    mneu.c = false;
    mneu.b = true;
    in_multi( h, &mneu );
    if ( ! mneu.c )
    {
        printf("in_multi - mneu.c false\n");
        failures++;
    }
    if ( mneu.b )
    {
        printf("in_multi - mneu.b truee\n");
        failures++;
    }
    if (mneu.neu.l != COOKIE)
    {
        printf("in_multi - mneu.neu.l = %08x\n", mneu.neu.l);
        failures++;
    }
    if (*(mneu.p_neu->p_l) != (COOKIE * 16))
    {
        printf("in_multi - *(mneu.p_neu.p_l) = %08x\n", *(mneu.p_neu->p_l));
        failures++;
    }

    a1[0].f1.sh = 64;
    a1[0].e = orange;
    a1[0].f3.s.l = 128;
    a1[0].l = -2;
    a1[1].f1.lo = 65;
    a1[1].e = blue;
    a1[1].f3.lp = (int *)malloc(sizeof(int));
    *(a1[1].f3.lp) = 129;
    a1[1].l = -1;
    a1[2].e = violet;
    a1[2].l = 3;
    array_op(h,a1);
    if (a1[2].f1.sh != 64)
    {
        printf("(a1[2].f1.sh != 64)\n");
        failures++;
    }
    if (a1[2].e != orange)
    {
        printf("(a1[2].e != orange)\n");
        failures++;
    }
    if (a1[1].f3.s.l != 128)
    {
        printf("(a1[1].f3.s.l != 128)\n");
        failures++;
    }
    if (a1[1].l != -2)
    {
        printf("(a1[1].l != -2)\n");
        failures++;
    }
    if (a1[0].f1.lo != 65)
    {
        printf("(a1[0].f1.lo != 65)\n");
        failures++;
    }
    if (a1[0].e != blue)
    {
        printf("(a1[0].e != blue)\n");
        failures++;
    }
    if (*(a1[2].f3.lp) != 129)
    {
        printf("(*(a1[2].f3.lp) != 129)\n");
        failures++;
    }
    if (a1[2].l != -1)
    {
        printf("(a1[2].l != -1)\n");
        failures++;
    }
    if (a1[1].e != violet)
    {
        printf("(a1[1].e != violet)\n");
        failures++;
    }
    if (a1[0].l != 3)
    {
        printf("(a1[0].l != 3)\n");
        failures++;
    }

    return failures;
}
