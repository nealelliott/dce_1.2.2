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
 * $Log: gridclpr.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:40  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:54  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:07:56  takano
 * 	First Submit
 * 	[1996/02/21  08:07:51  takano]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <nwtypes.h>
#include <errno.h>
#include <niterror.h>
#include <malloc.h>
#include <fshooks.h>
#include <signal.h>
#include <fcntl.h>
#include <nwsemaph.h>
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dceapi.h"
#include "dfamprt.h"
#include "cmnprt.h"




extern int errno;

/*EnglishComment**************************************************************/
/* ID   = Post_close                                                         */
/* name = Redirection of file closing (post hook)                            */
/* func = Starts a copyback thread.                                          */
/* i/o  = pcfcbs :CloseFileCallBackStruct * :inp: parameters for the close   */
/*                                                callback routine           */
/*        uiNWerrno_result: LONG          :inp: results from NetWare         */
/* return = 0     : normal end                                               */
/*          others: abnormal end (NetWare errno)                             */
/* common = none                                                             */
/* note   = none                                                             */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/



int Post_Close(CloseFileCallBackStruct *pcfcbs, LONG uiNWerrno_result)
{
	struct GTGroup_t	*pCur_TG	= NULL;
	struct OpenFile_t	*pOF		= NULL;

	if(pcfcbs->connection == 0){
		return uiNWerrno_result;
	}

	pCur_TG = GetGTGroupTable(pcfcbs->connection);
	if( pCur_TG == (struct GTGroup_t *)NULL)  {
		return uiNWerrno_result;
	}
	if ( pCur_TG->iLogoutStatus == ON) {
		pOF = SearchOpenFile_t_fd(pCur_TG,pcfcbs->fileHandle);
		if(pOF != NULL )
			SignalLocalSemaphore(pOF->uiClose_sh);
		return uiNWerrno_result;
	}

	pCur_TG->iExistThread++;

	pOF = SearchOpenFile_t_fd(pCur_TG,pcfcbs->fileHandle);
	if(pOF == NULL ){
		pCur_TG->iExistThread--;
		return uiNWerrno_result;
	} else {
		SignalLocalSemaphore(pOF->uiClose_sh);
	}
	pCur_TG->iExistThread--;

	return uiNWerrno_result;
}
