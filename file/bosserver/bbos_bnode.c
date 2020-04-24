/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bbos_bnode.c,v $
 * Revision 1.1.92.1  1996/10/02  17:03:44  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:45  damon]
 *
 * Revision 1.1.83.1  1994/10/20  19:29:35  sasala
 * 	Implement S12Y in bosserver subcomponent -UYHP: 12618
 * 	[1994/10/20  18:15:15  sasala]
 * 
 * Revision 1.1.81.3  1994/07/13  22:19:49  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:31  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  19:58:24  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:53:51  mbs]
 * 
 * Revision 1.1.81.2  1994/06/09  13:51:53  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:32  annie]
 * 
 * Revision 1.1.81.2  1994/06/09  13:51:53  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:32  annie]
 * 
 * Revision 1.1.81.1  1994/02/04  20:06:10  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:36  devsrc]
 * 
 * Revision 1.1.79.1  1993/12/07  17:12:53  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:55:31  jaffe]
 * 
 * Revision 1.1.4.6  1993/01/18  19:54:54  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:30:40  cjd]
 * 
 * Revision 1.1.4.5  1992/11/24  15:40:22  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:08:11  bolinger]
 * 
 * Revision 1.1.4.4  1992/11/18  17:37:45  jaffe
 * 	Transarc delta: comer-ot5749-bosserver-truncates-config-file 1.1
 * 	  Selected comments:
 * 	    There was a problem with the bosserver where the BosConfig file could
 * 	    be truncated under mysterious circumstances.  I believe the problem
 * 	    has been found.  It turned out the the bosserver was writing out the
 * 	    complete BosConfig file for every bnode created -- even those being
 * 	    created on reading the file at startup.  Thus, if the bosserver was
 * 	    killed after having processed a subset of the BosConfig file, it would
 * 	    have overwritten the existing file with the subset: truncated file.
 * 	    The obvious solution is to not rewrite the file when processing it on
 * 	    startup.
 * 	    In case there are additional problems, an end record with checksum can
 * 	    be enabled by turning on the 0x20 bit of the bosserver debug flag.
 * 	    Added an additional argument to bnode_Create to say whether or not to
 * 	    rewrite the config file.
 * 	[1992/11/17  19:35:06  jaffe]
 * 
 * Revision 1.1.4.3  1992/10/28  21:38:38  jaffe
 * 	Fixed RCSID
 * 	[1992/10/28  21:03:29  jaffe]
 * 
 * Revision 1.1.4.2  1992/10/27  20:08:34  jaffe
 * 	Transarc delta: bab-ot5471-bbos-uninstall-all 1.2
 * 	  Selected comments:
 * 	    The bos command and bosserver will now support a -all flag for
 * 	    removing all versions of the specified file.
 * 	    ot 5471
 * 	    Cosmetic cleanup.
 * 	    Make sure all bossvr_Log messages have newlines.
 * 	Transarc delta: bab-ot5549-bossvr-make-mutex-recursive-on-osf 1.2
 * 	  Selected comments:
 * 	    When the OSF first ported the bosserver to OSF/1, they ifdef'd out the call
 * 	    that made the bosserver's global mutex recursive.  It turns out that
 * 	    this prevented the bos restart command from working on the OSF/1 platform.
 * 	    ot 5549
 * 	    Return a more intelligent return code if an exec fails.
 * 	    Went ahead and removed the need for the bosserver mutex to
 * 	    be recursive altogether.
 * 	[1992/10/27  13:54:06  jaffe]
 * 
 * Revision 1.1.2.2  1992/04/14  03:02:29  mason
 * 	cma_init apparently doesn't unblock the delivery of SIGVTALRM, even
 * 	though it is interested in it.  In cases in which the bosserver has
 * 	to block that signal, it also has to unblock it for the child after
 * 	the fork.
 * 	Unblock the delivery of SIGVTALRM in the child after the fork, in those cases
 * 	in which we blocked it before the fork.
 * 	On our OSF/1 systems, fork does not disable the virtual timer alarm
 * 	in the child.  This caused children of the bosserver to be abnormally
 * 	terminated before cma_init was run to establish a handler for the
 * 	vtalrm signal.  The bosserver now disables vtalrm completely before
 * 	the fork on OSF/1 and re-enables it in the parent after the fork.
 * 	Completely disable vtalrm processing in the parent before the fork on
 * 	OSF/1 and re-enable it in the parent after the fork.
 * 	ot 2552
 * 	[1992/04/10  21:46:29  mason]
 * 
 * Revision 4.7  1992/04/07  13:57:21  bab
 * 	cma_init apparently doesn't unblock the delivery of SIGVTALRM, even
 * 	though it is interested in it.  In cases in which the bosserver has
 * 	to block that signal, it also has to unblock it for the child after
 * 	the fork.
 * 
 * 	Unblock the delivery of SIGVTALRM in the child after the fork, in those cases
 * 	in which we blocked it before the fork.
 * 
 * 	ot 2552
 * 	[from revision 4.6 by delta bab-ot2552-bosserver-fork-vtalrm, revision 1.2]
 * 
 * Revision 1.1  1992/01/19  02:49:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bbos_bnode.c -- generic bosserver bnode routine implementations
 *
 *	(based on AFS 3 code):
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/bosserver/bbos_bnode.c,v 1.1.92.1 1996/10/02 17:03:44 damon Exp $")

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbssmac.h>
#include <dfsbsssvc.h>
#include <dfsbssmsg.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <pthread.h>
#include <bbos_err.h>
#include <bbos_bnode.h>
#include <bossvr_sleep.h>
#include <bbos_pathnames.h>
#include <bossvr_debug.h>
#include <bossvr_trace.h>
#include <dcedfs/icl.h>

