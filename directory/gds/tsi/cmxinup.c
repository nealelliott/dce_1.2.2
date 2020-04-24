/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxinup.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:17  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:41  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:47:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:15  root]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:28  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:21:58  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:48:18  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:46:12  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:17  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:36  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:20  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:01  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxinup.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:17 $";
#endif

/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG  All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1993
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/****************************************************************************/
/*                                                                          */
/*                INTERFACE ICMX(L) ON TOP OF THE TCP/IP                    */
/*                ======================================                    */
/*                                                                          */
/* This code is a mapping of ICMX(L) as defined by CMX V2.5 until 3.1 onto  */
/* 4.3BSD sockets. Its roots are a similar implementation done for CMX V2.2.*/
/* This adaption adds major enhancements, higher robustness and the new     */
/* options available with CMX V2.5                                          */
/*   This code handles the convergence protocol according to RFC1006 (with  */
/* some local refinements). The RFC1006 protocol is defined in MSB byte     */
/* order. That has to be taken care of here.                                */
/*                                                                          */
/* Author: rit                                           Date:     6.03.89  */
/* Modifications: bee  V30                               Date:    24.08.90  */
/*              kam  V31				      	  05.03.92  */
/*                                                                          */
/* Defines:                                                                 */
/* NOSIGIO 	Do not compile code for signal driven I/O via SIGIO.        */
/* LOCALIOV 	Compile code for local implementation of vector I/O.        */
/*                                                                          */
/****************************************************************************/


#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#if defined(_AIX)
#include <sys/select.h>
#endif /* _AIX */
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#if defined(__OSF__)
#include <machine/endian.h>
#endif /* __OSF__ */
#include <sys/errno.h>
#include <stdarg.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/un.h>
#if !defined(_AIX) && !defined(HPUX) && !defined(SCO) && !defined(__OSF__)
#include <sys/filio.h>
#else
#include <sys/ioctl.h>
#endif
#include "cmx.h"
#include "cmxaddr.h"
#include "cmxextern.h"
#define	CMXINET_INTERN
#include "cmxinet.h"
#include "cmxtrace.h"
#include "cmx_tlv.h"
#include "cmx_macros.h"
#ifdef	ALLOC_TEST
#include "alloctrc.h"
#endif	/* ALLOC_TEST */

/**************************************************************************/
/*      D E C L A R A T I O N    O F    G L O B A L    D A T A            */
/**************************************************************************/
/*
 * global only for CMXINET.C und CMXINUP.C
 */

/*
static char sccsid[] = "@(#)cmxinup.c	311.59 93/08/24 CMX (NC1)";
*/

/*following data are global for t_optinit */
	int	tpdu = TC_TPDU_1024;
	int	credit = MAP_CREDIT(TC_TPDU_1024);
	int	sock_keep, sock_linger;

/**************************************************************************/
/*      D E C L A R A T I O N    O F    L O C A L    D A T A              */
/**************************************************************************/

static char	tc_buf[128];
static u_short	srcref, dstref;


static void tc_setflow(struct tc_con *conp);
static void prt10(register int num, char *str);
static unsigned int tc_getconind(struct tc_app *app);

/*------------------------------------------------------------------------*
 * MORE SOCKET HANDLING
 *------------------------------------------------------------------------*/

/*
 * tc_listen - enables a application to listen for incoming connections
 */
int tc_listen(int mode, struct tc_app *app)
{
	struct sockaddr_in sinme;
	struct tc_con *conp;
	int	res, sd;

	/*
	 * Allocate and init a connect element. Establish a socket in the
	 * proper domain and bind my port to it
	 */
	if ((conp = (struct tc_con *) calloc(sizeof(struct tc_con),
	    sizeof(char))) == NULLCP) {
		_t_error = T_ENOMEM;
		return (T_ERROR);
	}
	FLC_INI(conp->tc_flcontr);
	conp->tc_portnr = app->tc_portnr;
	conp->tc_state = T_IDLE;
	conp->savindata = NULL;
	conp->savoutdata = NULL;
	conp->read_state = R_IDLE;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	t_trc(T_FTCPSCK, 0, AF_INET, SOCK_STREAM, 0, sd, errno);
	if ((conp->tc_socket_fd = sd) == -1) {
		if (errno == EPROTONOSUPPORT || errno == EPFNOSUPPORT
		    || errno == ESOCKTNOSUPPORT || errno == EAFNOSUPPORT)
			_t_error = T_NOCCP;
		else if (errno == EMFILE || errno == ENOBUFS)
			_t_error = T_WAPP_LIMIT;
		else
			_t_error = errno;
		free((char *) conp);
		return (T_ERROR);
	}

	/*
	 * The address for the (passively) listening socket can be
	 * reused by the application at any time. Try to support this
	 * with the REUSEADDR option. If setting fails, bind will
	 * recognize and report via T_NOTFIRST
	 */
	/* The TCP port to be listened at is calculated as follows:
	 * T_REDIRECT:	port = (my PID modulo 0x8000) + MAX_PORT.
	 * T_PASSIVE:	port = as defined in my T selector.
	 * This assures some uniqueness of the T_REDIRECT port. There is nonzero
	 * probability that this port wraps around accidently and hits 
	 * some other unrelated port. The strategy to map it to
	 * (my port modulo 0x8000) + MAX_PORT had similar
	 * but more frequent collisions.
	 */
	tc_setsock(conp->tc_socket_fd,TC_BLOCK);

	memset((char *) &sinme, (int)0, sizeof(sinme));
	sinme.sin_family = AF_INET;
	if (mode == T_REDIRECT)
		sinme.sin_port = htons((u_short) ((tc_p.tc_flags.tc_mypid % 0x8000) + MAX_PORT));
		/*sinme.sin_port = htons((u_short) (conp->tc_portnr + MAX_PORT));*/
	else
		sinme.sin_port = htons((u_short) conp->tc_portnr);
	res = bind(conp->tc_socket_fd, (struct sockaddr *) &sinme, sizeof(sinme));
	t_trc(T_FTCPBIN, 0, conp->tc_socket_fd, &sinme, res, errno);
	if (res) {
		res = T_ERROR;
		if (errno == EADDRINUSE || errno == EADDRNOTAVAIL)
			res = T_NOTFIRST;
		else if (errno == EACCES)
			res = T_NOTPRIV;
		else
			_t_error = errno;
		close(conp->tc_socket_fd);
		FD_CLR(conp->tc_socket_fd,&t_fdset);
		FD_CLR(conp->tc_socket_fd,&t_rfds);
		t_trc(T_FCLOSE, 0, conp->tc_socket_fd);
		free((char *) conp);
		return (res);
	}

	res = listen(conp->tc_socket_fd, MAXLISTEN);
	t_trc(T_FTCPLIS, 0, conp->tc_socket_fd, MAXLISTEN, res, errno);
	if (res || tc_enabsig(conp)) {
		_t_error = errno;
		close(conp->tc_socket_fd);
		FD_CLR(conp->tc_socket_fd,&t_fdset);
		FD_CLR(conp->tc_socket_fd,&t_rfds);
		t_trc(T_FCLOSE, 0, conp->tc_socket_fd);
		free((char *)conp);
		return (T_ERROR);
	}

	/*
	 * set close-on-exec-flag
	 */
	FD_SET(conp->tc_socket_fd, &t_fdset);
	FD_SET(conp->tc_socket_fd, &t_rfds);
	fcntl(conp->tc_socket_fd, F_SETFD, 1);

	if (mode == T_PASSIVE)
		app->tc_pconp = conp;
	else
		app->tc_rconp = conp;

	return (T_OK);
}

/*
 * read data in nonblocking mode
 */
