/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxxup.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:42  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:01  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:49:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:53  root]
 * 
 * Revision 1.1.2.5  1994/07/18  13:40:50  marrek
 * 	Changes for threadsafe libxti.
 * 	[1994/07/18  12:12:05  marrek]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:45  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:23:07  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:48:28  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:47:04  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:55  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:02  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:52  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:16  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxxup.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:42 $";
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
static char	sccsid[] = "@(#)cmxxup.c	311.10 93/08/24 CMX (NC1)";
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
/* Public identifier starts with "tx".					    */
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
#define  sigemptyset(x)
#else
#include <signal.h>
#endif
#include <sys/types.h>
#include <fcntl.h>
#if defined(_AIX)
#include <sys/select.h>
#endif

#define NDEBUG		/* no assert */
#include <assert.h>

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

/*
 * There are some very poor TLI implementations. If we do not provide
 * an address, option, and user data buffer, even if we dont need the data,
 * we get some obscure behavior. So in these cases, we peovide a
 * dummy buffer. Another advantage is, that we see the delivered
 * data in the trace.
 */
static char		sv_addrb  [ML_ADDR];
static char		sv_optb   [ML_OPT];
static struct netbuf	sv_addr = {ML_ADDR,  0, sv_addrb  };
static struct netbuf	sv_opt  = {ML_OPT,   0, sv_optb   };
static struct netbuf	sv_nbnix= {0,        0, NULL      };
#define	NB_FIL(X,Y)	Y.len = 0; X = Y
#define	NB_CLR(X)	(X = sv_nbnix)

/*
 * NOTE:
 *	t_alloc and t_free are not used because:
 *	- alloc to much buffer, more then needed
 *	- give errors when one of the fields is -1 or -2 (info)
 */

/* -------------------------------------------------------------------- *
 * tx_accept                           					*
 *									*
 * TLOOK: T_DISCONNECT, T_LISTEN					*
 * -------------------------------------------------------------------- */
int tx_accept	(tx_app	*pv_app,
		 tx_con	*pv_con)
{
   int	v_rc, v_terrno, v_errno, v_fcntl;
   bool	v_cont = TRUE;

   /*
    * on some systems a nonbloking accept always returns
    * "TSYSERR - no more processes". 
    */
   v_fcntl = fcntl(pv_app->pconp->fd, F_GETFL, 0);

   while (v_cont) {
      fcntl (pv_app->pconp->fd, F_SETFL, v_fcntl & ~O_NONBLOCK);
      while ((v_rc = T_accept (pv_app->pconp->fd, pv_con->fd,
			       &pv_con->listq->call)) == -1 &&
	     t_errno == TSYSERR && errno == EINTR);
      fcntl (pv_app->pconp->fd, F_SETFL, v_fcntl);

      if (v_rc == 0) {
	 free (pv_con->listq);
	 pv_con->listq = NULL;
	 return (0);
      }

      if (t_errno == TLOOK) {
	 switch (tx_look (pv_app->pconp->fd)) {
	    case T_LISTEN:
	       tx_listen (pv_app);
	       break;
	    case T_DISCONNECT:
	       if (tx_rcvdisl (pv_app) == -1) {
		  /*
		   * I know the error could be on another connection,
		   * but if an error occurred with t_rcvdis, we are far away
		   * from normal operation and we have to avoid loops!
		   */
		  v_cont = FALSE;
		  break;
	       }
	       if (pv_con->listq == NULL) {	/* was it the current fd? */
		  _t_error = T_COLLISION;
		  return (-1);
	       }
	       break;
	    default:
	       t_trc (T_STRING, 0, "Default/t_look");
	       v_cont = FALSE;
	 }
      }
      else
	 v_cont = FALSE;
   }

   v_terrno = t_errno;
   v_errno  = errno;

   if (pv_con->listq != NULL) {
      tx_snddis (pv_app->pconp, &pv_con->listq->call);
      free (pv_con->listq);
      pv_con->listq = NULL;
   }

   pv_con->reason = T_RLCONNLOST;
   _t_error = tx_maperror (v_terrno, v_errno);

   return (-1);
}

