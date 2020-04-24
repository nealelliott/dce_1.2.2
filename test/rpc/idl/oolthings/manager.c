/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.6.2  1996/02/17  23:15:28  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:17  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:26:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:09  root]
 * 
 * Revision 1.1.3.3  1993/01/07  20:33:07  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:36:22  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:21:52  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:47:02  weisman]
 * 
 * Revision 1.1  1992/01/19  03:25:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#if defined(VMS) || defined(SNI_SVR4_POSIX)
#include <string.h>
#else
#include <strings.h>
#endif
#include <oolthings.h>

void his_op0(p_c_s,f_a)
    conf_struct *p_c_s;
    fixed_array f_a;
{
    int i;

    for(i=0; i<FIXED_SIZE; i++)
    {
        f_a[i] = 16 * p_c_s->c_a[i % CONF_SIZE];
    }

}

void his_op1(s,c_a,p_f,p_l,f_a)
    long s;
    conf_array c_a;
    long *p_f;
    long *p_l;
    fixed_array f_a;
{
    int i,j;

    j = 5;
    *p_f = j;
    for(i=0; i<CONF_SIZE; i++,j++)
    {
        f_a[j] = c_a[i];
    }
    *p_l = j - 1;
}

void his_op2(p_o_s,s,p_f,p_l,c_a)
    open_struct *p_o_s;
    long s;
    long *p_f;
    long *p_l;
    conf_array c_a;
{
    int i;

    *p_f = p_o_s->f;
    *p_l = p_o_s->l;
    for(i=p_o_s->f; i<=p_o_s->l; i++)
    {
        c_a[i] = p_o_s->c_a[i];
    }
}

globaldef oolthings_v0_0_epv_t oolthings_v0_0_m_epv = 
    {his_op0, his_op1, his_op2};
