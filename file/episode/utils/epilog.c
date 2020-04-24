/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: epilog.c,v $
 * Revision 1.1.61.1  1996/10/02  17:42:28  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:05  damon]
 *
 * $EndLog$
 */
/* epilog.c */

/*
 * (C) Copyright 1996, 1992 Transarc Corporation
 * All Rights Reserved
 */

#include <stdio.h>
#include <dcedfs/param.h>
#include <dcedfs/lock.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <ctype.h>
#include <dcedfs/com_err.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/logbuf.h>
#include <dcedfs/episode/us.h>
#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/aggregate.h>
#include <dcedfs/episode/boot.h>
#include <dcedfs/cmdAux.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/utils/epilog.c,v 1.1.61.1 1996/10/02 17:42:28 damon Exp $")

/* force loading of osi_ctime */
#if __STDC__
#define osi_ctime(x) ctime((time_t *)x)
#else /* __STDC__ */
#define osi_ctime(x) ctime((long *)x)
#endif /* __STDC__ */

char *programName = "epilog";
#define READLOG_NAME programName

#define READLOG_COMMON_OFFSET 0
#define READLOG_AGGR_OFFSET (READLOG_COMMON_OFFSET+0)
#define READLOG_VERBOSE_OFFSET (READLOG_COMMON_OFFSET+1)
#define READLOG_QUIET_OFFSET (READLOG_COMMON_OFFSET+2)
#define READLOG_RECOVERDEBUG_OFFSET (READLOG_COMMON_OFFSET+3)

/* loginfo */
#define LOGINFO_OFFSET 6

/* readlog */
#define READLOG_OFFSET 6
#define READLOG_ADDLENGTH_OFFSET (READLOG_OFFSET+0)

/* recoverlog */
#define RECOVERLOG_OFFSET 6
#define RECOVERLOG_ADDLENGTH_OFFSET (RECOVERLOG_OFFSET+0)
#define RECOVERLOG_NOWRITE_OFFSET (RECOVERLOG_OFFSET+1)

/*
 * shared variables
 */

char *aggrDev;
char aggrName[1024];
int verbose;
int quiet;
extern int afsdb_episode_recover;

/* information about the log itself. */
daddr_t *logBlockArray;	/* array of blocks in log container */
u_int lbArraySize;


/*
 * CommonArgs - handle arguments common to all commands in this package.
 */
static CommonArgs(as, arock)
  struct cmd_syndesc *as;
  char *arock;
  {
      FILE *afd;

      /* mandatory arguments */
      aggrDev = as->parms[READLOG_AGGR_OFFSET].items->data;
      afd = fopen(aggrDev, "r");
      if ((afd == NULL) && (*aggrDev != '/')) {
	  strcpy(aggrName, "/dev/");
	  strcat(aggrName, aggrDev);
	  aggrDev = aggrName;
	  afd = fopen(aggrDev, "r");
      }
      if (afd == NULL) {
	  printf("bad file name %s\n",aggrDev);
	  return 2;
      }
      fclose(afd);
      
      /* optional arguments */
      quiet = as->parms[READLOG_QUIET_OFFSET].items != 0;
      verbose = quiet ? 0 : (as->parms[READLOG_VERBOSE_OFFSET].items != 0);
      if (as->parms[READLOG_RECOVERDEBUG_OFFSET].items)
	  afsdb_episode_recover =
	      as->parms[READLOG_RECOVERDEBUG_OFFSET].items->data;
      return 0; /* done! */
  }

  static int VerifyEpisode(rfd, wfd, RawDev, isbsd, pdbsd, isepi, pdepi,
			   devNum, FixBitmap, SBno)
  int rfd, wfd;
  char *RawDev;
  int isbsd, isepi;
  struct PartData *pdbsd, *pdepi;
  dev_t devNum;
  int FixBitmap;
  u_long SBno;
  {/* Make an Episode file system. */
      struct async_device *aDev;
      long code;
      u_long maxOffset;
      char *Data;
      dev_t FakeUserDev;
      char ErrMsg[500];
      int Problems;
      int Warnings;
      
      Problems = Warnings = 0;
      
      if (isepi < edsk_EMPTY) {
	  fprintf(stderr, "%s: Not an Episode file system.\n", RawDev);
	  ++Problems;
      } else if (isbsd >= edsk_EMPTY) {
	  /* both Episode and BSD superblocks present */
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
	  fprintf(stderr,
		  "%s: cannot verify an aggregate with these parameters.\n",
		  RawDev);
	  return 1;
      }
      
      if (verbose) {
	 fprintf(stderr,
		 "Device %s, major %d, minor %d; total %lu %lu-byte blocks.\n",
		  RawDev, osi_major(devNum), osi_minor(devNum),
		  pdepi->TotalBlocks, pdepi->BlockSize);
	 fprintf(stderr,
		 "Block size %lu, frag size %lu, firstBlock %lu, nBlks %lu\n",
		  pdepi->BlockSize, pdepi->FragmentSize, pdepi->FirstBlock,
		  pdepi->TotalBlocks);
	 fprintf(stderr, "Principal superblock at byte %lu, nLogBlocks %lu.\n",
		 SBno, pdepi->NLogBlocks);
      }
      return 0;
}

