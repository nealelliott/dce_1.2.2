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
 * $Log: gridcmn2.c,v $
 * Revision 1.1.2.3  1996/03/22  06:27:26  takano
 * 	Fixed the confliction between the DLOGOUT command and the Copyback thread.
 * 	[1996/03/22  06:27:02  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:08:43  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:56  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  08:09:00  takano
 * 	First Submit
 * 	[1996/02/21  08:08:55  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <nwtypes.h>
#include <nwdir.h>
#include <errno.h>
#include <niterror.h>
#include <advanced.h>
#include <malloc.h>
#include <fshooks.h>
#include <signal.h>
#include <fcntl.h>
#include <process.h>
#include <nwsemaph.h>
#include <fileengd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utime.h>
#include <ctype.h>
#include <nwbindry.h>
#include <nwconn.h>
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dceapi.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "cmnprt.h"

/*EnglishComment**************************************************************/
/* ID   = MapNWFullpath                                                      */
/* name = Mapping the NetWare full path name                                 */
/* func = Conversion into the NetWare full path name.                        */
/*                                                                           */
/* i/o  = uiVolnum        :LONG  :inp: volume member                         */
/*        uiDirBase       :LONG  :inp: directory number                      */
/*        uiPathCompCnt   :LONG  :inp: the number of path hierarchical levels*/
/*        psPathString    :BYTE *:inp: NetWare-style path name               */
/*        pszNWFullpath   :BYTE *:out: NetWare full path name                */
/*		  pszNWPath       :BYTE *:out: NetWare-style path name(out of file   */
/*                                                                    name)  */
/*        piWildCard_flg  :int  *:out: pointer to wildcard character flag    */
/*                                                                           */
/* return = 0     : normal                                                   */
/*          others: abnormal (NetWare errno)                                 */
/*             pszNWFullpath   : NetWare full path name is set if successful */
/*             pszExceptCWDpath: the path name other than CWD (current       */
/*                               working directory) is set if successful.    */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

int		MapNWFullpath(	LONG 	uiVolnum,
						LONG 	uiDirBase,
						LONG 	uiPathCompCnt,
						BYTE 	*psPathString,
						BYTE 	*pszNWFullpath,
						BYTE	*pszNWPath,
						int		*piWildCard_flg)
{
	long	iCcode				= 0;
	long	iPc_w				= 0;
	LONG	iPc_CWD				= 0;
	long	iPsp				= 0;
	long	tmPathelm_w			= 0;
	long	iNamep				= 0;
	long	tmNWPath			= 0;
	BYTE	*pszVolName			= NULL;
	BYTE	*psPs_CWD			= NULL;

	pszVolName = (BYTE *) GW_malloc(_MAX_VOLUME+2);
	if (pszVolName == (BYTE *) NULL)
		return ERR_SERVER_OUT_OF_MEMORY;

	psPs_CWD = (BYTE *) GW_malloc(_MAX_PATH+1);
	if (psPs_CWD == (BYTE *) NULL) {
		GW_free(pszVolName,NULL);
		return ERR_SERVER_OUT_OF_MEMORY;
	}
	memset(pszVolName,NULL,_MAX_VOLUME+2);
	memset(psPs_CWD,NULL,_MAX_PATH+1);


	iCcode 	= GetVolumeName( (int)uiVolnum, pszVolName);
	if ( iCcode != 0 ) {
		GW_free(pszVolName,NULL);
		GW_free(psPs_CWD,NULL);
		return iCcode;
	}
	strcpy((char *) pszNWFullpath, (const char *) pszVolName);
	iNamep = strlen((const char *) pszVolName);


	iCcode = FEMapVolumeAndDirectoryToPath( (int) uiVolnum,
											 uiDirBase,
											 (BYTE *) psPs_CWD,
											 (LONG *) &iPc_CWD );
	if ( iCcode != 0 ) {
		GW_free(pszVolName,NULL);
		GW_free(psPs_CWD,NULL);
		return iCcode;
	}

	pszNWFullpath[iNamep++] = ':';
	iPc_w = (long) iPc_CWD;
	iPsp = 0;
	if ( psPs_CWD[0] ) {
		while( iPc_w-- ) {
			pszNWFullpath[iNamep++] = '\\';
			tmPathelm_w = psPs_CWD[iPsp++];
			while(tmPathelm_w--) {
				if ( piWildCard_flg != NULL ) {
					switch ( IsWildChar(&psPs_CWD[iPsp]) ) {
						case 0:
							pszNWFullpath[iNamep++] = psPs_CWD[iPsp++];
							break;
						case 3:
							pszNWFullpath[iNamep++] = '.';
							iPsp += 2;
							tmPathelm_w--;
							break;
						case 1:
							pszNWFullpath[iNamep++] = '?';
							iPsp += 2;
							tmPathelm_w--;
							*piWildCard_flg = ON;
							break;
						default:
							pszNWFullpath[iNamep++] = '*';
							iPsp += 2;
							tmPathelm_w--;
							*piWildCard_flg = ON;
							break;
					}
				}
				else
					pszNWFullpath[iNamep++] = psPs_CWD[iPsp++];
			}
		}
	}



	iPc_w = uiPathCompCnt;
	iPsp = 0;
	if ( psPathString[0] ) {
		while( iPc_w-- ) {
			pszNWFullpath[iNamep++] = '\\';
			if ( ( iPc_w == 0 ) &&
				 ( pszNWPath != (BYTE *) NULL ) ) {
				tmNWPath = iNamep;
			}
			tmPathelm_w = psPathString[iPsp++];
			while(tmPathelm_w--) {
				if ( piWildCard_flg != NULL ) {
					switch ( IsWildChar(&psPathString[iPsp]) ) {
						case 0:
							pszNWFullpath[iNamep++] = psPathString[iPsp++];
							break;
						case 3:
							pszNWFullpath[iNamep++] = '.';
							iPsp += 2;
							tmPathelm_w--;
							break;
						case 1:
							pszNWFullpath[iNamep++] = '?';
							iPsp += 2;
							tmPathelm_w--;
							*piWildCard_flg = ON;
							break;
						default:
							pszNWFullpath[iNamep++] = '*';
							iPsp += 2;
							tmPathelm_w--;
							*piWildCard_flg = ON;
							break;
					}
				} else
					pszNWFullpath[iNamep++] = psPathString[iPsp++];
			}
		}
	}
	pszNWFullpath[iNamep] = '\0';



	if ( pszNWPath != (BYTE *) NULL && tmNWPath > 0 ) {
		strncpy( pszNWPath,pszNWFullpath,tmNWPath );
		pszNWPath[tmNWPath] = '\0';
	}

	if (pszVolName != (BYTE *) NULL)
		GW_free(pszVolName,NULL);

	if (psPs_CWD != (BYTE *) NULL)
		GW_free(psPs_CWD,NULL);

	return iCcode;
}

/*EnglishComment**************************************************************/
/* ID   = IsWildChar                                                         */
/* name = Checking a wildcard character                                      */
/* func = Checks if the characters are a path name of a hook routine, and    */
/*        contain wildcard characters ('?'=0xffbf, '*'=0xffaa, '.'=0xffae).  */
/*                                                                           */
/* i/o  = pChar: BYTE *:inp: a pointer to the characters                     */
/*                                                                           */
/* return = 0: regular characters                                            */
/*          1: wildcard character (?)                                        */
/*          2: wildcard character (*)                                        */
/*          3: wildcard character (.)                                        */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

int	IsWildChar(BYTE *pChar)
{
	if (*pChar == 0xff) {
		if (*(pChar+1) == 0xbf)
			return 1;
		if (*(pChar+1) == 0xaa)
			return 2;
		if (*(pChar+1) == 0xae)
			return 3;
	}
	return 0;
}

/*EnglishComment**************************************************************/
/* ID   = FileCopyFromDFS                                                    */
/* name = Copying DFS file                                                   */
/* func = Copies a DFS file to a NetWare dummy file.                         */
/* i/o  = pszNWFullpath :BYTE *:inp: NetWare full path name                  */
/*        pszDFSFullpath:BYTE *:inp: DFS full path name                      */
/*        pCur_TG       :struct GTGroup_t:inp: current user group table      */
/*        iO_mode       :int   :out: open mode                               */
/*        iS_mode       :int   :out: sharing mode                            */
/*        iNWerrno      :int * :out: NetWareErrno                            */
/* return = 0 or higher (DFS file handle): successfully copied               */
/*          -1                           : failed to copy                    */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

int FileCopyFromDFS(BYTE				*pszNWFullpath,
					BYTE				*pszDFSFullpath,
					struct	GTGroup_t	*pCur_TG,
					int					iO_mode,
					int					iS_mode,
					int					*iNWerrno)
{
	int 	iRd_sz		= 0;
	int		iWt_sz		= 0;
	int 	iDFS_fd		= -1;
	int		iNW_fd		= -1;
	int 	iDFS_err	= 0;
	int		iExit_flgs	= 0;
	int		iCcode 		= 0;
	short	iErr_flg	= OFF;
	char 	*pBuf = NULL;
	struct	ModifyStructure *pNWmd_t=NULL;
	struct	stat		NW_fattr;
	struct	dstat		DFS_fattr;
	struct	utimbuf		NW_ftime;
	long 	uiObjID;
	char 	aszObjName[ MAX_NWUSERNAME ];
	char 	uname[ MAX_NWUSERNAME ];
	WORD 	usObjType;
	BYTE 	byLoginTime[7];

	GW_tsh_functrace("FileCopyFromDFS",GW_FUNC_START,NULL);

	*iNWerrno = NULL;
	if ( DFSstatReq(	pszDFSFullpath,
						&DFS_fattr,
						&iDFS_err,
						pCur_TG)) {
		switch(iDFS_err) {
			case DECOM:
			case DEREFUSEAGT:
			case DETIMEOUTAGT:
			case DEDFAMTERM:
			case DERESPONCEDATA:
				memset(aszObjName,NULL,MAX_NWUSERNAME);
				memset(uname,NULL,MAX_NWUSERNAME);
				GetConnectionInformation( (WORD)(pCur_TG->uiConnNum), aszObjName,
										  &usObjType,  &uiObjID, byLoginTime );

    			if ( usObjType != OT_USER )
					uname[0] = '\0';
				else
					strcpy(uname,aszObjName );
				GW_Message(DFAM_NWC,(int *) &(pCur_TG->uiConnNum),LOG_YES,KDDS11413);
				GW_Message(DFAM_CON,NULL,LOG_YES,KDDS11414,uname,iDFS_err);
				*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
		 		if (pCur_TG->iLogoutStatus == OFF) {
					pCur_TG->iLogoutStatus = ON;
					iCcode = ResumeThread(pCur_TG->iThreadID);
					switch(iCcode) {
					case ESUCCESS:
						*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
						GW_tsh_log(iCcode,"OPPR3");
						break;
					case EINVAL:
					case EWRNGKND:
					case EBADHNDL:
						*iNWerrno = ERR_FAILURE;
						GW_tsh_log(iCcode,"OPPR4");
						break;
					default:
						*iNWerrno = ERR_FAILURE;
						GW_tsh_log(iCcode,"OPPR5");
						break;
					}
				}
				break;
			case DENOENT:
				if ( !stat(pszNWFullpath,&NW_fattr))
					*iNWerrno = ERR_NO_FILES_FOUND;
				else
					ConvertErrno(NULL,iDFS_err,iNWerrno);
				break;
			default:
				ConvertErrno(NULL,iDFS_err,iNWerrno);
				break;
		}
		return -1;
	}
	else {
		if (stat(pszNWFullpath,&NW_fattr)) {
			*iNWerrno = ERR_NO_FILES_FOUND;
			return -1;
		}
	}

	iDFS_fd = DFSopenReq(	pszDFSFullpath,
							iO_mode,
							iS_mode,
							&iDFS_err,
							pCur_TG);
	if (iDFS_fd < 0) {
		switch(iDFS_err) {
		case DEBADF:
		case DEINVAL:
			GW_tsh_log(iDFS_err,"FCPDFS1");
			ConvertErrno(GW_FILEOPEN,iDFS_err,iNWerrno);

			break;
		case DECOM:
		case DEREFUSEAGT:
		case DETIMEOUTAGT:
		case DEDFAMTERM:
		case DERESPONCEDATA:
			memset(aszObjName,NULL,MAX_NWUSERNAME);
			memset(uname,NULL,MAX_NWUSERNAME);
			GetConnectionInformation( (WORD)(pCur_TG->uiConnNum), aszObjName,
										  &usObjType,  &uiObjID, byLoginTime );

   			if ( usObjType != OT_USER )
				uname[0] = '\0';
			else
				strcpy(uname,aszObjName );

			GW_Message(DFAM_NWC,(int *) &(pCur_TG->uiConnNum),LOG_YES,KDDS11413);
			GW_Message(DFAM_CON,NULL,LOG_YES,KDDS11414,uname,iDFS_err);
			*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
			GW_tsh_log(iDFS_err,"FCPDFS2");
			ConvertErrno(GW_FILEOPEN,iDFS_err,iNWerrno);
			if (pCur_TG->iLogoutStatus == OFF) {
				pCur_TG->iLogoutStatus = ON;
				iCcode = ResumeThread(pCur_TG->iThreadID);
				switch(iCcode) {
					case ESUCCESS:
						GW_tsh_log(iDFS_err,"FCPDFS3");
						*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
						break;
					case EINVAL:
					case EWRNGKND:
					case EBADHNDL:
						GW_tsh_log(iDFS_err,"FCPDFS4");
						*iNWerrno = ERR_FAILURE;
						break;
					default:
						GW_tsh_log(iDFS_err,"FCPDFS5");
						*iNWerrno = ERR_FAILURE;
						break;
				}
			}
			break;
		default:
			GW_tsh_log(iDFS_err,"FCPDFS6");
			ConvertErrno(GW_FILEOPEN,iDFS_err,iNWerrno);
			break;
		}
		iDFS_fd = -1;
		goto Err_exit;
	}

	if ( !stat(pszNWFullpath,&NW_fattr)) {
		if (remove((char *) pszNWFullpath) != 0) {
			*iNWerrno = NetWareErrno;

			if ((NW_fattr.st_mode & S_IFDIR) != S_IFDIR) {
				GW_tsh_log(*iNWerrno,"FCPDFS7");




			}
			goto Err_exit;
		}
	}

	if ((iNW_fd = open(	pszNWFullpath,
						O_CREAT | O_RDWR | O_BINARY,
						S_IRWXU | S_IRWXG | S_IRWXO)) < 0){
			*iNWerrno = errno;
			GW_tsh_log(*iNWerrno,"FCPDFS9");
			goto Err_exit;
	}

	pBuf = GW_malloc(pAgent_sp->iCom_SendBuffLen+1);
	if(pBuf == NULL){
		*iNWerrno = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit;
	}

	if (DFSCpFirstReadReq(iDFS_fd,
							pAgent_sp->iCom_SendBuffLen,
							&iDFS_err,
							pCur_TG) != 0) {
		switch(iDFS_err) {
			case DEBADF:
			case DEDEADLK:
				ConvertErrno(NULL,iDFS_err,iNWerrno);
				GW_tsh_log(iDFS_err,"FCPDFS11");
				break;
			case DECOM:
			case DEREFUSEAGT:
			case DETIMEOUTAGT:
			case DEDFAMTERM:
			case DERESPONCEDATA:
				memset(aszObjName,NULL,MAX_NWUSERNAME);
				memset(uname,NULL,MAX_NWUSERNAME);
				GetConnectionInformation( (WORD)(pCur_TG->uiConnNum), aszObjName,
										  &usObjType,  &uiObjID, byLoginTime );

    			if ( usObjType != OT_USER )
					uname[0] = '\0';
				else
					strcpy(uname,aszObjName );

				GW_Message(DFAM_NWC,(int *) &(pCur_TG->uiConnNum),LOG_YES,KDDS11413);
				GW_Message(DFAM_CON,NULL,LOG_YES,KDDS11414,uname,iDFS_err);
				*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
				GW_tsh_log(iDFS_err,"FCPDFS12");
				if (pCur_TG->iLogoutStatus == OFF) {
					pCur_TG->iLogoutStatus = ON;
					iDFS_fd = -1;
					iCcode = ResumeThread(pCur_TG->iThreadID);
					switch(iCcode) {
						case ESUCCESS:
							GW_tsh_log(iCcode,"FCPDFS13");
							*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
							break;
						case EINVAL:
						case EWRNGKND:
						case EBADHNDL:
							*iNWerrno = ERR_FAILURE;
							GW_tsh_log(iCcode,"FCPDFS14");
							break;
						default:
							*iNWerrno = ERR_FAILURE;
							GW_tsh_log(iCcode,"FCPDFS15");
							break;
					}
				}
				break;
			default:
				ConvertErrno(NULL,iDFS_err,iNWerrno);
				GW_tsh_log(iDFS_err,"FCPDFS16");
				break;
		}
		goto ReadEnd;
	}

	while (1) {
		memset(pBuf,NULL,pAgent_sp->iCom_SendBuffLen+1);
		iRd_sz = DFSCpMiddleReadReq(pBuf,
							&iDFS_err,
							pCur_TG);


		if (iRd_sz > 0) {
			ThreadSwitch();
			if (iErr_flg == ON)
				continue;
			iWt_sz = write(iNW_fd,pBuf,iRd_sz);
			if (iWt_sz < 0 || iWt_sz < iRd_sz){
				if (errno == EBADF) {
					*iNWerrno = errno;
					GW_tsh_log(errno,"FCPDFS9");
					iErr_flg = ON;
					continue;
				} else {
					*iNWerrno = errno;
					GW_tsh_log(*iNWerrno,"FCPDFS10");
					iErr_flg = ON;
					continue;
				}
			}
		}
		if (iRd_sz < 0) {
			switch(iDFS_err) {
			case DEBADF:
			case DEDEADLK:
				ConvertErrno(NULL,iDFS_err,iNWerrno);
				GW_tsh_log(iDFS_err,"FCPDFS11");
				break;
			case DECOM:
			case DEREFUSEAGT:
			case DETIMEOUTAGT:
			case DEDFAMTERM:
			case DERESPONCEDATA:
				memset(aszObjName,NULL,MAX_NWUSERNAME);
				memset(uname,NULL,MAX_NWUSERNAME);
				GetConnectionInformation( (WORD)(pCur_TG->uiConnNum), aszObjName,
										  &usObjType,  &uiObjID, byLoginTime );

    			if ( usObjType != OT_USER )
					uname[0] = '\0';
				else
					strcpy(uname,aszObjName );

				GW_Message(DFAM_NWC,(int *) &(pCur_TG->uiConnNum),LOG_YES,KDDS11413);
				GW_Message(DFAM_CON,NULL,LOG_YES,KDDS11414,uname,iDFS_err);
				*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
				GW_tsh_log(iDFS_err,"FCPDFS12");
				if (pCur_TG->iLogoutStatus == OFF) {
					pCur_TG->iLogoutStatus = ON;
					iDFS_fd = -1;
					iCcode = ResumeThread(pCur_TG->iThreadID);
					switch(iCcode) {
						case ESUCCESS:
							GW_tsh_log(iCcode,"FCPDFS13");
							*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
							break;
						case EINVAL:
						case EWRNGKND:
						case EBADHNDL:
							*iNWerrno = ERR_FAILURE;
							GW_tsh_log(iCcode,"FCPDFS14");
							break;
						default:
							*iNWerrno = ERR_FAILURE;
							GW_tsh_log(iCcode,"FCPDFS15");
							break;
					}
				}
				break;
			default:
				ConvertErrno(NULL,iDFS_err,iNWerrno);
				GW_tsh_log(iDFS_err,"FCPDFS16");
				break;
			}
			goto ReadEnd;
		}
		if ( iRd_sz == 0 || iRd_sz < pAgent_sp->iCom_SendBuffLen) {
				break;
		}
	}

ReadEnd:
	if (DFSCpLastReadReq(&iDFS_err,
						 pCur_TG) != 0) {
		switch(iDFS_err) {
			case DEBADF:
			case DEDEADLK:
				ConvertErrno(NULL,iDFS_err,iNWerrno);
				GW_tsh_log(iDFS_err,"FCPDFS11");
				break;
			case DECOM:
			case DEREFUSEAGT:
			case DETIMEOUTAGT:
			case DEDFAMTERM:
			case DERESPONCEDATA:
				memset(aszObjName,NULL,MAX_NWUSERNAME);
				memset(uname,NULL,MAX_NWUSERNAME);
				GetConnectionInformation( (WORD)(pCur_TG->uiConnNum), aszObjName,
										  &usObjType,  &uiObjID, byLoginTime );

    			if ( usObjType != OT_USER )
					uname[0] = '\0';
				else
					strcpy(uname,aszObjName );

				GW_Message(DFAM_NWC,(int *) &(pCur_TG->uiConnNum),LOG_YES,KDDS11413);
				GW_Message(DFAM_CON,NULL,LOG_YES,KDDS11414,uname,iDFS_err);
				*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
				GW_tsh_log(iDFS_err,"FCPDFS12");
				if (pCur_TG->iLogoutStatus == OFF) {
					pCur_TG->iLogoutStatus = ON;
					iDFS_fd = -1;
					iCcode = ResumeThread(pCur_TG->iThreadID);
					switch(iCcode) {
						case ESUCCESS:
							GW_tsh_log(iCcode,"FCPDFS13");
							*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
							break;
						case EINVAL:
						case EWRNGKND:
						case EBADHNDL:
							*iNWerrno = ERR_FAILURE;
							GW_tsh_log(iCcode,"FCPDFS14");
							break;
						default:
							*iNWerrno = ERR_FAILURE;
							GW_tsh_log(iCcode,"FCPDFS15");
							break;
					}
				}
				break;
			default:
				ConvertErrno(NULL,iDFS_err,iNWerrno);
				GW_tsh_log(iDFS_err,"FCPDFS16");
				break;
		}
	}

	if (close(iNW_fd)) {
		if (errno == EBADF) {
			*iNWerrno = errno;
		}
		else
			*iNWerrno = NetWareErrno;

		GW_tsh_log(*iNWerrno,"FCPDFS17");
		goto Err_exit;
	}
	iNW_fd = -1;

	if ( chmod(pszNWFullpath, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP |
							S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH)) {
		*iNWerrno = errno;
		GW_tsh_log(*iNWerrno,"FCPDFS18");
		goto Err_exit;
	}

	if ((pNWmd_t = (struct ModifyStructure *)GW_malloc(sizeof(struct ModifyStructure)))
		 == NULL ) {
		*iNWerrno = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit;
	}
	memset(pNWmd_t,NULL,sizeof(struct ModifyStructure));
	if ( ChangeDirectoryEntry(	pszNWFullpath,
								pNWmd_t,
								MFileAttributesBit,
								0) ) {
		*iNWerrno = NetWareErrno;
		GW_tsh_log(*iNWerrno,"FCPDFS19");
		goto Err_exit;
	}

	iExit_flgs = 0;
	NW_ftime.modtime = DFS_fattr.d_mtime;
	goto Normal_exit;

Err_exit:

	NW_ftime.modtime = NW_fattr.st_mtime;
	if (iDFS_fd >= 0 ) {
		if ( DFScloseReq(	iDFS_fd,
							&iDFS_err,
							pCur_TG)) {
			switch(iDFS_err) {
			case DEBADF:
				ConvertErrno(NULL,iDFS_err,iNWerrno);
				GW_tsh_log(*iNWerrno,"FCPDFS20");
				break;
			case DECOM:
			case DEREFUSEAGT:
			case DETIMEOUTAGT:
			case DEDFAMTERM:
			case DERESPONCEDATA:
				memset(aszObjName,NULL,MAX_NWUSERNAME);
				memset(uname,NULL,MAX_NWUSERNAME);
				GetConnectionInformation( (WORD)(pCur_TG->uiConnNum), aszObjName,
									  &usObjType,  &uiObjID, byLoginTime );

				if ( usObjType != OT_USER )
					uname[0] = '\0';
				else
					strcpy(uname,aszObjName );

				GW_Message(DFAM_NWC,(int *) &(pCur_TG->uiConnNum),LOG_YES,KDDS11413);
				GW_Message(DFAM_CON,NULL,LOG_YES,KDDS11414,uname,iDFS_err);
				*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
				if ( pCur_TG->iLogoutStatus == OFF) {
					pCur_TG->iLogoutStatus = ON;
					iCcode = ResumeThread(pCur_TG->iThreadID);
					switch(iCcode) {
						case ESUCCESS:
							*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
							GW_tsh_log(*iNWerrno,"FCPDFS21");
							break;
						case EINVAL:
						case EWRNGKND:
						case EBADHNDL:
							*iNWerrno = ERR_FAILURE;
							GW_tsh_log(*iNWerrno,"FCPDFS22");
							break;
						default:
							*iNWerrno = ERR_FAILURE;
							GW_tsh_log(*iNWerrno,"FCPDFS23");
							break;
					}
				}
				break;
			default:
				ConvertErrno(NULL,iDFS_err,iNWerrno);
				GW_tsh_log(*iNWerrno,"FCPDFS23");
				break;
			}
		}
	}
	iDFS_fd = -1;

Normal_exit:

	NW_ftime.acctime = NW_fattr.st_atime;
	utime(pszNWFullpath,&NW_ftime);

	if (iNW_fd >= 0) {
		if ( close(iNW_fd) ) {
			if (errno == EBADF) {
				*iNWerrno = errno;
			}
			else
				*iNWerrno = NetWareErrno;
		}
	}

	if (pBuf != NULL)
		GW_free(pBuf,NULL);
	if (pNWmd_t != NULL)
		GW_free(pNWmd_t,NULL);

	GW_tsh_functrace("FileCopyFromDFS",GW_FUNC_END,NULL);
	return iDFS_fd;
 }

/*EnglishComment**************************************************************/
/* ID   = FileCopyFromNW                                                     */
/* name = Copyback NetWare files                                             */
/* func = Copies back NetWare dummy files to DFS file.                       */
/* i/o  = pszNWFullpath :BYTE *:inp: NetWare full path name                  */
/*        iDFSHandle    :long  :inp: DFS file handle                         */
/*        pCur_TG       :struct GTGroup_t:inp: current user group table      */
/* return =  0: successfully copied                                          */
/*          -1: failed to copy                                               */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int FileCopyFromNW(	BYTE				*pszNWFullpath,
					long				iDFSHandle,
					struct	GTGroup_t	*pCur_TG)
{
	int 	iRd_sz = 0;
	int		iWt_sz = 0;
	int		iNW_fd = -1;
	int		iCcode = 0;
	int 	iDFS_err = 0;
	int 	iNWerrno = 0;
	short	iFirst_flg = 0;
	short	iWrite_flg = 0;
	char 	*pBuf = NULL;

	GW_tsh_functrace("FileCopyFromNW",GW_FUNC_START,NULL);


	if ((iNW_fd = open(	pszNWFullpath,
						O_RDONLY | O_BINARY,
						0)) < 0){
		iNWerrno = errno;
		GW_tsh_log(iNWerrno,"FCPNW1");
		return -1;
	}
	pBuf = GW_malloc(pAgent_sp->iCom_SendBuffLen+1);
	if (pBuf == NULL) {
		iNWerrno = ERR_SERVER_OUT_OF_MEMORY;
		GW_tsh_log(iNWerrno,"FCPNW2");
		goto Err_exit;
	}
	memset(pBuf,NULL,pAgent_sp->iCom_SendBuffLen+1);

    iRd_sz = 0;

	iCcode = DFSlseekReq(iDFSHandle, 0, 0, &iDFS_err, pCur_TG);
	if (iCcode != 0) {
		GW_tsh_log(iDFS_err,"FCPNW59");
		if (iDFS_err == DECOM || iDFS_err == DEREFUSEAGT ||
			iDFS_err == DETIMEOUTAGT || iDFS_err == DEDFAMTERM || iDFS_err == DERESPONCEDATA) {
			GW_tsh_log(iCcode,"FCPNW58");
			if (pCur_TG->iLogoutStatus == OFF) {
				pCur_TG->iLogoutStatus = ON;
				ConvertErrno(NULL,iDFS_err,&iNWerrno);
				iCcode = ResumeThread(pCur_TG->iThreadID);
				switch(iCcode) {
					case ESUCCESS:
					iNWerrno = ERR_NO_OPEN_PRIVILEGE;
					GW_tsh_log(iCcode,"FCPNW3");
					break;
					case EINVAL:
					case EWRNGKND:
					case EBADHNDL:
						iNWerrno = ERR_FAILURE;
						GW_tsh_log(iCcode,"FCPNW4");
						break;
					default:
						iNWerrno = ERR_FAILURE;
						GW_tsh_log(iCcode,"FCPNW5");
						break;
				}
			}
		}
		iCcode = -1;
		goto Err_exit;
	}

	while(( iRd_sz = read(iNW_fd,pBuf,pAgent_sp->iCom_SendBuffLen)) > 0){
		iWrite_flg = 1;
		if (iFirst_flg == 0) {
			if ( ( iWt_sz = DFSCpbkFirstWriteReq(iDFSHandle,
												pBuf,
												iRd_sz,
												&iDFS_err,
												pCur_TG) ) == -1) {
				GW_tsh_log(iDFS_err,"FCPNW66");
				if (iDFS_err == DECOM || iDFS_err == DEREFUSEAGT ||
					iDFS_err == DETIMEOUTAGT || iDFS_err == DEDFAMTERM || iDFS_err == DERESPONCEDATA) {
					if (pCur_TG->iLogoutStatus == OFF) {
						pCur_TG->iLogoutStatus = ON;
						ConvertErrno(NULL,iDFS_err,&iNWerrno);
						iCcode = ResumeThread(pCur_TG->iThreadID);
						switch(iCcode) {
							case ESUCCESS:
								iNWerrno = ERR_NO_OPEN_PRIVILEGE;
								GW_tsh_log(iCcode,"FCPNW3");
								break;
							case EINVAL:
							case EWRNGKND:
							case EBADHNDL:
								iNWerrno = ERR_FAILURE;
								GW_tsh_log(iCcode,"FCPNW4");
								break;
							default:
								iNWerrno = ERR_FAILURE;
								GW_tsh_log(iCcode,"FCPNW5");
								break;
						}
					}
				}
				iCcode = -1;
				goto Write_Exit;
			}
			iFirst_flg = 1;
		}
		else {
			if (( iWt_sz = DFSCpbkMiddleWriteReq(iDFSHandle,
										pBuf,
										iRd_sz,
										&iDFS_err,
										pCur_TG) ) == -1) {
				GW_tsh_log(iDFS_err,"FCPNW67");
 				if (iDFS_err == DECOM || iDFS_err == DEREFUSEAGT ||
					iDFS_err == DETIMEOUTAGT || iDFS_err == DEDFAMTERM || iDFS_err == DERESPONCEDATA) {
					GW_tsh_log(iCcode,"FCPNW56");
					if (pCur_TG->iLogoutStatus == OFF) {
						pCur_TG->iLogoutStatus = ON;
						ConvertErrno(NULL,iDFS_err,&iNWerrno);
						iCcode = ResumeThread(pCur_TG->iThreadID);
						switch(iCcode) {
							case ESUCCESS:
								iNWerrno = ERR_NO_OPEN_PRIVILEGE;
								GW_tsh_log(iCcode,"FCPNW3");
								break;
							case EINVAL:
							case EWRNGKND:
							case EBADHNDL:
								iNWerrno = ERR_FAILURE;
								GW_tsh_log(iCcode,"FCPNW4");
								break;
							default:
								iNWerrno = ERR_FAILURE;
								GW_tsh_log(iCcode,"FCPNW5");
								break;
						}
					}
				}
				iCcode = -1;
				goto Write_Exit;
			}
		}
		if(iRd_sz < pAgent_sp->iCom_SendBuffLen) {
			break;
		}
		ThreadSwitch();
		memset(pBuf,NULL,pAgent_sp->iCom_SendBuffLen+1);
	}

	if(iRd_sz < 0) {
		GW_tsh_log(iCcode,"FCPNW6");
		iCcode = iRd_sz;
	}

	if (iWrite_flg == 0)
		goto Err_exit;

Write_Exit:
	if ((iWt_sz = DFSCpbkLastWriteReq(iDFSHandle,
										&iDFS_err,
										pCur_TG) ) == -1) {
		if (iDFS_err == DECOM || iDFS_err == DEREFUSEAGT ||
				iDFS_err == DETIMEOUTAGT || iDFS_err == DEDFAMTERM || iDFS_err == DERESPONCEDATA) {
			GW_tsh_log(iCcode,"FCPNW50");
			if (pCur_TG->iLogoutStatus == OFF) {
				pCur_TG->iLogoutStatus = ON;
				ConvertErrno(NULL,iDFS_err,&iNWerrno);
				iCcode = ResumeThread(pCur_TG->iThreadID);
				switch(iCcode) {
					case ESUCCESS:
						iNWerrno = ERR_NO_OPEN_PRIVILEGE;
						GW_tsh_log(iCcode,"FCPNW3");
						break;
					case EINVAL:
					case EWRNGKND:
					case EBADHNDL:
						iNWerrno = ERR_FAILURE;
						GW_tsh_log(iCcode,"FCPNW4");
						break;
					default:
						iNWerrno = ERR_FAILURE;
						GW_tsh_log(iCcode,"FCPNW5");
						break;
				}
			}
		}
		iCcode = -1;
	}

Err_exit:

	if (iNW_fd != -1)
		close(iNW_fd);

	if (pBuf != NULL) {
		GW_free(pBuf,NULL);
	}

	GW_tsh_functrace("FileCopyFromNW",GW_FUNC_END,iCcode);

	return iCcode;
}
/*EnglishComment**************************************************************/
/* ID   = FAR_Terminate                                                      */
/* name = Termination for GateWayUNLOAD                                      */
/* func = Frees file accessing resources for GateWayUNLOAD.                  */
/* i/o  = none                                                               */
/* return = none                                                             */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
void	FAR_Terminate(void)
{
	struct	GTGroup_t 	*pCur_wp	= NULL;
	struct	OpenFile_t 	*pOF		= NULL;
	struct	GCopyBack_t *pCB		= NULL;
	struct	GCopyBack_t *pCB_w		= NULL;
	struct FileAccessList *pAcs_w = NULL;
	struct FileAccessList *pAcs	  = NULL;
	int					iSem_valu;
	short				uiCpBk_flg = ON;
	short				uiFopn_flg = ON;
	short				uiFacs_flg = ON;

	while(1) {
		delay(SHORTWAITTIME);
		iSem_valu = ExamineLocalSemaphore(uiCopyFile_sh);
		if (iSem_valu < 1)
			SignalLocalSemaphore(uiCopyFile_sh);
		else if (iSem_valu > 1)
			WaitOnLocalSemaphore(uiCopyFile_sh);
		else
			break;
	}

	pCur_wp = pTGroup_sp;
	while( pCur_wp != NULL) {
		delay(SHORTWAITTIME);
		pOF = pCur_wp->pOFile_sp;
		while(pOF != NULL) {
				close(pOF->uiNWHandle);
			if (pOF->uiClose_sh != NULL) {
				if (ExamineLocalSemaphore(pOF->uiClose_sh) < 0) {
					while(ExamineLocalSemaphore(pOF->uiClose_sh) < 0)
						SignalLocalSemaphore(pOF->uiClose_sh);
				}
			}
			while (pCur_wp->iExistThread != 0)
				delay(SHORTWAITTIME);
			DelCopyBack_t(pOF->aszNWFullpath);
			DelOpenFile_t(pOF,pCur_wp);
			pOF = pCur_wp->pOFile_sp;
		}
		pCur_wp = pCur_wp->pNext;
	}

	pAcs = pAcsList_sp;
	while( pAcs != NULL) {
		delay(SHORTWAITTIME);
		if (pAcs->uiAcces_sh > 0) {
			if (ExamineLocalSemaphore(pAcs_w->uiAcces_sh) < 0)
				SignalLocalSemaphore(pAcs_w->uiAcces_sh);
			else {
				pAcs_w = pAcs;
				pAcs = pAcs->pNext_t;
				CloseLocalSemaphore(pAcs_w->uiAcces_sh);
				GW_free(pAcs_w,NULL);
				continue;
			}
		}
		else {
			pAcs_w = pAcs;
			pAcs = pAcs->pNext_t;
			GW_free(pAcs_w,NULL);
			continue;
		}
		pAcs = pAcs->pNext_t;
	}

	pCB = pCopyBack_sp;
	while(pCB != NULL) {
		delay(SHORTWAITTIME);
		pCB_w = pCB;
		pCB = pCB->pNext_t;
		GW_free(pCB_w, NULL);
	}

	do {
		delay(SHORTWAITTIME);
		if (pCur_wp == NULL)
			uiFopn_flg = OFF;
		else {
			if (pCur_wp->pOFile_sp == NULL)
				uiFopn_flg = OFF;
			else
				uiFopn_flg = ON;
		}

		pAcs_w = pAcsList_sp;
		uiFacs_flg = OFF;
		while( pAcs_w != NULL) {
			if (pAcs_w->uiConnNum == pCur_wp->uiConnNum)
				uiFacs_flg = ON;
			pAcs_w = pAcs_w->pNext_t;
		}

		if (pCopyBack_sp == NULL)
			uiCpBk_flg = OFF;

	} while(uiFopn_flg == ON || uiFacs_flg == ON || uiCpBk_flg == ON);
}

/*EnglishComment**************************************************************/
/* ID   = Terminate_Grp                                                      */
/* name = Termination (for user logout)                                      */
/* func = Frees unfreed resources when user logs out.                        */
/* i/o  = pCur_TG       :struct GTGroup_t:inp: current user group table      */
/* return = none                                                             */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

void	Terminate_Grp(struct	GTGroup_t 	*pCur)
{
	struct	GTGroup_t 	*pCur_wp	= NULL;
	struct	OpenFile_t 	*pOF		= NULL;
	struct	OpenFile_t 	*pOF_b		= NULL;
	struct	GCopyBack_t *pCB		= NULL;
	struct FileAccessList *pAcs_w = NULL;
	short				uiFopn_flg = ON;
	short				uiFacs_flg = ON;

	while (ExamineLocalSemaphore(pCur->uiFHandle_sh) < 1) {
		SignalLocalSemaphore(pCur->uiFHandle_sh);
		delay(SHORTWAITTIME);
	}
	pCur_wp = pCur;
	pOF = pCur_wp->pOFile_sp;
	do {
		if (pOF == NULL)
			break;
		if (pOF->uiClose_sh != NULL) {
			if (ExamineLocalSemaphore(pOF->uiClose_sh) < 0) {
				SignalLocalSemaphore(pOF->uiClose_sh);

				delay(SHORTWAITTIME);

				if (ExamineLocalSemaphore(uiCopyFile_sh) < 0) {
					pCur->iSemaporCount--;
					SignalLocalSemaphore(uiCopyFile_sh);
				}
			}
		}

		pCB = SearchCopyBack_t(pOF->aszNWFullpath);
		if(pCB->iCopyBack_flg == ON && pCB->uiTasknum != NULL) {
			pCB->iDelay_flg = ON;
			ResumeThread(pCB->uiTasknum);
			do {
				delay(SHORTWAITTIME);
			} while( ((pCB = SearchCopyBack_t(pOF_b->aszNWFullpath)) !=
									(struct GCopyBack_t *)  NULL)
							&& pCB->iCopyBack_flg == ON );
		}

		delay(SHORTWAITTIME);
		pOF = pOF->pNextOF_t;
	} while (pCur_wp->iExistThread != 0);

	while(pOF != NULL) {
		delay(SHORTWAITTIME);
		if (pOF == pOF_b) {
			pOF_b = pOF;
			pOF = pCur_wp->pOFile_sp;
			continue;
		}
		pOF_b = pOF;
		pOF = pCur_wp->pOFile_sp;
	}
	DelAccessListConNoAll(pCur_wp->uiConnNum);

	do {
		delay(SHORTWAITTIME);
		if (pCur_wp->pOFile_sp == NULL)
			uiFopn_flg = OFF;
		else
			uiFopn_flg = ON;

		pAcs_w = pAcsList_sp;
		uiFacs_flg = OFF;
		while( pAcs_w != NULL) {
			delay(SHORTWAITTIME);
			if (pAcs_w->uiConnNum == pCur_wp->uiConnNum)
				uiFacs_flg = ON;
			pAcs_w = pAcs_w->pNext_t;
		}
	} while(uiFopn_flg == ON || uiFacs_flg == ON);

	while(1) {
		delay(SHORTWAITTIME);
		if (pCur->iSemaporCount > 0) {
			pCur->iSemaporCount--;
			if (ExamineLocalSemaphore(uiCopyFile_sh) < 1)
				SignalLocalSemaphore(uiCopyFile_sh);
		}
		else if (pCur->iSemaporCount < 0) {
			pCur->iSemaporCount++;
			if (ExamineLocalSemaphore(uiCopyFile_sh) > 1)
				WaitOnLocalSemaphore(uiCopyFile_sh);
		}
		else
			break;
	}
}

/*EnglishComment**************************************************************/
/* ID   = GW_FileBackUP                                                      */
/* name = Backup NetWare dummy files                                         */
/* func = Saves the NetWare dummy files to a backup directory.               */
/* i/o  = pOF :OpenFile_t *:inp: pointer to open open file table             */
/* return = 0     : successfully copied                                      */
/*          others: failed to copy                                           */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

int GW_FileBackUP(struct OpenFile_t 	*pOF )
{
	int 	iRd_sz = 0;
	int		iNW_fd = -1;
	int		iBK_fd = -1;
	int		iCcode = 0;
	char 	*pBuf			= NULL;
	char	*pbkupfilename	= NULL;
	char	*p,*pb;


	if (aszCopyBackDirName == NULL) {
		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11402);
		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11403);
		MsgPathNameSet(pOF);
		return(-1);
	}
	if (strlen(aszCopyBackDirName) == 0) {
		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11402);
		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11403);
		MsgPathNameSet(pOF);
		return(-1);
	}

	pb = strchr ( pOF->aszNWFullpath, ':');
	if (pb == NULL || *(pb+1) == '\0') {
		return(-1);
	}
	pb++;
	while(1) {
		p = strchr ( pb, '\\');
		if (p == NULL)
			break;
		pb = p+1;
	}

	pbkupfilename = GW_malloc(_MAX_PATH+1);
	if (pbkupfilename == (char *) NULL) {
		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11403);
		MsgPathNameSet(pOF);
		GW_tsh_log(NetWareErrno,"FLBK1");
		return(-1);
	}

	sprintf(pbkupfilename,"%s\\%s",aszCopyBackDirName,pb);

	if (strcmp(aszCopyBackDirName,pbkupfilename) == 0)	{
		iCcode = 1;
		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11403);
		MsgPathNameSet(pOF);
		GW_tsh_log(NetWareErrno,"FLBK2");
		goto Err_exit;
	}

	if (pOF->uiNWHandle != -1)
		close(pOF->uiNWHandle);

	if ((iNW_fd = open(	pOF->aszNWFullpath,
						O_RDONLY | O_BINARY,
						0)) == -1){
		iCcode = errno;
		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11403);
		MsgPathNameSet(pOF);
		GW_tsh_log(iCcode,"FLBK3");
		goto Err_exit;
	}

	if ((iBK_fd = open(	pbkupfilename,
						O_CREAT | O_RDWR | O_BINARY,
						0)) == -1){
		iCcode = errno;
		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11403);
		MsgPathNameSet(pOF);
		GW_tsh_log(iCcode,"FLBK4");
		goto Err_exit;
	}

	pBuf = GW_malloc(4097);
	if(pBuf == NULL){
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11403);
		MsgPathNameSet(pOF);
		GW_tsh_log(iCcode,"FLBK5");
		goto Err_exit;
	}

	while(1) {
		memset(pBuf,NULL,4097);
		if ((iRd_sz = read(iNW_fd,pBuf,4096)) <= 0) {
			if (iRd_sz == -1) {
				iCcode = errno;
				GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11403);
				MsgPathNameSet(pOF);
				GW_tsh_log(iCcode,"FLBK6");
				goto Err_exit;
			}
			break;
		}

		if (write(iBK_fd,pBuf,iRd_sz) != iRd_sz) {
			iCcode = errno;
			GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11403);
			MsgPathNameSet(pOF);
			GW_tsh_log(iCcode,"FLBK7");
			goto Err_exit;
		}
	}

	GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11404);
	GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11405);
	GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11407);
	MsgPathNameSet(pOF);
	GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11406);
	GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11408);
	GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11412,pb);

