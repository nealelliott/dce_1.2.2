/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk_client.c,v $
 * Revision 1.1.15.2  1996/02/18  19:25:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:57  marty]
 *
 * Revision 1.1.15.1  1995/12/08  15:03:52  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/11/03  18:09 UTC  psn
 * 	Change #include sec_login_util.h to sec_login.h for DCE 1.2.1 bug fix drop to OSF
 * 	[1995/12/08  14:37:47  root]
 * 
 * Revision 1.1.13.7  1994/08/03  18:59:21  mccann
 * 	include cleanup, fix new epoch
 * 	[1994/08/01  18:51:19  mccann]
 * 
 * Revision 1.1.13.6  1994/06/23  18:28:42  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:53:37  mccann]
 * 
 * Revision 1.1.13.5  1994/06/09  16:07:32  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:13  devsrc]
 * 
 * Revision 1.1.13.4  1994/05/25  22:22:23  peckham
 * 	Take operations with side-effects out of dce_assert().
 * 	[1994/05/25  22:18:10  peckham]
 * 
 * Revision 1.1.13.3  1994/05/12  21:10:56  peckham
 * 	Ignore SIGPIPE to avoid core dumps.
 * 	[1994/05/12  19:13:52  peckham]
 * 
 * Revision 1.1.13.2  1994/04/14  14:41:05  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:35:44  peckham]
 * 
 * Revision 1.1.13.1  1994/03/12  21:56:13  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:49:53  peckham]
 * 
 * Revision 1.1.9.4  1992/12/30  12:40:29  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:25:58  zeliff]
 * 
 * Revision 1.1.9.3  1992/11/04  20:22:08  keegan
 * 	The clerk was deleting all cached binding handles when the login
 * 	context changed, even if they were in use.  Now, the clerk calls
 * 	a new function (free_inuse_cached_handle) instead, which marks
 * 	the handle for deletion, but doesn't decrement the use count.
 * 	Also added some related instrumentation.
 * 	[1992/11/04  19:44:33  keegan]
 * 
 * Revision 1.1.9.2  1992/09/29  19:08:47  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:10:11  weisman]
 * 
 * Revision 1.1.2.3  1992/04/21  18:04:06  keegan
 * 	Plug a small memory leak in the clerk.
 * 	[1992/04/21  18:01:37  keegan]
 * 
 * Revision 1.1.2.2  1992/03/22  21:19:28  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:07:18  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerk_client.c
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
 * MODULE DESCRIPTION:
 *
 * Clerk client service thread
 *
 */

#include <dce/dce.h>
#include <dce/assert.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include <uc_clerk.h>
#include <cache.h>
#include <dce/sys_time.h>
#include <threads_lib.h>

#if defined(DCE_SEC)
# include <dce/sec_login.h>
#endif

typedef struct {
    void		(*rtn)(thread_t *);
    byte_t		op;
    byte_t		obj;
    byte_t		type;
    int                 timeout;
} optab_t;



/*
 * Function prototypes
 */

static dthread_address_t
clerk_client (link_t *);

static dthread_address_t
clerk_request (link_t *);

static void
stop_requests (link_t *);

static optab_t *
parse_arguments (thread_t *);

static void
notsupported (thread_t *);

static byte_t *
read_request (link_t *);

static void
write_response (thread_t *);

static link_t *
link_malloc (clerk_t *, int);

static void
link_free (link_t *);

static void
clerkLocal (thread_t   *);

#define DNSTRANSA	44
#define DNSUPLIST	45