/* -------------------------------------------------------------------- *
 * tx_bind                           					*
 *									*
 * no handling for t_errno TBUFOVFLW!					*
 * -------------------------------------------------------------------- */
int tx_bind	(tx_app		*pv_app,
		 tx_con		*pv_con,
		 unsigned	 pv_qlen)
{
   struct t_bind	v_req;
   struct t_bind	v_ret;
   int			v_rc;

   /*
    * addr can be copied from pv_app, it's already checked
    */
   v_req.addr = pv_app->addr;
   v_req.qlen = pv_qlen;

   /*
    * pv_qlen > 0 indicates a listening point for passive connection establ.
    * We have to check, if the TP automatically generated another address.
    */
   if (pv_qlen > 0) {
      if ((v_ret.addr.buf = malloc (v_req.addr.len)) == NULL) {
	 _t_error = T_ENOMEM;
	 return (-1);
      }
      v_ret.addr.maxlen = v_req.addr.len;
      v_ret.addr.len = 0;               /* just to avoid an XTI trace error */

      while ((v_rc = T_bind (pv_con->fd, &v_req, &v_ret)) == -1 &&
	     t_errno == TSYSERR && errno == EINTR);

      if (v_rc != -1) {
	 /*
	  *  check if bound address differ -> T_NOTFIRST
	  */
	 if (v_ret.addr.len != v_req.addr.len ||
	     memcmp (v_req.addr.buf, v_ret.addr.buf, v_ret.addr.len) != 0)
	    v_ret.qlen = 0;
	 free (v_ret.addr.buf);
	 return (v_ret.qlen);
      }

      free (v_ret.addr.buf);

      if (t_errno == TADDRBUSY || t_errno == TBUFOVFLW) {
         v_ret.qlen = 0;
         return (0);
      }

      return (-1);
   }

   while ((v_rc = T_bind (pv_con->fd, &v_req, NULL)) == -1 &&
	     t_errno == TSYSERR && errno == EINTR);

   if (v_rc != -1 || t_errno == TBUFOVFLW)
      return (0);

   _t_error = tx_maperror (t_errno, errno);

   /*
    * some TP's have difficulties when the bound address on
    * incomming connections differ. With XPG4 the addess for t_accept
    * may not be bound, so there is a chance to use the fd.
    */
   if (t_errno == TADDRBUSY)
      return (-2);
   else
      return (-1);
}

/* -------------------------------------------------------------------- *
 * tx_close                          					*
 * -------------------------------------------------------------------- */
int tx_close	(int	pv_fd)
{
   int	v_rc;

   while ((v_rc = T_close (pv_fd)) == -1 &&
	  t_errno == TSYSERR && errno == EINTR);

   if (v_rc == -1)
      _t_error = tx_maperror (t_errno, errno);

   return (v_rc);
}

/* -------------------------------------------------------------------- *
 * tx_connect                          					*
 *									*
 * Returncode:								*
 *	0 connection establishment initiated			        *
 *	1 connection full established					*
 *     -1 Error, _t_error set						*
 *									*
 * TLOOK: T_DISCONNECT							*
 * -------------------------------------------------------------------- */
int tx_connect	(tx_con		*pv_con,
		 struct t_call	*pv_sndcall)
{
   struct t_call	v_rcvcall;
   int			v_rc;

   /*
    * Its possible, that despite O_NONBLOCK the t_connect()
    * establishes the connection fully. Save the user-data
    * for the following t_concf() but ignore options.
    */
   memset (&v_rcvcall, 0, sizeof(v_rcvcall));
   if ((v_rcvcall.udata.buf = malloc(ML_UDATA)) == NULL) {
      _t_error = T_ENOMEM;
      return (-1);
   }
   v_rcvcall.udata.maxlen = ML_UDATA;

   while ((v_rc = T_connect (pv_con->fd, pv_sndcall, &v_rcvcall)) == -1 &&
	  t_errno == TSYSERR && errno == EINTR);

