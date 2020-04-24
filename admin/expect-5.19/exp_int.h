/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: exp_int.h,v $
 * Revision 1.1.2.1  1996/08/09  19:09:39  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:54:31  arvind]
 *
 * $EndLog$
 */

/* exp_int.h - private symbols common to both expect program and library

Written by: Don Libes, libes@cme.nist.gov, NIST, 12/3/90

Design and implementation of this program was paid for by U.S. tax
dollars.  Therefore it is public domain.  However, the author and NIST
would appreciate credit if this program or parts of it are used.
*/

#ifndef _EXPECT_INT_H
#define _EXPECT_INT_H

#ifndef TRUE
#define FALSE 0
#define TRUE 1
#endif

#ifndef HAVE_MEMCPY
#define memcpy(x,y,len) bcopy(y,x,len)
#endif

extern int errno;

int		Exp_StringMatch();
int		Exp_StringMatch2();
void		exp_console_set _ANSI_ARGS_((void));

#endif /* _EXPECT_INT_H */
