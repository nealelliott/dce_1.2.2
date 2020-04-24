/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: extern.c,v $
 * Revision 1.1.6.1  1996/10/03  14:59:03  arvind
 * 	define krb5_context here as a beta6 placebo.
 * 	[1996/09/16  20:47 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.4.2  1996/02/18  00:08:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:03  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:44:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:32  root]
 * 
 * Revision 1.1.2.3  1993/01/26  21:50:53  ahop
 * 	bl6d: initialize empty_string to {0,0} instead of {0,""} to prevent bad frees.
 * 	[1993/01/18  23:50:44  ahop]
 * 
 * Revision 1.1.2.2  1992/12/29  14:02:45  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:54:38  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:54:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/* 
 *
 * Copyright (c) Hewlett-Packard Company 1991, 1993
 * Unpublished work. All Rights Reserved.
 *
 */
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/kdc/extern.c,v $
 * $Author: arvind $
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
 * allocations of extern stuff
 */

#ifdef OSF_DCE
#define OSF_DCE_ARCHAISM
#endif

#include <krb5/copyright.h>

#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/kdb_dbm.h>

#include "extern.h"

/* real declarations of KDC's externs */
krb5_rcache kdc_rcache;

krb5_data empty_string = {0, (char *)0};
krb5_timestamp kdc_infinity = KRB5_INT32_MAX; /* XXX */

krb5_deltat max_life_for_realm = KRB5_KDB_MAX_LIFE;		/* XXX parameter per-realm? */
krb5_deltat max_renewable_life_for_realm = KRB5_KDB_MAX_RLIFE; /* XXX param per-realm? */
krb5_encrypt_block master_encblock;

krb5_keyblock master_keyblock;
krb5_principal master_princ;

volatile int signal_requests_exit = 0;	/* gets set when signal hits */

char *dbm_db_name = DEFAULT_DBM_FILE;

krb5_keyblock tgs_key;
krb5_kvno tgs_kvno;

static krb5_data tgs_name = {sizeof(TGTNAME)-1, TGTNAME};
krb5_data *tgs_server[4] = {0, &tgs_name, 0, 0};

#ifdef OSF_DCE_ARCHAISM
krb5_context kdc_context = NULL;
#endif
