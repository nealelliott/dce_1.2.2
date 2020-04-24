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
 * $Log: dced.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:25:56  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  22:36 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/23  00:21 UTC  tatsu_s
 * 	Added dced_register_local_mgrs() and dced_c_binding_syntax_local.
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/04  17:25 UTC  mdf
 * 	Merge for CHFts13120
 * 	[1995/12/08  17:58:43  root]
 * 
 * Revision 1.1.2.19  1994/10/03  19:19:12  rsalz
 * 	Register endpoints (OT CR 12061).
 * 	[1994/10/03  19:17:39  rsalz]
 * 
 * Revision 1.1.2.18  1994/09/06  19:56:21  rsalz
 * 	Add srvrexec_exited; finish OT CR 11791.
 * 	[1994/09/06  19:34:06  rsalz]
 * 
 * Revision 1.1.2.17  1994/08/28  17:56:15  rsalz
 * 	Add missing server convenience functions (OT CR 11791).
 * 	[1994/08/28  17:55:24  rsalz]
 * 
 * Revision 1.1.2.16  1994/08/25  16:32:01  rsalz
 * 	Add dce_server_register & co.
 * 	[1994/08/25  16:29:10  rsalz]
 * 
 * Revision 1.1.2.15  1994/08/24  16:32:00  baum
 * 	Added secval_status API
 * 	[1994/08/24  16:31:36  baum]
 * 
 * Revision 1.1.2.14  1994/08/22  19:32:54  bowe
 * 	Change signature of dce_server_sec_xxx() [CR 11799]
 * 	[1994/08/22  19:32:25  bowe]
 * 
 * Revision 1.1.2.13  1994/08/20  18:05:04  bowe
 * 	Added flags for dce_server_sec_begin(). Added/fixed prototypes
 * 	for convenience functions.  [CR 11791]
 * 	[1994/08/20  18:00:48  bowe]
 * 
 * Revision 1.1.2.12  1994/08/03  22:06:25  pwang
 * 	Added local keytab support [OT#11280]
 * 	[1994/08/03  21:57:36  pwang]
 * 
 * Revision 1.1.2.11  1994/06/09  16:02:46  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:21  devsrc]
 * 
 * Revision 1.1.2.10  1994/05/31  21:08:36  pwang
 * 	Added dced_g_uuid_all_servers for dced_server_start/stop().
 * 	Made cursor be poiinter in *_release_cursor().
 * 	[1994/05/31  20:47:13  pwang]
 * 
 * Revision 1.1.2.9  1994/05/13  19:21:07  rsalz
 * 	Remove context and km_thread params from dce_server_sec routines.
 * 	[1994/05/13  19:13:25  rsalz]
 * 
 * Revision 1.1.2.8  1994/05/12  19:30:20  rsalz
 * 	Add some dce_server functions.
 * 	Add and use dced_authorized
 * 	More srvr implementation; export their interfaces
 * 	[1994/05/12  19:26:19  rsalz]
 * 
 * Revision 1.1.2.7  1994/05/05  14:41:09  pwang
 * 	Moved utility routines and encrpt/decript to dced_util.h.
 * 	Changed dced_binding_from_rpc_binding interface.
 * 	Rename _cursor_ routines and _rkeytab_.
 * 	[1994/05/05  14:07:08  pwang]
 * 
 * Revision 1.1.2.6  1994/04/18  19:45:44  pwang
 * 	Added attr_id to dced_hostdata_read() (RPC47.3)
 * 	[1994/04/18  19:30:25  pwang]
 * 
 * Revision 1.1.2.5  1994/04/15  22:28:58  pwang
 * 	General cleanup and fixed a number of problems.
 * 	[1994/04/15  21:29:18  pwang]
 * 
 * Revision 1.1.2.4  1994/03/11  00:07:22  pwang
 * 	Added the proc_id(uuid_t*) for dced_server_start()
 * 	[1994/03/10  23:52:34  pwang]
 * 
 * Revision 1.1.2.3  1994/03/09  20:45:33  pwang
 * 	Moved dced services header files to dcedimpl.h, they are not
 * 	exported anymore.
 * 	[1994/03/09  20:45:10  pwang]
 * 
 * Revision 1.1.2.2  1994/03/04  16:55:18  bowe
 * 	Moved symbols for dced service names from here to dced_data.h
 * 	[1994/03/04  16:54:58  bowe]
 * 
 * Revision 1.1.2.1  1994/02/28  16:26:55  pwang
 * 	Initial Implementation.
 * 	[1994/02/28  16:05:59  pwang]
 * 
 * $EndLog$
 */

