/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * ID: $Id: patchlog.c,v 1.1.27.1 1996/10/02 17:25:24 damon Exp $
 *
 * COMPONENT_NAME: <FILL_ME_IN>
 *
 * The following functions list may not be complete.
 * Functions defined by/via macros may not be included.
 *
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
/*
 * HISTORY
 * $Log: patchlog.c,v $
 * Revision 1.1.27.1  1996/10/02  17:25:24  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:39  damon]
 *
 * Revision 1.1.22.3  1994/07/13  22:20:15  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:46  devsrc]
 * 
 * 	removed extra <<< theat was there
 * 	[1994/06/29  12:31:30  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  20:12:44  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:57:04  mbs]
 * 
 * Revision 1.1.22.2  1994/06/09  14:01:18  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:33  annie]
 * 
 * Revision 1.1.22.2  1994/06/09  14:01:18  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:33  annie]
 * 
 * Revision 1.1.22.1  1994/02/04  20:13:54  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:41  devsrc]
 * 
 * Revision 1.1.20.1  1993/12/07  17:19:32  jaffe
 * 	New File from Transarc 1.0.3a
 * 
 * 	$TALog: patchlog.c,v $
 * 	Revision 1.6  1994/11/01  21:27:43  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.5 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.4  1993/10/11  20:48:49  rajesh
 * 	Main purpose is to pass a pointer to a "const" log page header
 * 	instead of passing the log page header structure by value to
 * 	functions.
 * 
 * 	Remove an include file.
 * 	[from r1.3 by delta rajesh-db4421-record-checksum-length-in-disk-log, r1.5]
 * 
 * Revision 1.3  1993/10/08  19:14:23  rajesh
 * 	Modify code to incorporate Ted's suggestions.
 * 
 * 	Add copyright declarations. Remove unnecessary variables, header files.
 * 	[from r1.2 by delta rajesh-db4421-record-checksum-length-in-disk-log, r1.4]
 * 
 * $EndLog$
 */
/*
 * patchlog
 *
 * Use this program to change checksum version and lengths in the log on disk.
 * You can also use it to write a restart record onto the first log page.
 *
 * It relies on the output of the findlog program to tell it where
 * the aggregate is located.
 *
 * Caveats:
 *    Beware of changing the checksum length to be more than that the 
 *    the log you are messing with is setup for, as the offset calculation
 *    for log records will go awry.
 */

#include <stdio.h>
#include <dcedfs/param.h>
#include <dcedfs/osi_device.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <ctype.h>
#include <logbuf.h>
#include <logrec.h>
#include <recover.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/patchlog.c,v 1.1.27.1 1996/10/02 17:25:24 damon Exp $")

