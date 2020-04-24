/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: metamucil.c,v $
 * Revision 1.1.8.1  1996/10/17  18:33:27  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:56:35  damon]
 *
 * Revision 1.1.3.1  1994/07/13  22:31:09  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:14:27  devsrc]
 * 
 * 	moving files from file/episode to test/file
 * 	[1994/03/22  20:28:50  delgado]
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
 * 	[1992/05/22  20:04:43  jdp]
 * 
 * Revision 1.1.61.1  1994/02/04  20:14:16  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:52  devsrc]
 * 
 * Revision 1.1.59.1  1993/12/07  17:19:51  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  14:04:34  jaffe]
 * 
 * Revision 1.1.4.5  1993/01/19  15:31:00  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:52:10  cjd]
 * 
 * Revision 1.1.4.4  1992/11/24  16:55:52  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:24:59  bolinger]
 * 
 * Revision 1.1.4.3  1992/10/27  20:51:45  jaffe
 * 	Transarc delta: jdp-ot5291-have-metamucil-determine-block-size 1.2
 * 	  Selected comments:
 * 	    This delta gives metamucil the capability to determine aggregate block
 * 	    and fragment sizes by reading the aggregate header(s).
 * 	    Add new routine, GetBlockAndFragSize(), that reads the aggregate header
 * 	    (or alternate headers, if necessary) to determine the block and fragment
 * 	    sizes.  However, if the user explicitly specifies values via the environment
 * 	    or command line switches, they override the values from the aggregate header.
 * 	    If the user didn't specify a value and no aggregate header is found,
 * 	    the defaults of 8192 and 1024 are used.
 * 	    Forgot to reset the aggregate stream at the end of GetBlockAndFragSize().
 * 	    Seek to the beginning of the file at the end of GetBlockAndFragSize().
 * 	Transarc delta: jdp-ot5529-ravage-tools-should-use-strtol 1.1
 * 	  Selected comments:
 * 	    strtol is more flexible than atoi, interpreting things like (0x2000)
 * 	    correctly, so we use it instead
 * 	    Replace atoi(str) with strtol(str, NULL, 0).
 * 	[1992/10/27  12:59:37  jaffe]
 * 
 * Revision 1.1.4.2  1992/08/31  20:09:06  jaffe
 * 	Transarc delta: jdp-ot4856-lfs-new-log-magic-in-metamucil 1.1
 * 	  Selected comments:
 * 	    Update metamucil to recognize new (and old) log pages.
 * 	    Add entry for log pages with the new magic number.
 * 	[1992/08/30  02:40:05  jaffe]
 * 
 * $EndLog$
 */

/* Copyright (C) 1992, 1991 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>

extern int	getopt();
extern char 	*getenv();
extern char	*optarg;
extern int	optind;

#define DEF_BLOCK_SIZE	8192
#define DEF_FRAG_SIZE	1024

#define ENV_BLOCK_SIZE	"RVG_BLOCK_SIZE"
#define ENV_FRAG_SIZE	"RVG_FRAG_SIZE"

#define SIZE_OF(array)	(sizeof (array) / sizeof((array)[0]))

enum entry_type	{UNDEFINED, MAGIC, MAGIC_TABLE};

struct magic_stuff
{
   enum entry_type	type;
   void*		entry;
   unsigned		entry_size;
   unsigned		offset_or_frag_size;
   char*		name;
   unsigned		num_found;
};

static unsigned dir_entry_magic =	0x76e694c1;
static unsigned dir_noentry_magic =	0xb937c268;

static struct magic_stuff	dir_entries[] = 
{
   {MAGIC, &dir_entry_magic, 4, 0, "Dir. entry"},
   {MAGIC, &dir_noentry_magic, 4, 0, "Empty dir. entry"}
};

typedef struct epi_uuid {
  u_int32               longField1;
  u_int16               shortField1;
  u_int16               shortField2;
  unsigned char         miscChars[8];
} epi_uuid_t;

epi_uuid_t acl_uuid = {
  /* d076c532-0a1d-11ca-953d-02608c2ea96e */
  0xd076c532,
  0x0a1d,
  0x11ca,
  {0x95, 0x3d, 0x02, 0x60, 0x2e, 0xa9, 0x6e, 0x00}
};

static struct magic_stuff	frag_aligned[] =
{
   {MAGIC_TABLE, dir_entries, SIZE_OF(dir_entries), 32},
   {MAGIC, &acl_uuid, sizeof acl_uuid, 0, "ACL UUID"}
};

