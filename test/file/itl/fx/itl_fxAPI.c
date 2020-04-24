/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1996, 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fxAPI.c -- 
 *    Implements the RPC calls for the DFS file exporter, i.e. its API.
 *
 * NOTE:
 *    This code does NOT do any token management, nor does it allow the
 *    script writer to do so.  All token management is assumed to be done
 *    at the file exporter.
 */
/*
 * HISTORY
 * $Log: itl_fxAPI.c,v $
 * Revision 1.1.313.1  1996/10/17  18:26:10  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:11  damon]
 *
 * $EndLog$
 */

#include <assert.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>     /* for dce_cf.h */
#include <dcedfs/osi_dfserrors.h>
#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include <dcedfs/osi.h>	/* which includes lock.h which will be undone next */
#include <dcedfs/lock.h>
#include <dcedfs/nubik.h>
#include <dcedfs/common_data.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/tkn4int.h>
#include <dcedfs/fldb_proc.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_vectype.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_commonHyper.h>
#include <utils/itl/itl_commonTimeval.h>
#include <utils/itl/itl_commonFid.h>
#include <utils/itl/itl_fxDCEError.h>
#include <utils/itl/itl_fxCommon.h>
#include <utils/itl/itl_fxPrincipal.h>
#include <utils/itl/itl_fxConnect.h>
#include <utils/itl/itl_fxFileset.h>
#include <utils/itl/itl_fxToken.h>
#include <utils/itl/itl_fxStoreStatus.h>
#include <utils/itl/itl_fxFetchStatus.h>
#include <utils/itl/itl_fxVolSync.h>
#include <utils/itl/itl_fxFidName.h>
#include <utils/itl/itl_fxPipe.h>
#include <utils/itl/itl_fxAPI.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxAPI.c,v 1.1.313.1 1996/10/17 18:26:10 damon Exp $")

#define ITL_FX_DFS_SERVER_GROUP  "subsys/dce/dfs-fs-servers"
#define ITL_FX_NO_TOKEN_MGMT     0

/*
 *  ITL_FX_GET_STRING()
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
#define ITL_FX_GET_STRING(_usrPrompt, _strName, _strLenP)                    \
        MACRO_BEGIN                                                          \
          ITL_GET_STRING((_usrPrompt), (_strName), (_strLenP));              \
          assert((_strName) && strcmp((_strName), ""));                      \
        MACRO_END

itlFX_cell_t cellInfo;   /* all info about cell */

/*
 *  Take advantage of the fact that global variables are set to 0.  Define
 *  a "zero"  UUID.  This is really a constant.
 */
PRIVATE afsUUID zeroUUID;

PRIVATE char itlFX_InfoMsg[] = 
"The ITL cache manager API module provides command interfaces to the \
API's exported by the DFS file exporter.";

/*
 *  XXX -
 *    Need to define other AFS_FLAG flags and change code to react to them
 *    if required, i.e. AFS_FLAG_ZERO and AFS_FLAG_SKIPSTATUS.
 */

PRIVATE itlu_valueMap_t afsFlags[] = {
    {"AFS_FLAG_SEC_SERVICE", AFS_FLAG_SEC_SERVICE},
    {NULL,                   0}
};


/*
 *  Define command table entries supported by the functions in this file.
 */
EXPORT itl_commandTable_t itlFX_CommandTable[] = {
    ITL_FX_LOGIN_CMD,
    ITL_FX_FILESET_CONNECT_CMD,
    ITL_FX_USER_CONNECT_CMD,
    ITL_FX_SET_CONTEXT_CMD,
    ITL_FX_LOOKUP_ROOT_CMD,
    ITL_FX_LOOKUP_CMD,
    ITL_FX_CREATE_FILE_CMD,
    ITL_FX_REMOVE_FILE_CMD,
    ITL_FX_FETCH_DATA_CMD,
    ITL_FX_STORE_DATA_CMD,
    ITL_FX_FETCH_STATUS_CMD,
    ITL_FX_STORE_STATUS_CMD,
    ITL_FX_MAKE_DIR_CMD,
    ITL_FX_REMOVE_DIR_CMD,
    ITL_FX_SYM_LINK_CMD,
    ITL_FX_HARD_LINK_CMD,
    ITL_FX_RENAME_CMD,
    ITL_FX_READDIR_CMD,
    ITL_FX_GET_TIME_CMD
};

PRIVATE boolean_t GetCellInfo _TAKES(( void ));

/*
 *  GetCellInfo() --
 *    Obtains information about the local cell, e.g name, FLDB binding(s)
 *    etc.
 *
 *  RETURN CODE:
 *    FALSE - success
 *    TRUE  - failure
 */
