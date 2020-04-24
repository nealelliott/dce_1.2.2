/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: testacl.c,v $
 * Revision 1.1.659.1  1996/10/02  20:58:22  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:00  damon]
 *
 * $EndLog$
 */
/*
 *	testacl.c -- simple program to use for testing pieces of the ACL package
 *
 * Copyright (C) 1991, 1995 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/osi.h>
#include <com_err.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

#ifdef DACL_COMPILE_FOR_EPI
#include <dcedfs/syscall.h>
#include <dcedfs/aclint.h>
#endif /* DACL_COMPILE_FOR_EPI */

#include <dce/uuid.h>
#include <dce/id_base.h>
#include <dce/aclbase.h>
#include <dce/secidmap.h>

#include <dcedfs/fshs_deleg.h>
#include <dacl.h>
#include <dacl_debug.h>

RCSID("$Header:")

char		programName[BUFSIZ];
char		defaultFilename[BUFSIZ];
dacl_t		theAcl;
int 		is_interactive = 0;	/* safest assumption */
int		cmdSuccess = 0;		/* safest assumption */
int		writeToDisk = 0;	/* safest assumption */

int		timeEpiCalls = 0;	/* whether or not to time Episode calls */
struct timeval	correctedReadTime;
int		readTimeGood = 0;
struct timeval	correctedWriteTime;
int		writeTimeGood = 0;

char * filename = defaultFilename;
epi_uuid_t * mgrUuidP = &bosserverAclMgmtUuid;
char * epi_filenameP = (char *)NULL;
long syscallFlags = 0;
int useLocalUid = 0;

IMPORT long dacl_PermbitString _TAKES((
				       u_int32 *	permBitsP,
				       char *		permStringP
				     ));

/* the pmax compiler has trouble with these, if we put them where they belong */
IMPORT long dacl_From_SecAcl _TAKES((dacl_t * daclP, sec_acl_t * secAclP));
IMPORT long dacl_To_SecAcl _TAKES((dacl_t * daclP, sec_acl_t * secAclP));


#define DEFAULT_EPI_MGR_INDEX		12
#define DEFAULT_BOS_MGR_INDEX		13
#define DEFAULT_FILE_INDEX		14
#define DEFAULT_EPIFILE_INDEX		15
#define DEFAULT_EPI_INIT_FILE_INDEX	16
#define DEFAULT_EPI_INIT_DIR_INDEX	17
#define DEFAULT_LOCAL_UID_INDEX		18
#define DEFAULT_TIME_INDEX		19
#define DEFAULT_DEBUG_INDEX		20

PRIVATE void dacl_SetUpDefaultParams(aSyntaxP)
     struct cmd_syndesc *	aSyntaxP;
{
  cmd_IndexedAddParm(aSyntaxP, "-episode", CMD_FLAG, CMD_OPTIONAL, "",
		     DEFAULT_EPI_MGR_INDEX);
  cmd_IndexedAddParm(aSyntaxP, "-bosserver", CMD_FLAG, CMD_OPTIONAL, "",
		     DEFAULT_BOS_MGR_INDEX);
  cmd_MakeAlternate(aSyntaxP, "-episode", "-bosserver");

  cmd_IndexedAddParm(aSyntaxP, "-file", CMD_LIST, CMD_OPTIONAL, "acl file name",
		     DEFAULT_FILE_INDEX);
  cmd_IndexedAddParm(aSyntaxP, "-epifile", CMD_LIST, CMD_OPTIONAL, "episode file name",
		     DEFAULT_EPIFILE_INDEX);
  cmd_MakeAlternate(aSyntaxP, "-file", "-epifile");

  cmd_IndexedAddParm(aSyntaxP, "-if", CMD_FLAG, CMD_OPTIONAL, "",
		     DEFAULT_EPI_INIT_FILE_INDEX);
  cmd_IndexedAddParm(aSyntaxP, "-id", CMD_FLAG, CMD_OPTIONAL, "",
		     DEFAULT_EPI_INIT_DIR_INDEX);
  cmd_MakeAlternate(aSyntaxP, "-id", "-if");

  cmd_IndexedAddParm(aSyntaxP, "-local", CMD_FLAG, CMD_OPTIONAL, "",
		     DEFAULT_LOCAL_UID_INDEX);

  cmd_IndexedAddParm(aSyntaxP, "-epitime", CMD_FLAG, CMD_OPTIONAL, "",
		     DEFAULT_TIME_INDEX);

  cmd_IndexedAddParm(aSyntaxP, "-debug", CMD_SINGLE, CMD_OPTIONAL, "debug flag value",
		     DEFAULT_DEBUG_INDEX);
}

PRIVATE int dacl_ParseDebugFlag(flagString, flagValP)
     char *	flagString;
     long *	flagValP;
{
  int		rtnVal = 1;	/* assume success */
  char		secondCharacter = *(flagString + 1);
  
  if (*flagString == '0') {
    if ((secondCharacter == 'x') || (secondCharacter == 'X')) {
      /* we have a hex number */
      rtnVal = sscanf(flagString, "%lx", flagValP);
    }
    else {
      /* we have an octal number */
      rtnVal = sscanf(flagString, "%lo", flagValP);
    }
  }
  else {
    /* we have a decimal value */
    *flagValP = atol(flagString);
  }	
  
  return (rtnVal);
}

/*
 * *differenceP = * minuendP - *subtrahendP
 */
#define SEC_IN_USECS	1000000
void timeval_Subtract(minuendP, subtrahendP, differenceP)
     struct timeval *	minuendP;
     struct timeval *	subtrahendP;
     struct timeval *	differenceP;
{
  struct timeval	localMinuend;
  struct timeval	localSubtrahend;
  
