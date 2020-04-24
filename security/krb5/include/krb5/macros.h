/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: macros.h,v $
 * Revision 1.1.4.2  1996/02/18  23:02:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:13  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:12  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:00:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:53:06  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  macros.h V=2 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/macros.h,v $
 * $Author: marty $
 * $Id: macros.h,v 1.1.4.2 1996/02/18 23:02:44 marty Exp $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * macros used in Kerberos code.
 */


#ifndef KRB5_MACROS__
#define KRB5_MACROS__

#define krb5_princ_aref(princ, n, plen) (char *)(*plen = princ[n]->length, princ[n]->data)

#endif /* KRB5_MACROS__ */
