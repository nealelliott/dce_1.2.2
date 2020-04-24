/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: uclient.c,v $
 * Revision 1.1.4.2  1996/02/17  23:23:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:00  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:14:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:13  root]
 * 
 * Revision 1.1.2.2  1993/01/13  19:54:12  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:21:49  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:42:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <ncstest.h>
#include <sys/errno.h>

main(argc, argv)
int argc;
char *argv[];
{
   error_status_t st;
   ncstest_inargs_t inargs;
   ncstest_outargs_t outargs;
   int i, nbytes, maxbytes;
   char *cp;
   int rc;

   maxbytes = NCSTEST_MAX_ARGVBYTES;

   if (argc > NCSTEST_MAX_ARGC) 
   {
     printf("too many cmd line arguments: %d\n", argc);
     return -1;
   }
   
   inargs.argc = argc;
   cp = &inargs.argvchars[0];
   nbytes = 0;
   for (i=0; i<argc; i++)
   {
      nbytes = (strlen(argv[i]) + 1);
      if ((maxbytes -= nbytes) <= 0) 
      {
         printf("too many cmd line argv bytes: >%d\n", NCSTEST_MAX_ARGVBYTES);
         return(-1);
      }
      strcpy(cp, argv[i]);
      cp += nbytes;
   }
   ncstest_c_doit(&inargs, &outargs);
   if (errno == EFAULT)
      printf("copyin or copyout failed\n");
   if (outargs.st != 0)
     printf("ncstest_c_doit returned bad status\n");
}
