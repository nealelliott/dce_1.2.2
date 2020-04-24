/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
extern void randomhook(int a, int b, int c);

typedef void ((*pfiii)(int, int, int));

extern void real0(int, int, int);
extern void real1(int, int, int);
extern void real2(int, int, int);
extern void real3(int, int, int);
extern void real4(int, int, int);
extern void real5(int, int, int);
extern void real6(int, int, int);
extern void real7(int, int, int);
extern void real8(int, int, int);
extern void real9(int, int, int);
extern void real10(int, int, int);
extern void real11(int, int, int);
extern void real12(int, int, int);
extern void real13(int, int, int);
extern void real14(int, int, int);
extern void real15(int, int, int);
extern void real16(int, int, int);
extern void real17(int, int, int);
extern void real18(int, int, int);
extern void real19(int, int, int);
extern void real20(int, int, int);
extern void real21(int, int, int);
extern void real22(int, int, int);
extern void real23(int, int, int);
extern void real24(int, int, int);
extern void real25(int, int, int);
extern void real26(int, int, int);
extern void real27(int, int, int);
extern void real28(int, int, int);
extern void real29(int, int, int);
extern void real30(int, int, int);
extern void real31(int, int, int);


extern pfiii realvec[32] = {
    real0,
    real1,
    real2,
    real3,
    real4,
    real5,
    real6,
    real7,
    real8,
    real9,
    real10,
    real11,
    real12,
    real13,
    real14,
    real15,
    real16,
    real17,
    real18,
    real19,
    real20,
    real21,
    real22,
    real23,
    real24,
    real25,
    real26,
    real27,
    real28,
    real29,
    real30,
    real31,
};

unsigned long buf[64];

struct
{
    pfiii *vectors;
    unsigned long *start;
    unsigned long *end;
} randomhookbase = {
    realvec,
    buf,
    &buf[64],
};

main()
{
    int i;
    unsigned long *p, np, op;
    
    for (i=0; i<32; i++) {
	randomhook(i, -i, i*100);
    }
    op=0;
    for (p = buf; p<randomhookbase.start; p++) {
	np = *p;
	printf("%08x\n", np-op);
	op=np;
    }
    exit(0);
}

