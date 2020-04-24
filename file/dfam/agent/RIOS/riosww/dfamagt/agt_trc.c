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
static char *RCSID_agt_trc_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_trc.c,v 1.1.2.2 1996/03/09 20:44:24 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_trc.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:24  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:15  marty]
 *
 * Revision 1.1.2.1  1996/02/16  05:27:21  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:26:46  dce-sdc]
 * 
 * Revision 1.1.2.5  96/01/12  05:03:39  05:03:39  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:03:30  kishi]
 * 
 * Revision 1.1.2.4  1995/10/26  01:24:24  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:24:16  kishi]
 * 
 * Revision 1.1.2.3  1995/10/13  09:16:39  kishi
 * 	Add header comment.
 * 	[1995/10/13  09:16:26  kishi]
 * 
 * Revision 1.1.2.2  1995/09/29  00:04:26  kishi
 * 	Add header comment.
 * 	[1995/09/29  00:04:06  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:47:59  kishi
 * 	Initial revision
 * 	[1995/09/09  06:45:33  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#ifdef _DEBUG
  #include <stdio.h>
  #include <unistd.h>
  #include "agt_main.h"
#endif



#include "agt_agent.h"
#include "agt_errid.h"
#include "agt_trc.h"
#include "agt_msg.h"
#include "agt_ipc.h"
#include "agt_sck.h"
#include "agt_sem.h"
#include "agt_parm.h"
#include "agt_sig.h"


struct d_trc trc = {
    {'T', 'R', 'C', '*'},
    D_FALSE,
    D_FALSE,
    D_FALSE,

    (D_DFLT_MEMTRSIZE*D_UNIT_KB),
    D_DFLT_EVTTRFILES,
    (D_DFLT_EVTTRSIZE*D_UNIT_KB),
    (D_DFLT_PRCTRSIZE*D_UNIT_KB),

    NULL,
    0,

    0,
    NULL,
    0,

    {'\0'},
    0,

    D_FALSE,
    {'\0'}

  };

static int trc_fs_full = D_FALSE;

static void agt__trc_init_signal(void);
static void agt__trc_close_fd(void);
static void agt__trc_open_uxsock(void);
static void agt__trc_open_trace(void);
static int agt__trc_which_file(int num);
static int agt__trc_next_file(int num);
static void agt__trc_send_ready_to_main(void);
static int agt__trc_process_uxmsg(void);
static int agt__trc_write_trace(char *rbuf, int rlen,
				struct sockaddr_un *from);
static int agt__trc_stop_trace(char *rbuf, int rlen,
			       struct sockaddr_un *from);
static void agt__trc_truncate_open(int num);
static void agt__trc_sigusr1(int sig, int code, struct sigcontext *scp);



/**************************************************************************/
/* ID	= Agenttrace							  */
/* name = Event trace output			                          */
/* func = Output event trace specified in data. Also trace common memory  */
/*        if memory trace has been specified in the configuration file    */
/* io =									  */
/*	id  :char *:IN:event ID						  */
/*	len :int   :IN:length of data					  */
/*	data:char *:IN:data to be traced out				  */
/*	info:int   :IN:additional information				  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void Agenttrace(char *id, int len, char *data, int info)
{
    int totallen;
    int total32;
    int uxmsglen;
    long wlong;
    short wshort;
    struct timeval tv;
    int *current_thread_id;
    int rtn;
    int cplen;


    if (!((trc.memtr_on && (agt.shmid_t != -1)) || trc.evttr_on) ||
	trc.secondcall) {


	return;

    }
    trc.secondcall = D_TRUE;


    rtn = pthread_getspecific(thr_id, (pthread_addr_t *)&current_thread_id);
    if (rtn == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_23104, errno);
	sprintf(trc.errmsgbuf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	syslog(LOG_WARNING, trc.errmsgbuf);

	trc.secondcall = D_FALSE;
	return;
    }


    if (trc.tmsgbufsize == 0) {
	trc.tmsgb = (char *)malloc(D_TRC_LEN_MAX+D_TRC_MSG_OFFSET+D_TRC_UNIT);

	if (trc.tmsgb == NULL) {
	    sprintf(msg.wkbuf, "%i", D_EID_23001);
	    sprintf(trc.errmsgbuf, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
	    syslog(LOG_WARNING, trc.errmsgbuf);
	    sprintf(trc.errmsgbuf, catgets(agt.catd, NL_SETD, MSG02101, KDDS02101), agt.ownpid);
	    syslog(LOG_WARNING, trc.errmsgbuf);
	    trc.tmsgbufsize = -1;

	    trc.secondcall = D_FALSE;
	    return;
	} else {
	    trc.tmsgbufsize = D_TRC_LEN_MAX;
	}
    }

    totallen = len + D_TRC_UNIT;

    total32 = ((totallen + D_TRC_UNIT - 1)/D_TRC_UNIT) * D_TRC_UNIT;

    memcpy(&trc.tmsgb[D_TRC_MSG_OFFSET+0], D_TRCID_EVTRC, 2);
    memcpy(&trc.tmsgb[D_TRC_MSG_OFFSET+2], id, 6);
    if (gettimeofday(&tv, (struct timezone *)NULL) == -1) {

	tv.tv_usec = -1;
    }
    wlong = htonl((long)tv.tv_sec);
    memcpy(&trc.tmsgb[D_TRC_MSG_OFFSET+8], (char *)&wlong, 4);
    wlong = htonl(tv.tv_usec);
    memcpy(&trc.tmsgb[D_TRC_MSG_OFFSET+12], (char *)&wlong, 4);
    memset(&trc.tmsgb[D_TRC_MSG_OFFSET+12],
	   (*current_thread_id) & D_THREAD_ID_MASK, 1);
    wlong = htonl(agt.ownpid);
    memcpy(&trc.tmsgb[D_TRC_MSG_OFFSET+16], (char *)&wlong, 4);
    trc.tmsgb[D_TRC_MSG_OFFSET+20] = agt.proctype[0];
    trc.tmsgb[D_TRC_MSG_OFFSET+21] = agt.proctype[3];
    wshort = htons(len);
    memcpy(&trc.tmsgb[D_TRC_MSG_OFFSET+22], (char *)&wshort, 2);
    wlong = htonl(sck.gwipaddr.s_addr);
    memcpy(&trc.tmsgb[D_TRC_MSG_OFFSET+24], (char *)&wlong, 4);
    wlong = htonl(info);
    memcpy(&trc.tmsgb[D_TRC_MSG_OFFSET+28], (char *)&wlong, 4);
    if (D_TRC_LEN_MAX > len)
	cplen = len;
    else
	cplen = D_TRC_LEN_MAX;
    memcpy(&trc.tmsgb[D_TRC_MSG_OFFSET+32], data, cplen);


    if ((trc.memtr_on) && (agt.shmid_t  != -1)) {



	agt_sem_set_lock(agt.semid, D_SEM_TRC);

	if ((agt.main->memtr_size - agt.main->memtr_p) < total32) {



	    if (agt.main->memtr_size < total32) {

		sprintf(msg.wkbuf, "%i,%i,%i", D_EID_23002, total32, agt.main->memtr_size);
		sprintf(trc.errmsgbuf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
		AgentMsg(LOG_ERR, trc.errmsgbuf);

		agt_sem_release_lock(agt.semid, D_SEM_TRC);
		goto memtrace_done;
	    }

	    if ((agt.main->memtr_size - agt.main->memtr_p) >= 32) {
		memcpy(&agt.main->memtr_buf[agt.main->memtr_p],
		       D_TRCID_INVAL, strlen(D_TRCID_INVAL));
	    }

	    agt.main->memtr_p = 0;
	}

	if (D_TRC_LEN_MAX > totallen)
		cplen = totallen;
	else
		cplen = D_TRC_LEN_MAX;
	memcpy(&agt.main->memtr_buf[agt.main->memtr_p],
	       &trc.tmsgb[D_TRC_MSG_OFFSET], cplen);
	agt.main->memtr_p += total32;
	agt_sem_release_lock(agt.semid, D_SEM_TRC);
    }

memtrace_done:

    if (trc.evttr_on) {
	wlong = htonl(D_AG_TRACE);
	memcpy(&trc.tmsgb[0], (char *)&wlong, 4);
	wlong = htonl(total32+4);
	memcpy(&trc.tmsgb[4], (char *)&wlong, 4);
	wlong = htonl(total32);
	memcpy(&trc.tmsgb[8], (char *)&wlong, 4);

	uxmsglen = total32 + D_TRC_MSG_OFFSET;
	if (D_TRC_LEN_MAX > uxmsglen)
		cplen = uxmsglen;
	else
		cplen = D_TRC_LEN_MAX;
	if (sendto(sck.ux_fd, trc.tmsgb, cplen, 0,
		   &sck.trace_adr, sck.trace_adrlen) != uxmsglen) {

	    switch (errno) {
	    case ECONNREFUSED:
	    case EBADF:
	    case ENOBUFS:
#ifdef _DEBUG
		syslog(LOG_DEBUG, "%c%c%c%c: sendto failed in Agenttrace() with errno:%i\n", agt.proctype[0], agt.proctype[1], agt.proctype[2], agt.proctype[3], errno);
#endif
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i,%i", D_EID_23003, errno, uxmsglen);
		sprintf(trc.errmsgbuf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
		AgentTrace(TRACEDEB,"DEBUG ", strlen(trc.errmsgbuf), trc.errmsgbuf, 0);
		break;
	    }
	}
    }

    trc.secondcall = D_FALSE;

}

/**************************************************************************/
/* ID	= agt_trace							  */
/* name = Trace proccess routine					  */
/* func = Perform the following tasks:					  */
/*        1. initialize trace process signal				  */
/*        2. close unnecessary files					  */
/*        3. open UNIX domain socket for trace transmission  		  */
/*        4. open event trace file					  */
/*        5. output trace information					  */
/* io = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_trace(void)
{
    int readyfdnum = 0;
    int nfds;
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    int continue_loop = D_TRUE;
    int saved_errno;
    int rtn;



    trc.evttr_on = D_FALSE;

    agt.status = D_TRC_ST_INIT;

    agt__trc_init_signal();
    agt__trc_close_fd();
    agt__trc_open_uxsock();
    agt__trc_open_trace();
    agt.status = D_TRC_ST_INIT_D;
    agt__trc_send_ready_to_main();
    agt.status = D_TRC_ST_RUN;

    nfds = sck.ux_fd + 1;

    while (continue_loop) {

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);
	FD_SET(sck.ux_fd, &readfds);

	rtn = pthread_mutex_unlock(&sig_mutex);
	if (rtn == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_23106, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}

	errno = 0;
	readyfdnum = select(nfds, (int *)&readfds,
		      (int *)&writefds, (int *)&exceptfds, NULL);

	saved_errno = errno;
	rtn = pthread_mutex_lock(&sig_mutex);
	if (rtn == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_23108, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}

	errno =saved_errno;
	switch(readyfdnum) {
	case 0:
	    sprintf(msg.wkbuf, "%i", D_EID_23101);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02030, KDDS02030), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	case -1:
	    switch (errno) {
	    case EINTR:
		continue;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_23102, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02030, KDDS02030), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	    break;
	default:
	    continue_loop = agt__trc_process_uxmsg();
	    break;
	}
    }

    agt_exit(D_EXIT_OK);
}


/**************************************************************************/
/* ID	= agt__trc_init_signal						  */
/* name = Initialize trace process signal				  */
/* func = Initialize trace process signal				  */
/* io = none                                                              */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__trc_init_signal(void)
{

    agt_sig_action(SIGHUP, agt_sig_ok_exit);
    agt_sig_action(SIGALRM, agt_sig_ignore);
    agt_sig_action(SIGTERM, agt_sig_ignore);
    agt_sig_action(SIGCHLD, agt_sig_ignore);
    agt_sig_action(SIGUSR1, agt__trc_sigusr1);

}


/**************************************************************************/
/* ID	= agt__trc_close_fd						  */
/* name = Close opened files definition child process 			  */
/* func = Close opened files definition child process that will not be    */
/*        used by UNIX domain during trace process			  */
/* io = none                                                              */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__trc_close_fd(void)
{
    close(sck.ux_fd);
    sck.ux_fd = -1;
}


/**************************************************************************/
/* ID	= agt__trc_open_uxsock						  */
/* name = Open UNIX domain socket for trace transmition			  */
/* func = Open UNIX domain socket for trace transmition			  */
/* io = none                                                              */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__trc_open_uxsock(void)
{
    sck.ux_fd = agt_sck_open_unixsocket(sck.trace_adr.sun_path);
}


/**************************************************************************/
/* ID	= agt__trc_open_trace					     	  */	
/* name = Open event trace file						  */
/* func = Open event trace file and change access mode			  */
/* io = none                                                              */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__trc_open_trace(void)
{
    struct stat stbuf;


    agt.main->evtr_curfil = agt__trc_which_file(1);
    sprintf(trc.evfilename, "%s/%s%s%1i\0", D_TRC_FILE_PATH, agt.agent_id,
	    D_TRC_FILE_BASE, agt.main->evtr_curfil);

open_retry:
    if ((trc.tracefd = open(trc.evfilename, O_RDWR|O_APPEND|O_CREAT)) == -1) {
	switch (errno) {
	case EINTR:
	    goto open_retry;
	case ENFILE:
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02102, KDDS02102), strerror(errno));
	    AgentMsg(LOG_ERR, msg.buf);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02100, KDDS02100));
	    AgentMsg(LOG_WARNING, msg.buf);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_23109, errno, trc.evfilename);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02030, KDDS02030), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }

    if (chown(trc.evfilename, (uid_t)0, (gid_t)0) == -1) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_23127, errno, trc.evfilename);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02030, KDDS02030), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }
    if (chmod(trc.evfilename, (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) == -1) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_23128, errno, trc.evfilename);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02030, KDDS02030), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }


    stat(trc.evfilename, &stbuf);
    agt.main->evtr_p = stbuf.st_size;

    return;
}


