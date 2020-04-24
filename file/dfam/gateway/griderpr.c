
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
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
/* ID   = Pre_Erase                                                          */
/* name = Redirection of file removing (pre-hook)                            */
/* func = Deletes a DFS file.                                                */
/* i/o  = pefcbs :EraseFileCallBackStruct *:inp: parameters for erase        */
/*                                               callback routine            */
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



int Pre_Erase(EraseFileCallBackStruct *pefcbs)
{
	int		iCcode		= 0;
	int     iOldTGID	= 0;
	int		iDFSerrno	= 0;
	int		iRc;
	int		iWildCard_flg = OFF;
	BYTE    *pszNWFullpath_w	= (BYTE *) NULL;
	BYTE    *pszNWFullpath		= (BYTE *) NULL;
	BYTE    *pszNWpath			= (BYTE *) NULL;
	BYTE	*pszVolName			= (BYTE *) NULL;
	BYTE    *pszDFSFullpath		= (BYTE *) NULL;
	struct	GCopyBack_t	*pCB 	= NULL;
	struct	GTGroup_t	*pCur_TG = NULL;
	struct	DMAccessFile *pDMAccessFile;
	DIR			*pDir			= NULL;
	DIR			*pDirEntry		= NULL;
	struct	stat		NW_fattr;
	struct	dstat		DFS_fattr;
	long 	uiObjID;
	char 	aszObjName[ MAX_NWUSERNAME ];
	char 	uname[ MAX_NWUSERNAME ];
	WORD 	usObjType;
	BYTE 	byLoginTime[7];
	struct	FileAccessList *pAcs_w = NULL;
	int		iCopyBackTGID;
	int		iErrnoSave;
	int		iHookThreadID;

	if(pefcbs->connection == 0){
		return 0;
	}

	iHookThreadID = GetThreadID();
	iOldTGID = SetThreadGroupID(iMain_tgid);


	GW_tsh_functrace("Pre_Erase",GW_FUNC_START,NULL);

	pszNWFullpath_w = GW_malloc(_MAX_PATH+NAME_MAX+1);
	if (pszNWFullpath_w == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszNWFullpath = GW_malloc(_MAX_PATH+1);
	if (pszNWFullpath == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszNWpath = GW_malloc(_MAX_PATH+1);
	if (pszNWpath == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszVolName = GW_malloc(_MAX_VOLUME+2);
	if (pszVolName == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszDFSFullpath = GW_malloc(DFSPATH_MAX+1);
	if (pszDFSFullpath == (BYTE *) NULL)	{
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	memset(pszNWFullpath_w,NULL,_MAX_PATH+NAME_MAX+1);
	memset(pszNWFullpath,NULL,_MAX_PATH+1);
	memset(pszNWpath,NULL,_MAX_PATH+1);
	memset(pszVolName,NULL,_MAX_VOLUME+2);
	memset(pszDFSFullpath,NULL,DFSPATH_MAX+1);

	iCcode 	= GetVolumeName( (int)pefcbs->volume, pszVolName);
	if ( iCcode != 0 ) {
		goto Err_exit2;
	}
	iCcode = isDFAMVolume(pszVolName,NULL);

	pCur_TG = GetGTGroupTable(pefcbs->connection);
	if( pCur_TG == NULL ) {
		if ( iCcode == 1 ) {
			iCcode = ERR_NO_DELETE_PRIVILEGE;
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

	SetThreadGroupID(pCur_TG->iTGrpID);

	pAcs_w = (struct FileAccessList *)GW_malloc(sizeof(struct FileAccessList));
	if (pAcs_w ==  NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit;
	}

	pAcs_w->uiConnNum = pefcbs->connection;
	pAcs_w->uiTaskNum = pefcbs->task;
	pAcs_w->iThreadID = iHookThreadID;
	pAcs_w->uiAcces_sh = OpenLocalSemaphore((LONG)0);
	pAcs_w->iAccessListThreadStatus = ACCESSLIST_THREAD_IS_NOT_CREATED;
	pAcs_w->bIsCalledPostHook = OFF;
	pAcs_w->iAccessType = ERASE_ACCESS;

	iCopyBackTGID = BeginThreadGroup(AccessListTableDel, NULL, ST_SIZE, pAcs_w);
	iErrnoSave = errno;
	if (iCopyBackTGID == EFAILURE ) {
		switch(iErrnoSave) {
			case ENOMEM :
				iCcode = ERR_SERVER_OUT_OF_MEMORY;
				GW_tsh_log(iCcode, "ERPR13");
				break;
			default :
				iCcode = ERR_FAILURE;
				GW_tsh_log(iErrnoSave, "ERPR14");
				break;
		}
		goto Err_exit;
	}

	while (pAcs_w->iAccessListThreadStatus != ACCESSLIST_THREAD_IS_CREATED) {
		delay(SHORTWAITTIME);
	}

	iCcode = MapNWFullpath(	pefcbs->volume,
							pefcbs->dirBase,
							pefcbs->pathComponentCount,
							pefcbs->pathString,
							pszNWFullpath_w,
							pszNWpath,
							&iWildCard_flg);

	if (iCcode != 0) {
		GW_tsh_log(iCcode,"ERPR1");
		goto Err_exit;
	}

 	if ( iWildCard_flg == ON ) {
		if ( ( pDir = opendir(pszNWFullpath_w) ) != NULL ) {
			while ( ( pDirEntry = readdir(pDir) ) != NULL ) {
 				if ((pDirEntry->d_attr & _A_SUBDIR) == _A_SUBDIR)
 					continue;
				memset(pszNWFullpath,NULL,_MAX_PATH+1);
				strcpy(pszNWFullpath,pszNWpath);
				strcat(pszNWFullpath,pDirEntry->d_name);

				switch( GetDFSPath(pszNWFullpath, pszDFSFullpath,OPENMODE)) {
				case 0:
					break;
				case -2:
					iCcode =  ERR_SERVER_OUT_OF_MEMORY;
					goto Err_exit;
				default:
					iCcode = ERR_FAILURE;
					GW_tsh_log(iCcode,"ERPR2");
					goto Err_exit;
				}

				pCur_TG->iSemaporCount++;
				WaitOnLocalSemaphore(uiCopyFile_sh);

				pCB = SearchCopyBack_t(pszNWFullpath);
				if(pCB != NULL) {
					pCB->iErasereq_flg = ON;
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
						if (pCB != NULL && pCB->uiTasknum == NULL &&
											    pCB->iCopyBack_flg != ON)
							break;
					} while( ((pCB = SearchCopyBack_t(pszNWFullpath)) !=
											(struct GCopyBack_t *)  NULL));
				}

				pCur_TG->iSemaporCount--;
				SignalLocalSemaphore(uiCopyFile_sh);

				if ( DFSstatReq(pszDFSFullpath,
								&DFS_fattr,
								&iDFSerrno,
								pCur_TG)) {
					switch(iDFSerrno) {
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
										GW_tsh_log(iCcode,"ERPR21");
										break;
									case EINVAL:
									case EWRNGKND:
									case EBADHNDL:
										GW_tsh_log(iRc,"ERPR22");
										break;
									default:
										GW_tsh_log(iRc,"ERPR23");
										break;
								}
								goto Err_exit;
							}
							goto Err_exit;
						default:
							ConvertErrno(GW_FILEERASE,iDFSerrno,&iCcode);
							goto Err_exit;
					}
				}
				else {
					if (stat(pszNWFullpath,&NW_fattr)) {
						iCcode = ERR_NO_FILES_FOUND;
						goto Err_exit;
					}
				}

				pCur_TG->iSemaporCount++;
				WaitOnLocalSemaphore(uiCopyFile_sh);

				pDMAccessFile = DMAddAccessFileList(pszNWFullpath, iCopyBackTGID);

				pCur_TG->iSemaporCount--;
				SignalLocalSemaphore(uiCopyFile_sh);

				if (pDMAccessFile == NULL) {
					iCcode =  ERR_SERVER_OUT_OF_MEMORY;
					goto Err_exit;
				}

				if ( DFSremoveReq(	pszDFSFullpath,
									&iDFSerrno,
									pCur_TG)) {
					switch(iDFSerrno) {
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
									iCcode = ERR_FAILURE;
									GW_tsh_log(iCcode,"ERPR3");
									break;
								case EINVAL:
								case EWRNGKND:
								case EBADHNDL:
									iCcode = ERR_FAILURE;
									GW_tsh_log(iRc,"ERPR4");
									break;
								default:
									iCcode = ERR_FAILURE;
									GW_tsh_log(iRc,"ERPR5");
									break;
							}
						}
						break;
					default:
						ConvertErrno(GW_FILEERASE,iDFSerrno,&iCcode);
						GW_tsh_log(iDFSerrno,"ERPR6");
						break;
					}

					pCur_TG->iSemaporCount++;
					WaitOnLocalSemaphore(uiCopyFile_sh);

					DMDeleteAccessFileList(iCopyBackTGID);

					pCur_TG->iSemaporCount--;
					SignalLocalSemaphore(uiCopyFile_sh);

					goto Err_exit;
				} else {
					SetDelmarkByName( pszNWFullpath );
				}
			}
			closedir(pDir);
			pDir = NULL;
 		}
 		else {
			iCcode = NetWareErrno;
			GW_tsh_log(iCcode,"ERPR7");
			goto Err_exit;
 		}
	} else {
		switch( GetDFSPath(pszNWFullpath_w, pszDFSFullpath,OPENMODE)) {
		case 0:
			break;
		case -2:
			iCcode =  ERR_SERVER_OUT_OF_MEMORY;
			goto Err_exit;
		default:
			iCcode =  ERR_FAILURE;
			GW_tsh_log(iCcode,"ERPR8");
			goto Err_exit;
		}

		pCur_TG->iSemaporCount++;
		WaitOnLocalSemaphore(uiCopyFile_sh);

		pCB = SearchCopyBack_t(pszNWFullpath_w);
		if(pCB != NULL) {
			pCB->iErasereq_flg = ON;
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
				if (pCB != NULL && pCB->uiTasknum == NULL &&
										    pCB->iCopyBack_flg != ON)
					break;
			} while( ((pCB = SearchCopyBack_t(pszNWFullpath_w)) !=
											(struct GCopyBack_t *) NULL));
		}

		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);

		if ( DFSstatReq(pszDFSFullpath,
						&DFS_fattr,
						&iDFSerrno,
						pCur_TG)) {
			switch(iDFSerrno) {
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
								GW_tsh_log(iCcode,"ERPR81");
								break;
							case EINVAL:
							case EWRNGKND:
							case EBADHNDL:
								GW_tsh_log(iRc,"ERPR82");
								break;
							default:
								GW_tsh_log(iRc,"ERPR83");
								break;
						}
						goto Err_exit;
					}
					goto Err_exit;
				default:
					ConvertErrno(GW_FILEERASE,iDFSerrno,&iCcode);
					goto Err_exit;
			}
		}
		else {
			if (stat(pszNWFullpath_w,&NW_fattr)) {
				iCcode = ERR_NO_FILES_FOUND;
				goto Err_exit;
			}
		}
		pCur_TG->iSemaporCount++;
		WaitOnLocalSemaphore(uiCopyFile_sh);

		pDMAccessFile = DMAddAccessFileList(pszNWFullpath, iCopyBackTGID);

		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);

		if (pDMAccessFile == NULL) {
			iCcode =  ERR_SERVER_OUT_OF_MEMORY;
			goto Err_exit;
		}

		if ( DFSremoveReq(	pszDFSFullpath,
							&iDFSerrno,
							pCur_TG)) {
			switch(iDFSerrno) {
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
				iCcode = ERR_NO_DELETE_PRIVILEGE;
				if (pCur_TG->iLogoutStatus == OFF) {
					pCur_TG->iLogoutStatus = ON;
					iRc = ResumeThread(pCur_TG->iThreadID);
					switch(iRc) {
						case ESUCCESS:
							iCcode = ERR_NO_DELETE_PRIVILEGE;
							GW_tsh_log(iCcode,"ERPR9");
							break;
						case EINVAL:
						case EWRNGKND:
						case EBADHNDL:
							iCcode = ERR_FAILURE;
							GW_tsh_log(iRc,"ERPR11");
							break;
						default:
							iCcode = ERR_FAILURE;
							GW_tsh_log(iRc,"ERPR12");
							break;
					}
				}
				break;
			default:
				ConvertErrno(GW_FILEERASE,iDFSerrno,&iCcode);
				break;
			}

			pCur_TG->iSemaporCount++;
			WaitOnLocalSemaphore(uiCopyFile_sh);

			DMDeleteAccessFileList(iCopyBackTGID);

			pCur_TG->iSemaporCount--;
			SignalLocalSemaphore(uiCopyFile_sh);

			goto Err_exit;

		} else {
			SetDelmarkByName( pszNWFullpath_w );
		}
	}

	strcpy(pAcs_w->aszAccessFilePathName_b, pszNWFullpath_w);

	pAcs_w->aszAccessFilePathName_a[0] = '\0';

	pCur_TG->iSemaporCount++;
	WaitOnLocalSemaphore(uiCopyFile_sh);

	AddAccessList(pAcs_w);

	pCur_TG->iSemaporCount--;
	SignalLocalSemaphore(uiCopyFile_sh);

