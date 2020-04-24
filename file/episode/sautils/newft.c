/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: newft.c,v $
 * Revision 1.1.85.1  1996/10/02  17:27:56  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:17  damon]
 *
 * $EndLog$
 */
/* efts.c */

/*
 * (C) Copyright 1996, 1991 Transarc Corporation
 * All Rights Reserved
 */

/*
 * This command creates a new fileset in an existing aggregate.
 */

#include <stdio.h>
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/cmd.h>
#include <dcedfs/ftutil.h>
#include <dcedfs/vol_errs.h>

#include <efts_userint.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/sautils/newft.c,v 1.1.85.1 1996/10/02 17:27:56 damon Exp $")

/*
 * static function prototypes
 */

static long LookupAggregate(char *devName, ftu_aggrId_t *aggrId);
static void CommonFlags(struct cmd_syndesc *as);
static char *AggrTypeToString(long type);
static long LookupAggregate(char *name, ftu_aggrId_t *aggrId);
static long NewFtId(ftu_aggrId_t aggrId, u_long *newId);
static long GetFilesetId(ftu_aggrId_t aggrId, struct cmd_syndesc *as,
			 unsigned idOffset, unsigned nameOffset,
			 ftu_fsetId_t* fsetId);
static void DisplayFtType(struct vol_status *stsp);
static void DisplayOnLine(struct vol_status *stsp);

static int	verbose = 0;
static int	quiet = 0;
static char	*progname = "efts";
static int      dumpTraceLog = 0;

struct icl_log *iclLogP = 0;

/* Default fileset quota */
#ifndef DEFAULT_MAX_QUOTA
#define DEFAULT_MAX_QUOTA 5000
#endif /* DEFAULT_MAX_QUOTA */

#ifndef DEFAULT_NVOLS
#define DEFAULT_NVOLS 100
#endif

/*
 * Utilities
 */

/* Use VPrintf for verbose messages.  Use Printf for messages which can */
/* be suppressed with -quiet. */
#define VPrintf(x) if (verbose) (void)printf x
#define Printf(x) if (!quiet) (void)printf x

#define GET_AGGR_ID(as, offset, aggrId) \
     LookupAggregate((as)->parms[(offset)].items->data, aggrId)


static void CommonFlags(struct cmd_syndesc *as)
{
    int code;

    quiet = as->parms[QUIET_OFFSET].items != 0;
    verbose = quiet ? 0 : (as->parms[VERBOSE_OFFSET].items != 0);
    dumpTraceLog = as->parms[DUMPTRACELOG_OFFSET].items != 0;
    if (verbose) {
	ftu_SetLogStream(stderr);
    }
    if (dumpTraceLog) {
	if (code = icl_CreateLog("efts", 0, &iclLogP)) {
	    fprintf(stderr,
		    "Failed to create ICL log, code = %d -- continuing\n",
		    code);
	} else if ( code = ftu_CreateIclSet(iclLogP)) {
	    fprintf(stderr,
		    "Failed to create ftutil ICL event set, code = %d -- continuing\n",
		    code);
    	}
    }
}


/*
 * AggrTypeToString - convert the numeric type to a character string
 */
static char *AggrTypeToString(long type)
{
    static char *ag_types[MAX_AG_TYPE] = {
	"unknown",
	"ufs",
	"episode",
	"jfs",
	"vxfs",
	"dmepi",
    };

    return (type < MAX_AG_TYPE) ? ag_types[type] : ag_types[0];
}

/*
 * LookupAggregate - find information about an aggregate
 */
static long LookupAggregate(char *name, ftu_aggrId_t *aggrId)
{
    long code;

    if ((code = ftu_LookupAggrByName(name, aggrId)) != 0) {
	char fullDevName[MAXPATHLEN+1];

	code = ftu_GetDevice(name, fullDevName, (struct stat *)0);

	if (!code)
	    code = ftu_LookupAggrByDevice(fullDevName, aggrId);
    }

    return code;
}

/*
 * NewFtId - provide a unique 32 bit number which can be used as a fileset
 * identifier.
 */

static long NewFtId(ftu_aggrId_t aggrId, u_long *newId)
{
    unsigned index = 0, index2, maxId = 0;
    struct vol_status ftStatusArray[DEFAULT_NVOLS];
    long code;
    unsigned fsetsReturned;

    do {
	code = ftu_AggrEnumerateFsets(aggrId, &index,
				      ftStatusArray, DEFAULT_NVOLS,
				      &fsetsReturned);
	if (code)
	    return code;

	for (index2 = 0; index2 < fsetsReturned; index2++)
	    maxId = (maxId > AFS_hgetlo(ftStatusArray[index2].vol_st.volId)) ?
		maxId : AFS_hgetlo(ftStatusArray[index2].vol_st.volId);
    } while (!(fsetsReturned < DEFAULT_NVOLS));

    maxId++;
    if (newId)
	*newId = maxId;

    return 0;
}

