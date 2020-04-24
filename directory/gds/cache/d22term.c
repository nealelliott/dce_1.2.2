/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d22term.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:03  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:28:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:36  root]
 * 
 * Revision 1.1.8.3  1994/09/06  12:25:08  keutel
 * 	line 1 directive removed: OT 11971
 * 	[1994/09/06  11:14:16  keutel]
 * 
 * Revision 1.1.8.2  1994/05/10  15:51:26  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:17:37  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:40  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:55:24  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  07:48:51  marrek
 * 	July 1993 code drop.
 * 	[1993/08/02  12:16:04  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:42:07  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:14:23  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:31:42  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:38:30  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d22term.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:02 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d22term.c                                           */
/*								      */
/* AUTHOR       : R. Horn, D AP 11                                    */
/* DATE         : 10.05.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*exon ***********************************************************************/
/*                                                                           */
/*  FUNCTION:  d22_terminate ()                                              */
/*                                                                           */
/*  SHORT DESCRIPTION:                                                       */
/*     The function (signal routine) is called if a signal SIGTERM is        */
/*     send to the DUA-Cachehandler process. The function sets a process     */
/*     termination flag which is tested within the main routine. If the      */
/*     termination flag is set, the process terminates by securing the       */
/*     consistency of the cache files.                                       */
/*                                                                           */
/*  INPUT-PARAMETER:                                                         */
/*     signo = No. of occurred signal                                        */
/*                                                                           */
/*  OUTPUT-PARAMETER:                                                        */
/*                    None                                                   */
/*                                                                           */
/*  RETURN-VALUE:                                                            */
/*                    None                                                   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*  AUTHOR: R.Horn                                         DATE: 31.3.88     */
/*exoff **********************************************************************/

#include <gds.h>
#include <dce/d2dir.h>
#include <d23ipc.h>
#include <d22cache.h>


/* ----------------- declaration of global data ---------------------------- */

extern boolean d22_flgterm ;	/* process termination flag */

/* ------------------------------------------------------------------------- */

void d22_terminate (int signo) {
	/* service request in work -> set process termination flag */
	d22_flgterm = TRUE ;
}
