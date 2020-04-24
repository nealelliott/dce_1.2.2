/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif07.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:16  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:00  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:10:19  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:17  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:27  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:04  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:51  keutel
 * 	creation
 * 	[1994/03/21  13:32:33  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROEXIF07_H
#define _ROEXIF07_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roexif07_rcsid[] = { "@(#) $RCSfile: roexif07.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:16 $" } ;
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
/* NAME         : roexif07.h                                          */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-10                                            */
/*                                                                    */
/* COMPONENT    : Remote Opeartions Service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION  : Data declarations to be used in the Inform          */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roexif07.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-03-10|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

/* ******************* Constants defined **************************** */
/* *******************                   **************************** */

#define RC1372_GetAbsId           4







/* **************** Structures used in these inform types ********* */
/* ****************                                       ********* */


typedef struct rS1371_stInfrmAbsId{
    unsigned long  ulAssoId;
    signed   long  lInvId;
    signed   int   sAbsId;
}rT1371_stInfrmAbsId;

#endif	/* _ROEXIF07_H */
