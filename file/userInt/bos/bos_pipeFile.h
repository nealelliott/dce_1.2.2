/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bos_pipeFile.h,v $
 * Revision 1.1.9.1  1996/10/02  21:06:16  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:18  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:23:58  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:35:55  annie]
 * 
 * Revision 1.1.2.3  1993/01/21  16:28:40  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:21:56  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  19:50:32  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:35:29  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:49:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bos_pipeFile.h -- prototypes for routines that deal with the client end of NCS pipes
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */


#ifndef TRANSARC_BOS_PIPEFILE_H
#define TRANSARC_BOS_PIPEFILE_H

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/bbos_ncs_interface.h>

IMPORT void FilePipeAlloc _TAKES((
				  void *		state,
				  ndr_ulong_int		bsize,
				  ndr_byte **		buf,
				  ndr_ulong_int *	bcount
				));

IMPORT void FilePipePull _TAKES((
				 void *			state,
				 ndr_byte *		buf,
				 ndr_ulong_int		esize,
				 ndr_ulong_int *	ecount
			       ));

IMPORT void FilePipePush _TAKES((
				 void *			state,
				 ndr_byte *		buf,
				 ndr_ulong_int		ecount
			       ));

#endif /* TRANSARC_BOS_PIPEFILE_H */
