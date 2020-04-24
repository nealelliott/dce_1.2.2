/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: emiface.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:46  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:05  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:49:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:59  root]
 * 
 * Revision 1.1.2.3  1994/06/21  14:48:29  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:47:06  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:18:38  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:07  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:59  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:32  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: emiface.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:46 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/


#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <signal.h>
#include "emiface.h"

#define SIGSELECT  /* select calls selected, otherwise poll */

#ifdef	SIGSELECT
#include <sys/types.h>

#if defined(_AIX)
#include <sys/select.h>
#endif /* _AIX */

extern	fd_set	t_rfds;
extern	fd_set	t_wfds;
extern	fd_set	t_xfds;

#else

static	struct	pollfd	polls[NUM_FDS];
static	struct	pollfd	*pfds;

#ifdef	TTY_POLL

static	int	pstopoll;
static	struct	pollfd	psfd;
static	int	(*psfunc)();

void
ptypoll(fd, func)
int	fd;
int	(*func)();
{
	if(func == 0){
		pstopoll = 0;
		return;
	}
	psfunc = func;
	psfd.events = POLLIN|POLLPRI;
	psfd.fd = fd;
	pstopoll = 1;
}
#endif
#endif

void
ini_poll()
{
#ifndef	SIGSELECT
	pfds = polls;	/* initialize the poll structures */
#ifdef	TTY_POLL
	if(pstopoll)	/* check for pty polls */
		*pfds++ = psfd;
#endif
#endif
}

int
reg_poll(fd, events)
register int	fd, events;
{
#ifdef	SIGSELECT
	if(events & WANT_IN)
		FD_SET(fd, &t_rfds);
	else
		FD_CLR(fd, &t_rfds);
	if(events & WANT_OUT)
		FD_SET(fd, &t_wfds);
	else
		FD_CLR(fd, &t_wfds);
	if(events & WANT_CMD)
		FD_SET(fd, &t_xfds);
	else
		FD_CLR(fd, &t_xfds);
	return(0);
#else
	register struct	pollfd	*pf = pfds;

	pf->fd = fd;
	pf->events = 0;
	if(events & WANT_IN)
		pf->events = POLLIN;
	if(events & WANT_OUT)
		pf->events |= POLLOUT;
	if(events & WANT_CMD)
		pf->events |= POLLPRI;
	pfds++;
	return(pf - polls);
#endif
}

int
check_poll(wtim)
long	wtim;
{
#ifdef	SIGSELECT
	return(0);
#else
	return(poll(polls, (unsigned long)(pfds - polls), (int)wtim));
#endif
}

int
get_poll(xc)
int	xc;
{
	register int	revent = 0;

#ifndef	SIGSELECT
	register int	rev;
	if((rev = polls[xc].revents) != 0){
		if(rev & POLLIN)
			revent |= WANT_IN;
		if(rev & POLLOUT)
			revent |= WANT_OUT;
		if(rev & POLLPRI)
			revent |= WANT_CMD;
	}
#endif
	return(revent);
}

void
end_poll()
{
#ifndef	SIGSELECT
#ifdef	TTY_POLL
	if(pstopoll && polls[0].revents)
		(*psfunc)();
#endif
#endif
}

int
npoll()
{
#ifdef	SIGSELECT
	return(0);
#else
	return(pfds - polls);
#endif
}

void
poll_drop_fd(fd)
int	fd;
{
#ifdef	SIGSELECT
	FD_CLR(fd, &t_rfds);
	FD_CLR(fd, &t_wfds);
	FD_CLR(fd, &t_xfds);
#endif
}
