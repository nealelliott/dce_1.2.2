/*
 * Copyright (c) 1993 Transarc Corp.  All rights reserved.
 */

#include <sys/types.h>
#include <sys/param.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

#ifdef _AIX
#define MMT_SYNC_INVALIDATE MS_INVALIDATE
#else
#define MMT_SYNC_INVALIDATE (MS_SYNC | MS_INVALIDATE)
#endif /* _AIX */

/*
 * Error printing
 *
 * We flush stdout before printing an error message to stderr, so that if
 * they are both directed to the same file, they stay in step with each other.
 */

static char pnm[] = "mmaptest";		/* program name for error msgs */

void mmt_perror (s)
    char *s;
{
    fflush (stdout);
    perror (s);
}

int mmt_fprintf (FILE *stream, char *fmt, ...)
{
    va_list ap;
    int result;

    va_start(ap, fmt);
    fflush (stdout);
    result = vfprintf (stream, fmt, ap);
    va_end(ap);
    return result;
}

/* File status printing */

#ifdef _AIX
#define MMT_STAT_BSIZE UBSIZE
#else
#define MMT_STAT_BSIZE DEV_BSIZE
#endif /* _AIX */

int fstat_and_print (
    int f,
    struct stat *stbufP)
{
    int code;

    code = fstat (f, stbufP);
    if (code == -1) return code;
    printf ("Size %d bytes, Allocation %d %d-byte blocks.\n",
	stbufP->st_size, stbufP->st_blocks, MMT_STAT_BSIZE);
    return code;
}

sigjmp_buf env;

void signal_catcher (int sigval)
{
    signal (sigval, signal_catcher);
    siglongjmp (env, sigval);
}

void mmt_print_signal (int sigval)
{
    char *signame = "";
    switch (sigval) {
	case SIGBUS:
	    signame = "SIGBUS"; break;
	case SIGSEGV:
	    signame = "SIGSEGV"; break;
    }
    mmt_fprintf (stderr, "%s: %s error caught\n", pnm, signame);
}

