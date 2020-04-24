/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: commontest.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:20  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:03:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:42:57  root]
 * 
 * Revision 1.1.2.2  1993/01/11  22:12:24  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:04:29  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:34:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
**      commontest.h
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
**  10-feb-91     burton   original
**
**  %a%private_end  
**
*/

typedef unsigned char boolean;
extern boolean rpc_g_initialized;

#define CODING_ERROR(status)  *(status)=rpc_s_coding_error
#define RPC_VERIFY_INIT()  if (rpc_g_initialized == false) rpc__init()
