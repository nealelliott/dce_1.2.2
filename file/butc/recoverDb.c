/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: recoverDb.c,v $
 * Revision 1.1.71.1  1996/10/02  17:06:13  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:27  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1996 Transarc Corporation
 * All Rights Reserved
 */
/*
 * recoverDb.c -- 
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <dcedfs/param.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/budb_errs.h>
#include <dcedfs/tcdata.h>
#include <dcedfs/bubasics.h>
#include <com_err.h>
#include <string.h>
#include <error_macros.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/butc/recoverDb.c,v 1.1.71.1 1996/10/02 17:06:13 damon Exp $")

#define BELLCHAR 7      			/* ascii for bell */

/* GLOBAL CONFIGURATION PARAMETERS */
extern int autoQuery;
extern int queryoperator;

#define	dfprintf(x)
extern char bakDefaultConfDir[];
extern int debugLevel;
int readTape(), AddScanToDB(), debugPrintTapeLabel(), debugPrintVolumeHeader();
extern int ExtractTrailer();
/* Handle for the information read from all the tapes of a dump */
struct tapeScanInfo
{
    struct tapeScanHeader *headers;
};

/* One of these is created for each tape in the dump */
struct tapeScanHeader
{
    struct tapeScanHeader *next;
    struct butm_tapeLabel *label;
    struct tapeScanVolHeader *volumeHead;
};

/* One of these is created for each volume on the tape, and is chained onto
 * the tapeScanHeader
 */
struct tapeScanVolHeader
{
    struct tapeScanVolHeader *next;
    struct volumeHeader *header;
    long tapePosition;			    /* position on the tape */
    long lastFragment;			    /* last fragment of the volume */
};


/* ---- structures for data capacity evaluation */

/* Notes:
 *	1) doesn't count all the h/w file marks
 *	2) c_nvolumes field unused.
 */

struct cap
{
    long c_hw_filemark;
    long c_sw_filemark;
    long c_full_datablock;
    long c_partial_datablock;
    long c_vol_header;
    long c_label_block;
    unsigned long c_data_bytes;
    long c_null_volumes;
    long c_nvolumes;
};

struct cap Tstats;

/* ---- end data capacity evaluation */


extern struct tapeConfig globalTapeConfig;
extern struct deviceSyncNode *deviceLatch;

/* PrintTapeLabel
 *	print out the tape label.
 */

PrintTapeLabel(labelptr)
     struct butm_tapeLabel *labelptr;
{
    dce_svc_printf(BAK_S_TAPE_LABEL_HEADER_MSG);
    printf("----------\n");
    if (labelptr->name && (strcmp(labelptr->name, "") != 0))
      printf("name = \t\t%s\n", labelptr->name);
    else
      printf("name = \t\t<NULL>\n");
    printf("createTime = \t%s", ctime((time_t *)&labelptr->creationTime));
    if ( labelptr->expirationDate )
	printf("expireTime = \t%s", ctime((time_t *)&labelptr->expirationDate));
    printf("cell = \t\t%s\n", labelptr->cell);
    printf("size = \t\t%u Kbytes\n", labelptr->size);
    printf("dump path = \t%s\n", labelptr->dumpPath);

    if ( labelptr->structVersion >= TAPE_VERSION_1 )
    {
	printf("dump id = \t%u\n", labelptr->dumpid);
	printf("useCount = \t%d\n", labelptr->useCount);
    }
    dce_svc_printf(BAK_S_END_OF_TAPE_LABEL_MSG);
}

/* PrintVolumeHeader
 *	print the contents of a volume header. 
 */
static
PrintVolumeHeader(volHeader)
     struct volumeHeader *volHeader;
{
    afs_hyper_t vid;
    dce_svc_printf(BAK_S_FILESET_HEADER_MSG);
    printf("fileset name: %s\n", volHeader->volumeName);
    DFSH_MemFromNetHyper(vid, volHeader->volumeID);
    printf("fileset ID %lu,,%lu\n", AFS_HGETBOTH(vid));
    /* printf("server %d\n", volHeader->server); */
    printf("dumpSetName: %s\n", volHeader->dumpSetName);
    printf("dumpID %d\n", volHeader->dumpID);
    printf("level %d\n", volHeader->level);
    printf("parentID %d\n", volHeader->parentID);
    printf("endTime %d\n", volHeader->endTime);
    /* printf("versionflags %d\n", volHeader->versionflags); */
    printf("clonedate %s\n", ctime(&volHeader->cloneDate));
}

/* printScanInfo
 *	print out all the information gathered about this dump
 */

static
printScanInfo(tsiptr)
     struct tapeScanInfo *tsiptr;
{
    struct tapeScanHeader *tshptr;
    struct tapeScanVolHeader *tsvhptr;

    if ( tsiptr == 0 )
    	return(0);

    tshptr = tsiptr->headers;
    while ( tshptr != 0 )
    {
	debugPrintTapeLabel(tshptr->label);

	/* print all the volumes on this tape */
	tsvhptr = tshptr->volumeHead;
	while ( tsvhptr != 0 )
	{
	    debugPrintVolumeHeader(tsvhptr->header);
	    tsvhptr = tsvhptr->next;
	}

	tshptr = tshptr->next;
    }
}

/* freeScan
 *	free all the information about a single tape. Frees the volume
 *	information chain, and finally frees the tape label and the 
 *	parent structure
 */

static
freeScan(tshptr)
     struct tapeScanHeader *tshptr;
{
    struct tapeScanVolHeader *tsvhptr, *temp;

    if ( tshptr == 0 )
    	return(0);

    /* free all the volume information */
    tsvhptr = tshptr->volumeHead;
    while ( tsvhptr != 0 )
    {
	(void)osi_Free(tsvhptr->header, 
		 sizeof(struct volumeHeader));	/* free volume header */
	temp = tsvhptr;
	tsvhptr = tsvhptr->next;
	(void)osi_Free(temp, sizeof(struct tapeScanVolHeader));
    }