#ifndef _DCED_H
#define	_DCED_H

#include <dce/dced_base.h>
#include <dce/dcedhdmsg.h>
#include <dce/dced_data.h>

/*
**--------------- DCED API DATA DECLARATIONS -------------------
*/

/*
** dced_binding_handle_t defines a binding handle to
** contact with dced services in a host.
*/
typedef struct dced_binding_handle_s_t	*dced_binding_handle_t;

/*
** dced_cursor_t points to a dced entry cursor structure
*/
typedef struct dced_cursor_s_t	*dced_cursor_t;

/*
** dced_keytab_cursor_t points to a dced key table cursor 
** structure
*/
typedef struct dced_keytab_cursor_s_t	*dced_keytab_cursor_t;

/*
** dced_service_type_t distinguishes the services 
** provided by decd 
*/
typedef enum dced_service_type_e_t {
	dced_e_service_type_null,
	dced_e_service_type_ep,
	dced_e_service_type_srvrconf,
	dced_e_service_type_srvrexec,
	dced_e_service_type_hostdata,
	dced_e_service_type_keytab,
	dced_e_service_type_secval
} dced_service_type_t;

/*
**---------------- DCED APPLICATION PROGRAMMING INTERFACE -------------
*/

/*
** Common api
*/

/*
** dced_bind_create gets a binding for a dced service in a 
** specified host
*/
extern void dced_binding_create(
	/* [in]  */	dced_string_t		/* name */,
	/* [in]	 */	unsigned32		/* binding_syntax */,
	/* [out] */	dced_binding_handle_t*	/* h */,
	/* [out] */	error_status_t*		/* st */
);
#define	dced_c_binding_syntax_default	0x0000
#define	dced_c_binding_syntax_bypass	0x0001
#define	dced_c_binding_syntax_local	0x0002

/*
** dced_register_local_mgrs registers local, i.e., within the same
** address space, manager epvs for dced_c_binding_syntax_local
*/
extern void dced_register_local_mgrs(
	/* [in]  */	rpc_mgr_epv_t		/* hostdata epv */,
	/* [in]  */	rpc_mgr_epv_t		/* rkeytab epv */,
	/* [in]  */	rpc_mgr_epv_t		/* srvrconf epv */,
	/* [in]  */	rpc_mgr_epv_t		/* srvrexec epv */,
	/* [out] */	error_status_t*		/* st */
);

/*
**  dced_host_binding_from_rpc_binding converts a rpc binding for
**  a dced service to a dced host binding handle.
*/
extern void dced_binding_from_rpc_binding(
	/* [in]  */	dced_service_type_t	/* service */,
	/* [in]  */	rpc_binding_handle_t	/* rpc_handle */,
        /* [out] */     dced_binding_handle_t*	/* h */,
        /* [out] */     error_status_t*		/* st */
);

/*
** dced_binding_set_auth_info sets authentication, authorization, and
** protection level information for the dced server binding handle
*/
extern void dced_binding_set_auth_info(
	/* [in]  */     dced_binding_handle_t		/* h */,
	/* [in]  */     unsigned32			/* protect_level */,
	/* [in]  */     unsigned32			/* authn_svc */,
	/* [in]  */     rpc_auth_identity_handle_t	/* authn_identity */,
	/* [in]  */     unsigned32			/* authz_svc */,
	/* [out] */     error_status_t*			/* st */
);

