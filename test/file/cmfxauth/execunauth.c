/* Copyright (C) 1996 Transarc Corporation - All rights reserved. */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include <dcedfs/syscall.h>
#include <dcedfs/px.h>

int main(int argc, char *argv[], char *envp[])
{
    int i;
    char *myexec;

    myexec = argv[0];

    /* Usage: execunauth command-to-execute */
    if (argc < 2) {
	printf("\nUsage: %s command-to-execute\n", myexec);
	exit(1);
    }

    /* call resetpag() function to ditch any DCE credentials */
    if (afs_syscall(AFSCALL_RESETPAG, 0, 0, 0, 0, 0)) {
	printf("\n%s - resetpag failed (errno = %d).\n", myexec, errno);
	exit(1);
    }

    /* argv not always null-terminated, so left-shift arg list and terminate */
    for (i = 1; i < argc; i++) {
	argv[i - 1] = argv[i];
    }

    argv[argc - 1] = NULL;

    /* attempt to exec specified command */
    execve(argv[0], argv, envp);

    /* only reached if failed to exec */
    printf("\n%s - exec failed (errno = %d).\n", myexec, errno);
    return 1;
}
