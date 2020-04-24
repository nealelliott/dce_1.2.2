/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxxti.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:35  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:57  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:49:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:46  root]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:42  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:22:54  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:48:27  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:46:58  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:48  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:56  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:44  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:04  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxxti.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:35 $";
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
static char	sccsid[] = "@(#)cmxxti.c	311.22 93/08/24 CMX (NC1)";
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
/* SYSPID_MAX	highest pid occupied by a system process (init)		    */
/*		usually taken from limits.h				    */
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
#include <string.h>
#include <memory.h>

#include <fcntl.h>
#if defined(_AIX)
#include <sys/select.h>
#endif
#include <netinet/in.h>
#undef IP_OPTIONS
#undef IP_TOS
#undef IP_TTL
#include <assert.h>

#include "cmx.h"
#define	T_CMX_MORE	1
#if T_CMX_MORE != T_MORE
ERROR, definition changed
#endif
#undef	T_MORE
#define	T_CMX_CHECK	1
#if T_CMX_CHECK != T_CHECK
ERROR, definition changed
#endif
#undef	T_CHECK

#undef IP_OPTIONS		/* just to avoid compiler warnings	*/
#include "xti.h"
#define	T_XTI_MORE	0x001
#if T_XTI_MORE != T_MORE
ERROR, definition changed
#endif
#undef	T_MORE
#define	T_XTI_CHECK	0x008
#if T_XTI_CHECK != T_CHECK
ERROR, definition changed
#endif
#undef	T_CHECK

#include <ndsif.h>
#include <sys/signal.h>
#include "cmxextern.h"
#include "cxioctl.h"
#include "cmxaddr.h"
#include "cmxtrace.h"

#define CMXXTI_C
#include "cmxxti.h"

#ifdef	ALLOC_TEST
#include "alloc.c"
#include "alloctrc.h"
#endif	/* ALLOC_TEST */

/*
   ??? Marker
   dirx: no sptypes
   Signalization not configured
*/

/************************************************************************/
/* local definitions							*/
/************************************************************************/

/*
 * Legal pid for t_redirect() are > SYSPID_MAX.
 * If SYSPID_MAX is not defined, assume at least pids 1, 2, 3 are system's.
 */
#ifndef SYSPID_MAX
#define SYSPID_MAX	3
#endif

#define	OR_RMFD	0x0001		/* remove fd from t_fdset etc		*/
#define	OR_CLFD	0x0002		/* close fd				*/
#define	OR_DISC	0x0004		/* send t_snddis			*/
#define	OR_CONN	0x0100		/* normal connection			*/
#define	OR_PCON	0x0200		/* for passiv connection establishment	*/
#define	NOFD	-1		/* same as an t_open-error		*/

/************************************************************************/
/* local data								*/
/************************************************************************/
static tx_proc	tx_p = { 0 };

/************************************************************************/
/* Prototypes								*/
/************************************************************************/
static tx_app	*p_app_cre	(void);
static void	 p_app_free	(tx_app *, ui32);
static tx_app	*p_app_fbname	(union cx_unisel *);
static tx_app	*p_app_fbind	(ui32);
static tx_app	*p_app_fbtmn	(ui32);
static tx_app	*p_app_fbloc	(char *, int);
static tx_con	*p_con_cre	(tx_app *, ui32);
static void	 p_con_free	(tx_app *, tx_con *, ui32);
static tx_con	*p_con_fbind	(tx_con *, ui32);
static tx_tmn	*p_tmn_cre 	(void);
static void 	 p_tmn_free	(ui32	pv_ind);
static tx_tmn	*p_tmn_fbname	(struct t_myname *);
static tx_tmn	*p_tmn_fbind	(ui32);
static int	 p_pcon_cre	(tx_app *);

int t_attach	(struct t_myname *name, t_opta *opt);
int t_concf	(int *tref, t_opt1 *opt);
int t_conin	(int *, union t_address *, union t_address *, t_opt1 *);
int t_conrq	(int *, union t_address *, union t_address *, t_opt13 *);
int t_conrs	(int *tref, t_opt13 *opt);
int t_datago	(int *tref);
int t_datain	(int *tref, char *datap, int *datal, int *chain);
int t_datarq	(int *tref, char *datap, int *datal, int *chain);
int t_datastop	(int *tref);
int t_detach	(struct t_myname *pv_name);
int t_disin	(int *tref, int *reason, t_opt2 *opt);
int t_disrq	(int *tref, t_opt2 *opt);
int t_event	(int *tref, int cmode, t_opte *opt);
int t_info	(int *tref, t_opti *opt);
int t_redin	(int *tref, int *pid, t_opt23 *opt);
int t_redrq	(int *tref, int *pid, t_opt12 *opt);
int t_vdatain	(int *tref, struct t_data *vdata, int *vcnt, int *chain);
int t_vdatarq	(int *tref, struct t_data *vdata, int *vcnt, int *chain);
int t_xdatgo	(int *tref);
int t_xdatin	(int *tref, char *datap, int *datal);
int t_xdatrq	(int *tref, char *datap, int *datal);
int t_xdatstop	(int *tref);


typedef struct {
   int	sptypes;
   int	ok;
   int	nf;
} tx_resstr;

/************************************************************************/
/* p_tns_attach								*/
/************************************************************************/
static int p_tns_attach	(struct t_myname	*pv_name,
			 t_opta			*pv_opt,
			 int			 pv_apmode,
			 tx_resstr		*pv_res)
{
   tx_app		*v_app = NULL;
   tx_tmn		*v_tmn = NULL;
   union cx_unisel	*v_cxselp, v_cxsel;
   int			 v_cxnum;
   int			 v_rc;
   int			 v_type;

   /*
    * struct t_myname hides a lot of its information from the user.
    * We use union cx_unisel (untaged union) to get information out of it.
    *
    * Loop over a array of cx_unisel's. Inside there are bit-coded
    * CCP-types, therefore we have also to loop through the CCP-types.
    */

   if ((v_cxnum = (pv_name->t_mnlng - CXN_PRESIZE - CXN_FIXSIZE)
		  / sizeof (union cx_unisel)) < 1 ||
       v_cxnum > CXN_SELNUM) {
      _t_error = T_WPARAMETER;
      return (-1);
   }

   v_cxselp = ((struct cx_myname *)pv_name->t_mn)->cxn_sel;

   while (v_cxnum--) {
      for (v_type = 0x0001; v_type != 0; v_type <<= 1) {
	 if ((v_type & v_cxselp->cxs.cx_type) != 0) {

	    v_cxsel = *v_cxselp;
	    v_cxsel.cxs.cx_type = v_type;

	    /*
	     * Check reuse of the same CMX-application
	     */
	    if (p_app_fbname (&v_cxsel) != NULLAP) {
	       _t_error = T_WAPPLICATION;
	       goto errout;
	    }

	    if (v_app == NULL) {
	       /*
		* Create application and link application structure in chain.
		* Also checks the maximum number of applications (in XTI/TLI)
		*/
	       if ((v_app = p_app_cre()) == NULLAP)
		  goto errout;
	    }

	    /*
	     * Translate tsel to XTI/TLI address
	     * returns 0, when the address is not suported (cmxticfg)
	     */
	    if ((v_rc = tx_amsel (&v_cxsel, &v_app->addr, &v_app->tp,
		0, 0)) == -1)
	       goto errout;

	    if (v_rc == 0)
	       continue;		/* NOT SUPPORTED ADDRESS TYPE	*/

            /*
             * get TP-specific flags and reason mapping table
             * this call is just after tx_amsel valid!
             */
	    tx_tpinfo (&v_app->tpflag, &v_app->maprea);

	    /*
	     * fill the rest of the application
	     */
	    v_app->atyp = ATYP_TNS;
	    memcpy (&v_app->tsel, &v_cxsel, sizeof (v_app->tsel));

	    v_app->apmode = pv_apmode;

	    if (pv_opt != NULL) {
	       v_app->conlim = (pv_opt->opta1.t_conlim == T_NOLIMIT) ?
			       TX_CON_MAX : pv_opt->opta1.t_conlim;
	       v_app->uattid = (pv_opt->opta2.t_optnr != T_OPTA1) ?
			       pv_opt->opta2.t_uattid : 0;
	    }
	    else
	       v_app->conlim = 1;	/* uattid already 0 */

	    /*
	     * Set up TSAP for incoming connections
	     */
	    if ((pv_apmode & T_PASSIVE) != 0) {
	       if ((v_rc = p_pcon_cre(v_app)) != 0)
		  pv_res->nf++;
	       else
		  pv_res->ok++;
	    }

	    if ((pv_apmode & T_ACTIVE) != 0)
	       pv_res->ok++;

	    if (v_tmn == NULL) {
	       if ((v_tmn = p_tmn_cre ()) == NULL)
		  goto errout;
	       v_tmn->myname = *pv_name;
	    }
	    v_app->tmn = v_tmn->ind;
	    v_tmn->cnt++;
	    pv_res->sptypes |= v_type;
	    v_app = NULL;
	 }
      }
      v_cxselp++;
   }

   /*
    * usually, when the last address didn't taste well for us
    */
   if (v_app != NULLAP)
      p_app_free (v_app, OR_RMFD);

   return (0);

errout:
   if (v_app != NULLAP)
      p_app_free (v_app, OR_RMFD);
   return (-1);
}

