/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsSpoke_defs.h,v $
 * Revision 1.1.6.1  1996/10/17  18:28:29  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:18  damon]
 *
 * $EndLog$
 */
/*
 * ID: $Id: dfsSpoke_defs.h,v 1.1.6.1 1996/10/17 18:28:29 damon Exp $
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

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/dfsSpoke.h>
    
#ifndef SUNOS5
#define	CONVERT_LOCAL_STAT_TO_SPOKE_STAT(s,d)	\
    MACRO_BEGIN	\
    (d)->st_dev = (s)->st_dev;\
    (d)->st_ino = (s)->st_ino;\
    (d)->st_mode = (s)->st_mode;\
    (d)->st_nlink = (s)->st_nlink;\
    (d)->st_uid = (s)->st_uid;\
    (d)->st_gid = (s)->st_gid;\
    (d)->st_rdev = (s)->st_rdev;\
    (d)->st_size = (s)->st_size;\
    (d)->st_atime_ = (s)->st_atime;\
    (d)->st_mtime_ = (s)->st_mtime;\
    (d)->st_ctime_ = (s)->st_ctime;\
    (d)->st_blksize = (s)->st_blksize;\
    (d)->st_blocks = (s)->st_blocks;\
    MACRO_END
#else 
#define	CONVERT_LOCAL_STAT_TO_SPOKE_STAT(s,d)	\
    MACRO_BEGIN	\
    (d)->st_dev = (s)->st_dev;\
    (d)->st_ino = (s)->st_ino;\
    (d)->st_mode = (s)->st_mode;\
    (d)->st_nlink = (s)->st_nlink;\
    (d)->st_uid = (s)->st_uid;\
    (d)->st_gid = (s)->st_gid;\
    (d)->st_rdev = (s)->st_rdev;\
    (d)->st_size = (s)->st_size;\
    (d)->st_atime_ = (s)->st_atim.tv_sec;\
    (d)->st_mtime_ = (s)->st_mtim.tv_sec;\
    (d)->st_ctime_ = (s)->st_ctim.tv_sec;\
    (d)->st_blksize = (s)->st_blksize;\
    (d)->st_blocks = (s)->st_blocks;\
    MACRO_END
#endif

#ifndef SUNOS5 
#define	CONVERT_SPOKE_STAT_TO_LOCAL_STAT(s,d)	\
    MACRO_BEGIN	\
    (d)->st_dev = (s)->st_dev;\
    (d)->st_ino = (s)->st_ino;\
    (d)->st_mode = (s)->st_mode;\
    (d)->st_nlink = (s)->st_nlink;\
    (d)->st_uid = (s)->st_uid;\
    (d)->st_gid = (s)->st_gid;\
    (d)->st_rdev = (s)->st_rdev;\
    (d)->st_size = (s)->st_size;\
    (d)->st_atime = (s)->st_atime_;\
    (d)->st_mtime = (s)->st_mtime_;\
    (d)->st_ctime = (s)->st_ctime_;\
    (d)->st_blksize = (s)->st_blksize;\
    (d)->st_blocks = (s)->st_blocks;\
    MACRO_END
#else 
#define	CONVERT_SPOKE_STAT_TO_LOCAL_STAT(s,d)	\
    MACRO_BEGIN	\
    (d)->st_dev = (s)->st_dev;\
    (d)->st_ino = (s)->st_ino;\
    (d)->st_mode = (s)->st_mode;\
    (d)->st_nlink = (s)->st_nlink;\
    (d)->st_uid = (s)->st_uid;\
    (d)->st_gid = (s)->st_gid;\
    (d)->st_rdev = (s)->st_rdev;\
    (d)->st_size = (s)->st_size;\
    (d)->st_atim.tv_sec = (s)->st_atime_;\
    (d)->st_atim.tv_nsec = 0;\
    (d)->st_mtim.tv_sec = (s)->st_mtime_;\
    (d)->st_mtim.tv_nsec = 0;\
    (d)->st_ctim.tv_sec = (s)->st_ctime_;\
    (d)->st_ctim.tv_nsec = 0;\
    (d)->st_blksize = (s)->st_blksize;\
    (d)->st_blocks = (s)->st_blocks;\
    MACRO_END
#endif

extern boolean_t stat_compare _TAKES((
				      IN struct stat *sourceP,
				      IN struct stat *destP
				      ));

extern void dfsSpoke_NormalizeLocalDirent _TAKES((
						  IN struct dirent  *sourceP,
						  IN dfsSpoke_dirent_t *destP
						  ));

extern void dfsSpoke_LocalizeNormalDirent _TAKES((
						  IN dfsSpoke_dirent_t *sourceP,
						  IN struct dirent *destP
						  ));

