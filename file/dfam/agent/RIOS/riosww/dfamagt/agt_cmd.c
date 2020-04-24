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
static char *RCSID_agt_cmd_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_cmd.c,v 1.1.2.2 1996/03/11 14:05:53 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_cmd.c,v $
 * Revision 1.1.2.2  1996/03/11  14:05:53  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:41  marty]
 *
 * Revision 1.1.2.1  1996/02/19  10:18:11  dce-sdc
 * 	First Submit
 * 	[1996/02/19  10:17:48  dce-sdc]
 * 
 * Revision 1.1.2.4  96/01/12  04:28:50  04:28:50  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:28:41  kishi]
 * 
 * Revision 1.1.2.3  1995/10/26  00:34:58  kishi
 * 	Change Copyright.
 * 	[1995/10/26  00:34:46  kishi]
 * 
 * Revision 1.1.2.2  1995/09/23  09:54:04  kishi
 * 	Add header comment.
 * 	[1995/09/23  06:21:11  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:54:08  kishi
 * 	Initial revision
 * 	[1995/09/09  06:16:10  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#undef exit

#include <stdio.h>
#include <errno.h>
#include <syslog.h>
#include <sys/types.h>

#include "agt_agent.h"
#include "agt_errid.h"
#include "agt_cmd.h"
#include "agt_ipc.h"
#include "agt_sck.h"
#include "agt_parm.h"
#include "agt_msg.h"
#include "agt_sem.h"

#ifdef _DEBUG
  #include <syslog.h>
  #include <sys/ipc.h>
  #include "agt_main.h"
  #include "agt_trc.h"
#endif

static struct sockaddr_un myaddr;

struct d_cmd cmd = {
    D_DFLT_STOP_TIME,
    NULL,
    NULL,
    0
  };



static void agt__cmd_open_sock(void);
static void agt__cmd_send_to_agent(char *mbuf, int mlen);
static void agt__cmd_close_sock(void);
static int agt__cmd_check_reply_from_agent(void);
static int agt__cmd_build_req_send(char *bp, int *destp);
static int agt__cmd_build_req_stop(char *bp, int waittime);
static int agt__cmd_build_req_abort(char *bp);
static void agt__cmd_exit(int exstatus);



/**************************************************************************/
/* ID   = agt_cmd_stop						          */	
/* name = Sending stop  message instruction to main process		  */
/* func = If the Agent is running, the stop instruction is composed       */
/*        and sent to the main process                                    */
/* i/o  = none                                                            */
/* return = none                                                          */
/* note = none                                    			  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_cmd_stop(void)
{
    char mbuf[D_UXMSG_MAX];
    int mlen;
    long rtncode;

    if (agt_sem_check_if_agent_is_up(agt.semid) == 0) {
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01102, KDDS01102));
	agt__cmd_exit(D_EXIT_NOAGENT);
    }

    agt__cmd_open_sock();
    mlen = agt__cmd_build_req_stop(mbuf, cmd.stoptime);
    agt__cmd_send_to_agent(mbuf, mlen);
    switch (rtncode = agt__cmd_check_reply_from_agent()) {
    case D_AG_RT_OK:
	break;
    case D_AG_RT_INIT:
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01103, KDDS01103));
	agt__cmd_exit(D_EXIT_INIT);
	break;
    case D_AG_RT_STOP:
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01104, KDDS01104));
	agt__cmd_exit(D_EXIT_INPROG);
	break;
    case D_AG_RT_ABORT:
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01105, KDDS01105));
	agt__cmd_exit(D_EXIT_INPROG);
	break;
    default:
	sprintf(msg.wkbuf, "%i,%i", D_EID_10001, rtncode);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01106, KDDS01106),
		msg.wkbuf);
	agt__cmd_exit(D_EXIT_UNKNOWN);
	break;
    }

    agt__cmd_close_sock();

    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01100, KDDS01100));

    agt_sem_wait_agent_gets_down(agt.semid);

    agt__cmd_exit(D_EXIT_OK);
}


/**************************************************************************/
/* ID   = agt_cmd_abort							  */
/* name = Sending abort message instruction to main process		  */
/* func = If the Agent is running, the abort   instruction is composed    */
/*        and sent to the main process                                    */
/* i/o  = none                                                            */
/* return = none                                                          */
/* note = none                                    			  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_cmd_abort(void)
{
    char mbuf[D_UXMSG_MAX];
    int mlen;
    long rtncode;

    if (agt_sem_check_if_agent_is_up(agt.semid) == 0) {
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01102, KDDS01102));
	agt__cmd_exit(D_EXIT_NOAGENT);
    }

    agt__cmd_open_sock();
    mlen = agt__cmd_build_req_abort(mbuf);
    agt__cmd_send_to_agent(mbuf, mlen);
    switch (rtncode = agt__cmd_check_reply_from_agent()) {
    case D_AG_RT_OK:
	break;
    case D_AG_RT_INIT:
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01103, KDDS01103));
	agt__cmd_exit(D_EXIT_INIT);
	break;
    case D_AG_RT_ABORT:
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01105, KDDS01105));
	agt__cmd_exit(D_EXIT_INPROG);
	break;
    default:
	sprintf(msg.wkbuf, "%i,%i", D_EID_10002, rtncode);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01106, KDDS01106),
		msg.wkbuf);
	agt__cmd_exit(D_EXIT_UNKNOWN);
	break;
    }

    agt__cmd_close_sock();

    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01101, KDDS01101));

    agt_sem_wait_agent_gets_down(agt.semid);

    agt__cmd_exit(D_EXIT_OK);
}


/**************************************************************************/
/* ID   = agt_cmd_send							  */
/* name = Sending message to main process                                 */
/* func = If the Agent is running, messages are composed and sent to the  */
/*        main process                                                    */
/* i/o  = none                                                            */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_cmd_send(void)
{
    char mbuf[D_UXMSG_MAX];
    int mlen;
    long rtncode;
    int destpointer;

    if (agt_sem_check_if_agent_is_up(agt.semid) == 0) {
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01102, KDDS01102));
	agt__cmd_exit(D_EXIT_NOAGENT);
    }

    agt__cmd_open_sock();

    destpointer = 0;
    while ((mlen = agt__cmd_build_req_send(mbuf, &destpointer)) > 0) {

#ifdef _DEBUG_
    sprintf(msg.buf, "****: SENDMSG cmd(%i) sending to main (len:%i).\n", getpid(), mlen);
    fprintf(stderr, msg.buf);
    AgentMsg(LOG_DEBUG, msg.buf);
#endif

	agt__cmd_send_to_agent(mbuf, mlen);
	switch (rtncode = agt__cmd_check_reply_from_agent()) {
	case D_AG_RT_OK:
	    break;
	case D_AG_RT_INIT:
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01103, KDDS01103));
	    agt__cmd_exit(D_EXIT_INIT);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_10003, rtncode);
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01106, KDDS01106),
		    msg.wkbuf);
	    agt__cmd_exit(D_EXIT_UNKNOWN);
	    break;
	}
    }

    agt__cmd_close_sock();

    agt__cmd_exit(D_EXIT_OK);
}