Err_exit:
	if (iNW_fd != -1)
		close(iNW_fd);

	if (iBK_fd != -1)
		close(iBK_fd);

	if (pBuf != NULL)
		GW_free(pBuf,NULL);


	if (pbkupfilename != NULL)
		GW_free(pbkupfilename,NULL);

	return iCcode;
}



/*EnglishComment**************************************************************/
/* ID   = MakeSparseFile                                                     */
/* name = Making a spars file                                                */
/* func = Sparse-izes a NetWare file                                         */
/* i/o  = pOF:struct OpenFile_t *:inp: pointer to open file table            */
/* return = 0     : a sparse file successfully made                          */
/*          others: failed to make a sparse file                             */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

int MakeSparseFile(struct OpenFile_t *pOF)
{
	int		iCcode = 0;
	int		iNWerrno = 0;
	LONG	uiNW_fd;
	LONG	iFile_sz=0;
	BYTE	aszNWFullpath[_MAX_PATH+1];
	struct ModifyStructure *pNWmd_t = NULL;

	GW_tsh_functrace("MakeSparse",GW_FUNC_START,NULL);

	memset(aszNWFullpath,NULL,_MAX_PATH+1);
	strcpy(aszNWFullpath, pOF->aszNWFullpath);
	if ( (uiNW_fd = open(aszNWFullpath, O_RDWR | O_BINARY) ) == -1) {
		iNWerrno = NetWareErrno;
		GW_tsh_log(iNWerrno,"MKSP1");
		iCcode = -1;
		goto Err_exit;
	}
	iFile_sz = filelength(uiNW_fd);
	if( (int)iFile_sz < 0 ) {
		GW_tsh_log(iNWerrno,"MKSP2");
		iCcode = -1;
		goto Err_exit;
	}

	if ( chsize( uiNW_fd, (LONG)0 ) ) {
		iNWerrno = NetWareErrno;
		GW_tsh_log(iNWerrno,"MKSP3");
		iCcode = -1;
		goto Err_exit;
	}

	close(uiNW_fd);

	if (iFile_sz == 0)
		goto Err_exit;

	if ( (int)iFile_sz > 0 ) {
		iFile_sz--;
	}

	if ( (uiNW_fd = open(aszNWFullpath, O_WRONLY | O_BINARY) ) == -1) {
		iNWerrno = NetWareErrno;
		GW_tsh_log(iNWerrno,"MKSP4");
		iCcode = -1;
		goto Err_exit;
	}

	if ( (iCcode = lseek( uiNW_fd, iFile_sz, SEEK_SET) ) == -1) {
		iNWerrno = errno;
		GW_tsh_log(iNWerrno,"MKSP5");
		close(uiNW_fd);
		iCcode = -1;
		goto Err_exit;
	}

	if ( write( uiNW_fd, NULL, 1) <= 0) {
		iNWerrno = errno;
		GW_tsh_log(iNWerrno,"MKSP6");
		close(uiNW_fd);
		iCcode = -1;
		goto Err_exit;
	}

	close(uiNW_fd);

	if ( chmod(aszNWFullpath, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP |
							S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH)) {
		iNWerrno = errno;
		GW_tsh_log(iNWerrno,"MKSP7");
		iCcode = -1;
		goto Err_exit;
	}

	if ((pNWmd_t = (struct ModifyStructure *)GW_malloc(sizeof(struct ModifyStructure)))
		 == NULL ) {
		iNWerrno = ERR_SERVER_OUT_OF_MEMORY;
		GW_tsh_log(iNWerrno,"MKSP8");
		iCcode = -1;
		goto Err_exit;
	}

	memset(pNWmd_t,NULL,sizeof(struct ModifyStructure));
    pNWmd_t->MFileAttributesMask = pOF->uiAttrMatchBits;
	if ( ChangeDirectoryEntry(	aszNWFullpath,
								pNWmd_t,
								MFileAttributesBit,
								0) ) {
		iNWerrno = NetWareErrno;
		GW_tsh_log(iNWerrno,"MKSP9");
		iCcode = -1;
	}

	if(pNWmd_t != (struct ModifyStructure *) NULL)
		GW_free(pNWmd_t,NULL);

Err_exit:

	return iCcode;
}