/************************************************************************/
/* p_nds_attach								*/
/************************************************************************/
static int p_nds_attach	(struct t_myname	*pv_name,
			 t_opta			*pv_opt,
			 int			 pv_apmode,
			 tx_resstr		*pv_res)
{
   tx_app		*v_app = NULL;
   tx_tmn		*v_tmn = NULL;
   int			 v_rc;
   int			 v_cons;
   Nds_xtiselinfo	 v_xtisel;
   Nds_osixtiinfo	 v_osixti;
   char			 v_xtmp [1024];

   /*
    * Sorry for that coding!
    */
   v_xtisel.nds_osiaddr = (struct t_adrdesc *) pv_name;
   if (NDS_map_addrinfo (NDS_XTISEL, (Nds_mapinfo *) &v_xtisel) < 0) {
      _t_error = v_xtisel.nds_errno ;
      return (-1);
   }

   for (v_cons = 1; v_cons <= v_xtisel.nds_noxtiprov; v_cons++) {
      /*
       * Create application and link application structure in chain.
       * Also checks the maximum number of applications (in XTI/TLI)
       */
      if ((v_app = p_app_cre()) == NULLAP)
	 goto errout;

      /*
       * Translate to XTI/TLI-Address
       */
      v_osixti.nds_osi1addr = (struct t_adrdesc *) pv_name; /* t_myname */
      v_osixti.nds_xti1addr.maxlen = sizeof (v_xtmp);
      v_osixti.nds_xti1addr.buf = v_xtmp;
      v_osixti.nds_osi2addr = (struct t_adrdesc *) NULL;
      v_osixti.nds_xtiprovnm = (char *) NULL;
      v_osixti.nds_xtiprovidx = v_cons;

      if (NDS_map_addrinfo (NDS_OSITOXTI, (Nds_mapinfo *) &v_osixti) < 0) {
	 _t_error = v_osixti.nds_errno;
	 goto errout;
      }

      /*
       * Check reuse of the same CMX-application
       */
      if (p_app_fbloc (v_osixti.nds_xti1addr.buf,
                       v_osixti.nds_xti1addr.len) != NULL) {
         _t_error = T_WAPPLICATION;
         goto errout;
      }

      if ((v_app->tp = malloc (strlen (v_osixti.nds_xtiprovnm) + 1)) == NULL) {
	 _t_error = T_ENOMEM;
	 goto errout;
      }
      strcpy (v_app->tp, v_osixti.nds_xtiprovnm);

      if ((v_app->addr.buf = malloc (v_osixti.nds_xti1addr.len)) == NULL) {
	 _t_error = T_ENOMEM;
	 goto errout;
      }
      v_app->addr.len = v_osixti.nds_xti1addr.len;
      memcpy (v_app->addr.buf, v_osixti.nds_xti1addr.buf, v_app->addr.len);

      /*
       * fill the rest of the application
       */
      v_app->atyp = ATYP_DIRX;
      v_app->apmode = pv_apmode;

      if (pv_opt != NULL) {
	 v_app->conlim = (pv_opt->opta1.t_conlim == T_NOLIMIT) ?
			 TX_CON_MAX : pv_opt->opta1.t_conlim;
	 v_app->uattid = (pv_opt->opta2.t_optnr != T_OPTA1) ?
			 pv_opt->opta2.t_uattid : 0;
      }
      else
	 v_app->conlim = 1;		/* uattid already 0 */

      /*
       * Set up TSAP for incoming connections
       */
      if ((pv_apmode & T_PASSIVE) != 0) {
	 if ((v_rc = p_pcon_cre(v_app)) != 0)
	    pv_res->nf++;
	 else
	    pv_res->ok++;
      }

      if ((pv_apmode & T_ACTIVE) != 0)
	 pv_res->ok++;

      if (v_tmn == NULL) {
	 if ((v_tmn = p_tmn_cre ()) == NULL)
	    goto errout;
	 v_tmn->myname = *pv_name;
      }
      v_app->tmn = v_tmn->ind;
      v_tmn->cnt++;
      v_app = NULL;
      pv_res->sptypes = 0xffff;		/* at least one valid address	*/
   }

   return (0);

errout:
   if (v_app != NULLAP)
      p_app_free (v_app, OR_RMFD);
   return (-1);
}

/************************************************************************/
/* t_attach								*/
/************************************************************************/
int t_attach	(struct t_myname	*name,
		 t_opta			*opt)
{
   extern void t_addcclst(struct t_cclst *);
   static struct t_cclst xticclist = {1, {CX_XTIID}};

   tx_app		*v_app = NULL;
   tx_tmn		*v_tmn = NULL;
   int			 v_rc;
   int		 	 v_apmode = (T_ACTIVE|T_PASSIVE|T_REDIRECT);
   tx_resstr		 v_res;

   memset (&v_res, 0, sizeof(v_res));

   /*
    * if it's the first t_attach after fork()
    * cleanup all structures.
    */
   if (tx_p.mypid != t_pid) {
      if (tx_p.mypid != 0) {
	 /*
	  * This is a child which did not exec().
	  * Release all data structures but don't touch
	  * fd's for established connections
	  */
	 while ((v_app = tx_p.apptr) != NULLAP)
	    p_app_free (v_app, OR_RMFD);
      }

      tx_redclose();
      memset(&tx_p, 0, sizeof(tx_p));
      tx_p.mypid = t_pid;
   }

   /*
    * Check options, apmode and conlim
    */
   /* ignore T_REDIRECT flag because no support will be given */
   /* on foreign (non SNI) platforms */
   opt->opta1.t_apmode &= ~T_REDIRECT ;

   if (opt != NULL) {
      if ((opt->opta1.t_optnr != T_OPTA1
	   && opt->opta2.t_optnr != T_OPTA2
	   && opt->opta4.t_optnr != T_OPTA4
	   && opt->opta5.t_optnr != T_OPTA5
	   && opt->opta6.t_optnr != T_OPTA6)
	  || !(opt->opta1.t_apmode & (T_ACTIVE|T_PASSIVE|T_REDIRECT))
	  || (opt->opta1.t_apmode & ~(T_ACTIVE|T_PASSIVE|T_REDIRECT))
	  || opt->opta1.t_conlim < T_NOLIMIT
	  || opt->opta1.t_conlim == 0
	  || opt->opta1.t_conlim > TX_CON_MAX) {
	 _t_error = T_WPARAMETER;
	 return (T_ERROR);
      }
      v_apmode = opt->opta1.t_apmode;
   }

   /*
    * Check and switch address
    */
   if (p_tmn_fbname (name) != NULL) {
      _t_error = T_WAPPLICATION;
      return (T_ERROR);
   }
   if (name->t_mnmode == T_MNMODE) {
      if (p_tns_attach (name, opt, v_apmode, &v_res) == -1)
	 goto errout;
   }
   else
   {
      if ((((T_adrdesc *) name)->t_infotype == T_TSAPINFO) ||
	  (((T_adrdesc *) name)->t_infotype == T_NSAPINFO)) {
	 if (p_nds_attach (name, opt, v_apmode, &v_res) == -1)
	    goto errout;
      }
      else
      {
	 _t_error = T_WPARAMETER;
	 return (T_ERROR);
      }
   }

   /*
    * further processing only useful if we could logon somewhere
    */
   if (v_res.sptypes != 0) {
      if (opt != NULL &&
	  ((opt->opta2.t_optnr == T_OPTA2) ||
	   (opt->opta5.t_optnr == T_OPTA5) ||
	   (opt->opta6.t_optnr == T_OPTA6))) {
	 if (v_res.sptypes != 0xffff)
	    opt->opta2.t_sptypes |= v_res.sptypes;
         if ((v_tmn = p_tmn_fbname (name)) == NULL)
            goto errout;
	 opt->opta2.t_attid = v_tmn->ind;

	 if (opt->opta2.t_optnr != T_OPTA6)
	    opt->opta2.t_ccbits |= 0;	/* no CC's		*/
	 else
	    t_addcclst((struct t_cclst *)&xticclist);
      }

      /*
       * Create a IPC-listening TSAP.
       */
      if ((v_apmode & T_REDIRECT) != 0) {
	 if ((v_rc = tx_redopen ()) != 0)
	    v_res.nf++;
	 else
	    v_res.ok++;
      }

      /*
       * Here we go to accept some totaly useless applications
       * just T_REDIRECT was set and got T_NOTFIRST or
       * just T_PASSIVE was set and got T_NOTFIRST or
       * both with the same result. We ignore it!
       *
       * Now we have got a real new application
       */
      if (v_res.ok != 0)
	 return (T_OK);
      if (v_res.nf != 0)
	 return (T_NOTFIRST);
   } /* if (v_sptypes != 0) */

errout:
   if ((v_tmn = p_tmn_fbname (name)) != NULL) {
      while ((v_app = p_app_fbtmn (v_tmn->ind)) != NULL)
	 p_app_free (v_app, OR_RMFD);
   }

   return (T_ERROR);
}


/************************************************************************/
/* t_concf								*/
/************************************************************************/
int t_concf	(int	*tref,
		 t_opt1	*opt)
{
   tx_app	*v_app;
   tx_con	*v_conp;

   if ((v_app = p_app_fbind (GET_APP_IND(tref))) == NULLAP
       || (v_conp = p_con_fbind (v_app->conp, GET_CON_IND(tref))) == NULLCP
       || v_conp->state != SX_OUTC_I
       || v_conp->elast != T_CONCF) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   if (opt != NULL &&
       (opt->optc1.t_optnr != T_OPTC1
	|| opt->optc1.t_udatal < 0
	|| (opt->optc1.t_udatal > 0
	    && opt->optc1.t_udatal < v_conp->udata.len))) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   if (opt != NULL) {
      /* if class 0 or the TP should have no user data, the user get none */
      if (v_conp->info.connect <= 0)
         opt->optc1.t_udatal = 0;
      if (opt->optc1.t_udatal != 0
	  && (opt->optc1.t_udatal = v_conp->udata.len) != 0)
	 memcpy (opt->optc1.t_udatap, v_conp->udata.buf, v_conp->udata.len);
      opt->optc1.t_xdata = T_NO;
      opt->optc1.t_timeout = T_NO;
   }

   v_conp->event = v_conp->elast = T_NOEVENT;
   v_conp->state = SX_DATAXFER;
   free (v_conp->udata.buf);
   memset (&v_conp->udata, 0, sizeof(v_conp->udata));

   FD_SET(v_conp->fd, &t_fdset);
   FD_SET(v_conp->fd, &t_xfds);
   FD_SET(v_conp->fd, &t_rfds);
   fcntl (v_conp->fd, F_SETFD, (fcntl (v_conp->fd, F_GETFD) & ~FD_CLOEXEC));

   return (T_OK);
}

