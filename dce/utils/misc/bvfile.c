/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: bvfile.c,v $
 * Revision 1.1.4.2  1996/02/18  19:21:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:47  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:37:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:08:54  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:27  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:15:44  annie]
 * 
 * 	expand copyright with OSF copyright text
 * 
 * 	expand copyright with OSF copyright text
 * 
 * Revision 1.1.2.1  1994/07/27  19:19:54  rsalz
 * 	Add bvfile (OT CR 11466).
 * 	[1994/07/27  19:18:37  rsalz]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dce/dce.h>
#include <dce/rpc.h>
#include <dce/dce_utils.h>


void
dce_bindings_to_file(
    FILE			*F,
    rpc_binding_vector_t	*bv,
    error_status_t		*st
)
{
    unsigned32			i;
    unsigned char		*strbh;

    for (i = 0; i < bv->count; i++) {
	rpc_binding_to_string_binding(bv->binding_h[i], &strbh, st);
	if (*st != error_status_ok)
	    return;
	if (fprintf(F, "%s\n", strbh) == EOF) {
	    /* XXX -- will add an error code at some point. */
	    *st = rpc_s_invalid_string_binding;
	    return;
	}
	rpc_string_free(&strbh, st);
    }
}


void
dce_bindings_from_file(
    FILE			*F,
    rpc_binding_vector_t	**bv,
    error_status_t		*st
)
{
    rpc_binding_handle_t	*handles;
    rpc_binding_handle_t	bh;
    rpc_binding_vector_t	*new;
    unsigned32			count;
    unsigned32			i;
    char			buff[256];
    char			*p;

    count = 0;
    if ((handles = malloc(sizeof *handles)) == NULL) {
	*st = rpc_s_no_memory;
	return;
    }
    while (fgets(buff, sizeof buff, F) != NULL) {
	/* Parse this line; ignore it if errors. */
	if ((p = strchr(buff, '\n')) != NULL)
	    *p = '\0';
	rpc_binding_from_string_binding((unsigned char *)buff, &bh, st);
	if (*st != error_status_ok)
	    continue;
	if ((handles = realloc(handles, (count + 1) * sizeof *handles)) == NULL)
	    break;
	handles[count++] = bh;
    }

    /* Got anything? */
    if (count == 0) {
	*st = rpc_s_no_bindings;
	return;
    }

    /* Put the handles into a vector. */
    if ((new = malloc(sizeof *new + count * sizeof *handles)) == NULL) {
	/* XXX should free what we got, oh well. */
	*st = rpc_s_no_memory;
	return;
    }
    for (new->count = count, i = 0; i < count; i++)
	new->binding_h[i] = handles[i];

    /* Set return values. */
    *bv = new;
    *st = error_status_ok;
}
