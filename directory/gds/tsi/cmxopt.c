/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxopt.c,v $
 * Revision 1.1.5.2  1996/03/11  13:24:26  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:49  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:48:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:31  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:36  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:22:21  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:32  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:45  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:30  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:26  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cmxopt.c,v $ $Revision: 1.1.5.2 $ $Date: 1996/03/11 13:24:26 $";
#endif

/*
 * COPYRIGHT (C) 1992 BY SIEMENS AG All rights reserved
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
/*
static char sccsid[] = "@(#)cmxopt.c	311.15 93/08/24 CMX (NC1)";
*/

#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <tnsx.h>
#include <sys/types.h>
#include "cmx.h"
#include <cmx_addr.h>
#include "cmxtrace.h"
#include "cmxextern.h"
#include <netinet/in.h>
#include <sys/uio.h>
#include "cmxinet.h"

/*
 * Defines of environment variable and configuration file
 */
#define E_CMXTRACE "CMXTRACE="
#define E_CMXINIT "CMXINIT="
#define E_CMXSOCKET "CMXSOCKET="

#define CTRL_FILE "FILE_CONFIG"

/*
 * Declaration of internal functions called by t_getopt or by t_setopt
 */
int	t_getopt(int), t_opttrace(char *, int), t_optinit(char *);
int	t_optsocket(char *);

/*
 * Function t_getopt
 * gets options CMXTRACE CMXINIT CMXSOCKET from the
 * environment variables. If the environment variable is not set
 * the controlfile FILE_CONFIG will be read. If there is no information
 * in it, no option will be set.
 */
int
t_getopt(int trcdflt)
{

	char *linebuf;
	char *e_cmxtrace; 	/* environment variable CMXTRACE */
	char *e_cmxinit; 	/* environment variable CMXINIT */
	char *e_cmxsocket; 	/* environment variable CMXSOCKET */
	static int first=0;     /* check only one times */

	if (first)
		return(0);
	first = 1;


	e_cmxtrace = (char *)getenv("CMXTRACE");
	e_cmxinit = (char *)getenv("CMXINIT");
	e_cmxsocket = (char *)getenv("CMXSOCKET");
	
	if (((e_cmxtrace   == NULL) || (strlen(e_cmxtrace)   == 0)) ||
	    ((e_cmxinit == NULL) || (strlen(e_cmxinit) == 0)) ||
	    ((e_cmxsocket == NULL) || (strlen(e_cmxsocket) == 0))) { 
		/*
		 * Either CMXTRACE, CMXINIT or CMXSOCKET
		 * environment variables not set
		 */
		FILE 	* fp;
		if ((fp = fopen(CTRL_FILE, "r")) != NULL) {   
		    while (fgets(linebuf, BUFSIZ -1, fp) != NULL) {
			while (linebuf == (char *)' ')
				linebuf++;
			if ((!e_cmxtrace) &&
			    (strncmp(linebuf, E_CMXTRACE,
					sizeof(E_CMXTRACE)-1)==0))
				e_cmxtrace=linebuf+sizeof(E_CMXTRACE)-1;
			if ((!e_cmxinit) &&
			    (strncmp(linebuf, E_CMXINIT,
					sizeof(E_CMXINIT)-1)==0))
				e_cmxinit=linebuf+sizeof(E_CMXINIT)-1;
			if ((!e_cmxsocket) &&
			    (strncmp(linebuf, E_CMXSOCKET,
					sizeof(E_CMXSOCKET)-1)==0))
				e_cmxsocket=linebuf+sizeof(E_CMXSOCKET);
		    }
		    fclose(fp);
		}
	}
	
	/*
	 * call t_opt-functions according to environment variable
   	 * or entry in file
	 */

	if ((e_cmxtrace != NULL) && (strlen(e_cmxtrace) > (size_t) 0))
		 t_opttrace(e_cmxtrace,trcdflt);
	if ((e_cmxinit != NULL) && (strlen(e_cmxinit) > (size_t) 0))
		 t_optinit(e_cmxinit);
	if ((e_cmxsocket != NULL) && (strlen(e_cmxsocket) > (size_t) 0))
		 t_optsocket(e_cmxsocket);
	
	return(0);
}

/* Function t_setopt sets and resets options
 * t_setopt is a function of ICMX(L)
 */
