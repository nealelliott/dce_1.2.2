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
 * $Log: gfillsdr.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:11  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:30  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:58:15  takano
 * 	First Submit
 * 	[1996/02/21  07:58:10  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <nwenvrn.h>
#include <malloc.h>
#include <process.h>
#include <nwdir.h>
#include <niterror.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "err.h"
#include "gfiler.h"


/*EnglishComment**************************************************************/
/* ID   = fnDListdir                                                         */
/*                                                                           */
/* name = Displaying subdirectory information (main routine)                 */
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


void fnDListdir(struct GTGroup_t    *plUtbl,
				int                 iTask_num,
				unsigned char       *pucBuf,
				unsigned long       pulBuflen,
				unsigned char       *pucRetbuf,
				unsigned long       *pulRetlength)
{

	struct TableKeep          **pTaskTb;
	struct RetListdirDataKeep *pKeep;
	struct RetListdirDataKeep *pUpdate;
	struct TableKeep          *pTable;
	struct RetHeader          *plHeader;

	memset(pucRetbuf,0x00,*pulRetlength);

    plHeader = (struct RetHeader *)pucRetbuf;

	pTaskTb = &(plUtbl->pTblKeep_sp);
	memmove( plHeader->command_name , pucBuf , COMMLEN );
	while(*pTaskTb != NULL )
	{
		if((*pTaskTb)->task_num == iTask_num)

		{
			pTable = *pTaskTb;
			if (pulBuflen == COMMLEN)
			{

				fnDListdirContinue( plUtbl , pTable , pucBuf , pucRetbuf , pulRetlength );
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
					plHeader->ret_value  = FALSE;
					plHeader->next_frame = MEMERR;
					*pulRetlength = sizeof(struct RetHeader);

					fnCommnErrLog( pucRetbuf );

					return;
				}
				memset(pTable,0x00,sizeof(struct TableKeep));

				memmove(pTable->command_name,pucBuf,COMMLEN);
				pTable->task_num = iTask_num;

				pTable->pNext = plUtbl->pTblKeep_sp;
				plUtbl->pTblKeep_sp = pTable;

				fnDListdirNew( plUtbl ,  pTable , pucBuf , pucRetbuf , pulRetlength );
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
		memmove( plHeader->command_name,pucBuf,COMMLEN );
		plHeader->DFAM_errno = CABNORM;
		plHeader->ret_value  = FALSE;
		plHeader->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);

		fnCommnErrLog( pucRetbuf );

		return;
	}


	pTable = GW_malloc(sizeof(struct TableKeep));

	if(pTable == NULL)
	{
		strncpy( plHeader->command_name,pucBuf,COMMLEN );
		plHeader->DFAM_errno = GWMEMERR;
		plHeader->ret_value  = FALSE;
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

	fnDListdirNew( plUtbl , pTable , pucBuf , pucRetbuf , pulRetlength );
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
/* ID   = fnDListdirContinue                                                 */
/*                                                                           */
/* name = Displaying the subdirectory information (processing for the        */
/*        continued information)                                             */
/* func = In the case that Client sent "continued information."              */
/*           Gets the continued information chained from the task table,     */
/*           then returns it to Client.                                      */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pucBuf      :unsigned char *   :inp: Gateway receive buffer        */
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


void fnDListdirContinue(struct GTGroup_t *plUtbl,
                        struct TableKeep *pTable,
                        unsigned char    *pucBuf,
                        unsigned char    *pucRetbuf,
                        unsigned long    *pulRetlength)
{
	char                       *pcHeaderLastData;
	struct RetListdirDataKeep  *pDataTop;
	unsigned char              *indexp;
	struct GTGroup_t           *workp;

	workp = plUtbl;

	pTable->plRet_st = (struct RetHeader *)pucRetbuf;

	pTable->plRet_ldata = (struct RetListdirData *)(pucRetbuf+sizeof(struct RetHeader));

	strncpy((pTable->plRet_st)->command_name , pucBuf , COMMLEN );

	(pTable->plRet_st)->DFAM_errno = NORMAL;
	(pTable->plRet_st)->ret_value = TRUE;

	pDataTop = pTable->pData;

	pcHeaderLastData = pucRetbuf+(*pulRetlength)-sizeof(struct RetListdirData);


	(pTable->plRet_st)->next_frame = FALSE;

	while ( pDataTop != NULL )
	{
		if (((char *)((pTable->plRet_ldata)+1)) <= pcHeaderLastData)
		{

			*(pTable->plRet_ldata) = pDataTop->data;
			(pTable->plRet_ldata)->next_data = FALSE;
			pTable->pData = pDataTop->pNext;
			GW_free(pDataTop,0);
			pDataTop = NULL;
			pDataTop = pTable->pData;

			indexp = pucRetbuf + ( sizeof( struct RetHeader ) );
			while( (struct RetListdirData *)indexp != pTable->plRet_ldata )
			{
				((struct RetListdirData *)indexp)->next_data = TRUE;
				indexp += sizeof( struct RetListdirData );
			}
			pTable->plRet_ldata++;
		}

		else
		{
			(pTable->plRet_st)->next_frame = TRUE;
			break;
		}
	}


	*pulRetlength = ((char *)(pTable->plRet_ldata))-pucRetbuf;
}


/*EnglishComment**************************************************************/
/* ID   = fnDListdirNew                                                      */
/*                                                                           */
/* name = Displaying the subdirecoty information (processing for the new     */
/*        information)                                                       */
/* func = In the case that Client sent "new information."                    */
/*           Handles wildcard characters.                                    */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pucBuf      :unsigned char *   :inp: Gateway receive buffer        */
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

void fnDListdirNew(struct GTGroup_t    *plUtbl,
				   struct TableKeep    *pTable,
				   unsigned char       *pucBuf,
				   unsigned char       *pucRetbuf,
				   unsigned long       *pulRetlength)
{
	struct dlistdir_data    *plDlistdir;
	char                    aTarget[3];
	int		                delimita;
	int                     iPathlength;
	int						rtn;

	pTable->plRet_st = (struct RetHeader *)pucRetbuf;

	pTable->plRet_ldata = (struct RetListdirData *)(pucRetbuf+sizeof(struct RetHeader));

	plDlistdir = (struct dlistdir_data *)pucBuf;

	if(plDlistdir->path_name[0] == NULL)
	{
		memmove((pTable->plRet_st)->command_name,plDlistdir->command_name,COMMLEN);
		(pTable->plRet_st)->DFAM_errno = NULLPATH;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		return;
	}

	iPathlength = strlen(plDlistdir->path_name);
	aTarget[0] = '*';
	aTarget[1] = '?';
	aTarget[2] = '\0';


	if( D_strpbrk( plDlistdir->path_name , aTarget) == NULL )
	{
		delimita = iPathlength;
	}
	else
	{
		delimita = iPathlength -1 ;
	}

	memmove((pTable->plRet_st)->command_name,plDlistdir->command_name,COMMLEN);
	(pTable->plRet_st)->DFAM_errno = NORMAL;
	(pTable->plRet_st)->ret_value  = TRUE;

	if(iPathlength > delimita)
	{



		fnDListdir_wd( plUtbl , pTable , plDlistdir->path_name , pucRetbuf , pulRetlength , plDlistdir);
	}
	else
	{



		rtn = fnDListdir_dirwd( plUtbl, pTable , plDlistdir , pucRetbuf , pulRetlength );

		if( rtn != NORMAL )
		{
			(pTable->plRet_st)->DFAM_errno = rtn;
			(pTable->plRet_st)->ret_value  = FALSE;
			(pTable->plRet_st)->next_frame = FALSE;
			*pulRetlength = sizeof( struct RetHeader );
		}
	}
}


/*EnglishComment**************************************************************/
/* ID   = fnDListdir_dirwd                                                   */
/*                                                                           */
/* name = Processing for "no wildcard characters"                            */
/* func = Handles the case of "no wildcard characters."                      */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        plDlistdir  :struct dlistdir_data *:inp: structure for the sending */
/*                                                 data                      */
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

int fnDListdir_dirwd( struct GTGroup_t    	*plUtbl,
				      struct TableKeep    	*pTable,
					  struct dlistdir_data 	*plDlistdir,
				      unsigned char       	*pucRetbuf,
				      unsigned long       	*pulRetlength)
{
	char					*pWork;
	char                    *pcDFSpath;
	struct dstat            *pBuffer;
	char                    *Volume_path;
	int   			        i;
	int   			        iRet;
	int                     DFS_Errno;
	dmode_t					wk_mode;
	unsigned char			*path_name;

	pWork = plDlistdir->path_name;


	while((*pWork != '\/') && (*pWork != '\\') && (*pWork != '\0'))
	{
		pWork++;
	}


	if( (*pWork == '\0') || ((*(++pWork)) == '\0') )
	{
		return  NOENT;
	}

	Volume_path = GW_malloc(_MAX_PATH+1);
	if( Volume_path == 0 )
	{
		return  GWMEMERR;
	}

	for( i=0 ; (( *(pWork+i) != '\0') && (i < _MAX_PATH) ) ; i++ )
	{
		*(Volume_path+i) = *(pWork+i);
	}
	*(Volume_path+i) = '\0';

	pcDFSpath = GW_malloc(DFSPATH_MAX+1);
	if( pcDFSpath == 0 )
	{
		GW_free( Volume_path , 0 );
		return  GWMEMERR;
	}




	iRet = GetDFSPath( Volume_path , pcDFSpath , CHECKMODE );

	GW_free( Volume_path , 0 );

	if( iRet != GWERR_NORMAL )
	{
		GW_free( pcDFSpath , 0 );

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
	if ( pBuffer == NULL )
	{
		GW_free(pcDFSpath,0);
		return  GWMEMERR;
	}




	iRet = DFSstatReq( pcDFSpath , pBuffer , &DFS_Errno , plUtbl );

	GW_free(pcDFSpath,0);

	if (iRet == LIB_ERR)
	{
		GW_free(pBuffer,0);
		return  fnErrChange(DFS_Errno,plUtbl);
	}
	wk_mode = pBuffer->d_mode;

	GW_free( pBuffer , 0 );

	if ( wk_mode & DS_IFDIR )
	{
		path_name = GW_malloc( NW_PATHSIZE+NW_PATHSIZE );
		if( path_name == 0 )
		{
       		return  GWMEMERR;
		}

		strcpy( path_name , plDlistdir->path_name );
		strcat( path_name , "\\*.*" );




		fnDListdir_wd( plUtbl , pTable , path_name , pucRetbuf , pulRetlength , plDlistdir);

		GW_free( path_name , 0 );

		return  NORMAL;
	}
	else
	{
		return  NOENT;
	}
}


/*EnglishComment**************************************************************/
/* ID   = fnDListdir_wd                                                      */
/*                                                                           */
/* name = Processing for "wildcard characters"                               */
/* func = Handles the case that a wildcard character exists.                 */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pathp       :char *            :inp: designated directory path     */
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*        plDlistdir  :struct dlistdir_data *:inp: structure for the sending */
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

void fnDListdir_wd(struct GTGroup_t     *plUtbl,
				   struct TableKeep     *pTable,
                   char                 *pathp,
				   unsigned char        *pucRetbuf,
				   unsigned long        *pulRetlength,
				   struct dlistdir_data *plDlistdir)
{
	DIR                     *dirStructp;
	DIR                     *direntp;
	int                     NW_err_num;
	int                     iPathlength;
	int   			        i;
	unsigned char           *dirp;
	unsigned char           *path_name;
	char                    *Volume_path;
	char                    *pcDFSpath;
	struct dstat            *pBuffer;
	dmode_t					wk_mode;
	int                     iRet;
	char                    *pWork;
	int                     DFS_Errno;
    int						iValue;
	int						rtn;




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

	if(direntp == NULL)
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

	dirp = GW_malloc( NW_PATHSIZE+NW_PATHSIZE );
	if( dirp == 0 )
	{
		(pTable->plRet_st)->DFAM_errno = GWMEMERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
       	return;
	}

	strcpy( dirp , pathp );
	iPathlength = strlen( dirp );

	for( i=iPathlength-1 ; i >= 0 ; i-- )
	{


		if( (*(dirp+i) == '\\') || (*(dirp+i) == ':') )
		{
			*(dirp+i+1) = '\0';
			break;
		}

	}

	path_name = GW_malloc( NW_PATHSIZE+NW_PATHSIZE );
	if( path_name == 0 )
	{
		GW_free( dirp , 0 );

		(pTable->plRet_st)->DFAM_errno = GWMEMERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
       	return;
	}

	while ( direntp != NULL )
	{
		strcpy( path_name , dirp );
		strcat( path_name , direntp->d_name );

		pWork = path_name;

		while((*pWork != '\/') && (*pWork != '\\') && (*pWork != '\0'))
		{
			pWork++;
		}

		if((*pWork == '\0') || ((*(++pWork)) == '\0'))
		{
			rtn = -2;
			goto jump;
		}

		Volume_path = GW_malloc( _MAX_PATH+1 );
		if( Volume_path == 0 )
		{
			GW_free( dirp , 0 );
			GW_free( path_name , 0 );

			fnDListdir_err( pTable , pulRetlength , GWMEMERR );
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
			GW_free( dirp , 0 );
			GW_free( path_name , 0 );
			GW_free( Volume_path , 0 );

			fnDListdir_err( pTable , pulRetlength , GWMEMERR );
			return;
		}




		iRet = GetDFSPath( Volume_path , pcDFSpath , CHECKMODE );

		GW_free(Volume_path,0);

		if( iRet != GWERR_NORMAL )
		{
			GW_free(pcDFSpath,0);

			switch(iRet)
			{
				case GWERR_NOT_DFAMVOL:
					fnDListdir_err( pTable , pulRetlength , NOVOL );
					break;
				case GWERR_NOMEM:
					fnDListdir_err( pTable , pulRetlength , GWMEMERR );
					break;
				case GWERR_LONG:
					fnDListdir_err( pTable , pulRetlength , TOOLONG );
					break;
				case GWERR_INVAL:
				case GWERR_NUSE:
				case GWERR_NW_NOENT:
					rtn = -2;
					goto jump;
				default:
					fnDListdir_err( pTable , pulRetlength , ABNORMAL );
					break;
			}

			GW_free( dirp , 0 );
			GW_free( path_name , 0 );

			return;
		}

		pBuffer = GW_malloc(sizeof(struct dstat));
		if (pBuffer == NULL)
		{
			GW_free(pcDFSpath,0);
			GW_free( dirp , 0 );
			GW_free( path_name , 0 );

			fnDListdir_err( pTable , pulRetlength , GWMEMERR );
			return;
		}




		iRet = DFSstatReq( pcDFSpath , pBuffer , &DFS_Errno , plUtbl );

		GW_free( pcDFSpath , 0 );

		if ( iRet == LIB_ERR )
		{
			GW_free( pBuffer , 0 );

			if( fnErrChange(DFS_Errno,plUtbl) == NOENT )
			{
				rtn = -2;
				goto jump;
			}
			else if( fnErrChange(DFS_Errno,plUtbl) == PATHERR )
			{
				rtn = -3;
				goto jump;
			}
			else if( fnErrChange(DFS_Errno,plUtbl) == NORIGHT )
			{
				rtn = -4;
				goto jump;
			}
			else
			{
				fnDListdir_err( pTable , pulRetlength , fnErrChange(DFS_Errno,plUtbl) );
			}

			GW_free( dirp , 0 );
			GW_free( path_name , 0 );

			return;
		}
		wk_mode = pBuffer->d_mode;

		GW_free( pBuffer , 0 );

		if ( wk_mode & DS_IFDIR )
		{



			rtn = fnDListdir_op( plUtbl,pTable,(char *)path_name,plDlistdir,pucRetbuf,pulRetlength);
			if( rtn == -1 )
			{
				break;
			}
		}

jump:

		direntp = readdir(dirStructp);
		if( direntp == NULL )
		{
			if( errno != ENOENT )
   			{
				NW_err_num = NetWareErrno;
				fnDListdir_err( pTable , pulRetlength , fnNW_change_errno(NW_err_num) );
       			iValue = closedir( dirStructp );

				GW_free( dirp , 0 );
				GW_free( path_name , 0 );

       			return;
			}
       	}
 	}

	GW_free( dirp , 0 );
	GW_free( path_name , 0 );

	if( (strlen(pucRetbuf+sizeof(struct RetHeader)) ) == 0 )
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




	iValue = closedir( dirStructp );

	if( iValue != ESUCCESS )
	{
		NW_err_num = NetWareErrno;
		fnDListdir_err( pTable , pulRetlength , fnNW_change_errno(NW_err_num) );
	}
}


/*EnglishComment**************************************************************/
/* ID   = fnDListdir_err                                                     */
/*                                                                           */
/* name = Setting error code                                                 */
/* func = Sets error information to the send buffer to be sent to Client.    */
/*                                                                           */
/* i/o  = pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*        Err_no      :int               :inp: DFAM error number             */
/*          :                                                                */
/* return = none                                                             */
/*          :                                                                */
/* common = none                                                             */
/*          :                                                                */
/*  date = January 31, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/

void fnDListdir_err( struct TableKeep *pTable , unsigned long *pulRetlength , int   Err )
{
	(pTable->plRet_st)->DFAM_errno = Err;
	(pTable->plRet_st)->ret_value  = FALSE;
	(pTable->plRet_st)->next_frame = FALSE;
	*pulRetlength = sizeof( struct RetHeader );
}


/*EnglishComment**************************************************************/
/* ID   = fnDListdir_op                                                      */
/*                                                                           */
/* name = Operations for displaying the subdirectory information             */
/* func = Sets information to be sent to Agent.                              */
/*           - Converting file names                                         */
/*           - Handling options (E, D, T)                                    */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pcNetpath   :char            * :inp: NetWare path                  */
/*        plDlistdir  :struct dlistdir_data *:inp: Gateway receive buffer    */
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*          :                                                                */
/* return = none                                                             */
/*          :                                                                */
/* common = none                                                             */
/*          :                                                                */
/*  date = January 31, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/

int fnDListdir_op( struct GTGroup_t          *plUtbl,
                   struct TableKeep          *pTable,
                   char                      *pcNetpath,
                   struct dlistdir_data      *plDlistdir,
                   unsigned char             *pucRetbuf,
                   unsigned long             *pulRetlength)
{
	struct RetListdirData                    *data_stp;
	char                                     *pcHeaderLastData;
	unsigned char                            *indexp;
	struct RetListdirDataKeep                **icountp;
	int										 rtn_no;

	data_stp = GW_malloc( sizeof(struct RetListdirData) );
	if( data_stp == 0 )
	{
		(pTable->plRet_st)->DFAM_errno = GWMEMERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		return  (-1);
	}
	memset( data_stp , 0x00 , sizeof(struct RetListdirData) );

	rtn_no = fnDListdir_main( plUtbl , pcNetpath , data_stp , plDlistdir );

	(pTable->plRet_st)->DFAM_errno = rtn_no;

	if( rtn_no != NORMAL )
	{
		GW_free( data_stp , 0 );

		if( rtn_no == NOENT )
		{
			return (-2);
		}
		if( rtn_no == PATHERR )
		{
			return (-3);
		}
		if( rtn_no == NORIGHT )
		{
			return (-4);
		}
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof( struct RetHeader );
		return  (-1);
	}

	(pTable->plRet_st)->ret_value = TRUE;


	data_stp->sub = TRUE;

	pcHeaderLastData = pucRetbuf+(*pulRetlength)-sizeof(struct RetListdirData);
	if (((char *)(pTable->plRet_ldata)) <= pcHeaderLastData)
	{
		(pTable->plRet_st)->next_frame = FALSE;
		data_stp->next_data = FALSE;
		*(pTable->plRet_ldata) = *data_stp;
		indexp = pucRetbuf + ( sizeof( struct RetHeader) );
		while((struct RetListdirData *)indexp != pTable->plRet_ldata)
		{
			((struct RetListdirData *)indexp)->next_data = TRUE;
			indexp += sizeof( struct RetListdirData );
		}
		pTable->plRet_ldata++;
	}

	else
	{
		(pTable->plRet_st)->next_frame = TRUE;
		pTable->ppLDataKeep = &(pTable->pData);
		if( *(pTable->ppLDataKeep) == NULL)
		{
			*(pTable->ppLDataKeep) = GW_malloc(sizeof(struct RetListdirDataKeep));
			if (*(pTable->ppLDataKeep) == NULL)
			{
				(pTable->plRet_st)->DFAM_errno = GWMEMERR;
				(pTable->plRet_st)->ret_value = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				GW_free( data_stp , 0 );

				return  (-1);
			}
			(*(pTable->ppLDataKeep))->data = *data_stp;
			(*(pTable->ppLDataKeep))->pNext = NULL;
		}
		else
		{
			icountp = &(pTable->pData);
			while (*icountp != NULL)
			{
				icountp = &((*icountp)->pNext);
			}
			*icountp = GW_malloc(sizeof(struct RetListdirDataKeep));
			if (*icountp == NULL)
			{
				(pTable->plRet_st)->DFAM_errno = GWMEMERR;
				(pTable->plRet_st)->ret_value = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				GW_free( data_stp , 0 );

				return  (-1);
			}
			(*icountp)->data = *data_stp;
			(*icountp)->pNext = NULL;
		}
	}
	GW_free( data_stp , 0 );


	*pulRetlength = ((char *)(pTable->plRet_ldata))-pucRetbuf;

	return  0;
}


/*EnglishComment**************************************************************/
/* ID   = fnDListdir_main                                                    */
/*                                                                           */
/* name = Displaying the subdirectory information (main routine)             */
/* func = Sends a "get the subdirectory information" request to Agent.       */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pcNetpath   :char            * :inp: NetWare path                  */
/*        data_stp    :struct RetGrantData * :out: structure for Gateway's   */
/*                                                 send data                 */
/*        plDlistdir  :struct dlistdir_data *:inp: Gateway receive buffer    */
/*          :                                                                */
/* return = none                                                             */
/*          :                                                                */
/* common = none                                                             */
/*          :                                                                */
/*  date = January 31, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/

int fnDListdir_main( struct GTGroup_t *plUtbl , char *pcNetpath , struct RetListdirData *data_stp ,struct dlistdir_data *plDlistdir )
{
	char                                     *pcDFSpath;
	int                                      iRet;
	int                                      DFS_Errno;
	char                                     *Volume_path;
	struct dstat                             *pBuffer;
	int                                      i;
	unsigned int                             uiKyokabit;
	int                                      iDflag;
	int                                      iRflag;
	struct RightsMapping                     RightsMap;
	struct RightsMapping                     *NW_kenrimap;
	char                                     *pWork;
	int                                      count;

	pWork = pcNetpath;


	while((*pWork != '\/') && (*pWork != '\\') && (*pWork != '\0'))
	{
		pWork++;
	}


	if((*pWork == '\0') || ((*(++pWork)) == '\0'))
	{
		return  NOENT;
	}

	Volume_path = GW_malloc( _MAX_PATH+1 );
	if( Volume_path == 0 )
	{
		return  GWMEMERR;
	}

	for( i=0 ; (( *(pWork+i) != '\0') && (i < _MAX_PATH) ) ; i++ )
	{
		*(Volume_path+i) = *(pWork+i);
	}
	*(Volume_path+i) = '\0';

	pcDFSpath = GW_malloc( DFSPATH_MAX+1 );
	if( pcDFSpath == 0 )
	{
		return  GWMEMERR;
	}




    iRet = GetDFSPath( Volume_path , pcDFSpath , CHECKMODE );

	GW_free( Volume_path , 0 );

	if( iRet != GWERR_NORMAL )
	{
		GW_free( pcDFSpath , 0 );

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

	pBuffer = GW_malloc( sizeof(struct dstat) );
	if( pBuffer == NULL )
	{
		GW_free(pcDFSpath,0);
		return  GWMEMERR;
	}




	iRet = DFSstatReq( pcDFSpath , pBuffer , &DFS_Errno , plUtbl );

	if (iRet == LIB_ERR)
	{
		GW_free( pcDFSpath,0 );
		GW_free( pBuffer,0 );
		return  fnErrChange(DFS_Errno,plUtbl);
	}

	strcpy( data_stp->path_name , pcNetpath );

	iDflag = FALSE;
	iRflag = FALSE;

	for( count=0 ; count < 5 ; count++ )
	{
		switch( plDlistdir->option[count] )
		{



			case 'd' :
			case 't' :
						if( iDflag != TRUE )
						{
							data_stp->change_date = pBuffer->d_mtime;
							iDflag = TRUE;
						}
						break;
			case 'a' :
						if(iDflag != TRUE)
						{
							data_stp->change_date = pBuffer->d_mtime;
							iDflag = TRUE;

						}
			case 'e' :
						if(iRflag != TRUE)
						{



							uiKyokabit = DFSrightsReq( pcDFSpath , &DFS_Errno , plUtbl );

							if ( uiKyokabit == LIB_ERR )
							{

								GW_free( pcDFSpath , 0 );
								GW_free( pBuffer , 0 );
								return  fnErrChange(DFS_Errno,plUtbl);
							}
							NW_kenrimap = &RightsMap;
							if (pBuffer->d_mode & DS_IFDIR)
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

								GW_free( pcDFSpath , 0 );
								GW_free( pBuffer , 0 );
								return  MODEERR;
							}




							iRet = fnKenriMap( NW_kenrimap->trustee_list , data_stp->rights_list );

							iRflag = TRUE;
						}
						break;
			default:
						break;
		}
	}

	GW_free(pcDFSpath,0);
	GW_free(pBuffer,0);

	return  NORMAL;
}
