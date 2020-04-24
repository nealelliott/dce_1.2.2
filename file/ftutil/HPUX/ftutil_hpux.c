/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftutil_hpux.c,v $
 * Revision 1.1.21.1  1996/10/02  17:48:06  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:12  damon]
 *
 * Revision 1.1.16.3  1994/07/13  22:28:02  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:35:15  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:04  mbs]
 * 
 * Revision 1.1.16.2  1994/06/09  14:08:51  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:33  annie]
 * 
 * Revision 1.1.16.1  1994/02/04  20:19:19  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:24  devsrc]
 * 
 * Revision 1.1.14.2  1993/12/16  17:14:10  jaffe
 * 	Transarc Delta: berman-o-ot9620-make-LookupAggrByDevice-compare-rawdevs 1.2
 * 	    Delete unused variable.
 * 	[1993/12/16  14:32:28  jaffe]
 * 
 * Revision 1.1.14.1  1993/12/07  17:25:15  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:14:38  jaffe]
 * 
 * $EndLog$
 */
/* Copyright (C) 1993 Transarc Corporation - All rights reserved */

#include <string.h>
#include <dcedfs/stds.h>
#include <ftutil.h>


/*
 * ftu_GetRawDeviceName()
 */
long
ftu_GetRawDeviceName(char *blockDevName, char *rawDevName)
{
    char*	cp;
    char	scratch[MAXPATHLEN+1];
    struct stat	statBuf;

    /*
     * Ensure that we're dealing with a block device.
     */
    if (stat(blockDevName, &statBuf) == -1)
	return errno;

    if (!S_ISBLK(statBuf.st_mode))
	return FTU_E_NOT_A_BLOCK_DEVICE;

    (void)strncpy(scratch, blockDevName, MAXPATHLEN);
    scratch[MAXPATHLEN] = '\0';

    /*
     * rawDevName = rawdiskname(scratch);
     */
    if ((cp = strstr(scratch, "/dsk/")) != NULL) {
	*cp = '\0';
	(void)strcpy(rawDevName, scratch);
	(void)strcat(rawDevName, "/r");
	cp++;
    } else {
	cp = scratch;
	*rawDevName = '\0';
    }
    (void)strcat(rawDevName, cp);

    /* See if a file with the conjured-up name exists. */
    if (stat(rawDevName, &statBuf) == -1)
	return errno == ENOENT ? FTU_E_CANT_FIND_CHARACTER_DEVICE : errno;

    /* The file does exist.  See if it's a character device, and if it is, */
    /* see if it matches the block device. */
    if (!S_ISCHR(statBuf.st_mode))
	return FTU_E_CANT_FIND_CHARACTER_DEVICE;

    return 0;
}	/* ftu_GetRawDeviceName() */
 
