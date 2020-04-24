/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rortpz0.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:12:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:12  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:09  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:39  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:45  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:45  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:02  keutel
 * 	creation
 * 	[1994/03/21  13:17:50  keutel]
 * 
 * $EndLog$
 */
#ifndef _RORTPZ0_H
#define _RORTPZ0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rortpz0_rcsid[] = { "@(#) $RCSfile: rortpz0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:00 $" } ;
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
/* NAME         : rortpz0.h                                           */
/*                                                                    */
/* AUTHOR       : Shivkumar Haran                                     */
/* DATE         : 92-03-20                                            */
/*                                                                    */
/* COMPONENT    : Reliable transfer service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Contains the data structures and definitions        */
/*                related to the RT-INFORM functions.                 */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rortpz0.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 2.00   K0 |92-03-20|  original                      |skh |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


#define RC10z0_AssocInfo        ((signed short int)(RC1000_RtpmEvent | 0x0001))
#define RC10z1_PossibilityInfo  ((signed short int)(RC1000_RtpmEvent | 0x0002))
#define RC10z2_GenRtabPdvNode   ((signed short int)(RC1000_RtpmEvent | 0x0003))

typedef struct rS10z0_stAssocInfo {
    /*------------------------------------------------------------------------*/
    /* Fields relating to the TPM and SPM.                                    */
    /*------------------------------------------------------------------------*/
    unsigned long   int      hPeerAddress;               /*   = rtipsap       */

    /*------------------------------------------------------------------------*/
    /* Fields relating to RTPM.                                               */
    /*------------------------------------------------------------------------*/
    unsigned bAssocInitiatingRtpm  : 1;  /*   = rtiini        */
    unsigned bDialogueMode         : 1;  /*   = rtidialog     */
    unsigned bOwnerOfTurn          : 1;  /*   = rtiturn       */
    unsigned bX410Mode             : 1;  /*   = rtimode       */

        /*--------------------------------------------------------------------*/
        /* Fields applicable only for sending RTPM.                           */
        /* When bRequestTransferExists is TRUE, specifies that a              */
        /* RT-TRANSFER-req was issued by the RTPM but no RT-TRANSFER-cnf has  */
        /* been issued yet.                                                   */
        /*--------------------------------------------------------------------*/
    unsigned bRequestTransferExists: 1;  /*   = rtitexist     */
    unsigned long   int      ulRequestTransferId;        /*   = rtittrfid     */
    rT100n_stTpInfo          stRequestBuffer;            /*   = rtitapdu      */

        /*--------------------------------------------------------------------*/
        /* Fields applicable only for receiving RTPM.                         */
        /* When bBufferExists is TRUE, specifies that a RT-BUFFER has been    */
        /* issued by the RTS-user but no RT-TRANSFER-ind issued by the RTPM   */
        /* yet.                                                               */
        /*--------------------------------------------------------------------*/
    unsigned bBufferExists         : 1;  /*   = rtibexist     */
    unsigned long   int      ulBufferTransferId;         /*   = rtibtrfid     */
    rT100n_stTpInfo          stBuffer;                   /*   = rtibapdu      */

        /*--------------------------------------------------------------------*/
        /* Fields applicable for both the sending and the receiving RTPM :    */
        /*                                                                    */
        /* Valid values of bEventTransferType :                               */
        /* (a) RT-TRANSFER-ind (applicable only for receiver).                */
        /* (b) RT-TRANSFER-cnf (applicable only for sender). .                */
        /*                                                                    */
        /* Specifies that the corresponding event has been pushed out to the  */
        /* RTPM user and further no subsequent event received by the RTPM     */
        /* from the RTPM-user.                                                */
        /*                                                                    */
        /* (Note: A receipt of an event by the RTPM from the RTPM-user serves */
        /*        as an implicit acknowledgement of the fact that the event   */
        /*        has been collected by the RTPM user and can be deleted from */
        /*        the RTPM Recovery Data Base).                               */
        /*--------------------------------------------------------------------*/
    signed   long  int       lEventTransferType;         /*   = rtievent      */
    unsigned long  int       ulEventTransferId;          /*   = rtietrfid     */
    rT100n_stTpInfo          stEventTransferBuffer;      /*   = rtieapdu      */
    unsigned long  int       ulEventTransferResult;      /*   = rtieresult    */

    signed   long  int       lApplicationProtocol;       /*   = rtiaplprot    */

    /*------------------------------------------------------------------------*/
    /* Fields relating to ACPM.                                               */
    /*------------------------------------------------------------------------*/
    unsigned long   int      hPeerAeTitle;               /*   = rtiaplttl     */
    signed   short int       sApplContextId;             /*   = rtiaplcxt     */

    /*------------------------------------------------------------------------*/
    /* Fields relating to PPM.                                                */
    /*------------------------------------------------------------------------*/
    rTro61_vAbstrSynResult   vAbstrSynResult;            /*   = rtisynresult  */
    rTro60_vAbstrSynId       vAbstrSynId;                /*   = rtiabssyn     */
    rTro62_vTransSynId       vTransSynId;                /*   = rtitrfsyn     */
    rTro63_vPresContId       vPresContId;                /*   = rtiprscxtidf  */
    signed   short int       sDefContResult;             /*   = rtidfaresult  */
    signed   short int       sDefAbstrSynId;             /*   = rtidfaabssyn  */
    signed   short int       sDefTransSynId;             /*   = rtidfatrfsyn  */
    signed   short int       sRtsAbstrSynId;             /*   = rtiabsinc_rts */
} rT10z0_stAssocInfo;

typedef struct rS10z1_stPossiblityInfo {
    signed   long  int       lEvent;            /*  -> REQ or RSP service     */
                                                /*     events in rortpm0.h    */
    signed   bPossible      : 1;/* <-  Possible or not.       */
    signed   bX410Mode      : 1;/* <-  x.410 mode or not.     */
} rT10z1_stPossibilityInfo;

#endif	/* _RORTPZ0_H */
