/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dcp_utc.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:41  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:20:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:55:34  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:23  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:15:41  annie]
 * 
 * 	expand copyright with OSF copyright text
 * 
 * 	expand copyright with OSF copyright text
 * 
 * Revision 1.1.2.1  1994/07/01  14:25:29  rousseau
 * 	Implemented utc subcomponent.
 * 	[1994/07/01  14:24:35  rousseau]
 * 
 * $EndLog$
 */

/*
 * MODULE: dcp_utc.h
 *
 * DESCRIPTION:
 *   This module contains prototypes and definitions used the the utc
 * component of dcecp.
 *
 */

/* INCLUDE FILES  */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcedcpmsg.h>
#include <dce/dcesvcmsg.h>
#include <dce/utc.h>
#include <util_help.h>
#include <util_login.h>
#include <dcecp.h>

/* DECLARATIONS */

/* INTERNAL VARIABLES */

/* PROTOTYPES */

int utc_add                     (ClientData, Tcl_Interp *, int, char **);
int utc_compare                 (ClientData, Tcl_Interp *, int, char **);
int utc_convert                 (ClientData, Tcl_Interp *, int, char **);
int utc_multiply                (ClientData, Tcl_Interp *, int, char **);
int utc_subtract                (ClientData, Tcl_Interp *, int, char **);

