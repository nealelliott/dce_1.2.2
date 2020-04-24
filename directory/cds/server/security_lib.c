/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: security_lib.c,v $
 * Revision 1.1.8.2  1996/02/18  19:36:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:05  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:31:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:39  root]
 * 
 * Revision 1.1.6.5  1994/08/25  19:30:56  proulx
 * 	Delegation support.
 * 	[1994/08/25  19:26:32  proulx]
 * 
 * Revision 1.1.6.3  1994/06/09  18:43:43  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:16  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/14  14:51:27  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:21:32  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:12:15  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:44:53  peckham]
 * 
 * Revision 1.1.4.3  1993/01/29  21:51:44  keegan
 * 	Fix logic in 'security_check' when deleting entries.
 * 	Ensure that the check for 'admin' permission on the directory is
 * 	done if the directory stream has already been read.
 * 	[1993/01/29  20:41:07  keegan]
 * 
 * Revision 1.1.4.2  1992/12/30  14:23:03  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:23:34  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/09  16:35:50  keegan
 * 	Changed authorization code for delete entry to match documentation.
 * 	User must have delete permission to the entry OR administer permission
 * 	to the parent directory.
 * 	[1992/07/09  13:38:33  keegan]
 * 
 * Revision 1.1  1992/01/19  15:26:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module security_lib.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
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
 *
 */

#include <server.h>
#include <clerk.h>
#include <dbdef.h>
#include <events.h>
#include <find.h>
#include <names.h>
#include <security.h>
#include <dpeacl.h>
#

extern nsgbl_ns_t dns_nameserver;

/*
 * The diagnostic override variable is used to inhibit the checking
 * of access control entries.
 *
 * The node verification override is used to inhibit the verification
 * of inbound nodenames.
 *
 * The RETURN_DENIAL macro should be expanded to include the logging of
 * an event when the diagnostic override is used.
 */

static int diagnostic_override = 0;     /* No override, by default */
static int verifying_nodenames = 1;     /* On, by default */

#define RETURN_DENIAL(reason, index) {				\
    if (diagnostic_override == 1) 				\
    {				 				\
	return (DNS_SUCCESS); 					\
    }								\
    else							\
    { 								\
         return (reason);                                       \
    }                                                         	\
}

dns_status_t 
security_dacl_v0 (user_descriptor_t *,
		  struct db_data     *,
		  sec_acl_permset_t  ,
		  int                );


/*
 * security_log
 *     This routine logs the event for a security violation.
 *
 * Input:
 *     user_p = address of user descriptor
 *     name_p = full name being accessed
 *     ch_p = address of clearinghouse descriptor
 *
 */
void 
security_log (struct user_descriptor  *user_p,
              FullName_t              *name_p,
              struct nsgbl_ch         *ch_p)
{
#if defined(DCE_SEC)
  nsgbl_ns_t *ns_p;
  
  /* Not setup for rdacl interface */
  if (user_p->ud_ns_p == NULL)
    ns_p = &dns_nameserver;
  else
    ns_p = user_p->ud_ns_p;

  if (!name_p)
    name_p = &ch_p->ch_name;
  
  evl_SecurityFailure(ns_p, name_p);
#endif
}


/* -------------------------------------------------------------------------- */
#if defined(DCE_SEC)
/*
 * is_locksmith
 *       See if user is locksmith.  If so, grant access
 */
boolean32
    security_is_locksmith(
		    sec_id_pac_t *pac_p,
		    nsgbl_ns_t     *ns_p)
{
  /* Not setup for rdacl interface */
  if (ns_p == NULL)
      ns_p = &dns_nameserver;

  if ((ns_p->ns_Locksmith_enabled == TRUE) && (pac_p != NULL)) {
    unsigned32 st;
    
    if (uuid_equal(&pac_p->realm.uuid, &ns_p->ns_Locksmith_realm, &st) &&
	 uuid_equal(&pac_p->principal.uuid, &ns_p->ns_Locksmith_princ, &st))
	return(TRUE);
  }
  return(FALSE);
}

/*
 * security_any
 *       See if user has any access to entry. return either success or
 *       unknownentry.
 *
 *       Dynamic memory allocated for the acl using rpc_ss_allocate will
 *       be freed when the server stub exits.
 */
