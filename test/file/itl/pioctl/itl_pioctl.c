/*
 * @OSF_COPYRIGHT@
 *
 * (C) Copyright 1996, 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_pioctl.c -- 
 *    Implements the pioctl calls for the ITL pioctl interface.
 */

#include <utils/itl/itl_pioctl.h>


/*
 *  ITL_PIOCTL_GET_STRING()
 *    Prompts for and obtains a string.  Ensures we got a string and
 *    that it is not the empty string.
 *
 *  WARNING:
 *    Caller is responsible for freeing the string.
 *
 *  Arguments:
 *    _usrPrompt - the string to pass to itl_PromptUser()
 *    _strName - a character pointer variable to which the string will
 *               be assigned.
 *    _strLenP - pointer to an integer that will hold the length of the sting
 */

/* This is form cm.c */
#define MAX_SERVERS 1024

#define ITL_PIOCTL_GET_STRING(_usrPrompt, _strName, _strLenP)                \
        MACRO_BEGIN                                                          \
          ITL_GET_STRING((_usrPrompt), (_strName), (_strLenP));              \
          assert((_strName) && strcmp((_strName), ""));                      \
        MACRO_END

/* Following flags are defined in ioctl.h. They are exported to ITL interface */

PRIVATE itlu_valueMap_t pioctlCommands[] = {
    {"VIOCACCESS",             (long) VIOCACCESS},
    {"VIOC_AFS_DELETE_MT_PT",  (long) VIOC_AFS_DELETE_MT_PT},
    {"VIOC_AFS_STAT_MT_PT",    (long) VIOC_AFS_STAT_MT_PT},
    {"VIOC_AFS_SYSNAME",       (long) VIOC_AFS_SYSNAME},
    {"VIOC_CLOCK_MGMT",        (long) VIOC_CLOCK_MGMT},
    {"VIOCCKBACK",             (long) VIOCCKBACK},
    {"VIOCCKSERV",             (long) VIOCCKSERV},
    {"VIOC_FILE_CELL_NAME",    (long) VIOC_FILE_CELL_NAME},
    {"VIOCFLUSH",              (long) VIOCFLUSH},
    {"VIOC_FLUSHVOLUME",       (long) VIOC_FLUSHVOLUME},
    {"VIOCGETCACHEPARMS",      (long) VIOCGETCACHEPARMS},
    {"VIOCGETCELL",            (long) VIOCGETCELL},
    {"VIOCGETFID",             (long) VIOCGETFID},
    {"VIOCGETVOLSTAT",         (long) VIOCGETVOLSTAT},
    {"VIOCLISTSTORES",         (long) VIOCLISTSTORES},
    {"VIOCPREFETCH",           (long) VIOCPREFETCH},
    {"VIOCRESETSTORES",        (long) VIOCRESETSTORES},
    {"VIOCSETCACHESIZE",       (long) VIOCSETCACHESIZE},
    {"VIOCSETVOLSTAT",         (long) VIOCSETVOLSTAT},
    {"VIOCWHEREIS",            (long) VIOCWHEREIS},
    {NULL,                     0}
};

/* Those flags are used for VIOCACCESS command */
PRIVATE itlu_valueMap_t accessBits[] = {
    {"sec_acl_perm_read",   sec_acl_perm_read},
    {"sec_acl_perm_write",   sec_acl_perm_write},
    {"sec_acl_perm_execute",     sec_acl_perm_execute},
    {"sec_acl_perm_insert",   sec_acl_perm_insert},
    {"sec_acl_perm_control",   sec_acl_perm_control},
    {"sec_acl_perm_delete",   sec_acl_perm_delete},
    {NULL,                  0}
};

 /* Used for VIOCGETVOLSTAT pioctl call */
PRIVATE itlu_valueMap_t filesetStatusFlag[] = {
    {"VIOC_DEF_SGIDOK",   VIOC_DEF_SGIDOK},
    {"VIOC_DEF_DEVOK",   VIOC_DEF_DEVOK},
    {NULL,                  0}
};

/*
 *  Define command table entries supported by the functions in this file.
 */
EXPORT itl_commandTable_t itlPIOCTL_CommandTable[] = {
    ITL_PIOCTL_CMD
};

/*
 *  AddPIOCTLCommands() --
 *    Private function that really adds the PIOCTL commands.  Necessary to
 *    accomodate how pthreads does one time initialization.
 */
