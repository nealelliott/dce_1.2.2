/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: salvage_main.c,v $
 * Revision 1.1.96.1  1996/10/02  17:27:28  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:02  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1995, 1991 Transarc Corporation - All rights reserved. */

#include <dcedfs/param.h>
#include <dcedfs/osi_device.h>
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/ftutil.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/salvage/salvage_main.c,v 1.1.96.1 1996/10/02 17:27:28 damon Exp $")

#include <dcedfs/episode/async.h>
#include <dcedfs/episode/us.h>
#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/fid.h>
#include <dcedfs/episode/salvage.h>
#include <com_err.h>
#include <dcedfs/cmd.h>

#include <dcedfs/episode/boot.h>
#include <salvager.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsslvmac.h>
#include <dfsslvsvc.h>
#include <dfsslvmsg.h>

static salvage_DoSalvage = 0;		/* repair aggregate */
static salvage_DoVerify = 0;		/* check aggregate */
static salvage_DoRecovery = 0;		/* run recovery first */
static salvage_DisableDirScan = 0;
static salvage_Verbose = 0;
static salvage_Force = 0;		/* answer yes to confirmation reqs. */

#include <dcedfs/dfsftlmac.h>
#include <dcedfs/dfsftlsvc.h>
#include <dcedfs/dfsftlmsg.h>

void initialize_svc()
{
  error_status_t st;

  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);
  dfs_define_slv_message_tables();
}

/* VerifyEpisode -- checks out the specified device to see if it really looks
 *     like an Episode aggregate.  If so it initializes the user space runtime
 *     If anything goes wrong at this early point it just bails.  Then it calls
 *     the actual verify code with verify/salvage and [no]dirscan options set
 *     as specified.
 *
 * RETURN CODES -- The return value of this routine is to be passed directly to
 *     exit(2).  The severity is encoded in the low five bits.  The larger the
 *     number the worse the problem.  See salvage.h and salvage.c:
 *     epiz_GetExitCode for details. */

