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
 * Revision 1.1.4.2  1996/02/17  23:16:28  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:52  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:27:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:59  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:42:31  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:40:21  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <pointarray.h>
#include <test_common.h>

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    us33 *p_array;
    int i,j;

    p_array = (us33 *)malloc(sizeof(us33));
    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            (*p_array)[i][j] = 100*i + j;
            printf(" %d", (*p_array)[i][j]);
        }
        printf("\n");
    }
    my_oper(h, &p_array);
    for (i=0; i<3; i++)
    {
        for (j=0; j<3; j++)
        {
            printf(" %d", (*p_array)[i][j]);
        }
        printf("\n");
    }

   return 0;
}