PRIVATE boolean_t GetCellInfo()
{
    unsigned long errorCode = 0;
    char *cellNameP = NULL, *junctionIndexP;
    int cellNameLen;
    boolean_t retVal = TRUE;

    dce_cf_get_cell_name(&cellNameP, &errorCode);

    if ((errorCode == dce_cf_st_ok) &&
	(cellNameP != NULL)         &&
	(*cellNameP == '/')         &&
	((cellNameLen = strlen(cellNameP)) < ITL_FX_MAX_CELL_NAME))
    {
	/*
	 *  Get the path for the 'fs' junction.  Need to copy the cell
	 *  name to a larger array.
	 */
	strcpy(cellInfo.name, cellNameP);
	free(cellNameP);
	junctionIndexP = (char *) &(cellInfo.name[cellNameLen]);
	dfs_GetJunctionName(cellInfo.name, 0, junctionIndexP,
			    (ITL_FX_MAX_CELL_NAME - cellNameLen - 1), 
			    &errorCode);
	if (!errorCode) 
	{
	    struct ubik_client *fldbInfoP;

	    /* 
	     * Get the bindings to the FLserver.  The 0, 0, 0, imply
	     * do not use the un-authenticated binding, do not use the local
	     * authentication binding, and do not initialize the 
	     * authentication.
	     */
	    if ((errorCode = (unsigned long) 
		             ubik_ClientInit(NULL, cellInfo.name,
					     ITL_FX_DFS_SERVER_GROUP,
					     0, 0, 0, &fldbInfoP))
		          == 0)
	    {
		struct vlconf_cell cellConf;
		boolean_t done = FALSE;
		int i;

		for (i = 0; ((done == FALSE) || (i < fldbInfoP->nServers));
		     i++)
		{
		    if (fldbInfoP->conns[i])
		    {
			if ((errorCode = ubik_Call(VL_GetCellInfo, 
						   fldbInfoP, 0, &cellConf))
			              == error_status_ok)
			{
			    /*
			     *  Overwrite cellname with the junction in it
			     *  with just the pure cell name.
			     */
			    strcpy(cellInfo.name, (char *) cellConf.name);
			    cellInfo.cellId = cellConf.CellID;
			    cellInfo.ubikInfoP = fldbInfoP;
			    retVal = FALSE;
			    done = TRUE;
			}
			else
			{
			    done = TRUE;
			    ITL_FX_DCE_ERROR(errorCode, "VL_GetCellInfo error");
			}
		    }
		}
	    }
	    else
		ITL_FX_DCE_ERROR(errorCode, "ubik_ClientInit error");
	}
	else
	    ITL_FX_DCE_ERROR(errorCode, "dfs_GetJunctionName error");
    }
    else
    {
	itl_Error("Cannot find cell name, or name is invalid\n");
	itl_Error("\tName: %s, Length: %d", cellNameP, cellNameLen);
	ITL_FX_DCE_ERROR(errorCode, "DCE error");
    }

    return(retVal);
}

/*
 *  AddFXCommands() --
 *    Private function that really adds the FX commands.  Necessary to
 *    accomodate how pthreads does one time initialization.
 */
PRIVATE void AddFXCommands()
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
     *  Initialize FX error table.
     */
    osi_initDecodeTable();

    /*
     *  Get the information about this cell.
     */
    if (GetCellInfo())
    {
	itl_Error("Could NOT get cell information, exiting.\n");
	exit(-1);
    }

    /*
     *  Initialize our token revocation service, and all other sub-modules.
     */
    itlCommon_HyperInit();
    itlCommon_TimevalInit();
    itlCommon_FidInit();
    itlFX_TokenInit();
    itlFX_FilesetInit();
    itlFX_PrincipalInit();
    itlFX_ServerInit();
    itlFX_ConnectInit();
    itlFX_StoreStatusInit();
    itlFX_FetchStatusInit();
    itlFX_VolSyncInit();

    if (returnCode = itlu_ProcessValueMap(afsFlags, NULL))
    {
	itl_Error("itlu_ProcessValueMap (afsFlags) FAILED - %ld\n", 
		  returnCode);
	exit(-1);
    }

    (void) itl_AddModuleCmds("cm",
			     itlFX_CommandTable,
			     sizeof(itlFX_CommandTable) / 
			     sizeof(itl_commandTable_t),
			     COMMAND_MODE_ALL,
			     itlFX_InfoMsg);
}

/*
 *  itlFX_AddFXCommands() -
 *    Do everything necessary to make the ITL cache manager commands 
 *    accessable via the testing interface.
 */
SHARED void itlFX_AddFXCommands()
{
    static pthread_once_t addFXOnce = pthread_once_init;

    if (pthread_once(&addFXOnce, (pthread_initroutine_t) AddFXCommands))
	itl_Error("pthread_once failed in AddFXCommands\n");
}


/*
 *  itlFX_DceLogin() --
 *    This function establishes a DCE login context.
 */
SHARED boolean_t itlFX_DceLogin()
{
    char *userName, *userPassword;
    int strLength;

    ITL_FX_GET_STRING("User Name: ", userName, &strLength);
    ITL_FX_GET_STRING("User Password: ", userPassword, &strLength);

    if (itlFX_PrincipalAddName(userName, userPassword))
	itl_Error("Could NOT create login for, %s\n", userName);

    ITL_FREE(userPassword);
    ITL_FREE(userName);

    return(FALSE);
}


/*
 *  itlFX_FilesetConnect() --
 *    Establishes a connection to a particular fileset, and records
 *    necessary info.
 */
SHARED boolean_t itlFX_FilesetConnect()
{
    char *filesetName;
    error_status_t errorStatus;
    int strLength;

    ITL_FX_GET_STRING("Fileset Name: ", filesetName, &strLength);

    if (itlFX_FilesetAddName(filesetName))
	itl_Error("Could NOT connect to fileset, %s\n", filesetName);

    ITL_FREE(filesetName);

    return(FALSE);
}


/*
 *  itlFX_UserConnect() --
 *    Establishes a connection to a particular fileset, on a specified
 *    interface, for a specified user.
 */
SHARED boolean_t itlFX_UserConnect()
{
    char *filesetName, *ifName, *userName, *varName;
    int interface, strLength;
    error_status_t errorStatus;

    ITL_FX_GET_STRING("Fileset Name: ", filesetName, &strLength);

    /*
     *  Can't call ITL_FX_GET_STRING because the empty string is a valid
     *  reponse for a user name.  It is the un-authenticated user.
     */
    ITL_GET_STRING("User Name: ", userName, &strLength);
    assert(userName);
    ITL_FX_GET_STRING("Interface (PRIMARY or SECONDARY): ", ifName, 
		      &strLength);
    interface = (strcmp(ifName, "PRIMARY")) ? ITL_FX_SECONDARY_IF :
		                              ITL_FX_PRIMARY_IF;
    ITL_FX_GET_STRING("Fileset Handle Variable (fs_fxHandle): ", varName, 
		      &strLength);

    if (itlFX_ConnectCreate(filesetName, interface, userName, varName))
    {
	itl_Error("Could NOT establish user connection for,\n  Fileset: %s, Interface: %s, User: %s\n",
		  filesetName, ifName, userName);
    }

    ITL_FREE(varName);
    ITL_FREE(ifName);
    ITL_FREE(userName);
    ITL_FREE(filesetName);

    return(FALSE);
}

