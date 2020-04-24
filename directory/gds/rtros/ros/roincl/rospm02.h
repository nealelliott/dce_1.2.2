/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rospm02.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:30  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:41:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:22  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:41  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:31  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:39  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:44  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:13  keutel
 * 	creation
 * 	[1994/03/21  13:31:45  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROSPM02_H
#define _ROSPM02_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rospm02_rcsid[] = { "@(#) $RCSfile: rospm02.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:30 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1993                     */
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
/* NEW NAME     : rospm02.h                                           */
/*                                                                    */
/* AUTHOR       : Milind M. Aphale                                    */
/* DATE         : 93-04-02					      */
/*                                                                    */
/* COMPONENT    : RTROS session protocol machine                      */
/*                                                                    */
/* PRD#/VERS.	: RTROS-V3.00					      */
/*                                                                    */
/* DESCRIPTION	: Function call interface			      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  = @(#) rospm02.h 1.1 93/11/29 RTROS-V3.00               */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/*          |         |                                |    |         */
/* 3.00 K00 | 93-04-01|  original	       	       |ma  |	      */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ****************** External functions **************************** */
/* ******************                    **************************** */

signed short int FAR PASCAL ro0800_spm PROTO ((
	unsigned long int      /* hContext */
	));

signed short int FAR PASCAL ro0801_cleanUp PROTO ((
	signed   long int,     /* levent          */
	unsigned long int,     /* hP1orhContext   */
	unsigned long int,     /* hP2             */
	unsigned long int,     /* hP3             */
	unsigned long int      /* hP4             */
	));

signed short int FAR PASCAL ro0802_informSpm PROTO ((
	unsigned long  int,    /*  hContext       */        
	signed   short int,    /*  sInformType    */   
	void          FAR *    /*  pRequestedInfo */
	));

#endif	/* _ROSPM02_H */
