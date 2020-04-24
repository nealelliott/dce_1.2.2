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
 * $Log: gdirsync.c,v $
 * Revision 1.1.2.3  1996/03/22  07:02:14  takano
 * 	Fixed the confliction between the Directory Synchronization and the File-Access.
 * 	[1996/03/22  06:59:17  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:07:58  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:20  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  07:53:43  takano
 * 	First Submit
 * 	[1996/02/21  07:53:38  takano]
 * 
 * $EndLog$
 */
#include "gdir.h"

/*********************************************************** EnglishComment **/
/* ID = DMsync                                                               */
/*                                                                           */
/* name = Directory synchronization main routine                             */
/* func = Invokes synchronization function for each DFAM Volume Table.       */
/*                                                                           */
/* i/o  =                                                                    */
/* :                                                                         */
/* return = 0: normal end                                                    */
/*          EDMshutdownreq                                                   */
/*	    EDMagentdie                                                          */
/*          <-DMsyncVolume                                                   */
/*                                                                           */
/* common = pVolTbl->usVolStatus                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 14, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DMsync( void )
{
	int					iErr = 0;
	int					iRet = 0;
	int					iDFSErr;

	struct GVolume_t	*pVolTbl;
	time_t				uiPrevtime;
	time_t				uiNewtime;

    GW_tsh_functrace("DMsync", GW_FUNC_START, 0);


	pVolTbl = pVolume_sp;

	while ( pVolTbl != NULL ) {
		iRet = CheckVolumeMap( pVolTbl->aszNWVol );
		if ( iRet ) {
			pVolTbl = pVolTbl->pUshirop;
			continue;
		} else if ( pVolTbl->usVolStatus & NW_NOENT ) {
			pVolTbl = pVolTbl->pUshirop;
			continue;
		} else if ( pVolTbl->usVolStatus & NW_USERSKIP ) {
			pVolTbl = pVolTbl->pUshirop;
			continue;

		} else if ( pVolTbl->usVolStatus &
				(NW_SPACE|DFS_TIMEOUT|DFS_NOENT|DFS_ACCESS|DFS_ERROR) ) {

			GW_Message(DFAM_CON, 0, LOG_YES, KDDS11631, pVolTbl->aszNWVol);

			iRet = ClearVolume( pVolTbl, DMDelModeSync );
			if ( iRet )
				iErr = iRet;
			iRet = DMInitVolume( pVolTbl );
			switch ( iRet ) {
			case EDMshutdownreq:
			case EDMagentdie:
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11633, pVolTbl->aszNWVol);
				pVolTbl->usVolStatus |= DFS_TIMEOUT;
				iErr = iRet;
				goto labelDMsync_Q;
			case DEACCES:
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11633, pVolTbl->aszNWVol);
				pVolTbl->usVolStatus |= DFS_ACCESS;
				pVolTbl = pVolTbl->pUshirop;
				continue;
			case DENOENT:
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11633, pVolTbl->aszNWVol);
				pVolTbl->usVolStatus |= DFS_NOENT;
				pVolTbl = pVolTbl->pUshirop;
				continue;
			case EDMdiskspace:
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11671, pVolTbl->aszNWVol);
				pVolTbl->usVolStatus |= NW_SPACE;
				pVolTbl = pVolTbl->pUshirop;
				continue;
			case EDMdirectoryblock:
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11672, pVolTbl->aszNWVol);
				pVolTbl->usVolStatus |= NW_SPACE;
				pVolTbl = pVolTbl->pUshirop;
				continue;
			default:
				if ( (iRet > 0) && (iRet < 1200) ) {
					GW_Message(DFAM_CON, 0, LOG_YES, KDDS11633, pVolTbl->aszNWVol);
					pVolTbl->usVolStatus |= DFS_TIMEOUT;
					pVolTbl = pVolTbl->pUshirop;
					continue;
				} else {
					GW_Message(DFAM_CON, 0, LOG_YES, KDDS11632, pVolTbl->aszNWVol);
					pVolTbl->usVolStatus = 0;
					pVolTbl = pVolTbl->pUshirop;
					continue;
				}
			}
		} else {

			uiPrevtime = pVolTbl->uiSynctime;
			if( ShutdownReq() ) { iErr=EDMshutdownreq; goto labelDMsync_Q; }
			if( AgentDie() ) { iErr=EDMagentdie; goto labelDMsync_Q; }
			iRet = DFStimeReq( NULL, &iDFSErr, &gComS );
			if ( iRet == -1 ) {
				switch ( iDFSErr ) {

				case DENETWORKENV:
					GW_Message( DFAM_CON, 0, LOG_YES, KDDS11651);
					SetDeathFlag();
					iErr = EDMagentdie;
					goto labelDMsync_Q;
				case DEUNLOADTCPIP:
					GW_Message( DFAM_CON, 0, LOG_YES, KDDS11204);
					SetDeathFlag();
					iErr = EDMagentdie;
					goto labelDMsync_Q;
				case DEREFUSEAGT:
					GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
					SetDeathFlag();
					iErr = EDMagentdie;
					goto labelDMsync_Q;
				case DEINVALSOCKET:	case DEDFAMDCESYSTEM:	case DERESPONCEDATA:
					GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
					SetDeathFlag();
					iErr = EDMagentdie;
					goto labelDMsync_Q;
				case DEDFAMTONOVELL:
					GW_Message( DFAM_CON, 0, LOG_YES, KDDS11681, DEDFAMTONOVELL);
					SetDeathFlag();
					iErr = EDMagentdie;
					goto labelDMsync_Q;
				case DETIMEOUTAGT:
					GW_Message( DFAM_CON, 0, LOG_YES, KDDS11654);
					SetDeathFlag();
					iErr = EDMagentdie;
					goto labelDMsync_Q;
				case DEDFAMTERM:
					iErr = EDMshutdownreq;
					goto labelDMsync_Q;
				default:
					uiNewtime = uiPrevtime;
					break;
				}
			} else {
				uiNewtime = iRet;
			}

			iRet = DMsyncVol( pVolTbl, uiPrevtime );
			switch ( iRet ) {
			case 0:
				pVolTbl->uiSynctime = uiNewtime;
				break;
			case EDMshutdownreq:
			case EDMagentdie:
				iErr = iRet;
				goto labelDMsync_Q;
			case DENOENT:
				GW_Message( DFAM_CON, 0, LOG_YES, KDDS11664, pVolTbl->aszDFSPath );
				iErr = iRet;
				break;
			case DEACCES:
				GW_Message( DFAM_CON, 0, LOG_YES, KDDS11666, pVolTbl->aszDFSPath );
				iErr = iRet;
				break;
			case EDMdiskspace:
				GW_Message( DFAM_CON, 0, LOG_YES, KDDS11671, pVolTbl->aszNWVol);
				iErr = iRet;
				break;
			case EDMdirectoryblock:
				GW_Message( DFAM_CON, 0, LOG_YES, KDDS11672, pVolTbl->aszNWVol);
				iErr = iRet;
				break;
			default:
				if ( iRet < 1200 ) {
					GW_Message( DFAM_CON, 0, LOG_YES, KDDS11663, iRet );
					GW_Message( DFAM_CON, 0, LOG_YES, KDDS11665, pVolTbl->aszDFSPath );
					iErr = iRet;
					break;
				} else {
					iErr = iRet;
					break;
				}
			}
		}
		pVolTbl = pVolTbl->pUshirop;
	}

	GW_tsh_functrace("DMsync", GW_FUNC_END, iErr);
	return iErr;

labelDMsync_Q:
	GW_tsh_functrace("DMsync", GW_FUNC_END, iErr);
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = DMsyncVol                                                            */
/*                                                                           */
/* name = Making a copy of a directory tree for each volume                  */
/* func = From the result of DFSdirsyncReq(), makes a list of directories    */
/*        that need synchronization, and invokes DFSdirsycnReq() for each    */
/*        directory to request the synchronization.  If DFSdirsycnReq()      */
/*        returned a file name, takes an immediate action.                   */
/*                                                                           */
/* i/o  = pVolTbl   :struct GVolume_t *:inp: parameters for renaming         */
/*        uiPrevTime:time_t            :inp:                                 */
/*                                                                           */
/* return = 0: normal end                                                    */
/*          EDMagentdie                                                      */
/*          EDMbug                                                           */
/*          EDMwarning                                                       */
/*          EDMnameconv                                                       */
/*          <-DFSstatReq(exept communication error)                          */
/*          <-DFSopendirReq(exept communication error)                       */
/*          <-DFSdirsyncReq(exept communication error)                       */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 5, 1994 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DMsyncVol( struct GVolume_t *pVolTbl, time_t uiPrevTime )
{
	long				iErr = 0;
	int					iRet;

	struct ddirsync		*pDirSync = NULL;
	int					iDFSErr;
	struct ModifiedDir	*pModifiedDir = NULL;
	struct ModifiedDir	*pPrevDir = NULL;
	struct ModifiedDir	*pCur = NULL;
	struct ModifiedDir	*pTmp = NULL;
	struct dstat		DFSdstat;
	DDIR				*pDFSdir = NULL;

    GW_tsh_functrace("DMsyncVol", GW_FUNC_START, 0);
	PurgeOldDeletedEntry( pVolTbl->aszNWVol );


	iRet = DFSstatReq( pVolTbl->aszDFSPath, &DFSdstat, &iDFSErr, &gComS );
	if ( iRet ) {
		switch ( iDFSErr ) {

		case DENETWORKENV:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11651);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEUNLOADTCPIP:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11204);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEREFUSEAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEINVALSOCKET:	case DERESPONCEDATA:	case DEDFAMDCESYSTEM:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEDFAMTONOVELL:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11681, DEDFAMTONOVELL);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DETIMEOUTAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11654);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEDFAMTERM:
			iErr = EDMshutdownreq;
			goto labelDMsyncVol_Q;
		default:
			if ( iDFSErr < 1200 ) {
				iErr = iDFSErr;
				goto labelDMsyncVol_Q;
			} else {
				iErr = EDMbug;
				goto labelDMsyncVol_Q;
			}
		}
	}

	pDFSdir = (DDIR *)GW_malloc(sizeof(DDIR));
	if ( pDFSdir == NULL ) {
		iErr = EDMmalloc;
		goto labelDMsyncVol_Q;
	}
	iRet = DFSopendirReq( pVolTbl->aszDFSPath, pDFSdir, &iDFSErr, &gComS );
	if ( iRet ) {
		switch ( iDFSErr ) {

		case DENETWORKENV:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11651);
			SetDeathFlag();
			DMfree( pDFSdir, "DMSyncVol" );
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEUNLOADTCPIP:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11204);
			SetDeathFlag();
			DMfree( pDFSdir, "DMSyncVol" );
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEREFUSEAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			DMfree( pDFSdir, "DMSyncVol" );
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEINVALSOCKET:	case DERESPONCEDATA:	case DEDFAMDCESYSTEM:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			DMfree( pDFSdir, "DMSyncVol" );
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEDFAMTONOVELL:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11681, DEDFAMTONOVELL);
			SetDeathFlag();
			DMfree( pDFSdir, "DMSyncVol" );
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DETIMEOUTAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11654);
			SetDeathFlag();
			DMfree( pDFSdir, "DMSyncVol" );
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEDFAMTERM:
			iErr = EDMshutdownreq;
			goto labelDMsyncVol_Q;
		default:
			if ( iDFSErr < 1200 ) {
				DMfree( pDFSdir, "DMSyncVol" );
				iErr = iDFSErr;
				goto labelDMsyncVol_Q;
			} else {
				DMfree( pDFSdir, "DMSyncVol" );
				iErr = EDMbug;
				goto labelDMsyncVol_Q;
			}
		}
	}
	iRet = DFSclosedirReq( pDFSdir, &iDFSErr, &gComS );
	DMfree( pDFSdir, "DMSyncVol" );


	if ((pDirSync=(struct ddirsync *)GW_malloc(sizeof(struct ddirsync))) == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
		GW_tsh_functrace("DMsyncVol", GW_FUNC_END, EDMmalloc);
		iErr = EDMmalloc;
		goto labelDMsyncVol_Q;
	}

	if( ShutdownReq() ) { iErr=EDMshutdownreq; goto labelDMsyncVol_Q; }
	if( AgentDie() ) { iErr=EDMagentdie; goto labelDMsyncVol_Q; }


	WaitOnLocalSemaphore( uiCopyFile_sh );
	AccessListRoot.iAccessListMode = DMAccessListModeSync;
	SignalLocalSemaphore( uiCopyFile_sh );

	iRet = DFSdirsyncReq(	uiPrevTime, pVolTbl->aszDFSPath,
							&iDFSErr, pDirSync, &gComS );
	if ( (iRet) && (iDFSErr) ) {
		switch (iDFSErr ) {

		case DENETWORKENV:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11651);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEUNLOADTCPIP:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11204);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEREFUSEAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEINVALSOCKET:	case DERESPONCEDATA:	case DEDFAMDCESYSTEM:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEDFAMTONOVELL:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11681, DEDFAMTONOVELL);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DETIMEOUTAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11654);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEDFAMTERM:
			iErr = EDMshutdownreq;
			goto labelDMsyncVol_Q;
		default:
			iErr = iDFSErr;
			goto labelDMsyncVol_Q;
		}
	}
	if ( pDirSync->d_namlen == -1 ) {
		iErr = 0;
		goto labelDMsyncVol_N;

	} else if ( pDirSync->d_size != -1 ) {
		iRet = TouchFileByDFSPath(	pVolTbl,
									pDirSync->d_name,
									pDirSync->d_size,
									pDirSync->d_mtime );
		switch ( iRet ) {
		case 0:
			break;
		case EDMlongpath:
			break;
		case EDMsystemfile:
			break;

		case EDMagentdie:
			iErr = iRet;
			goto labelDMsyncVol_Q;
		case EDMshutdownreq:
			iErr = iRet;
			goto labelDMsyncVol_Q;
		case EDMdiskspace:
			iErr = iRet;
			goto labelDMsyncVol_Q;
		case EDMcreat:
			iErr = iRet;
			break;
		default:
			iErr = iRet;
			break;
		}
	} else {

		pTmp = (struct ModifiedDir *)GW_malloc(sizeof(struct ModifiedDir));
		if ( pTmp == NULL ) {
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
			iErr = EDMmalloc;
			goto labelDMsyncVol_Q;
		} else {
			pTmp->pszDFSModifiedPath = (char *)GW_malloc( strlen(pDirSync->d_name ) + 1);
			if ( pTmp->pszDFSModifiedPath == NULL ) {
				GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
				DMfree( pTmp, "DMsyncVol" );
				iErr = EDMmalloc;
				goto labelDMsyncVol_Q;
			} else {
				strcpy( pTmp->pszDFSModifiedPath, pDirSync->d_name );
				pTmp->pNext = NULL;
				pTmp->pPrev = NULL;
				pTmp->uiTime = pDirSync->d_mtime,
				pModifiedDir = pTmp;
			}
		}
	}


	pPrevDir = pModifiedDir;
	if( ShutdownReq() ) { iErr=EDMshutdownreq; goto labelDMsyncVol_Q; }
	if( AgentDie() ) { iErr=EDMagentdie; goto labelDMsyncVol_Q; }

