/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *      Copyright (C) 1996, 1990 Transarc Corporation
 *      All rights reserved.
 *
 *       UPDATE Server (AFS 4.0) Exported Procedures
 *
 * notes:
 * 1) locking strategy changed so that locking is done per exported
 *	procedure, rather than per subroutine call. This serializes
 *	operations, but eliminates the inconsistent, per subroutine
 *	locking. P.A.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>

#include <dirent.h>

#ifdef  AFS_AIX_ENV
    /*
     * Unfortunately in AIX valuable fields such as st_blksize are gone
     * from the stat structure!!
     */
#include <sys/statfs.h>
#endif /* AFS_AIX_ENV */

#include <dce/rpc.h>
#include <pthread.h>

#include <dcedfs/compat.h>
#include <dcedfs/dfsauth.h>

#include <update.h>
#include <upcommon.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsudtmac.h>
#include <dfsudtsvc.h>
#include <dfsudtmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/update/RCS/manager.c,v 4.25 1996/03/04 19:14:22 ota Exp $")

extern char *whoami;
extern pthread_mutex_t updateMutex;


/* Forward declarations for ANSI C */
long update_SendFile();
long update_SendDirInfo();

#define	LOCK_UPDATE_MUTEX					\
    if (pthread_mutex_lock (&updateMutex) != 0)			\
    {								\
        Log(udt_s_global_lock_locking_error,\
                whoami, errno); \
        exit(1);							\
    }

#define	UNLOCK_UPDATE_MUTEX					\
    if (pthread_mutex_unlock (&updateMutex) != 0)		\
    {								\
	Log(udt_s_global_lock_unlock_error,\
		whoami, errno);					\
	exit(1);							\
    }

/* used for premature routine exit */
#define	ERROR(n)						\
{								\
    code = n;							\
    goto error_exit;						\
}


/* UPDATE_GetServerInterfaces
 *    send a list of registered upserver interfaces to the caller.
 */

error_status_t UPDATE_GetServerInterfaces (h, serverInterfaces)
    /* [in] */   rpc_binding_handle_t         h;
    /* [out] */ dfs_interfaceList *serverInterfaces;
{
    long code = 0, code2 = 0;
    static char	routineName[] = "UPDATE_FetchFile";
    int savedCancelState;

    DFS_DISABLE_CANCEL(&savedCancelState, code);
    if (code) {
      LogError(udt_s_disable_cancel_failed, code, whoami, routineName);
	serverInterfaces->dfs_interfaceList_len = 0;
	return(code);
    }

    code = dfsauth_server_CheckAuthorization(&h,
					     (dacl_permset_t *)NULL,
					     (char *)NULL,
					     (uuid_t *)NULL);
    if ( code )
    {
	if ( code != DAUT_ERROR_ACCESS_DENIED) {
	    LogError(udt_s_auth_check_failed, code, whoami, routineName);
	}
	serverInterfaces->dfs_interfaceList_len = 0;
	ERROR(code);
    }
    dfs_GetServerInterfaces (UPDATE_v4_0_s_ifspec, serverInterfaces,
			     (unsigned32 *)&code);
    if (code) {
      LogError(udt_s_getserver_intfcs_failed, code, whoami, routineName);
      ERROR(code);
    }

error_exit:
    DFS_ENABLE_CANCEL(savedCancelState, code2);
    if (code2) {
      LogError(udt_s_enable_cancel_failed, code2, whoami, routineName);
	serverInterfaces->dfs_interfaceList_len = 0;
    }
    if (code)
      return code;
    else return code2;
}

/* UPDATE_FetchFile
 *	send the contents of a file to the caller.
 */

