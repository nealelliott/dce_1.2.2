/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rolctr3.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:50  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:54  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:21  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:31  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:52  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:39  keutel
 * 	creation
 * 	[1994/03/21  13:19:04  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROLCTR3_H
#define _ROLCTR3_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rolctr3_rcsid[] = { "@(#) $RCSfile: rolctr3.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:44 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONS AG	      */
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
/* NAME 	: rolcrt3.h					      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-15					      */
/*                                                                    */
/* COMPONENT	: RTS dispatcher				      */
/*								      */
/* PRD#/VERS.	: RTS-V03.00					      */
/*                                                                    */
/* DESCRIPTION	: Rts association control function internal	      */
/*		  definitions					      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rolctr3.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 02.00 K0 | 91-10-30|  original		       |jm  |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ***************** ASO-control function state ********************* */
/* ***************** values			********************* */

#define RC0530_Idle            0 
#define RC0531_Active          1 

typedef struct rS0530_stCxt { 
    signed   short int      sState;
    signed   short int      sApCxtId;
    unsigned short int      usAutNo;  
}rT0530_stCxt;

#endif	/* _ROLCTR3_H */
