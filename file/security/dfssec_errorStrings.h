/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfssec_errorStrings.h,v $
 * Revision 1.1.9.1  1996/10/02  20:59:21  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:38  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:19:55  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:39  annie]
 * 
 * Revision 1.1.2.3  1993/01/21  15:19:56  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:58:39  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  19:40:55  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:24:57  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:53:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dce/nbase.h>
#include <dce/rpc.h>

IMPORT char * RgySec_ErrorString _TAKES((
					 error_status_t *	secErrorP
				       ));

IMPORT void Print_DCE_Error _TAKES((
				    FILE *		stream,
				    char *		programName,
				    char *		routineName,
				    error_status_t *	routineErrorStatusP
				  ));

IMPORT char * SysErrString _TAKES((
				   int	errNumber
				 ));

IMPORT char * Rpc_Authn_Service_Strings _TAKES((
						long	authnService
					      ));

IMPORT char * Rpc_Authz_Service_Strings _TAKES((
						long	authzService
					      ));


