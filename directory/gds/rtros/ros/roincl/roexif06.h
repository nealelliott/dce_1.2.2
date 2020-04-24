/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif06.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:15  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:57  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:58  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:10:16  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:16  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:26  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:02  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:50  keutel
 * 	creation
 * 	[1994/03/21  13:32:31  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROEXIF06_H
#define _ROEXIF06_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roexif06_rcsid[] = { "@(#) $RCSfile: roexif06.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:15 $" } ;
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
/* NAME         : roexif06.h                                          */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-03                                            */
/*                                                                    */
/* COMPONENT    : Driver for IROS 2.5 to IROS 3.00 Function proto     */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Function Prototye For roexif03.c                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roexif06.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-03-03|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/* ***************** Function Protypes ****************************** */
/* ***************** for local functions***************************** */

static    signed short int NEAR PASCAL ro1331_attach PROTO (( 
                    unsigned long  int  FAR   *pulMyScbHd,
                    signed   long  int         lTaskId,
                    FARPROC                    tpduFct,
                    R_attapb            FAR	    *,
                    RO_attapb           FAR          *));

static    signed short int NEAR PASCAL ro1332_AssocFuns PROTO (( 
                    unsigned long  int  FAR   *pulMyScbHd,
                    signed   long  int         lTaskId,
                    FARPROC                    tpduFct,
                    R_excopb            FAR	    *,
                    RO_excopb           FAR          *));

static    signed short int NEAR PASCAL ro1333_Operations PROTO (( 
                    unsigned long  int  FAR   *pulMyScbHd,
                    signed   long  int         lTaskId,
                    FARPROC                    tpduFct,
                    R_excopb            FAR	    *,
                    RO_excopb           FAR          *));

static    signed short int NEAR PASCAL ro1334_Logging PROTO (( 
                    unsigned long  int  FAR   *pulMyScbHd,
                    signed   long  int         lTaskId,
                    FARPROC                    tpduFct,
                    RO_excopb           FAR          *,
                    R_excopb            FAR	    *));

static    signed short int NEAR PASCAL ro1335_Inform PROTO (( 
                    unsigned long  int  FAR   *pulMyScbHd,
                    signed   long  int         lTaskId,
                    FARPROC                    tpduFct,
                    R_excopb            FAR   *,
                    RO_excopb           FAR   *));

static    signed short int NEAR PASCAL ro1336_redirect PROTO (( 
                    unsigned long  int  FAR   *pulMyScbHd,
                    signed   long  int         lTaskId,
                    FARPROC                    tpduFct,
                    RO_rdrqpb           FAR   *,
                    R_rdrqpb            FAR   *));

static    signed short int NEAR PASCAL ro1337_CheckOrWait PROTO (( 
                    unsigned long  int  FAR   *pulMyScbHd,
                    signed   long  int         lTaskId,
                    FARPROC                    tpduFct,
                    RO_evtpb            FAR   *,
                    R_evtpb             FAR   *));

static    signed short int NEAR PASCAL ro1338_GetApplContext PROTO (( 
                    unsigned long  int         FAR   *pulMyScbHd,
                    signed   long  int                lTaskId,
                    FARPROC                           tpduFct,
                    rT06z0_stInfrmApplCxtName  FAR   *,
                    signed         int         ));

static    signed short int NEAR PASCAL ro133a_Objcpy PROTO ((
                unsigned long int FAR  *, 
                R_unsign          FAR  *, 
                R_unsign                )); 



static    void           NEAR   PASCAL ro133b_memset  PROTO ((
                char              FAR *,
                int                    ,
                unsigned long int      ));



static    signed short int NEAR PASCAL ro133c_PresAddrCpy PROTO ((
                RO_pradd            FAR  * ,
                R_pradd             FAR  * ));


static    signed short int NEAR PASCAL ro133d_SDataCpy PROTO ((
            RO_snfrag         FAR * ,
            R_sndata          FAR * ));

static    signed short int NEAR PASCAL ro133e_EvDataCpy PROTO ((
            RO_buffer        FAR  * ,
            R_evdata         FAR  * ));
          
static    signed short int NEAR PASCAL ro133f_OpValuCpy PROTO ((
            RO_opval         FAR  * ,
            R_opval          FAR  * ));

static    signed short int NEAR PASCAL ro133g_SnDataCpy PROTO ((
            RO_sndata        FAR  * ,
            R_sndata         FAR  * ));

static    signed short int NEAR PASCAL ro133i_retrieveEData PROTO ((
            R_evtpb                  FAR * pEvtPb     ,
            char             FAR  *  FAR * pUnUsedBuf ,
            signed long int          FAR * pUnUsedLen ,
            RO_evfrag                FAR * pEvFrag    ,
            R_evdata                 FAR * pEvData));

static    signed short int NEAR PASCAL ro133j_retrieveEvData PROTO ((
            R_evtpb                  FAR * pEvtPb     ,
            char             FAR  *  FAR * pUnUsedBuf ,
            signed long int          FAR * pUnUsedLen ,
            RO_evdata                FAR * pNEvData   ,
            R_evdata                 FAR * pOEvData));

static    char FAR * NEAR PASCAL ro133k_setParameter  PROTO ((
            char                      FAR *,
            char                FAR * FAR *,
            signed   long  int        FAR *,
            char                      FAR *,
            unsigned long  int             ,
            signed   short int             ));

static    signed short int NEAR PASCAL ro133l_retrieveObjID PROTO ((
                R_evtpb                 FAR * ,
                char              FAR * FAR * ,
                signed long int         FAR * ,
                unsigned long int       FAR *, 
                R_unsign          FAR * FAR *, 
                R_unsign                FAR *));

static    signed short int NEAR PASCAL ro133m_retrieveOpValue PROTO ((
                R_evtpb                 FAR * ,
                char              FAR * FAR * ,
                signed long int         FAR * ,
                RO_opval                FAR * ,
                R_opval                 FAR * ));

static                  int NEAR PASCAL ro133n_getROSEAbsID PROTO ((
                    unsigned long  int  FAR   *pulMyScbHd,
                    signed   long  int         lTaskId,
                    FARPROC                    tpduFct));

#endif	/* _ROEXIF06_H */
