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
 * $Log: dced_util.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:10  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:59:07  root]
 * 
 * Revision 1.1.2.9  1994/08/24  19:26:54  bowe
 * 	Expunge key encryption and transmit_rep code. [CR 11773]
 * 	[1994/08/24  19:10:21  bowe]
 * 
 * Revision 1.1.2.8  1994/08/20  18:05:06  bowe
 * 	Add #define for dced_keytab_env_name. [CR 11791]
 * 	[1994/08/20  18:02:23  bowe]
 * 
 * Revision 1.1.2.7  1994/08/03  22:06:30  pwang
 * 	Freed passwd before make it null in DCED_GEN_RANDOM_KEY.
 * 	[1994/08/03  22:01:28  pwang]
 * 
 * Revision 1.1.2.6  1994/07/06  20:17:10  pwang
 * 	Reorg dced free routines [OT#11003]
 * 	[1994/07/06  20:07:35  pwang]
 * 
 * Revision 1.1.2.5  1994/06/09  16:02:54  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:30  devsrc]
 * 
 * Revision 1.1.2.4  1994/05/31  21:08:50  pwang
 * 	Added #ifndef _DCED_UTIL_H, etc.
 * 	[1994/05/31  20:54:22  pwang]
 * 
 * Revision 1.1.2.3  1994/05/18  20:58:16  bowe
 * 	Added macros for random key specification
 * 	[1994/05/18  20:54:29  bowe]
 * 
 * Revision 1.1.2.2  1994/05/12  19:30:21  rsalz
 * 	Add some dce_server functions.
 * 	Add and use dced_authorized
 * 	More srvr implementation; export their interfaces
 * 	[1994/05/12  19:26:20  rsalz]
 * 
 * Revision 1.1.2.1  1994/05/05  14:41:07  pwang
 * 	Initial Implementation.
 * 	[1994/05/05  14:34:36  pwang]
 * 
 * $EndLog$
 */

#ifndef _DCED_UTIL_H
#define	_DCED_UTIL_H

/* env variables for passing from dced to servers started by it. */
#define dced_uuid_env_name	"DCED_UUIDS"
#define dced_conf_env_name	"DCED_CONF_FILE"
#define dced_keytab_env_name	"DCED_KEYTAB_FILE"

/* 
** For specifying that server generate a random key in keytab_add_key 
*/
#define DCED_GEN_RANDOM_KEY(k)	\
	if (k->passwd) free(k->passwd); k->passwd = NULL;
#define DCED_IS_RANDOM_KEY(k)	(!k->passwd)


/*
** Utility routines
*/

/*
** dced_entry_free_ptrs()
**    Free every non-null pointer in the entry pointed by entry_p
**    and intialize the entry. The entry_p is not freed.
*/
extern void dced_entry_free_ptrs(
	/* [in, out] */	dced_entry_t*	/* entry_p */
);

/*
** dced_entry_free()
**    Free every non-null pointer in the entry pointed by *entry_p;
**    and free *entry_p as well. Finally *entry_p will be set to NULL.
*/
extern void dced_entry_free(
	/* [in, out] */	dced_entry_t**	/* entry_p */
);

/*
** dced_entry_list_free_ptrs()
**    Free every entry of the list pointed by entry_list_p, and
**    then free the whole list. The entry list itself pointed  by
**    entry_list_p is not freed, but initialized.
*/
extern void dced_entry_list_free_ptrs(
	/* [in, out] */	dced_entry_list_t*	/* entry_list_p */
);

/*
** dced_entry_list_free()
**    Free every entry of the list pointed by *entry_list_p, and
**    then free the whole list. Finally the *entry_list_p is freed and
**    set to NULL.
*/
extern void dced_entry_list_free(
	/* [in, out] */	dced_entry_list_t**	/* entry_list_p */
);

/*
** dced_attr_list_free_ptrs()
**    Free every sec_attr of the list pointed by attr_list_p,
**    and then free the whole list. The attr list itself pointed  by
**    attr_list_p is not freed, but initialized.
*/
extern void dced_attr_list_free_ptrs(
	/* [in, out] */	dced_attr_list_t*	/* entry_list_p */
);