static unsigned avl_anode_magic = 	0x3198a2e0;
static unsigned bitmap_magic = 		0x5b7674d7;
static unsigned fs_anode_magic = 	0xb7afc1db;
static unsigned	indirect_magic = 	0x5a308d31;
static unsigned log_magic =		0x73697045;
static unsigned old_log_magic =		0x45706973;

static struct magic_stuff	block_aligned[] =
{
   {UNDEFINED},
#define FRAG_ALIGNED_INDEX	1
   {MAGIC_TABLE, frag_aligned, SIZE_OF(frag_aligned), 0},
   {MAGIC, &avl_anode_magic, 4, 4, "AVL anode block"},
   {MAGIC, &bitmap_magic, 4, 4, "Bitmap block"},
   {MAGIC, &fs_anode_magic, 4, 4, "FS anode block"},
   {MAGIC, &indirect_magic, 4, 4, "Indirect block"},
   {MAGIC, &log_magic, 4, 8, "Log page"},
   {MAGIC, &old_log_magic, 4, 8, "Log page (old format)"}
};

#define BAD_BLOCK_FRAME_SIZE	1024
static unsigned bad_block_magic = 	0x76618c6d;

static struct magic_stuff	bad_block_frames[] =
{
   {MAGIC, &bad_block_magic, 4, 4, "Bad block frame"}
};

#define DIR_PAGE_SIZE	8192
static unsigned dir_header_magic = 	0x2c70bf7f;
static unsigned dir_page_magic = 	0xb63185f6;

static struct magic_stuff	dir_pages[] = 
{
   {UNDEFINED},
   {MAGIC_TABLE, bad_block_frames, SIZE_OF(bad_block_frames),
       BAD_BLOCK_FRAME_SIZE},
   {MAGIC, &dir_header_magic, 4, 0, "Dir. header"},
   {MAGIC, &dir_page_magic, 4, 0, "Dir. page"}
};

#define AGGR_HEADER_QUANTUM		(64 * 1024)
#define AGGR_HEADER_SIZE		(7 * sizeof(long))
#define ANODE_BLOCK_MAGIC_OFFSET	(sizeof(long))
#define AGGR_HEADER_MAGIC_OFFSET	(3 * sizeof(long))
#define AGGR_HEADER_MAGIC		0x8946f098
#define AGGR_HEADER_BLOCKSIZE_OFFSET	(5 * sizeof(long))
#define AGGR_HEADER_FRAGSIZE_OFFSET	(6 * sizeof(long))
static long aggr_header_locs[] =
{
    1, 7, 46, 204, 836, 3955, 12836, 35592, 84148, 623118, 3682396,
    6017662, 39640819, 73675885, 563160323, 1269535423
};
static unsigned num_aggr_header_locs = SIZE_OF(aggr_header_locs);

static void	Fatal(char *, ...);
static void	GetBlockAndFragSize(FILE *, int *, int *);
static int	IsMeta(
    char *, u_int, struct magic_stuff *, u_int, u_int, char **);
static void	PrintCounts(struct magic_stuff *, u_int);
static void	PrintWhatIs(
    FILE *, char *, long, char *, u_int, struct magic_stuff *, u_int);
static void	UsageError(char *);
static void	Warn(char *, ...);

static char*	progname = "metamucil";


