/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * spoke.h -- 
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: spoke.h,v $
 * Revision 1.1.12.1  1996/10/17  18:18:09  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:57  damon]
 *
 * Revision 1.1.7.2  1993/07/19  19:40:11  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:30:47  zeliff]
 * 
 * Revision 1.1.5.3  1993/07/16  21:30:23  kissel
 * 	Get rid of the damn quotes in #include.
 * 	[1993/06/21  14:52:00  kissel]
 * 
 * Revision 1.1.2.3  1993/01/11  18:25:33  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:57:39  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  16:11:12  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  17:58:31  jaffe]
 * 
 * $EndLog$
 */
#ifndef _SPOKE_H_ENV_
#define _SPOKE_H_ENV_ 1

#include <queue.h>
#include <com.h>
#include <stub.h>

/* functions to turn FDs between internal and external forms.  Right
 * now, as per comment in open, we're really being bogus, and are
 * passing raw, unvalidated file descriptor #s over the net.
 */
#define wlr_MapOut(fd)		(fd)
#define wlr_MapIn(fd)		(fd)

/* prototypes  for stub.c */
extern wlr_ServerChmod _TAKES((struct wlr_call *cp, char *p, long mode));
extern wlr_ServerStat _TAKES((struct wlr_call *cp, char *p, struct stat *stp));
extern wlr_ServerOpen _TAKES((struct wlr_call *cp, char *p,
			      long flags, long mode, long *fdp));
extern wlr_ServerRead _TAKES((struct wlr_call *cp, long fd,
			      long offset, long len,
			      char *blockp, long *lenp));
extern wlr_ServerWrite _TAKES((struct wlr_call *callp, long fd,
			       long offset, long len, char *blockp,
			       long *lenp));
extern wlr_ServerFtrunc _TAKES((struct wlr_call *cp, long fd, long len));
extern wlr_ServerFstat _TAKES((struct wlr_call *cp, long fd, struct stat *stp));
extern wlr_ServerClose _TAKES((struct wlr_call *cp, long fd));
extern wlr_ServerTrunc _TAKES((struct wlr_call *cp, char *p, long len));
extern wlr_ServerRename _TAKES((struct wlr_call *cp, char *n1, char *n2));
extern wlr_ServerUnlink _TAKES((struct wlr_call *cp, char *p));
extern wrl_ServerLink _TAKES((struct wlr_call *cp, char *n1, char *n2));
extern wrl_ServerSymlink _TAKES((struct wlr_call *cp, char *n1, char *n2));
extern wlr_ServerMakedir _TAKES((struct wlr_call *cp, char *p, long mode));
extern wlr_ServerRmdir _TAKES((struct wlr_call *cp, char *p));
extern wlr_ServerUtimes _TAKES((struct wlr_call *cp, char *p, long mtimeSec,
		    long mtimeUsec, long atimeSec, long atimeUsec));
extern wlr_ServerReaddir _TAKES((struct wlr_call *cp,
		     long fd, long offset, long len,
		     char *blockp, long *lenp));
extern wlr_ServerReadlink _TAKES((struct wlr_call *cp, char *path, char *result,
			  long *len));
#endif /* _SPOKE_H_ENV_ */
