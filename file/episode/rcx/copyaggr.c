/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: copyaggr.c,v $
 * Revision 1.1.28.1  1996/10/02  17:26:41  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:33:30  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1993 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/syscall.h>
#include <dcedfs/dir_buffer.h>
#include <dcedfs/episode/efs_opcode.h>
#include <dcedfs/episode/efs_debug.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUF_SIZE (32*1024)
#define SMALL_BUF_SIZE (512)

void usage();

long asevent_efs_debug _TAKES((long, long, long));
int waitfreeze = 0;
long count = 0;
char program[512];
char errmsg[512];

/* Set from DEBUG environment variable.
 *   "1" Input files are not devices. Do not freeze or unfreeze. But still in
 *       debug mode.
 *   "2" Input files are device. Freeze and unfreeze will happen. In debug 
 *       mode.
 */
int debug = 0;

long asevent_efs_debug(/* IN */ long op, 
		       /* IN */ long parm1, 
		       /* IN */ long parm2)
{
    return afs_syscall (AFSCALL_EPISODE, EPIOP_DEBUG, EFS_DEBUG_ASYNC, 
			op, parm1, parm2);
}

void usage (/* null */)
{
    /* Setting environment variable COPYAGGR_NOCHECKSUM turns off 
     * checksumming source and target devices.
     */
    fprintf(stderr,
	    "Usage: %s <source device> <target device1> <target device2>",
	    program);
    fprintf(stderr,
	    "              [checksumDataFile] [freeze I/O count]", program);
    fprintf(stderr, "\n");
    exit(1);
}

char *findDevFile(/* IN */ char *dev)
{
    char *devfile, *rdev;

    rdev = (char *) calloc(1, 512);
    if ( devfile = strstr(dev,"/dev/") ) {
	/* absolute pathname for device */
	devfile += 5; 
	if (*devfile != 'r') {
	    sprintf(rdev, "/dev/r%s", devfile);
	} else {
	    sprintf(rdev, "%s", dev);
	}
    } else if (*dev != '/') {
	/* dev only contains the last component of the device pathname */
	if (*dev == 'r') dev++;
	sprintf(rdev, "/dev/r%s", dev);
    } else {
	return (char *)NULL;
    }
    return rdev;
}

/* Returns # bytes read */
long readDevice(/* IN    */ int rfd,
		/* IN    */ char *dev,
		/* IN    */ long i,
		/* INOUT */ char *buffer,
		/* IN    */ char buflen,
		/* INOUT */ long *blksize, 
		/* INOUT */ long *offsetP)
{
    long readOffset, nextReadOffset;
    long ioblksize = *blksize;
    long len;

    readOffset = lseek(rfd, 0, SEEK_CUR);
    if (readOffset != *offsetP) {
	fprintf(stderr,
		"readDevice: offset cookie %l != current pos %l in dev %s \n",
		*offsetP, readOffset, dev);
	exit(1);
    }
    len = read(rfd, buffer, ioblksize);
    if (len > 0 && len < ioblksize && ioblksize > SMALL_BUF_SIZE) {
	if (debug) {
	    fprintf(stderr,
		    "Iteration %d: read %d of %d bytes at offset %d from %s\n",
		    i, len, ioblksize, readOffset, dev);
	    fprintf(stderr, "\t setting read unit size to %d from %d\n",
		    SMALL_BUF_SIZE, ioblksize);
	}
	*blksize = ioblksize = SMALL_BUF_SIZE;
	lseek(rfd, readOffset, SEEK_SET);
	len = read(rfd, buffer, ioblksize);
    }
    if (debug)
	fprintf(stderr,
		"Iteration %d:  read %d of %d bytes at offset %d from %s\n",
		i, len, ioblksize, readOffset, dev);
    *offsetP = nextReadOffset = lseek(rfd, 0, SEEK_CUR);

    return len;
}		

/* This guarantees to write all the data it has been asked to do so */
void writeDevice (/* IN */ int wfd,
		  /* IN */ char *buffer,
		  /* IN */ long writeLen,
		  /* IN */ long *offsetP,
		  /* IN */ char *dev) 
{
    long writeOffset, offset;
    long len;
    char *buf;
    long writeCount;

    writeOffset = 0;
    len = writeLen;

    offset = lseek(wfd, 0, SEEK_CUR);
    if (offset != *offsetP) {
	fprintf(stderr,
		"writeDevice: offset cookie %l != current pos %l in %s \n",
		*offsetP, offset, dev);
	exit(1);
    }

    while (len > 0) {
	buf = buffer + writeOffset;
	writeCount = write(wfd, buf, len);
	if (writeCount <= 0) {
	    sprintf(errmsg, "Failed to write to target device %s", dev);
	    perror(errmsg);
	    exit(1);
	}
	if (debug) 
	    fprintf(stderr,
		    "wrote %d bytes (of %d bytes) at offset %d to %s \n",
		    writeCount, len, offset + writeOffset, dev);
	len -= writeCount;
	writeOffset += writeCount;
    }
    *offsetP = offset + writeLen;
}
    
