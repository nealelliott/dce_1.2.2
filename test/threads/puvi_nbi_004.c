/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: puvi_nbi_004.c,v $
 * Revision 1.1.16.2  1996/02/17  23:32:21  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:43  marty]
 *
 * Revision 1.1.16.1  1995/12/11  23:04:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:56  root]
 * 
 * Revision 1.1.13.1  1994/04/01  20:12:56  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:21:43  jd]
 * 
 * Revision 1.1.11.1  1993/10/12  19:38:29  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:29  rhw]
 * 
 * Revision 1.1.9.2  1993/06/14  20:19:23  sommerfeld
 * 	Use TIOCBREAK rather than CBREAK on HP-UX.
 * 	[1993/06/10  23:24:07  sommerfeld]
 * 
 * Revision 1.1.4.3  1993/02/01  21:46:46  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:08:11  hinman]
 * 
 * Revision 1.1.4.2  1992/12/30  16:56:25  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:14:28  zeliff]
 * 	Revision 1.1.2.2  1993/01/11  16:18:08  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1  1992/01/19  03:22:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1989, 1991 by
 *  Digital Equipment Corporation, Maynard, Massachusetts.
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
 *	PUVI_NBI_004
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-blocking I/O
 *
 *  ABSTRACT:
 *
 *	This program exercises CMA's version of "select".
 *
 *  AUTHORS:
 *
 *	Dave Winchell
 *
 *  CREATION DATE:
 *
 *	5 January 1989
 *
 *  MODIFICATION HISTORY:
 *	001	Paul Curtin	31 May 1990
 *		Converted test to use pthread interface.
 *	002	Webb Scales	10 September 1990
 *		Reordered includes to prevent ux.h from interfering with
 * 		file.h
 *	003	Dave Butenhof	11 October 1990
 *		Use cma_errno.h
 *	004	Dave Butenhof	5 February 1991
 *		Fix name string
 */


/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <cma_errno.h>
#include <dce/cts_test.h>
#include <stdio.h>
#include <sys/time.h>
#include <cma_ux.h>

#if _CMA_UNIX_TYPE != _CMA__SVR4
#include <sys/file.h>
#include <sgtty.h>
#else
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#endif

#if (_CMA_VENDOR_ == _CMA__HP)
#include <sys/ptyio.h>
#endif

#define thread_count	10	/* Number of threads to create */
#define iterations	40000	/* Number of iterations in each thread */

/* Routines to be executed as the thread actions */
static pthread_addr_t thread_compute  (pthread_addr_t);

static pthread_addr_t thread_usr  (pthread_addr_t);

/* 
 * Arrays with a lifetime at least as long as each thread that will
 * hold the inputs and outputs of the thread.  Normally, this storage
 * would be objects allocated from the heap.  We are using static
 * storage only because this is a simple test.
 */
static int 		input[thread_count];
static int		output[thread_count];

/* 
 * A mutex
 */
static pthread_mutex_t 	the_mutex;


int
main	(
	int	argc,
	char	*argv[])
    {
    int *ptr;
    int status;
    pthread_t	threads[thread_count];
    pthread_addr_t	result[thread_count];
    int	xstatus[thread_count];

    status = pthread_mutex_init (&the_mutex, pthread_mutexattr_default);
    check(status,"pthreat_mutex_init");
    status = pthread_create (&threads[0], 
			    pthread_attr_default, 
			    thread_compute,
			    (pthread_addr_t)(pthread_addr_t)0);
    check(status,"pthread_create");
    status = pthread_create (&threads[1], 
			    pthread_attr_default, 
			    thread_usr,
			    (argc > 1 ? (pthread_addr_t)argv[1]
                                 : (pthread_addr_t)0));
    check(status,"pthread_create");
    xstatus[0] = pthread_join (threads[0], (pthread_addr_t *)&ptr);
    xstatus[1] = pthread_join (threads[1], (pthread_addr_t *)&ptr);
    }


static pthread_addr_t
thread_compute	(
	pthread_addr_t	input_ptr)
    {
    int i;
    int status;

    for (i = 0; ; i++) {
	if (i%500000 == 0) {
	    status = pthread_mutex_lock (&the_mutex); 
	    check(status,"pthread_mutex_lock");
	    printf ("\t\t\tCompute Thread:  at iteration %d.\n", i);
	    status = pthread_mutex_unlock (&the_mutex); 
	    check(status,"pthread_mutex_unlock");
	    }
        }
    }


static pthread_addr_t
thread_usr	(
	pthread_addr_t	term)
    {
    int	    status;
    int     count;
    int     r, w, e, rv;
    int     fd;

#if _CMA_UNIX_TYPE == _CMA__SVR4
	struct termios 	attr;
#else
    struct sgttyb   ttyarg;
#endif

    struct timeval  timeout;

    char    buf[10];


    if (term) {
        if ((fd = open (term, 0, O_RDWR)) == -1) {
	    perror ("User Thread:  open failed");
	    return;
	    }
        }
    else 
        fd = 0;  /* stdin */

    timeout.tv_sec = 10;
    timeout.tv_usec = 10;

#if _CMA_UNIX_TYPE == _CMA__SVR4
	if ( tcgetattr(fd, &attr) != 0 )
		{
		perror("tcgetattr");
		return ((cma_t_address)-1);
		}
	attr.c_lflag	&=	~(ICANON);
	if ( tcsetattr(fd, TCSANOW, &attr) != 0 )
		{
		perror("tcsetatr");
		return( (cma_t_address)-1 );
		}
#else
    ioctl(fd, TIOCGETP, &ttyarg);
# ifdef __hpux
    ttyarg.sg_flags = ttyarg.sg_flags | TIOCBREAK;
# else
    ttyarg.sg_flags = ttyarg.sg_flags | CBREAK;
# endif
    ioctl(fd, TIOCSETP, &ttyarg);
#endif

    while(1) {
	while (1) {
	    r = 1 << fd; w = 0; e = 0;
	    rv = select (fd + 1, &r, &w, &e, &timeout);

	    if (rv > 0) break;
	    if (rv == 0) {
	        status = pthread_mutex_lock (&the_mutex); 
		check(status,"pthread_mutex_lock");
	        printf("User Thread:  select timeout\n");
		status = pthread_mutex_unlock (&the_mutex); 
		check(status,"pthread_mutex_unlock");
	        }
	    else perror ("User Thread:  select error");
	    }

	count = read(fd, buf, 10);
	buf[count] = '\0';

	status = pthread_mutex_lock (&the_mutex); 
	check(status,"pthread_mutex_lock");
#if 0
	write(fd, buf, count);
#endif
	printf ("User Thread: read \"%s\", select count = %d.\n", buf, rv);
	status = pthread_mutex_unlock (&the_mutex);
	check(status,"pthread_mutex_unlock");
        }
    }
