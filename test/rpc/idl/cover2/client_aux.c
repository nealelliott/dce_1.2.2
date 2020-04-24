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
 * Revision 1.1.6.2  1996/02/17  23:13:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:15  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:01:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:45  root]
 * 
 * Revision 1.1.4.2  1993/01/07  19:50:53  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:30:48  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/26  19:04:12  harrow
 * 	Remove test for [represent_as] on a conformant struct as it is
 * 	unsupported.
 * 	[1992/05/26  18:58:19  harrow]
 * 
 * Revision 1.1  1992/01/19  03:31:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h> 
#include <cover2.h>
#include <test_common.h>

static c_string is = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static c_string os_ref = "ZYXWVUTSRQPONMLKJIHGFEDCBA";

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    int failures = 0;
    outer outer_parm;
    long h_a[FDIM][FDIM];
    int i,j,f;
    idl_char *cp;
    c_string os_parm;
    r_p_c ap[FDIM][DIM2];
    char a[FDIM][DIM2];
    op3_elt b[FDIM];
    long a4[FDIM];
    long b4[FDIM];
    long c4[FDIM];
    long d4[FDIM];
    long ln4, f4, f24, f34, lt4;
    op5_union u5;
    op6_string s6;
    op7_array a7;
    op8_struct a8[FDIM];

    outer_parm.o_small = 2;
    outer_parm.o_inner.i1 = 3;
    outer_parm.o_inner.i2 = 4;
    outer_parm.o_short = 5;
    for (i=0; i<FDIM; i++)
    {
        for (j=0; j<FDIM; j++)
        {
            h_a[i][j] = i + 10*j;
        }
    }
    a_op0( h, &outer_parm, h_a );
    if (outer_parm.o_small != 4) failures++;
    if (outer_parm.o_inner.i1 != 9) failures++;
    if (outer_parm.o_inner.i2 != 16) failures++;
    if (outer_parm.o_short != 25) failures++;
    for (i=0; i<FDIM; i++)
    {
        for (j=0; j<FDIM; j++)
        {
            if (h_a[i][j] != i*101 + 1010*j) failures++;
        }
    }
    printf("%d failures after a_op0\n", failures);   

#if CONF_REP_AS
    strcpy( os_parm, is );
    a_op1( h, &os_parm );
    if (strcmp(os_parm, os_ref) != 0) failures++;
    printf("%d failures after a_op1\n", failures);   
