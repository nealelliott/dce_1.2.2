/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropzx.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:26  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:06  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:17  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:34  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:28  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:35  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:08  keutel
 * 	creation
 * 	[1994/03/21  13:33:06  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROROPZX_H
#define _ROROPZX_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roropzx_rcsid[] = { "@(#) $RCSfile: roropzx.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:26 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1993                     */
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
/* NAME         : roropzx.h                                           */
/*                                                                    */
/* AUTHOR       : Shivkumar Haran                                     */
/* DATE         : 93-07-23                                            */
/*                                                                    */
/* COMPONENT    : Remote Operations Service.                          */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V03.00 K00                                    */
/*                                                                    */
/* DESCRIPTION  : Definitions/Function prototypes for the MAVROS      */
/*                encoding and decoding routines.                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES : Depends on the MAVROS roropzz.h.             */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropzx.h	1.2 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 03.00 K00 |93-07-23|  original                      |skh |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/*----------------------------------------------------------------------------*/
/* The function prototypes for the encoding and decoding routines are         */
/* generated automatically by the MAVROS compiler in the file "roropzz.h".    */
/* This include file includes some header files also included in the ROPM     */
/* modules. So that there is no duplication of such includes, the MAVROS      */
/* header file is not included. Rather, just the prototypes are brought into  */
/* this separate header file created just for the purpose of ROPM. Certain    */
/* constants that are specific to these prototypes are also additionally      */
/* defined below.                                                             */
/*----------------------------------------------------------------------------*/

typedef unsigned long int      STOREHANDLE;
typedef void              NEAR *asn1_pvoid;

/*----------------------------------------------------------------------------*/
/* Encoding routines                                                          */
/*----------------------------------------------------------------------------*/


int PASCAL ROSEapdus_cod PROTO((STOREHANDLE,int,long,rT0715_unRoseApdus FAR *,asn1_pvoid));


/*----------------------------------------------------------------------------*/
/* Decoding routines                                                          */
/*----------------------------------------------------------------------------*/

int PASCAL ROSEapdus_dec PROTO((STOREHANDLE,unsigned long,rT0715_unRoseApdus FAR *,asn1_pvoid));
int PASCAL RTS_any_dec PROTO((STOREHANDLE, unsigned long, unsigned long FAR *,
			      int , void NEAR *));

#endif	/* _ROROPZX_H */