   if (v_rc != -1) {
      /*
       * connection fully established
       */
      pv_con->udata = v_rcvcall.udata;
      return (1);
   }

   free (v_rcvcall.udata.buf);

   if (t_errno == TNODATA) {
      /*
       * connection establishment initiated
       */
      return (0);
   }

   if (t_errno == TBUFOVFLW) {
      /*
       * we have a connection but lost some data
       */
      tx_snddis (pv_con, NULL);
      _t_error = tx_maperror (TBUFOVFLW, 0);
      return (-1);
   }

   /*
    * TLOOK/T_DISCONNECT seems only be possible in synchronous mode,
    * so we ignore it
    */
   _t_error = tx_maperror (t_errno, errno);
   return (-1);
}

/* -------------------------------------------------------------------- *
 * tx_getstate                        					*
 *									*
 * -------------------------------------------------------------------- */
int tx_getstate	(int	pv_fd)
{
   int	v_rc;

   /*
    * on SCO 3.2 getstate returns -1/TSTATECHNG without any noticable
    * reason for changing the state. But t_sync then returns the correct
    * state
    */ 
   while ((v_rc = T_getstate (pv_fd)) == -1 &&
      t_errno == TSYSERR && errno == EINTR);

#ifdef SCO
   if (v_rc == -1 && t_errno == TSTATECHNG)
      return (tx_sync (pv_fd));
   else
#endif

   return (v_rc);
}

/* -------------------------------------------------------------------- *
 * tx_listen                          					*
 *									*
 * TLOOK: T_DISCONNECT							*
 * -------------------------------------------------------------------- */
int tx_listen	(tx_app	*pv_app)
{
   tx_liststr	*v_listp, *v_lptr;
   tx_con	*v_conp;
   int		 v_rc;
   int 		 xti__errno = 0 ;
   bool		 v_cont = TRUE;

   if ((v_listp = calloc(1, sizeof(tx_liststr))) == NULL) {
      _t_error = T_ENOMEM;
      return (-1);
   }

   /* some systems return no sequence number, if there is no option buffer */
   v_listp->call.addr.maxlen  = ML_ADDR;
   v_listp->call.addr.buf     = v_listp->addr;
   NB_FIL (v_listp->call.opt, sv_opt);
   v_listp->call.udata.maxlen = ML_UDATA;
   v_listp->call.udata.buf    = v_listp->udata;

   v_conp = pv_app->pconp;

   while (v_cont) {
      while ((v_rc = T_listen (v_conp->fd, &v_listp->call)) == -1 &&
	     t_errno == TSYSERR && errno == EINTR);

      if (v_rc != -1) {
         NB_CLR (v_listp->call.opt);
	 /* chain it			*/
	 if ((v_lptr = v_conp->listq) == NULL)
	    v_conp->listq = v_listp;
	 else {
	    while (v_lptr->nxt != NULL)
	       v_lptr = v_lptr->nxt;
	    v_lptr->nxt = v_listp;
	 }
 	 return (0);
      }

      switch (t_errno) {
	 case TLOOK:
	    if (tx_look (v_conp->fd) == T_DISCONNECT) {
	       if (tx_rcvdisl (pv_app) == -1)
		  v_cont = FALSE;
	    }
	    else {
	       t_trc (T_STRING, 0, "Default/t_look");
	       v_cont = FALSE;
	    }
	    break;

	 case TBUFOVFLW:
	    /*
	     * we lost some data
	     */
	    tx_snddisl (pv_app, &v_listp->call);
	    xti__errno = TBUFOVFLW;
	    v_cont = FALSE;
	    break;

	 default:
	    v_cont = FALSE;
      }
   }

   _t_error = tx_maperror (xti__errno, errno);
   free (v_listp);
   return (-1);
}

/* -------------------------------------------------------------------- *
 * tx_look                           					*
 * -------------------------------------------------------------------- */
int tx_look	(int	pv_fd)
{
   int	v_look;

   while ((v_look = T_look (pv_fd)) == -1 &&
	  t_errno == TSYSERR && errno == EINTR);

   if (v_look == -1)
      _t_error = tx_maperror (t_errno, errno);

   return (v_look);
}

