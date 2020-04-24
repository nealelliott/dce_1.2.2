/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: policy.h,v $
 * Revision 1.1.4.2  1996/02/18  23:03:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:48  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:42  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:18:54  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:55:13  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:54:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  policy.h V=3 10/24/91 //littl/prgy/krb5/kdc
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/kdc/policy.h,v $
 * $Author: marty $
 * $Id: policy.h,v 1.1.4.2 1996/02/18 23:03:17 marty Exp $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * Declarations for policy.c
 */

#include <krb5/copyright.h>

#ifndef __KRB5_KDC_POLICY__
#define __KRB5_KDC_POLICY__

extern krb5_boolean against_postdate_policy PROTOTYPE((krb5_timestamp));
extern krb5_boolean against_flag_policy_as PROTOTYPE((const krb5_kdc_req *));
extern krb5_boolean against_flag_policy_tgs PROTOTYPE((const krb5_kdc_req *,
						       const krb5_ticket *));

#endif /* __KRB5_KDC_POLICY__ */
