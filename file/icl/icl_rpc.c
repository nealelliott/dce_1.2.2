/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: icl_rpc.c,v $
 * Revision 1.1.39.1  1996/10/02  17:52:20  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:58  damon]
 *
 * Revision 1.1.34.3  1994/07/13  22:25:41  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:07:39  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:42  mbs]
 * 
 * Revision 1.1.34.2  1994/06/09  14:11:33  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:25:17  annie]
 * 
 * Revision 1.1.34.1  1994/02/04  20:21:14  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:15  devsrc]
 * 
 * Revision 1.1.32.1  1993/12/07  17:27:00  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:38:45  jaffe]
 * 
 * $EndLog$
 */
/*
 * icl_rpc.idl -- RPC interface to ICL tracing admin functions
 *
 *	Copyright (C) 1993 Transarc Corporation
 *	All rights reserved.
 */

#include <dce/rpc.h>
#include <dcedfs/stds.h>
#include <dcedfs/icl.h>
#include <icl_rpc.h>
#include <icl_errs.h>

#ifndef KERNEL
#include <pthread.h>
#endif /* !KERNEL */

#define CHECKAUTH(hp)	dfsauth_server_CheckAuthorization(hp, NULL, NULL, NULL)

static
error_status_t SDFSTRACE_GetSetInfo _TAKES((
					    rpc_binding_handle_t	h,
					    unsigned32 all,
					    icl_bulkSetinfo_t *info));
static
error_status_t SDFSTRACE_SetSetInfo _TAKES((
					    rpc_binding_handle_t	h,
					    unsigned32 all,
					    icl_bulkSetinfo_t *info));
static
error_status_t SDFSTRACE_GetLogInfo _TAKES((
					    rpc_binding_handle_t	h,
					    idl_char *setname, 
					    unsigned32 all,
					    icl_bulkLoginfo_t *info));
static
error_status_t SDFSTRACE_SetLogInfo _TAKES((
					    rpc_binding_handle_t	h,
					    unsigned32 all,
					    icl_bulkLoginfo_t *info));
static
 error_status_t SDFSTRACE_ClearSet _TAKES((
					   rpc_binding_handle_t	h,
					   unsigned32 all,
					   icl_bulkSetinfo_t *info));
static
error_status_t SDFSTRACE_ClearLog _TAKES((
					  rpc_binding_handle_t	h,
					  unsigned32 all,
					  icl_bulkLoginfo_t *info));
static
error_status_t SDFSTRACE_DumpSet _TAKES((
					 rpc_binding_handle_t	h,
					 idl_char *filename,
					 unsigned32 all,
					 icl_bulkSetinfo_t *info));
  
/* declare endpoint manager vector */
DFSTRACE_v1_0_epv_t DFSTRACE_v1_0_mgr_epv =
{
    SDFSTRACE_GetSetInfo,
    SDFSTRACE_SetSetInfo,
    SDFSTRACE_GetLogInfo,
    SDFSTRACE_SetLogInfo,
    SDFSTRACE_ClearSet,
    SDFSTRACE_ClearLog,
    SDFSTRACE_DumpSet,
};

static
error_status_t SDFSTRACE_GetSetInfo(h, all, info)
  rpc_binding_handle_t	h;
  unsigned32 all;
  icl_bulkSetinfo_t *info;
{
    register struct icl_set *setp;
    unsigned32 code;
    int i;

    if (code = CHECKAUTH(&h))
	return code;

    code = 0;
    if (all)
    {
	osi_mutex_enter(&icl_lock);
	for(i = 0, setp = icl_allSets; 
	    setp && (i < info->count); 
	    setp = setp->nextp, i++) 
	{
	    (void) strncpy((char *)(info->setinfo[i].setName), setp->name, 
			   sizeof(info->setinfo[i].setName)-1);
	    info->setinfo[i].setName[sizeof(info->setinfo[i].setName)-1] = '\0';
	    info->setinfo[i].states = setp->states;
	}
	osi_mutex_exit(&icl_lock);
	info->count = i;
    }
    else
    {
	for(i = 0; i < info->count; i++)
	{
	    setp = icl_FindSet((char *)(info->setinfo[i].setName));
	    if (!setp)
		info->setinfo[i].states = ICL_ERROR_NOSET;
	    else
	    {
		info->setinfo[i].states = setp->states;
		icl_SetRele(setp);
	    }
	}
    }

    return code;
}

