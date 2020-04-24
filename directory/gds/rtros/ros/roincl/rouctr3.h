/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rouctr3.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:37  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:11:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:41:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:22:34  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:11:55  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:38  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:46  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:04:09  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:39:21  keutel
 * 	creation
 * 	[1994/03/21  13:33:24  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUCTR3_H
#define _ROUCTR3_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _rouctr3_rcsid[] = { "@(#) $RCSfile: rouctr3.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:37 $" } ;
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
/* NAME         : rouctr32.h                                          */
/*                                                                    */
/* AUTHOR       : Vinita                                              */
/* DATE         : 15-2-93                                             */
/*                                                                    */
/* COMPONENT    :                                                     */
/*                                                                    */
/* PRD#/VERS.   : RORTS-V3.0                                          */
/*                                                                    */
/* DESCRIPTION	: Private structure's definition                      */
/*                                                                    */
/* SYSTEM DEPENDENCE :  none                                          */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)rouctr3.h	1.1 94/01/26                                  */
/*                                                                    */
/* Vers.Nr.  | Date    | changes                       |mark| CR# EM# */
/* 2.0A00 K00| 91-09-19| original                      |vp  |         */
/*           |         |                               |    |         */
/*datoff **************************************************************/


/* ************** ASO Upper Control Context ************************* */
/* ******************                      ************************** */
                                   
typedef struct rS1530_stAsoContext {
   unsigned long  int 	hAsscHandle;
   unsigned long  int   hOperHandle;
} rT1530_stAsoContext;

/*
  - hAsscHandle : handle for event data structure used for 
                   bind & unbind (structure type is rT1504_stEvData)
  - hOperHandle : handle event data structure used for invoke
                   contains a link list (structure type is 
                   rT1531_stInvokeData)

*/


typedef struct rS1531_stInvokeData { 
   unsigned long int    hEvData; 
   unsigned long int    ulInvokeId; 
   unsigned long int    hNext;
}rT1531_stInvokeData;

/*
  - stEvdata   : event data structure used for bind & unbind 
  - ulInvokeId : Invoke Identifier
  - hNext      : handle for next Invoke Data
                 Value of '0' would indicate no further elements in
                 the queue 
*/

#endif	/* _ROUCTR3_H */
