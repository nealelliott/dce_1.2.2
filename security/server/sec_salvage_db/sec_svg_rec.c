/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_svg_rec.c,v $
 * Revision 1.1.100.2  1996/10/03  15:13:30  arvind
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:43 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Change sec_passwd_privkey -> sec_passwd_pubkey
 * 	[1996/09/10  21:08 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 *
 * 	CHFts19728: support salvage of krbtgt private key values
 * 	[1996/08/27  16:03 UTC  aha  /main/DCE_1.2.2/aha_dbpk1_1/1]
 *
 * 	CHFts19728: support salvaging of krbtgt private key
 * 	[1996/08/27  15:33 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 *
 * 	Modifications to allow salvaging krbtgt private key.
 * 	[1996/05/24  23:18 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:48 UTC  hanfei  /main/DCE_1.2/2]
 *
 * 	multi-cell group work: add codes to reconstruct foreign principal group members.
 * 	[1996/03/14  18:45 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 *
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:55  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/12/15  19:29 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
 *
 * 	rsdb_attr_sch_add expects a pointer to the sec_attr_schema_entry_t, so take &sch_entry.
 * 	Include rsdb_attr_schema.h
 * 	HP 	[1995/07/13  20:46 UTC  burati  /main/HPDCE02/7]
 *
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	HP 	[1995/07/13  20:42 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 *
 * 	More cleanup.
 * 	HP 	[1995/07/11  18:15 UTC  jrr  /main/HPDCE02/6]
 *
 * 	Fix memory corruption problems.
 * 	HP 	[1995/06/23  13:19 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/2]
 *
 * 	build acl separately from pgo to save memory
 * 	HP 	[1995/05/25  17:06 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/1]
 *
 * 	modify to use RSDB_PGO and RSDB_DIR macros
 * 	HP 	[1995/05/15  21:12 UTC  greg  /main/HPDCE02/4]
 *
 * 	merge.
 * 	HP 	[1995/01/17  17:53 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bugfix/1]
 *
 * 	add reconstruct for replist acls.
 * 	HP 	[1994/11/17  18:19 UTC  mullan_s  /main/HPDCE02/2]
 *
 * 	Merge unintegrated
 * 	[1995/12/18  17:35:38  psn]
 *
 * 	merge
 * 	HP 	[1994/10/27  13:46 UTC  hanfei  /main/hanfei_svg_bl1/3]
 *
 * 	fixed one memory.
 * 	HP 	[1994/10/25  17:26 UTC  hanfei  /main/hanfei_svg_bl1/2]
 *
 * 	Enhancement work for attribute schema and attributes.
 * 	HP 	[1994/09/14  19:23 UTC  hanfei  /main/hanfei_svg_bl1/1]
 *
 * 	codes for backward conversion from DCE 1.1 to DCE 1.0.3.
 * 	[1994/06/17  18:43:00  devsrc  1.1.91.4]
 *
 * Revision 1.1.100.1  1996/05/10  13:26:09  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:48 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/03/14  18:45 UTC  hanfei
 * 	multi-cell group work: add codes to reconstruct foreign principal group members.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:19 UTC  psn
 * 	rsdb_attr_sch_add expects a pointer to the sec_attr_schema_entry_t, so take &sch_entry.
 * 	Include rsdb_attr_schema.h
 * 	[1995/12/15  19:29 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
 * 
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	[1995/07/13  20:46 UTC  burati  /main/HPDCE02/7]
 * 
 * 	More cleanup.
 * 	[1995/07/13  20:42 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 * 
 * 	merge
 * 	[1995/05/26  13:34 UTC  hanfei  /main/HPDCE02/5]
 * 
 * 	Fix memory corruption problems.
 * 	[1995/07/06  18:05 UTC  jrr  /main/HPDCE02/hanfei_mothra_bl4/3]
 * 
 * 	build acl separately from pgo to save memory
 * 	[1995/06/23  13:19 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/2]
 * 
 * 	modify to use RSDB_PGO and RSDB_DIR macros
 * 	[1995/05/25  17:06 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/1]
 * 
 * 	merge.
 * 	[1995/05/02  13:28 UTC  hanfei  /main/HPDCE02/SEC_migrate_merge/2]
 * 
 * 	code for construct_version_check() and a bug fix for free(bad_ptr).
 * 	[1995/05/01  19:03 UTC  hanfei  /main/HPDCE02/SEC_migrate/hanfei_migrate_bl2/2]
 * 
 * 	Create SEC_migrate_merge mainline by merging SEC_migrate and HPDCE02.
 * 	[1995/04/20  20:48 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	merge more acl migration fixes.
 * 	[1995/04/20  17:12 UTC  greg  /main/HPDCE02/SEC_migrate/5]
 * 
 * 	Get the software version instead of the state file version in
 * 	construct_init_software_version.
 * 	[1995/04/20  17:01 UTC  greg  /main/HPDCE02/SEC_migrate/greg_sec_migrate_acls/2]
 * 
 * 	Merge work from greg_sec_migrate_acls branch.
 * 	[1995/04/18  18:20 UTC  greg  /main/HPDCE02/SEC_migrate/4]
 * 
 * 	merge
 * 	[1995/02/22  17:59 UTC  hanfei  /main/HPDCE02/SEC_migrate/2]
 * 
 * 	work for new data structures added in for dce1.1 migration work.
 * 	[1995/02/22  17:51 UTC  hanfei  /main/HPDCE02/hanfei_comp_bl2/1]
 * 
 * 	work for new data fields defined by migration codes
 * 	[1995/02/08  15:54 UTC  hanfei  /main/HPDCE02/SEC_migrate/1]
 * 
 * 	merge.
 * 	[1995/01/17  18:27 UTC  hanfei  /main/HPDCE02/3]
 * 
 * 	add reconstruct for replist acls.
 * 	[1995/01/17  17:53 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bugfix/1]
 * 
 * 	Merge unintegrated
 * 	[1994/11/17  18:19 UTC  mullan_s  /main/HPDCE02/2]
 * 
 * 	ERA enhancement work.
 * 	[1994/10/26  15:01:01  hanfei]
 * 	 *
 * 
 * 	merge
 * 	[1994/10/27  15:29 UTC  hanfei  /main/HPDCE02/1]
 * 
 * 	fixed one memory.
 * 	[1994/10/27  13:46 UTC  hanfei  /main/hanfei_svg_bl1/3]
 * 
 * 	Enhancement work for attribute schema and attributes.
 * 	[1994/10/25  17:26 UTC  hanfei  /main/hanfei_svg_bl1/2]
 * 
 * 	codes for backward conversion from DCE 1.1 to DCE 1.0.3.
 * 
 * 	[1994/09/14  19:23 UTC  hanfei  /main/hanfei_svg_bl1/1]
 * 
 * Revision 1.1.93.1  1994/10/26  15:17:39  hanfei
 * 	ERA enhancement work.
 * 	[1994/10/26  15:01:01  hanfei]
 * 
 * Revision 1.1.91.5  1994/09/20  19:18:47  hanfei
 * 	codes for backward conversion from DCE 1.1 to DCE 1.0.3
 * 	[1994/09/19  14:21:01  hanfei]
 * 
 * Revision 1.1.91.4  1994/06/17  18:43:00  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:20:11  devsrc]
 * 
 * Revision 1.1.91.3  1994/03/31  19:57:02  hanfei
 * 	work for ERA entries in policy file.
 * 	[1994/03/31  19:48:25  hanfei]
 * 
 * Revision 1.1.91.2  1994/03/07  15:06:32  hanfei
 * 	[OT 8067] verify required print files existed for reconstruction and
 * 	obtain user confirmation if reconstruct on default database.
 * 	[1994/03/04  19:25:37  hanfei]
 * 
 * Revision 1.1.91.1  1994/02/04  21:23:34  hanfei
 * 	fix memory leak and init cellname in construct_init to be NULL.
 * 	[1994/02/04  21:18:47  hanfei]
 * 
 * Revision 1.1.89.1  1993/10/05  22:31:08  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:14:05  mccann]
 * 
 * Revision 1.1.5.2  1993/06/24  20:40:55  hinman
 * 	[hinman] - Make include of sys/types.h available to all
 * 	[1993/06/22  11:46:17  hinman]
 * 
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:39:36  hinman]
 * 
 * Revision 1.1.2.4  1993/03/30  22:10:29  hanfei
 * 	authentication keys's default pepper values are printed/reconstructed.
 * 	[1993/03/30  22:08:58  hanfei]
 * 
 * Revision 1.1.2.3  1993/03/23  23:02:36  hanfei
 * 	add codes for salvager reconstruct.
 * 	[1993/03/23  22:49:08  hanfei]
 * 
 * Revision 1.1.2.2  1993/03/05  18:33:46  frisco
 * 	fix for RIOS port
 * 	[1993/03/03  20:02:46  frisco]
 * 
 * 	initial salvager functionality
 * 	[1993/03/02  21:28:24  frisco]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1992, 1995
** Unpublished work. All Rights Reserved.
*/
/* 
**      Rgy Salvager Tool Reconstruct Functions
*/

#include <sys/types.h>
#include <sys/file.h>
#include <macros.h>
#include <stdio.h>
#include <seckey_util.h>
#include <rsdb.h>
#include <rsdb_create.h>
#include <rsdb_util.h>
#include <rsdb_acct.h>
#include <rsdb_acl.h>
#include <rsdb_file.h>
#include <rsdb_auth.h>
#include <rsdb_policy.h>
#include <rsdb_pgo.h>
#include <rsdb_attr_schema.h>
#include <dce/sec_daclmgr.h>
#include <dce/dce_utils.h>
#include <un_io.h>
#include <rs_pvt.h>
#include <rs_ver.h>
#include <rs_ver_pvt.h>
#include <rsdb_attr_pvt.h>
#include <sec_attr_base.h>
#include <rs_rep_pvt.h>
#include <rs_attr_base.h>
#include <sec_svg.h>
#include <sec_svg_rec.h>
#include <sec_svg_tokens.h>
#include <sec_svg_util.h>
#include <sec_svc.h>
#include <dcesadmsg.h>

#define TOWER_MOD 24
#define MAX_LINE_LEN 256
#define NUM_SCHEMA_ATTR_ACLS 4

PRIVATE rec_member_list_item_t *head_member;
PRIVATE rec_princ_list_item_t *head_princ;

PRIVATE sec_acl_mgr_handle_t acl_mgr_handle;

PRIVATE boolean32 construct_tower
  (
    FILE *fp,
    rs_replica_twr_vec_p_t rep_twrs,
    error_status_t *status
  );

PRIVATE char *construct_attr_binding
  (
    FILE                        *fp,
    char 			*value,
    sec_attr_bind_info_t	**binding_p
  );

PRIVATE boolean32 check_file_exist
  (
    char     *pathname,
    char     *filename
  ) 
{
    FILE *fopen(), *fp; 
    sec_rgy_pname_t prt_file; 

    strcpy((char *) prt_file, (char *) pathname); 
    strcat((char *) prt_file, filename); 
    fp = fopen((char *) prt_file, "r");
    if ( fp != NULL ) {
    	fclose(fp);
	return TRUE;
    }
    else {
	fprintf(stderr, "Cannot read print file: %s\n", prt_file);
	return FALSE;
    }
}


PUBLIC boolean32 sec_svg_exist_prtfiles ( void )
{
    if ( check_file_exist((char *)prt_path, PRT_FILE_PRINC) != TRUE )
	return FALSE; 

    if ( check_file_exist((char *)prt_path, PRT_FILE_GROUP) != TRUE )
	return FALSE; 

    if ( check_file_exist((char *)prt_path, PRT_FILE_ORG) != TRUE )
	return FALSE; 

    if ( check_file_exist((char *)prt_base_path, PRT_FILE_MKEY) != TRUE )
	return FALSE; 

    if ( check_file_exist((char *)prt_path, PRT_FILE_POLICY) != TRUE )
	return FALSE; 

    if ( check_file_exist((char *)prt_path, PRT_FILE_RGY_STATE) != TRUE )
	return FALSE; 

    if ( check_file_exist((char *)prt_path, PRT_FILE_REPLICAS) != TRUE )
	return FALSE; 

    return TRUE;
}


 /*
  * g e t _ p r i n c i p a l _ u u i d
  *
  * get a principal's uuid based on its name
  * 
  */
PRIVATE uuid_t get_principal_uuid ( unsigned_char_p_t name ) 
{
    rec_princ_list_item_t   *ptr;
    uuid_t                  nil_id;
    error_status_t          status;

    ptr = head_princ->next;
    while ( ptr != NULL ) {
        if ( strcmp((char *)ptr->name, (char *)name) == 0 )
	    /* entry entered already */
	    return ptr->id;
	else
	    ptr = ptr->next;
    }
    uuid_create_nil(&nil_id, &status);
}


 /*
  * a d d _ p r i n c i p a l _ u u i d
  *
  * add a principal's uuid and name
  * 
  */
PRIVATE void add_principal_uuid
    ( 
     unsigned_char_p_t name,
     uuid_t         id
    ) 
{
    rec_princ_list_item_t *curr_ptr;

    curr_ptr = head_princ;

    while ( curr_ptr->next != NULL )
        curr_ptr = curr_ptr->next;

    curr_ptr->next = NEW(rec_princ_list_item_t);
    curr_ptr = curr_ptr->next;
    curr_ptr->name = (unsigned_char_p_t )dce_strdup((char *)name);
    curr_ptr->id = id;
    curr_ptr->next = NULL;
}


 /*
  * a d d _ m e m b e r _ l i s t _ n o _ d u p l i c a t e
  *
  * add a member to the list, make sure there is no duplication with
  * a pricipal member and group member.
  * 
  */
PRIVATE void add_member_list_no_duplicate
    ( 
     sec_rgy_domain_t domain,
     sec_rgy_name_t   domain_name,
     char 	      *value,
     uuid_p_t	      cell_uuidp,
     uuid_p_t	      princ_uuidp
    ) 
{
    static rec_member_list_item_t  *curr_ptr=NULL;
    rec_member_list_item_t  *mp;
    rec_member_list_item_t  *mcurr;
    char                    *domain_name_ptr, *name_ptr, *ptr;

    mp = head_member;
    domain_name_ptr = (char *) domain_name;
    name_ptr = value;

    if ( domain == sec_rgy_domain_person )
    { /* member can be recorded on both principal and group */
      /* consider principal same as group, and swap its data*/
        domain = sec_rgy_domain_group;
	ptr = domain_name_ptr;
	domain_name_ptr = name_ptr;
	name_ptr = ptr;
    }

    while ( (mcurr=mp->next) != NULL ) {
        if ( mcurr->domain == domain &&
	     strcmp((char *)mcurr->domain_name, domain_name_ptr) == 0 &&
	     strcmp((char *)mcurr->name, name_ptr) == 0 )
	    /* entry entered already */
	    return;
	else
	    mp = mcurr;
    }

    if ( curr_ptr == NULL )
        curr_ptr = head_member;

    /* copy the data in */
    curr_ptr->next = NEW (rec_member_list_item_t);
    curr_ptr = curr_ptr->next;
    curr_ptr->domain = domain;
    curr_ptr->domain_name = (char *)malloc(strlen(domain_name_ptr)+1);
    curr_ptr->name = (char *)malloc(strlen(name_ptr)+1);
    strcpy((char *)curr_ptr->name, name_ptr);
    strcpy((char *)curr_ptr->domain_name, domain_name_ptr);
    if ( cell_uuidp != NULL ) {
    	curr_ptr->cell_uuidp = (uuid_p_t)malloc(sizeof(uuid_t));
	*curr_ptr->cell_uuidp = *cell_uuidp;
    } else {
	curr_ptr->cell_uuidp = NULL;
    }
    if ( princ_uuidp != NULL ) {
    	curr_ptr->princ_uuidp = (uuid_p_t)malloc(sizeof(uuid_t));
        *curr_ptr->princ_uuidp = *princ_uuidp;
    } else {
	curr_ptr->princ_uuidp = NULL;
    }
    curr_ptr->next = NULL;
}


 /*
  * c o n s t r u c t _ i n i t
  *
  * initialize for reconstruction 
  * 
  */
PUBLIC void construct_init
  (
   error_status_t *status
  )
{
    sec_rgy_domain_t        domain;
    sec_id_t                rgy_creator;
    sec_id_t                rgy_local_cell;
    sec_rgy_properties_t    default_props;
    unsigned char           rep_name[] = "\0";
    rs_update_seqno_t       seqno;

    /*
     * Configure the acl manager with the max software version
     * supported by this rev of the security software.  Even though
     * the current operating version may be less than the maximum
     * (we will find out exactly what the current version is when
     * we reconstruct the state file somewhere down the road), the
     * reconstruction internals need to manipulate ACL database
     * artifacts at the max software version even though those 
     * artifacts may not be visible to external consumers of 
     * security services.
     */
    sec_acl_mgr_configure((sec_acl_mgr_config_t) NULL,
			  rs_c_max_software_vers,
			  (unsigned_char_p_t) NULL, 
			  &acl_mgr_handle, status);
    if (BAD_STATUS(status)) return;

    uuid_create_nil(&rgy_local_cell.uuid, status);
    uuid_create_nil(&rgy_creator.uuid, status);
    rgy_creator.name = 0;
    rgy_local_cell.name = NULL;
    rsdb_create_set_default_properties(&default_props);

    rsdb_create(rep_name, &rgy_creator, &rgy_local_cell,
                &default_props, status);
    if (BAD_STATUS(status)) return;

    rs_util_seqno_create(&seqno);
    rs_log_create(&seqno, status);
    if (BAD_STATUS(status)) return;

    INIT_HEAD_LIST(head_member,rec_member_list_item_t);
    INIT_HEAD_LIST(head_princ,rec_princ_list_item_t);
}


 /*
  * c o n s t r u c t _ s a v e
  *
  * save the reconstruct result
  * 
  */
PUBLIC void construct_save ( error_status_t *status )
{
    rsdb_delete_files(".new", status);
    if (BAD_STATUS(status)) return;

    rsdb_save(".new", status);
    if (BAD_STATUS(status)) return;

    rsdb_close_files();
    if (BAD_STATUS(status)) return;

    rsdb_rename_files(".new", "", status);
    if (BAD_STATUS(status)) return;
}


 /*
  * p a r s e _ l i n e
  *
  * parse a string into token and value assuming its syntax is 
  * token = value 
  * 
  */
PRIVATE void parse_line
  (
   char *string,
   char **token,
   char **value
  )
{
    char *ptr;

    ptr = string;
    while (*ptr == ' ')
        ptr++;
    *token = ptr;
    while (*ptr != ' ' && *ptr != '=')
        ptr++;
    *ptr = '\0';
    ptr++;
    while (*ptr == ' ' || *ptr == '=')
        ptr++;
    *value = ptr;
    while (*ptr != '\n' && *ptr != '\0')
        ptr++;
    *ptr = '\0';
}

 /*
  * n e x t _ l i n e _ p v t
  *
  * get next input line from print file, skip deliminators
  * 
  */
PRIVATE char *next_line_pvt ( FILE *fp, unsigned32 len )
{
    char      *newline, *ptr;
    boolean32 good_line = FALSE;

    newline = (char *) malloc(len);
    while (!good_line) {
        if (fgets(newline, len, fp)) {
            ptr = newline;
	    if ( *ptr == '#' ) { /* comments started with # */
		continue;
	    }
            while (!good_line && *ptr != '\n' && *ptr != '\0') {
                if (*ptr != ' ' && *ptr != '=')
                    good_line = TRUE;
                ptr++;
            }
        }
        else
            break;
    }
    if (good_line)
        return(newline);
    else {
        free(newline);
        return(NULL);
    }

}

PRIVATE char *next_n_line ( FILE *fp, unsigned32 len )
{
    return next_line_pvt( fp, len );
}

PRIVATE char *next_line ( FILE *fp )
{
    return next_line_pvt( fp, MAX_LINE_LEN );
}


 /*
  * g e t _ t o w e r
  */
PRIVATE void get_tower
  (
    char *string,
    unsigned32 length,
    idl_byte tower[]
  )
{
    int i;
    unsigned int hold[TOWER_STRING_MAX_LEN];
    char         hold_chars[3], *ptr;

    hold_chars[2] = '\0';
    ptr = string;
    for (i = 0; i < length; i++) {
       hold_chars[0] = *ptr++;
       hold_chars[1] = *ptr++;
       sscanf((char *) hold_chars, "%x", &hold[i]); 
    }
    for (i = 0; i < length; i++) {
        tower[i] = (idl_byte) hold[i];
    }
}


 /*
  * c o n s t r u c t _ v e r s i o n _ c h e c k
  *
  * initialize for reconstruction
  *
  */
PUBLIC void construct_version_check ( error_status_t *status )
{
    FILE *fopen(), *fp;
    sec_rgy_pname_t prt_file;
    char *line, *token, *value;
    int version;

    CLEAR_STATUS(status);

    strcpy((char *) prt_file, (char *) prt_path);
    strcat((char *) prt_file, PRT_FILE_RGY_STATE);

    /* open rgy_state.prt file */
    fp = fopen((char *) prt_file, "r");

    if (fp) {
        while (line = next_line(fp)) {
           if (line) {
               parse_line(line, &token, &value); 
               if (strcmp(token, prt_Rgy_State_File_Version) == 0) {
                   sscanf(value, "%d", &version);
                   if (version != rs_c_state_file_version) {
			SET_STATUS(status, sec_rgy_rep_bad_db_version);
		       free(line);
		       break;
		   }
               }
           }
           free(line);
        }
        fclose(fp);
    }
    else {
	*status = sec_rgy_dir_not_found;
    }
}


 /*
  * c o n s t r u c t _ a c l
  *
  * based on the acl list built, construct acls entries
  * 
  */
PUBLIC void construct_acl
  (
    rec_acl_list_item_t *acl_item_ptr,
    error_status_t *status
  )
{
    rsdb_acl_string_t       *acl_str;
    rsdb_acl_t              *real_acl;
    rsdb_pvt_id_t           new_acl_id;
    rsdb_acl_entry_t        *acl_entry_ptr;
    sec_acl_printstring_t   printstrings[MAX_ACL_PRINTSTRINGS];
    unsigned32              total_pstrings;
    uuid_t                  mgr_type_chain, mgr_uuid;
    sec_acl_printstring_t   mgr_pstring;
    boolean32               tokenize, is_foreign = false;
    unsigned32              size_used;
    int                     i;
    error_status_t          local_status;

    CLEAR_STATUS(&local_status);

    real_acl = NEW(rsdb_acl_t);
    real_acl->default_realm.name_len = 0;
    acl_str = acl_item_ptr->acl;
    if (!uuid_is_nil(&acl_str->realm_uuid, status)) {
        real_acl->default_realm.uuid = acl_str->realm_uuid;
    }
    else if (strcmp((char *) acl_str->realm, 
                 (char *) properties.realm) == 0) {
             memcpy(&real_acl->default_realm.uuid, &properties.realm_uuid, 
                  sizeof(uuid_t));
         }
    else {
            /* error! */
    }
    rsdb_acl_get_mgr_uuid_from_type(acl_str->mgr_type, &mgr_uuid, status);
    memcpy(&real_acl->sec_acl_manager_type, &mgr_uuid, sizeof(uuid_t));
    sec_acl_mgr_get_printstring(acl_mgr_handle, 
        &mgr_uuid, MAX_ACL_PRINTSTRINGS, 
        &mgr_type_chain, &mgr_pstring, &tokenize, &total_pstrings, 
        &size_used, printstrings, status);
    if (GOOD_STATUS(status)) {
        for (i = 0; i < acl_str->num_entries; i++) {
	    real_acl->entries[i].entry_info.foreign_id.id.name_len = 0;
	    real_acl->entries[i].entry_info.foreign_id.realm.name_len = 0;
            rsdb_str_to_acl_entry(acl_str->entries[i], 
                       &real_acl->entries[i], is_foreign, 
                       printstrings, total_pstrings, &local_status);
	    if ( BAD_STATUS(&local_status) )
		*status = local_status;
        }
        real_acl->num_entries = acl_str->num_entries;
        real_acl->acl_len = sizeof(rsdb_acl_fixed_t) +
                (acl_str->num_entries * sizeof(rsdb_acl_entry_t));
        rsdb_acl_store_by_id(acl_item_ptr->acl_id, real_acl);
    }
    free(real_acl);
}


 /*
  * g e t _ a c l
  *
  * get acl value from print file and build an acl list
  * 
  */
PRIVATE void get_acl
  (
    FILE *fp,
    boolean32 store_acl,
    boolean32 uuid_flag,
    char *string,
    char *entry_token,
    rsdb_pvt_id_t acl_id,
    rsdb_acl_mgr_type_t type,
    error_status_t *status
  )
{
    int i;
    char *line, *token, *value;
    rec_acl_list_item_t *acl_item;
    rsdb_acl_string_t *acl;

    acl = NEW(rsdb_acl_string_t);
    acl->mgr_type = type;
    acl->realm[0] = '\0';
    uuid_create_nil(&acl->realm_uuid, status);
    acl_item = NEW(rec_acl_list_item_t);
    if (!uuid_flag) {
        strcpy((char *) acl->realm, string);
    }
    else {
        uuid_from_string((unsigned_char_t *) string, &acl->realm_uuid, status);
	if (BAD_STATUS(status)) return;
    }
    line = next_line(fp);
    if (line) {
        parse_line(line, &token, &value);
        if (strcmp(token, prt_Num_Acl_Entries) == 0) {
            sscanf(value, "%d", &acl->num_entries);
	free(line);
        }
    }
    for (i = 0; i < acl->num_entries; i++) {
        line = next_line(fp);
	acl->entries[i] = NULL;
        if (line) {
            parse_line(line, &token, &value); 
            if (strcmp(token, entry_token) == 0) {
               acl->entries[i] = (char *) malloc(MAX_SIZE_ACL_ENTRY_STRING);
               strcpy(acl->entries[i], value);
            }
            else
           	error_st(*status, "Unable to parse print file");
	    free(line);
        }
        else
            error_st(*status, "Unable to parse print file");
    }

    acl_item->acl_id = acl_id;
    acl_item->acl = acl;
    acl_item->ptr = NULL;

    if ( store_acl ) {
	construct_acl(acl_item, status);
	if ( BAD_STATUS(status) ) {
	    return;
	}
    }

    for (i = 0; i < acl_item->acl->num_entries; i++) {
	if ( acl_item->acl->entries[i] != NULL )
	    free(acl_item->acl->entries[i]);
    }
    free(acl_item->acl);
    free(acl_item);
}


 /*
  * g e t _ k e y
  *
  * get in encrypted DES key
  * 
  */
PRIVATE void get_key
  (
    char *string,
    long length,
    char *key
  )
{
    int i;
    unsigned int hold[rsdb_auth_encrypt_key_max_len];
    char         hold_chars[3], *ptr;

    hold_chars[2] = '\0';
    ptr = string;
    for (i = 0; i < length; i++) {
       hold_chars[0] = *ptr++;
       hold_chars[1] = *ptr++;
       sscanf((char *) hold_chars, "%x", &hold[i]); 
    }
    ptr = key;
    for (i = 0; i < length; i++) {
        *key++ = (char) hold[i];
    }
}


 /*
  * s a v e _ m k e y
  *
  * save master key file
  * 
  */
PUBLIC void save_mkey
    ( 
     rsdb_auth_key_t *mkey,
     error_status_t *status
    ) 
{ 
    sec_rgy_name_t old_master_key_file;

    CLEAR_STATUS(status);

    strcpy((char *) old_master_key_file, (char *) MASTER_KEY_FILE);
    strcat((char *) old_master_key_file, ".old");

    rsdb_mkey_rename(MASTER_KEY_FILE, (char *) old_master_key_file, status);
    /* ok if this fails */

    rsdb_mkey_create(MASTER_KEY_FILE, mkey, status);
}


 /*
  * c o n s t r u c t _ d i r
  *
  * construct directory data in rgy database from its print file 
  * 
  */
PRIVATE boolean32 construct_dir
    ( 
     FILE *fp,
     sec_rgy_domain_t domain,
     boolean32        do_acl,
     error_status_t *status
    ) 
{ 
    char *line, *token, *value;
    sec_rgy_name_t name;
    rsdb_named_item_t   item;
    rsdb_acl_mgr_type_t pgo_type;

    /* find out a dir of which pgo */
    if (domain == sec_rgy_domain_person)
        pgo_type = rsdb_acl_mgr_type_person;
    else if (domain == sec_rgy_domain_group)
        pgo_type = rsdb_acl_mgr_type_group;
    else 	/* org */
        pgo_type = rsdb_acl_mgr_type_org;

    while (line = next_line(fp)) {
       if (line) {
           parse_line(line, &token, &value); 
	   if (strcmp(token, prt_Record_Number) == 0) {
	       free(line);
 	       return TRUE; /* for more processing */
	   }
	   else
           if (strcmp(token, prt_Name) == 0) {
               strcpy((char *) name, value);
	       if ( strcmp((char *)name, (char *)root_name_keys[domain].name) 
		   == 0 ) {
	       /* a root dir had been created as default, get this item */
		   if ( !rsdb_util_get_item_by_id(domain, START_OF_LIST, &item) ){
		       SET_STATUS(status, sec_rgy_dir_not_found);
		       error_st(*status, "Unable to get root directory item");
		   }
	       }
	       else { /* create the directory */
		   if ( ! do_acl ) {
		       rsdb_dir_create(domain, name, status);
		       if (BAD_STATUS(status))
		           error_st(*status, "Unable to create dir");
		   }
		   rsdb_util_get_by_name(domain, name, strlen((char *)name), 
					 &item);
	       }
           }
           else if (strcmp(token, prt_Obj_Acl_Def_Cell_Name) == 0) {
               get_acl(fp, do_acl, FALSE, value, prt_Obj_Acl_Entry, 
                       (RSDB_DIR(&item)).acl,  rsdb_acl_mgr_type_dir,
		       status);
               if (BAD_STATUS(status))
                   error_st(*status, "Unable to get directory acl");
           }
           else if (strcmp(token, prt_Obj_Acl_Def_Cell_UUID) == 0) {
               get_acl(fp, do_acl, TRUE, value, prt_Obj_Acl_Entry,
                       (RSDB_DIR(&item)).acl, rsdb_acl_mgr_type_dir,
		       status);
               if (BAD_STATUS(status))
                   error_st(*status, "Unable to get directory acl");
           }
           else if (strcmp(token, prt_Init_Obj_Acl_Def_Cell_Name) == 0) {
               get_acl(fp, do_acl, FALSE, value, prt_Init_Obj_Acl_Entry, 
                       (RSDB_DIR(&item)).def_obj_acl, pgo_type, status);
               if (BAD_STATUS(status))
                   error_st(*status, "Unable to get directory acl");
           }
           else if (strcmp(token, prt_Init_Obj_Acl_Def_Cell_UUID) == 0) {
               get_acl(fp, do_acl, TRUE, value, prt_Init_Obj_Acl_Entry,
                       (RSDB_DIR(&item)).def_obj_acl, pgo_type, status);
               if (BAD_STATUS(status))
                   error_st(*status, "Unable to get directory acl");
           }
           else if (strcmp(token, prt_Init_Cont_Acl_Def_Cell_Name) == 0) {
               get_acl(fp, do_acl, FALSE, value, prt_Init_Cont_Acl_Entry, 
                       (RSDB_DIR(&item)).def_dir_acl, rsdb_acl_mgr_type_dir,
		       status);
               if (BAD_STATUS(status))
                   error_st(*status, "Unable to get directory acl");
           }
           else if (strcmp(token, prt_Init_Cont_Acl_Def_Cell_UUID) == 0) {
               get_acl(fp, do_acl, TRUE, value, prt_Init_Cont_Acl_Entry,
                       (RSDB_DIR(&item)).def_dir_acl, rsdb_acl_mgr_type_dir,
		       status);
               if (BAD_STATUS(status))
                   error_st(*status, "Unable to get directory acl");
           }
	   free(line);
        }
    }
}


PUBLIC void clean_princ_list ()
{
    rec_princ_list_item_t       *ptr=NULL;
    rec_princ_list_item_t       *old_ptr;

    /* clean up principal uuid table */
    ptr = head_princ->next;
    while ( ptr != NULL ) {
	old_ptr = ptr;
	ptr = ptr->next;
	free(old_ptr->name);
        free(old_ptr);
    }
    free(head_princ);

}


 /*
  * c o n s t r u c t _ a c c t 
  *
  * construct account 
  * 
  */
PRIVATE void construct_acct
  (
    rec_acct_list_item_t    *acurr,
    error_status_t *status
  )
{
    krb5_keyblock               auth_key;
    rec_acct_authkey_item_t     *auth_keys_ptr;
    rec_acct_authkey_item_t     *auth_keys_prev;
    sec_passwd_rec_t            *passwd_ptr;

    if ( PASSWD_TYPE(&acurr->passwd) == sec_passwd_des ) {
        strncpy((char *)DES_PASSWD(&acurr->passwd), 
                (char *)acurr->ckey.contents, acurr->ckey.length);
    }

    if ( acurr->creator_name != NULL )
        acurr->admin_part.creator.principal = 
            get_principal_uuid(acurr->creator_name);
    if ( acurr->changer_name != NULL )
        acurr->admin_part.last_changer.principal = 
            get_principal_uuid(acurr->changer_name);

    rsdb_acct_add(&acurr->login_name, acurr->key_parts, 
                  &acurr->user_part, 
                  &acurr->admin_part, &acurr->passwd,
                  acurr->gen_unix_passwd, &acurr->passwd_version_number,
                  status);
    if (BAD_STATUS(status))
        error_st(*status, "Unable to add acct");

    /* add policy information to the account just created */
    rsdb_acct_auth_policy_set_info(&acurr->login_name, 
                                   &acurr->auth_policy, status);
    if (BAD_STATUS(status))
        error_st(*status, "Unable to set account policy");

    /* add back version authkeys, if any */
    auth_keys_ptr = acurr->auth_keys;
    while ( auth_keys_ptr != NULL ) {
        passwd_ptr=&(auth_keys_ptr->passwd);
        switch (PASSWD_TYPE(passwd_ptr)) {
        case sec_passwd_des:
            strncpy((char *)DES_PASSWD(passwd_ptr), 
                    (char *)auth_keys_ptr->dkey.contents, 
                    auth_keys_ptr->dkey.length);
                    break;
        case sec_passwd_pubkey:
            PUBKEY_PASSWD_DATA(passwd_ptr)=(idl_char *)malloc(auth_keys_ptr->dkey.length);
            if (PUBKEY_PASSWD_DATA(passwd_ptr) == NULL) {
                fprintf(stderr, "Out of memory\n");
                *status=sec_s_no_memory;
            } else {
                PUBKEY_PASSWD_LEN(passwd_ptr)=auth_keys_ptr->dkey.length;
                u_strncpy(PUBKEY_PASSWD_DATA(passwd_ptr),
                        auth_keys_ptr->dkey.contents,
                        auth_keys_ptr->dkey.length);
            }
            break;
        default:
            fprintf(stderr, "Key type not supported\n");
            *status=ek_err_passwd_invalid;
            break;
        }
        if (GOOD_STATUS(status))
            rsdb_acct_add_key_version(&acurr->login_name, passwd_ptr,
                                      FALSE, &auth_keys_ptr->expiration_date, 
                                      status);
        if (BAD_STATUS(status))
            error_st(*status, "Unable to add back auth key");
        auth_keys_ptr = auth_keys_ptr->next;
    }
}


 /*
  * c o n s t r u c t _ m e m b e r
  *
  * construct member base on the list built
  * 
  */
PUBLIC void construct_member
  (
    error_status_t *status
  )
{
    rec_member_list_item_t *mp;
    rec_member_list_item_t *mcurr;

    mp = head_member;

    while ( (mcurr=mp->next) != NULL ) {
        free (mp);
	if ( mcurr->cell_uuidp != NULL && mcurr->princ_uuidp != NULL ) {
	     rs_util_pgo_add_foreign_principal(mcurr->name,
                  mcurr->cell_uuidp, mcurr->princ_uuidp,
                  status);
	     if ( BAD_STATUS(status) ) {
	    	error_st(*status, "Unable to add foreign member to pgo");
	     }
	}

        rsdb_pgo_add_member(mcurr->domain,
            (unsigned_char_p_t) mcurr->domain_name,
            (unsigned_char_p_t) mcurr->name, status);
	if (BAD_STATUS(status))
	    error_st(*status, "Unable to add member to pgo");
	if ( mcurr->name != NULL )
	    free(mcurr->name);
	if ( mcurr->domain_name != NULL )
	    free(mcurr->domain_name);
	if ( mcurr->cell_uuidp != NULL )
	    free(mcurr->cell_uuidp);
	if ( mcurr->princ_uuidp != NULL )
	    free(mcurr->princ_uuidp);
	mp = mcurr;
    }
    free (mp);
}


 /*
  * c o n s t r u c t _ a u t h k e y s
  *
  * construct authkey from print file
  * 
  */
PRIVATE void construct_authkeys
  (
    FILE                    *fp,
    krb5_keyblock           *ckey,
    sec_passwd_rec_t        *passwd,
    rec_acct_authkey_item_t **auth_keys,
    error_status_t          *status
  )
{
    char                    *line, *value, *token;
    int                     num_auth_keys = 0;
    int                     num_key_stored = 0;
    sec_passwd_rec_t        *passwd_ptr; 
    rec_acct_authkey_item_t *auth_keys_ptr;
    krb5_keyblock           *dkey_ptr;

    ckey->contents = NULL;
    ckey->keytype = RSDB_AUTH_DEFAULT_KEY_TYPE;
    passwd_ptr = passwd;
    dkey_ptr = ckey;
    auth_keys_ptr = NEW(rec_acct_authkey_item_t);

    while (line = next_line(fp)) {
        if (line) {
            parse_line(line, &token, &value); 
            if (strcmp(token, prt_Num_Auth_Keys) == 0) {
                sscanf(value, "%d", &num_auth_keys);
            }
            else
                if (strcmp(token, prt_Auth_Key_Version) == 0) {
                    if ( num_key_stored >= 1 ) {
                        /* more than one, there is back version auth key */
                        if ( num_key_stored == 1 ) {
                            /* init auth_key for return */
                            *auth_keys = auth_keys_ptr;
                            auth_keys_ptr->next = NULL;
                        }
                        else {
                            /* more than one back version key, put on list */
                            *auth_keys = NEW(rec_acct_authkey_item_t);
                            (*auth_keys)->next = auth_keys_ptr;
                            auth_keys_ptr = *auth_keys;
                        }
                        passwd_ptr = &auth_keys_ptr->passwd;
                        PASSWD_TYPE(passwd_ptr) = sec_passwd_des;
                        dkey_ptr = &auth_keys_ptr->dkey;
                        dkey_ptr->keytype = RSDB_AUTH_DEFAULT_KEY_TYPE;
                    }
                    sscanf(value, "%d", &passwd_ptr->version_number);
                }
                else if (strcmp(token, prt_Auth_Key_Type) == 0) {
                    if (strcmp(value, prt_des) == 0)
                        PASSWD_TYPE(passwd_ptr) = RSDB_AUTH_DEFAULT_KEY_TYPE;
                    else if (strcmp(value, prt_keytype_encrypt_rsa_priv) == 0)
                        PASSWD_TYPE(passwd_ptr) = sec_passwd_pubkey;
                }
                else if (strcmp(token, prt_Auth_Key_Pepper) == 0) {
                    passwd_ptr->pepper = (idl_char *)malloc(rsdb_auth_pepper_size);
                    strcpy((char *)passwd_ptr->pepper, value);
                }
                else if (strcmp(token, prt_Auth_Key_Len) == 0) {
                    sscanf(value, "%d", &dkey_ptr->length);
                    free(line);
                    line = next_n_line(fp, (dkey_ptr->length*2)+MAX_LINE_LEN);
                    parse_line(line, &token, &value);
                    if (strcmp(token, prt_Auth_Key) == 0) {
                        dkey_ptr->contents = (krb5_octet *)malloc(dkey_ptr->length);
                        get_key(value, dkey_ptr->length, (char *)dkey_ptr->contents);
                    }
                        
                }
                else if (strcmp(token, prt_Auth_Key_Expire_Time) == 0) {
                    if ( num_key_stored == 0 && (strcmp(value, prt_none) != 0) )  {
                        error_st(*status, "Expected Auth_Key_Expire_Time = none");
                    }
                    else {
                        dt_to_sec(value, &auth_keys_ptr->expiration_date);
                    }
                    num_key_stored++;
                    if (num_key_stored == num_auth_keys) {
                        if (num_key_stored == 1) {
                            free(auth_keys_ptr);
                        }
                        free(line);
                        return;
                    }
                }
            free(line);
        }
    }
}


 /*
  * c o n s t r u c t _ c r e a t o r _ c h a n g e r
  *
  * construct creator and changer from print file
  * 
  */
PRIVATE void construct_creator_changer
  (
    FILE *fp,
    sec_rgy_acct_admin_t *admin_item,
    unsigned_char_p_t       *creator_name,
    unsigned_char_p_t       *changer_name,
    error_status_t *status
  )
{
    char    *token, *value, *line;
    uuid_t  uuid;

    *creator_name = *changer_name = NULL;

    while (line = next_line(fp)) {
       if (line) {
           parse_line(line, &token, &value); 
           if (strcmp(token, prt_Acct_Creator_Name) == 0) {
               *creator_name = (unsigned_char_p_t )dce_strdup(value);
	       admin_item->creator.cell = properties.realm_uuid;
	   }
	   else
           if (strcmp(token, prt_Acct_Creator_Cell_UUID) == 0) {
	       uuid_from_string((unsigned char *)value, 
                                 &(admin_item->creator.cell), status);
	   }
	   else
           if (strcmp(token, prt_Acct_Creator_Person_UUID) == 0) {
	       uuid_from_string((unsigned char *)value, 
                                 &(admin_item->creator.principal), status);
	   }
	   else
           if (strcmp(token, prt_Acct_Creation_Time) == 0) {
	       dt_to_sec(value, &admin_item->creation_date);
	   }
	   else
           if (strcmp(token, prt_Acct_Changer_Name) == 0) {
               *changer_name = (unsigned_char_p_t )dce_strdup(value);
	       admin_item->last_changer.cell = properties.realm_uuid;
	   }
	   else
           if (strcmp(token, prt_Acct_Changer_Cell_UUID) == 0) {
	       uuid_from_string((unsigned char *)value, 
                                 &(admin_item->last_changer.cell), status);
	   }
	   else
           if (strcmp(token, prt_Acct_Creator_Person_UUID) == 0) {
	       uuid_from_string((unsigned char *)value, 
                                 &(admin_item->last_changer.principal), status);
	   }
	   else
           if (strcmp(token, prt_Acct_Change_Time) == 0) {
	       dt_to_sec(value, &admin_item->change_date);
	       free(line);
	       return;
	   }
	   free(line);
       }
    }
}


 /*
  * c o n s t r u c t _ a c c t
  *
  * get account data from print file
  * 
  */
PRIVATE void get_acct
  (
    char *line,
    FILE *fp,
    boolean32      do_acl,
    sec_rgy_name_t name,
    char *group_name,
    sec_rgy_pgo_item_t *item,
    error_status_t *status
  )
{
    char *token, *value;
    long name_len;
    rec_acct_list_item_t        *acct_item;
    rec_acct_authkey_item_t     *auth_keys_ptr;
    rec_acct_authkey_item_t     *auth_keys_prev;
    sec_passwd_rec_t            *passwd_ptr;
    int i;

    /* initialization */
    acct_item = NEW(rec_acct_list_item_t);
    PLAIN_PASSWD(&acct_item->passwd) = NULL;
    acct_item->passwd.pepper = NULL;
    acct_item->gen_unix_passwd = FALSE;
    acct_item->auth_keys = NULL;
    acct_item->ckey.contents = NULL;
    acct_item->admin_part.flags = 0;
    acct_item->admin_part.authentication_flags = 0;
    acct_item->user_part.flags = 0;
    PASSWD_TYPE(&acct_item->passwd) = sec_passwd_des; 
    acct_item->passwd_version_number = 0;
    strcpy((char *)acct_item->login_name.pname, (char *)name);
    strcpy((char *)acct_item->login_name.gname, group_name);

    while (line = next_line(fp)) {
       if (line) {
           parse_line(line, &token, &value); 
           if (strcmp(token, prt_Acct_Org_Name) == 0) {
	       strcpy((char *)acct_item->login_name.oname, value);
	       construct_creator_changer(fp, &acct_item->admin_part, 
					 &acct_item->creator_name,
					 &acct_item->changer_name, status);
	   }
	   else
           if (strcmp(token, prt_Acct_Expire_Time) == 0) {
	       dt_to_sec(value, &acct_item->admin_part.expiration_date);
	   }
	   else
           if (strcmp(token, prt_Acct_Good_Since_Time) == 0) {
	       dt_to_sec(value, &acct_item->admin_part.good_since_date);
	   }
	   else
           if (strcmp(token, prt_Acct_Valid_For_Login_Flag) == 0) {
	       if ( strcmp(value, prt_true) == 0 )
		   acct_item->admin_part.flags |= sec_rgy_acct_admin_valid;
	   }
	   else
           if (strcmp(token, prt_Acct_Valid_As_Server_Flag) == 0) {
	       if ( strcmp(value, prt_true) == 0 )
		   acct_item->admin_part.flags |= sec_rgy_acct_admin_server;
	   }
	   else
           if (strcmp(token, prt_Acct_Valid_As_Client_Flag) == 0) {
	       if ( strcmp(value, prt_true) == 0 )
		   acct_item->admin_part.flags |= sec_rgy_acct_admin_client;
	   }
	   else
           if (strcmp(token, prt_Acct_Post_Dated_Cert_Ok_Flag) == 0) {
	       if ( strcmp(value, prt_true) == 0 )
		   acct_item->admin_part.authentication_flags |= 
		          sec_rgy_acct_auth_post_dated;
	   }
	   else
           if (strcmp(token, prt_Acct_Forwardable_Cert_Ok_Flag) == 0) {
	       if ( strcmp(value, prt_true) == 0 )
		   acct_item->admin_part.authentication_flags |= 
		          sec_rgy_acct_auth_forwardable;
	   }
	   else
           if (strcmp(token, prt_Acct_TGT_Auth_Cert_Ok_Flag) == 0) {
	       if ( strcmp(value, prt_true) == 0 )
		   acct_item->admin_part.authentication_flags |= 
		          sec_rgy_acct_auth_tgt;
	   }
	   else
           if (strcmp(token, prt_Acct_Renewable_Cert_Ok_Flag) == 0) {
	       if ( strcmp(value, prt_true) == 0 )
		   acct_item->admin_part.authentication_flags |= 
		          sec_rgy_acct_auth_renewable;
	   }
	   else
           if (strcmp(token, prt_Acct_Proxiable_Cert_Ok_Flag) == 0) {
	       if ( strcmp(value, prt_true) == 0 )
	          acct_item->admin_part.authentication_flags |= 
		          sec_rgy_acct_auth_proxiable;
	   }
	   else
           if (strcmp(token, prt_Acct_Dup_Session_Key_Ok_Flag) == 0) {
	       if ( strcmp(value, prt_true) == 0 )
		   acct_item->admin_part.authentication_flags |= 
		          sec_rgy_acct_auth_dup_skey;
	   }
	   else
           if (strcmp(token, prt_Unix_Key) == 0) {
	   /* Encryped UNIX passwd is informational, but can not 
	    * be applied to rgy data reconstruction.
	    */
	       strcpy((char *)acct_item->user_part.passwd, value);
	       PASSWD_TYPE(&acct_item->passwd) = sec_passwd_des;
	   }
	   else
           if (strcmp(token, prt_Plaintext_Passwd) == 0) {
	       if (strcmp(value, prt_Null) != 0) {
		   PLAIN_PASSWD(&acct_item->passwd) = 
                       (unsigned_char_p_t )dce_strdup(value);
		   PASSWD_TYPE(&acct_item->passwd) = sec_passwd_plain;
		   acct_item->gen_unix_passwd = TRUE;
	       }
	   }
	   else
           if (strcmp(token, prt_Home_Dir) == 0) {
	       strcpy((char *)acct_item->user_part.homedir, value);
	   }
	   else
           if (strcmp(token, prt_Shell) == 0) {
	       strcpy((char *)acct_item->user_part.shell, value);
	   }
	   else
           if (strcmp(token, prt_Gecos) == 0) {
	       strcpy((char *)acct_item->user_part.gecos, value);
	   }
	   else
           if (strcmp(token, prt_Passwd_Valid_Flag) == 0) {
	       if ( strcmp(value, prt_true) == 0 )
		   acct_item->user_part.flags |= sec_rgy_acct_user_passwd_valid;
	   }
	   else
           if (strcmp(token, prt_Passwd_Change_Time) == 0) {
  	       dt_to_sec(value, &acct_item->user_part.passwd_dtm);
	   }
	   else
           if (strcmp(token, prt_Max_Certificate_Lifetime) == 0) {
	       if (strcmp(value, prt_default_policy) == 0)
		   acct_item->auth_policy.max_ticket_lifetime == 
		       RSDB_DEF_AUTH_POL_MAX_TKT_LIFE;
	       else
		   sscanf(value, "%d", 
			  &acct_item->auth_policy.max_ticket_lifetime);
	   }
	   else
           if (strcmp(token, prt_Max_Renewable_Lifetime) == 0) {
	       if (strcmp(value, prt_default_policy) == 0)
		   acct_item->auth_policy.max_renewable_lifetime =
		       RSDB_DEF_AUTH_POL_MAX_RENEW_LIFE;
	       else
		   sscanf(value, "%d", 
			  &acct_item->auth_policy.max_renewable_lifetime);
	   }
	   else
	   if (strcmp(token, prt_Master_Key_Version) == 0) {
	       sscanf(value, "%d", &i);
	       acct_item->mkey_version = i;
	       construct_authkeys(fp, &acct_item->ckey, &acct_item->passwd, 
				  &acct_item->auth_keys, status);
	       acct_item->key_parts = sec_rgy_acct_key_person;

	       if ( do_acl ) {
	           construct_acct(acct_item, status);
		   if ( BAD_STATUS(status) )
			return;
	       }
               if ((PASSWD_TYPE(&acct_item->passwd) == sec_passwd_plain) 
                   && (PLAIN_PASSWD(&acct_item->passwd) != NULL)) {
	           free(PLAIN_PASSWD(&acct_item->passwd));
               }
               if (acct_item->passwd.pepper != NULL) {
                   free(acct_item->passwd.pepper);
               }
               if (acct_item->creator_name != NULL) {
                   free(acct_item->creator_name);
               }
               if (acct_item->changer_name != NULL) {
                   free(acct_item->changer_name);
               }
               if (acct_item->ckey.contents != NULL) {
                   free(acct_item->ckey.contents);
               }

               auth_keys_ptr = acct_item->auth_keys;
               while (auth_keys_ptr != NULL) {
                   if (auth_keys_ptr->dkey.contents != NULL)
                       free(auth_keys_ptr->dkey.contents);
                   passwd_ptr=&auth_keys_ptr->passwd;
                   if (PASSWD_TYPE(passwd_ptr) == sec_passwd_pubkey) {
                       if (PUBKEY_PASSWD_DATA(passwd_ptr))
                           free(PUBKEY_PASSWD_DATA(passwd_ptr));
                       PUBKEY_PASSWD_DATA(passwd_ptr)=NULL;
                       PUBKEY_PASSWD_LEN(passwd_ptr)=0;
                   }
                   if (auth_keys_ptr->passwd.pepper != NULL)
                       free(auth_keys_ptr->passwd.pepper);
                   auth_keys_prev = auth_keys_ptr;
                   auth_keys_ptr = auth_keys_ptr->next;
                   free(auth_keys_prev);
               }
	       free(acct_item);
	       free(line);
	       return;
	   }

	   free(line);

        }
    }
}


 /*
  * c o n s t r u c t _ p a s s w d _ p o l i c y
  *
  * construct account passwd policy from print file
  * 
  */
PRIVATE void construct_passwd_policy
  (
    char *line,
    FILE *fp,
    sec_rgy_name_t name
  )
{
    char           *token, *value;
    sec_rgy_plcy_t org_policy;
    error_status_t status;

    parse_line(line, &token, &value); 
    if ( strcmp(token, prt_Acct_Lifetime) == 0 )
        dt_to_period(value, &org_policy.acct_lifespan);
    free(line);

    line = next_line(fp);
    parse_line(line, &token, &value); 
    if ( strcmp(token, prt_Passwd_Min_Len) == 0 )
        sscanf(value, "%d", &org_policy.passwd_min_len);
    free(line);

    line = next_line(fp);
    parse_line(line, &token, &value); 
    if ( strcmp(token, prt_Passwd_Lifetime) == 0 )
        dt_to_period(value, &org_policy.passwd_lifetime);
    free(line);

    line = next_line(fp);
    parse_line(line, &token, &value); 
    if ( strcmp(token, prt_Passwd_Expire_Date) == 0 )
        dt_to_sec(value, &org_policy.passwd_exp_date);
    free(line);

    org_policy.passwd_flags = sec_rgy_plcy_pwd_flags_none;
    line = next_line(fp);
    parse_line(line, &token, &value); 
    if ( strcmp(token, prt_Passwd_All_Spaces_Ok) == 0 )
        if ( strcmp(value, prt_false) == 0 )
	    org_policy.passwd_flags |= sec_rgy_plcy_pwd_no_spaces;
    free(line);

    line = next_line(fp);
    parse_line(line, &token, &value); 
    if ( strcmp(token, prt_Passwd_All_Alphanumeric_Ok) == 0 )
        if ( strcmp(value, prt_false) == 0 )
	    org_policy.passwd_flags |= sec_rgy_plcy_pwd_non_alpha;
    free(line);

    rsdb_policy_set_info(name, &org_policy, &status);
    if (BAD_STATUS(&status))
        error_st(status, "Unable to set Org policy");
}


 /*
  * c o n s t r u c t _ a t t r _ v a l u e
  *
  * construct attribute values
  * 
  */
PRIVATE boolean32 construct_attr_value
    ( 
     FILE 			*fp,
     sec_attr_value_t           *attr_value_p,
     error_status_t          	*status
    )
{
    char    *line, *token, *value;
    int     i;

    line = next_line(fp);
    parse_line(line, &token, &value);

    if ( strcmp(token, prt_Attr_Value_Type) != 0 ) {
	fprintf(stderr, "Invalid token string - %s, %s expected\n", token, 
			prt_Attr_Value_Type);
	return FALSE;
    }

    if ( strcmp(value, prt_Attr_Value_Any) == 0 ) {
	attr_value_p->attr_encoding = sec_attr_enc_any;
    }
    else
    if ( strcmp(value, prt_Attr_Value_Void) == 0 ) {
	attr_value_p->attr_encoding = sec_attr_enc_void;
    }
    else
    if ( strcmp(value, prt_Attr_Value_Integer) == 0 ) {
	attr_value_p->attr_encoding = sec_attr_enc_integer;
	free(line);
    	line = next_line(fp);
    	parse_line(line, &token, &value);
	if ( strcmp(token, prt_Attr_Value) == 0 ) {
	    sscanf(value, "%d", &attr_value_p->tagged_union.signed_int);
	}
	else {
	    fprintf(stderr, "Invalid token string - %s, %s expected\n", value, 
		prt_Attr_Value);
	}
    }
    else
    if ( strcmp(value, prt_Attr_Value_Printstring) == 0 ) {
	char        *p;
	attr_value_p->attr_encoding = sec_attr_enc_printstring;
	free(line);
    	line = next_line(fp);
    	parse_line(line, &token, &value);
	if ( strcmp(token, prt_Attr_Value) == 0 ) {
	    p = malloc(strlen(value)+1);
	    attr_value_p->tagged_union.printstring = (sec_attr_enc_printstring_p_t) p; 
	    strcpy(p, value);
	}
	else {
	    fprintf(stderr, "Invalid token string - %s, %s expected\n", value, 
		prt_Attr_Value);
	}
    }
    else
    if ( strcmp(value, prt_Attr_Value_Printstring_Array) == 0 ) {
	sec_attr_enc_str_array_t	*str_array_p;
	unsigned32                      num_str_array;
	char                            *p;
	attr_value_p->attr_encoding = sec_attr_enc_printstring_array;
	free(line);
    	line = next_line(fp);
	parse_line(line, &token, &value);
	if ( strcmp(token, prt_Num_Attr_Printstring_Array_Entries) == 0 ) {
	    sscanf(value, "%d", &num_str_array);
	    str_array_p = attr_value_p->tagged_union.string_array =
		   (sec_attr_enc_str_array_t *) malloc(sizeof(unsigned32)+
		   num_str_array*sizeof(sec_attr_enc_printstring_p_t));
	    str_array_p->num_strings = num_str_array;
	    for ( i=0; i<num_str_array; i++ ) {
		free(line);
    		line = next_line(fp);
		parse_line(line, &token, &value);
		if ( strcmp(token, prt_Attr_Value) == 0 ) {
		    p = malloc(strlen(value)+1);
	    	    str_array_p->strings[i] = (sec_attr_enc_printstring_p_t) p;
	   	    strcpy(p, value);
		}
		else {
	    	    fprintf(stderr, "Invalid token string - %s, %s expected\n", value, 
			prt_Attr_Value);
		}
	    }
	}
	else {
	    fprintf(stderr, "Invalid token string - %s, %s expected\n", token,
		prt_Num_Attr_Printstring_Array_Entries);
	    free(line);
	    return FALSE;
	}
    }
    else
    if ((( strcmp(value, prt_Attr_Value_Encoding_Byte) == 0 ) ||
        ( strcmp(value, prt_Attr_Value_Confidential_Byte) == 0 ))) {

	if ( strcmp(value, prt_Attr_Value_Encoding_Byte) == 0 )
	    attr_value_p->attr_encoding = sec_attr_enc_bytes;
	else
	    attr_value_p->attr_encoding = sec_attr_enc_confidential_bytes;

	free(line);
    	line = next_line(fp);
	parse_line(line, &token, &value);
	if ( strcmp( token, prt_Attr_Value) == 0 ) {
	    attr_value_p->tagged_union.bytes = (sec_attr_enc_bytes_t *) malloc(sizeof(unsigned32)+strlen(value));
	    attr_value_p->tagged_union.bytes->length = strlen(value);
	    strncpy((char *)attr_value_p->tagged_union.bytes->data, value, attr_value_p->tagged_union.bytes->length);
	}
	else {
	    fprintf(stderr, "Invalid token string - %s, %s expected\n", value, 
		prt_Attr_Value);
	}
    }
    else
    if ( strcmp(value, prt_Attr_Value_I18n_Data) == 0 ) {
	unsigned32		codeset;
	attr_value_p->attr_encoding = sec_attr_enc_i18n_data;
	free(line);
    	line = next_line(fp);
	parse_line(line, &token, &value);
	if ( strcmp(token, prt_Attr_I18n_Codeset) != 0 ) {
	    fprintf(stderr, "Invalid token string - %s, %s expected\n", value, 
		prt_Attr_I18n_Codeset);
	    return FALSE;
	}
	sscanf(value, "%d", &codeset);
	free(line);
    	line = next_line(fp);
	parse_line(line, &token, &value);
	if ( strcmp( token, prt_Attr_Value) == 0 ) {
	    attr_value_p->tagged_union.idata = (sec_attr_i18n_data_t *) malloc(
		2*sizeof(unsigned32)+strlen(value));
	    attr_value_p->tagged_union.idata->length = strlen(value);
	    attr_value_p->tagged_union.idata->codeset = codeset;
	    strncpy((char *)attr_value_p->tagged_union.idata->data, value, attr_value_p->tagged_union.idata->length);
	}
	else {
	    fprintf(stderr, "Invalid token string - %s, %s expected\n", value, 
		prt_Attr_Value);
	    return FALSE;
	}
    }
    else
    if ( strcmp(value, prt_Attr_Value_UUID) == 0 ) {
	attr_value_p->attr_encoding = sec_attr_enc_uuid;
	free(line);
    	line = next_line(fp);
    	parse_line(line, &token, &value);
	if ( strcmp(token, prt_Attr_Value) == 0 ) {
	    uuid_from_string((unsigned_char_t *)value, &attr_value_p->tagged_union.uuid, status);
	    if ( BAD_STATUS(status) ) {
		error_st(*status, " Cannot convert uuid from string");
	    }
	}
	else {
	    fprintf(stderr, "Invalid token string - %s, %s expected\n", value, 
		prt_Attr_Value);
	}
    }
    else
    if ( strcmp(value, prt_Attr_Value_Set) == 0 ) {
	unsigned32		num_attr_sets;
	attr_value_p->attr_encoding = sec_attr_enc_attr_set;
	free(line);
    	line = next_line(fp);
    	parse_line(line, &token, &value);
	if ( strcmp(token, prt_Num_Attr_Set) == 0 ) {
	    sscanf(value, "%d", &num_attr_sets);
	    attr_value_p->tagged_union.attr_set = (sec_attr_enc_attr_set_t *) 
			malloc(sizeof(unsigned32)+
		num_attr_sets*(sizeof(uuid_t)));
	    attr_value_p->tagged_union.attr_set->num_members = num_attr_sets;
	    for ( i=0; i<num_attr_sets; i++ ) {
		free(line);
    		line = next_line(fp);
    		parse_line(line, &token, &value);
		if ( strcmp(token, prt_Attr_Value) == 0 ) {
		    uuid_from_string((unsigned_char_t *) value, 
			&attr_value_p->tagged_union.attr_set->members[i], status);
		    if ( BAD_STATUS(status) ) {
			error_st(*status, " Cannot convert uuid from string");
		    }
		}
		else {
	    	    fprintf(stderr, "Invalid token string - %s, %s expected\n", 
			value, prt_Attr_Value);
		}
	    }
	}
	else {
	    fprintf(stderr, "Invalid token string - %s, %s expected\n", value, 
		prt_Attr_Value_Set);
	    return FALSE;
	}	
    }
    else
    if ( strcmp(value, prt_Attr_Value_Binding) == 0 ) {
	attr_value_p->attr_encoding = sec_attr_enc_binding;
	free(line);
    	line = next_line(fp);
    	parse_line(line, &token, &value);
	if ( strcmp(token, prt_Attr_Bind_Auth_Type) == 0 ) {
	    value = construct_attr_binding(fp, value, &attr_value_p->tagged_union.binding);
	}
	else {
	    fprintf(stderr, "Invalid token string - %s, %s expected\n", value, 
		prt_Attr_Bind_Auth_Type);
	    return FALSE;
	}
    }
    else
	fprintf(stderr, "Invalid attribute valute type - %s\n", value);

    free(line);
}


 /*
  * c o n s t r u c t _ a t t r _ l i s t
  *
  * construct attribute list for attr values
  * 
  */
PRIVATE boolean32 construct_attr_list
    ( 
     FILE 			*fp,
     unsigned32                 num_attrs,
     sec_attr_t			**attrs,
     error_status_t          	*status
    )
{
    char 			*line, *token, *value;
    uuid_t                      attr_uuid;
    rsdb_attr_schema_entry_t    *sch_entry_p;
    sec_attr_t                  *attr_values_p;
    int                         i;

    *attrs = attr_values_p = malloc(num_attrs*sizeof(sec_attr_t));
    for ( i=0; i<num_attrs; i++ ) {
	line=next_line(fp);
	parse_line(line, &token, &value);
	if ( strcmp (token, prt_Attr_UUID) != 0 ) {
	    fprintf(stderr, "Invalid Attr entry - %s, %s expected\n",
	   	token, prt_Attr_UUID);
	    return FALSE;
	}
        uuid_from_string((unsigned_char_t *) value, &attr_uuid, status);
	if ( BAD_STATUS(status) ) {
	    error_st(*status, "Cannot convert uuid to string");
	}
	memcpy(&attr_values_p->attr_id, &attr_uuid, sizeof(uuid_t));
	
	/* get attr value */
	construct_attr_value(fp, &attr_values_p->attr_value, status);
	attr_values_p++;
	free(line);
    }
}


 /*
  * c o n s t r u c t _ p g o
  *
  * construct principal/group/org from print file
  * 
  */
PRIVATE boolean32 construct_pgo
    ( 
     FILE *fp,
     sec_rgy_domain_t   domain,
     boolean32          do_acl,
     sec_rgy_name_t     name,
     sec_rgy_pgo_item_t *item,
     rsdb_pvt_id_t      *acl_id_ptr,
     error_status_t     *status
    ) 
{ 
    char 	            *line, *token, *value;
    sec_rgy_foreign_id_t    creator_ids;
    sec_rgy_name_t          base_name;
    sec_attr_t	            *old_attrs, *attrs=NULL;
    unsigned32	            failure_index=0, num_attrs=0;
    rsdb_named_item_t       name_item;
    unsigned_char_p_t       tmp_name;
    int                     i, rc=TRUE;
    rsdb_credentials_t      credentials;
    boolean32               dummy;

    item->flags = sec_rgy_pgo_flags_none;

    while (line = next_line(fp)) {
       if (line) {
	   parse_line(line, &token, &value); 
           if (strcmp(token, prt_Name) == 0) {
	       strcpy((char *)&base_name, value);
	       /* assumption is there should be a "domain-root/" */
	       /* prepend to the pgo, take it out now.  If fail */
	       /* to parse, error ... */
	       if ( value == 
		     strpbrk(value, (char *)root_name_keys[domain].name) ) {
		     /* root directory staring at beginning, pass to next / */
		     if ( (value=strchr(value, '/')) != NULL ) {
			 value++;
			 strcpy((char *)name, value);
			 free (line);
			 continue;
		     }
	       }
	       SET_STATUS(status, sec_rgy_bad_name);
	       error_st(*status, "Incorrect pgo root directory name");
	   }
	   else if (strcmp(token, prt_UUID) == 0) {
	       uuid_from_string((unsigned char *)value, 
				&(item->id), status);
	       if (BAD_STATUS(status))
		 error_st(*status, "Unable to convert Realm UUID");
	       if ( domain == sec_rgy_domain_person ) {
		   if ( ! do_acl ) {
                       tmp_name = (unsigned_char_p_t )dce_strdup((char *)name);
                       add_principal_uuid(tmp_name, item->id);
                       free(tmp_name);
                   }
               }
           }
	   else
           if (strcmp(token, prt_Unix_ID) == 0) {
               sscanf(value, "%d", &item->unix_num);
           }
	   else
           if (strcmp(token, prt_Is_Alias_Flag) == 0) {
	       if (strcmp(value, prt_true) == 0)
		   item->flags |= sec_rgy_pgo_is_an_alias;
           }
	   else
           if (strcmp(token, prt_Is_Required_Flag) == 0) {
	       if (strcmp(value, prt_true) == 0)
		   item->flags |= sec_rgy_pgo_is_required;
           }
	   else
           if (strcmp(token, prt_Quota) == 0 && 
	       domain == sec_rgy_domain_person ) {
	       if (strcmp(value, prt_unlimited) == 0)
		   item->quota = sec_rgy_quota_unlimited;
	       else
		   sscanf(value, "%d", &item->quota);
	   }
	   else
	   if (strcmp(token, prt_Projlist_Ok_Flag) == 0 &&
	       domain == sec_rgy_domain_group ) {
	       if (strcmp(value, prt_true) == 0)
		   item->flags |= sec_rgy_pgo_projlist_ok;
	   }
	   else
	   if (strcmp(token, prt_Num_Attr_List_Entries) == 0 ) {
	        sscanf(value, "%d", &num_attrs);
		if ( num_attrs > 0 ) {
		    construct_attr_list(fp, num_attrs, &attrs, status);
		    if ( BAD_STATUS(status) ) {
			error_st(*status, "Constructing attr list failed");
		    }
		}
	   }
	   else
           if (strcmp(token, prt_Fullname) == 0) {
	       strcpy((char *)item->fullname, value);
	       free(line);
	       break;
	   }
	   free(line);
        }
	else
	{
	    error_st(*status, "Unable to parse print file");
	    rc = FALSE;
	}
    }

    if ( ! do_acl ) {
    	uuid_create_nil(&creator_ids.principal, status);
    	rsdb_pgo_add(domain, name, item, NULL_PTR,
		&creator_ids, true, &dummy, status);
    	/* get acl id for relating acl information from print file */
    	rsdb_get_credentials_by_unix_id(domain_info[domain].db, item->unix_num,
				    &credentials);
    	if (BAD_STATUS(status)) {
            error_st(*status, "Unable to add pgo to database");
	    rc = FALSE;
    	}

    	if ( num_attrs > 0 ) {
    	    svg_rs_attr_update_no_auth_check(base_name, num_attrs,
	    	 attrs, &failure_index, status);
    	    if ( BAD_STATUS(status) ) {
	    	fprintf(stderr, "Adding attribute instannces for %s failed\n",
                    name);
	    	error_st(*status);
    	    }
	}

    }
    else {
        dummy = rsdb_util_get_by_name(domain, name, strlen((char *)name),
            &name_item);
    	/* get acl id for relating acl information from print file */
    	rsdb_get_credentials_by_unix_id(domain_info[domain].db, 
				(RSDB_PGO(&name_item)).unix_id, &credentials);
    }

    /* get acl id for relating acl information from print file */
    rsdb_get_credentials_by_unix_id(domain_info[domain].db, item->unix_num,
				    &credentials);
    *acl_id_ptr = credentials.acl_id;

    old_attrs = attrs;
    for ( i=0; i<num_attrs; i++ ) {
	sec_attr_util_inst_free_ptrs(attrs);
	attrs++;
    }
    if ( old_attrs != NULL ) {
	free(old_attrs);
    }

    return (rc);
}


 /*
  * c o n s t r u c t _ p r i n c i p a l
  *
  * construct more information for principal from print file
  * 
  */
PRIVATE boolean32 construct_principal
    ( 
     FILE *fp,
     boolean32	   do_acl,
     error_status_t *status
    ) 
{ 
    char                *line, *token, *value;
    sec_rgy_name_t      name;
    sec_rgy_pgo_item_t  *item;
    rsdb_pvt_id_t       acl_id;

    item = NEW(sec_rgy_pgo_item_t);
    bzero(item, sizeof(sec_rgy_pgo_item_t));

    if ( construct_pgo(fp, sec_rgy_domain_person, do_acl, name, item, &acl_id,
		       status) == FALSE )
    {
        error_st(*status, "Unable to parse print file for pgo");
	return (FALSE);
    }
    while (line = next_line(fp)) {
       if (line) {
           parse_line(line, &token, &value); 
	   if (strcmp(token, prt_Member_Name) == 0) {
	       if ( ! do_acl )
	           add_member_list_no_duplicate(sec_rgy_domain_person, name, 
			value, NULL, NULL);
	   }
	   else
           if (strcmp(token, prt_Obj_Acl_Def_Cell_Name) == 0) {
               get_acl(fp, do_acl, FALSE, value, prt_Obj_Acl_Entry, 
		       acl_id, rsdb_acl_mgr_type_person,
		       status);
	       if (BAD_STATUS(status))
		   error_st(*status, "Unable to get principal acl");
	   }
	   else 
	   if (strcmp(token, prt_Obj_Acl_Def_Cell_UUID) == 0) {
	       get_acl(fp, do_acl, TRUE, value, prt_Obj_Acl_Entry,
		       acl_id, rsdb_acl_mgr_type_person, 
		       status);
	       if (BAD_STATUS(status))
		   error_st(*status, "Unable to get properties acl");
	   }
	   else
           if (strcmp(token, prt_Acct_Group_Name) == 0) {
	       get_acct(line, fp, do_acl, name, value, item, status);
	       if (BAD_STATUS(status)) return FALSE;
	   }
	   else
           if (strcmp(token, prt_Obj_Acl_Def_Cell_Name) == 0) {
               get_acl(fp, do_acl, FALSE, value, prt_Obj_Acl_Entry, 
		       acl_id, rsdb_acl_mgr_type_person,
		       status);
	       if (BAD_STATUS(status))
		   error_st(*status, "Unable to get properties acl");
	   }
	   else 
	   if (strcmp(token, prt_Obj_Acl_Def_Cell_UUID) == 0) {
	       get_acl(fp, do_acl, TRUE, value, prt_Obj_Acl_Entry,
		       acl_id, rsdb_acl_mgr_type_person,
		       status);
	       if (BAD_STATUS(status))
		   error_st(*status, "Unable to get properties acl");
	   }
	   else
	   if (strcmp(token, prt_Record_Number) == 0) {
	        free(line);
		if ( item != NULL )
		    free(item);
                return TRUE;
	   }
	   free (line);
        }
	else
	    error_st(*status, "Unable to parse print file");
    }
    if ( item != NULL )
        free(item);
    return (FALSE);
}


 /*
  * c o n s t r u c t _ g r o u p
  *
  * construct more information for group from print file
  * 
  */
PRIVATE boolean32 construct_group
    ( 
     FILE *fp,
     boolean32      do_acl,
     error_status_t *status
    ) 
{ 
    char                *line, *token, *value;
    sec_rgy_name_t      name;
    sec_rgy_pgo_item_t  *item;
    rsdb_pvt_id_t       acl_id;
    int                 i;

    item = NEW(sec_rgy_pgo_item_t);
    bzero(item, sizeof(sec_rgy_pgo_item_t));

    if ( construct_pgo(fp, sec_rgy_domain_group, do_acl, name, item, &acl_id,
		       status) == FALSE )
    {
        error_st(*status, "Unable to parse print file for pgo");
	return (FALSE);
    }


    while (line = next_line(fp)) {
       if (line) {
           parse_line(line, &token, &value); 
	   if (strcmp(token, prt_Member_Name) == 0) {
	       if ( ! do_acl )
	           add_member_list_no_duplicate(sec_rgy_domain_group, name, 
			value, NULL, NULL);
	   }
	   else
	   if (strcmp(token, prt_Foreign_Member_Name) == 0) {
	       if ( ! do_acl ) {
		   uuid_t	cell_uuid;
		   uuid_t	princ_uuid;
		   char		*member_name;
		   member_name = malloc(strlen(value)+1);
		   strcpy(member_name, value);
		   free(line);
		   /* get cell uuid and principal uuid in  */
		   line = next_line(fp);
		   parse_line(line, &token, &value);
                   if (strcmp(token, prt_Cell_UUID) == 0) {
	       		uuid_from_string((unsigned char *)value, 
				&cell_uuid, status);
	                if (BAD_STATUS(status))
		   	    error_st(status, "Unable to convert Cell UUID - %s"
				, value);
		   } else {
			fprintf(stderr, "Incorrect Group data file - foreign member name and uuids not in right sequence");
		   }
		   free(line);
		   line = next_line(fp);
		   parse_line(line, &token, &value);
                   if (strcmp(token, prt_Princ_UUID) == 0) {
	       		uuid_from_string((unsigned char *)value, 
				&princ_uuid, status);
	                if (BAD_STATUS(status))
		   	    error_st(status, "Unable to convert Principal UUID - %s"
				, value);
		   } else {
			fprintf(stderr, "Incorrect Group data file - foreign member name and uuids not in right sequence");
		   }

	           add_member_list_no_duplicate(sec_rgy_domain_group, name, 
			member_name, &cell_uuid, &princ_uuid);
	       }
	   }
	   else
           if (strcmp(token, prt_Obj_Acl_Def_Cell_Name) == 0) {
               get_acl(fp, do_acl, FALSE, value, prt_Obj_Acl_Entry, 
		       acl_id, rsdb_acl_mgr_type_group,
		       status);
	       if (BAD_STATUS(status))
		   error_st(*status, "Unable to get properties acl");
	   }
	   else 
	   if (strcmp(token, prt_Obj_Acl_Def_Cell_UUID) == 0) {
	       get_acl(fp, do_acl, TRUE, value, prt_Obj_Acl_Entry,
		       acl_id, rsdb_acl_mgr_type_group,
		       status);
	       if (BAD_STATUS(status))
		   error_st(*status, "Unable to get properties acl");
	   }
	   else
	   if (strcmp(token, prt_Record_Number) == 0) {
	        free(line);
		if ( item != NULL )
		    free(item);
                return TRUE;
	   }
	   free (line);
        }
	else {
	    error_st(*status, "Unable to parse print file");
	    return (FALSE);
	}
    }
    if ( item != NULL )
        free(item);
    return (FALSE);
}


 /*
  * c o n s t r u c t _ o r g
  *
  * construct more information for org from print file
  * 
  */
PRIVATE boolean32 construct_org
  ( 
    FILE           *fp,
    boolean32      do_acl,
    error_status_t *status
  ) 
{ 
    char                *line, *token, *value;
    sec_rgy_name_t      name;
    sec_rgy_pgo_item_t  *item;
    rsdb_pvt_id_t       acl_id;
    int                 i;

    item = NEW(sec_rgy_pgo_item_t);
    bzero(item, sizeof(sec_rgy_pgo_item_t));

    if ( construct_pgo(fp, sec_rgy_domain_org, do_acl, name, item, &acl_id,
		       status) == FALSE )
    {
        error_st(*status, "Unable to parse print file for pgo");
	return (FALSE);
    }


    while (line = next_line(fp)) {
        if (line) {
            parse_line(line, &token, &value); 
            if (strcmp(token, prt_Member_Name) == 0) {
                if ( ! do_acl )
                    add_member_list_no_duplicate(sec_rgy_domain_org, name, 
			value, NULL, NULL);
                free(line);
            }
            else if (strcmp(token, prt_Obj_Acl_Def_Cell_Name) == 0) {
                get_acl(fp, do_acl, FALSE, value, prt_Obj_Acl_Entry, 
                        acl_id, rsdb_acl_mgr_type_org,
                        status);
                if (BAD_STATUS(status))
                    error_st(*status, "Unable to get properties acl");
                free(line);
            }
            else if (strcmp(token, prt_Obj_Acl_Def_Cell_UUID) == 0) {
                get_acl(fp, do_acl, TRUE, value, prt_Obj_Acl_Entry,
                        acl_id, rsdb_acl_mgr_type_org,
                        status);
                if (BAD_STATUS(status))
                    error_st(*status, "Unable to get properties acl");
                free(line);
            }
            else if (strcmp(token, prt_Acct_Lifetime) == 0) {
                construct_passwd_policy(line, fp, name);
            }
            else if (strcmp(token, prt_Record_Number) == 0) {
		if ( item != NULL )
		    free(item);
	        free(line);
                return TRUE;
            } 
            else {
                free(line);
            }
        }
	else {
	    error_st(*status, "Unable to parse print file");
	    return (FALSE);
	}
    }
    if ( item != NULL )
        free(item);
    return (FALSE);
}


 /*
  * c o n s t r u c t _ p g o _ d o m a i n
  *
  * set up for pgo construction
  *
  * The do_acl flag is used to indicate if we need to 
  * reconstruct acl in this pass or not.  Since we
  * generate acl with its associated pgo in their
  * print file but its entry name cannot be resolved
  * until all pgo are reconstructed.  We used to keep 
  * an in memory-resident link list to keep all acl around
  * until all pgo/dir are constructed, then construct
  * the acls,  Hence memory space is a problem, we now
  * parse pgo print files twice, first pass is for pgo
  * entries so acl won't be recorded; second pass is
  * for acl only so pgo's entries are ignored and its
  * data are retrieved from the already built database..
  * 
  */
PUBLIC void construct_pgo_domain
  ( 
    sec_rgy_domain_t domain,
    boolean32	     do_acl,
    error_status_t *status
  ) 
{ 
    char            *line, *token, *value;
    FILE            *fopen(), *fp; 
    sec_rgy_pname_t prt_file; 
    boolean32       next_record = FALSE;

    strcpy((char *) prt_file, (char *) prt_path); 
    if (domain == sec_rgy_domain_person) 
        strcat((char *) prt_file, PRT_FILE_PRINC); 
    else if (domain == sec_rgy_domain_group) 
        strcat((char *) prt_file, PRT_FILE_GROUP); 
    else if (domain == sec_rgy_domain_org)
        strcat((char *) prt_file, PRT_FILE_ORG); 
    else 
        return;

    /* open .prt file */ 
    fp = fopen((char *) prt_file, "r"); 

    if (fp) { 
        line = next_line(fp);
        if (line) {
            parse_line(line, &token, &value);
            if (strcmp(token, prt_Record_Number) == 0)
                next_record = TRUE;
	    free(line);
            while (next_record) {
                line = next_line(fp);
                if (line) {
                    parse_line(line, &token, &value);
                    if (strcmp(token, prt_Object_Type) == 0) {
                        if (strcmp(value, prt_DIR) == 0)
                            next_record = construct_dir(fp, domain, do_acl, status);
                        else if ((strcmp(value, prt_PRINC) == 0) &&
                                 domain == sec_rgy_domain_person)
                            next_record = construct_principal(fp, do_acl, status);
                        else if ((strcmp(value, prt_GROUP) == 0) &&
                                 domain == sec_rgy_domain_group)
                            next_record = construct_group(fp, do_acl, status);
                        else if ((strcmp(value, prt_ORG) == 0) &&
                                 domain == sec_rgy_domain_org)
                            next_record = construct_org(fp, do_acl, status);
                        else {
                            error_st(*status, "Unable to parse print file");
                        }
                    }
                    else
                        error_st(*status, "Unable to parse print file");
		    free(line);
                }
		if (BAD_STATUS(status)) return;
            }
        }
        fclose(fp); 
    } 
    else 
        *status = sec_rgy_dir_not_found; 
} 


 /*
  * c o n s t r u c t _ m k e y
  *
  * construct master key from print file
  * 
  */
PUBLIC void construct_mkey 
  ( 
    error_status_t *status
  )
{
    FILE            *fopen(), *fp;
    char            *line, *token, *value;
    sec_rgy_pname_t prt_file;
    rsdb_auth_key_t mkey;
    int             version;

    strcpy((char *) prt_file, (char *) prt_base_path);
    strcat((char *) prt_file, PRT_FILE_MKEY);

    /* open mkey.prt file */
    fp = fopen((char *) prt_file, "r");

    if (fp) {
        while (line = next_line(fp)) {
           if (line) {
               parse_line(line, &token, &value); 
               if (strcmp(token, prt_Master_Key_Version) == 0) {
                   sscanf(value, "%d", &version);
                   mkey.key_version = (unsigned char) version;
               }
               else if (strcmp(token, prt_Master_Key_Keytype) == 0) {
                   if (strcmp(value, prt_des) == 0)
                       mkey.keytype = KEYTYPE_DES;
                   else
                       mkey.keytype = KEYTYPE_DES;
               }
               else if (strcmp(token, prt_Master_Key_Length) == 0) {
                   sscanf(value, "%d", &mkey.length);
               }
               else if (strcmp(token, prt_Master_Key) == 0) {
                   get_key(value, mkey.length, mkey.contents);
               }
               free(line);
           }
        }
        fclose(fp);
        save_mkey(&mkey, status);
        if (BAD_STATUS(status))
            error_st(*status, "Unable to create master key file");
	rs_mkey_init(rs_state.rep_state, status);
        if (BAD_STATUS(status))
            error_st(*status, "Unable to load mkey");
    }
    else
        *status = sec_rgy_dir_not_found;
}


PRIVATE void construct_attr_encoding
  (
    char 			*value,
    sec_attr_encoding_t         *attr_encoding_p
  )
{
    if ( strcmp(value, prt_attr_enc_any) == 0 )
	*attr_encoding_p = sec_attr_enc_any;
    else 
    if ( strcmp(value, prt_attr_enc_void) == 0 )
	*attr_encoding_p = sec_attr_enc_void;
    else 
    if ( strcmp(value, prt_attr_enc_integer) == 0 )
	*attr_encoding_p = sec_attr_enc_integer;
    else 
    if ( strcmp(value, prt_attr_enc_printstring) == 0 )
	*attr_encoding_p = sec_attr_enc_printstring;
    else 
    if ( strcmp(value, prt_attr_enc_printstring_array) == 0 )
	*attr_encoding_p = sec_attr_enc_printstring_array;
    else 
    if ( strcmp(value, prt_attr_enc_bytes) == 0 )
	*attr_encoding_p = sec_attr_enc_bytes;
    else 
    if ( strcmp(value, prt_attr_enc_confidential_bytes) == 0 )
	*attr_encoding_p = sec_attr_enc_confidential_bytes;
    else 
    if ( strcmp(value, prt_attr_enc_i18n_data) == 0 )
	*attr_encoding_p = sec_attr_enc_i18n_data;
    else 
    if ( strcmp(value, prt_attr_enc_uuid) == 0 )
	*attr_encoding_p = sec_attr_enc_uuid;
    else 
    if ( strcmp(value, prt_attr_enc_attr_set) == 0 )
	*attr_encoding_p = sec_attr_enc_attr_set;
    else 
    if ( strcmp(value, prt_attr_enc_binding) == 0 )
	*attr_encoding_p = sec_attr_enc_binding;
    else 
    if ( strcmp(value, prt_attr_enc_trig_binding) == 0 )
	*attr_encoding_p = sec_attr_enc_trig_binding;
    else {
        fprintf(stderr, "Invalid encoding type - %s\n", value);
	*attr_encoding_p = sec_attr_enc_void;
    }
}


PRIVATE void construct_attr_schema_aclmgr
  (
    FILE                        *fp,
    char 			*value,
    sec_attr_schema_entry_t	*sch_entry_p
  )
{
    char 			*line, *token, *p;
    rsdb_acl_mgr_type_t    	mgr_type;
    uuid_t                      mgr_type_chain, aclmgr_uuid;
    sec_acl_printstring_t       mgr_pstring;
    boolean32           	tokenize;
    unsigned32                  size_used, total_pstrings;
    sec_acl_printstring_t       printstrings[MAX_ACL_PRINTSTRINGS];
    error_status_t              lst;
    int		        	i, j;

    sscanf(value, "%d", &i);
    sch_entry_p->acl_mgr_set = malloc(sizeof(unsigned32)+
		i*sizeof(sec_attr_acl_mgr_info_p_t));
    sch_entry_p->acl_mgr_set->num_acl_mgrs = i;

    for ( i=0; i<sch_entry_p->acl_mgr_set->num_acl_mgrs; i++ ) {
	p = malloc(sizeof(sec_attr_acl_mgr_info_t));
	sch_entry_p->acl_mgr_set->mgr_info[i] = (sec_attr_acl_mgr_info_t *) p;
	memset(p, 0, sizeof(sec_attr_acl_mgr_info_t));
	line = next_line(fp);
	parse_line(line, &token, &value); 
	if ( strcmp(token, prt_Attr_Acl_Mgr_Type) == 0 ) {
	    if ( ! rsdb_aclmgr_str_to_type(value, &mgr_type) ) {
	    	fprintf(stderr, "Cannot convert acl str to type\n");
	    }
	    if ( ! rsdb_acl_get_mgr_uuid_from_type(mgr_type, 
		&aclmgr_uuid, &lst) ) {
		fprintf(stderr, "Cannot get acl uuid from type\n");
	    }
	    memcpy(&sch_entry_p->acl_mgr_set->mgr_info[i]->acl_mgr_type,
		&aclmgr_uuid, sizeof(uuid_t));
    	}
    	else
    	    fprintf(stderr, "Invalid - %s, should be - %s\n", token, 
			prt_Attr_Acl_Mgr_Type);

        sec_acl_mgr_get_printstring(acl_mgr_handle, 
            &aclmgr_uuid, MAX_ACL_PRINTSTRINGS, 
            &mgr_type_chain, &mgr_pstring, &tokenize, &total_pstrings, 
            &size_used, printstrings, &lst);
	if ( BAD_STATUS(&lst) ) {
	    error(lst, "Cannot get acl printstring");
	}

	for ( j=0; j<NUM_SCHEMA_ATTR_ACLS; j++ )
	{
	    free(line);
	    line = next_line(fp);
            parse_line(line, &token, &value); 
	    if ( strcmp(token, prt_Attr_Acl_Mgr_Query_Entry) == 0 ) {
                rsdb_attr_str_to_aclmgr(value, 
		    &sch_entry_p->acl_mgr_set->mgr_info[i]->query_permset,
		    printstrings, total_pstrings, &lst);
		if ( BAD_STATUS(&lst) ) {
		    error_st(lst, "Cannot convert acl string to acl type");
		}
	    }
	    else
	    if ( strcmp(token, prt_Attr_Acl_Mgr_Update_Entry) == 0 ) {
                rsdb_attr_str_to_aclmgr(value, 
		    &sch_entry_p->acl_mgr_set->mgr_info[i]->update_permset,
		    printstrings, total_pstrings, &lst);
		if ( BAD_STATUS(&lst) ) {
		    error_st(lst, "Cannot convert acl string to acl type");
		}
	    }
	    else
	    if ( strcmp(token, prt_Attr_Acl_Mgr_Test_Entry) == 0 ) {
                rsdb_attr_str_to_aclmgr(value, 
		    &sch_entry_p->acl_mgr_set->mgr_info[i]->test_permset,
		    printstrings, total_pstrings, &lst);
		if ( BAD_STATUS(&lst) ) {
		    error_st(lst, "Cannot convert acl string to acl type");
		}
	    }
	    else
	    if ( strcmp(token, prt_Attr_Acl_Mgr_Delete_Entry) == 0 ) {
                rsdb_attr_str_to_aclmgr(value, 
		    &sch_entry_p->acl_mgr_set->mgr_info[i]->delete_permset,
		    printstrings, total_pstrings, &lst);
		if ( BAD_STATUS(&lst) ) {
		    error_st(lst, "Cannot convert acl string to acl type");
		}
	    }
	    else {
		fprintf(stderr, "Invalid Acl Mgr Entry - %d\n", token);
	    }
	}
	free(line);
    }
}


PRIVATE void free_attr_schema_aclmgr
  (
    sec_attr_schema_entry_t	*sch_entry_p
  )
{
    int         i;

    if (sch_entry_p->acl_mgr_set == NULL) {
        return;
    }

    for (i = 0; i < sch_entry_p->acl_mgr_set->num_acl_mgrs; i++) {
        free(sch_entry_p->acl_mgr_set->mgr_info[i]);
    }

    free(sch_entry_p->acl_mgr_set);
}


PRIVATE void construct_attr_schema_flags
  (
    char 			*value,
    sec_attr_sch_entry_flags_t  *flag_p
  )

{
    if ( strcmp(value, prt_attr_sch_entry_none) == 0 );

    if ( strcmp(value, prt_attr_sch_entry_unique) == 0 )
	*flag_p = *flag_p | sec_attr_sch_entry_unique;
    if ( strcmp(value, prt_attr_sch_entry_multi_inst) == 0 )
	*flag_p = *flag_p | sec_attr_sch_entry_multi_inst;
    if ( strcmp(value, prt_attr_sch_entry_reserved) == 0 )
	*flag_p = *flag_p | sec_attr_sch_entry_reserved;
    if ( strcmp(value, prt_attr_sch_entry_use_defaults) == 0 )
	*flag_p = *flag_p | sec_attr_sch_entry_use_defaults;
}


PRIVATE void construct_attr_schema_intercell_act
  (
    char 			*value,
    sec_attr_intercell_action_t *action_p
  )
{
    if ( strcmp(value, prt_accept) == 0 )
	*action_p = sec_attr_intercell_act_accept;
    else
    if ( strcmp(value, prt_reject) == 0 )
	*action_p = sec_attr_intercell_act_reject;
    else
    if ( strcmp(value, prt_evaluate) == 0 )
	*action_p = sec_attr_intercell_act_evaluate;
    else
	fprintf(stderr, "Invalid intercell action - %s\n", value);
}


PRIVATE void construct_attr_schema_trig_type
  (
    char 			*value,
    sec_attr_trig_type_flags_t  *trig_type_p
  )
{
    if ( strcmp(value, prt_none) == 0 )
	*trig_type_p = sec_attr_trig_type_none;
    else
    if ( strcmp(value, prt_query) == 0 )
	*trig_type_p = sec_attr_trig_type_query;
    else
    if ( strcmp(value, prt_update) == 0 )
	*trig_type_p = sec_attr_trig_type_update;
    else
	fprintf(stderr, "Invalid trig type - %s\n", value);
}


PRIVATE void construct_binding_protect_level
  (
    char                        *value,
    unsigned32                  *protect_level_p
  )
{
    if ( strcmp(value, prt_rpc_c_protect_level_default) == 0 )
	*protect_level_p = rpc_c_protect_level_default;
    else 
    if ( strcmp(value, prt_rpc_c_protect_level_none) == 0 )
	*protect_level_p = rpc_c_protect_level_none;
    else 
    if ( strcmp(value, prt_rpc_c_protect_level_connect) == 0 )
	*protect_level_p = rpc_c_protect_level_connect;
    else 
    if ( strcmp(value, prt_rpc_c_protect_level_call) == 0 )
	*protect_level_p = rpc_c_protect_level_call; 
    else 
    if ( strcmp(value, prt_rpc_c_protect_level_pkt) == 0 )
	*protect_level_p = rpc_c_protect_level_pkt;
    else 
    if ( strcmp(value, prt_rpc_c_protect_level_pkt_integ) == 0 )
	*protect_level_p = rpc_c_protect_level_pkt_integ;
    else 
    if ( strcmp(value, prt_rpc_c_protect_level_pkt_privacy) == 0 )
	*protect_level_p = rpc_c_protect_level_pkt_privacy;
    else 
	fprintf(stderr, "Invalid binding protect level - %s\n", value);
}


PRIVATE void construct_binding_authn
  (
    char                        *value,
    unsigned32                  *authn_p
  )
{
    if ( strcmp(value, prt_rpc_c_authn_none) == 0 )
	*authn_p = rpc_c_authn_none;
    else 
    if ( strcmp(value, prt_rpc_c_authn_dce_secret) == 0 )
	*authn_p = rpc_c_authn_dce_secret;
    else 
    if ( strcmp(value, prt_rpc_c_authn_dce_public) == 0 )
	*authn_p = rpc_c_authn_dce_public;
    else 
    if ( strcmp(value, prt_rpc_c_authn_default) == 0 )
	*authn_p = rpc_c_authn_default;
    else 
	fprintf(stderr, "Invalid binding authn - %s\n", value);
}


PRIVATE void construct_binding_authz
  (
    char                        *value,
    unsigned32                  *authz_p
  )
{
    if ( strcmp(value, prt_rpc_c_authz_none) == 0 )
	*authz_p = rpc_c_authz_none; 
    else 
    if ( strcmp(value, prt_rpc_c_authz_name) == 0 )
	*authz_p = rpc_c_authz_name;
    else
    if ( strcmp(value, prt_rpc_c_authz_dce) == 0 )
	*authz_p = rpc_c_authz_dce;
    else
	fprintf(stderr, "Invalid binding authz - %s\n", value);
}


PRIVATE void construct_attr_binding_info
  (
    FILE                        *fp,
    sec_attr_binding_t	        *binding_info
  )
{
    char 			*line, *token, *value;
    unsigned32                  num_towers;
    int                         i;
    error_status_t              lst;

    line = next_line(fp);
    if (line) {
            parse_line(line, &token, &value); 
	    if ( strcmp(token, prt_Attr_Binding_String) == 0 ) {
		binding_info->bind_type = sec_attr_bind_type_string;
		binding_info->tagged_union.string_binding = malloc(strlen(value)+1);
		strcpy((char *)binding_info->tagged_union.string_binding, value);
	    }
	    else
	    if ( strcmp(token, prt_Num_Towers) == 0 ) {
		binding_info->bind_type = sec_attr_bind_type_twrs;
         	sscanf(value, "%d", &num_towers);
		binding_info->tagged_union.twr_set = malloc(sizeof(unsigned32)+
			num_towers*sizeof(sec_attr_twr_ref_t));
		binding_info->tagged_union.twr_set->count = 0;
		for ( i=0; i<num_towers; i++ ) {
		    construct_tower(fp, (rs_replica_twr_vec_p_t)
			binding_info->tagged_union.twr_set, &lst);
		    if (BAD_STATUS(&lst)) {
			error_st(lst, "Cannot construct binding tower");
		    }
		}
	    }
	    else
	    if ( strcmp(token, prt_Attr_Binding_Svr_Name_Syntax) == 0 ) {
		binding_info->bind_type = sec_attr_bind_type_svrname;
		binding_info->tagged_union.svrname = malloc(sizeof(sec_attr_bind_svrname));
		sscanf(value, "%d", 
			&binding_info->tagged_union.svrname->name_syntax);
		free(line);
    		line = next_line(fp);
	        parse_line(line, &token, &value); 
	        if ( strcmp(token, prt_Attr_Binding_Svr_Name) != 0 ) {
		    fprintf(stderr, "Invalid Attr Binding token - %s, %s expected\n",
			token, prt_Attr_Binding_Svr_Name);
		}
		binding_info->tagged_union.svrname = malloc(sizeof(sec_attr_bind_svrname));
		binding_info->tagged_union.svrname->name = malloc(strlen(value)+1);
		strcpy((char *)binding_info->tagged_union.svrname->name, value);
	    }
   	    else
		fprintf(stderr, "Invalid binding info - %s\n", value);
        }
}


PRIVATE char *construct_attr_binding
  (
    FILE                        *fp,
    char 			*value,
    sec_attr_bind_info_t	**binding_p
  )
{
    char 			*line, *token, *ret_value;
    sec_attr_bind_auth_info_t   auth_info;
    unsigned32                  num_bindings;
    int                         i;

    if ( strcmp(value, prt_bind_auth_none) == 0 ) {
	auth_info.info_type = sec_attr_bind_auth_none;
	line = next_line(fp);
	parse_line(line, &token, &ret_value);
    }
    else
    if ( strcmp(value, prt_bind_auth_dce) == 0 ) {
	auth_info.info_type = sec_attr_bind_auth_dce;
        while (line = next_line(fp)) {
            if (line) {
           	parse_line(line, &token, &ret_value); 
	    	if (strcmp(token, prt_Attr_Svr_Princ_Name) == 0 ) {
		    auth_info.tagged_union.dce_info.svr_princ_name = 
			malloc(strlen(ret_value) + 1);
		    strcpy((char *)auth_info.tagged_union.dce_info.svr_princ_name, ret_value);
		}
	    	else
		if (strcmp(token, prt_Attr_Auth_Protect_Level) == 0 ) {
		    construct_binding_protect_level(ret_value, 
			&auth_info.tagged_union.dce_info.protect_level);
		}
	    	else
		if (strcmp(token, prt_Attr_Auth_Authn) == 0 ) {
		    construct_binding_authn(ret_value, 
			&auth_info.tagged_union.dce_info.authn_svc);
		}
	    	else
		if (strcmp(token, prt_Attr_Auth_Authz) == 0 ) {
		    construct_binding_authz(ret_value, 
			&auth_info.tagged_union.dce_info.authz_svc);
		    line = next_line(fp);
		    parse_line(line, &token, &ret_value);
		    break; /* assuming authz is last one */
		}
	        free(line);
	    }
	    else
		break;
	}
    }
    else
	fprintf(stderr, "Invalid trig binding auth type - %s\n", value);

    if ( strcmp(token, prt_Attr_Num_Binding) == 0 ) {
         sscanf(ret_value, "%d", &num_bindings);
	 *binding_p = malloc(sizeof(sec_attr_bind_auth_info_t)+
		sizeof(unsigned32) + num_bindings*sizeof(sec_attr_binding_t));
	 memcpy(&((*binding_p)->auth_info), &auth_info, sizeof(auth_info));
	 (*binding_p)->num_bindings = num_bindings;
    }

    for ( i=0; i<num_bindings; i++ ) {
	construct_attr_binding_info(fp, &((*binding_p)->bindings[i]));
    }
    return ret_value;
}


 /*
  * c o n s t r u c t _ x a t t r s c h e m a _ a c l
  *
  * construct xattrschema acl
  * 
  */
PRIVATE void construct_xattrschema_acl
  (
    FILE *fp,
    boolean32   do_acl,
    char *entry_token,
    rsdb_pvt_id_t acl_id,
    rsdb_acl_mgr_type_t type,
    error_status_t *status
  )
{
    char *line, *token, *value;

    line = next_line(fp);
    parse_line(line, &token, &value); 
    if (strcmp(token, prt_Obj_Acl_Def_Cell_Name) == 0) {
        get_acl(fp, do_acl, FALSE, value, prt_Obj_Acl_Entry,
           rsdb_get_attr_schema_acl_id(), rsdb_acl_mgr_type_attr_schema, status);
        if (BAD_STATUS(status))
            error_st(*status, "Unable to get xattrschema acl");
    }
    else if (strcmp(token, prt_Obj_Acl_Def_Cell_UUID) == 0) {
        get_acl(fp, do_acl, TRUE, value, prt_Obj_Acl_Entry,
           rsdb_get_attr_schema_acl_id(), rsdb_acl_mgr_type_attr_schema, status);
    }
    else
	fprintf(stderr, "Invalid token string - %s, %s expected\n", token, 
			prt_Obj_Acl_Def_Cell_Name);
    free(line);
}


PRIVATE boolean32 construct_attr_schema_entry
  (
    FILE                        *fp,
    boolean32			do_acl,
    sec_attr_schema_entry_t     *sch_entry_p,
    error_status_t              *status
  )
{
    char 			*line, *token, *value;
    sec_rgy_name_t 		*name_p;
    sec_attr_acl_mgr_info_set_t *aclmgr_info_p;

    *status = error_status_ok;
    memset((char *)sch_entry_p, 0, sizeof(sec_attr_schema_entry_t));

    while (line = next_line(fp)) {
       if (line) {
           parse_line(line, &token, &value); 
	   if (strcmp(token, prt_Record_Number) == 0) {
	       free(line);
 	       return TRUE; /* for more processing */
	   }
	   else
           if (strcmp(token, prt_Name) == 0) {
	       name_p = malloc(strlen(value)+1);
               strcpy((char *) name_p, value);
	       sch_entry_p->attr_name = (unsigned_char_p_t) name_p;
           }
           else if (strcmp(token, prt_Xattrschema_Name) == 0) {
               construct_xattrschema_acl(fp, do_acl, prt_Obj_Acl_Entry, 
           	   rsdb_get_attr_schema_acl_id(), rsdb_acl_mgr_type_attr_schema,
		   status);
	       if ( BAD_STATUS(status) ) {
		   error_st(*status, "Fail to reconstruct xattrschema acl");
                   free(line);
                   return(FALSE);
               }
           }
           else if (strcmp(token, prt_UUID) == 0) {
	       uuid_from_string((unsigned char *)value, 
				&(sch_entry_p->attr_id), status);
	       if (BAD_STATUS(status))
		   error_st(status, "Unable to convert Schema Attr UUID - %s", value);
           }
           else if (strcmp(token, prt_Attr_Encoding) == 0) {
	       construct_attr_encoding(value, &sch_entry_p->attr_encoding);
           }
           else if (strcmp(token, prt_Num_Attr_Acl_Mgr_Entries) == 0) {
	       construct_attr_schema_aclmgr(fp, value, sch_entry_p);
           }
           else if (strcmp(token, prt_Attr_Schema_Entry_Flag) == 0) {
	       construct_attr_schema_flags(value,
			&sch_entry_p->schema_entry_flags);
           }
           else if (strcmp(token, prt_Attr_Intercell_Action) == 0) {
	       construct_attr_schema_intercell_act(value, 
			&sch_entry_p->intercell_action);
	   }
           else if (strcmp(token, prt_Attr_Trig_Type) == 0) {
	       construct_attr_schema_trig_type(value, 
			&sch_entry_p->trig_types);
	   }
           else if (strcmp(token, prt_Attr_Bind_Auth_Type) == 0) {
	       value = construct_attr_binding(fp, value, &sch_entry_p->trig_binding);
	   }
           else if (strcmp(token, prt_Attr_Schema_Scope) == 0) {
		sch_entry_p->scope = malloc(strlen(value) + 1);
		strcpy((char *)sch_entry_p->scope, value);
           }
           else if (strcmp(token, prt_Attr_Schema_Comment) == 0) {
		sch_entry_p->comment = malloc(strlen(value) + 1);
		strcpy((char *)sch_entry_p->comment, value);
           }
           free(line);
       }
       else
           return FALSE;
    }
    return FALSE;
}


PRIVATE void free_attr_schema_entry
  (
    boolean32			do_acl,
    sec_attr_schema_entry_t     *sch_entry_p
  )
{
    if (sch_entry_p->attr_name != NULL) {
        free(sch_entry_p->attr_name);
    }
    if (sch_entry_p->scope != NULL) {
        free(sch_entry_p->scope);
    }
    if (sch_entry_p->comment != NULL) {
        free(sch_entry_p->comment);
    }
}


 /*
  * c o n s t r u c t _ a t t r _ s c h e m a
  *
  * construct attribute schema
  * 
  */
PUBLIC void construct_attr_schema
  (
    boolean32	   do_acl,
    error_status_t *status
  )
{
    FILE                    *fopen(), *fp;
    char                    *line, *token, *value;
    sec_rgy_pname_t         prt_file;
    sec_attr_schema_entry_t sch_entry;
    rsdb_pvt_id_t           sch_entry_id;
    char                    *schema_name=SCHEMA_NAME;
    boolean32		    more_entries=TRUE;

    strcpy((char *) prt_file, (char *) prt_path);
    strcat((char *) prt_file, PRT_FILE_ATTR_SCHEMA);

    /* open policy.prt file */
    fp = fopen((char *) prt_file, "r");

    if (fp) {
    	CLEAR_STATUS(status);

	if ( line = next_line(fp) ) { /* skip first record number */
            free(line);
	    while ( more_entries ) {
		memset((char *)&sch_entry, 0, sizeof(sec_attr_schema_entry_t));
	    	more_entries = construct_attr_schema_entry(fp, do_acl, &sch_entry, 
			status);
	    	if ( BAD_STATUS(status) ) {
	    	    return;
	    	}
		if ( more_entries ) {
		    if ( ! do_acl )
	    	    	rsdb_attr_sch_add((sec_attr_component_name_t)schema_name, &sch_entry, status);
		}
                free_attr_schema_aclmgr(&sch_entry);
                free_attr_schema_entry(do_acl, &sch_entry);
	    	if ( BAD_STATUS(status) ) {
	    	    return;
	    	}
	    }
	}
    }
    else
        *status = sec_rgy_dir_not_found;
}


 /*
  * c o n s t r u c t _ p o l i c i e s
  *
  * construct policies from print file
  * 
  */
PUBLIC void construct_policies
  (
    boolean32      do_acl,
    error_status_t *status
  )
{
    FILE                    *fopen(), *fp;
    char                    *line, *token, *value;
    sec_rgy_pname_t         prt_file;
    sec_rgy_plcy_t          policy;
    sec_rgy_login_name_t    wild_login;
    sec_rgy_plcy_auth_t     auth_policy;
    sec_timeval_sec_t       princ_cache, group_cache, org_cache;
    sec_rgy_name_t          realm_name;
    uuid_t                  realm_uuid;
    long                    file_version;
    rsdb_encrypt_key_t      master_key;
    int                     version;
    sec_rgy_name_t          my_name;
    signed32                num_attrs;

    strcpy((char *) prt_file, (char *) prt_path);
    strcat((char *) prt_file, PRT_FILE_POLICY);

    /* open policy.prt file */
    fp = fopen((char *) prt_file, "r");

    if (fp) {
        rsdb_properties_get_info(&properties, status);
        properties.flags = sec_rgy_properties_none;
        rsdb_policy_get_info((unsigned_char_p_t)"", &policy, status);
	if (BAD_STATUS(status)) return;
        policy.passwd_flags = sec_rgy_plcy_pwd_flags_none;
        CLEAR_LOGIN_NAME(&wild_login);
        rsdb_auth_policy_get_info(&wild_login, &auth_policy, status);
	if (BAD_STATUS(status)) return;
        while (line = next_line(fp)) {
           if (line) {
               parse_line(line, &token, &value); 
               if (strcmp(token, prt_Rgy_Policy_File_Version) == 0) {
                   sscanf(value, "%d", &version);
                   rsdb_get_rgy_info_file_version(&file_version);
                   if (version != file_version) {
                           /* error place holder for real error */
                       *status = sec_rgy_dir_not_found;
		       free(line);
		       break;
                   }
               }
               else if (strcmp(token, prt_Prop_Read_Version) == 0) {
                   sscanf(value, "%d", &properties.read_version);
               }
               else if (strcmp(token, prt_Prop_Write_Version) == 0) {
                   sscanf(value, "%d", &properties.write_version);
               }
               else if (strcmp(token, prt_Min_Certificate_Lifetime) == 0) {
                   dt_to_period(value, &properties.minimum_ticket_lifetime);
               }
               else if (strcmp(token, prt_Default_Certificate_Lifetime) == 0) {
                   dt_to_period(value, 
                                &properties.default_certificate_lifetime);
               }
               else if (strcmp(token, prt_Low_Unix_ID_Principal) == 0) {
                   sscanf(value, "%d", &properties.low_unix_id_person);
               }
               else if (strcmp(token, prt_Low_Unix_ID_Group) == 0) {
                   sscanf(value, "%d", &properties.low_unix_id_group);
               }
               else if (strcmp(token, prt_Low_Unix_ID_Org) == 0) {
                   sscanf(value, "%d", &properties.low_unix_id_org);
               }
               else if (strcmp(token, prt_Max_Unix_ID) == 0) {
                   sscanf(value, "%d", &properties.max_unix_id);
               }
               else if (strcmp(token, prt_Rgy_Readonly_Flag) == 0) {
                   if (strcmp(value, prt_true) == 0)
                       properties.flags |= sec_rgy_prop_readonly;
               }
               else if (strcmp(token, prt_Auth_Certificate_Unbound_Flag) == 0) {
                   if (strcmp(value, prt_true) == 0)
                       properties.flags |= sec_rgy_prop_auth_cert_unbound;
               }
               else if (strcmp(token, prt_Shadow_Passwd_Flag) == 0) {
                   if (strcmp(value, prt_true) == 0)
                       properties.flags |= sec_rgy_prop_shadow_passwd;
               }
               else if (strcmp(token, prt_Embedded_Unix_ID_Flag) == 0) {
                   if (strcmp(value, prt_true) == 0)
                       properties.flags |= sec_rgy_prop_embedded_unix_id;
               }
               else if (strcmp(token, prt_Realm_Name) == 0) {
                   strcpy((char *) realm_name, value);
               }
               else if (strcmp(token, prt_Realm_UUID) == 0) {
                   uuid_from_string((unsigned char *)value, 
                                    &(realm_uuid), status);
                   if (BAD_STATUS(status))
                       error_st(*status, "Unable to convert Realm UUID");
               }
               else if (strcmp(token, prt_Unauthenticated_Quota) == 0) {
                   sscanf(value, "%d", &properties.unauthenticated_quota);
               }
               else if (strcmp(token, prt_Acct_Lifetime) == 0) {
                   dt_to_period(value, &policy.acct_lifespan);
               }
               else if (strcmp(token, prt_Passwd_Min_Len) == 0) {
                   sscanf(value, "%d", &policy.passwd_min_len);
               }
               else if (strcmp(token, prt_Passwd_Lifetime) == 0) {
                   dt_to_period(value, &policy.passwd_lifetime);
               }
               else if (strcmp(token, prt_Passwd_Expire_Date) == 0) {
                   dt_to_sec(value, &policy.passwd_exp_date);
               }
               else if (strcmp(token, prt_Passwd_All_Spaces_Ok) == 0) {
                   if (strcmp(value, prt_false) == 0)
                       policy.passwd_flags |= sec_rgy_plcy_pwd_no_spaces;
               }
               else if (strcmp(token, prt_Passwd_All_Alphanumeric_Ok) == 0) {
                   if (strcmp(value, prt_false) == 0)
                       policy.passwd_flags |= sec_rgy_plcy_pwd_non_alpha;
               }
               else if (strcmp(token, prt_Max_Certificate_Lifetime) == 0) {
                   dt_to_period(value, &auth_policy.max_ticket_lifetime);
               }
               else if (strcmp(token, prt_Max_Renewable_Lifetime) == 0) {
                   dt_to_period(value, &auth_policy.max_renewable_lifetime);
               }
               else if (strcmp(token, prt_Princ_Cache_State) == 0) {
                   dt_to_sec(value, &princ_cache);
               }
               else if (strcmp(token, prt_Group_Cache_State) == 0) {
                   dt_to_sec(value, &group_cache);
               }
               else if (strcmp(token, prt_Org_Cache_State) == 0) {
                   dt_to_sec(value, &org_cache);
               }
               else if (strcmp(token, prt_Policy_Num_Attrs) == 0) {
                   sscanf(value, "%d", &num_attrs);
		   rsdb_policy_upd_num_attrs(RSDB_POLICY_OBJ_ID, num_attrs);
	       }
               else if (strcmp(token, prt_Replist_Num_Attrs) == 0) {
                   sscanf(value, "%d", &num_attrs);
		   rsdb_policy_upd_num_attrs(RSDB_REPLIST_OBJ_ID, num_attrs);
	       }
               else if (strcmp(token, prt_Attr_Schema_Num_Attrs) == 0) {
                   sscanf(value, "%d", &num_attrs);
		   rsdb_policy_upd_num_attrs(RSDB_ATTR_SCHEMA_OBJ_ID, num_attrs);
	       }
               else if (strcmp(token, prt_My_Name) == 0) {
                   sscanf(value, "%s", my_name);
                   rsdb_set_my_server_name(my_name);
               }
               else if (strcmp(token, prt_Master_Key_Version) == 0) {
                   sscanf(value, "%d", &version);
                   master_key.mkey_version = (unsigned char) version;
               }
               else if (strcmp(token, prt_Master_Key_Keytype) == 0) {
                   if (strcmp(value, prt_des) == 0)
                       master_key.keytype = KEYTYPE_DES;
                   else
                       master_key.keytype = KEYTYPE_DES;
               }
               else if (strcmp(token, prt_Master_Key_Length) == 0) {
                   sscanf(value, "%d", &master_key.length);
               }
               else if (strcmp(token, prt_Master_Key) == 0) {
                   get_key(value, master_key.length, master_key.contents);
               }
               else if (strcmp(token, prt_Obj_Acl_Def_Cell_Name) == 0) {
                   get_acl(fp, do_acl, FALSE, value, prt_Obj_Acl_Entry, 
                           rsdb_get_rgy_acl_id(), rsdb_acl_mgr_type_rgy,
			   status);
                   if (BAD_STATUS(status))
                       error_st(*status, "Unable to get properties acl");
               }
               else if (strcmp(token, prt_Obj_Acl_Def_Cell_UUID) == 0) {
                   get_acl(fp, do_acl, TRUE, value, prt_Obj_Acl_Entry,
                           rsdb_get_rgy_acl_id(), rsdb_acl_mgr_type_rgy,
			   status);
                   if (BAD_STATUS(status))
                       error_st(*status, "Unable to get properties acl");
               }
               free(line);
           }
        }
        fclose(fp);
        if (GOOD_STATUS(status) ) {
 	    if ( ! do_acl ) { 
                rsdb_properties_set_info(&properties, status);
                if (BAD_STATUS(status))
                    error_st(*status, "Unable to set properties info");
                rsdb_properties_set_realm(realm_name, &realm_uuid, status);
                if (BAD_STATUS(status))
                    error_st(*status, "Unable to set properties realm");
                /* re-set properties with correct realm */
                rsdb_properties_get_info(&properties, status);
                rsdb_policy_set_info((unsigned_char_p_t)"", &policy, status);
                if (BAD_STATUS(status))
                    error_st(*status, "Unable to set policy info");
                rsdb_auth_policy_set_info(&wild_login, &auth_policy, status);
                if (BAD_STATUS(status))
                    error_st(*status, "Unable to set auth policy info");
                rsdb_store_enc_master_key(&master_key, status);
                if (BAD_STATUS(status))
                    error_st(*status, "Unable to store master key");
	    }
        }
        else 
            error_st(*status, "Unable to reconstruct properties");
    }
    else
        *status = sec_rgy_dir_not_found;
}


 /*
  * c o n s t r u c t _ r g y _ s t a t e
  *
  * construct rgy state from print file
  *
  */
PUBLIC void construct_rgy_state ( error_status_t *status )
{
    FILE                *fopen(), *fp;
    sec_rgy_pname_t     prt_file;
    char                *line, *token, *value;
    int                 version;
    boolean32           master_known;
    uuid_t              master_id;
    rs_update_seqno_t   master_seqno;
    unsigned32	        global_sw_rev;

    strcpy((char *) prt_file, (char *) prt_path);
    strcat((char *) prt_file, PRT_FILE_RGY_STATE);

    /* open rgy_state.prt file */
    fp = fopen((char *) prt_file, "r");

    rs_state.next_rep_state = 0;

    if (fp) {
        while (line = next_line(fp)) {
           if (line) {
               parse_line(line, &token, &value); 
               if (strcmp(token, prt_Rgy_State_File_Version) == 0) {
                   sscanf(value, "%d", &version);
                   if (version != rs_c_state_file_version) {
                       *status = sec_rgy_dir_not_found;
		       free(line);
		       break;
                   }
                   else
                       rs_state.version = version;
               }
               else if (strcmp(token, prt_Replica_State) == 0) {
                   if (strcmp(value, prt_unknown_to_master) == 0)
                       rs_state.rep_state = rs_c_state_unknown_to_master;
                   else if (strcmp(value, prt_uninitialized) == 0)
                       rs_state.rep_state = rs_c_state_uninitialized;
                   else if (strcmp(value, prt_initializing) == 0)
                       rs_state.rep_state = rs_c_state_initializing;
                   else if (strcmp(value, prt_in_service) == 0)
                       rs_state.rep_state = rs_c_state_in_service;
                   else if (strcmp(value, prt_renaming) == 0)
                       rs_state.rep_state = rs_c_state_renaming;
                   else if (strcmp(value, prt_copying_database) == 0)
                       rs_state.rep_state = rs_c_state_copying_dbase;
                   else if (strcmp(value, prt_in_maintenance) == 0)
                       rs_state.rep_state = rs_c_state_in_maintenance;
                   else if (strcmp(value, prt_master_key_changing) == 0)
                       rs_state.rep_state = rs_c_state_mkey_changing;
                   else if (strcmp(value, prt_becoming_master) == 0)
                       rs_state.rep_state = rs_c_state_becoming_master;
                   else if (strcmp(value, prt_closed) == 0)
                       rs_state.rep_state = rs_c_state_closed;
                   else if (strcmp(value, prt_deleted) == 0)
                       rs_state.rep_state = rs_c_state_deleted;
               }
               else if (strcmp(token, prt_Replica_Next_State) == 0) {
                   if ( rs_state.rep_state != rs_c_state_renaming )
		       error_st(*status, 
				"error parsing invalid Replica_Next_Stat");
		   else {
		       if (strcmp(value, prt_uninitialized) == 0)
			  rs_state.next_rep_state = rs_c_state_uninitialized;
		       else 
			  if (strcmp(value, prt_in_service) == 0)
			     rs_state.next_rep_state = rs_c_state_in_service;
		   }
               }
               else if (strcmp(token, prt_Cell_UUID) == 0) {
                   uuid_from_string((unsigned char *)value, 
                                    &(rs_state.cell_sec_id), status);
               }
               else if (strcmp(token, prt_Server_UUID) == 0) {
                   uuid_from_string((unsigned char *)value, 
                                    &(rs_state.rep_id), status);
               }
               else if (strcmp(token, prt_Initialization_UUID) == 0) {
                   uuid_from_string((unsigned char *)value, 
                                    &(rs_state.init_id), status);
               }
               else if (strcmp(token, prt_Master_File_Version) == 0) {
                   sscanf(value, "%d", &version);
                   if (version != rs_c_master_info_file_version) {
                           /* error place holder for real error */
                       *status = sec_rgy_dir_not_found;
		       free(line);
		       break;
                   }
               }
               else if (strcmp(token, prt_Master_Known_Flag) == 0) {
                   if (strcmp(value, prt_true) == 0)
                       master_known = true;
                   else
                       master_known = false;
               }
               else if (strcmp(token, prt_Master_UUID) == 0) {
                   uuid_from_string((unsigned char *)value, 
                                    &master_id, status);
               }
               else if (strcmp(token, prt_Master_Seqno) == 0) {
                   sscanf(value, "%d.%d", &master_seqno.high, 
                          &master_seqno.low);
               }
               else if (strcmp(token, prt_Master_Global_Software_Rev) == 0) {
                   sscanf(value, "%d", &global_sw_rev);
               }
               free(line);
           }
        }
        fclose(fp);
        rs_master_info_create(&master_id, &master_seqno, global_sw_rev,
		status);
	if (BAD_STATUS(status)) return;
        rs_state_create(status);
	if (BAD_STATUS(status)) return;
    }
    else
        *status = sec_rgy_dir_not_found;
}


 /*
  * c o n s t r u c t _ t o w e r
  *
  * construct tower information from print file
  * 
  */
PRIVATE boolean32 construct_tower
  (
    FILE *fp,
    rs_replica_twr_vec_p_t rep_twrs,
    error_status_t *status
  )
{
    char *line, *token, *value;
    int  i, tower_len, cur_twr;

    line = next_line(fp);
    if (line) {
       parse_line(line, &token, &value); 
       if (strcmp(token, prt_Tower_Length) == 0) {
           sscanf(value, "%d", &tower_len);
           cur_twr = rep_twrs->num_towers++;
	   rep_twrs->towers[cur_twr] = NULL;
           rep_twrs->towers[cur_twr] = (twr_p_t) 
               malloc((sizeof(idl_byte) * tower_len) + sizeof(unsigned32));
           rep_twrs->towers[cur_twr]->tower_length = tower_len;
           free(line);
           line = next_line(fp);
           if (line) {
               parse_line(line, &token, &value); 
               if (strcmp(token, prt_Tower) == 0) {
                   i = 0;
                   while(i + TOWER_MOD <= tower_len) {
                       get_tower(value, TOWER_MOD, 
                           &rep_twrs->towers[cur_twr]->tower_octet_string[i]);
                       i = i + TOWER_MOD;
                       if (tower_len > i) {
                           fscanf(fp, "%s", line);
                           value = line;
                       }
                   }
                   if (tower_len > i)
                       get_tower(value, (tower_len - i), 
                           &rep_twrs->towers[cur_twr]->tower_octet_string[i]);
               }
               else {
	 	  free(line);
                  return (FALSE);
	       }
               free(line);
           }
           else
              return (FALSE);
       }
       else
          return (FALSE);
    }
    else
       return (FALSE);
    return (TRUE);
}


 /*
  * c o n s t r u c t _ r e p l i s t _ a c l
  *
  * construct replist acl
  * 
  */
PRIVATE void construct_replist_acl
  (
    FILE *fp,
    boolean32	do_acl,
    char *entry_token,
    rsdb_pvt_id_t acl_id,
    rsdb_acl_mgr_type_t type,
    error_status_t *status
  )
{
    char *line, *token, *value;

    line = next_line(fp);
    parse_line(line, &token, &value); 
    if (strcmp(token, prt_Obj_Acl_Def_Cell_Name) == 0) {
        get_acl(fp, do_acl, FALSE, value, prt_Obj_Acl_Entry,
           rsdb_get_replist_acl_id(), rsdb_acl_mgr_type_replist, status);
        if (BAD_STATUS(status))
            error_st(*status, "Unable to get replist acl");
    }
    else if (strcmp(token, prt_Obj_Acl_Def_Cell_UUID) == 0) {
        get_acl(fp, do_acl, TRUE, value, prt_Obj_Acl_Entry,
           rsdb_get_replist_acl_id(), rsdb_acl_mgr_type_replist, status);
    }
    else
	fprintf(stderr, "Invalid token string - %s, %s expected\n", token, 
			prt_Obj_Acl_Def_Cell_Name);
    free(line);
}


PRIVATE void construct_rep_prop_type
  (
    char  	*prop_type_str,
    unsigned32	*prop_type
  )
{
    if (strcmp(prop_type_str, prt_init) == 0)
        *prop_type = rs_c_replica_prop_init;
    else if (strcmp(prop_type_str, prt_initing) == 0)
        *prop_type = rs_c_replica_prop_initing;
    else if (strcmp(prop_type_str, prt_update) == 0)
        *prop_type = rs_c_replica_prop_update;
    else if (strcmp(prop_type_str, prt_delete) == 0)
        *prop_type = rs_c_replica_prop_delete;
    else if (strcmp(prop_type_str, prt_unknown) == 0)
        *prop_type = rs_c_replica_prop_unknown;
    else if (strcmp(prop_type_str, prt_bad_sw_rev) == 0)
        *prop_type = rs_c_replica_prop_bad_sw_rev;
}


 /*
  * c o n s t r u c t _ r e p _ r e c
  *
  * construct replica records
  * 
  */
PRIVATE boolean32 construct_rep_rec
  (
    FILE                  *fp,
    boolean32		  do_acl,
    rs_replica_mgt_item_t *rep_mgt_p,
    error_status_t        *status
  )
{
    char            *line, *token, *value;
    int             i, num_towers;
    sec_rgy_name_t  *rep_name;
    boolean32       got_line = FALSE;

    line = next_line(fp);
    while (line) {
       got_line = TRUE;
       parse_line(line, &token, &value); 
       if (strcmp(token, prt_Record_Number) == 0) {
	   free(line);
           break;
       }
       else if (strcmp(token, prt_Replica_UUID) == 0) {
           uuid_from_string((unsigned char *)value, 
                            &rep_mgt_p->rep_id, status);
       }
       else if (strcmp(token, prt_Replica_Name) == 0) {
	   rep_name = NEW(sec_rgy_name_t);
           strcpy((char *)rep_name, value);
           rep_mgt_p->rep_name = (rs_replica_name_p_t)rep_name;
       }
       else if (strcmp(token, prt_Replist_Name) == 0) {
           construct_replist_acl(fp, do_acl, prt_Obj_Acl_Entry, 
           	rsdb_get_replist_acl_id(), rsdb_acl_mgr_type_replist, status);
	   if ( BAD_STATUS(status) )
		error_st(*status, "Fail to reconstruct replist acl");
           free(line);
	   return(FALSE);
       }
       else if (strcmp(token, prt_Num_Towers) == 0) {
           sscanf(value, "%d", &num_towers);
           rep_mgt_p->rep_twrs = (rs_replica_twr_vec_p_t) 
               malloc((sizeof(twr_p_t) * num_towers) + sizeof(unsigned32));
           rep_mgt_p->rep_twrs->num_towers = 0;
           for (i = 0; i < num_towers; i++)
               if ( construct_tower(fp, (rs_replica_twr_vec_p_t)rep_mgt_p->rep_twrs, 
			status) == FALSE ) {
		   error_st(*status, "Fail to reconstruct tower");
                   free(line);
		   return FALSE;
	       }
       }
       else if (strcmp(token, prt_Propagation_Type) == 0) {
	   construct_rep_prop_type(value, &rep_mgt_p->prop_type);
       }
       else if (strcmp(token, prt_Prev_Propagation_Type) == 0) {
	   construct_rep_prop_type(value, &rep_mgt_p->prev_prop_type);
       }
       else if (strcmp(token, prt_Initialization_UUID) == 0) {
           uuid_from_string((unsigned char *)value, 
                            &rep_mgt_p->init_id, status);
       }
       free(line);
       line = next_line(fp);
    }
    if (got_line)
        return(TRUE);
    else
        return(FALSE);
}


 /*
  * c o n s t r u c t _ r e p l i c a s
  *
  * construct replicas
  * 
  */
PUBLIC void construct_replicas
  (
    boolean32      do_acl,
    error_status_t *status
  )
{
    FILE *fopen(), *fp;
    sec_rgy_pname_t prt_file;
    char *line, *token, *value;
    int  i;
    boolean32 next_record = FALSE;
    rs_replica_mgt_item_t rep_mgt;

    CLEAR_STATUS(status);

    strcpy((char *) prt_file, (char *) prt_path);
    strcat((char *) prt_file, PRT_FILE_REPLICAS);

    /* open replicas.prt file */
    fp = fopen((char *) prt_file, "r");

    rep_mgt.rep_name = NULL;

    if (fp) {
        line = next_line(fp);
        if (line) {
           parse_line(line, &token, &value); 
           if (strcmp(token, prt_Record_Number) == 0) 
               next_record = TRUE;
           while (next_record) {
               next_record = construct_rep_rec(fp, do_acl, &rep_mgt, status);
               if (next_record) {
		   if ( ! do_acl )
                   	rsdb_replica_add(&rep_mgt, status);
		   if ( rep_mgt.rep_name != NULL )
		       free(rep_mgt.rep_name);
               }
	       if (BAD_STATUS(status)) return;
           }
           free(line);
        }
	for ( i=rep_mgt.rep_twrs->num_towers-1; i>=0; i-- )
	    free(rep_mgt.rep_twrs->towers[i]);
	free(rep_mgt.rep_twrs);
        fclose(fp);
    }
    else
        *status = sec_rgy_dir_not_found;
}
