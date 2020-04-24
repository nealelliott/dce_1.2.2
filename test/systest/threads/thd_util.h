/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: thd_util.h,v $
 * Revision 1.1.8.2  1996/02/18  23:08:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:28  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:50:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:23:22  root]
 * 
 * Revision 1.1.3.2  1993/02/05  15:27:23  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:48:28  cjd]
 * 
 * Revision 1.1.1.2  1993/02/05  13:15:35  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1  1992/01/19  14:35:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   	FILE_NAME: thd_util.h
 *
 *   	COMPONENT_NAME: dce.systest
 *
 *   	FUNCTIONS: none
 *
 *   	ORIGINS: 27
 *
 *   	(C) COPYRIGHT International Business Machines Corp. 1990
 *   	All Rights Reserved
 *
 *   	FOR_TEST: dcethrpc
 *
 *   	USAGE: n/a
 *
 *   	DESCRIPTION:
 *
 *		This file contains defines used by the function check_status
 *		and prototypes of functions in thd_util.c.  The defines allow
 *		check_status() to process status codes and handle message
 *		displaying and types of error handling to be done in a flex-
 *		able manner.
 *
 *   	HISTORY:
 *      	11/10/90        Gerald Cantor           Initial design/code.
 *		10/19/91	Gerald Cantor		Updated prologs.
 *		10/21/91	Gerald Cantor		ANSIfied functions.
 */
#define RPC	5 	/* status originating from a RPC call */
#define NOT_RPC	6 	/* status originating from a any other call */
#define EQUAL   10
#define NOT_EQUAL 11    
#define GR_THAN   12
#define LS_THAN   13

void make_header(char *); 
int check_status(error_status_t, error_status_t, int, char *, boolean32, int);
void string_binding_to_name(unsigned_char_p_t, char *, unsigned_char_p_t *p); 
char *addr_from_name(char *);
void print_exception(EXCEPTION *);