/* -------------------------------------------------------------------- *
 * tx_open                           					*
 * 									*
 * 									*
 * -------------------------------------------------------------------- */
int tx_open	(tx_app	*pv_app,
		 tx_con	*pv_con)
{
   int	v_fd;

   while ((v_fd = T_open (pv_app->tp, O_RDWR | O_NONBLOCK,
			  &pv_con->info)) == -1 &&
	  t_errno == TSYSERR && errno == EINTR);

   if (v_fd == -1) {
      _t_error = tx_maperror (t_errno, errno);
      return (-1);
   }

   fcntl (v_fd, F_SETFD, (fcntl (v_fd, F_GETFD) | FD_CLOEXEC));

   /*
    * tsdu = 0 : TP does not support the concept of TSDU
    * It's up to the user not to set T_MORE (DIR-X request)
    */
    if (pv_con->info.tsdu > t_tidusize || pv_con->info.tsdu == -1
	|| pv_con->info.tsdu == 0)
       pv_con->mltidu = t_tidusize;
    else
       if (pv_con->info.tsdu > 0)
	  pv_con->mltidu = (int)pv_con->info.tsdu;
       else {
	  /*
	   * TP does not support the transfer of normal data
	   */
	  tx_close (v_fd);
	  _t_error = T_NOCCP;
	  return (-1);
       }

   /*
    * normalize info-fields
    */
   if (pv_con->info.connect == -1)
      pv_con->info.connect = ML_UDATA;
   if ((pv_app->tpflag & TPF_NOCONNECT) != 0)
      pv_con->info.connect = 0;

   if (pv_con->info.discon == -1)
      pv_con->info.discon = ML_UDATA;
   if ((pv_app->tpflag & TPF_NODISCON) != 0)
      pv_con->info.discon = 0;

   if (pv_con->info.addr == -1)
      pv_con->info.addr = ML_ADDR;

   /*
    * is our address of proper size
    */
   if (pv_app->addr.len > pv_con->info.addr) {
      tx_close (v_fd);
      _t_error = T_WPARAMETER;
      return (-1);
   }

   return (v_fd);
}


/* -------------------------------------------------------------------- *
 * tx_rcv                         					*
 *									*
 * TLOOK: T_DISCONNECT | T_ORDREL					*
 * -------------------------------------------------------------------- */
int tx_rcv	(tx_con		*pv_con,
		 tx_datain	*pv_data)
{
   int	v_nbytes;

   while ((v_nbytes = T_rcv (pv_con->fd, pv_data->dmem, t_tidusize,
			     &pv_data->dflag)) == -1 &&
	  t_errno == TSYSERR && errno == EINTR);

   if (v_nbytes != -1) {
      if ((pv_data->dflag & T_EXPEDITED) != 0) {
	 /*
	  * we do not expect expedited data
	  */
	 tx_snddis (pv_con, NULL);
	 pv_con->reason = T_RLPROTERR;
	 return (-1);
      }
      else {
	 pv_data->dcnt = v_nbytes;
	 return (0);
      }
   }

   if (t_errno == TLOOK) {
      switch (tx_look (pv_con->fd)) {
	 case T_DISCONNECT:
	    tx_rcvdis (pv_con);
	    return (-1);

	 case T_ORDREL:
	    tx_rcvrel (pv_con);
	    return (-1);

	 default:
	    t_trc (T_STRING, 0, "Default/t_look");
	    pv_con->reason = T_RCCPEND;
      }
   }
   else
      pv_con->reason = T_RCCPEND;

   tx_snddis (pv_con, NULL);

   return (-1);
}

/* -------------------------------------------------------------------- *
 * tx_rcvconnect                       					*
 *									*
 * TLOOK: T_DISCONNECT							*
 * -------------------------------------------------------------------- */
