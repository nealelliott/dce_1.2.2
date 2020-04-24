/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: init.c,v $
 * Revision 1.1.2.2  1996/02/18  19:16:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:11  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:22  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/3  1995/07/27  20:55 UTC  tatsu_s
 * 	Submitted the fix for CHFts15852.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.local_rpc.b5/1  1995/07/26  17:20 UTC  tatsu_s
 * 	Tightened ACLs on passwd/group_override.
 * 
 * 	HP revision /main/HPDCE02/2  1995/06/26  18:50 UTC  mk
 * 	Merge in OT12573 fix from mk_dced_hostdata_init/1.
 * 
 * 	HP revision /main/HPDCE02/mk_dced_hostdata_init/1  1995/06/26  18:38 UTC  mk
 * 	Fix OT12573:  Add more hostdata files.
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/31  13:25 UTC  tatsu_s
 * 	Submitted the fix for OT12494/CHFts13959 .
 * 
 * 	HP revision /main/tatsu_s.dced_security.b0/1  1995/05/12  04:12 UTC  tatsu_s
 * 	Remove objectuuid.txt before creating.
 * 	[1995/12/08  20:48:58  root]
 * 
 * Revision 1.1.2.21  1994/09/26  17:49:20  rsalz
 * 	Fix names of CDECONFIG xattr object (OT CR 12293).
 * 	[1994/09/26  17:12:07  rsalz]
 * 
 * Revision 1.1.2.20  1994/09/22  22:00:32  bowe
 * 	Some svc work. [CR 11725]
 * 	[1994/09/22  21:42:46  bowe]
 * 
 * Revision 1.1.2.19  1994/09/22  18:01:02  bowe
 * 	Added cde session xattr object to init code. [CR 12293]
 * 	[1994/09/22  17:35:41  bowe]
 * 
 * Revision 1.1.2.18  1994/09/17  08:05:11  rsalz
 * 	Store dced object UUID in a file (OT CR 12127).
 * 	Remote ep-compat fake server (OT CR 12140).
 * 	[1994/09/17  07:56:51  rsalz]
 * 
 * Revision 1.1.2.17  1994/09/06  18:15:27  bowe
 * 	Write out entries for host and cell names to postproc file. [CR 11705]
 * 	[1994/09/06  18:14:35  bowe]
 * 
 * Revision 1.1.2.16  1994/08/26  20:13:03  bowe
 * 	Add code for hd entries for pe_site & routing file. [CR 11896]
 * 	Some svc work. [CR 11725]
 * 	[1994/08/26  20:10:07  bowe]
 * 
 * Revision 1.1.2.15  1994/08/22  15:50:49  bowe
 * 	Fix uninitialized field in struct (init_srvrconf) [CR 11795]
 * 	[1994/08/22  14:49:02  bowe]
 * 
 * Revision 1.1.2.14  1994/07/29  19:56:08  rsalz
 * 	Create keytab for host principal at init time (OT CR 11465).
 * 	[1994/07/29  19:37:13  rsalz]
 * 
 * Revision 1.1.2.13  1994/07/15  14:58:11  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/15  10:08:34  mdf]
 * 
 * Revision 1.1.2.12  1994/06/21  19:02:01  rsalz
 * 	Properly create hostdata objects (OT CR 11006).
 * 	[1994/06/21  19:01:13  rsalz]
 * 
 * Revision 1.1.2.11  1994/06/17  19:37:58  rsalz
 * 	Create hostdata objects; don't just_import them.
 * 	[1994/06/17  19:30:31  rsalz]
 * 
 * Revision 1.1.2.10  1994/06/01  16:22:51  rsalz
 * 	Fix -e flag; make init more robust
 * 	[1994/06/01  16:22:31  rsalz]
 * 
 * Revision 1.1.2.9  1994/05/27  16:31:13  rsalz
 * 	Fix bootstrap phase; cleanup EP and LLB compat code
 * 	[1994/05/27  16:30:22  rsalz]
 * 
 * Revision 1.1.2.8  1994/05/26  19:42:13  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:44  rsalz]
 * 
 * 	Checkpointing.
 * 	[1994/05/18  19:32:59  rsalz]
 * 
 * Revision 1.1.2.7  1994/05/10  20:15:12  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:42  rsalz]
 * 
 * Revision 1.1.2.6  1994/05/05  20:32:43  rsalz
 * 	Fix merge problems from previous release
 * 	[1994/05/05  20:32:01  rsalz]
 * 
 * Revision 1.1.2.5  1994/05/05  20:07:35  rsalz
 * 	Merged with changes from 1.1.2.4
 * 	[1994/05/05  20:07:00  rsalz]
 * 
 * 	Initial server functionality drop.
 * 	[1994/05/05  20:01:00  rsalz]
 * 
 * Revision 1.1.2.4  1994/05/05  14:41:16  pwang
 * 	Rename dced_c_service_rkeytab to dced_c_service_keytab.
 * 	[1994/05/05  14:37:54  pwang]
 * 
 * Revision 1.1.2.3  1994/04/21  03:55:15  rsalz
 * 	#if-out verbose printf's (will become SVC calls).
 * 	[1994/04/21  03:54:14  rsalz]
 * 
 * Revision 1.1.2.2  1994/03/29  22:31:27  bowe
 * 	Added name->uuid cach maintainence code
 * 	[1994/03/29  22:23:22  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:53  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:22  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <libimpl.h>

/* ______________________________________________________________________ */

/* Initialize the required hostdata objects/files:
 *	host_name, cell_name, dce_cf.db
 */

/* If file exists in dced's directory, report error and exit. */
static void
remove_file(char *file)
{
    if (unlink(file) < 0 && errno != ENOENT) {
	dce_svc_printf(DCED_S_INIT_REMOVE_FILE_FAIL_MSG, file, strerror(errno));
	exit(1);
    }
}

/* Default list of postprocessors to write to the file. */
static uuid_t *cf_pp_list[] = {
    &dced_g_uuid_cell_name, &dced_g_uuid_host_name, &dced_g_uuid_cell_aliases
};
#define sizeof_cf_pp_list	(sizeof cf_pp_list / sizeof cf_pp_list[0])

static	void
init_hostdata(
    error_status_t	*st
)
{
    char		*name, buf[1024];
    sec_attr_t		Attr;
    dced_entry_t	Entry;
    sec_attr_enc_str_array_t	*sa;
    unsigned32		i;
    dced_entry_data_t	id_d;

    *st = error_status_ok;

    /* Does dce_cf.db exist? */
    sprintf(buf, "%s/%s", dcelocal_path, dced_c_dce_cf_db);
    if (access(buf, F_OK) != 0) {
	if (creat(buf, 0666) != 0) {
	    dce_svc_printf(DCE_SVC(dhd_svc_handle, ""), dhd_s_general,
		svc_c_sev_fatal, dced_s_init_create_file_fail, buf,
		strerror(errno));
	}
    }

    /* Creat the items */
    Attr.attr_id = dced_g_uuid_fileattr;
    Attr.attr_value.attr_encoding = sec_attr_enc_printstring_array;
    sa = (sec_attr_enc_str_array_t *)malloc(sizeof(sec_attr_enc_str_array_t) +
		sizeof(sec_attr_enc_printstring_p_t) * 32);
    Attr.attr_value.tagged_union.string_array = sa;
    Attr.attr_value.tagged_union.string_array->num_strings = 1;

    Entry.name        = (idl_char *)dced_c_dce_cf_db;
    Entry.storage_tag = (idl_char *)buf;
    Entry.description = (idl_char *)"DCE name config file";
    Entry.id          = dced_g_uuid_dce_cf_db;
    sa->strings[0] = (idl_char *)"# DCE name config";
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, TRUE, &Attr, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, dced_c_dce_cf_db,
		strerror(errno));

    remove_file(dced_c_host_name);
    Entry.name        = Entry.storage_tag = (idl_char *)dced_c_host_name;
    Entry.description = (idl_char *)"Name of host";
    Entry.id          = dced_g_uuid_host_name;
    sa->num_strings = 1;
    dce_cf_get_host_name(&name, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_INIT_FUNCTION_FAIL_MSG, "dce_cf_get_host_name", (long)*st);
	sa->num_strings = 0;
    }
    else
	sa->strings[0] = (idl_char *)name;
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, FALSE, &Attr, st);
    if (name[0])
	free(name);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, dced_c_host_name,
		strerror(errno));

    remove_file(dced_c_cell_name);
    Entry.name        = Entry.storage_tag = (idl_char *)dced_c_cell_name;
    Entry.description = (idl_char *)"Name of cell";
    Entry.id          = dced_g_uuid_cell_name;
    sa->num_strings = 1;
    dce_cf_get_cell_name(&name, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_INIT_FUNCTION_FAIL_MSG, "dce_cf_get_cell_name", (long)*st);
	sa->num_strings = 0;
    }
    else
	sa->strings[0] = (idl_char *)name;
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, FALSE, &Attr, st);
    if (name[0])
	free(name);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, dced_c_cell_name,
		strerror(errno));

    remove_file(dced_c_cell_aliases);
    Entry.name        = Entry.storage_tag = (idl_char *)dced_c_cell_aliases;
    Entry.description = (idl_char *)"Cell aliases";
    Entry.id          = dced_g_uuid_cell_aliases;
    sa->strings[0] = (idl_char *)"# cell aliases";
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, FALSE, &Attr, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, dced_c_cell_aliases,
		strerror(errno));

    remove_file(dced_c_hostdata_post_proc);
    Entry.name        = Entry.storage_tag = (idl_char *)dced_c_hostdata_post_proc;
    Entry.description =
	(idl_char *)"Post-processors for hostdata object changes";
    Entry.id          = dced_g_uuid_hostdata_post_proc;
    sa->strings[0] = (idl_char *)"# post-processors";
    sa->num_strings = 1;
    /* Default entries for postprocessors file */
    for (i=0; i < sizeof_cf_pp_list; i++) {
	unsigned_char_t	*ucp;
	uuid_to_string(cf_pp_list[i], &ucp, st);
	sprintf(buf, "%s %s/bin/%s", ucp, dcelocal_path, DCECF_POSTPROC);
	free(ucp);
	sa->strings[sa->num_strings] = (idl_char *)dce_strdup(buf);
	sa->num_strings++;
    }
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, FALSE, &Attr, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG,
		dced_c_hostdata_post_proc, strerror(errno));
    /* free the dup'ed strings */
    for (i=1; i<sa->num_strings; i++)
	rpc_string_free(&sa->strings[i], st);

    sprintf(buf, "%s/%s", dcelocal_path, dced_c_pe_site_file);
    Entry.name        = (idl_char *)dced_c_pe_site;
    Entry.storage_tag = (idl_char *)buf;
    Entry.description = (idl_char *)"PE Site file";
    Entry.id          = dced_g_uuid_pe_site;
    sa->num_strings = 0;
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, TRUE, &Attr, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, buf, strerror(errno));

    sprintf(buf, "%s/%s", dcelocal_path, dced_c_svc_routing_file);
    Entry.name        = (idl_char *)dced_c_svc_routing;
    Entry.storage_tag = (idl_char *)buf;
    Entry.description = (idl_char *)"Serviceability file";
    Entry.id          = dced_g_uuid_svc_routing;
    sa->num_strings = 0;
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, TRUE, &Attr, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, buf, strerror(errno));

    sprintf(buf, "%s", dced_c_krb_conf_file);
    Entry.name        = (idl_char *)dced_c_krb_conf;
    Entry.storage_tag = (idl_char *)buf;
    Entry.description = (idl_char *)"Kerberos configuration file";
    Entry.id          = dced_g_uuid_krb_conf;
    sa->num_strings = 0;
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, TRUE, &Attr, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, buf, strerror(errno));

    sprintf(buf, "%s/%s", dcelocal_path, dced_c_dfs_cache_info_file);
    Entry.name        = (idl_char *)dced_c_dfs_cache_info;
    Entry.storage_tag = (idl_char *)buf;
    Entry.description = (idl_char *)"DFS CacheInfo file";
    Entry.id          = dced_g_uuid_dfs_cache_info;
    sa->num_strings = 0;
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, TRUE, &Attr, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, buf, strerror(errno));

    sprintf(buf, "%s/%s", dcelocal_path, dced_c_cds_conf_file);
    Entry.name        = (idl_char *)dced_c_cds_conf;
    Entry.storage_tag = (idl_char *)buf;
    Entry.description = (idl_char *)"CDS configuration file";
    Entry.id          = dced_g_uuid_cds_conf;
    sa->num_strings = 0;
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, TRUE, &Attr, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, buf, strerror(errno));

    sprintf(buf, "%s/%s", dcelocal_path, dced_c_cds_attributes_file);
    Entry.name        = (idl_char *)dced_c_cds_attributes;
    Entry.storage_tag = (idl_char *)buf;
    Entry.description = (idl_char *)"object identifiers for CDS attributes";
    Entry.id          = dced_g_uuid_cds_attributes;
    sa->num_strings = 0;
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, TRUE, &Attr, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, buf, strerror(errno));

    sprintf(buf, "%s/%s", dcelocal_path, dced_c_cds_globalnames_file);
    Entry.name        = (idl_char *)dced_c_cds_globalnames;
    Entry.storage_tag = (idl_char *)buf;
    Entry.description = (idl_char *)"X.500 naming attributes supported by CDS";
    Entry.id          = dced_g_uuid_cds_globalnames;
    sa->num_strings = 0;
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, TRUE, &Attr, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, buf, strerror(errno));

    sprintf(buf, "%s/%s", dcelocal_path, dced_c_passwd_override_file);
    Entry.name        = (idl_char *)dced_c_passwd_override;
    Entry.storage_tag = (idl_char *)buf;
    Entry.description = (idl_char *)"DCE password override file";
    Entry.id          = dced_g_uuid_passwd_override;
    sa->num_strings = 0;
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, TRUE, &Attr, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, buf, strerror(errno));

    sprintf(buf, "%s/%s", dcelocal_path, dced_c_group_override_file);
    Entry.name        = (idl_char *)dced_c_group_override;
    Entry.storage_tag = (idl_char *)buf;
    Entry.description = (idl_char *)"DCE group override file";
    Entry.id          = dced_g_uuid_group_override;
    sa->num_strings = 0;
    (*dced_hostdata_v1_0_epv.hostdata_create)(NULL, &Entry, TRUE, &Attr, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_INIT_CREATE_FILE_FAIL_MSG, buf, strerror(errno));

    free(sa);
    *st = error_status_ok;
}

