/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dpeaclaccess.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:00  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:26:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:14  root]
 * 
 * Revision 1.1.4.3  1994/08/25  19:30:46  proulx
 * 	Delegation support.
 * 	[1994/08/25  19:18:15  proulx]
 * 
 * Revision 1.1.4.2  1994/06/30  19:17:46  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:03:46  mccann]
 * 
 * Revision 1.1.4.1  1994/03/12  22:08:17  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:33:57  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:16:49  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:16:16  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1993-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 */
#if !defined(lint) && !defined(SABER)
static char *SccsID = "@(#)dpeaclaccess.c\t4\t(DECdns)\t12/11/1991";
#endif	/* neither lint nor saber-C */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/aclif.h>
#include <dce/aclbase.h>			/* QWERTY Change this */
#include <dpeacl.h>				/* QWERTY Change this */


/* masked_access
**
** Private macro to mask entry permset against mask_obj if necessary and return
** the result of that operation.  Note that this macro was ultimately derived
** from the DEC Security private routine "access_granted" and should track the
** intended functionality of that routine.  Note that the preprocessor name
** DPE_ALLOW_MASK_OBJ_ACLE is required to use MASK_OBJ. Note also that the
** choice of ACLE types in the lists below is based on the definition of
** MASK_OBJ and not on the DPE application requirements.
*/
#ifdef DPE_ALLOW_MASK_OBJ_ACLE

static long SAET_use_no_mask_obj =
		1 << sec_acl_e_type_user_obj | 1 << sec_acl_e_type_other_obj |
		1 << sec_acl_e_type_mask_obj | 1 << sec_acl_e_type_unauthenticated;
static long SAET_use_mask_obj =
		1 << sec_acl_e_type_group_obj | 1 << sec_acl_e_type_any_other |
		1 << sec_acl_e_type_user | 1 << sec_acl_e_type_group |
		1 << sec_acl_e_type_foreign_other | 1 << sec_acl_e_type_foreign_user |
		1 << sec_acl_e_type_foreign_group | 1 << sec_acl_e_type_extended;
#define masked_access( entry, mask_perms, granted ) \
	( granted) = ( \
	    ( 1 << (int) ( entry.entry_info.entry_type) & SAET_use_no_mask_obj) \
	    ?   ( entry.perms) \
	    :   ( 1 << (int) ( entry.entry_info.entry_type) & SAET_use_mask_obj) \
		?   ((entry.perms) & mask_perms) \
		:   NO_PERMISSIONS)
#else

static long SAET_no_mask_obj =
		1 << sec_acl_e_type_user_obj | 1 << sec_acl_e_type_other_obj |
		1 << sec_acl_e_type_unauthenticated |
		1 << sec_acl_e_type_group_obj | 1 << sec_acl_e_type_any_other |
		1 << sec_acl_e_type_user | 1 << sec_acl_e_type_group |
		1 << sec_acl_e_type_foreign_other | 1 << sec_acl_e_type_foreign_user |
		1 << sec_acl_e_type_foreign_group | 1 << sec_acl_e_type_extended;
#define masked_access( entry, mask_perms, granted ) \
	( granted) = ( \
	    ( 1 << (int) ( entry.entry_info.entry_type) & SAET_no_mask_obj) \
	    ?   ( entry.perms) \
	    :   NO_PERMISSIONS)
#endif


/* Private macro to compare foreign realm with accessor's realm */
#define foreign_realm_eq( entry) \
		uuid_equal(	&accessor_info->realm.uuid, \
				&sec_acl_p->sec_acl_entries[entry].entry_info.tagged_union.foreign_id.realm.uuid, \
				&st)

#define MAX_PERMISSIONS	( ~ (sec_acl_permset_t) 0)	/* ALL permissions. */
#define ENTRY_NOT_FOUND -1		/* Indicates no acl entry found */


