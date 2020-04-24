/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
 */
/*
 * HISTORY
 * $Log: cuvi_nbi_004.c,v $
 * Revision 1.1.18.2  1996/02/17  23:31:10  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:42  marty]
 *
 * Revision 1.1.18.1  1995/12/11  23:01:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:00  root]
 * 
 * Revision 1.1.15.1  1994/04/01  20:11:06  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:17:31  jd]
 * 
 * Revision 1.1.13.1  1993/10/12  19:36:45  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:37  rhw]
 * 
 * Revision 1.1.11.2  1993/06/14  20:17:29  sommerfeld
 * 	Use TIOCBREAK rather than CBREAK on HP-UX.
 * 	[1993/06/10  23:22:54  sommerfeld]
 * 
 * Revision 1.1.6.3  1992/12/30  16:51:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:07  zeliff]
 * 
 * Revision 1.1.6.2  1992/09/29  20:39:47  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:34:15  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/11  17:07:40  bolinger
 * 	Fix OT defect 2161:  provide/correct missing/bad cts_test() banners.
 * 	[1992/05/11  13:45:45  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:21:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1990
 *  by DIGITAL Equipment Corporation, Maynard, Mass.
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
 *	CUVI_NBI_004
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
 *
 *	001	Webb Scales	 4 September 1990
 *		Changed CMA calls to pass structures by reference.
 *	002	Webb Scales	26 September 1990
 *		Returned to using cma_ux wrappers, and put in global lock.
 *	003	Put in "retest" using a polling select just before reading.
 *	004	Dave Butenhof	11 October 1990
 *		Use cma_errno.h
 */


/*
 *  INCLUDE FILES
 */

#include <dce/cma.h>
#include <cma_errno.h>
#include <stdio.h>
#include <sys/time.h>
#include <cma_ux.h>

#if !defined(SNI_SVR4_POSIX)
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

#define select_threads	4	/* Number of threads doing selects */
#define compute_threads	2	/* Number of compute-bound threads */
#define thread_count	select_threads + compute_threads 


/* Routines to be executed as the thread actions */
static cma_t_address thread_compute  (cma_t_address);

static cma_t_address thread_select  (cma_t_address);


cma_t_thread	main_thread;
char		*filename = cma_c_null_ptr;


int
main	(
	int	argc,
	char	*argv[])
    {
    cma_t_thread	threads[thread_count];
    cma_t_integer	i;
    cma_t_address	dummy;

	cts_test("CUVI_NBI_004", "Test CMA select implementation interactively");

    cma_init ();

    cma_thread_get_self (&main_thread);
    filename = (argc > 1 ? argv[1] : cma_c_null_ptr);

    for (i = 0; i < compute_threads; i++)
	cma_thread_create (
		&threads[i], 
		&cma_c_null, 
		thread_compute,
		(cma_t_address)i);
    for (i = compute_threads; i < thread_count; i++)
	cma_thread_create (
		&threads[i], 
		&cma_c_null, 
		thread_select, 
		(cma_t_address)i);

    cma_lock_global ();
    printf (
	    "\nType characters on %s.  They should be echoed quickly.\n",
	    (argc > 1 ? argv[1] : "stdin"));
    printf ("\nType \"quit\" (in a single read) to quit.\n\n");
    cma_unlock_global ();

    /*
     * Go into an alertable (effectively infinite) wait
     */
    cma_thread_join (&threads[0], (cma_t_exit_status *)&dummy, &dummy);
    }


static cma_t_address
thread_compute	(
	cma_t_address	id)
    {
    int i;


    for (i = 0; ; i++) {
	if (i%500000 == 0) {
	    cma_lock_global ();
	    printf ("\t\t\tCompute Thread #%d:  at iteration %d.\n", id, i);
	    cma_unlock_global (); 
	    }
        }
    }


static cma_t_address
thread_select	(
	cma_t_address	id)
    {
    char    buf[10];
    char    id_str[24];
    int     count;
    int     r, w, e, rv;
    int     fd;
#if defined(SNI_SVR4_POSIX)
	struct termios 	attr;
#else
    struct sgttyb   ttyarg;
#endif /* defined(SNI_SVR4_POSIX) */
    struct timeval  poll, timeout;


    sprintf (id_str, "Select Thread #%d", id);

    if (filename) {
        if ((fd = open (filename, 0, O_RDWR)) == -1) {
	    perror (id_str);
	    return (cma_t_address)-1;
	    }
        }
    else 
        fd = 0;  /* stdin */

    poll.tv_sec = 0;
    poll.tv_usec = 0;
    timeout.tv_sec = 10;
    timeout.tv_usec = 10;

#if defined(SNI_SVR4_POSIX)
	if ( tcgetattr(fd, &attr) != 0 )
		{
		perror("tcgetattr");
		return ((cma_t_address)-1);
		}
	attr.c_lflag	&=	~(ICANON);
	attr.c_cc[VMIN] = 1;
	attr.c_cc[VTIME] = 0;
	if ( tcsetattr(fd, TCSAFLUSH, &attr) != 0 )
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
#endif /* defined(SNI_SVR4_POSIX) */

    cma_lock_global (); 
    printf("%s:  starting, fd = %d.\n", id_str, fd);
    cma_unlock_global (); 

    while(1) {
	while (1) {
	    r = 1 << fd; w = 0; e = 0;
	    rv = select (fd + 1, &r, &w, &e, &timeout);

	    if (rv > 0) break;
	    if (rv == 0) {
	        cma_lock_global (); 
	        printf("%s:  select timeout\n", id_str);
		cma_unlock_global (); 
	        }
	    else perror (id_str);
	    }

	cma_lock_global (); 
	printf("%s:  select successful, count = %d.\n", id_str, rv);
	cma_unlock_global (); 

	r = 1 << fd; w = 0; e = 0;
	rv = select (fd + 1, &r, &w, &e, &poll);

	if (rv > 0) {
	    count = read(fd, buf, 10);
	    buf[count] = '\0';

	    cma_lock_global ();
	    printf ("%s: read \"%s\".\n", id_str, buf);
	    cma_unlock_global ();

	    if (!strcmp (buf, "quit"))
		cma_thread_alert (&main_thread);
	    }
	else if (rv == 0) {
	    cma_lock_global (); 
	    printf("%s:  beaten to the read\n", id_str);
	    cma_unlock_global (); 
	    }
	else perror (id_str);
        }
    }

/*  DEC/CMS REPLACEMENT HISTORY, Element CUVI_NBI_004.C */
/*  *8    11-OCT-1990 13:04:09 BUTENHOF "Fix use of errno" */
/*  *7     2-OCT-1990 11:29:38 SCALES "Improve test" */
/*  *6    27-SEP-1990 14:16:25 SCALES "" */
/*  *5     4-SEP-1990 16:53:57 SCALES "Convert to new CMA interfaces" */
/*  *4     3-JUL-1990 12:07:05 BUTENHOF "Change copyright" */
/*  *3    21-JUN-1990 08:35:23 BUTENHOF "Convert to stream format for ULTRIX build" */
/*  *2    31-MAY-1990 17:38:36 SCALES "Integrate Apollo changes" */
/*  *1    17-MAY-1990 13:59:27 SCALES "Initial version" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CUVI_NBI_004.C */
