/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dacl_sec_acl.c,v $
 * $EndLog$
 */
/*
 *	dacl_sec_acl.c -- routines that inter-convert between DFS ACLs and DCE ACLs
 *
 * Copyright (C) 1991, 1995 Transarc Corporation
 * All rights reserved.
 */

#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/icl.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/security/dacl/RCS/dacl_sec_acl.c,v 4.35 1996/03/29 16:33:53 cfe Exp $")

#include <dce/rpc.h>
#include <dce/rgybase.h>
#include <dce/pgo.h>
#include <dce/secidmap.h>
#include <dce/aclbase.h>
#include <dce/binding.h>
#include <dce/sec_login.h>

#include <dcedfs/fshs_deleg.h>
#include <dacl.h>
#include <dacl_debug.h>
#include <dacl_trace.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

extern struct icl_set *dacl_iclSetp;
  
IMPORT char * RgySec_ErrorString _TAKES((error_status_t * secErrorP));

/*  prototypes for internal routines, so that we can check those calls */
PRIVATE long dacl_To_SecAcl_ExtendedInfo _TAKES((
     dacl_entry_t *	daclEntryP,
     sec_acl_entry_t *	secAclEntryP
     ));

PRIVATE long dacl_From_SecAcl_ExtendedInfo _TAKES((
     dacl_entry_t *	daclEntryP,
     sec_acl_entry_t *	secAclEntryP
     ));

PRIVATE long dacl_From_SecAcl_ExtendedInfo(daclEntryP, secAclEntryP)
     dacl_entry_t *	daclEntryP;
     sec_acl_entry_t *	secAclEntryP;
{
  long		rtnVal = DACL_SUCCESS;
  void (*free_func) _TAKES((void *ptr));
  void *(*alloc_func) _TAKES((unsigned long size));
  unsigned32 st;
  static char	routineName[] = "dacl_From_SecAcl_ExtendedInfo";

  daclEntryP->entry_type = dacl_entry_type_extended;
  daclEntryP->perms = secAclEntryP->perms;

  *((ndr_format_t *) &daclEntryP->entry_info.extended_info.formatLabel) =
    secAclEntryP->entry_info.tagged_union.extended_info->format_label;
  *((uuid_t *) &daclEntryP->entry_info.extended_info.extensionType) =
    secAclEntryP->entry_info.tagged_union.extended_info->extension_type;
  daclEntryP->entry_info.extended_info.numberBytes =
    secAclEntryP->entry_info.tagged_union.extended_info->num_bytes;
  /* get the current RPC memory alloc/dealloc routines and reenable the
     previous RPC memory alloc/dealloc routines  */
  rpc_sm_swap_client_alloc_free((idl_void_p_t (*)(idl_size_t)) malloc,
				free, &alloc_func, &free_func, &st);
  if (st == error_status_ok) {
      rpc_sm_set_client_alloc_free(alloc_func, free_func, &st);
  }
  if (st != error_status_ok) {
      return DACL_ERROR_BUFFER_ALLOCATION;
  }
  daclEntryP->entry_info.extended_info.infoP =
    (char *)(*alloc_func)(daclEntryP->entry_info.extended_info.numberBytes);
  if (daclEntryP->entry_info.extended_info.infoP != (char *)NULL) {
    bcopy((char *)secAclEntryP->entry_info.tagged_union.extended_info->pickled_data,
	  daclEntryP->entry_info.extended_info.infoP,
	  daclEntryP->entry_info.extended_info.numberBytes);
  }
  else {
    dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_SECACL_TRANSLATE,
	     ("%s: unable to allocate buffer for extended bytes for daclEntryP",
	      routineName));
    icl_Trace0(dacl_iclSetp, DACL_ICL_ACL_TRANSLATE_0 );
    rtnVal = DACL_ERROR_BUFFER_ALLOCATION;
  }
  
  return rtnVal;
}

/*
 * this routine is only exported for debugging, and should only be used by
 * the testing pgm
 */
/* The above no longer applies. It is used by dfs_dce_acl_CheckAccessAllowedPac
 * as well as sec_acl_FlattenAcl, DoSecAclXlate
 */