/**************************************************************************/
/* ID	= agt__trc_which_file						  */
/* name = Search trace file number for output				  */
/* func = Check the last when trace file was modificated and get the trace*/
/*        file number for output					  */
/* io =									  */
/*	num:int:IN:trace file number					  */
/* return =								  */
/*	newest:trace number of the file for output			  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__trc_which_file(int num)
{
    int done = D_FALSE;
    int newest;
    time_t newest_time;
    struct stat stbuf;


    newest = num;
    newest_time = 0;
    while (! done) {
	sprintf(trc.evfilename, "%s/%s%s%1i\0", D_TRC_FILE_PATH, agt.agent_id,
		D_TRC_FILE_BASE, num);

	if (stat(trc.evfilename, &stbuf) == -1) {
	    switch (errno) {
	    case ENOENT:
		done = D_TRUE;
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i,%s", D_EID_23110, errno, trc.evfilename);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02030, KDDS02030), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	} else {
	    if (newest_time <= stbuf.st_mtime) {
		newest_time = stbuf.st_mtime;
		newest = num;
	    }

	    if (num == agt.main->evtr_files) {
		done = D_TRUE;
	    } else {
		num++;
	    }
	}
    }

    return newest;
}


/**************************************************************************/
/* ID	= agt__trc_next_file						  */
/* name = Seach next trace file number					  */
/* func = Return the next trace file number according to num		  */
/* io =									  */
/*      num:int:IN:trace file number	                                  */
/* return =								  */
/*	num:trace file number						  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__trc_next_file(int num)
{
    return (num >= agt.main->evtr_files) ? 1 : num + 1;
}


/**************************************************************************/
/* ID	= agt__trc_send_ready_to_main					  */
/* name = Transmit information about trace process initialization	  */
/* func = Advise main process that initialization process has been        */
/*        completed							  */
/* io = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__trc_send_ready_to_main(void)
{
#define READY_MSG_SIZE 8
    char mbuf[8];

    long_to_char(&mbuf[0], D_AG_TRDY);
    long_to_char(&mbuf[4], 0);


	if (sendto(sck.ux_fd, mbuf, READY_MSG_SIZE, 0,
	       &sck.main_adr, sck.main_adrlen) != READY_MSG_SIZE) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_23111, errno);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02030, KDDS02030), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }

}


/**************************************************************************/
/* ID 	= agt__trc_process_uxmsg					  */
/* name = Trace process output and stop					  */	
/* func = Analyze information send to trace process then output or stop   */
/*        the trace process according to that information		  */
/* io = none                                                              */
/* return =                                                               */
/*	D_TRUE:normal end						  */
/*  D_FALSE:abnormal end						  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__trc_process_uxmsg(void)
{
    int msgtype;
    int returncode;

recv_retry:
    memset((char *)&sck.from, '\0', sizeof(sck.from));
    sck.fromlen = sizeof(sck.from);
    if ((sck.uxrcvlen = recvfrom(sck.ux_fd, sck.uxrcvdata, sck.uxrcvbufsize,
				 0, &sck.from, &sck.fromlen)) == -1) {
	switch (errno) {
	case EINTR:
	    goto recv_retry;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_23112, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02030, KDDS02030), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }

    sck.from.sun_path[sck.fromlen-sizeof(sck.from.sun_family)] = '\0';

    if (sck.uxrcvlen < 8) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_23113, sck.uxrcvlen, sck.from.sun_path);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	return D_TRUE;
    }

    switch (msgtype = char_to_num(&sck.uxrcvdata[0], 4)) {
    case D_AG_TRACE:
	returncode = agt__trc_write_trace(sck.uxrcvdata,
					  sck.uxrcvlen, &sck.from);
	break;
    case D_AG_STPTRC:
	returncode = agt__trc_stop_trace(sck.uxrcvdata,
					 sck.uxrcvlen, &sck.from);
	break;
    default:
	sprintf(msg.wkbuf, "%i,%i,%i,%s", D_EID_23114, msgtype, sck.uxrcvlen, sck.from.sun_path);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	return D_TRUE;
    }

    return returncode;
}


/**************************************************************************/
/* ID 	= agt__trc_write_trace						  */
/* name = Output of event trace information				  */
/* func = Output rbuf in event trace information file. If file is full,   */
/*        output in the next trace file    				  */
/* io =									  */
/*  rbuf:char *	             :IN:trace information 			  */
/*  rlen:int		     :IN:length of rbuf			          */
/*  from:struct sockaddr_un *:IN:address of process who requested output  */
/* return =								  */
/*	D_TRUE :normal end						  */
/*  D_FALSE:abnormal end						  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__trc_write_trace(char *rbuf, int rlen, struct sockaddr_un *from)
{
    int extlen;
    int tracelen;
    int datalen;
    int wlen;

    char *bufp;


    if (trc.tracefd == -1) {

	return D_TRUE;

    }

    extlen = char_to_num(&rbuf[4], 4);
    if (extlen < 4) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_23115, extlen, from->sun_path);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);

	return D_TRUE;
    }

    tracelen = char_to_num(&rbuf[8], 4);
    if ((tracelen == 0) || ((tracelen % D_TRC_UNIT) != 0)){
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_23116, tracelen, from->sun_path);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);

	return D_TRUE;
    }

    if ((agt.main->evtr_size - agt.main->evtr_p) < tracelen) {

	char *inv = D_TRC_EVE_INVALID_32;

	if ((agt.main->evtr_size - agt.main->evtr_p) < 32) {
	    goto skip_padding_on_write;
	}

	datalen = D_TRC_UNIT;
	while (datalen > 0) {
	    wlen = write(trc.tracefd, inv, datalen);
	    if (wlen == -1) {
		switch (errno) {
		case EINTR:
		    continue;
		case EIO:
		    trc_fs_full = D_FALSE;
		    sprintf(msg.wkbuf, "%i,%s", D_EID_23117, trc.evfilename);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02103, KDDS02103), "write", strerror(errno), msg.wkbuf);
		    AgentMsg(LOG_WARNING, msg.buf);
		    return D_TRUE;
		case ENOSPC:
		case EFBIG:
		    if (!trc_fs_full) {
			trc_fs_full = D_TRUE;
			sprintf(msg.wkbuf, "%i,%s", D_EID_23118, trc.evfilename);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02103, KDDS02103), "write", strerror(errno), msg.wkbuf);
			AgentMsg(LOG_WARNING, msg.buf);
		    }
		    return D_TRUE;
		default:
		    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_23119, errno, trc.evfilename);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02030, KDDS02030), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_abort();
		    break;
		}
	    }
	    trc_fs_full = D_FALSE;
	    datalen -= wlen;
	    inv += wlen;
	}

skip_padding_on_write:

	if (close(trc.tracefd) == -1) {
	    switch (errno) {
	    case ETIMEDOUT:
		sprintf(msg.wkbuf, "%i,%i", D_EID_23120, trc.tracefd);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02103, KDDS02103), "close", strerror(errno), msg.wkbuf);
		AgentMsg(LOG_WARNING, msg.buf);
		agt_exit(D_EXIT_OUTOFRSC);
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_23121, trc.tracefd);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02103, KDDS02103), "close", strerror(errno), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	}

	agt.main->evtr_curfil = agt__trc_next_file(agt.main->evtr_curfil);

	agt__trc_truncate_open(agt.main->evtr_curfil);
	agt.main->evtr_p = 0;
    }

    datalen = tracelen;
    bufp = &rbuf[12];

    while (datalen > 0) {
	wlen = write(trc.tracefd, bufp, datalen);
	if (wlen == -1) {
	    switch (errno) {
	    case EINTR:
		continue;
	    case EIO:
		trc_fs_full = D_FALSE;
		sprintf(msg.wkbuf, "%i,%s", D_EID_23122, trc.evfilename);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02103, KDDS02103), "write", strerror(errno), msg.wkbuf);
		AgentMsg(LOG_WARNING, msg.buf);
		return D_TRUE;
	    case ENOSPC:
	    case EFBIG:
		if (!trc_fs_full) {
		    trc_fs_full = D_TRUE;
		    sprintf(msg.wkbuf, "%i,%s", D_EID_23123, trc.evfilename);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02103, KDDS02103), "write", strerror(errno), msg.wkbuf);
		    AgentMsg(LOG_WARNING, msg.buf);
		}
		return D_TRUE;
	    default:
		sprintf(msg.wkbuf, "%i,%s", D_EID_23124, trc.evfilename);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02103, KDDS02103), "write", strerror(errno), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	}
	trc_fs_full = D_FALSE;
	datalen -= wlen;
	bufp += wlen;
	agt.main->evtr_p += wlen;
    }

    return D_TRUE;
}


/*****************************************************************************/
/* ID	= agt__trc_stop_trace						     */
/*                                                                           */
/* name = Outputting the information of the process (that requested to       */
/*        terminate the trace) to the trace                                  */
/* func = Outputs the information of the process (that asked to terminate    */
/*        the trace) to the trace and syslog file.                           */
/*                                                                           */
/* i/o  = rbuf: char *              :INP: information of the termination     */
/*                                        request                            */
/*        rlen: int                 :INP: size of rbuf                       */
/*        from: struct sockaddr_un *:INP: process information that requested */
/*                                        the termination                    */
/* return = D_FALSE: end                                                     */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static int agt__trc_stop_trace(char *rbuf, int rlen, struct sockaddr_un *from)
{
    int extlen;

    extlen = char_to_num(&rbuf[4], 4);
    if (extlen != 0) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_23102, extlen, from->sun_path);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
    }

    return D_FALSE;
}