error_status_t UPDATE_FetchFile(h, FileName, pipeStream)
    /* [in] */   rpc_binding_handle_t         h;
    /* [in] */   NameString_t     FileName;
    /* [out] */  pipe_t           *pipeStream;
{
    int     fd = -1;
    volatile int	    lockHeld = 0;
    struct  stat status;
    char   *pipeBuffP = (char *)0;
    volatile long code = 0, code2 = 0;
    static char	routineName[] = "UPDATE_FetchFile";
    volatile int savedCancelState;

    /* place a TRY/FINALLY around critical region */
    TRY
    {
	DFS_DISABLE_CANCEL(&savedCancelState, code);
	if (code) {
	    /* terminate the pipe */
	    pipeStream->push(pipeStream->state, (unsigned char *)pipeBuffP, 0);

	    LogError(udt_s_pipe_disable_cancel_failed, code, whoami, routineName);
	    ERROR(code);
	}

	LOCK_UPDATE_MUTEX;
	lockHeld = 1;
	code = dfsauth_server_CheckAuthorization(&h,
						 (dacl_permset_t *)NULL,
						 (char *)NULL,
						 (uuid_t *)NULL);
	if ( code )
	{
	    if ( code != DAUT_ERROR_ACCESS_DENIED) {
		LogError(udt_s_daut_error_auth_failed, code, whoami, routineName);
	    }

	    /* terminate the pipe */
	    pipeStream->push(pipeStream->state, (unsigned char *)pipeBuffP, 0);
	    ERROR(code);
	}
	if ( canExportObject(FileName) == 0 )
	{
	    /* terminate the pipe */
	    pipeStream->push(pipeStream->state, (unsigned char *)pipeBuffP, 0);

	    Log(udt_s_req_not_in_list, FileName);
	    ERROR(UP_NOT_EXPORTABLE);
	}

	fd = open(FileName, O_RDONLY, 0);       /* Open the target file */
	if (fd < 0 || fstat(fd, &status) < 0) {
	    /* terminate the pipe */
	    pipeStream->push(pipeStream->state, (unsigned char *)pipeBuffP, 0);

	    Log(udt_s_open_file_failed, FileName);
	    ERROR(UP_SOFT_ERROR);
	}
	code = update_SendFile(fd, pipeStream, &status);
	if (fd >=0)
	    close(fd);

    error_exit:;
    }
    FINALLY
    {
	if (lockHeld)
	    UNLOCK_UPDATE_MUTEX;
	DFS_ENABLE_CANCEL(savedCancelState, code2);
    }
    ENDTRY

    if (code2)
      LogError(udt_s_enable_cancel_failed2, code2, whoami, routineName);
    if (code)
      return code;
    else
      return code2;
}

/*
 * Fetch dir info about directory name and send it to remote client
 */

error_status_t UPDATE_FetchInfo(h, DirName, pipeStream)
    /* [in] */   rpc_binding_handle_t         h;
    /* [in] */   NameString_t     DirName;
    /* [out] */  pipe_t           *pipeStream;
{
    volatile int fd = -1;
    volatile int lockHeld = 0;
    struct stat status;
    char   *pipeBuffP = (char *)0;
    volatile long code = 0, code2 = 0;
    static char	routineName[] = "UPDATE_FetchInfo";
    volatile int savedCancelState;

    TRY
    {
	DFS_DISABLE_CANCEL(&savedCancelState, code);
	if (code) {
	    /* terminate the pipe */
	    pipeStream->push(pipeStream->state, (unsigned char *)pipeBuffP, 0);

	    LogError(udt_s_disable_cancel_failed2, code, whoami, routineName);
	    ERROR(code);
	}

	LOCK_UPDATE_MUTEX;
	lockHeld = 1;
	code = dfsauth_server_CheckAuthorization(&h,
						 (dacl_permset_t *)NULL,
						 (char *)NULL,
						 (uuid_t *)NULL);
	if ( code )
	{
	    if (code != DAUT_ERROR_ACCESS_DENIED) {
		LogError(udt_s_daut_error_auth_failed2, code, whoami, routineName);
	    }

	    /* terminate the pipe */
	    pipeStream->push(pipeStream->state, (unsigned char *)pipeBuffP, 0);
	    ERROR(code);
	}
	if ( canExportObject(DirName) == 0 )
	{
	    /* terminate the pipe */
	    pipeStream->push(pipeStream->state, (unsigned char *)pipeBuffP, 0);

	    Log(udt_s_req_not_in_list2, DirName);
	    ERROR(UP_NOT_EXPORTABLE);
	}

	fd = open(DirName, O_RDONLY, 0);
	if (fd < 0 || fstat(fd, &status) < 0) {
	    /* terminate the pipe */
	    pipeStream->push(pipeStream->state, (unsigned char *)pipeBuffP, 0);

	    Log(udt_s_open_dir_failed, DirName);
	    ERROR(UP_SOFT_ERROR);
	}

	if((status.st_mode & S_IFMT) != S_IFDIR){
	    /* terminate the pipe */
	    pipeStream->push(pipeStream->state, (unsigned char *)pipeBuffP, 0);

	    Log(udt_s_obj_not_dir,DirName);
	    ERROR(ENOENT);
	}
	code  =  update_SendDirInfo(DirName, pipeStream, &status);

    error_exit:;
    }
    FINALLY
    {
	if (fd >= 0)
	    close(fd);
	if (lockHeld)
	    UNLOCK_UPDATE_MUTEX;

	DFS_ENABLE_CANCEL(savedCancelState, code2);
    }
    ENDTRY

    if (code2)
      LogError(udt_s_enable_cancel_failed3, code2, whoami, routineName);
    if (code)
      return code;
    else
      return code2;
}