EXPORT long dacl_From_SecAcl(daclP, secAclP)
     dacl_t *		daclP;
     sec_acl_t *	secAclP;
{
  long			rtnVal = DACL_SUCCESS;
  int			i;
  long			totalNumEntries;	/* save a few pointer dereferences */
  int *			extendedEntryIndicesP = NULL;
  int			numbExtendedEntries = 0;
  int			simpleEntryIndex = -1;
  u_int32 *		numEntriesP = (u_int32 *)NULL;
  dacl_entry_t *	aclEntryP = (dacl_entry_t *)NULL;
  boolean32		mgrUuidCmp = true;
  error_status_t	uuidCmpStatus = error_status_ok;
  static char		routineName[] = "dacl_From_SecAcl";
  
  if ((daclP != (dacl_t *)NULL) && (secAclP != (sec_acl_t *)NULL)) {
/*
    mgrUuidCmp = uuid_equal(&(secAclP->sec_acl_manager_type),
			    (uuid_t *)&(episodeAclMgmtUuid),
			    &uuidCmpStatus);
*/
    if ((uuidCmpStatus == error_status_ok) && (mgrUuidCmp == true)) {
      bzero((char *)daclP, sizeof(dacl_t));
      
      totalNumEntries = secAclP->num_entries;
      extendedEntryIndicesP = (int *)dacl_Alloc(totalNumEntries * sizeof(int));
      if (extendedEntryIndicesP != (int *)NULL) {
	/* copy the acl header */
	*((uuid_t *) &daclP->default_realm) = secAclP->default_realm.uuid;
	*((uuid_t *) &daclP->mgr_type_tag) = secAclP->sec_acl_manager_type;
	daclP->num_entries = secAclP->num_entries;
	
	/* allocate as much space as we may need for each of the lists in the dacl */
	for (i = 0;
	     (i <= (int)dacl_complex_entry_type_other) && (rtnVal == DACL_SUCCESS);
	     i++) {
	  /* a bit conservative with the space, but we shouldn't run off the ends */
	  daclP->complex_entries[i].complex_entries =
	    (dacl_entry_t *)dacl_Alloc(totalNumEntries * sizeof(dacl_entry_t));
	  daclP->complex_entries[i].entries_allocated = totalNumEntries;
	  if (daclP->complex_entries[i].complex_entries != (dacl_entry_t *)NULL) {
	    bzero((char *)daclP->complex_entries[i].complex_entries,
		  totalNumEntries * sizeof(dacl_entry_t));
	  }
	  else {
	    dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_SECACL_TRANSLATE,
		     ("%s: unable to allocate buffer for complex entry array",
		      routineName));
	    icl_Trace0(dacl_iclSetp, DACL_ICL_ACL_TRANSLATE_1 );
	    rtnVal = DACL_ERROR_BUFFER_ALLOCATION;
	  }
	}
	
	if (rtnVal == DACL_SUCCESS) {
	  /* now, get each of the entries, in turn */
	  for (i = 0; (i < daclP->num_entries) && (rtnVal == DACL_SUCCESS); i++) {
	    simpleEntryIndex = -1;
	    aclEntryP = (dacl_entry_t *)NULL;
	    
	    switch ((int)(secAclP->sec_acl_entries[i].entry_info.entry_type)) {
	    case sec_acl_e_type_user_obj :
	      if (simpleEntryIndex == -1) {
		simpleEntryIndex = (int)dacl_simple_entry_type_userobj;
	      }
	      /* fall through to next case is intentional */
	    case sec_acl_e_type_group_obj :
	      if (simpleEntryIndex == -1) {
		simpleEntryIndex = (int)dacl_simple_entry_type_groupobj;
	      }
	      /* fall through to next case is intentional */
	    case sec_acl_e_type_other_obj :
	      if (simpleEntryIndex == -1) {
		simpleEntryIndex = (int)dacl_simple_entry_type_otherobj;
	      }
	      /* fall through to next case is intentional */
	    case sec_acl_e_type_any_other :
	      if (simpleEntryIndex == -1) {
		simpleEntryIndex = (int)dacl_simple_entry_type_anyother;
	      }
	      /* fall through to next case is intentional */
	    case sec_acl_e_type_mask_obj :
	      if (simpleEntryIndex == -1) {
		simpleEntryIndex = (int)dacl_simple_entry_type_maskobj;
	      }
            case sec_acl_e_type_user_obj_deleg:
	      /* fall through to next case is intentional */
	      if (simpleEntryIndex == -1) {
		simpleEntryIndex = (int)dacl_simple_entry_type_userobj_delegate;
	      }
            case sec_acl_e_type_group_obj_deleg:
	      if (simpleEntryIndex == -1) {
		simpleEntryIndex = (int)dacl_simple_entry_type_groupobj_delegate;
	      }
            case sec_acl_e_type_other_obj_deleg:
	      if (simpleEntryIndex == -1) {
		simpleEntryIndex = (int)dacl_simple_entry_type_otherobj_delegate;
	      }
            case sec_acl_e_type_any_other_deleg:
	      if (simpleEntryIndex == -1) {
		simpleEntryIndex = (int)dacl_simple_entry_type_anyother_delegate;
	      }
	    case sec_acl_e_type_unauthenticated :
	      if (simpleEntryIndex == -1) {
		simpleEntryIndex = (int)dacl_simple_entry_type_unauthmask;
	      }
	      
	      /* here's what all the above cases want to do */
	      if (simpleEntryIndex != -1) {
		if (daclP->simple_entries[simpleEntryIndex].is_entry_good == 0) {
		  daclP->simple_entries[simpleEntryIndex].is_entry_good = 1;
		  daclP->simple_entries[simpleEntryIndex].perms =
		    secAclP->sec_acl_entries[i].perms;
		}
		else {
		  dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_SECACL_TRANSLATE,
			   ("%s: duplicate simple entry, type %d, found in secAclP",
			    routineName, simpleEntryIndex));
		  icl_Trace1(dacl_iclSetp, DACL_ICL_ACL_TRANSLATE_2 , ICL_TYPE_LONG, simpleEntryIndex);
		  rtnVal = DACL_ERROR_DUPLICATE_ENTRY_FOUND;
		}
	      }
	      else {
		/* shouldn't get to here in this case */
		dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_SECACL_TRANSLATE,
			 ("%s: something strange happened in identifying the entry type",
			  routineName));
		icl_Trace0(dacl_iclSetp, DACL_ICL_ACL_TRANSLATE_3 );
	      }
	      
	      break;
	      
	    case sec_acl_e_type_user :
	      if (aclEntryP == (dacl_entry_t *)NULL) {
		numEntriesP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_user].num_entries);
		aclEntryP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_user].
		    complex_entries[*numEntriesP]);
		
		aclEntryP->entry_type = dacl_entry_type_user;
	      }
	      /* fall through to next case is intentional */
	    case sec_acl_e_type_group :
	      if (aclEntryP == (dacl_entry_t *)NULL) {
		numEntriesP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_group].
		    num_entries);
		aclEntryP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_group].
		    complex_entries[*numEntriesP]);
		
		aclEntryP->entry_type = dacl_entry_type_group;
	      }
	      /* fall through to next case is intentional */
	    case sec_acl_e_type_foreign_other :
	      if (aclEntryP == (dacl_entry_t *)NULL) {
		numEntriesP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_other].
		    num_entries);
		aclEntryP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_other].
		    complex_entries[*numEntriesP]);
		
		aclEntryP->entry_type = dacl_entry_type_foreign_other;
	      }
	    case sec_acl_e_type_user_deleg:
	      if (aclEntryP == (dacl_entry_t *)NULL) {
		numEntriesP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_user_delegate].
		    num_entries);
		aclEntryP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_user_delegate].
		    complex_entries[*numEntriesP]);
		
		aclEntryP->entry_type = dacl_entry_type_user_delegate;
	      }
            case sec_acl_e_type_group_deleg:
	      if (aclEntryP == (dacl_entry_t *)NULL) {
		numEntriesP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_group_delegate].
		    num_entries);
		aclEntryP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_group_delegate].
		    complex_entries[*numEntriesP]);
		
		aclEntryP->entry_type = dacl_entry_type_group_delegate;
	      }
            case sec_acl_e_type_for_other_deleg:
	      if (aclEntryP == (dacl_entry_t *)NULL) {
		numEntriesP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_for_other_delegate].
		    num_entries);
		aclEntryP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_for_other_delegate].
		    complex_entries[*numEntriesP]);
		
		aclEntryP->entry_type = dacl_entry_type_foreign_other_delegate;
	      }

	      /* here is the code that the six above cases want to execute */
	      aclEntryP->perms = secAclP->sec_acl_entries[i].perms;
	      *((uuid_t *) &aclEntryP->entry_info.id) =
		secAclP->sec_acl_entries[i].entry_info.tagged_union.id.uuid;
	      (*numEntriesP)++;
	      break;
	    case sec_acl_e_type_foreign_user :
	      if (aclEntryP == (dacl_entry_t *)NULL) {
		numEntriesP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_user].num_entries);
		aclEntryP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_user].
		    complex_entries[*numEntriesP]);
		
		aclEntryP->entry_type = dacl_entry_type_foreign_user;
	      }
	      
	      /* fall through to next case is intentional */
	    case sec_acl_e_type_foreign_group :
	      if (aclEntryP == (dacl_entry_t *)NULL) {
		numEntriesP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_group].
		    num_entries);
		aclEntryP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_group].
		    complex_entries[*numEntriesP]);
		
		aclEntryP->entry_type = dacl_entry_type_foreign_group;
	      }
              /* intentional fall through */
            case sec_acl_e_type_for_user_deleg:
	      if (aclEntryP == (dacl_entry_t *)NULL) {
		numEntriesP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_user_delegate].
		    num_entries);
		aclEntryP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_user_delegate].
		    complex_entries[*numEntriesP]);
		
		aclEntryP->entry_type = dacl_entry_type_foreign_user_delegate;
	      }

            /* intentional fall through */
            case sec_acl_e_type_for_group_deleg:	      
	      if (aclEntryP == (dacl_entry_t *)NULL) {
		numEntriesP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_group_delegate].
		    num_entries);
		aclEntryP =
		  &(daclP->complex_entries[(int)dacl_complex_entry_type_group_delegate].
		    complex_entries[*numEntriesP]);
		
		aclEntryP->entry_type = dacl_entry_type_foreign_group_delegate;
	      }

	      /* here is the code that the four above cases want to execute */
	      aclEntryP->perms = secAclP->sec_acl_entries[i].perms;
	      *((uuid_t *) &aclEntryP->entry_info.foreign_id.id) =
		secAclP->sec_acl_entries[i].entry_info.tagged_union.foreign_id.id.uuid;
	      *((uuid_t *) &aclEntryP->entry_info.foreign_id.realm) =
		secAclP->sec_acl_entries[i].entry_info.tagged_union.foreign_id.realm.uuid;
	      (*numEntriesP)++;
	      break;

	    case sec_acl_e_type_extended :
	      extendedEntryIndicesP[numbExtendedEntries++] = i;
	      break;
	      default :
		dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_SECACL_TRANSLATE,
			 ("%s: unrecognized entry type, %d, found in secAclP",
			  routineName,
			  (int)(secAclP->sec_acl_entries[i].entry_info.entry_type)));
		icl_Trace1(dacl_iclSetp, DACL_ICL_ACL_TRANSLATE_4 , ICL_TYPE_LONG, (int)(secAclP->sec_acl_entries[i].entry_info.entry_type));
	      rtnVal = DACL_ERROR_UNRECOGNIZED_ENTRY_TYPE;
	      break;
	    } 	/* end switch */
	  }	/* end for loop */
	  
	  if (rtnVal == DACL_SUCCESS) {
	    /* we process the extended entries afterwards to get the order right */
	    for (i = 0; i < numbExtendedEntries; i++) {
	      numEntriesP =
		&(daclP->complex_entries[(int)dacl_complex_entry_type_other].num_entries);
	      aclEntryP =
		&(daclP->complex_entries[(int)dacl_complex_entry_type_other].
		  complex_entries[*numEntriesP]);
	      
	      aclEntryP->entry_type = dacl_entry_type_extended;
	      
	      rtnVal = dacl_From_SecAcl_ExtendedInfo(aclEntryP,
			     &(secAclP->sec_acl_entries[extendedEntryIndicesP[i]]));
	      
	      if (rtnVal == DACL_SUCCESS) {
		(*numEntriesP)++;
	      }
	    }	/* end for loop processing extended entries */
	  }	/* end if the first for loop ended all right */
	}		/* end if the init went all right */
	if ( extendedEntryIndicesP != NULL )
	    dacl_Free(extendedEntryIndicesP, totalNumEntries * sizeof(int));
      }		/* end if we could alloc the extended entry array */
      else {
	dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_SECACL_TRANSLATE,
		 ("%s: error allocating extended entry array buffer", routineName));
	icl_Trace0(dacl_iclSetp, DACL_ICL_ACL_TRANSLATE_5 );
	rtnVal = DACL_ERROR_BUFFER_ALLOCATION;
      }
    }
    else {
      dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_SECACL_TRANSLATE,
	       ("%s: error incorrect ACL manager uuid in secAclP", routineName));
      icl_Trace0(dacl_iclSetp, DACL_ICL_ACL_TRANSLATE_6 );
      rtnVal = DACL_ERROR_INCORRECT_MGR_UUID;
    }
  }
  else {
    if (daclP == (dacl_t *)NULL) {
      dacl_LogParamError(routineName, "daclP", dacl_debug_flag, __FILE__, __LINE__);
      rtnVal = DACL_ERROR_PARAMETER_ERROR;
    }
    if (secAclP == (sec_acl_t *)NULL) {
      dacl_LogParamError(routineName, "secAclP", dacl_debug_flag, __FILE__, __LINE__);
      rtnVal = DACL_ERROR_PARAMETER_ERROR;
    }
  }
  
  return rtnVal;
}