static long GetFilesetId(
    ftu_aggrId_t aggrId,
    struct cmd_syndesc *as,
    unsigned idOffset,
    unsigned nameOffset,
    ftu_fsetId_t* fsetId)
{
    long code = 0;

    if (as->parms[idOffset].items)
	code = ftu_ParseFsetId(as->parms[idOffset].items->data, fsetId);
    else if (as->parms[nameOffset].items)
	code = ftu_AggrLookupFset(aggrId, as->parms[nameOffset].items->data,
				  fsetId);
    else {
	fprintf(stderr, "%s: Must specify either '-id' or '-name'\n",
		progname);
	code = CMD_TOOFEW;
    }

    return code;
}
/*
 * Interface routines
 */

/*
 * EnumerateFilesets - list all filesets on a given aggregate
 */
/* SHARED */
/* ARGSUSED */
int efts_Lsft(struct cmd_syndesc *as, char *arock)
{
    unsigned fsetIndex = 0;
    struct vol_status ftStatusArray[DEFAULT_NVOLS];
    long code;
    ftu_aggrId_t aggrId;
    unsigned fsetsReturned;
    unsigned i;
      
    CommonFlags(as);

    if (code = GET_AGGR_ID(as, LSFT_AGGREGATE_OFFSET, &aggrId))
	return code;

    Printf(("%-16s %10s%-12s\n", "NAME", " ", "ID"));

    do {
	code = ftu_AggrEnumerateFsets(aggrId, &fsetIndex,
				      ftStatusArray, DEFAULT_NVOLS,
				      &fsetsReturned);
	if (code)
	    break;

	for (i = 0; i < fsetsReturned; i++)
	    printf("%-16s %10lu,,%-10lu\n",
		   ftStatusArray[i].vol_st.volName,
		   AFS_HGETBOTH(ftStatusArray[i].vol_st.volId));
    } while (!(fsetsReturned < DEFAULT_NVOLS));

    return code;
}

/*
 * EnumerateAggregates - list all aggregates on the system
 */
/* SHARED */
/* ARGSUSED */
int efts_Lsaggr(struct cmd_syndesc *as, char *arock)
{
    long code;
    int i;
    struct aggrList *aggrs;
    unsigned numAggrs = 0;

    CommonFlags(as);

    if (code = ftu_ListAggrs(&aggrs, &numAggrs))
	return code;

    if (numAggrs) {
	struct aggrInfo aggrInfo;

	Printf(("%-16s %10s %-8s %10s %10s\n",
		"DEVICE NAME", "ID", "TYPE", "TOTAL", "FREE"));

	for(i = numAggrs - 1; i >= 0; --i) {
	    code = ftu_AggrGetInfo(aggrs[i].Id, &aggrInfo);
	    if (code) {
		(void)fprintf(stderr, "%s: Can't get info on %s (id: %lu): ",
			      progname, aggrs[i].dvname, aggrs[i].Id);
		com_err(0, code, 0);
		continue;		/* we continue on though... */
	    }
	    (void)printf("%-16s %10lu %-8s %10ld %10ld\n",
			 aggrs[i].dvname, aggrs[i].Id,
			 AggrTypeToString(aggrInfo.type),
			 aggrInfo.totalUsable, aggrInfo.realFree);
	}
    } else {
	Printf(("No aggregates registered\n"));
    }

    if (aggrs)
	osi_Free(aggrs, numAggrs*sizeof(struct aggrList));

    return 0;
}

/*
 * DetachAggregate - unregister an aggregate (this handles the user i.f.)
 */
/* SHARED */
/* ARGSUSED */
int efts_Detach(struct cmd_syndesc *as, char *arock)
{
    long code;
    ftu_aggrId_t aggrId;

    CommonFlags(as);

    if ((code = GET_AGGR_ID(as, DETACH_AGGREGATE_OFFSET, &aggrId)) == 0)
	code = ftu_DetachAggr(aggrId, FTU_ATTACH_FLAGS_MOUNTAGGR);

    return code;
}

/* SHARED */
/* ARGSUSED */
int efts_Attach(struct cmd_syndesc *as, char *arock)
{
    char *devName;
    long code;
    char fullDevName[MAXPATHLEN + 1];
    struct stat statBuf;
    char *aggrName;
    ftu_aggrId_t aggrId;

    CommonFlags(as);

    devName = as->parms[ATTACH_DEVICE_OFFSET].items->data;

    if (code = ftu_GetDevice(devName, fullDevName, &statBuf))
	return code;

    if (as->parms[ATTACH_NAME_OFFSET].items)
	aggrName = as->parms[ATTACH_NAME_OFFSET].items->data;
    else
	aggrName = fullDevName;

    if (as->parms[ATTACH_ID_OFFSET].items)
	aggrId = atol(as->parms[ATTACH_ID_OFFSET].items->data);
    else
	aggrId = statBuf.st_rdev;

    code = ftu_AttachAggr(fullDevName, ASTABTYPE_EPI, aggrName, aggrId,
			  FTU_ATTACH_FLAGS_MOUNTAGGR, (caddr_t)0);

    return code;
}