PRIVATE void AddPIOCTLCommands()
{
    long returnCode;

    /*
     *  Ensure the ITL utilities package is  initialized.
     */
    if (itlu_Init())
    {
	itl_Error("ITL utilities did not initialize, exiting.\n");
	exit(-1);
    }

    /*
     *  Initialize our pioctl sub-modules.
     */

    itlCommon_FidInit();
    itlCommon_SockaddrInit();
    itlCommon_TimevalInit();

    /* If we have to export some flags, export them here 
       by function call returnCode = itlu_ProcessValueMap(FlagArray, NULL) 
    */

    /*
     *  We can safely set this function is called
     *  via pthread_once.
     */
    if (returnCode = itlu_ProcessValueMap(pioctlCommands, NULL))
    {
        itl_Error("itlu_ProcessValueMap (pioctlCommands) FAILED - %ld\n",
                  returnCode);
        exit(-1);
    }
    if (returnCode = itlu_ProcessValueMap(accessBits, NULL))
    {
        itl_Error("itlu_ProcessValueMap (accessBits) FAILED - %ld\n",
                  returnCode);
        exit(-1);
    }
    if (returnCode = itlu_ProcessValueMap(filesetStatusFlag, NULL))
    {
        itl_Error("itlu_ProcessValueMap (filesetStatusFlag) FAILED - %ld\n",
                  returnCode);
        exit(-1);
    }
    (void) itl_AddModuleCmds("pioctl",
			     itlPIOCTL_CommandTable,
			     sizeof(itlPIOCTL_CommandTable)/
			     sizeof(itl_commandTable_t),
			     COMMAND_MODE_ALL,
			     "");
}

/*
 *  itlPIOCTL_AddPIOCTLCommands() -
 *    Do everything necessary to make the ITL cache manager commands 
 *    accessable via the testing interface.
 */
SHARED void itlPIOCTL_AddPIOCTLCommands()
{
    static pthread_once_t addPIOCTLOnce = pthread_once_init;

    if (pthread_once(&addPIOCTLOnce, (pthread_initroutine_t) AddPIOCTLCommands))
	itl_Error("pthread_once failed in AddPIOCTLCommands\n");
}


/* PIOCTL function calls defined from here. *
/*
 *  itlPIOCTL_Flush() --
 *    call pioctl function cm_PFlush().
 */
SHARED boolean_t itlPIOCTL_Flush(link)
    IN int link;
{
    char * fileName;
    struct afs_ioctl inoutBuffer;
    long returnCode;
    int fileNameLen;


    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    ITL_PIOCTL_GET_STRING("FileName: ", fileName, &fileNameLen);

    returnCode = pioctl( fileName, VIOCFLUSH, &inoutBuffer, link);
    ITL_PIOCTL_CHECK_ERROR(returnCode);
    ITL_FREE(fileName);
    return(FALSE);
}

/*
 *  itlPIOCTL_VolFlush() --
 *    call pioctl function cm_PFlushVolumeData().
 */
SHARED boolean_t itlPIOCTL_VolFlush(link)
    IN int link;
{
    char * fileName;
    struct afs_ioctl inoutBuffer;
    long returnCode;
    int fileNameLen;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    ITL_PIOCTL_GET_STRING("FileName: ", fileName, &fileNameLen);

    returnCode = pioctl( fileName, VIOC_FLUSHVOLUME, &inoutBuffer, link);
    ITL_FREE(fileName);
    ITL_PIOCTL_CHECK_ERROR(returnCode);
    return(FALSE);
}


/*
 *  itlPIOCTL_Prefetch() --
 *    call pioctl function cm_PBogus().
 */
SHARED boolean_t itlPIOCTL_Prefetch(link)
    IN int link;
{
    char * fileName;
    struct afs_ioctl inoutBuffer;
    long returnCode;
    int fileNameLen;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    ITL_PIOCTL_GET_STRING("FileName: ", fileName, &fileNameLen);
    returnCode = pioctl( fileName, VIOCPREFETCH, &inoutBuffer, link);
    ITL_PIOCTL_CHECK_ERROR(returnCode);
    ITL_FREE(fileName);
    return(FALSE);
}

/*
 *  itlPIOCTL_SetCacheSize() --
 *    call pioctl function cm_PBogus().
 */
SHARED boolean_t itlPIOCTL_SetCacheSize(link)
    IN int link;
{
    struct afs_ioctl inoutBuffer;
    long returnCode, cacheSize;
    int wordSize;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    itl_PromptUser("Cache Size (unit in K) : ");
    cacheSize = (long) itl_GetInteger((boolean_t *) 0);
    if ( cacheSize == 0 ) {
     	itl_Error("Cache size can not be zero\n");
    }
    inoutBuffer.in = (char *) &cacheSize;
    inoutBuffer.in_size = sizeof(long);
    returnCode = pioctl( (char *) 0 , VIOCSETCACHESIZE, &inoutBuffer, link);
    ITL_PIOCTL_CHECK_ERROR(returnCode);
    return(FALSE);
}

/*
 *  itlPIOCTL_GetCacheParms() --
 *    call pioctl function cm_PBogus().
 */
