/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krpc_helper_test.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:52  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:14:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:58  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:53:13  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:20:45  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:51:23  rsalz
 * 	Edited as part of rpc6 drop.
 * 	[1992/05/01  00:57:53  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:43:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca.
**
**
**  NAME
**
**      krpc_helper_test.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Kernel helper process test program.
**
**
*/

#include <commonp.h>
#include <com.h>
#include <krpc_helper.h>

void krpc_helperTest()
{
    int i, j, code;
    long count;
    struct krpc_helper *h;
    unsigned char message[30];
    unsigned char omessage[30];

    for (i=0; i<3; i++) {
        h = krpc_GetHelper();
        if (!h) {
            printf("krpc_GetHelper() failed\n");
            return;
        }
        for (j=0; j<10; j++) {
            message[j*2] = 't';
            message[j*2+1] = i+'0';
        }
        message[21] = 0;
        printf("message sent to user space: %s\n", message);
        krpc_WriteHelper (h, message, 20);
        code = krpc_ReadHelper (h, omessage, sizeof(omessage), &count);
        krpc_PutHelper(h);
        if (code != 0) {
            printf("krpc_ReadHelper failed; code %x\n", code);
        } else {
            printf("message recved from user space: %d bytes, %s\n",
                count, omessage);
        }
    }
}
