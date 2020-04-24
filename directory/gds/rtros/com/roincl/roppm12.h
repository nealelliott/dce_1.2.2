/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm12.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:51  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:01  root]
 * 
 * Revision 1.1.2.4  1994/08/22  15:21:35  keutel
 * 	OT 11620
 * 	[1994/08/19  13:47:13  keutel]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:01  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:32  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:38  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:21  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:51  keutel
 * 	creation
 * 	[1994/03/21  13:19:14  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPM12_H
#define _ROPPM12_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppm12_rcsid[] = { "@(#) $RCSfile: roppm12.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:52 $" } ;
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
/* NAME         : roppm12.h                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : PPM intern header                                   */
/*                Function prototyping                                */
/*                (without pres. context handling during decoding)    */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roppm12.h	1.2 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-03-24|  adaptation of ROS V2.5A00 K05 |hm  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/* ****************************************************************** */
/*                                                                    */
/*                    ROPPM02.C functions                             */
/*                                                                    */
/* ****************************************************************** */

/* ****************** Search a transfer syntax identifier *********** */
/* ******************                                     *********** */

signed short int PASCAL ro0425_getTransId PROTO ((
   rT0441_stEntContext    *pEntContext,
   signed         int      iAbsId,
   unsigned long  int FAR *oTransSynName));

/* Return value RC043C_NoTransId : Transfer syntax not found.
                otherwise        : Transfer syntax identifier.
                

   - pEntContext    : --> Pres. entity invocation context (pointer to).
   - iAbsId         : --> Abstract syntax identifier.
   - oTransSynName  : --> Transfer syntax object identifier (pointer to).
*/
/* ****************************************************************** */
/* ****************** Search an Abstract syntax identifier *********** */
/* ******************                                     *********** */

signed short int PASCAL ro0426_getAbsId PROTO ((
   signed   short int      sInformType,
   rT04z1_stAbsNameId FAR *pstAbsNameId,
   unsigned long  int      hContext ));

/* ****************************************************************** */
/* ************************* Get DCS ********************************* */
/* ******************                                     *********** */

signed short int PASCAL ro0427_getDcs PROTO ((
rT04z0_stDCS           FAR *pstDCS,
unsigned long int           hContext,
rT04z0_stDCS           FAR *pRequestedInfo));

/* ************************* Find AbsId******************************* */
/* ******************                                     *********** */
signed short int PASCAL ro0428_findAbsId PROTO ((
                                            rT04z2_stAbsIdFind FAR *pstAbsFnd));


/* ************************* Get  Mode******************************* */
/* ******************                                     *********** */


signed short int NEAR PASCAL ro0429_getMode PROTO (( 
unsigned long int        hContext,
rT04z3_stMode     FAR *  pMode ));


/* ****************************************************************** */
/*                                                                    */
/*                    ROPPM03.C functions                             */
/*                                                                    */
/*                    ROPPM03.C functions                             */
/*                                                                    */
/* ****************************************************************** */

/* ****************** PPM encoding/decoding functions (PPCI) ******** */
/* ******************                                        ******** */

/* ------------------ Encoding function ----------------------------- */

signed short int PASCAL ro0430_encodPpm PROTO ((
   unsigned       int        uOutEvent,
   unsigned long  int        ulConnectResult,
   unsigned long  int FAR  * uloffset,
   rT0430_stContext   NEAR * pContext,
   unsigned long  int FAR  * pStore,
   unsigned long  int FAR  * phNode));

/* Return value == 0: OK.
                <  0: Unrecoverable error (see roerval.h).
                >  0: Presentation provider abort reason (see rouply0.h).

   - uOutEvent      : --> Outgoing session event (defines the PPDU type).
   - ulConnectResult: --> Connect result (p4 of S-CONNECT response).
   - uloffset       : <-- Offset of the store is returned.
   - pContext       : --> PPM context (pointer to).
   - pStore         : <-- Store handle (encoded byte string) (pointer to).
   - phNode         : <-> Memory handle to PDV node (pointer to).
*/


/* ------------------ Decoding function ----------------------------- */

signed short int PASCAL ro0431_decodPpm PROTO ((
   unsigned       int        uInEvent,
   rT0430_stContext   NEAR * pContext,
   unsigned long  int FAR  * pStore,
   unsigned long  int FAR  * phNode,
   unsigned long  int        ulOffset));

/* Return value == 0: OK.
                <  0: Unrecoverable error (see roerval.h).
                >  0: Presentation provider abort reason (see rouply0.h).

   - uInEvent       : --> Incoming session event, mapped (defines the PPDU type).
   - pContext       : --> PPM context (pointer to).
   - pStore         : <-> Store handle (encoded byte string) (pointer to).
   - phNode         : <-- Memory handle to PDV node (pointer to).
*/


/* ********** ClearPdvNode sub-function (normal-mode user-data) ***** */
/* **********                                                   ***** */

signed short int PASCAL ro0432_clearUserData PROTO ((
   rT0417_stUserData  FAR *pUserData));     

/* Return value == 0: OK.
                != 0: One or more error(s) occured.

   - pUserData          : <-> Struct describing the user-data (pointer to).
*/

/* ****************************************************************** */
/*                                                                    */
/*                    ROPPM04.C functions                             */
/*                                                                    */
/* ****************************************************************** */

/* ****************** Some functions called from the PM function **** */
/* ******************                                            **** */

/* ------------------ Process an incoming event from PS-user -------- */

signed short int PASCAL ro0440_eventFromUser PROTO ((
   rT0430_stContext       *pContext,
   unsigned       int FAR *pInEvent,
   unsigned long  int      ulP1,
   unsigned long  int      ulP4,
   signed         int FAR *pLockP3, 
   signed         int FAR *pDirectSpm, 
   unsigned long  int FAR *phNode,  
   rT041G_unPpdu FAR *FAR *ppNode));

/* Return value == 0: OK.
                <  0: Unrecoverable error (see roerval.h).

   - pContext       : --> PPM context (pointer to).
   - pInEvent       : --> Incoming event from PS-user, original (pointer to).
                      <-- Incoming event from PS-user, mapped (pointer to).
   - ulP1           : --> Event parameter P1 (PDV node handle). 
   - ulP4           : --> Event parameter P4 (ConnectResult or ReleaseResult).
   - pLockP3        : <-- TRUE: locking of handle P3 necessary (pointer to).
   - pDirectSpm     : <-- TRUE: event which has to be mapped directly to SPM
                          (pointer to).
   - phNode         : <-- Memory handle to pres. PDV node (pointer to).
   - ppNode         : <-- Pointer to pres. PDV node (pointer to).
*/


/* ------------------ Process an incoming event from Spm ------------ */

signed short int PASCAL ro0441_eventFromSpm PROTO ((
   rT0430_stContext       *pContext,
   unsigned       int FAR *pInEvent,
   unsigned long  int FAR *pP1,
   unsigned long  int      ulP2,
   unsigned long  int      ulP4,
   signed         int FAR *pLockP3, 
   signed         int FAR *pDirectSpm, 
   unsigned long  int FAR *pStore,  
   unsigned long  int FAR *phNode,  
   rT041G_unPpdu FAR *FAR *ppNode));

/* Return value == 0: OK.
                <  0: Unrecoverable error (see roerval.h).
                >  0: Presentation provider abort reason.

   - pContext       : --> PPM context (pointer to).
   - pInEvent       : --> Incoming event from SPM, original (pointer to).
                      <-- Incoming event from SPM, mapped (pointer to).
   - pP1            : --> Event parameter P1 (Store handle) (pointer to).
                      <-- Event parameter P1 (PDV node handle) if X410-mode
                          and S-DATA indication or S-TOKEN-PLEASE indication
                          (pointer to).
   - ulP4           : --> Event parameter P4 (ConnectResult or ReleaseResult).
   - pLockP3        : <-- TRUE: locking of handle P3 necessary (pointer to).
   - pDirectSpm     : <-- TRUE: event which has to be mapped directly from SPM
                          (pointer to).
   - pStore         : <-- Store handle (encoded byte string) (pointer to).
                          Handle may be != 0 if a decode error occured.
   - phNode         : <-- Memory handle to pres. PDV node (pointer to).
   - ppNode         : <-- Pointer to pres. PDV node (pointer to).
*/


/* ------------------ Process PS-user data given to PS-user --------- */

signed short int PASCAL ro0444_dataToUser PROTO ((
   rT0430_stContext       *pContext,
   unsigned       int      uOutEvent,
   unsigned long  int FAR *pP1,
   unsigned long  int FAR *phNode,  
   unsigned long  int FAR *pX410RtpmNode,
   signed   long  int FAR *pbUserData,
   rT0417_stUserData  FAR *pUserData));

/* Return value == 0: OK.
                <  0: Unrecoverable error (see roerval.h).

   - pContext       : --> PPM context (pointer to).
   - pP1            : <-- Event parameter P1 (PDV node handle) (pointer to).
   - phNode         : <-- Memory handle to pres. PDV node (pointer to).
   - pX410RtpmNode  : --> PS-user data handle (X410-mode) (pointer to).
   - pbUserData     : --> PS-user data OPTIONAL flag
                          (normal-mode) (pointer to).
   - pUserData      : --> PS-user data struct within PDV node
                          (normal-mode) (pointer to).
*/


/* ------------------ Process PS-user data got from PS-user --------- */

signed short int PASCAL ro0445_dataFromUser PROTO ((
   rT0430_stContext       *pContext,
   unsigned       int      uOutEvent,
   unsigned long  int FAR *pP1,
   unsigned long  int      ulP4,
   unsigned long  int FAR *phNode,  
   unsigned long  int FAR *pX410RtpmNode,
   signed   long  int FAR *pbUserData,
   rT0417_stUserData  FAR *pUserData));

/* Return value == 0: OK.
                <  0: Unrecoverable error (see roerval.h).

   - pContext       : --> PPM context (pointer to).
   - uOutEvent      : --> Outgoing event.          
   - pP1            : <-> Event parameter P1 (PDV node handle) (pointer to).
   - phNode         : <-> Memory handle to pres. PDV node (pointer to).
   - pX410RtpmNode  : <-- PS-user data handle (X410-mode) (pointer to).
   - pbUserData     : <-- PS-user data OPTIONAL flag
                          (normal-mode) (pointer to).
   - pUserData      : <-- PS-user data struct within PDV node
                          (normal-mode) (pointer to).
*/


/* ------------------ Process an event which has to be mapped   ----- */
/* ------------------ directly from/to SPM (pseudo PA function) ----- */

signed short int PASCAL ro0446_eventDirect PROTO ((
   rT0430_stContext       *pContext,
   unsigned       int      uInEvent,
   unsigned       int FAR *pOutEvent,
   rT041G_unPpdu      FAR *pNode,
   rT0417_stUserData  FAR * FAR *pUserData));

/* Return value == 0: OK.
                <  0: Unrecoverable error (see roerval.h).
                >  0: Presentation provider abort reason.

   - pContext       : --> PPM context (pointer to).
   - uInEvent       : --> Incoming event from PS-user or SPM (pointer to).
   - pOutEvent      : <-- Outgoing event to PS-user or SPM (pointer to).
   - pNode          : --> Pointer to pres. PDV node.
   - pUserData      : <-- Pointer to PS-user data struct within PDV node
                          (normal-mode) (pointer to).
*/


/* ------------------ Process a presentation provider abort --------- */

signed short int PASCAL ro0447_providerAbort PROTO ((
   rT0430_stContext       *pContext,
   signed         int      iAbortReason,
   unsigned       int      uInEvent,
   unsigned long  int      ulP1,
   unsigned long  int      ulP2,
   unsigned long  int      ulP3,
   unsigned long  int      ulP4,
   unsigned long  int      hNode,
   unsigned long  int      hsStore));

/* Return value == 0: OK.
                <  0: Unrecoverable error (see roerval.h).

   - pContext       : --> PPM context (pointer to).
   - iAbortReason   : --> Provider abort reason.
   - uInEvent       : --> Incoming event, mapped.
   - ulP1           : --> Resource to be cleared: 
                          Event parameter P1 (PDV node handle). 
   - ulP2           : --> If RC080H_STRIGGERABORT:
                          Additional abort info.
   - ulP3           : --> Resource to be cleared: 
                          Event parameter P3 (Memory handle to structure)
                          (conn. establishment or ACTIVITY-START/RESUME).
   - ulP4           : --> If RC080H_STRIGGERABORT:
                          Whether additional abort info pos./neg. signed.
   - hNode          : --> Resource to be cleared: 
                          Memory handle to pres. PDV node.
   - hsStore        : --> Resource to be cleared: 
                          Store handle (encoded byte string).
*/

/* ****************************************************************** */
/*                                                                    */
/*                    ROPPM05.C, ROPPM06.C functions                  */
/*                                                                    */
/* ****************************************************************** */

/* ****************** PPA functions (State-Event-Matrix) ************ */
/* ******************                                    ************ */

/* CES compiler: error (const!) !?
signed short int (*const rv0460_vPpaMatrix [Sta_MaxStates] [IEv_MaxInEvent]) PROTO ((
*/
/* already defied in roppm00.c by kc on 21-9-93 
signed short int (PASCAL * rv0460_vPpaMatrix [RC0434_MaxStates] [RC0435_MaxInEvent])
PROTO ((
rT0430_stContext       *pContext,            * <-> PM context (pointer to)   *
rT0441_stEntContext    *pEntContext,         * --> Pres. entity invocation   *
                                             *     context (pointer to)      *
rT04a0_unConn      FAR *pConn,               * <-> Parameter P3 structure    *
                                             *                (pointer to)   *
rT041G_unPpdu      FAR *pNode,               * <-> Pres PDV node (pointer to)*
unsigned long  int FAR * FAR *pX410RtpmNode, * <-- Ptr to PS-user data handle*
                                             *     (X410-mode) (pointer to)  *
signed   long  int FAR * FAR *pbUserData,    * <-- Pointer to PS-user data   *
                                             *     OPTIONAL flag             *
                                             *     (normal-mode) (pointer to)*
rT0417_stUserData  FAR * FAR *pUserData,     * <-- Ptr to PS-user data struct*
                                                  (normal-mode) (pointer to)*
*signed       int FAR *pbEventId,*         * <-- TRUE: provider abort with *
                                             *     event id.                 *
                                             *     (pointer to)              *
unsigned       int FAR *pOutEvent,           * <-- Outgoing event(pointer to)*
unsigned long  int FAR *phNode,              * <-> Pres. PDV node(pointer to)*
unsigned long  int FAR *pP2,                 * <-> Event param P2 (ptr to) !?*
unsigned long  int FAR *pP3,                 * <-> Event param P2 (ptr to)   
unsigned long  int FAR *pP4                  * <-> Event param P4 (ptr to)   *
));
 */

#endif	/* _ROPPM12_H */
