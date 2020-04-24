/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: authtest_procs.c,v $
 * Revision 1.1.12.1  1996/10/02  20:58:41  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:12  damon]
 *
 * Revision 1.1.7.2  1994/07/13  22:26:44  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:14:09  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:01:37  mbs]
 * 
 * Revision 1.1.7.1  1994/06/09  14:19:18  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:14  annie]
 * 
 * Revision 1.1.3.3  1993/01/21  15:18:45  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:56:24  cjd]
 * 
 * Revision 1.1.3.2  1992/11/24  18:31:18  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:23:06  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:51:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  authtest_procs.c -- implementations of RPC routine used by simple rpc server
 *
 *
 *	Copyright (C) 1991 Transarc Corporation
 *	All rights reserved.
 */

#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/debug.h>
#include <dcedfs/osi.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */

#include <dce/rpc.h>
#include <dce/id_base.h>
#include <dce/rgybase.h>
#include <dce/pgo.h>

#include <authtest.h>

#include <dfsauth.h>
#include <dfsauth_debug.h>

extern long auth_debug_flag;
#define AUTH_DEBUG_BIT_RPC_TRACE	1

extern char programName[];
extern char authfilename[];
int userListFd = -1;

#define CREATION_MODE	0644
char defaultFilename[] = "UserList";

IMPORT char *	listFilenameP;

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dfsauth/authtest_procs.c,v 1.1.12.1 1996/10/02 20:58:41 damon Exp $")

IMPORT void Print_DCE_Error _TAKES((
				    FILE *		stream,
				    char *		programName,
				    char *		routineName,
				    error_status_t *	routineErrorStatusP
				  ));


     
#define DCE_ErrorBranch(routineName)					\
else {									\
  Print_DCE_Error(stderr, programName, routineName, &secErrorStatus);	\
  rtnVal = 1;								\
}


int FindObject(objectname, objectdomain, filename, openMode, userUuidP, leaveUserListOpen)
     char *		objectname;
     sec_rgy_domain_t	objectdomain;
     char *		filename;
     int		openMode;
     uuid_t *		userUuidP;
     int		leaveUserListOpen;
{
  int			rtnVal = 1;	/* assume user not in list */
  char			filenameBuffer[BUFSIZ];
  uuid_t		localUserUuid;
  uuid_t *		userUuidBufferP;
  uuid_t		fileEntryUuid;
  int			bytesRead = sizeof(uuid_t);	/* jump start the read loop */
  off_t			position;
  error_status_t	pgoError;
  
  userUuidBufferP = (userUuidP != (uuid_t *)NULL) ? userUuidP : &localUserUuid;

  if (objectname != (char *)NULL) {
    sec_rgy_pgo_name_to_id(sec_rgy_default_handle,
			   objectdomain,
			   objectname,
			   userUuidBufferP,
			   &pgoError);
  }
  else {
    /* we already have the uuid in this case; there is no need to do the mapping */
    pgoError = sec_rgy_status_ok;
  }
  
  if (pgoError == sec_rgy_status_ok) {
    /* if the list file has not yet been opened, do so now */
    if (userListFd < 0) {
      if ((filename != (char *)NULL) && (strlen(filename) > 0)) {
	(void)strncpy(filenameBuffer, filename, sizeof(filenameBuffer) - 1);
      }
      else {
	(void)strncpy(filenameBuffer, defaultFilename, sizeof(filenameBuffer) - 1);
      }
  
      userListFd = open(filenameBuffer, (openMode | O_CREAT), CREATION_MODE);
    }
    else {
      /* if it's already open, reposition the file pointer */
      position = lseek(userListFd, 0, L_SET);
      if (position == -1) {
	perror("lseek");
	if (close(userListFd) != 0) {
	  perror("close");
	}
	userListFd = -1;
	rtnVal = -1;
      }
    }
    
    
    if (userListFd >= 0) {
      while ((rtnVal == 1) && (bytesRead != 0)) {
	bytesRead = read(userListFd, &fileEntryUuid, sizeof(uuid_t));
	if (bytesRead == sizeof(uuid_t)) {
	  if (uuid_equal(userUuidBufferP, &fileEntryUuid, &pgoError) == true) {
	    if (pgoError == sec_rgy_status_ok) {
	      rtnVal = 0;
	    }
	    else {
	      Print_DCE_Error(stderr, programName, "uuid_equal", &pgoError);
	      rtnVal = -2;
	    }
	  }
	}
	else if (bytesRead == -1) {
	  perror("read");
	  if (close(userListFd) != 0) {
	    perror("close");
	  }
	  rtnVal = -1;
	}
      }

      /* close the user list file if we are not requested to leave it open */
      if (leaveUserListOpen == 0) {
	if (close(userListFd) != 0) {
	  perror("close");
	  rtnVal = -1;
	}
      }
    }
    else {
      perror("open or lseek");
      rtnVal = -1;
    }
    
  }
  else {
    Print_DCE_Error(stderr, programName, "sec_rgy_pgo_name_to_id", &pgoError);
    rtnVal = -2;
  }
  

  return rtnVal;
}