/**************************************************************************/
/* ID	= agt__cmd_open_sock						  */
/* name = Opening unix domain socket 					  */
/* func = Creates the password name to be used during binding and calls   */
/*        agt_sck_open_unixsocket fucntion to really open the socket      */
/* i/o  = none                                                            */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__cmd_open_sock(void)
{
    sprintf(myaddr.sun_path, "%s/.%s%05i", D_UXSOC_CMD_PATH,
	    D_UXSOC_CMND, agt.ownpid);
    sck.ux_fd = agt_sck_open_unixsocket(myaddr.sun_path);
}


/**************************************************************************/
/* ID   = agt__cmd_send_to_agent					  */
/* name = Sending message to main process                                 */
/* func = The fucntion sends command messages to main process             */
/* i/o  =                                                                 */
/*	    mbuf:char *: message information                              */
/*      mlen:int   : message length                                       */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__cmd_send_to_agent(char *mbuf, int mlen)
{
    if (sendto(sck.ux_fd, mbuf, mlen, 0, &sck.main_adr, sck.main_adrlen)
	!= mlen) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_10004, errno);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01106, KDDS01106),
		msg.wkbuf);
	agt__cmd_exit(D_EXIT_UNKNOWN);
    }
}


/**************************************************************************/
/* ID 	= agt__cmd_close_sock				                  */
/* name = Closing socket                        			  */
/* func = The fucntion closes the socket and releases addresses           */
/* i/o  = none                                                            */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__cmd_close_sock(void)
{
    if (shutdown(sck.ux_fd, 2) == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_10005, errno);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01106, KDDS01106),
		msg.wkbuf);
	agt__cmd_exit(D_EXIT_UNKNOWN);
    }

    close(sck.ux_fd);

    if (unlink(myaddr.sun_path) == -1) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_10006, errno, myaddr.sun_path);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01106, KDDS01106),
		msg.wkbuf);
	agt__cmd_exit(D_EXIT_UNKNOWN);
    }

    free(sck.myaddr);
}


