/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: tclUnixStr.c,v $
 * Revision 1.1  1996/09/27  20:01:41  wwang
 * Initial revision
 *
 * Revision 1.1  1996/09/27  19:59:14  wwang
 * Initial revision
 *
 * Revision 1.1.2.1  1994/07/12  21:19:37  mori_m
 * 	CR 11259:  First drop for Tcl I18N (messaging work).
 * 	[1994/07/12  21:14:57  mori_m]
 *
 * $EndLog$
 */
/* 
 * tclUnixStr.c --
 *
 *	This file contains procedures that generate strings
 *	corresponding to various UNIX-related codes, such
 *	as errno and signals.
 *
 * Copyright (c) 1991-1993 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#include "tclInt.h"
#include "tclUnix.h"
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dcetclmsg.h>

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ErrnoId --
 *
 *	Return a textual identifier for the current errno value.
 *
 * Results:
 *	This procedure returns a machine-readable textual identifier
 *	that corresponds to the current errno value (e.g. "EPERM").
 *	The identifier is the same as the #define name in errno.h.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_ErrnoId(res)
    int *res;			/* i18n execution result */
{
    switch (errno) {
#ifdef E2BIG
	case E2BIG: return "E2BIG";
#endif
#ifdef EACCES
	case EACCES: return "EACCES";
#endif
#ifdef EADDRINUSE
	case EADDRINUSE: return "EADDRINUSE";
#endif
#ifdef EADDRNOTAVAIL
	case EADDRNOTAVAIL: return "EADDRNOTAVAIL";
#endif
#ifdef EADV
	case EADV: return "EADV";
#endif
#ifdef EAFNOSUPPORT
	case EAFNOSUPPORT: return "EAFNOSUPPORT";
#endif
#ifdef EAGAIN
	case EAGAIN: return "EAGAIN";
#endif
#ifdef EALIGN
	case EALIGN: return "EALIGN";
#endif
#ifdef EALREADY
	case EALREADY: return "EALREADY";
#endif
#ifdef EBADE
	case EBADE: return "EBADE";
#endif
#ifdef EBADF
	case EBADF: return "EBADF";
#endif
#ifdef EBADFD
	case EBADFD: return "EBADFD";
#endif
#ifdef EBADMSG
	case EBADMSG: return "EBADMSG";
#endif
#ifdef EBADR
	case EBADR: return "EBADR";
#endif
#ifdef EBADRPC
	case EBADRPC: return "EBADRPC";
#endif
#ifdef EBADRQC
	case EBADRQC: return "EBADRQC";
#endif
#ifdef EBADSLT
	case EBADSLT: return "EBADSLT";
#endif
#ifdef EBFONT
	case EBFONT: return "EBFONT";
#endif
#ifdef EBUSY
	case EBUSY: return "EBUSY";
#endif
#ifdef ECHILD
	case ECHILD: return "ECHILD";
#endif
#ifdef ECHRNG
	case ECHRNG: return "ECHRNG";
#endif
#ifdef ECOMM
	case ECOMM: return "ECOMM";
#endif
#ifdef ECONNABORTED
	case ECONNABORTED: return "ECONNABORTED";
#endif
#ifdef ECONNREFUSED
	case ECONNREFUSED: return "ECONNREFUSED";
#endif
#ifdef ECONNRESET
	case ECONNRESET: return "ECONNRESET";
#endif
#if defined(EDEADLK) && (!defined(EWOULDBLOCK) || (EDEADLK != EWOULDBLOCK))
	case EDEADLK: return "EDEADLK";
#endif
#ifdef EDEADLOCK
	case EDEADLOCK: return "EDEADLOCK";
#endif
#ifdef EDESTADDRREQ
	case EDESTADDRREQ: return "EDESTADDRREQ";
#endif
#ifdef EDIRTY
	case EDIRTY: return "EDIRTY";
#endif
#ifdef EDOM
	case EDOM: return "EDOM";
#endif
#ifdef EDOTDOT
	case EDOTDOT: return "EDOTDOT";
#endif
#ifdef EDQUOT
	case EDQUOT: return "EDQUOT";
#endif
#ifdef EDUPPKG
	case EDUPPKG: return "EDUPPKG";
#endif
#ifdef EEXIST
	case EEXIST: return "EEXIST";
#endif
#ifdef EFAULT
	case EFAULT: return "EFAULT";
#endif
#ifdef EFBIG
	case EFBIG: return "EFBIG";
#endif
#ifdef EHOSTDOWN
	case EHOSTDOWN: return "EHOSTDOWN";
#endif
#ifdef EHOSTUNREACH
	case EHOSTUNREACH: return "EHOSTUNREACH";
#endif
#ifdef EIDRM
	case EIDRM: return "EIDRM";
#endif
#ifdef EINIT
	case EINIT: return "EINIT";
#endif
#ifdef EINPROGRESS
	case EINPROGRESS: return "EINPROGRESS";
#endif
#ifdef EINTR
	case EINTR: return "EINTR";
#endif
#ifdef EINVAL
	case EINVAL: return "EINVAL";
#endif
#ifdef EIO
	case EIO: return "EIO";
#endif
#ifdef EISCONN
	case EISCONN: return "EISCONN";
#endif
#ifdef EISDIR
	case EISDIR: return "EISDIR";
#endif
#ifdef EISNAME
	case EISNAM: return "EISNAM";
#endif
#ifdef ELBIN
	case ELBIN: return "ELBIN";
#endif
#ifdef EL2HLT
	case EL2HLT: return "EL2HLT";
#endif
#ifdef EL2NSYNC
	case EL2NSYNC: return "EL2NSYNC";
#endif
#ifdef EL3HLT
	case EL3HLT: return "EL3HLT";
#endif
#ifdef EL3RST
	case EL3RST: return "EL3RST";
#endif
#ifdef ELIBACC
	case ELIBACC: return "ELIBACC";
#endif
#ifdef ELIBBAD
	case ELIBBAD: return "ELIBBAD";
#endif
#ifdef ELIBEXEC
	case ELIBEXEC: return "ELIBEXEC";
#endif
#ifdef ELIBMAX
	case ELIBMAX: return "ELIBMAX";
#endif
#ifdef ELIBSCN
	case ELIBSCN: return "ELIBSCN";
#endif
#ifdef ELNRNG
	case ELNRNG: return "ELNRNG";
#endif
#ifdef ELOOP
	case ELOOP: return "ELOOP";
#endif
#ifdef EMFILE
	case EMFILE: return "EMFILE";
#endif
#ifdef EMLINK
	case EMLINK: return "EMLINK";
#endif
#ifdef EMSGSIZE
	case EMSGSIZE: return "EMSGSIZE";
#endif
#ifdef EMULTIHOP
	case EMULTIHOP: return "EMULTIHOP";
#endif
#ifdef ENAMETOOLONG
	case ENAMETOOLONG: return "ENAMETOOLONG";
#endif
#ifdef ENAVAIL
	case ENAVAIL: return "ENAVAIL";
#endif
#ifdef ENET
	case ENET: return "ENET";
#endif
#ifdef ENETDOWN
	case ENETDOWN: return "ENETDOWN";
#endif
#ifdef ENETRESET
	case ENETRESET: return "ENETRESET";
#endif
#ifdef ENETUNREACH
	case ENETUNREACH: return "ENETUNREACH";
#endif
#ifdef ENFILE
	case ENFILE: return "ENFILE";
#endif
#ifdef ENOANO
	case ENOANO: return "ENOANO";
#endif
#if defined(ENOBUFS) && (!defined(ENOSR) || (ENOBUFS != ENOSR))
	case ENOBUFS: return "ENOBUFS";
#endif
#ifdef ENOCSI
	case ENOCSI: return "ENOCSI";
#endif
#ifdef ENODATA
	case ENODATA: return "ENODATA";
#endif
#ifdef ENODEV
	case ENODEV: return "ENODEV";
#endif
#ifdef ENOENT
	case ENOENT: return "ENOENT";
#endif
#ifdef ENOEXEC
	case ENOEXEC: return "ENOEXEC";
#endif
#ifdef ENOLCK
	case ENOLCK: return "ENOLCK";
#endif
#ifdef ENOLINK
	case ENOLINK: return "ENOLINK";
#endif
#ifdef ENOMEM
	case ENOMEM: return "ENOMEM";
#endif
#ifdef ENOMSG
	case ENOMSG: return "ENOMSG";
#endif
#ifdef ENONET
	case ENONET: return "ENONET";
#endif
#ifdef ENOPKG
	case ENOPKG: return "ENOPKG";
#endif
#ifdef ENOPROTOOPT
	case ENOPROTOOPT: return "ENOPROTOOPT";
#endif
#ifdef ENOSPC
	case ENOSPC: return "ENOSPC";
#endif
#ifdef ENOSR
	case ENOSR: return "ENOSR";
#endif
#if defined(ENOSTR) && (!defined(ENOTTY) || (ENOTTY != ENOSTR))
	case ENOSTR: return "ENOSTR";
#endif
#ifdef ENOSYM
	case ENOSYM: return "ENOSYM";
#endif
#ifdef ENOSYS
	case ENOSYS: return "ENOSYS";
#endif
#ifdef ENOTBLK
	case ENOTBLK: return "ENOTBLK";
#endif
#ifdef ENOTCONN
	case ENOTCONN: return "ENOTCONN";
#endif
#ifdef ENOTDIR
	case ENOTDIR: return "ENOTDIR";
#endif
#if defined(ENOTEMPTY) && (!defined(EEXIST) || (ENOTEMPTY != EEXIST))
	case ENOTEMPTY: return "ENOTEMPTY";
#endif
#ifdef ENOTNAM
	case ENOTNAM: return "ENOTNAM";
#endif
#ifdef ENOTSOCK
	case ENOTSOCK: return "ENOTSOCK";
#endif
#ifdef ENOTTY
	case ENOTTY: return "ENOTTY";
#endif
#ifdef ENOTUNIQ
	case ENOTUNIQ: return "ENOTUNIQ";
#endif
#ifdef ENXIO
	case ENXIO: return "ENXIO";
#endif
#ifdef EOPNOTSUPP
	case EOPNOTSUPP: return "EOPNOTSUPP";
#endif
#ifdef EPERM
	case EPERM: return "EPERM";
#endif
#ifdef EPFNOSUPPORT
	case EPFNOSUPPORT: return "EPFNOSUPPORT";
#endif
#ifdef EPIPE
	case EPIPE: return "EPIPE";
#endif
#ifdef EPROCLIM
	case EPROCLIM: return "EPROCLIM";
#endif
#ifdef EPROCUNAVAIL
	case EPROCUNAVAIL: return "EPROCUNAVAIL";
#endif
#ifdef EPROGMISMATCH
	case EPROGMISMATCH: return "EPROGMISMATCH";
#endif
#ifdef EPROGUNAVAIL
	case EPROGUNAVAIL: return "EPROGUNAVAIL";
#endif
#ifdef EPROTO
	case EPROTO: return "EPROTO";
#endif
#ifdef EPROTONOSUPPORT
	case EPROTONOSUPPORT: return "EPROTONOSUPPORT";
#endif
#ifdef EPROTOTYPE
	case EPROTOTYPE: return "EPROTOTYPE";
#endif
#ifdef ERANGE
	case ERANGE: return "ERANGE";
#endif
#if defined(EREFUSED) && (!defined(ECONNREFUSED) || (EREFUSED != ECONNREFUSED))
	case EREFUSED: return "EREFUSED";
#endif
#ifdef EREMCHG
	case EREMCHG: return "EREMCHG";
#endif
#ifdef EREMDEV
	case EREMDEV: return "EREMDEV";
#endif
#ifdef EREMOTE
	case EREMOTE: return "EREMOTE";
#endif
#ifdef EREMOTEIO
	case EREMOTEIO: return "EREMOTEIO";
#endif
#ifdef EREMOTERELEASE
	case EREMOTERELEASE: return "EREMOTERELEASE";
#endif
#ifdef EROFS
	case EROFS: return "EROFS";
#endif
#ifdef ERPCMISMATCH
	case ERPCMISMATCH: return "ERPCMISMATCH";
#endif
#ifdef ERREMOTE
	case ERREMOTE: return "ERREMOTE";
#endif
#ifdef ESHUTDOWN
	case ESHUTDOWN: return "ESHUTDOWN";
#endif
#ifdef ESOCKTNOSUPPORT
	case ESOCKTNOSUPPORT: return "ESOCKTNOSUPPORT";
#endif
#ifdef ESPIPE
	case ESPIPE: return "ESPIPE";
#endif
#ifdef ESRCH
	case ESRCH: return "ESRCH";
#endif
#ifdef ESRMNT
	case ESRMNT: return "ESRMNT";
#endif
#ifdef ESTALE
	case ESTALE: return "ESTALE";
#endif
#ifdef ESUCCESS
	case ESUCCESS: return "ESUCCESS";
#endif
#ifdef ETIME
	case ETIME: return "ETIME";
#endif
#ifdef ETIMEDOUT
	case ETIMEDOUT: return "ETIMEDOUT";
#endif
#ifdef ETOOMANYREFS
	case ETOOMANYREFS: return "ETOOMANYREFS";
#endif
#ifdef ETXTBSY
	case ETXTBSY: return "ETXTBSY";
#endif
#ifdef EUCLEAN
	case EUCLEAN: return "EUCLEAN";
#endif
#ifdef EUNATCH
	case EUNATCH: return "EUNATCH";
#endif
#ifdef EUSERS
	case EUSERS: return "EUSERS";
#endif
#ifdef EVERSION
	case EVERSION: return "EVERSION";
#endif
#if defined(EWOULDBLOCK) && (!defined(EAGAIN) || (EWOULDBLOCK != EAGAIN))
	case EWOULDBLOCK: return "EWOULDBLOCK";
#endif
#ifdef EXDEV
	case EXDEV: return "EXDEV";
#endif
#ifdef EXFULL
	case EXFULL: return "EXFULL";
#endif
    }
    /* i18n
    return "unknown error";
    */
    *res = 1;		/* flag that return value needs to be freed later */
    return ((char *)dce_msg_get(tcl_s_unixstr_unknown_err));
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SignalId --
 *
 *	Return a textual identifier for a signal number.
 *
 * Results:
 *	This procedure returns a machine-readable textual identifier
 *	that corresponds to sig.  The identifier is the same as the
 *	#define name in signal.h.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_SignalId(sig, res)
    int sig;			/* Number of signal. */
    int *res;			/* i18n execution result */
{
    switch (sig) {
#ifdef SIGABRT
	case SIGABRT: return "SIGABRT";
#endif
#ifdef SIGALRM
	case SIGALRM: return "SIGALRM";
#endif
#ifdef SIGBUS
	case SIGBUS: return "SIGBUS";
#endif
#ifdef SIGCHLD
	case SIGCHLD: return "SIGCHLD";
#endif
#if defined(SIGCLD) && (!defined(SIGCHLD) || (SIGCLD != SIGCHLD))
	case SIGCLD: return "SIGCLD";
#endif
#ifdef SIGCONT
	case SIGCONT: return "SIGCONT";
#endif
#if defined(SIGEMT) && (!defined(SIGXCPU) || (SIGEMT != SIGXCPU))
	case SIGEMT: return "SIGEMT";
#endif
#ifdef SIGFPE
	case SIGFPE: return "SIGFPE";
#endif
#ifdef SIGHUP
	case SIGHUP: return "SIGHUP";
#endif
#ifdef SIGILL
	case SIGILL: return "SIGILL";
#endif
#ifdef SIGINT
	case SIGINT: return "SIGINT";
#endif
#ifdef SIGIO
	case SIGIO: return "SIGIO";
#endif
#if defined(SIGIOT) && (!defined(SIGABRT) || (SIGIOT != SIGABRT))
	case SIGIOT: return "SIGIOT";
#endif
#ifdef SIGKILL
	case SIGKILL: return "SIGKILL";
#endif
#if defined(SIGLOST) && (!defined(SIGIOT) || (SIGLOST != SIGIOT)) && (!defined(SIGURG) || (SIGLOST != SIGURG))
	case SIGLOST: return "SIGLOST";
#endif
#ifdef SIGPIPE
	case SIGPIPE: return "SIGPIPE";
#endif
#if defined(SIGPOLL) && (!defined(SIGIO) || (SIGPOLL != SIGIO))
	case SIGPOLL: return "SIGPOLL";
#endif
#ifdef SIGPROF
	case SIGPROF: return "SIGPROF";
#endif
#if defined(SIGPWR) && (!defined(SIGXFSZ) || (SIGPWR != SIGXFSZ))
	case SIGPWR: return "SIGPWR";
#endif
#ifdef SIGQUIT
	case SIGQUIT: return "SIGQUIT";
#endif
#ifdef SIGSEGV
	case SIGSEGV: return "SIGSEGV";
#endif
#ifdef SIGSTOP
	case SIGSTOP: return "SIGSTOP";
#endif
#ifdef SIGSYS
	case SIGSYS: return "SIGSYS";
#endif
#ifdef SIGTERM
	case SIGTERM: return "SIGTERM";
#endif
#ifdef SIGTRAP
	case SIGTRAP: return "SIGTRAP";
#endif
#ifdef SIGTSTP
	case SIGTSTP: return "SIGTSTP";
#endif
#ifdef SIGTTIN
	case SIGTTIN: return "SIGTTIN";
#endif
#ifdef SIGTTOU
	case SIGTTOU: return "SIGTTOU";
#endif
#if defined(SIGURG) && (!defined(SIGIO) || (SIGURG != SIGIO))
	case SIGURG: return "SIGURG";
#endif
#if defined(SIGUSR1) && (!defined(SIGIO) || (SIGUSR1 != SIGIO))
	case SIGUSR1: return "SIGUSR1";
#endif
#if defined(SIGUSR2) && (!defined(SIGURG) || (SIGUSR2 != SIGURG))
	case SIGUSR2: return "SIGUSR2";
#endif
#ifdef SIGVTALRM
	case SIGVTALRM: return "SIGVTALRM";
#endif
#ifdef SIGWINCH
	case SIGWINCH: return "SIGWINCH";
#endif
#ifdef SIGXCPU
	case SIGXCPU: return "SIGXCPU";
#endif
#ifdef SIGXFSZ
	case SIGXFSZ: return "SIGXFSZ";
#endif
    }
    /* i18n
    return "unknown signal";
     */
    *res = 1;		/* flag that return value needs to be freed later */
    return ((char *)dce_msg_get(tcl_s_unixstr_unknown_err));
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SignalMsg --
 *
 *	Return a human-readable message describing a signal.
 *
 * Results:
 *	This procedure returns a string describing sig that should
 *	make sense to a human.  It may not be easy for a machine
 *	to parse.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

char *
Tcl_SignalMsg(sig)
    int sig;			/* Number of signal. */
{
    switch (sig) {
#ifdef SIGABRT
	case SIGABRT: /* i18n	return "SIGABRT"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigabrt));
#endif
#ifdef SIGALRM
	case SIGALRM: /* i18n	return "alarm clock"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigalrm));
#endif
#ifdef SIGBUS
	case SIGBUS: /* i18n	return "bus error"; */
    		     return ((char *)dce_msg_get(tcl_s_unixstr_sigbus));
#endif
#ifdef SIGCHLD
	case SIGCHLD: /* i18n	return "child status changed"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigchld));
#endif
#if defined(SIGCLD) && (!defined(SIGCHLD) || (SIGCLD != SIGCHLD))
	case SIGCLD: /* i18n	return "child status changed"; */
    		     return ((char *)dce_msg_get(tcl_s_unixstr_sigchld));
#endif
#ifdef SIGCONT
	case SIGCONT: /* i18n	return "continue after stop"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigcont));
#endif
#if defined(SIGEMT) && (!defined(SIGXCPU) || (SIGEMT != SIGXCPU))
	case SIGEMT: /* i18n	return "EMT instruction"; */
    		     return ((char *)dce_msg_get(tcl_s_unixstr_sigemt));
#endif
#ifdef SIGFPE
	case SIGFPE: /* i18n	return "floating-point exception"; */
    		     return ((char *)dce_msg_get(tcl_s_unixstr_sigfpe));
#endif
#ifdef SIGHUP
	case SIGHUP: /* i18n	return "hangup"; */
    		     return ((char *)dce_msg_get(tcl_s_unixstr_sighup));
#endif
#ifdef SIGILL
	case SIGILL: /* i18n	return "illegal instruction"; */
    		     return ((char *)dce_msg_get(tcl_s_unixstr_sigill));
#endif
#ifdef SIGINT
	case SIGINT: /* i18n	return "interrupt"; */
    		     return ((char *)dce_msg_get(tcl_s_unixstr_sigint));
#endif
#ifdef SIGIO
	case SIGIO: /* i18n	return "input/output possible on file"; */
    		    return ((char *)dce_msg_get(tcl_s_unixstr_sigio));
#endif
#if defined(SIGIOT) && (!defined(SIGABRT) || (SIGABRT != SIGIOT))
	case SIGIOT: /* i18n	return "IOT instruction"; */
    		     return ((char *)dce_msg_get(tcl_s_unixstr_sigiot));
#endif
#ifdef SIGKILL
	case SIGKILL: /* i18n	return "kill signal"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigkill));
#endif
#if defined(SIGLOST) && (!defined(SIGIOT) || (SIGLOST != SIGIOT)) && (!defined(SIGURG) || (SIGLOST != SIGURG))
	case SIGLOST: /* i18n	return "resource lost"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_siglost));
#endif
#ifdef SIGPIPE
	case SIGPIPE: /* i18n	return "write on pipe with no readers"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigpipe));
#endif
#if defined(SIGPOLL) && (!defined(SIGIO) || (SIGPOLL != SIGIO))
	case SIGPOLL: /* i18n	return "input/output possible on file"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigpoll));
#endif
#ifdef SIGPROF
	case SIGPROF: /* i18n	return "profiling alarm"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigprof));
#endif
#if defined(SIGPWR) && (!defined(SIGXFSZ) || (SIGPWR != SIGXFSZ))
	case SIGPWR: /* i18n	return "power-fail restart"; */
    		     return ((char *)dce_msg_get(tcl_s_unixstr_sigpwr));
#endif
#ifdef SIGQUIT
	case SIGQUIT: /* i18n	return "quit signal"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigquit));
#endif
#ifdef SIGSEGV
	case SIGSEGV: /* i18n	return "segmentation violation"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigsegv));
#endif
#ifdef SIGSTOP
	case SIGSTOP: /* i18n	return "stop"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigstop));
#endif
#ifdef SIGSYS
	case SIGSYS: /* i18n	return "bad argument to system call"; */
    		     return ((char *)dce_msg_get(tcl_s_unixstr_sigsys));
#endif
#ifdef SIGTERM
	case SIGTERM: /* i18n	return "software termination signal"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigterm));
#endif
#ifdef SIGTRAP
	case SIGTRAP: /* i18n	return "trace trap"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigtrap));
#endif
#ifdef SIGTSTP
	case SIGTSTP: /* i18n	return "stop signal from tty"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigtstp));
#endif
#ifdef SIGTTIN
	case SIGTTIN: /* i18n	return "background tty read"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigttin));
#endif
#ifdef SIGTTOU
	case SIGTTOU: /* i18n	return "background tty write"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigttou));
#endif
#if defined(SIGURG) && (!defined(SIGIO) || (SIGURG != SIGIO))
	case SIGURG: /* i18n	return "urgent I/O condition"; */
    		     return ((char *)dce_msg_get(tcl_s_unixstr_sigurg));
#endif
#if defined(SIGUSR1) && (!defined(SIGIO) || (SIGUSR1 != SIGIO))
	case SIGUSR1: /* i18n	return "user-defined signal 1"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigusr1));
#endif
#if defined(SIGUSR2) && (!defined(SIGURG) || (SIGUSR2 != SIGURG))
	case SIGUSR2: /* i18n	return "user-defined signal 2"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigusr2));
#endif
#ifdef SIGVTALRM
	case SIGVTALRM: /* i18n	return "virtual time alarm"; */
    		        return ((char *)dce_msg_get(tcl_s_unixstr_sigvtalrm));
#endif
#ifdef SIGWINCH
	case SIGWINCH: /* i18n	return "window changed"; */
    		       return ((char *)dce_msg_get(tcl_s_unixstr_sigwinch));
#endif
#ifdef SIGXCPU
	case SIGXCPU: /* i18n	return "exceeded CPU time limit"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigxcpu));
#endif
#ifdef SIGXFSZ
	case SIGXFSZ: /* i18n	return "exceeded file size limit"; */
    		      return ((char *)dce_msg_get(tcl_s_unixstr_sigxfsz));
#endif
    }
    /* i18n	return "unknown signal"; */
    return ((char *)dce_msg_get(tcl_s_unixstr_unknown_signal));
}
