/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_log.h,v $
 * Revision 1.1.6.2  1996/02/18  23:00:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:47  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:28:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:51  root]
 * 
 * Revision 1.1.3.4  1992/12/29  13:26:11  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:28  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:10:56  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:47:09  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  22:30:35  emartin
 * 	Fix harmless mistyped parameter.
 * 
 * Revision 1.1.3.2  1992/10/07  20:28:42  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:13:05  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:46:30  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1  1992/01/19  14:42:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - pgo data structure utilities
 */

#ifndef rsdb_log__included
#define rsdb_log__included


#include <dce/nbase.h>

void rsdb_log_create (
#ifdef __STDC__
    Pointer             lbp,
    unsigned32          blen,
    error_status_t      *st
#endif
);

int rsdb_log_open_logfile (
#ifdef __STDC__
    char        *fname,
    int         mode
#endif
);

void rsdb_log_open (
#ifdef __STDC__
    void
#endif
);

void rsdb_log_close (
#ifdef __STDC__
    void
#endif
);

#endif

