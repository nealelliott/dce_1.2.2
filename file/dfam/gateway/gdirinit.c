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
 * $Log: gdirinit.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:50  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:13  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:47:47  takano
 * 	First Submit
 * 	[1996/02/21  07:47:43  takano]
 * 
 * $EndLog$
 */
#include "gdir.h"

/*********************************************************** EnglishComment **/
/* ID = DMinit                                                               */
/*                                                                           */
/* name = Directory synchronization and initialization                       */
/* func = Initializes all the volumes.                                       */
/*        If there occurs a serious error that needs to terminate Gateway,   */
/*        EDMinitfatal is returned.                                          */
/*                                                                           */
/* i/o  = piSuccess:int *:out: the number of successfully initialized volumes*/
/* return = EDMnullptr                                                       */
/*          EDMagentdie                                                      */
/*          EDMshutdownreq                                                   */
/*          EDManytimeout: each volume is dfsnoent or timeout                */
/*          EDMdirectoryblock                                                */
/*          EDMdiskspace                                                     */
/* common = none                                                             */
/* :::                                                                       */
/* note = (1) The lower routines initialize each volume.                     */
/*        (2) If an error occurred at the lower routines, the lower routine  */
/*            clear the volules, and free the volume tables.                 */
/* date = December 2, 1994 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DMinit( int *piSuccess )
{
	int			iErr = 0;
	int			iRet = 0;
	int			iTimeout = 0;
	struct GVolume_t	*pCurVol;
	struct GVolume_t	*pTmp;

    GW_tsh_functrace("DMinit", GW_FUNC_START, 0);

	*piSuccess = 0;

	if ( piSuccess == NULL ) {
		GW_tsh_functrace("DMinit", GW_FUNC_END, EDMnullptr);
		return EDMnullptr;
	}

	pCurVol = pVolume_sp;
	while ( pCurVol != NULL ) {
		if( ShutdownReq() ) { return EDMshutdownreq; }
		if( AgentDie() ) { return EDMagentdie; }
		GW_Message(DFAM_CON, 0, LOG_YES, KDDS11631, pCurVol->aszNWVol);

		iRet =CheckVolumeMap( pCurVol->aszNWVol );
		if ( iRet ) {
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11601, pCurVol->aszNWVol );
			GW_Message(DFAM_CON, 0, LOG_YES, KDDS11633, pCurVol->aszNWVol);
			pTmp = pCurVol;
			pCurVol->usVolStatus |= NW_NOENT;
			pCurVol->uiSynctime = 0;
			pCurVol = pCurVol->pUshirop;
			DeleteFromGVolume_t( pTmp );
			continue;
		} else {
			iRet = ClearVolume( pCurVol, DMDelModeInit );
			switch ( iRet ) {
			case 0:
				pCurVol->usVolStatus &= ~VOL_VIRGIN;
				break;
			case EDMshutdownreq:
			case EDMagentdie:
				iErr = iRet;
				pCurVol = pCurVol->pUshirop;
				continue;
			case EDMvolumefatal:
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11633, pCurVol->aszNWVol);
				pTmp = pCurVol;
				pCurVol->usVolStatus |= NW_NOENT;
				iErr = iRet;
				pCurVol = pCurVol->pUshirop;
				DeleteFromGVolume_t( pTmp );
				continue;
			case EDMkeepfile:
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11633, pCurVol->aszNWVol);
				pTmp = pCurVol;
				pCurVol->usVolStatus |= NW_USERSKIP;
				iErr = iRet;
				pCurVol = pCurVol->pUshirop;
				DeleteFromGVolume_t( pTmp );
				continue;
			default:
				pCurVol->usVolStatus &= ~VOL_VIRGIN;
				break;
			}


			iRet = DMInitVolume( pCurVol );
			switch ( iRet ) {
			case 0:
				break;
			case EDMshutdownreq:
			case EDMagentdie:
				iErr = iRet;
				goto labelDMinit_Q;
			case DENOENT:
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11633, pCurVol->aszNWVol);
				pTmp = pCurVol;
				pCurVol->usVolStatus |= DFS_NOENT;
				pCurVol = pCurVol->pUshirop;
				DeleteFromGVolume_t( pTmp );
				continue;
			case DEACCES:
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11633, pCurVol->aszNWVol);
				pTmp = pCurVol;
				pCurVol->usVolStatus |= DFS_ACCESS;
				pCurVol = pCurVol->pUshirop;
				DeleteFromGVolume_t( pTmp );
				continue;
			case EDMdiskspace:
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11671, pCurVol->aszNWVol);
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11633, pCurVol->aszNWVol);
				ClearVolume( pCurVol, DMDelModeClear );
				pTmp = pCurVol;
				iErr = iRet;
				pCurVol = pCurVol->pUshirop;
				DeleteFromGVolume_t( pTmp );
				continue;
			case EDMdirectoryblock:
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11672, pCurVol->aszNWVol);
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11633, pCurVol->aszNWVol);
				ClearVolume( pCurVol, DMDelModeClear );
				pTmp = pCurVol;
				iErr = iRet;
				pCurVol = pCurVol->pUshirop;
				DeleteFromGVolume_t( pTmp );
				continue;
			default:
				if ( iRet < 1200 ) {
					GW_Message(DFAM_CON, 0, LOG_YES, KDDS11633, pCurVol->aszNWVol);
					pCurVol->usVolStatus |= DFS_TIMEOUT;
					iTimeout = 1;
					pCurVol = pCurVol->pUshirop;
					continue;
				} else {
					iErr = EDMwarning;
					break;
				}
			}
		}

		(*piSuccess)++;
		GW_Message(DFAM_CON, 0, LOG_YES, KDDS11632, pCurVol->aszNWVol);
		pCurVol = pCurVol->pUshirop;
	}

	if ( iTimeout ) {
		iErr = EDManytimeout;
	}
	GW_tsh_functrace("DMinit", GW_FUNC_END, iErr);
	return iErr;

labelDMinit_Q:
	GW_tsh_functrace("DMinit", GW_FUNC_END, iErr);
	return iErr;

}

/*********************************************************** EnglishComment **/
/* ID = DMInitVolume                                                         */
/*                                                                           */
/* name = DFS directory tree copying                                         */
/* func = Copies the DFS Directory tree to a NetWare Volume.                 */
/* i/o  = pVolTbl:struct GVolume_t *:inp: target volume                      */
/*                                                                           */
/* return = 0: normal end                                                    */
/*          EDMagentdie:                                                     */
/*          EDMshutdownreq:                                                  */
/*          EDMtimeout:                                                      */
/*          EDMdfsnoent:                                                     */
/*          DENOENT:                                                         */
/*          DEACCES:                                                         */
/*          <-DFSChainFakeDirEnt                                             */
/*          EDMwarning:                                                      */
/*          EDMdiskspace:                                                    */
/*          EDMdirectoryblock:                                               */
/* common = none                                                             */
/* :::                                                                       */
/* note = (1) Since DMInitVolume(), an upper routine, has cleared all the    */
/*            volume, this function only copies the DFS directory structure. */
/*        (2) If an error occurred, the upper routines clear the volules.    */
/* date = December 12, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DMInitVolume( struct GVolume_t *pVolTbl )
{
	int						iErr = 0;
	int						iRet = 0;
	int						iDFSRet;
	time_t					uiNewTime;
	char					*pszNWFullPath = NULL;
	char					*pszDFSFullPath = NULL;
	struct DFSFakeDirEnt	*pDFSFirstDirEnt = NULL;
	struct DFSFakeDirEnt	*pDFSCurDirEnt = NULL;

    GW_tsh_functrace("DMInitVolume", GW_FUNC_START, 0);

	pszNWFullPath = (char *)GW_malloc( NW_MAX_PATH + 1 );
	pszDFSFullPath = (char *)GW_malloc( DFS_MAX_PATH + 1 );
	if ( (pszNWFullPath == NULL) || (pszDFSFullPath == NULL) ) {
		iErr = EDMmalloc;
		goto labelDMInitVolume_Q;
	}


	if ( (iRet = DFStimeReq( NULL, &iDFSRet, &gComS )) == -1 ) {
		switch ( iDFSRet ) {

		case DENETWORKENV:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11651);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMInitVolume_Q;
		case DEUNLOADTCPIP:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11204);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMInitVolume_Q;
		case DEREFUSEAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMInitVolume_Q;
		case DEINVALSOCKET:	case DERESPONCEDATA:	case DEDFAMDCESYSTEM:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMInitVolume_Q;
		case DEDFAMTONOVELL:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11681, DEDFAMTONOVELL);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMInitVolume_Q;
		case DETIMEOUTAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11654);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDMInitVolume_Q;
		case DEDFAMTERM:
			iErr = EDMshutdownreq;
			goto labelDMInitVolume_Q;
		default:
			break;
		}
		uiNewTime = 0;
	} else {
		uiNewTime = iRet;
	}


	if ( iRet = DFSChainFakeDirEnt(	pVolTbl->aszDFSPath,
								&pDFSFirstDirEnt,
								0,
								NULL ) ) {
		switch ( iRet ) {
		case EDMshutdownreq:
		case EDMagentdie:
			iErr = iRet;
			goto labelDMInitVolume_Q;
		case DENOENT:
			GW_Message(DFAM_CON, 0, LOG_YES, KDDS11664, pVolTbl->aszDFSPath);
			iErr = iRet;
			goto labelDMInitVolume_Q;
		case DEACCES:
			GW_Message(DFAM_CON, 0, LOG_YES, KDDS11666, pVolTbl->aszDFSPath);
			iErr = iRet;
			goto labelDMInitVolume_Q;
		default:
			if ( iRet < 1200 ) {
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11663, iRet);
				GW_Message(DFAM_CON, 0, LOG_YES, KDDS11665,pVolTbl->aszDFSPath);
				iErr = iRet;
				goto labelDMInitVolume_Q;
			}
		}
		iErr = EDMwarning;
	}
	pDFSCurDirEnt = pDFSFirstDirEnt;


	while ( pDFSCurDirEnt != NULL ) {

		sprintf( pszDFSFullPath, "%s/%s",
				pVolTbl->aszDFSPath, pDFSCurDirEnt->szName );

		if ( iRet = GetNWPath( pszDFSFullPath, pVolTbl->aszNWVol, pszNWFullPath,
				pDFSFirstDirEnt ) ) {
		}
		if( CheckSystemFile( pszNWFullPath ) ) {

			DeleteName_tByName( pszNWFullPath );

		} else {
			if ( pDFSCurDirEnt->usMode & S_IFDIR ) {

				iRet = MakeTree( pVolTbl, pszDFSFullPath, pszNWFullPath );
				switch ( iRet ) {
				case 0:
					break;
				case EDMshutdownreq:
				case EDMagentdie:
					iErr = iRet;
					goto labelDMInitVolume_Q;
				case EDMdiskspace:	case EDMdirectoryblock:
					iErr = iRet;
					goto labelDMInitVolume_Q;
				default:
					iErr = EDMwarning;
				}
			} else {

				iRet = TouchFile( pszNWFullPath,
						pDFSCurDirEnt->iSize, pDFSCurDirEnt->uiTime,
						NULL, DMMkModeNot );
				switch ( iRet ) {
				case 0:
					break;
				case EDMdiskspace:	case EDMdirectoryblock:
					iErr = iRet;
					goto labelDMInitVolume_Q;
				default:
					iErr = EDMwarning;
				}
			}
		}


		pDFSCurDirEnt = pDFSCurDirEnt->pNext;
	}

	if ( pDFSFirstDirEnt != NULL ) {
		DFSFreeFakeDirEntList( pDFSFirstDirEnt ); pDFSFirstDirEnt = NULL;
	}
	if ( iErr == 0 ) {
		pVolTbl->uiSynctime = uiNewTime;
	}

labelDMInitVolume_Q:
	if ( pDFSFirstDirEnt != NULL ) {
		DFSFreeFakeDirEntList( pDFSFirstDirEnt ); pDFSFirstDirEnt = NULL;
	}
	if ( pszDFSFullPath != NULL )
		GW_free( pszDFSFullPath, 0 );
	if ( pszNWFullPath != NULL )
		GW_free( pszNWFullPath, 0 );
    GW_tsh_functrace("DMInitVolume", GW_FUNC_END, iErr);
	return iErr;
}
