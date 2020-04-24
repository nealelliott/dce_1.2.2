/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: lclcalls.c,v $
 * Revision 1.1.4.2  1996/02/17  23:35:03  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:14  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:12:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:03  root]
 * 
 * Revision 1.1.2.2  1992/12/30  16:36:40  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:19:01  htf]
 * 
 * Revision 1.1  1992/01/19  15:35:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <signal.h>
#include <sys/errno.h>
#include <utctime.h>

/*
 * Main dts gettime syscall entry point.
 */
int utc_gettime(struct utc *utc)
{
    int retcode;

    extern int dts_syscall_installed();
    static first_call_done = 0;
    static syscall_exists = 0;

    /*
     * Check to see if the system call is installed.
     * If so callit, otherwise return error
     */

    if (syscall_exists) {
        retcode = kutc_gettime(utc);
    } else if (!first_call_done) {
	first_call_done++;
	if (syscall_exists = dts_syscall_installed()) {
            retcode = kutc_gettime(utc);
	} else {
	    retcode = user_utc_gettime(utc);
	}
    } else {
	    retcode = user_utc_gettime(utc);
    }
	

    return retcode;
}

int utc_adjtime(enum adjmode modeflag, union adjunion *argblk)
{
    int retcode;

    extern int dts_syscall_installed();
    static first_call_done = 0;
    static syscall_exists = 0;

    /*
     * Check to see if the system call is installed.
     * If so callit, otherwise return error
     */

    if (syscall_exists) {
        retcode = kutc_adjtime(modeflag, argblk);
    } else if (!first_call_done) {
	first_call_done++;
	if (syscall_exists = dts_syscall_installed()) {
            retcode = kutc_adjtime(modeflag, argblk);
	} else {
	    retcode = user_utc_adjtime(modeflag, argblk);
	}
    } else {
	retcode = user_utc_adjtime(modeflag, argblk);
    }


    return retcode;
}
