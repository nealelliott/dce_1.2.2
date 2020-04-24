/*
 *      SCCS:  @(#)llib-ldtet.c	1.2 (92/09/30) 
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

SCCS:   	@(#)llib-ldtet.c	1.2 92/09/30
NAME:		llib-ldtet.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	dtetlib lint library

MODIFICATIONS:

************************************************************************/


/* LINTLIBRARY */

#include <stdio.h>
#include <sys/types.h>
#include "dtmac.h"
#include "dtmsg.h"
#include "ptab.h"
#include "avmsg.h"
#include "btmsg.h"
#include "dtetlib.h"
#include "ldst.h"
#include "llist.h"
#include "ltoa.h"
#include "ftoa.h"
#include "sysent.h"
#include "valmsg.h"

char Assertmsg[] = "";

int Tbuf, Tio, Tloop, Tserv, Tsyncd, Ttcc, Ttccd, Ttrace, Txresd;

int avmsg2bs(from, to) 
struct avmsg *from; 
char *to; 
{ 
	return(0); 
}

int avmsgbslen(mp)
struct avmsg *mp;
{
	return(0);
}

int bs2avmsg(from, fromlen, to, tolen) 
char *from; 
int fromlen; 
struct avmsg **to; 
int *tolen; 
{ 
	return(0); 
}

int bs2btmsg(from, fromlen, to, tolen) 
char *from; 
int fromlen; 
struct btmsg **to; 
int *tolen; 
{ 
	return(0); 
}

int bs2dtmhdr(from, to, len) 
char *from; 
struct dtmhdr *to; 
int len; 
{ 
	return(0); 
}

int bs2st(from, to, st, nst, len) 
char *from, *to; 
struct stdesc *st; 
int nst, len; 
{ 
	return(0); 
}

int bs2valmsg(from, fromlen, to, tolen) 
char *from; 
int fromlen; 
struct valmsg **to; 
int *tolen; 
{ 
	return(0); 
}

int btmsg2bs(from, to) 
struct btmsg *from; 
char *to; 
{ 
	return(0); 
}

int bufchk(bpp, lp, newlen) 
char **bpp;
int *lp, newlen; 
{ 
	return(0); 
}

int buftrace(bpp, lp, newlen, file, line) 
char **bpp, *file;
int *lp, newlen, line; 
{ 
	return(0); 
}

int dmlen2bs(from, to) 
short from; 
char *to; 
{ 
	return(0); 
}

int dofork()
{
	return(0);
}

int dogetdtablesize()
{
	return(0);
}

int domkdir(path, mode)
char *path;
{
	return(0);
}

int dormdir(path)
char *path;
{
	return(0);
}

int dowait3(statp, options)
int *statp, options;
{
	return(0);
}

int dtmhdr2bs(from, to) 
struct dtmhdr *from; 
char *to; 
{ 
	return(0); 
}

int dtputenv(s)
char *s;
{
	return(0);
}

int eaccess(path, mode)
char *path;
int mode;
{
	return(0);
}

void endsysent() 
{ 
	; 
}

char *equindex(s)
char *s;
{
	return((char *) 0);
}

char *errname(errnum) 
int errnum; 
{ 
	return((char *) 0); 
}

int fappend(fd) 
int fd; 
{ 
	return(0); 
}

int fgetargs(fp, argv, maxargs) 
FILE *fp; 
char **argv; 
int maxargs; 
{ 
	return(0); 
}

int fioclex(fd) 
int fd; 
{ 
	return(0); 
}

int fionbio(fd) 
int fd; 
{ 
	return(0); 
}

char *ftoa(fval, flags, nflags)
int fval, nflags;
struct flags *flags;
{
	return((char *) 0);
}

void generror(errnum, file, line, s1, s2) 
int errnum, line; 
char *file, *s1, *s2; 
{ 
	;
}

void genfatal(errnum, file, line, s1, s2) 
int errnum, line; 
char *file, *s1, *s2; 
{ 
	; 
}

int getargs(s, argv, maxargs) 
char *s, **argv; 
int maxargs; 
{ 
	return(0); 
}

struct ptab *getptbyptype(ptype, start) 
int ptype; 
struct ptab *start; 
{ 
	return((struct ptab *) 0); 
}

struct ptab *getptbysyspid(sysid, pid) 
int sysid;
long pid;
{ 
	return((struct ptab *) 0); 
}

struct ptab *getptbysysptype(sysid, ptype) 
int sysid, ptype; 
{ 
	return((struct ptab *) 0); 
}

struct sysent *getsysbyid(sysid) 
int sysid; 
{ 
	return((struct sysent *) 0); 
}

struct sysent *getsysent() 
{ 
	return((struct sysent *) 0); 
}

void hexdump(dp, n, fp) 
char *dp; 
int n; 
FILE *fp; 
{ 
	; 
}

void listinsert(head, elem) 
struct llist **head, *elem; 
{ 
	; 
}

void listremove(head, elem) 
struct llist **head, *elem; 
{ 
	; 
}

char *ltoa(n) 
long n; 
{ 
	return((char *) 0); 
}

char *ltoo(n) 
long n; 
{ 
	return((char *) 0); 
}

char *ltox(n) 
long n; 
{ 
	return((char *) 0); 
}

int maperrno(errnum)
int errnum;
{
	return(0);
}

int mapsignal(signum)
int signum;
{
	return(0);
}

int mkalldirs(path)
char *path;
{
	return(0);
}

int mkoptarg(s, opt, arg, first)
char *s, *arg;
char opt;
int first;
{
	return(0);
}

char *mktfname(prefix)
char *prefix;
{
	return((char *) 0);
}

void prerror(fp, errnum, hdr, file, line, s1, s2) 
FILE *fp; 
int errnum, line; 
char *hdr, *file, *s1, *s2; 
{ 
	; 
}

char *ptflags(fval) 
int fval; 
{ 
	return((char *) 0); 
}

char *ptptype(ptype) 
int ptype; 
{ 
	return((char *) 0); 
}

char *ptrepcode(rc) 
int rc; 
{ 
	return((char *) 0); 
}

char *ptreqcode(request) 
int request; 
{ 
	return((char *) 0); 
}

char *ptstate(state) 
int state; 
{ 
	return((char *) 0); 
}

char *ptsvote(state) 
int state; 
{ 
	return((char *) 0); 
}

char *remvar(s, sysid)
char *s;
int sysid;
{
	return((char *) 0);
}

char *rtoa(rp) 
struct remid *rp; 
{ 
	return((char *) 0); 
}

int setsysent() 
{ 
	return(0); 
}

int st2bs(from, to, st, nst) 
char *from, *to; 
struct stdesc *st; 
int nst; 
{ 
	return(0); 
}

char *strstore(s)
char *s;
{
	return((char *) 0);
}

char *systate(state)
int state;
{
	return((char *) 0);
}

void tdump(from, len, title) 
char *from, *title; 
int len; 
{ 
	; 
}

void tiocnotty() 
{ 
	; 
}

void trace(s1, s2, s3, s4, s5, s6) 
char *s1, *s2, *s3, *s4, *s5, *s6; 
{ 
	;
}

char **traceargs(ptype, argv) 
int ptype; 
char **argv; 
{ 
	return((char **) 0); 
}

void traceinit(argc, argv) 
int argc; 
char **argv; 
{ 
	; 
}

int unmaperrno(repcode)
int repcode;
{
	return(0);
}

int unmapsignal(signum)
int signum;
{
	return(0);
}

int valmsg2bs(from, to) 
struct valmsg *from; 
char *to; 
{ 
	return(0); 
}

