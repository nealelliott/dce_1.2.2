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
 * $Log: gmaitbl.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:36  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:51  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:06:21  takano
 * 	First Submit
 * 	[1996/02/21  08:06:12  takano]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <nwsemaph.h>

#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfammsg.h"
#include "dfamprt.h"
#include "gmai.h"



/*EnglishComment**************************************************************
 *  ID      =   AddToGVolume_t                                               *
 *  name    =   Addition of new members to a Volume Table                    *
 *  func    =   Mallocs the structure Volume_t, and returns the allocated    *
 *              memory pointer.                                              *
 *  i/o     =   none                                                         *
 *  return  =   pointer to the allocated area :  normal end                  *
 *              NULL                          :  abnormal end (malloc failed)*
 *  common  =   none                                                         *
 *  note    =   none                                                         *
 *  date    =   Nov. 24, 1994  by Ichiyoshi                                  *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
struct GVolume_t * AddToGVolume_t()
{
	struct GVolume_t * pTblp;
	struct GVolume_t * pWorkp;


	GW_tsh_functrace( FUNC_ADDTOVOLUME, GW_FUNC_START, NULL);


	pTblp = (struct GVolume_t *)GW_malloc( sizeof(struct GVolume_t) );


	if (!pTblp) {
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11037 );
		goto EndOfFunc;
	} else;


	strcpy(pTblp->aszIcat, VOL_ICAT);
	pTblp -> pUshirop		=	NULL;
	pTblp -> pNamep			=	NULL;
	pTblp -> iVolNumber		=	0;
	pTblp -> aszNWVol[0]	=	'\0';
	pTblp -> aszDFSPath[0]	=	'\0';
	pTblp -> uiSynctime		= (LONG)0;
	pTblp -> usVolStatus	=	VOL_VIRGIN;


	if (!pVolume_sp) {
		pVolume_sp = pTblp;
		pTblp->pMaep = (struct GVolume_t *)0;
	} else {
		pWorkp = pVolume_sp;
		while ( (pWorkp->pUshirop) != (struct GVolume_t *)0) {
			pWorkp = pWorkp -> pUshirop;
		}
		(pWorkp -> pUshirop) = pTblp;
		(pTblp -> pMaep) = pWorkp;
	}


	(pTblp -> uiVolAndName_sh) = OpenLocalSemaphore( (LONG)1 );

EndOfFunc:
	GW_tsh_functrace( FUNC_ADDTOVOLUME, GW_FUNC_END, NULL);

	return(pTblp);

}




/*EnglishComment**************************************************************
 *  ID      =   DeleteFromGVolume_t                                          *
 *  name    =   VolumeTable member deletion                                  *
 *  Func    =   Deletes a single member of VolumeTable                       *
 *  i/o     =   pTblp   :   struct GVolume_t *  :i: a member to be deleted   *
 *  Return  =   none                                                         *
 *  common  =   none                                                         *
 *  note    =   none                                                         *
 *  date    =   Dec. 13, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
void DeleteFromGVolume_t(pTblp)
struct GVolume_t * pTblp;
{
	struct GVolume_t *pFront;
	struct GVolume_t *pBack;


	GW_tsh_functrace( FUNC_DELETEFROMVOLUME, GW_FUNC_START, NULL);

	if (pTblp->pUshirop != (struct GVolume_t *)0) {


		if (pTblp == pVolume_sp){
			pVolume_sp = pTblp->pUshirop;
			pVolume_sp->pMaep = (struct GVolume_t *)0;
		} else {
			pFront = pTblp -> pMaep;
			pBack = pTblp -> pUshirop;
			pFront->pUshirop = pBack;
			pBack->pMaep = pFront;
		}

	} else {

		if (pTblp == pVolume_sp){
			pVolume_sp = (struct GVolume_t *)0;
		} else {
			pFront = pTblp -> pMaep;
			pFront->pUshirop = (struct GVolume_t *)0;
		}
	}

	DeleteAllName_t( pTblp->pNamep);
	CloseLocalSemaphore ( pTblp -> uiVolAndName_sh );
	GW_free (pTblp, NULL);

	GW_tsh_functrace( FUNC_DELETEFROMVOLUME, GW_FUNC_END, NULL);

}




/*EnglishComment**************************************************************
 *  ID      =   AddToGAgent_t                                                *
 *  name    =   Adds a new member to Volume Table                            *
 *  func    =   Mallocs the structure Grp_t, and returns the memory pointer. *
 *  i/o     =   none                                                         *
 *  return  =   a pointer to the allocated memory : normal                   *
 *              NULL                              : abnormal                 *
 *  common  =   none                                                         *
 *  note    =   none                                                         *
 *  date    =   Nov. 24, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
struct GAgent_t * AddToGAgent_t()
{
	struct GAgent_t * pTblp;
	struct GAgent_t * pWorkp;


	GW_tsh_functrace( FUNC_ADDTOAGENT, GW_FUNC_START, NULL);


	pTblp = (struct GAgent_t *)GW_malloc( sizeof(struct GAgent_t) );

	if (!pTblp) {
		GW_Message( DFAM_CON, NULL, LOG_NO, KDDS11037 );
		goto EndOfFunc;
	} else;

	if (!pAgent_sp) {
		pAgent_sp = pTblp;
		pTblp->pMaep = (struct GAgent_t *)0;
	} else {
		pWorkp = pAgent_sp;
		while ( (pWorkp->pUshirop) != (struct GAgent_t *)0) {
			pWorkp = pWorkp -> pUshirop;
		}
		(pWorkp -> pUshirop) = pTblp;
		(pTblp -> pMaep) = pWorkp;
	}
	strcpy(pTblp->aszIcat, AGT_ICAT);
	pTblp -> pUshirop = (struct GAgent_t *)0;
	pTblp -> iCipherType = CIPHER_NONE;
	pTblp -> 	bAdmin_live = FALSE;
	pTblp -> 	bWatch_live = FALSE;
	pTblp -> 	bLogin_Stop = FALSE;
	pTblp -> 	iCom_SendBuffLen = (int)0;
	pTblp -> 	iCom_ResponseWaitTime = (long)0;
	pTblp -> 	iWatchDogWaitTime = (long)0;
	pTblp -> 	ushDFAMAgtTranceVer = (unsigned short)0;
	pTblp -> 	uiOpenFileNum = (unsigned short)0;

EndOfFunc:
	GW_tsh_functrace( FUNC_ADDTOAGENT, GW_FUNC_END, NULL);

	return(pTblp);

}



/*EnglishComment**************************************************************
 *  ID      =   DeleteFromGAgent_t                                           *
 *  name    =   Deletion of an Agent_t member                                *
 *  Func    =   Deletes a single member of DFAMGrp_t                         *
 *  i/o     =   pTblp   :   struct GAgent_t *   :i: a member to be deleted   *
 *  Return  =   none                                                         *
 *  common  =   none                                                         *
 *  note    =   none                                                         *
 *  date    =   Dec. 12, 1994 by Ichiyoshi                                   *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment*************************************************************/
void DeleteFromGAgent_t(pTblp)
struct GAgent_t * pTblp;
{
	struct GAgent_t *pFront;
	struct GAgent_t *pBack;


	GW_tsh_functrace( FUNC_DELETEFROMAGENT, GW_FUNC_START, NULL);

	if (pTblp->pUshirop != (struct GAgent_t *)0) {
		if (pTblp == pAgent_sp){
			pAgent_sp = pTblp->pUshirop;
			pAgent_sp->pMaep = (struct GAgent_t *)0;
		} else {
			pFront = pTblp -> pMaep;
			pBack = pTblp -> pUshirop;
			pFront->pUshirop = pBack;
			pBack->pMaep = pFront;
		}
	} else {
		if (pTblp == pAgent_sp){
			pAgent_sp = (struct GAgent_t *)0;
		} else {
			pFront = pTblp -> pMaep;
			pFront->pUshirop = (struct GAgent_t *)0;
		}
	}
	GW_free (pTblp, NULL);

	GW_tsh_functrace( FUNC_DELETEFROMAGENT, GW_FUNC_END, NULL);

}


