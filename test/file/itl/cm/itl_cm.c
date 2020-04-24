/*
 * @OSF_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: itl_cm.c,v $
 * Revision 1.1.6.1  1996/10/17  18:25:13  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:52:43  damon]
 *
 * $EndLog$
 */
/*
 * itl_cm.c -- 
 *    Code for pioctl interface functions. This library is set up for
 *  the other test modles 
 */
#include <assert.h>
#include <errno.h>
#include <pthread.h>

#include <dcedfs/ioctl.h>
#include <utils/itl/itl_cm.h>

PRIVATE char itlCMInfoMsg[] = 
"The ITL CM module provides more meaningful commands interface to pioctl function calls such as flush file, flush volume etc. It is expandable as needed later on.";


/*
 *  Define command table entries supported by the functions in this file.
 */
EXPORT itl_commandTable_t itlCM_CommandTable[] = {
    ITL_CM_FLUSH_FILE_CMD,
    ITL_CM_FLUSH_VOLUME_CMD,
    ITL_CM_GET_CACHE_SIZE_CMD,
    ITL_CM_SET_CACHE_SIZE_CMD,
    ITL_CM_GET_VOL_STAT_CMD,
    ITL_CM_SET_VOL_STAT_CMD,
    ITL_CM_PREFETCH_CMD,
    ITL_CM_CHECK_FILESET_CMD
};

/* Used for VIOCGETVOLSTAT pioctl call */
PRIVATE itlu_valueMap_t filesetStatFlag[] = {
    {"VIOC_DEF_SGIDOK",   VIOC_DEF_SGIDOK},
    {"VIOC_DEF_DEVOK",   VIOC_DEF_DEVOK},
    {NULL,                  0}
};


/*
 *  AddCMCommands() --
 *    Private function that really adds the pioctl commands.  Necessary to
 *    accomodate how pthreads does one time initialization.
 */
PRIVATE void AddCMCommands()
{
    long returnCode = 0;

    /*
     *  Ensure the ITL utilities package is  initialized.
     */
    if (itlu_Init())
    {
        itl_Error("ITL utilities did not initialize, exiting.\n");
        exit(-1);
    }
    returnCode = itlu_ProcessValueMap(filesetStatFlag, NULL);
    if(returnCode)
    {
        itl_Error("itlu_ProcessValueMap FAILED - %ld\n", returnCode);
        exit(-1);
    }

    (void) itl_AddModuleCmds("cm",
			     itlCM_CommandTable,
			     sizeof(itlCM_CommandTable) / 
			     sizeof(itl_commandTable_t),
			     COMMAND_MODE_ALL,
			     itlCMInfoMsg);
}

/*
 *  itlFile_AddCMCommands() -
 *    Do everything necessary to make the itlCM commands accessable via 
 *    the testing interface.
 */
SHARED void itlCM_AddCMCommands()
{
    static pthread_once_t addCMOnce = pthread_once_init;

    if (pthread_once(&addCMOnce, (pthread_initroutine_t) AddCMCommands))
	itl_Error("pthread_once failed in AddCMCommands\n");
}

/*
 *  itlCM_FlushFile() --
 *    This function issues the pioctl flash file call.
 */
SHARED boolean_t itlCM_FlushFile()
{
    int link=0;
    return(itlPIOCTL_Flush(link));
}

/*
 *  itlCM_FlushVolume() --
 *    This function issues the pioctl flush volume call.
 */
SHARED boolean_t itlCM_FlushVolume()
{
    int link=0;
    return(itlPIOCTL_VolFlush(link));
}

/*
 *  itlCM_SetCacheSize() --
 *    call pioctl function cm_PBogus().
 */
SHARED boolean_t itlCM_SetCacheSize()
{
    int link=0;

    return (itlPIOCTL_SetCacheSize(link));
}

/*
 *  itlCM_GetCacheSize() --
 *    call pioctl function cm_PBogus().
 */
SHARED boolean_t itlCM_GetCacheSize()
{
    int link=0;

    return (itlPIOCTL_GetCacheParms(link));
}

/*
 *  itlCM_GetVolStat() --
 *    call pioctl function cm_PBogus().
 */
SHARED boolean_t itlCM_GetVolStat()
{
    int link=0;

    return (itlPIOCTL_GetVolStat(link));
}

/*
 *  itlCM_SetVolStat() --
 *    call pioctl function cm_PBogus().
 */
SHARED boolean_t itlCM_SetVolStat()
{
    int link=0;

    return (itlPIOCTL_SetVolStat(link));
}

/*
 *  itlCM_Prefetch() --
 *    call pioctl function itlPIOCTL_Prefetch().
 */
SHARED boolean_t itlCM_Prefetch()
{
    int link=0;

    return (itlPIOCTL_Prefetch(link));
}

/*
 *  itlCM_CheckFileset() --
 *    call pioctl function itlPIOCTL_itlPIOCTL_FlushFilesetNameIDMapping().
 */
SHARED boolean_t itlCM_CheckFileset()
{
    int link=0;

    return (itlPIOCTL_FlushFilesetNameIDMapping(link));
}
