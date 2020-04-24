/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftu_util.c,v $
 * Revision 1.1.21.1  1996/10/02  17:48:55  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:39  damon]
 *
 * $EndLog$
 */

/*
 * (C) Copyright 1996, 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <string.h>

#include <dcedfs/stds.h>

#include <ftutil.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/ftu_util.c,v 1.1.21.1 1996/10/02 17:48:55 damon Exp $")

/*
 * The functionality of each of the following exported routines is described
 * in the header file.  The comments below only give implementation
 * specifics, where necessary.
 */


/******************************************************************************
 * Constants
 *****************************************************************************/

/* Combine the VOL_MAX_QUOTA constants into an easy-to-use afs_hyper_t constant */
EXPORT afs_hyper_t ftu_unlimitedQuota =
    AFS_HINIT(VOL_MAX_QUOTA_HIGH, VOL_MAX_QUOTA_LOW);


/******************************************************************************
 * Utility routines
 *****************************************************************************/

/*
 * ftu_GetFile()
 */
EXPORT long
ftu_GetFile(fileName, fullName, statBuf)
    char*	fileName;
    char*	fullName;
    struct stat	*statBuf;
{
    int		is_absolute = 0;
    struct stat	localStatBuf;
    char	pathBuf[MAXPATHLEN + 1];
    int		fileNameLen;
    int		pathLen;

    if (!fileName) 
	return ENOENT;

    fileNameLen = strlen(fileName);

    if (stat(fileName, &localStatBuf) == 0) {
	if (fileName[0] == '/')		/* absolute pathname */
	    (void) strcpy(pathBuf, fileName);
	else {
	    if (strncmp(fileName, "./", 2) == 0)
		fileName += 2;		/* Strip leading "./" if present */
	    /*
	     * Piece together a fully-qualified name
	     */
	    if (getwd(pathBuf) == 0)
		return errno;

	    pathLen = strlen(pathBuf);
	    if (pathLen + fileNameLen + 1 > MAXPATHLEN)
		return ENAMETOOLONG;
	    pathBuf[pathLen] = '/';
	    (void) strcpy(&pathBuf[pathLen + 1], fileName);
	    /*
	     * Stat again, just to double-check.
	     */
	    if (stat(pathBuf, &localStatBuf) != 0)
		return errno;
	}
    } else if (fileName[0] != '/' && strncmp(fileName, "./", 2) != 0) {
	/*
	 * Didn't find it, so try prefixing /dev; this is a bit of
	 * a hack, but is enshrined by tradition.  Less useful in
	 * the SVR4 world.
	 */
	if (sizeof "/dev/" + fileNameLen > MAXPATHLEN + 1)
	    return (ENAMETOOLONG);
	(void) strcpy(pathBuf, "/dev/");
	(void) strcpy(&pathBuf[sizeof "/dev/" - 1], fileName);

	if (stat(pathBuf, &localStatBuf) == -1)
	    return errno;
    } else {
	return errno;
    }

    /* Return full device name if the caller wants it */
    if (fullName != 0)
	(void)strcpy(fullName, pathBuf);

    /* Return stat results if the caller wants them */
    if (statBuf != 0)
	*statBuf = localStatBuf;

    return 0;
}	/* ftu_GetDevice() */

/*
 * ftu_GetDevice()
 */
EXPORT long
ftu_GetDevice(devName, fullDevName, statBuf)
    char*	devName;
    char*	fullDevName;
    struct stat	*statBuf;
{
    struct stat	localStatBuf;
    char	pathBuf[MAXPATHLEN + 1];
    int		error;
    struct stat *statP = (statBuf) ? statBuf : &localStatBuf;
    char	*pathP = (fullDevName) ? fullDevName : pathBuf;

    error = ftu_GetFile(devName, pathP, statP);
    if (error != 0)
	return error;

    /* check to see if we've got a device */
    if (!S_ISCHR(statP->st_mode) && !S_ISBLK(statP->st_mode))
	return FTU_E_NOT_A_DEVICE;

    return 0;
}	/* ftu_GetDevice() */


/*
 * ftu_GetRawDeviceName()
 */
#if defined(AFS_SUNOS5_ENV) || defined(AFS_HPUX_ENV)
/*
 * Converting SVR4-style device names is more complicated.
 * Do it in machine specific ftutil_<machine>.c.
 */
extern long ftu_GetRawDeviceName(char *, char *);
#else
/* EXPORT */
long
ftu_GetRawDeviceName(char *blockDevName, char *rawDevName)
{
    char*	cp;
    dev_t	devNum;
    char	scratch[MAXPATHLEN+1];
    struct stat	statBuf;
    
    /*
     * Ensure that we're dealing with a block device.
     */
    if (stat(blockDevName, &statBuf) == -1)
	return errno;

    if (!S_ISBLK(statBuf.st_mode))
	return FTU_E_NOT_A_BLOCK_DEVICE;

    devNum = statBuf.st_rdev;		/* Stash dev. num. for comparison. */

    (void)strncpy(scratch, blockDevName, MAXPATHLEN);
    scratch[MAXPATHLEN] = '\0';

    if ((cp = strrchr(scratch, '/')) != NULL) {	/* Found a slash */
	/*
	 * Copy everything up to the last slash
	 */
	*cp++ = '\0';			/* Terminate at the slash */
	(void)strcpy(rawDevName, scratch);
	(void)strcat(rawDevName, "/");
    } else {				/* No slash */
	/*
	 * Don't copy anything yet.
	 */
	cp = scratch;
	*rawDevName = '\0';		/* Make `rawDevName' an empty string */
    }

    /* At this point `cp' points to the basename of the block device. */

    /*
     * Append an "r" followed by the basename.
     */
    (void)strcat(rawDevName, "r");
    (void)strcat(rawDevName, cp);
    
    /* See if a file with the conjured-up name exists. */
    if (stat(rawDevName, &statBuf) == -1)
	return errno == ENOENT ? FTU_E_CANT_FIND_CHARACTER_DEVICE : errno;

    /* The file does exist.  See if it's a character device, and if it is, */
    /* see if it matches the block device. */
    if (!S_ISCHR(statBuf.st_mode) || statBuf.st_rdev != devNum)
	return FTU_E_CANT_FIND_CHARACTER_DEVICE;

    return 0;
}	/* ftu_GetRawDeviceName() */
#endif /* AFS_SUNOS5_ENV || AFS_HPUX_ENV */


/*
 * ftu_ParseFsetId()
 */
EXPORT long
ftu_ParseFsetId(fsetIdString, fsetIdP)
    IN char*		fsetIdString;
    OUT ftu_fsetId_t*	fsetIdP;
{
    char *cp = strstr(fsetIdString, ",,");
    char *delim;

    AFS_hzero(*fsetIdP);

    if (cp) {
	if (!isdigit(*fsetIdString))
	    return FTU_E_MALFORMED_FSET_ID;

	AFS_hset64(*fsetIdP, strtol(fsetIdString, &delim, 0), 0);

	if (delim != cp)
	    return FTU_E_MALFORMED_FSET_ID;

	cp += 2;
    } else {
	/* There is no high part */
	cp = fsetIdString;
    }

    if (!isdigit(*cp))
	return FTU_E_MALFORMED_FSET_ID;

    AFS_hadd32(*fsetIdP, strtol(cp, &delim, 0)); /* Set the low word */

    if (*delim != '\0')
	return FTU_E_MALFORMED_FSET_ID;

    return 0;
}	/* ftu_ParseFsetId() */

