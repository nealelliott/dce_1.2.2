/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk_listener.c,v $
 * Revision 1.1.16.1  1996/05/10  13:07:25  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/04/18  19:53 UTC  bissen
 * 	Fix for OT 13409. Remove the call to dthread_import_fd for the UNIX Domain
 * 	socket. CMA initialization automatically imports all open fd's at startup.
 * 	[1996/03/27  19:08 UTC  jss  /main/jss_dce12/1]
 *
 * Revision 1.1.11.6  1994/08/03  18:59:24  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:10:57  mccann]
 * 
 * Revision 1.1.11.5  1994/06/23  18:28:47  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:53:57  mccann]
 * 
 * Revision 1.1.11.4  1994/06/09  16:07:36  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:17  devsrc]
 * 
 * Revision 1.1.11.3  1994/04/14  14:41:09  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:36:00  peckham]
 * 
 * Revision 1.1.11.2  1994/04/09  18:16:33  jd
 * 	The copy of the parent's gid and uid into the child got reversed
 * 	during code cleanup.
 * 	[1994/04/09  18:01:41  jd]
 * 
 * Revision 1.1.11.1  1994/03/12  21:56:19  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:50:31  peckham]
 * 
 * Revision 1.1.9.3  1993/07/30  14:56:38  blurie
 * 	#8286 ( acl_edit hangs because the cdsadv never responds).
 * 
 * 		If SNI_SVR4_NAMED_PIPES is defined, use named pipes rather than
 * 		UNIX domain sockets in order to receive connections from
 * 		CDS clients.
 * 	[1993/07/30  13:39:21  blurie]
 * 
 * Revision 1.1.9.2  1993/06/24  19:46:33  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:36:02  hinman]
 * 
 * Revision 1.1.5.5  1993/02/03  22:33:16  blurie
 * 	Fix for OT #7105.  Remove #ifdefs around chmod() calls.
 * 	[1993/02/03  20:54:08  blurie]
 * 
 * Revision 1.1.5.4  1993/02/01  20:26:30  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  15:58:10  hinman]
 * 
 * Revision 1.1.5.3  1992/12/30  12:40:46  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:26:24  zeliff]
 * 	Revision 1.1.6.2  1993/01/11  16:09:15  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 9.5.1.4  92/12/17  13:30:28  blurie
 * 	Reentrant library changes.
 * 
 * 	Workaround for the fact that CMA does not import pre-opened file
 * 	descriptors into its database:
 * 
 * 		Change the file descriptor # which is being dup2()ed to.  From
 * 		3 to 5.  This is because the bourne shell already has fd 3
 * 		open when a process is exec()ed.
 * 	[92/12/17  10:39:01  blurie]
 * 
 * Revision 9.5.1.3  92/09/30  15:14:34  hinman
 * 	Acceptance of OSF rev 1.1.5.2
 * 
 * Revision 1.1.5.2  1992/09/29  19:08:58  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:10:17  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerk_listener.c
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
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>

#ifndef SOCK_STREAM             /* socket.h not idempotent in BSD ULTRIX */
#include <sys/socket.h>
#include <sys/un.h>
#endif

#include <stdlib.h>
#include <unistd.h>

#include <uc_clerk.h>

/*
 * Local function prototypes
 */
static dthread_address_t
clerk_listener (clerk_t *);

static void
listener_cleanup (clerk_t *);

int 
start_listener (clerk_t *);

void 
kill_listener (clerk_t *);

thread_t 
*alloc_child (link_t *,
              thread_t *);

void 
free_child (thread_t *);


/* ------------------------------------------------------------------------- */

/*
 *              s t a r t _ l i s t e n e r
 *
 * Allocates a thread block and creates a new thread to service
 * a new listener.
 *
 * Inputs:
 *      clerk_p         Clerk listener data
 *
 * Returns:
 *      TRUE if service started
 *      False for resource failures
 */