int tc_read_data(struct tc_con *conp, int amount)
{
	int	nread; /* amount bytes read */

	/*
	 * amount == NULL is only allowed if savindata != NULL
	 */
	if ((conp->savindata == NULL) && (amount == 0))
		return (T_ERROR);
	
	if (conp->savindata == NULL) {
		conp->savindata = (char *) calloc(amount,sizeof(char));
		conp->read_size = conp->tc_udatal;
		conp->fc_read->iov_len=amount;
		conp->fc_read->iov_base=conp->savindata;
	}

	while (((nread=
	read(conp->tc_socket_fd,conp->fc_read->iov_base,conp->fc_read->iov_len))
	  == -1) && errno == EINTR) 
		t_trc(T_FREAD, 0, conp->tc_socket_fd, 0, conp->fc_read->iov_len, nread, errno);
		
	t_trc(T_FREAD, 0, conp->tc_socket_fd, 0, conp->fc_read->iov_len, nread, errno);

	/* start local flow control (read) */

	if (((nread == -1) && (errno != EAGAIN)) || /* Unrecoverable error */
	     (nread == 0))                          /* no TCP-connection */
		return (T_ERROR);

	if (nread != -1) {
		/* 
		 * some (1<= x < conp->fc_read->iov_len)
		 *  data have been read
		 */
		conp->fc_read->iov_len -= nread;
		conp->fc_read->iov_base += nread;
	}
	if (conp->fc_read->iov_len == 0)
		return (T_OK);
	else
		return (T_RETRY);
	/* end checking result of read */

}

/*
 * tc_read - reading n bytes from socket with appropriate retries
 */
static int tc_read(int sd, char *buf, int n)
{
	register int	i, cc = 0;
	register char	*bp;
	 
	for (bp = buf, i = n; i > 0; bp += cc, i -= cc) {
		cc = read(sd, bp, (unsigned) i);
		t_trc(T_FREAD, 0, sd, 0, i, cc, errno);
		if (cc == 0)
			return (bp - buf);
		if (cc == -1)
			if (errno == EINTR)
				cc = 0;
			else
				return ((i = bp - buf) ? i : -1);
	}
	
	return (bp - buf);
}

/*
 * tc_writev - writing n bytes from cnt buffers
 */
int tc_writev(struct tc_con *conp, int sd, struct iovec *iov, int cnt, int n)
{
	int	j, ncnt, nrest;
	struct iovec	*piov;
	char 	*y;
#if defined(_AIX) || defined(__OSF__)
static	struct 	sigaction sig_ign = {SIG_PTR SIG_IGN,0,0};
#else
static	struct 	sigaction sig_ign = {0, SIG_PTR SIG_IGN, 0};
#endif /* _AIX || __OSF__ */
	struct 	sigaction sig_sav;
#if defined(HPUX)
	void	(* sig_old) ();
#endif

	piov = iov; /* initialize pointer to I/O-vector */

	/* 
	 * ignore signal SIGPIPE
	 */
	(void) sigaction(SIGPIPE,&sig_ign,&sig_sav);

#if defined(HPUX)
	/*
         * HPUX does not ignore the signal reset by sigaction.
         * ignore SIGPIPE with signal, but set and reset
         * addresspointer with sigaction (nonsense)
         */
	sig_old = signal(SIGPIPE,SIG_IGN);
#endif

	while((ncnt = writev(sd, piov, cnt)) == -1 && errno == EINTR)
		t_trc(T_FWRITE, 0, sd, 0, n, ncnt, errno);

	(void) sigaction(SIGPIPE,&sig_sav,NULL);

	t_trc(T_FWRITE, 0, sd, 0, n, ncnt, errno);

	/* determin rest of data to transfer */
	if (ncnt != -1)
		nrest = n - ncnt; /* n-ncnt data transferred */
	else
		nrest = n; /* no data transferred */

	if (ncnt == n) { /* all data transferred */
		if (conp->savoutdata != NULL) {
			/* saved data of flow control written */
			free(conp->savoutdata);
			conp->savoutdata = NULL;
			/* local flow control (write) not longer activ */
		}
		return (ncnt); /* all data transferred */
	}

	if ((ncnt == -1) && (errno != EAGAIN)) /* Unrecoverable error */
			return (T_ERROR); /* no data transferred */

	/* Start local flow control (write) */
	if (ncnt == -1) /*No data transferred at EAGAIN */
		ncnt = 0;

     	while (ncnt >= 0) {
		if (ncnt >= piov->iov_len) {
			ncnt -= piov->iov_len;
			piov++;
			cnt--;
		} else { /* Distinguish data to save */
			/*
			 * if write data of local flow control
			 * piov = conp->fc_write
			 */
			piov->iov_base += ncnt;
			piov->iov_len -= ncnt;
			ncnt = -1;
			/*
			 * Save data for local flow control (write) 
			 */
			if (conp->savoutdata == NULL){ /*save data */
				j=nrest; /* Length of data */
				y = conp->savoutdata = (char *) calloc(j,sizeof(char));
				conp->fc_write->iov_len=nrest;
				conp->fc_write->iov_base=y;
				while (j>0) {
					memcpy(y,piov->iov_base,piov->iov_len);
					y += piov->iov_len;
					j -= piov->iov_len;
					piov++;
					cnt--;
				} /* end while */
			} /* end save data */
		} /* end distinguish data to save */
	} /* end while */

	return (n - nrest); /* return number of bytes transferred */

	/* end local flow control (write) */
} /* end tc_writev */

/*
 * tc_enabsig - enable signalling SIGIO for a given socket
 * This involves three steps:
 * 1.	Assign a signal handler for SIGIO. Can't use sigvec(2), because
 *      CMX application depends on EINTR for interrupted syscalls.
 * 2.	Set the socket's FASYNC flag
 * 3.	Set the pid or process group id which is to receive SIGIO
 * Disabling SIGIO happens via close (?). Upon last DETACH, SIGIO is defaulted.
 */
static int	sigcnt;

static void tc_sigio(int sigg)
{

#if defined(_AIX) || defined(__OSF__)
static	struct 	sigaction this_routine = {SIG_PTR tc_sigio,0,0};
#else
static	struct 	sigaction this_routine = {0, SIG_PTR tc_sigio, 0};
#endif /* _AIX || __OSF__ */

	t_trc(T_FSIGIO, ++sigcnt);
	sigaction(SIGIO,&this_routine,NULL);
}

int tc_enabsig(struct	tc_con *conp)
/* ARGSUSED */
{
	int	on = 1;
#if defined(_AIX) || defined(__OSF__)
static	struct 	sigaction tcsigio = {SIG_PTR tc_sigio,0,0};
#else
static	struct 	sigaction tcsigio = {0, SIG_PTR tc_sigio, 0};
#endif /* _AIX || __OSF__ */

	if (!tc_p.tc_flags.tc_enabsig)
		return (T_OK);
#ifndef NOSIGIO
	if (t_sigio)         /* CMX-Handler aktiv      */
		;
	else {
		t_trc(T_FSIGIO, 0);
		sigaction(SIGIO,&tcsigio,&t_prevsigio);
		t_sigio = TRUE;
		if (t_prevsigio.sa_handler == SIG_DFL)
			;
		else {
			sigaction(SIGIO,&t_prevsigio,NULL);
			t_sigio = FALSE;
		}
	}
	if (ioctl(conp->tc_socket_fd, FIOASYNC, &on) == -1) {
		_t_error = errno;
		return (T_ERROR);
	}
#endif
	return (T_OK);
}

/*------------------------------------------------------------------------*
 * CONVERSION BETWEEN HOST AND NETWORK BYTEORDER
 *------------------------------------------------------------------------*/

/*
 * tc_uc2i - converting 4 consecutive bytes representing an integer in
 *           network byte order to an integer in host byte order
 *
 * ARGUMENTS
 *	in  ptr: pointer to the most significant byte of the integer to convert
 *	out val: resulting integer
 *
 * RETURN VALUE
 *	pointer to byte after converted integer
 */
