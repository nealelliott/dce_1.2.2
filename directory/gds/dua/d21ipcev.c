/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21ipcev.c,v $
 * Revision 1.1.6.2  1996/02/18  18:17:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:41  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:54:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:10:04  root]
 * 
 * Revision 1.1.4.6  1994/09/14  15:44:58  keutel
 * 	OT 11875: pthread_yield call in d21_evdispatcher added
 * 	[1994/09/14  15:09:35  keutel]
 * 
 * Revision 1.1.4.5  1994/08/17  13:56:42  keutel
 * 	OT 11222: adding CDS class and CDS class version via XDS
 * 	[1994/08/17  12:49:29  keutel]
 * 
 * Revision 1.1.4.4  1994/07/06  15:07:18  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:15:25  marrek]
 * 
 * Revision 1.1.4.3  1994/06/21  14:46:42  marrek
 * 	June 1994 code submission.
 * 	[1994/06/21  09:26:24  marrek]
 * 
 * Revision 1.1.4.2  1994/05/10  15:53:56  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:32:10  marrek]
 * 
 * Revision 1.1.4.1  1994/02/22  18:05:59  marrek
 * 	Changes for libdce.
 * 	[1994/02/10  17:04:31  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:32:51  marrek]
 * 
 * Revision 1.1.2.3  1993/08/11  20:49:44  mhickey
 * 	Added OSF1 to the platforms on which not to
 * 	include the sys/select.h file. Also cleaned
 * 	up the ifdef style to conform to our guidelines.
 * 
 * 	defect 8433.
 * 	[1993/08/11  20:39:03  mhickey]
 * 
 * Revision 1.1.2.2  1993/08/11  06:17:57  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:55:24  marrek]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21ipcev.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 18:17:01 $";
#endif

#include <gds.h>
#if defined(THREADSAFE)
#  include <pthread.h>
#  include <errno.h>
#  if defined(_AIX)
#    include <sys/select.h>
#  endif
#endif /* THREADSAFE */
#include <stdio.h>

#include <d21dua.h>

extern d21_ipc_info d21_ipc_tab[] ;

#ifdef THREADSAFE
static D23_ipcempb 	d21_empb ;
static pthread_t	d21_disp_handle ;
static Bool end_of_process = FALSE;
static Bool select_left = FALSE;
static pthread_cond_t detach_cond_handle;
static pthread_mutex_t detach_mutex_handle;
#endif

/**** LOCAL FUNCTIONS *****/

#if defined(THREADSAFE)
pthread_startroutine_t d21_evdispatcher ( pthread_addr_t );
static signed32 d21_get_ipc_event ( D21_abandon_info * ,
					signed16 , d21_ipc_info **  );
#endif

/* -------------------------------------------------------------------------- */

d2_ret_val d21_create_event_dispatcher(
  D2_pbhead * pbhead)
{
#ifdef THREADSAFE
    signed16		i ;
    d21_ipc_info	* ipcref ;

    /* get IPC-event fd's */
    d21_empb.d23_Ehdinfo.d2_version = D23_V02 ;
    if (d23_gipc_eminfo (&d21_empb) == D23_ERROR) {
	*pbhead = d21_empb.d23_Ehdinfo ;
	return (D2_ERROR) ;
    }

    /* create mutex and conditional variable for detaching */
    if (pthread_mutex_init (&detach_mutex_handle,pthread_mutexattr_default) < 0)
	{
	pbhead->d2_retcode = D2_TEMP_ERR ;
	pbhead->d2_errclass = D2_INT_ERR ;
	pbhead->d2_errvalue = errno ;
	return (D2_ERROR) ;
	}
    if (pthread_cond_init (&detach_cond_handle, pthread_condattr_default) < 0)
	{
	pbhead->d2_retcode = D2_TEMP_ERR ;
	pbhead->d2_errclass = D2_INT_ERR ;
	pbhead->d2_errvalue = errno ;
	return (D2_ERROR) ;
	}

    /* create IPC-association specific mutexes and conditional variables */
    for (i = 0, ipcref = d21_ipc_tab; i < D23_MXCLTPROC; i++, ipcref++) {
	if (pthread_mutex_init (&ipcref->d21_ipcev_mutex,
					      pthread_mutexattr_default) < 0) {
	    pbhead->d2_retcode = D2_TEMP_ERR ;
	    pbhead->d2_errclass = D2_INT_ERR ;
	    pbhead->d2_errvalue = errno ;
	    return (D2_ERROR) ;
	}
	if (pthread_cond_init (&ipcref->d21_ipcev_condvar,
						pthread_condattr_default) < 0) {
	    pbhead->d2_retcode = D2_TEMP_ERR ;
	    pbhead->d2_errclass = D2_INT_ERR ;
	    pbhead->d2_errvalue = errno ;
	    return (D2_ERROR) ;
	}
    }

    /* create IPC-event dispatcher thread */
    if (pthread_create (&d21_disp_handle, pthread_attr_default,
	(pthread_startroutine_t) d21_evdispatcher, (pthread_addr_t) NULL) < 0) {
	pbhead->d2_retcode = D2_TEMP_ERR ;
	pbhead->d2_errclass = D2_INT_ERR ;
	pbhead->d2_errvalue = errno ;
	return (D2_ERROR) ;
    }
#endif
    return (D2_NOERROR) ;
}