int bnode_waiting = 0;
static int bproc_pid=0;			/* pid of waker-upper */
struct bnode *allBnodes=0;	/* list of all bnodes */
struct bnode_proc *allProcs=0;	/* list of all processes for which we're waiting */
static struct bnode_type *allTypes=0;	/* list of registered type handlers */
extern char **environ;			/* env structure */

bnode_stats_t bnode_stats;		/* the child watch thread needs this */

extern struct icl_set *bossvr_iclSetp;	/* ICL event set pointer */

#if defined(AFS_OSF_ENV)
#define BOSSVR_FORK_LEAVES_VTALRM
#endif /* defined(AFS_OSF_ENV) */

/* Remember the name of the process, if any, that failed last */
static RememberProcName(ap)
register struct bnode_proc *ap; {
    register struct bnode *tbnodep;

    tbnodep = ap->bnode;
    if (tbnodep->lastErrorName) {
	osi_Free(tbnodep->lastErrorName, strlen(tbnodep->lastErrorName)+1);
	tbnodep->lastErrorName = (char *) 0;
    }
    if (ap->coreName) {
	tbnodep->lastErrorName = (char *) osi_Alloc(strlen(ap->coreName)+1);
	strcpy(tbnodep->lastErrorName, ap->coreName);
    }
}

/* utility for use by BOP_HASCORE functions to determine where a core file might
 * be stored.
 */
bnode_CoreName(abnode, acoreName, abuffer)
register struct bnode *abnode;
char *acoreName;
char *abuffer; {
    unsigned char *dcesptr;
   
  dcesptr = dce_sprintf(bss_s_admin_dir_core_file, afs_server_admin_dir);
  strcpy(abuffer, dcesptr);
  free(dcesptr);
  if (acoreName) {
	strcat(abuffer, acoreName);
	strcat(abuffer, ".");
    }
    strcat(abuffer, abnode->name);
    return 0;
}

/* save core file, if any */
SaveCore(abnode, aproc)
register struct bnode_proc *aproc;
register struct bnode *abnode; {
    char		tbuffer[256];
    struct stat		tstat;
    register long	code;
    char		oldCoreFileName[BUFSIZ];
    unsigned char *dcesptr;

    dcesptr = dce_sprintf(bss_s_old_admin_dir_corefile_name, afs_server_admin_dir);
    strcpy(oldCoreFileName, dcesptr);
    free(dcesptr);
    code = stat(oldCoreFileName, &tstat);
    if (code) return;
    
    bnode_CoreName(abnode, aproc->coreName, tbuffer);
    code = rename(oldCoreFileName, tbuffer);
}