/* 
 *  itlFX_SetContext() --
 *    Issue the AFS_SetContext() RPC.
 */
SHARED boolean_t itlFX_SetContext()
{
    itlFX_connect_t *connectInfoP;

    if ((connectInfoP = itlFX_ConnectGet()))
    {
	error_status_t returnCode;
	unsigned32 flags;
	itlFX_connect_t *secondaryConnectP;
	afsUUID secondaryUUID;

	itl_PromptUser("Flags ($AFS_FLAG_*): ");
	flags = (unsigned32) itl_GetInteger((boolean_t *) 0);

	/*
	 *  If the user set the AFS_FLAG_SEC_SERVICE flag, then prompt
	 *  for the second rpc handle, from which we will extract the 
	 *  UUID.
	 */
	if (flags & AFS_FLAG_SEC_SERVICE)
	{
	    if ((secondaryConnectP = itlFX_ConnectGet()))
	    {
		secondaryUUID = ITL_FX_SERVER_SECONDARY(ITL_FX_FILESET_SERVER(ITL_FX_CONNECT_FILESET(secondaryConnectP)));
	    }
	    else
	    {
		secondaryUUID = zeroUUID;
		itl_Error("Could NOT obtain valid fileset handle for secondary UUID.\n");
	    }
	}

	returnCode = AFS_SetContext(ITL_FX_CONNECT_HANDLE(connectInfoP),
				    epochStartTime, &tokenServerAddress,
				    flags, &secondaryUUID,
				    OSI_MAX_FILE_PARM_CLIENT, 0);
	ITL_FX_CHECK_ERROR(returnCode);
    }
    else
	itl_Error("Could NOT obtain valid fileset handle.\n");

    return(FALSE);
}

/* 
 *  itlFX_GetTime() --
 *    Issue the AFS_GetTime() RPC.
 */
SHARED boolean_t itlFX_GetTime()
{
    itlFX_connect_t *connectInfoP;

    if ((connectInfoP = itlFX_ConnectGet()))
    {
	struct timeval timeInfo;
	unsigned long notUsed;
	error_status_t returnCode;

	returnCode = AFS_GetTime(ITL_FX_CONNECT_HANDLE(connectInfoP),
				 (unsigned long *) &(timeInfo.tv_sec),
				 (unsigned long *) &(timeInfo.tv_usec),
				 &notUsed, &notUsed);
	ITL_FX_CHECK_ERROR(returnCode);
    }
    else
	itl_Error("Could NOT obtain valid fileset handle.\n");

    return(FALSE);
}

/* 
 *  itlFX_Lookup() --
 *    Issue the AFS_LookupRoot() or the AFS_Lookup() RPC.
 */
SHARED boolean_t itlFX_Lookup(doRootLookup)
  IN boolean_t doRootLookup;   /* TRUE - then do AFS_LookupRoot */
{
    itlFX_connect_t *connectInfoP;
    boolean_t gotLookupArgs = TRUE;
    int strLength;

    if ((connectInfoP = itlFX_ConnectGet()))
    {
	char *fidName, *fileName;
	afsFid *dirFidP;
	afs_hyper_t minVV;
	afsTaggedName nameInfo;

	fidName = fileName = NULL;
	/*
	 *  Prompt for a file id and name if we are doing a normal lookup.
	 */
	if (!doRootLookup)
	{
	    if (dirFidP = itlCommon_FidGet())
	    {
		ITL_FX_GET_STRING("File Name: ", fileName, &strLength);
		if (strLength >= AFS_NAMEMAX)
		{
		    ITL_FX_NAME_ERROR(fileName);
		    gotLookupArgs = FALSE;
		}
		(void) memset((char *) &nameInfo, 0, sizeof(nameInfo));
		strcpy((char *)(nameInfo.tn_chars), fileName);
	    }
	    else
	    {
		itl_Error("Could NOT obtain valid file identifier.\n");
		gotLookupArgs = FALSE;
	    }
	}

	/*
	 *  If all is well then continue.
	 */
	if (gotLookupArgs)
	{
	    error_status_t returnCode;
	    afsFid tempFid, fileId;
	    afs_token_t fileToken;
	    afsVolSync fileSync;
	    afsFetchStatus fileStatus, dirStatus;
	    char  *fileStatusName, *dirStatusName, *volSyncName;

	    itl_AdviseUser("minVV Value - \n");
	    itlCommon_HyperGet(&minVV);
	    fileStatusName = dirStatusName = volSyncName = NULL;
	    ITL_FX_GET_STRING("File Id Variable (fs_fxFid): ", fidName, 
			      &strLength);
	    ITL_FX_GET_STRING("File Status Variable (fs_fxFetchStatus): ", 
			      fileStatusName, &strLength);

	    if (!doRootLookup)
	    {
		ITL_FX_GET_STRING("Directory Status Variable (fs_fxFetchStatus): ", 
				  dirStatusName, &strLength);
	    }

	    ITL_FX_GET_STRING("Volume Sync Variable(fs_fxVolSync): ",
			      volSyncName, &strLength);
	    if (doRootLookup)
	    {
		/*
		 *  Build up the tempFid.
		 */
		tempFid.Cell = cellInfo.cellId;
		tempFid.Volume =
		    ITL_FX_FILESET_ID(ITL_FX_CONNECT_FILESET(connectInfoP));

		returnCode = AFS_LookupRoot(ITL_FX_CONNECT_HANDLE(connectInfoP),
					    &tempFid, (afs_hyper_t *) &minVV,
					    ITL_FX_NO_TOKEN_MGMT, &fileId,
					    &fileStatus, &fileToken, &fileSync);
	    }
	    else
	    {
		returnCode = AFS_Lookup(ITL_FX_CONNECT_HANDLE(connectInfoP),
					dirFidP, &nameInfo,
					(afs_hyper_t *) &minVV,
					ITL_FX_NO_TOKEN_MGMT, &fileId, 
					&fileStatus, &dirStatus,
					&fileToken, &fileSync);
	    }
	    ITL_FX_CHECK_ERROR(returnCode);

	    /*
	     *  Set the ITL command result variable so that scripts can
	     *  check for the existence of a file; an operation that
	     *  could fail but should not stop an existence check.
	     */
	    itl_SetIntResult(&commandResult, (int) returnCode);

	    /*
	     *  If the call was successful, then create the necessary
	     *  variables.
	     */
	    if (returnCode == error_status_ok)
	    {
		ITLU_ASSIGN_VARIABLE(fidName, &fileId, itlCommon_fidDataType);
		ITLU_ASSIGN_VARIABLE(fileStatusName, &fileStatus,
				    itlFX_fetchStatusDataType);
		ITLU_ASSIGN_VARIABLE(volSyncName, &fileSync,
				    itlFX_volSyncDataType);
		if (!doRootLookup)
		    ITLU_ASSIGN_VARIABLE(dirStatusName, &dirStatus,
					itlFX_fetchStatusDataType);
	    }

	    if (fileStatusName)
		ITL_FREE(fileStatusName);
	    if (dirStatusName)
		ITL_FREE(dirStatusName);
	    if (volSyncName)
		ITL_FREE(volSyncName);
	}
	if (fileName)
	    ITL_FREE(fileName);
	if (fidName)
	    ITL_FREE(fidName);
    }
    else
	itl_Error("Could NOT obtain valid fileset handle.\n");

    return(FALSE);
}

