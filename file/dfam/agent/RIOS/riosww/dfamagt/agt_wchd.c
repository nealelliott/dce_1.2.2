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
static char *RCSID_agt_wchd_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_wchd.c,v 1.1.2.2 1996/03/09 20:44:27 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_wchd.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:27  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:17  marty]
 *
 * Revision 1.1.2.1  1996/02/16  05:13:12  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:12:38  dce-sdc]
 * 
 * Revision 1.1.2.5  96/01/12  05:07:57  05:07:57  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:07:45  kishi]
 * 
 * Revision 1.1.2.4  1995/10/26  01:32:06  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:31:58  kishi]
 * 
 * Revision 1.1.2.3  1995/10/23  02:04:52  ichikawa
 * 	Before agt_abort,add AgentMsg.
 * 	[1995/10/23  02:04:41  ichikawa]
 * 
 * Revision 1.1.2.2  1995/09/29  00:29:36  kishi
 * 	Add header comment.
 * 	[1995/09/29  00:28:23  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:47:19  kishi
 * 	Initial revision
 * 	[1995/09/09  06:46:17  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <errno.h>
#include <syslog.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/socketvar.h>
#include <sys/types.h>
#ifdef _DEBUG
  #include <stdio.h>
#endif


#include "agt_agent.h"
#include "agt_errid.h"
#include "agt_wchd.h"
#include "agt_parm.h"
#include "agt_ipc.h"
#include "agt_sck.h"
#include "agt_msg.h"


struct d_wchd wchd = {
    {'W', 'C', 'H', 'D'},

    NULL,
    NULL,

    D_DFLT_HCINTERVAL,
    D_DFLT_HCTIMEOUT
};


static void agt__wchd_init_watchdog(void);
static void agt__wchd_set_hc_itimer(void);
static int agt__wchd_check_signal(void);
static void agt__wchd_send_hc_msg(struct agt_timer *tp);
static int agt__wchd_process_uxmsg(void);
static int agt__wchd_process_inmsg(void);
static void agt__wchd_send_msg(char *buf, int len);
static void agt__wchd_recv_hc_msg(char *rbuf, int rlen);
static void agt__wchd_process_hc_waittimeout(struct agt_timer *tp);


/**************************************************************************/
/* ID	= agt_wchd							  */
/* name = Wacthdog process main routine					  */
/* func = Wacthdog process main routine. Perform the following tasks:     */
/*        1. call agt__wchd_init_watchdog	                          */
/*        2. request to watchdog process is put in wait state using select*/
/*           system call. If request is received through UNIX internal    */
/*           protocol, call agt__wchd_process_uxmsg. If request had came  */
/*           from ARPA protocol, call agt__wchd_process_inmsg 		  */
/* io = none								  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_wchd(void)
{
    int readyfdnum;
    int nfds;
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    int continue_loop = D_TRUE;
    int savederrno;
    int rtn;

    agt__wchd_init_watchdog();

    nfds = ((sck.ux_fd > sck.gw_fd) ? sck.ux_fd : sck.gw_fd) + 1;

    agt__wchd_set_hc_itimer();

    while (continue_loop) {

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);
	FD_SET(sck.ux_fd, &readfds);
	FD_SET(sck.gw_fd, &readfds);

	rtn = pthread_mutex_unlock(&sig_mutex);
	if (rtn == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_24026, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02033, KDDS02033), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}

	errno = 0;
	readyfdnum = select(nfds, (int *)&readfds,
		      (int *)&writefds, (int *)&exceptfds, NULL);
	savederrno = errno;

	rtn = pthread_mutex_lock(&sig_mutex);
	if (rtn == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_24027, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02033, KDDS02033), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}

	errno = savederrno;
	switch (readyfdnum) {
	case 0:
	    sprintf(msg.wkbuf, "%i", D_EID_24001);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02033, KDDS02033), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	case -1:
	    switch (errno) {
	    case EINTR:
		continue;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_24002, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02033, KDDS02033), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	    break;
	default:
	    if (FD_ISSET(sck.ux_fd, &readfds)) {
		continue_loop = agt__wchd_process_uxmsg();
	    } else if (FD_ISSET(sck.gw_fd, &readfds)) {
		continue_loop = agt__wchd_process_inmsg();
	    } else {
		sprintf(msg.wkbuf, "%i", D_EID_24003);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02033, KDDS02033), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
	    }
	    break;
	}
    }

    agt_exit(D_EXIT_OK);
}


/**************************************************************************/
/* ID	= agt__wchd_init_watchdog					  */
/*                                                                        */
/* name = Timer initialization 					          */
/* func = Initializes the timer to used by the Watch dog process.         */
/*                                                                        */
/* i/o  = none                                                            */
/* return = none                                                          */
/* note = none                                                            */
/* date =  April 22, 1995 by Tetsuya Ichikawa                             */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__wchd_init_watchdog(void)
{

    AgentInitTimer();

    if (wchd.hcinterval > 0) {
	wchd.timerhcint = (struct agt_timer *)malloc(sizeof(struct agt_timer));
	if (wchd.timerhcint == NULL) {
	    switch (errno) {
	    case ENOMEM:
		sprintf(msg.wkbuf, "%i,%i", D_EID_24004, sizeof(struct agt_timer));
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
		AgentMsg(LOG_WARNING, msg.buf);
		agt_exit(D_EXIT_OUTOFRSC);
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i,%i", D_EID_24005, errno, sizeof(struct agt_timer));
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02033, KDDS02033), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	}
	wchd.timerhcint->seconds = 0;
	wchd.timerhcint->info = NULL;
	wchd.timerhcint->timer_func = agt__wchd_send_hc_msg;

	wchd.timerhcwait =
	  (struct agt_timer *)malloc(sizeof(struct agt_timer));
	if (wchd.timerhcwait == NULL) {
	    switch (errno) {
	    case ENOMEM:
		sprintf(msg.wkbuf, "%i,%i", D_EID_24006, sizeof(struct agt_timer));
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
		AgentMsg(LOG_WARNING, msg.buf);
		agt_exit(D_EXIT_OUTOFRSC);
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i,%i", D_EID_24007, errno, sizeof(struct agt_timer));
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02033, KDDS02033), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	}
	wchd.timerhcwait->seconds = 0;
	wchd.timerhcwait->info = NULL;
	wchd.timerhcwait->timer_func = agt__wchd_process_hc_waittimeout;
    }
}


/**************************************************************************/
/* ID	= agt__wchd_set_hc_itimer					  */
/*                                                                        */
/* name = Initialization of the sanity check timer                        */
/* func = Sets the timer for the sanity check that is done to the Gateway */
/*        connected to Agent.                                             */
/*                                                                        */
/* i/o  = none                                                            */
/* return = none                                                          */
/* note = none                                                            */
/* date =  April 22, 1995 by Tetsuya Ichikawa                             */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__wchd_set_hc_itimer(void)
{
    if (wchd.hcinterval) {
	wchd.timerhcint->seconds = wchd.hcinterval;
	AgentSetTimer(wchd.timerhcint);
    }
}


/**************************************************************************/
/* ID	= agt__wchd_send_hc_msg						  */
/*                                                                        */
/* name = Sending a message to Gateway for the sanity check               */
/* func = Sends a message to Gateway to make sure that Gateway is up and  */
/*        running.                                                        */
/*                                                                        */
/* i/o  = tp: struct agt_timer *:INP: currently not in use                */
/* return = none				                          */
/* note = none                                                            */
/* date =  April 22, 1995 by Tetsuya Ichikawa                             */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__wchd_send_hc_msg(struct agt_timer *tp)
{
#define HC_MSG_SIZE 8
    char mbuf[HC_MSG_SIZE];
    char *mp;
    int mlen;

    int sendlen;
    int sent;

    short_to_char(&mbuf[0], (short)D_GW_HCHK);
    short_to_char(&mbuf[2], (short)0);
    long_to_char(&mbuf[4], 0);
    mlen = HC_MSG_SIZE;

    for (sendlen = mlen, mp = mbuf; sendlen > 0;
	 sendlen -= sent, mp += sent) {

send_retry:
	sent = send(sck.gw_fd, mp, sendlen, 0);

	if (sent == -1) {
	    switch (errno) {
	    case EINTR:
		goto send_retry;
	    case ENOBUFS:
		sprintf(msg.wkbuf, "%i", D_EID_24010);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02564, KDDS02564), sck.gwname, "send", strerror(errno), msg.wkbuf);
		AgentMsg(LOG_WARNING, msg.buf);
		agt_exit(D_EXIT_OUTOFRSC);
		break;
	    case EPIPE:
	    case ENETDOWN:
	    case ECONNRESET:
	    case ECONNABORTED:
	    case ECONNREFUSED:
		sprintf(msg.wkbuf, "%i", D_EID_24010);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02562, KDDS02562), sck.gwname, strerror(errno), msg.wkbuf);
		AgentMsg(LOG_WARNING, msg.buf);
		agt_exit(D_EXIT_NETWORK);
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_24010, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02033, KDDS02033), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	}
    }

    AgentTrace(TRACEDEB,"SDDATA", mlen, mbuf, mlen);

    wchd.timerhcwait->seconds = wchd.hctimeout;
    AgentSetTimer(wchd.timerhcwait);
}



