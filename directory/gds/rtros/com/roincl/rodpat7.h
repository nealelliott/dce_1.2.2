/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rodpat7.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:46  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:52  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:19  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:29  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:45  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:37  keutel
 * 	creation
 * 	[1994/03/21  13:17:38  keutel]
 * 
 * $EndLog$
 */
#ifndef _RODPAT7_H
#define _RODPAT7_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rodpat7_rcsid[] = { "@(#) $RCSfile: rodpat7.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:42 $" } ;
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
/* NAME         : rodpat7.h                                           */
/*                                                                    */
/* AUTHOR       : Hubert Trieb                                        */
/* DATE         : 92-02-19                                            */
/*                                                                    */
/* COMPONENT    : Internal structures of status reading functions     */
/*                and prototyping of functions.                       */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rodpat7.h	1.1 94/01/26                                  */
/*                                                                    */
/*                       changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-05-10|  original                      | ht |         */
/*           |        |                                |    |         */
/*                                                                    */
/*datoff **************************************************************/

/* ****************** DEFINES ************************************** */

#define RC028g_FieldSize 4   /* number of Ecb and Acb blocks realocated  */
			     /* if current field is to small             */

typedef struct rS0286_EcbInfo {
    signed    short int  EcbNo;          /* identifier for Ecb            */
    unsigned  long  int  hownPsap;       /* hMyAddr                       */
    unsigned  long  int  h1;             /* Dummy                         */
    unsigned  long  int  h2;             /* hPresInv                      */
    unsigned  long  int  h3;             /* hApplInv                      */
			      /* ownPsap, h1, h2, h3 are the same handles */
			      /* given to ro0260_attach in ro0111_attach. */
			      /* These parameters are transparent for     */
			      /* dispatcher and recovery.                 */

} rT0286_EcbInfo;


typedef struct rS0287_AcbInfo {
    signed    short int  toEcb;     /* relation to EcbNo in rT0286_EcbInfo */
    rT10z0_stAssocInfo   stInfo;
} rT0287_AcbInfo;

signed short int PASCAL ro0289_readRdbRtpm  PROTO((
    char               FAR * pSyName,
    unsigned       int       lSyName,
    unsigned long  int FAR * hEcbField,
    signed   short int FAR * lenEcb,
    unsigned long  int FAR * hAcbField,
    signed   short int FAR * lenAcb));


signed short int ro028n_resize PROTO ((
    unsigned  long  int  NEAR  *        hBlInfo,
    char                 NEAR  * NEAR * pBlInfo,
    signed    short int  NEAR  *        maxBl  ,
    unsigned  long  int                 blSize  ));

#endif	/* _RODPAT7_H */
