/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krbconfig.c,v $
 * Revision 1.1.6.2  1996/02/18  00:14:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:46  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:59:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:56  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:56:46  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  krbconfig.c V=2 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/krbconfig.c,v $
 * $Author: marty $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * Configuration variables for libkrb.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_krbconfig_c [] =
"$Id: krbconfig.c,v 1.1.6.2 1996/02/18 00:14:11 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

krb5_deltat krb5_clockskew = 5 * 60;	/* five minutes */
krb5_cksumtype krb5_kdc_req_sumtype = CKSUMTYPE_RSA_MD4;
krb5_flags krb5_kdc_default_options = KDC_OPT_RENEWABLE_OK;
