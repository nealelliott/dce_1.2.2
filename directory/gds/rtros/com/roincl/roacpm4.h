/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roacpm4.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:16  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:37  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:24  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:12  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:23  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:28  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:46  keutel
 * 	creation
 * 	[1994/03/21  13:18:47  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROACPM4_H
#define _ROACPM4_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roacpm4_rcsid[] = { "@(#) $RCSfile: roacpm4.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:36 $" } ;
#endif

/* *************************************************************************/
/*                                                                         */
/*  * COPYRIGHT  (C)                                                       */
/*  * Siemens Nixdorf Informationssysteme AG 1992                          */
/*  * All rights reserved                                                  */
/*  *                                                                      */
/*  * This software is furnished under licence and may be used only        */
/*  * in accordance with the terms of that licence and with the            */
/*  * inclusion of the above copyright notice.                             */
/*  * This software may not be provided or otherwise made available        */
/*  * to, or used by, any other person. No title to or ownership of        */
/*  * the software is hereby  transferred.                                 */
/*                                                                         */
/* *************************************************************************/

/*daton ********************************************************************/
/*                                                                         */
/* TYPE         : INCLUDE                                                  */
/*                                                                         */
/* NAME         : roacpm4.h                                                */
/*                                                                         */
/* AUTHOR       : Baiju Dalal, Chandru                                     */
/* DATE         : 94-01-14                                                 */
/*                                                                         */
/* COMPONENT    : Association Control Service Element (ACSE)               */
/*                Protocol Machine                                         */
/*                                                                         */
/* PRD#/VERS.   : RTS-V2.00                                                */
/*                                                                         */
/* DESCRIPTION  : Declarations for local functions                   */
/*                                                                         */
/* SYSTEM DEPENDENCE :  none                                               */
/*                                                                         */
/* HISTORY      : sccsid  =  @(#)roacpm4.h	1.1 94/01/26                                       */
/*                                                                         */
/* Vers.Nr. | Date    | changes                      |mark| CR# EM#      */
/* 2.00 K0  | 94-01-14|  original                      |bd  |              */
/*          |         |                                |    |              */
/*datoff *******************************************************************/

/* mr?: das prototypiing der matrix fct ist nachzuziehen
*/


static signed short int NEAR PASCAL ro0609_initAbort PROTO ((
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
           unsigned long  int             p4));

static signed short int NEAR PASCAL ro0607_LocalError PROTO (( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

static signed short int NEAR PASCAL ro0610_STA0xA_ASCreq PROTO (( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

static signed short int NEAR PASCAL ro0611_STA2xA_ASCrsp PROTO ((
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

static signed short int NEAR PASCAL ro0613_STA0xAARQ PROTO ((
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

static signed short int NEAR PASCAL ro0614_STA1xAARE PROTO ((
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

static signed short int NEAR PASCAL
			ro0616_STA1xP_CONcnf_neg PROTO ((
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

static signed short int NEAR PASCAL ro0617_STA5xA_RLSreq PROTO (( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

static signed short int NEAR PASCAL ro0618_RLSrsp PROTO ((
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

static signed short int NEAR PASCAL ro0621_RLRQ PROTO (( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

static signed short int NEAR PASCAL ro0623_RLRE PROTO ((
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

static signed short int NEAR PASCAL ro0626_A_ABRreq PROTO ((
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

static signed short int NEAR PASCAL ro0627_ABRTorP_PABind PROTO (( 
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

static signed short int NEAR PASCAL ro0628_EXTERN PROTO ((
       rT0650_acse          NEAR *cxtPtr,
       signed   long  int         pmevnt,
       unsigned long  int             p1,
       unsigned long  int             p2,
       unsigned long  int             p3,
       unsigned long  int             p4 ));

#endif	/* _ROACPM4_H */
