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
 * $Log: gdirmk.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:54  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:16  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:52:00  takano
 * 	First Submit
 * 	[1996/02/21  07:51:55  takano]
 * 
 * $EndLog$
 */
#include "gdir.h"

/*********************************************************** EnglishComment **/
/* ID = MakeTree                                                             */
/*                                                                           */
/* name = Making directory tree on NetWare Volume                            */
/* func = Converts the names of the directory structure under the designated */
/*        DFS path, and copies them to the NetWare volume.                   */
/*        *1: Makes an immediate return when a connection error occurred.    */
/*        *2: Makes an immediate return when disk/directory area runs out.   */
/*                                                                           */
/* i/o  = pszNWPath:char *:inp: top directory to be deleted (should not be   */
/*                              the root directory)                          */
/*        iMode    :int   :inp: processing indicator:                        */
/*                              0: initialization (DMDelModeInit)            */
/*                              1: synchronization (DMDelModeSync)           */
/*                              2: termination (DMDelModeClear)              */
/*                                                                           */
/* return = 0: normal end                                                    */
/*          EDMmalloc                                                        */
/*          EDMagentdie                                                      */
/*          EDMbug                                                           */
/*          <-DFSstatReq                                                     */
/*          <-TouchDir                                                       */
/*          <-TouchFile                                                      */
/*          <-DFSChainFakeDirEnt                                             */
/*          <-NWChainFakeDirEnt                                              */
/*          others: error code                                               */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = February 21, 1995 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int MakeTree( struct GVolume_t *pVolTbl, char *pszDFSTopPath,
					char *pszNWTopPath)
{
	int						iErr = 0;
	int						iRet;
	int						iDFSErr;
	char					*pszNWParentOfTopPath = NULL;
	char					*pszNWDirname = NULL;
	char					*pszNWFilename = NULL;
	char					*pszDFSDirname = NULL;
	struct NWFakeDirEnt		*pNWCurDirDirEnt = NULL;
	struct NWFakeDirEnt		*pNWCurFileDirEnt = NULL;
	struct DFSFakeDirEnt	*pDFSFirstFileDirEnt = NULL;
	struct dstat			*pDFSdstat;

    GW_tsh_functrace("MakeTree", GW_FUNC_START, 0);

	if ( (pszNWParentOfTopPath=(char *)GW_malloc(NW_MAX_PATH+1)) == NULL ) {
		goto labelMakeTreeMallocQuit;
	} else if ( (pszNWDirname=(char *)GW_malloc(NW_MAX_PATH+1)) == NULL ) {
		goto labelMakeTreeMallocQuit;
	} else if ( (pszNWFilename=(char *)GW_malloc(NW_MAX_PATH+1)) == NULL ) {
		goto labelMakeTreeMallocQuit;
	} else if ( (pszDFSDirname=(char *)GW_malloc(DFS_MAX_PATH+1)) == NULL ) {
		goto labelMakeTreeMallocQuit;
	} else if ( (pDFSdstat=(struct dstat *)GW_malloc(sizeof(struct dstat))) == NULL) {
		goto labelMakeTreeMallocQuit;
	}


	iRet = DFSstatReq( pszDFSTopPath, pDFSdstat, &iDFSErr, &gComS );
	if ( iRet ) {
		switch ( iDFSErr ) {

		case DENETWORKENV:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11651);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelMakeTreeQuit;
		case DEUNLOADTCPIP:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11204);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelMakeTreeQuit;
		case DEREFUSEAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelMakeTreeQuit;
		case DEINVALSOCKET:	case DERESPONCEDATA:	case DEDFAMDCESYSTEM:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelMakeTreeQuit;
		case DEDFAMTONOVELL:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11681, DEDFAMTONOVELL);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelMakeTreeQuit;
		case DETIMEOUTAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11654);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelMakeTreeQuit;
		case DEDFAMTERM:
			iErr = EDMshutdownreq;
			goto labelMakeTreeQuit;
		default:
			if ( iDFSErr < 1200 ) {
				iErr = iDFSErr;
				goto labelMakeTreeQuit;
			} else {
				iErr = EDMbug;
				goto labelMakeTreeQuit;
			}
		}
	}


	pNWCurDirDirEnt =
			(struct NWFakeDirEnt *)GW_malloc( sizeof( struct NWFakeDirEnt ) );
	if ( pNWCurDirDirEnt == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
		iErr = EDMmalloc;
		goto labelMakeTreeQuit;
	} else {

		pNWCurDirDirEnt->iLink = -1;
		pNWCurDirDirEnt->byUndeleteable = FALSE;
		pNWCurDirDirEnt->iLevel = 0;
		pNWCurDirDirEnt->iOrder = 0;
		pNWCurDirDirEnt->iAttr = _A_SUBDIR;
		pNWCurDirDirEnt->pPrev = NULL;
		pNWCurDirDirEnt->pNext = NULL;
		pNWCurDirDirEnt->pParent = NULL;
		pNWCurDirDirEnt->uiTime = pDFSdstat->d_mtime;
		NWBasename( pNWCurDirDirEnt->szName, pszNWTopPath );
	}
	DMfree( pDFSdstat, "MakeTree" ); pDFSdstat = NULL;



	NWDirname( pszNWParentOfTopPath, pszNWTopPath );

	while ( pNWCurDirDirEnt != NULL ) {



		if ( NWGetFullPathFromDirEnt( pszNWDirname, pszNWParentOfTopPath,
				pNWCurDirDirEnt) ) {

			NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
			continue;
		}

		iRet = GetDFSPath( pszNWDirname, pszDFSDirname, OPENMODE );
		switch ( iRet ) {
		case 0:
			break;
		case GWERR_NOMEM:

			NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
			continue;
		default:
			NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
			continue;
		}


		if ( pNWCurDirDirEnt->iLink == -1 ) {

			pNWCurDirDirEnt->iLink = 0;

			if ( pNWCurDirDirEnt->iAttr & _A_SUBDIR ) {

				iRet = TouchDir( pszNWDirname, pNWCurDirDirEnt->uiTime,
						NULL, DMMkModeNot );
				switch ( iRet ) {
				case 0:
					break;
				case EDMsystemfile:
					NWDeleteFakeDirEnt(pNWCurDirDirEnt, &pNWCurDirDirEnt );
					continue;
				case EDMdiskspace:
					iErr = iRet;
					NWFreeFakeDirEntList( pNWCurDirDirEnt );
					pNWCurDirDirEnt = NULL;
					goto labelMakeTreeQuit;
				case EDMdirectoryblock:
					iErr = iRet;
					NWFreeFakeDirEntList( pNWCurDirDirEnt );
					pNWCurDirDirEnt = NULL;
					goto labelMakeTreeQuit;
				case EDMdirlevel:
					NWDeleteFakeDirEnt(pNWCurDirDirEnt, &pNWCurDirDirEnt );
					continue;
				case EDMmkdir:
					iErr = iRet;
					NWDeleteFakeDirEnt(pNWCurDirDirEnt, &pNWCurDirDirEnt );
					continue;

				default:
					iErr = iRet;
					break;
				}
			}



			if ( iRet = DFSChainFakeDirEnt(	pszDFSDirname,
											&pDFSFirstFileDirEnt,
											pNWCurDirDirEnt->iLevel + 1,
											NULL ) ) {
				switch ( iRet ) {
				case EDMshutdownreq:
				case EDMagentdie:
					iErr = iRet;
					goto labelMakeTreeQuit;
				case EDMmalloc:
					iErr = iRet;
				default:
					iErr = iRet;
				}
			}

			iRet = NWChainFakeDirEntFromDFSOne(
											pVolTbl,
											pszDFSDirname,
											pDFSFirstFileDirEnt,
											&pNWCurFileDirEnt);
			if ( iRet ) {
				iErr = iRet;
			}
			DFSFreeFakeDirEntList( pDFSFirstFileDirEnt );
			pDFSFirstFileDirEnt = NULL;


			while ( pNWCurFileDirEnt != NULL ) {
				if ( pNWCurFileDirEnt->iAttr & _A_SUBDIR ) {

					pNWCurFileDirEnt->pParent = pNWCurDirDirEnt;
					NWInsertDirEnt( pNWCurDirDirEnt, pNWCurFileDirEnt );
				} else if ( ( strlen(pszNWDirname) + 1 +
						strlen(pNWCurFileDirEnt->szName) ) <= _MAX_PATH ) {
					sprintf(	pszNWFilename, "%s\\%s", pszNWDirname,
							pNWCurFileDirEnt->szName );

					iRet = TouchFile(
							pszNWFilename, pNWCurFileDirEnt->iSize,
							pNWCurFileDirEnt->uiTime, NULL, DMMkModeNot );
					switch ( iRet ) {
					case 0:
						break;
					case EDMsystemfile:
						break;
					case EDMdiskspace:
						iErr = iRet;
						goto labelMakeTreeQuit;
					default:
						iErr = iRet;
						break;
					}
				}
				NWDeleteFakeDirEnt(pNWCurFileDirEnt, &pNWCurFileDirEnt );
			}
			NWMoveToNextFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );

		} else {

			NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
		}

	}

	if ( pszNWParentOfTopPath != NULL ) {
		DMfree( pszNWParentOfTopPath,"MakeTree" );
	}
	if ( pszNWDirname != NULL ) {
		DMfree( pszNWDirname,"MakeTree" );
	}
	if ( pszNWFilename != NULL ) {
		DMfree( pszNWFilename,"MakeTree" );
	}
	if ( pszDFSDirname != NULL ) {
		DMfree( pszDFSDirname,"MakeTree" );
	}
	GW_tsh_functrace("MakeTree", GW_FUNC_END, iErr);
	return iErr;

