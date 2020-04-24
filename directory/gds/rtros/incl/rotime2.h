/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rotime2.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:13  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:36:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:13  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:02  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:43  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:38  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:45  keutel
 * 	creation
 * 	[1994/03/21  13:28:06  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROTIME2_H
#define _ROTIME2_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rotime2_rcsid[] = { "@(#) $RCSfile: rotime2.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:13 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1991                     */
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
/* NAME         : rotime2.h                                           */
/*                                                                    */
/* AUTHOR       : Natalia Serra                                       */
/* DATE         : 19-9-91                                             */
/*                                                                    */
/* COMPONENT    :                                                     */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION	: Timer function's prototyping                        */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rotime2.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  | Date    | changes                       |mark| CR# EM# */
/* 2.0A00 K00| 19-09-91| original                      |ns  |         */
/* 2.0A00 K00| 04-12-91| add clearTimerQueue() function|ns  |	      */
/*           |         |                               |    |         */
/*datoff **************************************************************/


/*--------------------------------------------------------------------*/
/*                                                                    */
/* FUNCTION   : ro1100_timer()                                        */
/*                                                                    */
/* DESCRIPTION: This function is similar to a PM module.              */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed short int PASCAL ro1100_timer PROTO ((unsigned long int));

/* Return value == 0 : successful				      */
/*		!= 0 : error					      */
/* Parameter 1	     : timer context handle			      */


/*--------------------------------------------------------------------*/
/*                                                                    */
/* FUNCTION   : ro1101_cleanUp()                                      */
/*                                                                    */
/* DESCRIPTION: This function is similar to all the cleanUp functions */
/*		but in the Timer PM module, nothing is done.	      */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed short int PASCAL ro1101_cleanUp PROTO ((signed long int,
					       unsigned long int,
					       unsigned long int,
					       unsigned long int,
					       unsigned long int));

/* Return value == 0 : successful				      */
/*		!= 0 : error					      */
/* Parameter 1	     : <- pm event				      */
/* Parameter 2	     : <- pm context handle or p1		      */
/* Parameter 3	     : <- p2					      */
/* Parameter 4	     : <- p3					      */
/* Parameter 5	     : <- p4					      */


/*--------------------------------------------------------------------*/
/*                                                                    */
/* FUNCTION   : ro1102_checkTimer()                                   */
/*                                                                    */
/* DESCRIPTION: This function checks the list of timer calls.         */
/*              It returns whether one timer block has expired or     */
/*		not yet (or an error, if it occurs).		      */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed short int PASCAL ro1102_checkTimer PROTO ((unsigned long int FAR *,
						  signed long int FAR *));

/* Return value == 0 : One timer bloc has expired		      */
/*		 > 0 : No timer bloc has expired yet		      */
/*		 < 0 : Error					      */
/* Parameter 1	     : -> ACB handle				      */
/* Parameter 2	     : -> pm event				      */


/*--------------------------------------------------------------------*/
/*                                                                    */
/* FUNCTION   : ro1103_clearTimerQueue()			      */
/*                                                                    */
/* DESCRIPTION: This function frees all the elements of the list      */
/*              of calls to the timer belonging to a specific         */
/*		ECB (which is the parameter of the function).	      */
/*                                                                    */
/*                                                                    */
/*--------------------------------------------------------------------*/

signed short int PASCAL ro1103_clearTimerQueue PROTO ((unsigned long int));

/* Return value == 0 : successful				      */
/*		!= 0 : error					      */
/* Parameter 1	     : <- ECB handle				      */


/************************   END OF ROTIME2.H   ************************/

#endif	/* _ROTIME2_H */
