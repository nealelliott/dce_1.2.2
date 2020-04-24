/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cuvb_nbi_001.c,v $
 * Revision 1.1.44.2  1996/02/17  23:30:53  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:28  marty]
 *
 * Revision 1.1.44.1  1995/12/11  23:00:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:35:46  root]
 * 
 * Revision 1.1.42.1  1994/04/01  20:10:37  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:16:24  jd]
 * 
 * Revision 1.1.40.1  1993/10/12  19:36:19  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:25  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:33:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:09:03  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:21:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and copied
 *  only in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any other
 *  copies thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without notice
 *  and should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	CUVB_NBI_001
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	This program tests non-blocking I/O on sockets.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	21 November 1989
 *
 *  MODIFICATION HISTORY:
 *
 *	001	Webb Scales	4 September 1990
 *		Changed CMA calls to pass structures by reference.
 *	002	Dave Butenhof	23 May 1991
 *		Fix compilation errors under new MIPS C compiler.
 */


/*
 *  INCLUDE FILES 
 */

#include <dce/cma.h>
#include <dce/cts_test.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <cma_ux.h>
#include <netdb.h>


#define my_failure(message) {                                         \
        perror(message);                                              \
	cts_failed(message);                                         \
        } 

#define threads 10
#define MSG_SIZ 80

int                 t_socket[threads];
struct sockaddr_in  t_sockaddr[threads];


/*
 *  This routine sets up a socket and binds a name (port?) to it
 */
void
make_socket	(
	int			*s,
	struct sockaddr_in	*sa)
    {
    cma_t_integer	s_size;
    char		name[80];
    struct hostent	*he;


    *s = socket (AF_INET, SOCK_DGRAM, 0);
    if (*s == -1) my_failure ("Error calling 'socket'");

    /*
     * Build address to bind the socket to:  
     *    on this node, 
     *    port 0:  the system will assign us a port number (not 0)
     */
    memset (sa, 0, sizeof (*sa));
    sa->sin_family = AF_INET;
    sa->sin_port = 0;
    
    if (gethostname(name, sizeof(name)) < 0)
        my_failure("Can't get my host name");
    if ((he = gethostbyname(name)) == 0) 
        my_failure("Can't convert my host name to address");
    sa->sin_addr.s_addr = *(unsigned long*) he->h_addr_list[0];
    s_size = sizeof (*sa);

    if (bind (*s, sa, s_size) < 0) my_failure ("Error calling 'bind'");
    if (getsockname (*s, sa, &s_size) < 0)
        my_failure ("Error calling 'getsockname'");
}


void
t_send	(
	cma_t_integer		id,
	struct msghdr		*mh,
	struct sockaddr_in	*sa,
	char			*message)
    {
    cma_t_integer  c_cnt;


    mh->msg_name = (caddr_t)sa; /* Install target sockaddr in message header */
    mh->msg_namelen = sizeof(*sa);
    mh->msg_iov->iov_base = message;/* Attach the message */
    mh->msg_iov->iov_len = strlen (message) + 1;

    c_cnt = sendmsg (t_socket[id], mh, 0);

    if (c_cnt == -1) my_failure("Error calling 'sendmsg'");
    if (c_cnt != mh->msg_iov->iov_len) 
        cts_failed ("Error calling 'sendmsg', only partial message sent.\n");

    }

void
t_recv	(
	cma_t_integer	id,
	struct msghdr	*mh,
	char		*message)
    {
    cma_t_integer  c_cnt;


    mh->msg_name = (caddr_t)0;  /* Not interested in this information */
    mh->msg_iov->iov_base = message;/* Attach the message buffer */
    mh->msg_iov->iov_len = MSG_SIZ;

    c_cnt = recvmsg (t_socket[id], mh, 0);

    if (c_cnt == -1) my_failure("Error calling 'recvmsg'  ");
    if (c_cnt == 0)
        cts_failed ("Error calling 'recvmsg', only partial message received.\n");
    }


cma_t_address
relay	(
	cma_t_integer	id)
    {
    struct msghdr   my_msg_hdr;
    struct iovec    my_iov[1];
    char            my_message[MSG_SIZ];


    my_msg_hdr.msg_iov = my_iov;
    my_msg_hdr.msg_iovlen = 1;
    my_msg_hdr.msg_accrights = 0;
    my_msg_hdr.msg_accrightslen = 0;

    if (id == 0) {                 /* First thread in relay chain: initiator */
        /* Send the message on to the next guy */
        t_send (id, &my_msg_hdr, &t_sockaddr[id + 1], "hello...");
	t_recv (id, &my_msg_hdr, my_message);

	printf("Message received: \"%s\".\n", my_message);
        }
    else if (id == threads - 1) {  /* Last thread in relay chain: echoer */
        /* Send the message back to the previous guy, after adding our number*/
	t_recv (id, &my_msg_hdr, my_message);
	my_message[strlen (my_message) + 1] = '\0';
	my_message[strlen (my_message)] = '0' + id;
        t_send (id, &my_msg_hdr, &t_sockaddr[id - 1], my_message);
        }
    else {                         /* Other threads in chain: relayers */
        /* Send the message on to the next guy, after adding our number */
	t_recv (id, &my_msg_hdr, my_message);
	my_message[strlen (my_message) + 1] = '\0';
	my_message[strlen (my_message)] = '0' + id;
        t_send (id, &my_msg_hdr, &t_sockaddr[id + 1], my_message);

        /* Send the message back to the previous guy, after adding our number*/
	t_recv (id, &my_msg_hdr, my_message);
	my_message[strlen (my_message) + 1] = '\0';
	my_message[strlen (my_message)] = '0' + id;
        t_send (id, &my_msg_hdr, &t_sockaddr[id - 1], my_message);
        }
    }


main ()
    {
    cma_t_integer      cnt;
    cma_t_thread       thread[threads];
    cma_t_exit_status  exit_status;
    cma_t_address      result;


    cts_test ("CUVB_NBI_001", "Test non-blocking I/O on sockets");

    cma_init ();

    for (cnt = 0; cnt < threads; cnt++)
        make_socket (&t_socket[cnt], &t_sockaddr[cnt]);

    for (cnt = 1; cnt < threads; cnt++)
        cma_thread_create (
	        &thread[cnt],
	        &cma_c_null,
	        (cma_t_start_routine)relay,
	        (cma_t_address)cnt);

    relay (0);

    for (cnt = 1; cnt < threads; cnt++) {
        cma_thread_join (
	        &thread[cnt],
		&exit_status,
		&result);

	if (exit_status != cma_c_term_normal) {
	    printf ("Thread #%d exited with #%d.\n", cnt, exit_status);
	    cts_failed ("Thread exited abnormally.");
	    }

	cma_thread_detach (&thread[cnt]);
        }

    cts_result ();

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_001.C */
/*  *7    29-MAY-1991 18:02:57 BUTENHOF "Fix compilation errors" */
/*  *6     4-SEP-1990 16:52:43 SCALES "Convert to new CMA interfaces" */
/*  *5     3-JUL-1990 12:06:47 BUTENHOF "Change copyright" */
/*  *4    21-JUN-1990 08:34:39 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *3     7-JUN-1990 11:39:27 SCALES "Updates to CTS" */
/*  *2    31-MAY-1990 17:38:00 SCALES "Integrate Apollo changes" */
/*  *1    17-MAY-1990 13:59:03 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVB_NBI_001.C */
