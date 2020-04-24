/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: calock.c,v $
 * Revision 1.1.11.2  1996/02/18  19:24:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:13  marty]
 *
 * Revision 1.1.11.1  1995/12/08  00:10:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:39  root]
 * 
 * Revision 1.1.7.7  1994/08/16  17:27:15  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Undo the damage caused by CA_lock_depth[].
 * 	Replace CALockGlobal() by CALockData(),
 * 	CAUnlockGlobal() by CAUnlockData(),
 * 	CALockClient() by CALockData(),
 * 	CAUnlockClient() by CAUnlockData().
 * 	[1994/08/16  15:30:31  peckham]
 * 
 * Revision 1.1.7.6  1994/07/13  15:23:45  peckham
 * 	Collapse CA_GLOBAL CA_CLIENT CA_LIST_LOCK to one lock.
 * 	For cleanup later: Since this code has been running for
 * 	a long time, I am assuming there is always an unlock
 * 	for every lock. To get this fix out quickly I allow
 * 	lock nesting on the collapsed locks. This is tracked
 * 	by CA_lock_depth[] so locking only happens on the first call.
 * 	[1994/07/11  18:29:04  peckham]
 * 
 * Revision 1.1.7.5  1994/06/23  18:28:29  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:44:52  mccann]
 * 
 * Revision 1.1.7.4  1994/06/09  16:07:13  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:48  devsrc]
 * 
 * Revision 1.1.7.3  1994/06/07  16:32:47  peckham
 * 	OT#10550: fix CACHE_SEM_END_LOCK().
 * 	[1994/05/31  18:52:03  peckham]
 * 
 * Revision 1.1.7.2  1994/04/14  14:38:04  peckham
 * 	Remove _POSIX_THREADS conditionals.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Use CA_snapshot_cache() instead of CA_abort().
 * 	[1994/04/11  17:47:29  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  21:55:56  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:48:19  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  12:37:12  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:20:35  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  18:28:24  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:09:18  weisman]
 * 
 * Revision 1.1.2.4  1992/07/02  20:12:33  weisman
 * 	In the routine cache_init, there was the incorrect statement:
 * 
 * 	      CA_cancel_state[SEM_CNT] = CA_CANCEL_NONE;
 * 
 * 	It was changed to:
 * 
 * 	      CA_cancel_state[i] = CA_CANCEL_NONE;
 * 	[1992/07/02  20:12:11  weisman]
 * 
 * Revision 1.1.2.3  1992/06/30  19:29:51  peckham
 * 	OT#936 - make cache cancel-safe by disabling cancellability
 * 	during cache ops.
 * 	[1992/06/30  19:29:30  peckham]
 * 
 * Revision 1.1.2.2  1992/03/22  21:14:45  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:05:54  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: calock.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */
#include <cache.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>

#include <errno.h>
#include <string.h>

CA_PRIVATE pthread_once_t CA_once = pthread_once_init;
CA_PRIVATE pthread_mutex_t CA_mutex;
CA_PRIVATE pthread_mutex_t CA_lock_mutex[SEM_CNT];  
CA_PRIVATE int CA_cancel_state[SEM_CNT];

#if ((CANCEL_OFF != 0) && (CANCEL_ON != 0))
# define CA_CANCEL_NONE 0
#else
# if ((CANCEL_OFF != -1) && (CANCEL_ON != -1))
#  define CA_CANCEL_NONE -1
# else
#  define CA_CANCEL_NONE 1
# endif
#endif

#define CACHE_INIT		{ (void)pthread_once(&CA_once, CA_once_init); }
#define CACHE_LOCK		{ (void)pthread_mutex_lock(&CA_mutex); }
#define CACHE_END_LOCK		{ (void)pthread_mutex_unlock(&CA_mutex); }
#define CACHE_SEM_LOCK(n) \
	{ (void)pthread_mutex_lock(&CA_lock_mutex[n]); \
	  CA_cancel_state[n] = pthread_setcancel(CANCEL_OFF); }
#define CACHE_SEM_END_LOCK(n) \
	{ const int old_state = CA_cancel_state[n]; \
	  CA_cancel_state[n] = CA_CANCEL_NONE; \
	  (void)pthread_mutex_unlock(&CA_lock_mutex[n]); \
	  (void)pthread_setcancel(old_state); }

/*
 * Initialize this module's statics
 */
