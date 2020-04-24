/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: Trace.h,v $
 * Revision 1.1.10.2  1996/02/18  23:35:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:12  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:59:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:13  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:07:30  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:42:11  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:10:25  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:36:33  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:38:41  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:28:29  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  03:42:41  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:40:36  zeliff]
 * 
 * $EndLog$
 */
#ifndef _TRACE_H
#define _TRACE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char Trace_rcsid[] = "@(#)$RCSfile: Trace.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:35:19 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/* Traceinformation output on stderr 2.9.87 A.S. */
/* printf (...) ---> fprintf (stderr, ...)       */
/* stdout       ---> stderr                      */



#ifdef         TRACE
static char *__tform__ = "upid=%d file=%s line=%d : " ;
#define        trace(x)\
   {\
   fprintf (stderr, __tform__ , getpid () , __FILE__ , __LINE__ ) ;\
   fprintf x ;\
   fprintf (stderr, "\n" ) ;\
   fflush ( stderr ) ;\
   }
#else
#define        trace(x)
#endif


#endif /* _TRACE_H */
