/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm00.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:10  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:56  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:36:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:07  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:57  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:40  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:34  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:27  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:40  keutel
 * 	creation
 * 	[1994/03/21  13:27:58  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROSPM00_H
#define _ROSPM00_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rospm00_rcsid[] = { "@(#) $RCSfile: rospm00.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:10 $" } ;
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
/* NAME         : rospm00.h                                           */
/*                                                                    */
/* AUTHOR	: Milind M. Aphale    		                      */
/* DATE 	: 93-08-03					      */
/*                                                                    */
/* COMPONENT    : RTROS session protocol machine                      */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : Protocol machine service interface                  */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#) rospm00.h 1.1 93/11/29 RTROS-V3.00              */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/*           |        |                                |    |         */
/* 3.00  K00 |93-08-03|  original                      |ma  |         */
/*           |        |                                |    |         */
/*           |        |                                |    |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/* ****************** Protocol machine basic event number *********** */
/* ******************                                     *********** */

#define RC0800_SpmEvent           0x0500

/* ****************** Session service events ************************ */
/* ******************                        ************************ */

#define RC0801_SCONNECTreq	  0x0504   /* Connect events	      */
#define RC0802_SCONNECTind	  0x0501
#define RC0803_SCONNECTrsp	  0x0505
#define RC0804_SCONNECTcnf	  0x0503

#define RC0805_SRELEASEreq	  0x050c   /* Release events	      */
#define RC0806_SRELEASEind	  0x0508
#define RC0807_SRELEASErsp	  0x050d
#define RC0808_SRELEASEcnf	  0x050a

#define RC0809_SUABORTreq	  0x0514   /* User abort events       */
#define RC080a_SUABORTind	  0x050e

#define RC080b_SPABORTind	  0x0510   /* Provider abort event    */

#define RC080c_SDATAreq 	  0x0515   /* Normal data events      */
#define RC080d_SDATAind 	  0x0512

#define RC080e_STOKEN_PLEASEreq   0x0520   /* Token request events    */
#define RC080f_STOKEN_PLEASEind   0x0540

#define RC080g_SCONTROL_GIVEreq   0x0521   /* Token give events       */
#define RC080h_SCONTROL_GIVEind   0x0541

#define RC080i_SSYNC_MINORreq	  0x0522   /* Minor Synchronization   */
#define RC080j_SSYNC_MINORind	  0x0542   /* events		      */
#define RC080k_SSYNC_MINORrsp	  0x0523
#define RC080l_SSYNC_MINORcnf	  0x0543

#define RC080m_SUEXCEPT_REPORTreq 0x0524   /* User exception report   */
#define RC080n_SUEXCEPT_REPORTind 0x0544   /* events		      */

#define RC080o_SPEXCEPT_REPORTind 0x054d   /* Provider exception      */

#define RC080p_SACT_STARTreq	  0x0525   /* Activity Start events   */
#define RC080q_SACT_STARTind	  0x0545

#define RC080r_SACT_RESUMEreq	  0x0526   /* Activity Resume events  */
#define RC080s_SACT_RESUMEind	  0x0546

#define RC080t_SACT_ENDreq	  0x0527   /* Activity End events     */
#define RC080u_SACT_ENDind	  0x0547
#define RC080v_SACT_ENDrsp	  0x0528
#define RC080w_SACT_ENDcnf	  0x0548

#define RC080x_SACT_INTERRUPTreq  0x0529   /* Activity Interrupt      */
#define RC080y_SACT_INTERRUPTind  0x0549   /* events		      */
#define RC080z_SACT_INTERRUPTrsp  0x052a
#define RC080A_SACT_INTERRUPTcnf  0x054a

#define RC080B_SACT_DISCARDreq	  0x052b   /* Activity Discard	      */
#define RC080C_SACT_DISCARDind	  0x054b   /* events		      */
#define RC080D_SACT_DISCARDrsp	  0x052c
#define RC080E_SACT_DISCARDcnf	  0x054c

#define RC080F_SCONTINUE	  0x054e   /* Continue		      */
#define RC080G_SSYNCPOINT	  0x054f   /* Syncronization point    */
#define RC080H_STRIGGERABORT	  0x0550   /* Trigger Abort	      */


/* ************   Abort reasons for RC080b_SPABORTind	************* */
/* ************ 					************* */

#define RC0311_TransportDisconnect     0x00010500L
#define RC0312_ProtocolError	       0x00040500L
#define RC0314_UndefinedReason	       0x00080500L
#define RC0313_RestrictionInPICS       0x00100500L

#define RC0322_ConrqRetryReturn        0x02000501L


/* ************  Abort reasons for RC080H_STRIGGERABORT  ************ */
/* ************  and also used by RC080b_SPABORTind	 ************ */

#define RC0321_UdataTooLong	       0x01000501L
#define RC0323_CmxError 	       0x03000501L
#define RC0324_SysOssError	       0x04000501L
#define RC0325_OssError 	       0x05000501L


/* *** Description of the abort reasons and additional information *** *
 * *** (value "aboReason", returned in P4)			   *** *


      +---------+---------+---------+---------+
Byte  |    3	|    2	  |    1    |	 0    |
      +---------+---------+---------+---------+

3 :    Local abort reason

2 :    if (Value | 0x01) == TRUE  : P2 = -P2;
			 == FALSE : P2 =  P2;

       (The lowest bit of the third byte indicates if the value
	returned in P2 (addAboInf) is positive or negative)

1 :    Value == 05 : Protocol machine's basic event number

0 :    Value == 00 : Abort reason given by OSS (s_paboin function)
       Value == 01 : Local abort reason

 * ****************************************************************** */


/* ****************** Event-Parameter-Matrix ************************ *
 * ******************                        ************************ *

|------------------|------|----------|----------|----------|----------|
|event             |IN/OUT|p1        |p2        |p3        |p4        |
|------------------|------|----------|----------|----------|----------|
|SCONNECTreq       |IN    |udata     |offset    |rTro5e    |NULL      |
|SCONNECTind       |ppm   |udata     |offset    |rTro5f    |NULL/Pid  |
|SCONNECTrsp       |IN    |udata     |offset    |rTro5g    |conResult |
|SCONNECTcnf       |ppm   |udata     |offset    |rTro5g    |conResult |
|------------------|------|----------|----------|----------|----------|
|SRELEASEreq       |IN    |udata     |offset    |NULL      |NULL      |
|SRELEASEind       |ppm   |udata     |offset    |NULL      |NULL      |
|SRELEASErsp       |IN    |udata     |offset    |NULL      |relResult |
|SRELEASEcnf       |ppm   |udata     |offset    |NULL      |relResult |
|------------------|------|----------|----------|----------|----------|
|SUABORTreq        |IN    |udata     |offset    |NULL      |NULL      |
|SUABORTind        |ppm   |udata     |NULL      |NULL      |NULL      |
|------------------|------|----------|----------|----------|----------|
|SPABORTind	   |ppm   |NULL      |addAboInfo|NULL	   |aboReason |
|------------------|------|----------|----------|----------|----------|
|SDATAreq          |IN    |udata     |offset    |NULL      |NULL      |
|SDATAind          |ppm   |udata     |offset    |NULL      |NULL      |
|------------------|------|----------|----------|----------|----------|
|STOKEN_PLEASEreq  |IN	  |udata     |NULL	|NULL	   |tokens    |
|STOKEN_PLEASEind  |ppm   |udata     |NULL	|NULL	   |tokens    |
|------------------|------|----------|----------|----------|----------|
|SCONTROL_GIVEreq  |IN	  |udata     |NULL	|NULL	   |NULL      |
|SCONTROL_GIVEind  |ppm   |udata     |NULL	|NULL	   |NULL      |
|------------------|------|----------|----------|----------|----------|
|SSYNC_MINORreq    |IN	  |udata     |syncType	|NULL	   |userEvent |
|SSYNC_MINORind    |ppm   |udata     |syncType	|NULL	   |syncPoint |
|SSYNC_MINORrsp    |IN	  |udata     |NULL	|NULL	   |syncPoint |
|SSYNC_MINORcnf    |ppm   |udata     |NULL	|NULL	   |syncPoint |
|------------------|------|----------|----------|----------|----------|
|SUEXCEPT_REPORTreq|IN	  |udata     |NULL	|NULL	   |excReason |
|SUEXCEPT_REPORTind|ppm   |udata     |NULL	|NULL	   |excReason |
|------------------|------|----------|----------|----------|----------|
|SPEXCEPT_REPORTind|ppm   |NULL      |NULL	|NULL	   |excReason |
|------------------|------|----------|----------|----------|----------|
|SACT_STARTreq	   |IN	  |udata     |NULL	|rTro51    |NULL      |
|SACT_STARTind	   |ppm   |udata     |NULL	|rTro51    |NULL      |
|------------------|------|----------|----------|----------|----------|
|SACT_RESUMEreq    |IN	  |udata     |NULL	|rTro5i    |NULL      |
|SACT_RESUMEind    |ppm   |udata     |NULL	|rTro5i    |NULL      |
|------------------|------|----------|----------|----------|----------|
|SACT_ENDreq	   |IN	  |udata     |NULL	|NULL	   |userEvent |
|SACT_ENDind	   |ppm   |udata     |NULL	|NULL	   |syncPoint |
|SACT_ENDrsp       |IN    |udata     |NULL      |NULL      |NULL      |
|SACT_ENDcnf       |ppm   |udata     |NULL      |NULL      |NULL      |
|------------------|------|----------|----------|----------|----------|       
|SACT_INTERRUPTreq |IN	  |udata     |NULL	|NULL	   |intReason |
|SACT_INTERRUPTind |ppm   |udata     |NULL	|NULL	   |intReason |
|SACT_INTERRUPTrsp |IN	  |udata     |NULL	|NULL	   |NULL      |
|SACT_INTERRUPTcnf |ppm   |udata     |NULL	|NULL	   |NULL      |
|------------------|------|----------|----------|----------|----------|       
|SACT_DISCARDreq   |IN	  |udata     |NULL	|NULL	   |disReason |
|SACT_DISCARDind   |ppm   |udata     |NULL	|NULL	   |disReason |
|SACT_DISCARDrsp   |IN	  |udata     |NULL	|NULL	   |NULL      |
|SACT_DISCARDcnf   |ppm   |udata     |NULL	|NULL	   |NULL      |
|------------------|------|----------|----------|----------|----------|
|SCONTINUE	   |ppm   |NULL      |NULL	|NULL	   |NULL      |
|SSYNCPOINT	   |ppm   |NULL      |userEvent |NULL	   |SyncPoint |
|STRIGGERABORT	   |ppm   |NULL      |addAboInf |NULL	   |aboReason |
|------------------|------|----------|----------|----------|----------|        
|All other         |IN    |NOTDEFINED|NOTDEFINED|NOTDEFINED|NOTDEFINED|
|values            |OUT   |NOTDONE   |NOTDONE   |NOTDONE   |NOTDONE   |
|------------------|------|----------|----------|----------|----------|


IN         : It is an incoming event (received by the SPM).
OUT        : It is an outgoing event  (sent by the SPM).
NOTDEFINED : This parameter contains a not defined value.
NOTDONE    : Such an event is not issued by the SPM.
NULL       : Parameter not used. Must have the value NULL.
ppm        : This event is posted to the PPM           (see roapma0.h).
udata      : Store handle
             (user-data; NULL means no user-data)      (see rouply0.h).
conResult  : Connect result                            (see rouply0.h).
relResult  : Release result                            (see rouply0.h).
offset     : Offset in store from where encoding/decoding should begin.
aboReason  : Provider abort reason                     (see above).
addAboInfo : Additional abort information              (see above).
tokens     : Required tokens                           (see rouply0.h).
syncPoint  : Synchronization point serial number.
syncType   : Synchronization point type                (see rouply0.h).
excReason  : Exception report reason                   (see rouply0.h).
intReason  : Activity Interrupt reason                 (see rouply0.h).
disReason  : Activity Discard reason		       (see rouply0.h).
userEvent  : User specific event value.
rTro5e     : Local heap memory handle to S-CONNECT request direct
             mapped parameters                         (see rouply1.h).
rTro5f     : Local heap memory handle to S-CONNECT indication direct
             mapped paramters                          (see rouply1.h).
rTro5g     : Local heap memory handle to S-CONNECT response and
             S-CONNECT confirm direct mapped parameters
                                                       (see rouply1.h).
rTro51     : Local heap memory handle to S-ACTIVITY-START request and
             S-ACTIVITY-START indication direct mapped paramters
                                                       (see rouply0.h).
rTro5i     : Local heap memory handle to S-ACTIVITY-RESUME request and
             S-ACTIVITY-RESUME indication direct mapped paramters
                                                       (see rouply1.h).


Note the historically developed generic parameter use:
  p1:  User-data.
  p2:  Additional value "unsigned long int" for direct mapped parameter(s).
       (Used for receive buffer description in other protocol machines.)
  p3:  Local memory handle to direct mapped parameter(s).
       If incoming event: Is to be locked if the value is not NULL.
       (Used if the direct mapped parameter(s) exceed(s) the range of
       two "unsigned long int" values.
  p4:  Value "unsigned long int" for direct mapped parameter(s).

 * ****************** End of Event-Parameter-Matrix ***************** *
 * ******************                               ***************** */

#endif	/* _ROSPM00_H */
