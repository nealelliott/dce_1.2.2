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
 * Revision 1.1.4.2  1996/02/17  23:13:06  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:05  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:59:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:12  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:10:51  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:29:15  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:24:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <comprefs.h>
#include <test_common.h>

#define CONFSIZE 32

int client_aux(h,passes)
      handle_t h;
      int passes;
{
	error_status_t st;
    r_l_t f_r_l[3][3];
    int i,j;
    r_l_t o_r_l[CONFSIZE];


    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            f_r_l[i][j] = (long *)malloc(sizeof(long));
            *(f_r_l[i][j]) = i*100 + j;
        }
    }
    op_fixed( h, f_r_l );
    printf("Client received\n");
    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            printf( " %3d", *(f_r_l[i][j]) );
        }
        printf("\n");
    }

    for (i=0; i<CONFSIZE; i++)
    {
            o_r_l[i] = (long *)malloc(sizeof(long));
            *(o_r_l[i]) = 1 << i;
    }
    op_conf( h, CONFSIZE, o_r_l );
    printf("Client received\n");
    for (i=0; i<CONFSIZE; i++)
    {
        printf( " %08x", *(o_r_l[i]) );
        if ( (i % 8) == 7) printf("\n");
    }

    return 0;
}