bnode_GetString(abnode, abuffer, alen)
     register struct bnode *abnode;
     register char *abuffer;
     register long alen;
{
  int		rtnVal = 0;
  static char	routineName[] = "bnode_GetString";

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_0);
  rtnVal = BOP_GETSTRING(abnode, abuffer, alen);
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_1, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

bnode_GetParm(abnode, aindex, abuffer, alen)
     register struct bnode *abnode;
     register long aindex;
     register char *abuffer;
     long alen; 
{
  int		rtnVal = 0;
  static char	routineName[] = "bnode_GetParm";

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_2);
  rtnVal = BOP_GETPARM(abnode, aindex, abuffer, alen);
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_3, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

bnode_GetStat(abnode, astatus)
     register struct bnode *abnode;
     register long *astatus; 
{
  int		rtnVal = 0;
  static char	routineName[] = "bnode_GetStat";

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_4);
  rtnVal = BOP_GETSTAT(abnode, astatus);
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_5, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

bnode_RestartP(abnode)
     register struct bnode *abnode; 
{
  int		rtnVal = 0;
  static char	routineName[] = "bnode_RestartP";

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_6);
  rtnVal = BOP_RESTARTP(abnode);
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_7, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

bnode_Check(abnode)
     register struct bnode *abnode; 
{
  int		rtnVal = 0;
  static char	routineName[] = "bnode_Check";

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_8);

  if (abnode->flags & BNODE_WAIT) {
    abnode->flags &= ~BNODE_WAIT;
    bossvr_Wakeup((long)abnode);
  }
  rtnVal = 0;

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_9, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

/* tell if an instance has a core file */
bnode_HasCore(abnode)
     register struct bnode *abnode; 
{
  int		rtnVal = 0;
  static char	routineName[] = "bnode_HasCore";

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_10);
  rtnVal = BOP_HASCORE(abnode);
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_11, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

/* wait for all bnodes to stabilize */
bnode_WaitAll() 
{
  register struct bnode *tb;
  register long code;
  long stat;
  
  int		rtnVal = 0;
  static char	routineName[] = "bnode_WaitAll";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_12);
 retry:
  for(tb = allBnodes; tb; tb=tb->next) {
    bnode_Hold(tb);
    code = BOP_GETSTAT(tb, &stat);
    if (code) {
      bnode_Release(tb);
      rtnVal = code;
      break;
    }
    if (stat != tb->goal) {
      tb->flags |= BNODE_WAIT;
      bossvr_Sleep((long)tb);
      bnode_Release(tb);
      goto retry;
    }
    bnode_Release(tb);
  }

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_13, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

/* wait until bnode status is correct */
bnode_WaitStatus(abnode, astatus)
     int astatus;
     register struct bnode *abnode;
{
  register long code;
  long		stat;
  int		rtnVal = 0;
  static char	routineName[] = "bnode_WaitStatus";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_14);
  bnode_Hold(abnode);
  while (1) {
    /* get the status */
    code = BOP_GETSTAT(abnode, &stat);
    if (code) {
      rtnVal = code;
      break;
    }
    
    /* otherwise, check if we're done */
    if (stat == astatus) {
      bnode_Release(abnode);
      break;		/* done */
    }
    if (astatus != abnode->goal) {
      bnode_Release(abnode);
      rtnVal = -1;
      break;	/* no longer our goal, don't keep waiting */
    }
    /* otherwise, block */
    abnode->flags |= BNODE_WAIT;
    bossvr_Sleep((long)abnode);
  }

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_15, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

bnode_SetStat(abnode, agoal)
     register struct bnode *abnode;
     register int agoal; 
{
  int		rtnVal = 0;
  static char	routineName[] = "bnode_SetStat";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_16);
  abnode->goal = agoal;
  bnode_Check(abnode);
  BOP_SETSTAT(abnode, agoal);
  abnode->flags &= ~BNODE_ERRORSTOP;

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_17, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

bnode_SetGoal(abnode, agoal)
     register struct bnode *abnode;
     register int agoal; 
{
  int		rtnVal = 0;
  static char	routineName[] = "bnode_SetGoal";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_18);
  abnode->goal = agoal;
  bnode_Check(abnode);

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_19, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