CA_PRIVATE void 
CA_once_init (void)
{
    int i;

    (void)pthread_mutex_init(&CA_mutex, pthread_mutexattr_default);

    /* Get mutexes to protect locks from threads within this process */
    for (i=0; i<SEM_CNT; ++i) 
    {
      (void)pthread_mutex_init(&CA_lock_mutex[i], pthread_mutexattr_default);
      CA_cancel_state[i] = CA_CANCEL_NONE;
    }
  }

CA_PRIVATE int CA_blockCount = 0;
CA_PRIVATE sigset_t CA_blockSet;
CA_PRIVATE sigset_t CA_oldSet;

CA_PRIVATE int CA_setup_done;
CA_PRIVATE struct sembuf CA_sops[SEM_CNT];
#define CA_SEM_LOCKED	-1
#define CA_SEM_UNLOCKED	1

/* 
 * Locking is implemented with an Ultrix semaphores consisting
 * of 4 values.  Semphores are operated on atomically.  
 * For performance reasons only exclusive locks to each cache
 * is allowed.
 */

/*
 * Defunct routine
 */
void 
CASetUp (void)
{
  CACHE_INIT;

  CACHE_LOCK 
  {
    if (!CA_setup_done) 
    {
      register int i;

      (void)sigemptyset(&CA_blockSet);
      /* signals to stop while modifying the cache */
      (void)sigaddset(&CA_blockSet, SIGHUP);
      (void)sigaddset(&CA_blockSet, SIGINT);
      (void)sigaddset(&CA_blockSet, SIGTERM);

      for (i = 0; i < SEM_CNT; i++) 
      {
	CA_sops[i].sem_num = i;
	CA_sops[i].sem_op = CA_SEM_UNLOCKED;
	CA_sops[i].sem_flg = SEM_UNDO;
      }
      CA_setup_done = TRUE;
    }
  } CACHE_END_LOCK;
}

/*
 * Cache processing must be treated as a global section.
 * Block signals during cache processing.
 */
CA_PRIVATE void 
CA_Block (void)
{

  /* Get global mutex for access to block count */
  CACHE_LOCK 
  {

    /* If no other thread has blocked signals, do it now */
    if (!CA_blockCount++)
      (void)sigprocmask(SIG_BLOCK, &CA_blockSet, &CA_oldSet);

  } CACHE_END_LOCK;
}

CA_PRIVATE void 
CA_unBlock (void)
{
  /* Get global mutex for access to block count */
  CACHE_LOCK 
  {

    /* If last thread with lock, unblock signals */
    if (!--CA_blockCount)
      (void)sigprocmask(SIG_SETMASK, &CA_oldSet, (sigset_t *)0);

  } CACHE_END_LOCK;
}

/*
 * Initialize locks.  If we cannot get one, an error is logged but
 * we keep going.  
 */
dns_status_t 
CAInitLocks (void)
{
  DEB_ASCII_BUF_dns_status(statusBuf)
  ushort val[SEM_CNT];
  key_t CAnextid;		/* Next semaphore key */
  union semun {
      int val;
      struct semid_ds *buf;
      ushort *array;
  } semun_arg;
  register int i;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CAInitLocks()"));

  /* Get semid that is not being used */
  for (CAnextid = INIT_ID;;++CAnextid) 
  {
    if (0 <= (CAfixed_p->ca_semid = 
		semget(CAnextid, SEM_CNT, IPC_EXCL | IPC_CREAT | 0700)))
    {
	break;
    }
    /* Got an error, if key already exists try another */
    if (errno != EEXIST) 
    {
	/* Need to log an event here */
          DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_cache,
              svc_c_debug8,
              " <CAInitLocks() return(%s)",
              deb_ascii_dns_status(statusBuf,DNS_NOTFND)));
    	return(DNS_NOTFND);
  	}
    }
  
  /* 
   * Initialize semaphore values.  
   * All locks are exclusive.
   * You can write lock the entire cache for exclusive access,
   * lock the cache header and FreeList,
   * or lock the client or global cache. 
   * The lock on the cache header is used by uid_new to save
   * the last uid generated. It is an exclusive lock.
   * The list lock is for access to the free list and space counters and 
   * is implemented as an exclusive lock since all accesses are
   * with the intent to update the lists.  The list lock is necessary to
   * control access to the resource shared by the client and
   * global caches.  
   */
  for (i = 0; i < SEM_CNT; i++)
    val[i] = CA_SEM_UNLOCKED;

  semun_arg.array = val;

  if (semctl(CAfixed_p->ca_semid, SEM_CNT, SETALL, semun_arg) != -1) 
  {
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug8,
        " <CAInitLocks() return(%s)",
        deb_ascii_dns_status(statusBuf,DNS_SUCCESS)));
    return(DNS_SUCCESS);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CAInitLocks() return(%s)",
      deb_ascii_dns_status(statusBuf,DNS_NOTFND)));
  return(DNS_NOTFND);
}