/*
** d p e _ a c l _ m g r _ g e t _ a c c e s s
**
** Determine a principle's "complete extent of access" to some object.  This is
** useful for implementing operations like the conventional UNIX access
** function.  This routine implements a compatible subset of the DCE ACLs as
** required for DPE.
**
** The principle is represented by its PAC (accessor_info) and the object is
** represented by its ACL (sec_acl_p).  The principle output parameter
** (access_extent) is a mask containing a 1 for each permission granted by
** first ACL Entry category (see below) of the ACL which the PAC matches, and
** 0s elsewhere.  Note that the PAC may be NULL or unauthenticated.
**
** NORMALLY RETURNS TRUE, even when the access permissions are determined to be
** all 0 (NO_PERMISSIONS).  Returns false only on illogical error conditions
** (such as unsupported ACL Entry types), in which case the status output
** (st_p) gets the error status code and the permission output (access_extent)
** is set to NO_PERMISSIONS.
**
** The following ACL Entry types (sec_acl_e_type_*) are supported by this
** routine and are in categories checked in the order shown below:
**
**	user,foreign_user	At most one match
**	group,foreign_group	Union of all permissions for each matching group
**	other_obj		Matching local realm accesses
**	foreign_other		At most one can match
**	any_other		Catch-all
**	unauthenticated		(Note 1)
**      user_obj_delegate
**      user_delegate
**      foreign_user_delegate
**      group_obj_delegate
**      group_delegate
**      foreign_group_delegate
**      other_delegate
**      foreign_other_delegate
**      any_other_delegate
**      extended_delegate
**
** Note 1: sec_acl_e_type_unauthenticated is a mask used for all matches on
** unauthenticated PACs.  It is also intersected with sec_acl_e_type_any_other
** for NULL PACs.
**
** NB:	Note that this routine does not care at all about the meaning of any
**	particular permission bit.
**
** IMPLEMENTATION NOTES:
**
** The preprocessor variable DPE_ALLOW_MASK_OBJ_ACLE, when defined, will enable
** support for ACL Entry type sec_acl_e_type_mask_obj, otherwise such entries
** are considered invalid.
**
** The preprocessor variable DEBUG_dpe_acl_mgr_get_access is used for debugging
** purposes only, and in conjunction with the module gat.c.  That module also
** implements DEBUG_camga_print_now, also used for debuging
**
*/

PUBLIC boolean32 
dpe_acl_mgr_get_access (sec_acl_t               *acl_p,
			rpc_authz_cred_handle_t *cred_h_p,
                        sec_acl_permset_t       *access_extent_p, 
                        error_status_t          *st_p)
{
  sec_cred_pa_handle_t	*cred_pa;
  sec_id_pa_t *pa_p;
  boolean32   response = TRUE; /* initialize to a good response */
  uuid_t      *principal_p, *group_p;
  /* Dummy uuid - to spoof ACL permset algorithm */
  uuid_t       dummy_id = { /* 0021fade-0a6f-1dbc-89e8-0000c09ce054 */
    0x0021fade, 0x0a6f, 0x1dbc, 0x89, 0xe8, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54} };

  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_general,
		 svc_c_debug6,
		 " >dpe_acl_mgr_get_access()"));

  if (dce_acl_is_unauthenticated (*cred_h_p)) {
    /*
     * If we have no credentials, use dummy group and principal uuids
     */  
    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_general,
		   svc_c_debug6,
		   "Unauthenticated."));
    principal_p = &dummy_id;
    group_p = &dummy_id;
  }
  else {
    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_general,
		   svc_c_debug6,
		   "Authenticated."));
    /* 
     * Find who the caller is - we need the prin/group in case there are
     * user_obj and group_obj ACLEs.  Can't use the provided 
     * dce_inq_principal_and_group() because we don't have the client handle
     * to input. 
     */
    cred_pa = sec_cred_get_initiator(*cred_h_p, st_p);
    if (*st_p != error_status_ok) {
      response = FALSE;
    }
    else {
      pa_p = sec_cred_get_pa_data(cred_pa, st_p);
      if (*st_p != error_status_ok) {
	response = FALSE;
      }
      else {
	principal_p = &pa_p->principal.uuid;
	group_p = &pa_p->group.uuid;
      }
    }
  }
  
  /* 
   * Compare ACL with PAC, determining permissions. 
   */
  if (response) {
    dce_acl_inq_permset_for_creds(*cred_h_p, acl_p,
				  principal_p, group_p,
				  sec_acl_posix_no_semantics, access_extent_p, 
				  st_p);
    if (*st_p != error_status_ok || !*access_extent_p) { 
      *st_p = sec_acl_object_not_found;
      response = FALSE;
    }
  }
    
  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_general,
		 svc_c_debug2,
		 " <dpe_acl_mgr_get_access: return %d, %d",response,*st_p));
  
  return (response);

} /* End dpe_acl_mgr_get_access */





