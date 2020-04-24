/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm02.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:52  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:45  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:36  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:46  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:22  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:23  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:45  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:17  keutel
 * 	creation
 * 	[1994/03/21  13:27:02  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPM02_H
#define _ROPPM02_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppm02_rcsid[] = { "@(#) $RCSfile: roppm02.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:52 $" } ;
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
/* NAME         : roppm02.h                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 92-01-28                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Protocol machine calling interface                  */
/*                (without E/D Control functions)                     */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roppm02.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-01-28|  adaptation of ROS V2.5A00 K05 |hm  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/* ****************** Common functions ****************************** */
/* ******************                  ****************************** */

/* ------------------ PM function ----------------------------------- */

signed short int PASCAL ro0400_ppm PROTO ((
   unsigned long  int      hContext));

/* Return value == 0: OK.
                <  0: Unrecoverable error (see roerval.h).

   - hContext       : --> Memory handle to PPM context
                          (if 0, a context will be created).
*/


/* ------------------ CleanUp function ------------------------------ */

signed short int PASCAL ro0401_cleanUp PROTO ((
   signed   long  int      lEvent,
   unsigned long  int      ulP1orhContext,
   unsigned long  int      ulP2,
   unsigned long  int      ulP3,
   unsigned long  int      ulP4));

/* Return value == 0: OK.
                <  0: Unrecoverable error (see roerval.h).

   - lEvent         : --> Event to clear or 0.
   - ulP1orhContext : --> lEvent == 0: Memory handle to PPM context
                          lEvent != 0: PM event parameter 1.
   - ulP2           : --> PM event parameter 2.
   - ulP3           : --> PM event parameter 3.
   - ulP4           : --> PM event parameter 4.
*/


/* ------------------ ClearPdvNode function ------------------------- */

signed short int PASCAL ro0404_clearNodePpm PROTO ((
   unsigned long  int FAR *phNode));

/* Return value == 0: OK.
                <  0: Unrecoverable error (see roerval.h).

   - phNode         : <-> Memory handle to PDV node (pointer to).
*/

/* ****************** Special functions ***************************** */
/* ******************                   ***************************** */

/* ------------------ Get the PM number of a PDV node --------------- */

signed short int PASCAL ro0405_getPmNumb PROTO ((
   unsigned long  int      hNode));

/* Return value >= 0: Protocol machine number read from the PDV header
                      (values see rodpat0.h).
                <  0: Unrecoverable error (see roerval.h).

   - pNode          : --> Memory handle to PDV node (which is to be cleared).
*/


/* ------------------ Get a searched AbsId -------------------------- */

signed short int PASCAL ro0406_informPpm PROTO ((
unsigned long  int      hContext,      
signed   short int      sInformType,   
void               FAR *pRequestedInfo ));

/* Return value <= RCro6b_MaxAbstrSynId: Abstract syntax identifier (AbsId).
                >  RCro6b_MaxAbstrSynId: Not found.

   - pObjectId      : --> Object identifier of the searched abstract syntax
                          identifier (pointer to).
*/

#endif	/* _ROPPM02_H */