    (void)osi_Free(tshptr->label, sizeof(struct butm_tapeLabel));
    (void)osi_Free(tshptr, sizeof(struct tapeScanHeader));
    return(0);
}

/* freeScanInfo
 *	frees all the information gathered about a dump. The tapeScanInfo
 *	structure itself is left to the caller to handle
 */

static
freeScanInfo(tsiptr)
     struct tapeScanInfo *tsiptr;
{
    struct tapeScanHeader *tshptr, *temp;

    if ( tsiptr == 0 )
    	return(0);

    /* free information on all the tapes */
    tshptr = tsiptr->headers;
    while ( tshptr != 0 )
    {
	temp = tshptr;
	tshptr = tshptr->next;
    	freeScan(temp);
    }

    tsiptr->headers = 0;
    return(0);
}


/* Ask
 *	ask a question. returns true or false
 * exit:
 *	1 - yes
 *	0 - no
 */

long
Ask(st)
     char *st;
{
    char response;

    while (1)
    {
	FFlushInput(stdin);
	putchar(BELLCHAR);
	printf("%s? (y/n) ", st);
	fflush(stdout);
	response = getchar();
	if ( response == 'y' )
		return(1);
	else
	if ( response == 'n' )
    		return(0);
	dce_svc_printf(BAK_S_PLEASE_ANSWER_MSG);
    }
}

/* ScanDumps
 *	This set of code fragments read a tape, and add the information to
 * 	the database. Builds a literal structure.
 *	
 */

ScanDumps(ptr)
     struct scanTapeIf *ptr;
{
    struct butm_tapeInfo *curTapeInfo;
    struct tapeScanInfo tapeScanInfo;
    long buildDataBaseFlag;
    u_long taskId;
    long tapeNumber, retval;
    long moreTapes = 0;
    long code;

    bzero((char *)&tapeScanInfo, sizeof(tapeScanInfo));
    bzero((char *)&Tstats, sizeof(Tstats));

    curTapeInfo = (struct butm_tapeInfo *)osi_Alloc(sizeof(struct butm_tapeInfo));
    if (curTapeInfo == (struct butm_tapeInfo *)NULL)
      return (TC_NOMEMORY);
    bzero((char *)curTapeInfo, sizeof(struct butm_tapeInfo));
    curTapeInfo->structVersion = CUR_BUTM_VERSION;

    buildDataBaseFlag = ptr->addDbFlag;
    taskId = ptr->taskId;

    EnterDeviceQueue(deviceLatch);
    clearStatus(taskId, DRIVE_WAIT);

    code = butm_file_Instantiate (curTapeInfo, &globalTapeConfig);
    if ( code )
    {
	LogError("ScanTape: can't initialize tape module\n");
	goto abort;
    }

    tapeNumber = 0;
    while (1)
    { /*w*/
	tapeNumber++;
	code = readTape(curTapeInfo, tapeNumber, &tapeScanInfo, &moreTapes,
			taskId);

	if ( checkAbortByTaskId(taskId) )
	    goto exit;

	if ( code )
	{
	    /* error in reading the tape. For the moment, abort */
#ifdef TOLERATE_ERRORS
	    dce_svc_printf(BAK_S_ERRORS_IN_DUMP_MSG);
	    code = 0;
#else /* not TOLERATE_ERRORS */
            dce_svc_printf(BAK_S_THIS_DUMP_NO_PROCESS_MSG);
	    dce_svc_printf(BAK_S_TAPE_NOT_VALID_MSG);
	    goto exit;
#endif /* TOLERATE_ERRORS */
	}

	if ( moreTapes == 1 )
	{
	    /* no need to ask - we know there are more tapes */
	    continue;
	}

	retval = (queryoperator ? Ask("Are there more tapes"):1);

	if ( retval == 0 )
            break;
	/* otherwise continues reading tapes */
    } /*w*/

    /* done reading tapes */

    /* dataSummary(); */
    /* printScanInfo(&tapeScanInfo); */

    /* maybe list the information */
    if ( buildDataBaseFlag != 0 )
    {
	/* fprintf(stderr, "database add request\n"); */
	/* add this information to the database */
       dce_svc_printf(BAK_S_DONE_SCANNING_NOW_ADDING_MSG);
       code = AddScanToDB(&tapeScanInfo);
       if (code) { /* failed to add it to database */
	 dce_svc_printf(BAK_S_FAILED_WITH_STRING_MSG, dfs_dceErrTxt(code));
	 goto exit;
       }
       dce_svc_printf(BAK_S_DONE_MSG);
       fflush(stdout);
    }
    /* print out confirmation ? */
    /* dce_svc_printf(BAK_S_DONE_WITH_SCANDUMPS_MSG); */
    
exit:
    /* normal exit */
    code = 0;
    freeScanInfo(&tapeScanInfo);	    /* destroy scan information */

    if ( checkAbortByTaskId(taskId) )
	setStatus(taskId, ABORT_DONE);
    setStatus(taskId, TASK_DONE);

abort:
    /* error exit */
    (void)osi_Free(ptr, sizeof(struct scanTapeIf));
    LeaveDeviceQueue(deviceLatch);
    return(code);
}

/* validVolumeHeader
 * exit:
 *	0 - no
 *	1 - yes
 */

static long
validVolumeHeader(vhptr)
     struct volumeHeader *vhptr;
{
    long versionflags;
    struct sockaddr_in sa;
    afs_hyper_t vid;

