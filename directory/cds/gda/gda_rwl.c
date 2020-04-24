/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_rwl.c,v $
 * Revision 1.1.6.2  1996/02/18  19:31:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:28  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:13:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:35  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:01:43  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:53:36  mccann]
 * 
 * Revision 1.1.4.3  1994/06/23  18:30:08  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:28:10  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:38:10  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:24  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:01:46  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:09:52  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:27:32  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:44:26  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:24:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
 */

#include <gda.h>
#include <gda_common.h>
#include <gda_rwl.h>
#include <gda_main.h>
#include <errno.h>


/* Initialize and abort on error */
void
rwl_init (rwl_t *rwl)
{
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " >rwl_init"));

  if (pthread_mutex_init(&rwl->mutex, pthread_mutexattr_default))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)errno);
      }

  if (pthread_cond_init(&rwl->readers, pthread_condattr_default))
      {
	dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)errno);
      }

  if (pthread_cond_init(&rwl->writers, pthread_condattr_default))
      {
	dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)errno);
      }

  if (pthread_cond_init(&rwl->readers_to_writers, pthread_condattr_default))
      {
	dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)errno);
      }
  
  rwl->writing = 0;
  rwl->num_reading = 0;
  rwl->reader_to_writer_waiting = 0;
  rwl->num_readers_waiting = 0;
  rwl->num_writers_waiting = 0;
  rwl->num_writers_waiting_phase = 0;
  rwl->deleted = 0;
  
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <rwl_init"));
}	

/* before this routine is called, access to the rwl must be halted. i.e.
	noone should be allowed to call any of the rwl functions with this
	lock as a parameter again.  The lock must be write locked before
	a call to this procedure.
*/
void
rwl_free (rwl_t *rwl)
{
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " >rwl_free"));

  if (pthread_mutex_lock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)errno);
      }

  rwl->deleted = 1;
  rwl->writing = 0;

  if (rwl->num_readers_waiting)
      {
	if (pthread_cond_broadcast(&rwl->readers))
	    {
	      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)errno);
	    }
      }
  
  if (rwl->num_writers_waiting)
      {
	if (pthread_cond_broadcast(&rwl->writers))
	    {

	      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)errno);
	    }
      }

  for (;rwl->num_readers_waiting > 0 || rwl->num_writers_waiting > 0;)
      {
	if (pthread_cond_wait(&rwl->readers, &rwl->mutex))
	    {
              dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)errno);
	    }
      }

  if (pthread_cond_destroy(&rwl->readers))
      {
	dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)errno);
      }

  if (pthread_cond_destroy(&rwl->writers))
      {
	dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)errno);
      }

  if (pthread_cond_destroy(&rwl->readers_to_writers))
      {
	dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)errno);
      }

  if (pthread_mutex_unlock(&rwl->mutex))
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)errno);

  if (pthread_mutex_destroy(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)errno);
      }

  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <rwl_free"));

}	

/* 
 *read_lock
 * read access to list.  Abort on pthread errors. 
 */
int 
read_lock (rwl_t *rwl)
{
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " >read_lock"));

  if (pthread_mutex_lock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)errno);
      }

  while (rwl->writing > 0)
      {
	rwl->num_readers_waiting++;

	if (pthread_cond_wait(&rwl->readers, &rwl->mutex))
	    {
	      dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)errno);
	    }
	rwl->num_readers_waiting--;

	if (rwl->deleted)
	    {
	      /* 
	       * signal rwl_free that we have given up the lock
	       * attempt 
	       */
	      if (pthread_cond_signal(&rwl->readers))
		  {
		    dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)errno);
		  }

	      if (pthread_mutex_unlock(&rwl->mutex))
		  {
		    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, 
				   (long)errno);
		  }
 
	      DCE_SVC_DEBUG((
			   cds__svc_handle,
			   cds_svc_gda,
			   svc_c_debug8,
			   " <read_lock: return(READ_LOCK_DELETED)"));

	      return(READ_LOCK_DELETED);
	    }

      }
  rwl->num_reading++;

  if (pthread_mutex_unlock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)errno);
      }

  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <read_lock"));

  return(OK);
}

/*
 * write_lock
 *   lock for write access.  Abort on pthread errors.
 */
int 
write_lock (rwl_t *rwl)
{
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " >write_lock"));

  if (pthread_mutex_lock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)errno);
      }

  while (rwl->num_reading > 0 || rwl->writing > 0)
      {
	rwl->num_writers_waiting++;
	
	if (pthread_cond_wait(&rwl->writers, &rwl->mutex))
	    {
	      dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)errno);
	    }

	rwl->num_writers_waiting--;
	rwl->num_writers_waiting_phase--;
	
	if (rwl->deleted)
	    {
	      /* 
	       * signal rwl_free that we have given up the lock
	       * attempt 
	       */
	      if (pthread_cond_signal(&rwl->readers))
		  {
		    dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)errno);
		  }
	      
	      if (pthread_mutex_unlock(&rwl->mutex))
		  {
		    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, 
				   (long)errno);
		  }

	      DCE_SVC_DEBUG((
			  cds__svc_handle,
			  cds_svc_gda,
		          svc_c_debug2,
		          " <write_lock: return(WRITE_LOCK_DELETED)"));

	      return(WRITE_LOCK_DELETED);
	    }
      }
  rwl->writing++;
  
  if (pthread_mutex_unlock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)errno);
      }

  DCE_SVC_DEBUG((
		 cds__svc_handle,
		 cds_svc_gda,
		 svc_c_debug8,
		 " <write_lock"));
  
  return(OK);
}

