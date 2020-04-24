/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
 */
/*
 * HISTORY
 * $Log: prrb_nbi_001.c,v $
 * Revision 1.1.75.2  1996/02/17  23:31:33  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:02  marty]
 *
 * Revision 1.1.75.1  1995/12/11  23:02:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:18  root]
 * 
 * Revision 1.1.72.1  1993/10/12  19:37:17  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:52  rhw]
 * 
 * Revision 1.1.5.3  1992/12/30  16:53:15  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:11:35  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  20:39:56  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:34:34  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/15  16:55:52  bolinger
 * 	Fix OT defect 2921:  add testcase from defect 1678, which checks
 * 	operation of wrapped dup2().
 * 	[1992/05/15  13:54:22  bolinger]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1991 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 *    dup2_test.c -- To test CMA implementation of dup2.
 */

#include <pthread.h>
#include <dce/cma_stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <sys/file.h>
#include <dce/cts_test.h>
#if defined(SNI_SVR4_POSIX)
# include <fcntl.h>
#endif

main()
{
    int               rc;
    struct stat        fileStat;
    int                fd[2];

    cts_test("PRRB_NBI_001", "Test CMA implementation of dup2");

    fd[0] = open("./dummy1", O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd[0] == -1) {
        cts_failed("dup2_test: fd[0] = open failed, errno = %d", errno);
        exit(1);
    }

    fd[1] = open("./dummy2", O_CREAT | O_TRUNC, 0600);
    if (fd[1] == -1) {
        cts_failed("dup2_test: fd[1] = open failed, errno = %d", errno);
        exit(1);
    }

    rc = dup2(fd[0], fd[1]);
    if (rc == -1 || rc != fd[1]) {
        cts_failed("dup2_test: dup2 failed, errno = %d", errno);
        exit(1);
    }

    if (fstat(fd[1],&fileStat) == -1){
        cts_failed("dup2_test: fstat(fd[1]) failed, errno = %d", errno);
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);
    unlink("./dummy1");
    unlink("./dummy2");
	cts_result();
	exit(0);
}
