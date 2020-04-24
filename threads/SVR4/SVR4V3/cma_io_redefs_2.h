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
 * $Log: cma_io_redefs_2.h,v $
 * Revision 1.1.8.2  1996/02/18  23:09:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:33:05  marty]
 *
 * Revision 1.1.8.1  1995/12/07  21:55:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:03:04  root]
 * 
 * Revision 1.1.6.1  1994/06/09  13:36:48  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:56  devsrc]
 * 
 * Revision 1.1.4.2  1993/07/19  18:41:18  blurie
 * 	Fix for OT #8287 (need a cma wrapper for ioctl(...I_RECVFD...)).
 * 
 * 		Add SVR4 wrapper for cma_ioctl().
 * 	[1993/07/19  18:15:38  blurie]
 * 
 * Revision 1.1.2.2  1993/02/01  20:14:06  hinman
 * 	[hinman] - SVR4 changes
 * 	[1993/02/01  20:01:39  hinman]
 * 
 * $EndLog$
 */
#ifndef R_IO_REDEFS_2
#define R_IO_REDEFS_2

#undef  close
#ifdef __STDC__
#pragma weak close = _close
#define cma_close _close
#else /* no __STDC__ */
#define cma_close close
#endif /* __STDC__ */

#undef  open
#ifdef __STDC__
#pragma weak open = _open
#define cma_open _open
#else /* no __STDC__ */
#define cma_open open
#endif /* __STDC__ */

#undef  read
#ifdef __STDC__
#pragma weak read = _read
#define cma_read _read
#else /* no __STDC__ */
#define cma_read read
#endif /* __STDC__ */

#undef  readv
#ifdef __STDC__
#pragma weak readv = _readv
#define cma_readv _readv
#else /* no __STDC__ */
#define cma_readv readv
#endif /* __STDC__ */

#undef  write
#ifdef __STDC__
#pragma weak write = _write
#define cma_write _write
#else /* no __STDC__ */
#define cma_write write
#endif /* __STDC__ */

#undef  writev
#ifdef __STDC__
#pragma weak writev = _writev
#define cma_writev _writev
#else /* no __STDC__ */
#define cma_writev writev
#endif /* __STDC__ */

#undef  getmsg
#ifdef __STDC__
#pragma weak getmsg = _getmsg
#define cma_getmsg _getmsg
#else /* no __STDC__ */
#define cma_getmsg getmsg
#endif /* __STDC__ */

#undef  getpmsg
#ifdef __STDC__
#pragma weak getpmsg = _getpmsg
#define cma_getpmsg _getpmsg
#else /* no __STDC__ */
#define cma_getpmsg getpmsg
#endif /* __STDC__ */

#undef  putmsg
#ifdef __STDC__
#pragma weak putmsg = _putmsg
#define cma_putmsg _putmsg
#else /* no __STDC__ */
#define cma_putmsg putmsg
#endif /* __STDC__ */

#undef  putpmsg
#ifdef __STDC__
#pragma weak putpmsg = _putpmsg
#define cma_putpmsg _putpmsg
#else /* no __STDC__ */
#define cma_putpmsg putpmsg
#endif /* __STDC__ */

/*
 * cma_ioctl is called by the ioctl implementation in libsocket.
 * If libsocket is not linked cma_ioctl is used directly.
 */

#ifdef __STDC__
#pragma weak ioctl = cma_ioctl
#pragma weak _ioctl = cma_ioctl
#else /* no __STDC__ */
cma_io_redefs_2.h: local implementation of ioctl needs ANSI compiler
#endif /* __STDC__ */


#endif /* R_IO_REDEFS_2 */