Err_exit:

	pCur_TG->iExistThread--;

	if (iCcode != 0) {
		if (pAcs_w != NULL) {

			if (pAcs_w->iAccessListThreadStatus == ACCESSLIST_THREAD_IS_CREATED) {
				pAcs_w->iAccessListThreadStatus = ACCESSLIST_THREAD_TERMINATE_REQUEST;
				SignalLocalSemaphore(pAcs_w->uiAcces_sh);

				while (pAcs_w->iAccessListThreadStatus != ACCESSLIST_THREAD_IS_TERMINATED) {

					delay(SHORTWAITTIME);
				}
			}

			CloseLocalSemaphore(pAcs_w->uiAcces_sh);
			GW_free(pAcs_w, NULL);
		}
	}

Err_exit2:

	if (pDir != (DIR *) NULL)
		closedir(pDir);

	if (pszNWFullpath_w != (BYTE *) NULL)
		GW_free(pszNWFullpath_w,NULL);

	if (pszNWFullpath != (BYTE *) NULL)
		GW_free(pszNWFullpath,NULL);

	if (pszNWpath != (BYTE *) NULL)
		GW_free(pszNWpath,NULL);

	if (pszVolName != (BYTE *) NULL)
		GW_free(pszVolName,NULL);

	if (pszDFSFullpath != NULL)
		GW_free(pszDFSFullpath,NULL);


	GW_tsh_functrace("Pre_Erase",GW_FUNC_END,iCcode);

	SetThreadGroupID(iOldTGID);
	return iCcode;
}