main()
{
    printf ("mmap tests\n");

    signal (SIGBUS, signal_catcher);
    signal (SIGSEGV, signal_catcher);

    printf ("basic tests\n");

    {
	int bufsize = 8192*3;
	int f;
	caddr_t addr;
	int code;

	printf ("Open, mmap, munmap, close.\n");

	f = open ("test_file_basic_1", O_RDWR|O_CREAT|O_TRUNC, 0777);
	if (f == -1) {mmt_perror (pnm); goto done_basic_b;}
	addr = mmap (0, bufsize, PROT_WRITE|PROT_READ, MAP_SHARED, f, 0);
	if (addr == (caddr_t) -1) {mmt_perror (pnm); goto done_basic_a;}
	code = munmap (addr, bufsize);
	if (code == -1) mmt_perror (pnm);
    done_basic_a:
	code = close (f);
	if (code == -1) mmt_perror (pnm);
    done_basic_b:

	printf ("Open, mmap, close, munmap.\n");

	f = open ("test_file_basic_1", O_RDWR|O_CREAT|O_TRUNC, 0777);
	if (f == -1) {mmt_perror (pnm); goto done_basic_d;}
	addr = mmap (0, bufsize, PROT_WRITE|PROT_READ, MAP_SHARED, f, 0);
	if (addr == (caddr_t) -1) {mmt_perror (pnm); goto done_basic_c;}
	code = close (f);
	if (code == -1) mmt_perror (pnm);
	code = munmap (addr, bufsize);
	if (code == -1) mmt_perror (pnm);
	goto done_basic_d;

    done_basic_c:
	code = close (f);
	if (code == -1) mmt_perror (pnm);
    done_basic_d:
	;
    }

    printf ("msync tests\n");

    {
	char buf[8192*3];
	int bufsize = sizeof(buf);
	int f = open ("test_file_ms", O_RDWR|O_CREAT|O_TRUNC, 0777);
	caddr_t addr = mmap (0, bufsize, PROT_WRITE|PROT_READ,
			     MAP_SHARED, f, 0);
	long *laddr = (long *) addr;
	long *lbuf = (long *) buf;
	int code;
	int i;

	int modword = (8192/sizeof(long)) + (4096/sizeof(long));
	int modbyte = modword*sizeof(long);
	int modpage = (modbyte / 4096) * 4096;
	int notmodpage = modpage - 4096;
	int notmodblock = 0;

	/* bleah */
	if (addr == (caddr_t) -1) {mmt_perror (pnm); goto done_ms_a;}

	/* Extend via syscall */
	for (i=0; i<(bufsize/sizeof(long)); i++)
	    lbuf[i] = i*i;
	code = write (f, buf, bufsize);
	if (code == -1) {mmt_perror (pnm); goto done_ms;}
	if (code < bufsize) {
	    mmt_fprintf (stderr, "%s: wrote less data than expected\n", pnm);
	    goto done1;
	}

	printf ("modify every page, msync whole file, unmap\n");

	/* Modify every page */
	for (i=0; i<(bufsize/sizeof(long)); i++)
	    laddr[i] = i*i;

	/* msync whole file */
	code = msync (addr, bufsize, MS_SYNC);
	if (code == -1) {mmt_perror (pnm); goto done_ms;}

	/* unmap */
	munmap (addr, bufsize);

	/* Survived that test; now remap to continue msync tests */
	addr = mmap (0, bufsize, PROT_WRITE|PROT_READ, MAP_SHARED, f, 0);
	if (addr == (caddr_t) -1) {mmt_perror (pnm); goto done_ms;}

	printf ("msync whole file\n");

	/* Modify 2nd page in 2nd block */
	laddr [modword] = 37;

	/* msync whole file */
	code = msync (addr, bufsize, MS_SYNC);
	if (code == -1) {mmt_perror (pnm); goto done_ms;}

	printf ("msync modified page\n");

	/* Modify 2nd page in 2nd block */
	laddr [modword] = 37;

	/* msync modified page */
	code = msync (addr + modpage, 4096, MS_SYNC);
	if (code == -1) {mmt_perror (pnm); goto done_ms;}

	printf ("msync unmodified page in modified block\n");

	/* Modify 2nd page in 2nd block */
	laddr [modword] = 37;

	/* msync unmodified page */
	code = msync (addr + notmodpage, 4096, MS_SYNC);
	if (code == -1) {mmt_perror (pnm); goto done_ms;}

	printf ("msync unmodified block\n");

	/* Modify 2nd page in 2nd block */
	laddr [modword] = 37;

	/* msync unmodified block */
	code = msync (addr + notmodblock, 8192, MS_SYNC);
	if (code == -1) {mmt_perror (pnm); goto done_ms;}

	printf ("msync (MS_ASYNC)\n");

	/* Modify 2nd page in 2nd block */
	laddr [modword] = 37;

	/* msync (MS_ASYNC) */
	code = msync (addr, bufsize, MS_ASYNC);
	if (code == -1) {mmt_perror (pnm); goto done_ms;}

	printf ("msync (MS_INVALIDATE)\n");

	/* Modify 2nd page in 2nd block */
	laddr [modword] = 37;

	/* msync (MS_INVALIDATE) */
	code = msync (addr, bufsize, MS_INVALIDATE);
	if (code == -1) {mmt_perror (pnm); goto done_ms;}

#ifdef _AIX

        printf ("msync (MS_ASYNC | MS_INVALIDATE) -- not valid in AIX\n");

#else

	printf ("msync (MS_ASYNC | MS_INVALIDATE)\n");

	/* Modify 2nd page in 2nd block */
	laddr [modword] = 37;

	/* msync (MS_ASYNC | MS_INVALIDATE) */
	code = msync (addr, bufsize, MS_ASYNC | MS_INVALIDATE);
	if (code == -1) {mmt_perror (pnm); goto done_ms;}

#endif /* _AIX */

	/* Clean up */
done_ms:
	munmap (addr, bufsize);
done_ms_a:
	close (f);
    }

    printf ("consistency tests\n");

    {
	char buf[8192*3];
	int bufsize = sizeof(buf);
	int f = open ("test_file_1", O_RDWR|O_CREAT|O_TRUNC, 0777);
	caddr_t addr = mmap (0, bufsize, PROT_WRITE|PROT_READ,
			     MAP_SHARED, f, 0);
	long *laddr = (long *) addr;
	long *lbuf = (long *) buf;
	int sigval;
	int code;
	int i;

	/* bleah */
	if (addr == (caddr_t) -1) {mmt_perror (pnm); goto done1a;}

	/* catch SIGBUS */
	if ((sigval = sigsetjmp (env, 1)) != 0) {
	    mmt_print_signal (sigval);
	    goto done1;
	}

	printf ("Extend via syscall, read via map, and compare.\n");

	/* Extend via syscall */
	for (i=0; i<(bufsize/sizeof(long)); i++)
	    lbuf[i] = i*i;
	code = write (f, buf, bufsize);
	if (code == -1) {mmt_perror (pnm); goto done1;}
	if (code < bufsize) {
	    mmt_fprintf (stderr, "%s: wrote less data than expected\n", pnm);
	    goto done1;
	}

	/* Read via map and compare */
	for (i=0; i<(bufsize/sizeof(long)); i++)
	    if (laddr[i] != i*i) {
		mmt_fprintf (stderr, "%s: inconsistent data\n", pnm);
		goto done1;
	    }

	printf ("Write via syscall, read via map, and compare.\n");

	/* Write via syscall */
	for (i=0; i<(bufsize/sizeof(long)); i++)
	    lbuf[i] = i*i + 37;
	lseek (f, 0, 0);
	code = write (f, buf, bufsize);
	if (code == -1) {mmt_perror (pnm); goto done1;}
	if (code < bufsize) {
	    mmt_fprintf (stderr, "%s: wrote less data than expected\n", pnm);
	    goto done1;
	}

	/* Read via map and compare */
	for (i=0; i<(bufsize/sizeof(long)); i++)
	    if (laddr[i] != i*i + 37) {
		mmt_fprintf (stderr, "%s: inconsistent data\n", pnm);
		goto done1;
	    }

	printf ("Write via map, read via syscall, and compare.\n");

	/* Write via map */
	for (i=0; i<(bufsize/sizeof(long)); i++)
	    laddr[i] = i*i*i;

	/* Read via syscall */
	lseek (f, 0, 0);
	code = read (f, buf, bufsize);
	if (code == -1) {mmt_perror (pnm); goto done1;}
	if (code < bufsize) {
	    mmt_fprintf (stderr, "%s: read less data than expected: %d\n", pnm, code);
	    goto done1;
	}

	/* Compare */
	for (i=0; i<(bufsize/sizeof(long)); i++)
	    if (lbuf[i] != i*i*i) {
		mmt_fprintf (stderr, "%s: inconsistent data\n", pnm);
		goto done1;
	    }

	/* Clean up */
    done1:
	munmap (addr, bufsize);
    done1a:
	close (f);
    }

    printf ("EOF handling tests\n");

    printf ("File format: blocked\n");

    {
	int blocked_file_size = 8192 * 1 + 2048;
	int blocked_past_last_page = 8192 * 1 + 4096 + 2048;
	int blocked_on_last_page = 8192 * 1 + 2048 + 1024;
	char buf[8192*2];
	int blocked_bufsize = sizeof(buf);
	struct stat stbuf;
	int f = open ("test_file_2", O_RDWR|O_CREAT|O_TRUNC, 0777);
	caddr_t addr = mmap (0, blocked_bufsize, PROT_WRITE|PROT_READ,
			     MAP_SHARED, f, 0);
	long *laddr = (long *) addr;
	long *lbuf = (long *) buf;
	int sigval;
	int code;
	int i;
	char c;

	/* bleah */
	if (addr == (caddr_t) -1) {mmt_perror (pnm); goto done2a;}

	/* Extend via syscall */
	for (i=0; i<(blocked_file_size/sizeof(long)); i++)
	    lbuf[i] = i*i;
	code = write (f, buf, blocked_file_size);
	if (code == -1) {mmt_perror (pnm); goto done2;}
	if (code < blocked_file_size) {
	    mmt_fprintf (stderr, "%s: wrote less data than expected\n", pnm);
	    goto done2;
	}

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done2;}

	printf ("Read past last page.\n");

	/* catch SIGBUS */
	if (sigsetjmp (env, 1) != 0) {
	    printf ("Catching SIGBUS as expected.\n");
	    goto caught_blocked_1;
	}

	/* read past last page */
	c = addr [blocked_past_last_page];

	mmt_fprintf (stderr, "%s: Failed to generate SIGBUS\n", pnm);
	goto done2;

caught_blocked_1:

	printf ("Write past last page.\n");

	/* catch SIGBUS */
	if (sigsetjmp (env, 1) != 0) {
	    printf ("Catching SIGBUS as expected.\n");
	    goto caught_blocked_2;
	}

	/* write past last page */
	addr [blocked_past_last_page] = '7';

	mmt_fprintf (stderr, "%s: Failed to generate SIGBUS\n", pnm);
	goto done2;

caught_blocked_2:

	printf ("Read on last page beyond EOF.\n");

	/* catch SIGBUS */
	if ((sigval = sigsetjmp (env, 1)) != 0) {
	    mmt_print_signal (sigval);
	    goto done2;
	}

	/* read on last page beyond EOF. */
	c = addr [blocked_on_last_page];
	if (c != 0) {
	    mmt_fprintf (stderr, "%s: non-zero beyond EOF\n", pnm);
	    goto done2;
	}

	printf ("Write on last page beyond EOF.\n");

	/* write on last page beyond EOF. */
	addr [blocked_on_last_page] = '7';

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done2;}
	if (stbuf.st_size != blocked_file_size) {
	    mmt_fprintf (stderr, "%s: file has wrong size\n", pnm);
	    goto done2;
	}
	printf ("File size is OK.\n");

	/* Clean up */
    done2:
	munmap (addr, blocked_bufsize);
    done2a:
	close (f);
    }

    printf ("File format: fragmented\n");

    {
	int fragmented_file_size = 2048;
	int fragmented_past_last_page = 4196 * 1 + 2048;
	int fragmented_on_last_page = 2048 + 1024;
	char buf[4196*2];
	int fragmented_bufsize = sizeof(buf);
	struct stat stbuf;
	int f = open ("test_file_3", O_RDWR|O_CREAT|O_TRUNC, 0777);
	caddr_t addr = mmap (0, fragmented_bufsize, PROT_WRITE|PROT_READ,
			     MAP_SHARED, f, 0);
	long *laddr = (long *) addr;
	long *lbuf = (long *) buf;
	int sigval;
	int code;
	int i;
	char c;

	/* bleah */
	if (addr == (caddr_t) -1) {mmt_perror (pnm); goto done3a;}

	/* Extend via syscall */
	for (i=0; i<(fragmented_file_size/sizeof(long)); i++)
	    lbuf[i] = i*i;
	code = write (f, buf, fragmented_file_size);
	if (code == -1) {mmt_perror (pnm); goto done3;}
	if (code < fragmented_file_size) {
	    mmt_fprintf (stderr, "%s: wrote less data than expected\n", pnm);
	    goto done3;
	}

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done3;}

	printf ("Read past last page.\n");

	/* catch SIGBUS */
	if (sigsetjmp (env, 1) != 0) {
	    printf ("Catching SIGBUS as expected.\n");
	    goto caught_fragmented_1;
	}

	/* read past last page */
	c = addr [fragmented_past_last_page];

	mmt_fprintf (stderr, "%s: Failed to generate SIGBUS\n", pnm);
	goto done3;

