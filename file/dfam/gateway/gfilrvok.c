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
 * $Log: gfilrvok.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:18  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:36  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:00:28  takano
 * 	First Submit
 * 	[1996/02/21  08:00:24  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <nwenvrn.h>
#include <process.h>
#include <nwdir.h>
#include <niterror.h>
#include <nwbindry.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "err.h"
#include "gfiler.h"

/*EnglishComment**************************************************************/
/* ID   = fnDRevoke                                                          */
/*                                                                           */
/* name = Modifying the trustee information (main routine)                   */
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
/*  date = February 1, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void fnDRevoke(struct GTGroup_t  *plUtbl,
			   int               iTask_num,
			   unsigned char     *pucBuf,
			   unsigned long     pulBuflen,
			   unsigned char     *pucRetbuf,
			   unsigned long     *pulRetlength)
{
	struct TableKeep             **pTaskTb;
	struct RetRevokeDataKeep     *pKeep;
	struct RetRevokeDataKeep     *pUpdate;
	struct TableKeep             *pTable;
	struct RetHeader             *plHeader;

	memset(pucRetbuf,0x00,*pulRetlength);

    plHeader = (struct RetHeader *)pucRetbuf;

	pTaskTb = &(plUtbl->pTblKeep_sp);

	while(*pTaskTb != NULL )
	{
		if((*pTaskTb)->task_num == iTask_num )

		{
			pTable = *pTaskTb;
			if (pulBuflen == COMMLEN)
			{



				fnDRevokeContinue( pTable , pucBuf , pucRetbuf , pulRetlength);
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
					plHeader->DFAM_errno  = GWMEMERR;
					plHeader->ret_value   = FALSE;
					plHeader->next_frame  = FALSE;
					*pulRetlength = sizeof(struct RetHeader);

					fnCommnErrLog( pucRetbuf );

					return;
				}
				memset(pTable,0x00,sizeof(struct TableKeep));

				memmove(pTable->command_name,pucBuf,COMMLEN);
				pTable->task_num = iTask_num;

				pTable->pNext = plUtbl->pTblKeep_sp;
				plUtbl->pTblKeep_sp = pTable;




				fnDRevokeNew( plUtbl , pTable , pucBuf , pucRetbuf , pulRetlength );
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
		memmove(plHeader->command_name,pucBuf,COMMLEN);
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
		memmove(plHeader->command_name,pucBuf,COMMLEN);
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




	fnDRevokeNew( plUtbl , pTable , pucBuf , pucRetbuf , pulRetlength );
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
/* ID   = fnDRevokeContinue                                                  */
/*                                                                           */
/* name = Setting trustee (processing for the continued information)         */
/* func = In the case that Client sent "continued information."              */
/*           Gets the continued information chained from the task table,     */
/*           then returns it to Client.                                      */
/*                                                                           */
/* i/o  = pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pucBuf      :unsigned char *   :inp: Gateway receive buffer        */
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*          :                                                                */
/* return = none                                                             */
/*          :                                                                */
/* common = none                                                             */
/*          :                                                                */
/*  date = April 24, 1995 by Jun Nakagawa                                    */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/

void fnDRevokeContinue( struct TableKeep *pTable,
					    unsigned char    *pucBuf,
					    unsigned char    *pucRetbuf,
					    unsigned long    *pulRetlength)
{
	char                      *pcHeaderLastData;
	struct RetRevokeDataKeep  *pDataTop;
	unsigned char             *indexp;

	pTable->plRet_st = (struct RetHeader *)pucRetbuf;
	pTable->plRet_rvdata = (struct RetRevokeData *)(pucRetbuf+sizeof(struct RetHeader));

	(pTable->plRet_st)->ret_value = TRUE;
	(pTable->plRet_st)->DFAM_errno = NORMAL;
	memmove( ( pTable->plRet_st)->command_name , pucBuf , COMMLEN );
	pDataTop = pTable->pData;
	pcHeaderLastData = pucRetbuf+(*pulRetlength)-sizeof(struct RetRevokeData);


	(pTable->plRet_st)->next_frame = FALSE;

	while (pDataTop != NULL)
	{

		if ( (char *)(pTable->plRet_rvdata) <= pcHeaderLastData)
		{

			*(pTable->plRet_rvdata) = pDataTop->data;
			(pTable->plRet_rvdata)->next_data = FALSE;
			pTable->pData = pDataTop->pNext;
			GW_free(pDataTop,0);
			pDataTop = NULL;
			pDataTop = pTable->pData;

			indexp = pucRetbuf + ( sizeof( struct RetHeader ) );
			while( (struct RetRevokeData *)indexp != pTable->plRet_rvdata)
			{
				((struct RetRevokeData *)indexp)->next_data = TRUE;
				indexp += sizeof( struct RetRevokeData );
			}
			pTable->plRet_rvdata++;
		}

		else
		{
			(pTable->plRet_st)->next_frame = TRUE;
			break;
		}

	}


	*pulRetlength = ((char *)(pTable->plRet_rvdata))-pucRetbuf;
}


/*EnglishComment**************************************************************/
/* ID   = fnDRevokeNew                                                       */
/*                                                                           */
/* name = Modifying trustee (processing for the new information)             */
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
/*  date = February 1, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void fnDRevokeNew(struct GTGroup_t *plUtbl,
				  struct TableKeep *pTable,
				  unsigned char    *pucBuf,
				  unsigned char    *pucRetbuf,
				  unsigned long    *pulRetlength)
{
	struct drevoke_data *plDrevoke;
	char                aTarget[3];
	int                 delimita;
	int                 iPathlength;
	int                 iValue;
	DIR                 *dirStructp;
	DIR                 *direntp;
	int                 NW_err_num;
	unsigned char       *path_name;
	int                 i;
	int					rtn;

	pTable->plRet_st     = (struct RetHeader *)pucRetbuf;
	pTable->plRet_rvdata = (struct RetRevokeData *)(pucRetbuf+sizeof(struct RetHeader));
	plDrevoke = (struct drevoke_data *)pucBuf;
	memmove( (pTable->plRet_st)->command_name , pucBuf , COMMLEN );

	if( plDrevoke->path_name[0] == NULL )
	{
		(pTable->plRet_st)->DFAM_errno = NULLPATH;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		return;
	}

	iPathlength = strlen(plDrevoke->path_name);
	aTarget[0] = '*';
	aTarget[1] = '?';
	aTarget[2] = '\0';


	if( D_strpbrk( plDrevoke->path_name , aTarget) == NULL )
	{
		delimita = iPathlength;
	}
	else
	{
		delimita = iPathlength -1 ;
	}

	(pTable->plRet_st)->DFAM_errno = NORMAL;
	(pTable->plRet_st)->ret_value  = TRUE;

	if( iPathlength <= delimita )
	{





		rtn = fnDRevoke_op(plUtbl,pTable,(char *)plDrevoke->path_name,plDrevoke,pucRetbuf,pulRetlength);
		if( rtn == -2 )
		{
			fnDRevoke_err( pTable , pulRetlength , NOENT );
		}
		if( rtn == -3 )
		{
			fnDRevoke_err( pTable , pulRetlength , NOTRLST );
		}
		if( rtn == -4 )
		{
			fnDRevoke_err( pTable , pulRetlength , NORIGHT );
		}
		if( rtn == -5 )
		{
			fnDRevoke_err( pTable , pulRetlength , PATHERR );
		}
		if( rtn == -6 )
		{
			fnDRevoke_err( pTable , pulRetlength , ENOTLFS );
		}
		return;
	}






	dirStructp = opendir( plDrevoke->path_name );

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
		*pulRetlength = sizeof(struct RetHeader);




		closedir(dirStructp);

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

	while ( direntp != NULL )
	{
		strcpy( path_name , plDrevoke->path_name );
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




		rtn = fnDRevoke_op(plUtbl,pTable,(char *)path_name,plDrevoke,pucRetbuf,pulRetlength);
		if( rtn == -1 )
		{
			break;
		}




		direntp = readdir(dirStructp);

		if( direntp == NULL )
		{
			if( errno != ENOENT )
           	{
				GW_free( path_name , 0 );

				NW_err_num = NetWareErrno;
				(pTable->plRet_st)->DFAM_errno = fnNW_change_errno(NW_err_num);
				(pTable->plRet_st)->ret_value = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);




           		closedir( dirStructp );

               	return;
			}
       	}
	}

	GW_free( path_name , 0 );

	if( (strlen(pucRetbuf+sizeof(struct RetHeader)) ) == 0 )
	{
		if( rtn != -1 )
		{
			if( rtn == -3 )
			{
				(pTable->plRet_st)->DFAM_errno = NOTRLST;
				(pTable->plRet_st)->ret_value  = FALSE;
			}
			else if( rtn == -4 )
			{
				(pTable->plRet_st)->DFAM_errno = NORIGHT;
				(pTable->plRet_st)->ret_value  = FALSE;
			}
			else if( rtn == -5 )
			{
				(pTable->plRet_st)->DFAM_errno = PATHERR;
				(pTable->plRet_st)->ret_value  = FALSE;
			}
			else if( rtn == -6 )
			{
				(pTable->plRet_st)->DFAM_errno = ENOTLFS;
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
/* ID   = fnDRevoke_op                                                       */
/* name = Trustee modifying operations                                       */
/* func = Sets information to be sent to Agent.                              */
/*           - File name conversion                                          */
/*           - Mapping trustee                                               */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pcNetpath   :char *            :inp: NetWare path                  */
/*        plDrevoke   :struct drevoke_data *:inp: Gateway receive buffer     */
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

int  fnDRevoke_op(struct GTGroup_t     *plUtbl,
				  struct TableKeep     *pTable,
				  char                 *pcNetpath,
				  struct drevoke_data  *plDrevoke,
				  unsigned char        *pucRetbuf,
				  unsigned long        *pulRetlength)
{
	char               			*pcDFSpath;
	int                			iRet;
	int                			iAcl_length;
	unsigned int       			ushMode;
	int                			DFS_Errno;
	unsigned long      			ulUG_length;
	WORD               			ushKyokabit;
	char               			*Volume_path;
	struct dstat       			*pBuffer;
    char               			*aszDCEuid;
	char               			*pWork;
	int                			i;
	struct RightsMapping        RightsMap;
	struct RightsMapping        *NW_kenrimap;
	unsigned int       			uiEntry;
	struct GetTrustee  			*pGetTrustee;
	unsigned int       			uiCount;
	unsigned int       			uiRights;
	unsigned int        		uglen;
	unsigned char        		*charp;
	int                         rtn;

	NW_kenrimap = &RightsMap;
	iAcl_length = 0;
	memmove((pTable->plRet_st)->command_name,plDrevoke->command_name,COMMLEN);

	pWork = pcNetpath;


	while((*pWork != '\/') && (*pWork != '\\') && (*pWork != '\0'))
	{
		pWork++;
	}


	if((*pWork == '\0') || ((*(++pWork)) == '\0'))
	{
		return  (-2);
	}

	Volume_path = GW_malloc( _MAX_PATH+1 );

	if(Volume_path == NULL)
	{
		fnDRevoke_err( pTable , pulRetlength , GWMEMERR );
		return  (-1);
	}
	for( i=0 ; (( *(pWork+i) != '\0') && (i < _MAX_PATH) ) ; i++ )
	{
		*(Volume_path+i) = *(pWork+i);
	}
	*(Volume_path+i) = '\0';

	pcDFSpath = GW_malloc(DFSPATH_MAX+1);

	if(pcDFSpath == NULL)
	{
		GW_free( Volume_path , 0 );

		fnDRevoke_err( pTable , pulRetlength , GWMEMERR );
		return  (-1);
	}




	iRet = GetDFSPath( Volume_path , pcDFSpath , CHECKMODE );

	GW_free( Volume_path , 0 );

	if( iRet != GWERR_NORMAL )
	{
		GW_free(pcDFSpath,0);

		switch( iRet )
		{

			case GWERR_NOT_DFAMVOL:
									fnDRevoke_err( pTable , pulRetlength , NOVOL );
									return  (-1);
			case GWERR_NOMEM:
									fnDRevoke_err( pTable , pulRetlength , GWMEMERR );
									return  (-1);
			case GWERR_LONG:
									fnDRevoke_err( pTable , pulRetlength , TOOLONG );
									return  (-1);
			case GWERR_INVAL:
			case GWERR_NUSE:
			case GWERR_NW_NOENT:
									return  (-2);
			default:
									fnDRevoke_err( pTable , pulRetlength , ABNORMAL );
									return  (-1);
		}
	}


	aszDCEuid = GW_malloc(DFSGRP_MAX+1);

	if( aszDCEuid == NULL )
	{
		GW_free(pcDFSpath,0);

		fnDRevoke_err( pTable , pulRetlength , GWMEMERR );
		return  (-1);
	}
	if( plDrevoke->ug_class == DEFAULT_USER )
	{



		iRet = GetDCEUserName(plDrevoke->name,aszDCEuid);
	}
	else
	{



		iRet = GW_DCEGroup_Map(plDrevoke->name,aszDCEuid);
	}
	if( iRet != 0 )
	{
		GW_free( pcDFSpath , 0 );
		GW_free( aszDCEuid , 0 );

		switch( iRet )
		{
			case -1:
					if(plDrevoke->ug_class == DEFAULT_USER)
					{
						fnDRevoke_err( pTable , pulRetlength , NOUSER );
					}
					else
					{
						fnDRevoke_err( pTable , pulRetlength , NOGRP );
					}
					return  (-1);
			case -2:
					fnDRevoke_err( pTable , pulRetlength , NODFAMUSER );
					return  (-1);
			case -3:
					fnDRevoke_err( pTable , pulRetlength , GWMEMERR );
					return  (-1);
			case -4:
					fnDRevoke_err( pTable , pulRetlength , BINDERR );
					return  (-1);
			default:
					fnDRevoke_err( pTable , pulRetlength , FAILECHG );
					return  (-1);
		}
	}
	ulUG_length = strlen( aszDCEuid );


	switch( plDrevoke->ug_class )
	{
		case DEFAULT_USER:
							ushMode = DS_USER;
							break;
		case DEFAULT_GROUP:
							ushMode = DS_GROUP;
							break;
		default:
							GW_free(pcDFSpath,0);
							GW_free(aszDCEuid,0);

							fnDRevoke_err( pTable , pulRetlength , ERROPT );
							return  (-1);
	}


	pBuffer = GW_malloc( sizeof(struct dstat) );

	if (pBuffer == NULL)
	{
		GW_free(pcDFSpath,0);
		GW_free(aszDCEuid,0);

		fnDRevoke_err( pTable , pulRetlength , GWMEMERR );
		return  (-1);
	}




	iRet = DFSstatReq( pcDFSpath , pBuffer , &DFS_Errno , plUtbl );

	if (iRet == LIB_ERR)
	{
		GW_free(pcDFSpath,0);
		GW_free(aszDCEuid,0);
		GW_free(pBuffer,0);

		if( fnErrChange( DFS_Errno,plUtbl ) == NOENT )
		{
			return  (-2);
		}
		if( fnErrChange( DFS_Errno,plUtbl ) == NOTRLST )
		{
			return  (-3);
		}
		if( fnErrChange( DFS_Errno,plUtbl ) == NORIGHT )
		{
			return  (-4);
		}
		if( fnErrChange( DFS_Errno,plUtbl ) == PATHERR )
		{
			return  (-5);
		}
		if( fnErrChange( DFS_Errno,plUtbl ) == ENOTLFS )
		{
			return  (-6);
		}

		fnDRevoke_err( pTable , pulRetlength , fnErrChange(DFS_Errno,plUtbl) );
		return  (-1);
	}




	iRet = fnKenriBitmap( plDrevoke->t_list , &ushKyokabit );

	pTable->pcAclbuf = GW_malloc(2048);

	if ((pTable->pcAclbuf) == NULL)
	{
		GW_free(pcDFSpath,0);
		GW_free(aszDCEuid,0);
		GW_free(pBuffer,0);

		fnDRevoke_err( pTable , pulRetlength , GWMEMERR );
		return  (-1);
	}

	iAcl_length = 2048;




	iRet = DFSgetacl_syscallReq( pcDFSpath , ushMode , pTable->pcAclbuf ,
                       			 &iAcl_length , &DFS_Errno , plUtbl );

	while ( iRet == RETRY )
	{
		GW_free( pTable->pcAclbuf , 0 );
		pTable->pcAclbuf = NULL;

		pTable->pcAclbuf = GW_malloc( iAcl_length );

		if ( (pTable->pcAclbuf) == NULL )
		{
			GW_free(pcDFSpath,0);
			GW_free(aszDCEuid,0);
			GW_free(pBuffer,0);

	 		fnDRevoke_err( pTable , pulRetlength , GWMEMERR );
			return  (-1);
		}
		else
		{



			iRet = DFSgetacl_syscallReq( pcDFSpath , ushMode , pTable->pcAclbuf ,
                       					 &iAcl_length , &DFS_Errno , plUtbl );
		}
	}
	if ( iRet != SUCCESS )
	{


		GW_free(pcDFSpath,0);
		GW_free(aszDCEuid,0);
		GW_free(pBuffer,0);
		GW_free(pTable->pcAclbuf,0);
		pTable->pcAclbuf = NULL;

		if( fnErrChange( DFS_Errno,plUtbl ) == NOENT )
		{
			return  (-2);
		}
		if( fnErrChange( DFS_Errno,plUtbl ) == NOTRLST )
		{
			return  (-3);
		}
		if( fnErrChange( DFS_Errno,plUtbl ) == NORIGHT )
		{
			return  (-4);
		}
		if( fnErrChange( DFS_Errno,plUtbl ) == PATHERR )
		{
			return  (-5);
		}
		if( fnErrChange( DFS_Errno,plUtbl ) == ENOTLFS )
		{
			return  (-6);
		}

 		fnDRevoke_err( pTable , pulRetlength , fnErrChange(DFS_Errno,plUtbl) );
		return  (-1);
	}



	uiEntry = *((unsigned int *)pTable->pcAclbuf);
	pGetTrustee = (struct GetTrustee *)((pTable->pcAclbuf)+(sizeof(unsigned int)));
	uiRights = 0;
	for (uiCount = 0 ; uiCount < uiEntry ; uiCount++)
	{
		if( pGetTrustee->ug_length == 0xffffffff )
		{
			charp = (unsigned char *)pGetTrustee;
			charp = charp + sizeof(unsigned int) + sizeof(short) +
					sizeof(unsigned int);
			pGetTrustee = (struct GetTrustee *)charp;
		}
		else
		{
			if ( pGetTrustee->ug_length == (unsigned int)ulUG_length )
			{
				if (SUCCESS == strncmp(pGetTrustee->ug_name,aszDCEuid,ulUG_length))
				{
					uiRights = pGetTrustee->kyokabit;
					break;
				}
			}
			uglen = pGetTrustee->ug_length;
			charp = (unsigned char *)pGetTrustee;
			charp = charp + sizeof(unsigned int) + sizeof(short) +
					sizeof(unsigned int) + uglen;
			pGetTrustee = (struct GetTrustee *)charp;
		}
	}
	if ( uiCount == uiEntry )
	{
		GW_free( pcDFSpath , 0 );
		GW_free( aszDCEuid , 0 );
		GW_free( pBuffer , 0 );
		GW_free( pTable->pcAclbuf , 0 );
		pTable->pcAclbuf = NULL;

		return  (-3);
	}
	if ( pBuffer->d_mode & DS_IFDIR )
	{
		NW_kenrimap->mode = ACL_DIR;
	}
	else
	{
		NW_kenrimap->mode = ACL_FILE;
	}
	NW_kenrimap->ACL_list = uiRights;




	iRet = GW_Rights_Map( NW_kenrimap );

	if( iRet != SUCCESS )
	{
		GW_free( pcDFSpath , 0 );
		GW_free( aszDCEuid , 0 );
		GW_free( pBuffer , 0 );
		GW_free( pTable->pcAclbuf , 0 );
		pTable->pcAclbuf = NULL;

 		fnDRevoke_err( pTable , pulRetlength , MODEERR );
		return  (-1);
	}

	ushKyokabit = ushKyokabit ^ 0x02FB;


	ushKyokabit = ushKyokabit & (NW_kenrimap->trustee_list);

	GW_free(pTable->pcAclbuf,0);
	pTable->pcAclbuf = NULL;

	ulUG_length = strlen(aszDCEuid);




	rtn = fnDRevoke_set( plUtbl , pTable , pcNetpath , pcDFSpath , pucRetbuf , pulRetlength,
				   		 ushMode , ushKyokabit , ulUG_length , pBuffer , aszDCEuid );

	GW_free( pcDFSpath , 0 );
	GW_free( aszDCEuid , 0 );
	GW_free( pBuffer , 0 );

	return  rtn;
}


/*EnglishComment**************************************************************/
/* ID   = fnDRevoke_err                                                      */
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

void fnDRevoke_err( struct TableKeep *pTable , unsigned long *pulRetlength , int   Err_no )
{
	(pTable->plRet_st)->DFAM_errno = Err_no;
	(pTable->plRet_st)->ret_value  = FALSE;
	(pTable->plRet_st)->next_frame = FALSE;
	*pulRetlength = sizeof( struct RetHeader );
}


/*EnglishComment**************************************************************/
/* ID   = fnDRevoke_set                                                      */
/*                                                                           */
/* name = Setting the results for the trustee information modification       */
/* func = Sets the result information to the send buffer to be sent to Client*/
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pcNetpath   :char *            :inp: NetWare path                  */
/*        pcDFSpath   :char *            :inp: DFS path name                 */
/*        pucRetbuf   :unsigned char *   :i/o: Gateway send buffer           */
/*        pulRetlength:unsigned long *   :i/o: size of Gateway send buffer   */
/*        ushMode     :unsigned short    :inp: mode information              */
/*        ushKyokabit :unsigned short    :inp: allowed/prohibitted indicator */
/*        ulUG_length :unsigned long     :inp: size of user group name       */
/*        pBuffer     :struct dstat *    :inp: file directory information    */
/*        aszDCEuid   :char *            :inp: pointer to user/group name    */
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

int fnDRevoke_set(	 struct GTGroup_t    *plUtbl,
					 struct TableKeep    *pTable,
					 char                *pcNetpath,
					 char                *pcDFSpath,
					 unsigned char       *pucRetbuf,
					 unsigned long       *pulRetlength,
					 unsigned short      ushMode,
					 unsigned short      ushKyokabit,
					 unsigned long       ulUG_length,
					 struct dstat        *pBuffer,
					 char                *aszDCEuid )
{
	struct RetRevokeData         *data_stp;
	int                          rtn;
	char                         *pcHeaderLastData;
	unsigned char                *indexp;
	struct RetRevokeDataKeep     **icountp;

	data_stp = GW_malloc( sizeof(struct RetRevokeData) );
	if( data_stp == 0 )
	{
		(pTable->plRet_st)->DFAM_errno = GWMEMERR;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		return  (-1);
	}
	memset( data_stp , 0x00 , sizeof(struct RetRevokeData) );




	rtn = fnDRevoke_main( plUtbl , pTable , pcNetpath , pcDFSpath ,
				          ushMode , ushKyokabit , ulUG_length , pBuffer , aszDCEuid , data_stp );

	(pTable->plRet_st)->DFAM_errno = rtn;
	if( rtn != NORMAL )
	{
		GW_free( data_stp , 0 );

		if( rtn == NOENT )
		{
			return  (-2);
		}
		if( rtn == NOTRLST )
		{
			return  (-3);
		}
		if( rtn == NORIGHT )
		{
			return  (-4);
		}
		if( rtn == PATHERR )
		{
			return  (-5);
		}
		if( rtn == ENOTLFS )
		{
			return  (-6);
		}
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof( struct RetHeader );

		return  (-1);
	}

	(pTable->plRet_st)->ret_value  = TRUE;

	pcHeaderLastData = pucRetbuf+(*pulRetlength)-sizeof(struct RetRevokeData);
	if (((char *)(pTable->plRet_rvdata)) <= pcHeaderLastData)
	{
		(pTable->plRet_st)->next_frame = FALSE;
		data_stp->next_data = FALSE;

		*(pTable->plRet_rvdata) = *data_stp;

		indexp = pucRetbuf + ( sizeof( struct RetHeader) );

		while((struct RetRevokeData *)indexp != pTable->plRet_rvdata)
		{
			((struct RetRevokeData *)indexp)->next_data = TRUE;
			indexp += sizeof( struct RetRevokeData );
		}

		pTable->plRet_rvdata++;
	}

	else
	{
		(pTable->plRet_st)->next_frame = TRUE;

		pTable->ppRvDataKeep = &(pTable->pData);

		if( *(pTable->ppRvDataKeep) == NULL)
		{
			*(pTable->ppRvDataKeep) = GW_malloc(sizeof(struct RetRevokeDataKeep));
			if (*(pTable->ppRvDataKeep) == NULL)
			{
				(pTable->plRet_st)->DFAM_errno = GWMEMERR;
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);

				GW_free( data_stp , 0 );

				return  (-1);
			}
			(*(pTable->ppRvDataKeep))->data = *data_stp;
			(*(pTable->ppRvDataKeep))->pNext = NULL;
		}

		else
		{
			icountp = &(pTable->pData);
			while (*icountp != NULL)
			{
				icountp = &((*icountp)->pNext);
			}

			*icountp = GW_malloc(sizeof(struct RetRevokeDataKeep));
			if (*icountp == NULL)
			{
				(pTable->plRet_st)->DFAM_errno = GWMEMERR;
				(pTable->plRet_st)->ret_value  = FALSE;
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

	*pulRetlength = (((char *)pTable->plRet_rvdata))-pucRetbuf;

	return 0;
}


/*EnglishComment**************************************************************/
/* ID   = fnDRevoke_main                                                     */
/*                                                                           */
/* name = Modifying trustee (main routine)                                   */
/* func = Sends Agent a modifying-trustee request.                           */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pcNetpath   :char *            :inp: NetWare path                  */
/*        pcDFSpath   :char *            :inp: DFS path name                 */
/*        ushMode     :unsigned short    :inp: mode information              */
/*        ushKyokabit :unsigned short    :inp: allowed/prohibitted indicator */
/*        ulUG_length :unsigned long     :inp: size of user group name       */
/*        pBuffer     :struct dstat *    :inp: file directory information    */
/*        aszDCEuid   :char *            :inp: pointer to user/group name    */
/*        data_stp    :struct RetGrantData *:out: structure for Gateway's    */
/*                                                send data                  */
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

int  fnDRevoke_main( struct GTGroup_t      *plUtbl,
					 struct TableKeep      *pTable,
					 char                  *pcNetpath,
					 char                  *pcDFSpath,
					 unsigned short        ushMode,
					 unsigned short        ushKyokabit,
					 unsigned long         ulUG_length,
					 struct dstat          *pBuffer,
					 char                  *aszDCEuid,
					 struct RetRevokeData  *data_stp)
{
	struct RightsMapping         RightsMap;
	struct RightsMapping         *NW_kenrimap;
	int                          iRet;
	unsigned int                 uiKyokabit;
	int                          iAcl_length;
	int                          DFS_Errno;
	unsigned int                 uiCount;
	unsigned int                 uiEntry;
	struct GetTrustee            *pGetTrustee;
	unsigned int                 oflen;
	unsigned int                 uglen;
	unsigned char                *charp;

	iAcl_length = 0;
	NW_kenrimap = &RightsMap;
	if (pBuffer->d_mode & DS_IFDIR)
	{
		NW_kenrimap->mode = T_DIR;
	}
	else
	{
		NW_kenrimap->mode = T_FILE;
	}
	NW_kenrimap->trustee_list = ushKyokabit;




	iRet = GW_Rights_Map( NW_kenrimap );

	switch( iRet )
	{
		case SUCCESS:
						break;
		case MAP_ERR:
						NW_kenrimap->ACL_list = 0;
						break;
		case MODE_ERR:
		default:
						return  MODEERR;
	}


	pTable->pcAclbuf = GW_malloc(sizeof(unsigned int)+sizeof(unsigned long)+ulUG_length);

	if ( (pTable->pcAclbuf) == NULL )
	{
		return  GWMEMERR;
	}


	oflen = sizeof( unsigned int );

	*( ( unsigned int * )( pTable->pcAclbuf ) ) = NW_kenrimap->ACL_list;

	*( ( unsigned long * ) ( ( pTable->pcAclbuf ) + oflen ) ) = ulUG_length;

	memmove( ( pTable->pcAclbuf ) + oflen + sizeof( unsigned long ),aszDCEuid,ulUG_length );


	iAcl_length = sizeof( unsigned int ) + sizeof( unsigned long ) +
				  ( *( unsigned long * )( ( pTable->pcAclbuf ) + sizeof( unsigned int ) ) );

	NW_kenrimap->mode         = 0;
	NW_kenrimap->trustee_list = 0;
	NW_kenrimap->trustee_list = 0;




	iRet = DFSsetacl_syscallReq( pcDFSpath , ushMode , pTable->pcAclbuf ,
								 &iAcl_length , &DFS_Errno , plUtbl );
	if (iRet == LIB_ERR)
	{
		GW_free( pTable->pcAclbuf , 0 );
		pTable->pcAclbuf = NULL;

		return  fnErrChange(DFS_Errno,plUtbl);
	}

	GW_free( pTable->pcAclbuf , 0 );
	pTable->pcAclbuf = NULL;

	pTable->pcAclbuf = GW_malloc(2048);
	if ( (pTable->pcAclbuf) == NULL )
	{
		return  GWMEMERR;
	}
	iAcl_length = 2048;




	iRet = DFSgetacl_syscallReq( pcDFSpath , ushMode , pTable->pcAclbuf,
                   			     &iAcl_length , &DFS_Errno , plUtbl );

	while( iRet == RETRY )
	{
		GW_free( pTable->pcAclbuf , 0 );
		pTable->pcAclbuf = NULL;
		pTable->pcAclbuf = GW_malloc(iAcl_length);

		if ((pTable->pcAclbuf) == NULL)
		{
			return  GWMEMERR;
		}



		iRet = DFSgetacl_syscallReq( pcDFSpath , ushMode , pTable->pcAclbuf ,
                   					 &iAcl_length , &DFS_Errno , plUtbl );
	}
	if ( iRet != SUCCESS )
	{

		GW_free(pTable->pcAclbuf,0);
		pTable->pcAclbuf = NULL;

		return  fnErrChange(DFS_Errno,plUtbl);
	}



	uiEntry = *((unsigned int *)(pTable->pcAclbuf));
	pGetTrustee = (struct GetTrustee *)((pTable->pcAclbuf)+(sizeof(unsigned int)));
	uiKyokabit = 0;
	for ( uiCount = 0 ; uiCount < uiEntry ; uiCount++ )
	{
		if( pGetTrustee->ug_length == 0xffffffff )
		{
			charp = (unsigned char *)pGetTrustee;
			charp = charp + sizeof(unsigned int) + sizeof(short) +
					sizeof(unsigned int);
			pGetTrustee = (struct GetTrustee *)charp;
		}
		else
		{
			if (pGetTrustee->ug_length == (unsigned int)ulUG_length)
			{
				if (SUCCESS == strncmp(pGetTrustee->ug_name,aszDCEuid,ulUG_length))
				{
					uiKyokabit = pGetTrustee->kyokabit;
					break;
				}
			}
			uglen = pGetTrustee->ug_length;
			charp = (unsigned char *)pGetTrustee;
			charp = charp + sizeof(unsigned int) + sizeof(short) +
					sizeof(unsigned int) + uglen;
			pGetTrustee = (struct GetTrustee *)charp;
		}
	}
	GW_free(pTable->pcAclbuf,0);
	pTable->pcAclbuf = NULL;

	if ( pBuffer->d_mode & DS_IFDIR )
	{
		NW_kenrimap->mode = ACL_DIR;
	}
	else
	{
		NW_kenrimap->mode = ACL_FILE;
	}
	NW_kenrimap->ACL_list = uiKyokabit;




	iRet = GW_Rights_Map(NW_kenrimap);

	if( iRet != SUCCESS )
	{
		return  MODEERR;
	}




	iRet = fnKenriMap( NW_kenrimap->trustee_list , data_stp->t_list );


	strcpy( data_stp->path_name , pcNetpath );

	if ( pBuffer->d_mode & DS_IFDIR )
	{
		data_stp->dir_info = C_DIR;
	}
	else
	{
		data_stp->dir_info = C_FILE;
	}

	return  NORMAL;
}
