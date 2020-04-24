/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm14.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:56  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:04  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:04  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:34  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:41  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:28  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:55  keutel
 * 	creation
 * 	[1994/03/21  13:19:18  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPM14_H
#define _ROPPM14_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppm14_rcsid[] = { "@(#) $RCSfile: roppm14.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:54 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1992                     */
/*  * All rights reserved                                             */
/*  *                                                                 */
/*  * This software is furnished under licence and may be used only   */
/*  * in accordance with the terms of that licence and with the       */
/*  * inclusion of the above copyright notice.                        */
/*  * This software may not be provided or otherwise made available   */
/*  * to, or used by, any other person. No title to or ownership of   */
/*  * the software is hereby  transferred.                            */
/*                                                                    */
/* ****************************************************************** */

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : roppm14.h                                           */
/*                                                                    */
/* AUTHOR       : Shivkumar                                           */
/* DATE         : 93-06-18                                            */
/*                                                                    */
/* COMPONENT    : RTROS presentation protocol machine                 */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : PPM intern header                                   */
/*                Function prototyping                                */
/*                (presentation context handling during decoding)     */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roppm14.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 03.00  K00|93-06-18|  original                      |skh |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

#define PARAMETER   PROTO ((                                                 \
                            rT0430_stContext       *pContext,                \
                            rT0441_stEntContext    *pEntContext,             \
                            rT04a0_unConn      FAR *pConn,                   \
                            rT041G_unPpdu      FAR *pNode,                   \
                            unsigned long  int FAR * FAR *pX410RtpmNode,     \
                            signed   long  int FAR * FAR *pbUserData,        \
                            rT0417_stUserData  FAR * FAR *pUserData,         \
                            unsigned       int FAR *pOutEvent,               \
                            unsigned long  int FAR *phNode,                  \
                            unsigned long  int FAR *pP1,                     \
                            unsigned long  int FAR *pP2,                     \
                            unsigned long  int FAR *pP3,                     \
                            unsigned long  int FAR *pP4                      \
                            ))

/* State-Event-Matrix functions */
signed short int PASCAL ro0450_PCONreq PARAMETER;
signed short int PASCAL ro0451_CP PARAMETER;
signed short int PASCAL ro0452_CPA PARAMETER;
signed short int PASCAL ro0453_CPR PARAMETER;
signed short int PASCAL ro0454_SCONcnfNeg PARAMETER;
signed short int PASCAL ro0455_PCONrspPos PARAMETER;
signed short int PASCAL ro0456_PCONrspNeg PARAMETER;

signed short int PASCAL ro0460_localError PARAMETER;
signed short int PASCAL ro0461_dummyUnexpectedPpdu PARAMETER;
signed short int PASCAL ro0462_dummyUnexpectedSessEvent PARAMETER;
signed short int PASCAL ro0463_PRELreq PARAMETER;
signed short int PASCAL ro0464_SRELind PARAMETER;
signed short int PASCAL ro0465_SRELcnfPos PARAMETER;
signed short int PASCAL ro0466_SRELcnfNeg PARAMETER;
signed short int PASCAL ro0467_PRELrspPos PARAMETER;
signed short int PASCAL ro0468_PRELrspNeg PARAMETER;
signed short int PASCAL ro0469_PUABreq PARAMETER;
signed short int PASCAL ro046a_ARU PARAMETER;
signed short int PASCAL ro046b_ARP PARAMETER;
signed short int PASCAL ro046c_SPABind PARAMETER;
signed short int PASCAL ro046d_PDTreq PARAMETER;
signed short int PASCAL ro046e_TD PARAMETER;

#endif	/* _ROPPM14_H */
