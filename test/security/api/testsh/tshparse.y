%{
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tshparse.y,v $
 * Revision 1.1.9.2  1996/03/11  02:35:48  marty
 * 	Update OSF copyright years
 * 	[1996/03/10  20:04:10  marty]
 *
 * Revision 1.1.9.1  1995/12/11  21:50:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:21  root]
 * 
 * Revision 1.1.4.4  1992/12/31  17:35:22  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:59:33  htf]
 * 
 * Revision 1.1.4.3  1992/11/04  14:23:45  cuti
 * 	5912: Unable to compile under HP-UX ANSI=c compiler. Add #ifdef clause.
 * 	[1992/11/03  20:12:02  cuti]
 * 
 * Revision 1.1.4.2  1992/09/29  21:24:53  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge UPDATE: Use execve() instead of execvp() for all
 * 	           systems not just SVR4
 * 	[1992/09/24  14:25:07  sekhar]
 * 
 * 	[OT 5373]   SNI/SVR4 Merge.
 * 	            Also added OSF copyright marker and history log within
 * 	[1992/09/15  15:26:51  sekhar]
 * 
 * $EndLog$
 */
%}
/*
*/
%{
/*
 * tshparse.y - grammer for tsh
 * 
 * Copyright 1989 by Apollo Computer, Inc.
 * Copyright 1990 by Locus Computing Corp.
 * 
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Apollo Computer, Inc not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Apollo Computer, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 * 
 * @(#)tshparse.y	1.14
 *  
 *	Extracted	1/22/91 11:04:36 
 * 	from delta on	1/17/91 16:03:45
 *
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
#ifndef LINT
static char sccs_id[] = "@(#)tshparse.y	1.14";
#endif

#include <tsh.h>
#include	<errno.h>
#include	<fcntl.h>
#if defined(SYSV) || defined(SNI_SVR4_POSIX)
#include 	<malloc.h>
#include	<unistd.h>
#include	<limits.h>
#if defined(SNI_SVR4_POSIX)
#include	<sys/param.h>
#endif /* SNI_SVR4_POSIX */
#else
#include	<sys/param.h>
#include	<sys/file.h>
extern char *	malloc();
#endif /* SYSV || SNI_SVR4_POSIX */
extern char **	environ;
extern int	errno;
extern char *	sys_errlist[];
extern int	sys_nerr;
#if defined(YYDEBUG) && defined(SYSV)
int yydebug = 1;
#endif
extern int	errorflag;
extern int	errdefault;
extern FILE *	yyin;
extern FILE *	logfile;
extern int	outputmask;
extern int	scroutput;
extern searchlist_t	searcher;
extern int	verbose;
extern char *	TshFileName;

extern int	yylineno;
#ifdef SYSV
# define	TSHPATHLEN	PATH_MAX
#else
# define	TSHPATHLEN	MAXPATHLEN
#endif
#define	ARGSTART	4		/*the start of arg processing in */

#ifndef TRUE
#define	TRUE	1
#ifdef FALSE
#undef FALSE
#endif
#define	FALSE	0
#endif
					/* the arguments array */
char *		arguments[120] = {0};	/* ptr to argv */
int		argcnt = ARGSTART;
int		failcnt = 0;
int		passcnt = 0;
int		subfailcnt = 0;
int		subpasscnt = 0;

char **         s_envp;
%}

%union {
	char *	cp;
	char ** cpp;
	int	i;
}
%token	ECHOCMD
%token	<cp> ECHOSTR
%token	INCLUDE
%token	EXECUTE
%token	TEST
%token	RUN
%token	REMOTE
%token	ONERROR
%token	SETCMD
%token	READ
%token	<i> DEFAULT
%token	<i> STOP
%token	<i> CONTINUE
%token	SUMMARY
%token	SUBTOTAL
%token	<i> CLEAR
%token	<cp> STRING
%token	CR
%token	PAUSE
%token	UNSET
%type	<i> errcmd
%type	<i> subcmd
%%
command: /* empty Set */
	| command keyword 

