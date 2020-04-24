/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dacl_modify.c,v $
 * Revision 1.1.744.1  1996/10/02  18:15:47  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:42  damon]
 *
 * $EndLog$
 */
/*
 *	dacl_modify.c -- general routines for manipulating ACLs
 *
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/icl.h>

#include <dcedfs/fshs_deleg.h>
#include <dacl.h>
#include <dacl_debug.h>
#include <dacl_trace.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

extern struct icl_set *dacl_iclSetp;
  
RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/security/dacl/dacl_modify.c,v 1.1.744.1 1996/10/02 18:15:47 damon Exp $")

PRIVATE dacl_simple_entry_t * dacl_TestSimpleEntryInAcl(aclP, entryP)
     dacl_t *		aclP;
     dacl_entry_t *	entryP;
{
  dacl_simple_entry_t *	rtnVal = (dacl_simple_entry_t *)NULL;	/* assume not there */
  static char		routineName[] = "dacl_TestSimpleEntryInAcl";
  
  icl_Trace1(dacl_iclSetp, DACL_ICL_MODIFY_TESTENTRY_0 , ICL_TYPE_LONG, entryP->entry_type);

  if ((entryP->entry_type == dacl_entry_type_user_obj) &&
      (aclP->simple_entries[(int)dacl_simple_entry_type_userobj].is_entry_good == 1)) {
    rtnVal = &(aclP->simple_entries[(int)dacl_simple_entry_type_userobj]);
  }
  else if ((entryP->entry_type == dacl_entry_type_group_obj) &&
	   (aclP->simple_entries[(int)dacl_simple_entry_type_groupobj].is_entry_good
	    == 1)) {
    rtnVal = &(aclP->simple_entries[(int)dacl_simple_entry_type_groupobj]);
  }
  else if ((entryP->entry_type == dacl_entry_type_other_obj) &&
	   (aclP->simple_entries[(int)dacl_simple_entry_type_otherobj].is_entry_good
	    == 1)) {
    rtnVal = &(aclP->simple_entries[(int)dacl_simple_entry_type_otherobj]);
  }
  else if ((entryP->entry_type == dacl_entry_type_mask_obj) &&
	   (aclP->simple_entries[(int)dacl_simple_entry_type_maskobj].is_entry_good
	    == 1)) {
    rtnVal = &(aclP->simple_entries[(int)dacl_simple_entry_type_maskobj]);
  }
  else if ((entryP->entry_type == dacl_entry_type_anyother) &&
	   (aclP->simple_entries[(int)dacl_simple_entry_type_anyother].is_entry_good
	    == 1)) {
    rtnVal = &(aclP->simple_entries[(int)dacl_simple_entry_type_anyother]);
  }
  else if ((entryP->entry_type == dacl_entry_type_unauth) &&
	   (aclP->simple_entries[(int)dacl_simple_entry_type_unauthmask].is_entry_good
	    == 1)) {
    rtnVal = &(aclP->simple_entries[(int)dacl_simple_entry_type_unauthmask]);
  }
  else if ((entryP->entry_type == dacl_entry_type_user_obj_delegate) &&
	   (aclP->simple_entries[(int)dacl_simple_entry_type_userobj_delegate].is_entry_good
	    == 1)) {
    rtnVal = &(aclP->simple_entries[(int)dacl_simple_entry_type_userobj_delegate]);
  }
  else if ((entryP->entry_type == dacl_entry_type_group_obj_delegate) &&
	   (aclP->simple_entries[(int)dacl_simple_entry_type_groupobj_delegate].is_entry_good
	    == 1)) {
    rtnVal = &(aclP->simple_entries[(int)dacl_simple_entry_type_groupobj_delegate]);
  }
  else if ((entryP->entry_type == dacl_entry_type_other_obj_delegate) &&
	   (aclP->simple_entries[(int)dacl_simple_entry_type_otherobj_delegate].is_entry_good
	    == 1)) {
    rtnVal = &(aclP->simple_entries[(int)dacl_simple_entry_type_otherobj_delegate]);
  }
  else if ((entryP->entry_type == dacl_entry_type_any_other_delegate) &&
	   (aclP->simple_entries[(int)dacl_simple_entry_type_anyother_delegate].is_entry_good
	    == 1)) {
    rtnVal = &(aclP->simple_entries[(int)dacl_simple_entry_type_anyother_delegate]);
  }

  icl_Trace1(dacl_iclSetp, DACL_ICL_MODIFY_TESTENTRY_1 , ICL_TYPE_STRING, 
	     ((rtnVal!=(dacl_simple_entry_t*)NULL)?"":" not"));

  return rtnVal;
}

