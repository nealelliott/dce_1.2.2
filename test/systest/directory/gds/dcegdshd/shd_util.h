/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: shd_util.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:19  marty]
 *
 * 	Embedded copyright notice
 * 	[1993/02/05  14:40:15  cjd]
 *
 * Revision 1.1.4.1  1995/12/11  21:57:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:57:34  root]
 * 
 * Revision 1.1.2.2  1993/10/14  17:37:01  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:14  cmckeen]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/04  22:06:52  cjd]
 * 
 * Revision 1.1.2.1  1993/09/20  22:40:42  pellis
 * 	Moved from ./systest/directory/gds/*.
 * 	[1993/09/20  22:17:48  pellis]
 * 
 * Revision 1.1.2.2  1993/04/01  18:14:54  mhickey
 * 	Moved from systest/directory
 * 	[1993/04/01  18:14:07  mhickey]
 * 
 * Revision 1.1.2.4  1992/07/15  22:13:48  gmd
 * 	Correct include syntax for xom.h (again) due to
 * 	big xds/gds drop changing header file export location.
 * 	[1992/07/15  22:13:31  gmd]
 * 
 * Revision 1.1.2.3  1992/06/05  22:15:00  gmd
 * 	Include syntax for xds header files must include dce/ prefix
 * 	[1992/06/05  21:05:34  gmd]
 * 
 * Revision 1.1.2.2  1992/03/18  01:22:31  akers
 * 	Initial code changes for Directory services system test.
 * 	[1992/03/05  22:37:57  akers]
 * 
 * Revision 1.1  1992/01/19  14:34:57  devrcs
 * 	Initial revision
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
 *   	ORIGINS: 27
 *
 *   	(C) COPYRIGHT International Business Machines Corp. 1990
 *   	All Rights Reserved
 *
 *   	FOR_TEST(s): dcegdshd
 *
 *   	USAGE: n/a
 *
 *   	DESCRIPTION:
 *
 *		This file contains defines and function references needed
 *		by view_obj.
 *
 *   	HISTORY:
 *      	5/01/91        Initial Design		Gerald Cantor
 */
#ifndef SHD_UTIL_HEADER
#define SHD_UTIL_HEADER
#define RPC	5
#define NOT_RPC	6
#define EQUAL   10
#define NOT_EQUAL 11    
#define GR_THAN   12
#define LS_THAN   13
#include <xom.h>
#include <xomip.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdsgds.h>

void make_header(); 
void check_status();
void gds_check_status();
#endif














