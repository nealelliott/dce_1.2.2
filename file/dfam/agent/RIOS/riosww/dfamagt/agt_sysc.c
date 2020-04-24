/*
 * @HITACHI_COPYRIGHT@
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
static char *RCSID_agt_sysc_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_sysc.c,v 1.1.2.6 1996/04/17 17:28:05 root Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: agt_sysc.c,v $
 * Revision 1.1.2.6  1996/04/17  17:28:05  root
 * 	OT 13430 Submit
 * 	[1996/04/17  17:27:44  root]
 *
 * Revision 1.1.2.13  1996/03/29  01:30:03  ichikawa
 * 	Add agt_sig_action(SIGUSR1,agt__sysc_sig_usr1) to agt__sysc_child_process().
 * 	[1996/03/29  01:29:41  ichikawa]
 * 
 * Revision 1.1.2.12  96/02/14  02:26:02  02:26:02  ichikawa (T.Ichikawa)
 * 	Only when process exit D_EXIT_NETWORK,KDDS02134 message output.
 * 	[1996/02/14  02:25:51  ichikawa]
 * 
 * Revision 1.1.2.11  1996/01/12  04:57:56  kishi
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:57:39  kishi]
 * 
 * Revision 1.1.2.10  1995/10/26  01:12:04  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:11:44  kishi]
 * 
 * Revision 1.1.2.9  1995/10/23  01:46:59  ichikawa
 * 	Before agt_abort,add AgentMsg.
 * 	[1995/10/23  01:45:59  ichikawa]
 * 
 * Revision 1.1.2.8  1995/10/13  09:15:28  kishi
 * 	Add header comment.
 * 	[1995/10/13  09:15:17  kishi]
 * 
 * Revision 1.1.2.7  1995/09/28  12:04:44  kishi
 * 	Add header comment .
 * 	[1995/09/28  12:04:31  kishi]
 * 
 * Revision 1.1.2.6  1995/09/23  09:50:40  kishi
 * 	Add header comment.
 * 	[1995/09/23  09:42:01  kishi]
 * 
 * Revision 1.1.2.5  1995/09/19  10:43:53  ichikawa
 * 	Comment out miss.
 * 	[1995/09/19  10:43:40  ichikawa]
 * 
 * Revision 1.1.2.4  1995/09/19  07:32:01  ichikawa
 * 	Revision check deleted.
 * 	[1995/09/19  07:31:47  ichikawa]
 * 
 * Revision 1.1.2.3  1995/09/14  07:00:50  ichikawa
 * 	accept error management is changed.
 * 	[1995/09/14  07:00:09  ichikawa]
 * 
 * Revision 1.1.2.2  1995/09/14  05:41:37  ichikawa
 * 	accept error management is changed.
 * 	[1995/09/14  05:41:21  ichikawa]
 * 
 * Revision 1.1.2.1  1995/09/09  06:49:17  kishi
 * 	Initial revision
 * 	[1995/09/09  06:43:59  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#undef exit

#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <syslog.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <netinet/tcp.h>
#include <memory.h>
#include <sys/socketvar.h>

#ifdef _DEBUG
  #include <stdio.h>
  #include "agt_main.h"
#endif

#include "agt_agent.h"
#include "agt_errid.h"
#include "agt_sysc.h"
#include "agt_msg.h"
#include "agt_ipc.h"
#include "agt_sck.h"
#include "agt_timer.h"
#include "agt_usrc.h"
#include "agt_lib.h"
#include "agt_crypt.h"
#include "agt_sem.h"
#include "agt_sig.h"

struct d_local_sysc lsysc = {
    NULL
};


static void agt__sysc_init_signals(void);
static void agt__sysc_alloc_local_mem(void);
static void agt__sysc_close_fds(void);
static void agt__sysc_open_socks(void);
static void agt__sysc_open_stf(void);
static void agt__sysc_tell_ready_to_main(void);
static int agt__sysc_process_uxmsg(void);
static void agt__sysc_connect_done(char *buf, int len,
				   struct sockaddr_un *from, int fromlen);
static void agt__sysc_login_done(char *buf, int len,
				 struct sockaddr_un *from, int fromlen);
static void agt__sysc_gateway_down(char *buf, int len,
				   struct sockaddr_un *from, int fromlen);
static int agt__sysc_send_msg(char *buf, int len,
			      struct sockaddr_un *from, int fromlen);
static int agt__sysc_send_gw_msg(char *buf, int len,
			      struct sockaddr_un *from, int fromlen);
static int agt__sysc_start_stop(char *buf, int len,
				struct sockaddr_un *from, int fromlen);
static int agt__sysc_start_abort(char *buf, int len,
				 struct sockaddr_un *from, int fromlen);
static int agt__sysc_process_inet_accept(void);
static int agt__sysc_check_signal(void);
static void agt__sysc_sig_chld(int sig, int code, struct sigcontext *scp);
static void agt__sysc_aborting(struct agt_timer *tp);
static long agt__sysc_get_conn_num(void);
static void agt__sysc_kill_due_to_gwdown(pid_t pid);
static void agt__sysc_child_process(struct sockaddr_in *from, int fromlen);
static struct d_sysc *agt__sysc_get_sysc(struct d_sysc **top,
				 struct d_sysc **last, struct d_sysc *p);
static struct d_sysc *agt__sysc_find_sysc_by_pid(struct d_sysc *top,
						 pid_t key);
static struct d_sysc *agt__sysc_find_sysc_by_type(struct d_sysc *top,
						  char *key);
static struct d_sysc *agt__sysc_find_sysc_reconn(struct d_sysc *top,
						 long con_no, long addr);
static struct d_sysc *agt__sysc_find_sysc_current(struct d_sysc *top,
						  char *type, long addr);
static void agt__sysc_get_another_SYSC(void);
static struct d_sysc *agt__sysc_find_sysc_by_ipadr_alive(
					 struct d_sysc *top, long key);
static void agt__sysc_sig_usr1(int sig, int code, struct sigcontext *scp);
static void agt__sysc_abort_timeout(struct agt_timer *tp);
static void agt__sysc_to_hexstring(char *buf, int bufsz, char *str, int len);



/**************************************************************************/
/* ID   = agt_sysc							  */
/* name = Main routine of system communication process			  */
/* func = Perform the following tasks:					  */
/*	  1.set functions to be executed when transmitting SIGHUP,SIGTERM,*/
/*      SIGUSR1,SIGCHLD signals						  */
/*	  2.alloc area to be used by timer execution function		  */
/*	  3.close file descriptors not used by system communication process*/
/*	  4.open socket used during transmissions by system communication */
/*      and wait untill a request is made				  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_sysc(void)
{
    int readyfdnum;
    int nfds;
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    int rtn;
    int continue_loop = D_TRUE;
    int saved_errno;

    agt.status = D_SYSCOM_ST_INIT;

    agt__sysc_init_signals();
    agt__sysc_alloc_local_mem();
    agt__sysc_close_fds();
    agt__sysc_open_socks();
    agt__sysc_open_stf();
    agt.status = D_SYSCOM_ST_RUN;
    agt__sysc_tell_ready_to_main();


    nfds = ((sck.ux_fd > sck.gw_fd) ? sck.ux_fd : sck.gw_fd) + 1;

    while (continue_loop) {
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);
	FD_SET(sck.ux_fd, &readfds);
	FD_SET(sck.gw_fd, &readfds);

	rtn = pthread_mutex_unlock(&sig_mutex);
	if (rtn == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_22031, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}

	errno = 0;
	readyfdnum = select(nfds, (int *)&readfds,
		       (int *)&writefds, (int *)&exceptfds, NULL);
	saved_errno = errno;

	rtn = pthread_mutex_lock(&sig_mutex);
	if (rtn == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_22003, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}

	sprintf(msg.buf, "SELECT %i,err:%i", readyfdnum, errno);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);

	errno = saved_errno;
	switch (readyfdnum) {
	case 0:
	    sprintf(msg.wkbuf, "%i",  D_EID_22041);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	case -1:
	    switch (errno) {
	    case EINTR:
		continue;
	    default:
		sprintf(msg.wkbuf, "%i,%i",  D_EID_22001, errno);
		sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	    break;
	default:
	    if (FD_ISSET(sck.ux_fd, &readfds)) {
		continue_loop = agt__sysc_process_uxmsg();
	    } else if (FD_ISSET(sck.gw_fd, &readfds)) {
		continue_loop = agt__sysc_process_inet_accept();
	    } else {
		sprintf(msg.wkbuf, "%i", D_EID_22002);
		sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
	    }
	    break;
	}
    }

    agt_exit(D_EXIT_OK);
}


/**************************************************************************/
/* ID	= agt__sysc_init_signals					  */
/* name = Set function used during signal transmission			  */
/* func = Set function used during SIGHUP,SIGTERM,SIGUSR1,SIGCHLD signals */
/*        transmission. At the same time, initializes timer function	  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_init_signals(void)
{
    agt_sig_action(SIGHUP, agt_sig_ok_exit);
    agt_sig_action(SIGTERM, agt_sig_ignore);
    agt_sig_action(SIGUSR1, agt__sysc_sig_usr1);
    agt_sig_action(SIGCHLD, agt__sysc_sig_chld);

    AgentInitTimer();

}


/**************************************************************************/
/* ID	= agt__sysc_alloc_local_mem					  */
/* name = Alloc area used by timer function				  */
/* func = Alloc area for lsysc.stoptimer variable used by timer function  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_alloc_local_mem(void)
{
    lsysc.stoptimer = (struct agt_timer *)malloc(sizeof(struct agt_timer));
    if (lsysc.stoptimer == NULL) {
	switch (errno) {
	case ENOMEM:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_22007, sizeof(struct agt_timer));
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_22008, errno, sizeof(struct agt_timer));
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }
}


/**************************************************************************/
/* ID	= agt__sysc_close_fds						  */
/* name = Close unnecessary file descriptors				  */
/* func = Close file descriptors not used by system communication process.*/
/*        Close file descriptor used by operation information file and    */
/*        other openned descriptors					  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_close_fds(void)
{

    close(sck.ux_fd);
    close(agt.statfilefd);
}


/*****************************************************************************/
/* ID	= agt__sysc_open_socks						     */
/*                                                                           */
/* name = Opening socket for system communication process                    */
/* func = Opens socket (used for the system communication process) for the   */
/*        inet domain and unix domain.  Then calls the listen system call,   */
/*        and waits the connection establishment request from Gateway.       */
/*                                                                           */
/* i/o    = none                                                             */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static void agt__sysc_open_socks(void)
{
    struct sockaddr_in socadr;


    sck.ux_fd = agt_sck_open_unixsocket(sck.sysc_adr.sun_path);


    socadr.sin_family = AF_INET;
    socadr.sin_port = sck.portnumber;
    socadr.sin_addr.s_addr = INADDR_ANY;

    sck.gw_fd = agt_sck_open_inetsocket(&socadr);

    if (sck.gw_fd == -1) {
	agt_exit(D_EXIT_OK);
    }

    if (listen(sck.gw_fd, D_LISTEN_MAX) == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_22114, errno);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }
}