dns_status_t 
security_any (user_descriptor_t *user_p,
              struct db_data  *rec_p)
{
    error_status_t aclstatus;
    int           status;
    sec_acl_t     *acl_p;
    sec_acl_permset_t haverights;

    /* Get the acl */
    if ((status = dacl_get(rec_p, &acl_p, DNS_OBJECTACL)) != DNS_SUCCESS)
      status = DNS_UNKNOWNENTRY;
    else {
      /* Get users rights */
      if (dpe_acl_mgr_get_access(acl_p, &user_p->ud_cred, &haverights, 
				 &aclstatus) == FALSE)
      status = DNS_UNKNOWNENTRY;
    else 
	/* 
	 * If user has any rights at all, 
	 * return accessdenied, else hide the name
	 */
	if (haverights) status = DNS_SUCCESS;
	else status = DNS_UNKNOWNENTRY;
    }
    if (status != DNS_SUCCESS)
	if (security_is_locksmith(&user_p->ud_pac, user_p->ud_ns_p) == TRUE)
	    status = DNS_SUCCESS;
    RETURN_DENIAL(status, 11);
}

/*
 * security_dacl
 *       do dacl check.
 *       This routine will check for either all rights or at least one.
 *       We attempt to hide names so if user has not access at all,
 *       we return unknownentry vs accessdenied
 *
 *       Dynamic memory allocated for the acl using rpc_ss_allocate will
 *       be freed when the server stub exits.
 */
dns_status_t 
security_dacl (user_descriptor_t *user_p,
               struct db_data     *rec_p,
               sec_acl_permset_t  rights,
               int                testtype)
{
    error_status_t aclstatus;
    int           status=0;
    sec_acl_t     *acl_p;
    sec_acl_permset_t haverights;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >security_dacl(looking for rights = 0x%X)",
        rights));

    /*
     * First, see if we have a valid V1.1 credentials structure,
     * or if we should use the pre-v1.1 PAC for the security
     * check
     */
    if (user_p->ud_cred.data == NULL) {
      status = security_dacl_v0 (user_p, rec_p, rights, testtype);
      return(status);
    }

    /* 
     * Get the acl.  If cannot get, something is wrong so let users
     * in to fix it.
     */
    if ((status = dacl_get(rec_p, &acl_p, DNS_OBJECTACL)) != DNS_SUCCESS)
	status = DNS_UNKNOWNENTRY;
    else {
    
      /* First we get all of the users rights.  Then we test either
       * that the user has all rights indicated on input
       * or has at least one.  If the test fails we return
       * accessdenied if the user has some access to the entry
       * otherwise we return unknownentry.
       */
      if (dpe_acl_mgr_get_access (acl_p, &user_p->ud_cred, &haverights, 
				  &aclstatus) == FALSE)
	status = DNS_UNKNOWNENTRY;
      else {
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug6,
		       "  security_dacl (haverights = 0x%X)",
		       haverights));
	
	if ((testtype == ANYPerms) && (rights & haverights))
	  status = DNS_SUCCESS;
	else if ((testtype == ALLPerms) && ((rights & haverights) == rights))
	  status = DNS_SUCCESS;
	else 
	  /* 
	   * If user has any rights at all, 
	   * return accessdenied, else hide the name
	   */
	  if (haverights) 
	    status = DNS_ACCESSDENIED;
	  else 
	    status = DNS_UNKNOWNENTRY;
      }
    }
    
    /* Unknownentry turned into accessdenied for clearinghouses.  We
     * aren't trying to hide them
     */
    if ((status == DNS_UNKNOWNENTRY) && (rec_p->data_type == ET_clearinghouse))
	status = DNS_ACCESSDENIED;

    /* Check locksmith mode */
    if (status != DNS_SUCCESS)
	if (security_is_locksmith(&user_p->ud_pac, user_p->ud_ns_p) == TRUE)
	    status = DNS_SUCCESS;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug6,
	" <security_dacl: return(status=%d)",
	status));

    RETURN_DENIAL(status, 12);
}
#endif

/*
 * security_check for CDS
 * This routine determines the appropriate access check based on function
 * code and entry type.  The main difference in CDS and DNS is
 * the use of control access and the anding of rights. Control access
 * in cds does not mean you can do everything;  creates and
 * deletes require multiple bits.
 */
