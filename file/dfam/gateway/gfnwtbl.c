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
 * $Log: gfnwtbl.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:25  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:42  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:02:43  takano
 * 	First Submit
 * 	[1996/02/21  08:02:39  takano]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <nwsemaph.h>
#include <process.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "gfnw.h"


/*EnglishComment**************************************************************
 *  ID   = AddToName_t                                                       *
 *  name = Adding a member to GName_t                                        *
 *  func = Makes a new member on GName_t, and connects the member with       *
 *         pVolume_sp (list connection).                                     *
 *  i/o  = pszpath:BYTE *:input: the name to be added                        *
 *  return = 0: the volume is not DFAM Volume.                               *
 *           1: the volume is DFAM Volume.                                   *
 *  common = :::                                                             *
 *  note   = This function compares pszVolName with the member in the        *
 *           Gateway Volume, and returns the result.                         *
 *  date   = Dec. 13, 1994 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
struct GName_t *	AddToName_t( pszPath )
BYTE *pszPath;
{
	int		iCcode;
	int		iRtnc = GWERR_NORMAL;
	struct	GVolume_t	*pVolTblp;
	struct	GName_t		*pCur_Namep;
	struct	GName_t		*pNew_Namep;
	struct	GName_t		*pRtn_Namep;
	struct	GName_t		*pLastAllocp;
	BYTE	*pWPathp = pszPath;
	BYTE	aszWvol[VOLUMELEN_MAX + 1];
	BYTE	aszEntryName[_MAX_NAME];

	aszWvol[0] = '\0';




	iCcode = GetDFAMVolumeName(pszPath, aszWvol);
	if (iCcode != 0 ) {
		return(NULL);
	};


	isDFAMVolume(aszWvol, &pVolTblp);
	if (pVolTblp == NULL){
		return(NULL);
	};

	if ( (pVolTblp->pNamep) == NULL ) {
		pNew_Namep = AllocName_t();
		if ( pNew_Namep == NULL ) {
			iRtnc = GWERR_NOMEM;
			goto EndOfFunc;
		};
		pNew_Namep->pVolp = pVolTblp;
		pVolTblp->pNamep = pNew_Namep;
	};


	pCur_Namep = pVolTblp->pNamep;


	while(*pWPathp != ':') {
		pWPathp++;
	}
	pWPathp++;



	while( (pWPathp != NULL) && (*pWPathp != '\0') ) {

		ThreadSwitch();


		aszEntryName[0] = '\0';


		iCcode = GetEntry(pWPathp, aszEntryName, &pWPathp);
		if (iCcode != GWERR_NORMAL) {
			iRtnc = GWERR_BUG;
			goto EndOfFunc;
		};


		pRtn_Namep = NULL;
		iCcode = SearchName_tByName( aszEntryName, pCur_Namep, MODE_NW, &pRtn_Namep);


		if ( iCcode != FNWERR_NORMAL ) {

			pNew_Namep = AllocName_t();
			if ( pNew_Namep == NULL ) {
				iRtnc = GWERR_NOMEM;
				goto EndOfFunc;
			};

			pLastAllocp = pNew_Namep;


			pNew_Namep->bExist			=	TRUE;
			strcpy(pNew_Namep->aszEntryName, aszEntryName);

			strcpy(pNew_Namep->aszRealName, D_strlwr(aszEntryName));
		};

		switch(iCcode) {

		case FNWERR_NO_SUBDIR:
			pCur_Namep->pKoDirp = pNew_Namep;
			pNew_Namep->pOyaDirp = pCur_Namep;
			pCur_Namep = pNew_Namep;
		break;

		case FNWERR_NO_ENTRY:
			pRtn_Namep->pUshiroDirp =	pNew_Namep;
			pNew_Namep->pMaeDirp = pRtn_Namep;
			pNew_Namep->pOyaDirp = pCur_Namep;
			pCur_Namep = pNew_Namep;
		break;

		default:
			pCur_Namep = pRtn_Namep;
		}
	}

EndOfFunc:


	if(iRtnc != GWERR_NORMAL) {
		if ( pLastAllocp != NULL ) {
 			DeleteName_t(pLastAllocp, 1);
		};
		return(NULL);
	};

	return( pCur_Namep );
}




/*EnglishComment**************************************************************
 *  ID   = DelelteName_tByName                                               *
 *  name = Deleting a member from the Name Conversion Table                  *
 *  func = Deletes a Name-Conversion-Table member corresponding with the     *
 *         DFAM full path name.                                              *
 *  i/o  = pszPath:char *:input: the DFAM Volume name of the name to be      *
 *                               deleted                                     *
 *  return = none                                                            *
 *  common = none                                                            *
 *  note   = (1) This function is invoked in the special case that the       *
 *               actual DFAM Volume does not exist, but the name conversion  *
 *               table has an entry for the volume (i.e., a directory        *
 *               synchronization error).                                     *
 *           (2) The caller of this routine does not have to see if pszPath  *
 *               is a converted name: if found to be a non-converted name,   *
 *               the process is simply terminated, and if found to be a      *
 *               converted name, there is no need to notify the fact.        *
 *           (3) This function does not preserv the deleted entry.           *
 *  date   = Dec. 13, 1994 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
void DeleteName_tByName( pszPath )
BYTE *pszPath;
{
	struct GName_t * pStrp;
	struct GVolume_t*	pVolp;


	GW_tsh_functrace( FUNC_DELNAMETBYNAME, GW_FUNC_START, NULL);



	isDFAMVolume( pszPath, &pVolp );
	if (pVolp == NULL) {

		GW_tsh_functrace( FUNC_DELNAMETBYNAME, GW_FUNC_END, NULL);
		return;
	};

	WaitOnLocalSemaphore( pVolp->uiVolAndName_sh );


	pStrp = FindName_t( pszPath );
	if ( pStrp != NULL ) {
		DeleteName_t(pStrp, 1);
	};


	SignalLocalSemaphore( pVolp->uiVolAndName_sh );


	GW_tsh_functrace( FUNC_DELNAMETBYNAME, GW_FUNC_END, NULL);

	return;
}




/*EnglishComment**************************************************************
 *  ID   = SetDelmarkByName                                                  *
 *  name = Name conversion table marking routine                             *
 *  func = Sets a "deletion mark" to the member in the name conversion table *
 *         corresponding to the designated DFAM full path name.  Since the   *
 *         member is not actually deleted, the member comes back when        *
 *         GetDFSPath or GetNWPath designate the entry corresponding to the  *
 *         member.                                                           *
 *                                                                           *
 *  i/o  = pszPath:char *:inp: NetWare full path name of the member to be    *
 *                             deleted.                                      *
 *  return = GWERR_NORMAL (0) : normal                                       *
 *           GWERR_BUG (-100) : Abnormal (internal error)                    *
 *  common = none                                                            *
 *  note   = none                                                            *
 *  date   = Apr. 18, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int SetDelmarkByName( pszPath )
BYTE *pszPath;
{
	int	iRtnc = GWERR_NORMAL;
	struct GName_t * pStrp;
	struct GVolume_t *pVolp;


	GW_tsh_functrace( FUNC_SETDELMARKBYNAME, GW_FUNC_START, NULL);


	if (pszPath == NULL || *pszPath == '\0' ) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};

	isDFAMVolume( pszPath, &pVolp );
	if (pVolp == NULL) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};

	WaitOnLocalSemaphore( pVolp->uiVolAndName_sh );


	pStrp = FindName_t( pszPath );
	if ( pStrp != NULL ) {
		iRtnc = AddToDelList(pStrp);
	};

	SignalLocalSemaphore( pVolp->uiVolAndName_sh );

EndOfFunc:

	GW_tsh_functrace( FUNC_SETDELMARKBYNAME, GW_FUNC_END, iRtnc);
	return(iRtnc);
}




/*EnglishComment**************************************************************
 *  ID   = DeleteName_t                                                      *
 *  name = Name conversion table member deletion routine                     *
 *  func = Deletes a member in the designated name conversion table.  If the *
 *         deletion of a target member results that an upper member does not *
 *         have any lower members, or the target member is not a converted   *
 *         name, the upper member is deleted, too.  If all the members of a  *
 *         name conversion table were deleted by the ripple effect, a top    *
 *         pointer of the name conversion table is removed from Volume       *
 *         Table.                                                            *
 *  i/o  = pNamep: struct GName_t *: inp: pointer to the name conversion     *
 *                                        table                              *
 *         iMode : int             : inp: mode of how the deletion goes:     *
 *                                        1: recursive bottom-up deletion    *
 *                                        2: single entry deletion           *
 *  return = value: imi                                                      *
 *  common = none                                                            *
 *  note = This routine is called when the file on DFAM volume is deleted.   *
 *         If iMode=1, the deletion is attempted from the bottom to the      *
 *         top (recursively deleted).  If iMode=2, a single entry is         *
 *         deleted (used when DFAM is terminated).                           *
 *  date   = Apr. 18, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int DeleteName_t( pNamep, iMode )
struct GName_t *pNamep;
int	iMode;
{
	int	iCcode;
	struct GName_t *pPapap	=	NULL;
	struct GName_t *pAnikip =	NULL;
	struct GName_t *pOtotop =	NULL;
	BOOL	bKill_Papa = FALSE;

	if ( pNamep == NULL ) {
		GW_tsh_log( (int)pNamep , "DelNametBUG");
		return (GWERR_BUG);
	};

	if ( pNamep->pKoDirp != NULL ) {
		GW_tsh_log( (int)pNamep , "DelNametBUG");
		return (GWERR_BUG);
	};

	pPapap	=	pNamep->pOyaDirp;
	pAnikip	=	pNamep->pMaeDirp;
	pOtotop	=	pNamep->pUshiroDirp;



	if ( pOtotop != NULL ) {
		if ( pAnikip != NULL) {
			pAnikip->pUshiroDirp = pOtotop;
			pOtotop->pMaeDirp = pAnikip;
		} else {
			pOtotop->pMaeDirp = NULL;
			if (pPapap != NULL) {
				pOtotop->pOyaDirp = pPapap;
				pPapap->pKoDirp   = pOtotop;
			} else {
				if (pPapap->bConvflg == FALSE ) {
					bKill_Papa = TRUE;
				};
			}
		}
	} else {
		if ( pAnikip != NULL ) {
			pAnikip->pUshiroDirp = NULL;
		} else {
			if( pPapap != NULL ) {
				pPapap->pKoDirp = NULL;
				if ( pPapap->aszRealName[0] == '\0' ) {
					bKill_Papa = TRUE;
				};
			} else {
				(pNamep->pVolp)->pNamep = NULL;
			}
		}
	}

	iCcode = GW_free( pNamep, NULL );

	if (iMode == 1) {

		if ( bKill_Papa == TRUE ) {
			iCcode = DeleteName_t(pPapap, 1);
			if (iCcode != GWERR_NORMAL) {
				GW_tsh_log( (int)pNamep , "DelNametBUG");
				return(GWERR_BUG);
			};
		};
	};

	return(GWERR_NORMAL);

}




/*EnglishComment**************************************************************
 *  ID   = DeleteAllName_t                                                   *
 *  name = Name conversion table deletion routine                            *
 *  func = Deletes all the lists in the name conversion table, whose address *
 *         is given by the name conversion top pointer.                      *
 *  i/o  = pTopp : Struct GName_t *     :inp: top pointer to the name        *
 *                                            conversion table to be deleted *
 *  return = void                                                            *
 *  common = none                                                            *
 *  note = This routine recursively deletes the name conversion table in a   *
 *         top-down fashion.  This routine is called to delete the name      *
 *         conversion table when Volume Table is terminated.  This routine   *
 *         uses DeleteName_t to delete a single member.                      *
 *  date   = Dec. 21, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
void DeleteAllName_t(pTopp)
struct GName_t *pTopp;
{

	struct GName_t *pNowp = pTopp;
	struct GName_t *pNextp;

	if (pTopp == NULL) {
		goto EndOfFunc;
	};

	while(1) {
		ThreadSwitch();
		pNextp = pNowp->pUshiroDirp;

		if(pNowp->pKoDirp != NULL) {
			DeleteAllName_t(pNowp->pKoDirp);
		};

		DeleteName_t(pNowp,2);

		pNowp = pNextp;

		if (pNowp == NULL) {
			break;
		};

	}
EndOfFunc:
	return;

}




/*EnglishComment**************************************************************
 *  ID   = GetDFAMVolumeName                                                 *
 *  name = Volume name parsing routine                                       *
 *  func = Gets a Volume Name from the NetWare path name.                    *
 *                                                                           *
 *  i/o  = pszPathp:char *:inp: target NetWare path name                     *
 *         pszVolp :char *:out: pointer to the parsed Volume Name            *
 *  return = none                                                            *
 *  common = none                                                            *
 *  note   = A caller of this routine should have the area for pszVolp.      *
 *  date   = Dec. 15, 1994 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/


int GetDFAMVolumeName( pszPathp, pszVolp )
BYTE *pszPathp;
BYTE *pszVolp;
{
	BYTE *pszP=pszPathp;
	BYTE *pszV=pszVolp;

	if ( pszPathp == NULL || *pszPathp == '\0' ) {
		return(GWERR_BUG);
	};

	*pszVolp = '\0';

	while( (*pszP != '\0') && (*pszP != ':') ){
		*pszV++ = *pszP++;
	}
	*pszV = '\0';

	if ( *pszP != ':' ) {
		return(GWERR_ABEND);
	}

	return(GWERR_NORMAL);

}




/*EnglishComment**************************************************************
 *  ID   = isDFAMVolume                                                      *
 *  name = DFAM volume check routine                                         *
 *  func = Checks if the volume name in pszVolName is DFAM volume.           *
 *         And returns a pointer to the target GVolume_tg if necessary.      *
 *  i/o  = pszVolName:BYTE *           :inp: target volume name              *
 *         pTablep   :struct Volume_t**:out: pointer to the volume table     *
 *                                           pointer                         *
 *  return = NO(0) : is not a DFAM volume                                    *
 *           YES(1): is a DFAM volume                                        *
 *  common = none                                                            *
 *  note   = This function only returns the result of the comparison         *
 *           between the Gateway volume table member and pszVolName.         *
 *           If you do not need the pointer to the table, set NULL to        *
 *           pTablep.                                                        *
 *                                                                           *
 *  date   = Dec. 13, 1994 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/

int isDFAMVolume( pszVolName, pTablep )
BYTE *pszVolName;
struct GVolume_t **pTablep;
{
	int iRtnc = NO;
	struct GVolume_t *pVolp;
	char *pszWkp;
	char aszWvol[_MAX_VOLUME+1];


	GW_tsh_functrace( FUNC_ISDFAMVOL, GW_FUNC_START, NULL);

	if (pTablep != NULL) {
		*pTablep = NULL;
	};


	if (pVolume_sp == NULL) {
		goto EndOfFunc;
	};


	pszWkp = D_strchr(pszVolName, ':');
	if ( pszWkp != NULL ) {
		_splitpath(pszVolName, aszWvol, NULL, NULL, NULL);
		pszWkp = D_strchr(aszWvol, ':');
		if ( pszWkp != NULL ) {
			*pszWkp = '\0';
		};
	} else {
		if ( strlen(pszVolName) > _MAX_VOLUME ) {
			goto EndOfFunc;
		};
		strcpy( aszWvol, pszVolName);
	}

	pVolp = pVolume_sp;

	while(1) {
		if ( stricmp( pVolp->aszNWVol, aszWvol) == 0 ) {
			if (pTablep != NULL){
				*pTablep = pVolp;
			};
			iRtnc = YES;
			break;
		};
		if ( pVolp->pUshirop == NULL) {
			break;
		};
		pVolp = pVolp->pUshirop;
	}

EndOfFunc:
	GW_tsh_functrace( FUNC_ISDFAMVOL, GW_FUNC_END, iRtnc );
	return( iRtnc );
}




/*EnglishComment**************************************************************
 *  ID   = SearchName_tByName                                                *
 *  name = Entry searching routine                                           *
 *  func = To look for an entry whose name matches with pszEntryName,        *
 *         this routine searches the subdirectory information in the name    *
 *         conversion table pointed by pNamep, and returns the pointer to    *
 *         the same named entry.                                             *
 *  i/o  = pNamep: struct GName_t *:inp: pointer to the name conversion      *
 *                                        table of the subdirectory          *
 *         pszEntryName: BYTE *:inp: name to be searched                     *
 *         bSearchMode : BOOL  :inp: indicator of searching by DFS name or   *
 *                                   NetWare name:                           *
 *                                      MODE_DFS (0): search by DFS name.    *
 *                                      MODE_NW (1) : search by NetWare name.*
 *         pRtnp : struct GName_t **:oup: pointer to the searched name       *
 *                                        conversion table                   *
 *  return = 0: normal end (the target information searched)                 *
 *          -1: abnormal end (no name conversion table exists; pointer=NULL) *
 *          -2: abnormal end (no child directory information in the          *
 *                            designated subdirectory)                       *
 *          -3: abnormal end (no matched entry found)                        *
 *  common = none                                                            *
 *  note = This routine searches the subdirectory pointed by the child       *
 *         directory information in the name conversion table.  Thus         *
 *         pNamep should be NULL (i.e., no name conversion table) or the     *
 *         pointer to the subdirectory table.                                *
 *  date   = Dec. 16, 1994 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int SearchName_tByName ( pszEntryName, pNamep, bSearchMode, pRtnp)
BYTE			*pszEntryName;
struct GName_t	*pNamep;
BOOL			bSearchMode;
struct GName_t	**pRtnp;
{
	struct GName_t *pWorkp;

	*pRtnp = NULL;


	if (pNamep == NULL) {
		return(FNWERR_PTR_IS_NULL);
	};

	pWorkp = pNamep->pKoDirp;
	if (pWorkp == NULL){
		return(FNWERR_NO_SUBDIR);
	};


	while(1) {

		if (bSearchMode == MODE_NW){
			if ( strcmp (pWorkp->aszEntryName, pszEntryName) == 0 ) {
				*pRtnp = pWorkp;
				return(FNWERR_NORMAL);
			};
		} else {
			if ( strcmp (pWorkp->aszRealName, pszEntryName) == 0 ) {
				*pRtnp = pWorkp;
				return(FNWERR_NORMAL);
			};
		}

		if ( pWorkp->pUshiroDirp == NULL ) {
			break;
		};

		pWorkp = pWorkp->pUshiroDirp;
	}


	*pRtnp = pWorkp;
	return(FNWERR_NO_ENTRY);

}




/*EnglishComment**************************************************************
 *  ID   = FindName_t                                                        *
 *  name = name conversion table search routine                              *
 *  func = Searches the name conversion table for the designated name, and   *
 *         returns the pointer to the searched entry.                        *
 *  i/o  = pszPath : BYTE * :inp: target path name (full path name)          *
 *  return = NULL   : there is no table corresponding to the path name       *
 *           integer: pointer to the searched Name_t                         *
 *  common = none                                                            *
 *  note   = none                                                            *
 *  date   = Dec. 12, 1994 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
struct GName_t * FindName_t (pszPath)
BYTE *pszPath;
{
	int	iCcode;
	BYTE *pszBufp = pszPath;
	BYTE aszEntryName[_MAX_NAME];
	struct GVolume_t *pWvolp;
	struct GName_t *pWorkp;
	struct GName_t *pRecvp;
	struct GName_t *pRtnp;

	iCcode = isDFAMVolume(pszPath, &pWvolp);

	if (iCcode == NO) {
		pRtnp = NULL;
		goto EndOfFunc;
	};

	pWorkp = pWvolp->pNamep;


	while ( *pszBufp != ':' ) {
		pszBufp++;
	}
	pszBufp++;

	do {

		ThreadSwitch();


		aszEntryName[0] = '\0';

		iCcode = GetEntry( pszBufp, aszEntryName, &pszBufp );
		if (iCcode != GWERR_NORMAL) {
			pRtnp = NULL;
			goto EndOfFunc;
		};


		iCcode = SearchName_tByName( aszEntryName, pWorkp, MODE_NW, &pRecvp);
		if (iCcode != FNWERR_NORMAL) {
			pRtnp = NULL;
			goto EndOfFunc;
		};


		pWorkp = pRecvp;

	} while ( (pszBufp != NULL) && (*pszBufp != '\0') );

	pRtnp = pRecvp;

EndOfFunc:

	return( pRtnp );

}




/*EnglishComment**************************************************************
 *  ID   = AddToDelList                                                      *
 *  name = Deletion-mark setting routine                                     *
 *  func = Sets a "deleted mark" to the designated member in the name        *
 *         conversion table, and registers the member to the parent          *
 *         directory's deletion list.  If the number of the deletion list    *
 *         exceeds DELLIST_MAX, the top member of the list is freed to       *
 *         register a new entry.  The time of deletion is stored to          *
 *         uiDelTime.                                                        *
 *  i/o  = pNamep:struct GName_t * :inp: pointer to the name conversion      *
 *                                       to be set a "deleted mark"          *
 *  return = GWERR_NORMAL: normal end                                        *
 *           GWERR_BUG   : internal error                                    *
 *                                                                           *
 *  common = none                                                            *
 *  note   = This routine sets a "deleted mark" to the entry deleted by      *
 *           EraseDFAMFile or EraseDFAMDir.  Such entry does not exist in    *
 *           the DFAM and DFS, but exists in the name conversion table.      *
 *  date   = Feb. 18, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int AddToDelList( pNamep )
struct GName_t *pNamep;
{
	struct GName_t *pWPapap =	NULL;
	struct GName_t *pDelWorkp;
	int	iDelcnt;

	if ( pNamep == NULL) {
		GW_tsh_log( (int)pNamep , "Ad2DelLsBUG");
		return (GWERR_BUG);
	};

	pWPapap = pNamep->pOyaDirp;

	if (pWPapap == NULL) {

		GW_tsh_log( (int)pNamep , "Ad2DelLsBUG");
		return(GWERR_BUG);
	};

	if (pNamep->pDelListTop != NULL) {
		ClearFNWDellist(pNamep->pDelListTop);
		pNamep->pDelListTop = NULL;
	};

	pDelWorkp = pWPapap->pDelListTop;

	if (pDelWorkp == NULL) {
		pWPapap->pDelListTop = pNamep;

	} else {
		pDelWorkp = pWPapap->pDelListTop;
		iDelcnt = 1;
		while(pDelWorkp->pDelUshirop != NULL) {
			pDelWorkp = pDelWorkp->pDelUshirop;
			iDelcnt++;
		}
		pDelWorkp->pDelUshirop = pNamep;
		pNamep->pDelMaep = pDelWorkp;

		if (iDelcnt > DELLIST_MAX) {
			pDelWorkp = pWPapap->pDelListTop;
			pWPapap->pDelListTop = pDelWorkp -> pDelUshirop;
			(pDelWorkp->pDelUshirop) -> pDelMaep = NULL;
			DeleteName_t(pDelWorkp, 2);
		};
	}

	time( &(pNamep->uiDelTime) );
	pNamep -> bExist = FALSE;

	return(GWERR_NORMAL);
}




/*EnglishComment**************************************************************
 *  ID   = RestoreName_t                                                     *
 *  name = Deletion mark erasing routine                                     *
 *  func = Erases the "deletion mark" from the deletion list member.         *
 *  i/o  = pNamep : struct GName_t * :inp: pointer to the name conversion    *
 *                                         whose deletion-mark will be erased*
 *  return = GWERR_NORMAL: normal end                                        *
 *           GWERR_BUG   : internal error                                    *
 *  common = none                                                            *
 *  note   = none                                                            *
 *  date   = Feb. 18, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int RestoreName_t(pNamep)
struct GName_t *pNamep;
{

	struct GName_t *pFrontp;
	struct GName_t *pBackp;

	if (pNamep == NULL) {
		GW_tsh_log( (int)pNamep , "RstrNameBUG");
		return(GWERR_BUG);
	};

	pFrontp	=	pNamep->pDelMaep;
	pBackp	=	pNamep->pDelUshirop;


	if (pFrontp == NULL) {
		if (pBackp != NULL) {
			(pNamep->pOyaDirp)->pDelListTop = pBackp;
			pBackp->pDelMaep = NULL;
		} else {
			(pNamep->pOyaDirp)->pDelListTop = NULL;
		}
	} else {
		if (pBackp != NULL) {
			pFrontp->pDelUshirop = pBackp;
			pBackp->pDelMaep = pFrontp;
		} else {
			pFrontp->pDelUshirop = NULL;
		}
	}

	pNamep->pDelMaep = NULL;
	pNamep->pDelUshirop = NULL;
	pNamep->uiDelTime = (time_t)0;
	pNamep->bExist = TRUE;

	return(GWERR_NORMAL);
}




/*EnglishComment**************************************************************
 *  ID   = ClearFNWDellist                                                   *
 *  name = Deletion list deleting routine                                    *
 *  func = Frees all the deletion lists starting from pDelp.                 *
 *  i/o  = pDelp : struct GName_t * :inp: pointer to the deletion list to    *
 *                                        be freed                           *
 *  return =                                                                 *
 *  common = none                                                            *
 *  note   = This routine deletes all the deletion member under the          *
 *           designated subdirectory.  The searching range is limited to the *
 *           designated directory (i.e., does not go deeper than the dir.).  *
 *  date   = Feb. 18, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
void ClearFNWDellist(pDelp)
struct GName_t *pDelp;
{
	struct GName_t *pNowp = pDelp;
	struct GName_t *pNextp;

	while (pNowp != NULL) {
		pNextp = pNowp->pDelUshirop;

		DeleteName_t(pNowp, 2);

		pNowp = pNextp;
	}
	return;
}



/*EnglishComment**************************************************************
 *  ID   = EntSynChk                                                         *
 *  name = Path name syntax check routine                                    *
 *  func = Checks if the path name is valid.                                 *
 *                                                                           *
 *  i/o  = pEnt : BYTE * : i : target entry                                  *
 *  return = none                                                            *
 *  common = none                                                            *
 *  note   = none                                                            *
 *  date   = Feb. 18, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/

int EntSynChk ( pEnt )
BYTE *pEnt;
{
	BYTE	aszName[_MAX_PATH];
	BYTE	aszExt[_MAX_PATH];
	int		iRtnc = GWERR_NORMAL;


	DevideFileName(pEnt, aszName, aszExt);


	if ( (strlen(aszName) > 8) || (strlen(aszExt) > 3) ) {
		iRtnc = GWERR_INVAL;
	} else {


		if ( invalchk(aszName, OFF)) {
			iRtnc = GWERR_INVAL;
		} else {

			if ( aszExt[0] != '\0' ) {
				if ( invalchk(aszExt, OFF)){
					iRtnc = GWERR_INVAL;
				};
			};
		}
	}


	if (iRtnc != GWERR_NORMAL) {
		GW_tsh_functrace( FUNC_ENTSYNCHK, GW_FUNC_END, iRtnc);
	};

	return(iRtnc);

}




/*EnglishComment**************************************************************
 *  ID   = AllocName_t                                                       *
 *  name = Name conversion table holding routine                             *
 *  func = Holds the name conversion table, and returns the pointer to the   *
 *         table.                                                            *
 *                                                                           *
 *  i/o  =  none                                                             *
 *                                                                           *
 *  return = pointer to the assigned area                                    *
 *                                                                           *
 *  common = none                                                            *
 *  note   = none                                                            *
 *  date   = Feb. 18, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
struct GName_t * AllocName_t()
{
	struct GName_t *pGetp;

	pGetp = (struct GName_t *)GW_malloc( sizeof(struct GName_t) );
	if (pGetp != NULL) {

		strcpy(pGetp->aszIcat, NAME_ICAT);
		pGetp->pVolp			=	NULL;
		pGetp->pOyaDirp			=	NULL;
		pGetp->pKoDirp			=	NULL;
		pGetp->pMaeDirp			=	NULL;
		pGetp->pUshiroDirp		=	NULL;
		pGetp->pDelListTop		=	NULL;
		pGetp->pDelMaep			=	NULL;
		pGetp->pDelUshirop		=	NULL;
		pGetp->uiDelTime		=	(time_t)0;
		pGetp->bExist			=	FALSE;
		pGetp->aszEntryName[0]	=	'\0';
		pGetp->aszRealName[0]	=	'\0';
	};

	return(pGetp);
};




/*EnglishComment**************************************************************
 *  ID   = PurgeOldDeletedEntry                                              *
 *  name = Deletable Entry delete routine                                    *
 *  func = Deletes all the expired deletable-entries on the volume.          *
 *                                                                           *
 *  i/o  = none                                                              *
 *                                                                           *
 *  return = none                                                            *
 *                                                                           *
 *  common = none                                                            *
 *  note   = none                                                            *
 *  date   = Jan. 20, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
void PurgeOldDeletedEntry( pszVolName )
BYTE *pszVolName;
{
	time_t uiDelLimit;
	struct GName_t *pName;
	struct GVolume_t *pVol;
	BOOL   bSemaph = FALSE;


	GW_tsh_functrace( FUNC_PURGEOLDDELETED , GW_FUNC_START, NULL);

	if (pszVolName == NULL || *pszVolName == '\0') {
		GW_tsh_log( (int)pszVolName , "PurgeOldBUG");
		goto EndOfFunc;
	};

	if ( isDFAMVolume( pszVolName, &pVol ) == NO ) {
		GW_tsh_log( (int)pszVolName , "PurgeOldBUG");
		goto EndOfFunc;
	};

	WaitOnLocalSemaphore(pVol->uiVolAndName_sh);
	bSemaph = TRUE;


	time(&uiDelLimit);
	uiDelLimit -= DELETEDENTRYKEEPTIME;


	pName = pVol->pNamep;


	if (pName == NULL) {
		goto EndOfFunc;
	};

	DeleteAllDeleted(pVol->pNamep, uiDelLimit);

EndOfFunc:
	if ( bSemaph == TRUE ) {
		SignalLocalSemaphore(pVol->uiVolAndName_sh);
	};

	GW_tsh_functrace( FUNC_PURGEOLDDELETED , GW_FUNC_END, NULL);
	return;
}




/*EnglishComment**************************************************************
 *  ID   = DeleteAllDeleted                                                  *
 *  name = Expired entry deletion routine                                    *
 *  func = Searches the subdirectory (whose level is same as pTopp) for a    *
 *         expired entry, and deletes the entry.  If the entry has a sub-    *
 *         directory, the deletion goes recursively.                         *
 *  i/o  = pTopp : struct GName_t * :inp: top pointer to the subdirectory    *
 *                                         of the name conversion            *
 *         uiLimit: time_t          :inp: expiration period                  *
 *                                                                           *
 *  return = none                                                            *
 *                                                                           *
 *  common = none                                                            *
 *  note   =                                                                 *
 *  date   = Jan. 20, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
void DeleteAllDeleted(pTopp, uiLimit)
struct GName_t *pTopp;
time_t	uiLimit;
{

	struct GName_t *pNowp = pTopp;
	struct GName_t *pNextp;
	struct GName_t *pPapap;

	if (pTopp == NULL) {
		goto EndOfFunc;
	};

	pPapap = pNowp->pOyaDirp;

	while(1) {

		ThreadSwitch();

		pNextp = pNowp->pUshiroDirp;

		if(pNowp->pKoDirp != NULL) {
			DeleteAllDeleted(pNowp->pKoDirp, uiLimit);
		} else {
			if ( (pNowp->uiDelTime != (time_t)0) && (pNowp->uiDelTime <= uiLimit) ) {
				DeleteDelMember(pNowp);
			};
		}

		pNowp = pNextp;

		if (pNowp == NULL) {
			break;
		};

	}


	if ( (pPapap) && (pPapap->pKoDirp == NULL) && (pPapap->bConvflg == FALSE) ) {
		DeleteName_t(pPapap, 2);
	};


EndOfFunc:
	return;

}




/*EnglishComment**************************************************************
 *  ID   = DeleteDelMember                                                   *
 *  name = Deleted-entry deletion routine                                    *
 *  func = Deletes a deleted-entry (i.e., an entry with a deleted=mark"),    *
 *         and re-sets the pointers of the deleted list and name conversion  *
 *         list.                                                             *
 *  i/o  = pTopp : struct GName_t * :inp: top pointer to the subdirectory    *
 *                                         of the name conversion            *
 *                                                                           *
 *  return = none                                                            *
 *                                                                           *
 *  common = none                                                            *
 *  note   =                                                                 *
 *  date   = Jan. 20, 1995 by Isao Ichiyoshi                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
DeleteDelMember ( pNamep )
struct GName_t *pNamep;
{
	struct GName_t *pPapap;
	struct GName_t *pAnikip;
	struct GName_t *pOtotop;

	if ( pNamep == NULL ) {
		return;
	};

	pPapap = pNamep->pOyaDirp;
	pAnikip = pNamep->pDelMaep;
	pOtotop = pNamep->pDelUshirop;



	if ( pOtotop != NULL ) {
		if ( pAnikip != NULL) {
			pAnikip->pDelUshirop = pOtotop;
			pOtotop->pDelMaep = pAnikip;
		} else {
			pOtotop->pDelMaep = NULL;
			if (pPapap != NULL) {
				pPapap->pDelListTop   = pOtotop;
			};
		}
	} else {
		if ( pAnikip != NULL ) {
			pAnikip->pDelUshirop = NULL;
		} else {
			if( pPapap != NULL ) {
				pPapap->pDelListTop = NULL;
			};
		}
	}


	DeleteName_t(pNamep, 1);

	return;
}