#ifdef MIST
static u_char *tc_uc2i(u_char *ptr, int *val)
{
	u_int 	v_hilf;

	memcpy(&v_hilf,ptr,sizeof(v_hilf));
	*val = ntohl(v_hilf);
	return (ptr + sizeof (u_int));
}
#endif /* MIST */

#ifdef MIST
/*
 * tc_i2uc - converting an integer into 4 consecutive bytes representing 
 *           an integer in network byte order
 *
 * ARGUMENTS
 * 	in val:	integer to convert
 * 	in ptr:	pointer to the first byte the integer is to be placed to
 *
 * RETURN VALUE
 *	pointer to byte after converted integer
 */
static u_char *tc_i2uc(int val, u_char *ptr)
{
	int 	v_hilf;

	v_hilf = htonl(val);
	memcpy(ptr,&v_hilf,sizeof(v_hilf));
	return (ptr + sizeof (int));
}
#endif /* MIST */

/*
 * tc_uc2us - converting 2 consecutive bytes representing an unsigned short in
 *            network byte order to an unsigned short in host byte order
 *
 * ARGUMENTS
 *	 in  ptr: pointer to the most significant byte of the unsigned short to
 *			convert
 *	 out val: resulting unsigned short
 *
 * RETURN VALUE
 *	pointer to byte after converted unsigned short
 */
static u_char *tc_uc2us(u_char *ptr, u_short *val)
{
	u_short 	v_hilf;

	memcpy(&v_hilf,ptr,sizeof(v_hilf));
	*val = ntohs(v_hilf);
	return(ptr + sizeof(u_short));
}

/*
 * tc_us2uc - converting an unsigned short into 2 consecutive bytes
 *            representing an unsigned short in network byte order
 *
 * ARGUMENTS
 *	in val:	unsigned short to convert
 *	in ptr:	pointer to the first byte the unsigned short is to be placed to
 *
 * RETURN VALUE
 *	pointer to byte after converted unsigned short
 */
static u_char *tc_us2uc(u_short val, u_char *ptr)
{
	u_short	v_hilf;

	v_hilf = htons(val);
	memcpy(ptr,&v_hilf,sizeof(v_hilf));
	return (ptr + sizeof (short));
}

/*------------------------------------------------------------------------*
 * TPKT CONSTRUCTION AND ANALYSIS FOLLOWING RFC 1006
 *------------------------------------------------------------------------*/
/*
 * tc_sndfrm - builds and writes a TP0 frame
 */
/* VARARGS2 */
int tc_sndfrm(int code, struct tc_con *conp, ...) {

	va_list		ap;
	register u_char	*ptr;
	int		len = 0;
	int		fd;
	u_char		*lgthptr, *liptr;
	u_short		lgth;
	unsigned int	hconind;
	unsigned int	hreason = 0;
	unsigned int	hchain;
	struct iovec	*hvdata;
	unsigned int	hvcnt;
	unsigned int	hfd;
	unsigned int	hdatal;
	struct iovec	iovr[1];

	va_start(ap, conp);

	fd = conp->tc_socket_fd;
	tc_buf[0] = (u_char) TC_VRSN;
	tc_buf[1] = (u_char) TC_RSVD;
	lgthptr = (u_char *) &tc_buf[2];
	liptr = (u_char *) &tc_buf[4];
	tc_buf[5] = (u_char) code;
	ptr = (u_char *) &tc_buf[6];

	switch (code) {
	case CONRQ:	/* tc_sndfrm(CONRQ, conp, calling, optc1) */
	case CONRS: {	/* tc_sndfrm(CONRS, conp, called, optc1) */
		struct t_optc1 *opt;
		u_short	called;
		u_short	calling;
		struct tc_app	*appl;

		if (code == CONRQ) {

			calling = (u_short) va_arg(ap, int);
			called = (u_short) conp->tc_portnr;

			/* src- & dst-ref */
			appl = tc_srch_app( (int) calling);
			hconind = tc_getconind(appl);
			ptr = tc_us2uc(0, ptr);
			ptr = tc_us2uc(SET_SRCREF(appl->tc_ind, hconind), ptr);

			/* transport-class */
			*ptr++ = (u_char) TC_TP0;

			/* Parameter TPDU-Size */
			tc_setflow(conp); /* get TPDU-Size */
			*ptr++ = (u_char) TC_PAR_TPDULEN;
			*ptr++ = (u_char) 1;
			if ((conp->tc_partner == RFCV30) || (conp->tc_partner == RFCV25))
				*ptr++ = (u_char) conp->tc_mxtpdu<<4;
			else
				*ptr++ = (u_char) conp->tc_mxtpdu;

			/* Parameter calling Port */
			*ptr++ = (u_char) TC_PAR_CALLING;
			if ((conp->tc_partner == RFCV30) || (conp->tc_partner == RFCV25)) {
				*ptr++ = (u_char) 2;
				conp->tc_tcallnglen = 2;
				ptr = tc_us2uc(calling,ptr);
			} else {
				*ptr++ = (u_char)conp->tc_tcallnglen;
				memcpy(ptr,conp->tc_tselng,conp->tc_tcallnglen);
				ptr += conp->tc_tcallnglen;
			}

			/* Parameter called Port */
			*ptr++ = (u_char) TC_PAR_CALLED;
			*ptr++ = (u_char) conp->tc_tcalledlen;
			memcpy(ptr,conp->tc_tseled,conp->tc_tcalledlen);
			ptr += conp->tc_tcalledlen;

			/* user data */
			if (conp->tc_optdatlng) {
				len = conp->tc_optdatlng;
				memcpy((char *) ptr, conp->tc_optdat, len);
				free(conp->tc_optdat);
				conp->tc_optdatlng = 0;
			}
		}

		if (code == CONRS) {

			called = (u_short) va_arg(ap, int);
			calling = (u_short) conp->tc_portnr;

			/* src- & dst-ref */
			ptr = tc_us2uc(conp->tc_dstref, ptr);
			ptr = tc_us2uc(conp->tc_srcref, ptr);

			/* transport-class */
			*ptr++ = (u_char) TC_TP0;

			/* Parameter TPDU-Size */
			*ptr++ = (u_char) TC_PAR_TPDULEN;
			*ptr++ = (u_char) 1;
			if ((conp->tc_partner == RFCV30) || (conp->tc_partner == RFCV25))
				*ptr++ = (u_char) conp->tc_mxtpdu<<4;
			else
				*ptr++ = (u_char) conp->tc_mxtpdu;

			/* Parameter calling Port */
			*ptr++ = (u_char) TC_PAR_CALLING;
			*ptr++ = (u_char)conp->tc_tcallnglen;
			memcpy(ptr,conp->tc_tselng,conp->tc_tcallnglen);
			ptr += conp->tc_tcallnglen;

			/* Parameter called Port */
			*ptr++ = (u_char) TC_PAR_CALLED;
			*ptr++ = (u_char) conp->tc_tcalledlen;
			memcpy(ptr,conp->tc_tseled,conp->tc_tcalledlen);
			ptr += conp->tc_tcalledlen;

			if (conp->tc_flow.enab) {
				conp->tc_flow.credit = MAP_CREDIT(conp->tc_mxtpdu);
				conp->tc_flow.upwe = conp->tc_flow.credit;
			}
			conp->tc_mxtidu = MAP_TPDU(conp->tc_mxtpdu) - TC_DTPDU_PRE;

			/* user data */
			opt = (struct t_optc1 *) va_arg(ap, t_opt13 *);
			if (opt) {
				len = opt->t_udatal;
				memcpy((char *) ptr, opt->t_udatap, len);
			}
		}

		*liptr = TC_CR_FIX + conp->tc_tcalledlen + conp->tc_tcallnglen -4;
		lgth = *liptr + (TC_HDRLEN - 1) + len;
		break;
	}
	case DISRQ: {	/* tc_sndfrm(DISRQ, conp, reason, optc2) */
		struct t_optc2 *opt;

		/* src- & dst-ref */
		ptr = tc_us2uc(conp->tc_dstref, ptr);
		ptr = tc_us2uc(conp->tc_srcref, ptr);

		/* disconnect-reason */
		hreason = va_arg(ap, int);
		*ptr++ = (u_char) ((hreason == T_USER) ? 128 :
				(hreason == T_RPROTERR) ? 133 : hreason);

		/* user data */
		opt = (struct t_optc2 *) va_arg(ap, t_opt2 *);
		if (opt) {
			len = opt->t_udatal;
			memcpy((char *) ptr, opt->t_udatap, len);
		}
		*liptr = TC_DR_FIX;
		lgth = *liptr + (TC_HDRLEN - 1) + len;
		break;
	}
	case DATRQ: {	/* tc_sndfrm(DATRQ, conp, vdata, vcnt, datal, chain) */
		int	i, res;
		struct iovec	iov[T_VCNT + 1], *piov;

		if (conp->tc_flow.enab)
			*ptr = conp->tc_flow.tpdunr;
		else
			*ptr = 0;
		hvdata = va_arg(ap, struct iovec *);
		hvcnt = va_arg(ap, int);
		hdatal = va_arg(ap, int);
		hchain = va_arg(ap, int);
		if (hchain == T_END)
			*ptr |= TC_END;
		*liptr = TC_DT_FIX;
		lgth = *liptr + (TC_HDRLEN - 1) + hdatal;
		tc_us2uc(lgth, lgthptr);
		t_trc(T_FTCPPKT, TC_HDRLEN + 1, tc_buf);

		piov = (struct iovec *) hvdata;
		iov[0].iov_base = tc_buf;
		iov[0].iov_len = TC_HDRLEN + 1;
		for (i = 1; i <= hvcnt; i++, piov++) {
			iov[i].iov_base = piov->iov_base;
			iov[i].iov_len = piov->iov_len;
		}

		res = tc_writev(conp, fd, iov, hvcnt + 1, lgth);
		if (res < 0)
			return (T_ERROR);
		if (res != lgth) {
			FD_SET(conp->tc_socket_fd, &t_wfds);
			conp->tc_state=T_LOCFLOWCON;
			return (T_DATASTOP); /* local flow control (write) */
		}

		if (conp->tc_flow.enab) {
			conp->tc_flow.tpdunr++;
			if (conp->tc_flow.tpdunr != conp->tc_flow.upwe)
				return (T_OK);
			else {
				FLC_SET(conp->tc_flcontr, T_SNDSTOP);
				return (T_DATASTOP);
			}
		} else
			return (T_OK);
		
	}
	case ACKRQ: 	/* tc_sndfrm(ACKRQ, conp) */
		ptr = tc_us2uc(conp->tc_dstref, ptr);		/* dst-ref */
		*ptr = conp->tc_flow.yrtunr;	/* next expected TPDU-# */
		*liptr = TC_AK_FIX;
		lgth = *liptr + (TC_HDRLEN - 1) + len;
		break;

	case REDRQ: {	/* tc_sndfrm(REDRQ, conp, redfd, opt) */
		struct t_optc2 *opt;

		hfd = va_arg(ap, int);
		opt = (struct t_optc2 *) va_arg(ap, t_opt23 *);
		
		ptr = tc_us2uc(t_pid, ptr);
		*ptr++ = fd;
		*ptr++ = conp->tc_flcontr;
		ptr = tc_us2uc(conp->tc_mxtidu, ptr);
		*ptr++ = conp->tc_flow.enab;
		*ptr++ = conp->tc_flow.credit;
		*ptr++ = conp->tc_flow.tpdunr;
		*ptr++ = conp->tc_flow.yrtunr;
		*ptr++ = conp->tc_flow.lowe;
		*ptr++ = conp->tc_flow.upwe;
		ptr = tc_us2uc(conp->tc_dstref, ptr);		/* dst-ref */
		ptr = tc_us2uc(conp->tc_srcref, ptr);		/* src-ref */

		/* user data */
		if (opt) {
			len = opt->t_udatal;
			memcpy((char *) ptr, opt->t_udatap, len);
		}
		*liptr = TC_RR_FIX;
		lgth = *liptr + (TC_HDRLEN - 1) + len;
		fd = hfd;
		break;
	}
	default:
		return (T_ERROR);
	}
	tc_us2uc(lgth, lgthptr);
	t_trc(T_FTCPPKT, lgth, tc_buf);
	iovr[0].iov_base = tc_buf;
	iovr[0].iov_len = (int) lgth;
	if (tc_writev(conp, fd, iovr, 1, (int) lgth) != lgth)
		return (T_ERROR);

	return (T_OK);
}

