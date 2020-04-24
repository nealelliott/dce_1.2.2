/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxxrd.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:33  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:55  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:48:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:41  root]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:41  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:22:40  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:48:25  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:46:46  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:43  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:54  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:41  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:49  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxxrd.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:33 $";
#endif

/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG  All rights reserved
 * COPYRIGHT (C) Siemens Informationssysteme AG 1993
 *               All Rights Reserved
 *
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/*
static char	sccsid[] = "@(#)cmxxrd.c	311.7 93/08/24 CMX (NC1)";
*/

/****************************************************************************/
/*                                                                          */
/*                INTERFACE ICMX(L) ON TOP OF XTI/TLI                       */
/*                ===================================                       */
/*                                                                          */
/* This code is a mapping of ICMX(L) as defined by CMX V4.0 onto            */
/* XTI or TLI. Its roots are CMX(L) ON TOP OF THE TCP/IP and a similar	    */
/* proprietary implementation done by AP			            */
/* This adaption adds major enhancements, higher robustness and the new     */
/* options available with CMX V4.0.                                         */
/*                                                                          */
/* Author: mai                                           Date:    22.01.93  */
/*                                                                          */
/* Defines:                                                                 */
/*                                                                          */
/* NOTE:                                                                    */
/* 									    */
/* Identifier starts with "tx" instead of "tc" (cmxinet).		    */
/* 									    */
/* 									    */
/*                                                                          */
/****************************************************************************/

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include <errno.h>
#include <time.h>
#include <sys/time.h>
#ifdef	__TURBOC__
#include <sys/signal.h>
#include <process.h>
#include <dos.h>
#define  sigemptyset(x)
unsigned int alarm (unsigned int);
int sigaction (int, struct sigaction *, struct sigaction *);
int fcntl (int, int, ...);
#else
#include <signal.h>
#endif
#include <fcntl.h>
#if defined(_AIX)
#include <sys/select.h>
#endif

#include "cmx.h"
#undef	T_MORE
#undef	T_CHECK

#include "xti.h"
#undef	T_MORE
#undef	T_CHECK

#include "cmxextern.h"
#include "cmxtrace.h"
#include "cmxaddr.h"
#include "cmxxti.h"
#include "cmxxtr.h"

#ifdef	ALLOC_TEST
#include "alloctrc.h"
#endif	/* ALLOC_TEST */

/* --------------------------------------------------------------------	*/
/* REDIRECT								*/
/* --------------------------------------------------------------------	*/
/*
 * Redirection should be done one level above for all
 * library solutions. If, for example, a t_myname
 * contains TSELs for TCP/IP and XTI, 2 paths are set up for the
 * connection redirection, one using sockets for TCP/IP,
 * the other using LOOPSBKA for the XTI/TLI solution.
 *
 * Connection redirection does not require a normal application block
 * and connection block. It requires (especially with addressing)
 * a different processing approach. To enable another solution to
 * be integrated for the connection redirection if necessary, this connection
 * was totally disolved from the normal administration structures.
 */


typedef struct {
   int			pid;	/* parent process			*/
   int			fd;	/* fd which should be redirected	*/
   int			ladrl;	/* length of tsel			*/
   struct t_info	info;
   int                  flwctrl;/* flow control state at user level	*/
   int			mltidu;	/* maxl					*/
   int			udatal;	/* userdata				*/
   char			udata[T_RED_SIZE];
} txred_rq;

typedef struct {
   int	pid;			/* child process			*/
   int	ack;			/* 0 when ok				*/
} txred_ack;

typedef struct {
   int		fd;
   txred_rq	data;
} txred_in;

/*
 * static data only for incomming redirections
 */
static txred_in	sv_redin = {-1, {0}};

/*
** With t_timeout, there is a little problem. Also in success case
** t_connect takes a little time, especially on a heavy loaded computer,
** so I define, also for the case of T_NO a minimum for t_timeout.
*/
#define	MIN_TIMEOUT	5	/* seconds */

/*
** Maximum address length for the redirection connection
*/
#define	ML_RADR		256


static void p_alarm	(int	pv_sig)
{
   alarm (1);
}

static int p_rcv	(int     pv_fd,
			 char	*pv_buf,
			 int	 pv_nbytes)
{
   int	v_nbytes;
   int	v_flag;

   while (pv_nbytes != 0) {
      while ((v_nbytes = T_rcv (pv_fd, pv_buf, pv_nbytes, &v_flag)) == -1 &&
	     t_errno == TSYSERR && errno == EINTR);

      if (v_nbytes == -1)
	 return (-1);

      pv_nbytes -= v_nbytes;
      pv_buf += v_nbytes;
   }

   if (v_flag != 0)
      return (-1);

   return (0);
}

