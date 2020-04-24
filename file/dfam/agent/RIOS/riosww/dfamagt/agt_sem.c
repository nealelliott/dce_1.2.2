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
static char *RCSID_agt_sem_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_sem.c,v 1.1.2.2 1996/03/09 20:44:13 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_sem.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:13  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:08  marty]
 *
 * Revision 1.1.2.1  1996/02/16  05:50:37  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:50:08  dce-sdc]
 * 
 * Revision 1.1.2.4  96/01/12  04:51:53  04:51:53  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:51:39  kishi]
 * 
 * Revision 1.1.2.3  1995/10/26  01:04:51  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:04:46  kishi]
 * 
 * Revision 1.1.2.2  1995/09/23  09:51:31  kishi
 * 	Add header comment.
 * 	[1995/09/23  09:15:27  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:50:31  kishi
 * 	Initial revision
 * 	[1995/09/09  06:41:22  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include <syslog.h>

#include "agt_agent.h"
#include "agt_errid.h"
#include "agt_sem.h"
#include "agt_msg.h"


/**************************************************************************/
/* ID	= agt_sem_open_semaphore					  */
/* name = Create semaphore						  */
/* func = Create semaphre used among agent processes			  */
/* i/o = none								  */
/* return = none							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_sem_open_semaphore(void)
{
    key_t key;

    key = ftok(agt.ipcpath, D_ID_SEM);
    if (key == -1) {
	sprintf(msg.wkbuf, "%i,%s,%c", D_EID_20021, agt.ipcpath, D_ID_SEM);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02540, KDDS02540), "ftok", strerror(errno), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }
    agt.semid = semget(key, D_NUM_SEM, D_IPC_ACCESS_MODE|IPC_CREAT);
    if (agt.semid == -1) {
	switch (errno) {
	case ENOSPC:
	    sprintf(msg.wkbuf, "%s,%c", agt.ipcpath, D_ID_SEM);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02540, KDDS02540), "semget", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%s,%c", D_EID_20022, agt.ipcpath, D_ID_SEM);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02540, KDDS02540), "semget", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }
}


/**************************************************************************/
/* ID	= agt_sem_release_semaphore					  */
/* name = Release semaphore						  */
/* func = Release semaphre created in agt_sem_open_semaphore		  */
/* i/o = none								  */
/* return = none							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_sem_release_semaphore(void)
{
    if (semctl(agt.semid, 0, IPC_RMID) == -1) {
	switch (errno) {
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_20023, agt.semid);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02540, KDDS02540), "semctl", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    } else {
	agt.semid = -1;
    }
}


/**************************************************************************/
/* ID	= agt_sem_check_if_agent_is_up					  */
/* name = Check if agent is active					  */
/* func = Check semaphore value and certify that agent is active	  */
/* i/o = 								  */
/*	semid:int:IN:semaphore-ID					  */
/* return =								  */
/*	0:agent is stopped						  */
/*  1:agent is active							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_sem_check_if_agent_is_up(int semid)
{
    struct sembuf sops;


    if (semid == -1) {
	return 0;
    }

    sops.sem_num = D_SEM_UP;
    sops.sem_op = 0;
    sops.sem_flg = IPC_NOWAIT;

retry_semop:
    if (semop(semid, &sops, 1) == -1) {
	switch (errno) {
	case EINTR:
	    goto retry_semop;
	case EAGAIN:
	    return 1;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_20024, semid);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02540, KDDS02540), "semop", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }


    return 0;
}


/**************************************************************************/
/* ID	= agt_sem_set_agent_up_flg					  */
/* name = Decrease semaphore value					  */
/* func = Semaphore value is decreased by 1 to show that it is active	  */
/* i/o =								  */
/*  semid:int:IN:semaphore-ID						  */
/* return =								  */
/*	0:abnormal end(agent is already active)				  */
/*	1:normal end							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_sem_set_agent_up_flg(int semid)
{
    struct sembuf sops[2];


    if (semid == -1) {
	return 1;
    }

    sops[0].sem_num = D_SEM_UP;
    sops[0].sem_op = 0;
    sops[0].sem_flg = IPC_NOWAIT;
    sops[1].sem_num = D_SEM_UP;
    sops[1].sem_op = 1;
    sops[1].sem_flg = SEM_UNDO;

retry_semop:
    if (semop(semid, (struct sembuf *)&sops, 2) == -1) {
	switch (errno) {
	case EINTR:
	    goto retry_semop;
	case EAGAIN:
	    return 0;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_20025, errno, semid);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02540, KDDS02540), "semop", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }


    return 1;
}


/**************************************************************************/
/* ID	= agt_sem_set_lock						  */
/* name = Get semaphore for memory trace				  */
/* func = Check semaphore to ceretify that information can be written     */
/*        in memory trace				   		  */
/* i/o =								  */
/* 	semid :int:IN:semaphore-ID					  */
/*  semnum:int:IN:number of members in semaphore set			  */
/* return =								  */
/*	1:normal end							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_sem_set_lock(int semid, int semnum)
{
    struct sembuf sops[2];

    if (semid == -1) {
	return 1;
    }

    sops[0].sem_num = semnum;
    sops[0].sem_op = 0;
    sops[0].sem_flg = 0;
    sops[1].sem_num = semnum;
    sops[1].sem_op = 1;
    sops[1].sem_flg = SEM_UNDO;

retry_semop:
    if (semop(semid, (struct sembuf *)&sops, 2) == -1) {
	switch (errno) {
	case EINTR:
	    goto retry_semop;
	case EIDRM:
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i,%i", D_EID_20026, agt.ownpid, semid, semnum);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02540, KDDS02540), "semop", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }

    return 1;
}


/**************************************************************************/
/* ID	= agt_sem_release_lock						  */
/* name = Release resources controled by semaphore			  */
/* func = Increase semaphore value by 1 and release public resources	  */
/* i/o =								  */
/* 	semid :int:IN:semaphore-ID					  */
/*  semnum:int:IN:number of members in semaphore set			  */
/* return =								  */
/*	1:normal end							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_sem_release_lock(int semid, int semnum)
{
    struct sembuf sops;

    if (semid == -1) {
	return 1;
    }

    sops.sem_num = semnum;
    sops.sem_op = -1;
    sops.sem_flg = SEM_UNDO;

retry_semop:
    if (semop(semid, &sops, 1) == -1) {
	switch (errno) {
	case EINTR:
	    goto retry_semop;
	case EIDRM:
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i,%i", D_EID_20027, agt.ownpid, semid, semnum);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02540, KDDS02540), "semop", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }

    return 1;
}

/**************************************************************************/
/* ID	= agt_sem_wait_agent_gets_down					  */
/* name = Wait untill agent stops					  */
/* func = Check semaphore and wait until agent stops completely           */
/* i/o =								  */
/*	semid :int:IN:semaphore-ID		                          */
/* return =								  */
/*	0:abnormal end							  */
/*	1:normal end							  */
/* note = none								  */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int agt_sem_wait_agent_gets_down(int semid)
{
    struct sembuf sops;


    if (semid == -1) {
	return 1;
    }

    sops.sem_num = D_SEM_UP;
    sops.sem_op = 0;
    sops.sem_flg = 0;

retry_semop:
    if (semop(semid, &sops, 1) == -1) {
	switch (errno) {
	case EINTR:
	    goto retry_semop;
	case EIDRM:
	    return 1;
	default:
	    sprintf(msg.wkbuf, "%i,%i", D_EID_20028, semid);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02540, KDDS02540), "semop", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }


    return 1;
}