/*
 * tc_rcvhdr - read and analyse TP0 frame header
 */
int tc_rcvhdr(struct tc_con *conp, int *code)
{
	u_char vr, li, co;
	int	i;
	u_short	lgth;

	if (conp->read_state != R_IDLE && conp->read_state != R_INHEADER) {
		*code=conp->tpdu_code;
		return (T_OK);
	}

	conp->tc_udatal = conp->tc_length = 0;
	i = tc_read_data(conp, TC_HDRLEN);
	if (i == T_RETRY) {
		conp->read_state = R_INHEADER;
		return (T_RETRY);
	}
	if (i == T_ERROR) {
		conp->tc_reason = T_RLCONNLOST;
		return (T_ERROR);
	}
	conp->read_state = R_ENDHEADER;
	memcpy(tc_buf, conp->savindata, TC_HDRLEN);
	free(conp->savindata);
	conp->savindata = NULL;

	t_trc(T_FTCPHDR, TC_HDRLEN, tc_buf);

	vr = (u_char) tc_buf[0];
	co = (u_char) tc_buf[5];
	if (vr != TC_VRSN || MAP_CODE(co)) {
		tc_sndfrm(DISRQ, conp, T_RPROTERR, NULL);
		conp->tc_reason = T_RLPROTERR;
		return (T_ERROR);
	}

	tc_uc2us((u_char *) &tc_buf[2], &lgth);
	li = (u_char) tc_buf[4];
	conp->tc_length = lgth - TC_HDRLEN;
	conp->tc_udatal = conp->tc_length - (li - sizeof(u_char));
	*code = (int) co;
	conp->tpdu_code = *code;
	return (T_OK);
}

/*
 * tc_rcvbdy - read and analyse TP0 frame body
 *
 * This function was a switch over 5 different cases with no common part.
 * Now it is splitted into 5 functions, to avoid problems with varargs
 * on some machines.
 */
