/*
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
 * itl_bakCommands.h --
 *    Interface file for the ITL backup server test functions.
 */

#ifndef ITL_BAK_COMMANDS_H
#define ITL_BAK_COMMANDS_H

#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_bakListType.h>

/*
 *  Define all the useful bak API commands.
 */

#define ITL_BAK_LOGIN_CMD                                                    \
 {"dceLogin", itlBak_DceLogin, FALSE, FALSE, COMMAND_MODE_ALL,               \
      "Create a DCE login context.",                                         \
      "bak",                                                                 \
      "Create a DCE principal by passing in a name and password."}

#define ITL_BAK_SERVER_CONNECT_CMD                                           \
 {"bakServerConnect", itlBak_ServerConnect, FALSE, FALSE, COMMAND_MODE_ALL,  \
      "Make a connection to a backup server.",                               \
      "bak",                                                                 \
      ""}

#define ITL_BAK_ADD_VOLUME_CMD                                               \
 {"bakAddVolume", itlBak_AddVolume, FALSE, FALSE, COMMAND_MODE_ALL,          \
      "Add a volume to a dump and tape.",                                    \
      "bak",                                                                 \
      ""}

#define ITL_BAK_CREATE_DUMP_CMD                                              \
 {"bakCreateDump", itlBak_CreateDump, FALSE, FALSE, COMMAND_MODE_ALL,        \
      "Create a new dump.",                                                  \
      "bak",                                                                 \
      ""}

#define ITL_BAK_DELETE_DUMP_CMD                                              \
 {"bakDeleteDump", itlBak_DeleteDump, FALSE, FALSE, COMMAND_MODE_ALL,        \
      "Delete an existing dump.",                                            \
      "bak",                                                                 \
      ""}

#define ITL_BAK_DELETE_TAPE_CMD                                              \
 {"bakDeleteTape", itlBak_DeleteTape, FALSE, FALSE, COMMAND_MODE_ALL,        \
      "Delete a tape from the database.",                                    \
      "bak",                                                                 \
      ""}

