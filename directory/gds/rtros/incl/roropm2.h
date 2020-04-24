/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropm2.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:01  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:52  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:56  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:53  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:35  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:30  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:08  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:32  keutel
 * 	creation
 * 	[1994/03/21  13:27:44  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROROPM2_H
#define _ROROPM2_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roropm2_rcsid[] = { "@(#) $RCSfile: roropm2.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:01 $" } ;
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
/* NAME         : roropm2.h                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-02-05                                            */
/*                                                                    */
/* COMPONENT    : Remote Operations Service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Protocol Machine Access Interface                   */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropm2.h	1.2 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-02-05|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/* ************************ PM running function ***************************** */
/* ************************                     ***************************** */

signed   short int FAR PASCAL ro0700_ropm    PROTO ((
                                unsigned long  int  hContext));

/*
   Return value == 0 : successful completion
                != 0 : erroneous  completion
   hContext          : PM context handle.
*/

/* ************************ PM clean up function **************************** */
/* ************************                      **************************** */

signed   short int FAR PASCAL ro0701_cleanUp PROTO ((signed  long int lEvent,
                                                unsigned long int hP1orhContext,
                                                unsigned long int hP2,
                                                unsigned long int hP3,
                                                unsigned long int hP4));

/*
   Return value == 0 : successful completion
                != 0 : erroneous  completion

   --------------------------------------------------------------------------
   Mode 1            : Called by Dispatcher                                  
   --------------------------------------------------------------------------
   pEvent            : PM event     = 0                                      
   hP1orhContext     : Parameter-p1 = Context Handle.                        
   hP2               : Parameter-p2 = Not used                               
   hP3               : Parameter-p3 = Not used                               
   hP4               : Parameter-p4 = Not used                               
   --------------------------------------------------------------------------

   --------------------------------------------------------------------------
   Mode 2            : Called by ROPM or by Dispatcher.                      
   --------------------------------------------------------------------------
   pEvent            : PM event    != 0                                      
   hP1orhContext     : Parameter-p1 = PDV tree      (if used)                
                                    = NULL          (if not used)            
   hP2               : Parameter-p2 = Not used.                              
   hP3               : Parameter-p3 = Memory handle (if used)                
                                    = NULL          (if not used)            
   hP4               : Parameter-p4 = Not used.                              
   --------------------------------------------------------------------------
*/

/* ************************ PM encoding function **************************** */
/* ************************                      **************************** */

signed short int FAR PASCAL ro0702_encodRopm PROTO ((
                            rTro76_stDefinedBy FAR  *,
			    rTro01_pfvObjectId	     ,
                            unsigned long  int       ,
			    void               NEAR *,
                            unsigned long  int FAR  *,
                            unsigned long  int FAR  *,
                            signed         int      ));

/*
   Return value == 0: OK.
                != 0: Error
                      (Caller returns immediately with the same value,
                       mapping of the value within the PPM encoding function).

   - pDefinedBy     :  -> Not used for ROPM (NULL pointer). Supposed to contain
                          ANY DEFINED BY encoding information (pointer to).
   - pTrfSynObjId   :  -> Transfer syntax object identifier (pointer to).
   - hsStore        :  -> Store handle (encoded byte string).
   - pPpmContext    :  -> PPM context (pointer to).
   - phRoseApdus    : <-> Memory handle to PDV node (pointer to).
   - pLength        : <-  Total length of all embedded PDV encodings (pointer
                          to).
                          - ANY, OCTET STRING : scale "byte".
                          - BIT STRING        : scale "bit".
                            (lEncodingChoice from PDV header, see rouply0.h)
*/

/* ************************ PM decoding function **************************** */
/* ************************                      **************************** */

signed short int FAR PASCAL ro0703_decodRopm PROTO ((
                            signed   short int       ,
                            signed   long  int       ,
                            rTro76_stDefinedBy FAR  *,
			    rTro01_pfvObjectId	     ,
                            unsigned long  int       ,
			    void               NEAR *,
                            unsigned long  int FAR  *,
                            unsigned long  int       ,
                            signed         int      ));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value,
                       mapping of the value within the PPM decoding function).

   - sAbstrSynId    :  -> Abstract syntax identifier (describes the named
                          abstract syntax the node belongs to).
                          Has to be written into the PDV header
                         (is not necessary for the decoding function itself).
   - lEncodingChoice:  -> Encoding choice value of the PDV node:
                          - ANY
                          - OCTET STRING
                          - BIT STRING.
                          Has to be written into the PDV header
                          (is not necessary for the decoding function itself).
                          For values and further comments see rouply0.h.
   - pDefinedBy     :  -> Not used for RTPM (NULL pointer). Supposed to contain
                          ANY DEFINED BY encoding information (pointer to).
   - pTrfSynObjId   :  -> Transfer syntax object identifier (pointer to).
   - hsStore        :  -> Store handle (encoded byte string).
   - pPpmContext    :  -> PPM context (pointer to).
   - phRoseApdus    : <-  Memory handle to PDV node (pointer to).
   - ulLength       : <-  Total length of all embedded PDV encodings.
                          - ANY, OCTET STRING : scale "byte".
                          - BIT STRING        : scale "bit".
                          Note: ANY length only if within PDV-list or EXTERNAL
                                and if definite length encoding (otherwise 0).
