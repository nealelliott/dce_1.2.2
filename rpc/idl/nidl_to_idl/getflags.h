/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: getflags.h,v $
 * Revision 1.1.4.2  1996/02/18  23:46:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:48  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:36:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:18:05  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:14:50  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:47:50  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:15:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:11:01  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:00:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/****************************************************************/
/*  getflags.h  - constants for command line parsing            */
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

#ifndef __PROTOTYPE
#ifdef __STDC__
#define __PROTOTYPE(x) x
#else
#define __PROTOTYPE(x) ()
#endif
#endif

extern int atoi __PROTOTYPE((char *));
extern double atof __PROTOTYPE((char *));
extern long atol  __PROTOTYPE((char *));

typedef char *FLAGDEST;
typedef struct options
        {
        char *option;
        int ftype;
        FLAGDEST dest;
        } OPTIONS;

#define INTARG 0
#define STRARG 1
#define TOGGLEARG 2
#define CHRARG 3
#define FLTARG 4
#define LONGARG 5
#define ASSERTARG 6
#define DENYARG 7

#define HIDARG (128 << 8)
#define VARARGFLAG 64
#define MULTARGMASK 63

#define MULTARG(n, a) (((n) << 8) + a)
#define AINTARG(n) MULTARG(n,INTARG)
#define VINTARG(n) AINTARG(n|VARARGFLAG)
#define ASTRARG(n) MULTARG(n,STRARG)
#define VSTRARG(n) ASTRARG(n|VARARGFLAG)
#define ATOGGLEARG(n) MULTARG(n,TOGGLEARG)
#define AASSERTARG(n) MULTARG(n,ASSERTARG)
#define ADENYARG(n) MULTARG(n,DENYARG)
#define ACHRARG(n) MULTARG(n,CHRARG)
#define VCHRARG(n) ACHRARG(n|VARARGFLAG)
#define AFLTARG(n) MULTARG(n,FLTARG)
#define VFLTARG(n) AFLTARG(n|VARARGFLAG)
#define ALONGARG(n) MULTARG(n,LONGARG)
#define VLONGARG(n) AFLTARG(n|VARARGFLAG)

#define GETINT(s) {s = atoi(*++av); ac--;}
#define GETSTR(s) {s = *++av;ac--;}
#define GETCH(s) {av++; s = av[0][0]; ac--;}
#define GETFLT(s) {s = atof(*++av); ac--;}
#define GETLONG(s) {s = atol(*++av); ac--;}

void printflags __PROTOTYPE((OPTIONS table[]));
void getflags __PROTOTYPE((int argc, char **argv, OPTIONS table[]));
void flags_incr_count __PROTOTYPE((OPTIONS table[], char *option, int delta));
int flags_option_count __PROTOTYPE((OPTIONS table[], char *option));
int flags_other_count __PROTOTYPE((void));
char *flags_other __PROTOTYPE((int index));
