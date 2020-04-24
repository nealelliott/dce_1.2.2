/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: unistd.h,v $
 * Revision 1.1.6.2  1996/02/18  23:32:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:52  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:28:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:00:51  root]
 * 
 * Revision 1.1.4.1  1994/06/09  16:03:44  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:49:05  devsrc]
 * 
 * Revision 1.1.2.1  1993/12/12  01:38:19  rousseau
 * 	Initial 7.3 version.
 * 	[1993/12/12  01:37:56  rousseau]
 * 
 * $EndLog$
 */
/*
 * unistd.h --
 *
 *      Macros, CONSTants and prototypes for Posix conformance.
 *
 * Copyright 1989 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * $Header: /u0/rcs_trees/dce/rcs/admin/tcl7.3/compat/unistd.h,v 1.1.6.2 1996/02/18 23:32:22 marty Exp $
 */

#ifndef _UNISTD
#define _UNISTD

#include <sys/types.h>
#ifndef _TCL
#   include "tcl.h"
#endif

#ifndef NULL
#define NULL    0
#endif

/* 
 * Strict POSIX stuff goes here.  Extensions go down below, in the 
 * ifndef _POSIX_SOURCE section.
 */

extern void _exit _ANSI_ARGS_((int status));
extern int access _ANSI_ARGS_((CONST char *path, int mode));
extern int chdir _ANSI_ARGS_((CONST char *path));
extern int chown _ANSI_ARGS_((CONST char *path, uid_t owner, gid_t group));
extern int close _ANSI_ARGS_((int fd));
extern int dup _ANSI_ARGS_((int oldfd));
extern int dup2 _ANSI_ARGS_((int oldfd, int newfd));
extern int execl _ANSI_ARGS_((CONST char *path, ...));
extern int execle _ANSI_ARGS_((CONST char *path, ...));
extern int execlp _ANSI_ARGS_((CONST char *file, ...));
extern int execv _ANSI_ARGS_((CONST char *path, char **argv));
extern int execve _ANSI_ARGS_((CONST char *path, char **argv, char **envp));
extern int execvp _ANSI_ARGS_((CONST char *file, char **argv));
extern pid_t fork _ANSI_ARGS_((void));
extern char *getcwd _ANSI_ARGS_((char *buf, size_t size));
extern gid_t getegid _ANSI_ARGS_((void));
extern uid_t geteuid _ANSI_ARGS_((void));
extern gid_t getgid _ANSI_ARGS_((void));
extern int getgroups _ANSI_ARGS_((int bufSize, int *buffer));
extern pid_t getpid _ANSI_ARGS_((void));
extern uid_t getuid _ANSI_ARGS_((void));
extern int isatty _ANSI_ARGS_((int fd));
extern long lseek _ANSI_ARGS_((int fd, long offset, int whence));
extern int pipe _ANSI_ARGS_((int *fildes));
extern int read _ANSI_ARGS_((int fd, char *buf, size_t size));
extern int setgid _ANSI_ARGS_((gid_t group));
extern int setuid _ANSI_ARGS_((uid_t user));
extern unsigned sleep _ANSI_ARGS_ ((unsigned seconds));
extern char *ttyname _ANSI_ARGS_((int fd));
extern int unlink _ANSI_ARGS_((CONST char *path));
extern int write _ANSI_ARGS_((int fd, CONST char *buf, size_t size));

#ifndef	_POSIX_SOURCE
extern char *crypt _ANSI_ARGS_((CONST char *, CONST char *));
extern int fchown _ANSI_ARGS_((int fd, uid_t owner, gid_t group));
extern int flock _ANSI_ARGS_((int fd, int operation));
extern int ftruncate _ANSI_ARGS_((int fd, unsigned long length));
extern int readlink _ANSI_ARGS_((CONST char *path, char *buf, int bufsize));
extern int setegid _ANSI_ARGS_((gid_t group));
extern int seteuid _ANSI_ARGS_((uid_t user));
extern int setreuid _ANSI_ARGS_((int ruid, int euid));
extern int symlink _ANSI_ARGS_((CONST char *, CONST char *));
extern int ttyslot _ANSI_ARGS_((void));
extern int truncate _ANSI_ARGS_((CONST char *path, unsigned long length));
extern int vfork _ANSI_ARGS_((void));
#endif /* _POSIX_SOURCE */

#endif /* _UNISTD */

