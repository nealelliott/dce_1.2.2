/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_file.h,v $
 * Revision 1.1.414.1  1996/10/17  18:25:45  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:56  damon]
 *
 * Revision 1.1.409.1  1994/02/04  20:43:45  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:48  devsrc]
 * 
 * Revision 1.1.407.1  1993/12/07  17:43:37  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:11:12  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/11  18:29:48  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:07:34  htf]
 * 
 * Revision 1.1.2.3  1992/11/18  21:45:14  jaffe
 * 	Transarc delta: jess-itl-add-mknod 1.1
 * 	  Selected comments:
 * 	    Add the system call mknod to ITL file interface.
 * 	    See above.
 * 	Transarc delta: jess-itl-add-time-function 1.2
 * 	  Selected comments:
 * 	    Add time call to ITL interface. The ITL variable commandResult is
 * 	    used to pass the current time in seconds back to calling script
 * 	    if the call successful.
 * 	    See above.
 * 	    Change the return variable to user variable istead of CommandResult.
 * 	    No change.
 * 	[1992/11/18  14:49:31  jaffe]
 * 
 * Revision 1.1.2.2  1992/10/28  17:39:40  jaffe
 * 	New file
 * 	[1992/10/27  20:00:09  jaffe]
 * 
 * $EndLog$
 */
/*
 * itl_file.h -- 
 *    Definitions for itl_file.c.
 *
 *    Contains all the commands that a tester might want for writing 
 *    test programs involving basic file system operations.
 */

#ifndef ITL_FILE_H
#define ITL_FILE_H

#include <errno.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_stds.h>

typedef enum {
    HardLink,
    SymbolicLink
} itlFile_linkType_t;

typedef enum {
    FileStat,
    LinkStat
} itlFile_statType_t;

/*
 *  Define all the useful file system commands.
 */

#define ITL_FILE_OPEN_CMD                                             \
 {"open", itlFile_OpenOrCreat, TRUE, FALSE, COMMAND_MODE_ALL,         \
         "Open a file/device.",                                       \
         "file",                                                      \
         ""}

#define ITL_FILE_CREAT_CMD                                            \
 {"creat", itlFile_OpenOrCreat, FALSE, FALSE, COMMAND_MODE_ALL,       \
         "Create a new file.",                                        \
         "file",                                                      \
         ""}

#define ITL_FILE_GETWD_CMD                                            \
 {"getwd", itlFile_Getwd, FALSE, FALSE, COMMAND_MODE_ALL,             \
         "Get path-name of current dir.",                             \
         "file",                                                      \
         ""}

#define ITL_FILE_UMASK_CMD                                            \
 {"umask", itlFile_Umask, 0, 0, COMMAND_MODE_ALL,                     \
      "Set the umask",                                                \
      "file",                                                         \
      ""}

#define ITL_FILE_TIME_CMD                                             \
 {"time", itlFile_Time, 0, 0, COMMAND_MODE_ALL,                       \
      "Get the current time",                                         \
      "file",                                                         \
      ""}

#define ITL_FILE_SIGACTION_CMD                                        \
 {"sigaction", itlFile_Sigaction, 0, 0, COMMAND_MODE_ALL,             \
      "Set signal and sigaction",                                     \
      "file",                                                         \
      ""}

#define ITL_FILE_ALARM_CMD                                            \
 {"alarm", itlFile_Alarm, 0, 0, COMMAND_MODE_ALL,                     \
      "Set timeout value",                                            \
      "file",                                                         \
      ""}

#define ITL_FILE_ULIMIT_CMD                                           \
 {"ulimit", itlFile_Ulimit, 0, 0, COMMAND_MODE_ALL,                   \
      "Execute ulimit command.",                                      \
      "file",                                                         \
      ""}

#define ITL_FILE_CLOSE_CMD                                            \
 {"close", itlFile_Close, 0, FALSE, COMMAND_MODE_ALL,                 \
      "Close an open file/device.",                                   \
      "file",                                                         \
      ""}

