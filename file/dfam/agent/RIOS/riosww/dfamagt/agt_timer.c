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
static char *RCSID_agt_timer_c="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/dfamagt/agt_timer.c,v 1.1.2.3 1996/03/12 04:33:29 takano Exp $";
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_timer.c,v $
 * Revision 1.1.2.3  1996/03/12  04:33:29  takano
 * 	Delete 2byte code.
 * 	[1996/03/12  04:33:00  takano]
 *
 * Revision 1.1.2.2  1996/03/09  20:44:23  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:24:14  marty]
 * 
 * Revision 1.1.2.1  1996/02/16  05:30:30  dce-sdc
 * 	First Submit
 * 	[1996/02/16  05:30:02  dce-sdc]
 * 
 * Revision 1.1.2.4  96/01/12  05:02:15  05:02:15  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:01:52  kishi]
 * 
 * Revision 1.1.2.3  1995/10/26  01:22:39  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:22:32  kishi]
 * 
 * Revision 1.1.2.2  1995/09/28  12:36:58  kishi
 * 	Add header comment.
 * 	[1995/09/28  12:36:47  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  06:48:17  kishi
 * 	Initial revision
 * 	[1995/09/09  06:45:16  kishi]
 * 
 * $EndLog$
 */
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <syslog.h>


#include "agt_agent.h"
#include "agt_errid.h"
#include "agt_timer.h"
#include "agt_msg.h"


static void AgentSIGALRM(int sig, int code, struct sigcontext *scp);
static void agt__timer_adjust_time(unsigned int spent);
static void agt__timer_start_timer(void);

struct d_timer_table AgentTimer = {
    NULL,
    NULL,
    0,
    0,
    NULL
};


/**************************************************************************/
/* ID   = AgentInitTimer						  */
/* name = Initialize virtual timer					  */
/* func = Initialize virtual timer. Clear alarm, initialize SIGALRM and   */
/*        call free function to release virtual timer and inverter time   */
/*        structures							  */
/* i/o = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
void AgentInitTimer(void)
{
    struct agt_timer *wp;
    struct agt_timer_chkup *cp;

    for (wp = AgentTimer.wait_list; wp != NULL; wp = wp->next) {
	free(wp);
    }
    for (wp = AgentTimer.done_list; wp != NULL; wp = wp->next) {
	free(wp);
    }
    for (cp = AgentTimer.chkup_list; cp != NULL; cp = cp->next) {
	free(cp);
    }

    AgentTimer.wait_list = NULL;
    AgentTimer.done_list = NULL;
    AgentTimer.chkup_list = NULL;
    AgentTimer.cur_timer = 0;
    AgentTimer.itimer_on = D_FALSE;

    alarm(0);

    agt_sig_action(SIGALRM, AgentSIGALRM);
}


/**************************************************************************/
/* ID   = AgentSetTimer							  */
/* name = Start virtual timer						  */
/* func = Start virtual timer						  */	
/* i/o = 								  */
/*	timep:struct agt_timer *:IN:agt_timer structure			  */
/* return = none							  */
/* note = It is possible that the timer is already set when this function */
/*        is called. When this occurs, use malloc to alloc memory         */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/	
/**************************************************************************/
void AgentSetTimer(struct agt_timer *timep)
{
    struct agt_timer *oldp;
    struct agt_timer *curp;
    unsigned int timeleft;


    if (timep == NULL) {
	AgentTrace(TRACEDEB,"SETTIM", 0, NULL, (int)timep);
    } else {
	AgentTrace(TRACEDEB,"SETTIM", sizeof(struct agt_timer), (char *)timep, (int)timep);
    }


    if (AgentTimer.cur_timer > 0) {
	timeleft = alarm(0);

	agt__timer_adjust_time(
		(unsigned int)(AgentTimer.cur_timer - timeleft));
    }


    if (AgentTimer.wait_list == NULL) {
	AgentTimer.wait_list = timep;
	timep->next = NULL;
    } else {
	curp = AgentTimer.wait_list;
	oldp = NULL;
	while ((curp != NULL) && (curp->seconds <= timep->seconds)) {
	    oldp = curp;
	    curp = curp->next;
	}
	if (oldp == NULL) {
	    timep->next = AgentTimer.wait_list;
	    AgentTimer.wait_list = timep;
	} else {
	    timep->next = oldp->next;
	    oldp->next = timep;
	}
    }


    agt__timer_start_timer();

}


/**************************************************************************/
/* ID	= AgentGetTimer							  */
/* name = Get virtual timer from timeout list				  */
/* func = Get one virtual timer from timeout list. Block/unblock the      */
/*        begining and the end of timeout list				  */
/* io = none								  */
/* return =								  */
/* 	NULL		   :there is no virtual timer with timeout	  */
/*  pointer to agt_timer:pointer to agt_timer structure of virtual timer  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
struct agt_timer *AgentGetTimer(void)
{
    struct agt_timer *top;



    top = AgentTimer.done_list;

    if (top != NULL) {
	AgentTimer.done_list = top->next;
	top->next = NULL;
    }

#ifdef _DEBUG2
    AgentTrace(TRACEDEB,"GETTIM", 0, NULL, (int)top);
    if (top != NULL) {
	AgentTrace(TRACEDEB,"GETTI2", sizeof(struct agt_timer), (char *)top, (int)top);
    }
#endif

    return top;
}


/**************************************************************************/
/* ID	= AgentResetTimer						  */
/* name = Release virtual timer						  */
/* func = Release virtual timer opened by AgentSetTimer function.         */
/*        Call this function even if the timer has already been timed out */
/* io =									  */
/*  timep:struct agt_timer *:IN:agt_timer structure with the desired timer*/
/* return =								  */
/*	0 : timer release succeeded					  */
/*  -1: timer release succeeded (timer was not set)			  */	
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
int AgentResetTimer(struct agt_timer *timep)
{
    struct agt_timer *curp;
    struct agt_timer *oldp;


    if (timep == NULL) {
	AgentTrace(TRACEDEB,"RSTTIM", 0, NULL, (int)timep);
    } else {
	AgentTrace(TRACEDEB,"RSTTIM", sizeof(struct agt_timer), (char *)timep, (int)timep);
    }


    curp = AgentTimer.wait_list;
    oldp = NULL;
    while ((curp != NULL) && (curp != timep)) {
	oldp = curp;
	curp = curp->next;
    }

    if (curp == NULL) {
	curp = AgentTimer.done_list;
	oldp = NULL;
	while ((curp != NULL) && (curp != timep)) {
	    oldp = curp;
	    curp = curp->next;
	}

	if (curp == NULL) {
	} else if (oldp == NULL) {
	    AgentTimer.done_list = timep->next;
	} else {
	    oldp->next = timep->next;
	}
    } else if (oldp == NULL) {
	AgentTimer.wait_list = timep->next;
    } else {
	oldp->next = timep->next;
    }


    if ((AgentTimer.cur_timer > 0) &&
	(AgentTimer.wait_list == NULL) &&
	(! AgentTimer.itimer_on)) {
	alarm(0);

    }

    return (curp == NULL) ? 0 : 1;
}

/**************************************************************************/
/* ID	= AgentSIGALRM							  */
/* name = Routine to execute SIGALRM for virtual timer			  */
/* func = Routine to be executed when transmitting SIGALRM signal. Update */
/*        lasting time of virtual timer and when necessary, call alarm    */
/*        function							  */
/* io =									  */
/*      sig :int                :IN:signal number                         */
/*      code:int                :IN:status code                           */
/*      scp :struct sigcontext *:IN:signal information                    */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void AgentSIGALRM(int sig, int code, struct sigcontext *scp)
{
    struct agt_timer *wp;
    struct agt_timer *lastp;
    struct agt_timer_chkup *chkp;


#ifdef _DEBUG
    sprintf(msg.buf, "SIGALRM");
    AgentTrace(TRACEDEB,"SIGNAL", strlen(msg.buf), msg.buf, 0);
#endif

    agt__timer_adjust_time(AgentTimer.cur_timer);
    AgentTimer.cur_timer = 0;

    if (AgentTimer.wait_list != NULL) {


	for (lastp = AgentTimer.done_list;
	     (lastp != NULL) && (lastp->next != NULL); lastp = lastp->next);

	for (wp = AgentTimer.wait_list;
	     (wp != NULL) && (wp->seconds == 0); wp = wp->next) {



	    AgentTimer.wait_list = wp->next;
	    wp->next = NULL;


	    if (lastp == NULL) {
		AgentTimer.done_list = wp;
	    } else {
		lastp->next = wp;
	    }
	    lastp = wp;
	}
    }


    while ((wp = AgentGetTimer()) != NULL) {
	(*wp->timer_func)(wp);
    }


    AgentTimer.itimer_on = D_FALSE;
    for (chkp = AgentTimer.chkup_list; chkp != NULL; chkp = chkp->next) {


	if ((*chkp->func)()) {

	    AgentTimer.itimer_on = D_TRUE;
	    break;
	}
    }


    agt__timer_start_timer();

}


/**************************************************************************/
/* ID	= agt__timer_adjust_time					  */
/* name = Virtual timer lasting time update				  */
/* func = Update lasting time of virtual timer according to elapsed time  */
/* io =									  */
/*	spent:unsigned int:IN:elapsed time				  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__timer_adjust_time(unsigned int spent)
{
    struct agt_timer *wp;


    for (wp = AgentTimer.wait_list; wp != NULL; wp = wp->next) {

	if (wp->seconds >= spent) {
	    wp->seconds -= spent;
	} else {
	    wp->seconds = 0;
	}
    }
}


/**************************************************************************/
/* ID	= agt__timer_start_timer					  */
/* name = Set alarm							  */
/* func = Set alarm according to alarm system call			  */
/* io = none								  */
/* return = none							  */
/* note = none                                                            */
/* date =  Apr. 22, 1995 by Tetsuya Ichikawa                              */
/*                                                                        */
/* Trade Secret of Hitachi, Ltd.                                          */
/* Do not disclose without written concession from Hitachi, Ltd.          */
/* Copyright 1996 Hitachi, Ltd. All Rights  Reserved. Derechos Reservados.*/
/**************************************************************************/
static void agt__timer_start_timer(void)
{
    if (AgentTimer.itimer_on) {
	alarm(D_ALARM_INTERVAL);
	AgentTimer.cur_timer = D_ALARM_INTERVAL;
    } else if (AgentTimer.wait_list != NULL) {
	alarm(AgentTimer.wait_list->seconds);
	AgentTimer.cur_timer = AgentTimer.wait_list->seconds;
    } else {
	AgentTimer.cur_timer = 0;
    }

}

