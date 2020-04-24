/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bomb_test.c,v $
 * Revision 1.1.45.1  1996/10/02  17:03:41  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:44  damon]
 *
 * Revision 1.1.39.3  1994/07/13  22:19:14  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  17:55:06  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:53:50  mbs]
 * 
 * Revision 1.1.39.2  1994/06/09  13:51:51  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:27  annie]
 * 
 * Revision 1.1.39.1  1994/02/04  20:06:07  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:34  devsrc]
 * 
 * Revision 1.1.37.1  1993/12/07  17:12:51  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/06  13:33:46  jaffe]
 * 
 * $EndLog$
 */

/*
 * Copyright (C) 1993 Transarc Corporation - All rights reserved
 */

#include <stdio.h>
#include <pthread.h>

#include <dcedfs/stds.h>

#include <bomb.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bomb/bomb_test.c,v 1.1.45.1 1996/10/02 17:03:41 damon Exp $")


int
main(int argc, char* argv[])
{
    long	code;

    if (argc > 1) {			/* Do kernel test */
	bombPoint_t	bp;
	
	BOMB_KERNEL_UNSET("afscall_bomb#4");

	bp.bp_explosionType = BOMB_EXP_TYPE_ERROR;
	bp.bp_errorCode = 42;
	bp.bp_triggerType = BOMB_TRIG_TYPE_COUNTDOWN;
	bp.bp_count = 1;
	bp.bp_refreshCount = 1;
	code = BOMB_KERNEL_SET("afscall_bomb#1", &bp);
	if (code) {
	    fprintf(stderr, "Failed to set afscall_bomb#1\n");
	    exit(2);
	}

	code = BOMB_KERNEL_TEST();
	if (code != 42) {
	    fprintf(stderr, "Expected a 42\n");
	    exit(2);
	}

	code = BOMB_KERNEL_TEST();
	if (code != 42) {
	    fprintf(stderr, "Expected another 42\n");
	    exit(2);
	}

	code = BOMB_KERNEL_UNSET("afscall_bomb#1");
	if (code) {
	    fprintf(stderr, "Failed to unset afscall_bomb#1: %s\n",
		    dfs_dceErrTxt(code));
	    exit(2);
	}

	code = BOMB_KERNEL_TEST();
	if (code != 0) {
	    fprintf(stderr, "Expected a 0\n");
	    exit(2);
	}

	bp.bp_explosionType = BOMB_EXP_TYPE_IF;
	code = BOMB_KERNEL_SET("afscall_bomb#2", &bp);
	if (code) {
	    fprintf(stderr, "Failed to set afscall_bomb#2: %s\n",
		    dfs_dceErrTxt(code));
	    exit(2);
	}

	code = BOMB_KERNEL_TEST();
	if (code != 0) {
	    fprintf(stderr, "Expected a 0\n");
	    exit(2);
	}

	code = BOMB_KERNEL_UNSET("afscall_bomb#2");
	if (code) {
	    fprintf(stderr, "Failed to unset afscall_bomb#2: %s\n",
		    dfs_dceErrTxt(code));
	    exit(2);
	}

	bp.bp_explosionType = BOMB_EXP_TYPE_DELAY;
	bp.bp_delay = 5;
	code = BOMB_KERNEL_SET("afscall_bomb#3", &bp);
	if (code) {
	    fprintf(stderr, "Failed to set afscall_bomb#3: %s\n",
		    dfs_dceErrTxt(code));
	    exit(2);
	}

	code = BOMB_KERNEL_TEST();
	if (code != 0) {
	    fprintf(stderr, "Expected a 0\n");
	    exit(2);
	}

	code = BOMB_KERNEL_UNSET("afscall_bomb#3");
	if (code) {
	    fprintf(stderr, "Failed to unset afscall_bomb#3: %s\n",
		    dfs_dceErrTxt(code));
	    exit(2);
	}

	bp.bp_explosionType = BOMB_EXP_TYPE_RETURN;
	bp.bp_returnVal = 43;
	code = BOMB_KERNEL_SET("afscall_bomb#4", &bp);
	if (code) {
	    fprintf(stderr, "Failed to set afscall_bomb#4: %s\n",
		    dfs_dceErrTxt(code));
	    exit(2);
	}

	code = BOMB_KERNEL_TEST();
	if (code != 43) {
	    fprintf(stderr, "Expected a 43\n");
	    exit(2);
	}

	code = BOMB_KERNEL_TEST();
	if (code != 43) {
	    fprintf(stderr, "Expected another 43\n");
	    exit(2);
	}

	code = BOMB_KERNEL_UNSET("afscall_bomb#4");
	if (code) {
	    fprintf(stderr, "Failed to unset afscall_bomb#4: %s\n",
		    dfs_dceErrTxt(code));
	    exit(2);
	}

	code = BOMB_KERNEL_TEST();
	if (code != 0) {
	    fprintf(stderr, "Expected a 0\n");
	    exit(2);
	}

    } else {				/* Allow user space testing */
	code = BOMB_EXEC("test#1", 0);
	printf("code = %ld\n", code);
	
	BOMB_IF("test#2") {
	    printf("Taking alternate path\n");
	} BOMB_ELSE {
	    printf("Taking alternate else path\n");
	}
	
	printf("Before test#3\n");
	BOMB_POINT("test#3");
	printf("After test#3\n");
	
	BOMB_RETURN("test#4");
    }
}
