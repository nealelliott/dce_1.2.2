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
 * $Log: exp_tty.h,v $
 * Revision 1.1.2.1  1996/08/09  19:15:07  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:58:42  arvind]
 *
 * $EndLog$
 */

/* exp_tty.h - tty support definitions

Design and implementation of this program was paid for by U.S. tax
dollars.  Therefore it is public domain.  However, the author and NIST
would appreciate credit if this program or parts of it are used.
*/

#ifndef __EXP_TTY_H__
#define __EXP_TTY_H__

#include <expect_cf.h>

extern int exp_dev_tty;
extern int exp_ioctled_devtty;
extern int exp_stdin_is_tty;
extern int exp_stdout_is_tty;

void exp_tty_raw();
void exp_tty_echo();
void exp_tty_break();
int exp_tty_raw_noecho();
int exp_israw();
int exp_isecho();

void exp_tty_set();
int exp_tty_set_simple();
int exp_tty_get_simple();

#endif	/* __EXP_TTY_H__ */
