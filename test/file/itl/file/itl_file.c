/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_file.c,v $
 * Revision 1.1.113.1  1996/10/17  18:25:40  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:54  damon]
 *
 * Revision 1.1.108.1  1994/02/04  20:43:40  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:45  devsrc]
 * 
 * Revision 1.1.106.1  1993/12/07  17:43:33  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:10:50  jaffe]
 * 
 * Revision 1.1.9.2  1993/07/19  19:41:38  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:31:39  zeliff]
 * 
 * Revision 1.1.7.3  1993/07/16  21:53:44  kissel
 * 	*** empty log message ***
 * 	[1993/06/21  14:55:52  kissel]
 * 
 * Revision 1.1.5.2  1993/06/04  19:03:52  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:43:06  kissel]
 * 
 * Revision 1.1.5.3  1993/04/28  18:49:23  maunsell_c
 * 	ifdef SV_INTERRUPT
 * 	[1993/04/28  18:09:22  maunsell_c]
 * 
 * Revision 1.1.3.2  1993/02/26  18:01:40  kissel
 * 	Make HPUX be like OSF for flock header files, eliminate the call to flock,
 * 	change the call to utimes to utime, and getwd to getcwd.  Also, add checks
 * 	around more of the file control flags, e.g. F_SETOWN, F_NDELAY, etc. to
 * 	improve portability.
 * 	[1993/02/26  17:57:01  kissel]
 * 
 * Revision 1.1.2.7  1993/01/28  19:45:18  shl
 * 	Transarc delta: jess-ot6685-itl-add-signal-call 1.4
 * 	  Selected comments:
 * 	    In order to test the interruptabilty of sys v lock. This will be done
 * 	    by send a signal to a process sleeping on a locked file.
 * 	    Add the function call.
 * 	    Add alarm sys call here so that we can use timer in tests.
 * 	    Add alarm function call.
 * 	    Signal call is not standared among different platforms and may not work
 * 	    in multi thread env so I changed it to "sigvec" instead.
 * 	    "sigvec" command in ITL then asks for parameters:
 * 	    SIGNUM HANDLER SIGFLAG.
 * 	    See above.
 * 	    Remove some of signal number (they are not used).
 * 	    See above.
 * 	[1993/01/27  23:02:00  shl]
 * 
 * Revision 1.1.2.6  1993/01/13  21:07:41  shl
 * 	Transarc delta: fred-ot6322-remove-bogus-code-from-telldir 1.1
 * 	  Selected comments:
 * 	    To remove bogus code that checked for an ITL variable from the
 * 	    itlFile_TellDir function.
 * 	    Remove bogus code to set an ITL variable.  Also remove un-used function
 * 	    variables in itlFile_TellDir(), itlFile_RewindDir(), itlFile_CloseDir().
 * 	Transarc delta: fred-ot6406-require-lock-var-for-fcntl 1.3
 * 	  Selected comments:
 * 	    To fix the fcntl function to require a lock variable be supplied for
 * 	    locking operations.
 * 	    This is just a checkpoint.  Got a more important defect to fix.
 * 	    Remove code that checked for the empty string ("") for a variable name for
 * 	    the lock variable.  Now you must enter the name of a previously defined
 * 	    lock variable.
 * 	    Put finishing touches on this delta.
 * 	    More cleanup of the fcntl command.
 * 	    Fixed the compile problem with flock.h on pmax.
 * 	    Include fcntl.h on pmax to get definition of struct flock.
 * 	Transarc delta: jess-ot6339-fix-itl-utimes 1.2
 * 	  Selected comments:
 * 	    ITL sysxall test access.scr is failed on PMAX. The utimes function
 * 	    passes a zero as a pointer.
 * 	    Fix this seg fault.
 * 	    Remove the ifdef endif staff sinec there is no difference for
 * 	    localtime syscall among different platforms.
 * 	    See above.
 * 	Transarc delta: jess-ot6388-fix-itl-statfs 1.1
 * 	  Selected comments:
 * 	    satafs system call causes the core dump. This fixes the problem.
 * 	    The ITL_UTIL_FREE fileName before the syscall causes the problem.
 * 	[1993/01/13  14:00:12  shl]
 * 
 * Revision 1.1.2.5  1993/01/11  18:29:42  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:07:26  htf]
 * 
 * Revision 1.1.2.4  1992/12/09  21:32:08  jaffe
 * 	Transarc delta: fred-ot-6193-move-procid-to-base-itl 1.1
 * 	  Selected comments:
 * 	    To make the process id and system page size variables available to all test
 * 	    programs but placing their initialization in the ITL library and not the
 * 	    itlFile library.
 * 	    Also incorporated fixes to allow parameters to be passed to procedures
 * 	    started in detached threads.
 * 	    Remove initialization of process id and system page size variables.
 * 	Transarc delta: fred-ot6132-check-for-short-io 1.1
 * 	  Selected comments:
 * 	    To fix the read and write commands to check for short I/O.  Previously the
 * 	    user had to to this in the ITL script by looking at the CommandResult
 * 	    variable.  This option is still available if the user sets verifyStatus to
 * 	    false.
 * 	    Changed the read and write commands to call the COMPARE_IO_BYTES macro.
 * 	    Also fixed the read routine not to free the vector variable containing
 * 	    newly read data.
 * 	Transarc delta: fred-ot6188-set-variables-correctly 1.1
 * 	  Selected comments:
 * 	    Fix problems with vector data types in the file_test program.
 * 	    Use ITL_VEC_DATA_VALUE instead of VALUE_VAL to obtain pointer to the
 * 	    vector data in the write function.
 * 	    Add a call to itlValue_Copy in the fcntl function.
 * 	Transarc delta: fred-ot6200-write-correct-strings 1.1
 * 	  Selected comments:
 * 	    Fix bug with write command, and incorporate a fix from Ofer for the vector
 * 	    data type.
 * 	    Subtract one form length of string variables as reported by VALUE_SIZE
 * 	    macro.
 * 	Transarc delta: jess-ot6107-itl-fix-merge-prob 1.1
 * 	  Selected comments:
 * 	    This delta fixes the merge problem for mktime adjustment.
 * 	    Fix the problem.
 * 	[1992/12/04  20:28:45  jaffe]
 * 
 * Revision 1.1.2.3  1992/11/18  21:45:00  jaffe
 * 	Transarc delta: fred-itl-support-binary-data 1.1
 * 	  Selected comments:
 * 	    Add support for binary data (the ITL vector type) into the read and write
 * 	    routines for the file interface library.
 * 	    Added support for vector type to read and write functions.
 * 	Transarc delta: itl-sanzi-initial-spoke-library 1.1
 * 	  Selected comments:
 * 	    Addition of Spoke Library to the suite of ITL libraries.
 * 	    Port of file_test and its libs to the Sun.
 * 	    Port this file to run on the Sun.
 * 	Transarc delta: jess-itl-add-mknod 1.1
 * 	  Selected comments:
 * 	    Add the system call mknod to ITL file interface.
 * 	    See above.
 * 	Transarc delta: jess-itl-add-more-mode-flags 1.1
 * 	  Selected comments:
 * 	    Export more mode flags, such as S_IFDIR, S_RWXO, S_IROTH ... .
 * 	    See above.
 * 	Transarc delta: jess-itl-add-time-function 1.2
 * 	  Selected comments:
 * 	    Add time call to ITL interface. The ITL variable commandResult is
 * 	    used to pass the current time in seconds back to calling script
 * 	    if the call successful.
 * 	    See above.
 * 	    Change the return variable to user variable istead of CommandResult.
 * 	    See above.
 * 	Transarc delta: jess-itl-adjust-mktime 1.1
 * 	  Selected comments:
 * 	    Currently,  ITL utimes function call will ask user providing a
 * 	    "struct tm" infor, but the syscall mktime() interprets the data
 * 	    in a different way of the other system time functionss, like
 * 	    localtime(), gmtime(), ctime(), etc. This delta make adjustment for
 * 	    this problem temporarily (dose a decreament on the tm_mon).
 * 	    See above.
 * 	Transarc delta: jess-itl-fix-makefile 1.5
 * 	  Selected comments:
 * 	    This delta is not precisely named. Do't be confused that the delta
 * 	    is opened for porting Sanzi Rich's sun based distributed ITL to the
 * 	    rios and pmax platforms and also for some bug fixes.
 * 	    See above reason.
 * 	    Fix a memory failure bug (for readdir).
 * 	    Fix a parameter passing error.
 * 	    There was an unavailable error when calling server_register_if.
 * 	    This reopen fix the problem some way.
 * 	    Chaned the definition of stat_compare function because dfs does not
 * 	    care all of stat fields.
 * 	Transarc delta: jess-ot6043-itl-assign-var 1.2
 * 	  Selected comments:
 * 	    This delta move ASSIGN_ITL_VARIABLE macro into the itl/utils/itl_utils.h
 * 	    file so that it can be referenced from itl subcomponents.
 * 	    Remove the definition of the macro.
 * 	    Change the name to ITLU_ASSIGN_VARIABLE.
 * 	    Fix pmax compiling warnings.
 * 	[1992/11/18  14:48:40  jaffe]
 * 
 * Revision 1.1.2.2  1992/10/28  17:39:29  jaffe
 * 	New file
 * 	[1992/10/27  19:59:20  jaffe]
 * 
 * $EndLog$
 */
/*
 * itl_file.c -- 
 *    Code for the basic file system interface functions.
 */
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/file.h>
#include <sys/signal.h>
#include <signal.h>

#if !defined(__OSF1__) && !defined(SUNOS5) && !defined(__hpux)
#include <sys/flock.h>
#endif /* !defined(__OSF1__) && !defined(SUNOS5) && !defined(__hpux) */