/*EnglishComment**************************************************************/
/* ID   = MsgPathNameSet                                                     */
/* name = Division of a path name for error message displaying               */
/* func = Slices a path name into a block of 40 characters to put            */
/*        the path name into the error message.                              */
/* i/o  = pOF:struct OpenFile_t *:inp: pointer to open file table            */
/* return = none                                                             */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
void	MsgPathNameSet(struct OpenFile_t *pOF)
{
	short	len		= 0;
	short	w_len	= 0;
	short	wp		= 0;
	char	wk[1024];

	memset(wk,NULL,41);
	if ((len = strlen(pOF->aszNWFullpath)) > 40) {
		memcpy(wk,pOF->aszNWFullpath,40);
		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11409,wk);
		len -= 40;
		wp = 40;
		while(len > 0) {
			len -= 40;
			if (len > 40) {
				wp	+= 40;
				w_len = 40;
			}
			else {
				wp += len;
				w_len = len;
			}
			memset(wk,NULL,41);
			memcpy(wk,pOF->aszNWFullpath+wp,w_len);
			GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11410,wk);
		}
	}
	else {
		strcpy(wk,pOF->aszNWFullpath);
		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11411,pOF->aszNWFullpath);
	}
}


/*EnglishComment************************************************************/
/* ID   = DelayCotoff                                                      */
/* name = Reset copyback delay                                             */
/* func = Resets the delay of the copyback thread.                         */
/* io   = pTGroup : struct GTGroup_t *: i : address of the user thread     */
/*                                          group table                    */
/* return =  none                                                          */
/* common =  none                                                          */
/* note   =  none                                                          */
/* date   =  February 2, 1996                                              */
/*                                                                         */
/* Trade Secret of Hitachi, Ltd.                                           */
/* Do not disclose without written concession from Hitachi, Ltd.           */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.*/
/*EnglishComment************************************************************/