SHARED boolean_t itlPIOCTL_GetCacheParms(link)
    IN int link;
{
    struct afs_ioctl inoutBuffer;
    long returnCode;
    long outBuffer[16];
    int cachePerm, cacheUse, cacheFlag;
    char *varPermed, *varUsed, *varPlace;
    int eof,wordSize;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    ITL_PIOCTL_GET_STRING("Variable name for returned cache size permitted: ",
			  varPermed, &wordSize);

    ITL_PIOCTL_GET_STRING("Variable name for returned cache size used: ",
			  varUsed, &wordSize);

    ITL_PIOCTL_GET_STRING("Variable name for returned cache place: ",
			  varPlace, &wordSize );

    inoutBuffer.out_size = sizeof(outBuffer);
    inoutBuffer.out = (caddr_t) outBuffer;
    returnCode = pioctl( (char *) 0 , VIOCGETCACHEPARMS, &inoutBuffer, link);
    ITL_PIOCTL_CHECK_ERROR(returnCode);

    if (returnCode == 0 ) {

      cachePerm= (int)outBuffer[0];
      cacheUse = (int)outBuffer[1];
      cacheFlag = ( outBuffer[2]  << 31 ) ? 1 : 0;

      ITLU_ASSIGN_VARIABLE(varPermed, &cachePerm, itl_intType);       
      ITLU_ASSIGN_VARIABLE(varUsed, &cacheUse, itl_intType);       
      ITLU_ASSIGN_VARIABLE(varPlace, &cacheFlag, itl_intType);       

    }
    ITL_FREE(varPermed);
    ITL_FREE(varUsed);
    ITL_FREE(varPlace);
    return(FALSE);
}


SHARED boolean_t itlPIOCTL_Access(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    long returnCode, modBits;
    int wordSize;
    char * fileName;
    int fileNameLen;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    ITL_PIOCTL_GET_STRING("FileName: ", fileName, &fileNameLen);

    itl_PromptUser("ModBits ($sec_acl_perm_ flags) : ");
    modBits = (long) itl_GetInteger((boolean_t *) 0);

    wordSize = sizeof(long);
    inoutBuffer.in = (char *) &modBits;
    inoutBuffer.in_size = wordSize;
    returnCode = pioctl( fileName , VIOCACCESS, &inoutBuffer, linkFlag);

    ITL_PIOCTL_CHECK_ERROR(returnCode);
    ITL_FREE(fileName);

    return(FALSE);
}

SHARED boolean_t itlPIOCTL_DeleteMountPoint(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    long returnCode;
    char * mountPointName,*dirName;
    int fileNameLen,dirNameLen;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    ITL_PIOCTL_GET_STRING("Dir in which Mount Point exists: ",
			  dirName, &dirNameLen);
    if (dirNameLen >= AFS_NAMEMAX) {
     	itl_Error("Dir name is too long");
    }
    if( dirNameLen == 0 ) {
     	return( FALSE );
    }

    ITL_PIOCTL_GET_STRING("MountPoint Name: ", mountPointName, &fileNameLen);

    inoutBuffer.in =  mountPointName;
    inoutBuffer.in_size = fileNameLen+1;
    returnCode = pioctl( dirName, VIOC_AFS_DELETE_MT_PT, &inoutBuffer, linkFlag);
    ITL_PIOCTL_CHECK_ERROR(returnCode);
    ITL_FREE(mountPointName);
    ITL_FREE(dirName);
    return(FALSE);
}

SHARED boolean_t itlPIOCTL_StatMountPoint(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    long returnCode;
    char * mountPointName,*dirName;
    int fileNameLen,dirNameLen;
    char outBuffer[BUFSIZ+BUFSIZ+2];
    char *filesetType;
    char *cellName;
    char *filesetName;
    char ftype[2];
    char *str;
    int c = ':';

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    ITL_PIOCTL_GET_STRING("Dir in which mount point exists: ",
			  dirName, &dirNameLen);

    ITL_PIOCTL_GET_STRING("Mount point name: ", mountPointName, &fileNameLen);

    inoutBuffer.in = mountPointName;
    inoutBuffer.in_size = fileNameLen+1;
    inoutBuffer.out = (char *) outBuffer;
    inoutBuffer.out_size = sizeof(outBuffer);
    memset(outBuffer, 0, sizeof(outBuffer));

    
    ITL_PIOCTL_GET_STRING("Variable for fileset type: ",
			  filesetType, &fileNameLen);
    ITL_PIOCTL_GET_STRING("Variable for cell name): ", cellName, &fileNameLen);
    ITL_PIOCTL_GET_STRING("Variable for filesetName): ",
			  filesetName, &fileNameLen);

    returnCode = pioctl( dirName, VIOC_AFS_STAT_MT_PT, &inoutBuffer, 1);

    ITL_PIOCTL_CHECK_ERROR(returnCode);

    if(returnCode == 0 ) {
      memmove((char *)&ftype, outBuffer, sizeof(char));
      ftype[1]='\0';
      ITLU_ASSIGN_VARIABLE(filesetType, &ftype,itl_stringType);
      str = strchr( outBuffer, c );
      if ( str ) {
        *str = '\0';
        ITLU_ASSIGN_VARIABLE(cellName, (char *)&outBuffer[1],itl_stringType);
        ITLU_ASSIGN_VARIABLE(filesetName, str+1,itl_stringType);
      }
      else {
        ITLU_ASSIGN_VARIABLE(filesetName, (char *)&outBuffer[1],itl_stringType);
      }
    }

    ITL_FREE(mountPointName);
    ITL_FREE(cellName);
    ITL_FREE(filesetName);
    ITL_FREE(filesetType);
    ITL_FREE(dirName);
    return(FALSE);
}

