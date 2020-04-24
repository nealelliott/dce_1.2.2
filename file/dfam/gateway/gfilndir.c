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
 * $Log: gfilndir.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:13  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:32  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:58:42  takano
 * 	First Submit
 * 	[1996/02/21  07:58:36  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <nwenvrn.h>
#include <malloc.h>
#include <process.h>
#include <nwdir.h>
#include <niterror.h>
#include <nwbindry.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "dfamer.h"
#include "err.h"
#include "gfiler.h"


/*EnglishComment**************************************************************/
/* ID   = fnDNdir                                                            */
/*                                                                           */
/* name = Displaying file information (main routine)                         */
/* func = Checks if the data sent from Client is "new information" or        */
/*        "continued information," then invokes a new function or continued  */
/*        function respctively.                                              */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        iTask_num   :int               :inp: task number                   */
/*        pucBuf      :unsigned char *   :inp: Gateway receive buffer        */
/*        pulBuflen   :unsigned long     :inp: size of Gateway receive buffer*/
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*          :                                                                */
/* return = none                                                             */
/*          :                                                                */
/* common = none                                                             */
/*          :                                                                */
/*  date = February 2, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void fnDNdir(struct GTGroup_t     *plUtbl,
			 int                  iTask_num,
			 unsigned char        *pucBuf,
			 unsigned long        pulBuflen,
			 unsigned char        *pucRetbuf,
			 unsigned long        *pulRetlength)
{
	struct TableKeep              **pTaskTb;
	struct RetNdirDataKeep  	  *pKeep;
	struct RetNdirDataKeep  	  *pUpdate;
	struct TableKeep              *pTable;
	struct RetHeader              *plHeader;

	memset(pucRetbuf,0x00,*pulRetlength);

    plHeader = (struct RetHeader *)pucRetbuf;

	if( plUtbl == 0 )
	{
		memmove( plHeader->command_name , pucBuf , COMMLEN );
		plHeader->DFAM_errno = INTERE;
		plHeader->ret_value  = FALSE;
		plHeader->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

		fnCommnErrLog( pucRetbuf );

		return;
	}

	pTaskTb = &(plUtbl->pTblKeep_sp);
	while(*pTaskTb != NULL )
	{
		if((*pTaskTb)->task_num == iTask_num)

		{
			pTable = *pTaskTb;
			if (pulBuflen == COMMLEN)
			{

				fnDNdirContinue(plUtbl,pTable,pucBuf,pulBuflen,pucRetbuf,pulRetlength);
			}

			else
			{
				*pTaskTb = pTable->pNext;

				pKeep = pTable->pData;
				while(pKeep != NULL)
				{
					pUpdate = pKeep->pNext;
					GW_free(pKeep,0);
					pKeep = NULL;
					pKeep = pUpdate;
				}

				GW_free(pTable,0);
				pTable = NULL;
				pTable = GW_malloc(sizeof(struct TableKeep));

				if(pTable == NULL)
				{
					memmove(plHeader->command_name,pucBuf,COMMLEN);
					plHeader->DFAM_errno = GWMEMERR;
					plHeader->ret_value = FALSE;
					plHeader->next_frame = FALSE;
					*pulRetlength = sizeof(struct RetHeader);

					fnCommnErrLog( pucRetbuf );

					return;
				}
				memset(pTable,0x00,sizeof(struct TableKeep));

				memmove(pTable->command_name,pucBuf,COMMLEN);
				pTable->task_num = iTask_num;

				pTable->pNext = plUtbl->pTblKeep_sp;
				plUtbl->pTblKeep_sp = pTable;

				fnDNdirNew( plUtbl , pTable , pucBuf , pulBuflen , pucRetbuf , pulRetlength );

				fnCommnErrLog( pucRetbuf );

			    if( plHeader->DFAM_errno == AGCONERR )
				{
					fnResumeThread( plUtbl , pTable->agterr );
				}
			}

			if(((pTable->plRet_st)->ret_value == FALSE) || ((pTable->plRet_st)->next_frame == FALSE))
			{
				fnDelTask(plUtbl,pTable);
			}
			return;
		}
		pTaskTb = &((*pTaskTb)->pNext);
	}

	if (pulBuflen == COMMLEN)
	{
		memmove( plHeader->command_name , pucBuf , COMMLEN );
		plHeader->DFAM_errno = CABNORM;
		plHeader->ret_value  = FALSE;
		plHeader->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

		fnCommnErrLog( pucRetbuf );

		return;
	}

	else
	{
		pTable = GW_malloc(sizeof(struct TableKeep));

		if(pTable == NULL)
		{
			memmove( plHeader->command_name , pucBuf , COMMLEN );
			plHeader->DFAM_errno = GWMEMERR;
			plHeader->ret_value = FALSE;
			plHeader->next_frame = FALSE;
			*pulRetlength = sizeof(struct RetHeader);

			fnCommnErrLog( pucRetbuf );

			return;
		}
		memset(pTable,0x00,sizeof(struct TableKeep));

		memmove( pTable->command_name , pucBuf , COMMLEN );
		pTable->task_num = iTask_num;

		pTable->pNext = plUtbl->pTblKeep_sp;
		plUtbl->pTblKeep_sp = pTable;

		fnDNdirNew( plUtbl , pTable , pucBuf , pulBuflen , pucRetbuf , pulRetlength );
	}

	fnCommnErrLog( pucRetbuf );

    if( plHeader->DFAM_errno == AGCONERR )
	{
		fnResumeThread( plUtbl , pTable->agterr );
	}

	if(((pTable->plRet_st)->ret_value == FALSE) || ((pTable->plRet_st)->next_frame == FALSE))
	{
		fnDelTask(plUtbl,pTable);
	}
}


/*EnglishComment**************************************************************/
/* ID   = fnDNdirContinue                                                    */
/*                                                                           */
/* name = Getting file information (processing for the continued information)*/
/* func = In the case that Client sent "continued information."              */
/*           Gets the continued information chained from the task table,     */
/*           then returns it to Client.                                      */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pucBuf      :unsigned char *   :inp: Gateway receive buffer        */
/*        pulBuflen   :unsigned long     :inp: size of Gateway receive buffer*/
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*          :                                                                */
/* return = none                                                             */
/*          :                                                                */
/* common = none                                                             */
/*          :                                                                */
/*  date = February 2, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void fnDNdirContinue(struct GTGroup_t *plUtbl,
                     struct TableKeep *pTable,
                     unsigned char    *pucBuf,
                     unsigned long     pulBuflen,
                     unsigned char    *pucRetbuf,
                     unsigned long    *pulRetlength)
{
	char                    	*pcHeaderLastData;
	struct RetNdirDataKeep  	*pDataTop;
	unsigned char               *indexp;
	struct GTGroup_t            *workp;
	unsigned long               worklen;

	workp = plUtbl;

	pTable->plRet_st = (struct RetHeader *)pucRetbuf;

	worklen = pulBuflen;

	pTable->plRet_ndata = (struct RetNdirData *)(pucRetbuf+sizeof(struct RetHeader));

	memmove((pTable->plRet_st)->command_name , pucBuf , COMMLEN );
	(pTable->plRet_st)->DFAM_errno = NORMAL;
	(pTable->plRet_st)->ret_value = TRUE;

	pDataTop = pTable->pData;

	pcHeaderLastData = pucRetbuf + (*pulRetlength) - sizeof(struct RetNdirData);

	(pTable->plRet_st)->next_frame = FALSE;

	while ( pDataTop != NULL )
	{

		if ((char *)((pTable->plRet_ndata)) <= pcHeaderLastData)
		{
			*(pTable->plRet_ndata) = pDataTop->data;
			(pTable->plRet_ndata)->next_data = FALSE;
			pTable->pData = pDataTop->pNext;
			GW_free(pDataTop,0);
			pDataTop = NULL;
			pDataTop = pTable->pData;

			indexp = pucRetbuf + ( sizeof( struct RetHeader ) );
			while( (struct RetNdirData *)indexp != pTable->plRet_ndata)
			{
				((struct RetNdirData *)indexp)->next_data = TRUE;
				indexp += sizeof( struct RetNdirData );
			}
			pTable->plRet_ndata++;
		}

		else
		{
			(pTable->plRet_st)->next_frame = TRUE;
			break;
		}

	}


	*pulRetlength = ((char *)(pTable->plRet_ndata))-pucRetbuf;

}


/*EnglishComment**************************************************************/
/* ID   = fnDNdirNew                                                         */
/*                                                                           */
/* name = Getting file information (processing for the new information)      */
/* func = In the case that Client sent "new information."                    */
/*           Handles wildcard characters.                                    */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pucBuf      :unsigned char *   :inp: Gateway receive buffer        */
/*        pulBuflen   :unsigned long     :inp: size of Gateway receive buffer*/
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*          :                                                                */
/* return = none                                                             */
/*          :                                                                */
/* common = none                                                             */
/*          :                                                                */
/*  date = February 2, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void fnDNdirNew(struct GTGroup_t     *plUtbl,
				struct TableKeep     *pTable,
				unsigned char        *pucBuf,
				unsigned long        pulBuflen,
				unsigned char        *pucRetbuf,
				unsigned long        *pulRetlength)
{
	struct dndir_data    *plDndir;
	char                 aTarget[3];
	int                  delimita;
	int                  iPathlength;
	unsigned long        worklen;
	int					 rtn;

	pTable->plRet_st = (struct RetHeader *)pucRetbuf;
	worklen = pulBuflen;
	pTable->plRet_ndata = (struct RetNdirData *)(pucRetbuf+sizeof(struct RetHeader));
	plDndir = (struct dndir_data *)pucBuf;
	if(plDndir->path_name[0] == NULL)
	{
		memmove((pTable->plRet_st)->command_name,plDndir->command_name,COMMLEN);
		(pTable->plRet_st)->DFAM_errno = NULLPATH;
		(pTable->plRet_st)->ret_value = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		return;
	}

	iPathlength = strlen(plDndir->path_name);
	aTarget[0] = '*';
	aTarget[1] = '?';
	aTarget[2] = '\0';


	if( D_strpbrk( plDndir->path_name , aTarget) == NULL )
	{
		delimita = iPathlength;
	}
	else
	{
		delimita = iPathlength -1 ;
	}

	memmove((pTable->plRet_st)->command_name,plDndir->command_name,COMMLEN);

	(pTable->plRet_st)->DFAM_errno = NORMAL;
	(pTable->plRet_st)->ret_value = TRUE;

	if( iPathlength > delimita )
	{



		fnDNdir_wd( plUtbl , plDndir->path_name , pTable , pucRetbuf , pulRetlength );
	}

	else
	{
		if( plDndir->option[0] == 'F' )
		{



			rtn = fnDNdir_op( plUtbl , pTable , (char *)plDndir->path_name , pucRetbuf , pulRetlength );


			if( (strlen(pucRetbuf+sizeof(struct RetHeader)+sizeof(unsigned short)) ) == 0 )
			{
				if( rtn != -1 )
				{
					if( rtn == -3 )
					{
						(pTable->plRet_st)->DFAM_errno = PATHERR;
					}
					else if( rtn == -4 )
					{
						(pTable->plRet_st)->DFAM_errno = NORIGHT;
					}
					else
					{
						(pTable->plRet_st)->DFAM_errno = NOENT;
					}
				}
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);
			}
		}
		else
		{



			rtn = fnDNdir_dirwd( (char *)plDndir->path_name , plUtbl , pTable , pucRetbuf , pulRetlength );

			if( rtn != NORMAL )
			{
				(pTable->plRet_st)->DFAM_errno = rtn;
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);
			}
		}
	}
}


/*EnglishComment**************************************************************/
/* ID   = fnDNdir_dirwd                                                      */
/*                                                                           */
/* name = Processing for "no wildcard characters"                            */
/* func = Handles the case of "no wildcard characters."                      */
/*                                                                           */
/* i/o  = pathp       :char *            :inp: designated directory          */
/*        plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*          :                                                                */
/* return = none                                                             */
/*          :                                                                */
/* common = none                                                             */
/*          :                                                                */
/*  date = February 2, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/

int fnDNdir_dirwd( 	char                *pathp,
				   	struct GTGroup_t    *plUtbl,
					struct TableKeep    *pTable,
					unsigned char       *pucRetbuf,
					unsigned long       *pulRetlength)
{
	struct dstat        	*pBuffer;
	dmode_t					wk_mode;
	unsigned char       	*wk_path;
	char                	*pWork;
	char                 	*pcDFSpath;
	int                  	iRet;
	int                  	DFS_Errno;
	char                 	*Volume_path;
	int						rtn;
	int						i;

	pWork = pathp;


	while((*pWork != '\/') && (*pWork != '\\') && (*pWork != '\0'))
	{
		pWork++;
	}


	if( (*pWork == '\0') || ((*(++pWork)) == '\0') )
	{
		return  NOENT;
	}

	Volume_path = GW_malloc( _MAX_PATH+1 );
	if( Volume_path == 0 )
	{
		return  GWMEMERR;
	}
	memset( Volume_path , 0x00 , _MAX_PATH+1 );

	for( i=0 ; (( *(pWork+i) != '\0') && (i < _MAX_PATH) ) ; i++ )
	{
		*(Volume_path+i) = *(pWork+i);
	}
	*(Volume_path+i) = '\0';

	pcDFSpath = GW_malloc( DFSPATH_MAX+1 );
	if( pcDFSpath == 0 )
	{
		GW_free( Volume_path , 0 );
		return  GWMEMERR;
	}
	memset( pcDFSpath , 0x00 , DFSPATH_MAX+1 );




	iRet = GetDFSPath( Volume_path , pcDFSpath , CHECKMODE );

	GW_free( Volume_path , 0 );

	if( iRet != GWERR_NORMAL )
	{
		GW_free(pcDFSpath,0);
		switch( iRet )
		{
			case GWERR_NOT_DFAMVOL:
									return  NOVOL;
			case GWERR_NOMEM:
									return  GWMEMERR;
			case GWERR_LONG:
									return  TOOLONG;
			case GWERR_INVAL:
			case GWERR_NUSE:
			case GWERR_NW_NOENT:
									return  NOENT;
			default:
									return  ABNORMAL;
		}
	}

	pBuffer = GW_malloc(sizeof(struct dstat));
	if (pBuffer == NULL)
	{
		GW_free( pcDFSpath , 0 );
		return  GWMEMERR;
	}
	memset( pBuffer , 0x00 , sizeof(struct dstat) );




	iRet = DFSstatReq( pcDFSpath , pBuffer , &DFS_Errno , plUtbl );

	GW_free( pcDFSpath , 0 );

	if (iRet == LIB_ERR)
	{
		GW_free( pBuffer , 0 );
		return  fnErrChange( DFS_Errno,plUtbl );
	}
	wk_mode = pBuffer->d_mode;

	GW_free(pBuffer,0);

	if ( wk_mode & DS_IFDIR )
	{
		wk_path = GW_malloc( NW_PATHSIZE+NW_PATHSIZE );
		if( wk_path == 0 )
		{
    	   	return  GWMEMERR;
		}
		memset( wk_path , 0x00 , NW_PATHSIZE+NW_PATHSIZE );

		strcpy( wk_path , pathp );
		strcat( wk_path , "\\*.*" );



		fnDNdir_wd( plUtbl , wk_path , pTable , pucRetbuf , pulRetlength );

		GW_free( wk_path ,0);
	}
	else
	{
		rtn = fnDNdir_op( plUtbl , pTable , pathp , pucRetbuf,pulRetlength );


		if( (strlen(pucRetbuf+sizeof(struct RetHeader)+sizeof(unsigned short)) ) == 0 )
		{
			if( rtn != -1 )
			{
				if( rtn == -3 )
				{
					(pTable->plRet_st)->DFAM_errno = PATHERR;
				}
				else if( rtn == -4 )
				{
					(pTable->plRet_st)->DFAM_errno = NORIGHT;
				}
				else
				{
					(pTable->plRet_st)->DFAM_errno = NOENT;
				}
			}
			(pTable->plRet_st)->ret_value  = FALSE;
			(pTable->plRet_st)->next_frame = FALSE;
			*pulRetlength = sizeof(struct RetHeader);
		}

	}
	return  NORMAL;
}


/*EnglishComment**************************************************************/
/* ID   = fnDNdir_wd                                                         */
/*                                                                           */
/* name = Processing for "wildcard characters"                               */
/* func = Handles the case that a wildcard character exists.                 */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pathp       :char *            :inp: designated directory path     */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*                                                 data                      */
/*          :                                                                */
/* return = none                                                             */
/*          :                                                                */
/* common = none                                                             */
/*          :                                                                */
/*  date = February 2, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/

void fnDNdir_wd( struct GTGroup_t 	*plUtbl ,
				 unsigned char 		*pathp,
				 struct TableKeep   *pTable,
				 unsigned char      *pucRetbuf,
				 unsigned long      *pulRetlength)
{
	DIR                  *dirStructp;
	DIR                  *direntp;
	int                  NW_err_num;
	int                  iPathlength;
	unsigned char        *path_name;
	int   				 i;
	int					 rtn;
	int                  iValue;

	dirStructp = opendir( pathp );

	if( dirStructp == NULL )
	{
		NW_err_num = NetWareErrno;
		(pTable->plRet_st)->DFAM_errno = fnNW_change_errno(NW_err_num);
		if((pTable->plRet_st)->DFAM_errno == HEADONLY)
		{
			(pTable->plRet_st)->ret_value = TRUE;
		}
		else
		{
			(pTable->plRet_st)->ret_value = FALSE;
		}
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		return;
	}

	direntp = readdir( dirStructp );
	if( direntp == NULL )
	{
		if( errno == ENOENT )
       	{
			(pTable->plRet_st)->DFAM_errno = HEADONLY;
			(pTable->plRet_st)->ret_value  = TRUE;
		}
		else
		{
			NW_err_num = NetWareErrno;
			(pTable->plRet_st)->DFAM_errno = fnNW_change_errno(NW_err_num);
			(pTable->plRet_st)->ret_value = FALSE;
		}
		(pTable->plRet_st)->next_frame = FALSE;
		closedir(dirStructp);
		*pulRetlength = sizeof(struct RetHeader);
		return;
	}

	path_name = GW_malloc( NW_PATHSIZE+NW_PATHSIZE );
	if( path_name == 0 )
	{
		(pTable->plRet_st)->DFAM_errno = GWMEMERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
       	return;
	}

	memset( path_name , 0x00 , NW_PATHSIZE+NW_PATHSIZE );

	while ( direntp != NULL )
	{
	 	strcpy( path_name , pathp );
		iPathlength = strlen( path_name );
		for( i=iPathlength-1 ; i >= 0 ; i-- )
		{
			if( (*(path_name+i) == '\\') || (*(path_name+i) == ':') )
			{
				*(path_name+i+1) = '\0';
				break;
			}

		}
		strcat( path_name , direntp->d_name );




		rtn = fnDNdir_op( plUtbl , pTable , (char *)path_name , pucRetbuf , pulRetlength );

		if( rtn == -1 )
		{
			break;
		}




		direntp = readdir( dirStructp );

		if( direntp == NULL )
		{
			if( errno != ENOENT )
           	{
				GW_free( path_name , 0 );

				NW_err_num = NetWareErrno;
				(pTable->plRet_st)->DFAM_errno = fnNW_change_errno(NW_err_num);
				(pTable->plRet_st)->ret_value = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
           		closedir(dirStructp);
				*pulRetlength = sizeof(struct RetHeader);
               	return;
			}
       	}
	}

	GW_free( path_name , 0 );


	if( (strlen(pucRetbuf+sizeof(struct RetHeader)+sizeof(unsigned short)) ) == 0 )
	{
		if( rtn != -1 )
		{
			if( rtn == -3 )
			{
				(pTable->plRet_st)->DFAM_errno = PATHERR;
				(pTable->plRet_st)->ret_value  = FALSE;
			}
			else if( rtn == -4 )
			{
				(pTable->plRet_st)->DFAM_errno = NORIGHT;
				(pTable->plRet_st)->ret_value  = FALSE;
			}
			else
			{
				(pTable->plRet_st)->DFAM_errno = HEADONLY;
				(pTable->plRet_st)->ret_value  = TRUE;
			}
		}
		else
		{
			(pTable->plRet_st)->ret_value  = FALSE;
		}
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
	}




	iValue = closedir(dirStructp);

	if(iValue != ESUCCESS)
	{
		NW_err_num = NetWareErrno;
		(pTable->plRet_st)->DFAM_errno = fnNW_change_errno(NW_err_num);
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
   	   	return;
	}
}


/*EnglishComment**************************************************************/
/* ID   = fnDNdir_op                                                         */
/*                                                                           */
/* name = File information displaying                                        */
/* func = Sets data to be sent to Agent.                                     */
/*           - Converts file names                                           */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pcNetpath   :char *            :inp: NetWare path                  */
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*                                                 data                      */
/*          :                                                                */
/* return =  0: normal end                                                   */
/*          -1: abnormal end                                                 */
/*                                                                           */
/* common = none                                                             */
/*          :                                                                */
/*  date = May 10, 1995 by Jun Nakagawa                                      */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


int fnDNdir_op(	struct GTGroup_t    *plUtbl,
				struct TableKeep    *pTable,
				char                *pcNetpath,
				unsigned char       *pucRetbuf,
				unsigned long       *pulRetlength)
{
	struct RetNdirData         	*data_stp;
	char                       	*pcHeaderLastData;
	unsigned char              	*indexp;
	struct RetNdirDataKeep     	**icountp;
	int							iRet;

	data_stp = GW_malloc( sizeof(struct RetNdirData) );
	if( data_stp == 0 )
	{
		(pTable->plRet_st)->DFAM_errno = GWMEMERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		return (-1);
	}

	memset( data_stp , 0x00 , sizeof(struct RetNdirData) );

	iRet = fnDNdir_main( plUtbl , pcNetpath , data_stp );
	(pTable->plRet_st)->DFAM_errno = iRet;
	if( iRet != NORMAL )
	{
		GW_free( data_stp , 0 );

		if( iRet == NOENT )
		{
			return (-2);
		}
		if( iRet == PATHERR )
		{
			return (-3);
		}
		if( iRet == NORIGHT )
		{
			return (-4);
		}
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		return (-1);
	}

	(pTable->plRet_st)->ret_value = TRUE;

	pcHeaderLastData = pucRetbuf+(*pulRetlength)-sizeof(struct RetNdirData);
	if (((char *)(pTable->plRet_ndata)) <= pcHeaderLastData)
	{
		(pTable->plRet_st)->next_frame = FALSE;
		data_stp->next_data = FALSE;
		*(pTable->plRet_ndata) = *data_stp;
		indexp = pucRetbuf + ( sizeof( struct RetHeader) );
		while((struct RetNdirData *)indexp != pTable->plRet_ndata)
		{
			((struct RetNdirData *)indexp)->next_data = TRUE;
			indexp += sizeof( struct RetNdirData );
		}
		pTable->plRet_ndata++;
	}
	else
	{
		(pTable->plRet_st)->next_frame = TRUE;
		pTable->ppNDataKeep = &(pTable->pData);
		if( *(pTable->ppNDataKeep) == NULL)
		{
			*(pTable->ppNDataKeep) = GW_malloc(sizeof(struct RetNdirDataKeep));
			if (*(pTable->ppNDataKeep) == NULL)
			{
				(pTable->plRet_st)->DFAM_errno = GWMEMERR;
				(pTable->plRet_st)->ret_value = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				GW_free( data_stp , 0 );

				return (-1);
			}
			memset( *(pTable->ppNDataKeep) , 0x00 , sizeof(struct RetNdirDataKeep) );

			(*(pTable->ppNDataKeep))->data = *data_stp;
			(*(pTable->ppNDataKeep))->pNext = NULL;
		}

		else
		{
			icountp = &(pTable->pData);
			while (*icountp != NULL)
			{
				icountp = &((*icountp)->pNext);
			}
			*icountp = GW_malloc(sizeof(struct RetNdirDataKeep));
			if (*icountp == NULL)
			{
				(pTable->plRet_st)->DFAM_errno = GWMEMERR;
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				GW_free( data_stp , 0 );

				return (-1);
			}
			memset( *icountp , 0x00 , sizeof(struct RetNdirDataKeep) );

			(*icountp)->data = *data_stp;
			(*icountp)->pNext = NULL;
		}
	}

	GW_free( data_stp , 0 );

	*pulRetlength = ((char *)(pTable->plRet_ndata))-pucRetbuf;
	return 0;
}


/*EnglishComment**************************************************************/
/* ID   = fnDNdir_main                                                       */
/*                                                                           */
/* name = File information displaying (main routine)                         */
/* func = Sends a "get file information" request to Agent.                   */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pcNetpath   :char *            :inp: NetWare path                  */
/*        data_stp    :struct RetGrantData *:out: structure for Gateway's    */
/*                                                send data                  */
/*          :                                                                */
/* return = none                                                             */
/* common = none                                                             */
/*          :                                                                */
/*  date = January 31, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/

int fnDNdir_main( struct GTGroup_t *plUtbl , char *pcNetpath , struct RetNdirData *data_stp )
{
	char                       	*pWork;
	char                       	*Volume_path;
	int							i;
	int                        	iRet;
	struct dstat                *pBuffer;
	int                        	DFS_Errno;
	char                       	*aszNWUserName;
	unsigned int				uiKyokabit;
	struct RightsMapping       	RightsMap;

	pWork = pcNetpath;
	while((*pWork != '\/') && (*pWork != '\\') && (*pWork != '\0'))
	{
		pWork++;
	}

	if((*pWork == '\0') || ((*(++pWork)) == '\0'))
	{
		return NOENT;
	}

	Volume_path = GW_malloc( _MAX_PATH+1 );
	if( Volume_path == 0 )
	{
		return GWMEMERR;
	}
	memset( Volume_path , 0x00 , _MAX_PATH+1 );

	for( i=0 ; (( *(pWork+i) != '\0') && (i < _MAX_PATH) ) ; i++ )
	{
		*(Volume_path+i) = *(pWork+i);
	}
	*(Volume_path+i) = '\0';




	iRet = GetDFSPath( Volume_path , data_stp->dfs_name , CHECKMODE );

	GW_free( Volume_path , 0 );

	if( iRet != GWERR_NORMAL )
	{
		switch( iRet )
		{
			case GWERR_NOT_DFAMVOL:
									return  NOVOL;
			case GWERR_NOMEM:
									return  GWMEMERR;
			case GWERR_LONG:
									return  TOOLONG;
			case GWERR_INVAL:
			case GWERR_NUSE:
			case GWERR_NW_NOENT:
									return  NOENT;
			default:
									return  ABNORMAL;
		}
	}




	uiKyokabit = DFSrightsReq( data_stp->dfs_name , &DFS_Errno , plUtbl);

	if ( uiKyokabit == LIB_ERR )
	{
		return  fnErrChange( DFS_Errno,plUtbl );
	}

	pBuffer = GW_malloc( sizeof(struct dstat) );
	if (pBuffer == NULL)
	{
		return  GWMEMERR;
	}
	memset( pBuffer , 0x00 , sizeof(struct dstat) );




	iRet = DFSstatReq( data_stp->dfs_name , pBuffer , &DFS_Errno , plUtbl );

	if (iRet == LIB_ERR)
	{
		GW_free( pBuffer , 0 );
		return  fnErrChange( DFS_Errno,plUtbl );
	}


	if (pBuffer->d_mode & DS_IFDIR)
	{
		data_stp->dir_info = C_DIR;
		RightsMap.mode = ACL_DIR;
	}
	else
	{
		data_stp->dir_info = C_FILE;
		RightsMap.mode = ACL_FILE;
	}

	data_stp->file_size   = pBuffer->d_size;
	data_stp->change_date = pBuffer->d_mtime;

	if ( (*(pBuffer->d_uid)) == '\0')
	{
		data_stp->user_name[0] = 0x01;
		data_stp->user_name[1] = '\0';

		GW_free( pBuffer , 0 );
	}
	else
	{
		aszNWUserName = GW_malloc( UNAMELEN );
		if( aszNWUserName == 0 )
		{
			GW_free( pBuffer , 0 );
			return  GWMEMERR;
		}




		iRet = GetDFAMUserName( pBuffer->d_uid , aszNWUserName );

		GW_free( pBuffer , 0 );

		if( iRet != 0 )
		{
			GW_free( aszNWUserName , 0 );

			switch( iRet )
			{
				case -1:
						data_stp->user_name[0]=0x02;
						data_stp->user_name[1]='\0';
						break;
				case -2:
						return  GWMEMERR;
				case -3:
						return  BINDERR;
				default:
						return  FAILECHG;
			}
		}
		else
		{

			strcpy( data_stp->user_name , aszNWUserName );

			GW_free( aszNWUserName , 0 );
		}
	}

	RightsMap.ACL_list = uiKyokabit;




	RightsMap.trustee_list = 0;
	iRet = GW_Rights_Map( &RightsMap );
	if( iRet != SUCCESS )
	{
		return  MODEERR;
	}



	fnKenriMap( RightsMap.trustee_list , data_stp->rights_list );

	strcpy( data_stp->path_name , pcNetpath );

	return  NORMAL;
}
