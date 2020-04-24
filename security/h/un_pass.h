/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: un_pass.h,v $
 * Revision 1.1.14.2  1996/02/18  23:01:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:03  marty]
 *
 * Revision 1.1.14.1  1995/12/08  17:30:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:36  root]
 * 
 * Revision 1.1.7.5  1992/12/29  13:28:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:47:28  zeliff]
 * 
 * Revision 1.1.7.4  1992/12/04  19:43:08  marty
 * 	OSC1.1.1 upgrade.
 * 	[1992/12/04  19:25:14  marty]
 * 
 * Revision 1.1.10.2  1992/11/17  02:33:26  weisman
 * 		Added CONST to setkey parameter declaration.
 * 	[1992/11/17  02:30:20  weisman]
 * 
 * Revision 1.1.7.3  1992/10/20  15:50:36  sommerfeld
 * 	Change _HPUX_SOURCE to __hpux in #ifdef
 * 	[1992/10/05  16:18:12  sommerfeld]
 * 
 * 	Flush bogus prototypes on 'UX.
 * 	[1992/09/30  21:45:00  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:43:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  un_pass.h V=3 11/11/91 //nbs/dds_tools/src
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      prototypes for unix password routines
** 
*/

#ifndef un_pass_h__included
#define un_pass_h__included

#if defined(__hpux)
#include <unistd.h>
#else

char *getpass ( 
#ifdef __STDC__
     const char *prompt
#endif
);

void setkey ( 
#ifdef __STDC__
     const char *key
#endif
);

void encrypt ( 
#ifdef __STDC__
     char *block,
     int  ignored
#endif
);
#endif
#endif
