/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/*
 * HISTORY
 * $Log: dacl_print.c,v $
 * Revision 1.1.848.1  1996/10/02  20:56:06  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:45  damon]
 *
 * $EndLog$
 */

/*
 * dacl_printacl.c -- routines for printing interpreted acl contents (not 
 * just the bytes) to output streams.  useful only for debugging.  
 * dependencies allowed on dce files here, since this is testing only 
 * (and we have no other repository for the uuid to name mapping).
 */
/*
 * Copyright (C) 1995, 1991 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/icl.h>

#ifdef DACL_COMPILE_FOR_EPI
#include <dcedfs/syscall.h>
#include <dcedfs/aclint.h>
#endif /* DACL_COMPILE_FOR_EPI */

#if !defined(KERNEL)
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */
#endif /* !defined(KERNEL) */

#include <dce/uuid.h>
#include <dce/nbase.h>
#include <dce/secidmap.h>

#include <dcedfs/fshs_deleg.h>
#include <dacl.h>
#include <dacl_debug.h>
#include <dacl_trace.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dacl/dacl_print.c,v 1.1.848.1 1996/10/02 20:56:06 damon Exp $")

extern struct icl_set *dacl_iclSetp;
  
#define CREATION_MODE	0644

IMPORT char * RgySec_ErrorString _TAKES((error_status_t * secErrorP));

IMPORT char * dacl_EntryType_ToString _TAKES((dacl_entry_type_t probeType));

PRIVATE int dacl_PrintRealmName(stream, realmUuidP)
     FILE *		stream;
     uuid_t *		realmUuidP;
{
  int			rtnVal = 0;
  uuid_t		safePrincipalUuid;
  sec_rgy_name_t	cellName;
  error_status_t	pgoError = sec_rgy_status_ok;
  static char		routineName[] = "dacl_PrintRealmName";
  
  bzero((char *)&safePrincipalUuid, sizeof(uuid_t));

  /* allow for old-style ACLs, at least for awhile */
  if (bcmp((char *)&safePrincipalUuid, (char *)realmUuidP, sizeof(uuid_t)) != 0) {
    sec_id_gen_name(sec_rgy_default_handle,
		    realmUuidP,
		    &safePrincipalUuid,
		    NULL,
		    cellName,
		    NULL,
		    &pgoError);
    if (pgoError == sec_rgy_status_ok) {
      fprintf(stream, "default realm name: %s", cellName);
    }
    else {
      dprintf(dacl_debug_flag,
	      ("%s: sec_id_gen_name error on name: %s",
	       routineName, RgySec_ErrorString(&pgoError)));
      icl_Trace1(dacl_iclSetp, DACL_ICL_PRINT_NONE_12, ICL_TYPE_LONG, errno);
      rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
    }
  }
  else {
    fprintf(stream, "<(old-style) default realm name>");
  }

  return rtnVal;
}

