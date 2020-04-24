/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dacl_sec_rgy.c,v $
 * Revision 1.1.651.1  1996/10/02  20:56:13  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:49  damon]
 *
 * $EndLog$
*/
/*
 *	dacl_sec_rgy.c -- all the routines that require the sec_rgy interface
 *  and, therefore, should not be used (or compiled) inside the kernel (these
 *  routines are mostly used for testing purposes, anyway)
 *
 * Copyright (C) 1991, 1995 Transarc Corporation
 * All rights reserved.
 */

#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#ifndef KERNEL
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */
#endif

#include <dce/id_base.h>
#include <dce/pgo.h>
#include <dce/secidmap.h>

#include <pwd.h>
#include <grp.h>

#include <epi_id.h>

#include <../exception_print.h>
#include <dcedfs/fshs_deleg.h>
#include <dacl.h>
#include <dacl_debug.h>
#include <dacl_trace.h>

#include <dcedfs/icl.h>

extern struct icl_set *dacl_iclSetp;

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dacl/dacl_sec_rgy.c,v 1.1.651.1 1996/10/02 20:56:13 damon Exp $")
  
IMPORT char * RgySec_ErrorString _TAKES((error_status_t * secErrorP));

#ifdef AFS_SUNOS5_ENV
/* use short value here, since UFS only stores 16 bits on disk.  Now,
 * it may look like 32 bits are stored, and they are, for a while, but
 * not on disk, and when the UFS inode gets recycled, the bits revert
 * to 65534.  So, start it off there.
 */
static long unauthID = 65534;           /* Anonymous user ID */
#else /* not AFS_SUNOS5_ENV */
static long unauthID = -2;              /* Anonymous user ID */
#endif /* not AFS_SUNOS5_ENV */


/*
 * from dacl_pac.c
 */
EXPORT long dacl_InitPacList(PacListP, userNameP, groupNameP, duserNameP, dgroupNameP, local, unauth)
     pac_list_t *	PacListP;
     char *		userNameP;
     char *		groupNameP;
     char *		duserNameP;
     char *		dgroupNameP;
     int		local;
     int		unauth;
{
  long			rtnVal = DACL_SUCCESS;
  error_status_t	pgoError;
  volatile int		exceptionRaised, simple_id = 0;
  struct passwd *	pwentP;
  struct group *	grentP;
  uuid_t		realmUuid;

  static char		routineName[] = "dacl_InitPacList";
  
  if (duserNameP == NULL && dgroupNameP == NULL)
	simple_id = 1;
    
  if (unauth) {
        bcopy((char *)&unauthID,(char *)&PacListP->pacp->principal.uuid,
							sizeof(long));
        bcopy((char *)&unauthID, (char *)&PacListP->pacp->group.uuid, 
							sizeof(long));
        bcopy((char *)&unauthID, (char *)&PacListP->pacp->realm.uuid, 
							sizeof(long));
        return DACL_SUCCESS;
  }
  if (local) {
    bzero((char *)&(PacListP->pacp->realm.uuid), sizeof(uuid_t));

    pwentP = getpwnam(userNameP);
    if (pwentP) {
        bzero((char *)&(PacListP->pacp->principal.uuid), sizeof(uuid_t));
	bcopy((char *)&(pwentP->pw_uid), 
		(char *)&(PacListP->pacp->principal.uuid), sizeof(pwentP->pw_uid));
    } else {
	printf("%s: no passwd file entry found for %s", routineName,userNameP);
	exit(1);
    }
    grentP = getgrnam(groupNameP);
    if (grentP) {
	bzero((char *)&(PacListP->pacp->group.uuid), sizeof(uuid_t));
	bcopy((char *)&(grentP->gr_gid), (char *)&(PacListP->pacp->group.uuid), 
			sizeof(grentP->gr_gid));
    } else {
	printf("%s: no passwd file entry found for %s", routineName,userNameP);
	exit(1);
    }
  } else {
    /* find out the uuid for the local realm */
    TRY
      exceptionRaised = 0;
      sec_id_parse_name(sec_rgy_default_handle,
			(idl_char *) "/.:/root",
			NULL,
			(uuid_t *)&realmUuid,
			NULL,
			(uuid_t *)NULL,
			(error_status_t *)&pgoError);
    CATCH_ALL
      PrintException(THIS_CATCH, routineName, "sec_id_parse_name", __FILE__, __LINE__);
      exceptionRaised = 1;
    ENDTRY;






    if (rtnVal != 0 || exceptionRaised) {
      printf("error finding realm, %x/exception %d\n",
	     pgoError, exceptionRaised);
      exit(-1);
    }
  
    PacListP->pacp->realm.uuid = realmUuid;
    if (!simple_id)
        PacListP->next->pacp->realm.uuid = realmUuid;
    
    /* get the uuid for the user's name */
    TRY
        exceptionRaised = 0;
        sec_rgy_pgo_name_to_id(sec_rgy_default_handle,
			       sec_rgy_domain_person,
			       (unsigned char *)userNameP,
			       &(PacListP->pacp->principal.uuid),
			       (error_status_t *)&pgoError);
    CATCH_ALL
	PrintException(THIS_CATCH, routineName, "sec_rgy_pgo_name_to_id",
		       __FILE__, __LINE__);
        exceptionRaised = 1;
    ENDTRY;

    if (pgoError != sec_rgy_status_ok || exceptionRaised) {
	printf("user name: pgoError is %x/exception %d\n",
	       pgoError, exceptionRaised);
	exit(1);
    }

    if (!simple_id) {
    /* get the uuid for the delegate user's name */
    TRY
        exceptionRaised = 0;
        sec_rgy_pgo_name_to_id(sec_rgy_default_handle,
			       sec_rgy_domain_person,
			       (unsigned char *)duserNameP,
			       &(PacListP->next->pacp->principal.uuid),
			       &pgoError);
    CATCH_ALL
	PrintException(THIS_CATCH, routineName, "sec_rgy_pgo_name_to_id",
		       __FILE__, __LINE__);
        exceptionRaised = 1;
    ENDTRY;

    if (pgoError != sec_rgy_status_ok || exceptionRaised) {
	printf("delegate user name: pgoError is %x/exception %d\n",
	       pgoError, exceptionRaised);
	exit(1);
    }
    }
    
    /* now, get the uuid for the user's group */
    TRY
	exceptionRaised = 0;
	sec_rgy_pgo_name_to_id(sec_rgy_default_handle,
				 sec_rgy_domain_group,
				 (unsigned char *)groupNameP,
				 &(PacListP->pacp->group.uuid),
				 (error_status_t *)&pgoError);
    CATCH_ALL
	PrintException(THIS_CATCH, routineName, "sec_rgy_pgo_name_to_id",
			 __FILE__, __LINE__);
	exceptionRaised = 1;
    ENDTRY;

    if (pgoError != sec_rgy_status_ok || exceptionRaised) {
	printf("group: pgoError is %x/exception %d\n",
	       pgoError, exceptionRaised);
	exit(1);
    }

    if (!simple_id) {
    /* now, get the uuid for the delegate's group */
    TRY
	exceptionRaised = 0;
	sec_rgy_pgo_name_to_id(sec_rgy_default_handle,
				 sec_rgy_domain_group,
				 (unsigned char *)dgroupNameP,
				 &(PacListP->next->pacp->group.uuid),
				 &pgoError);
    CATCH_ALL
	PrintException(THIS_CATCH, routineName, "sec_rgy_pgo_name_to_id",
			 __FILE__, __LINE__);
	exceptionRaised = 1;
    ENDTRY;

    if (pgoError != sec_rgy_status_ok || exceptionRaised) {
	printf("delegate group: pgoError is %x/exception %d\n",
	       pgoError, exceptionRaised);
	exit(1);
    }
    }
  }
  return DACL_SUCCESS;
}

