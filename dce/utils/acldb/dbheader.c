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
 * $Log: dbheader.c,v $
 * Revision 1.1.4.2  1996/02/18  19:20:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:07:24  root]
 * 
 * Revision 1.1.2.6  1994/09/19  19:52:51  bowe
 * 	No more status param to dce_db_header_convert(). Now catch
 * 	the exception instead.  [CR 10000]
 * 	[1994/09/19  19:52:30  bowe]
 * 
 * Revision 1.1.2.5  1994/05/11  15:33:04  bowe
 * 	Correct args/cleanup dce_db_std_header_init() [CR 10603]
 * 	[1994/05/11  15:32:23  bowe]
 * 
 * Revision 1.1.2.4  1994/05/10  21:45:02  bowe
 * 	Fix implementation of dce_db_header_fetch(). (needed idl/es support)
 * 	Added 2 params to dce_db_std_header_init().  [CR 10000]
 * 	[1994/05/10  21:37:08  bowe]
 * 
 * Revision 1.1.2.3  1994/04/06  14:47:27  bowe
 * 	Null header before initializing at top of dce_db_std_header_init().
 * 	Also, optimize header type check and set type in item header struct.
 * 	[1994/04/06  14:47:10  bowe]
 * 
 * Revision 1.1.2.2  1994/03/09  22:56:43  bowe
 * 	Added global "dce_db_g_null_header". Changed dce_db_std_header_fetch()'s
 * 	hdr arg to "void *" (not **).  Removed some #ifdef'ed-out code.
 * 	[1994/03/09  22:48:18  bowe]
 * 
 * Revision 1.1.2.1  1994/01/24  22:01:39  bowe
 * 	Initial checkin
 * 	[1994/01/24  21:59:29  bowe]
 * 
 * Revision 1.1.2.1  1993/12/02  20:46:08  bowe
 * 	Initial revision.
 * 	[1993/12/02  20:44:48  bowe]
 * 
 * $EndLog$
 */

/* ______________________________________________________________________ 
 *
 *  Database / backing store header initialization and fetch routines.
 *  External interfaces:
 *	dce_db_std_header_init()
 *	dce_db_header_fetch()
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include <dce/uuid.h>
#include <dce/idl_es.h>
#include <dbimpl.h>

dce_db_header_t	dce_db_g_null_header;

/* ______________________________________________________________________ */
/* Initialize a standard header.  Caller passes in a pointer to the data
 * enry (which contains the header) and all the things to set. We then set
 * them.  This is safer than having the user muck with the header structure
 * itself.
 */

void
dce_db_std_header_init(
    dce_db_handle_t	h,
    dce_db_header_t	*hdr,
    uuid_t		*uuid,
    uuid_t		*acl_uuid,
    uuid_t		*def_object_acl,
    uuid_t		*def_container_acl,
    unsigned32		ref_count,
    error_status_t	*st
)
{
    if (h->info->header_type != dce_db_header_std) {
	*st = db_s_bad_header_type;
	return;
    }
    *st = error_status_ok;
    *hdr = dce_db_g_null_header;
    hdr->type = dce_db_header_std;

    /* Fill in the header fields */
    if (uuid)
	hdr->tagged_union.h.uuid = *uuid;
    if (acl_uuid)
	hdr->tagged_union.h.acl_uuid = *acl_uuid;
    if (def_object_acl)
	hdr->tagged_union.h.def_object_acl = *def_object_acl;
    if (def_container_acl)
	hdr->tagged_union.h.def_container_acl = *def_container_acl;
    hdr->tagged_union.h.ref_count = ref_count;
}

/* ______________________________________________________________________ */
/* Return a copy of the object's header (allocated by the encoding services).
 * Caller should free it if not in a server RPC operation.
 */
void
dce_db_header_fetch(
    dce_db_handle_t	h,
    void		*key,
    dce_db_header_t	*hdr,
    error_status_t	*st
)
{
    DBT			dkey, content;
    idl_ulong_int	esize;
    idl_byte		*ep;
    idl_es_handle_t	eh;
    error_status_t	x_st;

    if (h->info->header_type != dce_db_header_std) {
	*st = db_s_no_header;
	return;
    }

    *st = error_status_ok;
    /* Index type is always OK, since dce_db_open() checks. */
    switch (h->index_type) {
    case db_c_index_by_name: 
	dkey.size = strlen(key) + 1;
	break;
    case db_c_index_by_uuid:
	dkey.size = sizeof(uuid_t);
	break;
    }
    dkey.data = (char *)key;
    if ((h->db->get)(h->db, &dkey, &content, 0)) {
	*st = db_s_key_not_found;
	return;
    }

    ep    = (idl_byte *)content.data;
    esize = (idl_ulong_int)content.size;
    idl_es_decode_buffer(ep, esize, &eh, st);
    if (*st != error_status_ok) return;

    idl_es_set_attrs(eh, IDL_ES_NO_ENCODING_CHECK, st);
    if (*st != error_status_ok) {
	idl_es_handle_free(&eh, &x_st);
	return;
    }

    TRY
	/* decode just the header */
	dce_db_header_convert(eh, (void *)hdr);
    CATCH_ALL
	*st = rpc_s_comm_failure;
    ENDTRY
    idl_es_handle_free(&eh, &x_st);
    if (*st == error_status_ok && x_st != error_status_ok)
	*st = x_st;
}

