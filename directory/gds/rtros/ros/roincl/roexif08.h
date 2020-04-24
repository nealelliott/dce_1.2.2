/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif08.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:17  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:02  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:10:22  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:18  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:28  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:07  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:52  keutel
 * 	creation
 * 	[1994/03/21  13:32:35  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROEXIF08_H
#define _ROEXIF08_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roexif08_rcsid[] = { "@(#) $RCSfile: roexif08.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:17 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1992                     */
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
/* NAME         : roexif08.h                                          */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-24                                            */
/*                                                                    */
/* COMPONENT    : Remote Opeartions service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Protocol machine service interface                  */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roexif08.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-03-24|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/* ************ Function Proto Types for Logging Functions ********** */


	signed short int FAR PASCAL ro1340_Logging PROTO(( 
             RO_excopb      FAR * ));

        signed short int NEAR PASCAL ro1341_LogOn PROTO((
             RO_lgonpb       FAR * ));

        signed short int NEAR PASCAL ro1342_LogOff PROTO((void));

        signed short int NEAR PASCAL ro1343_LogWrite PROTO((
             RO_lgwrpb       FAR * ));

#endif	/* _ROEXIF08_H */
