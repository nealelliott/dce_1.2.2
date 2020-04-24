/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: globals.c,v $
BINRevision 1.1.2.1  1996/12/06  17:50:10  damon
BIN	Tandem dced fixes
BIN
 * Revision /main/gaz_dce1.2_tandem/1  1996/08/16  17:16 UTC  gaz
 * 	init the new field in the dced_g_info struct which contains pipe file descriptors
 * 
 * Revision 1.1.2.2  1996/02/18  19:16:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:06  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:16  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:39 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/2  1995/09/23  17:49 UTC  tatsu_s
 * 	Added anonymous hostdata directory.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/19  15:23 UTC  tatsu_s
 * 	Initial revision.
 * 	[1995/12/08  20:48:54  root]
 * 
 * $EndLog$
 */

#include <libimpl.h>
#include <sximpl.h>

/* ______________________________________________________________________ */

boolean32		dced_is_remote_disabled = TRUE;
boolean32		dced_is_server = FALSE;
volatile boolean32	dced_is_acl_enabled = FALSE;

dced_string_list_t	environ_string_list = {0};

char			*dced_g_anonymous_path = NULL;

/* Global definitions. */

/* DCE_SVC_DEFINE_HANDLE(dhd_svc_handle, dhd_svc_table, "dhd")
   svc__dhd_svc_handle needs to be visible. */
struct dce_svc_handle_s_t svc__dhd_svc_handle = {
    "dhd", dhd_svc_table };
dce_svc_handle_t dhd_svc_handle = &svc__dhd_svc_handle; 

volatile boolean32	dced_is_bootstrap = FALSE;

srvr_table_t		sc_table = {0};
srvr_table_t		sx_table = {0};
srvr_hash_table_t	sc_hash_table = {0};
srvr_hash_table_t	sx_hash_table = {0};
sx_pid_table_t		sx_pids = {0};

dced_g_info_t		dced_g_info = {
    { 0 }, { 0 }, NULL, NULL, NULL, NULL, NULL, { 0, 0 } };

name_cache_t	hd_name_cache = NULL;
name_cache_t	km_name_cache = NULL;
name_cache_t	as_name_cache = NULL;
name_cache_t	sc_name_cache = NULL;
name_cache_t	sx_name_cache = NULL;

/* database handles */
dce_db_handle_t	acl_db = NULL;
dce_db_handle_t	hd_db = NULL;
dce_db_handle_t	km_db = NULL;
dce_db_handle_t	sc_db = NULL;
dce_db_handle_t	sx_db = NULL;
dce_db_handle_t	as_db = NULL;

/* for initializing data */

sec_attr_t	dced_g_null_sec_attr = {0};
server_t	dced_g_null_server = {0};
