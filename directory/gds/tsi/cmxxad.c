/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxxad.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:31  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:54  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:48:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:38  root]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:40  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:22:36  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:48:23  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:46:42  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:41  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:52  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:39  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:46  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxxad.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:31 $";
#endif

/*
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

/*
static char	sccsid[] = "@(#)cmxxad.c	311.16 93/08/24 CMX (NC1)";
*/

/*****************************************************************************/
/*                                                                           */
/*        Address mapping for CMX on top of XTI/TLI                          */
/*        =========================================                          */
/*                                                                           */
/* Author: kam/mai                                       Date:    03.03.93   */
/*****************************************************************************/

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#if defined(_AIX)
#include <sys/select.h>
#endif
#if defined(SO_AVAIL)
#include <dlfcn.h>
#endif

#include "cmx.h"
#undef	T_MORE
#undef	T_CHECK

#include "xti.h"
#undef	T_MORE
#undef	T_CHECK

#include <sys/signal.h>
#include "cmxextern.h"

#include "cmxaddr.h"
#include "cmx_tlv.h"
#include "cmxxti.h"
#include "cmxtrace.h"
#ifdef	ALLOC_TEST
#include "alloctrc.h"
#endif	/* ALLOC_TEST */

#ifdef __TURBOC__
static char	sv_fncfg [] = "cmxti.cfg";
#else
static char	sv_fncfg [] = "/opt/lib/cmx/cmxti.cfg";
#endif

#define ML_CFG  256     /* maximum line length in CMXTICFG-File         */


typedef struct
{
   unsigned short        type;  /* Network type only one bit is set     */
   unsigned short	 cnt;	/* currently not used			*/
   char                 *tp;    /* transport provider                   */
   char                 *path;  /* path to the mapping library          */
   char                 *opt;   /* optional data from the config file   */
} tx_cfg;

/*
 * config data
 */
static	tx_cfg	*sv_cfg    = NULL;
static	int	 sv_cfgcnt = 0;
static	tx_am	 sv_am;

#ifndef	SO_AVAIL

typedef struct {
   char		*amname;
   tx_am	*amfkt;
}  tx_amtab;

extern tx_am    tx_cmxxloc;     /* local communication - ti_cots        */
extern tx_am	tx_cmxxt2x;	/* map TNS to XTI(SNI)			*/
extern tx_am	tx_cmxx25i;	/* map WAN3SBKA to ISO-X25		*/
extern tx_am	tx_cmxxeic;	/* map WANNEA   to EICON		*/
extern tx_am    tx_cmxxtcp;     /* map LOOPSBKA over TCP/IP             */

static tx_amtab	sv_amtab [] =
{
#ifdef SCO
 { "cmxxeic", &tx_cmxxeic },
 { "cmxxtcp", &tx_cmxxtcp },
#endif /* SCO */
/*
 { "cmxxloc", &tx_cmxxloc },
 { "cmxxt2x", &tx_cmxxt2x },
 { "cmxx25i", &tx_cmxx25i },
 { "cmxxeic", &tx_cmxxeic },
*/
 { NULL,      NULL        }};

#endif  /* !SO_AVAIL */

/*
 * Prototyping
 */
tx_opttlv *p_bui_opt (int pv_pid, ui8 pv_mode, char *pv_cfgopt);


/*
 * p_zcpy	+zero copy
 */
static char *p_zcpy	(char	*pv_dst,
			 char	*pv_src)
{
   while ((*pv_dst++ = *pv_src++) != '\0');
   return (pv_dst);
}

