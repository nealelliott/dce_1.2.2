/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: preauth.h,v $
 * Revision 1.1.2.1  1996/06/05  20:28:42  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:28:04  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * include/krb5/preauth.h
 *
 * (Originally written by Glen Machin at Sandia Labs.)
 *
 * Copyright 1992, 1995 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
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
 * Sandia National Laboratories also makes no representations about the 
 * suitability of the modifications, or additions to this software for 
 * any purpose.  It is provided "as is" without express or implied warranty.
 * 
 */
#ifndef KRB5_PREAUTH__
#define KRB5_PREAUTH__

#define MAX_PREAUTH_SIZE 20	/* Maximum size of PreAuthenticator.data */

/*
 * Note: these typedefs are subject to change.... [tytso:19920903.1609EDT]
 */
typedef krb5_error_code (krb5_preauth_obtain_proc)
    PROTOTYPE((krb5_context, krb5_principal client, krb5_address **src_addr,
	       krb5_pa_data *pa_data));

typedef krb5_error_code (krb5_preauth_verify_proc)
    PROTOTYPE((krb5_context, krb5_principal client, krb5_address **src_addr,
	       krb5_data *data));

typedef struct _krb5_preauth_ops {
    krb5_magic magic;
    int     type;
    int	flags;
    krb5_preauth_obtain_proc	*obtain;
    krb5_preauth_verify_proc	*verify;
} krb5_preauth_ops;

/*
 * Preauthentication property flags
 */
#define KRB5_PREAUTH_FLAGS_ENCRYPT	0x00000001
#define KRB5_PREAUTH_FLAGS_HARDWARE	0x00000002

#if 0
krb5_error_code get_random_padata
    PROTOTYPE((krb5_principal client, krb5_address **src_addr,
	       krb5_pa_data *data));

krb5_error_code verify_random_padata
    PROTOTYPE((krb5_principal client, krb5_address **src_addr,
	       krb5_data *data));
#endif

krb5_error_code get_unixtime_padata
    PROTOTYPE((krb5_context, krb5_principal client, 
	       krb5_address **src_addr, krb5_pa_data *data));

krb5_error_code verify_unixtime_padata
    PROTOTYPE((krb5_context, krb5_principal client, krb5_address **src_addr,
	       krb5_data *data));

krb5_error_code get_securid_padata
    PROTOTYPE((krb5_context, krb5_principal client, krb5_address **src_addr,
	       krb5_pa_data *data));

krb5_error_code verify_securid_padata
    PROTOTYPE((krb5_context, krb5_principal client, krb5_address **src_addr,
	       krb5_data *data));

#endif /* KRB5_PREAUTH__ */