caught_fragmented_1:

	printf ("Write past last page.\n");

	/* catch SIGBUS */
	if (sigsetjmp (env, 1) != 0) {
	    printf ("Catching SIGBUS as expected.\n");
	    goto caught_fragmented_2;
	}

	/* write past last page */
	addr [fragmented_past_last_page] = '7';

	mmt_fprintf (stderr, "%s: Failed to generate SIGBUS\n", pnm);
	goto done3;

caught_fragmented_2:

	printf ("Read on last page beyond EOF.\n");

	/* catch SIGBUS */
	if ((sigval = sigsetjmp (env, 1)) != 0) {
	    mmt_print_signal (sigval);
	    goto done3;
	}

	/* read on last page beyond EOF. */
	c = addr [fragmented_on_last_page];
	if (c != 0) {
	    mmt_fprintf (stderr, "%s: non-zero beyond EOF\n", pnm);
	    goto done3;
	}

	printf ("Write on last page beyond EOF.\n");

	/* write on last page beyond EOF. */
	addr [fragmented_on_last_page] = '7';

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done3;}
	if (stbuf.st_size != fragmented_file_size) {
	    mmt_fprintf (stderr, "%s: file has wrong size\n", pnm);
	    goto done3;
	}
	printf ("File size is OK.\n");

	/* Clean up */
    done3:
	munmap (addr, fragmented_bufsize);
    done3a:
	close (f);
    }

    printf ("File format: inline\n");

    {
	int inline_file_size = 60;
	int inline_past_last_page = 4196 * 1 + 2048;
	int inline_on_last_page = 80;
	char buf[4196*2];
	int inline_bufsize = sizeof(buf);
	struct stat stbuf;
	int f = open ("test_file_4", O_RDWR|O_CREAT|O_TRUNC, 0777);
	caddr_t addr = mmap (0, inline_bufsize, PROT_WRITE|PROT_READ,
			     MAP_SHARED, f, 0);
	long *laddr = (long *) addr;
	long *lbuf = (long *) buf;
	int sigval;
	int code;
	int i;
	char c;

	/* bleah */
	if (addr == (caddr_t) -1) {mmt_perror (pnm); goto done4a;}

	/* Extend via syscall */
	for (i=0; i<(inline_file_size/sizeof(long)); i++)
	    lbuf[i] = i*i;
	code = write (f, buf, inline_file_size);
	if (code == -1) {mmt_perror (pnm); goto done4;}
	if (code < inline_file_size) {
	    mmt_fprintf (stderr, "%s: wrote less data than expected\n", pnm);
	    goto done4;
	}

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done4;}

	printf ("Read past last page.\n");

	/* catch SIGBUS */
	if (sigsetjmp (env, 1) != 0) {
	    printf ("Catching SIGBUS as expected.\n");
	    goto caught_inline_1;
	}

	/* read past last page */
	c = addr [inline_past_last_page];

	mmt_fprintf (stderr, "%s: Failed to generate SIGBUS\n", pnm);
	goto done4;

