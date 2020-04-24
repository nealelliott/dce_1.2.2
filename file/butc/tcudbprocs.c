/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tcudbprocs.c,v $
 * Revision 1.1.95.1  1996/10/02  17:06:23  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:32  damon]
 *
 * $EndLog$
*/
/*
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * tcudbprocs.c.c -- save and restore bak database to/from tape
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
#include <dcedfs/param.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/signal.h>
#include <sys/file.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/tcdata.h>
#include <dcedfs/budb.h>
#include <dcedfs/budb_errs.h>
#include <dcedfs/butc.h>
#include <dcedfs/bubasics.h>
#include <error_macros.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

extern int debugLevel;
static long writeTapeLabel(), writeDbDump(), restoreDbEntries();
static void initTapeBuffering();

/* saveDbToTape
 *	dump backup database to tape
 */

long
saveDbToTape(taskId)
     unsigned32 *taskId;
{
    struct butm_tapeInfo *tapeInfo=0;
    char tapeName[BU_MAXTAPELEN];
    struct butm_tapeLabel *oldTapeLabel=0;
    int tapeMounted=0;
    long code = 0;
    long mount_code=0;
    extern struct deviceSyncNode *deviceLatch;
    extern struct tapeConfig globalTapeConfig;

    tapeInfo = (struct butm_tapeInfo *)osi_Alloc(sizeof(struct butm_tapeInfo));
    if (tapeInfo == (struct butm_tapeInfo *)NULL) {
      (void)osi_Free(taskId, sizeof(unsigned32));
      return(TC_NOMEMORY);
    }
    oldTapeLabel = (struct butm_tapeLabel *)osi_Alloc(sizeof(struct butm_tapeLabel));
    if (oldTapeLabel == (struct butm_tapeLabel *)NULL) {
        (void)osi_Free(tapeInfo, sizeof(struct butm_tapeInfo));
	(void)osi_Free(taskId, sizeof(unsigned32));
	return(TC_NOMEMORY);
    }
    EnterDeviceQueue(deviceLatch);              /* lock tape device */
    clearStatus(*taskId, DRIVE_WAIT);
    
    tapeInfo->structVersion = CUR_BUTM_VERSION;
    code = butm_file_Instantiate( tapeInfo, &globalTapeConfig);
    if ( code )
	ERROR_EXIT(code);

    /* construct the name of the tape */
    sprintf(tapeName, "%s.%-d", DUMP_TAPE_NAME, 1);

    while ( 1 )
    { /*w*/

	/* need a writeable tape */
	code = PromptForTape(SAVEDBOPCODE, tapeName, 0, *taskId, 1);
	if (code)
	    ERROR_EXIT(code);

	if ( checkAbortByTaskId(*taskId) )
	     goto error_exit;

	code = butm_Mount(tapeInfo, tapeName);
        if ( code == BUTM_NOTAPE )
	{
	    /* cannot open drive, probably no tape */
	    dce_svc_printf(BAK_S_NO_TAPE_MSG,globalTapeConfig.device);
	    continue;
	}
	else if ( code )
	    ERROR_EXIT(code);
	tapeMounted=1;
	bzero((char *)oldTapeLabel, sizeof(*oldTapeLabel));
	code = butm_ReadLabel(tapeInfo, oldTapeLabel);
	if ( code )
	{
	  /* either no label exists, or can't read tape label. In both cases,
	     we just try overwriting with another label */
	  dce_svc_printf(BAK_S_BUTC_UNABLE_TO_RD_TAPE_LABEL_MSG , dfs_dceErrTxt(code));
	  dce_svc_printf(BAK_S_PROCEEDING_TO_WRITE_NEW_LABEL_MSG, DUMP_TAPE_NAME, 1);
	}
	else
	{ /*e*/
	    /* check the label */
	    if ( (strcmp(oldTapeLabel->name, "") != 0)
	    &&   (strcmp(oldTapeLabel->name, tapeName) != 0 )
	       )
	    {	
		/* unacceptable label */
	      dce_svc_printf(BAK_S_UNACCEPTABLE_TAPE_LABEL_MSG, oldTapeLabel->name);
		code = butm_Dismount(tapeInfo);
		if ( code )
		    ERROR_EXIT(code);
		tapeMounted=0;
		continue;
	    }
	} /*e*/

	code = writeTapeLabel(tapeInfo, DUMP_TAPE_NAME, 1, 0, 
			      oldTapeLabel->useCount);
	if ( code == BUTM_READONLY )
	{
	    code = butm_Dismount(tapeInfo);
	    if ( code )
		ERROR_EXIT(code);
	    tapeMounted=0;
	    dce_svc_printf(BAK_S_TAPE_WR_PROT_MSG, tapeName);
	    dce_svc_printf(BAK_S_PLS_INSERT_WRTABLE_TAPE_MSG);
	    continue;
	}
	else if ( code )
	    ERROR_EXIT(code);

	break;
    } /*w*/

    dce_svc_printf(BAK_S_GOOD_TAPE_NOW_DUMPING_DB_MSG);

    /* we have a writable tape */
    code = writeDbDump(tapeInfo, DUMP_TAPE_NAME);

error_exit:
    if (tapeMounted && (mount_code = butm_Dismount(tapeInfo)))
    	LogError("saveDbToTape: tape dismount failed\n", 
		 mount_code);
    if ( checkAbortByTaskId(*taskId) ) {
	setStatus(*taskId, ABORT_DONE|TASK_DONE);
	LogTape("Database dump aborted\n");
	LeaveDeviceQueue(deviceLatch);
	(void)osi_Free(tapeInfo, sizeof(struct butm_tapeInfo));
	(void)osi_Free(oldTapeLabel, sizeof(struct butm_tapeLabel));
	(void)osi_Free(taskId, sizeof(unsigned32));
	return(code);
    }

    /* notify user and log the event */
    if ( code ) {
	dce_svc_printf(BAK_S_DB_DUMP_HAD_ERRORS_MSG);
	LogError("Database dump failed\n", code);
	setStatus(*taskId, TASK_ERROR);
    }
    else {
	long curtime;

	curtime = time(0);
	dce_svc_printf(BAK_S_DB_DMP_DONE_MSG);
	LogTape("Database successfully dumped on %s\n", ctime(&curtime));
    }
    setStatus(*taskId, TASK_DONE);
    LeaveDeviceQueue(deviceLatch);
    (void)osi_Free(tapeInfo, sizeof(struct butm_tapeInfo));
    (void)osi_Free(oldTapeLabel, sizeof(struct butm_tapeLabel));
    (void)osi_Free(taskId, sizeof(unsigned32));
    return(code);
}