static
error_status_t SDFSTRACE_SetSetInfo(h, all, info)
  rpc_binding_handle_t	h;
  unsigned32 all;
  icl_bulkSetinfo_t *info;
{
    register struct icl_set *setp;
    int i;
    unsigned32 code = 0;

    if (code = CHECKAUTH(&h))
	return code;

    if (all)
    {
	osi_mutex_enter(&icl_lock);
	for(setp = icl_allSets; setp; setp = setp->nextp) 
	{
	    setp->refCount++;	/* hold this guy */
	    osi_mutex_exit(&icl_lock);
	    /* don't set states on persistent sets -- must be done explicitly */
	    if ((setp->states & ICL_SETF_PERSISTENT) == 0)
		code = icl_SetSetStat(setp, info->setinfo[0].states);
	    osi_mutex_enter(&icl_lock);
	    if (--setp->refCount == 0)
		icl_ZapSet_r(setp);
	    if (code) break;
	}
	osi_mutex_exit(&icl_lock);
	info->count = 0;
    }
    else
    {
	for(i = 0; i < info->count; i++)
	{
	    setp = icl_FindSet((char *)(info->setinfo[i].setName));
	    if (!setp)
		info->setinfo[i].states = ICL_ERROR_NOSET;
	    else
	    {
		code = icl_SetSetStat(setp, info->setinfo[i].states);
		info->setinfo[i].states = code;
		icl_SetRele(setp);
	    }
	}
	code = 0;	/* pass code back in structure */
    }

    return code;
}

static
error_status_t SDFSTRACE_GetLogInfo(h, setname, all, info)
  rpc_binding_handle_t	h;
  idl_char setname[32];
  unsigned32 all;
  icl_bulkLoginfo_t *info;
{
    register struct icl_log *logp;
    register struct icl_set *setp;
    unsigned32 code;
    int i;
    int j;

    if (code = CHECKAUTH(&h))
	return code;

    code = 0;
    if (all)
    {
	osi_mutex_enter(&icl_lock);
	for(i = 0, logp = icl_allLogs; 
	    logp && (i < info->count); 
	    logp = logp->nextp, i++) 
	{
	    (void) strncpy((char *)(info->loginfo[i].logName), logp->name, 
			   sizeof(info->loginfo[i].logName)-1);
	    info->loginfo[i].logName[sizeof(info->loginfo[i].logName)-1] = '\0';
	    info->loginfo[i].states = logp->states;
	    info->loginfo[i].logSize = logp->logSize;
	    if (!logp->datap)	/* not allocated? */
		info->loginfo[i].logElements = logp->logSize+1;
	    else
		info->loginfo[i].logElements = logp->logElements;
	}
	osi_mutex_exit(&icl_lock);
	info->count = i;
    }
    else if (setname[0])
    {
	/* enumerate by set */
	setp = icl_FindSet((char *)&setname[0]);
	if (!setp)
	    code = ICL_ERROR_NOSET;
	else
	{
	    osi_mutex_enter(&setp->lock);
	    for(i = 0, j = 0; j < ICL_LOGSPERSET; j++) 
	    {
		logp = setp->logs[j];
		if (logp)
		{
		    osi_mutex_enter(&logp->lock);
		    (void) strncpy((char *)(info->loginfo[i].logName), logp->name, 
				   sizeof(info->loginfo[i].logName));
		    info->loginfo[i].logName[sizeof(info->loginfo[i].logName)-1] = '\0';
		    info->loginfo[i].states = logp->states;
		    info->loginfo[i].logSize = logp->logSize;
		    if (!logp->datap)	/* not allocated? */
			info->loginfo[i].logElements = logp->logSize+1;
		    else
			info->loginfo[i].logElements = logp->logElements;
		    osi_mutex_exit(&logp->lock);
		    i++;
		}
	    }
	    osi_mutex_exit(&setp->lock);
	    icl_SetRele(setp);
	    info->count = i;
	}
    }
    else
    {
	for(i = 0; i < info->count; i++)
	{
	    logp = icl_FindLog((char *)(info->loginfo[i].logName));
	    if (!logp)
		info->loginfo[i].states = ICL_ERROR_NOLOG;
	    else
	    {
		info->loginfo[i].states = logp->states;
		info->loginfo[i].logSize = logp->logSize;
		if (!logp->datap)	/* not allocated? */
		    info->loginfo[i].logElements = logp->logSize+1;
		else
		    info->loginfo[i].logElements = logp->logElements;
		icl_LogRele(logp);
	    }
	}
    }

    return code;
}