int tx_rcvconnect	(tx_con	*pv_con)
{
   struct t_call	v_call;
   int			v_rc;

   /*
    * We save the user data for the following t_concf() but ignore options
    * and address
    */
   NB_FIL (v_call.addr, sv_addr);
   NB_FIL (v_call.opt, sv_opt);

   if ((v_call.udata.buf = malloc(ML_UDATA)) == NULL) {
      pv_con->reason = T_RLCONGEST;
      return (-1);
   }
   v_call.udata.maxlen = ML_UDATA;
   v_call.udata.len = 0;

   while ((v_rc = T_rcvconnect (pv_con->fd, &v_call)) == -1 &&
	  t_errno == TSYSERR && errno == EINTR);

   NB_CLR (v_call.addr);
   NB_CLR (v_call.opt);

   if (v_rc != -1 || t_errno == TBUFOVFLW) {
      /*
       * connection now fully established.
       * If TP returns TBUFOVFLW, it means that the buffer for the
       * address, options, or user data is not large enough.
       * The message is ignored and the error,
       * i.e. that the user data buffer is too small, is hidden.
       */
      pv_con->udata = v_call.udata;
      return (0);
   }

   free (v_call.udata.buf);

   if (t_errno == TLOOK && tx_look (pv_con->fd) == T_DISCONNECT) {
      tx_rcvdis (pv_con);
      return (-1);
   }

   tx_snddis (pv_con, NULL);
   pv_con->reason = T_RCCPEND;
   return (-1);
}

/* -------------------------------------------------------------------- *
 * tx_rcvdis                         					*
 *									*
 * _t_error will not be set, nobody evaluates it			*
 * -------------------------------------------------------------------- */
int tx_rcvdis	(tx_con	*pv_con)
{
   int			v_rc;
   struct t_discon	v_discon;

   if (pv_con->udata.buf != NULL)
      v_discon.udata = pv_con->udata;
   else {
      memset (&v_discon, 0, sizeof(v_discon));
      if ((v_discon.udata.buf = malloc(ML_UDATA)) == NULL) {
	 /*
	  * when there is no chance to get the userdata,
	  * continue and try to get the reason
	  */
         ;
      }
      else
         v_discon.udata.maxlen = ML_UDATA;
   }

   while ((v_rc = T_rcvdis (pv_con->fd, &v_discon)) == -1 &&
	  t_errno == TSYSERR && errno == EINTR);

   if (v_rc != -1) {
      /*
       * we get rid of this connection
       */
      pv_con->udata  = v_discon.udata;
      pv_con->reason = v_discon.reason;
      return (0);
   }

   /* frees also pv_con->udata, when it was there */
   if (v_discon.udata.buf != NULL)
      free (v_discon.udata.buf);
   memset (&pv_con->udata, 0, sizeof(pv_con->udata));

   if (t_errno == TBUFOVFLW) {
      /*
       * sorry, the connection is gone, but our userdata-buffer was
       * to small
       */
      pv_con->reason = v_discon.reason;
      return (0);
   }

   /*
    * at this point, we could use t_snddis but this may cause an
    * endless loop. The connection will be anyway closed by the
    * user with a t_disin().
    */
   pv_con->reason = T_RCCPEND;
   return (-1);
}

/* -------------------------------------------------------------------- *
 * t_rcvdis for an pending passive connection establishment		*
 * -------------------------------------------------------------------- */