static int p_find_net	(char	*pv_net)
{
   typedef struct {
      unsigned short	type;
      char		*txt;
   } txt2typ;

   static txt2typ	sv_txt2typ[] = {
      {CX_WANNEA,	"WANNEA"},	/* CCP type for WAN1, WAN2 */
      {CX_LANSBKA,	"LANSBKA"},	/* CCP type for LAN2 */
      {CX_RFC1006,	"RFC1006"},	/* CCP type for RFC1006 */
      {CX_WAN3SBKA,	"WAN3SBKA"},	/* CCP type for WAN3 */
      {CX_WANSBKA,	"WANSBKA"},	/* CCP type for WAN4, WAN5, WAN6 */
      {CX_STANEA,	"STANEA"},	/* CCP type for CCP-STA1 as of V2.1 */
      {CX_EMSNA,	"EMSNA"},	/* CCP type for pseudo-CCP with SNA */
      {CX_SDLCSBKA,	"SDLCSBKA"},	/* CCP type for STA1/SDLC */
      {CX_LANINET, 	"LANINET"},	/* CCP type for LAN1 */
      {CX_LBS2NEA,	"LBS2NEA"},	/* CCP type for C30 */
      {CX_LOOPSBKA, 	"LOOPSBKA"},	/* CCP type for CMX-LOCAL */
      {CX_TRSNASBKA,	"TRSNASBKA"},	/* CCP type for TRANSIT-LU6.2 */
      {CX_LANGWSBKA,	"LANGWSBKA"},	/* CCP type for LAN2-WAN-LAN2 Gateway */
      {CX_LANBSBKA,	"LANBSBKA"},	/* CCP type for LAN2-LAN2 Bridge */
      {CX_OSITYPE,	"OSITYPE"},	/* CCP type for OSITYPE */
      {CX_WANGWSBKA,	"WANGWSBKA"},	/* CCP type for WAN-LAN2 GATEWAY */
      {0,		NULL}
   };
   txt2typ	*v_ptr = sv_txt2typ;

   for (v_ptr = sv_txt2typ; v_ptr->txt != NULL; v_ptr++)
      if (strcmp (pv_net, v_ptr->txt) == 0)
	 return (v_ptr->type);

   return (0);
}

static int p_init_cfg	(void)
{
   char		*v_net;
   char		*v_tp;
   char		*v_path;
   char		*v_opt;
   char		*v_ptr;
   FILE		*v_fdcfg;
   int		 v_size;
   int		 v_lcnt;
   unsigned short v_type;
   unsigned short v_typset;
   char		 v_line [ML_CFG];
   char		 v_trc [80];

   if (sv_cfg != NULL)
      return (0);

   v_lcnt = v_size = 0;
   v_typset = 0;

   if ((v_fdcfg = fopen (sv_fncfg, "r")) == NULL) {
      sprintf (v_trc, "%s: errno %d\n", sv_fncfg, errno);
      t_trc (T_STRING, 0, v_trc);
      _t_error = ENOENT;
      return (-1);
   }

   while (fgets (v_line, ML_CFG, v_fdcfg) != NULL) {
      v_lcnt++;
      if (*v_line == '#')
	 continue;

      if ((v_net = strtok (v_line, " \t\n")) != NULL &&
	  (v_type = p_find_net (v_net)) != 0 &&
	  (v_type & v_typset) == 0 &&
	  (v_tp = strtok (NULL, " \t\n")) != NULL &&
	  (v_path = strtok (NULL, " \t\n")) != NULL) {
	 sv_cfgcnt++;
	 v_typset |= v_type;
	 v_size += strlen(v_net) + strlen(v_tp) + strlen(v_path) + 3;
	 if ((v_opt = strtok (NULL, " \t\n")) != NULL)
	    v_size += strlen(v_opt) + 1;
      }
      else {
	 sprintf (v_trc, "%s.%d: invalid line or duplicate network\n",
		  sv_fncfg, v_lcnt);
	 t_trc (T_STRING, 0, v_trc);
      }
   }

   fseek (v_fdcfg, 0L, 0);
   if ((sv_cfg = calloc (1, (sv_cfgcnt * sizeof(tx_cfg)) + v_size)) == NULL) {
      _t_error = T_ENOMEM;
      return (-1);
   }
   v_ptr = (char *) &sv_cfg[sv_cfgcnt];
   v_lcnt = -1;				/* reuse as index for sv_cfg	*/
   v_typset = 0;

   while (fgets (v_line, ML_CFG, v_fdcfg) != NULL) {
      if (*v_line == '#')
	 continue;

      if ((v_net = strtok (v_line, " \t\n")) != NULL &&
	  (v_type = p_find_net (v_net)) != 0 &&
	  (v_type & v_typset) == 0 &&
	  (v_tp = strtok (NULL, " \t\n")) != NULL &&
	  (v_path = strtok (NULL, " \t\n")) != NULL) {
	 v_typset |= v_type;
	 v_lcnt++;
	 sv_cfg[v_lcnt].type = v_type;
	 sv_cfg[v_lcnt].tp = v_ptr;
	 v_ptr = p_zcpy (v_ptr, v_tp);
	 sv_cfg[v_lcnt].path = v_ptr;
	 v_ptr = p_zcpy (v_ptr, v_path);
	 if ((v_opt = strtok (NULL, " \t\n")) != NULL) {
	    sv_cfg[v_lcnt].opt = v_ptr;
	    v_ptr = p_zcpy (v_ptr, v_opt);
	 }
      }
   }

   fclose (v_fdcfg);
   return (0);
}

