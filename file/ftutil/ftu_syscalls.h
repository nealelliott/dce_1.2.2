/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftu_syscalls.h,v $
 * Revision 1.1.19.1  1996/10/02  17:48:50  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:37  damon]
 *
 * Revision 1.1.14.2  1994/06/09  14:09:19  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:58  annie]
 * 
 * Revision 1.1.14.1  1994/02/04  20:19:50  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:40  devsrc]
 * 
 * Revision 1.1.12.2  1994/01/20  18:43:16  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:31  annie]
 * 
 * Revision 1.1.12.1  1993/12/07  17:25:47  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:06:55  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1995, 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/ftutil/ftu_syscalls.h,v 1.1.19.1 1996/10/02 17:48:50 damon Exp $ */

#ifndef	_TRANSARC_DFS_FTU_SYSCALLS_H
#define	_TRANSARC_DFS_FTU_SYSCALLS_H

#include <dcedfs/ag_init.h>
#include <dcedfs/vol_init.h>

IMPORT long
ftu_AggrSyscall _TAKES((
    int	aggrOp,
    int	p1,
    int	p2,
    int	p3,
    int	p4
));

IMPORT long
ftu_VolSyscall _TAKES((
    int	volOp,
    int	p1,
    int	p2,
    int	p3,
    int	p4
));

/*
 * Aggregate registry ops.
 */
#define AG_ATTACH(astabP, flags, fstabInfo) \
    ftu_AggrSyscall(AGOP_ATTACH, (int)(astabP), (flags), (int)(fstabInfo), 0)
#define AG_DETACH(aggrId, flags) \
    ftu_AggrSyscall(AGOP_DETACH, (aggrId), (flags), 0, 0)
#define	AG_ENUMERATE(bufSize, buf, outLenP) \
    ftu_AggrSyscall(AGOP_AGGENUMERATE, (bufSize), (int)(buf), (int)(outLenP),0)

/*
 * Aggregate ops.
 */
#define AG_INFO(aggrId, aggrInfoP) \
    ftu_AggrSyscall(AGOP_AGGINFO, (aggrId), (int)(aggrInfoP), 0, 0)
#define AG_SYNC(aggrId, syncOp) \
    ftu_AggrSyscall(AGOP_SYNC, (aggrId), (syncOp), 0, 0)
#define AG_VOLCREATE(aggrId, fsetIdP, statusP, flags) \
    ftu_AggrSyscall(AGOP_VOLCREATE, (aggrId), (int)(fsetIdP), (int)(statusP), \
		    (flags))
#define AG_VOLENUMERATE(numFsets, iodescP, indexP, outLenP) \
    ftu_AggrSyscall(AGOP_VOLENUMERATE, (numFsets), (int)(iodescP), \
		    (int)(indexP), (int)(outLenP))

/*
 * Fileset ops.
 */
#define VOL_AGOPEN(agopenP, fsetDescP, errP) \
    ftu_VolSyscall(VOLOP_AGOPEN, (int)(agopenP), (int)(fsetDescP), \
		   (int)(errP), 0)
#define	VOL_CLOSE(fsetDesc, isAbort) \
    ftu_VolSyscall(VOLOP_CLOSE, (fsetDesc), (isAbort), 0, 0)
#define	VOL_GETSTATUS(fsetDesc, statusP) \
    ftu_VolSyscall(VOLOP_GETSTATUS, (fsetDesc), (int)(statusP), 0, 0)
#define	VOL_SETSTATUS(fsetDesc, mask, statusP, spareMask) \
    ftu_VolSyscall(VOLOP_SETSTATUS, (fsetDesc), (mask), (int)(statusP), \
		   (spareMask))
#define VOL_BULKSETSTATUS(arrayLen, statusArray) \
    ftu_VolSyscall(VOLOP_BULKSETSTATUS, (arrayLen), (int)(statusArray), 0, 0)
#define VOL_SETVV(fsetDesc, vvSetStr) \
    ftu_VolSyscall(VOLOP_SETVV, (fsetDesc), (int)(vvSetStr), 0, 0)
#define VOL_PUSHSTATUS(fsetDesc) \
    ftu_VolSyscall(VOLOP_PUSHSTATUS, (fsetDesc), 0, 0, 0)