/**************************************************************************/
/* ID	= agt__wchd_process_uxmsg					  */
/*                                                                        */
/* name = Sending a message to Gateway                                    */
/* func = Sends Gateway the information designated  at -msg option of the */
/*        dfaagt command.                                                 */
/*                                                                        */
/* i/o  = none								  */	
/* return = D_TRUE: normal end					          */
/* note = none                                                            */
/* date =  April 22, 1995 by Tetsuya Ichikawa                             */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__wchd_process_uxmsg(void)
{
    int msgtype;

recv_retry:
    memset((char *)&sck.from, '\0', sizeof(sck.from));
    sck.fromlen = sizeof(sck.from);
    if ((sck.uxrcvlen = recvfrom(sck.ux_fd, sck.uxrcvdata,
		 sck.uxrcvbufsize, 0, &sck.from, &sck.fromlen)) == -1) {
	switch (errno) {
	case EINTR:
	    goto recv_retry;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_24011, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02033, KDDS02033), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }

    sck.from.sun_path[sck.fromlen-sizeof(sck.from.sun_family)] = '\0';

    AgentTrace(TRACEERR,"RVUNIX", sck.uxrcvlen, sck.uxrcvdata, sck.uxrcvlen);

    if (sck.uxrcvlen < 8) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_24012, sck.uxrcvlen, sck.from.sun_path);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);

	return D_TRUE;
    }

    switch (msgtype = char_to_num(&sck.uxrcvdata[0], 4)) {
    case D_AG_WMSG:
	agt__wchd_send_msg(sck.uxrcvdata, sck.uxrcvlen);
	break;
    default:
	sprintf(msg.wkbuf, "%i,%i,%i,%s", D_EID_24013, msgtype, sck.uxrcvlen, sck.from.sun_path);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);

	break;
    }

    return D_TRUE;
}


