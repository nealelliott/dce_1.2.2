/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: dmn_acl_mgr.c,v $
 * Revision 1.1.8.1  1996/07/08  18:24:55  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:23 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:05:58  marty  1.1.6.2]
 *
 * Revision 1.1.6.2  1996/02/18  00:05:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:34  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:38:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:17:31  root]
 * 
 * Revision 1.1.2.11  1994/08/12  19:20:32  blazin
 * 	For CR 11431 added support for audit-admin group.
 * 	[1994/08/12  19:20:09  blazin]
 * 
 * Revision 1.1.2.10  1994/07/28  21:00:51  rousseau
 * 	Fix registration order and swap wrong out of order error messages.
 * 	[1994/07/28  20:57:54  rousseau]
 * 
 * Revision 1.1.2.9  1994/07/26  17:27:03  weisz
 * 	CR 11425: remove own acl manager and use DCE acl mgr library.
 * 	[1994/07/26  17:24:52  weisz]
 * 
 * Revision 1.1.2.8  1994/07/18  21:12:20  weisz
 * 	CR 10152: removed some printf constant messages.
 * 	[1994/07/18  21:11:53  weisz]
 * 
 * Revision 1.1.2.7  1994/07/07  19:46:05  rsalz
 * 	Use dmn_acl.h because dce/svcacl.h is going away (OT CR 10856).
 * 	[1994/07/07  19:04:59  rsalz]
 * 
 * Revision 1.1.2.6  1994/05/31  19:57:07  weisz
 * 	CR 10810: change sec_acl_printstring from to "crw", "crw" to "audit",
 * 	          and "Audit daemon ACL manager" respectively.
 * 	[1994/05/31  19:51:41  weisz]
 * 
 * Revision 1.1.2.5  1994/02/10  17:06:04  luan
 * 	CR 9866: Fixed uninitialized hostprin variable in
 * 	acl__create_default(), also changed its type from unsigned char *
 * 	to sec_rgy_name_t.
 * 	[1994/02/09  22:59:25  luan]
 * 
 * Revision 1.1.2.4  1994/01/28  23:09:53  burati
 * 	Use rdaclifv0.h until integrated with rdaclif v1.0 functionality
 * 	[1994/01/26  23:03:20  burati]
 * 
 * Revision 1.1.2.3  1994/01/25  04:59:32  luan
 * 	CR 9745 9746: File name paths consolidated.
 * 	Hard-coded file names replaced with macros
 * 	which generate names dynamically.
 * 	[1994/01/25  04:00:47  luan]
 * 
 * Revision 1.1.2.2  1994/01/21  20:48:55  luan
 * 	Changed to use <dce/svcacl.h> exported by dce/utils/svc.
 * 	The local (security/audit/auditd) svcacl.h is defuncted.
 * 	[1994/01/21  20:41:35  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:38:43  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:03  zeliff]
 * 
 * $EndLog$
 */

/*
** dmn_acl_mgr.c
** Initializes audit daemon ACL.
** Access control routines for audit daemon.
** Manager routines for audit daemon's rdacl interface.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <dce/dce.h>
#include <dce/id_base.h>
#include <dce/secidmap.h>
#include <dce/rpc.h>
#include <dce/aclif.h>
#include <dce/rdaclif.h>  /* New ACL interface */
#include <dce/rdaclifv0.h>
#include <dce/dceacl.h>
#include <dce/dce_cf.h>
#include <dce/dcesvcmsg.h>
#include <dce/dceaudmsg.h>
#include <dce/dceaudmac.h>
#include <dce/dceaudsvc.h>
#include <dce/audit.h>
#include <error_statusp.h>

#define sec_acl_perm_log sec_acl_perm_unused_00000080

/*
**  The following data must be set up by the server-specific code.
*/

extern char                   *auditd_identity;
uuid_t			       my_type_uuid;
dce_db_handle_t                acl_db;
char                          *my_type_uuid_string = "0022FF10-73FA-1C54-830B-02608C0FF790";
extern char	              *daemon_acl_file;
static sec_acl_printstring_t   audit_acl_help =
{
        "audit",        "Audit server", (sec_acl_permset_t)0
};
static sec_acl_printstring_t    my_printstrings[] =
{
    {   "c",    "control",      sec_acl_perm_owner },
    {   "r",    "read",         sec_acl_perm_read },
    {   "w",    "write",        sec_acl_perm_write },
    {   "l",    "log",          sec_acl_perm_log }
};

/*
**  Private data.
*/

static sec_acl_p_t	my_ap;
static pthread_mutex_t	my_mutex;

static void
audit_acl_resolve (
    handle_t                   handle,
    sec_acl_component_name_t   component_name,
    sec_acl_type_t             sec_acl_type,
    uuid_t                     *mgr_type_p,
    boolean32                  writing,
    void                       *resolver_arg_p,
    uuid_t                     *acl_uuid_p,
    error_status_t             *stp
)
{
    *acl_uuid_p = my_type_uuid;
    *stp = error_status_ok;
    return;
}

/*
**  Create a default ACL with the following entries:
**  	(<local-machine-principal>     cwrl)
**  	(ANY_OTHER			  r)
**  	(UNAUTHENTICATED		  r)
*/