SHARED boolean_t itlPIOCTL_Sysname(linkFlag)
    IN int linkFlag;
{
    char   outBuffer[sizeof(long) + AFS_NAMEMAX + 1];
    char   inBuffer[sizeof(long) + AFS_NAMEMAX + 1];
    struct afs_ioctl inoutBuffer;
    char *inSysName;
    int sysNameLen, eof, wordSize;
    long inCode = 0, outCode;
    long returnCode;
    int setFlag;
    char *setGetStr;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    inoutBuffer.in = (caddr_t)inBuffer;

    /* If there is "" string from user then the call will return the old 
     * sysname for @sys, otherwise it will assume that the user wants to
     * set the @sys var.
     */

    ITL_PIOCTL_GET_STRING("Set Flag ( String SET/GET): ",
                          setGetStr, &wordSize);
    setFlag = (strcmp(setGetStr, "GET")) ? 1 : 0;

    if ( setFlag == 0 ) {
      ITL_PIOCTL_GET_STRING("Variable name for returned @sys: ",
                            inSysName, &wordSize);
      memmove(inBuffer, (char *)&inCode, sizeof(long));
      inoutBuffer.in = inBuffer;
      inoutBuffer.in_size = sizeof(long);
    }
    else {
      ITL_PIOCTL_GET_STRING("New system name for @sys: ",
                            inSysName, &wordSize);
       inCode = 1;
       memmove(inBuffer, (char *)&inCode, sizeof(long));
       strcpy((char*)(inBuffer+sizeof(long)), inSysName);
       inoutBuffer.in = inBuffer;
       inoutBuffer.in_size = sizeof(long) + strlen(inSysName) + 1 ;
    }
    inoutBuffer.out = (caddr_t)outBuffer;
    inoutBuffer.out_size = sizeof(outBuffer);

    returnCode = pioctl( (char *) 0, VIOC_AFS_SYSNAME, &inoutBuffer, linkFlag);

    ITL_PIOCTL_CHECK_ERROR(returnCode);
    /* In case if we need sysname we have to export a var to the sysname. */
    if ( !returnCode && !inCode  ) {
      ITLU_ASSIGN_VARIABLE(inSysName, (char *)(outBuffer+sizeof(long)),itl_stringType);
    }
    if ( !returnCode && inCode  ) {
       /* New sysname is set and old name is not return. */
    } 
    ITL_FREE(inSysName);
    ITL_FREE(setGetStr);
    return(FALSE);
}

SHARED boolean_t itlPIOCTL_ClockMGMT(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    long returnCode;
    int eof, wordSize;
    /* There are 6 of fields with type long 
       and 2 of afsTimeval structures
       and 1 afsNetAddr.
    */
    char inBuffer[6*sizeof(long)+2*sizeof(struct afsTimeval)+sizeof(struct afsNetAddr)];
    char outBuffer[6*sizeof(long)+2*sizeof(struct afsTimeval)+sizeof(struct afsNetAddr)];
    long clockStyle, read_or_write, CM_clockset;
    long synchDistance, synchDispersion, residue;
    struct afsTimeval currTime, lastSetTime;
    struct afsNetAddr lastSetAddr;
    char *indexP;
    char *lastSetTimeVar, *lastSetAddrVar, *currTimeVar;
    char *varName;


    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    inoutBuffer.in=inBuffer;
    inoutBuffer.in_size=sizeof(inBuffer);
    inoutBuffer.out=outBuffer;
    inoutBuffer.out_size=sizeof(outBuffer);
    indexP = inBuffer;

    itl_PromptUser("Clock_style (must be 0) :");
    clockStyle = itl_GetInteger((boolean_t *) 0 );

    ITL_PIOCTL_GET_STRING("Read/Write flag (READ/WRITE):", varName,&wordSize);
    read_or_write = (strcmp(varName,"READ"))?1:0;

    ITL_FREE(varName);
    ITL_PIOCTL_GET_STRING("Set clock or not (YES/NO): ", varName,&wordSize);
    CM_clockset = (strcmp(varName,"NO"))?1:0;

    ITL_FREE(varName);

    if ( read_or_write != 0 ) {
      itl_PromptUser("SynchDistance ( integer ) : ");
      synchDistance = itl_GetInteger((boolean_t *) 0 );
      itl_PromptUser("SynchDispersion ( integer ) : ");
      synchDispersion = itl_GetInteger((boolean_t *) 0 );
    }
    memset(inBuffer, 0, sizeof(inBuffer));
    memset(outBuffer, 0, sizeof(outBuffer));
    memmove(indexP, (char *)&clockStyle, sizeof(long));
    indexP = indexP+sizeof(long);
    memmove(indexP, (char *)&read_or_write, sizeof(long));
    indexP = indexP+sizeof(long);
    memmove(indexP, (char *)&CM_clockset, sizeof(long));
    indexP = indexP+sizeof(long)+sizeof(struct afsTimeval);
    if ( read_or_write != 0 ) {
      memmove(indexP, (char *)&synchDistance, sizeof(long));
      indexP = indexP+sizeof(long);
      memmove(indexP, (char *)&synchDispersion, sizeof(long));
      indexP = indexP+sizeof(long);
    }
    indexP = indexP+sizeof(struct afsNetAddr)+sizeof(struct afsTimeval);
    
    ITL_PIOCTL_GET_STRING("Variable name for current time: ",
			  currTimeVar, &wordSize);

    ITL_PIOCTL_GET_STRING("Variable name for last set addr: ",
			  lastSetAddrVar, &wordSize);

    ITL_PIOCTL_GET_STRING("Variable name for last set time: ",
			  lastSetTimeVar, &wordSize);
    
    returnCode = pioctl( (char *) 0 , VIOC_CLOCK_MGMT, &inoutBuffer, linkFlag);
    ITL_PIOCTL_CHECK_ERROR(returnCode);
    if ( returnCode == 0 ) {
      indexP = outBuffer+3*sizeof(long);
      ITLU_ASSIGN_VARIABLE(currTimeVar,indexP,itlCommon_timevalDataType);

      indexP = indexP+sizeof(struct afsTimeval);
      ITLU_ASSIGN_VARIABLE(lastSetAddrVar,indexP,itlCommon_sockaddrDataType);

      indexP = indexP+sizeof(struct sockaddr);
      ITLU_ASSIGN_VARIABLE(lastSetTimeVar,indexP,itlCommon_timevalDataType);
    }

    ITL_FREE(lastSetTimeVar);
    ITL_FREE(lastSetAddrVar);
    ITL_FREE(currTimeVar);
    return(FALSE);
}

