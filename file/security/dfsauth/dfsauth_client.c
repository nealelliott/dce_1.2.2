/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *	dfsauth_client.c -- client-side routines for the DFS authentication package
 *
 * Copyright (C) 1995, 1991 Transarc Corporation
 * All rights reserved.
 */

#include <stdio.h>
  
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
  
#if !defined(KERNEL)
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */
#endif /* !defined(KERNEL) */

/* we still need this because we may still get Kerberos errors from security routines */  
#include <krb5/krb5.h>

#include <dce/nbase.h>
#include <dce/sec_login.h>
#include <dce/keymgmt.h>
  
#include <dcedfs/dacl.h>
#include <dfsauth.h>
  
#include <../dfssec_errorStrings.h>
#include <dfsauth_localprincipal.h>
#include <dfsauth_localkey.h>

#include <dfsauth_debug.h>

#include <../exception_print.h>

#include <dcedfs/icl.h>

/* Must follow inclusion of pthread_exc.h, in case cma wrapper for wait is declared */
#include <sys/wait.h>
  
#include <dfsauth_trace.h>

IMPORT struct icl_set *dfsauth_iclSetp;

PRIVATE int			errorTablesInitialized = 0;
PRIVATE sec_login_handle_t	dfsauth_client_loginHandle;
PRIVATE int			savedAuthnLevelGood = 0;
PRIVATE unsigned32		savedAuthnLevel = rpc_c_authn_level_default;

PRIVATE pthread_mutexattr_t	dfsauth_client_mutexattr;
PRIVATE pthread_mutex_t		dfsauth_client_mutex;
PRIVATE pthread_once_t		dfsauth_client_lockInitOnce = pthread_once_init;

PRIVATE long			dfsauth_client_InitAuthContext();

/* the server code needs these if it has to create its access list */
EXPORT int			dfsauth_client_initialized = 0;
EXPORT int			dfsauth_client_localauthset = 0;

EXPORT signed32			dfsauth_client_expiration = 0;

#define DFSAUTH_CLIENT_EXPIRATION_GRACE	(5*60) /* 5 minute grace period */


/*
 * We have to use exceptions here, since this can only be a void operation, because
 * it will be called through pthread_once().
 */
PRIVATE void dfsauth_client_LockInit()
{
  static char	routineName[] = "dfsauth_client_LockInit";
  
  /* apparently, PTHREAD_MUTEXATTR_DEFAULT is undefined, despite the documentation */
  TRY
    pthread_mutexattr_create(&dfsauth_client_mutexattr);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_mutexattr_create",
		   __FILE__, __LINE__);
    RERAISE;
  ENDTRY;

  TRY
    pthread_mutex_init(&dfsauth_client_mutex, dfsauth_client_mutexattr);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_mutex_init", __FILE__, __LINE__);
    RERAISE;
  ENDTRY;

  TRY
    pthread_mutexattr_delete(&dfsauth_client_mutexattr);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_mutexattr_delete",
		   __FILE__, __LINE__);
    RERAISE;
  ENDTRY;
}

/*
 * The following routine tries to grab the client package lock, initializing it if
 * necessary.  We have to use exceptions here or move this routine into another file,
 * due to the pre-processor implementation of the pthreads routines.
 */
PRIVATE long dfsauth_client_GrabLock()
{
  volatile long	rtnVal = 0;
  static char	routineName[] = "dfsauth_client_GrabLock";

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_0 );
  
  TRY
    pthread_once(&dfsauth_client_lockInitOnce, dfsauth_client_LockInit);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_once", __FILE__, __LINE__);
    rtnVal = DAUT_ERROR_THREAD_ERROR;
  ENDTRY;

  if (rtnVal == 0) {
    TRY
      pthread_mutex_lock(&dfsauth_client_mutex);
    CATCH_ALL
      PrintException(THIS_CATCH, routineName, "pthread_mutex_unlock", __FILE__, __LINE__);
      rtnVal = DAUT_ERROR_THREAD_ERROR;
    ENDTRY;
  }
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_1 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_2 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
  
  return rtnVal;
}

PRIVATE long dfsauth_client_ReleaseLock()
{
  volatile long	rtnVal = 0;
  static char	routineName[] = "dfsauth_client_ReleaseLock";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_3 );
  
  TRY
    pthread_mutex_unlock(&dfsauth_client_mutex);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_mutex_unlock", __FILE__, __LINE__);
    rtnVal = DAUT_ERROR_THREAD_ERROR;
  ENDTRY;
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_4 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_5 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
  
  return rtnVal;
}

#define DFSAUTH_CLIENT_ERROR_CLAUSE(calledRoutineName)			\
  if (exceptionRaised) {						\
    dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,				\
	    ("%s: %s raised an exception; see stdout", 			\
	     routineName, calledRoutineName));				\
    rtnVal = DAUT_ERROR_SEC_RGY_PGO_ERROR;				\
  }									\
  else {								\
    dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,				\
	    ("%s: %s returned error: %s",				\
	     routineName, calledRoutineName,				\
	     RgySec_ErrorString((error_status_t*)&secErrorStatus)));			\
    rtnVal = (long)secErrorStatus;						\
  }									\

