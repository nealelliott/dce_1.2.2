/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth_pathnames.c,v $
 * Revision 1.1.610.1  1996/10/02  20:59:03  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:25  damon]
 *
 * Revision 1.1.605.3  1994/07/13  22:26:35  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:14:01  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:01:45  mbs]
 * 
 * Revision 1.1.605.2  1994/06/09  14:19:35  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:28  annie]
 * 
 * Revision 1.1.605.1  1994/02/04  20:29:43  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:17:54  devsrc]
 * 
 * Revision 1.1.603.1  1993/12/07  17:33:22  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:55:29  jaffe]
 * 
 * Revision 1.1.5.3  1993/01/21  15:19:24  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:57:39  cjd]
 * 
 * Revision 1.1.5.2  1992/11/24  18:32:40  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:24:12  bolinger]
 * 
 * Revision 1.1.3.3  1992/01/24  03:48:31  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:19:15  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:23:38  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:39:30  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:21:45  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
 */
/*
 *	dfsauth_pathnames.c -- routines and declarations for manipulation of
 * various paths of significance to a server using the dfsauth package
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>

#if !defined(KERNEL)
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */
#endif /* !defined(KERNEL) */

#include <dfsauth.h>
#include <dfsauth_debug.h>
#include <dfsauth_trace.h>

#include <dfsauth_pathnames.h>

#include <../exception_print.h>		/* and pthread stuff */

#include <dcedfs/icl.h>

IMPORT struct icl_set *dfsauth_iclSetp;
     
#if !defined(DCELOCAL_PATH)
#define DCELOCAL_PATH	"/opt/dcelocal"
#endif /* !defined(DCELOCAL_PATH) */

#define DCEDIR_ENVVAR_NAME	"DCELOCAL"

#define SERVER_CONFIG_SUFFIX	"/var/dfs"
#define NOAUTH_FILENAME		"NoAuth"
#define DEFAULT_ADMIN_FILENAME	"admin.bos"


#if !defined(AFS_AIX31_ENV) && !defined (AFS_OSF_ENV) && !defined(AFS_HPUX_ENV)
IMPORT char * index _TAKES((
			    char *	stringP,
			    int		theChar
			  ));
#endif /* !AFS_AIX31_ENV && !AFS_OSF_ENV && !AFS_HPUX_ENV */

PRIVATE long dfsauth_pathnames_GrabLock _TAKES((void));
PRIVATE long dfsauth_pathnames_ReleaseLock _TAKES((void));
     
PRIVATE char *			dce_base_dir = (char *)NULL;
PRIVATE char *			server_config_dirname = (char *)NULL;
PRIVATE char *			server_config_noauth_filename = (char *)NULL;
PRIVATE pthread_mutexattr_t	dfsauth_pathnames_mutexattr;
PRIVATE pthread_mutex_t		dfsauth_pathnames_mutex;
PRIVATE pthread_once_t		dfsauth_pathnames_lockInitOnce = pthread_once_init;

/*
 * We have to use exceptions here, since this can only be a void operation, because
 * it will be called through pthread_once().
 */
PRIVATE void dfsauth_pathnames_LockInit()
#if defined(dce_54d)
     pthread_addr_t	ignoredArgP;
#endif /* defined(dce_54d) */
{
  static char	routineName[] = "dfsauth_pathnames_LockInit";
  
  /* apparently, PTHREAD_MUTEXATTR_DEFAULT is undefined, despite the documentation */
  TRY
    pthread_mutexattr_create(&dfsauth_pathnames_mutexattr);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_mutexattr_create",
		   __FILE__, __LINE__);
    RERAISE;
  ENDTRY;

  TRY
    pthread_mutex_init(&dfsauth_pathnames_mutex, dfsauth_pathnames_mutexattr);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_mutex_init", __FILE__, __LINE__);
    RERAISE;
  ENDTRY;

  TRY
    pthread_mutexattr_delete(&dfsauth_pathnames_mutexattr);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_mutexattr_delete",
		   __FILE__, __LINE__);
    RERAISE;
  ENDTRY;
}

/*
 * The following routine tries to grab the server package lock, initializing it if
 * necessary.  We have to use exceptions here or move this routine into another file,
 * due to the pre-processor implementation of the pthreads routines.
 */