SHARED boolean_t itlPIOCTL_FlushFilesetNameIDMapping(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    long returnCode;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    returnCode = pioctl( (char *) 0 , VIOCCKBACK, &inoutBuffer, linkFlag);
    ITL_PIOCTL_CHECK_ERROR(returnCode);

    return(FALSE);
}

SHARED boolean_t itlPIOCTL_CheckServers(linkFlag)
    IN int linkFlag;
{
    char   inBuffer[sizeof(long) + BUFSIZ + 1];
    char   outBuffer[sizeof(long) + (MAX_SERVERS * sizeof(struct sockaddr))];
    char                  * inBufferP, *outBufferP;
    struct afs_ioctl      inoutBuffer;
    int                   eof, wordSize;
    long     checkCodes = 0x2;       /* assume that check only local cell */
    char                  * cellName, *inCellName;
    long                  errorCode, returnCode;
    struct sockaddr       zerosockaddr;
    struct sockaddr  *downServersP =(struct sockaddr *)(outBuffer+sizeof(long));
    int                   i,j,outAddrCount;
    char                  * varName;
    char                  arrayName[256];
    char                  *numServerDown;


    inBufferP = inBuffer;
    outBufferP = outBuffer;

    inCellName = (inBufferP + sizeof(long));

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    /* initialize the input data for the pioctl */

    inoutBuffer.in = (caddr_t)inBuffer;

    itl_PromptUser("Check server bits (ox0, 0x1, or 0x2) : ");
    checkCodes = (int) itl_GetInteger((boolean_t *) 0);

    memmove(inBufferP, (char *)(&checkCodes), sizeof(long));

    /* init the output data for the pioctl */
    memset((char *)outBuffer, 0, sizeof(outBuffer));
    memset((char *)&zerosockaddr, 0, sizeof(struct sockaddr));
    inoutBuffer.out = (caddr_t)outBuffer;
    inoutBuffer.out_size = sizeof(outBuffer);

    itl_PromptUser("Cell Name (or \"\" instead) : ");
    cellName = itl_PeekString(&eof, &wordSize);

    if ( strcmp( cellName,"") ) {
       cellName = itl_GetString(&eof, &wordSize);
       (void)strcpy(inCellName, cellName);
       inoutBuffer.in_size = (sizeof(long) + strlen(inCellName)+1);
    }
    else {
       cellName= itl_GetString(&eof, &wordSize);
       inoutBuffer.in_size = sizeof(long);
    }

    ITL_PIOCTL_GET_STRING("Variable name for the number of servers down: ",
			  numServerDown, &wordSize);

    ITL_PIOCTL_GET_STRING("Variable base name for returned server sockaddrs: ",
			  varName, &wordSize);

    returnCode = pioctl( (char *) 0, VIOCCKSERV, &inoutBuffer, linkFlag);
    ITL_PIOCTL_CHECK_ERROR(returnCode);
    if (returnCode == 0) {
      memmove((char *)&errorCode, outBuffer, sizeof(long));
      if (errorCode == 0) {
        if (memcmp((char *)(&(downServersP[0])), (char *)&zerosockaddr,
               sizeof(struct sockaddr)) == 0) 
        {
          ITLU_ASSIGN_VARIABLE(numServerDown, &errorCode,itl_intType);
        }
        else {
          outAddrCount = 0;
          for(i=0; i<MAX_SERVERS; i++) {
            if (memcmp((char *)(&(downServersP[0])), (char *)&zerosockaddr,
               sizeof(struct sockaddr)) != 0) 
               outAddrCount++;
          }
          ITLU_ASSIGN_VARIABLE(numServerDown, &outAddrCount,itl_intType);
          for(i=0; i<outAddrCount; i++) {
            sprintf(arrayName,"%s[%d]\0",varName,i);
            ITLU_ASSIGN_VARIABLE(arrayName,&(downServersP[0]),itlCommon_sockaddrDataType);
          }
        }
     }
    } 

    ITL_FREE(cellName);
    ITL_FREE(varName);
    ITL_FREE(numServerDown);
    return(FALSE);
}

