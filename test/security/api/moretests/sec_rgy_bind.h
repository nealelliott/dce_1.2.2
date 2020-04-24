/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_rgy_bind.h,v $
 * Revision 1.1.8.3  1996/02/18  23:07:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:32  marty]
 *
 * Revision 1.1.8.2  1995/12/11  21:50:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:03  root]
 * 
 * Revision 1.1.3.2  1994/10/27  20:07:39  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:40:00  rrizzo]
 * 
 * Revision 1.1.3.1  1994/10/25  17:18:05  sekhar
 * 	[ OT 12398 ]  Checking in Sean Mullan's changes for testing
 * 		      security replication.
 * 	[1994/10/24  17:01:52  sekhar]
 * 
 * 	HP revision /main/mullan_mothra_test_repl_cov/1  1994/10/05  20:34 UTC  mullan_s
 * 	Common macros for binding to security replicas.
 * 
 * $EndLog$
 */

/*  sec_rgy_bind.h
**
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
**
*/

#include <dce/binding.h>
#include <dce/repadm.h>

#include <stdlib.h>

int                     test_replica;
char                    *sec_test_replication = NULL;
unsigned_char_t         *sec_replica_site_name;
error_status_t          rgy_st;
unsigned_char_t         *sec_master_site_name;

/*
 * SEC_RGY_SITE_OPEN_READ returns a registry handle to a security
 * server which will be used to read data from. If test_replica is TRUE,
 * the binding handle points to a replica, otherwise it points to
 * the master.
 */
#define SEC_RGY_SITE_OPEN_READ(CONTEXT_P)                                       \
    sec_test_replication = getenv("SEC_TEST_REPLICATION");                      \
    if ((sec_test_replication != NULL) &&                                       \
        ((*sec_test_replication == 'T') ||                                      \
         (*sec_test_replication == 't'))) {                                     \
       test_replica = TRUE;                                                     \
       sec_replica_site_name = getenv("SEC_REPLICA_SITE_NAME");	                \
    } else                                                                      \
       test_replica = FALSE;                                                    \
    if (test_replica) {                                                         \
        rs_replica_info_t  rep_info;                                            \
        sec_rgy_site_open_query(sec_replica_site_name, CONTEXT_P, &rgy_st);     \
        TSHMessage(ctx, MOTRACE, "sec_rgy_site_open_query status: %d\n",        \
                   rgy_st);							\
        if (TSHStatus(ctx, SETUP, "sec_rgy_site_open_query failed\n",		\
                      error_status_ok, rgy_st))					\
            return(rgy_st);							\
        sec_rgy_rep_admin_info(*(CONTEXT_P), &rep_info, &rgy_st);               \
        TSHMessage(ctx, MOTRACE, "sec_rgy_rep_admin_info status: %d\n",		\
                   rgy_st);							\
        if (TSHStatus(ctx, SETUP, "sec_rgy_rep_admin_info failed\n",		\
                      error_status_ok, rgy_st))					\
            return(rgy_st);							\
        if (rep_info.master) {                                                  \
            TSHMessage(ctx, MOTRACE, "FAILED: SEC_REPLICA_SITE_NAME is the",	\
                       "master replica, it should be read-only!");		\
            return;                                                             \
        }                                                                       \
    } else {                                                                    \
        sec_rgy_site_open_update(sec_master_site_name, CONTEXT_P, &rgy_st);     \
        TSHMessage(ctx, MOTRACE, "sec_rgy_site_open_update status: %d\n",	\
                   rgy_st);							\
        if (TSHStatus(ctx, SETUP, "sec_rgy_site_open_update failed\n",		\
                      error_status_ok, rgy_st))					\
            return(rgy_st);							\
        sec_replica_site_name = sec_master_site_name;                           \
    }

/*
 * SEC_RGY_SITE_OPEN_WRITE - returns a binding handle to the master
 * security server.
 */
#define SEC_RGY_SITE_OPEN_WRITE(CONTEXT_P)                                      \
    sec_rgy_site_open_update(sec_master_site_name, CONTEXT_P, &rgy_st);         \
    TSHMessage(ctx, MOTRACE, "sec_rgy_site_open_update status: %d\n",           \
               rgy_st);                                                         \
    if (TSHStatus(ctx, SETUP, "sec_rgy_site_open_update failed\n",              \
                  error_status_ok, rgy_st))                                     \
        return(rgy_st);                                                         \

/*
 * SEC_RGY_SITE_SYNCH - blocks until prior updates have been propagated
 * to the replica. If test_replica is FALSE, this macro does nothing.
 *
 * NOTE!: If the call to sec_rgy_wait_until_consistent fails, it is not
 * considered fatal. A warning is printed. If the caller considers it to
 * be fatal, then the caller should check rgy_st. Incidentally, if 
 * sec_rgy_wait_until_consistent does fail, something else is bound to
 * fail down the line.
 *
 */
#define SEC_RGY_SITE_SYNCH(CONTEXT) 		                                \
    if (test_replica) {                                                         \
        if (!(sec_rgy_wait_until_consistent(CONTEXT, &rgy_st))) {               \
            TSHMessage(ctx, MOFAILURE,						\
                       "sec_rgy_wait_until_consistent status: %d\n", rgy_st);	\
        } else if (rgy_st != error_status_ok)                                   \
            TSHMessage(ctx, MOFAILURE,                                          \
                       "sec_rgy_wait_until_consistent status: %d\n", rgy_st);   \
    }