/*****************************************************************************/
/* ID	= agt__sysc_open_stf						     */
/*                                                                           */
/* name = Opening "System status information file"                           */
/* func = Invokes the agt_stat_open_file function to open the System         */
/*        status information file.                                           */
/*                                                                           */
/* i/o    = none                                                             */
/* return = none                                                             */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static void agt__sysc_open_stf(void)
{
    agt.statfilefd = agt_stat_open_file(agt.statfilename, O_RDWR);
}


/**************************************************************************/
/* ID	= agt__sysc_tell_ready_to_main					  */
/* name = Tell main process	that initialization has been completed    */
/* func = Communicate main process tha system communication process       */
/*        initialization is finished					  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_tell_ready_to_main(void)
{
#define READY_MSG_SIZE 8
    char mbuf[8];

    long_to_char(&mbuf[0], D_AG_SRDY);
    long_to_char(&mbuf[4], 0);

    if (sendto(sck.ux_fd, mbuf, READY_MSG_SIZE, 0,
	       &sck.main_adr, sck.main_adrlen) != READY_MSG_SIZE) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_22009, errno);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }

    AgentTrace(TRACEERR,"UXSEND", READY_MSG_SIZE, mbuf, READY_MSG_SIZE);
}


/**************************************************************************/
/* ID	= agt__sysc_process_uxmsg					  */
/* name = Send message through UNIX domain				  */
/* func = Send message through UNIX domain. Depending on the message,     */
/*        connection, login or agent stopping process can be called       */
/* i/o = none		                                                  */
/* return =								  */
/*	D_TRUE:normal end						  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__sysc_process_uxmsg(void)
{
    int msgtype;

recv_loop:
    memset((char *)&sck.from, '\0', sizeof(sck.from));
    sck.fromlen = sizeof(sck.from);
    if ((sck.uxrcvlen = recvfrom(sck.ux_fd, sck.uxrcvdata, sck.uxrcvbufsize,
			 0, &sck.from, &sck.fromlen)) == -1) {
	switch (errno) {
	case EINTR:
	    goto recv_loop;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_22010, errno);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }

    sck.from.sun_path[sck.fromlen-sizeof(sck.from.sun_family)] = '\0';

    AgentTrace(TRACEERR,"UXRECV", sck.uxrcvlen, sck.uxrcvdata, sck.uxrcvlen);

    if (sck.uxrcvlen < 8) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22011, sck.uxrcvlen, sck.from.sun_path);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	return D_TRUE;
    }

    switch (msgtype = char_to_num(sck.uxrcvdata, 4)) {
    case D_AG_CONNEST:
	agt__sysc_connect_done(sck.uxrcvdata, sck.uxrcvlen,
			       &sck.from, sck.fromlen);
	break;
    case D_AG_LOGIN:
	agt__sysc_login_done(sck.uxrcvdata, sck.uxrcvlen,
			     &sck.from, sck.fromlen);
	break;
    case D_AG_GWDOWN:
	agt__sysc_gateway_down(sck.uxrcvdata, sck.uxrcvlen,
			       &sck.from, sck.fromlen);
	break;
    case D_AG_MSG:
	agt__sysc_send_msg(sck.uxrcvdata, sck.uxrcvlen,
			   &sck.from, sck.fromlen);
	break;
    case D_AG_WMSG:
	agt__sysc_send_gw_msg(sck.uxrcvdata, sck.uxrcvlen,
			   &sck.from, sck.fromlen);
	  break;
    case D_AG_STOP:
	agt__sysc_start_stop(sck.uxrcvdata, sck.uxrcvlen,
			     &sck.from, sck.fromlen);
	break;
    case D_AG_ABORT:
	agt__sysc_start_abort(sck.uxrcvdata, sck.uxrcvlen,
			      &sck.from, sck.fromlen);
	break;
    default:
	sprintf(msg.wkbuf, "%i,%i,%i,%s).\n", D_EID_22012, msgtype, sck.uxrcvlen, sck.from.sun_path);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	return D_TRUE;
    }


    return D_TRUE;
}


/**************************************************************************/
/* ID	= agt__sysc_connect_done					  */
/* name = Receive information about connection				  */
/* func = Receive information about connection requested by user, manager,*/
/*        or watchdog process. The receiving time is registered in the    */
/*        operation information file. When reconnection is requested, kill*/
/*        the corresponding process					  */
/* i/o =								  */
/* 	buf    :char *	    :IN:connection information			  */
/*	len    :int		    :IN:length of buf			  */
/*	from   :struct sockaddr_un *:IN:address of sender process         */
/*	fromlen:int		    :IN:number of bytes pointed by from	  */
/* return = none							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_connect_done(char *buf, int len,
				   struct sockaddr_un *from, int fromlen)
{
    int extlen;
    char id[5];
    long pid;

    int i;
    char *bp;
    struct d_sysc *syscp;
    struct d_sysc *syscp2;


    if ((extlen = char_to_num(&buf[4], 4)) != 8) {
	sprintf(msg.wkbuf, "%i,%s,%i", D_EID_22013, from->sun_path, extlen);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	return;
    }

    for (i = 0, bp = buf+8; i < 4; i++, bp++) {
	id[i] = *bp;
    }
    id[4] = '\0';
    pid = char_to_num(&buf[12], 4);

    syscp = agt__sysc_find_sysc_by_pid(agt.main->iusysctop, pid);
    if (syscp == NULL) {
	sprintf(msg.wkbuf, "%i,%s,%s,%i", D_EID_22014, from->sun_path, id, pid);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
    } else {
	if (syscp->status != D_SYSC_ST_INIT) {
	    sprintf(msg.wkbuf, "%i,%s,%x,%s,%i", D_EID_22015, from->sun_path, syscp->status, id, pid);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	    AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);

	    goto done;
	}

	syscp->status = D_SYSC_ST_RUN;
	memcpy(syscp->ptype, id, 4);

	agt_stat_update_entry(agt.statfilefd, pid, id, NULL, NULL);

	if (memcmp(syscp->ptype, D_PTYP_USRM, 4) == 0) {



	    syscp2 = agt__sysc_find_sysc_reconn(agt.main->iusysctop,
				    syscp->conn_no, syscp->ipaddr);

	    if (syscp2 != NULL) {
		char *cp;
		struct in_addr gwad;

		gwad.s_addr = syscp2->ipaddr;
		cp = agt_get_gwname(gwad);
		sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02135, KDDS02135), cp);
		AgentMsg(LOG_INFO, msg.buf);
		free(cp);

		do {

		    sprintf(msg.buf, "RECONNECT:SIGUSR1 to %c%c%c%c:%i.\n", syscp2->ptype[0], syscp2->ptype[1], syscp2->ptype[2], syscp2->ptype[3], syscp2->pid);
		    AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);

		    if (kill(syscp2->pid, SIGUSR1) == -1) {
			switch (errno) {
			case ESRCH:
			    break;
			default:
			    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_22016, errno, syscp2->pid);
			    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
			    AgentMsg(LOG_ERR, msg.buf);
			    agt_abort();
			    break;
			}
		    } else {
			syscp2->status = D_SYSC_ST_TERM;
		    }
		} while ((syscp2 = agt__sysc_find_sysc_reconn(syscp2->next,
				syscp->conn_no, syscp->ipaddr)) != NULL);
	    }
	}
    }

done:
    ;
}



/**************************************************************************/
/* ID	= agt__sysc_login_done						  */
/* name = Receive information about login				  */
/* func = Receive information about login requested by user, manager,     */
/*        or watchdog process. The receiving time is registered in the    */
/*        operation information file. When reconnection is requested, kill*/
/*        the corresponding process					  */
/* i/o =								  */
/*	buf    :char *		:IN:login information			  */
/*	len    :int		    :IN:length of buf			  */
/*	from   :struct sockaddr_un *:IN:address of sender process         */
/*	fromlen:int		    :IN:number of bytes pointed by from	  */
/* return = none							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_login_done(char *buf, int len,
				 struct sockaddr_un *from, int fromlen)
{
    int extlen;
    long pid;

    if ((extlen = char_to_num(&buf[4], 4)) < 5) {
	sprintf(msg.wkbuf, "%i,%s,%i", D_EID_22042, from->sun_path, extlen);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	return;
    }

    pid = char_to_num(&buf[8], 4);

    agt_stat_update_entry(agt.statfilefd, pid, NULL, NULL, &buf[12]);

    return;
}


/**************************************************************************/
/* ID	= agt__sysc_gateway_down					  */
/* name = Receive information about gateway shutdown			  */
/* func = Receive information about gateway shutdown. Kill all processes  */
/*        corresponding to that gateway					  */
/* i/o =								  */
/*	buf    :char *		:IN:login information			  */
/*	len    :int		    :IN:length of buf			  */
/*	from   :struct sockaddr_un *:IN:address of sender process         */
/*	fromlen:int		    :IN:number of bytes pointed by from	  */
/* return = none							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_gateway_down(char *buf, int len,
				   struct sockaddr_un *from, int fromlen)
{
    int extlen;
    pid_t pid;

    if ((extlen = char_to_num(&buf[4], 4)) != 4) {
	sprintf(msg.wkbuf, "%i,%s,%i", D_EID_22017, from->sun_path, extlen);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);

	return;
    }
    pid = char_to_num(&buf[8], 4);

    agt__sysc_kill_due_to_gwdown(pid);

}


/**************************************************************************/
/* ID	= agt__sysc_send_msg 						  */
/* name = Send message to gateway					  */
/* func = Send message to gateway					  */
/* i/o =								  */
/* 	buf    :char *		:IN:message				  */
/*  len    :int		    :IN:length of buf				  */
/*	from   :struct sockaddr_un *:IN:address of sender process         */
/*	fromlen:int		    :IN:number of bytes pointed by from	  */
/* return =     							  */
/*	D_FALSE:system communication process is in exit status		  */
/*  D_TRUE :otherwise						  	  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__sysc_send_msg(char *buf, int len,
			      struct sockaddr_un *from, int fromlen)
{
    int extlen;
    long msglen;
    char *msgp;
    long num_dest;

    long destcount;
    char *destp;
    struct in_addr ipaddr;
    struct d_sysc *syscp;

    if ((extlen = char_to_num(&buf[4], 4)) < 12) {
	sprintf(msg.wkbuf, "%i,%s,%i", D_EID_22020, from->sun_path, extlen);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	return D_TRUE;
    }

    if ((msglen = char_to_num(&buf[16], 4)) < 1) {
	sprintf(msg.wkbuf, "%i,%s,%i", D_EID_22022, from->sun_path, msglen);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	return D_TRUE;
    }
    msglen = msglen + 4*3;

    msgp = &buf[8];


    if ((num_dest = char_to_num(&buf[8+msglen], 4)) < 1) {
	sprintf(msg.wkbuf, "%i,%s,%i", D_EID_22023, from->sun_path, num_dest);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	return D_TRUE;
    }

    destp = &buf[12+msglen];

    for (destcount = 0; destcount < num_dest; destcount++) {
	ipaddr.s_addr = (unsigned long)char_to_num(destp, 4);
	destp += 4;

	syscp = agt__sysc_find_sysc_current(agt.main->iusysctop,
					    D_PTYP_WCHD, (long)ipaddr.s_addr);
	if (syscp == NULL) {
	    char *gwname;

	    gwname = agt_get_gwname(ipaddr);
	    agt__sysc_to_hexstring(msg.wkbuf, sizeof(msg.wkbuf), msgp, msglen);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02330, KDDS02330), gwname, msg.wkbuf);
	    AgentMsg(LOG_NOTICE, msg.buf);
	    free(gwname);
	    continue;
	} else {
	    GatewayMsg(msglen, msgp, syscp->pid);
	}
    }

    return D_TRUE;
}


/**************************************************************************/
/* ID	= agt__sysc_send_gw_msg						  */
/* name = Send message to watchdog process				  */
/* func = Send message to watchdog process connected to gateway           */
/* i/o =                                                                  */
/*      buf    :char *              :IN:message				  */
/*      len    :int                 :IN:buf length                        */
/*      from   :struct sockaddr_un *:IN:process address                   */
/*      fromlen:int                 :IN:number of bytes pointed by from	  */
/* return =     							  */
/*	D_FALSE:system communication process is in exit status		  */
/*  D_TRUE :otherwise						  	  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__sysc_send_gw_msg(char *buf, int len,
			      struct sockaddr_un *from, int fromlen)
{
    int extlen;
    long msglen;
    char *msgp;
    struct in_addr ipaddr;
    struct d_sysc *syscp;


    if ((extlen = char_to_num(&buf[4], 4)) < 12) {
	sprintf(msg.wkbuf, "%i,%s,%i", D_EID_22020, from->sun_path, extlen);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	return D_TRUE;
    }
    ipaddr.s_addr  = (unsigned long)char_to_num(&buf[8], 4);

    msglen = len - 12;
    msgp = &buf[12];

    syscp = agt__sysc_find_sysc_current(agt.main->iusysctop,
					D_PTYP_WCHD, (long)ipaddr.s_addr);
    if (syscp == NULL) {
	char *gwname;

	gwname = agt_get_gwname(ipaddr);
	agt__sysc_to_hexstring(msg.wkbuf, sizeof(msg.wkbuf), msgp, msglen);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02330, KDDS02330), gwname, msg.wkbuf);
	AgentMsg(LOG_NOTICE, msg.buf);
	free(gwname);
    } else {
	GatewayMsg(msglen, msgp, syscp->pid);
    }

    return D_TRUE;
}


/**************************************************************************/
/* ID	= agt__sysc_start_stop						  */
/* name = Stop system communication process				  */
/* func = Stop system communication process. If there is any gateway      */
/*        connected, send a message to communicate that the system        */
/*        communication process stopped					  */
/* i/o =                                                                  */
/*      buf    :char *              :IN:message				  */
/*      len    :int                 :IN:buf length                        */
/*      from   :struct sockaddr_un *:IN:process address                   */
/*      fromlen:int                 :IN:number of bytes pointed by from	  */
/* return =     							  */
/*	D_FALSE:system communication process is in exit status		  */
/*  D_TRUE :otherwise						  	  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__sysc_start_stop(char *buf, int len,
				struct sockaddr_un *from, int fromlen)
{
    int extlen;
    long seconds;
    struct d_sysc *syscp;
    struct d_sysc *wp;
#define STOP_MSG_SIZE 128
    char mbuf[STOP_MSG_SIZE];
    int msglen;


    if ((extlen = char_to_num(&buf[4], 4)) != 4) {
	sprintf(msg.wkbuf, "%i,%s,%i", D_EID_22024, from->sun_path, extlen);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	return D_TRUE;
    }

    if (agt.status < D_SYSCOM_ST_STOP) {
	seconds = char_to_num(&buf[8], 4);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02002, KDDS02002), seconds);
	AgentMsg(LOG_NOTICE, msg.buf);

	msglen = 8;
	long_to_char(&mbuf[0], D_GW_MSG_STOP);
	long_to_char(&mbuf[4], seconds);

	wp = agt.main->iusysctop;
	while ((syscp =
		agt__sysc_find_sysc_by_type(wp, D_PTYP_WCHD)) != NULL) {
	    wp = syscp->next;

	    if (syscp->status < D_SYSC_ST_TERM) {
		GatewayMsg(msglen, mbuf, syscp->pid);
	    }
	}

	agt.status = D_SYSCOM_ST_STOP;

	lsysc.stoptimer->seconds = seconds;
	lsysc.stoptimer->timer_func = agt__sysc_aborting;
	lsysc.stoptimer->info = (struct agt_timerinfo *)D_TIMER_STOP;
	AgentSetTimer(lsysc.stoptimer);
    }

    if (agt.main->iusysctop == NULL) {
	agt_exit(D_EXIT_OK);
    }

    return D_TRUE;
}


/**************************************************************************/
/* ID	= agt__sysc_start_abort						  */
/* name = Abort system communication process				  */
/* func = Abort system communication process. If there is any gateway     */
/*        connected, send a message to communicate that the system        */
/*        communication process aborted					  */
/* i/o =                                                                  */
/*      buf    :char *              :IN:message				  */
/*      len    :int                 :IN:buf length                        */
/*      from   :struct sockaddr_un *:IN:process address                   */
/*      fromlen:int                 :IN:number of bytes pointed by from	  */
/* return =                                                               */
/*	D_FALSE:system communication process is in exit status		  */
/*  D_TRUE :otherwise						  	  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__sysc_start_abort(char *buf, int len,
				 struct sockaddr_un *from, int fromlen)
{
    int extlen;

    if ((extlen = char_to_num(&buf[4], 4)) != 4) {
	sprintf(msg.wkbuf, "%i,%s,%i", D_EID_22025, from->sun_path, extlen);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	return D_TRUE;
    }

    if (agt.status < D_SYSCOM_ST_ABORT) {
	agt__sysc_aborting(NULL);
    }

    if (agt.main->iusysctop == NULL) {
	agt_exit(D_EXIT_OK);
    }

    return D_TRUE;
}


/**************************************************************************/
/* ID   = agt__sysc_process_inet_accept					  */
/* name = Connection							  */
/* func = Connect to gateway accoding to connect request. Register        */
/*        connection information in the system communication table. Then  */
/*        create child processes to continue execution                    */
/* i/o = none								  */
/* return =                                                               */ 
/*	D_FALSE:system communication process is in exit status		  */
/*  D_TRUE :otherwise						  	  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__sysc_process_inet_accept(void)
{
    struct sockaddr_in remote;
    int remotelen;
    int newsock;
    pid_t childpid;
    struct d_sysc *syscp;
    char *p;

    remotelen = sizeof(remote);
accept_roop:
    newsock = accept(sck.gw_fd, &remote, &remotelen);
    if (newsock == -1) {
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }

{
    int optval = 1;

    if (setsockopt(newsock,IPPROTO_TCP,TCP_NODELAY,&optval,sizeof(optval)) == -1){
    	sprintf(msg.wkbuf,"%i,%i",D_EID_22043,errno);
	sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02031,KDDS02031),msg.wkbuf);
	AgentMsg(LOG_ERR,msg.buf);
	agt_abort();
    }
}

    p = agt_get_gwname(remote.sin_addr);

    sprintf(msg.buf, "ACCEPT %s.%i", p, remote.sin_port);
    AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);

    if (agt.status >= D_SYSCOM_ST_STOP) {
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02132, KDDS02132), p);
	AgentMsg(LOG_NOTICE, msg.buf);

	if (close(newsock) == -1) {
	    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22027, errno, p);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}

	free(p);
	return D_TRUE;
    }

    if (sck.numremote > 0) {
	long i;

	for (i = 0;
	     (i < sck.numremote) &&
	     (sck.remoteip[i] != remote.sin_addr.s_addr);
	     i++);
	if (i >= sck.numremote) {
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02133, KDDS02133), p);
	    AgentMsg(LOG_WARNING, msg.buf);

	    if (shutdown(newsock, 2) == -1) {
		switch (errno) {
		case ENETDOWN:
		case ENOTCONN:
		    sprintf(msg.wkbuf, "%i", D_EID_22028);
		    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02562, KDDS02562), p, strerror(errno), msg.wkbuf);
		    AgentMsg(LOG_WARNING, msg.buf);
		    break;
		default:
		    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22028, errno, p);
		    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_abort();
		    break;
		}
	    }

	    if (close(newsock) == -1) {
		sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22029, errno, p);
		sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
	    }

	    free(p);
	    return D_TRUE;
	}
    }

    syscp = agt__sysc_get_sysc(&agt.main->frsysctop,
			       &agt.main->frsysclst, agt.main->frsysctop);
    if (syscp == NULL) {
	if (agt.main->num_sysc_segments == 1) {
	    agt__sysc_get_another_SYSC();
	}
	syscp = agt__sysc_get_sysc(&agt.main->frsysctop,
				   &agt.main->frsysclst, agt.main->frsysctop);
	if (syscp != NULL) {
	    goto sysc_expanded;
	}

	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02131, KDDS02131), p);
	AgentMsg(LOG_WARNING, msg.buf);

	if (close(newsock) == -1) {
	    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22030, errno, p);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}

	free(p);
	return D_TRUE;
    }

sysc_expanded:

    syscp->status = D_SYSC_ST_INIT;
    syscp->ipaddr = remote.sin_addr.s_addr;
    syscp->portno = remote.sin_port;
    memcpy(&syscp->ptype, "    ", 4);
    syscp->conn_no = agt__sysc_get_conn_num();
    syscp->exitstatus = 0;
    syscp->time_signaled.tv_sec = 0;
    syscp->time_signaled.tv_usec = 0;
    syscp->gw_conn_no = 0;
    agt_sysc_add_sysc(&agt.main->iusysctop, &agt.main->iusysclst, syscp);

    if ((childpid = fork()) == 0) {

	agt.ownpid = getpid();
	memcpy(&agt.proctype, "    ", 4);

	sck.gwipaddr = remote.sin_addr;
	sck.gwport = remote.sin_port;
	sck.gwconnid = 0;
	sck.gwname = p;

	AgentTrace(TRACEDEB,"PSTART", 0, NULL, 0);

	agt_sig_init_signal_threads();

#ifdef _DEBUG
	sprintf(msg.buf, "****: CHILD process starts.\n");
	AgentMsg(LOG_DEBUG, msg.buf);
	debug_wait_signal();
#endif

	close(sck.gw_fd);
	sck.gw_fd = newsock;
	if (close(sck.ux_fd) == -1) {
	    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_22101, errno, sck.ux_fd);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02034, KDDS02034), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}

	agt__sysc_child_process(&remote, remotelen);


	sprintf(msg.wkbuf, "%i", D_EID_22102);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_exit(D_EXIT_UNKNOWN);

    } else if (childpid > 0) {
	char hostnamebuf[64];
	char portnumber[10];

	syscp->pid = childpid;

	sprintf(portnumber, ".%i", remote.sin_port);
	p = agt_get_gwname(remote.sin_addr);
	strcpy(hostnamebuf, p);
	strcat(hostnamebuf, portnumber);

	sprintf(msg.buf, "pid:%i,remote:%s", childpid, hostnamebuf);
	AgentTrace(TRACEERR,"FORK  ", strlen(msg.buf), msg.buf, 0);

	agt_stat_add_entry(agt.statfilefd, "****", childpid,
			   NULL, NULL, hostnamebuf, "");

    } else {
	switch (errno) {
	case EAGAIN:
	case ENOMEM:
	    sprintf(msg.wkbuf, "%i", D_EID_22032);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02130, KDDS02130), p, strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_WARNING, msg.buf);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22033, errno, p);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
	agt__sysc_get_sysc(&agt.main->iusysctop, &agt.main->iusysclst, syscp);
	agt_sysc_add_sysc(&agt.main->frsysctop, &agt.main->frsysclst, syscp);
    }

    if (close(newsock) == -1) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22034, errno, p);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }

    free(p);
    return D_TRUE;
}



/**************************************************************************/
/* ID	= agt__sysc_sigchld_after					  */
/* name = Routine after SIGCHLD signal					  */
/* func = Perform the following tasks:					  */
/*        1.write trace information about SIGCHLD signal transmission     */
/*        2.when managing or watchdog processes stop, kill co-related     */
/*          processes  							  */
/*        3.delete information about killed processes from operation      */
/*          information file						  */
/*        4.release area from system communication table used by killed   */
/*          processes							  */ 
/* i/o =								  */
/* 	syscp:struct d_sysc *:IN:pointer to system communication table	  */
/* return = none							  */ 
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__sysc_sigchld_after(struct d_sysc *syscp)
{
    int kill_flag = D_FALSE;

    {
	char timebuf[32];
	struct tm *loctime;
	time_t caltime;
	int timelen;

	caltime = syscp->time_signaled.tv_sec;
	loctime = localtime(&caltime);
	timelen = strftime(timebuf, 31, "%Y/%m/%d %H:%M:%S", loctime);
	sprintf(msg.buf, "SIGCHLD pid:%i,%s.%06i:%c%c%c%c,%s,conn:%i,st:0x%x", syscp->pid, timebuf, syscp->time_signaled.tv_usec, syscp->ptype[0],syscp->ptype[1], syscp->ptype[2], syscp->ptype[3], inet_ntoa(syscp->ipaddr), syscp->conn_no, syscp->exitstatus);
	AgentTrace(TRACEERR,"SIGNAL", strlen(msg.buf), msg.buf, 0);
    }

#ifdef _DEBUG
    if (WIFSIGNALED(syscp->exitstatus)) {
	sprintf(msg.buf, "****: %c%c%c%c:%i killed by SIGNAL:%i\n", syscp->ptype[0],syscp->ptype[1], syscp->ptype[2], syscp->ptype[3], syscp->pid, WTERMSIG(syscp->exitstatus));
	AgentMsg(LOG_DEBUG, msg.buf);

	if ((access(DEBUG_SCRIPT_FILE_S, X_OK) == 0) &&
	    (fork() == 0)) {
	    char cmdbuf[200];
	    sprintf(cmdbuf, "%s %s %i %i %s\n", DEBUG_SCRIPT_FILE_S, agt.agenthostname, WTERMSIG(syscp->exitstatus), syscp->pid, agt.agent_id);
	    system(cmdbuf);
	    agt_bare_exit(0);
	}
    }
#endif

    if (memcmp(syscp->ptype, D_PTYP_USRM, 4) == 0) {
	if (syscp->exitstatus != 0) {
	    kill_flag = D_TRUE;
	} else {
	    if (agt__sysc_find_sysc_by_ipadr_alive(syscp->next, syscp->ipaddr)
		!= NULL) {
		kill_flag = D_TRUE;
	    }
	}
    }

    if (memcmp(syscp->ptype, D_PTYP_WCHD, 4) == 0) {
	if (syscp->exitstatus != 0) {
	    kill_flag = D_TRUE;
	} else {
	    if (agt__sysc_find_sysc_by_ipadr_alive(syscp->next, syscp->ipaddr)
		!= NULL) {
		kill_flag = D_TRUE;
	    }
	}
    }

    if (kill_flag == D_TRUE) {
	char *p;
	struct in_addr gwad;

	if (syscp->status != D_SYSC_ST_TERM){
		if (syscp->exitstatus == D_EXIT_NETWORK){
			gwad.s_addr = syscp->ipaddr;
			p = agt_get_gwname(gwad);

			sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02134, KDDS02134), p);
			AgentMsg(LOG_ERR, msg.buf);
			free(p);
		}
	}

	agt__sysc_kill_due_to_gwdown(syscp->pid);
    }

    agt__sysc_get_sysc(&agt.main->iusysctop,
		       &agt.main->iusysclst, syscp);
    agt_stat_delete_entry(agt.statfilefd, syscp->pid);
    agt_sysc_add_sysc(&agt.main->frsysctop,
		      &agt.main->frsysclst, syscp);

