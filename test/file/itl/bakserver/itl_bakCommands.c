/*
 * @OSF_COPYRIGHT@
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1993
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
/*
 * HISTORY
 * $Log: $
 *
 * $EndLog$
 *
 * itl_bakCommands.c --
 *    Contains all the C code to implement the ITL commands for testing the
 *    backup server.
 *
 *    This program invokes the backup server RPC's via the ubik library
 *    interface.  Thus it establishes a connection to the server via the
 *    ubik_ClientInit() call and issues RPC's via the ubik_Call() function.
 *    This is the expected use in an actual application and thus the reason
 *    it was done this way, rather than issue the RPC's directly to the
 *    backup server.
 */

#include <assert.h>
#include <stdio.h>     /* for dce_cf.h */
#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include <dcedfs/budb.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_commonSecurity.h>
#include <utils/itl/itl_bakCommon.h>
#include <utils/itl/itl_bakCommands.h>
#include <utils/itl/itl_bakVolumeEntry.h>
#include <utils/itl/itl_bakTapeEntry.h>
#include <utils/itl/itl_bakTapeSet.h>
#include <utils/itl/itl_bakDumpEntry.h>
#include <utils/itl/itl_bakPrincipal.h>
#include <utils/itl/itl_bakListType.h>
#include <utils/itl/itl_bakCharList.h>
#include <utils/itl/itl_bakDBHeader.h>

#define ITL_BAK_MAX_CELL_NAME 256
#define ITL_BAK_MAX_GROUP_NAME (2 * ITL_BAK_MAX_CELL_NAME)

PRIVATE char cellName[ITL_BAK_MAX_CELL_NAME];
PRIVATE char bakNameServerGroup[ITL_BAK_MAX_GROUP_NAME];

PRIVATE char itlBakInfoMsg[] = 
"The ITL BAK module provides command interfaces to the DFS bakserver.";


#define BUDB_IFSPEC (rpc_if_handle_t) BUDB_v4_0_c_ifspec
#define ITL_BAK_SERVER_GROUP  "subsys/dce/dfs-bak-servers"

/*
 *  UBIK_CALL_FLAGS --
 *    A constant of zero, since ubik_Call does not appear to use this
 *    argument.  This is here to make the code more readable.
 */
#define UBIK_CALL_FLAGS 0


/*
 *  ITL_BAK_GET_STRING()
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
#define ITL_BAK_GET_STRING(_usrPrompt, _strName, _strLenP)                   \
        MACRO_BEGIN                                                          \
          ITL_GET_STRING((_usrPrompt), (_strName), (_strLenP));              \
          assert((_strName) && strcmp((_strName), ""));                      \
        MACRO_END

/*
 *  GET_SERVER_HANDLE() --
 *    Simple macro to prompt for and get a server handle. (A ubik client
 *    info structure pointer for ubik_Call) These are pointers
 *    but are stored as ITL integers.
 *
 *  Arguments:
 *    _serverVar - variable that is assigned the server handle.
 */
#define GET_SERVER_HANDLE(_serverVar)                                      \
        itl_PromptUser("Backup Server Handle: ");                          \
        (_serverVar) = (struct ubik_client *) itl_GetInteger((boolean_t *) 0)

/*
 *  GET_DUMP_ID() --
 *    Simple macro to prompt for and get a dump identifier.
 *
 *  Arguments:
 *    _dmpId - variable that is assigned the dump id.
 */
#define GET_DUMP_ID(_dmpId)                                                \
        itl_PromptUser("Dump Id: ");                                       \
        (_dmpId) = (dumpId) itl_GetInteger((boolean_t *) 0);



/*
 *  Define command table entries supported by the functions in this file.
 */
PRIVATE itl_commandTable_t itlBak_commandTable[] = {
    ITL_BAK_LOGIN_CMD,
    ITL_BAK_SERVER_CONNECT_CMD,
    ITL_BAK_ADD_VOLUME_CMD,
    ITL_BAK_CREATE_DUMP_CMD,
    ITL_BAK_DELETE_DUMP_CMD,
    ITL_BAK_DELETE_TAPE_CMD,
    ITL_BAK_DELETE_VDP_CMD,
    ITL_BAK_FIND_CLONE_CMD,
    ITL_BAK_FIND_DUMP_CMD,
    ITL_BAK_FIND_LATEST_DUMP_CMD,
    ITL_BAK_FINISH_DUMP_CMD,
    ITL_BAK_FINISH_TAPE_CMD,
    ITL_BAK_GET_DUMPS_CMD,
    ITL_BAK_GET_TAPES_CMD,
    ITL_BAK_GET_VOLUMES_CMD,
    ITL_BAK_USE_TAPE_CMD,
    ITL_BAK_GET_TEXT_CMD,
    ITL_BAK_GET_TEXT_VERSION_CMD,
    ITL_BAK_SAVE_TEXT_CMD,
    ITL_BAK_FREE_ALL_LOCKS_CMD,
    ITL_BAK_FREE_LOCK_CMD,
    ITL_BAK_GET_INSTANCE_ID_CMD,
    ITL_BAK_GET_LOCK_CMD,
    ITL_BAK_DB_VERIFY_CMD,
    ITL_BAK_DUMP_DB_CMD,
    ITL_BAK_RESTORE_DB_HEADER_CMD
};

