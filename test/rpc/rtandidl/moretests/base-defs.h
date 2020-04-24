/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: base-defs.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:18  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:02:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:42:45  root]
 * 
 * Revision 1.1.2.2  1993/01/11  22:11:14  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:03:38  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:34:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  base-defs.h V=4 11/19/90 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/test/rpc/rtandidl/moretests/base-defs.h,v $
 * $Author: marty $
 * $Id: base-defs.h,v 1.1.4.2 1996/02/18 23:06:09 marty Exp $
 *
 * Copyright 1989,1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * Basic definitions for Kerberos V5 library
 */

#include <krb5/copyright.h>

#ifndef KRB5_BASE_DEFS__
#define KRB5_BASE_DEFS__

#include <krb5/wordsize.h>

#ifndef FALSE
#define	FALSE	0
#endif
#ifndef TRUE
#define	TRUE	1
#endif

typedef krb5_octet	krb5_boolean;
typedef	krb5_octet	krb5_msgtype;
typedef	krb5_octet	krb5_kvno;

typedef	krb5_ui_2	krb5_addrtype;
typedef krb5_ui_2	krb5_keytype;
typedef krb5_ui_2	krb5_enctype;
typedef krb5_ui_2	krb5_cksumtype;
typedef krb5_ui_2	krb5_authdatatype;

typedef	krb5_int32	krb5_flags;
typedef krb5_int32	krb5_timestamp;
typedef	krb5_int32	krb5_error_code;
typedef krb5_int32	krb5_deltat;

typedef struct _krb5_data {
    int length;
    char *data;
} krb5_data;


#ifdef __STDC__
typedef	void * krb5_pointer;
typedef void const * krb5_const_pointer;
#define PROTOTYPE(x) x
#ifdef NARROW_PROTOTYPES
#define DECLARG(type, val) type val
#define OLDDECLARG(type, val)
#else
#define DECLARG(type, val) val
#define OLDDECLARG(type, val) type val;
#endif /* Narrow prototypes */
#else
/* make const & volatile available without effect */
#define const
#define volatile
typedef char * krb5_pointer;
typedef char * krb5_const_pointer;
#define PROTOTYPE(x) ()
#define DECLARG(type, val) val
#define OLDDECLARG(type, val) type val;
#endif /* __STDC__ */

typedef	krb5_data **	krb5_principal;	/* array of strings */
					/* CONVENTION: realm is first elem. */
/* constant version thereof: */
typedef krb5_data * const *  krb5_const_principal;

#define krb5_princ_realm(princ) ((princ)[0])

#endif /* KRB5_BASE_DEFS__ */
