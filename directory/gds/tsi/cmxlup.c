/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxlup.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:23  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:47  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:48:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:26  root]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:33  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:22:15  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:48:21  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:46:27  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:27  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:43  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:28  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:20  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxlup.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:23 $";
#endif

/*
 * COPYRIGHT (C) 1990 BY SIEMENS AG  All rights reserved
 * Copyright (C) Siemens Nixdorf Informationssysteme AG 1990
 * 		 All Rights Reserved
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
 * cmxlup -- subprogram for preparing the ICMX(L) TRACE
 *
 * Defines:
 * TUI          for trace of TUI connection
 */

#define MIN(A, B)	(((A) > (B)) ? (B) : (A) < 0) ? 0 : (A)

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <netinet/in.h>
#ifdef __OSF__
#include <machine/endian.h>
#endif /* __OSF__ */
#include <sys/ioctl.h>
#include <sys/socket.h>
#if defined(_AIX)
#include <sys/select.h>
#endif
#include <sys/uio.h>
#include "cmx.h"
#include "cmxaddr.h"
#include "cmxtrace.h"
#include "cmxl.h"
#include "cxioctl.h"
/*
#include "cxmon.h"
*/
#ifdef TUI
#include "netun.h"
#endif

/*
static char sccsid[] = "@(#)cmxlup.c	311.20 93/08/24 CMX (NC1)";
*/

/*****************************************************************************/
/*      D E C L A R A T I O N  G L O B A L  D A T A                           */
/*****************************************************************************/

#ifndef OPEN_MAX
#define OPEN_MAX FD_SETSIZE
#endif

extern char	*sys_errlist[];
extern int	sys_nerr;

struct txt	kops[] = {
	{T_COM,		"T_COM"},
	{T_TUI,		"T_TUI"},
	{T_SNA,		"T_SNA"},
	{T_INET,	"T_INET"},
	{T_XTI,		"T_XTI"},
	{-1000,		NULL}
};

struct txt	reasons[] = {
	{T_USER,	"T_USER"},
	{T_RTIMEOUT,	"T_RTIMEOUT"},
	{T_RADMIN,	"T_RADMIN"},
	{T_RCCPEND,	"T_RCCPEND"},
	{T_RUNKNOWN,	"T_RUNKNOWN"},
	{T_RSAPCONGEST,	"T_RSAPCONGEST"},

	{T_RSAPNOTATT,	"T_RSAPNOTATT"},
	{T_RUNSAP,	"T_RUNSAP"},
	{T_RCONGEST,	"T_RCONGEST"},
	{T_RCONNFAIL,	"T_RCONNFAIL"},
	{T_RDUPREF,	"T_RDUPREF"},

	{T_RMISREF,	"T_RMISREF"},
	{T_RPROTERR,	"T_RPROTERR"},
	{T_RREFOFLOW,	"T_RREFOFLOW"},
	{T_RNOCONN,	"T_RNOCONN"},
	{T_RINLNG,	"T_RINLNG"},

	{T_RLCONGEST,	"T_RLCONGEST"},
	{T_RLNOQOS,	"T_RLNOQOS"},
	{T_RLPROTERR,	"T_RLPROTERR"},
	{T_RLINTIDU,	"T_RLINTIDU"},
	{T_RLNORMFLOW,	"T_RLNORMFLOW"},

	{T_RLEXFLOW,	"T_RLEXFLOW"},
	{T_RLINSAPID,	"T_RLINSAPID"},
	{T_RLINCEPID,	"T_RLINCEPID"},
	{T_RLINPAR,	"T_RLINPAR"},
	{T_RLNOPERM,	"T_RLNOPERM"},

	{T_RLPERMLOST,	"T_RLPERMLOST"},
	{T_RLNOCONN,	"T_RLNOCONN"},
	{T_RLCONNLOST,	"T_RLCONNLOST"},
	{T_RLNORESP,	"T_RLNORESP"},
	{T_RLIDLETRAF,	"T_RLIDLETRAF"},

	{T_RLRESYNC,	"T_RLRESYNC"},
	{T_RLEXLOST,	"T_RLEXLOST"},
	{T_RSYSERR,	"T_RSYSERR"},
	{T_RILLPWD,	"T_RILLPWD"},
	{T_RNETACC,	"T_RNETACC"},

