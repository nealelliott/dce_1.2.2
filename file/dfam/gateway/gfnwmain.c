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
 * $Log: gfnwmain.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:24  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:41  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:02:16  takano
 * 	First Submit
 * 	[1996/02/21  08:02:12  takano]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nwdir.h>
#include <ctype.h>
#include <nwsemaph.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <process.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "gfnw.h"


/*EnglishComment**************************************************************/
/*  ID   = GetNWPath                                                         */
/*  name = NetWare path name processing routine                              */
/*  func = Gets a NetWare path corresponding to the DFS path.                */
/*         (1) Returns the full path name of the DFAM Volume entry           */
/*             corresponding to the DFS full path name defined in pszDFSPath.*/
/*             Returns an error if the designated path name does not         */
/*             correspond with a DFAM Volume.                                */
/*         (2) To pinpoint the target volume, a parameter for a DFAM Volume  */
/*             is needed (pszVolName).                                       */
/*                                                                           */
/*  i/o  = pszDFSPath:BYTE *:inp: DFS full path name                         */
/*         pszVolName:BYTE *:inp: target volume name                         */
/*         pszNWPath :BYTE *:out: DFAM full path name                        */
/*         pDFSEntp  :struct DFSFakeDirEnt * :inp: top pointer to the DFS    */
/*                                                 entry list.               */
/*                                                                           */
/*  return = GWERR_NORMAL     : normal end                                   */
/*           GWERR_INVAL      : pszDFSPath does not correspond to DFAM       */
/*                              Volume.                                      */
/*           GWERR_NOT_DFAMVOL: pszVolName is not a Gateway volume.          */
/*           GWERR_NOMEM      : memory shortage                              */
/*           GWERR_LONG       : exceeded the area (255 bytes) for pszNWPath  */
/*           GWERR_KEY_IS_FULL: an unique key is not assigned.               */
/*           GWERR_BUG        : internal error                               */
/*                                                                           */
/*  common = none                                                            */
/*  note   = pDFSEntp and pNWEntp can be NULL.                               */
/*  date   = Dec. 13, 1994 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int GetNWPath ( pszDFSPath, pszVolName, pszNWPath , pDFSEntp)
BYTE	*pszDFSPath;
BYTE	*pszVolName;
BYTE	*pszNWPath;
struct DFSFakeDirEnt	*pDFSEntp;
{
	BYTE	aszEntryBuff[_MAX_PATH+1];
	BYTE	aszPath[_MAX_PATH+1];
	BYTE	aszName[_MAX_PATH+1];
	BYTE	aszExt[_MAX_PATH+1];
	BYTE	aszTestfname[_MAX_NAME];
	int		iCnt;
	int		iLen;
	int		iCcode;
	int		iRtnc = GWERR_NORMAL;
	BOOL	bConvflg = FALSE;
	BOOL	bSemaph  = FALSE;
	BYTE	*pszEntryBufp;
	BYTE	*pszDFSPathp=pszDFSPath;
	BYTE	*pszDFSp;
	struct	GVolume_t *pVolp;
	struct	GName_t	*pNametblp;
	struct	GName_t	*pNewNamep;
	struct	GName_t	*pMakeNamep;


	GW_tsh_functrace( FUNC_GETNWPATH, GW_FUNC_START, NULL);


	aszPath[0]		=	'\0';
	aszName[0]		=	'\0';
	aszExt[0]		=	'\0';
	*pszNWPath		=	'\0';


		if ( (pszDFSPath == NULL) || (*pszDFSPath == '\0') ) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};


	if ( (pszVolName == NULL) || (*pszVolName == '\0') ) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};


	if ( pszNWPath == NULL ) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};


	iCcode = isDFAMVolume(pszVolName, &pVolp);
	if (iCcode == NO) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};

	sprintf(pszNWPath, "%s:", pVolp->aszNWVol);

	WaitOnLocalSemaphore(pVolp->uiVolAndName_sh);
	bSemaph = TRUE;

	pNametblp = pVolp->pNamep;
	pszDFSp = &(pVolp->aszDFSPath[0]);
	iLen = strlen(pszDFSp);


	if ( strncmp(pszDFSp, pszDFSPath, iLen) != 0 ) {
		iRtnc = GWERR_INVAL;
		goto EndOfFunc;
	};

	pszDFSPathp += iLen;

	if (*pszDFSPathp == '\/') {
		pszDFSPathp++;
	};



	while(*pszDFSPathp != '\0') {
		aszEntryBuff[0] = '\0';
		pszEntryBufp = &aszEntryBuff[0];

		ThreadSwitch();


		while ( (*pszDFSPathp != '\0') && (*pszDFSPathp != '\/') ) {
			*pszEntryBufp++ = *pszDFSPathp++;
		};
		*pszEntryBufp = '\0';

		if ( *pszDFSPathp == '\/' ) {
			pszDFSPathp++;
			if ( aszEntryBuff[0] == '\0' ) {
				iRtnc = GWERR_INVAL;
				goto EndOfFunc;
			};
		};


		pNewNamep = NULL;
		if (pNametblp != NULL) {
			iCcode = SearchName_tByName( aszEntryBuff, pNametblp, MODE_DFS, &pNewNamep);



			if (iCcode == FNWERR_NORMAL) {


				if ( ( strlen(pszNWPath) + strlen(pNewNamep->aszEntryName + 1 ) > _MAX_PATH ) ) {
					iRtnc = GWERR_LONG;
					goto EndOfFunc;
				};

				sprintf( pszNWPath, "%s\\%s", pszNWPath, pNewNamep->aszEntryName );
				pNametblp = pNewNamep;
				continue;
			} else {

				pNametblp = NULL;
			}
		};




		bConvflg = FALSE;


		DevideFileName(aszEntryBuff, aszName, aszExt);


		if (strlen(aszName) > 8) {
				aszName[5] = '\0';

			bConvflg = TRUE;
		};


		if ( (bConvflg == FALSE) && (isDevice(aszName) == GWERR_ISDEVICE) ) {
			bConvflg = TRUE;
		};


		if (strlen(aszExt) > 3) {
			aszExt[0] = '\0';
			bConvflg = TRUE;
		};


		if ( invalchk(aszName, ON)){
			bConvflg = TRUE;
		};

		if ( invalchk(aszExt, ON)){
			bConvflg = TRUE;
			*aszExt = '\0';
		};


		if ( (upperchk(aszName)) ){
			bConvflg = TRUE;
		};

		if ( (upperchk(aszExt)) ){
			bConvflg = TRUE;
			*aszExt = '\0';
		};

		D_strupr(aszName);
		if (*aszExt) {
			D_strupr(aszExt);
		};




		if ( (bConvflg == FALSE) && ( strlen(aszName) == 8) ) {
			strcpy( aszTestfname, aszName);
			if ( *aszExt != '\0' ) {
				sprintf(aszTestfname, "%s.%s", aszTestfname, aszExt);
			};

			iCcode = SearchName_tByName( aszTestfname, pNametblp, MODE_NW, &pNewNamep);


			if ( iCcode == FNWERR_NORMAL ) {
				if ( strcmp ( (pNewNamep->aszRealName), aszEntryBuff) != 0 ) {
					bConvflg = TRUE;
				};
			};
		};


		if (bConvflg == TRUE) {
			iLen = strlen(aszName);
			if ( iLen > 5 ){
					aszName[5] = '\0';
					iLen = 5;
			};


			for(iCnt=iLen; iCnt<=5; iCnt++){
				aszName[iCnt] = '~';
			}
			aszName[6] = '\0';

			iCcode = SearchUniqueFileName( 	pszNWPath, aszName, aszExt, aszEntryBuff, aszTestfname, pDFSEntp);
			if (iCcode != GWERR_NORMAL && iCcode != GWERR_RECYCLE) {
				switch ( iCcode ) {
				case GWERR_KEY_IS_FULL:
				case GWERR_LONG:
					iRtnc = iCcode;
					break;
				default:
					iRtnc = GWERR_BUG;
					break;
				};
				goto EndOfFunc;
			};


			if ( ( strlen(pszNWPath) + strlen(aszTestfname) + 1 ) > _MAX_PATH ) {
				iRtnc = GWERR_LONG;
				goto EndOfFunc;
			};



			sprintf(pszNWPath, "%s\\%s", pszNWPath, aszTestfname);

			if (iCcode != GWERR_RECYCLE) {

				pMakeNamep = AddToName_t(pszNWPath);

				if (pMakeNamep == NULL) {
					iRtnc = GWERR_NOMEM;
					goto EndOfFunc;
				};


				strcpy (pMakeNamep->aszRealName, aszEntryBuff);
				pMakeNamep->bConvflg = TRUE;
			};

		} else {



			iLen = strlen(pszNWPath) + strlen(aszName) + 1;
			if (aszExt) {
				iLen += (strlen(aszExt) + 1);
			};

			if (iLen > _MAX_PATH) {
				iRtnc = GWERR_LONG;
				goto EndOfFunc;
			};

			if (*aszExt) {
				sprintf(pszNWPath, "%s\\%s.%s", pszNWPath, aszName, aszExt);
			} else {
				sprintf(pszNWPath, "%s\\%s", pszNWPath, aszName);
			}
		}
	}

EndOfFunc:
	if (bSemaph == TRUE) {
		SignalLocalSemaphore(pVolp->uiVolAndName_sh);
	};


	GW_tsh_functrace( FUNC_GETNWPATH, GW_FUNC_END, iRtnc);

	return(iRtnc);
}




/*EnglishComment**************************************************************/
/*  ID   = GetDFSPath                                                        */
/*  name = DFS full path getting routine                                     */
/*  func = Returns a DFS full path corresponding to the DFAM full path in    */
/*         pszDFAMPath.                                                      */
/*  i/o  = pszDFAMPath:BYTE *:inp: DFAM full path name                       */
/*         pszDFSPath :BYTE *:out: DFS full path name                        */
/*         iMode      :int   :inp: searching mode                            */
/*                            OPENMODE (open mode)                           */
/*                                 Does not check if it exists.  If the      */
/*                                 searched member has a deleted-mark, this  */
/*                                 routine removes the delete information,   */
/*                                 and handles it as a regular name          */
/*                                 conversion member.                        */
/*                            CHECKMODE (checking mode)                      */
/*                                 Checks if the designated NetWare name     */
/*                                 exist.  The delete information is not     */
/*                                 changed even if the searched member is a  */
/*                                 deleted-member.                           */
/*  return = GWERR_NORMAL     : normal end                                   */
/*           GWERR_NOT_DFAMVOL: not a DFAM volume                            */
/*           GWERR_NOMEM      : memory shortage                              */
/*           GWERR_LONG       : DFS name exceeds 1024                        */
/*           GWERR_INVAL      : invalid path name                            */
/*           GWERR_NUSE       : unusable Gateway Volume                      */
/*           GWERR_NW_NOENT   : pszDFAMPath does not exist (only CHECKMODE   */
/*                              is returned)                                 */
/*           GWERR_BUG        : internal error                               */
/*  common = none                                                            */
/*  note   =                                                                 */
/*  date   = Dec. 13, 1994 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int GetDFSPath ( pszDFAMPath, pszDFSPath, iMode )
BYTE *pszDFAMPath;
BYTE *pszDFSPath;
int  iMode;
{
	int		iRtnc = GWERR_NORMAL;
	int		iCcode;
	int		iDFSlen = 0;
	BOOL	bTblcont = FALSE;
	BOOL	bSemaph = FALSE;
	struct GVolume_t	*pW_Voltblp;
	struct GName_t		*pW_Nametblp;
	struct GName_t		*pW_RtnNametblp;
	BYTE	*pszW_DFAMp = pszDFAMPath;
	BYTE	*pszW_DFSp;
	BYTE	aszW_vol[_MAX_VOLUME];
	BYTE	aszW_Entbuf[_MAX_NAME];

	struct stat st;


	GW_tsh_functrace( FUNC_GETDFSPATH, GW_FUNC_START, NULL);


	if ( (pszDFAMPath == NULL) || (*pszDFAMPath == '\0') ) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};

	if ( pszDFAMPath == NULL ) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};

	if ( (iMode != OPENMODE) && (iMode != CHECKMODE) ) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};





	iCcode = GetDFAMVolumeName( pszDFAMPath, aszW_vol );
	if ( iCcode == GWERR_ABEND ) {
		iRtnc = GWERR_INVAL;
		goto EndOfFunc;
	};


	iCcode = isDFAMVolume( aszW_vol, &pW_Voltblp );
	if ( iCcode == NO ) {
		iRtnc = GWERR_NOT_DFAMVOL;
		goto EndOfFunc;
	};


	if ( pW_Voltblp -> usVolStatus != ALLCLEAR ) {
		iRtnc = GWERR_NUSE;
		goto EndOfFunc;
	}

	WaitOnLocalSemaphore(pW_Voltblp->uiVolAndName_sh);
	bSemaph = TRUE;

	pW_Nametblp = pW_Voltblp -> pNamep;
	if ( pW_Nametblp != NULL ) {
		bTblcont = TRUE;
	} else {
		bTblcont = FALSE;
	}


	strcpy ( pszDFSPath, pW_Voltblp->aszDFSPath );
	pszW_DFSp = pszDFSPath + strlen( pszDFSPath );

	pszW_DFAMp += (strlen( aszW_vol ) + 1);


	while ( (pszW_DFAMp != NULL) && (*pszW_DFAMp != '\0') ) {
		aszW_Entbuf[0] = '\0';

		ThreadSwitch();

		iCcode = GetEntry(pszW_DFAMp, aszW_Entbuf, &pszW_DFAMp);
		if (iCcode != GWERR_NORMAL) {
			iRtnc = GWERR_INVAL;
			goto EndOfFunc;
		};


		if ( aszW_Entbuf[0] == '\0' ) {
			break;
		};


		if ( EntSynChk( aszW_Entbuf ) != 0 ) {
			iRtnc = GWERR_INVAL;
			goto EndOfFunc;
		};

		if ( bTblcont == TRUE ) {
			iCcode = SearchName_tByName ( aszW_Entbuf, pW_Nametblp, MODE_NW, &pW_RtnNametblp );
			if ( (iCcode == FNWERR_NORMAL) && (pW_RtnNametblp != NULL) ) {
				pW_Nametblp = pW_RtnNametblp;


				if ( iDFSlen + strlen( pW_Nametblp->aszRealName ) + 1 > DFSPATH_MAX ) {
					iRtnc = GWERR_LONG;
					goto EndOfFunc;
				};

				if ( iMode == OPENMODE ) {
					if ( pW_Nametblp->bExist == FALSE ) {
						RestoreName_t( pW_Nametblp );
					};
				};


				sprintf(pszDFSPath, "%s\/%s", pszDFSPath, pW_Nametblp->aszRealName);
				iDFSlen += ( strlen(pW_Nametblp->aszRealName) + 1);

			} else {
				bTblcont = FALSE;
			}
		};

		if (bTblcont == FALSE) {

			if ( iDFSlen + strlen( aszW_Entbuf ) + 1 > DFSPATH_MAX ) {
				iRtnc = GWERR_LONG;
				goto EndOfFunc;
			};

			sprintf (pszDFSPath, "%s\/%s", pszDFSPath, D_strlwr( aszW_Entbuf) );
			iDFSlen += ( strlen(aszW_Entbuf) + 1);
		};
	}



	if ( iMode == CHECKMODE ) {
		if ( stat( pszDFAMPath, &st) != 0 ) {
			iRtnc = GWERR_NW_NOENT;
		};
	};

EndOfFunc:

	if (bSemaph == TRUE) {
		SignalLocalSemaphore(pW_Voltblp->uiVolAndName_sh);
	};



	if ( (iRtnc != GWERR_NORMAL) && (pszDFSPath != NULL) ) {
		*pszDFSPath = '\0';
	};


	GW_tsh_functrace( FUNC_GETDFSPATH, GW_FUNC_END, iRtnc);

	return(iRtnc);

}






/*EnglishComment**************************************************************/
/*  ID   = invalchk                                                          */
/*  name = Character check and tilde conversion routine                      */
/*  func = Searches for an invalid character, and converts the invalid       */
/*         character to a tilde.                                             */
/*         (1) Searches the designated character string for an invalid       */
/*             character, and converts the character into a tilde.           */
/*         (2) In the case of Hitachi-DFAM, second byte of Shift JIS code is */
/*             ignored (exceptionally, the space in the second byte will be  */
/*             converted to ~~).                                             */
/*                                                                           */
/*  i/o  = pszString    : BYTE *  :inp: target character string              */
/*         iConv        : int     :inp: tilde mode (converted/not converted) */
/*                                                                           */
/*  return = length of the converted characters.  Or, 0 if not converted.    */
/*                                                                           */
/*  common = none                                                            */
/*  note   = If NO(0) is set to iConv, this function returns when an invalid */
/*           character is found (the return value will be 0 or 1).  Set      */
/*           NO(0) to iConv if you want only to check if there is an invalid */
/*           character in the entry.                                         */
/*                                                                           */
/*  date   = Jan. 07, 1995 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int invalchk(pszString, iConv)
BYTE *pszString;
int iConv;
{
	int		iInvcnt = 0;
	int		iLcnt;

	for ( iLcnt=0; pszString[iLcnt] != '\0'; iLcnt++ ) {





		if ( (pszString[iLcnt] < 0x20) || ( D_strchr (INVALCHAR_NWENTRY, pszString[iLcnt]) ) ) {
			if (iConv == YES) {
				pszString[iLcnt] = '~';
			};

			iInvcnt++;

			if (iConv == NO) {
				break;
			};
		};
	}

	return(iInvcnt);
}





/*EnglishComment**************************************************************/
/*  ID   = upperchk                                                          */
/*  name = Capital letter checking routine                                   */
/*  func = Checks if there is a capital letter in the designated string.     */
/*  i/o  = pszString : BYTE * :inp: target character string                  */
/*  return = 0 : there was no capital letter in the string.                  */
/*           1 : there was at least one capital letter in the string.        */
/*  common = none                                                            */
/*  note   = This function finishes searching when a capital letter is       */
/*           detected: this function returns either "there was" or "there    */
/*           was not" as a value.                                            */
/*                                                                           */
/*  date   = Jan. 07, 1995 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int upperchk(pszString)
BYTE *pszString;
{

	for( ; *pszString != '\0'; pszString++) {


		if (isupper (*pszString)) {
			return(1);
		};
	}
	return(0);
}





/*EnglishComment**************************************************************/
/*  ID   = DevideFileName                                                    */
/*  name = Entry name splitting routine                                      */
/*  func = Splitts an entry name into a file name and an extension.          */
/*  i/o  =  pszFullname :inp: target entry name to be splitted               */
/*          pszName     :out: name other than an extension                   */
/*          pszExt      :out: extension                                      */
/*                                                                           */
/*  return = 0      : normal                                                 */
/*           others : abnormal (internal error, no parameter)                */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Jan. 07, 1995 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int DevideFileName(pszFullname, pszName, pszExt)
BYTE *pszFullname;
BYTE *pszName;
BYTE *pszExt;
{
	int iRtnc = GWERR_NORMAL;
	int iI,iJ;
	int iLastpos;

	if ( (pszFullname == NULL) || (*pszFullname == '\0') || (pszName == NULL) || (pszExt== NULL) ) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};

	*pszName = '\0';
	*pszExt = '\0';

	if ( *pszFullname == '\0' || pszFullname == NULL ){
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};

	iLastpos = strlen(pszFullname) - 1;
	for (iI=iLastpos; iI>=0; iI--){
		if (pszFullname[iI] == '.'){

			if ( (iI == iLastpos) || (iI == 0) ){
				strcpy(pszName, pszFullname);
				iRtnc = GWERR_NORMAL;
				goto EndOfFunc;
			};

			for(iJ=0; iJ<iI; iJ++){
				pszName[iJ] = pszFullname[iJ];
			}
			pszName[iI] = '\0';
			iI++;
			strcpy(pszExt, &pszFullname[iI]);
			iRtnc = GWERR_NORMAL;
			goto EndOfFunc;
		};
	}
	if (pszName[0] == '\0'){
		strcpy(pszName, pszFullname);
	};