static optab_t optab[] = {
    {clerkLocal, OP_Local, 0, 0, 0},
    {clerkEnumerateAttributes, OP_EnumerateAttributes, DNSTRANSA, CH_miscl, 0},
    {clerkReadAttribute, OP_ReadAttribute, DNSTRANSA, CH_read, 0},
    {clerkModifyAttribute, OP_ModifyAttribute, DNSTRANSA, CH_write, 0},
    {clerkTestAttribute, OP_TestAttribute, DNSTRANSA, CH_miscl, 0},
    {clerkCreateObject, OP_CreateObject, DNSTRANSA, CH_write, 0},
    {clerkEnumerateObjects, OP_EnumerateObjects, DNSTRANSA, CH_miscl, 0},
    {clerkDeleteObject, OP_DeleteObject, DNSTRANSA, CH_write, 0},
    {clerkCreateDirectory, OP_CreateDirectory, DNSTRANSA, CH_write, 0},
    {clerkDeleteDirectory, OP_DeleteDirectory, DNSTRANSA, CH_write, 0},
    {clerkAddReplica, OP_AddReplica, DNSTRANSA, CH_miscl, 60*60*24},
    {clerkRemoveReplica, OP_RemoveReplica, DNSTRANSA, CH_miscl, 60*60*24},
    {clerkEnumerateChildren, OP_EnumerateChildren, DNSTRANSA, CH_miscl, 0},
    {clerkSkulk, OP_Skulk, DNSTRANSA, CH_miscl, 60*60*24},
    {clerkCreateLink, OP_CreateLink, DNSTRANSA, CH_write, 0},
    {clerkDeleteLink, OP_DeleteLink, DNSTRANSA, CH_write, 0},
    {clerkResolveName, OP_ResolveName, DNSTRANSA, CH_miscl, 0},
    {clerkTestGroup, OP_TestGroup, DNSTRANSA, CH_miscl, 0},
    {nsclerkCreateChildEntry, OP_CreateChildEntry, DNSTRANSA, CH_write, 0},
    {clerkEnumerateSoftLinks, OP_EnumerateLinks, DNSTRANSA, CH_miscl, 0},
    {nsclerkDeleteChildEntry, OP_DeleteChildEntry, DNSTRANSA, CH_write, 0},
    {clerkReadReplica, OP_ReadReplica, DNSTRANSA, CH_read, 0},
    {nsclerkModifyReplica, OP_ModifyReplica, DNSTRANSA, CH_write, 0},
    {nsclerkLinkReplica, OP_LinkReplica, DNSTRANSA, CH_miscl, 0},
    {notsupported, 0, 0, 0, 0},
    {nsclerkCombine, OP_Combine, DNSUPLIST, CH_miscl, 60*60*24},
    {nsclerkDoUpdate, OP_DoUpdate, DNSUPLIST, CH_miscl, 0},
    {notsupported, 0, 0, 0, 0}, 
    {notsupported, 0, 0, 0, 0},
    {clerkNewEpoch, OP_NewEpoch, DNSTRANSA, CH_miscl, 60*60*24}
};
#define MIN_OPTAB (optab[0].op)
#define MAX_OPTAB (sizeof(optab)/sizeof(optab[0]))

/*
 * Cleanup for unlocking mutex
 */
static void 
link_unlock (dthread_mutex_t *mutex_p)
{
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >link_unlock"));

    if ((thstatus = dthread_unlock(&(*mutex_p))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <link_unlock"));
}


/*
 *		s t a r t _ c l i e n t
 *
 * Allocates a thread block and creates a new thread to service
 * a new client connection.
 *
 * Inputs:
 *	clerk_p		Clerk listener data
 *	fd		New file descriptor to client
 *
 * Returns:
 *	TRUE if service started
 *	False for resource failures - file descriptor is closed.
 */
int 
start_client (clerk_t  *clerk_p,
              int      fd)
{
    link_t	*ll_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug3,
        " >start_client(0x%p->%s,%d)",
        clerk_p,
        clerk_p->socket_name,
        fd));

    if ((ll_p = link_malloc(clerk_p, fd)) != NULL) {
	extern dthread_attr_t clerk_client_attr;

	if (0 <= dthread_create(&ll_p->ll_thread, &clerk_client_attr,
		(dthread_start_routine)clerk_client, (dthread_address_t)ll_p)) 
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug3,
                " <start_client(0x%p->%s,%d) return(TRUE)",
                clerk_p,
                clerk_p->socket_name,
                fd));

	    return(TRUE);
	}

	link_free(ll_p);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug3,
        " <start_client(0x%p->%s,%d) return(FALSE)",
        clerk_p,
        clerk_p->socket_name,
        fd));

    return(FALSE);
}

/*
 * NOTE: assumes that the clerk_mutex is held.
 */
