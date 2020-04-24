/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: adver_dacl.c,v $
 * Revision 1.1.10.1  1996/10/03  14:33:24  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  13:17:12  arvind]
 *
 * Revision 1.1.8.2  1996/02/18  19:23:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:43  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  00:10:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:12  root]
 * 
 * Revision 1.1.6.11  1994/09/15  19:36:48  mccann
 * 	change size to length OT12211
 * 	[1994/09/15  19:35:33  mccann]
 * 
 * Revision 1.1.6.10  1994/08/25  19:30:35  proulx
 * 	Put text strings to cds.sams and some cleanup
 * 	[1994/08/25  19:09:29  proulx]
 * 
 * Revision 1.1.6.9  1994/07/25  15:12:13  proulx
 * 	Use common ACL manager for subcomponent mgmt ACLs
 * 	[1994/07/25  14:03:16  proulx]
 * 
 * Revision 1.1.6.8  1994/06/30  19:15:05  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:35:05  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.6.7  1994/06/23  20:29:45  jd
 * 	Fix define. O_CLERK -> HCLERK. O_CLERK undefined.
 * 	[1994/06/23  20:29:22  jd]
 * 
 * Revision 1.1.6.6  1994/06/23  17:59:44  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  17:59:04  mccann]
 * 
 * Revision 1.1.6.5  1994/06/09  16:06:48  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:20  devsrc]
 * 
 * Revision 1.1.6.4  1994/04/29  15:49:28  peckham
 * 	Use dcelocal_path in filenames.
 * 	[1994/04/29  13:55:31  peckham]
 * 
 * Revision 1.1.6.3  1994/04/25  19:57:15  ohara
 * 	add stdio.h
 * 	[1994/04/21  16:06:58  ohara]
 * 
 * Revision 1.1.6.2  1994/04/19  17:05:10  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:23:09  mccann]
 * 
 * Revision 1.1.6.1  1994/03/12  21:54:47  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:44:17  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:34:46  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:16:56  zeliff]
 * 
 * Revision 1.1.2.4  1992/06/26  06:25:48  jim
 * 	Added include of msgblk.h for AIX 3.2.
 * 	[1992/05/29  18:35:04  jim]
 * 
 * Revision 1.1.2.3  1992/05/08  15:40:50  burati
 * 	Add rdacl_get_mgr_types_semantics support.
 * 	[1992/05/07  18:11:37  burati]
 * 
 * Revision 1.1.2.2  1992/03/22  21:08:57  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:03:30  weisman]
 * 
 * Revision 1.1  1992/01/19  15:26:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
** Module: adver_dacl.c
**
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
 * ABSTRACT:
 * Clerk (adver) ACL management interface. Security module, included
 * only if built with security enabled.
 *
*/

#include <adver.h>
#include <unistd.h>
#include <stdio.h>
#include <dns_strings.h>
#include <dns_cfg.h>
#include <dpeacl.h>

#include <dce/aclif.h>
#include <dce/dceacl.h>
#include <dce/rdaclifv0.h>
#include <dce/rdaclif.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <dce/secidmap.h>
#include <dce/dce_cf.h>
#include <dce/uuid.h>

/*
 * We will create/use the DACL for the cds-clerk, and allow the common
 * ACL manager for DCE 1.1 the privilege of managing it.
 *
 * Here we get/store the necessary info for creating the default DACL.
 */
char Clerk_Realm[100];     /* Our Cell       */
uuid_t Clerk_Realm_uuid;   /* Our CEll UUID */
char HostPrincipal[100];   /* host (machine) principal name */
uuid_t HostPrincipal_uuid; /* host (machine) principal uuid */
char AdminGroup[100];      /* cds-admin group name          */
uuid_t AdminGroup_uuid;    /* cds-admin group uuid          */
extern uuid_t clerk_mgmt_dacl_uuid; /* filled in previously in InitializeRPC */

/*
 * Handle to backing storage for this ACL 
 */
static dce_db_handle_t acl_db ;

/*
 * ACL print string and help string id table
 */
#define EVT_ENTRY_SIZE (3*sizeof(long))

static long clerk_mgmt_help_ids[] = 
{
  /* general ACL help */
  EVT_DACLM_CLERK_MGMT, EVT_DACLMH_CLERK_MGMT, (sec_acl_permset_t)0, 
  /* permission helps */
  EVT_DACLPREAD,        EVT_DACLH_R_CLERK, sec_acl_perm_read,
  EVT_DACLPWRITE,       EVT_DACLH_W_CLERK, sec_acl_perm_write,
  EVT_DACLPCONTROL,     EVT_DACLH_C_CLERK, sec_acl_perm_control
};

