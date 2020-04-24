/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rouctr4.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:19  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:05  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:36:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:20:23  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:06:06  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:48  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:42  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:53  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:51  keutel
 * 	creation
 * 	[1994/03/21  13:28:17  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUCTR4_H
#define _ROUCTR4_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rouctr4_rcsid[] = { "@(#) $RCSfile: rouctr4.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:19 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1991                     */
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
/* NAME         : rouctr4.h                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Vinita                              */
/* DATE         : 93-03-18                                            */
/*                                                                    */
/* COMPONENT    : ROS appliction entity manager                       */
/*                                                                    */
/* PRD#/VERS.   : ROS-V3.00                                           */
/*                                                                    */
/* DESCRIPTION  : ROS application entity manager invocation context   */
/*                interface                                           */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rouctr4.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/* 3.00 K0  | 93-03-18|  original                      |vp  |         */
/*                                                                    */
/*datoff **************************************************************/

#define RC0570_MaxApplCtxtInfos  8

/* ******************** mode selector values ******************************** */
#define RC0571_NormalMode        0
#define RC0572_SniPrivate        4

/* ************************************************************************** */
/* Application Context Information structure, one per Application Context     */
/* Name. One such Application-Context is selected during an ATTACH.           */
/* ************************************************************************** */
typedef struct rS0500_stApplCtxtInfo {
    rTro00_vObjectId      oApplCtxtName;
    unsigned short int    usAseConfig;
    signed long  int      lModeSelector;
    rTro60_vAbstrSynId    vAbstrSynId;  
} rT0500_stApplCtxtInfo;

/* ************************************************************************** */
/* Application Entity Invocation: A new structure created on every ATTACH.    */
/* Contains the Calling-Application-Entity description and the list of        */
/* Application-Contexts acceptable for incoming (and obviously, outgoing)     */
/* associations.                                                              */
/*									      */
/* Note: - OwnPSAP is stored in ECB					      */
/*	 - The Calling-Application-Entity-Title is stored in local heap       */
/*	   memory. stOwnAeTitle.hHanOfSub[0] contains a handle which points   */
/*	   to the Calling-Application-Entity-Title.			      */
/*	 - stOwnAeTitle.uLenOfSub[0] contains the length of the memory area   */
/*	   which contains the Calling Application-Entity-Title. 	      */
/*	 - stOwnAeTitle.uBasLength must be equal to			      */
/*	   "sizeof (rT0501_AEInvocation)"				      */
/*	 - All further variables of the structure "rT0200_rdbHeader"	      */
/*	   must contain NULL.						      */
/*									      */
/* ************************************************************************** */

typedef struct rS0501_stAeInvocation {
    rT0200_rdbHeader	  stOwnAeTitle;
    rTro74_stInvoc        stOwnInvoc;
    unsigned short int    usOffer;
    signed   short int    sNumbOfApplCtxts;
    rT0500_stApplCtxtInfo vApplCtxtInfos [RC0570_MaxApplCtxtInfos];
} rT0501_AEInvocation;

#endif	/* _ROUCTR4_H */