*/

/* ********************* PM clean up PDV node function ********************** */
/* *********************                               ********************** */

signed short int FAR PASCAL ro0704_clearNodeRopm PROTO ((
                                          unsigned long  int FAR *phRoseApdus));

/* Return value == 0: OK.
                <  0: Unrecoverable error (see roerval.h).

   - phRoseApdus    : <-> Memory handle to PDV node (pointer to).
*/


/* ********************* Function for requesting info    ******************** */
/* ********************* about status of RTPM objects    ******************** */

signed short int FAR PASCAL ro0705_informRopm  PROTO ((
           unsigned long  int  hContext,               /*  -> : see rortpm3.h */
           signed   short int  sInformType,            /*  -> : see rortpmi.h */
           void           FAR *pRequestedInformation));/* <-  : see rortpmi.h */

/* Return value == 0: OK.
                != 0: Error

   - hContext              :  -> Handle to context of RTPM relating to which
                                 information is requested.
   - sInformType           :  -> Type of information requested.
   - pRequestedInformation : <-  Information that was requested.
*/

/* ********************* PM encoding function  for       ******************** */
/* ********************* SNI Private Mode.               ******************** */

signed short int PASCAL ro070a_encodSniPRopm PROTO ((
                                   unsigned long int          hStore,
                                   void                NEAR  *pPpmContext,
                                   unsigned long int   FAR   *phRoseApdus));

/*
   Return value == 0: OK.
                != 0: Error
                      (Caller returns immediately with the same value,
                       mapping of the value within the PPM encoding function).

   - hsStore        : <-> Store handle (for encoded byte string).
   - pPpmContext    : <-> PPM context (pointer to).
   - phRoseApdus    :  -> Memory handle to PDV node (pointer to).
*/

/* ********************* PM decoding function  for       ******************** */
/* ********************* SNI Private Mode.               ******************** */

signed short int PASCAL ro070b_decodSniPRopm PROTO ((
                                   signed   short int  FAR   *psAbsId,
                                   unsigned long int          hStore,
                                   void                NEAR  *pPpmContext,
                                   unsigned long int   FAR   *phRoseApdus));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value,
                       mapping of the value within the PPM decoding function).
   - sAbsId         :     AbsId 
   - hsStore        :  -> Store handle (encoded byte string).
   - pPpmContext    :  -> PPM context (pointer to).
   - phRoseApdus    : <-  Memory handle to PDV node (pointer to).
*/

#endif	/* _ROROPM2_H */