keyword	: INCLUDE STRING CR
	{
		if(verbose)
			vlog("include %s\n",$2);
		doinclude($2);
	}
	| EXECUTE STRING CR
	{
		int	saveflag;

		if(verbose)
			vlog("execute %s\n",$2);
		saveflag = errorflag;
		doinclude($2);
		errorflag = saveflag;
	}
	| TEST args CR
	{
		if(verbose)
			vlog("test");
		if(runprog(-1)) {
			failcnt++;
			subfailcnt++;
			if(errorflag) {
				subtotal(0);
				summary();
				exit(-1);
			}
		}
		else {
			passcnt++;
			subpasscnt++;
		}

	}
	| RUN args CR
	{
		if(verbose)
			vlog("run");
		runprog(0);
	}
	| REMOTE args CR
	{
		if(verbose)
			vlog("remote");
		runprog(1);
	}
	| ONERROR errcmd CR
	{
		errorflag = $2;
	}
	| SUMMARY CR
	{
		if(verbose)
			vlog("summary\n");
		summary();
	}
	| SUBTOTAL subcmd CR
	{
		subtotal($2);
	}
	| ECHOCMD ECHOSTR 
	{
		log("%s\n",$2);
	}
	| SETCMD STRING  CR
	{
		/* Do not free the string. It is now part of the environment */
		if(verbose)
			vlog("set %s\n",$2);
		putenv($2);
		
	}
	| UNSET STRING CR
	{
		int	slength;
		char **	ep;

		if(verbose)
			vlog("unset %s\n",$2);

		slength = strlen($2);
		for(ep = environ;*ep;ep++) {
			if(!strncmp($2,*ep,slength) && (*ep[slength]=='='))
				break;
		}
		if(*ep) {
			while(*ep) {
				*ep = ep[1];
				ep++;
			}
		}
	}
	| PAUSE CR
	{
		register int	fd;
		char 	c;

		if(verbose)
			vlog("pause\n");
		if((fd = open("/dev/tty",O_RDWR)) < 0) {
			log("testsh: unable to open /dev/tty\n");
			exit(-1);
		}
		write(fd,"Press RETURN to continue Q to quit\n",35);
		for (;;) {
			if(read(fd,&c,1) <= 0) {
				log("testsh: Error on read of /dev/tty\n");
				exit(-2);
			}
			if(c == 'q' | c == 'Q') {
				log("testsh: Pause recieved user abort\n");
				exit(0);
			}
			if(c == '\n')
				break;
		}
		close(fd);
	}
	| READ STRING STRING CR
	{
		FILE *	fd;
		char	buf[128];
		int	i;

		if((fd = fopen("/dev/tty","w")) < (FILE *)0) {
			log("testsh: unable to open /dev/tty\n");
			exit(-1);
		}
		fprintf(fd,"%s\n",$2);
		fclose(fd);
		strcpy(buf,$3);
		buf[i = strlen($3)] = '=';
		buf[++i] = '\0';
		if((fd = fopen("/dev/tty","r")) < (FILE *)0) {
			log("testsh: unable to open /dev/tty\n");
			exit(-1);
		}
		fgets(&buf[i],128-i,fd);
		fclose(fd);
		i=strlen(buf);
		if(buf[--i] == '\n')
			buf[i] = '\0';
		putenv(buf);
	}
	| CR
	;

 errcmd	: STOP
	{ 
		if(verbose)
			vlog("onerror stop\n");
		$$ = 1;
	}
	| CONTINUE
	{
		if(verbose)
			vlog("onerror continue\n");
		$$ = 0;
	}
	| DEFAULT
	{
		if(verbose)
			vlog("onerror default\n");
		$$ = errdefault;
	}
	;

subcmd	: /* emtpy */
	{
		if(verbose)
			vlog("subtotal\n");
		$$ = 0;
	} 
	| CLEAR
	{
		if(verbose)
			vlog("subtotal clear\n");
		$$ = 1;
	} 
	;

