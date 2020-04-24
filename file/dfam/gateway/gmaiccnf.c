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
 * $Log: gmaiccnf.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:32  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:48  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:04:54  takano
 * 	First Submit
 * 	[1996/02/21  08:04:49  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <niterror.h>
#include <malloc.h>
#include <process.h>
#include <nwdir.h>
#include <ctype.h>
#include <nwenvrn.h>
#include <nwconn.h>
#include <nwsemaph.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netdb.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfammsg.h"
#include "dfamprt.h"
#include "gmai.h"
#include "gfnw.h"

extern LONG ulInitCode;
extern BOOL bDetail_Output;

/*EnglishComment**************************************************************
 *  ID      =   CheckVolumeFile                                              *
 *  name    =   Checks a Config file, and stores main global.                *
 *  func    =   Checks configuration files used by DFAM, and sets them       *
 *              onto the memory.                                             *
 *  i/o     =   none                                                         *
 *  return  =   GWERR_NORMAL  : normal                                       *
 *              GWERR_ABEND   : abnormal (file open error, definition error) *
 *              GWERR_NOMEM   : abnormal (insufficient memory)               *
 *              GWERR_BUG     : abnormal (internal error)                    *
 *              GWERR_UNKNOWN : abnormal (unknown error)                     *
 *  common  =   none                                                         *
 *  note    =   The error cases are as follows:                              *
 *                 1) no version information on the first line               *
 *                 2) DFAM Volume and DFS path are not in a pair             *
 *                 3) DFAM Volume does not exist, or it is not mounted       *
 *              The detailed error information is set to ulInitCode.         *
 *  date    =   Nov. 23, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int CheckVolumeFile(void){

	FILE 	*hFh  = NULL;
	int		iRtnc = GWERR_NORMAL;
	int		iCcode;
	int		iLinecnt;
	int		iVolnum = 0;
	BOOL	bIsVolumeDefined = FALSE;
	BOOL	bLoop;
	BOOL	bVersioninfo_defined = FALSE;
	char	*pszBufp = NULL;
	char	*pszWorkp;
	char	*pszVolp = NULL;
	struct GVolume_t *pVolp;

	ulInitCode = INIER_VOLCHK;
	bDetail_Output = FALSE;


	GW_tsh_functrace( FUNC_CHKVOLFILE, GW_FUNC_START, NULL);



	hFh = fopen ( VOLFILENAME, "r");
	if (hFh == NULL){
		ulInitCode |= INIERC_OPN;
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};

	iLinecnt = 1;
	bLoop = TRUE;

	while(bLoop == TRUE){

		iCcode = LineRead( hFh, &pszBufp);
		if ( iCcode == GWERR_EOF) {
			if ( pszBufp == NULL ) {
				break;
			} else {
				bLoop = FALSE;
			}
		} else if (iCcode == GWERR_NOMEM) {
			ulInitCode |= INIERC_MEM;
			iRtnc = iCcode;
			goto EndOfFunc;
		}

		if ( pszBufp == NULL ) {
			iRtnc = GWERR_BUG;
			ulInitCode |= INIERC_BUG;
			goto EndOfFunc;
		};

		if (iLinecnt == 1){
			if ( strcmp( pszBufp, FILEVERSION_DFAVOL) != 0 ) {
				iRtnc = GWERR_ABEND;
				ulInitCode |= INIERC_INT;
				goto EndOfFunc;
			} else {
				bVersioninfo_defined = TRUE;
			}
		} else {
			if (*pszBufp == '\0') {
				ulInitCode |= INIERC_INV;
				iRtnc = GWERR_ABEND;
				goto EndOfFunc;
			};
			if (iLinecnt % 2 == 0){
				iCcode = vol_exist_chk(pszBufp);


				if (iCcode == GWERR_ABEND) {
					if ( strlen (pszBufp) > GWMSG_STR_MAXSIZE ) {
						StrCut( pszBufp, GWMSG_STR_MAXSIZE );
					};
					GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11015, pszBufp);
					bDetail_Output = TRUE;
				};

				switch (iCcode) {
					case GWERR_NORMAL:
						break;
					case GWERR_ISSYS:
						ulInitCode |= (INIER_VOL_NWVOL | INIERC_SYS);
						iRtnc = GWERR_ABEND;
						goto EndOfFunc;
					case GWERR_DOUBLEDEF:
						ulInitCode |= (INIER_VOL_NWVOL | INIERC_DBL);
						iRtnc = GWERR_ABEND;
						goto EndOfFunc;
					case GWERR_ABEND:
					case GWERR_FAILURE:
						ulInitCode |= (INIER_VOL_NWVOL | INIERC_INV);
						iRtnc = GWERR_ABEND;
						goto EndOfFunc;
					default:
						iRtnc = GWERR_UNKNOWN;
						ulInitCode |= (INIER_VOL_NWVOL | INIERC_UNK);
						goto EndOfFunc;
				};

				pszWorkp = D_strpbrk( pszBufp, "\\\/");
				if ( pszWorkp != NULL) {
					pszVolp = pszWorkp + 1;
				} else {
					pszVolp = pszBufp;
				}


				pszWorkp = D_strchr(pszVolp, ':');
				if (pszWorkp != NULL) {
					*pszWorkp = '\0';
				};

				iCcode = GetVolumeNumber ( pszVolp, &iVolnum );
				if ( iCcode != ESUCCESS ) {
					GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11009, iCcode);
					ulInitCode |= (INIER_VOL_NWVOL | INIERC_API);
					iRtnc = GWERR_FAILURE;
					goto EndOfFunc;
				};


				pVolp = AddToGVolume_t();
				if (pVolp == NULL){
					ulInitCode |= (INIER_VOL_NWVOL | INIERC_MEM);
					iRtnc = GWERR_NOMEM;
					goto EndOfFunc;
				};
				strcpy (pVolp -> aszNWVol, strupr(pszVolp));
				pVolp -> iVolNumber = iVolnum;
				bIsVolumeDefined = TRUE;
			} else {
				iCcode = isDFSPath(pszBufp);
				if (iCcode != 0){
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_VOL_DFS | INIERC_INV);
					bDetail_Output = TRUE;
					GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11016 );
					goto EndOfFunc;
				};


				if ( pszBufp[strlen(pszBufp)-1] == '\/' ) {
					pszWorkp = strrchr(pszBufp, '\/');
					*pszWorkp = '\0';
				};


				if (pVolp == NULL) {
					GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11034);
					ulInitCode |= (INIER_VOL_DFS | INIERC_BUG);
					iRtnc = GWERR_BUG;
					goto EndOfFunc;
				};
				strcpy(pVolp->aszDFSPath , pszBufp);
				bIsVolumeDefined = FALSE;
				pVolp = NULL;
			}
		}
		free( pszBufp);
		pszBufp = NULL;
		iLinecnt++;
	}

	if( bVersioninfo_defined==FALSE ) {
		ulInitCode |= INIERC_INT;
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	if (pVolume_sp == NULL) {
		ulInitCode |= INIERC_NOT;
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	if ( bIsVolumeDefined == TRUE ) {
		ulInitCode |= INIERC_PAIR;
		iRtnc = GWERR_ABEND;
	};

EndOfFunc:

	if (pszBufp) {
		free(pszBufp);
	};

	if (hFh != NULL) {
		fclose(hFh);
	};

	if ( iRtnc == GWERR_NOMEM ) {
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11037 );
	};


	GW_tsh_functrace( FUNC_CHKVOLFILE, GW_FUNC_END, iRtnc);

	return(iRtnc);

}




/*EnglishComment**************************************************************
 *  ID      =   CheckAgentFile                                               *
 *  name    =   DFAMAGT.CFG file analysis routine                            *
 *  func    =   Analyzes DFAMAGT.CFG.                                        *
 *  i/o     =   none                                                         *
 *  return  =   GWERR_NORMAL  : normal                                       *
 *              GWERR_ABEND   : abnormal (file open error, definition error) *
 *              GWERR_NOMEM   : abnormal (insufficient memory)               *
 *              GWERR_BUG     : abnormal (internal error)                    *
 *  common  =   none                                                         *
 *  note    =   1) It is an error if there is no Agent data.                 *
 *              2) The detailed error information is set to ulInitCode.      *
 *  date    =   Nov. 25, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int CheckAgentFile(void){

	FILE 	*hFh  = NULL;
	int		iRtnc = GWERR_NORMAL;
	int		iCcode;
	int		iLinecnt;
	BOOL	bIsAgentDefined = FALSE;
	BOOL	bLoop;
	BOOL	bVersioninfo_defined = FALSE;
	BOOL	bMasterkey_defined = FALSE;
	char	*pszBufp = NULL;
	struct GAgent_t *pAgtp;
	struct hostent *pHost;
	struct nwsockent NWsockent;

	ulInitCode = INIER_AGTCHK;
	bDetail_Output = FALSE;


	GW_tsh_functrace( FUNC_CHKAGTFILE, GW_FUNC_START, NULL);


	hFh = fopen ( AGTFILENAME, "r");
	if (hFh == NULL){
		ulInitCode |= INIERC_OPN;
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};

	bLoop = TRUE;
	iLinecnt = 1;

	while(bLoop == TRUE){
		iCcode = LineRead(hFh, &pszBufp);
		if (iCcode == GWERR_EOF) {
			if (pszBufp == NULL) {
				break;
			} else {
				bLoop = FALSE;
			}
		} else if (iCcode == GWERR_NOMEM) {
			iRtnc = iCcode;
			ulInitCode |= INIERC_MEM;
			goto EndOfFunc;
		}

		if ( pszBufp == NULL ) {
			iRtnc = GWERR_BUG;
			ulInitCode |= INIERC_BUG;
			goto EndOfFunc;
		};

		switch(iLinecnt){
		case	1:
			if ( strcmp( pszBufp, FILEVERSION_DFAAGT) != 0 ) {
				iRtnc = GWERR_ABEND;
				ulInitCode |= INIERC_INT;
				goto EndOfFunc;
			} else {
				bVersioninfo_defined = TRUE;
			}
			break;

		case	2:
			if ( *pszBufp == '\0' ) {
				ulInitCode |= (INIER_AGT_HOST | INIERC_INV);
				iRtnc = GWERR_ABEND;
				goto EndOfFunc;
			};

			pHost = NWgethostbyname( &NWsockent, pszBufp);
			if (pHost == NULL) {
				ulInitCode |= (INIER_AGT_HOST | INIERC_INV);
				iRtnc = GWERR_ABEND;
				GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11017);
				bDetail_Output = TRUE;
				goto EndOfFunc;
			};


			pAgtp = AddToGAgent_t();
			if (pAgtp == NULL){
				iRtnc = GWERR_NOMEM;
				ulInitCode |= (INIER_AGT_HOST | INIERC_MEM);
				goto EndOfFunc;
			};

			strcpy (pAgtp -> aszAgentName, pszBufp);
			bIsAgentDefined = TRUE;
			break;

		case	3:
			if ( strlen (pszBufp) != MASTERKEYLEN ) {
				iRtnc = GWERR_ABEND;
				ulInitCode |= (INIER_AGT_KEY | INIERC_INV);
				goto EndOfFunc;
			};
			bMasterkey_defined = TRUE;
			break;

		default	:
			iRtnc = GWERR_ABEND;
			ulInitCode |= INIERC_INV;
			goto EndOfFunc;

		}
		free( pszBufp);
		pszBufp = NULL;
		iLinecnt++;
	}

	if ( bVersioninfo_defined==FALSE ) {
		ulInitCode |= INIERC_INT;
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	if (pAgent_sp == NULL || bMasterkey_defined == FALSE) {
		ulInitCode |= INIERC_NOT;
		iRtnc = GWERR_ABEND;
	};

EndOfFunc:
	if (pszBufp != NULL) {
		free(pszBufp);
		pszBufp = NULL;
	};

	if (hFh != NULL) {
		fclose(hFh);
	};

	if ( iRtnc == GWERR_NOMEM ) {
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11037);
	};


	GW_tsh_functrace( FUNC_CHKAGTFILE, GW_FUNC_END, iRtnc);


	return(iRtnc);

}




/*EnglishComment**************************************************************
 *  ID      =   CheckOptionFile                                              *
 *  name    =   DFAMOPT.CFG file analysis routine                            *
 *  func    =   Analyzes DFAMOPT.CFG, and set the contents onto the memory   *
 *              as global value.                                             *
 *  i/o     =   none                                                         *
 *  return  =   GWERR_NORMAL : normal                                        *
 *              GWERR_ABEND  : abnormal (file open error, definition error)  *
 *              GWERR_NOMEM  : abnormal (insufficient memory)                *
 *              GWERR_BUG    : abnormal (internal error)                     *
 *  common  =   none                                                         *
 *  note    =   Default values are set for the undefined options.            *
 *  date    =   Nov. 25, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int CheckOptionFile(void){

	FILE 	*hFh = NULL;
	char	aszWPath[_MAX_PATH + 1];
	char 	*pszBufp = NULL;
	char	*pszRvalp = NULL;
	char	*pszWkp = NULL;
	char	*pszWkp2;
	int		iCcode;
	int		iRtnc = GWERR_NORMAL;
	int		iLinecnt;
	int		iNewerFile;
	BOOL	bLoop;


	BOOL	bVersioninfo_defined		=	FALSE;
	BOOL	bDirsyncinterval_defined	=	FALSE;
	BOOL	bCopybackdelaytime_defined	=	FALSE;
	BOOL	bSendbufferlength_defined	=	FALSE;
	BOOL	bResponsewaittime_defined	=	FALSE;
	BOOL	bWatchdogwaittime_defined	=	FALSE;
	BOOL	bMountwarning_defined		=	FALSE;
	BOOL	bTimesync_defined			=	FALSE;
	BOOL	bCopybackdirectory_defined	=	FALSE;
	BOOL	bMessagelogfile_defined		=	FALSE;
	BOOL	bErrorlogfile_defined		=	FALSE;
	BOOL	bConnecttime_defined		=	FALSE;
	BOOL	bFilesparsedelay_defined	=	FALSE;

	ulInitCode = INIER_OPTCHK;
	bDetail_Output = FALSE;


	GW_tsh_functrace( FUNC_CHKOPTFILE, GW_FUNC_START, NULL);


	hFh = fopen ( OPTFILENAME, "r");
	if (hFh == NULL){
		iRtnc = GWERR_ABEND;
		ulInitCode |= INIERC_OPN;
		goto EndOfFunc;
	};

	bLoop = TRUE;
	iLinecnt = 1;

	while(bLoop == TRUE){


		iCcode = LineRead( hFh, &pszBufp);
		if (iCcode == GWERR_EOF) {
			if ( pszBufp == NULL ) {
				break;
			} else {
				bLoop = FALSE;
			}
		} else if (iCcode == GWERR_NOMEM) {
			iRtnc = iCcode;
			ulInitCode |= INIERC_MEM;
			goto EndOfFunc;
		}

		if ( pszBufp == NULL ) {
			iRtnc = GWERR_BUG;
			ulInitCode |= INIERC_BUG;
			goto EndOfFunc;
		};

		if (iLinecnt == 1){
			if ( (strcmp( pszBufp, FILEVERSION_DFAOPT) != 0) &&
				 (strcmp( pszBufp, CFGVER_0100) != 0) ) {
				iRtnc = GWERR_ABEND;
				ulInitCode |= INIERC_INT;
				goto EndOfFunc;
			} else {
				bVersioninfo_defined = TRUE;
			}

		} else {

			if ( strnicmp( pszBufp, "DIRSYNCINTERVAL=", 16) == 0) {


				if (bDirsyncinterval_defined == TRUE) {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_SYN | INIERC_DBL);
					goto EndOfFunc;
				};

				pszRvalp = pszBufp + 16;
				iCcode = RangeCheck(pszRvalp, DIRSYNCINTERVAL_MIN, DIRSYNCINTERVAL_MAX);
				if (iCcode < 0) {
					ulInitCode |= INIER_OPT_SYN;
					iRtnc = GWERR_ABEND;
					goto EndOfFunc;
				};
				iCcode *= MIN_TO_MILISEC;
				uiDirSyncInterval = (LONG)iCcode;
				bDirsyncinterval_defined	=	TRUE;


			} else
			if ( strnicmp( pszBufp, "COPYBACKDELAYTIME=", 18) == 0) {


				if (bCopybackdelaytime_defined == TRUE) {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_CPT | INIERC_DBL);
					goto EndOfFunc;
				};

				pszRvalp = pszBufp + 18;
				iCcode = RangeCheck(pszRvalp, COPYBACKDELAYTIME_MIN, COPYBACKDELAYTIME_MAX);
				if (iCcode < 0) {
					ulInitCode |= (INIER_OPT_CPT);
					iRtnc = GWERR_ABEND;
					goto EndOfFunc;
				};
				usCopyBackDelayTime = (WORD)iCcode;
				bCopybackdelaytime_defined	=	TRUE;


			} else
			if ( strnicmp( pszBufp, "SENDBUFFERLENGTH=", 17) == 0) {


				if (bSendbufferlength_defined == TRUE) {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_SBL | INIERC_DBL);
					goto EndOfFunc;
				};

				pszRvalp = pszBufp + 17;
				iCcode = RangeCheck(pszRvalp, SENDBUFFERLENGTH_MIN, SENDBUFFERLENGTH_MAX);
				if (iCcode < 0) {
					ulInitCode |= (INIER_OPT_SBL);
					iRtnc = GWERR_ABEND;
					goto EndOfFunc;
				};
				pAgent_sp -> iCom_SendBuffLen = iCcode;
				bSendbufferlength_defined	=	TRUE;


			} else
			if ( strnicmp( pszBufp, "RESPONSEWAITTIME=", 17) == 0) {


				if (bResponsewaittime_defined == TRUE) {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_RWT | INIERC_DBL);
					goto EndOfFunc;
				};

				pszRvalp =  pszBufp + 17;
				iCcode = RangeCheck(pszRvalp, RESPONSEWAITTIME_MIN, RESPONSEWAITTIME_MAX);
				if (iCcode < 0) {
					ulInitCode |= (INIER_OPT_RWT);
					iRtnc = GWERR_ABEND;
					goto EndOfFunc;
				};
				pAgent_sp->iCom_ResponseWaitTime = iCcode;
				bResponsewaittime_defined	=	TRUE;


			} else
			if ( strnicmp( pszBufp, "WATCHDOGWAITTIME=", 17) == 0) {


				if (bWatchdogwaittime_defined == TRUE) {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_WWT | INIERC_DBL);
					goto EndOfFunc;
				};

				pszRvalp = pszBufp + 17;
				iCcode = RangeCheck(pszRvalp, WATCHDOGWAITTIME_MIN, WATCHDOGWAITTIME_MAX);
				if (iCcode < 0) {
					ulInitCode |= (INIER_OPT_WWT);
					iRtnc = GWERR_ABEND;
					goto EndOfFunc;
				};
				pAgent_sp->iWatchDogWaitTime = iCcode;
				bWatchdogwaittime_defined	=	TRUE;


			} else
			if ( strnicmp( pszBufp, "CONNECTTIME=", 12) == 0) {


				if (bConnecttime_defined == TRUE) {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_CWT | INIERC_DBL);
					goto EndOfFunc;
				};

				pszRvalp = pszBufp + 12;
				iCcode = RangeCheck(pszRvalp, CONNECTTIME_MIN, CONNECTTIME_MAX);
				if (iCcode < 0) {
					ulInitCode |= (INIER_OPT_CWT);
					iRtnc = GWERR_ABEND;
					goto EndOfFunc;
				};
				pAgent_sp->uiConnectWaitTime = (LONG)iCcode;
				bConnecttime_defined	=	TRUE;


			} else
			if ( strnicmp( pszBufp, "FILESPARSEDELAYTIME=", 20) == 0) {


				if (bFilesparsedelay_defined == TRUE) {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_FSD | INIERC_DBL);
					goto EndOfFunc;
				};

				pszRvalp = pszBufp + 20;
				iCcode = RangeCheck(pszRvalp, FILESPARSEDELAYTIME_MIN, FILESPARSEDELAYTIME_MAX);
				if (iCcode < 0) {
					ulInitCode |= (INIER_OPT_FSD);
					iRtnc = GWERR_ABEND;
					goto EndOfFunc;
				};
				iFileSparseDelayTime = iCcode;
				bFilesparsedelay_defined	=	TRUE;


			} else
			if ( strnicmp( pszBufp, "TIMESYNC=", 9) == 0) {


				if (bTimesync_defined == TRUE) {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_TS | INIERC_DBL);
					goto EndOfFunc;
				};

				pszRvalp = pszBufp + 9;
				if ( (stricmp(pszRvalp, "YES") == 0) || (stricmp(pszRvalp, "Y") == 0) ) {
					bTimeSync = TRUE;
					bTimesync_defined	=	TRUE;
				} else
				if ( (stricmp(pszRvalp, "NO") == 0) || (stricmp(pszRvalp, "N") == 0) ) {
					bTimeSync = FALSE;
					bTimesync_defined	=	TRUE;
				} else {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_TS | INIERC_INV);
					goto EndOfFunc;
				}


			} else
			if ( strnicmp( pszBufp, "MOUNTWARNING=", 13) == 0) {


				if (bMountwarning_defined == TRUE) {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_MW | INIERC_DBL);
					goto EndOfFunc;
				};

				pszRvalp = pszBufp + 13;
				if ( (stricmp(pszRvalp, "YES") == 0) || (stricmp(pszRvalp, "Y") == 0) ) {
					bMountWarning = TRUE;
					bMountwarning_defined	=	TRUE;
				} else
				if ( (stricmp(pszRvalp, "NO") == 0) || (stricmp(pszRvalp, "N") == 0) ) {
					bMountWarning = FALSE;
					bMountwarning_defined	=	TRUE;
				} else {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_MW | INIERC_INV);
					goto EndOfFunc;
				}


			} else
			if ( strnicmp( pszBufp, "COPYBACKDIRECTORY=", 18) == 0) {

				bCopybackdirectory_defined	=	TRUE;


			} else
			if ( strnicmp( pszBufp, "MESSAGELOGFILE=", 15) == 0) {


				if (bMessagelogfile_defined == TRUE) {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_MLF | INIERC_DBL);
					goto EndOfFunc;
				};

				pszRvalp = pszBufp + 15;
				pszWkp = &aszWPath[0];
				iCcode = LogFileCheck(pszRvalp, &iNewerFile, &pszWkp);

				if (iCcode != GWERR_NORMAL) {
					ulInitCode |= INIER_OPT_MLF;
					switch(iCcode) {
					case GWERR_INVAL:
						ulInitCode |= INIERC_INV;
						iRtnc = GWERR_ABEND;
						break;
					case GWERR_FAILURE:
						ulInitCode |= INIERC_OPN;
						iRtnc = GWERR_ABEND;
						break;
					case GWERR_IS_DFAMVOL:
						ulInitCode |= INIERC_GWV;
						iRtnc = GWERR_ABEND;
						break;
					default:
						ulInitCode |= INIERC_BUG;
						iRtnc = GWERR_ABEND;

					}
					goto EndOfFunc;
				};


				pszWkp2 = pszWkp;
				while( pszWkp2 != NULL ) {
					pszWkp2 = D_strchr( pszWkp2, '\/' );
					if ( pszWkp2 != NULL ) {
						*pszWkp2 = '\\';
					};
				}

				sprintf( aszMessageLogFileName1, "%s.%s", pszWkp, "001");
				sprintf( aszMessageLogFileName2, "%s.%s", pszWkp, "002");
				bMessagelogfile_defined	=	TRUE;

				if (iNewerFile == 1) {
					pszCur_Msg_Fname = &aszMessageLogFileName1[0];
				} else {
					pszCur_Msg_Fname = &aszMessageLogFileName2[0];
				}


			} else
			if ( strnicmp( pszBufp, "ERRORLOGFILE=", 13) == 0) {


				if ( bErrorlogfile_defined == TRUE ) {
					iRtnc = GWERR_ABEND;
					ulInitCode |= (INIER_OPT_ELF | INIERC_DBL);
					goto EndOfFunc;
				};

				pszRvalp = pszBufp + 13;
				pszWkp = &aszWPath[0];
				iCcode = LogFileCheck(pszRvalp, &iNewerFile, &pszWkp);

				if (iCcode != GWERR_NORMAL) {
					ulInitCode |= (INIER_OPT_ELF);
					switch(iCcode) {
					case GWERR_INVAL:
						iRtnc = GWERR_ABEND;
						ulInitCode |= INIERC_INV;
						break;
					case GWERR_FAILURE:
						ulInitCode |= INIERC_OPN;
						iRtnc = GWERR_ABEND;
						break;
					case GWERR_IS_DFAMVOL:
						ulInitCode |= INIERC_GWV;
						iRtnc = GWERR_ABEND;
						break;
					default:
						ulInitCode |= INIERC_BUG;
						iRtnc = GWERR_ABEND;

					}
					goto EndOfFunc;
				};


				pszWkp2 = pszWkp;
				while( pszWkp2 != NULL ) {
					pszWkp2 = D_strchr( pszWkp2, '\/' );
					if ( pszWkp2 != NULL ) {
						*pszWkp2 = '\\';
					};
				}

				sprintf( aszErrorLogFileName1, "%s.%s", pszWkp, "001");
				sprintf( aszErrorLogFileName2, "%s.%s", pszWkp, "002");
				bErrorlogfile_defined	=	TRUE;

				if (iNewerFile == 1) {
					pszCur_Err_Fname = &aszErrorLogFileName1[0];
				} else {
					pszCur_Err_Fname = &aszErrorLogFileName2[0];
				}


			} else {
				ulInitCode |= (INIERC_NDF);
				iRtnc = GWERR_ABEND;
				goto EndOfFunc;
			}
		}

		free(pszBufp);
		pszBufp = NULL;
		iLinecnt++;
	}

EndOfFunc:

	if ( pszBufp != NULL ) {
		free(pszBufp);
		pszBufp = NULL;
	};

	if( hFh != NULL ) {
		fclose(hFh);
	};

	if ( iRtnc == GWERR_NOMEM ) {
		GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11037);
	};

	if ( (iRtnc==GWERR_NORMAL) && (bVersioninfo_defined==FALSE) ) {
		ulInitCode |= (INIERC_INT);
		iRtnc = GWERR_ABEND;
	};

	if (iRtnc == 0){


		if ( bDirsyncinterval_defined	==	FALSE ) {
			uiDirSyncInterval = DIRSYNCINTERVAL_DFL;
		};
		if ( bCopybackdelaytime_defined == FALSE ) {
			usCopyBackDelayTime = COPYBACKDELAYTIME_DFL;
		};

		if ( bSendbufferlength_defined	==	FALSE ) {
			pAgent_sp->iCom_SendBuffLen = SENDBUFFERLENGTH_DFL;
		};

		if ( bResponsewaittime_defined	==	FALSE ) {
			pAgent_sp->iCom_ResponseWaitTime = RESPONSEWAITTIME_DFL;
		};

		if ( bWatchdogwaittime_defined == FALSE ) {
			pAgent_sp->iWatchDogWaitTime = WATCHDOGWAITTIME_DFL;
		};

		if ( bConnecttime_defined == FALSE ) {
			pAgent_sp->uiConnectWaitTime = CONNECTTIME_DFL;
		};

		if ( bFilesparsedelay_defined == FALSE ) {
			iFileSparseDelayTime = FILESPARSEDELAYTIME_DFL;
		};


		if ( bMessagelogfile_defined == FALSE ) {
			pszWkp = &aszWPath[0];
			if ( LogFileCheck ( "SYS:\\DFAERM", &iNewerFile, &pszWkp ) == GWERR_NORMAL ) {
				strcpy ( aszMessageLogFileName1, MESSAGELOGFILE1_DFL );
				strcpy ( aszMessageLogFileName2, MESSAGELOGFILE2_DFL );

				if (iNewerFile == 1) {
					pszCur_Msg_Fname = &aszMessageLogFileName1[0];
				} else {
					pszCur_Msg_Fname = &aszMessageLogFileName2[0];
				}

			} else {
				ulInitCode |= INIER_OPT_MLF | INIERC_OPN;
				iRtnc = GWERR_ABEND;
			}
		}


		if ( (iRtnc == 0) &&  (bErrorlogfile_defined == FALSE) ) {
			pszWkp = &aszWPath[0];
			if ( LogFileCheck ( "SYS:\\DFAERL", &iNewerFile, &pszWkp ) == GWERR_NORMAL ) {
				strcpy ( aszErrorLogFileName1 , ERRORLOGFILE1_DFL );
				strcpy ( aszErrorLogFileName2 , ERRORLOGFILE2_DFL );

				if (iNewerFile == 1) {
					pszCur_Err_Fname = &aszErrorLogFileName1[0];
				} else {
					pszCur_Err_Fname = &aszErrorLogFileName2[0];
				}

			} else {
				ulInitCode |= INIER_OPT_ELF | INIERC_OPN;
				iRtnc = GWERR_ABEND;
			}
		}

	};


	if (iRtnc == 0) {
		strcpy( pGErrFile_p->aszErrLog001, aszErrorLogFileName1 );
		strcpy( pGErrFile_p->aszErrLog002, aszErrorLogFileName2 );
		strcpy( pGErrFile_p->aszMsgLog001, aszMessageLogFileName1 );
		strcpy( pGErrFile_p->aszMsgLog002, aszMessageLogFileName2 );
		strcpy( pGErrFile_p->aszFuncTrace, TRCFNAME_FUNC );
		strcpy( pGErrFile_p->aszMainTrace, TRCFNAME_MAIN );
		strcpy( pGErrFile_p->aszAdmnTrace, TRCFNAME_ADMIN );
		strcpy( pGErrFile_p->aszWDogTrace, TRCFNAME_WATCH );
	};


	GW_tsh_functrace( FUNC_CHKOPTFILE, GW_FUNC_END, iRtnc);

	return(iRtnc);
}




/*EnglishComment**************************************************************
 *  ID      =   RangeCheck                                                   *
 *  name    =   Range check routine                                          *
 *  func    =   Converts the character strings into int, and checks          *
 *              the range.                                                   *
 *  i/o     =   pszString   :   char *  :i: target character strings         *
 *              iMin        :   int     :i: minimal value                    *
 *              iMax        :   int     :i: maximum value                    *
 *  return  = none 0 integer: normal                                         *
 *            GWERR_ABEND   : abnormal (not numeric. Or no characters exist) *
 *            GWERR_EXEED   : abnormal (exceeded the maximum value)          *
 *            GWERR_LESS    : abnormal (is less than the minumal value)      *
 *  common  =   none                                                         *
 *  note    =   accepts only integers.                                       *
 *  date    =   Dec. 01, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int RangeCheck( pszString, iMin, iMax)
char	*pszString;
int	iMin;
int	iMax;
{
	int	iCcode;
	int	iRtnc = GWERR_NORMAL;
	int	iConv_value;
	char	aszW_text[11];
	char	*pszP = pszString;


	GW_tsh_functrace( FUNC_RANGECHECK, GW_FUNC_START, NULL);

	iCcode = isdigits(pszString);
	if (iCcode <= 0) {
		if( iCcode == GWERR_NOTNUM ) {
			ulInitCode |= INIERC_NNM;
			iRtnc = GWERR_ABEND;
			goto EndOfFunc;
		} else {
			ulInitCode |= INIERC_INV;
			iRtnc = GWERR_ABEND;
			goto EndOfFunc;
		};
	};
	itoa( iMax, aszW_text, 10);

	while (*pszP == '0'){
		pszP++;
	}

	if (*pszP == '\0'){
		pszP--;
	};

	if( strlen(aszW_text) < strlen(pszP) ){

		iRtnc = GWERR_EXCEED;
		ulInitCode |= INIERC_EXC;
		goto EndOfFunc;
	};

	if ( (strlen(aszW_text) == strlen(pszP)) && (strcmp(aszW_text, pszP) < 0) ) {

		iRtnc = GWERR_EXCEED;
		ulInitCode |= INIERC_EXC;
		goto EndOfFunc;
	};

	iConv_value = atoi(pszP);

	if (iConv_value < iMin) {

		iRtnc = GWERR_LESS;
		ulInitCode |= INIERC_LES;
		goto EndOfFunc;
	} else;

EndOfFunc:

	if ( iRtnc == GWERR_NORMAL ) {
		iRtnc = iConv_value;
	};


	GW_tsh_functrace( FUNC_RANGECHECK, GW_FUNC_END, iRtnc);

	return(iRtnc);
}




/*EnglishComment**************************************************************
 *  ID      =   isdigits                                                     *
 *  name    =   string numeric check routine                                 *
 *  func    =   Checks if the string is numeric.                             *
 *  i/o     =   pszString   :   char *  :i: the target string                *
 *  return  =   interger    : normal (returns the number of figures)         *
 *              GWERR_NOTNUM: abnormal (not numeric)                         *
 *              GWERR_BUG   : abnormal (null character string)               *
 *  common  =   none                                                         *
 *  note    =   1) accepts only integers.                                    *
 *              2) makes an abnormal return (GWERR_BUG) if a null character  *
 *                 string is inputed.                                        *
 *  date    =   Dec. 01, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int isdigits(pszString)
char *pszString;
{
	int iCnt;
	int iRtnc = GWERR_NORMAL;


	GW_tsh_functrace( FUNC_ISDIGITS, GW_FUNC_START, NULL);

	for (iCnt=0; pszString[iCnt] != '\0'; iCnt++){
		if ( isdigit(pszString[iCnt]) == 0) {
			iRtnc = GWERR_NOTNUM;
			goto EndOfFunc;
		} else;
	}


	if (iCnt == 0) {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};

EndOfFunc:

	if (iRtnc == GWERR_NORMAL) {
		iRtnc = iCnt;
	};


	GW_tsh_functrace( FUNC_ISDIGITS, GW_FUNC_END, iRtnc);

	return(iRtnc);
}




/*EnglishComment**************************************************************
 *  ID      =   vol_exist_chk                                                *
 *  name    =   NetWare volume check routine                                 *
 *  func    =   Checks if the volume name in pszVolp is valid.               *
 *  i/o     =   pszVolname  :   char *  :i: the target character string      *
 *  return  =   0 : normal                                                   *
 *             -1 : abnormal (not a NetWare volume)                          *
 *             -2 : abnormal (unexpected error)                              *
 *  common  =   none                                                         *
 *  note    =   1) The volume name may have ":"                              *
 *              2) It should have only a volume name.                        *
 *              3) The volume should exist and be mounted.                   *
 *                                                                           *
 *  date    =   Dec. 06, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int vol_exist_chk(pszVolname)
char *pszVolname;
{
	FILE_SERV_INFO	servinfo;
	int		iCcode;
	int		iRtnc = GWERR_NORMAL;
	int		iVolnum;
	int		iLen;
	int		iWorklength;
	WORD	usServID;
	VOLUME_INFO	vi;
	char	aszW_volname[_MAX_VOLUME+1];
	char	aszW_servname[_MAX_SERVER+1];
	char	*pszWorkp;
	char	*pszWorkp2;
	struct GVolume_t *pWVolp;


	GW_tsh_functrace( FUNC_VOLEXISTCHK, GW_FUNC_START, NULL);


	if ( D_strpbrk (pszVolname, "\\\/") != NULL ) {
		iWorklength = D_strcspn(pszVolname, "\\\/");
		if (iWorklength > _MAX_SERVER) {
			iRtnc = GWERR_ABEND;
			goto EndOfFunc;
		};

		strncpy(aszW_servname, pszVolname, iWorklength);
		aszW_servname[iWorklength] = '\0';
		pszWorkp = pszVolname + iWorklength + 1;

		if ( stricmp ( aszMyServerName, aszW_servname ) != 0 ) {
			iRtnc = GWERR_ABEND;
			goto EndOfFunc;
		};
	} else {
		pszWorkp = pszVolname;
	}

	iLen = strlen(pszWorkp);

	if (	(iLen > 16)	||
			(iLen < 2) 	||
			((iLen == 16) && (pszWorkp[15] != ':')) ||
			((iLen == 2)  && (pszWorkp[1] == ':')) ) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	strcpy(aszW_volname, pszWorkp);


	pszWorkp2 = D_strchr(aszW_volname, ':');
	if(pszWorkp2){
		if ( *(pszWorkp2 + 1) != '\0' ) {
			iRtnc = GWERR_ABEND;
			goto EndOfFunc;
		};
		*pszWorkp2 = '\0';
	};


	if ( !stricmp ( aszW_volname, VOL_SYS ) ) {
		iRtnc = GWERR_ISSYS;
		goto EndOfFunc;
	};


	if ( pVolume_sp != NULL ) {
		pWVolp = pVolume_sp;
		while(1) {
			if ( isDFAMVolume( (BYTE *)aszW_volname, NULL) == YES) {
				iRtnc = GWERR_DOUBLEDEF;
				goto EndOfFunc;
			};
			if ( pWVolp->pUshirop != NULL ) {
				pWVolp = pWVolp->pUshirop;
			} else {
				break;
			}
		}
	};


	iCcode = GetServerInformation( SERV_INFO_SIZE , &servinfo);
	if (iCcode != ESUCCESS) {
		GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11039, NetWareErrno);
		iRtnc = GWERR_FAILURE;
		goto EndOfFunc;
	};

	iCcode = GetFileServerID( servinfo.serverName,	&usServID);
	if (iCcode == UNKNOWN_FILE_SERVER){
		GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11040, NetWareErrno);
		iRtnc = GWERR_FAILURE;
		goto EndOfFunc;
	};


	iCcode = GetVolumeNumber( aszW_volname, &iVolnum);
	if (iCcode == ERR_VOLUME_DOES_NOT_EXIST){
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	iCcode = GetVolumeStatistics( usServID, (BYTE)iVolnum, sizeof(VOLUME_INFO), &vi);
	if (iCcode == ESUCCESS){
		if ( vi.isMounted == 0){
			iRtnc = GWERR_ABEND;
			goto EndOfFunc;
		} else;
	} else if (iCcode == ERR_INVALID_VOLUME){
		iRtnc =GWERR_ABEND;
		goto EndOfFunc;
	} else {
		GW_Message( DFAM_CON, NULL, LOG_YES, KDDS11041, NetWareErrno);
		iRtnc = GWERR_FAILURE;
		goto EndOfFunc;
	}

EndOfFunc:


	GW_tsh_functrace( FUNC_VOLEXISTCHK, GW_FUNC_END, iRtnc);

	return(iRtnc);
}




/*EnglishComment**************************************************************
 *  ID      =   isDFSPath                                                    *
 *  name    =   DFS path name check routine                                  *
 *  func    =   Checks if the volume name in pszDFSp is valid.               *
 *                i) checks if the format is /.../xxx..xxxx/fs/xxx...xxx...  *
 *               ii) checks if the length does not exceed 1022               *
 *  i/o     =   pszDFSp :   char *  :i: the target character string          *
 *  return  =   GWERR_NORMAL :  normal                                       *
 *              GWERR_ABEND  :  abnormal (invalid path name)                 *
 *              GWERR_BUG    :  abnormal (internal error)                    *
 *  note    =   1) This is a preliminary check (does not attempt an actual   *
 *                 access to DFS.                                            *
 *              2) The maximum length (1022) is checked to avoid the         *
 *                 situation that the name is too long to make an entry      *
 *                 under the mount point.
 *                                                                           *
 *  date    =   Dec. 07, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int	isDFSPath(pszDFSp)
BYTE *pszDFSp;
{

	char	*pszWorkp;
	int		iLen;
	int		iCnt;
	int		iRtnc = GWERR_NORMAL;


	GW_tsh_functrace( FUNC_ISDFSPATH, GW_FUNC_START, NULL);


	if (pszDFSp == NULL || *pszDFSp == '\0') {
		iRtnc = GWERR_BUG;
		goto EndOfFunc;
	};

	iLen = strlen(pszDFSp);


	if ( 	( iLen > (MAP_DFS_PATH_MAX+1) ) ||
			((iLen == MAP_DFS_PATH_MAX+1) && (pszDFSp[iLen-1] != '\/')) ) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	pszWorkp = strstr( pszDFSp, "\/...\/");
	if ( pszWorkp != pszDFSp ) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};
	pszWorkp += 5;


	iCnt = 0;
	while ( (*pszWorkp != '\0') && (*pszWorkp != '\/') ) {
		pszWorkp++;
		iCnt++;
	}


	if ( (iCnt == 0) || (iCnt > DFSENTRY_MAX) ) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	if ( strncmp( pszWorkp, "/fs", 3) != 0 ) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};

	pszWorkp+=3;

	if ( *pszWorkp != '\0' && *pszWorkp != '\/' ){
		iRtnc = GWERR_ABEND;
	};

EndOfFunc:


	GW_tsh_functrace( FUNC_ISDFSPATH, GW_FUNC_END, iRtnc);

	return(iRtnc);
}




/*EnglishComment**************************************************************
 *  ID      =   LogFileCheck                                                 *
 *  name    =   log file check routine for initialization routine            *
 *  func    =   1) Checks "MESSAGELOGFILE" and "ERRORLOGFILE" in the         *
 *                 DFAMOPT.CFG file.                                         *
 *              2) Checks if the directory exists (error if not).            *
 *  i/o     =   pszBufp :   char *  :i: The target character string          *
 *              piWhichNew  int  *  :o: which is new 1 or 2                  *
 *              ppszRtnbuf  char ** :o: Return FileName Buffer               *
 *  return  =   GWERR_NORMAL   : normal                                      *
 *              GWERR_INVAL    : abnormal (invalid path)                     *
 *              GWERR_FAILURE  : abnormal (open error)                       *
 *              GWERR_ISDFAMVOL: abnormal (DFAMVOLUME is designated)         *
 *  note    =  1) makes an error return if the log file exists in the volume *
 *                defined as GATEWAY Volume.                                 *
 *             2) makes an error return if the file cannot be opened.        *
 *             3) as a current file, a newer file of 2 files is returned.    *
 *  date    =   Dec. 08, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/

int	LogFileCheck( pszBufp, piWhichisNew, ppszRtnbuf)
char	*pszBufp;
int		*piWhichisNew;
char	**ppszRtnbuf;
{
	char aszVol[_MAX_SERVER + _MAX_VOLUME + 1];
	char aszWServName[_MAX_SERVER + 1];
	char aszPath[_MAX_PATH + 1];
	char aszFname[_MAX_FNAME + 1];
	char aszExt[_MAX_EXT + 1];
	char aszTestfname[_MAX_PATH+1];
	time_t	uiDate1;
	time_t	uiDate2;
	int iCcode;
	int iRtnc = GWERR_NORMAL;
	int iWorklength;


	GW_tsh_functrace( FUNC_LOGFILECHECK, GW_FUNC_START, NULL);

	D_strupr(pszBufp);


	_splitpath(pszBufp, aszVol, aszPath, aszFname, aszExt);



	if ( aszVol[0] == '\0') {
			iRtnc = GWERR_INVAL;
			goto EndOfFunc;
	};


	if ( D_strpbrk (aszVol, "\\\/") != 0 ) {
		iWorklength = D_strcspn(aszVol, "\\\/");
		if (iWorklength > _MAX_SERVER) {
			iRtnc = GWERR_INVAL;
			goto EndOfFunc;
		};
		strncpy(aszWServName, aszVol, iWorklength);
		aszWServName[iWorklength] = '\0';


		if ( (stricmp( aszWServName, aszMyServerName) != 0 ) ) {
			iRtnc = GWERR_INVAL;
			goto EndOfFunc;
		};

		strcpy ( aszVol,  (&aszVol[0] + iWorklength + 1) );
	};


	if ( ( aszVol[0] == '\0') || ( aszVol[0] == ':') ) {
		iRtnc = GWERR_INVAL;
		goto EndOfFunc;
	};


	if ( isDFAMVolume(aszVol, NULL) == YES ) {
		iRtnc = GWERR_IS_DFAMVOL;
		goto EndOfFunc;
	};


	if (aszExt[0] != '\0') {
		iRtnc = GWERR_INVAL;
		goto EndOfFunc;
	};


	if (strlen(pszBufp) > (_MAX_PATH - 4) ){
		iRtnc = GWERR_INVAL;
		goto EndOfFunc;
	};



	sprintf( aszTestfname, "%s.001", pszBufp);
	iCcode = isFileAbleToOpen(aszTestfname, &uiDate1);
	if ( uiDate1 != 0 )
		remove( aszTestfname );

	if (iCcode != 0) {
		iRtnc = GWERR_FAILURE;
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11018, aszTestfname);
		bDetail_Output = TRUE;
		goto EndOfFunc;
	};

	sprintf( aszTestfname, "%s.002", pszBufp);
	iCcode = isFileAbleToOpen(aszTestfname, &uiDate2);
	if ( uiDate2 != 0 )
		remove( aszTestfname );

	if (iCcode != 0) {
		iRtnc = GWERR_FAILURE;
		GW_Message ( DFAM_CON, NULL, LOG_NO, KDDS11018, aszTestfname);
		bDetail_Output = TRUE;
		goto EndOfFunc;
	}


	*piWhichisNew = 1;

EndOfFunc:

	if (iRtnc == GWERR_NORMAL) {
		sprintf (*ppszRtnbuf, "%s%s%s",aszVol, aszPath, aszFname);
		D_strupr(*ppszRtnbuf);
	} else {
		*ppszRtnbuf = NULL;
	}


	GW_tsh_functrace( FUNC_LOGFILECHECK, GW_FUNC_END, iRtnc);

	return(iRtnc);
}




/*EnglishComment**************************************************************
 *  ID      =   isFileAbleToOpen                                             *
 *  name    =   file open check routine                                      *
 *  func    =   checks if a file can be made.                                *
 *               1) makes a normal return if the file already exists.        *
 *               2) attempts to make a file if the file does not exist.      *
 *                   i) makes an error return if the file creation failed.   *
 *                  ii) if the file is successfully made, remove the file    *
 *                      and make a normal return.                            *
 *  i/o     =   pszTestFname:   char *  :i: the target character string      *
 *  return  =   GWERR_NORMAL :  normal                                       *
 *              GWERR_ABEND  :  abnormal (invalid path name)                 *
 *  note    =   none                                                         *
 *                                                                           *
 *  date    =   Dec. 08, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/


int isFileAbleToOpen(pszTestFname, uiLastModDate)
char *pszTestFname;
time_t	*uiLastModDate;
{
	int	iHandle;
	int	iRtnc = GWERR_NORMAL;
	struct stat buf;


	GW_tsh_functrace( FUNC_FILEABLEOPEN, GW_FUNC_START, NULL);

	*uiLastModDate = (time_t)0;

	iHandle = open( pszTestFname, O_RDWR, S_IWRITE | S_IREAD);
	if (iHandle == -1) {
		iHandle = open(pszTestFname, O_WRONLY | O_CREAT, S_IWRITE | S_IREAD);
		if (iHandle == -1) {
			iRtnc = GWERR_FAILURE;
			goto EndOfFunc;
		};
		close(iHandle);
		remove(pszTestFname);
		goto EndOfFunc;
	} else {

		fstat(iHandle, &buf);
		*uiLastModDate = buf.st_mtime;
		close(iHandle);
	}

EndOfFunc:


	GW_tsh_functrace( FUNC_FILEABLEOPEN, GW_FUNC_END, iRtnc);

	return(iRtnc);
}




/*EnglishComment**************************************************************
 *  ID      =   LineRead                                                     *
 *  name    =   get the contents defined in the file to the buffer           *
 *  func    =   1) reads the contents of the line (128 bytes at a time) onto *
 *                 the memory.                                               *
 *              2) realloc memory each time memory runs short.               *
 *  i/o     =   hFh      : FILE *  :i: the file pointer of the target file   *
 *              ppszBuf  : char ** :o: line buffer                           *
 *  return  =   GWERR_NORMAL    :   normal                                   *
 *              GWERR_EOF       :   end-of-file is detected                  *
 *              GWERR_NOMEM     :   shortage of memory                       *
 *  note    =   1) the allocated memory is freed after the usage.            *
 *              2) GW_Malloc is not used for realloc.                        *
 *  date    =   Dec. 01, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
int LineRead ( hFh, ppszBuf )
FILE *hFh;
char **ppszBuf;
{
	char	aszWbuf[LINEBUFLEN + 1];
	char	*pszGetbuf = NULL;
	char	*pszNewbuf = NULL;
	int		iLen;
	int		iMemcnt;
	int		iRtnc = GWERR_NORMAL;


	GW_tsh_functrace( FUNC_LINEREAD, GW_FUNC_START, NULL);

	*ppszBuf = NULL;

	pszGetbuf = (char *)malloc( LINEBUFLEN + 1);
	if ( !pszGetbuf ) {
		iRtnc = GWERR_NOMEM;
		goto EndOfFunc;
	};

	*pszGetbuf = '\0';
	iMemcnt = 1;

	while (1) {
		aszWbuf[0] = '\0';
		if ( fgets(aszWbuf, LINEBUFLEN+1 , hFh) == NULL) {
			iRtnc = GWERR_EOF;
			if (aszWbuf[0] == '\0') {
				if (*pszGetbuf == '\0'){
					free(pszGetbuf);
					pszGetbuf = NULL;
					break;
				};
			};
		};

		strcat( pszGetbuf, aszWbuf );

		if (iRtnc == GWERR_EOF) {
			break;
		}

		iLen = strlen( aszWbuf );
		if ( aszWbuf[iLen-1] == '\n' ) {
			*( strrchr( pszGetbuf, '\n' ) ) = '\0';
			break;
		} ;


		iMemcnt++;
		pszNewbuf = (char *)realloc ( pszGetbuf, ( LINEBUFLEN * iMemcnt + 1 ) );
		if (pszNewbuf == NULL) {
			free(pszGetbuf);
			pszGetbuf = NULL;
			iRtnc = GWERR_NOMEM;
			break;
		};

		pszGetbuf = pszNewbuf;
		pszNewbuf = NULL;
	}

EndOfFunc:
	*ppszBuf = pszGetbuf;


	GW_tsh_functrace( FUNC_LINEREAD, GW_FUNC_END, iRtnc);

	return(iRtnc);
}


/*EnglishComment**************************************************************
 *  ID      =   CopyBackDirCheck                                             *
 *  name    =   analyze the contents of COPYBACKDIRECTORY                    *
 *  func    =   In the checking of COPYBACKDIRECTORY, error cases are        *
 *              as follows:                                                  *
 *                1) no volume name                                          *
 *                2) the length of the server name and volume name is too    *
 *                   long.                                                   *
 *                3) the designated server name is not a current server.     *
 *                4) the volume name coinsides with GatewayVolume.           *
 *                5) the path is not a directory, or the path does not exist.*
 *              If satisfied all the checks, store the name to the global,   *
 *              and make a normal return.                                    *
 *  i/o     =   pszPath : BYTE * :i: the pointer of the target path          *
 *  return  =   GWERR_NORMAL    :   normal                                   *
 *              GWERR_ABEND     :   end-of-file is detected                  *
 *  note    =   1) the allocated memory is freed after used.                 *
 *              2) GW_Malloc is not used for realloc.                        *
 *  date    =   Dec. 01, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/


int CopyBackDirCheck( pszPath )
BYTE *pszPath;
{
	int iRtnc = GWERR_NORMAL;
	int iWorklength;
	char *pszWkp;
	char aszWVolName[_MAX_SERVER+_MAX_VOLUME+1];
	char aszWServName[_MAX_SERVER+1];
	struct stat Stattbl;


	GW_tsh_functrace( FUNC_COPYBACKDIRCHECK, GW_FUNC_START, NULL);


	_splitpath( pszPath, aszWVolName, NULL, NULL, NULL );


	if ( aszWVolName[0] == '\0') {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	if ( D_strpbrk (aszWVolName, "\\\/") != NULL ) {
		iWorklength = D_strcspn(aszWVolName, "\\\/");
		if ( iWorklength > _MAX_SERVER ) {
			iRtnc = GWERR_ABEND;
			goto EndOfFunc;
		};

		strncpy(aszWServName, aszWVolName, iWorklength);
		aszWServName[iWorklength] = '\0';


		if ( (stricmp( aszWServName, aszMyServerName) != 0 ) ) {
			iRtnc = GWERR_ABEND;
			goto EndOfFunc;
		};
		pszPath += iWorklength + 1;
		strcpy( aszWVolName, (&aszWVolName[0] + iWorklength + 1) );
	};

	iWorklength = strlen(aszWVolName);

	if (	(iWorklength > 16)	||
		(iWorklength < 2) 	||
		((iWorklength == 16) && (aszWVolName[15] != ':')) ||
		((iWorklength == 2)  && (aszWVolName[1] == ':')) ) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	if ( isDFAMVolume(pszPath, NULL) == YES ) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};

	if ( stat( pszPath, &Stattbl) != NULL ){
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	if ( (Stattbl.st_mode & S_IFDIR) != S_IFDIR ) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	if ( D_strpbrk ( pszPath, CHAR_WILDCARD ) != NULL ){
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	iWorklength = strlen(pszPath);
	pszWkp = pszPath + iWorklength - 1;
	if (*pszWkp == '\\') {
		if ( pszWkp == D_strrchr(pszPath, '\\') ){
			*pszWkp = '\0';
			iWorklength--;
		};
	} else if (*pszWkp == '\/') {
		*pszWkp = '\0';
		iWorklength--;
	};

	if ( strlen(pszPath) > COPYBACKDIRLEN_MAX ) {
		iRtnc = GWERR_ABEND;
		goto EndOfFunc;
	};


	pszWkp = pszPath;
	while( pszWkp != NULL ) {
		pszWkp = D_strchr( pszWkp, '\/' );
		if ( pszWkp != NULL ) {
			*pszWkp = '\\';
		};
	}

	strcpy (aszCopyBackDirName, D_strupr(pszPath));

EndOfFunc:

	if ( iRtnc != GWERR_NORMAL ) {
		ulInitCode |= (INIER_OPT_CBD | INIERC_INV);
		if ( strlen (pszPath) > GWMSG_STR_MAXSIZE ) {
			StrCut( pszPath, GWMSG_STR_MAXSIZE );
		};
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11019, pszPath);
		bDetail_Output = TRUE;
	};


	GW_tsh_functrace( FUNC_COPYBACKDIRCHECK, GW_FUNC_END, iRtnc);

	return(iRtnc);

}

