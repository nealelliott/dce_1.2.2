/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tpdu1.h,v $
 * Revision 1.1.4.2  1996/03/11  13:23:46  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:17  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:28  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:15:35  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:53:51  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:02:55  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:09:54  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:46:41  keutel
 * 	creation
 * 	[1994/03/21  13:38:32  keutel]
 * 
 * $EndLog$
 */
#ifndef _TPDU1_H
#define _TPDU1_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _tpdu1_rcsid[] = { "@(#) $RCSfile: tpdu1.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:46 $" } ;
#endif

/***************************************************************************\
*                                                                           *
*  * Copyright (c) 1993 by                                                  *
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

/*daton	***************************************************************/
/*								      */
/* TYPE		: MODULE					      */
/*								      */
/* NAME         : tpdu1.h                                              */
/*								      */
/* AUTHOR       : Baiju Dalal, Chandru                                 */
/* DATE         : 17-02-94                                            */
/*								      */
/* COMPONENT    : ROS Tpdu functions Prototypes                        */
/*								      */
/* PRD#/VERS.   : ROS-V03.00                                          */
/*								      */
/* DESCRIPTION	: This module is an implementation of the             */
/*		  TPDU interface (Virtual File Access Method).        */
/*								      */
/*		  Uses the original TPDU implementation of CoCoNet.   */
/*								      */
/* SYSTEM DEPENDENCE :	none					      */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. | Date    |	 Modification		       |mark| CR# EM# */
/*  03.00   |17-02-94 |  original                      | bd|         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


void PASCAL tp_strcpy  PROTO  ((
	char FAR *destination,
	char FAR *source));

unsigned long int PASCAL tp_fopen  PROTO  ((
	char  FAR  *fname,
	char  FAR  *fmode));


signed short int  PASCAL tp_fclose PROTO  ((
	unsigned long int   fp));

signed long int PASCAL tp_fseek PROTO  ((
	unsigned long int   fp,
	long int            offset,
	int                 mapptr));

signed short int  PASCAL tp_fwrite PROTO  ((
	void     FAR       *  ptr,
	unsigned long int     size,
	unsigned long int     numb,
	unsigned long int     fp));

void  PASCAL tp_fflush PROTO  ((
	unsigned long int   fp));

signed long int  PASCAL tp_fread PROTO  ((
	void     FAR       *  ptr,
	unsigned long int     size,
	unsigned long int     numb,
	unsigned long int     fp));


signed long int PASCAL tp_filelen  PROTO  ((
	unsigned long int fp));


#endif	/* _TPDU1_H */