EndOfFunc:

	if (iRtnc != GWERR_NORMAL) {
		GW_tsh_functrace( FUNC_DEVIDEFILENAME, GW_FUNC_START, iRtnc);
	};

	return( iRtnc );
}




/*EnglishComment**************************************************************/
/*  ID   = SearchUniqueFileName                                              */
/*  name = Unique NetWare entry name processing routine                      */
/*  func = Compares the NetWare directory information with the DFS file name */
/*         to get a unique NetWare entry name.                               */
/*  i/o  = pszRtnstring : BYTE * :inp: NetWare path name                     */
/*         pszRtnstring : BYTE * :inp: entry name (excluding an extension)   */
/*         pszRtnstring : BYTE * :inp: extension                             */
/*         pszRtnstring : BYTE * :i/o: pointer to a unique name              */
/*  return = GWERR_NORMAL      : normal end                                  */
/*           GWERR_LONG        : NetWare name exceeds 255                    */
/*           GWERR_KEY_IS_FULL : an unique key is not assigned.              */
/*           GWERR_RECYCLE     : The deleted name was reused.                */
/*           GWERR_BUG         : internal error                              */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Jan. 07, 1995 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int SearchUniqueFileName(pszNWPath, pszName, pszExt, pszRealName, pszRtnstring, pDFSp)
BYTE	*pszNWPath;
BYTE	*pszName;
BYTE	*pszExt;
BYTE	*pszRealName;
BYTE	*pszRtnstring;
struct DFSFakeDirEnt	*pDFSp;
{
	int iRtnc = GWERR_NORMAL;
	int		iCnt;
	int		iKey_Start;
	int		iKey_Now;
	BOOL	bFound = FALSE;
	struct DFSFakeDirEnt	*pDp;
	struct GName_t			*pW_Namep;
	struct GName_t	*pKeepNamep;
	BYTE	aszWorkFullPath[_MAX_PATH+1];
	BYTE	aszKeyBuf[3];

