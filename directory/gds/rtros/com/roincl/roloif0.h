/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roloif0.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:51  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:55  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:23  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:31  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:55  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:40  keutel
 * 	creation
 * 	[1994/03/21  13:17:40  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROLOIF0_H
#define _ROLOIF0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roloif0_rcsid[] = { "@(#) $RCSfile: roloif0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:45 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1990                     */
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
/* NAME         : roloifev.h                                          */
/*                                                                    */
/* AUTHOR       : C. Schoenherr                                       */
/* DATE         : 91-01-01                                            */
/*                                                                    */
/* COMPONENT    : ROS                                                 */
/*                                                                    */
/* PRD#/VERS.   : ROS-V2.50                                           */
/*                                                                    */
/* DESCRIPTION  : Events of the Lower Interface Protocol Machine      */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roloif0.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/*  2.50 K0 | 91-01-01|  original                      | sc |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


/* ****************** Lower Interface  event number ********************** */
/* ******************                               ********************** */

#define RC0300_LoifEvent         0x0300


/* ****************** Transport service events *************************** */
/* ******************                          *************************** */

/* ***** Incomming Events ************************************************ */
#define RC0301_LOPENreq         RC0300_LoifEvent + 0x0000
#define RC0303_LOPENrsp         RC0300_LoifEvent + 0x0001
#define RC0305_LCLOSEreq        RC0300_LoifEvent + 0x0002
#define RC0307_LWRITEreq        RC0300_LoifEvent + 0x0003

/* ***** Outgoing Events ************************************************* */
#define RC0302_LOPENind         RC0300_LoifEvent + 0x0004
#define RC0304_LOPENcnf         RC0300_LoifEvent + 0x0005
#define RC0306_LCLOSEind        RC0300_LoifEvent + 0x0006
#define RC0308_LREADind         RC0300_LoifEvent + 0x0007
#define RC0309_LWRITEcnf        RC0300_LoifEvent + 0x0008

/* ***** Event to receive an event *****************************************/
#define RC0310_LRECEIVE         RC0300_LoifEvent + 0x0009


/* ****************** Event-Parameter Matrix ************************ *
 * ******************                        ************************ *

  |---------------------|------|----------|----------|----------|----------|
  |event                |in/out|p1        |p2        |p3        |p4        |
  |---------------------|------|----------|----------|----------|----------|
  |RC0301_LOPENreq      |in    | -        | buffsize |hAddress  | -        |
  |RC0302_LOPENind      |   TPM| -        | -        |hAddress  | -        |
  |RC0303_LOPENrsp      |in    | -        | buffsize | -        | -        |
  |RC0304_LOPENcnf      |   TPM| -        | -        |hAddress  | -        |
  |---------------------|------|----------|----------|----------|----------|
  |RC0305_LCLOSEreq     |in    | -        | -        | -        | -        |
  |RC0306_LCLOSEind     |   TPM| -        |addInfo   |reason    | -        |
  |---------------------|------|----------|----------|----------|----------|
  |RC0307_LWRITEreq     |in    |hData     |offs/size | -        | -        |
  |RC0308_LREADind      |   TPM|hData     |offs/size | -        | -        |
  |---------------------|------|----------|----------|----------|----------|
  |RC0309_LWRITEcnf     |   TPM| -        | -        | -        | -        |
  |---------------------|------|----------|----------|----------|----------|
  |RC0310_LRECEIVE      |in    | -        | -        | -        | -        |
  |---------------------|------|----------|----------|----------|----------|
  |all other values     |in    |NOTDEFINED|NOTDEFINED|NOTDEFINED|NOTDEFINED|
  |                     |   out|NOTDONE   |NOTDONE   |NOTDONE   |NOTDONE   |
  |---------------------|------|----------|----------|----------|----------|

  in          : It is an incoming event (received by the Lower Interface)
  out         : It is an outgoing event (sent by the Lower Interface)
  TPM         : This event is posted to the TPM
  buffsize    : Size of max. needed read buffer
  hAddress    : Memory handle of a structure containing the T-Address
  hData       : Memory handle of data buffer
  offs/size   : Offset (low word) and Size (high word) of data in buffer
  (user)      : User defined value
  reason      : Reason of closing (local abort only)
  addInfo     : Additionaly information if local reason (abort)
  -           : Parameter not used
  NOTDEFINED  : This parameter contains a not defined value
  NOTDONE     : Such an event is not issued by the Lower Interface

 * ****************** End of event parameter matrix ***************** *
 * ******************                               ***************** */

#endif	/* _ROLOIF0_H */