static	void
init_rkeytab(
    error_status_t	*st
)
{
    dced_entry_t	Entry;

    Entry.name        = (idl_char *)"self";
    Entry.storage_tag = (idl_char *)"/krb5/v5srvtab";
    Entry.description = (idl_char *)"Host Principal Keytab";
    Entry.id          = dced_g_uuid_host_keyfile;
    (*dced_rkeytab_v1_0_epv.rkeytab_create)(NULL, &Entry, TRUE, NULL, st);
}

static idl_char dced_g_scope[] = "dced";
static idl_char dced_g_comment[] = "Well-known dced attribute";
static sec_attr_schema_entry_t dced_g_schema_fileattr = {
    (idl_char *)dced_c_filecontents_name, { 0 }, sec_attr_enc_printstring_array,
    NULL, (sec_attr_sch_entry_reserved), sec_attr_intercell_act_accept,
    sec_attr_trig_type_none, NULL,
    dced_g_scope, dced_g_comment
};
static sec_attr_schema_entry_t dced_g_schema_binfileattr = {
    (idl_char *)dced_c_bincontents_name, { 0 }, sec_attr_enc_bytes,
    NULL, sec_attr_sch_entry_reserved, sec_attr_intercell_act_accept,
    sec_attr_trig_type_none, NULL,
    dced_g_scope, dced_g_comment
};
static sec_attr_schema_entry_t dced_g_schema_dtsconfattr = {
    (idl_char *)dced_c_dtsconfattr_name, { 0 }, sec_attr_enc_printstring_array,
    NULL, sec_attr_sch_entry_reserved, sec_attr_intercell_act_accept,
    sec_attr_trig_type_none, NULL,
    dced_g_scope, dced_g_comment
};
static sec_attr_schema_entry_t dced_g_schema_cdeconfattr = {
    (idl_char *)dced_c_cdeses_name, { 0 }, sec_attr_enc_printstring_array,
    NULL, sec_attr_sch_entry_reserved, sec_attr_intercell_act_accept,
    sec_attr_trig_type_none, NULL,
    dced_g_scope, dced_g_comment
};
static sec_attr_schema_entry_t dced_g_schema_envaddattr = {
    (idl_char *)dced_c_envaddattr_name, { 0 }, sec_attr_enc_printstring_array,
    NULL, sec_attr_sch_entry_reserved, sec_attr_intercell_act_accept,
    sec_attr_trig_type_none, NULL,
    dced_g_scope, dced_g_comment
};