void DelayCutoff( struct	GTGroup_t	*pTGroup )
{
	struct	OpenFile_t *pOF			= NULL;
	struct	GCopyBack_t *pCB		= NULL;
	int bIsResume;


	if (pTGroup == NULL)
		return;

	bIsResume = OFF;

	pTGroup->iSemaporCount++;
	WaitOnLocalSemaphore(uiCopyFile_sh);

	pOF = pTGroup->pOFile_sp;
	while(pOF != NULL) {
		pCB = SearchCopyBack_t(pOF->aszNWFullpath);
		if(pCB->iCopyBack_flg == ON && pCB->uiTasknum != NULL) {
			pCB->iDelay_flg = ON;
			ResumeThread(pCB->uiTasknum);
			bIsResume = ON;
			do {

				pTGroup->iSemaporCount--;
				SignalLocalSemaphore(uiCopyFile_sh);

				delay(SHORTWAITTIME);

				pTGroup->iSemaporCount++;
				WaitOnLocalSemaphore(uiCopyFile_sh);

			} while( ((pCB = SearchCopyBack_t(pOF->aszNWFullpath)) !=
											(struct GCopyBack_t *)  NULL)
						&& pCB->iCopyBack_flg == ON );
		}

		if (bIsResume == ON) {
			pOF = pTGroup->pOFile_sp;
			bIsResume = OFF;
		}
		else {
			pOF = pOF->pNextOF_t;
		}
	}

	pTGroup->iSemaporCount--;
	SignalLocalSemaphore(uiCopyFile_sh);

	return;
}

