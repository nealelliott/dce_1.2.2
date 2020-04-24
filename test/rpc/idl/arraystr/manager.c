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
 * Revision 1.1.7.2  1996/02/17  23:12:33  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:43  marty]
 *
 * Revision 1.1.7.1  1995/12/11  19:56:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:24  root]
 * 
 * Revision 1.1.5.4  1993/02/01  21:33:13  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  21:03:29  hinman]
 * 
 * Revision 1.1.5.3  1993/01/07  20:35:03  hsiao
 * 	Remove comment and restore his_op0 as the first entry of arraystr_v0_0_m_epv
 * 	[1993/01/07  20:30:53  hsiao]
 * 
 * Revision 1.1.5.2  1993/01/07  19:05:13  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:26:33  bbelch]
 * 
 * Revision 1.1.2.3  1992/05/08  20:58:28  rsalz
 * 	Properly fix calling sequence of op0, idl_long_t --> idl_ulong_t.
 * 	[1992/05/08  18:46:53  rsalz]
 * 
 * Revision 1.1.2.2  1992/05/04  20:52:35  rsalz
 * 	Tweak calling sequence of his_op0 so that it matches the idl spec and
 * 	uses the idl_xxx_t types.
 * 	[1992/05/04  20:52:08  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:23:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <arraystr.h>
#include <test_common.h>

void his_op0
( h, c_a, f_l, l_l, l_a, s_s, s_a, s_u, f_u, l_u, u_a )
    /* [in] */handle_t h;
    /* [in, out] */idl_char c_a[26];
    /* [in, out] */idl_long_int *f_l;
    /* [in, out] */idl_long_int *l_l;
    /* [in, out] */idl_long_int l_a[20];
    /* [in] */idl_long_int s_s;
    /* [in, out] */idl_short_int s_a[];
    /* [in] */idl_long_int s_u;
    /* [in, out] */idl_long_int *f_u;
    /* [in, out] */idl_long_int *l_u;
    /* [in, out] */idl_ulong_int u_a[];
{
    int i;

#ifdef DEBUG
    printf( "Server received\n");
#endif
    for (i=0; i<26; i++) 
    {
#ifdef DEBUG
        printf( "%c ",c_a[i] );
#endif
        c_a[i] += ('a' -'A');
    }
#ifdef DEBUG
    printf("\n");
    for (i=(*f_l); i<=(*l_l); i++) printf( "%d ", l_a[i] );
    printf("\n");
#endif
    (*f_l)--;
    (*l_l)++;
    for (i=(*f_l); i<=(*l_l); i++) l_a[i] = i * 3;
    for (i=0; i<s_s; i++) 
    {
#ifdef DEBUG
        printf( "%d ", s_a[i] );
#endif
        s_a[i] = i * 5;
    }
#ifdef DEBUG
    printf("\n");
    for (i=(*f_u); i<=(*l_u); i++) printf("%u ", u_a[i] );
    printf("\n");
#endif
    (*f_u)--;
    (*l_u)++;
    for (i=(*f_u); i<=(*l_u); i++) u_a[i] = i * 7;
}

void his_op1
( h, in, p_out )
    /* [in] */handle_t h;
    /* [in] */ JIS_KANJI *in;
    /* [out] */ str_str_t *p_out;
{
    int str_length;

    for (str_length=0; in[str_length].row!=0; str_length++);
    p_out->s = str_length + 1;
    p_out->str = (JIS_KANJI *)rpc_ss_allocate(p_out->s * sizeof(JIS_KANJI));
    str_length = 0;
    while(true)
    {
        (p_out->str)[str_length].row = in[str_length].row;
        (p_out->str)[str_length].column = in[str_length].column;
        if ( (p_out->str)[str_length].row == 0 ) break;
        str_length++;
    } 
}

void his_op2
( h, in, p_out )
    /* [in] */handle_t h;
    /* [in] */ JIS_KANJI *in;
    /* [out] */ str_str_t *p_out;
{
    int str_length;

    for (str_length=0; in[str_length].row!=0; str_length++);
    p_out->s = str_length + 1;
    p_out->str = (JIS_KANJI *)rpc_ss_allocate(p_out->s * sizeof(JIS_KANJI));
    str_length = 0;
    while(true)
    {
        (p_out->str)[str_length].row = in[str_length].row;
        (p_out->str)[str_length].column = in[str_length].column;
        if ( (p_out->str)[str_length].row == 0 ) break;
        str_length++;
    } 
}

globaldef arraystr_v0_0_epv_t arraystr_v0_0_m_epv = {
    his_op0,
    his_op1,
    his_op2
};