	if (	pszNWPath	== NULL	||	*pszNWPath		== '\0' ||
			pszName		== NULL	||	*pszName		== '\0' ||
			pszExt		== NULL	||	pszRealName	== NULL ) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};


	iKey_Start = GetKeyNumber( pszRealName );

	if ( iKey_Start < 0 ) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};

	for (iCnt=0; iCnt <= KEYNUMBER_MAX-1; iCnt++){

		iKey_Now = iKey_Start + iCnt;
		if ( iKey_Now > (KEYNUMBER_MAX-1) ) {
			iKey_Now -= KEYNUMBER_MAX;
		};


		ito36(iKey_Now, aszKeyBuf);

		sprintf(pszRtnstring, "%s%s", pszName, aszKeyBuf);
		if (*pszExt){
			sprintf(pszRtnstring, "%s.%s", pszRtnstring, pszExt);
		} else;




		if( pDFSp != NULL ) {
			pDp = DFSExistchk( pszRtnstring, pDFSp );
			if ( pDp != NULL) {
				bFound = TRUE;
				continue;
			};
		};


		if ( NWExistchk( pszRtnstring, pszNWPath ) != NULL) {
			bFound = TRUE;
			continue;
		};



		if ( strlen(pszNWPath) + strlen(pszRtnstring) + 1 > _MAX_PATH ) {
			iRtnc = GWERR_LONG;
			goto EndOfFunc;
		};

		sprintf( aszWorkFullPath, "%s\\%s", pszNWPath, pszRtnstring);

		pW_Namep = FindName_t( aszWorkFullPath );

		if (pW_Namep != NULL) {
			bFound = TRUE;
			if (pW_Namep->bExist == FALSE){
				if ( pKeepNamep == NULL ){
					pKeepNamep = pW_Namep;
				};
			};
			continue;
		} else {
			bFound = FALSE;
			break;
		}

	}

