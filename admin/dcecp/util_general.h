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
 * $Log: util_general.h,v $
 * Revision 1.1.6.1  1996/08/09  11:53:16  arvind
 * 	Merge changes.
 * 	[1996/06/12  15:20 UTC  truitt  /main/DCE_1.2.2/truitt_dcecp_pk/1]
 *
 * 	Merge with latest HP fixes and enhancements.
 * 	[1996/02/06  17:48 UTC  truitt  /main/HPDCE02/2]
 *
 * 	Merge changes.
 * 	[1996/02/06  17:46 UTC  truitt  /main/HPDCE02/truitt_dcecp_chf60/1]
 *
 * 	ot13289: Ensure bindings are processed correctly.
 *
 * 	Initial DCE_1.2.2 branch.
 * 	[1996/06/12  14:42 UTC  truitt  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.4.2  1996/02/18  23:31:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:00  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  21:25:08  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  20:47 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/23  00:50 UTC  tatsu_s
 * 	Added dcp_general_is_bound_locally() and dcp_general_un/bind_local().
 * 	[1995/12/08  17:58:17  root]
 * 
 * Revision 1.1.2.5  1994/08/19  22:15:52  rousseau
 * 	Made aud and dts attributes and values abbreviateable (CR 11025).
 * 	[1994/08/19  22:15:04  rousseau]
 * 
 * Revision 1.1.2.4  1994/08/19  18:04:11  salamone
 * 	CR10683 - Provide generic routines to handle 1 sbin in TCL syntax.
 * 	[1994/08/19  17:50:17  salamone]
 * 
 * Revision 1.1.2.3  1994/06/09  16:02:36  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:12  devsrc]
 * 
 * Revision 1.1.2.2  1994/02/03  17:24:25  kevins
 * 	added a couple of interesting routines
 * 	[1994/02/03  17:18:49  kevins]
 * 
 * Revision 1.1.2.1  1993/12/20  20:08:11  kevins
 * 	New file for general utilities
 * 	[1993/12/20  18:49:19  kevins]
 * 
 * $EndLog$
 */

#ifndef _UTIL_GENERAL_H
#define _UTIL_GENERAL_H

char * expand_cell_name(char *, error_status_t *);
int isllist(char *);
int decode_uuid(uuid_t *, char *);
int input_parameter_check(Tcl_Interp *, unsigned32, int, char *);
int dcp_general_split_list(Tcl_Interp *, char *, int *, char ***);
int dcp_general_allocate_argv_for_one(Tcl_Interp *, char *, int *, char ***);
int dcp_general_find_attr_index(Tcl_Interp *, char *, int, char *[], int *);
int dcp_general_is_bound_locally(Tcl_Interp *);
int dcp_general_bind_local(Tcl_Interp *);
int dcp_general_unbind_local(Tcl_Interp *);

#define NUM_PARAM 2    /* average number of parameters expected in commands */

#endif
