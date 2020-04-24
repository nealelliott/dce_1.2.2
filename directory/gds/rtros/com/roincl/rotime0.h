/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rotime0.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:14  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:12:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:16  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:12  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:42  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:47  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:51  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:06  keutel
 * 	creation
 * 	[1994/03/21  13:17:52  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROTIME0_H
#define _ROTIME0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rotime0_rcsid[] = { "@(#) $RCSfile: rotime0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:02 $" } ;
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
/* NAME         : rotime0.h                                           */
/*                                                                    */
/* AUTHOR       : Natalia Serra                                       */
/* DATE         : 19-9-91                                             */
/*                                                                    */
/* COMPONENT	: RTS Timer protocol machine			      */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Timer interface                                     */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rotime0.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  | Date    | changes                       |mark| CR# EM# */
/* 2.0A00 K00| 19-09-91| original                      |ns  |         */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/* ****************** Protocol machine event number ***************** */
/* ******************                               ***************** */

#define RC1100_TimerEvent           0x1100

/* ****************** Timer-incoming events ************************* */
/* ******************                       ************************* */

#define RC1101_SetTimerReq          0x1100 
#define RC1102_ResetTimerReq        0x1101
#define RC1103_GetEventReq          0x1102


/* ****************** Timer-outgoing events ************************* */
/* ******************                       ************************* */

#define RC1104_TimerInd             0x1103 /* Release service events  */


/* ****************** Event-Parameter-Matrix ************************ *
 * ******************                        ************************ *

|-------------|------|--------------|---------------|----------|----------|
|event        |IN/OUT|p1            |p2             |p3        |p4        |
|-------------|------|--------------|---------------|----------|----------|
|SetTimerReq  |IN    |Delay of time |NULL	    |PM number |Call Id   |
|-------------|------|--------------|---------------|----------|----------|
|ResetTimerReq|IN    |Reset option  |NULL	    |PM number |Call Id   |
|-------------|------|--------------|---------------|----------|----------|
|GetEventReq  |IN    |NULL          |NULL           |NULL      |NULL      |
|-------------|------|--------------|---------------|----------|----------|
|TimerInd     |OUT   |NULL	    |NULL	    |NULL      |Call Id   |
|-------------|------|--------------|---------------|----------|----------|

IN	      : It is an incomming event (received by the Timer PM).
OUT	      : It is an outgoing event  (sent by the Timer PM).
NULL          : Parameter not used.
Delay of time : After how many secs. the timer must give an indication.
Reset option  : Two different reset options for a specific PM (p3):
		== 0 : Reset the first timer call identified by Call Id (p4).
		!= 0 : Reset all the timer calls.
PM number     : Protocol machine number where the Timer indication has to
		be sent.
Call Id       : Identifier which may be used to distinguish between different
		calls to the timer from the same PM.

 * ****************** End of Event-Parameter-Matrix ***************** *
 * ******************                               ***************** */

#endif	/* _ROTIME0_H */
