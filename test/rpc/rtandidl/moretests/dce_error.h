/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dce_error.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:24  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:04:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:05  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:03:58  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:35:21  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  22:13:09  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:05:09  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:34:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



#ifndef DCE_ERROR_H
#define DCE_ERROR_H

/* Constants */

#define dce_c_error_string_len 160 /* length of returned string */

/* Typedefs */

typedef unsigned char dce_error_string_t[dce_c_error_string_len];

/* Prototypes */

extern void dce_error_inq_text (
#ifdef __STDC__
    unsigned long           status_to_convert,
    unsigned char           *error_text,
    int                     *status
#endif    
);
    
#endif