/* restoreDbFromTape
 *	restore the backup database from tape.
 */

long
restoreDbFromTape(taskId)
     long *taskId;
{
    struct dumpNode *node;
    struct butm_tapeInfo *tapeInfo=0;
    char tapeName[BU_MAXTAPELEN];
    struct butm_tapeLabel *oldTapeLabel=0;
    int tapeMounted=0;
    long code = 0;
    long mount_code=0;
    extern struct tapeConfig globalTapeConfig;
    extern struct deviceSyncNode *deviceLatch;

    tapeInfo = (struct butm_tapeInfo *)osi_Alloc(sizeof(struct butm_tapeInfo));
    if (tapeInfo == (struct butm_tapeInfo *)NULL)
      return(TC_NOMEMORY);
    oldTapeLabel = (struct butm_tapeLabel *)osi_Alloc(sizeof(struct butm_tapeLabel));
    if (oldTapeLabel == (struct butm_tapeLabel *)NULL) {
      (void)osi_Free (tapeInfo, sizeof(struct butm_tapeInfo));
      return (TC_NOMEMORY);
    }
    EnterDeviceQueue(deviceLatch);              /* lock tape device */
    clearStatus(*taskId, DRIVE_WAIT);

    tapeInfo->structVersion = CUR_BUTM_VERSION;
    code = butm_file_Instantiate( tapeInfo, &globalTapeConfig);
    if ( code )
	ERROR_EXIT(code);
      
    /* construct the name of the tape */
    sprintf(tapeName, "%s.%-d", DUMP_TAPE_NAME, 1);

    while ( 1 )
    { /*w*/

	/* need a tape to read */
	code = PromptForTape(RESTOREDBOPCODE, tapeName, 0, *taskId, 1);
	if (code)
	    ERROR_EXIT(code);

	if ( checkAbortByTaskId(*taskId) )
	    goto error_exit;

	code = butm_Mount(tapeInfo, tapeName);
	if ( code == BUTM_NOTAPE )
	{
	    /* cannot open drive, probably no tape */
	    dce_svc_printf(BAK_S_NO_TAPE_MSG,globalTapeConfig.device);
	    continue;
	}
	else if ( code )
	    ERROR_EXIT(code);
	tapeMounted=1;
	code = butm_ReadLabel(tapeInfo, oldTapeLabel);
	if ( (code != 0)
	||   (  (code == 0)
	     && (strcmp(oldTapeLabel->name, tapeName) != 0)
	     )
	   )
	{
	    dce_svc_printf(BAK_S_BAD_TAPE_BAD_LABEL_MSG);
	    code = butm_Dismount(tapeInfo);
	    if ( code )
		ERROR_EXIT(code);
	    tapeMounted=0;
	    continue;
	}
	      
	break;
    } /*w*/

    code = restoreDbEntries(tapeInfo, DUMP_TAPE_NAME);

    if ( code == 0 )
	dce_svc_printf(BAK_S_DB_RESTORE_DONE_MSG);
    else
    {
	dce_svc_printf(BAK_S_DB_RESTORE_DONE_WITH_ERR_MSG);
	LogError("Database restore finished with errors\n",
		 code);
    }

error_exit:
    if (tapeMounted && (mount_code = butm_Dismount(tapeInfo)))
    	LogError("restoreDbFromTape: tape dismount failed\n", 
		 mount_code);
    LeaveDeviceQueue(deviceLatch);

    if ( checkAbortByTaskId(*taskId) )
	setStatus(*taskId, ABORT_DONE);

    setStatus(*taskId, TASK_DONE);
    (void)osi_Free(tapeInfo, sizeof(struct butm_tapeInfo));
    (void)osi_Free(oldTapeLabel, sizeof(struct butm_tapeLabel));
    return(code);
}


