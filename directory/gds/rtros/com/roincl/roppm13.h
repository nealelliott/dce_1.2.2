/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm13.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:54  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:11:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:03  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:03  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:33  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:40  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:24  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:53  keutel
 * 	creation
 * 	[1994/03/21  13:19:16  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPM13_H
#define _ROPPM13_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppm13_rcsid[] = { "@(#) $RCSfile: roppm13.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:53 $" } ;
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
/* NAME         : roppm13.h                                           */
/*                                                                    */
/* AUTHOR       : Harald Miersch                                      */
/* DATE         : 92-01-28                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : PPM intern header                                   */
/*                Function prototyping                                */
/*                (presentation context handling during decoding)     */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roppm13.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-01-28|  original                      |hm  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/* ****************************************************************** */
/*                                                                    */
/*                    ROPPM02.C functions                             */
/*                                                                    */
/* ****************************************************************** */

/* ****************** Presentation context handling functions, ****** */
/* ****************** called from the MAVROS created functions ****** */
/* ****************** which decode PPCI.                       ****** */

/* ------------------ CP PPDU --------------------------------------- */

signed short int PASCAL ro0420_syntaxCp PROTO ((
   void                   *pContext,
   rT041G_unPpdu      FAR *pNode));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value,
                       mapping of the value within the PPM encoding function).

   - pContext       : <-> PPM context (pointer to).
   - pNode          : --> Pres. PDV node (pointer to).
*/


/* ------------------ CPA PPDU -------------------------------------- */

signed short int PASCAL ro0421_syntaxCpa PROTO ((
   void                   *pContext,
   rT041G_unPpdu      FAR *pNode));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value,
                       mapping of the value within the PPM encoding function).

   - pContext       : <-> PPM context (pointer to).
   - pNode          : --> Pres. PDV node (pointer to).
*/


/* ------------------ CPR PPDU -------------------------------------- */

signed short int PASCAL ro0422_syntaxCpr PROTO ((
   void                   *pContext,
   rT041G_unPpdu      FAR *pNode));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value,
                       mapping of the value within the PPM encoding function).

   - pContext       : <-> PPM context (pointer to).
   - pNode          : --> Pres. PDV node (pointer to).
*/


/* ------------------ ARU PPDU -------------------------------------- */

signed short int PASCAL ro0423_syntaxAru PROTO ((
   void                   *pContext,
   rT041G_unPpdu      FAR *pNode));

/* Return value == 0: OK.
                != 0: Error
                      (caller returns immediately with the same value,
                       mapping of the value within the PPM encoding function).

   - pContext       : <-> PPM context (pointer to).
   - pNode          : --> Pres. PDV node (pointer to).
*/

#endif	/* _ROPPM13_H */
