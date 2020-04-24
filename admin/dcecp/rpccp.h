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
 * $Log: rpccp.h,v $
 * Revision 1.1.6.2  1996/02/18  23:31:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:56  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:23:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/26  18:07 UTC  jrr
 * 	Use better function names in rpccp.c
 * 	[1995/05/24  18:31 UTC  jrr  /main/jrr_dcecp_chfts15321/1]
 * 
 * Revision 1.1.4.4  1994/09/29  22:25:51  melman
 * 	removed resolve_binding()
 * 	[1994/09/29  22:20:04  melman]
 * 
 * Revision 1.1.4.3  1994/06/09  16:02:30  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:05  devsrc]
 * 
 * Revision 1.1.4.2  1994/05/27  15:36:10  kevins
 * 	added uuid compare and name compare commands.
 * 	[1994/05/27  15:35:13  kevins]
 * 
 * Revision 1.1.4.1  1994/03/17  21:52:42  melman
 * 	rpc command cleanup
 * 	[1994/03/17  20:46:31  melman]
 * 
 * Revision 1.1.2.1  1993/10/20  15:00:59  rousseau
 * 	Conversion from dsh to dcecp. Move strings into message catalog.
 * 	[1993/10/20  15:00:53  rousseau]
 * 
 * $EndLog$
 */

/*
 * PRE-HISTORY
 * Revision 1.1.2.9  1993/09/14  23:57:12  melman
 * 	Added rpc-profile remove
 * 	[1993/09/14  23:56:56  melman]
 *
 * Revision 1.1.2.8  1993/09/03  21:02:52  melman
 * 	Added rpc-profile list
 * 	[1993/09/03  21:02:41  melman]
 * 
 * Revision 1.1.2.7  1993/09/03  17:22:06  melman
 * 	Removed show_server and some cleanup
 * 	[1993/09/03  17:15:53  melman]
 * 
 * 	profile add and made expand_name_internal public
 * 	[1993/08/11  18:52:53  melman]
 * 
 * Revision 1.1.2.6  1993/08/06  21:51:55  melman
 * 	Added rpc-group list
 * 	[1993/08/06  21:51:30  melman]
 * 
 * Revision 1.1.2.5  1993/08/05  20:58:54  melman
 * 	addto and removefrom to add and remove
 * 	[1993/08/05  20:46:12  melman]
 * 
 * Revision 1.1.2.4  1993/08/04  20:49:15  melman
 * 	*** empty log message ***
 * 	[1993/08/04  20:47:40  melman]
 * 
 * Revision 1.1.2.3  1993/08/02  19:14:39  melman
 * 	Initial version of updated rpccp functionality
 * 	[1993/08/02  19:12:42  melman]
 * 
 * Revision 1.1.2.2  1993/07/23  20:03:35  kevins
 * 	Initial submission of dsh
 * 	[1993/07/23  19:36:27  kevins]
 * 
 */

#include <dcecp.h>

/****************************************************************
 * prototypes
 ****************************************************************/
int dcp_rpc_uuid_create     (ClientData, Tcl_Interp *, int, char **); 
int dcp_rpc_uuid_compare    (ClientData, Tcl_Interp *, int, char **); 
int dcp_name_expand         (ClientData, Tcl_Interp *, int, char **);
int dcp_name_compare        (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcentry_create     (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcentry_delete     (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcentry_show       (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcgroup_add        (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcgroup_delete     (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcgroup_list       (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcgroup_remove     (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcprofile_add      (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcprofile_delete   (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcprofile_list     (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcprofile_show     (ClientData, Tcl_Interp *, int, char **);
int dcp_rpcprofile_remove   (ClientData, Tcl_Interp *, int, char **);

int dcp_rpc_export          (ClientData, Tcl_Interp *, int, char **);
int dcp_rpc_import          (ClientData, Tcl_Interp *, int, char **);
int dcp_rpc_unexport        (ClientData, Tcl_Interp *, int, char **);

int dcp_endpoint_create     (ClientData, Tcl_Interp *, int, char **);
int dcp_endpoint_show       (ClientData, Tcl_Interp *, int, char **);
int dcp_endpoint_delete     (ClientData, Tcl_Interp *, int, char **);

