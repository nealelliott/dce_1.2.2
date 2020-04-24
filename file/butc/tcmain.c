/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1991, 1995, 1996 Transarc Corporation
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: tcmain.c,v $
 * $EndLog$
 */
/*
 * tcmain.c -- tape coordinator
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */

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
#include <dcedfs/volc.h>
#include <dcedfs/budb.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/butc/RCS/tcmain.c,v 10.16 1996/10/16 11:59:39 gait Exp $")

/* The following #define is for testing purposes and should not be
 * used under normal circumstances.
 */
/*
#define allow_multiple_bakservers
*/
#define TC_DEFAULT_ADMIN_FILE_NAME "admin.butc"
#define N_SECURITY_OBJECTS 3
#define ERRCODE_RANGE 8			/* from error_table.h */

char *budbGroup = (char *)NULL;
#ifdef allow_multiple_bakservers
char *budbSuffix = (char *)NULL;
#endif /* allow_multiple_bakservers */
#define BUTC_MAXCALLS 32

extern BUTC_v4_0_epv_t BUTC_v4_0_manager_epv;
extern void BUTC_ExecuteRequest();
struct udbHandleS udbHandle;

struct bc_config *bc_globalConfig;

FILE *logIO,*ErrorlogIO;

/* GLOBAL CONFIGURATION PARAMETERS */
int dump_namecheck;
int queryoperator;
int autoQuery = 1;              /* prompt for first tape */
int isafile = 0;		/* dump to file */
int sockFlag = PIPE_NONE;	/* socket transfer protocols */
int tapemounted;
char *opencallout;
char *closecallout;

/* BUTC File Names */



int debugLevel = 0;
struct tapeConfig globalTapeConfig;	/* configuration for this tape drive */
struct deviceSyncNode *deviceLatch;	/* serializes device access */
char *globalCellName;			/* Hold cell name to which we talk to*/
char *pFile = "/var/dfs/backup/conf_";


static long SafeATOL(anum)
char *anum; {
    long total;
    int tc;

    total = 0;
    while(tc = *anum) {
	if (tc < '0' || tc > '9') return -1;
	total *= 10;
	total += (tc - '0');
	anum++;
    }
    return total;
}

/* atocl
 *	Ascii to computer long, with units. Convert a string into a long.
 *	If units are specified, the units are returned as the base 10
 *	exponent of the units, e.g. K would return 3 for units.
 *	This routine only converts unsigned values.
 * entry:
 *	numstring - text string to be converted
 *	units - ptr to a long, will contain units
 * exit:
 *	nextchar - returns ptr to first char past terminator of parse
 *	number - returned value
 *	units - power of 10
 * fn return value:
 *	0 - conversion ok
 *	-1 - error in conversion
 * notes:
 *	should deal with signed numbers. Should signal error if no digits
 *	seen.
 */

atocl(numstring, nextchar, number, units)
     char *numstring;
     char **nextchar;
     long *number;
     long *units;
{
  long total;
  long fracn; /* Numerator of fraction */
  long fracd; /* Denominator of fraction */
  long nfactor;
  int nroundup; /* Rounds up any remainder */
  int digitsfound = 0;
  char tc;

  if (units)
    *units = 0;

  total = 0;
  while ( (*numstring == ' ')  || (*numstring == '\t'))
    numstring++;

  tc = *numstring;
  while ( (tc >= '0') && (tc <= '9') )
    {
      digitsfound = 1;
      total *= 10;
      total += (tc - '0');
      numstring++;
      tc = *numstring;
    }
  if ( digitsfound == 0 )
    return(-1);

  /* check for fractional part */
  fracn=0;
  fracd=1;
  if ( tc == '.')
  {
    numstring++;
    tc = *numstring;
    while ( (tc >= '0') && (tc <= '9') )
      {
        fracn *= 10;
        fracd *= 10;
        fracn += (tc - '0');
        numstring++;
        tc = *numstring;
      }
  }

  switch ( tc )
    {
    case 'g':
    case 'G':
      *units = 9;
      numstring++;
      if ( (*numstring != ' ') && (*numstring != '\t') && (*numstring !=  '\0')) {
	return(-1);
      }

      break;

    case 'm':
    case 'M':
      *units = 6;
      numstring++;
      if ( (*numstring != ' ') && (*numstring != '\t') && (*numstring !=  '\0')) {
	return(-1);
      }

      break;

    case 'k':
    case 'K':
      *units = 3;
      numstring++;

	if ( (*numstring != ' ') && (*numstring != '\t') && (*numstring !=  '\0')) {
	  return(-1);
	};
    case '\t':
    case ' ':
    case 0:
      break;

    default:
      return(-1);
    }

  if ( fracn > 0 ) /* If fraction exists add it to the integral part and
		    * adjust the units to show the same.
		    */
  {
    /* Convert capacity to bytes for roundin up */
	
    nfactor = 1 << (*units/3*10); /* M = 1 << 20 */
	
    nroundup=0;
    if((fracn*nfactor)%fracd) nroundup=1;
    total=total*nfactor + (fracn*nfactor/fracd) + nroundup;
    *units=0;
  }

  *number = total;
  if ( tc == 0 )
    *nextchar = 0;
  else
    *nextchar = numstring;
  return(0);
}

