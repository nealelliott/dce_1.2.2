/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: readlog.c,v $
 * Revision 1.1.68.1  1996/10/02  17:25:25  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:40  damon]
 *
 * Revision 1.1.63.3  1994/07/13  22:20:16  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:46  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  20:12:46  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:57:05  mbs]
 * 
 * Revision 1.1.63.2  1994/06/09  14:01:19  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:34  annie]
 * 
 * Revision 1.1.63.2  1994/06/09  14:01:19  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:34  annie]
 * 
 * Revision 1.1.63.1  1994/02/04  20:13:57  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:42  devsrc]
 * 
 * Revision 1.1.61.1  1993/12/07  17:19:34  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  14:02:06  jaffe]
 * 
 * $EndLog$
 */
/* readlog.c */
/*
 * Copyright (C) 1993, 1991 Transarc Corporation
 * All Rights Reserved
 */
#include <dcedfs/param.h>
#include <dcedfs/osi_device.h>
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

#include <ctype.h>

#include <com_err.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>
#include <dcedfs/episode/async.h>
#include <logbuf.h>
#include <logrec.h>
#include <recover.h>

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */

/* user space I/O simulation */
#include <dcedfs/episode/us.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/logbuf/readlog.c,v 1.1.68.1 1996/10/02 17:25:25 damon Exp $")

#define INFILENAME_OPT 1
#define OUTFILENAME_OPT 2
#define RECOVER_OPT 3
#define SETLOGBUF_OPT 4
#define SETRECOVER_OPT 5
#define SETASYNC_OPT 6
#define CKSUMTYPE_OPT 7
#define DUMP_FULL_LOG_OPT 8
#define QUIET_OPT 9
#define VERBOSE_OPT 10

extern int afsdb_episode_logbuf;
extern int afsdb_episode_recover, afsdb_episode_async;
int verbLevel = 1;

/* macros to control printing verbosity level */
#define Printf(x) if (verbLevel > 0) printf x
#define VPrintf(x) if (verbLevel > 1) printf x
#define EPrintf(x) fprintf(stderr,x)


/*
 * RunReadlog - the actual loop called after parsing the command line
 * arguments.
 */
static int RunReadlog(struct cmd_syndesc *as, char *arock)
{
    long code;
    char *arg_BlockFile, *arg_OutFile;
    FILE *blockFile;
    char line[1024],device[1024];
    int maj = 1;
    int min = 1;
    u_long  nLogBlocks;
    daddr_t *logBlockArray;	/* array of blocks in log container */
    u_int lbArraySize;
    long blk;
    struct recvrRec *rcvrRecordArray;
    unsigned rcvrRecordCount;
    struct async_device *deviceP;
    struct recvrRec *recP;
    unsigned blockSize, totalBlocks;
    dev_t devNum, FakeUserDev;
    struct async_device *aDev;
    long tryRecovery;
    long dumpFullLog = 0;
    int cksumVer;
    
    arg_BlockFile = as->parms[INFILENAME_OPT].items->data;
    if (as->parms[OUTFILENAME_OPT].items) {
	arg_OutFile = as->parms[OUTFILENAME_OPT].items->data;
    }
    
    tryRecovery = (as->parms[RECOVER_OPT].items != 0);

    if (as->parms[SETLOGBUF_OPT].items) {
	afsdb_episode_logbuf = atoi(as->parms[SETLOGBUF_OPT].items->data);
    }
    
    if (as->parms[SETRECOVER_OPT].items) {
	afsdb_episode_recover = atoi(as->parms[SETRECOVER_OPT].items->data);
    }

    if (as->parms[SETASYNC_OPT].items) {
	afsdb_episode_async = atoi(as->parms[SETASYNC_OPT].items->data);
    }
    
    if (as->parms[CKSUMTYPE_OPT].items) {
	cksumVer = atoi(as->parms[CKSUMTYPE_OPT].items->data);
    } else {
	/* Provide default type as CRC32 type checksum */
	cksumVer = 1;
    }
    
    dumpFullLog = (as->parms[DUMP_FULL_LOG_OPT].items != 0);
    
    if (as->parms[QUIET_OPT].items) {
	verbLevel = 0;
    }
    
    if (as->parms[VERBOSE_OPT].items) {
	verbLevel = 2;
    }
    
    /*
     * Command line options are now parsed.  We proceed to read this
     * log.
     */
    
    blockFile = fopen(arg_BlockFile, "r");
    if (!blockFile) {
	EPrintf(("bad file name %s\n", arg_BlockFile));
	return 2;
    }
    
    /*
     * we depend on the format of this file being 3 lines of comments
     * which we ignore, one line telling where the actual aggregate is,
     * one line telling us how many blocks there are in the log, one
     * line telling us the # of blocks in the aggregate, and one
     * line for each block in the log telling us its block address.
     * This should correspond to the values written out by findlog in
     * the anode tree.
     */
    if(!fgets(line, sizeof(line), blockFile)) {
	return EBADF;
    }
    if(!fgets(line, sizeof(line), blockFile)) {
	return EBADF;
    }
    if(!fgets(line, sizeof(line), blockFile)) {
	return EBADF;
    }
    
    /* read the device name */
    if(!fgets(device, sizeof(device), blockFile)) {
	return EBADF;
    }
    /* replace the trailing newline with a null */
    device[strlen(device)-1] = '\0';
    
    /* and the block size */
    if(!fgets(line, sizeof(line), blockFile)) {
	return EBADF;
    }
    blockSize = atoi(line);
    
    /* and the total # of blocks */
    if(!fgets(line, sizeof(line), blockFile)) {
	return EBADF;
    }
    totalBlocks = atoi(line) ;
    
    /* the total # of log blocks */
    if(!fgets(line, sizeof(line), blockFile)) {
	return EBADF;
    }
    nLogBlocks = atoi(line);
    
    /* Have to do this in the user-space simulator. */
    code = us_InitDevice (device, blockSize,
			  ((osi_off_t) (totalBlocks + nLogBlocks)) * ((osi_off_t) blockSize),
			  tryRecovery ? US_OFLAGS_SYNC : US_OFLAGS_RDONLY,
			  &FakeUserDev, &aDev);
    if (code != 0) {
	com_err ("readlog", code,
		 "Initializing device: %s (%d,%d) (%d,%d)",
		 device, US_MAJOR_DEV, 1,
		 osi_major(devNum), osi_minor(devNum));
	return 1;
    }
    
    code = asevent_InitDevice(maj, min, 0, &deviceP);
    if (code != 0) return code;
    
    lbArraySize = nLogBlocks * sizeof(daddr_t);
    
    /* The log module keeps this array if all goes well. */
    logBlockArray = (daddr_t *) osi_Alloc (lbArraySize);
    for (blk=0; blk<nLogBlocks; blk++) {
	if(!fgets(line, sizeof(line), blockFile)) {
	    return EBADF;
	}
	logBlockArray[blk] = atoi(line);
	VPrintf(("# looking at block #%d\n",logBlockArray[blk]));
    }
    Printf(("# read %d blocks\n", nLogBlocks));
    
    if (!dumpFullLog) {
	code = elbr_DumpLog(deviceP, logBlockArray, nLogBlocks,
			    &rcvrRecordArray, &rcvrRecordCount);
    } else {
	code = elbr_DumpEntireLog(deviceP, logBlockArray, nLogBlocks,
				  &rcvrRecordArray, &rcvrRecordCount);
    }

    if (code) { return (BUF_BADLOG); }
    
    Printf((rcvrRecordCount == 1 ? "# found %d log record\n" :
	    "# found %d log records\n", rcvrRecordCount));
    
    if ( verbLevel > 0) {
	for(recP = rcvrRecordArray; recP; recP = recP->nextLogRec)
	    elbr_ScriptRecord(recP);
    }
    
    if (tryRecovery) {
	code = elbr_Recover(deviceP, deviceP, logBlockArray, nLogBlocks);
	if (code) { return (code); }
    }
    
    code = us_close(FakeUserDev, 0);
    return code;
}


