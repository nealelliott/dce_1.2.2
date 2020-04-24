/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xcred_test.c,v $
 * Revision 1.1.12.1  1996/10/02  19:03:21  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:47  damon]
 *
 * Revision 1.1.7.2  1994/07/13  22:30:49  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:12:08  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:56  mbs]
 * 
 * Revision 1.1.7.1  1994/06/09  14:25:50  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:37:47  annie]
 * 
 * Revision 1.1.3.3  1993/01/21  16:34:15  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:28:08  cjd]
 * 
 * Revision 1.1.3.2  1993/01/13  20:10:24  shl
 * 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
 * 	  Selected comments:
 * 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
 * 	    remove extra ; after RCSID.
 * 	    Remove the function dacl_SetOwnerControl.  It is not used.
 * 	[1993/01/12  22:37:11  shl]
 * 
 * Revision 1.1  1992/01/19  02:45:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * (C) Copyright 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * xcred_test.c
 *	Test program for the extended credential package, calling
 *	the user-level routines directly.
 */
#include <xcred.h>		/*Exported xcred interface*/

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/xcred/xcred_test.c,v 1.1.12.1 1996/10/02 19:03:21 damon Exp $")

main(argc, argv)
    int argc;
    char **argv;

{ /*main*/

    printf("\nTesting the extended credential package directly at\n");
    printf("the user level has not yet been implemented.\n\n");
    printf("Please stay tuned.\n");

} /*main*/
