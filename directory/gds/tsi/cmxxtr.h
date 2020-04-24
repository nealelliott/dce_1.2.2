/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxxtr.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:40  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:49:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:51  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:44  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:22:59  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:53  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:00  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:50  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:11  marrek]
 * 
 * $EndLog$
 */
#ifndef _CMXXTR_H
#define _CMXXTR_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmxxtr_rcsid[] = "@(#)$RCSfile: cmxxtr.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:40 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/* #ident " @(#)cmxxtr.h	311.3 93/08/24 CMX (NC1)" */

#ifndef	__NOXTIDEFINES	/* defined in the trace modules */
#ifndef __TURBOC__

#define	T_accept	_trc_t_accept
#define	T_bind		_trc_t_bind
#define	T_close		_trc_t_close
#define	T_connect	_trc_t_connect
#define	T_getstate	_trc_t_getstate
#define	T_listen	_trc_t_listen
#define	T_look		_trc_t_look
#define	T_open		_trc_t_open
#define	T_rcv		_trc_t_rcv
#define	T_rcvconnect	_trc_t_rcvconnect
#define	T_rcvdis	_trc_t_rcvdis
#define	T_rcvrel	_trc_t_rcvrel
#define	T_snd		_trc_t_snd
#define	T_snddis	_trc_t_snddis
#define	T_sndrel	_trc_t_sndrel
#define	T_sync		_trc_t_sync
#define	T_unbind	_trc_t_unbind
#endif
#endif


#ifdef	__TURBOC__
#define	T_accept	t_accept
#define	T_bind		t_bind
#define	T_close		t_close
#define	T_connect	t_connect
#define	T_getstate	t_getstate
#define	T_listen	t_listen
#define	T_look		t_look
#define	T_open		t_open
#define	T_rcv		t_rcv
#define	T_rcvconnect	t_rcvconnect
#define	T_rcvdis	t_rcvdis
#define	T_rcvrel	t_rcvrel
#define	T_snd		t_snd
#define	T_snddis	t_snddis
#define	T_sndrel	t_sndrel
#define	T_sync		t_sync
#define	T_unbind	t_unbind
#endif

extern	void	*_trc_malloc(size_t);
extern	void	*_trc_calloc(size_t, size_t);

extern	int	_trc_t_accept(int, int, struct t_call *);
extern	int	_trc_t_bind(int, struct t_bind *, struct t_bind *);
extern	int	_trc_t_close(int);
extern	int	_trc_t_connect(int, struct t_call *, struct t_call *);
extern	int	_trc_t_getstate(int);
extern	int	_trc_t_listen(int, struct t_call *);
extern	int	_trc_t_look(int);
extern	int	_trc_t_open(char *, int, struct t_info *);
extern	int	_trc_t_rcv(int, char *, unsigned, int *);
extern	int	_trc_t_rcvconnect(int, struct t_call *);
extern	int	_trc_t_rcvdis(int, struct t_discon *);
extern	int	_trc_t_rcvrel(int);
extern	int	_trc_t_snd(int, char *, unsigned, int);
extern	int	_trc_t_snddis(int, struct t_call *);
extern	int	_trc_t_sndrel(int);
extern	int	_trc_t_unbind(int);
extern  int     _trc_t_sync (int);

/* #endif */	/* __NOXTIDEFINES */
#endif /* _CMXXTR_H */
