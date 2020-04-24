/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rotime3.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:19  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:12:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:20  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:13  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:44  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:49  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:49:56  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:08  keutel
 * 	creation
 * 	[1994/03/21  13:19:30  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROTIME3_H
#define _ROTIME3_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rotime3_rcsid[] = { "@(#) $RCSfile: rotime3.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:04 $" } ;
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
/* NAME         : rotime3.h                                           */
/*                                                                    */
/* AUTHOR       : Natalia Serra                                       */
/* DATE         : 19-9-91                                             */
/*                                                                    */
/* COMPONENT    :                                                     */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION	: Private structure's definition                      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rotime3.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  | Date    | changes                       |mark| CR# EM# */
/* 2.0A00 K00| 91-09-19| original                      |ns  |         */
/*           |         |                               |    |         */
/*datoff **************************************************************/


/* ****************** Timer invocation control structure ************ */
/* ******************                                    ************ */

typedef struct rS1100_ctrlTimer {
   unsigned long int   ulTimeDelay;
   unsigned long int   hACB;
   unsigned long int   hECB;
   unsigned long int   ulModuleNum;
   unsigned long int   ulTimerInvo;
   unsigned long int   hNext;
} rT1100_ctrlTimer;

#endif	/* _ROTIME3_H */
