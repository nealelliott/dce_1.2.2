/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: comtest_client_xfr.h,v $
 * Revision 1.1.6.2  1996/02/18  23:06:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:23  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:03:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:00  root]
 * 
 * Revision 1.1.4.2  1993/01/11  22:12:39  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:04:43  bbelch]
 * 
 * Revision 1.1.2.2  1992/07/15  01:27:33  smith
 * 	*** empty log message ***
 * 
 * Revision 1.1.1.2  1992/07/15  01:26:47  smith
 * 	xlc strictness required changes for compile.
 * 
 * Revision 1.1  1992/01/19  03:38:33  devrcs
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
**      comtest_client_xfr.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**  
**  This module contains definitions and preinitialized
**  objects used by test routines comtest_client_xfr and
**  comtest_object_xfr.
**
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**  25-jul-91   bmartin     change type of object vectors to rpc_mgr_proc_t;
**                          interface_[X|Y]_v2_0_epv_t is the wrong type.
**  22-jul-91   bmartin     correct brackets in initialization of
**                          COMTEST_obj_id.
**  10-feb-91   burton      original
**
**  %a%private_end  
**
*/

#include <tstdriver.h>            /* Local definitions and calling protocol 
                                     for test driver */
#include <comtest.h>              /* definitions for communications tests */
#include <comtest_server_if.h>    /* definitions for communications tests */
#include <comtest_interface_x.h>  /* definitions for communications tests */
#include <comtest_interface_y.h>  /* definitions for communications tests */

extern  unsigned32 in_status;
int comtest_client_open_environ ();
int comtest_client_signal_dispatch ();
int comtest_client_create_lock ();


#define COMTEST_SERVER_XFR_ARGS (1)

/*
 * COMTST_obj_id is a preinitialized object id used
 * to initialize the dynamically allocated interface id
 */

static uuid_t COMTST_obj_id =

{
   0x8c863a04, 0x13ec, 0x11c9, 0x9f, 0xe8, {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}
};

/*
 * COMTST_protseq_buf is a BUFSIZ char buffer for
 * reading in protseqs from the temporary file.
 */
char COMTST_protseq_buf[BUFSIZ];

/*
 * COMTST_interface_buf a single character
 * send by comtest_register_if to designate which
 * interface to use - currently either 'X' or 'Y'.
 */
char COMTST_interface_buf;

/*
 * COMTST_obj_buf is a BUFSIZ char buffer for
 * reading in object uuids from the temporary file.
 */
char COMTST_obj_buf[BUFSIZ];

/*
 * COMTST_object_vec_siz, is the number of
 * ops per interface.
 */

#define COMTST_object_vec_siz 2

/*
 * COMTST_object_vec_X, is a vector of entry
 * point addresses for each routine in the
 * test interface X.
 */
rpc_mgr_proc_t COMTST_object_vec_X [COMTST_object_vec_siz] =
{
    (void (*)(void))interfaceX_op1,
    (void (*)(void))interfaceX_op2
};


/*
 * COMTST_object_vec_Y, is a vector of entry
 * point addresses for each routine in the
 * test interface Y.
 */
rpc_mgr_proc_t COMTST_object_vec_Y [COMTST_object_vec_siz] =
{
    (void (*)(void))interfaceY_op1,
    (void (*)(void))interfaceY_op2
};