    if ( strncmp(vhptr->postamble, "T--NAME#", 8) == 0 )
    {
	/* do further sanity checks */
	versionflags = ntohl(vhptr->versionflags);
	if ( (versionflags ==  TAPE_VERSION_1) )
	{
	    return(1);					/* complete success */
	}
	else
	{
	    /* unexpected anomaly, fail */
	  dce_svc_printf(BAK_S_POSTAMBLE_MATCHED_FAILED_MSG);
	  dce_svc_printf(BAK_S_FILESET_HEADER_HEADER_MSG);
	  printf("-------------\n");
	  dce_svc_printf(BAK_S_PREAMBLE_POSTAMBLE_MSG, vhptr->preamble,
		 vhptr->postamble);
	  printf("fileset name: %s\n", vhptr->volumeName);
	  DFSH_MemFromNetHyper(vid, vhptr->volumeID);
	  printf("fileset ID %lu,,%lu\n", AFS_HGETBOTH(vid));
	  sa = *((struct sockaddr_in *) &vhptr->server);
	  printf("server %d\n", ntohl(sa.sin_addr.s_addr));
	  printf("part %d\n", ntohl(vhptr->part));
	  printf("from %d\n", ntohl(vhptr->from));
	  printf("frag %d\n", ntohl(vhptr->frag));
	  printf("magic %d\n", ntohl(vhptr->magic));
	  printf("contd %d\n", ntohl(vhptr->contd));
	  printf("dumpSetName: %s\n", vhptr->dumpSetName);
	  printf("dumpID %d\n", ntohl(vhptr->dumpID));
	  printf("level %d\n", ntohl(vhptr->level));
	  printf("parentID %d\n", ntohl(vhptr->parentID));
	  printf("endTime %d\n", ntohl(vhptr->endTime));
	  printf("versionflags %d\n", ntohl(vhptr->versionflags));
	  printf("clonedate (%d)\n", ntohl(vhptr->cloneDate));
	  return(0);
	}
    }
    return(0);						/* failed */
}

/* VolHeaderToHost
 *	convert the fields in the tapeVolHeader into host byte order,
 *      placing the converted copy of the structure into the hostVolHeader.
 *      The exception to this is that the volumeID is left in net order.
 * entry:
 *      tapeVolHeader - points to volume header read from tape
 *      hostVolHeader - pointer to struct for result
 * exit:
 *      hostVolHeader - information in host byte order
 */

VolHeaderToHost(hostVolHeader, tapeVolHeader)
     struct volumeHeader *hostVolHeader, *tapeVolHeader;
{
    /* is this a flag or a value ?? */
    switch ( ntohl(tapeVolHeader->versionflags) )
    {
      case TAPE_VERSION_1:
	/* sizes in K and fields in network order */
	/* do the conversion field by field */

	strcpy(hostVolHeader->preamble, tapeVolHeader->preamble);
	strcpy(hostVolHeader->postamble, tapeVolHeader->postamble);
	strcpy(hostVolHeader->volumeName, tapeVolHeader->volumeName);
	/* leave net hyper alone */
	hostVolHeader->volumeID = tapeVolHeader->volumeID;
	hostVolHeader->server = tapeVolHeader->server; /* struct. assignment */
	hostVolHeader->server.type = ntohs(hostVolHeader->server.type);
	hostVolHeader->part = ntohl(tapeVolHeader->part);
	hostVolHeader->from = ntohl(tapeVolHeader->from);
	hostVolHeader->frag = ntohl(tapeVolHeader->frag);
	hostVolHeader->magic = ntohl(tapeVolHeader->magic);
	hostVolHeader->contd = ntohl(tapeVolHeader->contd);
	strcpy(hostVolHeader->dumpSetName, tapeVolHeader->dumpSetName);
	hostVolHeader->dumpID = ntohl(tapeVolHeader->dumpID);
	hostVolHeader->level = ntohl(tapeVolHeader->level);
	hostVolHeader->parentID = ntohl(tapeVolHeader->parentID);
	hostVolHeader->endTime = ntohl(tapeVolHeader->endTime);
	hostVolHeader->versionflags = ntohl(tapeVolHeader->versionflags);
	hostVolHeader->cloneDate = ntohl(tapeVolHeader->cloneDate);
	break;

      default:
	printf("VolHeaderToHost: unknown verson (%d)\n",  
	       ntohl(tapeVolHeader->versionflags));
	return(-1);
	
    }
    return(0);
}

/* scanVolData
 *	Skips the volume data on the tape. The end of the volume data is
 *	detected by the presence of the volume trailer or by an EOF indication
 *	from butm.
 * entry:
 *	curTapePtr - tape info structure
 *	Tape must be positioned after volume header
 * exit:
 *	0 - success
 *	1 - internal failure - usually osi_Alloc
 *	2 - scan failure - 
 *	3 - empty volume, requires special handling
 *
 *	Tape positioned after at the beginning of the next tape file.
 *	In case of errors, positioned after the error causing block
 */

static
scanVolData(curTapePtr, volumeTrailer)
     struct butm_tapeInfo *curTapePtr;
     struct volumeHeader *volumeTrailer;
{
    struct volumeHeader tempTapeVolumeTrailer, vlTrailer;
    char *allocBuf = NULL;
    char *prevBuf1;
    char *prevBuf2;
    char *dataBuf;
    char *tmpBuf;
    long prevBytes1;
    long prevBytes2;
    long dataBytes;
    long bytesRead;
    long code;
    long vhSize = sizeof(struct volumeHeader);

    allocBuf = (char *) osi_Alloc(3*BUTM_BLOCKSIZE);
    if ( !allocBuf ) ERROR_EXIT(1);
    prevBuf1 = allocBuf;
    prevBuf2 = prevBuf1+BUTM_BLOCKSIZE;
    dataBuf = prevBuf2+BUTM_BLOCKSIZE;

    prevBytes1 = 0;
    prevBytes2 = 0;
    dataBytes = 0;

    /*
     * scan the tape until we reach the FILE_END mark. We need
     * the last three buffers to locate the volume trailer. The
     * current buffer will contain the FILE_END mark, prevBuf1
     * will contain the last datablock from the file and prevBuf2
     * will contain the second to last data block (if any). We need
     * the last two data blocks to handle cases where the volume
     * trailer spans block boundaries
     */
    while ( 1 ) { /*w*/
      pthread_yield();
      code = butm_ReadFileData(curTapePtr, dataBuf, BUTM_DATASIZE, &dataBytes);
      if (code) {
	if (code == BUTM_ENDVOLUME) {
	  /* ReadFileData skips the EOF mark */
	  Tstats.c_sw_filemark++;
	  break;
	}
	else 
	  ERROR_EXIT(code);
      }
      if (dataBytes < 0 || dataBytes > BUTM_DATASIZE)
	ERROR_EXIT(TC_SCANFAILURE);

      Tstats.c_data_bytes += dataBytes;
      bytesRead += dataBytes;
      dfprintf((stderr, "dataBytes %d code %d at 0x%x\n",
	       dataBytes, code, &buffer[readOffset])); 
      if (dataBytes == BUTM_DATASIZE)
          Tstats.c_full_datablock++;

      /*
       * shift the buffers before reading the next block
       */

      tmpBuf = prevBuf2;
      prevBuf2 = prevBuf1;
      prevBytes2 = prevBytes1;
      prevBuf1 = dataBuf;
      prevBytes1 = dataBytes;
      dataBuf = tmpBuf;
      dataBytes = 0;
    }

