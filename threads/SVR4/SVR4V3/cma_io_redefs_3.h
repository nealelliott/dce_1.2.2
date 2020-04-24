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
 * $Log: cma_io_redefs_3.h,v $
 * Revision 1.1.6.2  1996/02/18  23:09:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:33:08  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:55:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:07  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:36:49  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:57  devsrc]
 * 
 * Revision 1.1.2.2  1993/02/01  20:14:13  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  20:01:59  hinman]
 * 
 * $EndLog$
 */
#ifndef R_IO_REDEFS_3
#define R_IO_REDEFS_3

#undef  creat
#ifdef __STDC__
#pragma weak creat = _creat
#define cma_creat  _creat
#else /* no __STDC__ */
#define cma_creat  creat
#endif /* __STDC__ */

#undef  dup
#ifdef __STDC__
#pragma weak dup = _dup
#define cma_dup  _dup
#else /* no __STDC__ */
#define cma_dup  dup
#endif /* __STDC__ */

/*
 * cma_fcntl is called by the fcntl implementation in libsocket.
 * If libsocket is not linked cma_fcntl is used directly.
 */
#ifdef __STDC__
#pragma weak fcntl = cma_fcntl
#pragma weak _fcntl = cma_fcntl
#else /* no __STDC__ */
cma_io_redefs_3.h: local implementation of fcntl needs ANSI compiler
#endif /* __STDC__ */

#undef  pipe
#ifdef __STDC__
#pragma weak pipe = _pipe
#define cma_pipe   _pipe
#else /* no __STDC__ */
#define cma_pipe   pipe
#endif /* __STDC__ */


#endif /* R_IO_REDEFS_3 */
