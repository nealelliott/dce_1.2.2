/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_commonSockaddr.h --
 *    Declarations for using the DFS file id data type.
 *
 *    This module stores information about a DFS file id.
 *    This is used to uniquely identify files with the DFS name space.
 *
 *    The initialization routine must be called prior to using this
 *    module.  File id's are added via the Create function.  They  are
 *    never removed.  Once a file id is created it is accessed via a call
 *    to the Get function.
 */
/*
 * HISTORY
 * $Log: itl_commonSockaddr.h,v $
 * Revision 1.1.320.1  1996/10/17  18:25:32  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:51  damon]
 *
 * Revision 1.1.315.1  1994/02/04  20:43:29  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:41  devsrc]
 * 
 * Revision 1.1.313.2  1994/01/20  18:44:01  annie
 * 	added copyright header
 * 	[1994/01/20  18:40:20  annie]
 * 
 * Revision 1.1.313.1  1993/12/07  17:43:29  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:09:13  jaffe]
 * 
 * $EndLog$
 */

#ifndef ITL_COMMON_SOCKADDR_H
#define ITL_COMMON_SOCKADDR_H

#include <sys/socket.h>
#include <netinet/in.h>

extern int itlCommon_sockaddrDataType;

/**
 **  Function declarations
 **/

extern void itlCommon_SockaddrInit _TAKES((
					   void
					   ));

extern struct sockaddr *itlCommon_SockaddrGet _TAKES((
						      void
						      ));

extern var_val_t *itlCommon_SockaddrCreate _TAKES((
						   IN struct sockaddr *sockaddrP
						   ));

#endif /* ITL_COMMON_SOCKADDR_H */