static int VerifyEpisode (
  IN int rfd,
  IN int wfd,
  IN char *RawDev,
  IN int isbsd,
  IN struct PartData *pdbsd,
  IN int isepi,
  IN struct PartData *pdepi,
  IN dev_t devNum,
  IN u_long SBno)
{
    struct async_device *aDev;
    long code;
    dev_t FakeUserDev;
    osi_off_t maxOffset;
    char ErrMsg[MAXPATHLEN + 100];
    int exitCode;
    int Problems;
    int Warnings;
    struct dirb_params dirbParams;

    exitCode = Problems = Warnings = 0;

    if (isepi < edsk_EMPTY) {
	dce_svc_printf(SLV_S_NOT_EPISODE_SYSTEM_MSG, RawDev);
	++Problems;
    }

    /* Check for both Episode and BSD superblocks present */

    else if (isbsd >= edsk_EMPTY) {
	if (pdbsd->FileSysCreateTime != 0) {
	    char                              *e;
	    error_status_t                    st;

	    e = (char *) ((isbsd == edsk_EMPTY)
			  ? dce_msg_get_msg(slv_s_empty, &st)
			  : dce_msg_get_msg(slv_s_not_empty, &st));
	    dce_svc_printf(SLV_S_BSD_SYSTEM_CREATED_MSG, 
			   RawDev,
			   e,
			   ctime((const time_t *)&pdbsd->FileSysCreateTime));
	    free(e);
	}

	if (pdbsd->FileSysMountedAs[0] != '\0')
	    dce_svc_printf(SLV_S_BSD_SYSTEM_LAST_MOUNTED_MSG, RawDev,
		    pdbsd->FileSysMountedAs);
	if (pdepi->FileSysCreateTime != 0)
	    dce_svc_printf(SLV_S_EPISODE_SYSTEM_CREATED_MSG, RawDev,
			ctime((const time_t *)&pdepi->FileSysCreateTime));
	if (pdbsd->FileSysCreateTime >= pdepi->FileSysCreateTime) {
	    ++Warnings;
	    dce_svc_printf(SLV_S_BSD_PREDATES_EPISODE_MSG, RawDev);
	}
	if (salvage_DoSalvage) dce_svc_printf(SLV_S_STDERR_CONTINUING_MSG);
    }

    /* only Episode file sys */

    else {
	char date[32];			/* so we can flush trailing newline */

	if (salvage_Verbose && (pdepi->FileSysCreateTime != 0)) {
	    strcpy(date, ctime((const time_t *)&pdepi->FileSysCreateTime));
	    date[strlen(date)-1] = 0;
	    dce_svc_printf(SLV_S_EPISODE_CREATE_TIME_MSG, date);
	}
	if (salvage_Verbose && (pdepi->lastVerifyTime != 0)) {
	    strcpy(date, ctime((const time_t *)&pdepi->lastVerifyTime));
	    date[strlen(date)-1] = 0;
	    dce_svc_printf(SLV_S_EPISODE_VERIFY_TIME_MSG,
			   date, pdepi->lastVerifyCode);
	}
    }

    if (Problems != 0) {
	dce_svc_printf(SLV_S_CANNOT_VERIFY_AGGREGATE_MSG, RawDev);
	return EXIT_CRASH;
    }

    if (pdepi->TotalBlocks*pdepi->BlockSize >
	pdepi->minBlkCount*pdepi->minBlkSize) {
	dce_svc_printf(SLV_S_DEVICE_TOO_SMALL_MSG,
		       pdepi->TotalBlocks*pdepi->BlockSize,
		       pdepi->minBlkCount*pdepi->minBlkSize);
	Warnings++;
    }

    if (salvage_Verbose)
	dce_svc_printf(SLV_S_DEVICE_SPECS_MSG,
		       major(devNum), minor(devNum),
		       pdepi->minBlkCount, pdepi->minBlkSize);
    if (salvage_Verbose)
	dce_svc_printf(SLV_S_BLOCK_SIZE_MSG,
		       pdepi->BlockSize, pdepi->FragmentSize,
		       pdepi->FirstBlock, pdepi->TotalBlocks);
    if (salvage_Verbose)
	dce_svc_printf(SLV_S_PRINCIPAL_SUPERBLOCK_MSG,
		       SBno, pdepi->NLogBlocks);

    if (Warnings > 0 && salvage_DoSalvage && !salvage_Force) {
	char Buf[100];
	char *cp;
	int OK = 0;

	dce_svc_printf(SLV_S_REPAIR_AGGREGATE_ANYWAY_MSG , RawDev);
	fflush(stderr);
	if (fgets(Buf, sizeof(Buf), stdin) == NULL)
	    goto answered;
	for (cp = Buf; *cp != '\0'; ++cp) {
	    if (isupper(*cp)) *cp = tolower(*cp);
	    if (!isalpha(*cp)) *cp = '\0';
	}
	if (strcmp(Buf, "yes") == 0)
	    OK = 1;
	else
	    OK = 0;
answered:
	if (OK == 0) {
	    dce_svc_printf(SLV_S_NOT_REPAIRING_AGGREGATE_MSG, RawDev);
	    return EXIT_CRASH;
	} else {
	    dce_svc_printf(SLV_S_REPAIRING_AGGREGATE_MSG, RawDev);
	}
    }

    DIRB_ELBB_PARAMS(dirbParams).bufferPool = (char *)malloc(1000000);
    if (DIRB_ELBB_PARAMS(dirbParams).bufferPool == NULL) {
	dce_svc_printf(SLV_S_NO_SPACE_FOR_BUFFER_POOL_MSG );
	return EXIT_CRASH;
    }
    DIRB_ELBB_PARAMS(dirbParams).byteCount = 1000000;
    DIRB_EPIG_PARAMS(dirbParams).maxHandles = 1000;
    dirbParams.abuffers = 16;

    code = dirb_Init(&dirbParams);
    if (code) {
	dce_svc_printf(SLV_S_ERROR_DURING_MODULE_INIT_MSG, dfs_dceErrTxt(code));
	fflush(stderr);
	return EXIT_CRASH;
    }

    /* Open device */
    /* Have to do this in the user-space simulator. */
    code = asevent_InitDevice(US_MAJOR_DEV, 1, pdepi->BlockSize, &aDev);
    if (code != 0) {
	dce_svc_printf(SLV_S_ERROR_DURING_INITDEVICE_MSG, dfs_dceErrTxt(code),
		       RawDev, US_MAJOR_DEV, 1, major(devNum), minor(devNum)); 
	fflush(stderr);
	return EXIT_CRASH;
    }
    FakeUserDev = osi_makedev(US_MAJOR_DEV, 1);
    code = us_ioctl(FakeUserDev, EPIU_IOCTL_SET_DIR, "/");
    if (code != 0) {
	dce_svc_printf(SLV_S_ERROR_CALLING_US_IOCTL_MSG, dfs_dceErrTxt(code)); 
	fflush(stderr);
	return EXIT_CRASH;
    }
    code = us_ioctl(FakeUserDev, EPIU_IOCTL_SET_FILE, &(RawDev[1]));
    if (code != 0) {
	dce_svc_printf(SLV_S_ANOTHER_US_IOCTL_ERROR_MSG, dfs_dceErrTxt(code),
		       &(RawDev[1])); 
	fflush(stderr);
	return EXIT_CRASH;
    }
    code = us_open(FakeUserDev, ((salvage_DoSalvage || salvage_DoRecovery)
				 ? US_OFLAGS_SYNC : US_OFLAGS_RDONLY));
    if (code != 0) {
	dce_svc_printf(SLV_S_ERROR_CALLING_US_OPEN_MSG,
		       dfs_dceErrTxt(code), RawDev); 
	fflush(stderr);
	return EXIT_CRASH;
    }
    maxOffset = ((osi_off_t) pdepi->minBlkCount) * ((osi_off_t) pdepi->minBlkSize);
    code = us_ioctl(FakeUserDev, EPIU_IOCTL_SET_MAX_OFFSET,
		    (caddr_t)&maxOffset);
    if (code != 0) {
	dce_svc_printf(SLV_S_ERROR_US_IOCTL_MAX_OFFSET_MSG,
		       dfs_dceErrTxt(code), RawDev, 
		       pdepi->minBlkCount, pdepi->minBlkSize);
	fflush(stderr);
	return 1;
    }

    if ((pdepi->lastVerifyTime != 0) &&
	((salvage_DoRecovery && !(salvage_DoSalvage || salvage_DoVerify)) ||
	 (!salvage_DoRecovery && salvage_DoVerify))) {

	/* If the last verify time/code info is valid and we are doing a
         * recovery only or a verify only check that the time was reasonably
         * recent and that the last code was okay. */

	u_int lastVerify = (osi_Time() - pdepi->lastVerifyTime) /
	    (3600*24);
	char date[32];
	time_t lastTime = pdepi->lastVerifyTime;
	long lastCode = pdepi->lastVerifyCode;

	if ((lastVerify > 100) ||
	    (lastCode != 0 && lastCode != 3)) {

	    /* Generate warning when the last verify was more than
	     * 100 days ago.  If the last verify produced an error,
	     * return that error from this command too.  */

	    strcpy(date, ctime(&lastTime));
	    date[strlen(date)-1] = 0;	/* flush trailing newline */
	    if (lastCode != 0 && lastCode != 3) {
		dce_svc_printf(SLV_S_LASTVERIFY_BAD_MSG,
			       date, lastCode);
		if (salvage_DoRecovery)
		    return lastCode;	/* preserve exit code */
	    } else {
		dce_svc_printf(SLV_S_LASTVERIFY_OLD_MSG,
			       date, lastCode);
		if (salvage_DoVerify)
		    dce_svc_printf(SLV_S_OLDVERIFY_RO_MSG);
	    }
	}
    }

    /*
    ** Recovery only.
    **
    ** If all we want to do is run recovery, just call epig_InitAggregate
    ** and return.  If either a salvage or a verify is needed too, the
    ** recovery will be done in epig_VerifyExistingAggregate().
    */
    if (salvage_DoRecovery && !(salvage_DoSalvage || salvage_DoVerify)) {
	epi_anode_t	avl;

	code = epig_InitAggregate(major(FakeUserDev), minor(FakeUserDev),
				  1,	/* initLog = TRUE */
				  0,	/* don't care about deviceP */
				  &avl);
	if (code) {
	    dce_svc_printf(SLV_S_RECOVER_ERROR_INITIALIZING_AGGREGATE_MSG,
			   dfs_dceErrTxt(code), RawDev); 
	    fflush(stderr);
	    exitCode = EXIT_CRASH;
	} else { 
	    code = epig_CloseAggregate(avl);
	    if (code) {
		dce_svc_printf(SLV_S_ERROR_CLOSING_AGGREGATE_MSG,
			       dfs_dceErrTxt(code), RawDev); 
		fflush(stderr);
		exitCode = EXIT_CRASH;
	    }
	}
	
	code = fsync(wfd);
	if (code &&
	    /* on AIX3.1 can't fsync raw disk device */
	    ((errno != EINVAL) || (strncmp (RawDev,"/dev/r",6) != 0))) {
	    char                *ErrMsg;
	    
	    ErrMsg  = (char *) dce_sprintf(slv_s_error_on_fsync, RawDev);
	    perror(ErrMsg);
	    free(ErrMsg);
	    return EXIT_CRASH;
	}
	
	(void) asevent_CloseDevice(aDev);
	
	return exitCode;
    }

    /*
    ** Salvage.
    **
    ** Call epig_VerifyExistingAggregate to start the whole affair.
    */

    /* BEGIN */ {
	struct epiz_salvageState salvageState;
	struct epiz_salvageState * ss = &salvageState;
	int somePaths;			/* true => some problem anodes */
	int exitCode = 0;

	bzero( (char *)ss, sizeof(*ss) );
	if (salvage_DoSalvage)
	    ss->flags |= EPIZ_SS_SALVAGE;
	if (salvage_DoRecovery)
	    ss->flags |= EPIZ_SS_RECOVER;
	if (salvage_Verbose)
	    ss->flags |= EPIZ_SS_VERBOSE;

	/* Use the -nodirscan option to disable directory scanning for
         * aggregates created with test_anode */
	if (! salvage_DisableDirScan)
	    ss->dirScanPass = epis_VerifyAggrPaths;
	else
	    ss->flags |= EPIZ_SS_NODIRSCAN;

	/* Do the epig_VerifyExistingAggregate call */
	code = epig_VerifyExistingAggregate (aDev, ss);
	if (code != 0) {
	    if (code != -1) {
		dce_svc_printf(SLV_S_ERROR_VERIFYING_AGGREGATE_MSG,
		dfs_dceErrTxt(code), RawDev); 
		fflush(stderr);
	    }
	    exitCode = EXIT_CRASH;
	}

	if (salvage_DoSalvage || salvage_DoRecovery) {
	    code = fsync(wfd);
	    if (code &&
		/* on AIX3.1 can't fsync raw disk device */
		((errno != EINVAL) || (strncmp (RawDev,"/dev/r",6) != 0))) 

	      {
		char                *ErrMsg;

		ErrMsg  = (char *) dce_sprintf(slv_s_cannot_fsync_device,
					       RawDev);
		perror(ErrMsg);
		free(ErrMsg);
		return EXIT_CRASH;
	      }
	}

	/*
	** Display all the path names we know about.  Returns 1 if there were
	** any.
        */
	epiz_DisplayAllNames(ss);

	if (ss->flags & EPIZ_SS_NODIRSCAN)
	    dce_svc_printf(SLV_S_VOLS_PROCESSED_MSG,
			   ss->nVolumes, ss->nAnodes);
	else
	    dce_svc_printf(SLV_S_DIRS_PROCESSED_MSG, ss->nVolumes, ss->nAnodes,
			   ss->nDirectories, ss->nFiles, ss->nAuxs);
	exitCode |= epiz_GetExitCode(ss);
	if (exitCode && !(ss->flags & EPIZ_SS_LOGCLEAN) &&
	    !(ss->flags & EPIZ_SS_SALVAGE)) {
	    /* repeat this warning */
	    if (ss->flags & EPIZ_SS_RECOVER)
		dce_svc_printf (SLV_S_CANNOT_RECOVER_MSG);
	    else
		dce_svc_printf (SLV_S_RECOVERY_NEEDED_MSG);
	}

	(void) asevent_CloseDevice(aDev);

	return exitCode;
    }
}