PRIVATE long dfsauth_client_EstablishLocalAuthContext(refresh)
     int	refresh;
{
  volatile long		rtnVal = 0;
  volatile int		exceptionRaised = 0;
  char			localPrincipalName[BUFSIZ];
  volatile boolean32	setupRtn;
  boolean32		validationRtn = false;
  int			childPid;
  int			continueOK = 0;
  int			tryNextKey = 0;
  sec_passwd_rec_t *	passkeyRecordP;
  char *		keyCursorP;
  unsigned32		kvno;
  unsigned32		keyCounter = 0;
  sec_passwd_type_t	keytype = dfsauth_sec_key_mgmt_default_keytype;
  unsigned32		try_kvno = sec_passwd_c_version_none;	/* should be 0 */
  unsigned32		used_kvno = sec_passwd_c_version_none;	/* should be 0 */
  boolean32		password_reset_required = false;
  sec_login_auth_src_t	auth_source;

#if !defined(KERNEL)
  /* union wait is now obsolete */
  int		waitStatus;
#endif /* !defined(KERNEL) */
  volatile error_status_t	secErrorStatus = error_status_ok;
  static char		routineName[] = "dfsauth_EstablishLocalAuthContext";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_6 );
  
  /* if we're coming in through the init code, the lock has already been grabbed */

  continueOK = ((refresh && dfsauth_client_initialized && dfsauth_client_localauthset) ||
		 (!refresh));

  if (continueOK) {
    if (getuid() == 0) {
      rtnVal = dfsauth_PrinName_GetBaseName((char *)NULL, localPrincipalName,
					    sizeof(localPrincipalName));
      if (rtnVal == 0) {
	rtnVal = dfsauth_PrinName_MakePrincipalName(localPrincipalName,
						    sizeof(localPrincipalName));
      }
      
      if (rtnVal == 0) {
	if (refresh == 0) {
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		   ("%s: about to call sec_login_setup_identity with name: `%s'",
		    routineName, localPrincipalName));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_7 , ICL_TYPE_STRING, localPrincipalName);

	  TRY
	    exceptionRaised = 0;
	    setupRtn = sec_login_setup_identity(
		(unsigned char *)localPrincipalName, sec_login_no_flags,
		&dfsauth_client_loginHandle, (error_status_t *)&secErrorStatus);
	  CATCH_ALL
	    PrintException(THIS_CATCH, routineName, "sec_login_setup_identity",
			   __FILE__, __LINE__);
	    exceptionRaised = 1;
	  ENDTRY;

	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		   ("%s: sec_login_setup_identity returned", routineName));
	  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_8 );
	}
	else {
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		   ("%s: about to call sec_login_refresh_identity with name: `%s'",
		    routineName, localPrincipalName));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_9 , ICL_TYPE_STRING, localPrincipalName);

	  TRY
	    exceptionRaised = 0;
	    setupRtn = sec_login_refresh_identity(dfsauth_client_loginHandle,
					(error_status_t *)&secErrorStatus);
	  CATCH_ALL
	    PrintException(THIS_CATCH, routineName, "sec_login_refresh_identity",
			   __FILE__, __LINE__);
	    exceptionRaised = 1;
	  ENDTRY;

	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		   ("%s: sec_login_refresh_identity returned", routineName));
	  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_10 );
	}
	
	if ((secErrorStatus == error_status_ok) && (setupRtn == true) &&
	    ! exceptionRaised) {
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		   ("%s: about to call sec_login_valid_from_keytable", routineName));
	  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_11 );
	  
	  TRY
	    exceptionRaised = 0;
	    sec_login_valid_from_keytable(dfsauth_client_loginHandle,
					  dfsauth_sec_key_mgmt_authn_service,
					  NULL,
					  try_kvno,
					  &used_kvno,
					  &password_reset_required,
					  &auth_source,
					  (error_status_t *)&secErrorStatus);
	  CATCH_ALL
	    PrintException(THIS_CATCH, routineName, "sec_login_valid_from_keytable",
			   __FILE__, __LINE__);
	    exceptionRaised = 1;
	  ENDTRY;
	  
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		   ("%s: returned from sec_login_valid_from_keytable", routineName));
	  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_12 );
	  
	  if ((secErrorStatus == error_status_ok) && !exceptionRaised) {
	    validationRtn = true;
	  }
	  else {
	    if (exceptionRaised) {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		      ("%s: sec_login_valid_from_keytable raised an exception; see stdout",
		       routineName));
	      icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_2 );
	      rtnVal = DAUT_ERROR_SEC_RGY_PGO_ERROR;
	    }
	    else {
	      if (secErrorStatus == sec_key_mgmt_e_unauthorized) {
		dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
			("%s: unexpected authorization error returned from sec_login_valid_from_keytable\n",
			  routineName));
		icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_3 );
		rtnVal = (long)secErrorStatus;
	      }
	      else if (secErrorStatus == sec_key_mgmt_e_key_unavailable) {
		rtnVal = DAUT_ERROR_NO_VALID_KEY;
	      }
	      else {
		dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
			("%s: error from sec_login_valid_from_keytable for principal %s: %s\n",
			  routineName, localPrincipalName,
			  RgySec_ErrorString((error_status_t *)&secErrorStatus)));
		icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_4,
		    ICL_TYPE_STRING, localPrincipalName, ICL_TYPE_STRING,
		    RgySec_ErrorString((error_status_t *)&secErrorStatus));
		rtnVal = (long)secErrorStatus;
	      }
	    }
	  }
	    
	  if ((refresh == 0) &&
	      (secErrorStatus == error_status_ok) && (validationRtn == true) &&
	      ! exceptionRaised) {
	    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		     ("%s: about to call sec_login_set_context", routineName));
	    icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_13 );
	    
	    TRY
	      exceptionRaised = 0;
	      sec_login_set_context(dfsauth_client_loginHandle,
				(error_status_t *)&secErrorStatus);
	    CATCH_ALL
	      PrintException(THIS_CATCH, routineName, "sec_login_set_context",
			     __FILE__, __LINE__);
	      exceptionRaised = 1;
	    ENDTRY;
	    
	    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		     ("%s: sec_login_set_context returned", routineName));
	    icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_14 );
	    
	    if ((secErrorStatus == error_status_ok) && ! exceptionRaised) {
#ifdef notdef
#if !defined(KERNEL)
	      if ((childPid = fork()) != 0) {
		/* parent */
		while (wait(&waitStatus) != childPid) {
		  /* intentionally left blank */
		}
		sec_login_purge_context(&dfsauth_client_loginHandle,
					(error_status_t *)&secErrorStatus);
		if (secErrorStatus == error_status_ok) {
		  exit(0);
		}
		else {
		  dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
			  ("%s: sec_login_purge_context error: %s",
			   routineName,
			   RgySec_ErrorString((error_status_t *)&secErrorStatus)));
		  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_5,
			     ICL_TYPE_STRING,
			     RgySec_ErrorString((error_status_t *)&secErrorStatus));
		  exit(1);
		}
	      }
	      /* the child just continues normal execution */
#endif /* !defined(KERNEL) */
#endif /* notdef */
	    }
	    else {
	      if (exceptionRaised) {
		dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
			("%s: sec_login_set_context raised an exception; see stdout",
			 routineName));
		icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_6 );
		rtnVal = DAUT_ERROR_SEC_RGY_PGO_ERROR;
	      }
	      else {
		dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		    ("%s: sec_login_set_context error: %s",
		     routineName,
		     RgySec_ErrorString((error_status_t *)&secErrorStatus)));
		icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_7,
		     ICL_TYPE_STRING,
		     RgySec_ErrorString((error_status_t *)&secErrorStatus));
		rtnVal = (long)secErrorStatus;
	      }
	    }
	  }
	}
	else {
	  DFSAUTH_CLIENT_ERROR_CLAUSE("sec_rgy_pgo_setup_identity (or refresh)");
	}
      }
      else {
	if (rtnVal == DAUT_ERROR_FS_OPEN) {
	  /* probably trying to use -localauth on a machine w/o a local identity */
	  rtnVal = DAUT_ERROR_NO_LOCAL_IDENTITY;
	}
	
#if !defined(KERNEL)
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		 ("%s: dfsauth_PrinName_GetBaseName reported failure: %s", routineName,
		  dfs_dceErrTxt(rtnVal)));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_WARNINGS_15 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		 ("%s: dfsauth_PrinName_GetBaseName reported failure: %lu", routineName,
		  rtnVal));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_WARNINGS_16 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
      }
    }
    else {
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	       ("%s: attempt to setup local auth, not from root", routineName));
      icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_17 );
      rtnVal = DAUT_ERROR_UID_NOT_LOCAL_ROOT;
    }

    if (rtnVal == 0) {
	sec_login_get_expiration(dfsauth_client_loginHandle,
				 &dfsauth_client_expiration,
				 (error_status_t *)&secErrorStatus);
	if (secErrorStatus != error_status_ok &&
	    secErrorStatus != sec_login_s_not_certified) {
	    dfsauth_client_expiration = 0;
	    rtnVal = (long)secErrorStatus;
	}
    }

    if (rtnVal == 0) {
      dfsauth_client_localauthset = 1;
    }
  }
  else {
    /* can only get here if trying to refresh and something went wrong */
    /* if rtnVal != 0, error indicated earlier in code path */
    if (rtnVal == 0) {
      /* error caused by lack of client init */
      rtnVal = DAUT_ERROR_CLIENT_NOT_INITIALIZED;
    }
  }
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_18 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_19 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
  
  return rtnVal;
}

