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
 * Revision 1.1.4.2  1996/02/17  23:16:30  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:53  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:28:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:00  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:42:37  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:40:24  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <pointarray.h>
#include <test_common.h>

void his_oper (h, p_p_array)
      /*[in]*/ handle_t h;
      /*[in,out]*/ us33 **p_p_array;

{ 
    int i,j;
    unsigned short work;

    for (i=0; i<3; i++)
    {
        for (j=0; j<i; j++)
        {
            work = (**p_p_array)[i][j];
            (**p_p_array)[i][j] = (**p_p_array)[j][i];
            (**p_p_array)[j][i] = work;
        }
    }
}

globaldef pointarray_v0_0_epv_t pointarray_v0_0_m_epv = {his_oper};
