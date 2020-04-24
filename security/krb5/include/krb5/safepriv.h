/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: safepriv.h,v $
 * Revision 1.1.4.2  1996/02/18  23:03:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:24  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:02:01  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:54:03  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  safepriv.h V=2 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/safepriv.h,v $
 * $Author: marty $
 * $Id: safepriv.h,v 1.1.4.2 1996/02/18 23:03:06 marty Exp $
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
 * #defines for SAFE and PRIV message options.
 */


#ifndef KRB5_SAFE_PRIV__
#define KRB5_SAFE_PRIV__

#define KRB5_SAFE_NOTIME	0x1
#define KRB5_SAFE_DOSEQUENCE	0x2

#define KRB5_PRIV_NOTIME	0x1
#define KRB5_PRIV_DOSEQUENCE	0x2

#endif /* KRB5_SAFE_PRIV__ */