labelDMsyncVol_C:
	while ( (iRet = DFSdirsyncReq(	(int)-1, pVolTbl->aszDFSPath, &iDFSErr,
									pDirSync, &gComS ) ) == 0	)  {
		if ( pDirSync->d_namlen == -1 ) {
			break;
		} else if ( pDirSync->d_size != -1 ) {
			iRet = TouchFileByDFSPath(	pVolTbl,
										pDirSync->d_name,
										pDirSync->d_size,
										pDirSync->d_mtime );
			switch ( iRet ) {
			case EDMlongpath:
				break;
			default:
				iErr = iRet;
				break;
			}
		} else {
			pTmp = (struct ModifiedDir *)GW_malloc(sizeof(struct ModifiedDir));
			if ( pTmp == NULL ) {
				iErr = EDMwarning;
				break;
			} else {
				pTmp->pszDFSModifiedPath=(char *)GW_malloc(strlen(pDirSync->d_name)+1);
				if ( pTmp->pszDFSModifiedPath == NULL ) {
					DMfree( pTmp, "DMsyncVol"  );
					iErr = EDMwarning;
					break;
				} else {
					strcpy( pTmp->pszDFSModifiedPath, pDirSync->d_name );
					pPrevDir->pNext = pTmp;
					pTmp->pNext = NULL;
					pTmp->pPrev = pPrevDir;
					pTmp->uiTime = pDirSync->d_mtime,
					pPrevDir = pTmp;
				}
			}
		}
	}
	if ( iRet && iDFSErr ) {
		switch (iDFSErr) {
		case DENOENT:	case DENODEV:	case DEACCES:
			goto labelDMsyncVol_C;
		case DENETWORKENV:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11651);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEUNLOADTCPIP:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11204);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEREFUSEAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEINVALSOCKET:	case DERESPONCEDATA:	case DEDFAMDCESYSTEM:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEDFAMTONOVELL:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11681, DEDFAMTONOVELL);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DETIMEOUTAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11654);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMsyncVol_Q;
		case DEDFAMTERM:
			iErr = EDMshutdownreq;
			goto labelDMsyncVol_Q;
		default:
			iErr = EDMwarning;
			break;
		}
	}

	WaitOnLocalSemaphore( uiCopyFile_sh );
	AccessListRoot.iAccessListMode = DMAccessListModeInterval;
	DMFreeAccessFileList();
	SignalLocalSemaphore( uiCopyFile_sh );


	pCur = pModifiedDir;
	while ( pCur != NULL ) {
		iRet = DMsyncDir( pVolTbl, pCur->pszDFSModifiedPath,pCur->uiTime,
						DMDelModeSync );
		switch ( iRet ) {
		case EDMshutdownreq:
		case EDMagentdie:
			iErr = iRet;
			goto labelDMsyncVol_Q;
		case EDMdiskspace:
			iErr = iRet;
			goto labelDMsyncVol_Q;
		case EDMdirectoryblock:
			iErr = iRet;
			goto labelDMsyncVol_Q;
		case EDMlongpath:
			break;
		default:
			iErr = iRet;
		}
		pCur = pCur->pNext;

	}; pCur = pModifiedDir;
	while ( pCur != NULL ) {
		pTmp = pCur;
		pCur = pCur->pNext;
		DMfree( pTmp->pszDFSModifiedPath, "DMsyncVol"  );
		DMfree( pTmp, "DMsyncVol"  );
	}

