/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fxAPI.h -- 
 *    Declarations for functions in itl_fxAPI.h.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxAPI.h,v 1.1.11.1 1996/10/17 18:26:12 damon Exp $
 */
/*
 * HISTORY
 * $Log: itl_fxAPI.h,v $
 * Revision 1.1.11.1  1996/10/17  18:26:12  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:12  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:44:12  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:23:58  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:18  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:44  annie]
 * 
 * Revision 1.1.2.3  1993/02/16  16:36:00  jaffe
 * 	Transarc delta: fred-ot6925-make-fx-tests-use-correct-uuids 1.2
 * 	  Selected comments:
 * 	    Had to change the FX test program to conform with Mike Kazar's changes
 * 	    to the AFS_SetContext() call.  Now the test program calls dfsuuid_Create
 * 	    to generate correct uuid's for the primary and secondary interfaces.
 * 	    Removed the invocation of the DCE_ERROR macro from the ITL_FX_CHECK_ERROR
 * 	    macro.
 * 	[1993/02/15  19:32:14  jaffe]
 * 
 * Revision 1.1.2.2  1993/01/13  21:08:57  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	[1993/01/13  14:09:32  shl]
 * 
 * $EndLog$
 */

#ifndef ITL_FX_API_H
#define ITL_FX_API_H

#include <dcedfs/osi_dfserrors.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_fxDCEError.h>

typedef enum {
    itlFX_createFile,
    itlFX_createDir,
    itlFX_removeFile,
    itlFX_removeDir
} itlFX_fileDirOp_t;

/*
 *  Define all the useful FX API commands.
 */

#define ITL_FX_LOGIN_CMD                                                   \
 {"dceLogin", itlFX_DceLogin, FALSE, FALSE, COMMAND_MODE_ALL,              \
      "Create a DCE login context.",                                       \
      "fx",                                                                \
      "Create a DCE principal by passing in a name and password."}

#define ITL_FX_FILESET_CONNECT_CMD                                         \
 {"filesetConnect", itlFX_FilesetConnect, FALSE, FALSE, COMMAND_MODE_ALL,  \
      "Make a connection to a fileset (server).",                          \
      "fx",                                                                \
      ""}

#define ITL_FX_USER_CONNECT_CMD                                            \
 {"userConnect", itlFX_UserConnect, FALSE, FALSE, COMMAND_MODE_ALL,        \
      "Make a user connection to a fileset (server).",                     \
      "fx",                                                                \
      ""}

#define ITL_FX_SET_CONTEXT_CMD                                             \
 {"fxSetContext", itlFX_SetContext, FALSE, FALSE, COMMAND_MODE_ALL,        \
      "Establish a context with a DFS file exporter.",                     \
      "fx",                                                                \
      ""}

#define ITL_FX_GET_TIME_CMD                                                \
 {"fxGetTime", itlFX_GetTime, FALSE, FALSE, COMMAND_MODE_ALL,              \
      "Get the time from the DFS file exporter.",                          \
      "fx",                                                                \
      ""}

#define ITL_FX_LOOKUP_ROOT_CMD                                             \
 {"fxLookupRoot", itlFX_Lookup, TRUE, FALSE, COMMAND_MODE_ALL,             \
      "Lookup the root file id in a DFS fileset.",                         \
      "fx",                                                                \
      ""}

#define ITL_FX_LOOKUP_CMD                                                  \
 {"fxLookup", itlFX_Lookup, FALSE, FALSE, COMMAND_MODE_ALL,                \
      "Lookup a file id in a DFS fileset.",                                \
      "fx",                                                                \
      ""}

#define ITL_FX_CREATE_FILE_CMD                                             \
 {"fxCreateFile", itlFX_CreateOrRemove, itlFX_createFile, FALSE, COMMAND_MODE_ALL,     \
      "Create a new file.",                                                \
      "fx",                                                                \
      ""}

#define ITL_FX_MAKE_DIR_CMD                                             \
 {"fxMakeDir", itlFX_CreateOrRemove, itlFX_createDir, FALSE, COMMAND_MODE_ALL,     \
      "Create a new directory.",                                           \
      "fx",                                                                \
      ""}

#define ITL_FX_REMOVE_FILE_CMD                                             \
 {"fxRemoveFile", itlFX_CreateOrRemove, itlFX_removeFile, FALSE, COMMAND_MODE_ALL,     \
      "Remove a file.",                                                \
      "fx",                                                                \
      ""}