    /*
     * prevBuf1 and prevBuf2 now contain the last two datablocks
     * from the dump, and the FILE_END block is in dataBuf.
     */
    if (!ExtractTrailer(prevBuf2 + BUTM_HDRSIZE, prevBytes2, NULL,
			prevBuf1 + BUTM_HDRSIZE, prevBytes1, NULL,
			&vlTrailer))
      code = TC_MISSINGTRAILER;
    else if (!validVolumeHeader(&vlTrailer)) {
      fprintf(stderr, 
	   "scanVolData: Unable to locate valid trailer at end of fileset\n");
      code = TC_SCANFAILURE;
    }
    else {
      /* subtract size of the volume trailer from data read */
      bytesRead -= vhSize;
      if (volumeTrailer) {
	code = VolHeaderToHost(volumeTrailer, &vlTrailer);
	if (code)
	  ERROR_EXIT(TC_SCANFAILURE);
      }
    }

error_exit:
    if (code == BUTM_STATUS_EOF)
      code = 3;
    if (allocBuf)
      osi_Free(allocBuf, 3*BUTM_BLOCKSIZE);
    return (code);
}

/* checkInsertedTape
 *	check the tape to see if it follows in logical sequence from the
 *	last tape read. Issues error messages to user if tape is wrong.
 * exit: 
 *	0 - ok
 * 	1 - label does not follow sequence
 *	2 - label format error
 *	3 - other error
 */

static
checkInsertedTape(labelptr, infoptr)
     struct butm_tapeLabel *labelptr;
     struct tapeScanInfo *infoptr;
{
    struct tapeScanHeader *tsptr;
    int curseq, prevseq, curlength, prevlength;
    char *curdot, *prevdot;

    /* check for null labels first */
    if ( strcmp(labelptr->name, "") == 0 )
    	return(0);

    /* decode the label on the inserted tape. First we locate the sequence
     * number
     */
    curdot = strrchr(labelptr->name, '.');
    if ( curdot == 0 )
    {
	dce_svc_printf(BAK_S_MALFORMED_TAPE_LABEL_MSG, labelptr->name);
    	return(2);
    }

    curseq = atoi(curdot+1);

    if ( (infoptr == 0)
    ||   (infoptr->headers == 0)
       )
    {
	/* tape is acceptable as long as the sequence number is 1 */
	if ( curseq == 1 )
		return(0);
	else
	{
	    dce_svc_printf(BAK_S__TAPE_IS_NOT_FIRST_MSG, labelptr->name);
	    return(1);
	}
    }

    /* find the scan header for the last tape read. Already guaranteed
     * that we have at least one tape scan header
     */
    tsptr = infoptr->headers;
    while ( tsptr->next )
    	tsptr = tsptr->next;
    
    /* extract information from previous tape label */
    prevdot = strrchr(tsptr->label->name, '.');
    if ( prevdot == 0 )
    	return(3);

    /* compare textual label lengths and contents */
    curlength =  curdot - &labelptr->name[0];
    prevlength = prevdot - &tsptr->label->name[0];
    if ( curlength != prevlength )
	goto nomatch;

    /* compares, including trailing . */
    if ( strncmp(labelptr->name, tsptr->label->name, curlength) != 0 )
    	goto nomatch;
    
    prevdot++;
    prevseq = atoi(prevdot);

    if ( curseq != prevseq + 1 )
    	goto nomatch;

    /* just for good measure, check a few other things
     * Should we check expiration times? 
     */
    if ( (labelptr->cell != 0)
    ||   (tsptr->label->cell != 0)
       )
    {
	if ( ((labelptr->cell == 0) || (tsptr->label->cell == 0))
	||   ( strcmp(labelptr->cell,tsptr->label->cell) != 0 )
	   )
	{
	    dce_svc_printf(BAK_S_LABEL_MATCH_INCONISTANCY_ELSE_MSG);
	    goto nomatch;
	}
    }

    return(0);

nomatch:
    dce_svc_printf(BAK_S_ERROR_WRONG_TAPE_MSG);
    dce_svc_printf(BAK_S_LABEL_DOES_NOT_FOLLOW_MSG, labelptr->name, tsptr->label->name);
    dce_svc_printf(BAK_S_PLEASE_CORRECT_TAPE_MSG);
    return(1);
}

/* nextTapeLabel
 *	generate the name of the next tape label expected
 * exit: 
 *	ptr to static string
 */

char *
nextTapeLabel(infoptr, tapeNumber)
     struct tapeScanInfo *infoptr;
     long tapeNumber;
{
    struct tapeScanHeader *tsptr;
    int prevseq, prevlength;
    static char buffer[BU_MAXTAPELEN];
    char *prevdot;
    char *retval;

    retval = "";
    if ( (tapeNumber == 1)		/* trivial case */
    ||   (infoptr == 0 )		/* just in case of errors */
       )
    {
	return(retval);
    }
    	
    /* find the scan header for the last tape read. Already guaranteed
     * that we have at least one tape scan header
     */
    tsptr = infoptr->headers;
    while ( tsptr->next )
    	tsptr = tsptr->next;

    /* extract information from previous tape label */
    strcpy(buffer, tsptr->label->name);
    prevdot = strrchr(buffer, '.');
    if ( prevdot == 0 )
    	return(retval);

    prevdot++;
    prevseq = atoi(prevdot);

    if ( tapeNumber != prevseq + 1 )	/* consistency failure */
    	return(retval);

    sprintf(prevdot, "%-d", tapeNumber);
    return(&buffer[0]);
}

