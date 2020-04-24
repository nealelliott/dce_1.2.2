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
 * itl_fxFileset.c -- 
 *    C functions for implementing the server hash table.
 * 
 */
/*
 * HISTORY
 * $Log: itl_fxServer.c,v $
 * Revision 1.1.11.1  1996/10/17  18:26:40  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:24  damon]
 *
 * Revision 1.1.6.1  1994/02/04  20:44:44  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:14  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:44:35  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:50  annie]
 * 
 * Revision 1.1.2.3  1993/02/16  16:36:16  jaffe
 * 	Transarc delta: fred-ot6925-make-fx-tests-use-correct-uuids 1.2
 * 	  Selected comments:
 * 	    Had to change the FX test program to conform with Mike Kazar's changes
 * 	    to the AFS_SetContext() call.  Now the test program calls dfsuuid_Create
 * 	    to generate correct uuid's for the primary and secondary interfaces.
 * 	    Replaced calls to uuid_create() with dfsuuid_Create().
 * 	[1993/02/15  19:35:07  jaffe]
 * 
 * Revision 1.1.2.2  1993/01/13  21:34:16  shl
 * 	Transarc delta: fred-ot-6350-add-fx-tests 1.3
 * 	  Selected comments:
 * 	    To place the DFS fole exporter tests into the tree.
 * 	    Migrate the fx exerciser to the new fx interface (OT 6054)
 * 	    Fix compilation problems.
 * 	[1993/01/13  14:35:38  shl]
 * 
 * $EndLog$
 */

#include <assert.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <dce/uuid.h>
#include <dcedfs/afsvl_data.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/itl_utilsNameHash.h>
#include <utils/itl/itl_fxDCEError.h>
#include <utils/itl/itl_fxServer.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/itl/fx/itl_fxServer.c,v 1.1.11.1 1996/10/17 18:26:40 damon Exp $")

#define PRIMARY_IF_UUID   0
#define SECONDARY_IF_UUID 1

PRIVATE unsigned int serverHashType;


/*
 *  ServerInit() --
 *    To be called via pthread_once to do the real initialization.
 *
 *  ASSUMPTIONS:
 *    That the itlu_NameHashInit function has been called.
 */
PRIVATE void ServerInit()
{
    /*
     *  We can safely set serverHashType because this function is called
     *  via pthread_once.
     */
    serverHashType = itlu_NameHashGetType();
}

/*
 * itlFX_ServerInit() -- 
 *    Stuff that needs to be done before we get rolling.
 * 
 * RETURN CODES: none
 */
EXPORT void itlFX_ServerInit()
{
    static pthread_once_t hashOnce = pthread_once_init;

    /*
     *  First ensure the hash table module is initialized.
     */
    itlu_NameHashInit();

    if (pthread_once(&hashOnce, (pthread_initroutine_t) ServerInit))
	itl_Error("pthread_once failed in ServerInit\n");
}

/*
 * itlFX_ServerAddName() --
 *    Adds a server name and related info to the table.
 *
 * RETURN CODES: 
 *    FALSE - Successful addition.
 *    TRUE  - Unsuccessful addition, server name exists or is invalid.
 */
SHARED boolean_t itlFX_ServerAddName(vldbInfoP)
  IN struct vldbentry *vldbInfoP;
{
    boolean_t retVal = TRUE;
    char *name;
    itlFX_server_t *serverP;
    error_status_t errorStatus;
    unsigned long returnCode;

    ITL_UTIL_NEW(serverP, itlFX_server_t);

    ITL_UTIL_MEMCOPY(&(vldbInfoP->siteAddr[0]), &(serverP->address),
		     sizeof(struct sockaddr_in));
    serverP->address.sin_port = 0;
    ITL_UTIL_MEMCOPY(&(vldbInfoP->siteObjID[0]), &(serverP->objId), 
		     sizeof(afsUUID));

    if (dfsuuid_Create(&(serverP->primaryUUID), PRIMARY_IF_UUID) == 0)
    {
	if (dfsuuid_Create(&(serverP->secondaryUUID), SECONDARY_IF_UUID) == 0)
	{
	    name = (char *) vldbInfoP->sitePrincipal[0].text;
	    retVal = itlu_NameHashAdd(name, serverHashType, (void *) serverP);
	    /*
	     *  Set up principal name pointer by setting to point to the
	     *  name in the hash table.
	     */
	    if (!retVal)
	    {
		itlu_NameHashTable_t *entryP;

		entryP = itlu_NameHashLookup(name, serverHashType);
		assert(entryP != ITLU_NAME_HASH_NULL);
		serverP->principalName = ITLU_NAME_HASH_GET_NAME(entryP);
		itlu_NameHashRelease(entryP);
	    }
	}
	else
	{
	    ITL_FX_DCE_ERROR(returnCode, 
			     "Creation of secondary interface UUID FAILED");
	}
    }
    else
    {
	ITL_FX_DCE_ERROR(returnCode, 
			 "Creation of primary interface UUID FAILED");
    }

    if (retVal)
	ITL_UTIL_FREE(serverP);

    return(retVal);
}

/*
 * itlFX_ServerRemoveName() --
 *    Delete a server name from the table.
 *
 * RETURN CODES: void
 */
SHARED void itlFX_ServerRemoveName(nameP)
  IN char *nameP;   /* name of server to remove */
{
    itlFX_server_t *entryP;

    if ((entryP = (itlFX_server_t *) itlu_NameHashDelete(nameP, 
							 serverHashType))
	       != (itlFX_server_t *) 0)
	ITL_FREE(entryP);
    else 
	itl_Message("\nWARNING: Server, %s, NOT in table.\n\n", nameP);
}

/*
 * itlFX_ServerLookup() -- 
 *    Given a server name, lookup and return a pointer to the itlFX_server_t.
 *
 * RETURN CODES: 
 *    NULL - if no entry is found.
 *    Otherwise success.
 */
SHARED itlFX_server_t *itlFX_ServerLookup(nameP)
  IN char *nameP;                /* the server name to compare with */
{
    itlu_NameHashTable_t *entryP;
    itlFX_server_t *retVal = (itlFX_server_t *) 0;

    if ((entryP = itlu_NameHashLookup(nameP, serverHashType))
	       != ITLU_NAME_HASH_NULL)
    {
	retVal = ITLU_NAME_HASH_GET_DATA(entryP, itlFX_server_t);
	itlu_NameHashRelease(entryP);
    }

    return(retVal);
}