static int p_to_idle	(int		 pv_fd,
			 char		*pv_tp,
			 struct t_bind	*pv_req)
{
   int	v_rc;

   if (pv_fd != -1) {
      v_rc = T_getstate (pv_fd);
      if (v_rc == T_DATAXFER || v_rc == T_INCON || v_rc == T_OUTCON ||
	  v_rc == T_OUTREL || v_rc == T_INREL) {
	 if (T_look(pv_fd) == T_DISCONNECT)
	    T_rcvdis (pv_fd, NULL);
	 else
	    T_snddis (pv_fd, NULL);
      }

      if (T_getstate (pv_fd) == T_IDLE) {
	 /* statt T_unbind (pv_fd); */
	 return (pv_fd);
      }

      T_close (pv_fd);
   }

   if ((pv_fd = T_open (pv_tp, O_RDWR, NULL)) == -1 ||
       T_bind (pv_fd, pv_req, NULL) == -1 ) {
      _t_error = tx_maperror (t_errno, errno);

      if (pv_fd != -1)
	 T_close (pv_fd);
      return (-1);
   }

   return (pv_fd);
}

/* -------------------------------------------------------------------- *
 * tx_redrq                         					*
 *              							*
 * return value:                                                        *
 *	0	ok							*
 *     -1	error, _t_error set					*
 *      n       error, reason why the other process didn't accept       *
 * -------------------------------------------------------------------- */
int tx_redrq	(tx_con		*pv_conp,
		 struct netbuf	*pv_ladr,
		 int		 pv_pid,
		 int		 pv_timeout,
		 char		*pv_udatap,
		 int		 pv_udatal)
{
   char		       *v_tp;
   int			v_rc;
   int			v_fd = -1;
   int			v_atime;	/* alarm time		*/
   time_t		v_etime;	/* end time		*/
   time_t		v_stime;	/* sleep time		*/
   time_t	        v_usrtime;	/* user alarm time	*/
   union cx_unisel	v_tsel;
   struct t_bind	v_req = {{0, 0, NULL}, 0};
   struct t_call	v_call;
   txred_rq		v_redrq;
   txred_ack		v_redack;
   struct sigaction	v_act, v_oact;

   /*
   ** build own and partner address
   */
   v_tsel.cxs.cx_type = CX_LOOPSBKA;
   v_tsel.cxs.cxs_lng = CXS_SIZE;
   /* v_tsel.cxs.cxs_rest will currently not be avaluated	*/

   if ((v_rc = tx_amsel (&v_tsel, &v_req.addr, &v_tp, t_pid, 0)) < 0)
      return (-1);
   if (v_rc == 0) {
      /*
       * CX_LOOPSBKA not found in cmxxticfg
       */
      _t_error = T_W_NDS_ACCESS;
      goto errout_nomap;
   }

   memset (&v_call, 0, sizeof(v_call));
   if (tx_amaddr (CX_LOOPSBKA, &v_call.addr, NULL, pv_pid) == -1)
      goto errout_nomap;

   /*
   ** Preparations
   */
   if (pv_timeout == T_NOLIMIT)
      v_etime = 0xffffffffL;
   else
      v_etime = time (NULL) + pv_timeout;

   v_usrtime = alarm (0) + time (NULL);

   v_act.sa_handler = p_alarm;
   sigemptyset (&v_act.sa_mask);
   v_act.sa_flags = 0;

   /*
   ** establish connection
   */
   do {
      if ((v_fd = p_to_idle (v_fd, v_tp, &v_req)) == -1)
	 goto errout_nomap;

      if (pv_timeout == T_NO)
	 v_atime = MIN_TIMEOUT;
      else {
	 if ((v_atime = v_etime - time (NULL)) > MIN_TIMEOUT) {
	     v_stime = 2;
	     v_atime = MIN_TIMEOUT;
	 }
	 else
	    v_stime = 1;
      }

      sigaction (SIGALRM, &v_act, &v_oact);
      alarm (v_atime);
      v_rc = T_connect (v_fd, &v_call, NULL);
      alarm (0);
      sigaction (SIGALRM, &v_oact, NULL);

      if (v_usrtime != 0) {
	 if ((v_atime = v_usrtime - time (NULL)) < 0)
	    v_atime = 2;
	 alarm (v_atime);
      }

      if (v_rc == -1 && t_errno == TBUFOVFLW)
	 v_rc = 0;
      if (v_rc == -1) {
	 if (t_errno == TSYSERR && errno == EINTR) {
	    /*
	    ** interrupted by an signal, by one strange TLI-implementation
	    ** I get it always. Try to get the connection.
	    */
	    if (T_getstate (v_fd) == T_OUTCON &&
		T_look (v_fd) == T_CONNECT) {
	       if ((v_rc = T_accept (v_fd, v_fd, NULL)) == -1 &&
		   t_errno == TBUFOVFLW)
		  v_rc = 0;
	    }
	 }
      }

      if (v_rc == -1) {
	 if (pv_timeout == T_NOLIMIT)
	    goto errout;
	 if (pv_timeout == T_NO) {
	    _t_error = T_WPARAMETER;
	    goto errout_nomap;
	 }
	 if ((v_etime - time(NULL) <= 0)) {
	    _t_error = T_ETIMEOUT;
	    goto errout_nomap;
	 }
	 sleep (v_stime);
      }
   } while (v_rc == -1);

   free (v_call.addr.buf);
   free (v_req.addr.buf);
   v_call.addr.buf = v_req.addr.buf = NULL;

   v_redrq.pid	   = t_pid;
   v_redrq.fd      = pv_conp->fd;
   v_redrq.ladrl   = pv_ladr->len;
   v_redrq.info    = pv_conp->info;
   v_redrq.flwctrl = pv_conp->flwctrl;
   v_redrq.mltidu  = pv_conp->mltidu;

   if ((v_redrq.udatal = pv_udatal) != 0)
      memcpy (v_redrq.udata, pv_udatap, pv_udatal);

   if (T_snd (v_fd, (char *)&v_redrq, sizeof(v_redrq), 0) != sizeof(v_redrq))
      goto errout;
   if (T_snd (v_fd, pv_ladr->buf, pv_ladr->len, 0) != pv_ladr->len)
      goto errout;

   if (p_rcv (v_fd, (char *)&v_redack, sizeof (v_redack)) != 0)
      goto errout;

   if (v_redack.ack != 0)
      _t_error = v_redack.ack;

   T_snddis (v_fd, NULL);
   T_unbind (v_fd);
   T_close  (v_fd);

   return (v_redack.ack);

errout:
   _t_error = tx_maperror (t_errno, errno);
errout_nomap:
   if (v_call.addr.buf != NULL)
      free (v_call.addr.buf);
   if (v_req.addr.buf != NULL)
      free (v_req.addr.buf);

   if (v_fd != -1)
      T_close (v_fd);
   return (-1);
}