skip_sysc_check:

    if ((agt.status >= D_SYSCOM_ST_STOP) &&
	(agt.main->iusysctop == NULL)) {
	agt_bare_exit(D_EXIT_OK);
    }
}


/**************************************************************************/
/* ID = agt__sysc_sig_chld						  */
/* name = Routine to execute during SIGCHLD transmission    		  */
/* func = Routine to execute during SIGCHLD transmission. If there are    */
/*        killed co-processes, call agt__sysc_sigchld_after function to   */
/*        and continue other processing					  */
/* i/o =								  */
/*      sig :int              :IN:signal number                           */
/*      code:int              :IN:status code                             */
/*      scp :struct sigcontext:IN:signal information                      */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_sig_chld(int sig, int code, struct sigcontext *scp)
{
    struct d_sysc *syscp;
    pid_t dpid;
    int exstatus;
    int savederr;

    while (1) {
	exstatus = 0;
	errno =0;
	dpid = waitpid(-1, &exstatus, WNOHANG);
	savederr = errno;


	errno = savederr;
	switch (dpid) {
	case 0:
	    goto wait_done;
	case -1:
	    switch (errno) {
	    case EINTR:
		continue;
	    case ECHILD:
		goto wait_done;
	    default:
		sprintf(msg.wkbuf, "%i", D_EID_22036);
		sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	    break;
	default:
	    syscp = agt__sysc_find_sysc_by_pid(agt.main->iusysctop, dpid);
	    if (syscp == NULL) {

		;
	    } else {
		syscp->status = D_SYSC_ST_DEAD;
		syscp->exitstatus = exstatus;
		if (gettimeofday(&syscp->time_signaled,
				 (struct timezone *)NULL) == -1) {
		    sprintf(msg.wkbuf, "%i,%i", D_EID_22038, errno);
		    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_abort();
		}

		agt__sysc_sigchld_after(syscp);
	    }
	}
    }
wait_done:
    ;
}


/**************************************************************************/
/* ID	= agt__sysc_aborting						  */	
/* name = Execute abort process						  */
/* func = Execute abort process. If watchdog process is executing on      */
/*        connected gateway, send abort message. THen kill all processes  */
/*        related to the aborted process 				  */
/* i/o =								  */
/*	tp:struct agt_timer *:IN:pointer to timer structure		  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_aborting(struct agt_timer *tp)
{
#define STOP_MSG_SIZE 128
    char mbuf[STOP_MSG_SIZE];
    int msglen;
    struct d_sysc *wp;
    struct d_sysc *syscp;


    agt.status = D_SYSCOM_ST_ABORT;

    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02003, KDDS02003));
    AgentMsg(LOG_NOTICE, msg.buf);

    msglen = 4;
    long_to_char(&mbuf[0], D_GW_MSG_ABRT);

    wp = agt.main->iusysctop;
    while ((syscp = agt__sysc_find_sysc_by_type(wp, D_PTYP_WCHD)) != NULL) {
	wp = syscp->next;

	if (syscp->status < D_SYSC_ST_TERM) {
	    GatewayMsg(msglen, mbuf, syscp->pid);
	}
    }


    agt_sleep((unsigned int)1);


    for (wp = agt.main->iusysctop; wp != NULL; wp = wp->next) {

	sprintf(msg.buf, "MAY send SIGUSR1 to %c%c%c%c:%i", wp->ptype[0], wp->ptype[1], wp->ptype[2], wp->ptype[3], wp->pid);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);

	if (wp->status < D_SYSC_ST_TERM) {
	    errno = 0;
	    if (kill(wp->pid, SIGUSR1) == -1) {
		switch (errno) {
		case ESRCH:
		    break;
		default:
		    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_22039, errno, wp->pid);
		    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_abort();
		}
	    }
	    wp->status = D_SYSC_ST_TERM;

	    sprintf(msg.buf, "ABORT: SIGUSR1 to %c%c%c%c:%i,err:%i", wp->ptype[0], wp->ptype[1], wp->ptype[2], wp->ptype[3], wp->pid, errno);
	    AgentTrace(TRACEERR,"KILL  ", strlen(msg.buf), msg.buf, 0);
	}
    }

    AgentResetTimer(lsysc.stoptimer);
    lsysc.stoptimer->seconds = D_TIMER_ABORT_TIME;
    lsysc.stoptimer->timer_func = agt__sysc_abort_timeout;
    lsysc.stoptimer->info = (struct agt_timerinfo *)D_TIMER_ABORT;
    AgentSetTimer(lsysc.stoptimer);

}


/**************************************************************************/
/* ID	= agt__sysc_kill_due_to_gwdown					  */
/* name = Kill processes due to gateway shutdown detection		  */	
/* func = When the gateway is shutdown, all related processes are killed  */
/* i/o =								  */
/*	pid:pid_t:IN:process-ID who detected shutdown			  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_kill_due_to_gwdown(pid_t pid)
{
    struct d_sysc *down;
    struct d_sysc *current;
    struct d_sysc *syscp;
    long con_no;

    down = agt__sysc_find_sysc_by_pid(agt.main->iusysctop, pid);
    if (down == NULL) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_22018, pid);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
    } else {
	current = agt__sysc_find_sysc_current(agt.main->iusysctop,
					      D_PTYP_1ST_CONN, down->ipaddr);

	if (down->conn_no >= current->conn_no) {
	    con_no = agt.main->iusysclst->conn_no+1;
	} else {
	    con_no = current->conn_no;
	}

	syscp = agt__sysc_find_sysc_reconn(agt.main->iusysctop,
					   con_no, down->ipaddr);
	if (syscp != NULL) {
	    do {
		errno = 0;
		if (kill(syscp->pid, SIGUSR1) == -1) {
		    switch (errno) {
		    case ESRCH:
			break;
		    default:
			sprintf(msg.wkbuf, "%i,%i,%i", D_EID_22019, errno, syscp->pid);
			sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
			break;
		    }
		}
		syscp->status = D_SYSC_ST_TERM;

		sprintf(msg.buf, "GW-DOWN: SIGUSR1 to %c%c%c%c:%i, err:%i", syscp->ptype[0], syscp->ptype[1], syscp->ptype[2], syscp->ptype[3], syscp->pid, errno);
		AgentTrace(TRACEERR,"KILL  ", strlen(msg.buf), msg.buf, 0);
	    } while ((syscp = agt__sysc_find_sysc_reconn(syscp->next, con_no,
						down->ipaddr)) != NULL);
	}
    }
}


static next_conn_num = 1;

/**************************************************************************/
/* ID   = agt__sysc_get_conn_num                                          */
/*                                                                           */
/* name = Reallocation of the connection number                              */
/* func = Renumber the connection number if the connection numbers used in   */
/*        the system table exceeds LONG_MAX.                                 */
/*                                                                           */
/* i/o    = none                                                             */
/* return = next_conn_num: connection number                                 */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
static long agt__sysc_get_conn_num(void)
{
    if (next_conn_num >= (LONG_MAX - 1)) {
	struct d_sysc *wp;
	long newnum;

	for (wp = agt.main->iusysctop, newnum = 1;
	     (wp != NULL); wp = wp->next, newnum++) {
	    wp->conn_no = newnum;
	}
	next_conn_num = newnum;
    }

    return next_conn_num++;
}



/**************************************************************************/
/* ID	= agt__sysc_child_process					  */
/* name = Select type of connection requested by gateway		  */	
/* func = Perform the following tasks:					  */
/*        1.alloc buffer as requested by gateway			  */
/*        2.check encryption procedure					  */
/*        3.call agt_usr_main or agt_wchd function according to type of   */
/*          connection requested by gateway				  */
/* i/o = 								  */
/*	from   :struct sockaddr_in *:IN:gateway address			  */
/*  fromlen:int		    :IN:size of area to store address		  */
/* return =	none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_child_process(struct sockaddr_in *from, int fromlen)
{
#define RBUF_FROM_GW 128
#define SBUF_TO_SYSCOM 128
    char rbuf[RBUF_FROM_GW];
    int recvlen;
    char sbuf[SBUF_TO_SYSCOM];
    int sendlen;
    int sendrtn;
    char myaddr[25];
#define GW_ERR_CRYPT  0x00000001
#define GW_ERR_PROT   0x00000002
#define GW_ERR_NOBUF  0x00000003
#define GW_ERR_UNKWN  0xffffffff
    int conn_type;
    int i;
    int rtnselect;
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    int fdmax;
    int gwreturncode;

    gwreturncode = 0;
    sprintf(myaddr, "%s%s%05i", sck.tmpl_adr.sun_path,
	    D_UXSOC_CHLD, agt.ownpid);


    sck.ux_fd = agt_sck_open_unixsocket(myaddr);

    agt_sig_action(SIGCHLD, agt_sig_ignore);
    agt_sig_action(SIGUSR1,agt__sysc_sig_usr1);

select_retry:

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);
    FD_SET(sck.gw_fd, &readfds);
    fdmax = sck.gw_fd + 1;
    errno = 0;
    rtnselect = select(fdmax, (int *)&readfds,
		       (int *)&writefds, (int *)&exceptfds, NULL);

    switch (rtnselect) {
    case -1:
	switch (errno) {
	case EINTR:
	    goto select_retry;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_22123, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
    case 1:
	break;
    default:
	sprintf(msg.wkbuf, "%i,%i", D_EID_22124, errno);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }

    recvlen = recv(sck.gw_fd, rbuf, RBUF_FROM_GW, 0);

    if (recvlen == -1) {
	switch (errno) {
	case EWOULDBLOCK:
	case EINTR:
	    goto select_retry;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22105, errno, sck.gwname);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }

    AgentTrace(TRACEERR,"RVDATA", recvlen, rbuf, recvlen);

    if (recvlen != 24) {
	if (recvlen == 0){
	    sprintf(msg.wkbuf, "%i,%i", D_EID_22122, recvlen);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02562, KDDS02562), sck.gwname, "0", msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_NETWORK);
	} else {
	    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22106, recvlen, sck.gwname);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	    AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	    gwreturncode = GW_ERR_UNKWN;
	    goto snd_reply_to_gw;
	}
    }

    if ((i = char_to_num(&rbuf[0], 2)) != D_GW_CONN) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22107, i, sck.gwname);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	gwreturncode = GW_ERR_UNKWN;
	goto snd_reply_to_gw;
    }
    if ((i = char_to_num(&rbuf[4], 4)) != 16) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22108, i, sck.gwname);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	gwreturncode = GW_ERR_UNKWN;

	goto snd_reply_to_gw;
    }
/*    if ((i = char_to_num(&rbuf[8], 2)) != D_GW_PROTVER) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22109, i, sck.gwname);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	gwreturncode = GW_ERR_PROT;
	goto snd_reply_to_gw;
    }
*/
    conn_type = char_to_num(&rbuf[10], 2);
    switch (conn_type) {
    case D_GW_CON_CTRL:
	memcpy(&agt.proctype, D_PTYP_USRM, 4);
	break;
    case D_GW_CON_WCHD:
	memcpy(&agt.proctype, D_PTYP_WCHD, 4);
	break;
    case D_GW_CON_USER:
	memcpy(&agt.proctype, D_PTYP_USRC, 4);
	break;
    default:
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22110, conn_type, sck.gwname);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	gwreturncode = GW_ERR_UNKWN;
	goto snd_reply_to_gw;
    }

    sck.gwsndbufsize = sck.gwrcvbufsize = char_to_num(&rbuf[12], 4);
    sck.gwsnddata = (char *)malloc(sck.gwsndbufsize);
    if (sck.gwsnddata == NULL) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22111, sck.gwsndbufsize, sck.gwname);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	gwreturncode = GW_ERR_NOBUF;
	goto snd_reply_to_gw;
    }
    sck.gwrcvdata = (char *)malloc(sck.gwrcvbufsize);
    if (sck.gwrcvdata == NULL) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22112, sck.gwrcvbufsize, sck.gwname);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	gwreturncode = GW_ERR_NOBUF;
	goto snd_reply_to_gw;
    }

    sck.gwconnid = char_to_num(&rbuf[16], 4);

    crypt_info.crypttype = char_to_num(&rbuf[20], 4);
    {
	int i;

	for (i = 0; crypt_tbl[i].crypttype != 0xffffffff; i++) {
	    if (crypt_tbl[i].crypttype == crypt_info.crypttype) {
		crypt_info.encrypt = crypt_tbl[i].encrypt;
		crypt_info.decrypt = crypt_tbl[i].decrypt;
		goto crypt_done;
	    }
	}
	gwreturncode = GW_ERR_CRYPT;

crypt_done:
	;
    }