/*
** d p e _ a c l _ m g r _ g e t _ a c c e s s_v0
**
** Determine a principle's "complete extent of access" to some object.  This is
** useful for implementing operations like the conventional UNIX access
** function.  This routine implements a compatible subset of the DCE ACLs as
** required for DPE.
**
** The principle is represented by its PAC (accessor_info) and the object is
** represented by its ACL (sec_acl_p).  The principle output parameter
** (access_extent) is a mask containing a 1 for each permission granted by
** first ACL Entry category (see below) of the ACL which the PAC matches, and
** 0s elsewhere.  Note that the PAC may be NULL or unauthenticated.
**
** NORMALLY RETURNS TRUE, even when the access permissions are determined to be
** all 0 (NO_PERMISSIONS).  Returns false only on illogical error conditions
** (such as unsupported ACL Entry types), in which case the status output
** (st_p) gets the error status code and the permission output (access_extent)
** is set to NO_PERMISSIONS.
**
** The following ACL Entry types (sec_acl_e_type_*) are supported by this
** routine and are in categories checked in the order shown below:
**
**	user,foreign_user	At most one match
**	group,foreign_group	Union of all permissions for each matching group
**	other_obj		Matching local realm accesses
**	foreign_other		At most one can match
**	any_other		Catch-all
**	unauthenticated		(Note 1)
**
**      *_deleg                 Ignored
**
** Note 1: sec_acl_e_type_unauthenticated is a mask used for all matches on
** unauthenticated PACs.  It is also intersected with sec_acl_e_type_any_other
** for NULL PACs.
**
** NB:	Note that this routine does not care at all about the meaning of any
**	particular permission bit.
**
** IMPLEMENTATION NOTES:
**
** The preprocessor variable DPE_ALLOW_MASK_OBJ_ACLE, when defined, will enable
** support for ACL Entry type sec_acl_e_type_mask_obj, otherwise such entries
** are considered invalid.
**
** The preprocessor variable DEBUG_dpe_acl_mgr_get_access_v0 is used for debugging
** purposes only, and in conjunction with the module gat.c.  That module also
** implements DEBUG_camga_print_now, also used for debuging
**
** Note that this routine does virtually no error checking in the interest of
** speed. 
*/

