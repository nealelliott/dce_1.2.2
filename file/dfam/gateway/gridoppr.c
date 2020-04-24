
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
#include "dfammsg.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dceapi.h"
#include "dfamprt.h"
#include "cmnprt.h"




extern int errno;

/*EnglishComment**************************************************************/
/* ID   = Pre_Open                                                           */
/* name = Redirection of file opening (pre-hook)                             */
/* func = Handles DFS opening, file copying, and generating copyback thread. */
/* i/o  = pofcbs :xOpenFileCallBackStruct *:inp: parameters for open callback*/
/*                                               routine                     */
/* return = 0     : normal end                                               */
/*          others: abnormal end (returns NetWareErrno)                      */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/


int Pre_Open(xOpenFileCallBackStruct *pofcbs)
{
	int		iCcode		= 0;
	int     iOldTGID	= 0;
	int		iDFS_fd		= -1;
	int		iThreadID	= 0;
	int		iDFSerrno	= 0;
	int		iRc;
	int		iRecAcsRgt_dmy = 0;
	int		iHookThreadID;
	short	iSet_flg = 0;
	short	iNoOpen_flg = 0;
	int		iOpen_md;
	long	iShare_md;
	BYTE    *pszNWFullpath			= NULL;
	BYTE	*pszVolName				= NULL;
	BYTE    *pszDFSFullpath			= NULL;
	struct	GCopyBack_t *pCB		= NULL;
	struct	GCopyBack_t *pCB_w		= NULL;
	struct	OpenFile_t *pOF			= NULL;
	struct	OpenFile_t *pOF_w		= NULL;
	struct	OpenFile_t *pOF_b		= NULL;
	struct	GTGroup_t *pCur_TG		= NULL;
	struct	DMAccessFile *pDMAccessFile;
	long 	uiObjID;
	char 	aszObjName[ MAX_NWUSERNAME ];
	char 	uname[ MAX_NWUSERNAME ];
	WORD 	usObjType;
	BYTE 	byLoginTime[7];
	char	aszResumeNWfile[_MAX_PATH + 1];
	int		iErrnoSave;
	int		iCopyBackThreadGroupID;

	if(pofcbs->connection == 0){
		return 0;
	}

	iHookThreadID = GetThreadID();
	iOldTGID = SetThreadGroupID(iMain_tgid);

	GW_tsh_functrace("Pre_Open",GW_FUNC_START,NULL);


	pszNWFullpath = GW_malloc(_MAX_PATH+1);
	if (pszNWFullpath == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszVolName = GW_malloc(_MAX_VOLUME+2);
	if (pszVolName == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszDFSFullpath = GW_malloc(DFSPATH_MAX+1);
	if (pszDFSFullpath == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	memset(pszNWFullpath,NULL,_MAX_PATH+1);
	memset(pszVolName,NULL,_MAX_VOLUME+2);
	memset(pszDFSFullpath,NULL,DFSPATH_MAX+1);

	iCcode 	= GetVolumeName( (int)pofcbs->volume, pszVolName);
	if ( iCcode != 0 ) {
		goto Err_exit2;
	}
	iCcode = isDFAMVolume(pszVolName,NULL);

	pCur_TG = GetGTGroupTable(pofcbs->connection);
	if( pCur_TG == (struct GTGroup_t *) NULL ) {
		if ( iCcode == 1 ) {
			iCcode =  ERR_NO_OPEN_PRIVILEGE;
			goto Err_exit2;
		} else {
			iCcode = 0;
			goto Err_exit2;
		}
	} else {
		if ( iCcode == 0 ) {
			iCcode = 0;
			goto Err_exit2;
		}
	}


	if ( pCur_TG->iLogoutStatus == ON) {
		iCcode = 0;
		goto Err_exit2;
	}

	pCur_TG->iExistThread++;

	iThreadID = SetThreadGroupID(pCur_TG->iTGrpID);

	pOF = (struct OpenFile_t *)GW_malloc(sizeof(struct OpenFile_t));
	if (pOF == NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit;
	}

	pOF->uiConnum = pofcbs->connection;
	pOF->uiTasknum = pofcbs->task;
	pOF->uiVolnum = pofcbs->volume;
	pOF->uiDirBase = pofcbs->dirBase;
	pOF->uiPathCompCnt = pofcbs->pathComponentCount;
	pOF->uiNameSpace = pofcbs->nameSpace;
	pOF->uiAttrMatchBits = pofcbs->attributeMatchBits;
	pOF->uiReqdAcsRights = pofcbs->requestedAccessRights;
	pOF->uiCreFlagBits = (LONG)0;
	pOF->uiDataStrmNum = pofcbs->dataStreamNumber;
	pOF->uiNWHandle = 0;
	pOF->uiClose_sh = OpenLocalSemaphore( (LONG)0 );

	pOF->iThreadID = iHookThreadID;

	pOF->iCopybackThreadStatus = COPYBACK_THREAD_IS_NOT_CREATED;

	pOF->iCopyBackTGID = BeginThreadGroup(CopyBack, NULL, ST_SIZE, pOF);
	iErrnoSave = errno;
	if (pOF->iCopyBackTGID == EFAILURE ) {
		switch(iErrnoSave) {
		case ENOMEM :
			iCcode = ERR_SERVER_OUT_OF_MEMORY;
			GW_tsh_log(iErrnoSave,"OPPR8");
			break;
		default :
			iCcode = ERR_FAILURE;
			GW_tsh_log(iErrnoSave, "OPPR9");
			break;
		}
		goto Err_exit;
	}

	iCopyBackThreadGroupID = pOF->iCopyBackTGID;

	while (pOF->iCopybackThreadStatus != COPYBACK_THREAD_IS_CREATED) {
		delay(SHORTWAITTIME);
	}

	iCcode = MapNWFullpath(	pofcbs->volume,
							pofcbs->dirBase,
							pofcbs->pathComponentCount,
							pofcbs->pathString,
							pszNWFullpath,
							NULL,
							NULL);


	if (iCcode != 0) {
		GW_tsh_log(iCcode,"OPPR1");
		goto Err_exit;
	}

	switch( GetDFSPath(pszNWFullpath, pszDFSFullpath,OPENMODE)) {
	case 0:
		break;
	case -2:
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit;
	default:
		iCcode = ERR_FAILURE;
		goto Err_exit;
	}


	switch((int)((pofcbs->requestedAccessRights) & O_mask)) {
		case 0x01:
			iOpen_md = DO_RDONLY;
			break;
		default:
			iOpen_md = DO_RDWR;
			break;
	}
	iShare_md =	( (long)(pofcbs->requestedAccessRights) & S_mask ) >> 2;

	pCur_TG->iSemaporCount++;
	WaitOnLocalSemaphore(uiCopyFile_sh);
	pCB = SearchCopyBack_t(pszNWFullpath);
	if(pCB != (struct GCopyBack_t *) NULL) {
		iSet_flg = SearchrequestedAccessRights(pCur_TG,pszNWFullpath,&iRecAcsRgt_dmy);

		if (iSet_flg == 1) {
			switch((int)(iRecAcsRgt_dmy & O_mask)) {
				case 0x01:
					iOpen_md |= DO_RDONLY;
					break;
				default:
					iOpen_md = DO_RDWR;
					break;
			}
			iShare_md |= ( (long)(iRecAcsRgt_dmy & S_mask )) >> 2;
		}
		iCcode = AddCopyBack_t(pszNWFullpath, iRecAcsRgt_dmy | pofcbs->requestedAccessRights);



		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);
		pCur_TG->iSemaporCount++;
		WaitOnLocalSemaphore(uiCopyFile_sh);
		if(pCB->iCopyBack_flg == ON ) {
			iNoOpen_flg = 1;
			pCB->iOpenreq_flg = ON;
			if (pCB->uiTasknum != NULL) {
				ResumeThread(pCB->uiTasknum);
			}
			do {
				pCur_TG->iSemaporCount--;
				SignalLocalSemaphore(uiCopyFile_sh);
				ThreadSwitch();
				pCur_TG->iSemaporCount++;
				WaitOnLocalSemaphore(uiCopyFile_sh);
				if (pCB->uiTasknum != NULL) {
					ResumeThread(pCB->uiTasknum);
				}
			} while( ((pCB = SearchCopyBack_t(pszNWFullpath)) !=
										(struct GCopyBack_t *)  NULL)
					&& pCB->iCopyBack_flg == ON );
		}
	}
	else {
		iCcode = AddCopyBack_t(pszNWFullpath,pofcbs->requestedAccessRights);

		if( iCcode < 0) {


			pCur_TG->iSemaporCount--;
			SignalLocalSemaphore(uiCopyFile_sh);

			iCcode = ERR_SERVER_OUT_OF_MEMORY;
			goto Err_exit;
		}
	}

	pCur_TG->iSemaporCount--;
	SignalLocalSemaphore(uiCopyFile_sh);


	if (ExamineLocalSemaphore(pCur_TG->uiFHandle_sh) < 1) {

		pCur_TG->iSemaporCount++;
		WaitOnLocalSemaphore(uiCopyFile_sh);

		pOF_w = pCur_TG->pOFile_sp;

		while (1) {

			if (pOF_w == NULL) {

				iCcode = ERR_NO_MORE_FILE_HANDLES;
				DecCopyBackCounter(pszNWFullpath, iRecAcsRgt_dmy | pofcbs->requestedAccessRights);
				DelCopyBack_t(pszNWFullpath);

				pCur_TG->iSemaporCount--;
				SignalLocalSemaphore(uiCopyFile_sh);

				goto Err_StatReq;
			}

			pCB_w = SearchCopyBack_t(pOF_w->aszNWFullpath);

			if (pCB_w == NULL) {
				pOF_w = pOF_w->pNextOF_t;
				continue;
			}

			if (pCB_w->iCopyBack_flg == ON) {

				pCB_w->iDelay_flg = ON;
				strcpy(aszResumeNWfile, pCB_w->aszNWFullpath);

				if (pCB_w->uiTasknum != 0) {
					ResumeThread(pCB_w->uiTasknum);
					break;
				}
				else {
					break;
				}
			}
			pOF_w = pOF_w->pNextOF_t;
		}

		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);

		while (1) {

			delay(SHORTWAITTIME);

			pCur_TG->iSemaporCount++;
			WaitOnLocalSemaphore(uiCopyFile_sh);

			pCB_w = SearchCopyBack_t(aszResumeNWfile);

			if ((pCB_w == NULL)
			  ||(pCB_w->iCopyBack_flg == OFF)) {
				pCur_TG->iSemaporCount--;
				SignalLocalSemaphore(uiCopyFile_sh);
				break;
			}

			pCur_TG->iSemaporCount--;
			SignalLocalSemaphore(uiCopyFile_sh);

		}
	}

	pCur_TG->iSemaporCount++;
	WaitOnLocalSemaphore(uiCopyFile_sh);

	pDMAccessFile = DMAddAccessFileList(pszNWFullpath, pOF->iCopyBackTGID);

	pCur_TG->iSemaporCount--;
	SignalLocalSemaphore(uiCopyFile_sh);

	if (pDMAccessFile == NULL)	{
		iCcode =  ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit;
	}

	WaitOnLocalSemaphore(pCur_TG->uiFHandle_sh);
	if(pCB != (struct GCopyBack_t *) NULL || iNoOpen_flg == 1) {

		iDFS_fd = DFSopenReq(	pszDFSFullpath,
								iOpen_md,
								iShare_md,
								&iDFSerrno,
								pCur_TG);

		if (iDFS_fd < 0) {

			pCur_TG->iSemaporCount++;
			WaitOnLocalSemaphore(uiCopyFile_sh);

			DecCopyBackCounter(pszNWFullpath, iRecAcsRgt_dmy | pofcbs->requestedAccessRights);
			DelCopyBack_t(pszNWFullpath);

			DMDeleteAccessFileList(pOF->iCopyBackTGID);

			pCur_TG->iSemaporCount--;
			SignalLocalSemaphore(uiCopyFile_sh);

			SignalLocalSemaphore(pCur_TG->uiFHandle_sh);
			switch(iDFSerrno) {
			case DEINVAL:
				iCcode = ERR_FAILURE;
				GW_tsh_log(iDFSerrno,"OPPR2");
				goto Err_exit;
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
				GW_Message(DFAM_CON,NULL,LOG_YES,KDDS11414,uname,iDFSerrno);
				iCcode = ERR_FAILURE;
		 		if (pCur_TG->iLogoutStatus == OFF) {
					pCur_TG->iLogoutStatus = ON;
					iRc = ResumeThread(pCur_TG->iThreadID);
					switch(iRc) {
					case ESUCCESS:
						iCcode = ERR_NO_OPEN_PRIVILEGE;
						GW_tsh_log(iCcode,"OPPR3");
						break;
					case EINVAL:
					case EWRNGKND:
					case EBADHNDL:
						iCcode = ERR_FAILURE;
						GW_tsh_log(iRc,"OPPR4");
						break;
					default:
						iCcode = ERR_FAILURE;
						GW_tsh_log(iRc,"OPPR5");
						break;
					}
				} else {
					ConvertErrno(GW_FILEOPEN,iDFSerrno,&iCcode);
				}
				break;
			default:
				ConvertErrno(GW_FILEOPEN,iDFSerrno,&iCcode);
				break;
			}

			goto Err_exit;
		}
	} else {

		pCur_TG->iSemaporCount++;
		WaitOnLocalSemaphore(uiCopyFile_sh);

		iDFS_fd = FileCopyFromDFS(	pszNWFullpath,
								  	pszDFSFullpath,
								  	pCur_TG,
									iOpen_md,
								 	iShare_md,
									&iCcode);

		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);

		if (iDFS_fd == -1) {
			SignalLocalSemaphore(pCur_TG->uiFHandle_sh);

			pCur_TG->iSemaporCount++;
			WaitOnLocalSemaphore(uiCopyFile_sh);

			DecCopyBackCounter(pszNWFullpath, iRecAcsRgt_dmy | pofcbs->requestedAccessRights);
			DelCopyBack_t(pszNWFullpath);

			DMDeleteAccessFileList(pOF->iCopyBackTGID);

			pCur_TG->iSemaporCount--;
			SignalLocalSemaphore(uiCopyFile_sh);

			if (iCcode == ERR_INVALID_PATH) {
				iCcode = 0;

				pOF->iCopybackThreadStatus = COPYBACK_THREAD_TERMINATE_REQUEST;
				SignalLocalSemaphore(pOF->uiClose_sh);

				while (pOF->iCopybackThreadStatus != COPYBACK_THREAD_IS_TERMINATED) {
					delay(SHORTWAITTIME);
				}

				CloseLocalSemaphore(pOF->uiClose_sh);
				GW_free(pOF, NULL);

				goto Err_StatReq;
			}
			goto Err_exit;
		}
	}

	strcpy( pOF->aszNWFullpath, pszNWFullpath);
	pOF->uiRealAcsRights = iRecAcsRgt_dmy | pofcbs->requestedAccessRights;
	pOF->iDFSHandle = (LONG)iDFS_fd;

Err_exit:
	if (iCcode == 0) {
		pCur_TG->iSemaporCount++;
		WaitOnLocalSemaphore(uiCopyFile_sh);

		if (pCur_TG->pOFile_sp == NULL) {
			pCur_TG->pOFile_sp = pOF;
		}
		else {
			pOF_b = pCur_TG->pOFile_sp;
			pOF_w = pCur_TG->pOFile_sp;
			do {
				pOF_b = pOF_w;
				pOF_w = pOF_w->pNextOF_t;
			} while (pOF_w != NULL);
			pOF_b->pNextOF_t = pOF;
		}
	}
	else {
		if (pOF != NULL) {

			if (pOF->iCopybackThreadStatus == COPYBACK_THREAD_IS_CREATED) {
				pOF->iCopybackThreadStatus = COPYBACK_THREAD_TERMINATE_REQUEST;
				SignalLocalSemaphore(pOF->uiClose_sh);

				while (pOF->iCopybackThreadStatus != COPYBACK_THREAD_IS_TERMINATED) {
					delay(SHORTWAITTIME);
				}
			}

			CloseLocalSemaphore(pOF->uiClose_sh);
			GW_free(pOF, NULL);
		}
	}

Err_StatReq:
		pCur_TG->iExistThread--;

Err_exit2:

	if (pszNWFullpath != (BYTE *) NULL)
		GW_free(pszNWFullpath,NULL);

	if (pszVolName != (BYTE *) NULL)
		GW_free(pszVolName,NULL);

	if (pszDFSFullpath != NULL)
		GW_free(pszDFSFullpath,NULL);



	GW_tsh_functrace("Pre_Open",GW_FUNC_END,iCcode);

	SetThreadGroupID(iOldTGID);

	return iCcode;
}
