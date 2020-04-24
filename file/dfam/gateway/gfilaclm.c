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
 * $Log: gfilaclm.c,v $
 * Revision 1.1.2.2  1996/03/11  14:08:00  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:21  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:54:26  takano
 * 	First Submit
 * 	[1996/02/21  07:54:22  takano]
 * 
 * $EndLog$
 */
#include "dfamdef.h"
#include "dfamstr.h"
#include "dfamer.h"
#include "err.h"
#include "gfiler.h"

#define  TA_DIR     1
#define  TA_FILE    2
#define  AT_DIR     3
#define  AT_FILE    4
#define  CLEAR      0

/*EnglishComment**************************************************************/
/*   ID      =   GW_Rights_Map      File,Directory Access Rights             */
/*                                                                           */
/*   name    =   File,Directory Access Rights Converter                      */
/*   func    =   Get Trustee Rights,convert it to ACL ( mode = 1,2 )         */
/*               Get ACL,convert it to Trustee Rights ( mode = 3,4 )         */
/*   i/o     =   pRghtsMap: struct RightsMapping* : i/o : Access Rights      */
/*                                                                           */
/*   return  =   0: Success                                                  */
/*              -1: Mode or specified Pointer is invalid                     */
/*              -2: Impossible to map                                        */
/*                                                                           */
/*   common  =   none                                                        */
/*   note    =   none                                                        */
/*   date    =   February 22, 1995  by Tomoharu Sawahata and Jun Nakagawa    */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/**************************************************************EnglishComment*/

