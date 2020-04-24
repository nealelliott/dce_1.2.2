/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 */
/*
 * HISTORY
 * $Log: cma_dummy_defs_2.h,v $
 * Revision 1.1.6.2  1996/02/18  23:09:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:57  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:55:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:02:56  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:36:42  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:54  devsrc]
 * 
 * Revision 1.1.2.2  1993/02/01  20:13:38  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  20:00:50  hinman]
 * 
 * $EndLog$
 */
#ifndef R_DUMMY_DEFS_2
#define R_DUMMY_DEFS_2

#define close __dummy_close

#define open  __dummy_open

#define read  __dummy_read

#define readv __dummy_readv

#define write __dummy_write

#define writev __dummy_writev

#define getmsg __dummy_getmsg

#define getpmsg __dummy_getpmsg

#define putmsg __dummy_putmsg

#define putpmsg __dummy_putpmsg


#endif /* R_DUMMYS_DEFS2*/