/*
** dced_attr_list_free()
**    Free every sec_attr of the list pointed by *attr_list_p,
**    and then free the whole list. Finally the *attr_list_p itself is
**    freed and set to NULL.
*/
extern void dced_attr_list_free(
	/* [in, out] */	dced_attr_list_t**	/* attr_list_p */
);

/*
** dced_string_list_free_ptrs()
**    Free every string of the list, and then free the whole list.
**    The string list itself pointed  by  string_list_p is not freed,
**    but initialized.
*/
extern void dced_string_list_free_ptrs(
	/* [in, out] */	dced_string_list_t*	/* string_list_p */
);

/*
** dced_string_list_free()
**    Free every string of the list, and then free the whole list.
**    Finally the *string_list_p itself is freeed and set to NULL.
*/
extern void dced_string_list_free(
	/* [in, out] */	dced_string_list_t**	/* string_list_p */
);

/*
** dced_uuid_list_free_ptrs()
**    Free the uuid list, the uuid list itself pointed by uuid_list_p
**    is not freed,  but initialized.
*/
extern void dced_uuid_list_free_ptrs(
	/* [in, out] */	uuid_list_t*	/* uuid_list_p */
);

/*
** dced_uuid_list_free()
**    Free the uuid list, and then the *uuid_list_p is freed and set
**    to NULL.
*/
extern void dced_uuid_list_free(
	/* [in, out] */	uuid_list_t**	/* uuid_list_p */
);

/*
** dced_opnum_list_free_ptrs()
**    Free the opnum list, the opnum list itself pointed by opnum_list_p
**    is not freed,  but initialized.
*/
extern void dced_opnum_list_free_ptrs(
	/* [in, out] */	dced_opnum_list_t*	/* opnum_list_p */
);

/*
** dced_opnum_list_free()
**    Free the opnum list, and then the *opnum_list_p is freed and set
**    to NULL.
*/
extern void dced_opnum_list_free(
	/* [in, out] */	dced_opnum_list_t**	/* opnum_list_p */
);

/*
** dced_key_free_ptrs()
**    Free every non-null pointer in the key pointed by key_p
**    and intialize the key. The key_p is not freed.
*/
extern void dced_key_free_ptrs(
	/* [in, out] */	dced_key_t*	/* key_p */
);

/*
** dced_key_free()
**    Free every non-null pointer in the key pointed by *key_p;
**    and free *key_p as well. Finally *key_p will be set to NULL.
*/
extern void dced_key_free(
	/* [in, out] */	dced_key_t**	/* key_p */
);

/*
** dced_key_list_free_ptrs()
**    Free every key of the list pointed by key_list_p, and
**    then free the whole list. The key list itself pointed  by
**    key_list_p is not freed, but initialized.
*/
extern void dced_key_list_free_ptrs(
	/* [in, out] */	dced_key_list_t*	/* key_list_p */
);

/*
** dced_key_list_free()
**    Free every key of the list pointed by *key_list_p, and
**    then free the whole list. Finally the *key_list_p is free
**    and set to NULL.
*/
extern void dced_key_list_free(
	/* [in, out] */	dced_key_list_t**	/* key_list_p */
);

/*
** dced_tower_set_free()
**    Free the tower set pointed by *twr_set_p; and free *twr_set_p as well.
**    Finally *twr_set_p will be set to NULL.
** NOTE: The tower_set is assumed to be allocated by
**       rpc_tower_vector_from_binding and will be freed by
**       rpc_tower_vector_free
*/
extern void dced_tower_set_free(
	/* [in, out] */	sec_attr_twr_set_p_t*	/* twr_set_p */
);

/*
** dced_tower_list_free_ptrs()
**    Free every tower set of the list pointed by twr_set_list_p and
**    then free the whole list. The tower set list itself pointed by
**    twr_set_list_p is not freed, but initialized.
** NOTE: The each tower_set is assumed to be allocated by
**       rpc_tower_vector_from_binding and will be freed by
**       rpc_tower_vector_free
*/
extern void dced_tower_list_free_ptrs(
	/* [in, out] */	dced_tower_list_t*	/* twr_set_list_p */
);

