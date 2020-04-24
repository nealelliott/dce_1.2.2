/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: refmon.h,v $
 * Revision 1.1.2.2  1996/03/09  20:49:43  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:03  marty]
 *
 * Revision 1.1.2.1  1995/12/11  19:35:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:18:40  root]
 * 
 * $EndLog$
 */
/*
 */
#define DENY_ACCESS 0
#define GRANT_ACCESS 1

#define AUTH_OK 0
#define NO_AUTH 1
#define LOW_INTEGRITY 2

int reference_monitor(rpc_binding_handle_t, long *);
