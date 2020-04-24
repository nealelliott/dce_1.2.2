/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sysdep.h,v $
 * Revision 1.1.4.2  1996/02/18  23:46:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:56  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:37:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:18:37  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:34:03  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:48:43  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:33:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:14:22  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:00:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/****************************************************************/
/*  sysdep.h  - operating system and compiler dependencies      */
/****************************************************************/

/*
 * ==========================================================================
 * Confidential and Proprietary.  Copyright 1987 by Apollo Computer Inc.,
 * Chelmsford, Massachusetts.  Unpublished -- All Rights Reserved Under
 * Copyright Laws Of The United States.
 *
 * Apollo Computer Inc. reserves all rights, title and interest with respect
 * to copying, modification or the distribution of such software programs and
 * associated documentation, except those rights specifically granted by Apollo
 * in a Product Software Program License, Source Code License or Commercial
 * License Agreement (APOLLO NETWORK COMPUTING SYSTEM) between Apollo and
 * Licensee.  Without such license agreements, such software programs may not
 * be used, copied, modified or distributed in source or object code form.
 * Further, the copyright notice must appear on the media, the supporting
 * documentation and packaging as set forth in such agreements.  Such License
 * Agreements do not grant any rights to use Apollo Computer's name or trademarks
 * in advertising or publicity, with respect to the distribution of the software
 * programs without the specific prior written permission of Apollo.  Trademark
 * agreements may be obtained in a separate Trademark License Agreement.
 * ==========================================================================
 */

#ifndef sysdep_incl
#define sysdep_incl

/*
  define HASDIRTREE if OS has foo/widget/bar file system.
  if HASDIRTREE, define BRANCHCHAR and BRANCHSTRING appropriately
  define HASPOPEN if system can do popen()
  define HASINODES if system has real inodes returned by stat()
*/

/* MSDOS */
#ifdef MSDOS
#define BRANCHCHAR '\\'
#define BRANCHSTRING "\\"
#define HASDIRTREE
#define DEFAULT_IDIR "\\ncs\\include\\idl"
#define CD_IDIR "."
#ifdef TURBOC
#define stat(a,b) turboc_stat(a,b)
#endif
#endif


/* VAX VMS  */
#ifdef VMS
#define HASDIRTREE
#define BRANCHCHAR ']'
#define BRANCHSTRING "]"
#define DEFAULT_IDIR "ncs$idl:"
#define CD_IDIR "[]"
#endif


#if defined(BSD) || defined(SYS5) || defined(mips) || defined(ultrix)
#define UNIX
#define HASDIRTREE
#define HASPOPEN
#define HASINODES
#define BRANCHCHAR '/'
#define BRANCHSTRING "/"
#define DEFAULT_IDIR "/usr/include/idl"
#define CD_IDIR "."
#endif

#endif