/* 
 * Release locking semaphores
 */
void 
CACleanupLocks (void)
{
  if (0 <= CAfixed_p->ca_semid) 
  {
    (void)semctl(CAfixed_p->ca_semid, SEM_CNT, IPC_RMID, 0);
    CAfixed_p->ca_semid = -1;
  }
}

/*
 *  Obtain a write lock on the entire cache.
 *  Write locks allow exclusive access.
 */
void 
CAWriteLockCache (void)
{
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CAWriteLockCache()"));

  /* Order is important.  Lock global first */
  CALockData();
  CAHeaderLock();

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CAWriteLockCache()"));
}

/*
 * Release the write lock on the entire cache. 
 */
void 
CAWriteUnlockCache (void)
{

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CAWriteUnlockCache()"));

  CAUnlockData();
  CAHeaderUnlock();

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CAWriteUnlockCache()"));
}

/*
 * Obtain a write lock on the global cache.
 * Write locks allow exclusive access.
 */
void 
CALock (int num)
{
  struct sembuf *const sop_p = &CA_sops[num];

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CALock(%d)",
      num));

  CACHE_INIT;

  if (!CA_setup_done) 
  {
    CASetUp();
  }

  /* Protect threads from each other.  
   * Semaphores only help between processes 
   */
  CACHE_SEM_LOCK(num);

#ifdef CACHE_PARANOID
  if (sop_p->sem_op == CA_SEM_UNLOCKED) 
  {
#endif

    sop_p->sem_op = CA_SEM_LOCKED;

    do 
    {
      if (0 <= semop(CAfixed_p->ca_semid, sop_p, 1)) 
      {
	/* Block signals while manipulating cache */
	CA_Block();
	goto leave_CALock;
      }
    } while (errno == EINTR);

    dce_svc_printf(CDS_S_CACHE_SEMOP_MSG, strerror(errno));
    sop_p->sem_op = CA_SEM_UNLOCKED;

#ifdef CACHE_PARANOID
  } 
  else
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_ISLOCKED_A_MSG);
  }
#endif

  CACHE_SEM_END_LOCK(num);

leave_CALock:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CALock(%d)",
      num));
}

/*
 * Release the lock on a cache. 
 */
void 
CAUnlock (int num)
{
  struct sembuf *const sop_p = &CA_sops[num];

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CAUnlock(%d)",
      num));

#ifdef CACHE_PARANOID
  if (sop_p->sem_op != CA_SEM_LOCKED) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_NOTLOCKED_A_MSG);
  }
#endif

  /* Allow signals */
  CA_unBlock();

  /* Release lock by setting value back to 1 */
  sop_p->sem_op = CA_SEM_UNLOCKED;

  while (semop(CAfixed_p->ca_semid, sop_p, 1) < 0)
    if (errno != EINTR) 
    {
      dce_svc_printf(CDS_S_CACHE_SEMOP_MSG, strerror(errno));
      break;
    }

  /* Release semaphore for other threads */
  CACHE_SEM_END_LOCK(num);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CAUnlock(%d)",
      num));
}

#ifdef CACHE_PARANOID
/*
 * CA_AtomicLock
 *
 *	Set a flag with no contention. The use of a mutex
 *	is to guarantee a syncronization point.
 */
void 
CA_AtomicLock (int num)
{
  CACHE_LOCK 
  {
    CAfixed_p->ca_lock |= (1<<num);
  } CACHE_END_LOCK;
}
/*
 * CA_AtomicUnlock
 *
 *	Clear a flag with no contention. The use of a mutex
 *	is to guarantee a syncronization point.
 */
void 
CA_AtomicUnlock (int num)
{
  CACHE_LOCK 
  {
    CAfixed_p->ca_lock &= ~(1<<num);
  } CACHE_END_LOCK;
}
/*
 * CA_Locked
 *
 *	This routine is called for debugging purposes. It will check
 *	if the cache is currently locked by this process.
 */
int 
CA_Locked (int num)
{
    return (CA_blockCount && (CA_sops[num].sem_op == CA_SEM_LOCKED));
}
#endif /* CACHE_PARANOID */
