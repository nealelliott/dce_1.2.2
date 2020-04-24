/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_svc_server.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:55:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:18  root]
 * 
 * Revision 1.1.2.1  1994/06/14  19:57:46  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:15:21  weir]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1990, 1991  
 * Open Software Foundation, Inc. 
 *  
 * Permission is hereby granted to use, copy, modify and freely distribute 
 * the software in this file and its documentation for any purpose without 
 * fee, provided that the above copyright notice appears in all copies and 
 * that both the copyright notice and this permission notice appear in 
 * supporting documentation.  Further, provided that the name of Open 
 * Software Foundation, Inc. ("OSF") not be used in advertising or 
 * publicity pertaining to distribution of the software without prior 
 * written permission from OSF.  OSF makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty. 
 */
/*
 *  OSF DCE Version 1.0 
 */

/*
**	timop_svc_server.h
**
**	Server header file for timop_svc interface.
*/

#define NUM_OBJS		1	/* num of objs supported */
#define MAX_CONC_CALLS_PROTSEQ	5	/* max conc calls per protseq */
#define MAX_CONC_CALLS_TOTAL	10	/* max conc calls total */
#define MAX_NAME		1024	/* max size of name */

/* Success/failure for remote procedures. */
#define GETSPAN_OK		0	/* pass */
#define GETSPAN_ERR		1	/* fail */

/* Defines for access control. */
#define GETSPAN_OP		1	/* requested operation */
#define GRANT_ACCESS		0	/* reference monitor success */
#define DENY_ACCESS		1	/* reference monitor failure */
#define IS_AUTHORIZED		0	/* authorization success */
#define	NOT_AUTHORIZED		1	/* authorization failure */
#define SVC_REMOTE		2	/* remote serviceability operation */

/* Server key table for this example.  Change name of keyfile to suit your
   environment, and populate it with "rgy_edit ktadd tserver /tmp/tskeyfile". */
#define KEYFILE			"/tmp/tskeyfile"
#define KEYTAB			"FILE:" ## KEYFILE

/* Prototypes for server. */
int main(int _1, char *_2[]);
void getspan_ep(rpc_binding_handle_t _1, idl_long_int _2, idl_char *_3,
	timestamp_t _4, idl_long_int *_5, error_status_t *_6);
int do_getspan(idl_long_int _1, timestamp_t _2);
int ref_mon(rpc_binding_handle_t _1, int _2);
int is_authorized(unsigned_char_t *_1, int _2);
void install_filters();
void initialize_remote_svc();
void print_server_error(char *_1, error_status_t _2);
void print_refmon_error(char *_1, error_status_t _2);
void print_manager_error(char *_1, error_status_t _2);

/* NEW CODE */
void print_error(char *_1, error_status_t _2);
/* NEW CODE */


#define DO_ENTRY_NAME     55
/* ...flag to check location of entry name on server command line     */

#define DO_ENTRY_FLAG     44
/* ...flag to check which object type is specified for server         */

#define DO_SETUP          66
/* ...flag to check whether server is being started for first time    */

#define DO_FILTER_FLAG    77
/* ...flag to check whether serviceability filter should be activated */

#define DO_ROUTES         88
/* ...flag to check for svc routings                                  */

#define DO_DEBUG_ROUTES   99
/* ...flag to check for svc debug routings                            */

#define DO_DEBUG_LEVELS   95
/* ...flag to check for svc debug level settings                      */

#define MAX_SERVER_ARGS 12
/* ... i.e.: <server_name> (the executable itself)                    */
/*           <entry_name>  (name entry to export to)                  */
/*           <entry_flag>  (object type to associate with export)     */
/*           <setup_flag>  (=> this is setup; generate type UUIDs)    */
/*           <filter_flag> (switch to activate serviceability filter  */

#define MAX_ROUTES 10
/* ...maximum number of serviceability routings allowed               */

#define MAX_LEVELS 9
/* ...maximum number of separate debug levels allowed                 */

unsigned_char_t *server_name_G;
/* ...this server instance's entry name                               */

int entry_flag_G;
/* ...this server instance's entry object                             */