PUBLIC boolean32 
dpe_acl_mgr_get_access_v0 (sec_acl_t          *sec_acl_p,
                        sec_id_pac_t       *accessor_info,
                        sec_acl_permset_t  *access_extent,
                        error_status_t     *st_p)
{								/* Top of dpe_acl_mgr_get_access_v0 */


    int                 i,j;            /* For traversing entry list. */
    int                 user_entry,     /* For keeping track of entries... */
                        foreign_user_entry,
                        other_obj_entry,
                        foreign_other_entry,
			any_other_entry,
                        unauth_entry;
    sec_id_t            *group_id, *realm_id;
    error_status_t      st;
    sec_acl_permset_t   mask_perms, group_access;
    boolean32           chk_loc_groups = false,
			in_default_realm,
			one_group_found = false;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >dpe_acl_mgr_get_access_v0: initialise access=NO_PERMISSIONS"));

    CLEAR_STATUS(st_p);						/* Assume no illogical errors */
    *access_extent = NO_PERMISSIONS;				/* Assume no permissions at all */

    /* Only 1 of each type of entry could possibly match this
     *  principal id, so keep a running tab on if/where each
     *  type of entry is found in the list
     */
    user_entry = foreign_user_entry = 
    other_obj_entry = foreign_other_entry = unauth_entry = any_other_entry =
	ENTRY_NOT_FOUND;

    /* If the mask isn't explicitly set, then it should have no effect
        when masking, so turn all perm bits on by default */
    mask_perms = MAX_PERMISSIONS;

/*
** Here we filter out NULL PACs.  A lot of code in sections following this one
** assume we have a PAC.  Rather than use active code all over the place we put
** a single filtering IF here which completely handles NULL PACs.  Saves time.
*/
    if ( accessor_info == NULL)
    {

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug5,
            "dpe_acl_mgr_get_access_v0:  (NULL PAC, last ACLE = %2d)  ===>", 
	    sec_acl_p->num_entries - 1)); 

	for (i = 0; i < sec_acl_p->num_entries; i++)
	{   switch(sec_acl_p->sec_acl_entries[i].entry_info.entry_type)
	    {	case sec_acl_e_type_any_other:
		    any_other_entry = i;
		    break;
		case sec_acl_e_type_unauthenticated:
		    unauth_entry = i;
		    break;
		case sec_acl_e_type_other_obj:
		    /* This is here to be consistant with secd. 
		     * For a null pac, other_obj is used without comparing
		     * the realm uuid 
		     */
		    other_obj_entry = i;
		    break;
#ifdef DPE_ALLOW_MASK_OBJ_ACLE
		case sec_acl_e_type_mask_obj:
		    mask_perms = sec_acl_p->sec_acl_entries[i].perms;
		    break;
#endif
		default:
		    /* Ignore these entrees */
		    break;
		};						/* End switch on acl entry_type */
	};							/* End for num_entries */
	if ( ( other_obj_entry != ENTRY_NOT_FOUND) &&
	     ( unauth_entry != ENTRY_NOT_FOUND) )
	{    sec_acl_permset_t tmp1, tmp2;
	     masked_access( sec_acl_p->sec_acl_entries[ other_obj_entry],
			    mask_perms, tmp1);
	     masked_access( sec_acl_p->sec_acl_entries[ unauth_entry],
			    mask_perms, tmp2);
	     *access_extent = tmp1 & tmp2; 

	     DCE_SVC_DEBUG((
                 cds__svc_handle,
		 cds_svc_library,
		 svc_c_debug5,
		 "  dpe_acl_mgr_is_authorized: (8) access_extent = 0x%x",
                 *access_extent));
	 }
	else if ( ( any_other_entry != ENTRY_NOT_FOUND) &&
	     ( unauth_entry != ENTRY_NOT_FOUND) )
	{    sec_acl_permset_t tmp1, tmp2;
	     masked_access( sec_acl_p->sec_acl_entries[ any_other_entry],
			    mask_perms, tmp1);
	     masked_access( sec_acl_p->sec_acl_entries[ unauth_entry],
			    mask_perms, tmp2);
	     *access_extent = tmp1 & tmp2; 
 
             DCE_SVC_DEBUG((
                 cds__svc_handle,
		 cds_svc_library,
		 svc_c_debug5,
		 "  dpe_acl_mgr_is_authorized: (7) access_extent = 0x%x",
                 *access_extent));

        }
	else {
	    *access_extent = NO_PERMISSIONS;

	    DCE_SVC_DEBUG((
                 cds__svc_handle,
		 cds_svc_library,
		 svc_c_debug5,
		 "  dpe_acl_mgr_is_authorized: (6) access_extent = 0x%x (==NO_PERMISSIONS !!)",
                 *access_extent));
	}
	
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_library,
            svc_c_debug2,
            " <dpe_acl_mgr_get_access_v0: (1) return true"));

	return true; 
    }					/* End if NULL PAC then */

