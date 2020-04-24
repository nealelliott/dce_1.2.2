/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl_tclif.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:32  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:19:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:54:03  root]
 * 
 * Revision 1.1.2.2  1994/02/09  18:57:42  salamone
 * 	CR9883 Most dcecp components don't support help system.
 * 	[1994/02/09  18:33:51  salamone]
 * 
 * Revision 1.1.2.1  1993/12/27  20:46:31  salamone
 * 	Initial version of acl_tclif.h
 * 	[1993/12/27  20:43:49  salamone]
 * 
 * $EndLog$
 */


/*
 * MODULE:
 * acl_tclif.h
 *
 * DESCRIPTION:
 * External declarations to support TCL interface for ACLs.
 *
 */

#ifndef _ACL_TCLIF_H
#define _ACL_TCLIF_H

/*
 * PROTOTYPES 
 */
int dcp_acl_check(ClientData, Tcl_Interp *, int, char **);
int dcp_acl_delete(ClientData, Tcl_Interp *, int, char **);
int dcp_acl_modify(ClientData, Tcl_Interp *, int, char **);
int dcp_acl_permissions(ClientData, Tcl_Interp *, int, char **);
int dcp_acl_replace(ClientData, Tcl_Interp *, int, char **);
int dcp_acl_show(ClientData, Tcl_Interp *, int, char **);

#endif   /* _ACL_TCLIF_H */
