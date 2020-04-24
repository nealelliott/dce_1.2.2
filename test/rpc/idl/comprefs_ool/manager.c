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
 * Revision 1.1.4.2  1996/02/17  23:13:10  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:08  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:59:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:21  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:11:32  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:29:41  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:31:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <comprefs_ool.h>
#include <test_common.h>

void his_op_fixed( h, f_r_l )
   handle_t h;
    r_l_t f_r_l[3][3];
{
    int i,j;
    long work;

#ifdef DEBUG
    printf("Server received\n");
#endif
    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            work = *(f_r_l[i][j]);
#ifdef DEBUG
            printf( " %3d", work );
#endif
            *(f_r_l[i][j]) = (work / 100) + 100 * (work % 100);
        }
#ifdef DEBUG
        printf("\n");
#endif
    }
}

void his_op_conf ( h, s, o_r_l)
   handle_t h;
    long s;
    r_l_t o_r_l[];
{
    int i;

#ifdef DEBUG
    printf("Server received\n");
#endif
    for (i=0; i<s; i++)
    {
#ifdef DEBUG
        printf( " %08x", *(o_r_l[i]) );
#endif
        *(o_r_l[i]) ^= 0xFFFFFFFF;
#ifdef DEBUG
        if ( (i % 8) == 7) printf("\n");
#endif
    }
}

globaldef comprefs_ool_v0_0_epv_t comprefs_ool_v0_0_m_epv = {
    his_op_fixed,
    his_op_conf  
};