EXPORT long dacl_PrintAclEntry(stream, aclEntryP, defaultRealmUuidP, local)
     FILE *		stream;
     dacl_entry_t *	aclEntryP;
     epi_uuid_t *	defaultRealmUuidP;
     int		local;
{
  long			rtnVal = DACL_SUCCESS;
  char			permsetString[BUFSIZ];
  char *		typeString;
  char *		dataStringP;
  sec_rgy_name_t	globalName;
  
  static char		routineName[] = "dacl_PrintAclEntry";

  /* map any uuids we have (that we are able to map) */
  if ((aclEntryP->entry_type == dacl_entry_type_user_obj) ||
      (aclEntryP->entry_type == dacl_entry_type_group_obj) ||
      (aclEntryP->entry_type == dacl_entry_type_other_obj) ||
      (aclEntryP->entry_type == dacl_entry_type_mask_obj) ||
      (aclEntryP->entry_type == dacl_entry_type_anyother) ||
      (aclEntryP->entry_type == dacl_entry_type_user_obj_delegate) ||
      (aclEntryP->entry_type == dacl_entry_type_group_obj_delegate) ||
      (aclEntryP->entry_type == dacl_entry_type_other_obj_delegate) ||
      (aclEntryP->entry_type == dacl_entry_type_any_other_delegate) ||
      (aclEntryP->entry_type == dacl_entry_type_unauth)) {
    typeString = dacl_EntryType_ToString(aclEntryP->entry_type);
    fprintf(stream, "%s:\t", (typeString != (char *)NULL) ? typeString : "unknown type");
  }
  else if ((aclEntryP->entry_type == dacl_entry_type_user) ||
	   (aclEntryP->entry_type == dacl_entry_type_group) ||
	   (aclEntryP->entry_type == dacl_entry_type_foreign_user) ||
	   (aclEntryP->entry_type == dacl_entry_type_foreign_group) ||
	   (aclEntryP->entry_type == dacl_entry_type_foreign_other) ||
	   (aclEntryP->entry_type == dacl_entry_type_user_delegate) ||
	   (aclEntryP->entry_type == dacl_entry_type_group_delegate) ||
	   (aclEntryP->entry_type == dacl_entry_type_foreign_user_delegate) ||
	   (aclEntryP->entry_type == dacl_entry_type_foreign_group_delegate) ||
	   (aclEntryP->entry_type == dacl_entry_type_foreign_other_delegate)) {
    rtnVal = dacl_NameAndTypeStringsFromEntry(aclEntryP, defaultRealmUuidP, &typeString,
					      globalName, local);
    if (rtnVal == DACL_SUCCESS) {
      fprintf(stream, "%s:\t", (typeString != (char *)NULL) ? typeString : "unknown type");
      fprintf(stream, "%s:\t", globalName);
    }
  }
  else if (aclEntryP->entry_type == dacl_entry_type_extended) {
    typeString = dacl_EntryType_ToString(aclEntryP->entry_type);
    fprintf(stream, "%s:\t", (typeString != (char *)NULL) ? typeString : "unknown type");

    /* we can get away with this because the data string was init'd from printable chars */
    dataStringP = 
      (char *)osi_Alloc(aclEntryP->entry_info.extended_info.numberBytes + 1);
    bcopy(aclEntryP->entry_info.extended_info.infoP, dataStringP,
	  aclEntryP->entry_info.extended_info.numberBytes);
    dataStringP[aclEntryP->entry_info.extended_info.numberBytes] = '\0';
    fprintf(stream, "\"%s\":\t", dataStringP);
    osi_Free(dataStringP, aclEntryP->entry_info.extended_info.numberBytes + 1);
  }
  
  if (rtnVal == DACL_SUCCESS) {
    /* if everything is still OK so far */
    dacl_Permset_ToString(&(aclEntryP->perms), permsetString);
    fprintf(stream, "%s", permsetString);

  }
  
  fprintf(stream, "\n");
  fflush(stream);

  return rtnVal;
}

EXPORT long dacl_PrintAcl(stream, aclP, local)
     FILE *		stream;
     dacl_t *		aclP;
     int		local;
{
  long			rtnVal = DACL_SUCCESS;
  int			listCounter;
  int			entryCounter;
  dacl_entry_t		tempSimpleEntry;
  int			numberInThisList;
  dacl_entry_t *	thisListP;
  static char		routineName[] = "dacl_PrintAcl";
  
  if (aclP != (dacl_t *)NULL) {
    fprintf(stream, "ACL Manager Name: %s\n", dacl_AclMgrName(&(aclP->mgr_type_tag)));
    
    rtnVal = dacl_PrintRealmName(stream, (uuid_t *)(&(aclP->default_realm)), local);
    fprintf(stream, ":\n");

    /* first, print the simple entries */
    for (entryCounter = 0;
	 ((rtnVal == DACL_SUCCESS) &&
	  (entryCounter <= (int)dacl_simple_entry_type_unauthmask));
	 entryCounter++) {
      if (aclP->simple_entries[entryCounter].is_entry_good == 1) {
	/* map the simple entry type as approp */
	switch ((int)entryCounter) {
	case (int)dacl_simple_entry_type_userobj:
	  tempSimpleEntry.entry_type = dacl_entry_type_user_obj;
	  break;
	case (int)dacl_simple_entry_type_groupobj:
	  tempSimpleEntry.entry_type = dacl_entry_type_group_obj;
	  break;
	case (int)dacl_simple_entry_type_otherobj:
	  tempSimpleEntry.entry_type = dacl_entry_type_other_obj;
	  break;
	case (int)dacl_simple_entry_type_maskobj:
	  tempSimpleEntry.entry_type = dacl_entry_type_mask_obj;
	  break;
	case (int)dacl_simple_entry_type_anyother:
	  tempSimpleEntry.entry_type = dacl_entry_type_anyother;
	  break;
	case (int)dacl_simple_entry_type_unauthmask:
	  tempSimpleEntry.entry_type = dacl_entry_type_unauth;
	  break;
	case (int)dacl_simple_entry_type_userobj_delegate:
	  tempSimpleEntry.entry_type = dacl_entry_type_user_obj_delegate;
	  break;
	case (int)dacl_simple_entry_type_groupobj_delegate:
	  tempSimpleEntry.entry_type = dacl_entry_type_group_obj_delegate;
	  break;
	case (int)dacl_simple_entry_type_otherobj_delegate:
	  tempSimpleEntry.entry_type = dacl_entry_type_other_obj_delegate;
	  break;
	case (int)dacl_simple_entry_type_anyother_delegate:
	  tempSimpleEntry.entry_type = dacl_entry_type_any_other_delegate;
	  break;

	  /* no default since we know all the values of the outer iteration */
	}
	
	/* now, copy the permission sets */
	tempSimpleEntry.perms = aclP->simple_entries[entryCounter].perms;
	rtnVal = dacl_PrintAclEntry(stream, &tempSimpleEntry, &(aclP->default_realm),
				    local);
      }
    }
    
    /* now, print the entries in the lists, in turn */
    for (listCounter = 0;
	 (rtnVal == DACL_SUCCESS) && (listCounter <= (int)dacl_complex_entry_type_other);
	 listCounter++) {
      numberInThisList = aclP->complex_entries[listCounter].num_entries;
      thisListP = aclP->complex_entries[listCounter].complex_entries;
      
      for (entryCounter = 0;
	   (rtnVal == DACL_SUCCESS) && (entryCounter < numberInThisList);
	   entryCounter++) {
	rtnVal = dacl_PrintAclEntry(stream, &(thisListP[entryCounter]),
				    &(aclP->default_realm), local);
      }
    }
    
  }
  else {
    dacl_LogParamError(routineName, "aclP", dacl_debug_flag, __FILE__, __LINE__);
    rtnVal = DACL_ERROR_PARAMETER_ERROR;
  }
  
  return rtnVal;
}

