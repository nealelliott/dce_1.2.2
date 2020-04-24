/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utc_shared_create.c,v $
 * Revision 1.1.10.2  1996/02/17  23:35:14  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:28  marty]
 *
 * Revision 1.1.10.1  1995/12/08  18:13:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/05  19:12 UTC  pare
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1995/01/05  19:09 UTC  pare
 * 	Replace cma calls with pthread calls.
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  18:44 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:25:20  root]
 * 
 * Revision 1.1.8.3  1994/08/29  21:45:36  cbrooks
 * 	CR11905 semaphore ops need SEM_UNDO
 * 	[1994/08/29  21:39:00  cbrooks]
 * 
 * Revision 1.1.8.2  1994/08/23  20:32:17  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:54  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:30:21  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  13:13 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.5.2  1993/07/08  20:02:24  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:01:47  root]
 * 
 * Revision 1.1.3.2  1993/06/02  18:29:53  truitt
 * 	CHFts07497: created a semaphore cleanup routine, to be called by
 * 	the dtsd cleanup routine to deallocate the semaphore identifiers
 * 	used by dtsd upon configuration.
 * 	[1993/06/02  18:29:03  truitt]
 * 
 * Revision 1.1.1.10  1993/05/04  22:39:31  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.8.1  1994/05/12  15:17:50  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:11:06  rhw]
 * 
 * Revision 1.1.5.3  1992/12/30  16:37:24  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:19:53  htf]
 * 
 * Revision 1.1.5.2  1992/12/22  21:52:39  comuzzi
 * 	Removed use of SHM_DEST in shared memory create.
 * 	[1992/12/22  20:31:35  comuzzi]
 * 
 * Revision 1.1.3.2  1992/05/11  13:36:52  comuzzi
 * 	Removed usage of bzero, replace with memset.
 * 	[1992/05/07  21:14:55  comuzzi]
 * 
 * Revision 1.1  1992/01/19  15:35:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module dtss_shared.c
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
 * ABSTRACT:	This module implements shared memory primatives create and
 *		delete for the no-kernel-mods version of DTS.
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
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <utc_shared.h>

extern SharedState *utc_sharedptr;


/*
 * Create shared memory region and map it.
 */
int utc_sharedcreate(void)
{
  key_t key;
  int shmemid;
  int shmemflag;
  int writelkid;
  int readlkid;
  time_t timestamp;
  FILE *fp;
  char	shared_memory_filename[K_MAX_FILENAME+1];

  /*
   * Try and map existing shared memory
   */

  if (utc_sharedmap(0) == 0)
    return(0);

  /*
   * Create shared memory region
   */  

  for (key = INIT_KEY; ;++key ) {
    shmemid = shmget(key, sizeof(SharedState),
                     IPC_EXCL | IPC_CREAT | 0644 );
    if (shmemid != -1) break;
    if (errno != EEXIST) 	/* If error not 'key inuse' try again */
      return(-1);
  };
  
  /*
   * Lock it in memory
   */
/*  if (i = shmctl(shmemid, SHM_LOCK, 0))
    return(i); */

  /*
   * Create locks
   */

  for (key++; ;++key) {
     writelkid = semget(key, 1, IPC_EXCL | IPC_CREAT | 0644);
     if (writelkid != -1) break;
     if (errno != EEXIST)
       return(-1);
  };
  for (key++; ;++key) {
     readlkid = semget(key, 1, IPC_EXCL | IPC_CREAT | 0666);
     if (readlkid != -1) break;
     if (errno != EEXIST)
       return(-1);
  };

  /*
   * Shared memory has been created, Now map and load it.
   */

  shmemflag = 0;
  utc_sharedptr = utc_sharedmapid(shmemid, shmemflag);
  if (utc_sharedptr == (SharedState *)0)
    return(-1);

  /*
   * Obtain time as a unique ID and store in shared memory
   */

  timestamp = time((time_t *)NULL);

  /* Initialize header to nulls */
  memset((char *)utc_sharedptr, 0, sizeof(SharedState));

  /* Move fixed portion into shared memory */
  utc_sharedptr->version = SHARED_MEMORY_VERSION;
  utc_sharedptr->shmemid = shmemid;
  utc_sharedptr->timestamp = timestamp;
  utc_sharedptr->writelkid = writelkid;
  utc_sharedptr->readlkid = readlkid;
  utc_sharedptr->inaccinfinite = 1;
  
  /*
   * Now store in datafile
   */
  
  (void)strncpy(shared_memory_filename, dcelocal_path, K_MAX_FILENAME);
  (void)strncat(shared_memory_filename, SHARED_MEMORY_DATAFILE, K_MAX_FILENAME);
  fp = fopen(shared_memory_filename, "w");
  if (fp == NULL)
    return(-1);

  (void) fprintf(fp, "%d %d\n", shmemid, timestamp);
  fclose(fp);
  
  return(0);
}

/*
 * Detach and delete shared memory region
 */
void utc_shareddelete(void)
{
  int shmemid;

  shmemid = utc_sharedptr->shmemid;

  /* Detach shared memory */
  utc_sharedunmap();
  
  /* Delete shared memory */
  shmctl(shmemid, IPC_RMID, (struct shmid_ds *)NULL);
}

/*
 * Write lock shared memory
 */
void utc_writelock(void)
{
   struct sembuf writeops[1];
   struct sembuf readops[1];

#ifdef _DECTHREADS_
   /*
    * Lockout other CMA threads, this is not necessary if
    * semop, etc. thread-synchronous.
    */
   pthread_lock_global_np();
#endif /* _DECTHREADS_ */

   /* Indicate desire to write */
   writeops[0].sem_num = 0;
   writeops[0].sem_op = 1;
   writeops[0].sem_flg = SEM_UNDO;
   semop(utc_sharedptr->writelkid, writeops, 1);

   /* Wait until no readers */
   readops[0].sem_num = 0;
   readops[0].sem_op = 0;
   readops[0].sem_flg = 0;
   semop(utc_sharedptr->readlkid, readops, 1);
}

/*
 * Write unlock shared memory
 */
void utc_writeunlock(void)
{
   struct sembuf writeops[1];

   /* Indicate done writing */
   writeops[0].sem_num = 0;
   writeops[0].sem_op = -1;
   writeops[0].sem_flg = SEM_UNDO;
   semop(utc_sharedptr->writelkid, writeops, 1);

#ifdef _DECTHREADS_
   /*
    * unlock other CMA threads.
    */
   pthread_unlock_global_np();
#endif /* _DECTHREADS_ */

}
