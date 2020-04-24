/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spstep22.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:32  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:06  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:05  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:14:24  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:36  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:42  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:09:01  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:18  keutel
 * 	creation
 * 	[1994/03/21  13:37:41  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: spstep22.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:32 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * Copyright(C)                                                    */
/*  * Siemens Nixdorf Informationssysteme AG 1992                    */
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
/* NAME         : spstep2.c                                           */
/*                                                                    */
/* AUTHOR       : Monika Krause                                       */
/* DATE         : 04.03.92                                            */
/*                                                                    */
/* COMPONENT    : RTS Evaluation Program                              */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : This file provides a module which can be linked     */
/*                to the step program for logging ROS PDUs            */
/* IN-PARAMETERS:                                                     */
/*    type: record type of the logging record                         */
/*    time: at this time was the logging record written               */
/*    argn: Numbers of parameters by writing the logging record       */
/*    argv: content of the parameter                                  */
/*    argl: length of the parameter                                   */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) spstep22.c 1.1 93/11/29   RTS-V2.00";

/**********************************************************************/
/*                                                                    */
/*              I N C L U D E S                                       */
/*                                                                    */
/**********************************************************************/

/* common RTS-includes ************************************************/
#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <rogenrl.h>
#include <rolgval.h>

/* special RTS-includes for LOGGING ***********************************/
#include <spstep0.h>
#include <spstep1.h>
#include <sputil0.h>
#include <sproif0.h>
#include <spdpat0.h>
#include <sploif02.h>
#include <spprpm0.h>
#include <splctr0.h>
#include <spacpm0.h>
#include <spropm0.h>
#include <spuctr02.h>
#include <spspm0.h>
#include <spcmx01.h>
#include <sptime0.h>



/**********************************************************************/
/*                                                                    */
/*            G L O B A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : sp1220_writeLogRec()                                */
/*                                                                    */
/* AUTHOR       : Monika Krause                                       */
/* DATE         : 24/01/92                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int PASCAL sp1220_writeLogRec (
   signed   long  int             type,
   signed   long  int             time,
   unsigned short int             argn,
   char               FAR * FAR * argv,
   unsigned long  int       FAR * argl,
   unsigned short int             nerr)

/*                                                                    */
/* DESCRIPTION  : This function calls depending on the modulnumber    */
/*                programs, which evaluate the giving parameters      */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*    type: record type of the logging record                         */
/*    time: at this time was the logging record written               */
/*    argn: Numbers of parameters by writing the logging record       */
/*    argv: content of the parameter                                  */
/*    argl: length of the parameter                                   */
/*                                                                    */
/* OUT-PARAMETERS:  the same                                          */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/

/* ****************** l o c a l    v a r i a b l e s **************** */
/* ******************                                **************** */

signed long  int module;
signed short int    rcc;

/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

 module  = type;
 module &= RLro10_ModulNumber;

 switch (module)
  {
   case RL0000_ROUTIL: rcc = sp0000_traceutil (type, time, argn, argv, argl, nerr);
		       break;

   case RL1300_ROEXIF: rcc = sp1300_traceExif (type, time, argn, argv, argl, nerr);
		       break;

   case RL0200_RODPAT: rcc = sp0200_traceDpat (type, time, argn, argv, argl, nerr);
		       break;

   case RL0400_ROPRPM: rcc = sp0400_tracePrpm (type, time, argn, argv, argl, nerr);
		       break;

   case RL0500_ROLCTR: rcc = sp0500_traceLctr (type, time, argn, argv, argl, nerr);
		       break;

   case RL0600_ROACPM: rcc = sp0600_traceAcpm (type, time, argn, argv, argl, nerr);
		       break;

   case RL0700_ROROPM: rcc = sp0700_traceRopm (type, time, argn, argv, argl, nerr);
		       break;

   case RL0800_ROSEPM: rcc = sp0800_traceSpm (type, time, argn, argv, argl, nerr);
		       break;

   case RL1500_ROUCTR: rcc = sp1500_traceUctr (type, time, argn, argv, argl, nerr);
		       break;

   case RL1100_ROTIME: rcc = sp1100_traceTime (type, time, argn, argv, argl, nerr);
		       break;
   case RL2000_CMX: rcc = sp0300_traceCmx (type, time, argn, argv, argl, nerr);
		       break;
   case RL0300_ROLOIF: rcc = sp0300_traceLoif (type, time, argn, argv, argl, nerr);
		       break;

   default           : /* calling hexdump */
		       sp1251_recordHead(RC1200_OFFSET0,
					"R_UNKNOWN",
					"(no_rts)",
					type, time, 0 );
		       sp1266_hexDumpAll(argn, argv, argl,0);
		       rcc = 0;
		       break;
  }
return (rcc);

}/*end*/