static
error_status_t SDFSTRACE_SetLogInfo(h, all, info)
  rpc_binding_handle_t	h;
  unsigned32 all;
  icl_bulkLoginfo_t *info;
{
    register struct icl_log *logp;
    unsigned32 code;
    int i;

    if (code = CHECKAUTH(&h))
	return code;

    code = 0;
    if (all)
    {
	osi_mutex_enter(&icl_lock);
	for(logp = icl_allLogs; logp; logp = logp->nextp) 
	{
	    logp->refCount++;	/* hold this guy */
	    osi_mutex_exit(&icl_lock);
	    code = icl_LogSetSize(logp, info->loginfo[0].logSize);
	    osi_mutex_enter(&icl_lock);
	    if (--logp->refCount == 0)
		icl_ZapLog_r(logp);
	    if (code) break;
	}
	osi_mutex_exit(&icl_lock);
	info->count = 0;
    }
    else
    {
	for(i = 0; i < info->count; i++)
	{
	    logp = icl_FindLog((char *)(info->loginfo[i].logName));
	    if (!logp)
		info->loginfo[i].states = ICL_ERROR_NOLOG;
	    else
	    {
		code = icl_LogSetSize(logp, info->loginfo[0].logSize);
		info->loginfo[i].states = code;
		icl_LogRele(logp);
	    }
	}
	code = 0;	/* return results in states field of structure */
    }

    return code;
}

static
error_status_t SDFSTRACE_ClearSet(h, all, info)
  rpc_binding_handle_t	h;
  unsigned32 all;
  icl_bulkSetinfo_t *info;
{
    register struct icl_set *setp;
    int i;
    unsigned32 code = 0;

    if (code = CHECKAUTH(&h))
	return code;

    if (all)
    {
	osi_mutex_enter(&icl_lock);
	for(setp = icl_allSets; setp; setp = setp->nextp) 
	{
	    setp->refCount++;	/* hold this guy */
	    osi_mutex_exit(&icl_lock);
	    code = icl_ZeroSet(setp);
	    osi_mutex_enter(&icl_lock);
	    if (--setp->refCount == 0)
		icl_ZapSet_r(setp);
	    if (code) break;
	}
	osi_mutex_exit(&icl_lock);
	info->count = 0;
    }
    else
    {
	for(i = 0; i < info->count; i++)
	{
	    setp = icl_FindSet((char *)(info->setinfo[i].setName));
	    if (!setp)
		info->setinfo[i].states = ICL_ERROR_NOSET;
	    else
	    {
		code = icl_ZeroSet(setp);
		info->setinfo[i].states = code;
		icl_SetRele(setp);
	    }
	}
	code = 0;	/* pass code back in structure */
    }

    return code;
}