/* SHARED */
/* ARGSUSED */
int efts_CreateFt(struct cmd_syndesc *as, char *arock)
{
    long code;
    ftu_aggrId_t aggrId;
    afs_hyper_t ftId;
    char *aggr;
    char *name;
    char buf[512];
    long maxQuota = DEFAULT_MAX_QUOTA;
    ftu_fsetDesc_t ftDesc;
    struct vol_status status;

    CommonFlags(as);

    aggr = as->parms[CREATEFT_AGGREGATE_OFFSET].items->data;
    if (code = LookupAggregate(aggr, &aggrId))
	return code;

    /*
     * if a fileset id wasn't provided, we'll fill it in.
     */
    if (as->parms[CREATEFT_ID_OFFSET].items == 0) {
	u_int32 low;
	if (code = NewFtId(aggrId, &low))
	    return code;
	AFS_hset64(ftId, aggrId, low);

    } else {
	if (code = ftu_ParseFsetId(as->parms[CREATEFT_ID_OFFSET].items->data,
				   &ftId)) {
	    return code;
	}
    }

    if (as->parms[CREATEFT_NAME_OFFSET].items) {
	name = as->parms[CREATEFT_NAME_OFFSET].items->data;
    } else {
	char *cp = strrchr(aggr,'/');

	if (cp) {
	    sprintf(buf,"%s-%d.%d", cp+1, AFS_HGETBOTH(ftId));
	} else {
	    sprintf(buf,"%s-%d.%d", aggr, AFS_HGETBOTH(ftId));
	}
	name = buf;
    }

    if (as->parms[CREATEFT_QUOTA_OFFSET].items)
	maxQuota = atol(as->parms[CREATEFT_QUOTA_OFFSET].items->data);

    code = ftu_AggrCreateFset(aggrId, &ftId, name, (ftu_fsetId_t*)0,
			      FTU_FSET_STATES_RW|VOL_NOEXPORT, maxQuota,
			      AGGR_CREATE_ROOT);
    if (code)
	return code;

    /*
     * Newly created filesets are left off-line; bring this one on-line
     */
    code = ftu_AggrOpenFset(aggrId, &ftId,
			    FTU_SYS_GET_STATUS|FTU_SYS_SET_STATUS,
			    VOLERR_TRANS_SETSTATUS,
			    &ftDesc);
    if (code) {
	Printf(("Fileset successfully created, but left off-line\n"));
	return code;
    }

    if (code = ftu_FsetGetStatus(ftDesc, &status)) {
	Printf(("Fileset successfully created, but left off-line\n"));
	return code;
    }

    status.vol_st.states &= ~VOL_OFFLINE;

    if (code = ftu_FsetSetStatus(ftDesc, VOL_STAT_STATES, &status)) {
	Printf(("Fileset successfully created, but left off-line\n"));
	return code;
    }

    if (code = ftu_AggrCloseFset(ftDesc)) {
	Printf(("Fileset successfully created, but may be off-line\n"));
	return code;
    }

    return 0;
}


