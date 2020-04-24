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
static char *RCSID_agt_init_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_init.c,v 1.1.2.2 1996/03/09 20:44:03 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_init.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:03  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:23:59  marty]
 *
 * Revision 1.1.2.1  1996/02/16  05:07:04  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:06:36  dce-sdc]
 * 
 * Revision 1.1.2.6  96/01/31  04:49:32  04:49:32  ichikawa (T.Ichikawa)
 * 	Delete Debug segment.
 * 	[1996/01/31  04:49:19  ichikawa]
 * 
 * Revision 1.1.2.5  1996/01/12  04:33:04  kishi
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:32:55  kishi]
 * 
 * Revision 1.1.2.4  1995/10/26  00:41:14  kishi
 * 	Change Copyright.
 * 	[1995/10/26  00:40:59  kishi]
 * 
 * Revision 1.1.2.3  1995/09/23  09:53:24  kishi
 * 	Add header comment.
 * 	[1995/09/23  08:05:13  kishi]
 * 
 * Revision 1.1.2.2  1995/09/12  08:28:50  kishi
 * 	Add member of struct sockaddr_un.Then use macro SUN_LEN.
 * 	[1995/09/12  08:28:09  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:53:09  kishi
 * 	Initial revision
 * 	[1995/09/09  06:20:25  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#undef exit

#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <syslog.h>
#include <sys/select.h>
#include <sys/types.h>
#include <dirent.h>

#include "agt_agent.h"
#include "agt_init.h"
#include "agt_errid.h"
#include "agt_main.h"
#include "agt_trc.h"
#include "agt_sem.h"
#include "agt_sck.h"
#include "agt_ipc.h"
#include "agt_stf.h"
#include "agt_msg.h"
#include "agt_lib.h"
#include "agt_sig.h"
#include "agt_parm.h"

static void agt__init_reply_to_command(struct sockaddr_un *to, int tolen,
			       char *info, int msgtyp, int rtncode);
static void agt__init_do_ps(void);
static void agt__init_do_grep(void);
static void agt__init_do_rmfile(void);
static int agt__init_my_readline(int fd, char *bp, int blen);

static int agt_pipefd[2];


/**************************************************************************/
/* ID 	= agt_init_check_agent						  */
/* name = Check if agent is active					  */
/* func = Check if agent is active. If agent is not activated, delete     */
/*        all files in the /opt/dcelocal/hitachi/dfa/.dfa directory       */
/* i/o  = none				                                  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_init_check_agent(void)
{
    pid_t rmid,rtn;
    int rmrtn;


    if (agt_sem_set_agent_up_flg(agt.semid) == 0) {
        sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02005, KDDS02005));
        AgentMsg(LOG_WARNING, msg.buf);

        agt_exit(D_EXIT_ALREADY);
    }


    rmid = fork();
    if (rmid == 0) {
	agt__init_do_rmfile();
    } else if (rmid == -1){
	sprintf(msg.wkbuf, "%i", D_EID_21058);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02600, KDDS02600), strerror(errno), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_exit(D_EXIT_UNKNOWN);
    }
rm_wait:
    rtn = waitpid(rmid, &rmrtn, 0);
    if (rtn == -1) {
	switch (errno) {
	case EINTR:
	    goto rm_wait;
	default:
	    sprintf(msg.wkbuf, "%i,%i",D_EID_21059, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620),msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_UNKNOWN);
	}
    }
}

/**************************************************************************/
/* ID	= agt_init_check_environment					  */
/* name = Check agent environment					  */
/* func = Check if dfsd daemon is active. If YES, wait for a determined   */
/*        period of time and check again if it is still activated         */
/* i/o  = none				                                  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_init_check_environment(void)
{
    pid_t pspid, greppid, rtn;
    int psrtn, greprtn;

    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    int fdmax;
    int rtnselect;
    int msgtype;
    int count = 0;
    long checkpid;
    struct timeval waittime;



    while (1) {

	pipe(agt_pipefd);

	greppid = fork();
	if (greppid == 0) {
	    agt__init_do_grep();
	} else if (greppid == -1) {
	    sprintf(msg.wkbuf, "%i", D_EID_21048);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02600, KDDS02600), strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_UNKNOWN);
	}

	pspid = fork();
	if (pspid == 0) {
	    agt__init_do_ps();
	} else if (pspid == -1) {
	    sprintf(msg.wkbuf, "%i", D_EID_21049);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02600, KDDS02600), strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_UNKNOWN);
	}

	close(agt_pipefd[0]);
	close(agt_pipefd[1]);

ps_wait:
	rtn = waitpid(pspid, &psrtn, 0);
	if (rtn == -1) {
	    switch (errno) {
	    case EINTR:
		goto ps_wait;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_21050, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620),msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_UNKNOWN);
	    }
	}

grep_wait:
	rtn = waitpid(greppid, &greprtn, 0);
	if (rtn == -1) {
	    switch (errno) {
	    case EINTR:
		goto ps_wait;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_21051, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620),msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_UNKNOWN);
	    }
	}

	if ((rtn > 0) && (greprtn == 0)) {
	    return;
	}


retry:
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);
	FD_SET(sck.ux_fd,&readfds);
	fdmax = sck.ux_fd + 1;
	errno = 0;
	waittime.tv_sec = 1;
	waittime.tv_usec = 0;
	if ((rtnselect = select(fdmax,(int *)&readfds,(int *)&writefds,(int *)&exceptfds,&waittime)) == -1){
		switch(errno){
			case EINTR:
				goto retry;
			default:
				sprintf(msg.wkbuf,"%d,%d",D_EID_25137,errno);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
				AgentMsg(LOG_ERR,msg.buf);
				agt_abort();
		}
	}
	if (rtnselect != 0){
retry_recvfrom:
		memset((char *)&sck.from, '\0',sizeof(sck.from));
		sck.fromlen = sizeof(sck.from);
		if ((sck.uxrcvlen = recvfrom(sck.ux_fd,sck.uxrcvdata,12,0,&sck.from,&sck.fromlen)) == -1){
			switch(errno){
				case EINTR:
					goto retry_recvfrom;
				default:
					sprintf(msg.wkbuf,"%d,%d",D_EID_25138,errno);
					sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02007,KDDS02007),msg.wkbuf);
					AgentMsg(LOG_ERR,msg.buf);
					agt_exit(D_EXIT_UNKNOWN);
					break;
			}
		}
		checkpid = char_to_num((sck.uxrcvdata+8),4);
		sprintf(sck.from.sun_path,"%s%05i",D_DFLT_IPCPATH,checkpid);
		sck.fromlen = SUN_LEN(&(sck.from));
		msgtype = char_to_num(sck.uxrcvdata,4);
		switch(msgtype){
			case D_AG_CSTOP:
			case D_AG_CABORT:
				agt__init_reply_to_command(&sck.from,sck.fromlen,"TRDY",msgtype,D_AG_RT_OK);
				sleep(1);
				agt_exit(D_EXIT_OK);
			case D_AG_CMSG:
				agt__init_reply_to_command(&sck.from,sck.fromlen,"TRDY",msgtype,D_AG_RT_OK);
				goto retry_recvfrom;
			default:
				sprintf(msg.wkbuf,"%d,%d,%s,%d",D_EID_25139,msgtype,sck.from.sun_path,sck.uxrcvlen);
				sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02007,KDDS02007),msg.wkbuf);
				AgentTrace(TRACEDEB,"DEBUG ",strlen(msg.buf),msg.buf,0);
				goto retry_recvfrom;
		}
	}

	if (count == 0){
    		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02282, KDDS02282));
    		AgentMsg(LOG_ERR, msg.buf);
	}
	count++;
	agt_sleep(D_DFS_CHK_INTERVAL);
    }
}

/**************************************************************************/
/* ID	= agt__init_do_grep						  */
/* name = dfsd start check						  */
/* func = Check if result of ps -ef command has resulted in the activation*/
/*        of dfsd daemon						  */	
/* i/o  = none				                                  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__init_do_grep(void)
{
    char buf[512];
    char *cmdname;
    int cmdnamelen;
    int len;


    close(0);
    dup(agt_pipefd[0]);
    close(agt_pipefd[1]);
    while ((len = agt__init_my_readline(0, buf, sizeof(buf))) != -1) {
	cmdname = buf;
	if (cmdname != NULL) {
	    cmdnamelen = strlen(cmdname);
	    if (cmdnamelen > strlen(D_DFS_CHK_STRING)) {
		cmdname += (cmdnamelen - (strlen(D_DFS_CHK_STRING)));
	    } else if (cmdnamelen < strlen(D_DFS_CHK_STRING)) {
		continue;
	    }

	    if (strcmp(cmdname, D_DFS_CHK_STRING) == 0) {
		agt_bare_exit(D_EXIT_OK);
	    }
	}
    }


    agt_bare_exit(D_EXIT_UNKNOWN);
}

/**************************************************************************/
/* ID	= agt__init_do_ps						  */
/* name = Execute ps command						  */
/* func = Execute ps -ef command					  */
/* i/o  = none				                                  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__init_do_ps(void)
{
    close(1);
    dup(agt_pipefd[1]);
    close(agt_pipefd[0]);
    execl(D_DFS_CHK_PS, D_DFS_CHK_PS, D_DFS_CHK_PS_OPT, (char *)0);
}

/**************************************************************************/
/* ID	= agt__init_my_readline						  */
/* name = Read one line of information from  stdout 			  */
/* func = Read information from the file descriptor until \n is found     */
/* i/o  =   				                                  */
/*	fd  :int   :IN :file descriptor					  */
/*  bp  :char *:OUT:output information string                             */
/*  blen:int   :OUT:number of characters in the output string		  */
/* return =	number of characters in the output string		  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static int  agt__init_my_readline(int fd, char *bp, int blen)
{
    char *cp = bp;
    int readlen = 0;
    int rtn;

    while (1) {
	rtn = read(fd, cp, 1);
	if (rtn == 1) {
	    if (*cp == '\n') {
		*cp = '\0';
		goto read_done;
	    }
	    if (*cp == ' '){
		if (readlen >= 1){
			*cp ='\0';
			goto read_done;
		}
		else
			continue;
	    }
	    readlen++;
	    if (readlen < blen) {
		cp++;
	    }
	}
	if (rtn == 0) {
	    *cp = '\0';
	    if (readlen == 0) {
		readlen = -1;
	    }
	    goto read_done;
	}
	if (rtn == -1) {
	    switch (errno) {
	    case EINTR:
		continue;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_21052, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_bare_exit(D_EXIT_UNKNOWN);
	    }
	}
    }

read_done:
    return readlen;
}


/**************************************************************************/
/* ID	= agt_init_alloc_shmem						  */
/* name = Alloc memory for public use					  */
/* func = Alloc public memory for MAIN, SYSC process, memory trace	  */
/* i/o  = none				                                  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_init_alloc_shmem(void)
{
    key_t key;
    int i;
    struct d_sysc *wsyscp;


    key = ftok(agt.ipcpath, D_ID_SHM_M);
    if (key == -1) {
	sprintf(msg.wkbuf, "%i,%s,%c", D_EID_21001, agt.ipcpath, D_ID_SHM_M);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_exit(D_EXIT_UNKNOWN);
    }
    agt.shmid_m = shmget(key, D_SZ_SHM_M,
			 D_IPC_ACCESS_MODE|IPC_EXCL|IPC_CREAT);
    if (agt.shmid_m == -1) {
	int wk_shmid;

	switch (errno) {
	case EEXIST:
	    sprintf(msg.wkbuf, "%i,%08x", errno, key);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02521, KDDS02521), msg.wkbuf);
	    AgentMsg(LOG_WARNING, msg.buf);

	    wk_shmid = shmget(key, D_SZ_SHM_M, D_IPC_ACCESS_MODE);
	    if (wk_shmid == -1) {
		sprintf(msg.wkbuf, "%i,%08x,%i", D_EID_21002, key, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_UNKNOWN);
	    }
	    if (shmctl(wk_shmid, IPC_RMID, 0) == -1) {
		sprintf(msg.wkbuf, "%i,%08x,%i", D_EID_21003, key, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_UNKNOWN);
	    }
	    agt.shmid_m = shmget(key, D_SZ_SHM_M,
				 D_IPC_ACCESS_MODE|IPC_EXCL|IPC_CREAT);
	    if (agt.shmid_m == -1) {
		switch (errno) {
		case ENOSPC:
		case ENOMEM:
		    sprintf(msg.wkbuf, "%i,%i", D_ID_SHM_M, D_SZ_SHM_M);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02520, KDDS02520), "shmget", strerror(errno), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_exit(D_EXIT_OUTOFRSC);
		    break;
		default:
		    sprintf(msg.wkbuf, "%i,%08x,%i", D_EID_21004, key, errno);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_exit(D_EXIT_UNKNOWN);
		    break;
		}
	    }
	    break;
	case ENOSPC:
	case ENOMEM:
	    sprintf(msg.wkbuf, "%i,%i", D_ID_SHM_M, D_SZ_SHM_M);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02520, KDDS02520), "shmget", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%08x,%i", D_EID_21005, key, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_UNKNOWN);
	    break;
	}
    }

    agt.main = (struct d_maintbl *)shmat(agt.shmid_m, (char *)0, 0);
    if (agt.main == (struct d_maintbl *)-1) {
	sprintf(msg.wkbuf, "%i,%08x,%i", D_EID_21006, key, errno);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_exit(D_EXIT_UNKNOWN);
    }


    memcpy(agt.main->tableid, "AGTM", 4);
    agt.main->agt_status = 0;
    agt.main->num_proc = 0;
    agt.main->tracpid = 0;
    agt.main->syscpid = 0;
    agt.main->mainpid = getpid();

    agt.main->num_sysc_segments = 1;
    agt.main->num_sysc =
      (D_SZ_SHM_M-sizeof(struct d_maintbl)) / sizeof(struct d_sysc);

    wsyscp = (struct d_sysc *)((int)agt.main + sizeof(struct d_maintbl));
    for(i = 0; i < agt.main->num_sysc; i++) {
	agt_sysc_add_sysc(&agt.main->frsysctop, &agt.main->frsysclst, wsyscp);
	wsyscp++;
    }

    agt.main->evtr_size = trc.evttrsize;
    agt.main->evtr_files = trc.evttrfiles;
    agt.main->evtr_curfil = 0;
    agt.main->evtr_p = 0;
    agt.main->memtr_size = trc.memtrsize;
    agt.main->memtr_buf = NULL;
    agt.main->memtr_p = 0;


    if (trc.memtrsize != 0) {

	key = ftok(agt.ipcpath, D_ID_SHM_T);
	if (key == -1) {
	    sprintf(msg.wkbuf, "%i,%s,%c", D_EID_21007, agt.ipcpath, D_ID_SHM_T);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_UNKNOWN);
	}
	agt.shmid_t = shmget(key, trc.memtrsize,
			     D_IPC_ACCESS_MODE|IPC_EXCL|IPC_CREAT);
	if (agt.shmid_t == -1) {
	    int wk_shmid;

	    switch (errno) {
	    case EEXIST:
		sprintf(msg.wkbuf, "%i,%08x", errno, key);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02521, KDDS02521), msg.wkbuf);
		AgentMsg(LOG_WARNING, msg.buf);

		wk_shmid = shmget(key, trc.memtrsize, D_IPC_ACCESS_MODE);
		if (wk_shmid == -1) {
		    sprintf(msg.wkbuf, "%i,%08x,%i", D_EID_21008, key, errno);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_exit(D_EXIT_UNKNOWN);
		}
		if (shmctl(wk_shmid, IPC_RMID, 0) == -1) {
		    sprintf(msg.wkbuf, "%i,%08x,%i", D_EID_21009, key, errno);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_exit(D_EXIT_UNKNOWN);
		}
		agt.shmid_t = shmget(key, trc.memtrsize,
				     D_IPC_ACCESS_MODE|IPC_EXCL|IPC_CREAT);
		if (agt.shmid_t == -1) {
		    switch (errno) {
		    case ENOSPC:
		    case ENOMEM:
			sprintf(msg.wkbuf, "%i,%i", D_ID_SHM_T, trc.memtrsize);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02520, KDDS02520), "shmget", strerror(errno), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_exit(D_EXIT_OUTOFRSC);
			break;
		    default:
			sprintf(msg.wkbuf, "%i,%08x,%i", D_EID_21010, key, errno);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_exit(D_EXIT_UNKNOWN);
			break;
		    }
		}
		break;
	    case ENOSPC:
	    case ENOMEM:
		sprintf(msg.wkbuf, "%i,%i", errno, D_ID_SHM_T, trc.memtrsize);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02520, KDDS02520), "shmget", strerror(errno), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_OUTOFRSC);
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%08x,%i", D_EID_21011, key, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_UNKNOWN);
		break;
	    }
	}

	agt.main->memtr_buf =
	    (char *)shmat(agt.shmid_t, (char *)0, 0);
	if (agt.main->memtr_buf == (char *)-1) {
	    sprintf(msg.wkbuf, "%i,%08x,%i", D_EID_21012, key, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_UNKNOWN);
	}
    } else {
	agt.main->memtr_buf = NULL;
    }

    trc.memtr_on = (agt.main->memtr_buf != NULL);
    trc.evttr_on = (agt.main->evtr_files > 0);
    trc.prctr_on = (trc.prctrsize > 0);
}


/**************************************************************************/
/* ID   = agt_init_setup_signals					  */
/* name = Setting signal						  */
/* func = Change default operation of SIGHUP, SIGUSR1, SIGUSR2, SIGTERM,  */
/*        SIGCHLD signals						  */
/* i/o  = none				                                  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_init_setup_signals(void)
{
    agt_sig_action(SIGHUP, agt_sig_ignore);
    agt_sig_action(SIGUSR1, agt_sig_ignore);
    agt_sig_action(SIGUSR2, agt_sig_ignore);
    agt_sig_action(SIGTERM, agt_main_sig_term);
    agt_sig_action(SIGCHLD, agt_main_sig_chld);

    AgentInitTimer();
}


/**************************************************************************/
/* ID	= agt_init_go_background					  */
/* name = Allow background process					  */
/* func = Perform the following tasks:					  */
/*        1.close stdin, stdout						  */
/*        2.close file /dev/null to fill up file identification 0, 1	  */
/*        3.change working directory to the root directory		  */
/*        4.change file creation mode  mask to 0			  */
/*        5.create child process and exit father process		  */
/*        6.creatre thread						  */
/*        7.set process group-id					  */
/* i/o  = none				                                  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_init_go_background(void)
{
    pid_t childpid;


    close(0);
    close(1);

    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);


    if (chdir(D_WORK_DIR) == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_21020, errno);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_exit(D_EXIT_UNKNOWN);
    }


    umask(0000);



    if (!lmain.foreground) {

	if ((childpid = fork()) == 0) {
	    agt.ownpid = getpid();
	    agt.mainprocpid = agt.ownpid;
	    memcpy(&agt.proctype, D_PTYP_MAIN, 4);
	} else if (childpid > 0) {
	    agt_bare_exit(D_EXIT_OK);
	} else {
	    switch (errno) {
	    case EAGAIN:
	    case ENOMEM:
		sprintf(msg.wkbuf, "%s", "MAIN");
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02600, KDDS02600), strerror(errno), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_OUTOFRSC);
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_21021, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_UNKNOWN);
	    }
	}



	agt_sig_init_signal_threads();


	if (setsid() == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21022, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_UNKNOWN);
	}
    }
}


/**************************************************************************/
/* ID	= agt_init_open_uxsock						  */
/* name = Open unix domain socket					  */
/* func = Open unix domain socket for main process			  */
/* i/o  = none				                                  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_init_open_uxsock(void)
{
    sck.ux_fd = agt_sck_open_unixsocket(sck.main_adr.sun_path);
}


/**************************************************************************/
/* ID	= agt_init_create_processes					  */
/* name = Create system process, trace					  */
/* func = Create trace process when necessary. If it succeed, write trace */
/*        information in the stat file. Then create system communication  */
/*        process. If it succeed, write system communication information  */
/*        in the stat file						  */
/* i/o  = none				                                  */
/* return =								  */
/*     	0:abort request accepted					  */
/*      1:child process creation succeed				  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_init_create_processes(void)
{
    pid_t childpid;
#define RBUF_SIZE 8
    int msgtype;
    int rtn;
    int saved_errno;
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    int fdmax;
    int rtnselect;
    int checkpid;
    struct timeval waittime;


    if (agt.main->evtr_files > 0) {
	if ((childpid = fork()) == 0) {
	    agt.ownpid = getpid();
	    memcpy(&agt.proctype, D_PTYP_TRAC, 4);

	    agt_sig_init_signal_threads();
	    agt_trace();

	    sprintf(msg.buf, "%i", D_EID_21023);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	    agt_exit(D_EXIT_UNKNOWN);

	} else if (childpid > 0) {
	    agt.main->num_proc++;
	    agt.main->tracpid = childpid;


intr_loop1:
retry:
	    FD_ZERO(&readfds);
	    FD_ZERO(&writefds);
	    FD_ZERO(&exceptfds);
	    FD_SET(sck.ux_fd,&readfds);
	    fdmax = sck.ux_fd + 1;
	    errno = 0;
	    waittime.tv_sec = 5;
	    waittime.tv_usec = 0;
	    rtnselect = select(fdmax,(int *)&readfds,(int *)&writefds,(int *)&exceptfds,&waittime);
	    switch(rtnselect){
		case EINTR:
			goto retry;
		case 1:
			break;
		case 0:
			if (agt.main->tracpid == 0)
				goto fork_syscom;
			goto intr_loop1;
		default:
			sprintf(msg.wkbuf,"%d,%d",D_EID_21057,errno);
			sprintf(msg.buf,catgets(agt.catd,NL_SETD,MSG02620,KDDS02620),msg.wkbuf);
			AgentMsg(LOG_ERR,msg.buf);
			agt_abort();
	     }

	    memset((char *)&sck.from, '\0', sizeof(sck.from));
	    sck.fromlen = sizeof(sck.from);

	    rtn = pthread_mutex_unlock(&sig_mutex);
	    if (rtn == -1) {
		sprintf(msg.wkbuf, "%i,%i", D_EID_21013, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		agt_abort();
	    }

	    errno = 0;
	    sck.uxrcvlen = recvfrom(sck.ux_fd, sck.uxrcvdata,
				    RBUF_SIZE+4, 0, &sck.from, &sck.fromlen);

	    saved_errno = errno;
	    rtn = pthread_mutex_lock(&sig_mutex);
	    if (rtn == -1) {
		sprintf(msg.wkbuf, "%i,%i", D_EID_21014, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		agt_abort();
	    }

	    errno = saved_errno;
	    if (sck.uxrcvlen == -1) {
		switch (errno) {
		case EINTR:
		    if (agt.main->num_proc == 0) {
			goto fork_syscom;
		    } else {
			goto intr_loop1;
		    }
		default:
		    sprintf(msg.wkbuf, "%i,%i", D_EID_21024, errno);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_exit(D_EXIT_UNKNOWN);
		    break;
		}
	    }


	    AgentTrace(TRACEERR,"UXRECV", sck.uxrcvlen, sck.uxrcvdata, sck.uxrcvlen);

	    msgtype = char_to_num(sck.uxrcvdata, 4);
	    checkpid = char_to_num((sck.uxrcvdata+8),4);
	    sprintf(sck.from.sun_path,"%s%05i",D_DFLT_IPCPATH,checkpid);
	    sck.fromlen = SUN_LEN(&(sck.from));

	    switch (msgtype) {
	    case D_AG_TRDY:
		break;
	    case D_AG_CSTOP:
	    case D_AG_CABORT:
		agt__init_reply_to_command(&sck.from, sck.fromlen, "TRDY",
					   msgtype, D_AG_RT_OK);
		kill(-(agt.mainprocpid), SIGUSR1);
		agt_sleep(1);
		return 0;
	    case D_AG_CMSG:
		agt__init_reply_to_command(&sck.from, sck.fromlen,
					  "TRDY", msgtype, D_AG_RT_INIT);
		goto intr_loop1;
	    default:
		sprintf(msg.wkbuf, "%i,%i,%s,%i", D_EID_21025, msgtype, sck.from.sun_path, sck.uxrcvlen);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
		goto intr_loop1;
	    }


	    agt_stat_add_entry(agt.statfilefd, D_PTYP_TRAC,
			       agt.main->tracpid, NULL, NULL, "", "");

	} else {
	    switch (errno) {
	    case EAGAIN:
	    case ENOMEM:
		sprintf(msg.wkbuf, "%s", "TRAC");
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02600, KDDS02600), strerror(errno), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_OUTOFRSC);
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_21026, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_UNKNOWN);
	    }
	}
    }


fork_syscom:;

    if ((childpid = fork()) == 0) {
	agt.ownpid = getpid();
	memcpy(&agt.proctype, D_PTYP_SYSC, 4);

	agt_sig_init_signal_threads();
	agt_sysc();


	sprintf(msg.wkbuf, "%i", D_EID_21027);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	agt_exit(D_EXIT_UNKNOWN);

    } else if (childpid > 0) {
	agt.main->num_proc++;
	agt.main->syscpid = childpid;

intr_loop2:
	memset((char *)&sck.from, '\0', sizeof(sck.from));
	sck.fromlen = sizeof(sck.from);

	rtn = pthread_mutex_unlock(&sig_mutex);
	if (rtn == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21044, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    agt_abort();
	}

	errno = 0;
	sck.uxrcvlen = recvfrom(sck.ux_fd, sck.uxrcvdata, RBUF_SIZE+4,
				0, &sck.from, &sck.fromlen);

	saved_errno = errno;
	rtn = pthread_mutex_lock(&sig_mutex);
	if (rtn == -1) {
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21045, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    agt_abort();
	}

	errno = saved_errno;
	if (sck.uxrcvlen == -1) {
	    switch (errno) {
	    case EINTR:
		if (agt.main->syscpid > 0) {
		    goto intr_loop2;
		} else {
		    sprintf(msg.wkbuf, "%i", D_EID_21041);
		    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		    AgentMsg(LOG_ERR, msg.buf);
		    agt_exit(D_EXIT_UNKNOWN);
		}
		break;
	    default:
		sprintf(msg.wkbuf, "%i,%i", D_EID_21028, errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_UNKNOWN);
		break;
	    }
	}

	AgentTrace(TRACEERR,"UXRECV", sck.uxrcvlen, sck.uxrcvdata, sck.uxrcvlen);

	msgtype = char_to_num(sck.uxrcvdata, 4);
	checkpid = char_to_num((sck.uxrcvdata+8),4);
	sprintf(sck.from.sun_path,"%s%05i",D_DFLT_IPCPATH,checkpid);
	sck.fromlen = SUN_LEN(&(sck.from));

	switch (msgtype) {
	case D_AG_SRDY:
	    break;
	case D_AG_CSTOP:
	case D_AG_CABORT:
	    agt__init_reply_to_command(&sck.from, sck.fromlen, "SRDY",
				       msgtype, D_AG_RT_OK);
	    kill(-(agt.mainprocpid), SIGUSR1);
	    agt_sleep(1);
	    return 0;
	case D_AG_CMSG:
	    agt__init_reply_to_command(&sck.from, sck.fromlen, "SRDY",
				       msgtype, D_AG_RT_INIT);
	    goto intr_loop2;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%s,%i", D_EID_21029, msgtype, sck.from.sun_path, sck.uxrcvlen);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentTrace(TRACEDEB,"DEBUG ", strlen(msg.buf), msg.buf, 0);
	    goto intr_loop2;
	}


	agt_stat_add_entry(agt.statfilefd, D_PTYP_SYSC,
			   agt.main->syscpid, NULL, NULL, "", "");

    } else {
	switch (errno) {
	case EAGAIN:
	case ENOMEM:
	    sprintf(msg.wkbuf, "%s", "SYSC");
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02600, KDDS02600), strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_OUTOFRSC);
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21030, errno);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_UNKNOWN);
	}
    }

    return 1;
}


/**************************************************************************/
/* ID	= agt_init_statfile						  */
/* name = Create operation information	file				  */
/* func = Create operation inforamtion file and write in information about*/
/*        main process							  */
/* i/o  = none				                                  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_init_statfile(void)
{
    int alloc_size;


    alloc_size = strlen(D_STAT_PATH) + strlen(D_STAT_FILE) +
                  strlen(agt.agent_id) + 2;

    agt.statfilename = (char *)malloc(alloc_size);
    if (agt.statfilename == NULL) {
	switch (errno) {
	case ENOMEM:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21031, alloc_size);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_WARNING, msg.buf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_21032, errno, alloc_size);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_UNKNOWN);
	    break;
	}
    }
    sprintf(agt.statfilename, "%s/%s%s", D_STAT_PATH,
	    D_STAT_FILE, agt.agent_id);


    agt.statfilefd = agt_stat_create_file(agt.statfilename,
				(O_RDWR|O_TRUNC|O_CREAT),
				(S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH));
    if (chown(agt.statfilename, (uid_t)0, (gid_t)0) == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_21053, errno);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_exit(D_EXIT_UNKNOWN);
    }
    if (chmod(agt.statfilename, (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_21054, errno);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_exit(D_EXIT_UNKNOWN);
    }
    agt_stat_add_entry(agt.statfilefd, D_PTYP_MAIN, agt.main->mainpid,
		       NULL, NULL, "", "");
}


/**************************************************************************/
/* ID	= agt__init_reply_to_command					  */
/* name = Send result to command process				  */
/* func = Send information command process about the main process during  */
/*        its initialization 						  */
/* i/o  =     				                                  */
/* 	to     :struct sockaddr_un *:IN:address of command process        */
/*  tolen  :int	 	            :IN:address length			  */
/*  info   :char *		        :IN:command process information	  */
/*  msgtyp :int                 :IN:type of message			  */
/*  rtncode:int		            :IN:result information		  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__init_reply_to_command(struct sockaddr_un *to, int tolen,
				      char *info, int msgtyp, int rtncode)
{
    char sbuf[D_UXMSG_REPLY_MAX];

    long_to_char(&sbuf[0], D_AG_COMRTN);
    long_to_char(&sbuf[4], 4);
    long_to_char(&sbuf[8], rtncode);

    if (sendto(sck.ux_fd, sbuf, D_UXMSG_REPLY_MAX, 0, to, tolen)
	!= D_UXMSG_REPLY_MAX) {
	sprintf(msg.wkbuf, "%i,%i,%s,%i", D_EID_21033, errno, info, msgtyp);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_exit(D_EXIT_UNKNOWN);
    }

    AgentTrace(TRACEERR,"UXSEND", D_UXMSG_REPLY_MAX, sbuf, D_UXMSG_REPLY_MAX);
}


/**************************************************************************/
/* ID   = agt_init_syslog						  */
/* name = Open syslog							  */
/* func = Open syslog file						  */
/* i/o  = none				                                  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_init_syslog(void)
{
    agt_msg_open_syslog();
}


/**************************************************************************/
/* ID 	= agt_init_memory						  */
/* name = Alloc work buffer						  */	
/* func = Perform the following tasks:					  */
/*        1.get name of machine executing agent				  */
/*        2.alloc transmission buffer for socket transmission		  */
/*	  3.create socket transmission address for main process           */
/*	  4.create socket transmission address for trace process          */
/*	  5.create socket transmission address for system                 */
/*          communication process					  */
/* i/o  = none				                                  */
/* return = none						          */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_init_memory(void)
{

    agt.agenthostname = (char *)malloc(D_MAX_HOSTNAME_LEN);
    if (agt.agenthostname == NULL) {
	switch (errno) {
	case ENOMEM:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21034, D_MAX_HOSTNAME_LEN);
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_21035, errno, D_MAX_HOSTNAME_LEN);
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    agt_exit(D_EXIT_UNKNOWN);
	    break;
	}
    }

    if (gethostname(agt.agenthostname, D_MAX_HOSTNAME_LEN) == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_21036, errno);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	agt_exit(D_EXIT_UNKNOWN);
    }



    sck.uxrcvdata = (char *)malloc(D_UXMSG_MAX);
    if (sck.uxrcvdata == NULL) {
	switch (errno) {
	case ENOMEM:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21037, D_UXMSG_MAX);
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_21038, errno, D_UXMSG_MAX);
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    agt_exit(D_EXIT_UNKNOWN);
	    break;
	}
    } else {
	sck.uxrcvbufsize = D_UXMSG_MAX;
    }



    sck.uxsnddata = (char *)malloc(D_UXMSG_MAX);
    if (sck.uxsnddata == NULL) {
	switch (errno) {
	case ENOMEM:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_21039, D_UXMSG_MAX);
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02500, KDDS02500), strerror(errno), msg.wkbuf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_21040, errno, D_UXMSG_MAX);
	    fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    agt_exit(D_EXIT_UNKNOWN);
	    break;
	}
    } else {
	sck.uxsndbufsize = D_UXMSG_MAX;
    }



    sck.main_adr.sun_family = sck.tmpl_adr.sun_family;
    sprintf(sck.main_adr.sun_path, "%s%s",
	    sck.tmpl_adr.sun_path, D_UXSOC_MAIN);
    sck.main_adrlen = sizeof(sck.main_adr.sun_family) + strlen(sck.main_adr.sun_path) + sizeof(sck.main_adr.sun_len) +1;

    sck.trace_adr.sun_family = sck.tmpl_adr.sun_family;
    sprintf(sck.trace_adr.sun_path, "%s%s",
	    sck.tmpl_adr.sun_path, D_UXSOC_TRAC);
    sck.trace_adrlen =
      sizeof(sck.trace_adr.sun_family) + strlen(sck.trace_adr.sun_path) + sizeof(sck.trace_adr.sun_len) +1;

    sck.sysc_adr.sun_family = sck.tmpl_adr.sun_family;
    sprintf(sck.sysc_adr.sun_path, "%s%s",
	    sck.tmpl_adr.sun_path, D_UXSOC_SYSC);
    sck.sysc_adrlen =
      sizeof(sck.sysc_adr.sun_family) + strlen(sck.sysc_adr.sun_path) + sizeof(sck.sysc_adr.sun_len) +1;
}

