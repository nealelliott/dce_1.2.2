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
 * $Log: gtshmalc.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:07  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:14  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:18:28  takano
 * 	First Submit
 * 	[1996/02/21  08:18:23  takano]
 * 
 * $EndLog$
 */
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamext.h"
#include "dfamprt.h"
#include "dfammsg.h"
#include "dfamer.h"

#define NOT_FULL   0
#define FULL       1

/********************************************************* EnglishComment ***/
/*  ID      = DFAM Gateway malloc Function                                  */
/*  name    = DFAM Gateway GW_malloc                                        */
/*  func    = Get Clear Area for DFAM Gateway                               */
/*  io      = Size of Area                                                  */
/*  return  = Area Pointer                                                  */
/*  common  =                                                               */
/*  note    = Returns 0 for Error                                           */
/*                                                                          */
/* date = March 15, 1995  by T.Sawahata                                     */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/********************************************************* EnglishComment ***/
void *GW_malloc( size_t uiSize ){

	int						iThreadID;
	void					*pGMem_UsrPtr;
	void					*pGMem_GetPtr;
	size_t					uiGW_mal_Size;
	int						ipGMem_Array;
	size_t					uiGMem_Head;
	size_t					uiGMem_Foot;
	char                    cNameWork[20];
	struct GMem_HeadFoot	*pGMemHeadFoot;
	struct GMemManage_t		*pGMem_malloc;
	struct GMemManage_t		*pGMem_curTbl;
	struct GMem_AllcInfo	*pGMem_EndPtr;
	struct GMemManage_t		*GW_Make_GMemTbl( void );

	WaitOnLocalSemaphore( uiMalloc_sh );
	if ( uiSize == 0 ) {
		goto Return;
	}
	iThreadID = GetThreadID();
	uiGMem_Head = sizeof ( struct GMem_HeadFoot );
	uiGMem_Foot = sizeof ( struct GMem_HeadFoot );
	if ( uiSize > ( MAXSIZE32 - uiGMem_Head - uiGMem_Foot ) ) {


		goto Return;
	}
	if ( pMemManage_p == NULL ) {


		pGMem_malloc = (struct GMemManage_t *)calloc( 1,sizeof(struct GMemManage_t));

		if ( pGMem_malloc == NULL ) {
			goto Return;
		}
		pMemManage_p = pGMem_malloc;
		( pGMem_malloc -> SGMem_HInfo.pPrevGMemTbl ) = ( struct GMemManage_t * )0;


		( pGMem_malloc -> SGMem_HInfo.pNextGMemTbl ) = ( struct GMemManage_t * )0;


		( pGMem_malloc -> SGMem_HInfo.iGMemTblCnt ) = 1;
		pGMem_EndPtr = ( struct GMem_AllcInfo * )( pGMem_malloc );
		pGMem_EndPtr = ( struct GMem_AllcInfo * )( ( char * )pGMem_EndPtr
                       + sizeof( struct GMemManage_t ) - sizeof( struct GMem_AllcInfo ) );

		( pGMem_malloc -> SGMem_HInfo.pGMemTblEnd ) = pGMem_EndPtr;
		( pGMem_malloc -> SGMem_HInfo.iGMem_status ) = 0;
	}
	pGMem_curTbl = pMemManage_p;
	while(1){
		if ( ( pGMem_curTbl->SGMem_HInfo.iGMem_status ) == FULL ) {
			pGMem_curTbl = pGMem_curTbl->SGMem_HInfo.pNextGMemTbl;


			if ( pGMem_curTbl == NULL ){
				pGMem_curTbl = GW_Make_GMemTbl();
				if ( pGMem_curTbl == NULL ) {
					goto Return;
				}
				break;
			}
			continue;
		}
		break;
	}


	ipGMem_Array = 0;
	while ( ipGMem_Array < MAXARRNUM ) {
		if ( pGMem_curTbl->SGMem_AllcInfo[ipGMem_Array].pGMem_malc != NULL) {
			++ipGMem_Array;
			continue;
		}
		break;
	}
	if ( pGMem_curTbl->SGMem_AllcInfo[ipGMem_Array].pGMem_malc != NULL ) {
		goto Return;
	}


	uiGW_mal_Size = uiSize + uiGMem_Head + uiGMem_Foot;
	pGMem_GetPtr = calloc( 1,uiGW_mal_Size );
	if ( pGMem_GetPtr == NULL ) {
		goto Return;
	}

	pGMem_curTbl->SGMem_AllcInfo[ipGMem_Array].iGMem_thID = iThreadID;

	pGMem_curTbl->SGMem_AllcInfo[ipGMem_Array].pGMem_malc = pGMem_GetPtr;

	GetThreadName( iThreadID, cNameWork );
	memcpy ( pGMem_curTbl->SGMem_AllcInfo[ipGMem_Array].cGMem_thNm, cNameWork, 8 );

	uiGW_mal_Size = uiSize + uiGMem_Head + uiGMem_Foot;
	pGMem_curTbl->SGMem_AllcInfo[ipGMem_Array].uiGMem_size = uiGW_mal_Size;


	pGMemHeadFoot = ( struct GMem_HeadFoot * )pGMem_GetPtr;
	memcpy ( pGMemHeadFoot->cGMem_THNm, cNameWork, 8 );
	pGMemHeadFoot->uiSizeM  = uiGW_mal_Size;


	pGMem_UsrPtr = (void *)((char *)pGMem_GetPtr + uiGW_mal_Size - uiGMem_Foot);

	memcpy ( pGMem_UsrPtr, pGMem_GetPtr, sizeof(struct GMem_HeadFoot ));


	ipGMem_Array = 0;
	while ( ipGMem_Array < MAXARRNUM ) {
		if ( pGMem_curTbl->SGMem_AllcInfo[ipGMem_Array].pGMem_malc != NULL) {
			++ipGMem_Array;
			continue;
		}
		break;
	}
	if ( ipGMem_Array >= MAXARRNUM ) {
		pGMem_curTbl->SGMem_HInfo.iGMem_status = FULL;
	}


	pGMem_UsrPtr = ( void * )( ( char * )pGMem_GetPtr + uiGMem_Head );

	SignalLocalSemaphore( uiMalloc_sh );
	return( pGMem_UsrPtr );

Return:
	SignalLocalSemaphore( uiMalloc_sh );
	return(0);

}
/********************************************************* EnglishComment ***/
/*  ID      = GW_Make_GMemTbl                                               */
/*  name    = Allocate new memory management table node and initialize it.  */
/*  func    = Allocate new memory management table node and initialize it.  */
/*  io      = void                                                          */
/*  return  =                                                               */
/*       pointer to flesh allocated area                                    */
/*       NULL: error in allocation new area                                 */
/*  common  =                                                               */
/*                                                                          */
/* date = March 15, 1995  by T.Sawahata                                     */
/*                                                                          */
/* Trade Secret of Hitachi, Ltd.                                            */
/* Do not disclose without written concession from Hitachi, Ltd.            */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/********************************************************* EnglishComment ***/
struct GMemManage_t *GW_Make_GMemTbl( void ) {
	int						iWCount;
	struct GMem_AllcInfo	*pGMem_EndPtr;
	struct GMemManage_t		*pGMem_curTbl;
	struct GMemManage_t		*pGMemTbl;