/* parameter indexes */
int arg_aggregate;
int arg_recover;
int arg_verify;
int arg_salvage;
int arg_force;
int arg_verbose;

/* ADD_PARM -- adds a new cmd library paramter, assigning the resulting parms
 *     array index to the variable provided. */

#define ADD_PARM(ts,index,name,type,flags,desc) \
    (index = (ts)->nParms, \
     cmd_AddParm (ts, name, type, flags, desc))

static int salvage(
  IN struct cmd_syndesc *as,
  IN char *arock)
{/* Accept a request to do a salvage operation. */
    int ac, rfd, wfd;
    char RawDev[MAXPATHLEN + 1];
    char ErrMsg[MAXPATHLEN + 100];
    struct PartData BSDData;
    struct PartData EpiData;
    int BSDRes, EpiRes;
    u_long SBAddr;
    dev_t DevNum;
    long code;
    struct stat StatBuf;
    ftu_devLock_t DevLock;
    int DidLock = 0;

    char * arg_Device = as->parms[arg_aggregate].items->data;

    if (arg_Device == 0) {
	dce_svc_printf(SLV_S_NEED_DEVICE_NAME_MSG);
	exit(2);
    }

    /* We allow -recoveronly to be specified in conjunction with either
     * -verifyonly or -salvageonly (this would produce the default behavior);
     * odd as these might sound.
     * 
     * The command parser would have disallowed specifying -verify and 
     * -salvage 
     */
    if (as->parms[arg_verify].items) {
	salvage_DoVerify = 1;		/* verify */
    } else if (as->parms[arg_salvage].items) {
	salvage_DoSalvage = 1;		/* salvage */
    }
    if (as->parms[arg_recover].items) {
	salvage_DoRecovery = 1;		/* recover */
    }
    
    /* note: default is salvage, recover, no verify */
    if (!salvage_DoVerify && !salvage_DoSalvage && !salvage_DoRecovery) {
	salvage_DoRecovery = 1;
	salvage_DoSalvage = 1;
    }

    if (as->parms[arg_verbose].items) {
	salvage_Verbose = 1;
    }
    if (as->parms[arg_force].items) {
	salvage_Force = 1;		/* force salvage */
    }

    rfd = edsk_GetRawDevice(arg_Device, RawDev, &DevNum);
    if (rfd < 0) {
	dce_svc_printf(SLV_S_INVALID_DEVICE_MSG, arg_Device);
	exit(EXIT_CRASH);
    }

    if (stat(RawDev, &StatBuf) == -1) {
	dce_svc_printf(SLV_S_BAD_STAT_MSG, dfs_dceErrTxt(errno), RawDev); 
	fflush(stderr);
	exit(EXIT_CRASH);
    }

    if (salvage_DoSalvage) {
	dce_svc_printf(SLV_S_SALVAGING_DEVICE_MSG, arg_Device );
    }
    else if (salvage_DoVerify) {
	dce_svc_printf(SLV_S_VERIFYING_DEVICE_MSG, arg_Device );
    }
    if (salvage_DoRecovery) {
	dce_svc_printf(SLV_S_RUN_RECOVERY_MSG, arg_Device);
    }


    /* Only lock it if it is a char. device and we're going to write to it */
    if (S_ISCHR(StatBuf.st_mode) && (salvage_DoSalvage || salvage_DoRecovery)){
	code = ftu_LockAggrDevice(RawDev, &DevLock);
	if (code) {
	    dce_svc_printf(SLV_S_ERROR_LOCKING_DEVICE_MSG, dfs_dceErrTxt(code));
	    fflush(stderr);
	    exit(EXIT_CRASH);
	}
	DidLock = 1;
    }

    ac = 0;

    bzero((char *)&BSDData, sizeof(BSDData));
    BSDRes = edsk_CheckAsBSD(rfd, RawDev, &BSDData);
    if (BSDRes)
	    edsk_DumpPartData(&BSDData);

    bzero((char *)&EpiData, sizeof(EpiData));
    EpiRes = edsk_CheckAsEpisode(rfd, RawDev, &EpiData, &SBAddr);
    if (EpiRes && salvage_Verbose)
	    edsk_DumpPartData(&EpiData);

    ac = 0;
    if (salvage_DoSalvage || salvage_DoRecovery) {
	wfd = open(RawDev, O_RDWR, 0);
	if (wfd < 0) 

	  {
	    char                *ErrMsg;

	    ac = errno;

	    ErrMsg  = (char *) dce_sprintf(slv_s_error_open_for_write, RawDev);
	    free(ErrMsg);
	    perror(ErrMsg);
	  }

      } 

    else wfd = rfd;

    if (!ac)
	ac = VerifyEpisode(rfd, wfd, RawDev, BSDRes, &BSDData,
			   EpiRes, &EpiData, DevNum, SBAddr);
    if (rfd != wfd) close(wfd);
    close(rfd);
    if (ac == 0) {
	if (salvage_DoSalvage || salvage_DoVerify) {
	    dce_svc_printf(SLV_S_DONE_CHECKS_OUT_MSG, RawDev);
	}
    } else {
	if (!salvage_DoSalvage && (ac == 1))
	    dce_svc_printf(SLV_S_DONE_SOME_INCONSISTENCIES_MSG, RawDev);
	else if (salvage_DoSalvage && (ac == 3))
	    dce_svc_printf(SLV_S_DONE_SOME_INCONSISTENCIES_REPAIRED_MSG,
			   RawDev);
	else if (salvage_DoSalvage && (ac == 7))
	    dce_svc_printf(SLV_S_DONE_INCOMPLETE_MSG, RawDev);
	else
	    dce_svc_printf(SLV_S_DONE_PROBLEM_MSG, RawDev,
			   epiz_GetExitMessage(ac), ac);
    }

    if (DidLock) {
	code = ftu_UnlockAggrDevice(DevLock);
	if (code) {
	    dce_svc_printf(SLV_S_ERROR_UNLOCKING_DEVICE_MSG,
			   dfs_dceErrTxt(code)); 
	    fflush(stderr);
	    if (ac == 0) ac = EXIT_CRASH;
	}
    }

    /* Don't bother to return, exit with code returned by VerifyEpisode */
    exit(ac);
}

