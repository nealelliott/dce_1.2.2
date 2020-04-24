/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_afsconf.h,v $
 * Revision 1.1.12.1  1996/10/02  17:04:24  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:55  damon]
 *
 * Revision 1.1.6.1  1994/06/09  13:52:17  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:26:02  annie]
 * 
 * Revision 1.1.4.3  1993/01/18  19:55:59  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:32:16  cjd]
 * 
 * Revision 1.1.4.2  1992/11/24  15:41:00  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:08:56  bolinger]
 * 
 * 	Merging dfs5.5 to dfs5.11
 * 
 * Revision 1.1.2.2  1992/01/24  01:48:46  devsrc
 * 	Bring in dfs6.3 sources
 * 	[1992/01/22  19:30:32  devsrc]
 * 
 * Revision 1.1.4.2  91/12/19  12:55:38  devsrc
 * 	Merging dfs5.11 to dfs6.3
 * 
 * Revision 1.1.2.2  91/08/21  13:21:39  garyf
 * 	fix bad function prototype
 * 	[91/08/21  13:19:02  garyf]
 * 
 * $EndLog$
*/
/*
 *	bossvr_afsconf.h -- prototypes of routines to roughly approximate some of the afsconf
 * routines for use in bosserver testing (until the afsconf layer works for AFS 4)
 *
 *
 *	Copyright (C) 1991 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/cellconfig.h>
#include <dcedfs/keys.h>

#include <dce/rpc.h>

/* bosserver super-user info */
IMPORT int afsconf_AddUser _TAKES((
				   struct afsconf_dir *	bossvr_confdir,
				   char *		name
				 ));
IMPORT int afsconf_DeleteUser _TAKES((
				      struct afsconf_dir *	bossvr_confdir,
				      char *			name
				     ));
IMPORT int afsconf_GetNthUser _TAKES((
				      struct afsconf_dir *	bossvr_confdir,
				      int			number,
				      char *			nameP,
				      int			length
				    ));
EXPORT int afsconf_SuperUser _TAKES((
				     struct afsconf_dir *	bossvr_confdir,
				     rpc_binding_handle_t *	bosserverBindingP
				   ));
/* key info */
IMPORT int afsconf_AddKey _TAKES((
				  struct afsconf_dir *	bossvr_confdir,
				  long			kvno,
				  char			key[8]
				));
IMPORT int afsconf_DeleteKey _TAKES((
				     struct afsconf_dir *	bossvr_confdir,
				     long			kvno
				   ));
IMPORT int afsconf_GetKeys _TAKES((
				   struct afsconf_dir *		bossvr_confdir,
				   struct afsconf_keys *	keys
				 ));

/* cell info */
IMPORT int afsconf_GetCellInfo _TAKES((
				       struct afsconf_dir *	bossvr_confdir,
				       char *			cellName,
				       char *			service,
				       struct afsconf_cell *	cellInfoP
				     ));
IMPORT int afsconf_GetLocalCell _TAKES((
					struct afsconf_dir *	bossvr_confdir,
					char *			cellNameP,
					int			length
				      ));
IMPORT int afsconf_GetNoAuthFlag _TAKES((
					 struct afsconf_dir *	bossvr_confdir
				       ));
IMPORT int afsconf_SetCellInfo _TAKES((
				       char *			path,
				       struct afsconf_cell *	cellInfoP
				     ));
IMPORT int afsconf_SetNoAuthFlag _TAKES((
					 struct afsconf_dir *	bossvr_confdir,
					 long			flag
				       ));
				  
/* and an extra one just for kicks */
IMPORT int ka_KeyCheckSum _TAKES((
				  char *		theKey,
				  unsigned long *	theCheckSum
				));

