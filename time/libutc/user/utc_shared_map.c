/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utc_shared_map.c,v $
 * Revision 1.1.10.2  1996/02/17  23:35:15  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:29  marty]
 *
 * Revision 1.1.10.1  1995/12/08  18:13:07  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/05  19:15 UTC  pare
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1995/01/05  19:13 UTC  pare
 * 	Replace cma calls with pthread calls.
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  18:51 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:25:21  root]
 * 
 * Revision 1.1.8.3  1994/08/29  21:45:37  cbrooks
 * 	CR11905 semaphore ops need SEM_UNDO
 * 	[1994/08/29  21:39:02  cbrooks]
 * 
 * Revision 1.1.8.2  1994/08/23  20:32:18  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:56  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:30:22  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  13:16 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.8.1  1994/05/12  15:17:51  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:11:07  rhw]
 * 
 * 	HP revision /main/HPDCE01/2  1994/04/26  14:22 UTC  pare
 * 	merge fix for defects CHFts 11560 and 11753
 * 
 * 	HP revision /main/HPDCE01/pare_rodan/1  1994/04/26  14:15 UTC  pare
 * 	Fix for CHFts11753 and CHFts11050
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/31  20:10  pare
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.3.3  1993/09/28  14:23:33  pare
 * 	Removed OT8108 Fix -- this was causing a problem when trying to
 * 	restart DCE once dtsd was started and stopped. cdsd would hang
 * 	in utc_gettime waiting on the lock from utc_readlock.
 * 	See defect CHFts08311.
 * 	[1993/09/28  14:22:41  pare]
 * 
 * Revision 1.1.3.2  1993/07/15  17:30:16  truitt
 * 	OT8108: Use the correct semaphore number to fix the locking in
 * 	        utc_readlock().  Need to correctly get the value of this
 * 	        semaphore.  Put the cma global lock inside the loop, so
 * 	        if it gets unlocked, it will get locked again.
 * 	[1993/07/15  17:28:58  truitt]
 * 
 * Revision 1.1.1.10  1993/05/04  22:39:33  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.3.2  1992/12/30  16:37:32  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:19:58  htf]
 * 
 * Revision 1.1  1992/01/19  15:35:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module dtss_shared_map.c
 *	Version X0.6
 */
/*
 * Copyright (c) 1990 by
 * Digital Equipment Corporation, Maynard, Mass.
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:	This module implements shared memory primatives for the
 *		no-kernel-mods version of DTS.
 *
 * ENVIRONMENT:	Ultrix V4.0
 *
 * AUTHORS:	Joe Comuzzi
 *		Distributed Systems Services (DSS) 
 * 
 * DATE:	August 27, 1990
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <utc_shared.h>
#include <unistd.h>


SharedState *utc_sharedptr;


/*
 * Detach shared memory, zero shared state
 */
void utc_sharedunmap(void)
{
  /* Make sure pointer is valid before proceeding */
  if (utc_sharedptr != (SharedState *)NULL) {
    shmdt((char *)utc_sharedptr);	/* Detach region */
    utc_sharedptr = (SharedState *)NULL;
  };
}

/* 
 * Map shared memory region and return address in utc_sharedptr
 */
int utc_sharedmap( int flag )
{
  FILE *fp;
  time_t timestamp;
  int shmemid;
  char	shared_memory_filename[K_MAX_FILENAME+1];

  if (utc_sharedptr != (SharedState *)NULL)
    return(0);

  /*
   * Open the file which has the shared memory id and
   * timestamp in it and read them in. If we can't
   * treat it like the file didn't exist.
   */

  (void)strncpy(shared_memory_filename, dcelocal_path, K_MAX_FILENAME);
  (void)strncat(shared_memory_filename, SHARED_MEMORY_DATAFILE, K_MAX_FILENAME);
  fp = fopen(shared_memory_filename, "r");
  if (fp == NULL)
    return(-1);

  if(fscanf(fp, "%d %d", &shmemid, &timestamp) != 2) {
    fclose(fp);
    return(-1);
  }

  fclose(fp);
  
  utc_sharedptr = utc_sharedmapid(shmemid, flag);
  if(utc_sharedptr == (SharedState *)0)
    return(-1);

  if (utc_sharedptr->version != SHARED_MEMORY_VERSION ||
      utc_sharedptr->shmemid != shmemid ||
      utc_sharedptr->timestamp != timestamp) {
    utc_sharedunmap();
    return(-1);
  };
  return(0);
}

/*
 * Map shared memory region with id shmemid.
 */
SharedState *utc_sharedmapid(int shmemid, int flag)
{
  char *mappoint; 
  SharedState *result;

  mappoint = (char *)0;

  result = (SharedState *)shmat(shmemid, mappoint, flag | SHM_RND);
  if (result == (SharedState *)-1) {
    return((SharedState *)0);
  };

  return(result);
}

/*
 * Write lock shared memory
 */
void utc_readlock(void)
{
   struct sembuf writeops[1];
   struct sembuf readops[1];
   int write_in_progress = 0;

   /* remove OT8108 fix */
   /* do { */

#ifdef _DECTHREADS_
	/*
	 * Lockout other CMA threads. This is only necessary if
	 * semop, etc. is not thread-synchronous.
	 */
	pthread_lock_global_np();
#endif /* _DECTHREADS_ */
   do {     
	/* Wait until no one is writing */

	writeops[0].sem_num = 0;
	writeops[0].sem_op = 0;
	writeops[0].sem_flg = 0;
	semop(utc_sharedptr->writelkid, writeops, 1);

	/* Declare intention to read */

	readops[0].sem_num = 0;
	readops[0].sem_op = 1;
	readops[0].sem_flg = SEM_UNDO;
	semop(utc_sharedptr->readlkid, readops, 1);

	/* Check if anyone started writing */


	/* remove OT8108 fix */
        /*write_in_progress = semctl(utc_sharedptr->writelkid, 0, GETVAL); */
        semctl(utc_sharedptr->writelkid, 1, GETVAL, &write_in_progress);

	if (write_in_progress)
	    utc_readunlock();

   } while(write_in_progress);
}

/*
 * Read unlock shared memory
 */
void utc_readunlock(void)
{
   struct sembuf readops[1];

   /* Indicate done reading */

   readops[0].sem_num = 0;
   readops[0].sem_op = -1;
   readops[0].sem_flg = SEM_UNDO;
   semop(utc_sharedptr->readlkid, readops, 1);

#ifdef _DECTHREADS_
   /*
    * unlock other threads
    */
   pthread_unlock_global_np();
#endif /* _DECTHREADS_ */
}
