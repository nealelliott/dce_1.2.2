/*
 *      SCCS:  @(#)llib-lserv.c	1.2 (92/09/30) 
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

SCCS:   	@(#)llib-lserv.c	1.2 92/09/30
NAME:		llib-lserv.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	servlib lint library

MODIFICATIONS:

************************************************************************/


/* LINTLIBRARY */

#include <sys/types.h>
#include "dtmsg.h"
#include "ptab.h"
#include "synreq.h"
#include "servlib.h"

struct ptab *Ptab;

int sd_errno;
int tc_errno;
int xd_errno;

char Tcerrmsg[] = "";

void fiodead(pp)
struct ptab *pp;
{
	;
}

void op_fclose(pp)
struct ptab *pp;
{
	;
}

void op_fopen(pp)
struct ptab *pp;
{
	;
}

void op_gets(pp)
struct ptab *pp;
{
	;
}

void op_puts(pp)
struct ptab *pp;
{
	;
}

void ptadd(pp) 
struct ptab *pp; 
{ 
	; 
}

struct ptab *ptalloc() 
{ 
	return((struct ptab *) 0); 
}

void ptfree(pp) 
struct ptab *pp; 
{ 
	;
}

void ptrm(pp) 
struct ptab *pp; 
{ 
	; 
}

int sd_masync(snid, xrid, spno, vote, timeout, synreq, nsys)
long snid, xrid, spno;
int vote, timeout, *nsys;
struct synreq *synreq;
{
	return(0);
}

int sd_sasync(snid, spno, vote, timeout, synreq, nsys, xridp)
long snid, spno, *xridp;
int vote, timeout, *nsys;
struct synreq *synreq;
{
	return(0);
}

int sd_logoff(stayopen)
int stayopen;
{
	return(0);
}

int sd_logon()
{
	return(0);
}

char *sd_msgbuf(len)
int len;
{
	return((char *) 0);
}

long sd_snget()
{
	return(0L);
}

int sd_snsys(snid, snames, nsname)
long snid;
int *snames, nsname;
{
	return(0);
}

char *sd_talk(req, delay)
int req, delay;
{
	return((char *) 0);
}

int sd_usync(snid, xrid, spno, vote, timeout, synreq, nsys)
long snid, xrid, spno;
int vote, timeout, nsys;
struct synreq *synreq;
{
	return(0);
}

void si_clientloop(pp, delay)
struct ptab *pp;
int delay;
{
	;
}

void si_forkdaemon() 
{ 
	;
}

int si_main(argc, argv, needtetroot) 
int argc, needtetroot; 
char **argv; 
{ 
	return(0); 
}

void si_serverloop() 
{ 
	; 
}

void si_serverproc(pp) 
struct ptab *pp; 
{ 
	;
}

void si_service(pp) 
struct ptab *pp; 
{ 
	;
}

void si_servwait(pp, timeout) 
struct ptab *pp; 
int timeout; 
{ 
	;
}

void so_dead(pp) 
struct ptab *pp; 
{ 
	; 
}

int tc_access(sysid, path, mode)
int sysid, mode;
char *path;
{
	return(0);
}

int tc_cfname(sysid, cfname)
int sysid;
char *cfname;
{
	return(0);
}

int tc_chdir(sysid, dir)
int sysid;
char *dir;
{
	return(0);
}

int tc_configv(sysid, lines, nline)
int sysid, nline;
char **lines;
{
	return(0);
}

int tc_fclose(sysid, fid)
int sysid, fid;
{
	return(0);
}

int tc_fopen(sysid, file)
int sysid;
char *file;
{
	return(0);
}

int tc_kill(sysid, pid, signum)
int sysid, signum;
long pid;
{
	return(0);
}

int tc_lockfile(sysid, file, timeout)
int sysid, timeout;
char *file;
{
	return(0);
}

int tc_logoff(sysid)
int sysid;
{
	return(0);
}

int tc_logon(sysid)
int sysid;
{
	return(0);
}

long tc_mexec(sysid, path, argv, outfile)
int sysid;
char *path, **argv, *outfile;
{
	return(0L);
}

int tc_mkdir(sysid, dir)
int sysid;
char *dir;
{
	return(0);
}

char *tc_mksdir(sysid, dir, suffix)
int sysid;
char *dir, *suffix;
{
	return((char *) 0);
}

char *tc_mktmpdir(sysid, dir)
int sysid;
char *dir;
{
	return((char *) 0);
}

