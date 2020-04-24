/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_service_ultrix.h,v $
 * Revision 1.1.6.2  1996/02/18  23:12:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:27  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:14:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:59  root]
 * 
 * Revision 1.1.4.1  1994/04/21  22:06:10  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:59:57  cbrooks]
 * 
 * Revision 1.1.2.2  1992/12/30  20:42:54  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:22:51  htf]
 * 
 * Revision 1.1  1992/01/19  15:34:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module DTSS_SERVICE_ULTRIX.H
 *	Version X0.6
 */
/*
 * Copyright (c) 1989 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:    This is the VMS-specific header file for the DTSS_SERVICE
 *		module.
 *
 * ENVIRONMENT:	Unix/Ultrix
 *
 * AUTHORS:
 *		Cesar Cortes
 *		Distributed Processing Engineering (DPE)
 *
 * DATE: 	August 1989
 *
 */

#ifndef _DTSS_SERVICE_ULTRIX_H
#define _DTSS_SERVICE_ULTRIX_H 1

/*
 * include ULTRIX-specific headers from the system library
 * Note that there must be no <> or "" around the library names
 */

typedef int FDmask;

#define FDMASK_INIT (0);

StatusBlk RecvMsgGeneric(EventBlk *eblkPtr);

EventBlk *GetNewEblk (PortDsc *sockptr);

EventBlk *MakeEblk (PortDsc *sockptr);

#endif /* ifdef DTSS_SERVICE_ULTRIX.H */

/*  DEC/CMS REPLACEMENT HISTORY, Element DTSS_SERVICE_VMS.H */
/*  *4     2-AUG-1989 09:25:03 C_CORTES "Change file formats from variable length to Stream_LF" */
/*  *3    18-MAY-1989 09:55:39 NICHOLS "move all EFN defn's to ...EVENT_VMS.C" */
/*  *2    17-FEB-1989 11:02:51 NICHOLS "fix includes" */
/*  *1    13-FEB-1989 16:40:13 NICHOLS "VMS-specific defns for the DTSS service program" */
/*  DEC/CMS REPLACEMENT HISTORY, Element DTSS_SERVICE_VMS.H */
/*  DEC/CMS REPLACEMENT HISTORY, Element DTSS_SERVICE_ULTRIX.H */
/*  *3    16-JUL-1991 11:54:55 COMUZZI "Fixed protoype problem" */
/*  *2    16-JUL-1991 11:51:57 COMUZZI "Switched to new-style proto's" */
/*  *1     8-FEB-1990 12:37:58 NICHOLS "Rebuilt after Corruption" */
/*  DEC/CMS REPLACEMENT HISTORY, Element DTSS_SERVICE_ULTRIX.H */