/* readTape
 *	read all the information on a tape, and add it to the in-core
 *	structures
 * entry:
 *	curTapePtr - tape info structure
 *	tapeNumber - starting at 1. Used for consistency checks.
 *	infoPtr - in-core tape information. Current tape's contents are
 *		added to this.
 * exit:
 *	-1 - error - tape information not retained
 *	0 - tape added to scan information
 *	moreTapes set to 1 if this is not the last tape in the dump, else 0
 */

#define MAXTRIES 6 

static int
readTape(curTapePtr, tapeNumber, infoPtr, moreTapes, taskId)
     struct butm_tapeInfo *curTapePtr;
     long tapeNumber;
     struct tapeScanInfo *infoPtr;
     long *moreTapes;
     u_long taskId;
{
    struct butm_tapeLabel *tapeLabel;
    struct volumeHeader volHeader, hostVolHeader, *vhptr;
    struct volumeHeader volumeTrailer;
    struct tapeScanHeader *tshptr = 0;			/* incore start */
    struct tapeScanHeader *temptshptr;
    struct tapeScanVolHeader *tsvhptr, *prevtsvhptr;
    long nbytes;
    long code;
    int numtries = 0;
    long savePosition;

    *moreTapes = 0;
    tsvhptr = 0;
    prevtsvhptr = 0;

    tapeLabel = (struct butm_tapeLabel *)osi_Alloc(sizeof(struct butm_tapeLabel));
    if (tapeLabel == (struct butm_tapeLabel *)NULL)
      return TC_NOMEMORY;

    while (numtries < MAXTRIES) {

    /* prompt for a tape - generate the most reasonable prompt possible */
    if (tapeNumber == 1 && autoQuery == 1)
    	PromptForTape(SCANOPCODE, "", 0, taskId, numtries); 
    else 
      if (tapeNumber != 1)     {
    	PromptForTape(SCANOPCODE,
		      (char *) nextTapeLabel(infoPtr, tapeNumber),
		      0, taskId, numtries); 
    }

    if ( checkAbortByTaskId(taskId) )
	goto error;
    
    code = butm_Mount(curTapePtr, "");		/* open the tape device */
    if ( code == BUTM_NOTAPE )
    {
	/* tape not loaded or device offline */
	dce_svc_printf(BAK_S_NO_TAPE_MSG,globalTapeConfig.device);
	continue;
    }

    /* read the label on the tape */
    code = butm_ReadLabel(curTapePtr, tapeLabel);
    if ( code )
    	goto error;

    Tstats.c_label_block++;
    Tstats.c_hw_filemark++;

    if ( tapeNumber == 1 )
    	PrintTapeLabel(tapeLabel);



    /* if there is an existing header, check the tape label against it */
    code = checkInsertedTape(tapeLabel, infoPtr);

    if (!code) /* SUCCESS */
      break;
    
    /* Failure. Incremement numtries, and try again */
      numtries++;
      /* We must also do a dismount. */
      butm_Dismount(curTapePtr);
    }


    if (numtries == MAXTRIES) /* Failed, abort */
      goto error;

    /* add the label to the scan information */
    tshptr = (struct tapeScanHeader *) osi_Alloc(sizeof(struct tapeScanHeader));
    if ( tshptr == 0 )
    {
	code = -1;
	goto error;
    }
    bzero((char *)tshptr, sizeof(*tshptr));
    tshptr->label = (struct butm_tapeLabel *)
    			osi_Alloc(sizeof(struct butm_tapeLabel));
    if (tshptr->label == 0)
    {
	code = -1;
	goto error;
    }
    *tshptr->label = *tapeLabel;

    /* read all volumes */
    while(1)
    { /*w*/
	/* save the current file position */
	savePosition = curTapePtr->position;

	/* fprintf(stderr, "starting new fileset\n"); */
	/* read the start of the next file */
	code = butm_ReadFileBegin(curTapePtr);
	Tstats.c_sw_filemark++;

	/* fprintf(stderr, " butm_ReadFileBegin  returns %d\n", code); */

	/* Tapes made with 3.0 have no software EOT markers. Therefore
	 * at this point, we will most likely get a read error, indicating
	 * the end of this dump
	 */

	if ( code == BUTM_EOT )
	{
	    code = 0;
	    break;
	}
	else
	if ( code )
	{
	    /* check version number in the tape label */
	    if ( (tapeLabel->structVersion == TAPE_VERSION_1) )
	    {
		/* then a tape error is possible at this point, and it
		 * signals the end of the dump. Tapes that are continued
		 * have an EOT marker.
		 */
		dce_svc_printf(BAK_S_READ_ERROR_END_OF_TAPE_MSG);
		code = 0;
		break;
	    }
	    com_err("", code, "");
	    code = -1;
#ifdef TOLERATE_ERRORS
	    break;
#else /* not TOLERATE_ERRORS */
	    goto error;
#endif
	}

	/* now read the volume header */
	bzero((char *)&volHeader,sizeof(volHeader));
        code = butm_ReadFileDataCopy(curTapePtr,&volHeader,sizeof(volHeader),
				     &nbytes);
	Tstats.c_vol_header++;

	/* printf(" butm_ReadFileDataCopy returns %d\n", code); */
	if ( code )
	{
	    com_err("", code, "");
#ifdef TOLERATE_ERRORS
	    break;
#else /* not TOLERATE_ERRORS */
	    goto error;
#endif
	}

	code = VolHeaderToHost(&hostVolHeader, &volHeader); 
	/* printf(" VolHeaderToHost  returns %d\n", code); */
	if ( code )
	{
	    com_err("", code, "");
	    goto error;
	}
	PrintVolumeHeader(&hostVolHeader);

	/* add the volume header to the scan information. */

	tsvhptr = (struct tapeScanVolHeader *) 
			osi_Alloc(sizeof(struct tapeScanVolHeader));
	if ( tsvhptr == 0 )
	{
	    code = -1;
	    goto error;
	}
	bzero((char *)tsvhptr, sizeof(*tsvhptr));

	vhptr = (struct volumeHeader *)osi_Alloc(sizeof(struct volumeHeader));
	if ( vhptr == 0 )
	{
	    (void)osi_Free(tsvhptr, sizeof(struct tapeScanVolHeader));
	    code = -1;
	    goto error;
	}
	bzero((char *)vhptr, sizeof(*vhptr));

	/* save the position of this volume */
	tsvhptr->tapePosition = savePosition;

	/* now continue reading the tape. Skip all the volume data */
	code = scanVolData(curTapePtr, &volumeTrailer);

	if ( code == 0 )
	{ /*c0*/
	    /* normal termination, add the volume to our list */

	    tsvhptr->header = vhptr;
	    *vhptr = hostVolHeader;	/* structure assignment */
	    
	    /* now link this volume's header information to the chain */
	    if ( prevtsvhptr == 0 )
	    {
		/* first volume to be read */
		tshptr->volumeHead = tsvhptr;
		prevtsvhptr = tsvhptr;
	    }
	    else
	    {
		/* just link it on the end */
		prevtsvhptr->next = tsvhptr;
		prevtsvhptr = tsvhptr;
	    }
	    /* save information out of the volume trailer */
	    tsvhptr->lastFragment = (volumeTrailer.contd == 0) ? 1 : 0;
	    *moreTapes = volumeTrailer.contd;
	    
	    /* printf(" butm_ReadFileEnd  returns %d\n", code); */
	    if ( code == BUTM_EOT )
	    	break;
	    else
	    if ( code != 0 )
	    {
		dce_svc_printf(BAK_S_ERROR_FILE_END_MARKER_MSG);
		/* com_err("", code, ""); */
#ifdef TOLERATE_ERRORS
	    	break;
#else /* not TOLERATE_ERRORS */
	    	goto error;
#endif
	    }
	    
	} /*c0*/
	else
	if ( code == 3 )
	{
	    /* null volume, report error but continue the scan */
	    Tstats.c_null_volumes++;
	    dce_svc_printf(BAK_S_FILESET_IS_EMPTY_MSG, hostVolHeader.volumeName);

	    /* clean up the volume structures */
	    if ( vhptr )
	    	(void)osi_Free(vhptr, sizeof(struct volumeHeader));
	    if ( tsvhptr )
	    	(void)osi_Free(tsvhptr, sizeof(struct tapeScanVolHeader));
	    continue;
	}
	else
	{
	    /* default case - abort */
	    dce_svc_printf(BAK_S_SKIPPING_FILESET_DATA_MSG, code);
#ifdef TOLERATE_ERRORS
	    break;
#else /* not TOLERATE_ERRORS */
	   goto error;
#endif
	}

	/* debugPrintVolumeHeader(&volumeTrailer); */
	/* dce_svc_printf(BAK_S_END_OF_FILESET_MSG); */
	pthread_yield();
    } /*w*/

    code = butm_Dismount(curTapePtr);
    if (code)
	goto error;

    /* successfully read this tape. Add the information gathered to the
     * end of the dump information
     */
    if ( infoPtr->headers == 0 )
    {
	/* first tape */
	infoPtr->headers = tshptr;
    }
    else
    {
	/* append to the tape information */
	temptshptr = infoPtr->headers;
	while ( temptshptr->next != 0 )
		temptshptr = temptshptr->next;

	temptshptr->next = tshptr;
    }
    osi_Free(tapeLabel, sizeof(struct butm_tapeLabel));
    return(0);

error:
    butm_Dismount(curTapePtr);
    osi_Free(tapeLabel, sizeof(struct butm_tapeLabel));
#ifndef TOLERATE_ERRORS
    freeScan(tshptr);				/* free all tape info */
#endif /* not TOLERATE_ERRORS */
    return(code);
}


