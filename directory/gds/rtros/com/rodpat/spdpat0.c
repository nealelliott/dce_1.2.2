/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spdpat0.c,v $
 * Revision 1.1.4.2  1996/02/18  18:22:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:45:23  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:30  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:20  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:06  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:20  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:12  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:30:41  keutel
 * 	creation
 * 	[1994/03/21  13:17:35  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spdpat0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:22:30 $" } ;
#endif

/**********************************************************************/
/*                                                                    */
/*    COPYRIGHT  (C)						      */
/*    Siemens Nixdorf Informationssysteme AG 1991		      */
/*    All rights reserved					      */
/*								      */
/*    This software is furnished under licence and may be used only   */
/*    in accordance with the terms of that licence and with the       */
/*    inclusion of the above copyright notice.			      */
/*    This software may not be provided or otherwise made available   */
/*    to, or used by, any other person. No title to or ownership of   */
/*    the software is hereby transferred.			      */
/*                                                                    */
/**********************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME 	: spdpat0.c					      */
/*                                                                    */
/* AUTHOR	: Natalia Serra 				      */
/* DATE 	: 13-03-92					      */
/*                                                                    */
/* COMPONENT    : RTS Evaluation Program                              */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : In this module the trace record types are evaluated */
/*                                                                    */
/* HISTORY	:						      */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 2.0A00 K00| 13-03-92| original		       | ns |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spdpat0.c 1.1 93/11/29   RTS-V2.00";

/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>	  /* General RTS definitons		      */
#include <rolgval.h>	  /* General RTS definitons		      */

#include <spstep0.h>	  /* RTS-STEP General definitions	      */
#include <spdpat0.h>	  /* Function prototyping		      */

/**********************************************************************/
/*                                                                    */
/*              D E F I N E S                                         */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

        
/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME 	: sp0200_traceDpat()				      */
/*                                                                    */
/* AUTHOR	: Natalia Serra 				      */
/* DATE 	: 13-03-92					      */
/*                                                                    */
/* SYNTAX       :                                                     */
/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS: type, time, argn, argl, argv			      */
/*                                                                    */
/* OUT-PARAMETERS:   none                                             */
/*                                                                    */
/* RETURN VALUES :   none                                             */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/*                                                                    */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  Information is printed on stdout     */
/*                                                                    */
/* Vers.Nr.  | Date    | Changes                       |mark| CR# EM# */
/* 2.0A00 K00| 13-03-92| original		       | ns |	      */
/*           |         |                               |    |         */
/*inoff ***************************************************************/

signed short int sp0200_traceDpat (
long int	               type,
long int	               time,
unsigned short int             argn,
char               FAR * FAR * argv,
unsigned long int        FAR * argl,
unsigned short int             nerr)

{
   /*-----------------------------------------------------------------*/ 
   /* START                                                           */
   /*-----------------------------------------------------------------*/

   return (OK);
}


/*********************	    END OF SPDPAT0.C	  *********************/
