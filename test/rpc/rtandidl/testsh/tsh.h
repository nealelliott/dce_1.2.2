/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh.h,v $
 * Revision 1.1.12.2  1996/02/18  23:06:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:41  marty]
 *
 * Revision 1.1.12.1  1995/12/11  20:13:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:28  root]
 * 
 * Revision 1.1.10.1  1994/06/10  20:44:53  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:38  devsrc]
 * 
 * Revision 1.1.8.1  1993/09/29  19:47:31  desai
 * 	CR 8275 fix. Removed multiple definitions of variables from tsh.h which causes
 * 	compiler failures on other platforms.
 * 	[1993/09/29  19:46:33  desai]
 * 
 * Revision 1.1.6.3  1993/06/16  22:40:00  sommerfeld
 * 	Previous submit didn't work correctly
 * 	[1993/06/16  22:32:50  sommerfeld]
 * 
 * Revision 1.1.6.2  1993/06/16  19:16:25  sommerfeld
 * 		Define TRUE and FALSE if they are not defined. (They
 * 		should not be defined on open systems.)
 * 		Define PATH_MAX is it's not defined. (This macro should not be
 * 		used because it's a part of XPG2, old standard...)
 * 		[1993/05/11  14:32:14  tatsu_s]
 * 	[1993/06/16  17:38:54  sommerfeld]
 * 
 * Revision 1.1.2.5  1993/02/26  22:39:04  weisman
 * 	very major cleanup
 * 	[1993/02/26  22:16:00  weisman]
 * 
 * Revision 1.1.2.3  1992/11/10  19:29:33  cuti
 * 	Conform to DCE1.1 coding standard: change __STDC__ to _DCE_PROTO_
 * 	[1992/11/10  18:15:01  cuti]
 * 
 * Revision 1.1.2.2  1992/11/10  16:09:37  cuti
 * 	Change _PROTOTYPE to __STDC__
 * 	[1992/11/10  14:47:18  cuti]
 * 
 * Revision 1.1  1992/01/19  03:41:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * tsh.h - header for tsh
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
 * @(#)tsh.h	1.6
 *  
 *	Extracted	1/7/91 10:19:07 
 * 	from delta on	1/3/91 14:16:51
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>
          
#if !defined(TRUE)
#define TRUE    1
#endif                          /* !defined(TRUE) */

#if !defined(FALSE)
#define FALSE   0
#endif                          /* !defined(FALSE) */

#if !defined(PATH_MAX)
#if defined(_POSIX_PATH_MAX)
#define PATH_MAX        _POSIX_PATH_MAX
#else
#define PATH_MAX        1023
#endif                          /* defined(_POSIX_PATH_MAX) */
#endif                          /* !defined(PATH_MAX) */

#if defined(__STDC__)           /* other conditionals can be tested */
#define _DCE_PROTO_
#endif                          /* defined(__STDC__) */

#if defined(_DCE_PROTO_)
#define _DCE_PROTOTYPE_(arg) arg 
#else                           /* defined(_DCE_PROTO_) */
#define _DCE_PROTOTYPE_(arg) ()
#endif                          /* defined(_DCE_PROTO_) */

extern void	log _DCE_PROTOTYPE_((char *format, ...));
extern void	vlog _DCE_PROTOTYPE_((char *format, ...));

typedef struct searchlist {
	struct searchlist *	searchnext;
	char *				searchdir;
}	searchlist_t, *searchlist_p_t;

#define		MASKDEFAULT	1
#define		DOTTSHRC	".tshrc"
#define		ETCTSHRC	"/etc/tshrc"

/* 
 * Following define the mode argument for doinclude and 
 * push_include_stack.  
 */

#define EXECUTE_MODE 1
#define INCLUDE_MODE 2


void initialize_include_stack (void);
void push_include_stack (int mode);
int pop_include_stack (void);
extern FILE * yyin;
extern int yylineno;
extern char * TshFileName;

extern int errorflag;  		/* flag to stop on error 		*/
extern int errdefault;	        /* default value of error flag 	        */
extern int verboselog ;         /* TRUE if verbose set for log file 	*/
extern int verbosescrn ;        /* TRUE if verbose set for screen 	*/
extern int verbose ;            /* TRUE if verboselog or verbosescrn 	*/
extern int scroutput ;          /* TRUE if screen output		*/
extern int outputmask ;         /*  */

/*
 * Expand a possibly-tiny lex buf.
 */

#if defined (YYLMAX)
#   undef YYLMAX
#endif

#define YYLMAX 1000