#define PERM_STRING_CNT 3
static sec_acl_printstring_t clerk_printstrings[PERM_STRING_CNT+1];

/*
 * The clerk management DACL:
 */
boolean clerk_acl_ready = FALSE;

int 
clerk_mgmt_acl_init (void);

static int
create_def_clerk_mgmt_acl (sec_acl_t *);

static void
get_cell_and_host_info (error_status_t     *);

static void clerk_acl_resolve (handle_t handle,
			       sec_acl_component_name_t,
			       sec_acl_type_t,
			       uuid_t *,
			       boolean32,
			       void *,
			       uuid_t *,	
			       error_status_t *);

void mgt_register_acl_mgr_if (rpc_binding_vector_t	*bindings,
			      uuid_vector_t		*object_uuids,
			      error_status_t		*status);

void mgt_unregister_acl_mgr_if ( rpc_binding_vector_t *bindings,
				uuid_vector_t         *object_uuids,
				error_status_t        *status);

void mgt_export_acl_mgr_if (unsigned_char_t		*entry_name,
			    rpc_binding_vector_t	*bindings,
			    uuid_vector_t		*object_uuids,
			    error_status_t		*status);

void mgt_unexport_acl_mgr_if (unsigned_char_t		*entry_name,
			      uuid_vector_t		*object_uuids,
			      error_status_t		*status);

static void
build_printstrings (void);

/*
 * clerk_mgmt_acl_init
 * 
 * Initialize clerk mgmt DACL.
 * Called once at startup by InitializeRPC (in adver_cds_event.c), 
 * before the rdacl and cdscp  interface is registered.
 *
 * Returns: error_status_ok on success, other on error.
 */
