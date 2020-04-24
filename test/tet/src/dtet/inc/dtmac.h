/*
 *      SCCS:  @(#)dtmac.h	1.2 (92/09/30) 
 *
 *	UniSoft Ltd., London, England
 *
 * (C) Copyright 1992 X/Open Company Limited
 *
 * All rights reserved.  No part of this source code may be reproduced,
 * stored in a retrieval system, or transmitted, in any form or by any
 * means, electronic, mechanical, photocopying, recording or otherwise,
 * except as stated in the end-user licence agreement, without the prior
 * permission of the copyright owners.
 *
 * X/Open and the 'X' symbol are trademarks of X/Open Company Limited in
 * the UK and other countries.
 */

/************************************************************************

SCCS:   	@(#)dtmac.h	1.2 92/09/30 DTET release 1.0b
NAME:		dtmac.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	useful macros used in various parts of the system
	macros that interface to the trace subsystem

MODIFICATIONS:

************************************************************************/

/*
**	first some macros that might be system-dependent
*/

/* SIG_T is the type for the return value of signal(),
   SIG_FUNC_T is the type for the function argument to signal() */
#ifdef SVR2
#define SIG_T		int
#define SIG_FUNC_T	void
#else
#if defined(BSD42) || defined(BSD43)
#define SIG_T		int
#define SIG_FUNC_T	int
#else
#define SIG_T		void
#define SIG_FUNC_T	void
#endif /* BSD */
#endif /* SVR2 */

/* define NOMKDIR if the system does not have the mkdir() and rmdir() system
	calls - see dtetlib/mkdir.c */
#ifdef SVR2
#define NOMKDIR
#endif

/* decide what type of directory access method to use;
	these defines determine which include files are used */
#ifdef SVR2
#define NDIR	/* include <ndir.h> */
#else
#if defined(BSD42) || defined(BSD43) || defined(ultrix)
#define SYSDIR	/* include <sys/dir.h> */
#else
#define DIRENT	/* include <dirent.h> */
#endif /* BSD */
#endif /* SVR2 */

/* choose whether to include function declaration files (like <stdlib.h>)
	or declare non-int functions locally in each file */
#if defined(SVR2) || defined(BSD42) || defined(BSD43)
#define LOCAL_FUNCTION_DECL
#endif


/*
**	then macros that are common to all systems
*/

/* define TESTING here if it's not on the cc command line */
#ifndef TESTING
#define TESTING 0
#endif

/* maximimum and minimum macros */
#define MAX(a, b)	((a) > (b) ? (a) : (b))
#define MIN(a, b)	((a) < (b) ? (a) : (b))

/* maximum length for a path name - needs to be system-independent */
#define MAXPATH		1024

/* maximum length for a system name as found in the DTET systems file */
#define SNAMELEN	32


/* trace macros */
#ifdef NOTRACE

#define TRACE1(flag, level, s1)
#define TRACE2(flag, level, s1, s2)
#define TRACE3(flag, level, s1, s2, s3)
#define TRACE4(flag, level, s1, s2, s3, s4)
#define TRACE5(flag, level, s1, s2, s3, s4, s5)
#define TRACE6(flag, level, s1, s2, s3, s4, s5, s6)
#define TDUMP(flag, level, from, count, title)
#define BUFCHK(bpp, lp, newlen)	bufchk(bpp, lp, newlen)

#else /* NOTRACE */

#define TRACE1(flag, level, s1)	\
	TRACE2(flag, level, s1, (char *) 0)

#define TRACE2(flag, level, s1, s2) \
	TRACE3(flag, level, s1, s2, (char *) 0)

#define TRACE3(flag, level, s1, s2, s3) \
	TRACE4(flag, level, s1, s2, s3, (char *) 0)

#define TRACE4(flag, level, s1, s2, s3, s4) \
	TRACE5(flag, level, s1, s2, s3, s4, (char *) 0)

#define TRACE5(flag, level, s1, s2, s3, s4, s5) \
	TRACE6(flag, level, s1, s2, s3, s4, s5, (char *) 0)

#define TRACE6(flag, level, s1, s2, s3, s4, s5, s6) \
	if ((flag) >= (level)) trace(s1, s2, s3, s4, s5, s6); else

#define TDUMP(flag, level, from, count, title) \
	if ((flag) >= (level)) tdump(from, count, title); else

#define BUFCHK(bpp, lp, newlen) \
	buftrace(bpp, lp, newlen, srcFile, __LINE__)

#ifndef NEEDsrcFile
#define NEEDsrcFile
#endif


/*
**	declarations of extern functions and data items
*/

extern int Tbuf, Tio, Tloop, Tserv, Tsyncd, Ttcc, Ttccd, Ttcm, Ttrace, Txresd;

#if __STDC__

extern void tdump(char *, int, char *);
extern void trace(char *, char *, char *, char *, char *, char *);
extern char **traceargs(int, char **);
extern void traceinit(int, char **);

#else /* __STDC__ */

extern void tdump();
extern void trace();
extern char **traceargs();
extern void traceinit();

#endif /* __STDC__ */


#endif	/* NOTRACE */

