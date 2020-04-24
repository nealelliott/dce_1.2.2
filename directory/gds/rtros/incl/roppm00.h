/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm00.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:49  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:32  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:44  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:20  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:21  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:38  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:30  keutel
 * 	creation
 * 	[1994/03/21  13:26:55  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPM00_H
#define _ROPPM00_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppm00_rcsid[] = { "@(#) $RCSfile: roppm00.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:49 $" } ;
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
/* NAME         : roppm00.h                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 92-06-24                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Protocol machine service interface                  */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roppm00.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-03-24|  adaptation of ROS V2.5A00 K05 |hm  |         */
/* 2.0A00 K02|92-06-24|  abort reasons 0x..52 deleted, |hm  |         */
/*           |        |  PUAind: P4 before PCONrsp too |    |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/* ****************** Protocol machine basic event number *********** */
/* ******************                                     *********** */

#define RC0400_PpmEvent           0x0600

/* ****************** Presentation service events ******************* */
/* ******************                             ******************* */

/* N O T E :  Be  V E R Y   C A R E F U L  if you want to change      */
/*            numeric values of event numbers !                       */
/*            (The numbers are used as indices in State-Event-Matrices*/
/*            or their right sequence is necessary for defined ranges */
/*            of event numbers, see roppm03.h.)                       */

#define RC0401_PCONNECTreq        0x0600   /* Connect events          */
#define RC0402_PCONNECTind        0x0603
#define RC0403_PCONNECTrsp        0x0602
#define RC0404_PCONNECTcnf        0x0604

#define RC0405_PRELEASEreq        0x0607   /* Release events          */
#define RC0406_PRELEASEind        0x060a
#define RC0407_PRELEASErsp        0x0609
#define RC0408_PRELEASEcnf        0x060b

#define RC0409_PUABORTreq         0x060d   /* User abort events       */
#define RC040a_PUABORTind         0x060e

#define RC040b_PPABORTind         0x060f   /* Provider abort event    */

#define RC040c_PDATAreq           0x0611   /* Normal data events      */
#define RC040d_PDATAind           0x0610

#define RC040e_PTOKEN_PLEASEreq   0x0620   /* Token request events    */
#define RC040f_PTOKEN_PLEASEind   0x0640

#define RC040g_PCONTROL_GIVEreq   0x0621   /* Token give events       */
#define RC040h_PCONTROL_GIVEind   0x0641

#define RC040i_PSYNC_MINORreq     0x0622   /* Minor Synchronization   */
#define RC040j_PSYNC_MINORind     0x0642   /* events                  */
#define RC040k_PSYNC_MINORrsp     0x0623
#define RC040l_PSYNC_MINORcnf     0x0643

#define RC040m_PUEXCEPT_REPORTreq 0x0624   /* User exception report   */
#define RC040n_PUEXCEPT_REPORTind 0x0644   /* events                  */

#define RC040o_PPEXCEPT_REPORTind 0x064d   /* Provider exception      */

#define RC040p_PACT_STARTreq      0x0625   /* Activity Start events   */
#define RC040q_PACT_STARTind      0x0645

#define RC040r_PACT_RESUMEreq     0x0626   /* Activity Resume events  */
#define RC040s_PACT_RESUMEind     0x0646

#define RC040t_PACT_ENDreq        0x0627   /* Activity End events     */
#define RC040u_PACT_ENDind        0x0647
#define RC040v_PACT_ENDrsp        0x0628
#define RC040w_PACT_ENDcnf        0x0648

#define RC040x_PACT_INTERRUPTreq  0x0629  /* Activity Interrupt       */
#define RC040y_PACT_INTERRUPTind  0x0649  /* events                   */
#define RC040z_PACT_INTERRUPTrsp  0x062a
#define RC040A_PACT_INTERRUPTcnf  0x064a

#define RC040B_PACT_DISCARDreq    0x062b  /* Activity Discard         */
#define RC040C_PACT_DISCARDind    0x064b  /* events                   */
#define RC040D_PACT_DISCARDrsp    0x062c
#define RC040E_PACT_DISCARDcnf    0x064c

#define RC040F_PCONTINUE          0x064e  /* Local events             */
#define RC040G_PSYNCPOINT         0x064f  /* (to the PS-user)         */






/* ****************** Provider abort reason values (p4) ************* *
 * ******************                                   ************* *

    ___________________________________________________
   |   Byte 3   |   Byte 2   |   Byte 1   |   Byte 0   |
   |____________|____________|____________|____________|


   Byte 0 = Localization of abort source
   -------------------------------------

       B3  B2  B1  B0  
   0x              00  -- Abort by remote provider
   0x              01  -- Abort by local provider


   Byte 1 = Abort source
   ---------------------

       B3  B2  B1  B0  
   0x          06      -- Abort by presentation provider
                          (protocol machine basic event number RC0400_PpmEvent)


   Byte 2 = Abort reason
   ---------------------

       B3  B2  B1  B0  
   0x      00          -- Reason not specifed
   0x      01          -- Unrecognized PPDU
   0x      02          -- Unexpected PPDU
   0x      03          -- Unexpected session service primitive
   0x      04          -- Unrecognized PPDU parameter
   0x      05          -- Unexpected PPDU parameter
   0x      06          -- Invalid PPDU parameter value

                          Aborted by the local provider,
                          after RC080H_STRIGGERABORT
                          (After an OSS send function; after an OSS receive
                          function the connection is assumed as aborted and
                          the SPM returns with error code to the dispatcher.)
                          (remote: "Reason not specified"):
   0x      51          -- Session user-data too long
   0x      53          -- System call error within OSS 
                          (addAboInfo in p2: OSS addinfo)
   0x      54          -- CMX error
                          (addAboInfo in p2: OSS addinfo)
   0x      55          -- OSS error
                          (addAboInfo in p2: OSS error value, see "oss.h")

                          Aborted by the local provider,
                          other values
                          (remote: "Reason not specified"):
   0x      60          -- Wrong presentation-context-definition-list
                          (not provided at the rt_attach call)
   0x      61          -- Wrong default abstract syntax                
                          (not provided at the rt_attach call)
   0x      62          -- Wrong default transfer syntax                
                          (not provided at the rt_attach call)
   0x      63          -- Wrong presentation-context-definition-result-list
                          (invalid length or value)
   0x      64          -- Wrong abstract syntax identifier (PDV encoding)
   0x      65          -- Encoding error
*/

/* Some local abort reasons (description see above) */
#define RC040I_WrongAbstrSynId    0x60
#define RC040J_WrongDefAbstrSynId 0x61
#define RC040K_WrongDefTransSynId 0x62
#define RC040L_WrongSynResult     0x63
#define RC040M_WrongAbstrSynIdPdv 0x64
#define RC040N_EncodingError      0x65


/* Byte 3 = Event identifier
   -------------------------

       B3  B2  B1  B0  
   0x  00              -- CP PPDU
   0x  01              -- CPA PPDU
   0x  02              -- CPR PPDU
   0x  03              -- ARU PPDU
   0x  04              -- ARP PPDU
   0x  07              -- TD PPDU
   0x  0e              -- S-RELEASE indication
   0x  0f              -- S-RELEASE confirm
   0x  11              -- S-TOKEN-PLEASE indication
   0x  12              -- S-CONTROL-GIVE indication
   0x  13              -- S-SYNC-MINOR indication
   0x  14              -- S-SYNC-MINOR confirm
   0x  17              -- S-P-EXCEPTION-REPORT indication
   0x  18              -- S-U-EXCEPTION-REPORT indication
   0x  19              -- S-ACTIVITY-START indication
   0x  1a              -- S-ACTIVITY-RESUME indication
   0x  1b              -- S-ACTIVITY-INTERRUPT indication
   0x  1c              -- S-ACTIVITY-INTERRUPT confirm
   0x  1d              -- S-ACTIVITY-DISCARD indication
   0x  1e              -- S-ACTIVITY-DISCARD confirm
   0x  1f              -- S-ACTIVITY-END indication
   0x  20              -- S-ACTIVITY-END confirm

                          Local values:
   0x  30              -- S-CONNECT confirm negative
   0x  31              -- S-SYNCPOINT
                          (see definitions below)

   0x  40              -- Indicates that a send event (got from PS-user)
                          caused the problem
                          (see definition below)

   0x  80              -- Indicates that the value of p2 (addAboInfo) is
                          negative in reality (necessary because p2 is of
                          type "unsigned long int"), overwrites 0x40
                          (see definition below)

   0x  a0              -- Indicates that the received ARP PPDU contains
                          no event identifier value (the absence may be
                          legal or not)
                          (see definition below)
*/

/* Special event identifier values (description see above) */
#define RC040S_SConCnfNeg         0x30
#define RC040T_SSyncPoint         0x31

#define RC040U_SendEvent          0x40
#define RC040V_NegAddAboInfo      0x80
#define RC040W_NoEventIdReceived  0xa0


/* Example
   -------

   0x05010600 : Abort by remote presentation provider,
                unexpected parameter in CPA PPDU

 * ****************** End of provider abort reason values (p4) ****** *
 * ******************                                          ****** */

/* ****************** Additional abort info values (p2) ************* *
 * ******************                                   ************* *

   If abort reason "System call error within OSS" or "CMX error":
       OSS addinfo (see "oss.h")

   If abort reason "OSS error":
       OSS error value (see "oss.h")

   Otherwise: Not used


   Note: If byte 3 of p4 (see above) has the value 0x80, the value of p2
         is negative in reality (necessary because p2 is of type "unsigned
         long int").

 * ****************** End of additional abort info values (p2) ****** *
 * ******************                                          ****** */

/* ****************** Event-Parameter-Matrix ************************ *
 * ******************                        ************************ *

|------------------|------|----------|----------|----------|----------|
|event             |IN/OUT|p1        |p2        |p3        |p4        |
|------------------|------|----------|----------|----------|----------|
|PCONNECTreq       |IN    |udata     |NULL      |rTro6e    |NULL      |
|PCONNECTind       |acse  |udata     |NULL      |rTro6f    |NULL      |
|PCONNECTrsp       |IN    |udata     |NULL      |rTro6g    |conResult |
|PCONNECTcnf       |acse  |udata     |NULL      |rTro6h    |conResult |
|------------------|------|----------|----------|----------|----------|
|PRELEASEreq       |IN    |udata     |NULL      |NULL      |NULL      |
|PRELEASEind       |acse  |udata     |NULL      |NULL      |NULL      |
|PRELEASErsp       |IN    |udata     |NULL      |NULL      |relResult |
|PRELEASEcnf       |acse  |udata     |NULL      |NULL      |relResult |
|------------------|------|----------|----------|----------|----------|
|PUABORTreq        |IN    |udata     |NULL      |NULL      |NULL      |
|PUABORTind        |acse  |udata     |NULL      |NULL      |transSynId|
|------------------|------|----------|----------|----------|----------|
|PPABORTind        |acse  ||NULL     |addAboInfo|NULL      |aboReason |
|------------------|------|----------|----------|----------|----------|
|PDATAreq          |IN    |udata     |NULL      |NULL      |NULL      |
|PDATAind          |lctrl |udata     |NULL      |NULL      |NULL      |
|------------------|------|----------|----------|----------|----------|
|PTOKEN_PLEASEreq  |IN    |udata     |NULL      |NULL      |tokens   *|
|PTOKEN_PLEASEind  |lctrl |udata     |NULL      |NULL      |tokens   *|
|------------------|------|----------|----------|----------|----------|
|PCONTROL_GIVEreq  |IN    |udata     |NULL      |NULL      |NULL      |
|PCONTROL_GIVEind  |lctrl |udata     |NULL      |NULL      |NULL      |
|------------------|------|----------|----------|----------|----------|
|PSYNC_MINORreq    |IN    |udata     |syncType *|NULL      |userEvent*|
|PSYNC_MINORind    |lctrl |udata     |syncType *|NULL      |syncPoint*|
|PSYNC_MINORrsp    |IN    |udata     |NULL      |NULL      |syncPoint*|
|PSYNC_MINORcnf    |lctrl |udata     |NULL      |NULL      |syncPoint*|
|------------------|------|----------|----------|----------|----------|
|PUEXCEPT_REPORTreq|IN    |udata     |NULL      |NULL      |excReason*|
|PUEXCEPT_REPORTind|lctrl |udata     |NULL      |NULL      |excReason*|
|------------------|------|----------|----------|----------|----------|
|PPEXCEPT_REPORTind|lctrl |NULL      |NULL      |NULL      |excReason*|
|------------------|------|----------|----------|----------|----------|
|PACT_STARTreq     |IN    |udata     |NULL      |rTro51   *|NULL      |
|PACT_STARTind     |lctrl |udata     |NULL      |rTro51   *|NULL      |
|------------------|------|----------|----------|----------|----------|
|PACT_RESUMEreq    |IN    |udata     |NULL      |rTro5i   *|NULL      |
|PACT_RESUMEind    |lctrl |udata     |NULL      |rTro5i   *|NULL      |
|------------------|------|----------|----------|----------|----------|
|PACT_ENDreq       |IN    |udata     |NULL      |NULL      |userEvent*|
|PACT_ENDind       |lctrl |udata     |NULL      |NULL      |syncPoint*|
|PACT_ENDrsp       |IN    |udata     |NULL      |NULL      |NULL      |
|PACT_ENDcnf       |lctrl |udata     |NULL      |NULL      |NULL      |
|------------------|------|----------|----------|----------|----------|                   
|PACT_INTERRUPTreq |IN    |udata     |NULL      |NULL      |intReason*|
|PACT_INTERRUPTind |lctrl |udata     |NULL      |NULL      |intReason*|
|PACT_INTERRUPTrsp |IN    |udata     |NULL      |NULL      |NULL      |
|PACT_INTERRUPTcnf |lctrl |udata     |NULL      |NULL      |NULL      |
|------------------|------|----------|----------|----------|----------|                   
|PACT_DISCARDreq   |IN    |udata     |NULL      |NULL      |disReason*|
|PACT_DISCARDind   |lctrl |udata     |NULL      |NULL      |disReason*|
|PACT_DISCARDrsp   |IN    |udata     |NULL      |NULL      |NULL      |
|PACT_DISCARDcnf   |lctrl |udata     |NULL      |NULL      |NULL      |
|------------------|------|----------|----------|----------|----------|                   
|PCONTINUE         |ppm   |NULL      |NULL      |NULL	   |NULL      |
|PSYNCPOINT        |ppm   |NULL      |userEvent*|NULL      |syncPoint*|
|------------------|------|----------|----------|----------|----------|                   
|All other         |IN    |NOTDEFINED|NOTDEFINED|NOTDEFINED|NOTDEFINED|
|values            |OUT   |NOTDONE   |NOTDONE   |NOTDONE   |NOTDONE   |
|------------------|------|----------|----------|----------|----------|


IN         : It is an incoming event (received by the PPM).
OUT        : It is an outgoing event  (sent by the PPM).
NOTDEFINED : This parameter contains a not defined value.
NOTDONE    : Such an event is not issued by the PPM.
NULL       : Parameter not used. Must have the value NULL.
acse       : This event is posted to the ACPM          (see rodpat0.h).
lctrl      : This event is posted to the Lower Control Function of
             the Application Manager                   (see rodpat0.h).
*          : This direct mapped parameter is passed transparently
             from/to Lower Control Function to/from SPM.
udata      : Global heap memory handle of PDV tree root node
             (user-data; NULL means no user-data)      (see rouply0.h).
conResult  : Connect result                            (see above).
relResult  : Release result                            (see rouply0.h).
transSynId : Transfer syntax identifier of abort products user data (if there 
             are products user data, otherwise the identifier shall be ignored).
             Really necessary only if PUABORTind before PCONNECTcnf
             or PCONNECTrsp but filled always.
aboReason  : Provider abort reason                     (see above).
addAboInfo : Additional abort information              (see above).
tokens     : Required tokens                           (see rouply0.h).
syncPoint  : Synchronization point serial number.
syncType   : Synchronization point type                (see rouply0.h).
excReason  : Exception report reason                   (see rouply0.h).
intReason  : Activity Interrupt reason                 (see rouply0.h).
disReason  : Activity Discard reason                   (see rouply0.h).
userEvent  : User specific event value.
rTro6e     : Local heap memory handle to P-CONNECT request direct
             mapped parameters                         (see rouply1.h).
rTro6f     : Local heap memory handle to P-CONNECT indication direct
             mapped paramters                          (see rouply1.h).
rTro6g     : Local heap memory handle to P-CONNECT response direct
             mapped paramters                          (see rouply1.h).
rTro6h     : Local heap memory handle to P-CONNECT confirm direct
             mapped paramters                          (see rouply1.h).
rTro51     : Local heap memory handle to P-ACTIVITY-START request and
             P-ACTIVITY-START indication direct mapped paramters
                                                       (see rouply0.h).
rTro5i     : Local heap memory handle to P-ACTIVITY-RESUME request and
             P-ACTIVITY-RESUME indication direct mapped paramters
                                                       (see rouply1.h).


Note the historically developed generic parameter use:
  p1:  User-data.
  p2:  Additional value "unsigned long int" for direct mapped parameter(s).
  p3:  Local memory handle to direct mapped parameter(s).
       If incoming event: Is to be locked if the value is not NULL.
       (Used if the direct mapped parameter(s) exceed(s) the range of
       two "unsigned long int" values.
  p4:  Value "unsigned long int" for direct mapped parameter(s).

 * ****************** End of Event-Parameter-Matrix ***************** *
 * ******************                               ***************** */

#endif	/* _ROPPM00_H */
