/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roloif42.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:20  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:01  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:07  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:10:28  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:21  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:31  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:15  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:56  keutel
 * 	creation
 * 	[1994/03/21  13:32:45  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROLOIF42_H
#define _ROLOIF42_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roloif42_rcsid[] = { "@(#) $RCSfile: roloif42.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:20 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1990                     */
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
/* NAME         : roloif10.h                                          */
/*                                                                    */
/* AUTHOR       : Shanbhag, Sanjay                                    */
/* DATE         : 93-03-09                                            */
/*                                                                    */
/* COMPONENT    : RTROS                                                 */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V03.00                                          */
/*                                                                    */
/* DESCRIPTION	: Function prototyping for the Lower Interface (local */
/*		  functions) and local definitions.		      */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  " @(#)roloif42.h 1.1 93/11/29"                        */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 03.00 K0 | 93-03-09|  original		       |sss |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/


#define DESTRUCT               1
#define STOP                   2


/* ***********************       SEND FUNCTION       ********************* */
/* ***********************                           ********************* */

signed short int PASCAL ro0350_Send PROTO ((rT0300_roloifContext *));

/* Return value == 0 : success                                             */
/*               < 0 : error                                               */

/* P1:   lower interface context */



/* **********************      RECEIVE FUNCTION      ********************* */
/* **********************                            ********************* */

signed short int PASCAL ro0351_Receive PROTO ((rT0300_roloifContext *,
					       signed      long int *,
					unsigned long int,
					unsigned long int,
					unsigned long int,
					unsigned long int *,
					unsigned long int *,
					unsigned long int *,
					unsigned long int *));

/* Return value == 0	    : success					   */
/*		== DESTRUCT : destruct Pm invocation			   */
/*		 < 0	    : error					   */

/* P1:	 lower interface context */
/* P2:	 outgoing event 	 */
/* P3:	 OSS event		 */
/* P4:	 User data length	 */
/* P5:	 session connection id	 */
/* P6:	 event parameter P1	 */
/* P7:	 event parameter P2	 */
/* P8:	 event parameter P3	 */
/* P9:	 event parameter P4	 */



/* ***************      PROCESS RETURN VALUE FUNCTION      *************** */
/* ***************                                         *************** */

signed short int PASCAL ro0352_processReturnValue PROTO ((int,
						   rT0300_roloifContext *,
						   signed short int));

/* Return value == 0 : success                                             */
/*               < 0 : error                                               */

/* P1:	 return value to process		     */
/* P2:	 lower interface context		     */
/* P3:	 bool: it was a sending or a receiving event */



/* ****************	 WRITE DATA IN STORE FUNCTION	   *************** */
/* ****************					   *************** */

signed short int PASCAL ro0353_writeDataInStore PROTO ((int,
						 char *,
						 long,
						 int,
						 char,
						 unsigned long int *));

/* Return value == 0 : success                                           */
/*               < 0 : error                                             */

/* P1:	 session event			 */
/* P2:	 pt. to the data to be written	 */
/* P3:	 data length			 */
/* P4:	 session protokol version	 */
/* P5:	 more data for this store or not */
/* P6:	 return parameter: Store handle  */



/* ****************	 GET TRANSPORT ADDRESS FUNCTION	   *************** */
/* ****************					   *************** */

void PASCAL ro0354_getAddressWithoutPandSsel PROTO ((char *,
				              char **));

/* Return value == 0 : success                                             */
/*               < 0 : error                                               */

/* P1:	 <- pointer to the session address  */
/* P2:	 -> pt. to the address without PSEL */

#endif	/* _ROLOIF42_H */
