/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cvt_glob.c,v $
 * Revision 1.1.10.2  1996/02/18  18:53:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:09  marty]
 *
 * Revision 1.1.10.1  1995/12/07  22:24:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:13:59  root]
 * 
 * Revision 1.1.6.2  1993/07/07  20:05:12  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:35:44  ganni]
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
**  NAME:
**
**      cvt_glob.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**  Global data used by floating point conversion routines.
**
**  VERSION: DCE 1.0
**
*/


#include <cvt.h>
#include <cvt_pvt.h>

#if (NDR_LOCAL_INT_REP != ndr_c_int_big_endian)

idl_ulong_int vax_c[] = {

        0x00008000, 0x00000000, 0x00000000, 0x00000000,         /* ROPs */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,         /* zeros */
        0xffff7fff, 0xffffffff, 0xffffffff, 0xffffffff,         /* +huge */
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,         /* -huge */

};


idl_ulong_int ieee_s[] = {

        0x7fbfffff,             /* little endian ieee s nan */
        0xffffbf7f,             /* big endian ieee s nan */
        0x00000000,             /* le ieee s +zero */
        0x00000000,             /* be ieee s +zero */
        0x80000000,             /* le ieee s -zero */
        0x00000080,             /* be ieee s -zero */
        0x7f7fffff,             /* le ieee s +huge */
        0xffff7f7f,             /* be ieee s +huge */
        0xff7fffff,             /* le ieee s -huge */
        0xffff7fff,             /* be ieee s -huge */
        0x7f800000,             /* le ieee s +infinity */
        0x0000807f,             /* be ieee s +infinity */
        0xff800000,             /* le ieee s -infinity */
        0x000080ff,             /* be ieee s -infinity */

};

idl_ulong_int ieee_t[] = {

        0xffffffff, 0x7ff7ffff,         /* le ieee t nan */
        0xfffff77f, 0xffffffff,         /* be ieee t nan */
        0x00000000, 0x00000000,         /* le ieee t +zero */
        0x00000000, 0x00000000,         /* be ieee t +zero */
        0x00000000, 0x80000000,         /* le ieee t -zero */
        0x00000080, 0x00000000,         /* be ieee t -zero */
        0xffffffff, 0x7fefffff,         /* le ieee s +huge */
        0xffffef7f, 0xffffffff,         /* be ieee s +huge */
        0xffffffff, 0xffefffff,         /* le ieee s -huge */
        0xffffefff, 0xffffffff,         /* be ieee s -huge */
        0x00000000, 0x7ff00000,         /* le ieee t +infinity */
        0x0000f07f, 0x00000000,         /* be ieee t +infinity */
        0x00000000, 0xfff00000,         /* le ieee t -infinity */
        0x0000f0ff, 0x00000000,         /* be ieee t -infinity */

};


idl_ulong_int ibm_s[] = {

   0x000000ff,          /* ibm s invalid */
   0x00000000,          /* ibm s +zero */
   0x00000080,          /* ibm s -zero */
   0xffffff7f,          /* ibm s +huge */
   0xffffffff,          /* ibm s -huge */
   0xffffff7f,          /* ibm s +infinity */
   0xffffffff,          /* ibm s -infinity */

};

idl_ulong_int ibm_l[] = {

   0x000000ff, 0x00000000,              /* ibm t invalid */
   0x00000000, 0x00000000,              /* ibm t +zero */
   0x00000080, 0x00000000,              /* ibm t -zero */
   0xffffff7f, 0xffffffff,              /* ibm t +huge */
   0xffffffff, 0xffffffff,              /* ibm t -huge */
   0xffffff7f, 0xffffffff,              /* ibm t +infinity */
   0xffffffff, 0xffffffff,              /* ibm t -infinity */

};

idl_ulong_int cray[] = {

        0x00000060, 0x00000000,         /* cray invalid */
        0x00000000, 0x00000000,         /* cray +zero */
        0x00000080, 0x00000000,         /* cray -zero */
        0xffffff5f, 0xffffffff,         /* cray +huge */
        0xffffffdf, 0xffffffff,         /* cray -huge */
        0x00000060, 0x00000000,         /* cray +infinity */
        0x000000e0, 0x00000000,         /* cray -infinity */

};

idl_ulong_int int_c[] = {

        0x00000000,             /* le int nan */
        0x00000000,             /* be int nan */
        0x00000000,             /* le int zero */
        0x00000000,             /* be int zero */
        0x7fffffff,             /* le int +huge */
        0xffffff7f,             /* be int +huge */
        0x80000000,             /* le int -huge */
        0x00000080,             /* be int -huge */
        0x7fffffff,             /* le int +infinity */
        0xffffff7f,             /* be int +infinity */
        0x80000000,             /* le int -infinity */
        0x00000080,             /* be int -infinity */

};

