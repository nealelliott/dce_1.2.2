/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sputil0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:41  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:26  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:38:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:01  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:24  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:00:04  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:00  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:53  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:21  keutel
 * 	creation
 * 	[1994/03/21  13:29:06  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPUTIL0_H
#define _SPUTIL0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _sputil0_rcsid[] = { "@(#) $RCSfile: sputil0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:41 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * Copyright(C)                                                    */
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

/*daton **************************************************************/
/*                                                                   */
/* TYPE         : MODULE                                             */
/*                                                                   */
/* NAME         : sputil0.h                                          */
/*                                                                   */
/* AUTHOR       : Monika Krause                                      */
/* DATE         : 09/03/92                                           */
/*                                                                   */
/* COMPONENT    : ROSTEP / DOSTEP                                    */
/*                                                                   */
/* PRD#/VERS.   : RTS V2.0                                           */
/*                                                                   */
/* DESCRIPTION  :                                                    */
/*                                                                   */
/* SYSTEM DEPENDENCE :  none                                         */
/*                                                                   */
/*datoff *************************************************************/


signed short int sp0000_traceutil PROTO   ((
                                  signed long    int             type,
				  signed long    int             time,
				  unsigned short int             argn,
				  char                FAR * FAR *argv,
				  unsigned long  int        FAR *argl,
				  unsigned short int	         err));

#endif	/* _SPUTIL0_H */
