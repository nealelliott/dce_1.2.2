/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_spoke.h,v $
 * Revision 1.1.115.1  1996/10/17  18:28:36  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:22  damon]
 *
 * Revision 1.1.110.1  1994/02/04  20:46:17  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:57  devsrc]
 * 
 * Revision 1.1.108.1  1993/12/07  17:45:21  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  22:00:54  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: itl_spoke.h,v 1.1.115.1 1996/10/17 18:28:36 damon Exp $
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

#ifndef ITL_SPOKE_H
#define ITL_SPOKE_H

#include <dce/rpc.h>
#include <utils/itl/itl_stds.h>

extern int nspokes;

extern handle_t itlSpoke_GetHandle _TAKES((
					   void
					   ));
extern handle_t itlSpoke_GetHandleByString _TAKES((
					   char *
					   ));

/*
 *  itlSpoke_ForAllClients() --
 *    This function will apply a caller supplied function against all
 *    known clients.
 *
 * ARGUMENTS:
 *  func - this function will be called with the rpc handle, an info data
 *         string (currently the string binding), and the user data.
 *  dataP - a pointer to any data you like.
 */
extern boolean_t itlSpoke_ForallClients _TAKES((
				      IN boolean_t (*func)(
				                           IN handle_t rpcHandle,
                                                           IN pointer_t infoP,
							   IN pointer_t userDataP),
				      pointer_t dataP));

extern void itlSpoke_AddCmds _TAKES((
				     void
				     ));

#endif  /* ITL_SPOKE_H */
