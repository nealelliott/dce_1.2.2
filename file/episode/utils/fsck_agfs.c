/*
 * HISTORY
 * $Log$
 * $EndLog$
 */
/*
 * (C) Copyright 1994 Transarc Corporation
 * All Rights Reserved
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/vfstab.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <dcedfs/stds.h>
    
RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/utils/RCS/fsck_agfs.c,v 1.7 1994/12/27 17:46:33 cfe Exp $")

#define KBYTE 1024
#define EP_BUF_SIZE 256*KBYTE
#define SALVAGER_NO_VERIFY "/opt/dcelocal/etc/salvage_no_verify"
extern void split_up(char *, char *);
extern int get_vfs(char *, struct vfstab, char *);


/*
 * This program will either invoke the salvager or will examine the
 * aggr for the presence of unrecovered log records.  If it invokes the
 * salvager it either runs salvage -recoveronly -verifyonly, or it invokes
 * it with only the specific options specified via -o<salvager_options>.
 */

int interpret_status(int status)
{
    /*
     * Must match fsck(8) return codes.
     *  0 - All clear
     *  1 - Bad parameter specified
     * 32 - Need another attempt with bigger hammer
     * 33 - device is already mounted somewhere
     * 34 - device does not exist (can not stat)
     * 36 - uncorrectable errors (normal return)
     * 37 - signal caught during check
     * 39 - uncorrectable errors (immediate return)
     * 40 - for root fs same as 0
     */

    /* signal data in low byte, notice if died from signal */
    if (WIFSIGNALED(status))
	return(37);

    /* otherwise exit status in in top byte */
    status = WEXITSTATUS(status);

    if ((status == 0) || (status & 0x02))
	return(0);

    if ((status & 0x01) || (status & 0x04))
	return(32);

    if (status & 0x08)
	return(36);
	
    if (status & 0xf0)
	return(34);	/* note could also be 33 */

    fprintf(stderr, "Uninterpretable return from salvage: 0x%x\n", status);
    return(39);
}

main(int argc, char *argv[])
{
    char *extra_args=NULL, cmd[BUFSIZ], exargs[BUFSIZ], switches[BUFSIZ];
    int check_mountable = 0;
    int  status;

    /* step over prg name */
    argc--; argv++;

    /* process '-' prefixed arguments */
    while (argc && argv[0][0] == '-') {
	switch(argv[0][1]) {
	  case 'm':
	    check_mountable++;
	    argc--; argv++;
	    break;

	  case 'n':
	  case 'N':	
	  case 'y':
	  case 'Y':
	    argc--; argv++;
	    break;

	  case 'o':
	    argc--; argv++;
	    extra_args = *argv;
	    split_up(extra_args, exargs);
	    argc--; argv++;
	    break;

	  default:
	    fprintf(stderr,
		    "agfs usage: fsck [-F agfs] [-V] [{-m | -o <specific_options>}] [special...]\n");
	    argc--; argv++;
	    exit(1);
	    /*NOTREACHED*/
	}
    }

    /*
     * at this point argc should be 1 and the remaining arg is the special
     * device to check.
     */
    if (argc != 1) {
	fprintf(stderr, "fsck_agfs: Incorrect invocation\n");
	exit(1);
    }

    /* Handle "-m" switch here and ignore the rest */
    if (check_mountable) {
	/*
	 * We go off and check if this aggr is clean (no log to recover)
	 * and return immediately.  No changes to the aggr, and no salvager
	 * invocation required.  Exit codes will be:
	 *  0: aggr is clean and may be mounted (exported).
	 *  1: bad parameters specified
	 * 32: aggr is dirty (has log to recover).
	 * 33: aggr is already mounted.
	 * 34: bad device specified.
	 * 36: uncorrectable error
	 *
	 * Any non-zero exit code will cause the /usr/sbin/mountall to invoke
	 * a single-user shell to let the user fix things up.
	 */
	status = check_log(*argv);
	switch(status) {
	  case 0:
	    exit(0);

	  case ENXIO:
	    exit(1);

	  case 1:
	    exit(32);

	  case EBUSY:
	    exit(33);

	  case EINVAL:
	    exit(34);

	  case EIO:
	    exit(36);

	  default:
	    exit(1);

	}
	/* NOTREACHED */
    }

    /* no -m specified.  run the salvager */
    if (extra_args != NULL) {
	sprintf(cmd, "/opt/dcelocal/bin/salvage %s %s", *argv, exargs);
    } else {
	status = open(SALVAGER_NO_VERIFY, O_RDONLY);
	if (status > 0) {
	    /*
	     * presence of this file means admin wants to omit the verify
	     * pass in the salvager
	     */
	    (void) close(status);
	    strcpy(switches, "-recoveronly ");
	} else {
	    strcpy(switches, "-recoveronly -verifyonly ");
	}
	sprintf(cmd, "/opt/dcelocal/bin/salvage %s %s",	*argv, switches);
    }
    status = system(cmd);
    exit(interpret_status(status));
}


#include <pthread.h>
#include <dcedfs/osi.h>
#include <dcedfs/stds.h>
#include <dcedfs/param.h>
#include <dcedfs/debug.h>
#include <dcedfs/ftutil.h>
#include <dcedfs/osi_device.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/logbuf.h>

/* user space I/O simulation */
#include <dcedfs/episode/us.h>
#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/aggregate.h>
#include <dcedfs/episode/boot.h>

