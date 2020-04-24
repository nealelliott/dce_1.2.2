/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dacl_getmode.c,v $
 * Revision 1.1.640.1  1996/10/02  18:15:38  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:32  damon]
 *
 * $EndLog$
 */
/*
 *	dacl_getmode.c -- pull permission bits out of a linearized ACL
 *
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <dcedfs/fshs_deleg.h>
#include <dacl.h>
#include <dacl_debug.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dacl/dacl_getmode.c,v 1.1.640.1 1996/10/02 18:15:38 damon Exp $")

#define DACL_ALL_USER_PERMBITS	(S_IREAD | S_IWRITE | S_IEXEC)
EXPORT long dacl_ExtractPermBits(daclP, permBitsP)
     dacl_t *	daclP;
     u_int32 *	permBitsP;
{
  long			rtnVal = DACL_SUCCESS;
  u_int32		tempPermBits = 0;
  u_int32		thePermBits = 0;
  static char		routineName[] = "dacl_ExtractPermBits";

  if ((daclP != (dacl_t *)NULL) && (permBitsP != (u_int32 *)NULL)) {
    if (daclP->simple_entries[(int)dacl_simple_entry_type_userobj].is_entry_good == 1) {
      dacl_OnePermsetToPermBits(daclP->simple_entries[(int)
						      dacl_simple_entry_type_userobj].perms,
				&thePermBits);
    }
    else {
      thePermBits = DACL_ALL_USER_PERMBITS;
    }

    if (daclP->simple_entries[(int)dacl_simple_entry_type_maskobj].is_entry_good == 1) {
      dacl_OnePermsetToPermBits(daclP->simple_entries[(int)
						      dacl_simple_entry_type_maskobj].perms,
				&tempPermBits);
    }
    else {
      if (daclP->simple_entries[(int)dacl_simple_entry_type_groupobj].is_entry_good == 1) {
	dacl_OnePermsetToPermBits(daclP->simple_entries[(int)
							dacl_simple_entry_type_groupobj].perms,
				  &tempPermBits);
      }
      else {
	tempPermBits = DACL_ALL_USER_PERMBITS;
      }
    }
    thePermBits |= (tempPermBits >> 3);

    if (daclP->simple_entries[(int)dacl_simple_entry_type_otherobj].is_entry_good == 1) {
      dacl_OnePermsetToPermBits(daclP->simple_entries[
						      (int)dacl_simple_entry_type_otherobj].perms,
				&tempPermBits);
    }
    else {
      tempPermBits = DACL_ALL_USER_PERMBITS;
    }
    thePermBits |= (tempPermBits >> 6);

    *permBitsP = thePermBits;
  }
  else {
    if (daclP == (dacl_t *)NULL) {
      dacl_LogParamError(routineName, "daclP", 1, __FILE__, __LINE__);
    }
    if (permBitsP == (u_int32 *)NULL) {
      dacl_LogParamError(routineName, "permBitsP", 1, __FILE__, __LINE__);
    }
    
    rtnVal = DACL_ERROR_PARAMETER_ERROR;
  }
  
  return rtnVal;
}

/*
 * dacl_ExtractMinimumPermSet -- extract baseline permissions from ACL
 *
 * Get the intersection of the permissions granted by all entries, giving
 * zero if there is no any_other entry.  This intersection
 * is passed to the client, which can use it to get pessimistic evaluations
 * of access rights and sometimes thereby avoid unnecessary RPC's.
 */

EXPORT void dacl_ExtractMinimumPermSet(daclP, permsetP)
     dacl_t *		daclP;
     dacl_permset_t *	permsetP;
{
  int			i, j;
  dacl_entry_t *	thisListP = (dacl_entry_t *)NULL;

  /*
   * The any_other entry is crucial.  If there isn't one, random foreign
   * users can't get any access, so might as well drop out now.
   */
  if (!daclP->simple_entries[(int)dacl_simple_entry_type_anyother].is_entry_good) {
    *permsetP = 0;
    return;
  }

  *permsetP = ~0L;
  /* Get all the simple entries */
  for (i = 0; i <= (int)dacl_simple_entry_type_unauthmask; i++)
    if (daclP->simple_entries[i].is_entry_good)
      *permsetP &= daclP->simple_entries[i].perms;

  /* Get all the complex entries */
  for (i = 0; i <= (int)dacl_complex_entry_type_other; i++) {
    thisListP = daclP->complex_entries[i].complex_entries;
    for (j = 0; j < daclP->complex_entries[i].num_entries; j++)
      *permsetP &= thisListP[j].perms;
  }
}

EXPORT long dacl_ChmodAcl(daclP, permBits, forDirectory)
     dacl_t *	daclP;
     u_int32	permBits;
     int	forDirectory;
{
  long		rtnVal = DACL_SUCCESS;
  static char	routineName[] = "dacl_ChmodAcl";

  if (daclP != (dacl_t *)NULL) {
    /* first, deal with the user permbits */
    if (daclP->simple_entries[(int)dacl_simple_entry_type_userobj].is_entry_good == 1) {
      dacl_ChmodOnePermset(permBits,
			   &(daclP->simple_entries[(int)dacl_simple_entry_type_userobj].perms),
			   forDirectory);
    }

    /* now, the group permbits */
    if (daclP->simple_entries[(int)dacl_simple_entry_type_maskobj].is_entry_good == 1) {
      dacl_ChmodOnePermset(permBits << 3,
			   &(daclP->simple_entries[(int)dacl_simple_entry_type_maskobj].perms),
			   forDirectory);
    }
    else if (daclP->simple_entries[(int)dacl_simple_entry_type_groupobj].is_entry_good == 1) {
      dacl_ChmodOnePermset(permBits << 3,
			   &(daclP->simple_entries[(int)dacl_simple_entry_type_groupobj].perms),
			   forDirectory);
    }

    /* now, the other permbits */
    if (daclP->simple_entries[(int)dacl_simple_entry_type_otherobj].is_entry_good == 1) {
      dacl_ChmodOnePermset(permBits << 6,
			   &(daclP->simple_entries[(int)dacl_simple_entry_type_otherobj].perms),
			   forDirectory);
    }

  }
  else {
    dacl_LogParamError(routineName, "daclP", 1, __FILE__, __LINE__);
    rtnVal = DACL_ERROR_PARAMETER_ERROR;
  }

  return rtnVal;
}
