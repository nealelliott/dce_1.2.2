/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fin_rndkey.c,v $
 * Revision 1.1.6.2  1996/02/18  00:09:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:35  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:48:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:11  root]
 * 
 * Revision 1.1.4.2  1992/12/29  14:22:07  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:04:09  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  20:57:56  pato
 * 	Initial revision
 * 	[1992/05/14  15:22:54  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
*/

/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/crypto/common/fin_rndkey.c,v $
 * $Author: marty $
 *
 * Derived from:
 * Source: /afs/athena.mit.edu/astaff/project/krb5/src/lib/des/RCS/fin_rndkey.c,v
 * Author: jtkohl
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 */

#ifdef MODULE_VERSION_ID
static char rcsid_fin_rndkey_c[] =
"$Id: fin_rndkey.c,v 1.1.6.2 1996/02/18 00:09:58 marty Exp $";
#endif

#include <krb5/krb5.h>
#include <krb5/mit-des.h>
#include <krb5/ext-proto.h>

/*
        free any resources held by "seed" and assigned by init_random_key()
 */

krb5_error_code mit_des_finish_random_key (DECLARG(krb5_pointer *, seed))
OLDDECLARG(krb5_pointer *, seed)
{
    memset((char *)*seed, 0, sizeof(mit_des_random_key_seed) );
    xfree(*seed);
    *seed = 0;
    return 0;
}
