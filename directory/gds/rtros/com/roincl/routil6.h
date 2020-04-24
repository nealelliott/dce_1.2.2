/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: routil6.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:29  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:12:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:19:26  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:05:42  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:49  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:52  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:50:09  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:32:13  keutel
 * 	creation
 * 	[1994/03/21  13:17:54  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROUTIL6_H
#define _ROUTIL6_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _routil6_rcsid[] = { "@(#) $RCSfile: routil6.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:08 $" } ;
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
/* NAME         : routil6.h                                           */
/*                                                                    */
/* AUTHOR       : Chandrasekar.K.                                     */
/* DATE         : 93-04-20                                            */
/*                                                                    */
/* COMPONENT    : ROS common  loggin utilities                        */
/*                                                                    */
/* PRD#/VERS.   : RTROS-V3.00                                         */
/*                                                                    */
/* DESCRIPTION  : ROS logging utilities                               */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      : sccsid= "@(#) routil6.h 1.1 93/11/29"                           */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/*  3.00    |93-04-20 |  original                      |kc  |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/



typedef struct rS0060_stLogRecHead{
   long int lLength;
   long int lRecordType;
   long int lWriteTime;                        /* - Logging record*/
} rT0060_LogRecHead;                               /*   header        */

/* ************* Functions of roloif0.c calling session  ************ */

signed short int FAR PASCAL ro0060_OpenLog PROTO((
signed         char       FAR * fileName,
unsigned short int              fileMode,
unsigned short int              options,
unsigned long  int        FAR * hFileHandle,
void                FAR * FAR * lpOpenBuff));

       /*   = 0   sucessfully                       */
       /*   < 0   error                             */
       /*   > 0   not defined, length equal to NULL */

signed short int FAR PASCAL ro0061_CloseLog PROTO ((
unsigned long  int       hFileHandle,
void               FAR * lpOpenBuff,
signed   short int FAR * pbCloseAll));
       /*   = 0   sucessfully                       */
       /*   < 0   error                             */
       /*   > 0   not defined, length equal to NULL */

signed short int FAR PASCAL ro0062_WriteLog PROTO ((
unsigned long int        hFileHandle,
void               FAR * lpOpenBuff,
rT0060_LogRecHead  FAR * pHeader,
char               FAR * pRecord));
       /*   = 0   sucessfully                       */
       /*   < 0   error                             */
       /*   > 0   not defined, length equal to NULL */


/* ****************************************************************** */

#endif	/* _ROUTIL6_H */
