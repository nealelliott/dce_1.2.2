/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omVersion.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:04  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:41:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:11  root]
 * 
 * Revision 1.1.8.1  1994/02/22  19:00:47  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:08:58  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:25:13  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:27:50  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:40  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:07:42  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:42:53  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:49:27  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:22:52  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:51:39  melman]
 * 
 * $EndLog$
 */

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omVersion.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:45 $";
#endif

char xom_version[] = "@(#)libXOM 1.9 (BULL S.A) 7/1/92";

