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
 * $Log: dbdelete.c,v $
 * Revision 1.1.4.2  1996/02/18  19:20:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:27  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:28  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/05  15:18 UTC  mullan_s
 * 	Merge CHFts15134
 * 
 * 	HP revision /main/mullan_mothra_bin_compat5/2  1995/06/01  18:23 UTC  mullan_s
 * 	Call utc_gettime_noshm instead of utc_gettime.
 * 	[1995/12/08  18:07:20  root]
 * 
 * Revision 1.1.2.3  1994/09/05  19:18:22  bowe
 * 	Check if in iterator loop before deleting. [CR 11049]
 * 	[1994/09/05  19:17:41  bowe]
 * 
 * Revision 1.1.2.2  1994/02/16  22:22:20  bowe
 * 	Added sync call after deleting entry.
 * 	[1994/02/16  22:21:48  bowe]
 * 
 * Revision 1.1.2.1  1994/01/24  22:01:36  bowe
 * 	Initial checkin
 * 	[1994/01/24  21:59:27  bowe]
 * 
 * Revision 1.1.2.2  1993/12/14  19:45:43  bowe
 * 	Fix botched switch statement.  It had "switch (a == b)", leftover
 * 	from a former if statement.
 * 	[1993/12/14  19:42:16  bowe]
 * 
 * Revision 1.1.2.1  1993/12/02  20:46:04  bowe
 * 	Initial revision.
 * 	[1993/12/02  20:44:45  bowe]
 * 
 * $EndLog$
 */

/* ______________________________________________________________________ 
 *
 *  Database / backing store single-entry delete routines
 *  External interfaces:
 *	dce_db_delete()
 *	dce_db_delete_by_uuid()
 *	dce_db_delete_by_name()
 */

#include <stdio.h>
#include <string.h>

#include <dce/uuid.h>
#include <dce/utc.h>
#include <dbimpl.h>

void
dce__db_delete(
    dce_db_handle_t	h,
    void		*key,
    unsigned32		keysize,
    error_status_t	*st
)
{
    DBT		dkey;
    db_info_t	*dbinfo;

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
    dkey.data = key;
    pthread_mutex_lock(&h->mutex);
    if ((h->db->del)(h->db, &dkey, 0)) {
	*st = db_s_del_failed;
	pthread_mutex_unlock(&h->mutex);
	return;
    }
    /* sync database so delete takes effect now */
    (h->db->sync)(h->db, 0);

    /* set the modify time for the database itself */
    dbinfo = h->info;
    utc_gettime_noshm(&dbinfo->modified);
    pthread_mutex_unlock(&h->mutex);
}

void
dce_db_delete(
    dce_db_handle_t	h,
    void		*key,
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
    dce__db_delete(h, key, keysize, st);
}

void
dce_db_delete_by_name(
    dce_db_handle_t	h,
    char		*key,
    error_status_t	*st
)
{
    if (h->index_type != db_c_index_by_name) {
	*st = db_s_bad_index_type;
	return;
    }
    dce__db_delete(h, key, strlen(key)+1, st);
}

void
dce_db_delete_by_uuid(
    dce_db_handle_t	h,
    uuid_t		*key,
    error_status_t	*st
)
{
    if (h->index_type != db_c_index_by_uuid) {
	*st = db_s_bad_index_type;
	return;
    }
    dce__db_delete(h, key, sizeof(uuid_t), st);
}