/* validatePath
 * exit:
 *	0 - not ok
 *	1 - ok
 */
validatePath(tsiptr, pathptr)
     struct tapeScanInfo *tsiptr;     
     char *pathptr;
{
    char *up, *tp;
    char tapeName[BU_MAXTAPELEN];

    /* check length */
    if ( strlen(pathptr) > (unsigned)BU_MAX_DUMP_PATH-1 )
    {
	dce_svc_printf(BAK_S_PATHNAME_TOO_LONG_MSG);
	return(0);
    }

    if ( (tsiptr == 0)
    ||   (tsiptr->headers == 0)
    ||   (tsiptr->headers->label == 0)
       )
    {
	return(1);
    }
    strcpy(tapeName, tsiptr->headers->label->name);

    tp = strrchr(tapeName, '.');
    if ( tp == 0 )
    	return(1);
    *tp = 0;
    tp = strrchr(tapeName, '.');
    if ( tp == 0 )
    	return(1);
    tp++;

    up = strrchr(pathptr, '/');
    if ( up == 0 )
    {
	dce_svc_printf(BAK_S_PATHNAME_MISSING_SLASH_MSG);
    	return(0);
    }
    up++;

    if ( strcmp(up, tp) != 0 )
    {
	dce_svc_printf(BAK_S_INVALID_PATH_NAME_MSG);
	dce_svc_printf(BAK_S_DUMP_NAMES_MISMATCH_MSG, tp, up);
	return(0);
    }
    return(1);
}

/* AddScanToDB
 *	Add the dump to the dump database. Ubik version.
 * exit:
 *	0 - success
 * notes:
 *	
 */

static int
AddScanToDB(tsiptr)
     struct tapeScanInfo *tsiptr;
{
    struct budb_dumpEntry dumpEntry;
    struct budb_tapeEntry tapeEntry;
    struct budb_volumeEntry *volumeArray;

    struct tapeScanHeader *tshptr;
    struct butm_tapeLabel *labelptr;
    struct tapeScanVolHeader *tsvhptr;
    struct volumeHeader *vhptr;
    long dumpID, parentDumpID;
    long volCount,volIndex;
    char path[1024];
    long new;
    long code;