/************************************************************************/
/* t_conin								*/
/************************************************************************/
int t_conin	(int			*tref,
		 union t_address	*toaddr,
		 union t_address 	*fromaddr,
		 t_opt1			*opt)
{
   tx_app		*v_app;
   tx_tmn		*v_tmn;
   tx_con		*v_conp;
   tx_liststr		*v_listp;
   Nds_osixtiinfo	 v_osixti;

   if ((v_app = p_app_fbind (GET_APP_IND(tref))) == NULLAP
       || (v_conp = p_con_fbind (v_app->conp, GET_CON_IND(tref))) == NULLCP
       || v_conp->state != SX_INCO_I
       || v_conp->elast != T_CONIN) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   /*
    * between T_CONIN and t_conin() there we may have got a T_DISCONNECT
    */
   if ((v_listp = v_conp->listq) == NULL) {
      v_conp->reason = T_USER;
      v_conp->state = SX_DISI_I;
      v_conp->event = T_DISIN;
      _t_error = T_COLLISION;
      return (T_ERROR);
   }

   if (opt != NULL &&
       (opt->optc1.t_optnr != T_OPTC1 ||
	opt->optc1.t_udatal < 0 ||
	(opt->optc1.t_udatal > 0 &&
	 opt->optc1.t_udatal < v_listp->call.udata.len))) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   v_conp->event = v_conp->elast = T_NOEVENT;
   v_conp->state = SX_INRS_C;

   /*
    * return fromaddr, toaddr
    */
   v_tmn = p_tmn_fbind (v_app->tmn);
   toaddr->tmyname = v_tmn->myname;

   if (v_app->atyp == ATYP_TNS) {
      tx_bui_paddr  (v_app->tsel.cxs.cx_type,
		     &fromaddr->tpartaddr, &v_listp->call.addr);
   }
   else
   {
      v_osixti.nds_osi1addr = (struct t_adrdesc *) fromaddr;
      v_osixti.nds_osi2addr = (struct t_adrdesc *) NULL;
      v_osixti.nds_xti1addr.len = v_listp->call.addr.len;
      v_osixti.nds_xti1addr.buf = v_listp->call.addr.buf;
      v_osixti.nds_xtiprovnm = v_app->tp;
      if (NDS_map_addrinfo (NDS_XTITOOSI, (Nds_mapinfo *) &v_osixti) < 0) {
	 /* _t_error = v_osixti.nds_errno; */
	 _t_error = T_NOCCP;
	 return (T_ERROR);
      }
   }

   if (opt != NULL) {
      /* if class 0 or the TP should have no user data, the user get none */
      if (v_conp->info.connect <= 0)
         opt->optc1.t_udatal = 0;
      if (opt->optc1.t_udatal != 0
	  && (opt->optc1.t_udatal = v_listp->call.udata.len) != 0)
	 memcpy (opt->optc1.t_udatap, v_listp->call.udata.buf,
		 v_listp->call.udata.len);
      opt->optc1.t_xdata   = T_NO;
      opt->optc1.t_timeout = T_NO;
   }

   return (T_OK);
}



static tx_app *p_tns_conrq	(union t_address *pv_toaddr,
				 union t_address *pv_fromaddr,
				 struct t_call	 *pv_sndcall)
{
   tx_app		*v_app;
   union cx_unisel	 v_cxsel, *v_cxselp;
   int			 v_cxnum;

   /*
    * determine the network type from the transport addresse (toaddr)
    * which then searches in local name (fromaddr). Establish cx_unisel,
    * which can be used to search the application block!
    *
    * cmx2k firmly assumes that the CCP type is correctly filled out
    * in the transport address, so I also assume this
    */
   v_cxsel.cxs.cx_type = ((union cx_uniaddr *)pv_toaddr->tpartaddr.t_pa)
			 ->cxa.cx_type;
   v_cxsel.cxs.cxs_lng = 0;		/* as a empty-indicator	*/

   if (pv_fromaddr->tmyname.t_mnmode != T_MNMODE ||
       (v_cxnum = (pv_fromaddr->tmyname.t_mnlng - CXN_PRESIZE - CXN_FIXSIZE)
		  / sizeof (union cx_unisel)) < 1 ||
       v_cxnum > CXN_SELNUM) {
      _t_error = T_WPARAMETER;
      return (NULL);
   }

   v_cxselp = ((struct cx_myname *)pv_fromaddr->tmyname.t_mn)->cxn_sel;

   while (v_cxnum--) {
      if ((v_cxsel.cxs.cx_type & v_cxselp->cxs.cx_type) != 0) {
	 v_cxsel.cxs.cxs_lng = v_cxselp->cxs.cxs_lng;
	 memcpy (v_cxsel.cxs.cxs_rest, v_cxselp->cxs.cxs_rest, CXS_SIZE);
	 break;
      }
      v_cxselp++;
   }

   /*
    * Find CMX applikation by name
    */
   if (v_cxsel.cxs.cxs_lng == 0 ||
       (v_app = p_app_fbname (&v_cxsel)) == NULLAP) {
      _t_error = T_WAPPLICATION;
      return (NULL);
   }

   if ((v_app->apmode & T_ACTIVE) == 0) {
      _t_error = T_WSEQUENCE;
      return (NULL);
   }

   /*
    * Translate name to XTI/TLI address
    * (address buffer has to be freed afterwards)
    */
   if (tx_amaddr (v_cxsel.cxs.cx_type, &pv_sndcall->addr,
       &pv_toaddr->tpartaddr, 0) == -1)
      return (NULL);

   return (v_app);
}

static tx_app *p_nds_conrq	(union t_address *pv_toaddr,
				 union t_address *pv_fromaddr,
				 struct t_call	 *pv_sndcall)
{
   tx_app		*v_app;
   Nds_osixtiinfo	v_osixti;
   char			v_xtmp[1024];

   pv_sndcall->addr.maxlen = ML_ADDR;
   if ((pv_sndcall->addr.buf = malloc (pv_sndcall->addr.maxlen)) == NULL) {
      _t_error = T_ENOMEM;
      return (NULL);
   }

   v_osixti.nds_osi1addr = (struct t_adrdesc *) pv_fromaddr;
   v_osixti.nds_xti1addr.maxlen = sizeof(v_xtmp);
   v_osixti.nds_xti1addr.buf = v_xtmp;
   v_osixti.nds_osi2addr = (struct t_adrdesc *) pv_toaddr;
   v_osixti.nds_xti2addr.maxlen = pv_sndcall->addr.maxlen;
   v_osixti.nds_xti2addr.buf = pv_sndcall->addr.buf;
   v_osixti.nds_mprule = NDS_HPRIO ;

   if (NDS_map_addrinfo (NDS_OSITOXTI, (Nds_mapinfo *) &v_osixti) < 0) {
      _t_error = v_osixti.nds_errno ;
      return (NULL);
   }
   pv_sndcall->addr.len = v_osixti.nds_xti2addr.len;

   /*
    * Find CMX applikation by local nds-tsap 
    */
   if ((v_app = p_app_fbloc (v_osixti.nds_xti1addr.buf,
                             v_osixti.nds_xti1addr.len)) == NULL) {
      _t_error = T_WAPPLICATION;
      return (NULL);
   }

   if ((v_app->apmode & T_ACTIVE) == 0) {
      _t_error = T_WSEQUENCE;
      return (NULL);
   }

   return (v_app);
}

/************************************************************************/
/* t_conrq								*/
/************************************************************************/
int t_conrq	(int		 *tref,
		 union t_address *toaddr,
		 union t_address *fromaddr,
		 t_opt13	 *opt)
{
   tx_app		*v_app;
   tx_con		*v_conp;
   int			 v_rc;
   struct t_call	 v_sndcall;

   memset (&v_sndcall, 0, sizeof (v_sndcall));

   if (opt != NULL &&
       ((opt->optc1.t_optnr != T_OPTC1 && opt->optc3.t_optnr != T_OPTC3)
	|| opt->optc3.t_timeout < T_NOLIMIT
	|| opt->optc3.t_udatal < 0
	|| opt->optc3.t_udatal > T_CON_SIZE
	|| (opt->optc3.t_xdata != T_YES && opt->optc3.t_xdata != T_NO))) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   if (toaddr->tpartaddr.t_pamode == T_PAMODE) {
      if ((v_app = p_tns_conrq (toaddr, fromaddr, &v_sndcall)) == NULL)
	 goto errout;
   }
   else
   {
      if ((((T_adrdesc *) fromaddr)->t_infotype == T_TSAPINFO) ||
	  (((T_adrdesc *) fromaddr)->t_infotype == T_NSAPINFO)) {
	 if ((v_app = p_nds_conrq (toaddr, fromaddr, &v_sndcall)) == NULL)
	    goto errout;
      }
      else
      {
	 _t_error = T_WPARAMETER;
	 goto errout;
      }
   }

   /*
    * allocate and init a fresh connection element and
    * link it to the application block
    */
   if ((v_conp = p_con_cre (v_app, OR_CONN)) == NULLCP)
      goto errout;

   if ((v_conp->fd = tx_open (v_app, v_conp)) == -1) {
      p_con_free (v_app, v_conp, OR_RMFD | OR_CLFD | OR_CONN);
      goto errout;
   }
   v_conp->state = SX_OPEN;

   /*
    * if there are user-data, is it possible to send them ?
    */
   if (opt != NULL &&
       opt->optc1.t_udatal != 0 &&
       opt->optc1.t_udatal > v_conp->info.connect) {
      p_con_free (v_app, v_conp, OR_RMFD | OR_CLFD | OR_CONN);
      _t_error = T_WPARAMETER;
      goto errout;
   }

   /*
    * Ideally, qlen should always be 0, otherwise possible
    * passive connection setup requests could be met.
    */
   if (tx_bind (v_app, v_conp, 0) < 0) {
      p_con_free (v_app, v_conp, OR_RMFD | OR_CLFD | OR_CONN);
      goto errout;
   }
   v_conp->state = SX_BOUND;

   /*
    * fill the rest of the t_call-structur
    * (options currently not suported)
    */
   if (opt != NULL && opt->optc1.t_udatal != 0) {
      v_sndcall.udata.len = opt->optc1.t_udatal;
      v_sndcall.udata.buf = opt->optc1.t_udatap;
   }

   v_rc = tx_connect (v_conp, &v_sndcall);

   if (v_sndcall.addr.buf != NULL)
      free (v_sndcall.addr.buf);

   if (v_rc == -1) {
      p_con_free (v_app, v_conp, OR_RMFD | OR_CLFD | OR_CONN);
      return (T_ERROR);
   }

   /*
    * some TP's does a local connection acknowledge, in this case
    * the connection is fully established for the TP, but for
    * CMX we need t_event() and t_concf()
    */
   if (v_rc == 1) {
      /* connection fully established	*/
      v_conp->state = SX_OUTC_I;
      v_conp->event = T_CONCF;
   }
   else {
      /* connection establishment initiated	*/
      v_conp->state = SX_OUTC_C;
      FD_SET(v_conp->fd, &t_fdset);
      FD_SET(v_conp->fd, &t_xfds);
      FD_SET(v_conp->fd, &t_rfds);
   }

   v_conp->ucepid =
      (opt && opt->optc3.t_optnr == T_OPTC3) ? opt->optc3.t_ucepid : 0;

   *tref = SET_TREF(v_app->ind, v_conp->ind);
   return (T_OK);

errout:
   if (v_sndcall.addr.buf != NULL)
      free (v_sndcall.addr.buf);
   return (T_ERROR);
}

