/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: usrstep.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:58  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:06  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:32:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:12  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:07:07  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:48  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:38  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:54:15  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:16  keutel
 * 	creation
 * 	[1994/03/21  13:23:56  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: usrstep.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:58 $" } ;
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
/* NAME         : dummy.c                                             */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-05-04                                            */
/*                                                                    */
/* COMPONENT    : STEP dummy USER login records                       */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : main module of STEP                                 */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)usrstep.c	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-05-04|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

#define STEP_VERSION   300

/* ******************************************************************* */
/*           I n c l u d e  F i l e s                                  */
/* ******************************************************************* */

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <rogenrl.h>
#include <rouply0.h>
#include <rouply1.h>
#include <routil0.h>
 
#include <routil6.h>      /* for login data definitions */
#include <spstep0.h>
#include <spusrrec.h>
 
 
/*************************************************************************/
/*                     Local Constants and Macros                        */
/*************************************************************************/

/*************************************************************************/
/*************************************************************************/
/***                                                                   ***/
/***                        Global Functions                           ***/
/***                                                                   ***/
/*************************************************************************/
/*************************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: r_evalusrrec    				      */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-08-30                                            */
/*                                                                    */
/* SYNTAX       :                                                     */


void r_evalusrrec (
	signed long int       lType,
	signed long           lLen,
	char            FAR * pPtr)


/*                                                                    */
/* DESCRIPTION  :  This function dumps the hex code of the User rec   */
/*                 User has to replace this function with his own     */
/*                  function.                                         */
/*                                                                    */
/*                                                                    */
/* IN-PARAMETERS              :                                       */
/*                                                                    */
/* OUT-PARAMETERS             :                                       */
/*                                                                    */
/* RETURN VALUES              :                                       */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  none                                 */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

sp1266_hexDumpAll (1, (char FAR * FAR *)&pPtr, (unsigned long int FAR *)&lLen, 0); 
}/*end*/                                                                        
