/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_svchandle.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:41  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:15  root]
 * 
 * Revision 1.1.2.2  1994/06/17  18:43:06  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:20:22  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/25  18:41:02  sekhar
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 ) . Initial Version
 * 	[1994/03/25  18:39:58  sekhar]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

/*
 *   This file creates serviceability handles for sub-component
 *   tables using the DCE_SVC_DEFINE_HANDLE macro. Handles need to be 
 *   created only for components used by the security code in libdce .
 */

#include <dce/dce.h>
#include <dce/dce_svc.h>
#include <dcesecsvc.h>

DCE_SVC_DEFINE_HANDLE(sec_svc_handle, sec_svc_table, "sec")