/* -------------------------------------------------------------------------- */

d2_ret_val d21_remove_event_dispatcher(
  D2_pbhead * pbhead)
{
#ifdef THREADSAFE
    signed16		i ;
    d21_ipc_info	* ipcref ;
    char attbyte=0;

    end_of_process = TRUE;
    for (i = 0; i < d21_empb.d23_Enfds; i++)
	    (void)write(*(d21_empb.d23_Eipcfds + i), &attbyte, 1) ;

    if (pthread_mutex_lock(&detach_mutex_handle) < 0)
	{
	pbhead->d2_retcode = D2_TEMP_ERR ;
	pbhead->d2_errclass = D2_INT_ERR ;
	pbhead->d2_errvalue = errno ;
	return (D2_ERROR) ;
	}

    while (select_left == FALSE)
      if (pthread_cond_wait(&detach_cond_handle,&detach_mutex_handle) < 0)
	{
	pbhead->d2_retcode = D2_TEMP_ERR ;
	pbhead->d2_errclass = D2_INT_ERR ;
	pbhead->d2_errvalue = errno ;
        pthread_mutex_unlock(&detach_mutex_handle);
	return (D2_ERROR) ;
	}

    if (pthread_mutex_unlock(&detach_mutex_handle) < 0)
	{
	pbhead->d2_retcode = D2_TEMP_ERR ;
	pbhead->d2_errclass = D2_INT_ERR ;
	pbhead->d2_errvalue = errno ;
	return (D2_ERROR) ;
	}

    /* destroy mutex and conditional variable for detaching */
    if (pthread_mutex_destroy (&detach_mutex_handle) < 0)
	{
	pbhead->d2_retcode = D2_TEMP_ERR ;
	pbhead->d2_errclass = D2_INT_ERR ;
	pbhead->d2_errvalue = errno ;
	return (D2_ERROR) ;
	}
    if (pthread_cond_destroy (&detach_cond_handle) < 0)
	{
	pbhead->d2_retcode = D2_TEMP_ERR ;
	pbhead->d2_errclass = D2_INT_ERR ;
	pbhead->d2_errvalue = errno ;
	return (D2_ERROR) ;
	}

    /* destroy IPC-association specific mutexes and conditional variables */
    for (i = 0, ipcref = d21_ipc_tab; i < D23_MXCLTPROC; i++, ipcref++) {
	if (pthread_mutex_destroy (&ipcref->d21_ipcev_mutex) < 0) {
	    pbhead->d2_retcode = D2_TEMP_ERR ;
	    pbhead->d2_errclass = D2_INT_ERR ;
	    pbhead->d2_errvalue = errno ;
	    return (D2_ERROR) ;
	}
	if (pthread_cond_destroy (&ipcref->d21_ipcev_condvar) < 0) {
	    pbhead->d2_retcode = D2_TEMP_ERR ;
	    pbhead->d2_errclass = D2_INT_ERR ;
	    pbhead->d2_errvalue = errno ;
	    return (D2_ERROR) ;
	}
    }
#endif /* THREADSAFE */
    return (D2_NOERROR) ;
}

/* -------------------------------------------------------------------------- */

