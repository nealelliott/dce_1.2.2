/*
 *      SCCS:  @(#)dtetlib.h	1.3 (92/10/06) 
 *
 *	UniSoft Ltd., London, England
 *
 * (C) Copyright 1992 X/Open Company Limited
 *
 * All rights reserved.  No part of this source code may be reproduced,
 * stored in a retrieval system, or transmitted, in any form or by any
 * means, electronic, mechanical, photocopying, recording or otherwise,
 * except as stated in the end-user licence agreement, without the prior
 * permission of the copyright owners.
 *
 * X/Open and the 'X' symbol are trademarks of X/Open Company Limited in
 * the UK and other countries.
 */

/************************************************************************

SCCS:   	@(#)dtetlib.h	1.3 92/10/06 DTET release 1.0b
NAME:		dtetlib.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations for extern dtetlib functions not declared in other
	header files

MODIFICATIONS:

************************************************************************/


#if __STDC__

extern int bufchk(char **, int *, int);
extern int buftrace(char **, int *, int, char *, int);
extern int dofork();
extern int dogetdtablesize();
extern int domkdir(char *, int);
extern int dormdir(char *);
extern int dowait3(int *, int);
extern int dtputenv(char *);
extern int tet_eaccess(char *, int);
extern char *equindex(char *);
extern char *errname(int);
extern int fappend(int);
extern int fgetargs(FILE *, char **, int);
extern int fioclex(int);
extern int fionbio(int);
extern void generror(int, char *, int, char *, char *);
extern void genfatal(int, char *, int, char *, char *);
extern int getargs(char *, char **, int);
extern void hexdump(char *, int, FILE *);
extern int maperrno(int);
extern int mapsignal(int);
extern int mkalldirs(char *);
extern int mkoptarg(char *, int, char *, int);
extern char *mktfname(char *);
extern void prerror(FILE *, int, char *, char *, int, char *, char *);
extern char *ptflags(int);
extern char *ptptype(int);
extern char *ptrepcode(int);
extern char *ptreqcode(int);
extern char *ptstate(int);
extern char *ptsvote(int);
extern char *remvar(char *, int);
extern char *strstore(char *);
extern char *systate(int);
extern void tiocnotty();
extern int unmaperrno(int);
extern int unmapsignal(int);

#else /* __STDC__ */

extern int bufchk();
extern int buftrace();
extern int dofork();
extern int dogetdtablesize();
extern int domkdir();
extern int dormdir();
extern int dowait3();
extern int dtputenv();
extern int tet_eaccess();
extern char *equindex();
extern char *errname();
extern int fappend();
extern int fgetargs();
extern int fioclex();
extern int fionbio();
extern void generror();
extern void genfatal();
extern int getargs();
extern void hexdump();
extern int maperrno();
extern int mapsignal();
extern int mkalldirs();
extern int mkoptarg();
extern char *mktfname();
extern void prerror();
extern char *ptflags();
extern char *ptptype();
extern char *ptrepcode();
extern char *ptreqcode();
extern char *ptstate();
extern char *ptsvote();
extern char *remvar();
extern char *strstore();
extern char *systate();
extern void tiocnotty();
extern int unmaperrno();
extern int unmapsignal();

#endif /* __STDC__ */