SHARED boolean_t itlPIOCTL_GetCellName(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    long returnCode;
    char * fileName, *cellName;
    int fileNameLen;
    char  cellNameBuf[BUFSIZ+1];
    int eof, wordSize;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    ITL_PIOCTL_GET_STRING("FileName: ", fileName, &fileNameLen);

    ITL_PIOCTL_GET_STRING("Variable name for returned cell name: ",
                            cellName, &wordSize);

    inoutBuffer.out = cellNameBuf;
    inoutBuffer.out_size  = BUFSIZ+1;
    
    returnCode = pioctl( fileName, VIOC_FILE_CELL_NAME, &inoutBuffer, linkFlag);
    ITL_PIOCTL_CHECK_ERROR(returnCode);
    if ( returnCode == 0 ) {
       ITLU_ASSIGN_VARIABLE(cellName, cellNameBuf, itl_stringType);
    }

    ITL_FREE(fileName);
    ITL_FREE(cellName);
    return(FALSE);
}


SHARED boolean_t itlPIOCTL_ListCellConfig(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    long                  hostNumber = 0;
    int i;
    char                  outBuffer[osi_BUFFERSIZE];
    long                  numSockAddrs = 0;
    struct sockaddr *     hostSockaddrP;
    char *                cellNameP;
    char *cellName, arrayName[256];
    int eof, wordSize;
    char *numServers, *baseName;
    int returnCode;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);
    inoutBuffer.in = (caddr_t)(&hostNumber);
    inoutBuffer.in_size = sizeof(long);
    inoutBuffer.out = outBuffer;
    inoutBuffer.out_size = sizeof(outBuffer);
    
    itl_PromptUser("Index number to cell (begin with 0) : " );
    hostNumber = (long) itl_GetInteger((boolean_t *) 0 );

    memset(outBuffer, 0, sizeof(outBuffer));

    ITL_PIOCTL_GET_STRING("Variable for the number of servers in cell: ",
			  numServers, &wordSize);

    ITL_PIOCTL_GET_STRING("Base variable name for returned sockaddrs: ",
			  baseName, &wordSize);

    ITL_PIOCTL_GET_STRING("Variable name for returned cell name: ",
			  cellName, &wordSize);

    returnCode = pioctl( (char *) 0, VIOCGETCELL, &inoutBuffer, linkFlag);
    ITL_PIOCTL_CHECK_ERROR(returnCode);
    if (returnCode == 0) {
      memmove((char *)&numSockAddrs, outBuffer, sizeof(long));
      cellNameP =outBuffer+sizeof(long)+(numSockAddrs*sizeof(struct sockaddr));
      ITLU_ASSIGN_VARIABLE(numServers,&numSockAddrs,itl_intType);
      ITLU_ASSIGN_VARIABLE(cellName,cellNameP,itl_stringType);
      hostSockaddrP = (struct sockaddr *)(outBuffer + sizeof(long));
      for (i = 0;  i < numSockAddrs; i++) {
        sprintf(arrayName,"%s[%d]\0",baseName,i);
        ITLU_ASSIGN_VARIABLE(arrayName,&hostSockaddrP[i],itlCommon_sockaddrDataType);
      }
    }

    ITL_FREE(cellName);
    ITL_FREE(baseName);
    ITL_FREE(numServers);
    return(FALSE);
}