/* 
 *  itlFX_CreateOrRemove() --
 *    Issue the AFS_CreateFile(), AFS_MakeDir(), AFS_RemoveFile(), or
 *    AFS_RemoveDir() RPC.
 */
SHARED boolean_t itlFX_CreateOrRemove(rpcOp)
  IN itlFX_fileDirOp_t rpcOp;
{
    itlFX_connect_t *connectInfoP;
    int strLength;

    if ((connectInfoP = itlFX_ConnectGet()))
    {
	afsFid *dirFidP;

	/*
	 *  Prompt for a file id for the directory, and a file name.
	 */
	if (dirFidP = itlCommon_FidGet())
	{
	    error_status_t returnCode;
	    char *fileName, *fidName;
	    char *fileStatusName, *dirStatusName, *volSyncName;
	    afsStoreStatus statusInfo;
	    afs_hyper_t minVV;
	    afsFid fileId;
	    afs_token_t fileToken;
	    afsVolSync fileSync;
	    afsFetchStatus fileStatus, dirStatus;
	    afs_hyper_t returnTokenId;
	    afsFidTaggedName fileIdInfo;
	    afsTaggedName nameInfo;

	    if ((rpcOp == itlFX_createFile) || (rpcOp == itlFX_createDir))
	    {
		ITL_FX_GET_STRING("File Name: ", fileName, &strLength);
		if (strLength < AFS_NAMEMAX)
		{
		    itl_AdviseUser("Store Status -\n");
		    itlFX_StoreStatusGet(&statusInfo);
		}
		else
		    ITL_FX_NAME_ERROR(fileName);
		/* Copy it in case we need an afsTaggedName instead */
		(void) memset((char *) &nameInfo, 0, sizeof(nameInfo));
		strcpy((char *)(nameInfo.tn_chars), fileName);
	    }
	    else
	    {
		itl_AdviseUser("FidName -\n");
		itlFX_FidNameGet(&fileIdInfo);
		/*
		 *  No token mamagement yet, so we just 0 out this arg.
		 */
		AFS_hzero(returnTokenId);
	    }
	    itl_PromptUser("minVV Value -\n");
	    itlCommon_HyperGet(&minVV);

	    ITL_FX_GET_STRING("File Id Variable (fs_fxFid): ", fidName, 
			      &strLength);
	    ITL_FX_GET_STRING("Directory Status Varaible (fs_fxFetchStatus): ", 
			      dirStatusName, &strLength);
	    ITL_FX_GET_STRING("File Status Variable (fs_fxFetchStatus): ", 
			      fileStatusName, &strLength);
	    ITL_FX_GET_STRING("Volume Sync Variable (fs_fxVolSync): ", 
			      volSyncName, &strLength);

	    switch (rpcOp) {
	      case itlFX_createFile :
		  returnCode = AFS_CreateFile(ITL_FX_CONNECT_HANDLE(connectInfoP),
					      dirFidP, &nameInfo,
					      &statusInfo, (afs_hyper_t *) &minVV,
					      ITL_FX_NO_TOKEN_MGMT, &fileId, 
					      &fileStatus, &dirStatus,
					      &fileToken, &fileSync);
		  break;

	      case itlFX_createDir :
		  returnCode = AFS_MakeDir(ITL_FX_CONNECT_HANDLE(connectInfoP),
					   dirFidP, &nameInfo,
					   &statusInfo, (afs_hyper_t *) &minVV,
					   ITL_FX_NO_TOKEN_MGMT, &fileId, 
					   &fileStatus, &dirStatus,
					   &fileToken, &fileSync);
		  break;

	      case itlFX_removeFile :
		  returnCode = AFS_RemoveFile(ITL_FX_CONNECT_HANDLE(connectInfoP),
					      dirFidP, &fileIdInfo, 
					      &returnTokenId,
					      (afs_hyper_t *) &minVV, 
					      ITL_FX_NO_TOKEN_MGMT, &dirStatus,
					      &fileStatus, &fileId, &fileSync);
		  break;

	      case itlFX_removeDir :
		  returnCode = AFS_RemoveDir(ITL_FX_CONNECT_HANDLE(connectInfoP),
					     dirFidP, &fileIdInfo, 
					     &returnTokenId,
					     (afs_hyper_t *) &minVV, 
					     ITL_FX_NO_TOKEN_MGMT, &dirStatus,
					     &fileId, &fileStatus, &fileSync);
		  break;

	      default :
		  itl_Error("INTERNAL ERROR - Bad rpcOp (%d)\n", rpcOp);
	    }

	    ITL_FX_CHECK_ERROR(returnCode);

	    /*
	     *  If the call was successful, then create the necessary
	     *  variables.
	     */
	    if (returnCode == error_status_ok)
	    {
		ITLU_ASSIGN_VARIABLE(fidName, &fileId, itlCommon_fidDataType);
		ITLU_ASSIGN_VARIABLE(fileStatusName, &fileStatus,
				    itlFX_fetchStatusDataType);
		ITLU_ASSIGN_VARIABLE(volSyncName, &fileSync,
				    itlFX_fetchStatusDataType);
		ITLU_ASSIGN_VARIABLE(dirStatusName, &dirStatus,
				    itlFX_fetchStatusDataType);
	    }

	    if ((rpcOp == itlFX_createFile) || (rpcOp == itlFX_createDir))
		ITL_FREE(fileName);

	    ITL_FREE(fileStatusName);
	    ITL_FREE(dirStatusName);
	    ITL_FREE(volSyncName);
	    ITL_FREE(fidName);
	}
	else
	    itl_Error("Could NOT obtain valid file identifier.\n");
    }
    else
	itl_Error("Could NOT obtain valid fileset handle.\n");

    return(FALSE);
}

