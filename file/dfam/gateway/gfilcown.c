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
 * $Log: gfilcown.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:02  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:24  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:55:32  takano
 * 	First Submit
 * 	[1996/02/21  07:55:26  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nwdir.h>
#include <nwenvrn.h>
#include <malloc.h>
#include <process.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "err.h"
#include "gfiler.h"


/*EnglishComment**************************************************************/
/*  ID   = fnChgowner                                                        */
/*                                                                           */
/* name = Changing the owner of file/subdirectory                            */
/* func = Changes the owner of the file or subdirectory designated by Client.*/
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


void fnChgowner( struct GTGroup_t     *plUtbl ,
                 int                  iTask_num ,
                 unsigned char        *pucBuf ,
                 unsigned long        pulBuflen ,
                 unsigned char        *pucRetbuf ,
                 unsigned long        *pulRetlength  )
{
	struct chgowner_data	*plChgOwn;
	char                    *pcDFSpath;
	int						iRet;
	int						DFS_errno;
	char					*Volume_path;
    char                    *aszDCEuid;
	struct TableKeep        *pTable;
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

	memset(pTable,0x00,sizeof(struct TableKeep));
	memmove(pTable->command_name,pucBuf,COMMLEN);
	pTable->task_num = iTask_num;
	pTable->plRet_st = ( struct RetHeader * )pucRetbuf;
	plChgOwn = ( struct chgowner_data * )pucBuf;
	if(plChgOwn->dir_name[0] == NULL)
	{
		(pTable->plRet_st)->DFAM_errno = NULLPATH;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
        GW_free( pTable,0 );
		return;
	}

	memmove((pTable->plRet_st)->command_name , plChgOwn->command_name , COMMLEN);
 	pWork = ( char * )(plChgOwn->dir_name);


	while((*pWork != '\/') && (*pWork != '\\') && (*pWork != '\0'))
	{
		pWork++;
	}


	if( (*pWork == '\0') || ( (*(++pWork)) == '\0') )
	{
		(pTable->plRet_st)->DFAM_errno = NOENT;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
        GW_free( pTable,0 );
		return;
	}

	Volume_path = GW_malloc( _MAX_PATH+1 );
	if( Volume_path == NULL)
	{
		(pTable->plRet_st)->DFAM_errno = GWMEMERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

		fnCommnErrLog( pucRetbuf );

        GW_free( pTable,0 );
		return;
	}

	for( i=0 ; ( (*(pWork+i) != '\0') || (i < _MAX_PATH) ) ; i++ )
	{
		*(Volume_path+i) = *(pWork+i);
	}
	*(Volume_path+i) = '\0';

    pcDFSpath = GW_malloc(DFSPATH_MAX+1);
	if( pcDFSpath == NULL)
	{
		(pTable->plRet_st)->DFAM_errno = GWMEMERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

		fnCommnErrLog( pucRetbuf );

        GW_free( pTable , 0 );
		GW_free( Volume_path , 0 );
		return;
	}




 	iRet = GetDFSPath( Volume_path , pcDFSpath , CHECKMODE );

	GW_free( Volume_path , 0 );

	switch( iRet )
	{
		case GWERR_NORMAL:

				aszDCEuid = GW_malloc( DFSGRP_MAX+1 );
				if( aszDCEuid == NULL)
				{
					(pTable->plRet_st)->DFAM_errno = GWMEMERR;
					(pTable->plRet_st)->ret_value  = FALSE;
					(pTable->plRet_st)->next_frame = FALSE;
					*pulRetlength = sizeof(struct RetHeader);

					fnCommnErrLog( pucRetbuf );

        			GW_free( pTable , 0 );
					GW_free( pcDFSpath , 0 );
					return;
				}




				iRet = GetDCEUserName( plChgOwn->own_name , aszDCEuid );

				if(iRet == 0)
				{
					break;
				}
				else if(iRet == -1)
				{
					(pTable->plRet_st)->DFAM_errno = NOUSER;
				}
				else if(iRet == -2)
				{
					(pTable->plRet_st)->DFAM_errno = NODFAMUSER;
				}
				else if(iRet == -3)
				{
					(pTable->plRet_st)->DFAM_errno = GWMEMERR;
				}
				else if(iRet == -4)
				{
					(pTable->plRet_st)->DFAM_errno = BINDERR;
				}
				else
				{
					(pTable->plRet_st)->DFAM_errno = FAILECHG;
				}

				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				fnCommnErrLog( pucRetbuf );

        		GW_free( pTable,0 );
				GW_free( aszDCEuid , 0 );
				GW_free( pcDFSpath , 0 );
				return;


		case GWERR_NOT_DFAMVOL:
				(pTable->plRet_st)->DFAM_errno = NOVOL;
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				fnCommnErrLog( pucRetbuf );

        		GW_free( pTable,0 );
				GW_free(pcDFSpath,0);
				return;
		case GWERR_NOMEM:
				(pTable->plRet_st)->DFAM_errno = GWMEMERR;
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				fnCommnErrLog( pucRetbuf );

        		GW_free( pTable,0 );
				GW_free(pcDFSpath,0);
				return;
		case GWERR_LONG:
				(pTable->plRet_st)->DFAM_errno = TOOLONG;
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				fnCommnErrLog( pucRetbuf );

        		GW_free( pTable,0 );
				GW_free(pcDFSpath,0);
				return;
		case GWERR_INVAL:
		case GWERR_NUSE:
		case GWERR_NW_NOENT:
				(pTable->plRet_st)->DFAM_errno = NOENT;
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				fnCommnErrLog( pucRetbuf );

        		GW_free( pTable,0 );
				GW_free(pcDFSpath,0);
				return;
		default:
				(pTable->plRet_st)->DFAM_errno = ABNORMAL;
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				fnCommnErrLog( pucRetbuf );

        		GW_free( pTable,0 );
				GW_free(pcDFSpath,0);
				return;
	}




	iRet = DFSchownReq( pcDFSpath , aszDCEuid , NULL , &DFS_errno , plUtbl );

	GW_free( pcDFSpath,0 );
	GW_free( aszDCEuid,0 );

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
			(pTable->plRet_st)->ret_value = FALSE;
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
