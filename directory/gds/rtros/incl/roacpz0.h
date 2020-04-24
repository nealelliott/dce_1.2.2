/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roacpz0.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:31  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:33:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:02  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:30  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:05  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:04  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:56:43  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:08  keutel
 * 	creation
 * 	[1994/03/21  13:26:12  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROACPZ0_H
#define _ROACPZ0_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roacpz0_rcsid[] = { "@(#) $RCSfile: roacpz0.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:31 $" } ;
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
/* NAME         : roacpmz0.h                                          */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-03-30                                            */
/*                                                                    */
/* COMPONENT    : ACSE protocol machine                               */
/*                                                                    */
/* PRD#/VERS.   : ROS-V3.0                                            */
/*                                                                    */
/* DESCRIPTION  : Contains the data structures and definitions        */
/*                related to the ACSE-INFORM functions.               */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roacpz0.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-03-30|  original                      |kc  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

#define RC06z0_AcpmInform            (0x0700)
#define RC06z1_ApplCxtName           (RC06z0_AcpmInform | (0x0001))
#define RC06z2_ApplCxtID             (RC06z0_AcpmInform | (0x0002))
#define RC06z3_CheckApplCxtIds       (RC06z0_AcpmInform | (0x0003))
#define RC06z4_CheckLegal            (RC06z0_AcpmInform | (0x0004))


typedef struct rS06z0_stInfrmApplCxtName{      
    rTro00_vObjectId     vApplCxtName;            
    signed   short int   sApplCxtId;              
    unsigned long int    ulApplID;                
    unsigned long int    ulAssId;
}rT06z0_stInfrmApplCxtName;                    

#endif	/* _ROACPZ0_H */
