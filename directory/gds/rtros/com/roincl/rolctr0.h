/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rolctr0.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:48  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:53  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:20  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:30  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:49  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:38  keutel
 * 	creation
 * 	[1994/03/21  13:19:02  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROLCTR0_H
#define _ROLCTR0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rolctr0_rcsid[] = { "@(#) $RCSfile: rolctr0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:43 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1989 BY SIEMENS AG                              */
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
/* NAME 	: rolcrt0.h					      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-26					      */
/*                                                                    */
/* COMPONENT	: RORTS  lower control protocol                       */
/*		  machine					      */
/*								      */
/* PRD#/VERS.	: RORTS-V3.00					      */
/*                                                                    */
/* DESCRIPTION	: Protocol machine eventing interface		      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rolctr0.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 3.00 K0  | 90-12-31|  original		       |jm  |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/
/*
|----------------|------|------------|-------------|------------|----------|
|event	         |IN/OUT|p1	     |p2	   |p3	        | p4       |
|----------------|------|------------|-------------|------------|----------|
|AA_ASSOCIATEreq |IN/OUT|pdv  |noPdv |NULL         |rTro7e      |mapinginfo|
|AA_ASSOCIATEind |IN/OUT|pdv  |noPdv |NULL/Pid     |rTro7f      |assocDiag |
|AA_ASSOCIATErsp |IN/OUT|pdv  |noPdv |NULL         |rTro7g      |assocRest |
|AA_ASSOCIATEcnf |IN/OUT|pdv  |noPdv |NULL         |rTro7h      |assocRes  |
|----------------|------|------------|-------------|------------|----------|
|AA_RELEASEreq   |IN/OUT|pdv  |noPdv |closeReason  |NULL        |NULL      |
|AA_RELEASEind   |IN/OUT|pdv  |noPdv |closeReason  |NULL        |NULL      |
|AA_RELEASErsp   |IN/OUT|pdv  |noPdv |closeReason  |NULL        |closeRest |
|AA_RELEASEcnf   |IN/OUT|pdv  |noPdv |closeReason  |NULL        |closeRest |
|----------------|------|------------|-------------|------------|----------|
|AA_ABORTreq     |IN/OUT|pdv  |noPdv |NULL         |NULL        |NULL      |
|AA_ABORTind     |IN/OUT|pdv  |noPdv |NULL         |NULL        |abortResn |
|----------------|------|------------|-------------|------------|----------|
|AA_PABORTind    |IN/OUT|NULL        |addAbortInfo |NULL        |abortResn |
|----------------|------|------------|-------------|------------|----------|
|AA_EXTERN_1     |IN/OUT|NULL        |NULL         |NULL        |NULL      |
|AA_EXTERN_2     |IN/OUT|NULL        |NULL         |NULL        |NULL      |
|----------------|------|------------|-------------|------------|----------|
|PCONNECTreq     |IN/OUT|udata       |NULL         |rTro6e      |NULL      |
|PCONNECTind     |IN/OUT|udata       |NULL         |rTro6f      |NULL      |
|PCONNECTrsp     |IN/OUT|udata       |NULL         |rTro6g      |conResult |
|PCONNECTcnf     |IN/OUT|udata       |NULL         |rTro6h      |conResult |
|----------------|------|------------|-------------|------------|----------|
|PRELEASEreq     |IN/OUT|udata       |NULL         |NULL        |NULL      |
|PRELEASEind     |IN/OUT|udata       |NULL         |NULL        |NULL      |
|PRELEASErsp     |IN/OUT|udata       |NULL         |NULL        |relResult |
|PRELEASEcnf     |IN/OUT|udata       |NULL         |NULL        |relResult |
|----------------|------|------------|-------------|------------|----------|
|PUABORTreq      |IN/OUT|udata       |NULL         |NULL        |NULL      |
|PUABORTind      |IN/OUT|udata       |NULL         |NULL        |transSynId|
|----------------|------|------------|-------------|------------|----------|
|PPABORTind      |IN/OUT||NULL       |addAboInfo   |NULL        |aboReason |
|----------------|------|------------|-------------|------------|----------|
|PDATAreq        |IN/OUT|udata       |NULL         |NULL        |NULL      |
|PDATAind        |IN/OUT|udata       |NULL         |NULL        |NULL      |
|----------------|------|------------|-------------|------------|----------|
|PTOKEN_PLEASEreq|IN/OUT|udata       |NULL         |NULL        |tokens   *|
|PTOKEN_PLEASEind|IN/OUT|udata       |NULL         |NULL        |tokens   *|
|----------------|------|------------|-------------|------------|----------|
|PCONTROL_GIVEreq|IN/OUT|udata       |NULL         |NULL        |NULL      |
|PCONTROL_GIVEind|IN/OUT|udata       |NULL         |NULL        |NULL      |
|----------------|------|------------|-------------|------------|----------|
|PSYNC_MINORreq  |IN/OUT|udata       |syncType *   |NULL        |userEvent*|
|PSYNC_MINORind  |IN/OUT|udata       |syncType *   |NULL        |syncPoint*|
|PSYNC_MINORrsp  |IN/OUT|udata       |NULL         |NULL        |syncPoint*|
|PSYNC_MINORcnf  |IN/OUT|udata       |NULL         |NULL        |syncPoint*|
|----------------|------|------------|-------------|------------|----------|
|PUEXCEPT_REPORTreq|IN/OUT|udata     |NULL         |NULL        |excReason*|
|PUEXCEPT_REPORTind|IN/OUT|udata     |NULL         |NULL        |excReason*|
|------------------|------|----------|-------------|------------|----------|
|PPEXCEPT_REPORTind|IN/OUT|NULL      |NULL         |NULL        |excReason*|
|----------------|------|------------|-------------|------------|----------|
|PACT_STARTreq   |IN/OUT|udata       |NULL         |rTro51   *  |NULL      |
|PACT_STARTind   |IN/OUT|udata       |NULL         |rTro51   *  |NULL      |
|----------------|------|------------|-------------|------------|----------|
|PACT_RESUMEreq  |IN/OUT|udata       |NULL         |rTro5i   *  |NULL      |
|PACT_RESUMEind  |IN/OUT|udata       |NULL         |rTro5i   *  |NULL      |
|----------------|------|------------|-------------|------------|----------|
|PACT_ENDreq     |IN/OUT|udata       |NULL         |NULL        |userEvent*|
|PACT_ENDind     |IN/OUT|udata       |NULL         |NULL        |syncPoint*|
|PACT_ENDrsp     |IN/OUT|udata       |NULL         |NULL        |NULL      |
|PACT_ENDcnf     |IN/OUTludata       |NULL         |NULL        |NULL      |
|----------------|------|------------|-------------|------------|----------|  
|PACT_INTERRUPTreq|IN/OUT |udata     |NULL         |NULL        |intReason*|
|PACT_INTERRUPTind|IN/OUT |udata     |NULL         |NULL        |intReason*|
|PACT_INTERRUPTrsp|IN/OUT |udata     |NULL         |NULL        |NULL      |
|PACT_INTERRUPTcnf|IN/OUT |udata     |NULL         |NULL        |NULL      |
|----------------|------|------------|-------------|------------|----------|  
|PACT_DISCARDreq |IN/OUT|udata       |NULL         |NULL        |disReason*|
|PACT_DISCARDind |IN/OUT|udata       |NULL         |NULL        |disReason*|
|PACT_DISCARDrsp |IN/OUT|udata       |NULL         |NULL        |NULL      |
|PACT_DISCARDcnf |IN/OUT|udata       |NULL         |NULL        |NULL      |
|----------------|------|------------|-------------|------------|----------| 
|PCONTINUE       |IN/OUT|NULL        |NULL         |NULL	|NULL      |
|PSYNCPOINT      |IN/OUT|NULL        |userEvent*   |NULL        |syncPoint*|
|----------------|------|------------|-------------|------------|----------|
|All other       |IN/OUT|NOTDEFINED  |NOTDEFINED   |NOTDEFINED  |NOTDEFINED|
|values          |      |NOTDONE     |NOTDONE      |NOTDONE     |NOTDONE   |
|----------------|------|------------|-------------|------------|----------|


IN	   : It is an incomming event (received by the Upper Control).
OUT	   : It is an outgoing event  (sent by the Upper Control).
NOTDEFINED : This parameter contains a not defined value.
NOTDONE    : Such an event is not issued by the ro-pm.
NULL	   : Parameter not used.

rUser	   : This event is posted to the user of ro-pm service events
	     i.e. Externel Interface of ROS.

noUdata    : No user data for this event  (argument, result, error)
	     (see rouply0.h).

Pdv-tree   : Global heap memory handle of pdv-tree
	     root node (argument, result, error)   (see rouply0.h).

Edata      : Global heap memory handle of local_event_data
	     which is being passed to External interface of ROS.

noBuff	   : No event buffer for this event	   (see roulpy0.h).
evBuffer   : Global heap memory handle of an
	     event buffer			   (see roulpy0.h).
aboReason  : Provider abort reason		   (see rouply0.h).
adAboInfo  : Additional abort information	   (see rouply0.h).
asocReason : Association request/indication reason (see rouply0.h).
asocResRea : Association response/confirmation
	     reason and result			   (see rouply0.h).
rT0600	   : Local heap memory handle to A-Associate-request direct
	     mapped parameter (see above).
rT0601	   : Local heap memory handle to A-Associate-indication direct
	     mapped paramter  (see above).
rT0602	   : Local heap memory handle to A-Associate-response and
	     A-Associate-confirmation direct mapped parameter (see above).

clasMap    : Association class (low byte) Mapping information ([second
	     byte == 0] then acpm\ppm else rtpm)

 * ****************** End of event parameter matrix ***************** *
 * ******************				    ***************** */

#endif	/* _ROLCTR0_H */