#define ITL_FILE_READLINK_CMD                                         \
 {"readlink", itlFile_Readlink, 0, FALSE, COMMAND_MODE_ALL,           \
      "Readlink data from the symbolic link.",                        \
      "file",                                                         \
      ""}

#define ITL_FILE_READ_CMD                                             \
 {"read", itlFile_Read, 0, FALSE, COMMAND_MODE_ALL,                   \
      "Read data from an open file/device",                           \
      "file",                                                         \
      ""}

#define ITL_FILE_WRITE_CMD                                            \
 {"write", itlFile_Write, 0, FALSE, COMMAND_MODE_ALL,                 \
      "Write data to an open file/device",                            \
      "file",                                                         \
      ""}

#define ITL_FILE_LSEEK_CMD                                            \
 {"lseek", itlFile_Lseek, 0, FALSE, COMMAND_MODE_ALL,                 \
      "Move the read/write pointer in a file/device",                 \
      "file",                                                         \
      ""}

#define ITL_FILE_MKNOD_CMD                                            \
 {"mknod", itlFile_Mknod, 0, FALSE, COMMAND_MODE_ALL,                 \
      "Create special files",                 \
      "file",                                                         \
      ""}

#define ITL_FILE_HARD_LINK_CMD                                        \
 {"hardlink", itlFile_Link, HardLink, FALSE, COMMAND_MODE_ALL,        \
      "Make a hard link to a file",                                   \
      "file",                                                         \
      ""}

#define ITL_FILE_SYM_LINK_CMD                                         \
 {"symlink", itlFile_Link, SymbolicLink, FALSE, COMMAND_MODE_ALL,     \
      "Make a symbolic link to a file",                               \
      "file",                                                         \
      ""}

#define ITL_FILE_UNLINK_CMD                                           \
 {"unlink", itlFile_Unlink, 0, FALSE, COMMAND_MODE_ALL,               \
      "Synonym for remove",                                           \
      "file",                                                         \
      ""}

#define ITL_FILE_REMOVE_CMD                                           \
 {"remove", itlFile_Unlink, 0, FALSE, COMMAND_MODE_ALL,               \
      "Remove a file",                                                \
      "file",                                                         \
      ""}

#define ITL_FILE_MKDIR_CMD                                            \
 {"mkdir", itlFile_Mkdir, 0, FALSE, COMMAND_MODE_ALL,                 \
      "Make a directory",                                             \
      "file",                                                         \
      ""}

#define ITL_FILE_RMDIR_CMD                                            \
 {"rmdir", itlFile_Rmdir, 0, FALSE, COMMAND_MODE_ALL,                 \
      "Remove a directory",                                           \
      "file",                                                         \
      ""}

#define ITL_FILE_CHDIR_CMD                                            \
 {"chdir", itlFile_Chdir, 0, FALSE, COMMAND_MODE_ALL,                 \
      "Change the current directory",                                 \
      "file",                                                         \
      ""}

#define ITL_FILE_CHMOD_CMD                                            \
 {"chmod", itlFile_Chmod, 0, FALSE, COMMAND_MODE_ALL,                 \
      "Change the mode bits on a file/directory.",                    \
      "file",                                                         \
      ""}

#define ITL_FILE_CHOWN_CMD                                            \
 {"chown", itlFile_Chown, 0, FALSE, COMMAND_MODE_ALL,                 \
      "Change the owner and group identifiers of a file.",            \
      "file",                                                         \
      ""}

#define ITL_FILE_CHOWNER_CMD                                          \
 {"chowner", itlFile_Chown, UserId, FALSE, COMMAND_MODE_ALL,          \
      "Change the owner identifier of a file.",                       \
      "file",                                                         \
      ""}

#define ITL_FILE_CHGROUP_CMD                                          \
 {"chgroup", itlFile_Chown, GroupId, FALSE, COMMAND_MODE_ALL,         \
      "Change the group identifier of a file.",                       \
      "file",                                                         \
      ""}

