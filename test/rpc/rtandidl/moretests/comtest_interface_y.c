/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: comtest_interface_y.c,v $
 * Revision 1.1.6.2  1996/02/17  23:20:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:07  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:04:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:03  root]
 * 
 * Revision 1.1.4.1  1993/10/15  17:40:24  tom
 * 	Bug 8356 - Remove declaration of string_buf.
 * 	[1993/10/15  17:39:29  tom]
 * 
 * Revision 1.1.2.2  1993/01/11  22:12:54  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:04:56  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  %a%copyright(,**  )
**
**  NAME
**
**      comtest_interface_Y.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Server manager routines for performance and system execiser
**  auxiliary interface. This interface is generic.
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**  23-jul-91 bmartin   change comtest_server_xfr to comtst_svr_xfr to be
**                      consistent with VMS port changes.
**  07-dec-90 burton    original
**
**  %a%private_end  
**
*/
/*
#include    <commonp.h>
#include    <com.h>
*/
#include    <dce/rpc.h>
#include    <tstdriver.h>
#include    <comtest.h>
#include    <comtest_interface_y.h>

extern kstring_t  *comtest_cat_id ();


/***************************************************************************/

void interfaceY_imp1_op1 (h, imsg, sp, omsg, status)

handle_t                h;
unsigned long           imsg;
kstring_t               *sp;
unsigned long           *omsg;
unsigned32              *status;

{

    comtst_svr_xfr (h, imsg,
			(comtest_cat_id ("interfaceY_imp1_op1 - ", sp)),
			omsg, status);
}


/***************************************************************************/

void interfaceY_imp1_op2 (h, imsg, sp, omsg, status)

handle_t                h;
unsigned long           imsg;
kstring_t               *sp;
unsigned long           *omsg;
unsigned32              *status;

{

    comtst_svr_xfr (h, imsg,
			(comtest_cat_id ("interfaceY_imp1_op2 - ", sp)),
			omsg, status);
}

/***************************************************************************/

void interfaceY_imp2_op1 (h, imsg, sp, omsg, status)

handle_t                h;
unsigned long           imsg;
kstring_t               *sp;
unsigned long           *omsg;
unsigned32              *status;

{

    comtst_svr_xfr (h, imsg,
			(comtest_cat_id ("interfaceY_imp2_op1 - ", sp)),
			omsg, status);
}

/***************************************************************************/

void interfaceY_imp2_op2 (h, imsg, sp, omsg, status)

handle_t                h;
unsigned long           imsg;
kstring_t               *sp;
unsigned long           *omsg;
unsigned32              *status;

{

    comtst_svr_xfr (h, imsg,
			(comtest_cat_id ("interfaceY_imp2_op2 - ", sp)),
			omsg, status);
}


/***************************************************************************/

globaldef interface_Y_v2_0_epv_t interfaceY_imp1_epv =
{
    interfaceY_imp1_op1,
    interfaceY_imp1_op2,
};


/***************************************************************************/

globaldef interface_Y_v2_0_epv_t interfaceY_imp2_epv =
{
    interfaceY_imp2_op1,
    interfaceY_imp2_op2,
};

/***************************************************************************/

globaldef  rpc_mgr_epv_t * interfaceY_epv_list[] =
{
    (rpc_mgr_epv_t  *)&interfaceY_imp1_epv,
    (rpc_mgr_epv_t  *)&interfaceY_imp2_epv
};
