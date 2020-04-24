/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY 
 * $Log: dtscp.h,v $
 * Revision 1.1.4.2  1996/02/18  23:12:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:02  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:09:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:19  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:51:59  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:48  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/21  22:05:31  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:52:20  cbrooks]
 * 
 * $EndLog$
 */

#ifndef _DTSCP_H 
#define _DTSCP_H 1

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcedtsmsg.h>

#include <check_status.h>

/*
 * argument parser 
 */

int dts_dtscp_parseargs( int * _1, char *** _2, char ** _3);

/*
 * function to check a given binding handle; return TRUE if 
 * acceptable
 */

boolean dts_dtscp_choose_dts_binding(rpc_binding_handle_t);

boolean dts_dtscp_choose_first_binding( rpc_binding_handle_t) ;



/*
 * takes a DCE hostname, interface spec, and validation function as 
 * input, return a binding handle and status code
 */

void dts_dtscp_select_nsi_binding( 
				  char *,			/* in */
				  rpc_if_handle_t,		/* in */
				  boolean (*)(rpc_binding_handle_t), /* in */
				  rpc_binding_handle_t *,	/* out */
				  unsigned32 *);		/* out */


void dts_dtscp_do_import_binding( 
				 char [],
				 rpc_if_handle_t ,	
				 boolean (*)(rpc_binding_handle_t _1),
				 rpc_binding_handle_t  *,
				 unsigned32 * 
				 );

#endif /* _DTSCP_H */