PRIVATE long dfsauth_client_SetLocalAuthContext()
{
  /* Callers of this routine already hold the client mutex */
  return dfsauth_client_EstablishLocalAuthContext(0 /* => not a refresh */);
}

PRIVATE long dfsauth_client_CheckLocalAuthContext()
{
  long lockStatus = 0;
  long rtnVal = 0;
  static char routineName[] = "dfsauth_client_CheckLocalAuthContext";

  /*
   * This routine only does the refresh if the local auth context is
   * about to expire.
   */

  if (lockStatus = dfsauth_client_GrabLock()) {
    osi_printf("%s: unable to obtain dfsauth client package mutex\t%s, %d\n",
	       routineName, __FILE__, __LINE__);
    rtnVal = lockStatus;
  }

  if (rtnVal == 0) {
    time_t	now = osi_Time();

    if (dfsauth_client_expiration <= now + DFSAUTH_CLIENT_EXPIRATION_GRACE) {
      rtnVal = dfsauth_client_EstablishLocalAuthContext(1 /* => this is a refresh */);
    }

    if (lockStatus = dfsauth_client_ReleaseLock()) {
      osi_printf("%s: unable to release dfsauth client package mutex\t%s, %d\n",
		 routineName, __FILE__, __LINE__);

      if (rtnVal == 0) {
	  /* only return this value if returning it doesn't mask other errors */
	  rtnVal = lockStatus;
      }
    }
  }

  return rtnVal;
}

EXPORT long dfsauth_client_RefreshLocalAuthContext()
{
  long lockStatus = 0;
  long rtnVal = 0;
  static char routineName[] = "dfsauth_client_RefreshLocalAuthContext";

  if (lockStatus = dfsauth_client_GrabLock()) {
      osi_printf("%s: unable to obtain dfsauth client package mutex\t%s, %d\n",
		 routineName, __FILE__, __LINE__);
      rtnVal = lockStatus;
  }

  if (rtnVal == 0) {
      rtnVal = dfsauth_client_EstablishLocalAuthContext(1 /* => this is a refresh */);

      if (lockStatus = dfsauth_client_ReleaseLock()) {
	  osi_printf("%s: unable to release dfsauth client package mutex\t%s, %d\n",
		     routineName, __FILE__, __LINE__);

	  if (rtnVal == 0) {
	      /* only return this value if returning it doesn't mask other errors */
	      rtnVal = lockStatus;
	  }
      }
  }

  return rtnVal;
}

#ifndef KERNEL
PRIVATE AutoRefreshThread(arg)
  pthread_addr_t arg;
{
    long expire;
    long grace;
    long now;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    int code;
    struct timespec ts;
    error_status_t st;
    static char		routineName[] = "AutoRefreshThread";

    bzero((char *)&ts, sizeof(ts));
    code = pthread_cond_init(&cond, pthread_condattr_default);
    if (code) {
	osi_printf("%s: unable to initialize condition variable\t%s, %d\n",
		   routineName, __FILE__, __LINE__);
    }
    else {
	code = pthread_mutex_init(&mutex, pthread_mutexattr_default);
	if (code) {
	    osi_printf("%s: unable to initialize mutex\t%s, %d\n",
		       routineName, __FILE__, __LINE__);
	}
	else {
	    pthread_mutex_lock(&mutex);

	    /* main loop */
	    while(1) {
		sec_login_get_expiration(dfsauth_client_loginHandle, &expire, &st);
		if (st == sec_login_s_not_certified) {
		    st = 0;	/* just a warning */
		}
		if ((st != error_status_ok) || (expire == 0)) {
		    osi_printf("%s: unable to get expiration time of context (status=%d)\t%s, %d\n",
			       routineName, st, __FILE__, __LINE__);
		    icl_Trace1(dfsauth_iclSetp,
			       DFSAUTH_ICL_CLIENT_TRACE_NOEXPTIME,
			       ICL_TYPE_LONG, st);
		    /* Don't fail on this: default to min expiration time of 5m */
		    expire = time((long *)0) + (5*60);
		}
		dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
			 ("AutoRefreshThread: TGT will expire at %d", expire));
		icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_20 , ICL_TYPE_LONG, expire);
		

		/* the TGT will expire at 'expire', wake up 'grace' seconds in advance */
		/* compute 'grace' to be the lesser of 10 minutes or 1/2 the difference */
		now = time((long *)0);
		grace = (expire - now) / 2;
		if (grace > 10*60)
		    grace = 10*60;

		ts.tv_sec = expire - grace;
		pthread_cond_timedwait(&cond, &mutex, &ts);

		code = 1;
		if (dfsauth_client_GrabLock() == 0) {
		    code = dfsauth_client_initialized;
		    if (dfsauth_client_ReleaseLock()) {
			osi_printf("%s: unable to release dfsauth client package mutex\t%s, %d\n",
				   routineName, __FILE__, __LINE__);
		    }
		}
		if (code == 0) {
		    /* Time to exit */
		    icl_Trace0(dfsauth_iclSetp,
			       DFSAUTH_ICL_CLIENT_TRACE_THREADEXIT);
		    break;
		}
		dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
			 ("AutoRefreshThread: Refreshing TGT"));
		icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_21);
		if (dfsauth_client_RefreshLocalAuthContext() != 0)
		{
		    now = time((long *)0);
		    ts.tv_sec = now + 3*60;	/* failed, try again later */
		    pthread_cond_timedwait(&cond, &mutex, &ts);
		}		    
	    }

	    /* clean up after ourselves */
	    pthread_mutex_unlock(&mutex);
	    pthread_mutex_destroy(&mutex);
	}
	pthread_cond_destroy(&cond);
    }
}

