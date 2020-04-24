
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
/* ID   = Pre_Rename_Move                                                    */
/* name = Redirection of file renaming (pre-hook)                            */
/* func = Renames and moves files.                                           */
/* i/o  = prmecbs :RenameMoveEntryCallBackStruct*:inp: parameters for rename-*/
/*                                                     move callback routine */
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


int Pre_Rename_Move(RenameMoveEntryCallBackStruct *prmecbs)
{
	short	s_len		= 0;
	short	d_len		= 0;
	short	i,j;
	int		iCcode		= 0;
	int     iOldTGID	= 0;
	int		iThreadID	= 0;
	int		iDFSerrno	= 0;
	int		iRc;
	int		iWildCard_flg = OFF;
	int		iWildCard_flg2 = OFF;
	int		iCopyBackTGID;
	BYTE    *pszNWFullpath_s		= NULL;
	BYTE    *pszNWFullpath_d		= NULL;
	BYTE    *pszNWExceptCWDpath_s	= NULL;
	BYTE    *pszNWExceptCWDpath_d	= NULL;
	BYTE    *pszNWpath_s			= NULL;
	BYTE    *pszNWpath_d			= NULL;
	BYTE	*pszVolName				= NULL;
	BYTE    *pszDFSFullpath_s		= NULL;
	BYTE    *pszDFSFullpath_d		= NULL;
	BYTE	*ppath_s				= NULL;
	BYTE	*ppath_d				= NULL;
	BYTE	*ppath_w				= NULL;
	BYTE	*ppath_wd				= NULL;
	struct	GCopyBack_t *pCB		= NULL;
	struct	GTGroup_t *pCur_TG		= NULL;
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
	int		iErrnoSave;
	int		iHookThreadID;

	if(prmecbs->connection == 0){
		return 0;
	}

	iHookThreadID = GetThreadID();
	iOldTGID = SetThreadGroupID(iMain_tgid);


	GW_tsh_functrace("Pre_Rename_Move",GW_FUNC_START,NULL);

	pszNWFullpath_s = GW_malloc(_MAX_PATH+1);
	if (pszNWFullpath_s == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszNWFullpath_d = GW_malloc(_MAX_PATH+1);
	if (pszNWFullpath_d == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszNWExceptCWDpath_s = GW_malloc(_MAX_PATH+1);
	if (pszNWExceptCWDpath_s == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszNWExceptCWDpath_d = GW_malloc(_MAX_PATH+1);
	if (pszNWExceptCWDpath_d == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszNWpath_s = GW_malloc(_MAX_PATH+1);
	if (pszNWpath_s == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszNWpath_d = GW_malloc(_MAX_PATH+1);
	if (pszNWpath_d == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszVolName = GW_malloc(_MAX_VOLUME+2);
	if (pszVolName == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszDFSFullpath_s = GW_malloc(DFSPATH_MAX+1);
	if (pszDFSFullpath_s == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszDFSFullpath_d = GW_malloc(DFSPATH_MAX+1);
	if (pszDFSFullpath_d == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	memset(pszNWFullpath_s,NULL,_MAX_PATH+1);
	memset(pszNWFullpath_d,NULL,_MAX_PATH+1);
	memset(pszNWExceptCWDpath_s,NULL,_MAX_PATH+1);
	memset(pszNWExceptCWDpath_d,NULL,_MAX_PATH+1);
	memset(pszNWpath_s,NULL,_MAX_PATH+1);
	memset(pszNWpath_d,NULL,_MAX_PATH+1);
	memset(pszVolName,NULL,_MAX_VOLUME+2);
	memset(pszDFSFullpath_s,NULL,DFSPATH_MAX+1);
	memset(pszDFSFullpath_d,NULL,DFSPATH_MAX+1);

	iCcode 	= GetVolumeName((int)prmecbs->volume, pszVolName);
	if ( iCcode != 0 ) {
		goto Err_exit2;
	}

	iCcode = isDFAMVolume(pszVolName,NULL);

	pCur_TG = GetGTGroupTable(prmecbs->connection);
	if( pCur_TG == NULL ) {
		if ( iCcode == 1 ) {
			iCcode = ERR_NO_RENAME_PRIVILEGE;
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

	pAcs_w = (struct FileAccessList *)GW_malloc(sizeof(struct FileAccessList));
	if (pAcs_w == NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit;
	}

	pAcs_w->uiConnNum = prmecbs->connection;
	pAcs_w->uiTaskNum = prmecbs->task;
	pAcs_w->iThreadID = iHookThreadID;
	pAcs_w->uiAcces_sh = OpenLocalSemaphore((LONG)0);
	pAcs_w->iAccessListThreadStatus = ACCESSLIST_THREAD_IS_NOT_CREATED;
	pAcs_w->bIsCalledPostHook = OFF;
	pAcs_w->iAccessType = RENAME_ACCESS;

	iCopyBackTGID = BeginThreadGroup(AccessListTableDel, NULL, ST_SIZE, pAcs_w);
	iErrnoSave = errno;
	if (iCopyBackTGID == EFAILURE ) {
		switch(iErrnoSave) {
			case ENOMEM :
				iCcode = ERR_SERVER_OUT_OF_MEMORY;
				GW_tsh_log(iCcode, "RNPR9");
				break;
			default :
				iCcode = ERR_FAILURE;
				GW_tsh_log(iErrnoSave, "RNPR10");
				break;
		}
		goto Err_exit;
	}

	while (pAcs_w->iAccessListThreadStatus != ACCESSLIST_THREAD_IS_CREATED) {
		delay(SHORTWAITTIME);
	}

	iCcode = MapNWFullpath(	prmecbs->volume,
							prmecbs->dirBase,
							prmecbs->pathComponentCount,
							prmecbs->pathString,
							pszNWFullpath_s,
							pszNWExceptCWDpath_s,
							&iWildCard_flg);
	if (iCcode != 0) {
		GW_tsh_log(iCcode,"RNPR1");
		goto Err_exit;
	}

	iCcode = MapNWFullpath(	prmecbs->volume,
							prmecbs->newDirBase,
							prmecbs->originalNewCount,
							prmecbs->newPathString,
							pszNWFullpath_d,
							pszNWExceptCWDpath_d,
							&iWildCard_flg2);
	if (iCcode != 0) {
		GW_tsh_log(iCcode,"RNPR2");
		goto Err_exit;
	}

	if (iWildCard_flg == ON || iWildCard_flg2 == ON) {
		if ( ( pDir = opendir(pszNWFullpath_s) ) != NULL ) {
			while ( ( pDirEntry = readdir(pDir) ) != NULL ) {
				if ((pDirEntry->d_attr & _A_SUBDIR) == _A_SUBDIR)
 					continue;
				memset(pszNWpath_s,NULL,_MAX_PATH+1);
				memset(pszNWpath_d,NULL,_MAX_PATH+1);

				strcpy(pszNWpath_d,pszNWExceptCWDpath_d);
				s_len = strlen(pszNWExceptCWDpath_s);
				d_len = strlen(pszNWExceptCWDpath_d);
				ppath_w		= &(pDirEntry->d_name);
				ppath_s		= &(pszNWFullpath_s[s_len]);
				ppath_wd	= &(pszNWpath_d[d_len]);
				ppath_d		= &(pszNWFullpath_d[d_len]);
				for (i=0;i < 8;i++) {
					if (*ppath_d == '\0' || *ppath_d == '.')
						break;
					if (*ppath_d != '?') {
						*ppath_wd = *ppath_d;
						ppath_wd++;
						if (*ppath_d != '\0' && *ppath_d != '.')
							ppath_d++;
						if (*ppath_w != '\0' && *ppath_s != '?' && *ppath_w != '.')
							ppath_w++;
						if (*ppath_s != '\0' && *ppath_s != '.')
							ppath_s++;
					}
					else {
						if (*ppath_s != '?') {
							if (*ppath_w != '\0' && *ppath_w != '.') {
								*ppath_wd = *ppath_w;
								ppath_wd++;
							}
							if (*ppath_d != '\0' && *ppath_d != '.')
								ppath_d++;
							if (*ppath_w != '\0' && *ppath_w != '.')
								ppath_w++;
							if (*ppath_s != '\0' && *ppath_s != '.')
								ppath_s++;
						}
						else {
							if (*ppath_w != '\0' && *ppath_w != '.') {
								*ppath_wd = *ppath_w;
								ppath_wd++;
								if (*ppath_w != '\0' && *ppath_d != '.')
									ppath_w++;
							}
							if (*ppath_d != '\0' && *ppath_d != '.')
								ppath_d++;
							if (*ppath_s != '\0' && *ppath_s != '.')
								ppath_s++;
						}
					}
				}

				while(1) {
					if ((*ppath_d == '\0' || *ppath_d == '.') &&
						(*ppath_s == '\0' || *ppath_s == '.') &&
						(*ppath_w == '\0' || *ppath_w == '.'))
						break;
					if (*ppath_d != '\0' && *ppath_d != '.')
						ppath_d++;
					if (*ppath_s != '\0' && *ppath_s != '.')
						ppath_s++;
					if (*ppath_w != '\0' && *ppath_w != '.')
						ppath_w++;
				}

				if ((*ppath_d != '\0' && *ppath_d == '.')){
					*ppath_wd = *ppath_d;
					ppath_wd++;
					if (*ppath_d != '\0')
						ppath_d++;
					if (*ppath_s != '\0')
						ppath_s++;
					if (*ppath_w != '\0')
						ppath_w++;
				}

				for (j=0;j < 3;j++) {
					if (*ppath_d == '\0' || *ppath_d == '.')
						break;
					if (*ppath_d != '?') {
						*ppath_wd = *ppath_d;
						ppath_wd++;
						if (*ppath_d != '\0' && *ppath_d != '.')
							ppath_d++;
						if (*ppath_w != '\0' && *ppath_s != '?' && *ppath_w != '.')
							ppath_w++;
						if (*ppath_s != '\0' && *ppath_s != '.')
							ppath_s++;
					}
					else {
						if (*ppath_s != '?') {
							if (*ppath_w != '\0' && *ppath_w != '.') {
								*ppath_wd = *ppath_w;
								ppath_wd++;
							}
							if (*ppath_d != '\0' && *ppath_d != '.')
								ppath_d++;
							if (*ppath_w != '\0' && *ppath_w != '.')
								ppath_w++;
							if (*ppath_s != '\0' && *ppath_s != '.')
								ppath_s++;
						}
						else {
							if (*ppath_w != '\0' && *ppath_w != '.') {
								*ppath_wd = *ppath_w;
								ppath_wd++;
								if (*ppath_w != '\0' && *ppath_d != '.')
									ppath_w++;
							}
							if (*ppath_d != '\0' && *ppath_d != '.')
								ppath_d++;
							if (*ppath_s != '\0' && *ppath_s != '.')
								ppath_s++;
						}
					}
				}
				strcpy(pszNWpath_s,pszNWExceptCWDpath_s);
				strcat(pszNWpath_s,pDirEntry->d_name);
				switch( GetDFSPath(pszNWpath_s, pszDFSFullpath_s,OPENMODE) ) {
					case 0:
						break;
					case -2:
						iCcode =  ERR_SERVER_OUT_OF_MEMORY;
						goto Err_exit;
					default:
						iCcode = ERR_FAILURE;
						GW_tsh_log(iCcode,"RNPR21");
						goto Err_exit;
				}

				switch( GetDFSPath(pszNWpath_d, pszDFSFullpath_d,OPENMODE) ) {
					case 0:
						break;
					case -2:
						iCcode =  ERR_SERVER_OUT_OF_MEMORY;
						goto Err_exit;
					default:
						iCcode = ERR_FAILURE;
						GW_tsh_log(iCcode,"RNPR22");
						goto Err_exit;
				}

				pCur_TG->iSemaporCount++;
				WaitOnLocalSemaphore(uiCopyFile_sh);
				pCB = SearchCopyBack_t(pszNWpath_s);
				if(pCB != NULL) {
					pCB->iRenamereq_flg = ON;
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
					} while( ((pCB = SearchCopyBack_t(pszNWpath_s)) !=
											(struct GCopyBack_t *) NULL));
				}
				pCB = SearchCopyBack_t(pszNWpath_d);
				if(pCB != NULL) {
					pCB->iRenamereq_flg = ON;
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
					} while( ((pCB = SearchCopyBack_t(pszNWpath_d)) != NULL));
				}
				pCur_TG->iSemaporCount--;
				SignalLocalSemaphore(uiCopyFile_sh);

				if (strcmp(pszNWpath_d,pszNWpath_s) == 0) {
					iCcode = 0;

					pAcs_w->iAccessListThreadStatus = ACCESSLIST_THREAD_TERMINATE_REQUEST;

					SignalLocalSemaphore(pAcs_w->uiAcces_sh);

					while (pAcs_w->iAccessListThreadStatus != ACCESSLIST_THREAD_IS_TERMINATED) {

						delay(SHORTWAITTIME);
					}

					CloseLocalSemaphore(pAcs_w->uiAcces_sh);
					GW_free(pAcs_w, NULL);

					goto Err_exit;
				}

				if ( DFSstatReq(pszDFSFullpath_s,
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
										GW_tsh_log(iCcode,"RNPR221");
										break;
									case EINVAL:
									case EWRNGKND:
									case EBADHNDL:
										GW_tsh_log(iRc,"RNPR222");
										break;
									default:
										GW_tsh_log(iRc,"RNPR223");
										break;
								}
								goto Err_exit;
							}
							goto Err_exit;
						default:
							ConvertErrno(GW_FILERENAME,iDFSerrno,&iCcode);
							GW_tsh_log(iCcode,"RNPR224");
							goto Err_exit;
					}
				}
				else {
					if (stat(pszNWpath_s,&NW_fattr)) {
						iCcode = ERR_NO_FILES_FOUND;
						goto Err_exit;
					}
				}

				if ( DFSstatReq(pszDFSFullpath_d,
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
									GW_tsh_log(iCcode,"RNPR225");
									break;
								case EINVAL:
								case EWRNGKND:
								case EBADHNDL:
									GW_tsh_log(iRc,"RNPR226");
									break;
								default:
									GW_tsh_log(iRc,"RNPR227");
									break;
								}
								goto Err_exit;
							}
							goto Err_exit;
						case DENOENT:
							if (!stat(pszNWpath_d,&NW_fattr)) {
								iCcode = ERR_ALL_NAMES_EXIST;
								goto Err_exit;
							}
							break;
						default:
							ConvertErrno(GW_FILERENAME,iDFSerrno,&iCcode);
							goto Err_exit;
					}
				}
				else {
					iCcode = ERR_ALL_NAMES_EXIST;
					goto Err_exit;
				}

				pCur_TG->iSemaporCount++;
				WaitOnLocalSemaphore(uiCopyFile_sh);

				pDMAccessFile = DMAddAccessFileList(pszNWpath_s, iCopyBackTGID);

				if (pDMAccessFile == NULL) {
					iCcode =  ERR_SERVER_OUT_OF_MEMORY;

					pCur_TG->iSemaporCount--;
					SignalLocalSemaphore(uiCopyFile_sh);

					goto Err_exit;
				}

				pDMAccessFile = DMAddAccessFileList(pszNWpath_d, iCopyBackTGID);

				if (pDMAccessFile == NULL) {
					iCcode =  ERR_SERVER_OUT_OF_MEMORY;

					pCur_TG->iSemaporCount--;
					SignalLocalSemaphore(uiCopyFile_sh);

					goto Err_exit;
				}

				pCur_TG->iSemaporCount--;
				SignalLocalSemaphore(uiCopyFile_sh);

				if ( DFSrenameReq(	pszDFSFullpath_s,
									pszDFSFullpath_d,
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
										GW_tsh_log(iCcode,"RNPR23");
										break;
									case EINVAL:
									case EWRNGKND:
									case EBADHNDL:
										iCcode = ERR_FAILURE;
										GW_tsh_log(iRc,"RNPR24");
										break;
									default:
										iCcode = ERR_FAILURE;
										GW_tsh_log(iRc,"RNPR25");
										break;
								}
							}
							break;

						default:
							ConvertErrno(GW_FILERENAME,iDFSerrno,&iCcode);
							GW_tsh_log(iCcode,"RNPR26");
							break;
					}

					pCur_TG->iSemaporCount++;
					WaitOnLocalSemaphore(uiCopyFile_sh);

					DMDeleteAccessFileList(iCopyBackTGID);

					pCur_TG->iSemaporCount--;
					SignalLocalSemaphore(uiCopyFile_sh);

					goto Err_exit;

				} else {
					SetDelmarkByName(pszNWpath_s);
				}
			}
		}
		else {
			iCcode = NetWareErrno;
			GW_tsh_log(iCcode,"RNPR27");
			goto Err_exit;
		}
		closedir(pDir);
		pDir = NULL;
	}
	else {
		switch( GetDFSPath(pszNWFullpath_s, pszDFSFullpath_s,OPENMODE) ) {
			case 0:
				break;
			case -2:
				iCcode =  ERR_SERVER_OUT_OF_MEMORY;
				goto Err_exit;
			default:
				iCcode = ERR_FAILURE;
				GW_tsh_log(iCcode,"RNPR3");
				goto Err_exit;
		}

		switch( GetDFSPath(pszNWFullpath_d, pszDFSFullpath_d,OPENMODE) ) {
			case 0:
				break;
			case -2:
				iCcode =  ERR_SERVER_OUT_OF_MEMORY;
				goto Err_exit;
			default:
				iCcode = ERR_FAILURE;
				GW_tsh_log(iCcode,"RNPR4");
				goto Err_exit;
		}

		pCur_TG->iSemaporCount++;
		WaitOnLocalSemaphore(uiCopyFile_sh);
		pCB = SearchCopyBack_t(pszNWFullpath_s);
		if(pCB != NULL) {
			pCB->iRenamereq_flg = ON;
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
			} while( ((pCB = SearchCopyBack_t(pszNWFullpath_s)) !=
											(struct GCopyBack_t *) NULL));
		}

		pCB = SearchCopyBack_t(pszNWFullpath_d);
		if(pCB != NULL) {
			pCB->iRenamereq_flg = ON;
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
			} while( ((pCB = SearchCopyBack_t(pszNWFullpath_d)) != NULL));
		}
		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);

		if (strcmp(pszNWFullpath_d,pszNWFullpath_s) == 0) {
			if ( DFSstatReq(pszDFSFullpath_s,
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
									GW_tsh_log(iCcode,"RNPR41");
									break;
								case EINVAL:
								case EWRNGKND:
								case EBADHNDL:
									GW_tsh_log(iRc,"RNPR42");
									break;
								default:
									GW_tsh_log(iRc,"RNPR43");
									break;
							}
						}
						goto Err_exit;
					case DENOENT:
						iCcode = 0;

						pAcs_w->iAccessListThreadStatus = ACCESSLIST_THREAD_TERMINATE_REQUEST;

						SignalLocalSemaphore(pAcs_w->uiAcces_sh);

						while (pAcs_w->iAccessListThreadStatus != ACCESSLIST_THREAD_IS_TERMINATED) {

							delay(SHORTWAITTIME);
						}

						CloseLocalSemaphore(pAcs_w->uiAcces_sh);
						GW_free(pAcs_w, NULL);

						goto Err_exit;
					default:
						ConvertErrno(GW_FILERENAME,iDFSerrno,&iCcode);
						GW_tsh_log(iCcode,"RNPR43");
						goto Err_exit;
				}
			}
			iCcode = ERR_NO_RENAME_PRIVILEGE;
			goto Err_exit;
		}

		if ( DFSstatReq(pszDFSFullpath_s,
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
							GW_tsh_log(iCcode,"RNPR231");
							break;
						case EINVAL:
						case EWRNGKND:
						case EBADHNDL:
							GW_tsh_log(iRc,"RNPR232");
							break;
						default:
							GW_tsh_log(iRc,"RNPR233");
							break;
						}
						goto Err_exit;
					}
					goto Err_exit;
				default:
					ConvertErrno(GW_FILERENAME,iDFSerrno,&iCcode);
					goto Err_exit;
			}
		}
		else {
			if (stat(pszNWFullpath_s,&NW_fattr) != 0) {
				iCcode = ERR_ALL_NAMES_EXIST;
				goto Err_exit;
			}
		}

		if ( DFSstatReq(pszDFSFullpath_d,
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
							GW_tsh_log(iCcode,"RNPR234");
							break;
						case EINVAL:
						case EWRNGKND:
						case EBADHNDL:
							GW_tsh_log(iRc,"RNPR235");
							break;
						default:
							GW_tsh_log(iRc,"RNPR236");
							break;
						}
						goto Err_exit;
					}
					goto Err_exit;
				case DENOENT:
					if (stat(pszNWFullpath_d,&NW_fattr) == 0) {
						iCcode = ERR_ALL_NAMES_EXIST;
						goto Err_exit;
					}
					break;
				default:
					ConvertErrno(GW_FILERENAME,iDFSerrno,&iCcode);
					goto Err_exit;
			}
		}
		else {
			iCcode = ERR_ALL_NAMES_EXIST;
			goto Err_exit;
		}

		pCur_TG->iSemaporCount++;
		WaitOnLocalSemaphore(uiCopyFile_sh);

		pDMAccessFile = DMAddAccessFileList(pszNWFullpath_s, iCopyBackTGID);

		if (pDMAccessFile == NULL) {
			iCcode =  ERR_SERVER_OUT_OF_MEMORY;

			pCur_TG->iSemaporCount--;
			SignalLocalSemaphore(uiCopyFile_sh);

			goto Err_exit;
		}

		pDMAccessFile = DMAddAccessFileList(pszNWFullpath_d, iCopyBackTGID);

		if (pDMAccessFile == NULL) {
			iCcode =  ERR_SERVER_OUT_OF_MEMORY;

			pCur_TG->iSemaporCount--;
			SignalLocalSemaphore(uiCopyFile_sh);

			goto Err_exit;
		}

		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);

		if ( DFSrenameReq(	pszDFSFullpath_s,
							pszDFSFullpath_d,
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
							GW_tsh_log(iCcode,"RNPR5");
							break;
						case EINVAL:
						case EWRNGKND:
						case EBADHNDL:
							iCcode = ERR_FAILURE;
							GW_tsh_log(iRc,"RNPR6");
							break;
						default:
							iCcode = ERR_FAILURE;
							GW_tsh_log(iRc,"RNPR7");
							break;
						}
					}
					break;
				default:
					ConvertErrno(GW_FILERENAME,iDFSerrno,&iCcode);
					GW_tsh_log(iCcode,"RNPR8");
					break;
			}

			pCur_TG->iSemaporCount++;
			WaitOnLocalSemaphore(uiCopyFile_sh);

			DMDeleteAccessFileList(iCopyBackTGID);

			pCur_TG->iSemaporCount--;
			SignalLocalSemaphore(uiCopyFile_sh);

			goto Err_exit;

		} else {
			SetDelmarkByName(pszNWFullpath_s);
		}
	}

	strcpy(pAcs_w->aszAccessFilePathName_b, pszNWFullpath_s);
	strcpy(pAcs_w->aszAccessFilePathName_a, pszNWFullpath_d);

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

	if (pszNWFullpath_s != (BYTE *) NULL)
		GW_free(pszNWFullpath_s,NULL);

	if (pszNWFullpath_d != (BYTE *) NULL)
		GW_free(pszNWFullpath_d,NULL);

	if (pszNWExceptCWDpath_s != (BYTE *) NULL)
		GW_free(pszNWExceptCWDpath_s,NULL);

	if (pszNWExceptCWDpath_d != (BYTE *) NULL)
		GW_free(pszNWExceptCWDpath_d,NULL);

	if (pszNWpath_s != (BYTE *) NULL)
		GW_free(pszNWpath_s,NULL);

	if (pszNWpath_d != (BYTE *) NULL)
		GW_free(pszNWpath_d,NULL);

	if (pszVolName != (BYTE *) NULL)
		GW_free(pszVolName,NULL);

	if (pszDFSFullpath_s != NULL)
		GW_free(pszDFSFullpath_s,NULL);

	if (pszDFSFullpath_d != NULL)
		GW_free(pszDFSFullpath_d,NULL);


	GW_tsh_functrace("Pre_Rename_Move",GW_FUNC_END,iCcode);

	SetThreadGroupID(iOldTGID);
	return iCcode;
}
