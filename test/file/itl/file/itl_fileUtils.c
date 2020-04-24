/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_fileUtils.c,v $
 * Revision 1.1.409.1  1996/10/17  18:26:00  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:06  damon]
 *
 * Revision 1.1.404.1  1994/02/04  20:43:59  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:53  devsrc]
 * 
 * Revision 1.1.402.1  1993/12/07  17:43:48  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:13:14  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/11  18:30:10  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:08:31  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  17:48:32  jaffe
 * 	New file
 * 	[1992/10/27  22:27:06  jaffe]
 * 
 * 	@TRANSARC_COPYRIGHT@
 * 
 * 	$TALog: itl_fileUtils.c,v $
 * 	Revision 1.14  1994/11/01  22:28:02  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.13 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.13  1993/06/03  16:46:24  fred
 * 	[merge of changes from 1.9 to 1.10 into 1.12]
 * 
 * Revision 1.10  1993/04/21  19:12:35  srinivas
 * 	changes for solaris
 * 
 * 	changes for solaris
 * 	[from r1.9 by delta srinivas-itl-1, r1.1]
 * 
 * Revision 1.12  1993/06/03  15:27:16  fred
 * 	[merge of changes from 1.10 to 1.11 into 1.9]
 * 
 * Revision 1.11  1993/06/03  12:49:54  fred
 * 	To port all ITL based test programs to Solaris.  This includes the following
 * 	test programs,
 * 
 * 	fset_test, fx_test, bakserver_test file_test
 * 
 * 	as well as all the code under the src/test/file/itl directory.
 * 
 * 	The changes, to the .c files, if not explicitly listed, would normally be
 * 	one or more of the following.
 * 
 * 	include stdlib.h to define the free() function
 * 	include sys/types.h
 * 	work around the fact that the sys/mman.h file on Solaris defines
 * 	  the constants PRIVATE and SHARED.
 * 	remove extra ';', usually at the end of macro declarations
 * 
 * 	I believe all bsd'isms are now removed from ITL.
 * 
 * 	SUNOS5 porting changes.
 * 	[from r1.10 by delta fred-db3612-port-itl-to-solaris, r1.1]
 * 
 * Revision 1.9  1993/03/01  14:48:17  fred
 * 	To make ITL work on SUN DCE, i.e. for the Alpine group.  Most of the changes
 * 	relate to SunOS 4.1 since it is not ANSI.
 * 
 * 	Note:  I made the changes and validated that they did not break the RIOS
 * 	       or PMAX builds.  Ward Travis will validate that this compiles on the
 * 	       SUN, since to my knowledge our build environment does not support
 * 	       the SPARC.
 * 
 * 	Changes to support SUN DCE.
 * 	[from r1.8 by delta fred-db3201-make-itl-work-on-suns, r1.1]
 * 
 * Revision 6.2  1993/01/28  22:16:12  travis
 * 	Kick off a series of changes required to get this directory to
 * 	build under SunOS.
 * 
 * 	Cast key destructor argument properly.
 * 	[from r6.1 by delta travis-x-libitl-needs-a-memmove-macro, r1.1]
 * 
 * 	$EndTALog$
 * 
 * $EndLog$
 */
/*
 * itl_fsUtils.c -- 
 *    Utility functions to support the file system functions in itl_fs.c.
 */

#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_file.h>
#include <utils/itl/itl_fileUtils.h>

#define WORKING_DIR_SIZE      128  /* length of string to hold current */
                                   /* working directory string         */

PRIVATE pthread_mutex_t identifierMutex;
PRIVATE pthread_key_t curDirKey;

/*
 *  itlFile_CurDir_t --
 *    Holds information about the current working directory for a thread.
 */
typedef struct {
    unsigned int length;
    char currentWorkDir[WORKING_DIR_SIZE];
} itlFile_CurDir_t;

PRIVATE void UtilInit()
{
    #ifdef SUNOS5
    extern void free();
    #endif
    if (pthread_mutex_init(&identifierMutex, pthread_mutexattr_default) != 0)
    {
	itl_Error("File utilites did not initialize.\n");
	exit(-1);
    }

    /*
     *  Now allocate a thread slot for storing the current working directory.
     */
    if (pthread_keycreate(&curDirKey, (pthread_destructor_t) free) != 0) {
	itl_Error("Could not create working directory data area, errno = %d\n",
		       errno);
    }
}

