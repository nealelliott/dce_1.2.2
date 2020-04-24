/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ecb_encrypt.c,v $
 * Revision 1.1.6.2  1996/02/18  00:11:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:56  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:50:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:31  root]
 * 
 * Revision 1.1.4.2  1992/12/29  14:25:06  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:06:52  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  21:32:25  pato
 * 	Initial revision
 * 	[1992/05/14  15:12:08  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
*/

#if !defined(lint) && !defined(SABER)
static char *VersionID = "@(#)ecb_encrypt.c	2 - 01/09/91";
#endif	/* lint */

/*
 * des_ecb_encrypt.c - 
 *  A hollow implementation to do an encryption in ECB mode
 *
 *  Export version performs no encryption - copies input data to
 *  output argument.
 */

#include <krb5/krb5.h>
#include <krb5/mit-des.h>
#include <krb5/des_int.h>

#include <string.h>

/*
 * des_ecb_encrypt - {en,de}crypt a block in ECB mode
 */
int mit_des_ecb_encrypt(DECLARG(krb5_octet *, in),
                        DECLARG(krb5_octet *, out),
                        DECLARG(mit_des_key_schedule, schedule),
                        DECLARG(int, encrypt))
OLDDECLARG(krb5_octet *, in)
OLDDECLARG(krb5_octet *, out)
OLDDECLARG(mit_des_key_schedule, schedule)
OLDDECLARG(int, encrypt)
{
    memcpy((char *) out, (char *) in, sizeof(mit_des_cblock));
    return 0;
}