/**************************************************************************/
/* ID	= agt_init_once							  */
/* name = Create particular data key					  */
/* func = Create particular data key, or mutex to be used by thread. At   */
/*        the same time initializes signal				  */
/* i/o  = none				                                  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_init_once(void)
{
    int rtn;
    sigset_t sigmask;

    rtn = pthread_mutex_init(&sig_mutex, pthread_mutexattr_default);
    if (rtn == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_21015, errno);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	agt_exit(D_EXIT_UNKNOWN);
    }

    rtn = pthread_keycreate(&thr_id, (pthread_destructor_t)agt_do_nothing);
    if (rtn == -1) {
	sprintf(msg.wkbuf, "%i,%i", D_EID_21016, errno);
	fprintf(stderr, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	agt_exit(D_EXIT_UNKNOWN);
    }

    sigfillset(&sigmask);
    sigdelset(&sigmask, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &sigmask, NULL);

}

/**************************************************************************/
/* ID	= agt__init_do_rmfile						  */	
/* name = Delete unnecessary files					  */
/* func = Delete files in /opt/dcelocal/hitachi/dfa/.dfa directory	  */
/* i/o  = none				                                  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__init_do_rmfile(void)
{
	DIR *dirp;
	struct dirent *dp=NULL;

	if (chdir(sck.tmpl_adr.sun_path) == -1){
		sprintf(msg.wkbuf, "%i,%i", D_EID_21061,errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_UNKNOWN);
	}

	if ((dirp = opendir((const char *)sck.tmpl_adr.sun_path)) == NULL){
		sprintf(msg.wkbuf, "%i,%i", D_EID_21060,errno);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_exit(D_EXIT_UNKNOWN);
	}

	while(1){
		errno = 0;
		if ((dp = readdir(dirp)) != NULL){
			if ((strcmp(dp->d_name,".") == 0) || (strcmp(dp->d_name,"..") == 0) || (strcmp(dp->d_name,".dfM") == 0) || (strcmp(dp->d_name,".dfS") == 0) || (strcmp(dp->d_name,".dfT") == 0))
				continue;

			remove(dp->d_name);
			continue;
		}
		else if (errno != 0){
			switch(errno){
				case EINTR:
					continue;
				default:
					sprintf(msg.wkbuf, "%i,%i", D_EID_21062,errno);
					sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
					AgentMsg(LOG_ERR, msg.buf);
					agt_exit(D_EXIT_UNKNOWN);
			}
		}
		if (closedir(dirp) == -1){
			sprintf(msg.wkbuf, "%i,%i", D_EID_21063,errno);
			sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02620, KDDS02620), msg.wkbuf);
			AgentMsg(LOG_ERR, msg.buf);
			agt_exit(D_EXIT_UNKNOWN);
                }
		break;
	}
	agt_bare_exit(D_EXIT_OK);
}
