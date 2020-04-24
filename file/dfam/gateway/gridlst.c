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
 * $Log: gridlst.c,v $
 * Revision 1.1.2.3  1996/03/22  07:02:52  takano
 * 	Fixed the confliction between the Directory Synchronization and the File-Access.
 * 	[1996/03/22  06:59:43  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:08:54  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:06  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  08:13:50  takano
 * 	First Submit
 * 	[1996/02/21  08:13:45  takano]
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
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dceapi.h"
#include "dfamprt.h"
#include "cmnprt.h"




/*EnglishComment**************************************************************/
/* ID	= AccessListTableDel											     */
/*                                                                           */
/* name = Deleting a file access list (main routine)                         */
/* func = Deletes a file access list.                                        */
/*                                                                           */
/* i/o	= pAcs_t :struct FileAccessList * :inp: the address of the file      */
/*                                              access list                  */
/* common = none                                                             */
/* note   = none                                                             */
/* date = September 27, 1995  by Yoshiaki Okada                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/


void AccessListTableDel(struct FileAccessList *pAcs_t)
{
	struct	GTGroup_t *pCur_TG	= NULL;
	int		iThreadGroupID;
	int		iRc;

	pAcs_t->iAccessListThreadStatus = ACCESSLIST_THREAD_IS_CREATED;

	WaitOnLocalSemaphore(pAcs_t->uiAcces_sh);


	GW_tsh_functrace("AcsLstDel",GW_FUNC_START,255);

	if (pAcs_t->iAccessListThreadStatus == ACCESSLIST_THREAD_TERMINATE_REQUEST) {
		pAcs_t->iAccessListThreadStatus = ACCESSLIST_THREAD_IS_TERMINATED;
		goto Err_exit2;
	}

	pCur_TG = GetGTGroupTable(pAcs_t->uiConnNum);
	if ( pCur_TG == (struct GTGroup_t *)NULL) {
		GW_tsh_log(NetWareErrno,"ACSL21");
		GW_tsh_functrace("AcsLstDel",GW_FUNC_END,102);
		goto Err_exit;
	}
	pCur_TG->iExistThread++;

	iThreadGroupID = GetThreadGroupID();

	pCur_TG->iSemaporCount++;
	WaitOnLocalSemaphore(uiCopyFile_sh);

	iRc = DMDeleteAccessFileList(iThreadGroupID);

	pCur_TG->iSemaporCount--;
	SignalLocalSemaphore(uiCopyFile_sh);

	if (iRc != 0) {

		GW_tsh_log(iRc,"ACSL3");
	}

	pCur_TG->iExistThread--;

Err_exit:

	pCur_TG->iSemaporCount++;
	WaitOnLocalSemaphore(uiCopyFile_sh);

	DelAccessList(pAcs_t->uiConnNum,pAcs_t->aszAccessFilePathName_b,
					pAcs_t->aszAccessFilePathName_a);

	pCur_TG->iSemaporCount--;
	SignalLocalSemaphore(uiCopyFile_sh);

Err_exit2:
	GW_tsh_functrace("AcsLstDel",GW_FUNC_END,255);

	ExitThread(EXIT_THREAD, 0);
}