int 
clerk_mgmt_acl_init (void)
{
   boolean32    must_create_default_acl = FALSE;
   boolean32	must_store_acl = FALSE;
   boolean32 	have_old_acl_file = FALSE;
   unsigned int flags = db_c_index_by_uuid ;       
   error_status_t      status, file_status;
   static sec_acl_t    theAcl ;
   sec_acl_p_t	       temp_acl_p = &theAcl ;    
   char		mgt_acl_filename[CDS_STR_FNAME_MAX+1];
   char		new_mgt_acl_filename[CDS_STR_FNAME_MAX+1];
   char * 	lastchp = 0;


   DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		 " >clerk_mgmt_acl_init"));

   /*
    * Use old file name to build new file name
    */
   sprintf (mgt_acl_filename, CDS_CLERK_MGMT_ACL_FILENAME, dcelocal_path);
   
   strcpy( new_mgt_acl_filename, mgt_acl_filename ) ;
   if((lastchp = strrchr( new_mgt_acl_filename,  '/')) != 0) {
     lastchp++; }
   strcpy( lastchp, CDS_CLERK_MGMT_V1_ACL_FILENAME);

   /* 
    * See if v1.1-type file already exists 
    */
   if ( access( new_mgt_acl_filename, (R_OK) ) == 0) {
       
      /*
       * open the adver acl database - assume that it exists.
       * if it fails, then add the create flag, and try again 
       */
      dce_db_open( new_mgt_acl_filename, 
		  (const char *)0, /* backend type */
		  flags,           /* index by uuid */
		  (dce_db_convert_func_t)dce_rdacl_convert, /* cnvrt functn*/ 
		  &acl_db,         /* database handle */
		  &status) ; 
      if (status != error_status_ok) {
	 dce_svc_printf(CDS_S_ACLINIT_MSG);
	 return(status);
      }
      /* 
       * get the acl from the database
       */
      dce_db_fetch_by_uuid (acl_db, 
			    &clerk_mgmt_dacl_uuid,
			    temp_acl_p, 
			    &status) ;
      if (status != error_status_ok) { /* can't get it  */
	 dce_svc_printf(CDS_S_ACLINIT_MSG);
	 return(status);
      }
   }
   else {  /* don't have v1.0 acl file */

      must_store_acl = TRUE ;	 

      /* 
       * Check the old file  
       */
      if ( access ( mgt_acl_filename, (R_OK) ) == 0) {
	 /* 
	  * Try to read it 
	  */
	 dpe_acl_storage_read (mgt_acl_filename, 
			       &temp_acl_p, 
			       &file_status);
	 if (file_status != dpe_acl_s_ok) {
	    must_create_default_acl = TRUE ;
	    dce_svc_printf(CDS_S_NOACL_MSG, (long)status);
	                          /* failed to read, creating */
	 }
	 else
	   have_old_acl_file = TRUE ;
      }
      else {
	 must_create_default_acl = TRUE ;
      }

      /* 
       * since the new db doesn't exist, create one 
       */
      flags |= db_c_create ; /* create it  */

      dce_db_open (new_mgt_acl_filename,    /* filename */
		   (const char *)0,       /* backend type */
		   flags,                /* db_c_create & db_c_index_by_uuid */
		   (dce_db_convert_func_t)dce_rdacl_convert,  
		   &acl_db,               /* database handle */
		   &status);
      
      if (status != error_status_ok) {
	 dce_svc_printf(CDS_S_ACLWRITE_MSG, (long)status); 
	 return status ;
      }
   }  /* end don't have v1.0 acl file */
   
   /*
    * Now we have an open v1.0 acl file
    */
   /*
    * enable stub management memory allocation 
    * (for backing store code: rdacl_lookup ) 
    */
   rpc_sm_enable_allocate(&status);
   if (status != error_status_ok) {
      dce_svc_printf(CDS_S_BAD_RPC_ALLOC_MSG, (long)status); 
      return status ;
   }
    
   {
      /*
       * register object types for this database 
       * N.B. we also register a private resolver function 
       * to map input parms to a uuid 
       */
     sec_acl_printstring_t *clerk_mgmt_acl_help = &clerk_printstrings[0];
     sec_acl_printstring_t *clerk_mgmt_perm_help = &clerk_printstrings[1];
     
     build_printstrings ();
     
     dce_acl_register_object_type (acl_db, /* database handle */
				   &clerk_mgmt_dacl_uuid, /* clerk uuid  */
				   PERM_STRING_CNT,        
				   clerk_mgmt_perm_help,    /* "rwc" perms */
				   clerk_mgmt_acl_help,     /* "cds-clerk" */
				   sec_acl_perm_control,  
				   0, 
				   clerk_acl_resolve, (void *)NULL, 
				   0, /* flags = no owner obj, no group obj,
					 no orphans                */
				   &status);
     if (status != error_status_ok) {
       dce_svc_printf(CDS_S_BAD_REGISTER_OBJ_MSG, (long)status); 
       return status ;
     }
   }
    
   if (must_create_default_acl) { 
      /* 
       * Either corrupt or non-existent 
       */
      if ( create_def_clerk_mgmt_acl (temp_acl_p) != error_status_ok ) {
	 dce_svc_printf(CDS_S_ACLFAIL_MSG, (long)status);
	 return status;
      }
   }

   if (must_store_acl) {
      /*
       * Save our ACL in database
       */
      dce_db_store_by_uuid (acl_db, 
			    &clerk_mgmt_dacl_uuid,
			    temp_acl_p, 
			    &status);
      if (status != error_status_ok) {
	 dce_svc_printf(CDS_S_ACLWRITE_MSG, (long)status);
	 return status;
      }
   }

   if (have_old_acl_file) {
      (void)remove (mgt_acl_filename);
      dpe_acl_free_acl (temp_acl_p);
   }

   clerk_acl_ready = TRUE;

   DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		  " <clerk_mgmt_acl_init with %lx", (long)status));
   return (status);
}

/*
 * build_printstrings - build structure for printstrings needed
 * for the common acl manager out of our arrays.  Basicly, do
 * conversion from constants to sams file text strings and
 * move the strings in the printstring structure
 */
static void
build_printstrings (void) 
{
  unsigned char *tmpbuf;
  int  string_index,id_index;
  error_status_t status;

  for (id_index = 0, string_index = 0; 
       id_index < sizeof(clerk_mgmt_help_ids)/sizeof(long); 
       id_index += EVT_ENTRY_SIZE/sizeof(long), string_index++) {
    tmpbuf = dce_msg_get_msg(clerk_mgmt_help_ids[id_index], &status);
    strcpy ((char *)clerk_printstrings[string_index].printstring, 
	    (char *)tmpbuf);
    free (tmpbuf);
    tmpbuf = dce_msg_get_msg(clerk_mgmt_help_ids[id_index+1],&status); 
    strcpy ((char *)clerk_printstrings[string_index].helpstring, 
	    (char *)tmpbuf);
    free (tmpbuf);
    clerk_printstrings[string_index].permissions = clerk_mgmt_help_ids[id_index+2];
  }  
}

