/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: auto_spoke.h,v $
 * Revision 1.1.112.1  1996/10/17  18:28:17  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:11  damon]
 *
 * Revision 1.1.107.1  1994/02/04  20:46:10  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:55  devsrc]
 * 
 * Revision 1.1.105.1  1993/12/07  17:45:15  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  22:00:40  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: auto_spoke.h,v 1.1.112.1 1996/10/17 18:28:17 damon Exp $
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

#ifndef ITL_AUTO_SPOKE_H
#define ITL_AUTO_SPOKE_H

#include <utils/itl/itl_stds.h>

extern int itl_dce_login _TAKES((
				 IN char *princName,
				 IN char *princPasswd
				 ));

extern int spoke_AutoSpoke _TAKES((
				   IN char *fileName,
				   IN char *listenName
				   ));

extern void spoke_syncHubSpoke _TAKES((
				       void
				       ));

extern int spoke_RedirectOutput _TAKES((
					IN char *fileName
					));

extern void auto_spokeCleanup _TAKES((
				      IN int nspoke
				      ));


#endif  /* ITL_AUTO_SPOKE_H */