/**************************************************************************/
/* ID	= agt__wchd_process_inmsg					  */
/*                                                                        */
/* name = Receiving a reply for the sanity checking                       */
/* func = Receives a reply (as a response to the sanity checking) from    */
/*        Gateway.                                                        */
/*                                                                        */
/* i/o  = none								  */
/* return = D_TRUE: normal end	                                          */
/* note = none                                                            */
/* date =  April 22, 1995 by Tetsuya Ichikawa                             */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__wchd_process_inmsg(void)
{
#define INET_MSG_LEN 8
    char rbuf[INET_MSG_LEN];
    int rlen;

    int msgtype;

recv_retry:

    if ((rlen = recv(sck.gw_fd, &rbuf, INET_MSG_LEN, 0)) == -1) {

	switch (errno) {
	case EINTR:
	    goto recv_retry;
	case ECONNREFUSED:
	    sprintf(msg.wkbuf, "%i", D_EID_24016);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02562, KDDS02562), sck.gwname, strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_WARNING, msg.buf);
	    agt_exit(D_EXIT_NETWORK);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_24016, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02033, KDDS02033), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }


    AgentTrace(TRACEDEB,"RVDATA", rlen, rbuf, rlen);

    if (rlen == 0) {
	return D_FALSE;
    } else if (rlen == 8) {
	switch (msgtype = char_to_num(&rbuf[0], 2)) {
	case D_GW_HCHKR:
	    agt__wchd_recv_hc_msg(rbuf, rlen);
	    break;
	default:
	    AgentTrace(TRACEERR,"RVDATA", rlen, rbuf, rlen);
	    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_24017, msgtype, rlen);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    break;
	}
    } else {
	sprintf(msg.wkbuf, "%i,%i", D_EID_24018, rlen);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
    }

    return D_TRUE;
}