/**************************************************************************/
/* ID	= agt__trc_truncate_open					  */
/* name = Open event trace file						  */
/* func = Open event trace file specified by num number and change file   */
/*        access  mode      						  */
/* io =									  */
/*	num:int:IN:number of event trace file number			  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__trc_truncate_open(int num)
{
    sprintf(trc.evfilename, "%s/%s%s%1i\0", D_TRC_FILE_PATH, agt.agent_id,
	    D_TRC_FILE_BASE, num);

open_retry:
    if ((trc.tracefd = open(trc.evfilename, O_RDWR|O_TRUNC|O_CREAT)) == -1) {
	switch (errno) {
	case EINTR:
	    goto open_retry;
	case ENFILE:
	    sprintf(msg.wkbuf, "%i,%s", D_EID_23125, trc.evfilename);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02103, KDDS02103), "open", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_WARNING, msg.buf);
	    agt_abort();
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_23126, num, trc.evfilename);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02103, KDDS02103), "open", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
    }

    if (chown(trc.evfilename, (uid_t)0, (gid_t)0) == -1) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_23129, errno, trc.evfilename);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02030, KDDS02030), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }
    if (chmod(trc.evfilename, (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) == -1) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_23130, errno, trc.evfilename);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02030, KDDS02030), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }


    return;
}

/**************************************************************************/
/* ID	= agt__trc_sigusr1						  */
/* name = Routine to be executed when transmitting sigusr1 signal    	  */
/* func = Routine to be executed when transmitting sigusr1 signal.        */
/*        Perform exit process						  */
/* io = none								  */
/* return = none							  */	
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__trc_sigusr1(int sig, int code, struct sigcontext *scp)
{

    agt_exit(D_EXIT_OK);

}
