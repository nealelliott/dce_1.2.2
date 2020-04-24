/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright(c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED(DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright(C) 1991, 1995 Transarc Corporation
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: budb_test.c,v $
	* $EndLog$
 */
/*
 * budb_test.c - Test program for the Backup Database
 */

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dcedfs/stds.h>
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/lock.h>
#include <com_err.h>
#include <dcedfs/cmd.h>
#include <dcedfs/tcdata.h>
#include <dcedfs/butc.h>
#include <dcedfs/bc.h>
#include <dcedfs/flserver.h>
#include <dcedfs/flclient.h>
#include <dcedfs/budb.h>
#include <dce/dce.h>
#include <dce/utc.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

#define DUMPPATH	"DUMP"		/* test dump prefix */
#define VOLSET		"VOL"		/* test volume set prefix */
#define TAPESET		"TAPE"		/* test tape prefix */
#define MAXV		256

/* Global Variables */

static char *whoami = "budb Test Program";
struct udbHandleS udbHandle;
int debugLevel;
int noAuth;
int syncFlag;

/*
 * doubleTime() - return relative time as double precision floating point
 */

double doubleTime(
    double reftime)
{
    double timenow;
    utc_t utc;
    struct tm ttm, itm;
    long tns,ins;
    long tsec;

    utc_gettime(&utc);
    utc_localtime(&ttm,&tns,&itm,&ins,&utc);
    tsec = mktime(&ttm);
    timenow = (double)(tsec) + (double)(tns)/1000000000.0;
    return(timenow-reftime);
}

/*
 * longTime() - return relative time as a long integer
 */

long longTime(
    int reftime)
{
    utc_t utc;
    struct tm ttm, itm;
    long tns,ins;
    long tsec;

    utc_gettime(&utc);
    utc_localtime(&ttm,&tns,&itm,&ins,&utc);
    tsec = mktime(&ttm);
    return(tsec-reftime);
}

/*
 * ubikCreateDump() - Create a Dump and get back the Dump Id
 */

long ubikCreateDump(
    int32 *newDumpId,
    int dumpIndex)
{
    struct budb_dumpEntry dumpStruct;
    long code;

    bzero((char *)&dumpStruct,sizeof(dumpStruct));
    dumpStruct.id     = *newDumpId;
    dumpStruct.parent = 0;  /* Not Important */
    dumpStruct.level  = 0;  /* Full */

    strcpy((char *)dumpStruct.volumeSetName,VOLSET);
    sprintf((char *)dumpStruct.dumpPath,"/%s",DUMPPATH);
    sprintf((char *)dumpStruct.name,"%s.%s.%u",VOLSET,DUMPPATH,dumpIndex);
    dumpStruct.created = longTime(0);
    dumpStruct.incTime = 0;
    dumpStruct.nVolumes = 0;
    dumpStruct.tapes.a = 1;
    dumpStruct.tapes.b = 1;
    dumpStruct.tapes.maxTapes = 0;
    bzero((char *)dumpStruct.tapes.format,sizeof(dumpStruct.tapes.format));

    code = ubik_Call(BUDB_CreateDump,udbHandle.uh_client,
		     syncFlag,&dumpStruct);
    if ( code )
    {
	printf("createDump Failed: code=%d, '%s'\n",code,dfs_dceErrTxt(code));
	fflush(stdout);
    }
    return(code);
}

/*
 * ubikFinishDump() - Finish a Dump
 */

long ubikFinishDump(
    int32 dumpId,
    int dumpIndex)
{
    struct budb_dumpEntry dumpStruct;
    long code;

    bzero((char *)&dumpStruct,sizeof(dumpStruct));
    dumpStruct.id     = dumpId;
    dumpStruct.parent = 0;  /* Not Important */
    dumpStruct.level  = 0;  /* Full */

    strcpy((char *)dumpStruct.volumeSetName,VOLSET);
    sprintf((char *)dumpStruct.dumpPath,"/%s",DUMPPATH);
    sprintf((char *)dumpStruct.name,"%s.%s.%u",VOLSET,DUMPPATH,dumpIndex);
    dumpStruct.created = longTime(0);
    dumpStruct.incTime = 0;
    dumpStruct.nVolumes = 0;
    dumpStruct.tapes.a = 1;
    dumpStruct.tapes.b = 1;
    dumpStruct.tapes.maxTapes = 0;

    code = ubik_Call(BUDB_FinishDump,udbHandle.uh_client,
		     syncFlag,&dumpStruct);
    if ( code )
    {
	printf("finishDump Failed: code=%d, '%s'\n",code,dfs_dceErrTxt(code));
	fflush(stdout);
    }
    return(code);
}

/*
 * ubikDeleteDump() - delete the dump and all that goes with it
 */

long ubikDeleteDump(
    int32 dumpId)
{
    long code;

    code = ubik_Call(BUDB_DeleteDump,udbHandle.uh_client,
		     syncFlag,dumpId);
    if ( code )
    {
	printf("deleteDump Failed: code=%d, '%s'\n",code,dfs_dceErrTxt(code));
	fflush(stdout);
    }
    return(code);
}

/*
 * ubikDeleteVDP() - delete the dump and all that goes with it
 */

long ubikDeleteVDP(
    char *dsName,
    char *dumpPath,
    int32 dumpId)
{
    long code;

    code = ubik_Call(BUDB_DeleteVDP,udbHandle.uh_client,
		     syncFlag,
		     dsName,dumpPath,dumpId);
    if ( code )
    {
	printf("deleteVDP Failed: code=%d, '%s'\n",code,dfs_dceErrTxt(code));
	fflush(stdout);
    }
    return(code);
}

/*
 * ubikUseTape() - Add a new tape to the dump
 */

long ubikUseTape(
    int32 dumpId,
    int dumpIndex,
    int tapeIndex)
{
    struct budb_tapeEntry tapeStruct;
    long code;
    long new = 0;

    bzero((char *)&tapeStruct,sizeof(tapeStruct));
    tapeStruct.seq = tapeIndex;
    sprintf((char *)tapeStruct.name,"%s.%u.%u",TAPESET,dumpIndex,tapeIndex);

    tapeStruct.flags = BUDB_TAPE_BEINGWRITTEN;
    tapeStruct.written = longTime(0);
    tapeStruct.expires = 0;
    tapeStruct.dump = dumpId;
    tapeStruct.nVolumes = 0;
    tapeStruct.nFiles = 0;
    tapeStruct.nMBytes = tapeStruct.nBytes = 0;
    tapeStruct.useCount = 0;

    code = ubik_Call(BUDB_UseTape,udbHandle.uh_client,
		     syncFlag,&tapeStruct,&new);
    if ( code )
    {
	printf("UseTape Failed: code=%d. '%s'\n",code,dfs_dceErrTxt(code));
	fflush(stdout);
	return(code);
    }
    return(0);
}

/*
 * ubikFinishTape() - Add a new tape to the dump
 */

long ubikFinishTape(
    int32 dumpId,
    int dumpIndex,
    int tapeIndex)
{
    struct budb_tapeEntry tapeStruct;
    long code;
    long new = 0;

    bzero((char *)&tapeStruct,sizeof(tapeStruct));
    tapeStruct.seq = tapeIndex;
    sprintf((char *)tapeStruct.name,"%s.%u.%u",TAPESET,dumpIndex,tapeIndex);

    tapeStruct.flags = BUDB_TAPE_WRITTEN;
    tapeStruct.written = longTime(0);
    tapeStruct.expires = 0;
    tapeStruct.dump = dumpId;
    tapeStruct.nVolumes = 0;
    tapeStruct.nFiles = 0;
    tapeStruct.nMBytes = tapeStruct.nBytes = 0;
    tapeStruct.useCount = 0;

    code = ubik_Call(BUDB_UseTape,udbHandle.uh_client,
		     syncFlag,&tapeStruct,&new);
    if ( code )
    {
	printf("UseTape Failed: code=%d, '%s'\n",code,dfs_dceErrTxt(code));
	fflush(stdout);
	return(code);
    }
    return(0);
}

/*
 * ubikAddVolumes() - Add array of volume entries to the Backup Database
 */

long ubikAddVolumes(
    int32 dumpId,
    int dumpIndex,
    int tapeIndex,
    int volIndex,
    int volCount)
{
    long code;
    int index;
    static struct budb_volumeEntry volArray[BUDB_MAX_VOL_COUNT];

    /* Values for the fields are not meaningful */
    for ( index = 0 ; index < volCount ; index++ )
    {
	sprintf((char *)volArray[index].name,"%s.%u.%u",VOLSET,
		tapeIndex,index+volIndex);
	volArray[index].flags = 0;
	AFS_hset64(volArray[index].id, 0, index+volIndex);
	volArray[index].position = index+volIndex;
	volArray[index].clone = longTime(0);
	volArray[index].nBytes = index+volIndex;
	volArray[index].seq = index+volIndex;
	volArray[index].dump = dumpId;
	sprintf((char *)volArray[index].tape,"%s.%u.%u",TAPESET,
		dumpIndex,tapeIndex);
    }

    if ( volCount == 1 )
    {
	code = ubik_Call(BUDB_AddVolume,udbHandle.uh_client,
			 syncFlag,&volArray[0]);
    }
    else
    {
	code = ubik_PVCall(BUDB_AddVolumes,udbHandle.uh_client,
			   syncFlag,2,volCount,volArray);
	if ( code == UDOWNLVL )
	{
	    code = 0;
	    for ( index = 0 ; index < volCount ; index++ )
	    {
		code = ubik_Call(BUDB_AddVolume,udbHandle.uh_client,
				 syncFlag,&volArray[index]);
		if ( code ) break;
	    }
	}
    }
    if ( code )
    {
	printf("AddVolume Failed: code=%d, '%s'\n",code,dfs_dceErrTxt(code));
	fflush(stdout);
	return(code);
    }
    return(0);
}

/*
 * safely convert string to integer
 */

int budbStringToInt(
    char *str,
    int *res)
{
    if ( str == NULL || res == NULL ) return(EFAULT);

    *res = 0;
    while( *str != '\0' )
    {
	if (*str < '0' || *str > '9') return(EINVAL);

	/*
	 * (*res) = (*res) * 10 + (*str - '0')
	 */

	*res = ((((*res) << 2) + (*res)) << 1) + (*str - '0');
	str++;
    }

    return(0);
}

/*
 * UbikInit() - initialize communications with budb database
 */

int UbikInit(
    struct cmd_syndesc *as,
    char *arock)
{
    long code;
    char *ptr;
    char *groupName;
    char *cellName;
    int i;
    error_status_t st=0;
    char sfxBuf[200];
    double start;
    double elapsed;

    printf("\n");
    printf("Initialize Ubik\n");
    printf("\n");
    fflush(stdout);

    start = doubleTime(0);

    dce_cf_get_cell_name(&cellName,&st);
    if (st)
    {
	com_err(whoami,st,"Unable to obtain local cell name");
	return(1);
    }
    printf("Cell name: '%s'\n",cellName);
    fflush(stdout);

    dfs_GetBakJunctionName((char *)cellName,(char *)sfxBuf,
			   (int)sizeof(sfxBuf),(unsigned32 *)&st);

    if ((unsigned32)st != 0)
    {
	 printf("dfs_GetBakJunctionName(%s): '%s'",
		  cellName,dfs_dceErrTxt(st));
	 return(st);
    }

    groupName =(char *)osi_Alloc ((strlen(cellName)+strlen(sfxBuf)+1));
    if (groupName == (char *)NULL)
    {
	perror("malloc group name");
	exit(1);
    }
    sprintf(groupName,"%s%s",cellName,sfxBuf);
    printf("Group name: '%s'\n",groupName);
    fflush(stdout);

    code = ubik_ClientInit(NULL,(unsigned_char_t *)groupName,
			   (unsigned_char_t *)"subsys/dce/dfs-bak-servers",
			   noAuth,0,1,
			   (struct ubik_client **)&udbHandle.uh_client);
    if ( code )
    {
	printf("Ubik client initialization failed, code=%d, '%s'\n",
	       code,dfs_dceErrTxt(code));
	return(code);
    }
    printf("Client handle: %x\n",udbHandle.uh_client);
    fflush(stdout);

    code = ubik_Call(BUDB_GetInstanceId,udbHandle.uh_client,
		     syncFlag,&udbHandle.uh_instanceId);
    if ( code )
    {
	printf("Cannot establish instance ID: code=%d, '%s'\n",
	       code,dfs_dceErrTxt(code));
	return(code);
    }
    printf("Instance ID: %d\n",udbHandle.uh_instanceId);
    fflush(stdout);

    elapsed = doubleTime(start);

    printf("\n");
    printf("Ubik Initialize: %.1f\n",elapsed);

    return(0);
}

/*
 * SetEnv() - set or display environment variables
 */

int SetEnv(
    struct cmd_syndesc *as,
    char *arock)
{
    long code;

    if ( as->parms[0].items )
    {
	if ( strcmp(as->parms[0].items->data,"on") == 0 )
	{
	    noAuth = 0;
	}
	else if ( strcmp(as->parms[0].items->data,"off") == 0 )
	{
	    noAuth = 1;
	}
	else
	{
	    printf("Invalid auth flag: '%s'\n",as->parms[0].items->data);
	    fflush(stdout);
	    return(EINVAL);
	}
    }

    if ( as->parms[1].items )
    {
	if ( strcmp(as->parms[1].items->data,"on") == 0 )
	{
	    syncFlag = UF_SYNC_SITE;
	}
	else if ( strcmp(as->parms[1].items->data,"off") == 0 )
	{
	    syncFlag = 0;
	}
	else
	{
	    printf("Invalid sync flag: '%s'\n",as->parms[1].items->data);
	    fflush(stdout);
	    return(EINVAL);
	}
    }

    if ( as->parms[2].items )
    {
	code = budbStringToInt(as->parms[2].items->data,&debugLevel);
	if ( code )
	{
	    printf("Invalid Debug Level: '%s'\n",as->parms[2].items->data);
	    fflush(stdout);
	    return(code);
	}
    }

    printf("\n");
    printf("Environment Variables:\n");
    printf("\n");
    printf("    SyncFlag:       %s\n",syncFlag?"ON":"OFF");
    printf("    Authentication: %s\n",noAuth?"OFF":"ON");
    printf("    DebugLevel:     %d\n",debugLevel);

    fflush(stdout);
    return(0);
}

/*
 * VerifyDB() - Verify the Backup Database
 */

int VerifyDB(
    struct cmd_syndesc *as,
    char *arock)
{
    long status;
    long orphans;
    long host;
    struct hostent *hostPtr;
    long code;
    double start;			/* start time */
    double elapsed;			/* elapsed time */

    printf("\n");
    printf("Verify Database\n");
    printf("\n");
    fflush(stdout);

    start = doubleTime(0);

    code = ubik_Call(BUDB_DbVerify,udbHandle.uh_client,0,
		     &status,&orphans,&host);
    if ( code )
    {
	/* Error : Unable to verify Database */
	printf("BUDB_DbVerify Failed: code=%d, '%s'\n",dfs_dceErrTxt(code));
	fflush(stdout);
	return(code);
    }
    if ( status == 0 )
    {
	printf("Database OK\n");
	printf("\n");
	fflush(stdout);
    }
    else
	/* Error: Database NOT OK */
    {
	printf("Database is NOT_OK: status=%d\n",status);
	fflush(stdout);
	printf("Orphan blocks %d\n",orphans);
	fflush(stdout);
	hostPtr = gethostbyaddr((char *) &host,sizeof(host),AF_INET);
	if (hostPtr == 0 )
	    printf("Unable to lookup host id\n");
	else
	    printf("Database checker was %s\n",hostPtr->h_name);
	printf("\n");
	fflush(stdout);
    }

    elapsed = doubleTime(start);

    printf("VerifyDB: %.1f\n",elapsed);
    fflush(stdout);
    return(0);
}

/*
 * GenDumps() - Populate backup database with test data
 */

int GenDumps(
    struct cmd_syndesc *as,
    char *arock)
{
    int32 dumpId;			/* dump identifier */
    int code;				/* return code */
    double start;			/* start time */
    double elapsed;			/* elapsed time */
    double dumpstart;			/* start time for current dump */
    int startIndex;			/* Index for first new dump */
    int dumpIndex;			/* Index for next new dump */
    int numDump;			/* number of dumps to create */
    int numTape;			/* number of tapes per dump */
    int numVol;				/* number of volumes per tape */
    int tapeIndex;			/* loop index */
    int volIndex;			/* loop index */
    int i;				/* loop index */
    int volCount;			/* volumes added this iteration */
    int maxVol;				/* volumes added per transaction */

    code = 0;

    printf("\n");
    printf("Generate Dumps\n");
    printf("\n");
    fflush(stdout);

    /*
     * parse the command arguments
     */

    if ( as->parms[0].items )
    {
	code = budbStringToInt(as->parms[0].items->data,&startIndex);
	if ( code )
	{
	    printf("Invalid Dump Index: '%s'\n",as->parms[0].items->data);
	    fflush(stdout);
	    return(code);
	}
    }
    else return(EINVAL);

    if ( as->parms[1].items )
    {
	code = budbStringToInt(as->parms[1].items->data,&numDump);
	if ( code )
	{
	    printf("Invalid Dump Count: '%s'\n",as->parms[1].items->data);
	    fflush(stdout);
	    return(code);
	}
    }
    else return(EINVAL);

    if ( as->parms[2].items )
    {
	code = budbStringToInt(as->parms[2].items->data,&numTape);
	if ( code )
	{
	    printf("Invalid Tape Count: '%s'\n",as->parms[2].items->data);
	    fflush(stdout);
	    return(code);
	}
    }
    else return(EINVAL);

    if ( as->parms[3].items )
    {
	code = budbStringToInt(as->parms[3].items->data,&numVol);
	if ( code )
	{
	    printf("Invalid Volume Count: '%s'\n",as->parms[3].items->data);
	    fflush(stdout);
	    return(code);
	}
    }
    else return(EINVAL);

    if ( as->parms[4].items )
    {
	code = budbStringToInt(as->parms[4].items->data,&maxVol);
	if ( code != 0 || (maxVol <= 0 || maxVol > BUDB_MAX_VOL_COUNT) )
	{
	    printf("Invalid Volume Count: '%s'\n",as->parms[3].items->data);
	    fflush(stdout);
	    return(code?code:EINVAL);
	}
    }
    else maxVol = BUDB_MAX_VOL_COUNT;

    printf("Start index = %d\n",startIndex);
    printf("Number dumps = %d\n",numDump);
    printf("Tapes per dump = %d\n",numTape);
    printf("Filesets per tape = %d\n",numVol);
    printf("Filesets per transaction = %d\n",maxVol);
    printf("\n");
    fflush(stdout);

    start = doubleTime(0.0);
    dumpIndex = startIndex;
    for ( i = 0 ; i < numDump ; i++ )
    {
	dumpstart = doubleTime(0.0);

	/*
	 * create a dump
	 */
    
	dumpId = 0x12210000 + dumpIndex;
	code = ubikCreateDump(&dumpId,startIndex);
	if ( code )
	{
	     printf("CreateDump: code=%d, '%s'\n",code,dfs_dceErrTxt(code));
	     fflush(stdout);
	     return(code); 
	}
    
	if ( debugLevel > 0 )
	{
	    printf("Add Dump %s.%s.%u: id=%08X\n",
		   VOLSET,DUMPPATH,startIndex,dumpId);
	    fflush(stdout);
	}
    
	for ( tapeIndex = 0 ; tapeIndex < numTape ; tapeIndex++ )
	{
	    /*
	     * add a tape to the current dump
	     */
   
	   if ( debugLevel > 0 )
	    {
		printf("    ");
		printf("UseTape %s.%u.%u\n",TAPESET,dumpIndex,tapeIndex);
		fflush(stdout);
	    }
   
	    code = ubikUseTape(dumpId,dumpIndex,tapeIndex);
	    if ( code )
	    {
		printf("UseTape: code=%d, '%s'\n",code,dfs_dceErrTxt(code));
		fflush(stdout);
		return(code);
	    }
   
	    for ( volIndex = 0 ; volIndex < numVol ; volIndex += maxVol )
	    {
		if ( volIndex + maxVol >= numVol )
		{
		    volCount = numVol - volIndex;
		}
		else volCount = maxVol;
   
		/*
		 * Add some volumes to the current tape
		 */
   
		if ( debugLevel > 0 )
		{
		    printf("        ");
		    if ( volCount == 1 )
		    {
			printf("AddVol %s.%u.%u\n",VOLSET,tapeIndex,volIndex);
		    }
		    else
		    {
			printf("AddVol %s.%u.%u - %s.%u.%u\n",
			       VOLSET,tapeIndex,volIndex,
			       VOLSET,tapeIndex,volIndex+volCount-1);
		    }
		    fflush(stdout);
		}

		code = ubikAddVolumes(dumpId,dumpIndex,tapeIndex,
					  volIndex,volCount);
		if ( code )
		{
		    printf("AddVolumes: code=%d, '%s'\n",
			   code,dfs_dceErrTxt(code));
		    fflush(stdout);
		    return(code);
		}
	    }
   
	    /*
	     * finish the current tape
	     */
   
	    if ( debugLevel > 0 )
	    {
		printf("    ");
		printf("FinishTape %s.%u.%u\n",TAPESET,dumpIndex,tapeIndex);
		fflush(stdout);
	    }
   
	    code = ubikFinishTape(dumpId,dumpIndex,tapeIndex);
	    if ( code )
	    {
		printf("FinishTape: code=%d, '%s'\n",code,dfs_dceErrTxt(code));
		fflush(stdout);
		return(code);
	    }
	}
   
	/*
	 * Finish the current dump
	 */
   
	code = ubikFinishDump(dumpId,dumpIndex);
	if ( code )
	{
	    printf("FinishDump: code=%d, '%s'\n",code,dfs_dceErrTxt(code));
	    fflush(stdout);
	    return(code); 
	}

	elapsed = doubleTime(dumpstart);
	printf("%s.%s.%u (%08X): %.1f\n",
	       VOLSET,DUMPPATH,startIndex,dumpId,elapsed);
	fflush(stdout);
	dumpIndex++;
    }
    elapsed = doubleTime(start);

    printf("\n");
    printf("Generate Dumps: %.1f\n",elapsed);
    fflush(stdout);
    return(0);
}

/*
 * DelDumps() - Remove test data from backup database
 */

int DelDumps(
    struct cmd_syndesc *as,
    char *arock)
{
    int32 dumpId;			/* dump identifier */
    int startIndex;			/* start index */
    int dumpIndex;			/* dump index */
    int numDump;			/* number dumps */
    double start;			/* start time */
    double dumpstart;			/* dump start time */
    double elapsed;			/* elapsed time */
    int code;				/* return code */
    int i;				/* loop index */

    printf("\n");
    printf("Delete Dumps\n");
    printf("\n");
    fflush(stdout);

    /*
     * parse the command arguments
     */

    if ( as->parms[0].items )
    {
	code = budbStringToInt(as->parms[0].items->data,&startIndex);
	if ( code )
	{
	    printf("Invalid Dump Index: '%s'\n",as->parms[0].items->data);
	    fflush(stdout);
	    return(code);
	}
    }
    else return(EINVAL);

    if ( as->parms[1].items )
    {
	code = budbStringToInt(as->parms[1].items->data,&numDump);
	if ( code )
	{
	    printf("Invalid Dump Count: '%s'\n",as->parms[1].items->data);
	    fflush(stdout);
	    return(code);
	}
    }
    else return(EINVAL);

    printf("Start index = %d\n",startIndex);
    printf("Number dumps = %d\n",numDump);
    printf("\n");
    fflush(stdout);

    /*
     * delete the dumps
     */

    start = doubleTime(0.0);
    dumpIndex = startIndex;
    for ( i = 0 ; i < numDump ; i++ )
    {
	dumpstart = doubleTime(0.0);

	dumpId = 0x12210000 + dumpIndex;

	if ( debugLevel > 0 )
	{
	    printf("Delete Index %d (%08X)\n",dumpIndex,dumpId);
	    fflush(stdout);
	}

	code = ubikDeleteDump(dumpId);
	if ( code )
	{
	    printf("deleteDumpById failed: code=%d, '%s'\n",
		   code,dfs_dceErrTxt(code));
	    fflush(stdout);
	    return(code);
	}

	elapsed = doubleTime(dumpstart);
	printf("Index %d (%08X):%.1f\n",dumpIndex,dumpId,elapsed);
	fflush(stdout);
	dumpIndex++;
    }
    elapsed = doubleTime(start);

    printf("\n");
    printf("Delete Dumps: %.1f\n",elapsed);
    fflush(stdout);

    return(0);
}

/*
 * DelVDP() - Remove all dumps with given name and volumeset
 */

int DelVDP(
    struct cmd_syndesc *as,
    char *arock)
{
    int startIndex;			/* first index in sequence */
    int curIndex;			/* dump index to preserve */
    int curId;				/* dump ID to preserve */
    char dsName[128];			/* dataset name */
    char dumpPath[128];			/* dump path */
    double start;			/* start time */
    double elapsed;			/* elapsed time */
    int code;				/* return code */

    printf("\n");
    printf("Delete VDP\n");
    printf("\n");
    fflush(stdout);

    /*
     * parse the command arguments
     */

    if ( as->parms[0].items )
    {
	code = budbStringToInt(as->parms[0].items->data,&startIndex);
	if ( code )
	{
	    printf("Invalid Start Index: '%s'\n",as->parms[0].items->data);
	    fflush(stdout);
	    return(code);
	}
    }
    else return(EINVAL);

    if ( as->parms[1].items )
    {
	code = budbStringToInt(as->parms[1].items->data,&curIndex);
	if ( code )
	{
	    printf("Invalid Dump Index: '%s'\n",as->parms[0].items->data);
	    fflush(stdout);
	    return(code);
	}
    }
    else curIndex = -1;

    if ( curIndex >= 0 ) curId = 0x12210000 + curIndex;
    else curId = 0;
    sprintf(dsName,"%s.%s.%u",VOLSET,DUMPPATH,startIndex);
    sprintf(dumpPath,"/%s",DUMPPATH);

    printf("Dump name = '%s'\n",dsName);
    printf("Dump path = '%s'\n",dumpPath);
    printf("Preserve ID = %08X\n",curId);
    printf("\n");
    fflush(stdout);

    start = doubleTime(0.0);

    /*
     * delete the dumps
     */

    code = ubikDeleteVDP(dsName,dumpPath,curId);
    if ( code )
    {
	    printf("deleteVDP failed: code=%d, '%s'\n",
		   code,dfs_dceErrTxt(code));
	    fflush(stdout);
    }

    elapsed = doubleTime(start);

    printf("Delete VDP: %.1f\n",elapsed);
    fflush(stdout);

    return(0);
}

/*
 * Quit() - exit the test routine
 */

int Quit(
    struct cmd_syndesc *as,
    char *arock)
{
    exit(0);
}

/*
 * Usage() - tell how to run and exit
 */

int Usage(void)
{
    fprintf(stderr,"Usage: budb_test\n");
    fflush(stderr);
    exit(1);
}

/*
 * main() - Set up the Command Syntax for Backup Database checking,
 *           and then Dispatch commands entered.
 */

main(argc,argv)
  int argc;
  char *argv[];

{
    register struct cmd_syndesc *ts;             /* Syntax Handle */
    long targc;                                  /* number of args */
    char *targv[MAXV];                           /* array of args */
    char linebuf[1024];                          /* buffer for input */
    char *ptr;					 /* generic pointer */
    long code;                                   /* error code */
    unsigned32 status;

    debugLevel = 0;
    noAuth = 0;
    syncFlag = UF_SYNC_SITE;

    /* no command options */
    if ( argv[1] != NULL ) Usage();

    /** Create new commands **/

    /* verify command: verify the backup database */
    ts = cmd_CreateSyntax("verifydb",VerifyDB,0,"Verify backup database");

    /* ubikinit command: Initialize ubik */
    ts = cmd_CreateSyntax("ubikinit",UbikInit,0,"Initialze Ubik");

    /*
     * setenv command: set or display environment variables
     */

    ts = cmd_CreateSyntax("setenv", SetEnv, 0, "Set Enviroment Variables");
    cmd_AddParm(ts, "-authentication", CMD_SINGLE, CMD_OPTIONAL,
		"on/off");
    cmd_AddParm(ts, "-syncflag", CMD_SINGLE, CMD_OPTIONAL,
		"on/off");
    cmd_AddParm(ts, "-debuglevel", CMD_SINGLE, CMD_OPTIONAL,
		"debug level");

    /*
     * gendumps command: add dumps to the backup database
     */

    ts = cmd_CreateSyntax("gendumps", GenDumps, 0, "Generate Dumps");
    cmd_AddParm(ts, "-index", CMD_SINGLE, CMD_REQUIRED,
		"First dump index");
    cmd_AddParm(ts, "-dumps", CMD_SINGLE, CMD_REQUIRED,
		"Number dumps to create");
    cmd_AddParm(ts, "-tapes", CMD_SINGLE, CMD_REQUIRED,
		"Number tapes per dump");
    cmd_AddParm(ts, "-filesets", CMD_SINGLE, CMD_REQUIRED,
		"Number filesets per tape");
    cmd_AddParm(ts, "-maxfs", CMD_SINGLE, CMD_OPTIONAL,
		"Filsets added per transaction");

    /*
     * deldumps command: delete dumps from the backup database
     */

    ts = cmd_CreateSyntax("deldumps", DelDumps, 0, "Delete Dumps");
    cmd_AddParm(ts, "-index", CMD_SINGLE, CMD_REQUIRED,
		"First dump index");
    cmd_AddParm(ts, "-dumps", CMD_SINGLE, CMD_REQUIRED,
		"Number dumps to delete");

    /*
     * delvdp command: delete set of dumps from the backup database
     */

    ts = cmd_CreateSyntax("delvdp", DelVDP, 0, "Delete dump set");
    cmd_AddParm(ts, "-index", CMD_SINGLE, CMD_REQUIRED,
		"First dump index");
    cmd_AddParm(ts, "-keep", CMD_SINGLE, CMD_OPTIONAL,
		"Index to preserve");

    /*
     * quit command: quit from budb test
     */

    ts = cmd_CreateSyntax ("quit", Quit, 0, "Exit the Test Program");
    cmd_CreateAlias (ts, "exit");

    /*
     * Start Dispatch Loop
     */

    while (1)
    {
	    fprintf(stdout, "\nbudb> ");  /* prompt */
	fflush(stdout);

	ptr = gets(linebuf);
	if (!ptr)
	{
	    fprintf(stdout, "\n");
	    exit(0); /* EOF */
	}
	while ( *ptr != '\0' && isspace(*ptr) ) ptr++;
	if ( *ptr != '\0' )
	{
	    code = cmd_ParseLine (ptr, targv, &targc, MAXV);
	    if ( code )
	    {
		fprintf (stdout, "Couldn't parse line:  code=%d, '%s'",
			 code,dfs_dceErrTxt(code));
	    }
	    else
	    {
		cmd_Dispatch (targc, targv);
		cmd_FreeArgv (targv);
	    }
	}
    }
}