void 
kill_client (link_t *ll_p)
{
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug3,
        " >kill_client(0x%p->%d)",
        ll_p,
        ll_p->ll_fd));

    if ((thstatus = dthread_lock(&(ll_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

    /* Beginning of the 'protected code' */
    {
    
	switch ((int)ll_p->ll_state) {
	default:
	    ll_p->ll_state = ll_Cancel;
	    (void)dthread_cancel(ll_p->ll_thread);
	case ll_Rundown:
	case ll_Cancel:
	case ll_Off:
	    break;
	}

	while (ll_p->ll_state != ll_Off)
          {
              if ((thstatus = dthread_wait(&(ll_p->ll_state_changed),
                                           &(ll_p->ll_mutex))) < 0)
              {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
              }
          }

    }
    /*
     * End of the 'protected code' 
     */

    if ((thstatus = dthread_unlock(&(ll_p->ll_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug3,
        " <kill_client(0x%p->%d)",
        ll_p,
        ll_p->ll_fd));
}

void 
kill_clients (clerk_t *clerk_p)
{
    link_t *ll_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " >kill_clients(0x%p->%s)",
        clerk_p,
        clerk_p->socket_name));

    if ((thstatus = dthread_lock(&(clerk_p->mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */

	if ((ll_p = clerk_p->link_p) != NULL)
	    do
		kill_client(ll_p);
	    while ((ll_p = ll_p->next_p) != NULL);

	while (clerk_p->handler_count)
	{
              if ((thstatus = dthread_wait(&(clerk_p->state_changed),
                                           &(clerk_p->mutex))) < 0)
              {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
              }
          }


    /* End of the 'protected code' */
    if ((thstatus = dthread_unlock(&(clerk_p->mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " <kill_clients(0x%p->%s)",
        clerk_p,
        clerk_p->socket_name));
}

/*
 *		c l e r k _ c l i e n t
 *
 * Opens a UNIX-domain socket, accepts connections, and passes
 * the new file descriptor to a newly-created client thread.
 *
 * Inputs:
 *     th_p
 *
 * Returns:
 *	none
 */
static dthread_address_t 
clerk_client (link_t *ll_p)
{
  extern dthread_attr_t clerk_request_attr;
  dthread_t reqthrd;
  struct sigaction newsig;
  int thstatus, sig_status;

#if defined(DCE_CDS_DEBUG)

  int		link_fd = ll_p->ll_fd;

#endif

  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug3,
      " >clerk_client(0x%p->%d)",
      ll_p,
      link_fd));

  /*
   * Make sure sigpipe does not stop us.
   */
  newsig.sa_handler = SIG_IGN;
  sigemptyset(&newsig.sa_mask);
  newsig.sa_flags = 0;
  sig_status = sigaction(SIGPIPE, &newsig, NULL);
  dce_assert(cds__svc_handle, 0 <= sig_status);

  if ((thstatus = dthread_lock(&(ll_p->ll_mutex))) < 0)
  {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
  }

   /* Beginning of the 'protected code' */
    ll_p->ll_state = ll_On;
    {

        if ((thstatus = dthread_broadcast(&(ll_p->ll_state_changed))) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
        }
    }

  /* End of the 'protected code' */
  if ((thstatus = dthread_unlock(&(ll_p->ll_mutex))) < 0)
  {
    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
  }
  dthread_cleanup_push(link_free, ll_p);

  while ((ll_p->ll_reqData_p = read_request(ll_p)) != NULL) {

    if ((thstatus = dthread_lock(&(ll_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */

      if (!ll_p->ll_waiter_p)
	if (0 <= dthread_create(&reqthrd, &clerk_request_attr,
		(dthread_start_routine)clerk_request, (dthread_address_t)ll_p))
	  (void)dthread_detach(&reqthrd);

      {

          if ((thstatus = dthread_broadcast(&(ll_p->ll_state_changed))) < 0)
          {
	    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
          }
      }

    /* End of the 'protected code' */
    if ((thstatus = dthread_unlock(&(ll_p->ll_mutex))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    /*
     * Wait for the bait to be taken
     */

    if ((thstatus = dthread_lock(&(ll_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */
      dthread_cleanup_push(link_unlock, &ll_p->ll_mutex);
      while (ll_p->ll_reqData_p)
      {
          if ((thstatus = dthread_wait(&(ll_p->ll_state_changed),
                                       &(ll_p->ll_mutex))) < 0)
          {
	    dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
          }
      }
      dthread_cleanup_pop(0);

 /* End of the 'protected code' */
 if ((thstatus = dthread_unlock(&(ll_p->ll_mutex))) < 0)
 {
   dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
 }
  }
  
  stop_requests(ll_p);
  dthread_cleanup_pop(1);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug3,
      " <clerk_client(0x%p->%d) return(0)",
      ll_p,
      link_fd));

  return(0);
}

/*
 *		c l e r k _ r e q u e s t
 *
 * Parses and interprets incoming request, dispatches, and sends response.
 *
 * Inputs:
 *     th_p
 *
 * Returns:
 *	none
 */
static dthread_address_t 
clerk_request (link_t *ll_p)
{ /* st */
    DEB_ASCII_BUF_time_local(exp)
    thread_t	*th_p = alloc_child(ll_p, NULL);
    register optab_t *op_p;
    time_local_t relTime;
    struct sigaction newsig;
    int thstatus, sig_status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug3,
        " >clerk_request(0x%p->%d)",
        ll_p,
        ll_p->ll_fd));

    /*
     * Make sure sigpipe does not stop us.
     */
    newsig.sa_handler = SIG_IGN;
    sigemptyset(&newsig.sa_mask);
    newsig.sa_flags = 0;
    sig_status = sigaction(SIGPIPE, &newsig, NULL);
    dce_assert(cds__svc_handle, 0 <= sig_status);

    while (th_p) 
    { /* 1 */

      if ((thstatus = dthread_lock(&(ll_p->ll_mutex))) < 0)
      {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
      }

            /*
	     * Beginning of the 'protected code' 	
	     */

	    if (!ll_p->ll_waiter_p) 
	    {
		ll_p->ll_waiter_p = th_p;

		while ((ll_p->ll_state == ll_On) && !ll_p->ll_reqData_p)
		{
                    if ((thstatus = dthread_wait(&(ll_p->ll_state_changed),
                                                 &(ll_p->ll_mutex))) < 0)
                    {
		      dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
                    }
		}

		if (ll_p->ll_reqData_p) 
		{
		    COPY_bytes(&ll_p->ll_reqHdr, &th_p->clerkReq, 
			       sizeof(th_p->clerkReq));
		    th_p->clerkReqBuf_p = ll_p->ll_reqData_p;
		    ll_p->ll_reqData_p = NULL;

                   if ((thstatus = dthread_broadcast(&(ll_p->ll_state_changed))) < 0)
                    {
                        dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, 
				       (long)thstatus);
                    }
		}
		
		ll_p->ll_waiter_p = NULL;
	    }
      
      
      /* End of the 'protected code' */
      if ((thstatus = dthread_unlock(&(ll_p->ll_mutex))) < 0)
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
      }

    if (!th_p->clerkReqBuf_p) 
    {
	break;
    }

    /* 
     * New request 
     */
    ++ll_p->ll_clerk_p->Reads;

    th_p->clerkRspMsgEnd_p = th_p->clerkRspMsg_p;

    /*
     * Transfer critical data from request to response header
     */
    th_p->taOperation = UCP_EXTbyte(th_p->clerkReq.op);
    th_p->clerkDontCache = UCP_EXTbyte(th_p->clerkReq.dontCache);
    th_p->clerkConf = UCP_EXTbyte(th_p->clerkReq.conf);
    th_p->clerkPartialRes = UCP_EXTaddress(th_p->clerkReq.partialResults);
    th_p->clerkResLength = UCP_EXTlongword(th_p->clerkReq.resLen);
    th_p->clerkResLength_p = UCP_EXTaddress(th_p->clerkReq.resLen_p);
    th_p->clerkFromCache_p = UCP_EXTaddress(th_p->clerkReq.fromCache_p);
    th_p->clerkOutLink_p = UCP_EXTaddress(th_p->clerkReq.outLink_p);
    th_p->clerkCacheTimeout = UCP_EXTlongword(th_p->clerkReq.cacheTimeout);
    th_p->clerkWait = UCP_EXTlongword(th_p->clerkReq.wait);
    th_p->clerkStatus = DNS_SUCCESS;
    th_p->level = 0;
    th_p->cache_the_dir = 0;
    th_p->skipResolveNameCache = 0;
    th_p->used_cached_replica = 0;
    th_p->outLinked = 0;
    th_p->clerkFromCache = 0;
    th_p->cle_p = NULL;

    if ((op_p = parse_arguments(th_p)) != NULL) 
    {
	/*
	 * Assert -> FATAL exit if the expression is false !
	 */
	dce_assert(cds__svc_handle, (th_p->taOperation == op_p->op));

        th_p->taObj = op_p->obj;
        th_p->OPtype = op_p->type;
        th_p->DefaultTimeout = op_p->timeout;

        /* If DCE security enabled, the user's *opaque* login context  is
         * saved for hashing the cache, otherwise the local uid is used.
         * The opaque login context is saved in 'parse_arguments'
         */

#if !defined(DCE_SEC)
        th_p->user = dns_user_name(UCP_EXTlongword(th_p->clerkReq.user_uid),
                                   UCP_EXTlongword(th_p->clerkReq.user_gid));
#endif

        /* Set the thread timeout */
        if (th_p->clerkWait == 0) 
	{
	    if (th_p->DefaultTimeout > CAfixed_p->ca_ClerkTimeout) 
	    {
		th_p->clerkWait = th_p->DefaultTimeout;
	    }
	    else 
	    {
		th_p->clerkWait = CAfixed_p->ca_ClerkTimeout;
	    }
	}

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug3,
            " clerk_request: clerkWait is %d",
            th_p->clerkWait));

	/* ok, set absolute expiration time */
	sys_time_from_interval(&relTime, 0, 0, th_p->clerkWait);
	sys_time_to_absolute(&relTime, &(th_p->expire));

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug3,
            " clerk_request: expire is %s",
            deb_ascii_time_local(exp,&th_p->expire)));

        (*op_p->rtn)(th_p);
    }
    
	  
    if (th_p->clerkReqBuf_p) 
    {
	dns_free((char *)th_p->clerkReqBuf_p);
	th_p->clerkReqBuf_p = NULL;
    }

    if (th_p->taRspBuf_p) 
    {
	dns_free_buffer((char *)th_p->taRspBuf_p);
	th_p->taRspBuf_p = NULL;
    }

    /*
     * Grab the link for writing.
     */

    if ((thstatus = dthread_lock(&(ll_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */
	while (ll_p->ll_state == ll_On)
	    if (ll_p->ll_writer_p) 
	    {

                if ((thstatus = dthread_wait(&(ll_p->ll_state_changed),
                                             &(ll_p->ll_mutex))) < 0)
                {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
                }
	    } 	
	    else 
	    {
		ll_p->ll_writer_p = th_p;
		break;
	    }

      /* End of the 'protected code' */
      if ((thstatus = dthread_unlock(&(ll_p->ll_mutex))) < 0)
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
      }

    /*
     * If we got the link (it is still up)
     * send our response.
     */
    if (ll_p->ll_writer_p == th_p) 
    { /* 2 */

	write_response(th_p);
	/*
	 * CLOSE THE LINK DOWN IF WRITE FAILED 
	 */

      if ((thstatus = dthread_lock(&(ll_p->ll_mutex))) < 0)
      {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
      }

       /* Beginning of the 'protected code' */
	    ll_p->ll_writer_p = NULL;
	
       if ((thstatus = dthread_broadcast(&(ll_p->ll_state_changed))) < 0)
       {
	   dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
       }

	/*
	 * End of the 'protected code' 
	 */
	if ((thstatus = dthread_unlock(&(ll_p->ll_mutex))) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}
    }

    /* End request */
    ++ll_p->ll_clerk_p->Writes;

    /* Update global cache counters */
    if (th_p->cle_p != NULL)
    {
	(void)CAClearinghousesUsed(th_p->cle_p);
    }
  }


  if (th_p)
  {
      free_child(th_p);
  }
  

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug3,
      " <clerk_request(0x%p->%d) return(0)",
      ll_p,
      ll_p->ll_fd));

  return(0);
}



static void 
stop_requests (link_t *ll_p)
{
    int old_cancel = dthread_setcancel(CANCEL_OFF);
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
	" >stop_requests"));
    
    if ((thstatus = dthread_lock(&(ll_p->ll_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */
	ll_p->ll_state = ll_Rundown;
      {
          if ((thstatus = dthread_broadcast(&(ll_p->ll_state_changed))) < 0)
          {
	    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
          }
      }

	while (ll_p->ll_handler_count)
          {
              if ((thstatus = dthread_wait(&(ll_p->ll_state_changed),
                                           &(ll_p->ll_mutex))) < 0)
              {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
              }
          }

    /* End of the 'protected code' */
    if ((thstatus = dthread_unlock(&(ll_p->ll_mutex))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    (void)dthread_setcancel(old_cancel);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
	" <stop_requests"));
}

/* Parse arguments from library.  If we are using security, the 0th
 * argument, if non-0, is the client's secure login context.  If
 * present, import it and set our login context to that of the client.
 * The external (opaque) and internal login context for this user
 * account are saved in the link structure, because different processes
 * may have different access profiles (even though they are running
 * under 'same' user name). The login context is only supplied in the
 * 1st message from the client. If not supplied, we use the one
 * saved in the link structure.
*/
static optab_t 
*parse_arguments (thread_t *th_p)
{
#if defined(DCE_SEC)
    byte_t *p;
    int    done;
    handle_cache_t *cache_p;
#endif

    register byte_t **argPtr = th_p->clerkArgs;
    register byte_t **endArg = &th_p->clerkArgs[MAX_ARGS];
    register byte_t *reqPtr = th_p->clerkReqBuf_p;
    register byte_t *endReq = &reqPtr[UCP_EXTlongword(th_p->clerkReq.length)
	- (sizeof(th_p->clerkReq) - sizeof(th_p->clerkReq.length))];
    register int i = th_p->taOperation;
    register argcnt = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
	" >parse_arguments"));

    if (UCP_EXTbyte(th_p->clerkReq.version) != UCP_VERSION) {
	th_p->clerkStatus = DNS_CLERKBUG;

	DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
	" <parse_arguments: 1 ret(NULL)"));

	return(NULL);
    }

    while ((argPtr < endArg) && (reqPtr < endReq)) {
	register int length = GET16(reqPtr);
	/* If arg 0 is non-0, save opaque login context */
	if (argcnt++ == 0) {
	    if (length > sizeof(field16)) {

#if defined(DCE_SEC)

	      done = FALSE;

	      /* 
	       * Save the opaque and internal login context in the link structure,
	       * Save the opaque login context in the 'user_t' field of thread_t,
	       * for convience.
	       * Precede the context with its length.
	       * This will we used for hashing the cache.
	       */

	      /*
	       * if had one from before - see if it changed 
	       */
	      if (th_p->link_p->ll_opq_login_context) { 
		  if (EQ_bytes(th_p->link_p->ll_opq_login_context + 
			       sizeof(field16), reqPtr, length))
		    done = TRUE;
		  else 
		  {
                    DCE_SVC_DEBUG((
                        cds__svc_handle,
                        cds_svc_child,
                        svc_c_debug8,
                        " parse_arguments: Login context changed from %s to %s", 
                        th_p->link_p->ll_opq_login_context, 
                        reqPtr)); 

		    /* free it and get newer one*/
		    if (th_p->link_p->ll_opq_login_context)
			dns_free ((char *)th_p->link_p->ll_opq_login_context); 
		    /* release all cached binding handles */
		    while ((cache_p = 
			   (handle_cache_t *)th_p->link_p->ll_binding_hdr.next_p) !=
			   (handle_cache_t *)&th_p->link_p->ll_binding_hdr) 
			free_inuse_cached_handle (th_p, cache_p);
		  }
	      }
	      if (!done) {
		  p = th_p->link_p->ll_opq_login_context =
		    (byte_t *)dns_malloc((int)(length+sizeof(field16)));

		  /*
		   * note: length here includes itself + context 
		   */
		  PUT16 (p, length);
		  COPY_bytes (reqPtr, p, length);
	      }
	      reqPtr += length;

#endif	/* defined(DCE_SEC) */

	  }
	  length = GET16(reqPtr); /* arg 1 length */
	}

	/*
	 * Actual argment follows the length of the
	 * argument.
	 */
	*argPtr++ = reqPtr;
	reqPtr += length;
    }


#if defined(DCE_SEC)

    /* 
     * Add secure login context handle AND opaque context to thread_t.
     * We need it the former later on when we bind to a server.
     * We need the latter for hashing the cache.
     */

    th_p->user = th_p->link_p->ll_opq_login_context;

#endif

    if (((i -= MIN_OPTAB) < 0) || (MAX_OPTAB < i))
	i = 0;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug8,
	" <parse_arguments"));

    return(&optab[i]);
}

static void 
notsupported (thread_t *th_p)
{
    th_p->clerkStatus = DNS_NOTSUPPORTED;
}

static byte_t *
read_request (link_t *ll_p)
{
    byte_t *reqBuf_p = NULL;
    register char *buf_p;
    register int status, length;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " >read_request(%d)",
        ll_p->ll_fd));

    if ((status = dthread_read(ll_p->ll_fd, (char *)&ll_p->ll_reqHdr,
		sizeof(ll_p->ll_reqHdr))) != sizeof(ll_p->ll_reqHdr)) 
    {

read_errno:

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug7,
            " <read_request(%d) %d=read() errno(%d)",
            ll_p->ll_fd,
            status,
            errno));

	if (reqBuf_p) 
	{
	    dns_free((char *)reqBuf_p);	
	}

	return(NULL);
    }

    /*
     * Pick up length of request buffer, allocate, and read
     */
    if ((length = UCP_EXTlongword(ll_p->ll_reqHdr.length)
			- (sizeof(ll_p->ll_reqHdr)
			- sizeof(ll_p->ll_reqHdr.length))) != 0)
	if ((buf_p = dns_malloc(length)) != NULL) 	
	{
	    reqBuf_p = (byte_t *)buf_p;
	    /*
	     * Read remainder of the arguments sent by the client.
	     */
	    do {
		if ((status = dthread_read(ll_p->ll_fd, buf_p, length)) <= 0)
		    goto read_errno;
	    } while (buf_p += status, length -= status);
	} 
	else 	
	{
	    do 
	    {
		char buf[100];
		int size = (length < sizeof(buf)) ? length : sizeof(buf);

		if ((status = dthread_read(ll_p->ll_fd, buf, size)) <= 0)
		    goto read_errno;

	    } while ((length -= status) != 0);

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug7,
                " <read_request(%d) 0=malloc(%d) - ret(NULL)",
                ll_p->ll_fd,
                (sizeof(ll_p->ll_reqHdr.length)
	         + UCP_EXTlongword(ll_p->ll_reqHdr.length))));

	    return(NULL);
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " <read_request(%d) %d=read() - ret(reqbuf_p)",
        ll_p->ll_fd,
	(sizeof(ll_p->ll_reqHdr.length)
	 + UCP_EXTlongword(ll_p->ll_reqHdr.length))));

    return(reqBuf_p);
}