snd_reply_to_gw:
    short_to_char(&sbuf[0], (short)D_GW_CONNR);
    short_to_char(&sbuf[2], (short)0);
    long_to_char(&sbuf[4], 10);

    long_to_char(&sbuf[8], gwreturncode);
    short_to_char(&sbuf[12], (short)D_GW_PROTVER);
    long_to_char(&sbuf[14], sysconf(_SC_OPEN_MAX));


    sendlen = 18;

    AgentTrace(TRACEERR,"SDDATA", sendlen, sbuf, sendlen);

    while (sendlen > 0) {
	if ((sendrtn = send(sck.gw_fd, sbuf, sendlen, 0)) == -1) {
	    switch (errno) {
	    case EINTR:
		continue;
	    case ECONNREFUSED:
	    case ENETDOWN:
	    case ECONNRESET:
	    case EPIPE:
	    case EMSGSIZE:
	    case ECONNABORTED:
	    case ENOBUFS:
		sprintf(msg.wkbuf, "%i", D_EID_22113);
		sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02562, KDDS02562), sck.gwname, strerror(errno), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_NETWORK);
	    default:
		sprintf(msg.wkbuf, "%i", D_EID_22113);
		sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02562, KDDS02562), sck.gwname, strerror(errno), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
	    }
	}
	sendlen -= sendrtn;
    }

    if (gwreturncode != 0) {
	shutdown(sck.gw_fd, 2);
	close(sck.gw_fd);
	agt_exit(D_EXIT_UNKNOWN);
    }

    long_to_char(&sbuf[0], D_AG_CONNEST);
    long_to_char(&sbuf[4], 8);
    switch (conn_type) {
    case D_GW_CON_CTRL:
	memcpy(&sbuf[8], D_PTYP_USRM, 4);
	break;
    case D_GW_CON_WCHD:
	memcpy(&sbuf[8], D_PTYP_WCHD, 4);
	break;
    case D_GW_CON_USER:
	memcpy(&sbuf[8], D_PTYP_USRC, 4);
	break;
    default:
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22114, conn_type, inet_ntoa(from->sin_addr.s_addr));
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }
    long_to_char(&sbuf[12], agt.ownpid);

    if (sendto(sck.ux_fd, sbuf, 16, 0, &sck.sysc_adr, sck.sysc_adrlen) != 16) {
	switch (errno) {
	case ENOBUFS:
	    sprintf(msg.wkbuf, "%i,%i,%s,%s", D_EID_22115, errno, strerror(errno), sck.sysc_adr.sun_path);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	    AgentMsg(LOG_WARNING, msg.buf);
	    agt_exit(D_EXIT_OUTOFRSC);
	default:
	    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_22116, errno, sck.sysc_adr.sun_path);
	    sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
    }

    AgentTrace(TRACEERR,"UXSEND", 16, sbuf, 16);


    switch (conn_type) {
    case D_GW_CON_CTRL:
    case D_GW_CON_USER:
	agt_usrc_main();


	sprintf(msg.wkbuf, "%i", D_EID_22117);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
	break;
    case D_GW_CON_WCHD:
	agt_wchd();


	sprintf(msg.wkbuf, "%i", D_EID_22118);
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
	break;
    default:
	sprintf(msg.buf, "%i,%i,%s", D_EID_22119, conn_type, inet_ntoa(from->sin_addr.s_addr));
	sprintf(msg.buf,  catgets(agt.catd, NL_SETD, MSG02031, KDDS02031), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
	break;
    }
}

