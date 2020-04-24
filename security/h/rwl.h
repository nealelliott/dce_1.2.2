/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rwl.h,v $
 * Revision 1.1.4.2  1996/02/18  23:00:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:28:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:04  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:27:03  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:46  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rwl.h V=7 07/15/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *      read/write lock manager
 */

#ifndef rwl_h__included
#define rwl_h__included

#include <dce/nbase.h>

#include <dce/exc_handling.h>

/* 
* Global Variables
*/
 
extern boolean32 rwl_lock_debug;  /* initially false */

 
/* End Global Variables */


/*
 * Exceptions
 */

/* The first five exceptions may be raised by any lock call */
EXCEPTION rwl_e_fatal;            /* fatal internal error in lock module */
EXCEPTION rwl_e_init;             /* lock module has not been initialized */
EXCEPTION rwl_e_init_in_progress; /* lock module initialization in progress */
EXCEPTION rwl_e_mem;              /* insufficient memory - permanent conditon  */
EXCEPTION rwl_e_quota;            /* internal quota exceeded - temporary conditon */
EXCEPTION rwl_e_existence;        /* lock object does not exist */
EXCEPTION rwl_e_owner;            /* must own lock to perform requested operation */
EXCEPTION rwl_e_use;              /* invalid use of lock object */
EXCEPTION rwl_e_reuse;            /* requestor already owns the lock */
 
/* NOTE: the rwl_e_reuse error is not currently detected for read locks, as this
 * would require maintaining a list of threads holding read locks.
 */


/* all the fields in the lock handle structure are private and should be
 * treated as such by the application.  We use a structure rather than
 * a generic pointer because it makes for a slightly more robust 
 * implementation.
 */
typedef struct rwl_handle_t {
    unsigned  long        magic;        /* corruption guard */
    unsigned  long        lock_id;      /* private lock identifier */
    struct rwl_lock_info  *lock_info;   /* handle on private lock info */
} rwl_handle_t;

/* must be called once by application before any lock module operations 
 * are attempted.
 */
void rwl_module_init (
#ifdef __STDC__ 
   void
#endif
);

/* NOTE:  synchronicity of  lock create/delete operations is still sort of
 * flaky, so lock objects should only be created/deleted by a "distinguished"
 * task.  debug_info is a text string that is printed as part of the lock info
 * when rwl_lock_debug is true.
 */
void rwl_create_lock (
#ifdef __STDC__ 
   rwl_handle_t  *lock,
   char          *debug_info
#endif
);

void rwl_delete_lock (
#ifdef __STDC__
    rwl_handle_t  *lock
#endif
);

/* takes a read lock - allow concurrent read and riw, but no write locks */
void rwl_read_lock ( 
#ifdef __STDC__
    rwl_handle_t  lock
#endif
);

/* take a read-intend-write lock - allow concurrent read, but no riw or write
 * locks.  A single thread should take either a read or riw write lock, but
 * not both (a thread holding both will deadlock if it issues a promote request).
 */
void rwl_riw_lock (
#ifdef __STDC__
    rwl_handle_t  lock
#endif
);

/* take an exclusive write lock
 * 
 */
void rwl_write_lock (
#ifdef __STDC__
    rwl_handle_t  lock
#endif
);

/* promote a read-intend-write lock to a write lock.
 * 
 * Exceptions:
 *     rwl_e_use    lock is not a read-intend-write lock
 *     rwl_e_owner  requestor does not own the read-intend-write lock
 */
void rwl_promote_lock (
#ifdef __STDC__
    rwl_handle_t  lock
#endif
);


/* demote a write lock to a read-intend-write lock.
 *
 * Exceptions
 *     rwl_e_use    lock is not a write lock
 *     rwl_e_owner  requestor does not own the write lock
 */
void rwl_demote_lock (
#ifdef __STDC__
    rwl_handle_t  lock
#endif
);

/* give up a lock
 *
 * Exceptions
 *     rwl_e_use    lock is not locked
 *     rwl_e_owner  requestor does not own the lock (currently checked only
 *                    for write and read-intend-write locks)
 */
void rwl_unlock ( 
#ifdef __STDC__
    rwl_handle_t lock
#endif
); 

#endif

