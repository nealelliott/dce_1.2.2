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
 * $Log: expect.h,v $
 * Revision 1.1.2.1  1996/08/09  19:16:15  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:59:42  arvind]
 *
 * $EndLog$
 */

/* expect.h - include file for using the expect library, libexpect.a
from C or C++ (i.e., without Tcl)

Written by: Don Libes, libes@cme.nist.gov, NIST, 12/3/90

Design and implementation of this program was paid for by U.S. tax
dollars.  Therefore it is public domain.  However, the author and NIST
would appreciate credit if this program or parts of it are used.
*/

#ifndef _EXPECT_H
#define _EXPECT_H

#include <expect_comm.h>

enum exp_type {
	exp_end = 0,		/* placeholder - no more cases */
	exp_glob,		/* glob-style */
	exp_exact,		/* exact string */
	exp_regexp,		/* regexp-style, uncompiled */
	exp_compiled,		/* regexp-style, compiled */
	exp_null,		/* matches binary 0 */
	exp_bogus		/* aid in reporting compatibility problems */
};

struct exp_case {		/* case for expect command */
	char *pattern;
	regexp *re;
	enum exp_type type;
	int value;		/* value to be returned upon match */
};

EXTERN char *exp_buffer;		/* buffer of matchable chars */
EXTERN char *exp_buffer_end;		/* one beyond end of matchable chars */
EXTERN char *exp_match;			/* start of matched string */
EXTERN char *exp_match_end;		/* one beyond end of matched string */
EXTERN int exp_match_max;		/* bytes */
EXTERN int exp_timeout;			/* seconds */
EXTERN int exp_full_buffer;		/* if true, return on full buffer */
EXTERN int exp_remove_nulls;		/* if true, remove nulls */

EXTERN int exp_pty_timeout;		/* see Cray hooks in source */
EXTERN int exp_pid;			/* process-id of spawned process */
EXTERN int exp_autoallocpty;		/* if TRUE, we do allocation */
EXTERN int exp_pty[2];			/* master is [0], slave is [1] */
EXTERN char *exp_pty_slave_name;	/* name of pty slave device if we */
					/* do allocation */
EXTERN char *exp_stty_init;		/* initial stty args */
EXTERN int exp_ttycopy;			/* copy tty parms from /dev/tty */
EXTERN int exp_ttyinit;			/* set tty parms to sane state */
EXTERN int exp_console;			/* redirect console */

EXTERN jmp_buf exp_readenv;		/* for interruptable read() */
EXTERN int exp_reading;			/* whether we can longjmp or not */
#define EXP_ABORT	1		/* abort read */
#define EXP_RESTART	2		/* restart read */

EXTERN int exp_logfile_all;
EXTERN FILE *exp_debugfile;
EXTERN FILE *exp_logfile;

EXTERN int exp_disconnect EXP_PROTO(());
EXTERN FILE *exp_popen	EXP_PROTO((char *command));
EXTERN void (*exp_child_exec_prelude) EXP_PROTO((void));

#ifndef EXP_DEFINE_FNS
EXTERN int exp_spawnl	EXP_PROTOV(EXP_VARARGS(char *,file));
EXTERN int exp_expectl	EXP_PROTOV(EXP_VARARGS(int,fd));
EXTERN int exp_fexpectl	EXP_PROTOV(EXP_VARARGS(FILE *,fp));
/*EXTERN int exp_spawnl	EXP_PROTOV((char *file EXP_VARARGS));*/
/*EXTERN int exp_expectl	EXP_PROTOV((int fd EXP_VARARGS));*/
/*EXTERN int exp_fexpectl	EXP_PROTOV((FILE *fp EXP_VARARGS));*/
#endif

EXTERN int exp_spawnv	EXP_PROTO((char *file, char *argv[]));
EXTERN int exp_expectv	EXP_PROTO((int fd, struct exp_case *cases));
EXTERN int exp_fexpectv	EXP_PROTO((FILE *fp, struct exp_case *cases));

EXTERN int exp_spawnfd	EXP_PROTO((int fd));

#endif /* _EXPECT_H */
