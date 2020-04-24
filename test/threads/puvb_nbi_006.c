/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: puvb_nbi_006.c,v $
 * Revision 1.1.86.2  1996/02/17  23:32:18  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:41  marty]
 *
 * Revision 1.1.86.1  1995/12/11  23:04:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:54  root]
 * 
 * Revision 1.1.84.1  1994/04/01  20:12:52  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:21:36  jd]
 * 
 * Revision 1.1.82.1  1993/10/12  19:38:26  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:26  rhw]
 * 
 * Revision 1.1.4.5  1993/02/03  23:32:19  jd
 * 	Moved the include of pthread.h to before the check of
 * 	_CMA_UNIX_TYPE == _CNA_SVR4.
 * 	[1993/02/03  23:25:13  jd]
 * 
 * Revision 1.1.4.4  1993/02/01  21:46:35  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  17:07:55  hinman]
 * 
 * Revision 1.1.4.3  1992/12/30  16:56:15  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:14:20  zeliff]
 * 	Revision 1.1.7.2  1993/01/11  16:17:46  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 9.5.1.5  92/12/17  14:27:03  raj
 * 	[raj] New naming + parallelized reentrant library changes.
 * 
 * Revision 9.5.3.2  92/12/17  13:02:44  raj
 * 	[raj] New naming + parallelized reentrant library changes
 * 
 * Revision 9.5.1.4  92/10/20  16:43:11  raj
 * 	Merge in SVR4 changes
 * 
 * Revision 9.5.2.2  92/10/20  16:42:05  raj
 * 	Merge in SVR4 changes
 * 
 * Revision 9.5.1.3  92/08/05  13:56:36  hinman
 * 	[hinman] - Fold backward from DCE1TOP
 * 
 * Revision 9.5.2.2  92/08/05  11:28:23  blurie
 * 	Merge our changes into OSF files.
 * 
 * Revision 9.5.1.1  92/07/24  19:00:13  hinman
 * 	Create merge branch
 * 
 * Revision 9.5  92/07/24  19:00:11  hinman
 * 	Update checkin
 * 
 * Revision 1.1  1992/01/19  03:22:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *  Copyright (c) 1990, 1991 by
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
 *	PUVB_NBI_006
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	This test will varify that U*ix calls such as open and
 *	close really, truly do work.  
 *      (This differs from the previous test in that each thread
 *      acquires it's very own descriptor when reading data from
 *      the created file.)
 *
 *  AUTHORS:
 *
 *	CURTIN
 *
 *  CREATION DATE:
 *
 *	30 March 1990
 *
 *  MODIFICATION HISTORY:
 *	001	Paul Curtin	31 May 1990
 *		Converted test to use pthread interface.
 *	002	Paul Curtin	5 July 1990
 *		Adding ifdef proto
 *	003	Paul Curtin	17 July 1990
 *		Added an include file
 *	004	Webb Scales	10 September 1990
 *		Reordered includes to prevent ux.h from interfering with file.h
 *	005	Dave Butenhof	5 February 1991
 *		Fix name string, and remove incorrect call to cma_init().
 */


/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#if (_CMA_UNIX_TYPE == _CMA__SVR4) && (!defined(SNI_BSD_CMP))
#include <sys/types.h>/* bring in definition for ushort needed by sys/file.h */
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>		/* bring in definition for SEEK_SET */
#endif
#include <dce/cts_test.h>
#include <sys/file.h>
#include <limits.h>
#include <cma_ux.h>

#define BUF_SIZE     80
#define NUM_THREADS  5
#define NUM_ROWS     10

char          copy[NUM_ROWS][BUF_SIZE];     /* Holds copy of what is in file */
pthread_mutex_t   position_mutex;           /* Protection for file_position  */
int           file_position;                /* Tracks what has been read     */

