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
 * Revision 1.1.14.2  1996/02/17  23:23:02  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:41  marty]
 *
 * Revision 1.1.14.1  1995/12/11  20:13:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:26  root]
 * 
 * Revision 1.1.12.3  1993/06/16  22:39:57  sommerfeld
 * 	Previous submit didn't work correctly
 * 	[1993/06/16  22:32:35  sommerfeld]
 * 
 * Revision 1.1.12.2  1993/06/16  19:16:03  sommerfeld
 * 	 * Revision 1.1.10.2  1993/05/11  15:16:57  tatsu_s
 * 	 * 	Added exit(failcnt) at the end to be polite.
 * 	 * 	[1993/05/11  14:31:59  tatsu_s]
 * 	[1993/06/16  17:37:31  sommerfeld]
 * 
 * Revision 1.1.4.9  1993/02/26  22:39:01  weisman
 * 	very major cleanup
 * 	[1993/02/26  22:15:56  weisman]
 * 
 * Revision 1.1.4.8  1993/01/13  19:51:13  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:18:31  bbelch]
 * 
 * Revision 1.1.4.7  1992/11/18  21:05:46  cuti
 * 	Remove fmt initialization from STDC (_DCE_PROT_) block.
 * 	[1992/11/18  20:37:59  cuti]
 * 
 * Revision 1.1.4.6  1992/11/10  19:47:16  cuti
 * 	Conform to DCE1.1 coding standard: change __STDC__ to _DCE_PROTO_
 * 	[1992/11/10  18:14:42  cuti]
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

searchlist_t	searcher	= {NULL, ""};
FILE *	logfile = (FILE *)0;	/* Log File				*/
char *	TshFileName;

main(argc,argv,envp)
int	argc;
char *	argv[];
char ** envp;
{
    extern int failcnt;

    int	c;
    int	errflag = 0;
    register char *	cp;
    FILE *	tmpfd;          /* holds stdin during startup */
    searchlist_p_t	slist;

    errorflag = 0;
    errdefault = 0;
    verboselog = 0;
    verbosescrn = 0;
    verbose = 0;
    scroutput = 0;
    outputmask = MASKDEFAULT;
    initialize_include_stack ();

    while((c = getopt(argc,argv,"d:eI:l:L:sS")) != -1) {
        switch (c) {
        case 'd':
            outputmask = atoi(optarg);
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
                exit(2);
            }
            slist->searchdir = optarg;
            slist->searchnext = (searchlist_p_t) NULL;
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
                    exit(2);
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
    }
    verbose = verboselog+verbosescrn;

    if(errflag){
        fprintf(stderr,"Usage %s: [-d mask] [-eSs] [-I searchdir] [-l | -L logfile] fname [fname...]\n",argv[0]);
        exit(2);
    }

    /* Proccess the system tsh control file */
    if(!access(TshFileName=ETCTSHRC,R_OK)) {
        if(yyin = fopen(ETCTSHRC,"r")){
            yylineno = 1;
            yyparse();
            fclose(yyin);
        }
    }
    /* Process the user control file */
    if(!access(TshFileName=DOTTSHRC,R_OK)) {
        if(yyin = fopen(DOTTSHRC,"r")){
            yylineno = 1;
            yyparse();
            fclose(yyin);
        }
    }
    if(optind == argc) {
        yylineno = 1;
        TshFileName = "stdin";
        yyin=stdin;
        yyparse();
    }
    else {
        for(; optind< argc;optind++) {
            if(!access(argv[optind],R_OK)) {
                if(yyin = fopen(TshFileName = argv[optind],"r")){
                    yylineno = 1;
                    yyparse();
                    fclose(yyin);
                }
            }
            else 
                log("testsh: %s: %s\n",argv[optind],sys_errlist[errno>sys_nerr?0:errno]);
        }
    }
    exit (failcnt);
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
