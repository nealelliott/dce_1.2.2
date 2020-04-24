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
 * $Log: gdirclr.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:44  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:08  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:44:32  takano
 * 	First Submit
 * 	[1996/02/21  07:44:27  takano]
 * 
 * $EndLog$
 */
#include "gdir.h"

/*********************************************************** EnglishComment **/
/* ID = DMclear                                                              */
/*                                                                           */
/* name = Clearing DFAM Volume (when unload)                                 */
/* func = Clears all the volumes by invoking ClearVolume for each volume     */
/*        listed in DFAM Volume Table.                                       */
/*                                                                           */
/* i/o  = iMode: int * :inp: parameter for DeleteFile,DeleteDir function     */
/*                                                                           */
/* return =  0: normal end                                                   */
/*	     EDMwarning                                                          */
/* common = DFAM Volume table                                                */
/*	:::                                                                      */
/* note = none                                                               */
/* date = December 2, 1994 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DMclear( int iMode )
{
	int		iErr = 0;
	int		iRet;
	struct GVolume_t	*pCurVol;

    GW_tsh_functrace("DMclear", GW_FUNC_START, 0);
	pCurVol = pVolume_sp;
	while ( pCurVol != NULL ) {

		if ( !CheckVolumeMap( pCurVol->aszNWVol ) ) {
			if (pCurVol->usVolStatus & VOL_VIRGIN) {
				pCurVol = pCurVol->pUshirop;
				continue;
			} else if (pCurVol->usVolStatus & NW_USERSKIP) {
				pCurVol = pCurVol->pUshirop;
				continue;
			}

			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11634, pCurVol->aszNWVol );
			iRet = ClearVolume( pCurVol, iMode );
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11635, pCurVol->aszNWVol );

			if ( iRet ) {
				iErr = EDMwarning;
			}
		}
		pCurVol = pCurVol->pUshirop;
	}

	GW_tsh_functrace("DMclear", GW_FUNC_END, iErr);
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = ClearVolume                                                          */
/*                                                                           */
/* name = Clearing a particular DFAM Volume                                  */
/* func = Clears Volumes indicated by the designated DFAM Volume Table.      */
/*        If UNLOAD was request while the clearing, stop clearing , and goes */
/*        back to the main routine of the directory synchronization.         */
/*                                                                           */
/* i/o  = pVolTbl:struct GVolume_t *:inp: pointer to Volume Table that has a */
/*                                        list of DFAM Volumes to be cleared.*/
/*        iDeleteMode:int           :inp: file deletion mode (this tells     */
/*                                        where is this function invoked: at */
/*                                        initialization or termination)     */
/* return =  0: normal end                                                   */
/*           EDMkeepfile                                                     */
/*           EDMagentdie                                                     */
/*           EDMshutdownreq                                                  */
/*	     EDMwarning                                                          */
/*                                                                           */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 2, 1994 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int ClearVolume( struct GVolume_t *pVolTbl, int iDeleteMode )
{
	int						iErr = 0;
	int						iRet = 0;
	char					*pszNWFullPath = NULL;
	struct NWFakeDirEnt		*pNWCurDirEnt = NULL;
	struct NWFakeDirEnt		*pNWCurDirEntTmp = NULL;

    GW_tsh_functrace("ClearVolume", GW_FUNC_START, 0);

	pszNWFullPath = (char *)GW_malloc( _MAX_PATH + 1 );
	if ( pszNWFullPath == NULL ) {
		iErr = EDMmalloc;
		goto labelClearVolume_Q;
	}


	sprintf( pszNWFullPath, "%s:", pVolTbl->aszNWVol );
	if ( iRet = NWChainFakeDirEnt(	pszNWFullPath,
									&pNWCurDirEnt,
									0,
									NULL ) ) {
		iErr = EDMwarning;
	}

	if ( (iDeleteMode == DMDelModeInit) &&  bMountWarning ) {
		pNWCurDirEntTmp = pNWCurDirEnt;
		while ( pNWCurDirEntTmp != NULL ) {
			if ( !CheckSystemFile( pNWCurDirEntTmp->szName ) ) {
				iRet = DMYesNo( pVolTbl->aszNWVol );
				if ( iRet == EDMno ) {
					iErr = EDMkeepfile;
					goto labelClearVolume_Q;
				} else if ( iRet == EDMshutdownreq ) {
					iErr = iRet;
					goto labelClearVolume_Q;
				} else {
					break;
				}
			}
			pNWCurDirEntTmp = pNWCurDirEntTmp->pNext;
		}
	}


	while ( pNWCurDirEnt != NULL ) {
		sprintf( pszNWFullPath, "%s:\\%s", pVolTbl->aszNWVol, pNWCurDirEnt->szName );
		if ( !CheckSystemFile( pNWCurDirEnt->szName ) ) {

			if ( pNWCurDirEnt->iAttr & _A_SUBDIR ) {

				iRet = DeleteTree( pszNWFullPath, iDeleteMode );
				switch ( iRet ) {
				case EDMshutdownreq:
				case EDMagentdie:
					iErr = iRet;
					goto labelClearVolume_Q;
				case 0:
					break;
				case EDMkeepfile:
					iErr = EDMkeepfile;
					goto labelClearVolume_Q;
				default:
					iErr = EDMwarning;
				}
			} else {

				iRet = DeleteFile( pszNWFullPath, iDeleteMode );
				switch ( iRet ) {
				case EDMshutdownreq:
				case EDMagentdie:
					iErr = iRet;
					goto labelClearVolume_Q;
				case 0:
					break;
				case EDMkeepfile:
					iErr = EDMkeepfile;
					goto labelClearVolume_Q;
				default:
					iErr = EDMwarning;
				}
			}
		} else {
		}
		NWDeleteFakeDirEnt( pNWCurDirEnt, &pNWCurDirEnt );
	}

labelClearVolume_Q:
	if ( pszNWFullPath != NULL ) {
		GW_free( pszNWFullPath, 0 );
	}
	if ( pNWCurDirEnt != NULL ) {
		NWFreeFakeDirEntList( pNWCurDirEnt );
	}
	GW_tsh_functrace("ClearVolume", GW_FUNC_END, iErr);
	return iErr;
}