/* -------------------------------------------------------------------- *
 * tx_redopen                         					*
 *                              					*
 * return value:                                                        *
 *	0 ok                                                            *
 *     -1 error                                                         *
 *      1 not first (redirection not possible				*
 * -------------------------------------------------------------------- */
int tx_redopen	(void)
{
   char		       *v_tp;
   int			v_rc;
   union cx_unisel	v_tsel;
   struct t_bind	v_req = {{0, 0, NULL}, 1};
   struct t_bind	v_ret = {{ML_RADR, 0, NULL}, 0};
   char			v_bufret [ML_RADR];

   /*
    * we need only one listening fd for redirection
    */
   if (sv_redin.fd != -1)
      return (0);

   v_ret.addr.buf = v_bufret;	/* some compilers want it this way	*/

   v_tsel.cxs.cx_type = CX_LOOPSBKA;
   v_tsel.cxs.cxs_lng = CXS_SIZE;
   /* v_tsel.cxs.cxs_rest will currently not be avaluated	*/

   if ((v_rc = tx_amsel (&v_tsel, &v_req.addr, &v_tp, t_pid, 1)) < 0)
      return (-1);
   if (v_rc == 0) {
      /*
       * CX_LOOPSBKA not found in cmxxticfg
       */
      _t_error = T_W_NDS_ACCESS;
      return (-1);
   }

   if ((sv_redin.fd = T_open (v_tp, O_RDWR, NULL)) == -1) {
      _t_error = tx_maperror (t_errno, errno);
      free (v_req.addr.buf);
      return (-1);
   }

   fcntl (sv_redin.fd, F_SETFD, (fcntl (sv_redin.fd, F_GETFD) | FD_CLOEXEC));

   if (T_bind (sv_redin.fd, &v_req, &v_ret) == -1 ) {
      _t_error = tx_maperror (t_errno, errno);
      T_close (sv_redin.fd);
      sv_redin.fd = -1;
      free (v_req.addr.buf);
      return (-1);
   }

   if (v_req.addr.len != v_ret.addr.len ||
       memcmp (v_req.addr.buf, v_ret.addr.buf, v_ret.addr.len) != 0) {
      T_close (sv_redin.fd);
      sv_redin.fd = -1;
      free (v_req.addr.buf);
      return (1);
   }
   free (v_req.addr.buf);

   FD_SET(sv_redin.fd, &t_fdset);
   FD_SET(sv_redin.fd, &t_xfds);
   FD_SET(sv_redin.fd, &t_rfds);

   return (0);
}
/* -------------------------------------------------------------------- *
 * tx_redclose                         					*
 * -------------------------------------------------------------------- */
