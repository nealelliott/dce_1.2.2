
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
/* ID   = Pre_CreateOpen                                                     */
/* name = Redirection of file opening (pre-hook)                             */
/* func = Checks the results of pre-open.                                    */
/* i/o  = pcaocbs:xCreateAndOpenCallBackStruct *:inp: parameters for create- */
/*                                                    open callback routine  */
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


int Pre_CreateOpen(xCreateAndOpenCallBackStruct *pcaocbs)
{
	int		iCcode		= 0;
	int     iOldTGID	= 0;
	int		iDFS_fd		= -1;
	int		iThreadID	= 0;
	int		iDFSerrno	= 0;
	int		iRc;
	int		iHookThreadID;
	long	iShare_md 	= 0;
	BYTE    *pszNWFullpath			= NULL;
	BYTE	*pszVolName				= NULL;
	BYTE    *pszDFSFullpath			= NULL;
	struct	GCopyBack_t *pCB		= NULL;
	struct	GCopyBack_t *pCB_w		= NULL;
	struct	OpenFile_t *pOF			= NULL;
	struct	OpenFile_t *pOF_w		= NULL;
	struct	OpenFile_t *pOF_b		= NULL;
	struct	GTGroup_t *pCur_TG		= NULL;
	struct	stat		NW_fattr;
	struct	dstat		DFS_fattr;
	struct	DMAccessFile *pDMAccessFile;
	long 	uiObjID;
	char 	aszObjName[ MAX_NWUSERNAME ];
	char 	uname[ MAX_NWUSERNAME ];
	WORD 	usObjType;
	BYTE 	byLoginTime[7];
	char	aszResumeNWfile[_MAX_PATH + 1];
	int		iErrnoSave;
	int		iCopyBackThreadGroupID;

	if(pcaocbs->connection == 0){
		return 0;
	}

	iHookThreadID = GetThreadID();
	iOldTGID = SetThreadGroupID(iMain_tgid);

	GW_tsh_functrace("Pre_CreateOpen",GW_FUNC_START,NULL);

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

	iCcode 	= GetVolumeName( (int)pcaocbs->volume, pszVolName);
	if ( iCcode != 0 ) {
		goto Err_exit2;
	}
	iCcode = isDFAMVolume(pszVolName,NULL);

	pCur_TG = GetGTGroupTable(pcaocbs->connection);
	if( pCur_TG == NULL ) {
		if ( iCcode == 1 ) {
			iCcode = ERR_NO_CREATE_PRIVILEGE;
			goto Err_exit2;
		} else {
			iCcode = 0;
			goto Err_exit2;
		}
	} else {
		if ( iCcode == 0 ) {
			iCcode =  0;
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

	pOF->uiConnum = pcaocbs->connection;
	pOF->uiTasknum = pcaocbs->task;
	pOF->uiVolnum = pcaocbs->volume;
	pOF->uiDirBase = pcaocbs->dirBase;
	pOF->uiPathCompCnt = pcaocbs->pathComponentCount;
	pOF->uiNameSpace = pcaocbs->nameSpace;
	pOF->uiAttrMatchBits = pcaocbs->createAttributeBits;
	pOF->uiReqdAcsRights = pcaocbs->requestedAccessRights;
	pOF->uiRealAcsRights = pcaocbs->requestedAccessRights;
	pOF->uiCreFlagBits = pcaocbs->createFlagBits;
	pOF->uiDataStrmNum = pcaocbs->dataStreamNumber;
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
			GW_tsh_log(iErrnoSave,"COPR8");
			break;
		default :
			iCcode = ERR_FAILURE;
			GW_tsh_log(iErrnoSave, "COPR9");
			break;
		}
		goto Err_exit;
	}

	iCopyBackThreadGroupID = pOF->iCopyBackTGID;

	while (pOF->iCopybackThreadStatus != COPYBACK_THREAD_IS_CREATED) {
		delay(SHORTWAITTIME);
	}

	iCcode = MapNWFullpath(	pcaocbs->volume,
							pcaocbs->dirBase,
							pcaocbs->pathComponentCount,
							pcaocbs->pathString,
							pszNWFullpath,
							NULL,
							NULL);
	if (iCcode != 0) {
		goto Err_exit;
	}

	switch( GetDFSPath(pszNWFullpath, pszDFSFullpath,OPENMODE) ) {
	case 0:
		break;
	case -2:
		iCcode =  ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit;
	default:
		iCcode = ERR_FAILURE;
		GW_tsh_log(iCcode,"COPR1");
		goto Err_exit;
	}

	pCur_TG->iSemaporCount++;
	WaitOnLocalSemaphore(uiCopyFile_sh);

	iCcode = AddCopyBack_t(pszNWFullpath, pcaocbs->requestedAccessRights);
	if( iCcode < 0) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;

		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);

		goto Err_exit;
	}

	pCB = SearchCopyBack_t(pszNWFullpath);
	if(pCB != NULL) {
		if(pCB->iCopyBack_flg == ON ) {
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
	pCur_TG->iSemaporCount--;
	SignalLocalSemaphore(uiCopyFile_sh);

	if ( DFSstatReq(	pszDFSFullpath,
						&DFS_fattr,
						&iDFSerrno,
						pCur_TG)) {

		switch(iDFSerrno) {
			case DECOM:
			case DEREFUSEAGT:
			case DETIMEOUTAGT:
			case DEDFAMTERM:
			case DERESPONCEDATA:

				pCur_TG->iSemaporCount++;
				WaitOnLocalSemaphore(uiCopyFile_sh);

				DecCopyBackCounter(pszNWFullpath,pcaocbs->requestedAccessRights);
				DelCopyBack_t(pszNWFullpath);

				pCur_TG->iSemaporCount--;
				SignalLocalSemaphore(uiCopyFile_sh);

				memset(aszObjName,NULL,MAX_NWUSERNAME);
				memset(uname,NULL,MAX_NWUSERNAME);
				GetConnectionInformation( (WORD)(pCur_TG->uiConnNum), aszObjName,
								  &usObjType,  &uiObjID, byLoginTime );

    			if ( usObjType != OT_USER )
					uname[0] = '\0';
				else
					strcpy(uname,aszObjName );

				GW_Message(DFAM_NWC,(int *) &(pcaocbs->connection),LOG_YES,KDDS11413);
				GW_Message(DFAM_CON,NULL,LOG_YES,KDDS11414,uname,iDFSerrno);
				iCcode = ERR_FAILURE;
				if (pCur_TG->iLogoutStatus == OFF) {
					pCur_TG->iLogoutStatus = ON;
					iRc = ResumeThread(pCur_TG->iThreadID);
					switch(iRc) {
						case ESUCCESS:
							iCcode = ERR_NO_CREATE_PRIVILEGE;
							GW_tsh_log(iCcode,"COPR11");
							break;
						case EINVAL:
						case EWRNGKND:
						case EBADHNDL:
							iCcode = ERR_FAILURE;
							GW_tsh_log(iRc,"OPPR12");
							break;
						default:
							iCcode = ERR_FAILURE;
							GW_tsh_log(iRc,"OPPR13");
							break;
					}
					goto Err_exit;
				}
				goto Err_exit;
			case DENOENT:
				if (!stat(pszNWFullpath,&NW_fattr)) {

					pCur_TG->iSemaporCount++;
					WaitOnLocalSemaphore(uiCopyFile_sh);

					DecCopyBackCounter(pszNWFullpath,pcaocbs->requestedAccessRights);
					DelCopyBack_t(pszNWFullpath);

					pCur_TG->iSemaporCount--;
					SignalLocalSemaphore(uiCopyFile_sh);

					iCcode = ERR_NO_CREATE_PRIVILEGE;
					goto Err_exit;
				}
				break;
			default:

				pCur_TG->iSemaporCount++;
				WaitOnLocalSemaphore(uiCopyFile_sh);

				DecCopyBackCounter(pszNWFullpath,pcaocbs->requestedAccessRights);
				DelCopyBack_t(pszNWFullpath);

				pCur_TG->iSemaporCount--;
				SignalLocalSemaphore(uiCopyFile_sh);

				ConvertErrno(GW_FILECREATEOPEN,iDFSerrno,&iCcode);
				goto Err_exit;
		}
	}
	else {
		if ( stat(pszNWFullpath,&NW_fattr)) {
			iCcode = ERR_NO_CREATE_PRIVILEGE;

			pCur_TG->iSemaporCount++;
			WaitOnLocalSemaphore(uiCopyFile_sh);

			DecCopyBackCounter(pszNWFullpath,pcaocbs->requestedAccessRights);
			DelCopyBack_t(pszNWFullpath);

			pCur_TG->iSemaporCount--;
			SignalLocalSemaphore(uiCopyFile_sh);

			goto Err_exit;
		}
	}

	if (ExamineLocalSemaphore(pCur_TG->uiFHandle_sh) < 1) {

		pCur_TG->iSemaporCount++;
		WaitOnLocalSemaphore(uiCopyFile_sh);

		pOF_w = pCur_TG->pOFile_sp;

		while (1) {

			if (pOF_w == NULL) {

				iCcode = ERR_NO_MORE_FILE_HANDLES;
				DecCopyBackCounter(pszNWFullpath, pcaocbs->requestedAccessRights);
				DelCopyBack_t(pszNWFullpath);

				pCur_TG->iSemaporCount--;
				SignalLocalSemaphore(uiCopyFile_sh);

				goto Err_exit;
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
	iShare_md = DS_IRUSR | DS_IWUSR | DS_IXUSR |
				DS_IRGRP | DS_IWGRP | DS_IXGRP |
				DS_IROTH | DS_IXOTH;

	iDFS_fd = DFScreatReq(	pszDFSFullpath,
							iShare_md,
							&iDFSerrno,
							pCur_TG);

	if (iDFS_fd < 0) {

		pCur_TG->iSemaporCount++;
		WaitOnLocalSemaphore(uiCopyFile_sh);

		DMDeleteAccessFileList(pOF->iCopyBackTGID);

		DecCopyBackCounter(pszNWFullpath,pcaocbs->requestedAccessRights);
		DelCopyBack_t(pszNWFullpath);

		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);

		SignalLocalSemaphore(pCur_TG->uiFHandle_sh);
		switch(iDFSerrno) {
			case DEINVAL:
				GW_tsh_log(iDFSerrno,"COPR2");
				ConvertErrno(GW_FILECREATEOPEN,iDFSerrno,&iCcode);
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

				GW_Message(DFAM_NWC,(int *) &(pcaocbs->connection),LOG_YES,KDDS11413);
				GW_Message(DFAM_CON,NULL,LOG_YES,KDDS11414,uname,iDFSerrno);
				iCcode = ERR_NO_CREATE_PRIVILEGE;
				if (pCur_TG->iLogoutStatus == OFF) {
					pCur_TG->iLogoutStatus = ON;
					iRc = ResumeThread(pCur_TG->iThreadID);
					switch(iRc) {
						case ESUCCESS:
							iCcode = ERR_NO_CREATE_PRIVILEGE;
							GW_tsh_log(iRc,"COPR3");
							break;
						case EINVAL:
						case EWRNGKND:
						case EBADHNDL:
							iCcode = ERR_FAILURE;
							GW_tsh_log(iRc,"OPPR4");
							goto Err_exit;
						default:
							iCcode = ERR_FAILURE;
							GW_tsh_log(iRc,"OPPR5");
							goto Err_exit;
					}
				}
				else {
					GW_tsh_log(iDFSerrno,"OPPR6");
					iCcode = ERR_FAILURE;
				}
				break;
			default:
			GW_tsh_log(iDFSerrno,"OPPR7");
			ConvertErrno(GW_FILECREATEOPEN,iDFSerrno,&iCcode);
			break;
		}
		goto Err_exit;
	}

	strcpy( pOF->aszNWFullpath, pszNWFullpath);
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

	pCur_TG->iExistThread--;

Err_exit2:

	if (pszNWFullpath != (BYTE *) NULL)
		GW_free(pszNWFullpath,NULL);

	if (pszVolName != (BYTE *) NULL)
		GW_free(pszVolName,NULL);

	if (pszDFSFullpath != NULL)
		GW_free(pszDFSFullpath,NULL);

	GW_tsh_functrace("Pre_CreateOpen",GW_FUNC_END,iCcode);


	SetThreadGroupID(iOldTGID);
	return iCcode;
}