PRIVATE long dfsauth_pathnames_GrabLock()
{
  volatile long	rtnVal = 0;
  static char	routineName[] = "dfsauth_pathnames_GrabLock";

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_TRACE_0 );
  
  TRY
    pthread_once(&dfsauth_pathnames_lockInitOnce, dfsauth_pathnames_LockInit);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_once", __FILE__, __LINE__);
    rtnVal = DAUT_ERROR_THREAD_ERROR;
  ENDTRY;

  if (rtnVal == 0) {
    TRY
      pthread_mutex_lock(&dfsauth_pathnames_mutex);
    CATCH_ALL
      PrintException(THIS_CATCH, routineName, "pthread_mutex_unlock", __FILE__, __LINE__);
      rtnVal = DAUT_ERROR_THREAD_ERROR;
    ENDTRY;
  }
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_TRACE_1 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_TRACE_2 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
  
  return rtnVal;
}

PRIVATE long dfsauth_pathnames_ReleaseLock()
{
  volatile long	rtnVal = 0;
  static char	routineName[] = "dfsauth_pathnames_ReleaseLock";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_TRACE_3 );
  
  TRY
    pthread_mutex_unlock(&dfsauth_pathnames_mutex);
  CATCH_ALL
    PrintException(THIS_CATCH, routineName, "pthread_mutex_unlock", __FILE__, __LINE__);
    rtnVal = DAUT_ERROR_THREAD_ERROR;
  ENDTRY;
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_TRACE_4 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_TRACE_5 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */
  
  return rtnVal;
}

/*
 * We have to export this one for use by the server init routine in case it is
 * (legitimately) given a NULL pathname.
 */
EXPORT long 	dfsauth_InitPathnames()
{
  long		rtnVal = 0;
  long		lockStatus = 0;
  char *	dceEnvPath;
  static char	routineName[] = "dfsauth_InitPathnames";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PATHS,
	   ("%s: entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_PATHS_6 );
  
  rtnVal = dfsauth_pathnames_GrabLock();

  if (rtnVal == 0) {
    if (server_config_noauth_filename == (char *)NULL) {
#if !defined(DFSAUTH_LOCAL_DEBUG)
      dce_base_dir = osi_Alloc(strlen(DCELOCAL_PATH) + 1);
      osi_assert(dce_base_dir);
      (void)strcpy(dce_base_dir, DCELOCAL_PATH);
#else /* defined(DFSAUTH_LOCAL_DEBUG) */
      if ((dceEnvPath = getenv(DCEDIR_ENVVAR_NAME)) != (char *)NULL) {
	/* make a copy since we don't know how static the copy returned by getenv is */
	dce_base_dir = osi_Alloc(strlen(dceEnvPath) + 1);
	osi_assert(dce_base_dir);
	(void)strcpy(dce_base_dir, dceEnvPath);
      }
      else {
	dprintf(dfsauth_debug_flag,
		("%s: Error: unable to get base directory environment variable",
		 routineName));
	icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_NONE_0 );
	rtnVal = DAUT_ERROR_ENV_VAR;
      }
#endif /* defined(DFSAUTH_LOCAL_DEBUG) */
      
      server_config_dirname = osi_Alloc(strlen(dce_base_dir) +
					strlen(SERVER_CONFIG_SUFFIX) + 1);
      osi_assert(server_config_dirname);
      (void)sprintf(server_config_dirname, "%s%s", dce_base_dir, SERVER_CONFIG_SUFFIX);
      
      server_config_noauth_filename = osi_Alloc(strlen(server_config_dirname) +
						strlen(NOAUTH_FILENAME) + 2);
      osi_assert(server_config_noauth_filename);
      (void)sprintf(server_config_noauth_filename, "%s/%s", server_config_dirname,
		    NOAUTH_FILENAME);
    }

    if (lockStatus = dfsauth_pathnames_ReleaseLock()) {
      osi_printf("%s: error releasing dfsauth pathnames package lock\t%s, %d\n",
		 routineName, __FILE__, __LINE__);
      if (rtnVal == 0) {
	rtnVal = lockStatus;	/* don't mask any other errors */
      }
    }
  }
  else {
    osi_printf("%s: error obtaining dfsauth pathnames package lock\t%s, %d\n",
	       routineName, __FILE__, __LINE__);
  }
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PATHS,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_PATHS_7 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* defined(KERNEL) */  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PATHS,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_PATHS_8 , ICL_TYPE_LONG, rtnVal);
#endif /* defined(KERNEL) */

  return rtnVal;
}

