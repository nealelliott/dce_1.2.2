/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: int-proto.h,v $
 * Revision 1.1.8.2  1996/02/18  23:03:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:02  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:59:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:55  root]
 * 
 * Revision 1.1.4.1  1994/05/11  19:23:18  ahop
 * 	hp_sec_to_osf_1 drop
 * 	merge in krb5 beta 3 changes
 * 	[1994/05/03  19:33:30  ahop]
 * 
 * Revision 1.1.2.2  1992/12/29  15:56:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:36  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  int-proto.h V=4 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/int-proto.h,v $
 * $Author: marty $
 * $Id: int-proto.h,v 1.1.8.2 1996/02/18 23:03:29 marty Exp $
 *
 * Copyright 1990,1991 the Massachusetts Institute of Technology.
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
 * Function prototypes for Kerberos V5 library internal functions.
 */

#ifndef KRB5_INT_FUNC_PROTO__
#define KRB5_INT_FUNC_PROTO__
/* widen prototypes, if needed */
#include <krb5/widen.h>

krb5_error_code krb5_tgtname
    PROTOTYPE((const krb5_data *,
	       const krb5_data *,
	       krb5_principal *));
krb5_error_code krb5_get_cred_via_tgt
    PROTOTYPE((krb5_creds *,
	       const krb5_flags,
	       const krb5_enctype,
	       const krb5_cksumtype,
	       krb5_creds * ));
/* and back to normal... */
#include <krb5/narrow.h>

#endif /* KRB5_INT_FUNC_PROTO__ */