PRIVATE itlu_valueMap_t textBlockTypes[] = {
    {"TB_DUMPSCHEDULE", TB_DUMPSCHEDULE},
    {"TB_VOLUMESET",    TB_VOLUMESET},
    {"TB_TAPEHOSTS",    TB_TAPEHOSTS},
    {NULL,              0}
};

PRIVATE itlu_valueMap_t dumpFlags[] = {
    {"BUDB_DUMP_INCOMPLETE",  BUDB_DUMP_INCOMPLETE},
    {"BUDB_DUMP_TAPEERROR",   BUDB_DUMP_TAPEERROR},
    {"BUDB_DUMP_INPROGRESS",  BUDB_DUMP_INPROGRESS},
    {"BUDB_DUMP_ABORTED",     BUDB_DUMP_ABORTED},
    {NULL,                    0}
};

PRIVATE itlu_valueMap_t tapeFlags[] = {
    {"BUDB_TAPE_TAPEERROR",    BUDB_TAPE_TAPEERROR},
    {"BUDB_TAPE_DELETED",      BUDB_TAPE_DELETED},
    {"BUDB_TAPE_BEINGWRITTEN", BUDB_TAPE_BEINGWRITTEN},
    {"BUDB_TAPE_ABORTED",      BUDB_TAPE_ABORTED},
    {"BUDB_TAPE_STAGED",       BUDB_TAPE_STAGED},
    {"BUDB_TAPE_WRITTEN",      BUDB_TAPE_WRITTEN},
    {NULL,                     0}
};

PRIVATE itlu_valueMap_t volumeFlags[] = {
    {"BUDB_VOL_TAPEERROR",    BUDB_VOL_TAPEERROR},
    {"BUDB_VOL_FILEERROR",    BUDB_VOL_FILEERROR},
    {"BUDB_VOL_BEINGWRITTEN", BUDB_VOL_BEINGWRITTEN},
    {"BUDB_VOL_FIRSTFRAG",    BUDB_VOL_FIRSTFRAG},
    {"BUDB_VOL_LASTFRAG",     BUDB_VOL_LASTFRAG},
    {"BUDB_VOL_ABORTED",      BUDB_VOL_ABORTED},
    {NULL,                    0}
};

PRIVATE itlu_valueMap_t searchFlags[] = {
    {"BUDB_OP_NAMES",      BUDB_OP_NAMES},
    {"BUDB_OP_STARTS",     BUDB_OP_STARTS},
    {"BUDB_OP_ENDS",       BUDB_OP_ENDS},
    {"BUDB_OP_TIMES",      BUDB_OP_TIMES},
    {"BUDB_OP_MISC",       BUDB_OP_MISC},
    {"BUDB_OP_DUMPNAME",   BUDB_OP_DUMPNAME},
    {"BUDB_OP_VOLUMENAME", BUDB_OP_VOLUMENAME},
    {"BUDB_OP_TAPENAME",   BUDB_OP_TAPENAME},
    {"BUDB_OP_STARTTIME",  BUDB_OP_STARTTIME},
    {"BUDB_OP_RANGE",      BUDB_OP_RANGE},
    {"BUDB_OP_NPREVIOUS",  BUDB_OP_NPREVIOUS},
    {"BUDB_OP_NFOLLOWING", BUDB_OP_NFOLLOWING},
    {"BUDB_OP_DUMPID",     BUDB_OP_DUMPID},
    {"BUDB_OP_CLONETIME",  BUDB_OP_CLONETIME},
    {"BUDB_OP_DUMPTIME",   BUDB_OP_DUMPTIME},
    {"BUDB_OP_INCTIME",    BUDB_OP_INCTIME},
    {"BUDB_OP_FIRSTFRAG",  BUDB_OP_FIRSTFRAG},
    {NULL,                 0}
};

PRIVATE itlu_valueMap_t structureTypes[] = {
    {"SD_DBHEADER",          SD_DBHEADER},
    {"SD_DUMP",              SD_DUMP},
    {"SD_TAPE",              SD_TAPE},
    {"SD_VOLUME",            SD_VOLUME},
    {"SD_TEXT_DUMPSCHEDULE", SD_TEXT_DUMPSCHEDULE},
    {"SD_TEXT_VOLUMESET",    SD_TEXT_VOLUMESET},
    {"SD_TEXT_TAPEHOSTS",    SD_TEXT_TAPEHOSTS},
    {"SD_END",               SD_END},
    {NULL,                   0}
};