#if defined(_AIX) || defined(SUNOS5)
#include <sys/statfs.h>
#else
  #ifdef __hpux
    #include <sys/vfs.h>
    #define getwd(path)		(getcwd((path),sizeof(path)-1))
    #define utimes(fn, utp)	(utime((fn), (utp)))
  #else
    #include <sys/mount.h>
  #endif /* __hpux */
#endif  /* _AIX */

#include <ulimit.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#ifdef	SUNOS5   /* defined in mman.h, can you believe it */
#undef	SHARED
#define SHARED
#undef	PRIVATE
#define PRIVATE static
#endif

#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_file.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_stdtypes.h>
#include <utils/itl/itl_vectype.h>
#include <utils/itl/itl_fileUtils.h>
#include <utils/itl/itl_fileDir.h>
#include <utils/itl/itl_fileStat.h>
#include <utils/itl/itl_fileStatfs.h>
#include <utils/itl/itl_fileFlock.h>

#ifdef SUNOS4
int symlink();
#endif /* SUNOS4 */

/*
 *  COMPARE_IO_BYTES() --
 *    This macro is used by the read and write commands to verify that
 *    the number of bytes read/written is what was requested.  Note that
 *    it only complains if the verify status variable is set to true.
 *
 *  Arguments:
 *    _fName - (char *) name of function, i.e. read or write.
 *    _retBytes - number of bytes read/written.
 *    _askBytes - number of bytes requested to be read/written.
 */
#define COMPARE_IO_BYTES(_fName, _retBytes, _askBytes)                      \
        MACRO_BEGIN                                                         \
          if  (((_retBytes) >= 0) && ((_retBytes) != (_askBytes)) &&        \
	       itlu_IsVerifyTrue())                                         \
          {                                                                 \
	      itl_Error("%s I/O error, returned (%d), requested (%d).\n",   \
		        (_fName), (_retBytes), (_askBytes));                \
	      itlIO_TraceCurrentLocation();                                 \
          }                                                                 \
        MACRO_END
 
/*
 *  GET_FILE_IDENTIFIER() --
 *    Simple macro to prompt for and get a file identifier.  Makes it
 *    easy to change the prompt if necessary.
 *
 *  Arguments:
 *    _fidVar - variable that is assigned the file descriptor.
 */
#define GET_FILE_IDENTIFIER(_fidVar)                                        \
        itl_PromptUser("File Identifer: ");                                 \
        (_fidVar) = itl_GetInteger((boolean_t *) 0)

/*
 *  GET_DIR_POINTER() --
 *    Simple macro to prompt for and get a directory info pointer.
 *
 *  Arguments:
 *    _dirVar - variable that is assigned the directory pointer.
 */
#define GET_DIR_POINTER(_dirVar)                                            \
        itl_PromptUser("Directory Variable: ");                             \
        (_dirVar) = (DIR *) itl_GetInteger((boolean_t *) 0)

PRIVATE itl_var_t sigActionArray[31]; 

PRIVATE pthread_mutex_t sigactionMutex;
PRIVATE pthread_mutex_t utimesMutex;
PRIVATE pthread_mutex_t readdirMutex;


PRIVATE char itlFileInfoMsg[] = 
"The ITL File module provides command interfaces to the basic file system \
functions, such as open, close, read, etc.";


/*
 *  Define command table entries supported by the functions in this file.
 */
EXPORT itl_commandTable_t itlFile_CommandTable[] = {
    ITL_FILE_OPEN_CMD,
    ITL_FILE_CREAT_CMD,
    ITL_FILE_UMASK_CMD,    
    ITL_FILE_TIME_CMD,
    ITL_FILE_SIGACTION_CMD,
    ITL_FILE_ALARM_CMD,
    ITL_FILE_ULIMIT_CMD,
    ITL_FILE_GETWD_CMD,
    ITL_FILE_CLOSE_CMD,
    ITL_FILE_READLINK_CMD,
    ITL_FILE_READ_CMD,
    ITL_FILE_WRITE_CMD,
    ITL_FILE_LSEEK_CMD,
    ITL_FILE_MKNOD_CMD,
    ITL_FILE_HARD_LINK_CMD,
    ITL_FILE_SYM_LINK_CMD,
    ITL_FILE_UNLINK_CMD,
    ITL_FILE_REMOVE_CMD,
    ITL_FILE_MKDIR_CMD,
    ITL_FILE_RMDIR_CMD,
    ITL_FILE_CHDIR_CMD,
    ITL_FILE_CHMOD_CMD,
    ITL_FILE_CHOWN_CMD,
    ITL_FILE_CHOWNER_CMD,
    ITL_FILE_CHGROUP_CMD,
    ITL_FILE_TRUNCATE_CMD,
    ITL_FILE_FTRUNCATE_CMD,
    ITL_FILE_RENAME_CMD,
    ITL_FILE_UTIMES_CMD,
    ITL_FILE_STAT_CMD,
    ITL_FILE_LSTAT_CMD,
    ITL_FILE_STATFS_CMD,
    ITL_FILE_DUP_CMD,
    ITL_FILE_ACCESS_CMD,
    ITL_FILE_FSYNC_CMD,
    ITL_FILE_FCNTL_CMD,
#ifndef	SUNOS5
    ITL_FILE_FLOCK_CMD,
#endif
    ITL_FILE_MEM_MAP_CMD,
    ITL_FILE_MEM_PROTECT_CMD,
    ITL_FILE_MEM_SYNC_CMD,
    ITL_FILE_MEM_UNMAP_CMD,
    ITL_FILE_MEM_SEM_INIT_CMD,
    ITL_FILE_MEM_SEM_LOCK_CMD,
    ITL_FILE_MEM_SEM_UNLOCK_CMD,
    ITL_FILE_MEM_SEM_REMOVE_CMD,
    ITL_FILE_OPENDIR_CMD,
    ITL_FILE_READDIR_CMD,
    ITL_FILE_TELLDIR_CMD,
    ITL_FILE_SEEKDIR_CMD,
    ITL_FILE_REWINDDIR_CMD,
    ITL_FILE_CLOSEDIR_CMD
};


PRIVATE itlu_valueMap_t openFlags[] = {
    /*
     *  Standard flags first -- supported by all platforms.
     */
    {"O_APPEND",   O_APPEND},
    {"O_CREAT",    O_CREAT},
#ifdef O_DEFER    
    {"O_DEFER",    O_DEFER},
#endif    
    {"O_EXCL",     O_EXCL},
    {"O_NDELAY",   O_NDELAY},
    {"O_NOCTTY",   O_NOCTTY},
    {"O_NONBLOCK", O_NONBLOCK},
    {"O_RDONLY",   O_RDONLY},
    {"O_RDWR",     O_RDWR},
    {"O_SYNC",     O_SYNC},
    {"O_TRUNC",    O_TRUNC},
    {"O_WRONLY",   O_WRONLY},

    /*
     *  Platform dependent flags now.
     */
#ifdef _AIX
    {"O_DELAY",    O_DELAY},
    {"O_EXEC",     O_EXEC},
    {"O_NONE",     O_NONE},
    {"O_NSHARE",   O_NSHARE},
    {"O_RSHARE",   O_RSHARE},
#endif  /* _AIX */

    {NULL,         0}
};

PRIVATE itlu_valueMap_t fcntlFlags[] = {
    {"F_DUPFD",  F_DUPFD},
    {"F_GETFD",  F_GETFD},
    {"F_SETFD",  F_SETFD},
    {"F_GETFL",  F_GETFL},
    {"F_SETFL",  F_SETFL},
#ifdef F_GETOWN
    {"F_GETOWN", F_GETOWN},
#endif
#ifdef F_SETOWN
    {"F_SETOWN", F_SETOWN},
#endif
    {"F_GETLK",  F_GETLK},
    {"F_SETLK",  F_SETLK},
    {"F_SETLKW", F_SETLKW},

#ifdef _AIX
    {"F_CLOSEM", F_CLOSEM},
#endif  /* _AIX */

    {NULL,       0}
};

PRIVATE itlu_valueMap_t ulimitFlags[] = {

#ifdef _AIX
    {"GET_FSIZE", GET_FSIZE},
    {"SET_FSIZE", SET_FSIZE},
#endif  /* _AIX */

#if defined(__OSF1__) || defined(SUNOS5) || defined(__hpux)
    {"GET_FSIZE", UL_GETFSIZE},
    {"SET_FSIZE", UL_SETFSIZE},
#endif  /* defined(__OSF1__) || defined(SUNOS5) || defined(__hpux) */

    {NULL,       0}
};

PRIVATE itlu_valueMap_t setStatusFlags[] = {
    {"FAPPEND",   FAPPEND},
    {"FNDELAY",   FNDELAY},
#ifdef FNONBLOCK    
    {"FNONBLOCK", FNONBLOCK},
#else
#ifdef FNBLOCK
    {"FNBLOCK", FNBLOCK},
#endif
#endif    
#ifdef FSYNC
    {"FSYNC",     FSYNC},
#endif
#ifdef FASYNC
    {"FASYNC",    FASYNC},
#endif
    {NULL,        0}
};

PRIVATE itlu_valueMap_t setDescriptorFlags[] = {
    {"FD_CLOEXEC",   FD_CLOEXEC},
    {NULL,           0}
};

#ifndef	SUNOS5
PRIVATE itlu_valueMap_t flockFlags[] = {
#ifdef LOCK_SH
    {"LOCK_SH", LOCK_SH},
#endif
#ifdef LOCK_EX
    {"LOCK_EX", LOCK_EX},
#endif
#ifdef LOCK_NB
    {"LOCK_NB", LOCK_NB},
#endif
#ifdef LOCK_UN
    {"LOCK_UN", LOCK_UN},
#endif
    {NULL,      0}
};
#endif /* ifndef SUNOS5 */