EXPORT long dacl_WriteToDisk(aclP, filenameP)
     dacl_t *	aclP;
     char *	filenameP;
{
  long		rtnVal = DACL_SUCCESS;
  int		aclfd;
  char *	bufferP = (char *)NULL;
  char *	longBufferP = (char *)NULL;
  unsigned int	numBytes;
  unsigned int	longNumBytes;
  static char	routineName[] = "dacl_WriteToDisk";

  rtnVal = dacl_FlattenAcl(aclP, &bufferP, &numBytes);
  if (rtnVal == DACL_SUCCESS) {
    /* this will reparse the buffer, but this is testing-only code */
#if defined(DACL_LOCAL_DEBUG)    
    /*
     * we have to re-flatten here, since the above was flattening for disk,
     * and the validation routine assumes it is getting a syscall buffer
     */
    rtnVal = dacl_epi_FlattenAcl(aclP, &longBufferP, &longNumBytes, (u_int32)0);
    if (rtnVal == DACL_SUCCESS) {
      rtnVal = dacl_ValidateBuffer(longBufferP, &longNumBytes, &(aclP->mgr_type_tag),
				   0 /* => no repairs */, (dacl_t **)NULL);
      osi_Free((opaque)longBufferP, (long)longNumBytes);
    }
#endif /* defined(DACL_LOCAL_DEBUG) */
    
    if (rtnVal == DACL_SUCCESS) {
      /* acls are "immutable"....just write over the entire thing */
      aclfd = open(filenameP, (O_WRONLY | O_CREAT | O_TRUNC), CREATION_MODE);
      if (aclfd >= 0) {
	if (write(aclfd, bufferP, numBytes) != numBytes) {
	  dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_FS,
		   ("%s: %s write: %s", routineName, filenameP, SysErrString(errno)));
	  icl_Trace2(dacl_iclSetp, DACL_ICL_PRINT_FS_0 , ICL_TYPE_STRING, filenameP, ICL_TYPE_STRING, SysErrString(errno));
	  rtnVal = DACL_ERROR_FS_WRITE;
	}
	
	if (close(aclfd) != 0) {
	  dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_FS,
		   ("%s: %s close: %s", routineName, filenameP, SysErrString(errno)));
	  icl_Trace2(dacl_iclSetp, DACL_ICL_PRINT_FS_1 , ICL_TYPE_STRING, filenameP, ICL_TYPE_STRING, SysErrString(errno));
	  rtnVal = DACL_ERROR_FS_CLOSE;
	}
      }
      else {
	dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_FS,
		 ("%s: %s open: %s", routineName, filenameP, SysErrString(errno)));
	icl_Trace2(dacl_iclSetp, DACL_ICL_PRINT_FS_2 , ICL_TYPE_STRING, filenameP, ICL_TYPE_STRING, SysErrString(errno));
	rtnVal = DACL_ERROR_FS_OPEN;
      }
    }