/* LogTape
 *	checkpoint landmarks on the tapelog. Try to be minimally verbose.
 */

/* SHARED */
void LogTape(s,a,b,c,d,e,f,g,h,i,j)
{
    struct timeval tp;
    struct timezone tzp;
    struct tm *timePtr;
    char   timestring[32];

    static int day = 0;

    osi_gettimeofday(&tp,&tzp);
    timePtr = localtime((long *)&tp.tv_sec);

    /* if crossed a day boundary, or first time */
    if ( day != timePtr->tm_mday )
    {
        /* lock global mutex, ctime is not thread safe */
        pthread_lock_global_np();
        strcpy(timestring,ctime((long *)&(tp.tv_sec)));
        pthread_unlock_global_np();
	fprintf(logIO,"%s ",timestring);
	day = timePtr->tm_mday;
    }

    fprintf(logIO, "%02d:%02d:%02d: ",
	    timePtr->tm_hour, timePtr->tm_min, timePtr->tm_sec);
    fprintf(logIO, (char *)s,a,b,c,d,e,f,g,h,i,j);

    fflush(logIO);
}

/* checkpoint errors on the error log, also display it on stderr */
static void LogTapeError(s,a,b,c,d,e,f,g,h,i,j)
{
    struct timeval tp;
    struct timezone tzp;
    char   timestring[32];

    osi_gettimeofday(&tp,&tzp);

    /* lock global mutex, ctime is not thread safe */
    pthread_lock_global_np();
    strcpy(timestring,ctime((long *)&(tp.tv_sec)));
    pthread_unlock_global_np();
    timestring[strlen(timestring)-1] = '\0';

    fprintf(ErrorlogIO,"%s: ",timestring);
    fprintf(ErrorlogIO,(char *)s,a,b,c,d,e,f,g,h,i,j);
    fflush(ErrorlogIO);
}

/*
 * Log errors to the error log. msg is the format string without the trailing
 * line feed.
 */
/* SHARED */
void LogError(msg, errcode,a,b,c,d,e,f,g,h,i)
    char *msg;
    long errcode;
{
  if(debugLevel) {
    fprintf(stderr,msg,a,b,c,d,e,f,g,h,i);
    fprintf(stderr, "; %s\n", dfs_dceErrTxt(errcode));
  }
  LogTapeError(msg,a,b,c,d,e,f,g,h,i);
  LogTapeError("; %s\n", dfs_dceErrTxt(errcode));
}

/* replace last two ocurrences of / by _ */
static stringReplace(name)
char *name;
{
    char *pos;
    int i;
    char buffer[256];

    pos = (char *)strrchr(name,'/');
    *pos = '_';
    strcpy(buffer,pos);
    pos = (char *)strrchr(name,'/');
    *pos = '\0';
    strcat(name,buffer);
    return 0;


}

static stringNowReplace (logFile, deviceName)
char *logFile, *deviceName;

{
    char *pos = 0;
    char storeDevice[256];
    int mvFlag = 0;

    strcpy(storeDevice, deviceName);
    if (strncmp(deviceName, "/dev", 4) == 0) {
	deviceName += 4;
	mvFlag++;
    }
    while (pos = (char *)strchr(deviceName, '/'))
	*pos = '_';
    strcat (logFile, deviceName);
    /* now put back deviceName to the way it was */
    if (mvFlag) {
	mvFlag = 0;
	deviceName -= 4;
    }
    strcpy (deviceName, storeDevice);

    return (0);
}

/* GetDeviceConfig
 *	get the configuration information for a particular tape device
 *	as specified by the portoffset
 * entry:
 *	filename - full pathname of file containing the tape device
 *		configuration information
 *	config - for return results
 *	portOffset - for which configuration is required
 * notes:
 *	logging not available when this routine is called
 *	caller return value checks
 */