/* support routines */

static long
writeTapeLabel(tapeInfo, tapeBaseName, tapeSeq, expirationDate, useCount)
     struct butm_tapeInfo *tapeInfo;
     char *tapeBaseName;
     long tapeSeq;
     long expirationDate;
     long useCount;
{
    static struct butm_tapeLabel newLabel;
    char *tapeName = 0;
    long tapeNameLength;
    long code = 0;

    /* require minimum of  .n<null> i.e. 3 more than base length */
    tapeNameLength = strlen(tapeBaseName) + 10;
    tapeName = (char *) osi_Alloc(tapeNameLength);
    if ( tapeName == (char *)NULL )
      return(TC_NOMEMORY);

    sprintf(tapeName, "%s.%-d", tapeBaseName, tapeSeq);

    bzero((char *)&newLabel, sizeof(newLabel));
    GetNewLabel( tapeInfo, tapeName, &newLabel );
    newLabel.expirationDate = 0;
    newLabel.useCount = useCount + 1;
    
    ResetTapeState( tapeInfo, tapeName);
    code = butm_Create(tapeInfo, &newLabel);

error_exit:
    (void)osi_Free(tapeName, tapeNameLength);
    return(code);
}
 
/* writeDbDump
 * notes:
 *	this code assumes that the blocksize on reads is smaller than
 *	the blocksize on writes
 */