int GW_Rights_Map( struct	RightsMapping *pRghtsMap ) {

	unsigned int	uiA_list;
	unsigned int	uiT_list;
	struct RightsMapping  *pDFSACL;
	unsigned int	uialist_cmpw;
	unsigned int	uialist2_cmpw;
	unsigned int	uitlist_cmpw;
	unsigned int	uitlist2_cmpw;
	if ( pRghtsMap == NULL ) {
		return(-1);
	}
	pDFSACL = pRghtsMap;
	uialist_cmpw = CLEAR;
	uialist2_cmpw = CLEAR;
	uitlist_cmpw = CLEAR;
	uitlist2_cmpw = CLEAR;
	uiA_list = CLEAR;
	uiT_list = CLEAR;
	switch ( pDFSACL->mode ) {

		case TA_DIR:

			uiT_list = (unsigned int)( pDFSACL->trustee_list );


			if ( uiT_list == 0 ) {
				( pDFSACL->ACL_list ) = 0;
				return(0);
			}

			uitlist_cmpw = T_SUPER | T_READ | T_WRITE | T_MODIFY | T_Q;
			uitlist2_cmpw = uitlist_cmpw | uiT_list;
			if ( uitlist_cmpw == uitlist2_cmpw ) {
				return(-2);
			}

			uitlist_cmpw = uiT_list | T_FIND;
			if ( uitlist_cmpw == uiT_list ) {
				uiA_list = uiA_list | A_READ | A_EXECUTE;
			}
			uitlist_cmpw = uiT_list | T_ACCESS;
			if ( uitlist_cmpw == uiT_list ) {
				uiA_list = uiA_list | A_CONTROL;
			}
			uitlist_cmpw = uiT_list | T_CREATE;
			if ( uitlist_cmpw == uiT_list ) {
				uiA_list = uiA_list | A_WRITE | A_EXECUTE | A_INSERT;

			}
			uitlist_cmpw = uiT_list | T_ERASE;
			if ( uitlist_cmpw == uiT_list ) {
				uiA_list = uiA_list | A_WRITE | A_EXECUTE | A_DELETE;

			}
			( pDFSACL->ACL_list ) = uiA_list;
			return(0);

		case TA_FILE :
			uiT_list = (unsigned int)( pDFSACL->trustee_list );


			if ( uiT_list == 0 ) {
				( pDFSACL->ACL_list ) = 0;
				return(0);
			}

			uitlist_cmpw = T_SUPER | T_CREATE | T_ERASE | T_MODIFY | T_FIND | T_Q;

			uitlist2_cmpw = uitlist_cmpw | uiT_list;
			if ( uitlist_cmpw == uitlist2_cmpw ) {
				return(-2);
			}


			uitlist_cmpw = uiT_list | T_READ;
			if ( uitlist_cmpw == uiT_list ) {
				uiA_list = uiA_list | A_READ;
			}
			uitlist_cmpw = uiT_list | T_WRITE;
			if ( uitlist_cmpw == uiT_list ) {
				uiA_list = uiA_list | A_WRITE;
			}
			uitlist_cmpw = uiT_list | T_ACCESS;
			if ( uitlist_cmpw == uiT_list ) {
				uiA_list = uiA_list | A_CONTROL;
			}

			( pDFSACL->ACL_list ) = uiA_list;
			return(0);


		case AT_DIR:


			uiA_list = (unsigned int)( pDFSACL->ACL_list );


			uialist_cmpw = A_WRITE | A_INSERT | A_DELETE;

			uialist2_cmpw = uialist_cmpw | uiA_list;
			if ( uialist_cmpw == uialist2_cmpw ) {
				( pDFSACL->trustee_list ) = CLEAR;
				return(0);
			}


			uialist_cmpw = uiA_list | A_READ;
			if ( uialist_cmpw == uiA_list ) {
				uiT_list = uiT_list | T_FIND;
			}
			uialist_cmpw = uiA_list | A_EXECUTE;
			if ( uialist_cmpw == uiA_list ) {
				uiT_list = uiT_list | T_FIND;
			}
			uialist_cmpw = uiA_list | A_CONTROL;
			if ( uialist_cmpw == uiA_list ) {
				uiT_list = uiT_list | T_ACCESS;
			}

			uialist_cmpw = uiA_list | A_WRITE | A_EXECUTE | A_INSERT | A_DELETE;

			if ( uialist_cmpw == uiA_list ) {
				uiT_list = uiT_list | T_WRITE | T_CREATE | T_ERASE | T_MODIFY;

				( pDFSACL->trustee_list ) = (WORD)uiT_list;
				goto ATDIRet;
			} else {
				uialist_cmpw = uiA_list | A_WRITE | A_EXECUTE | A_INSERT;

				if ( uialist_cmpw == uiA_list ) {
					uiT_list = uiT_list | T_WRITE | T_CREATE | T_Q;
					( pDFSACL->trustee_list ) = (WORD)uiT_list;

					goto ATDIRet;
				}
				uialist_cmpw = uiA_list | A_WRITE | A_EXECUTE | A_DELETE;

				if ( uialist_cmpw == uiA_list ) {
					uiT_list = uiT_list | T_ERASE;
				}
				goto ATDIRet;
			}
ATDIRet:
	( pDFSACL->trustee_list ) = (WORD)uiT_list;
	return(0);

		case AT_FILE :
			uiA_list = (unsigned int)( pDFSACL->ACL_list );


			uialist_cmpw =  A_INSERT | A_DELETE;
			uialist2_cmpw = uialist_cmpw | uiA_list;
			if ( uialist_cmpw == uialist2_cmpw ){
				( pDFSACL->trustee_list ) = CLEAR;
				return(0);
			}


			uialist_cmpw = uiA_list | A_READ;
			if ( uialist_cmpw == uiA_list ) {
				uiT_list = uiT_list | T_READ | T_FIND;
			}
			uialist_cmpw = uiA_list | A_WRITE;
			if ( uialist_cmpw == uiA_list ) {
				uiT_list = uiT_list | T_WRITE | T_CREATE | T_Q;
			}
			uialist_cmpw = uiA_list | A_EXECUTE;
			if ( uialist_cmpw == uiA_list ) {
				uiT_list = uiT_list | T_READ | T_FIND;
			}
			uialist_cmpw = uiA_list | A_CONTROL;
			if ( uialist_cmpw == uiA_list ) {
				uiT_list = uiT_list | T_ACCESS;
			}

			( pDFSACL->trustee_list ) = (WORD)uiT_list;
			return(0);

		default:
			return(-1);
	}
}