EXPORT long dacl_AddLocalGroupToPac(pacP, groupNameP, local)
     sec_id_pa_t *	pacP;
     char * volatile		groupNameP;
     volatile int		local;
{
  volatile long			rtnVal = DACL_SUCCESS;
  volatile error_status_t	pgoError = error_status_ok;
  sec_id_t * volatile		newGroupSetP;	
  volatile uuid_t		theUuid;
  volatile int		exceptionRaised = 0;
  struct group *	grentP;
  static char		routineName[] = "dacl_AddLocalGroupToPac";
  
  /* XXX - we should check that the name passed is non-NULL */
  if (local) {
    grentP = getgrnam((char *)groupNameP);
    if (grentP) {
      bzero((char *)&theUuid, sizeof(uuid_t));
      bcopy((char *)&(grentP->gr_gid), (char *)&theUuid, 
					sizeof(grentP->gr_gid));
    } else {
      printf("%s: no group file entry found for %s", routineName, groupNameP);
      exit(1);
    }
  } else {
    TRY
      exceptionRaised = 0;
      sec_rgy_pgo_name_to_id(sec_rgy_default_handle,
			     sec_rgy_domain_group,
			     (unsigned char *)groupNameP,
			     (uuid_t *)&theUuid,
			     (error_status_t *)&pgoError);
    CATCH_ALL
      PrintException(THIS_CATCH, routineName, "sec_rgy_pgo_name_to_id",
		     __FILE__, __LINE__);
      exceptionRaised = 1;
    ENDTRY;
    if ((pgoError != sec_rgy_status_ok) || exceptionRaised) {
	printf("sec_rgy_pgo_name_to_id: %x, %s/exception %d\n",
	       pgoError, groupNameP, exceptionRaised);
	exit(1);
    }
  }
  
  newGroupSetP =(sec_id_t *)osi_Alloc((pacP->num_groups + 1)
				      * sizeof(sec_id_t));
  if (newGroupSetP == NULL) {
	printf("osi_Alloc failed\n");
	exit(1);
  }
  if (pacP->groups)
  	bcopy((char *)(pacP->groups), (char *)newGroupSetP,
	    pacP->num_groups * sizeof(sec_id_t));
  newGroupSetP[pacP->num_groups].uuid = theUuid;
  newGroupSetP[pacP->num_groups].name = (ndr_char *)NULL;
      
  if (pacP->groups)
  	osi_Free((char *)(pacP->groups), pacP->num_groups * sizeof(sec_id_t));
  pacP->groups = newGroupSetP;
  pacP->num_groups++;
  return DACL_SUCCESS;
}

/*
 * from dacl_modify.c
 */
