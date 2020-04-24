/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmxl.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:20  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:47:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:21  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:17:22  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:39  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:25  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:51:12  marrek]
 * 
 * $EndLog$
 */
#ifndef _CMXL_H
#define _CMXL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmxl_rcsid[] = "@(#)$RCSfile: cmxl.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:20 $";
#endif

/*
 * COPYRIGHT (C) 1988 BY SIEMENS AG  All rights reserved
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1990
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
 * Definitions for preparer of ICMX(L) trace
 */
/* #ident " @(#)cmxl.h	311.6 93/08/24 CMX (NC1)" */

#define VERSION	"4.0"

/*
 * Language dependencies
 * Default text in the case of non-existant msgcat are ENGLISH
 */

#define USAGE	catgets(catd, 4, 1, "Syntax: %s [-cdetvxD] file ...\n")
#define EOPEN	catgets(catd, 4, 2, "%s: error %d opening %s\n")
#define EBADENT	catgets(catd, 4, 3, "%s: bad first entry 0x%x\n")
#define EV22ENT	catgets(catd, 4, 4, "%s: can not process format 0x%x\n")

#define HEADLINE1	catgets(catd, 4, 5, " ICMX(L) TRACE (%c-V%s)\t\t\t\t\t%.15s\n")
#define HEADLINE21	catgets(catd, 4, 6, " OPTIONS '")
#define HEADLINE22	catgets(catd, 4, 7, "' TRACE FILE '%s' ICMX(L) %s")
#define HEADLINE23	catgets(catd, 4, 8, " DATA LENGTH %d\n\n")

struct txt {
	int	val;
	char	*str;
};

extern struct txt	kops[], reasons[];

#ifndef	u_char
#define	u_char	unsigned char
#endif

void prtxt (struct txt	*table, int entry);
void propta1 (int *ip);
void prmn (struct t_myname *p);
void proptc1 (int id,int *ip, int ul, char *cp);
void praddr (struct t_partaddr *p);
void propts1 (int *ip);
char *prerr (int err);
void prevt(int evt, int tref);
void prsockaddr (struct sockaddr *addr);
void prtpkt (u_char *buf, int len);
int  prthdr (u_char *buf, int	len);
int  prtbdy (u_char *buf, int len, int tpdu, int udat);
void prfdset (fd_set *mask, char *s);
void propti1 (int *ip);
void propti3 (int *ip);
void propte1 (int *ip);
char *priocmd (int entry);
char *hms30 (short secs, short mils);
char *hms25 (short secs, short mils);
char *prvers (int v);

extern int	vflag;
extern long	strt;
extern int	prx(void *, int, int, void (*)(char *));
void putline (char *l);
void pktline (char *l);

#endif /* _CMXL_H */