#define ITL_FILE_TRUNCATE_CMD                                         \
 {"truncate", itlFile_Truncate, 1, FALSE, COMMAND_MODE_ALL,           \
      "Truncate the length of a file.",                               \
      "file",                                                         \
      ""}

#define ITL_FILE_FTRUNCATE_CMD                                        \
 {"ftruncate", itlFile_Truncate, 0, FALSE, COMMAND_MODE_ALL,          \
      "Truncate the length of a file.",                               \
      "file",                                                         \
      ""}

#define ITL_FILE_RENAME_CMD                                           \
 {"rename", itlFile_Rename, 0, FALSE, COMMAND_MODE_ALL,               \
      "Rename a file/directory.",                                     \
      "file",                                                         \
      ""}

#define ITL_FILE_UTIMES_CMD                                           \
 {"utimes", itlFile_Utimes, 0, FALSE, COMMAND_MODE_ALL,               \
      "Change the access and modification times of a file.",          \
      "file",                                                         \
      ""}

#define ITL_FILE_STAT_CMD                                             \
 {"stat", itlFile_Stat, FileStat, FALSE, COMMAND_MODE_ALL,            \
         "Get statistics on a file.",                                 \
         "file",                                                      \
         ""}

#define ITL_FILE_LSTAT_CMD                                            \
 {"lstat", itlFile_Stat, LinkStat, FALSE, COMMAND_MODE_ALL,           \
         "Get statistics on a link.",                                 \
         "file",                                                      \
         ""}

#define ITL_FILE_STATFS_CMD                                           \
 {"statfs", itlFile_Statfs, 0, FALSE, COMMAND_MODE_ALL,               \
         "Get infomation about a mounted file system.",               \
         "file",                                                      \
         ""}

#define ITL_FILE_DUP_CMD                                              \
 {"dup", itlFile_Dup, FALSE, FALSE, COMMAND_MODE_ALL,                 \
         "Duplicate a file descriptor.",                              \
         "file",                                                      \
         ""}

#define ITL_FILE_ACCESS_CMD                                           \
 {"access", itlFile_Access, FALSE, FALSE, COMMAND_MODE_ALL,           \
         "Determine accessibility to a file.",                        \
         "file",                                                      \
         ""}

#define ITL_FILE_FSYNC_CMD                                            \
 {"fsync", itlFile_Fsync, FALSE, FALSE, COMMAND_MODE_ALL,             \
         "Flush all I/O buffers to a file.",                          \
         "file",                                                      \
         ""}

#define ITL_FILE_FCNTL_CMD                                            \
 {"fcntl", itlFile_Fcntl, FALSE, FALSE, COMMAND_MODE_ALL,             \
         "Do various file operations.",                               \
         "file",                                                      \
         ""}

#ifndef	SUNOS5
#define ITL_FILE_FLOCK_CMD                                            \
 {"flock", itlFile_Flock, FALSE, FALSE, COMMAND_MODE_ALL,             \
         "Place a lock over an entire file.",                         \
         "file",                                                      \
         ""}
#endif

#define ITL_FILE_MEM_MAP_CMD                                          \
 {"mmap", itlFile_MemMap, FALSE, FALSE, COMMAND_MODE_ALL,             \
         "Map a file to memory.",                                     \
         "file",                                                      \
         ""}

#define ITL_FILE_MEM_PROTECT_CMD                                      \
 {"mprotect", itlFile_MemProtect, FALSE, FALSE, COMMAND_MODE_ALL,     \
         "Change protction mode of a memory mapped file segment.",    \
         "file",                                                      \
         ""}

#define ITL_FILE_MEM_SYNC_CMD                                         \
 {"msync", itlFile_MemSync, FALSE, FALSE, COMMAND_MODE_ALL,           \
         "Synchronize a memory mapped file segment with the file",    \
         "file",                                                      \
         ""}

