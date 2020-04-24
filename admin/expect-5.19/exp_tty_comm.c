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
 * $Log: exp_tty_comm.c,v $
 * Revision 1.1.2.1  1996/08/09  19:15:18  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:58:52  arvind]
 *
 * $EndLog$
 */

/* exp_tty_comm.c - tty support routines common to both Expect program
   and library */

#include <expect_cf.h>
#include <stdio.h>

#include <exp_tty_in.h>
#include <exp_rename.h>
#include <exp_log.h>

#ifndef TRUE
#define FALSE 0
#define TRUE 1
#endif

int exp_disconnected = FALSE;		/* not disc. from controlling tty */

/*static*/ exp_tty exp_tty_current, exp_tty_cooked;
#define tty_current exp_tty_current
#define tty_cooked exp_tty_cooked

void
exp_init_tty()
{
	extern exp_tty exp_tty_original;

	/* save original user tty-setting in 'cooked', just in case user */
	/* asks for it without earlier telling us what cooked means to them */
	tty_cooked = exp_tty_original;

	/* save our current idea of the terminal settings */
	tty_current = exp_tty_original;
}

