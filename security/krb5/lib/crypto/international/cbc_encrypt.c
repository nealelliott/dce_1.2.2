/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cbc_encrypt.c,v $
 * Revision 1.1.9.2  1996/02/18  00:10:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:52  marty]
 *
 * Revision 1.1.9.1  1995/12/08  17:49:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:27  root]
 * 
 * Revision 1.1.7.1  1993/11/16  20:57:12  zeliff
 * 	OT9495: international crypto doesn't build
 * 	[1993/11/16  20:54:43  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/29  14:24:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:05:48  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  21:24:03  pato
 * 	Initial revision
 * 	[1992/05/14  15:09:55  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
*/

/*
 * des_cbc_encrypt.c - 
 *  a hollow implementation of the DES cipher function in cbc mode
 *
 *  Export version performs no encryption - copies input data to
 *  output argument.
 */

#include <krb5/krb5.h>
#include <krb5/mit-des.h>
#include <krb5/des_int.h>

#include <string.h>

/*
 * des_cbc_encrypt - {en,de}crypt a stream in CBC mode
 */
int mit_des_cbc_encrypt(DECLARG(krb5_octet *, in),
                        DECLARG(krb5_octet *, out),
                        DECLARG(krb5_int32, length),
                        DECLARG(mit_des_key_schedule, schedule),
                        DECLARG(krb5_octet *, ivec),
                        DECLARG(int, encrypt))
OLDDECLARG(krb5_octet *, in)
OLDDECLARG(krb5_octet *, out)
OLDDECLARG(krb5_int32, length)
OLDDECLARG(mit_des_key_schedule, schedule)
OLDDECLARG(krb5_octet *, ivec)
OLDDECLARG(int, encrypt)
{
    memcpy((char *) out, (char *) in, length);
    return 0;
}

