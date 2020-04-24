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
 * $Log: dced_server.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:46  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:30  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/23  00:22 UTC  tatsu_s
 * 	Added dced_c_binding_syntax_local.
 * 	[1995/12/08  17:59:02  root]
 * 
 * Revision 1.1.2.9  1994/08/24  21:02:01  pwang
 * 	Finished adding entryname to server obj [OT#11396]
 * 	[1994/08/24  21:00:09  pwang]
 * 
 * Revision 1.1.2.8  1994/08/23  19:23:59  rsalz
 * 	Add entryname field to server_t (OT CR 11396).
 * 	[1994/08/23  19:18:27  rsalz]
 * 
 * Revision 1.1.2.7  1994/06/09  16:02:52  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:28  devsrc]
 * 
 * Revision 1.1.2.6  1994/05/31  21:08:48  pwang
 * 	Used dced_g_uuid_all_servers for all dce services/servers in
 * 	dced_server_start/stop().
 * 	[1994/05/31  20:53:32  pwang]
 * 
 * Revision 1.1.2.5  1994/04/15  22:29:04  pwang
 * 	General cleanup and fixed a number of problems.
 * 	[1994/04/15  22:23:39  pwang]
 * 
 * Revision 1.1.2.4  1994/04/11  18:45:46  rsalz
 * 	Update to final RFC revision.
 * 	[1994/04/11  18:26:15  rsalz]
 * 
 * Revision 1.1.2.3  1994/03/11  00:07:24  pwang
 * 	Attribute related changes and cleanup.
 * 	[1994/03/10  23:56:58  pwang]
 * 
 * Revision 1.1.2.2  1994/03/09  20:57:08  pwang
 * 	Modified to match with attribute changes
 * 	[1994/03/09  20:54:32  pwang]
 * 
 * Revision 1.1.2.1  1994/02/28  16:27:04  pwang
 * 	Initial Implementation.
 * 	[1994/02/28  16:10:24  pwang]
 * 
 * $EndLog$
 */

/*
** Module: dced_server.c
**
** FACILITY: DCE Host Daemon API
**
** ABSTRACT: This module implements the dced srvrconf and srvrexec
**           functions
**
** SCOPE: Public 
**
** Date:
*/


#include <dcedimpl.h>

#define NULL_FIXEDATTR	(server_fixedattr_t *) 0

/*
**++
**
** dced_server_create creates a server configuration object
**
**--
*/
void 
dced_server_create(
	/* [in]     */	dced_binding_handle_t	h,
	/* [in,out] */	server_t		*object,
	/* [out]    */	error_status_t		*st
)
{
	
	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for srvrconf,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_srvrconf, st);

	/*
	** If the caller doesn't supply a uuid for the
	** new object, call uuid_create to create one.
	*/
	if (uuid_is_nil(&object->id, st)) {
		uuid_create(&object->id, st);
		CHECK_STATUS_RET(st);
	}

	/*
	** Create the srvrconf object
	*/
	Rsrvrconf_create(h, object, st);
	CHECK_STATUS_RET(st);

	/*
	** Invalidate the local cache
	*/
	SET_INVALID_CACHE(h);

	return;
}

/*
**++
**
** dced_server_delete deletes server configuration object
**
**--
*/
void 
dced_server_delete(
	/* [in]  */	dced_binding_handle_t	h,
	/* [in]  */	uuid_t			*id,
	/* [out] */	error_status_t		*st
)
{
	
	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for srvrconf,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_srvrconf, st);

	Rsrvrconf_delete(h, id, st);
	CHECK_STATUS_RET(st);

	/*
	** delete this server entry from local cache
	*/
	dced__cache_delete(h, id, st);
	if (!GOOD_STATUS(st)) {
		SET_INVALID_CACHE(h); 
		RESET_STATUS(st);
	}
	return;
}