int tc_rcvconincf (int code, struct tc_con *conp, t_opt1 *opt)
{
	/* tc_rcvbdy(CONIN, conp, opt) */
	/* tc_rcvbdy(CONCF, conp, opt) */

	register u_char *ptr = (u_char *) tc_buf;
	int		i;
	int		fd = conp->tc_socket_fd;
	int		len = conp->tc_length;
	int		udatal = conp->tc_udatal;
	u_short		cl;
	u_short		tpdusize = 0;

	if (tc_read(fd, tc_buf, len) != len) {
		conp->tc_reason = T_RLCONNLOST;
		return (T_ERROR);
	}
	t_trc(T_FTCPBDY, (CONIN << 8) | (len & 0xff), tc_buf, udatal);

	ptr = tc_uc2us(ptr, &dstref);
	ptr = tc_uc2us(ptr, &srcref);
	conp->tc_dstref = srcref;
	cl = *ptr++;
	if (cl != TC_TP0) {
		tc_sndfrm(DISRQ, conp, T_RPROTERR, NULL);
		conp->tc_reason = T_RLPROTERR;
		return (T_ERROR);
	}
	tc_setflow(conp);
	if (code == CONCF)
		conp->tc_mxtidu = MAP_TPDU(conp->tc_mxtpdu) - TC_DTPDU_PRE;
	/*
	 * For partner identification:
	 * CMX<=3.0 always sends the parameter TPDU-SIZE.
	 * Default value 4.0, is always decremented by CMX<=3.0
	 */
	conp->tc_partner=RFCV40;
	for (i = TC_PAR_OFF; i < (len - udatal - 1); ) {
		switch ((u_char) tc_buf[i++]) {
		case TC_PAR_TPDULEN:
			tpdusize = (u_char) tc_buf[++i];

			if (tpdusize >= 0x10) {
				/* tpdusize >= 0x10 ==>
				 *   Partner is CMX <=3.0
				 *   Correct value of tpdusize  */
				conp->tc_partner=RFCV30;
				tpdusize = tpdusize>>4;
			}
			switch (tpdusize) {
			case 0:
				tpdusize = TC_TPDU_4096;
				break;

			case TC_TPDU_4096:
			case TC_TPDU_2048:
			case TC_TPDU_1024:
			case TC_TPDU_512:
			case TC_TPDU_256:
			case TC_TPDU_128:
				break;

			default:
				tc_sndfrm(DISRQ, conp, T_RPROTERR, NULL);
				conp->tc_reason = T_RLPROTERR;
				return (T_ERROR);
			}

			if (code == CONIN) {
				conp->tc_mxtpdu = MIN(conp->tc_mxtpdu, (int) tpdusize);
			}
			if (code == CONCF) {
		
				if ((conp->tc_partner == RFCV30) || (conp->tc_partner == RFCV25)) {
					if (conp->tc_mxtpdu < (int) tpdusize) {
						tc_sndfrm(DISRQ, conp,
					     	T_RPROTERR, NULL);
						conp->tc_reason = T_RLPROTERR;
						return (T_ERROR);
					}
					if (conp->tc_flow.enab) {
						conp->tc_flow.credit =
							MAP_CREDIT(tpdusize);
						conp->tc_flow.upwe =
							conp->tc_flow.credit;
					}
				}
				conp->tc_mxtidu = MAP_TPDU(tpdusize) - TC_DTPDU_PRE;
			}
			i++;
			break;

		case TC_PAR_CALLED:
			cl = tc_buf[i];
			if (code == CONIN) {
				/* at RFC1006 incoming T-Selector
				 * must be equal to local T-selector
				 */
				if ((conp->tc_partner == RFCV40) &&
				   !(memcmp(conp->tc_tseled, &tc_buf[i+1] , cl))) {
					tc_sndfrm(DISRQ, conp, T_RPROTERR, NULL);
					conp->tc_reason = T_RLPROTERR;
					return (T_ERROR);
				}
				memcpy(conp->tc_tseled,&tc_buf[i+1],cl);
				conp->tc_tcalledlen = cl;
				if ((conp->tc_partner == RFCV25) || (conp->tc_partner == RFCV30))
					tc_uc2us((u_char *) &tc_buf[i+1], &portnrsav);
			}				
			i += cl + 1;
			break;

		case TC_PAR_CALLING:
			cl = tc_buf[i];
			if (code == CONIN) {
				memcpy(conp->tc_tselng,&tc_buf[i+1],cl);
				conp->tc_tcallnglen = cl;
			}				
			i += cl + 1;
			break;

		default:
			i += (tc_buf[i] + 1);
		}
	}

	if (conp->tc_partner == RFCV40) {
		/* Switch off flow control */
		conp->tc_flow.enab = 0;
		conp->tc_flow.credit = 0;
		conp->tc_flow.lowe = 0;
		conp->tc_flow.upwe = 0;
	}

	if ((conp->tc_partner == RFCV25)||(conp->tc_partner == RFCV30))
		tc_uc2us((u_char *) conp->tc_tselng,&conp->tc_portnr);

	if (opt) {
		opt->optc1.t_xdata = T_NO;
		opt->optc1.t_timeout = T_NO;
		if (opt->optc1.t_udatal) {
			opt->optc1.t_udatal = udatal;
			memcpy(opt->optc1.t_udatap, &tc_buf[len - udatal], udatal);
		}
	}

	conp->tc_xdata = T_NO;
	conp->tc_length = conp->tc_udatal = 0;
	return (T_OK);
}

int tc_rcvdisin(struct tc_con *conp, int *hreasptr, t_opt2 *opt)
{
	/* tc_rcvbdy(DISIN, conp, reason, opt) */
	register u_char *ptr = (u_char *) tc_buf;
	int		fd = conp->tc_socket_fd;
	int		len = conp->tc_length;
	int		udatal = conp->tc_udatal;
	int		reas;

	if (len) {
		conp->tc_reason = T_RUNKNOWN;
		if (tc_read(fd, tc_buf, len) != len)
			conp->tc_reason = T_RLCONNLOST;
		else {
			t_trc(T_FTCPBDY, (DISIN << 8) | (len & 0xff), tc_buf, udatal);
			if (opt && opt->optc2.t_udatal) {
				opt->optc2.t_udatal = udatal;
				memcpy(opt->optc2.t_udatap,
					&tc_buf[len - udatal], udatal);
			}
			reas = ptr[4];
			if ((reas >= 0 && reas <= 3) || 
    			    (reas >= 129 && reas <= 138))
				reas += 256;
			else if (reas == 128)
				reas = T_USER;
			else if (udatal) {
				ptr = (u_char *) &tc_buf[len - udatal];
				reas = ptr[0] << 8 | ptr[1];
			}
			conp->tc_reason = reas;
		}
	} else { 
		if (opt)
			opt->optc2.t_udatal = 0;
	}
	*hreasptr = conp->tc_reason;
	conp->tc_length = conp->tc_udatal = 0;
	return (T_OK);
}

int tc_rcvdatin(struct tc_con	*conp,
		struct iovec	*piov,
		unsigned int	hvcnt,
		unsigned int	hcnt)
{
	/* tc_rcvbdy(DATIN, conp, vdata, vcnt, cnt) */
	int	i;
	char	hi,*phi, *sav_base;
	struct iovec	iov[T_VCNT + 1];

	phi = &hi;
	sav_base=conp->savindata;
	iov[0].iov_base = phi;
	iov[0].iov_len = 1;
	memcpy(iov[0].iov_base,sav_base,iov[0].iov_len);
	sav_base++;
	for (i = 1; i <= hvcnt; i++, piov++) {
		/* Transfer data from savearea to user */
		memcpy(piov->iov_base,sav_base,piov->iov_len);
		sav_base+=piov->iov_len;
	}
	/* free allocated memory of local flow control (read) */
	free(conp->savindata);
	conp->savindata=NULL;

	t_trc(T_FTCPEOT, (DATIN << 8) | (i & 0xff));
	conp->tc_chain = ((*phi) & TC_END) ? T_END : T_MORE;
	conp->tc_length = 0;

	if (conp->tc_flow.enab
		&& ++conp->tc_flow.yrtunr % conp->tc_flow.credit == 0)
			if (tc_sndfrm(ACKRQ, conp)) {
				conp->tc_reason = T_RLCONNLOST;
				return (T_ERROR);
			}
	return (T_OK);
}

int tc_rcvackin(struct tc_con *conp)
{
	/* tc_rcvbdy(ACKIN, conp) */
	register u_char *ptr = (u_char *) tc_buf;
	int		fd = conp->tc_socket_fd;
	int		len = conp->tc_length;
	int		udatal = conp->tc_udatal;
	int	lo, t, tplus, yr;

	if (tc_read(fd, tc_buf, len) != len) {
		conp->tc_reason = T_RLCONNLOST;
		return (T_ERROR);
	}
	t_trc(T_FTCPBDY, (ACKIN << 8) | (len & 0xff), tc_buf, udatal);

	lo = conp->tc_flow.lowe;
	t = conp->tc_flow.tpdunr;
	tplus = t + 1;
	yr = ptr[2];
	if (yr < lo)
		yr += 128;
	if (t < lo)
		t += 128;
	if (tplus < lo)
		tplus += 128;
	if (yr < lo || yr > tplus) {
		tc_sndfrm(DISRQ, conp, T_RPROTERR, NULL);
		conp->tc_reason = T_RLPROTERR;
		return (T_ERROR);
	}

	conp->tc_flow.lowe = (unsigned int) ptr[2];
	conp->tc_flow.upwe = (unsigned int) ptr[2]+conp->tc_flow.credit;

	conp->tc_udatal = conp->tc_length = 0;
	return (T_OK);
}