bnode_SetFileGoal(abnode, agoal)
     register struct bnode *abnode;
     register int agoal; 
{
  int		rtnVal = 0;
  static char	routineName[] = "bnode_SetFileGoal";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_20);

  if (abnode->fileGoal != agoal) {
    abnode->fileGoal = agoal;
    WriteBossvrFile(0);
  }
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_21, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

bnode_InitFileGoal(abnode, agoal)
     register struct bnode *abnode;
     register int agoal; 
{
  int		rtnVal = 0;
  static char	routineName[] = "bnode_InitFileGoal";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_22);
  abnode->fileGoal = agoal;

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_23, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

/* apply a function to all bnodes in the system */
int bnode_ApplyInstance(aproc, arock)
     int (*aproc)();
     char *arock; 
{
  register struct bnode *tb, *nb;
  register long code;
  int		rtnVal = 0;
  static char	routineName[] = "bnode_ApplyInstance";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_24);
  
  for(tb = allBnodes; tb; tb=nb) {
    nb = tb->next;
    code = (*aproc) (tb, arock);
    if (code) {
      rtnVal = code;
      break;
    }
  }

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_25, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

struct bnode *bnode_FindInstance (aname)
     register char *aname; 
{
  register struct bnode *tb;

  for(tb=allBnodes;tb;tb=tb->next) {
    if (!strcmp(tb->name, aname)) return tb;
  }
  return (struct bnode *) 0;
}

static struct bnode_type *FindType(aname)
     register char *aname; 
{
  register struct bnode_type *tt;
  
  for(tt=allTypes;tt;tt=tt->next) {
    if (!strcmp(tt->name, aname)) return tt;
  }
  return (struct bnode_type *) 0;
}

bnode_Register(atype, aprocs, anparms)
     char *atype;
     int anparms;	    /* number of parms to create */
     struct bnode_ops *aprocs; 
{
  register struct bnode_type *tt;
  
  for(tt=allTypes;tt;tt=tt->next) {
    if (!strcmp(tt->name, atype)) break;
  }
  if (!tt) {
    tt = (struct bnode_type *) osi_Alloc(sizeof(struct bnode_type));
    bzero((char *)tt, sizeof(struct bnode_type));
    tt->next = allTypes;
    allTypes = tt;
    tt->name = atype;
  }
  tt->ops = aprocs;
  return 0;
}

long bnode_Create(atype, ainstance, abp, ap1, ap2, ap3, ap4, ap5, ap6, updateConfig)
     char *atype;
     char *ainstance;
     struct bnode **abp;
     char *ap1, *ap2, *ap3, *ap4, *ap5, *ap6;
     int updateConfig;		/* write bnode info to BosConfig */
{
  register struct bnode_type *	type;
  register struct bnode *	tb;
  static char			routineName[] = "bnode_Create";
  long				rtnVal = 0;

  IMPORT pthread_cond_t		bnodeTimeoutChangeCond;

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_26);

  if (bnode_FindInstance(ainstance)) {
    rtnVal = BZEXISTS;
  }
  else {
    type = FindType(atype);
    if (!type) {
      rtnVal = BZBADTYPE;
    }
    else {
      tb = (*type->ops->create)(ainstance, ap1, ap2, ap3, ap4, ap5, ap6);
      if (!tb) {
	rtnVal = BZNOCREATE;
      }
      else {
	*abp = tb;
	tb->type = type;
	bnode_SetStat(tb, tb->goal);    /* nudge it once */
	if (updateConfig)
	    WriteBossvrFile(0);		/* need to update BosConfig */
	
	/* if this bnode needs a timeout, tell the bnode timeout thread about it */
	if ((tb->flags & BNODE_NEEDTIMEOUT) != 0) {
	  if (pthread_cond_signal(&bnodeTimeoutChangeCond) != 0) {
	    bossvr_SvcLog(bss_s_error_signalling_bnode_timeout, routineName, errno);
	  }	/* end if signal error */
	}	/* end if bnode needs timeout */
      }	/* end else (if creation succeeded) */
    }	/* end else (if type found) */
  }	/* end else (if instance found) */
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_27, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