EXPORT long dfsauth_client_AutoRefreshLocalAuthContext()
{
    int	copy_dfsauth_client_initialized;
    static int	threadStarted = 0;
    long	rtnVal = 0;
    static char	routineName[] = "dfsauth_client_AutoRefreshLocalAuthContext";
    pthread_t	refreshThreadID;

    copy_dfsauth_client_initialized = 0;
    if (dfsauth_client_GrabLock() == 0) {
	copy_dfsauth_client_initialized = dfsauth_client_initialized;
	if (dfsauth_client_ReleaseLock()) {
	    osi_printf("%s: unable to release dfsauth client package mutex\t%s, %d\n",
		       routineName, __FILE__, __LINE__);
	}
    }

    if (copy_dfsauth_client_initialized == 0) {
	rtnVal = dfsauth_client_InitAuthContext(/*useNoAuth*/0, /*useLocalAuth*/1);
    } else {
	if (threadStarted)
	    threadStarted = 0;
    }

    /* start a thread to do the auto-refresh */
    if (rtnVal == 0 && threadStarted == 0) {
	rtnVal = pthread_create(&refreshThreadID, pthread_attr_default, 
				(pthread_startroutine_t)AutoRefreshThread, 
				(pthread_addr_t)NULL);
	if (rtnVal == 0)
	    threadStarted = 1;
    }
    return rtnVal;
}
#endif /* !KERNEL */

EXPORT long dfsauth_client_IsClientAuthenticated()
{
  long			rtnVal = 0;	/* assume unauthenticated */
  sec_login_handle_t	currentContext;
  error_status_t	contextError = error_status_ok;
  static char		routineName[] = "dfsauth_client_IsClientAuthenticated";
  
  sec_login_get_current_context(&currentContext, &contextError);
  if (contextError == error_status_ok) {
    rtnVal = 1;
  }
  else {
    if (contextError != sec_login_s_no_current_context) {
      fprintf(stderr,
	      "%s: unexpected return code from sec_login_get_current_context: %d\n",
	      routineName, contextError);
    }
  }

  return rtnVal;
}

/* routines that do the real work go here */
PRIVATE long dfsauth_client_InitAuthContext(useNoAuth, useLocalAuth)
     int	useNoAuth;
     int	useLocalAuth;
{
  long		rtnVal = 0;
  long		subStatus = 0;
  int		checkLocalAuth = 0;
  int		needThread = 0;
  static char	routineName[] = "dfsauth_client_InitAuthContext";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_22 );
  
#if !defined(FORCE_NOSECURITY)
  if ((rtnVal = dfsauth_client_GrabLock()) == 0) {
    if (dfsauth_client_initialized == 0) {
      if (errorTablesInitialized == 0) {
#if !defined(KERNEL)
	initialize_dau_error_table();
	initialize_dcl_error_table();
#endif /* !defined(KERNEL) */
	
	errorTablesInitialized = 1;
      }
      
      /* initialize the tracing package */
      if (dfsauth_InitTracingPackage() != 0) {
      osi_printf("%s: error initializing DFS trace package\t%s, %d\n",
		 routineName, __FILE__, __LINE__);
      }

      if ((useNoAuth == 0) && (useLocalAuth != 0)) {
	rtnVal = dfsauth_client_SetLocalAuthContext();
	needThread = 1;
      }

      dfsauth_client_initialized = 1;
    }
    else {
      /*
       * we're already initialized, so we'll go ahead and refresh
       * the localauth context, if necessary.
       */
      if ((useNoAuth == 0) && (useLocalAuth != 0)) {
	  checkLocalAuth = 1;
      }
    }
    if (subStatus = dfsauth_client_ReleaseLock()) {
      osi_printf("%s: unable to release dfsauth client package mutex\t%s, %d\n",
		 routineName, __FILE__, __LINE__);

      if (rtnVal == 0) {	/* don't mask other errors with this one */
	  rtnVal = subStatus;
      }
    }
    if (needThread) {
	subStatus = dfsauth_client_AutoRefreshLocalAuthContext();
	if (rtnVal == 0)
	    rtnVal = subStatus;
    }
    if (checkLocalAuth) {
	subStatus = dfsauth_client_CheckLocalAuthContext();
	if (rtnVal == 0)
	    rtnVal = subStatus;
    }
  }
  else {
    osi_printf("%s: unable to obtain dfsauth client package mutex\t%s, %d\n",
	       routineName, __FILE__, __LINE__);
  }

#endif /* !defined(FORCE_NOSECURITY) */
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_23 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_24 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */

  return rtnVal;
}

EXPORT long dfsauth_client_InitBindingAuth(serverBindingHandleP, useNoAuth, useLocalAuth,
					   serverPrincipalNameP)
     rpc_binding_handle_t *	serverBindingHandleP;
     int			useNoAuth;
     int			useLocalAuth;
     char *			serverPrincipalNameP;
{
  volatile long		rtnVal = 0;
  volatile int		exceptionRaised = 0;
  unsigned32		authnSvc = rpc_c_authn_dce_private;
  unsigned32		authnLevel = rpc_c_authn_level_default;
  volatile error_status_t	secErrorStatus;
  char			serverPrincipalName[BUFSIZ];
  static char		routineName[] = "dfsauth_client_InitBindingAuth";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_25 );