int t_setopt(int component, t_opts *opt) 
{

	extern int t_trcpid;

	t_trc(T_FSETOPT, 0, component, opt);

	if ((!opt) ||
	    (opt->opts1.t_optnr != T_OPTS1)) {
		_t_error = T_WPARAMETER;
		t_trc(T_FRETERR, _t_error);
		return(T_ERROR);
	}
	switch (component) {
	case T_LIB:
		switch (opt->opts1.t_optname) {
		case T_DEBUG:
			if (strlen(opt->opts1.t_optvalue) > (size_t)0) {
				/* set trace */
				if (t_trcpid == -1) {
					t_opttrace(opt->opts1.t_optvalue,T_TRDFLT);
					t_trc(T_FSETOPT, 0, component, opt);
				}
				else
					t_opttrace(opt->opts1.t_optvalue,T_TRDFLT);
			}
			else
				/* reset trace */
				t_trcoff(T_TRFLSH);
			t_trc (T_FRETOK, T_FSETOPT, T_OK);
			return(T_OK);
			/* NOTREACHED */
		default:
			_t_error = T_WPARAMETER;
			t_trc(T_FRETERR, _t_error);
			return(T_ERROR);
			/* NOTREACHED */
		}
	default:
		_t_error = T_WPARAMETER;
		t_trc(T_FRETERR, _t_error);
		return(T_ERROR);
		/* NOTREACHED */
	}
}

/*
 * Function t_opttrace sets trace options.
 *
 * The TRACE is controlled by the CMXTRACE environment variable:
 *
 *	CMXTRACE="[-sSDT] [-p fac] [-r fac] [-d len] [-G dirx] [-f dir]"
 *      D       Special debug for interconnection during an attach/detach/event
 *      T       Special debug for TCP/IP socket trace (includes option D)
 *      s       Usual TRACE (calls, arguments, addresses)
 *      S       Complete TRACE (calls, arguments, addresses, options, data)
 *      d len   Data lengths to be logged, default: T_TRLEN
 *      p fac   fac * BUFSIZ is the size of the data buffer. Default: T_TRFAC.
 *      r wrap  After fac * BUFSIZ the switch is made to the secondary file.
 *		Default: T_TRWRAP
 *      G dirx  Special debug for DIR.X accesses
 *      f dir   Directory for trace files
 *
 */
	int dx_trace=0;
int
t_opttrace(char *p_trace, int trcdflt)
{
	extern struct t_trcflag t_trcflg;
	extern int t_trclen, t_trcfac, t_trcwrap;
	int i;
	extern char t_trcdir[T_TRCDIR_SIZE];

	dx_trace = 0;
	if (p_trace == NULL) {
		if (trcdflt == T_TRNDFLT) {
			t_trcflg.sel = T_TRDONOT;
			return (T_ERROR);
		} else
			p_trace = "-S";
	}
	FPRINTF(stderr, "CMXTRACE == '%s'\n", p_trace);

	t_trcflg.deb = T_LIBDEBUG;
	t_trcfac = T_TRFAC;
	t_trcflg.sel = T_TRVERBOSE;
	t_trclen = T_TRLEN;
	t_trcwrap = T_TRWRAP;
	t_trcdir[0] = '\0';

	while (*p_trace) {
		switch (*p_trace++) {
		case '-':
		case ' ':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			break;

		case 's':  
			t_trcflg.sel = T_TRCOMPACT;
			break;

		case 'S':
			t_trcflg.sel = T_TRVERBOSE;
			break;

		case 'd':
			t_trclen = atoi(p_trace);
			if (t_trclen < 0)
				t_trclen = T_TRLEN;
			t_trclen &= 0xFFFF;
			break;
			
		case 'p':
			t_trcfac = atoi(p_trace);
			if (t_trcfac < 0 || t_trcfac > T_TRFAC)
				t_trcfac = T_TRFAC;
			break;

		case 'D':  
			t_trcflg.deb = T_LIBDEBUG;
			break;

		case 'T':  
			t_trcflg.deb = T_TCPDEBUG+T_LIBDEBUG;
			break;

		case 'r':
			t_trcwrap = atoi(p_trace) * BUFSIZ;
			if (t_trcwrap <= 0)
				t_trcwrap = T_TRWRAP;
			break;

		case 'G':
			dx_trace = atoi(p_trace);
			if (dx_trace < 0)
				dx_trace = 0;
			break;

		case 'f':
			while (*p_trace == ' ' || *p_trace == '\t')
				/* skip spaces and tabs at beginning */
				p_trace++;
			i=0;
			while (*p_trace && i < T_TRCDIR_SIZE && *p_trace != ' ' && *p_trace != '\t') {
				/* copy name of directory */
				t_trcdir[i++] = *p_trace;
				p_trace++;
			}
			if (i < T_TRCDIR_SIZE)
				t_trcdir[i]='\0';
			else
				FPRINTF(stderr, "size of directoryname too long",1 /* just to avoid HPUX errors */);
			break;

		default:
			FPRINTF(stderr, "t_trcon: bad char '%c'\n", *(p_trace - 1));
			break;
		}
	}
}

