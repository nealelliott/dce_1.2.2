/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: f_priv_enc.c,v $
 * Revision 1.1.4.2  1996/02/18  00:12:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:53:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:00  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:28:22  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:09:16  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  f_priv_enc.c V=3 10/24/91 //littl/prgy/krb5/lib/free
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/free/f_priv_enc.c,v $
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
 * krb5_free_priv_enc_part()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_f_priv_enc_c [] =
"$Id: f_priv_enc.c,v 1.1.4.2 1996/02/18 00:12:06 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

void
krb5_free_priv_enc_part(val)
register krb5_priv_enc_part *val;
{
    if (val->user_data.data)
	xfree(val->user_data.data);
    if (val->r_address)
	krb5_free_address(val->r_address);
    if (val->s_address)
	krb5_free_address(val->s_address);
    xfree(val);
    return;
}