#ifdef THREADSAFE
pthread_startroutine_t d21_evdispatcher(
  pthread_addr_t arg)
{
    int			cnt ;
    signed16		i ;
    D23_evntpb		evpb ;
    d21_ipc_info	*ipcref ;
    fd_set		rfds, wfds, xfds ;
    const char function_name[] = "d21_evdispatcher";

DUA_TRACE_ENTRY_EVENT_DISPATCHER
    /* initialize unused fd-arrays */
    FD_ZERO(&wfds) ;
    FD_ZERO(&xfds) ;
		
    for (;;) {
	/* set IPC-event fd(s) */
	FD_ZERO(&rfds) ;
	for (i = 0; i < d21_empb.d23_Enfds; i++)
	    FD_SET(*(d21_empb.d23_Eipcfds + i), &rfds) ;

	/* wait for IPC-event */
	while (((cnt = select (FD_SETSIZE, &rfds, &wfds, &xfds, NULL)) < 0 &&
						  errno == EINTR) || cnt == 0) ;

	pthread_yield();
	if (end_of_process == TRUE)
	  {
	  DUA_TRACE_SELECT_LEFT
	  select_left = TRUE;

	  pthread_mutex_lock(&detach_mutex_handle);
	  pthread_cond_signal(&detach_cond_handle);
	  pthread_mutex_unlock(&detach_mutex_handle);
	  (void)pthread_exit((pthread_addr_t) 0);
	  } /* of end_of process = true */

	if (cnt < 0) {
	    /* write error log-entry */
	    DUA_TRACE_SELECT_CALL_FAILED(errno)
	    continue ;
	}

	/* get IPC-event */
	evpb.d23_Bhdinfo.d2_version = D23_V02 ;
	if (d23_event (&evpb) == D23_ERROR) {
	    /* write error log-entry */
	    continue ;
	}

	if (evpb.d23_Bevtype == D23_NOEVNT)
	    /* no IPC-event available */
	    continue ;

	/* dispatch received IPC-event */
	for (i = 0, ipcref = d21_ipc_tab; i < D23_MXCLTPROC; i++, ipcref++)
	    if (ipcref->d21_i_used && ipcref->d21_i_assid == evpb.d23_Bassid)
		break ;

	ipcref->d21_ipcev_pblk = evpb ;
	ipcref->d21_ipcev_flag = TRUE ;

	while (ipcref->d21_ipcev_flag == TRUE)
	  {
	  if (pthread_mutex_lock (&ipcref->d21_ipcev_mutex) < 0)
	    {
	    DUA_TRACE_MUTEX_LOCK_FAILED("d21_ipcev_mutex")
	    }
	  if (pthread_cond_signal (&ipcref->d21_ipcev_condvar) < 0)
	    {
	    DUA_TRACE_PTHREAD_COND_SIGNAL_FAILED("d21_ipcev_condvar")
	    }
	  pthread_mutex_unlock (&ipcref->d21_ipcev_mutex);
	  pthread_yield();
	  }
    }
}
#endif

/* -------------------------------------------------------------------------- */

#ifdef THREADSAFE
static signed32 d21_get_ipc_event(
  D21_abandon_info * abandon_info,
  signed16 assid,
  d21_ipc_info **  ipcref)
{
const char function_name[] = "d21_get_ipc_event";
signed16 i ;
d21_ipc_info * ipchref ;
d2_ret_val ret_value = D2_NOERROR;
boolean cancel_e_catched = FALSE;
boolean abandon_thread_created = FALSE;
pthread_t d21_abandon_handle;

/* search IPC-table entry corresponding to IPC-association */
for (i = 0, ipchref = d21_ipc_tab; i < D23_MXCLTPROC; i++, ipchref++)
  {
  if (ipchref->d21_i_used && ipchref->d21_i_assid == assid)
	    break ;
  }

/* restore general cancelibility */
(void) pthread_setcancel(d21_old_cancel_state());

    /* lock access to IPC-event flag */
 if (pthread_mutex_lock (&ipchref->d21_ipcev_mutex) < 0)
  {
  ipchref->d21_ipcev_pblk.d23_Bhdinfo.d2_retcode = D2_TEMP_ERR ;
  ipchref->d21_ipcev_pblk.d23_Bhdinfo.d2_errclass = D2_INT_ERR ;
  ipchref->d21_ipcev_pblk.d23_Bhdinfo.d2_errvalue = errno ;
  ret_value = D2_ERROR;
  return (ret_value) ;
  }

/* wait for IPC-event */
while (!ipchref->d21_ipcev_flag || cancel_e_catched)
	/* wait on conditional variable */
  {
  cancel_e_catched = FALSE;
  TRY
    {
    if (pthread_cond_wait (&ipchref->d21_ipcev_condvar,
					       &ipchref->d21_ipcev_mutex) < 0)
      {
      ipchref->d21_ipcev_pblk.d23_Bhdinfo.d2_retcode = D2_TEMP_ERR ;
      ipchref->d21_ipcev_pblk.d23_Bhdinfo.d2_errclass = D2_INT_ERR ;
      ipchref->d21_ipcev_pblk.d23_Bhdinfo.d2_errvalue = errno ;

      /* unlock access to IPC-event flag */
      pthread_mutex_unlock (&ipchref->d21_ipcev_mutex) ;
      ret_value = D2_ERROR;
      } /* end of : pthread_cond_wait has failed */
    } /* end of TRY */
  CATCH (pthread_cancel_e)
    {
    cancel_e_catched = TRUE;
    if (abandon_info != NULL)
      {
      DUA_TRACE_CANCEL_EXCEPTION_DETECTED(abandon_info->op_code)
      }
    if (abandon_info != NULL &&
		( abandon_info->op_code == D23_READ ||
		  abandon_info->op_code == D23_COMPARE ||
		  abandon_info->op_code == D23_LIST ||
		  abandon_info->op_code == D23_SEARCH))
      {
      ret_value = D21_INTERRUPTED;

      if (pthread_create (&d21_abandon_handle,pthread_attr_default,
			  (pthread_startroutine_t)d21_abandon_handler,
			  (pthread_addr_t)abandon_info) < 0)
	{
	DUA_TRACE_PTHREAD_CREATE_FAILED("d21_abandon_handler")
	abandon_thread_created = FALSE;
	}
      else
	{
	abandon_thread_created = TRUE;
	}
      } /* end of: valid abandon request */
    } /* end of CATCH */
  ENDTRY
  } /* end of : while IPC-event flag == FALSE */

/* reset IPC-event flag */
ipchref->d21_ipcev_flag = FALSE ;

/* unlock access to IPC-event flag */
if (pthread_mutex_unlock (&ipchref->d21_ipcev_mutex) < 0)
  {
  ipchref->d21_ipcev_pblk.d23_Bhdinfo.d2_retcode = D2_TEMP_ERR ;
  ipchref->d21_ipcev_pblk.d23_Bhdinfo.d2_errclass = D2_INT_ERR ;
  ipchref->d21_ipcev_pblk.d23_Bhdinfo.d2_errvalue = errno ;
  ret_value = D2_ERROR;
  }	

if (abandon_thread_created == TRUE)
  {
  (void)pthread_join(d21_abandon_handle,NULL);
  }

*ipcref = ipchref ;
return (ret_value) ;
}
#endif

