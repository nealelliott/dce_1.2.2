/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 *
 * dfsSpoke.c --
 *    Interface routines for DFS specific commands used in the distributed
 *    ITL environment.
 *
 * HISTORY
 * $Log$
 * $EndLog$
 */

#include <dce/rpc.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_value.h>
#include <utils/itl/itl_file.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_fileUtils.h>
#include <utils/itl/itl_fileDir.h>
#include <utils/itl/itl_fileStat.h>
#include <utils/itl/defs.h>
#include <utils/itl/itl_spoke.h>
#include <utils/itl/dfsSpoke_defs.h>
#include <utils/itl/dfsSpoke_mgr.h>
#include <utils/itl/dfsSpoke_client.h>


PRIVATE char dfsSpoke_InfoMsg[] = 
"The ITL DFS spoke module provides command interfaces to do remote stat() and readdir() calls.  It will eventually be replaced.";


SHARED boolean_t dfsSpoke_Stat(arg)
  IN int arg;
{
    char		*fileName, *varName;
    handle_t		rpcHandle;
    struct stat 	statInfo;
    int 		length, err;
    boolean_t           retVal = FALSE;

    if (rpcHandle = itlSpoke_GetHandle())
    {
	itl_PromptUser("FileName: ");
	fileName = itl_GetString(NULL, &length);

	itl_PromptUser("Stat Variable Name: ");
	varName = itl_GetVariableName(&err, NULL);
    
	err = dfsSpokeClient_Stat(rpcHandle, fileName, &statInfo);

	ITLU_ASSIGN_VARIABLE(varName, &statInfo, itlFile_statDataType);
	ITL_UTIL_FREE(varName);
	itl_SetIntVariable("CommandResult", err, var_local);
    }
    else
	retVal = TRUE;

    return(retVal);
}

SHARED boolean_t dfsSpoke_Readdir(arg)
  IN int arg;
{
    char		*dirName;
    handle_t		rpcHandle;
    struct dirent	*direntP;
    int			i, err, length, numEntries;
    boolean_t           retVal = FALSE;

    if (rpcHandle = itlSpoke_GetHandle())
    {
	itl_PromptUser("Directory Name: ");
	dirName = itl_GetString(NULL, &length);

	err = dfsSpokeClient_Readdir(rpcHandle, dirName, &numEntries, &direntP);

	if (err == error_status_ok)
	{
	    itl_Message("Directory, %s, contains:\n", dirName);
	    for (i = 0; i < numEntries; i++) 
		itl_Message("%s\n", direntP[i].d_name);

	    ITL_FREE(direntP);
	}

	itl_SetIntVariable("CommandResult", err, var_local);
    }
    else
	retVal = TRUE;

    return(retVal);
}

typedef struct {
    struct stat		statInfo;
    char 		*fileName;
    boolean_t		identical;
} check_stat_info_t;

PRIVATE boolean_t StatAndCompare(rpcHandle, infoP, dataP)
  IN handle_t rpcHandle;    /* handle for rpc's to this client */
  IN pointer_t infoP;       /* appl. specific info from hash table */
  IN pointer_t dataP;       /* stat structure */
{
    struct stat		statInfo;
    int			err;
    boolean_t           retVal = TRUE;
    check_stat_info_t   *statP = (check_stat_info_t *) dataP;
    
    err = dfsSpokeClient_Stat(rpcHandle, statP->fileName, &statInfo);
    if (!err)
    {
	statP->identical = (statP->identical && 
			    dfsSpoke_StatCompare(&(statP->statInfo),
						 &statInfo));
    }
    else 
    {
	itl_Error("Stating file %s on spoke %s returned error: \n",
		  statP->fileName, (char *) infoP);
	DCE_ERROR(err, "Remote Stat FAILED");
	retVal = FALSE;
    }

    return(retVal);
}

EXPORT boolean_t dfsSpoke_CheckStat(arg)
  IN int arg;
{
    char 		*fileName;
    int			length, err;
    check_stat_info_t	localStat;
    boolean_t           retVal = FALSE;

    itl_PromptUser("FileName: ");
    fileName = itl_GetString(NULL, &length);

    err = stat(fileName, &(localStat.statInfo));
    localStat.identical = 1;
    localStat.fileName = fileName;
    
    if (itlSpoke_ForallClients(StatAndCompare, (pointer_t) &localStat))
    {
	if (localStat.identical) 
	{
	    itl_Message("All spokes have same view of stat for file %s\n", 
			fileName);
	}
	else
	{
	    itl_Error("Not all spokes have the same stat info for file %s\n", 
		      fileName);
	    retVal = TRUE;
	}
    }
    else
    {
	itl_Error("Failure in itlSpoke_ForAllClients\n");
	retVal = TRUE;
    }

    return(retVal);
}

PRIVATE itl_commandTable_t commandList[] = {
  {
    "rstat", dfsSpoke_Stat, 0, FALSE, 2, 
    "stat a file remotely",
    "spoke", "",
  },

  {
    "rreaddir", dfsSpoke_Readdir, 0, FALSE, 2, 
    "read a remote directory",
    "spoke", "",
  },

  {
    "check_stat", dfsSpoke_CheckStat, 0, FALSE, 2,
    "check the stat info for a file at each spoke",
    "spoke", "",
  }
};

PRIVATE void AddDFSSpokeCommands()
{
    if (!dfsSpokeMgr_Init())
    {
	(void) itl_AddModuleCmds("DFS spoke commands", commandList,
				 sizeof(commandList)/sizeof(commandList[0]),
				 2, dfsSpoke_InfoMsg);
    }
}

EXPORT void itlSpokeDFS_AddCommands()
{
    static pthread_once_t addOnce = pthread_once_init;

    if (pthread_once(&addOnce, (pthread_initroutine_t) AddDFSSpokeCommands))
	itl_Error("pthread_once failed in dfsSpoke_AddCommands\n");
}