int bnode_DeleteName(ainstance)
     char *ainstance; 
{
  register struct bnode *tb;
  
  int		rtnVal = 0;
  static char	routineName[] = "bnode_DeleteName";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_28);
  tb = bnode_FindInstance(ainstance);
  if (!tb) {
    rtnVal = BZNOENT;
  }
  else {
    rtnVal = bnode_Delete(tb);
  }
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_29, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

bnode_Hold(abnode)
     register struct bnode *abnode; 
{
  abnode->refCount++;
  return 0;
}

bnode_Release(abnode)
     register struct bnode *abnode; 
{
  abnode->refCount--;
  if (abnode->refCount == 0 && abnode->flags & BNODE_DELETE) {
    abnode->flags &= ~BNODE_DELETE;	/* we're going for it */
    bnode_Delete(abnode);
  }
  return 0;
}

int bnode_Delete(abnode)
     register struct bnode *abnode; 
{
  register long code;
  register struct bnode **lb, *ub;
  long 		temp;
  int		rtnVal = 0;
  static char	routineName[] = "bnode_Delete";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_30);
  if (abnode->refCount != 0) {
    abnode->flags |= BNODE_DELETE;
  }
  else {
    /* make sure the bnode is idle before zapping */
    bnode_Hold(abnode);
    code = BOP_GETSTAT(abnode, &temp);
    bnode_Release(abnode);
    if (code) {
      rtnVal = code;
    }
    else {
      if (temp != BSTAT_SHUTDOWN) {
	rtnVal = BZBUSY;
      }
      else {
	/* all clear to zap */
	for(lb = &allBnodes, ub = *lb; ub; lb= &ub->next, ub = *lb) {
	  if (ub == abnode) {
	    /* unthread it from the list */
	    *lb = ub->next;
	    break;
	  }
	}
	/* do this first, since bnode fields may be bad after BOP_DELETE */
	osi_Free(abnode->name, strlen(abnode->name)+1);
	
	rtnVal = BOP_DELETE(abnode);	/* don't play games like holding over this one */
	WriteBossvrFile(0);
      }
    }
  }
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_31, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

/* function to tell if there's a timeout coming up */
int bnode_PendingTimeout(abnode)
     register struct bnode *abnode; 
{
  int		rtnVal = 0;
  static char	routineName[] = "bnode_PendingTimeout";

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_32);
  rtnVal = (abnode->flags & BNODE_NEEDTIMEOUT);

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_33, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

/* function called to set / clear periodic bnode wakeup times */
int bnode_SetTimeout(abnode, atimeout)
     register struct bnode *abnode;
     long atimeout; 
{
  int			rtnVal = 0;
  static char		routineName[] = "bnode_SetTimeout";
  IMPORT pthread_cond_t	bnodeTimeoutChangeCond;

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_34);
  if (atimeout != 0) {
    abnode->nextTimeout = osi_Time() + atimeout;
    abnode->flags |= BNODE_NEEDTIMEOUT;
    abnode->period = atimeout;

    if (pthread_cond_signal(&bnodeTimeoutChangeCond) != 0) {
      bossvr_SvcLog(bss_s_error_signalling_bnode_timeout_in_setting_it,
		    routineName, errno);
    }
  }
  else {
    abnode->flags &= ~BNODE_NEEDTIMEOUT;
  }
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_35, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

/* used by new bnode creation code to format bnode header */
int bnode_InitBnode (abnode, abnodeops, aname)
     register struct bnode *abnode;
     char *aname;
     struct bnode_ops *abnodeops; 
{
  struct bnode **lb, *nb;
  
  int		rtnVal = 0;
  static char	routineName[] = "bnode_InitBnode";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_36);
  /* format the bnode properly */
  bzero((char *)abnode, sizeof(struct bnode));
  abnode->ops = abnodeops;
  abnode->name = (char *) osi_Alloc(strlen(aname)+1);
  strcpy(abnode->name, aname);
  abnode->flags = BNODE_ACTIVE;
  abnode->fileGoal = BSTAT_NORMAL;
  abnode->goal = BSTAT_SHUTDOWN;
  
  /* put the bnode at the end of the list so we write bnode file in same order */
  for(lb = &allBnodes, nb = *lb; nb; lb = &nb->next, nb = *lb);
  *lb = abnode;
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_37, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