int ListPacElts(stream, clientPacP)
     FILE *		stream;
     sec_id_pac_t *	clientPacP;
{
  uuid_t *		clientPrincipalUuidP = &(clientPacP->principal.uuid);
  uuid_t *		clientGroupUuidP = &(clientPacP->group.uuid);
  int			i;
  int			rtnVal = 0;
  sec_rgy_name_t	principalName;
  sec_rgy_name_t	groupName;
  error_status_t	secErrorStatus;
  int			accessProbe;
  int			accessGranted = 0;
  
  fprintf(stream, "pac is%s authenticated\n",
	  (clientPacP->authenticated == true) ? "" : " NOT");
  
  sec_rgy_pgo_id_to_name(sec_rgy_default_handle,
			 sec_rgy_domain_person,
			 clientPrincipalUuidP,
			 principalName,
			 &secErrorStatus);
  if (secErrorStatus == sec_rgy_status_ok) {
    accessProbe = FindObject((char *)NULL, sec_rgy_domain_person, authfilename, O_RDONLY,
			     clientPrincipalUuidP, 1);
    accessGranted = accessGranted || (accessProbe == 0);
    fprintf(stream, "client principal name: %s (access %s)\n",
	    principalName,
	    ((accessProbe == 0) ?
	     "granted" :
	     ((accessProbe == 1) ? "denied" : "error")));
  }
  DCE_ErrorBranch("sec_rgy_pgo_id_to_name");

  sec_rgy_pgo_id_to_name(sec_rgy_default_handle,
			 sec_rgy_domain_group,
			 clientGroupUuidP,
			 groupName,
			 &secErrorStatus);
  if (secErrorStatus == sec_rgy_status_ok) {
    accessProbe = FindObject((char *)NULL, sec_rgy_domain_group, authfilename, O_RDONLY,
			     clientGroupUuidP, 1);
    accessGranted = accessGranted || (accessProbe == 0);
    fprintf(stream, "client group name: %s (access %s)\n",
	    groupName,
	    ((accessProbe == 0) ?
	     "granted" :
	     ((accessProbe == 1) ? "denied" : "error")));
  }
  DCE_ErrorBranch("sec_rgy_pgo_id_to_name");

  fprintf(stream, "local concurrent groups: ");
  for (i = 0; i < clientPacP->num_groups; i++) {
    sec_rgy_pgo_id_to_name(sec_rgy_default_handle,
			   sec_rgy_domain_group,
			   &(clientPacP->groups[i].uuid),
			   groupName,
			   &secErrorStatus);
    if (secErrorStatus == sec_rgy_status_ok) {
      accessProbe = FindObject((char *)NULL, sec_rgy_domain_group, authfilename, O_RDONLY,
			       &(clientPacP->groups[i].uuid), 1);
      accessGranted = accessGranted || (accessProbe == 0);
      fprintf(stream, "%s%s (access %s)",
	      (i != 0) ? ", " : "", groupName,
	      ((accessProbe == 0) ?
	       "granted" :
	       ((accessProbe == 1) ? "denied" : "error")));
    }
    DCE_ErrorBranch("sec_rgy_pgo_id_to_name");
  }

  fprintf(stream, "\nforeign groups: ");
  for (i = 0; i < clientPacP->num_foreign_groups; i++) {
    sec_rgy_pgo_id_to_name(sec_rgy_default_handle,
			   sec_rgy_domain_group,
			   &(clientPacP->foreign_groups[i].id.uuid),
			   groupName,
			   &secErrorStatus);
    if (secErrorStatus == sec_rgy_status_ok) {
      accessProbe = FindObject((char *)NULL, sec_rgy_domain_group, authfilename, O_RDONLY,
			       &(clientPacP->foreign_groups[i].id.uuid), 1);
      accessGranted = accessGranted || (accessProbe == 0);
      fprintf(stream, "%s%s (access %s)",
	      (i != 0) ? ", " : "", groupName,
	      ((accessProbe == 0) ?
	       "granted" :
	       ((accessProbe == 1) ? "denied" : "error")));
    }
    DCE_ErrorBranch("sec_rgy_pgo_id_to_name");
  }
  
  fprintf(stream, "\nAccess %s\n", (accessGranted == 1) ? "granted" : "denied");
  fflush(stream);

  close(userListFd);
  userListFd = -1;

  return(rtnVal);
}