/**************************************************************************/
/* ID	= agt_sysc_add_sysc						  */
/* name = Add SYSC information						  */
/* func = Add SYSC information at the bottom of SYSC table list		  */
/* i/o =								  */
/*	top :struct d_sysc **:pointer to the top of SYSC table list	  */
/*	last:struct d_sysc **:pointer to the bottom of SYSC table list	  */
/*	p   :struct d_sysc * :SYSC information				  */
/* return =	none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_sysc_add_sysc(struct d_sysc **top,
		       struct d_sysc **last, struct d_sysc *p)
{
    p->next = NULL;
    if (*top == NULL) {
	*top = p;
    } else {
	(*last)->next = p;
    }
    *last = p;
}


/**************************************************************************/
/* ID	= agt__sysc_get_sysc						  */
/* name = Get SYSC information						  */
/* func = Get SYSC information from SYSC table list using p key     	  */
/* i/o =                                                                  */
/*	top :struct d_sysc **:pointer to the top of SYSC table list	  */
/*	last:struct d_sysc **:pointer to the bottom of SYSC table list	  */
/*	p   :struct d_sysc * :SYSC information				  */
/* return =								  */
/*      wp:pointer to SYSC information					  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static struct d_sysc *agt__sysc_get_sysc(struct d_sysc **top,
				 struct d_sysc **last, struct d_sysc *p)
{
    struct d_sysc *oldp;
    struct d_sysc *wp;

    wp = *top;
    oldp = NULL;
    while ((wp != NULL) && (wp != p)) {
	oldp = wp;
	wp = wp->next;
    }

    if (wp != NULL) {
	if (wp == *top) {
	    *top = (*top)->next;
	} else if (oldp != NULL) {
	    oldp->next = wp->next;
	}
	if (wp == *last) {
	    *last = oldp;
	}
    }
    return wp;
}

/**************************************************************************/
/* ID	= agt__sysc_find_sysc_by_pid					  */
/* name = Search SYSC information					  */
/* func = Search SYSC information from SYSC table list using key  	  */
/* i/o =								  */
/*	top :struct d_sysc **:pointer to the top of SYSC table list	  */
/*  key :pid_t	    :IN:process-ID					  */
/* return =								  */   
/*      p   :pointer to SYSC information                                  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static struct d_sysc *agt__sysc_find_sysc_by_pid(struct d_sysc *top, pid_t key)
{
    struct d_sysc *p;

    for (p = top; (p != NULL) && (p->pid != key); p = p->next) {
    }

    return p;
}

/**************************************************************************/
/* ID	= agt__sysc_find_sysc_by_type					  */
/* name = Search SYSC information					  */
/* func = Search SYSC information from SYSC table list using key  	  */
/* i/o =                                                                  */
/*  top :struct d_sysc *:IN:pointer to the top of SYSC table list     	  */
/*	key :char *         :IN:process type				  */
/* return =								  */
/*      p   :pointer to SYSC information                                  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static struct d_sysc *agt__sysc_find_sysc_by_type(struct d_sysc *top,
						  char *key)
{
    struct d_sysc *p;

    for (p = top;
	 (p != NULL) && (memcmp(p->ptype, key, 4) != 0); p = p->next) {
    }

    return p;
}


/**************************************************************************/
/* ID   = agt__sysc_find_sysc_reconn					  */
/* name = Search SYSC information					  */
/* func = Search SYSC information from SYSC table list using con_no	  */
/* i/o =                                                                  */
/*  top :struct d_sysc *:IN:pointer to the top of SYSC table list     	  */
/*	con_no:long         :IN:connection number			  */
/*  addr  :long         :IN:gateway IP address			          */
/* return =								  */
/*      p   :pointer to SYSC information                                  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static struct d_sysc *agt__sysc_find_sysc_reconn(struct d_sysc *top,
						 long con_no, long addr)
{
    struct d_sysc *p;

    p = top;
    while ((p != NULL) &&
	   (p->conn_no < con_no)) {
	if ((p->ipaddr == addr) && (p->status < D_SYSC_ST_TERM)) {
	    break;
	}
	p = p->next;
    }

    return ((p == NULL) || (p->conn_no >= con_no)) ? NULL : p;
}


/**************************************************************************/
/* ID	= agt__sysc_find_sysc_current					  */
/* name = Search SYSC information					  */
/* func = Search SYSC information from SYSC table list using type and addr*/
/* i/o =                                                                  */
/*  top :struct d_sysc *:IN:pointer to the top of SYSC table list     	  */
/*	type:char *         :IN:process type				  */
/*      addr  :long     :IN:gateway IP address				  */
/* return =								  */
/*      p   :pointer to SYSC information                                  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static struct d_sysc *agt__sysc_find_sysc_current(struct d_sysc *top,
						  char *type, long addr)
{
    struct d_sysc *p;
    struct d_sysc *syscp;

    syscp = NULL;
    p = top;
    while (p != NULL) {
	if ((p->ipaddr == addr) &&
	    (memcmp(p->ptype, type, 4) == 0)) {
	    syscp = p;
	}
	p = p->next;
    }

    return syscp;
}

/**************************************************************************/
/* ID	= agt__sysc_get_another_SYSC					  */
/* name = Create SYSCOM table						  */
/* func = Create new SYSCOM table and include in table list		  */
/* io = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_get_another_SYSC(void)
{
    key_t key;
    int i;
    int num_newsysc;
    struct d_sysc *wsyscp;


    key = ftok(agt.ipcpath, D_ID_SHM_M2);
    if (key == -1) {
	return;
    }
    agt.shmid_m2 = shmget(key, D_SZ_SHM_M,
			  D_IPC_ACCESS_MODE|IPC_EXCL|IPC_CREAT);
    if (agt.shmid_m2 == -1) {
	int wk_shmid;

	switch (errno) {
	case EEXIST:
	    sprintf(msg.wkbuf, "%i,%08x", errno, key);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02521, KDDS02521), msg.wkbuf);
	    AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);

	    wk_shmid = shmget(key, D_SZ_SHM_M, D_IPC_ACCESS_MODE);
	    if (wk_shmid == -1) {
		return;
	    }
	    if (shmctl(wk_shmid, IPC_RMID, 0) == -1) {
		return;
	    }
	    agt.shmid_m2 = shmget(key, D_SZ_SHM_M,
				 D_IPC_ACCESS_MODE|IPC_EXCL|IPC_CREAT);
	    if (agt.shmid_m2 == -1) {
		return;
	    }
	    break;
	default:
	    return;
	}
    }

    wsyscp = (struct d_sysc *)shmat(agt.shmid_m2, (char *)0, 0);
    if (wsyscp == (struct d_sysc *)-1) {
	return;
    }

    agt.main->num_sysc_segments += 1;
    num_newsysc= D_SZ_SHM_M / sizeof(struct d_sysc);
    agt.main->num_sysc += num_newsysc;

    sprintf(msg.buf, "SYSC expanded size:%i,numSYSC:%i,ttlSYSC:%i",
	    D_SZ_SHM_M, num_newsysc, agt.main->num_sysc);
    AgentTrace(TRACEDEB,"RSCGET", strlen(msg.buf), msg.buf, 0);

    for(i = 0; i < num_newsysc; i++) {
	agt_sysc_add_sysc(&agt.main->frsysctop, &agt.main->frsysclst, wsyscp);
	wsyscp++;
    }
}

/**************************************************************************/
/* ID   = agt__sysc_find_sysc_by_ipadr_alive				  */
/* name = Search existing connections					  */
/* func = Check if connection key is not killed in the same gateway. If   */
/*        not, return sysc						  */
/* i/o =								  */
/*	top:struct d_sysc *:IN:pointer to the top of SYSC table list      */
/*  key:long           :IN:gateway IP address				  */
/* return = 								  */
/*      p   :pointer to SYSC information                                  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static struct d_sysc *agt__sysc_find_sysc_by_ipadr_alive(
					 struct d_sysc *top, long key)
{
    struct d_sysc *p;

    for (p = top; (p != NULL); p = p->next) {
	if ((p->ipaddr == key) && (p->status < D_SYSC_ST_TERM)) {
	    break;
	}
    }

    return p;
}

/**************************************************************************/
/* ID 	= agt__sysc_sig_usr1						  */
/* name = Execute when SIGUSR1 is transmitted				  */
/* func = Execute when SIGUSR1 is transmittedDExecute exit process	  */
/* i/o = none			   					  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_sig_usr1(int sig, int code, struct sigcontext *scp)
{
    agt_exit(D_EXIT_OK);
}

/**************************************************************************/
/* ID	= agt__sysc_abort_timeout					  */
/* name = Execute when timeout occurs					  */
/* func = Execute when time set by agent is outDExecute exit process     */
/* i/o = none			   					  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_abort_timeout(struct agt_timer *tp)
{

    agt_exit(D_EXIT_OK);

}

/**************************************************************************/
/* ID	= agt__sysc_to_hexstring					  */
/* name = Convert string						  */
/* func = convert byte string to hexdecimal strings			  */
/* i/o = 								  */
/*	buf  :char *:OUT:store data converted into hexa			  */
/*      bufsz:int   :IN :buf size					  */
/*      str  :char *:IN :string						  */
/*      len  :int   :IN :string length					  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__sysc_to_hexstring(char *buf, int bufsz, char *str, int len)
{
#define D_MAX_BYTES_TO_SHOW  32
    int bytes_conv;
    int i;
    char *dp = buf;
    char *sp = str;

    bytes_conv = (len <= D_MAX_BYTES_TO_SHOW) ?
      len : D_MAX_BYTES_TO_SHOW;
    bytes_conv = (bufsz <= bytes_conv) ? bufsz : bytes_conv;

    memset(buf, '\0', bufsz);
    for (i = 0; i < bytes_conv; i++, sp++, dp += 2) {
	if ((i < (bytes_conv - 1)) &&
	    ((i > 0) && ((i % 4) == 0))) {
	    if (bufsz < (dp - buf) + 1) {
		return;
	    }
	    sprintf(dp, " ");
	    dp++;
	}
	if (bufsz < (dp - buf) + 2) {
	    return;
	}
	sprintf(dp, "%02x", (unsigned char)*sp);
    }
}