static void 
write_response (thread_t *th_p)
{
    ucp_resp_header rspBuf;
    register link_t *ll_p = th_p->link_p;
    char *buf_p = (char *)th_p->clerkRspBuf_p;
    register int status, length = 0;

    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
	" >write_response"));
    /*
     * Figure length of response arguments
     */
    if (th_p->clerkRspMsgEnd_p)
	    length = th_p->clerkRspMsgEnd_p - (byte_t *)buf_p;

    /* Exceptions don't always pass data */
    if (length == 0) length = sizeof(rspBuf);

    UCP_INSlongword(rspBuf.length, length - sizeof(rspBuf.length));

    UCP_INSlongword(rspBuf.sequence, UCP_EXTlongword(th_p->clerkReq.sequence));

    UCP_INSaddress(rspBuf.handle, UCP_EXTaddress(th_p->clerkReq.handle));

    UCP_INSlongword(rspBuf.status, th_p->clerkStatus);

    COPY_bytes(&rspBuf, buf_p, sizeof(rspBuf));

    /*
     * Send the reply to the client.
     */
    do {

	if ((status = dthread_write(ll_p->ll_fd, buf_p, length)) <= 0)
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
		cds_svc_child,
		svc_c_debug7,
		" <write_response: return because dth_write failed"));

	    return;
	}
	
    } while (buf_p += status, length -= status);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
	" <write_response"));
}