/*
 *  AddBakCommands() --
 *    Private function that really adds the bak commands.  Necessary to
 *    accomodate how pthreads does one time initialization.
 */
PRIVATE void AddBakCommands()
{
    long returnCode = 0;
    unsigned long errorCode = 0;
    char *cellNameP = NULL, junctionName[ITL_BAK_MAX_CELL_NAME];

    /*
     *  Get the DCE cell name.  This is needed for ubik_ClientInit calls.
     */
    dce_cf_get_cell_name(&cellNameP, &errorCode);

    if ((errorCode == dce_cf_st_ok) &&
	(cellNameP != NULL)         &&
	(*cellNameP == '/')         &&
	((int) strlen(cellNameP) < ITL_BAK_MAX_CELL_NAME))
    {
	strcpy(cellName, cellNameP);
	free(cellNameP);
	dfs_GetBakJunctionName(cellName, junctionName,
			       (ITL_BAK_MAX_CELL_NAME - strlen(cellName)),
			       (unsigned32 *) &errorCode);
	if (!errorCode)
	{
	    /*
	     *  Create the name service group used for the ubik_ClientInit
	     *  calls.
	     */
	    assert(((int) strlen(cellName) + (int) strlen(junctionName)) < 
		   ITL_BAK_MAX_GROUP_NAME);
	    sprintf(bakNameServerGroup, "%s%s", cellName, junctionName);

	    /*
	     *  Ensure all name tables are initialized.
	     */
	    if (itlu_Init())
	    {
		itl_Error("ITL utilities did not initialize, exiting.\n");
		exit(-1);
	    }

	    /*
	     *  Initialize all other sub-modules.
	     */
	    itlCommon_HyperInit();
	    itlCommon_SecurityInit();
	    itlBak_CommonInit();
	    itlBak_DBHeaderInit();
	    itlBak_DumpEntryInit();
	    itlBak_PrincipalInit();
	    itlBak_TapeEntryInit();
	    itlBak_TapeSetInit();
	    itlBak_VolumeEntryInit();
	    itlBak_ListInit();
	    itlBak_CharListInit();

	    /*
	     *  Turn all of the flags for various bak operations into
	     *  variables.  Yeah, the code doesn't stop at the first failure
	     *  but this thing just should not fail.  If it does, well we
	     *  wouldn't want the debugger writers to have nothing to do.
	     */
	    returnCode = itlu_ProcessValueMap(textBlockTypes, NULL);
	    returnCode = itlu_ProcessValueMap(dumpFlags, NULL);
	    returnCode = itlu_ProcessValueMap(tapeFlags, NULL);
	    returnCode = itlu_ProcessValueMap(volumeFlags, NULL);
	    returnCode = itlu_ProcessValueMap(searchFlags, NULL);
	    returnCode = itlu_ProcessValueMap(structureTypes, NULL);
	    if(returnCode)
	    {
		itl_Error("itlu_ProcessValueMap FAILED - %ld\n", returnCode);
		exit(-1);
	    }

	    (void) itl_AddModuleCmds("bak",
				     itlBak_commandTable,
				     sizeof(itlBak_commandTable) / 
				     sizeof(itl_commandTable_t),
				     COMMAND_MODE_ALL,
				     itlBakInfoMsg);
	}
	else
	    itlu_DCEError(errorCode, "dfs_GetBakJunctionName error");
    }
    else
    {
	itl_Error("Cannot find cell name (%s), or name is invalid\n", 
		  cellNameP);
	itlu_DCEError(errorCode, "DCE error");
	exit(-1);
    }
}

/*
 *  itlBak_AddBakCommands() -
 *    Do everything necessary to make the bak commands accessable via 
 *    the testing interface.
 */
EXPORT void itlBak_AddBakCommands()
{
    static pthread_once_t addBakOnce = pthread_once_init;

    if (pthread_once(&addBakOnce, (pthread_initroutine_t) AddBakCommands))
	itl_Error("pthread_once failed in AddBakCommands\n");
}

/*
 *  itlBak_DceLogin() --
 *    This function establishes a DCE login context.
 */
SHARED boolean_t itlBak_DceLogin()
{
    char *userName, *userPassword;
    int strLength;

    ITL_BAK_GET_STRING("User Name: ", userName, &strLength);
    ITL_BAK_GET_STRING("User Password: ", userPassword, &strLength);

    if (!itlCommon_SecuritySetContext(userName, userPassword))
	itl_Error("Could NOT create login for, %s\n", userName);

    ITL_FREE(userPassword);
    ITL_FREE(userName);

    return(FALSE);
}

