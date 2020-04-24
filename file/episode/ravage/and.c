/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: and.c,v $
 * Revision 1.1.13.1  1996/10/02  17:25:52  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:59  damon]
 *
 * Revision 1.1.8.1  1994/06/09  14:01:40  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:56  annie]
 * 
 * Revision 1.1.4.3  1993/01/19  15:30:49  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:51:53  cjd]
 * 
 * Revision 1.1.4.2  1992/10/27  20:51:17  jaffe
 * 	Transarc delta: jdp-ot5529-ravage-tools-should-use-strtol 1.1
 * 	  Selected comments:
 * 	    strtol is more flexible than atoi, interpreting things like (0x2000)
 * 	    correctly, so we use it instead
 * 	    Replace atoi(str) with strtol(str, NULL, 0).
 * 	[1992/10/27  12:58:49  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/22  21:09:17  jdp
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
 * 	[1992/05/22  19:58:10  jdp]
 * 
 * $EndLog$
 */

/* Copyright (C) 1992, 1991 Transarc Corporation - All rights reserved */

#include <stdio.h>

main(argc, argv)
   int		argc;
   char*	argv[];
{
   if (argc < 3)
      {
	 fprintf(stderr, "usage: %s <num1> <num2>", argv[0]);
	 exit(2);
      }

   printf("%d\n", strtol(argv[1], NULL, 0) & strtol(argv[2], NULL, 0));
}	/* main() */
