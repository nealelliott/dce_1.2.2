/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: errors.h,v $
 * Revision 1.1.8.2  1996/02/18  23:45:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:07  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:17:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:11:20  root]
 * 
 * Revision 1.1.4.2  1993/07/07  19:58:30  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:31:46  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME
**
**      ERRORS.H
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**
**
**  VERSION: DCE 1.0
**
*/

#ifndef ERRORS_H
#define ERRORS_H

#include <errno.h>
#include <nidl.h>
#include <nametbl.h>


/*
 *  The following error and warning routines are NOT function prototyped
 *  since they are designed, a la printf, to accept a variable number of
 *  arguments without using the varargs nonsense.
 */
void error();
void warning();

void log_source_error();
void log_source_warning();
void log_error();
void log_warning();

typedef struct {
    long msg_id;
    char *arg1;
    char *arg2;
    char *arg3;
    char *arg4;
    char *arg5;
} idl_error_list_t;

typedef idl_error_list_t *idl_error_list_p;

void error_list(
#ifdef PROTO
    int vecsize,
    idl_error_list_p errvec,
    boolean exitflag
#endif
);

void inq_name_for_errors(
#ifdef PROTO
    char *name
#endif
);

void set_name_for_errors(
#ifdef PROTO
    char *name
#endif
);

boolean print_errors(
#ifdef PROTO
    void
#endif
);

void yyerror(
#ifdef PROTO
    char *message
#endif
);

/*
 *  The following global variables are used by error reporting routines, most
 *  notably yyerror().  The signature of yyerror is defined by yacc to be:
 *
 *          void yyerror(char *message)
 *
 *  Because of the limitations of the routine signature, yyerror relies on
 *  global variables defined by the parser to output additional information
 *  about the error.  Since IDL uses multiple parsers, the global variables
 *  needed depend on which parse is active.  The workaround used here is
 *  that before each individual parse, the pointer variables below will be
 *  set to point to the relevant globals for that parse.  The error routines
 *  can then access the relevant data indirectly through the pointer variables.
 */
extern FILE     **yyin_p;
extern int      *yylineno_p;
extern int      *yynerrs_p;
extern char     *yytext_p;

/*
 * Error info to be fillin the fe_info nodes
 */
extern int          error_count;
extern STRTAB_str_t error_file_name_id;

#ifdef DUMPERS
#define INTERNAL_ERROR(string) {printf("Internal Error Diagnostic: %s\n",string);warning(NIDL_INTERNAL_ERROR,__FILE__,__LINE__);}
#else
#define INTERNAL_ERROR(string) {error(NIDL_INTERNAL_ERROR,__FILE__,__LINE__); if (0) printf(string);}
#endif
#endif