/* 
 *  itlFX_DataFetchOrStore() --
 *    Issue the AFS_FetchData() or AFS_StoreData() RPC.
 */
SHARED boolean_t itlFX_DataFetchOrStore(doFetch)
  IN boolean_t doFetch;   /* TRUE dor AFS_FetchData() */
{
    itlFX_connect_t *connectInfoP;
    int strLength;

    if ((connectInfoP = itlFX_ConnectGet()))
    {
	afsFid *fileIdP;

	if (fileIdP = itlCommon_FidGet())
	{
	    error_status_t returnCode;
	    char *fileStatusName, *volSyncName, *dataBuffer, *fetchVarName;
	    char defaultBuffer;
	    signed32 length;
	    afs_hyper_t minVV, position;
	    afs_token_t fileToken;
	    afsVolSync fileSync;
	    afsFetchStatus fileStatus;
	    afsStoreStatus inStatus;
	    pipe_t dataStream;
	    itlFX_pipeState_t pipeState;
	    var_val_t *dataP;

	    /*
	     *  Obtain necessary input data for the function call.
	     */
	    itl_AdviseUser("minVV Value -\n");
	    itlCommon_HyperGet(&minVV);
	    itl_AdviseUser("Position Value -\n");
	    itlCommon_HyperGet(&position);
	    itl_PromptUser("Length: ");
	    length = (signed32) itl_GetInteger((boolean_t *) 0);

	    if (doFetch)
	    {
		ITL_FX_GET_STRING("Fetch Data Variable (fs_fxStoreStatus: ", 
				  fetchVarName, &strLength);
		/*
		 *  Need to allocate a buffer to hold the data.  If length
		 *  is zero, we "fake" it.
		 */
		if (length)
		    ITL_ALLOC(dataBuffer, char *, length);
		else
		{
		    defaultBuffer = '\0';
		    dataBuffer = &defaultBuffer;
		}
	    }
	    else
	    {
		itlFX_StoreStatusGet(&inStatus);
		itl_PromptUser("Store Data: ");
		dataP = itlValue_Read((var_val_t *) 0, itl_vectorType);
		assert(dataP);

		/*
		 *  Set dataBuffer to point to the data in the vector 
		 *  type variable.
		 */
		dataBuffer = (char *) itlVector_Value(dataP);
		assert(VALUE_SIZE(dataP) >= length);
	    }

	    ITL_FX_GET_STRING("File Status Variable (fs_fxFetchStatus): ", 
			      fileStatusName, &strLength);
	    ITL_FX_GET_STRING("Volume Sync Variable (fs_fxVolSync): ",
			      volSyncName, &strLength);

	    /*
	     *  Set up common elements of the pipe structure.
	     */
	    ITL_FX_PIPE_BUF_ALLOC(&pipeState);
	    ITL_FX_PIPE_SET_IO_BUF(&pipeState, dataBuffer, length);
	    dataStream.alloc = itlFX_PipeAlloc;
	    dataStream.state = (char *) &pipeState;

	    if (doFetch)
	    {
		dataStream.push = itlFX_PipePush;
		returnCode = AFS_FetchData(ITL_FX_CONNECT_HANDLE(connectInfoP),
					   fileIdP, (afs_hyper_t *) &minVV, 
					   (afs_hyper_t *) &position, length,
					   ITL_FX_NO_TOKEN_MGMT, &fileStatus,
					   &fileToken, &fileSync, &dataStream);
	    }
	    else
	    {
		dataStream.pull = itlFX_PipePull;
		returnCode = AFS_StoreData(ITL_FX_CONNECT_HANDLE(connectInfoP),
					   fileIdP, &inStatus, 
					   (afs_hyper_t *) &position, 
					   length, (afs_hyper_t *) &minVV, 
					   ITL_FX_NO_TOKEN_MGMT, &dataStream,
					   &fileStatus, &fileSync);
		itl_FreeVarVal(dataP);
	    }
	    ITL_FX_CHECK_ERROR(returnCode);

	    /*
	     *  If the call was successful, then create the necessary
	     *  variables.
	     */
	    if (returnCode == error_status_ok)
	    {
		ITLU_ASSIGN_VARIABLE(fileStatusName, &fileStatus,
				    itlFX_fetchStatusDataType);
		ITLU_ASSIGN_VARIABLE(volSyncName, &fileSync,
				    itlFX_volSyncDataType);
		if (doFetch)
		{
		    /*
		     *  If the read request was for 0 bytes, we basically
		     *  set this variable to the empty string.
		     */
		    if (length)
			dataP = itl_SetVectorType(dataBuffer, length);
		    else
			dataP = itl_SetVectorType(dataBuffer, 1);
		    assert(dataP);
		    if (!itl_SetVariable(fetchVarName, dataP, var_local))
			itl_Error("Failed to set variable, %s\n", 
				  fetchVarName);
		    itl_FreeVarVal(dataP);
		}
	    }

	    if (doFetch && length)
		ITL_FREE(dataBuffer);

	    ITL_FX_PIPE_BUF_FREE(&pipeState);
	    ITL_FREE(fileStatusName);
	    ITL_FREE(volSyncName);
	}
	else
	    itl_Error("Could NOT obtain valid file identifier.\n");
    }
    else
	itl_Error("Could NOT obtain valid fileset handle.\n");

    return(FALSE);
}

