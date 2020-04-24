/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm11.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:48  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:59  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:00  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:30  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:16  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:49  keutel
 * 	creation
 * 	[1994/03/21  13:19:10  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPM11_H
#define _ROPPM11_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppm11_rcsid[] = { "@(#) $RCSfile: roppm11.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:51 $" } ;
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
/* NAME         : roppm11.h                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-01-28                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : PPM intern header                                   */
/*                Type and constant definitions                       */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roppm11.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-01-28|  original                      |hm  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/* ****************************************************************** */
/*                                                                    */
/*                    Type definitions                                */
/*                                                                    */
/* ****************************************************************** */

/* ****************** Union: Event parameter P3 structures, ********* */
/* ****************** during connection establishment       ********* */

typedef union rU04a0_unConn {
   rTro6e_stConnReqPpm    stReq;
   rTro6f_stConnIndPpm    stInd;
   rTro6g_stConnRspPpm    stRsp;
   rTro6h_stConnCnfPpm    stCnf;
} rT04a0_unConn;

#endif	/* _ROPPM11_H */