#endif

    f = FDIM - 13 - 1;
    for ( i=0; i<=13; i++ )
    {
        for( j=0; j<DIM2; j++ )
        {
            ap[f + i][j] = (r_p_c)malloc(sizeof(char));
            *(ap[f +i][j]) = 'A' + i + 13*j;
        }
    }
    a_op2( h, FDIM, (idl_long_int *)&f, ap, a );
    for ( i=f; i<FDIM; i++ )
    {
        for ( j=0; j<DIM2; j++ )
        {
            if (a[i][j] != *(ap[i][j])) failures++;
        }
    }
    printf("%d failures after a_op2\n", failures);   

    for (i=0; i<FDIM; i++)
    {
        b[i].a = i;
        b[i].p_a = (long *)malloc(sizeof(long));
        *(b[i].p_a) = i * 10;
        b[i].p_b = (long *)malloc(sizeof(long));
        *(b[i].p_b) = i * 100;
        b[i].b = i * 1000;
    }
    a_op3( h, b );
    for (i=0; i<FDIM; i++)
    {
        if (b[i].a != i * 2) failures++;
        if (*(b[i].p_a) != i * 30) failures++;
        if (*(b[i].p_b) != i * 400) failures++;
        if (b[i].b != i * 5000) failures++;
    }
    printf("%d failures after a_op3\n", failures);
   
    ln4 = 15;
    for ( i=0; i<ln4; i++)
        a4[i] = i * 16;
    f4 = FDIM - 1;
    f24 = 5;
    for ( i=f24; i<FDIM; i++)
        c4[i] = i *256;
    f34 = 0;
    lt4 = 0;
    a_op4( h, ln4, a4, FDIM-1, &f4, b4, FDIM, f24, c4, &f34, &lt4, d4);
    for (i=0; i<ln4; i++)
    {
        if ( b4[f4 +i] != a4[i] )
             failures++;
    }
    for (i=f34; i<=lt4; i++)
    {
        if ( d4[i] != c4[i+1] )
            failures++;
    }
    printf("%d failures after a_op4\n", failures);

    /*
     * Test the various "escape characters" supported by the IDL compiler.
     * If any of these aren't be handled correctly we should get an
     * "invalid tag" exception.
     */
    TRY
        u5.sw = AUDIBLE_BELL;
        a_op5(h, &u5);
    CATCH(rpc_x_invalid_tag)
        failures++;
    ENDTRY
    TRY
        u5.sw = BACKSPACE;
        a_op5(h, &u5);
    CATCH(rpc_x_invalid_tag)
        failures++;
    ENDTRY
    TRY
        u5.sw = FORMFEED;
        a_op5(h, &u5);
    CATCH(rpc_x_invalid_tag)
        failures++;
    ENDTRY
    TRY
        u5.sw = NEWLINE;
        a_op5(h, &u5);
    CATCH(rpc_x_invalid_tag)
        failures++;
    ENDTRY
    TRY
        u5.sw = RETURN;
        a_op5(h, &u5);
    CATCH(rpc_x_invalid_tag)
        failures++;
    ENDTRY
    TRY
        u5.sw = TAB;
        a_op5(h, &u5);
    CATCH(rpc_x_invalid_tag)
        failures++;
    ENDTRY
    TRY
        u5.sw = VERTICAL_TAB;
        a_op5(h, &u5);
    CATCH(rpc_x_invalid_tag)
        failures++;
    ENDTRY
    TRY
        u5.sw = BACKSLASH;
        a_op5(h, &u5);
    CATCH(rpc_x_invalid_tag)
        failures++;
    ENDTRY
    TRY
        u5.sw = QUOTE;
        a_op5(h, &u5);
    CATCH(rpc_x_invalid_tag)
        failures++;
    ENDTRY
    TRY
        u5.sw = APOSTROPHE;
        a_op5(h, &u5);
    CATCH(rpc_x_invalid_tag)
        failures++;
    ENDTRY
    TRY
        u5.sw = XON;
        a_op5(h, &u5);
    CATCH(rpc_x_invalid_tag)
        failures++;
    ENDTRY
    TRY
        u5.sw = XOFF;
        a_op5(h, &u5);
    CATCH(rpc_x_invalid_tag)
        failures++;
    ENDTRY
    /* Try a truly bogus discriminator */
    TRY
        u5.sw = 'x';
        a_op5(h, &u5);
        failures++;
    CATCH(rpc_x_invalid_tag)
    CATCH_ALL
        failures++;
    ENDTRY
    printf("%d failures after a_op5\n", failures);

    /*
     * Test the code path for strings that have to spell the string size
     * expression like: sizeof(struct tagname)
     */
    for (i=0; i<26; i++)
        s6.s[i].c = is[i];
    s6.s[i].c = '\0';

    a_op6(h, &s6);

    for (i=0, j=25; i<26; i++, j--)
        if (s6.s[i].c != is[j])
        {
            failures++;
            break;
        }
    printf("%d failures after a_op6\n", failures);

    /*
     * Test the code path for spelling varying information
     * in the 'P' routine for an out_of_line type.
     */
    for (i=0; i<FDIM; i++)
    {
        a4[i] = b4[i] = random();
        b4[i] *= 2;
        a7[i] = &a4[i];
    }

    a_op7(h, a7);

    for (i=0; i<FDIM; i++)
        if (*(a7[i]) != b4[i])
        {
            failures++;
            break;
        }
    printf("%d failures after a_op7\n", failures);

    /*
     * Test the code path for allocating storage for an [out]-only conformant
     * array containing [ref] pointers not as the array element type.
     */
    for (i=0; i<FDIM; i++)
        a8[i].lp = (long *)malloc(sizeof(op8_struct));

    a_op8(h, FDIM, a8);

    for (i=0; i<FDIM; i++)
        if (*(a8[i].lp) != i*100)
        {
            failures++;
            break;
        }
    printf("%d failures after a_op8\n", failures);

    return failures;
}
