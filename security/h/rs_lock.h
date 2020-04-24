/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_lock.h,v $
 * Revision 1.1.7.2  1996/02/18  22:58:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:29  marty]
 *
 * Revision 1.1.7.1  1995/12/13  16:25:09  root
 * 	Submit
 * 	[1995/12/11  15:14:41  root]
 * 
 * Revision 1.1.5.2  1994/08/04  16:12:45  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:09:27  mdf]
 * 
 * Revision 1.1.3.3  1992/12/29  13:07:54  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:13  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/07  20:25:37  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:15  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:34:35  burati
 * 	 Second replication code drop: bl5
 * 	define LOCK_GLOBAL, UNLOCK_GLOBAL, GLOBAL_RIW_LOCK, END_GLOBAL_RIW_LOCK,
 * 	GLOBAL_RIW_PROMOTE/DEMOTE
 * 
 * Revision 1.1  1992/01/19  14:43:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 */
/* 
 *      Registry Server - lock manager - macro assist
 */

#ifndef rs_lock__included
#define rs_lock__included

#include <dce/dce.h>
#include <rs_util.h>

#include <dce/rpc.h>
#include <dce/nbase.h>

#include <stdio.h>
#include <dce/exc_handling.h>
#include <sec_svc.h>
/*
 * CMA CD4 currently defines the pthread_cancel_e exception in the wrong
 * set of include files - work around this problem by defining it here if
 * it is missing.... For other implementations of exc_handling.h, we
 * expect that pthread_cancel_e is defined.
 */
#ifdef CMA_INCLUDE
#   ifndef pthread_cancel_e
#       define pthread_cancel_e	cma_e_alerted
#   endif
#endif

/*
 * Global Variables
 */

extern boolean32 rs_lock_debug;  /* initially false */

/* End Global Variables */


typedef enum lock_type_t {
        lock_db, lock_replist, lock_log, lock_kdc, lock_master_info,
	lock_login_info
} lock_type_t;

typedef enum lock_mode_t {
        lock_mode_read, lock_mode_write, lock_mode_riw
} lock_mode_t;

/*
 * Macros for lock management
 */

#define LOCK(lock_type, lock_mode) \
    { \
    lock_type_t save_lock_type = lock_type; \
    lock_mode_t save_lock_mode = lock_mode; \
    lock(lock_type, lock_mode); \
    TRY {
	
#define UNLOCK \
    } CATCH(pthread_cancel_e) { \
        unlock(save_lock_type, save_lock_mode); \
        RERAISE; \
    } CATCH_ALL { \
        dce_svc_printf(SEC_RS_LOCK_FATAL_EXC_MSG,     \
                       mode_string(save_lock_mode),   \
                       type_string(save_lock_type));  \
    } ENDTRY; \
        unlock(save_lock_type, save_lock_mode); \
    }

#define LOCK_GLOBAL(lock_mode) \
    { \
    lock_mode_t save_lock_mode = lock_mode; \
    lock_global(lock_mode); \
    TRY {

#define UNLOCK_GLOBAL \
    } CATCH(pthread_cancel_e) { \
        unlock_global(save_lock_mode); \
        RERAISE; \
    } CATCH_ALL { \
        dce_svc_printf(SEC_RS_GLOBAL_LOCK_FATAL_EXC_MSG,   \
                       mode_string(save_lock_mode));       \
    } ENDTRY; \
        unlock_global(save_lock_mode); \
    }


#define READ_LOCK(type)   LOCK(type, lock_mode_read)

#define END_READ_LOCK UNLOCK

#define WRITE_LOCK(type)  LOCK(type, lock_mode_write)

#define END_WRITE_LOCK  UNLOCK


#define READ_INTEND_WRITE_LOCK(type) \
    LOCK(type, lock_mode_riw)

#define READ_INTEND_WRITE_PROMOTE(type) \
    lock_promote(type)

#define READ_INTEND_WRITE_DEMOTE(type) \
    lock_demote(type)

#define END_READ_INTEND_WRITE_LOCK UNLOCK 


#define GLOBAL_RIW_LOCK     LOCK_GLOBAL(lock_mode_riw)

#define END_GLOBAL_RIW_LOCK UNLOCK_GLOBAL

#define GLOBAL_RIW_PROMOTE \
    lock_promote_global()

#define GLOBAL_RIW_DEMOTE \
    lock_demote_global()


/*
* Procedure Signatures
*/

void lock (
#ifdef __STDC__
    lock_type_t type,
    lock_mode_t mode
#endif
);


void lock_promote (
#ifdef __STDC__
    lock_type_t lock_type
#endif
);

void lock_demote (
#ifdef __STDC__
    lock_type_t lock_type
#endif
);

void lock_global (
#ifdef __STDC__
    lock_mode_t lock_mode
#endif
);

void lock_promote_global(
#ifdef __STDC__
    void
#endif
);

void lock_demote_global(
#ifdef __STDC__
    void
#endif
);

/* the lock mode isn't useful on "unlocks" under the current implementation of
 * locks, but it might be under other implementations (indeed, it was under
 * the original Domain/OS implementation).  We provide wrappers that accept
 * the lock mode so we can alter the lock implementation with minimal
 * impact on the rest of the server code.
 */

#define unlock(type, mode) real_unlock(type)

void real_unlock (
#ifdef __STDC__
    lock_type_t type
#endif
);

#define unlock_global(mode) real_unlock_global()

void real_unlock_global (
#ifdef __STDC__
    void
#endif
);


/* U N L O C K _ A L L
 *
 * Releases all locks currently held by the invoking thread.  It's up to each
 * thread to give up it's locks when terminating.  There is no process-global
 * mechanism for releasing locks.
 */
void unlock_all(
#ifdef __STDC__
    void
#endif
);


char *mode_string (
#ifdef __STDC__
    lock_mode_t  mode
#endif
);

char *type_string (
#ifdef __STDC__
    lock_type_t  type
#endif
);


#endif

