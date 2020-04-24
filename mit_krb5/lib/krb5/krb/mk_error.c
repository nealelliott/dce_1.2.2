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
 * $Log: mk_error.c,v $
 * Revision 1.1.2.1  1996/06/05  21:03:47  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:00:42  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/mk_error.c
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 *
 * krb5_mk_error() routine.
 */

#include "k5-int.h"

/*
 formats the error structure *dec_err into an error buffer *enc_err.

 The error buffer storage is allocated, and should be freed by the
 caller when finished.

 returns system errors
 */
krb5_error_code
krb5_mk_error(context, dec_err, enc_err)
    krb5_context context;
    const krb5_error *dec_err;
    krb5_data *enc_err;
{
    krb5_error_code retval;
    krb5_data *new_enc_err;

    if (retval = encode_krb5_error(dec_err, &new_enc_err))
	return(retval);
    *enc_err = *new_enc_err;
    krb5_xfree(new_enc_err);
    return 0;
}
