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
static char *RCSID_agt_msg_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_msg.c,v 1.1.2.2 1996/03/09 20:44:09 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_msg.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:09  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:05  marty]
 *
 * Revision 1.1.2.1  1996/02/16  06:02:40  dce-sdc
 * 	First Submit
 * 	[1996/02/16  06:01:59  dce-sdc]
 * 
 * Revision 1.1.2.5  96/01/12  04:39:11  04:39:11  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:39:01  kishi]
 * 
 * Revision 1.1.2.4  1995/10/26  00:59:33  kishi
 * 	Change Copyright.
 * 	[1995/10/26  00:59:25  kishi]
 * 
 * Revision 1.1.2.3  1995/09/23  09:52:10  kishi
 * 	Add header comment.
 * 	[1995/09/23  08:53:22  kishi]
 * 
 * Revision 1.1.2.2  1995/09/12  08:29:10  kishi
 * 	Add member of struct sockaddr_un.Then use macro SUN_LEN.
 * 	[1995/09/12  08:28:32  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:51:59  kishi
 * 	Initial revision
 * 	[1995/09/09  06:35:54  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <sys/un.h>
#include <syslog.h>
#include <memory.h>

#include "agt_agent.h"
#include "agt_acs.h"
#include "agt_msg.h"
#include "agt_ipc.h"
#include "agt_errid.h"
#include "agt_login.h"
#include "agt_sig.h"
#include "agt_sysid.h"

struct d_msg msg = {
    {'M', 'S', 'G', '*'},

    {'\0'},

    0,
    0,

    0,
    {'\0'}
};
extern struct d_login_info user_info;



/**************************************************************************/
/* ID	= AgentMsg							  */
/* name = Print message into syslog file				  */ 
/* func = Print string specified in message in the syslog file. Write     */
/*        message to event trace file when it is necessary		  */
/* i/o =								  */
/*	priority:int   :IN:priority information used by syslog		  */
/*  message :char *:IN:information about message to write in syslog	  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void AgentMsg(priority,message)
int priority;
char *message;
{
	char *eventmsg;
	char *workmsg;
	int eventmsglen;
	char savedc = message[0];
	char *savedp = message;
	struct timeval wait_tv;

	while (1) {
	    char *p;
	    char *current;

	    current = savedp;
	    *savedp = savedc;
	    for (savedc = '\0', p = savedp; *p != '\0'; p++) {
		if (*p == '\n') {
		    savedp = ++p;
		    savedc = *p;
		    *p = '\0';
		    break;
		}
	    }

	    if(syslog(priority, current) == -1){
		eventmsglen = strlen(message);
		agt_usrc_oserr_trace(DSYSLOG,errno);
		AgentTrace(TRACEERR,"SYSLOG",eventmsglen,message,0);
	    }

	    if (savedc == '\0') {
		break;
	    } else {
		wait_tv.tv_sec = 0;
		wait_tv.tv_usec = D_SYSLOG_WAIT_TIME;
		select(0, (int *)0, (int *)0, (int *)0, &wait_tv);
	    }
	}


	if ((trc.evttrfiles != 0) || (trc.memtrsize != 0)){
		eventmsglen = strlen(message);

		if ((eventmsg = (char *)malloc(eventmsglen+sizeof(priority)+sizeof(eventmsglen))) == NULL){
			switch(errno){
				case ENOMEM:
					sprintf(msg.wkbuf,"%d,%d",0,eventmsglen+sizeof(priority)+sizeof(eventmsglen));
					fprintf(stderr,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),msg.wkbuf);
					goto end;
				default:
					sprintf(msg.wkbuf,"%d,%d,%d",0,errno,eventmsglen+sizeof(priority)+sizeof(eventmsglen));
					fprintf(stderr,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
					agt_abort();
					return;
			}
		}

		workmsg = eventmsg;

		memcpy(workmsg,(char *)&priority,sizeof(priority));
		workmsg += sizeof(priority);
		memcpy(workmsg,(char *)&eventmsglen,sizeof(eventmsglen));
		workmsg += sizeof(eventmsglen);
		memcpy(workmsg,message,eventmsglen);

		AgentTrace(TRACEERR,"SYSLOG",(eventmsglen+sizeof(priority)+sizeof(eventmsglen)),eventmsg,0);
		free(eventmsg);
	}
end:
	;;
}

/**************************************************************************/
/* ID	= GatewayMsg							  */
/* name = Send a request to watchdog process to send a message            */
/* func = Send a message to the connected gateway through the watchdog    */
/*        process. The watchdog process is specified in the wdpid field   */
/*        in the message						  */
/* i/o =								  */
/*	length :int   :IN:ength of the data in message			  */
/*	message:char *:IN:specify the message to be sent to gateway	  */
/*  wdpid  :pid_t :IN:watchdog process-ID connected to the gateway	  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void GatewayMsg(msglen,message,wdpid)
int msglen;
char *message;
pid_t wdpid;
{
	char *sendmsg;
	char *msgwork;
	int i;
	int ttllen;
	struct sockaddr_un wdadr;
	int adrlen;
	struct sockaddr_un *adrp;

	ttllen = msglen + 4*3;
	if ((sendmsg = (char *)malloc(ttllen)) == NULL){
		switch(errno){
			case ENOMEM:
				sprintf(msg.wkbuf,"%d,%d",D_EID_25123,ttllen);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02500,KDDS02500),strerror(errno),msg.wkbuf);
				AgentMsg(LOG_WARNING,msg.buf);
				agt_exit(D_EXIT_OUTOFRSC);
				return;
			default:
				sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25124,errno,ttllen);
				if (memcmp(agt.proctype,"USRC",4) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
				else if (memcmp(agt.proctype,"USRM",4) == 0)
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
				else
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02031,KDDS02031),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}

	msgwork = sendmsg;

	i = D_AG_WMSG;
	memcpy(msgwork,(char *)&i,sizeof(i));
	msgwork += sizeof(i);

	if (wdpid == 0) {
	    adrp = &sck.sysc_adr;
	    adrlen = sck.sysc_adrlen;

	    msglen += 4;
	    memcpy(msgwork,(char *)&msglen,sizeof(msglen));
	    msgwork += sizeof(msglen);
    	    msglen -= 4;
	    memcpy(msgwork,(char *)&sck.gwipaddr,sizeof(sck.gwipaddr));
	    msgwork += sizeof(sck.gwipaddr);
	} else {
	    ttllen -= 4;
	    sprintf(wdadr.sun_path, "%s%s%05i", sck.tmpl_adr.sun_path, D_UXSOC_CHLD, wdpid);
	    adrlen = SUN_LEN(&wdadr);
	    adrp = &wdadr;

	    memcpy(msgwork,(char *)&msglen,sizeof(msglen));
	    msgwork += sizeof(msglen);
	}

	memcpy(msgwork,message,msglen);

#ifdef _DEBUG100
	AgentTrace(TRACEDEB,"SDMSG ",ttllen,sendmsg,0);
#endif

	while(1){
		if (sendto(sck.ux_fd,sendmsg,ttllen,0,(struct sockaddr *)adrp,adrlen) != ttllen){
	    		switch (errno) {
				case EINTR:
					continue;
	    			default:
					sprintf(msg.wkbuf,"%d,%d,%d",D_EID_25128,errno,user_info.uid);
                                if (memcmp(agt.proctype,"USRC",4) == 0)
                                        sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02034,KDDS02034),msg.wkbuf);
                                else if (memcmp(agt.proctype,"USRM",4) == 0)
                                        sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02032,KDDS02032),msg.wkbuf);
                                else
                                        sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02031,KDDS02031),msg.wkbuf);
                                AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
	    		}
		}
		break;
	}


	if (trc.evttrfiles != 0)
                AgentTrace(TRACEERR,"GWMESG",ttllen,sendmsg,wdpid);

        free(sendmsg);
}


/**************************************************************************/
/* ID	= agt_msg_open_syslog						  */
/* name = Open syslog file						  */
/* func = Open syslog file						  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_msg_open_syslog(void)
{

    sprintf(msg.syslogid, "%s%s", D_SYSLOG_ID, agt.agent_id);
    openlog(msg.syslogid, (LOG_PID|LOG_CONS|LOG_NDELAY|LOG_NOWAIT), LOG_USER);

}


