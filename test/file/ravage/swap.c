/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: swap.c,v $
 * Revision 1.1.8.1  1996/10/17  18:33:39  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:56:41  damon]
 *
 * Revision 1.1.3.1  1994/07/13  22:31:17  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:14:52  devsrc]
 * 
 * 	moving files from file/episode to test/file
 * 	[1994/03/22  20:28:59  delgado]
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
 * 	[1992/05/22  20:14:40  jdp]
 * 
 * Revision 1.1.4.4  1993/01/19  15:31:23  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:52:44  cjd]
 * 
 * Revision 1.1.4.3  1992/11/24  16:55:55  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:25:06  bolinger]
 * 
 * Revision 1.1.4.2  1992/10/27  20:52:18  jaffe
 * 	Transarc delta: jdp-ot5529-ravage-tools-should-use-strtol 1.1
 * 	  Selected comments:
 * 	    strtol is more flexible than atoi, interpreting things like (0x2000)
 * 	    correctly, so we use it instead
 * 	    Replace atoi(str) with strtol(str, NULL, 0).
 * 	[1992/10/27  13:00:13  jaffe]
 * 
 * $EndLog$
 */

/* Copyright (C) 1992, 1991 Transarc Corporation - All rights reserved */

#include <dcedfs/sysincludes.h>
#include <stdlib.h>


main(argc, argv)
   int		argc;
   char*	argv[];
{
   int		aggr_fd;
   char*	buf1;
   char*	buf2;
   long		byte_address1;
   long		byte_address2;
   unsigned	num;
   int		num_bytes1;
   int		num_bytes2;
   int		value;

   if (argc < 5)
      {
	 fprintf(stderr,
		 "usage: %s <aggr> <byte address 1> <byte address 2> <num>\n",
		 argv[0]);
	 exit(2);
      }
   
   if ((aggr_fd = open(argv[1], O_RDWR)) == -1)
      {
	 fprintf(stderr, "%s: %s: Can't open for writing\n", argv[0], argv[1]);
	 exit(2);
      }

   byte_address1 = strtol(argv[2], NULL, 0);
   byte_address2 = strtol(argv[3], NULL, 0);
   num = strtol(argv[4], NULL, 0);

   if ((buf1 = malloc(2 * num)) == NULL)
      {
	 fprintf(stderr, "%s: Can't malloc %d bytes\n", argv[0], 2 * num);
	 exit(2);
      }
   buf2 = buf1 + num;
	 
   /*
   ** Read first blob
   */
   if (lseek(aggr_fd, byte_address1, SEEK_SET) == -1)
      {
	 fprintf(stderr, "%s: %s: Seek failed\n", argv[0], argv[1]);
	 exit(2);
      }

   if ((num_bytes1 = read(aggr_fd, buf1, num)) == -1)
      {
	 fprintf(stderr, "%s: %s: Read failed\n", argv[0], argv[1]);
	 exit(2);
      }

   /*
   ** Read second blob
   */
   if (lseek(aggr_fd, byte_address2, SEEK_SET) == -1)
      {
	 fprintf(stderr, "%s: %s: Seek failed\n", argv[0], argv[1]);
	 exit(2);
      }

   if ((num_bytes2 = read(aggr_fd, buf2, num)) == -1)
      {
	 fprintf(stderr, "%s: %s: Read failed\n", argv[0], argv[1]);
	 exit(2);
      }

   /*
   ** Write the first blob
   */
   if (lseek(aggr_fd, byte_address2, SEEK_SET) == -1)
      {
	 fprintf(stderr, "%s: %s: Seek failed\n", argv[0], argv[1]);
	 exit(2);
      }

   if (write(aggr_fd, buf1, num_bytes1) < num_bytes1)
      {
	 fprintf(stderr, "%s: %s: Write failed\n", argv[0], argv[1]);
	 exit(2);
      }

   /*
   ** Write the second blob
   */
   if (lseek(aggr_fd, byte_address1, SEEK_SET) == -1)
      {
	 fprintf(stderr, "%s: %s: Seek failed\n", argv[0], argv[1]);
	 exit(2);
      }

   if (write(aggr_fd, buf2, num_bytes2) < num_bytes2)
      {
	 fprintf(stderr, "%s: %s: Write failed\n", argv[0], argv[1]);
	 exit(2);
      }

   if (close(aggr_fd) == -1)
      {
	 fprintf(stderr, "%s: %s: Can't close\n", argv[0], argv[1]);
	 exit(2);
      }
}	/* main() */	
