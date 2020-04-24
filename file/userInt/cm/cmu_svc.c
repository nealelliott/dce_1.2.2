/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmu_svc.c,v $
 * Revision 1.1.7.1  1996/10/02  21:23:40  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:27  damon]
 *
 * Revision 1.1.2.1  1994/08/08  17:05:17  sasala
 * 	Implement partial S12Y (Internationalization) for cm command, including general Makefile changes
 * 	[1994/08/08  15:30:04  sasala]
 * 
 * $EndLog$
 */

/*
 * This module declares the DFS cm (cmu) handle and
 * registers the table with a call to the OSF supplied macro
 * DCE_SVC_DEFINE_HANDLE.
 *
 * DFS s12y is done in a manner to minimize the amount of messages
 * that need be present for any given component. As such, each
 * directory where s12y is done contains a sams file for that code.
 *
 * In order to register the messages needed by a given function,
 * each of the separate message tables used needs to be registered.
 *
 * This module also registers the tables for the components used in
 * its invocation.
 */

#include <stdio.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>

#include <dfscmumac.h>
#include <dfscmusvc.h>
#include <dfscmumsg.h>

DCE_SVC_DEFINE_HANDLE(cmu_svc_handle,  /* The cm handle (to the cmu table) */
                      cmu_svc_table,   /* The cmu table itself */
                      "cmu")

void dfs_define_cmu_message_tables()
        {
          error_status_t st ;

          /* register cmu messages       */
          dce_msg_define_msg_table(cmu__table,  /* The cm cmu  msg table)         */
                                   sizeof cmu__table / sizeof cmu__table[0], /* number */
                                   &st);
          if (st != svc_s_ok)
             fprintf(stderr, "Cannot register cmu incore message table 0x%lx\n", st);

        }
