/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY 
 * $Log: ndrtypes.h,v $
 * Revision 1.1.128.2  1996/02/18  22:57:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:03  marty]
 *
 * Revision 1.1.128.1  1995/12/08  00:22:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:45  root]
 * 
 * Revision 1.1.126.3  1994/06/10  20:54:51  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:17  devsrc]
 * 
 * Revision 1.1.126.2  1994/04/06  22:05:35  tom
 * 	Bug 9965 - ndr_true should be defined to lowercase true.
 * 	[1994/04/06  21:57:21  tom]
 * 
 * Revision 1.1.126.1  1994/01/21  22:39:43  cbrooks
 * 	platform dependent NDR types
 * 	[1994/01/21  16:59:35  cbrooks]
 * 
 * $EndLog$
 */
/*
**  NAME:
**
**      ndrtypes.h
**
**  FACILITY:
**
**      IDL Stub Support Include File
**
**  ABSTRACT:
**
**  This file is new for DCE 1.1. This is a platform specific file that
**  defines the base level ndr types. This file is indirectly included 
**  in all files via the idlbase.h file. 
**
*/

/*
 * This particular file defines the NDR types for a little-endian 
 * architecture. This file also depends on the presence of a ANSI 
 * C compiler, in that it uses the signed keyword to create the 
 * ndr_small_int type.
 */

#ifndef _NDR_TYPES_H 
#define  _NDR_TYPES_H


typedef unsigned char 		ndr_boolean ;
#define ndr_false       false
#define ndr_true        true
typedef unsigned char 		ndr_byte ;

typedef unsigned char 		ndr_char ;

typedef signed char 		ndr_small_int ;

typedef unsigned char 		ndr_usmall_int ;

typedef short int 		ndr_short_int ;

typedef unsigned short int	ndr_ushort_int ;

typedef long int 		ndr_long_int ;

typedef unsigned long int 	ndr_ulong_int ;

/* 
 * the reps for hyper must match the little-endian NDR rep since
 *  defined(vax) || defined(M_I86) => defined(ALIGNED_SCALAR_ARRAYS) 
 */

struct ndr_hyper_int_rep_s_t {
    ndr_ulong_int low; 
    ndr_long_int high;
};

typedef struct ndr_hyper_int_rep_s_t ndr_hyper_int ;

struct ndr_uhyper_int_rep_s_t  {
    ndr_ulong_int low; 
    ndr_ulong_int high;
};

typedef struct ndr_uhyper_int_rep_s_t 		ndr_uhyper_int ;

typedef float 		        ndr_short_float ;
typedef double 			ndr_long_float ;


#endif /* _NDR_TYPES_H */