/**************************************************************************/
/* ID 	= agt__cmd_check_reply_from_agent				  */
/* name = Releasing transmission information of main process              */
/* func = The function checks the content of the message received from    */
/*        main process and if it fails, exit                              */
/* i/o  = none                                                            */
/* return = result information from main process                          */ 
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__cmd_check_reply_from_agent(void)
{
    struct sockaddr_un remote;
    int remotelen;
    char rbuf[D_UXMSG_REPLY_MAX];
    int rlen;
    int msgtype;
    int msglen;

recv_loop:
    memset((char *)&remote, '\0', sizeof(remote));
    remotelen = sizeof(remote);
    rlen = recvfrom(sck.ux_fd, rbuf, D_UXMSG_REPLY_MAX,
		    0, &remote, &remotelen);

    if (rlen == -1) {
	switch (errno) {
	case EINTR:
	    goto recv_loop;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_10007, errno);
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01106, KDDS01106),
		    msg.wkbuf);
	    agt__cmd_exit(D_EXIT_UNKNOWN);
	    break;
	}
    }

    remote.sun_path[remotelen- sizeof(remote.sun_family)] = '\0';

#ifdef _DEBUG_
    sprintf(msg.buf, "****: CMD (%i) got reply from %s(fromlen:%i,len:%i).\n", getpid(), remote.sun_path, remotelen, rlen);
    fprintf(stderr, msg.buf);
    AgentMsg(LOG_DEBUG, msg.buf);
#endif

    if (rlen != D_UXMSG_REPLY_MAX) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_10008, rlen);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01106, KDDS01106),
		msg.wkbuf);
	agt__cmd_exit(D_EXIT_UNKNOWN);
    }

    switch (msgtype = char_to_num(&rbuf[0], 4)) {
    case D_AG_COMRTN:
	if (msglen = char_to_num(&rbuf[4], 4) != 4) {
	    remote.sun_path[rlen] = '\0';
	    sprintf(msg.wkbuf, "%i,%i,%i,%s",
		    D_EID_10009, msglen, msgtype, remote.sun_path);
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01106, KDDS01106),
		    msg.wkbuf);
	    agt__cmd_exit(D_EXIT_UNKNOWN);
	}
	break;
    default:
	remote.sun_path[rlen] = '\0';
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_10010, msgtype, remote.sun_path);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG01106, KDDS01106),
		msg.wkbuf);
	agt__cmd_exit(D_EXIT_UNKNOWN);
	break;
    }

    return char_to_num(&rbuf[8], 4);
}


/**************************************************************************/
/* ID 	= agt__cmd_build_req_send					  */
/* name = Composing message to be sent to main process                    */
/* func = The fucntion stores information about message to be sent to main*/
/*        process in a array of string of characters                      */
/* i/o  =                                                                 */
/*	bp   :char *:OUT: message information                             */
/*	destp:int * :OUT: message composing flag                          */
/* return = message length                                                */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int build_req_send_destp;
static int build_req_send_msgsize;
static int build_req_send_destmax;

static int agt__cmd_build_req_send(char *bp, int *destp)
{
    int destnum;
    int extmsglen;
    int totallen;
    int i;
    int j;

    if (*destp == 0) {
	build_req_send_destp = 0;
	build_req_send_msgsize = strlen(cmd.mesg) + 1;
	build_req_send_destmax =
	  (D_UXMSG_MAX - 8 - 16 - build_req_send_msgsize)/4;

    }

    if (build_req_send_destp >= cmd.destnum) {
	return 0;
    }

    destnum = (cmd.destnum < build_req_send_destmax) ?
               cmd.destnum : build_req_send_destmax;

    extmsglen = 4*4 + 4*destnum + build_req_send_msgsize;

    totallen = extmsglen + 4*3;

    long_to_char(bp, D_AG_CMSG);
    bp += 4;

    long_to_char(bp, extmsglen);
    bp += 4;

    long_to_char(bp,agt.ownpid);
    bp += 4;

    long_to_char(bp, D_GW_MSG_CNSL);
    bp += 4;

    long_to_char(bp, 0);
    bp += 4;

    long_to_char(bp, build_req_send_msgsize);
    bp += 4;

    memcpy(bp, cmd.mesg, build_req_send_msgsize);
    bp += build_req_send_msgsize;

    long_to_char(bp, destnum);
    bp += 4;

    for (i = build_req_send_destp, j = destnum; j > 0; i++, j--) {
	long_to_char(bp, cmd.dest[i]);
	bp += 4;
    }

    build_req_send_destp += destnum;
    *destp = build_req_send_destp;

    return totallen;
}