/************************************************************************/
/* t_conrs								*/
/************************************************************************/
int t_conrs	(int		*tref,
		 t_opt13	*opt)
{
   tx_app	*v_app;
   tx_con	*v_conp;

   if ((v_app = p_app_fbind (GET_APP_IND(tref))) == NULLAP
       || (v_conp = p_con_fbind (v_app->conp, GET_CON_IND(tref))) == NULLCP
       || v_conp->state != SX_INRS_C) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   if (opt != NULL &&
       ((opt->optc1.t_optnr != T_OPTC1 && opt->optc3.t_optnr != T_OPTC3)
	|| opt->optc3.t_udatal < 0
	|| opt->optc3.t_udatal > T_CON_SIZE
        || (opt->optc3.t_udatal != 0
         && opt->optc3.t_udatal > v_conp->info.connect)
	|| opt->optc3.t_xdata != T_NO
	|| opt->optc3.t_timeout < T_NOLIMIT
	|| (opt->optc3.t_udatal > 0 && opt->optc3.t_udatap == NULL))) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   /*
    * between t_conin() and t_conrs() there we may have got a T_DISCONNECT
    * within t_event, unknown by the caller
    */
   if ((v_conp->listq) == NULL) {
      v_conp->reason = T_USER;
      v_conp->state = SX_DISI_I;
      v_conp->event = T_DISIN;
      _t_error = T_COLLISION;
      return (T_ERROR);
   }

   /*
    * send user-data, bend therfore the buf-pointer
    */
   if (opt != NULL) {
      v_conp->listq->call.udata.len = opt->optc3.t_udatal;
      v_conp->listq->call.udata.buf = opt->optc3.t_udatap;
   }
   else
      v_conp->listq->call.udata.len = 0;

   if (tx_accept (v_app, v_conp) == -1) {
      v_conp->event = T_DISIN;
      v_conp->state = SX_DISI_I;
      return (T_ERROR);
   }

   v_conp->state = SX_DATAXFER;
   v_conp->ucepid =
      (opt && opt->optc3.t_optnr == T_OPTC3) ? opt->optc3.t_ucepid : 0;

   FD_SET(v_conp->fd, &t_fdset);
   FD_SET(v_conp->fd, &t_xfds);
   FD_SET(v_conp->fd, &t_rfds);
   fcntl (v_conp->fd, F_SETFD, (fcntl (v_conp->fd, F_GETFD) & ~FD_CLOEXEC));

   return (T_OK);
}


/************************************************************************/
/* t_datago								*/
/************************************************************************/
int t_datago	(int	*tref)
{
   tx_app	*v_app;
   tx_con	*v_conp;

   if ((v_app = p_app_fbind (GET_APP_IND(tref))) == NULLAP
       || (v_conp = p_con_fbind (v_app->conp, GET_CON_IND(tref))) == NULLCP
       || v_conp->state != SX_DATAXFER) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   v_conp->flwctrl &= ~FX_RCVSTOP;
   FD_SET(v_conp->fd, &t_rfds);

   return (T_OK);
}

/************************************************************************/
/* t_datain								*/
/************************************************************************/
int t_datain	(int	*tref,
		 char	*datap,
		 int	*datal,
		 int	*chain)
{
   int			v_size;
   int			v_vcnt = 1;
   struct t_data	v_vdata;

   v_vdata.t_datap = datap;
   v_vdata.t_datal = *datal;

   if ((v_size = t_vdatain(tref, &v_vdata, &v_vcnt, chain)) != T_ERROR)
      *datal = v_vdata.t_datal;

   return (v_size);
}

/************************************************************************/
/* t_datarq								*/
/************************************************************************/
int t_datarq	(int	*tref,
		 char	*datap,
		 int	*datal,
		 int	*chain)
{
   struct t_data	v_vdata;
   int			v_vcnt = 1;

   v_vdata.t_datap = datap;
   v_vdata.t_datal = *datal;
   return (t_vdatarq(tref, &v_vdata, &v_vcnt, chain));
}

/************************************************************************/
/* t_datastop								*/
/************************************************************************/
int t_datastop	(int	*tref)
{
   tx_app	*v_app;
   tx_con	*v_conp;

   if ((v_app = p_app_fbind (GET_APP_IND(tref))) == NULLAP
       || (v_conp = p_con_fbind (v_app->conp, GET_CON_IND(tref))) == NULLCP
       || v_conp->state != SX_DATAXFER) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   v_conp->flwctrl |= FX_RCVNSTOP;
   FD_CLR(v_conp->fd, &t_rfds);

   return (T_OK);
}

/************************************************************************/
/* t_detach								*/
/************************************************************************/
int t_detach	(struct t_myname	*pv_name)
{
   tx_app		*v_app;
   tx_tmn		*v_tmn;
   int			 v_cnt = 0;

   /*
    * Find t_myname
    */
   if ((v_tmn = p_tmn_fbname (pv_name)) == NULL) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   while ((v_app = p_app_fbtmn (v_tmn->ind)) != NULL) {
      p_app_free (v_app, OR_RMFD | OR_DISC);
      v_cnt++;
   }

   /*
    * No one application found to be freed?
    */
   if (v_cnt == 0) {
      _t_error = T_WAPPLICATION;
      return (T_ERROR);
   }

   return (T_OK);
}


/************************************************************************/
/* t_disin								*/
/************************************************************************/
int t_disin	(int	*tref,
		 int	*reason,
		 t_opt2	*opt)
{
   tx_app	*v_app;
   tx_con	*v_conp;
   tx_reason	*v_reap;
   int		 v_done = 0;

   if ((v_app = p_app_fbind (GET_APP_IND(tref))) == NULLAP
       || (v_conp = p_con_fbind (v_app->conp, GET_CON_IND(tref))) == NULLCP
       || v_conp->state != SX_DISI_I
       || v_conp->elast != T_DISIN) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   if (opt != NULL &&
       (opt->optc2.t_optnr != T_OPTC2
	|| opt->optc2.t_udatal < 0
	|| (opt->optc2.t_udatal > 0
	 && opt->optc2.t_udatal < v_conp->udata.len))) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   if ((v_reap = v_app->maprea) != NULL) {
      while (!v_done && (v_reap->xti | v_reap->cmx) != 0) {
         if (v_reap->xti == v_conp->reason) {
            *reason = v_reap->cmx;
            v_done = 1;
         }
         v_reap++;
      }
   }

   if (!v_done && (v_app->tpflag & TPF_ISOREASON) != 0) {
      switch (v_conp->reason) {
         case 128: *reason = T_USER;
 		   v_done = 1;
                   break;
         case   0:
         case   1:
         case   2:
         case   3:
         case 129:
         case 130:
         case 131:
         case 132:
         case 133:
         case 135:
         case 136:
         case 138:
		   *reason = v_conp->reason + T_RUNKNOWN;
 		   v_done = 1;
		   break;
      }
   }

   if (!v_done)
      *reason = v_conp->reason;

   if (opt != NULL) {
      /* if class 0 or the TP should have no user data, the user get none */
      if (v_conp->info.discon <= 0)
         opt->optc2.t_udatal = 0;
      if (opt->optc2.t_udatal != 0
       && (opt->optc2.t_udatal = v_conp->udata.len) != 0)
      memcpy (opt->optc2.t_udatap, v_conp->udata.buf, v_conp->udata.len);
   }

   v_conp->state = SX_BOUND;
   p_con_free (v_app, v_conp, OR_RMFD | OR_CLFD | OR_CONN);
   return (T_OK);
}

/************************************************************************/
/* t_disrq								*/
/************************************************************************/
int t_disrq	(int	*tref,
		 t_opt2	*opt)
{
   tx_app	*v_app;
   tx_con	*v_conp;
   struct t_call v_call;

   if ((v_app = p_app_fbind (GET_APP_IND(tref))) == NULLAP
       || (v_conp = p_con_fbind (v_app->conp, GET_CON_IND(tref))) == NULLCP) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   if (opt != NULL &&
       (opt->optc2.t_optnr != T_OPTC2
	|| opt->optc2.t_udatal < 0
	|| opt->optc2.t_udatal > T_DIS_SIZE
	|| (opt->optc2.t_udatal > 0
	 && opt->optc2.t_udatal > v_conp->info.discon))) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   if (opt != NULL) {
      memset (&v_call, 0, sizeof (v_call));
      v_call.udata.len = opt->optc2.t_udatal;
      v_call.udata.buf = opt->optc2.t_udatap;
      tx_snddis (v_conp, &v_call);
   }
   else
      tx_snddis (v_conp, NULL);

   v_conp->state = SX_BOUND;
   p_con_free (v_app, v_conp, OR_RMFD | OR_CLFD | OR_CONN);

   return (T_OK);
}

