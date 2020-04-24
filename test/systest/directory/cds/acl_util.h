/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl_util.h,v $
 * Revision 1.1.2.2  1996/03/09  20:50:36  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:40  marty]
 *
 * Revision 1.1.2.1  1995/12/11  21:56:16  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/02  20:29 UTC  bissen
 * 	merge from HPDCE01
 * 
 * 	HP revision /main/HPDCE01/1  1994/04/01  17:44 UTC  truitt
 * 
 * 	HP revision /main/truitt_junk/1  1994/04/01  16:58 UTC  truitt
 * 	move file to cds directory
 * 	[1995/12/11  20:56:07  root]
 * 
 * 	    Loading drop Dce1_0_1fnl
 * 	[1992/09/21  18:36:01  osfrcs]
 * 
 * Revision 1.1.8.2  1993/07/09  10:58:46  root
 * 	Initial King Kong branch
 * 	[1993/07/09  10:58:10  root]
 * 
 * Revision 1.1.6.2  1993/04/23  16:23:09  giza
 * 	DCE 1.0.1 Release
 * 	[1993/04/23  16:22:18  giza]
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
 *   	FILE_NAME: acl_util.h
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
 *   	FOR_TEST(s): dcegdacl
 *
 *   	USAGE: n/a
 *
 *   	DESCRIPTION:
 *
 *		This file contains defines and function references used
 *		by acl_util.c.
 *
 *   	HISTORY:
 *      	5/01/91        Initial Design		Gerald Cantor
 *	       11/02/91		ANSIfied function
 *				declarations.		Gerald Cantor
 */
#define TRUE	1
#define FALSE	0
#define RPC	5
#define NOT_RPC	6
#define EQUAL   10
#define NOT_EQUAL 11    
#define GR_THAN   12
#define LS_THAN   13

void make_header(char *); 
void check_status(unsigned32, unsigned32, int, char *, boolean32);
void gds_check_status(DS_status, DS_status, int, char *, boolean32);
void check_exit_code(boolean32, char *);
void show_gds_status(OM_private_object);
