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
 * $Log: cma_ux_redefs.h,v $
 * Revision 1.1.8.2  1996/02/18  23:09:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:33:18  marty]
 *
 * Revision 1.1.8.1  1995/12/07  21:55:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:18  root]
 * 
 * Revision 1.1.6.1  1994/06/09  13:36:55  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:01  devsrc]
 * 
 * Revision 1.1.4.2  1993/06/24  20:51:05  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:43:15  hinman]
 * 
 * Revision 1.1.2.2  1993/02/01  20:14:42  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  20:03:02  hinman]
 * 
 * $EndLog$
 */
#ifndef R_UX_REDEFS
#define R_UX_REDEFS

#undef fork
#ifdef __STDC__ 
#pragma weak fork = _fork
#define cma_fork _fork
#else /* no __STDC__ */
#define cma_fork fork
#endif /* __STDC__ */

#undef execve
#ifdef __STDC__ 
#pragma weak execve = _execve
#define cma_execve _execve
#else /* no __STDC__ */
#define cma_execve execve
#endif /* __STDC__ */


#endif /*  R_UX_REDEFS */