/*
 * link_malloc - allocate per-link resources
 */
static link_t 
*link_malloc (clerk_t  *clerk_p,
              int      fd)
{
    register link_t	*ll_p = (link_t *)dns_malloc(sizeof(*ll_p));
    link_t 		*link_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
	" >link_malloc"));

    if (!ll_p) {
	(void)dthread_close(fd);

	DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
	" <link_malloc: ll_p is invalid"));

	return(ll_p);
    }

    ZERO_bytes(ll_p, sizeof(*ll_p));
    --ll_p->ll_fd;

    if (dthread_create_mutex(&ll_p->ll_mutex) < 0) {
	(void)dthread_close(fd);
	link_free(ll_p);

	DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
	" <link_malloc: 1 ret(NULL)"));

	return(NULL);
    }

    ll_p->ll_fd = fd;

    if (dthread_create_condition(&ll_p->ll_state_changed) < 0) {
	link_free(ll_p);

	DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
	" <link_malloc: 2 ret(NULL)"));

	return(NULL);
    }

    /* Initialize linked list of cached handles */
    ll_p->ll_binding_hdr.prev_p = (handle_cache_t *)&ll_p->ll_binding_hdr;
    ll_p->ll_binding_hdr.next_p = (handle_cache_t *)&ll_p->ll_binding_hdr;

    ll_p->ll_state = ll_Init;

    if ((ll_p->ll_clerk_p = clerk_p) != NULL) 
    {

      if ((thstatus = dthread_lock(&(clerk_p->mutex))) < 0)
      {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
      }

       /* Beginning of the 'protected code' */

	    if ((link_p = clerk_p->link_p) != NULL) {
		link_p->prev_p = ll_p;
		ll_p->next_p = link_p;
		clerk_p->link_p = ll_p;
	    } else
		clerk_p->link_p = ll_p;

	    clerk_p->handler_count++;

      /* End of the 'protected code' */

      if ((thstatus = dthread_unlock(&(clerk_p->mutex))) < 0)
      {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
      }
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
	" <link_malloc"));

    return(ll_p);
}

