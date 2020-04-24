/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_server.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:37  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:55:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:32  root]
 * 
 * Revision 1.1.2.1  1994/06/14  20:00:12  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:16:53  weir]
 * 
 * $EndLog$
 */
/*
 * OLD HISTORY:
 * 
 * Revision 1.2.3.5  1993/10/28  21:07:31  weir
 * 	String handling fixes for OT 9215
 * 	[1993/10/28  21:06:50  weir]
 *
 * Revision 1.2.3.4  1993/10/28  20:37:33  weir
 * 	String handling fixes for OT 9215
 * 	[1993/10/28  20:36:38  weir]
 * 
 * Revision 1.2.3.3  1993/10/21  19:44:50  weir
 * 	timop for DCE 1.0.3, with following fixes:
 * 	CR 4804: use DCE status codes
 * 	CR 4805: use relative principal names
 * 	CR 7460: initialize remote status parameter in server manager
 * 	CR 9215: port to HP platform.
 * 	[1993/10/21  19:38:29  weir]
 * 
 * Revision 1.2.2.3  1993/02/04  20:32:39  cjd
 * 	Embedded copyright notice
 * 	[1993/02/04  20:30:09  cjd]
 * 
 * Revision 1.2.2.2  1992/12/18  18:18:55  marcyw
 * 	Defect 4998: adding disclaimer comment
 * 	[1992/12/18  18:12:38  marcyw]
 * 
 * Revision 1.2  1992/01/19  22:14:32  devrcs
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
**	timop_server.h
**
**	Server header file for timop interface.
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

/* Server key table for this example.  Change name of keyfile to suit your
   environment, and populate it with "rgy_edit ktadd tserver /tmp/tkeyfile". */
#define KEYFILE			"/tmp/tkeyfile"
#define KEYTAB			"FILE:" ## KEYFILE

/* Prototypes for server. */
int main(int _1, char *_2[]);
void getspan_ep(rpc_binding_handle_t _1, idl_long_int _2, timestamp_t _3,
	idl_long_int *_4, error_status_t *_5);
int do_getspan(idl_long_int _1, timestamp_t _2);
int ref_mon(rpc_binding_handle_t _1, int _2);
int is_authorized(unsigned_char_t *_1, int _2);

void print_error(char *_1, error_status_t _2);