  /* make local copies, since we may need to play with the values */
  localMinuend.tv_sec = minuendP->tv_sec;
  localMinuend.tv_usec = minuendP->tv_usec;
  localSubtrahend.tv_sec = subtrahendP->tv_sec;
  localSubtrahend.tv_usec = subtrahendP->tv_usec;

  /* normalize the times, just in case */
  while (localMinuend.tv_usec >= SEC_IN_USECS) {
    localMinuend.tv_usec -= SEC_IN_USECS;
    localMinuend.tv_sec++;
  }

  while (localSubtrahend.tv_usec >= SEC_IN_USECS) {
    localSubtrahend.tv_usec -= SEC_IN_USECS;
    localSubtrahend.tv_sec++;
  }
  
  /* "borrow" for the microseconds, if necessary */
  if (localMinuend.tv_usec < localSubtrahend.tv_usec) {
    localMinuend.tv_usec += SEC_IN_USECS;
    localMinuend.tv_sec--;
  }
  
  differenceP->tv_usec = localMinuend.tv_usec - localSubtrahend.tv_usec;
  differenceP->tv_sec = localMinuend.tv_sec - localSubtrahend.tv_sec;
}
#undef SEC_IN_USECS

long ReadEpiAcl(epi_filenameP)
     char *	epi_filenameP;
{
  long			rtnVal = DACL_SUCCESS;
  char			aclBuffer[8 * 1024];	/* max size of an epi acl */
  int			aclBufferSize = sizeof(aclBuffer);
  static char		routineName[] = "ReadEpiAcl";

  /* all the following variables are only necessary if we are timing the syscalls */
  int			syscallRtn;
  int			beginTimeRtn;
  int			endTimeRtn;
  struct timeval	beginCorrectionTime;
  struct timeval	endCorrectionTime;
  struct timeval	correctionTime;
  struct timeval	beginReadTime;
  struct timeval	endReadTime;
  struct timeval	readTime;
  
#ifdef DACL_COMPILE_FOR_EPI
  beginTimeRtn = osi_GetTime(&beginReadTime);
  syscallRtn = afs_syscall(AFSCALL_VNODE_OPS, VNX_OPCODE_GETACL, (int)epi_filenameP,
			   (int)aclBuffer, (int)&aclBufferSize, syscallFlags);
  endTimeRtn = osi_GetTime(&endReadTime);

  if ((beginTimeRtn == 0) && (endTimeRtn == 0) && (timeEpiCalls)) {
    beginTimeRtn = osi_GetTime(&beginCorrectionTime);
    endTimeRtn = osi_GetTime(&endCorrectionTime);
    if ((beginTimeRtn == 0) && (endTimeRtn == 0)) {
      timeval_Subtract(&endReadTime, &beginReadTime, &readTime);
      timeval_Subtract(&endCorrectionTime, &beginCorrectionTime, &correctionTime);
      timeval_Subtract(&readTime, &correctionTime, &correctedReadTime);
      readTimeGood = 1;
    }
  }
  
  if (syscallRtn == 0) {
    /* parse the byte buffer into the global acl buffer */
    rtnVal = dacl_ParseSyscallAcl(aclBuffer, aclBufferSize, &theAcl, &episodeAclMgmtUuid);
  }
  else {
    /* for some reason, we couldn't read the ACL; let the caller know */
    rtnVal = errno;
  }
#else /* DACL_COMPILE_FOR_EPI */
  rtnVal = EINVAL;
#endif /* DACL_COMPILE_FOR_EPI */

  return rtnVal;
}


long InitAcl(srcFilenameP, epi_filenameP)
     char *	srcFilenameP;
     char *	epi_filenameP;
{
  long			rtnVal = DACL_SUCCESS;
  sec_rgy_name_t	theGlobalName;
  uuid_t		realmUuid;
  error_status_t	parseStatus;
  static char		routineName[] = "InitAcl";
  
  if (epi_filenameP == (char *)NULL) {
    if (srcFilenameP != (char *)NULL) {
      /* read the acl off the disk */
      rtnVal = dacl_ReadFromDisk(&theAcl, srcFilenameP, mgrUuidP);
      if (rtnVal != 0) {
	com_err(programName, rtnVal, "reading acl from disk (%s)", routineName);
      }
    }
    else {
      /* make up the acl from scratch */
      bzero((char *)(&(theAcl)), sizeof(dacl_t));

      (void)strcpy((char *)theGlobalName, "/.:/root");
      sec_id_parse_name(sec_rgy_default_handle,
			theGlobalName,
			NULL,
			&realmUuid,
			NULL,
			(uuid_t *)NULL,
			&parseStatus);
      rtnVal = parseStatus;
      if (rtnVal == 0) {
	theAcl.default_realm = *((epi_uuid_t *) &realmUuid);
	theAcl.mgr_type_tag = *mgrUuidP;
	theAcl.num_entries = 3;
	/*
	  theAcl.dacl_entries = (dacl_entry_t *)osi_Alloc(theAcl.num_entries *
	  sizeof(dacl_entry_t));
	 */
	
	/* initialize the required acl entries */
	theAcl.simple_entries[(int)dacl_simple_entry_type_userobj].is_entry_good = 1;
	dacl_Permset_FromString(
		&(theAcl.simple_entries[(int)dacl_simple_entry_type_userobj].perms),
				"rwxcid");
	
	theAcl.simple_entries[(int)dacl_simple_entry_type_groupobj].is_entry_good = 1;
	dacl_Permset_FromString(
		&(theAcl.simple_entries[(int)dacl_simple_entry_type_groupobj].perms),
				"r");
	
	theAcl.simple_entries[(int)dacl_simple_entry_type_otherobj].is_entry_good = 1;
	dacl_Permset_FromString(
		&(theAcl.simple_entries[(int)dacl_simple_entry_type_otherobj].perms),
				"r");
      }
    }
  }
  else {
    /* in this case, we try to work with real Episode ACLs */
    rtnVal = ReadEpiAcl(epi_filenameP);
  }
  
  return rtnVal;
}