/* 
 *  itlFX_StatusFetchOrStore() --
 *    Issue the AFS_FetchStatus() or AFS_StoreStatus() RPC.
 */
SHARED boolean_t itlFX_StatusFetchOrStore(doFetch)
  IN boolean_t doFetch;   /* TRUE do AFS_FetchStatus() */
{
    itlFX_connect_t *connectInfoP;
    int strLength;

    if ((connectInfoP = itlFX_ConnectGet()))
    {
	afsFid *fileIdP;

	if (fileIdP = itlCommon_FidGet())
	{
	    error_status_t returnCode;
	    char *fileStatusName, *volSyncName;
	    afs_hyper_t minVV;
	    afs_token_t fileToken;
	    afsVolSync fileSync;
	    afsFetchStatus fileStatus;
	    afsStoreStatus inStatus;

	    /*
	     *  Obtain necessary input data for the function call.
	     */
	    itl_AdviseUser("minVV Value -\n");
	    itlCommon_HyperGet(&minVV);
 
	    if (!doFetch)
	    {
		itlFX_StoreStatusGet(&inStatus);
	    }

	    ITL_FX_GET_STRING("Status Variable (fs_fxFetchStatus): ", 
			      fileStatusName, &strLength);
	    ITL_FX_GET_STRING("Volume Sync Variable (fs_fxVolSync): ",
			      volSyncName, &strLength);

	    if (doFetch)
	    {
		returnCode = AFS_FetchStatus(ITL_FX_CONNECT_HANDLE(connectInfoP),
					     fileIdP, (afs_hyper_t *) &minVV, 
					     ITL_FX_NO_TOKEN_MGMT, &fileStatus,
					     &fileToken, &fileSync);
	    }
	    else
	    {
		returnCode = AFS_StoreStatus(ITL_FX_CONNECT_HANDLE(connectInfoP),
					     fileIdP, &inStatus, 
					     (afs_hyper_t *) &minVV, 
					     ITL_FX_NO_TOKEN_MGMT,
					     &fileStatus, &fileSync);
	    }
	    ITL_FX_CHECK_ERROR(returnCode);

	    /*
	     *  If the call was successful, then create the necessary
	     *  variables.
	     */
	    if (returnCode == error_status_ok)
	    {
		ITLU_ASSIGN_VARIABLE(fileStatusName, &fileStatus,
				    itlFX_fetchStatusDataType);
		ITLU_ASSIGN_VARIABLE(volSyncName, &fileSync,
				    itlFX_fetchStatusDataType);
	    }

	    ITL_FREE(fileStatusName);
	    ITL_FREE(volSyncName);
	}
	else
	    itl_Error("Could NOT obtain valid file identifier.\n");
    }
    else
	itl_Error("Could NOT obtain valid fileset handle.\n");

    return(FALSE);
}

/* 
 *  itlFX_MakeLink() --
 *    Issue the AFS_Symlink() or AFS_HardLink() RPC.
 */
