/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roexif02.h,v $
 * Revision 1.1.4.2  1996/03/11  13:22:11  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:10:54  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:21:51  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:10:05  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:52:12  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:01:22  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:02:50  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:38:46  keutel
 * 	creation
 * 	[1994/03/21  13:32:23  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROEXIF02_H
#define _ROEXIF02_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roexif02_rcsid[] = { "@(#) $RCSfile: roexif02.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:11 $" } ;
#endif

/* *********************************************************************** */
/*                                                                         */
/*  * COPYRIGHT  (C)                                                       */
/*  * Siemens Nixdorf Informationssysteme AG 1993                          */
/*  * All rights reserved                                                  */
/*  *                                                                      */
/*  * This software is furnished under licence and may be used only        */
/*  * in accordance with the terms of that licence and with the            */
/*  * inclusion of the above copyright notice.                             */
/*  * This software may not be provided or otherwise made available        */
/*  * to, or used by, any other person. No title to or ownership of        */
/*  * the software is hereby  transferred.                                 */
/*                                                                         */
/* *********************************************************************** */

/*daton ****************************************************************** */
/*                                                                         */
/* TYPE         : INCLUDE                                                  */
/*                                                                         */
/* NAME         : roexif02.h                                               */
/*                                                                         */
/* AUTHOR       : Chandrasekar.K.                                          */
/* DATE         : 93.02.23                                                 */
/*                                                                         */
/* COMPONENT    : rts external interface intern include file               */
/*                                                                         */
/* PRD#/VERS.   : RTS-V3.00                                                */
/*                                                                         */
/* DESCRIPTION  :                                                          */
/*                                                                         */
/* SYSTEM DEPENDENCE :                                                     */
/*                                                                         */
/*                                                                         */
/*                                                                         */
/* HISTORY      : sccsid = " ";                                            */
/*                                                                         */
/* Vers.Nr. | Date    |  changes                       |mark| CR# EM       */
/* 3.00 K0  | 93-02-23|  original                      |kc  |              */
/*          |         |                                |    |              */
/*datoff ***************************************************************** */

/* *********************************************************************** */
/*                                                                         */
/*                    Function prototypes.                                 */
/*                                                                         */
/* *********************************************************************** */


signed long int FAR PASCAL ro1320_taskId PROTO ((void));
FARPROC         FAR PASCAL ro1321_ssiMpi PROTO ((FARPROC, signed long int));

#endif	/* _ROEXIF02_H */