PRIVATE dacl_entry_t * dacl_TestComplexEntryInList(entryListP, numbInList, entryP)
     dacl_entry_t *		entryListP;
     u_int32			numbInList;
     dacl_entry_t *		entryP;
{
  dacl_entry_t *	rtnVal = (dacl_entry_t *)NULL;	/* assume it's not there */
  int			entryCount;
  static char		routineName[] = "dacl_TestComplexEntryInList";

  icl_Trace1(dacl_iclSetp, DACL_ICL_MODIFY_TESTENTRY_2 , ICL_TYPE_LONG, entryP->entry_type);

  for (entryCount = 0;
       (rtnVal == (dacl_entry_t *)NULL) && (entryCount < numbInList);
       entryCount++) {
    icl_Trace2(dacl_iclSetp, DACL_ICL_MODIFY_TESTENTRY_3 , ICL_TYPE_LONG, entryCount, ICL_TYPE_LONG, entryListP[entryCount].entry_type);
    if (entryP->entry_type == entryListP[entryCount].entry_type) {
      if ((entryP->entry_type == dacl_entry_type_user) ||
	  (entryP->entry_type == dacl_entry_type_group) ||
	  (entryP->entry_type == dacl_entry_type_foreign_other) ||
          (entryP->entry_type == dacl_entry_type_user_delegate) ||
          (entryP->entry_type == dacl_entry_type_group_delegate)) {
	if (Epi_PrinId_Cmp(&(entryP->entry_info.id),
			   &(entryListP[entryCount].entry_info.id)) == 0) {
	  rtnVal = &(entryListP[entryCount]);
	}
      }
      else if ((entryP->entry_type == dacl_entry_type_foreign_user) ||
	       (entryP->entry_type == dacl_entry_type_foreign_group) ||
               (entryP->entry_type == dacl_entry_type_foreign_user_delegate) ||
               (entryP->entry_type == dacl_entry_type_foreign_group_delegate)) {
	if ((bcmp((char *)&(entryP->entry_info.foreign_id.realm),
		  (char *)&(entryListP[entryCount].entry_info.foreign_id.realm),
		  sizeof(epi_uuid_t))
	     == 0) &&
	    (Epi_PrinId_Cmp(&(entryP->entry_info.foreign_id.id),
			    &(entryListP[entryCount].entry_info.foreign_id.id))
	     == 0)) {
	  rtnVal = &(entryListP[entryCount]);
	}
      }
      else {
	/* type is enough to match in these cases */
	/* this is now an error condition */
      }
    }
  }

  icl_Trace1(dacl_iclSetp, DACL_ICL_MODIFY_TESTENTRY_4 , ICL_TYPE_STRING,
	     ((rtnVal!=(dacl_entry_t*)NULL) ? "" :" not"));
  return rtnVal;
}


