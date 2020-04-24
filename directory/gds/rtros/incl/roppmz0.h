/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppmz0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:56  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:49  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:45  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:49  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:25  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:26  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:54  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:23  keutel
 * 	creation
 * 	[1994/03/21  13:27:33  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPMZ0_H
#define _ROPPMZ0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppmz0_rcsid[] = { "@(#) $RCSfile: roppmz0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:56 $" } ;
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
/* NAME         : roppmz0.h                                           */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE         : 93-04-07                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : PPM protocol interface                              */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roppmz0.h	1.2 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-02-12|  original                      |vp  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/



/* ****************** Static Structure for get DCS  ***************** */
/* ****************** used by PPM inform function   ***************** */
/* **************************  vinita ******************************* */


typedef struct rS04z0_stDCS {
  
   signed   short int      pcnum     ; 
   rTro60_vAbstrSynId      vAbstrSynId;
   rTro61_vAbstrSynResult  vAbstrSynResult;
   signed   short int      vTransSynId [RCro01_MaxAbstrSyn];
   signed   long  int      vPresContId [RCro01_MaxAbstrSyn];

} rT04z0_stDCS;


typedef struct rS04z1_stAbsNameId {
  
   signed   short int      sAbsId  ; 
   rTro00_vObjectId        oAbsName;

} rT04z1_stAbsNameId;

typedef struct rS04z2_stAbsIdFind {
    signed short int sAbsId;
    signed short int bFound;
    signed long int lAplId;
}rT04z2_stAbsIdFind;

typedef struct rS04z3_stMode {
     signed short int sMode;
}rT04z3_stMode;
     

/* ******************* Inform Function constants ******************** */
#define RC04z0_PpmInform            (0x0600)

#define RC04zP_ACSEAbId              (RC04z0_PpmInform | (0x0001))
#define RC04zQ_EXIFAbId              (RC04z0_PpmInform | (0x0002))
#define RC04zR_getDCS                (RC04z0_PpmInform | (0x0003))
#define RC04zS_findAbsId             (RC04z0_PpmInform | (0x0004)) 
#define RC04zT_getMode               (RC04z0_PpmInform | (0x0005)) 


/* *************End of  Inform Function constants ******************** */

#endif	/* _ROPPMZ0_H */
