/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: srvrtab.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:19  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:13  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:10:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:14  root]
 * 
 * Revision 1.1.2.2  1993/01/11  23:40:49  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:14:19  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:50  devrcs
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
**      srvrtab.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Tests for the Server Object services of the RPC Communications 
**  Component
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**  23-jul-91 bmartin   change all comtest_server_xxx functions to 
**                      comtst_svr_xxx first for portability, 5 were over
**                      30 characters, and then for consistency.
**  15-oct-90 burton    original
**
**  %a%private_end  
**
*/

#include <tstdriver.h>

 int comtst_svr_inq_bindings ();
 int comtst_svr_register_if ();
 int comtst_svr_use_all_protseqs ();
 int comtst_svr_use_protseq ();
 int comtst_svr_use_protseq_if ();
 int comtst_svr_use_all_protseqs_if ();
 int comtst_svr_use_protseq_ep ();



struct  cmd  cmdtab[] = {
{ "comtest_server_inq_bindings",     comtst_svr_inq_bindings             },
{ "comtest_server_register_if",      comtst_svr_register_if              },
{ "comtest_server_use_all_protseqs", comtst_svr_use_all_protseqs         },
{ "comtest_server_use_protseq",      comtst_svr_use_protseq              },
{ "comtest_server_use_protseq_if",      comtst_svr_use_protseq_if        },
{ "comtest_server_use_all_protseqs_if",   comtst_svr_use_all_protseqs_if },
{ "comtest_server_use_protseq_ep",   comtst_svr_use_protseq_ep           },
{ 0 },
};

char  *tstscript  =  "./testscript";   /*  Default input testscript file */
