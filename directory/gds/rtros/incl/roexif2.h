/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif2.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:41  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:34:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:18  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:38  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:14  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:14  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:15  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:19  keutel
 * 	creation
 * 	[1994/03/21  13:26:37  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROEXIF2_H
#define _ROEXIF2_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roexif2_rcsid[] = { "@(#) $RCSfile: roexif2.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:41 $" } ;
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
/* NAME         : roexif2.h                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 91-10-05                                            */
/*                                                                    */
/* COMPONENT    : RTS external interface modul TPDU interface         */
/*                                                                    */
/* PRD#/VERS.   : RTS-V02.00                                          */
/*                                                                    */
/* DESCRIPTION  : RTS internal returns to TPDU interface              */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      : sccsid  =  " @(#)roexif2.h 1.1 93/11/29"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 02.00 K0 | 91-10-05|  original                      |jm  |         */
/*          |         |                                |    |         */
/* datoff *********************************************************** */

/* *************  TPDU function numbers ***************************** */
/* *************                        ***************************** */

#define RC0101_Open     1
#define RC0102_Read     2
#define RC0103_Write    3
#define RC0104_Position 4
#define RC0105_Close    5
#define RC0106_Delete   6

typedef union rU0100_unCallBack {
    Ptp1001   stOpen;
    Ptp1002   stRead;
    Ptp1003   stWrite;
    Ptp1004   stPosition;
    Ptp1005   stClose;
    Ptp1006   stDelete;
}
rT0100_unCallBack;

/* *************  Prototyping of ROEXIF functions ******************* */
/* *************                                  ******************* */


int FAR PASCAL EXPORT ro0199_tpduCallBack PROTO ((signed short int       ,
						  rT0100_unCallBack FAR *));

/* Return value: Return TPDU functions return values (see tpdu.h) */
/*               or "TP_CALL_ERR" if Parameter1 is unkown.        */
/* Paramer1    : TPDU function number                             */
/* Paramer2    : ITPDU parameter block union (pointer to)         */

#endif	/* _ROEXIF2_H */
