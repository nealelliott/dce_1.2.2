/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: main.c,v $
 * Revision 1.1.10.2  1996/02/17  23:26:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:00  marty]
 *
 * Revision 1.1.10.1  1995/12/11  21:50:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:18  root]
 * 
 * Revision 1.1.8.2  1993/08/05  21:16:08  cuti
 * 	 Assign exit value according to failcnt.
 * 	[1993/08/05  19:08:16  cuti]
 * 
 * Revision 1.1.4.7  1993/01/21  22:51:16  sekhar
 * 	[OT 3408]  Process -d switch correctly and avoid an infinte loop.
 * 	[1993/01/17  23:10:21  sekhar]
 * 
 * Revision 1.1.4.6  1992/12/31  17:34:59  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:59:22  htf]
 * 
 * Revision 1.1.4.5  1992/11/19  13:17:20  sekhar
 * 	[OT 5912]  In functions log() and vlog() initialize fmt using va_arg
 * 	           only when varargs interface i.e. _DCE_PROTO_ is not defined.
 * 	[1992/11/18  19:49:09  sekhar]
 * 
 * Revision 1.1.4.4  1992/11/10  19:23:55  cuti
 * 	Conform to DCE1.1 coding standard: change __STDC__ to _DCE_PROTO_
 * 	[1992/11/10  17:49:27  cuti]
 * 
 * Revision 1.1.4.5  1992/11/04  19:50:34  cuti
 * 	5912: Unable to compile under HP-UX ANSI=c compiler. Add #ifdef clause.
 * 	[1992/11/03  20:15:12  cuti]
 * 
 * Revision 1.1.4.2  1992/09/29  21:24:46  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge UPDATE: Use execve() instead of execvp() for all
 * 	           systems not just SVR4
 * 	[1992/09/24  14:23:30  sekhar]
 * 
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/15  14:55:36  sekhar]
 * 
 * Revision 1.1  1992/01/19  04:07:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * main.c - main line for tsh
 * 
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
 *
 * @(#)main.c	1.8
 *  
 *	Extracted	1/7/91 10:19:05 
 * 	from delta on	1/3/91 14:16:36
 *
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
#ifndef LINT
static char sccs_id[] = "@(#)main.c	1.8";
#endif

#include	<tsh.h>
#if defined(SYSV) || defined(SNI_SVR4_POSIX)
#include	<unistd.h>
#else
#include 	<sys/file.h>
#endif
#if defined(_DCE_PROTO_)
#include 	<stdarg.h>
#else
#include	<varargs.h>
#endif
#include	<errno.h>
extern int	errno;
extern char *	sys_errlist[];
extern int	sys_nerr;
extern char *	optarg;
extern int	optind;
extern FILE *	yyin;
extern int	yylineno;

int	errorflag = 0;		/* flag to stop on error 		*/
int	errdefault = 0;		/* default value of error flag 		*/

int	verboselog = 0;		/* TRUE if verbose set for log file 	*/
int	verbosescrn = 0;	/* TRUE if verbose set for screen 	*/
int	verbose = 0;		/* TRUE if verboselog or verbosescrn 	*/
int	scroutput = 0;		/* TRUE if screen output		*/
int	outputmask = MASKDEFAULT;

extern int failcnt;
extern char **  s_envp;
							/* additional search directories 		*/
searchlist_t	searcher	= {0};
FILE *	logfile = (FILE *)0;	/* Log File				*/
char *	TshFileName;

main(argc,argv,envp)
int	argc;
char *	argv[];
char ** envp;
{

	int	c;
	int	errflag = 0;
	register char *	cp;
	FILE *	tmpfd;			/* holds stdin during startup */
	searchlist_p_t	slist;
	int		save_optind;

	s_envp = envp;
        save_optind = optind;  /* required to process -d option */
	while((c = getopt(argc,argv,"d:eI:l:L:sS")) != -1) {
		switch (c) {
		case 'd':
			/* The debug level after -d is optional */
                        /* Check to make sure a debug  level is specified */
                        /* If none is specified the default is MASKDEFAULT */

			if ((optind - save_optind) == 1)
			    outputmask = atoi(optarg);
			else {
			    outputmask = MASKDEFAULT;
			    optind-- ;
			}
			break;
		case 'e':
			errorflag = errdefault = 1;
			break;
		case 'I':
			slist = &searcher;
			while(slist->searchnext)
				slist = slist->searchnext;
			slist->searchnext = (searchlist_p_t)malloc(sizeof(searchlist_t));
			slist = slist->searchnext;
			if(!slist) {
				log("testsh: %s: %s\n",optarg,
				    sys_errlist[errno>sys_nerr?0:errno]);
				exit(-2);
			}
			slist->searchdir = optarg;
			slist->searchnext = (searchlist_p_t)0;
			break;
		case 'L':
			verboselog++;
			/* falls through */
		case 'l':	
			if(*optarg == '-' && optarg[1] == '\0')
				logfile = stdout;
			else {
				fclose(stdout);
				if((logfile = fopen(optarg,"a")) == (FILE *)0){
					log("testsh: %s: %s\n",optarg,
				    	sys_errlist[errno>sys_nerr?0:errno]);
					exit(-2);
				}
			}
			break;
		case 'S':
			verbosescrn++;
			/* falls through */
		case 's':
			scroutput++;
			break;
		case '?':
		default:
			errflag++;
		}
		save_optind = optind; /* required to process -d option */
	}
	verbose = verboselog+verbosescrn;

	if(errflag){
		fprintf(stderr,"Usage %s: [-d mask] [-eSs] [-I searchdir] [-l | -L logfile] fname [fname...]\n",argv[0]);
		exit(-2);
	}

	/* Proccess the system tsh control file */
	if(!access(TshFileName=ETCTSHRC,R_OK)) {
		if(yyin = fopen(ETCTSHRC,"r")){
			yylineno = 0;
			yyparse();
			fclose(yyin);
		}
	}
	/* Process the user control file */
	if(!access(TshFileName=DOTTSHRC,R_OK)) {
		if(yyin = fopen(DOTTSHRC,"r")){
			yylineno = 0;
			yyparse();
			fclose(yyin);
		}
	}
	if(optind == argc) {
		yylineno = 0;
		TshFileName = "stdin";
		yyin=stdin;
		yyparse();
	}
	else {
		for(; optind< argc;optind++) {
			if(!access(argv[optind],R_OK)) {
				if(yyin = fopen(TshFileName = argv[optind],"r")){
					yylineno = 0;
					yyparse();
					fclose(yyin);
				}
			}
			else 
				log("testsh: %s: %s\n",argv[optind],sys_errlist[errno>sys_nerr?0:errno]);
		}
	}
        if (failcnt != 0) {
            exit(1);
        }
        else
            exit(0);
}


void
log
#if defined(_DCE_PROTO_)
(char *fmt, ...)
#else
(va_alist)
va_dcl
#endif
{
	va_list	args;
#if defined(_DCE_PROTO_)
	va_start(args, fmt);
#else
	char *	fmt;

	va_start(args);
	fmt = va_arg(args,char *);
#endif

	if(logfile)
		vfprintf(logfile,fmt,args);
	if(scroutput)
		vfprintf(stderr,fmt,args);
	else if(!logfile )
		vfprintf(stdout,fmt,args);
	va_end(args);
}

/*
 * vlog - displays verbose output
 *
 * Nota Bene - verbose screen output is always sent to standard error
 */
void
vlog

#if defined(_DCE_PROTO_)
(char *fmt, ...)
#else
(va_alist)
va_dcl
#endif
{
	va_list	args;
#if defined(_DCE_PROTO_)
	va_start(args, fmt);
#else
	char *	fmt;

	va_start(args);
	fmt = va_arg(args, char *);
#endif


	if(verboselog)
		vfprintf(logfile,fmt,args);
	if(verbosescrn)
		vfprintf(stderr,fmt,args);
	va_end(args);
}