labelDMsyncVol_N:

	WaitOnLocalSemaphore( uiCopyFile_sh );
	AccessListRoot.iAccessListMode = DMAccessListModeInterval;
	DMFreeAccessFileList();
	SignalLocalSemaphore( uiCopyFile_sh );

	GetNWPath2(NULL, NULL, NULL );
	if ( pDirSync != NULL ) {
		DMfree( pDirSync, "DMsyncVol"  );
	}
	GW_tsh_functrace("DMsyncVol", GW_FUNC_END, iErr);
	return iErr;

labelDMsyncVol_Q:

	WaitOnLocalSemaphore( uiCopyFile_sh );
	AccessListRoot.iAccessListMode = DMAccessListModeInterval;
	DMFreeAccessFileList();
	SignalLocalSemaphore( uiCopyFile_sh );

	GetNWPath2(NULL, NULL, NULL );

	pCur = pModifiedDir;
	while ( pCur != NULL ) {
		pTmp = pCur;
		pCur = pCur->pNext;
		DMfree( pTmp->pszDFSModifiedPath, "DMsyncVol"  );
		DMfree( pTmp, "DMsyncVol"  );
	}
	if ( pDirSync != NULL ) {
		DMfree( pDirSync, "DMsyncVol"  );
	}
	GW_tsh_functrace("DMsyncVol", GW_FUNC_END, iErr);
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = DMsyncDir                                                            */
/*                                                                           */
/* name = Directory tree synchronization                                     */
/* func = Synchronizes all the directory trees under the designated DFS path.*/
/*                                                                           */
/* i/o  = pVolTbl:struct GVolume_t *:inp: pointer to the target volume table */
/*        pszDFSModifiedPath:char * :inp: DFS top directory to be            */
/*                                        synchronized                       */
/*        uiTopDirTime      :time_t :inp: time stamp on the top directory    */
/*        iDelMode          :int *  :inp: parameters to be passed to         */
/*                                        DeleteFile and DeleteDir           */
/* return = 0: normal end                                                    */
/*          EDMmalloc                                                        */
/*          EDMlongpath                                                      */
/*          EDMnameconv                                                      */
/*          <-MakeTree                                                       */
/*          others: error code                                               */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 16, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DMsyncDir(struct GVolume_t *pVolTbl,
				char *pszDFSModifiedPath, time_t uiTopDirTime,
				int iDelMode)
{
	int						iErr = 0;
	int						iRet;
	char					*pszNWTopPath = NULL;
	char					*pszNWParentOfTopPath = NULL;
	char					*pszNWDirname = NULL;
	char					*pszNWFilename = NULL;
	char					*pszDFSDirname = NULL;

	struct NWFakeDirEnt		*pNWCurDirDirEnt = NULL;
	struct NWFakeDirEnt		*pNWFirstFileDirEnt = NULL;
	struct NWFakeDirEnt		*pNWCurFileDirEnt = NULL;
	struct NWFakeDirEnt		*pNWFirstFileDirEntInVol = NULL;
	struct DFSFakeDirEnt	*pDFSFirstFileDirEnt = NULL;
	struct stat				*pStat = NULL;

	int						iLocation;

    GW_tsh_functrace("DMsyncDir", GW_FUNC_START, 0);

	WaitOnLocalSemaphore( uiCopyFile_sh );
	AccessListRoot.iAccessListMode = DMAccessListModeSync;
	SignalLocalSemaphore( uiCopyFile_sh );


	if ( (pszNWTopPath=(char *)GW_malloc(NW_MAX_PATH + 1)) == NULL ) {
		goto labelDMsyncDirMallocQuit;
	} else if ( (pszNWParentOfTopPath=(char *)GW_malloc(NW_MAX_PATH + 1)) == NULL ) {
		goto labelDMsyncDirMallocQuit;
	} else if ( (pszNWDirname=(char *)GW_malloc(NW_MAX_PATH + 1)) == NULL ) {
		goto labelDMsyncDirMallocQuit;
	} else if ( (pszNWFilename=(char *)GW_malloc(NW_MAX_PATH + 1)) == NULL ) {
		goto labelDMsyncDirMallocQuit;
	} else if ( (pszDFSDirname=(char *)GW_malloc(DFS_MAX_PATH + 1)) == NULL ) {
		goto labelDMsyncDirMallocQuit;
	}

	if ( (pStat=(struct stat *)GW_malloc(sizeof(struct stat)) ) == NULL ) {
		goto labelDMsyncDirMallocQuit;
	}


	iRet = GetNWPath2( pszDFSModifiedPath, pVolTbl->aszNWVol, pszNWTopPath );
	switch( iRet ) {
	case 0:
		break;
	default:
		iErr = iRet;
		goto labelDMsyncDirQuit;
	}

	if ( (pszNWTopPath[strlen(pszNWTopPath)-1] == ':') ) {

		strcat( pszNWTopPath, "\\" );

	}


	pNWCurDirDirEnt =
			(struct NWFakeDirEnt *)GW_malloc( sizeof( struct NWFakeDirEnt ) );
	if ( pNWCurDirDirEnt == NULL ) {
		goto labelDMsyncDirMallocQuit;
	} else {

		pNWCurDirDirEnt->iLink = -1;
		pNWCurDirDirEnt->byUndeleteable = FALSE;
		pNWCurDirDirEnt->iLevel = 0;
		pNWCurDirDirEnt->iOrder = 0;
		pNWCurDirDirEnt->iAttr = _A_SUBDIR;
		pNWCurDirDirEnt->pPrev = NULL;
		pNWCurDirDirEnt->pNext = NULL;
		pNWCurDirDirEnt->pParent = NULL;
		pNWCurDirDirEnt->uiTime = uiTopDirTime;
		NWBasename( pNWCurDirDirEnt->szName, pszNWTopPath );
	}


	NWDirname( pszNWParentOfTopPath, pszNWTopPath );

	while ( pNWCurDirDirEnt != NULL ) {



		if ( NWGetFullPathFromDirEnt( pszNWDirname, pszNWParentOfTopPath,
				pNWCurDirDirEnt) ) {

			NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
			continue;
		}

		iRet = GetDFSPath( pszNWDirname, pszDFSDirname, OPENMODE );
		if ( iRet == GWERR_NOMEM ) {

			NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
			iErr = EDMwarning;
			continue;
		} else if ( iRet != 0 ) {

			NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
			iErr = EDMwarning;
			continue;
		}


		if ( pNWCurDirDirEnt->iLink == -1 ) {

			if ( *(pNWCurDirDirEnt->szName) != '\0' ) {
				iLocation = 1;
			} else {
				iLocation = 0;
			}

			pNWCurDirDirEnt->iLink = 0;

			if ( iLocation != 0 ) {

				iRet = stat( pszNWDirname, pStat );
				if ( iRet ) {
					if ( errno != ENOENT ) {
						NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
						continue;
					} else {
						iRet = MakeTree( pVolTbl, pszDFSDirname, pszNWDirname );
						switch ( iRet ) {
						case EDMshutdownreq:
						case EDMagentdie:
							iErr = iRet;
							goto labelDMsyncDir_Q;
						case EDMdiskspace:
							iErr = iRet;
							goto labelDMsyncDir_Q;
						case EDMdirectoryblock:
							iErr = iRet;
							goto labelDMsyncDir_Q;
						default:
							iErr = EDMwarning;
						}
						NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
						continue;
					}
				} else {
					if ( pStat->st_attr & _A_SUBDIR ) {
						TouchDir( pszNWDirname, pNWCurDirDirEnt->uiTime,
								pStat, DMMkModeExist );
					} else {
						DeleteFile( pszNWDirname, iDelMode );
						iRet = MakeTree( pVolTbl, pszDFSDirname, pszNWDirname );
						switch ( iRet ) {
						case EDMshutdownreq:
						case EDMagentdie:
							iErr = iRet;
							goto labelDMsyncDir_Q;
						case EDMdiskspace:
							iErr = iRet;
							goto labelDMsyncDir_Q;
						case EDMdirectoryblock:
							iErr = iRet;
							goto labelDMsyncDir_Q;
						default:
							iErr = EDMwarning;
						}
						NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
						continue;
					}
				}

			}


			if ( iRet = NWChainFakeDirEnt(	pszNWDirname,
											&pNWFirstFileDirEntInVol,
											0,
											NULL ) ) {
				iErr = EDMwarning;
			}


			if ( iRet = DFSChainFakeDirEnt(	pszDFSDirname,
											&pDFSFirstFileDirEnt,
											pNWCurDirDirEnt->iLevel + 1,
											NULL ) ) {
				switch ( iRet ) {
				case EDMshutdownreq:
				case EDMagentdie:
					iErr = iRet;
					goto labelDMsyncDir_Q;
				default:
					iErr = EDMwarning;
				}
			}

			if ( iRet = NWChainFakeDirEntFromDFSOne(
											pVolTbl,
											pszDFSDirname,
											pDFSFirstFileDirEnt,
											&pNWFirstFileDirEnt) ) {
				iErr = EDMwarning;
			}
			pNWCurFileDirEnt = pNWFirstFileDirEnt;
			DFSFreeFakeDirEntList( pDFSFirstFileDirEnt );
			pDFSFirstFileDirEnt = NULL;




			iRet = DeleteUnknownFile(	pszNWDirname,
										pNWFirstFileDirEnt,
										pNWFirstFileDirEntInVol,
										iDelMode );
			switch ( iRet ) {
			case 0:	case EDMlongpath:
				break;
			default:
				iErr = EDMwarning;
				break;
			}

			NWFreeFakeDirEntList( pNWFirstFileDirEntInVol );
			pNWFirstFileDirEntInVol = NULL;


			while ( pNWCurFileDirEnt != NULL ) {
				if ( iLocation != 0) {
					if ( (strlen(pszNWDirname) + 1 +
							strlen(pNWCurFileDirEnt->szName)) <= _MAX_PATH ) {
						sprintf(	pszNWFilename, "%s\\%s",
									pszNWDirname, pNWCurFileDirEnt->szName );
					} else {
						NWDeleteFakeDirEnt(pNWCurFileDirEnt, &pNWCurFileDirEnt);
						continue;
					}
				} else {
					sprintf(	pszNWFilename, "%s%s",
								pszNWDirname, pNWCurFileDirEnt->szName );
				}

				if ( pNWCurFileDirEnt->iAttr & _A_SUBDIR ) {

					pNWCurFileDirEnt->pParent = pNWCurDirDirEnt;
					NWInsertDirEnt( pNWCurDirDirEnt, pNWCurFileDirEnt );
				} else {


					if ( stat( pszNWFilename, pStat ) ) {
						if ( errno != ENOENT ) {
							NWDeleteFakeDirEnt(pNWCurFileDirEnt, &pNWCurFileDirEnt);
							continue;
						} else {
							iRet = TouchFile(
									pszNWFilename, pNWCurFileDirEnt->iSize,
									pNWCurFileDirEnt->uiTime,
									NULL, DMMkModeNot );
							if ( iRet ) {
								iErr = EDMwarning;
							}
						}
					} else {
						if ( pStat->st_attr & _A_SUBDIR ) {
							DeleteTree( pszNWFilename, iDelMode );
							iRet = TouchFile(
									pszNWFilename, pNWCurFileDirEnt->iSize,
									pNWCurFileDirEnt->uiTime,
									pStat, DMMkModeNot );
						} else {
							iRet = TouchFile(
									pszNWFilename, pNWCurFileDirEnt->iSize,
									pNWCurFileDirEnt->uiTime,
									pStat, DMMkModeExist );
							if ( iRet ) {
								iErr = EDMwarning;
							}
						}
					}
				}
				NWDeleteFakeDirEnt(pNWCurFileDirEnt, &pNWCurFileDirEnt);

			}
			NWMoveToNextFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );

		} else {

			NWDeleteFakeDirEnt( pNWCurDirDirEnt, &pNWCurDirDirEnt );
		}

	}

	DMfree( pszNWTopPath, "DMsyncVol"  );
	DMfree( pszNWParentOfTopPath, "DMsyncVol"  );
	DMfree( pszNWDirname, "DMsyncVol"  );
	DMfree( pszNWFilename, "DMsyncVol" );
	DMfree( pszDFSDirname, "DMsyncVol" );
	if ( pStat != NULL ) {
		DMfree( pStat, "DMsyncVol"  );
	}
	GW_tsh_functrace("DMsyncDir", GW_FUNC_END, iErr);
	return iErr;