dns_status_t 
security_check (user_descriptor_t *user_p,
                db_stream_t     *stream_p,
                SecurityOpCodes opcode,
                int             type,
                ObjUID_t        *objid_p)
{
#if !defined(DCE_SEC)
    return(DNS_SUCCESS);
#else
    sec_acl_permset_t rights;
    dns_status_t status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >security_check"));

    switch ((int)opcode) {
      case Create_Entry_check:
	rights = sec_acl_perm_insert;
	break;

      case Delete_Entry_check:
	rights = sec_acl_perm_delete;
	break;

      case Create_Dir_check:  /* Write access to clearinghouse */
	if (type == ET_clearinghouse)
	{
	    rights = sec_acl_perm_write;
	}
	else 
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_server,
		svc_c_debug6,
		" <security_check: (1) return(DNS_NOTSUPPORTED)"));

	    return(DNS_NOTSUPPORTED);
	}
	break;
      case Delete_Dir_check:
	if (type == ET_clearinghouse)
	{
	    rights = sec_acl_perm_write;
	}
	else	
	    if (type == ET_directory)
	    {
		rights = sec_acl_perm_delete;
	    }
	    else
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_server,
		    svc_c_debug6,
		    " <security_check: (2) return(DNS_NOTSUPPORTED)"));

		return(DNS_NOTSUPPORTED);
	    }
	break;
      case Modify_check:

      case Modify_Del_check:
        rights = sec_acl_perm_write;
	break;

      case Modify_Rptype_check:
	if (type == ET_clearinghouse)
	{
	    rights = sec_acl_perm_write;
	}
	else
	{
	     DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_server,
		svc_c_debug6,
		" <security_check: (3) return(DNS_NOTSUPPORTED)"));

	    return(DNS_NOTSUPPORTED);
	}
	
	break;
      case Skulk_check:
	/* Any updates => you can skulk it */
        rights = sec_acl_perm_admin | sec_acl_perm_write | 
	  sec_acl_perm_insert | sec_acl_perm_delete;
	break;

      case Super_check:            /* Skulking, Replication,
				    * and inchname, background stuff */   
        rights = sec_acl_perm_admin;
	break;

      case Enum_Att_check:

      case Enum_Entry_check:

      case Resolve_check:

      case Read_check:
	rights = sec_acl_perm_read;
	break;

      case Test_check:
	rights = sec_acl_perm_read | sec_acl_perm_test;

	break;
      case Replica_Add_check:
        if (type == ET_clearinghouse)
	{
	    rights = sec_acl_perm_write;
	}
	else
	{
	     DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_server,
		svc_c_debug6,
		" <security_check: (4) return(DNS_NOTSUPPORTED)"));

	    return(DNS_NOTSUPPORTED);
	}
	break;
      case Replica_Remove_check:
        if (type == ET_clearinghouse)
	{
	    rights = sec_acl_perm_write;
	}	
	else
	{
	     DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_server,
		svc_c_debug6,
		" <security_check: (5) return(DNS_NOTSUPPORTED)"));

	    return(DNS_NOTSUPPORTED);
	}
	break;
      default:
	 DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_server,
		svc_c_debug6,
		" <security_check: (6) return(DNS_NOTSUPPORTED)"));

	return(DNS_NOTSUPPORTED);
    }

    if ((type == ET_directory) || (type == ET_clearinghouse))
    {
	status = security_dacl(user_p, &stream_p->db_dir, rights, ANYPerms);
    }
    else {
	/* 
	 * Delete object/link requires delete permission to entry OR
	 * administer permission to parent directory.
	 * (in OSF/DCE 1.0, this used to be: delete perm. to entry OR 
	 * delete perm. to parent directory)
	 */
	status = security_dacl(user_p, &stream_p->db_entry, rights, ANYPerms);
	if (opcode == Delete_Entry_check) {
	    
	    /* If directory is not read, do it now */
	    if (!stream_p->db_dir.data_p) {

	      /* preserve previous status */
		if (db_dir_read(stream_p) != DNS_SUCCESS) 
		{
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_server,
			svc_c_debug6,
			" <security_check: (1) return(sts=%d)",
			status));

		    return (status);
		}
	    }
