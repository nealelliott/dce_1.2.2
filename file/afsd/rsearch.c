/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsearch.c,v $
 * Revision 1.1.45.1  1996/10/02  16:57:02  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:26  damon]
 *
 * Revision 1.1.39.3  1994/08/01  17:46:19  sasala
 * 	Merged with changes from 1.1.39.2
 * 	[1994/08/01  17:46:14  sasala]
 * 
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  15:07:45  sasala]
 * 
 * 	Implement partial S12Y (internationalization) for DFS commands and daemons
 * 	[1994/07/21  14:51:09  sasala]
 * 
 * Revision 1.1.39.2  1994/06/09  13:49:42  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:23:13  annie]
 * 
 * Revision 1.1.39.1  1994/02/04  20:03:58  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:05:36  devsrc]
 * 
 * Revision 1.1.37.1  1993/12/07  17:11:00  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:40:07  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/18  19:49:16  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:22:27  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  15:24:27  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:03:41  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:36:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1989 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* cptres_search: search for a domain given a query type.
 * (Useful with Bind distributions predating bind 4.8.)
 * Returns the query-buffer in which answers are to be found.
 * Uses the same searching strategy as gethostbyname().
 * Thus, this routine can be used to bind host names or
 * search for MX records for a domain name.
 */

#include <dcedfs/param.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#ifndef AFS_RESOLVER_ENV
#define AFS_RESOLVER_ENV 1
#endif /* AFS_RESOLVER_ENV */

#if AFS_RESOLVER_ENV
#include <sys/param.h>
#include <netinet/in.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <strings.h>
#undef	r_type		/* XXXX */
#include <arpa/nameser.h>
#include <resolv.h>

#include <dce/dce.h>
#include <dce/dcesvcmsg.h>
#include <dfsdsdmac.h>
#include <dfsdsdsvc.h>
#include <dfsdsdmsg.h>
 
static int oneTry(dom, sfx, class, type, answer, anslen)
char *dom, *sfx;		/* domain name and suffix */
int class, type;		/* class and type of query */
char *answer;		/* buffer to put answer */
int anslen;		/* size of answer */
{
    char buf[PACKETSZ], tryname[2*MAXDNAME + 1];
#ifndef	AFS_AIX31_ENV
    extern char *strcpy();
#endif
    int n;
#ifdef RES_DEFNAMES
    unsigned long oldDefn;
#endif /* RES_DEFNAMES */

    if (!(_res.options & RES_INIT))
	if (res_init() == -1)
	    return(-1);
#ifdef DEBUG
    if (_res.options & RES_DEBUG)
	dce_svc_printf(DSD_S_SEARCH_MSG);
#endif /* DEBUG */
    /* append to form a new name to lookup */
    strcpy(tryname, dom);
    if (sfx != NULL) {
	strcat(tryname, ".");
	strcat(tryname, sfx);
    }
#ifdef RES_DEFNAMES
    /* Keep res_mkquery from appending anything silly here. */
    oldDefn = _res.options & RES_DEFNAMES;
    _res.options &= ~RES_DEFNAMES;
#endif /* RES_DEFNAMES */
#ifdef DEBUG
    if (_res.options & RES_DEBUG)
	dce_svc_printf(DSD_S_TRYNAME_MSG, tryname);
#endif /* DEBUG */
    n = res_mkquery(QUERY, tryname, class, type, (char *)NULL, 0, NULL, buf, sizeof(buf));
#ifdef RES_DEFNAMES
    _res.options |= oldDefn;
#endif /* RES_DEFNAMES */

    if (n <= 0) {
#ifdef DEBUG
	if (_res.options & RES_DEBUG)
	    dce_svc_printf(DSD_S_BADMKQUERY_MSG);
#endif /* DEBUG */
	return (n);
    }
    n = res_send(buf, n, answer, anslen);
    if (n < 0) {
#ifdef DEBUG
	if (_res.options & RES_DEBUG)
	    dce_svc_printf(DSD_S_BADSEND_MSG);
#endif /* DEBUG */
	return (n);
    }
    return(n);
}

/* For each of a sequence of queries, form a query, send it, await a reply.
   Always use a QUERY operation, no extra data, no extra resource records.
   Return the size of the response, or -1 for errors. */

cptres_search(name, class, type, answer, anslen)
char *name;		/* domain name */
int class, type;		/* class and type of query */
char *answer;		/* buffer to put answer */
int anslen;		/* size of answer */
{
    register char *cp;
#ifdef RES_DNSRCH
    register char **domain;
#endif /* RES_DNSRCH */
    register HEADER *hp;
#ifndef	AFS_AIX31_ENV
    extern char *strcpy();
#endif
    int n, NumDots;

    if (!(_res.options & RES_INIT))
	if (res_init() == -1)
	    return(-1);
#ifdef DEBUG
    if (_res.options & RES_DEBUG)
	dce_svc_printf(DSD_S_SEARCH2_MSG);
#endif /* DEBUG */
    errno = 0;
    for (cp = name, NumDots = 0; *cp; cp++)
	if (*cp == '.')
	    NumDots++;
    if ((NumDots && cp[-1] == '.') || (_res.options & RES_DEFNAMES) == 0) {
	if (cp[-1] == '.')
	    cp[-1] = 0;
	n = oneTry(name, (char *)NULL, class, type, answer, anslen);
	if (cp[-1] == 0)
	    cp[-1] = '.';
	return (n);
    }
    n = -1;
#ifdef RES_DNSRCH
    /* Try all in the domain name search list if we're running a bind recent enough to support it.
        Otherwise just search the default domain. */
    if (_res.options & RES_DNSRCH)
	for (domain = _res.dnsrch; *domain; domain++) {
	    n = oneTry(name, *domain, class, type, answer, anslen);
	    if (n <= 0) return(n);
	    hp = (HEADER *) answer;
	    if (hp->rcode != NXDOMAIN || ! hp->aa) return (n);
	}
#else /* RES_DNSRCH */
    n = oneTry(name, _res.defdname, class, type, answer, anslen);
    if (n <= 0) return(n);
    hp = (HEADER *) answer;
    if (hp->rcode != NXDOMAIN || ! hp->aa) return (n);
#endif /* RES_DNSRCH */
    /* For AFS use, try even an undotted single-component name. */
    /* if (NumDots == 0) return (n); */
    return (oneTry(name, (char *)NULL, class, type, answer, anslen));
}

#else /* AFS_RESOLVER_ENV */

cptres_search(name, class, type, answer, anslen)
char *name;		/* domain name */
int class, type;		/* class and type of query */
char *answer;		/* buffer to put answer */
int anslen;		/* size of answer */
{
    return (-1);
}
#endif /* AFS_RESOLVER_ENV */
