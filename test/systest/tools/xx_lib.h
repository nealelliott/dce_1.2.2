/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xx_lib.h,v $
 * Revision 1.1.10.2  1996/02/18  23:08:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:36  marty]
 *
 * Revision 1.1.10.1  1995/12/11  22:51:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:23:56  root]
 * 
 * Revision 1.1.8.2  1993/07/28  19:40:48  cmckeen
 * 	HP's TET'ized version of the system tests
 * 	[1993/07/27  13:56:15  cmckeen]
 * 
 * Revision 1.1.4.3  1993/05/27  19:03:15  eheller
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.2  1993/05/27  14:23:40  eheller
 * 	*** empty log message ***
 * 	[1993/05/26  18:47:23  eheller]
 * 
 * Revision 1.1.5.2  1993/04/23  16:20:27  sekhar
 * 	Merged Hp_dce1_0_1 changes
 * 	[1993/04/16  15:03:38  sekhar]
 * 
 * Revision 1.1.1.7  1993/03/11  22:17:03  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.2.2  1993/02/05  15:29:55  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:50:36  cjd]
 * 
 * Revision 1.1  1992/01/19  14:33:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   FILE_NAME: xx_lib.h
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS: None
 *
 *   ORIGINS: 27
 *
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1991
 *   All Rights Reserved
 *
 *
 *   FOR_TEST: Any
 *
 *   DESCRIPTION:
 *      This is the include file for the C version of the
 *      DCE System Test logging tools.
 *
 *   HISTORY: 
 *      11/20/90   Ken Walker     Initial code
 */

int xx_error();
int xx_log();
int xx_severe();
int xx_warning();