static long
writeDbDump(tapeInfo, tapeName)
    struct butm_tapeInfo *tapeInfo;
    char * tapeName;
{
    int moreDataFlag;
    long blockSize;
    long writeBufNbytes = 0;
    char *writeBuffer = 0;
    char *writeBufPtr;
    long transferSize;
    char *readBufPtr;
    long maxReadSize;
    charListT *charList;
    long done;
    long code;
    extern struct udbHandleS udbHandle;

    maxReadSize = BUDB_MAX_CHAR_LIST;
    charList = (charListT *)osi_Alloc(sizeof(charListT));
    if (charList == (charListT *)NULL)
      return (TC_NOMEMORY);
    charList->charListT_len = 0;
    blockSize = BUTM_DATASIZE;
    writeBuffer = (char *) osi_Alloc(BUTM_BLOCKSIZE);
    if ( writeBuffer == NULL ) {
      osi_Free(charList, sizeof(charListT));
      return (TC_NOMEMORY);
    }

    /* have to write enclose the dump in file marks */
    code = butm_WriteFileBegin(tapeInfo);
    if ( code )
	ERROR_EXIT(code);

    writeBufPtr = writeBuffer + BUTM_HDRSIZE;
    moreDataFlag = 1;
    while ( moreDataFlag ) { /*w*/
      if ( charList->charListT_len == 0 ) {
	/* get the data */
	code = ubik_Call(BUDB_DumpDB,  udbHandle.uh_client,
				      UF_SINGLESERVER,
				      maxReadSize, &done, charList);
	if ( code ) {
	  LogError("writeDbDump: ubik_Call failed\n", code);
	  goto error_exit;	    
	}
	readBufPtr = (char *)charList->charListT_val;
	if ( debugLevel )
	  dce_svc_printf(BAK_S_READ_DONE_IS_MSG, charList->charListT_len,
		 done);
      }
      if ( (charList->charListT_len == 0) && (done != 0) )
	break;

      /* compute how many bytes we need to transfer to the write Buffer */
      if ( (blockSize - writeBufNbytes) > charList->charListT_len )
	transferSize = charList->charListT_len;
      else
	transferSize = blockSize - writeBufNbytes;

      bcopy(readBufPtr, writeBufPtr, transferSize);
      charList->charListT_len -= transferSize;
      writeBufPtr += transferSize;
      readBufPtr += transferSize;
      writeBufNbytes += transferSize;

      if ( done )
	break;

      if ( writeBufNbytes == blockSize ) {
	/* full buffer, write it out */
	code = butm_WriteFileData( tapeInfo, writeBuffer, blockSize);
	if ( code )
	  ERROR_EXIT(code);
	    
	if ( debugLevel ) {
	  int i;
	  for ( i = 1; i<= 20; i++)
	    printf("%x ", writeBuffer[i]);
	  printf("\n");
	}
	/* check if tape is full TBD */
	writeBufPtr = writeBuffer + BUTM_HDRSIZE;
	writeBufNbytes = 0;
      }
    } /*w*/
    
    /* no more data to be read - if necessary, flush out the last buffer */
    if ( writeBufNbytes > 0 )
    {
	code = butm_WriteFileData(tapeInfo, writeBuffer, blockSize);
	if ( code )
	    ERROR_EXIT(code);
	if ( debugLevel )
	{
	    int i;
	    for ( i = 1; i<= 20; i++)
		printf("%x ", writeBuffer[BUTM_HDRSIZE+i]);
	    printf("\n");
	}
    }

    code = butm_WriteFileEnd(tapeInfo);
    if ( code )
	ERROR_EXIT(code);

    code = ubik_Call(BUDB_DumpDB,  udbHandle.uh_client,
				  UF_END_SINGLESERVER, 0);
error_exit:
    (void)osi_Free(writeBuffer, BUTM_BLOCKSIZE);
    (void)osi_Free(charList, sizeof(charListT));
    return(code);
}


/* restoreDbEntries
 *	restore all the items on the tape
 * entry:
 *	tape positioned after tape label
 */

static long
restoreDbEntries(tapeInfo, tapeName)
    struct butm_tapeInfo *tapeInfo;
    char * tapeName;
{
    struct structDumpHeader netItemHeader, hostItemHeader;
    long more = 1;
    long code = 0;

    /* clear state for the buffer routine(s) */
    initTapeBuffering();

    code = butm_ReadFileBegin(tapeInfo);
    if ( code )
	ERROR_EXIT(code);    

    bzero((char *)&netItemHeader, sizeof(netItemHeader));

    /* get the first item */
    code = getTapeData(tapeInfo, tapeName, &netItemHeader,
		       sizeof(netItemHeader));
    if ( code )
	ERROR_EXIT(code);

    structDumpHeader_ntoh(&netItemHeader, &hostItemHeader);
    if ( debugLevel )
	printStructDumpHeader(&hostItemHeader);

    while ( more )
    {
	switch ( hostItemHeader.type )
	{
	  case SD_DBHEADER:
	    if ( debugLevel )
		dce_svc_printf(BAK_S_DUMP_HEADER_BUTC_MSG);
	    code = restoreDbHeader(tapeInfo, tapeName, &hostItemHeader);
	    if ( code )
		ERROR_EXIT(code);
	    break;

	  case SD_DUMP:
	    if ( debugLevel )
		fprintf(stderr, "Dump:\n");
	    code = restoreDbDump(tapeInfo, tapeName, &hostItemHeader);
	    if ( code )
		ERROR_EXIT(code);
	    break;

	  case SD_TAPE:
	  case SD_VOLUME:
	    ERROR_EXIT(-1);
	    break;

	  case SD_TEXT_DUMPSCHEDULE:
	  case SD_TEXT_VOLUMESET:
	  case SD_TEXT_TAPEHOSTS:
	    if ( debugLevel )
		fprintf(stderr, "Text: type %d\n", hostItemHeader.type);
	    restoreText(tapeInfo, tapeName, &hostItemHeader);
	    break;

	  case SD_END:
	    if ( debugLevel )
		dce_svc_printf(BAK_S_END_OF_DB_DUMP_MSG);
	    more = 0;
	    break;

	  default:
	    dce_svc_printf(BAK_S_HEADER_TYPE_MSG, hostItemHeader.type);
	    ERROR_EXIT(-1);
	    break;
	}
    }

    code = butm_ReadFileEnd(tapeInfo);
    if ( code )
	ERROR_EXIT(code);    
error_exit:
    return(code);
}

