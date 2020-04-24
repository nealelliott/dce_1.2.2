/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: args.h,v $
 * Revision 1.1.4.2  1996/02/18  22:57:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:44  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:24:24  root
 * 	Submit
 * 	[1995/12/11  15:14:09  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:05:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:07  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:41:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  args.h V=2 04/29/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      edrgy command line argument parsing routines - public header file
** 
*/

#ifndef _args_h_included
#define _args_h_included

/* 
 * args_get 
 * takes a string as input (buf) and yields a count of arguments and
 * an "argv" buffer of pointers to each argument.
 */

void args_get(
#ifdef __STDC__
    char  *buf,
    int   *argc,
    char  ***argv
#endif
);

/* 
 * match_command 
 * takes a key and string as input and returns whether the
 * string matches the key where at least min_len characters
 * of the key are required to be specified.
 */


boolean32 match_command(
#ifdef __STDC__
   char *key,
   char *str,
   int  min_len
#endif
);

#endif
