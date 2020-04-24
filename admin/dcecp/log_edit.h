/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: log_edit.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:47  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:21:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:56:10  root]
 * 
 * Revision 1.1.2.4  1994/09/08  14:00:42  salamone
 * 	CR11968-Setting authentication on binding handles.
 * 	[1994/09/08  13:59:15  salamone]
 * 
 * Revision 1.1.2.3  1994/08/18  21:39:18  salamone
 * 	CR10865 - Handle modifying and displaying routing specifications
 * 		  with multiple routes in TCL and string syntax.
 * 	[1994/08/18  21:38:25  salamone]
 * 
 * Revision 1.1.2.2  1994/05/18  20:41:01  salamone
 * 	CR10636 - "log show" and "log list" should display the appropriate
 * 		   info if unauthenticated.
 * 	[1994/05/13  21:32:53  salamone]
 * 
 * Revision 1.1.2.1  1994/04/26  15:32:32  salamone
 * 	Initial version of log_edit.h
 * 	[1994/04/25  13:36:46  salamone]
 * 
 * $EndLog$
 */


/*
 * MODULE:
 * log_edit.h
 *
 * DESCRIPTION:
 * External declarations for editing routing specifications for 
 * serviceability and debug.
 *
 */

#ifndef _LOG_EDIT_H
#define _LOG_EDIT_H

/*
 * DECLARATIONS 
 */
#define DCP_C_LOG_SEP_CHAR                 ':'
#define DCP_C_LOG_ROUTESEP_CHAR            ';'
#define DCP_C_LOG_FLAGSEP_CHAR             ','
#define DCP_C_LOG_LEVELSEP_CHAR            '.'

/* 
 * This structure contains all the necessary information to
 * perform a log operation. Each log operation will refresh this
 * cache. 
 * If a list of servers are specified, the acl operation 
 * will be performed on each server in the order specified. Note
 * that the log operation on each server will refresh this cache.
 */
typedef struct dcp_log_info_s_t {
    char 		    *named_object; 
    handle_t                server_handle;
} dcp_log_info_t, *dcp_log_info_p_t;


/*
 * PROTOTYPES 
 */
int  dcp_log_bind_named_object(Tcl_Interp *, dcp_log_info_p_t);
int  dcp_log_do_list_comp(dcp_log_info_p_t, Tcl_Interp *);
int  dcp_log_do_list_subcomp(char *, dcp_log_info_p_t, Tcl_Interp *);
int  dcp_log_do_show_svc_routings(boolean32, dcp_log_info_p_t, Tcl_Interp *);
int  dcp_log_do_modify_routings(char *, dcp_log_info_p_t, Tcl_Interp *);

#endif   /* _LOG_EDIT_H */