#define ITL_FX_REMOVE_DIR_CMD                                             \
 {"fxRemoveDir", itlFX_CreateOrRemove, itlFX_removeDir, FALSE, COMMAND_MODE_ALL,     \
      "Remove a directory.",                                                \
      "fx",                                                                \
      ""}

#define ITL_FX_FETCH_DATA_CMD                                              \
 {"fxFetchData", itlFX_DataFetchOrStore, TRUE, FALSE, COMMAND_MODE_ALL,    \
      "Fetch data from a file (read).",                                    \
      "fx",                                                                \
      ""}

#define ITL_FX_STORE_DATA_CMD                                              \
 {"fxStoreData", itlFX_DataFetchOrStore, FALSE, FALSE, COMMAND_MODE_ALL,   \
      "Store data to a file (write).",                                     \
      "fx",                                                                \
      ""}

#define ITL_FX_FETCH_STATUS_CMD                                               \
 {"fxFetchStatus", itlFX_StatusFetchOrStore, TRUE, FALSE, COMMAND_MODE_ALL,  \
      "Fetch the status of file.",                                            \
      "fx",                                                                   \
      ""}

#define ITL_FX_STORE_STATUS_CMD                                               \
 {"fxStoreStatus", itlFX_StatusFetchOrStore, FALSE, FALSE, COMMAND_MODE_ALL, \
      "Store the status of file.",                                            \
      "fx",                                                                   \
      ""}

#define ITL_FX_SYM_LINK_CMD                                                   \
 {"fxSymlink", itlFX_MakeLink, TRUE, FALSE, COMMAND_MODE_ALL,                 \
      "Create a symbolic link to a file.",                                    \
      "fx",                                                                   \
      ""}

#define ITL_FX_HARD_LINK_CMD                                                  \
 {"fxHardlink", itlFX_MakeLink, FALSE, FALSE, COMMAND_MODE_ALL,               \
      "Create a hard link to an existing file.",                              \
      "fx",                                                                   \
      ""}

#define ITL_FX_RENAME_CMD                                                     \
 {"fxRename", itlFX_Rename, FALSE, FALSE, COMMAND_MODE_ALL,                   \
      "Rename an existing file.",                                             \
      "fx",                                                                   \
      ""}

#define ITL_FX_READDIR_CMD                                                    \
 {"fxReaddir", itlFX_Readdir, FALSE, FALSE, COMMAND_MODE_ALL,                 \
      "Read a directory entry.",                                              \
      "fx",                                                                   \
      ""}


/*
 *  ITL_FX_CHECK_ERROR() --
 *    Macro for checking return codes from API calls.
 */
#define ITL_FX_CHECK_ERROR(_errCode)                                        \
        MACRO_BEGIN                                                         \
           error_status_t realError;                                        \
                                                                            \
          if ((_errCode) != 0) {                                            \
            realError = osi_errDecode((_errCode));                          \
            (void) itlu_CheckResult(realError);                             \
	  }                                                                 \
        MACRO_END


/*
 * External data types.
 */

extern itl_commandTable_t itlFX_CommandTable[];

/*
 *  Exported Functions
 */

extern void itlFX_AddFXCommands _TAKES((
					void
					));

extern boolean_t itlFX_DceLogin _TAKES((
					void
					));

extern boolean_t itlFX_FilesetConnect _TAKES((
					      void
					      ));

extern boolean_t itlFX_UserConnect _TAKES((
					   void
					   ));

extern boolean_t itlFX_SetContext _TAKES((
					  void
					  ));

extern boolean_t itlFX_GetTime _TAKES((
				       void
				       ));

extern boolean_t itlFX_Lookup _TAKES((
				      IN boolean_t dorRootLookup
				      ));

extern boolean_t itlFX_CreateOrRemove _TAKES((
					      IN itlFX_fileDirOp_t rpcOp
					      ));

extern boolean_t itlFX_DataFetchOrStore _TAKES((
						IN boolean_t doFetch
						));

extern boolean_t itlFX_StatusFetchOrStore _TAKES((
						  IN boolean_t doFetch
						  ));

extern boolean_t itlFX_MakeLink _TAKES((
					IN boolean_t makeSymbolic
					));

extern boolean_t itlFX_Rename _TAKES((
				      IN void
				      ));

extern boolean_t itlFX_Readdir _TAKES((
				       IN void
				       ));

#endif   /* ITL_FX_API_H */
