/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: speed.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:15  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:48  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:14:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:49  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:52:36  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:20:06  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:50:55  rsalz
 * 	Edited as part of rpc6 drop.
 * 	[1992/05/01  00:57:29  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:42:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  speed.c V=1 11/07/90 //crown/prgy/krb5/lib/destoo
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
**    ** OSF DCE Confidential - DCE Security Component BL 6   **
**    ** OSF DCE AUTH 3 - prerelease                          **
**
*/
#ifndef _KERNEL
#include <stdio.h>
#else
#define fprintf(str, msg) printf(msg)
#endif
#include <des.h>
#ifndef _KERNEL
int main(argc, argv)
int argc;
char*argv[];
{
    int i;
    if(argc != 2) {
        fprintf(stderr,"Usage: speed #\n");
        exit(1);
    }
    i = atoi(argv[1]);
    for(;i;i--) {
        des_test();
    }

}
#endif

#ifdef UCRYPT
int des_test() {
    char key[64];
    char block[64];
    setkey(key);
    encrypt(block,1);
}

#else

int des_test() {
    des_cblock src;
    des_cblock dest;
    des_key_schedule sched;

    /* for now, src, dest, and sched all random? */
    des_ecb_encrypt(src,dest,sched,1);
}
#endif

#ifdef ACRYPT
int des_ecb_encrypt(clear, cipher, schedule, encrypt)
    unsigned long *clear;
    unsigned long *cipher;
    int encrypt;
    des_key_schedule schedule;
{
    if (encrypt)
        des_$encrypt (clear, cipher, schedule);
    else
        des_$decrypt (clear, cipher, schedule);

    return 0;
}
#endif