/**************************************************************************/
/* ID 	= agt__cmd_build_req_stop					  */
/* name = Composing stop request message                                  */
/* func = The function stores the message requested to stop the Agent in  */
/*        a array of string of characters                                 */
/* i/o  =                                                                 */
/*	bp      :char *:OUT: message information                          */
/*	waittime:int   :IN : waiting time until stop                      */
/* return = message length                                                */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__cmd_build_req_stop(char *bp, int waittime)
{
    int extmsglen;
    int totallen;

    extmsglen = 4;
    totallen = extmsglen + 4*3;

    long_to_char(bp, D_AG_CSTOP);
    bp += 4;

    long_to_char(bp, extmsglen);
    bp += 4;

    long_to_char(bp,agt.ownpid);
    bp += 4;

    long_to_char(bp, waittime);

    return totallen;
}


/**************************************************************************/
/* ID	= agt__cmd_build_req_abort					  */
/* name = Composing abort request message                                 */
/* func = The function stores the message requested to abort the Agent in */
/*        a array of string of characters                                 */
/* i/o  =                                                                 */
/*	bp      :char *:OUT: message information                          */
/* return = message length                                                */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__cmd_build_req_abort(char *bp)
{
    int extmsglen;
    int totallen;

    extmsglen = 0;
    totallen = extmsglen + 4*3;

    long_to_char(bp, D_AG_CABORT);
    bp += 4;

    long_to_char(bp, extmsglen);
    bp += 4;

    long_to_char(bp,agt.ownpid);
    bp += 4;

    return totallen;
}


/**************************************************************************/
/* ID   = agt_cmd_prmemtr						  */
/* name = Exiting command processing                                      */
/* func = The function exits command processing                           */
/* i/o  = none                                                            */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_cmd_prmemtr(void)
{
    key_t key;
    long i;
    int  agt_is_down = D_FALSE;
    char *trcbufp;


    if (agt_sem_check_if_agent_is_up(agt.semid) == 0) {
	agt_is_down = D_TRUE;
	fprintf(stderr, "AGENT is not up, but trying anyway...\n");
    }



    key = ftok(agt.ipcpath, D_ID_SHM_T);
    if (key == -1) {
	fprintf(stderr, "ftok failed (%s, %c).\n", agt.ipcpath, D_ID_SHM_T);
	if (agt_is_down) {
	    agt__cmd_exit(D_EXIT_NOAGENT);
	} else {
	    agt__cmd_exit(D_EXIT_UNKNOWN);
	}
    }
    agt.shmid_t = shmget(key, trc.memtrsize, D_IPC_ACCESS_MODE);
    if (agt.shmid_t == -1) {
	switch (errno) {
	case ENOSPC:
	case ENOMEM:
	    if (agt_is_down) {
		fprintf(stderr, "AGENT is not up.\n");
	    }
	    fprintf(stderr, "shmget failed (%i, %i, %i).\n", errno, D_ID_SHM_T, trc.memtrsize);
	    if (agt_is_down) {
		agt__cmd_exit(D_EXIT_NOAGENT);
	    } else {
		agt__cmd_exit(D_EXIT_OUTOFRSC);
	    }
	    break;
	default:
	    fprintf(stderr, "shmget failed (%8x, %i).\n", key, errno);
	    agt__cmd_exit(D_EXIT_UNKNOWN);
	    break;
	}
    }

    trcbufp = (char *)shmat(agt.shmid_t, (char *)0, 0);
    if (trcbufp == (char *)-1) {
	fprintf(stderr, "shmat failed (%8x, %i).\n", key, errno);
	agt__cmd_exit(D_EXIT_UNKNOWN);
    }

    for (i=0; i < trc.memtrsize; i++) {
	fprintf(stdout, "%c", (unsigned char)trcbufp[i]);
    }

    fclose(stdout);

    agt__cmd_exit(D_EXIT_OK);
}


/**************************************************************************/
/* ID	= agt__cmd_exit							  */
/* name = Exiting process  						  */
/* func = The function exits the process    				  */ 
/* i/o  =                                                                 */
/*	exstatus:int:IN:exit status					  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__cmd_exit(int exstatus)
{

#ifdef _DEBUG
    if (debug.noexit) {
	agt_exit(exstatus);
    }
#endif

    agt_bare_exit(exstatus);

}
