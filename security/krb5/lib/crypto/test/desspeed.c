/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: desspeed.c,v $
 * Revision 1.1.2.2  1996/03/09  20:45:38  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:25:04  marty]
 *
 * Revision 1.1.2.1  1995/12/08  17:50:34  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/08/21  17:36 UTC  sommerfeld
 * 	DES/MD5 performance test.
 * 	[1995/08/17  21:50 UTC  sommerfeld  /main/sommerfeld_mothpatch/1]
 * 
 * $EndLog$
 */

/*
 * desspeed - timing test of varous DES & MD5 modes of operation.
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/syscall.h>

#include <krb5/base-defs.h>
#include <krb5/encryption.h>
#include <krb5/des_int.h>
#include <krb5/rsa-md5.h>

unsigned char bigblock1[1024*64];
mit_des_cblock key, block1, block2;
mit_des_key_schedule keysched;
unsigned char bigblock2[1024*64];

/* subtime - subtract two struct timevals, return double result */

void subtime(struct timeval *tvs, struct timeval *tve, double *res)
{
	long sec;
	long usec;

	sec = tve->tv_sec - tvs->tv_sec;
	usec = tve->tv_usec - tvs->tv_usec;

	if (usec < 0) {
		usec += 1000000;
		sec--;
	}

	*res = (double)sec + (double)usec/1000000.;
}

typedef void ((*testfn_t)(int nloops, int bytecount, int direction));

void test_key_schedule(int nloops, int bytecount, int direction)
{
    int i;
    for (i=0; i<nloops; i++) 
    {
	make_key_sched(key, keysched);
    }
}

void test_ecb_encrypt(int nloops, int bytecount, int direction)
{
    int i;
    nloops /= 2;
    for (i=0; i<nloops; i++) 
    {
	mit_des_ecb_encrypt(block1, block2, keysched, direction);
	mit_des_ecb_encrypt(block2, block1, keysched, direction);	
    }
}

void test_cbc_encrypt(int nloops, int bytecount, int direction)
{
    int i;
    for (i=0; i<nloops; i++) 
    {
	mit_des_cbc_encrypt(
	    bigblock1,
	    bigblock2,
	    bytecount, keysched, block1, direction);
    }
}

void test_md5(int nloops, int bytecount, int direction)
{
    int i;
    for (i=0; i<nloops; i++) 
    {
	MD5_CTX ctx;
	MD5Init(&ctx);
	MD5Update(&ctx, bigblock1, bytecount);
	MD5Final(&ctx);
    }
}

void test_md5_des(int nloops, int bytecount, int direction)
{
    int i;
    for (i=0; i<nloops; i++) 
    {
	MD5_CTX ctx;
	MD5Init(&ctx);
	MD5Update(&ctx, bigblock1, bytecount);
	MD5Final(&ctx);
	mit_des_cbc_encrypt(ctx.digest, bigblock2, 16, keysched, block1, direction);
    }
}

int nloops = 0;

void dotest (testfn_t fn, char *tag, int bytecount, char *units, int direction)
{
    struct timeval start, end;
    double wall;
    double speed;
    
    printf("testing %s: ");
    fflush(stdout);
    (*fn) (10, bytecount, direction);
    gettimeofday(&start, 0);
    (*fn) (nloops, bytecount, direction);
    gettimeofday(&end, 0);
    subtime(&start, &end, &wall);

    speed = (nloops * bytecount) / wall;
    printf("%.6f s; %.6f %ss/s, %.6f us/%s\n", wall, speed, units, 1000000.0/speed, units);
}
	
main(int argc, char **argv)
{
    int i, npasses;
    int fastnloops, slownloops;

    switch (argc) {
    case 1:
	nloops = 100000;
	npasses = 1;
	break;
    case 2:
	nloops = atoi(argv[1]);
	npasses = 1;
	break;
    case 3:
	nloops = atoi(argv[1]);	    
	npasses = atoi(argv[2]);
	break;
    default:
	fprintf(stderr, "usage: desspeed [niter] [npasses]\n");
	exit(1);
    }
    if (nloops < 100) nloops = 100;
    
    fastnloops = nloops;
    slownloops = fastnloops/10;
    if (slownloops < 10) slownloops=10;
    if (npasses < 1) npasses = 1;
    
    printf("%d pass%s of %d or %d iterations:\n", npasses,
	   (npasses == 1)?"":"es", fastnloops, slownloops);
    for (i=0; i<npasses; i++) 
    {
	nloops = fastnloops;	
	dotest(test_key_schedule, "key schedule", 1, "schedule", 0);
	dotest(test_ecb_encrypt, "ECB encrypt", 1, "encrypt", MIT_DES_ENCRYPT);
	dotest(test_ecb_encrypt, "ECB encrypt", 8, "byte", MIT_DES_ENCRYPT);

	dotest(test_ecb_encrypt, "ECB decrypt", 1, "decrypt", MIT_DES_DECRYPT);
	dotest(test_ecb_encrypt, "ECB decrypt", 8, "byte", MIT_DES_DECRYPT);

	dotest(test_cbc_encrypt, "CBC encrypt-16", 16, "byte", MIT_DES_ENCRYPT);
	
	dotest(test_cbc_encrypt, "CBC decrypt-16", 16, "byte", MIT_DES_DECRYPT);
	dotest(test_md5, "MD5-64", 64, "byte", -1);
	dotest(test_md5_des, "MD5-DES-64", 64, "byte", MIT_DES_ENCRYPT);

	nloops = slownloops;
	dotest(test_cbc_encrypt, "CBC encrypt-1024", 1024, "byte", MIT_DES_ENCRYPT);
	dotest(test_cbc_encrypt, "CBC decrypt-1024", 1024, "byte", MIT_DES_DECRYPT);

	dotest(test_md5, "MD5-1024", 1024, "byte", -1);
	dotest(test_md5_des, "MD5-DES-1024", 1024, "byte", MIT_DES_ENCRYPT);	
    }
    printf("PASSED (this test can't fail...)\n");
    return 0;
}

    