/************************************************************************/
/* t_event								*/
/************************************************************************/
int t_event	(int	*tref,
		 int	 cmode,
		 t_opte	*opt)
{
   tx_app		*v_app;
   tx_con		*v_conp, *v_ncp;
   tx_liststr		*v_listp;
   tx_dirxbuf		*v_dirxp;
   struct netbuf	 v_ladr;
   int		 	 v_evt = T_NOEVENT;
   int		 	 v_look;
   int		 	 v_nbytes;

   if (tx_p.datain.dcnt != 0) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   if (opt != NULL &&
       ((opt->opte1.t_optnr != T_OPTE1 &&
	 opt->opte2.t_optnr != T_OPTE2)
	|| opt->opte1.t_timeout < T_NOLIMIT)) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   v_app = tx_p.apptr ;
   assert (v_app != NULLAP);	/* prevented by cmx2k	*/

   /*
    * check for a pending T_REDIN
    */

   if (tx_redevent (&v_ladr) == 1) {
      if (v_ladr.len == sizeof(union cx_unisel)) {
	 if ((v_app = p_app_fbname ((union cx_unisel *)v_ladr.buf)) == NULLAP ||
	     (v_app->apmode | T_REDIRECT) == 0) {
	    tx_redin (NULL, T_WAPPLICATION);
	    free (v_ladr.buf);
	    goto nextout;
	 }
      }
      else {
         v_dirxp = (tx_dirxbuf *)v_ladr.buf;
	 if ((v_app = p_app_fbloc ((char *)&v_dirxp->tsap,
                                   v_dirxp->tsaplen)) == NULLAP ||
	     (v_app->apmode | T_REDIRECT) == 0) {
	    tx_redin (NULL, T_WAPPLICATION);
	    free (v_ladr.buf);
	    goto nextout;
         }
      }
      free (v_ladr.buf);

      if ((v_ncp = p_con_cre (v_app, OR_CONN)) == NULLCP) {
	 tx_redin (NULL, T_WCONN_LIMIT);
	 goto nextout;
      }

      if (tx_redin (v_ncp, 0) != -1) {
	 v_ncp->state = SX_REDIN_I;
	 v_evt = T_REDIN;
	 *tref = SET_TREF(v_app->ind, v_ncp->ind);
	 goto out;
      }
      else {
	 p_con_free (v_app, v_ncp, OR_RMFD | OR_CONN);
      }
   }

nextout:
   v_app = tx_p.apptr;

   /*
    * Loop through all applications for already queued t_listen's.
    */
   do {
      if (v_app->pconp != NULLCP &&
	  (v_listp = v_app->pconp->listq) != NULL) {
	 v_app->pconp->listq = v_listp->nxt;
	 v_listp->nxt = NULL;
	 if ((v_ncp = p_con_cre (v_app, OR_CONN)) == NULLCP) {
	    tx_snddisl (v_app, &v_listp->call);
	    free (v_listp);
	    continue;
	 }
	 v_ncp->listq = v_listp;
	 if ((v_ncp->fd = tx_open (v_app, v_ncp)) == -1 ||
	     tx_bind (v_app, v_ncp, 0) == -1) {
	    tx_snddisl (v_app, &v_listp->call);
	    v_ncp->state = SX_OPEN;
	    p_con_free (v_app, v_ncp, OR_RMFD | OR_CLFD | OR_CONN);
	    continue;
	 }
	 v_ncp->state = SX_INCO_I;
	 v_evt = T_CONIN;
	 *tref = SET_TREF(v_app->ind, v_ncp->ind);
      }
   } while (tx_p.apptr != (v_app = v_app->nxt) && v_evt == T_NOEVENT);

   if (v_evt != T_NOEVENT) {
      tx_p.apptr = v_app;
      goto out;
   }

   /*
    * Loop through all connect elements and report an unhandled event
    * THIS SHOULD BE DONE IN FASHION ALWAYS SHOWING THE OLDEST EVENT FIRST!
    */

   do {
      if ((v_conp = v_app->conp) != NULLCP) {
	 do {
	    switch (v_conp->state) {
	       case SX_OUTC_I:
	       case SX_INCO_I:
	       case SX_DISI_I:
	       case SX_REDIN_I:
		  v_evt = v_conp->event;
		  *tref = SET_TREF(v_app->ind, v_conp->ind);
		  break;
	    }
	    v_conp = v_conp->nxt;
	 } while (v_app->conp != v_conp && v_evt == T_NOEVENT);
      }
      if (v_evt != T_NOEVENT)
	 v_app->conp = v_conp;
   } while (tx_p.apptr != (v_app = v_app->nxt) && v_evt == T_NOEVENT);

   if (v_evt != T_NOEVENT) {
      tx_p.apptr = v_app;
      goto out;
   }

   /*
    * No event currently in state tables pending.
    * Loop through all application for T_CONIN's
    */
   do {
      if ((v_conp = v_app->pconp) != NULLCP) {
	 v_look = tx_look(v_conp->fd);
	 switch (v_look) {
	    case 0:			/* nothing to lose	*/
	       break;

	    case T_LISTEN:
	       if (tx_listen (v_app) == -1)
		  continue;
	       v_listp = v_app->pconp->listq;
	       v_app->pconp->listq = v_listp->nxt;
	       v_listp->nxt = NULL;
	       if ((v_ncp = p_con_cre (v_app, OR_CONN)) == NULLCP) {
		  tx_snddisl (v_app, &v_listp->call);
		  free (v_listp);
		  continue;
	       }
	       v_ncp->listq = v_listp;
	       if ((v_ncp->fd = tx_open (v_app, v_ncp)) == -1 ||
		   tx_bind (v_app, v_ncp, 0) == -1) {
		  tx_snddisl (v_app, &v_listp->call);
		  v_ncp->state = SX_OPEN;
		  p_con_free (v_app, v_ncp, OR_RMFD | OR_CLFD | OR_CONN);
		  continue;
	       }
	       v_ncp->state = SX_INCO_I;
	       v_evt = T_CONIN;
	       *tref = SET_TREF(v_app->ind, v_ncp->ind);
	       break;

	    case T_DISCONNECT:
	       tx_rcvdisl (v_app);	/* no error handling		*/
	       break;

	    default:
	       t_trc (T_STRING, 0, "Default/t_look");
	 }
      }
   } while (tx_p.apptr != (v_app = v_app->nxt) && v_evt == T_NOEVENT);

   if (v_evt != T_NOEVENT) {
      tx_p.apptr = v_app;
      goto out;
   }

   /*
    * Loop through all connection elements and check for events.
    */
   do {
      if ((v_conp = v_app->conp) != NULLCP) {
	 do {
	    v_look = tx_look(v_conp->fd);
	    switch (v_look) {
	       case 0:		/* nothing to lose	*/
		  break;

	       case T_CONNECT:
		  if (v_conp->state != SX_OUTC_C)
		     goto errout;
		  if (tx_rcvconnect(v_conp) == -1) {
		     v_conp->state = SX_DISI_I;
		     v_evt = T_DISIN;
		  }
		  else {
		     v_conp->state = SX_OUTC_I;
		     v_evt = T_CONCF;
		  }
		  break;

	       case T_DATA:
		  if (v_conp->state != SX_DATAXFER)
		     goto errout;
		  if ((v_conp->flwctrl & FX_RCVSTOP) != 0)
                     break;
		  if (tx_p.datain.dmem == NULL &&
		      (tx_p.datain.dmem = malloc (t_tidusize)) == NULL) {
		     tx_snddis(v_conp, NULL);
		     v_conp->state = SX_DISI_I;
		     v_conp->reason = T_RLCONGEST;
		     v_evt = T_DISIN;
		  }
		  else {
		     tx_p.datain.dbuf = tx_p.datain.dmem;
		     if (tx_rcv (v_conp, &tx_p.datain) == -1) {
			v_conp->state = SX_DISI_I;
			v_evt = T_DISIN;
		     }
		     else {
			v_conp->state = SX_DATAINP;
			v_evt = T_DATAIN;
		     }
		  }
		  break;

	       case T_DISCONNECT:
		  tx_rcvdis (v_conp);
		  v_conp->state = SX_DISI_I;
		  v_evt = T_DISIN;
		  break;

	       case T_ORDREL:
		  if (v_conp->state != SX_DATAXFER)
		     goto errout;
		  tx_rcvrel (v_conp);
		  v_conp->state = SX_DISI_I;
		  v_evt = T_DISIN;
		  break;

	       case T_GODATA:
		  if (v_conp->state != SX_DATAXFER)
		     goto errout;
		  FD_CLR(v_conp->fd, &t_wfds);
		  if (v_conp->dcnt != 0) {
		     v_nbytes = tx_snd (v_conp, v_conp->dbuf,
					v_conp->dcnt, v_conp->dflag);
		     if (v_nbytes == -1) {
			v_conp->state = SX_DISI_I;
			v_evt = T_DISIN;
		     }
		     else {
			v_conp->dbuf += v_nbytes;
			v_conp->dcnt -= v_nbytes;

			if (v_conp->dcnt == 0 &&
			    (v_conp->flwctrl & FX_SNDNSTOP) != 0) {
			   v_conp->flwctrl &= ~FX_SNDNSTOP;
			   v_evt = T_DATAGO;
			}
			else
			    FD_SET(v_conp->fd, &t_wfds);
		     }
		  }
		  else {
		     if ((v_conp->flwctrl & FX_SNDNSTOP) != 0) {
			v_conp->flwctrl &= ~FX_SNDNSTOP;
			v_evt = T_DATAGO;
		     }
		  }
		  break;

	       case T_EXDATA:		/* no expidited data	*/
	       case T_GOEXDATA:		/* no expidited data	*/
	       case T_LISTEN:		/* not on this fd	*/
		  goto errout;

	       case -1:
		  tx_snddis (v_conp, NULL);
		  v_conp->state = SX_DISI_I;
		  v_conp->reason = T_RLCONNLOST;
		  v_evt = T_DISIN;
		  break;

	       default:
		  t_trc (T_STRING, 0, "Default/t_look");
	    }
	    if (v_evt != T_NOEVENT)
	       *tref = SET_TREF(v_app->ind, v_conp->ind);
	 } while (v_app->conp != (v_conp = v_conp->nxt) && v_evt == T_NOEVENT);
      }
      v_app->conp = v_conp;
   } while (tx_p.apptr != (v_app = v_app->nxt) && v_evt == T_NOEVENT);

   if (v_evt == T_NOEVENT)
      return (T_NOEVENT);

   tx_p.apptr = v_app;
   goto out;


errout:
   tx_snddis (v_conp, NULL);
   v_conp->state = SX_DISI_I;
   v_conp->reason = T_RLPROTERR;
   *tref = SET_TREF(v_app->ind, v_conp->ind);
   _t_error = T_CCP_END;
   return (T_ERROR);

out:
   /*
    * All done. Return event and possibly tref and options
    */
   v_app = p_app_fbind (GET_APP_IND(tref));
   v_conp = p_con_fbind (v_app->conp, GET_CON_IND(tref));
   v_conp->event = v_conp->elast = v_evt;

   if (opt != NULL && opt->opte1.t_optnr == T_OPTE1) {
      opt->opte1.t_attid = v_app->tmn;
      opt->opte1.t_uattid = v_app->uattid;
      opt->opte1.t_ucepid = v_conp->ucepid;

      if (v_evt == T_DATAIN)
	 opt->opte1.t_evdat = tx_p.datain.dcnt;
      else
	 opt->opte1.t_evdat = 0;
   }

   if (v_evt == T_DATAIN)
      tx_p.datain.tref = *tref;

   return (v_evt);
}

