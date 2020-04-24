/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: des_neuter.c,v $
 * Revision 1.1.8.2  1996/02/17  23:59:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:13  marty]
 *
 * Revision 1.1.8.1  1995/12/08  00:13:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:21  root]
 * 
 * Revision 1.1.6.2  1994/09/20  18:18:21  tom
 * 	Correct prototypes to match des.h
 * 	[1994/09/20  17:13:13  tom]
 * 
 * Revision 1.1.6.1  1994/01/21  22:30:48  cbrooks
 * 	Code Cleanup
 * 	[1994/01/21  20:20:51  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  22:34:34  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:49:37  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  19:35:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:15:15  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  17:36:15  rsalz
 * 	"Changed pas part of rpc6 drop."
 * 	[1992/05/01  17:33:55  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:17:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <dce/dce.h>
#include <des.h>

int des_key_sched(k,s)
     des_cblock *k;
     des_key_schedule s;
{
    memset( s, 0, sizeof(des_key_schedule)) ;
    return 0;
}

unsigned long des_cbc_cksum(in,out,length,key,iv)
    des_cblock  *in;            /* >= length bytes of inputtext */
    des_cblock  *out;           /* >= length bytes of outputtext */
    register long length;       /* in bytes */
    des_key_schedule key;           /* precomputed key schedule */
    des_cblock  iv;             /* 8 bytes of ivec */
{
    memset( out, 0, sizeof(des_cblock)) ;
}

int des_ecb_encrypt(in, out, schedule, encrypt)
        des_cblock *in;
        des_cblock *out;
        des_key_schedule schedule;
        int encrypt;
{

    memcpy( out, in, sizeof(des_cblock)) ;
    return 0;
}

int des_cbc_encrypt(in, out, length, schedule, ivec, encrypt)
        des_cblock *in;
        des_cblock *out;
        signed32 length;
        des_key_schedule schedule;
        des_cblock ivec;
        int encrypt;
{
    memcpy( out, in, length ) ;
    return 0;
}

int sec_des_is_weak_key(key)
     des_cblock key;
{
   return 0;
}
