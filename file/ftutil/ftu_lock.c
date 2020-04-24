/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ftu_lock.c,v $
 * Revision 1.1.25.1  1996/10/02  17:48:42  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:34  damon]
 *
 * Revision 1.1.20.3  1994/07/13  22:28:14  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:35:26  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:18  mbs]
 * 
 * Revision 1.1.20.2  1994/06/09  14:09:15  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:23:54  annie]
 * 
 * Revision 1.1.20.1  1994/02/04  20:19:45  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:37  devsrc]
 * 
 * Revision 1.1.18.2  1994/01/20  18:43:12  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:27  annie]
 * 
 * Revision 1.1.18.1  1993/12/07  17:25:40  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:06:11  jaffe]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1990, 1991, 1992 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

#include <unistd.h>

#include <dcedfs/stds.h>

#include <ftutil.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/ftu_lock.c,v 1.1.25.1 1996/10/02 17:48:42 damon Exp $")

/*
 * The functionality of each of the following exported routines is described
 * in the header file.  The comments below only give implementation
 * specifics, where necessary.
 */


/******************************************************************************
 * Aggregate locking routines
 *****************************************************************************/

/*
 * ftu_LockAggrDevice()
 *	Open the file, obtain the lock, and then check to see if the aggregate
 *	is attached.  We allow ftu_LookupAggrByDevice() to return ENOSYS
 *	since the requisite kernel objects may not be present; in this case,
 *	we know that the aggregate can't be attached.
 */
EXPORT long
ftu_LockAggrDevice(devName, devLock)
    IN char*		devName;
    OUT ftu_devLock_t*	devLock; /* `*devLock' is really a file descriptor. */
{
    long 		code = 0;
    int			fd;
    struct stat		statBuf;

    if (stat(devName, &statBuf) == -1)
	return errno;

    if (!S_ISCHR(statBuf.st_mode))
	return FTU_E_NOT_A_CHARACTER_DEVICE;

    if ((fd = open(devName, O_RDWR)) == -1)
	return errno;

    /* Try to grab an advisory lock. */
    if (osi_ExclusiveLockNoBlock(fd) == -1)
	if ((errno == EAGAIN || errno == EACCES))
	    code = FTU_E_DEVICE_LOCKED;
        else
	    code = errno;

    if (!code) {
	code = ftu_LookupAggrByDevice(devName, (ftu_aggrId_t*)0);
	if (code == ENOSYS || code == FTU_E_NOT_ATTACHED)
	    code = 0;			/* The aggr is not attached */
	else if (code == 0)
	    code = FTU_E_ATTACHED;	/* The aggr is attached */
    }

    if (!code)
	*devLock = (ftu_devLock_t)fd;
    else {
	(void)osi_UnLock(fd);
	(void)close(fd);
    }

    return code;
}	/* ftu_LockAggrDevice() */


/*
 * ftu_UnlockAggrDevice()
 */
EXPORT long
ftu_UnlockAggrDevice(devLock)
    IN ftu_devLock_t	devLock; /* `*devLock' is really a file descriptor. */
{
    int			fd = (int)devLock;

    /* Unlock the file */
    if (osi_UnLock(fd) == -1) {
	(void)close(fd);
	return errno;
    }

    if (close(fd) == -1)
	return errno;

    return 0;
}	/* ftu_UnlockAggrDevice() */