int AuthExtractClientId(authtestBindingP)
     IN rpc_binding_handle_t *	authtestBindingP;
{
  int			rtnVal = 1;
  static char		routineName[] = "AuthExtractClientId";
  char			calleeName[BUFSIZ];
  unsigned_char_p_t	serverPrinName;
  rpc_authz_handle_t	clientId;
  unsigned32		authnLevel;
  unsigned32		authnSvc;
  unsigned32		authzSvc;
  sec_id_pac_t *	clientPacP;
  error_status_t	extractError;

  (void)strcpy(calleeName, "rpc_binding_inq_auth_client");
  rpc_binding_inq_auth_client(*authtestBindingP,
			      &clientId,
			      &serverPrinName,
			      &authnLevel,
			      &authnSvc,
			      &authzSvc,
			      &extractError);
  if (extractError == error_status_ok) {
    if (authzSvc == rpc_c_authz_name) {
      fprintf(stderr, "%s: %s: rpc_c_authz_name\n", routineName, calleeName);
      fprintf(stdout, "%s: client name is %s\n", routineName, (char *)clientId);
      rtnVal = 0;
    }
    else if (authzSvc == rpc_c_authz_none) {
      fprintf(stderr, "%s: %s: rpc_c_authz_none\n", routineName, calleeName);
    }
    else if (authzSvc == rpc_c_authz_dce) {
      fprintf(stderr, "%s: %s: rpc_c_authz_dce\n", routineName, calleeName);
      clientPacP = (sec_id_pac_t *)clientId;
      rtnVal = ListPacElts(stdout, clientPacP);
    }
    else {
      fprintf(stderr, "%s: %s: unrecognized authz svc: %#x\n", routineName, calleeName,
	      authzSvc);
    }
  }
  else {
    fprintf(stderr, "%s: %s error: %s\n", routineName, calleeName,
	    RgySec_ErrorString(&extractError));
  }

  return rtnVal;
}

long AuthCommonInit(routineName, theCommStatusP, globalLockHeldP, authtestBindingP)
     IN  char *			routineName;
     OUT error_status_t *	theCommStatusP;
     OUT int *			globalLockHeldP;
     IN  rpc_binding_handle_t *	authtestBindingP;
{
  long		rtnVal = 0;
  
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_RPC_TRACE, ("%s called.", routineName));
  