/*
 * link_free - release per-link resources
 */
static void 
link_free (link_t *ll_p)
{
    int thstatus;
    int fd;
    clerk_t *clerk_p;
    link_t *next_p, *prev_p;


#if defined(DCE_SEC)
  handle_cache_t *cache_p;
#endif


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " >link_free(%lx)",
      ll_p));

    /*
     * (the following is only active if there is a mutex)
     */
    if (!dthread_equal(ll_p->ll_mutex, dthread_null)) 
    {

      if ((thstatus = dthread_lock(&(ll_p->ll_mutex))) < 0)
      {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
      }

       /* Beginning of the 'protected code' */


	    if (0 <= (fd = ll_p->ll_fd)) {
		ll_p->ll_fd = -1;
		(void)dthread_close(fd);
	    }

	    ll_p->ll_state = ll_Rundown;

	    if (!dthread_equal(ll_p->ll_state_changed, dthread_null)) 
	    {

		if ((thstatus = dthread_broadcast(&(ll_p->ll_state_changed))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
                }

		/*
		 * Unblock any waiting request thread
		 * and wait for them to die.
		 */
		while (ll_p->ll_handler_count)
		{
                    if ((thstatus = dthread_wait(&(ll_p->ll_state_changed),
                                                 &(ll_p->ll_mutex))) < 0)
                    {
		      dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
                    }
		}
		
	    }
      
    
	    

   /* End of the 'protected code' */
   if ((thstatus = dthread_unlock(&(ll_p->ll_mutex))) < 0)
   {
     dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
   }

	ll_p->ll_state = ll_Off;
	if (!dthread_equal(ll_p->ll_state_changed, dthread_null)) {
          {

	     if ((thstatus = dthread_broadcast(&(ll_p->ll_state_changed))) < 0)
              {
		dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, 
			       (long)thstatus);
              }
          }
	}

	if ((clerk_p = ll_p->ll_clerk_p) != NULL) {
	    /*
	     * Remove myself from the list
	     */

          if ((thstatus = dthread_lock(&(clerk_p->mutex))) < 0)
          {
	    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
          }

           /* Beginning of the 'protected code' */

		if ((next_p = ll_p->next_p) != NULL)
		    next_p->prev_p = ll_p->prev_p;

		if ((prev_p = ll_p->prev_p) != NULL)
		    prev_p->next_p = ll_p->next_p;
		else
		    clerk_p->link_p = ll_p->next_p;

		if (--clerk_p->handler_count == 0)
                {
                    if ((thstatus = dthread_broadcast(&(clerk_p->state_changed))) < 0)
                    {
		      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, 
				     (long)thstatus);
                    }
                }

#if defined(DCE_SEC)
		/* Release cached stuff */
		while ((cache_p =
			(handle_cache_t *)ll_p->ll_binding_hdr.next_p) !=
		       (handle_cache_t *)&ll_p->ll_binding_hdr) 
		  delete_cache_handle(ll_p, cache_p);
		if (ll_p->ll_opq_login_context) {
		  dns_free((char *)ll_p->ll_opq_login_context);
		  ll_p->ll_opq_login_context = NULL;
		}
#endif


       /* End of the 'protected code' */
       if ((thstatus = dthread_unlock(&(clerk_p->mutex))) < 0)
       {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
       }
	}

          if ((thstatus = dthread_delete_mutex(&ll_p->ll_mutex)) < 0)
          {
              dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
          }
    }

    if (!dthread_equal(ll_p->ll_state_changed, dthread_null))
      {
          if ((thstatus = dthread_delete_condition(&ll_p->ll_state_changed)) < 0)
          {
              dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
          }
      }

    if (!dthread_equal(ll_p->ll_thread, dthread_null))
	(void)dthread_detach(&ll_p->ll_thread);

    dns_free((char *)ll_p);
 
 DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " <link_free(%lx)",
      ll_p));
}
void 
QueueClearinghouse (thread_t    *th_p,
                    FullName_t  *name_p,
                    ObjUID_t    *id_p,
                    int         cnttype,
                    Set_t       *address_p)
{
  CleUsed_t *cle_p;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug8,
      " >QueueClearinghouse"));
  /* 
   * Clearinghouses accessed are added to a queue to be put into
   * the cache as part of post processing.  This will greatly reduce
   * elapsed time/request
   */
  cle_p = (CleUsed_t *)dns_malloc(sizeof(*cle_p)
			- sizeof(cle_p->address) + LEN_Set(address_p));
  if (cle_p) {
    cle_p->next_p = th_p->cle_p;
    th_p->cle_p = cle_p;
    COPY_FullName(name_p, &cle_p->name);
    COPY_ObjUID(id_p, &cle_p->id);
    COPY_Set(address_p, &cle_p->address);
    cle_p->cnttype = cnttype;
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug8,
      " <QueueClearinghouse"));

}

#define FUNC_P clerkArgs[0]
/* 
 * Entry point for local operaions 
 */
void
clerkLocal (thread_t *th_p)
{

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug4,
      " >clerkLocal"));
    

  switch ((int)EXT8(th_p->FUNC_P)) {
    case op_creatects:
         clerkCreateCTS(th_p);
	 break;
    case op_noop:
	 th_p->clerkStatus = DNS_SUCCESS;
	 break;
    case op_cacheid:
	 clerkGetInfo(th_p);
	 break;
       }

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug4,
      " <clerkLocal"));
}
