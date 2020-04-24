/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsSpoke_defs.c,v $
 * Revision 1.1.6.1  1996/10/17  18:28:28  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:17  damon]
 *
 * $EndLog$
 */
/*
 * ID: $Id: dfsSpoke_defs.c,v 1.1.6.1 1996/10/17 18:28:28 damon Exp $
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

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <utils/itl/itl.h>
#include <utils/itl/dfsSpoke_defs.h>

EXPORT boolean_t dfsSpoke_StatCompare(sourceP, destP)
  IN struct stat *sourceP;
  IN struct stat *destP;
{
    /* 
     * --XXX -- 
     *      DFS does not really care about the differences of following fields.
     *      Maybe, it will be used for AFS or some other file systems. 
     *      Here is needs to be enhanced later.

#if defined(SUNOS4) || defined(SUNOS5) || defined(_AIX)
    if ( destP->st_dev != sourceP->st_dev ){
       itl_Error("Local st_dev %d is different from the remote st_dev %d\n", sourceP->st_dev, destP->st_dev);
       return FALSE;
     }
#endif
    if ( destP->st_blocks != sourceP->st_blocks ){
       itl_Error("Local st_blocks %d is different from the remote st_blocks %d\n", sourceP->st_blocks, destP->st_blocks);
       return FALSE;
     }
    if ( destP->st_rdev != sourceP->st_rdev ){
       itl_Error("Local st_rdev %d is different from the remote st_rdev %d\n", sourceP->st_rdev, destP->st_rdev);
       return FALSE;
     }
    if ( destP->st_blksize != sourceP->st_blksize ){
       itl_Error("Local st_blksize %d is different from the remote st_blksize %d\n", sourceP->st_blksize, destP->st_blksize);
       return FALSE;
     }
    if ( destP->st_ino != sourceP->st_ino ){
       itl_Error("Local st_ino %d is different from the remote st_ino %d\n", sourceP->st_ino, destP->st_ino);
       return FALSE;
     }

     *  XXX - End
     */

    /*
     * The followings are what DFS wants to compare with.
     */
    if ( destP->st_mode != sourceP->st_mode ){
       itl_Error("Local st_mode %d is different from the remote st_mode %d\n", sourceP->st_mode, destP->st_mode);
       return FALSE;
     }
    if ( destP->st_nlink != sourceP->st_nlink ){
       itl_Error("Local st_nlink %d is different from the remote st_nlink %d\n", sourceP->st_nlink, destP->st_nlink);
       return FALSE;
     }
    if ( destP->st_uid != sourceP->st_uid ){
       itl_Error("Local st_uid %d is different from the remote st_uid %d\n", sourceP->st_uid, destP->st_uid);
       return FALSE;
     }
    if ( destP->st_gid != sourceP->st_gid ){
       itl_Error("Local st_uid %d is different from the remote st_gid %d\n", sourceP->st_gid, destP->st_gid);
       return FALSE;
     }
    if ( destP->st_size != sourceP->st_size ){
       itl_Error("Local st_size %d is different from the remote st_size %d\n", sourceP->st_size, destP->st_size);
       return FALSE;
     }
    if ( destP->st_atime != sourceP->st_atime ){
       itl_Error("Local st_atime %d is different from the remote st_atime %d\n", sourceP->st_atime, destP->st_atime);
       return FALSE;
     }
    if ( destP->st_mtime != sourceP->st_mtime ){
       itl_Error("Local st_mtime %d is different from the remote st_mtime %d\n", sourceP->st_mtime, destP->st_mtime);
       return FALSE;
     }
    if ( destP->st_ctime != sourceP->st_ctime ){
       itl_Error("Local st_ctime %d is different from the remote st_ctime %d\n", sourceP->st_ctime, destP->st_ctime);
       return FALSE;
     }
     return TRUE;
}


EXPORT void dfsSpoke_NormalizeLocalDirent(sourceP, destP)
  IN struct dirent  *sourceP;
  IN dfsSpoke_dirent_t *destP;
{
#ifdef SUNOS4
    destP->d_fileno = sourceP->d_fileno;
#else /* SUNOS4 */
    destP->d_ino = sourceP->d_ino;
#endif /* SUNOS4 */
    destP->d_reclen = sourceP->d_reclen;
#ifdef SUNOS5
    destP->d_namlen = (unsigned16) strlen(sourceP->d_name);
#else /* SUNOS5 */
    destP->d_namlen = sourceP->d_namlen;
#endif  /* SUNOS5 */
    memcpy(destP->d_name, sourceP->d_name, 
	   (int) strlen((char *) &sourceP->d_name));
}

EXPORT void dfsSpoke_LocalizeNormalDirent(sourceP, destP)
  IN dfsSpoke_dirent_t *sourceP;
  IN struct dirent *destP;
{
#ifdef SUNOS4
    destP->d_fileno = sourceP->d_fileno;
#else /* SUNOS4 */
    destP->d_ino = sourceP->d_ino;
#endif /* SUNOS4 */    
    destP->d_reclen = sourceP->d_reclen;

#ifndef SUNOS5
    destP->d_namlen = sourceP->d_namlen;
#endif  /* SUNOS5 */

    memcpy(destP->d_name, sourceP->d_name, 
	   (int) strlen((char *) &sourceP->d_name));
}

