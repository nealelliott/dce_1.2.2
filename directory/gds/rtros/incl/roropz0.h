/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropz0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:21:02  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:53  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:36:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:59  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:54  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:36  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:31  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:58:11  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:34  keutel
 * 	creation
 * 	[1994/03/21  13:27:46  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROROPZ0_H
#define _ROROPZ0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roropz0_rcsid[] = { "@(#) $RCSfile: roropz0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:21:02 $" } ;
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
/* NAME         : roropz0.h                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-06                                            */
/*                                                                    */
/* COMPONENT    : Remote Operation Servive  protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RTROS 3.00                                          */
/*                                                                    */
/* DESCRIPTION  : Contains the data structures and definitions        */
/*                related to the RO-INFORM functions.                 */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropz0.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 2.00   K0 |93-04-06|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


#define RC07z0_PossibilityInfo       (RC0700_RoseEvent | 0x0001)


typedef struct rS07z0_stPossiblityInfo {
    signed   long  int       lEvent;       /*  -> REQ or RSP service     */
                                           /*     events in rortpm0.h    */
    signed   long  int       lInvokeId;    /*  -> Invoke Id for OPERATION*/
    signed   short int       bPossible;    /* <-  Possible or not.       */
    unsigned long  int       ulAcb;        /* -> Association Id          */
} rT07z0_stPossibilityInfo;

#endif	/* _ROROPZ0_H */