int tx_rcvdisl	(tx_app	*pv_app)
{
   int			v_rc;
   tx_liststr	      **v_listap, *v_listp;
   tx_con	       *v_con;
   struct t_discon	v_discon;

   memset (&v_discon, 0, sizeof(v_discon));
   if ((v_discon.udata.buf = malloc(ML_UDATA)) == NULL) {
      /*
       * when there is no chance to get the userdata,
       * continue and try to get the reason
       */
      ;
   }
   else
      v_discon.udata.maxlen = ML_UDATA;

   while ((v_rc = T_rcvdis (pv_app->pconp->fd, &v_discon)) == -1 &&
	  t_errno == TSYSERR && errno == EINTR);

   if (v_rc == -1) {
      if (t_errno == TBUFOVFLW)
	 v_discon.udata.len = 0;
      else {
	 if (v_discon.udata.buf != NULL)
	    free (v_discon.udata.buf);
	 return (-1);
      }
   }

   /*
    * now we have to look for the matching list element
    */
   v_listap = &pv_app->pconp->listq;
   while ((v_listp = *v_listap) != NULL) {
      if (v_listp->call.sequence == v_discon.sequence) {
	 *v_listap = v_listp->nxt;
	 free (v_listp);
	 free (v_discon.udata.buf);
	 return (0);
      }
      v_listap = &v_listp->nxt;
   }

   /*
    * look into already announced connection establishments
    */
   if ((v_con = pv_app->conp) != NULL) {
      do {
	 if (v_con->listq != NULL &&
	     v_con->listq->call.sequence == v_discon.sequence) {
	    free (v_con->listq);
	    v_con->listq = NULL;
	    v_con->udata = v_discon.udata;
	    v_con->reason = v_discon.reason;
	    return (0);
	 }
	 v_con = v_con->nxt;
      }
      while (v_con != pv_app->conp);
   }

   /*
    * We got a rcvdis for a connection, from which we are not aware of.
    * Could come from failed attempt to get rid of it.
    */
   t_trc(T_STRING, 0, "tx_rcvdisl for an unaware connection, sequence#");
   if (v_discon.udata.buf != NULL)
      free (v_discon.udata.buf);
   return (0);
}

/* -------------------------------------------------------------------- *
 * tx_rcvrel                         					*
 *									*
 * TLOOK: T_DISCONNECT							*
 * _t_error will not be set, nobody evaluates it			*
 * -------------------------------------------------------------------- */
int tx_rcvrel	(tx_con	*pv_con)
{
   int			v_rc;

   /*
    * CMX on top of XTI do not use this feature, but some TPs deliver
    * T_ORDREL instead of T_DISCONNECT, so we handle it.
    */

   while ((v_rc = T_rcvrel (pv_con->fd)) == -1 &&
	  t_errno == TSYSERR && errno == EINTR);

   if (v_rc != -1) {
      v_rc = tx_getstate (pv_con->fd);
      if (v_rc == T_DATAXFER || v_rc == T_INREL)
	 while ((v_rc = T_sndrel (pv_con->fd)) == -1 &&
		t_errno == TSYSERR && errno == EINTR);
   }

   /*
    * the user will get a T_DISIN without any user data
    */
   if (pv_con->udata.buf != NULL)
      free (pv_con->udata.buf);
   memset (&pv_con->udata, 0, sizeof(pv_con->udata));

   if (v_rc != -1) {
      /*
       * we get rid of this connection
       */
      pv_con->reason = T_USER;
      return (0);
   }

   tx_snddis (pv_con, NULL);
   pv_con->reason = T_RCCPEND;
   return (-1);
}

/* -------------------------------------------------------------------- *
 * tx_snd                         					*
 *									*
 * TLOOK: T_DISCONNECT, T_ORDREL					*
 * -------------------------------------------------------------------- */
int tx_snd	(tx_con		*pv_con,
		 char		*pv_buf,
		 unsigned	 pv_nbytes,
		 int		 pv_flags)
{
   int		v_rc;
   unsigned	v_nbytes;

   v_nbytes = 0;

   for (;;) {
      while ((v_rc = T_snd (pv_con->fd, pv_buf, pv_nbytes, pv_flags)) == -1 &&
	     t_errno == TSYSERR && errno == EINTR);

      if (v_rc == 0) {
	 /*
	  * This may cause endless loops!
	  * in socket environment this means the connection died!
	  */
	 _t_error = T_COLLISION;
	 pv_con->reason = T_RCCPEND;
	 return (-1);
      }

      if (v_rc > 0) {
	 v_nbytes  += v_rc;
	 pv_nbytes -= v_rc;
	 if (pv_nbytes == 0)
	    return (v_nbytes);
	 pv_buf += v_rc;
      }
      else {
	 if (t_errno == TFLOW)
	    return  (v_nbytes);

	 if (t_errno == TLOOK) {
	    switch (tx_look (pv_con->fd)) {
	       case T_DISCONNECT:
		  tx_rcvdis (pv_con);
	          _t_error = T_COLLISION;
		  return (-1);

	       case T_ORDREL:
		  tx_rcvrel (pv_con);
	          _t_error = T_COLLISION;
		  return (-1);

	       default:
		  t_trc (T_STRING, 0, "Default/t_look");
		  pv_con->reason = T_RCCPEND;
	    }
	 }
	 else
	    pv_con->reason = T_RLCONNLOST;

	 tx_snddis (pv_con, NULL);
	 _t_error = T_COLLISION;
	 return (-1);
      }
   }
}

