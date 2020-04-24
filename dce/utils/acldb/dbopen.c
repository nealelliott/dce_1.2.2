/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993,1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dbopen.c,v $
 * Revision 1.1.6.1  1996/05/10  13:06:51  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/04/19  20:36 UTC  arvind
 * 	Re-merge from /HPDCE02/5 to put back  db_s_stat_fail
 *
 * 	HP revision /main/DCE_1.2/arvind_checkout/1  1996/04/19  20:33 UTC  arvind
 * 	Re-merge from /HPDCE02/5 to put back db_s_stat_faill
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:34 UTC  psn
 * 	Submitted the fix for CHFts16512.
 * 	[1995/09/29  20:40 UTC  tatsu_s  /main/HPDCE02/8]
 *
 * 	Added db_c_lock_nowait.
 * 	[1995/09/23  00:16 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_local.b0/1]
 *
 * 	merged mutex leak fixes - CHFts16245
 * 	[1995/09/08  18:04 UTC  shu  /main/HPDCE02/7]
 *
 * 	fix more leaks
 * 	[1995/09/08  17:49 UTC  shu  /main/HPDCE02/shu_mothra_fix/2]
 *
 * 	destroy mutex to avoid leaks
 * 	[1995/09/07  19:02 UTC  shu  /main/HPDCE02/shu_mothra_fix/1]
 *
 * 	Merge fix for CHFts16255
 * 	[1995/09/06  19:18 UTC  mullan_s  /main/HPDCE02/6]
 *
 * 	Remove db_s_stat_fail error.
 * 	[1995/09/01  19:23 UTC  mullan_s  /main/HPDCE02/mullan_bc_errtable_skew/1]
 *
 * 	Merge CHFts15134
 * 	[1995/06/05  15:18 UTC  mullan_s  /main/HPDCE02/5]
 *
 * 	Merge out
 * 	[1995/06/01  18:23 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat5/3]
 *
 * 	Merge
 * 	[1995/05/22  17:04 UTC  mullan_s  /main/HPDCE02/4]
 *
 * 	If db_c_lock flag passed to dce_db_open, call dce_db_lock_file
 * 	instead of dce_db_lock.
 * 	[1995/05/22  15:38 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 *
 * 	Merge
 * 	[1995/05/15  19:00 UTC  mullan_s  /main/HPDCE02/3]
 *
 * 	If db_c_lock flag is specified, call dce_db_lock in dce_db_open
 * 	and dce_db_unlock in dce_db_close.
 * 	[1995/05/15  18:47 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat8/1]
 *
 * 	Merge
 * 	[1995/05/08  21:11 UTC  mullan_s  /main/HPDCE02/2]
 *
 * 	Add new API, dce_db_stat to return file status information.
 * 	[1995/05/08  19:38 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat4/1]
 *
 * 	Call utc_gettime_noshm instead of utc_gettime.
 * 	[1995/05/31  18:35 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat5/2]
 *
 * 	Binary Compatibility Merge
 * 	[1995/04/03  21:53 UTC  mullan_s  /main/HPDCE02/1]
 *
 * 	Add dce_db_open_file.
 * 	[1995/01/11  18:13 UTC  mullan_s  /main/mullan_mothra_bin_compat/1]
 *
 * Revision 1.1.2.7  1994/09/16  20:22:20  bowe
 * 	Initialize app_mutex in handle. [CR 12231]
 * 	[1994/09/16  20:20:27  bowe]
 * 
 * Revision 1.1.2.6  1994/05/27  17:43:37  bowe
 * 	Call free(*h) instead of free(h). [10779]
 * 	[1994/05/27  17:43:15  bowe]
 * 
 * Revision 1.1.2.5  1994/05/10  21:45:04  bowe
 * 	Use correct name: dce_db_convert_func_t
 * 	[1994/05/10  21:38:01  bowe]
 * 
 * Revision 1.1.2.4  1994/04/09  20:48:18  bowe
 * 	When opening with db_c_create, pass O_EXCL flag to dbopen() [10287].
 * 	Rearrange so handle is allocated after error checks [10286].
 * 	[1994/04/09  20:47:44  bowe]
 * 
 * Revision 1.1.2.3  1994/03/29  21:04:34  bowe
 * 	Set non-recursive mutex attribute.
 * 	[1994/03/29  21:03:57  bowe]
 * 
 * Revision 1.1.2.2  1994/03/09  22:56:44  bowe
 * 	Accomodate simple acl_uuid in data header.
 * 	[1994/03/09  22:49:12  bowe]
 * 
 * Revision 1.1.2.1  1994/01/24  22:01:44  bowe
 * 	Initial checkin
 * 	[1994/01/24  21:59:34  bowe]
 * 
 * Revision 1.1.4.1  1994/01/07  20:27:27  bowe
 * 	Added "sync" call after writing to the database.
 * 	[1994/01/07  19:20:48  bowe]
 * 
 * Revision 1.1.2.1  1993/12/02  20:46:13  bowe
 * 	Initial revision.
 * 	[1993/12/02  20:44:51  bowe]
 * 
 * $EndLog$
 */

/* ______________________________________________________________________ 
 *
 *  Database / backing store open routines
 *  External interfaces:
 *	dce_db_open()
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#include <dce/uuid.h>
#include <dce/utc.h>
#include <dbimpl.h>

static boolean			dce_db_s_mutex_setup;
static pthread_mutexattr_t	dce_db_s_mutex_attr;
static pthread_once_t		dce_db_s_once_block = pthread_once_init;

/* forward refs */
static void	dce_db__info_new(dce_db_handle_t, error_status_t *);
static void	dce_db__info_read(dce_db_handle_t, error_status_t *);

/* ______________________________________________________________________ */
/*	Open the database
 */

static void
dce_db__init_mutex_attr( void )
{
    pthread_mutexattr_t	new_attr;

    dce_db_s_mutex_attr = pthread_mutexattr_default;
#if defined(MUTEX_NONRECURSIVE_NP)
    if (pthread_mutexattr_create(&new_attr) < 0)
	return;
    if (pthread_mutexattr_setkind_np(&new_attr, MUTEX_NONRECURSIVE_NP) < 0) {
	pthread_mutexattr_delete(&new_attr);
	return;
    }
    dce_db_s_mutex_attr = new_attr;
#endif
    dce_db_s_mutex_setup = TRUE;
}

/*
 * dce_db_open
 *
 * LOCKING RULES:
 *	If the db_c_lock flag is specified, the file is shared
 *	locked if the db_c_readonly flag was also specified. 
 *      Otherwise the file is locked exclusively. This is an
 *	advisory interprocess lock. Multi-threaded programs
 *      which specify the db_c_lock flag should mutex lock the
 *      call to dce_db_open to prevent other threads from
 *      interfering.
 *
 *	dce_db_lock creates a mutex lock on the in-memory copy 
 *	of the dce_db_handle_t returned by dce_db_open, It does
 *	not mutex lock the file which it was read from.
 */ 
void
dce_db_open(
    const char          *filename,
    const char          *backend_type,
    unsigned32          flags,
    dce_db_convert_func_t convert,
    dce_db_handle_t     *h,
    error_status_t      *st
)
{
    *st = error_status_ok;

    /* caller's umask will do the right thing with file mode */
    dce_db_open_file(filename, backend_type, flags, 0666, NULL,
                     convert, h, st);
}

/*
 * This routine was added so that the caller could specify
 * additional useful options to the db layer. These include
 * the mode of the file (mode) to be created and access specific 
 * flags (openinfo) for the chosen backend type.
 *
 * dce_db_open now calls dce_db_open_file.
 *
 * NOTE: see comments for dce_db_open on locking rules.
 */
void
dce_db_open_file(
    const char		*filename,
    const char		*backend_type,
    unsigned32		flags,
    int                 mode,
    void                *openinfo,
    dce_db_convert_func_t convert,
    dce_db_handle_t	*h,
    error_status_t	*st
)
{
    int			oflags;
    DBTYPE		type;
    unsigned32		index_type;
    boolean		readonly;
    DB			*db;
    pthread_mutex_t	mutex, app_mutex;

    *st = error_status_ok;
    *h = NULL;

    /* Look at flags for index type. Return error if both types are set. */
    if ((flags & (db_c_index_by_name | db_c_index_by_uuid)) ==
		 (db_c_index_by_name | db_c_index_by_uuid)) {
	*st = db_s_bad_index_type;
	return;
    }
    /* Same story for header type. */
    if ((flags & (db_c_std_header | db_c_acl_uuid_header)) ==
		 (db_c_std_header | db_c_acl_uuid_header)) {
	*st = db_s_bad_header_type;
	return;
    }

    if (!filename || *filename == '\0') {
	*st = db_s_no_name_specified;
	return;
    }

    oflags = 0;
    if (flags & db_c_create)
	oflags |= (O_CREAT | O_EXCL);

    /* be sure one of the index types is specified */
    if (flags & db_c_index_by_name)
	index_type = db_c_index_by_name;
    else if (flags & db_c_index_by_uuid)
	index_type = db_c_index_by_uuid;
    else {
	*st = db_s_bad_index_type;
	return;
    }

    if (flags & db_c_readonly) {
	oflags |= O_RDONLY;
	readonly = TRUE;
    }
    else {
	oflags |= O_RDWR;
	readonly = FALSE;
    }

    /* Set up type, which depends on the backend. */
    if (!backend_type || backend_type[0] == 0 ||
		!strcmp(backend_type, "bsd4.4-hash")) {
	type = DB_HASH;
    }
    else if (!strcmp(backend_type, "bsd4.4-btree")) {
	type = DB_BTREE;
    }
    else if (!strcmp(backend_type, "bsd4.4-recno")) {
	type = DB_RECNO;
    }
#ifdef NotImplementedYet
    else if (!strcmp(backend_type, "bsd4.4-mpool")) {
	type = DB_MPOOL;
    }
#endif
    else {
	type = DB_HASH;
    }

    if (!(db = dbopen(filename, oflags, mode, type, openinfo))) {
	if (errno == EACCES)
	    *st = db_s_open_failed_eacces;
	else if (errno == ENOENT)
	    *st = db_s_open_failed_enoent;
	else if (errno == EEXIST)
	    *st = db_s_open_already_exists;
	else
	    *st = db_s_open_failed;
	return;
    }

    if (!dce_db_s_mutex_setup) {
	pthread_once(&dce_db_s_once_block,
		(pthread_initroutine_t)dce_db__init_mutex_attr);
    }
    if (pthread_mutex_init(&mutex, dce_db_s_mutex_attr) != 0) {
	*st = db_s_mutex_init_fail;
	return;
    }
    if (pthread_mutex_init(&app_mutex, dce_db_s_mutex_attr) != 0) {
#ifndef __hpux_CHFts16245
	pthread_mutex_destroy(&mutex);
#endif
	*st = db_s_mutex_init_fail;
	return;
    }

    /* allocate handle */
    (*h) = (dce_db_handle_t)malloc(sizeof(struct dce_db_handle_s_t));
    memset(*h, 0, sizeof(struct dce_db_handle_s_t));
    (*h)->db		= db;
    (*h)->mutex		= mutex;
    (*h)->app_mutex	= app_mutex;
    (*h)->index_type	= index_type;
    (*h)->readonly	= readonly;
    (*h)->lockflag	= (flags & (db_c_lock|db_c_lock_nowait)) ? TRUE : FALSE;
    (*h)->info		= NULL;

    /* 
     * If db_c_lock flag specified, call dce__db_lock_file. This will 
     * exclusively lock the file unless it is opened readonly, in which 
     * case the lock is shared. The file will be unlocked by dce_db_close.
     */
    if ((*h)->lockflag) {
	dce__db_lock_file(*h, true, ((flags&db_c_lock_nowait)? true : false), st);
	if (*st != error_status_ok) {
	    (void)((*h)->db->close)((*h)->db);
#ifndef __hpux_CHFts16245
	    pthread_mutex_destroy(&(*h)->mutex);
	    pthread_mutex_destroy(&(*h)->app_mutex);
#endif
	    if ((*h)->info)
		free((*h)->info);
	    free(*h);
	    *h = NULL;
	    return;
        }
    }

    /* If creating, write database header as a data entry, tagged with our
     * hard-coded key. */
    if (flags & db_c_create) {
	dce_db__info_new(*h, st);
	if (flags & db_c_std_header) {
	    (*h)->info->header_type = dce_db_header_std;
	}
	else if (flags & db_c_acl_uuid_header) {
	    (*h)->info->header_type = dce_db_header_acl_uuid;
	}
	else {
	    (*h)->info->header_type = dce_db_header_none;
	}
	dce_db__info_write(*h, st);
    }
    else {
	dce_db__info_read(*h, st);

	/* Compare with flags passed int.  Complain if index mismatch. */
	/* check if one of the flags were specified as arguement. If so,
	 * be sure it agrees with what we have stored in database. */
	if (flags & (db_c_index_by_name | db_c_index_by_uuid)) {
	    if (! ( ((flags & db_c_index_by_name) == (*h)->index_type) ||
		    ((flags & db_c_index_by_uuid) == (*h)->index_type) ) ) {
		*st = db_s_index_type_mismatch;
		(void)((*h)->db->close)((*h)->db);
#ifndef __hpux_CHFts16245
	        pthread_mutex_destroy(&(*h)->mutex);
		pthread_mutex_destroy(&(*h)->app_mutex);
#endif
		if ((*h)->info)
		    free((*h)->info);
		free(*h);
		*h = NULL;
		return;
	    }
	}
	(*h)->index_type  = (*h)->info->index_type;
    }
    (*h)->convert = convert;
}

/* ______________________________________________________________________ */
/*	Close the database.  This is not in its own module because
 *	the caller is supposed to close a db after opening it. I.e.,
 *	open and close should *both* be called.
 */

void
dce_db_close(
    dce_db_handle_t	*h,
    error_status_t	*st
)
{
    *st = error_status_ok;
    if ((*h)->readonly == FALSE) {
	dce_db__info_write(*h, st);
    }

    /*
     * If file is locked, unlock file before closing.
     * (dce_db__info_write already flushed the data to disk)
     */
    if ((*h)->lockflag) {
	dce__db_lock_file(*h, false, false, st);
    }
    (void)((*h)->db->close)((*h)->db);
    pthread_mutex_destroy(&(*h)->mutex);
#ifndef __hpux_CHFts16245
    pthread_mutex_destroy(&(*h)->app_mutex);
#endif
    if ((*h)->info)
	free((*h)->info);
    free(*h);
    *h = NULL;
}

/*
 * dce_db_stat
 *
 * Return file status info.
 */
void
dce_db_stat(
    dce_db_handle_t	h,
    struct stat		*db_stat,
    error_status_t	*st
)
{
    *st = error_status_ok;

    if (fstat((h->db->fd)(h->db), db_stat) == -1)
	*st = db_s_stat_fail;
}

/* ______________________________________________________________________ */
/*  Allocate and initialize a new datase info structure */

static void
dce_db__info_new(
    dce_db_handle_t	h,
    error_status_t	*st
)
{
    utc_t		now;

    *st = error_status_ok;
    h->info = calloc(1, sizeof(db_info_t));
    utc_gettime_noshm(&now);
    h->info->created	= now;
    h->info->modified	= now;
    h->info->index_type	= h->index_type;
}

/* Read info header from database into this database's private structure. */
static void
dce_db__info_read(
    dce_db_handle_t	h,
    error_status_t	*st
)
{
    DBT		dkey, content;

    *st = error_status_ok;
    dkey.data = DBHEADER_KEY;
    dkey.size = strlen((char *)dkey.data) + 1;
    pthread_mutex_lock(&h->mutex);
    if ((h->db->get)(h->db, &dkey, &content, 0)) {
	*st = db_s_fetch_failed;
	/* free stuff first ... */
	pthread_mutex_unlock(&h->mutex);
	return;
    }
    if (!h->info) {
	h->info = malloc(sizeof(db_info_t));
    }
    memcpy(h->info, content.data, sizeof(db_info_t));
    pthread_mutex_unlock(&h->mutex);
}

/* Write info header to the database. */
void
dce_db__info_write(
    dce_db_handle_t	h,
    error_status_t	*st
)
{
    DBT		dkey, content;

    *st = error_status_ok;
    dkey.data = DBHEADER_KEY;
    dkey.size = strlen((char *)dkey.data) + 1;
    content.data = (char *)h->info;
    content.size = sizeof(db_info_t);
    pthread_mutex_lock(&h->mutex);
    if ((h->db->put)(h->db, (DBT *)&dkey, (DBT *)&content, 0)) {
	*st = db_s_store_failed;
	/* free stuff first ... */
    }
    (h->db->sync)(h->db, 0);
    pthread_mutex_unlock(&h->mutex);
}