PRIVATE void dfsauth_LogFsErr(routineNameP, severityStrP, fsOpStrP, theError)
     char *	routineNameP;
     char *	severityStrP;
     char *	fsOpStrP;
     int	theError;
{
  dprintf(dfsauth_debug_flag,
	  ("%s: %s: %s of NoAuth file, %s, returned error: %s",
	   (routineNameP != (char *)NULL) ? routineNameP : "(unknown routine)",
	   (severityStrP != (char *)NULL) ? severityStrP : "(unknown severity)",
	   (fsOpStrP != (char *)NULL) ? fsOpStrP : "(unknown fs op)",
	   server_config_noauth_filename,
	   SysErrString(theError)));
  icl_Trace4(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_NONE_1A, ICL_TYPE_STRING,
	     ((routineNameP != (char*)NULL) ? routineNameP : "(unknown routine)"),
	     ICL_TYPE_STRING,
	     ((severityStrP != (char*)NULL) ? severityStrP : "(unknown severity)"),
	     ICL_TYPE_STRING, ((fsOpStrP != (char*)NULL) ? fsOpStrP : "(unknown fs op)"),
	     ICL_TYPE_STRING, server_config_noauth_filename);
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_NONE_1B, ICL_TYPE_STRING, SysErrString(theError));
}

EXPORT long dfsauth_server_SetNoAuthStatus(newStatus)
     IN int 	newStatus;
{
  long		rtnVal = 0;
  long		lockStatus = 0;
  int		noauthFd;
  int		savedErrno;
  static char	routineName[] = "dfsauth_server_SetNoAuthStatus";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PATHS,
	   ("%s: entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_PATHS_9 );
  
  /* dfsauth_InitPathnames will decide on its own whether to init or not */
  rtnVal = dfsauth_InitPathnames();

  if (rtnVal == 0) {
    rtnVal = dfsauth_pathnames_GrabLock();
    if (rtnVal == 0) {
      if (newStatus != 0) {
	/* create the NoAuth file, ignoring dup error */
	noauthFd = open(server_config_noauth_filename, (O_RDONLY | O_CREAT), 0400);
	savedErrno = errno;
	if (noauthFd >= 0) {
	  if (close(noauthFd) != 0) {
	    savedErrno = errno;
	    
	    /*
	     * just log the error and return, this error does not need to be propagated to
	     * the caller
	     */
	    dfsauth_LogFsErr(routineName, "Warning", "close", savedErrno);
	  }
	}
	else {
	  dfsauth_LogFsErr(routineName, "Error", "open", savedErrno);
	  rtnVal = DAUT_ERROR_FS_OPEN;
	}
      }
      else {
	/* unlink the NoAuth file, ignoring ENOENT */
	if (unlink(server_config_noauth_filename) != 0) {
	  savedErrno = errno;
	  if (savedErrno != ENOENT) {
	    dfsauth_LogFsErr(routineName, "Error", "unlink", savedErrno);
	    rtnVal = DAUT_ERROR_FS_UNLINK;
	  }
	}
      }

      if (lockStatus = dfsauth_pathnames_ReleaseLock()) {
	osi_printf("%s: error releasing dfsauth pathnames package lock\t%s, %d\n",
		   routineName, __FILE__, __LINE__);
	if (rtnVal == 0) {	/* don't mask other errors with this one */
	  rtnVal = lockStatus;
	}
      }
    }
    else {
      osi_printf("%s: error obtaining dfsauth pathnames package lock\t%s, %d\n",
		 routineName, __FILE__, __LINE__);
    }
  }
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PATHS,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_PATHS_10 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* defined(KERNEL) */  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PATHS,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_PATHS_11 , ICL_TYPE_LONG, rtnVal);
#endif /* defined(KERNEL) */

  return rtnVal;
}