#if !defined(FORCE_NOSECURITY)

  if ((serverBindingHandleP != (rpc_binding_handle_t *)NULL) &&
      (serverPrincipalNameP != (char *)NULL)) {

    /* the init routine will test for init condition, itself */
    rtnVal = dfsauth_client_InitAuthContext(useNoAuth, useLocalAuth);
    
    if (rtnVal == 0) {
#if !defined(FORCE_NOAUTH)
      if (useNoAuth == 0) {
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		 ("%s: about to call rpc_binding_inq_auth_info\n", routineName));
	icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_26 );

	TRY
	  exceptionRaised = 0;
	  rpc_binding_inq_auth_info(*serverBindingHandleP,
				    (unsigned_char_p_t *)NULL,
				    (unsigned32 *)NULL,
				    (unsigned32 *)NULL,
				    (rpc_auth_identity_handle_t *)NULL,
				    (unsigned32 *)NULL,
				    (error_status_t *)&secErrorStatus);
	CATCH_ALL
	  PrintException(THIS_CATCH, routineName, "rpc_binding_inq_auth_info",
			 __FILE__, __LINE__);
	  exceptionRaised = 1;
	ENDTRY;

	if ((secErrorStatus == rpc_s_binding_has_no_auth) && ! exceptionRaised) {
	  /*
	   *  We call this just in case we didn't get a real principal name,
	   * but did get something that can be turned into a principal name.
	   * If we did get a real principal name, this won't hurt it.
	   */
	  rtnVal = dfsauth_PrinName_GetName(serverPrincipalNameP, (char *)NULL,
					    serverPrincipalName,
					    sizeof(serverPrincipalName));
	  if (rtnVal == 0) {
	    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		     ("%s: about to call rpc_binding_set_auth_info, authn svc = %s, principal name: %s",
		      routineName, 
		      ((authnSvc == rpc_c_authn_none) ?
		       "rpc_c_authn_none" : "rpc_c_authn_dce_private"),
		      serverPrincipalNameP));
	    icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_27 , ICL_TYPE_STRING, ((authnSvc==rpc_c_authn_none)?"rpc_c_authn_none":"rpc_c_authn_dce_private"), ICL_TYPE_STRING, serverPrincipalNameP);
	    
	    TRY
	      exceptionRaised = 0;
	      rpc_binding_set_auth_info(*serverBindingHandleP,
					(unsigned char *)serverPrincipalNameP,
					authnLevel,
					authnSvc,
					(rpc_auth_identity_handle_t)NULL,
					rpc_c_authz_dce,
					(error_status_t *)&secErrorStatus);
	    CATCH_ALL
	      PrintException(THIS_CATCH, routineName, "rpc_binding_set_auth_info",
			     __FILE__, __LINE__);
	      exceptionRaised = 1;
	    ENDTRY;
	    
	    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		     ("%s: rpc_binding_set_auth_info returned", routineName));
	    icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_28 );
	    
	    if ((secErrorStatus != error_status_ok) || exceptionRaised) {
	      if (exceptionRaised) {
		dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
			("%s: rpc_binding_set_auth_info raised an exception; see stdout",
			 routineName));
		icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_8 );
		rtnVal = DAUT_ERROR_RPC_ERROR;
	      }
	      else {
		dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
			("%s: rpc_binding_set_auth_info error: %s",
			 routineName,
			RgySec_ErrorString((error_status_t *)&secErrorStatus)));
		icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_9,
			ICL_TYPE_STRING,
			RgySec_ErrorString((error_status_t *)&secErrorStatus));
		if ((secErrorStatus == sec_login_s_no_current_context) ||
		    (secErrorStatus == KRB5_FCC_NOFILE)) {
		  rtnVal = DAUT_ERROR_NO_CREDENTIALS_FOUND;
		}
		else {
		  rtnVal = (long)secErrorStatus;
		}
	      }
	    }
	  }
	  else {
#if !defined(KERNEL)
	    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		     ("%s: PrinName_GetName reported failure: %s", routineName,
		      dfs_dceErrTxt(rtnVal)));
	    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_WARNINGS_29 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
	    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		     ("%s: PrinName_GetName reported failure: %lu", routineName,
		      rtnVal));
	    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_WARNINGS_30 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
	  }
	}
	else {
	  if (exceptionRaised) {
	    dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		    ("%s: rpc_binding_inq_auth_info raised an exception; see stdout",
		     routineName));
	    icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_10 );
	    rtnVal = DAUT_ERROR_RPC_ERROR;
	  }
	  else {
	    if (secErrorStatus != error_status_ok) {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		      ("%s: rpc_binding_inq_auth_info error: %s",
		       routineName,
		      RgySec_ErrorString((error_status_t *)&secErrorStatus)));
	      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_11,
		      ICL_TYPE_STRING,
		      RgySec_ErrorString((error_status_t *)&secErrorStatus));
	      rtnVal = (long)secErrorStatus;
	    }
	  }
	}
      }
#endif /* !defined(FORCE_NOAUTH) */
    }
  }
  else {
    if (serverBindingHandleP != (rpc_binding_handle_t *)NULL) {
      dfsauth_LogParamError(routineName, "serverBindingHandleP");
    }
    if (serverPrincipalNameP != (char *)NULL) {
      dfsauth_LogParamError(routineName, "serverPrincipalNameP");
    }

    rtnVal = DAUT_ERROR_PARAMETER_ERROR;
  }

#endif /* !defined(FORCE_NOSECURITY) */
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_31 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_32 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */

  return rtnVal;
}

EXPORT long dfsauth_client_InitAuthentication(serverBindingHandleP, 
					      serverLocationP, useNoAuth, useLocalAuth)
     rpc_binding_handle_t *	serverBindingHandleP;
     char *			serverLocationP;
     int			useNoAuth;
     int			useLocalAuth;
{
  volatile long			rtnVal = 0;
  volatile int			exceptionRaised = 0;
  unsigned32			authnSvc = rpc_c_authn_dce_private;
  unsigned32			authnLevel = rpc_c_authn_level_default;
  char				serverPrincipalName[BUFSIZ];
  volatile error_status_t	secErrorStatus;
  static char			routineName[] = "dfsauth_client_InitAuthentication";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_33 );
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s: serverLocationP: %s, useNoAuth? %s, useLocalAuth? %s",
	    routineName, (serverLocationP != (char *)NULL) ? serverLocationP : "local",
	    useNoAuth ? "yes" : "no", useLocalAuth ? "yes" : "no"));
  icl_Trace3(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_34 , ICL_TYPE_STRING,
	     ((serverLocationP != (char *)NULL) ? serverLocationP : "local"),
	     ICL_TYPE_STRING, (useNoAuth ? "yes" : "no"),
	     ICL_TYPE_STRING, (useLocalAuth ? "yes" : "no"));
    