static
GetDeviceConfig(config, portOffset)
     struct tapeConfig *config;
     long portOffset;
{
#define	LINESIZE	256
    FILE *devFile;
    static char filename[500];
    char line[LINESIZE];
    char devName[LINESIZE];
    long aport;
    long capacity, units;
    long retval;
    char *ptr;
    long fmSize, fmUnits;
    long code;

    /* retval = BUTM_NOTCONFIGURED; */
    retval = 1;
    strcpy(filename, DCELOCAL_PATH);
    strcat(filename, "/var/dfs/backup/TapeConfig");
#ifdef allow_multiple_bakservers
  if (budbSuffix) {
    strcat(filename, ".");
    strcat(filename, budbSuffix);
  }
#endif /* allow_multiple_bakservers */

    devFile = fopen(filename,"r");
    if(devFile == 0)
    {
	dce_svc_printf(BAK_S_CANT_OPEN_DEVICE_MSG, filename);
	return(retval);
    }

    while(1)
    {
	if ( fgets(line,LINESIZE-1,devFile) == NULL)
		break;

#ifdef notdef
	/* break out the first field for parsing */
	ptr = index(line, ' ');
	if ( ptr == (char *) 0 )
		break;
#endif /* notdef */
	code = atocl(line, &ptr, &capacity, &units);
	if ( code )
	{
	    dce_svc_printf(BAK_S_TAPE_CAPACITY_PARSE_ERROR_MSG, line);
	    break;
	}

	code = atocl(ptr, &ptr, &fmSize, &fmUnits);
	if ( code )
	{
	    dce_svc_printf(BAK_S_ERROR_PARSING_MARK_SIZE_MSG);
	    break;
	}

	code = sscanf(ptr, "%s %u\n", devName, &aport);
	if ( code != 2 )
	{
	    dce_svc_printf(BAK_S_PARSE_FAILED_ON_MSG, ptr);
	    break;
	}

	if(aport == portOffset)
	{
	    if ( units != 0 )
	    {
		/* if any units are specified then we treat the specification
		 * of capacity as bytes. Therefore we must convert the units
		 * from bytes to kbytes
		 */
		if ( (units < 3)			/* minimum is K */
		||  ((units % 3) != 0)			/* must be K,M or G */
		   )
		{
		    dce_svc_printf(BAK_S_BAD_UNITS_VALUE_MSG, units);
		    break;
		}
		units = units - 3;
		while ( units > 0 )
		{
		    capacity = capacity*1024;
		    units -= 3;
		}
	    }
	    config->capacity = capacity;

	    /* needs fixing for string size and termination checks */
	    strncpy(config->device,devName,100);

	    /* compute specified size of file marks in bytes */
	    while ( fmUnits > 0 )
	    {
		fmSize = fmSize * 10;
		fmUnits--;
	    }
	    if ( fmSize < 0 )
	    {
		dce_svc_printf(BAK_S_INVALID_MARK_SIZE_MSG);
		retval = -1;
		break;
	    }
	    config->fileMarkSize = fmSize;

	    retval = 0;
	    break;
	}

    }
    fclose(devFile);

    return(retval);
}


/* GetConfigParams
 */
static
GetConfigParams(filename)
     char *filename;
{
    char paramFile[256];
    FILE *devFile;
    char line[LINESIZE], cmd[LINESIZE], value[LINESIZE];
    int32 retval;
    int cnt;

    /* DEFAULT SETTINGS FOR GLOBAL PARAMETERS */
    dump_namecheck = 1;                   /* check tape name on dumps */
    queryoperator  = 1;                   /* can question operator */
    opencallout    = (char *)0;           /* open  callout routine */
    closecallout   = (char *)0;           /* close callout routine */
    tapemounted    = 0;                   /* tape is not mounted */


    strcpy(paramFile, DCELOCAL_PATH);
    strcat(paramFile, filename);
    stringNowReplace(paramFile,globalTapeConfig.device);

    devFile = fopen(paramFile,"r");
    if (!devFile) return(0);

    printf("Opened file %s\n", paramFile);

    while (fgets(line,LINESIZE-1,devFile))
    {
	cnt = sscanf(line, "%s %s", cmd, value);
	if (cnt != 2)
	{
	    if (cnt > 0)
	        LogTape("Bad line in %s: %s\n", paramFile, line);
	    continue;
	}

	for (cnt=0; (size_t)cnt<strlen(cmd); cnt++)
	  if (islower(cmd[cnt])) cmd[cnt] = toupper(cmd[cnt]);

	if (!strcmp(cmd,"NAME_CHECK"))
	{
	    for (cnt=0; (size_t)cnt<strlen(value); cnt++)
	        if (islower(value[cnt])) value[cnt] = toupper(value[cnt]);

	    if (!strcmp(value,"NO"))
	    {
	        printf( "Dump tape name check is disabled\n");
	        dump_namecheck = 0;
	    }
	    else
	    {
	        printf( "Dump tape name check is enabled\n");
	        dump_namecheck = 1;
	    }
	}

	else if (!strcmp(cmd,"MOUNT"))
	{
	    opencallout = malloc(strlen(value)+1);
	    strcpy(opencallout, value);
	    printf( "Tape mount callout routine is %s\n", opencallout);
	}

	else if (!strcmp(cmd,"UNMOUNT"))
	{
	    closecallout = malloc(strlen(value)+1);
	    strcpy(closecallout, value);
	    printf( "Tape unmount callout routine is %s\n", closecallout);
	}

	else if (!strcmp(cmd,"ASK"))
	{
	    for (cnt=0; (size_t)cnt<strlen(value); cnt++)
	        if (islower(value[cnt])) value[cnt] = toupper(value[cnt]);

	    if (!strcmp(value,"NO"))
	    {
	        printf( "Operator queries are disabled\n");
		queryoperator = 0;
	    }
	    else
	    {
	        printf( "Operator queries are enabled\n");
		queryoperator = 1;
	    }
	}

	else if (!strcmp(cmd,"FILE"))
	{
	    for (cnt=0; (size_t)cnt<strlen(value); cnt++)
	        if (islower(value[cnt])) value[cnt] = toupper(value[cnt]);

	    if (!strcmp(value,"YES"))
	    {
	        printf( "Will dump to a file\n");
		isafile = 1;
	    }
	    else
	    {
	        printf( "Will not dump to a file\n");
		isafile = 0;
	    }
	}

#if !defined(OSF_NO_SOCKET_DUMP)
	else if (!strcmp(cmd,"SOCKET"))
	{
	    for (cnt=0; (size_t)cnt<strlen(value); cnt++)
	        if (islower(value[cnt])) value[cnt] = toupper(value[cnt]);

	    if (!strcmp(value,"LOCAL"))
	    {
	        printf( "Dump/Restore will use Unix domain sockets\n" );
		sockFlag = PIPE_UNIX;
	    }
	    else if (!strcmp(value,"YES"))
	    {
	        printf( "Dump/Restore will use TCP (NOT SECURE)\n" );
		sockFlag = PIPE_TCP | PIPE_UNIX;
	    }
	    else
	    {
	        printf( "Dump/Restore will use DCE pipes\n" );
		sockFlag = PIPE_NONE;
	    }
	}
#endif /* OSF_NO_SOCKET_DUMP */
    }

    fclose(devFile);
    return(retval);
}