/*
 * Function t_optinit sets init options.
 *
 * Initialization is controlled using the environment variable CMXINIT:
 *
 *	CMXINIT="[-s] [-S sig] [-t size] [-f] [-c cc]"
 *   s        sets SIGIO
 *   S sig    sets sig as a signal
 *   c cc     sets ROUTINGINFORMATION. cc is a CC list in the form
 *            "Wi" or "Lj", with non-empty subsets i and j from
 *            {1, 2, 3, 4, 5, 6}, e.g. "W135"
 *   t size   defines TPDU size when establishing a connection via LAN1
 *   f        turns off the flow control via LAN1 (to CMX V3.0 and V2.5)
 *            by means of AK-TPDUs.
 */
int
t_optinit(char *p_init)
{

	extern  int	tpdu, credit, t_cmxsig;
	extern 	Ts_route	envrout;
	static 	int 	i;
	static 	char	c;
	static  int     rout = 0;               /* no ROUTINGINFORMATION */
	static	short wlist[] = { 0, T_W1, T_W2, T_W3, T_W4, T_W5, T_W6 };
	static	short llist[] = { 0, T_L1, T_L2, T_L3, T_L4, T_L5, T_L6 };

	while (*p_init) {
		switch (*p_init++) {
		case '-':
		case ' ':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			continue;

		case 't':		/* TPDU size */
			i = atoi(p_init);
			switch (i) {
			case 4096:
				tpdu = TC_TPDU_4096;
				break;

			case 2048:
				tpdu = TC_TPDU_2048;
				break;

			case 512:
				tpdu = TC_TPDU_512;
				break;

			case 256:
				tpdu = TC_TPDU_256;
				break;

			case 128:
				tpdu = TC_TPDU_128;
				break;

			default:
				tpdu = TC_TPDU_1024;
				break;
			}
			if (credit != 0) MAP_CREDIT(tpdu);
			t_tidusize = tpdu;
			continue;

		case 'f':               /* No explicit flow control */
			credit = 0;
			break;

		case 's':               /* CMX signalling */
			t_cmxsig = SIGIO;
			break;

		case 'S':               /* CMX signalling */
			t_cmxsig = atoi(p_init);
			if (t_cmxsig <= 0 || t_cmxsig > NSIG)
				t_cmxsig = 0;
			break;
	
		case 'c':               /* e.g. "-c W135" */
			while (*p_init == ' ' || *p_init == '\t')
				p_init++;	/* skip ' ' und '\t' */
			if (*p_init == 'L') {
				p_init++;
				while ((c = *p_init++) > '0' && c <= '6')
					rout |= llist[c - '0'];
				envrout = (Ts_route) rout;
			}
			if (*p_init == 'W') {
				p_init++;
				while ((c = *p_init++) > '0' && c <= '6')
					rout |= wlist[c - '0'];
				envrout = (Ts_route) rout;
			}
		}
	}

	return(0);
}

/*
 * Function t_optsocket sets socket options
 *
 * Initialization is controlled by the environment variable CMXSOCKET
 *
 *	CMXSOCKET="[-K keep] [-L linger]"
 *   keep    timer for SO_KEEPALIVE [sec]
 *   linger  timer for SO_LINGER [sec]
 *
 */

int
t_optsocket(char *p_socket)
{
	extern	int	sock_keep, sock_linger;
 
	while (*p_socket) {
		switch (*p_socket++) {
		case '-':
		case ' ':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			continue;

		case 'K':
			sock_keep = atoi(p_socket);
			break;

		case 'L':
			sock_linger = atoi(p_socket);
			break;
		default:
			FPRINTF(stderr, "t_trcsock: bad char '%c'\n", *(p_socket - 1));
		}
	}
	return(0);
}
