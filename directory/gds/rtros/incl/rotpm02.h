/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rotpm02.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:16  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:36:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:17  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:04  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:45  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:39  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:44  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:47  keutel
 * 	creation
 * 	[1994/03/21  13:28:10  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROTPM02_H
#define _ROTPM02_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rotpm02_rcsid[] = { "@(#) $RCSfile: rotpm02.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:16 $" } ;
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
/* NAME         : rotpm00.h                                           */
/*                                                                    */
/* AUTHOR       : H. Shivkumar                                        */
/* DATE         : 93-07-26                                            */
/*                                                                    */
/* COMPONENT    : ROS Transprot Protocol Machine                      */
/*                                                                    */
/* PRD#/VERS.   : ROS-V03.00                                          */
/*                                                                    */
/* DESCRIPTION  : Protocol Machine Access Interface.                  */
/*                                                                    */
/* SYSTEM DEPENDENCE : none                                           */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rotpm02.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  Modification                  |mark| CR# FM# */
/* 03.00 K00| 93-07-26|  original                      |skh |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


signed short int PASCAL ro0900_tpm PROTO ((unsigned long int hContext));


signed short int PASCAL ro0901_cleanUp PROTO  ((
                                    signed long int Event,
                                    unsigned long int par1,
                                    unsigned long int par2,
                                    unsigned long int par3,
                                    unsigned long int par4));

#endif	/* _ROTPM02_H */