/* Check for administer permission on directory */
	    if (security_dacl(user_p, &stream_p->db_dir, 
			      sec_acl_perm_admin, ANYPerms) == DNS_SUCCESS)
	    {		
		status = DNS_SUCCESS;
	    }	    
	}
    }
 
    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug6,
	" <security_check: @end - return(sts=%d)",
	status));

    return(status);

#endif /* dce_sec */
}

/*
 * security_dacl_v0
 *       do dacl check, a la pre v1.1 when we only have the old style 
 *       PAC in the user structure.
 *
 *       This routine will check for either all rights or at least one.
 *       We attempt to hide names so if user has not access at all,
 *       we return unknownentry vs accessdenied
 *
 *       Dynamic memory allocated for the acl using rpc_ss_allocate will
 *       be freed when the server stub exits.
 */
dns_status_t 
security_dacl_v0 (user_descriptor_t *user_p,
		  struct db_data     *rec_p,
		  sec_acl_permset_t  rights,
		  int                testtype)
{
    error_status_t aclstatus;
    int           status=0;
    sec_acl_t     *acl_p;
    sec_acl_permset_t haverights;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >security_dacl(looking for rights = 0x%X)",
        rights));

#ifdef DCE_SEC 

    /* 
     * Get the acl.  If cannot get, something is wrong so let users
     * in to fix it.
     */
    if ((status = dacl_get(rec_p, &acl_p, DNS_OBJECTACL)) != DNS_SUCCESS)
	status = DNS_UNKNOWNENTRY;
    else {
    
      /* First we get all of the users rights.  Then we test either
       * that the user has all rights indicated on input
       * or has at least one.  If the test fails we return
       * accessdenied if the user has some access to the entry
       * otherwise we return unknownentry.
       */
      if (dpe_acl_mgr_get_access_v0 (acl_p, &user_p->ud_pac, &haverights, 
				     &aclstatus) == FALSE)
	status = DNS_UNKNOWNENTRY;
      else {
	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug5,
		       "  security_dacl (haverights = 0x%X)",
		       haverights));
	
	if ((testtype == ANYPerms) && (rights & haverights))
	  status = DNS_SUCCESS;
	else if ((testtype == ALLPerms) && ((rights & haverights) == rights))
	  status = DNS_SUCCESS;
	else 
	  /* 
	   * If user has any rights at all, 
	   * return accessdenied, else hide the name
	   */
	  if (haverights) 
	    status = DNS_ACCESSDENIED;
	  else 
	    status = DNS_UNKNOWNENTRY;
      }
    }
    
    /* Unknownentry turned into accessdenied for clearinghouses.  We
     * aren't trying to hide them
     */
    if ((status == DNS_UNKNOWNENTRY) && (rec_p->data_type == ET_clearinghouse))
	status = DNS_ACCESSDENIED;

    /* Check locksmith mode */
    if (status != DNS_SUCCESS)
	if (security_is_locksmith(&user_p->ud_pac, user_p->ud_ns_p) == TRUE)
	    status = DNS_SUCCESS;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug6,
	" <security_dacl: return(status=%d)",
	status));

    RETURN_DENIAL(status, 12);
}
#endif



/* -------------------------------------------------------------------------- */

/*
 * ROUTINE - security_diag_set_override
 *
 * This routine sets the state of the ACS checking override.
 *
 * Arguments:
 *     new_state (in) - The desired state of the override variable
 *
 * Returns
 *     The previous state of the override variable.
 */
int 
security_diag_set_override (int new_state)
{
    int previous_state;

    previous_state = diagnostic_override;
    diagnostic_override = new_state;

    return (previous_state);
}

/* -------------------------------------------------------------------------- */


/*
 * ROUTINE - security_diag_read_override
 *
 * This routine reads the state of the ACS checking override.
 *
 * Returns
 *     The current state of the override variable.
 */
int 
security_diag_read_override (void)
{   
    return (diagnostic_override);
}


/* -------------------------------------------------------------------------- */


/*
 * ROUTINE - security_diag_set_node_verif
 *
 * This routine sets the state of the node verification override.
 *
 * Arguments:
 *     new_state (in) - The desired state of the override variable
 *
 * Returns
 *     The previous state of the override variable.
 */
int 
security_diag_set_node_verif (int new_state)
{
    int previous_state;

    previous_state = verifying_nodenames;
    verifying_nodenames = new_state;

    return (previous_state);
}