    char *volumesetNamePtr();

    if ( (tsiptr == 0)
    ||   (tsiptr->headers == 0)
       )
      return(TC_INTERNALERROR);

    /* check for existence of required information */
    if ( (tsiptr->headers->volumeHead == 0)
    ||   (tsiptr->headers->volumeHead->header == 0 )
       )
	return(TC_NOVOLUMEONTAPE);

    dumpID = tsiptr->headers->volumeHead->header->dumpID;

    if ( bcdb_FindDumpByID(dumpID, &dumpEntry) == 0 ) 
      return(BUDB_DUMPIDEXISTS);

    if ( strlen(tsiptr->headers->label->dumpPath) == 0 )
    { /*i*/
	/* keep trying until we get a reasonable path */
	while ( 1 )
	{ /*w*/
	    dce_svc_printf(BAK_S_NEED_DUMP_SCHED_PATH_MSG, tsiptr->headers->label->name);
	    fflush(stdout);
	    code = fscanf(stdin, "%s", path);
	    if ( code != 1 )
	    {
		dce_svc_printf(BAK_S_UNRECOGNIZE_PATH_NAME_MSG);
		continue;
	    }

	    /* trailing component name must match tape label */
	    if ( validatePath(tsiptr, path) )
	    	break;
	} /*w*/
    } 
    else
    	strcpy(path, tsiptr->headers->label->dumpPath);

    /* set ptrs for the scan traversal */
    tshptr = tsiptr->headers;
    labelptr = tshptr->label;			/* get 1st tape label */

    /* get 1st volume info, for dump header construction */
    tsvhptr = tshptr->volumeHead;
    vhptr = tsvhptr->header;

    /* construct dump entry */
    bzero((char *)&dumpEntry, sizeof(dumpEntry));

    dumpEntry.id = dumpID;
    dumpEntry.parent = vhptr->parentID;
    dumpEntry.level = vhptr->level;
    dumpEntry.flags = 0;

    if ( volumesetNamePtr(vhptr->dumpSetName) == 0 )
	return(TC_INTERNALERROR);

    strcpy((char *)dumpEntry.volumeSetName, 
	   (char *)volumesetNamePtr(vhptr->dumpSetName));
    strcpy((char *)dumpEntry.dumpPath, (char *)path);
    strcpy((char *)dumpEntry.name, (char *)vhptr->dumpSetName);

    dumpEntry.created = labelptr->creationTime;
    dumpEntry.incTime = 0;
    dumpEntry.nVolumes = 0;

    default_tapeset(&dumpEntry.tapes, vhptr->dumpSetName);
    code = bcdb_CreateDump(&dumpEntry);
    if ( code )
	return(code);
    parentDumpID = vhptr->parentID;		/* save for later checks */

    /*
     * allocate an array for the tape volumes
     */