caught_inline_1:

	printf ("Write past last page.\n");

	/* catch SIGBUS */
	if (sigsetjmp (env, 1) != 0) {
	    printf ("Catching SIGBUS as expected.\n");
	    goto caught_inline_2;
	}

	/* write past last page */
	addr [inline_past_last_page] = '7';

	mmt_fprintf (stderr, "%s: Failed to generate SIGBUS\n", pnm);
	goto done4;

caught_inline_2:

	printf ("Read on last page beyond EOF.\n");

	/* catch SIGBUS */
	if ((sigval = sigsetjmp (env, 1)) != 0) {
	    mmt_print_signal (sigval);
	    goto done4;
	}

	/* read on last page beyond EOF. */
	c = addr [inline_on_last_page];
	if (c != 0) {
	    mmt_fprintf (stderr, "%s: non-zero beyond EOF\n", pnm);
	    goto done4;
	}

	printf ("Write on last page beyond EOF.\n");

	/* write on last page beyond EOF. */
	addr [inline_on_last_page] = '7';

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done4;}
	if (stbuf.st_size != inline_file_size) {
	    mmt_fprintf (stderr, "%s: file has wrong size\n", pnm);
	    goto done4;
	}
	printf ("File size is OK.\n");

	/* Clean up */
    done4:
	munmap (addr, inline_bufsize);
    done4a:
	close (f);
    }

    printf ("File format: blocked, but recently truncated to very small\n");

    {
	int truncated_file_size = 60;
	int truncated_past_last_page = 4196 * 1 + 2048;
	int truncated_on_last_page = 80;
	char buf[8192*2];
	int truncated_bufsize = sizeof(buf);
	struct stat stbuf;
	int f = open ("test_file_5", O_RDWR|O_CREAT|O_TRUNC, 0777);
	caddr_t addr = mmap (0, truncated_bufsize, PROT_WRITE|PROT_READ,
			     MAP_SHARED, f, 0);
	long *laddr = (long *) addr;
	long *lbuf = (long *) buf;
	int sigval;
	int code;
	int i;
	char c;

	/* bleah */
	if (addr == (caddr_t) -1) {mmt_perror (pnm); goto done5a;}

	/* Extend via syscall */
	for (i=0; i<(truncated_bufsize/sizeof(long)); i++)
	    lbuf[i] = i*i;
	code = write (f, buf, truncated_bufsize);
	if (code == -1) {mmt_perror (pnm); goto done5;}
	if (code < truncated_bufsize) {
	    mmt_fprintf (stderr, "%s: wrote less data than expected\n", pnm);
	    goto done5;
	}

	/* Truncate */
	code = ftruncate (f, truncated_file_size);
	if (code == -1) {mmt_perror (pnm); goto done5;}

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done5;}

	printf ("Read past last page.\n");

	/* catch SIGBUS */
	if (sigsetjmp (env, 1) != 0) {
	    printf ("Catching SIGBUS as expected.\n");
	    goto caught_truncated_1;
	}

	/* read past last page */
	c = addr [truncated_past_last_page];

	mmt_fprintf (stderr, "%s: Failed to generate SIGBUS\n", pnm);
	goto done5;

