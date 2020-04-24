/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: emiface.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:47  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:05  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:49:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:01  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:46  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:23:09  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:18:39  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:08  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:16:00  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:33  marrek]
 * 
 * $EndLog$
 */
#ifndef _EMIFACE_H
#define _EMIFACE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char emiface_rcsid[] = "@(#)$RCSfile: emiface.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:47 $";
#endif

/* #ident " @(#)emiface.h	311.3 93/08/24 CMX (NC1)" */
/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


/*
 * @(#)emiface.h	1.6 91/09/09 
 */
/*
 * header file for the EM interface
 */

#ifdef	SIGPOLL

#include <poll.h>

#else

struct	pollfd	{
	int	fd;
	int	events;
	int	revents;
};

#define	POLLIN	1
#define	POLLOUT	2
#define	POLLPRI	4

#endif

#define	NUM_FDS		32

#define	WANT_IN		1
#define	WANT_OUT	2
#define	WANT_CMD	4

/*
 * forward definitions
 */
#ifdef	__STDC__
extern	void	ptypoll(int, int (*)(void));
extern	void	ini_poll(void);
extern	void	end_poll(void);
extern	void	poll_drop_fd(int);
extern	int	reg_poll(int, int);
extern	int	check_poll(long);
extern	int	get_poll(int);
extern	int	npoll(void);
#else
extern	void	ptypoll();
extern	void	ini_poll();
extern	void	end_poll();
extern	void	poll_drop_fd();
extern	int	reg_poll();
extern	int	check_poll();
extern	int	get_poll();
extern	int	npoll();
#endif
#endif /* _EMIFACE_H */
