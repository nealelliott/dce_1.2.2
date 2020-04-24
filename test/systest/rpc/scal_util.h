/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: scal_util.h,v $
 * Revision 1.1.8.2  1996/03/09  20:51:36  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:21  marty]
 *
 * Revision 1.1.8.1  1995/12/13  21:55:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:24  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:22:09  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:21:42  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:38:06  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:58:38  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:05:37  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:14:46  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:37:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *      FILE_NAME: scal_util.h
 *
 *      COMPONENT_NAME: dce.systest
 *
 *      FUNCTIONS: none
 *
 *      ORIGINS: 27
 *
 *      (C) COPYRIGHT International Business Machines Corp. 1990
 *      All Rights Reserved
 *
 *      FOR_TEST(s): dcerpid3
 *
 *      USAGE: n/a
 *
 *      DESCRIPTION:
 *
 *              This file contains defines used by the function check_status.
 *              The defines allow check_status() to process status codes and
 *              handle message displaying and types of error handling to be
 *              done in a flexable manner.
 *
 *      HISTORY:
 *              11/10/90        Gerald Cantor           Initial design/code
 *              12/19/91	Gerald Cantor		Updated prologs.
 */
#define RPC	5
#define NOT_RPC	6
#define EQUAL   10
#define NOT_EQUAL 11    
#define GR_THAN   12
#define LS_THAN   13
