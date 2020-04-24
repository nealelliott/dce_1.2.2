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
 * $Log: gdirlist.c,v $
 * Revision 1.1.2.3  1996/03/22  07:02:40  takano
 * 	Fixed the confliction between the Directory Synchronization and the File-Access.
 * 	[1996/03/22  06:59:35  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:07:51  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:14  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  07:48:11  takano
 * 	First Submit
 * 	[1996/02/21  07:48:06  takano]
 * 
 * $EndLog$
 */
#include "gdir.h"

/*********************************************************** EnglishComment **/
/* ID = NWFreeFakeDirEntList                                                 */
/*                                                                           */
/* name = Releasing NWFakeDirEnt structure                                   */
/* func = Frees all the entries in the NWFakeDirEnt structure.               */
/*                                                                           */
/* i/o  = pHook:struct NWFakeDirEnt *:inp: pointer to a list entry (any      */
/*                                         entry)                            */
/* return =       void:                                                      */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 16, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
void NWFreeFakeDirEntList( struct NWFakeDirEnt *pHook )
{
	struct NWFakeDirEnt *pCur;

    GW_tsh_functrace("NWFreeFakeDirEntList", GW_FUNC_START, 0);

	pCur = pHook;
	while ( pCur != NULL ) {
		NWDeleteFakeDirEnt( pCur, &pCur );
	}
	GW_tsh_functrace("NWFreeFakeDirEntList", GW_FUNC_END, 0);

}

/*********************************************************** EnglishComment **/
/* ID = DFSFreeFakeDirEntList                                                */
/*                                                                           */
/* name = Releasing DFSFakeDirEnt structure                                  */
/* func = Frees all the entries in the DFSFakeDirEnt structure.              */
/*                                                                           */
/* i/o  = pHook:struct DFSFakeDirEnt *:inp: pointer to a list entry (any     */
/*                                          entry)                           */
/* return =       void:                                                      */
/* common = none                                                             */
/* :::                                                                       */
/* note = none                                                               */
/* date = December 16, 1994 by Ryuji Shinchi                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*********************************************************** EnglishComment **/
void DFSFreeFakeDirEntList( struct DFSFakeDirEnt *pHook )
{
	struct DFSFakeDirEnt *pCur;

    GW_tsh_functrace("DFSFreeFakeDirEntList", GW_FUNC_START, 0);

	pCur = pHook;
	while ( pCur != NULL ) {
		DFSDeleteFakeDirEnt( pCur, &pCur );
	}
	GW_tsh_functrace("DFSFreeFakeDirEntList", GW_FUNC_END, 0);

}

/************************************************************ EnglishComment */
/* ID = DMAddAccessFileList                                                  */
/*                                                                           */
/* name = Adding an entry to the hook routine access history                 */
/* func = Adds an entry (corresponding to the path given by a parameter) to  */
/*        the access history.  If the same named entry already exists,       */
/*        increments the counter.                                            */
/*                                                                           */
/* i/o  = pszPath :char *:inp: the path name to be registered with the       */
/*                             access history                                */
/*                                                                           */
/* return = pointer to the registered entry: successfully registered         */
/*          NULL: failed to register                                         */
/*                                                                           */
/* date = August 2, 1995  by Ryuji Shinchi                                   */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/************************************************************ EnglishComment */
struct DMAccessFile *DMAddAccessFileList( char *pszPath, int iThID )
{
	struct DMAccessFile		*pNew;


	pNew = (struct DMAccessFile *)GW_malloc( sizeof (struct DMAccessFile) );
	if ( pNew == NULL )
		return NULL;
	pNew->pszPath = (char *)GW_malloc( strlen( pszPath ) + 1 );
	if ( pNew->pszPath == NULL ) {
		GW_free( pNew, 0 );
		return NULL;
	}


	memcpy( pNew->aszEC, "AccFile=", 8 );
	strcpy( pNew->pszPath, pszPath );
	pNew->iCount = 1;
	pNew->iThID = iThID;

	pNew->pNext = AccessListRoot.pAccessFile;
	AccessListRoot.pAccessFile = pNew;

	return pNew;
}


/************************************************************ EnglishComment */
/* ID = DMDeleteAccessFileList                                               */
/*                                                                           */
/* name = Deleting an entry from the hook routine access history             */
/* func = Deletes an entry (corresponding to a path given by a parameter)    */
/*        from the access history.                                           */
/*                                                                           */
/* i/o  = pszPath :char *:inp: the path name to be deleted from the access   */
/*                             history                                       */
/*                                                                           */
/* return = 0: successfully deleted                                          */
/*          1: the target entry does nor exist in the history                */
/*                                                                           */
/* date = August 2, 1995  by Ryuji Shinchi                                   */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/************************************************************ EnglishComment */
int DMDeleteAccessFileList( int iThID )
{
	struct DMAccessFile		**ppCurAccessFile;
	struct DMAccessFile		*pTmp;
	BOOL					bMached = FALSE;

	ppCurAccessFile = &(AccessListRoot.pAccessFile);
	while ( *ppCurAccessFile ) {
		if ( iThID == (*ppCurAccessFile)->iThID ) {

			bMached = TRUE;

			(*ppCurAccessFile)->iCount--;

			if ( (AccessListRoot.iAccessListMode != DMAccessListModeSync)
					&& ( (*ppCurAccessFile)->iCount == 0) ) {
				pTmp = *ppCurAccessFile;
				*ppCurAccessFile = (*ppCurAccessFile)->pNext;
				GW_free( pTmp->pszPath, 0 );
				GW_free( pTmp, 0 );
			} else {
				ppCurAccessFile = &((*ppCurAccessFile)->pNext);
			}
		} else {
			ppCurAccessFile = &((*ppCurAccessFile)->pNext);
		}

		ThreadSwitch();

	}

	if ( bMached == FALSE ) {

		return 1;
	}

	return 0;
}


/************************************************************ EnglishComment */
/* ID = DMFreeAccessFileList                                                 */
/*                                                                           */
/* name = Releasing the hook routine access history                          */
/* func = Frees an entry whose corresponding file has completed its access   */
/*        from the access history.                                           */
/*                                                                           */
/* i/o  = none                                                               */
/*                                                                           */
/* return = 0: successfully released                                         */
/*          1: failed to release                                             */
/*                                                                           */
/* date = August 2, 1995  by Ryuji Shinchi                                   */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/************************************************************ EnglishComment */
int DMFreeAccessFileList( void )
{
	struct DMAccessFile		**ppCurAccessFile;
	struct DMAccessFile		*pTmp;

	ppCurAccessFile = &(AccessListRoot.pAccessFile);
	while ( *ppCurAccessFile ) {
		if ( (AccessListRoot.iAccessListMode == DMAccessListModeTerm)
				|| ((*ppCurAccessFile)->iCount == 0) ) {

			pTmp = *ppCurAccessFile;
			*ppCurAccessFile = (*ppCurAccessFile)->pNext;
			GW_free( pTmp->pszPath, 0 );
			GW_free( pTmp, 0 );
		} else {
			ppCurAccessFile = &((*ppCurAccessFile)->pNext);
		}
		ThreadSwitch();
	}

	return 0;
}

/************************************************************ EnglishComment */
/* ID = DMCheckAccessFileList                                                */
/*                                                                           */
/* name = Searching the access history for the path name                     */
/* func = Searches the hook routine access history, and checks if there is   */
/*        a path designated by a parameter.                                  */
/*                                                                           */
/* i/o  = pszPath :char *:inp: the target path name to be searched for       */
/*                                                                           */
/* return = 0: the path does not exist in the history                        */
/*          1: the path exists in the history                                */
/*                                                                           */
/* date = August 2, 1995  by Ryuji Shinchi                                   */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/************************************************************ EnglishComment */
int DMCheckAccessFileList( char *pszPath )
{
	int		iErr = 0;
	struct DMAccessFile	*pCurAccessFile;

	pCurAccessFile = AccessListRoot.pAccessFile;

	while ( pCurAccessFile != NULL ) {
		if ( !strcmp(pszPath, pCurAccessFile->pszPath) ) {
			iErr = 1;
			break;
		}
		pCurAccessFile = pCurAccessFile->pNext;
		ThreadSwitch();
	}

	return iErr;
}

