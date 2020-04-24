/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_ncs_procs.c,v $
 * Revision 1.1.31.1  1996/10/02  17:04:37  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:02  damon]
 *
 * $EndLog$
 */
/*
 *  bossvr_ncs_procs.c -- implementations of RPC routines exported by the bosserver
 *
 *	Copyright (C) 1995, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <ctype.h>

#include <dcedfs/ktime.h>

#include <dce/dce_cf.h>
#include <dce/binding.h>
#include <dce/rpc.h>
#include <dce/keymgmt.h>
#include <dce/dce_error.h>
#include <dce/secidmap.h>

#include <dcedfs/dfsauth.h>
#include <dcedfs/icl.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbssmac.h>
#include <dfsbsssvc.h>
#include <dfsbssmsg.h>
#include <pthread.h>

#include <bbos_ncs_interface.h>
#include <bbos_util.h>

#include <bbos_bnode.h>

#include <bossvr_afsFiles.h>
#include <bbos_pathnames.h>

#include <bossvr_debug.h>
#include <bossvr_trace.h>

extern struct icl_set *bossvr_iclSetp;		/* ICL event set pointer */

/*
 * Unfortunately, we have to define this, here, since it isn't defined in a header
 * file suitable for general consumption....sigh.
 */
#define NIDL_PIPE_BUFF_SIZE	2048

IMPORT long dfsauth_PrinName_GetName _TAKES((
					     char *	baseNameP,
					     char *	uuidStringP,
					     char *	nameBufferP,
					     int	length
					   ));

IMPORT pthread_mutex_t		bosserverMutex;
IMPORT pthread_cond_t		newRestartTimeCond;
IMPORT struct ktime		bossvr_nextRestartKT;
IMPORT struct ktime		bossvr_nextDayKT;
IMPORT struct afsconf_dir *	bossvr_confdir;

IMPORT char * RgySec_ErrorString _TAKES((error_status_t * dceErrorP));

#define BOSSVR_MAX_USERNAME_LENGTH	256
#define BOSSVR_MAX_CELLNAME_LENGTH	MAXCELLCHARS
#define BOSSVR_MAX_HOSTNAME_LENGTH	MAXHOSTCHARS

#define BOSSVR_BITS_PER_CHAR	8
#define BOSSVR_BITSOF(type)	(sizeof(type) * BOSSVR_BITS_PER_CHAR)

#define BOSSVR_AUTHZN_IS_REQD	1
#define BOSSVR_AUTHZN_IS_NOT_REQD	0

/* make sure we get prototypes checked for the internal routines */
PRIVATE long LockBossvrMutex _TAKES((IN char * routineName));
PRIVATE long UnlockBossvrMutex _TAKES((IN char * routineName));
PRIVATE long BossvrCommonInit _TAKES((
				      IN  char *		routineName,
				      OUT error_status_t *	theCommStatusP,
				      OUT int *			globalLockHeldP,
				      IN  rpc_binding_handle_t *	bosserverBindingP,
				      IN  int			authznReqd,
				      OUT int *			savedCancelStateP
				    ));
PRIVATE long BossvrCommonTerm _TAKES((
				      IN char *	routineName,
				      IN long	procRtnVal,
				      IN int	globalLockHeld,
				      IN int	savedCancelState
				    ));

PRIVATE long LockBossvrMutex(routineName)
     IN char *	routineName;
{
  long		rtnVal = 0;

  if ((rtnVal = pthread_mutex_lock(&bosserverMutex)) != 0) {
    bossvr_SvcLog(bss_s_global_lock_error, routineName, errno);
    rtnVal = BZ_ERROR_INTERNAL_ERROR;
  }

  return rtnVal;
}

PRIVATE long UnlockBossvrMutex(routineName)
     IN char *	routineName;
{
  long		rtnVal = 0;

  if ((rtnVal = pthread_mutex_unlock(&bosserverMutex)) != 0) {
    bossvr_SvcLog(bss_s_global_unlock_error, routineName, errno);
    rtnVal = BZ_ERROR_INTERNAL_ERROR;
  }
  
  return rtnVal;
}

PRIVATE long BossvrCommonInit(routineName, theCommStatusP, globalLockHeldP,
			      bosserverBindingP, authznReqd, savedCancelStateP)
     IN  char *			routineName;
     OUT error_status_t *	theCommStatusP;
     OUT int *			globalLockHeldP;
     IN  rpc_binding_handle_t *	bosserverBindingP;
     IN  int			authznReqd;
     OUT int *			savedCancelStateP;
{
  long		rtnVal = 0;
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_RPCTRACE,
		  ("%s called.", routineName));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_PROCS_RPCTRACE_0 , ICL_TYPE_STRING, routineName);
  
  /* first, disable cancellability */
  if ((*savedCancelStateP = pthread_setcancel(CANCEL_OFF)) != -1) {
    rtnVal = LockBossvrMutex(routineName);
    if (rtnVal == 0) {
      *globalLockHeldP = 1;
      
      if (authznReqd != 0) {
	rtnVal = dfsauth_server_CheckAuthorization(bosserverBindingP,
						   (dacl_permset_t *)NULL,
						   (char *)NULL, (uuid_t *)NULL);
	if (rtnVal == DAUT_ERROR_ACCESS_DENIED) {
	  rtnVal = BZACCESS;
	}
      }
    }
    
    *theCommStatusP = error_status_ok;
  }
  else {
    bossvr_SvcLog(bss_s_disable_cancelability_err, routineName, errno);
    rtnVal = BZ_ERROR_INTERNAL_ERROR;
  }
  
  return rtnVal;
}

PRIVATE long BossvrCommonTerm(routineName, procRtnVal, globalLockHeld, savedCancelState)
     IN char *	routineName;
     IN long	procRtnVal;
     IN int	globalLockHeld;
     IN int	savedCancelState;
{
  long		rtnVal = 0;
  static char	localRoutineName[] = "BossvrCommonTerm";
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_RPCTRACE,
	("%s returning: %s.", routineName, dfs_dceErrTxt(procRtnVal)));
  icl_Trace2(bossvr_iclSetp, BOSSVR_ICL_PROCS_RPCTRACE_1 , ICL_TYPE_STRING,
	     routineName, ICL_TYPE_STRING, dfs_dceErrTxt(procRtnVal));
  
  if (globalLockHeld != 0) {
    rtnVal = UnlockBossvrMutex(routineName);
  }
  
  /* map non-bosserver error codes */
  if (procRtnVal != 0) {
    if ((procRtnVal < ERROR_TABLE_BASE_bbs) || (procRtnVal > BZNOTIME)) {
      bossvr_SvcLog(bss_s_mapping_nonbosserver_error_code,
		 localRoutineName, routineName, procRtnVal,
		 dfs_dceErrTxt(procRtnVal));
      procRtnVal = BZ_ERROR_INTERNAL_ERROR;
    }
  }
  
  /* re-enable previous cancellability state (if that state is not CANCEL_OFF) */
  /* The curent state should be CANCEL_OFF when this routine is called. */
  if ((savedCancelState != CANCEL_OFF) && (pthread_setcancel(savedCancelState) == -1)) {
    bossvr_SvcLog(bss_s_reenable_cancelability_error, routineName, errno);
    rtnVal = BZ_ERROR_INTERNAL_ERROR;
  }
  
  /* don't lose any info about any failures prior to unlocking the global lock */
  return ((procRtnVal != 0) ? procRtnVal : rtnVal);
}

void bossvr_SetUpBossvrOutString(theBossvrOutStrP, srcString, srcStrOK)
     bossvr_out_string *		theBossvrOutStrP;
     char *			srcString;
     int			srcStrOK;
{
  if (srcStrOK) {
    theBossvrOutStrP->theString = rpc_ss_allocate(strlen(srcString) + 1);
    strcpy((char *)theBossvrOutStrP->theString, srcString);
  }
  else {
    theBossvrOutStrP->theString = rpc_ss_allocate(1);
    (theBossvrOutStrP->theString)[0] = '\0';
  }
  theBossvrOutStrP->strLen = strlen((char *)theBossvrOutStrP->theString);
}

