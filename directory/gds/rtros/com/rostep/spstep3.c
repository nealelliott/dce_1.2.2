/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spstep3.c,v $
 * Revision 1.1.4.2  1996/03/11  13:19:53  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:01  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:31:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:04  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:07:00  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:49:42  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:33  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:53:52  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:07  keutel
 * 	creation
 * 	[1994/03/21  13:23:34  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spstep3.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:53 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * Copyright(C)                                                    */
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


/*daton ************************************************************* */
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : spstep3.c                                           */
/*                                                                    */
/* AUTHOR       : Monika Krause                                       */
/* DATE         : 04.03.92                                            */
/*                                                                    */
/* COMPONENT    : RTS Evaluation Program                              */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : This file provides a module which can be linked     */
/*                to the step program for logging ROS PDUs            */
/*                                                                    */
/* HISTORY      :       sccsid = %@(#)spstep3.c	1.1% %94/01/26%                          */
/*                                                                    */
/*datoff **************************************************************/
/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spstep3.c 1.1 93/11/29   RTS-V2.00";

/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

/* common RTS-includes ************************************************/
#include <rogenrl.h>
#include <rolgval.h>

/* special RTS-includes for LOGGING ***********************************/
#include <spstep0.h>
#include <spstep1.h>
#include <spusrrec.h>

/**********************************************************************/
/*                                                                    */
/*              L O C A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : sp1225_writeUserRec()                               */
/*                                                                    */
/* AUTHOR       : Monika Krause                                       */
/* DATE         : 24/01/92                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL sp1225_writeUserRec (
   signed   long  int              type,
   signed   long  int              time,
   unsigned short int              argn,
   char                FAR * FAR * argv,
   unsigned long  int        FAR * argl,
   unsigned short int              nerr)

/*                                                                    */
/* DESCRIPTION  : This function make only a hexdump of the userdata   */
/*                The user can write here his own routine             */
/*                                                                    */
/* IN-PARAMETERS :                                                    */
/*    type: record type of the logging record                         */
/*    time: at this time was the logging record written               */
/*    argn: Numbers of parameters by writing the logging record       */
/*    argv: content of the parameter                                  */
/*    argl: length of the parameter                                   */
/*    nerr: number of not written records			      */
/*                                                                    */
/* REMARKS       :  Information is printed to stdout                  */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

sp1251_recordHead (RC1200_OFFSET0, "R_USER", "(no_rts)", type, time, 0);
/*
sp1266_hexDumpAll (argn, argv, argl, 0);  by kc for dir */
/* call user function which writes the user records */
r_evalusrrec( (long int) type,
	      (long) argl[0],
	      (char FAR *) argv[0] );


return (0);

}/*end*/