PRIVATE long dacl_To_SecAcl_ExtendedInfo(daclEntryP, secAclEntryP)
     dacl_entry_t *	daclEntryP;
     sec_acl_entry_t *	secAclEntryP;
{
  long		rtnVal = DACL_SUCCESS;
  void (*free_func) _TAKES((void *ptr));
  void *(*alloc_func) _TAKES((unsigned long size));
  unsigned32 st;
  static char	routineName[] = "dacl_To_SecAcl_ExtendedInfo";

  secAclEntryP->entry_info.entry_type = sec_acl_e_type_extended;
  secAclEntryP->perms = daclEntryP->perms;

  /* get the current RPC memory alloc/dealloc routines and reenable the
     previous RPC memory alloc/dealloc routines  */
  rpc_sm_swap_client_alloc_free((idl_void_p_t (*)(idl_size_t)) malloc,
                                free, &alloc_func, &free_func, &st);
  if (st == error_status_ok) {
    rpc_sm_set_client_alloc_free(alloc_func, free_func, &st);
  }
  if (st != error_status_ok) {
    return DACL_ERROR_BUFFER_ALLOCATION;
  }
  /* allocate the extended info structure, with the array of bytes inline */
  secAclEntryP->entry_info.tagged_union.extended_info =
    (sec_acl_extend_info_t *)(*alloc_func)(sizeof(sec_acl_extend_info_t) +
					   daclEntryP->entry_info.extended_info.numberBytes);

  if (secAclEntryP->entry_info.tagged_union.extended_info != (sec_acl_extend_info_t *)NULL) {
    secAclEntryP->entry_info.tagged_union.extended_info->format_label =
      *((ndr_format_t *) &daclEntryP->entry_info.extended_info.formatLabel);
    secAclEntryP->entry_info.tagged_union.extended_info->extension_type =
      *((uuid_t *) &daclEntryP->entry_info.extended_info.extensionType);
    secAclEntryP->entry_info.tagged_union.extended_info->num_bytes =
      daclEntryP->entry_info.extended_info.numberBytes;
    bcopy(daclEntryP->entry_info.extended_info.infoP,
	  (char *)secAclEntryP->entry_info.tagged_union.extended_info->pickled_data,
	  secAclEntryP->entry_info.tagged_union.extended_info->num_bytes);
  }
  else {
    dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_SECACL_TRANSLATE,
	     ("%s: error allocating buttfer for extended info for secAclEntryP",
	      routineName));
    icl_Trace0(dacl_iclSetp, DACL_ICL_ACL_TRANSLATE_7 );
    rtnVal = DACL_ERROR_BUFFER_ALLOCATION;
  }

  return rtnVal;
}