int InitProc(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  /* if this routine returns anything other than 0, nothing further will be done */
  int				rtnVal = 0;
  static char			routineName[] = "InitProc";
  
  if ((strcmp("help", aSyntaxP->name) != 0) && (strcmp("apropos", aSyntaxP->name) != 0)) {
    if (cmd_IsParamPresent(aSyntaxP, DEFAULT_EPI_MGR_INDEX)) {
      mgrUuidP = &episodeAclMgmtUuid;
    }

    if (cmd_IsParamPresent(aSyntaxP, DEFAULT_TIME_INDEX)) {
      timeEpiCalls = 1;
    }
    
    if (cmd_IsParamPresent(aSyntaxP, DEFAULT_FILE_INDEX)) {
      filename = cmd_FirstData(aSyntaxP, DEFAULT_FILE_INDEX);
    }
    if (cmd_IsParamPresent(aSyntaxP, DEFAULT_EPIFILE_INDEX)) {
      epi_filenameP = cmd_FirstData(aSyntaxP, DEFAULT_EPIFILE_INDEX);

#if defined(DACL_COMPILE_FOR_EPI)
      syscallFlags = VNX_ACL_REGULAR_ACL;

      if (cmd_IsParamPresent(aSyntaxP, DEFAULT_EPI_INIT_FILE_INDEX)) {
	syscallFlags = VNX_ACL_INITIAL_ACL;
      }
      else if (cmd_IsParamPresent(aSyntaxP, DEFAULT_EPI_INIT_DIR_INDEX)) {
	syscallFlags = VNX_ACL_DEFAULT_ACL;
      }
#endif /* defined(DACL_COMPILE_FOR_EPI) */
    }

    if (cmd_IsParamPresent(aSyntaxP, DEFAULT_LOCAL_UID_INDEX)) {
      useLocalUid = 1;
    }
    
    if (cmd_IsParamPresent(aSyntaxP, DEFAULT_DEBUG_INDEX)) {
      (void)dacl_ParseDebugFlag(cmd_FirstData(aSyntaxP, DEFAULT_DEBUG_INDEX),
				&dacl_debug_flag);
    }
    
    is_interactive = isatty(0) && isatty(1) && isatty(2);
    
    /* we always want to see and file system error if we printing to a terminal */
    if (is_interactive) {
      dacl_debug_flag |= DACL_DEBUG_MASK(DACL_DEBUG_BIT_FS);
    }
    
    if (strcmp("createondisk", aSyntaxP->name) != 0) {
      if (strcmp("create", aSyntaxP->name) != 0) {
	rtnVal = InitAcl(filename, epi_filenameP);
      }
      else {
	rtnVal = InitAcl((char *)NULL, epi_filenameP);
      }
    }
  }
    
  return rtnVal;
}

int TermProc(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int		ignoredRtnVal = 0;
  long		opRtnVal = DACL_SUCCESS;
  int		entriesToFree;
  char *	aclBufferP = (char *)NULL;
  unsigned int	sizeofAclBuffer = 0;
  struct stat	statBuf;
  u_int32	modeBits;
  char		modeString[10];
  char *	linearAclP = (char *)NULL;
  unsigned int	bufferSize;
  static char	routineName[] = "TermProc";

  /* all the following variables are only necessary if we are timing the syscalls */
  int			syscallRtn;
  int			beginTimeRtn;
  int			endTimeRtn;
  struct timeval	beginCorrectionTime;
  struct timeval	endCorrectionTime;
  struct timeval	correctionTime;
  struct timeval	beginWriteTime;
  struct timeval	endWriteTime;
  struct timeval	writeTime;

  /*
   * this routine is executed as long as the command routine did not exit the pgm;
   * and its return value is completely ignored
   */
  
  if (cmdSuccess != 0) {
    opRtnVal = dacl_PrintAcl(stdout, &theAcl, useLocalUid);
    
    /* now, print mode bits from the ACL */
    /* linearize the ACL, since that's the only interface we have */
    opRtnVal = dacl_FlattenAcl(&theAcl, &linearAclP, &bufferSize);
    if (opRtnVal == DACL_SUCCESS) {
      opRtnVal = dacl_ExtractPermBits(&theAcl, &modeBits);
      if (opRtnVal == DACL_SUCCESS) {
	opRtnVal = dacl_PermbitString(&modeBits, modeString);
	if (opRtnVal == DACL_SUCCESS) {
	  fprintf(stdout, "\nMode Bits from Acl: %s\n", modeString);
	}
      }
      osi_Free((opaque)linearAclP, (long)bufferSize);
    }
    
    if (opRtnVal == DACL_SUCCESS) {
      if (writeToDisk != 0) {
	if (epi_filenameP == (char *)NULL) {
	  opRtnVal = dacl_WriteToDisk(&theAcl, filename);
	}
	else {
#ifdef DACL_COMPILE_FOR_EPI
	  /* get the mode bits */
	  opRtnVal = stat(epi_filenameP, &statBuf);
	  if (opRtnVal == 0) {
	    /* flatten ACL into the buffer */
	    opRtnVal = dacl_epi_FlattenAcl(&theAcl, &aclBufferP, &sizeofAclBuffer,
					   statBuf.st_mode);
	    if (opRtnVal == DACL_SUCCESS) {
	      beginTimeRtn = osi_GetTime(&beginWriteTime);
	      syscallRtn = afs_syscall(AFSCALL_VNODE_OPS, VNX_OPCODE_SETACL, (int)epi_filenameP,
				       (int)aclBufferP, sizeofAclBuffer, syscallFlags);
	      endTimeRtn = osi_GetTime(&endWriteTime);

	      if ((beginTimeRtn == 0) && (endTimeRtn == 0) && (timeEpiCalls)) {
		beginTimeRtn = osi_GetTime(&beginCorrectionTime);
		endTimeRtn = osi_GetTime(&endCorrectionTime);
		if ((beginTimeRtn == 0) && (endTimeRtn == 0)) {
		  timeval_Subtract(&endWriteTime, &beginWriteTime,
				   &writeTime);
		  timeval_Subtract(&endCorrectionTime, &beginCorrectionTime,
				   &correctionTime);
		  timeval_Subtract(&writeTime, &correctionTime,
				   &correctedWriteTime);
		  writeTimeGood = 1;
		}
	      }
  
	      if (syscallRtn != 0) {
		opRtnVal = errno;
	      }
	      osi_Free(aclBufferP, sizeofAclBuffer);
	    }
	  }		/* end if stat succeeded */
	  else {
	    opRtnVal = DACL_ERROR_FS_STAT;
	  }
#else /* DACL_COMPILE_FOR_EPI */
	  opRtnVal = EINVAL;
#endif /* DACL_COMPILE_FOR_EPI */
	}
	if (opRtnVal != 0) {
	  com_err(programName, opRtnVal, "writing acl to disk (%s)", routineName);
	}
      }
    }
    else {
      com_err(programName, opRtnVal, "printing acl to stdout (%s)", routineName);
    }
    dacl_FreeAclEntries(&theAcl);
  }

#if DACL_COMPILE_FOR_EPI
  /* now print both the read and write times, if required */
  if (timeEpiCalls) {
    if (readTimeGood) {
      fprintf(stdout, "\nTime to read the ACL: %ld s, %ld us",
	      correctedReadTime.tv_sec, correctedReadTime.tv_usec);
    }
    if (writeTimeGood) {
      fprintf(stdout, "\nTime to write the ACL: %ld s, %ld us",
	      correctedWriteTime.tv_sec, correctedWriteTime.tv_usec);
    }
    if (readTimeGood || writeTimeGood) {
      fprintf(stdout, "\n");
    }
  }
#endif /* DACL_COMPILE_FOR_EPI */  

  return ignoredRtnVal;
}


