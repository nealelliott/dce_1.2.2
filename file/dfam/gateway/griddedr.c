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
 * $Log: griddedr.c,v $
 * Revision 1.1.2.3  1996/03/22  07:35:17  takano
 * 	modified the table deletion method in the POST routine.
 * 	[1996/03/22  07:31:39  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:08:49  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:02  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  08:12:14  takano
 * 	First Submit
 * 	[1996/02/21  08:12:09  takano]
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
#include "dfammsg.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dceapi.h"
#include "dfamprt.h"
#include "cmnprt.h"




extern int errno;

/*EnglishComment**************************************************************/
/* ID   = Pre_Deletedir                                                      */
/* name = Redirection of deleting directory (pre-hook)                       */
/* func = Deletes a DFS directory.                                           */
/* i/o  = pddcbs :DeleteDirCallBackStruct *:inp: parameters for delete-      */
/*                                               directory callback          */
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


int Pre_Deletedir(DeleteDirCallBackStruct *pddcbs)
{
	int		iCcode		= 0;
	int     iOldTGID	= 0;
	int		iThreadID	= 0;
	int		iDFSerrno	= 0;
	int		iRc;
	BYTE	*pszNWFullpath		= NULL;
	BYTE	*pszVolName			= NULL;
	BYTE	*pszDFSFullpath		= NULL;
	struct	GTGroup_t *pCur_TG	= NULL;
	struct	DMAccessFile *pDMAccessFile;
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

	if(pddcbs->connection == 0){
		return 0;
	}

	iHookThreadID = GetThreadID();
	iOldTGID = SetThreadGroupID(iMain_tgid);

	GW_tsh_functrace("Pre_Deletedir",GW_FUNC_START,NULL);


	pszNWFullpath = GW_malloc(_MAX_PATH+1);
	if (pszNWFullpath == (BYTE *) NULL)	{
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszVolName = GW_malloc(_MAX_VOLUME+2);
	if (pszVolName == (BYTE *) NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	pszDFSFullpath = GW_malloc(DFSPATH_MAX+1);
	if (pszDFSFullpath == (BYTE *) NULL)	 {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit2;
	}

	memset(pszNWFullpath,NULL,_MAX_PATH+1);
	memset(pszVolName,NULL,_MAX_VOLUME+2);
	memset(pszDFSFullpath,NULL,DFSPATH_MAX+1);

	iCcode 	= GetVolumeName( (int)pddcbs->volume, pszVolName);
	if ( iCcode != 0 ) {
		goto Err_exit2;
	}
	iCcode = isDFAMVolume(pszVolName,NULL);

	pCur_TG = GetGTGroupTable(pddcbs->connection);
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
		iCcode =  0;
		goto Err_exit2;
	}
	pCur_TG->iExistThread++;

	iThreadID = SetThreadGroupID(pCur_TG->iTGrpID);

	pAcs_w = (struct FileAccessList *)GW_malloc(sizeof(struct FileAccessList));
	if (pAcs_w ==  NULL) {
		iCcode = ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit;
	}

	pAcs_w->uiConnNum = pddcbs->connection;
	pAcs_w->uiTaskNum = 0L;
	pAcs_w->iThreadID = iHookThreadID;
	pAcs_w->uiAcces_sh = OpenLocalSemaphore((LONG)0);
	pAcs_w->iAccessListThreadStatus = ACCESSLIST_THREAD_IS_NOT_CREATED;
	pAcs_w->bIsCalledPostHook = OFF;
	pAcs_w->iAccessType = DELETEDIR_ACCESS;

	iCopyBackTGID = BeginThreadGroup(AccessListTableDel, NULL, ST_SIZE, pAcs_w);
	iErrnoSave = errno;
	if (iCopyBackTGID == EFAILURE ) {
		switch(iErrnoSave) {
			case ENOMEM :
				iCcode = ERR_SERVER_OUT_OF_MEMORY;
				GW_tsh_log(iCcode, "DEDR6");
				break;
			default :
				iCcode = ERR_FAILURE;
				GW_tsh_log(iErrnoSave, "DEDR7");
				break;
		}
		goto Err_exit;
	}

	while (pAcs_w->iAccessListThreadStatus != ACCESSLIST_THREAD_IS_CREATED) {
		delay(SHORTWAITTIME);
	}

	iCcode = MapNWFullpath(	pddcbs->volume,
							pddcbs->dirBase,
							pddcbs->pathComponentCount,
							pddcbs->pathString,
							pszNWFullpath,
							NULL,
							NULL);
	if (iCcode != 0) {
		GW_tsh_log(iCcode,"DEDR1");
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
		GW_tsh_log(iCcode,"DEDR2");
		goto Err_exit;
	}

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
#ifdef DBUG
printf("DEL-DIR ResumeThread ERROR : ESUCCESS\n");
#endif
							iCcode = ERR_FAILURE;
							GW_tsh_log(iCcode,"DEDR21");
							break;
						case EINVAL:
						case EWRNGKND:
						case EBADHNDL:
							iCcode = ERR_FAILURE;
							GW_tsh_log(iRc,"DEDR22");
							break;
						default:
							iCcode = ERR_FAILURE;
							GW_tsh_log(iRc,"DEDR23");
							break;
					}
					goto Err_exit;
				}
				goto Err_exit;
			default:
				ConvertErrno(GW_DIRDEL,iDFSerrno,&iCcode);
				goto Err_exit;
		}
	}
	else {
		if ( stat(pszNWFullpath,&NW_fattr)) {
			ConvertErrno(GW_DIRDEL,iDFSerrno,&iCcode);
			goto Err_exit;
		}
	}

	if ((DFS_fattr.d_mode & DS_IFDIR) != DS_IFDIR) {

		iCcode = ERR_DIRECTORY_NOT_EMPTY;
		goto Err_exit;
	}

	pCur_TG->iSemaporCount++;
	WaitOnLocalSemaphore(uiCopyFile_sh);

	pDMAccessFile = DMAddAccessFileList(pszNWFullpath, iCopyBackTGID);

	pCur_TG->iSemaporCount--;
	SignalLocalSemaphore(uiCopyFile_sh);

	if (pDMAccessFile == NULL)	{
		iCcode =  ERR_SERVER_OUT_OF_MEMORY;
		goto Err_exit;
	}

	if ( DFSrmdirReq(	pszDFSFullpath,
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

			if(pCur_TG->iLogoutStatus == OFF) {
				pCur_TG->iLogoutStatus = ON;
				iRc = ResumeThread(pCur_TG->iThreadID);
				switch(iRc) {
					case ESUCCESS:
						iCcode = ERR_FAILURE;
						GW_tsh_log(iCcode,"DEDR3");
						break;
					case EINVAL:
					case EWRNGKND:
					case EBADHNDL:
						iCcode = ERR_FAILURE;
						GW_tsh_log(iRc,"DEDR4");
						break;
					default:
						iCcode = ERR_FAILURE;
						GW_tsh_log(iRc,"DEDR5");
						break;
				}
			}
			break;
		default:
			ConvertErrno(GW_DIRDEL,iDFSerrno,&iCcode);
			break;
		}

		pCur_TG->iSemaporCount++;
		WaitOnLocalSemaphore(uiCopyFile_sh);

		DMDeleteAccessFileList(iCopyBackTGID);

		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);

		goto  Err_exit;
	} else {
		SetDelmarkByName( pszNWFullpath );
	}

	strcpy(pAcs_w->aszAccessFilePathName_b, pszNWFullpath);
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

	if (pszNWFullpath != (BYTE *) NULL)
		GW_free(pszNWFullpath,NULL);

	if (pszVolName != (BYTE *) NULL)
		GW_free(pszVolName,NULL);

	if (pszDFSFullpath != NULL)
		GW_free(pszDFSFullpath,NULL);


	GW_tsh_functrace("Pre_Deletedir",GW_FUNC_END,NULL);

	SetThreadGroupID(iOldTGID);

	return iCcode;
}