/*
 *  itlFile_utilInit() --
 *    Do any required initialization for the file utilities module.
 */
SHARED void itlFile_utilInit()
{
    static pthread_once_t utilOnce = pthread_once_init;

    if (pthread_once(&utilOnce, (pthread_initroutine_t) UtilInit))
	itl_PrintError("pthread_once failed in itlFile_utilInit\n");
}

/*
 *  itlFile_FlagsToStr()
 *    Converts flags from the data type itlu_valueMap_t, from binary 
 *    form to string form.
 *
 *  WARNING:
 *    Caller is responsible for freeing string that is returned.
 */
SHARED char *itlFile_FlagsToStr(flags, flagData, bitMask)
  IN itlu_valueMap_t *flags;   /* array of flags and values */
  IN long flagData;            /* the actual data to compare */
  IN boolean_t bitMask;        /* TRUE - flag values are bit mask */
                               /* FALSE - flags values are just values */
{
    itlu_valueMap_t *mapIndex;
    int stringLen = 256;
    char *printStrg;
    char *index;

    /*
     *  Allocate memory for the string to hold the flags.
     */
    ITL_UTIL_ALLOC(printStrg, char *, stringLen);
    index = printStrg;

    for (mapIndex = flags; mapIndex->name; mapIndex++) {
        if ((bitMask) ? flagData & mapIndex->value 
	              : flagData == mapIndex->value) {
	    /*
	     *  Check to see if we need more memory.  The 2 in the
	     *  following calculations is to account for the space 
	     *  between flag name, and if this would be the last flag,
	     *  the end of string character.
	     */
	    if ((index + strlen(mapIndex->name) + 2) >=
		(printStrg + stringLen)) {
		stringLen += 256;
		ITL_UTIL_REALLOC(printStrg, char *, stringLen);
	    }
            sprintf(index, "%s ", mapIndex->name);
            index += strlen(mapIndex->name) + 1;  /* 1, for space */
        }
    }
    index -= 1;
    *index = '\0';
    return(printStrg);
}

/*
 *  itlFile_GetIdentifier() --
 *     Gets either a user identifier or a group identifier.
 */
SHARED int itlFile_GetIdentifier(idType)
  IN itlFile_idType_t idType;
{
    struct group *groupInfo;
    struct passwd *passwdInfo;
    char *name, *promptId;
    int eof, wordSize;
    int retId = ITL_FILE_INVALID_ID;
    char promptStr[64];
    
    if (idType == UserId)
	promptId = "User";
    else
	promptId = "Group";

    sprintf(promptStr, "%s Id (enter NOT_CHANGED to leave as is): ", promptId);

    itl_PromptUser(promptStr);
    if ((name = itl_GetString(&eof, &wordSize)) != NULL) 
    {
	/*
	 *  If the user entered the string, "NOT_CHANGED" then we
	 *  don't need to do anything else.
	 */
	if (!strcmp(name, "NOT_CHANGED"))
	    retId = -1;
	else
	{
	    /*
	     *  Must single thread here because the data returned by the
	     *  functions to get the password and group entries is .
	     */
	    pthread_mutex_lock(&identifierMutex);
	    switch (idType) {
	      case UserId:
		  if ((passwdInfo = getpwnam(name)) != (struct passwd *) 0)
		      retId = (int) passwdInfo->pw_uid;
		  else
		      itl_Error("Invalid user name, %s\n", name);
		  break;

	      case GroupId: 
		  if ((groupInfo = getgrnam(name)) != (struct group *) 0)
		      retId = (int) groupInfo->gr_gid;
		  else
		      itl_Error("Invalid group name, %s\n", name);
		  break;

	      default:
		  itl_Error("Invalid parameter to itlFile_GetIdentifer, %d\n",
			    idType);
	    }
	    pthread_mutex_unlock(&identifierMutex);
	}
	ITL_UTIL_FREE(name);
    }
    else
	ITL_FILE_NULL_RETURNED;

    return(retId);
}