EXPORT long dacl_InitAclEntryFromStrings(aclEntryP, defaultRealmUuidP,
					 typeStringP, nameStringP,
					 permStringP, dataStringP, mgrUuidP, local)
     dacl_entry_t *	aclEntryP;
     epi_uuid_t *	defaultRealmUuidP;
     char *		typeStringP;
     char * volatile	nameStringP;
     char *		permStringP;
     char *		dataStringP;
     epi_uuid_t *	mgrUuidP;
     int		local;
{
  volatile long			rtnVal = DACL_SUCCESS;
  volatile error_status_t		pgoError;
  sec_rgy_name_t		tempString;	 /* Used to hold nameStringP */
  uuid_t			principalUuid;
  uuid_t			cellUuid;
  boolean32			entryIsInDefaultRealm;
  volatile int			exceptionRaised;
  struct passwd *		pwentP;
  int				prinUid;
  struct group *		grentP;
  int				groupUid;
  static char			routineName[] = "dacl_InitAclEntryFromStrings";

  icl_Trace0(dacl_iclSetp, DACL_ICL_RGY_TRACE_3 );
  bzero((char *) &principalUuid, sizeof (uuid_t));
  rtnVal = dacl_EntryType_FromString(&(aclEntryP->entry_type), typeStringP);
  if (rtnVal == DACL_SUCCESS) {
    bzero((char *)&(aclEntryP->entry_info), sizeof(aclEntryP->entry_info));
    
    /* parameter checks and warning messages */
    if ((aclEntryP->entry_type != dacl_entry_type_user) &&
	(aclEntryP->entry_type != dacl_entry_type_group) &&
	(aclEntryP->entry_type != dacl_entry_type_foreign_user) &&
        (aclEntryP->entry_type != dacl_entry_type_foreign_user_delegate) &&
	(aclEntryP->entry_type != dacl_entry_type_foreign_group) &&
	(aclEntryP->entry_type != dacl_entry_type_foreign_group_delegate) &&
	(aclEntryP->entry_type != dacl_entry_type_foreign_other) &&
	(aclEntryP->entry_type != dacl_entry_type_foreign_other_delegate) &&
       	(aclEntryP->entry_type != dacl_entry_type_user_delegate) &&
       	(aclEntryP->entry_type != dacl_entry_type_group_delegate) &&
       	(aclEntryP->entry_type != dacl_entry_type_foreign_other_delegate) &&
	(nameStringP != (char *)NULL)) {
      icl_Trace2(dacl_iclSetp, DACL_ICL_RGY_WARNINGS_4 , ICL_TYPE_STRING, nameStringP, ICL_TYPE_STRING, typeStringP);
    }
    if ((aclEntryP->entry_type != dacl_entry_type_extended) &&
	(dataStringP != (char *)NULL)) {
      icl_Trace2(dacl_iclSetp, DACL_ICL_RGY_WARNINGS_5 , ICL_TYPE_STRING, dataStringP, ICL_TYPE_STRING, typeStringP);
    }
    
    if ((aclEntryP->entry_type == dacl_entry_type_user_obj) ||
        (aclEntryP->entry_type == dacl_entry_type_user_obj_delegate) ||
        (aclEntryP->entry_type == dacl_entry_type_group_obj_delegate) ||
	(aclEntryP->entry_type == dacl_entry_type_group_obj)) {
      if (dacl_AreObjectEntriesRequired(mgrUuidP) == 0) {
	icl_Trace2(dacl_iclSetp, DACL_ICL_RGY_NONE_9, ICL_TYPE_LONG, aclEntryP->entry_type, ICL_TYPE_STRING, dacl_AclMgrName(mgrUuidP));
	rtnVal = DACL_ERROR_ILLEGAL_ENTRY;
      }
    }
    /* map any uuids we have (that we are able to map) */
    else if ((aclEntryP->entry_type == dacl_entry_type_user) ||
             (aclEntryP->entry_type == dacl_entry_type_user_delegate) ||
	     (aclEntryP->entry_type == dacl_entry_type_foreign_other) ||
             (aclEntryP->entry_type == dacl_entry_type_foreign_other_delegate) ||
             (aclEntryP->entry_type == dacl_entry_type_foreign_user_delegate) ||
	     (aclEntryP->entry_type == dacl_entry_type_foreign_user)) {
      if (nameStringP != (char *)NULL) {
	if (local) {
	  if ((aclEntryP->entry_type == dacl_entry_type_user) ||
              (aclEntryP->entry_type == dacl_entry_type_user_delegate)) {
	    /* this is the only one of these types we can map locally */
	    pwentP = getpwnam((char *)nameStringP);
	    if (pwentP) {
	      /*
	       * we don't use the epi id macros here, because we don't know how big
	       * an epi is (we can find out, but it's hardly worthwhile
	       */
	      bzero((char *)(&(aclEntryP->entry_info.id)), sizeof(epi_uuid_t));
	      bcopy((char *)&(pwentP->pw_uid), (char *)(&(aclEntryP->entry_info.id)),
		    sizeof(pwentP->pw_uid));
	    }
	    else {
	      icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_TRACE_6 , ICL_TYPE_STRING, nameStringP);
	      rtnVal = DACL_ERROR_UNRECOGNIZED_USER_OR_GROUP;
	    }
	  }
	  else {
	    /* some sort of error message */
	    icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_TRACE_7 , ICL_TYPE_STRING, typeStringP);
	    rtnVal = DACL_ERROR_NONLOCAL_ENTRY_TYPE;
	  }
	}
	else {
	  if ((aclEntryP->entry_type == dacl_entry_type_foreign_other) ||
              (aclEntryP->entry_type == dacl_entry_type_foreign_other_delegate)) {
	    (void)sprintf((char *)tempString, "/.../%s/root", nameStringP);
	  }
	  else {
	    (void)strncpy((char *)tempString,
			(char *)nameStringP, sizeof(tempString) - 1);
	  }
            {
	     unsigned char * uuidstr = NULL;
                unsigned32 ustatus;

                fprintf(stderr,"Calling sec_id_parse_name: \n");
                fprintf(stderr,"name = %s\n", tempString);
                uuid_to_string(&principalUuid, &uuidstr, &ustatus);
                if (uuidstr)
                    fprintf(stderr,"principalUuid = %s \n", uuidstr);
                else
                    fprintf(stderr,"principalUuid is NULL\n");
	    }
	  TRY
	    exceptionRaised = 0;
	    sec_id_parse_name(sec_rgy_default_handle,
			      tempString,
			      NULL,
			      &cellUuid,
			      NULL,
			      &principalUuid,
			      (error_status_t *)&pgoError);
	  CATCH_ALL
	    PrintException(THIS_CATCH, routineName, "sec_id_parse_name",
			   __FILE__, __LINE__);
	    exceptionRaised = 1;
	  ENDTRY;
	  
	  if ((pgoError == error_status_ok) && !exceptionRaised) {
	  {
	     unsigned char * uuidstr;
	     unsigned32 ustatus;

	     fprintf(stderr,"sec_id_parse_name returns: \n");
	     uuid_to_string(&cellUuid, &uuidstr, &ustatus);
	     fprintf(stderr,"name = %s, cellUUid = %s\n", tempString, uuidstr);
	     uuid_to_string(&principalUuid, &uuidstr, &ustatus);
	     fprintf(stderr,"principalUuid = %s \n", uuidstr);
	  }
	  if ((aclEntryP->entry_type == dacl_entry_type_foreign_other) ||
	      (aclEntryP->entry_type == dacl_entry_type_foreign_other_delegate)){
	      /* copy the uuid into the appropriate place */
	      aclEntryP->entry_info.id = *((epi_uuid_t *) &principalUuid);
	  }
	  else {
	      /* now, determine whether or not the returned cell uuid is the default */
	      entryIsInDefaultRealm = uuid_equal((uuid_t *)defaultRealmUuidP,
						 &cellUuid, (error_status_t *)&pgoError);
	      if (pgoError == error_status_ok) {
		if (entryIsInDefaultRealm == true) {
		  if (aclEntryP->entry_type == dacl_entry_type_foreign_user) {
		    aclEntryP->entry_type = dacl_entry_type_user;
		  }
		  else if (aclEntryP->entry_type == dacl_entry_type_foreign_group) {
		    aclEntryP->entry_type = dacl_entry_type_group;
		  }else if (aclEntryP->entry_type == dacl_entry_type_foreign_user_delegate){
		    aclEntryP->entry_type = dacl_entry_type_user_delegate;
                  }else if (aclEntryP->entry_type == dacl_entry_type_foreign_group_delegate){
		    aclEntryP->entry_type = dacl_entry_type_group_delegate;		  
                  }
		  /* copy the uuid into the appropriate place */
		  aclEntryP->entry_info.id = *((epi_uuid_t *) &principalUuid);
		}
		else {
		  if (aclEntryP->entry_type == dacl_entry_type_user) {
		    aclEntryP->entry_type = dacl_entry_type_foreign_user;
		  }
		  else if (aclEntryP->entry_type == dacl_entry_type_group) {
		    aclEntryP->entry_type = dacl_entry_type_foreign_group;
		  }
		  else if (aclEntryP->entry_type == dacl_entry_type_user_delegate) {
		    aclEntryP->entry_type = dacl_entry_type_foreign_user_delegate;
		  }
		  else if (aclEntryP->entry_type == dacl_entry_type_group_delegate) {
		    aclEntryP->entry_type = dacl_entry_type_foreign_group_delegate;
		  }
		  
		  /* copy the uuids into the appropriate place */
		  aclEntryP->entry_info.foreign_id.id =
		    *((epi_uuid_t *) &principalUuid);
		  aclEntryP->entry_info.foreign_id.realm =
		    *((epi_uuid_t *) &cellUuid);
		}
	      }
	      else {
		/* error comparing uuids */
		icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_NONE_10 ,
			ICL_TYPE_STRING,
			RgySec_ErrorString((error_status_t *)&pgoError));
		rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
	      }
	    }
	  }
	  else {
	    if (exceptionRaised) {
	      icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_NONE_11 , ICL_TYPE_STRING, tempString);
	      rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
	    }
	    else {
	      icl_Trace2(dacl_iclSetp, DACL_ICL_RGY_NONE_12 , 
		      ICL_TYPE_STRING, tempString, ICL_TYPE_STRING, 
		      RgySec_ErrorString((error_status_t *)&pgoError));
	      if (pgoError == sec_rgy_object_not_found) {
		rtnVal = DACL_ERROR_UNRECOGNIZED_USER_OR_GROUP;
	      }
	      else {
		rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
	      }
	    }
	  }
	}
      }
      else {
	icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_NONE_13 , ICL_TYPE_STRING, typeStringP);
	rtnVal = DACL_ERROR_MISSING_NAME;
      }
    }
    else if ((aclEntryP->entry_type == dacl_entry_type_group) ||
	     (aclEntryP->entry_type == dacl_entry_type_foreign_group) ||
             (aclEntryP->entry_type == dacl_entry_type_group_delegate)||
             (aclEntryP->entry_type == dacl_entry_type_foreign_group_delegate)) {
      if (local) {
	if (nameStringP) {
	  if ((aclEntryP->entry_type == dacl_entry_type_group) ||
             (aclEntryP->entry_type == dacl_entry_type_group_delegate)){
	    /* this is the only one of these types we can map locally */
	    grentP = getgrnam((char *)nameStringP);
	    if (grentP) {
	      /* translate group id into uuid */
	      /*
	       * we don't use the epi id macros here, because we don't know how big
	       * an epi is (we can find out, but it's hardly worthwhile
	       */
	      bzero((char *)(&(aclEntryP->entry_info.id)), sizeof(epi_uuid_t));
	      bcopy((char *)&(grentP->gr_gid), (char *)(&(aclEntryP->entry_info.id)),
		    sizeof(grentP->gr_gid));
	    }
	    else {
	      icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_TRACE_8 , ICL_TYPE_STRING, nameStringP);
	      rtnVal = DACL_ERROR_UNRECOGNIZED_USER_OR_GROUP;
	    }
	  }
	  else {
	    icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_TRACE_9 , ICL_TYPE_STRING, typeStringP);
	    rtnVal = DACL_ERROR_NONLOCAL_ENTRY_TYPE;
	  }
	}
	else {
	  icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_NONE_14 , ICL_TYPE_STRING, typeStringP);
	  rtnVal = DACL_ERROR_MISSING_NAME;
	}
      }
      else {
	if (nameStringP != (char *)NULL) {
	    (void)strncpy((char *)tempString, (char *)nameStringP, 
		sizeof(tempString) - 1);
	  TRY
	    exceptionRaised = 0;
	    sec_id_parse_group(sec_rgy_default_handle,
			       tempString,
			       NULL,
			       &cellUuid,
			       NULL,
			       &principalUuid,
			       (error_status_t *)&pgoError);
	  CATCH_ALL
	    PrintException(THIS_CATCH, routineName, "sec_id_parse_group",
			   __FILE__, __LINE__);
	    exceptionRaised = 1;
	  ENDTRY;
	  
	  if ((pgoError == error_status_ok) && !exceptionRaised) {
	    if ((aclEntryP->entry_type == dacl_entry_type_foreign_other)||
                (aclEntryP->entry_type == dacl_entry_type_foreign_other_delegate)) {
	      /* copy the uuid into the appropriate place */
	      aclEntryP->entry_info.id = *((epi_uuid_t *) &principalUuid);
	    }
	    else {
	      /* now, determine whether or not the returned cell uuid is the default */
	      entryIsInDefaultRealm = uuid_equal((uuid_t *)defaultRealmUuidP,
						 &cellUuid, (error_status_t *)&pgoError);
	      if (pgoError == error_status_ok) {
		if (entryIsInDefaultRealm == true) {
		  if (aclEntryP->entry_type == dacl_entry_type_foreign_user) {
		    aclEntryP->entry_type = dacl_entry_type_user;
		  }else if (aclEntryP->entry_type == dacl_entry_type_foreign_user_delegate){
                    aclEntryP->entry_type = dacl_entry_type_user_delegate;
                  }
		  
		  /* copy the uuid into the appropriate place */
		  aclEntryP->entry_info.id = *((epi_uuid_t *) &principalUuid);
		}
		else {
		  if (aclEntryP->entry_type == dacl_entry_type_user) {
		    aclEntryP->entry_type = dacl_entry_type_foreign_user;
		  }
		  else if (aclEntryP->entry_type == dacl_entry_type_user_delegate){
		    aclEntryP->entry_type = dacl_entry_type_foreign_user_delegate;
                  } 
		  /* copy the uuids into the appropriate place */
		  aclEntryP->entry_info.foreign_id.id =
		    *((epi_uuid_t *) &principalUuid);
		  aclEntryP->entry_info.foreign_id.realm =
		    *((epi_uuid_t *) &cellUuid);
		}
	      }
	      else {
		/* error comparing uuids */
		icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_NONE_15 , ICL_TYPE_STRING,
			RgySec_ErrorString((error_status_t *)&pgoError));
		rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
	      }
	    }
	  }
	  else {
	    if (exceptionRaised) {
	      icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_NONE_16 , ICL_TYPE_STRING, nameStringP);
	    }
	    else {
	      icl_Trace2(dacl_iclSetp, DACL_ICL_RGY_NONE_17,
			 ICL_TYPE_STRING, nameStringP,
			 ICL_TYPE_STRING,
			 RgySec_ErrorString((error_status_t *)&pgoError));
	    }
	    rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
	  }
	}
	else {
	  icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_NONE_18 , ICL_TYPE_STRING, typeStringP);
	  rtnVal = DACL_ERROR_MISSING_NAME;
	}
      }
    }
    
    else if (aclEntryP->entry_type == dacl_entry_type_extended) {
      bzero((char *)&(aclEntryP->entry_info.extended_info.extensionType),
	    sizeof(epi_uuid_t));
      bzero((char *)&(aclEntryP->entry_info.extended_info.formatLabel),
	    sizeof(dacl_format_label_t));
      aclEntryP->entry_info.extended_info.numberBytes = strlen(dataStringP);
      aclEntryP->entry_info.extended_info.infoP = 
	(char *)osi_Alloc(aclEntryP->entry_info.extended_info.numberBytes);
      bcopy(dataStringP, aclEntryP->entry_info.extended_info.infoP,
	    aclEntryP->entry_info.extended_info.numberBytes);
    }

    if (rtnVal == DACL_SUCCESS) {
      /* if everything has worked so far ... */
      dacl_Permset_FromString(&(aclEntryP->perms), permStringP);
    }
  }	/* end if we got a valid entry type from the type field */
  
  icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_TRACE_10 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));

  return rtnVal;
}