/*
 *  seekFlags - is the table of flags for the whence parameter to lseek
 *
 *  NOTE:  These flags are exported for the flock type to use.
 */

SHARED itlu_valueMap_t seekFlags[] = {
    {"SEEK_SET", SEEK_SET},
    {"SEEK_CUR", SEEK_CUR},
    {"SEEK_END", SEEK_END},
    {NULL,       0}
};

/*
 *  chmodFlags - is the table of flags for the chmod function.
 */
PRIVATE itlu_valueMap_t chmodFlags[] = {
    {"S_ISUID",  S_ISUID},
    {"S_ISGID",  S_ISGID},
    {"S_ISVTX",  S_ISVTX},
    {"S_ENFMT",  S_ENFMT},
    {"S_IRUSR",  S_IRUSR},
    {"S_IWUSR",  S_IWUSR},
    {"S_IXUSR",  S_IXUSR},
    {"S_IRGRP",  S_IRGRP},
    {"S_IWGRP",  S_IWGRP},
    {"S_IXGRP",  S_IXGRP},
    {"S_IROTH",  S_IROTH},
    {"S_IWOTH",  S_IWOTH},
    {"S_IXOTH",  S_IXOTH},
    {"S_IRWXO",  S_IRWXO},
    {"S_IFDIR",  S_IFDIR},
    {"S_IFCHR",  S_IFCHR},
    {"S_IFBLK",  S_IFBLK},
    {"S_IFREG",  S_IFREG},
    {"S_IFLNK",  S_IFLNK},
    {"S_IFSOCK", S_IFSOCK},
    {"S_IFIFO",  S_IFIFO},
    {NULL,       0}
};

/*
 *  accessFlags - is the table of flags for the access() call.
 */
PRIVATE itlu_valueMap_t accessFlags[] = {
#ifdef R_ACC
    {"R_ACC",  R_ACC},
#endif
#ifdef W_ACC    
    {"W_ACC",  W_ACC},
#endif
#ifdef X_ACC    
    {"X_ACC",  X_ACC},
#endif
#ifdef E_ACC    
    {"E_ACC",  E_ACC},
#endif
#ifdef NO_ACC    
    {"NO_ACC", NO_ACC},
#endif
#ifdef	R_OK    
    {"R_OK",  R_OK},
#endif
#ifdef W_OK
    {"W_OK",  W_OK},
#endif
#ifdef X_OK
    {"X_OK",  X_OK},
#endif
#ifdef F_OK
    {"F_OK",  F_OK},
#endif
    {NULL,     0}
};

/*
 *  mmapProtect - is the table of protection flags for memory mapped files.
 */
PRIVATE itlu_valueMap_t mmapProtectFlags[] = {
    {"PROT_NONE",  PROT_NONE},
    {"PROT_READ",  PROT_READ},
    {"PROT_WRITE", PROT_WRITE},
    {"PROT_EXEC",  PROT_EXEC},
    {NULL,         0}
};

/*
 *  mmapFlags - is the table of flags for memory mapped files.
 */
PRIVATE itlu_valueMap_t mmapFlags[] = {
    {"MAP_SHARED",     MAP_SHARED},
    {"MAP_PRIVATE",    MAP_PRIVATE},
#ifdef MAP_FILE    
    {"MAP_FILE",       MAP_FILE},
#endif
#ifdef MAP_ANONYMOUS    
    {"MAP_ANONYMOUS",  MAP_ANONYMOUS},
#endif    
#ifdef MAP_TYPE
    {"MAP_TYPE",       MAP_TYPE},
#endif
    {"MAP_FIXED",      MAP_FIXED},
#ifdef MAP_VARIABLE    
    {"MAP_VARIABLE",   MAP_VARIABLE},
#endif    
    {NULL,             0}
};

/*
 *  mmapSyncFlags - is the table of flags for synchronizing memory mapped 
 *                  files.
 */
PRIVATE itlu_valueMap_t mmapSyncFlags[] = {
    {"MS_ASYNC",      MS_ASYNC},
#ifdef MS_SYNC    
    {"MS_SYNC",       MS_SYNC},
#endif    
    {"MS_INVALIDATE", MS_INVALIDATE},
    {NULL,            0}
};

/*
 *  msemFlags - is the table of flags for memory semaphores.
 */
PRIVATE itlu_valueMap_t msemFlags[] = {
#ifdef MSEM_UNLOCKED    
    {"MSEM_UNLOCKED",   MSEM_UNLOCKED},
    {"MSEM_LOCKED",     MSEM_LOCKED},
    {"MSEM_IF_NOWAIT",  MSEM_IF_NOWAIT},
    {"MSEM_IF_WAITERS", MSEM_IF_WAITERS},
#endif    
    {NULL,              0}
};

/*
 *  signalFlags 
 */
PRIVATE itlu_valueMap_t signalFlags[] = {
    {"SIGALRM",  SIGALRM},
    {"SIGUSR1",  SIGUSR1},
    {"SIGUSR2",  SIGUSR2},
    {NULL,         0}
};
/*
 *  signalFlags 
 */
PRIVATE itlu_valueMap_t sigactionFlags[] = {
    {"SA_ONSTACK",   SA_ONSTACK},
#ifdef SA_RESTART
    {"SA_RESTART",   SA_RESTART},
#endif
#ifdef SV_INTERRUPT
    {"SV_INTERRUPT",  SV_INTERRUPT},
#endif
#ifdef SV_ONSTACK
    {"SV_ONSTACK",  SV_ONSTACK},
#endif
#ifdef SUNOS5
    {"SA_SIGINFO",   SA_SIGINFO},
    {"SA_RESETHAND", SA_RESETHAND},
    {"SA_NODEFER",   SA_NODEFER},
    {"SA_NOCLDWAIT", SA_NOCLDWAIT},
    {"SA_WAITSIG",   SA_WAITSIG},
#endif  /* SUNOS5 */
    {NULL,           0}
};

/*
 *  AddFileCommands() --
 *    Private function that really adds the file commands.  Necessary to
 *    accomodate how pthreads does one time initialization.
 */
PRIVATE void AddFileCommands()
{
    long returnCode = 0;

    /*
     *  Ensure all name tables are initialized.
     */
    if (itlu_Init())
    {
	itl_Error("ITL utilities did not initialize, exiting.\n");
	exit(-1);
    }
    itlFile_DirInit();
    itlFile_StatInit();
    itlFile_FlockInit();
    itlFile_StatfsInit();

    /*
     *  Turn all of the flags for various file interfaces into ITL
     *  variables.  Yeah, the code doesn't stop at the first failure
     *  but this thing just should not fail.  If it does, well we
     *  wouldn't want the debugger writers to have nothing to do.
     */
    returnCode = itlu_ProcessValueMap(openFlags, NULL);
    returnCode = itlu_ProcessValueMap(fcntlFlags, NULL);
    returnCode = itlu_ProcessValueMap(setStatusFlags, NULL);
    returnCode = itlu_ProcessValueMap(setDescriptorFlags, NULL);
#ifndef SUNOS5
    returnCode = itlu_ProcessValueMap(flockFlags, NULL);
#endif  /* SUNOS5 */
    returnCode = itlu_ProcessValueMap(ulimitFlags, NULL);
    returnCode = itlu_ProcessValueMap(seekFlags, NULL);
    returnCode = itlu_ProcessValueMap(chmodFlags, NULL);
    returnCode = itlu_ProcessValueMap(accessFlags, NULL);
    returnCode = itlu_ProcessValueMap(mmapProtectFlags, NULL);
    returnCode = itlu_ProcessValueMap(mmapFlags, NULL);
    returnCode = itlu_ProcessValueMap(mmapSyncFlags, NULL);
    returnCode = itlu_ProcessValueMap(msemFlags, NULL);
    returnCode = itlu_ProcessValueMap(signalFlags, NULL);
    returnCode = itlu_ProcessValueMap(sigactionFlags, NULL);
    if(returnCode)
    {
	itl_Error("itlu_ProcessValueMap FAILED - %ld\n", returnCode);
	exit(-1);
    }

    (void) itl_AddModuleCmds("file",
			     itlFile_CommandTable,
			     sizeof(itlFile_CommandTable) / 
			     sizeof(itl_commandTable_t),
			     COMMAND_MODE_ALL,
			     itlFileInfoMsg);

    pthread_mutex_init(&utimesMutex, pthread_mutexattr_default);
    pthread_mutex_init(&readdirMutex, pthread_mutexattr_default);
    pthread_mutex_init(&sigactionMutex, pthread_mutexattr_default);
}

/*
 *  itlFile_AddFileCommands() -
 *    Do everything necessary to make the itlFile commands accessable via 
 *    the testing interface.
 */
SHARED void itlFile_AddFileCommands()
{
    static pthread_once_t addFileOnce = pthread_once_init;

    itlFile_utilInit();
    if (pthread_once(&addFileOnce, (pthread_initroutine_t) AddFileCommands))
	itl_Error("pthread_once failed in AddFileCommands\n");
}

/*
 *  itlFile_OpenOrCreat() --
 *    Issue the open() or creat() system call.
 */