/*
 * This file is called to read data from the specified target file, (ie., fd)
 * and write data into the provided pipeStream which is "connected" to the
 * client. The number of bytes to be transferred, at one time, is determined by
 * the NIDL_PIPE_BUFF_SIZE. A optimal size should be determined in the future !
 */
long update_SendFile(fd, pipeStream, status)
     register int            fd;
     register struct pipe_t  *pipeStream;
     register struct stat    *status;
{
    long   length;
    int    blockSize = 0;
    int    nbytes, numBytes;
    char   * volatile pipeBuff = (char *)0;
    volatile long   code = 0;
#ifdef AFS_AIX_ENV
    struct statfs tstatfs;
#endif /* AFS_AIX_ENV */

    LOCK_UPDATE_MUTEX;
    TRY
    {
	length = status->st_size;   	/* The target file size */

#ifdef  AFS_AIX_ENV
	fstatfs(fd, &tstatfs);
	blockSize = tstatfs.f_bsize;
#else
	blockSize = status->st_blksize;     /* the prefer block size */
#endif

	pipeBuff = (char *)malloc(blockSize);
	if (!pipeBuff) {
	    Log(udt_s_no_pipebuff, whoami, errno);
	    ERROR(UP_SOFT_ERROR);
	}

	while (!code && length) {
	    /*
	     * Fetch the file and write it to the Stream-Pipe.
	     */
	    nbytes = (length > blockSize ? blockSize : length);
	    numBytes = read(fd, pipeBuff, nbytes);

	    if (numBytes <= 0) {
		Log(udt_s_fsread_failed);
		code = UP_SOFT_ERROR;
		break;
	    }
	    pipeStream->push(pipeStream->state, (unsigned char *)pipeBuff, numBytes);
	    length -= numBytes;
	}
	pipeStream->push(pipeStream->state, (unsigned char *)pipeBuff, 0);

	if (pipeBuff)
	    free(pipeBuff);

    error_exit:;
    }
    FINALLY
    {
	UNLOCK_UPDATE_MUTEX;
    }
    ENDTRY

    return code;
}

/* update_SendDirInfo
 *	send a directory listing of the named directory
 */

