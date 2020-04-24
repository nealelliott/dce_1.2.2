/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: sendrcv.c,v $
BINRevision 1.1.2.1  1996/10/02  17:49:39  damon
BIN	Newest DFS from Transarc
BIN
 * Revision 1.1.2.2  1994/08/08  18:54:41  mckeen
 * 	Added conditional include of time
 * 	[1994/08/08  17:47:16  mckeen]
 *
 * Revision 1.1.2.1  1994/07/26  19:13:27  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:58:00  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:05:41  mckeen]
 * 
 * $EndLog$
 */

/*
 * sendrcv.c - Send/receive from socket with exponential backoff.
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfs_login/sendrcv.c,v 1.1.2.1 1996/10/02 17:49:39 damon Exp $ */

#include <sys/types.h>
#include <sys/socket.h>
#ifdef OSF1
#include <sys/time.h>
#else
#include <time.h>
#endif

/*
 * sendrcv - Send packet to socket with backoff, optionally receive reply.
 *
 * Inputs:
 *	sock		: Communications socket.  Must be bound to destination. 
 *	sdata		: Source data.
 *	slen		: Source data length.
 *	itimeout	: Initial timeout (seconds).
 *	tincr		: Timeout increment (timeout += tincr at each iteration).
 *	mxtimeout	: Maximum timeout (seconds).
 * In/Outs:
 *	dlen		: Destination data max length.	On return filled 
 *			  in with actual length received.
 *	ddata		: Destination data buffer.  If NULL, no reply or retries
 *			  are attempted.
 * Return value:
 *	Number of bytes read on success, -1 otherwise.	Errno may be set as well.
 */
int
sendrcv
#ifdef __STDC__
	(int sock, char *sdata, int slen, unsigned long itimeout, int tincr, int mxtimeout, int *dlen, char *ddata)
#else
	(sock, sdata, slen, itimeout, tincr, mxtimeout, dlen, ddata)
	int sock;
	char *sdata;
	int slen;
	unsigned long itimeout;
	int tincr;
	int mxtimeout;
	int *dlen;
	char *ddata;
#endif /* __STDC__ */
{
	struct timeval timeout;
	fd_set readfs;
	int ready;

	timeout.tv_sec = itimeout;
	timeout.tv_usec = 0;		
	for (; timeout.tv_sec <= mxtimeout; timeout.tv_sec += tincr) {
		if (send(sock, sdata, slen, 0) < 0) {
			return(-1);
		}
		if (ddata) {
			/*
			 * We're expecting something in return.	 Wait until something
			 * does or we timeout.
			 */
			FD_ZERO(&readfs);
			FD_SET(sock, &readfs);
			if ((ready = select(sock + 1, (int *)&readfs, 0, 0, &timeout)) < 0) {
				return(-1);
			} else if ((ready) && (FD_ISSET(sock, &readfs))) {
				*dlen = recv(sock, ddata, *dlen, 0);
				return(*dlen);
			}
		} else {
			return(0);
		}
	}	
	return(-1);
}
