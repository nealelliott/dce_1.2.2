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
 * Revision 1.1.6.2  1996/02/17  23:23:08  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:44  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:13:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:37  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:51:41  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:19:09  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:50:39  rsalz
 * 	 06-apr-92 nm    - misc cleanup
 * 	                 - /dev/ncshelper -> /dev/ncs
 * 	                 - <auth_helper.h> -> <dce/auth_helper.h>
 * 	[1992/05/01  00:57:16  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:43:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
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
**  A user-space program (helper) that uses the "NCS device driver" to
**  fetch, process, and respond to kernel-initiated requests for help
**  in doing authentication stuff.  The real work is in a library generated
**  by the Security component (libauthelper.a).  In "production"
**  environments, the real helper program is "dfsbind", which also links
**  with libauthelper.a.
**
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <dce/auth_helper.h>

static int helper_fd = 0;

int helper_debug=0;

#define dprintf if(helper_debug)printf
static void helper_init ()
{
    helper_fd = open ("/dev/ncs", O_RDWR, 0);
    if (helper_fd < 0) {
        perror("/dev/ncs");
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
    *inlen = read (helper_fd, inbuf, maxinlen);
    dprintf("kernel read returned %d, errno %d\n", *inlen, errno);
    if (*inlen < 0) {
        *inlen = 0;
        return errno;
    }
    return 0;
}
        
static int helper_put_request (outbuf, outlen)
     char *outbuf;
     int outlen;
{
    if (write(helper_fd, outbuf, outlen) != outlen) {
        perror ("helper_put_request");
        sleep(5);
    }
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