int 
read_to_write_lock (rwl_t *rwl)
{
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " >read_to_write_lock"));

  if (pthread_mutex_lock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)errno);
      }

  if (rwl->reader_to_writer_waiting == 1)
      {
	DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
	            svc_c_debug2,
	            "read_to_write_lock: upgrade blocked"));

	if (pthread_mutex_unlock(&rwl->mutex))
	    {
              dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)errno);
	    }
	return(READ_TO_WRITE_LOCK_BLOCKED);
      }

  while(rwl->num_reading > 1)
      {
	rwl->reader_to_writer_waiting++;

	if (pthread_cond_wait(&rwl->readers_to_writers, &rwl->mutex))
	    {
	      dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)errno);
	    }
	rwl->reader_to_writer_waiting--;
      }

  rwl->num_reading--;
  rwl->writing++;

  if (pthread_mutex_unlock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)errno);
      }

  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <read_to_write_lock"));

  return(OK);
}

int 
write_to_read_lock (rwl_t *rwl)
{
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " >write_to_read_lock"));

  if (pthread_mutex_lock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)errno);
      }

  rwl->num_reading++;
  rwl->writing--;

  if(rwl->num_readers_waiting)
      {
	if(pthread_cond_broadcast(&rwl->readers))
	    {
	      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)errno);
	    }
      }

  if (pthread_mutex_unlock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)errno);
      }

  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <write_to_read_lock"));

  return(0);
}

void 
read_unlock (rwl_t *rwl)
{
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " >read_unlock"));

  if (pthread_mutex_lock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)errno);
      }

  rwl->num_reading--;
  if(rwl->num_reading == 1 && rwl->reader_to_writer_waiting == 1)
      {
	rwl->num_writers_waiting_phase = rwl->num_writers_waiting + 1;

	if (pthread_cond_signal(&rwl->readers_to_writers))
	    {
	      /* Perhaps the writers should be signaled here.
		 For now, just report the error. */
	      rwl->num_writers_waiting_phase = 0;

	      dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)errno);
	    }
      }
  else if (rwl->num_reading == 0 && rwl->num_writers_waiting > 0)
      {
	rwl->num_writers_waiting_phase = rwl->num_writers_waiting;

	if (pthread_cond_signal(&rwl->writers))
	    {
	      dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)errno);
	    }
      }
  else if(rwl->num_readers_waiting > 0 &&	rwl->num_writers_waiting == 0)
      if (pthread_cond_broadcast(&rwl->readers))
	  {
	    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)errno);
	  }
  

  if (pthread_mutex_unlock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)errno);
      }

  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <read_unlock"));
}

void
write_unlock (rwl_t *rwl)
{
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " >write_unlock"));

  if (pthread_mutex_lock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)errno);
      }

  rwl->writing--;
  if (rwl->num_writers_waiting_phase > 0)
      {
	if (pthread_cond_signal(&rwl->writers))
	    {
	      dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)errno);
	    }
      }
  else if (rwl->num_readers_waiting)
      {
	if (pthread_cond_broadcast(&rwl->readers))
	    {
	      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)errno);
	    }
      }
  else if (rwl->num_writers_waiting)
      if (pthread_cond_signal(&rwl->writers))
	  {
	    dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)errno);
	  }

  if (pthread_mutex_unlock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)errno);
      }

  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <write_unlock"));

}

int 
read_trylock (rwl_t *rwl)
{
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " >read_trylock"));

  if (pthread_mutex_trylock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXTRYLOCK_MSG, (long)errno);
	DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
	        svc_c_debug2,
	        " <read_trylock: return(READ_TRYLOCK_FAIL)"));
	    
	return(READ_TRYLOCK_FAIL);
      }
	

  if (rwl->writing > 0)
      {
	if (pthread_mutex_unlock(&rwl->mutex))
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)errno);
	    }
	return(READ_TRYLOCK_FAIL);		
      }

  rwl->num_reading++;

  if (pthread_mutex_unlock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)errno);
      }  

  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <read_trylock"));

  return(OK);
}

int 
write_trylock (rwl_t *rwl)
{
  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " >write_trylock"));

  if (pthread_mutex_trylock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXTRYLOCK_MSG, (long)errno);
	DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
	        svc_c_debug2,
	        " <write_trylock: return(WRITE_TRYLOCK_FAIL)"));

	return(WRITE_TRYLOCK_FAIL);
      }
	
  if (rwl->num_reading > 0 || rwl->writing > 0)
      {
	if (pthread_mutex_unlock(&rwl->mutex))
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)errno);
	    }
	return(WRITE_TRYLOCK_FAIL);
      }
  rwl->writing++;

  if (pthread_mutex_unlock(&rwl->mutex))
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)errno);
      }

  DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug8,
            " <write_trylock"));

  return(OK);
}