SHARED boolean_t itlFX_MakeLink(makeSymbolic)
  IN boolean_t makeSymbolic;   /* TRUE do AFS_Symlink() */
{
    itlFX_connect_t *connectInfoP;
    int strLength;
    boolean_t gotArgs = TRUE;

    if ((connectInfoP = itlFX_ConnectGet()))
    {
	afsFid *fileIdP;

	if (fileIdP = itlCommon_FidGet())
	{
	    error_status_t returnCode;
	    char *fileStatusName, *dirStatusName, *volSyncName;
	    char *fileName, *fidName, *pathName;
	    afs_hyper_t minVV;
	    afsFid fileId, *linkFidP;
	    afs_token_t fileToken;
	    afsVolSync fileSync;
	    afsFetchStatus fileStatus, dirStatus;
	    afsStoreStatus inStatus;
	    afsTaggedName nameInfo;
	    afsTaggedPath pathInfo;

	    /*
	     *  Obtain necessary input data for the function call.
	     */
	    ITL_FX_GET_STRING("File Name: ", fileName, &strLength);
	    if (strLength >= AFS_NAMEMAX)
	    {
		ITL_FX_NAME_ERROR(fileName);
		gotArgs = FALSE;
	    }
	    (void) memset((char *) &nameInfo, 0, sizeof(nameInfo));
	    strcpy((char*)(nameInfo.tn_chars), fileName);

	    if (makeSymbolic) 
	    {
		ITL_FX_GET_STRING("Path Name: ", pathName, &strLength);
		if (strLength >= AFS_PATHMAX)
		{
		    ITL_FX_PATH_ERROR(pathName);
		    gotArgs = FALSE;
		}
		(void) memset((char *) &pathInfo, 0, sizeof(pathInfo));
		strcpy((char *)(pathInfo.tp_chars), pathName);
		itlFX_StoreStatusGet(&inStatus);
	    }
	    else
	    {
		if ((linkFidP = itlCommon_FidGet()) == (afsFid *) 0)
		{
		    itl_Error("Could NOT obtain valid file identifier.\n");
		    gotArgs = FALSE;
		}
	    }

	    itl_AdviseUser("minVV Value -\n");
	    itlCommon_HyperGet(&minVV);

	    if (makeSymbolic)
		ITL_FX_GET_STRING("File Id Variable (fs_fxFid): ", fidName, 
				  &strLength);

	    ITL_FX_GET_STRING("File Status Variable (fs_fxFetchStatus): ", 
			      fileStatusName, &strLength);
	    ITL_FX_GET_STRING("Directory Status Varaible (fs_fxFetchStatus): ", 
			      dirStatusName, &strLength);
	    ITL_FX_GET_STRING("Volume Sync Variable (fs_fxVolSync): ", 
			      volSyncName, &strLength);

	    if (gotArgs)
	    {
		if (makeSymbolic)
		    returnCode = AFS_Symlink(ITL_FX_CONNECT_HANDLE(connectInfoP),
					     fileIdP, &nameInfo,
					     &pathInfo,
					     &inStatus, (afs_hyper_t *) &minVV, 
					     ITL_FX_NO_TOKEN_MGMT,
					     &fileId, &fileStatus, &dirStatus,
					     &fileToken, &fileSync);
		else
		    returnCode = AFS_HardLink(ITL_FX_CONNECT_HANDLE(connectInfoP),
					      fileIdP, &nameInfo,
					      linkFidP, (afs_hyper_t *) &minVV, 
					      ITL_FX_NO_TOKEN_MGMT,
					      &fileStatus, &dirStatus,
					      &fileSync);

		ITL_FX_CHECK_ERROR(returnCode);

		/*
		 *  If the call was successful, then create the necessary
		 *  variables.
		 */
		if (returnCode == error_status_ok)
		{
		    ITLU_ASSIGN_VARIABLE(fileStatusName, &fileStatus,
					itlFX_fetchStatusDataType);
		    ITLU_ASSIGN_VARIABLE(dirStatusName, &dirStatus,
					itlFX_fetchStatusDataType);
		    ITLU_ASSIGN_VARIABLE(volSyncName, &fileSync,
					itlFX_fetchStatusDataType);
		    if (makeSymbolic)
			ITLU_ASSIGN_VARIABLE(fidName, &fileId,
					    itlCommon_fidDataType);
		}
	    }

	    if (makeSymbolic)
	    {
		ITL_FREE(fidName);
		ITL_FREE(pathName);
	    }

	    ITL_FREE(fileStatusName);
	    ITL_FREE(dirStatusName);
	    ITL_FREE(volSyncName);
	}
	else
	    itl_Error("Could NOT obtain valid file identifier.\n");
    }
    else
	itl_Error("Could NOT obtain valid fileset handle.\n");

    return(FALSE);
}

/* 
 *  itlFX_Rename() --
 *    Issue the AFS_Rename() RPC.
 */
SHARED boolean_t itlFX_Rename()
{
    itlFX_connect_t *connectInfoP;
    int strLength;

    if ((connectInfoP = itlFX_ConnectGet()))
    {
	afsFid *oldDirFidP, *newDirFidP;

	if (oldDirFidP = itlCommon_FidGet())
	{
	    error_status_t returnCode;
	    char *oldDirStatusName, *newDirStatusName;
	    char *oldFidName, *newFidName;
	    char *oldFileStatusName, *newFileStatusName, *volSyncName;
	    afs_hyper_t minVV;
	    afsFid oldFileFid, newFileFid, *linkFidP;
	    afsVolSync volSync;
	    afsFetchStatus oldDirStatus, newDirStatus;
	    afsFetchStatus oldFileStatus, newFileStatus;
	    afsFidTaggedName oldNameInfo, newNameInfo;
	    afs_hyper_t returnTokenId;	    

	    itlFX_FidNameGet(&oldNameInfo);
	    if (newDirFidP = itlCommon_FidGet())
	    {
		itlFX_FidNameGet(&newNameInfo);
		/*
		 *  No token mamagement yet, so we just 0 out this arg.
		 */
		AFS_hzero(returnTokenId);

		itl_AdviseUser("minVV Value -\n");
		itlCommon_HyperGet(&minVV);

		ITL_FX_GET_STRING("Old Directory Status Varaible (fs_fxFetchStatus): ", 
				  oldDirStatusName, &strLength);
		ITL_FX_GET_STRING("New Directory Status Varaible (fs_fxFetchStatus): ", 
				  newDirStatusName, &strLength);
		ITL_FX_GET_STRING("Old File Id Variable (fs_fxFid): ", 
				  oldFidName, &strLength);
		ITL_FX_GET_STRING("Old File Status Variable (fs_fxFetchStatus): ", 
				  oldFileStatusName, &strLength);
		ITL_FX_GET_STRING("New File Status Variable (fs_fxFetchStatus): ", 
				  newFileStatusName, &strLength);
		ITL_FX_GET_STRING("New File Id Variable (fs_fxFid): ", 
				  newFidName, &strLength);
		ITL_FX_GET_STRING("Volume Sync Variable (fs_fxVolSync): ", 
				  volSyncName, &strLength);

		returnCode = AFS_Rename(ITL_FX_CONNECT_HANDLE(connectInfoP),
					oldDirFidP, &oldNameInfo, newDirFidP,
					&newNameInfo, &returnTokenId,
					(afs_hyper_t *) &minVV, 
					ITL_FX_NO_TOKEN_MGMT,
					&oldDirStatus, &newDirStatus,
					&oldFileFid, &oldFileStatus,
					&newFileFid, &newFileStatus, &volSync);

		ITL_FX_CHECK_ERROR(returnCode);

		/*
		 *  If the call was successful, then create the necessary
		 *  variables.
		 */
		if (returnCode == error_status_ok)
		{
		    ITLU_ASSIGN_VARIABLE(oldDirStatusName, &oldDirStatus,
					itlFX_fetchStatusDataType);
		    ITLU_ASSIGN_VARIABLE(newDirStatusName, &newDirStatus,
					itlFX_fetchStatusDataType);
		    ITLU_ASSIGN_VARIABLE(oldFidName, &oldFileFid,
					itlCommon_fidDataType);
		    ITLU_ASSIGN_VARIABLE(oldFileStatusName, &oldFileStatus,
					itlFX_fetchStatusDataType);
		    ITLU_ASSIGN_VARIABLE(newFileStatusName, &newFileStatus,
					itlFX_fetchStatusDataType);
		    ITLU_ASSIGN_VARIABLE(newFidName, &newFileFid,
					itlCommon_fidDataType);
		    ITLU_ASSIGN_VARIABLE(volSyncName, &volSync,
					itlFX_volSyncDataType);
		}

		ITL_FREE(oldDirStatusName);
		ITL_FREE(newDirStatusName);
		ITL_FREE(oldFileStatusName);
		ITL_FREE(newFileStatusName);
		ITL_FREE(oldFidName);
		ITL_FREE(newFidName);
		ITL_FREE(volSyncName);
	    }
	    else
		itl_Error("Could NOT obtain valid file identifier.\n");
	}
	else
	    itl_Error("Could NOT obtain valid file identifier.\n");
    }
    else
	itl_Error("Could NOT obtain valid fileset handle.\n");

    return(FALSE);
}