/*
** dced_tower_list_free()
**    Free every tower set of the list pointed by *twr_set_list_p  and
**    then free the whole list. Finally the *twr_set_list_p is free
**    and set to NULL.
** NOTE: The each tower_set is assumed to be allocated by
**       rpc_tower_vector_from_binding and will be freed by
**       rpc_tower_vector_free
*/
extern void dced_tower_list_free(
	/* [in, out] */	dced_tower_list_t**	/* twr_set_list_p */
);

/*
** dced_fixedattr_free_ptrs()
**    Free every non-null pointer in the server_fixedattr_t pointed
**    by fixedattr_p and intialize the server_fixattr_t. The fixedattr_p
**    is not freed.
*/
extern void dced_fixedattr_free_ptrs(
	/* [in, out] */	server_fixedattr_t*	/* fixedattr_p */
);

/*
** dced_fixedattr_free()
**    Free every non-null pointer in the server_fixedattr pointed
**    by *fixedattr_p; and free *fixedattr_p as well. Finally
**    *fixedattr_p will be set to NULL.
*/
extern void dced_fixedattr_free(
	/* [in, out] */	server_fixedattr_t**	/* fixedattr_p */
);

/*
** dced_service_free_ptrs()
**    Free every non-null pointer in the service_t pointed
**    by service_p and intialize the service_t. The service_p
**    is not freed.
*/
extern void dced_service_free_ptrs(
	/* [in, out] */	service_t*	/* service_p */
);

/*
** dced_service_free()
**    Free every non-null pointer in the service_t pointed
**    by service_p; and free *service_p as well. Finally
**    *service_p will be set to NULL.
*/
extern void dced_service_free(
	/* [in, out] */	service_t**	/* service_p */
);

/*
** dced_service_list_free_ptrs()
**    Free every service of the list pointed by service_list_p, and
**    then free the whole list. The service list itself pointed  by
**    service_list_p is not freed, but initialized.
*/
extern void dced_service_list_free_ptrs(
	/* [in, out] */	service_list_t*	/* service_list_p */
);

/*
** dced_service_list_free()
**    Free every service of the list pointed by *service_list_p, and
**    then free the whole list. Finally the *service_list_p is freed
**    and set to NULL.
*/
extern void dced_service_list_free(
	/* [in, out] */	service_list_t**	/* service_list_p */
);

/*
** dced_server_free_ptrs()
**    Free every non-null pointer in the server_t pointed
**    by server_p and intialize the server_t. The server_p
**    is not freed.
*/
extern void dced_server_free_ptrs(
	/* [in, out] */	server_t*	/* server_p */
);

/*
** dced_server_free()
**    Free every non-null pointer in the server_t pointed
**    by *server_p; and free server_p as well. Finally
**    *server_p will be set to NULL.
*/
extern void dced_server_free(
	/* [in, out] */	server_t**	/* server_p */
);

/*
** dced_sm_client_free()
**    A wraper to rpc_sm_client_free().
*/
extern void dced_sm_client_free(
	void*	/* node_to_free */
);

/*
** dced_sm_free()
**    A wraper to rpc_sm_free().
*/
extern void dced_sm_free(
	void*	/* node_to_free */
);

/*
** entry_free_ptrs()
**    Call (*deallocate)() to free every non-null pointer in the entry
**    pointed by entry and intialize the entry. The entry is not freed.
*/
extern void entry_free_ptrs(
	dced_entry_t*	/* entry */,
	void (*)(void*)	/* deallocate */
);

/*
** entry_free()
**    Call *deallocate)() to free every non-null pointer in the entry
**    pointed by *entry; and free entry as well. Finally *entry will
**    be set to NULL.
*/
extern void entry_free(
	dced_entry_t**	/* entry */,
        void (*)(void*)	/* deallocate */
);

/*
** entry_list_free_ptrs()
**    Call (*deallocate)() to free every entry of the list pointed
**    by entry_list, and then free the whole list. The entry list
**    itself pointed  by entry_list is not freed, but initialized.
*/
extern void entry_list_free_ptrs(
	dced_entry_list_t*	/* entry_list */,
        void (*)(void*)		/* deallocate */
);