/************************************************************************/
/* t_info								*/
/************************************************************************/
int t_info	(int	*tref,
		 t_opti	*opt)
{
   tx_app	*v_app;
   tx_con	*v_conp;

   if ((v_app = p_app_fbind (GET_APP_IND(tref))) == NULLAP
       || (v_conp = p_con_fbind (v_app->conp, GET_CON_IND(tref))) == NULLCP
       || v_conp->state != SX_DATAXFER) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   if (opt == NULL ||
       (opt->opti1.t_optnr != T_OPTI1 && opt->opti3.t_optnr != T_OPTI3)) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   opt->opti1.t_maxl = v_conp->mltidu;
   if (opt->opti3.t_optnr == T_OPTI3) {
      opt->opti3.t_fdsetsize = FD_SETSIZE;
      opt->opti3.t_openmax = open_max;
      opt->opti3.t_tsdu = (int)v_conp->info.tsdu;
      opt->opti3.t_etsdu = -2;

      if (v_conp->info.connect < T_CON_SIZE)
	 opt->opti3.t_condata = opt->opti3.t_concfdata = (int)v_conp->info.connect;
      else
	 opt->opti3.t_condata = opt->opti3.t_concfdata = T_CON_SIZE;

      if (v_conp->info.discon < T_DIS_SIZE)
	 opt->opti3.t_disdata = (int)v_conp->info.discon;
      else
	 opt->opti3.t_disdata = T_DIS_SIZE;

      opt->opti3.t_adrsize = (int)v_conp->info.addr;
   }
   return (T_OK);
}

/************************************************************************/
/* t_redin								*/
/************************************************************************/
int t_redin	(int		*tref,
		 int		*pid,
		 t_opt23	*opt)
{
   tx_app	*v_app;
   tx_con	*v_conp;
   int		 v_rc;

   if ((v_app = p_app_fbind (GET_APP_IND(tref))) == NULLAP
       || (v_conp = p_con_fbind (v_app->conp, GET_CON_IND(tref))) == NULLCP
       || v_conp->state != SX_REDIN_I
       || v_conp->elast != T_REDIN) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   if (opt != NULL &&
       ((opt->optc2.t_optnr != T_OPTC2 && opt->optc3.t_optnr != T_OPTC3)
	|| opt->optc2.t_udatal < 0
	|| (opt->optc2.t_udatal > 0
	 && opt->optc2.t_udatal < v_conp->udata.len))) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   v_conp->event = v_conp->elast = T_NOEVENT;

   /*
    * t_sync sollte bereits den state liefern, als su auf SCO 3.2 aber IDLE
    */
   if ((v_rc = tx_sync (v_conp->fd)) == -1 ||
       (v_rc != T_DATAXFER && tx_getstate(v_conp->fd) != T_DATAXFER)) {
      v_conp->reason = T_USER;
      v_conp->state = SX_DISI_I;
      v_conp->event = T_DISIN;
      _t_error = T_COLLISION;
      return (T_ERROR);
   }

   FD_SET(v_conp->fd, &t_fdset);
   FD_SET(v_conp->fd, &t_xfds);
   FD_SET(v_conp->fd, &t_rfds);
   v_conp->state = SX_DATAXFER;

   /*
    * get pid from redirector. In cmxxrd this pid was stored in
    * ucepid in the connection block.
    */
   *pid = v_conp->ucepid;       
   v_conp->ucepid = 0;   

   if (opt != NULL) {
      if (opt->optc2.t_udatal != 0
	  && (opt->optc2.t_udatal = v_conp->udata.len) != 0)
	 memcpy (opt->optc2.t_udatap, v_conp->udata.buf, v_conp->udata.len);
      if (opt->optc3.t_optnr == T_OPTC3) {
	 opt->optc3.t_xdata = T_NO;
	 opt->optc3.t_timeout = T_NO;
	 v_conp->ucepid = opt->optc3.t_ucepid;
      }
      else
	 v_conp->ucepid = 0;
   }

   if (v_conp->udata.buf != NULL)
      free (v_conp->udata.buf);
   memset (&v_conp->udata, 0, sizeof(v_conp->udata));

   return (T_OK);
}

/************************************************************************/
/* t_redrq								*/
/************************************************************************/
int t_redrq	(int		*tref,
		 int		*pid,
		 t_opt12	*opt)
{
   tx_app	*v_app;
   tx_con	*v_conp;
   tx_tmn	*v_tmn;
   char		*v_udatap;
   tx_dirxbuf	 v_dirxbuf;
   int		 v_udatal;
   int		 v_timeout;
   struct netbuf v_netbuf;

   if (tx_p.datain.dcnt != 0 && tx_p.datain.tref == *tref) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   if ((v_app = p_app_fbind (GET_APP_IND(tref))) == NULLAP
       || (v_conp = p_con_fbind (v_app->conp, GET_CON_IND(tref))) == NULLCP
       || v_conp->state != SX_DATAXFER
       || (v_conp->flwctrl & FX_SNDSTOP) != 0) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   if (*pid == tx_p.mypid
       || *pid <= SYSPID_MAX) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   v_timeout = v_udatal = 0;
   v_udatap = NULL;

   if (opt != NULL) {
      if (((opt->optc1.t_optnr != T_OPTC1 && opt->optc2.t_optnr != T_OPTC2)
	   || (v_udatal = opt->optc1.t_udatal) < 0
	   || opt->optc1.t_udatal > T_RED_SIZE
	   || (opt->optc1.t_optnr == T_OPTC1
	    && (v_timeout = opt->optc1.t_timeout) < T_NOLIMIT))) {
	 _t_error = T_WPARAMETER;
	 return (T_ERROR);
      }
      v_udatap = opt->optc1.t_udatap;
   }

   if (v_app->atyp == ATYP_TNS) {
      v_netbuf.len = sizeof(union cx_unisel);
      v_netbuf.buf = (char *)&v_app->tsel;

      if (tx_redrq (v_conp, &v_netbuf, *pid, v_timeout,
	  v_udatap, v_udatal) != 0)
	 return (T_ERROR);
   }
   else {
      v_tmn = p_tmn_fbind (v_app->tmn);
      v_dirxbuf.myname = v_tmn->myname;
      if ((v_dirxbuf.tsaplen = MIN(v_app->addr.len, sizeof(struct t_myname)))
          != 0)
         memcpy (&v_dirxbuf.tsap, v_app->addr.buf, v_dirxbuf.tsaplen);

      v_netbuf.len = sizeof(v_dirxbuf);
      v_netbuf.buf = (char *)&v_dirxbuf;

      if (tx_redrq (v_conp, &v_netbuf, *pid, v_timeout,
	  v_udatap, v_udatal) != 0)
	 return (T_ERROR);
   }
   /*
    * release connection structur and fd
    */

   tx_close (v_conp->fd);
   p_con_free (v_app, v_conp, OR_RMFD | OR_CONN);

   return (T_OK);
}

/************************************************************************/
/* t_vdatain								*/
/************************************************************************/
int t_vdatain	(int		*tref,
		 struct t_data	*vdata,
		 int		*vcnt,
		 int		*chain)
{
   tx_app       *v_app;
   tx_con       *v_conp;
   int		 v_ind;
   int		 v_cnt  = *vcnt;
   int		 v_size = 0;

   if ((v_app = p_app_fbind(GET_APP_IND(tref))) == NULLAP
       || (v_conp = p_con_fbind(v_app->conp, GET_CON_IND(tref))) == NULLCP
       || v_conp->state != SX_DATAINP
       || v_conp->elast != T_DATAIN) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   if (v_cnt < 1 || v_cnt > T_VCNT || vdata == NULL) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   for (v_ind = 0; v_ind < v_cnt; v_ind++) {
      if (vdata[v_ind].t_datap == NULL || vdata[v_ind].t_datal < 1) {
	 _t_error = T_WPARAMETER;
	 return (T_ERROR);
      }
   }

   for (v_ind = 0; v_ind < v_cnt; v_ind++) {
      v_size = MIN(tx_p.datain.dcnt, vdata[v_ind].t_datal);
      if (v_size > 0) {
	 memcpy (vdata[v_ind].t_datap, tx_p.datain.dbuf, v_size);
	 tx_p.datain.dcnt -= v_size;
	 tx_p.datain.dbuf += v_size;
      }
      vdata[v_ind].t_datal = v_size;
   }

   if (tx_p.datain.dcnt == 0) {
      *chain = tx_p.datain.dflag;
      v_conp->state = SX_DATAXFER;
      v_conp->event = v_conp->elast = T_NOEVENT;
   }
   *chain = tx_p.datain.dflag;

   return (tx_p.datain.dcnt);
}