static long
budbInitialize ()

{
    long code = 0;
    error_status_t st=0;
    char sfxBuf[200];

    if (budbGroup) {
    /* For now, lets get ubik_ClientInit to do the auth initialization. Later
       on, we may have to do the initialization much before (to catch
       unauthenticated calls) before we get to ubik_ClientInit. Then we may
       have to change the initAuth parameter */
      code = ubik_ClientInit(NULL,
			     (unsigned_char_t *)budbGroup,
			     (unsigned_char_t *)"subsys/dce/dfs-bak-servers",
			     0, 0, 1,
			     (struct ubik_client **)&udbHandle.uh_client);
      if (code)
	goto error_exit;
    }
    else {
	dfs_GetBakJunctionName ((char *)globalCellName, (char *)sfxBuf,
				(int)sizeof(sfxBuf),
				(unsigned32 *)&st);
	if ((unsigned32)st != 0) {
	 dce_svc_printf(BAK_S_CANNOT_DET_BK_JUNCTION_MSG, globalCellName, dfs_dceErrTxt(st));
	 exit(1);
        }
	budbGroup = (char *)osi_Alloc ((strlen(globalCellName)+
				    strlen(sfxBuf)+1)*
				    sizeof(char));
	if (budbGroup == (char *)NULL)
	    goto error_exit;
	sprintf(budbGroup, "%s%s", globalCellName, sfxBuf);

	code = ubik_ClientInit(NULL,
			       (unsigned_char_t *)budbGroup,
			       (unsigned_char_t *)"subsys/dce/dfs-bak-servers",
			       0, 0, 1,
			       (struct ubik_client **)&udbHandle.uh_client);
	if (code)
	    goto error_exit;
    }
    return 0;

  error_exit:
    dce_svc_printf(BAK_S_UBIK_CLIENT_INIT_FAILED_MSG);
    return (code);
}

static void initButcLog(myName)
char *myName;
{
  char logFile[500], oldLogFile[500];

  strcpy(logFile,DCELOCAL_PATH);
  strcat (logFile, "/var/dfs/backup/TL");
#ifdef allow_multiple_bakservers
  if (budbSuffix) {
    strcat(logFile, ".");
    strcat(logFile, budbSuffix);
  }
#endif /* allow_multiple_bakservers */
  stringNowReplace(logFile, globalTapeConfig.device);
  strcpy(oldLogFile, logFile);
  strcat(oldLogFile, ".old");
  rename(logFile, oldLogFile);
  logIO = fopen(logFile,"w");
  if(!logIO){
    dce_svc_printf(BAK_S_FAILED_TO_OPEN_MSG, myName, logFile);
    exit(1);
  }

  strcpy(logFile,DCELOCAL_PATH);
  strcat (logFile, "/var/dfs/backup/TE");
#ifdef allow_multiple_bakservers
  if (budbSuffix) {
    strcat(logFile, ".");
    strcat(logFile, budbSuffix);
  }
#endif /* allow_multiple_bakservers */
  stringNowReplace(logFile, globalTapeConfig.device);
  strcpy(oldLogFile, logFile);
  strcat(oldLogFile, ".old");
  rename(logFile, oldLogFile);
  ErrorlogIO = fopen(logFile,"w");
  if(!ErrorlogIO){
    dce_svc_printf(BAK_S_FAILED_TO_OPEN_MSG, myName, logFile);
    exit(1);
  }
    (void) GetConfigParams(pFile);

}

