/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cksum.c,v $
 * Revision 1.1.8.2  1996/02/18  00:10:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:53  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:49:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:28  root]
 * 
 * Revision 1.1.6.1  1993/11/16  20:57:14  zeliff
 * 	OT9495: international crypto doesn't build
 * 	[1993/11/16  20:54:47  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/29  14:24:46  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:06:38  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  21:31:02  pato
 * 	Initial revision
 * 	[1992/05/14  15:10:32  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
*/

#include <krb5/krb5.h>
#include <krb5/mit-des.h>
#include <krb5/des_int.h>

#include <string.h>

/*
 * des_cbc_cksum.c - 
 *  Hollow implementation to compute an 8 byte checksum using DES in CBC mode
 *
 *  Export version performs no encryption - constant function will zero
 *  output data.
 *
 *  Note: in, out, and ivec are really of type mit_des_cblock
 */

unsigned long
des_cbc_cksum(  DECLARG(krb5_octet *, in),
                    DECLARG(krb5_octet *, out),
                    DECLARG(krb5_int32, length),
                    DECLARG(mit_des_key_schedule, schedule),
                    DECLARG(krb5_octet *, ivec))
OLDDECLARG(krb5_octet *, in)
OLDDECLARG(krb5_octet *, out)
OLDDECLARG(long, length)
OLDDECLARG(mit_des_key_schedule, schedule)
OLDDECLARG(krb5_octet *, ivec)
{
    memset((char *) out, 0, sizeof(mit_des_cblock));
    return 0;
}

void mit_des_cbc_cksum(  
                    DECLARG(krb5_octet *, in),
                    DECLARG(krb5_octet *, out),
                    DECLARG(krb5_int32, length),
                    DECLARG(mit_des_key_schedule, key),
                    DECLARG(krb5_octet *, iv))
OLDDECLARG(krb5_octet *, in)            /* >= length bytes of inputtext */
OLDDECLARG(krb5_octet *, out)           /* >= length bytes of outputtext */
OLDDECLARG(long, length)                /* in bytes */
OLDDECLARG(mit_des_key_schedule, key)   /* precomputed key schedule */
OLDDECLARG(krb5_octet *, iv)            /* 8 bytes of ivec */
{
    (void) des_cbc_cksum(in, out, length, key, iv);
}

