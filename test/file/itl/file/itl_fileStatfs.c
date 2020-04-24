/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_fileStatfs.c,v $
 * Revision 1.1.108.1  1996/10/17  18:25:57  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:04  damon]
 *
 * Revision 1.1.103.1  1994/02/04  20:43:56  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:52  devsrc]
 * 
 * Revision 1.1.101.1  1993/12/07  17:43:45  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:12:57  jaffe]
 * 
 * Revision 1.1.9.2  1993/07/19  19:42:04  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:31:57  zeliff]
 * 
 * Revision 1.1.7.3  1993/07/16  21:54:21  kissel
 * 	Put HP RP work on the 1.0.3i branch.
 * 	[1993/06/21  15:05:33  kissel]
 * 
 * Revision 1.1.5.2  1993/06/04  19:05:03  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:44:14  kissel]
 * 
 * Revision 1.1.3.2  1993/02/26  18:02:07  kissel
 * 	Include vfs.h for HPUX, just like Sun (and a few other things, too).
 * 	[1993/02/26  17:58:01  kissel]
 * 
 * Revision 1.1.2.4  1993/01/11  18:30:05  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:08:14  htf]
 * 
 * Revision 1.1.2.3  1992/11/18  21:45:26  jaffe
 * 	Transarc delta: itl-sanzi-initial-spoke-library 1.1
 * 	  Selected comments:
 * 	    Addition of Spoke Library to the suite of ITL libraries.
 * 	    Port of file_test and its libs to the Sun.
 * 	    Port this file to run on the Sun.
 * 	[1992/11/18  14:50:46  jaffe]
 * 
 * Revision 1.1.2.2  1992/10/28  17:48:11  jaffe
 * 	Transarc delta: jess-itl-add-statfs 1.1
 * 	  Selected comments:
 * 	    Add statfs function call to the itl file interface.
 * 	    Define statfs data type utils.
 * 	[1992/10/27  22:25:24  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fileStat.c -- 
 *    Functions to implement the variable type for the structure
 *    returned by the statfs() system call.
 */

#if defined(SUNOS4) || defined(__hpux)
#include <sys/vfs.h>
#endif  /* SUNOS4 || __hpux */

#include <sys/types.h>
#if !defined(__hpux)
#include <sys/statfs.h>
#endif
#include <utils/itl/itl_fileStatfs.h>

int itlFile_statfsDataType;

PRIVATE itlValue_varTypeInfo_t statfsVarType = {
    "fs_statfs",
    itlFile_StatfsCopy,
    itlFile_StatfsNew,
    itlFile_StatfsFree,
    NULL,
    itlFile_StatfsBinaryOps,
    NULL,
    NULL,
    NULL,
    itlFile_StatfsDisplay,
    itlFile_StatfsGetField,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
    };

PRIVATE char *statfsFieldNames[] = {
#ifndef	SUN0S5
    "f_version", /*for aix only */
    "f_type",
    "f_bsize",
#else
    "f_fstyp",
    "f_bsize",
    "f_frsize",
#endif
    "f_blocks",
    "f_bfree",
#ifndef SUNOS5
    "f_bavil",
#endif
    "f_files",
    "f_ffree",
#ifndef	SUNOS5
    "f_fsid1",
    "f_fsid2",
    "f_spare0", 		/* osf declares this as an arry of 9 long */
    "f_spare1", 		/* osf declares this as an arry of 9 long */
    "f_spare2", 		/* osf declares this as an arry of 9 long */
    "f_spare3", 		/* osf declares this as an arry of 9 long */
    "f_spare4", 		/* osf declares this as an arry of 9 long */
    "f_spare5", 		/* osf declares this as an arry of 9 long */
    "f_spare6", 		/* osf declares this as an arry of 9 long */
    "f_spare7", 		/* osf declares this as an arry of 9 long */
    "f_spare8", 		/* osf declares this as an arry of 9 long */
    "f_mntonname", 	/* osf it is char of macnamelen*/
    "f_mntfromname",	/* osf it is char of macnamelen*/
    "f_fname", 		/* aix, array of 32 chars */
    "f_fpack",		/* aix, array of 32 chars */
    "f_fname_max", 	/* aix */ 
    "f_vfstype",  	/* aix */
    "f_nlsdirtype", 	/* aix */
    "f_vfsnumber", 	/* aix */
    "f_vfsoff", 	/* aix */
    "f_vfslen",  	/* aix */
    "f_vfsvers" 	/* aix */ 
#else
    "f_fname",
    "f_fpack"
#endif
};

/*
 *  StatfsInit() --
 *    To be called via pthread_once to do the real initialization.
 */
PRIVATE void StatfsInit()
{
    /*
     *  We can safely set itlFile_statfsDataType because this function is called
     *  via pthread_once.
     */
    itlFile_statfsDataType = itlValue_RegisterVarType(&statfsVarType);
}

/*
 * itlFile_StatInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
SHARED void itlFile_StatfsInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    if (pthread_once(&hashOnce, (pthread_initroutine_t) StatfsInit))
	itl_PrintError("pthread_once failed in StatfsInit\n");
}

SHARED var_val_t *itlFile_StatfsNew(newStatfsP, newType)
  IN pointer_t newStatfsP;
  IN int newType;
{
    var_val_t *newVarP;
    struct statfs *statfsEntryP = (struct statfs *) newStatfsP;

    ITL_UTIL_NEW(newVarP, var_val_t);
    VALUE_TYPE(newVarP) = newType;
    VALUE_VAL(newVarP) = newStatfsP;
    VALUE_SIZE(newVarP) = (int) sizeof(struct statfs);
    return(newVarP);
}

SHARED boolean_t itlFile_StatfsFree(freeStatfsP)
  IN var_val_t *freeStatfsP;
{
    if (VALUE_VAL(freeStatfsP))
	ITL_UTIL_FREE(VALUE_VAL(freeStatfsP));
    return(FALSE);
}

SHARED var_val_t *itlFile_StatfsCopy(copyStatfsP)
  IN var_val_t *copyStatfsP;
{
    var_val_t *newCopyP;
    struct statfs *statfsP;

    newCopyP = itl_NewVarVal();
    ITL_UTIL_ALLOC(statfsP, struct statfs *, VALUE_SIZE(copyStatfsP));
    VALUE_SIZE(newCopyP) = VALUE_SIZE(copyStatfsP);
    VALUE_TYPE(newCopyP) = VALUE_TYPE(copyStatfsP);
    VALUE_VAL(newCopyP) = statfsP;
    ITL_UTIL_MEMCOPY(VALUE_VAL(copyStatfsP), VALUE_VAL(newCopyP), 
		     VALUE_SIZE(copyStatfsP));
    return(newCopyP);
}

SHARED char *itlFile_StatfsDisplay(displayStatfsP, lengthP)
  IN var_val_t *displayStatfsP;
  OUT int *lengthP;
{
    char *returnStr;
    struct statfs *statfsP;
    unsigned int strLength;

    statfsP = (struct statfs *) VALUE_VAL(displayStatfsP);

    /*
     *  The calculations below are just estimates.  The values for each
     *  field include the estimate of size, plus the space to hold the
     *  field name, a space, an equals sign, another space, and a comma in 
     *  the output string.
     */


     /* Do we have better way to have the buffer allocated? */

    strLength = 35 + /*  "f_version", for aix only */
                35 + /* "f_type" */
                35 + /* "f_bsize" */
    	 	35 + /* "f_blocks" */
    		35 + /* "f_bfree", */
    		35 + /* "f_bavil", */
    		35 + /* "f_files", */
    		35 + /* "f_ffree", */
    		68 + /* "f_fsid",  */
    		180 + /* "f_spare", osf declares this as an arry of 9 */
    		35 + /* "f_mntonname",  osf */
    		35 + /* "f_mntfromname",   osf */
    		35 + /* "f_fname",  aix, array of 32 chars */
    		35 + /* "f_fpack",  aix, array of 32 chars */
    		35 + /* "f_fname_max",    aix */
    		35 + /* "f_vfstype",       aix */
    		35 + /* "f_nlsdirtype", aix */
    		35 + /* "f_vfsnumber",  aix */
    		35 + /* "f_vfsoff",     aix */
     		35 + /* "f_vfslen",  aix */
    		35;	/* "f_vfsvers"    aix */


    ITL_UTIL_ALLOC(returnStr, char *, strLength);

    /*
     *  We should be able to use the result of sprintf, given
     *  sprintf returns the length of the string.  But CMA appears to
     *  returning the string pointer, so we'll do the safe thing and just
     *  do strlen().
     */
#if !defined(SUNOS5)
    sprintf(returnStr, "{type = %d, bsize = %d, blocks = %d, bfree = %d\n",
	    statfsP->f_type, statfsP->f_bsize, statfsP->f_blocks, statfsP->f_bfree);
#ifdef __hpux
    sprintf(&(returnStr[strlen(returnStr)]), 
	    " bavail = %d, files = %d, ffree = %d, fsid1 = %d, fsid2 = %d\n",
	    statfsP->f_bavail, statfsP->f_files, statfsP->f_ffree, statfsP->f_fsid[0], statfsP->f_fsid[1]);
#else
    sprintf(&(returnStr[strlen(returnStr)]), 
	    " bavail = %d, files = %d, ffree = %d, fsid1 = %d, fsid2 = %d\n",
	    statfsP->f_bavail, statfsP->f_files, statfsP->f_ffree, statfsP->f_fsid.val[0], statfsP->f_fsid.val[1]);
#endif

#ifdef _AIX   /* AIX uses different fields for file sysname, here are
                 some of them used. Someothers can be added later */

    sprintf(&(returnStr[strlen(returnStr)]), 
	    " fname = %s, fpack = %s}\n",
	    statfsP->f_fname, statfsP->f_fpack);

#else   /* _AIX , Now it is OSF */

    sprintf(&(returnStr[strlen(returnStr)]),
#if defined(SUNOS4) || defined(__hpux)
	    "\n"
#else  /* SUNOS4 || __hpux */
	    " dirmountedon = %s, mountedfs = %s}\n",
	    statfsP->f_mntonname, statfsP->f_mntfromname
#endif
	    );

#endif  /* _AIX */

#else  /*SUNOS5*/
    sprintf(returnStr, "{type = %d, bsize = %d, blocks = %d, bfree = %d\n",
	    statfsP->f_fstyp, statfsP->f_bsize, statfsP->f_blocks, statfsP->f_bfree);
    sprintf(&(returnStr[strlen(returnStr)]), 
	    " frsize = %d, files = %d, ffree = %d \n",
	    statfsP->f_frsize, statfsP->f_files, statfsP->f_ffree);
    statfsP->f_fname[5] = statfsP->f_fpack[5] = '\0';
    sprintf(&(returnStr[strlen(returnStr)]), 
	    " fname = %s, fpack = %s}\n",
	    statfsP->f_fname, statfsP->f_fpack);

#endif /*SUNOS5*/
    if (lengthP)
	*lengthP = strLength;

    return(returnStr);
}