/* intialize the whole system */

int bnode_Init() 
{
  long junk;
  register long code = 0;
  
  static initDone = 0;
  
  if (initDone) return 0;
  initDone = 1;
  bzero((char *)&bnode_stats, sizeof(bnode_stats));
  return code;
}
    
/* free token list returned by parseLine */
bnode_FreeTokens(alist)
    register struct bnode_token *alist; {
    register struct bnode_token *nlist;
    for(; alist; alist = nlist) {
	nlist = alist->next;
	osi_Free(alist->key, strlen(alist->key)+1);
	osi_Free(alist, sizeof(struct bnode_token));
    }
    return 0;
}

static space(x)
int x; {
    if (x == 0 || x == ' ' || x == '\t' || x== '\n') return 1;
    else return 0;
}

bnode_ParseLine(aline, alist)
    char *aline;
    struct bnode_token **alist; {
    char tbuffer[256];
    register char *tptr;
    int inToken;
    struct bnode_token *first, *last;
    register struct bnode_token *ttok;
    register int tc;
    
    inToken = 0;	/* not copying token chars at start */
    first = (struct bnode_token *) 0;
    last = (struct bnode_token *) 0;
    while (1) {
	tc = *aline++;
	if (tc == 0 || space(tc)) {    /* terminating null gets us in here, too */
	    if (inToken) {
		inToken	= 0;	/* end of this token */
		*tptr++ = 0;
		ttok = (struct bnode_token *) osi_Alloc(sizeof(struct bnode_token));
		ttok->next = (struct bnode_token *) 0;
		ttok->key = (char *) osi_Alloc(strlen(tbuffer)+1);
		strcpy(ttok->key, tbuffer);
		if (last) {
		    last->next = ttok;
		    last = ttok;
		}
		else last = ttok;
		if (!first) first = ttok;
	    }
	}
	else {
	    /* an alpha character */
	    if (!inToken) {
		tptr = tbuffer;
		inToken = 1;
	    }
	    if (tptr - tbuffer >= sizeof(tbuffer)) return -1;   /* token too long */
	    *tptr++ = tc;
	}
	if (tc == 0) {
	    /* last token flushed 'cause space(0) --> true */
	    if (last) last->next = (struct bnode_token *) 0;
	    *alist = first;
	    return 0;
	}
    }
}

/* check first token to see whether or not it line is executable */
int bnode_CheckExec(aline)
  char *aline;
{
    register char *p;
    register char *q;
    char c;
    int rtnVal;

    /* get first token from line -- skip leading spaces */
    for(p = aline; *p && space(*p); p++)
	;
    for(q = p; *p && !space(*p); p++)
	;

    if (*q)
    {
	c = *p;
	*p = '\0';
	rtnVal = (access(q, X_OK) != 0) ? BZNOTFOUND : 0;
	*p = c;
    }
    else
	rtnVal = BZNOENT;

    return rtnVal;
} 

#define	MAXVARGS	    128
/* actually do the exec, called only in the subprocess */
int bnode_DoExec(alist)
     struct bnode_token *alist;
{
  int			rtnVal = 0;
  char *		argv[MAXVARGS];
  register struct bnode_token	*tt;
  register int		i;
  static char		routineName[] = "bnode_DoExec";

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_38);

  /* close random fd's */
  for(i=3;i<64;i++)  {
    if((rtnVal = fcntl(i, F_GETFD, 0)) < 0) {
      bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
		      ("fcntl failed, %d", errno));
      continue;
    }
    else if (rtnVal & 0x1 == 0x1) {
      bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
		      ("close on exec already set"));
      continue;
    }
    else {
      bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
		      ("close on exec being set"));
      rtnVal |= FD_CLOEXEC;
      (void)fcntl(i, F_SETFD, rtnVal);
    }
  }

  /* convert linked list of tokens into argv structure */
  for(tt=alist,i=0; i<=MAXVARGS && tt; tt=tt->next,i++) {
    argv[i] = tt->key;
  }
  argv[i] = (char *) 0;   /* null-terminated */
  
  /* name of guy to call is argv[0] */
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s: about to exec %s with %d args", routineName, alist->key, i));
  icl_Trace2(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_39, ICL_TYPE_STRING, alist->key, ICL_TYPE_LONG, i);
  execve(alist->key, argv, environ);	/* shouldn't return */
  /* if we get to here, the exec failed; indicate why in the rtnVal */
  rtnVal = errno;

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s: about to return %d (this is an error condition!)",
	    routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_40, ICL_TYPE_LONG, rtnVal);
  return rtnVal;
}