static FindLog(blockSize, totalBlocks)
  unsigned blockSize;
  unsigned totalBlocks;
  {
      int maj = 1;
      int min = 1;
      epi_anode_t avl, logHandle;
      long code;
      struct epia_info info;
      u_long  nLogBlocks;
      long blk;
      struct recvrRec *rcvrRecordArray;
      unsigned rcvrRecordCount;
      unsigned logIndex = 3; /* XXX: find a better way to get this! */
      struct aggregate *h;
      struct async_device *deviceP;
      struct recvrRec *recP;
      unsigned index;
      
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
      nLogBlocks = AFS_hgetlo(info.length) / deviceP->blkSize;
      /* if (expectedNLogBlocks && (nLogBlocks != expectedNLogBlocks))
       * { return (EPI_E_NOTANAGGREGATE); }
       */
      lbArraySize = nLogBlocks * sizeof(daddr_t);
      
      /* The log module keeps this array if all goes well. */
      logBlockArray = (daddr_t *) osi_Alloc (lbArraySize);
      for (blk=0; blk<nLogBlocks; blk++) {
	  /* All log pages must be pre-allocated.  There is a faster way to
	   * look up all these addresses... */
	  code = epia_Map (logHandle, blk, &logBlockArray[blk], /*device*/0);
	  if (code) { return (EPI_E_BADLOGBLOCKS); }
      }
      
      code = epia_Close(logHandle);
      return code;
  }

/*
 * readlog_LogInfo - provide information about an aggregate's log.
 *
 */
static readlog_LogInfo(as, arock)
  struct cmd_syndesc *as;
  char *arock;
  {
      int rfd;
      int code;
      struct PartData epiData, bsdData;
      char rawDev[1024], baseName[1024];
      u_long sbAddr;
      dev_t devNum, aDev;
      dev_t fakeUserDev;
      long bsdRes, epiRes;
      struct async_device *dP;
      struct epig_params epigParams;

      /*
       * CommonArgs will handle setting up the common names, assuring
       * the aggregate exists, etc.
       */
      code = CommonArgs();
      if (code)
	  return code;

      /* obtain a raw file descriptor for the aggregate. */
      
      rfd = edsk_GetRawDevice(aggrDev, rawDev, baseName, &devNum);
      if (rfd < 0 ) {
	  return rfd;
      }

      bzero(&bsdData, sizeof(bsdData));
      bsdRes = edsk_CheckAsBSD(rfd, rawDev, baseName, &bsdData);
      if (verbose) {
	  edsk_DumpPartData(&bsdData);
      }

      bzero(&epiData, sizeof(epiData));
      epiRes = edsk_CheckAsEpisode(rfd, rawDev, &epiData, &sbAddr);
      if (verbose) {
	  edsk_DumpPartData(&epiData);
      }

      code = VerifyEpisode(rfd, rfd, rawDev, bsdRes, &bsdData, epiRes,
			   &epiData, devNum, 0, sbAddr);
      if (code) {
	  com_err(READLOG_NAME, code,
		  "%s does not look like an Episode Aggregate\n",
		 aggrDev);
	  return code;
      }
      if (verbose) {
	  fprintf(stderr,
		  "Done.  %s checks out as an Episode aggregate.\n",
		  rawDev);
	  edsk_DumpPartData(&epiData);
      }

      /*
       * We believe we are looking at an Episode aggregate.  Now we
       * initialize Episode.
       */
      epigParams.elbbParams.bufferPool = osi_Alloc(1024 * 1024);
      epigParams.elbbParams.byteCount = 1024 * 1024;
      epigParams.maxHandles = 1000;
      code = epig_Init(&epigParams);
      if (code) {
	  com_err (READLOG_NAME, code, "during module initialization");
	  return 1;
      }

      /*
       * Now, we initialize the us_io package to point to the 
       * right files.
       */
      code = us_InitDevice (rawDev, epiData.BlockSize,
			    ((osi_off_t) epiData.TotalBlocks) * ((osi_off_t) epiData.BlockSize),
			    US_OFLAGS_RDONLY, &fakeUserDev, &aDev);
      if (code != 0) {
	  com_err (READLOG_NAME, code,
		   "Initializing device: %s (%d,%d) (%d,%d)",
		   rawDev, US_MAJOR_DEV, 1,
		   osi_major(devNum), osi_minor(devNum));
	  return 1;
      }

      code = FindLog(epiData.BlockSize, epiData.TotalBlocks);
      if (code != 0) {
	  com_err(READLOG_NAME, code,
		  "calling FindLog");
	  return 1;

      }

      printf("%s: %d blocks allocated to log\n",
	     rawDev, lbArraySize);

      if (verbose) {
	  unsigned index;
	  printf("%s: log blocks allocated at:\n", rawDev);
	  for (index = 0; index < lbArraySize; index++) {
	       printf("\t%d\n",*(logBlockArray+index));
	   }
      }

      /*
       * Now check the restart status of the log (e.g. is it clean?)
       */
      asevent_InitDevice(US_MAJOR_DEV, 1, 0, &dP);
      if (!dP) {
	  /* we need a large enough block size to read the restart area */
	  asevent_InitDevice(US_MAJOR_DEV, 1, 1024, &dP);
      }
      if (elbr_IsLogClean(dP,*logBlockArray)) {
	  printf("%s: log is clean\n",rawDev);
      } else {
	  printf("%s: log must be recovered\n",rawDev);
      }
      
      return 0;
  }

