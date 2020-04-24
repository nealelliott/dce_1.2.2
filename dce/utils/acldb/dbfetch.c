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
 * $Log: dbfetch.c,v $
 * Revision 1.1.4.2  1996/02/18  19:20:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:28  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:07:22  root]
 * 
 * Revision 1.1.2.2  1994/06/16  19:35:20  rsalz
 * 	Remove compiler warning.
 * 	[1994/06/16  19:32:26  rsalz]
 * 
 * Revision 1.1.2.1  1994/01/24  22:01:37  bowe
 * 	Initial checkin
 * 	[1994/01/24  21:59:28  bowe]
 * 
 * Revision 1.1.2.2  1993/12/14  19:45:44  bowe
 * 	Fix botched switch statement.  It had "switch (a == b)", leftover
 * 	from a former if statement.
 * 	[1993/12/14  19:42:08  bowe]
 * 
 * Revision 1.1.2.1  1993/12/02  20:46:05  bowe
 * 	Initial revision.
 * 	[1993/12/02  20:44:47  bowe]
 * 
 * $EndLog$
 */

/* ______________________________________________________________________ 
 *
 *  Database / backing store entry fetch routines
 *  External interfaces:
 *	dce_db_fetch()
 *	dce_db_fetch_by_uuid()
 *	dce_db_fetch_by_name()
 */

#include <stdio.h>
#include <string.h>

#include <dce/uuid.h>
#include <dce/idl_es.h>
#include <dbimpl.h>

void
dce__db_fetch(
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
    error_status_t	x_st;

    if (!h->convert) {
	*st = db_s_missing_convert;
	return;
    }
    *st = error_status_ok;

    dkey.size = keysize;
    dkey.data = (char *)key;

    if ((h->db->get)(h->db, &dkey, &content, 0)) {
	*st = db_s_key_not_found;
	return;
    }

    ep    = (idl_byte *)content.data;
    esize = (idl_ulong_int)content.size;
    idl_es_decode_buffer(ep, esize, &eh, st);
    if (*st != error_status_ok) return;

    (h->convert)(eh, data, st);		/* decode the data */
    if (*st != error_status_ok) {
	st = &x_st;	/* so *_free call does not blow away this status */
    }
    idl_es_handle_free(&eh, st);
}

void
dce_db_fetch(
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
    default:
	*st = db_s_bad_index_type;
	return;
	break;
    }

    dce__db_fetch(h, key, keysize, data, st);
}

void
dce_db_fetch_by_name(
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
    dce__db_fetch(h, key, strlen(key)+1, data, st);
}

void
dce_db_fetch_by_uuid(
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
    dce__db_fetch(h, key, sizeof(uuid_t), data, st);
}

