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
 * $Log: gfilrght.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:14  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:33  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:59:07  takano
 * 	First Submit
 * 	[1996/02/21  07:59:00  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <nwdir.h>
#include <nwenvrn.h>
#include <nwdir.h>
#include <niterror.h>
#include <nwbindry.h>
#include <process.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "err.h"
#include "gfiler.h"


/*EnglishComment**************************************************************/
/* ID   = fnDRights                                                          */
/*                                                                           */
/* name = Getting the effective trustee information (main routine)           */
/* func = Gets the login user's effective trustee corresponding to the path  */
/*        designated by Client.                                              */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        iTask_num   :int               :inp: task number                   */
/*        pucBuf      :unsigned char    *:inp: Gateway receive buffer        */
/*        pulBuflen   :unsigned long     :inp: size of Gateway receive buffer*/
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*          :                                                                */
/* return = none                                                             */
/*          :                                                                */
/* common = none                                                             */
/*          :                                                                */
/*  date = February 1, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void fnDRights(struct GTGroup_t *plUtbl,
               int               iTask_num,
               unsigned char    *pucBuf,
               unsigned long     pulBuflen,
               unsigned char    *pucRetbuf,
               unsigned long    *pulRetlength)
{
	struct dright_data            *plDrights;
	char                          *pcDFSpath;
	int                            iRet;
	int                            DFS_Errno;
	char                          *Volume_path;
	struct dstat                  *pBuffer;
	struct TableKeep              *pTable;
	struct RightsMapping           RightsMap;
	struct RightsMapping          *NW_kenrimap;
	unsigned int                   uiKyokabit;
	char                          *pWork;
	int                            i;
	unsigned long                  worklen;

	memset( pucRetbuf , 0x00 , *pulRetlength );
	NW_kenrimap = &RightsMap;

	pTable = GW_malloc(sizeof(struct TableKeep));
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
	pTable->plRet_st = (struct RetHeader *)pucRetbuf;
	pTable->plRet_rgdata = (struct RetRightsData *)(pucRetbuf+sizeof(struct RetHeader));
	memmove( pTable->command_name , pucBuf , COMMLEN);
	pTable->task_num = iTask_num;
	plDrights = (struct dright_data *)pucBuf;

	if(plDrights->path_name[0] == NULL)
	{
		memmove((pTable->plRet_st)->command_name,pucBuf,COMMLEN);
		(pTable->plRet_st)->DFAM_errno = NULLPATH;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		GW_free(pTable,0);
		pTable = NULL;
		return;
	}

	memmove((pTable->plRet_st)->command_name,plDrights->command_name,COMMLEN);
	pWork = (char *)(plDrights->path_name);


	while((*pWork != '\/') && (*pWork != '\\') && (*pWork != '\0'))
	{
		pWork++;
	}


	if((*pWork == '\0') || ((*(++pWork)) == '\0'))
	{
		(pTable->plRet_st)->DFAM_errno = NOENT;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		GW_free(pTable,0);
		return;
	}

	Volume_path = GW_malloc(_MAX_PATH+1);
	if( Volume_path == 0 )
	{
		(pTable->plRet_st)->DFAM_errno = GWMEMERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

		fnCommnErrLog( pucRetbuf );

		GW_free(pTable,0);
		return;
	}

	for( i=0 ; (( *(pWork+i) != '\0') && (i < _MAX_PATH) ) ; i++ )
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

		GW_free(Volume_path,0);
		GW_free(pTable,0);
		return;
	}




	iRet = GetDFSPath( Volume_path , pcDFSpath , CHECKMODE );

	GW_free( Volume_path , 0 );

	switch(iRet)
	{
		case GWERR_NORMAL:
				break;
		case GWERR_NOT_DFAMVOL:
				(pTable->plRet_st)->DFAM_errno = NOVOL;
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				fnCommnErrLog( pucRetbuf );

				GW_free(pcDFSpath,0);
				GW_free(pTable,0);
				return;
		case GWERR_NOMEM:
				(pTable->plRet_st)->DFAM_errno = GWMEMERR;
				(pTable->plRet_st)->ret_value = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				fnCommnErrLog( pucRetbuf );

				GW_free(pcDFSpath,0);
				GW_free(pTable,0);
				return;
		case GWERR_LONG:
				(pTable->plRet_st)->DFAM_errno = TOOLONG;
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				fnCommnErrLog( pucRetbuf );

				GW_free(pcDFSpath,0);
				GW_free(pTable,0);
				return;
		case GWERR_INVAL:
		case GWERR_NUSE:
		case GWERR_NW_NOENT:
				(pTable->plRet_st)->DFAM_errno = NOENT;
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				fnCommnErrLog( pucRetbuf );

				GW_free(pcDFSpath,0);
				GW_free(pTable,0);
				return;
		default:
				(pTable->plRet_st)->DFAM_errno = ABNORMAL;
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				fnCommnErrLog( pucRetbuf );

				GW_free(pcDFSpath,0);
				GW_free(pTable,0);
				return;
	}

	pBuffer = GW_malloc( sizeof(struct dstat) );
	if (pBuffer == NULL)
	{
		(pTable->plRet_st)->DFAM_errno = GWMEMERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

		fnCommnErrLog( pucRetbuf );

		GW_free(pcDFSpath,0);
		GW_free(pTable,0);
		return;
	}




	iRet = DFSstatReq( pcDFSpath , pBuffer , &DFS_Errno , plUtbl );

	if (iRet == LIB_ERR)
	{
		(pTable->plRet_st)->DFAM_errno = fnErrChange(DFS_Errno,plUtbl);
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

	    if( (pTable->plRet_st)->DFAM_errno == AGCONERR )
		{
			fnResumeThread( plUtbl , pTable->agterr );
		}

		fnCommnErrLog( pucRetbuf );

		GW_free(pcDFSpath,0);
		GW_free(pTable,0);
		GW_free(pBuffer,0);
		return;
	}

	if (pBuffer->d_mode & DS_IFDIR)
	{
		(pTable->plRet_rgdata)->dir_info = C_DIR;
	}
	else
	{
		(pTable->plRet_rgdata)->dir_info = C_FILE;
	}

	GW_free( pBuffer , 0 );




	uiKyokabit = DFSrightsReq( pcDFSpath , &DFS_Errno , plUtbl );

	GW_free( pcDFSpath , 0 );

	if ( uiKyokabit == LIB_ERR )
	{
		(pTable->plRet_st)->DFAM_errno = fnErrChange(DFS_Errno,plUtbl);
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

	    if( (pTable->plRet_st)->DFAM_errno == AGCONERR )
		{
			fnResumeThread( plUtbl , pTable->agterr );
		}

		fnCommnErrLog( pucRetbuf );

		GW_free( pTable , 0 );
		return;
	}

	if ((pTable->plRet_rgdata)->dir_info == C_DIR)
	{
		NW_kenrimap->mode = ACL_DIR;
	}
	else
	{
		NW_kenrimap->mode = ACL_FILE;
	}
	NW_kenrimap->ACL_list = uiKyokabit;




	iRet = GW_Rights_Map( NW_kenrimap );

	if( iRet != SUCCESS )
	{
		(pTable->plRet_st)->DFAM_errno = MODEERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

		fnCommnErrLog( pucRetbuf );

		GW_free( pTable , 0 );
		return;
	}




	iRet = fnKenriMap(NW_kenrimap->trustee_list,(pTable->plRet_rgdata)->rights_list);

	(pTable->plRet_st)->DFAM_errno = NORMAL;
	(pTable->plRet_st)->ret_value  = TRUE;
	(pTable->plRet_st)->next_frame = FALSE;
	*pulRetlength = (sizeof(struct RetHeader))+(sizeof(struct RetRightsData));

	GW_free( pTable , 0 );
}
