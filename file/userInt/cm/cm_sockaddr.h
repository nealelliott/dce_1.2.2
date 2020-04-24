/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cm_sockaddr.h,v $
 * Revision 1.1.9.1  1996/10/02  21:09:48  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:24  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:24:09  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:36:07  annie]
 * 
 * Revision 1.1.2.5  1993/01/21  16:29:08  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:22:28  cjd]
 * 
 * Revision 1.1.2.4  1992/11/24  20:40:24  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:35:57  bolinger]
 * 
 * Revision 1.1.2.3  1992/09/25  19:44:25  jaffe
 * 	Cleanup Minor header differences
 * 	[1992/09/24  15:46:46  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  21:53:41  jaffe
 * 	Transarc delta: bab-ot3716-cm-kernel-sockaddr 1.4
 * 	  Selected comments:
 * 	    Our pioctls on OSF/1 pass kernel sockaddrs (that include a length field)
 * 	    to user-space programs.  The cm user interface code needs to accomodate this
 * 	    (though this really should be changed inside the kernel...the kernel should
 * 	    be doing this translation).
 * 	    ot 3716
 * 	    Defined macro to zero length field of kernel-type sockaddrs.
 * 	    Forgot to byte-swap the length and family fields on machines that
 * 	    need it.
 * 	    Byte swap the length and family fields, in addition to zeroing the length
 * 	    field, on machines that need it.
 * 	    Move this modification into the kernel, where it belongs.
 * 	    Debugging.
 * 	[1992/08/30  13:36:45  jaffe]
 * 
 * $EndLog$
 */
/*
 *      Copyright (C) 1992 Transarc Corporation
 *      All rights reserved.
 */

/*
 *	cm_sockaddr.h -- routines to interconvert between kernel and
 *  user-space representations of sockaddrs for machines that need it.
 *  Currently, the only machine known to need it is OSF/1.
 *
 *    The problem that this macro solves really should be considered a bug
 *  in the port of our kernel pioctls.
 */

#ifndef TRANSARC_USERINT_CM_SOCKADDR_H
#define TRANSARC_USERINT_CM_SOCKADDR_H 1

#include <dcedfs/param.h>

/* the following structures describe the different headers possible for the sockaddrs */

/* this is what comes out of the kernel on OSF/1 */
typedef struct cm_sockaddr_with_len {
  u_char	sa_len;
  u_char	sa_family;
} cm_sockaddr_with_len_t;

/*
 * This seems to be what everyone else uses, it's really only useful at this point
 * for purposes of illustration.
 */
typedef struct cm_sockaddr_no_len {
  u_short	sa_family;
} cm_sockaddr_no_len_t;

#if defined(AFS_OSF_ENV)
/* this one applies to any other byte-swapped environment, also */
#define cm_sockaddr_ClearLength(kernelSockaddrP) \
  ((cm_sockaddr_no_len_t *)(kernelSockaddrP))->sa_family >>= 8;	\
  ((cm_sockaddr_no_len_t *)(kernelSockaddrP))->sa_family &= 0xff
#else /* defined(AFS_OSF_ENV) */
#define cm_sockaddr_ClearLength(kernelSockaddrP) \
  ((cm_sockaddr_with_len_t *)(kernelSockaddrP))->sa_len = 0
#endif /* defined(AFS_OSF_ENV) */

#endif /* TRANSARC_USERINT_CM_SOCKADDR_H */
