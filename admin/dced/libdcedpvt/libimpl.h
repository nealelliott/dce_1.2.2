/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: libimpl.h,v $
 * Revision 1.1.2.2  1996/02/18  23:32:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:26  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:29  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:39 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/3  1995/09/23  17:49 UTC  tatsu_s
 * 	Added anonymous hostdata directory.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/2  1995/09/23  00:43 UTC  tatsu_s
 * 	Added dcedpvt_server.h.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/19  15:23 UTC  tatsu_s
 * 	Initial revision.
 * 	[1995/12/08  20:49:00  root]
 * 
 * $EndLog$
 */

#ifndef	_DCED_LIBIMPL_H
#define	_DCED_LIBIMPL_H

/* System header files. */
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>

/* DCE facilities we use. */
#include <dce/dce.h>
#include <dce/dce_utils.h>
#define DCE_ASSERT
#include <dce/assert.h>
#include <dce/dce_msg.h>
#include <dce/dbif.h>
#include <dce/dce_cf.h>
/* #include <dce/dceacl.h> */
#include <dce/aclif.h>

/* DCE facilities we provide. */
#include <dce/dced_data.h>
#include <hostdata.h>
#include <rkeytab.h>
#include <srvrconf.h>
#include <srvrexec.h>
#include <rs_attr_schema.h>

/* ACL definitions. */
#include <acldefs.h>
#include <dce/dced_aclbits.h>

/* Implementation details. */
#include <dced_convert.h>
#include <dce/dcedhdmsg.h>
#include <dcedhdsvc.h>
#include <dcedhdmac.h>
#include <dce_lock.h>

/* Global variables. */
#include <dcedpvt_server.h>

/* ______________________________________________________________________ */

/* for convenience */
#define CHECK_STAT_RET(st)	if ((st) != error_status_ok) return;

/* ______________________________________________________________________ */

#define DCED_DIRECTORY		"/var/dced"
#define DCED_OBJECT_FILE	"objectuuid.txt"

/* ______________________________________________________________________ */

typedef struct name_cache_s_t *name_cache_t;

/* ______________________________________________________________________ */

extern boolean32		dced_is_remote_disabled;
extern boolean32		dced_is_server;
extern volatile boolean32	dced_is_acl_enabled;
extern dced_string_list_t	environ_string_list;
extern char			*dced_g_anonymous_path;

/* name caches */
extern name_cache_t	hd_name_cache;
extern name_cache_t	km_name_cache;
extern name_cache_t	as_name_cache;
extern name_cache_t	sc_name_cache;
extern name_cache_t	sx_name_cache;

/* database handles */
extern dce_db_handle_t	acl_db;
extern dce_db_handle_t	hd_db;
extern dce_db_handle_t	km_db;
extern dce_db_handle_t	sc_db;
extern dce_db_handle_t	sx_db;
extern dce_db_handle_t	as_db;

/* for initializing data */

extern sec_attr_t	dced_g_null_sec_attr;
extern server_t		dced_g_null_server;

/* ______________________________________________________________________ */

extern	boolean32	name_cache_find_uuid(name_cache_t, char*, uuid_t*);
extern	void	name_cache_add(name_cache_t, char*, uuid_t*);
extern	void	name_cache_del(name_cache_t, char*);

/* naming functions. */
extern	void	dced_find_attr_schema_from_name(name_cache_t,
	dce_db_handle_t, char*, attr_schema_data_t*, error_status_t*);
extern	void	dced_find_entry_from_name(name_cache_t,
	dce_db_handle_t, char*, dced_entry_data_t*, error_status_t*);
extern	void	dced_find_server_from_name(name_cache_t,
	dce_db_handle_t, char*, server_data_t*, error_status_t*);
extern	void	dced_find_attr_schema_from_name(name_cache_t,
	dce_db_handle_t, char*, attr_schema_data_t*, error_status_t*);
extern	void	dced_resolve_container(handle_t, sec_acl_component_name_t,
	sec_acl_type_t, uuid_t*, boolean32, void*, uuid_t*, error_status_t*);
extern	void	dced_resolve_object(handle_t, sec_acl_component_name_t,
	sec_acl_type_t, uuid_t*, boolean32, void*, uuid_t*, error_status_t*);

/* copying functions. */
extern	void	fixedattr_copy(server_fixedattr_t*, server_fixedattr_t*,
	dced_alloc_func_t, error_status_t*);
extern	void	attr_list_copy(dced_attr_list_t*, dced_attr_list_t*,
	dced_alloc_func_t, error_status_t*);
extern	void	server_copy(server_t*, server_t*,
	dced_alloc_func_t, error_status_t*);
extern	void	string_list_copy(dced_string_list_t*, dced_string_list_t*,
	dced_alloc_func_t, error_status_t*);
extern	void	string_list_append(dced_string_list_t*, dced_string_list_t*,
	dced_alloc_func_t, error_status_t*);
extern	void	tower_list_copy(dced_tower_list_t*, dced_tower_list_t*,
	dced_alloc_func_t, error_status_t*);
extern	void	srvr_qualify_prin_names(dced_string_list_t*, error_status_t*);
extern	idl_char	*strdup_for_client(idl_char*, error_status_t*);
extern	void	string_copy(dced_string_t*, dced_string_t*, dced_alloc_func_t, error_status_t *);

/* security functions */
extern	boolean32	dced_privacy_ok(handle_t, error_status_t *);

#endif	/* _DCED_LIBIMPL_H */