EXPORT long dacl_NameAndTypeStringsFromEntry(aclEntryP, defaultRealmUuidP, typeStringPP,
					     globalName, local)
     dacl_entry_t *	aclEntryP;
     epi_uuid_t *	defaultRealmUuidP;
     char **		typeStringPP;
     sec_rgy_name_t	globalName;
     int		local;
{
  volatile long		rtnVal = DACL_SUCCESS;
  sec_rgy_name_t	objectName;
  sec_rgy_domain_t	objectDomain;
  char *		typeString;
  volatile error_status_t	pgoError;
  uuid_t		safePrincipalUuid;
  uuid_t *		principalUuidP;
  uuid_t *		realmUuidP;
  uuid_t		nullUuid;
  sec_rgy_name_t	principalName;
  sec_rgy_name_t	cellName;
  volatile int		exceptionRaised;
  struct passwd *	pwentP;
  struct group *	grentP;
  int			prinUid = -2;
  int			groupId = -2;
  unsigned char	*	nameUuidStringP;
  unsigned32		uuidError;
  
  long			unixId;
  uuid_t		realPrincipalUuid;
  
  sec_rgy_handle_t	secRgyHandle;
  char			secRoutineName[BUFSIZ];
  static char		routineName[] = "dacl_NameAndTypeStringsFromEntry";

  icl_Trace0(dacl_iclSetp, DACL_ICL_RGY_TRACE_11 );

  /* dacl_EntryType_ToString returns a static pointer */
  *typeStringPP = dacl_EntryType_ToString(aclEntryP->entry_type);

  /* map any uuids we have (that we are able to map) */
  if ((aclEntryP->entry_type == dacl_entry_type_user) ||
      (aclEntryP->entry_type == dacl_entry_type_user_delegate) ||
      (aclEntryP->entry_type == dacl_entry_type_group) ||
      (aclEntryP->entry_type == dacl_entry_type_group_delegate) ||
      (aclEntryP->entry_type == dacl_entry_type_foreign_user) ||
      (aclEntryP->entry_type == dacl_entry_type_foreign_user_delegate) ||
      (aclEntryP->entry_type == dacl_entry_type_foreign_group) ||
      (aclEntryP->entry_type == dacl_entry_type_foreign_group_delegate)) {

    if (!local) {
      /* first, get the name of the cell - useful in a number of situations */
      strcpy((char *)cellName, "");

      /*
       * We ignore the error, since the uuid_create_nil is only supposed
       * to return success.
       */
      uuid_create_nil(&nullUuid, &uuidError);

      /* pick up the right realm and principal uuids */
      
      if ((aclEntryP->entry_type == dacl_entry_type_user) ||
          (aclEntryP->entry_type == dacl_entry_type_user_delegate) ||
	  (aclEntryP->entry_type == dacl_entry_type_group) ||
          (aclEntryP->entry_type == dacl_entry_type_group_delegate)){
	principalUuidP = (uuid_t *)&(aclEntryP->entry_info.id);
	realmUuidP = (uuid_t *)defaultRealmUuidP;
      }
      else {
	principalUuidP = (uuid_t *)&(aclEntryP->entry_info.foreign_id.id);
	realmUuidP = (uuid_t *)&(aclEntryP->entry_info.foreign_id.realm);
      }

      strncpy(secRoutineName, "sec_id_gen_name", sizeof(secRoutineName));
      sec_id_gen_name(sec_rgy_default_handle,
		      realmUuidP,
		      &nullUuid,
		      (void *)NULL,
		      cellName,
		      (void *)NULL,
		      (error_status_t *)&pgoError);

      if (pgoError != error_status_ok) {
	icl_Trace2(dacl_iclSetp, DACL_ICL_RGY_NONE_19,
		   ICL_TYPE_STRING, secRoutineName,
		   ICL_TYPE_STRING,
		   RgySec_ErrorString((error_status_t *)&pgoError));
	/* can't find it, just translate it into a string */
	uuid_to_string(realmUuidP, &nameUuidStringP, &uuidError);
	if (uuidError == error_status_ok) {
	  strncpy((char *)cellName, (char *)nameUuidStringP, sizeof(sec_rgy_name_t));
	  rpc_string_free(&nameUuidStringP, &uuidError);

	  /* don't bother trying to translate the rest */
	  rtnVal = DACL_ERROR_UNRECOGNIZED_USER_OR_GROUP;
	  rpc_string_free(&nameUuidStringP, &uuidError);
	}
	else {
	  rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
	}
      }

      if ((rtnVal == DACL_SUCCESS) && (sizeof(epi_principal_id_t) < sizeof(epi_uuid_t))) {
	/* in this case, we have to explicitly bind to the foreign site */
	sec_rgy_site_open(cellName,
			  &secRgyHandle,
			  (error_status_t *)&pgoError);
	      
	if (pgoError == error_status_ok) {
	  Epi_PrinId_FromUuid((epi_principal_id_t *)&unixId, principalUuidP);
	  TRY
	    exceptionRaised = 0;
	    sec_rgy_pgo_unix_num_to_id(secRgyHandle,
				       ((aclEntryP->entry_type == dacl_entry_type_user) ?
					sec_rgy_domain_person : sec_rgy_domain_group),
				       unixId,
				       &realPrincipalUuid,
				       (error_status_t *)&pgoError);
	  
	  CATCH_ALL
	    PrintException(THIS_CATCH, routineName,
			   "sec_rgy_pgo_unix_num_to_id",
			   __FILE__, __LINE__);
	    exceptionRaised = 1;
	  ENDTRY;
	  
	  if ((pgoError == error_status_ok) && !exceptionRaised) {
	    principalUuidP = &realPrincipalUuid;
	  }
	  else {
	    if (exceptionRaised) {
	      icl_Trace0(dacl_iclSetp, DACL_ICL_RGY_NONE_20 );
	    }
	    else {
	      icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_NONE_21,
		ICL_TYPE_STRING, 
		RgySec_ErrorString((error_status_t *)&pgoError));
	    }
	    
	    rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
	  }
	}
	else {
	  icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_NONE_22 ,
		   ICL_TYPE_STRING,
		   RgySec_ErrorString((error_status_t *)&pgoError));
	  rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
	}
      }
    }

    if (rtnVal == DACL_SUCCESS) {
      if ((aclEntryP->entry_type == dacl_entry_type_user) ||
          (aclEntryP->entry_type == dacl_entry_type_user_delegate) ||
          (aclEntryP->entry_type == dacl_entry_type_foreign_user_delegate) ||
	  (aclEntryP->entry_type == dacl_entry_type_foreign_user)) {
	if (local) {
	  if ((aclEntryP->entry_type == dacl_entry_type_user) ||
              (aclEntryP->entry_type == dacl_entry_type_user_delegate)) {
	    bcopy((char *)(&(aclEntryP->entry_info.id)), (char *)&prinUid, sizeof(int));
	    pwentP = getpwuid(prinUid);
	    if (pwentP) {
	      strncpy((char *)globalName, pwentP->pw_name, sizeof(sec_rgy_name_t));
	    }
	    else {
	      icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_TRACE_12 , ICL_TYPE_LONG, prinUid);
	      rtnVal = DACL_ERROR_UNRECOGNIZED_USER_OR_GROUP;
	    }
	  }
	  else {
	    icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_TRACE_13 , ICL_TYPE_LONG, *typeStringPP);
	    rtnVal = DACL_ERROR_NONLOCAL_ENTRY_TYPE;
	  }
	}
	else {
	  TRY
	    exceptionRaised = 0;
  	    sec_id_gen_name(sec_rgy_default_handle,
			    realmUuidP,
			    principalUuidP,
			    globalName,
			    cellName,
			    principalName,
			    (error_status_t *)&pgoError);
	  CATCH_ALL
	    PrintException(THIS_CATCH, routineName, "sec_id_gen_name",
			   __FILE__, __LINE__);
            exceptionRaised = 1;
	  ENDTRY;
	    
	  if ((pgoError == sec_rgy_status_ok) && !exceptionRaised) {
	    if (aclEntryP->entry_type == dacl_entry_type_user) {
	      /* only use the principal name within the local realm */
	      (void)strncpy((char *)globalName,(char *) principalName, 
		           sizeof(sec_rgy_name_t));
	    }
	  }
	  else {
	    if (exceptionRaised) {
	      icl_Trace0(dacl_iclSetp, DACL_ICL_RGY_NONE_23 );
	      rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
	    }
	    else {
	      icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_NONE_24,
		       ICL_TYPE_STRING,
		       RgySec_ErrorString((error_status_t *)&pgoError));
	      if (pgoError == sec_rgy_object_not_found) {
		rtnVal = DACL_ERROR_UNRECOGNIZED_USER_OR_GROUP;
	      }
	      else {
		rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
	      }
	    }
	  }
	}
      }
      else if ((aclEntryP->entry_type == dacl_entry_type_group) ||
               (aclEntryP->entry_type == dacl_entry_type_group_delegate)||
               (aclEntryP->entry_type == dacl_entry_type_foreign_group_delegate)||
	       (aclEntryP->entry_type == dacl_entry_type_foreign_group)) {
	if (local) {
	  if ((aclEntryP->entry_type == dacl_entry_type_group)||
               (aclEntryP->entry_type == dacl_entry_type_group_delegate)) {
	    bcopy((char *)(&(aclEntryP->entry_info.id)), (char *)&groupId, sizeof(int));
	    grentP = getgrgid(groupId);
	    if (grentP) {
	      strncpy((char *)globalName, grentP->gr_name, sizeof(sec_rgy_name_t));
	    }
	    else {
	      icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_TRACE_14 , ICL_TYPE_LONG, groupId);
	      rtnVal = DACL_ERROR_UNRECOGNIZED_USER_OR_GROUP;
	    }
	  }
	  else {
	    icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_TRACE_15 , ICL_TYPE_STRING, *typeStringPP);
	    rtnVal = DACL_ERROR_NONLOCAL_ENTRY_TYPE;
	  }
	}
	else {
	  TRY
	    exceptionRaised = 0;
            sec_id_gen_group(sec_rgy_default_handle,
			     realmUuidP,
			     principalUuidP,
			     globalName,
			     cellName,
			     principalName,
			     (error_status_t *)&pgoError);
	  CATCH_ALL
	    PrintException(THIS_CATCH, routineName, "sec_id_gen_group",
			   __FILE__, __LINE__);
            exceptionRaised = 1;
	  ENDTRY;
	  
	  if ((pgoError == error_status_ok) && !exceptionRaised) {
	    if (aclEntryP->entry_type == dacl_entry_type_group) {
	      /* only use the principal name within the local realm */
	      (void)strncpy((char *)globalName, (char *)principalName, 
		            sizeof(sec_rgy_name_t));
	    }
	  }
	  else {
	    if (exceptionRaised) {
	      icl_Trace0(dacl_iclSetp, DACL_ICL_RGY_NONE_25 );
	      rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
	    }
	    else {
	      icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_NONE_26, ICL_TYPE_STRING,
		      RgySec_ErrorString((error_status_t *)&pgoError));
	      if (pgoError == sec_rgy_object_not_found) {
		rtnVal = DACL_ERROR_UNRECOGNIZED_USER_OR_GROUP;
	      }
	      else {
		rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
	      }
	    }
	  }
	}
      }
    }

    if (rtnVal == DACL_ERROR_UNRECOGNIZED_USER_OR_GROUP) {
      /*
       * Generate the uuid/uid string, in case the client might be able to make
       * use of it.
       */
      
      if (local) {
	sprintf((char *)globalName, "%d",
		(aclEntryP->entry_type == dacl_entry_type_user) ? prinUid : groupId);
      }
      else {
	strncpy((char *)globalName, "", sizeof(sec_rgy_name_t));
	
	uuid_to_string(principalUuidP, &nameUuidStringP, &uuidError);
	if (uuidError == error_status_ok) {
	  strncpy((char *)globalName, (char *)nameUuidStringP, sizeof(sec_rgy_name_t));
	  rpc_string_free(&nameUuidStringP, &uuidError);
	  /*
	   *  We ignore the error, since there isn't much we can do if the rpc allocates
	   * a string (which is where uuid_to_string allocates its string), but can't
	   * free it.
	   */

	  if ((aclEntryP->entry_type == dacl_entry_type_foreign_user) ||
              (aclEntryP->entry_type == dacl_entry_type_foreign_user_delegate) ||
              (aclEntryP->entry_type == dacl_entry_type_foreign_group_delegate) ||
	      (aclEntryP->entry_type == dacl_entry_type_foreign_group)) {
	    /*
	     *  At this point, cellName is something meaningful, or we would
	     * have jumped around all this code.
	     */
	    strncat((char *)globalName, "@",
		    sizeof(sec_rgy_name_t) - strlen((char *)globalName));
	    strncat((char *)globalName, (char *)cellName,
		    sizeof(sec_rgy_name_t) - strlen((char *)globalName));
	  }
	}
      }
    }
		
  }
  else if ((aclEntryP->entry_type == dacl_entry_type_foreign_other)||
           (aclEntryP->entry_type == dacl_entry_type_foreign_other_delegate)) {
    /*
     * This has to be separated out, since we must explicitly tell sec_id_gen_name to
     * ignore the principal uuid.
     */
    bzero((char *)&safePrincipalUuid, sizeof(uuid_t));

    TRY
      exceptionRaised = 0;
      sec_id_gen_name(sec_rgy_default_handle,
		      (uuid_t *)&(aclEntryP->entry_info.id),
		      &safePrincipalUuid,
		      NULL,
		      cellName,
		      NULL,
		      (error_status_t *)&pgoError);
    CATCH_ALL
      PrintException(THIS_CATCH, routineName, "sec_id_gen_name", __FILE__, __LINE__);
      exceptionRaised = 1;
    ENDTRY

    if ((pgoError == sec_rgy_status_ok) && !exceptionRaised) {
      sprintf((char *)globalName, "%s/<auth_user>", cellName);
    }
    else {
      if (exceptionRaised) {
	icl_Trace0(dacl_iclSetp, DACL_ICL_RGY_NONE_27 );
      }
      else {
	icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_NONE_28, ICL_TYPE_STRING,
		RgySec_ErrorString((error_status_t *)&pgoError));
      }
      rtnVal = DACL_ERROR_SEC_RGY_PGO_ERROR;
    }
  }

#if defined(KERNEL)
  icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_TRACE_16 , ICL_TYPE_LONG, rtnVal);
#else /* defined(KERNEL) */
  icl_Trace1(dacl_iclSetp, DACL_ICL_RGY_TRACE_17 , ICL_TYPE_STRING, dfs_dceErrTxt(rtnVal));
#endif /* defined(KERNEL) */

  return rtnVal;
}