/*
 *  itlBak_ServerConnect() --
 *    Establishes a connection to a backup server via the ubik interface.
 */
SHARED boolean_t itlBak_ServerConnect()
{
    int status, strLength;
    int noAuth, localAuth, initAuth;
    char *authMode, *handleVar;
    struct ubik_client *callHandle;

    noAuth = localAuth = initAuth = 0;
    ITL_BAK_GET_STRING("Authentication Mode (init, local, none): ", 
		       authMode, &strLength);
    /*
     *  Verify we have a valid authentication mode.
     */
    if (!strcmp(authMode, "init"))
	initAuth = 1;
    else
    {
	if (!strcmp(authMode, "local"))
	    localAuth = 1;
	else
	{
	    if (!strcmp(authMode, "none"))
		noAuth = 1;
	    else
		itl_Error("Authentication mode, %s, is invalid.\n");
	}
    }

    if (initAuth || localAuth || noAuth)
    {
	ITL_BAK_GET_STRING("Handle Name: ", handleVar, &strLength);

	status = ubik_ClientInit(BUDB_IFSPEC, bakNameServerGroup,
				 ITL_BAK_SERVER_GROUP, noAuth, localAuth,
				 initAuth, &callHandle);
	ITL_BAK_CHECK_ERROR(status);
	if (status == 0)
	    ITLU_ASSIGN_VARIABLE(handleVar, &callHandle, itl_intType);

	ITL_FREE(handleVar);
    }

    ITL_FREE(authMode);

    return(FALSE);
}

/*
 *  itlBak_AddVolume
 *    Issues the BUDB_AddVolume RPC.
 */
SHARED boolean_t itlBak_AddVolume()
{
    int status;
    budb_volumeEntry volumeEntry;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    itlBak_VolumeEntryGet(&volumeEntry);
    status = ubik_Call(BUDB_AddVolume,  callHandle, UBIK_CALL_FLAGS,
		       &volumeEntry);
    ITL_BAK_CHECK_ERROR(status);

    return(FALSE);
}

/*
 *  itlBak_CreateDump
 *    Issues the BUDB_CreateDump RPC.
 */
SHARED boolean_t itlBak_CreateDump()
{
    int status;
    budb_dumpEntry dumpEntry;
    struct ubik_client *callHandle;
    char *dumpEntryVar;

    GET_SERVER_HANDLE(callHandle);
    if (itlBak_DumpEntryGet(&dumpEntryVar, &dumpEntry))
    {
	status = ubik_Call(BUDB_CreateDump,  callHandle, UBIK_CALL_FLAGS,
			   &dumpEntry);
	ITL_BAK_CHECK_ERROR(status);

	if (status == 0)
	    ITLU_ASSIGN_VARIABLE(dumpEntryVar, &dumpEntry, 
				 itlBak_dumpEntryDataType);

	ITL_FREE(dumpEntryVar);
    }
    else
	itl_Error("Invalid fs_bakDumpEntry variable name entered.  The variable must already exist.\n");

    return(FALSE);
}

/*
 *  itlBak_DeleteDump
 *    Issues the BUDB_DeleteDump RPC.
 */
SHARED boolean_t itlBak_DeleteDump()
{
    int status;
    dumpId dumpIdentifier;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    GET_DUMP_ID(dumpIdentifier);
    status = ubik_Call(BUDB_DeleteDump,  callHandle, UBIK_CALL_FLAGS,
		       dumpIdentifier);
    ITL_BAK_CHECK_ERROR(status);

    return(FALSE);
}

/*
 *  itlBak_DeleteTape
 *    Issues the BUDB_DeleteTape RPC.
 */
SHARED boolean_t itlBak_DeleteTape()
{
    int status;
    budb_tapeEntry tapeEntry;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    itlBak_TapeEntryGet(&tapeEntry);
    status = ubik_Call(BUDB_DeleteTape,  callHandle, UBIK_CALL_FLAGS,
		       &tapeEntry);
    ITL_BAK_CHECK_ERROR(status);

    return(FALSE);
}

/*
 *  itlBak_DeleteVDP
 *    Issues the BUDB_DeleteVDP RPC.
 */
SHARED boolean_t itlBak_DeleteVDP()
{
    int status, strLength;
    char *dumpName, *dumpPathName;
    struct ubik_client *callHandle;
    dumpId dumpIdentifier;

    GET_SERVER_HANDLE(callHandle);
    ITL_BAK_GET_STRING("Dump Name: ", dumpName,  &strLength);
    ITL_BAK_GET_STRING("Dump Path Name: ", dumpPathName,  &strLength);
    GET_DUMP_ID(dumpIdentifier);
    status = ubik_Call(BUDB_DeleteVDP,  callHandle, UBIK_CALL_FLAGS,
		       dumpName, dumpPathName, dumpIdentifier);
    ITL_BAK_CHECK_ERROR(status);

    ITL_FREE(dumpName);
    ITL_FREE(dumpPathName);

    return(FALSE);
}

