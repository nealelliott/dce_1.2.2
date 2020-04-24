/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_svc_client.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:26  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:55:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:14  root]
 * 
 * Revision 1.1.2.1  1994/06/14  19:57:40  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:15:17  weir]
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
**	timop_svc_client.h
**
**	Client header file for timop_svc interface.
*/

#define MAX_SERVERS	10		/* single-digit server_num's, 0...9 */
#define CLIENT_NUM	-1		/* not equal to any server_num */
#define MAX_RANDOM	(10*1000*1000)	/* big, to observe threads in action */
#define DO_WORK_OK	0		/* pass */
#define DO_WORK_ERR	1		/* fail */


/* Package up do_work() args in a struct, because
   pthreads start routines take only one argument. */
typedef struct work_arg {
	int			server_num;	 /* as ordered in arg list */
	unsigned_char_t		*server_name;	 /* as named in arg list */
	int			object_ix;	 /* object we're binding to */
	rpc_binding_handle_t	bind_handle;	 /* binding handle to server */
	rpc_binding_handle_t	svc_bind_handle; /* svc binding handle */
	idl_long_int		rand;		 /* input to the rpc call */
	int			status;		 /* returned from do_work() */
	int			call_nr;	 /* number (from 0) of thread's call */
} work_arg_t;

/* Prototypes for client. */
int main(int _1, char *_2[]);
void do_work(work_arg_t *_1);
void print_report(unsigned_char_t *_1, int _2, utc_t *_3, long _4);

/* NEW CODE */
void print_error(char *_1, error_status_t _2);
/* NEW CODE */

#define MAX_ROUTES 10
/* ...maximum number of serviceability routings allowed               */

#define MAX_LEVELS 9
/* ...maximum number of separate debug levels allowed                 */


#define DO_EXTRA_SERVERS  66
/* ...check number of servers specified on client's command line          */
#define DO_SERVER_IX      11
/* ...check where on client's command line list of servers starts         */
#define DO_STRING_BINDING 77
/* ...check whether client is using string binding                        */
#define DO_ONE_OBJECT     44
/* ...return specified object number                                      */
#define DO_OBJECT_BINDING 88
/* ...check which object client is associating with bind attempt          */
#define DO_FILTER_TOGGLE  55
/* ...check for serviceability toggle                                     */
#define DO_OBJECT_NR      22
/* ...check for multiple object UUIDs                                     */
#define GET_ROUTE_LIST    20
/* ...check for whether to request svc route list from server             */
#define GET_COMPONENTS    30
/* ...check for whether to request list of registered svc components      */
#define GET_SUBCOMPONENTS 31
/* ...check for whether to request list of subcomponents                  */
#define SET_ROUTES        21
/* ...check for whether to request remote svc route setting(s)            */
#define SET_DEBUG_ROUTES  24
/* ...check for whether to request remote svc debug route setting(s)      */
#define SET_DEBUG_LEVELS  27
/* ...check for whether to request remote svc debug level setting(s)      */
