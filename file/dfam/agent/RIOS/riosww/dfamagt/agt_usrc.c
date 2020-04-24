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
static char *RCSID_agt_usrc_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_usrc.c,v 1.1.2.3 1996/03/12 04:30:41 takano Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_usrc.c,v $
 * Revision 1.1.2.3  1996/03/12  04:30:41  takano
 * 	Delete 2byte code.
 * 	[1996/03/12  04:30:05  takano]
 *
 * Revision 1.1.2.2  1996/03/09  20:44:26  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:16  marty]
 * 
 * Revision 1.1.2.1  1996/02/16  05:15:14  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:14:47  dce-sdc]
 * 
 * Revision 1.1.2.13  96/02/06  07:50:24  07:50:24  ichikawa (T.Ichikawa)
 * 	When Gateway is down,change syslog message to AgentTrace.
 * 	[1996/02/06  07:50:08  ichikawa]
 * 
 * Revision 1.1.2.12  1996/01/12  05:06:41  kishi
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:06:32  kishi]
 * 
 * Revision 1.1.2.11  1995/11/14  02:34:57  ichikawa
 * 	Change disposition of recv error.
 * 	[1995/11/14  02:34:44  ichikawa]
 * 
 * Revision 1.1.2.10  1995/11/13  02:39:43  ichikawa
 * 	Change dispositon of ntohl.
 * 	[1995/11/13  02:39:31  ichikawa]
 * 
 * Revision 1.1.2.9  1995/11/06  09:29:48  ichikawa
 * 	When recv system call error,don't take shelter errno.
 * 	[1995/11/06  09:29:05  ichikawa]
 * 
 * Revision 1.1.2.8  1995/11/02  07:15:10  ichikawa
 * 	Delete error check.
 * 	[1995/11/02  07:14:59  ichikawa]
 * 
 * Revision 1.1.2.7  1995/11/01  07:28:26  takano
 * 	Error Check.
 * 	[1995/11/01  07:28:09  takano]
 * 
 * Revision 1.1.2.6  1995/10/26  01:27:28  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:27:15  kishi]
 * 
 * Revision 1.1.2.5  1995/09/29  04:24:52  ichikawa
 * 	Modify Trace informain.
 * 	[1995/09/29  04:24:39  ichikawa]
 * 
 * Revision 1.1.2.4  1995/09/29  00:13:25  kishi
 * 	Add header comment.
 * 	[1995/09/29  00:13:07  kishi]
 * 
 * Revision 1.1.2.3  1995/09/28  10:13:38  ichikawa
 * 	Delete Timeout disposition.
 * 	[1995/09/28  10:13:17  ichikawa]
 * 
 * Revision 1.1.2.3  1995/09/28  10:00:51  ichikawa
 * 	Delete Timer disposition.
 * 	[1995/09/28  10:00:24  ichikawa]
 * 
 * Revision 1.1.2.2  1995/09/14  05:51:29  ichikawa
 * 	malloc management is changed.
 * 	Add cpread management.
 * 	[1995/09/14  05:51:15  ichikawa]
 * 
 * Revision 1.1.2.1  1995/09/09  01:34:40  kishi
 * 	Initial revision
 * 	[1995/09/08  10:59:17  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <syslog.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/uio.h>
#include <sys/select.h>
#include <sys/types.h>
#include <memory.h>

#include "agt_agent.h"
#include "agt_sck.h"
#include "agt_usrc.h"
#include "agt_acs.h"
#include "agt_file.h"
#include "agt_msg.h"
#include "agt_timer.h"
#include "agt_login.h"
#include "agt_errid.h"
#include "agt_sigdce.h"
#include "hi_err.h"

static int agt__usrc_call_fileap(unsigned short command,char *recvbuf);
static void agt__usrc_recv_data(char *recvbuf,unsigned int recvdatalen,int type);
static void agt__usrc_recv_hdata(unsigned short *command,char *recvbuf);
static void agt__usrc_send_data(char *sendbuf,unsigned int senddatalen);
static void agt__usrc_send_hdata(unsigned short command,unsigned int senddatalen);
static void agt__usrc_timer_start(int type);
static void agt__usrc_exit(struct agt_timer *tp);
static void agt__usrc_init_signal(void);
static void agt__usrc_okexit(void);

struct agt_timer *usertimer;
extern struct d_login_info user_info;
#ifdef _FRAGWRITE
struct sdvec {

	char *datap;
	int len;
};
static void agt__usrc_send_datav(struct sdvec *vec, int count);
#endif

int datasize;
struct osinf oserr;
int traceflag;
/**************************************************************************/
/* ID	= agt_usrc_main							  */
/* name = User management main process					  */
/* func = Initialize user management main process. After transmitting     */
/*        information requested by gateway call agt__usrc_call_fileap     */
/* io = none		                                                  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_usrc_main(void)
{
	unsigned short command;
	char *recvbuf;
	char senddata[8];
	char headbuf[8];


	traceflag = 0;


	agt__usrc_init_signal();

	while(1){

        	agt__usrc_recv_hdata(&command,headbuf);
		if (datasize < 0){
			sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25003,datasize,user_info.uid);
			if (memcmp(agt.proctype,"USRM",4) == 0)
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
			else
                        	sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
                        AgentMsg(LOG_ERR, msg.buf);
                        agt_usrc_send_emsg(senddata,AGT_GWMSG_ERRMSG);
			agt_exit(D_EXIT_NETWORK);
		}

		switch(command){
			case DFAM_WRITE:
			case DFAM_CPWRITE:
				traceflag = 1;
				break;
			default:
				break;
		}


		if ((recvbuf = (char *)malloc(datasize+1)) == NULL){
                        switch(errno){
                        case ENOMEM:
				d_errno = errno;
				command |= 0x00008000;
                                sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25004,(HSIZE + datasize),user_info.uid);
                                sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),msg.wkbuf);
                                AgentMsg(LOG_WARNING, msg.buf);
				agt_usrc_send_emsg(senddata,command);
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25203,errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",4) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_exit(D_EXIT_NETWORK);
                        default:
                                sprintf(msg.wkbuf, "%d,%d,%d",D_EID_25005,errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",4) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
                                	sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
                                AgentMsg(LOG_ERR, msg.buf);
                                agt_abort();
                	}

		}


		if (datasize > 0) {
			agt__usrc_recv_data(recvbuf,datasize,1);
		}


		if (agt__usrc_call_fileap(command,recvbuf) == -1){

                	sprintf(msg.wkbuf, "%d,%d",D_EID_25006,user_info.uid);
			if (memcmp(agt.proctype,"USRM",4) == 0)
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
			else
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
                        AgentMsg(LOG_ERR, msg.buf);
			agt_usrc_send_emsg(senddata,AGT_GWMSG_ERRMSG);
			agt_exit(D_EXIT_NETWORK);
		}

		free(recvbuf);
	}
}

/**************************************************************************/
/* ID	= agt__usrc_call_fileap					 	  */
/* name = Select agent API to be executed				  */
/* func = Select API to be executed according to what is defined in       */
/*        command							  */
/* io =									  */
/*	command:unsigned short:IN:command description number		  */
/*	recvbuf:char *        :IN:information about gateway request       */
/* return =								  */
/*	0 :normal end							  */
/*  -1:abnormal end							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int agt__usrc_call_fileap(unsigned short command,char *recvbuf)
{
	if(agt.status == D_MAN_ST_USEX)
		return(0);
	switch(command){
		case DFAM_OPEN:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_open(recvbuf);
			break;
		case DFAM_CLOSE:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_close(recvbuf);
			break;
		case DFAM_CREAT:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_creat(recvbuf);
			break;
		case DFAM_REMOVE:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_remove(recvbuf);
			break;
		case DFAM_RENAME:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_rename(recvbuf);
			break;
		case DFAM_READ:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_read(recvbuf);
			break;
		case DFAM_WRITE:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_write(recvbuf);
			break;
		case DFAM_MKDIR:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_mkdir(recvbuf);
			break;
		case DFAM_RMDIR:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_rmdir(recvbuf);
			break;
		case DFAM_SYSCALL:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_acl_main(recvbuf);
			break;
		case DFAM_OPENDIR:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_opendir(recvbuf);
			break;
		case DFAM_READSTAT:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_readstat(recvbuf);
			break;
		case DFAM_CLOSEDIR:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_closedir(recvbuf);
			break;
		case DFAM_TIME:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_time_gettime(recvbuf);
			break;
		case DFAM_DIRSYNC:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_dsync_main(recvbuf);
			break;
		case DFAM_RIGHTS:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_acl_rights(recvbuf);
			break;
		case DFAM_STAT:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_stat(recvbuf);
			break;
		case DFAM_UTIME:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_utime(recvbuf);
			break;
		case DFAM_CHOWN:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_chown(recvbuf);
			break;
		case DFAM_LingetId:
			if (agt.status != D_MAN_ST_RUN)
				return(-1);
			agt.status = LinGetId(recvbuf);
			break;
		case DFAM_LinGetPasswd:
			if (agt.status != D_MAN_ST_LGP)
				return(-1);
			agt.status = LinGetPasswd(recvbuf);
			break;
		case DFAM_LinResetKey:
			if (agt.status != D_MAN_ST_LRK)
				return(-1);
			agt.status = LinResetKey();
			break;
		case DFAM_Lout:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt.status = Lout();
			agt__usrc_okexit();
			break;
		case DFAM_LSEEK:
                        if (agt.status != D_MAN_ST_FA)
                                return(-1);
			agt_file_lseek(recvbuf);
			break;
		case DFAM_CPREAD:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_cpread(recvbuf);
			break;
		case DFAM_CPWRITE:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_cpwrite(recvbuf);
			break;
		case DFAM_CPWRITEEND:
			if (agt.status != D_MAN_ST_FA)
				return(-1);
			agt_file_cpwrite_end(recvbuf);
			break;
		default:
			d_errno = ECOM;
			return(-1);
	}
	return(0);
}

/**************************************************************************/
/* ID	= agt__usrc_recv_hdata						  */
/* name = Transmit header information from gateway			  */
/* func = Transmit header information from gateway and unpack transmitted */
/*        data								  */
/* io =									  */
/*	command:unsigned short *:OUT:command description number		  */
/*	recvbuf:char *        :IN:information about gateway request       */
/* return =								  */
/*	0 :normal end							  */
/*  -1:abnormal end							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__usrc_recv_hdata(unsigned short *command,char *recvbuf)
{

	agt__usrc_recv_data(recvbuf,HSIZE,0);


	memcpy((char *)command,recvbuf,sizeof(*command));
	*command = ntohs(*command);
	memcpy((char *)&datasize,recvbuf+4,sizeof(datasize));
	datasize = ntohl(datasize);
}

/**************************************************************************/
/* ID	= agt__usrc_send_hdata						  */
/* name = Transmit header information to gateway			  */
/* func = Pack informantion and send header information to gateway	  */
/* io =									  */
/*	command:unsigned short *:IN:command description number		  */
/*	senddatalen:char *      :IN:length of real data			  */
/* return =								  */
/*	0 :normal end							  */
/*  -1:abnormal end							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__usrc_send_hdata(unsigned short command,unsigned int senddatalen)
{
	char sendbuf[HSIZE];


	memset(sendbuf,0,8);
	command = htons(command);
	memcpy(sendbuf,(char *)&command,sizeof(command));
	senddatalen = htonl(senddatalen);
	memcpy(&sendbuf[4],(char *)&senddatalen,sizeof(senddatalen));
	agt__usrc_send_data(sendbuf,HSIZE);
	command = ntohs(command);
	switch(command){
		case DFAM_READ_R:
		case DFAM_CPREAD_R:
			traceflag = 1;
			break;
		default:
			break;
	}
}


/**************************************************************************/
/* ID	= agt__usrc_recv_data						  */
/* name = Transmit execution information requested by gateway	          */	
/* func = Transmit execution information requested by gateway. If the     */
/*        information is splited in various pieces, set the timer and if  */
/*        the transmition is not completed until the time expires it is   */
/*        assumed that an error has occured                               */
/* io =									  */
/*	recvbuf    :char *      :OUT:information requested by gateway	  */
/*	recvdatalen:unsigned int:IN :length of recvbuf			  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__usrc_recv_data(char *recvbuf,unsigned int recvdatalen,int type)
{

	int recvlen;
	char *movepoint;
	int rtn, rtnselect,recv_buflen;
	int saved_errno;
	fd_set readfds;
	fd_set writefds;
	fd_set exceptfds;
	int fdmax;
	struct timeval wait_time;



	movepoint = recvbuf;

	while(1){

		if (recvdatalen < sck.gwrcvbufsize)
			recv_buflen = recvdatalen;
		else
			recv_buflen = sck.gwrcvbufsize;
		while(1){

select_retry:
		    rtn = pthread_mutex_unlock(&sig_mutex);
		    if (rtn == -1) {
			sprintf(msg.wkbuf, "%i,%i", D_EID_25129, errno);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		    }

		    FD_ZERO(&readfds);
		    FD_ZERO(&writefds);
		    FD_ZERO(&exceptfds);
		    FD_SET(sck.gw_fd, &readfds);
		    fdmax = sck.gw_fd + 1;
		    errno = 0;
		    if (type == 0){
		    	rtnselect = select(fdmax, (int *)&readfds,
				 (int *)&writefds, (int *)&exceptfds, NULL);
		    }
		    else{
				wait_time.tv_sec = sck.readtimeout;
				wait_time.tv_usec = 0;
				rtnselect = select(fdmax, (int *)&readfds,(int *)&writefds, (int *)&exceptfds, NULL);
		    }

            saved_errno = errno;

		    rtn = pthread_mutex_lock(&sig_mutex);
		    if (rtn == -1) {
			sprintf(msg.wkbuf, "%i,%i", D_EID_25130, errno);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		    }

		    errno = saved_errno;
		    switch (rtnselect) {
		    case -1:
			switch (errno) {
			case EINTR:
			    goto select_retry;
			default:
			    sprintf(msg.wkbuf, "%i,%i", D_EID_25131, errno);
			    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			    AgentMsg(LOG_ERR, msg.buf);
			    agt_abort();
			}
			case 0:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25016,errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",4) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_exit(D_EXIT_NETWORK);
		    case 1:
			break;
		    default:
			sprintf(msg.wkbuf, "%i,%i", D_EID_25132, errno);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_abort();
		    }

		    recvlen = recv(sck.gw_fd,movepoint,recv_buflen,0);

		    if (recvlen == -1 ){
			switch(errno){
			case EINTR:
			    goto select_retry;
			case EBADF:
			case ENOTSOCK:
			case EFAULT:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25234,errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",4) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
			default:

			    sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25008,errno,user_info.uid);
			    if (memcmp(agt.proctype,"USRM",4) == 0)
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
			    else
			        sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
			    AgentMsg(LOG_ERR,msg.buf);
			    agt_exit(D_EXIT_NETWORK);
			}
		    }


		    break;
		}


		if (traceflag == 0){
			AgentTrace(TRACECOM,"RVDATA",recvlen,movepoint,recvlen);
		}
		else{
			AgentTrace(TRACECOM,"RVDATA",((recvlen > 16) ? 16 : recvlen),movepoint,recvlen);
		}
		if (recvlen == 0){
			AgentTrace(TRACEERR,"USRCOM",12,"Gateway down",12);
			agt_exit(D_EXIT_OK);
		}

		if (recvdatalen == recvlen)
			break;


		recvdatalen -= recvlen;
		movepoint += recvlen;

	}
	traceflag = 0;
}

#ifdef _FRAGWRITE
/**************************************************************************/
/* ID	= agt__usrc_send_data 						  */
/* name = Standardize information send to gateway			  */
/* func = Standardize information in the witev form before sending to     */
/*        gateway							  */
/* io =									  */
/*	sendbuf    :char *      :IN:information to send to gateway	  */
/*	senddatalen:unsigned int:IN:length of sendbuf			  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__usrc_send_data(char *sendbuf,unsigned int senddatalen)
{
	struct sdvec vec;

	vec.datap = sendbuf;
	vec.len = senddatalen;
	agt__usrc_send_datav(&vec,1);
}

/**************************************************************************/
/* ID	= agt__usrc_send_datav						  */
/* name = Transmit data to gateway					  */
/* func = Transmit data in vec to gateway. If the transmittion fails,     */
/*        try again until it succeed 					  */
/* io =									  */
/*	vec  :struct sdvec *:IN:data					  */
/*	count:int           :IN:string number of vec			  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__usrc_send_datav(struct sdvec *vec, int count)
{
        int sendlen;
        int saved_errno;
        fd_set readfds;
        fd_set writefds;
        fd_set exceptfds;
        int fdmax;
        int rtn, rtnselect;
	unsigned short cmd;



	if (count == 2){
        	memcpy((char *)&cmd,vec->datap,sizeof(cmd));
        	cmd = ntohs(cmd);
        	if ((cmd != DFAM_READ_R) && (cmd != DFAM_CPREAD_R)){
                	AgentTrace(TRACECOM,"SDDATA",HSIZE,vec->datap,HSIZE);
			AgentTrace(TRACECOM,"SDDATA",(vec+1)->len,(vec+1)->datap,(vec+1)->len);
		}
		else{
			AgentTrace(TRACECOM,"SDDATA",HSIZE,vec->datap,HSIZE);
			AgentTrace(TRACECOM,"SDDATA",(((vec+1)->len > 16) ? 16 : (vec+1)->len),(vec+1)->datap,(vec+1)->len);
		}
	}
	else{
		AgentTrace(TRACECOM,"SDDATA",vec->len,vec->datap,vec->len);
	}


        while(1){
                rtn = pthread_mutex_unlock(&sig_mutex);
                if (rtn == -1) {
                        sprintf(msg.wkbuf, "%i,%i", D_EID_25133, errno);
                        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
                        AgentMsg(LOG_ERR, msg.buf);
                        agt_abort();
                }

                FD_ZERO(&readfds);
                FD_ZERO(&writefds);
                FD_ZERO(&exceptfds);
                FD_SET(sck.gw_fd, &writefds);
                fdmax = sck.gw_fd + 1;
                errno = 0;
                rtnselect = select(fdmax, (int *)&readfds,(int *)&writefds, (int *)&exceptfds, NULL);

                saved_errno = errno;

                rtn = pthread_mutex_lock(&sig_mutex);
                if (rtn == -1) {
                        sprintf(msg.wkbuf, "%i,%i", D_EID_25134, errno);
                        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
                        AgentMsg(LOG_ERR, msg.buf);
                        agt_abort();
                }

                errno = saved_errno;
                switch (rtnselect) {
                case -1:
                        switch (errno) {
                        case EINTR:
                                continue;

                        default:
                                sprintf(msg.wkbuf, "%i,%i", D_EID_25135, errno);
                                sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
                                AgentMsg(LOG_ERR, msg.buf);
                                agt_abort();
                                break;
                        }
                        break;

                case 1:

                        break;

                default:
                        sprintf(msg.wkbuf, "%i,%i", D_EID_25136, errno);
                        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
                        AgentMsg(LOG_ERR, msg.buf);
                        agt_abort();
                        break;
                }

                if ((sendlen = writev(sck.gw_fd, (struct iovec *) vec, count)) == -1) {
                        switch(errno){
                        case EWOULDBLOCK:
                        case EINTR:
                                continue;

                        case ENOBUFS:
                                sprintf(msg.wkbuf,"%d,%d",D_EID_25009,user_info.uid);
                                sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),msg.wkbuf);
                                AgentMsg(LOG_WARNING,msg.buf);
                                agt_exit(D_EXIT_OUTOFRSC);
                                break;

                        case ENOTCONN:
                        case EPIPE:
                        case ENETDOWN:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25141,errno,user_info.uid);
                                if (memcmp(agt.proctype,"USRM",4) == 0) {
                                        sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
                                } else {
                                                sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
                                }
                                AgentMsg(LOG_ERR,msg.buf);
                                agt_abort();
                                break;
                        default:

                                sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25010,errno,user_info.uid);
                                if (memcmp(agt.proctype,"USRM",4) == 0) {
                                        sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
                                } else {
                                                sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
                                }
                                AgentMsg(LOG_ERR,msg.buf);
                                agt_abort();
                                break;
                        }
                }

                while (sendlen > 0) {
                        if (sendlen >= vec->len) {

                                sendlen -= vec->len;
                                ++vec;
                                --count;
                        } else {

                                vec->datap += sendlen;
                                vec->len -= sendlen;
                                sendlen = 0;
                        }
                }
                if (count <= 0) {

                        break;
                }
        }
        traceflag = 0;
#ifdef _DEBUG
	AgentTrace(TRACEDEB,"SDEND ",12,"senddata end",12);
#endif
}
#else
/**************************************************************************/
/* ID	= agt__usrc_send_data 						  */
/* name = Standardize information send to gateway			  */
/* func = Standardize information in the witev form before sending to     */
/*        gateway							  */
/* io =									  */
/*	sendbuf    :char *      :IN:information to send to gateway	  */
/*	senddatalen:unsigned int:IN:length of sendbuf			  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__usrc_send_data(char *sendbuf,unsigned int senddatalen)
{
	int send_buflen;
	int sendlen;
	char *movepoint;
	int saved_errno;
	fd_set readfds;
	fd_set writefds;
	fd_set exceptfds;
	int fdmax;
	int rtn, rtnselect;

	movepoint = sendbuf;


	if (traceflag == 0)
		AgentTrace(TRACECOM,"SDDATA",senddatalen,movepoint,senddatalen);
	if (traceflag == 1)
		AgentTrace(TRACECOM,"SDDATA",((senddatalen > 16) ? 16 : senddatalen),movepoint,senddatalen);


	while(1){

		if (senddatalen < sck.gwsndbufsize)
			send_buflen = senddatalen;
		else
			send_buflen = sck.gwsndbufsize;



		while(1){

select_retry:
  		        rtn = pthread_mutex_unlock(&sig_mutex);
		        if (rtn == -1) {
			        sprintf(msg.wkbuf, "%i,%i", D_EID_25133, errno);
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			        AgentMsg(LOG_ERR, msg.buf);
			        agt_abort();
		        }

		        FD_ZERO(&readfds);
		        FD_ZERO(&writefds);
		        FD_ZERO(&exceptfds);
		        FD_SET(sck.gw_fd, &writefds);
		        fdmax = sck.gw_fd + 1;
		        errno = 0;
		        rtnselect = select(fdmax, (int *)&readfds,
					   (int *)&writefds, (int *)&exceptfds, NULL);

                        saved_errno = errno;

		        rtn = pthread_mutex_lock(&sig_mutex);
		        if (rtn == -1) {
			        sprintf(msg.wkbuf, "%i,%i", D_EID_25134, errno);
			        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR, msg.buf);
				agt_abort();
			}

			errno = saved_errno;
			switch (rtnselect) {
			case -1:
			    switch (errno) {
			    case EINTR:
				goto select_retry;
			    default:
				sprintf(msg.wkbuf, "%i,%i", D_EID_25135, errno);
				sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
				AgentMsg(LOG_ERR, msg.buf);
				agt_abort();
			    }
			case 1:
			    break;
			default:
			    sprintf(msg.wkbuf, "%i,%i", D_EID_25136, errno);
			    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			    AgentMsg(LOG_ERR, msg.buf);
			    agt_abort();
			}

			if ((sendlen = send(sck.gw_fd,movepoint,send_buflen,0)) == -1){
				switch(errno){
				        case EWOULDBLOCK:
					case EINTR:
						goto select_retry;
					case ENOBUFS:
						sprintf(msg.wkbuf,"%d,%d",D_EID_25009,user_info.uid);
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),msg.wkbuf);
						AgentMsg(LOG_WARNING,msg.buf);
						agt_exit(D_EXIT_OUTOFRSC);
					case ENOTCONN:
					case EPIPE:
					case ENETDOWN:

						agt_exit(D_EXIT_NETWORK);
						break;
					default:

						sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25010,errno,user_info.uid);
						if (memcmp(agt.proctype,"USRM",4) == 0)
							sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
						else
							sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
						AgentMsg(LOG_ERR,msg.buf);
						agt_abort();
				}
			}
			break;
		}

		if (senddatalen == sendlen)
			break;


		senddatalen -= sendlen;
		movepoint += sendlen;
	}
	traceflag = 0;
}
#endif

/**************************************************************************/
/* ID	= agt_usrc_send_emsg						  */
/* name = Send  error information to gateway				  */
/* func = Pack error information and send to gateway 			  */
/* io =									  */
/*	senddata:char *        :IN:error information			  */
/*  command :unsigned short:IN:length of senddata			  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_usrc_send_emsg(char *senddata,unsigned short command)
{
	int status = -1;
	int r_errno = 0;


	if (command != AGT_GWMSG_ERRMSG){
		agt__usrc_send_hdata(command,HSIZE);

		status = htonl(status);
		memcpy(senddata,(char *)&status,sizeof(status));
		if (trnserr(&d_errno,&r_errno) == FALSE){
			sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25205,d_errno,user_info.uid);
			if (memcmp(agt.proctype,"USRM",sizeof(agt.proctype)) == 0)
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
			else
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
			AgentMsg(LOG_ERR,msg.buf);
			agt_abort();
		}

		r_errno = htonl(r_errno);
		memcpy(&senddata[4],(char *)&r_errno,sizeof(r_errno));

		agt__usrc_send_data(senddata,HSIZE);
	}
	else
		agt__usrc_send_hdata(command,0);
}

#ifdef _FRAGWRITE
#define HDR_OFS_LEN 4

/**************************************************************************/
/* ID	= agt_usrc_send_data						  */
/* name = Standardize information send to gateway			  */
/* func = Standardize information in the witev form before sending to     */
/*        gateway							  */
/* io =                                                                   */
/*	senddata   :char *	  :IN:information to send to gateway	  */
/*	senddatalen:int		  :IN:length of senddata		  */
/*	command    :unsigned short:IN:command information		  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_usrc_send_data(char *senddata,int senddatalen,unsigned short command)
{
        char hdrbuf[HSIZE];
        struct sdvec    vec[2];

        memset(hdrbuf, 0, sizeof(hdrbuf));

        command = htons(command);
        memcpy(hdrbuf,(char *)&command,sizeof(command));

        senddatalen = htonl(senddatalen);
        memcpy(&hdrbuf[HDR_OFS_LEN],(char *)&senddatalen,sizeof(senddatalen));

        vec[0].datap = hdrbuf;
        vec[0].len = HSIZE;
        vec[1].datap = senddata;
        vec[1].len = senddatalen;

        agt__usrc_send_datav(vec, 2);

        command = ntohs(command);
        switch(command){
                case DFAM_READ_R:
		case DFAM_CPREAD_R:
                        traceflag = 1;
                        break;
                default:
                        break;
        }
}
#else
/**************************************************************************/
/* ID	= agt_usrc_send_data						  */
/* name = Standardize information send to gateway			  */
/* func = Standardize information in the witev form before sending to     */
/*        gateway							  */
/* io =                                                                   */
/*	senddata   :char *	  :IN:information to send to gateway	  */
/*	senddatalen:int		  :IN:length of senddata		  */
/*	command    :unsigned short:IN:command information		  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_usrc_send_data(char *senddata,int senddatalen,unsigned short command)
{
	agt__usrc_send_hdata(command,senddatalen);
	agt__usrc_send_data(senddata,senddatalen);
}
#endif


/**************************************************************************/
/* ID   = agt__usrc_timer_start                                           */
/* name = Set timer for manager(user) process			          */
/* func = Set timer for manager(user) process when he is receiving an     */
/*        information or performing exit process                          */
/* io =                                                                   */
/*      type:int:IN:type of timer setting                                 */   
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__usrc_timer_start(int type)
{
	if (type == USRC_WAIT)
		usertimer->seconds = sck.readtimeout;
	else
		usertimer->seconds = sck.sdtimeout;
	usertimer->timer_func = agt__usrc_exit;
	AgentSetTimer(usertimer);
}

/**************************************************************************/
/* ID = agt__usrc_init_signal						  */
/* name = Set signal for manager(user) process				  */
/* func = Initialize timer signal for manager(user) process		  */
/* io = none								  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__usrc_init_signal(void)
{

	AgentInitTimer();

	if ((sck.sdtimeout != 0) || (sck.readtimeout != 0)){

		if ((usertimer = (struct agt_timer *)malloc(sizeof(struct agt_timer))) == NULL){
			switch(errno){
				case ENOMEM:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25011,sizeof(struct agt_timer),user_info.uid);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),msg.wkbuf);
					AgentMsg(LOG_WARNING,msg.buf);
					agt_exit(D_EXIT_OUTOFRSC);
				default:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25012,errno,user_info.uid);
					if (memcmp(agt.proctype,"USRM",4) == 0)
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
					else
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
			}
		}
	}


	agt_sig_dce_init();
}

/**************************************************************************/
/* ID = agt__usrc_exit							  */
/* name = Routine to exit manager(user) process				  */
/* func = Routine to exit manager(user) process	 when an abnormal end     */
/* io = none                                                              */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__usrc_exit(struct agt_timer *tp)
{

	sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02563,KDDS02563),sck.gwname);
	AgentMsg(LOG_ERR,msg.buf);

	agt.status = D_MAN_ST_USEX;



	if (memcmp(agt.proctype,"USRM",4) == 0)
		sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
	else
		sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
