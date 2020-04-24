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
static char *RCSID_agt_main_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_main.c,v 1.1.2.2 1996/03/09 20:44:08 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_main.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:08  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:04  marty]
 *
 * Revision 1.1.2.1  1996/02/16  06:04:34  dce-sdc
 * 	First Submit
 * 	[1996/02/16  06:04:02  dce-sdc]
 * 
 * Revision 1.1.2.7  96/01/12  04:37:43  04:37:43  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:37:32  kishi]
 * 
 * Revision 1.1.2.6  1995/10/26  00:57:25  kishi
 * 	Change Copyright.
 * 	[1995/10/26  00:56:50  kishi]
 * 
 * Revision 1.1.2.5  1995/10/25  02:00:47  takano
 * 	Check uid.
 * 	[1995/10/25  02:00:28  takano]
 * 
 * Revision 1.1.2.4  1995/10/13  09:11:15  kishi
 * 	Add header comment.
 * 	[1995/10/13  09:11:03  kishi]
 * 
 * Revision 1.1.2.3  1995/09/23  09:52:22  kishi
 * 	Add header comment.
 * 	[1995/09/23  08:49:42  kishi]
 * 
 * Revision 1.1.2.2  1995/09/22  04:45:19  kishi
 * 	Cast NULL , value of nl_catd.
 * 	[1995/09/22  04:45:09  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:52:15  kishi
 * 	Initial revision
 * 	[1995/09/09  06:35:37  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#undef exit

#include <stdio.h>
#include <errno.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <locale.h>

#include "agt_agent.h"
#include "agt_errid.h"
#include "agt_main.h"
#include "agt_init.h"
#include "agt_parm.h"
#include "agt_msg.h"
#include "agt_ipc.h"
#include "agt_sck.h"
#include "agt_lib.h"
#include "agt_sig.h"

struct d_agt agt = {
   {'A', 'G', 'T', '*'},
   {' ', ' ', ' ', ' '},
   0,
   0,
   0,
   0,
   (struct d_maintbl *)NULL,
   {  {'\0'},
      0,
      0,},
   0,
   -1,
   -1,
   -1,
   (char *)NULL,
   (char *)NULL,
   "",
   (char *)NULL,
   -1,
   (struct catalog_descriptor *)-1,
   1
};

struct d_thread_info thread_info[D_THREAD_MAX] = {
  {{NULL, 0, 0}, -1},
  {{NULL, 0, 0}, -1},
  {{NULL, 0, 0}, -1}
 };

pthread_mutex_t sig_mutex;

pthread_key_t thr_id;

static pthread_once_t DfamOnceBlock = pthread_once_init;

struct d_local_main lmain = {
   D_FALSE
 };

static void agt__main_agent(void);
static void agt__main_sockmesg(char *buf, int len,
			       struct sockaddr_un *from, int fromlen);
static void agt__main_stop(char *buf, int len,
			   struct sockaddr_un *from, int fromlen);
static void agt__main_abort(char *buf, int len,
			    struct sockaddr_un *from, int fromlen);
static void agt__main_send(char *buf, int len,
			   struct sockaddr_un *from, int fromlen);
static void agt__main_reply_to_command(struct sockaddr_un *to, int tolen,
				       int rtncode, char *info, int msgtyp);
static void agt__main_tell_stop_trace(void);

#ifdef _DEBUG
static void reset_debug_loop_flag(int sig, int code, struct sigcontext *scp);

struct d_debug_info debug = {
    10,

    {0, 0},

    D_FALSE,
    D_TRUE,
    D_FALSE

};

#endif



/**************************************************************************/
/* ID	= main								  */
/* name = Main function of agent					  */
/* func = Create thread and initialize signal and agent			  */
/* i/o =								  */
/*	argc  :int   :IN:number of parameters needed to excute agent	  */
/*  argv[]:char *:content of parameters used during execution of agent	  */
/* return = 								  */
/*	1:normal end							  */
/*	0:abnormal end							  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int main(int argc, char *argv[])
{
    int function;
    int rtn;


if (getuid() != 0){
sprintf(msg.wkbuf, "%i",  D_EID_21122);
fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02008, KDDS02008), msg.wkbuf);
agt_exit(D_EXIT_PERMIT);
}

if (pthread_once(&DfamOnceBlock,
		     (pthread_initroutine_t)agt_init_once) == -1) {
	sprintf(msg.wkbuf, "%i",  D_EID_21120);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	agt_exit(D_EXIT_UNKNOWN);
    }

    thread_info[D_THREAD_MAIN].thread_id = D_THREAD_MAIN;
    thread_info[D_THREAD_MAIN].handle = pthread_self();

    rtn = pthread_mutex_lock(&sig_mutex);
    if (rtn == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_21121, errno);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	agt_abort();
    }


    agt.ownpid = getpid();
    agt.mainprocpid = agt.ownpid;
    memcpy(&agt.proctype, "main", 4);

    if (!setlocale(LC_ALL, "")) {
	fprintf(stderr, KDDS01107, strerror(errno));
	putenv("LANG=");
    } else {
	agt.catd = catopen(D_CAT_AGTFILE, 0);
    }


    agt_sig_init_signal_threads();

#ifdef _DEBUG
    {
	agt_sig_action(SIGUSR2, reset_debug_loop_flag);
    }
#endif

    agt_sig_dce_init();

    function = agt_parm_cmd_line_opt(argc, argv);

    agt_parm_config_file();
    agt_init_memory();
    agt_sem_open_semaphore();

    switch (function) {
    case D_FUNC_STARTAGT:
	agt__main_agent();
	break;
    case D_FUNC_STOPAGT:
	agt_sig_action(SIGHUP,agt_sig_ok_exit);
	agt_sig_action(SIGTERM,agt_sig_ok_exit);
	agt_cmd_stop();
	break;
    case D_FUNC_ABORTAGT:
	agt_sig_action(SIGHUP,agt_sig_ok_exit);
	agt_sig_action(SIGTERM,agt_sig_ok_exit);
	agt_cmd_abort();
	break;
    case D_FUNC_SNDMESG:
	agt_sig_action(SIGHUP,agt_sig_ok_exit);
	agt_sig_action(SIGTERM,agt_sig_ok_exit);
	agt_cmd_send();
	break;
    case D_FUNC_PRMEMTR:
	agt_sig_action(SIGHUP,agt_sig_ok_exit);
	agt_sig_action(SIGTERM,agt_sig_ok_exit);
	agt_cmd_prmemtr();
	break;
    default:
	sprintf(msg.wkbuf, "%i,%i",  D_EID_21101, function);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	agt_exit(D_EXIT_UNKNOWN);
	break;
    }
    agt_exit(D_EXIT_OK);
}


/**************************************************************************/
/* ID	= agt__main_agent						  */
/* name = Execution main process of agent				  */
/* func = Perform the following tasks:					  */
/*	  1. open syslog file						  */
/*	  2. allow process to execute in background			  */	
/*	  3. check execution of agent					  */
/*	  4. set signal process						  */
/*	  5. open UNIX domain socket					  */
/*	  6. check DFS environment					  */
/*	  7. initialize operation information file			  */
/*	  8. wait for command request					  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__main_agent(void)
{
    int nfds;
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    int num_readyfds;
    int continue_loop = D_TRUE;
    int savederr;
    int rtn;

    agt.status = D_MAN_ST_INIT;

    agt_init_syslog();
    agt_init_go_background();
    agt_init_check_agent();
    agt_init_setup_signals();
    agt_init_open_uxsock();
    agt_init_check_environment();


    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02000, KDDS02000));
    AgentMsg(LOG_INFO, msg.buf);

    agt_init_alloc_shmem();
    agt.main->agt_status = D_MAN_ST_INIT;

    agt_init_statfile();
    if (agt_init_create_processes() == 0) {
	agt_term_remove_statfile();
	agt_term_release_shmem();
	agt_term_close_unixsocket();
	agt_term_remove_semaphore();

	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02006, KDDS02006));
	AgentMsg(LOG_INFO, msg.buf);

	agt_term_close_syslog();

	agt_exit(D_EXIT_OK);
    }

    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02001, KDDS02001));
    AgentMsg(LOG_INFO, msg.buf);

    agt.status = D_MAN_ST_RUN;
    agt.main->agt_status = D_MAN_ST_RUN;
    nfds = sck.ux_fd + 1;

    while (continue_loop) {
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);
	FD_SET(sck.ux_fd, &readfds);

	rtn = pthread_mutex_unlock(&sig_mutex);
	if (rtn == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21115, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    agt_abort();
	}

	errno = 0;
	num_readyfds = select(nfds, (int *)&readfds,
			(int *)&writefds, (int *)&exceptfds, NULL);

	savederr = errno;

	rtn = pthread_mutex_lock(&sig_mutex);
	if (rtn == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21116, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    agt_abort();
	}

	sprintf(msg.buf, "SELECT %i,err:%i", num_readyfds, errno);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);

	errno = savederr;
        switch (num_readyfds) {
	case 0:
	    sprintf(msg.wkbuf, "%i", D_EID_21102);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    continue;
	case -1:
            switch (errno) {
	    case EINTR:
		break;
            default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_21103, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	    continue;
	case 1:
	    memset((char *)&sck.from, '\0', sizeof(sck.from));
	    sck.fromlen = sizeof(sck.from);
	    sck.uxrcvlen = recvfrom(sck.ux_fd, sck.uxrcvdata, D_UXMSG_MAX, 0,
			      &sck.from, &sck.fromlen);
	    if (sck.uxrcvlen == -1) {
                switch (errno) {
	        case EINTR:
	            continue;
                default:
		    sprintf(msg.wkbuf, "%i,%i", D_EID_21104, errno);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_abort();
	        }
	    } else {

		AgentTrace(TRACEERR,"UXRECV", sck.uxrcvlen, sck.uxrcvdata, sck.uxrcvlen);

		if (sck.uxrcvlen < 8) {
		    sprintf(msg.wkbuf, "%i,%i", D_EID_21106, sck.uxrcvlen);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
		}
		agt__main_sockmesg(sck.uxrcvdata, sck.uxrcvlen,
				   &sck.from, sck.fromlen);
	    }
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21105, num_readyfds);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
        }
    }

    agt_term_remove_statfile();
    agt_term_release_shmem();
    agt_term_close_unixsocket();
    agt_term_remove_semaphore();

    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02004, KDDS02004));
    AgentMsg(LOG_INFO, msg.buf);

    agt_term_close_syslog();

    agt_exit(D_EXIT_OK);
}


/**************************************************************************/
/* ID	= agt__main_sockmesg						  */
/* name = Command analyzer						  */
/* func = Message from the command is analyzed and executed. The message  */
/*        maybe an instruction to stop main process, to abort execution,  */
/*        or to send message to gateway					  */
/* i/o =								  */
/*	buf    :char *	  :IN:message information			  */
/*  len    :int		  :IN:length of message information		  */
/*  from   :struct sockaddr_un:IN:message address			  */
/*  fromlen:int		  :IN:number of bytes pointed by from		  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__main_sockmesg(char *buf, int len,
			       struct sockaddr_un *from, int fromlen)
{
    long cmd;
    long checkpid;

    from->sun_path[fromlen-sizeof(from->sun_family)] = '\0';

    cmd = char_to_num(buf, 4);

    switch (cmd) {
    case D_AG_CSTOP:

	checkpid = char_to_num((buf+8),4);
	sprintf(from->sun_path,"%s%05i",D_DFLT_IPCPATH,checkpid);
	fromlen = sizeof(from->sun_family)+strlen(from->sun_path) + 1;
        agt__main_stop(buf, len, from, fromlen);
	break;
    case D_AG_CABORT:

	checkpid = char_to_num((buf+8),4);
	sprintf(from->sun_path,"%s%05i",D_DFLT_IPCPATH,checkpid);
	fromlen = sizeof(from->sun_family)+strlen(from->sun_path) + 1;
	agt__main_abort(buf, len, from, fromlen);
	break;
    case D_AG_CMSG:

	checkpid = char_to_num((buf+8),4);
	sprintf(from->sun_path,"%s%05i",D_DFLT_IPCPATH,checkpid);
	fromlen = sizeof(from->sun_family)+strlen(from->sun_path) + 1;
	agt__main_send(buf, len, from, fromlen);
	break;
    default:
	sprintf(msg.wkbuf, "%i,%i", D_EID_21107, cmd);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	break;
    }
}


/**************************************************************************/
/* ID	= agt__main_send						  */
/* name = Send message to system communication process			  */
/* func = The message from command is sent to system communication process*/
/* i/o =								  */
/*  buf    :char *            :IN:message information			  */
/*  len    :int               :IN:length of message inforamtion		  */
/*  from   :struct sockaddr_un:IN:command address	                  */
/*  fromlen:int               :IN:number of bytes pointed by from         */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__main_send(char *buf, int len,
			   struct sockaddr_un *from, int fromlen)
{
    char wkbuf[8];

    memcpy(wkbuf,buf,8);
    buf += 4;
    memcpy(buf,wkbuf,8);
    len -= 4;


    long_to_char(buf, D_AG_MSG);

send_sendto_retry:
    if (sendto(sck.ux_fd, buf, len, 0,
	       &sck.sysc_adr, sck.sysc_adrlen) != len) {
	switch (errno) {
	case EINTR:
	    goto send_sendto_retry;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21108, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
    }

    AgentTrace(TRACEERR,"UXSEND", len, buf, len);

    agt__main_reply_to_command(from, fromlen,
			       D_AG_RT_OK, "reply", D_AG_CMSG);
}


/**************************************************************************/
/* ID	= agt__main_stop						  */
/* name = Send request to stop system communication process		  */
/* func = Send request from command to stop system communication process  */
/* i/o =								  */
/*  buf    :char *            :IN:message information			  */
/*  len    :int               :IN:length of message inforamtion		  */
/*  from   :struct sockaddr_un:IN:command address	                  */
/*  fromlen:int               :IN:number of bytes pointed by from         */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__main_stop(char *buf, int len,
			   struct sockaddr_un *from, int fromlen)
{
    char wkbuf[8];

    if ((len != 16) || (char_to_num(&buf[4], 4) != 4))  {
	agt__main_reply_to_command(from, fromlen, D_AG_RT_EPRM,
				   "reply", D_AG_CSTOP);
    } else {
	if (agt.main->agt_status == D_MAN_ST_STOP) {
	    agt__main_reply_to_command(from, fromlen, D_AG_RT_STOP,
				       "reply", D_AG_CSTOP);
	} else if (agt.main->agt_status == D_MAN_ST_ABRT) {
	    agt__main_reply_to_command(from, fromlen, D_AG_RT_ABORT,
				       "reply", D_AG_CSTOP);
	} else {
	    memcpy(wkbuf,buf,8);
	    buf += 4;
	    memcpy(buf,wkbuf,8);
	    len -= 4;

	    long_to_char(buf, D_AG_STOP);

stop_sendto_retry:
	    if (sendto(sck.ux_fd, buf, len, 0, &sck.sysc_adr,
		       sck.sysc_adrlen) != len) {
		switch (errno) {
		case EINTR:
		    goto stop_sendto_retry;
		default:
		    sprintf(msg.wkbuf, "%i,%i", D_EID_21109, errno);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_abort();
		}
	    }

	    AgentTrace(TRACEERR,"UXSEND", len, buf, len);

	    agt__main_reply_to_command(from, fromlen, D_AG_RT_OK,
				       "reply", D_AG_CSTOP);
	    agt.status = D_MAN_ST_STOP;
	    agt.main->agt_status = D_MAN_ST_STOP;
	}
    }
}


/**************************************************************************/
/* ID	= agt__main_abort						  */
/* name = Send request to abort system communication process		  */
/* func = Send request from command to abort system communication process */
/* i/o =								  */
/*  buf    :char *            :IN:message information			  */
/*  len    :int               :IN:length of message inforamtion		  */
/*  from   :struct sockaddr_un:IN:command address	                  */
/*  fromlen:int               :IN:number of bytes pointed by from         */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__main_abort(char *buf, int len,
			    struct sockaddr_un *from, int fromlen)
{
    int msglen;
    char wkbuf[8];

    if ((len != 12) || (char_to_num(&buf[4], 4) != 0))  {
	agt__main_reply_to_command(from, fromlen, D_AG_RT_EPRM,
				   "reply", D_AG_CABORT);
    } else {
	if (agt.main->agt_status == D_MAN_ST_ABRT) {
	    agt__main_reply_to_command(from, fromlen, D_AG_RT_ABORT,
				       "reply", D_AG_CABORT);
	} else {
	    memcpy(wkbuf,buf,8);
	    buf+=4;
	    memcpy(buf,wkbuf,8);
	    len -= 4;

	    long_to_char(&buf[0], D_AG_ABORT);
	    long_to_char(&buf[4], 4);
	    long_to_char(&buf[len], D_AG_ABORT_CMD);

	    msglen = len + 4;

abort_sendto_retry:
	    if (sendto(sck.ux_fd, buf, msglen, 0, &sck.sysc_adr,
		       sck.sysc_adrlen) != msglen) {
		switch (errno) {
		case EINTR:
		    goto abort_sendto_retry;
		default:
		    sprintf(msg.wkbuf, "%i,%i", D_EID_21110, errno);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_abort();
		}
	    }

	    AgentTrace(TRACEERR,"UXSEND", msglen, buf, msglen);

	    agt__main_reply_to_command(from, fromlen, D_AG_RT_OK,
				       "reply", D_AG_CABORT);
	    agt.status = D_MAN_ST_ABRT;
	    agt.main->agt_status = D_MAN_ST_ABRT;
	}
    }
}




/**************************************************************************/
/* ID	= agt__main_reply_to_command					  */
/* name = Send result to command process				  */
/* func = Send back the result of the execution requested by the command  */
/*        process							  */
/* i/o =								  */
/*	to     :struct sockaddr_un *:IN:address of command process address*/
/*  tolen  :int		  :IN:number of bytes of the area to store address*/
/*  rtncode:int		  :IN:result					  */
/*  info   :char *      :IN:inforamtion to write in the log file	  */	
/*  msgtyp :int		    :IN:message type				  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__main_reply_to_command(struct sockaddr_un *to, int tolen,
				       int rtncode, char *info, int msgtyp)
{
#define SBUF_SIZE 12
    char sbuf[SBUF_SIZE];

    long_to_char(&sbuf[0], D_AG_COMRTN);
    long_to_char(&sbuf[4], 4);
    long_to_char(&sbuf[8], rtncode);

reply_sendto_retry:
    if (sendto(sck.ux_fd, sbuf, SBUF_SIZE, 0, to, tolen)
	!= SBUF_SIZE) {
	switch (errno) {
	case EINTR:
	    goto reply_sendto_retry;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i,%s,%i", D_EID_21111, errno, rtncode, info, msgtyp);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
    }

    AgentTrace(TRACEERR,"UXSEND", SBUF_SIZE, sbuf, SBUF_SIZE);

}

/**************************************************************************/
/* ID	= agt_main_sig_term						  */
/* name = SIGTERM execution routine					  */
/* func = This is the routine to be called  when receiving SIGTERM signal.*/
/*        Send request to system communication process to abort		  */
/* i/o =								  */
/*	sig :int	        :IN:signal number			  */
/*      code:int		:IN:status code				  */
/*      scp :struct sigcontext *:IN:signal information			  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_main_sig_term(int sig, int code, struct sigcontext *scp)
{
#define SBUF_SIZE 12
    char sbuf[SBUF_SIZE];


#ifdef _DEBUG
    sprintf(msg.buf, "SIGTERM");
    AgentTrace(TRACEDEB,"SIGNAL", strlen(msg.buf), msg.buf, 0);
#endif

    long_to_char(sbuf, D_AG_ABORT);
    long_to_char(&sbuf[4], 4);
    long_to_char(&sbuf[8], D_AG_ABORT_TERM);

sigterm_sendto_retry:
    if (sendto(sck.ux_fd, sbuf, SBUF_SIZE, 0, &sck.sysc_adr,
	       sck.sysc_adrlen) != SBUF_SIZE) {
	switch (errno) {
	case EINTR:
	    goto sigterm_sendto_retry;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21114, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
    }

    AgentTrace(TRACEERR,"UXSEND", SBUF_SIZE, sbuf, SBUF_SIZE);

    agt.status = D_MAN_ST_ABRT;
    agt.main->agt_status = D_MAN_ST_ABRT;
}


/**************************************************************************/
/* ID	= agt_main_sig_chld						  */
/* name = SIGCLD execution routine					  */
/* func = This is the routine to be called  when receiving SIGCLD signal. */
/*        Send request to system communication process to abort		  */
/* i/o =								  */
/*	sig :int	        :IN:signal number			  */
/*      code:int		:IN:status code				  */
/*      scp :struct sigcontext *:IN:signal information			  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_main_sig_chld(int sig, int code, struct sigcontext *scp)
{
    pid_t dpid;
    int dstatus;
    int savederr;
    struct timeval time_signaled;
    char timebuf[32];
    struct tm *loctime;
    time_t caltime;
    int timelen;

    while (1) {
	errno = 0;
	dstatus = 0;
	dpid = waitpid(-1, &dstatus, WNOHANG);
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
		sprintf(msg.wkbuf, "%i,%i", D_EID_21117, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	    break;
	default:

	    if (gettimeofday(&time_signaled,
			     (struct timezone *)NULL) == -1) {
		sprintf(msg.wkbuf, "%i,%i", D_EID_21118, dpid);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
	    }

	    caltime = time_signaled.tv_sec;
	    loctime = localtime(&caltime);
	    timelen = strftime(timebuf, 31, "%Y/%m/%d %H:%M:%S", loctime);
	    sprintf(msg.buf, "SIGCHLD pid:%i,%s.%06i,st:0x%x", dpid, timebuf, time_signaled.tv_usec, dstatus);

#ifdef _DEBUG
	    if (WIFSIGNALED(dstatus)) {
		sprintf(msg.buf, "****: %i killed by SIGNAL:%i\n", dpid, WTERMSIG(dstatus));
		AgentMsg(LOG_DEBUG, msg.buf);

		if ((access(DEBUG_SCRIPT_FILE_M, X_OK) == 0) &&
		    (fork() == 0)) {
		    char cmdbuf[200];
		    sprintf(cmdbuf, "%s %s %i %i %s\n", DEBUG_SCRIPT_FILE_M, agt.agenthostname, WTERMSIG(dstatus), dpid, agt.agent_id);
		    system(cmdbuf);
		    agt_exit(0);
		}
	    }
#endif

	    if (dpid == agt.main->tracpid) {
		AgentTrace(TRACEERR,"SIGNAL", strlen(msg.buf), msg.buf, 0);
		agt.main->tracpid = 0;
		agt.main->num_proc--;
		agt.main->evtr_files = -1;
		trc.evttr_on = D_FALSE;
		if ((agt.main->agt_status < D_MAN_ST_STOP) ||
		    (agt.main->syscpid != 0)) {


		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02300, KDDS02300), dstatus);
		    AgentMsg(LOG_WARNING, msg.buf);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02100, KDDS02100));
		    AgentMsg(LOG_WARNING, msg.buf);
		} else {
		    ;
		}
		agt_stat_delete_entry(agt.statfilefd, dpid);
	    } else if (dpid == agt.main->syscpid) {
		AgentTrace(TRACEERR,"SIGNAL", strlen(msg.buf), msg.buf, 0);
		agt.main->syscpid = 0;
		agt.main->num_proc--;
		if (agt.main->agt_status >= D_MAN_ST_STOP) {
		    ;
		} else {
		    if ((dstatus == 0) && (agt.status == D_MAN_ST_INIT)) {

			agt_exit(D_EXIT_OK);
		    } else {
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02301, KDDS02301), dstatus);
			AgentMsg(LOG_ERR, msg.buf);
			sprintf(msg.wkbuf, "%i", D_EID_21112);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
			agt_abort();
		    }
		}
		agt_stat_delete_entry(agt.statfilefd, dpid);


		if (agt.main->tracpid != 0) {
		    agt__main_tell_stop_trace();
		}
	    } else {
		;
	    }
	    break;
	}
    }

wait_done:
    if (agt.main->num_proc == 0) {

	agt_term_remove_statfile();
	agt_term_release_shmem();
	agt_term_close_unixsocket();
	agt_term_remove_semaphore();

	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02004, KDDS02004));
	AgentMsg(LOG_INFO, msg.buf);

	agt_term_close_syslog();

	agt_exit(D_EXIT_OK);
    }
}

/**************************************************************************/
/* ID	= agt__main_tell_stop_trace					  */
/* name = Send request to stop trace					  */
/* func = Send request to the trace process to stop			  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__main_tell_stop_trace(void)
{
    char *bp;

    bp = sck.uxsnddata;

    long_to_char(bp, D_AG_STPTRC);
    bp += 4;

    long_to_char(bp, 0);

    sck.uxsndlen = 8;

stoptrace_sendto_retry:
    if (sendto(sck.ux_fd, sck.uxsnddata, sck.uxsndlen, 0,
	       &sck.trace_adr, sck.trace_adrlen) != sck.uxsndlen) {
	switch (errno) {
	case EINTR:
	    goto stoptrace_sendto_retry;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21119, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	}
    }

    AgentTrace(TRACEERR,"UXSEND", sck.uxsndlen, sck.uxsnddata, sck.uxsndlen);

}


#ifdef _DEBUG

/*****************************************************************************/
/* ID	= reset_debug_loop_flag						     */
/*                                                                           */
/* name = Debug indicator setting function                                   */
/* func = Sets the debug indicator when the signal, SIGUSR2, is received.    */
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
static void reset_debug_loop_flag(int sig, int code, struct sigcontext *scp)
{
    debug.debug_loop = D_FALSE;
}

/*****************************************************************************/
/* ID	= check_debug_loop						     */
/*                                                                           */
/* name = Checking the debug loop indicator                                  */
/* func = With checking the value of debug.debug_loop, determines whether    */
/*        to continue the loop or not.                                       */
/*                                                                           */
/* i/o    = none                                                             */
/*                                                                           */
/* return = D_TRUE : the loop continues                                      */
/*          D_FALSE: the loop is ended, or Agent is not started with a       */
/*                   debug mode                                              */
/* note   = none                                                             */
/* date   = October 12, 1995  by Tetsuya Ichikawa                            */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
int check_debug_loop(void)
{
    int old_debug_loop;

    if (!debug.waitsignal) {
	return D_FALSE;
    } else {
	old_debug_loop = debug.debug_loop;
	debug.debug_loop = D_TRUE;

	return (old_debug_loop == D_TRUE);
    }
}

/*****************************************************************************/
/* ID	= debug_forever							     */
/*                                                                           */
/* name = An eternal loop with using "sleep"                                 */
/* func = Gets into an eternal loop if Agent is started with -noexit option. */
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
void debug_forever(void)
{
    if (debug.noexit) {
	for (;;) {
	    agt_sleep(D_UNIT_MB);
#ifdef _DEBUG
	    sprintf(msg.buf, "****: sleep returned on process (%i,%c%c%c%c,errno:%i).\n", getpid(), agt.proctype[0], agt.proctype[1], agt.proctype[2], agt.proctype[3], errno);
	    AgentMsg(LOG_DEBUG, msg.buf);
#endif
	}
    }
}


/*****************************************************************************/
/* ID	= debug_wait_signal						     */
/*                                                                           */
/* name = Periodic checking of the debug indicator                           */
/* func = With the interval designated by debug.loop_interval, invokes       */
/*        check_debug_loop to check the debug indicator.                     */
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
void debug_wait_signal(void)
{
    if (debug.waitsignal) {
		;;
#ifdef _DEBUG
	sprintf(msg.buf, "****: process (%i,%c%c%c%c) is waiting for SIGUSR2, type' kill -USR2 %i' to continue.\n", getpid(), agt.proctype[0], agt.proctype[1], agt.proctype[2], agt.proctype[3], getpid());
	AgentMsg(LOG_DEBUG, msg.buf);
#endif
    }

    while(check_debug_loop()) {

	debug.tmv.tv_sec = debug.loop_interval;
	debug.tmv.tv_usec = 0;

	select(0, 0, 0, 0, &debug.tmv);
    }
}



#endif