#define ADD_TYPE_INDEX	0
#define ADD_NAME_INDEX	1
#define ADD_PERM_INDEX	2
#define ADD_DATA_INDEX	3
int DoAdd(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int		rtnVal = 0;
  char *	nameStringP = (char *)NULL;
  char *	dataStringP = (char *)NULL;
  static char	routineName[] = "DoAdd";
  
  if (cmd_IsParamPresent(aSyntaxP, ADD_NAME_INDEX)) {
    nameStringP = cmd_FirstData(aSyntaxP, ADD_NAME_INDEX);
  }
  if (cmd_IsParamPresent(aSyntaxP, ADD_DATA_INDEX)) {
    dataStringP = cmd_FirstData(aSyntaxP, ADD_DATA_INDEX);
  }
  
  rtnVal = dacl_AddEntryToAcl(&theAcl,
			      cmd_FirstData(aSyntaxP, ADD_TYPE_INDEX), nameStringP,
			      cmd_FirstData(aSyntaxP, ADD_PERM_INDEX),
			      dataStringP, mgrUuidP, useLocalUid);
  if (rtnVal == 0) {
    cmdSuccess = writeToDisk = 1;
  }
  else {
    com_err(programName, rtnVal, "adding entry to acl (%s)", routineName);
  }

  return rtnVal;
}

#define CHMOD_MODE_INDEX	0
int DoChmod(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int		rtnVal = 0;
  u_int32	thePermBits = 0;
  static char	routineName[] = "DoChmod";
  
  sscanf(cmd_FirstData(aSyntaxP, CHMOD_MODE_INDEX), "%o", &thePermBits);
  rtnVal = dacl_ChmodAcl(&theAcl, thePermBits, 0);

  if (rtnVal == 0) {
    cmdSuccess = writeToDisk = 1;
  }
  else {
    com_err(programName, rtnVal, "chmod of acl (%s)", routineName);
  }
  
  return rtnVal;
}


int DoCreate(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int		rtnVal = 0;
  static char	routineName[] = "DoCreate";

  cmdSuccess = writeToDisk = 1;	/* force acl out to disk */
  
  return rtnVal;
}

