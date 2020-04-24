/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: comtest_interface_x.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:23  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:04  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:03:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:01  root]
 * 
 * Revision 1.1.2.2  1993/01/11  22:12:44  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:04:47  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:34  devrcs
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
**      comtest_interface_X.c
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
**  23-jul-91 bmartin   change comtest_server_xfr to comtst_svr_xfr 
**                      to be consistent with VMS port changes.
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
#include    <comtest_interface_x.h>

extern sstring_t  *comtest_cat_id ();




/***************************************************************************/

void interfaceX_imp1_op1 (h, imsg, sp, omsg, status)

handle_t                h;
unsigned long           imsg;
sstring_t               *sp;
unsigned long           *omsg;
unsigned32              *status;

{

    comtst_svr_xfr (h, imsg,
			(comtest_cat_id ("interfaceX_imp1_op1 - ", sp)),
			omsg, status);
}


/***************************************************************************/

void interfaceX_imp1_op2 (h, imsg, sp, omsg, status)

handle_t                h;
unsigned long           imsg;
sstring_t               *sp;
unsigned long           *omsg;
unsigned32              *status;

{

    comtst_svr_xfr (h, imsg,
			(comtest_cat_id ("interfaceX_imp1_op2 - ", sp)),
			omsg, status);
}

/***************************************************************************/

void interfaceX_imp2_op1 (h, imsg, sp, omsg, status)

handle_t                h;
unsigned long           imsg;
sstring_t               *sp;
unsigned long           *omsg;
unsigned32              *status;

{

    comtst_svr_xfr (h, imsg,
			(comtest_cat_id ("interfaceX_imp2_op1 - ", sp)),
			omsg, status);
}

/***************************************************************************/

void interfaceX_imp2_op2 (h, imsg, sp, omsg, status)

handle_t                h;
unsigned long           imsg;
sstring_t               *sp;
unsigned long           *omsg;
unsigned32              *status;

{

    comtst_svr_xfr (h, imsg,
			(comtest_cat_id ("interfaceX_imp2_op2 - ", sp)),
			omsg, status);
}


/***************************************************************************/

globaldef interface_X_v2_0_epv_t interfaceX_imp1_epv =
{
    interfaceX_imp1_op1,
    interfaceX_imp1_op2,
};


/***************************************************************************/

globaldef interface_X_v2_0_epv_t interfaceX_imp2_epv =
{
    interfaceX_imp2_op1,
    interfaceX_imp2_op2,
};

/***************************************************************************/

globaldef  rpc_mgr_epv_t * interfaceX_epv_list[] =
{
    (rpc_mgr_epv_t  *)&interfaceX_imp1_epv,
    (rpc_mgr_epv_t  *)&interfaceX_imp2_epv
};
