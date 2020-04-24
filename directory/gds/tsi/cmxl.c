/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxl.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:18  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:43  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:47:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:18  root]
 * 
 * Revision 1.1.2.6  1994/08/24  08:35:35  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:35:58  marrek]
 * 
 * Revision 1.1.2.5  1994/07/18  13:40:45  marrek
 * 	Changes for threadsafe libxti.
 * 	[1994/07/18  12:11:41  marrek]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:30  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:22:09  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:48:19  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:46:22  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:20  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:38  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:22  marrek
 * 	Bug fixes in April 1994 submission.
 * 	[1994/05/04  09:57:41  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:10  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxl.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:18 $";
#endif

/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG  All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1990
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
 * CMX TRACE Tool
 *
 * cmxl -- Preparing the ICMX(L) TRACE
 *
 * Call:
 * 	cmxl [-cdentvx] file ...
 *              c       Logon and connection phases
 *              d       Data phase
 *              e       Events
 *              n       DO NOT ignore incorrect entries
 *              t       t_error() calls
 *              v       full preparation
 *              x       restricted preparation
 *              file    file with TRACE
 *
 * cmxl reads the ICMX(L) TRACE in raw format from the file and
 * prepares it for printing in accordance with the options after stdout.
 *
 * Defines:
 * NOTYET       Specially for EVEN shorted TIDU/ETSDU logging
 */

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#if defined(_AIX)
#include <sys/select.h>
#endif
#include <sys/uio.h>
#include <xti.h>
#undef	T_CHECK
#undef	T_MORE
#include "cmx.h"
#define  CMXTRACE_XTI
#include "cmxtrace.h"
#include "cmxl.h"
#include "cxioctl.h"
/*
#include "cxmon.h"
*/
#include "sna_def.h"
#include <time.h>

#define MIN(A, B)	((A) > (B) ? (B) : (A) < 0 ? 0 : (A))

/*
static char sccsid[] = "@(#)cmxl.c	311.22 93/08/24 CMX (NC1)";
*/

/*****************************************************************************/
/*      D E C L A R A T I O N  G L O B A L  D A T A                           */
/*****************************************************************************/

#define TRUE	1
#define FALSE	0

extern char	*sys_errlist[];
extern int	sys_nerr;

static char	*(*hms)(short, short);
static char	*av0 = NULL;

static int	cflag, dflag, nodeb, eflag, nflag, tflag;
       long	strt;
       int	vflag;
static int      fdsetsize=128; /* default value according to CMX<=V3.0 */
static int      msgsize=109;   /* default value according to CMX<=V3.0 */
static char	Xflag;
static char	vers;
static int	filter = FALSE ;


extern char	*optarg;
extern int	optind, opterr;

/*****************************************************************************/
/*      D E C L A R A T I O N  I N T E R N A L  D A T A                      */
/*****************************************************************************/

#ifdef INTLM
#include <nl_types.h>
#define CMXTOOLCAT	"cmxcmd"
nl_catd	catopen();
char	*catgets();
static nl_catd	catd = (nl_catd) -1;
#else
#undef catgets
#define catgets(a,b,c,d) d
#endif

static void t_printhead(char * file);
static void expand(char	*file);
static void plu (char *n, unsigned char *b);

static void XtiprPtr	(void *pv_ptr, char *pv_txt);
static void XtiprBind	(void *pv_ptr, struct t_bind *pv_bind, char *pv_txt);
static void XtiprCall	(void *pv_ptr, struct t_call *pv_call, char *pv_txt, int pv_flag);
static void XtiFd	(char *pv_txt);
static void XtiFin	(char *pv_txt);
static void XtiprNbuf	(void *pv_ptr, struct netbuf *pv_nb, char *pv_txt);
static void XtirxdBuf	(void *pv_ptr, struct netbuf *pv_nb, char *pv_txt);
static void XtiCall	(char *pv_fkt, int pv_flag);
static void XtiCallfin	(char *pv_fkt, char *pv_str, int pv_flag);
#define	FLAG_SEQUENCE	0x01
static void XtiprRet	(struct	xti_ret	*xr);

/*---------------------------------------------------------------------------*
 *      M A I N   P R O G R A M
 *---------------------------------------------------------------------------*/

int main(int argc, char **argv);
void rqflush (void);
/*
 * main - in loop preparation of TRACE files from CLI
 */
main(int argc, char **argv)
{
	register int	i;
	int	res;
	char	*file;

	av0 = "cmxl";
	opterr = 0;
	res = 0;
	cflag = dflag = eflag = nflag = tflag = FALSE;
	vflag = FALSE;          /* full preparation: no */
	nodeb = TRUE;           /* ignore DEBUG entries: yes */

	setlocale(LC_ALL, "");
#ifdef INTLM
	catd = catopen(CMXTOOLCAT, 0);  /* Close tut exit() */
#endif

	/*
	 * Option analysis
	 */
	while ((i = getopt(argc, argv, "cdDentvxX")) != EOF) {

		switch (i) {
		case 'c':
			cflag++;
			break;

		case 'd':
			dflag++;
			break;

		case 'D':
			nodeb = FALSE;
			Xflag = TRUE;
			break;

		case 'e':
			eflag++;
			break;

		case 'n':
			nflag++;
			break;

		case 't':
			tflag++;
			break;

		case 'v':
			vflag = TRUE;
			break;

		case 'x':
			vflag = FALSE;
			break;

		case 'X':
			Xflag = TRUE;
			break;

		case '?':
			res++;
		}
	}

	if ((cflag == 0) && (dflag == 0) && (eflag == 0))
		cflag = eflag = dflag = TRUE;

	if (argc == optind || res) {
		if (res) {
			fprintf(stderr, USAGE, av0);
			exit(1);
			/* NOTREACHED */
		}
		/* program is used as a filter */
		filter = TRUE ;
		setbuf (stdin, NULL) ;
		setbuf (stdout, NULL) ;
		hms = hms30 ;
		expand ("") ;
		exit (0) ;
	}

	/* file ... prepare */
	while ((file = argv[optind++]) != NULL) {
		if (freopen(file, "r", stdin) == NULL) {
			fprintf(stderr, EOPEN, av0, errno, file);
			exit(1);
			/* NOTREACHED */
		}
		expand(file);
	}
	exit(0);
}

static	char			*cp;
static	int			i;
static	int			t_trclen;
static	struct t_trcentry	tl;
static	struct t_data		vp[T_VCNT];
static	char			buf[BUFSIZ];
static	char			strbuf [80];

void rqflush (void) {
    if (filter) {
	/* print filter operation termination character */
	fputc ('\0', stdout) ;
    }
}

/*
 * expand - preparation of stdin TRACE in accordance with stdout
 */
static void expand(char	*file)
{

	for (; fread (&tl, T_LHEADER, 1, stdin); rqflush ()) {
		switch (tl.id) {

		case T_FFIRST: { /* V2.2 */
			fprintf(stderr, EV22ENT, av0, tl.id);
			exit(1);
			}
		case T_LIBV25:
		case T_LIBV30:
		case T_LIBV30B: {
			fread (&tl.t_ffirst, sizeof (tl.t_ffirst), 1, stdin);
			vers = '?';
			t_printhead(file);
			break;
			}
		case T_LIBV40:   /* V4.0 */ {
		 	fread (&tl.t_ffirst2, sizeof (tl.t_ffirst2), 1, stdin);
			vers = tl.t_ffirst2.version;
			fdsetsize = tl.t_ffirst2.fdsetsize;
			msgsize = tl.t_ffirst2.msgsize;
			t_printhead(file);
			break;
			}
		case T_DATTACH:
		case T_DDETACH:
			if (nodeb)
				continue;
			printf("%s\t", hms(tl.relsec, tl.milsec));
			prtxt(kops, tl.info);
			(void) putchar('\n');
			break;

		case T_FATTACH:
			fread (&tl.t_fattach, sizeof (tl.t_fattach), 1, stdin);

			if (tl.t_fattach.opt) {
				printf("%s t_attach(0x%x, 0x%x)\n",
					hms(tl.relsec, tl.milsec), tl.t_fattach.name,
					tl.t_fattach.opt);
				propta1(&(tl.t_fattach.t_optnr));
			} else
				printf("%s t_attach(0x%x, NULL)\n",
					hms(tl.relsec, tl.milsec), tl.t_fattach.name);

			printf("\t   name: ");
			prmn(&(tl.t_fattach.t_mn));
			prx(&(tl.t_fattach.t_mn),
				MIN(tl.t_fattach.t_mn.t_mnlng,
					sizeof (struct t_myname)), 0, putline);
			break;

		case T_FRETATT: {

			int i;

			fread (&tl.t_fretatt, sizeof (tl.t_fretatt), 1, stdin);
			if ((tl.t_fretatt.t_optnr == T_OPTA2) ||
				(tl.t_fretatt.t_optnr == T_OPTA5) ||
				(tl.t_fretatt.t_optnr == T_OPTA6)) {
				printf("\t\tt_uattid 0x%x t_attid 0x%x t_sptypes 0x%x ",
					tl.t_fretatt.t_uattid,
					tl.t_fretatt.t_attid,
					tl.t_fretatt.t_sptypes);
					printf("\n");
				if (tl.t_fretatt.t_optnr == T_OPTA6) {
					printf("\t\tt_cclist (%#x): %d ",
						tl.t_fretatt.t_ccbits,
						tl.t_fretatt.t_cc.cl_num);
					if (tl.t_fretatt.t_cc.cl_num != CMX_ALLCCIDS) {
						for (i=0; i < (int)tl.t_fretatt.t_cc.cl_num; i++)
							printf("%x ",(int)tl.t_fretatt.t_cc.cl_id[i]);
					}
					printf("\n");
				}
				else
				printf("\t\tt_ccbits %#x\n",tl.t_fretatt.t_ccbits);
			}
			printf("%s   ", hms(tl.relsec, tl.milsec));

			if (tl.t_fretatt.retval == T_OK)
				printf("T_OK (%d)\n", T_OK);
			else if (tl.t_fretatt.retval == T_NOTFIRST)
				printf("T_NOTFIRST (%d)\n", T_NOTFIRST);
			else
				printf("? (0x%x)\n", tl.t_fretatt.retval);
			break;
			}
		case T_FDETACH:
			fread (&tl.t_fdetach, sizeof (tl.t_fdetach), 1, stdin);

			printf("%s t_detach(0x%x)\n", hms(tl.relsec, tl.milsec),
				tl.t_fdetach.name);

			printf("\t   name: ");
			prmn(&(tl.t_fdetach.t_mn));
			prx(&(tl.t_fdetach.t_mn),
				MIN(tl.t_fdetach.t_mn.t_mnlng,
					sizeof (struct t_myname)), 0, putline);
			break;

		case T_FCONRQ:
			fread (&tl.t_fconrq, sizeof (tl.t_fconrq), 1, stdin);
			if (!cflag)
				continue;

			if (tl.t_fconrq.opt) {
				printf("%s t_conrq(0x%x, 0x%x, 0x%x, 0x%x)\n",
					hms(tl.relsec, tl.milsec), tl.t_fconrq.tref,
					tl.t_fconrq.toaddr, tl.t_fconrq.fromaddr,
					tl.t_fconrq.opt);
				proptc1(tl.id, &(tl.t_fconrq.t_optnr),
					0, tl.t_fconrq.udata);
			} else
				printf("%s t_conrq(0x%x, 0x%x, 0x%x, NULL)\n",
					hms(tl.relsec, tl.milsec), tl.t_fconrq.tref,
					tl.t_fconrq.toaddr, tl.t_fconrq.fromaddr);

			printf("\t   toaddr: ");
			praddr(&tl.t_fconrq.t_pa);
			prx(&(tl.t_fconrq.t_pa),
				MIN(tl.t_fconrq.t_pa.t_palng,
					sizeof (struct t_partaddr)), 0, putline);

			printf("\t   fromaddr: ");
			prmn(&tl.t_fconrq.t_mn);
			prx(&(tl.t_fconrq.t_mn),
				MIN(tl.t_fconrq.t_mn.t_mnlng,
					sizeof (struct t_myname)), 0, putline);
			break;

		case T_FCONCF:
			fread (&tl.t_fconcf, sizeof (tl.t_fconcf), 1, stdin);
			if (!cflag)
				continue;

			if (tl.info == T_TRSTART && nodeb)
				/* Starteintraege nur bei Option -D */
				continue;

			if (tl.t_fconcf.opt) {
				printf("%s t_concf(0x%x, 0x%x)\n",
					hms(tl.relsec, tl.milsec), tl.t_fconcf.tref, tl.t_fconcf.opt);
				proptc1(tl.id, &(tl.t_fconcf.t_optnr),
					tl.t_fconcf.udatal, tl.t_fconcf.udata);
			} else
				printf("%s t_concf(0x%x, NULL)\n",
					hms(tl.relsec, tl.milsec), tl.t_fconcf.tref);
			break;

		case T_FCONIN:
			fread (&tl.t_fconin, sizeof (tl.t_fconin), 1, stdin);
			if (!cflag)
				continue;

			if (tl.info == T_TRSTART && nodeb)
				/* Start entries only with option -D */
				continue;

			if (tl.info == T_TRSTART) {
				printf("%s t_conin(0x%x, ---, ---, NULL)\n",
					hms(tl.relsec, tl.milsec));
				continue;
				}
			if (tl.t_fconin.opt) {
				printf("%s t_conin(0x%x, 0x%x, 0x%x, 0x%x)\n",
					hms(tl.relsec, tl.milsec), tl.t_fconin.tref,
					tl.t_fconin.toaddr, tl.t_fconin.fromaddr,
					tl.t_fconin.opt);
				proptc1(tl.id, &(tl.t_fconin.t_optnr),
					tl.t_fconin.udatal, tl.t_fconin.udata);
			} else
				printf("%s t_conin(0x%x, 0x%x, 0x%x, NULL)\n",
					hms(tl.relsec, tl.milsec), tl.t_fconin.tref,
					tl.t_fconin.toaddr, tl.t_fconin.fromaddr);

			printf("\t   toaddr: ");
			prmn(&tl.t_fconin.t_mn);
			prx(&(tl.t_fconin.t_mn),
				MIN(tl.t_fconin.t_mn.t_mnlng, sizeof
					(struct t_myname)), 0, putline);

			printf("\t   fromaddr: ");
			praddr(&tl.t_fconin.t_pa);
			prx(&(tl.t_fconin.t_pa),
				MIN(tl.t_fconin.t_pa.t_palng,
					sizeof (struct t_partaddr)), 0, putline);
			break;

		case T_FCONRS:
			fread (&tl.t_fconrs, sizeof (tl.t_fconrs), 1, stdin);
			if (!cflag)
				continue;

			if (tl.t_fconrs.opt) {
				printf("%s t_conrs(0x%x, 0x%x)\n",
					hms(tl.relsec, tl.milsec), tl.t_fconrs.tref, tl.t_fconrs.opt);
				proptc1(tl.id, &(tl.t_fconrs.t_optnr),
					0, tl.t_fconrs.udata);
			} else
				printf("%s t_conrs(0x%x, NULL)\n",
					hms(tl.relsec, tl.milsec), tl.t_fconrs.tref);
			break;

		case T_FDISRQ:
			fread (&tl.t_fdisrq, sizeof (tl.t_fdisrq), 1, stdin);
			if (!cflag)
				continue;

			if (tl.t_fdisrq.opt) {
				printf("%s t_disrq(0x%x, 0x%x)\n",
					hms(tl.relsec, tl.milsec), tl.t_fdisrq.tref, tl.t_fdisrq.opt);
				proptc1(tl.id, &(tl.t_fdisrq.t_optnr),
					0, tl.t_fdisrq.udata);
			} else
				printf("%s t_disrq(0x%x, NULL)\n",
					hms(tl.relsec, tl.milsec), tl.t_fdisrq.tref);
			break;

		case T_FDISIN:
			fread (&tl.t_fdisin, sizeof (tl.t_fdisin), 1, stdin);
			if (!cflag)
				continue;

			if (tl.info == T_TRSTART && nodeb)
				/* Start entries only with option -D */
				continue;

			printf("%s t_disin(0x%x, ",
				hms(tl.relsec, tl.milsec), tl.t_fdisin.tref);

			if (tl.info != T_TRSTART)
				prtxt(reasons, tl.t_fdisin.reason);
			else
				printf("(--)");

			if (tl.t_fdisin.opt) {
				printf(", 0x%x)\n", tl.t_fdisin.opt);
				if (tl.info != T_TRSTART)
					proptc1(tl.id, &(tl.t_fdisin.t_optnr),
					tl.t_fdisin.udatal, tl.t_fdisin.udata);
			} else
				printf(", NULL)\n");
			break;

		case T_FREDRQ:
			fread (&tl.t_fredrq, sizeof (tl.t_fredrq), 1, stdin);
			if (!cflag)
				continue;

			if (tl.t_fredrq.opt) {
				printf("%s t_redrq(0x%x, %d, 0x%x)\n",
					hms(tl.relsec, tl.milsec), tl.t_fredrq.tref,
					tl.t_fredrq.pid, tl.t_fredrq.opt);
				proptc1(tl.id, &(tl.t_fredrq.t_optnr),
					0, tl.t_fredrq.udata);
			} else
				printf("%s t_redrq(0x%x, %d, NULL)\n",
					hms(tl.relsec, tl.milsec), tl.t_fredrq.tref, tl.t_fredrq.pid);
			break;

		case T_FREDIN:
			fread (&tl.t_fredin, sizeof (tl.t_fredin), 1, stdin);
			if (!cflag)
				continue;

			if (tl.info == T_TRSTART && nodeb)
				/* Start entries only with option -D */
				continue;

			if (tl.t_fredin.opt && tl.info != T_TRSTART) {
				printf("%s t_redin(0x%x, %d, 0x%x)\n",
					hms(tl.relsec, tl.milsec), tl.t_fredin.tref,
					tl.t_fredin.pid, tl.t_fredin.opt);
				proptc1(tl.id, &(tl.t_fredin.t_optnr),
					tl.t_fredin.udatal, tl.t_fredin.udata);
			} else
				printf("%s t_redin(0x%x, %d, NULL)\n",
					hms(tl.relsec, tl.milsec), tl.t_fredin.tref, tl.t_fredin.pid);
			break;

		case T_FDATARQ:
			fread(&tl.t_fdatarq, sizeof (tl.t_fdatarq), 1, stdin);
			if (tl.info != T_TRSTART)
				fread(buf, MIN(sizeof (buf), tl.t_fdatarq.datacnt), 1, stdin);
			if (!dflag)
				continue;

			printf("%s t_datarq(0x%x, 0x%x, %d, ",
				hms(tl.relsec, tl.milsec), tl.t_fdatarq.tref,
				tl.t_fdatarq.datap, tl.t_fdatarq.datal);

			if (tl.t_fdatarq.chain == T_END)
				printf("T_END)\n");
			else if (tl.t_fdatarq.chain == T_MORE)
				printf("T_MORE)\n");
			else
				printf("? (0x%x))\n", tl.t_fdatarq.chain);

			if (vflag && tl.t_fdatarq.datacnt &&tl.info != T_TRSTART) {
				printf("\t   datap:\n");
				prx(buf, MIN(sizeof (buf), tl.t_fdatarq.datacnt),
					0, putline);
			}
			break;

		case T_FVDATARQ:
			fread(&tl.t_fvdatarq, sizeof (tl.t_fvdatarq), 1, stdin);

			if (!dflag && tl.info != T_TRSTART) {
				for (i = 0; i < MIN(tl.t_fvdatarq.vcnt, T_VCNT); i++) {
					fread((char *) &vp[i],
						sizeof (struct t_data), 1, stdin);
					fread(buf, MIN(vp[i].t_datal, t_trclen),
						1, stdin);
				}
				continue;
			}

			printf("%s t_vdatarq(0x%x, 0x%x, %d, ",
				hms(tl.relsec, tl.milsec), tl.t_fvdatarq.tref,
				tl.t_fvdatarq.vdata, tl.t_fvdatarq.vcnt);

			if (tl.info == T_TRSTART) {
				printf ("---)\n");
				break; /* keine weiteren Ausgaben bei Starteintrag */
			}
			if (tl.t_fvdatarq.chain == T_END)
				printf("T_END)\n");
			else if (tl.t_fvdatarq.chain == T_MORE)
				printf("T_MORE)\n");
			else
				printf("? (0x%x))\n", tl.t_fvdatarq.chain);

			for (i = 0; i < MIN(tl.t_fvdatarq.vcnt, T_VCNT); i++) {
				fread((char *) &vp[i],
					sizeof (struct t_data), 1, stdin);
				printf("\t   vdata[%d].t_datap 0x%x  vdata[%d].t_datal %d\n",
					i, vp[i].t_datap, i, vp[i].t_datal);
				fread(buf, MIN(vp[i].t_datal, t_trclen),
					1, stdin);
				if (vflag)
					prx(buf, MIN(vp[i].t_datal, t_trclen),
						0, putline);
			}
			break;
#ifdef NOTYET
		case T_FDATARQS:
			fread (&tl.t_fdatarqs, sizeof (tl.t_fdatarqs), 1, stdin);
			if (!dflag)
				continue;

			printf("%s t_datarq(0x%x, 0x%x, %d, ",
				hms(tl.relsec, tl.milsec), tl.t_fdatarqs.tref,
				tl.t_fdatarqs.datap, tl.t_fdatarqs.datal);

			if (tl.t_fdatarqs.chain == T_END)
				printf("T_END)\n");
			else if (tl.t_fdatarqs.chain == T_MORE)
				printf("T_MORE)\n");
			else
				printf("? (0x%x))\n", tl.t_fdatarqs.chain);
			break;
#endif
		case T_FXDATRQ:
			fread (&tl.t_fxdatrq, sizeof (tl.t_fxdatrq), 1, stdin);
			if (!dflag)
				continue;

			printf("%s t_xdatrq(0x%x, 0x%x, %d)\n",
				hms(tl.relsec, tl.milsec), tl.t_fxdatrq.tref,
				tl.t_fxdatrq.datap, tl.t_fxdatrq.datal);

			if (vflag && tl.t_fxdatrq.datal) {
				printf("\t   datap:\n");
				prx(tl.t_fxdatrq.data, MIN(T_EXP_SIZE, tl.t_fxdatrq.datal),
					0, putline);
			}
			break;
#ifdef NOTYET
		case T_FXDATRQS:
			fread (&tl.t_fxdatrqs, sizeof (tl.t_fxdatrqs), 1, stdin);
			if (!dflag)
				continue;

			printf("%s t_xdatrq(0x%x, 0x%x, %d)\n",
				hms(tl.relsec, tl.milsec), tl.t_fxdatrqs.tref,
				tl.t_fxdatrqs.datap, tl.t_fxdatrqs.datal);
			break;
#endif
		case T_FDATAIN:
			fread (&tl.t_fdatain, sizeof (tl.t_fdatain), 1, stdin);
			if (tl.info == T_TRFLSH)
				fread(buf, MIN(sizeof (buf), tl.t_fdatain.datacnt),
					1, stdin);
			if (!dflag)
				continue;

			if (tl.info == T_TRSTART && nodeb)
				/* Start entries only with option -D */
				continue;

			printf("%s t_datain(0x%x, 0x%x, ", hms(tl.relsec, tl.milsec),
				tl.t_fdatain.tref, tl.t_fdatain.datap);
			if (tl.info == T_TRNFLSH)
				printf("%d><%d, ",
					tl.t_fdatain.datal, tl.t_fdatain.datal);
			else
				if (tl.info == T_TRSTART) {
					printf("%d>---, ---)\n",
						(tl.t_fdatain.datal >> 16) & 0xFFFF);
					break; /* No further output */
				}
				else
					printf("%d><%d, ",
						(tl.t_fdatain.datal >> 16) & 0xFFFF,
						tl.t_fdatain.datal & 0xFFFF);

			if (tl.t_fdatain.chain == T_END)
				printf("T_END)\n");
			else if (tl.t_fdatain.chain == T_MORE)
				printf("T_MORE)\n");
			else
				printf("? (0x%x))\n", tl.t_fdatain.chain);

			if (vflag && tl.info == T_TRFLSH && tl.t_fdatain.datacnt) {
				printf("\t   datap:\n");
				prx(buf, MIN(sizeof (buf), tl.t_fdatain.datacnt),
					0, putline);
			}
			break;

		case T_FVDATAIN:
			fread(&tl.t_fvdatain, sizeof (tl.t_fvdatain), 1, stdin);

			if (tl.info == T_TRSTART && nodeb)
				/* Start entries only with option -D */
				continue;

			if (!dflag && tl.info != T_TRSTART) {
				for (i = 0; i < MIN(tl.t_fvdatain.vcnt, T_VCNT); i++) {
					fread((char *) &vp[i],
						sizeof (struct t_data), 1, stdin);
					if (tl.info != T_TRFLSH)
						continue;
					fread(buf, MIN(vp[i].t_datal & 0xFFFF, t_trclen),
						1, stdin);
				}
				continue;
			}

			printf("%s t_vdatain(0x%x, 0x%x, %d, ",
				hms(tl.relsec, tl.milsec), tl.t_fvdatain.tref,
				tl.t_fvdatain.vdata, tl.t_fvdatain.vcnt);

			if (tl.info == T_TRSTART) {
				printf("---)\n");
				break; /* Keine weitere Augabe */
			}

			if (tl.t_fvdatain.chain == T_END)
				printf("T_END)\n");
			else if (tl.t_fvdatain.chain == T_MORE)
				printf("T_MORE)\n");
			else
				printf("? (0x%x))\n", tl.t_fvdatain.chain);

			for (i = 0; i < MIN(tl.t_fvdatain.vcnt, T_VCNT); i++) {
				fread((char *) &vp[i], sizeof (struct t_data),
					1, stdin);
				printf("\t   vdata[%d].t_datap 0x%x  vdata[%d].t_datal ",
					i, vp[i].t_datap, i);
				if (tl.info == T_TRNFLSH) {
					printf("%d><%d\n",
						vp[i].t_datal, vp[i].t_datal);
					continue;
				}
				printf("%d><%d\n",
					(vp[i].t_datal >> 16) & 0xFFFF,
					vp[i].t_datal & 0xFFFF);
				fread(buf, MIN(vp[i].t_datal & 0xFFFF, t_trclen),
					1, stdin);
				if (vflag)
					prx(buf, MIN(vp[i].t_datal & 0xFFFF, t_trclen),
						0, putline);
			}
			break;
#ifdef NOTYET
		case T_FDATAINS:
			fread (&tl.t_fdatains, sizeof (tl.t_fdatains), 1, stdin);
			if (!dflag)
				continue;

			printf("%s t_datain(0x%x, 0x%x, %d, ",
				hms(tl.relsec, tl.milsec), tl.t_fdatains.tref,
				tl.t_fdatains.datap, tl.t_fdatains.datal);

			if (tl.t_fdatains.chain == T_END)
				printf("T_END)\n");
			else if (tl.t_fdatains.chain == T_MORE)
				printf("T_MORE)\n");
			else
				printf("? (0x%x))\n", tl.t_fdatains.chain);
			break;
#endif
		case T_FXDATIN:
			fread (&tl.t_fxdatin, sizeof (tl.t_fxdatin), 1, stdin);
			if (!dflag)
				continue;

			if (tl.info == T_TRSTART && nodeb)
				/* Start entries only with option -D */
				continue;

			printf("%s t_xdatin(0x%x, 0x%x, ",
				hms(tl.relsec, tl.milsec), tl.t_fxdatin.tref,
				tl.t_fxdatin.datap);
			if (tl.info == T_TRNFLSH)
				printf("%d><%d)\n",
					tl.t_fxdatin.datal, tl.t_fxdatin.datal);
			else
				if (tl.info == T_TRSTART) {
					printf("%d>---, ---)\n",
						(tl.t_fxdatin.datal >> 16) & 0xFFFF);
					break; /* No furhter output */
				}
				else
					printf("%d><%d)\n",
						(tl.t_fxdatin.datal >> 16) & 0xFFFF,
						tl.t_fxdatin.datal & 0xFFFF);
			tl.t_fxdatin.datal &= 0xFFFF;

			if (vflag && tl.t_fxdatin.datal) {
				printf("\t   datap:\n");
				prx(tl.t_fxdatin.data,
					MIN(T_EXP_SIZE, tl.t_fxdatin.datal),
					0, putline);
			}
			break;
#ifdef NOTYET
		case T_FXDATINS:
			fread (&tl.t_fxdatins, sizeof (tl.t_fxdatins), 1, stdin);
			if (!dflag)
				continue;

			printf("%s t_xdatin(0x%x, 0x%x, %d)\n",
				hms(tl.relsec, tl.milsec), tl.t_fxdatins.tref,
				tl.t_fxdatins.datap, tl.t_fxdatins.datal);
			break;
#endif
		case T_FDATAGO:
		case T_FDATASTOP:
		case T_FXDATGO:
		case T_FXDATSTOP:
			fread (&tl.t_fflow, sizeof (tl.t_fflow), 1, stdin);
			if (!dflag)
				continue;

			if (tl.id == T_FDATAGO)
				cp = "t_datago";
			if (tl.id == T_FDATASTOP)
				cp = "t_datastop";
			if (tl.id == T_FXDATGO)
				cp = "t_xdatgo";
			if (tl.id == T_FXDATSTOP)
				cp = "t_xdatstop";
			printf("%s %s(0x%x)\n", hms(tl.relsec, tl.milsec), cp, tl.t_fflow.tref);
			break;

		case T_FERROR:
			if (!tflag)
				continue;

			printf("%s t_error()\t%s (%d) ############################\n",
				hms(tl.relsec, tl.milsec), prerr(tl.info), tl.info);
			break;

		case T_DEVENT:
			fread (&tl.t_devent, sizeof (tl.t_devent), 1, stdin);
			if (nodeb || !eflag)
				continue;

			printf("%s\t", hms(tl.relsec, tl.milsec));
			prtxt(kops, tl.info);
			if (tl.t_devent.cmode == T_WAIT)
				printf(": T_WAIT (%d)", T_WAIT);
			else if (tl.t_devent.cmode == T_CHECK)
				printf(": T_CHECK (%d)", T_CHECK);
			else
				printf(": ? (0x%x)", tl.t_devent.cmode);
			(void) putchar('\n');
			break;

		case T_FEVENT:
			fread (&tl.t_fevent, sizeof (tl.t_fevent), 1, stdin);
			if (!eflag)
				continue;

			printf("%s t_event(0x%x, ", hms(tl.relsec, tl.milsec),
				tl.t_fevent.tref);
			if (tl.t_fevent.cmode == T_WAIT)
				printf("T_WAIT");
			else if (tl.t_fevent.cmode == T_CHECK)
				printf("T_CHECK");
			else
				printf("? (0x%x)", tl.t_fevent.cmode);
			if (tl.t_fevent.opt) {
				printf(", 0x%x)\n", tl.t_fevent.opt);
				propte1(&(tl.t_fevent.t_optnr));
			} else
				printf(", NULL)\n");
			break;

		case T_FINFO:
		case T_FINFO3:
			if (tl.id == T_FINFO)
				fread (&tl.t_finfo, sizeof (tl.t_finfo), 1, stdin);
			else
				fread (&tl.t_finfo3, sizeof (tl.t_finfo3), 1, stdin);
			if (!dflag)
				continue;

			if (tl.info == T_TRSTART && nodeb)
				/* Start entries only with option -D */
				continue;

			if (tl.t_finfo.opt && tl.info != T_TRSTART) {
				printf("%s t_info(0x%x, 0x%x)\n",
					hms(tl.relsec, tl.milsec), tl.t_finfo.tref, tl.t_finfo.opt);
				if (tl.id == T_FINFO)
					propti1(&(tl.t_finfo.t_optnr));
				else
					propti3(&(tl.t_finfo3.t_optnr));
			} else {
				printf("%s t_info(0x%x",
					hms(tl.relsec, tl.milsec), tl.t_finfo.tref);
				if (tl.info == T_TRSTART)
					printf(", --- )\n");
				else
					printf(", NULL)\n");
			}
			break;

		case T_FGETLOC:
			fread(&tl.t_fgetloc, sizeof (tl.t_fgetloc), 1, stdin);
			fread(buf, MIN(sizeof (buf), tl.t_fgetloc.lglob), 1, stdin);

			if (tl.t_fgetloc.opt) {
				printf("%s t_getloc(0x%x, ? (0x%x)\n",
					hms(tl.relsec, tl.milsec), tl.t_fgetloc.glob, tl.t_fgetloc.opt);
			} else
				printf("%s t_getloc(0x%x, NULL)\n",
					hms(tl.relsec, tl.milsec), tl.t_fgetloc.glob);

			printf("\t   glob:\n");
			prx(buf, MIN(sizeof (buf), tl.t_fgetloc.lglob), 0, putline);

			if (tl.t_fgetloc.loc) {
				printf("\t   loc 0x%x: ", tl.t_fgetloc.loc);
				prmn(&tl.t_fgetloc.t_mn);
				if (vflag)
					prx(&(tl.t_fgetloc.t_mn),
						MIN(tl.t_fgetloc.t_mn.t_mnlng,
							sizeof (struct t_myname)),
						0, putline);
			}
			break;

		case T_FGETADDR:
			fread(&tl.t_fgetaddr, sizeof (tl.t_fgetaddr), 1, stdin);
			fread(buf, MIN(sizeof (buf), tl.t_fgetaddr.lglob), 1, stdin);

			if (tl.t_fgetaddr.opt) {
				printf("%s t_getaddr(0x%x, ? (0x%x))\n",
					hms(tl.relsec, tl.milsec), tl.t_fgetaddr.glob, tl.t_fgetaddr.opt);
			} else
				printf("%s t_getaddr(0x%x, NULL)\n",
					hms(tl.relsec, tl.milsec), tl.t_fgetaddr.glob);

			printf("\t   glob:\n");
			prx(buf, MIN(sizeof (buf), tl.t_fgetaddr.lglob), 0, putline);

			if (tl.t_fgetaddr.addr) {
				printf("\t   addr 0x%x: ", tl.t_fgetaddr.addr);
				praddr(&tl.t_fgetaddr.t_pa);
				if (vflag)
					prx(&(tl.t_fgetaddr.t_pa),
						MIN(tl.t_fgetaddr.t_pa.t_palng,
							sizeof (struct t_partaddr)),
						0, putline);
			}
			break;

		case T_FGETNAME:
			fread(&tl.t_fgetname, sizeof (tl.t_fgetname), 1, stdin);

			if (tl.t_fgetname.opt) {
				printf("%s t_getname(0x%x, ? (0x%x))\n",
					hms(tl.relsec, tl.milsec), tl.t_fgetname.addr, tl.t_fgetname.opt);
			} else
				printf("%s t_getname(0x%x, NULL)\n",
					hms(tl.relsec, tl.milsec), tl.t_fgetname.addr);

			printf("\t   addr: ");
			praddr(&tl.t_fgetname.t_pa);
			prx(&(tl.t_fgetname.t_pa),
				MIN(tl.t_fgetname.t_pa.t_palng,
					sizeof (struct t_partaddr)),
				0, putline);

			if (tl.t_fgetname.name) {
				fread(buf, MIN(sizeof (buf), tl.t_fgetname.lname),
					1, stdin);
				printf("\t   glob:\n", tl.t_fgetname.name);
				if (vflag)
					prx(buf, MIN(sizeof (buf), tl.t_fgetname.lname),
						0, putline);
			}
			break;

		case T_FSETOPT:
			fread (&tl.t_fsetopt, sizeof (tl.t_fsetopt), 1, stdin);

			if (tl.t_fsetopt.component == T_LIB)
				printf("%s t_setopt(T_LIB, ",hms(tl.relsec, tl.milsec));
			else
				printf("%s t_setopt(0x%x, ",hms(tl.relsec, tl.milsec));

			if (tl.t_fsetopt.opt) {
				printf("0x%x)\n",tl.t_fsetopt.opt);
				propts1(&(tl.t_fsetopt.t_optnr));
			} else
				printf("NULL)\n");

			break;

		case T_FCALLBACK:
			fread (&tl.t_fcallback, sizeof (tl.t_fcallback), 1, stdin);
			if (tl.info == 0) {
				if (tl.t_fcallback.opt) {
					printf("%s t_fcallback(0x%x, 0x%x, 0x%x)\n",
						hms(tl.relsec, tl.milsec), tl.t_fcallback.routine, tl.t_fcallback.usr, tl.t_fcallback.opt);
				} else
				printf("%s t_callback(0x%x, 0x%x, NULL)\n",
						hms(tl.relsec, tl.milsec), tl.t_fcallback.routine, tl.t_fcallback.usr);
			}
			else
				printf("%s   old callback routine was: 0x%x\n",
						hms(tl.relsec, tl.milsec), tl.t_fcallback.routine);
			break;

		/*------------------------------------------------------------*
		 * Logging of call events here
		 *------------------------------------------------------------*/

		case T_DRETERR:
		case T_FRETERR:
			fread(&tl.t_freterr, sizeof (tl.t_freterr), 1, stdin);
			if (nodeb && tl.id == T_DRETERR)
				continue;
			printf("%s   ", hms(tl.relsec, tl.milsec));
			if (tl.id == T_DRETERR)
				(void) putchar('\t');
			printf("T_ERROR %s (%d) ############################\n",
				prerr(tl.t_freterr.error), tl.t_freterr.error);
			break;

		case T_DRETOK:
		case T_FRETOK:
			fread(&tl.t_fretok, sizeof (tl.t_fretok), 1, stdin);
			if (nodeb && tl.id == T_DRETOK)
				continue;

			switch (tl.info) {
			case T_FATTACH:
				printf("%s   ", hms(tl.relsec, tl.milsec));
				if (tl.id == T_DRETOK)
					(void) putchar('\t');
				if (tl.t_fretok.retval == T_OK)
					printf("T_OK (%d)\n", T_OK);
				else if (tl.t_fretok.retval == T_NOTFIRST)
					printf("T_NOTFIRST (%d)\n", T_NOTFIRST);
				else
					printf("? (0x%x)\n", tl.t_fretok.retval);
				break;

			case T_FCONRQ:
				if (cflag) {
					printf("%s   tref 0x%x\n", hms(tl.relsec, tl.milsec),
						tl.t_fretok.tref);
				}
				printf("\t    T_OK (%d)\n", T_OK);
				break;

			case T_FDATARQ:
			case T_FVDATARQ:
			case T_FXDATRQ:
				if (dflag) { 
				printf("%s   ", hms(tl.relsec, tl.milsec));
				if (tl.t_fretok.retval == T_DATASTOP)
					printf("T_DATASTOP (%d)\n", T_DATASTOP);
				else if (tl.t_fretok.retval == T_XDATSTOP)
					printf("T_XDATSTOP (%d)\n", T_XDATSTOP);
				else if (tl.t_fretok.retval == T_OK)
					printf("T_OK (%d)\n", T_OK);
				else
					printf("? (0x%x)\n", tl.t_fretok.retval);
				}


				break;

			case T_FDATAIN:
			case T_FVDATAIN:
			case T_FXDATIN:
				if (dflag)
					if (tl.t_fretok.retval == T_OK) {
						printf("%s   ", hms(tl.relsec, tl.milsec));
						(void) putchar('\t');
						printf("T_OK (%d)\n", T_OK);
					}
					else
						printf("%s   %d\n", hms(tl.relsec, tl.milsec), tl.t_fretok.retval);

				break;

			case T_FEVENT:
				if (!eflag)
					break;
				printf("%s   ", hms(tl.relsec, tl.milsec));
				prevt(tl.t_fretok.retval, tl.t_fretok.tref);
				break;
			default:
				printf("%s   ", hms(tl.relsec, tl.milsec));
				printf("T_OK (%d)\n", T_OK);
				break;
			}
			break;

		case T_FRETEVT:
			fread(&tl.t_fevent, sizeof (tl.t_fevent), 1, stdin);
			if (!eflag)
				break;

			printf("%s   ", hms(tl.relsec, tl.milsec));
			prevt(tl.t_fevent.cmode, tl.t_fevent.tref);
			if (vflag) {
				printf("\t\tt_attid 0x%x  t_uattid 0x%x\n\t\tt_ucepid 0x%x  t_evdat 0x%x (%d)\n",
					tl.t_fevent.t_attid, tl.t_fevent.t_uattid,
					tl.t_fevent.t_ucepid, tl.t_fevent.t_evdat,
					tl.t_fevent.t_evdat);
			}
			break;

		/*------------------------------------------------------------*
		 * CMX signals SIGTERM and SIGIO (INET module)
		 *------------------------------------------------------------*/

		case T_FSIGTERM:
		case T_FSIGIO:
			if (eflag)
				printf("\t SIG%d #%d\n", tl.info, tl.relsec);
			break;

		/*------------------------------------------------------------*
		 * Log and socket calls of INET module(s)
		 *------------------------------------------------------------*/

		case T_FTCPSCK:
			fread(&tl.t_ftcpsck, sizeof (tl.t_ftcpsck), 1, stdin);
			if (nodeb)
				continue;

			printf("%s\tsocket(", hms(tl.relsec, tl.milsec));
			if (tl.t_ftcpsck.dom == AF_UNIX)
				printf("AF_UNIX, ");
			else if (tl.t_ftcpsck.dom == AF_INET)
				printf("AF_INET, ");
			else
				printf("0x%x, ", tl.t_ftcpsck.dom);
			if (tl.t_ftcpsck.type == SOCK_STREAM)
				printf("SOCK_STREAM, ");
			else
				printf("0x%x, ", tl.t_ftcpsck.type);
			printf("%d) = %d\n", tl.t_ftcpsck.prot, tl.t_ftcpsck.res);

			if (tl.t_ftcpsck.res < 0) {
				i = tl.t_ftcpsck.cmx_errno;
				printf("\t\terrno %d: %s ###########\n", i,
					(i >= 0 && i <= sys_nerr) ? sys_errlist[i] : "?");
			}
			break;

		case T_FTCPGSOPT:
		case T_FTCPSSOPT:
			fread(&tl.t_ftcpsopt, sizeof (tl.t_ftcpsopt), 1, stdin);
			if (nodeb)
				continue;

			if (tl.id == T_FTCPSSOPT)
				printf("%s\tsetsockopt(%d, ", hms(tl.relsec, tl.milsec),tl.t_ftcpsopt.socket);
			else
				printf("%s\tgetsockopt(%d, ", hms(tl.relsec, tl.milsec),tl.t_ftcpsopt.socket);

			if (tl.t_ftcpsopt.level == SOL_SOCKET)
				printf("SOL_SOCKET, ");
			else
				printf("0x%x, ", tl.t_ftcpsopt.level);

			if (tl.t_ftcpsopt.optname == SO_REUSEADDR)
				printf("SO_REUSEADDR, ");
			else if (tl.t_ftcpsopt.optname == SO_KEEPALIVE)
				printf("SO_KEEPALIVE, ");
#define SO_PCBADDR 0x1101
			else if (tl.t_ftcpsopt.optname == SO_PCBADDR)
				printf("SO_PCBADDR, ");
			else if (tl.t_ftcpsopt.optname == SO_DEBUG)
				printf("SO_DEBUG, ");
			else if (tl.t_ftcpsopt.optname == SO_LINGER)
				printf("SO_LINGER, ");
			else
				printf("0x%x, ", tl.t_ftcpsopt.optname);

			printf("0x%x, %d) = %d\n", tl.t_ftcpsopt.optval, tl.t_ftcpsopt.optlen, tl.t_ftcpsopt.res);

			if (tl.t_ftcpsopt.res < 0) {
				i = tl.t_ftcpsopt.cmx_errno;
				printf("\t\terrno %d: %s ###########\n", i,
					(i >= 0 && i <= sys_nerr) ? sys_errlist[i] : "?");
			}
			break;

		case T_FTCPACC:
		case T_FTCPBIN:
		case T_FTCPCON:
			fread(&tl.t_ftcpabc, sizeof (tl.t_ftcpabc), 1, stdin);
			if (nodeb)
				continue;

			if (tl.id == T_FTCPACC)
				cp = "accept";
			if (tl.id == T_FTCPBIN)
				cp = "bind";
			if (tl.id == T_FTCPCON)
				cp = "connect";
			printf("%s\t%s(%d, ", hms(tl.relsec, tl.milsec),
				cp, tl.t_ftcpabc.sd);
			prsockaddr((struct sockaddr *)tl.t_ftcpabc.name);
			printf(",) = %d\n", tl.t_ftcpabc.res);

			if (tl.t_ftcpabc.res < 0) {
				i = tl.t_ftcpabc.cmx_errno;
				printf("\t\terrno %d: %s ###########\n", i,
					(i >= 0 && i <= sys_nerr) ? sys_errlist[i] : "?");
			}
			break;

		case T_FTCPLIS:
			fread(&tl.t_ftcplis, sizeof (tl.t_ftcplis), 1, stdin);
			if (nodeb)
				continue;

			printf("%s\tlisten(%d, %d) = %d\n",
				hms(tl.relsec, tl.milsec), tl.t_ftcplis.sd,
				tl.t_ftcplis.back, tl.t_ftcplis.res);

			if (tl.t_ftcplis.res < 0) {
				i = tl.t_ftcplis.cmx_errno;
				printf("\t\terrno %d: %s ###########\n", i,
					(i >= 0 && i <= sys_nerr) ? sys_errlist[i] : "?");
			}
			break;

		case T_FTUIIN:
		case T_FTUIOUT:
			fread(buf, MIN(sizeof (buf), MIN(tl.info, T_TRENTLNG) & 0xff), 1, stdin);
			if (nodeb)
				continue;
			printf("\t\tTUIMSG: %d byte, 3/4 is %d (%d traced)\n",
				tl.relsec, tl.relsec * 3 / 4,
				MIN(tl.info, T_TRENTLNG));
			prx(buf, MIN(tl.info, T_TRENTLNG) & 0xff, 0, pktline);
			break;

		case T_FTCPPKT:
		case T_FTCPHDR:
			fread(buf, MIN(sizeof (buf), tl.info), 1, stdin);
			if (nodeb)
				continue;
			if (tl.id == T_FTCPPKT)
				prtpkt((u_char *)buf, MIN(sizeof (buf), tl.info));
			else
				prthdr((u_char *)buf, MIN(sizeof (buf), tl.info));
			break;

		case T_FTCPBDY:
			fread(buf, tl.info & 0xff, 1, stdin);
			if (nodeb)
				continue;

			prtbdy((u_char *)buf, tl.info & 0xff, (tl.info >> 8) & 0xff, tl.relsec/*, tl.milsec*/);
			break;

		case T_FTCPEOT:
			if (nodeb)
				continue;
			printf("\t\tTPKT (cnt): tpdunr %d  eot %s (0x%.2x)\n",
				tl.info & 0x7f, tl.info & 0x80 ? "END" : "MORE",
				tl.info & 0xFF);
			break;

		case T_FTCPSEL: {

			fread(&tl.t_ftcpsel, sizeof (tl.t_ftcpsel), 1, stdin);
			if (nodeb)
				continue;

			if (tl.info) {
				printf("%s\tselect(%d, rfds, wfds, xfds, ",
					hms(tl.relsec, tl.milsec), tl.t_ftcpsel.nsd);
				if (tl.t_ftcpsel.sec == -1)
					printf("NULL)");
				else
					printf("%d)", tl.t_ftcpsel.sec);
				printf(" = %d\n", tl.t_ftcpsel.res);
				if (tl.t_ftcpsel.res == 0)
					break;
				if (tl.t_ftcpsel.res < 0) {
					i = tl.t_ftcpsel.cmx_errno;
					printf("\t\terrno %d: %s ###########\n",
						i, (i >= 0 && i <= sys_nerr)
							? sys_errlist[i] : "?");
					break;
				}
			}

			prfdset(&tl.t_ftcpsel.rfds, "rfds");
			prfdset(&tl.t_ftcpsel.wfds, "wfds");
			prfdset(&tl.t_ftcpsel.xfds, "xfds");

			break;
		}

		case T_FCBRES: {

			fread(&tl.t_fcbres, sizeof (tl.t_fcbres), 1, stdin);
			if (nodeb)
				continue;

			if (tl.info) {
				printf("%s\tcallback_result(%d, c_rfds, c_wfds, c_xfds, ",
					hms(tl.relsec, tl.milsec), tl.t_fcbres.nsd);
				if (tl.t_fcbres.sec == -1)
					printf("NULL)");
				else
					printf("%d)", tl.t_fcbres.sec);
				printf(" = ");
				if (tl.t_fcbres.res == T_TSEVENT)
					printf("T_TSEVENT\n");
				else if (tl.t_fcbres.res == T_USEREVENT)
					printf("T_USEREVENT\n");
				else if (tl.t_fcbres.res == T_NOEVENT)
					printf("T_NOEVENT\n");
				else
					printf("0x%x\n", tl.t_fcbres.res);
				if (tl.t_fcbres.res == 0)
					break;
				if (tl.t_fcbres.res < 0) {
					i = tl.t_fcbres.cmx_errno;
					printf("\t\terrno %d: %s ###########\n",
						i, (i >= 0 && i <= sys_nerr)
							? sys_errlist[i] : "?");
					break;
				}
			}

			prfdset(&tl.t_fcbres.rfds, "c_rfds");
			prfdset(&tl.t_fcbres.wfds, "c_wfds");
			prfdset(&tl.t_fcbres.xfds, "c_xfds");

			break;
		}
		/*------------------------------------------------------------*
		 * System calls
		 *------------------------------------------------------------*/
		case T_FOPEN:
		case T_FMKNOD:
		case T_FUNLINK:
			fread(&tl.t_fopen, sizeof (tl.t_fopen), 1, stdin);
			if (nodeb)
				continue;
			printf ("%s\t", hms(tl.relsec, tl.milsec));
			if (tl. id == T_FOPEN)
				printf("open(\"%.*s\", %#o)",
					16, tl.t_fopen.path,
					tl.t_fopen.mode);
			else if (tl. id == T_FMKNOD)
				printf("mknod(\"%.*s\", %#o, %d)", 16,
					tl.t_fopen.path,
					tl.t_fopen.mode,tl.info);
			else
				printf("unlink(\"%.*s\")", 16,tl.t_fopen.path);

			printf (" == %d\n", tl.t_fopen.res);

			if (tl.t_fopen.res < 0) {
				i = tl.t_fopen.cmx_errno;
				printf("\t\terrno %d: %s ###########\n", i,
					(i >= 0 && i <= sys_nerr) ? sys_errlist[i] : "?");
			}
			break;

		case T_FCLOSE:
			fread(&tl.t_fsvc, sizeof (tl.t_fsvc), 1, stdin);
			if (nodeb)
				continue;

			printf("%s\tclose(%d)\n",
				hms(tl.relsec, tl.milsec), tl.t_fsvc.fd);
			break;

		case T_FREAD:
		case T_FWRITE:
		case T_FIOCTL:
		case T_FFCNTL:
			fread(&tl.t_fsvc, sizeof (tl.t_fsvc), 1, stdin);
			if (nodeb)
				continue;

			if (tl.id == T_FREAD)
				cp = "read";
			else if (tl.id == T_FWRITE)
				cp = "write";
			else if (tl.id == T_FFCNTL)
				cp = "fcntl";
			else
				cp = "ioctl";
			if (tl.id == T_FIOCTL)
				printf("%s\t%s(%d, %s, 0x%x) = %d\n",
					hms(tl.relsec, tl.milsec), cp, tl.t_fsvc.fd,
					priocmd(tl.t_fsvc.arg2),
					tl.t_fsvc.arg3, tl.t_fsvc.res);
			else if (tl.id == T_FFCNTL)
				printf("%s\t%s(%d, %d, 0x%x) = %d\n",
					hms(tl.relsec, tl.milsec), cp, tl.t_fsvc.fd,
					tl.t_fsvc.arg2,
					tl.t_fsvc.arg3, tl.t_fsvc.res);
			else
				printf("%s\t%s(%d, , %d) = %d\n",
					hms(tl.relsec, tl.milsec), cp, tl.t_fsvc.fd,
					tl.t_fsvc.arg3, tl.t_fsvc.res);

			if (tl.t_fsvc.res == -1) {
				i = tl.t_fsvc.cmx_errno;
				printf("\t\terrno %d: %s ###########\n", i,
					(i >= 0 && i <= sys_nerr) ? sys_errlist[i] : "?");
			}
			break;
		case T_FSTRING:
			fgets (strbuf, 80,stdin);
			if (nodeb)
				continue;

			printf("%s string (%d): %s", hms(tl.relsec, tl.milsec), tl.info,strbuf);
			break;
		case T_FSNAPIPE:
			fread(&tl.t_fsnapipe, sizeof (tl.t_fsnapipe), 1, stdin);
			i = tl. t_fsnapipe. len_ges-sizeof(tl. t_fsnapipe);
			if (i)
				fread(buf, i, 1, stdin);
			if (nodeb)
				continue;

			printf("\t\t- SNA pipe (%d):", tl. info);
			printf (" length == %d\n", tl. t_fsnapipe. len_ges);

			printf ("\t\t  Function==%d, fill==%d, pid==%d, transref==%d\n",
			   tl. t_fsnapipe. fkt_aufruf, tl. t_fsnapipe. p_fill,
				tl. t_fsnapipe. proz_id, tl. t_fsnapipe. transref);

			printf ("\t\t  Parameters == %d, %d, %d, %d, %d\n",
				tl. t_fsnapipe. param,
				tl. t_fsnapipe. param1, tl. t_fsnapipe. param2,
				tl. t_fsnapipe. param3, tl. t_fsnapipe. param4);

			if (i) {
				printf ("\t\t  Transport sna address:\n");
				plu ("local", (u_char *)buf);
				printf ("local adr==0x%02x\t", buf[8]);
				printf ("fill==0x%02x\n", buf[9]);
				plu ("partner", (u_char *)buf+10);
				printf ("region==0x%02x\t", buf[18]);
				printf ("procesor==0x%02x\n", buf[19]);
				i -= sizeof (trans_adr);
			}
			if (i) {
				printf ("\t\t  User data:\n");
				prx (buf + sizeof (trans_adr), i, 0, putline);
			}

			break;

		case T_DLERRO:
		case T_DLERRS:
		case T_STRING:
			fread(&tl.t_fdlerr, sizeof (tl.t_fdlerr),1,stdin);
			printf("%s ", hms(tl.relsec, tl.milsec));
			switch (tl.id) {
				case T_DLERRO:
					printf("error at dlopen: ");
					break;
				case T_DLERRS:
					printf("error at dlsym: ");
					break;
				case T_STRING:
					printf("CMX-DEB: ");
					break;
			}
			printf("%s\n", (char *)tl.t_fdlerr.errarray);
			break;

		case T_UNAME:
			fread(&tl.t_funame, sizeof (tl.t_funame),1,stdin);
			if (nodeb)
				continue;

			printf(" %s %s %s %s %s ## %s\n",
				tl.t_funame.unameres.sysname,
				tl.t_funame.unameres.nodename,
				tl.t_funame.unameres.release,
				tl.t_funame.unameres.version,
				tl.t_funame.unameres.machine,
				tl.t_funame.version);
			break;

		/*------------------------------------------------------------*
		 * XTI
		 *------------------------------------------------------------*/
		case T_XTI_ACCEPT:
			fread(&tl.t_fXaccept, sizeof (tl.t_fXaccept), 1, stdin);
			if(Xflag) {
				printf("%s >t_accept(fd=%d, resfd=%d", hms(tl.relsec, tl.milsec),
					tl.t_fXaccept.fd, tl.t_fXaccept.resfd);
				XtiprPtr(tl.t_fXaccept.callp, ", call");
				printf (")\n");
				/* XXX */
				XtiprCall(tl.t_fXaccept.callp, &tl.t_fXaccept.call, "call", FLAG_SEQUENCE);
			}
			XtirxdBuf(tl.t_fXaccept.callp, &tl.t_fXaccept.call.addr, "addr.buf");
			XtirxdBuf(tl.t_fXaccept.callp, &tl.t_fXaccept.call.opt, "opt.buf");
			XtirxdBuf(tl.t_fXaccept.callp, &tl.t_fXaccept.call.udata, "udata.buf");
			break;

		case T_XTI_ACCEPT_FIN:
			XtiFin ("t_accept");
			break;

		case T_XTI_BIND:
			fread(&tl.t_fXbind, sizeof (tl.t_fXbind), 1, stdin);
			if(Xflag) {
				printf("%s >t_bind(fd=%d", hms(tl.relsec, tl.milsec), tl.t_fXbind.fd);
				XtiprPtr(tl.t_fXbind.reqp, ", req");
				XtiprPtr(tl.t_fXbind.retp, ", ret");
				printf (")\n");
				/* XXX */
				XtiprBind(tl.t_fXbind.reqp, &tl.t_fXbind.req, "req");
				XtiprBind(tl.t_fXbind.retp, &tl.t_fXbind.ret, "ret");
			}
			XtirxdBuf (tl.t_fXbind.reqp, &tl.t_fXbind.req.addr, "req.addr.buf");
			break;

		case T_XTI_BIND_FIN:
			fread(&tl.t_fXbindfin, sizeof (tl.t_fXbindfin), 1, stdin);
			if(Xflag) {
				printf("%s <t_bind(...", hms(tl.relsec, tl.milsec));
				XtiprRet(&tl.t_fXbindfin.xret);
				/* XXX */
				XtiprBind(tl.t_fXbindfin.retp, &tl.t_fXbindfin.ret, "ret");
			}
			XtirxdBuf (tl.t_fXbindfin.retp, &tl.t_fXbindfin.ret.addr, "ret.addr.buf");
			break;

		case T_XTI_CLOSE:
			XtiFd ("t_close");
			break;

		case T_XTI_CLOSE_FIN:
			XtiFin ("t_close");
			break;

		case T_XTI_CONNECT:
			fread(&tl.t_fXconnect, sizeof (tl.t_fXconnect), 1, stdin);
			if(Xflag) {
				printf("%s >t_connect(fd=%d", hms(tl.relsec, tl.milsec), tl.t_fXconnect.fd);
				XtiprPtr(tl.t_fXconnect.sndcallp, ", sndcall");
				XtiprPtr(tl.t_fXconnect.rcvcallp, ", rcvcall");
				printf (")\n");
				/* XXX */
				XtiprCall(tl.t_fXconnect.sndcallp, &tl.t_fXconnect.sndcall, "sndcall", 0);
				XtiprCall(tl.t_fXconnect.rcvcallp, &tl.t_fXconnect.rcvcall, "rcvcall", 0);
			}
			XtirxdBuf(tl.t_fXconnect.sndcallp, &tl.t_fXconnect.sndcall.addr, "sndcall.addr.buf");
			XtirxdBuf(tl.t_fXconnect.sndcallp, &tl.t_fXconnect.sndcall.opt, "sndcall.opt.buf");
			XtirxdBuf(tl.t_fXconnect.sndcallp, &tl.t_fXconnect.sndcall.udata, "sndcall.udata.buf");
			break;

		case T_XTI_CONNECT_FIN:
			XtiCallfin("t_connect", "rcvcall", 0);
			break;

		case T_XTI_GETSTATE:
			XtiFd ("t_getstate");
			break;

		case T_XTI_GETSTATE_FIN:
			XtiFin ("t_getstate");
			break;

		case T_XTI_LISTEN:
			XtiCall("t_listen", 0);
			break;

		case T_XTI_LISTEN_FIN:
			XtiCallfin("t_listen", "call", FLAG_SEQUENCE);
			break;

		case T_XTI_LOOK:
			XtiFd ("t_look");
			break;

		case T_XTI_LOOK_FIN:
			XtiFin ("t_look");
			break;

		case T_XTI_OPEN:
			fread(&tl.t_fXopen, sizeof (tl.t_fXopen), 1, stdin);
			if (tl.t_fXopen.namep != NULL)
				fread(buf, XMIN(BUFSIZ, tl.t_fXopen.namel), 1, stdin);
			if(Xflag) {
				printf("%s >t_open(", hms(tl.relsec, tl.milsec));
				XtiprPtr(tl.t_fXopen.namep, "name");
				printf(", oflag=0x%x", tl.t_fXopen.oflag);
				XtiprPtr(tl.t_fXopen.infop, ", info");
				printf (")\n");
				if (tl.t_fXopen.namep != NULL)
					printf ("\t\tname=\"%s\"\n", buf);
			}
			break;

		case T_XTI_OPEN_FIN:
			fread(&tl.t_fXopenfin, sizeof (tl.t_fXopenfin), 1, stdin);
			if(Xflag) {
				printf("%s <t_open(...", hms(tl.relsec, tl.milsec));
				XtiprRet(&tl.t_fXopenfin.xret);
				if(tl.t_fXopenfin.infop != NULL) {
					printf("\t\tinfo: addr=%d, options=%d, tsdu=%d, etsdu=%d,\n",
						tl.t_fXopenfin.info.addr, tl.t_fXopenfin.info.options,
						tl.t_fXopenfin.info.tsdu, tl.t_fXopenfin.info.etsdu);
					printf("\t\t      connect=%d, discon=%d, servtype=%d\n",
						tl.t_fXopenfin.info.connect, tl.t_fXopenfin.info.discon,
						tl.t_fXopenfin.info.servtype);
				}
			}
			break;

		case T_XTI_RCV:
			fread(&tl.t_fXrcvreq, sizeof (tl.t_fXrcvreq), 1, stdin);
			if(Xflag) {
				printf("%s >t_rcv(fd=%d", hms(tl.relsec, tl.milsec), tl.t_fXrcvreq.fd);
				XtiprPtr(tl.t_fXrcvreq.bufp, ", buf");
				printf(", nbytes=%u, flags)\n", tl.t_fXrcvreq.nbytes);
			}
			break;

		case T_XTI_RCV_FIN:
			fread(&tl.t_fXrcvfin, sizeof (tl.t_fXrcvfin), 1, stdin);
			if(Xflag) {
				printf("%s <t_rcv(...", hms(tl.relsec, tl.milsec));
				printf(", flags=0x%x", tl.t_fXrcvfin.flags);
				XtiprRet(&tl.t_fXrcvfin.xret);
			}
			if (tl.t_fXrcvfin.bufp != NULL && tl.t_fXrcvfin.nbytest > 0) {
				fread(buf, tl.t_fXrcvfin.nbytest, 1, stdin);
				if(Xflag) {
					printf ("\t   buf:\n");
					prx (buf, tl.t_fXrcvfin.nbytest, 0, putline);
				}
			}
			break;

		case T_XTI_RCVCONNECT:
			XtiCall("t_rcvconnect", 0);
			break;

		case T_XTI_RCVCONNECT_FIN:
			XtiCallfin("t_rcvconnect", "call", 0);
			break;

		case T_XTI_RCVDIS:
			fread(&tl.t_fXdis, sizeof (tl.t_fXdis), 1, stdin);
			if(Xflag) {
				printf("%s >t_rcvdis(fd=%d", hms(tl.relsec, tl.milsec), tl.t_fXdis.fd);
				XtiprPtr(tl.t_fXdis.disp, ", discon");
				printf (")\n");
				XtiprNbuf(tl.t_fXdis.disp, &tl.t_fXdis.dis.udata, "udata");
			}
			XtirxdBuf(tl.t_fXdis.disp, &tl.t_fXdis.dis.udata, "udata.buf");
			break;

		case T_XTI_RCVDIS_FIN:
			fread(&tl.t_fXdis, sizeof (tl.t_fXdis), 1, stdin);
			if(Xflag) {
				printf("%s <t_rcvdis(...", hms(tl.relsec, tl.milsec));
				XtiprRet(&tl.t_fXdis.xret);
				XtiprNbuf(tl.t_fXdis.disp, &tl.t_fXdis.dis.udata, "udata");
				if (tl.t_fXdis.disp != NULL)
					printf ("\t\treason=%d, sequence=%d\n",
						tl.t_fXdis.dis.reason, tl.t_fXdis.dis.sequence);
			}
			XtirxdBuf (tl.t_fXdis.disp, &tl.t_fXdis.dis.udata, "udata.buf");
			break;

		case T_XTI_RCVREL:
			XtiFd ("t_rcvrel");
			break;

		case T_XTI_RCVREL_FIN:
			XtiFin ("t_rcvrel");
			break;

		case T_XTI_SND:
			fread(&tl.t_fXsndreq, sizeof (tl.t_fXsndreq), 1, stdin);
			if(Xflag) {
				printf("%s >t_snd(fd=%d", hms(tl.relsec, tl.milsec), tl.t_fXsndreq.fd);
				XtiprPtr(tl.t_fXsndreq.bufp, ", buf");
				printf(", nbytes=%u, flags=0x%x)\n", tl.t_fXsndreq.nbytes, tl.t_fXsndreq.flags);
			}
			if (tl.t_fXsndreq.bufp != NULL && tl.t_fXsndreq.nbytest > 0) {
				fread(buf, tl.t_fXsndreq.nbytest, 1, stdin);
				if(Xflag) {
					printf ("\t   buf:\n");
					prx (buf, tl.t_fXsndreq.nbytest, 0, putline);
				}
			}
			break;

		case T_XTI_SND_FIN:
			XtiFin ("t_snd");
			break;

		case T_XTI_SNDDIS:
			fread(&tl.t_fXcall, sizeof (tl.t_fXcall), 1, stdin);
			if(Xflag) {
				printf("%s >t_snddis(fd=%d", hms(tl.relsec, tl.milsec), tl.t_fXcall.fd);
				XtiprPtr(tl.t_fXcall.callp, ", call");
				printf (")\n");
				XtiprNbuf(tl.t_fXcall.callp, &tl.t_fXcall.call.udata, "udata");
				if (tl.t_fXcall.callp != NULL)
					printf ("\t\tsequence=%d\n", tl.t_fXcall.call.sequence);
			}
			XtirxdBuf (tl.t_fXcall.callp, &tl.t_fXcall.call.udata, "udata.buf");
			break;

		case T_XTI_SNDDIS_FIN:
			XtiFin ("t_snddis");
			break;

		case T_XTI_SNDREL:
			XtiFd ("t_sndrel");
			break;

		case T_XTI_SNDREL_FIN:
			XtiFin ("t_sndrel");
			break;

		case T_XTI_SYNC:
			XtiFd ("t_sync");
			break;

		case T_XTI_SYNC_FIN:
			XtiFin ("t_sync");
			break;

		case T_XTI_UNBIND:
			XtiFd ("t_unbind");
			break;

		case T_XTI_UNBIND_FIN:
			XtiFin ("t_unbind");
			break;

		default:
			if (nflag) {
				fprintf(stderr, EBADENT, av0, tl.id);
				exit(1);
				/* NOTREACHED */
			}
		}
	}
}

static void plu (char *n, unsigned char *b)
{
	int	a;
	printf ("\t\t  %s lu name==[", n);
	for (a = 0; a < 8 && b[a]; a++)
		printf ("%02x", b[a]);
	printf ("]\t");
}

static void XtiprRet (struct	xti_ret	*pv_xr)
{
	printf(") = %d\n", pv_xr->retval);
	if(pv_xr->retval < 0)
		printf("\t\t XTI-ERRORMESSAGE ###########(t_errno = %d, errno = %d)\n",
			pv_xr->xti_errno, pv_xr->cmx_errno);
}

static void XtiCall (
	char	*pv_fkt,
	int	 pv_flag)
{
	fread(&tl.t_fXcall, sizeof (tl.t_fXcall), 1, stdin);
	if(Xflag) {
		printf("%s >%s(fd=%d", hms(tl.relsec, tl.milsec), pv_fkt, tl.t_fXcall.fd);
		XtiprPtr(tl.t_fXcall.callp, ", call");
		printf (")\n");
		XtiprCall(tl.t_fXcall.callp, &tl.t_fXcall.call, "call", pv_flag);
	}
}

static void XtiCallfin (
	char	*pv_fkt,
	char	*pv_str,
	int	 pv_flag)
{
	fread(&tl.t_fXcallfin, sizeof (tl.t_fXcallfin), 1, stdin);
	if(Xflag) {
		printf("%s <%s(...", hms(tl.relsec, tl.milsec), pv_fkt);
		XtiprRet(&tl.t_fXcallfin.xret);
		XtiprCall(tl.t_fXcallfin.callp, &tl.t_fXcallfin.call, pv_str, pv_flag);
	}
	XtirxdBuf(tl.t_fXcallfin.callp, &tl.t_fXcallfin.call.addr, "addr.buf");
	XtirxdBuf(tl.t_fXcallfin.callp, &tl.t_fXcallfin.call.opt, "opt.buf");
	XtirxdBuf(tl.t_fXcallfin.callp, &tl.t_fXcallfin.call.udata, "udata.buf");
}

static void XtiFd (char	*pv_txt)
{
	fread(&tl.t_fXfd, sizeof (tl.t_fXfd), 1, stdin);
	if(Xflag)
		printf("%s >%s(fd=%d)\n",
			hms(tl.relsec, tl.milsec), pv_txt, tl.t_fXfd.fd);
}

static void XtiFin (char	*pv_txt)
{
	fread(&tl.t_fXfin, sizeof (tl.t_fXfin), 1, stdin);
	if(Xflag) {
		printf("%s <%s(...", hms(tl.relsec, tl.milsec), pv_txt);
		XtiprRet(&tl.t_fXfin.xret);
	}
}

static void XtirxdBuf (
	void		*pv_ptr,
	struct netbuf	*pv_nb,
	char		*pv_txt)
{
	if (pv_ptr == NULL || pv_nb->len <= 0)
		return;

	fread(buf, XMIN(BUFSIZ, pv_nb->len), 1, stdin);

	if (Xflag) {
		printf ("\t   %s:\n", pv_txt);
		prx (buf, XMIN(BUFSIZ, pv_nb->len), 0, putline);
	}
}

static void XtiprPtr	(
	void		*pv_ptr,
	char		*pv_txt)
{
	if(pv_ptr != NULL)
		printf("%s=0x%p", pv_txt, pv_ptr);
	else
		printf("%s=NULL", pv_txt);
}

static void XtiprNbuf (
	void		*pv_ptr,
	struct netbuf	*pv_nb,
	char		*pv_txt)
{
	if (pv_ptr == NULL || !Xflag)
		return;

	printf("\t\t%s: maxlen=%d, len=%d, buf=0x%x\n",
		pv_txt, pv_nb->maxlen, pv_nb->len, pv_nb->buf);
}

static void XtiprCall (
	void		*pv_ptr,
	struct t_call	*pv_call,
	char		*pv_txt,
	int		 pv_flag)
{
	if (pv_ptr == NULL || !Xflag)
		return;

	printf("\t       %7s: addr.maxlen=%d, .len=%d, .buf=0x%x\n",
		pv_txt, pv_call->addr.maxlen, pv_call->addr.len, pv_call->addr.buf);
	printf("\t\t\topt.maxlen=%d, .len=%d, .buf=0x%x\n",
		pv_call->opt.maxlen, pv_call->opt.len, pv_call->opt.buf);
	printf("\t\t\tudata.maxlen=%d, .len=%d, .buf=0x%x\n",
		pv_call->udata.maxlen, pv_call->udata.len, pv_call->udata.buf);
	if ((pv_flag & FLAG_SEQUENCE) != 0)
		printf("\t\t\tsequence=%d\n", pv_call->sequence);
}

static void XtiprBind (
	void		*pv_ptr,
	struct t_bind	*pv_bind,
	char		*pv_txt)
{
	if (pv_ptr == NULL)
		return;

	printf("\t\t%s: addr.maxlen=%d, .len=%d, .buf=0x%x, qlen=%d\n",
		pv_txt, pv_bind->addr.maxlen, pv_bind->addr.len,
		pv_bind->addr.buf, pv_bind->qlen);
}

static void t_printhead(char * file)
{

	t_trclen = tl.t_ffirst.len;
	if ((tl.id != T_LIBV30B && tl.id != T_LIBV30 &&
		tl.id != T_LIBV40 && tl.id != T_LIBV25 ) ||
		((t_trclen > sizeof (buf)) && nodeb)) {
		fprintf(stderr, EBADENT, av0, tl.id);
		exit(1);
	}
	if (tl.id == T_LIBV30 || tl.id == T_LIBV30B || tl.id == T_LIBV40 )
		hms = hms30;
	if (tl.id == T_LIBV25)
		hms = hms25;
	strt = tl.t_ffirst.abs;
	printf(HEADLINE1, vers, VERSION, ctime(&strt) + 4);
	printf(HEADLINE21);
	if (cflag)
		(void) putchar('c');
	if (dflag)
		(void) putchar('d');
	if (eflag)
		(void) putchar('e');
	if (tflag)
		(void) putchar('t');
	if (vflag)
		(void) putchar('v');
	else
		(void) putchar('x');
	if(Xflag)
		(void) putchar('X');
	if (!nodeb)
		(void) putchar('D');
	cp = strrchr(file, '/');
	if (cp == NULL)
		cp = file;
	else
		cp += 1;
	printf(HEADLINE22, cp, prvers(tl.id));
	if (t_trclen)
		printf(HEADLINE23, t_trclen);
	else
		printf("\n\n");

	return;
}
