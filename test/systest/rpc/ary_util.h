/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ary_util.h,v $
 * Revision 1.1.10.2  1996/03/09  20:50:53  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:53  marty]
 *
 * Revision 1.1.10.1  1995/12/13  21:55:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:10  root]
 * 
 * Revision 1.1.6.2  1993/07/09  12:24:57  root
 * 	Initial King Kong branch
 * 	[1993/07/09  12:24:30  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:25:28  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:53:47  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:01:19  mcfarland]
 * 
 * 	Remove common util.h routines
 * 	[1992/04/15  15:56:36  rps]
 * 
 * Revision 1.1.1.2  1992/05/28  15:11:57  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:37:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   	FILE_NAME: ary_util.h
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
 *   	FOR_TEST: dcerpary
 *
 *   	USAGE: n/a
 *
 *   	DESCRIPTION:
 *
 *		This file contains defines used by the function check_status
 *		and prototypes of functions in ary_util.c.  The defines allow
 *		check_status() to process status codes and handle message
 *		displaying and types of error handling to be done in a flex-
 *		able manner.
 *
 *   	HISTORY:
 *      	11/10/90        Gerald Cantor           Initial design/code.
 *		10/19/91	Gerald Cantor		Updated prologs.
 *		12/20/91	Gerald Cantor		Added print_exception().
 */
boolean32 compare_text(array_groups_t *);