	pGMem_curTbl = pMemManage_p;
	while(1) {
		if ( pGMem_curTbl->SGMem_HInfo.pNextGMemTbl != NULL ) {
			pGMem_curTbl = pGMem_curTbl->SGMem_HInfo.pNextGMemTbl;
			continue;
		}
		break;
	}
	pGMemTbl = calloc(1,sizeof(struct GMemManage_t));
	if ( pGMemTbl == NULL ) {
		return(0);
	}
	( pGMem_curTbl->SGMem_HInfo.pNextGMemTbl ) = pGMemTbl;
	( pGMemTbl->SGMem_HInfo.pPrevGMemTbl ) = pGMem_curTbl;
	iWCount = ( pGMem_curTbl->SGMem_HInfo.iGMemTblCnt );
	( pGMemTbl->SGMem_HInfo.iGMemTblCnt ) = ++iWCount;

	pGMem_EndPtr = ( struct GMem_AllcInfo * )(( char * )pGMemTbl
                             + sizeof( struct GMemManage_t ) - sizeof( struct GMem_AllcInfo ));
	( pGMemTbl->SGMem_HInfo.pGMemTblEnd ) = pGMem_EndPtr;
	( pGMemTbl->SGMem_HInfo.iGMem_status ) = NOT_FULL;
	return( pGMemTbl );
}
