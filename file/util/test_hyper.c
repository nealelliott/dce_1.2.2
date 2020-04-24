/* Copyright (C) 1996 Transarc Corporation - All rights reserved. */

/* Validation test suite for hyper arithmetic macros. */

#include <sys/types.h>
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <stdio.h>
#include "hyper.h"

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/util/test_hyper.c,v 1.1.2.1 1996/10/02 21:13:23 damon Exp $")

struct {
    int a_hi;
    int a_lo;
    int i;
    int r_hi;
    int r_lo;
} tests[] = {
-1, -1, -1, -1, -2,
-1, -1, 0, -1, -1,
-1, -1, 1, 0, 0,
-1, 0, -1, -2,-1,
-1, 0, 0, -1,0,
-1, 0, 1, -1,1,
-1, 1, -1, -1,0,
-1, 1, 0, -1,1,
-1, 1, 1, -1,2,
0, -1, -1, 0,-2,
0, -1, 0, 0,-1,
0, -1, 1, 1,0,
0, 0, -1, -1,-1,
0, 0, 0, 0, 0,
0, 0, 1, 0, 1,
0, 1, -1, 0, 0,
0, 1, 0, 0, 1,
0, 1, 1, 0, 2,
1, -1, -1, 1,-2,
1, -1, 0, 1,-1,
1, -1, 1, 2,0,
1, 0, -1, 0,-1,
1, 0, 0, 1, 0,
1, 0, 1, 1,1,
1, 1, -1, 1, 0,
1, 1, 0, 1,1,
1, 1, 1, 1,2,
};
#define NTESTS (sizeof(tests)/sizeof(tests[0]))

struct {
    char *str;
    u_long hi, lo;
    int code;
} strs[] = {
    "09", 0,0, 1,
    "d", 0,0, 1,
    "0x123456789,,4", 0,0, 1,
    "4,,0x123456789", 0,0, 1,
    "1,, 4", 0,0, 1,
    ",,0", 0,0, 1,
    "1,,", 0,0, 1,
    ",,", 0,0, 1,
    "-", 0,0, 1,
    "-,,0", 0,0, 1,
    "0,,-", 0,0, 1,
    "1 ,,4", 0,1, 0,			/* parse terminates on space */
    "1", 0,1, 0,
    " 1", 0,1, 0,			/* skip leading whitespace */
    "1,", 0,1, 0,			/* parse terminates on comma */
    "1,,2,", 1,2, 0,
    "-1", (u_long)-1,(u_long)-1, 0,
    "0xa5a5a5a5,,0xfffffffe", 0xa5a5a5a5, (u_long)-2, 0,
    "53024283256946687", 12345677,(u_long)-1, 0,
    "53024287551913981", 12345678,0xfffffffd, 0,
    "-53024287551913981", 0xff439eb1,3, 0,
    "034,,0x555", 034,0x555, 0,
    "0x12345678901234", 0x123456,0x78901234, 0,
};
#define NSTRS (sizeof(strs)/sizeof(strs[0]))

int errors = 0;

void Fail(char *test,			/* test name */
	  afs_hyper_t b, afs_hyper_t i,	/* operands */
	  afs_hyper_t a, afs_hyper_t r)	/* answer, expected result */
{
    printf ("%s: unexpected result: %d,,%u +/- %d,,%u => %d,,%u (expected %d,,%u)\n",
	    test, AFS_hgethi(b), AFS_hgetlo(b), AFS_hgethi(i), AFS_hgetlo(i),
	    AFS_hgethi(a), AFS_hgetlo(a), AFS_hgethi(r), AFS_hgetlo(r));
    errors++;
}

Try(const char *s, afs_hyper_t r, int c)
{
    afs_hyper_t h;
    char *sp;
    int code = dfsh_StrToHyper(s, &h, &sp);
    if (c) {
	/* expect and error */
	if (!code) {
	    fprintf(stderr,
		    "Expected conversion error %s instead got %d,,%u\n",
		    s, AFS_HGETBOTH(h));
	    errors++;
	}
    } else {
	if (code) {
	    fprintf(stderr, "Failed to convert %s\n", s);
	    errors++;
	} else if (!AFS_hsame(h, r)) {
	    fprintf(stderr,
		    "Failed to convert %s to %d,,%u instead got %d,,%u\n",
		    s, AFS_HGETBOTH(r), AFS_HGETBOTH(h));
	    errors++;
	}
    }
}

main(argc, argv)
{
    int n;

    for (n=0; n<NTESTS; n++) {
	afs_hyper_t r, a, b;
	afs_hyper_t ii,c;
	int32 i;
	char str[32];

	AFS_hset64(r, tests[n].r_hi, tests[n].r_lo);
	AFS_hset64(a, tests[n].a_hi, tests[n].a_lo);
	i = tests[n].i;
	AFS_hset64(ii,((i<0) ? -1 : 0),i);
	b = a;
	AFS_hadd32(a,i);
#ifdef HAS_LONG_LONG_TYPE
	{   long long llb, lli, lla, llr;
#define ToLL(b) ((((long long)AFS_hgethi(b))<<32) | \
		 ((long long)AFS_hgetlo(b)&0xffffffff))
	    llb = ToLL(b);
	    lli = i;
	    lla = ToLL(a);
	    llr = ToLL(r);
	    if (llb+lli != llr) {
		printf("Misscomputed result: %lld + %lld = %lld (!%lld)\n",
		       llb, lli, llb+lli, llr);
	    }
	}
#endif
	if (!AFS_hsame(a,r))
	    Fail ("AFS_hadd32", b,ii,a,r);
	a = ii;
	AFS_hadd(a,b);
	if (!AFS_hsame(a,r))
	    Fail ("AFS_hadd", ii,b,a,r);

	a = ii;
	c = b;
	AFS_hnegate(c);
	AFS_hsub(a,c);
	if (!AFS_hsame(a,r))
	    Fail ("AFS_hsub", ii,b,a,r);

	if (i > 0) {
	    a = b;
	    AFS_hincr(a);
	    if (!AFS_hsame(a,r))
		Fail ("hincr", b,ii,a,r);
	} else if (i < 0) {
	    a = b;
	    AFS_hdecr(a);
	    if (!AFS_hsame(a,r))
		Fail ("hdecr", b,ii,a,r);
	}

	dfsh_HyperToStr(&b, str);
	Try(str, b, 0);
    }

    /* Try some special string conversions */
    for (n=0; n<NSTRS; n++) {
	afs_hyper_t i;
	AFS_hset64(i, strs[n].hi, strs[n].lo);
	Try (strs[n].str, i, strs[n].code);
    }


    {   /* Try some large shifts too. */
	afs_hyper_t h, h1;
	AFS_hset64(h, 1,1);
	h1 = h;
	AFS_hleftshift(h, 24);
	AFS_hleftshift(h, 24);
	AFS_hleftshift(h1, 48);
	if (!AFS_hsame(h,h1)) {
	    fprintf(stderr, "Large left shifts broken\n");
	    errors++;
	}
	AFS_hset64(h, 0x80000000, 0x80000000);
	h1 = h;
	AFS_hrightshift(h, 24);
	AFS_hrightshift(h, 24);
	AFS_hrightshift(h1, 48);
	if (!AFS_hsame(h,h1) || AFS_hcmp64(h,0,0x00008000) != 0) {
	    fprintf(stderr, "Large right shifts broken\n");
	    errors++;
	}
    }

    if (errors == 0) {
	printf ("All tests passed\n");
	exit (0);
    } else
	exit (1);
}

