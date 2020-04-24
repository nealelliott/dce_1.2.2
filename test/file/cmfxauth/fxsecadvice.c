/* Copyright (C) 1996 Transarc Corporation - All rights reserved. */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <dcedfs/syscall.h>
#include <dcedfs/fshs.h>
#include <dcedfs/px.h>

int main(int argc, char *argv[])
{
    char *pos;
    struct fshs_security_advice secAdvice;

    /* Usage: fxsecadvice minlocal maxlocal minremote maxremote */

    if (argc != 5) {
	printf("\nUsage: %s minlocal maxlocal minremote maxremote\n", argv[0]);
	exit(1);
    }

    /* fill in secAdvice as specified */

    errno = 0;

    secAdvice.formatTag = FSHS_SEC_ADVICE_FORMAT_1;

    secAdvice.local.minProtectLevel = strtol(argv[1], &pos, 10);

    if (errno || pos == argv[1]) {
	printf("\n%s - %s is an invalid argument.\n", argv[0], argv[1]);
	exit(1);
    }

    secAdvice.local.maxProtectLevel = strtol(argv[2], &pos, 10);

    if (errno || pos == argv[2]) {
	printf("\n%s - %s is an invalid argument.\n", argv[0], argv[2]);
	exit(1);
    }

    secAdvice.nonLocal.minProtectLevel = strtol(argv[3], &pos, 10);

    if (errno || pos == argv[3]) {
	printf("\n%s - %s is an invalid argument.\n", argv[0], argv[3]);
	exit(1);
    }

    secAdvice.nonLocal.maxProtectLevel = strtol(argv[4], &pos, 10);

    if (errno || pos == argv[4]) {
	printf("\n%s - %s is an invalid argument.\n", argv[0], argv[4]);
	exit(1);
    }

    /* make system call to set values in kernel */

    if (afs_syscall(AFSCALL_PX, PXOP_PUTSECADVICE, (int)&secAdvice, 0, 0, 0)) {
	printf("\n%s - failed (errno = %d).\n", argv[0], errno);
	exit(1);
    } else {
	exit(0);
    }
}
