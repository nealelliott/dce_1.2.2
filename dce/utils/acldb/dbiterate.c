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
 * $Log: dbiterate.c,v $
 * Revision 1.1.4.2  1996/02/18  19:20:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:07:29  root]
 * 
 * Revision 1.1.2.7  1994/09/05  19:18:23  bowe
 * 	Check if in iterator loop before starting/counting. [CR 11049]
 * 	[1994/09/05  19:17:47  bowe]
 * 
 * Revision 1.1.2.6  1994/07/28  14:03:43  rsalz
 * 	Use dce_strdup (OT CR 11343).
 * 	[1994/07/28  13:42:10  rsalz]
 * 
 * Revision 1.1.2.5  1994/06/16  19:35:22  rsalz
 * 	Copy iteration uuid into aligned space (OT CR 10957).
 * 	[1994/06/16  19:33:32  rsalz]
 * 
 * Revision 1.1.2.4  1994/03/28  21:34:53  bowe
 * 	Added new function: dce_db_inq_count()
 * 	[1994/03/28  21:18:23  bowe]
 * 
 * Revision 1.1.2.3  1994/03/24  17:05:18  bowe
 * 	Check index type in dce_db_iter_next_by_{name,uuid}()
 * 	[1994/03/24  16:39:53  bowe]
 * 
 * Revision 1.1.2.2  1994/03/09  21:49:01  bowe
 * 	Added dce_db_iter_next_by_name(), dce_db_iter_next_by_uuid()
 * 	[1994/03/09  21:26:28  bowe]
 * 
 * Revision 1.1.2.1  1994/01/24  22:01:42  bowe
 * 	Initial checkin
 * 	[1994/01/24  21:59:32  bowe]
 * 
 * Revision 1.1.2.1  1993/12/02  20:46:11  bowe
 * 	Initial revision.
 * 	[1993/12/02  20:44:49  bowe]
 * 
 * $EndLog$
 */

/* ______________________________________________________________________ 
 *
 *  Database / backing store entry iteration routines
 *  External interfaces:
 *	dce_db_iter_start()
 *	dce_db_iter_next()
 *	dce_db_iter_done()
 */

#include <stdio.h>

#include <dbimpl.h>
#include <dce/uuid.h>
#include <dce/dce_utils.h>

/* ______________________________________________________________________ */

void
dce_db_iter_start(
    dce_db_handle_t	h,
    error_status_t	*st
)
{
    if (h->iter_state != ITER_NOT) {
	*st = db_s_iter_not_allowed;
	return;
    }
    *st = error_status_ok;
    h->iter_state = ITER_BEGIN;
    h->namekey = NULL;
}

void
dce_db_iter_next(
    dce_db_handle_t	h,
    void		**key,
    error_status_t	*st
)
{
    DBT		dkey, xdata;

    do {
	switch (h->iter_state) {
	default:
	case ITER_AT_END:
	    /* not sure if this should be flagged as an error */
	    *st = db_s_no_more;
	    return;
	case ITER_BEGIN:
	    if ((h->db->seq)(h->db, &dkey, &xdata, R_FIRST)) {
		h->iter_state = ITER_AT_END;
		/* not sure if this should be flagged as an error */
		*st = db_s_no_more;
		return;
	    }
	    h->iter_state = ITER_IN_PROGRESS;
	    break;
	case ITER_IN_PROGRESS:
	    if ((h->db->seq)(h->db, &dkey, &xdata, R_NEXT)) {
		h->iter_state = ITER_AT_END;
		*st = db_s_no_more;
		return;
	    }
	    break;
	}
    } while (strcmp(DBHEADER_KEY, (char *)dkey.data) == 0);

    if (h->index_type == db_c_index_by_uuid) {
	memcpy(&h->uuidkey, dkey.data, sizeof h->uuidkey);
	*key = &h->uuidkey;
    }
    else {
	if (h->namekey)
	    free(h->namekey);
	*key = h->namekey = dce_strdup((char *)dkey.data);
    }
    *st = error_status_ok;
}

void
dce_db_iter_next_by_name(
    dce_db_handle_t	h,
    char		**key,
    error_status_t	*st
)
{
    if (h->index_type != db_c_index_by_name) {
	*st = db_s_bad_index_type;
	return;
    }
    dce_db_iter_next(h, (void **)key, st);
}

void
dce_db_iter_next_by_uuid(
    dce_db_handle_t	h,
    uuid_t		**key,
    error_status_t	*st
)
{
    if (h->index_type != db_c_index_by_uuid) {
	*st = db_s_bad_index_type;
	return;
    }
    dce_db_iter_next(h, (void **)key, st);
}

void
dce_db_iter_done(
    dce_db_handle_t     h,
    error_status_t      *st
)
{
    *st = error_status_ok;
    if (h->namekey) {
	free(h->namekey);
	h->namekey = NULL;
    }
    h->iter_state = ITER_NOT;
}

/* Return the number of items in the backing store. */
void
dce_db_inq_count(
    dce_db_handle_t     h,
    unsigned32		*count,
    error_status_t      *st
)
{
    DBT		dkey, xdata;

    if (h->iter_state != ITER_NOT) {
	*st = db_s_iter_not_allowed;
	return;
    }
    *st = error_status_ok;
    *count = 0;

    /* Note: no storage is allocated here, so there is no cleanup. */
    if ((h->db->seq)(h->db, &dkey, &xdata, R_FIRST))
	return;					/* 0 items */
    if (strcmp(DBHEADER_KEY, (char *)(dkey.data)))
	(*count)++;

    do {
	if ((h->db->seq)(h->db, &dkey, &xdata, R_NEXT))
	    return;
	/* Check for database header. If so, get next key. */
	if (!strcmp(DBHEADER_KEY, (char *)(dkey.data)))
	    continue;
	(*count)++;
    } while (TRUE);
}