/* restoreDbHeader
 *	restore special items in the header
 */

restoreDbHeader(tapeInfo, tapeName, nextHeader)
     struct butm_tapeInfo *tapeInfo;
     char *tapeName;
     struct structDumpHeader *nextHeader;
{
    struct structDumpHeader netItemHeader;
    struct DbHeader netDbHeader, hostDbHeader;
    long code = 0;

    extern struct udbHandleS udbHandle;

    bzero((char *)&netDbHeader, sizeof(netDbHeader));
    code = getTapeData(tapeInfo, tapeName, &netDbHeader,
		       sizeof(netDbHeader));
    if ( code )
	ERROR_EXIT(code);

    DbHeader_ntoh(&netDbHeader, &hostDbHeader);
    if ( debugLevel )
	printDbHeader(&hostDbHeader);

    code = ubik_Call(BUDB_RestoreDbHeader, udbHandle.uh_client, UF_SYNC_SITE,
	             &hostDbHeader);
    if ( code )
	ERROR_EXIT(code);

    /* get the next item */
    bzero((char *)nextHeader, sizeof(*nextHeader));
    code = getTapeData(tapeInfo, tapeName, &netItemHeader,
		       sizeof(netItemHeader));
    if ( code )
        ERROR_EXIT(code);

    structDumpHeader_ntoh(&netItemHeader, nextHeader);
    
error_exit:
    return(code);
}


/* restoreDbDump
 *	restore a single dump, including all its tapes and volumes, from
 *	the tape.
 * entry:
 *	nextHeader - ptr to structure for return value
 * exit:
 *	nextHeader - next structure header from tape
 * notes: 
 *	upon entry, the dump structure header has been read confirming that
 *	a database dump tree exists on the tape
 */

restoreDbDump(tapeInfo, tapeName, nextHeader)
     struct butm_tapeInfo *tapeInfo;
     char *tapeName;
     struct structDumpHeader *nextHeader;
{
    struct budb_dumpEntry netDumpEntry, hostDumpEntry;
    struct budb_tapeEntry netTapeEntry, hostTapeEntry;
    struct budb_volumeEntry *hostVolumeArray;
    struct budb_dbVolume netVolumeEntry;
    struct structDumpHeader netItemHeader;
    long volCount,volIndex;
    long newTape;
    int restoreThisDump = 1;
    long code = 0;

    extern struct udbHandleS udbHandle;

    /* read dump */
    bzero((char *)&netDumpEntry, sizeof(netDumpEntry));
    code = getTapeData(tapeInfo, tapeName, &netDumpEntry,
		       sizeof(netDumpEntry));
    if ( code )
	ERROR_EXIT(code);

    dumpEntry_ntoh(&netDumpEntry, &hostDumpEntry);
    if ( debugLevel )
	printDumpEntry(&hostDumpEntry);