caught_truncated_1:

	printf ("Write past last page.\n");

	/* catch SIGBUS */
	if (sigsetjmp (env, 1) != 0) {
	    printf ("Catching SIGBUS as expected.\n");
	    goto caught_truncated_2;
	}

	/* write past last page */
	addr [truncated_past_last_page] = '7';

	mmt_fprintf (stderr, "%s: Failed to generate SIGBUS\n", pnm);
	goto done5;

caught_truncated_2:

	printf ("Read on last page beyond EOF.\n");

	/* catch SIGBUS */
	if ((sigval = sigsetjmp (env, 1)) != 0) {
	    mmt_print_signal (sigval);
	    goto done5;
	}

	/* read on last page beyond EOF. */
	c = addr [truncated_on_last_page];
	if (c != 0) {
	    mmt_fprintf (stderr, "%s: non-zero beyond EOF\n", pnm);
	    goto done5;
	}

	printf ("Write on last page beyond EOF.\n");

	/* write on last page beyond EOF. */
	addr [truncated_on_last_page] = '7';

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done5;}
	if (stbuf.st_size != truncated_file_size) {
	    mmt_fprintf (stderr, "%s: file has wrong size\n", pnm);
	    goto done5;
	}
	printf ("File size is OK.\n");

	/* Clean up */
    done5:
	munmap (addr, truncated_bufsize);
    done5a:
	close (f);
    }

    printf ("File format: empty\n");

    {
	int empty_file_size = 0;
	int empty_past_last_page = 2048;
	char buf[4196];
	int empty_bufsize = sizeof(buf);
	struct stat stbuf;
	int f = open ("test_file_6", O_RDWR|O_CREAT|O_TRUNC, 0777);
	caddr_t addr = mmap (0, empty_bufsize, PROT_WRITE|PROT_READ,
			     MAP_SHARED, f, 0);
	long *laddr = (long *) addr;
	long *lbuf = (long *) buf;
	int code;
	int i;
	char c;

	/* bleah */
	if (addr == (caddr_t) -1) {mmt_perror (pnm); goto done6a;}

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done6;}

	printf ("Read past last page.\n");

	/* catch SIGBUS */
	if (sigsetjmp (env, 1) != 0) {
	    printf ("Catching SIGBUS as expected.\n");
	    goto caught_empty_1;
	}

	/* read past last page */
	c = addr [empty_past_last_page];

	mmt_fprintf (stderr, "%s: Failed to generate SIGBUS\n", pnm);
	goto done6;

