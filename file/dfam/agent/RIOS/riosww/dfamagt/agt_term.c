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
static char *RCSID_agt_term_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_term.c,v 1.1.2.2 1996/03/09 20:44:21 marty Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_term.c,v $
 * Revision 1.1.2.2  1996/03/09  20:44:21  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:13  marty]
 *
 * Revision 1.1.2.1  1996/02/16  05:37:39  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:37:14  dce-sdc]
 * 
 * Revision 1.1.2.4  96/01/12  04:59:14  04:59:14  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  04:59:03  kishi]
 * 
 * Revision 1.1.2.3  1995/10/26  01:18:33  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:18:13  kishi]
 * 
 * Revision 1.1.2.2  1995/09/28  12:24:05  kishi
 * 	Add header comment.
 * 	[1995/09/28  12:23:55  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:48:54  kishi
 * 	Initial revision
 * 	[1995/09/09  06:44:22  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#undef close
#undef shutdown
#undef unlink

#include <sys/types.h>
#include <errno.h>
#include <syslog.h>
#include <sys/shm.h>


#include "agt_agent.h"
#include "agt_errid.h"
#include "agt_term.h"
#include "agt_msg.h"
#include "agt_sem.h"


/**************************************************************************/
/* ID	= agt_term_remove_statfile					  */
/* name = Delete operation information file				  */
/* func = Close operation information file and then delete		  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_term_remove_statfile(void)
{
    if (close(agt.statfilefd) == -1) {
	switch (errno) {
	case ENOSPC:
	    sprintf(msg.wkbuf, "%i,%i).\n", D_EID_21201, agt.statfilefd);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02161, KDDS02161), "close", strerror(errno), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_exit(D_EXIT_OUTOFRSC);
	    break;
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_21202, errno, agt.statfilefd);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }

    agt.statfilefd = -1;


    if (unlink(agt.statfilename) == -1) {
	switch (errno) {
	default:
	    sprintf(msg.wkbuf, "%i,%i,%s", D_EID_21203, errno, agt.statfilename);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }
}


/**************************************************************************/
/* ID	= agt_term_close_syslog						  */
/* name = Close syslog file						  */
/* func = Close syslog file						  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_term_close_syslog(void)
{
    closelog();
}


/**************************************************************************/
/* ID	= agt_term_release_shmem					  */
/* name = Release common memory						  */
/* func = Release common area allocated by agent			  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */  
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_term_release_shmem(void)
{
    if ((agt.shmid_t != -1) && (shmctl(agt.shmid_t, IPC_RMID, 0) == -1)) {
	switch (errno) {
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_21204, errno, agt.shmid_t);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    } else {
	agt.shmid_t = -1;
	trc.memtr_on = D_FALSE;
    }


    if (agt.main->num_sysc_segments >= 2) {
	key_t key;
	int wkshmid;


	key = ftok(agt.ipcpath, D_ID_SHM_M2);
	if (key == -1) {
	    goto shmid_m2_done;
	}

	wkshmid = shmget(key, D_SZ_SHM_M, D_IPC_ACCESS_MODE);
	if (wkshmid == -1) {
	    goto shmid_m2_done;
	}

	if (shmctl(wkshmid, IPC_RMID, 0) == -1) {
	    switch (errno) {
	    default:
		sprintf(msg.wkbuf, "%i,%i,%i", D_EID_21206, errno, wkshmid);
		sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
		AgentMsg(LOG_ERR, msg.buf);
		agt_abort();
		break;
	    }
	}
    }

shmid_m2_done:
    if (shmctl(agt.shmid_m, IPC_RMID, 0) == -1) {
	switch (errno) {
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_21205, errno, agt.shmid_m);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    } else {
	agt.shmid_m = -1;
    }

}


/**************************************************************************/
/* ID	= agt_term_close_unixsocket					  */
/* name = Close connection						  */
/* func = Close UNIX domain connection with agent			  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_term_close_unixsocket(void)
{
    if (shutdown(sck.ux_fd, 2) == -1) {
	switch (errno) {
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_21207, errno, sck.ux_fd);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }
    if (close(sck.ux_fd) == -1) {
	switch (errno) {
	default:
	    sprintf(msg.wkbuf, "%i,%i,%i", D_EID_21208, errno, sck.ux_fd);
	    sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	    AgentMsg(LOG_ERR, msg.buf);
	    agt_abort();
	    break;
	}
    }

    if (unlink(sck.myaddr) == -1) {
	sprintf(msg.wkbuf, "%i,%i,%s", D_EID_21209, errno, sck.myaddr);
	sprintf(msg.buf, catgets(agt.catd, NL_SETD, MSG02007, KDDS02007), msg.wkbuf);
	AgentMsg(LOG_ERR, msg.buf);
	agt_abort();
    }

    free(sck.myaddr);
}


/**************************************************************************/
/* ID	= agt_term_remove_semaphore					  */
/* name = Delete semaphore						  */
/* func = Delete semaphore used by agent				  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void agt_term_remove_semaphore(void)
{
    agt_sem_release_semaphore();
}
