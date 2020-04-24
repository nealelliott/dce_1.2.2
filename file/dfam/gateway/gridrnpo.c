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
 * $Log: gridrnpo.c,v $
 * Revision 1.1.2.3  1996/03/22  07:37:51  takano
 * 	modified the table deletion method in the POST routine.
 * 	[1996/03/22  07:34:17  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:08:57  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:08  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  08:15:55  takano
 * 	First Submit
 * 	[1996/02/21  08:15:50  takano]
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
/* ID   = Post_Rename_Move                                                   */
/* name = Redirection of file renaming (post-hook)                           */
/* func = Renames and moves files.                                           */
/* i/o  = prmecbs :RenameMoveEntryCallBackStruct*:inp: parameters for rename-*/
/*                                                     move callback routine */
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


int Post_Rename_Move(RenameMoveEntryCallBackStruct *prmecbs,LONG uiNWerrno_result)
{
	struct FileAccessList *pAcs_t=NULL;
	struct	GTGroup_t *pCur_TG	= NULL;

	if(prmecbs->connection == 0){
		return uiNWerrno_result;
	}

	pCur_TG = GetGTGroupTable(prmecbs->connection);
	if( pCur_TG == (struct GTGroup_t *)NULL ) {
		return uiNWerrno_result;
	}
	pCur_TG->iExistThread++;

	pAcs_t = SerchAccessList(prmecbs->connection, prmecbs->task, RENAME_ACCESS);
	if (pAcs_t != NULL)	{
		pAcs_t->bIsCalledPostHook = ON;
		SignalLocalSemaphore(pAcs_t->uiAcces_sh);
	}

	pCur_TG->iExistThread--;

	return uiNWerrno_result;
}
