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
 * $Log: cma_stdio_redefs.h,v $
 * Revision 1.1.6.2  1996/02/18  23:09:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:33:16  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:55:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:16  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:36:53  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:27:00  devsrc]
 * 
 * Revision 1.1.2.2  1993/02/01  20:14:35  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  20:02:47  hinman]
 * 
 * $EndLog$
 */
#ifndef  R_STDIO_REDEFS
#define  R_STDIO_REDEFS

#undef system
#ifdef __STDC__
# pragma weak system = _system
# define cma_system _system
#else
# define cma_system system
#endif

#undef popen
#ifdef __STDC__
# pragma weak popen = _popen
# define cma_popen  _popen
#else
# define cma_popen popen
#endif

#endif /* R_STDIO_REDEFS */
