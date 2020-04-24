/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl_util.c,v $
 * Revision 1.1.2.2  1996/03/09  20:50:35  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:39  marty]
 *
 * Revision 1.1.2.1  1995/12/11  21:56:15  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/02  20:27 UTC  bissen
 * 	merge from HPDCE01
 * 
 * 	HP revision /main/HPDCE01/1  1994/04/01  17:44 UTC  truitt
 * 
 * 	HP revision /main/truitt_junk/1  1994/04/01  16:58 UTC  truitt
 * 	move file to cds directory
 * 	[1995/12/11  20:56:06  root]
 * 
 * 	    Loading drop Dce1_0_1fnl
 * 	[1992/09/21  18:36:00  osfrcs]
 * 
 * Revision 1.1.8.2  1993/07/09  10:57:30  root
 * 	Initial King Kong branch
 * 	[1993/07/09  10:57:04  root]
 * 
 * Revision 1.1.6.2  1993/04/23  17:07:26  giza
 * 	DCE 1.0.1 Release
 * 	[1993/04/23  17:06:43  giza]
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1990, 1991, 1992 OPEN SOFTWARE FOUNDATION, INC. 
 * ALL RIGHTS RESERVED 
 */
/*
 * OSF DCE Version 1.0, UPDATE 1.0.1
 */
/*
 *
 *   	FILE_NAME: acl_util.c
 *
 *   	COMPONENT_NAME: dce.systest
 *
 *   	FUNCTIONS: make_header
 *		   check_status
 *		   gds_check_status
 *		   check_exit_code
 *		   show_gds_status
 *
 *   	ORIGINS: 27
 *
 *   	(C) COPYRIGHT International Business Machines Corp. 1991
 *   	All Rights Reserved
 *
 *   	FOR_TEST(s): dcegdacl
 *
 *   	USAGE: n/a
 *
 *   	DESCRIPTION:
 *
 *		This file contains functions used by clients and servers.
 *
 *      	Function make_header constructs a header to be logged with 
 *		all messages.  The header has the form of hostname, program 
 *		name and process id number.  This header aids in determining 
 *		which program logged which message.
 *
 *              Function check_status receives a status code, the expected
 *              value of the status, the message to log if the status code
 *              does not match the expected value, an opcode and a boolean 
 *		value.  If the values do not satisfy the opcode, an error 
 * 		message is constructed by appending the message passed to 
 *		the header.  The status value is appended to the error 
 *		message.   If the boolean value is true, the workstation
 *              will log the message via xx_severe and exit.  If the boolean
 *              value is false, the workstation will log the message via
 *              xx_error and continue.
 *
 *		Function gds_check_status performs the same functions as
 *		check_status but it works with status codes that of type
 *		DS_status, which is an OM construct.
 *
 *              Function check_exit_code simply calls xx_severe or xx_error
 *              with the error message passed to it, depending on the boolean
 *              exit status.  If the status is true, the workstation exits.
 *              If false, the testcase continues.
 *
 *		Function show_gds_status performs and om_get on the DS_status
 *		object passed to it and prints out the error number.  This
 *		will identify what kind of error occured.
 *
 *   	HISTORY:
 *      	05/10/90        Gerald Cantor           Initial design/code
 *		11/02/91	Gerald Cantor 		ANSIfied functions and
 *							updated prologs and
 *							comments.
 *		11/02/91	Gerald Cantor 		Removed mod parameter
 *							from check_status.
 */
#include <stdio.h>
#include <dce/rpcbase.h>
#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdsgds.h>
#include "acl_util.h"

char header[512], buf[128];

/*
 * make_header create a string that contains the hostname, program name and
 * process id number of the process.  This header is used for logging pur-
 * poses.
 */
void make_header(char *prog)
{
	int i = 0, pid_num = getpid();
	char host[30];

	gethostname(host, 30);
	sprintf(header, "host:%s\tprogram:%s\tpid:%d\n", host, prog, pid_num);
}

/*
 * check_status checks the status code passed with the expected results. If
 * they do not satisfy the opcode, a message is composed of the header string,
 * the message passed and the value of the status.  That message is logged as 
 * an error.
 */
void check_status(unsigned32 st, unsigned32 expected_status, int op_code, 
		  char *message, boolean32 exit_status)
{
	char *error_message = (char *)malloc(strlen(header) + strlen(message) + 20);
	int rc = 0;

	sprintf(error_message, "%s%s%s%d", header, message, "Status = ", st);
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
	free(error_message);
}

/*
 * gds_check_status checks the DS_status status code passed with the expected 
 * results. If they do not satisfy the opcode, a message is composed of the 
 * header string and the message passed.  That message is logged as an error.
 * gds_show_status() is called to print out the integer value of the error.
 */
void gds_check_status(DS_status st, DS_status expected_status, int op_code, 
		      char *message, boolean32 exit_status)
{
	char *error_message = (char *)malloc(strlen(header) + strlen(message) + 1);

	sprintf(error_message, "%s%s\n", header, message);
	switch(op_code)
	{
		case NOT_EQUAL:
			if (st != expected_status)	
			{
				show_gds_status(st);
				check_exit_code(exit_status, error_message);
			}
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
	free(error_message);
}

/*
 * check_exit_code determines whether or not to exit the test with a severe
 * error message or just to log an error message and continue based upon
 * the boolean value passed.
 */
void check_exit_code(boolean32 st, char *message)
{
	switch(st)
	{
		case TRUE:
			xx_severe(message);
			exit(1);
		case FALSE:
			xx_error(message);
			break;
	}
}

/*
 * show_gds_status gets the integer value of the error that occured.  This
 * will allow the tester to know what the error is.
 */
void show_gds_status(OM_private_object status)
{
	OM_public_object r_status_p;
	OM_value_position total_number;
	OM_return_code retc;

	if (status == NULL)
		return;
	/* get a public copy of r_status  */
        retc = (om_get(status, OM_NO_EXCLUSIONS, ((OM_type_list) 0),
                        OM_FALSE, ((OM_value_position) 0),
                        ((OM_value_position) 0), &r_status_p,
                        &total_number ));

	sprintf(buf, "Error value = %d", r_status_p->value.integer);
	xx_error(buf);	
}