/* Here if we have a non-NULL PAC.  PRE-PROCESS the acl entries so we only have
** to loop through once looking for specific types of entries. 
**
** Note, the accessor may be a member of multiple {foreign_}groups.  Therefore,
** the *group checks must be done below, in a separate pass through the entry list.
*/

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug5,
        "dpe_acl_mgr_get_access_v0: (PAC = %s@%s+%dL+%dF, last ACLE = %2d)  ===>  ",
        accessor_info->principal.name, 
	accessor_info->realm.name,
        accessor_info->num_groups,
	accessor_info->num_foreign_groups,
	sec_acl_p->num_entries - 1)); 

    in_default_realm = uuid_equal( &accessor_info->realm.uuid,	/* Set this just once now we know we have a PAC */
				   &sec_acl_p->default_realm.uuid, &st);

    for (i = 0; i < sec_acl_p->num_entries; i++) {

	/* Check for existence of each type of entry, and keep track
         * of where each type was found in the entry list.
         * Don't mark type as found if the associated realm id's
         * are not the same
         */
        switch(sec_acl_p->sec_acl_entries[i].entry_info.entry_type) {
            case sec_acl_e_type_user:
                if ( in_default_realm &&
		     uuid_equal( &accessor_info->principal.uuid,
                                 &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.id.uuid, &st))
                {   user_entry = i; }
                break;
            case sec_acl_e_type_foreign_user:
                if ((uuid_equal(&accessor_info->principal.uuid,
                                &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.foreign_id.id.uuid,
                                &st)) && (foreign_realm_eq(i)))
                {   foreign_user_entry = i; }
                break;
            case sec_acl_e_type_other_obj:
                if ( in_default_realm) other_obj_entry = i;
                break;
            case sec_acl_e_type_foreign_other:
                if ( uuid_equal( &accessor_info->realm.uuid,
				 &sec_acl_p->sec_acl_entries[ i ].entry_info.tagged_union.id.uuid, &st))
		    foreign_other_entry = i;
                break;
            case sec_acl_e_type_unauthenticated:
                unauth_entry = i;
                break;
	    case sec_acl_e_type_group:				/* These are ignored by this pass over the acl */
	    case sec_acl_e_type_foreign_group:
		break;
	    case sec_acl_e_type_any_other:
		any_other_entry = i;
		break;
#ifdef DPE_ALLOW_MASK_OBJ_ACLE
	    case sec_acl_e_type_mask_obj:
                mask_perms = sec_acl_p->sec_acl_entries[i].perms;
                break;
#endif
	    case sec_acl_e_type_user_obj:			/* USER_OBJ entries NOT supported */
	    case sec_acl_e_type_group_obj:			/* GROUP_OBJ entries NOT supported */
	    case sec_acl_e_type_extended:			/* Extended entries NOT supported */
#ifndef DPE_ALLOW_MASK_OBJ_ACLE
	    case sec_acl_e_type_mask_obj:
#endif
            default:
		/* Ignore these */
                break;
        }
    }

/*
** Now that we know which entries match the user described in the PAC, check
** the permissions corresponding to each entry until access is granted by one
** of them.
*/

    /* USER check */
    if ( user_entry != ENTRY_NOT_FOUND)
    {	masked_access( sec_acl_p->sec_acl_entries[ user_entry],
                       mask_perms, *access_extent );
	goto chk_unauth; }

    /* FOREIGN_USER check */
    if ( foreign_user_entry != ENTRY_NOT_FOUND)
    {	masked_access( sec_acl_p->sec_acl_entries[ foreign_user_entry],
                       mask_perms, *access_extent);
	goto chk_unauth; }

    /* GROUP checks */
    group_access = NO_PERMISSIONS;
    for (i = 0; i < sec_acl_p->num_entries; i++) {
        switch(sec_acl_p->sec_acl_entries[i].entry_info.entry_type) {
            case sec_acl_e_type_group:
            case sec_acl_e_type_foreign_group:
	    {   sec_acl_permset_t tmperm;
		if (sec_acl_p->sec_acl_entries[i].entry_info.entry_type
                                                    == sec_acl_e_type_group)
		{   chk_loc_groups = in_default_realm;
                    group_id = 
                        &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.id;
                    realm_id = &sec_acl_p->default_realm; }
                else {			 /* == sec_acl_e_type_foreign_group */
                    chk_loc_groups = foreign_realm_eq(i);
                    group_id = 
                        &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.foreign_id.id;
                    realm_id =
                        &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.foreign_id.realm; }

                /* Check either the local groups or the foreign groups */
                if (chk_loc_groups)
                {   /* CHECK PAC GROUP */
                    if (uuid_equal(&accessor_info->group.uuid, &group_id->uuid, &st) )
		    {   one_group_found = true;
                        masked_access( sec_acl_p->sec_acl_entries[i],
                                       mask_perms, tmperm);
			group_access = (group_access | tmperm); }

                    /* CHECK LOCAL GROUPS */
                    for (j = 0; (j < accessor_info->num_groups); j++)
		    {	if (uuid_equal(&accessor_info->groups[j].uuid, &group_id->uuid, &st) )
			{   one_group_found = true;
                            masked_access( sec_acl_p->sec_acl_entries[i],
					   mask_perms, tmperm);
			    group_access = (group_access | tmperm); }}}	
                else {
                    /* CHECK FOREIGN GROUPS */
                    for (j = 0; (j < accessor_info->num_foreign_groups); j++)
		    {	if (( uuid_equal( &accessor_info->foreign_groups[j].id.uuid,
                                          &group_id->uuid, &st)) &&
                            ( uuid_equal( &accessor_info->foreign_groups[j].realm.uuid,
                                          &realm_id->uuid, &st)) )
			{   one_group_found = true;
                            masked_access( sec_acl_p->sec_acl_entries[i],
                                           mask_perms, tmperm);
			    group_access = (group_access | tmperm); }}}
                break; }					/* Bottom of sec_acl_e_type_*group case */
            default: break;
        } /* switch (entry type) */
    } /* GROUP check */
