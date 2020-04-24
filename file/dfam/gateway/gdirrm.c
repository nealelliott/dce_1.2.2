/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gdirrm.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:57  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:19  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:53:10  takano
 * 	First Submit
 * 	[1996/02/21  07:53:06  takano]
 * 
 * $EndLog$
 */
#include "gdir.h"

/*********************************************************** EnglishComment **/
/* ID = DeleteTree                                                           */
/*                                                                           */
/* name = Deleting all the directories under the designated top directory    */
/* func = Hierarchically deletes all the directories under the designated    */
/*        top directory.                                                     */
/* i/o  = pszNWPath:char *:inp: the name of the top directory to be deleted  */
/*        iMode    :int   :inp: processing indicator:                        */
/*                              0: initialization (DMDelModeInit)            */
/*                              1: synchronization (DMDelModeSync)           */
/*                              2: termination (DMDelModeClear)              */
/*                        (iMode is passed to DeleteFile and DeleteDir)      */
/* return = 0: normal end                                                    */
/*          EDMmalloc                                                        */
/*          EDMwarning                                                       */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = February 21, 1995 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DeleteTree( char *pszNWTopPath, int iMode )
{
	int						iErr = 0;
	int						iRet;
	char					*pszNWParentOfTopPath = NULL;
	char					*pszNWDirname = NULL;
	char					*pszNWFilename = NULL;
	struct NWFakeDirEnt		*pNWCurDirDirEnt = NULL;
	struct NWFakeDirEnt		*pNWCurFileDirEnt = NULL;

    GW_tsh_functrace("DeleteTree", GW_FUNC_START, 0);

	if ( (pszNWParentOfTopPath=(char *)GW_malloc( NW_MAX_PATH + 1 )) == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
		iErr = EDMmalloc;
		goto labelDeleteTree_Q;
	} else if ( (pszNWDirname=(char *)GW_malloc( NW_MAX_PATH + 1 )) == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
		iErr = EDMmalloc;
		goto labelDeleteTree_Q;
	} else if ( (pszNWFilename=(char *)GW_malloc( NW_MAX_PATH + 1 )) == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
		iErr = EDMmalloc;
		goto labelDeleteTree_Q;
	}


	pNWCurDirDirEnt =
			(struct NWFakeDirEnt *)GW_malloc( sizeof( struct NWFakeDirEnt ) );
	if ( pNWCurDirDirEnt == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
		GW_tsh_functrace("DeleteTree", GW_FUNC_END, EDMmalloc);
		iErr = EDMmalloc;
		goto labelDeleteTree_Q;
	} else {

		pNWCurDirDirEnt->iLink = -1;
		pNWCurDirDirEnt->byUndeleteable = FALSE;
		pNWCurDirDirEnt->iLevel = 0;
		pNWCurDirDirEnt->iOrder = 0;
		pNWCurDirDirEnt->iAttr = _A_SUBDIR;
		pNWCurDirDirEnt->pPrev = NULL;
		pNWCurDirDirEnt->pNext = NULL;
		pNWCurDirDirEnt->pParent = NULL;
		NWBasename( pNWCurDirDirEnt->szName, pszNWTopPath );
	}



	NWDirname( pszNWParentOfTopPath, pszNWTopPath );

	while ( pNWCurDirDirEnt != NULL ) {
		if ( iMode != DMDelModeClear ) {
			if( ShutdownReq() ) { iErr=EDMshutdownreq; goto labelDeleteTree_Q; }
			if( AgentDie() ) { iErr=EDMagentdie;  goto labelDeleteTree_Q; }
		}



		if ( NWGetFullPathFromDirEnt( pszNWDirname, pszNWParentOfTopPath,
				pNWCurDirDirEnt) ) {

			NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
			continue;
		}
		if ( pNWCurDirDirEnt->iLink == -1 ) {



			if ( iRet = NWChainFakeDirEnt(	pszNWDirname,
											&pNWCurFileDirEnt,
											pNWCurDirDirEnt->iLevel + 1,
											&(pNWCurDirDirEnt->iLink ) ) ) {
				iErr = EDMwarning;
			}


			while ( pNWCurFileDirEnt != NULL ) {
				if ( pNWCurFileDirEnt->iAttr & _A_SUBDIR ) {

					pNWCurFileDirEnt->pParent = pNWCurDirDirEnt;
					NWInsertDirEnt( pNWCurDirDirEnt, pNWCurFileDirEnt );
				} else if ( (strlen(pszNWDirname) + 1 +
						strlen(pNWCurFileDirEnt->szName) ) <= _MAX_PATH ) {

					sprintf( pszNWFilename, "%s\\%s",
							pszNWDirname, pNWCurFileDirEnt->szName );
					iRet = DeleteFile( pszNWFilename, iMode );
					switch ( iRet ) {
					case EDMkeepfile:
						NWMakeFakeDirEntUndeleteable( pNWCurDirDirEnt );
						iErr = EDMkeepfile;
						goto labelDeleteTree_Q;
					case EDMunlink:
						iErr = EDMwarning;
						NWMakeFakeDirEntUndeleteable( pNWCurDirDirEnt );
					}
				}
				NWDeleteFakeDirEnt(pNWCurFileDirEnt, &pNWCurFileDirEnt );
			}
			NWMoveToNextFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );

		} else {

			if ( !pNWCurDirDirEnt->byUndeleteable ) {
				iRet = DeleteDir( pszNWDirname, iMode );
				switch ( iRet ) {
				case EDMkeepfile:
					NWMakeFakeDirEntUndeleteable( pNWCurDirDirEnt );
					iErr = EDMkeepfile;
					goto labelDeleteTree_Q;
				case EDMunlink:
					iErr = EDMwarning;
					NWMakeFakeDirEntUndeleteable( pNWCurDirDirEnt );
				}
			}
			NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
		}

	}

labelDeleteTree_Q:
	if ( pNWCurFileDirEnt != NULL ) {
		NWFreeFakeDirEntList( pNWCurFileDirEnt );
	}
	if ( pNWCurDirDirEnt != NULL ) {
		NWFreeFakeDirEntList( pNWCurDirDirEnt );
	}

	if ( pszNWParentOfTopPath != NULL ) {
		DMfree( pszNWParentOfTopPath, "DeleteTree" );
	}
	if ( pszNWDirname != NULL ) {
		DMfree( pszNWDirname, "DeleteTree" );
	}
	if ( pszNWFilename != NULL ) {
		DMfree( pszNWFilename, "DeleteTree" );
	}

	GW_tsh_functrace("DeleteTree", GW_FUNC_END, iErr);
	return iErr;
}
