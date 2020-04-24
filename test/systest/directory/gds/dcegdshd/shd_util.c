/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: shd_util.c,v $
 * Revision 1.1.6.2  1996/02/17  23:26:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:32  marty]
 *
 * Revision 1.1.6.1  1995/12/11  21:57:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:57:32  root]
 * 
 * Revision 1.1.4.2  1994/03/23  16:06:57  keutel
 * 	xt_dump no longer exists
 * 	[1994/03/21  18:14:33  keutel]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/05  14:40:10  cjd]
 * 
 * Revision 1.1.4.1  1994/02/23  21:43:37  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:38:02  pellis]
 * 
 * Revision 1.1.2.2  1993/10/14  17:36:59  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:12  cmckeen]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/04  22:06:46  cjd]
 * 
 * Revision 1.1.2.1  1993/09/20  22:40:41  pellis
 * 	Moved from ./systest/directory/gds/*.
 * 	[1993/09/20  22:17:38  pellis]
 * 
 * Revision 1.1.2.2  1993/04/01  18:14:49  mhickey
 * 	Moved from systest/directory.
 * 	[1993/04/01  18:13:26  mhickey]
 * 
 * Revision 1.1.2.2  1992/03/18  01:22:24  akers
 * 	Initial code changes for Directory services.
 * 	[1992/03/05  22:37:03  akers]
 * 
 * Revision 1.1  1992/01/19  14:34:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   	FILE_NAME: shd_util.c
 *
 *   	COMPONENT_NAME: dce.systest
 *
 *   	FUNCTIONS: make_header
 *		   check_status
 *		   gds_check_status
 *		   check_exit_code
 *
 *   	ORIGINS: 27
 *
 *   	(C) COPYRIGHT International Business Machines Corp. 1991
 *   	All Rights Reserved
 *
 *   	FOR_TEST(s): dcegdshd
 *
 *   	USAGE: n/a
 *
 *   	DESCRIPTION:
 *
 *		This file contains functions used by view_obj.
 *
 *      	Function make_header constructs a header to be logged with 
 *		all messages.  The header has the form of hostname, program 
 *		name and process id number.  This header aids in determining 
 *		which program logged which message.
 *
 *      	Functions (gds_)check_status receives a status code, the 
 *		expected value of the status, the message to log if the 
 *		status code does not match the expected value and a boolean 
 *		value.  If the values do not match, an error message is 
 *		constructed by appending the message passed to the header.  
 *		If the boolean value is TRUE, the workstation will exit with 
 *		a call to puts.  If the boolean value is false, the 
 *		workstation will call puts and continue.
 *
 *   	HISTORY:
 *      	11/10/90        Gerald Cantor           Initial design/code
 *		12/18/90	Gerald Cantor		Convert check_status 
 *							to use boolean 
 *							parameter.
 */
#include <dce/dce.h>
#include "shd_util.h"

char header[512], buf[128];
void check_exit_code();

/*
 * make_header create a string that contains the hostname, program name and
 * process id number of the process.  This header is used for logging pur-
 * poses.
 */
void make_header(prog)
char *prog;
{
	int i = 0, pid_num = getpid();
	char host[30];

	gethostname(host, 30);
	sprintf(header, "host:%s\tprogram:%s\tpid:%d\n", host, prog, pid_num);
}

void gds_check_status(st, expected_status, op_code, message, exit_status)
DS_status st;
DS_status expected_status;
int op_code;
char *message;
long exit_status;
{
	char error_message[512];

	sprintf(error_message, "%s%s", header, message);
	switch(op_code)
	{
		case NOT_EQUAL:
			if (st != expected_status)	
				check_exit_code(exit_status, error_message);
			break;
		case EQUAL:
			if (st == expected_status)	
				check_exit_code(exit_status, error_message);
			break;
		case GR_THAN:
			if (st > expected_status)	
				check_exit_code(exit_status, error_message);
			break;
		case LS_THAN:
			if (st < expected_status)	
				check_exit_code(exit_status, error_message);
			break;
	}
}

/*
 * check_exit_code determines whether or not to exit the test with a severe
 * error message or just to log an error message and continue based upon
 * the boolean value passed.
 */
void check_exit_code(st, message)
long st;
char *message;
{
	switch(st)
	{
		case TRUE:
			puts(message);
			exit(1);
		case FALSE:
			puts(message);
			break;
	}
}
