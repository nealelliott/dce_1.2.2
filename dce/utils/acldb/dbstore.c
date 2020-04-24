/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dbstore.c,v $
 * Revision 1.1.4.2  1996/02/18  19:20:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:41  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/05  15:18 UTC  mullan_s
 * 	Merge CHFts15134
 * 
 * 	HP revision /main/mullan_mothra_bin_compat5/2  1995/06/01  18:23 UTC  mullan_s
 * 	Use utc_gettime_noshm instead of utc_gettime.
 * 	[1995/12/08  18:07:34  root]
 * 
 * Revision 1.1.2.5  1994/09/05  19:18:24  bowe
 * 	Check if in iterator loop before storing. [CR 11049]
 * 	[1994/09/05  19:17:50  bowe]
 * 
 * Revision 1.1.2.4  1994/06/10  22:26:18  cbrooks
 * 	CR 10897
 * 	[1994/06/10  21:11:40  cbrooks]
 * 
 * Revision 1.1.2.3  1994/06/07  14:44:04  bowe
 * 	Free ES buffer with rpc_sm_free(), not free(). [CR 10757]
 * 	[1994/06/07  14:43:48  bowe]
 * 
 * Revision 1.1.2.2  1994/03/09  22:56:45  bowe
 * 	Accomodate simple acl_uuid in data header.
 * 	[1994/03/09  22:49:27  bowe]
 * 
 * Revision 1.1.2.1  1994/01/24  22:01:45  bowe
 * 	Initial checkin
 * 	[1994/01/24  21:59:35  bowe]
 * 
 * Revision 1.1.4.1  1994/01/07  20:27:29  bowe
 * 	Added "sync" call after writing to the database.
 * 	[1994/01/07  19:20:39  bowe]
 * 
 * Revision 1.1.2.1  1993/12/02  20:46:14  bowe
 * 	Initial revision.
 * 	[1993/12/02  20:44:52  bowe]
 * 
 * $EndLog$
 */

/* ______________________________________________________________________ 
 *
 *  Database / backing store entry store routines
 *  External interfaces:
 *	dce_db_store()
 *	dce_db_store_by_uuid()
 *	dce_db_store_by_name()
 */

#include <stdio.h>
#include <string.h>

#include <dce/uuid.h>
#include <dce/idl_es.h>
#include <dce/utc.h>
#include <dbimpl.h>

void
dce__db_store(
    dce_db_handle_t	h,
    void		*key,
    unsigned32		keysize,
    void		*data,
    error_status_t	*st
)
{
    DBT			dkey, content;
    idl_ulong_int	esize;
    idl_byte		*ep;
    idl_es_handle_t	eh;
    db_info_t		*dbinfo;
    error_status_t	st2;

    if (!h->convert) {
	*st = db_s_missing_convert;
	return;
    }
    if (h->readonly == TRUE) {
	*st = db_s_readonly;
	return;
    }
    if (h->iter_state != ITER_NOT) {
	*st = db_s_iter_not_allowed;
	return;
    }
    *st = error_status_ok;

    dkey.size = keysize;
    dkey.data = key;			/* key pointer */

    switch (h->info->header_type) {
	dce_db_header_t		*hdr;		/* obj/record header */
	dce_db_dataheader_t	*shdr;		/* standard header */
    case dce_db_header_none:
	break;
    case dce_db_header_std:		/* standard header */
	/* Set the modify time for this object/record. */
	hdr = (dce_db_header_t *)data;
	shdr = &hdr->tagged_union.h;
	hdr->type = dce_db_header_std;	/* for case in conversion routine */
        utc_gettime_noshm(&shdr->modified);	/* modification time */
	shdr->modified_count++;		/* modification counter */
	break;
    case dce_db_header_acl_uuid:	/* ACL header */
	/* No real action - this is just the UUID of the ACL. Set the type. */
	hdr = (dce_db_header_t *)data;
	hdr->type = dce_db_header_acl_uuid;
	break;
    }

    idl_es_encode_dyn_buffer(&ep, &esize, &eh, st);
    if (*st != error_status_ok) return;

    (h->convert)(eh, data, st);		/* encode the data */
    if (*st != error_status_ok) {
	error_status_t x_st;
	idl_es_handle_free(&eh, &x_st);
	return;
    }

    content.data = ep;			/* fill in struct for dbm routine */
    content.size = esize;
    pthread_mutex_lock(&h->mutex);

    /* now store it */
    if ((h->db->put)(h->db, &dkey, &content, 0)) {
	*st = db_s_store_failed;
    }
    else {
	/* set the modify time for the database itself */
	dbinfo = h->info;
        utc_gettime_noshm(&dbinfo->modified);
    }
    (h->db->sync)(h->db, 0);
    pthread_mutex_unlock(&h->mutex);
    idl_es_handle_free(&eh, st);
    rpc_sm_client_free(ep, &st2);
    if (*st == error_status_ok && st2 != error_status_ok)
	*st = st2;
}

void
dce_db_store(
    dce_db_handle_t	h,
    void		*key,
    void		*data,
    error_status_t	*st
)
{
    unsigned32		keysize;

    switch (h->index_type) {
    case db_c_index_by_name: 
	keysize = strlen(key) + 1;
	break;
    case db_c_index_by_uuid:
	keysize = sizeof(uuid_t);
	break;
    }

    dce__db_store(h, (void *)key, keysize, data, st);
}

void
dce_db_store_by_name(
    dce_db_handle_t	h,
    char		*key,
    void		*data,
    error_status_t	*st
)
{
    if (h->index_type != db_c_index_by_name) {
	*st = db_s_bad_index_type;
	return;
    }
    dce__db_store(h, (void *)key, strlen(key)+1, data, st);
}

void
dce_db_store_by_uuid(
    dce_db_handle_t	h,
    uuid_t		*key,
    void		*data,
    error_status_t	*st
)
{
    if (h->index_type != db_c_index_by_uuid) {
	*st = db_s_bad_index_type;
	return;
    }
    dce__db_store(h, (void *)key, sizeof(uuid_t), data, st);
}