#define ITL_BAK_DELETE_VDP_CMD                                               \
 {"bakDeleteVDP", itlBak_DeleteVDP, FALSE, FALSE, COMMAND_MODE_ALL,          \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_FIND_CLONE_CMD                                               \
 {"bakFindClone", itlBak_FindClone, FALSE, FALSE, COMMAND_MODE_ALL,          \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_FIND_DUMP_CMD                                                \
 {"bakFindDump", itlBak_FindDump, FALSE, FALSE, COMMAND_MODE_ALL,            \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_FIND_LATEST_DUMP_CMD                                         \
 {"bakFindLatestDump", itlBak_FindLatestDump, FALSE, FALSE, COMMAND_MODE_ALL,\
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_FINISH_DUMP_CMD                                              \
 {"bakFinishDump", itlBak_FinishDump, FALSE, FALSE, COMMAND_MODE_ALL,        \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_FINISH_TAPE_CMD                                              \
 {"bakFinishTape", itlBak_FinishTape, FALSE, FALSE, COMMAND_MODE_ALL,        \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_GET_DUMPS_CMD                                                \
 {"bakGetDumps", itlBak_GetList, bakDumpList, FALSE, COMMAND_MODE_ALL,       \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_GET_TAPES_CMD                                                \
 {"bakGetTapes", itlBak_GetList, bakTapeList, FALSE, COMMAND_MODE_ALL,       \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_GET_VOLUMES_CMD                                              \
 {"bakGetVolumes", itlBak_GetList, bakVolumeList, FALSE, COMMAND_MODE_ALL,   \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_USE_TAPE_CMD                                                 \
 {"bakUseTape", itlBak_UseTape, FALSE, FALSE, COMMAND_MODE_ALL,              \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_GET_TEXT_CMD                                                 \
 {"bakGetText", itlBak_GetText, FALSE, FALSE, COMMAND_MODE_ALL,              \
      "Get a text segment from the backup server.",                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_GET_TEXT_VERSION_CMD                                         \
 {"bakGetTextVersion", itlBak_GetTextVersion, FALSE, FALSE, COMMAND_MODE_ALL,\
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_SAVE_TEXT_CMD                                                \
 {"bakSaveText", itlBak_SaveText, FALSE, FALSE, COMMAND_MODE_ALL,            \
      "Store a text segment in the backup server.",                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_FREE_ALL_LOCKS_CMD                                           \
 {"bakFreeAllLocks", itlBak_FreeAllLocks, FALSE, FALSE, COMMAND_MODE_ALL,    \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_FREE_LOCK_CMD                                                \
 {"bakFreeLock", itlBak_FreeLock, FALSE, FALSE, COMMAND_MODE_ALL,            \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_GET_INSTANCE_ID_CMD                                          \
 {"bakGetInstanceId", itlBak_GetInstanceId, FALSE, FALSE, COMMAND_MODE_ALL,  \
      "Get an instance identifier from backup server.",                      \
      "bak",                                                                 \
      ""}

#define ITL_BAK_GET_LOCK_CMD                                                 \
 {"bakGetLock", itlBak_GetLock, FALSE, FALSE, COMMAND_MODE_ALL,              \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_DB_VERIFY_CMD                                                \
 {"bakDbVerify", itlBak_DbVerify, FALSE, FALSE, COMMAND_MODE_ALL,            \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_DUMP_DB_CMD                                                  \
 {"bakDumpDb", itlBak_DumpDB, FALSE, FALSE, COMMAND_MODE_ALL,                \
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

#define ITL_BAK_RESTORE_DB_HEADER_CMD                                        \
 {"bakRestoreDbHeader", itlBak_RestoreDbHeader, FALSE, FALSE, COMMAND_MODE_ALL,\
      "Ask Vijay.",                                                          \
      "bak",                                                                 \
      ""}

/*
 *  ITL_BAK_CHECK_ERROR() --
 *    Macro for checking return codes from bak API calls.
 */
#define ITL_BAK_CHECK_ERROR(_errCode)                                       \
        MACRO_BEGIN                                                         \
          if ((_errCode) != 0)                                              \
            (void) itlu_CheckResult(_errCode);                              \
        MACRO_END

extern void itlBak_AddBakCommands _TAKES((
					  void
					  ));

extern boolean_t itlBak_DceLogin _TAKES((
					 void
					 ));

extern boolean_t itlBak_ServerConnect _TAKES((
					      void
					      ));

extern boolean_t itlBak_AddVolume _TAKES((
					  void
					  ));

extern boolean_t itlBak_CreateDump _TAKES((
					   void
					   ));

extern boolean_t itlBak_DeleteDump _TAKES((
					   void
					   ));

extern boolean_t itlBak_DeleteTape _TAKES((
					   void
					   ));

extern boolean_t itlBak_DeleteVDP _TAKES((
					  void
					  ));

extern boolean_t itlBak_FindClone _TAKES((
					  void
					  ));

extern boolean_t itlBak_FindDump _TAKES((
					 void
					 ));

extern boolean_t itlBak_FindLatestDump _TAKES((
					       void
					       ));

extern boolean_t itlBak_FinishDump _TAKES((
					   void
					   ));

extern boolean_t itlBak_FinishTape _TAKES((
					   void
					   ));

extern boolean_t itlBak_GetList _TAKES((
					IN itlBak_listTypes_t listType
					));

extern boolean_t itlBak_UseTape _TAKES((
					void
					));

extern boolean_t itlBak_GetText _TAKES((
					void
					));

extern boolean_t itlBak_GetTextVersion _TAKES((
					       void
					       ));

extern boolean_t itlBak_SaveText _TAKES((
					 void
					 ));

extern boolean_t itlBak_FreeAllLocks _TAKES((
					     void
					     ));

extern boolean_t itlBak_FreeLock _TAKES((
					 void
					 ));

extern boolean_t itlBak_GetInstanceId _TAKES((
					      void
					      ));

extern boolean_t itlBak_GetLock _TAKES((
					void
					));

extern boolean_t itlBak_DbVerify _TAKES((
					 void
					 ));

extern boolean_t itlBak_DumpDB _TAKES((
				       void
				       ));

extern boolean_t itlBak_RestoreDbHeader _TAKES((
						void
						));

#endif /* ITL_BAK_COMMANDS_H */
