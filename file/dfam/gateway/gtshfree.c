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
 * $Log: gtshfree.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:02  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:12  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:17:36  takano
 * 	First Submit
 * 	[1996/02/21  08:17:27  takano]
 * 
 * $EndLog$
 */
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "dfamer.h"

/************************************************************ EnglishComment */
/*  ID      = Gateway free Function                                          */
/*  name    = DFAM Gateway GW_free                                           */
/*  func    = Free the Area taken by GW_malloc                               */
/*  io      = ptr        :void * : i : Area Pointer you want to free         */
/*            iThread ID :int    : i : Area owner's Thread ID                */
/*                                    if you want to free all the areas the  */
/*                                    thread has.Another Case this must be 0 */
/*  return  = Number of freed area(s)                                        */
/*  common  =                                                                */
/*  note    = Return 0 : No memory  Return -1 : Parameter Error              */
/*            Return -2: DFA System Error                                    */
/*                                                                           */
/* date = May 23, 1995  by T.Sawahata                                        */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/************************************************************ EnglishComment */
int GW_free( void *ptr, int iThreadID ) {


	struct  GMemManage_t *pGMem_t;
	int		iArryNum;
	size_t	uiGMem_Head;
	int		ifreMem_num;
	int 	GW_Clear_GMemTbl( struct GMemManage_t*, int );
	void    *ptr_free;
	int		iReturn;

// EnglishComment start
/*********** GW_free Information ************************************************/
/*                                                                              */
/*   See the GW_malloc Information for the table structure                      */
/*   User hands me the pointer to User Area Pointer and free it if it is        */
/*   Right Pointer .                                                            */
/*                                                                              */
/*   Caution: GMemManage_t MemCopy has not already finished .                   */
/*                                                                              */
/********************************************************************************/

//How to Analyze the Input Parameter in this Function.
//
//            Case 1: ( "ptr" was specified ) & ( "iThreadID" = 0 )
//                    We free the User Memory by the "ptr".
//                    Search the GMemManage_t and free the one Memory "ptr" has,
//                    and clear the table.
//                    If the "ptr" was not match all the pointer in GMemManage_t,
//                    No Memory will be free and return the Error Code.
//
//            Case 2: ( "ptr" was specified ) & ( "iThreadID" was specified,too )
//                    This is an invalid use of this Function.
//                    No Area will be freed.
//
//            Case 3: ( "ptr" = NULL ) & ( "iThreadID" was specified )
//                    We free the User Memory by the "iThreadID".
//                    Search the GMemManage_t and free all the Memory iTheadID has.
//                    and clear the table.
//
//            Case 4: ( "ptr" = NULL ) & ( "iThreadID" = 0,too )
//                    We free Nothing. Return Error Code.
//
// EnglishComment end
	WaitOnLocalSemaphore( uiMalloc_sh );
    ifreMem_num = 0;

	uiGMem_Head = sizeof( struct GMem_HeadFoot );
	ptr_free = (void *)(((char *)ptr) - uiGMem_Head) ;
	if (( ptr != NULL ) && ( iThreadID == 0 )){
		pGMem_t = pMemManage_p;
		if ( pGMem_t == NULL ) {
			iReturn = -2;
			goto Return;
		}
		while(1){
			iArryNum = 0;
			while( iArryNum < MAXARRNUM ) {
				if ( pGMem_t->SGMem_AllcInfo[ iArryNum ].pGMem_malc == ptr_free ) {
					break;
				}
				++iArryNum;
			}
			if ( iArryNum >= MAXARRNUM ) {

				if ( pGMem_t-> SGMem_HInfo.pNextGMemTbl == NULL ) {


                    iReturn = -2;
					goto Return;
				}
				pGMem_t = pGMem_t->SGMem_HInfo.pNextGMemTbl;

				continue;
			}else {
				break;
			}
		}
	free( (void*)ptr_free );
	GW_Clear_GMemTbl( pGMem_t, iArryNum );
	ifreMem_num = 1;
	iReturn = 1;
	goto Return;
	}
	if (( ptr == NULL ) && ( iThreadID != 0 )) {


		pGMem_t = pMemManage_p;
		while(1){
			iArryNum = 0;
			while( iArryNum < MAXARRNUM ) {
				if ( pGMem_t->SGMem_AllcInfo[ iArryNum ].iGMem_thID == iThreadID ) {
					ptr_free = ( pGMem_t->SGMem_AllcInfo[ iArryNum ].pGMem_malc );
					free( ptr_free );
					GW_Clear_GMemTbl( pGMem_t, iArryNum );
					++ifreMem_num;
				}
				++iArryNum;
			}
			if ( pGMem_t->SGMem_HInfo.pNextGMemTbl == NULL ) {

				break;
			}
			pGMem_t = pGMem_t->SGMem_HInfo.pNextGMemTbl;
		}
		iReturn = ifreMem_num;
		goto Return;
	}

	if (((ptr != NULL)&&(iThreadID != 0)) || ((ptr == NULL)&&(iThreadID == 0))) {

		iReturn = -1;
		goto Return;
	}
Return:
	SignalLocalSemaphore( uiMalloc_sh );
	return(iReturn);

}
/********************************************************* EnglishComment ***/
/*  ID      = GW_Clear_GMemTbl                                              */
/*  name    = Free memory management table that all its entories are freed  */
/*  func    = Free memory management table that all its entories are freed  */
/*  io      =                                                               */
/*    pGMem_tG:struct GMemManage_t *: i: Pointer to empty mamory management */
/*                                         table that all its               */
/*  return  = 0: success                                                    */
/*  common  =                                                               */
/*                                                                          */
/* date = March 15, 1995  by T.Sawahata                                     */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/********************************************************* EnglishComment ***/
int GW_Clear_GMemTbl( struct GMemManage_t *pGMem_tG, int iArryNumG ) {



	struct  GMemManage_t *pGMem_tGw1;
	struct  GMemManage_t *pGMem_tGw2;
	void	GW_free_end( int );
	int		iWCount;


	pGMem_tGw1 = pGMem_tG;
	pGMem_tGw1->SGMem_AllcInfo[ iArryNumG ].iGMem_thID = 0;
	pGMem_tGw1->SGMem_AllcInfo[ iArryNumG ].pGMem_malc = NULL;
	memset( pGMem_tGw1->SGMem_AllcInfo[ iArryNumG ].cGMem_thNm, 0x00,8 );

	pGMem_tGw1->SGMem_AllcInfo[ iArryNumG ].uiGMem_size = NULL;

	pGMem_tGw1->SGMem_HInfo.iGMem_status = NOT_FULL;


	iArryNumG = 0;
	while ( iArryNumG < MAXARRNUM ) {
		if ( (pGMem_tGw1->SGMem_AllcInfo[ iArryNumG ].pGMem_malc ) != NULL ) {
			break;
		}
			++iArryNumG;
	}
	if ( iArryNumG >= MAXARRNUM ) {
		pGMem_tGw2 = pMemManage_p;
		if ( pGMem_tGw1 == pGMem_tGw2 ) {
			if ( pGMem_tGw1->SGMem_HInfo.pNextGMemTbl != NULL ) {


				pGMem_tGw2 = pGMem_tGw1->SGMem_HInfo.pNextGMemTbl;

				pMemManage_p = pGMem_tGw2;
				pGMem_tGw2->SGMem_HInfo.pPrevGMemTbl = NULL;

				free ( pGMem_tGw1 );
				iWCount = 1;

				while ( pGMem_tGw2 != NULL ) {
					pGMem_tGw2->SGMem_HInfo.iGMemTblCnt = iWCount;
					pGMem_tGw2 = pGMem_tGw2 ->SGMem_HInfo.pNextGMemTbl;
					++iWCount;
				}
			}
		} else {
				iWCount = pGMem_tGw1->SGMem_HInfo.iGMemTblCnt;

				pGMem_tGw2 = pGMem_tGw1;
				pGMem_tGw1 = pGMem_tGw1->SGMem_HInfo.pNextGMemTbl;

				if( pGMem_tGw1 != NULL ){
					pGMem_tGw1->SGMem_HInfo.pPrevGMemTbl = pGMem_tGw2->SGMem_HInfo.pPrevGMemTbl;

				}
				pGMem_tGw1 = pGMem_tGw2;
				pGMem_tGw2 = pGMem_tGw2->SGMem_HInfo.pPrevGMemTbl;

				pGMem_tGw2->SGMem_HInfo.pNextGMemTbl = pGMem_tGw1->SGMem_HInfo.pNextGMemTbl;

				pGMem_tGw2 = pGMem_tGw1->SGMem_HInfo.pNextGMemTbl;

				free ( pGMem_tGw1 );

				while ( pGMem_tGw2 != NULL ) {


					pGMem_tGw2->SGMem_HInfo.iGMemTblCnt = iWCount;
					pGMem_tGw2 = pGMem_tGw2 ->SGMem_HInfo.pNextGMemTbl;
					++iWCount;
				}
			}
	}
		return(0);
}