static
error_status_t SDFSTRACE_ClearLog(h, all, info)
  rpc_binding_handle_t	h;
  unsigned32 all;
  icl_bulkLoginfo_t *info;
{
    register struct icl_log *logp;
    unsigned32 code;
    int i;

    if (code = CHECKAUTH(&h))
	return code;

    code = 0;
    if (all)
    {
	osi_mutex_enter(&icl_lock);
	for(logp = icl_allLogs; logp; logp = logp->nextp) 
	{
	    logp->refCount++;	/* hold this guy */
	    osi_mutex_exit(&icl_lock);
	    /* don't clear persistent logs -- must be done explicitly */
	    if ((logp->states & ICL_LOGF_PERSISTENT) == 0)
		code = icl_ZeroLog(logp);
	    osi_mutex_enter(&icl_lock);
	    if (--logp->refCount == 0)
		icl_ZapLog_r(logp);
	    if (code) break;
	}
	osi_mutex_exit(&icl_lock);
	info->count = 0;
    }
    else
    {
	for(i = 0; i < info->count; i++)
	{
	    logp = icl_FindLog((char *)(info->loginfo[i].logName));
	    if (!logp)
		info->loginfo[i].states = ICL_ERROR_NOLOG;
	    else
	    {
		code = icl_ZeroLog(logp);
		info->loginfo[i].states = code;
		icl_LogRele(logp);
	    }
	}
	code = 0;	/* return results in states field of structure */
    }

    return code;
}

static
error_status_t SDFSTRACE_DumpSet(h, filename, all, info)
  rpc_binding_handle_t	h;
  idl_char filename[ICL_RPC_MAX_FILE_NAME];
  unsigned32 all;
  icl_bulkSetinfo_t *info;
{
    register struct icl_log *logp;
    register struct icl_set *setp;
    unsigned32 code = 0;
    long startTime;
    FILE *outFilep;
    int i;
    int j;
    extern FILE *icl_OpenLogFile();
    int dumpStyle;

    /* XXX Since the RPC interface does not indicated the style, impose
     * formatted style for now XXX
     */
    dumpStyle = ICL_DUMP_FORMATTED;

    if (code = CHECKAUTH(&h))
	return code;

    /* first open file */
    if (!(outFilep = icl_OpenLogFile((char *)&filename[0], /*quiet*/1)))
    {
	info->count = 0;
	return ICL_ERROR_CANTOPEN;
    }

    startTime = time((long *) 0);
    (void) fprintf(outFilep, "DFS Trace Dump -\n\n   Date: %s",
		   ctime(&startTime));

    if (all)
    {
	osi_mutex_enter(&icl_lock);
	for(logp = icl_allLogs; logp; logp=logp->nextp) 
	{
	    logp->refCount++;	/* hold this guy */
	    osi_mutex_exit(&icl_lock);
	    osi_mutex_enter(&logp->lock);
	    (void) fprintf(outFilep, "\nContents of log %s:\n", logp->name);
	    icl_DumpLog(outFilep, logp, dumpStyle);
	    osi_mutex_exit(&logp->lock);
	    osi_mutex_enter(&icl_lock);
	    if (--logp->refCount == 0)
		icl_ZapLog_r(logp);
	}
	osi_mutex_exit(&icl_lock);
	info->count = 0;
    }
    else 
    {
	for(i = 0; i < info->count; i++)
	{
	    /* dump by set */
	    setp = icl_FindSet((char *)(info->setinfo[i].setName));
	    if (!setp)
		info->setinfo[i].states = ICL_ERROR_NOSET;
	    else
	    {
		osi_mutex_enter(&setp->lock);
		for(j = 0; j < ICL_LOGSPERSET; j++) 
		{
		    logp = setp->logs[j];
		    if (logp)
		    {
			osi_mutex_enter(&logp->lock);
			(void) fprintf(outFilep, "\nContents of log %s:\n", logp->name);
			icl_DumpLog(outFilep, logp, dumpStyle);
			osi_mutex_exit(&logp->lock);
		    }
		}
		osi_mutex_exit(&setp->lock);
		icl_SetRele(setp);
		info->setinfo[i].states = 0;
	    }
	}
    }
    (void) fclose(outFilep);

    return 0;
}
  
