/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY 
 * $Log: selectnsibinding.c,v $
 * Revision 1.1.4.2  1996/02/17  23:34:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:03  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:09:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:27  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:52:01  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:50  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/21  22:05:49  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:49:35  cbrooks]
 * 
 * $EndLog$
 */

#include <dtscp.h>
#include <stdio.h>

static const char * const HostBindingFmt = "/.:/hosts/%s/dts-entity" ;

/*
 * dts_dtscp_select_nsi_binding (hostname, interface, validation, 
 			 binding_handle, status)
 *
 * given a hostname and an interface, search the name space for that 
 * interface that is exported by the dts-entity on that host.
 */

#define HOSTBINDINGSIZE 256 

void dts_dtscp_select_nsi_binding(
  char * hostname, 
  rpc_if_handle_t interface, 
  boolean (*Vfunc)(rpc_binding_handle_t),
  rpc_binding_handle_t * binding_h,
  unsigned32 * statusp
  )
{
    char HostBinding[ HOSTBINDINGSIZE  ] ;
    
    sprintf( HostBinding, HostBindingFmt, hostname) ;
    
    dts_dtscp_do_import_binding( HostBinding, interface, Vfunc, 
				binding_h, statusp);
    return ;
}


