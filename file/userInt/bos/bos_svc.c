/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bos_svc.c,v $
 * Revision 1.1.7.1  1996/10/02  21:06:17  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:19  damon]
 *
 * Revision 1.1.2.1  1994/08/24  17:57:51  sasala
 * 	Implement partial S12Y for DFS - bos
 * 	[1994/08/24  17:43:06  sasala]
 * 
 * $EndLog$
 */

/*
 * This module declares the DFS bos (bos) handle and
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

#include <dfsbosmac.h>
#include <dfsbossvc.h>
#include <dfsbosmsg.h>

DCE_SVC_DEFINE_HANDLE(bos_svc_handle,  /* The bos handle (to the bos table) */
                      bos_svc_table,   /* The bos table itself */
                      "bos")

void dfs_define_bos_message_tables()
        {
          error_status_t st ;

          /* register bos messages       */
          dce_msg_define_msg_table(bos__table,  /* The bos bos  msg table)         */
                                   sizeof bos__table / sizeof bos__table[0], /* number */
                                   &st);
          if (st != svc_s_ok)
             fprintf(stderr, "Cannot register bos incore message table 0x%lx\n", st);

        }