/* SHARED */
int efts_SetStatusFt(struct cmd_syndesc *as, char *arock)
{
    long code;
    ftu_aggrId_t aggrId;
    afs_hyper_t ftId;
    char *aggr;
    char *flag;
    long maxQuota = DEFAULT_MAX_QUOTA;
    ftu_fsetDesc_t ftDesc;
    struct vol_status status;

    CommonFlags(as);

    aggr = as->parms[SETSTATUS_AGGREGATE_OFFSET].items->data;
    if (code = LookupAggregate(aggr, &aggrId))
	return code;

    ftu_ParseFsetId(as->parms[SETSTATUS_ID_OFFSET].items->data,
			&ftId);
    

    if (as->parms[SETSTATUS_TYPE_OFFSET].items)
    {
	char *type;

	type = as->parms[SETSTATUS_TYPE_OFFSET].items->data;
	code = ftu_AggrOpenFset(aggrId, &ftId,
			    FTU_SYS_GET_STATUS|FTU_SYS_SET_STATUS,
			    VOLERR_TRANS_SETSTATUS,
			    &ftDesc);
	if (code) {
	    Printf(("Fileset could not be opened\n"));
	    return code;
	}

	if (code = ftu_FsetGetStatus(ftDesc, &status)) {
	    Printf(("Failed to get status\n"));
	    return code;
	}
	if (!strcmp(type, "RO")){
	    status.vol_st.type = VOL_READONLY;
	    status.vol_st. states = VOL_READONLY | VOL_TYPE_RO;
	} else if (!strcmp(type, "BK")){
	    status.vol_st.type = VOL_READONLY;
	    status.vol_st.states = VOL_READONLY | VOL_TYPE_BK;
	} else if (!strcmp(type, "RW")){
	    status.vol_st.type = VOL_RW;
	    status.vol_st.states = VOL_RW;
	} else {
	    Printf(("Type MUST be RW, RO or BK. SetStatus failed\n"));
	}

	if (code = ftu_FsetSetStatus(ftDesc, VOL_STAT_STATES, &status)) {
	    Printf(("Failed setting status\n"));
	    ftu_AggrCloseFset(ftDesc); 
	    return code;
	}

	if (code = ftu_AggrCloseFset(ftDesc)) {
	    Printf(("Fileset wasn't closed succesfully\n"));
	    return code;
	}
     }  
        

    if (as->parms[SETSTATUS_FLAG_OFFSET].items) {
	flag = as->parms[SETSTATUS_FLAG_OFFSET].items->data;
	if (flag[0] == 'd') /* DELONSALVAGE (inconsistent bit) */
	{
	    code = ftu_AggrOpenFset(aggrId, &ftId,
				    VOL_SYS_GETSTATUS | VOL_SYS_SETSTATUS | 
				    VOL_SYS_CREATE | VOL_SYS_SETACL |
				    VOL_SYS_COPYACL | VOL_SYS_DELETE | 
				    VOL_SYS_TRUNCATE | VOL_SYS_WRITE | 
				    VOL_SYS_SETATTR | VOL_SYS_GETATTR | 
				    VOL_SYS_SCAN | VOL_SYS_SYNC | 
				    VOL_SYS_APPENDDIR,
			    VOLERR_TRANS_SETSTATUS,
			    &ftDesc);
	    if (code) {
		Printf(("Fileset could not be opened\n"));
		return code;
	    }
	    ftu_AggrAbortFsetOpen(ftDesc);
	    return 0;
	}
	code = ftu_AggrOpenFset(aggrId, &ftId,
				FTU_SYS_GET_STATUS|FTU_SYS_SET_STATUS,
				VOLERR_TRANS_SETSTATUS,
				&ftDesc);
	if (code) {
	    Printf(("Fileset could not be opened\n"));
	    return code;
	}

	if (code = ftu_FsetGetStatus(ftDesc, &status)) {
	    Printf(("Failed to get status\n"));
	    return code;
	}
	switch (flag[0]) {
	  case 'e': status.vol_st.states &= ~VOL_NOEXPORT; break; /*export*/
	  case 'n': status.vol_st.states |= VOL_NOEXPORT; break; /*noexport*/
	  case 'o': 
	    if (flag[1] == 'f')
		status.vol_st.states |= VOL_OFFLINE; /*offline*/
	    else 
		status.vol_st.states &= ~VOL_OFFLINE; /*online*/
	    break;  
	  case 'b': return 0; /* by opening it it's busy */
	  default:
	    printf("Unknown flag\n");
	    return 1;
	}

	if (code = ftu_FsetSetStatus(ftDesc, VOL_STAT_STATES, &status)) {
	    Printf(("Failed setting status\n"));
	    ftu_AggrCloseFset(ftDesc);
	    return code;
	}

	if (code = ftu_AggrCloseFset(ftDesc)) {
	    Printf(("Fileset wasn't closed succesfully\n"));
	    return code;
	}
    }
    return 0;
}
/* SHARED */
int efts_GetStatusFt(struct cmd_syndesc *as, char*arock)
{
    long code;
    ftu_aggrId_t aggrId;
    ftu_fsetId_t ftId;
    struct vol_status ftStatus;
    struct aggrInfo aggrInfo;
    long bogusSignedTime;
    afs_hyper_t hDum;
    
    
    CommonFlags(as);
    
    if (code = GET_AGGR_ID(as, GETSTATUS_AGGREGATE_OFFSET, &aggrId))
	return code;
    
    code = GetFilesetId(aggrId, as, GETSTATUS_ID_OFFSET, GETSTATUS_NAME_OFFSET,
			&ftId);
    if (code)
	return code;
    
    if (code = ftu_AggrGetFsetStatus(aggrId, &ftId, &ftStatus))
	return code;
    
    printf("_____________________________________________\n");    
    printf("%s %lu,,%lu ", ftStatus.vol_st.volName,
	   AFS_HGETBOTH(ftStatus.vol_st.volId));
    (void) DisplayFtType(&ftStatus);
    code =  ftu_AggrGetInfo(aggrId, &aggrInfo);
    if (code == 0) {
	printf(" %s", AG_TYPE_TO_STR(aggrInfo.type));
    }
    switch (ftStatus.vol_st.states & VOL_REPFIELD) {
      case VOL_REP_RELEASE:
	printf(" Release"); break;
      case VOL_REP_LAZY:
	printf(" Scheduled"); break;
      case VOL_REP_NONE:
	printf("   ");
	break;
      default:	/* oddball */ /* Jeff K. likes this */
	printf(" Repl=%#lx", ftStatus.vol_st.states & VOL_REPFIELD);
	break;
    }
    printf("  states %#lx ", ftStatus.vol_st.states);
    (void) DisplayOnLine(&ftStatus);
    printf("\n");
    printf("    Parent %lu,,%-7lu Clone %lu,,%-7lu Backup %lu,,%lu\n",
	   AFS_HGETBOTH(ftStatus.vol_st.parentId),
	   AFS_HGETBOTH(ftStatus.vol_dy.cloneId),
	   AFS_HGETBOTH(ftStatus.vol_dy.backupId));
    printf("    llBack %lu,,%-7lu llFwd %lu,,%-7lu Version %lu,,%-7lu\n",
	   AFS_HGETBOTH(ftStatus.vol_dy.llBackId),
	   AFS_HGETBOTH(ftStatus.vol_dy.llFwdId),
	   AFS_HGETBOTH(ftStatus.vol_dy.volversion));
    hDum = ftStatus.vol_dy.allocLimit;
    AFS_hrightshift(hDum, 10);	/* cvt bytes to K */
    printf("    %10lu K alloc limit", AFS_hgetlo(hDum));
    hDum = ftStatus.vol_dy.allocUsage;
    AFS_hrightshift(hDum, 10);	/* cvt bytes to K */
    printf("; %10lu K alloc usage\n", AFS_hgetlo(hDum));
    hDum = ftStatus.vol_dy.visQuotaLimit;
    AFS_hrightshift(hDum, 10);	/* cvt bytes to K */
    printf("    %10lu K quota limit", AFS_hgetlo(hDum));
    hDum = ftStatus.vol_dy.visQuotaUsage;
    AFS_hrightshift(hDum, 10);	/* cvt bytes to K */
    printf("; %10lu K quota usage", AFS_hgetlo(hDum));
    if (ftStatus.vol_dy.minQuota != 0) {
	printf(" %7d K MinQuota ", ftStatus.vol_dy.minQuota);
    }
    printf("\n");
    if (ftStatus.vol_dy.creationDate.sec != 0) {
	bogusSignedTime = (long) ftStatus.vol_dy.creationDate.sec;
	printf("    Creation %s", ctime(&bogusSignedTime));
    }
    if (ftStatus.vol_dy.updateDate.sec != 0) {
	bogusSignedTime = (long) ftStatus.vol_dy.updateDate.sec;
	if (ftStatus.vol_dy.creationDate.sec != 0
	    && ftStatus.vol_dy.creationDate.sec > ftStatus.vol_dy.updateDate.sec)
	    bogusSignedTime = (long) ftStatus.vol_dy.creationDate.sec;
	printf("    Last Update %s", ctime(&bogusSignedTime));
    }
    
    printf("\n");
    printf("_____________________________________________\n");    
    return 0;
}

