/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dce.h,v $
 * Revision 1.1.6.2  1996/02/18  23:32:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:18  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:35:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:07:00  root]
 * 
 * Revision 1.1.4.5  1994/09/23  19:59:56  tom
 * 	Back out change of unsigned/signed64 to typedef hyper
 * 	[1994/09/23  19:26:43  tom]
 * 
 * Revision 1.1.4.3  1994/08/16  18:12:40  cbrooks
 * 	CR11494 64bit changes
 * 	[1994/08/16  18:11:30  cbrooks]
 * 
 * Revision 1.1.4.2  1994/01/19  17:42:39  cbrooks
 * 	Code Cleanup
 * 	[1994/01/18  22:58:56  cbrooks]
 * 
 * Revision 1.1.4.1  1994/01/14  14:59:32  rousseau
 * 	Changed to use new DCE RFC 34.2 ENDIAN convention.
 * 	[1994/01/14  14:59:00  rousseau]
 * 
 * Revision 1.1.2.1  1993/12/09  23:40:35  melman
 * 	Made dce.h machine specific
 * 	[1993/12/09  23:40:18  melman]
 * 
 * $EndLog$
 */
#if !defined(_DCE_H)
#define _DCE_H

/*
 * Common definitions for DCE
 * This is a machine specific file that must be ported to each platform.
 */ 

/* 
 * Define the endianess of the platform. Pulled in from machine/endian.h.
 */
#include <machine/endian.h>


/* Only one place needed for DCE to define these */
#define FALSE 0
#define TRUE 1

#if !defined(MIN)
#  define MIN(x, y)         ((x) < (y) ? (x) : (y))
#endif

#if !defined(MAX)
#  define MAX(x, y)         ((x) > (y) ? (x) : (y))
#endif


/* 
 * The following allows for the support of both old and new style 
 * function definitions and prototypes.  All DCE code is required to 
 * be ANSI C compliant and to use prototypes.  For those components 
 * that wish to support old-style definitions, the following macros 
 * must be used.
 *
 *  Declare a prototype like this (don't use variables):
 *      int foo _DCE_PROTOTYPE_((int, void *, struct bar *))
 *  
 *  Define a function like this:
 *      int foo 
 *      #if defined(_DCE_PROTO_)
 *              (
 *              int a, 
 *              void *b,
 *              struct bar *c
 *              )
 *      #else
 *              (a, b, c)
 *              int a;
 *              void *b;
 *              struct bar *c;
 *      #endif
 */
#if defined(__STDC__)                   /* other conditionals can be tested */
#  define _DCE_PROTO_
#endif                                  /* defined(__STDC__) */

#if defined(_DCE_PROTO_)
#  define _DCE_PROTOTYPE_(arg) arg 
#else                                   /* defined(_DCE_PROTO_) */
#  define _DCE_PROTOTYPE_(arg) ()
#endif                                  /* defined(_DCE_PROTO_) */

/* 
 * For those components wishing to support platforms where void 
 * pointers are not available, they can use the following typedef for 
 * a generic pointer type.  If they are supporting such platforms they 
 * must use this.
 */
#if defined(__STDC__)
#  define _DCE_VOID_
#endif                                  /* defined(__STDC__) */

#if defined(_DCE_VOID_)
  typedef void * pointer_t;
#else                                   /* defined(_DCE_VOID_) */
  typedef char * pointer_t;
#endif                                  /* defined(_DCE_VOID_) */

/* 
 * Here is a macro that can be used to support token concatenation in 
 * an ANSI and non-ANSI environment.  Support of non-ANSI environments 
 * is not required, but where done, this macro must be used.
 */
#if defined(__STDC__)
#  define _DCE_TOKENCONCAT_
#endif

#if defined(_DCE_TOKENCONCAT_)
#  define DCE_CONCAT(a, b)      a ## b
#else                                   /* defined(_DCE_TOKENCONCAT_) */
#  define DCE_CONCAT(a, b)      a/**/b
#endif                                  /* defined(_DCE_TOKENCONCAT_) */

/*
 * Define the dcelocal and dceshared directories
 */
extern const char *dcelocal_path;
extern const char *dceshared_path;

/* If DCE_DEBUG is defined then debugging code is activated. */
#define DCE_DEBUG 

/* 
 * Machine dependent typedefs for boolean, byte, and (un)signed integers.
 * All DCE code should be using these typedefs where applicable.
 * The following are defined in nbase.h:
 *     unsigned8       unsigned  8 bit integer
 *     unsigned16      unsigned 16 bit integer
 *     unsigned32      unsigned 32 bit integer
 *     signed8           signed  8 bit integer       
 *     signed16          signed 16 bit integer
 *     signed32          signed 32 bit integer
 * Define the following from idl types in idlbase.h (which is included 
 * by nbase.h:
 *     byte            unsigned  8 bits
 *     boolean         unsigned  8 bits   
 * Define (un)signed64 to be used with the U64* macros
 */
#include <dce/nbase.h>
typedef idl_byte        byte;
typedef idl_boolean     boolean;
typedef struct unsigned64_s_t {
    unsigned long hi;
    unsigned long lo;
} unsigned64;

typedef struct signed64_s_t {
    unsigned long hi;
    unsigned long lo;
} signed64;

typedef struct unsigned48_s_t {
    unsigned long  int  lo;             /* least significant 32 bits */
	unsigned short int  hi;             /* most significant 16 bits */
} unsigned48;

typedef struct unsigned128_s_t {
    unsigned long lolo;
    unsigned long lohi;
    unsigned long hilo;
    unsigned long hihi;
} unsigned128;

/* 
 * Serviceability and perhaps other DCE-wide include files 
 * will be included here.  This is a sample only.
 */
#include <dce/dce_svc.h>

#endif                                  /* _DCE_H */
