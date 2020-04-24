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
 * $Log: gdirent.c,v $
 * Revision 1.1.2.3  1996/03/22  07:01:59  takano
 * 	Fixed the confliction between the Directory Synchronization and the File-Access.
 * 	[1996/03/22  06:59:05  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:07:45  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:09  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  07:46:11  takano
 * 	First Submit
 * 	[1996/02/21  07:45:19  takano]
 * 
 * $EndLog$
 */
#include "gdir.h"

/*********************************************************** EnglishComment **/
/* ID = DFSChainFakeDirEnt                                                   */
/*                                                                           */
/* name = Expanding DFS directory entries                                    */
/* func = Sets all the information of the files and directories under the    */
/*        designated DFS directory to the structure of DFSFakeDirEnt, and    */
/*        connects the structures.                                           */
/*                                                                           */
/* i/o  = pszDFSDir  :char *  :inp: the top directory to start searching     */
/*        ppFirst    :struct DFSFakeDirEnt *  :inp: the target directory     */
/*        iLevel     :int     :inp: the depth of the target directory (from  */
/*                                  the top directory)                       */
/*        piLink     :int *   :inp: the number of files under the top        */
/*                                  directory (option)                       */
/*                                                                           */
/* return = 0: successful                                                    */
/*	    EDMagentdie: communication broken                                    */
/*	    EDMshutdownreq: UNLOAD detected                                      */
/*	    EDMwarning                                                           */
/*	    EDMbug                                                               */
/*	    <-DFSopendirReq                                                      */
/*	    <-DFSclosedirReq                                                     */
/*	    <-DFSreadstatReq                                                     */
/*                                                                           */
/* common = none                                                             */
/* :::                                                                       */
/* note = (1) If communication error occurs, this function frees the         */
/*        dynamically allocated memory.  In the case on non-communication    */
/*        error (such as DFSreadstatReq and DFSclosedirReq), this function   */
/*        does not discard the generated list.                               */
/*        (2) If this function detects UNLOAD, it frees all memory allocated */
/*        in this function, and returns EDMshutdownreq.                      */
/*                                                                           */
/* date = December 15, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DFSChainFakeDirEnt(		char	*pszDFSDir,
							struct	DFSFakeDirEnt **ppFirst,
							int 	iLevel,
							int		*piLink )
{
	struct	DFSFakeDirEnt	*pPrev = NULL;
	struct	DFSFakeDirEnt	*pCur = NULL;
	struct	DFSFakeDirEnt	*pDFS0th = NULL;

	DDIR				*pDFSdir = NULL;
	struct ddirent		*pDFSdirent = NULL;
	int					iDFSErr;
	int					iOrder = 0;
	int					iRet;
	int					iErr = 0;
	int					iLen;


	*ppFirst = NULL;
	if ( piLink != NULL ) {
		*piLink = 0;
	}


	pDFS0th = GW_malloc( sizeof(struct DFSFakeDirEnt) );
	if ( pDFS0th == NULL ) { goto labelDFSChainFakeDirEnt_Q; }
	pDFSdir = GW_malloc( sizeof(DDIR) );
	if ( pDFSdir == NULL ) { goto labelDFSChainFakeDirEnt_Q; }
	pDFSdirent = GW_malloc( sizeof(struct ddirent) );
	if ( pDFSdirent == NULL ) { goto labelDFSChainFakeDirEnt_Q; }

	pDFS0th->pNext = NULL;
	pPrev = pDFS0th;


	iLen = strlen( pszDFSDir );
	if ( strcmp( pszDFSDir, "/" ) && (pszDFSDir[iLen-1] == '/') ) {
		pszDFSDir[iLen-1] = '\0';
	}

	if( ShutdownReq() ) { iErr=EDMshutdownreq; goto labelDFSChainFakeDirEnt_Q; }
	if( AgentDie() ) { iErr=EDMagentdie; goto labelDFSChainFakeDirEnt_Q; }

	if( iRet = DFSopendirReq( pszDFSDir, pDFSdir, &iDFSErr, &gComS ) ) {
		switch ( iDFSErr ) {

		case DENETWORKENV:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11651);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEUNLOADTCPIP:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11204);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEREFUSEAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEINVALSOCKET:	case DERESPONCEDATA:	case DEDFAMDCESYSTEM:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEDFAMTONOVELL:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11681, DEDFAMTONOVELL);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DETIMEOUTAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11654);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEDFAMTERM:
			iErr = EDMshutdownreq;
			goto labelDFSChainFakeDirEnt_Q;

		default:
			if ( iDFSErr < 1200 ) {
				iErr = iDFSErr;
			} else {
				GW_Message( DFAM_CON, 0, LOG_YES, KDDS11624, iDFSErr );
				iErr = EDMbug;
			}
			goto labelDFSChainFakeDirEnt_Q;
		}
	}

labelDFSChainFakeDirEnt_continueDFSreadstatReq:
	while(  !DFSreadstatReq(	pDFSdir,
							DFSreadstatAll,
							pDFSdirent,
							&iDFSErr,
							&gComS ) ) {

		pCur = (struct DFSFakeDirEnt *)GW_malloc( sizeof(struct DFSFakeDirEnt));
		if ( pCur == NULL ) {
			iErr = EDMwarning;
			continue;
		}
		pCur->iLink = -1;
		pCur->iLevel = iLevel;
		pCur->iOrder = iOrder;
		pCur->usMode = pDFSdirent->d_mode;
		pCur->uiTime = pDFSdirent->d_mtime;
		pCur->iSize = pDFSdirent->d_size;
		pCur->byUndeleteable = 0;
		if ( pPrev == pDFS0th ) {
			pCur->pPrev = NULL;
		} else {
			pCur->pPrev = pPrev;
		}
		pCur->pParent = NULL;
		pCur->pNext = NULL;
		DFSBasename( pCur->szName, pDFSdirent->d_name );
		pPrev->pNext = pCur;

		pPrev = pCur;
		iOrder++;
	}
	if ( iDFSErr ) {
		switch ( iDFSErr ) {

		case DENETWORKENV:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11651);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEUNLOADTCPIP:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11204);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEREFUSEAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEINVALSOCKET:	case DERESPONCEDATA:	case DEDFAMDCESYSTEM:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEDFAMTONOVELL:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11681, DEDFAMTONOVELL);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DETIMEOUTAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11654);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEDFAMTERM:
			iErr = EDMshutdownreq;
			goto labelDFSChainFakeDirEnt_Q;

		case DENOENT:	case DENAMETOOLONG:
			goto labelDFSChainFakeDirEnt_continueDFSreadstatReq;


		default:
			if ( iDFSErr < 1200 ) {
				iRet = DFSclosedirReq( pDFSdir, &iDFSErr, &gComS );
				iErr = iDFSErr;
			} else {
				GW_Message( DFAM_CON, 0, LOG_YES, KDDS11624, iDFSErr );
				iErr = EDMbug;
			}
			goto labelDFSChainFakeDirEnt_Q;
		}
	}

	iRet = DFSclosedirReq( pDFSdir, &iDFSErr, &gComS );
	if ( iRet ) {
		switch ( iDFSErr ) {

		case DENETWORKENV:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11651);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEUNLOADTCPIP:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11204);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEREFUSEAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEINVALSOCKET:	case DERESPONCEDATA:	case DEDFAMDCESYSTEM:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEDFAMTONOVELL:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11681, DEDFAMTONOVELL);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DETIMEOUTAGT:
			GW_Message( DFAM_CON, 0, LOG_YES, KDDS11654);
			SetDeathFlag();
			iErr = EDMagentdie;
			goto labelDFSChainFakeDirEnt_Q;
		case DEDFAMTERM:
			iErr = EDMshutdownreq;
			goto labelDFSChainFakeDirEnt_Q;
		default:
			if ( iDFSErr >= 1200 ) {
				GW_Message( DFAM_CON, 0, LOG_YES, KDDS11653);
				SetDeathFlag();
				iErr = EDMagentdie;
				goto labelDFSChainFakeDirEnt_Q;
			}
			iErr = EDMwarning;
			break;
		}
	}

	*ppFirst = pDFS0th->pNext;
	if ( piLink != NULL ) {
		*piLink = iOrder;
	}
	DMfree( pDFS0th, "DFSChainFakeDirEnt" );
	DMfree( pDFSdir, "DFSChainFakeDirEnt"  );
	DMfree( pDFSdirent, "DFSChainFakeDirEnt"  );
	return iErr;

labelDFSChainFakeDirEnt_Q:
	if (pDFS0th->pNext != NULL) { DFSFreeFakeDirEntList( pDFS0th->pNext ); }
	if (pDFS0th != NULL) { DMfree( pDFS0th, "DFSChainFakeDirEnt"  ); }
	if (pDFSdir != NULL) { DMfree( pDFSdir, "DFSChainFakeDirEnt"  ); }
	if (pDFSdirent != NULL) { DMfree( pDFSdirent, "DFSChainFakeDirEnt"  ); }
	return iErr;
}


/*********************************************************** EnglishComment **/
/* ID = NWChainFakeDirEntFromDFSOne                                          */
/* name = Making an NWFakeDirEnt list with using a DFSFakeDirEnt list        */
/* func = Makes an NWFakeDirEnt list with using a DFSFakeDirEnt list.        */
/*        Uses file names in the DFSFakeDirEnt list, or converted ones.      */
/*        (uses the same information for the remainings)                     */
/*                                                                           */
/* i/o  = pszDFSDirname  :char *:inp: parent path of DFSFakeDirEnt list      */
/*                                    (used for restoring a full path)       */
/*        pDFSHead:struct DFSFakeDirEnt *:inp: pointer to the 1st entry      */
/*        ppNWHead:struct NWFakeDirEnt **:out: pointer to the allocated list */
/*                                                                           */
/* return =  0: normal end                                                   */
/*           EDMwarning: memory shortage                                     */
/* common = none                                                             */
/*                                                                           */
/* note = (1) This function only makes an NWFakeDirEnt list that carries the */
/*            converted file names of DFSFakeDirEnt.                         */
/*        (2) This function then compares the file names in the list with    */
/*            the actual files in the DFAM Volume.                           */
/*        (3) If the name conversion was failed, DFSFakeDirEnt is deleted.   */
/* date = December 16, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int  NWChainFakeDirEntFromDFSOne(	struct GVolume_t		*pVolTbl,
									char 					*pszDFSDirname,
									struct DFSFakeDirEnt	*pDFSHead,
									struct NWFakeDirEnt		**ppNWHead )
{
	int						iErr = 0;
	int						iRet;
	struct NWFakeDirEnt		*pNWCur;
	struct NWFakeDirEnt		*pNWPrev;
	struct DFSFakeDirEnt	*pDFSCur;


	char 					*pszDFSFullPath = NULL;
	char					*pszNWFullPath = NULL;
	struct NWFakeDirEnt		NW0th;

	pszDFSFullPath = (char *)GW_malloc( DFS_MAX_PATH + 1);
	pszNWFullPath = (char *)GW_malloc( NW_MAX_PATH + 1);
	if ( (pszDFSFullPath == NULL) || (pszNWFullPath == NULL) ) {
		iErr = EDMmalloc;
		goto labelNWChainFakeDirEntFromDFSOne_q;
	}

	*ppNWHead = NULL;
	NW0th.pNext = NULL;

	pDFSCur = pDFSHead;
	pNWPrev = &NW0th;
	while ( pDFSCur != NULL ) {
		sprintf( pszDFSFullPath, "%s/%s", pszDFSDirname, pDFSCur->szName );
		if ( iRet = GetNWPath( pszDFSFullPath, pVolTbl->aszNWVol, pszNWFullPath,
				pDFSHead ) ) {

			pDFSCur = pDFSCur->pNext;
			continue;
		}

		pNWCur = (struct NWFakeDirEnt *)GW_malloc(sizeof(struct NWFakeDirEnt));
		if ( pNWCur == NULL ) {
			iErr = EDMwarning;
			pDFSCur = pDFSCur->pNext;
			continue;
		}
		pNWCur->iLink = -1;
		pNWPrev->pNext = pNWCur;
		pNWCur->pNext = NULL;
		pNWCur->pParent = NULL;
		if ( pNWPrev == &NW0th ) {
			pNWCur->pPrev = NULL;
		} else {
			pNWCur->pPrev = pNWPrev;
		}
		if ( S_ISDIR(pDFSCur->usMode) ) {
			pNWCur->iAttr = _A_SUBDIR;
		} else {
			pNWCur->iAttr = _A_NORMAL;
		}
		pNWCur->uiTime = pDFSCur->uiTime;
		pNWCur->iSize = pDFSCur->iSize;
		pNWCur->iOrder = pDFSCur->iOrder;
		NWBasename( pNWCur->szName, pszNWFullPath );

		pNWPrev = pNWCur;
		pDFSCur = pDFSCur->pNext;
	}

	*ppNWHead = NW0th.pNext;

labelNWChainFakeDirEntFromDFSOne_q:
	if ( pszDFSFullPath != NULL )
		GW_free( pszDFSFullPath, 0 );
	if ( pszNWFullPath != NULL )
		GW_free( pszNWFullPath, 0 );
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = NWChainFakeDirEnt                                                    */
/*                                                                           */
/* name = Expansion of directories                                           */
/* func = Sets all the information of the files and directories under the    */
/*        designated NetWare directory to the structure of NWFakeDirEnt, and */
/*        connects the structures.                                           */
/*                                                                           */
/* i/o  = pszNWFullPath:char *  :inp: the top directory to start searching   */
/*        ppNWHead:struct NWFakeDirEnt ** :out: pointer to the allocated list*/
/*        piLink  :int *  :inp: the number of files under the top directory  */
/*                              (option)                                     */
/*                                                                           */
/* return =  0: normal end                                                   */
/*           EDMopendir                                                      */
/*           EDMwarning: memory shortage                                     */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 2, 1994 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int NWChainFakeDirEnt(		char					*pszNWFullPath,
							struct NWFakeDirEnt 	**ppNWHead,
							int						iLevel,
							int						*piLink)
{
	struct	NWFakeDirEnt	*pPrev;
	struct	NWFakeDirEnt	*pCur;
	struct	NWFakeDirEnt	NW0th;
	DIR					*pDIRopen;
	DIR					*pDIRread;
	int					iOrder = 0;
	char				*pszNWTmpPath = NULL;
	int					iErr = 0;
	char				*pDe;

	pszNWTmpPath = (char *)GW_malloc(NW_MAX_PATH + 1 + 4);
	if ( pszNWTmpPath == NULL ) {
		iErr = EDMmalloc;
		goto labelNWChainFakeDirEnt_q;
	}

	if ( piLink != NULL ) {
		*piLink = 0;
	}

	*ppNWHead = NULL;
	NW0th.pNext = NULL;

	pDe = D_strrchr( pszNWFullPath, '\\' );
	if ( (pDe != NULL) && (pDe == (pszNWFullPath+strlen(pszNWFullPath)-1) ) ) {
		sprintf( pszNWTmpPath, "%s*.*", pszNWFullPath );
	} else {
		sprintf( pszNWTmpPath, "%s\\*.*", pszNWFullPath );
	}
	if ( (pDIRopen = opendir( pszNWTmpPath )) == NULL ) {
		if ( errno == ENOENT ) {
			iErr = 0;
			goto labelNWChainFakeDirEnt_q;
		} else {
			iErr = EDMopendir;
			goto labelNWChainFakeDirEnt_q;
		}
	}

	pPrev = &NW0th;
	while( (pDIRread = readdir( pDIRopen )) != NULL ) {
		pCur = (struct NWFakeDirEnt *)GW_malloc( sizeof(struct NWFakeDirEnt) );
		if ( pCur == NULL ) {
			iErr = EDMwarning;
			continue;
		}
		pCur->iLink = -1;
		pCur->byUndeleteable = FALSE;
		pCur->iLevel = iLevel;
		pCur->iOrder = iOrder;
		pCur->iAttr = pDIRread->d_attr;
		if ( pPrev == &NW0th ) {
			pCur->pPrev = NULL;
		} else {
			pCur->pPrev = pPrev;
		}
		pCur->pNext = NULL;
		pCur->pParent = NULL;
		strcpy( pCur->szName, pDIRread->d_name );
		pPrev->pNext = pCur;
		pPrev = pCur;
		iOrder++;
	}
	closedir( pDIRopen );

	*ppNWHead = NW0th.pNext;
	if ( piLink != NULL ) {
		*piLink = iOrder;
	}

labelNWChainFakeDirEnt_q:
	if ( pszNWTmpPath != NULL )
		GW_free( pszNWTmpPath, 0);
	return iErr;
}


/*********************************************************** EnglishComment **/
/* ID = DFSCatFakeDirEnt                                                     */
/*                                                                           */
/* name = Chaining the file names in the directory list to get a path name   */
/* func = Searches the DFSFakeDirEnt list backward (i.e., going towards the  */
/*        parent directory), and chains the file names.                      */
/*                                                                           */
/* i/o  = pszDFSPath  :char *          :out: pointer to the path name area   */
/*        pTail :struct DFSFakeDirEnt *:inp: pointer to DFSFakeDirEnt that   */
/*                                           has a list of files whose path  */
/*                                           names are searched.             */
/* return = 0:           normal end                                          */
/*          EDMlongpath: generated path is too long                          */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 5, 1994 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DFSCatFakeDirEnt( char *pszDFSPath, struct DFSFakeDirEnt *pTail )
{
	int					iErr = 0;
	struct DFSFakeDirEnt	*pCur;
	char				*pszTail = NULL;
	char				*pszPath = NULL;

	if ( pTail->pParent == NULL ) {
		strcpy( pszDFSPath, pTail->szName );
		return 0;
	}

	*pszDFSPath = '\0';

	pszTail = GW_malloc( DFS_MAX_PATH + 1 );
	pszPath = GW_malloc( DFS_MAX_PATH + 1 );
	if ( (pszTail==NULL) || (pszPath==NULL) ) {
		iErr = EDMmalloc;
		goto labelDFSCatFakeDirEnt_q;
	}

	strcpy( pszTail, pTail->szName );
	pCur = pTail->pParent;
	while ( pCur != NULL ){
		if ( (strlen(pCur->szName) + 1 + strlen(pszTail)) > DFS_MAX_PATH ) {
			iErr = EDMlongpath;
			goto labelDFSCatFakeDirEnt_q;
		}
		sprintf( pszPath, "%s/%s", pCur->szName, pszTail );
		strcpy( pszTail, pszPath );
		pCur = pCur->pParent;
	}

	strcpy( pszDFSPath, pszTail );

labelDFSCatFakeDirEnt_q:
	if ( pszTail != NULL )
		GW_free( pszTail, 0 );
	if ( pszPath != NULL )
		GW_free( pszPath, 0 );
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = NWCatFakeDirEnt                                                      */
/*                                                                           */
/* name = Chaining the file names in the directory list to get a path name   */
/* func = Searches the NWFakeDirEnt list backward (i.e., going towards the   */
/*        parent directory), and chains the file names.                      */
/*                                                                           */
/* i/o  = pszNWPath  :char *          :out: pointer to the path name area    */
/*        pTail :struct NWFakeDirEnt *:inp: pointer to NWFakeDirEnt that     */
/*                                          has a list of files whose path   */
/*                                          names are searched.              */
/* return = 0:           normal end                                          */
/*          EDMlongpath: generated path is too long                          */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 5, 1994 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int NWCatFakeDirEnt( char *pszNWPath, struct NWFakeDirEnt *pTail )
{
	int					iErr = 0;
	struct NWFakeDirEnt	*pCur;
	char				*pszTail = NULL;
	char				*pszPath = NULL;

	if ( pTail->pParent == NULL ) {
		strcpy( pszNWPath, pTail->szName );
		return 0;
	}

	pszTail = GW_malloc( NW_MAX_PATH + 1 );
	pszPath = GW_malloc( NW_MAX_PATH + 1 );
	if ( (pszTail==NULL) || (pszPath==NULL) ) {
		iErr = EDMmalloc;
		goto labelNWCatFakeDirEnt_q;
	}

	*pszNWPath = '\0';
	strcpy( pszTail, pTail->szName );
	pCur = pTail->pParent;
	while ( pCur != NULL ){
		if ( (strlen(pCur->szName) + 1 + strlen(pszTail)) > NW_MAX_PATH ) {
			iErr = EDMlongpath;
			goto labelNWCatFakeDirEnt_q;
		}
		sprintf( pszPath, "%s\\%s", pCur->szName, pszTail );
		strcpy( pszTail, pszPath );
		pCur = pCur->pParent;
	}

	if ( pszTail[0] == '\\' ) {
		strcpy( pszNWPath, pszTail+1 );
	} else {
		strcpy( pszNWPath, pszTail );
	}

labelNWCatFakeDirEnt_q:
	if ( pszTail != NULL )
		GW_free( pszTail, 0 );
	if ( pszPath != NULL )
		GW_free( pszPath, 0 );
	return iErr;
}

/*********************************************************** EnglishComment **/
/* ID = DFSInsertFakeDirEnt                                                  */
/*                                                                           */
/* name = Adding an entry to the middle of the directory list                */
/* func = Adds pDFSChildDirEnt after the entry pointed by pDFSCurDirEnt.     */
/*        The pPrev pointer must be set by the caller.                       */
/*                                                                           */
/* i/o  = pDFSCurDirEnt:struct DFSFakeDirEnt *:inp: the new entry is added   */
/*                                                  after this entry         */
/*        pDFSChildDirEnt:struct DFSFakeDirEnt *:inp: an added entry         */
/*                                                                           */
/* return = 0: normal end                                                    */
/*          EDMmalloc:                                                       */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = February 20, 1995 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DFSInsertDirEnt(	struct DFSFakeDirEnt *pDFSCurDirDirEnt,
						struct DFSFakeDirEnt *pDFSCurFileDirEnt )
{
	struct DFSFakeDirEnt	*pNew;

	pNew = (struct DFSFakeDirEnt *)GW_malloc( sizeof( struct DFSFakeDirEnt) );
	if ( pNew == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
		return EDMmalloc;
	}


	memcpy( pNew, pDFSCurFileDirEnt, sizeof(struct DFSFakeDirEnt ));

	if ( pDFSCurDirDirEnt->pNext != NULL ) {
		(pDFSCurDirDirEnt->pNext)->pPrev = pNew;
	}
	pNew->pNext = pDFSCurDirDirEnt->pNext;
	pNew->pPrev = pDFSCurDirDirEnt;
	pDFSCurDirDirEnt->pNext = pNew;

	return 0;
}

/*********************************************************** EnglishComment **/
/* ID = NWInsertFakeDirEnt                                                   */
/*                                                                           */
/* name = Adding an entry to the middle of the directory list                */
/* func = Adds pNWChildDirEnt after the entry pointed by pNWCurDirEnt.       */
/*        The pPrev pointer must be set by the caller.                       */
/*                                                                           */
/* i/o  = pNWCurDirEnt:struct NWFakeDirEnt *:inp: the new entry is added     */
/*                                                after this entry           */
/*        pNWChildDirEnt:struct NWFakeDirEnt *:inp: an added entry           */
/*                                                                           */
/* return = 0: normal end                                                    */
/*          EDMmalloc:                                                       */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = February 20, 1995 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int NWInsertDirEnt(	struct NWFakeDirEnt *pNWCurDirDirEnt,
					struct NWFakeDirEnt *pNWCurFileDirEnt )
{
	struct NWFakeDirEnt	*pNew;

	pNew = (struct NWFakeDirEnt *)GW_malloc( sizeof( struct NWFakeDirEnt) );
	if ( pNew == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11613 );
		return EDMmalloc;
	}


	memcpy( pNew, pNWCurFileDirEnt, sizeof(struct NWFakeDirEnt ));

	if ( pNWCurDirDirEnt->pNext != NULL ) {
		(pNWCurDirDirEnt->pNext)->pPrev = pNew;
	}
	pNew->pNext = pNWCurDirDirEnt->pNext;
	pNew->pPrev = pNWCurDirDirEnt;
	pNWCurDirDirEnt->pNext = pNew;

	return 0;
}


/*********************************************************** EnglishComment **/
/* ID = DFSMakeFakeDirEntUndeleteable                                        */
/*                                                                           */
/* name = Setting a "delete-prohibition" stamp to the directory list.        */
/* func = Searches the DFSFakeDirEnt list backward (i.e., going toward the   */
/*        parent directory), and sets a "delete-prohibition" stamp           */
/*        hierarchically.  This function is invoked when a file cannot be    */
/*        (is not) deleted.                                                  */
/*                                                                           */
/* i/o  = pDir: struct DFSFakeDirEnt *:inp: fakeDirEnt of the parent         */
/*                                          directory whose child directory  */
/*                                          could not be deleted             */
/* return = 0: normal end                                                    */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DFSMakeFakeDirEntUndeleteable( struct DFSFakeDirEnt *pDir )
{
	struct DFSFakeDirEnt	*pCur;

	pCur = pDir;
	while ( pCur != NULL ){
		pCur->byUndeleteable |= 1;
		pCur = pCur->pParent;
	}

	return 0;
}

/*********************************************************** EnglishComment **/
/* ID = NWMakeFakeDirEntUndeleteable                                         */
/*                                                                           */
/* name = Setting a "delete-prohibition" stamp to the directory list.        */
/* func = Searches the NWFakeDirEnt list backward (i.e., going toward the    */
/*        parent directory), and sets a "delete-prohibition" stamp           */
/*        hierarchically.  This function is invoked when a file cannot be    */
/*        (is not) deleted.                                                  */
/*                                                                           */
/* i/o  = pDir: struct NWFakeDirEnt *:inp: fakeDirEnt of the parent          */
/*                                         directory whose child directory   */
/*                                         could not be deleted              */
/* return = 0: normal end                                                    */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = November 30, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int NWMakeFakeDirEntUndeleteable( struct NWFakeDirEnt *pDir )
{
	struct NWFakeDirEnt	*pCur;

	pCur = pDir;
	while ( pCur != NULL ){
		pCur->byUndeleteable |= 1;
		pCur = pCur->pParent;
	}

	return 0;
}

/*********************************************************** EnglishComment **/
/* ID = DFSDeleteFakeDirEnt                                                  */
/*                                                                           */
/* name = Deleting an entry from a directory list                            */
/* func = Deletes an entry from the fakeDirEnt list, and reconnects the list.*/
/* i/o  = pCur :struct DFSFakeDirEnt *:inp: pointer to the to-be-deleted     */
/*                                          fakeDirEnt                       */
/*        ppNew:struct DFSFakeDirEnt *:inp: pointer next to the deleted entry*/
/*                                          (it is either before or after    */
/*                                           the pointer)                    */
/* return = 0: normal end                                                    */
/*          EDMnullptr                                                       */
/* common = none                                                             */
/* :::                                                                       */
/* note = (1) If ppNew is not NULL, the pointer is located to the entry next */
/*            to the deleted entry.  If the next entry is NULL, a pointer    */
/*            pointing the previous entry is set.                            */
/*        (2) NULL is set if all the lists are deleted.                      */
/*        (3) pCur and *ppNew can be same.                                   */
/*                                                                           */
/* date = December 5, 1994 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DFSDeleteFakeDirEnt(	struct DFSFakeDirEnt *pCur,
							struct DFSFakeDirEnt **ppNew )
{
	struct DFSFakeDirEnt	*pTmp;

	if ( pCur == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11624, EDMnullptr);
		if ( ppNew != NULL ) {
			*ppNew = NULL;
		}
		return EDMnullptr;
	}

	if ( pCur->pNext != NULL ) {
		(pCur->pNext)->pPrev = pCur->pPrev;
		pTmp = pCur->pNext;
	} else {
		pTmp = pCur->pPrev;
	}

	if ( pCur->pPrev != NULL ) {
		(pCur->pPrev)->pNext = pCur->pNext;
		if ( pCur->pParent != NULL ) {
			(pCur->pParent)->iLink--;
		}
	}

	DMfree( pCur, "DFSDeleteFakeDirEnt" );

	if ( ppNew != NULL ) {
		*ppNew = pTmp;
	}

	ThreadSwitch();

	return 0;
}

/*********************************************************** EnglishComment **/
/* ID = NWDeleteFakeDirEnt                                                   */
/*                                                                           */
/* name = Deleting an entry from a directory list                            */
/* func = Deletes an entry from the fakeDirEnt list, and reconnects the list.*/
/* i/o  = pCur :struct NWFakeDirEnt *:inp: pointer to the to-be-deleted      */
/*                                          fakeDirEnt                       */
/*        ppNew:struct NWFakeDirEnt *:inp: pointer next to the deleted entry */
/*                                          (it is either before or after    */
/*                                           the pointer)                    */
/* return = 0: normal end                                                    */
/*          EDMnullptr                                                       */
/* common = none                                                             */
/* :::                                                                       */
/* note = (1) If ppNew is not NULL, the pointer is located to the entry next */
/*            to the deleted entry.  If the next entry is NULL, a pointer    */
/*            pointing the previous entry is set.                            */
/*        (2) NULL is set if all the lists are deleted.                      */
/*                                                                           */
/* date = December 5, 1994 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int NWDeleteFakeDirEnt(	struct NWFakeDirEnt *pCur,
						struct NWFakeDirEnt **ppNew )
{
	struct NWFakeDirEnt	*pTmp;

	if ( pCur == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11624, EDMnullptr);
		if ( ppNew != NULL ) {
			*ppNew = NULL;
		}
		return EDMnullptr;
	}

	if ( pCur->pNext != NULL ) {
		(pCur->pNext)->pPrev = pCur->pPrev;
		pTmp = pCur->pNext;
	} else {
		pTmp = pCur->pPrev;
	}

	if ( pCur->pPrev != NULL ) {
		(pCur->pPrev)->pNext = pCur->pNext;
	}

	DMfree( pCur, "NWDeleteFakeDirEnt" );

	if ( ppNew != NULL ) {
		*ppNew = pTmp;
	}

	ThreadSwitch();

	return 0;
}

/*********************************************************** EnglishComment **/
/* ID = NWMoveToNextFakeDirEnt                                               */
/*                                                                           */
/* name = Moving the pointer backward                                        */
/* func = Moves the pointer (pointing the entry in the NWFakeDirEnt list)    */
/*        backward.  The pointer does not move if it points the last entry.  */
/*                                                                           */
/* i/o  = pCur :struct DFSFakeDirEnt * :inp: pointer next to this entry      */
/*        ppNew:struct DFSFakeDirEnt * :inp: next to pCur, or pCur itself    */
/*                                                                           */
/* return =  0: normal end                                                   */
/*           EDMnullptr                                                      */
/* common = none                                                             */
/* :::                                                                       */
/* note = (1) Returns pCur if the pCur is the last of the list.              */
/*        (2) If there is a next entry, sets pCur to the entry.              */
/* date = December 5, 1994 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int NWMoveToNextFakeDirEnt(	struct NWFakeDirEnt *pCur,
							struct NWFakeDirEnt **ppNew )
{
	if ( pCur == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11624, EDMnullptr);
		*ppNew = NULL;
		return EDMnullptr;
	}

	if ( pCur->pNext != NULL ) {
		*ppNew = pCur->pNext;
	} else {
		*ppNew = pCur;
	}

	return 0;
}

/*********************************************************** EnglishComment **/
/* ID = DFSMoveToNextFakeDirEnt                                              */
/*                                                                           */
/* name = Moving the pointer backward                                        */
/* func = Moves the pointer (pointing the entry in the DFSFakeDirEnt list)   */
/*        backward.  The pointer does not move if it points the last entry.  */
/*                                                                           */
/* i/o  = pCur :struct NWFakeDirEnt * :inp: pointer next to this entry       */
/*        ppNew:struct NWFakeDirEnt * :inp: next to pCur, or pCur itself     */
/*                                                                           */
/* return =  0: normal end                                                   */
/*           EDMnullptr                                                      */
/* common = none                                                             */
/* :::                                                                       */
/* note = (1) Returns pCur if the pCur is the last of the list.              */
/*        (2) If there is a next entry, sets pCur to the entry.              */
/* date = December 5, 1994 by Ryuji Shinchi                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
int DFSMoveToNextFakeDirEnt(	struct DFSFakeDirEnt *pCur,
								struct DFSFakeDirEnt **ppNew )
{
	if ( pCur == NULL ) {
		GW_Message( DFAM_CON, 0, LOG_YES, KDDS11624,EDMnullptr);
		*ppNew = NULL;
		return EDMnullptr;
	}

	if ( pCur->pNext != NULL ) {
		*ppNew = pCur->pNext;
	} else {
		*ppNew = pCur;
	}

	return 0;
}
