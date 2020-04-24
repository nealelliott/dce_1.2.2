/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: lsv_svc.c,v $
 * Revision 1.1.9.1  1996/10/02  17:46:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:37:14  damon]
 *
 * Revision 1.1.2.1  1994/10/26  15:46:07  sasala
 * 	UHYP: CR 12618 Implement S12Y in DFS flserver
 * 	[1994/10/26  15:43:48  sasala]
 * 
 * $EndLog$
 */

/*
 * This module declares the DFS flserver (lsv) handle and
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

#include <dfslsvmac.h>
#include <dfslsvsvc.h>
#include <dfslsvmsg.h>

DCE_SVC_DEFINE_HANDLE(lsv_svc_handle,  /* The flserver handle (to the lsv table) */
                      lsv_svc_table,   /* The lsv table itself */
                      "lsv")

void dfs_define_lsv_message_tables()
        {
          error_status_t st ;

          /* register lsv messages       */
          dce_msg_define_msg_table(lsv__table,  /* The flserver lsv  msg table)        */
                                   sizeof lsv__table / sizeof lsv__table[0], /* number */
                                   &st);
          if (st != svc_s_ok)
             fprintf(stderr, "Cannot register lsv incore message table 0x%lx\n", st);

        }
