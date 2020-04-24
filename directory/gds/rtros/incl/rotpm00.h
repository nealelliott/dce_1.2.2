/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rotpm00.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:15  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:36:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:15  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:03  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:44  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:38  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:41  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:46  keutel
 * 	creation
 * 	[1994/03/21  13:28:09  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROTPM00_H
#define _ROTPM00_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rotpm00_rcsid[] = { "@(#) $RCSfile: rotpm00.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:15 $" } ;
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
/* NAME         : rotpm00.h                                           */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay                                    */
/* DATE         : 93-03-05                                            */
/*                                                                    */
/* COMPONENT    : RTROS                                               */
/*                                                                    */
/* PRD#/VERS.   : RTROS V03.00                                        */
/*                                                                    */
/* DESCRIPTION  : Events of the Transport Protocol Machine            */
/*                (for CMX and RFC1006).                              */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rotpm00.h	1.2 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 03.00 K00| 93-03-05|  original                      |sss |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************** Protocol machine event number ********************** */
/* ******************                               ********************** */

#define RC0900_TpmEvent         0x0400


/* ****************** Transport service events *************************** */
/* ******************                          *************************** */

    /* ***** DO NOT CHANGE THESE VALUES !!!                                */
    /* ***** Used from Session as index in a jump table                    */

#define RC0901_TCONNECTreq      (RC0900_TpmEvent | 0x00000005L)
#define RC0902_TCONNECTind      (RC0900_TpmEvent | 0x00000000L)
#define RC0903_TCONNECTrsp      (RC0900_TpmEvent | 0x00000006L)
#define RC0904_TCONNECTcnf      (RC0900_TpmEvent | 0x00000001L)

#define RC0905_TDISCONNECTreq   (RC0900_TpmEvent | 0x00000007L)
#define RC0906_TDISCONNECTind   (RC0900_TpmEvent | 0x00000002L)

#define RC0907_TDATAreq         (RC0900_TpmEvent | 0x00000008L)
#define RC0908_TDATAind         (RC0900_TpmEvent | 0x00000004L)

#define RC0909_TCONTINUE        (RC0900_TpmEvent | 0x00000003L)

/* please change the values correctly.... */

#define RC090a_TREDIRECTreq     (RC0900_TpmEvent | 0x00000009L)
#define RC090b_TREDIRECTind     (RC0900_TpmEvent | 0x0000000aL)

/* ********************************************************************* */
/*************  Abort Reasons For RC0906_TDISCONNECTind  *****************/
/* ********************************************************************* */
#define RC0301_DisconnectReasons	0x00000400
#define RC0302_OtherError		0x03000401
#define RC0303_CMXError	        	0x02000401
#define RC0304_SysError 		0x01000401


/* *** Description of the abort reasons and additional information *** *
 * *** (value "aboReason", returned in P3)			   *** *


      +---------+---------+---------+---------+
Byte  |    3	|    2	  |    1    |	 0    |
      +---------+---------+---------+---------+

3 :    Local or CMX abort reason

2 :    In the case of local Disconnect :

          if (value | 0x01) == TRUE  : P2 = -P2;
	      		    == FALSE : P2 =  P2;

          (The lowest bit of the third byte indicates if the value
	   returned in P2 (addInfo) is positive or negative)

       In the case of remote (CMX) Disconnect :

          Both the bits of this byte along with the lowest bit of
          the third byte denote the value returned by the CMX
          function t_disin() in the parameter reason.
       
1 :    Value == 04 : Protocol machine's basic event number

0 :    Value == 00 : Abort reason given by CMX (t_disin function)
       Value == 01 : Local abort reason

 * ****************************************************************** */


/* ********************* Mask to find CMX error  type ********************* */

#define CMXMASK   0xF000

/* ********************* Event-Parameter Matrix ************************** *
 * *********************                        ************************** *

|---------------------|------|----------|----------|------------|----------|
|event                |in/out|p1        |p2        |p3          |p4        |
|---------------------|------|----------|----------|------------|----------|
|RC0901_TCONNECTreq   |IN    |NULL      |NULL      |hCalledAddr |NULL      |
|RC0902_TCONNECTind   |spm   |NULL      |tiduSize  |hCallingAddr|hCalledAdd|
|RC0903_TCONNECTrsp   |IN    |NULL      |NULL      |NULL        |NULL      |
|RC0904_TCONNECTcnf   |spm   |NULL      |tiduSize  |hRespondAddr|NULL      |
|---------------------|------|----------|----------|------------|----------|
|RC0905_TDISCONNECTreq|IN    |NULL      |NULL      |reason      |NULL      |
|RC0906_TDISCONNECTind|spm   |NULL      |addInfo   |reason      |NULL      |
|---------------------|------|----------|----------|------------|----------|
|RC0907_TDATAreq      |IN    |hStore    |Offs/Size |NULL        |NULL      |
|RC0908_TDATAind      |spm   |hStore    |NULL      |NULL        |NULL      |
|---------------------|------|----------|----------|------------|----------|
|RC0909_TContinue     |spm   |NULL      |NULL      |NULL        |NULL      |
|---------------------|------|----------|----------|------------|----------|
|RC090a_TREDIRECTreq  |IN    |hStore    |offset    |NULL        |PID       |   
|RC090b_TREDIRECTind  |spm   |hStore    |offset    |NULL        |PID       |   
|---------------------|------|----------|----------|------------|----------|
|All other values     |IN    |NOTDEFINED|NOTDEFINED|NOTDEFINED  |NOTDEFINED|
|                     |OUT   |NOTDONE   |NOTDONE   |NOTDONE     |NOTDONE   |
|---------------------|------|----------|----------|------------|----------|

IN          : It is an incoming event (received by the TPM).
OUT         : It is an outgoing event  (sent by the TPM).
NOTDEFINED  : This parameter contains a not defined value.
NOTDONE     : Such an event is not issued by the TPM.
NULL        : Parameter not used. Must have the value NULL.
spm         : This event is posted to the SPM
hCalledAddr : Memory handle of a structure containing the Called     T-Address
hCallingAddr: Memory handle of a structure containing the Calling    T-Address
hRespondAddr: Memory handle of a structure containing the Responding T-Address
hStore      : Store handle containing user data
offset      : Offset in store from where user data begins
reason      : Disconnect reason. Not present on a TDISCONNECTreq when the
              underlying loif interafaces to CMX.
addInfo     : Additional information if local reason (abort)
PID         : In case of REDIRECT request - 
              Process ID of the process to which redirection is taking place.
              In case of REDIRECT indication - 
              Process ID of the process from which redirection has taken place.


Note the historically developed generic parameter use:
  p1:  User-data.
  p2:  Additional value "unsigned long int" for direct mapped parameter(s).
  p3:  Local memory handle to direct mapped parameter(s).
       If incoming event: Is to be locked if the value is not NULL.
       (Used if the direct mapped parameter(s) exceed(s) the range of
       two "unsigned long int" values.
  p4:  Value "unsigned long int" for direct mapped parameter(s).


 * ****************** End of event parameter matrix ***************** *
 * ******************                               ***************** */

#endif	/* _ROTPM00_H */
