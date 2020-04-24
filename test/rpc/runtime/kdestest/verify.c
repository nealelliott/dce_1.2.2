/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: verify.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:17  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:49  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:14:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:50  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:52:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:20:12  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:51:01  rsalz
 * 	Edited as part of rpc6 drop.
 * 	[1992/05/01  00:57:34  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:42:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  verify.c V=1 11/07/90 //crown/prgy/krb5/lib/destoo
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
**    ** OSF DCE Confidential - DCE Security Component BL 6   **
**    ** OSF DCE AUTH 3 - prerelease                          **
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/test/rpc/runtime/kdestest/verify.c,v $
 * $Author: marty $
 *
 * Copyright 1988 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <mit-copyright.h>.
 *
 * Program to test the correctness of the DES library
 * implementation.
 *
 * exit returns  0 ==> success
 *              -1 ==> error
 */

#ifndef lint
static char rcsid_verify_c[] =
"$Header: /u0/rcs_trees/dce/rcs/test/rpc/runtime/kdestest/verify.c,v 1.1.6.2 1996/02/17 23:23:17 marty Exp $";
#endif  lint

#ifndef _KERNEL
#include <mit-copyright.h>
#include <stdio.h>
#include <errno.h>
#include <des.h>
#else
#include <dce/sec_authn.h>
#endif

static char *progname;
static int nflag = 2;
static int vflag;
static int mflag;
static int zflag;
static int pid;
static int des_debug;
static sec_des_key_schedule KS;
static unsigned char cipher_text[64];
static unsigned char clear_text[64] = "Now is the time for all " ;
static unsigned char clear_text2[64] = "7654321 Now is the time for ";
static unsigned char clear_text3[64] = {2,0,0,0, 1,0,0,0};
static unsigned char output[64];
static sec_des_block zero_text = {0x0,0,0,0,0,0,0,0};
static sec_des_block msb_text = {0x0,0,0,0,0,0,0,0x40}; /* to ANSI MSB */
static sec_des_block *input;

/* 0x0123456789abcdef */
static sec_des_key default_key = {
  0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef 
};
static sec_des_key key2 = { 0x08,0x19,0x2a,0x3b,0x4c,0x5d,0x6e,0x7f };
static sec_des_key key3  = { 0x80,1,1,1,1,1,1,1 };
static sec_des_block s_key;
static sec_des_block default_ivec = {
    0x12,0x34,0x56,0x78,0x90,0xab,0xcd,0xef
};
static sec_des_block *ivec;
static sec_des_key zero_key = {1,1,1,1,1,1,1,1}; /* just parity bits */
static int i,j;

/*
 * Can also add :
 * plaintext = 0, key = 0, cipher = 0x8ca64de9c1b123a7 (or is it a 1?)
 */

void do_encrypt(sec_des_block *in, sec_des_block *out);
void do_decrypt(sec_des_block *in, sec_des_block *out);

#ifdef _KERNEL
des_verify_main(argc, argv)
#else
main(argc,argv)
#endif
    int argc;
    char *argv[];
{
    /* Local Declarations */
    long in_length;

    progname=argv[0];           /* salt away invoking program */

    /* Assume a long is four bytes */
    if (sizeof(long) != 4) {
        printf("\nERROR,  size of long is %d",sizeof(long));
	return;
    }

    while (--argc > 0 && (*++argv)[0] == '-')
        for (i=1; argv[0][i] != '\0'; i++) {
            switch (argv[0][i]) {

                /* debug flag */
            case 'd':
                des_debug=3;
                continue;

            case 'z':
                zflag = 1;
                continue;

            case 'm':
                mflag = 1;
                continue;

            default:
                printf("%s: illegal flag \"%c\" ",
                       progname,argv[0][i]);
		return;
            }
        };

    if (argc) {
        printf("Usage: %s [-dmz]\n", progname);
	return;
    }

    /* use known input and key */

    /* ECB zero text zero key */
    if (zflag) {
        input = &zero_text;
        sec_des_key_sched(&zero_key,KS);
        printf("plaintext = key = 0, cipher = 0x8ca64de9c1b123a7\n");
        do_encrypt(input,(sec_des_block *)cipher_text);
        printf("        cipher  = (low to high bytes)\n                ");
        for (j = 0; j<=7; j++)
            printf("%02x ",cipher_text[j]);
        printf("\n");
        do_decrypt((sec_des_block *)output,(sec_des_block *)cipher_text);
        return;
    }

    if (mflag) {
        input = &msb_text;
        sec_des_key_sched(&key3,KS);
        printf("plaintext = 0x00 00 00 00 00 00 00 40, ");
        printf("key = 0, cipher = 0x??\n");
        do_encrypt(input,(sec_des_block *)cipher_text);
        printf("        cipher  = (low to high bytes)\n                ");
        for (j = 0; j<=7; j++) {
            printf("%02x ",cipher_text[j]);
        }
        printf("\n");
        do_decrypt((sec_des_block *)output,(sec_des_block *)cipher_text);
        return;
    }

    /* ECB mode Davies and Price */
    {
        input = &zero_text;
        sec_des_key_sched(&key2,KS);
        printf("Examples per FIPS publication 81, keys ivs and cipher\n");
        printf("in hex.  These are the correct answers, see below for\n");
        printf("the actual answers.\n\n");
        printf("Examples per Davies and Price.\n\n");
        printf("EXAMPLE ECB        key = 08192a3b4c5d6e7f\n");
        printf("        clear = 0\n");
        printf("        cipher = 25 dd ac 3e 96 17 64 67\n");
        printf("ACTUAL ECB\n");
        printf("        clear \"%s\"\n", input);
        do_encrypt(input,(sec_des_block *)cipher_text);
        printf("        cipher  = (low to high bytes)\n                ");
        for (j = 0; j<=7; j++)
            printf("%02x ",cipher_text[j]);
        printf("\n\n");
        do_decrypt((sec_des_block *)output,(sec_des_block *)cipher_text);
    }

    /* ECB mode */
    {
        sec_des_key_sched(&default_key,KS);
        input = (sec_des_block *)clear_text;
        ivec = &default_ivec;
        printf("EXAMPLE ECB        key = 0123456789abcdef\n");
        printf("        clear = \"Now is the time for all \"\n");
        printf("        cipher = 3f a4 0e 8a 98 4d 48 15 ...\n");
        printf("ACTUAL ECB\n        clear \"%s\"",input);
        do_encrypt(input,(sec_des_block *)cipher_text);
        printf("\n        cipher      = (low to high bytes)\n                ");
        for (j = 0; j<=7; j++) {
            printf("%02x ",cipher_text[j]);
        }
        printf("\n\n");
        do_decrypt((sec_des_block *)output,(sec_des_block *)cipher_text);
    }

    /* CBC mode */
    printf("EXAMPLE CBC        key = 0123456789abcdef");
    printf("        iv = 1234567890abcdef\n");
    printf("        clear = \"Now is the time for all \"\n");
    printf("        cipher =        e5 c7 cd de 87 2b f2 7c\n");
    printf("                        43 e9 34 00 8c 38 9c 0f\n");
    printf("                        68 37 88 49 9a 7c 05 f6\n");

    printf("ACTUAL CBC\n        clear \"%s\"\n",input);
    in_length = strlen(input);
    sec_des_cbc_encrypt(input,(sec_des_block *)cipher_text,(long) in_length,KS,ivec,1);
    printf("        ciphertext = (low to high bytes)\n");
    for (i = 0; i <= 7; i++) {
        printf("                ");
        for (j = 0; j <= 7; j++) {
            printf("%02x ",cipher_text[i*8+j]);
        }
        printf("\n");
    }
    sec_des_cbc_encrypt((sec_des_block *)cipher_text,
      (sec_des_block *)clear_text,(long) in_length,KS,ivec,0);
    printf("        decrypted clear_text = \"%s\"\n",clear_text);

    printf("EXAMPLE CBC checksum");
    printf("        key =  0123456789abcdef        iv =  1234567890abcdef\n");
    printf("        clear =                \"7654321 Now is the time for \"\n");
    printf("        checksum        58 d2 e7 7e 86 06 27 33, ");
    printf("or some part thereof\n");
    input = (sec_des_block *)clear_text2;
    sec_des_cbc_cksum(input,(sec_des_block *)cipher_text,(long) strlen(input),KS,ivec);
    printf("ACTUAL CBC checksum\n");
    printf("                encrypted cksum = (low to high bytes)\n                ");
    for (j = 0; j<=7; j++)
        printf("%02x ",cipher_text[j]);
    printf("\n\n");
}

flip(array)
    char *array;
{
    register old,new,i,j;
    /* flips the bit order within each byte from 0 lsb to 0 msb */
    for (i = 0; i<=7; i++) {
        old = *array;
        new = 0;
        for (j = 0; j<=7; j++) {
            if (old & 01)
                new = new | 01;
            if (j < 7) {
                old = old >> 1;
                new = new << 1;
            }
        }
        *array = new;
        array++;
    }
}

void do_encrypt(in,out)
    sec_des_block *in;
    sec_des_block *out;
{
    for (i =1; i<=nflag; i++) {
        sec_des_ecb_encrypt(in,out,KS,1);
        if (des_debug) {
            printf("\nclear %s\n",in);
            for (j = 0; j<=7; j++)
                printf("%02X ",in[0].bits[j] & 0xff);
            printf("        cipher ");
            for (j = 0; j<=7; j++)
                printf("%02X ",out[0].bits[j] & 0xff);
        }
    }
}

void do_decrypt(in,out)
     sec_des_block *in;
     sec_des_block *out;
    /* try to invert it */
{
    for (i =1; i<=nflag; i++) {
        sec_des_ecb_encrypt(out,in,KS,0);
        if (des_debug) {
            printf("clear %s\n",in);
            for (j = 0; j<=7; j++)
                printf("%02X ",in[0].bits[j] & 0xff);
            printf("        cipher ");
            for (j = 0; j<=7; j++)
                printf("%02X ",out[0].bits[j] & 0xff);
        }
    }
}