#define DACL_PRINT_AND_SET_ERROR	\
  dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_SECACL_TRANSLATE,		\
	   ("%s: %s returned error: %s",				\
	    routineName, secRoutineName, RgySec_ErrorString(&pgoError))); \
  rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR


PRIVATE long dacl_FindFullUuid(entryP, defaultCellUuidP)
     sec_acl_entry_t *		entryP;
     uuid_t *			defaultCellUuidP;
{
  long				rtnVal = DACL_SUCCESS;
  sec_rgy_name_t		cellName;
  sec_rgy_name_t		ignoredGlobalName;
  uuid_t			fullUuid;
  long				unixId;
  sec_rgy_handle_t		secRgyHandle;
  sec_rgy_domain_t		nameDomain;
  uuid_t *			principalUuidP;
  error_status_t		pgoError;
  char				secRoutineName[BUFSIZ];
  static char			routineName[] = "dacl_FindFullUuid";
  sec_rgy_bind_auth_info_t      bindingAuthInfo;
  sec_login_handle_t            loginContext;
  int                           unAuth = 0;

  sec_login_get_current_context(&loginContext, &pgoError);
  if ( pgoError == sec_login_s_no_current_context ) {
      unAuth=1;
      /* bind to registry unauthenticated */
      bindingAuthInfo.info_type = sec_rgy_bind_auth_none;

      strncpy(secRoutineName, "sec_rgy_site_bind", sizeof(secRoutineName));
      sec_rgy_site_bind((unsigned_char_t *)"/.:",
			&bindingAuthInfo,
			&secRgyHandle,
			&pgoError);
      if ( pgoError != error_status_ok ) {
	  DACL_PRINT_AND_SET_ERROR;
	  return rtnVal;
      }
  } else {
      secRgyHandle = sec_rgy_default_handle; 
  }

  if (entryP->entry_info.entry_type == sec_acl_e_type_user) {
    strncpy(secRoutineName, "sec_id_gen_name (local)", sizeof(secRoutineName));
    sec_id_gen_name(secRgyHandle,
		    defaultCellUuidP,
		    &(entryP->entry_info.tagged_union.id.uuid),
		    (void *)NULL,
		    cellName,
		    ignoredGlobalName,	/* need this to get it to check the uuid */
		    &pgoError);
    if (pgoError == sec_rgy_object_not_found) {
      /* we might have a short uuid, set up for doing the translation */
      nameDomain = sec_rgy_domain_person;
      principalUuidP = &(entryP->entry_info.tagged_union.id.uuid);
    }
  }
  else if (entryP->entry_info.entry_type == sec_acl_e_type_group) {
    strncpy(secRoutineName, "sec_id_gen_group (local)", sizeof(secRoutineName));
    sec_id_gen_group(secRgyHandle,
		     defaultCellUuidP,
		     &(entryP->entry_info.tagged_union.id.uuid),
		     (void *)NULL,
		     cellName,
		     ignoredGlobalName,	/* need this to get it to check the uuid */
		     &pgoError);
    if (pgoError == sec_rgy_object_not_found) {
      /* we might have a short uuid, set up for doing the translation */
      nameDomain = sec_rgy_domain_group;
      principalUuidP = &(entryP->entry_info.tagged_union.id.uuid);
    }
  }
  else if (entryP->entry_info.entry_type == sec_acl_e_type_foreign_user) {
    strncpy(secRoutineName, "sec_id_gen_name (foreign)", sizeof(secRoutineName));
    sec_id_gen_name(secRgyHandle,
		    &(entryP->entry_info.tagged_union.foreign_id.realm.uuid),
		    &(entryP->entry_info.tagged_union.foreign_id.id.uuid),
		    (void *)NULL,
		    cellName,
		    ignoredGlobalName,	/* need this to get it to check the uuid */
		    &pgoError);
    if (pgoError == sec_rgy_object_not_found) {
      /* we might have a short uuid, set up for doing the translation */
      nameDomain = sec_rgy_domain_person;
      principalUuidP = &(entryP->entry_info.tagged_union.foreign_id.id.uuid);
    }
  }
  else if (entryP->entry_info.entry_type == sec_acl_e_type_foreign_group) {
    strncpy(secRoutineName, "sec_id_gen_group (foreign)", sizeof(secRoutineName));
    sec_id_gen_group(secRgyHandle,
		     &(entryP->entry_info.tagged_union.foreign_id.realm.uuid),
		     &(entryP->entry_info.tagged_union.foreign_id.id.uuid),
		     (void *)NULL,
		     cellName,
		     ignoredGlobalName,	/* need this to get it to check the uuid */
		     &pgoError);
    if (pgoError == sec_rgy_object_not_found) {
      /* we might have a short uuid, set up for doing the translation */
      nameDomain = sec_rgy_domain_group;
      principalUuidP = &(entryP->entry_info.tagged_union.foreign_id.id.uuid);
    }
  }
  
  if (pgoError == sec_rgy_object_not_found) {
      /* we might have a short uuid, try doing the translation */
      /* first, though, we have to bind to the cell's secd */
      if ( unAuth ) {
	  strncpy(secRoutineName, "sec_rgy_site_bind", sizeof(secRoutineName));
	  sec_rgy_site_bind(cellName,
			    &bindingAuthInfo,
			    &secRgyHandle,
			    &pgoError);
      } else {
	  strncpy(secRoutineName, "sec_rgy_site_open", sizeof(secRoutineName));
	  sec_rgy_site_open(cellName,
			    &secRgyHandle,
			    &pgoError);
      }
      if (pgoError == error_status_ok) {
	  Epi_PrinId_FromUuid((epi_principal_id_t *)&unixId, principalUuidP);

	  strncpy(secRoutineName, "sec_rgy_pgo_unix_num_to_id",
		  sizeof(secRoutineName));
	  sec_rgy_pgo_unix_num_to_id(secRgyHandle,
				     nameDomain,
				     unixId,
				     &fullUuid,
				     &pgoError);
	  if (pgoError == error_status_ok) {
	      entryP->entry_info.tagged_union.foreign_id.id.uuid = fullUuid;
	  }
	  else {
	      DACL_PRINT_AND_SET_ERROR;
	  }
      }
      else {
	  DACL_PRINT_AND_SET_ERROR;
      }
  }
  else if (pgoError != error_status_ok) {
    DACL_PRINT_AND_SET_ERROR;
  }
  
  return rtnVal;
}