/*
** dced_binding_free releases a binding for a dced service
*/
extern void dced_binding_free(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [out] */	error_status_t*		/* st */
);

/*
** dced_inq_id inquires one's uuid by its name from a specified host.
*/
extern void dced_inq_id(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	dced_string_t		/* name */,
	/* [out] */     uuid_t*			/* id */,
	/* [out] */     error_status_t*		/* st */
);

/*
**  dced_inq_name inquires one's name by its uuid from a specified host.
*/
extern void dced_inq_name(
	/* [in]  */ 	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */, 
	/* [out] */	dced_string_t*		/* name */,
	/* [out] */	error_status_t*		/* st */
);

/*
**  dced_get_list retruns a list of entries maintained  by 
**  the dced service in a specified host.
*/
extern void dced_list_get(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [out] */	dced_entry_list_t*	/* list */,
	/* [out] */	error_status_t*		/* st */
);
	
/*
**  dced_list_release releases the list return by dced_get_list,
*/
extern void dced_list_release(
	/* [in]     */	dced_binding_handle_t	/* h */,
	/* [in,out] */	dced_entry_list_t*	/* list */,
	/* [out]    */	error_status_t*		/* st */
);

/*
**  dced_initialize_cursor sets the cursor to the starting position
**  of the cached list. This cursor will be used to traverse this
**  list.
*/
extern void dced_initialize_cursor(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [out] */	dced_cursor_t*		/* cursor */,
	/* [out] */	error_status_t*		/* st */
);

/*
**  dced_entry_get_next returns the entry pointed by cursor and advances
**  cursor.
*/
extern void dced_entry_get_next(
	/* [in,out] */	dced_cursor_t		/* cursor */,
	/* [out]    */	dced_entry_t**		/* entry */,
	/* [out]    */	error_status_t*		/* st */
);

/*
**  dced_cursor_release releases the cursor.
*/
extern void dced_release_cursor(
	/* [in,out] */	dced_cursor_t*		/* cursor */,
	/* [out]    */  error_status_t*		/* st */
);

/*
**  dced_entry_add adds a entry to a dced service  in a specified host.
*/
extern void dced_entry_add(
	/* [in]     */	dced_binding_handle_t	/* h */,
	/* [in,out] */	dced_entry_t*		/* entry */,
	/* [out]    */	error_status_t*		/* st */
);
	
/*
**  dced_entry_remove removes a entry from a  dced service in a
**  specified host.
*/
extern void dced_entry_remove(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */,
	/* [out] */	error_status_t*		/* st */
);


/*
** dced_object_read_all reads all object data of a dced service from a
** specified host.
*/
extern void dced_object_read_all(
	/* [in]  */	dced_binding_handle_t   /* h */,
	/* [out] */	unsigned32*		/* count */,
	/* [out] */	void**			/* list */,
	/* [out] */	error_status_t*		/* st */
);


/*
** dced_object_read reads object data of a dced service from a
** specified host.
*/
extern void dced_object_read(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */,
	/* [out] */	void**			/* data */,
	/* [out] */	error_status_t*		/* st */
);

/*
** dced_objects_releases releases object data of a dced service
*/
extern void dced_objects_release(
	/* [in]     */	dced_binding_handle_t	/* h */,
	/* [in]     */	unsigned32		/* count */,
	/* [in,out] */	void*			/* objects */,
	/* [out]    */	error_status_t*		/* st */
);

/*
** server 
*/

/*
** dced_server_create creates a server configuration object
*/
extern void dced_server_create(
	/* [in]     */	dced_binding_handle_t	/* h */,
	/* [in,out] */	server_t*		/* object */,
	/* [out]    */	 error_status_t*	/* st */
);