EndOfFunc:




	if ( (!iRtnc)&& (bFound) ){
		if (pKeepNamep != NULL) {


			RestoreName_t(pKeepNamep);
			strcpy( pKeepNamep->aszRealName, pszRealName);
			iRtnc = GWERR_RECYCLE;
		} else {
			iRtnc = GWERR_KEY_IS_FULL;
		}
	} else;


	if ((iRtnc != GWERR_NORMAL) && (iRtnc != GWERR_RECYCLE)) {
		GW_tsh_functrace( FUNC_SEARCHUNIQUEFILENAME, GW_FUNC_END, iRtnc);
	};
	return(iRtnc);

}




/*EnglishComment**************************************************************/
/*  ID   =  ito36                                                            */
/*  name = 36-digit conversion routine                                       */
/*  func = Coverts an inputted int value into a 36-digit character string.   */
/*  i/o  = iNumber : int : i : int value before conversion                   */
/*         pszRtnstring : char * : converted 36-digit character string       */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Jan. 07, 1995 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
void	ito36(iNumber, pszRtnstring)
int	iNumber;
char	*pszRtnstring;
{
	int	iKeta_of_1;
	int	iKeta_of_36;

	iKeta_of_1 = iNumber % 36;
	iKeta_of_36 = iNumber / 36;
	pszRtnstring[0] = STR36[iKeta_of_36];
	pszRtnstring[1] = STR36[iKeta_of_1];
	pszRtnstring[2] = '\0';

}