EXPORT long dacl_To_SecAcl(daclP, secAclP)
  dacl_t *		daclP;
  sec_acl_t *	secAclP;
{
    long		rtnVal = DACL_SUCCESS;
    int			secAclNumEntries = 0;
    sec_acl_entry_t *	secAclEntriesP;
    int			totalNumEntries;
    int			entryCounter;
    int			listCounter;
    int			thisListNumEntries;
    dacl_entry_t *	thisListP;
    sec_acl_entry_t *	entryP;
    uuid_t *		secAclRealmP;
    void (*free_func) _TAKES((void *ptr));
    void *(*alloc_func) _TAKES((unsigned long size));
    unsigned32 st;
    static char		routineName[] = "dacl_To_SecAcl";
    
    if ((daclP != (dacl_t *)NULL) && (secAclP != (sec_acl_t *)NULL)) {
	bzero((char *)secAclP, sizeof(sec_acl_t));

	totalNumEntries = daclP->num_entries;
	/* get the current RPC memory alloc/dealloc routines and reenable the
	 previous RPC memory alloc/dealloc routines  */
	rpc_sm_swap_client_alloc_free((idl_void_p_t (*)(idl_size_t)) malloc,
				      free, &alloc_func, &free_func, &st);
	if (st == error_status_ok) {
	    rpc_sm_set_client_alloc_free(alloc_func, free_func, &st);
	}
	if (st != error_status_ok) {
	    return DACL_ERROR_BUFFER_ALLOCATION;
	}
	secAclP->sec_acl_entries = secAclEntriesP =
	  (sec_acl_entry_t *)(*alloc_func)(totalNumEntries *
					   sizeof(sec_acl_entry_t));
	if (secAclP->sec_acl_entries != (sec_acl_entry_t *)NULL) {
	    bzero((char *)(secAclP->sec_acl_entries),
		  totalNumEntries * sizeof(sec_acl_entry_t));

	    /* copy the acl header */
	    secAclP->default_realm.uuid = *((uuid_t *) &daclP->default_realm);
	    secAclP->sec_acl_manager_type = *((uuid_t *) &daclP->mgr_type_tag);
	    secAclP->num_entries = daclP->num_entries;

	    secAclRealmP = &(secAclP->default_realm.uuid);

	    /* first, copy all the simple entries */
	    for (entryCounter = 0;
		 entryCounter <= (int)dacl_simple_entry_type_unauthmask;
		 entryCounter++) {
		if (daclP->simple_entries[entryCounter].is_entry_good == 1){

		    entryP=&(secAclEntriesP[secAclNumEntries]);

		    switch((int)entryCounter) {
		      case (int)dacl_simple_entry_type_userobj :
			  entryP->entry_info.entry_type =
			      sec_acl_e_type_user_obj;
			  entryP->perms =
			      daclP->simple_entries[entryCounter].perms;
			  break;
			case (int)dacl_simple_entry_type_groupobj :
			    entryP->entry_info.entry_type =
				sec_acl_e_type_group_obj;
			  entryP->perms =
			      daclP->simple_entries[entryCounter].perms;
			  break;
			case (int)dacl_simple_entry_type_otherobj :
			    entryP->entry_info.entry_type =
				sec_acl_e_type_other_obj;
			  entryP->perms =
			      daclP->simple_entries[entryCounter].perms;
			  break;
			case (int)dacl_simple_entry_type_anyother :
			    entryP->entry_info.entry_type =
				sec_acl_e_type_any_other;
			  entryP->perms =
			      daclP->simple_entries[entryCounter].perms;
			  break;
			case (int)dacl_simple_entry_type_maskobj :
			    entryP->entry_info.entry_type =
				sec_acl_e_type_mask_obj;
			  entryP->perms =
			      daclP->simple_entries[entryCounter].perms;
			  break;
			case (int)dacl_simple_entry_type_unauthmask :
			    entryP->entry_info.entry_type =
				sec_acl_e_type_unauthenticated;
			  entryP->perms =
			      daclP->simple_entries[entryCounter].perms;
			  break;
                        case (int)dacl_simple_entry_type_userobj_delegate:
			    entryP->entry_info.entry_type =
				sec_acl_e_type_user_obj_deleg;
			  entryP->perms =
			      daclP->simple_entries[entryCounter].perms;
			  break;
                        case (int)dacl_simple_entry_type_groupobj_delegate:
			    entryP->entry_info.entry_type =
				sec_acl_e_type_group_obj_deleg;
			  entryP->perms =
			      daclP->simple_entries[entryCounter].perms;
			  break;
                        case (int)dacl_simple_entry_type_otherobj_delegate:
			    entryP->entry_info.entry_type =
				sec_acl_e_type_other_obj_deleg;
			  entryP->perms =
			      daclP->simple_entries[entryCounter].perms;
			  break;
                        case (int)dacl_simple_entry_type_anyother_delegate:
			    entryP->entry_info.entry_type =
				sec_acl_e_type_any_other_deleg;
			  entryP->perms =
			      daclP->simple_entries[entryCounter].perms;
			  break;
			  /* no default since we know the values of the outer iteration*/
		      }	/* end switch statement */

		    secAclNumEntries++;
		}	/* end if the simple entry is good */
	    }		/* end iteration through simple entry list */

	    /* now, copy the complex entries */
	    for (listCounter = 0;
		 (listCounter <= (int)dacl_complex_entry_type_other) &&
		 (rtnVal == DACL_SUCCESS);
		 listCounter++) {
		thisListNumEntries =
		    daclP->complex_entries[listCounter].num_entries;
		thisListP =
		    daclP->complex_entries[listCounter].complex_entries;
		for (entryCounter = 0;
		     (entryCounter < thisListNumEntries) &&
		     (rtnVal == DACL_SUCCESS);
		     entryCounter++, secAclNumEntries++) {

		    entryP=&(secAclEntriesP[secAclNumEntries]);

		    entryP->perms =
			thisListP[entryCounter].perms;

		    switch ((int)(thisListP[entryCounter].entry_type)) {
		      case (int)dacl_entry_type_user:
			entryP->entry_info.entry_type = sec_acl_e_type_user;
			entryP->entry_info.tagged_union.id.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.id);
			/* expand the principal uuid from the kernel, if necessary*/
			if (!isFullUuid(&(entryP->entry_info.tagged_union.id.uuid))){
			    rtnVal = dacl_FindFullUuid(entryP, secAclRealmP);
			}
			break;

		      case (int)dacl_entry_type_group:
			entryP->entry_info.entry_type = sec_acl_e_type_group;
			entryP->entry_info.tagged_union.id.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.id);

			/* expand the group uuid from the kernel, if necessary: */
			if (!isFullUuid(&(entryP->entry_info.tagged_union.id.uuid))){
			    rtnVal = dacl_FindFullUuid(entryP, secAclRealmP);
			}
			break;

		      case (int)dacl_entry_type_foreign_other:
			entryP->entry_info.entry_type =
			    sec_acl_e_type_foreign_other;
			entryP->entry_info.tagged_union.id.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.id);
			break;
                      case (int) dacl_entry_type_user_delegate:
			entryP->entry_info.entry_type =
			    sec_acl_e_type_user_deleg;
			entryP->entry_info.tagged_union.id.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.id);
			break;
                      case (int) dacl_entry_type_group_delegate:
			entryP->entry_info.entry_type =
			    sec_acl_e_type_group_deleg;
			entryP->entry_info.tagged_union.id.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.id);
			break;
                      case (int) dacl_entry_type_foreign_other_delegate:
			entryP->entry_info.entry_type =
			    sec_acl_e_type_for_other_deleg;
			entryP->entry_info.tagged_union.id.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.id);
			break;
		      case (int)dacl_entry_type_foreign_user:
			entryP->entry_info.entry_type =
			    sec_acl_e_type_foreign_user;

			/* first, copy the principal uuid */
			entryP->entry_info.tagged_union.foreign_id.id.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.foreign_id.id);

			/* now, copy the realm uuid */
			entryP->entry_info.tagged_union.foreign_id.realm.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.foreign_id.realm);

			/* expand the principal uuid from the kernel,if necessary */
			if (!isFullUuid(&(entryP->entry_info.tagged_union.foreign_id.id.uuid)))	{
			    rtnVal = dacl_FindFullUuid(entryP, secAclRealmP);
			}
			break;

		      case (int)dacl_entry_type_foreign_group:
			entryP->entry_info.entry_type =
			    sec_acl_e_type_foreign_group;

			/* first, copy the principal uuid */
			entryP->entry_info.tagged_union.foreign_id.id.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.foreign_id.id);

			/* now, copy the realm uuid */
			entryP->entry_info.tagged_union.foreign_id.realm.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.foreign_id.realm);

			/* expand the group uuid from the kernel, if necessary: */
			if (!isFullUuid(&(entryP->entry_info.tagged_union.foreign_id.id.uuid))){
			    rtnVal = dacl_FindFullUuid(entryP, secAclRealmP);
			}
			break;

                      case (int)dacl_entry_type_foreign_user_delegate:
			entryP->entry_info.entry_type =
			    sec_acl_e_type_for_user_deleg;

			/* first, copy the principal uuid */
			entryP->entry_info.tagged_union.foreign_id.id.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.foreign_id.id);

			/* now, copy the realm uuid */
			entryP->entry_info.tagged_union.foreign_id.realm.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.foreign_id.realm);

			/* expand the group uuid from the kernel, if necessary: */
			if (!isFullUuid(&(entryP->entry_info.tagged_union.foreign_id.id.uuid))){
			    rtnVal = dacl_FindFullUuid(entryP, secAclRealmP);
			}
			break;

                      case (int)dacl_entry_type_foreign_group_delegate:
			entryP->entry_info.entry_type =
			    sec_acl_e_type_for_group_deleg;

			/* first, copy the principal uuid */
			entryP->entry_info.tagged_union.foreign_id.id.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.foreign_id.id);

			/* now, copy the realm uuid */
			entryP->entry_info.tagged_union.foreign_id.realm.uuid =
			  *((uuid_t *) &thisListP[entryCounter].entry_info.foreign_id.realm);

			/* expand the group uuid from the kernel, if necessary: */
			if (!isFullUuid(&(entryP->entry_info.tagged_union.foreign_id.id.uuid))){
			    rtnVal = dacl_FindFullUuid(entryP, secAclRealmP);
			}
			break;

		      case (int)dacl_entry_type_extended:
			rtnVal =
			    dacl_To_SecAcl_ExtendedInfo(&(thisListP[entryCounter]),
							entryP);
			break;

			default :
			    dmprintf(dacl_debug_flag,
				     DACL_DEBUG_BIT_SECACL_TRANSLATE,
				     ("%s: unrecoginzed entry type, %d, found in daclP",
				      routineName,
				      (int)(thisListP[entryCounter].entry_type)));
			icl_Trace1(dacl_iclSetp, DACL_ICL_ACL_TRANSLATE_9,
				   ICL_TYPE_LONG, (thisListP[entryCounter].entry_type));
			rtnVal = DACL_ERROR_UNRECOGNIZED_ENTRY_TYPE;
		    }	/* end switch statement */
		}
	    }
	}	/* end if we could allocate space */
	else {
	    dmprintf(dacl_debug_flag, DACL_DEBUG_BIT_SECACL_TRANSLATE,
		     ("%s: error allocating buffer for secAclP entries",
		      routineName));
	    icl_Trace0(dacl_iclSetp, DACL_ICL_ACL_TRANSLATE_10 );
	    rtnVal = DACL_ERROR_BUFFER_ALLOCATION;
	}
    }
    else {
	if (daclP == (dacl_t *)NULL) {
	    dacl_LogParamError(routineName, "daclP", dacl_debug_flag,
			       __FILE__, __LINE__);
	    rtnVal = DACL_ERROR_PARAMETER_ERROR;
	}
	if (secAclP == (sec_acl_t *)NULL) {
	    dacl_LogParamError(routineName, "secAclP", dacl_debug_flag,
			       __FILE__, __LINE__);
	    rtnVal = DACL_ERROR_PARAMETER_ERROR;
	}
    }
    return rtnVal;
}

