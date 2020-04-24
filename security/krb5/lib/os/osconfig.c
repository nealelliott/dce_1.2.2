/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: osconfig.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:58  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:36  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:30:55  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:18:29  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  osconfig.c V=2 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/osconfig.c,v $
 * $Author: marty $
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
 * Definition of default configuration parameters.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_config_fn_c [] =
"$Id: osconfig.c,v 1.1.4.2 1996/02/18 00:15:39 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/osconf.h>
#include <krb5/config.h>


char *krb5_config_file = DEFAULT_CONFIG_FILENAME;
char *krb5_trans_file = DEFAULT_TRANS_FILENAME;
char *krb5_defkeyname  = DEFAULT_KEYTAB_NAME;
#ifdef USE_DBM_LNAME
char *krb5_lname_file = DEFAULT_LNAME_FILENAME;
#endif

int krb5_max_dgram_size = MAX_DGRAM_SIZE;
int krb5_max_skdc_timeout = MAX_SKDC_TIMEOUT;
int krb5_skdc_timeout_shift = SKDC_TIMEOUT_SHIFT;
int krb5_skdc_timeout_1 = SKDC_TIMEOUT_1;

#ifdef KRB5_USE_INET
char *krb5_kdc_udp_portname = KDC_PORTNAME;
#endif

char *krb5_default_pwd_prompt1 = DEFAULT_PWD_STRING1;
char *krb5_default_pwd_prompt2 = DEFAULT_PWD_STRING2;