/* -------------------------------------------------------------------------- */

signed32 d21_ipcinvoke(
  D23_inrqpb * sndpb)
{
    signed32		result ;

    /* send IPC-request */
    if ((result = d23_invoke (sndpb)) == D23_ERROR)
	return (result) ;

#ifdef THREADSAFE
    {
    d21_ipc_info	*ipcref ;

    /* wait for return buffer message */
    if (sndpb->d23_6imdat == D23_MRDATA) {
	if (d21_get_ipc_event (NULL,sndpb->d23_6assid, &ipcref) == D2_ERROR) {
	    sndpb->d23_6hdinfo = ipcref->d21_ipcev_pblk.d23_Bhdinfo ;
	    return (D23_ERROR) ;
	}
	if (ipcref->d21_ipcev_pblk.d23_Bevtype != D23_RRQBFEVNT) {
	    sndpb->d23_6hdinfo.d2_retcode = D2_TEMP_ERR ;
	    sndpb->d23_6hdinfo.d2_errclass = D2_INT_ERR ;
	    sndpb->d23_6hdinfo.d2_errvalue = D23_ABOERR ;
	    return (D23_ERROR) ;
	}
    }
    }
#endif
    return (result) ;
}

/* -------------------------------------------------------------------------- */

signed32 d21_ipcreceive(
  D21_abandon_info * abandon_info,
  D23_wtrspb * rcvpb,
  signed16 mflg)
{
#ifdef THREADSAFE
    D23_rbufpb		rbpb ;
    d21_ipc_info	* ipcref ;
    signed32 ret_value = D2_NOERROR;

    if (mflg == D23_MRDATA) {
	/* return communication buffer */
	rbpb.d23_Chdinfo.d2_version = D23_V02 ;
	rbpb.d23_Cassid = rcvpb->d23_7assid ;
	if (d23_retbuf (&rbpb) == D23_ERROR) {
	    rcvpb->d23_7hdinfo = rbpb.d23_Chdinfo ;
	    return (D2_ERROR) ;
	}
    }
    /* wait for IPC-event */
    if ((ret_value = d21_get_ipc_event(abandon_info,rcvpb->d23_7assid, &ipcref))
			== D2_ERROR)
        {
	rcvpb->d23_7hdinfo = ipcref->d21_ipcev_pblk.d23_Bhdinfo ;
	return (ret_value) ;
        }

    /* supply waitrs-parameter block (from event-parameter block) */
    rcvpb->d23_7assid = ipcref->d21_ipcev_pblk.d23_Bassid ;
rcvpb->d23_7invinfo = ipcref->d21_ipcev_pblk.d23_Binvinfo ;
    rcvpb->d23_7rltid =
	    (ipcref->d21_ipcev_pblk.d23_Bevtype == D23_ABOEVNT) ? D23_ABORT :
					      ipcref->d21_ipcev_pblk.d23_Bopid ;
    rcvpb->d23_7rmdat = ipcref->d21_ipcev_pblk.d23_Bmdat ;
    rcvpb->d23_7rfrdt = ipcref->d21_ipcev_pblk.d23_Bref ;
    rcvpb->d23_7lnrdt = ipcref->d21_ipcev_pblk.d23_Bln ;
    return (ret_value) ;
#else
    return (d23_waitrs (rcvpb)) ;
#endif
}