	{-1000,		NULL}
};

/*****************************************************************************/
/*      D E C L A R A T I O N  I N T E R N A L  D A T A                      */
/*****************************************************************************/

static struct txt	evts[] = {
	{T_NOEVENT,	"T_NOEVENT"},
	{T_DATAIN,	"T_DATAIN"},
	{T_DATAGO,	"T_DATAGO"},
	{T_XDATIN,	"T_XDATIN"},
	{T_XDATGO,	"T_XDATGO"},
	{T_CONIN,	"T_CONIN"},
	{T_CONCF,	"T_CONCF"},
	{T_DISIN,	"T_DISIN"},
	{T_REDIN,	"T_REDIN"},
	{-1000,		NULL}
};

static struct txt	tuicmd[] = {
#ifdef TUI
	{S_OWNER,	"S_OWNER"},
	{S_OPON,	"S_OPON"},
	{S_RESP,	"S_RESP"},
	{S_SIGTYPE,	"S_SIGTYPE"},
	{S_SIGREL,	"S_SIGREL"},
	{S_INFO,	"S_INFO"},
	{S_CLON,	"S_CLON"},
	{S_REDRQ,	"S_REDRQ"},
	{S_WRITE,	"S_WRITE"},
	{S_DETACH,	"S_DETACH"},
	{S_FASTWRITE,	"S_FASTWRITE"},

	{G_RESP,	"G_RESP"},
	{G_OPON,	"G_OPON"},
	{G_INFO,	"G_INFO"},
	{G_SIGC,	"G_SIGC"},
	{G_CLON,	"G_CLON"},
	{G_REDIN,	"G_REDIN"},
	{G_READ,	"G_READ"},
	{G_PARAM,	"G_PARAM"},
	{G_NAMST,	"G_NAMST"},
	{G_EVENT,	"G_EVENT"},
#endif
	{-1000,		NULL}
};

static struct txt	iocmd[] = {
	{STNOCMD,	"STNOCMD"},
	{STINIRQ,	"STINIRQ"},
	{STTRMRQ,	"STTRMRQ"},
	{STEVENT,	"STEVENT"},
	{STCONRQ,	"STCONRQ"},
	{STCONIN,	"STCONIN"},
	{STCONRS,	"STCONRS"},
	{STCONCF,	"STCONCF"},
	{STDISRQ,	"STDISRQ"},
	{STDISIN,	"STDISIN"},
	{STREDRQ,	"STREDRQ"},
	{STREDIN, 	"STREDIN"},
	{STDATRQ,	"STDATRQ"},
	{STDATIN,	"STDATIN"},
	{STXDTRQ, 	"STXDTRQ"},
	{STXDTIN,	"STXDTIN"},
	{STDATSTOP,	"STDATSTOP"},
	{STXDTSTOP,	"STXDTSTOP"},
	{STDATGO,	"STDATGO"},
	{STXDTGO,	"STXDTGO"},
	{STINFO,	"STINFO"},
	{ST2INFO,	"ST2INFO"},
	{STVEVENT,	"STVEVENT"},
	{STWEVENT,	"STWEVENT"},
	{-1000,		NULL}
};

static struct txt	errors[] = {
	{T_OK,		"T_OK (T_NOERROR)"},
	{T_UNSPECIFIED,	"T_UNSPECIFIED"},
	{T_WSEQUENCE,	"T_WSEQUENCE"},
	{T_WREQUEST,	"T_WREQUEST"},
	{T_WPARAMETER,	"T_WPARAMETER"},
	{T_WAPPLICATION,"T_WAPPLICATION"},
	{T_WAPP_LIMIT,	"T_WAPP_LIMIT"},
	{T_WCONN_LIMIT,	"T_WCONN_LIMIT"},
	{T_WTREF,	"T_WTREF"},
	{T_WTUI_MSG,	"T_WTUI_MSG"},
	{T_COLLISION,	"T_COLLISION"},
	{T_WPROC_LIMIT,	"T_WPROC_LIMIT"},
	{T_NOCCP,	"T_NOCCP"},
	{T_ETIMEOUT,	"T_ETIMEOUT"},
	{T_WROUTINFO,	"T_WROUTINFO"},
	{T_CCP_END,	"T_CCP_END"},
	{T_WRED_LIMIT,	"T_WRED_LIMIT"},
	{T_WLIBVERSION,	"T_WLIBVERSION"},
	{T_CBRECURSIVE,	"T_CBRECURSIVE"},
	{T_W_NDS_ACCESS,"T_W_NDS_ACCESS"},
	{-1000,		NULL}
};

