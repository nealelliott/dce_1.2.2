/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roacpm0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:28  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:28  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:33:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:57  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:28  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:04  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:02  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:56:36  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:03  keutel
 * 	creation
 * 	[1994/03/21  13:26:08  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROACPM0_H
#define _ROACPM0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roacpm0_rcsid[] = { "@(#) $RCSfile: roacpm0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:28 $" } ;
#endif

/* ***************************************************************************/
/*                                                                           */
/*  * COPYRIGHT  (C)                                                         */
/*  * Siemens Nixdorf Informationssysteme AG 1991                            */
/*  * All rights reserved                                                    */
/*  *                                                                        */
/*  * This software is furnished under licence and may be used only          */
/*  * in accordance with the terms of that licence and with the              */
/*  * inclusion of the above copyright notice.                               */
/*  * This software may not be provided or otherwise made available          */
/*  * to, or used by, any other person. No title to or ownership of          */
/*  * the software is hereby  transferred.                                   */
/*                                                                           */
/* ***************************************************************************/

/*daton **********************************************************************/
/*                                                                           */
/* TYPE         : INCLUDE                                                    */
/*                                                                           */
/* NAME         : roacpm0.h                                                  */
/*                                                                           */
/* AUTHOR       : Shivkumar, Haran; Michael Rippstain                                           */
/* DATE         : 91-12-11                                                   */
/*                                                                           */
/* COMPONENT    : Association Control Service Element (ACSE)                 */
/*                Protocol Machine                                           */
/*                                                                           */
/* PRD#/VERS.   : RTS-V2.0                                                   */
/*                                                                           */
/* DESCRIPTION  : Protocol machine eventing interface                        */
/*                                                                           */
/* SYSTEM DEPENDENCE :  none                                                 */
/*                                                                           */
/* HISTORY      : sccsid  =  @(#)roacpm0.h	1.1 94/01/26                                         */
/*                                                                           */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM#        */
/* 2.00  K0 | 91-12-11|  original                      |skh |                */
/*          | 92-01-09|  service events EXTERN_1 and   |mr  |                */
/*          |         |  EXTERN_2 added                |    |                */
/*          |         |                                |    |                */
/*datoff *********************************************************************/

/* ********************** Protocol machine event number ******************** */
/* **********************                               ******************** */

#define RC0600_AcpmEvent           0x0700

/* ********************** AC-PM service events ***************************** */
/* **********************                      ***************************** */

#define RC0601_AA_ASSOCIATEreq     0x0700  /* Associate service events       */
#define RC0602_AA_ASSOCIATEind     0x0702
#define RC0603_AA_ASSOCIATErsp     0x0701
#define RC0604_AA_ASSOCIATEcnf     0x0703

#define RC0605_AA_RELEASEreq       0x0707  /* Release service events         */
#define RC0606_AA_RELEASEind       0x0706
#define RC0607_AA_RELEASErsp       0x0708
#define RC0608_AA_RELEASEcnf       0x0709

#define RC0609_AA_ABORTreq         0x070d  /* User abort service events      */
#define RC060a_AA_ABORTind         0x070e

#define RC060b_AA_PABORTind        0x070f  /* Provider abort service         */
					   /* events                         */

#define RC060c_AA_EXTERN_1         0x0710  /* Disruption by external events  */
#define RC060d_AA_EXTERN_2         0x0711  /*  see: Draft Technical          */
					   /*  Corrigenda to ISO 8649 and    */
					   /*  8650 (ACSE)(Defect Report     */
					   /*  8649/006)                     */

/* ****************** Abort indication reasons (AA_ABORTind) *************** */
/* ******************                                        *************** */

#define RC0610_AA_UserABORT         0x00UL
#define RC0611_AA_ProvABORT        (0x00010000UL | RC0600_AcpmEvent | 0x00UL)
#define RC0612_AAREapduError       (0x01000000UL | RC0600_AcpmEvent | 0x01UL)
#define RC0613_RLRQapduError       (0x02000000UL | RC0600_AcpmEvent | 0x01UL)
#define RC0614_RLREapduError       (0x03000000UL | RC0600_AcpmEvent | 0x01UL)
#define RC0615_ABRTapduError       (0x04000000UL | RC0600_AcpmEvent | 0x01UL)
#define RC0616_PROTsequError       (0x05000000UL | RC0600_AcpmEvent | 0x01UL)

/* ****************** Reason in AA_RELEASE req/ind (p3) ******************** */
/* ******************                                   ******************** */

#define RC061a_NotDefined          (0x00000000L | RC0600_AcpmEvent)
#define RC061b_Normal              (0x00010000L | RC0600_AcpmEvent)
#define RC061c_Urgent              (0x00020000L | RC0600_AcpmEvent)
#define RC061d_UserDefined         (0x00030000L | RC0600_AcpmEvent)

/* ****************** Reason in AA_RELEASE rsp/cnf (p3) ******************** */
/* ******************                                   ******************** */

#define RC061i_NotDefined          (0x00000000L | RC0600_AcpmEvent)
#define RC061j_Normal              (0x00010000L | RC0600_AcpmEvent)
#define RC061k_NotFinished         (0x00020000L | RC0600_AcpmEvent)
#define RC061l_UserDefined         (0x00030000L | RC0600_AcpmEvent)


/* ****************** Result in AA_ASSOCIATE rsp/cnf (p4) ****************** */
/* ******************                                     ****************** */

/*
   result (A-ASSOCIATE-rsp/cnf) = ( Diagnostics | Result | ResultSource )

    ___________________________________________________
   |   Byte 3   |   Byte 2   |   Byte 1   |   Byte 0   |
   |____________|____________|____________|____________|


** Note: result = 0x0  => Association accepted.

   Byte 3 = Not used
   -----------------


   Byte 2 = Not used
   -----------------

   Note exception : = RCro81_UserReject when underlying layers reject.
		      Used only by underlying layers.


   Byte 1 = ResultSource (protocol-machine-number)
   -----------------------------------------------

   For values, see individual service interfaces.
   For e.g. for ResultSource = ACSE, value used is RC0600_AcpmEvent
   defined in file "roacpm0.h".
*/

/*
   Byte 0, High-nibble = Result (reject-type) :
   --------------------------------------------

       B3  B2  B1  HN  LN
   0x              0       -- Accepted
   0x              1       -- Rejected permanent
   0x              2       -- Rejected transient
*/

#define RC0620_Accepted             0x00000000UL
#define RC0621_PermReject           0x00000010UL
#define RC0622_TempReject           0x00000020UL

/*
  Byte 0, Low-nibble = Diagnostics (only in normal mode) :
  --------------------------------------------------------

      B3  B2  B1* HN  LN    (Only if ResultSource is Acse service user)
  0x                   0  -- Null
  0x                   1  -- No reason given
  0x                   2  -- Application Context Name not supported.
  0x                   3  -- Calling AP-title not recognised.
  0x                   4  -- Calling AP-invocation-identifier not recognised.
  0x                   5  -- Calling AE-qualifier not recognised.
  0x                   6  -- Calling AE-invocation-identifier not recognised.
  0x                   7  -- Called AP-title not recognised.
  0x                   8  -- Called AP-invocation-identifier not recognised.
  0x                   9  -- Called AE-qualifier not recognised.
  0x                   a  -- Called AE-invocation-identifier not recognised.

  * : Byte 1 Always null (i.e Acse service user)
*/

#define RC0625_Null                 0x00000000UL
#define RC0626_NoReason             0x00000001UL
#define RC0627_UnsupApplCtxt        0x00000002UL
#define RC0628_UnrecClgApTitle      0x00000003UL
#define RC0629_UnrecClgApInvocId    0x00000004UL
#define RC062a_UnrecClgAeQual       0x00000005UL
#define RC062b_UnrecClgAeInvocId    0x00000006UL
#define RC062c_UnrecCldApTitle      0x00000007UL
#define RC062d_UnrecCldApInvocId    0x00000008UL
#define RC062e_UnrecCldAeQual       0x00000009UL
#define RC062f_UnrecCldAeInvocId    0x0000000aUL

/*
       B3  B2  B1* HN  LN    (Only if ResultSource is Acse Service Provider)
   0x                   0  -- Null
   0x                   1  -- No reason given
   0x                   2  -- No common ACSE version.

   * : Always ORed with RC0600_AcpmEvent (i.e. Acse service provider)
*/

#define RC062g_NoCommAcseVersion    0x00000002UL

/* *********************** Event-Parameter-matrix ************************** **
** ***********************                        ************************** **


|----------------|------|--------------|-------------|------------|------------|
|event           |IN/OUT|p1            |p2           |p3          | p4         |
|----------------|------|--------------|-------------|------------|------------|
|AA_ASSOCIATEreq |IN    |pdv  |noPdv   |NULL         |rTro7e      |NULL        |
|AA_ASSOCIATEind |aUser |pdv  |noPdv   |NULL/Pid     |rTro7f      |assocDiag   |
|AA_ASSOCIATErsp |IN    |pdv  |noPdv   |NULL         |rTro7g      |assocResult |
|AA_ASSOCIATEcnf |aUser |pdv  |noPdv   |NULL         |rTro7h      |assocResult |
|----------------|------|--------------|-------------|------------|------------|
|AA_RELEASEreq   |IN    |pdv  |noPdv   |closeReason  |NULL        |NULL        |
|AA_RELEASEind   |aUser |pdv  |noPdv   |closeReason  |NULL        |NULL        |
|AA_RELEASErsp   |IN    |pdv  |noPdv   |closeReason  |NULL        |closeResult |
|AA_RELEASEcnf   |aUser |pdv  |noPdv   |closeReason  |NULL        |closeResult |
|----------------|------|--------------|-------------|------------|------------|
|AA_ABORTreq     |IN    |pdv  |noPdv   |NULL         |NULL        |NULL        |
|AA_ABORTind     |aUser |pdv  |noPdv   |NULL         |NULL        |abortReason |
|----------------|------|--------------|-------------|------------|------------|
|AA_PABORTind    |aUser |NULL          |addAbortInfo |NULL        |abortReason |
|----------------|------|--------------|-------------|------------|------------|
|AA_EXTERN_1     |IN    |NULL          |NULL         |NULL        |NULL        |
|AA_EXTERN_2     |IN    |NULL          |NULL         |NULL        |NULL        |
|----------------|------|--------------|-------------|------------|------------|
|ALL other       |IN    |NOTDEFINED    |NOTDEFINED   |NOTDEFINED  |NOTDEF.     |
|values          |aUser |NOTDONE       |NOTDONE      |NOTDONE     |NOTDONE     |
|----------------|------|----------------------------|------------|------------|

IN           : Incoming event (received by the ACPM).
OUT          : Outgoing event (sent by the ACPM).
NOTDEFINED   : This parameter contains an undefined value.
NOTDONE      : Such an event is not issued by the ACPM
NULL         : Parameter not used.
aUser        : This event is posted to the user of ACPM service events.
pdv          : Global heap memory handle of pdv-tree root node.
noPdv        : No user data for this event.
assocDiag    : Association establishment diagnostic in indication
	       Only for a local matter to inform the user
assocResult  : Association establishment result values in response/confirm
closeReason  : Association release reason in request/indication/response/confirm
closeResult  : Association release result values in response/confirm
abortReason  : Provider abort reason.
addAbortInfo : Additional abort information.
rTro7e       : Local heap memory handle of A-ASSOCIATE-request
               service primitive parameters                      (see rouply1.h)
rTro7f       : Local heap memory handle to A-ASSOCIATE-indication
               service primitive parameters                      (see rouply1.h)
rTro7g       : Local heap memory handle to A-ASSOCIATE-response
               service primitive parameters                      (see rouply1.h)
rTro7h       : Local heap memory handle to A-ASSOCIATE-confirmation
               service primitive parameters                      (see rouply1.h)


** ********************** End of event parameter matrix ********************* **
** **********************                               ********************* */

#endif	/* _ROACPM0_H */