long update_SendDirInfo(DirName, pipeStream, status)
     char     *DirName;
     pipe_t   *pipeStream;
     register struct stat   *status;
{
    struct dirent *dp;
    DIR * volatile dirp = NULL;
    FILE * volatile stream = NULL;
    struct stat tstatus;
    char filename[MAX_NAME_SIZE];
    char tempFile[MAX_NAME_SIZE];
    int fd, tfd;
    volatile long code = 0;

    LOCK_UPDATE_MUTEX;
    TRY
    {
	dirp = opendir(DirName);
	if ( dirp == NULL )
	    ERROR(UP_SOFT_ERROR);


	{
	char *dcesptr;
	dcesptr = (char *)dce_sprintf(udt_s_get_self_pid, getpid(), pthread_self());
	strcpy(tempFile , dcesptr);
	free(dcesptr);
	}

	stream = fopen(tempFile,"w");
	if ( stream == NULL )
	    ERROR(UP_SOFT_ERROR);

	while(dp = readdir((DIR *)dirp)){
	    strcpy(filename, DirName);
	    strcat(filename,"/");
	    strcat(filename,dp->d_name);

	    tfd = open(filename, O_RDONLY, 0);
	    if (tfd < 0 || fstat(tfd, &tstatus) < 0) {
		Log(udt_s_failed_to_open, filename);
		ERROR(UP_SOFT_ERROR);
	    }
	    if((tstatus.st_mode & S_IFMT) != S_IFDIR)  {/* not a directory */
		fprintf((FILE *)stream, "%s %u %u %u %u %u %u\n",
			filename,
			tstatus.st_mtime,
			tstatus.st_size,
			tstatus.st_mode,
			tstatus.st_uid,
			tstatus.st_gid,
			tstatus.st_atime);
	    }
	    code = close(tfd);
	    if(code) {
		Log(udt_s_close_file_failed,filename);
		ERROR(UP_SOFT_ERROR);
	    }
	    fflush(stdout);
	}

	if (stream) {
	    if (ferror((FILE *)stream))
		if (!code) code = UP_SOFT_ERROR;
	    fclose((FILE *)stream);
	    stream = NULL;
	}

	if (code == 0) {
	    fd = open(tempFile, O_RDONLY, 0);
	    fstat(fd, &tstatus);
	    code = update_SendFile(fd, pipeStream, &tstatus);
	    if ( code )
		code = UP_SOFT_ERROR;
	    close(fd);
	    unlink(tempFile);
	}

    error_exit:;
    }
    FINALLY
    {
	if (dirp)
	    closedir((DIR *)dirp);

	if (stream)
	    fclose((FILE *)stream);

	UNLOCK_UPDATE_MUTEX;
    }
    ENDTRY

    return code;
}

/* UPDATE_FetchObjectInfo
 *	returns a stat of the named object. Selected items from the stat
 *	are returned to the caller.
 */

error_status_t
UPDATE_FetchObjectInfo(h, objectName, fileStatP)
    /* [in] */  rpc_binding_handle_t         h;
    /* [in] */	NameString_t     objectName;
    /* [out]*/	updateFileStatP fileStatP;
{
    struct stat statBuf;
    long code = 0, code2 = 0;
    int savedCancelState;
    static char	routineName[] = "UPDATE_FetchObjectInfo";

    DFS_DISABLE_CANCEL(&savedCancelState, code);
    if (code) {
      LogError(udt_s_disable_cancel_failed4, code, whoami, routineName);
	return(code);
    }

    LOCK_UPDATE_MUTEX;
#ifdef notdef
    code = dfsauth_server_CheckAuthorization(&h,
					     (dacl_permset_t *)NULL,
					     (char *)NULL,
					     (uuid_t *)NULL);

    if ( code )
    {
        if (code != DAUT_ERROR_ACCESS_DENIED) {
            LogError(udt_s_daut_error_auth_failed4, code, whoami, routineName);
        }
	ERROR(code);
    }
#endif
    if ( canExportObject(objectName) == 0 )
    {
        Log(udt_s_obj_export_failed, objectName);
	ERROR(UP_NOT_EXPORTABLE);
    }
    code = stat((char *)objectName, &statBuf);
    if ( code )
    {
	Log(udt_s_stat_obj_failed, objectName, errno);
	ERROR(UP_SOFT_ERROR);
    }

    /* copy the information back to the caller's structure */
    AFS_hset64(fileStatP->fileLength, 0, statBuf.st_size);
    fileStatP->mode = 	statBuf.st_mode;
    fileStatP->uid = 	statBuf.st_uid;
    fileStatP->gid = 	statBuf.st_gid;
    fileStatP->mtime = 	statBuf.st_mtime;
    fileStatP->atime = 	statBuf.st_atime;

error_exit:
    UNLOCK_UPDATE_MUTEX;

    DFS_ENABLE_CANCEL(savedCancelState, code2);
    if (code2)
      LogError(udt_s_enable_cancel_failed4, code2, whoami, routineName);
    if (code)
      return code;
    else
      return code2;
}

/* declare and initialize the manager's entry point vector  */
UPDATE_v4_0_epv_t   UPDATE_v4_0_manager_epv = {
                                 UPDATE_GetServerInterfaces,
                                 UPDATE_FetchInfo,
                                 UPDATE_FetchFile,
			         UPDATE_FetchObjectInfo};