int check_log(char *devName)
{

    char 			RawDev[MAXPATHLEN + 1], *bufData;
    dev_t 			DevNum, FakeUserDev;
    ftu_devLock_t 		DevLock;
    struct PartData 		EpiData;
    struct epig_params 		epigParams;
    struct epig_status		epigStatus;
    epi_anode_t 		avl = (epi_anode_t)0;
    epi_anode_t			logHandle = (epi_anode_t)0;
    u_long 			SBAddr, logIndex;
    daddr_t 			first;
    int 			rfd, EpiRes, code, clean;
    int 			maj = 1;
    int				min = 1;


    /* get/ensure raw device name, open read-only */
    rfd = edsk_GetRawDevice(devName, RawDev, &DevNum);
    if (rfd < 0) {
	fprintf(stderr, "fsck_agfs: Device %s is invalid\n", devName);
	return(ENXIO);
    }

    /* lock the device to prevent touching in-use devices */
    code = ftu_LockAggrDevice(RawDev, &DevLock);
    if (code) {
	fprintf(stderr,
		"fsck_agfs: %s is a mounted file system, ignored\n",
		RawDev);
	return(EBUSY);
    }

    /* sanity check the device as episode and load up basic aggr info */
    /* using rfd, get descriptive data about partition */
    bzero((char *)&EpiData, sizeof(EpiData));
    EpiRes = edsk_CheckAsEpisode(rfd, RawDev, &EpiData, &SBAddr);
    if (EpiRes != edsk_FULL) {
	fprintf(stderr,
		"fsck_agfs: Device %s does not contain an LFS aggregate\n",
		RawDev);
	return(EINVAL);
    }

    /* Don't need the device open anymore */
    code = close(rfd);
    if (code != 0) {
	fprintf(stderr,	"fsck_agfs: Error in close: %s\n", RawDev);
	return(EIO);
    }

    /* allocate buffer space for the user-runtime */
    bufData = (char *)osi_Alloc(EP_BUF_SIZE);
    if (!bufData) {
	fprintf(stderr, "fsck_agfs: Cannot allocate buffer space\n");
	return(EIO);
    }

    epigParams.elbbParams.bufferPool = bufData;
    epigParams.elbbParams.byteCount = EP_BUF_SIZE;
    epigParams.maxHandles = 1000;

    code = epig_Init(&epigParams);
    if (code) {
	fprintf(stderr, "fsck_agfs: Error during user-runtime initialization");
	return(EIO);
    }

    /* Initialize the user-space runtime using EpiData, open RawDev RONLY */
    code = us_InitDevice (RawDev, EpiData.BlockSize,
			  ((osi_off_t) EpiData.TotalBlocks) * ((osi_off_t) EpiData.BlockSize),
			  US_OFLAGS_RDONLY, &FakeUserDev, NULL);
    if (code) {
	fprintf(stderr,	"fsck_agfs: Error initializing device: %s\n", RawDev);
	return(EIO);
    }

    /* Init the aggr as if it were device (1,1) */
    /* With the user space initialized, we can get an aggr handle anode */
    code = epig_InitAggregate(maj, min, 0, 0, &avl);
    if (code != 0) {
	fprintf(stderr,	"fsck_agfs: Error in InitAggregate: %s\n", RawDev);
	return(EIO);
    }
      
    /* find the logIndex via get status */
    code = epig_GetAggregateStatus(avl, &epigStatus);
    if (code != 0) {
	fprintf(stderr,	"fsck_agfs: Error in epig_GetAggregateStatus: %s\n",
		RawDev);
	return(EIO);
    }

    logIndex = epigStatus.logIndex;

    /* use avl to get a handle (anode) on the log */
    code = epia_Open(avl, logIndex, 0, 0, &logHandle);
    if (code != 0) {
	fprintf(stderr,	"fsck_agfs: Error in epia_Open: %s\n", RawDev);
	return(EIO);
    }
      
    /* Find the first page of the log */
    code = epia_Map(logHandle, 0, &first, NULL);
    if (code != 0) {
	fprintf(stderr,	"fsck_agfs: Error in epia_Map: %s\n", RawDev);
	return(EIO);
    }

    /* Test for a restart record */
    clean = elbr_IsLogClean (epia_AnodeDevice(logHandle), first);

    /* close down log anode now */
    code = epia_Close(logHandle);
    if (code != 0) {
	fprintf(stderr,	"fsck_agfs: Error in epia_Close: %s\n", RawDev);
	return(EIO);
    }
    
    /* close down aggregate anode now */
    code = epig_CloseAggregate(avl);
    if (code != 0) {
	fprintf(stderr,	"fsck_agfs: Error in epig_CloseAggregate: %s\n",
		RawDev);
	return(EIO);
    }

    /* release lock on aggregate */
    code = ftu_UnlockAggrDevice(DevLock);
    if (code) {
	fprintf(stderr, "fsck_agfs: Error in ftuUnlockAggrDevice: %s\n",
		RawDev);
	return(EIO);
    }

    if (clean) {
	fprintf(stdout, "agfs fsck: sanity check: %s okay\n",
		RawDev);
	return(0);
    } else {
	fprintf(stdout, "agfs fsck: sanity check: %s needs checking\n",
		RawDev);
	return(1);
    }
}