/*
**  dced_server_start starts a specified server or DCE services on a
**  specified host.
**  If id == dced_g_uuid_all_servers 
**      then all DCE services will be attempted to start
**           and the attributes is ignored. (Not support yet)
**      else the server identified by id will be attempted to start.
**  The proc_id, if is not the nil UUID, will identify the running 
**  server.  If  is nil UUID, then an instance UUID will be created and 
**  returned.
*/
void 
dced_server_start(
	/* [in]      */	dced_binding_handle_t	h,
	/* [in]      */	uuid_t			*id,
	/* [in]      */	dced_attr_list_t	*attributes,
	/* [in, out] */ uuid_t                  *proc_id,
	/* [out]     */	error_status_t		*st
)
{

	boolean32	all_services = FALSE;	

	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for srvrconf,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_srvrconf, st);

	all_services = uuid_equal(id, &dced_g_uuid_all_servers, st);
	CHECK_STATUS_RET(st);

	if (all_services) {
		/*
		** If id is dced_g_uuid_all_servers, all of dce service
		** will be attempted to start. This feature is not
		** supported yet.
		*/
		SET_STATUS(st, dced_s_no_support);
	} else {
		/*
		** Start a registered server
		** The fixedattr is passed as null now. It may
		** be changed after server is implemented.
		*/
		Rsrvrconf_start(h, id, NULL_FIXEDATTR, attributes, proc_id, st);
	}

	return;
}	

/*
**++
**
** dced_server_stop stops a specified server or DCE services on a
** specified host.
** If id == dced_g_uuid_all_servers 
**     then all DCE services will be attempted to stop
**     else the specified server will be attempted to stop.
**
**--
*/
void 
dced_server_stop(
	/* [in]  */	dced_binding_handle_t	h,
	/* [in]  */	uuid_t			*id,
	/* [in] */	srvrexec_stop_method_t	flag,
	/* [out] */     error_status_t		*st
)
{

	boolean32	all_services = FALSE;
	
	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for srvrexec,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_srvrexec, st);

	/*
	** If id == dced_g_uuid_all_servers then all of dce service 
	** will be attempted to stop. Otherwise the specified
	** server will be stoped 
	*/
	all_services = uuid_equal(id, &dced_g_uuid_all_servers, st);
	CHECK_STATUS_RET(st);

	if (all_services) {
		/*
		** Shutdown all dce service
		*/
		Rsrvrexec_shutdown(h, st);
		CHECK_STATUS_RET(st);

		/*
		** Invalidate local cache
		*/
		SET_INVALID_CACHE(h);
	} else {
		/*
		** Stop a dce server
		*/
		Rsrvrexec_stop(h, id, flag, st);
		CHECK_STATUS_RET(st);

		/*
		** delete this server entry from local cache
		*/
		dced__cache_delete(h, id, st);
		if(!GOOD_STATUS(st)) {
			SET_INVALID_CACHE(h);
			RESET_STATUS(st);
		}
	}

	return;
}	

/*
**++
**
** dced_server_enable_if enables a service provided 
** by a server "id"  on a specified host. The service 
** is referred to by interface.
**
**--
*/
void
dced_server_enable_if(
	/* [in]  */	dced_binding_handle_t	h,
	/* [in]  */	uuid_t			*id,
	/* [in]  */	rpc_if_id_t		*interface,
	/* [out] */	error_status_t		*st
)
{
	
	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for srvrexec,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_srvrexec, st);

	/*
	** Call srvrexec enable service to enable the interface.
	*/
	Rsrvrexec_enable_service(h, id, interface, st);
	return;
}

/*
**++
**
** dced_server_disable_if disables a service provided 
** by a server "id"  on a specified host. The service 
** is referred to by interface.
**
**--
*/
void 
dced_server_disable_if(
	/* [in]  */	dced_binding_handle_t	h,
	/* [in]  */	uuid_t			*id,
	/* [in]  */	rpc_if_id_t		*interface,
	/* [out] */	error_status_t		*st
)
{
	
	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for srvrexec,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_srvrexec, st);

	/*
	** Call srvrexec disable service to disable the interface.
	*/
	Rsrvrexec_disable_service(h, id, interface, st);
	return;
}

/*
**++
**
** dced_server_modify_attributes modify the attribute values
** of a server object on a specified host.
**
**--
*/
void 
dced_server_modify_attributes(
	/* [in]  */	dced_binding_handle_t	h,
	/* [in]  */	uuid_t			*id,
	/* [in]  */	dced_attr_list_t	*attributes,
	/* [out] */	error_status_t		*st
)
{
	
	CLEAR_STATUS(st);

	/*
	** If the dced binding handle is not for srvrconf,
	** quit now with bad binding status.
	*/
	CHECK_BINDING_SET_RET(h, dced_e_service_type_srvrconf, st);

	/*
	** Replace  server attributes
	*/
	Rsrvrconf_attr_replace(h, id, attributes, st);
	return;
}	