AgentMsg(LOG_ERR,msg.buf);

	agt_exit(D_EXIT_NETWORK);
}

/**************************************************************************/
/* ID	= agt__usrc_okexit						  */
/* name = Routine to exit manager(user) process				  */
/* func = Routine to exit manager(user) process	 when an normal end       */
/*        occurs							  */
/* io = none                                                              */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__usrc_okexit(void)
{
	agt.status = D_MAN_ST_USEX;
	if (shutdown(sck.gw_fd,2) == -1){
		switch(errno){
			case EINVAL:
			case ENETDOWN:
			case ENOTCONN:

				agt_exit(D_EXIT_NETWORK);
			default:

				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25018,errno,user_info.uid);
				if (memcmp(agt.proctype,"USRM",4) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}
	while(1){
		if (close(sck.gw_fd) == -1){
			switch(errno){
				case EINTR:
					continue;
				default:

					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25019,errno,user_info.uid);
					if (memcmp(agt.proctype,"USRM",4) == 0)
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
					else
						sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_abort();
			}
		}
		break;
	}
	agt_exit(D_EXIT_OK);
}

/**************************************************************************/
/* ID	= agt_usrc_oserr_trace						  */
/* name = Write error trace information					  */
/* func = If an error occured in system call,write system call an errno   */
/*        information into trace					  */
/* io = 								  */
/*	osinf :int         :IN:system call descriptor			  */
/*	derrno:unsigned int:IN:errno information			  */
/* return = none                                                          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_usrc_oserr_trace(int osinf,unsigned int derrno)
{
	oserr.syscall = osinf;
	oserr.derrno = derrno;
	memcpy(msg.buf,(char *)&oserr,sizeof(oserr));
	AgentTrace(TRACEERR,"ERROS ",sizeof(oserr),msg.buf,0);
}

