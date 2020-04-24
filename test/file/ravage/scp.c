/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: scp.c,v $
 * Revision 1.1.8.1  1996/10/17  18:33:36  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:56:40  damon]
 *
 * Revision 1.1.3.1  1994/07/13  22:31:15  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:14:50  devsrc]
 * 
 * 	moving files from file/episode to test/file
 * 	[1994/03/22  20:28:57  delgado]
 * 
 * 	Transarc delta: jdp-ot3675-lfs-add-salvager-tests 1.1
 * 	  Files modified:
 * 	    Makefile
 * 	    ravage: Makefile, and.c, bash.c, copy.c, copyall.sh
 * 	    ravage: metamucil.c, nebconv.pl, pfaults.pl, ravage.sh
 * 	    ravage: reravage.sh, rvg_setup.sh, scp.c, setall.sh, swap.c
 * 	    ravage: swapall.sh, which.sh, zeroall.sh
 * 	    scavenge: Makefile, aggrinfo.pl, efs.pl, efs_query.pl
 * 	    scavenge: epidump.pl, fsdump.pl, simple_test.pl, upquota.pl
 * 	    scavenge: util.pl
 * 	  Selected comments:
 * 	    This delta adds the ravager and scavenger tests to the Episode source tree.
 * 	    These packages are for testing the Episode salvager.
 * 	[1992/05/22  20:12:40  jdp]
 * 
 * Revision 1.1.62.1  1994/02/04  20:14:19  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:53  devsrc]
 * 
 * Revision 1.1.60.1  1993/12/07  17:19:53  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  14:04:47  jaffe]
 * 
 * Revision 1.1.4.3  1993/01/19  15:31:17  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:52:35  cjd]
 * 
 * Revision 1.1.4.2  1992/10/27  20:51:57  jaffe
 * 	Transarc delta: jdp-ot5529-ravage-tools-should-use-strtol 1.1
 * 	  Selected comments:
 * 	    strtol is more flexible than atoi, interpreting things like (0x2000)
 * 	    correctly, so we use it instead
 * 	    Replace atoi(str) with strtol(str, NULL, 0).
 * 	[1992/10/27  12:59:55  jaffe]
 * 
 * $EndLog$
 */

/* Copyright (C) 1992, 1991 Transarc Corporation - All rights reserved */

#include <dcedfs/osi.h>

extern int	getopt();
extern char 	*getenv();
extern char	*optarg;
extern int	optind;
extern long	strtol();

#define DEF_BUF_SIZE	8192

#define ENV_BUF_SIZE	"RVG_BUF_SIZE"

static void	UsageError(void);
static void	Fseek(FILE *, long, int, char *);
static void	Fwrite(char *, int, FILE *, char *);

char *progname;