SHARED boolean_t itlPIOCTL_GetFid(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    long returnCode;
    int fileNameLen;
    char * fileName;
    struct afsFid * afsFidP;
    char *fidVarName;
    int eof,wordSize;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);
    ITL_UTIL_ALLOC( afsFidP, struct afsFid *, sizeof(struct afsFid));

    ITL_PIOCTL_GET_STRING("File name: ", fileName, &fileNameLen);

    ITL_PIOCTL_GET_STRING("Variable Name for returned afsFid struct : ",
			  fidVarName, &wordSize);

    inoutBuffer.out = (char *) afsFidP;
    inoutBuffer.out_size  = sizeof(struct afsFid);
    
    returnCode = pioctl( fileName, VIOCGETFID, &inoutBuffer, linkFlag);
    ITL_PIOCTL_CHECK_ERROR(returnCode);
    if ( returnCode == 0 ) {
      ITLU_ASSIGN_VARIABLE(fidVarName, afsFidP, itlCommon_fidDataType);
    } 

    ITL_FREE(afsFidP);
    ITL_FREE(fileName);
    ITL_FREE(fidVarName);
    return(FALSE);
}


SHARED boolean_t itlPIOCTL_GetVolStat(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    long returnCode;
    int fileNameLen;
    char * fileName;
    char *filesetStatusVar;
    long fstatus;
    int eof,wordSize;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    ITL_PIOCTL_GET_STRING("File name: ", fileName, &fileNameLen);

    ITL_PIOCTL_GET_STRING("Variable name for returned fileset status: ",
			  filesetStatusVar, &wordSize);

    inoutBuffer.out = (char *) &fstatus;
    inoutBuffer.out_size  = sizeof(long);
    
    returnCode = pioctl( fileName, VIOCGETVOLSTAT, &inoutBuffer, linkFlag);
    ITL_PIOCTL_CHECK_ERROR(returnCode);
    if ( returnCode == 0  ) {
      ITLU_ASSIGN_VARIABLE(filesetStatusVar, &fstatus, itl_intType);
    } 

    ITL_FREE(filesetStatusVar);
    ITL_FREE(fileName);
    return(FALSE);
}

SHARED boolean_t itlPIOCTL_ListStores(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    long returnCode;
    char *numOfFileVar, *varName;
    int eof, wordSize;
    char arrayName[256];

    /* from cm.c. Note that the maximun files is 50 ? */
    register long code;
    register long i;
    struct cm_liststores {
        long currentStores;
        long hyperCount;
        struct thyper { /* define locally to avoid picking up RPC includes */
            long high;
            long low;
        } volIDs[50];
    } tbuffer;


    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);


    inoutBuffer.out = (char *) &tbuffer;
    inoutBuffer.out_size  = sizeof(tbuffer);
    
    returnCode = pioctl( (char *) 0, VIOCLISTSTORES, &inoutBuffer, linkFlag);
    ITL_PIOCTL_CHECK_ERROR(returnCode);

    ITL_PIOCTL_GET_STRING("Variable name for number of files in the queue: ",
			  numOfFileVar, &wordSize);

    ITLU_ASSIGN_VARIABLE(numOfFileVar, &tbuffer.currentStores, itl_intType);

    ITL_PIOCTL_GET_STRING("String as base name of an array: ",
			  varName, &wordSize);

    if ( returnCode == 0 ) {
      if (tbuffer.currentStores != 0) {
        for(i = 0; i < tbuffer.hyperCount; i++) {
          sprintf(arrayName,"%s[%d]",varName,i);
          ITLU_ASSIGN_VARIABLE(arrayName,&tbuffer.volIDs[i],itlCommon_hyperDataType);
        }
      }
    }
    else {
      itl_Error("pioctl list stores failed\n");
    }

    ITL_FREE(numOfFileVar);
    ITL_FREE(varName);
    return(FALSE);
}

SHARED boolean_t itlPIOCTL_ResetStores(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    long returnCode;
    int eof,wordSize;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);
    
    returnCode = pioctl( (char *) 0, VIOCRESETSTORES, &inoutBuffer, linkFlag);
    ITL_PIOCTL_CHECK_ERROR(returnCode);

    return(FALSE);
}

SHARED boolean_t itlPIOCTL_SetVolStat(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    long returnCode;
    int fileNameLen;
    char * fileName;
    long filesetFlag;

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    ITL_PIOCTL_GET_STRING("File name: ", fileName, &fileNameLen);

    itl_PromptUser("Fileset Status bits ($VIOC_DEF_ ): ");
    filesetFlag = (long) itl_GetInteger((boolean_t *) 0);

    inoutBuffer.in = (char *) &filesetFlag;
    inoutBuffer.in_size  = sizeof(long);
    
    returnCode = pioctl( fileName, VIOCSETVOLSTAT, &inoutBuffer, linkFlag);
    ITL_PIOCTL_CHECK_ERROR(returnCode);

    ITL_FREE(fileName);
    return(FALSE);
}