/*
** If one or more groups found then we take the union of all matching group's
** permissions and call that the access extent.
*/
    if (one_group_found)
    {	
         *access_extent = group_access;				/* We HAVE the access.  Do not add any other perms. */

         DCE_SVC_DEBUG((
             cds__svc_handle,
	     cds_svc_library,
	     svc_c_debug5,
	     "  dpe_acl_mgr_is_authorized: (5) access_extent = 0x%x",
             *access_extent));

	goto chk_unauth; 
    }

    /* OTHER_OBJ check */
    if ( other_obj_entry != ENTRY_NOT_FOUND)
    {	masked_access( sec_acl_p->sec_acl_entries[other_obj_entry],
                       mask_perms, *access_extent);
	goto chk_unauth; }

    /* FOREIGN_OTHER check */
    if ( foreign_other_entry != ENTRY_NOT_FOUND)
    {	masked_access( sec_acl_p->sec_acl_entries[foreign_other_entry],
                       mask_perms, *access_extent);
	goto chk_unauth; }

    /* ANY_OTHER check */
    if ( any_other_entry != ENTRY_NOT_FOUND)
    {	masked_access( sec_acl_p->sec_acl_entries[any_other_entry],
                       mask_perms, *access_extent);
	goto chk_unauth; }

/*
** Here when NO ACL Entry was found giving any sort of access.  Return no access.
*/

    *access_extent = NO_PERMISSIONS;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <dpe_acl_mgr_get_access_v0: (2) return true: access_extent = 0x%x (==NO_PERMISSIONS !)",
	*access_extent));

    return true;

/* UNAUTHENTICATED check
** Here when access was found for one of the above checks.
** If the PAC isn't authenticated then access_extent MUST be masked by unauthenticated entry!
*/
chk_unauth:
    if ( ! accessor_info->authenticated)
    {	if (unauth_entry == ENTRY_NOT_FOUND)			/* Here if unauthenticated PAC. */
        {   
	    *access_extent = NO_PERMISSIONS;			/* Here if no unauth ACL entry.  Say NO access at all. */

	    DCE_SVC_DEBUG((
                 cds__svc_handle,
		 cds_svc_library,
		 svc_c_debug5,
		 "  dpe_acl_mgr_is_authorized: (3) access_extent = 0x%x (==NO_PERMISSIONS !)",
                 *access_extent));
	}
        else {	sec_acl_permset_t uperm;			/* Have unauth ACL entry. */
		masked_access( sec_acl_p->sec_acl_entries[unauth_entry],
			       mask_perms, uperm);
		*access_extent = *access_extent & uperm; 		/* Disallow prev perms not in unauth ACL entry */

                DCE_SVC_DEBUG((
                   cds__svc_handle,
		   cds_svc_library,
		   svc_c_debug5,
		   "  dpe_acl_mgr_is_authorized: (4) access_extent = 0x%x",
                   *access_extent));
         }
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <dpe_acl_mgr_get_access_v0: @ end - return true"));

    return true;
}								/* Bottom of dpe_acl_mgr_get_access_v0 */