args	: /* empty */
	{
		arguments[argcnt] = (char *)0;
	}
	| args STRING
	{
		arguments[argcnt++] = $2;
	}
	;
%%
summary()
{
	log("Summary: %d passed, %d failed\n",passcnt,failcnt);
}

subtotal(clrflag)
int	clrflag;
{
	log("Subtotal: %d passed, %d failed\n",subpasscnt,subfailcnt);
	if(clrflag)
		subpasscnt = subfailcnt = 0;
}

/* 
 * runprog - run a program or test 
 *
 * INPUT - runflag this determines the options that are prepended to
 * 		the command line.
 *		if runflag is greater than 0 an rsh is prepended
 *		if runflag is less than 0 the test options (if any)
 *			are inserted
 */

runprog(runflag)
int	runflag;
{
	register int i; /* a general useful int */
	int	pid;
	int	stat_loc = -1; /* assume failure */
	char	buf[12];
	searchlist_p_t	slist;

	i = ARGSTART;
	if(runflag < 0 && outputmask != MASKDEFAULT) {
		sprintf(buf,"%d",outputmask);
		arguments[i-2] = arguments[i];
		arguments[i--] = buf;
		arguments[i--] = "-d";
	}
	if(runflag < 0 && scroutput) {
		arguments[i-1] = arguments[i];
		arguments[i--] = "-s";
	}
	if(runflag>0)
		arguments[--i] = "rsh";
	
	fflush(stdout);
	arguments[argcnt] = (char *)0;
	if(verbose) {
		register char **	cp;
		for(cp = &arguments[i];*cp;vlog(" %s",*cp++));
		vlog("\n");
	}
	switch(pid = fork())  {
	case -1:
		log("fork failed %d\n",errno);
		break;
	case 0:
		execve(arguments[i],&arguments[i],s_envp);
		slist = &searcher;
		while(slist=slist->searchnext) {
			int	t;
			char tmpbuf[TSHPATHLEN];
			strcpy(tmpbuf,slist->searchdir);
			if(tmpbuf[t = strlen(slist->searchdir)] != '/') {
				tmpbuf[t++] = '/';
				tmpbuf[t] = '\0';
			}
			strcat(tmpbuf,arguments[i]);
			execve(tmpbuf,&arguments[i],s_envp);
		}
		log("testsh: Unable to exec %s\n",arguments[i]);
		exit(-1);
		break;
	default:
		wait(&stat_loc);
		break;
	}
	for(i=ARGSTART;i<argcnt;free(arguments[i++]));
	argcnt = ARGSTART;
	return(stat_loc);
}
_doinclude(pathname)
char *	pathname;
{
	char	*oldfname;
	FILE	*fd;
	FILE	*ofd;
	int	oldlineno;

	oldfname=TshFileName;
	if(fd = fopen(TshFileName=pathname,"r")){
		ofd = yyin;
		yyin = fd;
		oldlineno = yylineno;
		yylineno = 0;
		yyparse();
		yylineno = oldlineno;
		yyin = ofd;
		yychar = -1;	/* re-initialize the state */
		fclose(fd);
	}
	TshFileName=oldfname;
}
doinclude(pathname)
char * pathname;
{
	int	error = FALSE;		/* assume success */
	searchlist_p_t	slist;

	if(!access(pathname,R_OK)) {
		_doinclude(pathname);
	}
	else {
		error = TRUE;
		slist = &searcher;
		while(slist = slist->searchnext) {
			int	t;
			char tmpbuf[TSHPATHLEN];

			strcpy(tmpbuf,slist->searchdir);
			if(tmpbuf[t = strlen(slist->searchdir)] != '/') {
				tmpbuf[t++] = '/';
				tmpbuf[t] = '\0';
			}
			strcat(tmpbuf,pathname);
			if(!access(tmpbuf,R_OK))  {
				error = FALSE;
				_doinclude(tmpbuf);
			}
		}
	}
	if(error)
		log("testsh: %s: %s\n",pathname,sys_errlist[errno>sys_nerr?0:errno]);
		
	free(pathname);
}