/*
** entry_list_free()
**    Call (*deallocate)() to free every entry of the list pointed
**    by *entry_list, and then free the whole list. Finally the
**    *entry_list is freed and set to NULL.
*/
extern void entry_list_free(
	dced_entry_list_t**	/* entry_list */,
        void (*)(void*)		/* deallocate */
);

/*
** attr_list_free_ptrs()
**    Call (*deallocate)() to free every sec_attr of the list pointed
**    by attr_list, and then free the whole list. The attr list itself
**    pointed  by attr_list is not freed, but initialized.
*/
extern void attr_list_free_ptrs(
	dced_attr_list_t*	/* attr_list */,
        void (*)(void*)		/* deallocate */
);

/*
** attr_list_free()
**    Call (*deallocate)() to free every sec_attr of the list pointed
**    by *attr_lis, and then free the whole list. Finally the *attr_list
**    itself is freed and set to NULL.
*/
extern void attr_list_free(
	dced_attr_list_t**	/* attr_list */,
	void (*)(void*)		/* deallocate */
);

/*
** string_list_free_ptrs()
**    Call (*deallocate)() to free every string of the list, and then
**    free the whole list. The string list itself pointed  by
**    string_list is not freed, but initialized.
*/
extern void string_list_free_ptrs(
	dced_string_list_t*	/* string_list */,
	void (*)(void*)		/* deallocate */
);

/*
** string_list_free()
**    Call (*deallocate)() to free every string of the list, and
**    then free the whole list. Finally the *string_list itself is
**    freeed and set to NULL.
*/
extern void string_list_free(
	dced_string_list_t**	/* string_list */,
	void (*)(void*)		/* deallocate */
);

/*
** uuid_list_free_ptrs()
**    Call (*deallocate)() to free the uuid list, the uuid list
**    itself pointed by uuid_list is not freed,  but initialized.
*/
extern void uuid_list_free_ptrs(
	uuid_list_t*	/* uuid_list */,
	void (*)(void*)	/* deallocate */
);

/*
** uuid_list_free()
**    Call (*deallocate)() to free the uuid list, and then
**    the *uuid_list is freed and set to NULL.
*/
extern void uuid_list_free(
	uuid_list_t**	/* uuid_list */,
	void (*)(void*)	/* deallocate */
);

/*
** opnum_list_free_ptrs()
**    Call (*deallocate)() to free the opnum list, the opnum list
**    itself pointed by opnum_list is not freed,  but initialized.
*/
extern void opnum_list_free_ptrs(
	dced_opnum_list_t*	/* opnum_list */,
	void (*)(void*)		/* deallocate */
);

/*
** opnum_list_free()
**    Call (*deallocate)() to free the opnum list, and then the
**    *opnum_list is freed and set to NULL.
*/
extern void opnum_list_free(
	dced_opnum_list_t**	/* opnum_list */,
	void (*)(void*)		/* deallocate */
);

/*
** key_free_ptrs()
**    Call (*deallocate)() to free every non-null pointer in
**    the key pointed by key and intialize the key. The key
**    is not freed.
*/
extern void key_free_ptrs(
	dced_key_t*	/* key */,
	void (*)(void*)	/* deallocate */
);

/*
** key_free()
**    Call (*deallocate)() to free every non-null pointer in the key
**    pointed by *key; and free *key as well. Finally *key will be
**    set to NULL.
*/
extern void key_free(
	dced_key_t**	/* key */,
	void (*)(void*)	/* deallocate */
);

/*
** key_list_free_ptrs()
**    Call (*deallocate)() to free every key of the list pointed
**    by key_list, and then free the whole list. The key list itself
**    pointed  by  key_list is not freed, but initialized.
*/
extern void key_list_free_ptrs(
	dced_key_list_t*	/* key_list */,
	void (*)(void*)		/* deallocate */
);

/*
** key_list_free()
**    Call (*deallocate)() to free every key of the list pointed by
**    *key_list, and then free the whole list. Finally the *key_list
**    is free and set to NULL.
*/
extern void key_list_free(
	dced_key_list_t**	/* key_list */,
	void (*)(void*)		/* deallocate */
);