static struct txt	oa1[] = {
	{T_ACTIVE,	"T_ACTIVE"},
	{T_PASSIVE,	"T_PASSIVE"},
	{T_REDIRECT,	"T_REDIRECT"},
	{-1000,		NULL}
};

#define	CRTPDU	0xe0
#define	CCTPDU	0xd0
#define	DRTPDU	0x80
#define	DTTPDU	0xf0
#define	AKTPDU	0x60
#define	RRTPDU	0x05

static struct txt	tp0[] = {
	{CRTPDU,	"CR"},
	{CCTPDU,	"CC"},
	{DRTPDU,	"DR"},
	{DTTPDU,	"DT"},
	{AKTPDU,	"AK"},
	{RRTPDU,	"RR"},
	{-1000,	NULL}
};

/*
 * Constant texts for address formats and CC status
 */
struct adrform {
	char	*formst;
	int	formid;
} adrform [] = {
	{"WANNEA",	CX_WANNEA},
	{"LANSBKA",	CX_LANSBKA},
	{"LANINET",	CX_LANINET},
	{"RFC1006",	CX_RFC1006},
	{"WAN3SBKA",	CX_WAN3SBKA},
	{"WANSBKA",	CX_WANSBKA},
	{"STANEA",	CX_STANEA},
	{"EMSNA",	CX_EMSNA},
	{"SDLCSBKA",	CX_SDLCSBKA},
	{"LBS2NEA",	CX_LBS2NEA},
	{"LOOPSBKA",	CX_LOOPSBKA},
	{"TRSNASBKA",	CX_TRSNASBKA},
	{"LANGWSBKA",	CX_LANGWSBKA},
	{"WANGWSBKA",	CX_WANGWSBKA},
	{"OSITYPE",	CX_OSITYPE},
	{NULL,		0}
};

/*---------------------------------------------------------------------------*
 *      H E L P   F U N C T I O N S
 *---------------------------------------------------------------------------*/

/*
 * prmn - clear text representation of formats of LOCAL NAME
 */
void prmn (struct t_myname	*p)
{
	int	j, sel_cnt;
	union cx_unisel	*c;
	u_short	s;

	if (p->t_mnmode == T_MNMODE && p->t_mnlng <= sizeof(struct t_myname)) {
		sel_cnt = (p->t_mnlng - (CXN_PRESIZE + CXN_FIXSIZE)) / 
			sizeof (union cx_unisel);
		c = ((struct cx_myname *) p->t_mn)->cxn_sel;
		if (sel_cnt > 0 && sel_cnt <= CXN_SELNUM) {
			while (sel_cnt--) {
				s = c->cxs.cx_type;
				for (j = 0; adrform[j].formid && s; j++) {
					if (s & adrform[j].formid) {
						printf("%s ", adrform[j].formst);
						s &= ~adrform[j].formid;
					}
				}
				c++;
			}
			(void) putchar('\n');
			return;
		}
	}
	printf("?\n");
}

/*
 * praddr - clear text representation of TRANSPORT ADDRESS formats
 */
void praddr (struct t_partaddr	*p)
{
	int	j;
	struct cx_addr	*c;

	if (p->t_pamode == T_PAMODE && p->t_palng <= sizeof(struct t_partaddr)) {
		c = (struct cx_addr *) p->t_pa;
		for (j = 0; adrform[j].formid && c->cx_type; j++) {
			if (c->cx_type & adrform[j].formid) {
				printf("%s\n", adrform[j].formst);
				return;
			}
		}
	}
	printf("?\n");
}

/*
 * prtxt - clear text representation of entry from table
 */
void prtxt (struct txt	*table, int	entry)
{
	register	j;

	for (j = 0; table[j].val >= 0; j++) {
		if (table[j].val == entry) {
			printf("%s (%d)", table[j].str, entry);
			break;
		}
	}
	if (table[j].val < 0)
		printf("? (0x%.2x)", entry);
	return;
}

/*
 * priocmd - clear text representation of binaerem ioctl-cmd entry
 */
