/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_client.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:55:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:29  root]
 * 
 * Revision 1.1.2.1  1994/06/14  20:00:04  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:16:50  weir]
 * 
 * $EndLog$
 */
/*
 * OLD HISTORY
 * 
 * Revision 1.2.3.3  1993/10/21  19:44:47  weir
 * 	timop for DCE 1.0.3, with following fixes:
 * 	CR 4804: use DCE status codes
 * 	CR 4805: use relative principal names
 * 	CR 7460: initialize remote status parameter in server manager
 * 	CR 9215: port to HP platform.
 * 	[1993/10/21  19:38:26  weir]
 *
 * Revision 1.2.2.3  1993/02/04  20:32:29  cjd
 * 	Embedded copyright notice
 * 	[1993/02/04  20:29:49  cjd]
 * 
 * Revision 1.2.2.2  1992/12/18  18:18:30  marcyw
 * 	Defect 4998: adding disclaimer comment
 * 	[1992/12/18  18:12:17  marcyw]
 * 
 * Revision 1.2  1992/01/19  22:14:29  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * 
 */
/*
 * The Example code is provided to you as "Pass-through Code".
 * Pass-through Code and its related Documentation are not warrented,
 * supported, or indemnified by OSF, and are provided "AS IS".
 * You can use, sublicense, or distribute the same at your own risk.
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
**	timop_client.h
**
**	Client header file for timop interface.
*/

#define MAX_SERVERS	10		/* single-digit server_num's, 0...9 */
#define CLIENT_NUM	-1		/* not equal to any server_num */
#define MAX_RANDOM	(10*1000*1000)	/* big, to observe threads in action */
#define DO_WORK_OK	0		/* pass */
#define DO_WORK_ERR	1		/* fail */

/* Package up do_work() args in a struct, because
   pthreads start routines take only one argument. */
typedef struct work_arg {
	int			server_num;	/* as ordered in arg list */
	unsigned_char_t		*server_name;	/* as named in arg list */
	rpc_binding_handle_t	bind_handle;	/* binding handle to server */
	idl_long_int		rand;		/* input to the rpc call */
	int			status;		/* returned from do_work() */
} work_arg_t;

/* Prototypes for client. */
int main(int _1, char *_2[]);
void do_work(work_arg_t *_1);
void print_report(unsigned_char_t *_1, int _2, utc_t *_3, long _4);

/* NEW CODE */
void print_error(char *_1, error_status_t _2);
/* NEW CODE */

