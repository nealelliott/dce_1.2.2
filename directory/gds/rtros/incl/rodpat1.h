/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rodpat1.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:34  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:34:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:06  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:32  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:07  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:06  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:56:52  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:12  keutel
 * 	creation
 * 	[1994/03/21  13:26:20  keutel]
 * 
 * $EndLog$
 */
#ifndef _RODPAT1_H
#define _RODPAT1_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rodpat1_rcsid[] = { "@(#) $RCSfile: rodpat1.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:34 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONS AG         */
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
/* NAME         : rodpat1.h                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller                                      */
/* DATE         : 91-12-01                                            */
/*                                                                    */
/* COMPONENT    : RTS dispatcher                                      */
/*                                                                    */
/* PRD#/VERS.   : RTS-V02.00                                          */
/*                                                                    */
/* DESCRIPTION  : Dispatchers interface to En/Decoding                */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  = "@(#) @(#)rodpat1.h1.293/12/20"                       */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 02.00 K0 | 91-12-01|  original                      |jm  |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************** Encoding function (called ********************* */
/* ****************** via dispatcher)           ********************* */

signed short int PASCAL ro0247_encodingPm  PROTO ((signed  short int        ,
						   rTro76_stDefinedBy FAR  *,
						   rTro01_pfvObjectId	    ,
						   unsigned long int        ,
						   void               NEAR *,
						   unsigned long int  FAR  *,
						   unsigned long int  FAR  *,
						   signed        int));

/* Return value == 0: succes                                  */
/*              != 0: error                                   */
/*                                                            */
/* Parameter 1     : pm number                                */
/* Parameter 2     : as parameter 1 of pm's encoding function */
/* Parameter 3     : as parameter 2 of pm's encoding function */
/* Parameter 4     : as parameter 3 of pm's encoding function */
/* Parameter 5     : as parameter 4 of pm's encoding function */
/* Parameter 6     : as parameter 5 of pm's encoding function */
/* Parameter 7     : as parameter 6 of pm's encoding function */
/* Parameter 8     : as parameter 7 of pm's encoding function */

/* ****************** Decoding function (called ********************* */
/* ****************** via dispatcher)           ********************* */

signed short int PASCAL ro0248_decodingPm PROTO ((signed  short  int      ,
						  signed  short  int      ,
						  signed  long   int      ,
						  rTro76_stDefinedBy FAR *,
						  rTro01_pfvObjectId	  ,
						  unsigned long  int      ,
						  void              NEAR *,
						  unsigned long  int FAR *,
						  unsigned long  int      ,
						  signed         int));

/* Return value == 0: succes                                  */
/*              != 0: error                                   */
/*                                                            */
/* Parameter 1     : pm number                                */
/* Parameter 2     : as parameter 1 of pm's decoding function */
/* Parameter 3     : as parameter 2 of pm's decoding function */
/* Parameter 4     : as parameter 3 of pm's decoding function */
/* Parameter 5     : as parameter 4 of pm's decoding function */
/* Parameter 6     : as parameter 5 of pm's decoding function */
/* Parameter 7     : as parameter 6 of pm's decoding function */
/* Parameter 8     : as parameter 7 of pm's decoding function */
/* Parameter 9     : as parameter 8 of pm's decoding function */
/* Parameter 10    : as parameter 9 of pm's decoding function */


/* ****************** Rtpm Syntax Match Encoding ******************** */
/* ****************** function                   ******************** */
/* ****************** (called via Dispatcher)    ******************** */

signed short int PASCAL ro0240_encodSyntaxMatchRtpm PROTO ((
				      unsigned long   int   FAR *phRtseUserData,
				      unsigned long   int        hsStore));

/* Return value == 0: succes                                          */
/*              != 0: error                                           */
/*                                                                    */
/* phRtseUserData  -> : The ROSE PDV node that is to be encoded       */
/*                      within an EXTERNAL into the store.            */
/*                                                                    */
/* hsStore        <-> : Handle to store into which the encoded string */
/*                      is to be written.                             */

/* ****************** Rtpm Syntax Match Decoding ******************** */
/* ****************** function                   ******************** */
/* ****************** (called via Dispatcher)    ******************** */

signed short int PASCAL ro0241_decodSyntaxMatchRtpm PROTO ((
				      unsigned long   int   FAR *phRtseUserData,
				      unsigned long   int        hsStore));

/* Return value == 0: succes                                          */
/*              != 0: error                                           */
/*                                                                    */
/* hsStore         -> : Description of store that contains the encoded*/
/*                      string to be decoded.                         */
/*                                                                    */
/* hRtseUserData  <-  : The ROSE PDV node that is decoded from the    */
/*                      provided STORE.                               */
/*                                                                    */

/* ****************** SNI Private Encoding Func. ******************** */
/* ****************** (called via Dispatcher)    ******************** */

signed short int PASCAL ro0242_encodSniP PROTO ((
					     signed   short int       pmNo,
					     unsigned long  int       pStore,
					     void              NEAR  *pContext,
					     unsigned long  int FAR  *phNode));

/* Return value == 0: succes                                          */
/*              != 0: error                                           */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/*                                                                    */

/* ****************** SNI Private Decoding Func. ******************** */
/* ****************** (called via Dispatcher)    ******************** */

signed short int PASCAL ro0243_decodSniP PROTO ((
					      signed short int         sPmNo,
					      unsigned long  int       hsStore,
					      void              NEAR * pContext,
					      unsigned long  int FAR * phNode,
                                              signed   short int FAR * psAbsId));

/* Return value == 0: succes                                          */
/*              != 0: error                                           */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/*                                                                    */

/* ****************** X.410 Mode Encoding Func.  ******************** */
/* ****************** (called via Dispatcher)    ******************** */

signed short int PASCAL ro0244_encodX410 PROTO ((
					      signed   short int       sPmNo,
					      unsigned long  int FAR * phsStore,
					      void              NEAR * pContext,
					      unsigned long  int FAR * phNode));

/* Return value == 0: succes                                          */
/*              != 0: error                                           */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/*                                                                    */

/* ****************** X.410 Mode Decoding Func.  ******************** */
/* ****************** (called via Dispatcher)    ******************** */

signed short int PASCAL ro0245_decodX410 PROTO ((
					      signed   short int       sPmNo,
					      unsigned       int       uInEvent,
					      signed   long  int       lType,
					      unsigned long  int       hsStore,
					      void              NEAR * pContext,
					      unsigned long  int FAR * phNode));

/* Return value == 0: succes                                          */
/*              != 0: error                                           */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/*                                                                    */

#endif	/* _RODPAT1_H */