/* routines that do the inter-conversions and parsing/flattening */
EXPORT long sec_acl_FlattenAcl(secAclP, byteBufferPP, bytesInBufferP)
     sec_acl_t *	secAclP;
     char **		byteBufferPP;
     unsigned int *	bytesInBufferP;
{
  long		rtnVal = DACL_SUCCESS;
  dacl_t	theDacl;

  rtnVal = dacl_From_SecAcl(&theDacl, secAclP);
  if (rtnVal == DACL_SUCCESS) {
    rtnVal = dacl_FlattenAclWithModeBits(&theDacl, byteBufferPP, bytesInBufferP, 0,
					 0 /* => don't use mode bits */,
					 0 /* => not for disk (for syscall) */);
    dacl_FreeAclEntries(&theDacl);
  }
  
  return rtnVal;
}

EXPORT long sec_acl_ParseAcl(secAclP, byteBufferP, bytesInBuffer)
     sec_acl_t *	secAclP;
     char *		byteBufferP;
     unsigned int	bytesInBuffer;
{
  long		rtnVal = DACL_SUCCESS;
  dacl_t	theDacl;
  
  rtnVal = dacl_ParseSyscallAcl(byteBufferP, bytesInBuffer, &theDacl, &episodeAclMgmtUuid);
  if (rtnVal == DACL_SUCCESS) {
    rtnVal = dacl_To_SecAcl(&theDacl, secAclP);
    dacl_FreeAclEntries(&theDacl);
  }

  return rtnVal;
}
