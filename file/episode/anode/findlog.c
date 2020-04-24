/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: findlog.c,v $
 * Revision 1.1.83.1  1996/10/02  17:17:28  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:27:16  damon]
 *
 * $EndLog$
 */
/* findlog.c */
/*
 * (C) Copyright 1996, 1991 Transarc Corporation
 * All Rights Reserved
 */
#include <dcedfs/param.h>
#include <dcedfs/osi_device.h>
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/debug.h>
#include <dcedfs/osi.h>

#include <com_err.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/logbuf.h>
/* #include "recover.h" */

/* user space I/O simulation */
#include <dcedfs/episode/us.h>

#include "anode.h"
#include "fixed_anode.h"
#include "aggregate.h"
/* #include <dcedfs/episode/anode_block.h> */

#ifdef DIOCGDINFO
/* define DKTYPENAMES to get the identifiers dktypenames[], DKMAXTYPES, fstypenames[], FSMAXTYPES */
/* #define DKTYPENAMES 1 */
#include <sys/disklabel.h>
#endif /* DIOCGDINFO */

#include "boot.h"

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/findlog.c,v 1.1.83.1 1996/10/02 17:17:28 damon Exp $")

#if __STDC__
#define osi_ctime(x) ctime((time_t *)x)
#else /* __STDC__ */
#define osi_ctime(x) ctime((long *)x)
#endif /* __STDC__ */


static char *arg_Device;
static FILE *blockFile;

static int ScanArgs(int argc, char **argv)
{
      static char UsageArgs[] = "[-setdebug #] deviceName blockListFile";
      int ac;
      extern int afsdb_episode_logbuf, afsdb_episode_recover;
      char hostname[80];

      arg_Device = NULL;
      if(argc <= 2) {
	  printf("Usage: %s %s\n", argv[0], UsageArgs);
	  return(2);
      }
      for (ac = 1; ac < argc && argv[ac][0] == '-'; ++ac) {
	  if (strcmp(argv[ac], "-setdebug") == 0) {
	      afsdb_episode_recover = afsdb_episode_logbuf = atoi(argv[++ac]);
	  } else {
	      printf("Bad option: ``%s''.  Usage: %s %s\n",
		     argv[ac], argv[0], UsageArgs);
	      return (2);
	  }
      }
      if (ac < argc) {
	  arg_Device = argv[ac];
	  ++ac;
      } else {
	  printf("Missing deviceName param\nUsage: %s %s\n", argv[0],
		 UsageArgs);
	  return 2;
      }
      if (ac < argc) {
	  blockFile = fopen(argv[ac],"w");
	  if (!blockFile) {
	      printf("bad file name %s\n",argv[ac]);
	      return 2;
	  }
	  gethostname(hostname, sizeof(hostname));
	  fprintf(blockFile,"this file has 3 lines of comments, aggregate, block size,\nmax block #, # of blocks & then 1 line/block\n");
	  fprintf(blockFile,"reading aggregate %s on %s\n",arg_Device, hostname);
	  fprintf(blockFile, "%s\n", arg_Device);
	  ++ac;
      } else {
	  printf("Missing blockFileName param\nUsage: %s %s\n", argv[0],
		 UsageArgs);
	  return 2;
      }
      if (ac < argc) {
	  printf("Excess args, starting with ``%s''.  Usage: %s %s\n",
		 argv[ac], argv[0], UsageArgs);
	  return (2);
      }
      return 0;
  }

static int VerifyEpisode(
  int rfd,
  char *RawDev,
  int isbsd,
  struct PartData *pdbsd,
  int isepi,
  struct PartData *pdepi,
  dev_t devNum,
  int FixBitmap,
  u_long SBno)
{/* Verify an Episode file system. */
    int Problems = 0;

    if (isepi < edsk_EMPTY) {
	fprintf(stderr, "%s: Not an Episode file system.\n", RawDev);
	++Problems;
    } else if (isbsd >= edsk_EMPTY) {		/* both Episode and BSD superblocks present */
	if (pdbsd->FileSysCreateTime != 0)
	    fprintf(stderr, "%s: %s BSD file sys created %s",
		    RawDev, (isbsd == edsk_EMPTY ? "Empty" : "*NON-EMPTY*"),
		    osi_ctime(&pdbsd->FileSysCreateTime));
	if (pdbsd->FileSysMountedAs[0] != '\0')
	    fprintf(stderr, "%s: BSD file sys last mounted as %s\n",
		    RawDev, pdbsd->FileSysMountedAs);
	if (pdepi->FileSysCreateTime != 0)
	    fprintf(stderr, "%s: Episode file sys created %s",
		    RawDev, osi_ctime(&pdepi->FileSysCreateTime));
	if (pdbsd->FileSysCreateTime >= pdepi->FileSysCreateTime) {
	    fprintf(stderr, "*** %s: BSD file system created more recently than Episode file system\n",
		    RawDev);
	}
	else if (FixBitmap) fprintf(stderr, "*** CONTINUING\n");
    } else {	/* only Episode file sys */
	if (pdepi->FileSysCreateTime != 0)
	    fprintf(stderr, "%s: Episode file sys created %s",
		    RawDev, osi_ctime(&pdepi->FileSysCreateTime));
    }

    if (Problems != 0) {
	fprintf(stderr, "%s: cannot verify an aggregate with these parameters.\n", RawDev);
	return 1;
    }

    fprintf(stderr, "Device %s, major %d, minor %d; total %lu %lu-byte blocks.\n",
	     RawDev, osi_major(devNum), osi_minor(devNum),
	     pdepi->TotalBlocks, pdepi->BlockSize);
    fprintf(stderr, "Block size %lu, frag size %lu, firstBlock %lu, nBlocks %lu\n",
	     pdepi->BlockSize, pdepi->FragmentSize, pdepi->FirstBlock, pdepi->TotalBlocks);
    fprintf(stderr, "Principal superblock at byte %lu*%d, nLogBlocks %lu.\n",
	     SBno, EPISODE_SUPERBLOCK_QUANTUM, pdepi->NLogBlocks);

    return 0;
}