/************************************************************************/
/* t_vdatarq								*/
/************************************************************************/
int t_vdatarq	(int		*tref,
		 struct t_data	*vdata,
		 int		*vcnt,
		 int		*chain)
{
   tx_app       *v_app;
   tx_con       *v_conp;
   char		*v_ptr;
   int		 v_ind;
   int		 v_cnt  = *vcnt;
   int		 v_size = 0;
   int		 v_chain;
   int		 v_sent;

   if ((v_app = p_app_fbind(GET_APP_IND(tref))) == NULLAP
       || (v_conp = p_con_fbind(v_app->conp, GET_CON_IND(tref))) == NULLCP
       || v_conp->state != SX_DATAXFER
       || (v_conp->flwctrl & FX_SNDSTOP) != 0) {
      _t_error = T_WSEQUENCE;
      return (T_ERROR);
   }

   if ((*chain != T_CMX_MORE && *chain != T_END)
       || v_cnt < 1 || v_cnt > T_VCNT || vdata == NULL) {
      _t_error = T_WPARAMETER;
      return (T_ERROR);
   }

   for (v_ind = 0; v_ind < v_cnt; v_ind++) {
      v_size += vdata[v_ind].t_datal;
      if (vdata[v_ind].t_datap == NULL || vdata[v_ind].t_datal < 1
	  || v_size > v_conp->mltidu) {
	 _t_error = T_WPARAMETER;
	 return (T_ERROR);
      }
   }

   if (*chain == T_CMX_MORE)
      v_chain = T_XTI_MORE;
   else
      v_chain = 0;			/* no flag set means T_END	*/

   if (v_cnt == 1) {
      v_sent = tx_snd (v_conp, vdata[0].t_datap, vdata[0].t_datal, v_chain);

      if (v_sent == -1) {
	 v_conp->state = SX_DISI_I;
	 v_conp->event = T_DISIN;
	 return (T_ERROR);
      }

      if ((v_size -= v_sent) != 0) {
	 if (v_conp->dmem == NULL &&
	     (v_conp->dmem = malloc (v_conp->mltidu)) == NULL)
	    goto errout;

	 v_conp->dbuf = v_conp->dmem;
	 memcpy (v_conp->dbuf, vdata[0].t_datap + v_sent, v_size);
      }
   }
   else {
      if (v_conp->dmem == NULL &&
	  (v_conp->dmem = malloc (v_conp->mltidu)) == NULL)
	 goto errout;

      v_ptr = v_conp->dmem;

      for (v_ind = 0; v_ind < v_cnt; v_ind++) {
	 memcpy (v_ptr, vdata[v_ind].t_datap, vdata[v_ind].t_datal);
	 v_ptr += vdata[v_ind].t_datal;
      }

      v_sent = tx_snd (v_conp, v_conp->dmem, v_size, v_chain);

      if (v_sent == -1) {
	 v_conp->state = SX_DISI_I;
	 v_conp->event = T_DISIN;
	 return (T_ERROR);
      }
      v_size -= v_sent;
      v_conp->dbuf = v_conp->dmem + v_sent;
   }

   if (v_size != 0) {
      v_conp->dcnt = v_size; 		/* # of bytes to snd left	*/
      v_conp->dflag = v_chain;
      v_conp->flwctrl |= FX_SNDNSTOP;
      FD_SET(v_conp->fd, &t_wfds);
      return (T_DATASTOP);
   }

   return (T_OK);

errout:
   tx_snddis(v_conp, NULL);
   v_conp->state = SX_DISI_I;
   v_conp->event = T_DISIN;
   v_conp->reason = T_RLCONGEST;
   _t_error = T_ENOMEM;
   return (T_ERROR);
}

/************************************************************************/
/* t_xdatgo								*/
/************************************************************************/
int t_xdatgo	(int	*tref)
/* ARGSUSED */
{
   _t_error = T_WSEQUENCE;
   return (T_ERROR);
}

/************************************************************************/
/* t_xdatin								*/
/************************************************************************/
int t_xdatin	(int	*tref,
		 char	*datap,
		 int	*datal)
/* ARGSUSED */
{
   _t_error = T_WSEQUENCE;
   return (T_ERROR);
}

/************************************************************************/
/* t_xdatrq								*/
/************************************************************************/
int t_xdatrq	(int	*tref,
		 char	*datap,
		 int	*datal)
/* ARGSUSED */
{
   _t_error = T_WSEQUENCE;
   return (T_ERROR);
}

/************************************************************************/
/* t_xdatstop								*/
/************************************************************************/
int t_xdatstop	(int	*tref)
/* ARGSUSED */
{
   _t_error = T_WSEQUENCE;
   return (T_ERROR);
}

/*
 * p_tmn_cre - create and link tmyname element to chain and
 *             estimate the tmyname-ID
 */
static tx_tmn *p_tmn_cre (void)
{
   tx_tmn	*insptr, *hcomp;
   int		 ind;

   if (tx_p.nr_tmn >= TX_APP_MAX) {
      _t_error = T_WAPP_LIMIT;
      return (NULL);
   }

   if ((insptr = (tx_tmn *) calloc(1, sizeof(tx_tmn)))
       == NULL) {
      _t_error = T_ENOMEM;
      return (NULL);
   }

   tx_p.nr_tmn++;

   if (tx_p.tmnptr == NULL) {
      tx_p.tmnptr = insptr->nxt = insptr->prv = insptr;
      insptr->ind = 1;
      return (insptr);
   }
   hcomp = tx_p.tmnptr;
   do {
      if ((ind = ((tx_p.tmnptr->ind + 1) & TX_APP_MASK))
	  != tx_p.tmnptr->nxt->ind) {
	 insptr->nxt = tx_p.tmnptr->nxt;
	 insptr->prv = tx_p.tmnptr;
	 insptr->nxt->prv = tx_p.tmnptr->nxt = insptr;
	 insptr->ind = ind;
	 tx_p.tmnptr = insptr;
	 return (insptr);
      }
      tx_p.tmnptr = tx_p.tmnptr->nxt;
   } while (tx_p.tmnptr != hcomp);

   assert (0);	/* CANNOT HAPPEN */
}

/*
 * p_tmn_free - decrease counter in tmyname element. If its not longer used
 * - counter is zero - unlink tmyname element from chain.
 */
static void p_tmn_free	(ui32	pv_ind)
{
   tx_tmn	*v_ptr;

   v_ptr = p_tmn_fbind (pv_ind);

   if (--v_ptr->cnt > 0)
      return;

   /*
    * unqueue and remove tmynameblock
    */
   if ((tx_p.tmnptr)->nxt == tx_p.tmnptr)
      tx_p.tmnptr = NULL;
   else {
      if (tx_p.tmnptr == v_ptr)
	 tx_p.tmnptr = v_ptr->nxt;
      v_ptr->prv->nxt = v_ptr->nxt;
      v_ptr->nxt->prv = v_ptr->prv;
   }
   free (v_ptr);
   tx_p.nr_tmn--;
}

/*
 * tmyname find by tmyname
 */
static tx_tmn *p_tmn_fbname	(struct t_myname	*pv_name)
{
   tx_tmn	*v_ptr;

   if ((v_ptr = tx_p.tmnptr) == NULL)
      return (NULL);

   do {
      if (memcmp (&v_ptr->myname, pv_name, 
	          MIN(v_ptr->myname.t_mnlng, sizeof(struct t_myname))) == 0)
	 return (v_ptr);
      v_ptr = v_ptr->nxt;
   }
   while (v_ptr != tx_p.tmnptr);
   return (NULL);
}


static tx_tmn *p_tmn_fbind	(ui32	pv_ind)
{
   tx_tmn	*v_ptr;

   if ((v_ptr = tx_p.tmnptr) == NULL) {
      t_trc (T_STRING, 0, "tmyname queue emty");
      assert (0);			/* should not happen		*/
   }

   do {
      if (v_ptr->ind == pv_ind)
	 return (v_ptr);
      v_ptr = v_ptr->nxt;
   }
   while (v_ptr != tx_p.tmnptr);

   t_trc (T_STRING, 0, "tmyname-element not found");
   assert (0);			/* should not happen		*/
}


/*
 * p_app_cre - create and link application element to chain and
 *             estimate the Application-ID
 */
static tx_app *p_app_cre (void)
{
   tx_app	*insptr, *hcomp;
   int		 ind;

   if (tx_p.nr_att >= TX_APP_MAX) {
      _t_error = T_WAPP_LIMIT;
      return (NULLAP);
   }

   if ((insptr = (tx_app *) calloc(1, sizeof(tx_app)))
       == NULLAP) {
      _t_error = T_ENOMEM;
      return (NULLAP);
   }

   tx_p.nr_att++;
   insptr->tmn = NO_TMN;
   insptr->tpflag = TPF_ISOREASON;

   if (tx_p.apptr == NULLAP) {
      tx_p.apptr = insptr->nxt = insptr->prv = insptr;
      insptr->ind = 0;
      return (insptr);
   }
   hcomp = tx_p.apptr;
   do {
      if ((ind = ((tx_p.apptr->ind + 1) & TX_APP_MASK))
	  != tx_p.apptr->nxt->ind) {
	 insptr->nxt = tx_p.apptr->nxt;
	 insptr->prv = tx_p.apptr;
	 insptr->nxt->prv = tx_p.apptr->nxt = insptr;
	 insptr->ind = ind;
	 tx_p.apptr = insptr;
	 return (insptr);
      }
      tx_p.apptr = tx_p.apptr->nxt;
   } while (tx_p.apptr != hcomp);

   assert (0);	/* CANNOT HAPPEN */
}

/*
 * p_app_free - unlink application element from chain,
 * shutdown its connections, close its sockets and free its connect elements
 *
 * aptr	  application pointer
 * flags   orders
 */