/*EnglishComment**************************************************************/
/*  ID   = from36toi                                                         */
/*  name = 36-digit-to-int conversion routine                                */
/*  func = Coverts a 36-digit character string into an int value.            */
/*  i/o  = pszString : i : 36-digit character string                         */
/*  return = converted init value                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Jan. 07, 1995 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int from36toi(pszString)
char *	pszString;
{
	int iRtnval;
	int iI,iJ;

	for(iI=0; iI<2; iI++){
		for(iJ=0; pszString[iI] != STR36[iJ]; iJ++);
		if(iI == 0){
			iRtnval = iJ * 36;
		} else {
			iRtnval += iJ;
		}
	}

	return(iRtnval);
}




/*EnglishComment**************************************************************/
/*  ID   = GetKeyNumber                                                      */
/*  name = Getting key routine                                               */
/*  func = Calculates the value corresponding to a key number from the       */
/*         character string designated in pszString.                         */
/*                                                                           */
/*  i/o  = pszString: BYTE * :inp: character string to get a key number      */
/*                                                                           */
/*  return =  value of key number                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = The return value of this function is, first, multiplying the    */
/*           each character code from the beginning with a weighed number    */
/*           (*1, *2, ...), second, summingthem up, then cutting the sum     */
/*           to fit 2 digits of 36-digit.                                    */
/*  date   = Jan. 20, 1995 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int GetKeyNumber( pszString )
BYTE *pszString;
{
	BYTE	*pszNow = pszString;;
	int iMojicnt = 1;
	int	iNowValue;
	int	iTotal = 0;

	while ( *pszNow != '\0' ) {
		iNowValue = (int)*pszNow * iMojicnt;
		iTotal += iNowValue;
		iMojicnt++;
		pszNow++;
	}

	return( iTotal % KEYNUMBER_MAX );
}




/*EnglishComment**************************************************************/
/*  ID   = DFSExistchk                                                       */
/*  name = DFS entry searching routine                                       */
/*  func = Searches an entry name list given from the upper module to check  */
/*         if the designated name exists in DFS.                             */
/*                                                                           */
/*  i/o  = pszName: i : BYTE * : target entry                                */
/*         pDFSp  : i : struct DFSFakeDirEnt * : pointer to DFS entry list   */
/*                                                                           */
/*  return = not NULL : entry was found (returns apointer to the name)       */
/*           NULL     : no entry found                                       */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Jan. 20, 1995 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
struct DFSFakeDirEnt * DFSExistchk ( pszName, pDFSp )
BYTE *pszName;
struct DFSFakeDirEnt *pDFSp;
{
	struct DFSFakeDirEnt *pWkp = pDFSp;
	BYTE aszName[_MAX_NAME];


