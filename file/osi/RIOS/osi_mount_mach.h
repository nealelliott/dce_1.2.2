/*
 * HISTORY
 * $Log$
 * $EndLog$
 */
/* Copyright (C) 1993 Transarc Corporation - All rights reserved */

#ifndef TRANSARC_OSI_MOUNT_MACH_H
#define	TRANSARC_OSI_MOUNT_MACH_H

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/osi/RIOS/RCS/osi_mount_mach.h,v 1.3 1994/04/29 20:07:55 berman Exp $ */

#include <sys/types.h>
#include <sys/vmount.h>

#define OSI_MOUNT_TYPE_DFS	7
#define OSI_MOUNT_TYPE_LFS	10
#define OSI_MOUNT_TYPE_AGFS	11

#define OSI_MOUNT_FLAGS_READONLY	MNT_READONLY
#define OSI_MOUNT_FLAGS_NOSUID		MNT_NOSUID

extern int osi_umount(const char* mp, int flags);

#endif	/* TRANSARC_OSI_MOUNT_MACH_H */