/*
 * This function will return a var_val_t pointer.
 * Not all of the fields are list here. We will add more as necessary.
 *
 */

SHARED var_val_t *itlFile_StatfsGetField(statfsValP, fieldName)
  IN var_val_t *statfsValP;
  IN char *fieldName;
{
    var_val_t *retValP = (var_val_t *) 0;
    int fieldNum, convertVal;
    char *strVal;
    struct statfs *statfsP;

    statfsP = (struct statfs *) VALUE_VAL(statfsValP);

    if (itl_LookupTable(fieldName, statfsFieldNames,
			sizeof(statfsFieldNames) / sizeof(char *),
			&fieldNum) == 1)
    {
        convertVal = 0;
        strVal = NULL;
	switch (fieldNum) {
#ifndef	SUNOS5
	  case 1:   /* f_bsize */
	    convertVal = (int) statfsP->f_bsize;
	    break;
	  case 2:   /* f_blocks */
	    convertVal = (int) statfsP->f_blocks;
	    break;
	  case 3:   /* f_bfree */
	    convertVal = (int) statfsP->f_bfree;
	    break;
	  case 4:   /* f_bavail */
	    convertVal = (int) statfsP->f_bavail;
	    break;
	  case 5:   /* f_files */
	    convertVal = (int) statfsP->f_files;
	    break;
	  case 6:   /* f_ffree */
	    convertVal = (int) statfsP->f_ffree;
	    break;
	  case 7:   /* f_fsid1 , this one is two long */
#ifdef __hpux
	    convertVal = (int) statfsP->f_fsid[0];
#else
	    convertVal = (int) statfsP->f_fsid.val[0];
#endif
	    break;
	  case 8:   /* f_fsid2 , this one is two long */
#ifdef __hpux
	    convertVal = (int) statfsP->f_fsid[1];
#else
	    convertVal = (int) statfsP->f_fsid.val[1];
#endif
	    break;
#ifdef _AIX

          /* how about the char fileds */
	  case 0:   /* f_version */
	    convertVal = (int) statfsP->f_version;
	    break;
	  case 22:   /* f_fname */
	    strVal = (char *) statfsP->f_fname;
	    break;
	  case 23:   /* f_fname */
	    strVal = (char *) statfsP->f_fpack;
	    break;

#else  /* _AIX */
          case 19:
#if !defined(SUNOS4) && !defined(__hpux)
            strVal = (char * ) statfsP->f_mntonname;
            break;
          case 20:
            strVal = (char * ) statfsP->f_mntfromname;
            break;
#endif /* SUNOS4 || __hpux */
	  case 9:   /* f_spare0 */
	    convertVal = (int) statfsP->f_spare[0];
	    break;
	  case 10:   /* f_spare1 */
	    convertVal = (int) statfsP->f_spare[1];
	    break;
	  case 11:   /* f_spare2 */
	    convertVal = (int) statfsP->f_spare[2];
	    break;
	  case 12:   /* f_spare3 */
	    convertVal = (int) statfsP->f_spare[3];
	    break;
	  case 13:   /* f_spare4 */
	    convertVal = (int) statfsP->f_spare[4];
	    break;
	  case 14:   /* f_spare5 */
	    convertVal = (int) statfsP->f_spare[5];
	    break;
	  case 15:   /* f_spare6 */
	    convertVal = (int) statfsP->f_spare[6];
	    break;
	  case 16:   /* f_spare7 */
	    convertVal = (int) statfsP->f_spare[7];
	    break;
	  case 17:   /* f_spare8  */
	    convertVal = (int) statfsP->f_spare[8];
	    break;

#endif  /* _AIX */

#else /* SUNOS5 */
	  case 0:   /* f_fstyp */
	    convertVal = (int) statfsP->f_fstyp;
	    break;
	  case 1:   /* f_bsize */
	    convertVal = (int) statfsP->f_bsize;
	    break;
	  case 2:   /* f_frsize */
	    convertVal = (int) statfsP->f_frsize;
	    break;
	  case 3:   /* f_blocks */
	    convertVal = (int) statfsP->f_blocks;
	    break;
	  case 4:   /* f_bfree */
	    convertVal = (int) statfsP->f_bfree;
	    break;
	  case 5:   /* f_files */
	    convertVal = (int) statfsP->f_files;
	    break;
	  case 6:   /* f_ffree */
	    convertVal = (int) statfsP->f_ffree;
	    break;
	  case 7:   /* f_fname */
	    statfsP->f_fname[5] = '\0';
	    strVal = (char *) statfsP->f_fname;
	    break;
	  case 8:   /* f_fpack */
	    statfsP->f_fpack[5] = '\0';
	    strVal = (char *) statfsP->f_fpack;
	    break;

#endif
	}
        if ( strVal && !convertVal) 
           retValP = itlStds_StringVal(strVal);
        else 
	   retValP = itlStds_IntVal(convertVal);
    }

    return(retValP);
}

SHARED boolean_t itlFile_StatfsBinaryOps(leftValP, rightValP, opStr)
  INOUT var_val_t *leftValP;
  IN var_val_t *rightValP;
  IN char *opStr;
{
    itl_Error("Binary operatons on struct stat not currently implemented.\n");
}
