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
 * $Log: cma_io_redefs_4.h,v $
 * Revision 1.1.8.2  1996/02/18  23:09:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:33:10  marty]
 *
 * Revision 1.1.8.1  1995/12/07  21:55:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:10  root]
 * 
 * Revision 1.1.6.1  1994/06/09  13:36:51  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:58  devsrc]
 * 
 * Revision 1.1.4.2  1993/06/24  20:50:55  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:43:08  hinman]
 * 
 * Revision 1.1.2.2  1993/02/01  20:14:21  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  20:02:14  hinman]
 * 
 * $EndLog$
 */
#ifndef R_IO_REDEFS_4
#define R_IO_REDEFS_4

#undef poll
#ifdef __STDC__
#pragma weak poll = _poll
#pragma weak _abi_poll = _poll
#define cma_poll _poll
#else /* no __STDC__ */
#define cma_poll poll
#endif /* __STDC__ */

#endif /*  R_IO_REDEFS_4 */

