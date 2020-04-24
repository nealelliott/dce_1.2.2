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
 * itl_fxFileset.c -- 
 *    C functions for implementing the fileset hash table.
 */
/*
 * HISTORY
 * $Log: itl_fxFileset.c,v $
 * Revision 1.1.320.1  1996/10/17  18:26:31  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:20  damon]
 *
 * $EndLog$
 */

#include <pthread.h>
#include <dcedfs/common_data.h>
#include <dcedfs/afsvl_data.h>
#include <dcedfs/lock.h>
#include <dcedfs/nubik.h>
#include <dcedfs/fldb_proc.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_utilsNameHash.h>
#include <utils/itl/itl_fxDCEError.h>
#include <utils/itl/itl_fxCommon.h>
#include <utils/itl/itl_fxServer.h>
#include <utils/itl/itl_fxFileset.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxFileset.c,v 1.1.320.1 1996/10/17 18:26:31 damon Exp $")

PRIVATE unsigned int filesetHashType;

/*
 *  FilesetInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void FilesetInit()
{
    /*
     *  We can safely set filesetHashType because this function is called
     *  via pthread_once.
     */
    filesetHashType = itlu_NameHashGetType();
}

/*
 * itlFX_FilesetInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlFX_FilesetInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    /*
     *  First ensure the hash table module is initialized.
     */
    itlu_NameHashInit();

    if (pthread_once(&hashOnce, (pthread_initroutine_t) FilesetInit))
	itl_Error("pthread_once failed in FilesetInit\n");
}

/*
 * itlFX_FilesetAddName() --
 *    Adds a fileset name and related info to the table.
 *
 * RETURN CODES: 
 *    FALSE - Successful addition.
 *    TRUE  - Unsuccessful addition, fileset name exists or is invalid.
 */
SHARED boolean_t itlFX_FilesetAddName(nameP)
  IN char *nameP;       /* fileset's name */
{
    boolean_t retVal = FALSE;
    struct vldbentry vldbInfo;
    error_status_t errorStatus;
    long returnCode;

    if ((returnCode = ubik_Call(VL_GetEntryByName, ITL_FX_CELL_UBIK_INFO,
				0, nameP, &vldbInfo))
	           == error_status_ok)
    {
	itlFX_fileset_t *filesetP;
	itlFX_server_t *serverP;

	ITL_UTIL_NEW(filesetP, itlFX_fileset_t);
	filesetP->filesetId = vldbInfo.VolIDs[0];

	/*
	 *  If this server does not exist then create it.  Two lookups are
	 *  done to avoid any race conditions.
	 */
	if (!(serverP = itlFX_ServerLookup((char *) 
					   vldbInfo.sitePrincipal[0].text)))
	{
	    (void) itlFX_ServerAddName(&vldbInfo);
	    serverP = itlFX_ServerLookup((char *)
					 vldbInfo.sitePrincipal[0].text);
	}

	if (serverP)
	{
	    filesetP->serverP = serverP;
	    retVal = itlu_NameHashAdd(nameP, filesetHashType,
				      (void *) filesetP);
	}
	else
	{
	    itl_Error("Could NOT find or create a server entry for, %s\n",
		      vldbInfo.sitePrincipal[0].text);
	    retVal = TRUE;
	}

	if (retVal)
	    ITL_UTIL_FREE(filesetP);
    }
    else
    {
	ITL_FX_DCE_ERROR(returnCode, "VL_GetEntryByName FAILED");
	retVal = TRUE;
    }

    return(retVal);
}

/*
 * itlFX_FilesetRemoveName() --
 *    Delete a fileset name from the table.
 *
 * RETURN CODES: void
 */
SHARED void itlFX_FilesetRemoveName(nameP)
  IN char *nameP;   /* name of fileset to remove */
{
    itlFX_fileset_t *entryP;

    if ((entryP = (itlFX_fileset_t *) itlu_NameHashDelete(nameP, 
							  filesetHashType))
	       != (itlFX_fileset_t *) 0)
	ITL_FREE(entryP);
    else 
	itl_Message("\nWARNING: Fileset, %s, NOT in table.\n\n", nameP);
}

/*
 * itlFX_FilesetLookup() --
 *    Given a fileset name, lookup and return the pointer to 
 *    itlFX_fileset_t that describes that fileset.
 *
 * RETURN CODES: 
 *    NULL - if no entry is found.
 *    Otherwise success.
 */
SHARED itlFX_fileset_t *itlFX_FilesetLookup(nameP)
  IN char *nameP;                /* the fileset name to compare with */
{
    itlu_NameHashTable_t *entryP;
    itlFX_fileset_t *retVal = (itlFX_fileset_t *) 0;

    if ((entryP = itlu_NameHashLookup(nameP, filesetHashType))
	       != ITLU_NAME_HASH_NULL)
    {
	retVal = ITLU_NAME_HASH_GET_DATA(entryP, itlFX_fileset_t);
	itlu_NameHashRelease(entryP);
    }

    return(retVal);
}