/* SHARED */
/* ARGSUSED */
int efts_DeleteFt(struct cmd_syndesc *as, char *arock)
{
    ftu_aggrId_t	aggrId;
    long		code;
    ftu_fsetId_t	fsetId;

    CommonFlags(as);

    if (code = GET_AGGR_ID(as, DELETEFT_AGGREGATE_OFFSET, &aggrId))
	return code;

    code = GetFilesetId(aggrId, as, DELETEFT_ID_OFFSET, DELETEFT_NAME_OFFSET,
			&fsetId);
    if (code)
	return code;

    if (code = ftu_AggrDestroyFset(aggrId, &fsetId))
	return code;

    return 0;
}

/* SHARED */
/* ARGSUSED */
int efts_SetQuota(as, arock)
    struct cmd_syndesc *as;
    char *arock;
{
    long code;
    ftu_aggrId_t aggrId;
    ftu_fsetId_t ftId;
    struct vol_status ftStatus;

    CommonFlags(as);

    if (code = GET_AGGR_ID(as, SETQUOTA_AGGREGATE_OFFSET, &aggrId))
	return code;

    code = GetFilesetId(aggrId, as, SETQUOTA_ID_OFFSET, SETQUOTA_NAME_OFFSET,
			&ftId);
    if (code)
	return code;

    AFS_hset64(ftStatus.vol_dy.visQuotaLimit, 0,
	       atol(as->parms[SETQUOTA_QUOTA_OFFSET].items->data));
    AFS_hleftshift(ftStatus.vol_dy.visQuotaLimit, 10);	/* cvt to 1K units */

    code = ftu_AggrSetFsetStatus(aggrId, &ftId, VOL_STAT_VISLIMIT, &ftStatus);
    if (code)
	return code;

    return 0;
}

/* SHARED */
/* ARGSUSED */
int efts_Aggrinfo(struct cmd_syndesc *as, char *arock)
{
    struct aggrInfo aggrInfo;
    ftu_aggrId_t aggrId;
    long code;

    CommonFlags(as);

    if (code = GET_AGGR_ID(as, AGGRINFO_AGGREGATE_OFFSET, &aggrId))
	return code;

    if(code = ftu_AggrGetInfo(aggrId, &aggrInfo))
	return code;

    printf("information for aggregate %s (id %d)\n",aggrInfo.name,aggrId);
    printf("device: %s\n",aggrInfo.devName);
    printf("type: %s (%d)\n",AggrTypeToString(aggrInfo.type), aggrInfo.type);
    printf("total usable space: %d\n",aggrInfo.totalUsable);
    printf("free space: %d\n",aggrInfo.realFree);
    printf("minimum free space: %d\n",aggrInfo.minFree);
    return 0;
}

