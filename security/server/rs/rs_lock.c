/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_lock.c,v $
 * Revision 1.1.8.2  1996/02/18  00:17:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:43  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:51:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:25  root]
 * 
 * Revision 1.1.6.1  1994/07/15  15:01:22  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:19:23  mdf]
 * 
 * Revision 1.1.3.4  1992/12/29  16:35:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:23:37  zeliff]
 * 
 * Revision 1.1.3.3  1992/10/07  20:32:43  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:16:24  sekhar]
 * 
 * Revision 1.1.4.2  1992/10/06  16:09:43  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1.3.2  1992/09/29  20:12:44  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:36:51  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      Registry Server - lock manager
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rs_lock.c	11 - 09/20/91";
#endif

#include <dce/nbase.h>

#include <un_io.h>
#include <un_strng.h>
#include <bstring.h>
#include <macros.h>
#include <pthread.h>
#include <rwl.h>
#include <rs_task.h>
#include <rs_lock.h>


PUBLIC  boolean32 rs_lock_debug = false;

PRIVATE  boolean32 locks_inited = false;
PRIVATE  boolean32 fatal_error_occurred = false;

typedef struct lock_pvt_t {
    rwl_handle_t        lock_handle;
    char                *debug_info;
} lock_pvt_t;

/* Database locks */
static  lock_pvt_t  db;

/* Replica list locks */
static  lock_pvt_t  replist;

/* Log locks */
static  lock_pvt_t  log;

/* KDC lock */
static  lock_pvt_t  kdc;

/* master info lock */
static  lock_pvt_t  master_info;

/* master info lock */
static  lock_pvt_t  login;

#define NUM_LOCKS 6
static  lock_pvt_t  *known_locks[NUM_LOCKS] = { &db,
                                                &replist,
                                                &log,
                                                &kdc,
                                                &master_info,
                                                &login
                                              };

static  char   *lock_debug_info[NUM_LOCKS] = { "database lock",
                                               "replica list lock",
                                               "log lock",
                                               "KDC lock",
                                               "master info lock",
                                               "login info lock"
                                             };

PRIVATE print_lock_info
#ifndef __STDC__
    ( type, lock_p )
    char        *type;
    lock_pvt_t  *lock_p;
#else
  (
    char        *type,
    lock_pvt_t  *lock_p
  )
#endif
{
    char            buf[128];

    if (lock_p == &replist) {
        sprintf(buf, "%s: replist\n", type);
    } else if (lock_p == &db) {
        sprintf(buf, "%s: db\n", type);
    } else if (lock_p == &log) {
        sprintf(buf, "%s: log\n", type);
     } else if (lock_p == &kdc) {
        sprintf(buf, "%s: kdc\n", type);
     } else if (lock_p == &master_info) {
        sprintf(buf, "%s: master_info\n", type);
     } else if (lock_p == &login) {
        sprintf(buf, "%s: login_info\n", type);
    } else {
        sprintf(buf, "%s: <unknown lock>\n", type);
    }

    write(2, buf, strlen(buf));
}

PRIVATE void lock_init (
#ifdef __STDC__
    void
#endif
)
{
    int         i;

    if (!locks_inited) {

	if (rs_lock_debug) {
	    rwl_lock_debug = true;
	}

	rwl_module_init();
	
	for (i = 0; i < NUM_LOCKS; i++) {
	    rwl_create_lock(&((known_locks[i])->lock_handle),
			    lock_debug_info[i]);
	}
	
	locks_inited = true;
    }
}

PRIVATE lock_pvt_t *get_lock_info
#ifndef __STDC__
    ( lock_type )
    lock_type_t  lock_type;
#else
  (
    lock_type_t  lock_type
  )
#endif
{
    lock_pvt_t  *lock_p;

    if (!locks_inited)
        lock_init();

    switch (lock_type) {
        case lock_db:
            lock_p = &db;
            break;

        case lock_replist:
            lock_p = &replist;
            break;

        case lock_log:
            lock_p = &log;
            break;

        case lock_kdc:
            lock_p = &kdc;
            break;

        case lock_master_info:
            lock_p = &master_info;
            break;

        case lock_login_info:
            lock_p = &login;
            break;
    }

    return lock_p;
}


/*
 * PUBLIC ROUTINES
 */


PUBLIC void lock
#ifndef __STDC__
    ( lock_type, lock_mode )
    lock_type_t  lock_type;
    lock_mode_t  lock_mode;
#else
  (
    lock_type_t  lock_type,
    lock_mode_t  lock_mode
  )
#endif
{
    lock_pvt_t  *lock_p;

    lock_p = get_lock_info(lock_type);

    switch (lock_mode) {

    case lock_mode_read:
	rwl_read_lock(lock_p->lock_handle);
	break;

    case lock_mode_riw:
	rwl_riw_lock(lock_p->lock_handle);
	break;

    case lock_mode_write:
	rwl_write_lock(lock_p->lock_handle);
	break;
    }

}


PUBLIC void lock_promote
#ifndef __STDC__
    ( lock_type )
    lock_type_t lock_type;
#else
  (
    lock_type_t lock_type
  )
#endif
{
    lock_pvt_t  *lock_p;

    lock_p = get_lock_info(lock_type);

    rwl_promote_lock(lock_p->lock_handle);

}


PUBLIC void lock_demote
#ifndef __STDC__
    ( lock_type )
    lock_type_t  lock_type;
#else
  (
    lock_type_t  lock_type
  )
#endif
{
    lock_pvt_t  *lock_p;

    lock_p = get_lock_info(lock_type);

    rwl_demote_lock(lock_p->lock_handle);
}


PUBLIC void lock_global
#ifndef __STDC__
    ( lock_mode )
    lock_mode_t  lock_mode;
#else
  (
    lock_mode_t  lock_mode
  )
#endif
{
    lock(lock_replist, lock_mode);
    lock(lock_db, lock_mode);
}


PUBLIC void real_unlock
#ifndef __STDC__
    ( lock_type )
    lock_type_t  lock_type;
#else
  (
    lock_type_t  lock_type
  )
#endif
{
    lock_pvt_t  *lock_p;

    lock_p = get_lock_info(lock_type);

    rwl_unlock(lock_p->lock_handle);

}


PUBLIC void real_unlock_global (
#ifdef __STDC__
    void
#endif
)
{
    real_unlock(lock_db);
    real_unlock(lock_replist);
}



PUBLIC void lock_promote_global (
#ifdef __STDC__
    void
#endif
)
{
    lock_promote(lock_replist);
    lock_promote(lock_db);
}


PUBLIC void lock_demote_global (
#ifdef __STDC__
    void
#endif
)
{
    lock_demote(lock_replist);
    lock_demote(lock_db);
}


PUBLIC void unlock_all (
#ifdef __STDC__
    void
#endif
)
{
   return;   /* NOT IMPLEMENTED */
}

PRIVATE char *type_strings[NUM_LOCKS] = { "database",
					  "replica list",
					  "log",
                                          "KDC",
					  "master info",
					  "login info"
					 };

PUBLIC char *type_string
#ifndef __STDC__
    ( type )
    lock_type_t  type;
#else
  (
    lock_type_t  type
  )
#endif
{
    return type_strings[type];
}

#define NUM_MODES 3
PRIVATE char *mode_strings[NUM_MODES] = { "read",
					  "write",
					  "read-intend-write"
					 };

PUBLIC char *mode_string
#ifndef __STDC__
    ( mode )
    lock_mode_t  mode;
#else
  (
    lock_mode_t  mode
  )
#endif
{
    return mode_strings[mode];
}
