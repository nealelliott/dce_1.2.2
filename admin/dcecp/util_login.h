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
 * $Log: util_login.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:03  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:25:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:58:25  root]
 * 
 * Revision 1.1.2.8  1994/09/01  14:16:57  rousseau
 * 	Binding authentication changes
 * 	[1994/09/01  14:16:19  rousseau]
 * 
 * Revision 1.1.2.7  1994/06/09  16:02:39  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:16  devsrc]
 * 
 * Revision 1.1.2.6  1994/06/06  18:39:29  rousseau
 * 	Defuncted old functions. Code review changes.
 * 	[1994/06/06  18:39:12  rousseau]
 * 
 * Revision 1.1.2.5  1994/03/31  16:34:21  rousseau
 * 	Fix handling of _c and _u (CR 10100 10101).
 * 	[1994/03/31  16:31:47  rousseau]
 * 
 * Revision 1.1.2.4  1994/03/17  21:37:16  rousseau
 * 	Included <dce/secidmap.h> (CR 10075).
 * 	[1994/03/17  21:37:04  rousseau]
 * 
 * Revision 1.1.2.3  1993/12/30  19:58:17  rousseau
 * 	Changed include of dcemsgmsg.h to dcesvcmsg.h
 * 	[1993/12/30  19:58:09  rousseau]
 * 
 * Revision 1.1.2.2  1993/12/27  16:29:29  rousseau
 * 	Updated convenience variable support.
 * 	[1993/12/27  16:29:22  rousseau]
 * 
 * Revision 1.1.2.1  1993/12/21  16:17:52  rousseau
 * 	Style guide changes.
 * 	[1993/12/21  16:17:27  rousseau]
 * 
 * $EndLog$
 */

/*
 * PRE-HISTORY
 * Revision 1.1.2.5  1993/11/11  19:52:39  rousseau
 * 	Added prototype for delete_all_login_contexts().
 * 	[1993/11/11  19:52:30  rousseau]
 *
 * Revision 1.1.2.4  1993/11/11  14:28:18  rousseau
 * 	Moved initialization of global_context_list to dcecp_login.c
 * 	[1993/11/11  14:28:11  rousseau]
 * 
 * Revision 1.1.2.3  1993/11/11  13:53:59  rousseau
 * 	Added global_login_context definition.
 * 	[1993/11/11  13:53:53  rousseau]
 * 
 * Revision 1.1.2.2  1993/11/10  18:01:57  rousseau
 * 	Added #include <string.h>
 * 	[1993/11/10  18:01:51  rousseau]
 * 
 * Revision 1.1.2.1  1993/11/05  15:29:08  rousseau
 * 	Initial version.
 * 	[1993/11/05  15:28:39  rousseau]
 */

#ifndef _UTIL_LOGIN_H
#define _UTIL_LOGIN_H

/*
 * MODULE: util_login.h
 *
 * DESCRIPTION:
 *   This module contains prototypes and definitions for maintaining
 * the global stack of login contexts.
 *
 */

/* INCLUDE FILES  */

#include <string.h>
#include <dce/dce.h>
#include <dce/binding.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dce/dcedcpmsg.h>
#include <dce/sec_login.h>
#include <dce/secidmap.h>
#include <dce/pgo.h>
#include <dcecp.h>
#include <util_var.h>

/* DECLARATIONS */

/* INTERNAL VARIABLES */

typedef struct dcp_login_context_s_t {
  struct dcp_login_context_s_t  *next;          /* Pointer to next structure */
  sec_login_handle_t            context;        /* The login context */
  char                          *princ;         /* The principal name */
  char                          *cell;          /* The cell name */
} dcp_login_context_t, *dcp_login_context_p_t;

pthread_mutex_t                 login_list;     /* For global_context_list */
pthread_mutex_t                 initial_login;  /* For initial context     */

/* PROTOTYPES */

int add_login_context(Tcl_Interp *, sec_login_handle_t);
int delete_login_context(Tcl_Interp *);
int delete_all_login_contexts(Tcl_Interp *);
int check_initial_auth(Tcl_Interp *);
int dcp_get_princ_and_cell(Tcl_Interp *, sec_login_handle_t, char **, char **);
int dcp_is_logged_in(void);

#endif /* _UTIL_LOGIN_H */