	if ( pszName == NULL && *pszName == NULL ) {

		GW_tsh_functrace( FUNC_DFSEXISTCHK, GW_FUNC_END, NULL);
		return(NULL);
	};


	strcpy( aszName, pszName);
	D_strlwr(aszName);

	while ( pWkp != NULL ) {
		if (  strcmp ((pWkp->szName), aszName) == 0 ) {
			return(pWkp);
		};
		pWkp = pWkp->pNext;
	}


	return(NULL);

}



/*EnglishComment**************************************************************/
/*  ID   = NWExistchk                                                        */
/*  name = NetWare searching routine                                         */
/*  func = Checks if the target name exists in NetWare.                      */
/*                                                                           */
/*  i/o  = pszFullpath : i : BYTE * : NetWare full path name for the target  */
/*                                                                           */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = Jan. 20, 1995 by Isao Ichiyoshi                                 */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
BYTE * NWExistchk ( pszEntry, pszPath )
BYTE *pszEntry;
BYTE *pszPath;
{
	DIR *pDirp;
	DIR *pDirentp;
	BYTE aszChkPath[_MAX_PATH+1];
	BOOL bFound;
	BYTE *pWkp;
	BYTE *pRtnp = NULL;

	if (pszEntry == NULL || *pszEntry == '\0') {
		goto EndOfFunc;
	};

	if (pszPath == NULL || *pszPath == '\0') {
		goto EndOfFunc;
	}

	strcpy (aszChkPath, pszPath);

	pWkp = D_strrchr( aszChkPath, '\\');

	if ( *(pWkp + 1) == '\0' ){
		*pWkp = '\0';
	};


	if ( (strlen(aszChkPath) + 3 + 1) > _MAX_PATH ) {
		goto EndOfFunc;
	};

	strcat(aszChkPath, WILDCARD);

	pDirp = opendir (aszChkPath);

	if (pDirp == NULL) {
		goto EndOfFunc;
	};

	bFound = FALSE;

	while (1) {
		pDirentp = readdir(pDirp);
		if (pDirentp == NULL) {
			break;
		};

		if ( strcmp(pDirentp->d_name, pszEntry) == 0) {
			bFound = TRUE;
			break;
		};
	}

	closedir(pDirp);

	if (bFound == TRUE) {
		pRtnp = pszEntry;
	};

EndOfFunc:

	return(pRtnp);
}


