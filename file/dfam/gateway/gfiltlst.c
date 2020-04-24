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
 * $Log: gfiltlst.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:19  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:37  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:00:59  takano
 * 	First Submit
 * 	[1996/02/21  08:00:55  takano]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <nwdir.h>
#include <string.h>
#include <nwenvrn.h>
#include <malloc.h>
#include <nwdir.h>
#include <process.h>
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
/* ID   = fnDTlist                                                           */
/*                                                                           */
/* name = Getting ACL trustee (main routine)                                 */
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
/*  date = May 7, 1995 by Jun Nakagawa                                       */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/


void fnDTlist(  struct GTGroup_t  *plUtbl,
				int               iTask_num,
				unsigned char     *pucBuf,
				unsigned long     pulBuflen,
				unsigned char     *pucRetbuf,
				unsigned long     *pulRetlength )
{
	struct TableKeep        	  **pTaskTb;
	struct RetTlistDataKeep       *pKeep;
	struct RetTlistDataKeep       *pUpdate;
	struct TableKeep              *pTable;
	struct RetHeader              *plHeader;

	memset(pucRetbuf,0x00,*pulRetlength);

    plHeader = (struct RetHeader *)pucRetbuf;

	pTaskTb = &(plUtbl->pTblKeep_sp);

	while(*pTaskTb != NULL )
	{
		if((*pTaskTb)->task_num == iTask_num)

		{
			pTable = *pTaskTb;
			if (pulBuflen == COMMLEN)
			{

				fnDTlistContinue(plUtbl,pTable,pucBuf,pulBuflen,pucRetbuf,pulRetlength);
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

				fnDTlistNew(plUtbl,pTable,pucBuf,pucRetbuf,pulRetlength);
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

	fnDTlistNew( plUtbl , pTable , pucBuf , pucRetbuf , pulRetlength );
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
/* ID   = fnDTlistContinue                                                   */
/*                                                                           */
/* name = Getting trustee (processing for the continued information)         */
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

void fnDTlistContinue(struct GTGroup_t *plUtbl,
					  struct TableKeep *pTable,
					  unsigned char    *pucBuf,
					  unsigned long    pulBuflen,
					  unsigned char    *pucRetbuf,
					  unsigned long    *pulRetlength)
{
	char                     *pcHeaderLastData;
	struct RetTlistDataKeep  *pDataTop;
	unsigned char            *indexp;
	struct GTGroup_t         *workp;
	unsigned long            worklen;

	workp = plUtbl;

	pTable->plRet_st = (struct RetHeader *)pucRetbuf;

	worklen = pulBuflen;

	pTable->plRet_tdata = (struct RetTlistData *)(pucRetbuf+sizeof(struct RetHeader));

	memmove((pTable->plRet_st)->command_name,pucBuf,COMMLEN);
	(pTable->plRet_st)->DFAM_errno = NORMAL;
	(pTable->plRet_st)->ret_value  = TRUE;

	pDataTop = pTable->pData;

	pcHeaderLastData = pucRetbuf+(*pulRetlength)-sizeof(struct RetTlistData);


	(pTable->plRet_st)->next_frame = FALSE;

	while ( pDataTop != NULL )
	{
		if ((char *)((pTable->plRet_tdata)) <= pcHeaderLastData)
		{

			*(pTable->plRet_tdata) = pDataTop->data;
			(pTable->plRet_tdata)->next_data = FALSE;
			pTable->pData = pDataTop->pNext;
			GW_free(pDataTop,0);
			pDataTop = NULL;
			pDataTop = pTable->pData;

			indexp = pucRetbuf + ( sizeof( struct RetHeader ) );
			while( (struct RetTlistData *)indexp != pTable->plRet_tdata )
			{
				((struct RetTlistData *)indexp)->next_data = TRUE;
				indexp += sizeof( struct RetTlistData );
			}
			pTable->plRet_tdata++;
		}

		else
		{
			(pTable->plRet_st)->next_frame = TRUE;
			break;
		}
	}


	*pulRetlength = ((char *)(pTable->plRet_tdata))-pucRetbuf;
}


/*EnglishComment**************************************************************/
/* ID   = fnDTlistNew                                                        */
/*                                                                           */
/* name = Getting trustee (processing for the new information)               */
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
/*  date = January 30, 1995 by Jun Nakagawa                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/

void fnDTlistNew(struct GTGroup_t   *plUtbl,
				 struct TableKeep   *pTable,
				 unsigned char      *pucBuf,
				 unsigned char      *pucRetbuf,
				 unsigned long      *pulRetlength)
{
	struct dtlist_data              *plDtlist;
	char                            aTarget[3];
	int                             delimita;
	int                             iPathlength;
	int                             iValue;
	DIR                             *dirStructp;
	DIR                             *direntp;
	int                             NW_err_num;
	unsigned char                   *path_name;
	int   				            i;
	int								rtn;

	pTable->plRet_st = (struct RetHeader *)pucRetbuf;

	pTable->plRet_tdata = (struct RetTlistData *)(pucRetbuf+sizeof(struct RetHeader));

	plDtlist = (struct dtlist_data *)pucBuf;

	if(plDtlist->path_name[0] == NULL)
	{
		memmove((pTable->plRet_st)->command_name,pucBuf,COMMLEN);
		(pTable->plRet_st)->DFAM_errno = NULLPATH;
		(pTable->plRet_st)->ret_value  = FALSE;
		(pTable->plRet_st)->next_frame = FALSE;
		*pulRetlength = sizeof(struct RetHeader);
		return;
	}

	iPathlength = strlen(plDtlist->path_name);
	aTarget[0] = '*';
	aTarget[1] = '?';
	aTarget[2] = '\0';


	if( D_strpbrk( plDtlist->path_name , aTarget) == NULL )
	{
		delimita = iPathlength;
	}
	else
	{
		delimita = iPathlength - 1 ;
	}

	memmove( (pTable->plRet_st)->command_name , plDtlist->command_name , COMMLEN );

	(pTable->plRet_st)->DFAM_errno = NORMAL;
	(pTable->plRet_st)->ret_value  = TRUE;



	if( iPathlength <= delimita )
	{





		rtn = fnGetTrustee(plUtbl,pTable,(char *)plDtlist->path_name,plDtlist,pucRetbuf,pulRetlength);
		if( rtn == -2 )
		{
			fnDtlist_err( pTable , pulRetlength , NOENT );
		}
		if( rtn == -3 )
		{
			fnDtlist_err( pTable , pulRetlength , PATHERR );
		}

		return;
	}



	dirStructp = opendir( plDtlist->path_name );

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





	direntp = readdir(dirStructp);

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
		iValue = closedir(dirStructp);
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
		strcpy( path_name , plDtlist->path_name );
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




		rtn = fnGetTrustee( plUtbl , pTable , (char *)path_name , plDtlist , pucRetbuf , pulRetlength );
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




               	closedir(dirStructp);

				NW_err_num = NetWareErrno;
				(pTable->plRet_st)->DFAM_errno = fnNW_change_errno(NW_err_num);
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
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
				(pTable->plRet_st)->DFAM_errno = PATHERR;
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
	}
}


/*EnglishComment**************************************************************/
/* ID   = fnGetTrustee                                                       */
/*                                                                           */
/* name = Operations to get trustee                                          */
/* func = (1) Sets information to be sent to Agent.                          */
/*        (2) Gets trustee                                                   */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pcNetpath   :char            * :inp: NetWare path                  */
/*        plDtlist    :struct dtlist_data *:inp: Gateway receive buffer      */
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

int  fnGetTrustee(struct GTGroup_t    *plUtbl,
				  struct TableKeep    *pTable,
				  char                *pcNetpath,
				  struct dtlist_data  *plDtlist,
				  unsigned char       *pucRetbuf,
				  unsigned long       *pulRetlength)
{
	char                *pcDFSpath;
	int                 iRet;
	int                 iAcl_length;
	unsigned short      ushMode;
	int                 DFS_Errno;
	char                *Volume_path;
	char                *pWork;
	int                 i;
	unsigned char       *wkACLbufp;
	unsigned char       *ACLbufp;
	int					rtn;

	iAcl_length = 0;



	pWork = pcNetpath;


	while((*pWork != '\/') && (*pWork != '\\') && (*pWork != '\0'))
	{
		pWork++;
	}


	if((*pWork == '\0') || ((*(++pWork)) == '\0'))
	{
		return  -2;
	}

	Volume_path = GW_malloc(_MAX_PATH+1);
	if( Volume_path == 0 )
	{
		fnDtlist_err( pTable , pulRetlength , GWMEMERR );
		return  -1;
	}

	for( i=0 ; (( *(pWork+i) != '\0') && (i < _MAX_PATH) ) ; i++ )
	{
		*(Volume_path+i) = *(pWork+i);
	}
	*(Volume_path+i) = '\0';

	pcDFSpath = GW_malloc(DFSPATH_MAX+1);
	if( pcDFSpath == 0 )
	{
		GW_free(Volume_path,0);
		fnDtlist_err( pTable , pulRetlength , GWMEMERR );
		return  -1;
	}

	iRet = GetDFSPath(Volume_path,pcDFSpath,CHECKMODE);

	GW_free(Volume_path,0);

	if( iRet != GWERR_NORMAL )
	{
		GW_free(pcDFSpath,0);

		switch( iRet )
		{
			case GWERR_NOT_DFAMVOL:
									fnDtlist_err( pTable , pulRetlength , NOVOL );
									return  -1;
			case GWERR_NOMEM:
									fnDtlist_err( pTable , pulRetlength , GWMEMERR );
									return  -1;
			case GWERR_LONG:
									fnDtlist_err( pTable , pulRetlength , TOOLONG );
									return  -1;
			case GWERR_INVAL:
			case GWERR_NUSE:
			case GWERR_NW_NOENT:
									return  -2;
			default:
									fnDtlist_err( pTable , pulRetlength , ABNORMAL );
									return  -1;
		}
	}

	wkACLbufp = GW_malloc(2048);

	if (wkACLbufp == NULL)
	{
		GW_free(pcDFSpath,0);
		fnDtlist_err( pTable , pulRetlength , GWMEMERR );
		return  -1;
	}
	iAcl_length = 2048;

	switch(plDtlist->ug_class)
	{
		case DEFAULT_USER:
					ushMode = DS_USER;
					break;
		case DEFAULT_GROUP:
					ushMode = DS_GROUP;
					break;
		case DEFAULT_BOTH:
					ushMode = DS_USER | DS_GROUP;
					break;
		default:
					GW_free(pcDFSpath,0);
					GW_free(wkACLbufp,0);
					fnDtlist_err( pTable , pulRetlength , ERROPT );
					return  -1;
	}




	iRet = DFSgetacl_syscallReq( pcDFSpath , ushMode , wkACLbufp , &iAcl_length , &DFS_Errno , plUtbl );

	while ( iRet == RETRY )
	{
		GW_free( wkACLbufp , 0 );

		wkACLbufp = GW_malloc( iAcl_length );
		if (wkACLbufp == NULL)
		{
			GW_free( pcDFSpath , 0 );
			fnDtlist_err( pTable , pulRetlength , GWMEMERR );
			return  -1;
		}




		iRet = DFSgetacl_syscallReq(pcDFSpath,ushMode,wkACLbufp,&iAcl_length,&DFS_Errno,plUtbl);
	}

    if ( iRet != SUCCESS )
	{

		GW_free( pcDFSpath , 0 );
		GW_free( wkACLbufp , 0 );

		if( fnErrChange(DFS_Errno,plUtbl) == NOENT )
		{
			return  -2;
		}
		else if( fnErrChange(DFS_Errno,plUtbl) == PATHERR )
		{
			return  -3;
		}
		else
		{
			fnDtlist_err( pTable , pulRetlength , fnErrChange(DFS_Errno,plUtbl) );
			return  -1;
		}
	}

	pTable->pcAclbuf = GW_malloc( iAcl_length );

	if ( (pTable->pcAclbuf) == NULL )
	{
		GW_free(wkACLbufp,0);
		GW_free(pcDFSpath,0);
		fnDtlist_err( pTable , pulRetlength , GWMEMERR );
		return  -1;
	}

	ACLbufp = pTable->pcAclbuf;
	ACLbufp += 4;




	*( (unsigned int *)(pTable->pcAclbuf) ) = 0;
	switch(plDtlist->ug_class)
	{
		case DEFAULT_USER:
					rtn = fdDTlist_sort( pTable , pulRetlength , &ACLbufp , wkACLbufp , DS_USER );
					break;
		case DEFAULT_GROUP:
					rtn = fdDTlist_sort( pTable , pulRetlength , &ACLbufp , wkACLbufp , DS_GROUP );
					break;
		case DEFAULT_BOTH:
		default:
					rtn = fdDTlist_sort( pTable , pulRetlength , &ACLbufp , wkACLbufp , DS_USER );
					if( rtn != 0 )
					{
						break;
					}
					rtn = fdDTlist_sort( pTable , pulRetlength , &ACLbufp , wkACLbufp , DS_GROUP );
					break;
	}

	GW_free( wkACLbufp , 0 );

	if( rtn == 0 )
	{



		rtn = fnTrustee_op( plUtbl , pTable , pcDFSpath , pcNetpath , pucRetbuf , pulRetlength );
		if( rtn == 0 )
		{
			GW_free( pcDFSpath , 0 );
			GW_free( pTable->pcAclbuf , 0 );
			pTable->pcAclbuf = 0;

			return  0;
		}
		if( rtn == -3 )
		{
			GW_free( pcDFSpath , 0 );
			GW_free( pTable->pcAclbuf , 0 );
			pTable->pcAclbuf = 0;

			return  -3;
		}
	}
	GW_free( pcDFSpath , 0 );
	GW_free( pTable->pcAclbuf , 0 );
	pTable->pcAclbuf = 0;

	return  (-1);
}


/*EnglishComment**************************************************************/
/* ID   = fnTrustee_op                                                       */
/*                                                                           */
/* name = Setting the results for the trustee information setting            */
/* func = Sets the result information to the send buffer to be sent to       */
/*        Client, and sends a "trustee setting request" to Agent.            */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pcDFSpath   :char *            :inp: DFS path name                 */
/*        pcNetpath   :char *            :inp: NetWare path                  */
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

int  fnTrustee_op(struct GTGroup_t   *plUtbl,
				  struct TableKeep   *pTable,
				  char               *pcDFSpath,
				  char               *pcNetpath,
				  unsigned char      *pucRetbuf,
				  unsigned long      *pulRetlength)
{
	char                             *pcHeaderLastData;
	unsigned char                    *pGetTrustee;
	unsigned char                    *indexp;
	struct RetTlistDataKeep          **icountp;
	unsigned int                     count;
	unsigned char                    *charp;
	struct RetTlistData              *data_stp;
	int								 rtn;


	pGetTrustee      = (unsigned char *)((pTable->pcAclbuf)+(sizeof(unsigned int)));
	pcHeaderLastData = pucRetbuf+(*pulRetlength)-sizeof(struct RetTlistData);
	count            = *(( unsigned int * ) pTable->pcAclbuf );

	data_stp = GW_malloc( sizeof(struct RetTlistData) );
	if( data_stp == 0 )
	{
		fnDtlist_err( pTable , pulRetlength , GWMEMERR );
		return  (-1);
	}

	while( count != 0 )
	{
		memset( data_stp , 0x00 , sizeof(struct RetTlistData) );




		rtn = fnDTlist_main( plUtbl , pTable , pcDFSpath , data_stp , pGetTrustee , pcNetpath );

		(pTable->plRet_st)->DFAM_errno = rtn;

		if( rtn != NORMAL )
		{
			GW_free( data_stp , 0 );
			if( rtn == -3 )
			{
				return (-3);
			}
			else
			{
				(pTable->plRet_st)->ret_value  = FALSE;
				(pTable->plRet_st)->next_frame = FALSE;
				*pulRetlength = sizeof( struct RetHeader );
			}
			return  (-1);
		}

		if((((char *)(pTable->plRet_tdata)) <= pcHeaderLastData))
		{
			(pTable->plRet_st)->next_frame = FALSE;
			data_stp->next_data = FALSE;

			*(pTable->plRet_tdata) = *data_stp;

			indexp = pucRetbuf + ( sizeof( struct RetHeader) );

			while((struct RetTlistData *)indexp != pTable->plRet_tdata)
			{
				((struct RetTlistData *)indexp)->next_data = TRUE;
				indexp += sizeof( struct RetTlistData );
			}

			pTable->plRet_tdata++;

		}
		else
		{
			(pTable->plRet_st)->next_frame = TRUE;
			pTable->ppTDataKeep = &(pTable->pData);

			if( *(pTable->ppTDataKeep) == NULL)
			{

				*(pTable->ppTDataKeep) = GW_malloc(sizeof(struct RetTlistDataKeep));
				if (*(pTable->ppTDataKeep) == NULL)
				{
					fnDtlist_err( pTable , pulRetlength , GWMEMERR );

					GW_free( data_stp , 0 );

					return  (-1);
				}
				(*(pTable->ppTDataKeep))->data = *data_stp;
				(*(pTable->ppTDataKeep))->pNext = NULL;
			}
			else
			{

				icountp = &(pTable->pData);
				while (*icountp != NULL)
				{
					icountp = &((*icountp)->pNext);
				}

				*icountp = GW_malloc(sizeof(struct RetTlistDataKeep));
				if (*icountp == NULL)
				{
					fnDtlist_err( pTable , pulRetlength , GWMEMERR );

					GW_free( data_stp , 0 );

					return  -1;
				}
				(*icountp)->data = *data_stp;
				(*icountp)->pNext = NULL;
			}
		}

		count--;
		charp =  pGetTrustee;
		pGetTrustee = charp + sizeof(unsigned int) + sizeof(short) + sizeof(unsigned int) +
					((struct GetTrustee *)pGetTrustee)->ug_length;

	    *pulRetlength = ( (char *)(pTable->plRet_tdata))-pucRetbuf;
	}

	GW_free( data_stp , 0 );

	return  0;
}


/*EnglishComment**************************************************************/
/* ID   = fnDTlist_main                                                      */
/*                                                                           */
/* name = Getting trustee (main routine)                                     */
/* func = (1) Mapping trustee                                                */
/*        (2) User name conversion                                           */
/*                                                                           */
/* i/o  = plUtbl      :struct GTGroup_t *:inp: pointer to thread table       */
/*        pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pcDFSpath   :char *            :inp: DFS path                      */
/*        data_stp    :struct RetGrantData *:out: structure for Gateway's    */
/*                                                send data                  */
/*        pGetTrustee :unsigned char *   :inp: structure to get trustee      */
/*        pcNetpath   :char *            :inp: NetWare path                  */
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

int fnDTlist_main( 	struct GTGroup_t   		*plUtbl,
					struct TableKeep   		*pTable,
					char               		*pcDFSpath,
					struct RetTlistData 	*data_stp ,
					unsigned char 			*pGetTrustee ,
				  	char               		*pcNetpath )
{
	char							aszNWUserName[UNAMELEN+1];
	struct RightsMapping            RightsMap;
	struct RightsMapping            *NW_kenrimap;
	int                             iRet;
	int                             DFS_Errno;
	struct dstat                    *pBuffer;
	unsigned int                    uiKyokabit;
	int								wklen;
	char                            *w_namep;
	int								mask_sw;

	NW_kenrimap = &RightsMap;
	uiKyokabit  = ((struct GetTrustee *)pGetTrustee)->kyokabit;
	strcpy( data_stp->path_name , pcNetpath );

	pBuffer = GW_malloc(sizeof(struct dstat));
	if (pBuffer == NULL)
	{
		return	GWMEMERR;
	}


	iRet = DFSstatReq(pcDFSpath,pBuffer,&DFS_Errno,plUtbl);
	if (iRet == LIB_ERR)
	{
		GW_free(pBuffer,0);
		return  fnErrChange(DFS_Errno,plUtbl);
	}

	if (pBuffer->d_mode & DS_IFDIR)
	{
		NW_kenrimap->mode = ACL_DIR;
	}
	else
	{
		NW_kenrimap->mode = ACL_FILE;
	}

	GW_free(pBuffer,0);

	NW_kenrimap->ACL_list = uiKyokabit;




	iRet = GW_Rights_Map(NW_kenrimap);

	if (iRet != SUCCESS)
	{
		return  MODEERR;
	}



	iRet = fnKenriMap( NW_kenrimap->trustee_list , data_stp->t_info );

	(pTable->plRet_st)->DFAM_errno = NORMAL;
	(pTable->plRet_st)->ret_value  = TRUE;


	data_stp->ug_info = ((struct GetTrustee *)pGetTrustee)->entrymode;

	if(((struct GetTrustee *)pGetTrustee)->ug_length == -1 )
	{
	    ((struct GetTrustee *)pGetTrustee)->ug_length = 0;
		iRet = -1;
	}
	else
	{
		w_namep = GW_malloc(((struct GetTrustee *)pGetTrustee)->ug_length+1);
		if ( w_namep == NULL)
		{
			return  GWMEMERR;
		}
		memcpy( w_namep , ( (struct GetTrustee *)pGetTrustee)->ug_name ,
						  ( (struct GetTrustee *)pGetTrustee)->ug_length );
		*(w_namep+(((struct GetTrustee *)pGetTrustee)->ug_length)) = '\0';

		if( ((struct GetTrustee *)pGetTrustee)->entrymode < DTHRESHLD )
		{
			mask_sw = DS_USER;
		}
		else
		{
			mask_sw = DS_GROUP;
		}

		if( mask_sw == DS_USER)
		{



			iRet = GetDFAMUserName(w_namep,aszNWUserName);
		}
		else
		{



			iRet = GW_DFAMGroup_Map(w_namep,aszNWUserName);
		}

		GW_free( w_namep , 0 );
	}
	switch ( iRet )
	{
		case  0:
				break;
		case -1:
				aszNWUserName[0] = '\0';
				memset( data_stp->t_info , 0x00 , TLENGTH );
				break;
		case -2:
				return  GWMEMERR;
		case -3:
				return  BINDERR;
		default:
				return  FAILECHG;
	}


	wklen = strlen( aszNWUserName);
	memcpy( data_stp->user_name , aszNWUserName , wklen );
	data_stp->user_name[wklen] = '\0';

	return  NORMAL;
}


/*EnglishComment**************************************************************/
/* ID   = fnDTlist_sort                                                      */
/*                                                                           */
/* name = Sorting the user group names                                       */
/* func = Sorts the user group names.                                        */
/*                                                                           */
/* i/o  = pTable      :struct TableKeep *:inp: pointer to task table         */
/*        pulRetlength:unsigned long *   :inp: size of Gateway send buffer   */
/*        ACLbufp     :unsigned char **  :out: address to copy ACL inf.      */
/*        wkACLbufp   :unsigned char *   :inp: reference information         */
/*        ug_info     :short             :inp: user group information        */
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

int fdDTlist_sort( struct TableKeep 	*pTable ,
					unsigned long 		*pulRetlength ,
					unsigned char       **ACLbufp ,
					unsigned char 		*wkACLbufp ,
					short 				ug_info )
{
	int							num;
	int							sw;
	int							i;
	int							index;
	char						*namep;
	char						*wkp;
	char                		aszNWUserName[UNAMELEN+1];
	int 						iRet;
	int                 		count;
	int							mask_sw;

	num = 0;
	sw 	= 0;
	count = *((unsigned int *)wkACLbufp);
	wkp = wkACLbufp + 4;
	for( i=0 ; i < count ; i++ )
	{
		if( ( (struct GetTrustee *)wkp)->ug_length == -1)
		{
			index = (sizeof(unsigned int)) + (sizeof(short)) + (sizeof(unsigned int));
		}
		else
		{
			if( ((struct GetTrustee *)wkp)->entrymode < DTHRESHLD )
			{
				mask_sw = DS_USER;
			}
			else
			{
				mask_sw = DS_GROUP;
			}
			index = (sizeof(unsigned int)) + (sizeof(short)) + (sizeof(unsigned int))
					+ ( (struct GetTrustee *)wkp)->ug_length;

			if( mask_sw == ug_info )
			{
				namep = GW_malloc( ( (struct GetTrustee *)wkp)->ug_length+1 );

				if ( namep == NULL )
				{
					fnDtlist_err( pTable , pulRetlength , GWMEMERR );
					return  -1;
				}
				memcpy( namep, ( (struct GetTrustee *)wkp)->ug_name ,
							   ( (struct GetTrustee *)wkp)->ug_length );
				*( namep + ( ( (struct GetTrustee *)wkp)->ug_length ) ) = '\0';

				if( ug_info == DS_USER )
				{



					iRet = GetDFAMUserName( namep , aszNWUserName );
				}
				else
				{



					iRet = GW_DFAMGroup_Map( namep , aszNWUserName );
				}
				GW_free( namep , 0 );

				if( iRet != -1 )
				{
					sw = 1;
					num++;
					memcpy( *ACLbufp , wkp , index );
					(*ACLbufp) += index;
				}
			}
		}
		wkp = wkp + index;
	}

	wkp = wkACLbufp + 4;

	for( i=0 ; i < count ; i++ )
	{
		if( ((struct GetTrustee *)wkp)->entrymode < DTHRESHLD )
		{
			mask_sw = DS_USER;
		}
		else
		{
			mask_sw = DS_GROUP;
		}

		if( ( (struct GetTrustee *)wkp)->ug_length == -1)
		{
			index = (sizeof(unsigned int)) + (sizeof(short)) + (sizeof(unsigned int));

			if( mask_sw == ug_info )
			{
				if( sw == 0 )
				{
					sw = 1;
					num++;
					memcpy( *ACLbufp , wkp , index );
					(*ACLbufp) += index;
				}
			}
		}
		else
		{
			index = (sizeof(unsigned int)) + (sizeof(short)) + (sizeof(unsigned int))
										   + ( (struct GetTrustee *)wkp)->ug_length;
			if( mask_sw == ug_info )
			{
				namep = GW_malloc( ( (struct GetTrustee *)wkp)->ug_length+1 );

				if ( namep == NULL)
				{
					fnDtlist_err( pTable , pulRetlength , GWMEMERR );
					return  -1;
				}
				memcpy( namep, ( (struct GetTrustee *)wkp)->ug_name ,
							   ( (struct GetTrustee *)wkp)->ug_length );
				*( namep + ( ( (struct GetTrustee *)wkp)->ug_length ) ) = '\0';

				if( ug_info == DS_USER )
				{



					iRet = GetDFAMUserName( namep , aszNWUserName );
				}
				else
				{



					iRet = GW_DFAMGroup_Map( namep , aszNWUserName );
				}

				GW_free( namep , 0 );

				if(iRet == -1)
				{
					if( sw == 0 )
					{
						sw = 1;
						num++;
						memcpy( *ACLbufp , wkp , index );
						(*ACLbufp) += index;
					}
				}
			}
		}
		wkp = wkp + index;
	}

	*( (unsigned int *)(pTable->pcAclbuf) ) += num;

	return  0;
}


/*EnglishComment**************************************************************/
/* ID   = fnDtlist_err                                                       */
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

void fnDtlist_err( struct TableKeep *pTable , unsigned long *pulRetlength , int   Err_no )
{
	(pTable->plRet_st)->DFAM_errno = Err_no;
	(pTable->plRet_st)->ret_value  = FALSE;
	(pTable->plRet_st)->next_frame = FALSE;
	*pulRetlength = sizeof( struct RetHeader );
}