static WorkerBee(as, arock)
struct cmd_syndesc *as;
char *arock; {
    char *pid, *curptr, *bakGroup;
    long code;
    int i;
    char *whoami = "butc";
    pthread_t gc;
    pthread_attr_t threadAttr;
    unsigned_char_t *butcUuidStr;
    uuid_t butcUuid;
    static char inHostName[TC_MAXHOSTLEN+1];
    char *dceHostName, *cellName, *hosts;
    struct hostent *he;
    unsigned32 haddr;
    udbClientTextP ctPtr;
    error_status_t st1, st2;
    /*process arguments */
    long portOffset = 0;
    char *adminFileName;
    char sfxBuf[200];
    globaldef uuid_t type_uuid = {
	/* 0d7e772e-2bd2-11ca-b7ea-02608c2fed8a */
        0x0d7e772e,
        0x2bd2,
        0x11ca,
        0xb7,
        0xea,
        {0x02, 0x60, 0x2f, 0xed, 0x8a, 0x00}
      };

    debugLevel = 0;
    if (as->parms[0].items){
      portOffset = SafeATOL(as->parms[0].items->data);
      if(portOffset < 0){
        com_err(whoami, TC_BADPORTOFFSET,
		"Illegal port offset '%s'", as->parms[0].items->data);
        return (1);
      }
      if(portOffset > BC_MAXPORTOFFSET){
        com_err(whoami, TC_BADPORTOFFSET,
		"%u exceeds max port offset %u",portOffset,
		BC_MAXPORTOFFSET);
        return (1);
      }
    }
    else
      portOffset=0;

#ifdef ADMINLIST
    if (as->parms[1].items && as->parms[1].items->data){
      adminFileName = (char *)osi_Alloc((strlen(as->parms[1].items->data)+1)*sizeof(char));
      if (adminFileName == (char *)NULL) {
	com_err(whoami, TC_NOMEMORY,
		"failed to allocate admin file name string");
	return (1);
      }
      strcpy(adminFileName, as->parms[1].items->data);
    }
    else {
      adminFileName = (char *)osi_Alloc((strlen(TC_DEFAULT_ADMIN_FILE_NAME)+1)*sizeof(char));
      if (adminFileName == (char *)NULL) {
	com_err(whoami, TC_NOMEMORY,
		"failed to allocate admin file name string");
	return (1);
      }
      strcpy(adminFileName, TC_DEFAULT_ADMIN_FILE_NAME);
    }
#else
      adminFileName = (char *)osi_Alloc((strlen(TC_DEFAULT_ADMIN_FILE_NAME)+1)*sizeof(char));
      strcpy(adminFileName, TC_DEFAULT_ADMIN_FILE_NAME);
#endif

    if (as->parms[2].items) {
      debugLevel = SafeATOL(as->parms[2].items->data);
      if(debugLevel == -1){
	com_err(whoami, TC_BADDEBUGLVL, "error setting debug level to '%s'",
		as->parms[2].items->data);
	return (1);
	}
    }
    else
      debugLevel=0;

    if (as->parms[4].items) autoQuery = 0;

#ifdef allow_multiple_bakservers
    if (as->parms[5].items && as->parms[5].items->data)
      bakGroup = as->parms[5].items->data;
    else if(bakGroup = getenv("DFS_BAKSERVER_GROUP_NAME"))
      ;
    else
#endif /* allow_multiple_bakservers */
      bakGroup = (char *)NULL;
    if (bakGroup)
    {
#ifdef allow_multiple_bakservers
      dce_cf_get_cell_name(&cellName, &code);
      if (code == 0) {
	if(((strncmp(bakGroup,cellName,strlen(cellName))==0 &&
	   (curptr = (char *)&(bakGroup[strlen(cellName)]))) ||
	    (strncmp(bakGroup, "/.:", strlen("/.:"))==0 &&
	    (curptr = (char *)&(bakGroup[strlen("/.:")])))) &&
	   strncmp(curptr,"/subsys/dce/dfs/",strlen("/subsys/dce/dfs/")) == 0
	   && strchr((char *)&(curptr[strlen("/subsys/dce/dfs/")]),
		     '/') == (char *)NULL) {
#endif /* allow_multiple_bakservers */
	  budbGroup = (char *)osi_Alloc(strlen(bakGroup)+1);
	  if (budbGroup == (char *)NULL) {
	    com_err(whoami, TC_NOMEMORY,
		   "; Unable to allocate for backup server group name string");
	    return (1);
	  }
	  strcpy(budbGroup, bakGroup);
#ifdef allow_multiple_bakservers
	  dfs_GetBakJunctionName ((char *)cellName, (char *)sfxBuf,
				  (int)sizeof(sfxBuf),
				  (unsigned32 *)&st1);
	  if ((unsigned32)st1 != 0) {
	    com_err(whoami, st1,
		   "; Unable to determine bak junction name for cell %s; %s\n",
		   cellName, dfs_dceErrTxt(st1));
	    return(st1);
	  }
	  if (strcmp(curptr, sfxBuf) != 0) {
	    budbSuffix = (char *)osi_Alloc(strlen(bakGroup)+1);
	    if ( budbSuffix == (char *)NULL ) {
	      com_err(whoami, TC_NOMEMORY,
		      "; Unable to allocate for backup server group name string");
	      return(TC_NOMEMORY);
	    }
	    strcpy(budbSuffix,
		   (char *)&(curptr[strlen("/subsys/dce/dfs/")]));
	  }
	  else
	    budbSuffix = (char *)NULL;
	}
	else {
	  com_err(whoami, TC_BADGROUP,
		  "; bakserver group name %s not adhering to convention\
 /.../%s/subsys/dce/dfs/<group name>\n",
		  bakGroup, cellName);
	  return(1);
	}
      }
      else {
	com_err(whoami, TC_BADGROUP, "; unable to obtain cell name");
	return(1);
      }
#endif /* allow_multiple_bakservers */
    }

    if(GetDeviceConfig(&globalTapeConfig,portOffset)){
      com_err(whoami, TC_BADCONFIGFILE, "Unable to read TapeConfig file");
      return (1);
    }
    /* Initialize the logs */
    initButcLog(whoami);

    /*
     * First, we get our DCE hostname. This would be the name of the machine
     * that butc is running. This would not be affected by the -cell option.
     * The -cell option indicates that butc has to talk to the ftserver and
     * bakserver for the cell indicated and make authenticated RPCs to these
     * servers. The globalCellName variable holds the cell name entered on
     * the command line. The cellName variable holds the local cell name.
     */
    dce_cf_get_cell_name (&cellName, &st1);
    if (st1) {
      com_err(whoami, st1, "Unable to obtain local cell name");
      return (1);
    }
    dce_cf_get_host_name (&hosts, &st2);
    if(st2) {
      com_err(whoami, st2, "Unable to obtain local host name");
      return (1);
    }
    dceHostName = (char *)osi_Alloc((strlen(cellName)+strlen(hosts) + 2) *
				 sizeof(char));
    if ( dceHostName == (char *)NULL) {
      com_err(whoami, TC_NOMEMORY,
	      "Unable to allocate for the host name string");
      return (1);
    }
    sprintf(dceHostName, "%s/%s", cellName, hosts);
    /* Derive the full Internet style host name for this machine */
    code = gethostname(inHostName, TC_MAXHOSTLEN);
    if (!code) {
      he = gethostbyname(inHostName);
      if (he) {
	memcpy((char *)&haddr,he->h_addr, 4);
	he = gethostbyaddr((char *)&haddr, 4, AF_INET);
	if (he)
	  strncpy((char *)inHostName, he->h_name, TC_MAXHOSTLEN);
      }
    }
    if (as->parms[3].items && as->parms[3].items->data) {
      /* When we do get to handle non-local cells, we have to copy the
       * cell name to the globalCellName string. We have to allocate for
       * it before copying. -- DONE 1/7/93
       */
      globalCellName = (char *)osi_Alloc(strlen(as->parms[3].items->data)+1);
      if (globalCellName == (char *)NULL) {
	com_err(whoami, TC_NOMEMORY,
		"Unable to allocate for the cell name string");
	return 1;
      }
      strcpy(globalCellName, as->parms[3].items->data);
    }
    else {
      globalCellName = (char *)osi_Alloc(strlen(cellName)+1);
      if (globalCellName == (char *)NULL) {
	com_err(whoami, TC_NOMEMORY,
		"Unable to allocate for the cell name string");
	return 1;
      }
      strcpy(globalCellName, cellName);
    }
    (void)free(cellName);
    (void)free(hosts);

    dce_svc_printf(BAK_S_STARTUP_MSG_MSG, portOffset, debugLevel, globalCellName);
    LogTape("Starting tape coordinator: TCID %u, debug level: %u,  cell %s\n",
	    portOffset,debugLevel, globalCellName);

    /*
     * Once the -cell is implemented, make sure passing this non-local
     * cell name to this routine works.
     */
    code = vldb_InitUbikHandle(globalCellName);
    if (code) /* This one prints a message already, so just quit */
      return 1;

    initStatus();
    code = budbInitialize();
    if (code) {
      com_err(whoami, code, "failed to initialize ubik client");
      return 1;
    }
    code = ubik_Call(BUDB_GetInstanceId,  udbHandle.uh_client, UF_SYNC_SITE,
                     &udbHandle.uh_instanceId);
    if ( code )
    {
        com_err(whoami, code, "can't establish instance Id");
	return(1);
    }

    /* initialize the client text structures */
    bc_globalConfig=(struct bc_config *)osi_Alloc(sizeof(struct bc_config));
    if (bc_globalConfig == (struct bc_config *)NULL) {
	com_err(whoami, TC_NOMEMORY,
		"Unable to allocate for the global configuration structures");
	return 1;
    }
    bzero((char *)bc_globalConfig, sizeof(struct bc_config));
    ctPtr = &bc_globalConfig->configText[0];

    for ( i = 0; i < TB_NUM; i++ )
    {
	bzero((char *)ctPtr, sizeof(*ctPtr));
	ctPtr->textType = i;
	ctPtr++;
    }

    code = bc_GetHostText();
    if (code)
      return 1;

    code = bc_getUuidFromBinding (dceHostName, inHostName, portOffset,
				  &butcUuidStr);
    if (code != 0 || !butcUuidStr)
    {
	com_err(whoami, code,
		"This combination of  TCID %d and host (%s or %s) not in backup database\n",
		portOffset, dceHostName, inHostName);
	osi_Free(dceHostName, strlen(dceHostName)+1);
	return (1);
    }

#ifdef DEBUG
    dce_svc_printf(BAK_S_UUID_BEFORE_CONVER_MSG, butcUuidStr);
#endif

    uuid_from_string(butcUuidStr, &butcUuid, (unsigned32 *)&code);
    if (code)
    {
	com_err(whoami, code, "failed to obtain tape coordinator UUID");
	osi_Free(dceHostName, strlen(dceHostName)+1);
	return(1);
    }
#ifdef DEBUG
    print_uuid(butcUuid);
#endif

    /*
     * Check to see if butc is already running on the same tcid
     */
    if (compat_DetectDuplicateServer((rpc_if_handle_t)BUTC_v4_0_s_ifspec,
				     &butcUuid,
				     (rpc_binding_handle_t *)NULL,
				     &st1) && st1 == 0) {
      /* detected a duplicate, have to quit */
      com_err(whoami, TC_DUPLICATE,
	      "Unable to startup tape coordinator on tcid %d",
	      portOffset);
      osi_Free(dceHostName, strlen(dceHostName)+1);
      return (1);
    }

    dfs_register_rpc_server((rpc_if_handle_t)BUTC_v4_0_s_ifspec,
			    (rpc_mgr_epv_t)&BUTC_v4_0_manager_epv,
			    &butcUuid,
			    &type_uuid,
			    BUTC_MAXCALLS,
			    adminFileName,
			    "DFS Butc",
			    (error_status_t *)&code);
    if (code)
    {
	com_err(whoami, code,
		"failed to register tape coordinator interface with runtime");
	osi_Free(dceHostName, strlen(dceHostName)+1);
	return(1);
    }

    /*initialize the dumpNode list */
    InitNodeList();

    deviceLatch = (struct deviceSyncNode *)osi_Alloc(sizeof(struct deviceSyncNode));
    if (deviceLatch == (struct deviceSyncNode *)NULL) {
      com_err(whoami, TC_NOMEMORY,
	      "failed to allocate for the tape device serializer structure");
	osi_Free(dceHostName, strlen(dceHostName)+1);
	return(1);
    }
    bzero((char *)deviceLatch, sizeof(struct deviceSyncNode));
    lock_Init(&(deviceLatch->deviceLock));
    deviceLatch->flags = 0;
    /* create & init attr values needed by the threads */
    if (pthread_attr_create(&threadAttr) != 0)
      {
    com_err(whoami, TC_THREAD,
      "failed to create attribute for the tape coordinator background thread");
    osi_Free(dceHostName, strlen(dceHostName)+1);
    return(1);
  }
    if (pthread_attr_setstacksize(&threadAttr, TC_DEFAULT_STACK_SIZE) != 0)  {
      com_err(whoami, TC_THREAD,
         "failed to set stacksize for the tape coordinator background thread");
	osi_Free(dceHostName, strlen(dceHostName)+1);
	return (1);
    }

   /* initialize database support
     initialize the volume support
     initialize logs */

     /* Create a single security object, in this case the null security object, for unauthenticated connections, which will be used to control security on connections made to this server */

    osi_Free(dceHostName, strlen(dceHostName)+1);
    rpc_server_listen(BUTC_MAXCALLS, (unsigned32 *)&code);
    Quit("StartServer returned?");
}

