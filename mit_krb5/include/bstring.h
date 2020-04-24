/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: bstring.h,v $
 * Revision 1.1.2.1  1996/06/05  20:24:52  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:25:13  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * #include file for bstring(3) & sys5 version functions
 * home.
 */
#ifndef __BSTRING__
#define __BSTRING__
#if defined(__STDC__) || defined(_WINDOWS)
/* compat-sys5 */
/* these are in <string.h> */
extern int bcmp  (const char *, const char *, int );
extern int bcopy  (const char *, char *, int );
extern int bzero  (char *, int );
#else /* STDC */
/* compat-sys5 */
extern char *memccpy  ();
extern char *memchr  ();
extern int memcmp  ();
extern char *memcpy  ();
extern char *memset  ();

extern int bcmp  ();
extern int bcopy  ();
extern int bzero  ();

#endif /* STDC */
#endif /* __BSTRING__ */
