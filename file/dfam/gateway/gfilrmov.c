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
 * $Log: gfilrmov.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:16  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:35  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:59:59  takano
 * 	First Submit
 * 	[1996/02/21  07:59:54  takano]
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
#include "err.h"
#include "gfiler.h"


/*EnglishComment**************************************************************/
/* ID   = fnDRemove                                                          */
/*                                                                           */
/* name = Deleting the trustee information (main routine)                    */
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


void fnDRemove(struct GTGroup_t  *plUtbl,
			   int               iTask_num,
			   unsigned char     *pucBuf,
			   unsigned long     pulBuflen,
			   unsigned char     *pucRetbuf,
			   unsigned long     *pulRetlength)
{
	struct TableKeep         **pTaskTb;
	struct RetRemoveDataKeep  *pKeep;
	struct RetRemoveDataKeep  *pUpdate;
	struct TableKeep          *pTable;
	struct RetHeader          *plHeader;

	memset(pucRetbuf,0x00,*pulRetlength);

	pTaskTb = &(plUtbl->pTblKeep_sp);

    plHeader = (struct RetHeader *)pucRetbuf;

	while(*pTaskTb != NULL)
	{
		if((*pTaskTb)->task_num == iTask_num)
		{
			pTable = *pTaskTb;
			if (pulBuflen == COMMLEN)
			{

				fnDRemoveContinue( pTable , pucBuf , pucRetbuf , pulRetlength );
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
					memcpy(plHeader->command_name,pucBuf,COMMLEN);
					plHeader->DFAM_errno = GWMEMERR;
					plHeader->ret_value = FALSE;
					plHeader->next_frame = FALSE;
					*pulRetlength = sizeof(struct RetHeader);

					fnCommnErrLog( pucRetbuf );

					return;
				}
				memset(pTable,0x00,sizeof(struct TableKeep));

				memcpy(pTable->command_name,pucBuf,COMMLEN);
				pTable->task_num = iTask_num;

				pTable->pNext = plUtbl->pTblKeep_sp;
				plUtbl->pTblKeep_sp = pTable;

				fnDRemoveNew(plUtbl,pTable,pucBuf,pucRetbuf,pulRetlength);
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

	fnDRemoveNew(plUtbl,pTable,pucBuf,pucRetbuf,pulRetlength);
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
/* ID   = fnDRemoveContinue                                                  */
/*                                                                           */
/* name = Deleting trustee (processing for the continued information)        */
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
/*  date = February 1, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/

void fnDRemoveContinue( struct TableKeep *pTable,
					    unsigned char    *pucBuf,
					    unsigned char    *pucRetbuf,
					    unsigned long    *pulRetlength)
{
	unsigned char            *pcHeaderLastData;
	struct RetRemoveDataKeep *pDataTop;
	unsigned char            *indexp;

	pTable->plRet_st = (struct RetHeader *)pucRetbuf;
	pTable->plRet_rmdata = (struct RetRemoveData *)(pucRetbuf+sizeof(struct RetHeader));
	memmove( ( pTable->plRet_st)->command_name , pucBuf , COMMLEN );
	(pTable->plRet_st)->ret_value  = TRUE;
	(pTable->plRet_st)->DFAM_errno = NORMAL;

	pDataTop = pTable->pData;

	pcHeaderLastData = pucRetbuf+(*pulRetlength)-sizeof(struct RetRemoveData);


	(pTable->plRet_st)->next_frame = FALSE;

	while ( pDataTop != NULL )
	{
		if ((char *)((pTable->plRet_rmdata)) <= pcHeaderLastData)
		{
			*(pTable->plRet_rmdata) = pDataTop->data;
			(pTable->plRet_rmdata)->next_data = FALSE;
			pTable->pData = pDataTop->pNext;
			GW_free(pDataTop,0);
			pDataTop = NULL;

			pDataTop = pTable->pData;

			indexp = pucRetbuf + ( sizeof( struct RetHeader ) );
			while( (struct RetRemoveData *)indexp != pTable->plRet_rmdata)
			{
				((struct RetRemoveData *)indexp)->next_data = TRUE;
				indexp += sizeof( struct RetRemoveData );
			}
			pTable->plRet_rmdata++;
		}

		else
		{
			(pTable->plRet_st)->next_frame = TRUE;
			break;
		}
	}


	*pulRetlength = ((char *)(pTable->plRet_rmdata))-pucRetbuf;
}


/*EnglishComment**************************************************************/
/* ID   = fnDRemoveNew                                                       */
/*                                                                           */
/* name = Deleting trustee (processing for the new information)              */
/* func = In the case that Client sent "new information"                     */
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


void fnDRemoveNew(struct GTGroup_t *plUtbl,
				  struct TableKeep *pTable,
				  unsigned char    *pucBuf,
				  unsigned char    *pucRetbuf,
				  unsigned long    *pulRetlength)
{
	struct dremove_data 	*plDremove;
	char                 	aTarget[3];
	int                  	delimita;
	int                  	iPathlength;
	int                  	iValue;
	DIR                 	*dirStructp;
	DIR                 	*direntp;
	int                  	NW_err_num;
	unsigned char       	*path_name;
	int                 	i;
	int						rtn;

	pTable->plRet_st = (struct RetHeader *)pucRetbuf;
	pTable->plRet_rmdata = (struct RetRemoveData *)(pucRetbuf+sizeof(struct RetHeader));
	plDremove = (struct dremove_data *)pucBuf;
	if(plDremove->path_name[0] == NULL)
	{
		memmove((pTable->plRet_st)->command_name,pucBuf,COMMLEN);
		(pTable->plRet_st)->DFAM_errno = NULLPATH;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		return;
	}

	iPathlength = strlen(plDremove->path_name);
	aTarget[0] = '*';
	aTarget[1] = '?';
	aTarget[2] = '\0';


	if( D_strpbrk( plDremove->path_name , aTarget ) == NULL )
	{
		delimita = iPathlength;
	}
	else
	{
		delimita = iPathlength -1 ;
	}

	memmove((pTable->plRet_st)->command_name,pucBuf,COMMLEN);
	(pTable->plRet_st)->ret_value = TRUE;
	(pTable->plRet_st)->DFAM_errno = NORMAL;

	if(iPathlength <= delimita)
	{



		rtn = fnDRemove_op(plUtbl,pTable,(char *)plDremove->path_name,plDremove,pucRetbuf,pulRetlength);
		if( rtn == -2 )
		{
			fnDRemove_err( pTable , pulRetlength , NOENT );
		}
		if( rtn == -3 )
		{
			fnDRemove_err( pTable , pulRetlength , NOTRLST );
		}
		if( rtn == -4 )
		{
			fnDRemove_err( pTable , pulRetlength , NOTDEL );
		}
		if( rtn == -5 )
		{
			fnDRemove_err( pTable , pulRetlength , NORIGHT );
		}
		if( rtn == -6 )
		{
			fnDRemove_err( pTable , pulRetlength , PATHERR );
		}
		if( rtn == -7 )
		{
			fnDRemove_err( pTable , pulRetlength , ENOTLFS );
		}
		return;
	}




	dirStructp = opendir( plDremove->path_name );

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
			(pTable->plRet_st)->ret_value  = FALSE;
		}
		(pTable->plRet_st)->next_frame = FALSE;




		closedir( dirStructp );

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

	while ( direntp != NULL )
	{
		strcpy( path_name , plDremove->path_name );
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




		rtn = fnDRemove_op(plUtbl,pTable,(char *)path_name,plDremove,pucRetbuf,pulRetlength);
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
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;




          		closedir( dirStructp );

				*pulRetlength = sizeof(struct RetHeader);
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
				(pTable->plRet_st)->DFAM_errno = NOTDEL;
				(pTable->plRet_st)->ret_value  = FALSE;
			}
			else if( rtn == -5 )
			{
				(pTable->plRet_st)->DFAM_errno = NORIGHT;
				(pTable->plRet_st)->ret_value  = FALSE;
			}
			else if( rtn == -6 )
			{
				(pTable->plRet_st)->DFAM_errno = PATHERR;
				(pTable->plRet_st)->ret_value  = FALSE;
			}
			else if( rtn == -7 )
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




	iValue = closedir( dirStructp );

	if(iValue != ESUCCESS)
	{
		NW_err_num = NetWareErrno;
		(pTable->plRet_st)->DFAM_errno = fnNW_change_errno(NW_err_num);
		(pTable->plRet_st)->ret_value = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
       	return;
	}
}


/*EnglishComment**************************************************************/
/* ID   = fnDRemove_op                                                       */
/*                                                                           */
/* name = Trustee deleting operations                                        */
/* func = Sets information to be sent to Agent.                              */
/*           - File name conversion                                          */
/*           - Mapping trustee                                               */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pcNetpath   :char            * :inp: NetWare path                  */
/*        plDremove   :struct dremove_data * :inp: Gateway receive buffer    */
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

int  fnDRemove_op(struct GTGroup_t     *plUtbl,
				  struct TableKeep     *pTable,
				  char                 *pcNetpath,
				  struct dremove_data  *plDremove,
				  unsigned char        *pucRetbuf,
				  unsigned long        *pulRetlength)

{
	char                 *pcDFSpath;
	int                  iRet;
	unsigned int         ushMode;
	unsigned long        ulUG_length;
	char                 *Volume_path;
    char                 *aszDCEuid;
	char                 *pWork;
	int                  i;
	int					 rtn;

	memmove((pTable->plRet_st)->command_name,plDremove->command_name,COMMLEN);



	pWork = pcNetpath;


	while((*pWork != '\/') && (*pWork != '\\') && (*pWork != '\0'))
	{
		pWork++;
	}


	if((*pWork == '\0') || ((*(++pWork)) == '\0'))
	{
		return (-2);
	}

	Volume_path = GW_malloc(_MAX_PATH+1);

	if(Volume_path == NULL)
	{
		fnDRemove_err( pTable , pulRetlength , GWMEMERR );
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

		fnDRemove_err( pTable , pulRetlength , GWMEMERR );
		return  (-1);
	}




	iRet = GetDFSPath( Volume_path , pcDFSpath , CHECKMODE );

	GW_free( Volume_path , 0 );

	if( iRet != GWERR_NORMAL )
	{
		GW_free( pcDFSpath , 0 );

		switch( iRet )
		{

			case GWERR_NOT_DFAMVOL:
							fnDRemove_err( pTable , pulRetlength , NOVOL );
							return  (-1);
			case GWERR_NOMEM:
							fnDRemove_err( pTable , pulRetlength , GWMEMERR );
							return  (-1);
			case GWERR_LONG:
							fnDRemove_err( pTable , pulRetlength , TOOLONG );
							return  (-1);
			case GWERR_INVAL:
			case GWERR_NUSE:
			case GWERR_NW_NOENT:
							return  (-2);
			default:
							fnDRemove_err( pTable , pulRetlength , ABNORMAL );
							return  (-1);
		}
	}

	switch( plDremove->ug_class )
	{
		case DEFAULT_USER:
							ushMode = DS_USER;
							break;
		case DEFAULT_GROUP:
							ushMode = DS_GROUP;
							break;
		default:
							GW_free(pcDFSpath,0);
							fnDRemove_err( pTable , pulRetlength , ERROPT );
							return  (-1);
	}
	aszDCEuid = GW_malloc(DFSGRP_MAX+1);
	if( aszDCEuid == NULL )
	{
		GW_free(pcDFSpath,0);

		fnDRemove_err( pTable , pulRetlength , GWMEMERR );
		return  (-1);
	}

	if( plDremove->ug_class == DEFAULT_USER )
	{



		iRet = GetDCEUserName(plDremove->name,aszDCEuid);
	}
	else
	{



		iRet = GW_DCEGroup_Map(plDremove->name,aszDCEuid);
	}
	if(iRet != 0)
	{
		GW_free(pcDFSpath,0);
		GW_free(aszDCEuid,0);

		switch( iRet )
		{
			case -1:
					if(plDremove->ug_class == DEFAULT_USER)
					{
						fnDRemove_err( pTable , pulRetlength , NOUSER );
					}
					else
					{
						fnDRemove_err( pTable , pulRetlength , NOGRP );
					}
					return  (-1);
			case -2:
					fnDRemove_err( pTable , pulRetlength , NODFAMUSER );
					return  (-1);
			case -3:
					fnDRemove_err( pTable , pulRetlength , GWMEMERR );
					return  (-1);
			case -4:
					fnDRemove_err( pTable , pulRetlength , BINDERR );
					return  (-1);
			default:
					fnDRemove_err( pTable , pulRetlength , FAILECHG );
					return  (-1);
		}
	}

	ulUG_length = strlen(aszDCEuid);

	pTable->pcAclbuf =(char *)GW_malloc(sizeof(unsigned long)+ulUG_length);

	if ( (pTable->pcAclbuf) == NULL )
	{
		GW_free(pcDFSpath,0);
		GW_free(aszDCEuid,0);

		fnDRemove_err( pTable , pulRetlength , GWMEMERR );
		return  (-1);
	}

	*( (unsigned long *)(pTable->pcAclbuf) ) = ulUG_length;
	memcpy((pTable->pcAclbuf)+sizeof(unsigned long),aszDCEuid,ulUG_length);




	rtn = fnDRemove_set(plUtbl,pTable,pcDFSpath,ushMode,pcNetpath,pucRetbuf,pulRetlength);

	GW_free( pcDFSpath , 0 );
	GW_free( aszDCEuid , 0 );
	GW_free( pTable->pcAclbuf , 0 );
	pTable->pcAclbuf = NULL;

	return rtn;
}


/*EnglishComment**************************************************************/
/* ID   = fnDRemove_err                                                      */
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

void fnDRemove_err( struct TableKeep *pTable , unsigned long *pulRetlength , int   Err_no )
{
	(pTable->plRet_st)->DFAM_errno = Err_no;
	(pTable->plRet_st)->ret_value  = FALSE;
	(pTable->plRet_st)->next_frame = FALSE;
	*pulRetlength = sizeof( struct RetHeader );
}


/*EnglishComment**************************************************************/
/* ID   = fnDRemove_set                                                      */
/*                                                                           */
/* name = Setting the results (for the trustee information deleting)         */
/* func = Sets the result information to the send buffer to be sent to       */
/*        Client, and sends a "trustee information modifying" request to     */
/*        Agent.                                                             */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pcDFSpath   :char *            :inp: DFS path name                 */
/*        ushMode     :unsigned short    :inp: mode information              */
/*        pcNetpath   :char *            :inp: NetWare path name             */
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

int  fnDRemove_set(struct GTGroup_t    *plUtbl,
				   struct TableKeep    *pTable,
				   char                *pcDFSpath,
				   unsigned short      ushMode,
				   char                *pcNetpath,
				   unsigned char       *pucRetbuf,
				   unsigned long       *pulRetlength)
{
	int                            iRet;
	unsigned long                  iAcl_length;
	int                            DFS_Errno;
	struct RetRemoveData           data_st;
	char                           *pcHeaderLastData;
	struct dstat                   *pBuffer;
	unsigned char                  *indexp;
	struct RetRemoveDataKeep       **icountp;

	memset( &data_st , 0x00 , sizeof(struct RetRemoveData) );

	iAcl_length = 0;
	iAcl_length = sizeof(unsigned long)+(*((unsigned long *)(pTable->pcAclbuf)));

	pBuffer = (struct dstat *)GW_malloc(sizeof(struct dstat));

	if ( pBuffer == NULL )
	{
		fnDRemove_err( pTable , pulRetlength , GWMEMERR );
		return  (-1);
	}




	iRet = DFSstatReq( pcDFSpath , pBuffer , &DFS_Errno , plUtbl );

	if (iRet == LIB_ERR)
	{
		GW_free(pBuffer,0);

		if( fnErrChange(DFS_Errno,plUtbl) == NOENT )
		{
			return  (-2);
		}
		if( fnErrChange(DFS_Errno,plUtbl) == NOTRLST )
		{
			return  (-3);
		}
		if( fnErrChange(DFS_Errno,plUtbl) == NOTDEL )
		{
			return  (-4);
		}
		if( fnErrChange(DFS_Errno,plUtbl) == NORIGHT )
		{
			return  (-5);
		}
		if( fnErrChange(DFS_Errno,plUtbl) == PATHERR )
		{
			return  (-6);
		}
		if( fnErrChange(DFS_Errno,plUtbl) == ENOTLFS )
		{
			return  (-7);
		}
		fnDRemove_err( pTable , pulRetlength , fnErrChange(DFS_Errno,plUtbl) );
		return  (-1);
	}
	if (pBuffer->d_mode & DS_IFDIR)
	{
		data_st.dir_info = C_DIR;
	}
	else
	{
		data_st.dir_info = C_FILE;
	}

	GW_free( pBuffer , 0 );




	iRet = DFSdelacl_syscallReq( pcDFSpath , ushMode , pTable->pcAclbuf,
								(int *)&iAcl_length , &DFS_Errno , plUtbl );

	if (iRet == LIB_ERR)
	{
		if( fnErrChange(DFS_Errno,plUtbl) == NOENT )
		{
			return  (-2);
		}
		if( fnErrChange(DFS_Errno,plUtbl) == NOTRLST )
		{
			return  (-3);
		}
		if( fnErrChange(DFS_Errno,plUtbl) == NOTDEL )
		{
			return  (-4);
		}
		if( fnErrChange(DFS_Errno,plUtbl) == NORIGHT )
		{
			return  (-5);
		}
		if( fnErrChange(DFS_Errno,plUtbl) == PATHERR )
		{
			return  (-6);
		}
		if( fnErrChange(DFS_Errno,plUtbl) == ENOTLFS )
		{
			return  (-7);
		}

		fnDRemove_err( pTable , pulRetlength , fnErrChange(DFS_Errno,plUtbl) );
		return  (-1);
	}
	strcpy( data_st.path_name , pcNetpath );

	(pTable->plRet_st)->DFAM_errno = NORMAL;
	(pTable->plRet_st)->ret_value  = TRUE;

	pcHeaderLastData = pucRetbuf+(*pulRetlength)-sizeof(struct RetRemoveData);
	if (((char *)(pTable->plRet_rmdata)) <= pcHeaderLastData)
	{
		(pTable->plRet_st)->next_frame = FALSE;
		data_st.next_data = FALSE;
		*(pTable->plRet_rmdata) = data_st;
		indexp = pucRetbuf + ( sizeof( struct RetHeader) );
		while((struct RetRemoveData *)indexp != pTable->plRet_rmdata)
		{
			((struct RetRemoveData *)indexp)->next_data = TRUE;
			indexp += sizeof( struct RetRemoveData );
		}
		pTable->plRet_rmdata++;
	}
	else
	{
		(pTable->plRet_st)->next_frame = TRUE;
		pTable->ppRmDataKeep = &(pTable->pData);
		if( *(pTable->ppRmDataKeep) == NULL)
		{
			*(pTable->ppRmDataKeep) = GW_malloc(sizeof(struct RetRemoveDataKeep));
			if (*(pTable->ppRmDataKeep) == NULL)
			{
				(pTable->plRet_st)->DFAM_errno = GWMEMERR;
				(pTable->plRet_st)->ret_value = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);
				return  (-1);
			}
			(*(pTable->ppRmDataKeep))->data = data_st;
			(*(pTable->ppRmDataKeep))->pNext = NULL;
		}
		else
		{
			icountp = &(pTable->pData);
			while (*icountp != NULL)
			{
				icountp = &((*icountp)->pNext);
			}
			*icountp = GW_malloc(sizeof(struct RetRemoveDataKeep));
			if (*icountp == NULL)
			{
				(pTable->plRet_st)->DFAM_errno = GWMEMERR;
				(pTable->plRet_st)->ret_value = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof(struct RetHeader);
				return  (-1);
			}
			(*icountp)->data = data_st;
			(*icountp)->pNext = NULL;
		}
	}

	*pulRetlength = ((char *)(pTable->plRet_rmdata))-pucRetbuf;

	return NORMAL;
}
