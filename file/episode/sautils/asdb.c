/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: asdb.c,v $
 * Revision 1.1.81.1  1996/10/02  17:27:44  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:08  damon]
 *
 * $EndLog$
 */
/*
*/
/*
 * Copyright (C) 1992, 1991 Transarc Corporation
 * All rights reserved.
 */

/*
 * asdb -- Interface to async debug hooks.
 *
 * Usage:
 *
 *     asdb [-sfuw [-c count] device] [-p amount]
 *
 *     -s: status
 *     -f: freeze
 *     -u: unfreeze
 *         device: name of the device in /dev. Must be mounted.
 *     -c: step
 *         count: specified how many I/Os to allow
 *     -w: wait until frozen
 *     -p: pin memory to simulate smaller configuration
 *         amount: number of KB to pin total.  Specifying zero means free any
 *                 memory previously pinned.
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/syscall.h>
#include <dcedfs/dir_buffer.h>
#include <dcedfs/episode/efs_opcode.h>
#include <dcedfs/episode/efs_debug.h>
#include <string.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/sautils/asdb.c,v 1.1.81.1 1996/10/02 17:27:44 damon Exp $")

void usage();
long asevent_efs_debug _TAKES((long, long, long));
long count;
int waitfreeze = 0;

main(argc, argv)
    int argc;
    char **argv;
{
    char device[20];
    char *dev;
    char *devfile;
    struct stat status;
    long result;
    long op = 0;
    dev_t rdev;
    long amount;
    char errmsg[100];

    osi_setlocale(LC_ALL, "");
    for (;;) {
	argc--;
	argv++;
	if (!argc || **argv != '-')
	    break;
	switch ((*argv)[1]) {
	  case 'f':
	    op = EFS_DEBUG_ASYNC_FREEZE;
	    count = 1;
	    break;
	  case 'u':
	    op = EFS_DEBUG_ASYNC_UNFREEZE;
	    break;
	  case 's':
	    op = EFS_DEBUG_ASYNC_QUERY;
	    break;
	  case 'c':
	    if (--argc == 0) usage();
	    count = atoi(*++argv) + 1;
	    op = EFS_DEBUG_ASYNC_FREEZE;
	    break;
	  case 'w':
	    waitfreeze = 1;
	    break;
	  case 'p':
	    if (--argc == 0) usage();
	    amount = atoi(*++argv);
	    op = EFS_DEBUG_ASYNC_PINCORE;
	    break;
	  default:
	    usage();
	}
    }
    if (!op) usage();
    if (op == EFS_DEBUG_ASYNC_PINCORE) {
	if (asevent_efs_debug(op, amount, 0)) {
	    perror("efs_debug system call failed");
	    exit(1);
	}
	
    } else {
	if (argc > 1) usage();
	dev = *argv;

#ifdef __hpux
	/* This device name is used in asevent_efs_debug to look up a
	 * record in a hash table.  The record was put there by
	 * epimount or efts attach, among others, which use the block
	 * device name.  The hash is on the major and minor numbers.
	 * On HP/UX, the major number for a block device is different
	 * from the major number for the corresponding raw device
	 * (e.g. for a scsi disk the block device is 7 and the raw
	 * device is 2F; see /etc/lsdev).  Thus, using the raw device
	 * name here would fail to find the record in the hash
	 * table.
	 */
	sprintf (device, "%s", dev);
#else
	if (devfile = strstr(dev,"/dev/") ) {
	    /* absolute pathname for device */
	    devfile += 5; 
	    if (*devfile != 'r') {
		sprintf(device,"/dev/r%s",devfile);
	    } else {
		sprintf(device,"%s",dev);
	    }
	} else if (*dev != '/') {
	    /* dev only contains the last component of the device pathname */
	    if (*dev == 'r') dev++;
	    sprintf(device, "/dev/r%s", dev);
	} else {
	    /* invalid device name */
	    fprintf(stderr,"%s is not a valid device name \n",dev);
	    exit(1);
	}
#endif /* __hpux */

	if (stat(device, &status) != 0) {
	    sprintf(errmsg,"can't stat device %s",device);
	    perror(errmsg);
	    exit(1);
	}
	rdev = status.st_rdev;
	if (asevent_efs_debug(op, (long) rdev, (long)&count)) {
	    perror("efs_debug system call failed");
	    exit(1);
	}
	if (waitfreeze) {
	    for (;;) {
		if (asevent_efs_debug(EFS_DEBUG_ASYNC_QUERY,
				      (long) rdev, (long)&count)) {
		    perror("efs_debug query call failed");
		    exit(1);
		}
		if (count <= 1) break;
		sleep(2);
	    }
	}
	if (op == EFS_DEBUG_ASYNC_QUERY) {
	    char message[30];
	    if (count > 1) {
		sprintf(message, "freezing (%d i/o's to go)", count - 1);
	    } else {
		strcpy(message, count? "frozen" : "not frozen");
	    }
	    printf("Device %s is %s\n", device, message);
	}
    }	
    exit(0);
}

void usage() {
    fprintf(stderr, "usage: asdb [-fusw [-c count] device-name] [-p amount]\n");
    exit(1);
}

long asevent_efs_debug(op, parm1, parm2)
     long op, parm1, parm2;
{
    return afs_syscall (AFSCALL_EPISODE, EPIOP_DEBUG, EFS_DEBUG_ASYNC, 
			op, parm1, parm2);
}

