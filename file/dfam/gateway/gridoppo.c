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
 * $Log: gridoppo.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:55  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:06  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:14:37  takano
 * 	First Submit
 * 	[1996/02/21  08:14:24  takano]
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




extern int errno;

/*EnglishComment**************************************************************/
/* ID   = Post_Open                                                          */
/* name = Redirection of file opening (post hook)                            */
/* func = Checks the results of opening, and saves a file handle.            */
/* i/o  = pofcbs:xOpenFileCallBackStruct *:inp: parameters for open callback */
/*                                              routine                      */
/*        uiNWerrno_result: LONG          :inp: results from NetWare         */
/* return = uiNWerrno_result                                                 */
/*                                                                           */
/* common = none                                                             */
/* note   = nonblocking                                                      */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/


int Post_Open( xOpenFileCallBackStruct *pofcbs, LONG uiNWerrno_result )
{
	struct	OpenFile_t *pOF		= NULL;
	struct	GTGroup_t *pCur_TG	= NULL;

	if(pofcbs->connection == 0){
		return uiNWerrno_result;
	}

	pCur_TG = GetGTGroupTable(pofcbs->connection);
	if( pCur_TG == (struct GTGroup_t *)NULL ) {
		return uiNWerrno_result;
	}
	pCur_TG->iExistThread++;

	pOF = SearchOpenFile_t_cb(pCur_TG, pofcbs);
	if(pOF == NULL) {
		pCur_TG->iExistThread--;
		return uiNWerrno_result;
	}

	if ( uiNWerrno_result == 0 ) {
		pOF->iOpenFailure_flg = OFF;
		pOF->uiNWHandle = *(pofcbs->puiFileHandle);
	} else {
		pOF->iOpenFailure_flg = ON;
		SignalLocalSemaphore(pOF->uiClose_sh);
	}

	pCur_TG->iExistThread--;

	if(ExamineLocalSemaphore(uiCopyFile_sh) < 1) {
		pCur_TG->iSemaporCount--;
		SignalLocalSemaphore(uiCopyFile_sh);
	}

	return uiNWerrno_result;
}

