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
 * $Log: exp_prog.h,v $
 * Revision 1.1.2.1  1996/08/09  19:12:09  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:56:34  arvind]
 *
 * $EndLog$
 */

/* exp_prog.h - private symbols common to both expect program and library

Written by: Don Libes, libes@cme.nist.gov, NIST, 12/3/90

Design and implementation of this program was paid for by U.S. tax
dollars.  Therefore it is public domain.  However, the author and NIST
would appreciate credit if this program or parts of it are used.
*/

#ifndef _EXPECT_PROG_H
#define _EXPECT_PROG_H

#include <expect_tcl.h>
#include <exp_int.h>

/* yes, I have a weak mind */
#define streq(x,y)	(0 == strcmp((x),(y)))

#endif /* _EXPECT_PROG_H */
