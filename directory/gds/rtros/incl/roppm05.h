/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm05.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:55  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:48  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:43  root]
 * 
 * Revision 1.1.2.1  1994/08/22  15:21:58  keutel
 * 	OT 11620
 * 	[1994/08/19  14:22:31  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPM05_H
#define _ROPPM05_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppm05_rcsid[] = { "@(#) $RCSfile: roppm05.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:55 $" } ;
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
/* NAME         : roppm05.h                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-03-24                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Prototyping of E/D Control functions                */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roppm05.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-03-24|  original                      |hm  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/* ****************** E/D Control functions (encoding) ************** */
/* ******************                                  ************** */

/* ------------------ Simply-encoded presentation user-data --------- */

signed short int PASCAL ro0410_encodeSimply PROTO ((
   rTro01_pfvObjectId	    FAR *pTransSynName,
   signed   short int       FAR *pPmNumb,
   void                         *pCxt,
   unsigned long  int            hNode));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value).

   - pTransSynName  : <-- Pointer to transfer syntax object identifier
                          (pointer to).
   - pPmNumb        : <-- Protocol machine number (pointer to) (see rodpat0.h).
   - pCxt       : --> PPM context (pointer to).
   - hNode          : --> Memory handle to PDV node.
*/


/* ------------------ PDV-list -------------------------------------- */

signed short int PASCAL ro0411_encodePdvList PROTO ((
   rT0410_stPdvList         FAR *pEncodingInfo,
   rTro01_pfvObjectId	    FAR *pTransSynName,
   signed   short int       FAR *pPmNumb,
   void                         *pCxt,
   unsigned long  int            hNode));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value).

   - pEncodingInfo  : <-- Encoding information (pointer to) (see roppm01.h).
   - pTransSynName  : <-- Pointer to transfer syntax object identifier
                          (pointer to).
   - pPmNumb        : <-- Protocol machine number (pointer to) (see rodpat0.h).
   - pCxt       : --> PPM context (pointer to).
   - hNode          : --> Memory handle to PDV node.
*/


/* ------------------ EXTERNAL -------------------------------------- */

signed short int PASCAL ro0412_encodeExternal PROTO ((
   rT0411_stExternal        FAR *pEncodingInfo,
   unsigned long  int FAR * FAR *pTransSynName,
   signed   short int       FAR *pPmNumb,
   void                         *pCxt,
   unsigned long  int            hNode));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value).

   - pEncodingInfo  : <-- Encoding information (pointer to) (see roppm01.h).
   - pTransSynName  : <-- Pointer to transfer syntax object identifier
                          (pointer to).
   - pPmNumb        : <-- Protocol machine number (pointer to) (see rodpat0.h).
   - pCxt       : --> PPM context (pointer to).
   - hNode          : --> Memory handle to PDV node.
*/


/* ------------------ ANY, ANY DEFINED BY --------------------------- */

signed short int PASCAL ro0413_encodeAny PROTO ((
   rTro76_stDefinedBy FAR * FAR *pDefinedBy,
   unsigned long  int FAR * FAR *pTransSynName,
   signed   short int       FAR *pPmNumb,
   void                         *pCxt,
   unsigned long  int            hNode));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value).

   - pDefinedBy     : --> Pointer to ANY DEFINED BY encoding information which
                          is to be stored in the PPM context (pointer to).
                          NULL pointer if nothing is to be stored.
                      <-- Pointer to ANY DEFINED BY encoding information which
                          is stored now in the PPM context (pointer to).
                          NULL pointer if nothing is stored.
   - pTransSynName  : <-- Pointer to transfer syntax object identifier
                          (pointer to).
   - pPmNumb        : <-- Protocol machine number (pointer to) (see rodpat0.h).
   - pCxt       : --> PPM context (pointer to).
   - hNode          : --> Memory handle to PDV node.
*/

/* ****************** E/D Control functions (decoding) ************** */
/* ******************                                  ************** */

/* ------------------ Simply-encoded presentation user-data --------- */

signed short int PASCAL ro0415_decodeSimply PROTO ((
   rTro01_pfvObjectId	    FAR *pTransSynName,
   signed   short int       FAR *pAbstrSynId,
   signed   short int       FAR *pPmNumb,
   void                         *pCxt));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value).

   - pTransSynName  : <-- Pointer to transfer syntax object identifier
                          (pointer to).
   - pAbstrSynId    : <-- Abstract syntax identifier (pointer to).
   - pPmNumb        : <-- Protocol machine number (pointer to) (see rodpat0.h).
   - pCxt       : --> PPM context (pointer to).
*/


/* ------------------ PDV-list -------------------------------------- */

signed short int PASCAL ro0416_decodePdvList PROTO ((
   rTro01_pfvObjectId	    FAR *pTransSynName,
   signed   short int       FAR *pAbstrSynId,
   signed   short int       FAR *pPmNumb,
   rT0410_stPdvList         FAR *pEncodingInfo,
   void                         *pCxt));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value).

   - pTransSynName  : <-- Pointer to transfer syntax object identifier
                          (pointer to).
   - pAbstrSynId    : <-- Abstract syntax identifier (pointer to).
   - pPmNumb        : <-- Protocol machine number (pointer to) (see rodpat0.h).
   - pEncodingInfo  : --> Encoding information (pointer to) (see roppm01.h).
   - pCxt       : --> PPM context (pointer to).
*/


/* ------------------ EXTERNAL -------------------------------------- */

signed short int PASCAL ro0417_decodeExternal PROTO ((
   rTro01_pfvObjectId	    FAR *pTransSynName,
   signed   short int       FAR *pAbstrSynId,
   signed   short int       FAR *pPmNumb,
   rT0411_stExternal        FAR *pEncodingInfo,
   void                         *pCxt));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value).

   - pTransSynName  : <-- Pointer to transfer syntax object identifier
                          (pointer to).
   - pAbstrSynId    : <-- Abstract syntax identifier (pointer to).
   - pPmNumb        : <-- Protocol machine number (pointer to) (see rodpat0.h).
   - pEncodingInfo  : --> Encoding information (pointer to) (see roppm01.h).
   - pCxt       : --> PPM context (pointer to).
*/


/* ------------------ ANY, ANY DEFINED BY --------------------------- */

signed short int PASCAL ro0418_decodeAny PROTO ((
   rTro76_stDefinedBy FAR * FAR *pDefinedBy,
   rTro01_pfvObjectId	    FAR *pTransSynName,
   signed   short int       FAR *pAbstrSynId,
   signed   short int       FAR *pPmNumb,
   signed   short             sAnyNumb,
   void                         *pCxt));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value).

   - pDefinedBy     : --> Pointer to ANY DEFINED BY encoding information which
                          is to be stored in the PPM context (pointer to).
                          NULL pointer if nothing is to be stored.
                      <-- Pointer to ANY DEFINED BY encoding information which
                          is stored now in the PPM context (pointer to).
                          NULL pointer if nothing is stored.
   - pTransSynName  : <-- Pointer to transfer syntax object identifier
                          (pointer to).
   - pAbstrSynId    : <-- Abstract syntax identifier (pointer to).
   - pPmNumb        : <-- Protocol machine number (pointer to) (see rodpat0.h).
   - sAnyNumb       : --> ANY number (implicit encoding information).
   - pCxt       : --> PPM context (pointer to).
*/
signed    short  int     ro042a_indefDecset PROTO (( void       *pContext));
signed    long   int     ro042b_indefDecget PROTO (( void       *pContext));
signed    short  int     ro042c_indefDecinc PROTO (( void      *pContext));
#endif	/* _ROPPM05_H */
