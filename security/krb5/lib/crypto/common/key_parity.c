/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: key_parity.c,v $
 * Revision 1.1.6.2  1996/02/18  00:10:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:37  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:48:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:13  root]
 * 
 * Revision 1.1.4.2  1992/12/29  14:22:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:04:22  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  20:59:22  pato
 * 	Initial revision
 * 	[1992/05/14  15:24:49  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
*/

/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/crypto/common/key_parity.c,v $
 * $Author: marty $
 *
 * Derived from:
 * Source: /afs/athena.mit.edu/astaff/project/krb5/src/lib/des/RCS/key_parity.c,v
 * Author: jtkohl
 *
 * Copyright 1989, 1990 by the Massachusetts Institute of Technology.
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
 * These routines check and fix parity of encryption keys for the DES
 * algorithm.
 *
 * Under U.S. law, this software may not be exported outside the US
 * without license from the U.S. Commerce department.
 *
 * These routines form the library interface to the DES facilities.
 *
 */


#ifdef MODULE_VERSION_ID
static char rcsid_key_parity_c[] =
"$Id: key_parity.c,v 1.1.6.2 1996/02/18 00:10:08 marty Exp $";
#endif

#include <krb5/krb5.h>
#include <krb5/des_int.h>

#include "odd.h"          /* Load compile-time generated odd_parity table */

/*
 * des_fixup_key_parity: Forces odd parity per byte; parity is bits
 *                       8,16,...64 in des order, implies 0, 8, 16, ...
 *                       vax order.
 */
void
mit_des_fixup_key_parity(key)
     register mit_des_cblock key;
{
    int i;

    for (i=0; i<sizeof(mit_des_cblock); i++)
      key[i] = odd_parity[key[i]];

    return;
}

/*
 * des_check_key_parity: returns true iff key has the correct des parity.
 *                       See des_fix_key_parity for the definition of
 *                       correct des parity.
 */
int
mit_des_check_key_parity(key)
     register mit_des_cblock key;
{
    int i;

    for (i=0; i<sizeof(mit_des_cblock); i++)
      if (key[i] != odd_parity[key[i]])
	return(0);

    return(1);
}
