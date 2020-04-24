/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bash.c,v $
 * Revision 1.1.87.1  1996/10/02  17:25:53  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:33:00  damon]
 *
 * Revision 1.1.80.2  1994/06/09  14:01:42  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:36:57  annie]
 * 
 * Revision 1.1.80.1  1994/02/04  20:14:14  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:10:51  devsrc]
 * 
 * Revision 1.1.78.1  1993/12/07  17:19:50  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  14:04:19  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/19  15:30:52  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:51:57  cjd]
 * 
 * Revision 1.1.4.3  1992/11/24  16:26:43  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:24:49  bolinger]
 * 
 * Revision 1.1.4.2  1992/10/27  20:51:29  jaffe
 * 	Transarc delta: jdp-ot5529-ravage-tools-should-use-strtol 1.1
 * 	  Selected comments:
 * 	    strtol is more flexible than atoi, interpreting things like (0x2000)
 * 	    correctly, so we use it instead
 * 	    Replace atoi(str) with strtol(str, NULL, 0).
 * 	[1992/10/27  12:59:04  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/22  21:09:31  jdp
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
 * 	[1992/05/22  19:59:53  jdp]
 * 
 * $EndLog$
 */

/* Copyright (C) 1992, 1991 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#include <stdlib.h>

main(argc, argv)
   int		argc;
   char*	argv[];
{
   int		aggr_fd;
   char*	buf;
   long		byte_address;
   unsigned	num;			/* # of values to write */
   unsigned	size;			/* bytes being set */
   unsigned	nValues;		/* number of byte values specified */
   unsigned	valueLen;		/* number of bytes in each value */
   u_int32	value[16];
   int i;
   int code;

   if (argc < 4)
      {
usage:
	 fprintf(stderr, "usage: %s <aggr> <byte address> <num> [-W32|-W16] <value>*\n",
		 argv[0]);
	 exit(2);
      }
   
   if ((aggr_fd = open(argv[1], O_WRONLY)) == -1)
      {
	 fprintf(stderr, "%s: %s: Can't open for writing\n", argv[0], argv[1]);
	 exit(2);
      }

   byte_address = strtoul(argv[2], NULL, 0);
   num = strtoul(argv[3], NULL, 0);

   /* parse the various values */
   nValues = argc-4;
   valueLen = 1;
   if (strncmp(argv[4],"-W",2) == 0) {
       nValues--;
       if (strcmp(argv[4], "-W32") == 0)
	   valueLen = 4;		/* assume values are 32-bit words */
       else if (strcmp(argv[4], "-W16") == 0)
	   valueLen = 2;		/* assume values are 16-bit words */
       else goto usage;
   }
   if (nValues > sizeof(value)/sizeof(value[0])) {
       fprintf (stderr, "Too many values are specified\n");
       goto usage;
   }
   if (nValues > num) {
       fprintf (stderr, "More values specified than required\n");
       goto usage;
   }
   for (i=0; i<nValues; i++) {
       char *cp;
       char *ap = argv[argc-nValues+i];
       long discard;

       /* handle negative numbers... */
       if (*ap == '-') 
	   value[i] = (u_int32)strtol(ap, &cp, 0);
       else
	   value[i] = (u_int32)strtoul(ap, &cp, 0);
       if (cp-ap != strlen(ap)) {
	   fprintf (stderr, "Illegal value: %s\n", ap);
	   goto usage;
       }
       if (((valueLen == 1) &&
	    ((discard = value[i] & 0xffffff00) != 0) &&
	    (discard != 0xffffff00))
	   || ((valueLen == 2) &&
	       ((discard = value[i] & 0xffff0000) != 0) &&
	       (discard != 0xffff0000))) {
	   fprintf (stderr, "Value too large: %d\n", value[i]);
	   goto usage;
       }
   }
   size = num*valueLen;			/* number of bytes to affect */
   if (nValues == 0)
       buf = calloc(size, sizeof(char));
   else
       buf = malloc(size);
   if (buf == NULL) {
       fprintf(stderr, "%s: Can't allocate %d bytes\n", argv[0], size);
       exit(2);
   }

   if (nValues > 0) {
       if ((nValues == 1) && (valueLen == 1))
	   memset(buf, value[0], size);
       else {
	   int v = 0;			/* value index */
	   for (i=0; i<num; i++) {
	       u_int32 *buf32 = (u_int32 *)buf;
	       u_int16 *buf16 = (u_int16 *)buf;

	       switch (valueLen) {
		 case 1: buf[i] = value[v]; break;
		 case 2: buf16[i] = value[v]; break;
		 case 4: buf32[i] = value[v]; break;
	       }
	       v = (v+1) % nValues;
	   }
       }
   }
	       
	       
   code = lseek(aggr_fd, byte_address, SEEK_SET);
   if (code == -1)
      {
	 fprintf(stderr, "%s: %s: Seek failed\n", argv[0], argv[1]);
	 exit(2);
      }

   code = write(aggr_fd, buf, size);
   if (code != size)
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