int bnode_NewProc(abnode, aexecString, coreName, aproc)
     struct bnode_proc **aproc;
     char *coreName;
     struct bnode *abnode;
     char *aexecString; 
{
  struct bnode_token *tlist;
  register long code;
  register struct bnode_proc *tp;
  
#if defined(BOSSVR_FORK_LEAVES_VTALRM)
  sigset_t		oldMask;
  sigset_t		newMask;
  struct itimerval	disabledTimer;		/* always remains zeroed */
  struct itimerval	currentTimer;
#endif /* defined(BOSSVR_FORK_LEAVES_VTALRM) */

  int			rtnVal = 0;
  static char		routineName[] = "bnode_NewProc";

  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered, bnode name: %s",
	    routineName, (abnode->name) ? abnode->name : ""));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_41, ICL_TYPE_STRING, ((abnode->name)?abnode->name:""));

  code = bnode_ParseLine(aexecString, &tlist);  /* try parsing first */
  if (code) {
    rtnVal = code;
  }
  else {
    tp = (struct bnode_proc *) osi_Alloc(sizeof(struct bnode_proc));
    bzero((char *)tp, sizeof(struct bnode_proc));
    tp->next = allProcs;
    allProcs = tp;
    *aproc = tp;
    tp->bnode = abnode;
    tp->comLine = aexecString;
    tp->coreName = coreName;	/* may be null */
    abnode->procStartTime = osi_Time();
    abnode->procStarts++;

    /*
     * Note that the signal manipulation on only really necessary on platforms
     * that don't disble vtlarm for the child on fork, such as OSF/1.  On other
     * platforms, this processing is unnecessary, but won't hurt anything.
     */
    /*
     * First, we temporarily block SIGVTALRM to keep it from hitting the child.
     * We do this in the parent to avoid a race condition.
     */
#if defined(BOSSVR_FORK_LEAVES_VTALRM)
    if ((sigemptyset(&newMask) == 0) &&
	(sigaddset(&newMask, SIGVTALRM) == 0) && 
	(sigprocmask(SIG_BLOCK, &newMask, (sigset_t *)NULL) == 0)) {

      /* clear out the disabled timer */
      timerclear(&(disabledTimer.it_interval));
      timerclear(&(disabledTimer.it_value));

      /* get the current itimer & stash it away to be reused */
      if (setitimer(ITIMER_VIRTUAL, &disabledTimer, &currentTimer) != 0) {
	bossvr_SvcLog(bss_s_parent_failed_to_disable_virt_itimer,
		      routineName, errno);
      }
#endif /* defined(BOSSVR_FORK_LEAVES_VTALRM) */

      code = fork();
      if (code < 0) {
	rtnVal = errno;
	bossvr_SvcLog(bss_s_Failed_to_fork_bnode_creating_process, abnode->name);
	bnode_FreeTokens(tlist);
	osi_Free(tp, sizeof(struct bnode_proc));
      }
      else {
	if (code == 0) {
#if defined(BOSSVR_FORK_LEAVES_VTALRM)
	  /* unblock vtalrm in case the child wants it */
	  if (sigprocmask(SIG_UNBLOCK, &newMask, (sigset_t *)NULL) != 0) {
	    bossvr_SvcLog(bss_s_child_error_unblocking_SIGVTALRM,
			  routineName, errno);
	    exit(errno);
	  }
#endif /* defined(BOSSVR_FORK_LEAVES_VTALRM) */

	  /* child, just exec the right stuff */
	  code = bnode_DoExec(tlist);
	  bossvr_SvcLog(bss_s_child_exiting_with_err_due_to_exec_failure,
			routineName, code);
	  exit(code);
	}	    
#if defined(BOSSVR_FORK_LEAVES_VTALRM)
	else {
	  /* parent, restore old processing of VTALRM */
	  if (setitimer(ITIMER_VIRTUAL, &currentTimer, (struct itimerval *)NULL) == 0) {
	    if (sigprocmask(SIG_UNBLOCK, &newMask, (sigset_t *)NULL) != 0) {
	      bossvr_SvcLog(bss_s_parent_error_unblocking_SIGVTALRM,
			    routineName, errno);
	      rtnVal = errno;
	    }
	  }
	  else {
	    bossvr_SvcLog(bss_s_error_reenabling_virt_itimer, routineName, errno);
	    rtnVal = errno;
	  }
	}
#endif /* defined(BOSSVR_FORK_LEAVES_VTALRM) */
      }

#if defined(BOSSVR_FORK_LEAVES_VTALRM)
    }
    else {
      bossvr_SvcLog(bss_s_failed_to_block_SIGVTALRM, routineName, errno);
      rtnVal = errno;
    }
#endif /* defined(BOSSVR_FORK_LEAVES_VTALRM) */

    if (rtnVal == 0) {
      bnode_FreeTokens(tlist);
      tp->pid = code;
      tp->flags = BPROC_STARTED;
      tp->flags &= ~BPROC_EXITED;
      bnode_Check(abnode);
    }
  }  

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s: returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_42, ICL_TYPE_LONG, rtnVal);
  return rtnVal;
}

