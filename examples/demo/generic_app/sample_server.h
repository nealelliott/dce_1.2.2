/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: sample_server.h,v $
 * Revision 1.1.7.3  1996/02/18  23:38:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:19  marty]
 *
 * Revision 1.1.7.2  1995/12/08  20:54:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:49:59  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:00  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:15  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/21  19:13:33  weir
 * 	Submit sample program sources (CR 12635)
 * 	[1994/10/21  19:12:47  weir]
 * 
 * $EndLog$
 */
/****************************************************************************/
/* [26.IX.94]                                                               */
/*                                                                          */
/* sample_server.h --                                                       */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*                            -77 cols-                                     */
/****************************************************************************/

/*                                                                          */
/* The following is passed via server_acl_mgr_setup() to the calls to       */
/*  server_create_acl(), where it is used to get a UUID to put in the       */
/*  ACLs we are creating, which will identify a user. In other words, at    */
/*  present the application is set up in such a way as to allow only the    */
/*  Cell Administrator principal to be a user with any kind of permissions  */
/*  at all on the objects we are creating. That's why the client can only   */
/*  be run successfully by a user dce_login'd as "cell_admin". The reason   */
/*  for doing things this way is that it allows us to have a user principal */
/*  we can always rely on being present, and thus avoid having to set up    */
/*  some user principals ourselves. Not that this would be so hard...       */
#define SAMPLE_OWNER "cell_admin"

/* Keytab file name:                                                        */
#define KEYTAB "FILE:/tmp/sample_keytab"

/* Default leaf name and length for the server entry:                       */
#define NAMELEN 20
#define DEFNAME "sample_server_entry"

#define IF_ANNOTATION "Sample interface, version 1.0"

/* At present we set up only two objects...                                 */
/* A "well-known" residual name for the management "object":                */
#define MGMT_OBJ_NAME "server_mgmt"
/*                                                                          */
/* A residual name for a sample object:                                     */
#define SAMPLE_OBJECT_NAME "sample_object"

/* Relative pathname at which to locate newly-created backing store         */
/*  databases. Note that this is interpreted as the name of a subdirectory: */
#define ACL_DB_PATH "db_sample_acl"
/* Maximum length of a database pathname string...                          */
#define MAX_ACL_PATH_SIZE 50

/* Time allowed for cleanup of name space:                                  */
#define CLEANUPTIME 60

/* Maximum number of serviceability routings allowed:                       */
#define MAX_ROUTES 10

/* Maximum number of separate debug levels allowed:                         */
#define MAX_LEVELS 9



/* Data structure for holding server entry names to pass to                 */
/*  server_export_objects():                                                */
typedef struct {
	unsigned32 count;
	unsigned_char_t *name[1];
} entryname_vector_t;




/* Handle for serviceability calls                                          */
dce_svc_handle_t smp_svc_handle;

/* Sample server-specific definitions:                                      */

/* Used by remote bind interface...                                         */
/* These are the backing store database handles:                            */
dce_db_handle_t db_acl, db_object, db_name;

#define mgmt_perm_inq_if sec_acl_perm_unused_00000080 
#define mgmt_perm_inq_pname sec_acl_perm_unused_00000100
#define mgmt_perm_inq_stats sec_acl_perm_unused_00000200
#define mgmt_perm_ping sec_acl_perm_unused_00000400
#define mgmt_perm_kill sec_acl_perm_unused_00000800



/* The constants below come from aclbase.h (aclbase.idl)...                 */
#define OBJ_OWNER_PERMS sec_acl_perm_read | sec_acl_perm_write \
				| sec_acl_perm_delete | sec_acl_perm_test \
				| sec_acl_perm_control | sec_acl_perm_execute

#define ALL_MGMT_PERMS mgmt_perm_inq_if | mgmt_perm_inq_pname \
				| mgmt_perm_inq_stats | mgmt_perm_ping \
				| mgmt_perm_kill | sec_acl_perm_test \
				| sec_acl_perm_control




/* These are the two entry point vectors that are explicitly initialized:   */
extern rdaclif_v1_0_epv_t dce_acl_v1_0_epv;

extern sample_bind_v1_0_epv_t sample_bind_epv;

/* This global boolean records whether the backing store databases have     */
/*  actually been opened or not. This allows us to avoid calling the        */
/*  server_acl_mgr_close() routine when there are no open databases; call-  */
/*  the dce_db_close() routine on an unopened database will result in a     */
/*  core dump.                                                              */
boolean32 databases_open;

