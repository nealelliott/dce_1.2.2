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
 * $Log: gtshfall.c,v $
 * Revision 1.1.2.2  1996/03/11  14:09:00  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:11  marty]
 *
 * Revision 1.1.2.1  1996/02/21  08:17:09  takano
 * 	First Submit
 * 	[1996/02/21  08:17:05  takano]
 * 
 * $EndLog$
 */
#include"dfamdef.h"
#include"dfamstr.h"
#include"dfamprt.h"
#include"dfamext.h"

#include "dfamer.h"

#define RW_OK     0
#define GMEMSIZE  65536

/*********************************************************** EnglishComment **/
/*  ID = GW_free_all                                                         */
/*  name = Clean up Mem pool                                                 */
/*  func = Free all fragments allocated by GW_malloc function and remained   */
/*          in Server's Mem pool                                             */
/*  i/o  = none                                                              */
/*  return = none                                                            */
/* date = May 23, 1995  by T.Sawahata                                        */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*********************************************************** EnglishComment **/

void GW_free_all( void ) {


	struct GMem_AllcInfo	*pGMemAllcEnd_t;
	struct GMem_AllcInfo	*pGMemAllc_t;
	struct GMemManage_t 	*pGMemw_t;
	struct GMemManage_t 	*pGMemwF_t;
	void					*pMalcArea;
	int						iCNT;

	WaitOnLocalSemaphore( uiMalloc_sh );


	pGMemw_t = pMemManage_p;
	while ( pGMemw_t != NULL ) {

		pGMemAllc_t = pGMemw_t->SGMem_AllcInfo;
		pGMemAllcEnd_t = pGMemw_t->SGMem_HInfo.pGMemTblEnd;
		iCNT = NULL;
		while ( pGMemAllc_t < pGMemAllcEnd_t ) {
			pMalcArea = ( pGMemAllc_t->pGMem_malc );
			if ( pMalcArea != NULL ) {
				free( pMalcArea );
			}
			++pGMemAllc_t;

		}
		pGMemwF_t = pGMemw_t;
		pGMemw_t = pGMemw_t->SGMem_HInfo.pNextGMemTbl;
		free( pGMemwF_t );
	}
	pMemManage_p = NULL;
	SignalLocalSemaphore( uiMalloc_sh );

}