PRIVATE dacl_entry_t * dacl_TestComplexEntryInAcl(aclP, entryP)
     dacl_t *			aclP;
     dacl_entry_t *		entryP;
{
  dacl_entry_t *	rtnVal = (dacl_entry_t *)NULL;	/* assume it's not here */
  int			listCount;
  static char		routineName[] = "dacl_TestComplexEntryInAcl";

  icl_Trace1(dacl_iclSetp, DACL_ICL_MODIFY_TESTENTRY_5 , ICL_TYPE_LONG, entryP->entry_type);

  for (listCount = 0;
       (rtnVal == (dacl_entry_t *)NULL) && (listCount <= (int)dacl_complex_entry_type_other);
       listCount++) {
    rtnVal = dacl_TestComplexEntryInList(aclP->complex_entries[listCount].complex_entries,
					 aclP->complex_entries[listCount].num_entries,
					 entryP);
  }
  
  icl_Trace1(dacl_iclSetp, DACL_ICL_MODIFY_TESTENTRY_6, 
	     ICL_TYPE_STRING, ((rtnVal != (dacl_entry_t*)NULL) ? "" : " not"));
  return rtnVal;
}

EXPORT long dacl_AddEntryToAcl(aclP, typeStringP, nameStringP, permStringP,
			       dataStringP, mgrUuidP, local)
     dacl_t *		aclP;
     char *		typeStringP;
     char *		nameStringP;
     char *		permStringP;
     char *		dataStringP;
     epi_uuid_t *	mgrUuidP;
     int		local;
{
  long				rtnVal = DACL_SUCCESS;
  dacl_entry_t *		newEntryArray = (dacl_entry_t *)NULL;
  dacl_entry_t			newEntry;
  dacl_simple_entry_t *		thisSimpleEntryP = (dacl_simple_entry_t *)NULL;
  int				intEntryType;
  dacl_complex_entry_t *	thisComplexListP = (dacl_complex_entry_t *)NULL;
  static char			routineName[] = "dacl_AddEntryToAcl";
  
  rtnVal = dacl_InitAclEntryFromStrings(&newEntry, &(aclP->default_realm),
					typeStringP, nameStringP,
					permStringP, dataStringP, mgrUuidP, local);
  
  if (rtnVal == DACL_SUCCESS) {
    intEntryType = newEntry.entry_type;
    if ((long)intEntryType == newEntry.entry_type) {
      switch (intEntryType) {
	/* for the simple entries, we already have all the info we need */
      case (int)dacl_entry_type_user_obj:
	if (thisSimpleEntryP == (dacl_simple_entry_t *)NULL) {
	  thisSimpleEntryP =
	    &(aclP->simple_entries[(int)dacl_simple_entry_type_userobj]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_group_obj:
	if (thisSimpleEntryP == (dacl_simple_entry_t *)NULL) {
	  thisSimpleEntryP =
	    &(aclP->simple_entries[(int)dacl_simple_entry_type_groupobj]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_other_obj:
	if (thisSimpleEntryP == (dacl_simple_entry_t *)NULL) {
	  thisSimpleEntryP =
	    &(aclP->simple_entries[(int)dacl_simple_entry_type_otherobj]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_mask_obj:
	if (thisSimpleEntryP == (dacl_simple_entry_t *)NULL) {
	  thisSimpleEntryP =
	    &(aclP->simple_entries[(int)dacl_simple_entry_type_maskobj]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_anyother:
	if (thisSimpleEntryP == (dacl_simple_entry_t *)NULL) {
	  thisSimpleEntryP =
	    &(aclP->simple_entries[(int)dacl_simple_entry_type_anyother]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_unauth:
	if (thisSimpleEntryP == (dacl_simple_entry_t *)NULL) {
	  thisSimpleEntryP =
	    &(aclP->simple_entries[(int)dacl_simple_entry_type_unauthmask]);
	}
      /* intentional fall through */
      case (int) dacl_entry_type_user_obj_delegate:
	if (thisSimpleEntryP == (dacl_simple_entry_t *)NULL) {
	  thisSimpleEntryP =
	    &(aclP->simple_entries[(int)dacl_simple_entry_type_userobj_delegate]);
	}      
      case (int) dacl_entry_type_group_obj_delegate:
	if (thisSimpleEntryP == (dacl_simple_entry_t *)NULL) {
	  thisSimpleEntryP =
	    &(aclP->simple_entries[(int)dacl_simple_entry_type_groupobj_delegate]);
	}
      /* intentional fall through */
      case (int) dacl_entry_type_other_obj_delegate:
	if (thisSimpleEntryP == (dacl_simple_entry_t *)NULL) {
	  thisSimpleEntryP =
	    &(aclP->simple_entries[(int)dacl_simple_entry_type_otherobj_delegate]);
	}
      /* intentional fall through */
      case (int) dacl_entry_type_any_other_delegate:
	if (thisSimpleEntryP == (dacl_simple_entry_t *)NULL) {
	  thisSimpleEntryP =
	    &(aclP->simple_entries[(int)dacl_simple_entry_type_anyother_delegate]);
	}

	if (thisSimpleEntryP->is_entry_good == 0) {
	  thisSimpleEntryP->is_entry_good = 1;
	  thisSimpleEntryP->perms = newEntry.perms;
	  aclP->num_entries++;
	}
	else {
	  icl_Trace1(dacl_iclSetp, DACL_ICL_MODIFY_PARSE_7 , ICL_TYPE_LONG, newEntry.entry_type);
	  rtnVal = DACL_ERROR_ENTRY_EXISTS;
	}
	
	break;
	
	/* now, we deal with the complex entry types */
      case (int)dacl_entry_type_user:
	if (thisComplexListP == (dacl_complex_entry_t *)NULL) {
	  thisComplexListP = &(aclP->complex_entries[(int)dacl_complex_entry_type_user]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_user_delegate:
	if (thisComplexListP == (dacl_complex_entry_t *)NULL) {
	  thisComplexListP = &(aclP->complex_entries[(int)dacl_complex_entry_type_user_delegate]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_group:
	if (thisComplexListP == (dacl_complex_entry_t *)NULL) {
	  thisComplexListP = &(aclP->complex_entries[(int)dacl_complex_entry_type_group]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_group_delegate:
	if (thisComplexListP == (dacl_complex_entry_t *)NULL) {
	  thisComplexListP = &(aclP->complex_entries[(int)dacl_complex_entry_type_group_delegate]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_foreign_other:
	if (thisComplexListP == (dacl_complex_entry_t *)NULL) {
	  thisComplexListP = &(aclP->complex_entries[(int)dacl_complex_entry_type_other]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_foreign_other_delegate:
	if (thisComplexListP == (dacl_complex_entry_t *)NULL) {
	  thisComplexListP = &(aclP->complex_entries[(int)dacl_complex_entry_type_for_other_delegate]);
	}
	/* fall through to next case is intentional */

      case (int)dacl_entry_type_foreign_user:
	if (thisComplexListP == (dacl_complex_entry_t *)NULL) {
	  thisComplexListP = &(aclP->complex_entries[(int)dacl_complex_entry_type_user]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_foreign_user_delegate:
	if (thisComplexListP == (dacl_complex_entry_t *)NULL) {
	  thisComplexListP = &(aclP->complex_entries[(int)dacl_complex_entry_type_user_delegate]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_foreign_group:
	if (thisComplexListP == (dacl_complex_entry_t *)NULL) {
	  thisComplexListP = &(aclP->complex_entries[(int)dacl_complex_entry_type_group]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_foreign_group_delegate:
	if (thisComplexListP == (dacl_complex_entry_t *)NULL) {
	  thisComplexListP = &(aclP->complex_entries[(int)dacl_complex_entry_type_group_delegate]);
	}
	/* fall through to next case is intentional */
      case (int)dacl_entry_type_extended:
	if (thisComplexListP == (dacl_complex_entry_t *)NULL) {
	  thisComplexListP = &(aclP->complex_entries[(int)dacl_complex_entry_type_other]);
	}
	
	if (dacl_TestComplexEntryInList(thisComplexListP->complex_entries,
					thisComplexListP->num_entries,
					&newEntry) == (dacl_entry_t *)NULL) {
	  /* see if we already have an entry allocated */
	  if (thisComplexListP->num_entries < thisComplexListP->entries_allocated) {
	      /* have one we can use */
	      thisComplexListP->complex_entries[thisComplexListP->num_entries] = newEntry;
	      thisComplexListP->num_entries++;
	  }
	  else {
	      newEntryArray = (dacl_entry_t *)osi_Alloc(((thisComplexListP->num_entries + 1) *
							 sizeof(dacl_entry_t)));
	      osi_assert(newEntryArray);
	      bcopy((char *)(thisComplexListP->complex_entries),
		    (char *)newEntryArray,
		    (thisComplexListP->num_entries * sizeof(dacl_entry_t)));
	      newEntryArray[thisComplexListP->num_entries] = newEntry;

	      if (thisComplexListP->entries_allocated != 0) {
		  osi_Free((opaque)(thisComplexListP->complex_entries),
			   (long)(thisComplexListP->entries_allocated * sizeof(dacl_entry_t)));
	      }

	      thisComplexListP->complex_entries = newEntryArray;
	      thisComplexListP->num_entries++;
	      thisComplexListP->entries_allocated = thisComplexListP->num_entries;
	  }
	  aclP->num_entries++;
	}
	else {
	  icl_Trace1(dacl_iclSetp, DACL_ICL_MODIFY_PARSE_8 , ICL_TYPE_LONG, newEntry.entry_type);
	  rtnVal = DACL_ERROR_ENTRY_EXISTS;
	}
	
	break;
	
      default:
	/* some sort of unrecognized type logging */
	rtnVal = DACL_ERROR_UNRECOGNIZED_ENTRY_TYPE;
	break;
      }	/* end switch */
      
    }
    else {
      icl_Trace1(dacl_iclSetp, DACL_ICL_MODIFY_NONE_0 , ICL_TYPE_LONG, newEntry.entry_type);
      rtnVal = DACL_ERROR_ENTRY_TYPE_TOO_LARGE;
    }
  }
  
  return rtnVal;
}

EXPORT long dacl_ModifyAclEntry(aclP, typeStringP, nameStringP, permStringP,
				dataStringP, local)
     dacl_t *	aclP;
     char *	typeStringP;
     char *	nameStringP;
     char *	permStringP;
     char *	dataStringP;
     int	local;
{
  int			rtnVal = DACL_SUCCESS;
  dacl_entry_t		newEntry;
  dacl_entry_t *	thisEntryP = (dacl_entry_t *)NULL;
  dacl_simple_entry_t *	thisSimpleEntryP = (dacl_simple_entry_t *)NULL;
  static char		routineName[] = "dacl_ModifyAclEntry";
  
  rtnVal = dacl_InitAclEntryFromStrings(&newEntry, &(aclP->default_realm),
					typeStringP, nameStringP,
					permStringP, dataStringP,
					&(aclP->mgr_type_tag),
					local);
  if (rtnVal == DACL_SUCCESS) {
    thisSimpleEntryP = dacl_TestSimpleEntryInAcl(aclP, &newEntry);
    if (thisSimpleEntryP != (dacl_simple_entry_t *)NULL) {
      thisSimpleEntryP->perms = newEntry.perms;
    }
    else {
      thisEntryP = dacl_TestComplexEntryInAcl(aclP, &newEntry);
      if (thisEntryP != (dacl_entry_t *)NULL) {
	/* don't fool around, just do it */
	*thisEntryP = newEntry;
      }
      else {
	icl_Trace0(dacl_iclSetp, DACL_ICL_MODIFY_WARNINGS_9 );
	rtnVal = DACL_ERROR_ENTRY_NOT_FOUND;
      } 
    }
  }

  return rtnVal;
}

EXPORT long dacl_DeleteAclEntry(aclP, typeStringP, nameStringP, mgrUuidP, local)
     dacl_t *		aclP;
     char *		typeStringP;
     char *		nameStringP;
     epi_uuid_t *	mgrUuidP;
     int		local;
{
  int			rtnVal = DACL_SUCCESS;
  dacl_entry_t *	newEntryArray = (dacl_entry_t *)NULL;
  dacl_entry_t		newEntry;
  dacl_simple_entry_t *	thisSimpleEntryP = (dacl_simple_entry_t *)NULL;
  dacl_entry_t *	thisEntryP = (dacl_entry_t *)NULL;
  int			oldEntryCount;
  int			newEntryCount;
  int			listCount;
  int			listFound = 0;
  dacl_entry_t *	thisListP;
  int			numEntriesThisList;
  int		        entriesAllocatedThisList;
  static char		routineName[] = "dacl_DeleteAclEntry";
  
  rtnVal = dacl_InitAclEntryFromStrings(&newEntry, &(aclP->default_realm),
					typeStringP, nameStringP,
					(char *)NULL, (char *)NULL,
					mgrUuidP, local);
  if (rtnVal == DACL_SUCCESS) {
    if ((dacl_AreObjectEntriesRequired(mgrUuidP) == 0) ||
	((newEntry.entry_type != dacl_entry_type_user_obj) &&
	 (newEntry.entry_type != dacl_entry_type_group_obj))) {
      thisSimpleEntryP = dacl_TestSimpleEntryInAcl(aclP, &newEntry);
      if (thisSimpleEntryP != (dacl_simple_entry_t *)NULL) {
	thisSimpleEntryP->is_entry_good = 0;
	aclP->num_entries--;
      }
      else {
	thisEntryP = dacl_TestComplexEntryInAcl(aclP, &newEntry);
	if (thisEntryP != (dacl_entry_t *)NULL) {
	  for (listCount = 0;
	       (listFound == 0) && (listCount <= (int)dacl_complex_entry_type_other);
	       listCount++) {
	    thisListP = aclP->complex_entries[listCount].complex_entries;
	    numEntriesThisList = aclP->complex_entries[listCount].num_entries;
	    entriesAllocatedThisList = aclP->complex_entries[listCount].entries_allocated;

	    if ((thisEntryP >= thisListP) &&
		(thisEntryP < (thisListP + numEntriesThisList))) {

	      if (numEntriesThisList == 1) {
		  /* deleting the last extended entry */
		  osi_Free((opaque)thisListP,
			   (long)(entriesAllocatedThisList * sizeof(dacl_entry_t)));
		  
		  aclP->complex_entries[listCount].complex_entries = NULL;
		  aclP->complex_entries[listCount].num_entries = 0;
		  aclP->complex_entries[listCount].entries_allocated = 0;
	      }
	      else {
		  for (oldEntryCount = 0, newEntryCount = 0;
		       oldEntryCount < numEntriesThisList;
		       oldEntryCount++) {

		      if (&(thisListP[oldEntryCount]) != thisEntryP) {
			  if (oldEntryCount != newEntryCount) {
			      /* if so, old must be > new.  slide entries down */
			      thisListP[newEntryCount] = thisListP[oldEntryCount];
			  }
			  newEntryCount++;
		      }
		  }
		  aclP->complex_entries[listCount].num_entries--;
	      }
	      aclP->num_entries--;

	      listFound = 1;
	    }
	  }
	}
	
	else {
	  icl_Trace0(dacl_iclSetp, DACL_ICL_MODIFY_WARNINGS_10 );
	  rtnVal = DACL_ERROR_ENTRY_NOT_FOUND;
	}
      }
    }
    else {
      icl_Trace1(dacl_iclSetp, DACL_ICL_MODIFY_WARNINGS_11 , ICL_TYPE_STRING, typeStringP);
      rtnVal = DACL_ERROR_ENTRY_REQUIRED;
    }
  }

  return rtnVal;
}

EXPORT long dacl_DeleteAllEntries(aclP)
     dacl_t *	aclP;
{
  long		rtnVal = DACL_SUCCESS;
  int		i;
  
  /* invalidate all the simple entries */
  for (i = 0; i <= (int)dacl_simple_entry_type_unauthmask; i++) {
    aclP->simple_entries[i].is_entry_good = 0;
  }

  /* now, get rid of all the complex entries and any extended info */
  rtnVal = dacl_FreeAclEntries(aclP);
  
  aclP->num_entries = 0;
  
  return rtnVal;
}