main(argc, argv)
int argc;
char **argv;
{
    FILE *fh;
    char *name;
    char *cksumTypeStr;
    int cksumType;
    int cksumLength;
    char line[1024], device[1024];
    int blockSize;
    int nLogBlocks;
    int *logBlockArray;
    int logBlockArraySize;
    int totalBlocks;
    int blk;
    struct logRec restart;
    int i;
    char *blockData, *dataP;
    int blkOffset;
    int nread, nwrite;
    int devfd;
    struct logPageHdr *logPageHdr;
    int code;
    int writeRestart = 0;


    if (argc < 4 || argc > 5) {
	fprintf(stderr,"Usage: %s <findlog-filename> <checksum-type> <checksum-length> [-restart] \n",
		argv[0]);
	exit(1);
    }
    
    name = argv[1];
    cksumTypeStr = argv[2];
    
    cksumType = atoi(cksumTypeStr);
    cksumLength = atoi(argv[3]);

    if (argc == 5) {
	writeRestart = 1;
    }

    fh = fopen(name, "r");
    if (!fh) {
	fprintf(stderr, "bad file name %s\n", name);
	exit(2);
    }
    
    if(!fgets(line, sizeof(line), fh)) {
	return EBADF;
    }
    if(!fgets(line, sizeof(line), fh)) {
	return EBADF;
    }
    if(!fgets(line, sizeof(line), fh)) {
	return EBADF;
    }

    /* read the device name */
    if(!fgets(device, sizeof(device), fh)) {
	return EBADF;
    }
    /* replace the trailing newline with a null */
    device[strlen(device)-1] = '\0';

    /* and the block size */
    if(!fgets(line, sizeof(line), fh)) {
	return EBADF;
    }
    blockSize = atoi(line);
    
    /* and the total # of blocks */
    if(!fgets(line, sizeof(line), fh)) {
	return EBADF;
    }
    totalBlocks = atoi(line) ;

    /* the total # of log blocks */
    if(!fgets(line, sizeof(line), fh)) {
	return EBADF;
    }
    nLogBlocks = atoi(line);
    logBlockArraySize = nLogBlocks * sizeof(int);
    logBlockArray = (int *) malloc (logBlockArraySize);
    for (i=0; i < nLogBlocks; i++) {
	if(!fgets(line, sizeof(line), fh)) {
	    return EBADF;
	}
	logBlockArray[i] = atoi(line);
    }
    
    devfd = open(device, O_SYNC | O_RDWR);
    if (devfd < 0) {
	fprintf(stderr, "Device %s is invalid \n", device);
	perror("");
	exit(1);
    }
    blockData = (char *) malloc(blockSize);
    /* Handle all log blocks except the first one that has restart record*/
    for (i = nLogBlocks - 1; i >= 0 ; i--) {
	blk = logBlockArray[i];
	blkOffset = blk * blockSize;
	if (lseek(devfd, blkOffset, SEEK_SET) != blkOffset) {
	    fprintf(stderr, "Unable to access block %d of device %s \n",
		    blk, device);
	    perror("");
	    exit(3);
	}
	nread = read(devfd, blockData, blockSize);
	if (nread < blockSize) {
	    fprintf(stderr, "Unable to read block %d of device %s\n",
		    blk, device);
	    perror("");
	    exit (4);
	}
	logPageHdr = (struct logPageHdr *)blockData;
	logPageHdr->cksum_ver = cksumType;
	logPageHdr->cksum_len = cksumLength;
	if (lseek(devfd, blkOffset, SEEK_SET) != blkOffset) {
	    fprintf(stderr, "Unable to access block %d of device %s \n",
		    blk, device);
	    perror("");
	    exit(5);
	}
	nwrite = write(devfd, blockData, blockSize);
	if (nwrite != blockSize) {
	    fprintf(stderr, "Unable to write block %d of device %s\n",
		    blk, device);
	    perror("");
	    exit (6);
	}
	    
    }

    if (writeRestart) {
	/* write the restart record */
	elbr_InitLogRecHdr( &(restart.hdr) );
	restart.hdr.recType = LOGREC_RSTRT;
	restart.oldData = (caddr_t)0;
	restart.newData = (caddr_t)0;
	restart.lsn = 1;
	
	dataP = blockData;
	dataP += sizeof(struct logPageHdr);
	dataP += cksumLength;

	/* Positioned at first log record offset now */
	bcopy((caddr_t)&restart.hdr, dataP,
	      sizeof (restart.hdr));
	dataP += sizeof(restart.hdr);
	LOGCOPY((caddr_t)&restart.lsn, dataP,
		sizeof (restart.lsn));
	if (lseek(devfd, blkOffset, SEEK_SET) != blkOffset) {
	    fprintf(stderr, "Unable to access block %d of device %s \n",
		    blk, device);
	    perror("");
	    exit(7);
	}
	nwrite = write(devfd, blockData, blockSize);
	if (nwrite != blockSize) {
	    fprintf(stderr, "Unable to write block %d of device %s\n",
		    blk, device);
	    perror("");
	    exit (8);
	}
    }
    close (devfd);
}