#define ITL_FILE_MEM_UNMAP_CMD                                        \
 {"munmap", itlFile_MemUnmap, FALSE, FALSE, COMMAND_MODE_ALL,         \
         "Unmap a memory mapped file..",                              \
         "file",                                                      \
         ""}

#define ITL_FILE_MEM_SEM_INIT_CMD                                     \
 {"msem_init", itlFile_MemSemInit, FALSE, FALSE, COMMAND_MODE_ALL,    \
         "Create a memory mapped semaphore.",                         \
         "file",                                                      \
         ""}

#define ITL_FILE_MEM_SEM_LOCK_CMD                                     \
 {"msem_lock", itlFile_MemSemLockOps, TRUE, FALSE, COMMAND_MODE_ALL,  \
         "Lock a memory mapped semaphore.",                           \
         "file",                                                      \
         ""}

#define ITL_FILE_MEM_SEM_UNLOCK_CMD                                    \
 {"msem_unlock", itlFile_MemSemLockOps, FALSE, FALSE, COMMAND_MODE_ALL,\
         "Unlock a memory mapped semaphore.",                          \
         "file",                                                       \
         ""}

#define ITL_FILE_MEM_SEM_REMOVE_CMD                                    \
 {"msem_remove", itlFile_MemSemRemove, FALSE, FALSE, COMMAND_MODE_ALL, \
         "Remove a memory mapped semaphore.",                          \
         "file",                                                       \
         ""}

#define ITL_FILE_OPENDIR_CMD                                          \
 {"opendir", itlFile_OpenDir, 0, FALSE, COMMAND_MODE_ALL,             \
      "Issue the opendir call.",                                      \
      "file",                                                         \
      ""}

#define ITL_FILE_READDIR_CMD                                          \
 {"readdir", itlFile_ReadDir, 0, FALSE, COMMAND_MODE_ALL,             \
      "Read an entry from an open directory.",                        \
      "file",                                                         \
      ""}

#define ITL_FILE_TELLDIR_CMD                                          \
 {"telldir", itlFile_TellDir, 0, FALSE, COMMAND_MODE_ALL,             \
      "Determine current offset into an open directory.",             \
      "file",                                                         \
      ""}

#define ITL_FILE_SEEKDIR_CMD                                          \
 {"seekdir", itlFile_SeekDir, 0, FALSE, COMMAND_MODE_ALL,             \
      "Seek to a new location to read an open directory.",            \
      "file",                                                         \
      ""}

#define ITL_FILE_REWINDDIR_CMD                                        \
 {"rewinddir", itlFile_RewindDir, 0, FALSE, COMMAND_MODE_ALL,         \
      "Set the directory read pointer to start of the directory.",    \
      "file",                                                         \
      ""}

#define ITL_FILE_CLOSEDIR_CMD                                         \
 {"closedir", itlFile_CloseDir, 0, FALSE, COMMAND_MODE_ALL,           \
      "Close an open directory.",                                     \
      "file",                                                         \
      ""}

/*
 * External data types.
 */

extern itl_commandTable_t itlFile_CommandTable[];

/*
 *  Exported Functions
 */

/*
 *  ITL_FILE_CHECK_ERROR_CODE --
 *    This checks the values returned by OS and DFS system calls.  It is
 *    intended to handle two types of error reporting.
 *      1)  Traditional OS functions that return either 0 or -1 and then
 *          set errno to communicate the true error.
 *      2)  DFS functions that return only an error code that indicates
 *          success, or the exact failure.
 */
#define ITL_FILE_CHECK_ERROR_CODE(_retval)                                  \
        MACRO_BEGIN                                                         \
          int _funcArg;                                                     \
                                                                            \
          if ((_retval) < 0)                                                \
            _funcArg = errno;                                               \
          else                                                              \
            _funcArg = (_retval);                                           \
                                                                            \
          (void) itlu_CheckResult(_funcArg);                                \
        MACRO_END                                                           \