/*
 *  itlBak_FindClone
 *    Issues the BUDB_FindClone RPC.
 */
SHARED boolean_t itlBak_FindClone()
{
    int status, strLength;
    char *volumeName;
    char *cloneTimeVar, *cloneSpareVar;
    struct ubik_client *callHandle;
    dumpId dumpIdentifier;
    unsigned32 cloneTime, cloneSpare;

    GET_SERVER_HANDLE(callHandle);
    GET_DUMP_ID(dumpIdentifier);
    ITL_BAK_GET_STRING("Volume Name: ", volumeName,  &strLength);
    ITL_BAK_GET_STRING("Clone Time Variable Name: ", cloneTimeVar,  
		       &strLength);
    ITL_BAK_GET_STRING("Clone Spare Variable Name: ", cloneSpareVar,  
		       &strLength);

    status = ubik_Call(BUDB_FindClone,  callHandle, UBIK_CALL_FLAGS,
		       dumpIdentifier, volumeName, &cloneTime, &cloneSpare);
    ITL_BAK_CHECK_ERROR(status);

    if (status == 0)
    {
	ITLU_ASSIGN_VARIABLE(cloneTimeVar, &cloneTime, itl_intType);
	ITLU_ASSIGN_VARIABLE(cloneSpareVar, &cloneSpare, itl_intType);
    }

    ITL_FREE(volumeName);
    ITL_FREE(cloneTimeVar);
    ITL_FREE(cloneSpareVar);

    return(FALSE);
}

/*
 *  itlBak_FindDump
 *    Issues the BUDB_FindDump RPC.
 */
SHARED boolean_t itlBak_FindDump()
{
    int status, strLength;
    char *volumeName;
    char *dumpEntryVar;
    budb_dumpEntry dumpEntry;
    struct ubik_client *callHandle;
    unsigned32 beforeDate, dateSpare;

    GET_SERVER_HANDLE(callHandle);
    ITL_BAK_GET_STRING("Volume Name: ", volumeName,  &strLength);
    beforeDate = (unsigned32) itlBak_CommonGetTime("Before Date", FALSE);
    dateSpare = (unsigned32) itlBak_CommonGetTime("Date Spare", FALSE);
    ITL_BAK_GET_STRING("Dump Entry Variable Name: ", dumpEntryVar,  
		       &strLength);

    status = ubik_Call(BUDB_FindDump,  callHandle, UBIK_CALL_FLAGS,
		       volumeName, beforeDate, dateSpare, &dumpEntry);
    ITL_BAK_CHECK_ERROR(status);

    if (status == 0)
    {
	ITLU_ASSIGN_VARIABLE(dumpEntryVar, &dumpEntry, 
			     itlBak_dumpEntryDataType);
    }

    ITL_FREE(volumeName);
    ITL_FREE(dumpEntryVar);

    return(FALSE);
}

/*
 *  itlBak_FindLatestDump
 *    Issues the BUDB_FindlatestDump RPC.
 */
SHARED boolean_t itlBak_FindLatestDump()
{
    int status, strLength;
    char *volumeName, *dumpName;
    char *dumpEntryVar;
    budb_dumpEntry dumpEntry;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    ITL_BAK_GET_STRING("Volume Name: ", volumeName,  &strLength);
    ITL_BAK_GET_STRING("Dump Name: ", dumpName,  &strLength);
    ITL_BAK_GET_STRING("Dump Entry Variable Name: ", dumpEntryVar,  
		       &strLength);

    status = ubik_Call(BUDB_FindLatestDump,  callHandle, UBIK_CALL_FLAGS,
		       volumeName, dumpName, &dumpEntry);
    ITL_BAK_CHECK_ERROR(status);

    if (status == 0)
    {
	ITLU_ASSIGN_VARIABLE(dumpEntryVar, &dumpEntry, 
			     itlBak_dumpEntryDataType);
    }

    ITL_FREE(volumeName);
    ITL_FREE(dumpName);
    ITL_FREE(dumpEntryVar);

    return(FALSE);
}

/*
 *  itlBak_FinishDump
 *    Issues the BUDB_FinishDump RPC.
 */