int 
start_listener (clerk_t *clerk_p)
{
    extern dthread_attr_t clerk_listener_attr;
    int ret = FALSE;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug2,
        " >start_listener(0x%p->%s)",
        clerk_p,
        clerk_p->socket_name));


    if (0 <= dthread_create(&clerk_p->listener_thread, &clerk_listener_attr,
        (dthread_start_routine)clerk_listener, (dthread_address_t)clerk_p)) 
    {
        ret = TRUE;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug2,
        " <start_listener(0x%p->%s) return(%d)",
        clerk_p,
        clerk_p->socket_name,
        ret));

    return(ret);
}


/*-------------------------------------------------------------------------- */

/*
 * Kill off the listener thread.
 */
void 
kill_listener (clerk_t *clerk_p)
{
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug2,
        " >kill_listener(0x%p->%s)",
        clerk_p,
        clerk_p->socket_name));

    if ((thstatus = dthread_lock(&(clerk_p->mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

    /* Beginning of the 'protected code' */
        if (!dthread_equal(dthread_null, clerk_p->listener_thread)) 	
	{
            dthread_address_t join_status;

            /* Cancel the listener */
            (void)dthread_cancel(clerk_p->listener_thread);

	    if ((thstatus = dthread_unlock(&(clerk_p->mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	    }

            (void)dthread_join(clerk_p->listener_thread, &join_status);

            if ((thstatus = dthread_lock(&(clerk_p->mutex))) < 0)
            {
	      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
            }

            (void)dthread_detach(&clerk_p->listener_thread);
            dthread_assign(dthread_null, &clerk_p->listener_thread);
        }
    
    /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(clerk_p->mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug2,
        " <kill_listener(0x%p->%s)",
        clerk_p,
        clerk_p->socket_name));
}


/* -------------------------------------------------------------------------- */


/*
 *              c l e r k _ l i s t e n e r
 *
 * Opens a UNIX-domain socket, accepts connections, and passes
 * the new file descriptor to a newly-created client thread.
 *
 * Inputs:
 *      none
 *
 * Returns:
 *      none
 */
static dthread_address_t 
clerk_listener (clerk_t *clerk_p)
{       
    int         cfd;
    int		 thstatus;
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug2,
        " >clerk_listener(%s)",
        clerk_p->socket_name));
  
    dthread_cleanup_push(listener_cleanup, clerk_p);

    /*
     * Create Unix domain socket and listen for connections
     * (only if socket specified on command line; otherwise use stdin)
     */
    if (clerk_p->socket_name[0] != 0)
	{
	cfd = clerk_socket_open(clerk_p->socket_name);
                } 
		else 
		{
        cfd = 5;

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug2,
            "  clerk_listener no socket name, will import fd %d",
            cfd));

    }

    if ((thstatus = dthread_lock(&(clerk_p->mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */

        /* If didn't work, update status and abort thread */

        if (0 <= (clerk_p->listenerFd = cfd)) 
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug2,
                "  clerk_listener state=On"));

            clerk_p->clerk_state = clerk_On;
        } 
	else 
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug2,
                "  clerk_listener state=Rundown"));
            clerk_p->clerk_state = clerk_Rundown;
        }

        if ((thstatus = dthread_broadcast(&(clerk_p->state_changed))) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	}

    /* End of the 'protected code' */
    if ((thstatus = dthread_unlock(&(clerk_p->mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
  
    if (0 <= cfd) 
    {
        while (clerk_p->clerk_state == clerk_On) 
	{
            int nfd = clerk_socket_accept(cfd);
      
            if (nfd < 0) 
	    {
                break;
            } 
            else if (!start_client(clerk_p, nfd)) 
	    {
                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_child,
                    svc_c_debug2,
                    "  clerk_listener start client failed, about to close %d",
                    nfd));

                (void)dthread_close(nfd);

            } 
	    else 
	    {
                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_child,
                    svc_c_debug2,
                    "  clerk_listener started client for %d",
                    nfd));
            }

        } /* End while */
    }

    dthread_cleanup_pop(1);
  
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug2,
        " <clerk_listener(%s) return(%p)",
        clerk_p->socket_name,
        (dthread_address_t)0));

    return((dthread_address_t)0);

}


/* -------------------------------------------------------------------------- */


/*
 * Clean up the listener variables on cancel
 */
