/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_ch_lock.h,v $
 * Revision 1.1.8.2  1996/02/18  22:58:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:26  marty]
 *
 * Revision 1.1.8.1  1995/12/13  16:25:03  root
 * 	Submit
 * 	[1995/12/11  15:14:38  root]
 * 
 * Revision 1.1.6.2  1994/08/04  16:12:44  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:09:25  mdf]
 * 
 * Revision 1.1.2.5  1992/12/29  13:07:44  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:05  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:10:13  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:45:42  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:06:21  emartin
 * 	bl6c: do BEGIN_READ_EVENT
 * 
 * Revision 1.1.2.3  1992/10/07  20:25:26  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:11  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:34:15  burati
 * 	 Second replication code drop: bl5
 * 	do BEGIN/END_RIW_EVENT, BEGIN/END_WRITE_EVENT, BEGIN/END_GLOBAL_RIW_EVENT
 * 
 * Revision 1.1.2.2  1992/08/31  18:17:29  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  16:36:02  sekhar]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */

/*
 *      Entry point check_and_lock macros 
 */

#ifndef rs_ch_lock__included
#define rs_ch_lock__included

#include <rs_lock.h>
#include <rs_rep_util.h>


#define UNLOCK_IF_LOCKED \
    } CATCH(pthread_cancel_e) { \
        if (lock_taken) \
            unlock(save_lock_type, save_lock_mode); \
        RERAISE; \
    } CATCH_ALL { \
        dce_svc_printf(SEC_RS_LOCK_FATAL_EXC_MSG,     \
                       mode_string(save_lock_mode),   \
                       type_string(save_lock_type));  \
    } ENDTRY; \
        if (lock_taken) \
            unlock(save_lock_type, save_lock_mode); \
    }

/* 
 * Lock the specified database for a read operation.
 * Perform various state checks for the "event" which is about
 * to be performed and return a bad status if the replica is 
 * in a bad state. otherwise take a read lock on the specified 
 * database.
 */ 
#define BEGIN_READ_EVENT(event_type, lock_type, lock_status) \
    { \
    lock_type_t save_lock_type = (lock_type); \
    lock_mode_t save_lock_mode = lock_mode_read; \
    boolean32   lock_taken = false; \
    TRY { \
        rs_rep_util_enable_event((event_type), (lock_type), lock_mode_read, \
            (NULL), &lock_taken, (lock_status)); \
        if (GOOD_STATUS((lock_status))) \

#define END_READ_EVENT  UNLOCK_IF_LOCKED

/* 
 * Lock a database for a read operation.  Perform various
 * state checks and return a bad status if the replica is in a bad 
 * state. otherwise take a read lock on the specified database.
 */ 
#define BEGIN_READ(lock_type, lock_status) \
    BEGIN_READ_EVENT(rs_c_event_read, (lock_type), (lock_status))

#define END_READ     UNLOCK_IF_LOCKED

/*
 * Lock the specified database for read intend to write. 
 * Perform various state checks for the "event" which is about
 * to be performed and return a bad status if the replica is 
 * in a bad state. otherwise take a riw lock on the specified 
 * database.
 */ 
#define BEGIN_RIW_EVENT(event_type, lock_type, lock_status) \
    { \
    lock_type_t save_lock_type = (lock_type); \
    lock_mode_t save_lock_mode = lock_mode_riw; \
    boolean32   lock_taken = false; \
    TRY { \
        rs_rep_util_enable_event((event_type), (lock_type), lock_mode_riw, \
            (NULL), &lock_taken, (lock_status)); \
        if (GOOD_STATUS((lock_status))) \

#define END_RIW_EVENT   UNLOCK_IF_LOCKED

#define RIW_PROMOTE(lock_type) \
    READ_INTEND_WRITE_PROMOTE(lock_type)

#define RIW_DEMOTE(lock_type) \
    READ_INTEND_WRITE_DEMOTE(lock_type)

#define BEGIN_MASTER_RIW(lock_type, lock_status) \
    BEGIN_RIW_EVENT((rs_c_event_master_write), (lock_type), (lock_status))

#define END_MASTER_RIW  END_RIW_EVENT

/* 
 * Lock the specified database for an update.
 * Perform various state checks for the "event" which is about
 * to be performed and return a bad status if the replica is 
 * in a bad state. otherwise take a write lock on the specified 
 * database.
 *
 * If this replica thinks it is the master and it receives
 * a propagation from another master, it checks whether
 * it should go into obsolete master state.
 */ 
#define BEGIN_SLAVE_WRITE_EVENT(event_type, lock_type, master_inf, lock_status) \
    { \
    lock_type_t save_lock_type = (lock_type); \
    lock_mode_t save_lock_mode = lock_mode_write; \
    boolean32   lock_taken = false; \
    TRY { \
        rs_rep_util_enable_event((event_type), (lock_type), lock_mode_write, \
            (master_inf), &lock_taken, (lock_status)); \
        if (GOOD_STATUS((lock_status))) \

#define END_SLAVE_WRITE_EVENT UNLOCK_IF_LOCKED


#define GLOBAL_UNLOCK_IF_LOCKED \
    } CATCH(pthread_cancel_e) { \
        if (lock_taken) \
            unlock_global(save_lock_mode); \
        RERAISE; \
    } CATCH_ALL { \
        dce_svc_printf(SEC_RS_GLOBAL_LOCK_FATAL_EXC_MSG,    \
                       mode_string(save_lock_mode));        \
    } ENDTRY; \
        if (lock_taken) \
            unlock_global(save_lock_mode); \
    }

/* 
 * Lock the database for global read-intend-write. 
 * Perform various state checks for the "event" which is about
 * to be performed and return a bad status if the replica is 
 * in a bad state. otherwise take a global riw lock on the 
 * database.
 */ 
#define BEGIN_GLOBAL_RIW_EVENT(event_type, lock_status) \
    { \
    lock_mode_t save_lock_mode = lock_mode_riw; \
    boolean32   lock_taken = false; \
    TRY { \
        rs_rep_util_enable_global_event((event_type), lock_mode_riw, \
            (NULL), &lock_taken, (lock_status)); \
        if (GOOD_STATUS((lock_status))) \

#define END_GLOBAL_RIW_EVENT    GLOBAL_UNLOCK_IF_LOCKED

/* 
 * Lock the database for global read-intend-write at a slave.
 * Perform various state checks for the "event" which is about
 * to be performed and return a bad status if the replica is 
 * in a bad state. otherwise take a global riw lock on the 
 * database.
 *
 * If this replica thinks it is the master and it receives
 * a propagation from another master, it checks whether
 * it should go into obsolete master state.
 */ 
#define BEGIN_GLOBAL_RIW_SLAVE_EVENT(event_type, master_inf, lock_status) \
    { \
    lock_mode_t save_lock_mode = lock_mode_riw; \
    boolean32   lock_taken = false; \
    TRY { \
        rs_rep_util_enable_global_event((event_type), lock_mode_riw, \
            (master_inf), &lock_taken, (lock_status)); \
        if (GOOD_STATUS((lock_status))) \

#define END_GLOBAL_RIW_SLAVE_EVENT    GLOBAL_UNLOCK_IF_LOCKED

#endif