EXPORT long dfsauth_server_GetNoAuthStatus(oldStatusP)
     INOUT int *	oldStatusP;
{
  long		rtnVal = 0;
  long		lockStatus = 0;
  int		savedErrno;
  struct stat	statBuffer;
  static char	routineName[] = "dfsauth_server_GetNoAuthStatus";
  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PATHS,
	   ("%s: entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_PATHS_12 );
  
  /* dfsauth_InitPathnames will decide, on its own, whether or not to do the init */
  rtnVal = dfsauth_InitPathnames();

  if (rtnVal == 0) {
    rtnVal = dfsauth_pathnames_GrabLock();

    if (rtnVal == 0) {
      if (oldStatusP != (int *)NULL) {
#if !defined(KERNEL) && !defined(FORCE_NOAUTH)
	/* stat the NoAuth file */
	if (stat(server_config_noauth_filename, &statBuffer) == 0) {
	  *oldStatusP = 1;
	}
	else {
	  savedErrno = errno;
	  *oldStatusP = 0;
	  
	  /*
	   * This is not really only an error condition if the only "problem" is
	   * that the file does not exist.
	   */
	  if (savedErrno != ENOENT) {
	    dfsauth_LogFsErr(routineName, "Error", "stat", savedErrno);
	    rtnVal = DAUT_ERROR_FS_STAT;
	  }
	}
#else /* !defined(KERNEL)  && !defined(FORCE_NOAUTH) */
	*oldStatusP = 1;/* kernel servers always think that noauth is in effect (?) */
	/* to turn on this checking in the kernel would probably involve using namei() */
#endif /* !defined(KERNEL) && !defined(FORCE_NOAUTH) */
      }
      else {
	dfsauth_LogParamError(routineName, "oldStatusP");
	rtnVal = DAUT_ERROR_PARAMETER_ERROR;
      }
    }

    if (lockStatus = dfsauth_pathnames_ReleaseLock()) {
      osi_printf("%s: error releasing dfsauth pathnames package lock\t%s, %d\n",
		 routineName, __FILE__, __LINE__);
      if (rtnVal == 0) {	/* don't mask other errors with this one */
	rtnVal = lockStatus;
      }
    }
  }
  else {
    osi_printf("%s: error obtaining dfsauth pathnames package lock\t%s, %d\n",
	       routineName, __FILE__, __LINE__);
  }
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PATHS,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_PATHS_13 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* !defined(KERNEL) */  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PATHS,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_PATHS_14 , ICL_TYPE_LONG, rtnVal);
#endif /* !defined(KERNEL) */

  return rtnVal;
}

EXPORT long dfsauth_GenerateAdminListPath(filenameP, fullpathnameP, fullpathlength)
     IN    char *	filenameP;
     INOUT char *	fullpathnameP;
     IN	   int		fullpathlength;
{
  long		rtnVal = 0;
  char *	adminFilenameP;
  static char	routineName[] = "dfsauth_GenerateAdminListPath";

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PATHS,
	   ("%s: entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_PATHS_15 );
  
  if (server_config_noauth_filename == (char *)NULL) {
    rtnVal = dfsauth_InitPathnames();
  }

  if (rtnVal == 0) {
    if (fullpathnameP != (char *)NULL) {
      if (filenameP != (char *)NULL) {
	adminFilenameP = filenameP;
      }
      else {
	adminFilenameP = DEFAULT_ADMIN_FILENAME;
      }

      /* if any slashes in filenameP */
      if (index(adminFilenameP, '/') != 0) {
	/* copy filename into full path (checking to be sure there is enough space) */
	if ((unsigned)fullpathlength > strlen(adminFilenameP)) {
	  (void)strcpy(fullpathnameP, adminFilenameP);
	}
	else {
	  rtnVal = DAUT_ERROR_BUFFER_TOO_SMALL;
	}
      }
      else {
	/* prepend the server config dir name to the filename (checking to be sure...) */
	if ((unsigned)fullpathlength > (strlen(server_config_dirname) +
					strlen(adminFilenameP) + 2)) {
	  (void)sprintf(fullpathnameP, "%s/%s", server_config_dirname, adminFilenameP);
	}
	else {
	  rtnVal = DAUT_ERROR_BUFFER_TOO_SMALL;
	}
      }
    }
    else {
      if (fullpathnameP == (char *)NULL) {
	dfsauth_LogParamError(routineName, "fullpathnameP");
      }
      rtnVal = DAUT_ERROR_PARAMETER_ERROR;
    }
  }
  
#if !defined(KERNEL)
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PATHS,
	   ("%s exiting, returning %s", routineName, dfs_dceErrTxt(rtnVal)));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_PATHS_16 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#else /* defined(KERNEL) */  
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PATHS,
	   ("%s exiting, returning %lu", routineName, rtnVal));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_PATHNAMES_PATHS_17 , ICL_TYPE_LONG, rtnVal);
#endif /* defined(KERNEL) */

  return rtnVal;
}