static int p_find_cfg	(unsigned short	pv_type)
{
   int	v_ind;

   for (v_ind = 0; v_ind < sv_cfgcnt; v_ind++)
      if (sv_cfg[v_ind].type == pv_type)
	 return (v_ind);

   return (-1);
}

static int p_dll_path	(int	pv_cfgind)
{
   static char	*sv_dll_path = NULL;
#ifdef SO_AVAIL
   static void	*sv_dll_hndl;
#else
   tx_amtab	 *v_ptr;
   char		  v_trc[80];
#endif /* SO_AVAIL */

   if (sv_cfg[pv_cfgind].path == sv_dll_path)
      return (0);

#ifdef SO_AVAIL
   if (sv_dll_path != NULL) {
      /*
       * unmap shared object
       */
      if (dlclose (sv_dll_hndl) == -1) {
	 t_trc (T_DLERRO, 0, dlerror());
	 _t_error = T_EIO;
	 return (-1);
      }
      sv_dll_path = NULL;
   }

   /*
    * map shared object
    */
   if ((sv_dll_hndl = dlopen(sv_cfg[pv_cfgind].path, RTLD_LAZY)) == NULL) {
      t_trc (T_DLERRO, 0, dlerror());
      _t_error = T_W_NDS_ACCESS;
      return (-1);
   }

   if ((sv_am.myname2netbuf = 
          (TX_mn2nb  *)dlsym(sv_dll_hndl, "tx_myname2netbuf")) == NULL ||
       (sv_am.paddr2netbuf =
          (TX_pa2nb  *)dlsym(sv_dll_hndl, "tx_paddr2netbuf")) == NULL ||
       (sv_am.netbuf2paddr =
          (TX_nb2pa  *)dlsym(sv_dll_hndl, "tx_netbuf2paddr")) == NULL ||
       (sv_am.getopt =
          (TX_getopt *)dlsym(sv_dll_hndl, "tx_getopt")) == NULL ||
       (sv_am.setopt =
          (TX_setopt *)dlsym(sv_dll_hndl, "tx_setopt")) == NULL ||
       (sv_am.getinf =
          (TX_getinf *)dlsym(sv_dll_hndl, "tx_getinf")) == NULL) {
      t_trc (T_DLERRS, 0, dlerror());
      _t_error = T_W_NDS_ACCESS;
      return (-1);
   }
#else
   v_ptr = sv_amtab;
   while (v_ptr->amname != NULL) {
      if (strcmp (v_ptr->amname, sv_cfg[pv_cfgind].path) == 0) {
	 sv_am = *v_ptr->amfkt;
	 break;
      }
      v_ptr++;
   }
   if (v_ptr->amname == NULL) {
      sprintf (v_trc, "<%s> not in map table", sv_cfg[pv_cfgind].path);
      t_trc (T_STRING, 0, v_trc);
      _t_error = T_W_NDS_ACCESS;
      return (-1);
   }
#endif /* SO_AVAIL */

   sv_dll_path = sv_cfg[pv_cfgind].path;
   return (0);
}

tx_opttlv *p_bui_opt	(int	pv_pid,
			 ui8	pv_mode,
			 char  *pv_cfgopt)
{
   static tx_opttlv	sv_opt;

   int		v_rc;
   char		v_pid[16];

   sv_opt.lng = 0;

   if (pv_pid != 0) {
      sprintf (v_pid, "%d", pv_pid);
      v_rc = cxtlv_create (AMO_T_REDIR, strlen(v_pid), (ui8 *)v_pid,
			   sv_opt.tlv, AMO_TLV_SIZE);
      if (v_rc < 0)
	 return (NULL);
      sv_opt.lng = v_rc;

      v_rc = cxtlv_create (AMO_T_MODE, 1, &pv_mode, 
			   sv_opt.tlv+sv_opt.lng, AMO_TLV_SIZE - sv_opt.lng);
      if (v_rc < 0)
	 return (NULL);
      sv_opt.lng += v_rc;
   }

   if (pv_cfgopt != NULL) {
      v_rc = cxtlv_create (AMO_T_CFG,
			   sizeof(pv_cfgopt) + 1, (ui8 *)pv_cfgopt,
			   sv_opt.tlv+sv_opt.lng, AMO_TLV_SIZE - sv_opt.lng);
      if (v_rc < 0)
	 return (NULL);
      sv_opt.lng += v_rc;
   }

   if (sv_opt.lng == 0)
      return (NULL);
   else
      return (&sv_opt);
}


