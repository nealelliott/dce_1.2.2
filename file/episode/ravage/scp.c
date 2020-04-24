/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: scp.c,v $
 * Revision 1.1.69.1  1996/10/02  17:26:10  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:33:13  damon]
 *
 * $EndLog$
 */

/* Copyright (C) 1994, 1991 Transarc Corporation - All rights reserved */

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
    char*	srcName;
    char*	destName;
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

	    /* Read list of non-zero block addresss from file and use that to
             * seek (instead of searching) in the source file. */

	    nzb_fname = optarg;
	    break;

	 case 'n':
	    if (verbose)
		 UsageError();

	    /* Print non-zero block addresses to <stdout>. */

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
    if ((nzb && num_args < 1) || (!nzb && num_args > 2))
	UsageError();

    if (nzb_fname && (nzb_stream = fopen(nzb_fname, "r")) == NULL) {
	fprintf(stderr, "%s: %s: Can't open for reading\n",
		argv[0], nzb_fname);
	exit(2);
    }

    if (num_args > 0) {
	srcName = argv[optind];
	if (strcmp("-", srcName) == 0) {
	    src = stdin;
	    srcName = "<STDIN>";
	} else if ((src = fopen(srcName, "r")) == NULL) {
	    fprintf(stderr, "%s: %s: Can't open for reading\n",
		    argv[0], srcName);
	    exit(2);
	}
	num_args--;
	optind++;
    } else {
	src = stdin;
	srcName = "<STDIN>";
    }

    if (num_args > 0) {
	destName = argv[optind];
	if (strcmp("-", destName) == 0) {
	    dest = stdout;
	    destName = "<STDOUT>";
	} else if ((dest = fopen(argv[optind], "w")) == NULL) {
	    fprintf(stderr, "%s: %s: Can't open for writing\n",
		    argv[0], destName);
	    exit(2);
	}
    } else {
	if (!nzb) {

	    /* If no dest is specified then write to <stdout> assuming it is
             * seekable.  Unless "-n" is specified in which case no dest
             * specification means don't write the dest file at all just write
             * the non-zero block addresses to stdout. */

	   dest = stdout;
	   destName = "<STDOUT>";
       }
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
	    Fseek(src, byte_address, SEEK_SET, srcName);
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
		Fseek(dest, byte_address, SEEK_SET, destName);
		Fwrite(buf, num_bytes, dest, destName);
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
	    Fseek(dest, src_length - 1, SEEK_SET, destName);
	    Fwrite(zero_buf, 1, dest, destName);
	}
	if (nzb)
	    printf("%u\n", src_length);
    }

    if (dest != NULL && fclose(dest) == EOF) {
	fprintf(stderr, "%s: %s: Can't close\n", argv[0], destName);
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
	    "usage:\t%s [-B <buf size>] [-N <nzb file>|-v] [<src> [<dest>]]\n",
	    progname);
    fprintf(stderr, "\t%s [-B <buf size>] [-N <nzb file>] -n <src> [<dest>]\n",
	    progname);
    exit(2);
}	/* UsageError() */