#if defined(DACL_LOCAL_DEBUG) && !defined(KERNEL)
    else {
      fprintf(stderr, "%s: acl failed validation, error: %s\n",
	      routineName, dfs_dceErrTxt(rtnVal));
    }
#endif /* defined(DACL_LOCAL_DEBUG) && !defined(KERNEL) */    
    
    /* if the flatten call, fails, it returns any allocated memory */
    osi_Free((opaque)bufferP, (long)numBytes);
  }
  
  return rtnVal;
}

EXPORT long dacl_CreateAclOnDisk(filenameP, mgrUuidP, defaultRealmUuidP,
				 userObjPermsP, groupObjPermsP, otherObjPermsP,
				 useEpisodeFile, epiSyscallFlags)
     char *		filenameP;
     epi_uuid_t *	mgrUuidP;
     epi_uuid_t *	defaultRealmUuidP;
     dacl_permset_t *	userObjPermsP;
     dacl_permset_t *	groupObjPermsP;
     dacl_permset_t *	otherObjPermsP;
     int		useEpisodeFile;
     long		epiSyscallFlags;
{
  long		rtnVal = 0;
  struct stat	statBuf;
  int		savedErrno;
  int		statRtn;
  dacl_t	theAcl;
  int		i;
#if defined(DACL_COMPILE_FOR_EPI)
  char *	bufferP;
  unsigned int	buffersize;
  int		errcode;
#endif /* defined(DACL_COMPILE_FOR_EPI) */
  static char	routineName[] = "dacl_CreateAclOnDisk";
  
  dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_FS,
	   ("%s: entered, filename is %s",
	    routineName, (filenameP != (char *)NULL) ? filenameP : "NULL"));
  icl_Trace1(dacl_iclSetp, DACL_ICL_PRINT_FS_3,
	     ICL_TYPE_STRING, ((filenameP != (char *) NULL) ? filenameP : "NULL"));

  statRtn = stat(filenameP, &statBuf);
  savedErrno = errno;
  if (((!useEpisodeFile) && (statRtn == -1) && (errno == ENOENT)) ||
      (useEpisodeFile && (statRtn == 0))) {
    dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_FS,
	     ("%s: about to create ACL", routineName));
    icl_Trace0(dacl_iclSetp, DACL_ICL_PRINT_FS_4 );

    theAcl.mgr_type_tag = *mgrUuidP;
    if (defaultRealmUuidP != (epi_uuid_t *)NULL) {
      theAcl.default_realm = *defaultRealmUuidP;
    }
    else {
      bzero((char *)(&(theAcl.default_realm)), sizeof(epi_uuid_t));
    }
    
    /* init the simple entry array */
    for (i = 0; i <= (int)dacl_simple_entry_type_unauthmask; i++) {
      theAcl.simple_entries[i].is_entry_good = 0;
    }

    /* init the complex entry lists */
    for (i = 0; i <= (int)dacl_complex_entry_type_other; i++) {
      theAcl.complex_entries[i].num_entries = 0;
      theAcl.complex_entries[i].entries_allocated = 0;
      theAcl.complex_entries[i].complex_entries = (dacl_entry_t *)NULL;
    }
    
    if (dacl_AreObjectEntriesRequired(mgrUuidP) != 0) {
      /* allocate the required ACL entries */
      
      /* initialize the values in the required entries */
      theAcl.simple_entries[(int)dacl_simple_entry_type_userobj].is_entry_good = 1;
      theAcl.simple_entries[(int)dacl_simple_entry_type_userobj].perms =
	((userObjPermsP != (dacl_permset_t *)NULL) ? *userObjPermsP : 0);

      theAcl.simple_entries[(int)dacl_simple_entry_type_groupobj].is_entry_good = 1;
      theAcl.simple_entries[(int)dacl_simple_entry_type_groupobj].perms =
	((groupObjPermsP != (dacl_permset_t *)NULL) ? *groupObjPermsP : 0);

      theAcl.simple_entries[(int)dacl_simple_entry_type_otherobj].is_entry_good = 1;
      theAcl.simple_entries[(int)dacl_simple_entry_type_otherobj].perms =
	((otherObjPermsP != (dacl_permset_t *)NULL) ? *otherObjPermsP : 0);

      theAcl.num_entries = 3;
    }
    else {
      if ((userObjPermsP != (dacl_permset_t *)NULL) ||
	  (groupObjPermsP != (dacl_permset_t *)NULL)) {
	dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_WARNINGS,
		 ("%s: Warning: user_obj and group_obj perms ignored for ACL mgr type %s",
		  routineName, dacl_AclMgrName(mgrUuidP)));
	icl_Trace1(dacl_iclSetp, DACL_ICL_PRINT_WARNINGS_5,
		   ICL_TYPE_STRING, dacl_AclMgrName(mgrUuidP));
      }
      
      if (otherObjPermsP != (dacl_permset_t *)NULL) {
	theAcl.simple_entries[(int)dacl_simple_entry_type_otherobj].is_entry_good = 1;
	theAcl.simple_entries[(int)dacl_simple_entry_type_otherobj].perms =
	  *otherObjPermsP;

	theAcl.num_entries = 1;
      }
      else {
	theAcl.num_entries = 0;
      }
    }

    if (!useEpisodeFile) {
      rtnVal = dacl_WriteToDisk(&theAcl, filenameP);
    }
    else {
#if defined(DACL_COMPILE_FOR_EPI)
      /* flatten acl for episode */
      rtnVal = dacl_epi_FlattenAcl(&theAcl, &bufferP, &buffersize, statBuf.st_mode);
      if (rtnVal == 0) {
	/* make the syscall to write the ACL */
	errcode = afs_syscall(AFSCALL_VNODE_OPS, VNX_OPCODE_SETACL,
			      (int)filenameP, (int)bufferP, buffersize, epiSyscallFlags);
	if (errcode) {
	  dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_FS,
		   ("%s: %s afs_syscall to write ACL: %s",
		    routineName, filenameP, SysErrString(savedErrno)));
	  icl_Trace2(dacl_iclSetp, DACL_ICL_PRINT_FS_6, ICL_TYPE_STRING, filenameP,
		     ICL_TYPE_STRING, SysErrString(savedErrno));
	  rtnVal = errno;
	}

	(void)osi_Free((opaque)bufferP, buffersize);
      }
      else {
	dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_FS,
		 ("%s: error flattening new ACL for Episode write: %s",
		  routineName, dfs_dceErrTxt(rtnVal)));
	icl_Trace1(dacl_iclSetp, DACL_ICL_PRINT_FS_7, ICL_TYPE_STRING,
		   dfs_dceErrTxt(rtnVal));
      }
