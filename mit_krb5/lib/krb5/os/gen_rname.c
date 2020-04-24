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
 * $Log: gen_rname.c,v $
 * Revision 1.1.2.1  1996/06/05  21:09:51  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:06:03  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/os/gen_rname.c
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * take a port-style address and unique string, and return
 * a replay cache tag string.
 */

#define NEED_SOCKETS
#include "k5-int.h"
#include "os-proto.h"

krb5_error_code
krb5_gen_replay_name(context, address, uniq, string)
    krb5_context 	  context;
    const krb5_address 	* address;
    const char 		* uniq;
    char 	       ** string;
{
    char * tmp;
    int i;

    if ((*string = malloc(strlen(uniq) + (address->length * 2) + 1)) == NULL)
	return ENOMEM;

    sprintf(*string, "%s", uniq);
    tmp = (*string) + strlen(uniq);
    for (i = 0; i < address->length; i++) {
	sprintf(tmp, "%.2x", address->contents[i] & 0xff);
	tmp += 2;
    }
    return 0;
}
