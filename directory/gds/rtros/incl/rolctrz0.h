/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rolctrz0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:44  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:34:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:22  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:40  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:16  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:17  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:22  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:25  keutel
 * 	creation
 * 	[1994/03/21  13:26:43  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROLCTRZ0_H
#define _ROLCTRZ0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rolctrz0_rcsid[] = { "@(#) $RCSfile: rolctrz0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:44 $" } ;
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
/* NAME 	: rolcrtz.h					      */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE 	: 93-03-30					      */
/*                                                                    */
/* COMPONENT	: RORTS Lower Control header                          */
/*								      */
/* PRD#/VERS.	: RTS-V03.00					      */
/*                                                                    */
/* DESCRIPTION	: Rts association control function inform  	      */
/*		  structure  					      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rolctrz0.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 02.00 K0 | 91-10-30|  original		       |vp  |	      */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/* ***************** ASO-control function state ********************* */
/* ***************** values			********************* */


#define RC05z0_LoctrInform   0x0600 
#define RC05z1_AplCxtCmp     (RC05z0_LoctrInform | 0x0001)
  

typedef struct rS05z0_stAplCxtCmp { 
    signed   short int      bAplCxtCmpRes;
    signed   short int      sApCxtId;
}rT05z0_stAplCxtCmp;

#endif	/* _ROLCTRZ0_H */
