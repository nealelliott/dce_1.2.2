/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdscommon.h,v $
 * Revision 1.1.10.2  1996/02/18  23:37:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:27  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:36:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:19  root]
 * 
 * Revision 1.1.8.1  1994/02/22  19:23:52  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:00  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:02:20  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:10:16  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:19:50  marrek]
 * 
 * Revision 1.1.4.2  1992/12/30  23:30:21  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  23:20:27  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:16:16  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:43:18  melman]
 * 
 * $EndLog$
 */
#ifndef _XDSCOMMON_H
#define _XDSCOMMON_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdscommon_rcsid[] = "@(#)$RCSfile: xdscommon.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:37:51 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xdscommon.h                                         */
/*                                                                    */
/* AUTHOR       : V. Ryan                                             */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XDS                                                 */
/*                                                                    */
/* DOK.-REF.    :                                                     */ 
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : manifest constants                                  */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   1.0    | 90-10-01| Original                       | VR |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/


#ifndef  XDSCOMMON_HEADER
#define  XDSCOMMON_HEADER


#ifndef NULL
#define NULL    0
#endif

#ifndef FALSE
#define FALSE    0
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef OK
#define OK    0
#endif

#ifndef NOTOK
#define NOTOK    -1
#endif

#ifndef DONE
#define DONE    1
#endif


#endif  /* XDSCOMMON_HEADER */

#endif /* _XDSCOMMON_H */