int bnode_StopProc(aproc, asignal)
register struct bnode_proc *aproc;
int asignal; 
{
  int		rtnVal = 0;
  static char	routineName[] = "bnode_StopProc";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_43);

  if (!(aproc->flags & BPROC_STARTED) || (aproc->flags & BPROC_EXITED)) {
    rtnVal = BZNOTACTIVE;
  }
  else {
    rtnVal = kill(aproc->pid, asignal);
    bnode_Check(aproc->bnode);
  }
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s: returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_44, ICL_TYPE_LONG, rtnVal);
  return rtnVal;
}

int bnode_Deactivate(abnode)
     register struct bnode *abnode; 
{
  register struct bnode **pb, *tb;
  struct bnode *nb;
  int		rtnVal = BZNOENT;
  static char	routineName[] = "bnode_Deactivate";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s entered", routineName));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_45);
  if (!(abnode->flags & BNODE_ACTIVE)) {
    rtnVal = BZNOTACTIVE;
  }
  else {
    for(pb = &allBnodes,tb = *pb; tb; tb=nb) {
      nb = tb->next;
      if (tb == abnode) {
	*pb = nb;
	tb->flags &= ~BNODE_ACTIVE;
	rtnVal = 0;
	break;
      }
    }
  }
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	   ("%s: returning %d", routineName, rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_46, ICL_TYPE_LONG, rtnVal);
  return rtnVal;
}

int DeleteProc(abproc)
     register struct bnode_proc *abproc; 
{
  register struct bnode_proc **pb, *tb;
  struct bnode_proc *nb;
  
  for(pb = &allProcs,tb = *pb; tb; pb = &tb->next, tb=nb) {
    nb = tb->next;
    if (tb == abproc) {
      *pb = nb;
      osi_Free(tb, sizeof(struct bnode_proc));
      return 0;
    }
  }
  return BZNOENT;
}

/*
 * Call vnode-specific validation routine */
long bnode_IsValid(atype, ainstance, ap1, ap2, ap3, ap4, ap5, ap6)
     char *atype;
     char *ainstance;
     char *ap1, *ap2, *ap3, *ap4, *ap5, *ap6;
{
    register struct bnode_type *	type;
    static char			routineName[] = "bnode_IdValid";
    long				rtnVal = 0;
    
    bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
		    ("%s entered", routineName));
    icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_47);
    
    type = FindType(atype);
    if (!type) {
	rtnVal = BZBADTYPE;
    }
    else {
	rtnVal = (*type->ops->isvalid)(ainstance, ap1, ap2, ap3, ap4, ap5, ap6);
    }
    
    bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
		    ("%s returning %d", routineName, rtnVal));
    icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_BNODE_BNODE_48, ICL_TYPE_LONG, rtnVal);
    
    return rtnVal;
}