char *priocmd (int	entry)
{
	register	j;
	int		cmd;
	char		c;
	static char	s[16];

	c = CX_GCMDCHAR(entry);
	cmd = CX_GCMDID(entry);

	switch (c) {
	case 'P':		/* T_TUI */
		for (j = 0; tuicmd[j].val >= 0; j++) {
			if (tuicmd[j].val == entry)
				return (tuicmd[j].str);
		}
		break;

	case 'C':               /* T_COM: administration */
		if (cmd == 8)
			return ("CX_PRENETOPEN");
		break;

	case 'S':               /* T_COM: commnikacation */
		for (j = 0; iocmd[j].val >= 0; j++) {
			if (iocmd[j].val == cmd)
				return (iocmd[j].str);
		}
		break;
	}

	sprintf(s, "? (0x%.8x)", entry);
	return (s);
}

/*
 * prevt - prepare events
 */
void prevt(int evt, int tref)
{
	register	j;

	for (j = 0; evts[j].val >= 0; j++) {
		if (evts[j].val == evt) {
			printf("%s (%d)", evts[j].str, evt);
			break;
		}
	}

	if (evts[j].val < 0) {
		printf("? (0x%x)", evt);
		return;
	}

	if (evt != T_NOEVENT)
		printf(" tref 0x%x\n", tref);
	else
		(void) putchar('\n');
}

/*
 * prerr - prepare error code
 */
char *prerr (int err)
{
	register	j;

	for (j = 0; errors[j].val >= 0; j++) {
		if (errors[j].val == err)
			return (errors[j].str);
	}

	if (errors[j].val < 0)
		if (err >= 0 && err <= sys_nerr)
			return (sys_errlist[err]);

	return ("?");
}

/*
 * putline - output of a prepared prx line
 * pktline - output of a prepared prx line for TPKT
 */
void putline(char *l)
{
	printf("\t%s", l);
}

void pktline(char *l)
{
	printf("\t%s", l);
}

/*
 * hms - calculate seconds (relative) in mm:ss:mmm
 */
char *hms30 (short secs, short mils)
{
	char	*cp;
	long	sec = (long) secs + strt;

	cp = ctime(&sec);
	cp = &cp[14];

	sprintf (cp+5, ".%03d", (int) mils);
	return (cp);
}
/*
 * hms25 - calculate seconds (relative) in hh:mm:ss
 */
char *hms25 (short secs, short mils)
{
	static char	*cp = "\t";
	static long	secsl;

	secsl = (long) secs + strt;
	if (secsl <= -1)
		return (cp);
	cp = ctime(&secsl);
	cp[19] = '\0';
	cp = &cp[11];
	return (cp);
}


/*
 * propta1 - prepare t_opta1 or t_opta2
 */
void propta1 (int	*ip)
{
	struct t_opta2	*p = (struct t_opta2 *) ip;

	if (!vflag)
		return;

	printf("\t   opt:\n\t\tt_optnr ");
	if (p->t_optnr == T_OPTA1)
		printf("T_OPTA1 (%d)", T_OPTA1);
	else if (p->t_optnr == T_OPTA2)
		printf("T_OPTA2 (%d)", T_OPTA2);
	else if (p->t_optnr == T_OPTA4)
		printf("T_OPTA4 (%d)", T_OPTA4);
	else if (p->t_optnr == T_OPTA5)
		printf("T_OPTA5 (%d)", T_OPTA5);
	else if (p->t_optnr == T_OPTA6)
		printf("T_OPTA6 (%d)", T_OPTA6);
	else
		printf("? (0x%x)", p->t_optnr);

	printf("  t_apmode ");
	if ((p->t_apmode & (T_ACTIVE|T_PASSIVE|T_REDIRECT)) != 0
		&& (p->t_apmode & ~(T_ACTIVE|T_PASSIVE|T_REDIRECT)) == 0) {
		register j;
		int	first = 0;
		int	ap;

		ap = p->t_apmode;
		for (j = 0; oa1[j].val && ap; j++) {
			if (ap & oa1[j].val) {
				printf("%s%s", first ? "|" : "", oa1[j].str);
				first++;
				ap &= ~oa1[j].val;
			}
		}
		printf(" (%d)", p->t_apmode);
	} else
		printf("? (0x%x)", p->t_apmode);

	printf("\n\t\tt_conlim ", p->t_conlim);
	if (p->t_conlim == T_NOLIMIT)
		printf("T_NOLIMIT (%d)", T_NOLIMIT);
	else if (p->t_conlim > 0)
		printf("%d", p->t_conlim);
	else
		printf("? (%d)", p->t_conlim);

	(void) putchar('\n');
}

