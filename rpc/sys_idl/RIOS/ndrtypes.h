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
 * Revision 1.1.134.2  1996/02/18  22:57:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:08  marty]
 *
 * Revision 1.1.134.1  1995/12/08  00:22:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:51  root]
 * 
 * Revision 1.1.132.3  1994/06/10  20:54:57  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:23  devsrc]
 * 
 * Revision 1.1.132.2  1994/04/06  22:05:37  tom
 * 	Bug 9965 - ndr_true should be defined to lowercase true.
 * 	[1994/04/06  21:57:23  tom]
 * 
 * Revision 1.1.132.1  1994/01/21  22:39:53  cbrooks
 * 	platform dependent NDR data types
 * 	[1994/01/21  17:14:33  cbrooks]
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
 * This particular file defines the NDR types for a big-endian
 * architecture. This file also depends on the presence of a ANSI 
 * C compiler, in that it uses the signed keyword to create the 
 * ndr_small_int type.
 */

/****
 **** RS6000 specific definitions of the default reps for NDR scalar types.
 ****/

#ifndef _NDRTYPES_H
#define _NDRTYPES_H

typedef unsigned char 		ndr_boolean ;

#define ndr_false false
#define ndr_true  true

typedef unsigned char 		ndr_byte ;

typedef unsigned char 		ndr_char ;

typedef signed char 		ndr_small_int ;

typedef unsigned char 		ndr_usmall_int ;

typedef short int 		ndr_short_int ;

typedef unsigned short int 	ndr_ushort_int ;

typedef long int 		ndr_long_int ;

typedef unsigned long int 	ndr_ulong_int ;

struct ndr_hyper_int_rep_s_t   {
    ndr_long_int high; 
    ndr_ulong_int low;
};

typedef struct ndr_hyper_int_rep_s_t ndr_hyper_int ;

struct ndr_uhyper_int_rep_s_t  {
    ndr_ulong_int high; 
    ndr_ulong_int low;
};

typedef struct ndr_uhyper_int_rep_s_t ndr_uhyper_int ;

typedef float 		ndr_short_float ;
typedef double 		ndr_long_float ;

#endif /* NDRTYPES_H */
