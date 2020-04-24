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
 * $Log: read_pwd.c,v $
 * Revision 1.1.2.1  1996/06/05  21:19:26  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:09:32  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/os/read_pwd.c
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * libos: krb5_read_password for BSD 4.3
 */

#include "k5-int.h"
#ifndef _MSDOS
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>

#ifdef sun
#include <sgtty.h>
#endif

extern int errno;

#ifdef ECHO_PASSWORD
#define cleanup(errcode) (void) signal(SIGINT, ointrfunc); return errcode;
#else

/* POSIX_* are auto-magically defined in <krb5/config.h> at source
   configuration time. */

#ifdef POSIX_TERMIOS
#include <termios.h>
#else
#include <sys/ioctl.h>
#endif /* POSIX_TERMIOS */

#ifdef POSIX_TERMIOS
#define cleanup(errcode) (void) signal(SIGINT, ointrfunc); tcsetattr(fd, TCSANOW, &save_control); return errcode;
#else
#ifdef sun
#define cleanup(errcode) (void) signal(SIGINT, ointrfunc); stty(fd, (char *)&tty_savestate); return errcode;
#else /* !sun */
#define cleanup(errcode) (void) signal(SIGINT, ointrfunc); ioctl(fd, TIOCSETP, (char *)&tty_savestate); return errcode;
#endif /* sun */
#endif /* POSIX_TERMIOS */

#endif /* ECHO_PASSWORD */

static jmp_buf pwd_jump;


static krb5_sigtype
intr_routine()
{
    longjmp(pwd_jump, 1);
    /*NOTREACHED*/
}

krb5_error_code
krb5_read_password(context, prompt, prompt2, return_pwd, size_return)
    krb5_context context;
    char *prompt;
    char *prompt2;
    char *return_pwd;
    int *size_return;
{
    /* adapted from Kerberos v4 des/read_password.c */
#if defined(__STDC__) || defined(mips) || defined(_WINDOWS)
    /* readin_string is used after a longjmp, so must be volatile */
    volatile
#endif
      char *readin_string = 0;
    register char *ptr;
    int scratchchar;
    krb5_sigtype (*ointrfunc)();
#ifndef ECHO_PASSWORD
#ifdef POSIX_TERMIOS
    struct termios echo_control, save_control;
    int fd;

    /* get the file descriptor associated with stdin */
    fd=fileno(stdin);

#ifdef notdef
    /* don't want to read password from anything but a terminal */
    if (!isatty(fd)) {
        fprintf(stderr,"Can only read password from a tty\n"); /* XXX */
        errno=ENOTTY; /* say innapropriate ioctl for device */
	return errno;
    }
#endif /* notdef */

    if (tcgetattr(fd, &echo_control) == -1)
	return errno;

    save_control = echo_control;
    echo_control.c_lflag &= ~(ECHO|ECHONL);
    
    if (tcsetattr(fd, TCSANOW, &echo_control) == -1)
	return errno;
#else
    /* 4.3BSD style */
    struct sgttyb tty_state, tty_savestate;
    int fd;

    /* get the file descriptor associated with stdin */
    fd=fileno(stdin);

#ifdef notdef
    /* don't want to read password from anything but a terminal */
    if (!isatty(fd)) {
        fprintf(stderr,"Can only read password from a tty\n"); /* XXX */
        errno=ENOTTY; /* say innapropriate ioctl for device */
	return errno;
    }
#endif /* notdef */

    /* save terminal state */
    if (
#ifdef sun
	gtty(fd,(char *)&tty_savestate)
#else
	ioctl(fd,TIOCGETP,(char *)&tty_savestate)
#endif
	== -1) 
	return errno;

    tty_state = tty_savestate;

    tty_state.sg_flags &= ~ECHO;
    if (
#ifdef sun
	stty(fd,(char *)&tty_state)
#else
	ioctl(fd,TIOCSETP,(char *)&tty_state)
#endif
	== -1)
	return errno;
#endif

#endif /* ECHO_PASSWORD */

    if (setjmp(pwd_jump)) {
	/* interrupted */
	if (readin_string) {
	    (void) memset((char *)readin_string, 0, *size_return);
	    krb5_xfree(readin_string);
	}
	(void) memset(return_pwd, 0, *size_return);
	cleanup(KRB5_LIBOS_PWDINTR);
    }
    /* save intrfunc */
    ointrfunc = signal(SIGINT, intr_routine);

    /* put out the prompt */
    (void) fputs(prompt,stdout);
    (void) fflush(stdout);
    (void) memset(return_pwd, 0, *size_return);

    if (fgets(return_pwd, *size_return, stdin) == NULL) {
	/* error */
	(void) putchar('\n');
	(void) memset(return_pwd, 0, *size_return);
	cleanup(KRB5_LIBOS_CANTREADPWD);
    }
    (void) putchar('\n');
    /* fgets always null-terminates the returned string */

    /* replace newline with null */
    if (ptr = strchr(return_pwd, '\n'))
	*ptr = '\0';
    else /* flush rest of input line */
	do {
	    scratchchar = getchar();
	} while (scratchchar != EOF && scratchchar != '\n');

    if (prompt2) {
	/* put out the prompt */
	(void) fputs(prompt2,stdout);
	(void) fflush(stdout);
	readin_string = malloc(*size_return);
	if (!readin_string) {
	    (void) memset(return_pwd, 0, *size_return);
	    cleanup(ENOMEM);
	}
	(void) memset((char *)readin_string, 0, *size_return);
	if (fgets((char *)readin_string, *size_return, stdin) == NULL) {
	    /* error */
	    (void) putchar('\n');
	    (void) memset((char *)readin_string, 0, *size_return);
	    (void) memset(return_pwd, 0, *size_return);
	    krb5_xfree(readin_string);
	    cleanup(KRB5_LIBOS_CANTREADPWD);
	}
	(void) putchar('\n');

	if (ptr = strchr((char *)readin_string, '\n'))
	    *ptr = '\0';
        else /* need to flush */
	    do {
		scratchchar = getchar();
	    } while (scratchchar != EOF && scratchchar != '\n');
	    
	/* compare */
	if (strncmp(return_pwd, (char *)readin_string, *size_return)) {
	    (void) memset((char *)readin_string, 0, *size_return);
	    (void) memset(return_pwd, 0, *size_return);
	    krb5_xfree(readin_string);
	    cleanup(KRB5_LIBOS_BADPWDMATCH);
	}
	(void) memset((char *)readin_string, 0, *size_return);
	krb5_xfree(readin_string);
    }
    
    /* reset intrfunc */
    (void) signal(SIGINT, ointrfunc);

#ifndef ECHO_PASSWORD
#ifdef POSIX_TERMIOS
    if (tcsetattr(fd, TCSANOW, &save_control) == -1)
	return errno;
#else
    if (
#ifdef sun
	stty(fd, (char *)&tty_savestate)
#else
	ioctl(fd, TIOCSETP, (char *)&tty_savestate)
#endif
	== -1)
	return errno;
#endif
#endif /* ECHO_PASSWORD */
    *size_return = strlen(return_pwd);

    return 0;
}
#else /* MSDOS */
/* Don't expect to be called, just define it for sanity and the linker.
*/

krb5_error_code
krb5_read_password(context, prompt, prompt2, return_pwd, size_return)
    krb5_context context;
    char *prompt;
    char *prompt2;
    char *return_pwd;
    int *size_return;
{
   *size_return = 0;
   return 0;
}
#endif   /* !MSDOS */
