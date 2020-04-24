/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dev_helper.c,v $
 * Revision 1.1.4.2  1996/02/17  23:23:09  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:45  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:13:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:41  root]
 * 
 * Revision 1.1.2.2  1993/01/13  19:51:58  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:19:29  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:43:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  dev_helper.c V=2 10/18/91 //littl/prgy/src/helper
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. 
**
**   Copyright (c) Hewlett-Packard Company 1991
**   Unpublished work. All Rights Reserved.
**  
**
**  NAME
**
**      dev_helper.c
**
**  FACILITY:
**
**      DCE Security - Kernel authenticated RPC support.
**
**  ABSTRACT:
**
**      Device driver interface to kernel helper.
**      This module was split out of auth_helper.c to make it easier
**      to integrate the latter module with the system-call based
**      kernel helper interface used by DFS.
**      The device-driver based interface is still in use at HP for testing
**      and development.
**
**    
**
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <auth_helper.h>

#define AIXSYS

#ifdef AIXSYS
extern int ncsread(struct iovec *iovp, int iovcnt);
extern int ncswrite(struct iovec *iovp, int iovcnt);
extern int ncsopen();
#endif

static int helper_fd = 0;

int helper_debug=0;

#define dprintf if(helper_debug)printf
static void helper_init ()
{
   int rc;

   rc = ncsopen();
   if (rc < 0) 
   {
      perror(" could not initialize helper...");
      exit(1);
   }

   auth_helper_init();
}

static int helper_get_request (inbuf, maxinlen, inlen)
     char *inbuf;
     int maxinlen;
     int *inlen;
{
    dprintf("(helper_get_request) about to call read...\n");
    *inlen = ncsread(inbuf, maxinlen);
    dprintf("kernel read returned %d, errno %d\n", *inlen, errno);
    if (*inlen < 0) 
    {
       *inlen = 0;
        return errno;
    }
    return 0;
}
        
static int helper_put_request (outbuf, outlen)
     char *outbuf;
     int outlen;
{
      long len;

      if ((len = ncswrite(outbuf, outlen)) != outlen) {
         perror("helper_put_request");
         sleep(5);
      }
      else
         dprintf("put request into kernel\n");

}

main(argc, argv)
    int argc;
    char **argv;
{
    if (argc > 1) {
        if (strcmp(argv[1], "-d") == 0)
            helper_debug++;
    }
       
    helper_init();

    for (;;) {
        unsigned long opcode, code;
        char inbuf[8192], outbuf[8192];
        char *inptr;
        int inlen, outlen;
        
        code = helper_get_request (inbuf, sizeof(inbuf), &inlen);
        if (code == EINTR) {
          dprintf("interrupted system call.. restarting..\n");
                continue;
        }
        if (code != 0) {
            perror ("helper_get_request failed");
            sleep(5);
            continue;
        }
        do_auth_request(inbuf, inlen, outbuf, &outlen);
        helper_put_request (outbuf, outlen);
    }
}