int Checksum(/* IN */ char *trdev, 
	     /* OUT */ unsigned long *trdevCrcP)
{
    unsigned long trdevCrc;
    int rfd, len;
    char *buffer;
    long readOffset, ioblksize;
    long i;

    trdevCrc = 0;

    buffer = (char *) calloc(1, BUF_SIZE);
    if (buffer == (char *)NULL) {
	fprintf(stderr, "Checksum: Could not allocate memory for buffer\n");
    }

    rfd = open(trdev, O_RDONLY);
    if (rfd < 0) {
	sprintf(errmsg, "Could not open target device %s", trdev);
	perror(errmsg);
    }

    i = 0;    
    ioblksize = BUF_SIZE;
    readOffset = lseek(rfd, 0, SEEK_CUR), 
    len = readDevice(rfd, trdev, i, buffer, BUF_SIZE, &ioblksize, &readOffset);
    while (len > 0 ) {
	CalcCRC32Std (buffer, len, &trdevCrc);
	i++;
	len = readDevice(rfd, trdev, i, buffer, BUF_SIZE,
			 &ioblksize, &readOffset);
    }

    close(rfd);
    (void) free((void *)buffer);

    *trdevCrcP = trdevCrc;
    return 0;
}
    
/* Freeze aggregate */
void freezeAggr(/* IN */ dev_t devN,
		/* IN */ int freezeIOCount) 
{
    int count;

    count = freezeIOCount;

    if (asevent_efs_debug(EFS_DEBUG_ASYNC_FREEZE,
			  (long) devN,
			  (long) &count)) {
	perror("efs_debug ASYNC_FREEZE system call failed");
	exit(1);
    }

    for (;;) {
	if (asevent_efs_debug(EFS_DEBUG_ASYNC_QUERY,
			      (long) devN, (long)&count)) {
	    perror("efs_debug query call failed");
	    exit(1);
	}
	if (count <= 1) break;
	sleep(2);
    }

    /* Allow time for last I/O to go thru */
    sleep(5);

}

/* Unfreeze aggregate */
void unfreezeAggr(/* IN */ dev_t devN)
{
    int count;

    if (asevent_efs_debug(EFS_DEBUG_ASYNC_UNFREEZE,
			  (long) devN,
			  (long) &count)) {
	perror("efs_debug system call failed");
	exit(1);
    }
}