int main(argc, argv)
  IN int argc;
  IN char **argv;
{
    register struct cmd_syndesc      *ts;
    char                              *cmd_explanation;
    error_status_t                    st;

    osi_setlocale(LC_ALL, "");

    initialize_svc();
    initialize_asy_error_table();
    initialize_lgb_error_table();
    initialize_epi_error_table();

    cmd_explanation = (char *) dce_msg_get_msg(slv_s_cmd_salvage, &st);
    ts = cmd_CreateSyntax((char *)0, salvage, 0, cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = (char *) dce_msg_get_msg(slv_s_cmd_name, &st);
    ADD_PARM(ts, arg_aggregate, "-aggregate", CMD_SINGLE, 0, cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = (char *) dce_msg_get_msg(slv_s_cmd_run_recovery, &st);
    ADD_PARM(ts, arg_recover, "-recoveronly", CMD_FLAG, CMD_OPTIONAL, cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = (char *) dce_msg_get_msg(slv_s_cmd_check_aggregate, &st);
    ADD_PARM(ts, arg_verify, "-verifyonly", CMD_FLAG, CMD_OPTIONAL, cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = (char *) dce_msg_get_msg(slv_s_cmd_salvage_no_recovery, &st);
    ADD_PARM(ts, arg_salvage, "-salvageonly", CMD_FLAG, CMD_OPTIONAL, cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = (char *) dce_msg_get_msg(slv_s_cmd_salvage_only, &st);
    (void)cmd_MakeAlternate(ts, "-verifyonly", cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = (char *) dce_msg_get_msg(slv_s_cmd_always_salvage, &st);
    ADD_PARM(ts, arg_force, "-force", CMD_FLAG, CMD_OPTIONAL, cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = (char *) dce_msg_get_msg(slv_s_cmd_verbose, &st);
    ADD_PARM(ts, arg_verbose, "-verbose", CMD_FLAG, CMD_OPTIONAL, cmd_explanation);
    free(cmd_explanation);

    return cmd_Dispatch(argc, argv);
}