static void 
listener_cleanup (clerk_t *clerk_p)
{
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >listener_cleanup"));

    if ((thstatus = dthread_lock(&(clerk_p->mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* 
      * Beginning of the 'protected code' 
      */

        if (0 <= clerk_p->listenerFd) 
	{
            (void)dthread_close(clerk_p->listenerFd);
            clerk_p->listenerFd = -1;
        }

    /* End of the 'protected code' */
    if ((thstatus = dthread_unlock(&(clerk_p->mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    /* 
     * Deassign child thread file descriptors.  This will
     * cause them to exit when they try to do a read or write 
     */

    kill_clients(clerk_p);


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <listener_cleanup"));
}


/* -------------------------------------------------------------------------- */


/*
 * Allocate a new child thread, and initialize it 
 */
thread_t 
*alloc_child (link_t    *ll_p,
              thread_t  *parent_p)
{
    int thstatus;
    thread_t *new_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >alloc_child"));

    new_p = (thread_t *)dns_get_buffer(clerk_thread_bt,
                sizeof(thread_t)+sizeof(ucp_resp_header)+MAX_MSG_SIZE);
    if (new_p != NULL) 
    {
        ZERO_bytes(new_p, sizeof(*new_p));

	if ((thstatus = dthread_create_mutex(&new_p->mutex)) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
        }

	if ((thstatus = dthread_create_condition(&new_p->taRspCond)) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
        }

        new_p->clerkRspBuf_p = SKIP_bytes(new_p, sizeof(thread_t));
        new_p->clerkRspMsg_p =
                SKIP_bytes(new_p->clerkRspBuf_p, sizeof(ucp_resp_header));
        new_p->clerkRspBufLen = MAX_MSG_SIZE;

        if (parent_p) {
	    new_p->clerkReq.user_uid = parent_p->clerkReq.user_uid;
	    new_p->clerkReq.user_gid = parent_p->clerkReq.user_gid;
            new_p->clerkReq.UnAuth = parent_p->clerkReq.UnAuth;
            new_p->clerkReq.TrustAll = parent_p->clerkReq.TrustAll;
            new_p->user = parent_p->user;
            /* Inherit parents timeout */
            new_p->expire = parent_p->expire;

#ifdef DCE_SEC

            new_p->login_context = parent_p->login_context;

#endif

        }
    
        /* Add to list of threads */
        if ((new_p->link_p = ll_p) != NULL) {

            if ((thstatus = dthread_lock(&(ll_p->ll_mutex))) < 0)
            {
	      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
            }

	    /* Beginning of the 'protected code' */

                if ((new_p->next_p = ll_p->ll_thread_p) != NULL) 
		{
                    new_p->next_p->prev_p = new_p;
                }

                ll_p->ll_thread_p = new_p;
                ll_p->ll_handler_count++;

	    /* End of the 'protected code' */

	    if ((thstatus = dthread_unlock(&(ll_p->ll_mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	    }
        }
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <alloc_child"));

    return(new_p);
}


/* -------------------------------------------------------------------------- */


/*
 * Remove thread from child list and clean up resources
 */
void 
free_child (thread_t *th_p)
{
    link_t        *ll_p = th_p->link_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >free_child"));
  
    /* Remove myself from the list */

    if ((thstatus = dthread_lock(&(ll_p->ll_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */

        if (th_p->next_p) 
	{
            th_p->next_p->prev_p = th_p->prev_p;
        }

        if (th_p->prev_p) 
	{
            th_p->prev_p->next_p = th_p->next_p;
        } 
	else 
	{
            ll_p->ll_thread_p = th_p->next_p;
        }
  
        if (--ll_p->ll_handler_count == 0) 
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

    if (th_p->clerkReqBuf_p) {
        dns_free((char *)th_p->clerkReqBuf_p);
    }

    if (th_p->taRspBuf_p) {
        dns_free_buffer((char *)th_p->taRspBuf_p);
    }

    if ((thstatus = dthread_delete_condition(&th_p->taRspCond)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_delete_mutex(&th_p->mutex)) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
    }

    dns_free_buffer((char *)th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <free_child"));
}