/*
** dced_server_delete deletes server configuration object
*/
extern void dced_server_delete(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */,
	/* [out] */	 error_status_t*	/* st */
);

/*
**  dced_server_start starts a specified server or DCE services on a
**  specified host.
**  If id == dced_g_uuid_all_servers 
**      then all DCE services will be attempted to start
**           and the attributes is ignored. (Not support yet)
**      else the server identified by id will be attempted to start.
**  The proc_id, if is not nil UUID, will identify the running
**  server.  If is nil UUID, an instance UUID will be created and 
**  returned.
*/
extern void dced_server_start(
	/* [in]      */	dced_binding_handle_t	/* h */,
	/* [in]      */	uuid_t*			/* id */,
	/* [in]      */	dced_attr_list_t*	/* attributes */,
	/* [in, out] */ uuid_t*			/* proc_id */,
	/* [out]     */	error_status_t*		/* st */
);

/*
**  dced_server_stop  stops a specified server or DCE services in a
**  specified host.
**  If id == dced_g_uuid_all_servers 
**      then all DCE services will be attempted to stop
**      else the $srvr_name will be attempted to stop.
*/
extern void dced_server_stop(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */,
	/* [in]  */	srvrexec_stop_method_t	/* flag */,
	/* [out] */     error_status_t*		/* st */
);

/*
** dced_server_enable_if enables a service provided 
** by a server "id"  in a specified host. The service 
** is referred to by interface.
*/
extern void dced_server_enable_if(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */,
	/* [in]  */	rpc_if_id_t*		/* interface */,
	/* [out] */	error_status_t*		/* st */
);

/*
** dced_server_disable_if disables a service provided 
** by a server "id"  in a specified host. The service 
** is referred to by interface.
*/
extern void dced_server_disable_if(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */,
	/* [in]  */	rpc_if_id_t*		/* interface */,
	/* [out] */	error_status_t*		/* st */
);

/*
** dced_srvrconf_modify_attributes modify the attribute values
** or service in the server object in a specified host.
*/
extern void dced_server_modify_attributes(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */,
	/* [in]  */	dced_attr_list_t*	/* attributes */,
	/* [out] */	error_status_t*		/* st */
);	

/*
** hostdata
*/

/*
** dced_hostdata_create creates a configuration entry and data 
** to be maintained by the dced  service in a specified 
** host.
** The difference between the this api and dced_entry_add
** is that the former creates a entry and data but the latter 
** only adds a entry.
*/
extern void dced_hostdata_create(
	/* [in]      */	dced_binding_handle_t	/* h */,
	/* [in,out]  */	dced_entry_t*		/* entry */,
	/* [in]      */	dced_attr_list_t*	/* data */,
	/* [out]     */	error_status_t*		/* st */
);
	
/*
** dced_hostdata_delete deletes the configuration 
** entry and data maintained by the dced service in a 
** specified host.
** The difference between the this api and dced_entry_remove
** is that the former deletes a entry and its hostdata but 
** the latter only remove a entry.
*/
extern void dced_hostdata_delete(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */,
	/* [out] */	error_status_t*		/* st */
);
	
	
/*
** dced_hostdata_read reads data associated with
** the id in a specified host. 
*/
extern void dced_hostdata_read(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*	 		/* id */,
	/* [in]  */	uuid_t*	 		/* attr_id */,
	/* [out] */	sec_attr_t**		/* data */,
	/* [out] */	error_status_t*		/* st */
);
	
/*
** dced_hostdata_write replaces any existing data associated 
** with id in a specified host. 
** But if the entry referred to by id is not maintained 
** by the dced service, this operation will be  aborted. 
*/
extern void dced_hostdata_write(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */,
	/* [in]  */	dced_attr_list_t*	/* data */,
	/* [out] */	error_status_t*		/* st */
);

/*
** rkeytab
*/