/*
 * tx_amsel	Address mapping for TSEL
 *
 * Returncodes:
 *	-1	Error, _t_error set
 *	 0	nothing found for the adresstyp
 *	 1	ok
 */

int tx_amsel	(union cx_unisel	*pv_tsel,
		 struct netbuf		*pv_addr,
		 char  		       **pv_tp,
		 int	 		 pv_pid,
		 ui8			 pv_mode)
{
   int	 		v_ind;
   struct netbuf       *v_tsel;
   tx_opttlv	       *v_opt;

   if (pv_tsel->cxs.cxs_lng <= 0 ||
       pv_tsel->cxs.cxs_lng > CXS_SIZE) {
      _t_error = T_WPARAMETER;
      return (-1);
   }

   if (p_init_cfg () == -1)
      return (-1);

   /*
    * check if this CCP-Type is currently supported
    */
   if ((v_ind = p_find_cfg (pv_tsel->cxs.cx_type)) == -1)
      return (0);			/* sorry, no	*/

   if (p_dll_path (v_ind) == -1)
      return (-1);

   /*
    * everything fine, now we fill the tp into the application structure
    */
   *pv_tp = sv_cfg[v_ind].tp;

   /*
    * translate TSEL-part (just one cx_unisel)
    * in case of pv_pid != 0 (redirection), pv_tsel is not needed
    */
   v_opt = p_bui_opt (pv_pid, pv_mode, sv_cfg[v_ind].opt);

   if (pv_pid == 0)
      v_tsel = (*sv_am.myname2netbuf) (pv_tsel, v_opt);
   else
      v_tsel = (*sv_am.myname2netbuf) (NULL, v_opt);

   if (v_tsel == NULL) {
      _t_error = T_WPARAMETER;
      return (-1);
   }

   if ((pv_addr->buf = malloc (v_tsel->len)) == NULL) {
      _t_error = T_ENOMEM;
      return (-1);
   }

   pv_addr->maxlen = pv_addr->len = v_tsel->len;
   memcpy (pv_addr->buf, v_tsel->buf, v_tsel->len);

   return (1);
}

/* 
 * tx_tpinfo
 *
 * gets some TP specific information
 * - Flags
 * - reason mapping table
 *
 * ATTENTION: this code have to be called directly after tx_amsel,
 *            the correct Adress mapping library must be linked,
 *	      here no dynamic linking will be done
 *
 * Errors will be ignored!
 */
void tx_tpinfo	(int			*pv_flag,
		 tx_reason	       **pv_rtab)
{
   tx_reason	*v_rtab;
   int		 v_size;

   (*sv_am.getinf) (pv_flag, &v_size, &v_rtab);

   if (v_size != 0) {
      if ((*pv_rtab = malloc (v_size)) == NULL)
         return;
      memcpy (*pv_rtab, v_rtab, v_size);
   }
}

int tx_amaddr	(unsigned short		 pv_type,
		 struct	netbuf		*pv_buf,
		 struct t_partaddr	*pv_paddr,
		 int			 pv_pid)
{
   struct netbuf	*v_buf;
   tx_opttlv		*v_opt;
   int			 v_ind;

   if (p_init_cfg () == -1)
      return (-1);

   /*
    * map address mapping library
    */
   if ((v_ind = p_find_cfg (pv_type)) == -1 ||
       p_dll_path (v_ind) == -1)
      return (-1);

   v_opt = p_bui_opt (pv_pid, 0, sv_cfg[v_ind].opt);

   if ((v_buf = (*sv_am.paddr2netbuf) (pv_paddr, v_opt)) == NULL) {
      _t_error = T_WPARAMETER;
      return (-1);
   }

   if ((pv_buf->buf = malloc (v_buf->len)) == NULL) {
      _t_error = T_ENOMEM;
      return (-1);
   }

   pv_buf->len = v_buf->len;
   memcpy (pv_buf->buf, v_buf->buf, v_buf->len);

   return (0);
}