labelDMsyncDirMallocQuit:
	GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
	iErr = EDMmalloc;
labelDMsyncDir_Q:
	if ( pNWCurDirDirEnt != NULL ) {
		NWFreeFakeDirEntList( pNWCurDirDirEnt ); pNWCurDirDirEnt = NULL;
	}
labelDMsyncDirQuit:
	if ( pNWCurFileDirEnt != NULL ) {
		NWFreeFakeDirEntList( pNWCurFileDirEnt );
		pNWCurFileDirEnt = NULL;
	}
	if ( pNWFirstFileDirEntInVol != NULL ) {
		NWFreeFakeDirEntList( pNWFirstFileDirEntInVol );
		pNWFirstFileDirEntInVol = NULL;
	}
	if ( pDFSFirstFileDirEnt != NULL ) {
		DFSFreeFakeDirEntList( pDFSFirstFileDirEnt );
		pDFSFirstFileDirEnt = NULL;
	}
	if ( pszNWTopPath != NULL ) {
		DMfree( pszNWTopPath, "DMsyncVol"  );
	}
	if ( pszNWParentOfTopPath != NULL ) {
		DMfree( pszNWParentOfTopPath, "DMsyncVol"  );
	}
	if ( pszNWDirname != NULL ) {
		DMfree( pszNWDirname, "DMsyncVol"  );
	}
	if ( pszNWFilename != NULL ) {
		DMfree( pszNWFilename, "DMsyncVol"  );
	}
	if ( pszDFSDirname != NULL ) {
		DMfree( pszDFSDirname, "DMsyncVol"  );
	}
	if ( pStat != NULL ) {
		DMfree( pStat, "DMsyncVol"  );
	}

	WaitOnLocalSemaphore( uiCopyFile_sh );
	AccessListRoot.iAccessListMode = DMAccessListModeInterval;
	DMFreeAccessFileList();
	SignalLocalSemaphore( uiCopyFile_sh );

	GW_tsh_functrace("DMsyncDir", GW_FUNC_END, iErr);
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = DeleteUnknowFile                                                     */
/*                                                                           */
/* name = Deletion a file that exists on NetWare but not on DFS              */
/* func = Deletes a file that exists on DFS but not on NetWare               */
/*        (1) Compares all the DFS files/directories under the designated    */
/*            directory with those of NetWare.                               */
/*        (2) If there is a file/directory tree that exists on DFS but not   */
/*            on NetWare, makes the file/directory on NetWare.               */
/*        (3) If there is a file/directory tree that exists on NetWare but   */
/*            not on DFS, deletes the file/directory from NetWare.           */
/*                                                                           */
/* i/o  = pszNWDirname:char *               :inp: target directory           */
/*        pNW1stDirEnt:struct NWFakeDirEnt *:inp: a list of NetWare file     */
/*                                           names which were converted from */
/*                                           the DFS file names              */
/*        pNW1stDirEntInVol:struct NWFakeDirEntInVol *:inp: a list of files  */
/*                                           residing on the NetWare side    */
/*        iDelMode: int * :inp: parameters to be passed to DeleteFile and    */
/*                              DeleteDir                                    */
/* :                                                                         */
/* return = 0: normal end                                                    */
/*          EDMmalloc                                                        */
/*          EDMwarning                                                       */
/*          <-DeleteFile                                                     */
/*          <-DeleteTree                                                     */
/*                                                                           */
/* common = apszLogName: A string registering the file names reserved by     */
/*                       the system.                                         */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 16, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DeleteUnknownFile(
			char					*pszNWDirname,
			struct NWFakeDirEnt		*pNW1stDirEnt,
			struct NWFakeDirEnt		*pNW1stDirEntInVol,
			int						iDelMode)
{
	int						iErr = 0;
	int						iRet = 0;
	struct NWFakeDirEnt		*pNWDirEnt;
	struct NWFakeDirEnt		*pNWDirEntInVol;
	char					*pszNWFullPath;

    GW_tsh_functrace("DeleteUnknownFile", GW_FUNC_START, 0);


	if ( (pszNWFullPath=(char *)GW_malloc(NW_MAX_PATH + 1)) == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613);
		GW_tsh_functrace("DeleteUnknownFile", GW_FUNC_END, EDMmalloc);
		return EDMmalloc;
	}

	pNWDirEntInVol = pNW1stDirEntInVol;
	while ( pNWDirEntInVol != NULL ) {
		pNWDirEnt = pNW1stDirEnt;

		while ( pNWDirEnt != NULL ) {
			if ( !strcmp( pNWDirEnt->szName, pNWDirEntInVol->szName ) ) {
				break;
			}
			ThreadSwitch();
			pNWDirEnt = pNWDirEnt->pNext;
		}
		if ( pNWDirEnt == NULL ) {
			char	*pDe;
			pDe = D_strrchr( pszNWDirname, '\\' );
			if ( (pDe != NULL) &&
					(pDe == (pszNWDirname+strlen(pszNWDirname)-1) ) ) {
				sprintf( pszNWFullPath, "%s%s", pszNWDirname, pNWDirEntInVol->szName );
			} else {
				if ( (strlen(pszNWDirname) + 1 + strlen(pNWDirEntInVol->szName))
						<= _MAX_PATH ) {
					sprintf( pszNWFullPath, "%s\\%s", pszNWDirname,
							pNWDirEntInVol->szName );
				} else {
					pNWDirEntInVol = pNWDirEntInVol->pNext;
					continue;
				}
			}
			if ( pNWDirEntInVol->iAttr & _A_SUBDIR ) {
				iRet = DeleteTree( pszNWFullPath, iDelMode);
				switch ( iRet ) {
				case 0:	case EDMsystemfile:
					break;
				default:
					iErr = iRet;
				}
			} else {
				iRet = DeleteFile( pszNWFullPath, iDelMode);
				switch ( iRet ) {
				case 0:	case EDMsystemfile:
					break;
				default:
					iErr = iRet;
				}
			}
		}
		ThreadSwitch();
		pNWDirEntInVol = pNWDirEntInVol->pNext;
	}

	DMfree( pszNWFullPath, "DeleteUnknownFile" );
	GW_tsh_functrace("DeleteUnknownFile", GW_FUNC_END, iErr);
	return iErr;
}
