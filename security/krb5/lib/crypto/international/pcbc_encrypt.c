/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pcbc_encrypt.c,v $
 * Revision 1.1.6.2  1996/02/18  00:11:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:58  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:50:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:32  root]
 * 
 * Revision 1.1.4.2  1992/12/29  14:25:18  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:07:01  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  21:33:21  pato
 * 	Initial revision
 * 	[1992/05/14  15:13:08  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
*/

/*
 * des_pcbc_encrypt.c - 
 *  Hollow implementation to encrypt a string of characters in error 
 *  propagation mode.
 *
 *  Export version performs no encryption - copies input data to
 *  output argument.
 */

#include <krb5/krb5.h>
#include <krb5/mit-des.h>
#include <krb5/des_int.h>

#include <string.h>

/*
 * des_pcbc_encrypt - {en,de}crypt a stream in PCBC mode
 */
int des_pcbc_encrypt(DECLARG(mit_des_cblock *, in),
                        DECLARG(mit_des_cblock *, out),
                        DECLARG(long, length),
                        DECLARG(mit_des_key_schedule, schedule),
                        DECLARG(mit_des_cblock, ivec),
                        DECLARG(int, encrypt))
OLDDECLARG(mit_des_cblock *, in)
OLDDECLARG(mit_des_cblock *, out)
OLDDECLARG(long, length)
OLDDECLARG(mit_des_key_schedule, schedule)
OLDDECLARG(mit_des_cblock, ivec)
OLDDECLARG(int, encrypt)
{
    memcpy((char *) out, (char *) in, length);
    return 0;
}

