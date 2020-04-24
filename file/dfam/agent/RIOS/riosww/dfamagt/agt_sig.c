/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
static char *RCSID_agt_sig_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_sig.c,v 1.1.2.2 1996/03/09 20:44:14 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_sig.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:14  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:09  marty]
 *
 * Revision 1.1.2.1  1996/02/16  05:39:14  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:38:49  dce-sdc]
 * 
 * Revision 1.1.2.7  96/01/12  04:53:17  04:53:17  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:53:09  kishi]
 * 
 * Revision 1.1.2.6  1995/10/26  01:06:33  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:06:16  kishi]
 * 
 * Revision 1.1.2.5  1995/10/23  01:11:42  ichikawa
 * 	Before agt_exit or agt_abort,add AgentMsg.
 * 	[1995/10/23  01:11:29  ichikawa]
 * 
 * Revision 1.1.2.4  1995/10/13  09:13:41  kishi
 * 	Add header comment.
 * 	[1995/10/13  09:13:31  kishi]
 * 
 * Revision 1.1.2.3  1995/09/26  02:15:12  kishi
 * 	Include pthread.h for <cma_> jacket.
 * 	[1995/09/26  02:14:55  kishi]
 * 
 * Revision 1.1.2.2  1995/09/23  09:51:19  kishi
 * 	Add header comment.
 * 	[1995/09/23  09:22:32  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:50:12  kishi
 * 	Initial revision
 * 	[1995/09/09  06:42:43  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <errno.h>
#include <syslog.h>
#include <sys/wait.h>


#include "agt_agent.h"
#include "agt_errid.h"
#include "agt_sig.h"
#include "agt_msg.h"
#include "agt_lib.h"

struct d_sig_table sigtable[D_SIG_MAX_SIZE] = {
    {SIGHUP, agt_sig_ignore},
    {SIGINT, agt_sig_ignore},
    {SIGALRM, agt_sig_ignore},
    {SIGTERM, agt_sig_ignore},
    {SIGUSR1, agt_sig_ignore},
    {SIGUSR2, agt_sig_ignore},
    {SIGCHLD, agt_sig_ignore},
    {SIGPROF, agt_sig_ignore},
    {SIGIO, agt_sig_ignore},
    {SIGTSTP, agt_sig_ignore},
    {SIGTTIN, agt_sig_ignore},
    {SIGTTOU, agt_sig_ignore},
    {SIGURG, agt_sig_ignore},
    {SIGLOST, agt_sig_ignore},
    {0, NULL},
    {0, NULL},
    {0, NULL},
    {0, NULL},
    {0, NULL},
    {0, NULL},
};

static void agt_sig_USR1(pthread_addr_t arg);
static void agt_sig_SIGNALS(pthread_addr_t arg);
static int agt__sig_signo_to_entryno(int sig);


/**************************************************************************/
/* ID	= agt_sig_action						  */
/* name = Register signal function in signal table			  */
/* func = Register signal function func in signal table			  */
/* i/o =								  */
/*	sig   :int   :IN:signal number					  */
/*	func():void *:IN:function called during signal transmission	  */
/* return = none							  */
/* note = If a signal not defined in DCE is specified, abort		  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_sig_action(int sig, void (*func)())
{
    int i;

    switch (sig) {
    case SIGHUP:
    case SIGINT:
    case SIGALRM:
    case SIGTERM:
    case SIGUSR1:
    case SIGUSR2:
    case SIGCHLD:
    case SIGPROF:
    case SIGIO:
    case SIGTSTP:
    case SIGTTIN:
    case SIGTTOU:
    case SIGURG:
	for (i = 0; i < D_SIG_MAX_SIZE; i++) {
	    if ((sigtable[i].sig == sig) ||
		(sigtable[i].sig == 0)) {
		break;
	    }
	}
	if (i < D_SIG_MAX_SIZE) {
	    sigtable[i].func = func;
	} else {
	    sprintf(msg.wkbuf, "%i", D_EID_20031);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
	break;

    case SIGILL:
    case SIGABRT:
    case SIGEMT:
    case SIGFPE:
    case SIGBUS:
    case SIGSEGV:
    case SIGSYS:
    case SIGPIPE:

    case SIGQUIT:
    case SIGTRAP:
    case SIGVTALRM:
    case SIGSTOP:
    case SIGCONT:
    case SIGPWR:
    case SIGWINCH:
    default:
	sprintf(msg.wkbuf, "%i", D_EID_20032);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
	break;
    }
}

/**************************************************************************/
/* ID	= agt_sig_init_signal_threads					  */
/* name = Create thread for SIGUSR1 and other signals			  */
/* func = Create thread for SIGUSR1. Then also create threads for ohter   */
/*        signals							  */
/* io = none								  */
/* return = none							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_sig_init_signal_threads(void)
{
    int rtn;

    rtn = pthread_create(&thread_info[D_THREAD_USR1].handle,
			 pthread_attr_default,
			 (pthread_startroutine_t)agt_sig_USR1,
			 (pthread_addr_t)D_THREAD_USR1);
    if (rtn == -1) {
	sprintf(msg.wkbuf, "%i,%i,%i", D_EID_20033, errno, D_THREAD_USR1);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	agt_exit(D_EXIT_UNKNOWN);
    }

    rtn = pthread_create(&thread_info[D_THREAD_SIGNALS].handle,
			 pthread_attr_default,
			 (pthread_startroutine_t)agt_sig_SIGNALS,
			 (pthread_addr_t)D_THREAD_SIGNALS);
    if (rtn == -1) {
	sprintf(msg.wkbuf, "%i,%i,%i", D_EID_20034, errno, D_THREAD_SIGNALS);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	agt_exit(D_EXIT_UNKNOWN);
    }
}

/**************************************************************************/
/* ID	= agt_sig_USR1							  */
/* name = Program executed during SIGUSR1 transmission			  */
/* func = During tranmission of SIGUSR1 signal, execute funtions          */
/*        registered in SIGUSR1 signal table				  */
/* i/o = 								  */
/*	arg:pthread_addr_t:IN:address of thread data			  */
/* return = none							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt_sig_USR1(pthread_addr_t arg)
{
    int rtn;
    int id;
    int sig_no;
    int ent_no;
    sigset_t waiting;

    id = (int)arg;
    rtn = pthread_setspecific(thr_id,
			      (pthread_addr_t *)&thread_info[id].thread_id);
    if (rtn == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_20035, errno);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }
    thread_info[id].thread_id = id;

    while (1) {
	(void)sigemptyset(&waiting);
	(void)sigaddset(&waiting, SIGUSR1);

#ifdef _DEBUG
fprintf(stderr, "%c%c%c%c: sigwait(SIGUSR1)\n", agt.proctype[0], agt.proctype[1], agt.proctype[2], agt.proctype[3]);
#endif

	errno = 0;
	sig_no = sigwait(&waiting);

#ifdef _DEBUG
fprintf(stderr, "%c%c%c%c: SIGNAL:SIGUSR1\n", agt.proctype[0], agt.proctype[1], agt.proctype[2], agt.proctype[3]);
#endif
	ent_no = agt__sig_signo_to_entryno(sig_no);
	if (ent_no != -1) {
	    switch (sig_no) {
	    case SIGUSR1:
		(*sigtable[ent_no].func)(sig_no, 0, NULL);
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i,%i", D_EID_20036, sig_no, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
	    }
	}
    }
}


/**************************************************************************/
/* ID   = agt_sig_SIGNALS						  */
/* name = Program executed during signals except SIGUSR1		  */
/* func = This program is executed when any other signal then SIGUSR1 is  */
/*        transmitted. Execute functions registered in the signal table   */
/* i/o =								  */
/*	arg:pthread_addr_t:IN:address of thread data			  */
/* return = none							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt_sig_SIGNALS(pthread_addr_t arg)
{
    int id;
    int saved_errno;
    int sig_no;
    int ent_no;
    sigset_t waiting;
    int rtn;

    id = (int)arg;
    rtn = pthread_setspecific(thr_id,
			      (pthread_addr_t *)&thread_info[id].thread_id);
    if (rtn == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_20037, errno);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }
    thread_info[id].thread_id = id;

    rtn = pthread_mutex_lock(&sig_mutex);
    if (rtn == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_20038, errno);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }

    while (1) {
	(void)sigemptyset(&waiting);
	(void)sigaddset(&waiting, SIGHUP);
	(void)sigaddset(&waiting, SIGINT);
	(void)sigaddset(&waiting, SIGALRM);
	(void)sigaddset(&waiting, SIGTERM);
	(void)sigaddset(&waiting, SIGUSR2);
	(void)sigaddset(&waiting, SIGCHLD);

#ifdef _DEBUG
sprintf(msg.buf, "%c%c%c%c: sigwait(SIGNALS)\n", agt.proctype[0], agt.proctype[1], agt.proctype[2], agt.proctype[3]);
AgentMsg(LOG_DEBUG, msg.buf);
#endif

	rtn = pthread_mutex_unlock(&sig_mutex);
	if (rtn == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_20039, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}

	errno = 0;
	sig_no = sigwait(&waiting);
	saved_errno = errno;

	rtn = pthread_mutex_lock(&sig_mutex);
	if (rtn == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_20040, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}

#ifdef _DEBUG
sprintf(msg.buf, "%c%c%c%c: SIGNAL:%i\n", agt.proctype[0], agt.proctype[1], agt.proctype[2], agt.proctype[3], sig_no);
AgentMsg(LOG_DEBUG, msg.buf);
#endif

	errno = saved_errno;
	ent_no = agt__sig_signo_to_entryno(sig_no);
	if (ent_no != -1) {
	    switch (sig_no) {
	    case SIGHUP:
	    case SIGINT:
	    case SIGALRM:
	    case SIGTERM:
	    case SIGUSR2:
	    case SIGCHLD:
	    case SIGPROF:
	    case SIGIO:
	    case SIGTSTP:
	    case SIGTTIN:
	    case SIGTTOU:
	    case SIGURG:
	    case SIGLOST:
		(*sigtable[ent_no].func)(sig_no, 0, NULL);
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i,%i", D_EID_20041, sig_no, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
	    }
	}
    }
}


/**************************************************************************/
/* ID	= agt_sig_ignore						  */
/* name = Routine executed during signal transmission			  */
/* func = Routine executed during signal transmission. When transmitting  */
/*        SIGCHLD signal, get all killed child processes. Nothing is done */
/*        when transmitting any other signal				  */
/* i/o =								  */
/*	sig :int		:IN:signal number			  */
/*	code:int		:IN:code number				  */
/*	scp :struct sigcontext *:IN:signal information			  */
/* return = none							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_sig_ignore(int sig, int code, struct sigcontext *scp)
{
    pid_t dpid;
    int dstatus;

    switch (sig) {
    case SIGCHLD:
	while (1) {
	    dpid = waitpid(-1, &dstatus, WNOHANG);
	    switch (dpid) {
	    case 0:
		return;
	    case -1:
		switch (errno) {
		case EINTR:
		    continue;
		case ECHILD:
		    return;
		default:
		    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_20042, sig, errno);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_abort();
		}
		break;
	    default:
		break;
	    }
	}
	break;
    default:

	break;
    }
}


/*****************************************************************************/
/* ID	= agt_sig_ok_exit						     */
/*                                                                           */
/* name = Calling "agt_bare_exit function"                                   */	
/* func = When the SIGHUP or SIGTERM signal is received, this function is    */
/*        invoked to call the agt_bare_exit function.                        */
/*                                                                           */
/* i/o    = sig :int                :INP: signal number                      */
/*          code:int                :INP: status code                        */
/*	    scp :struct sigcontext *:INP: signal information                 */
/*                                                                           */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
void agt_sig_ok_exit(int sig, int code, struct sigcontext *scp)
{

    agt_bare_exit(D_EXIT_OK);

}

/**************************************************************************/
/* ID	= agt__sig_signo_to_entryno					  */
/* name = Search signal table						  */
/* func = Search signal table and return entry number correspondent to    */
/*        signal specified in sig					  */
/* i/o =								  */
/*	sig:int:IN:signal number					  */
/* return =								  */
/*	entry number in the signal table corresponding to sig		  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__sig_signo_to_entryno(int sig)
{
    int i;

    for (i = 0; i < D_SIG_MAX_SIZE; i++) {
	if (sigtable[i].sig == sig) {
	    break;
	}
    }

    return (i < D_SIG_MAX_SIZE) ? i : -1;
}