/*
 * proptc1 - prepare t_optc1, t_optc2, t_optc3
 */
void proptc1 (int id,int *ip, int ul, char *cp)
{
	struct t_optc3	*p = (struct t_optc3 *) ip;

	if (!vflag)
		return;

	printf("\t   opt:\n\t\tt_optnr ");
	if (p->t_optnr == T_OPTC1)
		printf("T_OPTC1 (%d)", T_OPTC1);
	else if (p->t_optnr == T_OPTC2)
		printf("T_OPTC2 (%d)", T_OPTC2);
	else if (p->t_optnr == T_OPTC3)
		printf("T_OPTC3 (%d)", T_OPTC3);
	else
		printf("? (0x%x)", p->t_optnr);

	printf("  t_udatap 0x%x  t_udatal ", p->t_udatap);
	if (id == T_FCONIN || id == T_FCONCF || id == T_FDISIN || id == T_FREDIN)
		/* '><' Format for udatal */
		printf("%d><%d\n", (ul >> 16) & 0xFFFF, p->t_udatal);
	else
		printf("%d\n", p->t_udatal);

	if (p->t_udatal)
		prx(cp, MIN(T_MSG_SIZE, p->t_udatal), 0, putline);

	if (p->t_optnr != T_OPTC2) {
		printf("\t\tt_xdata ");
		if (p->t_xdata == T_YES)
			printf("T_YES (%d)", T_YES);
		else if (p->t_xdata == T_NO)
			printf("T_NO (%d)", T_NO);
		else
			printf("? (0x%x) ", p->t_xdata);

		printf("  t_timeout ");
		if (p->t_timeout == T_NO)
			printf("T_NO (%d)", T_NO);
		else if (p->t_timeout == T_NOLIMIT)
			printf("T_NOLIMIT (%d)", T_NOLIMIT);
		else if (p->t_timeout > 0)
			printf("%d", p->t_timeout);
		else
			printf("? (%d)", p->t_timeout);

		if (p->t_optnr == T_OPTC3)
			printf("  t_ucepid 0x%x", p->t_ucepid);
		(void) putchar('\n');
	}
}


/*
 * propti1 - prepare t_opti1
 */
void propti1 (int *ip)
{
	struct t_opti1	*p = (struct t_opti1 *) ip;

	printf("\t   opt:\n\t\tt_optnr ");
	if (p->t_optnr == T_OPTI1)
		printf("T_OPTI1 (%d)", T_OPTI1);
	else
		if (p->t_optnr == T_OPTI2)
			printf("T_OPTI2 (%d)", T_OPTI2);
		else
			printf("? (0x%x)", p->t_optnr);

	printf("  t_maxl %d\n", p->t_maxl);
}
/*
 * propti3 - prepare t_opti3
 */
void propti3 (int *ip)
{
	struct t_opti3	*p = (struct t_opti3 *) ip;

	printf("\t   opt:\n\t\tt_optnr ");
	if (p->t_optnr == T_OPTI3)
		printf("T_OPTI3 (%d)", T_OPTI3);
	else
		printf("? (0x%x)", p->t_optnr);

	printf("\tt_maxl %d\n", p->t_maxl);
	printf("\t\tt_fdsetsize %d", p->t_fdsetsize);
	printf("\tt_openmax %d\n", p->t_openmax);
	printf("\t\tt_tsdu %d", p->t_tsdu);
	printf("\t\tt_etsdu %d\n", p->t_etsdu);
	printf("\t\tt_condata %d", p->t_condata);
	printf("\t\tt_concfdata %d\n", p->t_concfdata);
	printf("\t\tt_disdata %d", p->t_disdata);
	printf("\t\tt_adrsize %d\n", p->t_adrsize);
}

/*
 * propte1 - prepare t_opte1
 */