/* -------------------------------------------------------------------- *
 * tx_snddis                         					*
 *									*
 * TLOOK: T_DISCONNECT							*
 * -------------------------------------------------------------------- */
int tx_snddis	(tx_con		*pv_con,
		 struct t_call	*pv_call)
{
   int	v_rc;
#ifdef SCO
   int	v_fcntl;
#endif

   v_rc = tx_getstate (pv_con->fd);

   if (v_rc == T_DATAXFER || v_rc == T_INCON || v_rc == T_OUTCON ||
       v_rc == T_OUTREL || v_rc == T_INREL) {
#ifdef SCO
      v_fcntl = fcntl(pv_con->fd, F_GETFL, 0);
      fcntl (pv_con->fd, F_SETFL, v_fcntl & ~O_NONBLOCK);
#endif

      while ((v_rc = T_snddis (pv_con->fd, pv_call)) == -1 &&
	     t_errno == TSYSERR && errno == EINTR);
#ifdef SCO
      fcntl (pv_con->fd, F_SETFL, v_fcntl & ~O_NONBLOCK);
#endif
   }

   if (v_rc != -1)
      return (0);

   if (t_errno == TLOOK && tx_look (pv_con->fd) == T_DISCONNECT) {
      tx_rcvdis (pv_con);
      return (-1);
   }

   _t_error = tx_maperror (t_errno, errno);
   return (-1);
}

/* -------------------------------------------------------------------- *
 * tx_snddis for an pending passive connection establishment		*
 *									*
 * TLOOK: T_DISCONNECT							*
 * -------------------------------------------------------------------- */
int tx_snddisl	(tx_app		*pv_app,
		 struct t_call	*pv_call)
{
   int	v_rc;
#ifdef SCO
   int	v_fcntl;
#endif

   v_rc = tx_getstate (pv_app->pconp->fd);
   if (v_rc == T_INCON || v_rc == T_INREL) {
#ifdef SCO
      v_fcntl = fcntl(pv_app->pconp->fd, F_GETFL, 0);
      fcntl (pv_app->pconp->fd, F_SETFL, v_fcntl & ~O_NONBLOCK);
#endif
      while ((v_rc = T_snddis (pv_app->pconp->fd, pv_call)) == -1 &&
	     t_errno == TSYSERR && errno == EINTR);
#ifdef SCO
      fcntl (pv_app->pconp->fd, F_SETFL, v_fcntl & ~O_NONBLOCK);
#endif
   }

   if (v_rc != -1)
      return (0);

   if (t_errno == TLOOK && tx_look (pv_app->pconp->fd) == T_DISCONNECT) {
      tx_rcvdisl (pv_app);
      return (-1);
   }

   /*
    * some TP returns in case of t_listen/TBUFOFLOW no sequence number,
    * this could give an new error here
    */
   _t_error = tx_maperror (t_errno, errno);
   return (-1);
}

/* -------------------------------------------------------------------- *
 * tx_sync                          					*
 * -------------------------------------------------------------------- */
int tx_sync	(int	pv_fd)
{
   int	v_state;

   /* 
    * on SCO the state returned by sync after fork is not valid.
    */
   while ((v_state = T_sync (pv_fd)) == -1 &&
	  t_errno == TSYSERR && errno == EINTR);

   if (v_state == -1)
      _t_error = tx_maperror (t_errno, errno);

   return (v_state);
}

