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
 * $Log: init_ctx.c,v $
 * Revision 1.1.2.2  1996/11/14  19:41:44  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/14  19:39:48  arvind]
 *
 * Revision 1.1.2.1  1996/06/05  21:02:52  arvind
 * 	     DCE1.2.2 Drop 2 to OSF
 * 	     [1996/06/05  19:59:54  arvind]
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/init_ctx.c
 *
 * Copyright 1994 by the Massachusetts Institute of Technology.
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
 * krb5_init_contex()
 */

#include "k5-int.h"

krb5_error_code INTERFACE
krb5_init_context(context)
	krb5_context *context;
{
	krb5_context ctx;
	krb5_error_code retval;

	*context = 0;

	ctx = malloc(sizeof(struct _krb5_context));
	if (!ctx)
		return ENOMEM;
	memset(ctx, 0, sizeof(struct _krb5_context));
	ctx->magic = KV5M_CONTEXT;

	/* Set the default encryption types, possible defined in krb5/conf */
	if (retval = krb5_set_default_in_tkt_etypes(ctx, NULL))
		goto cleanup;

	if (retval = krb5_os_init_context(ctx))
		goto cleanup;
	

	ctx->default_realm = 0;

	*context = ctx;
	return 0;

cleanup:
	krb5_free_context(ctx);
	return retval;
}

void
krb5_free_context(ctx)
	krb5_context	ctx;
{
     krb5_os_free_context(ctx);

     if (ctx->etypes)
          free(ctx->etypes);

     if (ctx->default_realm)
	  free(ctx->default_realm);

     ctx->magic = 0;
     free(ctx);
}

/*
 * Set the desired default etypes, making sure they are valid.
 */
krb5_error_code
krb5_set_default_in_tkt_etypes(context, etypes)
	krb5_context context;
	const krb5_enctype *etypes;
{
    krb5_enctype * new_etypes;
    int i;

    if (etypes) {
	for (i = 0; etypes[i]; i++) {
	    if (!valid_etype(etypes[i])) 
		return KRB5_PROG_ETYPE_NOSUPP;
	}

	/* Now copy the default etypes into the context pointer */
	if (new_etypes = (krb5_enctype *)malloc(sizeof(krb5_enctype) * i))
	    memcpy(new_etypes, etypes, sizeof(krb5_enctype) * i);
	else
	    return ENOMEM;

    } else {
	i = 2;

	/* Should reset the list to the runtime defaults */
	if (new_etypes = (krb5_enctype *)malloc(sizeof(krb5_enctype) * i)) {
	    i = 0;
	    if (valid_etype(ETYPE_DES_CBC_MD5))
		new_etypes[i++] = ETYPE_DES_CBC_MD5;
	    if (valid_etype(ETYPE_DES_CBC_CRC))
		new_etypes[i++] = ETYPE_DES_CBC_CRC;
	    if (i == 0)
		abort();
	} else {
	    return ENOMEM;
	}
    }

    if (context->etypes) 
        free(context->etypes);
    context->etypes = new_etypes;
    context->etype_count = i;
    return 0;
}

krb5_error_code
krb5_get_default_in_tkt_etypes(context, etypes)
    krb5_context context;
    krb5_enctype **etypes;
{
    krb5_enctype * old_etypes;

    if (old_etypes = (krb5_enctype *)malloc(sizeof(krb5_enctype) *
				(context->etype_count + 1))) {
	memcpy(old_etypes, context->etypes, sizeof(krb5_enctype) * 
		  		context->etype_count);
	old_etypes[context->etype_count] = 0;
    } else {
	return ENOMEM;
    }

    *etypes = old_etypes;
    return 0;
	
}