#if !defined(FORCE_NOSECURITY)

  /* the init routine will test for initialization, itself */
  rtnVal = dfsauth_client_InitAuthContext(useNoAuth, useLocalAuth);
  
  if (rtnVal == 0) {
    if (serverBindingHandleP != (rpc_binding_handle_t *)NULL) {
      if (useNoAuth == 0) {
	rtnVal = dfsauth_PrinName_GetName(serverLocationP, (char *)NULL,
					  serverPrincipalName,
					  sizeof(serverPrincipalName));
	if (rtnVal == 0) {
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		   ("%s: about to call rpc_binding_set_auth_info, authn svc = %s, principal name: %s",
		    routineName, 
		    ((authnSvc == rpc_c_authn_none) ?
		     "rpc_c_authn_none" : "rpc_c_authn_dce_private"),
		    serverPrincipalName));
	  icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_35 , ICL_TYPE_STRING, ((authnSvc==rpc_c_authn_none)?"rpc_c_authn_none":"rpc_c_authn_dce_private"), ICL_TYPE_STRING, serverPrincipalName);
	  
	  TRY
	    exceptionRaised = 0;
	    rpc_binding_set_auth_info(*serverBindingHandleP,
				      (unsigned char *)serverPrincipalName,
				      authnLevel,
				      authnSvc,
				      (rpc_auth_identity_handle_t)NULL,
				      rpc_c_authz_dce,
				      (error_status_t *)&secErrorStatus);
	  CATCH_ALL
	    PrintException(THIS_CATCH, routineName, "rpc_binding_set_auth_info",
			   __FILE__, __LINE__);
	    exceptionRaised = 1;
	  ENDTRY;

	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		   ("%s: rpc_binding_set_auth_info returned", routineName));
	  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_36 );
	  
	  if ((secErrorStatus != error_status_ok) || exceptionRaised) {
	    if (exceptionRaised) {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		      ("%s: rpc_binding_set_auth_info raised an exception; see stdout",
		       routineName));
	      icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_12 );
	      rtnVal = DAUT_ERROR_RPC_ERROR;
	    }
	    else {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		      ("%s: rpc_binding_set_auth_info error: %s",
		       routineName,
		       RgySec_ErrorString((error_status_t *)&secErrorStatus)));
	      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_13,
		       ICL_TYPE_STRING,
		       RgySec_ErrorString((error_status_t *)&secErrorStatus));
	      if ((secErrorStatus == sec_login_s_no_current_context) ||
		  (secErrorStatus == KRB5_FCC_NOFILE)) {
		rtnVal = DAUT_ERROR_NO_CREDENTIALS_FOUND;
	      }
	      else {
		rtnVal = (long)secErrorStatus;
	      }
	    }
	  }
	}
	else {
#if !defined(KERNEL)
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		   ("%s: PrinName_GetName reported failure: %s", routineName,
		    dfs_dceErrTxt(rtnVal)));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_WARNINGS_37 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
	  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		   ("%s: PrinName_GetName reported failure: %lu", routineName,
		    rtnVal));
	  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_WARNINGS_38 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
	  
	}
      }
    }
  }

#endif /* !defined(FORCE_NOSECURITY) */

#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_39 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_40 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */

  return rtnVal;
}

EXPORT long dfsauth_client_InitNullAuthentication(serverBindingHandleP, 
						  serverLocationP, useNoAuth, useLocalAuth)
     rpc_binding_handle_t *	serverBindingHandleP;
     char *			serverLocationP;
     int			useNoAuth;
     int			useLocalAuth;
{
  volatile long			rtnVal = 0;
  volatile int			exceptionRaised = 0;
  unsigned32			authnSvc = rpc_c_authn_dce_private;
  unsigned32			authnLevel = rpc_c_authn_level_default;
  unsigned_char_t *		serverPrincipalNameP;
  volatile error_status_t	secErrorStatus;
  static char			routineName[] = "dfsauth_client_InitNullAuthentication";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_41 );
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s: serverLocationP: %s, useNoAuth? %s, useLocalAuth? %s",
	    routineName, (serverLocationP != (char *)NULL) ? serverLocationP : "local",
	    useNoAuth ? "yes" : "no", useLocalAuth ? "yes" : "no"));
  icl_Trace3(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_42, ICL_TYPE_STRING, 
	     ((serverLocationP != (char *)NULL) ? serverLocationP : "local"),
	     ICL_TYPE_STRING, (useNoAuth ? "yes" : "no"),
	     ICL_TYPE_STRING, (useLocalAuth ? "yes" : "no"));
    
#if !defined(FORCE_NOSECURITY)

  /* the initialization routine will test for initialization, itself */
  rtnVal = dfsauth_client_InitAuthContext(useNoAuth, useLocalAuth);
  
  if (rtnVal == 0) {
    if (useNoAuth == 0) {
#ifdef notdef
      if (useNoAuth != 0) {
	/*
	  authnSvc = rpc_c_authn_none;
	  */
	authnLevel = rpc_c_authn_level_none;
      }
#endif /* notdef */
      
#ifdef notdef
      rtnVal = dfsauth_PrinName_GetName(serverLocationP, (char *)NULL,
					serverPrincipalName,
					sizeof(serverPrincipalName));
#endif /* notdef */
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
	       ("%s: about to call rpc_mgmt_inq_server_princ_name", routineName));
      icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_43 );
      rpc_mgmt_inq_server_princ_name(*serverBindingHandleP,
				     authnSvc,
				     &serverPrincipalNameP,
				     (error_status_t *)&secErrorStatus);
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
	       ("%s: returned from call to rpc_mgmt_inq_server_princ_name",
		routineName));
      icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_44 );
      if (secErrorStatus) {
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		 ("%s: rpc_mgmt_inq_server_princ_name returned error %s",
		  routineName,
		  RgySec_ErrorString((error_status_t *)&secErrorStatus)));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_45,
		ICL_TYPE_STRING,
		RgySec_ErrorString((error_status_t *)&secErrorStatus));
	rtnVal = (long)secErrorStatus;
      }
      
      if (rtnVal == 0) {
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_DCE_TRACE,
		 ("%s: about to call rpc_binding_set_auth_info, authn svc = %s, principal name: %s",
		  routineName, 
		  ((authnSvc == rpc_c_authn_none) ?
		   "rpc_c_authn_none" : "rpc_c_authn_dce_private"),
		  serverPrincipalNameP));
	icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_46 , ICL_TYPE_STRING, ((authnSvc==rpc_c_authn_none)?"rpc_c_authn_none":"rpc_c_authn_dce_private"), ICL_TYPE_STRING, serverPrincipalNameP);
	
	TRY
	  exceptionRaised = 0;
	  rpc_binding_set_auth_info(*serverBindingHandleP,
				    serverPrincipalNameP,
				    authnLevel,
				    authnSvc,
				    (rpc_auth_identity_handle_t)NULL,
				    rpc_c_authz_dce,
				    (error_status_t *)&secErrorStatus);
	CATCH_ALL
	  PrintException(THIS_CATCH, routineName, "rpc_binding_set_auth_info",
			 __FILE__, __LINE__);
	  exceptionRaised = 1;
	ENDTRY;

	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
		 ("%s: rpc_binding_set_auth_info returned", routineName));
	icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_47 );

	if ((secErrorStatus != error_status_ok) || exceptionRaised) {
	  if (exceptionRaised) {
	    dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		    ("%s: rpc_binding_set_auth_info raised an exception; see stdout",
		     routineName));
	    icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_14 );
	    rtnVal = DAUT_ERROR_RPC_ERROR;
	  }
	  else {
	    dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		    ("%s: rpc_binding_set_auth_info error: %s",
		     routineName,
		     RgySec_ErrorString((error_status_t *)&secErrorStatus)));
	    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_15,
		    ICL_TYPE_STRING,
		    RgySec_ErrorString((error_status_t *)&secErrorStatus));
	    if ((secErrorStatus == sec_login_s_no_current_context) ||
		(secErrorStatus == KRB5_FCC_NOFILE)) {
	      rtnVal = DAUT_ERROR_NO_CREDENTIALS_FOUND;
	    }
	    else {
	      rtnVal = (long)secErrorStatus;
	    }
	  }
	}
	rpc_string_free(&serverPrincipalNameP,
				(error_status_t *)&secErrorStatus);
      }
      else {
#if !defined(KERNEL)
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		 ("%s: PrinName_GetName reported failure: %s", routineName,
		  dfs_dceErrTxt(rtnVal)));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_WARNINGS_48 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
	dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
		 ("%s: PrinName_GetName reported failure: %lu", routineName,
		  rtnVal));
	icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_WARNINGS_49 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
      }
    }
  }