labelMakeTreeMallocQuit:
	GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
	iErr = EDMmalloc;
labelMakeTreeQuit:
	if ( pDFSFirstFileDirEnt != NULL ) {
		DFSFreeFakeDirEntList( pDFSFirstFileDirEnt );
		pDFSFirstFileDirEnt = NULL;
	}
	if ( pNWCurFileDirEnt != NULL ) {
		NWFreeFakeDirEntList( pNWCurFileDirEnt );
		pNWCurFileDirEnt = NULL;
	}
	if ( pNWCurDirDirEnt != NULL ) {
		NWFreeFakeDirEntList( pNWCurDirDirEnt );
		pNWCurDirDirEnt = NULL;
	}
	if ( pDFSdstat != NULL ) {
		DMfree( pDFSdstat, "MakeTree" );
	}
	if ( pszNWParentOfTopPath != NULL ) {
		DMfree( pszNWParentOfTopPath,"MakeTree" );
	}
	if ( pszNWDirname != NULL ) {
		DMfree( pszNWDirname,"MakeTree" );
	}
	if ( pszNWFilename != NULL ) {
		DMfree( pszNWFilename,"MakeTree" );
	}
	if ( pszDFSDirname != NULL ) {
		DMfree( pszDFSDirname,"MakeTree" );
	}
	GW_tsh_functrace("MakeTree", GW_FUNC_END, iErr);
	return iErr;
}