pthread_addr_t
threads_routine	(
	pthread_addr_t arg)
{
      char buf2[BUF_SIZE];
      int  c_index;
      int  pstatus;
      int  status;
      int  flags;
      int  mode;
      int  file_descr;
      int  position;
      char *path;
      int  i;


      path = "/tmp/cma_junk";
      flags = O_CREAT | O_RDWR;
      mode = 0400 | 0200 | 0100 | 0070 | 0007;   /* Give everyone access */


      /* Open the file */

      status = open(path, flags, mode);
      if (status == -1) {
	perror ("Open failed");
	return;
      }
      else
	file_descr = status;


      for (i = 1; i <= 2; i++) {	

	/* - lock the position counter on the file */
	pstatus = pthread_mutex_lock (&position_mutex);
	check (pstatus,"pthread_mutex_lock");
	position = file_position * BUF_SIZE;
	file_position++;
	if ((lseek(file_descr, position, SEEK_SET)) == -1) {
	  perror ("Lseek failed");
	  return;
	}
	pstatus = pthread_mutex_unlock (&position_mutex);
	check(pstatus,"pthread_mutex_unlock");


	if ((read(file_descr, buf2, BUF_SIZE)) == -1) {	  
	  perror ("Read failed");
	  return;
	}	

	c_index = buf2[0] - 97;     /* a to j */

	status = strcmp(&copy[c_index][0], &buf2[0]);
	if (status != 0) {
	  perror ("Data Corruption");
	  return;
	}
	else
	  copy[c_index][0] = 'z';      /* For parent to see later */
      }

      /* Close the file */

      if ((close(file_descr)) == -1) {
	perror("Close failed");
	return;
      }

}

main ()
    {
      pthread_t      thread_handles[NUM_THREADS];
      int exit_status;
      pthread_addr_t     result;
      int  status;
      int  pstatus;
      int  flags;
      int  mode;
      int  letter;
      int  file_descr;
      char buf1[BUF_SIZE];
      int  check_off[9];
      char *path;
      int  i,a,b,c,x,y;


      cts_test ("PUVB_NBI_006", "This tests U*ix file I/O calls.");  

      file_position = 0;

      status = pthread_mutex_init (&position_mutex, 
					pthread_mutexattr_default);
      check(pstatus,"pthread_mutex_init");

      /* Do some set up */

      path = "/tmp/cma_junk";
      flags = O_CREAT | O_RDWR;
      mode = 0400 | 0200 | 0100 | 0070 | 0007;   /* Give everyone access */


      /* Open the file */

      status = open(path, flags, mode);
      if (status == -1) {
	perror ("Open failed");
	return;
      }
      else
	file_descr = status;
      

      /* Fill the file, and the copy */

      for (letter = 'a'; letter <= 'j'; letter ++) {	
        for (i = 0; i < BUF_SIZE-2; i++)
	  buf1[i] = copy[letter-97][i] = letter;

	buf1[BUF_SIZE-2] = copy[letter-97][BUF_SIZE-2] = '\n';
	buf1[BUF_SIZE-1] = copy[letter-97][BUF_SIZE-1] = '\0';

	if ((write(file_descr, buf1, BUF_SIZE)) == -1) {	  
	  perror ("Write failed");
	  return;
	}
      }


      /* Return the descriptor to the beginning of the file */

      if ((lseek(file_descr, 0, SEEK_SET)) == -1) {
	perror("Lseek failed");
	return;
      }


      /* Create threads */

      for (x = 0; x < NUM_THREADS; x++)
	pstatus = pthread_create(&thread_handles[x],
			  pthread_attr_default,
			  threads_routine,
			  (pthread_addr_t)0);
	check(pstatus,"pthread_create");
      

      /* Join the last thread created */

      exit_status = pthread_join(thread_handles[NUM_THREADS-1],
		      &result);

      
      /* Check that the threads got all the data */

      for (i = 0; i <= 9; i++)
	if (copy[i][0] != 'z') {
	  perror("Data Check Error");
	  return;
	  }


      /* Close the file */

      if ((close(file_descr)) == -1) {
	perror("Close failed");
	return;
      }


      /* Remove the file from the directory */

      if ((unlink(path)) == -1) {
	    perror("Close failed");
	    return;
	    }
      
      cts_result ();  
      
    }
