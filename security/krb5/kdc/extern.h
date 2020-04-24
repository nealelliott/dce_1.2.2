/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: extern.h,v $
 * Revision 1.1.6.1  1996/10/03  14:59:14  arvind
 * 	placebos for beta6 code.
 * 	[1996/09/16  20:47 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.4.2  1996/02/18  23:03:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:44  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:44:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:32  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:02:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:54:42  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:54:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  extern.h V=3 10/24/91 //littl/prgy/krb5/kdc
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/kdc/extern.h,v $
 * $Author: arvind $
 * $Id: extern.h,v 1.1.6.1 1996/10/03 14:59:14 arvind Exp $
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
 * <<< Description >>>
 */

#include <krb5/copyright.h>

#ifndef __KRB5_KDC_EXTERN__
#define __KRB5_KDC_EXTERN__

/* various externs for KDC */
extern krb5_rcache kdc_rcache;                /* KDC's replay cache */

extern krb5_data empty_string;		/* an empty string */
extern krb5_timestamp kdc_infinity;	/* greater than all other timestamps */

extern krb5_deltat max_life_for_realm;	/* XXX should be a parameter? */
extern krb5_deltat max_renewable_life_for_realm; /* XXX should be a parameter? */
extern krb5_encrypt_block master_encblock;

extern krb5_keyblock master_keyblock;
extern krb5_principal master_princ;

extern volatile int signal_requests_exit;
extern char *dbm_db_name;

extern krb5_keyblock tgs_key;
extern krb5_kvno tgs_kvno;
extern krb5_data *tgs_server[4];

#ifdef OSF_DCE_ARCHAISM
extern krb5_context kdc_context;
#endif

#endif /* __KRB5_KDC_EXTERN__ */