FindLog(unsigned blockSize, unsigned totalBlocks)
  {
    int maj = 1;
     int min = 1;
     epi_anode_t avl, logHandle;
     long code;
     struct epia_info info;
     u_long  nLogBlocks;
     u_long length;
     daddr_t *logBlockArray;	/* array of blocks in log container */
     u_int lbArraySize;
     long blk;
     unsigned logIndex = 3; /* XXX: find a better way to get this! */
     struct async_device *deviceP;

     avl = (epi_anode_t)0;
     code = epig_InitAggregate(maj, min, 0, 0, &avl);
     if (code != 0) return code;

     code = asevent_InitDevice(maj, min, 0, &deviceP);
     if (code != 0) return code;

     code = epia_Open(avl, logIndex, 0, 0, &logHandle);
     if (code != 0) return code;


     code = epia_GetInfo(logHandle, &info);
     if (code != 0) return code;

     /* get conservative estimate of log size */
     if (!AFS_hfitsinu32(info.length)) { return (EPI_E_BADARGS); }
     length = AFS_hgetlo(info.length);
     nLogBlocks = length / deviceP->blkSize;
     /* if (expectedNLogBlocks && (nLogBlocks != expectedNLogBlocks))
      * { return (EPI_E_NOTANAGGREGATE); }
      */
     lbArraySize = nLogBlocks * sizeof(daddr_t);

     /* The log module keeps this array if all goes well. */
     logBlockArray = (daddr_t *) osi_Alloc (lbArraySize);
     fprintf(blockFile, "%d\n",deviceP->blkSize);
     fprintf(blockFile, "%d\n",totalBlocks);
     fprintf(blockFile, "%d\n",nLogBlocks);
     for (blk=0; blk<nLogBlocks; blk++) {
	 /* All log pages must be pre-allocated.  There is a faster way to
	  * look up all these addresses... */
	 code = epia_Map (logHandle, blk, &logBlockArray[blk], /*device*/0);
	 if (code) { return (EPI_E_BADLOGBLOCKS); }
	 /* new scheme is to just print the log record locations */
	 fprintf(blockFile, "%d\n",logBlockArray[blk]);
     }

     code = epia_Close(logHandle);
     return code;
 }

main(int argc, char **argv)
{

      /* Accept a request to do a scavenge operation. */
    int ac, rfd;
    char RawDev[MAXPATHLEN + 1];
    struct PartData BSDData;
    struct PartData EpiData;
    int BSDRes, EpiRes;
    u_long SBAddr;
    dev_t devNum, FakeUserDev;
    long code;
    char *bufData;
    struct async_device *aDev;
    struct epig_params epigParams;

    osi_setlocale(LC_ALL, "");
    ac = ScanArgs(argc, argv);
    if (ac != 0) exit(ac);

    rfd = edsk_GetRawDevice(arg_Device, RawDev, &devNum);
    if (rfd < 0) return rfd;

    bzero((caddr_t)&BSDData, sizeof(BSDData));
    BSDRes = edsk_CheckAsBSD(rfd, RawDev, &BSDData);
    printf("edsk_CheckAsBSD(``%s'') returns %d.\n", RawDev, BSDRes);
    edsk_DumpPartData(&BSDData);

    bzero((caddr_t)&EpiData, sizeof (EpiData));
    EpiRes = edsk_CheckAsEpisode(rfd, RawDev, &EpiData, &SBAddr);
    printf("edsk_CheckAsEpisode(``%s'') returns %d; SB@%lu*%d.\n", RawDev,
	   EpiRes, SBAddr, EPISODE_SUPERBLOCK_QUANTUM);
    edsk_DumpPartData(&EpiData);

    ac = VerifyEpisode(rfd, RawDev, BSDRes, &BSDData, EpiRes,
		       &EpiData, devNum, 0, SBAddr);
    if (ac == 0) {
	fprintf(stderr,
		"Done.  %s checks out as an Episode aggregate.\n",
		RawDev);
	edsk_DumpPartData(&EpiData);
    } else {
	fprintf(stderr,
		"Problem with verifying %s as an Episode aggregate.\n",
		RawDev);
    }
    close(rfd);

    bufData = (char *)osi_Alloc(1000000);
    if (!bufData) {
	fprintf(stderr, "findlog: cannot allocate space.\n");
	return 1;
    }
    epigParams.elbbParams.bufferPool = bufData;
    epigParams.elbbParams.byteCount = 1000000;
    epigParams.maxHandles = 1000;
    if (code = epig_Init(&epigParams)) {
	com_err ("checkaggr", code, "during module initialization");
	return 1;
    }

    /* Have to do this in the user-space simulator. */
    code = us_InitDevice (RawDev, EpiData.BlockSize,
			  (((osi_off_t)EpiData.TotalBlocks) * ((osi_off_t)EpiData.BlockSize)),
			  US_OFLAGS_RDONLY, &FakeUserDev, &aDev);
    if (code != 0) {
	com_err ("findlog", code,
		 "Initializing device: %s (%d,%d) (%d,%d)",
		 RawDev, US_MAJOR_DEV, 1, osi_major(devNum), osi_minor(devNum));
	return 1;
    }

    code = FindLog(EpiData.BlockSize, EpiData.TotalBlocks);
    if (code != 0) {
	com_err("findlog", code, "calling FindLog");
	return 1;
    }

    printf("finished findlog\n");

    exit(0);
}
