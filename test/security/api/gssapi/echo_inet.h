/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: echo_inet.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:14  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:19:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:51:37  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:45:19  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:45  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/09  15:51:06  wray
 * 	Initial submission (OT#10019)
 * 	[1994/03/09  15:40:27  wray]
 * 
 * $EndLog$
 */
/*
 * The Echo example program is derived from code found in
 * W. Richard Stevens UNIX Network Programming, copyright 1990, reprinted
 * by permission of Prentice-Hall, Inc., Englewood Cliffs, New Jersey.
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define DEFAULT_ECHO_PORT "6000"
#define SERVICE_SECURE 2
#define SERVICE_INSECURE 1
