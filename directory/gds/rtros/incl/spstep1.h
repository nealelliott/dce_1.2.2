/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spstep1.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:35  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:22  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:38:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:53  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:20  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  15:00:01  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:56  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:59:44  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:37:14  keutel
 * 	creation
 * 	[1994/03/21  13:28:59  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPSTEP1_H
#define _SPSTEP1_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _spstep1_rcsid[] = { "@(#) $RCSfile: spstep1.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:35 $" } ;
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
/* NAME         : spstep1.h                                          */
/*                                                                   */
/* AUTHOR	: Monika Krause 				     */
/* DATE         : 09/03/92                                           */
/*                                                                   */
/* COMPONENT    : RTSTEP                                             */
/*                                                                   */
/* PRD#/VERS.   : RTS V2.0                                           */
/*                                                                   */
/* DESCRIPTION  :                                                    */
/*                                                                   */
/* SYSTEM DEPENDENCE :  none                                         */
/*                                                                   */
/*datoff *************************************************************/


int usr_ros PROTO ((unsigned char FAR *rcrd,
		    unsigned long	len));

signed short int PASCAL sp1220_writeLogRec PROTO ((
                                      signed   long  int            type,
				      signed   long  int            time,
				      unsigned short int            argn,
				      char               FAR * FAR *argv,
				      unsigned long  int       FAR *argl,
				      unsigned short int            nerr));

signed short int PASCAL sp1225_writeUserRec PROTO ((
                                      signed   long  int            type,
				      signed   long  int	    time,
				      unsigned short int	    argn,
				      char               FAR * FAR *argv,
				      unsigned long  int       FAR *argl,
				      unsigned short int	    nerr));

#endif	/* _SPSTEP1_H */
