/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_bnode_cronops.c,v $
 * Revision 1.1.77.1  1996/10/02  17:04:26  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:56  damon]
 *
 * Revision 1.1.69.1  1994/10/20  19:29:42  sasala
 * 	Implement S12Y in bosserver subcomponent -UYHP: 12618
 * 	[1994/10/20  18:15:22  sasala]
 * 
 * Revision 1.1.67.3  1994/07/13  22:19:53  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:35  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  19:58:29  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:53:56  mbs]
 * 
 * Revision 1.1.67.2  1994/06/09  13:52:18  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:03  annie]
 * 
 * Revision 1.1.67.2  1994/06/09  13:52:18  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:03  annie]
 * 
 * Revision 1.1.67.1  1994/02/04  20:06:22  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:41  devsrc]
 * 
 * Revision 1.1.65.1  1993/12/07  17:13:00  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:57:05  jaffe]
 * 
 * Revision 1.1.3.4  1993/01/18  19:56:03  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:32:23  cjd]
 * 
 * Revision 1.1.3.3  1992/12/09  19:19:58  jaffe
 * 	Transarc delta: comer-ot6105-cleanup-cronops 1.1
 * 	  Selected comments:
 * 	    The cron_setstat code was using if (astatus = BSTAT_NORMAL), not
 * 	    if (astatus == BSTAT_NORMAL).
 * 	    See above.
 * 	[1992/12/03  20:47:51  jaffe]
 * 
 * Revision 1.1.3.2  1992/11/24  15:41:06  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:09:01  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:49:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bossvr_bnode_cronops.c -- routines specific to cron-type bosserver bnodes
 *
 *	(based on AFS 3 code):
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>


RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bosserver/bossvr_bnode_cronops.c,v 1.1.77.1 1996/10/02 17:04:26 damon Exp $")

#include <dcedfs/osi.h>
#include <time.h>
#include <ctype.h>
#include <dcedfs/ktime.h>
#include <bbos_err.h>
#include <bbos_bnode.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbssmac.h>
#include <dfsbsssvc.h>
#include <dfsbssmsg.h>

static int cron_timeout(), cron_getstat(), cron_setstat(), cron_delete();
static int cron_procexit(), cron_getstring(), cron_getparm(), cron_restartp();
static int cron_hascore(), cron_isvalid();
struct bnode *cron_create();
extern char *ktime_DateOf();

#define	SDTIME		60	    /* time in seconds given to a process to evaporate */

struct bnode_ops cronbnode_ops = {
    cron_create,
    cron_timeout,
    cron_getstat,
    cron_setstat,
    cron_delete,
    cron_procexit,
    cron_getstring,
    cron_getparm,
    cron_restartp,
    cron_hascore,
    cron_isvalid,
};
    
struct cronbnode {
    struct bnode b;
    long zapTime;		/* time we sent a sigterm */
    char *command;
    char *whenString;		/* string rep. of when to run */
    struct bnode_proc *proc;
    long lastStart;		/* time last started process */
    long nextRun;		/* next time to run, if no proc running */
    struct ktime whenToRun;	/* high-level rep of when should we run this guy */
    long when;			/* computed at creation time and procexit time */
    char everRun;		/* true if ever ran */
    char waitingForShutdown;	/* have we started any shutdown procedure? */
    char running;   /* is process running? */
    char killSent;  /* have we tried sigkill signal? */
};

static int cron_hascore(abnode)
register struct cronbnode *abnode; {
    char tbuffer[256];

    bnode_CoreName(abnode, (char *) 0, tbuffer);
    if (access(tbuffer, 0) == 0) return 1;
    else return 0;
}

/* run at creation or after process exit.  figures out if we're all done (if a
    one shot run) or when we should run again.  Sleeps until we should run again.
    Note that the computation of when we should run again is made in procexit
    and/or create procs.  This guy only schedules the sleep */
ScheduleCronBnode(abnode)
register struct cronbnode *abnode; {
    register long code;
    register long temp;
    struct bnode_proc *tp;

    if (!(abnode->b.goal))
	return 0;	/* not supposed to run yet */
    /* otherwise we're supposed to be running, figure out when */
    if (abnode->when == 0) {
	/* one shot */
	if (abnode->everRun) {
	    /* once is enough */
	    bnode_Delete(abnode);
	    return 0;
	}
	/* otherwise start it */
	if (!abnode->running) {
	    /* start up */
	    abnode->lastStart = osi_Time();
	    code = bnode_NewProc(abnode, abnode->command, (char *) 0, &tp);
	    if (code) {
		bossvr_SvcLog(bss_s_cron_bnode_nostart1, abnode->b.name, code);
		return code;
	    }
	    abnode->everRun = 1;
	    abnode->running = 1;
	    abnode->proc = tp;
	    return 0;
	}
    }
    else {
	/* run periodically */
	if (abnode->running) return 0;
	/* otherwise find out when to run it, and do it then */
	temp = abnode->when - osi_Time();
	if (temp < 1) temp = 1;	/* temp is when to start dude */
	bnode_SetTimeout(abnode, temp);
    }
}

static int cron_restartp (abnode)
register struct cronbnode *abnode; {
    return 0;
}

static int cron_delete(abnode)
struct cronbnode *abnode; {
    osi_Free(abnode->command, strlen(abnode->command) + 1);
    osi_Free(abnode->whenString, strlen(abnode->whenString) + 1);
    osi_Free(abnode, sizeof(struct cronbnode));
    return 0;
}