/*
  rtnVal = LockBozoMutex(routineName);
*/
  if (rtnVal == 0) {
    *globalLockHeldP = 1;
/*
    if (!afsconf_SuperUser(bozo_confdir, authtestBindingP)) {
      rtnVal = BZACCESS;
    }

    if (AuthExtractClientId(authtestBindingP) != 0) {
      fprintf(stdout, "%s: no identity found for client\n", routineName);
    }
*/

    rtnVal = dfsauth_server_CheckAuthorization(authtestBindingP, (dacl_permset_t *)NULL,
					       listFilenameP, (uuid_t *)NULL);
    dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_WARNINGS,
	     ("%s: access is%s authorized",
	      routineName, (rtnVal == 0) ? "" : " not"));
    
    fflush(stdout);
  }

  *theCommStatusP = error_status_ok;
  
  return rtnVal;
}

long AuthCommonTerm(routineName, procRtnVal, globalLockHeld)
     IN char *	routineName;
     IN long	procRtnVal;
     IN int	globalLockHeld;
{
  long		rtnVal = 0;
  
  dmprintf(auth_debug_flag, AUTH_DEBUG_BIT_RPC_TRACE,
	   ("%s returning: %s.", routineName, dfs_dceErrTxt(procRtnVal)));
  
  if (globalLockHeld != 0) {
/*
    rtnVal = UnlockBozoMutex(routineName);
*/
  }
  
  /* don't lose any info about any failures prior to unlocking the global lock */
  return ((procRtnVal != 0) ? procRtnVal : rtnVal);
}

long AUTH_ServerPlaceHolder(authtestBinding, theCommStatus)
     IN	 rpc_binding_handle_t	authtestBinding;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  int		globalLockHeld = 0;
  static char	routineName[] = "AUTH_ServerPlaceHolder";

  rtnVal = AuthCommonInit(routineName, theCommStatus, &globalLockHeld, &authtestBinding);
  
  if (rtnVal == 0) {
    /* routine does nothing */
  }

  rtnVal = AuthCommonTerm(routineName, rtnVal, globalLockHeld);
  return rtnVal;
}

long AUTH_ServerKill(authtestBinding, theCommStatus)
     IN	 rpc_binding_handle_t	authtestBinding;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  int		globalLockHeld = 0;
  static char	routineName[] = "AUTH_ServerKill";

  rtnVal = AuthCommonInit(routineName, theCommStatus, &globalLockHeld, &authtestBinding);
  
  if (rtnVal == 0) {
    exit(0);
  }

  rtnVal = AuthCommonTerm(routineName, rtnVal, globalLockHeld);
  return rtnVal;
}

long AUTH_ServerSetDebug(authtestBinding, newValue, oldValueP, theCommStatus)
     IN	 rpc_binding_handle_t	authtestBinding;
     IN	 long int		newValue;
     OUT long int *		oldValueP;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  int		globalLockHeld = 0;
  static char	routineName[] = "AUTH_ServerSetDebug";

  rtnVal = AuthCommonInit(routineName, theCommStatus, &globalLockHeld, &authtestBinding);
  
  if (rtnVal == 0) {
    *oldValueP = dfsauth_debug_flag;
    dfsauth_debug_flag = newValue;
  }

  rtnVal = AuthCommonTerm(routineName, rtnVal, globalLockHeld);
  return rtnVal;
}

long AUTH_ServerGetDebug(authtestBinding, currentValueP, theCommStatus)
     IN	 rpc_binding_handle_t	authtestBinding;
     OUT long int *		currentValueP;
     OUT error_status_t *	theCommStatus;
{
  long		rtnVal = 0;
  int		globalLockHeld = 0;
  static char	routineName[] = "AUTH_ServerGetDebug";

  rtnVal = AuthCommonInit(routineName, theCommStatus, &globalLockHeld, &authtestBinding);
  
  if (rtnVal == 0) {
    *currentValueP = dfsauth_debug_flag;
  }

  rtnVal = AuthCommonTerm(routineName, rtnVal, globalLockHeld);
  return rtnVal;
}