void propte1 (int *ip)
{
	struct t_opte1	*p = (struct t_opte1 *) ip;

	if (!vflag)
		return;

	printf("\t   opt:\n\t\tt_optnr ");
	if (p->t_optnr == T_OPTE1)
		printf("T_OPTE1 (%d)", T_OPTE1);
	else if (p->t_optnr == T_OPTE2)
		printf("T_OPTE2 (%d)", T_OPTE2);
	else
		printf("? (0x%x)", p->t_optnr);

	printf("  t_timeout ");
	if (p->t_timeout == T_NO)
		printf("T_NO (%d)", T_NO);
	else if (p->t_timeout == T_NOLIMIT)
		printf("T_NOLIMIT (%d)", T_NOLIMIT);
	else if (p->t_timeout > 0)
		printf("%d", p->t_timeout);
	else
		printf("? (%d)", p->t_timeout);
	(void) putchar('\n');
}

/*
 * propts1 - prepare t_opts1
 */
void propts1 (int	*ip)
{
	struct t_opts1	*p = (struct t_opts1 *) ip;

	if (!vflag)
		return;

	printf("\t   opt:\n\t\tt_optnr ");
	if (p->t_optnr == T_OPTS1)
		printf("T_OPTS1 (%d)", T_OPTS1);
	else
		printf("? (0x%x)", p->t_optnr);

	printf("  t_optname :  ");
	if (p->t_optname == T_DEBUG)
		printf("T_DEBUG (%d)\n", p->t_optname);
	else
		printf("? (0x%x)\n", p->t_optname);

	printf("\t   optvalue: %s\n", (char *)(&p->t_optvalue));
}

/*
 * prfdset - prepare fdset
 */
void prfdset (fd_set *mask, char *s)
{
	register int	i, j = 0;

	printf("\t\t%s:", s);
	for (i = 0; i < OPEN_MAX; i++) {
		if (FD_ISSET(i, mask)) {
			if (j && j % 16 == 0)
				printf("\n\t\t%s:", s);
			printf(" %d", i);
			j++;
		}
	}
	if (j == 0)
		printf(" none.");
	(void) putchar('\n');
}

/*
 * prsockaddr - prepare socket address
 */
void prsockaddr (struct sockaddr	*addr)
{
	register int	i;
	register u_char	*cp;

	i = addr->sa_family;
	cp = (u_char *) addr->sa_data;

	if (i == AF_UNIX) {
		printf("AF_UNIX ");
		printf("\"%.*s\"", MIN(strlen((char *)cp), sizeof (struct sockaddr)), cp);
	} else if (i == AF_INET) {
		printf("AF_INET ");

		printf("\"%d.%d.%d.%d.%d\"",
			cp[2], cp[3], cp[4], cp[5],     /* INTERNET - address */
			cp[0] << 8 | cp[1]);            /* Port number */
	} else {
		printf("UNKNOWN");
		prx(cp, sizeof (struct sockaddr), 0, putline);
	}
}

/*
 * prtpkt - preparation of TPKT from INET module in specified length
 * prthdr - preparation of TPKT header from INET module in specified length
 * prtbdy - preparation of TPKT trunk from INET module in specified length
 * TPKT comprises:
 *
 *   0  1  2  3  4  5
 * V   TPKT header   |TPKT (cont.) |
 * +--+--+-----+     |             |
 * |VR|RS|LGTH |     |             V
 * +--+--+-----+--+  V             +----------
 *             |LI|<-------------->|USER DATA
 *             +--+--+             +----------
 *                |CO|     ...
 *                +--+
 * VR vers   LGTH lgth (MSB)
 * RS rsrv   LI   length indicator
 */
#define HDRLEN	6

void prtpkt (u_char *buf, int len)
{
	int	lgth;

	lgth = (buf[2] << 8) | buf[3];

	if (lgth == 0)
		return;

	if (prthdr(buf, HDRLEN) == -1)
		return;

	lgth = (buf[2] << 8) | buf[3];
	if (lgth <= HDRLEN			
	    || lgth > len
	    || (int)(lgth - (HDRLEN - 1) - buf[4]) < 0)
		return;
	prtbdy(&buf[6], lgth - HDRLEN, buf[5], lgth - (HDRLEN - 1) - buf[4]);
}

