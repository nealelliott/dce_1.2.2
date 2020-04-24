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
 * $Log: dced_util.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:48  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:34  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/28  21:44 UTC  mk
 * 	Merge in fixes from mk_mothra_dced/1.
 * 
 * 	HP revision /main/mk_mothra_dced/1  1995/04/28  21:37 UTC  mk
 * 	Fix OT12828:  Eliminate multiple frees of dced_entry_s_t members in
 * 	entry_free_ptrs() and fix another deallocation bug in string_list_free_ptrs().
 * 	[1995/12/08  17:59:04  root]
 * 
 * Revision 1.1.2.5  1994/08/24  19:26:53  bowe
 * 	Expunge key encryption and transmit_rep code. [CR 11773]
 * 	[1994/08/24  19:10:17  bowe]
 * 
 * Revision 1.1.2.4  1994/08/23  19:24:01  rsalz
 * 	Add entryname field to server_t (OT CR 11396).
 * 	[1994/08/23  19:18:37  rsalz]
 * 
 * Revision 1.1.2.3  1994/07/06  20:17:08  pwang
 * 	Reorg dced free routines [OT#11003]
 * 	[1994/07/06  20:06:50  pwang]
 * 
 * Revision 1.1.2.2  1994/06/09  16:02:53  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:29  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/15  22:29:05  pwang
 * 	Initial Implementation
 * 	[1994/04/15  21:32:53  pwang]
 * 
 * $EndLog$
 */

/*
** Module: dced_util.c
**
** FACILITY: DCE Host Daemon utilities 
**
** ABSTRACT: This module implements some convienent utility routine
**	     for the dced
**
** SCOPE: Public
**
** Date:
*/

#include <dcedimpl.h>

static dced_entry_t		s_null_entry;
static dced_key_t		s_null_key;
static server_fixedattr_t	s_null_fixedattr;
static service_t		s_null_service;
static server_t			s_null_server;

/*
**++
**
** dced_entry_free_ptrs()
**    Free every non-null pointer in the entry pointed by entry_p
**    and intialize the entry. The entry_p is not freed.
**
**--
*/
void
dced_entry_free_ptrs(dced_entry_t *entry_p)
{

	entry_free_ptrs(entry_p, free);

	return;
}

/*
**++
**
** dced_entry_free()
**    Free every non-null pointer in the entry pointed by *entry_p;
**    and free entry_p as well. Finally *entry_p will be set to NULL.
**
**--
*/
void
dced_entry_free(dced_entry_t **entry_p)
{

        entry_free(entry_p, free);

	return;
}

/*
**++
**
** dced_entry_list_free_ptrs()
**    Free every entry of the list pointed by entry_list_p, and
**    then free the whole list. The entry list itself pointed  by 
**    entry_list_p is not freed, but initialized. 
**
**--
*/
void
dced_entry_list_free_ptrs(dced_entry_list_t *entry_list_p)
{
	entry_list_free_ptrs(entry_list_p, free);

	return;
}

/*
**++
**
** dced_entry_list_free()
**    Free every entry of the list pointed by *entry_list_p, and
**    then free the whole list. Finally the *entry_list_p is freed and
**    set to NULL.
**
**--
*/
void
dced_entry_list_free(dced_entry_list_t **entry_list_p)
{

	entry_list_free(entry_list_p, free);

	return;
}

/*
**++
**
** dced_attr_list_free_ptrs()
**    Free every sec_attr of the list pointed by attr_list_p,
**    and then free the whole list. The attr list itself pointed  by 
**    attr_list_p is not freed, but initialized. 
**
**--
*/
void
dced_attr_list_free_ptrs(dced_attr_list_t *attr_list_p)
{

	attr_list_free_ptrs(attr_list_p, free);

	return;
}


/*
**++
**
** dced_attr_list_free()
**    Free every sec_attr of the list pointed by *attr_list_p,
**    and then free the whole list. Finally the *attr_list_p itself is 
**    freed and set to NULL.
**
**--
*/
void
dced_attr_list_free(dced_attr_list_t **attr_list_p)
{

	attr_list_free(attr_list_p, free);

	return;
}

/*
**++
**
** dced_string_list_free_ptrs()
**    Free every string of the list, and then free the whole list. 
**    The string list itself pointed  by  string_list_p is not freed, 
**    but initialized. 
**
**--
*/
void
dced_string_list_free_ptrs(dced_string_list_t *string_list_p)
{

	string_list_free_ptrs(string_list_p, free);
	
	return;
}

/*
**++
**
** dced_string_list_free()
**    Free every string of the list, and then free the whole list. 
**    Finally the *string_list_p itself is freeed and set to NULL.
**
**--
*/
void
dced_string_list_free(dced_string_list_t **string_list_p)
{

	string_list_free(string_list_p, free);

	return;
}

/*
**++
**
** dced_uuid_list_free_ptrs()
**    Free the uuid list, the uuid list itself pointed by uuid_list_p 
**    is not freed,  but initialized. 
**
**--
*/
void
dced_uuid_list_free_ptrs(uuid_list_t *uuid_list_p)
{

	uuid_list_free_ptrs(uuid_list_p, free);

	return;
}

/*
**++
**
** dced_uuid_list_free()
**    Free the uuid list, and then the *uuid_list_p is freed and set 
**    to NULL.
**
**--
*/
void
dced_uuid_list_free(uuid_list_t **uuid_list_p)
{

	uuid_list_free(uuid_list_p, free);

	return;
}


/*
**++
**
** dced_opnum_list_free_ptrs()
**    Free the opnum list, the opnum list itself pointed by opnum_list_p 
**    is not freed,  but initialized. 
**
**--
*/
void
dced_opnum_list_free_ptrs(dced_opnum_list_t *opnum_list_p)
{

	opnum_list_free_ptrs(opnum_list_p, free);

	return;
}

/*
**++
**
** dced_opnum_list_free()
**    Free the opnum list, and then the *opnum_list_p is freed and set 
**    to NULL.
**
**--
*/
void
dced_opnum_list_free(dced_opnum_list_t **opnum_list_p)
{

	opnum_list_free(opnum_list_p, free);

	return;
}

/*
**++
**
** dced_key_free_ptrs()
**    Free every non-null pointer in the key pointed by key_p
**    and intialize the key. The key_p is not freed.
**
**--
*/
void
dced_key_free_ptrs(dced_key_t *key_p)
{

	key_free_ptrs(key_p, free);

	return;
}

/*
**++
**
** dced_key_free()
**    Free every non-null pointer in the key pointed by *key_p;
**    and free *key_p as well. Finally *key_p will be set to NULL.
**
**--
*/
void
dced_key_free(dced_key_t **key_p)
{

	key_free(key_p, free);

	return;
}


/*
**++
**
** dced_key_list_free_ptrs()
**    Free every key of the list pointed by key_list_p, and
**    then free the whole list. The key list itself pointed  by 
**    key_list_p is not freed, but initialized. 
**
**--
*/
void
dced_key_list_free_ptrs(dced_key_list_t *key_list_p)
{

	key_list_free_ptrs(key_list_p, free);

	return;
}

/*
**++
**
** dced_key_list_free()
**    Free every key of the list pointed by *key_list_p, and
**    then free the whole list. Finally the *key_list_p is free
**    and set to NULL.
**
**--
*/
void
dced_key_list_free(dced_key_list_t **key_list_p)
{

	key_list_free(key_list_p, free);

	return;
}

/*
**++
**
** dced_tower_set_free()
**    Free the tower set pointed by *twr_set_p; and free *twr_set_p as well. 
**    Finally *twr_set_p will be set to NULL.
** NOTE: The tower_set is assumed to be allocated by 
**	 rpc_tower_vector_from_binding and will be freed by
**	 rpc_tower_vector_free
**
**--
*/
void
dced_tower_set_free(sec_attr_twr_set_p_t *twr_set_p)
{

	tower_set_free(twr_set_p, free);

	return;
}

/*
**++
**
** dced_tower_list_free_ptrs()
**    Free every tower set of the list pointed by twr_set_list_p and
**    then free the whole list. The tower set list itself pointed by
**    twr_set_list_p is not freed, but initialized.
** NOTE: The each tower_set is assumed to be allocated by 
**	 rpc_tower_vector_from_binding and will be freed by
**	 rpc_tower_vector_free
**
**--
*/
void
dced_tower_list_free_ptrs(dced_tower_list_t *twr_set_list_p)
{

	tower_list_free_ptrs(twr_set_list_p, free);

	return;
}

/*
**++
**
** dced_tower_list_free()
**    Free every tower set of the list pointed by *twr_set_list_p  and
**    then free the whole list. Finally the *twr_set_list_p is free
**    and set to NULL.
** NOTE: The each tower_set is assumed to be allocated by 
**	 rpc_tower_vector_from_binding and will be freed by
**	 rpc_tower_vector_free
**
**--
*/
void
dced_tower_list_free(dced_tower_list_t **twr_set_list_p)
{

	tower_list_free(twr_set_list_p, free);

	return;
}

/*
**++
**
** dced_fixedattr_free_ptrs()
**    Free every non-null pointer in the server_fixedattr_t pointed 
**    by fixedattr_p and intialize the server_fixattr_t. The fixedattr_p 
**    is not freed.
**
**--
*/
void
dced_fixedattr_free_ptrs(server_fixedattr_t *fixedattr_p)
{

	fixedattr_free_ptrs(fixedattr_p, free);

	return;
}

/*
**++
**
** dced_fixedattr_free()
**    Free every non-null pointer in the server_fixedattr pointed 
**    by *fixedattr_p; and free *fixedattr_p as well. Finally 
**    *fixedattr_p will be set to NULL.
**
**--
*/
void
dced_fixedattr_free(server_fixedattr_t **fixedattr_p)
{

	fixedattr_free(fixedattr_p, free);

	return;
}

/*
**++
**
** dced_service_free_ptrs()
**    Free every non-null pointer in the service_t pointed 
**    by service_p and intialize the service_t. The service_p 
**    is not freed.
**
**--
*/
void
dced_service_free_ptrs(service_t *service_p)
{

	service_free_ptrs(service_p, free);

	return;
}

/*
**++
**
** dced_service_free()
**    Free every non-null pointer in the service_t pointed 
**    by *service_p; and free *service_p as well. Finally 
**    *service_p will be set to NULL.
**
**--
*/
void
dced_service_free(service_t **service_p)
{

	service_free(service_p, free);

	return;
}

/*
**++
**
** dced_service_list_free_ptrs()
**    Free every service of the list pointed by service_list_p, and
**    then free the whole list. The service list itself pointed  by 
**    service_list_p is not freed, but initialized. 
**
**--
*/
void
dced_service_list_free_ptrs(service_list_t *service_list_p)
{

	service_list_free_ptrs(service_list_p, free);

	return;
}

/*
**++
**
** dced_service_list_free()
**    Free every service of the list pointed by *service_list_p, and
**    then free the whole list. Finally the *service_list_p is freed
**    and set to NULL.
**
**--
*/
void
dced_service_list_free(service_list_t **service_list_p)
{

	service_list_free(service_list_p, free);

	return;
}


/*
**++
**
** dced_server_free_ptrs()
**    Free every non-null pointer in the server_t pointed 
**    by server_p and intialize the server_t. The server_p 
**    is not freed.
**
**--
*/
void
dced_server_free_ptrs(server_t *server_p)
{

	server_free_ptrs(server_p, free);

	return;
}

/*
**++
**
** dced_server_free()
**    Free every non-null pointer in the server_t pointed 
**    by *server_p; and free *server_p as well. Finally 
**    *server_p will be set to NULL.
**
**--
*/
void
dced_server_free(server_t **server_p)
{

	server_free(server_p, free);

	return;
}

/*
**++
**
** dced_sm_client_free()
**    A wraper to rpc_sm_client_free().
**
**--
*/
void
dced_sm_client_free(void *node_to_free)
{
	error_status_t	status;

	CLEAR_STATUS(&status);

	rpc_sm_client_free(node_to_free, &status);
	if (!GOOD_STATUS(&status)) {
		dce_fprintf(stderr, status);
        }

	return;
}

/*
**++
**
** dced_sm_free()
**    A wraper to rpc_sm_free().
**
**--
*/
void
dced_sm_free(void *node_to_free)
{
	error_status_t	status;

	CLEAR_STATUS(&status);

	rpc_sm_free(node_to_free, &status);
	if (!GOOD_STATUS(&status)) {
		dce_fprintf(stderr, status);
        }

	return;
}

/*
**++
**
** entry_free_ptrs()
**    Call (*deallocate)() to free every non-null pointer in the entry 
**    pointed by entry and intialize the entry. The entry is not freed.
**
**--
*/
void
entry_free_ptrs(dced_entry_t	*entry, 
		void		(*deallocate)(void *ptr))
{

	if (entry == NULL) {
		return;
	}

	if (entry->name) {
		(*deallocate)(entry->name);
	}
	if (entry->description) {
		(*deallocate)(entry->description);
	}
#ifndef HPDCE_OT12828_FIX
	/* In server/init.c we aliased the pointers and IDL ES preserved
	 * that aliasing so make sure we don't free the pointer twice. */
	if (entry->storage_tag && entry->storage_tag != entry->name) {
#else
	if (entry->storage_tag) {
#endif /* HPDCE_OT12828_FIX */
		(*deallocate)(entry->storage_tag);
	}
   	*entry = s_null_entry;

	return;
}

/*
**++
**
** entry_free()
**    Call *deallocate)() to free every non-null pointer in the entry 
**    pointed by *entry; and free entry as well. Finally *entry will 
**    be set to NULL.
**
**--
*/
void
entry_free(dced_entry_t	**entry, 
	   void		(*deallocate)(void *ptr))
{


    	if (*entry) {
		entry_free_ptrs(*entry, deallocate);
		(*deallocate)(*entry);
		*entry = NULL;
    	}

	return;
}

/*
**++
**
** entry_list_free_ptrs()
**    Call (*deallocate)() to free every entry of the list pointed 
**    by entry_list, and then free the whole list. The entry list 
**    itself pointed  by entry_list is not freed, but initialized.
**
**--
*/
void
entry_list_free_ptrs(dced_entry_list_t	*entry_list, 
		     void		(*deallocate)(void *ptr))
{
	int i;

	if (EMPTY_LIST(entry_list)) {
		return;
	}

    	for (i=0; i<entry_list->count; i++) {
		entry_free_ptrs(&entry_list->list[i], deallocate);
	}
	(*deallocate)(entry_list->list);
    	entry_list->count = 0;
    	entry_list->list = NULL;

	return;
}

/*
**++
**
** entry_list_free()
**    Call (*deallocate)() to free every entry of the list pointed 
**    by *entry_list, and then free the whole list. Finally the 
**    *entry_list is freed and set to NULL.
**
**--
*/
void
entry_list_free(dced_entry_list_t	**entry_list, 
		void			(*deallocate)(void *ptr))
{
    	if (*entry_list) {
		entry_list_free_ptrs(*entry_list, deallocate);
		(*deallocate)(*entry_list);
		*entry_list = NULL;
    	}

	return;
}

/*
**++
**
** attr_list_free_ptrs()
**    Call (*deallocate)() to free every sec_attr of the list pointed 
**    by attr_list, and then free the whole list. The attr list itself 
**    pointed  by attr_list is not freed, but initialized.
**
**--
*/
void
attr_list_free_ptrs(dced_attr_list_t	*attr_list,
		    void		(*deallocate)(void *ptr))
{
	int i;

	if (EMPTY_LIST(attr_list)) {
		return;
	}

    	for (i=0; i<attr_list->count; i++) {
		sec_attr_util_free(deallocate, &attr_list->list[i]);
	}
    	(*deallocate)(attr_list->list);
    	attr_list->count = 0;
    	attr_list->list = NULL;

	return;
}

/*
**++
**
** attr_list_free()
**    Call (*deallocate)() to free every sec_attr of the list pointed 
**    by *attr_lis, and then free the whole list. Finally the *attr_list
**    itself is freed and set to NULL.
**
**--
*/
void
attr_list_free(dced_attr_list_t	**attr_list, 
		void		(*deallocate)(void *ptr))
{
    	if (*attr_list) {
		attr_list_free_ptrs(*attr_list, deallocate);
		(*deallocate)(*attr_list);
		*attr_list = NULL;
    	}

	return;
}

/*
**++
**
** string_list_free_ptrs()
**    Call (*deallocate)() to free every string of the list, and then 
**    free the whole list. The string list itself pointed  by  
**    string_list is not freed, but initialized.
**
**--
*/
void
string_list_free_ptrs(dced_string_list_t	*string_list, 
		      void			(*deallocate)(void *ptr))
{
	int i;

	if (EMPTY_LIST(string_list)) {
		return;
	}

	for (i=0; i<string_list->count; i++) {
#ifndef HPDCE_OT12828_FIX
		(*deallocate)(string_list->list[i]);
#else
		(*deallocate)(&string_list->list[i]);
#endif /* HPDCE_OT12828_FIX */
	}
    	(*deallocate)(string_list->list);
    	string_list->count = 0;
    	string_list->list = NULL;

	return;
}

/*
**++
**
** string_list_free()
**    Call (*deallocate)() to free every string of the list, and 
**    then free the whole list. Finally the *string_list itself is 
**    freeed and set to NULL.
**
**--
*/
void
string_list_free(dced_string_list_t	**string_list,
		 void			(*deallocate)(void *ptr))
{

	if (*string_list) {
		string_list_free_ptrs(*string_list, deallocate);
		(*deallocate)(*string_list);
		*string_list = NULL;
    	}

	return;
}

/*
**++
**
** uuid_list_free_ptrs()
**    Call (*deallocate)() to free the uuid list, the uuid list 
**    itself pointed by uuid_list is not freed,  but initialized.
**
**--
*/
void
uuid_list_free_ptrs(uuid_list_t *uuid_list,
		    void	(*deallocate)(void *ptr))
{
	if (EMPTY_LIST(uuid_list)) {
		return;
	}

    	(*deallocate)(uuid_list->list);
    	uuid_list->count = 0;
    	uuid_list->list = NULL;

	return;
}

/*
**++
**
** uuid_list_free()
**    Call (*deallocate)() to free the uuid list, and then 
**    the *uuid_list is freed and set to NULL.
**
**--
*/
void
uuid_list_free(uuid_list_t	**uuid_list, 
	       void		(*deallocate)(void *ptr))
{

	if (*uuid_list) {
		uuid_list_free_ptrs(*uuid_list, deallocate);
		(*deallocate)(*uuid_list);
		*uuid_list = NULL;
    	}

	return;
}

/*
**++
**
** opnum_list_free_ptrs()
**    Call (*deallocate)() to free the opnum list, the opnum list 
**    itself pointed by opnum_list is not freed,  but initialized.
**
**--
*/
void
opnum_list_free_ptrs(dced_opnum_list_t	*opnum_list, 
	       	     void		(*deallocate)(void *ptr))
{

	if (EMPTY_LIST(opnum_list)) {
		return;
	}

    	(*deallocate)(opnum_list->list);
    	opnum_list->count = 0;
    	opnum_list->list = NULL;

	return;
}

/*
**++
**
** opnum_list_free()
**    Call (*deallocate)() to free the opnum list, and then the 
**    *opnum_list is freed and set to NULL.
**
**--
*/
void
opnum_list_free(dced_opnum_list_t	**opnum_list, 
	       	void			(*deallocate)(void *ptr))
{

	if (*opnum_list) {
		opnum_list_free_ptrs(*opnum_list, deallocate);
		(*deallocate)(*opnum_list);
		*opnum_list = NULL;
    	}

	return;
}

/*
**++
**
** key_free_ptrs()
**    Call (*deallocate)() to free every non-null pointer in 
**    the key pointed by key and intialize the key. The key 
**    is not freed.
**
**--
*/
void
key_free_ptrs(dced_key_t	*key,
	      void		(*deallocate)(void *ptr))
{

	if (key == NULL) {
		return;
	}

	if (key->principal) {
		(*deallocate)(key->principal);
	}
	if (key->passwd) {
		switch (key->passwd->key.key_type) {

		case sec_passwd_none:
		    break;
		case sec_passwd_plain:
		    if (key->passwd->key.tagged_union.plain) {
			(*deallocate)(key->passwd->key.tagged_union.plain);
		    }
	    	    break;
		case sec_passwd_des:
		default:
		    break;
		}
		(*deallocate)(key->passwd);
    	}
    	*key = s_null_key;

	return;
}

/*
**++
**
** key_free()
**    Call (*deallocate)() to free every non-null pointer in the key 
**    pointed by *key; and free *key as well. Finally *key will be 
**    set to NULL.
**
**--
*/
void
key_free(dced_key_t	**key, 
	 void		(*deallocate)(void *ptr))
{

	if (*key) {
		key_free_ptrs(*key, deallocate);
		(*deallocate)(*key);
		*key = NULL;
	}

	return;
}

/*
**++
**
** key_list_free_ptrs()
**    Call (*deallocate)() to free every key of the list pointed 
**    by key_list, and then free the whole list. The key list itself 
**    pointed  by  key_list is not freed, but initialized.
**
**--
*/
void
key_list_free_ptrs(dced_key_list_t	*key_list, 
	 	   void			(*deallocate)(void *ptr))
{
	int i;

	if (EMPTY_LIST(key_list)) {
		return;
	}

	for (i=0; i<key_list->count; i++) {
		key_free_ptrs(&key_list->list[i], deallocate);
	}
	(*deallocate)(key_list->list);
	key_list->count = 0;
	key_list->list = NULL;

	return;
}

/*
**++
**
** key_list_free()
**    Call (*deallocate)() to free every key of the list pointed by 
**    *key_list, and then free the whole list. Finally the *key_list 
**    is free and set to NULL.
**
**--
*/
void
key_list_free(dced_key_list_t	**key_list,
	      void		(*deallocate)(void *ptr))
{

	if (*key_list) {
		key_list_free_ptrs(*key_list, deallocate);
		(*deallocate)(*key_list);
		*key_list = NULL;
    	}

	return;
}

/*
**++
**
** tower_set_free()
**    Call (*deallocate)() to free the tower set pointed by *twr_set; 
**    and free *twr_set as well. Finally *twr_set will be set to NULL.
** NOTE: The tower_set is assumed to be allocated by
**       rpc_tower_vector_from_binding and will be freed by
**       rpc_tower_vector_free
**
**--
*/
void
tower_set_free(sec_attr_twr_set_p_t	*twr_set, 
	       void			(*deallocate)(void *ptr))
{

	error_status_t status;

    	if (*twr_set) {
		rpc_tower_vector_free((rpc_tower_vector_p_t *) twr_set, 
				      &status);
		if (!GOOD_STATUS(&status)) {
			dce_fprintf(stderr, status);
		}
	}

	return;

}

/*
**++
**
** tower_list_free_ptrs()
**    Call (*deallocate)() to free every tower set of the list pointed 
**    by twr_set_list and then free the whole list. The tower set list 
**    itself pointed by twr_set_list is not freed, but initialized.
** NOTE: The each tower_set is assumed to be allocated by
**       rpc_tower_vector_from_binding and will be freed by
**       rpc_tower_vector_free
**
**--
*/
void
tower_list_free_ptrs(dced_tower_list_t	*twr_set_list, 
		     void		(*deallocate)(void *ptr))
{
	int i;

	if (EMPTY_LIST(twr_set_list)) {
		return;
	}

	for (i=0; i<twr_set_list->count; i++) {
		tower_set_free(&twr_set_list->list[i], deallocate);
	}
    	(*deallocate)(twr_set_list->list);
    	twr_set_list->count = 0;
    	twr_set_list->list = NULL;

	return;
}

/*
**++
**
** tower_list_free()
**    Call (*deallocate)() to free every tower set of the list pointed 
**    by *twr_set_list and then free the whole list. Finally the 
**    *twr_set_list is free and set to NULL.
** NOTE: The each tower_set is assumed to be allocated by
**       rpc_tower_vector_from_binding and will be freed by
**       rpc_tower_vector_free
**
**--
*/
void
tower_list_free(dced_tower_list_t	**twr_set_list, 
		void			(*deallocate)(void *ptr))
{

	if (*twr_set_list) {
		tower_list_free_ptrs(*twr_set_list, deallocate);
		(*deallocate)(*twr_set_list);
		*twr_set_list = NULL;
    	}

	return;
}

/*
**++
**
** fixedattr_free_ptrs()
**    Call (*deallocate)() to free every non-null pointer in 
**    the server_fixedattr_t pointed by fixedattr and intialize 
**    the server_fixattr_t. The fixedattr is not freed.
**
**--
*/
void
fixedattr_free_ptrs(server_fixedattr_t	*fixedattr,
		    void		(*deallocate)(void *ptr))
{

    	if (fixedattr == NULL) {
		return;
	}

	if (fixedattr->program) {
		(*deallocate)(fixedattr->program);
	}
    	string_list_free_ptrs(&fixedattr->arguments, deallocate);
    	uuid_list_free_ptrs(&fixedattr->prerequisites, deallocate);
    	uuid_list_free_ptrs(&fixedattr->keytables, deallocate);
    	if (fixedattr->posix_dir) {
		(*deallocate)(fixedattr->posix_dir);
	}
    	*fixedattr = s_null_fixedattr;

	return;
}

/*
**++
**
** fixedattr_free()
**    Call (*deallocate)() to free every non-null pointer in 
**    the server_fixedattr pointed by *fixedattr; and free *fixedattr
**    as well. Finally *fixedattr will be set to NULL.
**
**--
*/
void
fixedattr_free(server_fixedattr_t	**fixedattr,
	       void			(*deallocate)(void *ptr))
{

	if (*fixedattr) {
		fixedattr_free_ptrs(*fixedattr, deallocate);
		(*deallocate)(*fixedattr);
		*fixedattr = NULL;
    	}

	return;
}

/*
**++
**
** service_free_ptrs()
**    Call (*deallocate)() to free every non-null pointer in the 
**    service_t pointed by service and intialize the service_t. 
**    The service is not freed.
**
**--
*/
void
service_free_ptrs(service_t	*service,
		  void		(*deallocate)(void *ptr))
{

	if (service == NULL) {
		return;
	}

	if (service->ifname) {
		(*deallocate)(service->ifname);
	}
    	if (service->annotation) {
		(*deallocate)(service->annotation);
	}
    	tower_list_free_ptrs(&service->towers, deallocate);
    	uuid_list_free_ptrs(&service->objects, deallocate);
    	opnum_list_free_ptrs(&service->operations, deallocate);
    	if (service->entryname) {
		(*deallocate)(service->entryname);
	}
    	*service = s_null_service;

	return;
}

/*
**++
**
** service_free()
**    Call (*deallocate)() to free every non-null pointer in 
**    the service_t pointed by *service; and free *service as well. 
**    Finally *service will be set to NULL.
**
**--
*/
void
service_free(service_t	**service,
	     void	(*deallocate)(void *ptr))
{

	if (*service) {
		service_free_ptrs(*service, deallocate);
		(*deallocate)(*service);
		*service = NULL;
    	}

	return;
}

/*
**++
**
** service_list_free_ptrs()
**    Call (*deallocate)() to free every service of the list pointed by 
**    service_list, and then free the whole list. The service list itself 
**    pointed  by service_list is not freed, but initialized.
**
**--
*/
void
service_list_free_ptrs(service_list_t	*service_list,
		       void		(*deallocate)(void *ptr))
{
    int i;

	if (EMPTY_LIST(service_list)) {
		return;
	}

    	for (i=0; i<service_list->count; i++) {
		service_free_ptrs(&service_list->list[i], deallocate);
	}
    	(*deallocate)(service_list->list);
    	service_list->count = 0;
    	service_list->list = NULL;

	return;
}

/*
**++
**
** service_list_free()
**    Call (*deallocate)() to free every service of the list pointed 
**    by *service_list, and then free the whole list. Finally the 
**    *service_list is freed and set to NULL.
**
**--
*/
void
service_list_free(service_list_t	**service_list,
		  void			(*deallocate)(void *ptr))
{

	if (*service_list) {
		service_list_free_ptrs(*service_list, deallocate);
		(*deallocate)(*service_list);
		*service_list = NULL;
    	}

	return;
}

/*
**++
**
** server_free_ptrs()
**    Call (*deallocate)() to free every non-null pointer in 
**    the server_t pointed by server and intialize the server_t. 
**    The server is not freed.
**
**--
*/
void
server_free_ptrs(server_t	*server,
		 void		(*deallocate)(void *ptr))
{

	if (server == NULL) {
		return;
	}

    	if (server->name) {
		(*deallocate)(server->name);
	}
    	if (server->entryname) {
		(*deallocate)(server->entryname);
	}
    	service_list_free_ptrs(&server->services, deallocate);
    	fixedattr_free_ptrs(&server->fixed, deallocate);
    	attr_list_free_ptrs(&server->attributes, deallocate);
    	string_list_free_ptrs(&server->prin_names, deallocate);
    	*server = s_null_server;

	return;
}

/*
**++
**
** server_free()
**    Call (*deallocate)() to free every non-null pointer in 
**    the server_t pointed by *server; and free *server as well. 
**    Finally *server will be set to NULL.
**
**--
*/
void
server_free(server_t	**server,
	    void	(*deallocate)(void *ptr))
{

	if (*server) {
		server_free_ptrs(*server, deallocate);
		(*deallocate)(*server);
		*server = NULL;
    	}

	return;
}