static void
acl__create_default(
    sec_acl_t		**app,
    error_status_t	*st
)
{
    error_status_t      stp;
    sec_acl_t		*ap;
    sec_acl_entry_t	*ep;
    sec_rgy_handle_t	h;
    unsigned char	*cellname;
    sec_rgy_name_t	hostprin;
    sec_rgy_name_t	groupname;
    boolean32           groupname_found = FALSE;
    sec_rgy_name_t	audit_admin_group_name = "subsys/dce/audit-admin";
    uuid_t		celluuid;
    uuid_t		hostuuid;
    uuid_t		groupuuid;
    sec_acl_permset_t   permset = (sec_acl_permset_t) 0;

    /* Get some config info. */
    dce_cf_get_cell_name((char **)&cellname, st);
    if (*st != dce_cf_st_ok)
	return;
    sec_rgy_site_open(cellname, &h, st);
    if (*st != error_status_ok)
	return;
    sec_id_parse_name(h, cellname, NULL,  &celluuid, NULL, NULL, st);
    if (*st != error_status_ok) {
        sec_rgy_site_close(h, &stp);
	return;
    }
    sec_id_parse_name(h, (unsigned_char_t *)auditd_identity, NULL,  NULL, hostprin, &hostuuid, st);
    if (*st != error_status_ok) {
        sec_rgy_site_close(h, &stp);
	return;
    }

    /*
     * Get the default audit server's group name.  Later on, if it has been
     * found not to exist, we will omit this acl entry. 
     */

    sec_id_parse_group(h, audit_admin_group_name,  NULL,  NULL, groupname, &groupuuid, st);
    if (*st == error_status_ok)
	groupname_found = TRUE;

    sec_rgy_site_close(h, st);
    if (*st != error_status_ok)
	return;

    /* Get storage for the new acl; fill in the header. */
    ap = *app = (sec_acl_t *)malloc(sizeof *ap);
    dce_acl_obj_init(&my_type_uuid, ap, st);

    if (*st != error_status_ok)
    {
       dce_svc_printf( AUD_S_DMN_ACL_OBJ_INIT_MSG, *st);
       return;
    }

    /* Allow "cwrl" access to the host machine's principal. */
    permset = sec_acl_perm_owner | sec_acl_perm_read |
                sec_acl_perm_write | sec_acl_perm_log;
    dce_acl_obj_add_user_entry(ap, permset, &hostuuid, st);

    if (*st != error_status_ok)
    {
       dce_svc_printf( AUD_S_DMN_ACL_OBJ_ADD_USER_ENTRY_MSG, *st);
       return;
    }

    /* Allow "cwrl" access to the audit-admin group if it exists. */

    if (groupname_found)
    {
       dce_acl_obj_add_group_entry(ap, permset, &groupuuid, st);

       if (*st != error_status_ok)
       {
           dce_svc_printf( AUD_S_DMN_ACL_OBJ_ADD_GROUP_ENTRY_MSG, *st);
       }
    }

    /* Allow "r" access to any other principals. */
    permset = sec_acl_perm_read;
    dce_acl_obj_add_obj_entry(ap, sec_acl_e_type_any_other, permset, st);

    if (*st != error_status_ok)
    {
       dce_svc_printf( AUD_S_DMN_ACL_OBJ_ADD_OBJ_ENTRY_MSG, *st);
       return;
    }

    /* Allow "r" access to unauthenticated users. */
    permset = sec_acl_perm_read;
    dce_acl_obj_add_unauth_entry(ap, permset, st);

    if (*st != error_status_ok)
       dce_svc_printf( AUD_S_DMN_ACL_OBJ_ADD_UNAUTH_ENTRY_MSG, *st);
}

/*
**  Initialize the ACL.  Set up the mutex.  Try to read ACL from backing
**  storage; init and write it out if that fails.
*/