/* SHARED */
int efts_Dump(struct cmd_syndesc *as, char *arock)
{
    char* aggr;
    long code;
    ftu_aggrId_t aggrId;
    ftu_fsetId_t fsetId;
    ftu_dumpCriteria_t dumpCriteria;
    FILE *fileStream;
    char *fileName;
    char errStr[512];

    CommonFlags(as);

    aggr = as->parms[DUMP_AGGREGATE_OFFSET].items->data;
    if (code = GET_AGGR_ID(as, DUMP_AGGREGATE_OFFSET, &aggrId))
	return code;

    code = GetFilesetId(aggrId, as, DUMP_ID_OFFSET, DUMP_NAME_OFFSET,
			&fsetId);
    if (code)
	return code;

    bzero((char*)&dumpCriteria, sizeof dumpCriteria);

    if (as->parms[DUMP_TIME_OFFSET].items) {
	if (strcmp(as->parms[DUMP_TIME_OFFSET].items->data, "0") == 0) {
	    dumpCriteria.dc_type = FTU_DUMP_TYPE_FULL;
	} else {

	    code = ktime_DateToLong(as->parms[DUMP_TIME_OFFSET].items->data,
				    &dumpCriteria.dc_date.sec);
	    if (code) 
		return code;
	    dumpCriteria.dc_date.usec = 0;
	    dumpCriteria.dc_type = FTU_DUMP_TYPE_DATE_WITH_DIRS;	    
	}
    } else {
	if (strcmp(as->parms[DUMP_VERSION_OFFSET].items->data, "0") == 0) {
	    dumpCriteria.dc_type = FTU_DUMP_TYPE_FULL;
	} else {
	    dumpCriteria.dc_type = FTU_DUMP_TYPE_VERS_WITH_DIRS;
	    code = dfsh_StrToHyper(as->parms[DUMP_VERSION_OFFSET].items->data,
				   &dumpCriteria.dc_version, NULL);
	    if (code) 
		return ERANGE;
	}
    }
    fileName = NULL;
    if (as->parms[DUMP_FILE_OFFSET].items) {	
	fileName = as->parms[DUMP_FILE_OFFSET].items->data;
	fileStream = fopen(fileName, "w");
	if (fileStream == (FILE *)NULL) {
	    sprintf(errStr,
		    "Couldn't create file '%s' (errno %d)", fileName, errno);
	    perror(errStr);
	    exit(1);
	}
    } else {
	fileStream = stdout;
    }
    if (code = ftu_AggrDumpFset(aggrId, &fsetId, &dumpCriteria, fileStream)) {
	return code;
    }
    return 0;
}

/* SHARED */
int efts_Restore(struct cmd_syndesc *as, char *arock)
{
    char* aggr;
    long code;
    int filesetCreated = 0;
    ftu_aggrId_t aggrId;
    ftu_fsetId_t fsetId;
    char* name;
    char buf[512];
    ftu_fsetDesc_t fsetDesc;
    struct vol_status status;
    char *fileName;
    FILE *fileStream;
    char errStr[512];

    CommonFlags(as);

    aggr = as->parms[DUMP_AGGREGATE_OFFSET].items->data;
    if (code = GET_AGGR_ID(as, DUMP_AGGREGATE_OFFSET, &aggrId))
	return code;

    /*
     * if a fileset id wasn't provided, we'll fill it in.
     */
    if (as->parms[RESTORE_ID_OFFSET].items) {
	code = ftu_ParseFsetId(as->parms[RESTORE_ID_OFFSET].items->data,
			       &fsetId);
	if (code) 
	    return code;
	
	code = ftu_AggrOpenFset(aggrId, &fsetId,
			    FTU_SYS_RESTORE|FTU_SYS_GET_STATUS
			    |FTU_SYS_SET_STATUS|FTU_SYS_DESTROY,
			    VOLERR_TRANS_RESTORE,
			    &fsetDesc);
	if (code) {
	    if (code != FTU_E_NO_SUCH_FSET) {
		return code;
	    } else {
		if (!as->parms[RESTORE_NAME_OFFSET].items) {	
		    char *cp = strrchr(aggr,'/');
		    if (cp) {
			sprintf(buf,"%s-%d.%d", cp+1, AFS_HGETBOTH(fsetId));
		    } else {
			sprintf(buf,"%s-%d.%d", aggr, AFS_HGETBOTH(fsetId));
		    }
		    name = buf;
		} else {
		    name = as->parms[RESTORE_NAME_OFFSET].items->data;
		}
		code = ftu_AggrCreateFset(aggrId, &fsetId, name,
					  (ftu_fsetId_t*)0,
					  FTU_FSET_STATES_RW,
					  DEFAULT_MAX_QUOTA, 0);
		if (code) {
		    return code;
		}
		code = ftu_AggrOpenFset(aggrId, &fsetId,
					FTU_SYS_RESTORE|FTU_SYS_GET_STATUS
					|FTU_SYS_SET_STATUS|FTU_SYS_DESTROY,
					VOLERR_TRANS_RESTORE,
					&fsetDesc);
		if (code) {
		    (void) ftu_AggrDestroyFset(aggrId, &fsetId);
		    return code;
		}
		filesetCreated = 1;
	    }
	}
    } else if (as->parms[RESTORE_NAME_OFFSET].items) {
	name = as->parms[RESTORE_NAME_OFFSET].items->data;
	code = ftu_AggrLookupFset(aggrId,
				  as->parms[RESTORE_NAME_OFFSET].items->data,
				  &fsetId);	
	if (code) {
	    if (code != FTU_E_NO_SUCH_FSET) {
		return code;
	    } else {
		u_int32 low;
		if (code = NewFtId(aggrId, &low)) {
		    return code;
		}
		AFS_hset64(fsetId, aggrId, low);
		code = ftu_AggrCreateFset(aggrId, &fsetId, name,
					  (ftu_fsetId_t*)0,
					  FTU_FSET_STATES_RW,
					  DEFAULT_MAX_QUOTA, 0);
		if (code) {
		    return code;
		}
		code = ftu_AggrOpenFset(aggrId, &fsetId,
					FTU_SYS_RESTORE|FTU_SYS_GET_STATUS
					|FTU_SYS_SET_STATUS|FTU_SYS_DESTROY,
					VOLERR_TRANS_RESTORE,
					&fsetDesc);
		if (code) {
		    (void) ftu_AggrDestroyFset(aggrId, &fsetId);
		    return code;
		}
		filesetCreated = 1;
	    }
	} else {
	    code = ftu_AggrOpenFset(aggrId, &fsetId,
				    FTU_SYS_RESTORE|FTU_SYS_GET_STATUS
				    |FTU_SYS_SET_STATUS|FTU_SYS_DESTROY,
				    VOLERR_TRANS_RESTORE,
				    &fsetDesc);
	    if (code) 
		return code;
	}
    }

    fileName = NULL;
    if (as->parms[RESTORE_FILE_OFFSET].items) {	
	fileName = as->parms[RESTORE_FILE_OFFSET].items->data;
	fileStream = fopen(fileName, "r");
	if (fileStream == (FILE *)NULL) {
	    sprintf(errStr,
		    "Couldn't access file '%s' (errno %d) ", fileName, errno);
	    perror(errStr);
	    exit(1);
	}
    } else {
	fileStream = stdin;
    }

    if (code = ftu_FsetRestore(fsetDesc, fileStream, 0)) {
	if (filesetCreated) {
	    (void)ftu_FsetDestroy(fsetDesc);
	} else {
	    if (code == FTU_E_DISJOINT_VERSION ||
		code == FTU_E_DISJOINT_DATE) {
		ftu_AggrCloseFset(fsetDesc);
	    } else {
		ftu_AggrAbortFsetOpen(fsetDesc);
	    }
	}
	return code;
    }

    if (code = ftu_FsetGetStatus(fsetDesc, &status)) {
	Printf(("Fileset successfully restored, but left off-line\n"));
	return code;
    }

    status.vol_st.states &= ~VOL_OFFLINE;

    if (code = ftu_FsetSetStatus(fsetDesc, VOL_STAT_STATES, &status)) {
	Printf(("Fileset successfully restored, but left off-line\n"));
	return code;
    }

    if (code = ftu_AggrCloseFset(fsetDesc)) {
	Printf(("Fileset successfully restored, but may be off-line\n"));
	return code;
    }

    return 0;
}