char *tc_msgbuf(sysid, len)
int sysid, len;
{
	return((char *) 0);
}

int tc_putenv(sysid, env)
int sysid;
char *env;
{
	return(0);
}

int tc_putenvv(sysid, envp, nenv)
int sysid, nenv;
char **envp;
{
	return(0);
}

int tc_puts(sysid, fid, line)
int sysid, fid;
char *line;
{
	return(0);
}

int tc_putsv(sysid, fid, lines, nline)
int sysid, fid, nline;
char **lines;
{
	return(0);
}

char **tc_rcvconfv(sysid, nlines, done)
int sysid, *nlines, *done;
{
	return((char **) 0);
}

int tc_rmdir(sysid, dir)
int sysid;
char *dir;
{
	return(0);
}

int tc_rxfile(sysid, from, to)
int sysid;
char *from, *to;
{
	return(0);
}

int tc_sndconfv(sysid, lines, nline)
int sysid, nline;
char **lines;
{
	return(0);
}

char *tc_sharelock(sysid, lockdir, timeout)
int sysid, timeout;
char *lockdir;
{
	return((char *) 0);
}

int tc_sysname(sysid, snames, nsname)
int sysid, *snames, nsname;
{
	return(0);
}

char *tc_talk(sysid, req, delay)
int sysid, req, delay;
{
	return((char *) 0);
}

long tc_texec(sysid, path, argv, outfile, snid, xrid)
int sysid;
char *path, **argv, *outfile;
long snid, xrid;
{
	return(0L);
}

int tc_tslfiles(sysid, files, nfile, subdir)
int sysid, nfile;
char **files, *subdir;
{
	return(0);
}

int tc_tsmfiles(sysid, files, nfile, subdir)
int sysid, nfile;
char **files, *subdir;
{
	return(0);
}

long tc_uexec(sysid, path, argv, snid, xrid)
int sysid;
char *path, **argv;
long snid, xrid;
{
	return(0L);
}

int tc_unlink(sysid, file)
int sysid;
char *file;
{
	return(0);
}

int tc_wait(sysid, pid, timeout, statp)
int sysid, timeout, *statp;
long pid;
{
	return(0);
}

int ti_logoff(pp, stayopen)
struct ptab *pp;
int stayopen;
{
	return(0);
}

int ti_logon(pp)
struct ptab *pp;
{
	return(0);
}

char *ti_msgbuf(pp, newlen)
struct ptab *pp;
int newlen;
{
	return((char *) 0);
}

int ti_talk(pp, delay)
struct ptab *pp;
int delay;
{
	return(0);
}

int ti_tcmputenv(psysid, snid, xrid, sname, nsname)
int psysid, *sname, nsname;
long snid, xrid;
{
	return(0);
}

int xd_cfname(ecfname, dcfname, ccfname)
char *ecfname, *dcfname, *ccfname;
{
	return(0);
}

int xd_fclose(fid)
int fid;
{
	return(0);
}

int xd_fopen(fname)
char *fname;
{
	return(0);
}

char *xd_gets(fid)
int fid;
{
	return((char *) 0);
}

char **xd_getsv(fid, nlines, eof)
int fid, *nlines, *eof;
{
	return((char **) 0);
}

int xd_icend(xrid)
long xrid;
{
	return(0);
}

int xd_icstart(xrid, icno, activity, tpcount)
long xrid, activity;
int icno, tpcount;
{
	return(0);
}

int xd_logoff()
{
	return(0);
}

int xd_logon()
{
	return(0);
}

char *xd_msgbuf(len)
int len;
{
	return((char *) 0);
}

char **xd_rcfname()
{
	return((char **) 0);
}

int xd_result(xrid, result)
long xrid;
int result;
{
	return(0);
}

char *xd_talk(req, delay)
int req, delay;
{
	return((char *) 0);
}

int xd_tpend(xrid)
long xrid;
{
	return(0);
}

int xd_tpstart(xrid, tpno)
long xrid;
int tpno;
{
	return(0);
}

int xd_xfile(from, to)
char *from, *to;
{
	return(0);
}

int xd_xres(xrid, line)
long xrid;
char *line;
{
	return(0);
}

int xd_xresv(xrid, lines, nlines)
long xrid;
char **lines;
int nlines;
{
	return(0);
}

long xd_xropen(xfname)
char *xfname;
{
	return(0L);
}

int xd_xrsys(xrid, snames, nsname)
long xrid;
int *snames, nsname;
{
	return(0);
}