/*
 *  itlFile_SetWorkingDir() --
 *    Sets the cached value of the current working directory.  This
 *    is cached in a thread data area to speed up the test code.  Calls
 *    to the getcwd() routine are expensive.
 */
SHARED void itlFile_SetWorkingDir(newDir)
  IN char *newDir;
{
    itlFile_CurDir_t *dirInfoP;

    /*
     *  If newDir is NULL, then we allocate the memory for the slot.
     *  This should only happen once per thread.
     */
    if (newDir == (char *) 0)
    {
	ITL_UTIL_NEW(dirInfoP, itlFile_CurDir_t);
	(void) getcwd(dirInfoP->currentWorkDir, WORKING_DIR_SIZE);
	dirInfoP->length = strlen(dirInfoP->currentWorkDir);
	if (dirInfoP->length == (WORKING_DIR_SIZE - 1))
	    itl_AdviseUser("\nWarning: Current working directory, may be larger than allocated buffer.\n\n");
    }
    else
    {
	if ((pthread_getspecific(curDirKey, (pthread_addr_t *) &dirInfoP))
	           == 0)
	{
	    if (dirInfoP)
	    {
		strcpy(dirInfoP->currentWorkDir, newDir);
		dirInfoP->length = strlen(newDir);
	    }
	    else
		itl_Error("Working directory data area not allocated.\n");
	}
	else
	    itl_Error("Failed to get working directory data.\n");
    }

    /*
     *  Put the data in the slot.
     */
    if (pthread_setspecific(curDirKey, (pthread_addr_t) dirInfoP) != 0)
	itl_Error("Cannot set thread working directory, errno = %d\n", 
		  errno);
}



/*
 *  itlFile_GetWorkingDir() --
 *    This gets the cached value of the current working directory.
 *
 *  XXX:
 *    Perhaps this chould just be part of the BuildFullPathName function to
 *    increase performance.
 */
SHARED void itlFile_GetWorkingDir(dirP, lengthP)
  OUT char **dirP;            /* set to the current working directory value */
  OUT unsigned int *lengthP;  /* set to length of the current working dir */
{
    itlFile_CurDir_t *curDirP;
    int dataOK;

    if ((dataOK = pthread_getspecific(curDirKey, (pthread_addr_t *) &curDirP))
	== 0)
    {
	if (!curDirP)  /* Need to set working directory */
	{
	    itlFile_SetWorkingDir((char *) 0);
	    dataOK = pthread_getspecific(curDirKey, 
					 (pthread_addr_t *) &curDirP);
	}
    }
    if (dataOK != 0)
    {
	itl_Error("Call to pthread_getspecific failed, errno = %d\n", errno);
	itl_Error("\n*** FATAL ***: Can't get working dir.\n");
	exit(-1);
    }
    *dirP = curDirP->currentWorkDir;
    *lengthP = (int) curDirP->length;
}

/*
 *  itlFile_BuildFullPathName() --
 *    Generate the full pathname to a file.  This is required so that
 *    we can have unique filenames for hashing purposes.
 *
 *  WARNING:
 *    This function allocates memory that the caller is expected to free.
 */
SHARED void itlFile_BuildFullPathName(nameP, uniqueNameP)
  IN char *nameP;           /* the file name */
  OUT char **uniqueNameP;   /* the full path name of the file */
{
    unsigned int nameLength = 0;
    char *curDirP;
    char slashChar[2];

    if (*nameP != '/')
    {
	itlFile_GetWorkingDir(&curDirP, &nameLength);
	slashChar[0] = '/';
	slashChar[1] = '\0';
    }
    else
    {
	curDirP = "";
	slashChar[0] = '\0';
    }

    /*
     *  The 2 in the following allocation accounts for the additional '/'
     *  and the '\0' character.
     */
    ITL_UTIL_ALLOC(*uniqueNameP, char *, (nameLength + strlen(nameP) + 2));
    sprintf(*uniqueNameP, "%s%s%s", curDirP, slashChar, nameP);
}

