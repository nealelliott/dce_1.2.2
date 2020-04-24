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
 * $Log: expect_cf.h,v $
 * Revision 1.1.2.2  1996/10/03  17:54:52  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  17:22:57  arvind]
 *
 * Revision 1.1.2.1  1996/08/09  19:18:15  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  19:06:33  arvind]
 * 
 * $EndLog$
 */
/* expect_cf.h.  Generated automatically by configure.  */
/*
 * Check for headers
 */
#ifndef __EXPECT_CF_H__
#define __EXPECT_CF_H__

/* #undef NO_STDLIB_H */		/* Tcl requires this name */
#define HAVE_STDARG_H 1
#define HAVE_VARARGS_H 1
#define HAVE_STROPTS_H 1
#define HAVE_SYSCONF_H 1
/* #undef HAVE_SYS_FCNTL_H */
#define HAVE_SYS_WAIT_H 1
/* #undef HAVE_SYS_BSDTYPES_H */	/* nice ISC special */
#define HAVE_SYS_SELECT_H 1	/* nice ISC special */
#define HAVE_SYS_TIME_H 1		/* nice ISC special */
/* #undef HAVE_SYS_PTEM_H */		/* SCO needs this for window size */
/* #undef HAVE_STRREDIR_H */		/* Solaris needs this for console redir */
/* #undef HAVE_STRPTY_H */		/* old-style Dynix ptys need this */
#define HAVE_UNISTD_H 1
#define HAVE_SYSMACROS_H 1
/* #undef HAVE_TIOCGWINSZ_IN_TERMIOS_H */
/* #undef HAVE_TCGETS_OR_TCGETA_IN_TERMIOS_H */

/* #undef pid_t */
#define RETSIGTYPE void
#define TIME_WITH_SYS_TIME 1	/* ok to include both time.h and sys/time.h */

/*
 * This section is for compile macros needed by
 * everything else.
 */

/*
 * Check for functions
 */
#define HAVE_MEMCPY 1
#define HAVE_SYSCONF 1
/* #undef SIMPLE_EVENT */
#define HAVE_STRFTIME 1
#define HAVE_MEMMOVE 1
/* #undef HAVE_TIMEZONE */		/* timezone() a la Pyramid */
/* #undef HAVE_TCLREGERROR */
#define HAVE_STRCHR 1

#ifndef HAVE_STRCHR
#define strchr(s,c) index(s,c)
#endif /* HAVE_STRCHR */

/*
 * timezone
 */
#define HAVE_SV_TIMEZONE 1

/*
 * wait status type
 */
/* #undef NO_UNION_WAIT */

/* #undef WNOHANG_REQUIRES_POSIX_SOURCE */

/*
 * Signal stuff. Setup the return type
 * and if signals need to be re-armed.
 */
/*#ifndef RETSIGTYPE*/
/*#define RETSIGTYPE void*/
/*#endif*/
#define REARM_SIG 1

/*
 * Generate correct type for select mask
 */
#ifndef SELECT_MASK_TYPE
#define SELECT_MASK_TYPE fd_set
#endif

/*
 * Check how stty works
 */
/* #undef STTY_READS_STDOUT */

/*
 * Check for tty/pty functions and structures
 */
#define POSIX 1
#define HAVE_TCSETATTR 1
#define HAVE_TERMIO 1
#define HAVE_TERMIOS 1
#define HAVE_SGTTYB 1
/* #undef HAVE__GETPTY */
/* #undef HAVE_GETPTY */
/* #undef HAVE_PTC */
#define HAVE_PTC_PTS 1
/* #undef HAVE_PTYM */
/* #undef HAVE_PTYTRAP */
/* #undef HAVE_PTMX */
/* #undef HAVE_PTMX_BSD */

/*
 * Special hacks
 */
/* #undef CONVEX */
/* #undef SUNOS */

#ifdef SUNOS
#define __EXTENSIONS__
#endif /* SUNOS */

#define WNOHANG_BACKUP_VALUE 1

#if (TCL_MAJOR_VERSION == 7 && TCL_MINOR_VERSION >= 5) || TCL_MAJOR_VERSION > 7
#define tcl_AsyncReady Tcl_AsyncReady()
#endif

#endif	/* __EXPECT_CF_H__ */
