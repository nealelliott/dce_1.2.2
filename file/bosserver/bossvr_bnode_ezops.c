/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_bnode_ezops.c,v $
 * Revision 1.1.74.1  1996/10/02  17:04:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:57  damon]
 *
 * Revision 1.1.68.3  1994/07/13  22:19:54  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:36  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  19:58:31  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:53:57  mbs]
 * 
 * Revision 1.1.68.2  1994/06/09  13:52:20  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:05  annie]
 * 
 * Revision 1.1.68.2  1994/06/09  13:52:20  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:05  annie]
 * 
 * Revision 1.1.68.1  1994/02/04  20:06:23  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:42  devsrc]
 * 
 * Revision 1.1.66.1  1993/12/07  17:13:02  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:57:18  jaffe]
 * 
 * Revision 1.1.3.3  1993/01/18  19:56:07  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:32:30  cjd]
 * 
 * Revision 1.1.3.2  1992/11/24  15:41:12  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:09:07  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:49:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bossvr_bnode_ezops.c -- routines specific to simple-type bosserver bnodes
 *
 *	(based one AFS 3 code):
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */


RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bosserver/bossvr_bnode_ezops.c,v 1.1.74.1 1996/10/02 17:04:27 damon Exp $")

#include <dcedfs/osi.h>
#include <bbos_err.h>
#include <bbos_bnode.h>

static int ez_timeout(), ez_getstat(), ez_setstat(), ez_delete();
static int ez_procexit(), ez_getstring(), ez_getparm(), ez_restartp();
static int ez_hascore(), ez_isvalid();
struct bnode *ez_create();

#define	SDTIME		60	    /* time in seconds given to a process to evaporate */

struct bnode_ops ezbnode_ops = {
    ez_create,
    ez_timeout,
    ez_getstat,
    ez_setstat,
    ez_delete,
    ez_procexit,
    ez_getstring,
    ez_getparm,
    ez_restartp,
    ez_hascore,
    ez_isvalid,
};
    
struct ezbnode {
    struct bnode b;
    long zapTime;		/* time we sent a sigterm */
    char *command;
    struct bnode_proc *proc;
    long lastStart;		/* time last started process */
    char waitingForShutdown;	/* have we started any shutdown procedure? */
    char running;   /* is process running? */
    char killSent;  /* have we tried sigkill signal? */
};

static int ez_hascore(abnode)
register struct ezbnode *abnode; {
    char tbuffer[256];

    bnode_CoreName(abnode, (char *) 0, tbuffer);
    if (access(tbuffer, 0) == 0) return 1;
    else return 0;
}

static int ez_restartp (abnode)
register struct ezbnode *abnode; {
    struct bnode_token *tt;
    register long code;
    struct stat tstat;
    
    code = bnode_ParseLine(abnode->command, &tt);
    if (code) return 0;
    if (!tt) return 0;
    code = stat(tt->key, &tstat);
    if (code) {
	bnode_FreeTokens(tt);
	return 0;
    }
    if (tstat.st_ctime > abnode->lastStart) code = 1;
    else code = 0;
    bnode_FreeTokens(tt);
    return code;
}

static int ez_delete(abnode)
struct ezbnode *abnode; {
    osi_Free(abnode->command, strlen(abnode->command) + 1);
    osi_Free(abnode, sizeof(struct ezbnode));
    return 0;
}

struct bnode *ez_create(ainstance, acommand)
char *ainstance;
char *acommand; {
    register struct ezbnode *te;

    te = (struct ezbnode *) osi_Alloc(sizeof(struct ezbnode));
    bzero((char *)te, sizeof(struct ezbnode));
    bnode_InitBnode(te, &ezbnode_ops, ainstance);
    te->command = (char *) osi_Alloc(strlen(acommand)+1);
    strcpy(te->command, acommand);
    return (struct bnode *) te;
}

/* called to SIGKILL a process if it doesn't terminate normally */
static int ez_timeout(abnode)
struct ezbnode *abnode; {
    if (!abnode->waitingForShutdown) return 0;	/* spurious */
    /* send kill and turn off timer */
    bnode_StopProc(abnode->proc, SIGKILL);
    abnode->killSent = 1;
    bnode_SetTimeout(abnode, 0);
    return 0;
}

static int ez_getstat(abnode, astatus)
struct ezbnode *abnode;
long *astatus; {
    register long temp;
    if (abnode->waitingForShutdown) temp = BSTAT_SHUTTINGDOWN;
    else if (abnode->running) temp = BSTAT_NORMAL;
    else temp = BSTAT_SHUTDOWN;
    *astatus = temp;
    return 0;
}

static int ez_setstat(abnode, astatus)
register struct ezbnode *abnode;
long astatus; {
    struct bnode_proc *tp;
    register long code;

    if (abnode->waitingForShutdown) return BZBUSY;
    if (astatus == BSTAT_NORMAL && !abnode->running) {
	/* start up */
	abnode->lastStart = osi_Time();
	code = bnode_NewProc(abnode, abnode->command, (char *) 0, &tp);
	if (code) return code;
	abnode->running = 1;
	abnode->proc = tp;
	return 0;
    }
    else if (astatus == BSTAT_SHUTDOWN && abnode->running) {
	/* start shutdown */
	bnode_StopProc(abnode->proc, SIGTERM);
	abnode->waitingForShutdown = 1;
	bnode_SetTimeout(abnode, SDTIME);
	return 0;
    }
    return 0;
}

static int ez_procexit(abnode, aproc)
struct ezbnode *abnode;
struct bnode_proc *aproc; {
    /* process has exited */
    register long code;

    abnode->waitingForShutdown = 0;
    abnode->running = 0;
    abnode->killSent = 0;
    abnode->proc = (struct bnode_proc *) 0;
    bnode_SetTimeout(abnode, 0);	/* clear timer */
    if (abnode->b.goal)
	code = ez_setstat(abnode, BSTAT_NORMAL);
    else code = 0;
    return code;
}

static int ez_getstring(abnode, abuffer, alen)
struct ezbnode *abnode;
char *abuffer;
long alen;{
    return -1;	    /* don't have much to add */
}

static ez_getparm(abnode, aindex, abuffer, alen)
struct ezbnode *abnode;
long aindex;
char *abuffer;
long alen; {
    if (aindex > 0) return BZDOM;
    strcpy(abuffer, abnode->command);
    return 0;
}

/* validate acommand  -- takes same args as ez_create */
static ez_isvalid(ainstance, acommand)
char *ainstance;
char *acommand; 
{
    /* valid if executable */
    return bnode_CheckExec(acommand);
}