main(argc, argv)
    int		argc;
    char*	argv[];
{
    long	byte_address = 0;
    int		buf_size = 0;
    char*	buf;
    FILE*	dest = NULL;
    unsigned	num_args;
    int		num_bytes;
    unsigned	num_zero = 0;
    unsigned	num_non_zero = 0;
    unsigned	nzb = 0;
    char*	nzb_fname = NULL;
    FILE*	nzb_stream = NULL;
    int		opt;
    FILE*	src;
    long	src_length = 0;
    long	dst_length = 0;
    int		srcind;
    int		verbose = 0;
    char*	zero_buf;

    progname = argv[0];
    while ((opt = getopt(argc, argv, "B:N:nv")) != -1) {
	switch (opt) {
	 case 'B':
	    buf_size = strtol(optarg, (char **)NULL, 0);
	    break;

	 case 'N':
	    if (verbose)
		 UsageError();

	    nzb_fname = optarg;
	    break;

	 case 'n':
	    if (verbose)
		 UsageError();

	    nzb = 1;
	    break;

	 case 'v':
	    if (nzb || nzb_fname)
		 UsageError();

	    verbose = 1;
	    break;

	 case '?':
	 default:
	    UsageError();
	}
    }

    num_args = argc - optind;
    if (nzb && num_args < 1 || !nzb && num_args < 2)
	UsageError();

    if (nzb_fname && (nzb_stream = fopen(nzb_fname, "r")) == NULL) {
	fprintf(stderr, "%s: %s: Can't open for reading\n",
		argv[0], nzb_fname);
	exit(2);
    }

    if (strcmp("-", argv[optind]) == 0) {
	if (nzb_stream) {
	    fprintf(stderr, "%s: Can't have both -N and -\n", argv[0]);
	    exit(2);
	 } else
	    src = stdin;
    } else if ((src = fopen(argv[optind], "r")) == NULL) {
        fprintf(stderr, "%s: %s: Can't open for reading\n",
		argv[0], argv[optind]);
	exit(2);
    }
    srcind = optind;

    optind++;
    if (num_args >= 2 && (dest = fopen(argv[optind], "w")) == NULL) {
	 fprintf(stderr, "%s: %s: Can't open for writing\n",
		 argv[0], argv[optind]);
	 exit(2);
    }

    if (buf_size == 0) {
	char *env_buf_size = getenv(ENV_BUF_SIZE);
	if (env_buf_size != NULL)
	    buf_size = strtol(env_buf_size, (char **)NULL, 0);
	else
	    buf_size = DEF_BUF_SIZE;
     }

    if ((buf = malloc((unsigned)buf_size)) == NULL) {
	 fprintf(stderr, "%s: Can't malloc %d bytes\n", argv[0], buf_size);
	 exit(2);
    }

    if ((zero_buf = calloc((unsigned)buf_size, sizeof (char))) == NULL) {
	 fprintf(stderr, "%s: Can't calloc %d bytes\n", argv[0], buf_size);
	 exit(2);
     }

    for (;;) {
	if (nzb_stream) {
	    if (fscanf(nzb_stream, "%u", &byte_address) == EOF)
		break;
	    Fseek(src, byte_address, SEEK_SET, argv[srcind]);
	    src_length = byte_address;
	}

	num_bytes = fread(buf, sizeof (char), buf_size, src);
	if (num_bytes == 0)
	    break;

	src_length += num_bytes;

	if (num_bytes < buf_size || nzb_stream ||
	    memcmp(buf, zero_buf, buf_size)) {
	    dst_length = src_length;
	    if (dest != NULL) {
		Fseek(dest, byte_address, SEEK_SET, argv[optind]);
		Fwrite(buf, num_bytes, dest, argv[optind]);
		num_non_zero++;
	     }
	     if (nzb)
		 printf("%u\n", byte_address);
	 } else
	     num_zero++;

	 byte_address += buf_size;
    }

    if (src_length != dst_length) {
	if (dest != NULL) {
	    Fseek(dest, src_length - 1, SEEK_SET, argv[optind]);
	    Fwrite(zero_buf, 1, dest, argv[optind]);
	}
	if (nzb)
	    printf("%u\n", src_length);
    }

    if (dest != NULL && fclose(dest) == EOF) {
	fprintf(stderr, "%s: %s: Can't close\n", argv[0], argv[optind]);
	exit(2);
    }

    if (verbose)
	printf("Non-zero bufs:\t%u\nZero bufs:\t%u\nTotal bufs:\t%u\n",
	      num_non_zero, num_zero, num_non_zero + num_zero);

    return 0;
}	/* main() */

static void
Fseek(
    FILE *fp,
    long off,
    int whence,
    char *filename)
{
    if (fseek(fp, off, whence) == -1) {
	fprintf(stderr, "%s: %s: Seek failed\n", progname, filename);
	exit(2);
    }
}

static void
Fwrite(
    char *buf,
    int nbyte,
    FILE *fp,
    char *filename)
{
    if (fwrite(buf, sizeof (char), nbyte, fp) == 0) {
	fprintf(stderr, "%s: %s: Write failed\n", progname, filename);
	exit(2);
    }
}

static void
UsageError(void)
{
    fprintf(stderr,
	    "usage:\t%s [-B <buf size>] [-N <nzb file>|-v] <src> <dest>\n",
	    progname);
    fprintf(stderr, "\t%s [-B <buf size>] [-N <nzb file>] -n <src> [<dest>]\n",
	    progname);
    exit(2);
}	/* UsageError() */
