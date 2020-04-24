/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: weak_key.c,v $
 * Revision 1.1.6.2  1996/02/18  00:10:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:42  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:49:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:18  root]
 * 
 * Revision 1.1.4.2  1992/12/29  14:22:55  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:04:48  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  21:01:45  pato
 * 	 OT#2330 - Don't check for weak keys when DES not enabled.
 * 	Initial revision
 * 	[1992/05/14  15:27:43  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
*/

/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/crypto/common/weak_key.c,v $
 * $Author: marty $
 *
 * Derived from:
 * Source: /afs/athena.mit.edu/astaff/project/krb5/src/lib/des/RCS/weak_key.c,v
 * Author: jtkohl
 *
 * Copyright 1989,1990 by the Massachusetts Institute of Technology.
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
 * Under U.S. law, this software may not be exported outside the US
 * without license from the U.S. Commerce department.
 *
 * These routines form the library interface to the DES facilities.
 *
 * Originally written 8/85 by Steve Miller, MIT Project Athena.
 */

#ifdef MODULE_VERSION_ID
static char rcsid_weak_key_c[] =
"$Id: weak_key.c,v 1.1.6.2 1996/02/18 00:10:24 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/des_int.h>

/*
 * The following are the weak DES keys:
 */
static mit_des_cblock weak[16] = {
    /* weak keys */
    {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
    {0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe},
    {0x1f,0x1f,0x1f,0x1f,0x0e,0x0e,0x0e,0x0e},
    {0xe0,0xe0,0xe0,0xe0,0xf1,0xf1,0xf1,0xf1},

    /* semi-weak */
    {0x01,0xfe,0x01,0xfe,0x01,0xfe,0x01,0xfe},
    {0xfe,0x01,0xfe,0x01,0xfe,0x01,0xfe,0x01},

    {0x1f,0xe0,0x1f,0xe0,0x0e,0xf1,0x0e,0xf1},
    {0xe0,0x1f,0xe0,0x1f,0xf1,0x0e,0xf1,0x0e},

    {0x01,0xe0,0x01,0xe0,0x01,0xf1,0x01,0xf1},
    {0xe0,0x01,0xe0,0x01,0xf1,0x01,0xf1,0x01},

    {0x1f,0xfe,0x1f,0xfe,0x0e,0xfe,0x0e,0xfe},
    {0xfe,0x1f,0xfe,0x1f,0xfe,0x0e,0xfe,0x0e},

    {0x01,0x1f,0x01,0x1f,0x01,0x0e,0x01,0x0e},
    {0x1f,0x01,0x1f,0x01,0x0e,0x01,0x0e,0x01},

    {0xe0,0xfe,0xe0,0xfe,0xf1,0xfe,0xf1,0xfe},
    {0xfe,0xe0,0xfe,0xe0,0xfe,0xf1,0xfe,0xf1}
};

/*
 * mit_des_is_weak_key: returns true iff key is a [semi-]weak des key.
 *
 * Requires: key has correct odd parity.
 */
int
mit_des_is_weak_key(key)
     mit_des_cblock key;
{
#ifdef DES_NEUTER
    return 0;
#else
    int i;
    mit_des_cblock *weak_p = weak;

    for (i = 0; i < (sizeof(weak)/sizeof(mit_des_cblock)); i++) {
	if (!memcmp((char *)weak_p++,(char *)key,sizeof(mit_des_cblock)))
	    return 1;
    }

    return 0;
#endif
}