main(int argc, char **argv)
{
    char *sdev, *tdev1, *tdev2;
    char *srdev, *trdev1, *trdev2;
    struct stat status;
#ifdef AFS_HPUX_ENV
    dev_t sdevN;
#endif
    dev_t srdevN;
    long i, len;
    long ioblksize;
    int rfd, wfd1, wfd2;
    long readOffset, write1Offset, write2Offset;
    char *buffer;
    char *checksumDataFile;
    unsigned long sdevCrc, tdev1Crc, tdev2Crc;
    FILE *fd;
    char *envDebug;
    int freezeIOCount = 0;

    /* Set from COPYAGGR_NOCHECKSUM env. var */
    int noChecksum = 0;

    if ((envDebug = getenv("DEBUG")) != (char *)NULL) {
	debug = atoi(envDebug);
    }
    osi_setlocale(LC_ALL, "");

    sdevCrc = tdev1Crc = tdev2Crc = 0;

    strncpy(program, argv[0], 512);
    if (argc < 4) usage();

    sdev = argv[1];
    tdev1 = argv[2];
    tdev2 = argv[3];

    if (argc >= 5) {
	checksumDataFile = argv[4];
    }
    
    if (argc > 5) {
	freezeIOCount = atoi(argv[5]);
    }

    if (getenv("COPYAGGR_NOCHECKSUM")) {
	noChecksum = 1;
	fprintf(stderr, "%s: invoked in no checksum mode \n", argv[0]);
    }

    if (debug == 1) {
	srdev = sdev;
	trdev1 = tdev1;
	trdev2 = tdev2;
    } else {
	srdev = findDevFile(sdev);  
	trdev1 = findDevFile(tdev1);  
	trdev2 = findDevFile(tdev2);  
    }

    if (srdev == (char *)NULL) {
	fprintf(stderr, "Invalid specification for source device %s \n",
		sdev);
	exit(1);
    }

    if (trdev1 == (char *)NULL) {
	fprintf(stderr, "Invalid specification for target device %s \n",
		tdev1);
	exit(1);
    }

    if (trdev2 == (char *)NULL) {
	fprintf(stderr, "Invalid specification for target device %s \n",
		tdev1);
	exit(1);
    }

#ifdef AFS_HPUX_ENV
    if (stat(sdev, &status) != 0) {
	sprintf(errmsg,"can't stat source device %s", sdev);
	perror(errmsg);
	exit(1);
    }
    sdevN = status.st_rdev;
#endif /* AFS_HPUX_ENV */

    if (stat(srdev, &status) != 0) {
	sprintf(errmsg,"can't stat source device %s", srdev);
	perror(errmsg);
	exit(1);
    }
    srdevN = status.st_rdev;

    if (stat(trdev1, &status) != 0) {
	sprintf(errmsg,"can't stat target device %s", trdev1);
	perror(errmsg);
	exit(1);
    }

    if (stat(trdev2, &status) != 0) {
	sprintf(errmsg,"can't stat target device %s", trdev2);
	perror(errmsg);
	exit(1);
    }

    if (debug != 1) 
#ifdef AFS_HPUX_ENV
	freezeAggr(sdevN, freezeIOCount);
#else
	freezeAggr(srdevN, freezeIOCount);
#endif

    /* Copy aggregate */
    buffer = (char *) calloc(1, BUF_SIZE);
    if (buffer == (char *)NULL) {
	fprintf(stderr, "%s: Could not allocate memory for buffer\n", argv[0]);
	exit(1);
    }

    rfd = open(srdev, O_RDONLY);
    if (rfd < 0) {
	sprintf(errmsg, "Could not open source device %s", srdev);
	perror(errmsg);
	exit(1);
    }
    wfd1 = open(trdev1, O_RDWR);
    if (wfd1 < 0) {
	sprintf(errmsg, "Could not open target device %s\n", trdev1);
	perror(errmsg);
	exit(1);
    }
    wfd2 = open(trdev2, O_RDWR);
    if (wfd2 < 0) {
	sprintf(errmsg, "Could not open target device %s\n", trdev2);
	perror(errmsg);
	exit(1);
    }

    i = 0;				
    ioblksize = BUF_SIZE;
    readOffset = lseek(rfd, 0, SEEK_SET), 
    write1Offset = lseek(wfd1, 0, SEEK_SET);
    write2Offset = lseek(wfd2, 0, SEEK_SET);

    len = readDevice(rfd, srdev, i, buffer, BUF_SIZE, &ioblksize, &readOffset);
    while (len > 0 ) {
	/* Write to device 1 */
	writeDevice(wfd1, buffer, len, &write1Offset, trdev1);

	/* Write to device 2 */
	writeDevice(wfd2, buffer, len, &write2Offset, trdev2);

	/* Checksum source device */
	if (!noChecksum) 
	    CalcCRC32Std (buffer, len, &sdevCrc);
	i++;

	len = readDevice(rfd, srdev, i, buffer, BUF_SIZE,
			 &ioblksize, &readOffset);
    }

    if (len < 0) {
	sprintf(errmsg, "Failed to read source device %s", srdev);
	perror(errmsg);
	exit(1);
    }
    
    /* All ok */
    close(rfd);
    close(wfd1);
    close(wfd2);
    
    /* Checksum target devices */
    if (!noChecksum) {
	if (Checksum(trdev1, &tdev1Crc)) {
	    fprintf(stderr, "%s: unable to calculate checksum for %s\n",
		    argv[0], trdev1);
	    exit(1);
	}

	if (Checksum(trdev2, &tdev2Crc)) {
	    fprintf(stderr, "%s: unable to calculate checksum for %s\n",
		    argv[0], trdev2);
	    exit(1);
	}

	if (sdevCrc != tdev1Crc) {
	    fprintf(stderr,
		    "%s: source dev %s checksum %lu differs from target dev %s checksum %lu\n",
		    argv[0], srdev, sdevCrc, trdev1, tdev1Crc);
	    exit(1);
	}

	if (sdevCrc != tdev2Crc) {
	    fprintf(stderr,
		    "%s: source dev %s checksum %lu differs from target dev %s checksum %lu\n",
		    argv[0], srdev, sdevCrc, trdev2, tdev2Crc);
	    exit(1);
	}
    }
    
    if (debug != 1) 
#ifdef AFS_HPUX_ENV
	unfreezeAggr(sdevN);
#else
	unfreezeAggr(srdevN);
#endif

    (void) free ((void *)buffer);

    /* write out checksum */
    if (!noChecksum) {
	if ((int)strlen(checksumDataFile) > 0) {
	    if (strcmp(checksumDataFile, "stdout") == 0) {
		fd = stdout;
	    } else {
		fd = fopen(checksumDataFile, "w");
	    }
	    if (fd == (FILE *)NULL) {
		sprintf(errmsg,
			"unable to open checksum file %s", checksumDataFile);
		perror(errmsg);
		exit(1);
	    }
	    fprintf(fd, "%lu\n", sdevCrc);
	    fflush(fd);
	    if (strcmp(checksumDataFile, "stdout") != 0) {
		fclose(fd);
	    }
	}
    }
    return 0;
}
