/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsSpoke_client.h,v $
 * Revision 1.1.6.1  1996/10/17  18:28:26  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:16  damon]
 *
 * $EndLog$
 */
/*
 * ID: $Id: dfsSpoke_client.h,v 1.1.6.1 1996/10/17 18:28:26 damon Exp $
 *
 * COMPONENT_NAME: Distributed ITL Development Infrastructure
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1993
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
#ifndef DFS_SPOKE_CLIENT_H
#define DFS_SPOKE_CLIENT_H

#include <dce/rpc.h>
#include <utils/itl/itl_stds.h>

extern unsigned32 dfsSpokeClient_Stat _TAKES((
					      IN handle_t h,
					      IN char *f,
					      INOUT struct stat	*stat_buf
					      ));

extern unsigned32 dfsSpokeClient_Readdir _TAKES((
						 IN handle_t h,
						 IN char * f,
						 INOUT int * n_entries,
						 OUT struct dirent ** dir_entries
						 ));

#endif  /* DFS_SPOKE_CLIENT_H */