void tx_redclose	(void)
{
   /*
    * close the listening fd for redirection
    */
   if (sv_redin.fd != -1) {
      T_close (sv_redin.fd);
      FD_CLR(sv_redin.fd, &t_fdset);
      FD_CLR(sv_redin.fd, &t_xfds);
      FD_CLR(sv_redin.fd, &t_rfds);
      sv_redin.fd = -1;
   }
}

/* -------------------------------------------------------------------- *
 * tx_redevent                         					*
 *									*
 * return value:							*
 *	0 nothing to do							*
 *     -1 error								*
 *	1 data for T_REDIRECT ready					*
 * -------------------------------------------------------------------- */
int tx_redevent (struct netbuf	*pv_ladr)
{
#define TMPLNG	1024
   int			v_rc;
   struct t_call	v_call = {{TMPLNG, 0, NULL},
				  {TMPLNG, 0, NULL},
				  {TMPLNG, 0, NULL}, 0};
   char			v_mist [TMPLNG];

   pv_ladr->buf = NULL;

   if (sv_redin.fd == -1 ||
       (v_rc = tx_look (sv_redin.fd)) == 0)
      return (0);

   if (v_rc != T_LISTEN)
      return (-1);

/*
   memset (&v_call, 0, sizeof(v_call));
*/
   v_call.addr.buf  = v_mist;
   v_call.opt.buf   = v_mist;
   v_call.udata.buf = v_mist;
   if ((v_rc = T_listen (sv_redin.fd, &v_call)) == -1 && t_errno != TBUFOVFLW) {
      _t_error = tx_maperror (t_errno, errno);
      return (-1);
   }

   if ((v_rc = T_accept (sv_redin.fd, sv_redin.fd, &v_call)) == -1 ||
       p_rcv (sv_redin.fd, (char *)&sv_redin.data, sizeof (sv_redin.data))
	      != 0) {
      goto errout;
   }

   pv_ladr->len = sv_redin.data.ladrl;
   if ((pv_ladr->buf = malloc (pv_ladr->len)) == NULL) {
      _t_error = T_ENOMEM;
      goto errout_e;
   }

   if (p_rcv (sv_redin.fd, pv_ladr->buf, pv_ladr->len) != 0)
      goto errout;

   return (1);

errout:
   _t_error = tx_maperror (t_errno, errno);
errout_e:
   tx_redclose();
   tx_redopen ();

   if (pv_ladr->buf != NULL)
      free (pv_ladr->buf);

   return (-1);
}

/* -------------------------------------------------------------------- *
 * tx_redin                         					*
 * -------------------------------------------------------------------- */
int tx_redin    (tx_con			*pv_conp,
		 int			 pv_ack)
{
   txred_ack	v_ack;
   int		v_rc = 0;

   if (pv_conp != NULL) {
      pv_conp->fd      = sv_redin.data.fd;
      pv_conp->info    = sv_redin.data.info;
      pv_conp->flwctrl = sv_redin.data.flwctrl;
      pv_conp->mltidu  = sv_redin.data.mltidu;
      pv_conp->ucepid  = sv_redin.data.pid;

      if ((pv_conp->udata.len = MIN(sv_redin.data.udatal, T_RED_SIZE)) != 0) {
	 if ((pv_conp->udata.buf = malloc (pv_conp->udata.len)) == NULL) {
	    pv_ack = T_ENOMEM;
	    v_rc = -1;
	 }
	 else
	    memcpy (pv_conp->udata.buf, &sv_redin.data.udata,
		    pv_conp->udata.len);
      }

      /* Lazy t_sync without any check, just to have a valid fd */
      tx_sync (pv_conp->fd);
   }

   v_ack.pid = t_pid;
   v_ack.ack = pv_ack;

   if (T_snd (sv_redin.fd, (char *)&v_ack, sizeof(v_ack), 0)
       != sizeof(v_ack)) {
      tx_redclose();
      tx_redopen ();
   }
   else {
      fd_set		v_fds;
      struct timeval	v_tv;

      FD_ZERO(&v_fds);
      FD_SET(sv_redin.fd, &v_fds);
      memset (&v_tv, 0, sizeof (v_tv));
      v_tv.tv_sec=5;

      select (sizeof(v_fds)*8, &v_fds, NULL, NULL, &v_tv);

      if (T_look(sv_redin.fd) != T_DISCONNECT ||
	  T_rcvdis (sv_redin.fd, NULL) == -1) {
	 tx_redclose();
	 tx_redopen ();
      }
   }

   return (v_rc);
}
