/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif05.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:14  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:56  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:56  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:10:14  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:15  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:25  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:02:59  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:49  keutel
 * 	creation
 * 	[1994/03/21  13:32:29  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROEXIF05_H
#define _ROEXIF05_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roexif05_rcsid[] = { "@(#) $RCSfile: roexif05.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:14 $" } ;
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
/* NAME         : roexif05.h                                          */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-23                                            */
/*                                                                    */
/* COMPONENT    : ROS external interface modul TPDU interface         */
/*                                                                    */
/* PRD#/VERS.   : RTS-V03.00                                          */
/*                                                                    */
/* DESCRIPTION  : ROS internal returns to TPDU interface              */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      : sccsid  =  " @(#)roexif05.h 1.1 93/11/29"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 02.00 K0 | 91-10-05|  original                      |jm  |         */
/*          |         |                                |    |         */
/* datoff *********************************************************** */

/* *************  TPDU function numbers ***************************** */
/* *************                        ***************************** */

#define RC1301_Open     1
#define RC1302_Read     2
#define RC1303_Write    3
#define RC1304_Position 4
#define RC1305_Close    5
#define RC1306_Delete   6

typedef union rU1300_unCallBack {
    Ptp1001   stOpen;
    Ptp1002   stRead;
    Ptp1003   stWrite;
    Ptp1004   stPosition;
    Ptp1005   stClose;
    Ptp1006   stDelete;
}
rT1300_unCallBack;


/* *************  Prototyping of ROEXIF functions ******************* */
/* *************                                  ******************* */


int FAR PASCAL EXPORT ro1399_tpduCallBack PROTO ((signed short int       ,
						  rT1300_unCallBack FAR *));

/* Return value: Return TPDU functions return values (see tpdu.h) */
/*               or "TP_CALL_ERR" if Parameter1 is unkown.        */
/* Paramer1    : TPDU function number                             */
/* Paramer2    : ITPDU parameter block union (pointer to)         */

#endif	/* _ROEXIF05_H */