#endif /* !defined(FORCE_NOSECURITY) */

#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_50 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_51 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */

  return rtnVal;
}

EXPORT long dfsauth_client_SetFullEncryption(serverBindingHandleP)
     rpc_binding_handle_t *	serverBindingHandleP;
{
  volatile long			rtnVal = 0;
  long			lockStatus = 0;
  volatile int			exceptionRaised = 0;
  unsigned_char_t *		serverPrinNameP;
  unsigned32			authnSvc;
  rpc_auth_identity_handle_t	authnIdentity;
  unsigned32			authzSvc;
  volatile error_status_t	secErrorStatus;
  static char			routineName[] = "dfsauth_client_SetFullEncryption";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_52 );

  if (serverBindingHandleP != (rpc_binding_handle_t *)NULL) {
#if !defined(FORCE_NOAUTH)
    rtnVal = dfsauth_client_GrabLock();

    if (rtnVal == 0) {
      if (dfsauth_client_initialized == 1) {
	TRY
	  exceptionRaised = 0;
	  rpc_binding_inq_auth_info(*serverBindingHandleP,
				    &serverPrinNameP,
				    &savedAuthnLevel,
				    &authnSvc,
				    &authnIdentity,
				    &authzSvc,
				    (error_status_t *)&secErrorStatus);
	CATCH_ALL
	  PrintException(THIS_CATCH, routineName, "rpc_binding_inq_auth_info",
			 __FILE__, __LINE__);
	  exceptionRaised = 1;
	ENDTRY;

	if ((secErrorStatus == error_status_ok) && ! exceptionRaised) {
	  savedAuthnLevelGood = 1;
	  TRY
	    exceptionRaised = 0;
	    rpc_binding_set_auth_info(*serverBindingHandleP,
				      serverPrinNameP,
				      rpc_c_authn_level_pkt_privacy,
				      authnSvc,
				      authnIdentity,
				      authzSvc,
				      (error_status_t *)&secErrorStatus);
	  CATCH_ALL
	    PrintException(THIS_CATCH, routineName, "rpc_binding_set_auth_info",
			   __FILE__, __LINE__);
	    exceptionRaised = 1;
	  ENDTRY;

	  if ((secErrorStatus != error_status_ok) || exceptionRaised) {
	    if (exceptionRaised) {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		       ("%s: rpc_binding_set_auth_info raised an exception; see stdout",
			routineName));
	      icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_16 );
	    }
	    else {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		      ("%s: rpc_binding_set_auth_info error: %s",
		       routineName,
		       RgySec_ErrorString((error_status_t *)&secErrorStatus)));
	      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_17,
		      ICL_TYPE_STRING,
		      RgySec_ErrorString((error_status_t *)&secErrorStatus));
	    }
	    
	    if (secErrorStatus == rpc_s_unsupported_protect_level)
	    {
		/* try again at packet integrity level */
		TRY
		    exceptionRaised = 0;
		    rpc_binding_set_auth_info(*serverBindingHandleP,
					  serverPrinNameP,
					  rpc_c_protect_level_pkt_integ,
					  authnSvc,
					  authnIdentity,
					  authzSvc,
					  (error_status_t *)&secErrorStatus);
		CATCH_ALL
		    PrintException(THIS_CATCH, routineName, "rpc_binding_set_auth_info",
				   __FILE__, __LINE__);
		    exceptionRaised = 1;
		ENDTRY;

		if ((secErrorStatus != error_status_ok) || exceptionRaised) {
		    /* return RPC error if we can't even get packet integrity */
		    if (exceptionRaised) {
			dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
				("%s: rpc_binding_set_auth_info raised an exception; see stdout",
				 routineName));
			icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_18 );
			rtnVal = DAUT_ERROR_RPC_ERROR;
		    }
		    else {
			dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
				("%s: rpc_binding_set_auth_info error: %s",
				 routineName,
				 RgySec_ErrorString((error_status_t *)&secErrorStatus)));
			icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_19,
			    ICL_TYPE_STRING, RgySec_ErrorString(
			    (error_status_t *)&secErrorStatus));
			rtnVal = (long)secErrorStatus;
		    }
		}
		else 	
		    /* return error code indicating encryption unavailable
		       but integrity is */
		    rtnVal = DAUT_ERROR_PROT_LEVEL_UNSUPPORTED;
	    }
	    else
		rtnVal = (long)secErrorStatus;
	  }
	}
	else {
	  if (exceptionRaised) {
	    dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		    ("%s: rpc_binding_inq_auth_info raised an exception; see stdout",
		     routineName));
	    icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_20 );
	    rtnVal = DAUT_ERROR_RPC_ERROR;
	  }
	  else {
	    if (secErrorStatus == rpc_s_binding_has_no_auth) {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		      ("%s: Error: authentication info required for full encryption: (no noauth)",
		       routineName,
		       RgySec_ErrorString((error_status_t *)&secErrorStatus)));
	      icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_21 );
	      rtnVal = DAUT_ERROR_CLIENT_SERVICE_NOT_SUPP_NOAUTH;
	    }
	    else {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		      ("%s: rpc_binding_inq_auth_info error: %s",
		       routineName,
		       RgySec_ErrorString((error_status_t *)&secErrorStatus)));
	      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_22,
		      ICL_TYPE_STRING,
		      RgySec_ErrorString((error_status_t *)&secErrorStatus));
	      rtnVal = (long)secErrorStatus;
	    }
	  }
	}
      }
      else {
	rtnVal = DAUT_ERROR_CLIENT_NOT_INITIALIZED;
      }

      if (lockStatus = dfsauth_client_ReleaseLock()) {
	osi_printf("%s: unable to release client package mutex\t%s, %d\n",
		   routineName, __FILE__, __LINE__);
	if (rtnVal == 0) {	/* don't mask other errors with this one */
	  rtnVal = lockStatus;
	}
      }
    }
    else {
      osi_printf("%s: unable to grab client package mutex\t%s, %d\n",
		 routineName, __FILE__, __LINE__);
    }
#endif /* !defined(FORCE_NOAUTH) */
  }
  else {
    dfsauth_LogParamError(routineName, "serverBindingHandleP");
    rtnVal = DAUT_ERROR_PARAMETER_ERROR;
  }
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_53 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_54 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */

  return rtnVal;
}