DCE_SVC_DEFINE_HANDLE(bak_svc_handle, bak_svc_table, "bak")

void initialize_svc()
{
  error_status_t st;

  bak_svc_handle = dce_svc_register(bak_svc_table,  (idl_char *)"bak",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dce_msg_define_msg_table(bak__table,
        sizeof bak__table / sizeof bak__table[0], &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Warning Cannot register bak incore message table 0x%lx\
n", st);
}

main(argc, argv)
int argc;
char **argv; {
    struct cmd_syndesc *ts;
    struct cmd_item *ti;
    char *nsgroup;
    char sfxBuf[200];
    error_status_t st = 0;
    unsigned char *cmd_explanation;

    osi_setlocale(LC_ALL, "");
    initialize_svc();

  cmd_explanation = dce_msg_get_msg(bak_s_tape_coordinator, &st);
    ts=cmd_CreateSyntax((char *) 0, WorkerBee,(char *) 1,
			(char *) cmd_explanation);
  free(cmd_explanation);

  cmd_explanation = dce_msg_get_msg(bak_s_tc_number_butc, &st);
    cmd_AddParm(ts, "-tcid", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation);
  free(cmd_explanation);

#ifdef ADMINLIST
  cmd_explanation = dce_msg_get_msg(bak_s_filename_butc, &st);
    cmd_AddParm(ts, "-adminlist", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation);
  free(cmd_explanation);
#else
    cmd_Seek(ts, 2);
#endif

  cmd_explanation = dce_msg_get_msg(bak_s_trace_level_butc, &st);
    cmd_AddParm(ts, "-debuglevel", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation);
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(bak_s_cellname_butc, &st);
    cmd_AddParm(ts, "-cell", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation);
  free(cmd_explanation);

    cmd_AddParm(ts, "-noautoquery", CMD_FLAG, CMD_OPTIONAL,
		    "do not query operator for first tape");

#ifdef allow_multiple_bakservers

  cmd_explanation = dce_msg_get_msg(bak_s_server_group, &st);
    cmd_AddParm(ts, "-bakgroup", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation);
  free(cmd_explanation);

#endif /* allow_multiple_bakservers */

    dfs_installInterfaceDescription ((rpc_if_handle_t)BUTC_v4_0_s_ifspec,
				      (rpc_if_handle_t)BUTC_v4_0_s_ifspec,
				     (unsigned32)1, /* provider_version number */
				     (unsigned_char_t *)"Transarc tape coordinator Interface",
				     (error_status_t *)&st);
    if (st != (error_status_t)0) {
	LogError("Can't install interface description: %lu %s\n",
		 st, dfs_dceErrTxt(st));
	dce_svc_printf(BAK_S_CANT_INSTALL_INTERFACE_DESC_MSG,
		       dfs_dceErrTxt(st));
	exit(1);
    }

    /*
     * as a workaround to the DG private sockets/pipe argument problem
     * (transarc DB 6731), we disallow private DG sockets.  Do it here,
     * after we know the DCE RPC is initialized, as the last thing we do
     * before starting real work.
     */
#ifndef AFS_AIX_ENV
    rpc__dg_disa_priv_sockets();
#endif /* AFS_AIX_ENV */

    /* special case "no args" case since cmd_dispatch gives help message
     * instead
     * Just set all items to NULL since WorkerBee will
     * fill in the proper defaults
     */
    if (argc == 1) {
	ts = (struct cmd_syndesc *)osi_Alloc(sizeof(struct cmd_syndesc));
	bzero((char *)ts, sizeof(*ts));
	ts->parms[0].items = (struct cmd_item *)NULL;
	ts->parms[1].items = (struct cmd_item *)NULL;
	ts->parms[2].items = (struct cmd_item *)NULL;
	ts->parms[3].items = (struct cmd_item *)NULL;
	ts->parms[4].items = (struct cmd_item *)NULL;
	return WorkerBee(ts, (char *) 0);
    }
    else
	return cmd_Dispatch(argc, argv);
}

Quit(msg, a, b)
    char *msg;
{
    fprintf(stderr, msg, a, b);
    exit(1);
}

bc_SafeATOI(anum)
char *anum;
{
    long total;
    int tc;

    total = 0;
    while(tc = *anum) {
	if (!strcmp(anum, "g") || !strcmp(anum, "G")) {
	    total *= 1024*1024;
	    break;
	}
	if (!strcmp(anum, "m") || !strcmp(anum, "M")) {
	    total *= 1024;
	    break;
	}
	if (tc < '0' || tc > '9') return -1;
	total *= 10;
	total += (tc - '0');
	anum++;
    }
    return total;
}