void tx_bui_paddr	(unsigned short		 pv_type,
			 struct t_partaddr	*pv_fromaddr,
			 struct netbuf		*pv_buf)
{
   struct t_partaddr    *v_paddr;
   int			 v_ind;

   if (p_init_cfg () == -1)
      return;

   /*
    * map address mapping library
    */
   if ((v_ind = p_find_cfg (pv_type)) == -1 ||
       p_dll_path (v_ind) == -1)
      return;

   if ((v_paddr = (*sv_am.netbuf2paddr) (pv_buf)) == NULL)
      return;

   memcpy (pv_fromaddr, v_paddr, v_paddr->t_palng);
}

/*
 * Position of PAD digit in a DTE address
 *
 * X.121 say nothing about leading or trailing pad digit,
 * nor if it have to be 0xf or a leading 0x0.
 *
 * tx_bcd_addr move in case of odd length addresses the
 * pad digit where you want it.
 *
 * pv_mode:
 *   0		TNS format, 0xf trailing
 *   PADM_LEAD	0xf as a leading pad digit
 *   PADM_ZERO  0x0 trailing, do not use this, gives wrong numbers
 *   PADM_LEAD | PADM_ZERO
 *		0x0 as a leading pad digit
 *
 * more then one 0xf pads will be reduced to none (by even numbers) or
 * one (by odd numbers).
 */
/*
 * the length of the address is usually 20 to 32 digits.
 */
#define	L_ADR	256
#define LM_ADR	0xff	/* mask to cut too long pv_len */
#define STD_PAD	0x0f

/*
 * pv_bcdin and pv_bcdout could be the same address,
 * working in place is allowed
 */
int tx_bcd_addr	(int		 pv_mode,
		 int		 pv_len,
		 unsigned char	*pv_bcdin,
		 unsigned char	*pv_bcdout)
{
   unsigned char	*v_ptr, *v_bufp, *v_adr_start;
   int			 v_len, v_rc;
   unsigned char	 v_buf [L_ADR + 4];

   pv_len &= LM_ADR;	/* cut too long adresses */

   memset (v_buf, 0x00, sizeof(v_buf));

   /* copy pv_bcdin into local buffer	*/
   v_ptr = pv_bcdin;
   v_bufp = v_buf + 1;

   while (pv_len--) {
      *v_bufp++ = (*v_ptr >> 4) & 0x0f; 
      *v_bufp++ = *v_ptr & 0x0f; 
      v_ptr++;
   }

   /* pad the address with one STD_PAD	*/
   *v_bufp = STD_PAD;

   /* normalize leading pads */
   v_bufp = v_buf;
   while (*v_bufp == 0x00)
      *v_bufp++ = STD_PAD;
      
   /* determin start of address	*/
   v_adr_start = v_buf;
   while (*v_adr_start == STD_PAD)
      v_adr_start++;

   /*
    * determin the real length of the address
    * there could be
    *	leading zeros or 0xf's (already normalized)
    *   the address
    *   trailing 0xf's
    */
   v_len = 0;
   v_bufp = v_adr_start;
   while (*v_bufp++ != STD_PAD)
     v_len++;

   /* odd numbers and leading pads! we start one before	*/
   if ((v_len & 1) != 0 && (pv_mode & PADM_LEAD) != 0)
      v_adr_start--;

   /* adjust len	*/
   v_rc = (v_len + 1) / 2;

   /* pad with zero or STD_PAD	*/
   if ((pv_mode & PADM_ZERO) != 0) {
      v_bufp = v_adr_start;
      v_len  = v_rc * 2;
      while (v_len--) {
         if (*v_bufp == STD_PAD)
            *v_bufp = 0;
         v_bufp++;
      }
   }

   v_ptr = pv_bcdout;
   v_len = v_rc;
   while (v_len--) {
      *v_ptr    = *v_adr_start++ << 4;
      *v_ptr++ |= *v_adr_start++;
   }
   
   return (v_rc);
}
