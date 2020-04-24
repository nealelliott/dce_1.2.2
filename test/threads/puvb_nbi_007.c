/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: puvb_nbi_007.c,v $
 * Revision 1.1.67.2  1996/02/17  23:32:19  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:42  marty]
 *
 * Revision 1.1.67.1  1995/12/11  23:04:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:55  root]
 * 
 * Revision 1.1.65.1  1994/04/01  20:12:54  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:21:40  jd]
 * 
 * Revision 1.1.63.1  1993/10/12  19:38:27  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:27  rhw]
 * 
 * Revision 1.1.5.3  1993/02/01  21:46:41  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:08:03  hinman]
 * 
 * Revision 1.1.5.2  1992/12/30  16:56:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:14:24  zeliff]
 * 	Revision 1.1.3.2  1993/01/11  16:17:55  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 9.5.1.2  92/11/23  16:36:11  blurie
 * 	For SVR4, use memset() instead of bzero() so that we can build
 * 	the tests without using reentrant libraries or libucb.
 * 	[92/11/23  16:35:03  blurie]
 * 
 * Revision 9.5.1.1  92/07/24  19:00:38  hinman
 * 	Create merge branch
 * 
 * Revision 9.5  92/07/24  19:00:36  hinman
 * 	Update checkin
 * 
 * Revision 1.1.2.2  1992/05/22  18:43:50  bolinger
 * 	Fix OT defect 3644:  provide test coverage for re-issued recvmsg()
 * 	and recvfrom() syscalls, to ensure that CMA wrapper replaces an
 * 	OUT parameter overwritten by the call.  Also change to exercise
 * 	sendto() as well as sendmsg().
 * 	[1992/05/22  18:42:08  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:22:23  devrcs
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
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	PUVB_NBI_007
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	This program tests behavior of recvmsg()/recvfrom() when re-issue
 *	of call is needed -- to make sure they don't zero address buffer
 *	info.
 *
 *  AUTHORS:
 *
 *	Webb Scales
 *
 *  CREATION DATE:
 *
 *	19 May 1992 - created from puvb_nbi_001.c
 *
 *  MODIFICATION HISTORY:
 */

/*
 *  INCLUDE FILES 
 */

#include <pthread.h>
#include <dce/cts_test.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netdb.h>


#define my_failure(message) {                                         \
        perror(message);                                              \
	cts_failed(message);                                         \
        } 

#define threads 10
#define MSG_SIZ 80

int                 t_socket[threads];
struct sockaddr_in  t_sockaddr[threads];

pthread_mutex_t start_mutex;
pthread_cond_t start_cond;
int start_count;

/*
 *  This routine sets up a socket and binds a name (port?) to it
 */