static void p_app_free	(tx_app	*delptr,
			 ui32	 flags)
{
   int	v_terror;

   /*
    * we dont want do falsify the _t_error
    */
   v_terror = _t_error;

   assert ((tx_p.apptr != NULLAP && delptr != NULLAP));
					/* CANNOT HAPPEN 	*/
   /*
    * remove all connection blocks
    */
   while (delptr->conp != NULLCP)
      p_con_free (delptr, delptr->conp, flags | OR_CONN);
   if (delptr->pconp != NULLCP)
      p_con_free (delptr, delptr->pconp, flags | OR_CLFD | OR_PCON);


   if (delptr->tmn != NO_TMN)
      p_tmn_free (delptr->tmn);

   if (delptr->maprea != NULL)
      free (delptr->maprea);

   if (delptr->addr.buf != NULL)
      free (delptr->addr.buf);
   /*
    * unqueue and remove application block
    */
   if ((tx_p.apptr)->nxt == tx_p.apptr) {
      tx_p.apptr = NULLAP;
      tx_redclose();
      if (tx_p.datain.dmem != NULL) {
	 free (tx_p.datain.dmem);
	 tx_p.datain.dmem = NULL;
      }
      assert (tx_p.tmnptr == NULL);
   }
   else {
      if (tx_p.apptr == delptr)
	 tx_p.apptr = delptr->nxt;
      delptr->prv->nxt = delptr->nxt;
      delptr->nxt->prv = delptr->prv;
   }
   free (delptr);
   tx_p.nr_att--;

   _t_error = v_terror;
}

/*
 * Application find by name
 */
static tx_app *p_app_fbname	(union cx_unisel	*pv_tsel)
{
   tx_app	*v_ptr;

   if ((v_ptr = tx_p.apptr) == NULL)
      return (NULLAP);

   do {
      if (memcmp (&v_ptr->tsel, pv_tsel, sizeof(union cx_unisel)) == 0)
	 return (v_ptr);
      v_ptr = v_ptr->nxt;
   }
   while (v_ptr != tx_p.apptr);
   return (NULLAP);
}

/*
 * Application find by ind
 *
 * usually after t_event() the anchor into the application-list points
 * to the requested application + 1. Therefore we search in reverse order.
 */
static tx_app *p_app_fbind	(ui32	pv_ind)
{
   tx_app	*v_ptr;

   if ((v_ptr = tx_p.apptr) == NULL)
      return (NULLAP);

   do {
      if (v_ptr->ind == pv_ind)
	 return (v_ptr);
      v_ptr = v_ptr->prv;
   }
   while (v_ptr != tx_p.apptr);
   return (NULLAP);
}

/*
 * Application find by tmn-index
 */
static tx_app *p_app_fbtmn	(ui32	pv_tmn)
{
   tx_app	*v_ptr;

   if ((v_ptr = tx_p.apptr) == NULL)
      return (NULLAP);

   do {
      if (v_ptr->tmn == pv_tmn)
	 return (v_ptr);
      v_ptr = v_ptr->prv;
   }
   while (v_ptr != tx_p.apptr);
   return (NULLAP);
}

/*
 * Application find by an local address
 *
 * At nds t_conrq we get not the same locale name as by t_attach!
 * we get just a part of it, so we search just this, without tmn.
 */
static tx_app *p_app_fbloc	(char	*pv_loc,
				 int	 pv_len)
{
   tx_app	*v_ptr;

   if ((v_ptr = tx_p.apptr) == NULL)
      return (NULLAP);

   do {
      if (v_ptr->addr.len == pv_len && 
          memcmp (v_ptr->addr.buf, pv_loc, pv_len) == 0)
	 return (v_ptr);
      v_ptr = v_ptr->prv;
   }
   while (v_ptr != tx_p.apptr);
   return (NULLAP);
}

/*
 * p_pcon_cre   Create a connection element for passive connection establishment
 */
static int p_pcon_cre	(tx_app	*pv_app)
{
   tx_con	*v_conp;
   int		 v_rc;
   int		 v_qlen;

   if ((v_conp = p_con_cre (pv_app, OR_PCON)) == NULLCP)
      return (T_ERROR);

   if ((v_conp->fd = tx_open (pv_app, v_conp)) == -1) {
      p_con_free (pv_app, v_conp, OR_RMFD | OR_CLFD | OR_PCON);
      return (T_ERROR);
   }
   v_conp->state = SX_OPEN;

   /*
    * The user limits with conlim the number of connections for this 
    * process. qlen is the number of pending conection establishments.
    * qlen have also another meaning. There are some TP's with a limited
    * qlen for all processes. One process can take all of them, and no other
    * process can have  a passive connection establishment.
    */
   v_qlen = (pv_app->conlim + 15) / 16;

   /*
    * Return code of 0 means, we've got qlen == 0 or another address,
    * so we cannot receive any connection establishement.
    */
   if ((v_rc = tx_bind (pv_app, v_conp, v_qlen)) <= 0) {
      p_con_free (pv_app, v_conp, OR_RMFD | OR_CLFD | OR_PCON);
      if (v_rc == -1)
	 return (T_ERROR);
      else
	 return (T_NOTFIRST);
   }
   v_conp->state = SX_BOUND;
   FD_SET(v_conp->fd, &t_fdset);
   FD_SET(v_conp->fd, &t_xfds);
   FD_SET(v_conp->fd, &t_rfds);

   return (T_OK);
}

/*
 * p_con_cre - Create, init and link a new connection element
 */
static tx_con *p_con_cre (tx_app *pv_app,
			  ui32	  pv_flag)
{
   tx_con	**chdr, *insptr, *hcomp;
   int		  ind;

   if ((pv_flag & OR_CONN) != 0) {
      if (pv_app->conlim == 0) {
	 _t_error = T_WCONN_LIMIT;
	 return (NULLCP);
      }
      chdr = &pv_app->conp;
   }
   else {
      if ((pv_flag & OR_PCON) != 0)
	 chdr = &pv_app->pconp;
      else
	 assert (0);			/* internal error	*/
   }

   if ((insptr = (tx_con *) calloc (1, sizeof(tx_con))) == NULLCP) {
      _t_error = T_ENOMEM;
      return (NULLCP);
   }

   insptr->fd = NOFD;
   if ((pv_flag & OR_CONN) != 0)
      pv_app->conlim--;		/* decrease possible new connections	*/

   if (*chdr == NULLCP) {
      *chdr = insptr->nxt = insptr->prv = insptr;
      insptr->ind = 1;
      return (insptr);
   }
   hcomp = *chdr;
   do {
      if ((ind = (((*chdr)->ind + 1) & TX_CON_MASK))
	  != (*chdr)->nxt->ind) {
	 insptr->nxt = (*chdr)->nxt;
	 insptr->prv = *chdr;
	 insptr->nxt->prv = (*chdr)->nxt = insptr;
	 insptr->ind = ind;
	 *chdr = insptr;
	 return (insptr);
      }
      *chdr = (*chdr)->nxt;
   } while (hcomp != *chdr);

   assert (0);		/* CANNOT HAPPEN */
}

/*
 * p_con_free - unlink connection element from chain
 * shutdown its connections, close its sockets and free its connect elements
 *
 * chdr    Header on application list
 * cptr	  connection pointer
 * flags   orders
 *		OR_CLFD	close fd
 *		OR_RMFD	remove fd from FDSET
 *		OF_DISC	close connection (if required snddis, unbind and close)
 */
static void p_con_free	(tx_app	*pv_app,
			 tx_con	*delptr,
			 ui32	 flags)
{
   int		 v_fd;
   int		 v_terror;
   tx_liststr	*v_listp;
   tx_con      **chdr = NULL;

   /*
    * we don't want do falsify the _t_error
    */
   v_terror = _t_error;

   if ((flags & OR_CONN) != 0) {
      pv_app->conlim++;
      chdr = &pv_app->conp;
      if (tx_p.datain.tref == SET_TREF(pv_app->ind, delptr->ind))
	 tx_p.datain.dcnt = 0;
   }
   else {
      if ((flags & OR_PCON) != 0)
	 chdr = &pv_app->pconp;
   }

   assert ((chdr != NULL && *chdr != NULLCP && delptr != NULLCP));
      					/* CANNOT HAPPEN 	*/

   if ((v_fd = delptr->fd) != NOFD) {
      /*
       * always remove fd from FDSET
       */
      FD_CLR(v_fd, &t_fdset);
      FD_CLR(v_fd, &t_xfds);
      FD_CLR(v_fd, &t_rfds);
      FD_CLR(v_fd, &t_wfds);

      while ((v_listp = delptr->listq) != NULL) {
         delptr->listq = v_listp->nxt;
         tx_snddisl (pv_app, &v_listp->call);
	 free (v_listp);
      }

      if (((flags & OR_DISC) != 0) && (delptr->state > SX_BOUND))
	 tx_snddis (delptr, NULL);

      if ((flags & OR_CLFD) != 0) {
	 if (delptr->state >= SX_BOUND) {
	    tx_unbind (delptr);
	 }
	 if (delptr->state >= SX_OPEN) {
	    tx_close (v_fd);
	 }
      }
   }

   if (delptr->udata.buf != NULL)
      free (delptr->udata.buf);
   if (delptr->dmem != NULL)
      free (delptr->dmem);

   /*
    * unqueue and remove connection block
    */
   if (delptr->nxt == delptr)
      *chdr = NULLCP;
   else {
      delptr->prv->nxt = delptr->nxt;
      delptr->nxt->prv = delptr->prv;
      *chdr = delptr->nxt;
   }
   free (delptr);

   if (v_terror != 0)
      _t_error = v_terror;
}


/*
 * Connection find by ind
 *
 * usually after t_event() the anchor into the connection-list points
 * to the requested connection + 1. Therefore we search in reverse order.
 */
static tx_con *p_con_fbind (tx_con	*pv_con,
			    ui32	 pv_ind)
{
   tx_con	*v_ptr = pv_con;

   do {
      if (v_ptr->ind == pv_ind)
	 return (v_ptr);
      v_ptr = v_ptr->prv;
   }
   while (v_ptr != pv_con);
   return (NULLCP);
}