    /* add it to the database */
    code = ubik_Call(BUDB_CreateDump, udbHandle.uh_client, UF_SYNC_SITE,
		     &hostDumpEntry);
    if ( code )
    {
	if ( code == BUDB_DUMPIDEXISTS )
	{
	    /* continue with restore, but ignore this dump */
	    dce_svc_printf(BAK_S_DUMP_ID_NOT_ADDED_TO_DB_MSG, hostDumpEntry.id);
	    LogError("Dump id %d not added to database\n",
		     code, hostDumpEntry.id);
	    code = 0;
	    restoreThisDump = 0;
	}
	else
	    ERROR_EXIT(code);
    }

    /* get the next item */
    bzero((char *)nextHeader, sizeof(*nextHeader));
    code = getTapeData(tapeInfo, tapeName, &netItemHeader,
		       sizeof(netItemHeader));
    if ( code )
        ERROR_EXIT(code);

    structDumpHeader_ntoh(&netItemHeader, nextHeader);

    /* allocate array for host volumes */
    hostVolumeArray = (struct budb_volumeEntry *)
		      osi_Alloc(BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
    if ( !hostVolumeArray )
	ERROR_EXIT(TC_NOMEMORY);

    while ( nextHeader->type == SD_TAPE )
    { /*t*/
	/* add every tape to the db */
	if ( debugLevel )
	    fprintf(stderr, "Tape:\n");

	bzero((char *)&netTapeEntry, sizeof(netTapeEntry));
	code = getTapeData(tapeInfo, tapeName, &netTapeEntry, 
		           sizeof(netTapeEntry));
	if ( code )
	{
	    osi_Free(hostVolumeArray,
		     BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
	    ERROR_EXIT(code);
	}
	tapeEntry_ntoh(&netTapeEntry, &hostTapeEntry);
	if ( debugLevel )
	    printTapeEntry(&hostTapeEntry);

	if ( restoreThisDump != 0 )
	{
	    code = ubik_Call(BUDB_UseTape, udbHandle.uh_client, UF_SYNC_SITE,
			     &hostTapeEntry, &newTape);
	    if ( code )
	    {
		osi_Free(hostVolumeArray,
		         BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
		ERROR_EXIT(code);
	    }
	}
	
	/* now setup all the volumes */
	bzero((char *)nextHeader, sizeof(*nextHeader));
	code = getTapeData(tapeInfo, tapeName, &netItemHeader,
			   sizeof(netItemHeader));
	if ( code )
	{
	    osi_Free(hostVolumeArray,
		     BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
	    ERROR_EXIT(code);
	}
	
	structDumpHeader_ntoh(&netItemHeader, nextHeader);
 	
	volCount = 0;
	while ( nextHeader->type == SD_VOLUME )
	{ /*v*/
	    /* every tape consists of 0 or more volumes */
	    if ( debugLevel )
		fprintf(stderr, "Volume:\n");
	    bzero((char *)&netVolumeEntry, sizeof(netVolumeEntry));
	    code = getTapeData(tapeInfo, tapeName, &netVolumeEntry, 
			       sizeof(netVolumeEntry));
	    if ( code )
	    {
	        osi_Free(hostVolumeArray,
		         BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
		ERROR_EXIT(code);
	    }

	    volumeEntry_ntoh(&netVolumeEntry, &hostVolumeArray[volCount]);
	    if ( debugLevel )
		printVolumeEntry(&hostVolumeArray[volCount]);

	    if ( restoreThisDump != 0 )
	    {
		volCount++;
		if ( volCount == BU_MAXVOLCOUNT )
		{
		    /*
		     * try to add the volumes to the database in bulk, if the
		     * server is down level then add them one at a time
		     */

		    code = ubik_PVCall(BUDB_AddVolumes, udbHandle.uh_client,
					UF_SYNC_SITE, 2, /* provider version */
				        volCount,hostVolumeArray);
		    if ( code == UDOWNLVL )
		    {
			for ( volIndex = 0 ; volIndex < volCount ; volIndex++ )
			{
		            code = ubik_Call(BUDB_AddVolume,
					     udbHandle.uh_client, UF_SYNC_SITE,
				             &hostVolumeArray[volIndex]);
			    if ( code ) break;
			}
		    }

		    if ( code )
	            {
	                osi_Free(hostVolumeArray,
				BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
		        ERROR_EXIT(code);
	            }
		    volCount = 0;
		}
	    }
	    
	    /* get the next header */
	    bzero((char *)nextHeader, sizeof(*nextHeader));
	    code = getTapeData(tapeInfo, tapeName, &netItemHeader,
			       sizeof(netItemHeader));
	    if ( code )
	    {
	        osi_Free(hostVolumeArray,
			 BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
		ERROR_EXIT(code);
	    }
	    
	    structDumpHeader_ntoh(&netItemHeader, nextHeader);
	} /*v*/

	/*
	 * add any remaining volumes to the database before processing the
	 * next tape
	 */

	if ( volCount > 0 )
	{
	    /*
	     * try to add the volumes to the database in bulk, if the
	     * server is down level then add them one at a time
	     */

	    code = ubik_PVCall(BUDB_AddVolumes, udbHandle.uh_client,
				UF_SYNC_SITE, 2, /* provider version */
			        volCount,hostVolumeArray);
	    if ( code == UDOWNLVL )
	    {
		for ( volIndex = 0 ; volIndex < volCount ; volIndex++ )
		{
	            code = ubik_Call(BUDB_AddVolume,
				     udbHandle.uh_client, UF_SYNC_SITE,
			             &hostVolumeArray[volIndex]);
		    if ( code ) break;
		}
	    }

	    if ( code )
            {
	        osi_Free(hostVolumeArray,
			 BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
	        ERROR_EXIT(code);
            }
	}

	if ( restoreThisDump != 0 )
	{
	    code = ubik_Call(BUDB_FinishTape, udbHandle.uh_client, UF_SYNC_SITE,
			     &hostTapeEntry);
	    if ( code )
	    {
	        osi_Free(hostVolumeArray,
		         BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
		ERROR_EXIT(code);
	    }
	}
    } /*t*/

    /* free the array of host volumes */
    osi_Free(hostVolumeArray,
             BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));

    /* indicate that we are done */
    if ( restoreThisDump != 0 )
	code = ubik_Call(BUDB_FinishDump, udbHandle.uh_client, UF_SYNC_SITE,
			 &hostDumpEntry);
error_exit:
    return(code);
}

/* saveTextFile
 *	Save the specified file as configuration text in the ubik database.
 *	Have to setup the client text structure so that we can call
 *	the routine to transmit the text to the db.
 */

long
saveTextFile(textType, fileName)
     long textType;
     char *fileName;
{
    udbClientTextP ctPtr = 0;
    long code = 0;

    ctPtr = (udbClientTextP) osi_Alloc(sizeof(*ctPtr));
    if ( ctPtr == (udbClientTextP)NULL )
      return(TC_NOMEMORY);
    bzero((char *)ctPtr, sizeof(*ctPtr));

    ctPtr->textType = textType;

    /* lock the text in the database */
    code = bc_LockText(ctPtr);
    if ( code ) {
      (void)osi_Free(ctPtr, sizeof(*ctPtr));
      return (code);
    }
    ctPtr->textStream = fopen(fileName, "r");
    if ( ctPtr->textStream == NULL )
	ERROR_EXIT(-1);

    /* now send the text to the database */
    code = bcdb_SaveTextFile(ctPtr);
    if ( code )
	ERROR_EXIT(code);

    (void)fclose(ctPtr->textStream);
error_exit:
    code = bc_UnlockText(ctPtr);
    (void)osi_Free((char *)ctPtr, sizeof (*ctPtr));
    return(code);
}

/* restoreText
 *	read the text off the tape, and store it in the appropriate
 *	text type in the database.
 * entry:
 *	nextHeader - ptr to struct for return information
 * exit:
 * 	nextHeader - struct header for next item on the tape
 */

restoreText(tapeInfo, tapeName, nextHeader)
     struct butm_tapeInfo *tapeInfo;
     char *tapeName;
     struct structDumpHeader *nextHeader;
{
    char filename[64];
    long nbytes;
    char *readBuffer = (char *)0;
    long transferSize;
    struct structDumpHeader netItemHeader;
    int fid = -1;
    long code = 0;
    
    udbClientTextP ctPtr = (udbClientTextP)NULL;
    long textType;

    ctPtr = (udbClientTextP) osi_Alloc(sizeof(*ctPtr));
    if ( ctPtr == (udbClientTextP)NULL )
      return(TC_NOMEMORY);

    /* determine the type of text block */
    switch ( nextHeader->type )
    {
      case SD_TEXT_DUMPSCHEDULE:
	textType = TB_DUMPSCHEDULE;
	break;

      case SD_TEXT_VOLUMESET:
	textType = TB_VOLUMESET;
	break;

      case SD_TEXT_TAPEHOSTS:
	textType = TB_TAPEHOSTS;	
	break;

      default:
	ERROR_EXIT(-1);
	break;
    }

    /* open the text file */
    strcpy(filename, "/tmp/bu_XXXXXX");
    fid = open((char *)mktemp(filename), O_RDWR|O_CREAT|O_EXCL, 0600);
    if ( fid < 0 )
	ERROR_EXIT(-1);

    /* place the text into a file */
    readBuffer = (char *) osi_Alloc(BUTM_DATASIZE);
    if ( readBuffer == (char *)NULL ) {
      (void)osi_Free(ctPtr, sizeof(udbClientTextT));
      close(fid);
      return(TC_NOMEMORY);
    }
    nbytes = nextHeader->size;
    while ( nbytes > 0 )
    {
	transferSize = (BUTM_DATASIZE < nbytes) ? BUTM_DATASIZE : nbytes;

	/* read it from the tape */
	code = getTapeData(tapeInfo, tapeName, readBuffer, transferSize);
	if ( code )
	    ERROR_EXIT(code);

	/* write to the file */
	if (write(fid, readBuffer, transferSize) != transferSize)
	    ERROR_EXIT(-1);

	nbytes -= transferSize;
    }

    close(fid);	
    fid = -1;    
    code = saveTextFile(textType, filename);
    if ( code )	
	ERROR_EXIT(code);
    unlink(filename);	

    /* get the next header */
    bzero((char *)nextHeader, sizeof(*nextHeader));
    code = getTapeData(tapeInfo, tapeName, &netItemHeader,
		           sizeof(netItemHeader));
    if ( code )
        ERROR_EXIT(code);

    structDumpHeader_ntoh(&netItemHeader, nextHeader);

error_exit:
    (void)osi_Free(ctPtr, sizeof(*ctPtr));
    (void)osi_Free(readBuffer, BUTM_DATASIZE);
    if ( fid != -1 )
	close(fid);
    return(code);
}


/* ----------------------------------
 * Tape data buffering - for reading database dumps
 * ----------------------------------
 */

static char *tapeReadBuffer = 0;		/* input buffer */
static char *tapeReadBufferPtr = 0;		/* position in buffer */
static long blockSize = BUTM_DATASIZE;		/* read block size */
static long nbytes = 0;				/* # bytes left in buffer */

static void
initTapeBuffering()
{
    nbytes = 0;
}

/* getTapeData
 *	Read information from tape, and place the requested number of bytes
 *	in the buffer supplied 
 * entry:
 *	tapeInfo
 *	tapeName - base name of tape. seq. of 1 assumed on entry.
 *	buffer - buffer for requested data
 *	requestedBytes - no. of bytes requested
 * exit:
 *	fn retn - 0, ok, n, error
 */

getTapeData(tapeInfo, tapeName, buffer, requestedBytes)
     struct butm_tapeInfo *tapeInfo;
     char *tapeName;
     char *buffer;
     long requestedBytes;
{
    long transferBytes;
    long code = 0;

    if ( tapeReadBuffer == NULL )
    {
	tapeReadBuffer = (char *)osi_Alloc(BUTM_BLOCKSIZE);
	if ( tapeReadBuffer == NULL )
	  return(TC_NOMEMORY);
    }

    while ( requestedBytes > 0 )
    {
	if ( nbytes == 0 )
	{
	    /* get more data */
	    code = butm_ReadFileData(tapeInfo, tapeReadBuffer, blockSize,
				     &nbytes);

	    /* deal with tape change */

	    if ( code )
		ERROR_EXIT(code);

	    tapeReadBufferPtr = tapeReadBuffer + BUTM_HDRSIZE;
	}
	
	/* compute min(nbytes, requestedBytes) */
	transferBytes = (nbytes < requestedBytes ) ? nbytes : requestedBytes;

	/* copy out */
	bcopy(tapeReadBufferPtr, buffer, transferBytes);
	tapeReadBufferPtr += transferBytes;
	buffer += transferBytes;
	nbytes -= transferBytes;
	requestedBytes -= transferBytes;
    }

error_exit:
    return(code);
}