void
make_socket	(
	int			*s,
	struct sockaddr_in	*sa)
    {
    int	s_size;
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
t_sendmsg	(
	int		id,
	struct msghdr		*mh,
	struct sockaddr_in	*sa,
	char			*message)
{
    int  c_cnt;

    mh->msg_name = (caddr_t)sa; /* Install target sockaddr in message header */
    mh->msg_namelen = sizeof(*sa);
    mh->msg_iov->iov_base = message;/* Attach the message */
    mh->msg_iov->iov_len = strlen (message) + 1;

    c_cnt = sendmsg (t_socket[id], mh, 0);

    if (c_cnt == -1)
		my_failure("Error calling 'sendmsg'");
    if (c_cnt != mh->msg_iov->iov_len) 
        cts_failed ("Error calling 'sendmsg', only partial message sent.\n");
}

void
t_sendto	(
	int		id,
	struct msghdr		*mh,
	struct sockaddr_in	*sa,
	char			*message)
{
    int  c_cnt;

    c_cnt = sendto (t_socket[id], message, strlen(message) + 1, 0,
		sa, sizeof (*sa));

    if (c_cnt == -1)
		my_failure("Error calling 'sendto'");
    if (c_cnt != strlen(message) + 1)
        cts_failed ("Error calling 'sendto', only partial message sent.\n");
}

void
t_send	(
	int		id,
	struct msghdr		*mh,
	struct sockaddr_in	*sa,
	char			*message)
{
	if (id % 2 == 0)
		t_sendmsg(id, mh, sa, message);
	else
		t_sendto(id, mh, sa, message);
}

void
t_recvmsg	(
	int	id,
	struct msghdr	*mh,
	char		*message)
{
    int  c_cnt;
	struct sockaddr saddr;
	int slen;

	slen = sizeof (saddr);
#if (_CMA_UNIX_TYPE == _CMA__SVR4)
	memset(&saddr, '\0', slen);
#else
	bzero(&saddr, slen);
#endif
	mh->msg_name = (caddr_t)&saddr;
	mh->msg_namelen = slen;
	mh->msg_iov->iov_base = message;/* Attach the message buffer */
	mh->msg_iov->iov_len = MSG_SIZ;
	c_cnt = recvmsg (t_socket[id], mh, 0);
	slen = mh->msg_namelen;
    if (c_cnt == -1)
		my_failure("Error calling 'recvmsg'  ");
    if (c_cnt == 0)
        cts_failed ("Error calling 'recvmsg', only partial message received");
	if (!slen)
		cts_failed ("recvmsg cleared address buffer length");
}

void
t_recvfrom	(
	int	id,
	struct msghdr	*mh,
	char		*message)
{
    int  c_cnt;
	struct sockaddr saddr;
	int slen;

	slen = sizeof (saddr);
#if (_CMA_UNIX_TYPE == _CMA__SVR4)
	memset(&saddr, '\0', slen);
#else
	bzero(&saddr, slen);
#endif
	c_cnt = recvfrom (t_socket[id], message, MSG_SIZ, 0, &saddr, &slen);
    if (c_cnt == -1)
		my_failure("Error calling 'recvfrom'  ");
    if (c_cnt == 0)
        cts_failed ("Error calling 'recvfrom', only partial message received");
	if (!slen)
		cts_failed ("recvmsg/recvfrom cleared address buffer length");
}

void
t_recv	(
	int	id,
	struct msghdr	*mh,
	char		*message)
{
	if (id % 2 == 0)
	 	t_recvmsg(id, mh, message);
	else
	 	t_recvfrom(id, mh, message);
}


pthread_addr_t
relay	(
	int	id)
    {
    struct msghdr   my_msg_hdr;
    struct iovec    my_iov[1];
    char            my_message[MSG_SIZ];


	pthread_mutex_lock(&start_mutex);
	++start_count;
	if (start_count == threads)
		pthread_cond_signal(&start_cond);
	pthread_mutex_unlock(&start_mutex);
	
    my_msg_hdr.msg_namelen = sizeof (t_sockaddr[id]);
    my_msg_hdr.msg_iov = my_iov;
    my_msg_hdr.msg_iovlen = 1;
    my_msg_hdr.msg_accrights = 0;
    my_msg_hdr.msg_accrightslen = 0;

    if (id == 0) {                 /* First thread in relay chain: initiator */
		pthread_mutex_lock(&start_mutex);
		while (start_count < threads)
			pthread_cond_wait(&start_cond, &start_mutex);
		pthread_mutex_unlock(&start_mutex);

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
    int      cnt;
    pthread_t       thread[threads];
    int	 status;
    int  exit_status;
    pthread_addr_t      result;


    cts_test ("PUVB_NBI_007", "Test re-issued recvmsg()/recvfrom()");

	pthread_mutex_init(&start_mutex, pthread_mutexattr_default);
	pthread_cond_init(&start_cond, pthread_condattr_default);
    for (cnt = 0; cnt < threads; cnt++)
        make_socket (&t_socket[cnt], &t_sockaddr[cnt]);

    for (cnt = 1; cnt < threads; cnt++)
        status = pthread_create (
	        &thread[cnt],
	        pthread_attr_default,
	        relay,
	        (pthread_addr_t)cnt);
	check(status,"pthread_create");

    relay (0);

    for (cnt = 1; cnt < threads; cnt++) {
        exit_status = pthread_join (
				    thread[cnt],
				    &result);

	if (exit_status == -1) {
	    printf ("Thread #%d exited with #%d.\n", cnt, exit_status);
	    cts_failed ("Thread exited abnormally.");
	    }

	status = pthread_detach (&thread[cnt]);
	check(status,"pthread_detach");
        }

    cts_result ();

    }
