/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roloif9.h,v $
 * Revision 1.1.4.2  1996/03/11  13:20:48  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:09:43  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:35:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:19:30  root]
 * 
 * Revision 1.1.2.3  1994/06/13  15:05:43  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/13  14:59:19  devsrc]
 * 
 * 	cr10871 - fix copyright
 * 
 * Revision 1.1.2.2  1994/05/10  16:00:20  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:57:33  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:35:29  keutel
 * 	creation
 * 	[1994/03/21  13:26:51  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROLOIF9_H
#define _ROLOIF9_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roloif9_rcsid[] = { "@(#) $RCSfile: roloif9.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:20:48 $" } ;
#endif

/* ****************************************************************** */
/*                                                                    */
/*  * COPYRIGHT  (C)                                                  */
/*  * Siemens Nixdorf Informationssysteme AG 1990                     */
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
/* NAME         : roloif9.h                                           */
/*                                                                    */
/* AUTHOR       : Monika Krause                                       */
/* DATE         : 91-11-08                                            */
/*                                                                    */
/* COMPONENT    : ROS common utilities   internal                     */
/*                                                                    */
/* PRD#/VERS.   : RTS-V2.0                                            */
/*                                                                    */
/* DESCRIPTION  : ROS common utilities   internal variables           */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCE :                                                */
/*                                                                    */
/* HISTORY      : sccsid= "@(#) roloif9.h 1.1 93/11/29"                           */
/*                                                                    */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM# */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


#define RC0029_RosLogType           1020  /* used session logging type     */
                                          /* if rts writes an logging      */
                                          /* record into session's logging */
                                          /* file                          */

typedef struct {
   long int rs0001_rlength;
   long int rs0001_recType;
   long int rs0001_wrtTime;                        /* - Logging record*/
} RS0001_logRecHead;                               /*   header        */

/* ************* Functions of roloif0.c calling session  ************ */

signed short int PASCAL ro0340_openLog PROTO ((char FAR*,
					       unsigned short int,
					       unsigned short int));
       /*   = 0   sucessfully                       */
       /*   < 0   error                             */
       /*   > 0   not defined, length equal to NULL */

signed short int PASCAL ro0341_closeLog PROTO ((void));
       /*   = 0   sucessfully                       */
       /*   < 0   error                             */
       /*   > 0   not defined, length equal to NULL */

signed short int PASCAL ro0342_writeLog PROTO ((RS0001_logRecHead FAR*,
							     char FAR*));
       /*   = 0   sucessfully                       */
       /*   < 0   error                             */
       /*   > 0   not defined, length equal to NULL */


/* ****************************************************************** */
/* ****************************************************************** */

#endif	/* _ROLOIF9_H */
