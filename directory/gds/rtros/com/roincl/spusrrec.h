/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: spusrrec.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:12:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:31  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:44  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:52  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:55  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:50:16  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:19  keutel
 * 	creation
 * 	[1994/03/21  13:18:31  keutel]
 * 
 * $EndLog$
 */
#ifndef _SPUSRREC_H
#define _SPUSRREC_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _spusrrec_rcsid[] = { "@(#) $RCSfile: spusrrec.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:11 $" } ;
#endif

/**********************************************************************/
/*                                                                    */
/*    COPYRIGHT  (C)						      */
/*    Siemens Nixdorf Informationssysteme AG 1993		      */
/*    All rights reserved					      */
/*								      */
/*    This software is furnished under licence and may be used only   */
/*    in accordance with the terms of that licence and with the       */
/*    inclusion of the above copyright notice.			      */
/*    This software may not be provided or otherwise made available   */
/*    to, or used by, any other person. No title to or ownership of   */
/*    the software is hereby transferred.			      */
/*                                                                    */
/**********************************************************************/

/*daton ***************************************************************/
/*								      */
/* TYPE         : INCLUDE                                             */
/*								      */
/* NAME 	: spusrrec.h      				      */
/*								      */
/* AUTHOR	: Shivkumar       		                      */
/* DATE 	: 12-11-93					      */
/*								      */
/* COMPONENT	: RTS / STEP					      */
/*								      */
/* PRD#/VERS.	: RTS-V2.00					      */
/*								      */
/* DESCRIPTION	: RTS-STEP General definitions and functions	      */
/*		  prototyping					      */
/*								      */
/* SYSTEM DEPENDENCE :	none					      */
/*								      */
/* USED PRODUCTS:   RTS V2.00					      */
/*		    CES V5.2   C-Entwicklungssystem		      */
/*								      */
/*								      */
/* Vers.Nr. | Date    | Changes 		       |mark| CR# EM# */
/* 03.00 K00| 12-11-93| original		       | skh|	      */
/*	    |	      | 			       |    |	      */
/*datoff **************************************************************/

void r_evalusrrec PROTO ((
signed long int       lType,
signed long           lLen,
char            FAR * pPtr));

#endif	/* _SPUSRREC_H */