int tc_rcvredin(struct tc_con *conp, int *hpid, t_opt23 *opt)
{
	/* tc_rcvbdy(REDIN, conp, pid, opt) */
	register u_char *ptr = (u_char *) tc_buf;
	int		fd = conp->tc_socket_fd;
	int		len = conp->tc_length;
	int		udatal = conp->tc_udatal;
	u_short		us;

	if (tc_read(fd, tc_buf, len) != len) {
		conp->tc_reason = T_RLCONNLOST;
		return (T_ERROR);
	}
	t_trc(T_FTCPBDY, (REDIN << 8) | (len & 0xff), tc_buf, udatal);

	close(conp->tc_socket_fd);
	FD_CLR(conp->tc_socket_fd, &t_fdset);
	FD_CLR(conp->tc_socket_fd, &t_rfds);
	t_trc(T_FCLOSE, 0, conp->tc_socket_fd);

	ptr = tc_uc2us(ptr, &us);    /* using hpid as argument to ...*/
	*hpid = us;		     /* ... tc_uc2us fails on T4600 */
	conp->tc_socket_fd = *ptr++;
	conp->tc_flcontr = *ptr++;
	ptr = tc_uc2us(ptr, &us);
	conp->tc_mxtidu = (int)us;
	conp->tc_flow.enab = *ptr++;
	conp->tc_flow.credit = *ptr++;
	conp->tc_flow.tpdunr = *ptr++;
	conp->tc_flow.yrtunr = *ptr++;
	conp->tc_flow.lowe = *ptr++;
	conp->tc_flow.upwe = *ptr++;
	ptr = tc_uc2us(ptr, &conp->tc_dstref);
	ptr = tc_uc2us(ptr, &conp->tc_srcref);

	conp->tc_udatal = conp->tc_length = 0;

	if (opt) {
		if (opt->optc3.t_optnr == T_OPTC3) {
			opt->optc3.t_timeout = T_NO;
			opt->optc3.t_xdata = T_NO;
		}
		if (opt->optc3.t_udatal) {
			opt->optc3.t_udatal = udatal;
			memcpy(opt->optc3.t_udatap, &tc_buf[len - udatal], udatal);
		}
	}
	return (T_OK);
}

/*
 * tc_setflow - get flow control values from environment or return defaults
 */
static void tc_setflow(struct tc_con	*conp)
{
	
	extern	int	tpdu;
	extern	int	credit;

	conp->tc_mxtpdu = tpdu;
	if (credit) {
		conp->tc_flow.enab = 1;
		conp->tc_flow.credit = credit;
		conp->tc_flow.lowe = 0;
		conp->tc_flow.upwe = credit;
	}

	return;
}

/*------------------------------------------------------------------------*
 * ADDRESS CONSTRUCTION AND ANALYSIS
 *------------------------------------------------------------------------*/

/*
 * tc_getmyport - get my port number from T-Selector for LAN1 in LOCAL NAME
 */
int tc_getmyport(struct t_myname *name)
{
	struct cx_myname *p = (struct cx_myname *) name->t_mn;
	union cx_unisel	*p1;
	int	i;
	char	tsel[CXS_SIZE + 1];
	int	sel_cnt;

	sel_cnt = (((struct t_myname *) name)->t_mnlng - (CXN_PRESIZE +
		CXN_FIXSIZE))/sizeof (union cx_unisel);
	if (sel_cnt < 1 || sel_cnt > CXN_SELNUM)
		return (0);
	p1 = &p->cxn_sel[sel_cnt];
	while (sel_cnt--) {
		p1--;
		if (p1->cxs.cx_type == CX_LANINET) {
			if (p1->cxs.cxs_lng < 1 || p1->cxs.cxs_lng > CXS_SIZE)
				return (0);
			for (i = 0; i < (int) p1->cxs.cxs_lng; i++)
				tsel[i] = p1->cxs.cxs_rest[i];
			tsel[i] = 0;
			return ((int) strtol(tsel, NULL, 10));
		}
	}
	return (102); /* Default for RFC1006 */
}

/*
 * tc_getmytsel - get my T-selector from RFC1006/LANINET T-selector
 */
void tc_getmytsel(struct tc_app	*appl, struct tc_con *conp)
{
	struct cx_myname *p = (struct cx_myname *) appl->tc_name.t_mn;
	union cx_unisel	*p1;
	int	i;
	int	sel_cnt;

	sel_cnt = (appl->tc_name.t_mnlng - (CXN_PRESIZE +
		CXN_FIXSIZE))/sizeof (union cx_unisel);
	if (sel_cnt < 1 || sel_cnt > CXN_SELNUM)
		return;				/* (-1) */
	p1 = &p->cxn_sel[sel_cnt];
	while (sel_cnt--) {
		p1--;
		if (p1->cxs.cx_type == CX_RFC1006) {
			if (p1->cxs.cxs_lng < 1 || p1->cxs.cxs_lng > CXS_SIZE)
				return;		/* (-1) */
			conp->tc_tcallnglen = p1->cxs.cxs_lng;
			memcpy(conp->tc_tselng, p1->cxs.cxs_rest,
							conp->tc_tcallnglen);
			return;			/* (1) */
		}
	}

	/* local name does not contain CX_RFC1006 */

	sel_cnt = (appl->tc_name.t_mnlng - (CXN_PRESIZE +
		CXN_FIXSIZE))/sizeof (union cx_unisel);
	p1 = &p->cxn_sel[sel_cnt];
	while (sel_cnt--) {
		p1--;
		if (p1->cxs.cx_type == CX_LANINET) {
			if (p1->cxs.cxs_lng < 1 || p1->cxs.cxs_lng > CXS_SIZE)
				return;		/* (-1) */
			conp->tc_tcallnglen = 2;
			i= atoi(p1->cxs.cxs_rest);
			(void) tc_us2uc((u_short) i,(u_char *) conp->tc_tselng);
			return;			/* (1) */
		}
	}
	return;					/* (-1) */
}

/*
 * tc_gethisport - get peer port and tsel from peer's  TRANSPORT ADDRESS
 */
void tc_gethisport(struct t_partaddr *paddr, struct tc_con *hconp)
{
	int	hplen, i;
	uchar	*hp;
	uchar   htsel[34],hport[4];

	hp = (uchar *) &(((struct sbka_addr *) paddr->t_pa)->ka_rest[0]);
	hplen = (int) (((struct sbka_addr *) paddr->t_pa)->ka_size);

	/* get peer T-selector */
	i = cxtlv_find_type(hp,hplen,0x80,htsel,34);
	if (i >0) {
		hconp->tc_tcalledlen=CXTLV_VAL_LENGTH(htsel);
		memcpy((char *)hconp->tc_tseled,CXTLV_VALUE(htsel),hconp->tc_tcalledlen);
	}
	else {
		_t_error = T_WPARAMETER;
		return;			/* T_ERROR */
	}

	/* get peer port number */
	if ((((struct sbka_addr *) paddr->t_pa)->cx_type) == CX_RFC1006) {
		hconp->tc_partner = RFCV40;
		if (cxtlv_find_type(hp,hplen,0x81,hport,4) <= 0)
			hconp->tc_portnr = 102;
		else
			(void) tc_uc2us(CXTLV_VALUE(hport),&hconp->tc_portnr);
	}
	else {
		hconp->tc_partner = RFCV30;
		hconp->tc_portnr = (u_short) (atoi(hconp->tc_tseled));
		hconp->tc_tcalledlen=2;
		tc_us2uc(hconp->tc_portnr,(u_char *) hconp->tc_tseled);
	}
	return;				/* 0 */
}

