/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roacpm01.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:32  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:21  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:08  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:21  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:15  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:42  keutel
 * 	creation
 * 	[1994/03/21  13:18:33  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROACPM01_H
#define _ROACPM01_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roacpm01_rcsid[] = { "@(#) $RCSfile: roacpm01.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:33 $" } ;
#endif

/* *************************************************************************/
/*                                                                         */
/*  * COPYRIGHT  (C)                                                       */
/*  * Siemens Nixdorf Informationssysteme AG 1992                          */
/*  * All rights reserved                                                  */
/*  *                                                                      */
/*  * This software is furnished under licence and may be used only        */
/*  * in accordance with the terms of that licence and with the            */
/*  * inclusion of the above copyright notice.                             */
/*  * This software may not be provided or otherwise made available        */
/*  * to, or used by, any other person. No title to or ownership of        */
/*  * the software is hereby  transferred.                                 */
/*                                                                         */
/* *************************************************************************/

/*daton ********************************************************************/
/*                                                                         */
/* TYPE         : INCLUDE                                                  */
/*                                                                         */
/* NAME         : roacpm01.h                                               */
/*                                                                         */
/* AUTHOR       : Michael Rippstain                                        */
/* DATE         : 92-01-09                                                 */
/*                                                                         */
/* COMPONENT    : Association Control Service Element (ACSE)               */
/*                                                                         */
/* PRD#/VERS.   : RTS-V2.00                                                */
/*                                                                         */
/* DESCRIPTION  : Internal definitions                                     */
/*                Include for Help Functions for the Protocol Machine      */
/*                                                                         */
/* SYSTEM DEPENDENCE :  none                                               */
/*                                                                         */
/* HISTORY      : sccsid  =  @(#)roacpm01.h	1.1 94/01/26                                       */
/*                                                                         */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM#      */
/* 2.00 K0  | 92-01-23|  original                      |mr  |              */
/*          |         |                                |    |              */
/*datoff *******************************************************************/


/* ****************** Defines for the help functions ********************* */
/* ****************** (argument of copyApTitle)      ********************* */


#define RC0690_Called   0
#define RC0691_Calling  1
#define RC0692_Respond  2

/* ****************** Function prototyping ******************************* */
/* ******************                      ******************************* */


signed short int FAR PASCAL
   ro0650_copyAplCxtName            PROTO (
	  (signed short int        ,
	   rTro01_pfvObjectId        )
				     );

signed short int FAR PASCAL
   ro0651_cmpAndRetrieveAplCxtName  PROTO (
	  (signed   short int FAR *,
	   rTro01_pfvObjectId         )
				     );

signed short int FAR PASCAL
   ro0652_prpCpyTtlQul              PROTO (
	  (signed   short int       ,    /* mode      ; */
	   unsigned long  int  FAR *,    /* stAplTitle; */
	   signed   long  int  FAR *,    /* bApTitle  ; */
	   signed   long  int  FAR *,    /* lApTitle  ; */
	   rT0635_uApTitle     FAR *,    /* uApTitle  ; */
	   signed   long  int  FAR *,    /* bAeQuali  ; */
	   signed   long  int  FAR *,    /* lAeQuali  ; */
	   rT0637_uAeQualifier FAR * )   /* uAeQuali  ; */
				      );

signed short int FAR PASCAL
   ro0653_retrieveApTitle           PROTO (
	  (unsigned long  int  FAR *,    /* stAplTitle; */
	   signed   long  int       ,    /*  bApTitle;  */
	   signed   long  int       ,    /*  lApTitle;  */
	   rT0635_uApTitle     FAR *,    /*  uApTitle;  */
	   signed   long  int       ,    /*  bAeQuali;  */
	   signed   long  int       ,    /*  lAeQuali;  */
	   rT0637_uAeQualifier FAR * )   /*  uAeQuali;  */
				      );

signed short int FAR PASCAL
   ro0654_copyInvocId               PROTO (
	  (signed   long  int  FAR  *,    /*  bApInvocId;*/
	   signed   long  int  FAR  *,    /*  lApInvocId;*/
	   signed   long  int  FAR  *,    /*  bAeInvocId;*/
	   signed   long  int  FAR  * )   /*  lAeInvocId;*/
				       );

signed short int FAR PASCAL
   ro0655_cmpInvocId                PROTO (
	  (signed   long  int        ,    /*  bApInvocId  */
	   signed   long  int        ,    /*  lApInvocId  */
	   signed   long  int        ,    /*  bAeInvocId  */
	   signed   long  int        ,    /*  lAeInvocId  */
	   unsigned long  int  FAR * )   /*  pulReason   */
				       );

signed short int FAR PASCAL
   ro0656_copyAeTitle               PROTO (
	  (rTro72_unAeTitle    NEAR *,    /* pSource      IN  */
	   signed   long  int  FAR  *,    /* pbApTitle    OUT */
	   signed   long  int  FAR  *,    /* plApTitle        */
	   rT0635_uApTitle     FAR  *,    /* puApTitle        */
	   signed   long  int  FAR  *,    /* pbAeQuali        */
	   signed   long  int  FAR  *,    /* plAeQuali        */
	   rT0637_uAeQualifier FAR  * )   /* puAeQuali        */
				       );

signed short int FAR PASCAL
   ro0657_cmpAeTitle                PROTO (
	  (unsigned long  int        ,    /* hAeTitle1,   IN  */
	   unsigned long  int  FAR  * )   /* pulReason    OUT */
				       );
signed    short int NEAR PASCAL ro0658_CheckAplCxtIDs PROTO ((  
  signed   short  int       sSourID,                        
  signed   short  int       sTarID,                         
  signed   short  int     * pbResult ));                        

signed  short int NEAR PASCAL 
   ro0659_retriveRDN            PROTO(               
        (rTro69_stRDN  FAR  *      pLocRDN,                             
         rT0631_stRDN  FAR  *      pPduRDN)
                                           );                             
                                                                       
signed  short int NEAR PASCAL 
   ro065a_retriveAVA           PROTO(  
         (rTro68_stAVA                  FAR  *pLocAVA,      
          rT0630_stAttriValueAssertion  FAR  *pPduAVA)
                                                      );      

signed  short int NEAR PASCAL 
    ro065b_CpyRDN             PROTO (     
        (rT0631_stRDN  FAR  *      pPduRDN,                   
         rTro69_stRDN  FAR  *      pLocRDN)
                                            );                   

signed  short int NEAR PASCAL 
    ro065c_CpyAVA             PROTO(     
        (rT0630_stAttriValueAssertion  FAR  *pPduAVA,         
         rTro68_stAVA                  FAR  *pLocAVA)
                                                     );          

#endif	/* _ROACPM01_H */
