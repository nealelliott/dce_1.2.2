/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kernel_speed.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:12  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:47  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:13:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:47  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:52:26  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:19:56  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:50:44  rsalz
 * 	Edited as part of rpc6 drop.
 * 	[1992/05/01  00:57:21  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:42:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * ncstest server.
 *
 * This is another ncs test suite.  This code is designed to work
 * in both user and kernel RPC environments.  This somewhat limits
 * what can be done in the code and most definitely induces strange
 * looking code. 
 *
 * Parts of the code in this module get used in 3 different contexts:
 * (1) Standard user-space ncstest client.
 * (2) User space driver for in-kernel ncstest client.
 * (3) In-kernel ncstest client.
 *
 * With lots of luck, this is one of the last client / server templates
 * that we'll have to write for such purposes.  Just snap new tests into
 * the ifinfo array and away we go...!
 *
 * KERNEL_TEST_DRIVER is #define'd when this module is being built for
 * context (2).  _KERNEL is #define'd for context (3).  Nothing in
 * particular is #define'd for context (1); though _KERNEL and
 * KERNEL_TEST_DRIVER will NOT be defined).
 */

#  include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define NCSDEVICE "/dev/ncs"
#define NIOCDESVERIFY          _IO('N', 11)           /* run v2client */
#define NIOCDESSPEED           _IOW('N', 12, int)     /* run v2client */


/**************************************************************************/
/* 
 * Stuff used in the standard user mode code
 * and in the in-kernel test (contexts 1 and 3)
 */

/*
 * K E R N E L _ D O I T
 *
 * Get the kernel to start the test server.
 */
static kernel_doit(argc, argv)
int argc;
char *argv[];
{
    int fd;
    int count;

    if ((fd = open(NCSDEVICE, O_RDWR)) == -1)
    {
        printf("can't open '%s'\n", NCSDEVICE);
        perror("");
        exit(-1);
    }
    count = atoi(argv[1]);
    
    printf("count is %d\n", count);

    if (ioctl(fd, NIOCDESSPEED, &count) == -1)
    {
        perror("ioctl");
        exit(-1);
    }
    return 0;
}



/**************************************************************************/
/*
 * The following code is used only in user space (both contexts 1 and 2).
 */

/* 
 * M A I N
 */
main(argc, argv)
int argc;
char *argv[];
{
    kernel_doit(argc, argv);
}