#define VOL_SYNC(fsetDesc, syncOp) \
    ftu_VolSyscall(VOLOP_SYNC, (fsetDesc), (syncOp), 0, 0)
#define VOL_CLONE(destDesc, srcDesc) \
    ftu_VolSyscall(VOLOP_CLONE, (destDesc), (srcDesc), 0, 0)
#define VOL_UNCLONE(fsetDesc, backDesc) \
    ftu_VolSyscall(VOLOP_UNCLONE, (fsetDesc), (backDesc), 0, 0)
#define VOL_RECLONE(fsetDesc, backDesc) \
    ftu_VolSyscall(VOLOP_RECLONE, (fsetDesc), (backDesc), 0, 0)
#define VOL_DEPLETE(fsetDesc) \
    ftu_VolSyscall(VOLOP_DEPLETE, (fsetDesc), 0, 0, 0)
#define VOL_DESTROY(fsetDesc) \
    ftu_VolSyscall(VOLOP_DESTROY, (fsetDesc), 0, 0, 0)
#define VOL_SWAPVOLIDS(fsetDesc1, fsetDesc2) \
    ftu_VolSyscall(VOLOP_SWAPVOLIDS, (fsetDesc1), (fsetDesc2), 0, 0)
#define VOL_PROBE(fsetDesc) \
    ftu_VolSyscall(VOLOP_PROBE, (fsetDesc), 0, 0, 0)

/*
 * File ops.
 */
#define	VOL_SCAN(fsetDesc, index) \
    ftu_VolSyscall(VOLOP_SCAN, (fsetDesc), (index), 0, 0)
#define	VOL_CREATE(fsetDesc, index, xvattrP) \
    ftu_VolSyscall(VOLOP_CREATE, (fsetDesc), (index), (int)(xvattrP), 0)
#define	VOL_TRUNCATE(fsetDesc, size) \
    ftu_VolSyscall(VOLOP_TRUNCATE, (fsetDesc), (size), 0, 0)
#define	VOL_DELETE(fsetDesc) \
    ftu_VolSyscall(VOLOP_DELETE, (fsetDesc), 0, 0, 0)
#define	VOL_GETATTR(fsetDesc, xvattrP) \
    ftu_VolSyscall(VOLOP_GETATTR, (fsetDesc), (int)(xvattrP), 0, 0)
#define	VOL_SETATTR(fsetDesc, xvattrP) \
    ftu_VolSyscall(VOLOP_SETATTR, (fsetDesc), (int)(xvattrP), 0, 0)
#define	VOL_GETACL(fsetDesc, aclP, which) \
    ftu_VolSyscall(VOLOP_GETACL, (fsetDesc), (int)(aclP), (which), 0)
#define	VOL_SETACL(fsetDesc, aclP, which, index) \
    ftu_VolSyscall(VOLOP_SETACL, (fsetDesc), (int)(aclP), (which), (index))
#define VOL_COPYACL(fsetDesc, destWhich, srcIndex, srcWhich) \
    ftu_VolSyscall(VOLOP_COPYACL, (fsetDesc), (destWhich), (srcIndex), \
		   (srcWhich))
#define	VOL_READ(fsetDesc, iodescP, outlenP) \
    ftu_VolSyscall(VOLOP_READ, (fsetDesc), (int)(iodescP), (int)(outlenP), 0)
#define	VOL_WRITE(fsetDesc, iodescP) \
    ftu_VolSyscall(VOLOP_WRITE, (fsetDesc), (int)(iodescP), 0, 0)
#define VOL_READDIR(fsetDesc, iodescP, nEntriesP) \
    ftu_VolSyscall(VOLOP_READDIR, (fsetDesc), (int)(iodescP), \
		   (int)(nEntriesP), 0)
#define VOL_APPENDDIR(fsetDesc, iodescP, nEntries, keepOffsets) \
    ftu_VolSyscall(VOLOP_APPENDDIR, (fsetDesc), (int)(iodescP), (nEntries), \
		   (keepOffsets))
#define	VOL_READHOLE(fsetDesc, readHoleDescP) \
    ftu_VolSyscall(VOLOP_READHOLE, (fsetDesc), (int)(readHoleDescP), 0, 0)



#endif	/* _TRANSARC_DFS_FTU_SYSCALLS_H */
