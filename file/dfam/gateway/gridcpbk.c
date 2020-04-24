
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <nwtypes.h>
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
#include <nwbindry.h>
#include <ctype.h>
#include <nwconn.h>
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dceapi.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "cmnprt.h"




extern int errno;

/*EnglishComment**************************************************************/
/* ID   = CopyBack                                                           */
/* name = File copyback                                                      */
/* func = Handles file copyback, and file sparsing.                          */
/* i/o  = pOF :struct OpenFile_t *:inp:pointer to open file                  */
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


void CopyBack( struct OpenFile_t *pOF )
{
	int		iCcode = 0;
	int		iDFS_err = 0;
	int		iSparse_flg = ON;
	short	iDelay_end	= ON;
	BYTE    *pszDFSFullpath			= NULL;
	struct	GCopyBack_t	*pCB = NULL;
	struct	stat		NW_fattr;
	struct	utimbuf		NW_ftime;
	struct	dutimbuf	DFS_ftime;
	struct	GTGroup_t	*pCur_TG=NULL;
	long 	uiObjID;
	char 	aszObjName[ MAX_NWUSERNAME ];
	char 	uname[ MAX_NWUSERNAME ];
	WORD 	usObjType;
	BYTE 	byLoginTime[7];

	pOF->iCopybackThreadStatus = COPYBACK_THREAD_IS_CREATED;

	WaitOnLocalSemaphore(pOF->uiClose_sh);

	if (pOF->iCopybackThreadStatus == COPYBACK_THREAD_TERMINATE_REQUEST) {
		pOF->iCopybackThreadStatus = COPYBACK_THREAD_IS_TERMINATED;
		ExitThread(EXIT_THREAD, 0);
	}

	WaitOnLocalSemaphore(uiCopyFile_sh);

	GW_tsh_functrace("CopyBack",GW_FUNC_START,NULL);

	if (pOF == (struct OpenFile_t *) NULL) {
		SignalLocalSemaphore(uiCopyFile_sh);
		GW_tsh_log(NetWareErrno,"CPBKE");
		GW_tsh_functrace("CopyBackN",GW_FUNC_END,255);
		ExitThread(EXIT_THREAD, 0);
	}

	if ( pOF->iOpenFailure_flg == ON) {
		GW_tsh_log(NetWareErrno,"CPBK1");
		DecCopyBackCounter(pOF->aszNWFullpath, pOF->uiRealAcsRights);
		DelCopyBack_t((BYTE *)pOF->aszNWFullpath);
		DMDeleteAccessFileList(pOF->iCopyBackTGID);
		pCur_TG = GetGTGroupTable(pOF->uiConnum);
		if (pCur_TG != (struct GTGroup_t *) NULL) {

			pCur_TG->iExistThread++;
			if (pOF->iDFSHandle != -1) {
				DFScloseReq(pOF->iDFSHandle,
							&iDFS_err,
							pCur_TG);
				SignalLocalSemaphore(pCur_TG->uiFHandle_sh);
			}
			DelOpenFile_t(pOF,pCur_TG);
			pCur_TG->iExistThread--;

		}
		SignalLocalSemaphore(uiCopyFile_sh);
		GW_tsh_functrace("CopyBack1",GW_FUNC_END,0);
		ExitThread(EXIT_THREAD, 0);
	}

	pCur_TG = GetGTGroupTable(pOF->uiConnum);
	if ( pCur_TG == (struct GTGroup_t *)NULL) {
		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11401);
		GW_tsh_log(NetWareErrno,"CPBK1");
		DecCopyBackCounter(pOF->aszNWFullpath, pOF->uiRealAcsRights);
		DelCopyBack_t((BYTE *)pOF->aszNWFullpath);
		DMDeleteAccessFileList(pOF->iCopyBackTGID);
		SignalLocalSemaphore(uiCopyFile_sh);
		GW_tsh_functrace("CopyBack2",GW_FUNC_END,0);
		ExitThread(EXIT_THREAD, 0);
	}
	pCur_TG->iExistThread++;
	pCur_TG->iSemaporCount++;

	pCB = SearchCopyBack_t(pOF->aszNWFullpath);
	if(pCB == NULL) {

		GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11401);
		GW_tsh_log(NetWareErrno,"CPBK3");
		if (pOF->iDFSHandle != -1) {
			DFScloseReq(pOF->iDFSHandle,
							&iDFS_err,
							pCur_TG);
			SignalLocalSemaphore(pCur_TG->uiFHandle_sh);
		}
		DMDeleteAccessFileList(pOF->iCopyBackTGID);
		iCcode = DelOpenFile_t(pOF,pCur_TG);
		pCur_TG->iExistThread--;

		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);

		GW_tsh_functrace("CopyBack21",GW_FUNC_END,0);
		ExitThread(EXIT_THREAD, 0);
	} else {
		if(pCB->iCopyBack_flg == ON ) {
			do {
				pCur_TG->iSemaporCount--;
				SignalLocalSemaphore(uiCopyFile_sh);
				ThreadSwitch();
				pCur_TG->iSemaporCount++;
				WaitOnLocalSemaphore(uiCopyFile_sh);
			} while(pCB->iCopyBack_flg == ON);
		}

		pCB->iCopyBack_flg = ON;

		if (usCopyBackDelayTime != 0 && pCur_TG->iLogoutStatus == OFF) {
			pCB->uiTasknum = GetThreadID();
			pCur_TG->iSemaporCount--;
			SignalLocalSemaphore(uiCopyFile_sh);
			delay(usCopyBackDelayTime);
			pCB->uiTasknum = NULL;
			pCur_TG->iSemaporCount++;
			WaitOnLocalSemaphore(uiCopyFile_sh);
		}

		if (pCur_TG->iLogoutStatus == ON) {
			iDelay_end = OFF;
		}
		else if (ExamineLocalSemaphore(pCur_TG->uiFHandle_sh) < 0) {
			iDelay_end = OFF;
		}
		else if ( pCB->iOpenreq_flg == ON ){
			pCB->iOpenreq_flg = OFF;
			iSparse_flg = OFF;
			iDelay_end = OFF;
		}
		else if ( pCB->iRenamereq_flg == ON){
				pCB->iRenamereq_flg = OFF;
				iSparse_flg = ON;
				iDelay_end = OFF;
		}
		else if ( pCB->iErasereq_flg == ON){
			DecCopyBackCounter(pOF->aszNWFullpath, pOF->uiRealAcsRights);
			if(pOF->iDFSHandle > 0) {
				if ( DFScloseReq(	pOF->iDFSHandle,
									&iDFS_err,
									pCur_TG)) {
					switch(iDFS_err) {
						case DEBADF:
							GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11401);
							GW_tsh_log(iDFS_err,"CPBK4");
							GW_tsh_functrace("CopyBack4",GW_FUNC_END,-1);
							DelCopyBack_t((BYTE *)pOF->aszNWFullpath);
							DMDeleteAccessFileList(pOF->iCopyBackTGID);
							iCcode = DelOpenFile_t(pOF,pCur_TG);
							pCur_TG->iExistThread--;
							pCur_TG->iSemaporCount--;
							SignalLocalSemaphore(pCur_TG->uiFHandle_sh);
							SignalLocalSemaphore(uiCopyFile_sh);
							ExitThread(EXIT_THREAD, -1);
						case DECOM:
						case DEREFUSEAGT:
						case DETIMEOUTAGT:
						case DEDFAMTERM:
						case DERESPONCEDATA:
							GW_tsh_log(iDFS_err,"CPBK41");
							if (pCur_TG->iLogoutStatus == OFF) {
								pCur_TG->iLogoutStatus = ON;
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
								iCcode = ResumeThread(pCur_TG->iThreadID);
								switch(iCcode) {
								case ESUCCESS:
									GW_tsh_log(iCcode,"CPBK42");
									break;
								case EINVAL:
								case EWRNGKND:
								case EBADHNDL:
									GW_tsh_log(iCcode,"CPBK43");
									break;
								default:
									GW_tsh_log(iCcode,"CPBK44");
									break;
								}
							}
							break;
						default:
							GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11401);
							GW_tsh_log(iDFS_err,"CPBK5");
							break;
					}
				}
				pOF->iDFSHandle = -1;
			}
			SignalLocalSemaphore(pCur_TG->uiFHandle_sh);
			pCB->iErasereq_flg = OFF;
			pCB->iCopyBack_flg = OFF;
			DelCopyBack_t((BYTE *)pOF->aszNWFullpath);
			goto Err_Exit;
		}
		else if ( pCB->iDelay_flg == ON){
			pCB->iDelay_flg = OFF;
			iDelay_end = OFF;
		}
		else;

		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);
	}


	pCur_TG->iSemaporCount++;
	WaitOnLocalSemaphore(uiCopyFile_sh);
	iCcode = DecCopyBackCounter(pOF->aszNWFullpath, pOF->uiRealAcsRights);
	if ( stat(pOF->aszNWFullpath,&NW_fattr)) {
		GW_tsh_functrace("CopyBack5",GW_FUNC_END,-1);
		GW_tsh_log(NetWareErrno,"CPBK6");
		pCB->iCopyBack_flg = OFF;
		if (pOF->iDFSHandle != -1) {
			DFScloseReq(pOF->iDFSHandle,
							&iDFS_err,
							pCur_TG);
		}
		DelCopyBack_t((BYTE *)pOF->aszNWFullpath);
		DMDeleteAccessFileList(pOF->iCopyBackTGID);
		iCcode = DelOpenFile_t(pOF,pCur_TG);
		pCur_TG->iSemaporCount--;
		pCur_TG->iExistThread--;
		SignalLocalSemaphore(pCur_TG->uiFHandle_sh);
		SignalLocalSemaphore(uiCopyFile_sh);
		ExitThread(EXIT_THREAD, -1);
	}

	NW_ftime.modtime = NW_fattr.st_mtime;

	DFS_ftime.modtime = (dtime_t)NW_fattr.st_mtime;
	DFS_ftime.actime = (dtime_t)NW_fattr.st_atime;

	switch(iCcode) {
		case 0:
			if ( pOF->uiRealAcsRights & O_WRITE_MODE ) {
				if (FileCopyFromNW(	pOF->aszNWFullpath,
								pOF->iDFSHandle,
								pCur_TG) != 0) {
					iDelay_end = OFF;
					GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11401);
					GW_tsh_log(iCcode,"CPBK70");
					break;
				}
			}
			break;
		case 1:
			if (FileCopyFromNW(	pOF->aszNWFullpath,
								pOF->iDFSHandle,
								pCur_TG) != 0) {
				iDelay_end = OFF;
				GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11401);
				GW_tsh_log(iCcode,"CPBK71");
				break;
			}
			break;
		case 3:
			if (FileCopyFromNW(	pOF->aszNWFullpath,
								pOF->iDFSHandle,
								pCur_TG) != 0)	 {
				iDelay_end = OFF;
				GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11401);
				GW_tsh_log(iCcode,"CPBK73");
				break;
			}
			break;
		default:
			break;
	}

	if (pCur_TG->iLogoutStatus == ON) {
		iDelay_end = OFF;
	}
	else if (ExamineLocalSemaphore(pCur_TG->uiFHandle_sh) < 0) {
		iDelay_end = OFF;
	}
	else if ( pCB->iOpenreq_flg == ON ){
		pCB->iOpenreq_flg = OFF;
		iSparse_flg = OFF;
		iDelay_end = OFF;
	}
	else if ( pCB->iRenamereq_flg == ON){
			pCB->iRenamereq_flg = OFF;
			iSparse_flg = ON;
			iDelay_end = OFF;
	}
	else if ( pCB->iErasereq_flg == ON){
		DecCopyBackCounter(pOF->aszNWFullpath, pOF->uiRealAcsRights);
		if(pOF->iDFSHandle > 0) {
			if ( DFScloseReq(	pOF->iDFSHandle,
								&iDFS_err,
								pCur_TG)) {
				switch(iDFS_err) {
					case DEBADF:
						GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11401);
						GW_tsh_log(iDFS_err,"CPBK4");
						GW_tsh_functrace("CopyBack4",GW_FUNC_END,-1);
						DelCopyBack_t((BYTE *)pOF->aszNWFullpath);
						DMDeleteAccessFileList(pOF->iCopyBackTGID);
						iCcode = DelOpenFile_t(pOF,pCur_TG);
						pCur_TG->iExistThread--;
						pCur_TG->iSemaporCount--;
						SignalLocalSemaphore(pCur_TG->uiFHandle_sh);
						SignalLocalSemaphore(uiCopyFile_sh);
						ExitThread(EXIT_THREAD, -1);
					case DECOM:
					case DEREFUSEAGT:
					case DETIMEOUTAGT:
					case DEDFAMTERM:
					case DERESPONCEDATA:
						GW_tsh_log(iDFS_err,"CPBK41");
						if (pCur_TG->iLogoutStatus == OFF) {
							pCur_TG->iLogoutStatus = ON;
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
							iCcode = ResumeThread(pCur_TG->iThreadID);
							switch(iCcode) {
							case ESUCCESS:
								GW_tsh_log(iCcode,"CPBK42");
								break;
							case EINVAL:
							case EWRNGKND:
							case EBADHNDL:
								GW_tsh_log(iCcode,"CPBK43");
								break;
							default:
								GW_tsh_log(iCcode,"CPBK44");
								break;
							}
						}
						break;
					default:
						GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11401);
						GW_tsh_log(iDFS_err,"CPBK5");
						break;
				}
			}
			pOF->iDFSHandle = -1;
		}
		SignalLocalSemaphore(pCur_TG->uiFHandle_sh);
		pCB->iErasereq_flg = OFF;
		pCB->iCopyBack_flg = OFF;
		DelCopyBack_t((BYTE *)pOF->aszNWFullpath);
		goto Err_Exit;
	}
	else if ( pCB->iDelay_flg == ON){
		pCB->iDelay_flg = OFF;
		iDelay_end = OFF;
	}

	if (iSparse_flg == ON && iDelay_end == ON) {
		if (iFileSparseDelayTime != 0 && pCur_TG->iLogoutStatus == OFF) {
			pCB->uiTasknum = GetThreadID();
			pCur_TG->iSemaporCount--;
			SignalLocalSemaphore(uiCopyFile_sh);
			delay(iFileSparseDelayTime*60000);
			pCB->uiTasknum = NULL;
			pCur_TG->iSemaporCount++;
			WaitOnLocalSemaphore(uiCopyFile_sh);
		}

		if ( pCB->iOpenreq_flg == ON ){
			pCB->iOpenreq_flg = OFF;
			iSparse_flg = OFF;
		}
		else if ( pCB->iRenamereq_flg == ON){
			pCB->iRenamereq_flg = OFF;
			iSparse_flg = ON;
		}
		else if ( pCB->iErasereq_flg == ON){
			pCB->iErasereq_flg = OFF;
			iSparse_flg = OFF;
		}
		else;
	}


	if(pOF->iDFSHandle > 0) {
		if ( DFScloseReq(	pOF->iDFSHandle,
							&iDFS_err,
							pCur_TG)) {
			switch(iDFS_err) {
				case DECOM:
				case DEREFUSEAGT:
				case DETIMEOUTAGT:
				case DEDFAMTERM:
				case DERESPONCEDATA:
					GW_tsh_log(iDFS_err,"CPBK45");
		 			if (pCur_TG->iLogoutStatus == OFF) {
						pCur_TG->iLogoutStatus = ON;
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
						iCcode = ResumeThread(pCur_TG->iThreadID);
						switch(iCcode) {
							case ESUCCESS:
								GW_tsh_log(iCcode,"CPBK42");
								break;
							case EINVAL:
							case EWRNGKND:
							case EBADHNDL:
								GW_tsh_log(iCcode,"CPBK43");
								break;
							default:
								GW_tsh_log(iCcode,"CPBK44");
								break;
						}
					}
				default:
					pCB->iCopyBack_flg = OFF;
					DelCopyBack_t((BYTE *)pOF->aszNWFullpath);
					GW_Message(DFAM_NWC,(int *) &(pOF->uiConnum),LOG_YES,KDDS11401);
					GW_tsh_log(iDFS_err,"CPBK8");
					GW_tsh_functrace("CopyBack6",GW_FUNC_END,-1);
					DMDeleteAccessFileList(pOF->iCopyBackTGID);
					iCcode = DelOpenFile_t(pOF,pCur_TG);
					pCur_TG->iExistThread--;
					pCur_TG->iSemaporCount--;
					SignalLocalSemaphore(pCur_TG->uiFHandle_sh);
					SignalLocalSemaphore(uiCopyFile_sh);
					ExitThread(EXIT_THREAD, -1);
			}
		}

		SignalLocalSemaphore(pCur_TG->uiFHandle_sh);
	}

	if ( stat(pOF->aszNWFullpath,&NW_fattr)) {
		pCB->iCopyBack_flg = OFF;
		DelCopyBack_t((BYTE *)pOF->aszNWFullpath);
		GW_tsh_functrace("CopyBackS",GW_FUNC_END,-1);
		GW_tsh_log(NetWareErrno,"CPBKST");
		DMDeleteAccessFileList(pOF->iCopyBackTGID);
		iCcode = DelOpenFile_t(pOF,pCur_TG);
		pCur_TG->iExistThread--;
		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);
		ExitThread(EXIT_THREAD, -1);
	}
	pszDFSFullpath = GW_malloc(DFSPATH_MAX+1);
	if (pszDFSFullpath == (BYTE *) NULL) {
		pCB->iCopyBack_flg = OFF;
		DelCopyBack_t((BYTE *)pOF->aszNWFullpath);
		GW_tsh_functrace("CopyBackD",GW_FUNC_END,-1);
		GW_tsh_log(NetWareErrno,"CPBKSU");
		DMDeleteAccessFileList(pOF->iCopyBackTGID);
		iCcode = DelOpenFile_t(pOF,pCur_TG);
		pCur_TG->iExistThread--;
		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);
		ExitThread(EXIT_THREAD, -1);
	}
	memset(pszDFSFullpath,NULL,DFSPATH_MAX+1);
	switch( GetDFSPath(pOF->aszNWFullpath, pszDFSFullpath, OPENMODE) ) {
		case 0:
			break;
		default:
			pCB->iCopyBack_flg = OFF;
			DelCopyBack_t((BYTE *)pOF->aszNWFullpath);
			GW_free(pszDFSFullpath,NULL);
			GW_tsh_functrace("CopyBackD",GW_FUNC_END,-1);
			GW_tsh_log(NetWareErrno,"CPBKSU");
			DMDeleteAccessFileList(pOF->iCopyBackTGID);
			iCcode = DelOpenFile_t(pOF,pCur_TG);
			pCur_TG->iExistThread--;
			pCur_TG->iSemaporCount--;
			SignalLocalSemaphore(uiCopyFile_sh);
			ExitThread(EXIT_THREAD, -1);
	}

	if (DFSutimeReq((char *) pszDFSFullpath, &DFS_ftime, &iDFS_err, pCur_TG)) {
		if (iDFS_err == DECOM || iDFS_err == DEREFUSEAGT ||
					iDFS_err == DETIMEOUTAGT || iDFS_err == DEDFAMTERM  || iDFS_err == DERESPONCEDATA) {
			GW_tsh_log(iDFS_err,"CPBKU1");
			if (pCur_TG->iLogoutStatus == OFF) {
				pCur_TG->iLogoutStatus = ON;
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
				iCcode = ResumeThread(pCur_TG->iThreadID);
				switch(iCcode) {
					case ESUCCESS:
						GW_tsh_log(iCcode,"CPBKU2");
						break;
					case EINVAL:
					case EWRNGKND:
					case EBADHNDL:
						GW_tsh_log(iCcode,"CPBKU3");
						break;
					default:
						GW_tsh_log(iCcode,"CPBKU4");
						break;
				}
			}
		}
		pCB->iCopyBack_flg = OFF;
		DelCopyBack_t((BYTE *)pOF->aszNWFullpath);
		GW_free(pszDFSFullpath,NULL);
		GW_tsh_functrace("CopyBackU",GW_FUNC_END,-1);
		GW_tsh_log(NetWareErrno,"CPBKSU");
		DMDeleteAccessFileList(pOF->iCopyBackTGID);
		iCcode = DelOpenFile_t(pOF,pCur_TG);
		pCur_TG->iExistThread--;
		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);
		ExitThread(EXIT_THREAD, 0);
	}
	GW_free(pszDFSFullpath,NULL);

	if ( (pCB->iRead_cnt == 0)
		 && (pCB->iWrite_cnt == 0)
		 && (iSparse_flg == ON) ) {
			MakeSparseFile(pOF);
			utime(pOF->aszNWFullpath,&NW_ftime);
	}

	pCB->iCopyBack_flg = OFF;
	DelCopyBack_t((BYTE *)pOF->aszNWFullpath);

Err_Exit:

	DMDeleteAccessFileList(pOF->iCopyBackTGID);
	iCcode = DelOpenFile_t(pOF,pCur_TG);
	pCur_TG->iSemaporCount--;
	SignalLocalSemaphore(uiCopyFile_sh);
	pCur_TG->iExistThread--;
	if (iCcode != 0) {
		GW_Message(DFAM_NWC,(int *) &(pCur_TG->uiConnNum),LOG_YES,KDDS11401);
		GW_tsh_log(iDFS_err,"CPBK10");
		GW_tsh_functrace("CopyBack7",GW_FUNC_END,-1);
		ExitThread(EXIT_THREAD, -1);
	}


	GW_tsh_functrace("CopyBack",GW_FUNC_END,-1);

	ExitThread(EXIT_THREAD, 0);
}