static readlog_Readlog()
  {
      return 0;
  }

static readlog_Recoverlog()
  {
      return 0;
  }


readlog_ui_SetupNormalDefaults(syntax)
  struct cmd_syndesc *syntax;
  {
      (void)cmd_Seek(syntax, READLOG_COMMON_OFFSET);
      (void)cmd_AddParm(syntax,"-aggr", CMD_SINGLE, 0, "aggregate path");
      (void)cmd_AddParm(syntax, "-verbose", CMD_FLAG, CMD_OPTIONAL, "");
      (void)cmd_AddParm(syntax, "-quiet", CMD_FLAG, CMD_OPTIONAL, "");
      (void)cmd_AddParm(syntax, "-setrecoverdebug", CMD_SINGLE, CMD_OPTIONAL,
			"value to use for debugging");
  }

readlog_ui_SetupLogInfo()
  {
      register struct cmd_syndesc *syntax;
      syntax = cmd_CreateSyntax("loginfo", readlog_LogInfo, 0,
				"Get information about this log");
      readlog_ui_SetupNormalDefaults(syntax);
  }

readlog_ui_SetupReadlog()
  {
      register struct cmd_syndesc *syntax;

      syntax = cmd_CreateSyntax("readlog", readlog_Readlog, 0, 
				"Dump contents of a log");
      readlog_ui_SetupNormalDefaults(syntax);
      (void)cmd_IndexedAddParm(syntax, "-addlength", CMD_SINGLE, 0,
			       "# of additional pages to look at",
			       READLOG_ADDLENGTH_OFFSET);
  }

readlog_ui_SetupRecoverlog()
  {
      register struct cmd_syndesc *syntax;

      syntax = cmd_CreateSyntax("recoverlog", readlog_Recoverlog, 0, 
				"Replay a log");
      readlog_ui_SetupNormalDefaults(syntax);
      (void)cmd_IndexedAddParm(syntax, "-addlength", CMD_SINGLE, CMD_OPTIONAL,
			       "# of additional pages to look at",
			       RECOVERLOG_ADDLENGTH_OFFSET);
      (void)cmd_IndexedAddParm(syntax, "-nowrite", CMD_FLAG, CMD_OPTIONAL,
			       "simulate recovery work",
			       RECOVERLOG_NOWRITE_OFFSET);
  }

readlog_ui_SetUpSyntax()
  {
      readlog_ui_SetupLogInfo();
      readlog_ui_SetupReadlog();
      readlog_ui_SetupRecoverlog();
  }

main(argc, argv)
  int argc;
  char **argv;
  {
      long code;

      initialize_asy_error_table();
      initialize_lgb_error_table();
      initialize_epi_error_table();
      initialize_cmd_error_table();

      readlog_ui_SetUpSyntax();

      code = cmd_Dispatch(argc, argv);
      if (code) {
	  com_err("readlog", code, (char *)0);
      }
      return code;
  }


