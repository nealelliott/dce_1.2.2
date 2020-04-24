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
 * Revision 1.1.6.2  1996/02/17  23:15:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:16  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:25:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:05  root]
 * 
 * Revision 1.1.3.3  1993/01/07  20:32:28  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:36:05  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:21:47  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:46:57  weisman]
 * 
 * Revision 1.1  1992/01/19  03:25:25  devrcs
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
#include <oolsmall.h>

void his_v1_op0(p_v_s,p_l,f_a)
    v1_vary_struct *p_v_s;
    long *p_l;
    v1_fixed_array f_a;
{
    int i;

    *p_l = p_v_s->l;
    for(i=0; i<=p_v_s->l; i++)
    {
        f_a[i] = p_v_s->f_a[i];
    }

}

void his_v1_op2(p_o_s,s,p_l,c_a)
    v1_open_struct *p_o_s;
    long s;
    long *p_l;
    v1_conf_array c_a;
{
    int i;

    *p_l = p_o_s->l;
    for(i=0; i<=p_o_s->l; i++)
    {
        c_a[i] = p_o_s->c_a[i];
    }
}

globaldef oolsmall_v0_0_epv_t oolsmall_v0_0_m_epv =  {
his_v1_op0, his_v1_op2
};
