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
 * $Log: exp_pty.h,v $
 * Revision 1.1.2.1  1996/08/09  19:12:50  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:56:48  arvind]
 *
 * $EndLog$
 */

/* exp_pty.h - declarations for pty allocation and testing

Written by: Don Libes, NIST,  3/9/93

Design and implementation of this program was paid for by U.S. tax
dollars.  Therefore it is public domain.  However, the author and NIST
would appreciate credit if this program or parts of it are used.

*/

int exp_pty_test_start();
void exp_pty_test_end();
int exp_pty_test();
void exp_pty_unlock();
int exp_pty_lock();

extern char *exp_pty_slave_name;