    volumeArray = (struct budb_volumeEntry *)
		  osi_Alloc(BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
    if ( !volumeArray )
	return(TC_NOMEMORY);

    /* now write information for each volume */
    /* for every tape */
    while ( tshptr != 0 )
    { /*wt*/
	bzero((char *)&tapeEntry, sizeof(tapeEntry));
	
	strcpy((char *)&tapeEntry.name[0], (char *)labelptr->name);
	/* tapeEntry.flags = BUDB_TAPE_BEINGWRITTEN; */
	tapeEntry.written = labelptr->creationTime;
	tapeEntry.expires = labelptr->expirationDate;
	tapeEntry.seq = extractTapeSeq(labelptr->name);
	if ( tapeEntry.seq < 0 )
	{
	    osi_Free(volumeArray,
		     BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
	     return(TC_INTERNALERROR);
	}

	tapeEntry.dump = dumpID;

	code = bcdb_UseTape(&tapeEntry, &new);
	if ( code )
	{
	    osi_Free(volumeArray,
		     BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
	    return(code);
	}

	tsvhptr = tshptr->volumeHead;

	/* for every volume on that tape */
	volCount = 0;
	while ( tsvhptr != 0 )
	{ /*wv*/
	    vhptr = tsvhptr->header;

	    bzero((char *)&volumeArray[volCount],sizeof(volumeArray[volCount]));

	    volumeArray[volCount].dump = dumpID;
	    strcpy((char *)&volumeArray[volCount].tape[0],
		   (char *)labelptr->name);

	    strcpy((char *)&volumeArray[volCount].name[0],
		   (char *)vhptr->volumeName);
	    if ( vhptr->frag == 1 )
		volumeArray[volCount].flags = BUDB_VOL_FIRSTFRAG;
	    if ( tsvhptr->lastFragment )
		volumeArray[volCount].flags |= BUDB_VOL_LASTFRAG;

	    DFSH_MemFromNetHyper(volumeArray[volCount].id, vhptr->volumeID);
	    volumeArray[volCount].position = tsvhptr->tapePosition;
	    volumeArray[volCount].clone = vhptr->cloneDate;
	    volumeArray[volCount].seq = vhptr->frag-1;
	    volCount++;
	    if ( volCount == BU_MAXVOLCOUNT )
	    {
	        code = bcdb_AddVolumes(volCount,volumeArray);
	        if ( code )
		{
		    osi_Free(volumeArray,
			     BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
		    return(code);
	        }
		volCount = 0;
	    }
	    
	    tsvhptr = tsvhptr->next;
	} /*wv*/

	/*
	 * add any remaining volumes to the database
	 */

	if ( volCount > 0 )
	{
	    code = bcdb_AddVolumes(volCount,volumeArray);
	    if ( code )
	    {
	        osi_Free(volumeArray,
		         BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
		return(code);
	    }
	}

	tshptr = tshptr->next;

	tapeEntry.flags = BUDB_TAPE_WRITTEN;
	code = bcdb_FinishTape(&tapeEntry);
	if ( code )
	{
	    osi_Free(volumeArray,
		     BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));
	    return(code);
	}

    } /*wt*/

    /*
     * free the volume array
     */

    osi_Free(volumeArray,BU_MAXVOLCOUNT*sizeof(struct budb_volumeEntry));

    /* check for existence of parent */
    if ( (parentDumpID != 0)
    &&   (bcdb_FindDumpByID(parentDumpID, &dumpEntry) != 0 )
       )
    {
	/* warning if no parent dump */
	dce_svc_printf(BAK_S_PARENT_MISSING_MSG, parentDumpID);
	dce_svc_printf(BAK_S_IT_SHOULD_BE_RESTORED_MSG);
    }
    return(0);
}

/* volumesetNamePtr
 *	return a pointer to a (static) volume set name string.
 * entry:
 *	ptr - ptr to a dump name
 * exit:
 *	0 - error. Can't extract volumeset name.
 *	ptr - to static volumeset string.
 */

char *
volumesetNamePtr(ptr)
     char *ptr;
{
    static char vsname[BU_MAXUNAMELEN];
    char *dotPtr;
    int dotIndex;

    dotPtr = strchr(ptr, '.');
    if ( dotPtr == 0 )
	return(0);

    dotIndex = dotPtr - ptr;
    if ( dotIndex + 1 > sizeof(vsname) )
	return(0);				/* name too long */

    strncpy(&vsname[0], ptr, dotIndex);
    vsname[dotIndex] = 0;				/* ensure null terminated */

    return(&vsname[0]);
}



/* extractTapeSeq
 *	The routine assumes that tape names have an embedded sequence number
 *	as the trialing component. It is suggested that any tape naming
 *	changes retain the trailing seq. number
 * entry: 
 *	tapename - ptr to tape name 
 * exit:
 *	0 or positive - sequence number
 *	-1 - error, couldn't extract sequence number
 */

extractTapeSeq(tapename)
     char *tapename;
{
    char *sptr;

    sptr = strrchr(tapename, '.');
    if ( sptr == 0 )
    {
	dce_svc_printf(BAK_S_TAPE_LABEL_IS_MALFORMED_MSG, tapename);
    	return(-1);
    }

    sptr++;
    return(atol(sptr));
}

/* debugging support */

/* dump a section of memory */

dump( st, ptr, size)
     char *st;
     char *ptr;
     long size;
{
    long i;
    long linewidth;

    linewidth = 1;

    printf("%s\n", st);
    for ( i = 1; i <= size; i++ )
    {
	printf("0x%3x ", *ptr);
	ptr++;
	linewidth++;
	if (linewidth >= 10)
	{
	    printf("\n");
	    linewidth = 1;
	}
    }
    printf("\n");
}


/* debugPrintTapeLabel
 *	print out the tape label. Mostly for debugging
 */

static int
debugPrintTapeLabel(labelptr)
     struct butm_tapeLabel *labelptr;
{
    dce_svc_printf(BAK_S_TAPE_LABEL_MSG);
    printf("----------\n");
    printf("structVersion =\t%d\n",labelptr->structVersion);
    printf("creationTime =\t\t(%d) %s", labelptr->creationTime,
    				ctime((time_t *)&labelptr->creationTime));
    printf("expirationDate =\t(%d) %s", labelptr->expirationDate,
    				ctime((time_t *)&labelptr->expirationDate));
    printf("name =\t\t%s\n", labelptr->name);
    dce_svc_printf(BAK_S_SKIPPING_CREATOR_MSG);
    printf("cell =\t\t%s\n", labelptr->cell);
    printf("comment =\t%s\n", labelptr->comment);
    printf("size =\t\t%d\n", labelptr->size);
    dce_svc_printf(BAK_S_END_OF_TAPE_LABEL_MSG);
}

/* debugPrintVolumeHeader
 *	print the contents of a volume header. Mostly for debug
 */
static int
debugPrintVolumeHeader(volHeader)
     struct volumeHeader *volHeader;
{
    afs_hyper_t vid;

    struct sockaddr_in sa;
    dce_svc_printf(BAK_S_FILESET_HEADER_HEADER_MSG);
    printf("-------------\n");
    dce_svc_printf(BAK_S_PREAMBLE_POSTAMBLE_MSG, volHeader->preamble,
	   volHeader->postamble);
    dce_svc_printf(BAK_S_FILESET_NAME_BUTC_MSG, volHeader->volumeName);
    DFSH_MemFromNetHyper(vid, volHeader->volumeID);
    dce_svc_printf(BAK_S_FILESET_ID_MSG, AFS_HGETBOTH(vid));
    sa = *((struct sockaddr_in *) &volHeader->server);
    printf("server %d\n", ntohl(sa.sin_addr.s_addr));
    printf("part %d\n", volHeader->part);
    printf("from %d\n", volHeader->from);
    printf("frag %d\n", volHeader->frag);
    printf("magic %d\n", volHeader->magic);
    printf("contd %d\n", volHeader->contd);
    printf("dumpSetName: %s\n", volHeader->dumpSetName);
    printf("dumpID %d\n", volHeader->dumpID);
    printf("level %d\n", volHeader->level);
    printf("parentID %d\n", volHeader->parentID);
    printf("endTime %d\n", volHeader->endTime);
    printf("versionflags %d\n", volHeader->versionflags);
    printf("clonedate (%d) %s\n", volHeader->cloneDate,
		ctime(&volHeader->cloneDate));
}

/* summary of data contents */
dataSummary()
{
    dce_svc_printf(BAK_S_RAW_DATE_SUMM_MSG);
    printf("----------------\n");
    printf("hw file marks	%d\n", Tstats.c_hw_filemark);
    printf("sw file marks	%d\n", Tstats.c_sw_filemark);
    printf("full_datablocks	%d\n", Tstats.c_full_datablock);
    printf("partial datablocks	%d\n", Tstats.c_partial_datablock);
    printf("vol headers		%d\n", Tstats.c_vol_header);
    printf("labels		%d\n", Tstats.c_label_block);
    printf("data bytes		%u\n", Tstats.c_data_bytes);
    printf("nfilesets		%d\n", Tstats.c_nvolumes);
}
