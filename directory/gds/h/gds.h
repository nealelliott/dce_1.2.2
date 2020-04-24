/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gds.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:01:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:21  root]
 * 
 * Revision 1.1.2.4  1994/09/19  08:39:38  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:33:50  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:46:21  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:25  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:56:06  marrek
 * 	Bug fixes.
 * 	[1994/04/29  09:28:46  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:38:54  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:19  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:11:30  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:52:44  marrek]
 * 
 * $EndLog$
 */
/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* Filename     : gds.h                                               */
/*                                                                    */
/* Description  : This File includes other include files, that are    */
/*                required for the GDS for purposes of integration    */
/*                to DCE (thread safety, serviceability, code cleanup)*/
/*                                                                    */
/* Date         : 12/07/1993                                          */
/*                                                                    */
/* AUTHOR       : E. Kraemer, BU BA NM12                              */
/*                                                                    */
/**********************************************************************/

#ifndef _GDS_H
#define _GDS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gds_rcsid[] = "@(#)$RCSfile: gds.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:11 $";
#endif

#ifdef THREADSAFE
#ifndef PTHREAD
# include <pthread.h>
#endif
#endif

#ifndef _DCE_H
# include <dce/dce.h>
#endif
#include <dce/dce_msg.h>
#include <dce/dcegdssvc.h>
#include <dce/dcegdsmsg.h>
#include <dce/dcegdsmac.h>
#include <d27svc.h>

#endif