#else /* defined(DACL_COMPILE_FOR_EPI) */
      rtnVal = EINVAL;
#endif /* defined(DACL_COMPILE_FOR_EPI) */
    }

    dacl_FreeAclEntries(&theAcl);    /* clean up the allocated ACL */
  }
  else {
    if (!useEpisodeFile) {
      if (statRtn == 0) {
	dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_FS,
		 ("%s: file already exists", routineName));
	icl_Trace0(dacl_iclSetp, DACL_ICL_PRINT_FS_8 );

	rtnVal = DACL_ERROR_ACL_FILE_EXISTS;
      }
      else {
	dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_FS,
		 ("%s: %s stat: %s", routineName, filenameP, SysErrString(savedErrno)));
	icl_Trace2(dacl_iclSetp, DACL_ICL_PRINT_FS_9,
		   ICL_TYPE_STRING, filenameP,
		   ICL_TYPE_STRING, SysErrString(savedErrno));
	rtnVal = DACL_ERROR_FS_STAT;
      }
    }
    else {
      dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_FS,
	       ("%s: %s stat: %s", routineName, filenameP, SysErrString(savedErrno)));
      icl_Trace2(dacl_iclSetp, DACL_ICL_PRINT_FS_10 , 
		 ICL_TYPE_STRING, filenameP,
		 ICL_TYPE_STRING, SysErrString(savedErrno));
      rtnVal = DACL_ERROR_FS_STAT;
    }
  }


  dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_FS,
	   ("%s: exiting, returning %ld", routineName, rtnVal));
  icl_Trace1(dacl_iclSetp, DACL_ICL_PRINT_FS_11 , ICL_TYPE_LONG, rtnVal);
  return rtnVal;
}
