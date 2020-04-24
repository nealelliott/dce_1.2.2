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
 * Revision 1.1.4.2  1996/02/17  23:18:10  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:10  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:31  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:29:38  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:17:00  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <ue2.h>
#include <test_common.h>

void mgr_op1(h)
handle_t h;
{
    EXCEPTION_INIT(my_exc1);
    EXCEPTION_INIT(my_exc2);
    EXCEPTION_INIT(my_exc3);
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

/*globaldef*/ ue2_v0_0_epv_t ue2_v0_0_m_epv = {
    mgr_op1,
    mgr_op2,
    mgr_op3};