struct bnode *cron_create(ainstance, acommand, awhen)
char *ainstance;
char *awhen;
char *acommand; {
    register struct cronbnode *te;
    register long code;

    te = (struct cronbnode *) osi_Alloc(sizeof(struct cronbnode));
    bzero((char *)te, sizeof(struct cronbnode));
    code = ktime_ParsePeriodic(awhen, &te->whenToRun);
    if (code < 0) {
	osi_Free(te, sizeof(struct cronbnode));
	return (struct bnode *) 0;
    }
    bnode_InitBnode(te, &cronbnode_ops, ainstance);
    te->when = ktime_next(&te->whenToRun, 0);
    te->command = (char *) osi_Alloc(strlen(acommand)+1);
    strcpy(te->command, acommand);
    te->whenString = (char *) osi_Alloc(strlen(awhen)+1);
    strcpy(te->whenString, awhen);
    return (struct bnode *) te;
}

/* called to SIGKILL a process if it doesn't terminate normally.  In cron, also
    start up a process if it is time and not already running */
static int cron_timeout(abnode)
struct cronbnode *abnode; {
    register long temp;
    register long code;
    struct bnode_proc *tp;

    if (!abnode->running) {
	if (abnode->when == 0) return 0;    /* spurious timeout activation */
	/* not running, perhaps we should start it */
	if (osi_Time() >= abnode->when) {
	    abnode->lastStart = osi_Time();
	    bnode_SetTimeout(abnode, 0);
	    code = bnode_NewProc(abnode, abnode->command, (char *) 0, &tp);
	    if (code) {
		bossvr_SvcLog(bss_s_cron_bnode_nostart2, abnode->b.name, code);
		return code;
	    }
	    abnode->everRun = 1;
	    abnode->running = 1;
	    abnode->proc = tp;
	}
	else {
	    /* woke up too early, try again */
	    temp = abnode->when - osi_Time();
	    if (temp < 1) temp = 1;
	    bnode_SetTimeout(abnode, temp);
	}
    }
    else {
	if (!abnode->waitingForShutdown) return 0;	/* spurious */
	/* send kill and turn off timer */
	bnode_StopProc(abnode->proc, SIGKILL);
	abnode->killSent = 1;
	bnode_SetTimeout(abnode, 0);
    }
    return 0;
}

static int cron_getstat(abnode, astatus)
struct cronbnode *abnode;
long *astatus; {
    register long temp;
    if (abnode->waitingForShutdown) temp = BSTAT_SHUTTINGDOWN;
    else if (abnode->b.goal == 0) temp = BSTAT_SHUTDOWN;
    else if (abnode->everRun && abnode->when == 0 && !abnode->running) {
	/* special hack: bnode deletion won't happen if bnode is active, so
	   we make bnodes that are ready to be deleted automatically appear
	   as BSTAT_SHUTDOWN so bnode_Delete is happy. */
	temp = BSTAT_SHUTDOWN;
    }
    else temp = BSTAT_NORMAL;
    *astatus = temp;
    return 0;
}

static int cron_setstat(abnode, astatus)
register struct cronbnode *abnode;
long astatus; {

    if (abnode->waitingForShutdown) return BZBUSY;
    if (astatus == BSTAT_SHUTDOWN && abnode->running) {
	/* start shutdown */
	bnode_StopProc(abnode->proc, SIGTERM);
	abnode->waitingForShutdown = 1;
	bnode_SetTimeout(abnode, SDTIME);
	return 0;
    }
    else if (astatus == BSTAT_NORMAL) {
	ScheduleCronBnode(abnode);
    }
    return 0;
}

static int cron_procexit(abnode, aproc)
struct cronbnode *abnode;
struct bnode_proc *aproc; {
    /* process has exited */

    /* log interesting errors for folks */
    if (aproc->lastSignal) {
	bossvr_SvcLog(bss_s_cron_exit_by_signal, abnode->b.name, aproc->lastSignal);
    }
    else if (aproc->lastExit) {
	bossvr_SvcLog(bss_s_cron_exit_with_code, abnode->b.name, aproc->lastExit);
    }

    abnode->waitingForShutdown = 0;
    abnode->running = 0;
    abnode->killSent = 0;
    abnode->proc = (struct bnode_proc *) 0;

    /* figure out when to run next */
    abnode->when = ktime_next(&abnode->whenToRun, 0);

    /* otherwise compute next time to run */
    ScheduleCronBnode(abnode);
    return 0;
}

static int cron_getstring(abnode, abuffer, alen)
struct cronbnode *abnode;
char *abuffer;
long alen;{
    if (abnode->running) strcpy(abuffer, "running now");
    else if (abnode->when==0) strcpy(abuffer, "waiting to run once");
    else  
         {
         char *dcesptr;
         dcesptr = dce_sprintf(bss_s_next_time_to_run, ktime_DateOf(abnode->when));
         strcpy(abuffer , dcesptr);
         free(dcesptr);
         }
    return 0;
}

static cron_getparm(abnode, aindex, abuffer, alen)
struct cronbnode *abnode;
long aindex;
char *abuffer;
long alen; {
    if (aindex == 0) strcpy(abuffer, abnode->command);
    else if (aindex == 1) {
	strcpy(abuffer, abnode->whenString);
    }
    else return BZDOM;
    return 0;
}

/* validate acommand -- takes same args as cron_create */
static cron_isvalid(ainstance, acommand, awhen)
char *ainstance;
char *awhen;
char *acommand;
{
    /* valid if executable */
    if (*awhen == '\0') {
	return BZNOTIME;	/* need a valid time */
    }
    return bnode_CheckExec(acommand);
}
