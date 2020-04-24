/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util.h,v $
 * Revision 1.1.8.2  1996/02/18  23:08:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:33  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:51:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:23:51  root]
 * 
 * Revision 1.1.4.2  1993/02/05  15:29:44  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:50:28  cjd]
 * 
 * Revision 1.1.2.2  1992/04/09  17:31:51  rps
 * 	Created from merge of rpc/*_util.h
 * 	[1992/04/09  17:28:24  rps]
 * 
 * $EndLog$
 */
/*
 *
 *   	FILE_NAME: util.h
 *
 *   	COMPONENT_NAME: dce.systest
 *
 *   	FUNCTIONS: none
 *
 *   	ORIGINS: 2 27
 *
 *   	(C) COPYRIGHT International Business Machines Corp. 1990
 *   	All Rights Reserved
 *
 *   	FOR_TEST: dcerp*
 *
 *   	USAGE: n/a
 *
 *   	DESCRIPTION:
 *
 *              This file contains defines used by the function check_status
 *              and prototypes of functions in rec_util.c.  The defines allow
 *              check_status() to process status codes and handle message
 *              displaying and types of error handling to be done in a flex-
 *              able manner.
 *
 *		Defines for what type of namespace entry to create are also
 *		specified.
 *
 *   	HISTORY:
 *      	5/01/91        	Gerald Cantor		Initial design/code.
 *		10/19/91	Gerald Cantor 		Updated prologs and
 *							ANSIfied functions.
 *		10/19/91	Gerald Cantor 		Added contents of
 *							globals.h to rec_util.h.
 *		01/03/92	Gerald Cantor		Added declaration for
 *							print_exception().
 */
#include <dce/cma.h>

#define RPC	5       /* status originating from a RPC call */
#define NOT_RPC	6       /* status originating from a any other call */
#define EQUAL		10
#define NOT_EQUAL	11
#define GR_THAN		12
#define LS_THAN		13
#define SERVER_ENTRY    102 	/* create a server entry in the namespace */
#define SERVICE_GROUP   103 	/* place the server entry in a service group */
#define CONFIG_PROFILE  104 	/* place the server entry in a config profile */

void make_header(char *);
int check_status(error_status_t, error_status_t, int, char *, boolean32, int);
int check_exit_code(boolean32, char *);
void string_binding_to_name(unsigned_char_p_t, char *,
			    unsigned_char_p_t *, unsigned_char_p_t *);
char *name_from_addr(char *);
char *addr_from_name(char *);
idl_boolean close_sf (idl_short_float, idl_short_float);
idl_boolean close_lf (idl_long_float, idl_long_float);
void print_exception(EXCEPTION *);
