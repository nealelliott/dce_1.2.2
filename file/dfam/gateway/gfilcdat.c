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
 * $Log: gfilcdat.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:02  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:23  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:55:09  takano
 * 	First Submit
 * 	[1996/02/21  07:55:05  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nwdir.h>
#include <nwenvrn.h>
#include <process.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "err.h"
#include "gfiler.h"


/*EnglishComment**************************************************************/
/*  ID   = fnChgdate                                                         */
/*                                                                           */
/* name = Changing the last update time and date                             */
/* func = Senrs Gateway a "change last update time and date," which was      */
/*        passed from Client.                                                */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: connection number             */
/*        iTask_num   :int               :inp: task number                   */
/*        pucBuf      :unsigned char *   :inp: Gateway receive buffer        */
/*        pulBuflen   :unsigned long     :inp: size of Gateway receive buffer*/
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*          :                                                                */
/* return =  none                                                            */
/*                                                                           */
/* common = none                                                             */
/*          :                                                                */
/* note   = none                                                             */
/*  date = April 17, 1995 by Jun Nakagawa                                    */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void fnChgdate(  struct GTGroup_t     *plUtbl ,
                 int                  iTask_num ,
                 unsigned char        *pucBuf ,
                 unsigned long        pulBuflen ,
                 unsigned char        *pucRetbuf ,
                 unsigned long        *pulRetlength  )
{
	struct chgdate_data		*plChgDate;
	char                    *pcDFSpath;
	int						iRet;
	int						DFS_errno;
	char					*Volume_path;
	struct TableKeep		*pTable;
	struct dutimbuf			utime;
	char                    *pWork;
	int                     i;
	unsigned long           worklen;

	pTable = GW_malloc( sizeof(struct TableKeep) );
	if(pTable == NULL)
	{
		worklen = pulBuflen;
		memmove(((struct RetHeader *)pucRetbuf)->command_name,pucBuf,COMMLEN);
		((struct RetHeader *)pucRetbuf)->DFAM_errno = GWMEMERR;
		((struct RetHeader *)pucRetbuf)->ret_value  = FALSE;
		((struct RetHeader *)pucRetbuf)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

		fnCommnErrLog( pucRetbuf );

		return;
	}

	memset( pTable , 0x00 , sizeof(struct TableKeep) );
	memmove( pTable->command_name , pucBuf , COMMLEN );
	pTable->task_num = iTask_num;
	pTable->plRet_st = ( struct RetHeader * )pucRetbuf;
	plChgDate = ( struct chgdate_data * )pucBuf;

	if( plChgDate->file_name[0] == NULL )
	{
		(pTable->plRet_st)->DFAM_errno = NULLPATH;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		GW_free( pTable,0 );
		return;
	}

	memmove((pTable->plRet_st)->command_name , plChgDate->command_name , COMMLEN);
	pWork = (char *)(plChgDate->file_name);


	while((*pWork != '\/') && (*pWork != '\\') && (*pWork != '\0'))
	{
		pWork++;
	}


	if( (*pWork == '\0') || ((*(++pWork)) == '\0') )
	{
		(pTable->plRet_st)->DFAM_errno = NOENT;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		GW_free( pTable,0 );
		pTable = NULL;
		return;
	}

	Volume_path = GW_malloc( _MAX_PATH+1 );
	if( Volume_path == 0 )
	{
		(pTable->plRet_st)->DFAM_errno = GWMEMERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

		fnCommnErrLog( pucRetbuf );

		GW_free( pTable,0 );
		return;
	}

	for( i=0 ; ( ( *(pWork+i) != '\0') || (i < _MAX_PATH) ) ; i++ )
	{
		*(Volume_path+i) = *(pWork+i);
	}
	*(Volume_path+i) = '\0';

    pcDFSpath = GW_malloc( DFSPATH_MAX+1 );
	if( pcDFSpath == 0 )
	{
		(pTable->plRet_st)->DFAM_errno = GWMEMERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

		fnCommnErrLog( pucRetbuf );

		GW_free( pTable,0 );
		GW_free( Volume_path , 0 );
		return;
	}




	iRet = GetDFSPath( Volume_path , pcDFSpath , CHECKMODE );

	GW_free( Volume_path , 0 );

	switch( iRet ){
		case GWERR_NORMAL:
			break;
		default:
			if(iRet == GWERR_NOT_DFAMVOL)
			{
				(pTable->plRet_st)->DFAM_errno = NOVOL;
			}
			else if(iRet == GWERR_NOMEM)
			{
				(pTable->plRet_st)->DFAM_errno = GWMEMERR;
			}
			else if(iRet ==  GWERR_LONG)
			{
				(pTable->plRet_st)->DFAM_errno = TOOLONG;
			}
			else if(iRet == GWERR_INVAL)
			{
				(pTable->plRet_st)->DFAM_errno = NOENT;
			}
			else if(iRet == GWERR_NUSE)
			{
				(pTable->plRet_st)->DFAM_errno = NOENT;
			}
			else if(iRet == GWERR_NW_NOENT)
			{
				(pTable->plRet_st)->DFAM_errno = NOENT;
			}
			else
			{
				(pTable->plRet_st)->DFAM_errno = ABNORMAL;
			}

			(pTable->plRet_st)->ret_value  = FALSE;
			(pTable->plRet_st)->next_frame = FALSE;
			*pulRetlength = sizeof(struct RetHeader);

			fnCommnErrLog( pucRetbuf );

			GW_free( pTable,0 );
			GW_free( pcDFSpath,0 );
			return;
	}

	utime.actime  = (dtime_t)(plChgDate->chg_date);
	utime.modtime = (dtime_t)(plChgDate->chg_date);




	iRet = DFSutimeReq( pcDFSpath , &utime , &DFS_errno , plUtbl );

	GW_free( pcDFSpath,0 );

	switch( iRet ){
		case SUCCESS:
			(pTable->plRet_st)->ret_value  = TRUE;
			(pTable->plRet_st)->DFAM_errno = NORMAL;
			break;
		case LIB_ERR:
			(pTable->plRet_st)->ret_value  = FALSE;
			(pTable->plRet_st)->DFAM_errno = fnErrChange( DFS_errno,plUtbl );
		    if( (pTable->plRet_st)->DFAM_errno == AGCONERR )
			{
				fnResumeThread( plUtbl , pTable->agterr );
			}
			break;
		default:
			(pTable->plRet_st)->ret_value  = FALSE;
			(pTable->plRet_st)->DFAM_errno = fnErrChange( DFS_errno,plUtbl );
		    if( (pTable->plRet_st)->DFAM_errno == AGCONERR )
			{
				fnResumeThread( plUtbl , pTable->agterr );
			}
			break;
	}
	(pTable->plRet_st)->next_frame = FALSE;
	*pulRetlength = (sizeof(struct RetHeader));

	fnCommnErrLog( pucRetbuf );

	GW_free( pTable,0 );
}
