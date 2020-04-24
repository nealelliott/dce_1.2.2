/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxxtr.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:39  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:00  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:49:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:50  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:43  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:22:57  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:52  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:59  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:48  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:09  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxxtr.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:39 $";
#endif

/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1992
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/*****************************************************************************/
/*                                                                           */
/*        ICMX(L)on top of XTI/TLI TRACE FUNCTIONS                           */
/*        ========================================			     */
/*                                                                           */
/*****************************************************************************/

/*
static char sccsid[] = "@(#)cmxxtr.c	311.3 93/08/24 CMX (NC1)";
*/

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <xti.h>
#undef	T_MORE
#undef	T_CHECK

#include <sys/types.h>
#include "cmx.h"
#undef	T_MORE
#undef	T_CHECK
#include "cmxtrace.h"
#define	__NOXTIDEFINES
#include "cmxxtr.h"

extern	int	t_errno;
extern	int	errno;

int _trc_t_accept (
	int		 fd,
	int		 resfd,
	struct t_call	*call)
{
	int	retval;

	t_trc(T_XTI_ACCEPT, 0, fd, resfd, call);
	retval = t_accept(fd, resfd, call);
	t_trc(T_XTI_ACCEPT_FIN, 0, retval, t_errno, errno);
	return(retval);
}

int _trc_t_bind (
	int		 fd,
	struct t_bind	*req,
	struct t_bind	*ret)
{
	int	retval;

	t_trc(T_XTI_BIND, 0, fd, req, ret);
	retval = t_bind(fd, req, ret);
	t_trc(T_XTI_BIND_FIN, 0, ret, retval, t_errno, errno);
	return(retval);
}

int _trc_t_close (int	fd)
{
	int	retval;

	t_trc(T_XTI_CLOSE, 0, fd);
	retval = t_close(fd);
	t_trc(T_XTI_CLOSE_FIN, 0, retval, t_errno, errno);
	return(retval);
}

int _trc_t_connect (
	int		 fd,
	struct t_call	*sndcall,
	struct t_call	*rcvcall)
{
	int	retval;

	t_trc(T_XTI_CONNECT, 0, fd, sndcall, rcvcall);
	retval = t_connect(fd, sndcall, rcvcall);
	t_trc(T_XTI_CONNECT_FIN, 0, rcvcall, retval, t_errno, errno);
	return(retval);
}

int _trc_t_getstate (int	fd)
{
	int	retval;

	t_trc(T_XTI_GETSTATE, 0, fd);
	retval = t_getstate(fd);
	t_trc(T_XTI_GETSTATE_FIN, 0, retval, t_errno, errno);
	return(retval);
}

int _trc_t_listen (
	int		fd,
	struct	t_call	*call)
{
	int	retval;

	t_trc(T_XTI_LISTEN, 0, fd, call);
	retval = t_listen(fd, call);
	t_trc(T_XTI_LISTEN_FIN, 0, call, retval, t_errno, errno);
	return(retval);
}

int _trc_t_look (int	fd)
{
	int	retval;

	t_trc(T_XTI_LOOK, 0, fd);
	retval = t_look(fd);
	t_trc(T_XTI_LOOK_FIN, 0, retval, t_errno, errno);
	return(retval);
}

int _trc_t_open (
	char		*name,
	int		 oflag,
	struct	t_info	*info)
{
	int	retval;

	t_trc(T_XTI_OPEN, 0, name, oflag, info);
	retval = t_open(name, oflag, info);
	t_trc(T_XTI_OPEN_FIN, 0, info, retval, t_errno, errno);
	return(retval);
}

int _trc_t_rcv (
	int		 fd,
	char		*buf,
	unsigned	 nbytes,
	int		*flags)
{
	int	retval;

	t_trc(T_XTI_RCV, 0, fd, buf, nbytes);
	retval = t_rcv(fd, buf, nbytes, flags);
	t_trc(T_XTI_RCV_FIN, 0, buf, nbytes, *flags, retval, t_errno, errno);
	return(retval);
}

int _trc_t_rcvconnect (
	int		 fd,
	struct t_call	*call)
{
	extern int t_rcvconnect (int, struct t_call *);
	int	retval;

	t_trc(T_XTI_RCVCONNECT, 0, fd, call);
	retval = t_rcvconnect(fd, call);
	t_trc(T_XTI_RCVCONNECT_FIN, 0, call, retval, t_errno, errno);
	return(retval);
}

int _trc_t_rcvdis (
	int		 fd,
	struct t_discon	*discon)
{
	int	retval;

	t_trc(T_XTI_RCVDIS, 0, fd, discon);
	retval = t_rcvdis(fd, discon);
	t_trc(T_XTI_RCVDIS_FIN, 0, discon, retval, t_errno, errno);
	return(retval);
}

int _trc_t_rcvrel (int	fd)
{
	int	retval;

	t_trc(T_XTI_RCVREL, 0, fd);
	retval = t_rcvrel(fd);
	t_trc(T_XTI_RCVREL_FIN, 0, retval, t_errno, errno);
	return(retval);
}

int _trc_t_snd (
	int		 fd,
	char		*buf,
	unsigned	 nbytes,
	int		 flags)
{
	int	retval;

	t_trc(T_XTI_SND, 0, fd, buf, nbytes, flags);
	retval = t_snd(fd, buf, nbytes, flags);
	t_trc(T_XTI_SND_FIN, 0, retval, t_errno, errno);
	return(retval);
}

int _trc_t_snddis (
	int		 fd,
	struct t_call	*call)
{
	int	retval;

	t_trc(T_XTI_SNDDIS, 0, fd, call);
	retval = t_snddis(fd, call);
	t_trc(T_XTI_SNDDIS_FIN, 0, retval, t_errno, errno);
	return(retval);
}

int _trc_t_sndrel (int	fd)
{
	int	retval;

	t_trc(T_XTI_SNDREL, 0, fd);
	retval = t_sndrel(fd);
	t_trc(T_XTI_SNDREL_FIN, 0, retval, t_errno, errno);
	return(retval);
}

int _trc_t_unbind (int	fd)
{
	int	retval;

	t_trc(T_XTI_UNBIND, 0, fd);
	retval = t_unbind(fd);
	t_trc(T_XTI_UNBIND_FIN, 0, retval, t_errno, errno);
	return(retval);
}

int _trc_t_sync (int	fd)
{
	int	retval;

	t_trc(T_XTI_SYNC, 0, fd);
	retval = t_sync(fd);
	t_trc(T_XTI_SYNC_FIN, 0, retval, t_errno, errno);
	return(retval);
}