/*
** dced_keytab_create creates a keytab entry and a list
** of keys  to be maintained by the dced  service in a 
** specified host.
** The difference between the this api and dced_entry_add
** is that the former creates a entry and keys but the latter 
** only adds a entry.
*/
extern void dced_keytab_create(
	/* [in]      */	dced_binding_handle_t	/* h */,
	/* [in,out]  */	dced_entry_t*		/* entry */,
	/* [in,out]  */	dced_key_list_t*	/* keys */,
	/* [out]     */	error_status_t*		/* st */
);
	
/*
** dced_keytab_delete deletes a keytab entry and 
** its keys maintained by the dced service in a specified host.
** The difference between the this api and dced_entry_remove
** is that the former deletes a entry and its keytab but 
** the latter only removes a entry.
*/
extern void dced_keytab_delete(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */,
	/* [out] */	error_status_t*		/* st */
);

/*
** dced_keytab_initialize_cursor generates a keytable cursor to
** traverse the requested keytable for particular principal's key(s)
*/
extern void dced_keytab_initialize_cursor(
	/* [in]  */	dced_binding_handle_t	/* h */,
	/* [in]  */	uuid_t*			/* id */,
	/* [out] */	dced_keytab_cursor_t*	/* cursor */,
	/* [out] */	error_status_t*		/* st */
);

/*
** dced_keytab_get_next_key returns the key under the cursor
*/
extern void dced_keytab_get_next_key(
	/* [in,out] */	dced_keytab_cursor_t	/* cursor */,
	/* [out]    */	dced_key_t**		/* key */,
	/* [out]    */	error_status_t*		/* st */
);

/*
** dced_keytab_release_cursor relases a keytable cursor
*/
extern void dced_keytab_release_cursor(
	/* [in,out] */	dced_keytab_cursor_t*	/* cursor */,
	/* [out]    */	error_status_t*		/* st */
);

/*
** dced_keytab_add_key adds a  key to the keytab in a specified host. 
*/
extern void dced_keytab_add_key(
	/* [in]     */	dced_binding_handle_t	/* h */,
	/* [in]     */	uuid_t*			/* id */,
	/* [in,out] */	dced_key_t*		/* key */,
	/* [out]    */	error_status_t*		/* st */
);

/*
** dced_keytab_remove_key deletes a  key from the keytab 
** in a specified host.  
*/
extern void dced_keytab_remove_key(
	/* [in]     */	dced_binding_handle_t	/* h */,
	/* [in]     */	uuid_t*			/* id */,
	/* [in,out] */	dced_key_t*		/* key */,
	/* [out]    */	error_status_t*		/* st */
);

/*
** dced_keytab_change_key updates a key in both registry and
** keytable in a specified host.
*/
extern void dced_keytab_change_key(
	/* [in]     */	dced_binding_handle_t	/* h */,
	/* [in]     */	uuid_t*			/* id */,
	/* [in,out] */	dced_key_t*		/* key */,
	/* [out]    */	error_status_t*		/* st */
);

/*
** secval
*/

/*
** dced_secval_start starts secval service in a 
** specified host. 
*/
extern void dced_secval_start(
	/* [in]  */ 	dced_binding_handle_t	/* h */,
	/* [out] */	error_status_t*		/* st */
);

/*
** dced_secval_stop stops secval service in a  
** specified host. 
*/
extern void dced_secval_stop(
	/* [in]  */ 	dced_binding_handle_t	/* h */,
	/* [out] */	error_status_t*		/* st */
);

/*
** dced_secval_validate validates the principal's credential 
** via a DCE security server at the specified host
*/
extern void dced_secval_validate(
	/* [in]  */ 	dced_binding_handle_t	/* h */,
	/* [out] */	error_status_t*		/* st */
);

