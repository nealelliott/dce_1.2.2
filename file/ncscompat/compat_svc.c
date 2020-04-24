/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY
 * $Log: compat_svc.c,v $
 * Revision 1.1.7.1  1996/10/02  17:54:46  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:23  damon]
 *
 * Revision 1.1.2.1  1994/08/17  15:35:17  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands a daemons
 * 	[1994/08/17  15:34:30  ruby]
 * 
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/04  14:59:53  ruby]
 * 
 * $EndLog$
 */
/*
 * compat_svc.c -- servicability initialization
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#include <stdio.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsncsmac.h>
#include <dfsncssvc.h>
#include <dfsncsmsg.h>

DCE_SVC_DEFINE_HANDLE(ncs_svc_handle, ncs_svc_table, "ncs")

int  compat_svc_must_init = 1;

void compat_svc_init()

{
  if (compat_svc_must_init)

    {
      error_status_t st;

      compat_svc_must_init = 0;

      dce_msg_define_msg_table(ncs__table,
			       sizeof ncs__table / sizeof ncs__table[0],
			       &st);

      if (st != svc_s_ok)

	fprintf(stderr,
		"Warning Cannot register ncs__table incore message ncs__table 0x%lx\n",
		ncs__table,
		st);
    }
}