main(argc, argv)
   int		argc;
   char*	argv[];
{
   FILE*		aggr_stream = stdin;
   int			block_size = 0;
   char*		buf;
   unsigned		buf_size;
   int			frag_size = 0;
   unsigned		num_bytes;
   unsigned		num_meta;
   unsigned		num_user;
   int			opt;
   struct magic_stuff*	root_table;
   unsigned		root_table_size;
   int			verbose = 0;
   long			what_addr = -1;
   char*		zero_buf;

   progname = argv[0];

   while ((opt = getopt(argc, argv, "b:f:vw:")) != -1)
      switch (opt)
	 {
	 case 'b':
	    block_size = strtol(optarg, NULL, 0);
	    break;

	 case 'f':
	    frag_size = strtol(optarg, NULL, 0);
	    break;

	 case 'v':
	    if (what_addr != -1)
	       UsageError(argv[0]);

	    verbose = 1;
	    break;

	 case 'w':
	    if (verbose)
	       UsageError(argv[0]);

	    what_addr = strtol(optarg, NULL, 0);
	    break;

	 case '?':
	 default:
	    UsageError(argv[0]);
	 }

   if (optind < argc && (aggr_stream = fopen(argv[optind], "r")) == NULL)
      Fatal("%s: Can't open for reading", argv[optind]);

   if (block_size == 0)
      {
	 char*	env_block_size = getenv(ENV_BLOCK_SIZE);

	 if (env_block_size != NULL)
	    block_size = strtol(env_block_size, NULL, 0);
      }

   if (frag_size == 0)
      {
	 char*	env_frag_size = getenv(ENV_FRAG_SIZE);

	 if (env_frag_size != NULL)
	    frag_size = strtol(env_frag_size, NULL, 0);
      }
   
   GetBlockAndFragSize(aggr_stream, &block_size, &frag_size);
   if (!block_size)
       block_size = DEF_BLOCK_SIZE;
   if (!frag_size)
       frag_size = DEF_FRAG_SIZE;

   if (verbose)
       fprintf(stderr, "Using block size = %d, and frag size = %d\n",
	       block_size, frag_size);

   block_aligned[FRAG_ALIGNED_INDEX].offset_or_frag_size = frag_size;

   if (block_size >= DIR_PAGE_SIZE)
      {
	 block_aligned[0].type = MAGIC_TABLE;
	 block_aligned[0].entry = dir_pages;
	 block_aligned[0].entry_size = SIZE_OF(dir_pages);
	 block_aligned[0].offset_or_frag_size = DIR_PAGE_SIZE;
	 buf_size = block_size;
	 root_table = block_aligned;
	 root_table_size = SIZE_OF(block_aligned);
      }
   else
      {
	 dir_pages[0].type = MAGIC_TABLE;
	 dir_pages[0].entry = block_aligned;
	 dir_pages[0].entry_size = SIZE_OF(block_aligned);
	 dir_pages[0].offset_or_frag_size = block_size;
	 buf_size = DIR_PAGE_SIZE;
	 root_table = dir_pages;
	 root_table_size = SIZE_OF(dir_pages);
      }

   if ((buf = malloc(buf_size)) == NULL)
      Fatal("Can't malloc %d bytes", buf_size);

   if (what_addr != -1)
      {
	 PrintWhatIs(aggr_stream, argv[optind], what_addr, buf, buf_size,
		     root_table, root_table_size);
	 exit(0);
      }

   if ((zero_buf = calloc(buf_size, sizeof(char))) == NULL)
      Fatal("Can't calloc %d bytes", buf_size);

   while (num_bytes = fread(buf, sizeof(char), buf_size, aggr_stream))
      if (num_bytes < buf_size ||
	  IsMeta(buf, num_bytes, root_table, root_table_size, buf_size, NULL))
	 {
	    if (fwrite(buf, sizeof(char), num_bytes, stdout) == 0)
	       Fatal("fwrite() failed");
	    num_meta++;
	 }
      else
	 {
	    if (fwrite(zero_buf, sizeof(char), buf_size, stdout) ==0)
	       Fatal("fwrite() failed");
	    num_user++;
	 }

   if (verbose)
      {
	 PrintCounts(root_table, root_table_size);

	 fprintf(stderr, "\nMeta blocks: %u\nUser blocks: %u\n",
		 num_meta, num_user);
	 fprintf(stderr, "Total blocks: %u\n", num_meta + num_user);
      }
}	/* main() */


static void
Fatal(char *template, ...)
{
   va_list	args;

   va_start(args, template);
   vfprintf(stderr, template, args);
   fprintf(stderr, ", errno = %d\n", errno);
   va_end(args);
   exit(2);
}	/* Fatal() */


static void
GetBlockAndFragSize(aggr_stream, block_size_p, frag_size_p)
    FILE*	aggr_stream;
    int*	block_size_p;
    int*	frag_size_p;
{
    char	buf[AGGR_HEADER_SIZE];
    int		block_size;
    int		frag_size;
    unsigned	bytes_read;
    unsigned	i;
    unsigned	magic1;
    unsigned	magic2;
    long	offset;

    for (i = 0; i < num_aggr_header_locs; i++) {
	offset = aggr_header_locs[i] * AGGR_HEADER_QUANTUM;

	if (fseek(aggr_stream, offset, SEEK_SET) == -1)
	    Fatal("Seek failed");

	bytes_read = fread(buf, 1, AGGR_HEADER_SIZE, aggr_stream);
	if (bytes_read < AGGR_HEADER_SIZE) {
	    Warn("Bad aggregate header at block $block; trying next one");
	    continue;
	}

	bcopy(buf + ANODE_BLOCK_MAGIC_OFFSET, (char *)&magic1, sizeof magic1);
	bcopy(buf + AGGR_HEADER_MAGIC_OFFSET, (char *)&magic2, sizeof magic2);

	if (magic1 != avl_anode_magic || magic2 != AGGR_HEADER_MAGIC) {
	    Warn("Bad aggregate header at block $block; trying next one");
	    continue;
	} else {
	    bcopy(buf + AGGR_HEADER_BLOCKSIZE_OFFSET, (char *)&block_size,
		  sizeof block_size);
	    if (*block_size_p != 0 && *block_size_p != block_size) {
		Warn("User-specified block size of %d does not aggree with aggregate header: %d\n",
		     *block_size_p, block_size);
		Warn("Using user-specified value\n");
	    } else {
		*block_size_p = block_size;
	    }

	    bcopy(buf + AGGR_HEADER_FRAGSIZE_OFFSET, (char *)&frag_size,
		  sizeof frag_size);
	    if (*frag_size_p != 0 && *frag_size_p != frag_size) {
		Warn("User-specified frag size of %d does not aggree with aggregate header: %d\n",
		     *frag_size_p, frag_size);
		Warn("Using user-specified value\n");
	    } else {
		*frag_size_p = frag_size;
	    }
	    break;
	}
    }

    /* Reset the stream */
    if (fseek(aggr_stream, 0L, SEEK_SET) == -1)
	Fatal("Seek failed");
}	/* GetBlockAndFragSize() */


