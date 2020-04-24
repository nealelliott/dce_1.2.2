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
 * Revision 1.1.4.2  1996/02/17  23:12:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:43  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:56:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:23  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:05:06  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:26:29  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <arraystr.h>
#include <test_common.h>

#define CONFSIZE 10
#define OPENSIZE 20

int client_aux(h,passes)
      handle_t h;
      int passes;
{
   int failures = 0;
   error_status_t st;
    ndr_char c_a[26];
    long l_a[20];
    long f_l;
    long l_l;
    short s_a[CONFSIZE];
    long f_u;
    long l_u;
    unsigned long u_a[OPENSIZE];
    int i;
    JIS_KANJI kanji[50];
    str_str_t str_str;

    for (i=0; i<26; i++) c_a[i] = 'A' + i;
    for (i=0; i<20; i++) l_a[i] = 0;
    f_l = 5;
    l_l = 15;
    for (i=f_l; i<=l_l; i++) l_a[i] = i * 2;
    for (i=0; i<CONFSIZE; i++) s_a[i] = i * 4;
    for (i=0; i<OPENSIZE; i++) u_a[i] = 0;
    f_u = 4;
    l_u = 12;
    for (i=f_u; i<=l_u; i++) u_a[i] = i * 8;
    op0( h, c_a, &f_l,&l_l, l_a, CONFSIZE, s_a, OPENSIZE, &f_u, &l_u, u_a );
    printf( "Client received\n");
    for (i=0; i<26; i++) printf( "%c ",c_a[i] );
    printf("\n");
    for (i=f_l; i<=l_l; i++) printf( "%d ", l_a[i] );
    printf("\n");
    for (i=0; i<CONFSIZE; i++) printf( "%d ", s_a[i] );
    printf("\n");
    for (i=f_u; i<=l_u; i++) printf("%u ", u_a[i] );
    printf("\n");

    for (i=0; i<26; i++)
    {
        kanji[i].row = 'A' + i;
        kanji[i].column = 'a' + i;
    }
    kanji[26].row = 0;
    kanji[26].column = 0;
    op1( h, kanji, &str_str );
    for( i=0; str_str.str[i].row != 0; i++)
    {
        if (kanji[i].row != str_str.str[i].row) printf("String row error\n");
        if (kanji[i].column != str_str.str[i].column) 
                                                printf("String column error\n");
    }
    if (i != 26) printf("String length error\n");
    else printf("String op1 successful\n");

    op2( h, kanji, &str_str );
    for( i=0; str_str.str[i].row != 0; i++)
    {
        if (kanji[i].row != str_str.str[i].row) printf("String row error\n");
        if (kanji[i].column != str_str.str[i].column) 
                                                printf("String column error\n");
    }
    if (i != 26) 
      { 
      printf("String length error\n");
      failures++;
      }
    else printf("String op2 successful\n");

    return failures;
}
