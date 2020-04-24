/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: read_pwd.c,v $
 * Revision 1.1.9.1  1996/10/03  15:06:46  arvind
 * 	Correct compiler warnings.
 * 	[1996/09/16  21:32 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.7.2  1996/02/18  00:15:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:02  marty]
 * 
 * Revision 1.1.7.1  1995/12/08  17:43:53  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/22  15:23 UTC  cuti
 * 	CHFts08653 fix.
 * 
 * 	HP revision /main/cuti_mothra_fix/1  1995/06/20  20:10 UTC  cuti
 * 	CHFts08653: change signal() and longjmp signal handler to TRY/CATCH clause
 * 	[1995/12/08  17:18:39  root]
 * 
 * Revision 1.1.3.3  1992/12/29  16:31:10  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:18:44  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:43:01  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:35:22  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:50:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  read_pwd.c V=5 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/read_pwd.c,v $
 * $Author: arvind $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
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
 * Please note:  Use of this routine, you need to have a signal handler
 * before calling it and inside signal handler, it need to cancel itself,
 * such as using pthread_signal_to_cancel.np() in DCE environment. 
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_read_pwd_c[] =
"$Id: read_pwd.c,v 1.1.9.1 1996/10/03 15:06:46 arvind Exp $";
#endif	/* lint */

#include <pthread.h>

#include <krb5/krb5.h>

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>

#include <krb5/ext-proto.h>

/* POSIX_* are auto-magically defined in <krb5/config.h> at source
   configuration time. */

#ifdef POSIX_TERMIOS
#include <termios.h>
#else
#include <sys/ioctl.h>
#endif /* POSIX_TERMIOS */

extern int errno;

#ifdef POSIX_TERMIOS
# if defined(SNI_SVR4_POSIX)
#define cleanup(errcode) tcsetattr(0, TCSANOW, &save_control); return errcode;
# else
#define cleanup(errcode) tcsetattr(0, TCSANOW, &save_control); return errcode;
# endif
#else
# if defined(SNI_SVR4_POSIX)
#define cleanup(errcode) ioctl(0, TIOCSETP, (char *)&tty_savestate); return errcode;
# else
#define cleanup(errcode) ioctl(0, TIOCSETP, (char *)&tty_savestate); return errcode;
# endif
#endif

krb5_error_code
krb5_read_password(prompt, prompt2, return_pwd, size_return)
char *prompt;
char *prompt2;
char *return_pwd;
int *size_return;
{

    /* adapted from Kerberos v4 des/read_password.c */
    char * volatile readin_string = 0;
    register char *ptr;
    int scratchchar;
    krb5_sigtype (*ointrfunc)();


#ifdef POSIX_TERMIOS
    struct termios echo_control, save_control;

    if (tcgetattr(0, &echo_control) == -1)
	return errno;

    save_control = echo_control;
    echo_control.c_lflag &= ~(ECHO|ECHONL);
    
    if (tcsetattr(0, TCSANOW, &echo_control) == -1)
	return errno;
#else
    /* 4.3BSD style */
    struct sgttyb tty_state, tty_savestate;

    /* save terminal state */
    if (ioctl(0,TIOCGETP,(char *)&tty_savestate) == -1) 
	return errno;

    tty_state = tty_savestate;

    tty_state.sg_flags &= ~ECHO;
    if (ioctl(0,TIOCSETP,(char *)&tty_state) == -1)
	return errno;
#endif

/*  Remove longjump signal handler which deosn't work with thread */

TRY {
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
	    xfree(readin_string);
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
	    xfree(readin_string);
	    cleanup(KRB5_LIBOS_BADPWDMATCH);
	}
	(void) memset((char *)readin_string, 0, *size_return);
	xfree(readin_string);
    }

} 
CATCH_ALL {	/* interrupted */
	if (readin_string) {
	    (void) memset((char *)readin_string, 0, *size_return);
	    xfree(readin_string);
	}
	(void) memset(return_pwd, 0, *size_return);
	cleanup(KRB5_LIBOS_PWDINTR);
    
}

ENDTRY


    
#ifdef POSIX_TERMIOS
    if (tcsetattr(0, TCSANOW, &save_control) == -1)
	return errno;
#else
    if (ioctl(0, TIOCSETP, (char *)&tty_savestate) == -1)
	return errno;
#endif
    *size_return = strlen(return_pwd);

    return 0;
}
