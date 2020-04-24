/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: itl_cm.h,v $
 * Revision 1.1.6.1  1996/10/17  18:25:15  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:43  damon]
 *
 * $EndLog$
 */
/*
 * itl_cm.h -- 
 *    Definitions for itl_cm.c.
 *
 *    Contains commands that a tester might want for writing 
 *    test programs involving some of pioctl function calls.
 */

#ifndef ITL_CM_H
#define ITL_CM_H

#include <utils/itl/itl.h>
#include <utils/itl/itl_utils.h>
#include <utils/itl/itl_pioctl.h>

/*
 *  Define all the useful pioctl commands.
 */

#define ITL_CM_FLUSH_FILE_CMD                                             \
 {"flush_file", itlCM_FlushFile, TRUE, FALSE, COMMAND_MODE_ALL,         \
         "Flush a file.",                                       \
         "cm",                                                      \
         ""}
#define ITL_CM_FLUSH_VOLUME_CMD                                            \
 {"flush_fileset", itlCM_FlushVolume, FALSE, FALSE, COMMAND_MODE_ALL,       \
         "Flush a volume.",                                        \
         "cm",                                                      \
         ""}
#define ITL_CM_SET_CACHE_SIZE_CMD                                            \
 {"get_cache_size", itlCM_GetCacheSize, FALSE, FALSE, COMMAND_MODE_ALL,       \
         "Get caceh size.",                                        \
         "cm",                                                      \
         ""}
#define ITL_CM_GET_CACHE_SIZE_CMD                                            \
 {"set_cache_size", itlCM_SetCacheSize, FALSE, FALSE, COMMAND_MODE_ALL,       \
         "Set caceh size.",                                        \
         "cm",                                                      \
         ""}
#define ITL_CM_GET_VOL_STAT_CMD                                            \
 {"get_fileset_stat", itlCM_GetVolStat, FALSE, FALSE, COMMAND_MODE_ALL,       \
         "Get volume stat.",                                        \
         "cm",                                                      \
         ""}
#define ITL_CM_SET_VOL_STAT_CMD                                            \
 {"set_fileset_stat", itlCM_SetVolStat, FALSE, FALSE, COMMAND_MODE_ALL,       \
         "Set volume stat.",                                        \
         "cm",                                                      \
         ""}
#define ITL_CM_PREFETCH_CMD                                            \
 {"prefetch_file", itlCM_Prefetch, FALSE, FALSE, COMMAND_MODE_ALL,       \
         "Prefetch a file.",                                        \
         "cm",                                                      \
         ""}
#define ITL_CM_CHECK_FILESET_CMD                                            \
 {"check_fileset", itlCM_CheckFileset, FALSE, FALSE, COMMAND_MODE_ALL,       \
         "Check fileset.",                                        \
         "cm",                                                      \
         ""}

extern itl_commandTable_t itlFile_CommandTable[];


extern void itlCM_AddCMCommands _TAKES((
					    void
					    ));

extern boolean_t itlCM_FlushFile _TAKES((
					    void
					     ));

extern boolean_t itlCM_FlushVolume _TAKES((
				       void
				       ));

extern boolean_t itlCM_SetCacheSize _TAKES((
				       void
				       ));
extern boolean_t itlCM_GetCacheSize _TAKES((
				       void
				       ));

extern boolean_t itlCM_GetVolStat _TAKES((
				       void
				       ));
extern boolean_t itlCM_SetVolStat _TAKES((
				       void
				       ));
extern boolean_t itlCM_Prefetch _TAKES((
				       void
				       ));
extern boolean_t itlCM_CheckFileset _TAKES((
				       void
				       ));

#endif   /* ITL_CM_H */