SHARED boolean_t itlFile_OpenOrCreat(doOpen)
  IN boolean_t doOpen;   /*  TRUE issue open(), else creat() */
{
    char *fileName, *varName;
    int flags, mode;
    int eof, wordSize;
    int returnCode;
    boolean_t err;

    itl_PromptUser("Filename: ");
    if ((fileName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	if (doOpen)
	{
	    itl_PromptUser("Flags ($O_* flags): ");
	    flags = itl_GetInteger((boolean_t *) 0);
	}

	itl_PromptUser("Mode bits (0x# [hex], 0# [octal], # [decimal]): ");
	mode = itl_GetInteger((boolean_t *) 0);

	/*
	 *  Prompt for the variable name to hold the file descriptor.
	 */
	itl_PromptUser("Variable Name: ");
	varName = itl_GetVariableName(&err, NULL);

	if (doOpen)
	    returnCode = open(fileName, flags, mode);
	else
	    returnCode = creat(fileName, mode);
	ITL_FILE_CHECK_ERROR_CODE((returnCode < 0) ? returnCode : 0);

	/*
	 *  If the call was successful, then assign the return 
	 *  value to the user supplied variable.
	 */
	if (returnCode >= 0)
	{
	    ITLU_ASSIGN_VARIABLE(varName, &returnCode, itl_intType);
	}
	ITL_UTIL_FREE(varName);
	ITL_UTIL_FREE(fileName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
 *  itlFile_Umask() --
 *    Issue the umask() system call.
 */
SHARED boolean_t itlFile_Umask()
{
    mode_t  maskBits;
    int eof, wordSize;
    int returnCode;

    itl_PromptUser("Mask bits (0x# [hex], 0# [octal], # [decimal]): ");
    maskBits = itl_GetInteger((boolean_t *) 0);
    returnCode = umask(maskBits);
    ITL_FILE_CHECK_ERROR_CODE((returnCode < 0 ) ? returnCode : 0);
    itl_SetIntResult(&commandResult, returnCode);
    return(FALSE);
}
/*
 *  itlFile_Time() --
 *    Issue the time() call.
 */
SHARED boolean_t itlFile_Time()
{
    int eof, wordSize;
    time_t currentTime;
    char *varName;
    boolean_t err;

    itl_PromptUser("Variable Name: ");
    if ((varName = itl_GetString(&eof, &wordSize)) != NULL)
    {
	currentTime = time((time_t *) 0);
	ITLU_ASSIGN_VARIABLE(varName, &currentTime, itl_intType);
	ITL_UTIL_FREE(varName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}


void itl_sigUsrAction(sig)
  int sig;
{
   itl_var_t *procVar;
   
   itl_AdviseUser(" Signal caught %d\n",sig);
   procVar = &sigActionArray[sig-1];
   itl_ExecuteVar((pointer_t)procVar);
   return;
}


/*
 *  itlFile_Sigaction() --
 *    Issue the sigaction() call.
 */
SHARED boolean_t itlFile_Sigaction()
{
    int returnCode;
    char *procName;
    itl_var_t *procVar;
    int err, vlen;
    int signalNum;
    int sigActFlag;
#if defined(__hpux) || defined(__OSF1__)
    struct sigvec act;
    struct sigvec oact;
#else
#if defined(_AIX) && defined(sigaction)
#undef sigaction
#endif
    struct sigaction act;
    struct sigaction oact;
#endif

    /*
     * This function call will always assume that the signal is
     * SIGUSR1 and the action is NULL.
     * The major purpose of this call is to test the interruptabilty
     * of blocking lock.
     */
     itl_PromptUser("Input signals ($SIG__ -- flags): ");
     signalNum = itl_GetInteger((boolean_t *) 0);

     itl_PromptUser("Input handler (A string or proc variable name): ");
     procName = itl_GetVariableName(&err, &vlen);
     procVar = itl_LocateVariable(procName);

     pthread_mutex_lock(&sigactionMutex);
     sigActionArray[signalNum-1]= *procVar;
     pthread_mutex_unlock(&sigactionMutex);

     itl_PromptUser("Input sigaction flags ($SA__-- flags): ");
     sigActFlag = itl_GetInteger((boolean_t *) 0);
#if defined(__hpux) || defined(__OSF1__)
     act.sv_handler = itl_sigUsrAction;
     act.sv_mask = 0;
     act.sv_flags = sigActFlag; 

     oact.sv_handler = SIG_DFL;
     oact.sv_mask = 0;
     oact.sv_flags = 0;

     returnCode = sigvec(signalNum, &act, &oact);
#else
     act.sa_handler = itl_sigUsrAction;
     sigemptyset(&act.sa_mask);
     act.sa_flags = sigActFlag; 

     oact.sa_handler = SIG_DFL;
     sigemptyset(&oact.sa_mask);
     oact.sa_flags = 0;

     returnCode = sigaction(signalNum, &act, &oact);
#endif

     ITL_FILE_CHECK_ERROR_CODE(returnCode);

     ITL_FREE(procName);
     return(FALSE);
}
/*
 *  itlFile_Alarm() --
 *    Issue the alarm() call.
 */
SHARED boolean_t itlFile_Alarm()
{
    int returnCode, timeOut;

    /*
     * This function call will send SIGALARM to a process which
     * has been blocked.
     * Use this call with signal call to test the interruptabilty
     * of blocking lock.
     */
     itl_PromptUser("Set the timer (in second) : ");
     timeOut = itl_GetInteger((boolean_t *) 0);
     returnCode = alarm((unsigned int) timeOut);
     ITL_FILE_CHECK_ERROR_CODE((returnCode<0)?0:0);
     return(FALSE);
}

/*
 *  itlFile_Ulimit() --
 *    Issue the ulimit() system call.
 */
SHARED boolean_t itlFile_Ulimit()
{
    int eof, wordSize;
    int ulimitCommand;
    long newLimit = 0;
    boolean_t err;
    int returnCode;

#ifdef SUNOS4
    itl_Error("ulimit not implemented on SUN.\n");
#else /* SUNOS4 */
    
    itl_PromptUser("Command ($GET_FSIZE or $SET_FSIZE): ");
    ulimitCommand = itl_GetInteger((boolean_t *) 0);

#ifdef _AIX
    if ( ulimitCommand == SET_FSIZE )
#else
    if ( ulimitCommand == UL_SETFSIZE )
#endif

    {
	itl_PromptUser("New limit: ");
	newLimit = (long) itl_GetInteger((boolean_t *) 0);
    }
    returnCode = ulimit( ulimitCommand, newLimit);
    ITL_FILE_CHECK_ERROR_CODE((returnCode < 0) ? returnCode : 0);
    itl_SetIntResult(&commandResult, returnCode);

#endif    /* SUNOS4 */

    return(FALSE);
}

#ifndef PATH_MAX
#include <sys/param.h>
#endif
/*
 *  itlFile_Getwd() --
 *    Issue the getwd() system call.
 */
SHARED boolean_t itlFile_Getwd()
{
    char *pathName, *varName, *retVal;
    int eof, wordSize, bytesToRead;
    boolean_t err;
#ifndef PATH_MAX
#define PATH_MAX MAXPATHLEN
#endif    
    ITL_UTIL_ALLOC(pathName, char *, PATH_MAX + 1);
    
    itl_PromptUser("Buffer Variable for Pathname: ");
    /*
     *  Check for the empty string ("").  If there then user is not
     *  interested in keeping the data.
     */
    varName = itl_PeekString(&eof, &wordSize);
    if (strcmp(varName, ""))
    {
	ITL_UTIL_FREE(varName);
	varName = itl_GetVariableName(&err, NULL);
    }
    else
    {
	ITL_UTIL_FREE(varName);
	/*
	 *  Clear the "" from the input stream.
	 */
	varName = itl_GetString(&eof, &wordSize);
    }
#ifndef	SUNOS5
    retVal = (char *) getwd(pathName);
#else
    retVal = (char *) getcwd(pathName,PATH_MAX);
#endif
    ITL_FILE_CHECK_ERROR_CODE(retVal ? 0 : -1);
    if (retVal && strcmp(varName, ""))
	ITLU_ASSIGN_VARIABLE(varName, pathName, itl_stringType);

    ITL_UTIL_FREE(varName);
    ITL_UTIL_FREE(pathName);

    return(FALSE);
}


/*
 *  itlFile_Close() --
 *    Issue the close() system call.
 */
SHARED boolean_t itlFile_Close()
{
    int fileId, returnCode;
    int eof, wordSize;

    GET_FILE_IDENTIFIER(fileId);
    returnCode = close(fileId);
    ITL_FILE_CHECK_ERROR_CODE(returnCode);
    return(FALSE);
}

/*
 *  itlFile_Readlink() --
 *    Issue the readlink() system call.
 */
SHARED boolean_t itlFile_Readlink()
{
    int returnCode;
    char *pathName, *readBuffer, *bufferName;
    int eof, wordSize;
    boolean_t err;

    itl_PromptUser("Filename: ");
    if ((pathName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	ITL_UTIL_ALLOC(readBuffer, char *, PATH_MAX + 1);
	itl_PromptUser("Buffer Variable: ");

	/*
	 *  Check for the empty string ("").  If there then user is not
	 *  interested in keeping the data.
	 */
	bufferName = itl_PeekString(&eof, &wordSize);
	if (strcmp(bufferName, ""))
	{
	    ITL_UTIL_FREE(bufferName);
	    bufferName = itl_GetVariableName(&err, NULL);
	}
	else
	{
	    ITL_UTIL_FREE(bufferName);
	    /*
	     *  Clear the "" from the input stream.
	     */
	    bufferName = itl_GetString(&eof, &wordSize);
	}
	returnCode = readlink(pathName, readBuffer, PATH_MAX + 1);
	ITL_FILE_CHECK_ERROR_CODE((returnCode < 0) ? returnCode : 0);
	
	/*
	 *  The commandResult variable is set on every call to the
	 *  value returned by readlink().  It can be used to obtain
	 *  the length of the linkname that was read in.
	 */
	itl_SetIntResult(&commandResult, returnCode);

	/*
	 *  If bufferName is NULL then the user did not enter a variable
	 *  name for the data.  So we just end up discarding it.  
	 */
	if ((returnCode >= 0) && strcmp(bufferName, "")
	    && returnCode <= PATH_MAX) {
	    /* readlink() does not null-terminate its result, so we have to do it. */
	    readBuffer[returnCode] = '\0';
	    ITLU_ASSIGN_VARIABLE(bufferName, readBuffer, itl_stringType);
	}

	ITL_UTIL_FREE(bufferName);
	ITL_UTIL_FREE(readBuffer);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
*  itlFile_Read() --
 *    Issue the read() system call.
 */
SHARED boolean_t itlFile_Read()
{
    int returnCode, fileId;
    char *readBuffer, *bufferName;
    unsigned int bytesToRead, bytesToAlloc;
    int eof, wordSize, dataType;
    boolean_t err;

    GET_FILE_IDENTIFIER(fileId);
    itl_PromptUser("Number bytes to read: ");
    bytesToRead = (unsigned int) itl_GetInteger((boolean_t *) 0);
    bytesToAlloc = bytesToRead;
    itl_PromptUser("Buffer Variable: ");

    /*
     *  Check for the empty string ("").  If there then user is not
     *  interested in keeping the data.
     */
    bufferName = itl_PeekString(&eof, &wordSize);
    if (strcmp(bufferName, ""))
    {
	itl_var_t *bufferVarP;

	ITL_UTIL_FREE(bufferName);
	bufferName = itl_GetVariableName(&err, NULL);

	/*
	 *  Check if the variable exist.  If it does then use it's
	 *  type when we assign the value later.  Otherwise, the default
	 *  is to use the byte array data type, 'vector';
	 */
	if (bufferVarP = itl_LocateVariable(bufferName))
	    dataType = VAR_TYPE(bufferVarP);
	else
	    dataType = itl_vectorType;

	/*
	 *  If the dataType is a string then add one to the value entered
	 *  by the user to account for the end of string character.
	 */
	if (dataType == itl_stringType)
	    bytesToAlloc++;
    }
    else
    {
	ITL_UTIL_FREE(bufferName);
	/*
	 *  Clear the "" from the input stream.
	 */
	bufferName = itl_GetString(&eof, &wordSize);
    }

    assert(bytesToAlloc);
    ITL_UTIL_ALLOC(readBuffer, char *, bytesToAlloc);
    returnCode = read(fileId, readBuffer, bytesToRead);

    COMPARE_IO_BYTES("Read", returnCode, bytesToRead);

    /*
     *  Special processing for read call since if successful it
     *  can be >= 0.  So if it is not  -1 we set the value sent to
     *  the ITL_FILE_CHECK_ERROR_CODE macro to be 0.
     */
    ITL_FILE_CHECK_ERROR_CODE((returnCode < 0) ? returnCode : 0);

    /*
     *  The commandResult variable is set on every call to the
     *  value returned by read().  It can be used to validate if 
     *  the number of bytes read equals what was requested.
     */
    itl_SetIntResult(&commandResult, returnCode);

    /*
     *  If bufferName is NULL then the user did not enter a variable
     *  name for the data.  So we just end up discarding it.  
     */
    if ((returnCode >= 0) && strcmp(bufferName, ""))
    {
	if (dataType == itl_stringType) 
	{
	    readBuffer[bytesToRead] = '\0';
	    ITLU_ASSIGN_VARIABLE(bufferName, readBuffer, dataType);
	}
	else 
	{
	    var_val_t *newVectorP;
            
	    /*
	     *  vector types requires special consideration since it's
	     *  size cannot be determined from the data it encapsulates.
	     */
	    newVectorP = itl_SetVectorType(readBuffer, bytesToRead);
	    if (!itl_SetVariable(bufferName, newVectorP, var_local))
		itl_Error("Failed to set variable, %s\n", bufferName);

            itlValue_Free(newVectorP);
	    ITL_UTIL_FREE(newVectorP);
	}
    }

    ITL_UTIL_FREE(bufferName);
    ITL_UTIL_FREE(readBuffer);
	
    return(FALSE);
}

/*
 *  itlFile_Write() --
 *    Issue the write() system call.
 */
SHARED boolean_t itlFile_Write()
{
    int returnCode, fileId, size;
    char *dataP;
    var_val_t *writeVarP;

    GET_FILE_IDENTIFIER(fileId);
    itl_PromptUser("Data: ");

    /*
     *  Ideally we should just invoke the itlValue_Read() function with 
     *  the vector data type.  However, that issues a prompt for 
     *  "vector" which may be confusing.  So the following code is here
     *  to avoid confusing the user.
     */
    writeVarP = itl_GetVarVal();
    assert(writeVarP);
    if ((VALUE_TYPE(writeVarP) == itl_stringType) ||
	(VALUE_TYPE(writeVarP) == itl_vectorType))
    {
	if (VALUE_TYPE(writeVarP) == itl_stringType) 
	{
	    dataP = VALUE_VAL(writeVarP);
	    size = VALUE_SIZE(writeVarP) - 1;
	}
	else
	{
	    dataP = (char *) itlVector_Value(writeVarP);
	    size = VALUE_SIZE(writeVarP);
	}
	returnCode = write(fileId, dataP, size);
	COMPARE_IO_BYTES("Write", returnCode, size);
	ITL_FILE_CHECK_ERROR_CODE((returnCode < 0) ? returnCode : 0);
	/*
	 *  The commandResult variable is set on every call to the
	 *  value returned by write().  It can be used to validate if 
	 *  the number of bytes written equals what was requested.
	 */
	itl_SetIntResult(&commandResult, returnCode);
    }
    else
	itl_Error("Can only write strings or vectors.\n");
    
    itl_FreeVarVal(writeVarP);

    return(FALSE);
}

/*
 *  itlFile_Lseek() --
 *    Issue the lseek() system call.
 */
SHARED boolean_t itlFile_Lseek()
{
    int fileId;
    long offset, returnCode;
    int whence;

    GET_FILE_IDENTIFIER(fileId);
    itl_PromptUser("Whence (choose $SEEK_SET, $SEEK_CUR, or $SEEK_END): ");
    whence = itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("File Offset: ");
    offset = (long) itl_GetInteger((boolean_t *) 0);
    returnCode = lseek(fileId, offset, whence);
    ITL_FILE_CHECK_ERROR_CODE((returnCode < 0) ? (int) returnCode : 0);
    /*
     *  The commandResult variable is set on every call to the
     *  value returned by lseek().  It can be used to determine the
     *  current location in the file.
     */
    itl_SetIntResult(&commandResult, (int) returnCode);

    return(FALSE);
}

/*
 *  itlFile_Mknod() --
 *    Issue the mknod() system call.
 */
SHARED boolean_t itlFile_Mknod()
{
    int fileId;
    long fileType, permBits, returnCode;
    int dev;
    int eof, wordSize;
    char *pathName;


    itl_PromptUser("Path name: ");
    if ((pathName = itl_GetString(&eof, &wordSize)) != NULL) 
    {

        itl_PromptUser("File Type (choose $S_IFCHR, $S_IFBLK, $S_IFREG, $S_IFIFO): ");
        fileType = itl_GetInteger((boolean_t *) 0);
        itl_PromptUser("Permission Bits ( Octal or $S_* )): ");
        permBits = itl_GetInteger((boolean_t *) 0);
        itl_PromptUser("dev (numeric value): ");
        dev = (int) itl_GetInteger((boolean_t *) 0);
        returnCode = mknod(pathName, fileType|permBits, dev);
        ITL_FILE_CHECK_ERROR_CODE((returnCode < 0) ? (int) returnCode : 0);
        /*
         *  The commandResult variable is set on every call to the
         *  value returned by mknod(). The -1 or 0 will be returned.
         */
        itl_SetIntResult(&commandResult, (int) returnCode);
	ITL_UTIL_FREE(pathName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
 *  itlFile_Link() --
 *    Issue the link() symlink() system call.
 */
SHARED boolean_t itlFile_Link(linkType)
  IN itlFile_linkType_t linkType;
{
    int returnCode;
    char *path1, *path2;
    int eof, wordSize;
    int (*linkFunc)(const char *, const char *);

    linkFunc = (linkType == HardLink)
      ? (int (*)(const char *, const char *))link
	: (int (*)(const char *, const char *))symlink;

    itl_PromptUser("File to link to: ");
    if ((path1 = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	itl_PromptUser("Link name: ");
	if ((path2 = itl_GetString(&eof, &wordSize)) != NULL) 
	{
	    returnCode = linkFunc(path1, path2);
	    ITL_FILE_CHECK_ERROR_CODE(returnCode);
	    ITL_UTIL_FREE(path2);
	}
	else
	    ITL_FILE_NULL_RETURNED;

	ITL_UTIL_FREE(path1);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
 *  itlFile_Unlink() --
 *    Issue the unlink() system call.
 */
SHARED boolean_t itlFile_Unlink()
{
    int returnCode;
    char *fileName;
    int eof, wordSize;

    itl_PromptUser("File to remove: ");
    if ((fileName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	returnCode = unlink(fileName);
	ITL_FILE_CHECK_ERROR_CODE(returnCode);
	ITL_UTIL_FREE(fileName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
 *  itlFile_Mkdir() --
 *    Issue the mkdir() system call.
 */
SHARED boolean_t itlFile_Mkdir()
{
    char *dirName;
    int mode;
    int eof, wordSize;
    int returnCode;

    itl_PromptUser("Directory Name: ");
    if ((dirName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
        itl_PromptUser("Mode bits (0x# [hex], 0# [octal], # [decimal]): ");
        mode = itl_GetInteger((boolean_t *) 0);
	returnCode = mkdir(dirName, mode);
	ITL_FILE_CHECK_ERROR_CODE(returnCode);
	ITL_UTIL_FREE(dirName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
 *  itlFile_Rmdir() --
 *    Issue the rmdir() system call.
 */
SHARED boolean_t itlFile_Rmdir()
{
    char *dirName;
    int eof, wordSize;
    int returnCode;

    itl_PromptUser("Directory Name: ");
    if ((dirName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	returnCode = rmdir(dirName);
	ITL_FILE_CHECK_ERROR_CODE(returnCode);
	ITL_UTIL_FREE(dirName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
 *  itlFile_Chdir() --
 *    Issue the chdir() system call.
 */
SHARED boolean_t itlFile_Chdir()
{
    char *dirName;
    int eof, wordSize;
    int returnCode;

    itl_PromptUser("Directory Name: ");
    if ((dirName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	returnCode = chdir(dirName);
	ITL_FILE_CHECK_ERROR_CODE(returnCode);
	ITL_UTIL_FREE(dirName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
 *  itlFile_Chmod() --
 *    Issue the chmod() system call.
 */
SHARED boolean_t itlFile_Chmod()
{
    char *fileName, *modeStrg;
    int mode;
    int eof, wordSize;
    int returnCode;

    itl_PromptUser("Filename: ");
    if ((fileName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	itl_PromptUser("Mode bits ($S_* flags): ");
	mode = itl_GetInteger((boolean_t *) 0);
	returnCode = chmod(fileName, mode);
	ITL_FILE_CHECK_ERROR_CODE(returnCode);
	ITL_UTIL_FREE(fileName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}


/*
 *  itlFile_Chown() --
 *    Issue the chown() system call.  The function parameter has the 
 *    following meaning.
 *      0       - change both the user and group id's
 *      Userid  - change just the user id
 *      GroupId - change just the group id
 */
SHARED boolean_t itlFile_Chown(whatToChange)
  IN int whatToChange;
{
    char *fileName;
    int ownerId, groupId;
    int eof, wordSize;
    int returnCode;
    boolean_t okToExecute = TRUE;

    itl_PromptUser("Filename: ");
    if ((fileName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	switch (whatToChange) {
	  case 0:   /* change both the user and group identifiers */
	      if (((ownerId = itlFile_GetIdentifier(UserId)) 
		   == ITL_FILE_INVALID_ID)	             ||
		  ((groupId = itlFile_GetIdentifier(GroupId)) 
		   == ITL_FILE_INVALID_ID))
		  okToExecute = FALSE;
	      break;

	  case UserId:
	      groupId = -1;
	      if ((ownerId = itlFile_GetIdentifier(UserId)) 
		  == ITL_FILE_INVALID_ID)
		  okToExecute = FALSE;
	      break;

	  case GroupId:
	      ownerId = -1;
	      if ((groupId = itlFile_GetIdentifier(GroupId)) 
		  == ITL_FILE_INVALID_ID)
		  okToExecute = FALSE;
	      break;
	  default:
	      okToExecute = FALSE;
	      itl_Error("Invalid parameter to itlFile_Chown, %d\n", 
			whatToChange);
	}
	if (okToExecute)
	{
	    returnCode = chown(fileName, ownerId, groupId);
	    ITL_FILE_CHECK_ERROR_CODE(returnCode);
	}
	ITL_UTIL_FREE(fileName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
 *  itlFile_Truncate() --
 *    Issue the truncate() system call or the
 *    ftruncate() system call depending on the doTruncate flag.
 */
SHARED boolean_t itlFile_Truncate(doTruncate)
  IN boolean_t doTruncate;
{
    int returnCode, fileId;
    char *fileName;
    int eof, wordSize;
    long offset;

    if (doTruncate) 
    {
	itl_PromptUser("Filename: ");
	if ((fileName = itl_GetString(&eof, &wordSize)) == NULL) 
	{
	    ITL_FILE_NULL_RETURNED;
	}
    }
    else
    {
	GET_FILE_IDENTIFIER(fileId);
    }

    itl_PromptUser("File Offset: ");
    offset = (long) itl_GetInteger((boolean_t *) 0);
    if (doTruncate)
    {
	returnCode = truncate(fileName, offset);
	ITL_UTIL_FREE(fileName);
    }
    else
	returnCode = ftruncate(fileId, offset);

    ITL_FILE_CHECK_ERROR_CODE(returnCode);

    return(FALSE);
}

/*
 *  itlFile_Rename() --
 *    Issue the rename() system call.
 */
SHARED boolean_t itlFile_Rename()
{
    int returnCode;
    char *oldName, *newName;
    int eof, wordSize;

    itl_PromptUser("Old Name: ");
    if ((oldName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	itl_PromptUser("New Name: ");
	if ((newName = itl_GetString(&eof, &wordSize)) != NULL) 
	{
	    returnCode = rename(oldName, newName);
	    ITL_FILE_CHECK_ERROR_CODE(returnCode);
	    ITL_UTIL_FREE(newName);
	}
	else
	    ITL_FILE_NULL_RETURNED;
	ITL_UTIL_FREE(oldName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
 *  itlFile_Utimes() --
 *    Issue the utimes() system call.
 */
SHARED boolean_t itlFile_Utimes()
{
    int returnCode;
    char *fileName, *timePrompt;
    struct timeval chgTimes[2], *utimesPtr;
    int eof, wordSize;

    itl_PromptUser("Filename: ");
    if ((fileName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	struct tm *tmInfo;
	char *separatorP, *dateP, *timeP, *fieldP;
	int i, j, *tmFieldP;
	boolean_t dateIsNull, timeIsNull;

	/*
	 *  Must use a mutex here because the time routines use
	 *  static data areas.
	 */
	pthread_mutex_lock(&utimesMutex);
        {
        time_t t;
        t = 0;
	tmInfo = (struct tm *) localtime((time_t *) &t);  /* initialize time info */
        }
	itl_AdviseUser("Entering the NULL string (\"\") for the date and\n");
	itl_AdviseUser("time values, and 0 for the micro-seconds value\n");
	itl_AdviseUser("will cause the current time to be used.\n");
	for (i = 0; i < 2; i++)
	{
	    dateIsNull = timeIsNull = FALSE;
	    timePrompt = (i == 0) ? "Access" : "Modification";

	    /*
	     *  Ask for time in format easiest for the user, i.e.
	     *  date, time, and microseconds.
	     */
	    itl_PromptUser("%s Date (\"mm/dd/yy\"): ", timePrompt); 
	    if ((dateP = itl_GetString(&eof, &wordSize)) != NULL)
	    {
		if (strcmp(dateP, ""))
		{
		    /*
		     *  Update tm structure with users date.
		     */
		    for (fieldP = dateP, j = 0; j < 3; j++)
		    {
			switch (j) {
			  case 0:
			    tmFieldP = &(tmInfo->tm_mon);
			    break;
			  case 1:
			    tmFieldP = &(tmInfo->tm_mday);
			    break;
			  case 2:
			    tmFieldP = &(tmInfo->tm_year);
			    break;
			}
			*tmFieldP = (int) strtol(fieldP, &separatorP, 10);
			if (j < 2)
			{
			    if (*separatorP == '/')
				fieldP = separatorP + 1;
			    else
			    {
				itl_Error("Invalid date specified, no /.\n");
				break;
			    }
			}
		    }
		    /*
		     *  mktime treats the month based on 0-11 rather than
		     *  the more intuitive 1-12.  (This must be a UNIXism
		     *  from days of old.)  So subtract one from the month
		     *  entered by the user.
		     */ 
		    tmInfo->tm_mon = tmInfo->tm_mon - 1; 
		}
		else
		    dateIsNull = TRUE;

		itl_PromptUser("%s Time (\"hh:mm:ss\"!): ", timePrompt); 
		if ((timeP = itl_GetString(&eof, &wordSize)) != NULL)
		{
		    if (strcmp(timeP, ""))
		    {
			/*
			 *  Update tm structure with users time.
			 */
			for (fieldP = timeP, j = 0; j < 3; j++)
			{
			    switch (j) {
			      case 0:
				tmFieldP = &(tmInfo->tm_hour);
				break;
			      case 1:
				tmFieldP = &(tmInfo->tm_min);
				break;
			      case 2:
				tmFieldP = &(tmInfo->tm_sec);
				break;
			    }
			    *tmFieldP = (int) strtol(fieldP, &separatorP, 10);
			    if (j < 2)
			    {
				if (*separatorP == ':')
				    fieldP = separatorP + 1;
				else
				{
				    itl_Error("Invalid time specified, no :.\n");
				    break;
				}
			    }
			}
		    }
		    else
			timeIsNull = TRUE;

		    chgTimes[i].tv_sec = (dateIsNull && timeIsNull) ? 0
			                              : mktime(tmInfo);
		    ITL_UTIL_FREE(timeP);

		}
		else
		    ITL_FILE_NULL_RETURNED;
		    
		ITL_UTIL_FREE(dateP);
	    }
	    else
		ITL_FILE_NULL_RETURNED;

	    itl_PromptUser("%s Time (micro-seconds): ", timePrompt);
	    chgTimes[i].tv_usec = itl_GetInteger((boolean_t *) 0);
	}
	/*
	 *  If the result of user date and time infor is that all fields
	 *  are zero, then we pass in a
	 *  NULL pointer to force the current time to be used.
	 */
	if ((chgTimes[0].tv_sec == 0) && (chgTimes[0].tv_usec == 0) &&
	    (chgTimes[1].tv_sec == 0) && (chgTimes[1].tv_usec == 0))
	    utimesPtr = (struct timeval *) 0;
	else
	    utimesPtr = chgTimes;
	returnCode = utimes(fileName, utimesPtr);
	ITL_FILE_CHECK_ERROR_CODE(returnCode);
	ITL_UTIL_FREE(fileName);
	pthread_mutex_unlock(&utimesMutex);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

#ifdef SUNOS4
int lstat();
#endif  /* SUNOS4 */

/*
 *  itlFile_Stat() --
 *    Issue the stat()  system call.
 */
SHARED boolean_t itlFile_Stat(statType)
  IN itlFile_statType_t statType;
{
    char *fileName, *varName;
    int returnCode;
    struct stat statBuf;
    int eof, wordSize;
    boolean_t err;
    int (*statFunc)(const char *, struct stat *);

    statFunc = (statType == FileStat)
      ? (int (*)(const char *, struct stat *))stat
	: (int (*)(const char *, struct stat *))lstat;

    itl_PromptUser("Filename: ");
    if ((fileName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	/*
	 *  Prompt for the variable name to hold the data returned
	 *  by stat.
	 */
	itl_PromptUser("Variable Name (fs_stat): ");

	/*
	 *  Check for the empty string ("").  If there then user is not
	 *  interested in keeping the data.
	 */
	varName = itl_PeekString(&eof, &wordSize);
	if (strcmp(varName, ""))
	{
	    ITL_UTIL_FREE(varName);
	    varName = itl_GetVariableName(&err, NULL);
	}
	else
	{
	    ITL_UTIL_FREE(varName);
	    /*
	     *  Clear the "" from the input stream.
	     */
	    varName = itl_GetString(&eof, &wordSize);
	}
	returnCode = statFunc(fileName, &statBuf);
	ITL_FILE_CHECK_ERROR_CODE(returnCode);
	/*
	 *  If the call was successful, then assign the return 
	 *  value to the user supplied variable.
	 */
	if ((returnCode == 0) && strcmp(varName, ""))
	    ITLU_ASSIGN_VARIABLE(varName, &statBuf, itlFile_statDataType);

	ITL_UTIL_FREE(varName);
	ITL_UTIL_FREE(fileName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
 *  itlFile_Statfs() --
 *    Issue the statfs()  system call.
 */
SHARED boolean_t itlFile_Statfs()
{

    char *fileName, *varName;
    int returnCode;
    struct statfs statfsBuf;
    int eof, wordSize;
    boolean_t err;

    itl_PromptUser("Filename: ");
    if ((fileName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	/*
	 *  Prompt for the variable name to hold the data returned
	 *  by statfs.
	 */
	itl_PromptUser("Variable Name (fs_statfs): ");

	/*
	 *  Check for the empty string ("").  If there then user is not
	 *  interested in keeping the data.
	 */
	varName = itl_PeekString(&eof, &wordSize);
	if (strcmp(varName, ""))
	{
	    ITL_UTIL_FREE(varName);
	    varName = itl_GetVariableName(&err, NULL);
	}
	else
	{
	    ITL_UTIL_FREE(varName);
	    /*
	     *  Clear the "" from the input stream.
	     */
	    varName = itl_GetString(&eof, &wordSize);
	}
#ifdef SUNOS5
	returnCode = statfs(fileName, &statfsBuf, 0, 0);
#else      
	returnCode = statfs(fileName, &statfsBuf);
#endif
	ITL_FILE_CHECK_ERROR_CODE(returnCode);

	/*
	 *  If the call was successful, then assign the return 
	 *  value to the user supplied variable.
	 */
	if ((returnCode == 0) && strcmp(varName, ""))
	    ITLU_ASSIGN_VARIABLE(varName, &statfsBuf, itlFile_statfsDataType);

	ITL_UTIL_FREE(varName);
	ITL_UTIL_FREE(fileName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
 *  itlFile_Dup() --
 *    Issue the dup() system call.
 */
SHARED boolean_t itlFile_Dup()
{
    char *varName;
    int returnCode, oldFileId;
    boolean_t err;

    GET_FILE_IDENTIFIER(oldFileId);
    itl_PromptUser("Variable Name for Dup'ed File Identifier: ");
    varName = itl_GetVariableName(&err, NULL);
    returnCode = dup(oldFileId);
    ITL_FILE_CHECK_ERROR_CODE((returnCode < 0) ? returnCode : 0);
    /*
     *  If call was successful, then place the new file descriptor in the
     *  variable name supplied by the user.
     */
    if (returnCode >= 0)
	ITLU_ASSIGN_VARIABLE(varName, &returnCode, itl_intType);

    ITL_UTIL_FREE(varName);

    return(FALSE);
}

/*
 *  itlFile_Access() --
 *    Issue the access() system call.
 */
SHARED boolean_t itlFile_Access()
{
    char *fileName;
    int returnCode, mode;
    int eof, wordSize;

    itl_PromptUser("Filename: ");
    if ((fileName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
#ifdef R_OK
	itl_PromptUser("Access Flags ($_OK* flags): ");
#else /* R_OK */
	itl_PromptUser("Access Flags ($_ACC* flags): ");
#endif /* R_OK */
	mode = itl_GetInteger((boolean_t *) 0);
	returnCode = access(fileName, mode);
	ITL_FILE_CHECK_ERROR_CODE(returnCode);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}

/*
 *  itlFile_Fsync() --
 *    Issue the fsync() system call.
 */
SHARED boolean_t itlFile_Fsync()
{
    int returnCode, fileId;
    int eof, wordSize;

    GET_FILE_IDENTIFIER(fileId);
    returnCode = fsync(fileId);
    ITL_FILE_CHECK_ERROR_CODE(returnCode);

    return(FALSE);
}

/*
 *  itlFile_Fcntl() --
 *    Issue the fcntl() system call.
 */
SHARED boolean_t itlFile_Fcntl()
{
    int returnCode, fileId;
    struct flock *flockP;
    int command, argParam = 0;


    GET_FILE_IDENTIFIER(fileId);
    itl_PromptUser("Command ($F_ flags): ");
    command = itl_GetInteger((boolean_t *) 0);

    /*
     *  The command determines what we do next.
     */
    switch (command) {
      case F_SETFD:   /* set the close-on-exec flag */
	itl_PromptUser("Close-On-Exec Flag ($FD_CLOEXEC or 0): ");
	argParam = itl_GetInteger((boolean_t *) 0);
	break;

      case F_SETFL:   /* set the file status flags */
	itl_PromptUser("Status Flags ($F* flags): ");
	argParam = itl_GetInteger((boolean_t *) 0);
	break;

      case F_GETLK:  /* get the variable to hold lock info */
      case F_SETLK:  /* get the variable that holds lock info */
      case F_SETLKW:
	if ((flockP = itlFile_FlockGet()) != (struct flock *) 0)
	    argParam = (int) flockP;
	else
	{
	    itl_Error("Must enter a previously defined flock variable name.\n");
	    argParam = 0;
	}
	break;

#ifdef F_SETOWN
      case F_SETOWN:   /* set the process group/id. */
	itl_PromptUser("Process group/id: ");
	argParam = itl_GetInteger((boolean_t *) 0);
	break;
#endif /* F_SETOWN */

#ifdef _AIX
      case F_CLOSEM:   /* get the file descriptor */
	GET_FILE_IDENTIFIER(argParam);
	break;
#endif  /* _AIX */

      default:   /*  no additional work needs to be done */
	break;
    }
    returnCode = fcntl(fileId, command, argParam);

#ifdef F_GETOWN
    /*
     *  The F_GETOWN command returns a negative value for success.
     */
    if (command == F_GETOWN)
	returnCode = abs(returnCode);
#endif /* F_GETOWN */

    ITL_FILE_CHECK_ERROR_CODE((returnCode == -1) ? -1 : 0);

    /*
     *  If the call was successful, then assign the return 
     *  value to the ITL command result variable.
     */
    if (returnCode != -1)
	itl_SetIntResult(&commandResult, (int) returnCode);

    return(FALSE);
}

/*
 *  itlFile_Flock() --
 *    Issue the flock() system call.
 */
SHARED boolean_t itlFile_Flock()
{
#if defined(SUNOS5) || defined(__hpux)
    itl_Error("flock not defined on Solaris\n");
#else /* defined(SUNOS5) || defined(__hpux) */
    int fileId, returnCode, operation;

    GET_FILE_IDENTIFIER(fileId);
    itl_PromptUser("Operation ($LOCK_ flags): ");
    operation = itl_GetInteger((boolean_t *) 0);
    returnCode = flock(fileId, operation);
    ITL_FILE_CHECK_ERROR_CODE(returnCode);

#endif /* defined(SUNOS5) || defined(__hpux) */

    return(FALSE);
}


/**
 **  Memory mapped files routines.
 **/

/*
 *  itlFile_MemMap() --
 *    Issue the mmap() system call.
 */
SHARED boolean_t itlFile_MemMap()
{
    int flags, protection;
    size_t length;
    caddr_t address, returnCode;
    off_t offset;
    int fileId;
    char *varName;
    boolean_t err;

    itl_PromptUser("Map Address (0 = system to define): ");
    address = (caddr_t) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Length: ");
    length = (size_t) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Protection Flags ($PROT_* flags): ");
    protection = itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Mapping Flags ($MAP_* flags): ");
    flags = itl_GetInteger((boolean_t *) 0);

    /*
     *  If MAP_ANONYMOUS is specified then we expect the value -1 to
     *  be specified for the file descriptor.  Otherwise we expect a valid
     *  file descriptor.
     */
    GET_FILE_IDENTIFIER(fileId);
    itl_PromptUser("File Offset: ");
    offset = (off_t) itl_GetInteger((boolean_t *) 0);

    /*
     *  Prompt for the variable name to hold the address returned by mmap().
     */
    itl_PromptUser("Variable Name: ");
    varName = itl_GetVariableName(&err, NULL);

    returnCode = mmap(address, length, protection, flags, fileId, offset);
    ITL_FILE_CHECK_ERROR_CODE((returnCode == (caddr_t) -1) ? -1 : 0);

    /*
     *  If successfule, assign the address to the variable supplied by
     *  the user.
     */
    if (returnCode != (caddr_t) -1)
	ITLU_ASSIGN_VARIABLE(varName, &returnCode, itl_intType);

    ITL_UTIL_FREE(varName);

    return(FALSE);
}

/*
 *  itlFile_MemProtect() --
 *    Issue the mprotect() system call.
 */
SHARED boolean_t itlFile_MemProtect()
{
    int  protection;
    size_t length;
    caddr_t address;
    int returnCode;

    itl_PromptUser("Map Address: ");
    address = (caddr_t) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Length: ");
    length = (size_t) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Protection Flags ($PROT_* flags): ");
    protection = itl_GetInteger((boolean_t *) 0);
    returnCode = mprotect(address, length, protection);
    ITL_FILE_CHECK_ERROR_CODE(returnCode);
    return(FALSE);
}

/*
 *  itlFile_MemSync() --
 *    Issue the msync() system call.
 */
SHARED boolean_t itlFile_MemSync()
{
    int  flags;
    size_t length;
    caddr_t address;
    int returnCode;

    itl_PromptUser("Map Address: ");
    address = (caddr_t) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Length: ");
    length = (size_t) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Sync Flags ($MS_* flags): ");
    flags = itl_GetInteger((boolean_t *) 0);
    returnCode = msync(address, length, flags);
    ITL_FILE_CHECK_ERROR_CODE(returnCode);
    return(FALSE);
}

/*
 *  itlFile_MemUnmap() --
 *    Issue the munmap() system call.
 */
SHARED boolean_t itlFile_MemUnmap()
{
    size_t length;
    caddr_t address;
    int returnCode;

    itl_PromptUser("Map Address: ");
    address = (caddr_t) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Length: ");
    length = (size_t) itl_GetInteger((boolean_t *) 0);
    returnCode = munmap(address, length);
    ITL_FILE_CHECK_ERROR_CODE(returnCode);
    return(FALSE);
}

/*
 *  itlFile_MemSemInit() --
 *    Issue the msem_init() system call.
 */
SHARED boolean_t itlFile_MemSemInit()
{

#if defined(SUNOS4) || defined(SUNOS5)
    itl_Error("msem_init not implemented on SUN.\n");
#else /* SUNOS4 || SUNOS5*/
    msemaphore *semAddress, *returnCode;
    int initialValue;

    itl_PromptUser("Semaphore Address: ");
    semAddress = (msemaphore *) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Initial Value ($MSEM_* flags): ");
    initialValue = itl_GetInteger((boolean_t *) 0);
    returnCode = msem_init(semAddress, initialValue);
    ITL_FILE_CHECK_ERROR_CODE((returnCode == (msemaphore *) 0) ? -1 : 0);

#endif  /* SUNOS4 || SUNOS5 */

    return(FALSE);
}

/*
 *  itlFile_MemSemLockOps() --
 *    Issue the msem_lock() or msem_unlock() system call.
 */
SHARED boolean_t itlFile_MemSemLockOps(doLock)
  IN boolean_t doLock;
{

#if defined(SUNOS4) || defined(SUNOS5)
    itl_Error("msem_lock/unlock not implemented on SUN.\n");
#else /* SUNOS4 || SUNOS5 */
    
    msemaphore *semAddress;
    int returnCode;
    int condition;
    int (*lockFunc)();

    lockFunc = (doLock) ? msem_lock : msem_unlock;

    itl_PromptUser("Semaphore Address: ");
    semAddress = (msemaphore *) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Condition ($MSEM_* flags): ");
    condition = itl_GetInteger((boolean_t *) 0);
    returnCode = lockFunc(semAddress, condition);
    ITL_FILE_CHECK_ERROR_CODE(returnCode);

#endif  /* SUNOS4 || SUNOS5 */

    return(FALSE);
}

/*
 *  itlFile_MemSemRemove() --
 *    Issue the msem_remove() system call.
 */
SHARED boolean_t itlFile_MemSemRemove()
{

#if defined(SUNOS4) || defined(SUNOS5)
    itl_Error("msem_remove not implemented on SUN.\n");
#else  /* SUNOS4 || SUNOS5 */

    msemaphore *semAddress;
    int returnCode;;

    itl_PromptUser("Semaphore Address: ");
    semAddress = (msemaphore *) itl_GetInteger((boolean_t *) 0);
    returnCode = msem_remove(semAddress);
    ITL_FILE_CHECK_ERROR_CODE(returnCode);

#endif  /* SUNOS4 || SUNOS5 */

    return(FALSE);
}


/**
 **  Directory routines.
 **/


/*
 *  itlFile_OpenDir() --
 *    Issue the opendir() system call.
 */
SHARED boolean_t itlFile_OpenDir()
{
    char *directoryName, *varName;
    DIR *dirDataP;
    int eof, wordSize;
    boolean_t err;


    itl_PromptUser("Directory Name: ");
    if ((directoryName = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	itl_PromptUser("Variable Name: ");
	varName = itl_GetVariableName(&err, NULL);
	dirDataP = opendir(directoryName);
	ITL_FILE_CHECK_ERROR_CODE(dirDataP ? 0 : -1);
	/*
	 *  If the command was successful, then place the DIR structure
	 *  in the variable name given by the user.
	 */
	if (dirDataP)
	    ITLU_ASSIGN_VARIABLE(varName, (int) &dirDataP, itl_intType);

	ITL_UTIL_FREE(varName);
	ITL_UTIL_FREE(directoryName);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(FALSE);
}
	    
/*
 *  itlFile_ReadDir() --
 *    Issue the readdir()  system call.
 */
SHARED boolean_t itlFile_ReadDir()
{
    char *varName;
    DIR *dirDataP;
    struct dirent *direntP;
    int eof, wordSize;
    boolean_t err;

    GET_DIR_POINTER(dirDataP);
    /*
     *  Prompt for the variable name to hold the pointer returned
     *  by readdir.
     */
    itl_PromptUser("Variable Name (fs_dirent): ");

    /*
     *  Check for the empty string ("").  If there then user is not
     *  interested in keeping the data.
     */
    varName = itl_PeekString(&eof, &wordSize);
    if (strcmp(varName, ""))
    {
	ITL_UTIL_FREE(varName);
	varName = itl_GetVariableName(&err, NULL);
    }
    else
    {
	ITL_UTIL_FREE(varName);
	/*
	 *  Clear the "" from the input stream.
	 */
	varName = itl_GetString(&eof, &wordSize);
    }
    if (dirDataP != (DIR *) 0)
    {
	int oldErrno, atEnd = 0;
	int checkError = 0;

	pthread_mutex_lock(&readdirMutex);

	/*
	 *  Man page says that if readdir reads the end of the directory
	 *  it will return NULL and the value of errno will be unchanged
	 *  compared to what it was before the call.  If NULL is returned
	 *  and errno changed, then something likely went wrong.
	 */
	oldErrno = errno = 0;
	direntP = readdir(dirDataP);

	if (!direntP)
	{
	    if (oldErrno == errno)   /* read end of directory, no error */
		atEnd = 1;
	    else
		checkError = -1;
	}
	ITL_FILE_CHECK_ERROR_CODE(checkError);

	/*
	 *  The commandResult variable is set on every call to indicate
	 *  if we have reached the end of the directory.
	 */
	itl_SetIntResult(&commandResult, atEnd);

	/*
	 *  If the call was successful and the user supplied a variabl
	 *  name, then assign the return 
	 *  value to the user supplied variable.
	 */
	if (direntP && strcmp(varName, ""))
	    ITLU_ASSIGN_VARIABLE(varName, direntP, itlFile_dirDataType);

	pthread_mutex_unlock(&readdirMutex);
    }
    else
	itl_Error("Attempting to use a NULL DIR pointer.\n");

    ITL_UTIL_FREE(varName);

    return(FALSE);
}

/*
 *  itlFile_TellDir() --
 *    Issue the telldir() system call.
 */
SHARED boolean_t itlFile_TellDir()
{
    DIR *dirDataP;

    GET_DIR_POINTER(dirDataP);
    if (dirDataP != (DIR *) 0)
    {
	long dirOffset;

	/*
	 *  No check for error because no errors defined for telldir().
	 */
	dirOffset = telldir(dirDataP);

	/*
	 *  The commandResult variable is set on every call to the
	 *  value returned by telldir().  Scripts can then access it via
	 *  the CommandResult variable.
	 */
	itl_SetIntResult(&commandResult, (int) dirOffset);
    }
    else
	itl_Error("Attempting to use a NULL DIR pointer.\n");

    return(FALSE);
}

/*
 *  itlFile_SeekDir() --
 *    Issue the seekdir() system call.
 */
SHARED boolean_t itlFile_SeekDir()
{
    DIR *dirDataP;
    long location;

    GET_DIR_POINTER(dirDataP);
    itl_PromptUser("Location: ");
    location = (long) itl_GetInteger((boolean_t *) 0);

    if (dirDataP != (DIR *) 0)
	/*
	 *  No check for error because no errors defined for telldir().
	 */
	seekdir(dirDataP, location);
    else
	itl_Error("Attempting to use a NULL DIR pointer.\n");

    return(FALSE);
}

/*
 *  itlFile_RewindDir() --
 *    Issue the RewindDir() system call.
 */
SHARED boolean_t itlFile_RewindDir()
{
    DIR *dirDataP;

    GET_DIR_POINTER(dirDataP);
    if (dirDataP != (DIR *) 0)
	/*
	 *  No check for error because no errors defined for telldir().
	 */
	rewinddir(dirDataP);
    else
	itl_Error("Attempting to use a NULL DIR pointer.\n");

    return(FALSE);
}

/*
 *  itlFile_CloseDir() --
 *    Issue the CloseDir() system call.
 */
SHARED boolean_t itlFile_CloseDir()
{
    DIR *dirDataP;
    int returnCode;

    GET_DIR_POINTER(dirDataP);
    if (dirDataP != (DIR *) 0)
    {
	returnCode = closedir(dirDataP);
	ITL_FILE_CHECK_ERROR_CODE(returnCode);
    }
    else
	itl_Error("Attempting to use a NULL DIR pointer.\n");

    return(FALSE);
}