static int
IsMeta(buf, buf_size, magic_table, magic_table_size, frag_size, namep)
   char*		buf;
   unsigned		buf_size;
   struct magic_stuff*	magic_table;
   unsigned		magic_table_size;
   unsigned		frag_size;
   char**		namep;
{
   char*		frag;
   unsigned		i;
   int			is_meta = 0;

   for (i = 0; i < magic_table_size && !is_meta; ++i)
      {
	 struct magic_stuff*	msp = &magic_table[i];
	 
	 for (frag = buf; frag < buf + buf_size && !is_meta; frag += frag_size)
	    {
	       char*	magic_number;

	       switch(msp->type)
		  {
		  case MAGIC:
		     magic_number = frag + msp->offset_or_frag_size;

		     if (msp->entry_size == sizeof(unsigned))
			is_meta = *(unsigned*)magic_number ==
			   *(unsigned*)msp->entry;
		     else
			is_meta = !bcmp(magic_number, (char*)msp->entry,
					msp->entry_size);

		     if (is_meta)
			{
			   msp->num_found++;
			   if (namep)
			      *namep = msp->name;
			}

		     break;

		  case MAGIC_TABLE:
		     is_meta = IsMeta(frag, frag_size,
				      (struct magic_stuff*)msp->entry,
				      msp->entry_size,
				      msp->offset_or_frag_size,
				      namep);
		     break;

		  case UNDEFINED:
		     break;

		  default:
		     assert(0);		/* Shouldn't get here */
		     break;
		  }
	    }
      }

   return is_meta;
}	/* IsMeta() */


static void
PrintCounts(table, table_size)
   struct magic_stuff*	table;
   unsigned		table_size;
{
   int	i;

   for (i = 0; i < table_size; ++i)
      {
	 struct magic_stuff*	msp = &table[i];

	 switch (msp->type)
	    {
	    case MAGIC:
	       fprintf(stderr, "%s: %u\n", msp->name, msp->num_found);
	       break;

	    case MAGIC_TABLE:
	       PrintCounts((struct magic_stuff*)msp->entry, msp->entry_size);
	       break;

	    case UNDEFINED:
	       break;

	    default:
	       assert(0);		/* Shouldn't get here */
	       break;
	    }
      }
}	/* PrintCounts() */


static void
PrintWhatIs(aggr_stream, aggr_name, addr, buf, buf_size,
	    root_table, root_table_size)
   FILE* 		aggr_stream;
   char*		aggr_name;
   long			addr;
   char*		buf;
   unsigned		buf_size;
   struct magic_stuff*	root_table;
   unsigned		root_table_size;
{
   char*	name;
   int		num_bytes;

   addr &= ~(buf_size - 1);

   if (fseek(aggr_stream, addr, SEEK_SET) == -1)
      Fatal("%s: Seek failed", aggr_name);

   if ((num_bytes = fread(buf, sizeof(char), buf_size, aggr_stream)) == 0)
      Fatal("%s: Read failed", aggr_name);
   
   if (IsMeta(buf, num_bytes, root_table, root_table_size, buf_size, &name))
      printf("The region from %ld to %ld (%u bytes) contains a %s\n",
	     addr, addr + buf_size - 1, buf_size, name);
   else
      printf("No meta data\n");
}	/* PrintWhatIs() */


static void
UsageError(prog_name)
   char*	prog_name;
{
   Fatal("usage: %s [-b <block size>] [-f <frag size>] [-v|-w <addr>] [<aggr>]",
	 prog_name);
}	/* UsageError() */


static void
Warn(char *template, ...)
{
   va_list	args;

   va_start(args, template);
   vfprintf(stderr, template, args);
   va_end(args);
}	/* Warn() */