#define CREATEOD_USEROBJ_INDEX	0
#define CREATEOD_GROUPOBJ_INDEX	1
#define CREATEOD_OTHEROBJ_INDEX	2
int DoCreateOnDisk(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int			rtnVal = 0;

  dacl_permset_t	userObjPerms;
  dacl_permset_t	groupObjPerms;
  dacl_permset_t	otherObjPerms;
  dacl_permset_t *	userObjPermsP = (dacl_permset_t *)NULL;
  dacl_permset_t *	groupObjPermsP = (dacl_permset_t *)NULL;
  dacl_permset_t *	otherObjPermsP = (dacl_permset_t *)NULL;
  sec_rgy_name_t	theGlobalName;
  uuid_t		realmUuid;
  error_status_t	parseStatus;

  char *		createFilenameP;

  static char		routineName[] = "DoCreateOnDisk";

  if (cmd_IsParamPresent(aSyntaxP, CREATEOD_USEROBJ_INDEX)) {
    dacl_Permset_FromString(&userObjPerms,
			    cmd_FirstData(aSyntaxP, CREATEOD_USEROBJ_INDEX));
    userObjPermsP = &userObjPerms;
  }
  if (cmd_IsParamPresent(aSyntaxP, CREATEOD_GROUPOBJ_INDEX)) {
    dacl_Permset_FromString(&groupObjPerms,
			    cmd_FirstData(aSyntaxP, CREATEOD_GROUPOBJ_INDEX));
    groupObjPermsP = &groupObjPerms;
  }
  if (cmd_IsParamPresent(aSyntaxP, CREATEOD_OTHEROBJ_INDEX)) {
    dacl_Permset_FromString(&otherObjPerms,
			    cmd_FirstData(aSyntaxP, CREATEOD_OTHEROBJ_INDEX));
    otherObjPermsP = &otherObjPerms;
  }
  
  if (useLocalUid) {
    bzero((char *)&realmUuid, sizeof(epi_uuid_t));
  }
  else {
    (void)strcpy((char *)theGlobalName, "/.:/root");
    sec_id_parse_name(sec_rgy_default_handle,
		      theGlobalName,
		      NULL,
		      &realmUuid,
		      NULL,
		      (uuid_t *)NULL,
		      &parseStatus);
    rtnVal = parseStatus;
  }
  
  if (rtnVal == 0) {
    theAcl.default_realm = *((epi_uuid_t *) &realmUuid);
  }

  createFilenameP = ((cmd_IsParamPresent(aSyntaxP, DEFAULT_EPIFILE_INDEX)) ?
		     epi_filenameP : filename);
  rtnVal = dacl_CreateAclOnDisk(createFilenameP, mgrUuidP, (epi_uuid_t *)&realmUuid,
				userObjPermsP, groupObjPermsP, otherObjPermsP,
				cmd_IsParamPresent(aSyntaxP, DEFAULT_EPIFILE_INDEX),
				syscallFlags);

  cmdSuccess = writeToDisk = 0;	/* don't force acl out to disk; it's already there */
  
  return rtnVal;
}