/*
 * Generate a default ACL for clerk management consisting
 * of:
 *   an entry containing all permissions for the machine principal,
 *   an entry containing all permissions for the cds-admin group,
 *   an entry containing read permission for 'any_other' 
 *   an entry containing read permission for unauthenticated users.
 *
 * Returns error_status_ok on success; other on failure
*/
static int 
  create_def_clerk_mgmt_acl (sec_acl_t *acl_p)
{
  sec_acl_entry_t      *sec_p;
  error_status_t 	status;
  boolean32 		orig_sec_nsi_state; 	/* original nsi state for   */
                                                /* security bindings  	    */
                                                /* (formally 'BIND_PE_SITE')*/ 
  sec_acl_permset_t   permset = (sec_acl_permset_t) 0;
  

  DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		 " >create_def_clerk_mgmt_acl"));

  /* Get the Cell and host names and their UUID.
   * This requires that we be able to talk to the registry server,
   * which means that we must have a valid login context.
   * Also, disable the use of nsi binding by the security code while
   * we do this because we don't want it to try to make cds api calls
   * here.
   */
  orig_sec_nsi_state = sec_rgy_enable_nsi (0); /* disable */

  get_cell_and_host_info(&status);

  sec_rgy_enable_nsi(orig_sec_nsi_state);     /* restore regardless of status*/

  if (status != error_status_ok) 
  {
      dce_svc_printf(CDS_S_ACLFAIL_MSG, (long)status);
      return(status);
  }

  /*
   * Create this ACL for clerk object in the acl database
   */
  dce_acl_obj_init( &clerk_mgmt_dacl_uuid, acl_p, &status);
  if (status != error_status_ok) {
     dce_svc_printf(CDS_S_BAD_ACL_INIT_OBJ_MSG, (long)status);
     return(status);
  }

  /*
   * Get local realm and add it here 
   */

  /*
   * 1st entry = "rwc" access for machine principal 
   */
  permset = (sec_acl_perm_control | sec_acl_perm_read | sec_acl_perm_write);
  dce_acl_obj_add_user_entry (acl_p, permset, &HostPrincipal_uuid, &status);
  if (status != error_status_ok) {
     dce_svc_printf(CDS_S_BAD_ADD_USER_ACL_MSG, (long)status);
     return (status);
  }

  /*
   * 2nd entry = "rwc" access for cds-admin group    
   */
  permset = (sec_acl_perm_control | sec_acl_perm_read | sec_acl_perm_write);
  dce_acl_obj_add_group_entry( acl_p, permset, &AdminGroup_uuid, &status);
  if (status != error_status_ok) {
     dce_svc_printf(CDS_S_BAD_ADD_GROUP_ACL_MSG, (long)status);
     return (status);
  }
  
  /*
   * 3rd entry = "r" access to any other principals 
   */
  permset = sec_acl_perm_read ;
  dce_acl_obj_add_any_other_entry( acl_p, permset, &status);
  if (status != error_status_ok) {
     dce_svc_printf(CDS_S_BAD_ADD_ANY_ACL_MSG, (long)status);
     return (status);
  }
  
  /*
   * 4th entry = "r" access for the unauthenticated users 
   */
  permset = sec_acl_perm_read ;
  dce_acl_obj_add_unauth_entry (acl_p, permset, &status);
  if (status != error_status_ok) {
     dce_svc_printf(CDS_S_BAD_ADD_UNAUTH_ACL_MSG, (long)status);
     return (status);
  }
  
  DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		 " <create_def_clerk_mgmt_acl"));

  return(error_status_ok);
}

/*
 * Get cell name, host principal, cds-admin group name from
 * configuration file(s).  Let values in cds.conf override any
 * defaults. Also lookup their UUIDS. (makes call to registry server, 
 * using unathenticated requests)
 */
#define NULL_STRING "\0"

