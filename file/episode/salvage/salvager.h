/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: salvager.h,v $
 * Revision 1.1.5.1  1996/10/02  17:27:33  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:04  damon]
 *
 * 	Transarc delta: ota-db3352-diff-COW-and-backing-blockmaps 1.2
 * 	  Selected comments:
 * 	    During block map checking verify that the COW and backing anodes have
 * 	    compatable structures.
 * 	    In the process we revitalize the COWFIXPASS of the salvager which had
 * 	    largely atrophied.  We now do COW fixing on every pass.  However, since
 * 	    we allow arbitrarily long backing chains we may have to repeatedly check
 * 	    for COW block map errors if we modified any block map on the previous
 * 	    pass.  To avoid loops, we only modify the block maps of non-COW files on
 * 	    the first pass (or second pass if duplicates are being resolved).
 * 	    Also cleanup many ReportError calls.
 * 	    Remove gratuitous aggrAnode argument from epis_VerifyAggrPaths.
 * 	    Accidently reversed FRAG and BLOCK reason msgs in epiz_CheckBlock.
 * 	[1993/05/10  17:48:07  jaffe]
 *
 * Revision 1.1.8.2  1994/07/13  22:22:30  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:43:53  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:57:18  mbs]
 * 
 * Revision 1.1.8.1  1994/06/09  14:03:07  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:38:09  annie]
 * 
 * Revision 1.1.2.4  1993/01/19  15:33:25  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:55:27  cjd]
 * 
 * Revision 1.1.2.3  1992/11/24  16:56:24  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:25:51  bolinger]
 * 
 * 	Merging dfs6.3
 * 	[1992/01/24  00:10:19  devsrc]
 * 
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:29:57  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:13:52  devsrc
 * 	Fixed logs
 * 	[1992/01/23  19:44:10  zeliff]
 * 
 * Revision 1.1.2.2  1992/11/18  19:41:49  jaffe
 * 	Transarc delta: cburnett-ot5890-rename-salvage.h-to-salveger.h 1.1
 * 	  Selected comments:
 * 	    Rename salvage/salvage.p.h to salvage/salvager.p.h to avoid having
 * 	    two salvage.h include files.  There is also a salvage.h in the anode dir.
 * 	    new file salvager.p.h
 * 	[1992/11/17  21:19:13  jaffe]
 * 
 * $EndLog$
*/
/* Copyright (C) 1990,1991 Transarc Corporation - All rights reserved */

#ifndef TRANSARC_EPIS_SALVAGER_H
#define TRANSARC_EPIS_SALVAGER_H

#ifndef KERNEL

#include <dcedfs/dir_buffer.h>
#include <name.h>

/* SHARED */
long epis_Walk _TAKES(( 
  IN epi_anode_t volC,
  IN epi_volFileId_t *fid,
  IN struct epiz_salvageState * ss,
  IN struct epis_path * path ));

/* SHARED */
long epis_VerifyAcl _TAKES((
    IN char * buffer,
    IN u_long length,
    IN struct epiz_salvageState * ss,
    IN opaque rock ));

/* SHARED */
long epis_VerifyPlist _TAKES((
    IN char * buffer,
    IN u_long length,
    IN struct epiz_salvageState * ss,
    IN opaque rock ));

/* SHARED */
long epis_VerifyAggrPaths _TAKES((
    IN struct epiz_salvageState * ss ));

/* EXPORT */
long epis_VerifyDevPaths _TAKES(( 
    IN struct async_device *device,
    IN struct epiz_salvageState * ss ));

/* SHARED */
void SetDirHandle _TAKES((
  IN epi_anode_t h,			/* anode */
  OUT struct Dir_Handle * dh));		/* directory handle */

/* SHARED */
void PutDirHandle _TAKES((
  IN struct Dir_Handle * dh));

#endif	/* KERNEL */

#endif /* TRANSARC_EPIS_SALVAGER_H */
