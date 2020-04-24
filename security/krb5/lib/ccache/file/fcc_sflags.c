/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_sflags.c,v $
 * Revision 1.1.6.2  1996/02/18  00:09:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:22  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:47:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:02  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:46  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:54:25  sommerfeld]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_sflags.c,v $
 * $Author: marty $
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
 * This file contains the source code for krb5_fcc_set_flags.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_set_flags_c[] =
"$Id: fcc_sflags.c,v 1.1.6.2 1996/02/18 00:09:40 marty Exp $";
#endif /* !lint && !SABER */


#include "fcc.h"

/*
 * Requires:
 * id is a cred cache returned by krb5_fcc_resolve or
 * krb5_fcc_generate_new, but has not been opened by krb5_fcc_initialize.
 *
 * Modifies:
 * id
 * 
 * Effects:
 * Sets the operational flags of id to flags.
 */
krb5_error_code
krb5_fcc_set_flags(id, flags)
   krb5_ccache id;
   krb5_flags flags;
{
    /* XXX This should check for illegal combinations, if any.. */
    if (flags & KRB5_TC_OPENCLOSE) {
	/* asking to turn on OPENCLOSE mode */
	if (!OPENCLOSE(id)) {
	    (void) krb5_fcc_close_file (id);
	}
    } else {
	/* asking to turn off OPENCLOSE mode, meaning it must be
	   left open.  We open if it's not yet open */
	MAYBE_OPEN(id, FCC_OPEN_RDONLY);
    }

    ((krb5_fcc_data *) id->data)->flags = flags;
    return KRB5_OK;
}