int prthdr (u_char *buf, int	len)
{
	int	lgth;

	printf("\t\tTPKT: ");
	if (len < HDRLEN)
		return (-1);

	lgth = (buf[2] << 8) | buf[3];
	printf("vers %d  rsrv %d  lgth %d  li %d  code ",
		buf[0], buf[1], lgth, buf[4]);
	prtxt(tp0, buf[5]);
	(void) putchar('\n');
	return (buf[4]);
	printf(" UNKNOWN\n");
	prx(buf, len, 0, pktline);
	return (-1);
}

int prtbdy (u_char *buf, int len, int tpdu, int udat)
{
	int	off;
	u_char	*cp;

	printf("\t\tTPKT (cnt): ");
	if (len < 0 || udat < 0 || len <= udat) {
		printf("INVALID LENGTH FIELD\n");
		return (-1);
	}
	if (tpdu != RRTPDU && tpdu != CRTPDU && tpdu != CCTPDU
	    && tpdu != DRTPDU && tpdu != DTTPDU && tpdu != AKTPDU) {
		printf("UNKNOWN TPDU\n");
		return (-1);
	}

	if (tpdu == RRTPDU) {
		printf("pid %d  fd %d  flctl 0x%.2x  tidu %d enab %d\n",
			ntohs((* (u_short *) &buf[0])), buf[2], buf[3] & 0xFF,
			ntohs((* (u_short *) &buf[4])), buf[6]);	
		printf("\t\tcredit %d  tpdunr %d  yrtunr %d  lowe %d  upwe %d  ",
			buf[7], buf[8], buf[9], buf[10], buf[11]);
	} else if (tpdu == AKTPDU) {
		printf("next exp. tpdunr %d\n", buf[2] & 0x7f);
		return (0);	/* keine Daten tracen */
	} else if (tpdu == DTTPDU) {
		printf("tpdunr %d  eot %s (0x%.2x)\n",
			buf[0] & 0x7f, buf[0] & 0x80 ? "END" : "MORE", buf[0]);
		return (0);	/* keine Daten tracen */
	} else {
		printf("dst 0x%.4x  src 0x%.4x  ",
			(buf[0] << 8) | buf[1], (buf[2] << 8) | buf[3]);
		if (tpdu == DRTPDU) {
			printf("reas %d  ", buf[4]);
			off = len - udat - 5;
			if (off) {
				printf("%d byte of parameters\n", off);
				prx(&buf[5], off, 0, pktline);
				printf("\t\t");
			}
		} else {
			printf("class %s (0x%.2x)\n\t\t",
				buf[4] == 0 ? "TP0" : "?", buf[4]);
			cp = &buf[5];
			off = len - udat - 5;
			while (off > 0) {
				if (cp[0] == 0xc0) {
					if (cp[2] > 0x10) {
						cp[2] = cp[2]>>4;
						printf("CMX V3.0 Format: ");
					}
					else
						printf("RFC1006 Format: ");
					printf("tpdu size %d  ",
						1 << (cp[2] & 0x0F));
					cp += 3;
					off -= 3;
					continue;
				}
				if (cp[0] == 0xc1) {
					printf("\n\t\tcalling TSAP-Id:\n");
					prx(&cp[2], cp[1], 0, pktline);
					off -= (cp[1]+2);
					cp += (cp[1]+2);
					continue;
				}
				if (cp[0] == 0xc2) {
					printf("\t\tcalled TSAP-Id:\n");
					prx(&cp[2], cp[1], 0, pktline);
					off -= (cp[1]+2);
					cp += (cp[1]+2);
					continue;
				}
				printf("UNKNOWN PARAMETER (%d byte)\n", off);
				prx(cp, off, 0, pktline);
	 			cp += off;
				printf("\t\t");
				break;
			}
		}

	}

	if (udat) {
		printf("\n\t\t%d byte user data (%d traced)\n",
			udat, MIN(udat, 16));
		prx(&buf[len - udat], MIN(udat, 16), 0, pktline);
	} else
		printf("\t\tno user data\n");
	return (0);
}

/*
 * prvers - prepare CMX version
 */
char *prvers (int	v)
{
	static char	s[16];

	sprintf(s, "V%d.%d%c",
		v >> 8 & 0x0F, v >> 4 & 0x0F, ((v & 0x0F) == 0 ) ? ' ' : (v & 0x0F) + '7');

	return (s);
}