/*
** tower_set_free()
**    Call (*deallocate)() to free the tower set pointed by *twr_set;
**    and free *twr_set as well. Finally *twr_set will be set to NULL.
** NOTE: The tower_set is assumed to be allocated by
**       rpc_tower_vector_from_binding and will be freed by
**       rpc_tower_vector_free
*/
extern void tower_set_free(
	sec_attr_twr_set_p_t*	/* twr_set */,
	void (*)(void*)		/* deallocate */
);

/*
** tower_list_free_ptrs()
**    Call (*deallocate)() to free every tower set of the list pointed
**    by twr_set_list and then free the whole list. The tower set list
**    itself pointed by twr_set_list is not freed, but initialized.
** NOTE: The each tower_set is assumed to be allocated by
**       rpc_tower_vector_from_binding and will be freed by
**       rpc_tower_vector_free
*/
extern void tower_list_free_ptrs(
	dced_tower_list_t*	/* twr_set_list */,
	void (*)(void*)		/* deallocate */
);

/*
** tower_list_free()
**    Call (*deallocate)() to free every tower set of the list pointed
**    by *twr_set_list and then free the whole list. Finally the
**    *twr_set_list is free and set to NULL.
** NOTE: The each tower_set is assumed to be allocated by
**       rpc_tower_vector_from_binding and will be freed by
**       rpc_tower_vector_free
*/
extern void tower_list_free(
	dced_tower_list_t**	/* twr_set_list */,
	void (*)(void*)		/* deallocate */
);

/*
** fixedattr_free_ptrs()
**    Call (*deallocate)() to free every non-null pointer in
**    the server_fixedattr_t pointed by fixedattr and intialize
**    the server_fixattr_t. The fixedattr is not freed.
*/
extern void fixedattr_free_ptrs(
	server_fixedattr_t*	/* fixedattr */,
	void (*)(void*)		/* deallocate */
);


/*
** fixedattr_free()
**    Call (*deallocate)() to free every non-null pointer in
**    the server_fixedattr pointed by *fixedattr; and free *fixedattr
**    as well. Finally *fixedattr will be set to NULL.
*/
extern void fixedattr_free(
	server_fixedattr_t**	/* fixedattr */,
	void (*)(void*)		/* deallocate */
);

/*
** service_free_ptrs()
**    Call (*deallocate)() to free every non-null pointer in the
**    service_t pointed by service and intialize the service_t.
**    The service is not freed.
*/
extern void service_free_ptrs(
	service_t*	/* service */,
	void (*)(void*)	/* deallocate */
);

/*
** service_free()
**    Call (*deallocate)() to free every non-null pointer in
**    the service_t pointed by *service; and free *service as well.
**    Finally *service will be set to NULL.
*/
extern void service_free(
	service_t**	/* service */,
	void (*)(void*)	/* deallocate */
);

/*
** service_list_free_ptrs()
**    Call (*deallocate)() to free every service of the list pointed by
**    service_list, and then free the whole list. The service list itself
**    pointed  by service_list is not freed, but initialized.
*/
extern void service_list_free_ptrs(
	service_list_t*	/* service_list */,
	void (*)(void*)	/* deallocate */
);

/*
** service_list_free()
**    Call (*deallocate)() to free every service of the list pointed
**    by *service_list, and then free the whole list. Finally the
**    *service_list is freed and set to NULL.
*/
extern void service_list_free(
	service_list_t**	/* service_list */,
	void (*)(void*)		/* deallocate */
);

/*
** server_free_ptrs()
**    Call (*deallocate)() to free every non-null pointer in
**    the server_t pointed by server and intialize the server_t.
**    The server is not freed.
*/
extern void server_free_ptrs(
	server_t*	/* server */,
	void (*)(void*)	/* deallocate */
);

/*
** server_free()
**    Call (*deallocate)() to free every non-null pointer in
**    the server_t pointed by *server; and free *server as well.
**    Finally *server will be set to NULL.
*/
extern void server_free(
	server_t**	/* server */,
	void (*)(void*)	/* deallocate */
);

#endif