/*
 * tc_gethisaddr - get peer's internet address from its TRANSPORT ADDRESS
 */
char *tc_gethisaddr (struct t_partaddr *paddr)
{
	struct sbka_addr *sbp = (struct sbka_addr *) paddr->t_pa;

	if (paddr->t_palng < CXP_PRESIZE
		|| paddr->t_palng > (CXP_PRESIZE + T_PASIZE)
		|| ((sbp->cx_type != CX_LANINET) 
			&& (sbp->cx_type != CX_RFC1006))
		|| sbp->ka_size < CX_INADDR_OFF
		|| sbp->ka_size > SBKA_ADDRSIZE)
		return (NULL);
	return (&sbp->ka_rest[CX_INADDR_OFF]);
}

/*
 * tc_sethisaddr - set peer's TRANSPORT ADDRESS from internet address 
 */

static unsigned char	tc_nsap[] = {	/* NSAP part of TRANSPORT ADDRESS of peer */
	0x30, 0x0d,		/* type for NSAP and length of NSAP */
	0x03, 0x49, 0x00, 0x00, /* length of IDP and IDP */
	0x00,			/* null IDI */
	0x07, 0x00, 0x01,	/* length of DSP and SN-I */
	0x00, 0x00, 0x00, 0x00,	/* SN-A == Internet address */
	0xfe			/* NSEL */
};

void tc_sethisaddr(struct tc_con *conp)
{
	struct t_partaddr *paddr = (struct t_partaddr *) &conp->tc_addr;
	struct sbka_addr *sbp = (struct sbka_addr *) paddr->t_pa;
	int	i, j = 1;
	char	*tselp;

	/* address format and size */
	if (conp->tc_partner == RFCV40) {
		sbp->cx_type = CX_RFC1006;
		sbp->ka_type = SBKA_TYPE;
		sbp->ka_size = sizeof (tc_nsap) + 2 * sizeof (char) + conp->tc_tcallnglen;
	} else {
		i = (conp->tc_portnr & 0xffff);
		while (i /= 10)
			j++;		/* # of digits in port number */
		sbp->cx_type = CX_LANINET;
		sbp->ka_type = SBKA_TYPE;
		sbp->ka_size = sizeof (tc_nsap) + 2 * sizeof (char) + j;
	}

	/* NSAP address */
	memcpy((char *) sbp->ka_rest, tc_nsap, sizeof (tc_nsap));
	((struct in_addr *) &sbp->ka_rest[CX_INADDR_OFF])->s_addr =
		conp->tc_inaddr.s_addr;

	/* TSEL */
	tselp = sbp->ka_rest + sizeof (tc_nsap);
	*tselp++ = 0x80;		/* type for TSEL */
	if ((conp->tc_partner == RFCV30) || (conp->tc_partner == RFCV25)) {
		*tselp++ = j;			/* length of TSEL */
		prt10(conp->tc_portnr & 0xffff, tselp);
	} else {
		*tselp++ = conp->tc_tcallnglen; /* length of TSEL */
		memcpy(tselp,&conp->tc_tselng,conp->tc_tcallnglen);

	}

	/* external description */
	paddr->t_pamode = T_PAMODE;
	paddr->t_pares = 0;
	paddr->t_palng = CXP_PRESIZE + CXA_FIXSIZE + sbp->ka_size;
}

/*
 * prt10 - print in decimals
 */
static void prt10(register int num, char *str)
{
	register int	i;
	register char	*t;
	char		temp[11];

	t = temp;
	t[0] = '\0';
	for (i = 1; i <= 10; i++) {
		t[i] = num % 10 + '0';
		num /= 10;
	}
	for (i = 10; t[i] == '0'; i--)
		continue;
	if (i == 0)
		i++;
	while (i > 0)
		*str++ = t[i--];
}

/*------------------------------------------------------------------------*
 * HANDLING FOR APPLICATION AND CONNECT CHAINS
 *------------------------------------------------------------------------*/

/*
 * tc_rd_free - free connect element for redirected connection establishment
 */
void tc_rd_free(struct tc_app *app)
{
	int	fd = app->tc_rconp->tc_socket_fd;

	close(fd);
	FD_CLR(fd, &t_fdset);
	FD_CLR(fd, &t_rfds);
	t_trc(T_FCLOSE, 0, fd);
	free((char *) app->tc_rconp);
	app->tc_rconp = NULLCP;
	tc_p.tc_flags.tc_nfound = 0;
}

/*
 * tc_con_ins - link connect element into chain anchored in application
 */
void tc_con_ins(struct tc_app *app, struct tc_con *insptr)
{
	struct tc_con *hconp;
	int	ind = -1;

	if (!(app->tc_conp)) {
		insptr->tc_nxt = insptr->tc_prv = insptr;
		insptr->tc_ind = 1;
		app->tc_conp = insptr;
		return;			/* 1 */
	}
	hconp = app->tc_conp;
	do {
		if ((ind = ((app->tc_conp->tc_ind + 1) & TC_CON_MASK))
		    != app->tc_conp->tc_nxt->tc_ind) {
			insptr->tc_nxt = app->tc_conp->tc_nxt;
			insptr->tc_prv = app->tc_conp;
			insptr->tc_nxt->tc_prv = app->tc_conp->tc_nxt = insptr;
			insptr->tc_ind = ind;
			app->tc_conp = insptr;
			return;		/* ind */
		}
		app->tc_conp = app->tc_conp->tc_nxt;
	} while (hconp != app->tc_conp);

	abort();	/* CANNOT HAPPEN */
	return;		/* T_ERROR */
}

/*
 * tc_getconind  - get next index for constructing srcref (for conrq)  
 */
static unsigned int tc_getconind(struct tc_app *app)
{
	struct tc_con *hhconp;
	int	ind = -1;

	if (!(app->tc_conp)) {
		return (1);
	}
	hhconp = app->tc_conp;
	do {
		if ((ind = ((app->tc_conp->tc_ind + 1) & TC_CON_MASK))
		    != app->tc_conp->tc_nxt->tc_ind) {
			return (ind);
		}
		app->tc_conp = app->tc_conp->tc_nxt;
	} while (hhconp != app->tc_conp);

	abort();	/* CANNOT HAPPEN */
	return (T_ERROR);
}

/*
 * tc_con_del - unlink connect element from chain
 */
void tc_con_del(struct tc_app *app, struct tc_con *delptr)
{
	if (!(app->tc_conp && delptr))
		return;			/* T_ERROR */

	if (delptr->tc_nxt == delptr)
		app->tc_conp = NULLCP;
	else {
		delptr->tc_prv->tc_nxt = delptr->tc_nxt;
		delptr->tc_nxt->tc_prv = delptr->tc_prv;
		app->tc_conp = delptr->tc_nxt;
	}
	free((char *)delptr);
	return;				/* T_OK */
}

/*
 * tc_get_conptr - search connect element by index
 */
struct tc_con *tc_get_conptr(struct tc_app *app, int ind)
{
	struct tc_con *hconp;

	if (app->tc_conp) {
		hconp = app->tc_conp;
		do {
			if (app->tc_conp->tc_ind == ind)
				return (app->tc_conp);
			app->tc_conp = app->tc_conp->tc_nxt;
		} while (hconp != app->tc_conp);
	}

	return (NULLCP);
}

/*
 * tc_srch_app - search application element by port number
 */
struct tc_app *tc_srch_app(int port)
{
	struct tc_app *happ;