long BOSSVR_GetServerStatus (bosserverBinding, serverStatusP, theCommStatus)
     IN rpc_binding_handle_t	bosserverBinding;
     OUT ulong *		serverStatusP;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  int			globalLockHeld = 0;
  int			savedCancelState;
  int			isServerNoauth = 0;
  long			dfsauthError;
  static char		routineName[] = "BOSSVR_GetServerStatus";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_NOT_REQD, &savedCancelState);

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_RPCTRACE,
		  ("BOSSVR_GetServerStatus entered"));
  icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_PROCS_RPCTRACE_2);
  *serverStatusP = 0;

  if (rtnVal == 0) {
    /* first, check the authorization checking status of the machine */
    dfsauthError = dfsauth_server_GetNoAuthStatus(&isServerNoauth);
    if (dfsauthError == 0) {
      *serverStatusP |= ((isServerNoauth) ? BOSSVR_SERVER_NOAUTH : 0);
    }
    else {
      bossvr_SvcLog(bss_s_auth_checking_status_determination_err,
		    routineName, dfsauthError);
      rtnVal = BZ_ERROR_INTERNAL_ERROR;
    }
    
    /* now, find out if the directory access looks OK to the bosserver */
    if (rtnVal == 0) {
      *serverStatusP |= ((DirAccessOK()) ? 0 : BOSSVR_BADDIRACCESS);
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_RPCTRACE,
		  ("BOSSVR_GetServerStatus returning %d", rtnVal));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_PROCS_RPCTRACE_3 , ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

long BOSSVR_CreateBnode (bosserverBinding, type, instance, p1, p2, p3, p4, p5, p6,
		       theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN unsigned char *		type;
     IN unsigned char *		instance;
     IN unsigned char *		p1;
     IN unsigned char *		p2;
     IN unsigned char *		p3;
     IN unsigned char *		p4;
     IN unsigned char *		p5;
     IN unsigned char *		p6;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  struct bnode *	newBnodeP;
  int			globalLockHeld = 0;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_CreateBnode";

  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BNODE,
	 ("BOSSVR_CreateBnode params: name = %s, type = %s, cmdLine = %s %s %s %s %s %s",
	  instance, type, p1, p2, p3, p4, p5, p6));
  icl_Trace4(bossvr_iclSetp, BOSSVR_ICL_PROCS_BNODE_4A, ICL_TYPE_STRING, instance, ICL_TYPE_STRING, type, ICL_TYPE_STRING, p1, ICL_TYPE_STRING, p2);
  icl_Trace4(bossvr_iclSetp, BOSSVR_ICL_PROCS_BNODE_4B, ICL_TYPE_STRING, p3, ICL_TYPE_STRING, p4, ICL_TYPE_STRING, p5, ICL_TYPE_STRING, p6);
  
  if (rtnVal == 0) {
    /* create a bnode and update BosConfig (update = 1) */
    rtnVal = bnode_IsValid(type, instance, p1, p2, p3, p4, p5, p6);
    if (rtnVal == 0) {
      rtnVal = bnode_Create(type, instance, &newBnodeP, p1, p2, p3, p4, p5, p6, /*upd?*/1);
      if (rtnVal == 0) {
        bnode_SetStat(newBnodeP, BSTAT_NORMAL);
      }
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_DeleteBnode (bosserverBinding, instance, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  unsigned char *	instance;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  int		globalLockHeld = 0;
  int		savedCancelState;
  static char	routineName[] = "BOSSVR_DeleteBnode";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    rtnVal = bnode_DeleteName(instance);
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_SetStatus (bosserverBinding, instance, status, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN unsigned char *		instance;
     IN long 			status;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  struct bnode *	theBnodeP;
  int			globalLockHeld = 0;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_SetStatus";

  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    theBnodeP = bnode_FindInstance(instance);
    if (theBnodeP != (struct bnode *)NULL) {
      bnode_Hold(theBnodeP);
      bnode_SetFileGoal(theBnodeP, status);
      rtnVal = bnode_SetStat(theBnodeP, status);
      bnode_Release(theBnodeP);
    }
    else {
      rtnVal = BZNOENT;
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_GetStatus (bosserverBinding, instance, intStat, statdescrP, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  unsigned char *	instance;
     OUT long *			intStat;
     OUT bossvr_out_string *	statdescrP;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  struct bnode *	theBnodeP;
  int			globalLockHeld = 0;
  char			statusString[BOSSVR_BSSIZE];
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_GetStatus";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_NOT_REQD, &savedCancelState);

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_RPCTRACE,
	   ("BOSSVR_GetStatus params: instance %s.", instance));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_PROCS_RPCTRACE_5 , ICL_TYPE_STRING, instance);

  statusString[0] = '\0';
  *intStat = 0;
  
  if (rtnVal == 0) {
    theBnodeP = bnode_FindInstance(instance);
    if (theBnodeP != (struct bnode *)NULL) {
      bnode_Hold(theBnodeP);

      rtnVal = bnode_GetStat(theBnodeP, intStat);
      if (rtnVal == 0) {
	rtnVal = bnode_GetString(theBnodeP, statusString, BOSSVR_BSSIZE);
	if (rtnVal != 0) {
	  statusString[0] = '\0';
	  rtnVal = 0;		/* we really ignore an error here */
	}
      }

      bnode_Release(theBnodeP);
    }
    else {
      rtnVal = BZNOENT;
    }
  }
  
  bossvr_SetUpBossvrOutString(statdescrP, statusString, (rtnVal == 0));

  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

struct enumNameCtr {
  char *	instanceName;
  int		enumCtr;
};

static int EnumerationCountingFunction(abnode, arock)
     struct bnode *	abnode;
     char *		arock;
{
  int			rtnVal = 0;
  struct enumNameCtr *	counterStruct = (struct enumNameCtr *)arock;

  if (counterStruct->enumCtr-- == 0) {
    (void)strcpy(counterStruct->instanceName, abnode->name);
    rtnVal = 1;
  }

  return rtnVal;
}

long BOSSVR_EnumerateInstance (bosserverBinding, instanceNum, inameP, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN long			instanceNum;
     OUT bossvr_out_string *	inameP;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  struct enumNameCtr	enumData;
  int			globalLockHeld = 0;
  char			inameBuffer[BOSSVR_BSSIZE];
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_EnumerateInstance";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_NOT_REQD, &savedCancelState);

  inameBuffer[0] = '\0';
  
  if (rtnVal == 0) {
    enumData.enumCtr = instanceNum;
    enumData.instanceName = inameBuffer;

    (void)bnode_ApplyInstance(EnumerationCountingFunction, &enumData);

    if (enumData.enumCtr >= 0) {	/* instance > # of actual instances */
      rtnVal = BZDOM;
    }
  }
  
  bossvr_SetUpBossvrOutString(inameP, inameBuffer, (rtnVal == 0));

  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_GetInstanceInfo (bosserverBinding, instance, typeP, status, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  unsigned char *	instance;
     OUT bossvr_out_string *	typeP;
     OUT struct bossvr_status *	status;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  struct bnode *	instanceBnodeP = (struct bnode *)NULL;
  int			globalLockHeld = 0;
  char			typeBuffer[BUFSIZ];
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_GetInstanceInfo";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_NOT_REQD, &savedCancelState);
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_RPCTRACE,
	   ("BOSSVR_GetInstanceInfo params: process: %s.", instance));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_PROCS_RPCTRACE_6 , ICL_TYPE_STRING, instance);
  
  /* set up default values */
  bzero((char *)status, sizeof(struct bossvr_status));
  typeBuffer[0] = '\0';
  
  if (rtnVal == 0) {
    instanceBnodeP = bnode_FindInstance(instance);
    if (instanceBnodeP != (struct bnode *)NULL) {
      bnode_Hold(instanceBnodeP);
      
      (void)strncpy(typeBuffer, instanceBnodeP->type->name, sizeof(typeBuffer) - 1);
      
      status->goal = instanceBnodeP->goal;
      status->fileGoal = instanceBnodeP->fileGoal;
      status->procStartTime = instanceBnodeP->procStartTime;
      status->procStarts = instanceBnodeP->procStarts;
      status->lastAnyExit = instanceBnodeP->lastAnyExit;
      status->lastErrorExit = instanceBnodeP->lastErrorExit;
      status->errorCode = instanceBnodeP->errorCode;
      status->errorSignal = instanceBnodeP->errorSignal;
      if (instanceBnodeP->flags & BNODE_ERRORSTOP) {
	status->flags |= BOSSVR_ERRORSTOP;
      }
      if (bnode_HasCore(instanceBnodeP)) {
	status->flags |= BOSSVR_HASCORE;
      }

      bnode_Release(instanceBnodeP);
    }
    else {
      rtnVal = BZNOENT;
    }
  }
  
  bossvr_SetUpBossvrOutString(typeP, typeBuffer, (rtnVal == 0));

  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_GetInstanceParm (bosserverBinding, instance, num, parmP, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  unsigned char *	instance;
     IN  long			num;
     OUT bossvr_out_string *	parmP;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  struct bnode *	instanceBnodeP;
  int			globalLockHeld = 0;
  char			parmBuffer[BOSSVR_BSSIZE];
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_GetInstanceParm";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_NOT_REQD, &savedCancelState);

  parmBuffer[0] = '\0';
  if (rtnVal == 0) {
    instanceBnodeP = bnode_FindInstance(instance);
    if (instanceBnodeP != (struct bnode *)NULL) {
      bnode_Hold(instanceBnodeP);
      rtnVal = bnode_GetParm(instanceBnodeP, num, parmBuffer, BOSSVR_BSSIZE);
      bnode_Release(instanceBnodeP);
    }
    else {
      rtnVal = BZNOENT;
    }
  }
  
  bossvr_SetUpBossvrOutString(parmP, parmBuffer, (rtnVal == 0));

  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}


long BOSSVR_AddSUser (bosserverBinding, filenameP, typeStrP, nameP, permsStrP,
		      flags, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN	 unsigned char *	filenameP;
     IN	 unsigned char *	typeStrP;
     IN  unsigned char *	nameP;
     IN	 unsigned char *	permsStrP;
     IN  ulong			flags;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  int			globalLockHeld = 0;
  dacl_t		theAcl;
  char			realPermString[BUFSIZ];
  char			fullpathname[BUFSIZ];
  long			dfsauthRtnVal = 0;
  sec_rgy_name_t	theGlobalName;
  uuid_t		realmUuid;
  sec_rgy_bind_auth_info_t	bindingAuthInfo;
  sec_rgy_handle_t		secdHandle;
  error_status_t	ncs_error_status;
  char *		errorStringP = (char *)NULL;
  int			savedCancelState;
  long			createFile = (flags & BOSSVR_ADDADMIN_CREATELIST);
  static char		routineName[] = "BOSSVR_AddSUser";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_RPCTRACE,
	   ("BOSSVR_AddSUser params: name: %s, type: %s.", nameP, typeStrP));
  icl_Trace2(bossvr_iclSetp, BOSSVR_ICL_PROCS_RPCTRACE_7 , ICL_TYPE_STRING, nameP, ICL_TYPE_STRING, typeStrP);
  
  if (rtnVal == 0) {
    if (dfsauthRtnVal == 0) {
      rtnVal = dfsauth_GenerateAdminListPath(filenameP, fullpathname, sizeof(fullpathname));
      if (rtnVal == 0) {
	rtnVal = dacl_ReadFromDisk(&theAcl, fullpathname, &bosserverAclMgmtUuid);
	if ((rtnVal == 0) && (createFile != 0) &&
	    (BBOS_IS_NO_TYPE(typeStrP)) && (BBOS_IS_NO_NAME(nameP))) {
	  /*   The caller asked to only create the file, not add any
	   * administrators.  However, the file already exists.  In this case, we
	   * return an error.
	   */
	  rtnVal = BZEXISTS;
	}
	else if (rtnVal == DACL_ERROR_ACLFILE_NOT_FOUND) {
	  if (createFile != 0) {
	    bindingAuthInfo.info_type = sec_rgy_bind_auth_none;
	    (void)strcpy((char *)theGlobalName, (char *)CELL_DIR_ROOT);
	    sec_rgy_site_bind(theGlobalName,
			      &bindingAuthInfo,
			      &secdHandle,
			      &ncs_error_status);
	    
	    if (ncs_error_status == error_status_ok) {
	      /* get the local realm uuid from the security server */
	      (void)sprintf((char *)theGlobalName, "%s%s", (char *)CELL_DIR_ROOT, "root");
	      
	      sec_id_parse_name(secdHandle,
				(unsigned char *) theGlobalName,
				NULL,
				&realmUuid,
				NULL,
				(uuid_t *)NULL,
				&ncs_error_status);
	      
	      if (ncs_error_status == error_status_ok) {
		rtnVal = dacl_CreateAclOnDisk(fullpathname, &bosserverAclMgmtUuid,
					      (epi_uuid_t *)&realmUuid,
					      (dacl_permset_t *)NULL,
					      (dacl_permset_t *)NULL,
					      (dacl_permset_t *)NULL,
					      0 /* don't use Episode file */, 0);
	      }
	      else {
		errorStringP = RgySec_ErrorString(&ncs_error_status);

		if (errorStringP) {
		  bossvr_SvcLog(bss_s_root_name_parse_failure,
				routineName, errorStringP);
		}
		else {
		  bossvr_SvcLog(bss_s_root_name_parse_failure_unk, routineName);
		}

		if (errorStringP) osi_Free(errorStringP, sizeof(dce_error_string_t));
		rtnVal = BZ_ERROR_INTERNAL_ERROR;
	      }
	    }
	    else {
	      errorStringP = RgySec_ErrorString(&ncs_error_status);

	      if (errorStringP) {
	        bossvr_SvcLog(bss_s_secd_handle_setup_failure,
			      routineName, errorStringP);
	      }
	      else {
	        bossvr_SvcLog(bss_s_secd_handle_setup_failure_unk, routineName);
	      }

	      if (errorStringP) osi_Free(errorStringP, sizeof(dce_error_string_t));
	      rtnVal = BZ_ERROR_INTERNAL_ERROR;
	    }
	    
	    if (rtnVal == 0) {
	      rtnVal = dacl_ReadFromDisk(&theAcl, fullpathname, &bosserverAclMgmtUuid);
	    }
	  }
	  else {
	    rtnVal = BZ_ADMIN_FILE_NOT_FOUND;
	  }
	}
	
	if ((rtnVal == 0) && (!BBOS_IS_NO_TYPE(typeStrP)) && (!BBOS_IS_NO_NAME(nameP))) {
	  (void)strncpy(realPermString, DACL_USERLIST_PERMSTRING,
			sizeof(realPermString) - 1);
	  if ((strcmp((char *)typeStrP, "user") == 0) || (strcmp((char *)typeStrP, "group") == 0) ||
	      (strcmp((char *)typeStrP, "foreign_user") == 0) ||
	      (strcmp((char *)typeStrP, "foreign_group") == 0) ||
	      (strcmp((char *)typeStrP, "foreign_other") == 0)) {
	    rtnVal = dacl_AddEntryToAcl(&theAcl, (char *) typeStrP,
					(char *) nameP,
					(char *) realPermString,
					(char *) NULL, &bosserverAclMgmtUuid,
					0 /* don't use local ids */);
	    if (rtnVal == 0) {
	      rtnVal = dacl_WriteToDisk(&theAcl, fullpathname);
	      if (rtnVal == 0) {
		rtnVal = dacl_FreeAclEntries(&theAcl);
	      }	/* end if we wrote to disk successfully */
	    }	/* end if we added the entry successfully */
	    else {
	      if (rtnVal == DACL_ERROR_ENTRY_EXISTS) {
		rtnVal = BZEXISTS;
	      }
	      else if (rtnVal == DACL_ERROR_UNRECOGNIZED_USER_OR_GROUP) {
		rtnVal = BZ_ERROR_UNKNOWN_USER_OR_GROUP;
	      }
	    }
	    
	  }		/* end if had an entry type we were willing to add */
	  else {
	    rtnVal = BZ_ILLEGAL_ENTRY_TYPE;
	  }
	}		/* end if we read from disk successfully & had something to add */      }		/* end if we generated the full pathname successfully */

    }
    else {
      bossvr_SvcLog(bss_s_localauth_context_setup_failure,
		 routineName, dfs_dceErrTxt(dfsauthRtnVal));
      rtnVal = BZ_ERROR_INTERNAL_ERROR;
    }
  }		/* end if we initialized properly */
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}


long BOSSVR_DeleteSUser (bosserverBinding, filenameP, typeStrP, nameP, flags,
			 theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN	 unsigned char *	filenameP;
     IN	 unsigned char *	typeStrP;
     IN  unsigned char *	nameP;
     IN  ulong			flags;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  long		tempRtnVal = 0;
  dacl_t	theAcl;
  int		globalLockHeld = 0;
  char		fullpathname[BUFSIZ];
  long		dfsauthRtnVal = 0;
  int		savedCancelState;
  long		removeFile = (flags & BOSSVR_RMADMIN_REMOVELIST);
  long		clearList = (flags & BOSSVR_RMADMIN_CLEARLIST);
  static char	routineName[] = "BOSSVR_DeleteSUser";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    if (dfsauthRtnVal == 0) {
      rtnVal = dfsauth_GenerateAdminListPath(filenameP, fullpathname, sizeof(fullpathname));
      if (rtnVal == 0) {
	if ((rtnVal = dacl_ReadFromDisk(&theAcl, fullpathname, &bosserverAclMgmtUuid)) == 0) {
	  if (clearList) {
	    rtnVal = dacl_DeleteAllEntries(&theAcl);
	  }
	  else if ((!BBOS_IS_NO_TYPE(typeStrP)) && (!BBOS_IS_NO_NAME(nameP))) {
	    rtnVal = dacl_DeleteAclEntry(&theAcl, (char *) typeStrP,
					 (char *) nameP, &bosserverAclMgmtUuid,
					 0 /* don't use local ids */);
	  }
	  
	  if (rtnVal == 0) {
	    if ((theAcl.num_entries == 0) && (removeFile != 0)) {
	      rtnVal = unlink(fullpathname);
	      if (rtnVal != 0) {
		rtnVal = BZIO;
	      }
	    }
	    else if ((theAcl.num_entries > 0) && (removeFile != 0) &&
		     (BBOS_IS_NO_TYPE(typeStrP)) && (BBOS_IS_NO_NAME(nameP))) {
	      /*  The caller has only requested the removal of the list,
	       * but there are still entries present.  In this case, we return
	       * an error.
	       */
	      rtnVal = BZBUSY;
	    }
	    else {
	      rtnVal = dacl_WriteToDisk(&theAcl, fullpathname);
	    }

	    /*  Free the entries unconditionally, but only send back the return code
	     * from freeing the entries, if it doesn't mask an earlier error.
	     */
	    tempRtnVal = dacl_FreeAclEntries(&theAcl);
	    if (rtnVal == 0) {
	      rtnVal = tempRtnVal;
	    }
	  }
	  else {
	    if (rtnVal == DACL_ERROR_ENTRY_NOT_FOUND) {
	      rtnVal = BZNOENT;
	    }
	  }
	}
	else {
	  if (rtnVal == DACL_ERROR_ACLFILE_NOT_FOUND) {
	    rtnVal = BZ_ADMIN_FILE_NOT_FOUND;
	  }
	}
      }
    }
    else {
      bossvr_SvcLog(bss_s_localauth_context_setup_failure2,
		 routineName, dfs_dceErrTxt(dfsauthRtnVal));
      rtnVal = BZ_ERROR_INTERNAL_ERROR;
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_ListSUsers (bosserverBinding, filenameP, an, nameP, typeStrP, permsStrP,
			theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN	 unsigned char *	filenameP;
     IN  long			an;
     OUT bossvr_out_string *	nameP;
     OUT bossvr_out_string *	typeStrP;
     OUT bossvr_out_string *	permsStrP;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  int			globalLockHeld = 0;
  int			adjustedIndex;
  char *		internalTypeStringP;
  sec_rgy_name_t	userNameBuffer;
  sec_rgy_domain_t	nameDomain;
  dacl_t		theAcl;
  dacl_entry_t *	theEntryP = (dacl_entry_t *)NULL;
  dacl_entry_t *	thisEntryP;
  char			fullpathname[BUFSIZ];
  long			dfsauthRtnVal = 0;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_ListSUsers";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_NOT_REQD, &savedCancelState);

  userNameBuffer[0] = '\0';
  if (rtnVal == 0) {
    if (dfsauthRtnVal == 0) {
      rtnVal = dfsauth_GenerateAdminListPath(filenameP, fullpathname, sizeof(fullpathname));
      if (rtnVal == 0) {
	if ((rtnVal = dacl_ReadFromDisk(&theAcl, fullpathname,
					&bosserverAclMgmtUuid)) == 0)	 {
	  if (theAcl.num_entries > an) {
	    /* find the correct entry */
	    if (an <
		theAcl.complex_entries[(int)dacl_complex_entry_type_user].num_entries) {
	      theEntryP = &(theAcl.complex_entries[(int)dacl_complex_entry_type_user].
			    complex_entries[an]);
	    }
	    else {
	      adjustedIndex =
		an - theAcl.complex_entries[(int)dacl_complex_entry_type_user].num_entries;
	      if (adjustedIndex <
		  theAcl.complex_entries[(int)dacl_complex_entry_type_group].num_entries) {
		theEntryP = &(theAcl.complex_entries[(int)dacl_complex_entry_type_group].
			      complex_entries[adjustedIndex]);
	      }
	      else {
		rtnVal = BZNOENT; 
	      }
	    }
	    
	    if (theEntryP != (dacl_entry_t *)NULL) {
	      rtnVal = dacl_NameAndTypeStringsFromEntry(theEntryP, &(theAcl.default_realm),
					       &internalTypeStringP, userNameBuffer,
					       0 /* don't use local ids */);
	      /* remap some of the problem error codes */
	      if (rtnVal == DACL_ERROR_SEC_RGY_PGO_ERROR) {
		rtnVal = BZ_SEC_RGY_PGO_ERROR;
	      }
	      else if (rtnVal == DACL_ERROR_UNIMPLEMENTED_ENTRY_TYPE) {
		rtnVal = BZ_ILLEGAL_ENTRY_TYPE;
	      }
	      else if (rtnVal == DACL_ERROR_UNRECOGNIZED_USER_OR_GROUP) {
		/* log this one, but return success, since we have something to print */
		bossvr_SvcLog(bss_s_adminlist_uuid_unrecognized,
			      routineName, userNameBuffer,
			      fullpathname);
		rtnVal = 0;
	      }
	    }
	    else {
	      rtnVal = BZNOENT;
	    }
	  }
	  else {
	    rtnVal = BZNOENT;
	  }
	}
	else {
	  if (rtnVal == DACL_ERROR_ACLFILE_NOT_FOUND) {
	    rtnVal = BZ_ADMIN_FILE_NOT_FOUND;
	  }
	}
      }
    }
    else {
      bossvr_SvcLog(bss_s_localauth_context_setup_failure3,
		    routineName, dfs_dceErrTxt(dfsauthRtnVal));
      rtnVal = BZ_ERROR_INTERNAL_ERROR;
    }
  }
  
  /* everything except the name and type is ignored for this impl */
  bossvr_SetUpBossvrOutString(permsStrP, (char *)NULL, 0);

  bossvr_SetUpBossvrOutString(nameP, userNameBuffer, (rtnVal == 0));
  bossvr_SetUpBossvrOutString(typeStrP, internalTypeStringP,
			  (rtnVal == 0) && (internalTypeStringP != (char *)NULL));

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_RPCTRACE,
	   ("%s exiting; returning name: %s", routineName, nameP->theString));
  icl_Trace2(bossvr_iclSetp, BOSSVR_ICL_PROCS_RPCTRACE_8 , ICL_TYPE_STRING, routineName, ICL_TYPE_STRING, nameP->theString);
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);

  return rtnVal;
}

long BOSSVR_ListKeys (bosserverBinding, prinNameP, an, kvno, key, keyinfo, theCommStatus)
     IN	 rpc_binding_handle_t		bosserverBinding;
     IN	 unsigned char *		prinNameP;
     IN  long				an;
     OUT long *				kvno;
     OUT struct bossvr_key *		key;
     OUT struct bossvr_keyInfo *	keyinfo;
     OUT error_status_t *		theCommStatus;
{
  long			rtnVal = 0;
  int			noAuth;
  struct stat		statInfo;
  int			globalLockHeld = 0;
  char			principalNameBuffer[BUFSIZ];
  error_status_t	secError;
  sec_passwd_rec_t *	keydataP;
  char *		keyBytesP;
  int			realKeyLength;
  dfsauth_keyinfo_t	dfsauth_keyinfo;
  char *		errorStringP = (char *)NULL;
  
  char *		keyCursor;
  int			keyCounter;
  sec_passwd_type_t	keytype = dfsauth_sec_key_mgmt_default_keytype;
  error_status_t	secReleaseError;

  int			savedCancelState;
  static char		routineName[] = "BOSSVR_ListKeys";

  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  *kvno = 0;
  bzero((char *)key, sizeof(struct bossvr_key));
  bzero((char *)keyinfo, sizeof(struct bossvr_keyInfo));
  
  if (rtnVal == 0) {
    if (strlen((char *)prinNameP) != 0) {
      (void)strncpy(principalNameBuffer, (char *)prinNameP, sizeof(principalNameBuffer));
    }
    else {
      rtnVal = dfsauth_PrinName_GetName((char *)NULL, (char *)NULL,
					principalNameBuffer, sizeof(principalNameBuffer));
    }
    
    if (rtnVal == 0) {
      sec_key_mgmt_initialize_cursor(dfsauth_sec_key_mgmt_authn_service,
				     (void *)NULL,
				     (unsigned char *)principalNameBuffer,
				     &keytype,
				     (void **)&keyCursor,
				     &secError);
      if (secError == error_status_ok) {
	for (keyCounter = 0;
	     (keyCounter <= an) && (secError == error_status_ok);
	     keyCounter++) {
	  sec_key_mgmt_get_next_key((void *)keyCursor,
				    (unsigned char **)NULL,
				    (unsigned32 *)kvno,
				    (void **)&keydataP,
				    &secError);
	  if (secError == error_status_ok) {
	    if (keyCounter < an) {
	      /* this isn't the one we want, yet */
	      /* the whole process looks pretty inefficient, doesn't it? */
	      dfsauth_sec_key_mgmt_free_key(keydataP);
	    }
	  }
	  else {
	    if (secError == sec_key_mgmt_e_unauthorized) {
	      bossvr_SvcLog(bss_s_unex_autherror_from_getnext_key,
			    routineName);
	      rtnVal = BZACCESS;	/* this should not happen */
	    }
	    else if (secError == sec_key_mgmt_e_key_unavailable) {
	      rtnVal = BZDOM;
	    }
	    else {
	      errorStringP = RgySec_ErrorString(&secError);

	      if (errorStringP) {	
	        bossvr_SvcLog(bss_s_error_getting_princ_keynum,
			      routineName, keyCounter,
			      principalNameBuffer, errorStringP);
	      }
	      else {
	        bossvr_SvcLog(bss_s_error_getting_princ_keynum_unk,
			      routineName, keyCounter,
			      principalNameBuffer);
	      }

	      if (errorStringP) osi_Free(errorStringP, sizeof(dce_error_string_t));
	      rtnVal = BZ_SEC_KEY_MGMT_ERROR;
	    }
	  }
	}

	sec_key_mgmt_release_cursor((void *)&keyCursor,
				    &secReleaseError);
	if (secReleaseError != error_status_ok) {
	  errorStringP = RgySec_ErrorString(&secReleaseError);

	  if (errorStringP) {	
	    bossvr_SvcLog(bss_s_error_releasing_keyfile_cursor,
			  routineName, errorStringP);
	  }
	  else {
	    bossvr_SvcLog(bss_s_error_releasing_keyfile_cursor_unk,
			  routineName);
	  }

	  if (errorStringP) osi_Free(errorStringP, sizeof(dce_error_string_t));
	  rtnVal = BZ_SEC_KEY_MGMT_ERROR;
	}
      }
      else {
	if (secError == sec_key_mgmt_e_unauthorized) {
	  bossvr_SvcLog(bss_s_unex_autherr_from_key_cursor_init,
			routineName);
	  rtnVal = BZACCESS;	/* this should not happen */
	}
	else {
	  errorStringP = RgySec_ErrorString(&secError);

	  if (errorStringP) {	
	    bossvr_SvcLog(bss_s_error_initing_princname_keyfile_cursor,
			  routineName, principalNameBuffer, errorStringP);
	  }
	  else {
	    bossvr_SvcLog(bss_s_error_initing_princname_keyfile_cursor_unk,
			  routineName, principalNameBuffer);
	  }

	  if (errorStringP) osi_Free(errorStringP, sizeof(dce_error_string_t));
	  rtnVal = BZ_SEC_KEY_MGMT_ERROR;
	}
      }

      if (secError == error_status_ok) {
	/* in this case, there should only be data in the first entry of the array */
	if (keydataP[0].key.key_type == sec_passwd_des) {
	  keyBytesP = (char *)keydataP[0].key.tagged_union.des_key;
	  realKeyLength = sizeof(sec_passwd_des_key_t);
	}
#if defined(DFSAUTH_FOR_EXPORT)
	/* only allow this option if we think DES keys might not come back */
	else if (keydataP[0].key.key_type == sec_passwd_plain) {
	  keyBytesP = keydataP[0].key.tagged_union.plain;
	  realKeyLength = strlen(keydataP[0].key.tagged_union.plain);
	}
#endif /* defined(DFSAUTH_FOR_EXPORT) */
	else {
	  bossvr_SvcLog(bss_s_incorrect_getkey_keytype, routineName);
	  rtnVal = BZ_ERROR_INTERNAL_ERROR;
	}
      }
      
      if (rtnVal == 0) {
	rtnVal = dfsauth_server_GetNoAuthStatus(&noAuth);
	if (rtnVal == 0) {
	  if (noAuth != 0) {
	    /* the server is running no auth */

	    /* copy the key data */
#if !defined(DFSAUTH_FOR_EXPORT)
	    if (realKeyLength != sizeof(key->data)) {
	      bossvr_SvcLog(bss_s_keylength_not_bosservers_warning,
			    routineName, realKeyLength,
			    sizeof(key->data));
	    }
#endif /* !defined(DFSAUTH_FOR_EXPORT) */
	    /* make sure we only copy what we can handle */
	    bcopy((char *)keyBytesP, (char *)key->data,
		  ((sizeof(key->data) <= realKeyLength) ?
		   sizeof(key->data) : realKeyLength));
	  }

	  /* make sure we have an identity when we try to reach the registry */
	  (void)dfsauth_client_InitAuthentication((rpc_binding_handle_t *)NULL,
						  (char *)NULL,
						  0, /* => not noauth */
						  1 /* => use localauth */);
	  /* we can get most of the key info, even if the above failed */
	  dfsauth_sec_key_mgmt_keyinfo(&(keydataP[0]), &dfsauth_keyinfo,
				       principalNameBuffer);
	  (void)dfsauth_client_CleanupAuthentication();

	  /* now, copy out the relevant fields */
	  keyinfo->mod_sec = dfsauth_keyinfo.mod_sec;
	  keyinfo->keyCheckSum = dfsauth_keyinfo.keyCheckSum;

	  /* free the key data */
	  dfsauth_sec_key_mgmt_free_key(keydataP);
	}
      }	/* end if we got the all right */
    }	/* end if we got the principal name all right */
  }	/* end if there was not problem with the init */
  

  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_AddKey (bosserverBinding, prinNameP, kvno, passwdP, localOnly, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN	 unsigned char *	prinNameP;
     IN  long			kvno;
     IN	 unsigned char *	passwdP;
     IN	 long			localOnly;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  int			globalLockHeld = 0;
  char			principalNameBuffer[BUFSIZ];
  char			calleeName[BUFSIZ];
  error_status_t	secKeyError = error_status_ok;
  sec_timeval_period_t	garbageCollectTime;
  sec_passwd_rec_t	keyOrPasswordRecord;
  long			dfsauthRtnVal;
  char *		errorStringP = (char *)NULL;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_AddKey";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    if (strlen((char *)prinNameP) != 0) {
      (void)strncpy(principalNameBuffer, (char *)prinNameP, sizeof(principalNameBuffer));
    }
    else {
      rtnVal = dfsauth_PrinName_GetName((char *)NULL, (char *)NULL,
					principalNameBuffer, sizeof(principalNameBuffer));
    }
    
    if (rtnVal == 0) {
      keyOrPasswordRecord.version_number = kvno;
      keyOrPasswordRecord.pepper = (unsigned char *)NULL;
      keyOrPasswordRecord.key.key_type = sec_passwd_plain;
      keyOrPasswordRecord.key.tagged_union.plain = passwdP;
      
      if (localOnly == 0) {
	dfsauthRtnVal = dfsauth_client_InitAuthentication((rpc_binding_handle_t *)NULL,
							  (char *)NULL,
							  0, /* => not noauth */
							  1 /* => use localauth */);
	if (dfsauthRtnVal == 0) {
	  strcpy(calleeName, "sec_key_mgmt_change_key");
	  sec_key_mgmt_change_key(dfsauth_sec_key_mgmt_authn_service,
				  (void *)NULL,
				  (unsigned char *)principalNameBuffer,
				  kvno,
				  (unsigned32 *)&keyOrPasswordRecord,
				  &garbageCollectTime,
				  &secKeyError);
	  
	  dfsauthRtnVal = dfsauth_client_CleanupAuthentication();
	  if (dfsauthRtnVal) {
	    bossvr_SvcLog(bss_s_localauth_context_teardown_failed,
			  routineName, dfs_dceErrTxt(dfsauthRtnVal));
	    /* don't disguise key error in this case */
	  }
	}
	else {
	  bossvr_SvcLog(bss_s_setup_localauth_context_not_successful,
			routineName, dfs_dceErrTxt(dfsauthRtnVal));
	  rtnVal = BZ_ERROR_INTERNAL_ERROR;
	}
      }
      else {
	/* only update the local copy */
	strcpy(calleeName, "sec_key_mgmt_set_key");
	sec_key_mgmt_set_key(dfsauth_sec_key_mgmt_authn_service,
			     (void *)NULL,
			     (unsigned char *)principalNameBuffer,
			     kvno,
			     &keyOrPasswordRecord,
			     &secKeyError);
      }
      
      if (secKeyError != error_status_ok) {
	if (secKeyError == sec_key_mgmt_e_unauthorized) {
	  bossvr_SvcLog(bss_s_unex_autherr_from_key_mgt, routineName);
	  rtnVal = BZACCESS;	/* this should not happen */
	}
	else if (secKeyError == sec_key_mgmt_e_not_implemented) {
	  rtnVal = BZNOTIMPL;
	}
	else {
	  errorStringP = RgySec_ErrorString(&secKeyError);

	  if (errorStringP) {
	    bossvr_SvcLog(bss_s_add_key_version_for_princpal_failed,
			  routineName, calleeName, kvno,
			  principalNameBuffer, errorStringP);
	  }
	  else {
	    bossvr_SvcLog(bss_s_add_key_version_for_princpal_failed_unk,
			  routineName, calleeName, kvno,
			  principalNameBuffer);
	  }

	  if (errorStringP) osi_Free(errorStringP, sizeof(dce_error_string_t));
	  rtnVal = BZ_SEC_KEY_MGMT_ERROR;
	}
      }
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_DeleteKey (bosserverBinding, prinNameP, kvno, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN	 unsigned char *	prinNameP;
     IN  long			kvno;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  int			globalLockHeld = 0;
  char			principalNameBuffer[BUFSIZ];
  error_status_t	secKeyError;
  sec_passwd_type_t	keytype = dfsauth_sec_key_mgmt_default_keytype;
  char *		errorStringP = (char *)NULL;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_DeleteKey";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    if (strlen((char *)prinNameP) != 0) {
      (void)strncpy(principalNameBuffer, (char *)prinNameP, sizeof(principalNameBuffer));
    }
    else {
      rtnVal = dfsauth_PrinName_GetName((char *)NULL, (char *)NULL,
					principalNameBuffer, sizeof(principalNameBuffer));
    }
    
    if (rtnVal == 0) {
      sec_key_mgmt_delete_key_type(dfsauth_sec_key_mgmt_authn_service,
				   (void *)NULL,
				   (unsigned char *)principalNameBuffer,
				   &keytype,
				   kvno,
				   &secKeyError);
      if (secKeyError != error_status_ok) {
	if (secKeyError == sec_key_mgmt_e_unauthorized) {
	  bossvr_SvcLog(bss_s_unex_autherr_from_key_mgt2, routineName);
	  rtnVal = BZACCESS;	/* this should not happen */
	}
	else {
	  errorStringP = RgySec_ErrorString(&secKeyError);

	  if (errorStringP) {
	    bossvr_SvcLog(bss_s_del_key_version_failed_for_principal,
			  routineName, kvno, principalNameBuffer,
			  errorStringP);
	  }
	  else {
	    bossvr_SvcLog(bss_s_del_key_version_failed_for_principal_unk,
			  routineName, kvno, principalNameBuffer);
	  }

	  if (errorStringP) osi_Free(errorStringP, sizeof(dce_error_string_t));
	  rtnVal = BZ_SEC_KEY_MGMT_ERROR;
	}
      }
    }
  }
 
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_GenerateKey (bosserverBinding, prinNameP, kvno,  theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN	 unsigned char *	prinNameP;
     IN  long			kvno;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  int			globalLockHeld = 0;
  char			principalNameBuffer[BUFSIZ];
  error_status_t  	secKeyError = error_status_ok;
  sec_timeval_period_t	garbageCollectTime;
  sec_passwd_type_t	keytype = dfsauth_sec_key_mgmt_default_keytype;
  sec_passwd_rec_t *	keydataP;
  char			calleeName[512];
  long			dfsauthRtnVal;
  char *		errorStringP = (char *)NULL;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_GenerateKey";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    if (strlen((char *)prinNameP) != 0) {
      (void)strncpy(principalNameBuffer, (char *)prinNameP, sizeof(principalNameBuffer));
    }
    else {
      rtnVal = dfsauth_PrinName_GetName((char *)NULL, (char *)NULL,
					principalNameBuffer, sizeof(principalNameBuffer));
    }
    
    if (rtnVal == 0) {
      dfsauthRtnVal = dfsauth_client_InitAuthentication((rpc_binding_handle_t *)NULL,
							(char *)NULL,
							0, /* => not noauth */
							1 /* => use localauth */);
      if (dfsauthRtnVal == 0) {
	  strcpy(calleeName, "sec_key_mgmt_gen_rand_key");
	  sec_key_mgmt_gen_rand_key(dfsauth_sec_key_mgmt_authn_service,
				    (void *)NULL,
				    (unsigned char *)principalNameBuffer,
				    (void *)&keytype,
				    kvno,
				    (void **)&keydataP,
				    &secKeyError);
	  if (secKeyError == error_status_ok) {
	    strcpy(calleeName, "sec_key_mgmt_change_key");
	    sec_key_mgmt_change_key(dfsauth_sec_key_mgmt_authn_service,
				    (void *)NULL,
				    (unsigned char *)principalNameBuffer,
				    kvno,
				    (void *)keydataP,
				    &garbageCollectTime,
				    &secKeyError);
	    
	  }
	  
	  dfsauth_sec_key_mgmt_free_key(keydataP);	/* free the generated array */
	if (secKeyError != error_status_ok) {
	  /* random key generation did not work */
	  errorStringP = RgySec_ErrorString(&secKeyError);

	  if (errorStringP) {
	    bossvr_SvcLog(bss_s_error_genning_random_key,
			  routineName, errorStringP);
	  }
	  else {
	    bossvr_SvcLog(bss_s_error_genning_random_key_unk, routineName);
	  }

	  if (errorStringP) osi_Free(errorStringP, sizeof(dce_error_string_t));
	  rtnVal = BZ_SEC_KEY_MGMT_ERROR;
	}

	dfsauthRtnVal = dfsauth_client_CleanupAuthentication();
	if (dfsauthRtnVal) {
	  bossvr_SvcLog(bss_s_localauth_context_teardown_failed2,
			routineName, dfs_dceErrTxt(dfsauthRtnVal));
	  /* don't disguise key error in this case */
	}
      }
      else {
	bossvr_SvcLog(bss_s_setup_localauth_context_failure2,
		      routineName, dfs_dceErrTxt(dfsauthRtnVal));
	rtnVal = BZ_ERROR_INTERNAL_ERROR;
      }
      
      if (secKeyError != error_status_ok) {
	if (secKeyError == sec_key_mgmt_e_unauthorized) {
	  bossvr_SvcLog(bss_s_unex_auth_error2, routineName, calleeName);
	  rtnVal = BZACCESS;	/* this should not happen */
	}
	else if (secKeyError == sec_key_mgmt_e_not_implemented) {
	  rtnVal = BZNOTIMPL;
	}
	else {
	  errorStringP = RgySec_ErrorString(&secKeyError);

	  if (errorStringP) {
	    bossvr_SvcLog(bss_s_error_generating_random_key2,
			  routineName, calleeName,
			  principalNameBuffer, kvno, errorStringP);
	  }
	  else  {
	    bossvr_SvcLog(bss_s_error_generating_random_key2_unk,
			  routineName, calleeName,
			  principalNameBuffer, kvno);
	  }

	  if (errorStringP) osi_Free(errorStringP, sizeof(dce_error_string_t));
	  rtnVal = BZ_SEC_KEY_MGMT_ERROR;
	}
      }
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_GarbageCollectKeys (bosserverBinding, prinNameP, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN	 unsigned char *	prinNameP;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  int			globalLockHeld = 0;
  char			principalNameBuffer[BUFSIZ];
  error_status_t	secKeyError;
  char *		errorStringP = (char *)NULL;
  char			calleeName[BUFSIZ];
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_GarbageCollectKeys";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    if (strlen((char *)prinNameP) != 0) {
      (void)strncpy(principalNameBuffer, (char *)prinNameP, sizeof(principalNameBuffer));
    }
    else {
      rtnVal = dfsauth_PrinName_GetName((char *)NULL, (char *)NULL,
					principalNameBuffer, sizeof(principalNameBuffer));
    }
    
    if (rtnVal == 0) {
      strcpy(calleeName, "sec_key_mgmt_garbage_collect");
      sec_key_mgmt_garbage_collect(dfsauth_sec_key_mgmt_authn_service,
				   (void *)NULL,
				   (unsigned char *)principalNameBuffer,
				   &secKeyError);

      if (secKeyError != error_status_ok) {
	if (secKeyError == sec_key_mgmt_e_unauthorized) {
	  bossvr_SvcLog(bss_s_unex_autherr_from_key_mgt3, routineName);
	  rtnVal = BZACCESS;	/* this should not happen */
	}
	else if (secKeyError == sec_key_mgmt_e_not_implemented) {
	  rtnVal = BZNOTIMPL;
	}
	else {
	  errorStringP = RgySec_ErrorString(&secKeyError);

	  if (errorStringP) {
	    bossvr_SvcLog(bss_s_garbage_collect_of_keys_failed,
			  routineName, calleeName, errorStringP);
	  }
	  else {
	    bossvr_SvcLog(bss_s_garbage_collect_of_keys_failed_unk,
			  routineName, calleeName);
	  }

	  if (errorStringP) osi_Free(errorStringP, sizeof(dce_error_string_t));
	  rtnVal = BZ_SEC_KEY_MGMT_ERROR;
	}
      }
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_GetCellName (bosserverBinding, nameP, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     OUT bossvr_out_string *	nameP;
     OUT error_status_t *	theCommStatus;
{
  char *	cellNameP = (char *)NULL;
  unsigned long	rtnVal = 0;
  int		globalLockHeld = 0;
  int		savedCancelState;
  static char	routineName[] = "BOSSVR_GetCellName";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_NOT_REQD, &savedCancelState);

  if (rtnVal == 0) {
    dce_cf_get_cell_name(&cellNameP, &rtnVal);
    if (rtnVal == dce_cf_st_ok) {
      rtnVal = 0;
    }
    else {
      if (rtnVal == dce_cf_e_file_open) {
	rtnVal = BZIO;
      }
      else if (rtnVal == dce_cf_e_no_mem) {
	rtnVal = BZ_NOMEM;
      }
      else if (rtnVal == dce_cf_e_no_match) {
	rtnVal = BZNOENT;
      }
    }
  }
  
  bossvr_SetUpBossvrOutString(nameP, cellNameP, (rtnVal == 0));

  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_SetTStatus (bosserverBinding, instance, status, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  unsigned char *	instance;
     IN  long			status;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  struct bnode *	instanceBnodeP;
  int			globalLockHeld = 0;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_SetTStatus";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    instanceBnodeP = bnode_FindInstance(instance);
    if (instanceBnodeP != (struct bnode *)NULL) {
      bnode_Hold(instanceBnodeP);
      rtnVal = bnode_SetStat(instanceBnodeP, status);
      bnode_Release(instanceBnodeP);
    }
    else {
      rtnVal = BZNOENT;
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

/* bnode process controlling routines */
static int WaitProc(abnode, arock)
     register struct bnode *abnode;
     char *arock; 
{
  /* only wait for bnodes that are shutting down */
  if (abnode->goal != BSTAT_NORMAL) {
    bnode_Hold(abnode);
    bnode_WaitStatus(abnode, BSTAT_SHUTDOWN);
    bnode_Release(abnode);
  }
  
  return 0;	/* don't stop apply function early, no matter what */
}

static RestartProc(abnode, arock)
     struct bnode *abnode;
     char *arock; 
{
  /* don't restart ones that want to stay stopped */
  if (abnode->fileGoal != BSTAT_SHUTDOWN) {
    bnode_Hold(abnode);
    bnode_SetStat(abnode, BSTAT_NORMAL);
    bnode_Release(abnode);
  }
  
  return 0;
}

static int ShutdownProc(abnode, arock)
     struct bnode *abnode;
     char *arock; 
{
  bnode_Hold(abnode);
  bnode_SetStat(abnode, BSTAT_SHUTDOWN);
  bnode_Release(abnode);
  return 0;
}

/* RPC routines that call bnode process controlling routines */

/*
 * The routine bossvr_ShutdownAll (note lower-case prefix) does the work of shutting
 * down processes, but assumes that the global lock has already been obtained on its
 * behalf.
 */
long bossvr_ShutdownAll ()
{
  long		rtnVal = 0;

  rtnVal = bnode_ApplyInstance(ShutdownProc, (char *)NULL); /* shut down the processes */
  if (rtnVal == 0) {
    rtnVal = bnode_ApplyInstance(WaitProc, (char *)NULL); /* wait until shutdown is done */
  }

  return rtnVal;
}

long BOSSVR_ShutdownAll (bosserverBinding, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  int		globalLockHeld = 0;
  int		savedCancelState;
  static char	routineName[] = "BOSSVR_ShutdownAll";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
/*
    rtnVal = bossvr_ShutdownAll();
 */
    rtnVal = bnode_ApplyInstance(ShutdownProc, (char *)NULL); /* shut down the processes */
  }

  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_RestartAll (bosserverBinding, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  int		globalLockHeld = 0;
  int		savedCancelState;
  static char	routineName[] = "BOSSVR_RestartAll";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    rtnVal = bossvr_ShutdownAll();
    if (rtnVal == 0) {
      rtnVal = bnode_ApplyInstance(RestartProc, (char *)NULL);   /* restart them all */
    }	/* end if restart completed OK */
  }	/* end if shutdown completed OK */
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_StartupAll (bosserverBinding, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  int		globalLockHeld = 0;
  int		savedCancelState;
  static char	routineName[] = "BOSSVR_StartupAll";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);

  if (rtnVal == 0) {
    rtnVal = bnode_ApplyInstance(RestartProc, (char *)NULL);
  }

  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_SetNoAuthFlag (bosserverBinding, flag, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  long			flag;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  int		globalLockHeld = 0;
  int		savedCancelState;
  static char	routineName[] = "BOSSVR_SetNoAuthFlag";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    rtnVal = dfsauth_server_SetNoAuthStatus(flag);
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_ReBossvr (bosserverBinding, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  int			globalLockHeld = 0;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_ReBossvr";

  IMPORT pthread_cond_t	newRestartTimeCond;
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    if (ktime_ParsePeriodic("now", &bossvr_nextRestartKT) == 0) {
      if (pthread_cond_signal(&newRestartTimeCond) != 0) {
	bossvr_SvcLog(bss_s_restarttime_change_failure,
		      routineName, errno);
      }
    }
    else {
      bossvr_SvcLog(bss_s_parse_present_restart_time_error, routineName);
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_Restart (bosserverBinding, instance, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  unsigned char *	instance;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  struct bnode *	instanceBnodeP;
  int			globalLockHeld = 0;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_Restart";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);

  if (rtnVal == 0) {
    instanceBnodeP = bnode_FindInstance(instance);
    if (instanceBnodeP != (struct bnode *)NULL) {
      bnode_Hold(instanceBnodeP);

      bnode_SetStat(instanceBnodeP, BSTAT_SHUTDOWN);
      rtnVal = bnode_WaitStatus(instanceBnodeP, BSTAT_SHUTDOWN); /* this can fail */
      bnode_SetStat(instanceBnodeP, BSTAT_NORMAL);

      bnode_Release(instanceBnodeP);
    }
    else {
      rtnVal = BZNOENT;
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}



#define	BOSSVR_OLDTIME	    (7*24*3600)	    /* 7 days old */
static void SaveOldFiles(aname)
     char *aname;
{
  register long code;
  char bbuffer[256], obuffer[256];
  struct stat tstat;
  register long now;
  long oldTime, bakTime;
  
  strcpy(bbuffer, aname);
  strcat(bbuffer, ".BAK");
  strcpy(obuffer, aname);
  strcat(obuffer, ".OLD");

  now = osi_Time();
  
  code = stat(aname, &tstat);
  if (code >= 0) {
    code = stat(obuffer, &tstat);    /* discover old file's time */
    if (code) oldTime = 0;
    else oldTime = tstat.st_mtime;
    
    code = stat(bbuffer, &tstat);    /* discover back file's time */
    if (code) bakTime = 0;
    else bakTime = tstat.st_mtime;
    
    if (bakTime && ((oldTime == 0) || (bakTime < now - BOSSVR_OLDTIME))) {
      /* no .OLD file, or .BAK is at least a week old */
      code = rename(bbuffer, obuffer);	/* HERE - why assign this value? */
    }
    
    /* finally rename to .BAK extension */
    rename(aname, bbuffer);
  }
}

long BOSSVR_Install (bosserverBinding, path, size, flags, date, thePipeP, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  unsigned char *	path;
     IN  long			size;
     IN  unsigned32		flags;
     IN  long	       		date;
     IN  pipe_t *		thePipeP;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  char			localBuffer[NIDL_PIPE_BUFF_SIZE];
  unsigned long		itemCount;
  long			totalBytes = 0;
  long			bytesWritten;
  int			newFd;
  int			renameCode;
  char			nameBuffer[BUFSIZ];
  char			fullPathName[BUFSIZ];
  char 		      * pathname;
  struct timeval	fileTimesToSet[2];
  int			globalLockHeld = 0;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_Install";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);

  if (rtnVal == 0) {
    /* 
     * check to see if the path name is a full path name.  If not, 
     * make it relative to DCELOCAL and adjust path.
     */
    pathname = (char *)path;
    if (*pathname != '/') {
	(void) sprintf(fullPathName, "%s/%s", dce_base_dir, pathname);
	pathname = fullPathName;
    }
    (void)strcpy(nameBuffer, pathname);
    (void)strcat(nameBuffer, ".NEW");
    newFd = open(nameBuffer, O_CREAT | O_RDWR | O_TRUNC, 0755);
    
    do {
      (*(thePipeP->pull)) (thePipeP->state, (unsigned char *)localBuffer,
			   NIDL_PIPE_BUFF_SIZE, &itemCount);
      if (itemCount > 0) {
	bytesWritten = write(newFd, localBuffer, itemCount);
	totalBytes += bytesWritten;
      }
    } while (itemCount > 0);
    close(newFd);
    
    /* sanity check */
    if (totalBytes != size) {
      unlink(nameBuffer);
      rtnVal = BZINSTALLFAILED;
    }
    
    SaveOldFiles(pathname);	/* the claim in bosserver 3.0 is that we don't care is this
			   doesn't work */
    
    renameCode = rename(nameBuffer, pathname);
    if (renameCode >= 0) {
      fileTimesToSet[0].tv_sec = fileTimesToSet[1].tv_sec = date;
      fileTimesToSet[0].tv_usec = fileTimesToSet[1].tv_usec = 0;
      utimes(pathname, fileTimesToSet);
    }
    else {
/* XXX - here - log errno */
      rtnVal = BZINSTALLFAILED;
    }
  }
  else {
    /* we drain the pipe, but we throw the info on the ground */
    do {
      (*(thePipeP->pull)) (thePipeP->state, (unsigned char *)localBuffer,
			   NIDL_PIPE_BUFF_SIZE, &itemCount);
    } while (itemCount > 0);
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_UnInstall (bosserverBinding, path, flags, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  unsigned char *	path;
     IN  unsigned32		flags;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  char		oldBuffer[BUFSIZ];
  char		bakBuffer[BUFSIZ];
  char		fullPathName[BUFSIZ];
  char	      * pathname;
  int		renameCode;
  int		globalLockHeld = 0;
  int		savedBakErrno = 0;
  int		savedOldErrno = 0;
  int		savedCancelState;
  int		noentCount = 0;
  static char	routineName[] = "BOSSVR_Uninstall";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);

  if (rtnVal == 0) {
    /* 
     * check to see if the path name is a full path name.  If not, 
     * make it relative to DCELOCAL and adjust path.
     */
    pathname = (char *)path;
    if (*pathname != '/') {
	(void) sprintf(fullPathName, "%s/%s", dce_base_dir, pathname);
	pathname = fullPathName;
    }

    (void)strcpy(bakBuffer, pathname);
    (void)strcat(bakBuffer, ".BAK");
    (void)strcpy(oldBuffer, pathname);
    (void)strcat(oldBuffer, ".OLD");
    
    if ((flags & BOSSVR_UNINSTALL_ALL) == 0) {
      renameCode = rename(bakBuffer, pathname);
      savedBakErrno = errno;
      if (renameCode == 0) {
	/* now, rename .OLD to .BAK */
	rename(oldBuffer, bakBuffer);	/* we don't care if this fails */
      }
      else {
	/* can't find .BAK, try .OLD */
	renameCode = rename(oldBuffer, pathname);
	savedOldErrno = errno;
	if (renameCode != 0) {
	  if (savedOldErrno == ENOENT) {
	    rtnVal = BZNOENT;
	  }
	  else {
	    bossvr_SvcLog(bss_s_err_renam_dotold_after_dotbak_mv_failed,
			  routineName, savedOldErrno, savedBakErrno);
	    rtnVal = BZ_ERROR_INTERNAL_ERROR;
	  }
	}
      }
    }
    else {
      if (unlink(pathname) != 0) {
	if (errno == ENOENT) {
	  noentCount++;
	}
	else {
	  bossvr_SvcLog(bss_s_rm_current_file_copy_failed,
			routineName, pathname, errno);
	  rtnVal = BZ_ERROR_INTERNAL_ERROR;
	}
      }
      if (unlink(bakBuffer) != 0) {
	if (errno == ENOENT) {
	  noentCount++;
	}
	else {
	  bossvr_SvcLog(bss_s_rm_bak_file_copy_failed,
			routineName, bakBuffer, errno);
	  rtnVal = BZ_ERROR_INTERNAL_ERROR;
	}
      }
      if (unlink(oldBuffer) != 0) {
	if (errno == ENOENT) {
	  noentCount++;
	}
	else {
	  bossvr_SvcLog(bss_s_rm_old_file_copy_failed,
			routineName, oldBuffer, errno);
	  rtnVal = BZ_ERROR_INTERNAL_ERROR;
	}
      }
      
      if (noentCount == 3) {
	/* we were not able to find any versions of the file */
	rtnVal = BZNOENT;
      }
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}


long BOSSVR_GetDates(bosserverBinding, aname, atime, abakTime, aoldTime, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  unsigned char *	aname;
     OUT long *			atime;
     OUT long *			abakTime;
     OUT long *			aoldTime;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  register long 	code;
  struct stat		tstat;
  char			tbuffer [BOSSVR_BSSIZE];
  char			fullPathName[BUFSIZ];
  char		      * pathname;
  int			globalLockHeld = 0;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_GetDates";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_NOT_REQD, &savedCancelState);

  bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_RPCTRACE,
	   ("BOSSVR_GetDates params: file: %s", aname));
  icl_Trace1(bossvr_iclSetp, BOSSVR_ICL_PROCS_RPCTRACE_9 , ICL_TYPE_STRING, aname);
  
  *atime = 0;
  *abakTime = 0;
  *aoldTime = 0;
  
  if (rtnVal == 0) {
    pathname = (char *)aname;
    if (*pathname != '/') {
	(void) sprintf(fullPathName, "%s/%s", dce_base_dir, pathname);
	pathname = fullPathName;
    }

    code = stat(pathname, &tstat);
    if (code == 0) *atime = tstat.st_mtime;
    
    (void)strcpy(tbuffer, pathname);
    (void)strcat(tbuffer, ".BAK");
    code = stat(tbuffer, &tstat);
    if (code == 0) *abakTime = tstat.st_mtime;
    
    (void)strcpy(tbuffer, pathname);
    (void)strcat(tbuffer, ".OLD");
    code = stat(tbuffer, &tstat);
    if (code == 0) *aoldTime = tstat.st_mtime;
  }  

  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

static int ZapFile(dirName, fileName)
     char *	dirName;
     char *	fileName;
{
  char	fullPathBuffer[BUFSIZ];

  (void)sprintf(fullPathBuffer, "%s/%s", dirName, fileName);
  return unlink(fullPathBuffer);
}

long BOSSVR_Prune (bosserverBinding, flags, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  unsigned32		flags;
     OUT error_status_t *	theCommStatus;
{
  register struct dirent *	dirEntryP;
#define	NULL_DIR_ENTRY_P	(struct dirent *)NULL
  DIR *		dirP;
  int		fileNameLength;
  char *	fileNameSuffix;
  long		rtnVal = 0;
  int		globalLockHeld = 0;
  int		savedCancelState;
  static char	routineName[] = "BOSSVR_Prune";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    /* first check all the binary files for backup and old files, if necessary */
    if (((flags & BOSSVR_PRUNEOLD) != 0) || ((flags & BOSSVR_PRUNEBAK) != 0)) {
      dirP = opendir(afs_bin_dir);
      if (dirP != (DIR *)NULL) {
	for (dirEntryP = readdir(dirP);
	     dirEntryP != NULL_DIR_ENTRY_P;
	     dirEntryP = readdir(dirP)) {
	  fileNameLength = strlen(dirEntryP->d_name);
	  fileNameSuffix = ((fileNameLength > 4) ?
			    dirEntryP->d_name + fileNameLength - 4:
			    (char *)NULL);
	  if (fileNameSuffix != (char *)NULL) {
	    if (((flags & BOSSVR_PRUNEOLD) != 0) &&
		(strncmp(fileNameSuffix, ".OLD", 4) == 0)) {
	      ZapFile(afs_bin_dir, dirEntryP->d_name);
	    }
	    
	    if (((flags & BOSSVR_PRUNEBAK) != 0) &&
		(strncmp(fileNameSuffix, ".BAK", 4) == 0)) {
	      ZapFile(afs_bin_dir, dirEntryP->d_name);
	    }
	  }	/* end if there is a suffix to check */
	}	/* end for loop */	
	closedir(dirP);
      }	/* end if dir pointer is non-null */
    }
    
    /* now look for core files, if necessary */
    if ((flags & BOSSVR_PRUNECORE) != 0) {
      dirP = opendir(afs_server_admin_dir);
      if (dirP != (DIR *)NULL) {
	for (dirEntryP = readdir(dirP);
	     dirEntryP != NULL_DIR_ENTRY_P;
	     dirEntryP = readdir(dirP)) {
	  if (strncmp(dirEntryP->d_name, "core", strlen("core")) == 0) {
	    ZapFile(afs_server_admin_dir, dirEntryP->d_name);
	  }
	}
	closedir(dirP);
      }
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

#define CTIME_YEAR_INDEX	20
#define CTIME_MONTH_OFFSET	4
#define CTIME_TIME_OFFSET	11
long BOSSVR_SetRestartTime (bosserverBinding, type, restartTime, theCommStatus)
     IN	 rpc_binding_handle_t		bosserverBinding;
     IN  long				type;
     IN  struct bossvr_netKTime *		restartTime;
     OUT error_status_t *		theCommStatus;
{
  long			rtnVal = 0;
  int			globalLockHeld = 0;
  long			nowInSecs;
  char *		nowKtimeString;
  unsigned int		i;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_SetRestartTime";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    /* "now" only makes a little sense in this context, so we change it a bit if needed */
    if ((restartTime->mask & KTIME_NOW) != 0) {
      nowInSecs = osi_Time();
      nowKtimeString = (char *)ktime_DateOf(nowInSecs);
      nowKtimeString[CTIME_YEAR_INDEX] = '\0';	/* get rid of the year */
      for (i = 0; i < strlen(nowKtimeString); i++) {
	if (isupper(nowKtimeString[i])) {
	  nowKtimeString[i] = tolower(nowKtimeString[i]);
	}
      }
      
      /* get rid of the month and date */
      for (i = CTIME_MONTH_OFFSET; i < CTIME_TIME_OFFSET; i++) {
	nowKtimeString[i] = ' ';
      }
      
      rtnVal = ktime_ParsePeriodic(nowKtimeString, (struct ktime *)restartTime);
    }
    
    if (rtnVal == 0) {
      switch(type) {
      case 1:
	bossvr_nextRestartKT = *((struct ktime *) restartTime);
	break;
	
      case 2:
	bossvr_nextDayKT = *((struct ktime *) restartTime);
	break;
	
      default:
	rtnVal = BZDOM;
	break;
      }
      
      if (rtnVal == 0) {
	rtnVal = WriteBossvrFile(0);
	if (pthread_cond_signal(&newRestartTimeCond) != 0) {
	  bossvr_SvcLog(bss_s_signal_new_restarttime_failed_again,
			routineName, errno);
	}
      }
    }
    else {
      rtnVal = BZDATEPARSINGERROR;
    }
  }

  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_GetRestartTime (bosserverBinding, type, restartTime, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  long				type;
     OUT struct bossvr_netKTime *		restartTime;
     OUT error_status_t *		theCommStatus;
{
  long		rtnVal = 0;
  int		globalLockHeld = 0;
  int		savedCancelState;
  static char	routineName[] = "BOSSVR_GetRestartTime";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_NOT_REQD, &savedCancelState);
  
  bzero((char *)restartTime, sizeof(struct bossvr_netKTime));
  
  if (rtnVal == 0) {
    switch(type) {
      case 1:
	*((struct ktime *) restartTime) = bossvr_nextRestartKT;
	break;
	
      case 2:
	*((struct ktime *) restartTime) = bossvr_nextDayKT;
	break;

      default:
	rtnVal = BZDOM;
	break;
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}


long BOSSVR_GetLog (bosserverBinding, name, thePipeP, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  unsigned char *	name;
     OUT pipe_t *		thePipeP;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  ndr_byte	dataBuffer[NIDL_PIPE_BUFF_SIZE];
  int		byteCount;
  FILE *	theStreamP;
  int		globalLockHeld = 0;
  int		authznReqd;
  int		savedCancelState;
  static char	routineName[] = "BOSSVR_GetLog";

  authznReqd = ((index((char *)name, '/') != (char *)NULL) ?
		BOSSVR_AUTHZN_IS_REQD : BOSSVR_AUTHZN_IS_NOT_REQD);
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			  authznReqd, &savedCancelState);
  
  if (rtnVal == 0) {
    theStreamP = fopen((char *)name, "r");
    if (theStreamP != (FILE *)NULL) {
      byteCount = fread((char *)dataBuffer, sizeof(ndr_byte), NIDL_PIPE_BUFF_SIZE, theStreamP);
      while (byteCount > 0) {
	(*(thePipeP->push))(thePipeP->state, dataBuffer, byteCount);
	byteCount = fread((char *)dataBuffer, sizeof(ndr_byte), NIDL_PIPE_BUFF_SIZE, theStreamP);
      }
      fclose(theStreamP);
    }
    else {
      rtnVal = BZNOENT;
    }
  }
  
  /* always terminate the output pipe */
  (*(thePipeP->push))(thePipeP->state, (unsigned char *)NULL, 0);

  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_WaitAll (bosserverBinding, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  int		globalLockHeld = 0;
  int		savedCancelState;
  static char	routineName[] = "BOSSVR_WaitAll";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);

  if (rtnVal == 0) {
    rtnVal = bnode_WaitAll();
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

long BOSSVR_SetDebug (bosserverBinding, newDebugValP, oldDebugValP, theCommStatus)
     IN	 rpc_binding_handle_t	bosserverBinding;
     IN  long *			newDebugValP;
     IN  long *			oldDebugValP;
     OUT error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  int			globalLockHeld = 0;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_SetDebug";
  
#if !defined(DFS_BOSSERVER_ALLOW_SETDEBUG)
  rtnVal = BZNOTIMPL;
#else /* !defined(dfs_bos_debug_bosserver) */
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    *oldDebugValP = bossvr_debug_flag;
    if (newDebugValP != (long *)NULL) {
      bossvr_debug_flag = *newDebugValP;
    }
  }

  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
#endif /* !defined(DFS_BOSSERVER_ALLOW_SETDEBUG) */
  return rtnVal;
}

long int BOSSVR_GetServerInterfaces (bosserverBinding, serverInterfacesP, theCommStatus)
     IN    handle_t		bosserverBinding;
     INOUT dfs_interfaceList *	serverInterfacesP;
     OUT   error_status_t *	theCommStatus;
{
  long			rtnVal = 0;
  int			globalLockHeld = 0;
  error_status_t	compatCode;
  char *		errorStringP = (char *)NULL;
  int			savedCancelState;
  static char		routineName[] = "BOSSVR_GetServerInterfaces";
  
  rtnVal = BossvrCommonInit(routineName, theCommStatus, &globalLockHeld, &bosserverBinding,
			    BOSSVR_AUTHZN_IS_NOT_REQD, &savedCancelState);
  
  if (rtnVal == 0) {
    /* call to get the interfaces supported bu the server */
    dfs_GetServerInterfaces (BOSSVR_v0_0_s_ifspec, serverInterfacesP, &compatCode);
    if (compatCode != error_status_ok) {
      errorStringP = RgySec_ErrorString(&compatCode);

      if (errorStringP) {
        bossvr_SvcLog(bss_s_cant_get_svr_intfc_from_ncscompat,
		      routineName, errorStringP);
      }
      else {
        bossvr_SvcLog(bss_s_cant_get_svr_intfc_from_ncscompat_unk, routineName);
      }

      if (errorStringP) osi_Free(errorStringP, sizeof(dce_error_string_t));
      rtnVal = BZ_ERROR_INTERNAL_ERROR;
    }
  }
  
  rtnVal = BossvrCommonTerm(routineName, rtnVal, globalLockHeld, savedCancelState);
  return rtnVal;
}