/*
 * Set up the syntax of the command, call the command dispatcher.
 *
 */
main(int argc, char **argv)
{
    long code;
    struct cmd_syndesc *syntax;
    struct elbb_params elbbParams;
    
    /*
     * This command has only one sub-option, so we do not set the cmd name.
     * If future versions add sub-commands, this will need to be changed.
     */
    syntax = cmd_CreateSyntax((char *)0, RunReadlog, 0,
			      "Run readlog operation");
    afsl_Assert(syntax);
    
    code = cmd_IndexedAddParm(syntax, "-infile", CMD_SINGLE, CMD_REQUIRED,
			      "file describing the log & aggregate",
			      INFILENAME_OPT);
    afsl_MBZ(code);
    
    code = cmd_IndexedAddParm(syntax, "-outfile", CMD_SINGLE, CMD_OPTIONAL,
			      "file to write output", OUTFILENAME_OPT);
    afsl_MBZ(code);
    
    code = cmd_IndexedAddParm(syntax, "-recover", CMD_FLAG, CMD_OPTIONAL,
			      "recover log", RECOVER_OPT);
    afsl_MBZ(code);
    
    code = cmd_IndexedAddParm(syntax, "-setlogbufdebug", CMD_SINGLE,
			      CMD_OPTIONAL, "set logbuf debugging variable",
			      SETLOGBUF_OPT);
    afsl_MBZ(code);
    
    code = cmd_IndexedAddParm(syntax, "-setrecoverdebug", CMD_SINGLE,
			      CMD_OPTIONAL, "set recover debugging variable",
			      SETRECOVER_OPT);
    afsl_MBZ(code);
    
    code = cmd_IndexedAddParm(syntax, "-setasyncdebug", CMD_SINGLE,
			      CMD_OPTIONAL, "set async debugging variable",
			      SETASYNC_OPT);
    afsl_MBZ(code);
    
    code = cmd_IndexedAddParm(syntax, "-cksumtype", CMD_SINGLE, CMD_OPTIONAL,
			      "log check sum type", CKSUMTYPE_OPT);
    afsl_MBZ(code);
    
    code = cmd_IndexedAddParm(syntax, "-fulllog", CMD_FLAG, CMD_OPTIONAL,
			      "dump complete log", DUMP_FULL_LOG_OPT);
    afsl_MBZ(code);
    
    code = cmd_MakeAlternate(syntax, "-recover", "-fulllog");
    afsl_MBZ(code);
    
    code = cmd_IndexedAddParm(syntax, "-quiet", CMD_FLAG, CMD_OPTIONAL,
			      "suppress output", QUIET_OPT);
    afsl_MBZ(code);
    
    code = cmd_IndexedAddParm(syntax, "-verbose", CMD_FLAG, CMD_OPTIONAL,
			      "suppress output", VERBOSE_OPT);
    afsl_MBZ(code);
    
    
    code = cmd_MakeAlternate(syntax, "-quiet", "-verbose");
    afsl_MBZ(code);
    
    code = cmd_Dispatch(argc, argv);
    if (code) {
	com_err("readlog", code, (char *)0);
    }
    return code;
}