/* SHARED */
/* ARGSUSED */
int efts_Clone(struct cmd_syndesc *as, char *arock)
{
    char* aggr;
    long code;
    ftu_aggrId_t aggrId;
    ftu_fsetId_t srcId;
    ftu_fsetId_t destId;
    char *name;
    char buf[512];

    CommonFlags(as);

    aggr = as->parms[CLONE_AGGREGATE_OFFSET].items->data;
    if (code = GET_AGGR_ID(as, CLONE_AGGREGATE_OFFSET, &aggrId))
	return code;

    code = GetFilesetId(aggrId,
			as, CLONE_SRC_ID_OFFSET, CLONE_SRC_NAME_OFFSET,
			&srcId);
    if (code)
	return code;

    /*
     * if a dest. fileset id wasn't provided, we'll fill it in.
     */
    if (as->parms[CLONE_DEST_ID_OFFSET].items == 0) {
	u_int32 low;
	if (code = NewFtId(aggrId, &low)) {
	    return code;
	}
	AFS_hset64(destId, aggrId, low);
    } else {
	ftu_ParseFsetId(as->parms[CLONE_DEST_ID_OFFSET].items->data,
			&destId);
    }

    if (as->parms[CLONE_DEST_NAME_OFFSET].items) {
	name = as->parms[CLONE_DEST_NAME_OFFSET].items->data;
    } else {
	char *cp = strrchr(aggr,'/');
	
	if (cp) {
	    sprintf(buf,"%s-%d.%d", cp+1, AFS_HGETBOTH(destId));
	} else {
	    sprintf(buf,"%s-%d.%d", aggr, AFS_HGETBOTH(destId));
	}
	name = buf;
    }

    code = ftu_AggrCloneFset(aggrId, &srcId, &destId, name,
			     FTU_FSET_STATES_BK);
    if (code)
	return code;

    return 0;
}

/* SHARED */
/* ARGSUSED */
int efts_Clonesys(struct cmd_syndesc *as, char *arock)
{
    fprintf(stderr, "clonesys - not yet implemented\n");
    CommonFlags(as);
    return 0;
}

/* SHARED */
/* ARGSUSED */
int efts_Reclone(struct cmd_syndesc *as, char *arock)
{
    long code;
    ftu_aggrId_t aggrId;
    ftu_fsetId_t ftId;

    CommonFlags(as);

    if (code = GET_AGGR_ID(as, RECLONE_AGGREGATE_OFFSET, &aggrId))
	return code;

    code = GetFilesetId(aggrId, as, RECLONE_ID_OFFSET, RECLONE_NAME_OFFSET,
			&ftId);
    if (code)
	return code;

    if (code = ftu_AggrRecloneFset(aggrId, &ftId))
	return code;

    return 0;
}

