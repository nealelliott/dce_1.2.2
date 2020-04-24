/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sputil0.c,v $
 * Revision 1.1.4.2  1996/03/11  13:20:26  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:25  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:33:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:18:50  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:08:30  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:50:17  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:59:59  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:56:24  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:34:57  keutel
 * 	creation
 * 	[1994/03/21  13:25:56  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = { "@(#) $RCSfile: sputil0.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:26 $" } ;
#endif

/***********************************************************************/
/*                                                                     */
/*    Copyright(C)                                                     */
/*    Siemens Nixdorf Informationssysteme AG 1992                      */
/*    All rights reserved                                              */
/*                                                                     */
/*    This software is furnished under licence and may be used only    */
/*    in accordance with the terms of that licence and with the        */
/*    inclusion of the above copyright notice.                         */
/*    This software may not be provided or otherwise made available    */
/*    to, or used by, any other person. No title to or ownership of    */
/*    the software is hereby transfered.                               */
/*                                                                     */
/***********************************************************************/

/*daton ************************************************************* */
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : sputil0.c                                           */
/*                                                                    */
/* AUTHOR       : Monika Krause                                       */
/* DATE         : 20.01.92                                            */
/*                                                                    */
/* COMPONENT    : RTS Evaluation Program                              */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : In this module the trace record types are evaluated */
/*                                                                    */
/*                                                                    */
/*datoff **************************************************************/
/**********************************************************************/
/*                                                                    */
/*                     Module Identification                          */
/*                                                                    */
/**********************************************************************/

static char *sccsid =    "@(#) sputil0.c 1.1 93/11/29   RTS-V2.00";

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
#include <sputil0.h>
#include <spstep0.h>

/**********************************************************************/
/*                                                                    */
/*            G L O B A L  F U N C T I O N S                          */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : sp0000_traceutil()                                  */
/*                                                                    */
/* AUTHOR       : Monika Krause                                       */
/* DATE         : 24/01/92                                            */
/*                                                                    */
/* SYNTAX       :                                                     */

signed short int sp0000_traceutil (
	signed long int                 type,
	signed long int                 time,
	unsigned short int	        argn,
	char                FAR * FAR * argv,
	unsigned long int         FAR * argl,
	unsigned short int              errCount)

/*                                                                    */
/* DESCRIPTION  : Evaluation of the trace logging records             */
/*                                                                    */
/* IN-PARAMETERS:                                                     */
/*    type: record type of the logging record                         */
/*    time: at this time was the logging record written               */
/*    argn: Numbers of parameters by writing the logging record       */
/*    argv: content of the parameter                                  */
/*    argl: length of the parameter                                   */
/*                                                                    */
/* OUT-PARAMETERS:   none                                             */
/* RETURN VALUES :   rcc                                              */
/*                                                                    */
/* GLOBAL DATA   ( read only ):  none                                 */
/* GLOBAL DATA   ( modified ) :  none                                 */
/*                                                                    */
/* REMARKS                    :  Information is printed on stdout     */
/*                                                                    */
/*inoff ***************************************************************/

{/*entr*/
/* ****************** S T A R T ************************************* */
/* ******************           ************************************* */

type &= RLro20_TypeField;

switch (type) {

/*
 * ==> trace function entry
 */
 case (RL0001_FctTraceEntry & RLro20_TypeField) :
 sp1251_recordHead(RC1200_OFFSET0      ,
		   "R_TRACE"           ,
		   "(entry)"           ,
		     type              ,
		     time                ,
		     errCount           );


/*
 * 1. function name
 */
	if (argl[0] > 0) {
	    sp1264_write(RC1202_OFFSET2, "Function: %s", argv[0], 0, 0, 0);
	}
	break;
        
/*
 * ==> trace function exit
 */
    case (RL0002_FctTraceReturn & RLro20_TypeField):
	sp1251_recordHead(RC1200_OFFSET0   ,
		       "R_TRACE"           ,
		       "(exit)            ",
		       type                ,
		       time                ,
		       errCount           );

/*
 * 1. function name  and 2. returncode
 */
	if (argl[0] > 0) {
	sp1264_write(RC1202_OFFSET2			   ,
		    "Function: %s          Return_code: %d",
		     argv[0]				   ,
		     argv[1]				   ,
		     0					   ,
		     0					   );
	}
	break;
   }
   return (0);
}/*end*/