EXPORT long dfsauth_client_ResetDefaultEncryption(serverBindingHandleP)
     rpc_binding_handle_t *	serverBindingHandleP;
{
  volatile long			rtnVal = 0;
  long				lockStatus = 0;
  volatile int			exceptionRaised = 0;
  unsigned_char_t *		serverPrinNameP;
  unsigned32			authnLevel;
  unsigned32			authnSvc;
  rpc_auth_identity_handle_t	authnIdentity;
  unsigned32			authzSvc;
  volatile error_status_t	secErrorStatus;
  static char			routineName[] = "dfsauth_client_ResetDefaultEncryption";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_55 );

  if (serverBindingHandleP != (rpc_binding_handle_t *)NULL) {
#if !defined(FORCE_NOAUTH)
    rtnVal = dfsauth_client_GrabLock();

    if (rtnVal == 0) {
      if (dfsauth_client_initialized == 1) {
	TRY
	  exceptionRaised = 0;
	  rpc_binding_inq_auth_info(*serverBindingHandleP,
				    &serverPrinNameP,
				    &authnLevel,
				    &authnSvc,
				    &authnIdentity,
				    &authzSvc,
				    (error_status_t *)&secErrorStatus);
	CATCH_ALL
	  PrintException(THIS_CATCH, routineName, "rpc_binding_inq_auth_info",
			 __FILE__, __LINE__);
	  exceptionRaised = 1;
	ENDTRY;
	
	if ((secErrorStatus == error_status_ok) && ! exceptionRaised) {
	  TRY
	    exceptionRaised = 0;
	    rpc_binding_set_auth_info(*serverBindingHandleP,
				      serverPrinNameP,
				      savedAuthnLevel,
				      authnSvc,
				      authnIdentity,
				      authzSvc,
				      (error_status_t *)&secErrorStatus);
	  CATCH_ALL
	    PrintException(THIS_CATCH, routineName, "rpc_binding_set_auth_info",
			   __FILE__, __LINE__);
	    exceptionRaised = 1;
	  ENDTRY;
	  
	  if ((secErrorStatus != error_status_ok) || exceptionRaised) {
	    if (exceptionRaised) {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		      ("%s: rpc_binding_set_auth_info raised an exception; see stdout",
		       routineName));
	      icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_23 );
	      rtnVal = DAUT_ERROR_RPC_ERROR;
	    }
	    else {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		      ("%s: rpc_binding_set_auth_info error: %s",
		       routineName,
		       RgySec_ErrorString((error_status_t *)&secErrorStatus)));
	      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_24,
		      ICL_TYPE_STRING,
		      RgySec_ErrorString((error_status_t *)&secErrorStatus));
	      rtnVal = (long)secErrorStatus;
	    }
	  }
	}
	else {
	  if (exceptionRaised) {
	    dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		    ("%s: rpc_binding_inq_auth_info raised an exception; see stdout",
		     routineName));
	    icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_25 );
	    rtnVal = DAUT_ERROR_RPC_ERROR;
	  }
	  else{
	    if (secErrorStatus != rpc_s_binding_has_no_auth) {
	      dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		      ("%s: rpc_binding_inq_auth_info error: %s",
		       routineName,
		       RgySec_ErrorString((error_status_t *)&secErrorStatus)));
	      icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_26,
		      ICL_TYPE_STRING,
		      RgySec_ErrorString((error_status_t *)&secErrorStatus));
	      rtnVal = (long)secErrorStatus;
	    }
	  }
	}
      }
      else {
	rtnVal = DAUT_ERROR_CLIENT_NOT_INITIALIZED;
      }

      if (lockStatus = dfsauth_client_ReleaseLock()) {
	osi_printf("%s: unable to release dfsauth client package lock\t%s, %d\n",
		   routineName, __FILE__, __LINE__);
	if (rtnVal == 0) {	/* don't mask other errors with this one */
	  rtnVal = lockStatus;
	}
      }
    }
    else {
      osi_printf("%s: unable to obtain dfsauth client package lock\t%s, %d\n",
		 routineName, __FILE__, __LINE__);
    }
#endif /* !defined(FORCE_NOAUTH) */
  }
  else {
    dfsauth_LogParamError(routineName, "serverBindingHandleP");
    rtnVal = DAUT_ERROR_PARAMETER_ERROR;
  }
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_56 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_57 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */

  return rtnVal;
}

EXPORT long dfsauth_client_CleanupAuthentication()
{
  volatile long		rtnVal = 0;
  long			lockStatus = 0;
  volatile int		exceptionRaised = 0;
  volatile error_status_t	secErrorStatus;
  static char		routineName[] = "dfsauth_client_CleanupAuthentication";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_58 );

#if !defined(FORCE_NOSECURITY)

  rtnVal = dfsauth_client_GrabLock();

  if (rtnVal == 0) {
    if (dfsauth_client_initialized == 1) {
      if (dfsauth_client_localauthset != 0) {
	TRY
	  exceptionRaised = 0;
	  sec_login_purge_context(&dfsauth_client_loginHandle,
					(error_status_t *)&secErrorStatus);
	CATCH_ALL
	  PrintException(THIS_CATCH, routineName, "sec_login_purge_context",
			 __FILE__, __LINE__);
	  exceptionRaised = 1;
	ENDTRY;
	
	if ((secErrorStatus == error_status_ok) && !exceptionRaised) {
	  dfsauth_client_localauthset = 0;
	}
	else {
	  if (exceptionRaised) {
	    dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		    ("%s: sec_login_purge_context raised an exception; see stdout",
		     routineName));
	    icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_27 );
	    rtnVal = DAUT_ERROR_SEC_RGY_PGO_ERROR;
	  }
	  else {
	    dprintf(DFSAUTH_DEBUG_DCE_TRACE_ERRORS,
		    ("%s: sec_login_purge_context error: %s",
		     routineName,
		     RgySec_ErrorString((error_status_t *)&secErrorStatus)));
	    icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_NONE_28,
		    ICL_TYPE_STRING,
		    RgySec_ErrorString((error_status_t *)&secErrorStatus));
	  }
	}
      }

      dfsauth_client_initialized = 0;
    }
    else {
      rtnVal = DAUT_ERROR_CLIENT_NOT_INITIALIZED;
    }

    if (lockStatus = dfsauth_client_ReleaseLock()) {
      osi_printf("%s: unable to release dfsauth client package lock\t%s, %d\n",
		 routineName, __FILE__, __LINE__);
      if (rtnVal == 0) {	/* don't mask other errors with this one */
	rtnVal = lockStatus;
      }
    }
  }
  else {
    osi_printf("%s: unable to obtain dfsauth client package lock\t%s, %d\n",
	       routineName, __FILE__, __LINE__);
  }

#endif /* !defined(FORCE_NOSECURITY) */
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_59 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_CLIENT_TRACE_60 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */

  return rtnVal;
}
