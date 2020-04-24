/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: make_sched.c,v $
 * Revision 1.1.6.2  1996/02/18  00:11:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:57  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:50:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:31  root]
 * 
 * Revision 1.1.4.2  1992/12/29  14:25:13  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:06:57  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  21:32:52  pato
 * 	Initial revision
 * 	[1992/05/14  15:12:37  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
*/

/*
 * des_make_sched.c - 
 *  Hollow implementation to permute a DES key, returning the resulting
 *  key schedule
 *
 *  Export version performs no encryption - constant function will zero
 *  output data.
 */

#include <krb5/krb5.h>
#include <krb5/mit-des.h>
#include <krb5/des_int.h>

#include <string.h>

/*
 * Permute the key to give us our key schedule.
 */
int make_key_sched( DECLARG(krb5_octet *, key),
                    DECLARG(mit_des_key_schedule, schedule))
OLDDECLARG(krb5_octet *, key)
OLDDECLARG(mit_des_key_schedule, schedule)
{
    memset((char *) key, 0, sizeof(mit_des_cblock));
    return 0;
}