#define ITL_FILE_NULL_RETURNED                                              \
        itl_Error("NULL string returned --\n\tFile: %s\n\tLine: %d\n",      \
		  __FILE__, __LINE__);


extern void itlFile_AddFileCommands _TAKES((
					    void
					    ));

extern boolean_t itlFile_OpenOrCreat _TAKES((
					     IN boolean_t doOpen
					     ));

extern boolean_t itlFile_Close _TAKES((
				       void
				       ));

extern boolean_t itlFile_Read _TAKES((
				      void
				      ));

extern boolean_t itlFile_Readlink _TAKES((
				      void
				      ));

extern boolean_t itlFile_Write _TAKES((
				       void
				       ));

extern boolean_t itlFile_Lseek _TAKES((
				       void
				       ));

extern boolean_t itlFile_Mknod _TAKES((
				       void
				       ));

extern boolean_t itlFile_Link _TAKES((
				      IN itlFile_linkType_t
				      ));

extern boolean_t itlFile_Unlink _TAKES((
					void
					));

extern boolean_t itlFile_Mkdir _TAKES((
				       void
				       ));

extern boolean_t itlFile_Rmdir _TAKES((
				       void
				       ));

extern boolean_t itlFile_Chdir _TAKES((
				       void
				       ));

extern boolean_t itlFile_Chmod _TAKES((
				       void
				       ));

extern boolean_t itlFile_Chown _TAKES((
				       IN int whatToChange
				       ));

extern boolean_t itlFile_Truncate _TAKES((
					  IN int doTruncate
					  ));

extern boolean_t itlFile_Rename _TAKES((
					void
					));

extern boolean_t itlFile_Utimes _TAKES((
					void
					));

extern boolean_t itlFile_Stat _TAKES((
				      IN itlFile_statType_t
				      ));

extern boolean_t itlFile_Statfs _TAKES((
					void
					));

extern boolean_t itlFile_Dup _TAKES((
				     void
				     ));

extern boolean_t itlFile_Access _TAKES((
					void
					));

extern boolean_t itlFile_Fsync _TAKES((
				       void
				       ));

extern boolean_t itlFile_Fcntl _TAKES((
				       void
				       ));

extern boolean_t itlFile_Flock _TAKES((
				       void
				       ));

extern boolean_t itlFile_MemMap _TAKES((
					void
					));

extern boolean_t itlFile_MemProtect _TAKES((
					    void
					    ));

extern boolean_t itlFile_MemSync _TAKES((
					 void
					 ));

extern boolean_t itlFile_MemUnmap _TAKES((
					  void
					  ));

extern boolean_t itlFile_MemSemInit _TAKES((
					    void
					    ));

extern boolean_t itlFile_MemSemLockOps _TAKES((
					       IN boolean_t doLock
					       ));

extern boolean_t itlFile_MemSemRemove _TAKES((
					      void
					      ));

extern boolean_t itlFile_OpenDir _TAKES((
					 void
					 ));

extern boolean_t itlFile_ReadDir _TAKES((
					 void
					 ));

extern boolean_t itlFile_TellDir _TAKES((
					 void
					 ));

extern boolean_t itlFile_SeekDir _TAKES((
					 void
					 ));

extern boolean_t itlFile_RewindDir _TAKES((
					   void
					   ));

extern boolean_t itlFile_CloseDir _TAKES((
					  void
					  ));

extern boolean_t itlFile_Umask _TAKES (( 
					void 
					));

extern boolean_t itlFile_Time _TAKES (( 
				       void 
				       ));

extern boolean_t itlFile_Sigaction _TAKES (( 
				       void 
				       ));

extern boolean_t itlFile_Alarm _TAKES (( 
				       void 
				       ));

extern boolean_t itlFile_Ulimit _TAKES ((
					 void 
					 ));

extern boolean_t itlFile_Getwd _TAKES (( 
					void 
					));

/*
 *  The flag map for lseek() is exported for the flock type to use.
 */
extern itlu_valueMap_t seekFlags[];


#endif /* ITL_FILE_H */


