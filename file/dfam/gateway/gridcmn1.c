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
 * $Log: gridcmn1.c,v $
 * Revision 1.1.2.3  1996/03/22  07:37:08  takano
 * 	modified the table deletion method in the POST routine.
 * 	[1996/03/22  07:33:34  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:08:42  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:55  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  08:08:29  takano
 * 	First Submit
 * 	[1996/02/21  08:08:24  takano]
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
#include <nwbindry.h>
#include <nwconn.h>
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dceapi.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "cmnprt.h"

/*EnglishComment**************************************************************/
/* ID	= GetGTGroupTable                                                    */
/* name = Getting the address of the user thread group table.                */
/* func = Gets the address of GTGroup_t with using a connection number       */
/*        as a key.                                                          */
/*                                                                           */
/* i/o  = ConnNum :LONG	       :inp: connection number                       */
/* return = NULL  : No user thread group table                               */
/*          others: User thread group table exists (address of user          */
/*                  thread group table)                                      */
/* common = none                                                             */
/* note   = non blocking                                                     */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

struct GTGroup_t *GetGTGroupTable(LONG uiConnNum)
{
	struct GTGroup_t	*pTGroup_wp;

	pTGroup_wp = pTGroup_sp;
	while( pTGroup_wp != NULL) {
		if(pTGroup_wp->uiConnNum == uiConnNum) {
			break;
		}
		pTGroup_wp = pTGroup_wp->pNext;
	}

	return pTGroup_wp;
}

/*EnglishComment**************************************************************/
/* ID	= SearchOpenFile_t_cb                                                */
/* name = Searching the open file table (for Open-hook)                      */
/* func = Searches the open file table with using callback routine           */
/*        parameters as a key.                                               */
/*                                                                           */
/* i/o	= pCur_TG:GTGroup_t *:inp: the address of user thread group table    */
/*        pofcbs:xOpenFileCallBackStruct *:inp: open callback                */
/*                                                                           */
/* return = NULL  : No open file table                                       */
/*          others: Open file table exists (address of open file table)      */
/* common = none                                                             */
/* note   = none                                                             */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

struct 	OpenFile_t 	*SearchOpenFile_t_cb(struct GTGroup_t	*pCur_TG,
										 xOpenFileCallBackStruct *pofcbs)
{
	struct OpenFile_t *pOF_w;
	int iThreadID;

	if (pCur_TG == (struct GTGroup_t *) NULL)
		return((struct OpenFile_t *) NULL);

	if (pofcbs == (xOpenFileCallBackStruct *) NULL)
		return((struct OpenFile_t *) NULL);

	iThreadID = GetThreadID();

	pOF_w = pCur_TG->pOFile_sp;

	while ( pOF_w != NULL) {
		if (pOF_w->uiConnum == pofcbs->connection	&&
			pOF_w->uiTasknum == pofcbs->task		&&
			pOF_w->iThreadID == iThreadID           &&
			pOF_w->uiNWHandle == 0 ) {
				return	(pOF_w);
		}
		pOF_w = (struct OpenFile_t *) pOF_w->pNextOF_t;
	}
	return	((struct OpenFile_t *) NULL);
}

/*EnglishComment**************************************************************/
/* ID	= SearchOpenFile_t_cbCO                                              */
/* name = Searching the open file table (for Create-hook)                    */
/* func = Searches the open file table with using callback routine           */
/*        parameters as a key.                                               */
/*                                                                           */
/* i/o	= pCur_TG:GTGroup_t *:inp: the address of user thread group table    */
/*        pofcbs:xCreateAndOpenCallBackStruct *:inp: open callback           */
/*                                                                           */
/* return = NULL  : No open file table                                       */
/*          others: Open file table exists (address of open file table)      */
/* common = none                                                             */
/* note   = none                                                             */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

struct 	OpenFile_t 	*SearchOpenFile_t_cbCO(struct GTGroup_t	*pCur_TG,
										 xCreateAndOpenCallBackStruct *pofcbs)
{
	struct OpenFile_t *pOF_w;
	int iThreadID;

	if (pCur_TG == (struct GTGroup_t *) NULL)
		return((struct OpenFile_t *) NULL);

	if (pofcbs == (xCreateAndOpenCallBackStruct *) NULL)
		return((struct OpenFile_t *) NULL);

	iThreadID = GetThreadID();

	pOF_w = pCur_TG->pOFile_sp;

	while ( pOF_w != NULL ) {
		if (pOF_w->uiConnum == pofcbs->connection	&&
			pOF_w->uiTasknum == pofcbs->task		&&
			pOF_w->iThreadID == iThreadID			&&
			pOF_w->uiNWHandle == 0 ) {
				return	(pOF_w);
		}
		pOF_w = (struct OpenFile_t *) pOF_w->pNextOF_t;
	}
	return	((struct OpenFile_t *) NULL);
}
/*EnglishComment**************************************************************/
/* ID   = SearchrequestedAccessRights                                        */
/* name = Searching the open file table to get Requested Access Right of     */
/*        the same named file                                                */
/* func = Searches the open file table with using a path name as a key,      */
/*        gets Requested Access Right, and sets mask to the Right.           */
/*                                                                           */
/* i/o	= pCur_TG:GTGroup_t * :inp: the address of user thread group table   */
/*        pszNWpath:BYTE *    :inp: NetWare full path name                   */
/*        iRecAcsRgt_dmy:int *:inp: area for Requested Access Right          */
/*                                                                           */
/* return = 1: Opened same-named file exists (iRecAcsRgt_dmy is effective)   */
/*          0: No opened same-named file (iRecAcsRgt_dmy is not effective)   */
/* common = none                                                             */
/* note   = none                                                             */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

short	SearchrequestedAccessRights(struct GTGroup_t *pCur_TG,
									char *pszNWpath,
									int	*iRecAcsRgt_dmy)
{
	short iFound_flg = 0;
	struct OpenFile_t *pOF_w;

	if (pCur_TG == (struct GTGroup_t *) NULL)
		return(iFound_flg);

	pOF_w = pCur_TG->pOFile_sp;

	while ( pOF_w != NULL ) {
		if (!strcmp(pOF_w->aszNWFullpath, pszNWpath)) {
				*iRecAcsRgt_dmy |= pOF_w->uiRealAcsRights;
				iFound_flg = 1;
		}
		pOF_w = (struct OpenFile_t *) pOF_w->pNextOF_t;
	}
	return	(iFound_flg);
}

/*EnglishComment**************************************************************/
/* ID   = SearchOpenFile_t_fd                                                */
/* name = Searching the open file table                                      */
/* func = Searches the open file table with using a NetWare file handle      */
/*        as a key.                                                          */
/*                                                                           */
/* i/o	= pCur_TG:GTGroup_t * :inp: the address of user thread group table   */
/*        iNW_fd:LONG         :inp: NetWare file handle                      */
/*                                                                           */
/* return = NULL  : No open file table                                       */
/*          others: Open file table exists (address of open file table)      */
/*                                                                           */
/* common = none                                                             */
/* note   = none                                                             */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
struct 	OpenFile_t 	*SearchOpenFile_t_fd(struct GTGroup_t	*pCur_TG,
									 	 LONG		iNW_fd)
{
	struct OpenFile_t *pOF_w;

	if (pCur_TG == (struct GTGroup_t *) NULL)
		return((struct OpenFile_t *) NULL);

	pOF_w = pCur_TG->pOFile_sp;

	while ( pOF_w != NULL ) {
		if (pOF_w->uiNWHandle == iNW_fd) {
			if (ExamineLocalSemaphore(pOF_w->uiClose_sh) == -1)
				return(pOF_w);
		}
		pOF_w = (struct OpenFile_t *) pOF_w->pNextOF_t;
	}
	return(0);
}

/*EnglishComment**************************************************************/
/* ID   = DelOpenFile_t                                                      */
/* name = Deleting the open file table                                       */
/* func = Deletes the open file table.                                       */
/*                                                                           */
/* i/o  = pOF:struct OpenFile_t *    :inp: the address of open file table    */
/*        pCur_TG: struct GTGroup_t *:inp: the address of the current user   */
/*                                         thread group table                */
/*                                                                           */
/* return = -1: the target open file table does not exist.                   */
/*           0: successfully deleted the target open file table.             */
/*                                                                           */
/* common = none                                                             */
/* note   = none                                                             */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

int	DelOpenFile_t(struct OpenFile_t *pOF,struct GTGroup_t *pCur_TG)
{
	struct OpenFile_t *pOF_wp;
	struct OpenFile_t *pOF_pwp;

	if (pOF == (struct OpenFile_t *) NULL)
		return(-1);

	if (pCur_TG == (struct GTGroup_t *) NULL)
		return(-1);

	pOF_pwp = pOF_wp = pCur_TG->pOFile_sp;
	while (pOF_wp != NULL) {
		if (pOF == pOF_wp) {
			break;
		}
		pOF_pwp = pOF_wp;
		pOF_wp = (struct OpenFile_t *) pOF_wp->pNextOF_t;
	}
	if (pOF_wp == NULL) {
		return (-1);
	} else {
		if (pOF_wp == pCur_TG->pOFile_sp) {
			pCur_TG->pOFile_sp = (struct OpenFile_t *) pOF_wp->pNextOF_t;
		} else {
			pOF_pwp->pNextOF_t = pOF_wp->pNextOF_t;
		}
		if (pOF_wp->uiClose_sh > 0)
			CloseLocalSemaphore(pOF_wp->uiClose_sh);
		GW_free(pOF_wp,NULL);
	}
	return (0);
}
/*EnglishComment**************************************************************/
/* ID   = SearchCopyBack_t                                                   */
/* name = Searching the target file for copyback (opening)                   */
/* func = Searches the copyback table with using a path name as a key.       */
/*        Returns the table address if the target file is registered.        */
/*                                                                           */
/* i/o  = pszNWFullpath	:BYTE *	:out: NetWare full path name                 */
/*                                                                           */
/* return = NULL  : No copyback table                                        */
/*          others: Copyback table exists (address of copyback table)        */
/*                                                                           */
/* common = none                                                             */
/* note   = none                                                             */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
struct GCopyBack_t *SearchCopyBack_t(BYTE *pszNWFullpath)
{
	struct GCopyBack_t *pCopyBack_wp;

	pCopyBack_wp = NULL;

	if(pCopyBack_sp != NULL) {
		pCopyBack_wp = pCopyBack_sp;
		do {
			if(!strcmp(pCopyBack_wp->aszNWFullpath, pszNWFullpath)) {
				return(pCopyBack_wp);
			}
			pCopyBack_wp = (struct GCopyBack_t *) pCopyBack_wp->pNext_t;
		} while(pCopyBack_wp != (struct GCopyBack_t *)NULL);
	}
	return(pCopyBack_wp);
}

/*EnglishComment**************************************************************/
/* ID   = AddCopyBack_t                                                      */
/* name = Making/adding the copyback table                                   */
/* func = Makes a copyback table, and adds the table to the end of the table */
/*        chain.  If the table already exists, adds the read/write counter   */
/*        by 1.                                                              */
/*                                                                           */
/* i/o  = pszNWFullpath    :BYTE *:inp: NetWare full path name               */
/*        uiReqdAcsRights:LONG  :inp: parameters for requestedAccessRights   */
/*                                                                           */
/* return = 0: successful                                                    */
/*          1: failed                                                        */
/*                                                                           */
/* common = none                                                             */
/* note   = none                                                             */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int AddCopyBack_t(BYTE *pszNWFullpath, LONG uiReqdAcsRights)
{
	struct GCopyBack_t	*pCopyBack_wp;
	struct GCopyBack_t	*pCopyBack_pwp;

	pCopyBack_pwp = pCopyBack_wp = pCopyBack_sp;
	while( pCopyBack_wp != NULL ) {
		if( !strcmp(pCopyBack_wp->aszNWFullpath, pszNWFullpath)) {
			if( (uiReqdAcsRights & O_WRITE_MODE) ) {
				pCopyBack_wp->iWrite_cnt++;
			} else {
				pCopyBack_wp->iRead_cnt++;
			}
			return (0);
		}
		pCopyBack_pwp = pCopyBack_wp;
		pCopyBack_wp = (struct GCopyBack_t *) pCopyBack_pwp->pNext_t;
	}

	pCopyBack_wp = (struct GCopyBack_t *)GW_malloc(sizeof(struct GCopyBack_t));
	if(pCopyBack_wp == NULL) {
		return (-1);
	}
	strcpy(pCopyBack_wp->aszNWFullpath, pszNWFullpath);

	if( (uiReqdAcsRights & O_WRITE_MODE) ) {
		pCopyBack_wp->iWrite_cnt = 1;
		pCopyBack_wp->iRead_cnt = 0;
	} else {
		pCopyBack_wp->iWrite_cnt = 0;
		pCopyBack_wp->iRead_cnt = 1;
	}
	pCopyBack_wp->iCopyBack_flg = OFF;
	pCopyBack_wp->iOpenreq_flg = OFF;
	pCopyBack_wp->iRenamereq_flg = OFF;
	pCopyBack_wp->iErasereq_flg = OFF;
	pCopyBack_wp->iDelay_flg = OFF;
	pCopyBack_wp->pNext_t = NULL;

	if (pCopyBack_sp == NULL) {
		pCopyBack_sp = pCopyBack_wp;
	} else {
		pCopyBack_pwp->pNext_t = pCopyBack_wp;
	}

	return (0);
}

/*EnglishComment**************************************************************/
/* ID   = DelCopyBack_t                                                      */
/* name = Deleting the copyback table                                        */
/* func = Deletes the copyback table.                                        */
/*                                                                           */
/* i/o  = pszNWfullpath :char *:inp: NetWare full path name                  */
/*                                                                           */
/* return =  0: copyback table successfully deleted                          */
/*          -1: failed to delete the copyback table                          */
/*                                                                           */
/* common = none                                                             */
/* note   = none                                                             */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int DelCopyBack_t(BYTE *pszNWFullpath)
{
	struct GCopyBack_t *pCopyBack_wp;
	struct GCopyBack_t *pCopyBack_pwp;

	pCopyBack_wp = pCopyBack_pwp = pCopyBack_sp;

	if (pCopyBack_sp != NULL) {
		pCopyBack_wp = pCopyBack_sp;
		do {
			if (!strcmp(pCopyBack_wp->aszNWFullpath, pszNWFullpath)) {
				break;
			}
			pCopyBack_pwp = pCopyBack_wp;
			pCopyBack_wp = pCopyBack_wp->pNext_t;
		} while(pCopyBack_wp != NULL);
	}
	if (pCopyBack_wp == NULL){
		return(-1);
	}
	if( (pCopyBack_wp->iWrite_cnt == 0) && (pCopyBack_wp->iRead_cnt == 0) ){
		if(pCopyBack_wp == pCopyBack_sp){
			pCopyBack_sp = pCopyBack_wp->pNext_t;
		} else {
			pCopyBack_pwp->pNext_t = pCopyBack_wp->pNext_t;
		}
		GW_free(pCopyBack_wp,NULL);
	} else {
		return(-2);
	}

	return(0);
}

/*EnglishComment**************************************************************/
/* ID   = DecCopyBackCounter                                                 */
/* name = Counter value decrement                                            */
/* func = Decrements the read/write counter values.                          */
/*                                                                           */
/* i/o  = pszNWFullpath:BYTE *:inp: NetWare full path name                   */
/*        uiReqdAcsRights   :LONG  :inp: parameters for requestedAccessRights*/
/*                                                                           */
/* return = -1: the target copyback file does not exist.                     */
/*           0: the counter is not 0 after the counter was decremented.      */
/*           1: the write counter becomes 0 after the decrement.             */
/*           2: the read counter becomes 0 after the decrement, and the      */
/*              write counter is also 0.                                     */
/*           3: the write counter becomes 0 after the decrement, and the     */
/*              read counter is also 0.                                      */
/*                                                                           */
/* common = none                                                             */
/* note   = none                                                             */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
int DecCopyBackCounter(BYTE *pszNWFullpath, LONG uiReqdAcsRights)
{
	struct GCopyBack_t	*pCopyBack_wp;
	struct GCopyBack_t	*pCopyBack_pwp;

	pCopyBack_pwp = pCopyBack_wp = pCopyBack_sp;
	while (pCopyBack_wp != NULL) {
		if( !strcmp(pCopyBack_wp->aszNWFullpath, pszNWFullpath)) {
			if( (uiReqdAcsRights & O_WRITE_MODE) ) {
				pCopyBack_wp->iWrite_cnt--;
				if (pCopyBack_wp->iWrite_cnt == 0 ) {
					if ( pCopyBack_wp->iRead_cnt == 0 ) {
						return (3);
					}
					return (1);
				}
				else
					return(2);
			} else {
				pCopyBack_wp->iRead_cnt--;
				if ( pCopyBack_wp->iRead_cnt == 0 ) {
					if ( pCopyBack_wp->iWrite_cnt == 0 ) {
						return (2);
					}
			 	}
			}
			return (2);
		}
		pCopyBack_pwp = pCopyBack_wp;
		pCopyBack_wp = pCopyBack_pwp->pNext_t;
	}

	return (-1);
}

/*EnglishComment**************************************************************/
/* ID	= AddAccessList					                                     */
/*                                                                           */
/* name = Appending to the file access list                                  */
/* func = Adds an entry of a file/directory list to the access list.         */
/*                                                                           */
/* i/o	= pAcs_t :FileAccessList *:inp: the address of the file access list  */
/*                                                                           */
/* common = none                                                             */
/* note   = none                                                             */
/* date = September 27, 1995  by Yoshiaki Okada                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
void	AddAccessList(struct	FileAccessList	*pAcs_t)
{
	if (pAcs_t == (struct	FileAccessList *) NULL)
		return;

	if (pAcsList_sp != (struct	FileAccessList *) NULL) {
		pAcs_t->pNext_t = pAcsList_sp;
		pAcsList_sp = pAcs_t;
	}
	else
		pAcsList_sp = pAcs_t;
	return;
}

/*EnglishComment**************************************************************/
/* ID   = DelAccessList                                                      */
/* name = Deleting the file access list                                      */
/* func = Deleting the file access list                                      */
/* i/o	= uiConNo    :LONG   :inp: connection number                         */
/*        pPathName_b:char * :inp: path name 	                             */
/*        pPathName_a:char * :inp: path name (used only for renaming)        */
/* return = 0     : normal end                                               */
/*          others: abnormal end (returns NetWareErrno)                      */
/*                                                                           */
/* common = none                                                             */
/* note   = blocking                                                         */
/* date   = Dec. 23, 1994 by Yoshiaki Okada                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
void	DelAccessList(LONG uiConNo,char *pPathName_b,char *pPathName_a)
{
	struct	FileAccessList *pAcs_w = NULL;
	struct	FileAccessList *pAcs_b = NULL;

	if ((pPathName_b == NULL) || (uiConNo == 0))
		return;

	pAcs_w = pAcsList_sp;

	while ( pAcs_w != NULL ) {
		if ((uiConNo == pAcs_w->uiConnNum) &&
			(strcmp(pPathName_b,pAcs_w->aszAccessFilePathName_b) == 0)) {
			if (pPathName_a != NULL) {
				if (strcmp(pPathName_a,pAcs_w->aszAccessFilePathName_a) != 0) {
					pAcs_b = pAcs_w;
					pAcs_w = (struct FileAccessList *) pAcs_w->pNext_t;
					continue;
				}
			}
			if (pAcs_w == pAcsList_sp)
				pAcsList_sp = pAcs_w->pNext_t;
			else
				pAcs_b->pNext_t = pAcs_w->pNext_t;
			CloseLocalSemaphore(pAcs_w->uiAcces_sh);
			GW_free(pAcs_w,NULL);
			return;
		}
		pAcs_b = pAcs_w;
		pAcs_w = (struct FileAccessList *) pAcs_w->pNext_t;
	}
	return;
}

/*EnglishComment**************************************************************/
/* ID	= DelAccessListConNoAll											     */
/*                                                                           */
/* name = Deleting a file access list                                        */
/* func = Deletes a file access list with using a connection number as a key.*/
/*                                                                           */
/* i/o	= uiConNo    :LONG   :inp: connection number                         */
/*                                                                           */
/* common = none                                                             */
/* note   = none                                                             */
/* date = September 27, 1995  by Yoshiaki Okada                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
void	DelAccessListConNoAll(LONG	uiConNo)
{
	struct	FileAccessList *pAcs_w = NULL;
	struct	FileAccessList *pAcs_b = NULL;

	if (uiConNo == 0)
		return;

	pAcs_w = pAcsList_sp;

	while ( pAcs_w != (struct FileAccessList *) NULL ) {
		if ((uiConNo == pAcs_w->uiConnNum)) {
			if (pAcs_w->uiAcces_sh > 0) {
				if (ExamineLocalSemaphore(pAcs_w->uiAcces_sh) < 0)
					SignalLocalSemaphore(pAcs_w->uiAcces_sh);
				else {
					if (pAcs_w == pAcsList_sp)
						pAcsList_sp = pAcs_w->pNext_t;
					else
						pAcs_b->pNext_t = pAcs_w->pNext_t;
					CloseLocalSemaphore(pAcs_w->uiAcces_sh);
					GW_free(pAcs_w,NULL);
				}
			}
			else {
				if (pAcs_w == pAcsList_sp)
					pAcsList_sp = pAcs_w->pNext_t;
				else
					pAcs_b->pNext_t = pAcs_w->pNext_t;
				GW_free(pAcs_w,NULL);
			}

			if (pAcs_b != (struct FileAccessList *) NULL)
				pAcs_w = (struct FileAccessList *) pAcs_b->pNext_t;
			else
				pAcs_w = pAcsList_sp;
			continue;
		}
		pAcs_b = pAcs_w;
		pAcs_w = (struct FileAccessList *) pAcs_w->pNext_t;
	}
	return;
}

/*EnglishComment**************************************************************/
/* ID	= SerchAccessList												     */
/*                                                                           */
/* name = Searching an access list                                           */
/* func = Searches an access list.                                           */
/*                                                                           */
/* i/o	= uiConNo    :LONG   :inp: connection number                         */
/*        uiTaskNum  :LONG   :inp: task number                               */
/*        iAccessType :int   :inp: access type                               */
/*                                                                           */
/* common = none                                                             */
/* note   = none                                                             */
/* date = September 27, 1995  by Yoshiaki Okada                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/
struct FileAccessList *SerchAccessList(LONG uiConNo, LONG uiTaskNum, int iAccessType)
{
	struct FileAccessList *pAcs_w = NULL;
	int iThreadID;

	if (uiConNo == 0)
		return((struct FileAccessList *) NULL);

	iThreadID = GetThreadID();

	pAcs_w = pAcsList_sp;
	while ( pAcs_w != NULL ) {
		if ((uiConNo == pAcs_w->uiConnNum) &&
			(pAcs_w->uiTaskNum == uiTaskNum) &&
			(pAcs_w->iAccessType == iAccessType) &&
			(pAcs_w->iThreadID == iThreadID) &&
			(pAcs_w->bIsCalledPostHook == OFF)) {

			return((struct FileAccessList *) pAcs_w);
		}
		pAcs_w = (struct FileAccessList *) pAcs_w->pNext_t;
	}
	return((struct FileAccessList *) NULL);
}