/*EnglishComment**************************************************************/
/* ID   = ConvertErrno                                                       */
/* name = Error number conversion                                            */
/* func = Converts a DFS errno into a NetWare errno.                         */
/* i/o  = AcsMode  :int  :inp: file access status                            */
/*        iDFSerrno:int  :inp: DFS error number                              */
/*	      iNWerrno :int *:out: NetWare error number                          */
/* return = none                                                             */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
void	ConvertErrno(int	AcsMode,
					int	iDFSerrno,
					int	*iNWerrno)
{
	switch(iDFSerrno) {
		case	DENOENT:
		case	DENOTDIR:
		case	DEISDIR:
		case	DENOENTRY:
			if((AcsMode == GW_FILEERASE && iDFSerrno == DENOENT) ||
			 (AcsMode == GW_FILEERASE && iDFSerrno == DEISDIR))
				*iNWerrno = ERR_NO_FILES_FOUND;
			else if (AcsMode == GW_FILERENAME)
				*iNWerrno = ERR_NO_FILES_FOUND;
			else
				*iNWerrno = ERR_INVALID_PATH;
			break;
		case	DEIO:
		case	DENXIO:
		case	DEXDEV:
		case	DENODEV:
		case	DEINVAL:
			*iNWerrno = ERR_FAILURE;
			break;
		case	DEAGAIN:
			if (AcsMode == GW_FILEOPEN)
				*iNWerrno = ERR_FILE_IN_USE;
			else if (AcsMode == GW_FILECREATEOPEN)
				*iNWerrno = ERR_FILE_IN_USE;
			else if (AcsMode == GW_FILEERASE)
				*iNWerrno = ERR_ALREADY_IN_USE;
			else if (AcsMode == GW_FILERENAME)
				*iNWerrno = ERR_ALL_FILES_IN_USE;
			else if (AcsMode == GW_FILECLOSE)
				*iNWerrno = ERR_FILE_IN_USE;
			else if (AcsMode == GW_DIRCREATE)
				*iNWerrno = ERR_FILE_IN_USE;
			else if (AcsMode == GW_DIRDEL)
				*iNWerrno = ERR_DIRECTORY_IN_USE;
			else
				*iNWerrno = ERR_ALREADY_IN_USE;
			break;
		case	DEBADF:
			*iNWerrno = ERR_INVALID_FILE_HANDLE;
			break;
		case	DENOMEM:
			*iNWerrno = ERR_NO_ALLOC_SPACE;
			break;
		case	DEACCES:
			if (AcsMode == GW_FILEOPEN)
				*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
			else if (AcsMode == GW_FILECREATEOPEN)
				*iNWerrno = ERR_NO_CREATE_PRIVILEGE;
			else if (AcsMode == GW_FILEERASE)
				*iNWerrno = ERR_NO_DELETE_PRIVILEGE;
			else if (AcsMode == GW_FILERENAME)
				*iNWerrno = ERR_NO_RENAME_PRIVILEGE;
			else if (AcsMode == GW_FILECLOSE)
				*iNWerrno = ERR_NO_OPEN_PRIVILEGE;
			else if (AcsMode == GW_DIRCREATE)
				*iNWerrno = ERR_NO_CREATE_PRIVILEGE;
			else if (AcsMode == GW_DIRDEL)
				*iNWerrno = ERR_NO_DELETE_PRIVILEGE;
			else
				*iNWerrno = ERR_FAILURE;
			break;
		case	DEEXIST:
		case	DENOTEMPTY:
			*iNWerrno = ERR_DIRECTORY_NOT_EMPTY;
			break;
		case	DENFILE:
		case	DEMFILE:
		case	DENOSPC:
			*iNWerrno = ERR_NO_MORE_FILE_HANDLES;
			break;
		case	DENAMETOOLONG:
			*iNWerrno = ERR_INVALID_NAME_LENGTH;
			break;
		case	DECOMNOMEM:
			*iNWerrno = 1;
			break;
		case	DENOTTY:
			*iNWerrno = ERR_BAD_FILE_NAME;
			break;
		default:
			*iNWerrno = ERR_FAILURE;
			return;
	}
}
