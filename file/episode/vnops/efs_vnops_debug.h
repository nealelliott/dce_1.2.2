/*
 * Copyright (C) 1994 Transarc Corporation
 * All rights reserved.
 */

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_vnops_debug.h,v 1.6 1996/05/02 21:22:01 rajesh Exp $ */

extern long efs_vnops_debug (long, long, long);

#define EFS_DEBUG_VNOPS_SYNC_VNODES 0x1
/* sub cases */
#define EFS_DEBUG_VNOPS_SYNC_VNODES_METAONLY 0x1
#define EFS_DEBUG_VNOPS_GET_ALLOCINFO 0x2


/* For EFS_DEBUG_VNOPS_GET_ALLOCINFO */
enum efs_regionType {efs_regionAllocated = 1, efs_regionCow = 2, 
			 efs_regionHole = 3, efs_regionInline = 4,
			 efs_regionPurged = 5};
    
struct efs_allocRegionInfo {
    enum efs_regionType type;
    long offset;
    long length;
    daddr_t dblk;
};

struct efs_allocInfo {
    int offset; 	/* Initialize to zero the first time */
    int numRegions;    
    int eof;
    struct efs_allocRegionInfo *regions;
};