	if (tc_p.tc_apptr) {
		happ = tc_p.tc_apptr;
		do {
			if (tc_p.tc_apptr->tc_portnr == port)
				return (tc_p.tc_apptr);
			tc_p.tc_apptr = tc_p.tc_apptr->tc_nxt;
		} while (happ != tc_p.tc_apptr);
	}

	return (NULLAP);
}


/*
 * tc_get_app -  search application element by index
 */
struct tc_app *tc_get_apptr(int ind)
{
	struct tc_app *happ;

	if (tc_p.tc_apptr) {
		happ = tc_p.tc_apptr;
		do {
			if (tc_p.tc_apptr->tc_ind == ind)
				return (tc_p.tc_apptr);
			tc_p.tc_apptr = tc_p.tc_apptr->tc_nxt;
		} while (happ != tc_p.tc_apptr);
	}

	return (NULLAP);
}

/*
 * tc_app_ins - link application element into chain
 * NOTE: application chain is sorted in ascending order with respect to index
 */
void tc_app_ins(struct tc_app *insptr)
{
	struct tc_app *saveptr;
	int	ind = -1;

	if (!(tc_p.tc_apptr)) {
		insptr->tc_nxt = insptr->tc_prv = insptr;
		insptr->tc_ind = 0;
		tc_p.tc_apptr = insptr;
		return;				/* 0 */
	}
	saveptr = tc_p.tc_apptr;
	do {
		if ((ind = ((tc_p.tc_apptr->tc_ind + 1) & TC_APP_MASK))
		    != tc_p.tc_apptr->tc_nxt->tc_ind) {
			insptr->tc_nxt = tc_p.tc_apptr->tc_nxt;
			insptr->tc_prv = tc_p.tc_apptr;
			insptr->tc_nxt->tc_prv = tc_p.tc_apptr->tc_nxt = insptr;
			insptr->tc_ind = ind;
			tc_p.tc_apptr = insptr;
			return;			/* ind */
		}
		tc_p.tc_apptr = tc_p.tc_apptr->tc_nxt;
	} while (tc_p.tc_apptr != saveptr);

	abort();	/* CANNOT HAPPEN */
	return;		/* T_ERROR */
}

/*
 * tc_app_del - unlink application element from chain
 */
static void tc_app_del(struct tc_app *delptr)
{
	if (!(tc_p.tc_apptr && delptr))
		return;			/* T_ERROR */
	if ((tc_p.tc_apptr)->tc_nxt == tc_p.tc_apptr)
		tc_p.tc_apptr = NULLAP;
	else {
		if (tc_p.tc_apptr == delptr)
			tc_p.tc_apptr = delptr->tc_nxt;
		delptr->tc_prv->tc_nxt = delptr->tc_nxt;
		delptr->tc_nxt->tc_prv = delptr->tc_prv;
	}
	free((char *)delptr);
	return;				/* T_OK */
}

/*
 * tc_app_free - unlink application element from chain
 * shutdown its connections, close its sockets and free its connect elements
 */
void tc_app_free(struct tc_app *aptr, int closeit, int discon)
{
	register int	fd;

	if (aptr->tc_pconp != NULLCP) {
		fd = aptr->tc_pconp->tc_socket_fd;
		FD_CLR(fd, &t_fdset);
		FD_CLR(fd, &t_rfds);
		close(fd);
		t_trc(T_FCLOSE, 0, fd);
		free((char *) aptr->tc_pconp);
		aptr->tc_pconp = NULLCP;
		tc_p.tc_flags.tc_nfound = 0;
	}
	if (aptr->tc_rconp != NULLCP)
		tc_rd_free(aptr);
	while (aptr->tc_conp) {
		fd = aptr->tc_conp->tc_socket_fd;
		if (discon) {
			/*
			 * Shutdown a pending or established connection with
			 * reason T_USER. Close socket, unlink connect element.
			 */
			if (aptr->tc_conp->tc_state == T_INCONPEND
			    || aptr->tc_conp->tc_state == T_DATTRFRDY)
				(void) tc_sndfrm(DISRQ, aptr->tc_conp, T_USER, NULL);
		}
		if (closeit) {
			FD_CLR(fd, &t_fdset);
			FD_CLR(fd, &t_rfds);
			FD_CLR(fd, &t_wfds);
			close(fd);
			t_trc(T_FCLOSE, 0, fd);
		}
		tc_con_del(aptr, aptr->tc_conp);
		tc_p.tc_flags.tc_nfound = 0;
	}
	tc_app_del(aptr);
}

void tc_setsock(int fd,int mode)
{
extern	struct t_trcflag t_trcflg;
	int res, on=1, tmpfctl, pcb_addr, pcb_len=sizeof(int);
	struct linger linger;

	/* set socketoption REUSEADDR */
	res=setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	t_trc(T_FTCPSSOPT,0,fd, SOL_SOCKET, SO_REUSEADDR, on, sizeof(on),res,errno);

	if (mode == TC_NBLOCK) {
		/* set non-blocking-mode */
		tmpfctl = fcntl(fd, F_GETFL, 0);
 		(void) fcntl(fd, F_SETFL, tmpfctl | O_NONBLOCK);
	}

	/* set socket-trace */
	if (t_trcflg.deb == T_LIBDEBUG + T_TCPDEBUG) {
		/*
		* TCP/IP-socket-trace requested
		*
		* Trace handling for TCP/IP socket trace as follows:
	 	*   setsockopt (set debug-mode)
	 	*   getsockopt (get pcb-address) 
 	 	*/
		res=setsockopt(fd, SOL_SOCKET, SO_DEBUG, (char *)&on, sizeof(on));
		t_trc(T_FTCPSSOPT, 0, fd, SOL_SOCKET, SO_DEBUG, on, sizeof(on), res, errno);
#define SO_PCBADDR 0x1101 /* later in sys/socket.h */
		res=getsockopt(fd, SOL_SOCKET, SO_PCBADDR, (char *)&pcb_addr, (int *)&pcb_len);
		t_trc(T_FTCPGSOPT, 0, fd, SOL_SOCKET, SO_DEBUG, pcb_addr, sizeof(pcb_addr), res, errno);
	}

	/* set socket option KEEPALIVE */
	if (sock_keep) {
		res=setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&sock_keep, sizeof(sock_keep));
		t_trc(T_FTCPSSOPT,0,fd, SOL_SOCKET, SO_KEEPALIVE, sock_keep, sizeof(sock_keep),res,errno);
	}

	/* set socket option LINGER */
	if (sock_linger) {
		linger.l_linger = sock_linger;
		linger.l_onoff = 1;
		res=setsockopt(fd, SOL_SOCKET, SO_LINGER, (char *)&linger, sizeof(linger));
		t_trc(T_FTCPSSOPT,0,fd, SOL_SOCKET, SO_LINGER, sock_linger, sizeof(linger),res,errno);
	}

	return;
}

#if defined(SCO)

static int writev(int fd, struct iovec *iov, int iovcnt)

{
	int	nwritev, byte_cnt = 0;
	struct iovec *last_iov = iov + iovcnt -1;

	if (iovcnt < 1) { 
		errno=EINVAL;
		return (-1); 
	}
	do {
		if (iov->iov_len < 0)  { 
			errno=EINVAL;
			return (-1); 
		}
		while ((nwritev = write(fd, iov->iov_base, iov->iov_len) 
                 == -1) && errno == EINTR)
			t_trc(T_FWRITE, 0, sd, 0, i, cc, errno);
                   
		t_trc(T_FWRITE, 0, sd, 0, i, cc, errno);

		/* EAGAIN is equal to EWOULDBLOCK */
		if (((nwritev == -1) && (errno != EAGAIN)) ||
					/* Unrecoverable error */
	     	(nwritev == 0))         /* no TCP-connection */
			return (T_ERROR);

		if (nwritev != -1)
			byte_cnt += nwritev;
		else
			return(byte_cnt);

		iov++;
	} while (iov <= last_iov);
	return (byte_cnt);
}
#endif
