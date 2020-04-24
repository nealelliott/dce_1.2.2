/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roloif32.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:19  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:05  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:10:26  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:20  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:30  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:13  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:54  keutel
 * 	creation
 * 	[1994/03/21  13:32:43  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROLOIF32_H
#define _ROLOIF32_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roloif32_rcsid[] = { "@(#) $RCSfile: roloif32.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:19 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1991                     */
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
/* NAME         : roloif3.h                                           */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay.                                   */
/* DATE         : 93-03-05                                            */
/*                                                                    */
/* COMPONENT    :                                                     */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V03.00                                        */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roloif32.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  | Date    | changes                       |mark| CR# EM# */
/* 3.0A00 K00| 91-10-28| original                      |ns  |         */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/* ************    Values for stopState context field	 ************ */

#define NO_STOP 	 0	 /* No stop condition is set	      */
#define STOP_TRUE	 1	 /* Stop set, valid in all situations */

/* ************    Values for maximum length for a tmp. file ******** */

#define MAXFILELEN       25      /* used in redirection               */

/* ************    Other values for NetBIOS                  ******** */

#define RC0330_NetBIOSHeadLen    1  /* Length of NetBIOS header       */


/* *************    Lower Interface context structure	 ************ */
/* *************					 ************ */

typedef struct rS0300_roloifContext {

   int                  iTransConId;     /* Transport connection identifier*/
   unsigned long  int   ulTiduLength;    /* length of a TIDU               */
   unsigned long  int   hRecStore;       /* store handle for Receive       */
   unsigned long  int   hFirstSendQu;    /* hdl of the first element in    */
                                         /* the Send Queue                 */
   unsigned long  int   hLastSendQu;     /* hdl of the last element in     */
                                         /* the Send Queue                 */
   signed	  int   iStopState;      /* stop state. Possible values    */
				         /* defined above 		   */
   signed   short int   bNetBIOS;        /* TRUE if NetBIOS is supported   */
   unsigned long  int   hSendData;       /* Memory handle of Send Data     */
   char                *pSendData;       /* Pointer to Send Data.          */
   unsigned long  int   ulSendLen;       /* Length of data in pSendData.   */
   unsigned       char  cHeadLen;        /* Length of NetBIOS header       */
} rT0300_roloifContext;


/* ******************    Send data control structure    ************* */
/* ******************                                   ************* */

typedef struct rS0301_sendQueue {

   signed	 int   iSendEvent;
   unsigned long int   hSendStore;
   unsigned long int   ulDataLen;
   unsigned long int   ulP2;
   unsigned long int   ulP3;
   unsigned long int   ulP4;
   unsigned long int   hNextElem;

} rT0301_sendQueue;

#endif	/* _ROLOIF32_H */