#else /* Big-endian Data */

idl_ulong_int vax_c[] = {

        0x00800000, 0x00000000, 0x00000000, 0x00000000,         /* ROPs */
        0x00000000, 0x00000000, 0x00000000, 0x00000000,         /* zeros */
        0xff7fffff, 0xffffffff, 0xffffffff, 0xffffffff,         /* +huge */
        0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,         /* -huge */

};


idl_ulong_int ieee_s[] = {

        0xffffbf7f,             /* little endian ieee s nan */
        0x7fbfffff,             /* big endian ieee s nan */
        0x00000000,             /* le ieee s +zero */
        0x00000000,             /* be ieee s +zero */
        0x00000080,             /* le ieee s -zero */
        0x80000000,             /* be ieee s -zero */
        0xffff7f7f,             /* le ieee s +huge */
        0x7f7fffff,             /* be ieee s +huge */
        0xffff7fff,             /* le ieee s -huge */
        0xff7fffff,             /* be ieee s -huge */
        0x0000807f,             /* le ieee s +infinity */
        0x7f800000,             /* be ieee s +infinity */
        0x000080ff,             /* le ieee s -infinity */
        0xff800000,             /* be ieee s -infinity */

};

idl_ulong_int ieee_t[] = {

        0xffffffff, 0xfffff77f,         /* le ieee t nan */
        0x7ff7ffff, 0xffffffff,         /* be ieee t nan */
        0x00000000, 0x00000000,         /* le ieee t +zero */
        0x00000000, 0x00000000,         /* be ieee t +zero */
        0x00000000, 0x00000080,         /* le ieee t -zero */
        0x80000000, 0x00000000,         /* be ieee t -zero */
        0xffffffff, 0xffffef7f,         /* le ieee s +huge */
        0x7fefffff, 0xffffffff,         /* be ieee s +huge */
        0xffffffff, 0xffffefff,         /* le ieee s -huge */
        0xffefffff, 0xffffffff,         /* be ieee s -huge */
        0x00000000, 0x0000f07f,         /* le ieee t +infinity */
        0x7ff00000, 0x00000000,         /* be ieee t +infinity */
        0x00000000, 0x0000f0ff,         /* le ieee t -infinity */
        0xfff00000, 0x00000000,         /* be ieee t -infinity */

};


idl_ulong_int ibm_s[] = {

   0xff000000,          /* ibm s invalid */
   0x00000000,          /* ibm s +zero */
   0x80000000,          /* ibm s -zero */
   0x7fffffff,          /* ibm s +huge */
   0xffffffff,          /* ibm s -huge */
   0x7fffffff,          /* ibm s +infinity */
   0xffffffff,          /* ibm s -infinity */

};

idl_ulong_int ibm_l[] = {

   0xff000000, 0x00000000,              /* ibm t invalid */
   0x00000000, 0x00000000,              /* ibm t +zero */
   0x80000000, 0x00000000,              /* ibm t -zero */
   0x7fffffff, 0xffffffff,              /* ibm t +huge */
   0xffffffff, 0xffffffff,              /* ibm t -huge */
   0x7fffffff, 0xffffffff,              /* ibm t +infinity */
   0xffffffff, 0xffffffff,              /* ibm t -infinity */

};

idl_ulong_int cray[] = {

        0x60000000, 0x00000000,         /* cray invalid */
        0x00000000, 0x00000000,         /* cray +zero */
        0x80000000, 0x00000000,         /* cray -zero */
        0x5fffffff, 0xffffffff,         /* cray +huge */
        0xdfffffff, 0xffffffff,         /* cray -huge */
        0x60000000, 0x00000000,         /* cray +infinity */
        0xe0000000, 0x00000000,         /* cray -infinity */

};

idl_ulong_int int_c[] = {

        0x00000000,             /* le int nan */
        0x00000000,             /* be int nan */
        0x00000000,             /* le int zero */
        0x00000000,             /* be int zero */
        0xffffff7f,             /* le int +huge */
        0x7fffffff,             /* be int +huge */
        0x00000080,             /* le int -huge */
        0x80000000,             /* be int -huge */
        0xffffff7f,             /* le int +infinity */
        0x7fffffff,             /* be int +infinity */
        0x00000080,             /* le int -infinity */
        0x80000000,             /* be int -infinity */

};

#endif /* Big-Endian Data */