static void
  get_cell_and_host_info (error_status_t *st_p)
{
  char *cellname, *hostprinc;
  unsigned_char_p_t uuid_string;
  sec_rgy_handle_t rgy_context; 
  sec_rgy_bind_auth_info_t auth_info;
  unsigned32 status;
  char *cfg_h, *p;
  error_status_t st;

  /*
   * Get the cell name from the dce config file 
   */
  dce_cf_get_cell_name (&cellname, st_p);
  if (*st_p == dce_cf_st_ok) {
    strcpy (Clerk_Realm, cellname);
    free(cellname);
  }

  /*
   * Get the cell uuid 
   */

  /* Open a connection with the local registry site.
   * 
   * Bind to it without authentication, since the advertiser
   * does not have a login context.
   * NOTE: this will change. Adver will run as machine principal, so that
   * its identity will be refreshed automatically by the sec_clientd.
   */
  auth_info.info_type = sec_rgy_bind_auth_none;
  sec_rgy_site_bind ((unsigned char *)NULL_STRING, &auth_info, &rgy_context, st_p);
  if (*st_p != error_status_ok) 
  {
      dce_svc_printf(CDS_S_SITE_BIND_MSG, (long)*st_p);
      return;
  }
  sec_id_parse_name(rgy_context, (unsigned char *)Clerk_Realm,
		      NULL, &Clerk_Realm_uuid, NULL, NULL, st_p);
  if (*st_p != error_status_ok) 
  {
     dce_svc_printf(CDS_S_PARSE_NAME_MSG, (long)*st_p); 
     return;
  }

  /*
   * Get host principal name (check cds.conf first, then dce_cf.db) 
   */
  cfg_h = dns_cfgopen(NULL, NULL);
  if ((p = dns_cfgtostr(cfg_h, 
			"security.host_princ_name",
			NULL)) == NULL) {
    dce_cf_prin_name_from_host (NULL,&hostprinc,&st);
    if (st == dce_cf_st_ok) {
      strcpy(HostPrincipal, hostprinc);
      free(hostprinc);
    }
  }
  else strcpy (HostPrincipal, p);

  DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		 " Host principal: %s", HostPrincipal));

  /*
   * Get the host uuid 
   */
  sec_rgy_pgo_name_to_id (rgy_context, 
			  sec_rgy_domain_person,
			  (unsigned char *)HostPrincipal,
			  &HostPrincipal_uuid,
			  st_p);
  
  if (*st_p != rpc_s_ok) {
    dce_svc_printf(CDS_S_HOSTFAIL_MSG, (long)*st_p);
    return;
  }


  /*
   * Check cds.conf for cds-admin group name 
   */
  if ((p = dns_cfgtostr(cfg_h, 
			"security.admin_group_name",
			NULL)) == NULL) {
     /*
      * TEMPORARY: hard-wired admin group name 
      */
     strcpy (AdminGroup, "subsys/dce/cds-admin");
  }
  else strcpy (AdminGroup, p);

  /*
   * Get the admin group uuid 
   */
  sec_rgy_pgo_name_to_id (rgy_context, 
			  sec_rgy_domain_group,
			  (unsigned char *)AdminGroup,
			  &AdminGroup_uuid,
			  st_p);
  if (*st_p != rpc_s_ok) {
     dce_svc_printf(CDS_S_GROUPFAIL_MSG, (long)*st_p);
     return;
  }

  dns_cfgclose (cfg_h);
  sec_rgy_site_close(rgy_context, st_p);

  if (DCE_SVC_DEBUG_ATLEAST(cds__svc_handle, cds_svc_adver, svc_c_debug8)) {
    uuid_to_string (&Clerk_Realm_uuid, &uuid_string, st_p);
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " Cell name: %s, uuid: %s",
		   Clerk_Realm, uuid_string));
    rpc_string_free(&uuid_string, &status);
    uuid_to_string (&HostPrincipal_uuid, &uuid_string, st_p);
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " Host principal: %s, uuid: %s",
		   HostPrincipal,  uuid_string));
    rpc_string_free(&uuid_string, &status);
    uuid_to_string (&AdminGroup_uuid, &uuid_string, st_p);
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " Admin group: %s, uuid: %s",
		   AdminGroup, uuid_string));
    rpc_string_free(&uuid_string, &status);
  }
}


/*
 * clerk_acl_resolve - 
 *
 * resolver function for the new DCE 1.1 ACL library. 
 * see manpages for the dce_acl_resolve_by_{name,uuid}
 * for the interface to this function 
 * 
 * in our (simple, degenerate?) case, we have no database of objects 
 * to protect via our acls. 
 * As long as it's an object acl they want to see, we return the 
 * our manager_type uuid and our "key" uuid are one and the same, .
 */
static void clerk_acl_resolve (handle_t handle,		/* [in] parameters  */
			       sec_acl_component_name_t component_name,
			       sec_acl_type_t sec_acl_type,
			       uuid_t * mgr_type_p,
			       boolean32 writing,
			       void * resolver_arg_p,
			       uuid_t * acl_uuid_p,	/* [out] parameters */
			       error_status_t * stp )
{

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " >clerk_acl_resolve(,,,%p,,,%p,)", mgr_type_p, acl_uuid_p ));

    if ( sec_acl_type == sec_acl_type_object ) {
       *acl_uuid_p = clerk_mgmt_dacl_uuid ;
       *stp = error_status_ok ;
    }
    else {
       *stp = sec_acl_invalid_acl_type;
       DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug2,
		      "Call to clerk_acl_resolve with non-object ACL type",
		      *stp));
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " <clerk_acl_resolve(,,,,,,,%p)",
		   stp));
    return ;

}







