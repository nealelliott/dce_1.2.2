/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prog1.c,v $
 * Revision 1.1.133.1  1996/10/17  18:19:52  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:50:34  damon]
 *
 * Revision 1.1.128.1  1994/02/04  20:40:34  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:22:29  devsrc]
 * 
 * Revision 1.1.126.1  1993/12/07  17:41:51  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:54:26  jaffe]
 * 
 * Revision 1.1.4.5  1993/01/11  18:26:34  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:59:39  htf]
 * 
 * Revision 1.1.4.4  1992/11/24  21:44:52  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:44:30  bolinger]
 * 
 * Revision 1.1.4.3  1992/10/28  16:12:44  jaffe
 * 	Transarc delta: jess-add-dumpkernel-in-fs-tests 1.3
 * 	  Selected comments:
 * 	    The icldump command and debug option for the tests have been added in the
 * 	    fs test scripts. You can run fs/tests with -debug to dump kernel in case
 * 	    of error. The delta also adds the icl_DumpKernel function in the C programm.
 * 	    Add the icl_DumpKernel function call in each error case.
 * 	    Put the debug option in test1.
 * 	    When the debug is set the dumpfile is always created eventhough
 * 	    there is no error.
 * 	    Fix the problem.
 * 	[1992/10/27  18:11:11  jaffe]
 * 
 * Revision 1.1.4.2  1992/08/28  18:11:20  jaffe
 * 	Transarc delta: rajesh-clean-fs-tests 1.6
 * 	  Selected comments:
 * 	    To remove make, clean options from fs tests
 * 	    Really no change
 * 	    Make the scripts modular, structured.
 * 	    The variable names are changed to follow coding standards.
 * 	    Apply modification mentioned in revision 1.2 to 2 missed test file
 * 	    err1 and err2.
 * 	    Some bug fixes, and revision of tests err1 and err2. Introduction of
 * 	    a common functions ProcessResult to analyse the result.
 * 	    Further changes to some utility functions.
 * 	    Added error test err2 to the Makefile
 * 	[1992/08/28  12:12:01  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/21  12:39:39  jaffe
 * 	Transarc delta: fred-merge-tests 1.1
 * 	  Files modified:
 * 	    Makefile
 * 	    bos: Makefile, READ_ME, init_dce, kills, runc, runtests
 * 	    bos: setup, shows
 * 	    bos/tests: Makefile, test0, test1, test10, test11, test12
 * 	    bos/tests: test2, test3, test4, test5, test6, test7, test8
 * 	    bos/tests: test9; bos/tests/procs: Makefile
 * 	    cm: Makefile, runtests, test0, test1, test2, test3, test4
 * 	    cm: test5, test6; config: chk_errnr, echeck, environment
 * 	    fs: Makefile, READ_ME, err1, err2, longalpha, longfox
 * 	    fs: longmix, prog1.c, rtest1, shortalpha, shortfox, shortmix
 * 	    fs: test1, test10, test2, test2b, test2s, test3, test4
 * 	    fs: test5, test6, test7, test8, test9
 * 	    fts: Makefile, check, etest.sed, etest1, etest2, etest3
 * 	    fts: etest4, etest5, etest6, rtest1, rtest2, runrtests
 * 	    fts: runtests, test1, test10, test11, test12, test13, test2
 * 	    fts: test3, test4, test5, test6, test7, test8, test9
 * 	    low: Makefile, ctrl.asc, prog11.c, prog12.c, prog14.c
 * 	    low: prog3a.c, prog8.c, prog9.c, rtest1, rtest2, styl.asc
 * 	    low: test1, test10, test11, test12, test13, test14, test2
 * 	    low: test3, test4, test5, test6, test7, test8, test9
 * 	    naming: Makefile, bostest, fstest, ftstest
 * 	    scout: Makefile, test1, test2; setup: Makefile, environment
 * 	    ubik: Makefile, READ_ME, init, kills, runc, setup, shows
 * 	    ubik: udbg; util: Makefile, sys.c
 * 	  Selected comments:
 * 	    Created this delta to merge Phil Hirsch's final DFS test code changes with
 * 	    the test code that the OSF had.  Hopefully, this means we and the OSF
 * 	    should be looking at the same test code.
 * 	    Changes to facilitate merge of local DFS test code with the OSF's.
 * 	[1992/05/20  18:40:47  jaffe]
 * 
 * Revision 1.1  1992/01/19  03:17:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*****************************************************************************/
/* Copyright (C) 1990,1992 Transarc Corporation - All rights reserved */
/*****************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <string.h>
#include <dcedfs/icl.h>

#ifndef  TRUE
#define  TRUE 1
#endif

char message_buff[256];
int debug=TRUE;


void errex(s) char *s;
{
        FILE *dumpfilep;
        char dumpfile[256];

        printf("\n%s failed; errno = %d\n",s,errno);
        if (debug) {
            sprintf(dumpfile,"/tmp/low.prog1.kernel.dump.%d",getpid());
            dumpfilep = fopen(dumpfile, "w");
            icl_DumpKernel(dumpfilep, NULL, ICL_DUMP_FORMATTED);
            fclose(dumpfilep);
        }
        if (debug) abort();
        exit(1);
}


main(argc,argv) 
  int argc;
  char *argv[];
{
    char from, to, c, *fn="./tfile", *pgm, *p;
    int cnt, cnt1, fd, ln; 
    long i, nt;
    extern int atoi();
    extern long atol();

    pgm = (p=strchr(argv[0],'/')) ? p+1 : argv[0];
    setbuf(stdout, (char *) NULL);
    if (argc == 3) 
    {
	nt = atol(argv[1]);
	ln = atoi(argv[2]);
    }
    else 
    {
	printf("ERROR: usage: %s letter-number-to-change\ndone\n", pgm); 
        exit(2);
    }

    from = 'a'+(ln-1); 
    to = 'A'+(ln-1);

    printf("Changing %c and %c (letter %d)\n",from,to,ln);

    for (i=1, cnt1=(-1); !nt || i<=2*nt; ++i)
    {
	if ((fd = open(fn,O_RDWR)) == -1)
	{
	    sprintf(message_buff,"ERROR: %s: cannot open %s\ndone\n", pgm, fn); 
	    errex(message_buff);
	}

	cnt = 0;
	while (read(fd,&c,1) == 1)
	{
	    if (c == from) 
	    {
		(void)lseek(fd, -1L, 1); 
		(void)write(fd, &to, 1); 
		++cnt;
	    }
	}

	(void)close(fd);

	c = from; 
	from = to; 
	to = c;

	if (cnt1 == -1)
	    cnt1 = cnt;
	else 
	{
	    if (cnt != cnt1)
	    {
		sprintf(message_buff,"ERROR: %s iteration %d: wrong number of changes (%d vs %d)\n", pgm, i, cnt, cnt1);
	        errex(message_buff);
	    }
	}

	if (i%40 == 0)
	    printf("Changed %c and %c %ld times\n", from, to, i/2);
    }

    printf("Done changing %c and %c (%d times each pass)\n", from, to, cnt);
    exit(0);
}