spell_it_out(PacList, userObjId, groupObjId, perms)
	pac_list_t *PacList;
	epi_principal_id_t *userObjId, *groupObjId; 
	char  *perms;
{
	sec_id_pa_t *pacp = PacList->pacp;
	unsigned16 num_groups, num_foreign_groups, num_foreign_groupsets;
	int i, j;
	unsigned_char_t *string_uuid;
	error_status_t st;
	pac_list_t *next;

	printf("perms requested:\t%s\n", perms);
	printf("file owner:\t%d\n", *((long *)(userObjId)));
	printf("file group:\t%d\n\n", *((long *)(groupObjId)));

	next = PacList;
	while (pacp) {
		printf("user\t\t\t%d\n", *((long *)&(pacp->principal.uuid)));
		printf("\tgroup\t\t%d\n", *((long *)&(pacp->group.uuid)));
		num_groups = pacp->num_groups;
		i = 0;
		while (num_groups) {
		     printf("\tsupp group\t%d\n", 
					*((long *)&(pacp->groups[i].uuid)));
			i++;
			num_groups--;
		}
	
		i = 0;
		num_foreign_groupsets = pacp->num_foreign_groupsets;
	
		while (num_foreign_groupsets) {
			uuid_to_string(&(pacp->foreign_groupsets[i].realm.uuid), 
					&string_uuid, &st);
			printf("realm: %s\n", string_uuid);
			rpc_string_free(string_uuid);
			num_foreign_groups = pacp->foreign_groupsets[i].num_groups;
			j = 0;
			while (num_foreign_groups) {
				printf("\tsupplementary group\t%d\n", 
	                        *((long *)&(pacp->foreign_groupsets[i].groups[j].uuid)));
	                        j++;
	                        num_foreign_groups--;
			}
			num_foreign_groupsets--;
			i++;
		}
		if (next->next) {
			pacp = next->next->pacp;
			next = next->next;
		} else
			pacp = NULL;
	}

}
	
	
#define CHECK_USER_INDEX	0
#define CHECK_GROUP_INDEX	1
#define CHECK_PERM_INDEX	2
#define CHECK_CONCGROUP_INDEX	3
#define CHECK_USEROBJ_INDEX	4
#define CHECK_GROUPOBJ_INDEX	5
#define CHECK_UNAUTH_INDEX	6
#define CHECK_PRINT_INDEX	7
#define CHECK_DUSER_INDEX	8
#define CHECK_DGROUP_INDEX	9
#define CHECK_DCONCGROUP_INDEX	10
int DoCheckAccess(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int			rtnVal = 0;
  cmd_dataIter_t	groupIter;
  char *		thisGroupP; 
  char *		Perms;
  pac_list_t		PacList, NxtPacList;
  sec_id_pa_t		Pac, NxtPac;
  dacl_permset_t	permsRequested;
  epi_principal_id_t	userObjId;
  epi_principal_id_t	groupObjId;
  epi_principal_id_t *	userObjIdP = (epi_principal_id_t *)NULL;
  epi_principal_id_t *	groupObjIdP = (epi_principal_id_t *)NULL;
  u_int32		modeBits = 0L;
  struct stat		statBuf;
  dacl_permset_t	accessAllowed = 0L;
  int			printAccessSet = cmd_IsParamPresent(aSyntaxP, CHECK_PRINT_INDEX);
  char			accessString[65];	/* 2 longs and a terminator */
  dacl_entry_t		userObjMapEntry;
  dacl_entry_t		groupObjMapEntry;
  char *duser = NULL;
  char *dgroup = NULL;
  char *user = NULL;
  char *group = NULL;
  int unauth = 0, simple_id = 0;
  static char		routineName[] = "DoCheckAccess";

  bzero((char *) &PacList, sizeof(pac_list_t));
  bzero((char *) &NxtPacList, sizeof(pac_list_t));
  bzero((char *) &Pac, sizeof(sec_id_pa_t));
  bzero((char *) &NxtPac, sizeof(sec_id_pa_t));

  PacList.pacp = &Pac;
  user = cmd_FirstData(aSyntaxP, CHECK_USER_INDEX);
  group = cmd_FirstData(aSyntaxP, CHECK_GROUP_INDEX);
  Perms = cmd_FirstData(aSyntaxP, CHECK_PERM_INDEX);
  if (cmd_IsParamPresent(aSyntaxP, CHECK_UNAUTH_INDEX))
	unauth++; 
  if (cmd_IsParamPresent(aSyntaxP, CHECK_DUSER_INDEX) ||
			cmd_IsParamPresent(aSyntaxP, CHECK_DGROUP_INDEX)) {
      PacList.next = &NxtPacList;
      NxtPacList.pacp = &NxtPac;
      duser = cmd_FirstData(aSyntaxP, CHECK_DUSER_INDEX);
      dgroup = cmd_FirstData(aSyntaxP, CHECK_DGROUP_INDEX);
  }

  dacl_InitPacList(&PacList, user, group, duser, dgroup, useLocalUid, unauth);

  if (duser == NULL && dgroup == NULL)
     simple_id = 1;

  /* Initiator */
  for (thisGroupP = cmdIter_FirstData(&groupIter, CHECK_CONCGROUP_INDEX, 
				aSyntaxP);
       				(rtnVal == 0) && (thisGroupP != (char *)NULL);
       				thisGroupP = cmdIter_NextData(&groupIter)) {
       dacl_AddLocalGroupToPac(&Pac, thisGroupP, useLocalUid);
  }

  if (!simple_id) {
  /* Delegate */
    for (thisGroupP = cmdIter_FirstData(&groupIter, CHECK_DCONCGROUP_INDEX, 
				aSyntaxP); (rtnVal == 0) && 
				(thisGroupP != (char *)NULL);
       				thisGroupP = cmdIter_NextData(&groupIter))
       dacl_AddLocalGroupToPac(&NxtPac, thisGroupP, useLocalUid);
  }
  
  if (cmd_IsParamPresent(aSyntaxP, CHECK_UNAUTH_INDEX)) {
    PacList.authenticated = 0;
    NxtPacList.authenticated = 0;
  } else {
    PacList.authenticated = 1;
    NxtPacList.authenticated = 1;
  }
  
  dacl_Permset_FromString(&permsRequested, Perms);
  if (epi_filenameP) {
      printf("using attributes from %s\n", epi_filenameP);
      /* get the user and group obj ids from the file */
      if (stat(epi_filenameP, &statBuf)) {
	printf("stat of Episode file %s returns %d\n", epi_filenameP, errno);
	exit(1);
      }
      bzero((char *)&userObjId, sizeof(epi_principal_id_t));
      bzero((char *)&groupObjId, sizeof(epi_principal_id_t));

      bcopy((char *)&(statBuf.st_uid), (char *)&userObjId, 
						sizeof(statBuf.st_uid));
      bcopy((char *)&(statBuf.st_gid), (char *)&groupObjId, 
						sizeof(statBuf.st_gid));
      modeBits = statBuf.st_mode;
	
      /* note that the following two routines SHOULD produce the same rtnVal */
      spell_it_out(&PacList, &userObjId, &groupObjId, Perms);
      if (printAccessSet) {
	  rtnVal = dacl_epi_CheckAccessAllowedPac(&theAcl, &modeBits, 
		&permsRequested, &PacList, &userObjId, 
		&groupObjId, 0, &accessAllowed);
      } else {
	  rtnVal = dacl_epi_CheckAccessPac(&theAcl, &modeBits, 
		&permsRequested, &PacList,
		&userObjId, &groupObjId, 0);
      }
   } else {
      /* Don't read an actual Episode file, take attributes from command
       * line
       */
      if (cmd_IsParamPresent(aSyntaxP, CHECK_USEROBJ_INDEX)) {
	/* we cheat to avoid doing the user mapping outside the dacl package */
	rtnVal = dacl_InitAclEntryFromStrings(&userObjMapEntry,
					      &(theAcl.default_realm),
					      "user",
					      cmd_FirstData(aSyntaxP, CHECK_USEROBJ_INDEX),
					      "r"	/* irrelevant */,
					      (char *)NULL /* data string */,
					      mgrUuidP,
					      useLocalUid);
	bcopy((char *)(&userObjMapEntry.entry_info.id), (char *)&userObjId,
	      sizeof(epi_principal_id_t));
	userObjIdP = &userObjId;
      }

      if (cmd_IsParamPresent(aSyntaxP, CHECK_GROUPOBJ_INDEX)) {
	/* we cheat to avoid doing the user mapping outside the dacl package */
	rtnVal = dacl_InitAclEntryFromStrings(&groupObjMapEntry,
					      &(theAcl.default_realm),
					      "group",
					      cmd_FirstData(aSyntaxP, CHECK_GROUPOBJ_INDEX),
					      "r"	/* irrelevant */,
					      (char *)NULL /* data string */,
					      mgrUuidP,
					      useLocalUid);
	bcopy((char *)(&groupObjMapEntry.entry_info.id), (char *)&groupObjId,
	      sizeof(epi_principal_id_t));
	groupObjIdP = &groupObjId;
      }
      
      /* note that the following two routines SHOULD produce the same rtnVal */
      if (printAccessSet) {
	rtnVal = dacl_epi_CheckAccessAllowedPac(&theAcl, &modeBits 
			/*(u_int32 *)NULL*/, &permsRequested,
			&PacList,
			userObjIdP,		/* user obj */
			groupObjIdP,		/* group obj */
			0,
			&accessAllowed);
      } else {
	rtnVal = dacl_epi_CheckAccessPac(&theAcl, &modeBits /*(u_int32 *)NULL*/, &permsRequested, &PacList,
				     userObjIdP,	/* user obj */
				     groupObjIdP,     /* group obj */
				     0);
      }
    }
    if (rtnVal == DACL_SUCCESS)
    	printf("Return DACL_SUCCESS\n");
    else if (rtnVal == DACL_ERROR_ACCESS_DENIED)
    	printf("Return DACL_ERROR_ACCESS_DENIED\n");
    else
    	printf("Return %d\n", rtnVal);

    if ((rtnVal == DACL_SUCCESS) || (rtnVal == DACL_ERROR_ACCESS_DENIED)) {
        dacl_Permset_ToString(&accessAllowed, accessString);
        printf("Access allowed set: %s\n", accessString);
    }
    
    cmdSuccess = 1;
    writeToDisk = 0;
  return rtnVal;
}