/* 
 *  itlFX_Readdir() --
 *    Issue the AFS_Readdir() RPC.
 */
SHARED boolean_t itlFX_Readdir()
{
    itlFX_connect_t *connectInfoP;
    int strLength;

    if ((connectInfoP = itlFX_ConnectGet()))
    {
	afsFid *fileIdP;

	if (fileIdP = itlCommon_FidGet())
	{
	    error_status_t returnCode;
	    char *dirStatusName, *volSyncName, *dataBuffer, *nextOffsetName;
	    char *dirDataVar, defaultBuffer;
	    unsigned32 offset, size, nextOffset;
	    afs_hyper_t minVV;
	    afs_token_t fileToken;
	    afsVolSync fileSync;
	    afsFetchStatus dirStatus;
	    pipe_t dataStream;
	    itlFX_pipeState_t pipeState;
	    var_val_t *dataP;
	    afs_hyper_t netInOffset, netOutOffset;

	    /*
	     *  Obtain necessary input data for the function call.
	     */
	    itl_PromptUser("Offset: ");
	    offset = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_PromptUser("Size: ");
	    size = (unsigned32) itl_GetInteger((boolean_t *) 0);
	    itl_AdviseUser("minVV Value -\n");
	    itlCommon_HyperGet(&minVV);

	    ITL_FX_GET_STRING("Next Offset Variable (int): ", 
			      nextOffsetName, &strLength);
	    AFS_hset64(netInOffset, 0, offset);
	    /*
	     *  Need to allocate a buffer to hold the data.  If the size
	     *  is 0, we "fake" it.
	     */
	    if (size)
		ITL_ALLOC(dataBuffer, char *, size);
	    else 
	    {
		defaultBuffer = '\0';
		dataBuffer = &defaultBuffer;
	    }

	    ITL_FX_GET_STRING("Directory Status Variable (fs_fxFetchStatus): ", 
			      dirStatusName, &strLength);
	    ITL_FX_GET_STRING("Volume Sync Variable (fs_fxVolSync): ",
			      volSyncName, &strLength);
	    ITL_FX_GET_STRING("Directory Data Variable (vector): ", 
			      dirDataVar, &strLength);

	    /*
	     *  Set up the pipe structure.
	     */
	    ITL_FX_PIPE_BUF_ALLOC(&pipeState);
	    ITL_FX_PIPE_SET_IO_BUF(&pipeState, dataBuffer, size);
	    dataStream.alloc = itlFX_PipeAlloc;
	    dataStream.state = (char *) &pipeState;
	    dataStream.push = itlFX_PipePush;

	    returnCode = AFS_Readdir(ITL_FX_CONNECT_HANDLE(connectInfoP),
				     fileIdP, &netInOffset, size, 
				     (afs_hyper_t *) &minVV, ITL_FX_NO_TOKEN_MGMT,
				     &netOutOffset, &dirStatus, &fileToken,
				     &fileSync, &dataStream);

	    ITL_FX_CHECK_ERROR(returnCode);
	    nextOffset = AFS_hgetlo(netOutOffset);

	    /*
	     *  If the call was successful, then create the necessary
	     *  variables.
	     */
	    if (returnCode == error_status_ok)
	    {
		ITLU_ASSIGN_VARIABLE(nextOffsetName, &nextOffset, itl_intType);
		ITLU_ASSIGN_VARIABLE(dirStatusName, &dirStatus,
				    itlFX_fetchStatusDataType);
		ITLU_ASSIGN_VARIABLE(volSyncName, &fileSync,
				    itlFX_volSyncDataType);

		/*
		 *  If size was zero, then just create an empty string.
		 */
		if (size)
		    dataP = itl_SetVectorType(dataBuffer, size);
		else
		    dataP = itl_SetVectorType(dataBuffer, 1);
		assert(dataP);
		if (!itl_SetVariable(dirDataVar, dataP, var_local))
		    itl_Error("Failed to set variable, %s\n", dirDataVar);
		itl_FreeVarVal(dataP);
	    }

	    if (size)
		ITL_FREE(dataBuffer);
	    ITL_FX_PIPE_BUF_FREE(&pipeState);
	    ITL_FREE(nextOffsetName);
	    ITL_FREE(dirStatusName);
	    ITL_FREE(volSyncName);
	    ITL_FREE(dirDataVar);
	}
	else
	    itl_Error("Could NOT obtain valid file identifier.\n");
    }
    else
	itl_Error("Could NOT obtain valid fileset handle.\n");

    return(FALSE);
}