static	void
init_attr_schema(
    error_status_t		*st
)
{
    rs_cache_data_t		rscache;
    sec_attr_acl_mgr_info_set_t *acl_mgr_set;
    sec_attr_acl_mgr_info_t	acl_mgr[2];

    acl_mgr_set = malloc(sizeof *acl_mgr_set
		    + sizeof (sec_attr_acl_mgr_info_t)); 
    acl_mgr_set->mgr_info[0] = &acl_mgr[0];
    acl_mgr_set->mgr_info[1] = &acl_mgr[1];

    acl_mgr_set->num_acl_mgrs = 1;
    acl_mgr[0].acl_mgr_type   = hd_acl;
    acl_mgr[0].query_permset  = dced_perm_read;
    acl_mgr[0].update_permset = dced_perm_write;
    acl_mgr[0].test_permset   = dced_perm_read;
    acl_mgr[0].delete_permset = dced_perm_delete;

    dced_g_schema_fileattr.attr_id     = dced_g_uuid_fileattr;
    dced_g_schema_fileattr.acl_mgr_set = acl_mgr_set;
    (*dced_rs_attr_schema_v0_0_epv.rs_attr_schema_create_entry)
	(NULL, dced_g_scope, &dced_g_schema_fileattr,
	 &rscache, st);
    dced_g_schema_binfileattr.attr_id     = dced_g_uuid_binfileattr;
    dced_g_schema_binfileattr.acl_mgr_set = acl_mgr_set;
    (*dced_rs_attr_schema_v0_0_epv.rs_attr_schema_create_entry)
	(NULL, dced_g_scope, &dced_g_schema_binfileattr,
	 &rscache, st);

    acl_mgr_set->num_acl_mgrs = 2;
    acl_mgr[0].acl_mgr_type   = sc_acl;
    acl_mgr[0].query_permset  = dced_perm_read;
    acl_mgr[0].update_permset = dced_perm_write;
    acl_mgr[0].test_permset   = dced_perm_read;
    acl_mgr[0].delete_permset = dced_perm_delete;
    acl_mgr[1].acl_mgr_type   = sx_acl;
    acl_mgr[1].query_permset  = dced_perm_read;
    acl_mgr[1].update_permset = dced_perm_write;
    acl_mgr[1].test_permset   = dced_perm_read;
    acl_mgr[1].delete_permset = dced_perm_delete;

    dced_g_schema_dtsconfattr.attr_id     = dced_g_uuid_dtsconfattr;
    dced_g_schema_dtsconfattr.acl_mgr_set = acl_mgr_set;
    (*dced_rs_attr_schema_v0_0_epv.rs_attr_schema_create_entry)
	(NULL, dced_g_scope, &dced_g_schema_dtsconfattr,
	 &rscache, st);
    dced_g_schema_envaddattr.attr_id      = dced_g_uuid_envaddattr;
    dced_g_schema_envaddattr.acl_mgr_set  = acl_mgr_set;
    (*dced_rs_attr_schema_v0_0_epv.rs_attr_schema_create_entry)
	(NULL, dced_g_scope, &dced_g_schema_envaddattr,
	 &rscache, st);
    dced_g_schema_cdeconfattr.attr_id     = dced_g_uuid_cdeses;
    dced_g_schema_cdeconfattr.acl_mgr_set = acl_mgr_set;
    (*dced_rs_attr_schema_v0_0_epv.rs_attr_schema_create_entry)
	(NULL, dced_g_scope, &dced_g_schema_cdeconfattr,
	 &rscache, st);

    free(acl_mgr_set);
}

void
dced_init_databases(
    error_status_t		*st
)
{
    init_acl_database(st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
		       "ACL database", (long)*st);
    init_hostdata(st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
		       "hostdata database", (long)*st);
    init_rkeytab(st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
		       "keytab database", (long)*st);
    init_attr_schema(st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
		       "xattrschema database", (long)*st);
    return;
}

/* ______________________________________________________________________ */
