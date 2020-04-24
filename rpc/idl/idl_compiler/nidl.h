/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nidl.h,v $
 * Revision 1.1.10.2  1996/02/18  23:45:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:25  marty]
 *
 * Revision 1.1.10.1  1995/12/07  22:21:04  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:04 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:12:47  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:31  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  00:44:17  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  22:58:25  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:25:01  bfc]
 * 
 * Revision 1.1.6.2  1993/07/07  20:02:35  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:33:59  ganni]
 * 
 * Revision 1.1.4.3  1993/01/03  21:40:44  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  14:37:05  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  18:49:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:04:22  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/06  21:06:29  harrow
 * 	Change MALLOC macro to return zeroed memory becaue the
 * 	backend depends sometimes depends upon it.
 * 	[1992/07/06  14:47:31  harrow]
 * 
 * Revision 1.1  1992/01/19  03:03:00  devrcs
 * 	Initial revision
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
**      NIDL.H
**
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**      Mandatory header file containing all system dependent
**      includes and common macros used by the IDL compiler.
**
**  VERSION: DCE 1.0
*/

#ifndef NIDLH_INCL
#define NIDLH_INCL

#define NIDLBASE_H

/* Base include files needed by all IDL compiler modules */

#include <stdio.h>
#include <string.h>

#ifdef DUMPERS
# define DEBUG_VERBOSE 1
#endif

#if defined(__STDC__) || defined(_WIN32) 
#   include <stdlib.h>
#   ifndef CHAR_BIT
#       include <limits.h>  /* Bring in limits.h if not cacaded in yet */
#   endif
#else /* prototypes that normally come from stdlib.h */
    extern void *malloc();
    extern void *calloc();
    extern void *realloc();
    extern void free();
    extern char *getenv(char*);
    extern int atoi();
    extern double atof();
    extern long atol();
#endif
#if defined DEBUG_VERBOSE || defined DUMPERS
#   ifdef __STDC__
#       include <assert.h>
#   else
#       define assert(ex) if (ex) ;
#   endif
#endif
#include <sysdep.h>

/*
 * some generally useful types and macros
 */

typedef unsigned char       unsigned8;
typedef unsigned short int  unsigned16;
typedef unsigned long int   unsigned32;

typedef unsigned8 boolean;
#define true 1
#define false 0
#ifndef TRUE
#define TRUE true
#define FALSE false
#endif

/*
 * IDL's model of the info in a UUID (see uuid_t in nbase.idl)
 */

typedef struct
{
    unsigned32      time_low;
    unsigned16      time_mid;
    unsigned16      time_hi_and_version;
    unsigned8       clock_seq_hi_and_reserved;
    unsigned8       clock_seq_low;
    unsigned8       node[6];
} nidl_uuid_t;

/*
 * Include files needed by the remaining supplied definitions in this file.
 * These need to be here, since they depend on the above definitions.
 */

#include <errors.h>
#include <nidlmsg.h>

/* Language enum.  Here for lack of any place else. */
typedef enum {
    lang_ada_k,
    lang_basic_k,
    lang_c_k,
    lang_cobol_k,
    lang_fortran_k,
    lang_pascal_k
} language_k_t;

/*
 * Macro jackets for each of the C memory management routines.
 * The macros guarantee that control will not return to the caller without
 * memory; therefore the call site doesn't have to test.
 */
/*
 *  A temp pointer to be used by the MALLOC macros.
 */
static heap_mem * MALLOC_temp_ptr;

#define MALLOC(n)                                \
    (((MALLOC_temp_ptr = (heap_mem*)calloc (1,(n))) == (heap_mem*)NULL) ?  \
        (error (NIDL_OUTOFMEM), (heap_mem*)NULL) : MALLOC_temp_ptr)

#define CALLOC(n,m)                                   \
    (((MALLOC_temp_ptr = (heap_mem*)calloc ((n), (m))) == (heap_mem*)NULL) ?  \
        (error (NIDL_OUTOFMEM), (heap_mem*)NULL) : MALLOC_temp_ptr)

#define REALLOC(p,n)                                           \
    (((MALLOC_temp_ptr = (heap_mem*)realloc ((char*)(p), (n))) == (heap_mem*)NULL) ? \
        (error (NIDL_OUTOFMEM), (heap_mem*)NULL) : MALLOC_temp_ptr )

#define FREE(p) free ((heap_mem*) (p));


/*
 * Enable YYDEBUG, and ASSERTION checking, if DUMPERS is defined
 */
#ifdef DUMPERS
#  define YYDEBUG 1
   /* If ASSERTION expression is FALSE, then issue warning */
#  define ASSERTION(x) if (!(x)) warning(NIDL_INTERNAL_ERROR, __FILE__, __LINE__)
#else
#  define ASSERTION(x) if (0) (x)
#endif

/*
 * Some common yacc prototypes, since some of our compilers
 * are more strict than others...
 */

extern void yywhere(
#ifdef PROTO
    void
#endif
);

extern int yylex(
#ifdef PROTO
    void
#endif
);

extern void yyerror(
#ifdef PROTO
    char *
#endif
);

extern int yylook(
#ifdef PROTO
    void
#endif
);

static int yywrap(
#ifdef PROTO
    void
#endif
);

static void yymark(
#ifdef PROTO
    void
#endif
);


#endif
