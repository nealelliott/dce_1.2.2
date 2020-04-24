/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil3.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:12:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:23  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:15  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:47  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:50  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:50:03  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:11  keutel
 * 	creation
 * 	[1994/03/21  13:19:43  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUTIL3_H
#define _ROUTIL3_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _routil3_rcsid[] = { "@(#) $RCSfile: routil3.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:06 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1992 by                                                  *
*  * SIEMENS NIXDORF INFORMATIONSSYSTEME AG                                 *
*  * All rights reserved                                                    *
*  *                                                                        *
*  * This software is furnished under licence and may be used only in       *
*  * accordance with the terms of that licence and with the inclusion of    *
*  * the above copyright notice. This software may not be provided or       *
*  * otherwise made available to, or used by, any other person. No title    *
*  * to or ownership of the software is hereby transferred.                 *
*                                                                           *
\***************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : INCLUDE                                             */
/*                                                                    */
/* NAME         : routil3.h                                           */
/*                                                                    */
/* AUTHOR       : Gerhard Jahn                                        */
/* DATE         : 11-JUN-1992                                         */
/*                                                                    */
/* COMPONENT    : RTS common utilities                                */
/*                                                                    */
/* PRD#/VERS.   : RTS-V02.00                                          */
/*                                                                    */
/* DESCRIPTION  : RTS common utilities                                */
/*                                                                    */
/*             - Logging system facility                              */
/*                                                                    */
/* SYSTEM DEPENDENCE :  no                                            */
/*                                                                    */
/* HISTORY      : sccsid= "@(#) routil3.h 1.1 93/11/29"                           */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/*   1.00   | 11-06-92|  original                      | gj |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************  RTS-V2.0 Logging cfg-filename  ***************** */
#define  RTS_LOGCFGNAME  "rts_log.cfg" /* name for logging configuration */

/* ****************  RTS-V2.0 Logging configuration  **************** */
typedef struct {
	unsigned short int   loglevel;
        unsigned short int   recnummax;
	char                 filename[120];
	unsigned short int   confreadmax;
} rs0004_configpar;

#endif	/* _ROUTIL3_H */