SHARED boolean_t itlBak_FinishDump()
{
    int status;
    char *dumpEntryVar;
    budb_dumpEntry dumpEntry;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    if (itlBak_DumpEntryGet(&dumpEntryVar, &dumpEntry))
    {
	status = ubik_Call(BUDB_FinishDump,  callHandle, UBIK_CALL_FLAGS,
			   &dumpEntry);
	ITL_BAK_CHECK_ERROR(status);

	if (status == 0)
	{
	    ITLU_ASSIGN_VARIABLE(dumpEntryVar, &dumpEntry, 
				 itlBak_dumpEntryDataType);
	}

	ITL_FREE(dumpEntryVar);
    }
    else
	itl_Error("Invalid fs_bakDumpEntry variable name entered.  The variable must already exist.\n");

    return(FALSE);
}

/*
 *  itlBak_FinishTape
 *    Issues the BUDB_FinishTape RPC.
 */
SHARED boolean_t itlBak_FinishTape()
{
    int status, strLength;
    budb_tapeEntry tapeEntry;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    itlBak_TapeEntryGet(&tapeEntry);

    status = ubik_Call(BUDB_FinishTape,  callHandle, UBIK_CALL_FLAGS,
		       &tapeEntry);
    ITL_BAK_CHECK_ERROR(status);

    return(FALSE);
}

/*
 *  itlBak_GetList
 *    Issues the BUDB_GetDumps, BUDB_GetTapes, and BUDB_GetVolumes RPC's.
 */
SHARED boolean_t itlBak_GetList(listType)
  itlBak_listTypes_t listType;
{
    int status, strLength;
    signed32 majorVersion, flags, start, end, index;
    signed32 nextIndex, dbUpdate;
    char *name;
    char *nextIndexVar, *dbUpdateVar, *listVar;
    struct ubik_client *callHandle;
    pointer_t *listP;
    char *listPrompt;
    error_status_t (*listFunc)();

    switch (listType) {
      case bakVolumeList:
	listPrompt = "Volumes Variable Name: ";
	listFunc = BUDB_GetVolumes;
	break;

      case bakTapeList:
	listPrompt = "Tapes Variable Name: ";
	listFunc = BUDB_GetTapes;
	break;

      case bakDumpList:
	listPrompt = "Dumps Variable Name: ";
	listFunc = BUDB_GetDumps;
	break;

      default:
	itl_Error("Unexpected list type in GetList, %d\n", listType);
	break;
    }

    GET_SERVER_HANDLE(callHandle);
    itl_PromptUser("Major Version: ");
    majorVersion = (signed32) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Flags (BUDB_OP_* flags): ");
    flags = (signed32) itl_GetInteger((boolean_t *) 0);
    ITL_BAK_GET_STRING("Name: ", name,  &strLength);
    itl_PromptUser("Start (as dump id): ");
    start = (signed32) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("End: ");
    end = (signed32) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Index: ");
    index = (signed32) itl_GetInteger((boolean_t *) 0);
    ITL_BAK_GET_STRING("Next Index Variable Name: ", nextIndexVar,  
		       &strLength);
    ITL_BAK_GET_STRING("dbUpdate Variable Name: ", dbUpdateVar,  &strLength);
    ITL_BAK_GET_STRING(listPrompt, listVar,  &strLength);

    status = ubik_Call(listFunc,  callHandle, UBIK_CALL_FLAGS,
		       majorVersion, flags, name, start, end, index,
		       &nextIndex, &dbUpdate, &listP);
    ITL_BAK_CHECK_ERROR(status);

    if (status == 0)
    {
	ITLU_ASSIGN_VARIABLE(nextIndexVar, &nextIndex, itl_intType);
	ITLU_ASSIGN_VARIABLE(dbUpdateVar, &dbUpdate, itl_intType);
	itlBak_ListCreate(listType, listVar, listP);
    }

    ITL_FREE(name);
    ITL_FREE(nextIndexVar);
    ITL_FREE(dbUpdateVar);
    ITL_FREE(listVar);

    /*
     *  Free the memory that was allocated for the list by the RPC.
     *  We can do this because itlBak_ListCreate() will copy the list.
     */
    ITL_FREE(listP);

    return(FALSE);
}

/*
 *  itlBak_UseTape
 *    Issues the BUDB_UseTape RPC.
 */
SHARED boolean_t itlBak_UseTape()
{
    int status, strLength;
    char *newVar;
    signed32 new = 0;
    budb_tapeEntry tapeEntry;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    itlBak_TapeEntryGet(&tapeEntry);
    ITL_BAK_GET_STRING("New Variable Name: ", newVar,  &strLength);

    status = ubik_Call(BUDB_UseTape,  callHandle, UBIK_CALL_FLAGS,
		       &tapeEntry, &new);
    ITL_BAK_CHECK_ERROR(status);

    if (status == 0)
	ITLU_ASSIGN_VARIABLE(newVar, &new, itl_intType);

    ITL_FREE(newVar);

    return(FALSE);
}