caught_empty_1:

	printf ("Write past last page.\n");

	/* catch SIGBUS */
	if (sigsetjmp (env, 1) != 0) {
	    printf ("Catching SIGBUS as expected.\n");
	    goto caught_empty_2;
	}

	/* write past last page */
	addr [empty_past_last_page] = '7';

	mmt_fprintf (stderr, "%s: Failed to generate SIGBUS\n", pnm);
	goto done6;

caught_empty_2:

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done6;}
	if (stbuf.st_size != empty_file_size) {
	    mmt_fprintf (stderr, "%s: file has wrong size\n", pnm);
	    goto done6;
	}
	printf ("File size is OK.\n");

	/* Clean up */
    done6:
	munmap (addr, empty_bufsize);
    done6a:
	close (f);
    }

    printf ("Truncation tests\n");

    {
	int starting_size = 8192 * 4;
	int on_last_page = starting_size - 10;
	int first_trunc_size = 8192 * 2 + 2048;
	int second_trunc_size = 8192 * 1 + 2048;
	char buf[8192*4];
	int trunc_bufsize = sizeof(buf);
	struct stat stbuf;
	int f = open ("test_file_7", O_RDWR|O_CREAT|O_TRUNC, 0777);
	caddr_t addr = mmap (0, trunc_bufsize, PROT_WRITE|PROT_READ,
			     MAP_SHARED, f, 0);
	long *laddr = (long *) addr;
	long *lbuf = (long *) buf;
	int sigval;
	int code;
	int i;
	char c1, c2;

	/* bleah */
	if (addr == (caddr_t) -1) {mmt_perror (pnm); goto done7a;}

	/* Extend via syscall */
	for (i=0; i<(starting_size/sizeof(long)); i++)
	    lbuf[i] = i*i;
	code = write (f, buf, starting_size);
	if (code == -1) {mmt_perror (pnm); goto done7;}
	if (code < starting_size) {
	    mmt_fprintf (stderr, "%s: wrote less data than expected\n", pnm);
	    goto done7;
	}

	/* Invalidate all pages so it looks like they've never been accessed */
        code = msync (addr, starting_size, MMT_SYNC_INVALIDATE);
	if (code == -1) {mmt_perror (pnm); goto done7;}

	/* Bring some pages into memory by accessing them */
	c1 = addr [on_last_page];
	c2 = addr [second_trunc_size + 10];

	printf ("Truncate to page not previously accessed.\n");

	/* Truncate */
	code = ftruncate (f, first_trunc_size);
	if (code == -1) {mmt_perror (pnm); goto done7;}

	printf ("Read past last page.\n");

	/* catch SIGBUS */
	if (sigsetjmp (env, 1) != 0) {
	    printf ("Catching SIGBUS as expected.\n");
	    goto caught_trunc_1;
	}

	/* read past last page */
	c1 = addr [on_last_page];

	mmt_fprintf (stderr, "%s: Failed to generate SIGBUS\n", pnm);
	goto done7;

caught_trunc_1:

	printf ("Write past last page.\n");

	/* catch SIGBUS */
	if (sigsetjmp (env, 1) != 0) {
	    printf ("Catching SIGBUS as expected.\n");
	    goto caught_trunc_2;
	}

	/* write past last page */
	addr [on_last_page] = '7';

	mmt_fprintf (stderr, "%s: Failed to generate SIGBUS\n", pnm);
	goto done7;

caught_trunc_2:

	printf ("Read on new last page beyond EOF; page not previously used.\n");

	/* catch SIGBUS */
	if ((sigval = sigsetjmp (env, 1)) != 0) {
	    mmt_print_signal (sigval);
	    goto done7;
	}

	/* read on last page beyond EOF. */
	c1 = addr [first_trunc_size + 10];
	if (c1 != 0) {
	    mmt_fprintf (stderr, "%s: non-zero beyond EOF\n", pnm);
	    goto done7;
	}

	printf ("Write on last page beyond EOF.\n");

	/* write on last page beyond EOF. */
	addr [first_trunc_size + 10] = '7';

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done7;}
	if (stbuf.st_size != first_trunc_size) {
	    mmt_fprintf (stderr, "%s: file has wrong size\n", pnm);
	    goto done7;
	}
	printf ("File size is OK.\n");

	printf ("Truncate to page previously accessed.\n");

	/* Truncate again */
	code = ftruncate (f, second_trunc_size);
	if (code == -1) {mmt_perror (pnm); goto done7;}

	printf ("Read on new last page beyond EOF; page previously used.\n");

	/* catch SIGBUS */
	if ((sigval = sigsetjmp (env, 1)) != 0) {
	    mmt_print_signal (sigval);
	    goto done7;
	}

	/* read on last page beyond EOF. */
	c1 = addr [second_trunc_size + 10];
	if (c1 != 0) {
	    mmt_fprintf (stderr, "%s: non-zero beyond EOF\n", pnm);
	    goto done7;
	}

	printf ("Write on last page beyond EOF.\n");

	/* write on last page beyond EOF. */
	addr [second_trunc_size + 10] = '7';

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done7;}
	if (stbuf.st_size != second_trunc_size) {
	    mmt_fprintf (stderr, "%s: file has wrong size\n", pnm);
	    goto done7;
	}
	printf ("File size is OK.\n");

	/* Clean up */
    done7:
	munmap (addr, trunc_bufsize);
    done7a:
	close (f);
    }

    printf ("Hole in file tests\n");

    {
	char buf[8192 * 5];
	int hole_bufsize = sizeof(buf);
	int in_block_4_hole = 8192 * 3 + 2048;
	int in_block_2_hole = 8192 * 1 + 2048;
	struct stat stbuf;
	int f = open ("test_file_8", O_RDWR|O_CREAT|O_TRUNC, 0777);
	caddr_t addr = mmap (0, hole_bufsize, PROT_WRITE|PROT_READ,
			     MAP_SHARED, f, 0);
	long *laddr = (long *) addr;
	long *lbuf = (long *) buf;
	int sigval;
	int code;
	int i;
	char c;

	/* bleah */
	if (addr == (caddr_t) -1) {mmt_perror (pnm); goto done8a;}

	/* Write blocks 1, 3, and 5 */
	for (i=0; i<(8192/sizeof(long)); i++)
	    lbuf[i] = i*i;
	code = write (f, buf, 8192);
	if (code == -1) {mmt_perror (pnm); goto done8;}
	if (code < 8192) {
	    mmt_fprintf (stderr, "%s: wrote less data than expected\n", pnm);
	    goto done8;
	}
	lseek (f, 8192 * 2, 0);
	for (i=0; i<(8192/sizeof(long)); i++)
	    lbuf[i] = i*i;
	code = write (f, buf, 8192);
	if (code == -1) {mmt_perror (pnm); goto done8;}
	if (code < 8192) {
	    mmt_fprintf (stderr, "%s: wrote less data than expected\n", pnm);
	    goto done8;
	}
	lseek (f, 8192 * 4, 0);
	for (i=0; i<(8192/sizeof(long)); i++)
	    lbuf[i] = i*i;
	code = write (f, buf, 8192);
	if (code == -1) {mmt_perror (pnm); goto done8;}
	if (code < 8192) {
	    mmt_fprintf (stderr, "%s: wrote less data than expected\n", pnm);
	    goto done8;
	}

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done8;}

	printf ("Read from hole at block 4.\n");

	/* catch SIGBUS */
	if ((sigval = sigsetjmp (env, 1)) != 0) {
	    mmt_print_signal (sigval);
	    goto done8;
	}

	/* read from hole. */
	c = addr [in_block_4_hole];
	if (c != 0) {
	    mmt_fprintf (stderr, "%s: non-zero in hole\n", pnm);
	    goto done8;
	}

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done8;}

	printf ("Write to hole just read from.\n");

	/* write to hole just read from. */
	addr [in_block_4_hole] = '7';

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done8;}

	printf ("Write to hole not read from.\n");

	/* write to hole not read from. */
	addr [in_block_2_hole] = '7';

	/* Report size and allocation */
	fsync (f);
	code = fstat_and_print (f, &stbuf);
	if (code == -1) {mmt_perror (pnm); goto done8;}

	/* Clean up */
    done8:
	munmap (addr, hole_bufsize);
    done8a:
	close (f);
    }

    printf ("Fsync tests -- not yet implemented.\n");

    printf ("Space and quota error tests -- not yet implemented.\n");

    printf ("Fileset op tests -- not yet implemented.\n");

    printf ("Multiple process tests -- not yet implemented.\n");
}