/**************************************************************************/
/* ID	= agt__wchd_send_msg						  */
/*                                                                        */
/* name = Sending a message to Gateway 					  */
/* func = Sends Gateway the information pointed by "buf."                 */
/*                                                                        */
/* i/o  = buf:char *:INP: area for the information to be sent to Gateway  */
/*        len:int   :INP: size of the above buffer 			  */
/* return = none							  */	
/* note = none                                                            */
/* date =  April 22, 1995 by Tetsuya Ichikawa                             */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__wchd_send_msg(char *buf, int len)
{
    char *sp;
    int extlen;
    int msgcode;

    int sendlen;
    int sent;


    if ((extlen = char_to_num(&buf[4], 4)) < 4) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_24019, extlen);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	return;
    }

    msgcode = char_to_num(&buf[8], 4);
    if ((msgcode < D_GW_MSG_STOP) || (D_GW_MSG_CLIE < msgcode)) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_24020, msgcode);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	return;
    }

    short_to_char(&buf[0], (short)D_GW_MSG);
    short_to_char(&buf[2], (short)0);

    for (sendlen = len, sp = buf; sendlen > 0; sendlen -= sent, sp += sent) {

send_retry:
	sent = send(sck.gw_fd, sp, sendlen, 0);

	if (sent == -1) {
	    switch (errno) {
	    case EINTR:
		goto send_retry;
	    case ENOBUFS:
		sprintf(msg.wkbuf, "%i", D_EID_24023);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02564, KDDS02564), sck.gwname, "send", strerror(errno), msg.wkbuf);
		AgentMsg(LOG_WARNING, msg.buf);
		agt_exit(D_EXIT_OUTOFRSC);
		break;
	    case EPIPE:
	    case ENETDOWN:
	    case ECONNRESET:
	    case ECONNABORTED:
	    case ECONNREFUSED:
		sprintf(msg.wkbuf, "%i", D_EID_24023);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02562, KDDS02562), sck.gwname, strerror(errno), msg.wkbuf);
		AgentMsg(LOG_WARNING, msg.buf);
		agt_exit(D_EXIT_NETWORK);
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_24023, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02033, KDDS02033), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	}
    }


    AgentTrace(TRACEERR,"SDDATA", len, buf, len);
}




/**************************************************************************/
/* ID	= agt__wchd_recv_hc_msg						  */
/*                                                                        */
/* name = Setting the timer again                                         */
/* func = Resets the timer for the Watchdog process, and sets the timer   */
/*        again.                                                          */
/*                                                                        */
/* i/o  = rbuf:char *:OUT: area for the data received from Gateway        */
/*        rlen:int   :OUT: size of "rbuf"				  */
/*                                                                        */
/* return = none                                                          */    
/* note = none                                                            */
/* date =  April 22, 1995 by Tetsuya Ichikawa                             */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__wchd_recv_hc_msg(char *rbuf, int rlen)
{
    int extlen;

    extlen = char_to_num(&rbuf[4], 4);
    if (extlen != 0) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_24024, extlen);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);

	return;
    }


    AgentResetTimer(wchd.timerhcwait);


    agt__wchd_set_hc_itimer();
}


/**************************************************************************/
/* ID	= agt__wchd_process_hc_waittimeout				  */
/*                                                                        */
/* name = Timeout monitoring routine for sanity checking                  */
/* func = When the Gateway's response to the sanity check message does    */
/*        not come in within the specified period for the sanity check,   */
/*        this routine is invoked to report the system communication      */
/*        process that Gateway is down.                                   */
/*                                                                        */
/* i/o  = tp:struct agt_timer *:INP: currently not in use                 */
/* return = none                                                          */
/* note = none                                                            */
/* date =  April 22, 1995 by Tetsuya Ichikawa                             */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__wchd_process_hc_waittimeout(struct agt_timer *tp)
{
#define GWDOWN_MSG_SIZE 12
    long_to_char(&sck.uxsnddata[0], D_AG_GWDOWN);
    long_to_char(&sck.uxsnddata[4], 4);
    long_to_char(&sck.uxsnddata[8], agt.ownpid);

    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02220, KDDS02220), sck.gwname);
    AgentMsg(LOG_ERR, msg.buf);

    if (sendto(sck.ux_fd, sck.uxsnddata, GWDOWN_MSG_SIZE, 0,
	       &sck.sysc_adr, sck.sysc_adrlen) != GWDOWN_MSG_SIZE) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_24025, errno);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02033, KDDS02033), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }

    AgentTrace(TRACEERR,"UXSEND", 12, sck.uxsnddata, 12);
}

