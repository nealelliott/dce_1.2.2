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
 * Revision 1.1.4.2  1996/02/17  23:18:04  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:06  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:26  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:29:12  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:16:40  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <ue1.h>
#include <test_common.h>

void mgr_op1(h)
handle_t h;
{
    EXCEPTION_INIT(my_exc1);
    RAISE(my_exc1);
}

void mgr_op2(h)
handle_t h;
{
    RAISE(rpc_x_invalid_tag);
}

void mgr_op3(h)
handle_t h;
{
    RAISE(my_exc3);
}

/*globaldef*/ ue1_v0_0_epv_t ue1_v0_0_m_epv = {
    mgr_op1,
    mgr_op2,
    mgr_op3};
