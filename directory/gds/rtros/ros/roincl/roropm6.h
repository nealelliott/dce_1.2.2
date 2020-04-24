/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roropm6.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:25  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:04  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:14  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:28  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:26  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:35  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:03:30  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:04  keutel
 * 	creation
 * 	[1994/03/21  13:33:01  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROROPM6_H
#define _ROROPM6_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roropm6_rcsid[] = { "@(#) $RCSfile: roropm6.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:25 $" } ;
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
/* NAME         : roropm6.h                                           */
/*                                                                    */
/* AUTHOR       : Shivkumar, H.                                       */
/* DATE         : 93-07-22                                            */
/*                                                                    */
/* COMPONENT    : Remote Operation  service protocol machine          */
/*                                                                    */
/* PRD#/VERS.   : ROS-V3.0                                            */
/*                                                                    */
/* DESCRIPTION  : Internally used non-Automata related prototypes.    */
/*                                                                    */
/* SYSTEM DEPENDENCIES : None                                         */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roropm6.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* ----------|--------|--------------------------------|----|-------- */
/* 3.00   K0 |93-07-22|  original                      |skh |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/

signed  short int NEAR PASCAL ro0707_GiveProviderRejInd PROTO ((
                                         rT0730_stContext   NEAR * pContext,
                                         signed   long  int FAR  * plOutEvent,
                                         signed   long  int        lReason,
                                         unsigned long  int FAR  * phP1,
                                         unsigned long  int FAR  * phP2,
                                         unsigned long  int FAR  * phP3,
                                         unsigned long  int FAR  * phP4));

signed short int PASCAL ro0708_ProviderAbort PROTO ((
        rT0730_stContext   NEAR  *,       
        signed   long  int        ,
        signed         int        ,
        unsigned long  int        ,           
        unsigned long  int        ,          
        unsigned long  int        ,         
        unsigned long  int        ));        

signed short int PASCAL ro0709_ProviderReject PROTO ((
        rT0730_stContext   NEAR  *,       
        signed   long  int FAR   *,
        signed   long  int FAR   *,
        unsigned long  int FAR   *,           
        unsigned long  int FAR   *,          
        unsigned long  int FAR   *,         
        unsigned long  int FAR   *,        
        signed   short int        ,
        signed   short int        ,
        signed   long  int        ));

signed short int PASCAL ro070v_GetFromQueue PROTO ((
        rT0730_stContext   NEAR  *,       
        signed   long  int FAR   *,
        unsigned long  int FAR   *,           
        unsigned long  int FAR   *,          
        unsigned long  int FAR   *,         
        unsigned long  int FAR   *));        

signed short int PASCAL ro070u_PutInQueue PROTO ((
        rT0730_stContext   NEAR *,       
        signed   long  int   ,
        unsigned long  int   ,          
        unsigned long  int   ,          
        unsigned long  int   ,         
        unsigned long  int  ));        
                                
signed short int PASCAL ro070s_GetRtseAbsId PROTO (( 
        signed short int      * ));

#endif	/* _ROROPM6_H */
