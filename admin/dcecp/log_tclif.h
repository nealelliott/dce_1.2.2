/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: log_tclif.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:48  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:21:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:56:14  root]
 * 
 * Revision 1.1.2.1  1994/04/26  15:32:34  salamone
 * 	Initial version of log_tclif.h
 * 	[1994/04/25  13:37:28  salamone]
 * 
 * $EndLog$
 */


/*
 * MODULE:
 * log_tclif.h
 *
 * DESCRIPTION:
 * External declarations to support TCL interface for servicability.
 *
 */

#ifndef _LOG_TCLIF_H
#define _LOG_TCLIF_H

/*
 * PROTOTYPES 
 */
int dcp_log_list(ClientData, Tcl_Interp *, int, char **);
int dcp_log_modify(ClientData, Tcl_Interp *, int, char **);
int dcp_log_show(ClientData, Tcl_Interp *, int, char **);

#endif   /* _LOG_TCLIF_H */