/*
** dced_secval_status returns whether secval has been activated
** or not.
*/
extern void dced_secval_status(
	/* [in]  */ 	dced_binding_handle_t	/* h */,
	/* [out] */	boolean32 *		/* is_active */,
	/* [out] */	error_status_t*		/* st */
);

/*
** dced_secval_start starts secval service in a 
** specified host. 
*/
extern void dced_secval_update(
	/* [in]  */ 	dced_binding_handle_t	/* h */,
	/* [out] */	error_status_t*		/* st */
);


/*
** local server/dced
*/

/*
**  dce_server_inq_attr gets a server's attribute
*/
extern void dce_server_inq_attr(
	/* [in]  */	uuid_t*			/* attribute_uuid */, 
	/* [in]  */	sec_attr_t*		/* value */,
	/* [out] */	error_status_t*		/* st */
);

/*
**  dce_server_inq_uuids returns the srvrconf and srvrexec uuids of
**  a server that has been started by dced
*/
extern void dce_server_inq_uuids(
	/* [out] */	uuid_t*			/* conf_entry_uuid */, 
	/* [out] */	uuid_t*			/* exec_entry_uuid */, 
	/* [out] */	error_status_t*		/* st */
);

/*
**  dce_server_register registers a server with the RPC runtime and the
**  local dced
*/

typedef struct dce_server_type_s_t {
    uuid_t		type;
    rpc_mgr_epv_t	epv;
} dce_server_type_t;

typedef struct dce_server_register_data_s_t {
    rpc_if_handle_t	ifhandle;
    rpc_mgr_epv_t	epv;
    unsigned32		num_types;
    dce_server_type_t	*types;
} dce_server_register_data_t;

typedef struct dce_server_handle_s_t *dce_server_handle_t;

extern void dce_server_register(
	/* [in]  */	unsigned32		/* flags */, 
	/* [in]  */	server_t*		/* server */, 
	/* [in]  */	dce_server_register_data_t* /* data */,
	/* [out] */     dce_server_handle_t*	/* h */,
	/* [out] */	error_status_t*		/* st */
);
#define dce_server_c_ns_export		0x0001
#define dce_server_c_no_protseqs	0x0002
#define dce_server_c_no_endpoints	0x0004
#define dce_service_c_no_ns_export	service_c__not_exported

extern void dce_server_use_protseq(
	/* [in]  */     dce_server_handle_t	/* h */,
	/* [in]  */	unsigned char*		/* protseq */,
	/* [out] */	error_status_t*		/* st */
);

extern void dce_server_unregister(
	/* [in]  */     dce_server_handle_t*	/* h */,
	/* [out] */	error_status_t*		/* st */
);

/*
**  dce_server_disable_service disables an interface in the local server
*/
extern void dce_server_disable_service(
	/* [in]  */     dce_server_handle_t	/* h */,
	/* [in]  */	rpc_if_handle_t		/* ifspec */,
	/* [out] */	error_status_t*		/* st */
);

/*
**  dce_server_enable_service enables an interface in the local server
*/
extern void dce_server_enable_service(
	/* [in]  */     dce_server_handle_t	/* h */,
	/* [in]  */	rpc_if_handle_t		/* ifspec */,
	/* [out] */	error_status_t*		/* st */
);


/*
**  dce_server_sec_begin sets up a login context and key management for
**  a server.
*/
extern void dce_server_sec_begin(
	/* [in]  */	unsigned32		/* flags */, 
	/* [out] */	error_status_t*		/* st */
);
#define dce_server_c_manage_key		0x0001
#define dce_server_c_login		0x0002

/*
**  dce_server_sec_done takes down everything set up by dce_server_sec_begin
*/
extern void dce_server_sec_done(
	/* [out] */	error_status_t*		/* st */
);

/*
**  dce_server_inq_server retrieves the server_t structure
*/
extern void dce_server_inq_server(
	/* [out] */	server_t **		/* server */,
	/* [out] */	error_status_t *	/* st */
);

#endif
