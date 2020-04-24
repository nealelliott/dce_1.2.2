/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roppm04.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:54  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:47  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:41  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:48  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:24  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:25  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:51  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:36:21  keutel
 * 	creation
 * 	[1994/03/21  13:27:07  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROPPM04_H
#define _ROPPM04_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roppm04_rcsid[] = { "@(#) $RCSfile: roppm04.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:54 $" } ;
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
/* NAME         : roppm04.h                                           */
/*                                                                    */
/* AUTHOR       : Johann Mueller, Harald Miersch                      */
/* DATE         : 92-02-21                                            */
/*                                                                    */
/* COMPONENT    : RTS presentation protocol machine                   */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : Presentation entity invocation context              */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)roppm04.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  |Date    |  changes                       |mark| CR# EM# */
/* 2.0A00 K00|92-02-21|  adaptation of ROS V2.5A00 K05 |hm  |         */
/*           |        |                                |    |         */
/*datoff **************************************************************/


/* ****************** Abstract syntax related information *********** */
/* ******************                                     *********** */

#define RC0440_MaxDecodeControl  8    /* max. number of PM numbers in the */
                                      /* Decode Control Array             */
#define RC0441_NoPmNumb         -1    /* used for not defined elements of */
                                      /* the Decode Control Array         */
                                      /* (for online-troubleshooting)     */

typedef struct rS0440_stAbstrSyn {
   rTro00_vObjectId       oAbstrSynName;
   signed   short int     vTransSynId [RCro03_MaxTransPerAbstr + 1];
   signed   short int     vPmNumb     [RC0440_MaxDecodeControl];
} rT0440_stAbstrSyn;

/*
   - oAbstrSynName  : Object identifier which identifies the abstract syntax.
                      The first element contains the number of components
                      (if NULL: syntax not defined).
   - vTransSynId    : Array of transfer syntax identifiers (TransId's).
                      The TransId's are used as index in the array of transfer
                      syntax object identifiers (see rT0441_EntContext).
                      Defines the transfer syntaxes which are proposed for
                      the abstract syntax.
                      The first element contains the number of components
                      (if NULL: error).
   - vPmNumb        : Decode Control Array.
                      Array of protocol machine numbers. Used by the PPM Deco-
                      der to find out which PM belongs to the next abstract
                      syntax within the named abstract syntax. (The PPM Deco-
                      gets by its caller the actual index into the array.) 
                      Use of the array elements:
                      0: PM corresponding to the first abstract syntax of the
                         named abstract syntax.
                         Use is mandatory.
                      1: PM corresponding to the next abstract syntax if the
                         beginning tag of the named abstract syntax is
                         legal-unknown within the first abstract syntax.
                         Used only for the RTSE including abstract syntax
                         in the current implementation.
                      2: reserved
                      3...(RC0440_MaxDecodeControl - 1):
                         PM corresponding to the next abstract syntax if
                         an ANY occurs after the begin of the named abstract
                         syntax. The "ANY number" defined at compilation time
                         is the index of the array element.
                         Used only for the RTSE including abstract syntax
                         in the current implementation.
                      Not defined elements have the value defined above.
*/


/* ****************** Presentation entity invocation context ******** */
/* ****************** (PEIcontext)                           ******** */

typedef struct rS0441_stEntContext {
   rT0440_stAbstrSyn      vAbstrSyn     [RCro01_MaxAbstrSyn];
   rTro00_vObjectId       vTransSynName [RCro02_MaxTransSyn];
} rT0441_stEntContext;

/*
   - vAbstrSyn      : Array of abstract syntax related informations.
                      Array index: Abstract syntax identifier (AbsId).
   - vTransSynName  : Array of transfer syntax object identifiers.
                      Array index: Transfer syntax identifier (TransId).
*/

#endif	/* _ROPPM04_H */