/*
 *  itlBak_GetText
 *    Issues the BUDB_GetText RPC.
 */
SHARED boolean_t itlBak_GetText()
{
    int status, strLength;
    char *nextOffsetVar, *charListVar;
    signed32 lockHandle, textType, maxLength, offset, nextOffset;
    charListT *charListP;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);    
    itl_PromptUser("Lock Handle: ");
    lockHandle = (signed32) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Text Type (TB_* flags): ");
    textType = (signed32) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Maximum Length: ");
    maxLength = (signed32) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Offset: ");
    offset = (signed32) itl_GetInteger((boolean_t *) 0);
    ITL_BAK_GET_STRING("Next Offset Variable Name: ", nextOffsetVar,  
		       &strLength);
    if (itlBak_CharListVariableGet(&charListVar, &charListP))
    {
	status = ubik_Call(BUDB_GetText,  callHandle, UBIK_CALL_FLAGS,
			   lockHandle, textType, maxLength, offset, 
			   &nextOffset, charListP);
	ITL_BAK_CHECK_ERROR(status);

	if (status == 0)
	{
	    ITLU_ASSIGN_VARIABLE(nextOffsetVar, &nextOffset, itl_intType);
	    ITLU_ASSIGN_VARIABLE(charListVar, charListP, 
				 itlBak_charListDataType);
	}

	ITL_FREE(charListVar);
	ITL_FREE(charListP);
    }
    else
	itl_Error("Invalid charListT variable name entered.  The variable must already exist.\n");

    ITL_FREE(nextOffsetVar);

    return(FALSE);
}

/*
 *  itlBak_GetTextVersion
 *    Issues the BUDB_GetTextVersion RPC.
 */
SHARED boolean_t itlBak_GetTextVersion()
{
    int status, strLength;
    char *textVersionVar;
    signed32 textType, textVersion;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    itl_PromptUser("Text Type (TB_* flags): ");
    textType = (signed32) itl_GetInteger((boolean_t *) 0);
    ITL_BAK_GET_STRING("Text Version Variable Name: ", textVersionVar,  
		       &strLength);

    status = ubik_Call(BUDB_GetTextVersion,  callHandle, UBIK_CALL_FLAGS,
		       textType, &textVersion);
    ITL_BAK_CHECK_ERROR(status);

    if (status == 0)
	ITLU_ASSIGN_VARIABLE(textVersionVar, &textVersion, itl_intType);

    ITL_FREE(textVersionVar);

    return(FALSE);
}

/*
 *  itlBak_SaveText
 *    Issues the BUDB_SaveText RPC.
 */
SHARED boolean_t itlBak_SaveText()
{
    int status, strLength;
    signed32 lockHandle, textType, offset, flags;
    charListT charList;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);    
    itl_PromptUser("Lock Handle: ");
    lockHandle = (signed32) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Text Type (TB_* flags): ");
    textType = (signed32) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Offset: ");
    offset = (signed32) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Flags (BUDB_OP_* flags): ");
    flags = (signed32) itl_GetInteger((boolean_t *) 0);
    itlBak_CharListDataGet(&charList);
    status = ubik_Call(BUDB_SaveText,  callHandle, UBIK_CALL_FLAGS,
		       lockHandle, textType, offset, flags, &charList);
    ITL_BAK_CHECK_ERROR(status);

    return(FALSE);
}

/*
 *  itlBak_FreeAllLocks
 *    Issues the BUDB_FreeAllLocks RPC.
 */
SHARED boolean_t itlBak_FreeAllLocks()
{
    int status;
    unsigned32 instanceId;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    itl_PromptUser("Instance Id: ");
    instanceId = (unsigned32) itl_GetInteger((boolean_t *) 0);

    status = ubik_Call(BUDB_FreeAllLocks,  callHandle, UBIK_CALL_FLAGS,
		       instanceId);
    ITL_BAK_CHECK_ERROR(status);

    return(FALSE);
}

/*
 *  itlBak_FreeLock
 *    Issues the BUDB_FreeLock RPC.
 */
SHARED boolean_t itlBak_FreeLock()
{
    int status;
    unsigned32 lockHandle;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    itl_PromptUser("Lock Handle: ");
    lockHandle = (unsigned32) itl_GetInteger((boolean_t *) 0);

    status = ubik_Call(BUDB_FreeLock,  callHandle, UBIK_CALL_FLAGS,
		       lockHandle);
    ITL_BAK_CHECK_ERROR(status);

    return(FALSE);
}

/*
 *  itlBak_GetInstanceId() --
 *    Issues the BUDB_GetInstanceId RPC.
 */