/* -------------------------------------------------------------------- *
 * tx_unbind                         					*
 * -------------------------------------------------------------------- */
int tx_unbind	(tx_con	*pv_con)
{
   int	v_rc;

   v_rc = tx_getstate (pv_con->fd);

   /*
    * in case of other states, t_close will handle it!
    */
   if (v_rc == T_IDLE)
      while ((v_rc = T_unbind (pv_con->fd)) == -1 &&
	     t_errno == TSYSERR && errno == EINTR);

   if (v_rc != -1)
      return (0);

   /*
    * we ignore TLOOK/T_LISTEN as unbind is followed by t_close
    */
   _t_error = tx_maperror (t_errno, errno);
   return (-1);
}

/* -------------------------------------------------------------------- *
 * tx_maperror                         					*
 * -------------------------------------------------------------------- */
int tx_maperror	(int	pv_terrno,
		 int	pv_errno)
{
   static struct tx_errmap {
      int	terrno;
      int	terror;
      int	abort;
   } sv_errmap [] =
       {{TBADADDR,	T_WPARAMETER,  0}, /* incorrect addr format */
	{TBADOPT,	T_EIO,	       0}, /* incorrect option format */
	{TACCES,	EACCES,	       0}, /* incorrect permissions */
	{TBADF,		T_EIO,	       1}, /* illegal transport fd */
	{TNOADDR,	T_EIO,	       0}, /* couldn't allocate addr */
	{TOUTSTATE,	T_EIO,	       1}, /* out of state */
	{TBADSEQ,	T_EIO,	       1}, /* bad call sequence number */
	{TSYSERR,	T_EIO,	       0}, /* system error */
	{TLOOK,		T_EIO,	       0}, /* event requires attention */
	{TBADDATA,	T_EIO,	       0}, /* illegal amount of data */
	{TBUFOVFLW,	T_EIO,	       0}, /* buffer not large enough */
	{TFLOW,		T_EIO,	       0}, /* flow control */
	{TNODATA,	T_EIO,	       0}, /* no data */
	{TNODIS,	T_EIO,	       0}, /* discon_ind not found on queue */
	{TNOUDERR,	T_EIO,	       0}, /* unidata error not found */
	{TBADFLAG,	T_EIO,	       1}, /* bad flags */
	{TNOREL,	T_EIO,	       1}, /* no ord rel found on queue */
	{TNOTSUPPORT,	T_EIO,	       1}, /* primitive not supported */
	{TSTATECHNG,	T_COLLISION,   0}, /* state is in process of changing */
	{TNOSTRUCTYPE,	T_EIO,	       1}, /* unsupported struct-type requested */
	{TBADNAME,	T_EIO,	       0}, /* invalid transport provider name */
	{TBADQLEN,	T_EIO,	       1}, /* qlen is zero */
	{TADDRBUSY,	T_EIO,	       0}, /* address in use */
	{TINDOUT,	T_EIO,	       1}, /* outstanding connection indications */
	{TPROVMISMATCH,	T_EIO,	       1}, /* transport provider mismatch */
	{TRESQLEN,	T_EIO,	       1}, /* resfd specified to accept w/qlen>0 */
	{TRESADDR,	T_EIO,	       0}, /* resfd not bound to same addr as fd */
	{TQFULL,	T_EIO,	       0}, /* incoming connection queue full */
	{TPROTO,	T_EIO,	       0}};/* XTI protocol error */
   static int	sv_errmcnt = sizeof (sv_errmap) / sizeof (struct tx_errmap);

   int	v_ind;

   if (pv_terrno == TSYSERR)
      if (pv_errno == EINTR)
	 return (T_EINTR);
      else
	 return (pv_errno);

   for (v_ind = 0; v_ind < sv_errmcnt; v_ind++)
      if (pv_terrno == sv_errmap[v_ind].terrno)
	 if (sv_errmap[v_ind].abort != 0)
	    assert (0);
	 else
	    return (sv_errmap[v_ind].terror);

   t_trc (T_STRING, 0, "unknown t_errno");
   return (T_EIO);
}