/* SHARED */
/* ARGSUSED */
int efts_Lsquota(struct cmd_syndesc *as, char *arock)
{
    long code;
    ftu_aggrId_t aggrId;
    ftu_fsetId_t ftId;
    afs_hyper_t hDum;
    struct vol_status ftStatus;

    CommonFlags(as);

    if (code = GET_AGGR_ID(as, LSQUOTA_AGGREGATE_OFFSET, &aggrId))
	return code;

    code = GetFilesetId(aggrId, as, LSQUOTA_ID_OFFSET, LSQUOTA_NAME_OFFSET,
			&ftId);
    if (code)
	return code;

    if (code = ftu_AggrGetFsetStatus(aggrId, &ftId, &ftStatus))
	return code;

    hDum = ftStatus.vol_dy.allocLimit;
    AFS_hrightshift(hDum, 10);	/* report in 1K units */
    (void)printf("Alloc limit=%ld, ", AFS_hgetlo(hDum));

    hDum = ftStatus.vol_dy.visQuotaLimit;
    AFS_hrightshift(hDum, 10);	/* report in 1K units */
    (void)printf("vis limit=%ld, ", AFS_hgetlo(hDum));

    hDum = ftStatus.vol_dy.allocUsage;
    AFS_hrightshift(hDum, 10);	/* report in 1K units */
    (void)printf("alloc usage=%ld, ", AFS_hgetlo(hDum));

    hDum = ftStatus.vol_dy.visQuotaUsage;
    AFS_hrightshift(hDum, 10);	/* report in 1K units */
    (void)printf("vis usage=%ld, ", AFS_hgetlo(hDum));

    hDum = ftStatus.vol_dy.visQuotaLimit;
    AFS_hsub(hDum, ftStatus.vol_dy.visQuotaUsage);
    AFS_hrightshift(hDum, 10);	/* report in 1K units */
    (void)printf("vis avail=%ld\n", AFS_hgetlo(hDum));

    return 0;
}

/* SHARED */
/* ARGSUSED */
int efts_Renameft(struct cmd_syndesc *as, char *arock)
{
    fprintf(stderr, "renameft - not yet implemented\n");
    CommonFlags(as);
    return 0;
}


main(int argc, char **argv)
{
    long code;

    osi_setlocale(LC_ALL, "");  
  
    efts_ui_SetUpSyntax();

    code = cmd_Dispatch(argc, argv);
    if (code) {
	com_err(progname, code, (char *)0);
	if ( dumpTraceLog ) {
	    fprintf(stderr, "Dumping trace log ... \n");
	    icl_DumpUserToFile(stderr);
	}
    }

    return code ? 1 : 0;
}

/*
 * This section includes routines that display fileset structures
 */
static void DisplayFtType(struct vol_status *stsp)
{
    /* Print (on stdout) some representation of the fileset type. */
    register unsigned long Tp;
    register unsigned long Sts;
    register char *cp;

    Tp = stsp->vol_st.type;
    Sts = stsp->vol_st.states;
    cp = NULL;
    switch (Sts & VOL_TYPEFIELD) {
	case VOL_TYPE_RW:
	    cp = "RW"; break;
	case VOL_TYPE_RO:
	    cp = "RO"; break;
	case VOL_TYPE_BK:
	    cp = "BK"; break;
	case VOL_TYPE_TEMP:
	    cp = "TEMP"; break;
	case 0:
	    break;	/* fall through */
	default:
	    /* Oddball: note it and leave. */
	    printf("tf=%lx", Sts & VOL_TYPEFIELD);
	    return;
    }
    if (cp == NULL) {
	switch (Tp) {
	    case VOL_RW:
		cp = "RW"; break;
	    case VOL_READONLY:
		cp = "RO/BK"; break;
	}
    }
    if (cp != NULL) {
	fputs(cp, stdout);
    } else {
	printf("type-%lu", Tp);
    }
}

static void DisplayOnLine(struct vol_status *stsp)
{
    register unsigned long states = stsp->vol_st.states;
    char *msg;

    if (states & (VOL_ZAPME | VOL_CLONEINPROG | VOL_OFFLINE | VOL_DELONSALVAGE
		   | VOL_DEADMEAT | VOL_OUTOFSERVICE
		   | VOL_MOVE_TARGET | VOL_MOVE_SOURCE)) {
	if (states & VOL_DEADMEAT) {
	    msg = " (deleted)";
	} else if (states & VOL_ZAPME) {
	    msg = " (delete now)";
	} else if (states & VOL_CLONEINPROG) {
	    msg = " (unfinished clone exists)";
	} else if (states & VOL_DELONSALVAGE) {
	    msg = " (inconsistent--being updated)";
	} else if (states & VOL_MOVE_SOURCE) {
	    msg = " (being moved from here)";
	} else if (states & VOL_MOVE_TARGET) {
	    msg = " (being moved to here)";
	} else if (states & VOL_OFFLINE) {
	    msg = "";
	} else {
	    msg = " (out of service)";
	}
	printf("**Off-line%s", msg);
	if (stsp->vol_st.accError)
	    printf(": %d", stsp->vol_st.accError);
    } else {
	printf("On-line");
    }
}