SHARED boolean_t itlBak_GetInstanceId()
{
    int status, strLength;
    unsigned32 instanceId;
    char *instanceVar;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    ITL_BAK_GET_STRING("Instance Variable Name: ", instanceVar, &strLength);

    status = ubik_Call(BUDB_GetInstanceId,  callHandle, UBIK_CALL_FLAGS,
		       &instanceId);
    ITL_BAK_CHECK_ERROR(status);

    if (status == 0)
	ITLU_ASSIGN_VARIABLE(instanceVar, &instanceId, itl_intType);

    ITL_FREE(instanceVar);

    return(FALSE);
}

/*
 *  itlBak_GetLock() --
 *    Issues the BUDB_GetLock RPC.
 */
SHARED boolean_t itlBak_GetLock()
{
    int status, strLength;
    unsigned32 instanceId, lockHandle;
    signed32 lockName, expiration;
    char *lockHandleVar;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    itl_PromptUser("Instance Id: ");
    instanceId = (unsigned32) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Lock Name: ");
    lockName = (unsigned32) itl_GetInteger((boolean_t *) 0);
    itl_PromptUser("Expiration: ");
    expiration = (unsigned32) itl_GetInteger((boolean_t *) 0);
    ITL_BAK_GET_STRING("Lock Handle Variable Name: ", lockHandleVar, 
		       &strLength);

    status = ubik_Call(BUDB_GetLock,  callHandle, UBIK_CALL_FLAGS,
		       instanceId, lockName, expiration, &lockHandle);
    ITL_BAK_CHECK_ERROR(status);

    if (status == 0)
	ITLU_ASSIGN_VARIABLE(lockHandleVar, &lockHandle , itl_intType);

    ITL_FREE(lockHandleVar);

    return(FALSE);
}

/*
 *  itlBak_DbVerify() --
 *    Issues the BUDB_DbVerify RPC.
 */
SHARED boolean_t itlBak_DbVerify()
{
    int status, strLength;
    signed32 statusArg, orphans, host;
    char *statusArgVar, *orphansVar, *hostVar;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    ITL_BAK_GET_STRING("Status Variable Name: ", statusArgVar, &strLength);
    ITL_BAK_GET_STRING("Orphans Variable Name: ", orphansVar, &strLength);
    ITL_BAK_GET_STRING("Host Variable Name: ", hostVar, &strLength);

    status = ubik_Call(BUDB_DbVerify,  callHandle, UBIK_CALL_FLAGS,
		       &statusArg, &orphans, &host);
    ITL_BAK_CHECK_ERROR(status);

    if (status == 0)
    {
	ITLU_ASSIGN_VARIABLE(statusArgVar, &statusArg , itl_intType);
	ITLU_ASSIGN_VARIABLE(orphansVar, &orphans , itl_intType);
	ITLU_ASSIGN_VARIABLE(hostVar, &host , itl_intType);
    }

    ITL_FREE(statusArgVar);
    ITL_FREE(orphansVar);
    ITL_FREE(hostVar);

    return(FALSE);
}

/*
 *  itlBak_DumpDB
 *    Issues the BUDB_DumpDB RPC.
 */
SHARED boolean_t itlBak_DumpDB()
{
    int status, strLength;
    char *charListVar, *flagsVar;
    signed32 maxLength, flags;
    charListT *charListP;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);    
    itl_PromptUser("Maximum Length: ");
    maxLength = (signed32) itl_GetInteger((boolean_t *) 0);
    ITL_BAK_GET_STRING("Flags Variable Name: ", flagsVar,  &strLength);
    if (itlBak_CharListVariableGet(&charListVar, &charListP))
    {
	status = ubik_Call(BUDB_DumpDB,  callHandle, UBIK_CALL_FLAGS,
			   maxLength, &flags, charListP);
	ITL_BAK_CHECK_ERROR(status);

	if (status == 0)
	{
	    ITLU_ASSIGN_VARIABLE(flagsVar, &flags, itl_intType);
	    ITLU_ASSIGN_VARIABLE(charListVar, charListP, 
				 itlBak_charListDataType);
	}

	ITL_FREE(charListVar);
	ITL_FREE(charListP);
    }
    else
	itl_Error("Invalid charListT variable name entered.  The variable must already exist.\n");

    ITL_FREE(flagsVar);

    return(FALSE);
}

/*
 *  itlBak_RestoreDbHeader
 *    Issues the BUDB_RestoreDbHeader RPC.
 */
SHARED boolean_t itlBak_RestoreDbHeader()
{
    int status;
    DbHeader dbHeader;
    struct ubik_client *callHandle;

    GET_SERVER_HANDLE(callHandle);
    itlBak_DBHeaderGet(&dbHeader);

    status = ubik_Call(BUDB_RestoreDbHeader,  callHandle, UBIK_CALL_FLAGS,
		       &dbHeader);
    ITL_BAK_CHECK_ERROR(status);

    return(FALSE);
}