#define DELETE_TYPE_INDEX	0
#define DELETE_NAME_INDEX	1
int DoDelete(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int		rtnVal = 0;
  char *	nameStringP = (char *)NULL;
  static char	routineName[] = "DoDelete";
  
  if (cmd_IsParamPresent(aSyntaxP, DELETE_NAME_INDEX)) {
    nameStringP = cmd_FirstData(aSyntaxP, DELETE_NAME_INDEX);
  }
  
  rtnVal = dacl_DeleteAclEntry(&theAcl,
			       cmd_FirstData(aSyntaxP, DELETE_TYPE_INDEX), nameStringP,
			       mgrUuidP, useLocalUid);
  if (rtnVal == 0) {
    cmdSuccess = writeToDisk = 1;
  }
  else {
    com_err(programName, rtnVal, "deleting acl entry (%s)", routineName);
  }
  
  return rtnVal;
}

int DoList(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int		rtnVal = 0;
  
  cmdSuccess = 1;
  writeToDisk = 0;

  return rtnVal;
}


#define MODIFY_TYPE_INDEX	0
#define MODIFY_NAME_INDEX	1
#define MODIFY_PERM_INDEX	2
#define MODIFY_DATA_INDEX	3
int DoModify(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int		rtnVal = 0;
  char *	nameStringP = (char *)NULL;
  char *	dataStringP = (char *)NULL;
  static char	routineName[] = "DoModify";
  
  if (cmd_IsParamPresent(aSyntaxP, MODIFY_NAME_INDEX)) {
    nameStringP = cmd_FirstData(aSyntaxP, MODIFY_NAME_INDEX);
  }
  if (cmd_IsParamPresent(aSyntaxP, MODIFY_DATA_INDEX)) {
    dataStringP = cmd_FirstData(aSyntaxP, MODIFY_DATA_INDEX);
  }
  
  rtnVal = dacl_ModifyAclEntry(&theAcl,
			       cmd_FirstData(aSyntaxP, MODIFY_TYPE_INDEX), nameStringP,
			       cmd_FirstData(aSyntaxP, MODIFY_PERM_INDEX),
			       dataStringP, useLocalUid);
  if (rtnVal == 0) {
    cmdSuccess = writeToDisk = 1;
  }
  else {
    com_err(programName, rtnVal, "modifying acl entry (%s)", routineName);
  }
  
  return rtnVal;
}

int DoSecAclXlate(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int		rtnVal = 0;
  sec_acl_t	secAcl;
  dacl_t	secondDacl;
  static char	routineName[] = "DoSecAclXlate";
  
  rtnVal = dacl_To_SecAcl(&theAcl, &secAcl);
  if (rtnVal == DACL_SUCCESS) {
    rtnVal = dacl_From_SecAcl(&secondDacl, &secAcl);
    if (rtnVal == DACL_SUCCESS) {
      fprintf(stdout, "The re-translated dacl:\n");
      rtnVal = dacl_PrintAcl(stdout, &secondDacl, useLocalUid);
      if (rtnVal != DACL_SUCCESS) {
	com_err(programName, rtnVal, " (printing re-translated dacl_t (%s))", routineName);
      }
    }
    else {
      com_err(programName, rtnVal, " (translating to back to dacl_t (%s))", routineName);
    }
  }
  else {
    com_err(programName, rtnVal, " (translating to sec_acl_t (%s))", routineName);
  }
  
  return rtnVal;
}


void SetUpAdd()
{
  struct cmd_syndesc *	addSyntaxP;
  
  addSyntaxP = cmd_CreateSyntax("add", DoAdd,
				(char *)NULL, "add user to acl in file");
  cmd_AddParm(addSyntaxP, "-type", CMD_SINGLE, CMD_REQUIRED, "acl entry type");
  cmd_AddParm(addSyntaxP, "-name", CMD_SINGLE, CMD_OPTIONAL, "principal name");
  cmd_AddParm(addSyntaxP, "-perm", CMD_SINGLE, CMD_REQUIRED, "permission string");
  cmd_AddParm(addSyntaxP, "-data", CMD_SINGLE, CMD_OPTIONAL, "data string");
  dacl_SetUpDefaultParams(addSyntaxP);
}