SHARED boolean_t itlPIOCTL_WhereIs(linkFlag)
    IN int linkFlag;
{
    struct afs_ioctl inoutBuffer;
    char                  outBuffer[osi_BUFFERSIZE];
    int                   eof, wordSize;
    long                  returnCode;
    char                  * varName;
    char                  arrayName[256];
    char * fileName;
    struct sockaddr *sockaddrP;
    char *cellNameP;
    long numSockaddr;
    char * filesetName;
    char * numSockaddrVar;
    int i;


    sockaddrP = (struct sockaddr *)((char *)outBuffer + sizeof(long));

    PIOCTL_INIT_AFS_IOCTL(&inoutBuffer);

    /* initialize the input data for the pioctl */

    memset(outBuffer,0,  sizeof(outBuffer));
    memset(arrayName, 0, sizeof(arrayName));

    inoutBuffer.out = (caddr_t)outBuffer;
    inoutBuffer.out_size = sizeof(outBuffer);
    

    ITL_PIOCTL_GET_STRING("File name: ", fileName, &wordSize);

    ITL_PIOCTL_GET_STRING("Variable name for the num of sockaddrs: ",
                           numSockaddrVar, &wordSize);

    ITL_PIOCTL_GET_STRING("Base variable name for the sockaddrs: ",
			  varName, &wordSize);

    ITL_PIOCTL_GET_STRING("Variable name for fileset name: ",
			  filesetName, &wordSize);

    returnCode = pioctl( fileName, VIOCWHEREIS, &inoutBuffer, linkFlag);

    ITL_PIOCTL_CHECK_ERROR(returnCode);
    if (returnCode == 0) {
      memmove((char *)&numSockaddr, outBuffer, sizeof(long));
      ITLU_ASSIGN_VARIABLE(numSockaddrVar,&numSockaddr, itl_intType);

      if (numSockaddr != 0) {
        for(i=0; i<numSockaddr; i++) {
          sprintf(arrayName,"%s[%d]",varName,i);
          ITLU_ASSIGN_VARIABLE((char *)arrayName,&(sockaddrP[i]),itlCommon_sockaddrDataType);
        }
        ITLU_ASSIGN_VARIABLE(filesetName,&(sockaddrP[numSockaddr]),itl_stringType);
      }
    }
    ITL_FREE(fileName);
    ITL_FREE(varName);
    ITL_FREE(filesetName);
    ITL_FREE(numSockaddrVar);
    return(FALSE);
}


/* 
 *  itlPIOCTL_Pioctl() --
 *    call pioctl function pioctl().
 */
SHARED boolean_t itlPIOCTL_Pioctl()
{
    int command;
    int linkFlag;
    int strLen;
    char * linkFlagStr;

    itl_PromptUser("Command (Choose Command Constants (such as $VIOCACCESS) :");
    command = itl_GetInteger((boolean_t *) 0);
    ITL_PIOCTL_GET_STRING("Command (FollowLinkFlag (TRUE/FALSE) : ",
			  linkFlagStr, &strLen);
    if ( strcmp(linkFlagStr,"TRUE") == 0 ) 
      linkFlag = 1;
    else 
      linkFlag = 0;
    ITL_FREE(linkFlagStr);
    switch ( command ) {
      case VIOCACCESS :
           return (itlPIOCTL_Access(linkFlag));
      case VIOC_AFS_DELETE_MT_PT:
           return( itlPIOCTL_DeleteMountPoint(linkFlag));
      case VIOC_AFS_STAT_MT_PT:
           return( itlPIOCTL_StatMountPoint(linkFlag));
      case VIOC_AFS_SYSNAME:
           return( itlPIOCTL_Sysname(linkFlag));
      case VIOC_CLOCK_MGMT:
           return( itlPIOCTL_ClockMGMT(linkFlag));
      case VIOCCKBACK:
           return( itlPIOCTL_FlushFilesetNameIDMapping(linkFlag));
      case VIOCCKSERV:
           return( itlPIOCTL_CheckServers(linkFlag));
      case VIOC_FILE_CELL_NAME:
           return( itlPIOCTL_GetCellName(linkFlag));
      case VIOCFLUSH:
           return( itlPIOCTL_Flush(linkFlag));
      case VIOC_FLUSHVOLUME:
           return( itlPIOCTL_VolFlush(linkFlag));
      case VIOCGETCACHEPARMS:
           return( itlPIOCTL_GetCacheParms(linkFlag));
      case VIOCGETCELL:
           return( itlPIOCTL_ListCellConfig(linkFlag));
      case VIOCGETFID:
           return( itlPIOCTL_GetFid(linkFlag));
      case VIOCGETVOLSTAT:
           return( itlPIOCTL_GetVolStat(linkFlag));
      case VIOCLISTSTORES:
           return( itlPIOCTL_ListStores(linkFlag));
      case VIOCPREFETCH:
           return( itlPIOCTL_Prefetch(linkFlag));
      case VIOCRESETSTORES:
           return( itlPIOCTL_ResetStores(linkFlag));
      case VIOCSETCACHESIZE:
           return( itlPIOCTL_SetCacheSize(linkFlag));
      case VIOCSETVOLSTAT:
           return( itlPIOCTL_SetVolStat(linkFlag));
      case VIOCWHEREIS:
           return( itlPIOCTL_WhereIs(linkFlag));
      default :
           itl_Error("You input an illegal command\n");
           exit(-1);
    }
}