void
acl_init(
    char		*file,
    error_status_t	*st
)
{
    sec_acl_t           local_acl;
    sec_acl_p_t         ap = &local_acl;
    boolean32           must_create_default_acl = FALSE;
    boolean32           must_store_acl = FALSE;

    if (pthread_mutex_init(&my_mutex, pthread_mutexattr_default) < 0) {
	*st = errno;
	return;
    }
    uuid_from_string((unsigned_char_t *)my_type_uuid_string, &my_type_uuid, st);
    if (BAD_STATUS(st)) {
       return;
    }
     /* enable stub management memory allocation, for backing store code */
     rpc_sm_enable_allocate(st);
     if (*st != error_status_ok) return;

    /* Got a valid file? */
    if (access(file, R_OK) == 0)   /* file exists */
    {
       /* open the auditd acl database */
       dce_db_open(file, (const char *)0, db_c_index_by_uuid,
                    (dce_db_convert_func_t)dce_rdacl_convert,
                     &acl_db, st);
       if (*st != error_status_ok)
       {
          dce_svc_printf( AUD_S_DMN_DB_OPEN_MSG, *st);
          return;
       }
       ap = &local_acl;
       /* try and fetch from database */
       dce_db_fetch_by_uuid(acl_db, &my_type_uuid, ap, st);
       if (*st != error_status_ok)    /* can't get it */
            must_create_default_acl = TRUE;
    }
    else
    {
       must_store_acl = TRUE;
       must_create_default_acl = TRUE;

       dce_db_open(file, (const char *)0, db_c_index_by_uuid | db_c_create,
                    (dce_db_convert_func_t)dce_rdacl_convert,
                     &acl_db, st);
       if (*st != error_status_ok)
       {
          dce_svc_printf( AUD_S_DMN_DB_OPEN_CREATE_MSG, *st);
          (void)remove(file);
          return;
       }
    }

    {
    /*
     *  always register object types for this database
     *  also register a private resolver function to map
     *  input parms to a uuid.
     */
      unsigned32  print_str_size = (sizeof my_printstrings /
                                   sizeof my_printstrings[0]);

       dce_acl_register_object_type(acl_db,
                                    &my_type_uuid,
                                    print_str_size,
                                    my_printstrings,
                                    &audit_acl_help,
                                    sec_acl_perm_control, 0,
                                    audit_acl_resolve, (void *)NULL, 0,
                                    st);
       if (*st != error_status_ok)
       {
          dce_svc_printf( AUD_S_DMN_ACL_REGISTER_OBJ_TYPE_MSG, *st);
          return;
       }
    }

    if (must_create_default_acl)
    {
       /* No acl file, or it was corrupt.  Create a default acl to use
        * and write it back out to the file. */
       acl__create_default(&ap, st);
       if (*st != error_status_ok) return;
        must_store_acl = TRUE;
    }

    if (must_store_acl)
    {
       dce_db_store_by_uuid(acl_db, &my_type_uuid, ap, st);
       if (*st != error_status_ok)
       {
          dce_svc_printf( AUD_S_DMN_DB_STORE_UUID_MSG, *st);
          return;
       }
    }

    my_ap = ap;
}

/*
**  Register the rdacl interface.
*/

extern rdaclif_v1_0_epv_t dce_acl_v1_0_epv;
extern rdaclif_v0_0_epv_t dce_acl_v0_0_epv;

void
acl_register_if(
    unsigned_char_t		*entry_name,
    rpc_binding_vector_t	*bindings,
    uuid_vector_t		*uuidvec,
    error_status_t		*st
)
{
    /* register version 1 */
    rpc_server_register_if(rdaclif_v1_0_s_ifspec,
                           (uuid_t *) NULL,
                           (rpc_mgr_epv_t)&dce_acl_v1_0_epv,
                           st);
    if (*st != rpc_s_ok) return;

    rpc_ep_register(rdaclif_v1_0_s_ifspec, bindings, uuidvec,
            (unsigned char *)"Audit ACL Manager", st);
    if (*st != rpc_s_ok) return;

    /* register version 0 as well */
    rpc_server_register_if(rdaclif_v0_0_s_ifspec,
                           (uuid_t *) NULL,
                           (rpc_mgr_epv_t)&dce_acl_v0_0_epv,
                           st);
    if (*st != rpc_s_ok) return;

    rpc_ep_register(rdaclif_v0_0_s_ifspec, bindings, uuidvec,
            (unsigned char *)"Audit ACL Manager", st);

    rpc_ns_binding_export(rpc_c_ns_syntax_dce, entry_name,
        rdaclif_v0_0_s_ifspec, bindings, uuidvec, st);

    rpc_ns_binding_export(rpc_c_ns_syntax_dce, entry_name,
        rdaclif_v1_0_s_ifspec, bindings, uuidvec, st);
}

/*
**  Check for log access.
*/

void
acl_test_log_access(
    handle_t			h,
    boolean32			*result,
    error_status_t		*st
)
{
    dce_acl_is_client_authorized(h, &my_type_uuid, &my_type_uuid,
                                 (uuid_t *)0, (uuid_t *)0,
                                 (sec_acl_permset_t)sec_acl_perm_log,
                                  result, st);
}

/*
**  Check for read access.
*/

void
acl_test_read_access(
    handle_t			h,
    boolean32			*result,
    error_status_t		*st
)
{
    dce_acl_is_client_authorized(h, &my_type_uuid, &my_type_uuid,
                                 (uuid_t *)0, (uuid_t *)0,
                                 (sec_acl_permset_t)sec_acl_perm_read,
                                  result, st);
}

/*
**  Check for write access.
*/

void
acl_test_write_access(
    handle_t			h,
    boolean32			*result,
    error_status_t		*st
)
{
    dce_acl_is_client_authorized(h, &my_type_uuid, &my_type_uuid,
                                 (uuid_t *)0, (uuid_t *)0,
                                 (sec_acl_permset_t)sec_acl_perm_write,
                                  result, st);
}

/*
**  Check for control access.
*/

void
acl_test_control_access(
    handle_t			h,
    boolean32			*result,
    error_status_t		*st
)
{
    dce_acl_is_client_authorized(h, &my_type_uuid, &my_type_uuid,
                                 (uuid_t *)0, (uuid_t *)0,
                                 (sec_acl_permset_t)sec_acl_perm_control,
                                  result, st);
}