void SetUpCheckAccess()
{
  struct cmd_syndesc *	checkAccessSyntaxP;
  
  checkAccessSyntaxP = cmd_CreateSyntax("check", DoCheckAccess, (char *)NULL,
					"check requested access against acl in file");
  cmd_AddParm(checkAccessSyntaxP, "-user", CMD_SINGLE, CMD_REQUIRED, "user name");
  cmd_AddParm(checkAccessSyntaxP, "-group", CMD_SINGLE, CMD_REQUIRED, "group name");
  cmd_AddParm(checkAccessSyntaxP, "-perm", CMD_SINGLE, CMD_REQUIRED, "permission string");
  cmd_AddParm(checkAccessSyntaxP, "-concgroup", CMD_LIST, CMD_OPTIONAL, "group name");
  cmd_AddParm(checkAccessSyntaxP, "-userobj", CMD_SINGLE, CMD_OPTIONAL, "user obj name");
  cmd_AddParm(checkAccessSyntaxP, "-groupobj", CMD_SINGLE, CMD_OPTIONAL, "group obj name");
  cmd_AddParm(checkAccessSyntaxP, "-unauth", CMD_FLAG, CMD_OPTIONAL, "");
  cmd_AddParm(checkAccessSyntaxP, "-print", CMD_FLAG, CMD_OPTIONAL, "");
  cmd_AddParm(checkAccessSyntaxP, "-duser", CMD_SINGLE, CMD_OPTIONAL, "delegate user name");
  cmd_AddParm(checkAccessSyntaxP, "-dgroup", CMD_SINGLE, CMD_OPTIONAL, "delegate group name");
  cmd_AddParm(checkAccessSyntaxP, "-dconcgroup", CMD_LIST, CMD_OPTIONAL, "delegate group name");
  dacl_SetUpDefaultParams(checkAccessSyntaxP);
}

void SetUpChmod()
{
  struct cmd_syndesc *	chmodSyntaxP;
  

  chmodSyntaxP = cmd_CreateSyntax("chmod", DoChmod, (char *)NULL,
				  "change ACL access perms according to chmod spec");
  cmd_AddParm(chmodSyntaxP, "-mode", CMD_SINGLE, CMD_REQUIRED, "mode bits");
  dacl_SetUpDefaultParams(chmodSyntaxP);
}

void SetUpCreate()
{
  struct cmd_syndesc *	createSyntaxP;

  createSyntaxP = cmd_CreateSyntax("create", DoCreate,
				   (char *)NULL, "create empty acl in file");
  dacl_SetUpDefaultParams(createSyntaxP);
}

void SetUpCreateOnDisk()
{
  struct cmd_syndesc *	createSyntaxP;

  createSyntaxP = cmd_CreateSyntax("createondisk", DoCreateOnDisk,
				   (char *)NULL, "create empty acl in file");
  cmd_AddParm(createSyntaxP, "-userobjperm", CMD_SINGLE, CMD_OPTIONAL,
	      "permission string");
  cmd_AddParm(createSyntaxP, "-groupobjperm", CMD_SINGLE, CMD_OPTIONAL,
	      "permission string");
  cmd_AddParm(createSyntaxP, "-otherobjperm", CMD_SINGLE, CMD_OPTIONAL,
	      "permission string");
  dacl_SetUpDefaultParams(createSyntaxP);
}

void SetUpDelete()
{
  struct cmd_syndesc *	deleteSyntaxP;
  
  deleteSyntaxP = cmd_CreateSyntax("delete", DoDelete,
				   (char *)NULL, "delete acl entry in file");
  cmd_AddParm(deleteSyntaxP, "-type", CMD_SINGLE, CMD_REQUIRED, "acl entry type");
  cmd_AddParm(deleteSyntaxP, "-name", CMD_SINGLE, CMD_OPTIONAL, "principal name");
  dacl_SetUpDefaultParams(deleteSyntaxP);
}

void SetUpList()
{
  struct cmd_syndesc *	listSyntaxP;
  
  listSyntaxP = cmd_CreateSyntax("list", DoList,
				 (char *)NULL, "list acl in file");
  dacl_SetUpDefaultParams(listSyntaxP);
}

void SetUpModify()
{
  struct cmd_syndesc *	modifySyntaxP;
  
  modifySyntaxP = cmd_CreateSyntax("modify", DoModify,
				   (char *)NULL, "modify acl entry in file");
  cmd_AddParm(modifySyntaxP, "-type", CMD_SINGLE, CMD_REQUIRED, "acl entry type");
  cmd_AddParm(modifySyntaxP, "-name", CMD_SINGLE, CMD_OPTIONAL, "principal name");
  cmd_AddParm(modifySyntaxP, "-perm", CMD_SINGLE, CMD_REQUIRED, "permission string");
  cmd_AddParm(modifySyntaxP, "-data", CMD_SINGLE, CMD_OPTIONAL, "data string");
  dacl_SetUpDefaultParams(modifySyntaxP);
}

void SetUpSecAclXlate()
{
  struct cmd_syndesc *	xlateSyntaxP;

  xlateSyntaxP = cmd_CreateSyntax("secacl", DoSecAclXlate,
				  (char *)NULL, "translate a dacl to sec_acl and back");
  dacl_SetUpDefaultParams(xlateSyntaxP);
}


void SetUpSyntax()
{
  SetUpAdd();
  SetUpCheckAccess();
  SetUpChmod();
  SetUpCreate();
  SetUpCreateOnDisk();
  SetUpDelete();
  SetUpList();
  SetUpModify();
  SetUpSecAclXlate();
}


int main(argc, argv)
     int	argc;
     char *	argv[];
{
  int	rtnVal = 0;

  (void)strncpy(programName, argv[0], sizeof(programName));
  (void)strcpy(defaultFilename, "AclFile");

  (void)cmd_SetBeforeProc(InitProc, (char *)NULL);
  (void)cmd_SetAfterProc(TermProc, (char *)NULL);

   SetUpSyntax();
  
   cmd_Dispatch(argc, argv);
}
