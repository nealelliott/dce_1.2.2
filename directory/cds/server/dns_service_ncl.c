/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dns_service_ncl.c,v $
 * Revision 1.1.12.2  1996/02/18  19:36:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:39  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:30:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:12  root]
 * 
 * Revision 1.1.10.8  1994/09/06  17:37:08  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:30:33  proulx]
 * 
 * Revision 1.1.10.7  1994/08/24  20:14:22  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:08:25  mccann]
 * 
 * Revision 1.1.10.6  1994/08/03  19:04:09  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:24:59  mccann]
 * 
 * Revision 1.1.10.5  1994/06/23  19:11:00  jd
 * 	Remove evtblk. It no longer exists
 * 	[1994/06/23  15:41:11  jd]
 * 
 * Revision 1.1.10.4  1994/06/09  18:43:23  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:54  devsrc]
 * 
 * Revision 1.1.10.3  1994/05/12  21:12:19  peckham
 * 	Use INIT_time_quad() macro.
 * 	[1994/05/12  19:18:10  peckham]
 * 
 * Revision 1.1.10.2  1994/04/14  14:51:18  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:20:32  peckham]
 * 
 * Revision 1.1.10.1  1994/03/12  22:11:21  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:42:29  peckham]
 * 
 * Revision 1.1.8.1  1993/09/24  16:56:00  peckham
 * 	OT#8390: Purge unused status codes
 * 	    Removed stale conditionalized program code.
 * 	[1993/09/24  16:54:57  peckham]
 * 
 * Revision 1.1.5.2  1992/12/30  14:21:43  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:21:52  zeliff]
 * 
 * Revision 1.1.2.7  1992/06/03  01:12:35  keegan
 * 	Fix OT 3836: if can't start advertiser thread, exit server with error msg.
 * 	[1992/06/02  18:16:23  keegan]
 * 
 * Revision 1.1.2.6  1992/05/14  17:25:23  keegan
 * 	Return DNS_SUCCESS in 'user_setup' on good return.
 * 	[1992/05/14  16:05:59  keegan]
 * 
 * Revision 1.1.2.5  1992/05/13  12:32:30  keegan
 * 	Function 'user_setup' now checks for errors from 'net_LocalName' and
 * 	returns an error itself.  'DNS_NAMESERVER error returned if user_setup
 * 	fails.
 * 	[1992/05/12  20:17:26  keegan]
 * 
 * Revision 1.1.2.4  1992/05/05  14:49:04  keegan
 * 	Make sure the root directory in a newly created clearinghouse is
 * 	accessible before exporting the server's bindings to the clearinghouse
 * 	object.
 * 	[1992/05/05  13:26:12  keegan]
 * 
 * Revision 1.1.2.3  1992/04/29  18:10:48  keegan
 * 	Fix race condition in 'cdsd' initialization between security initialization
 * 	thread and namespace creation thread.
 * 	[1992/04/29  14:33:21  keegan]
 * 
 * Revision 1.1.2.2  1992/03/22  22:30:24  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:35:02  weisman]
 * 
 * Revision 1.1  1992/01/19  15:26:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module dns_service_ncl.c
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

#include <server.h>
#include <dce/assert.h>
#include <netdb.h>
#include <clerk.h>
#include <names.h>

#include <dce/server_ncl_defs.h>
#include <ta.h>

#include <dce/cds_clerkserver.h>
#include <dce/dce_cf.h>
#include <dce/utc.h>


/*
 *  DTSS UTC base time is October 15, 1582 (Gregorian).
 *  DNS base time is November 17, 1858 (Smithsonian).
 */
static time_quad_t base = INIT_time_quad(0x0135886Au, 0xC7960000u);
static time_quad_t nulltime = INIT_time_quad(0, 0);

/* local function prototypes */

static dns_status_t
probe_root_dir (
    nsgbl_ch_t     *);

int
dns_cvt_status (
    dns_status_t    );

static int
user_setup (
    user_descriptor_t	*,
    nsgbl_ch_t		*);


/*
 * dns_initialize
 *
 * Handle request to initialize a namespace on the server.
 *
 * ns_p :         (IN) address of nameserver mgmt data
 * nick_p :       (IN) Ptr. to nickname of new namespace (opaque simple name)
 * rootch_p :     (IN) Ptr. to name of new root clearinghouse (opaque full name)
 * cts_p :       (OUT) CTS of created namespace
 *
 */
int 
dns_initialize (nsgbl_ns_t     *ns_p,
                SimpleName_t   *rootch_p,
                ObjUID_t       *uid_p)
{
    DEB_ASCII_BUF_nsgbl_ns(nsBuf)
    DEB_ASCII_BUF_SimpleName(rootchBuf)
    DEB_ASCII_BUF_ObjUID(uidBuf)
    struct nsgbl_ch    *ch_p;
    struct buf {
	user_descriptor_t b_user;
    } *buf_p;
    dns_status_t        status;
    bytes_u 		mytower[READ_BUF_SIZE];
    db_stream_t         *stream_p;
    file_info_t		*file_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >dns_initialize(%s,%s,%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        deb_ascii_SimpleName(rootchBuf,rootch_p),
        deb_ascii_ObjUID(uidBuf,uid_p)));

    buf_p = (struct buf *)dns_malloc(sizeof(struct buf));
    if (!buf_p) {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
            cds_svc_server,
	    svc_c_debug1,
	    "  dns_initialize: NoResources"));

	status = CDS_NONSRESOURCES;
	goto leave_dns_initialize;
    }

    /*
     * lock the memory section 
     */

    if ((thstatus = dthread_lock(&(ns_p->ns_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        { /* b */
   
        ch_p = nsgbl_ch_malloc(ns_p);
        if (ch_p)
            {
	    /* fill in the user context */
	    ZERO_bytes(&buf_p->b_user, sizeof(buf_p->b_user));
	    if (user_setup(&buf_p->b_user, ch_p) != DNS_SUCCESS) {
	        status = DNS_NAMESERVERBUG;

		DCE_SVC_DEBUG((
		    cds__svc_handle,
                    cds_svc_server,
	            svc_c_debug1,
          	    "  dns_initialize: user_setup failed - status = DNS_NAMESERVERBUG"));

		goto error_condition;
	      }
		
            status = mgmt_create_namespace(ns_p, &buf_p->b_user,
		ch_p, rootch_p);

            if (status == DNS_SUCCESS)
                {
		COPY_ObjUID(&ch_p->ch_nsuid, uid_p);
                /* Add file name of new clearinghouse to the the file
                 * "dns_files.txt"
                 */
                status = sys_mgmt_file_add(ns_p, ch_p->ch_filename);
                if (status != DNS_SUCCESS)
		{
		    DCE_SVC_DEBUG((
   		       cds__svc_handle,
                       cds_svc_server,
	               svc_c_debug1,
          	       "  dns_initialize: sys_mgmt_file_add failed"));

                    goto error_condition;
		}
		
                /* set clearinghouse state to ON */

		if ((thstatus = dthread_lock(&(ch_p->ch_mutex))) < 0)
		{
		    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, 
				   (long)thstatus);
		}

		ch_p->ch_state = dns_On;

		if ((thstatus = dthread_broadcast(&(ch_p->ch_state_changed))) < 0)
		{
		    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, 
				   (long)thstatus);
                }

		if ((thstatus = dthread_unlock(&(ch_p->ch_mutex))) < 0)
		{
		    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, 
				   (long)thstatus);
		}

                /*
                 * Send info about new clearinghouse to advertiser.
                 */
                adver_signal_new_info(ns_p->ns_advertiser);

	    } /* end if (status == DNS_SUCCESS) */
            else
                nsgbl_ch_free(ch_p);

	} /* end if (ch_p) */
	
        else
            status = DNS_NONSRESOURCES;
    }
error_condition:

    /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(ns_p->ns_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, 
				   (long)thstatus);
    }

    /*
     * If we successfully created the namespace, wait for the advertisement
     * to be sent out.  This will allow the configuration procedure to
     * "know" that the clerk has been informed of the new namespace.
     *
     * Since the advertiser pinger thread needs access to the set of
     * clearinghouses, we must first release the ns_mutex or we will
     * cause a deadlock.
     */
    if (status == DNS_SUCCESS)
        {
        adver_wait_for_advertisement(ns_p->ns_advertiser);

	/* Make sure we can read the root directory before we continue.*/
	status = probe_root_dir(ch_p);
	if (status != DNS_SUCCESS) 
	{
	    LOG_ASCII_BUF_dns_status(statusBuf)

	    dce_svc_printf(CDS_S_SERVER_NEWREAD_EB_MSG,
			   deb_ascii_dns_status(statusBuf,status));
	  }

	/* After advertiser is with it, update local towers */

	if (!net_export_ch(ch_p)) {

	  /* If we can't export our bindings to the clearinghouse,
	   * then acl_edit isn't going to be able to bind to it,
	   * and dce_config won't succeed, so stop now!
	   */

	    dce_svc_printf(CDS_S_SERVER_EXPORTFAIL_EB_MSG);
	}

	/*
	 * Get what address should be 
	 */
	if (towerset_build(mytower)) 	
	{
	    file_p = db_find_file(&ch_p->ch_uid);
	    if (file_p) 
	    {
		/*
		 * Read in pseudo directory 
		 */
		if (db_get_stream(file_p, &stream_p, 
				  OP_ReadAttribute) == DNS_SUCCESS) 
		{
		    if (db_pseudo_read_for_update(stream_p) == DNS_SUCCESS) 
			(void)write_chlastaddress(stream_p, (Set_t *)mytower);

		    db_free_stream(stream_p);
		}
	    }
	}
	else 
	{ 
	    dce_svc_printf(CDS_S_SERVER_TOWERFAIL_EB_MSG);
	}
    }

    dns_free((char *)buf_p);

leave_dns_initialize:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <dns_initialize(%s,%s,%s) return(%d)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        deb_ascii_SimpleName(rootchBuf,rootch_p),
        deb_ascii_ObjUID(uidBuf,uid_p),
        status));

    return (status);
}

/*
 * dns_enable
 *
 * Enable the nameserver entity
 * start the advertiser thread
 *
 * ns_p : (IN) address of nameserver mgmt data
 *
 */
int 
dns_enable (nsgbl_ns_t     *ns_p)
{
    DEB_ASCII_BUF_nsgbl_ns(nsBuf)
    struct nsgbl_ch    *ch_p;
    dns_status_t        status;
    int thstatus;
    LOG_ASCII_BUF_FullName(fullnameBuf)


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >dns_enable(%s,%lu)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p)));

    if (ns_p->ns_state == dns_On) {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
            cds_svc_server,
	    svc_c_debug1,
	    "  dns_enable: DNS_RESP_SUCCESS"));

	status = CDS_SUCCESS;
	goto leave_dns_enable;
    }

    status = net_open(ns_p, &ns_p->ns_ta_id);
    if (status != DNS_SUCCESS)
	{
	if (ns_p->ns_ta_id)
	    {
	    (void)net_close(ns_p->ns_ta_id);
	    ns_p->ns_ta_id = 0;
	    }

	DCE_SVC_DEBUG((
	    cds__svc_handle,
            cds_svc_server,
	    svc_c_debug1,
	    "  dns_enable: net_open failed - %d",status));

	goto leave_dns_enable;
	}
    /*
     * lock the memory section 
     */

    if ((thstatus = dthread_lock(&(ns_p->ns_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        {  /* Beginning of the 'protected code' */

        /*
         * Initialize the advertiser pinger control block and then
         * start the adver thread
         */
	ns_p->ns_advertiser = adver_start(ns_p);

        if (!ns_p->ns_advertiser)
            {
            /*
             * close the net too
             */
            if (ns_p->ns_ta_id)
		{
		(void)net_close(ns_p->ns_ta_id);
		ns_p->ns_ta_id = 0;
		}
	    if ((thstatus = dthread_unlock(&(ns_p->ns_mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	    }

            status = CDS_NONSRESOURCES;

	    DCE_SVC_DEBUG((
  	       cds__svc_handle,
               cds_svc_server,
	       svc_c_debug1,
	       "  dns_enable: adver_start() failed - %d", status));

	    goto leave_dns_enable;
            }
            
	ns_p->ns_state = dns_On;
	
	if ((thstatus = dthread_broadcast(&(ns_p->ns_state_changed))) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	}

    } /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(ns_p->ns_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    sys_mgmt_file_load(ns_p);

    /* Reload each clearinghouse from disk */     
    ch_p = NULL;
    while (TRUE) 
    {
	ch_p = nsgbl_ch_next(ns_p, ch_p);
	if (!ch_p) break;
	status = db_setup(ch_p);

	if (status != DNS_SUCCESS) 
	{
	  dce_svc_printf(CDS_S_SERVER_LOADFAIL_EB_MSG, (long)status);
	}
    }

    if ((thstatus = dthread_lock(&(ns_p->ns_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{ /* lock */

	    /* 
	     * Enable all the clearinghouses
	     */

	    /*
	     * Wait for security initialization to complete.
	     */

#if defined(DCE_SEC)

	    if ((thstatus = dthread_lock(&(ns_p->ns_sec_mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	    }

	    /* Beginning of the 'protected code' */
	    
	    while (ns_p->ns_sec_state != cds_sec_On) 
              if ((thstatus = dthread_wait(&(ns_p->ns_sec_state_changed),
                                           &(ns_p->ns_sec_mutex))) < 0)
              {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
              }
    
	    /* End of the 'protected code' */

	    if ((thstatus = dthread_unlock(&(ns_p->ns_sec_mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	    }

#endif
	ch_p = NULL;
	while (TRUE)
	    {
		ch_p = nsgbl_ch_next(ns_p, ch_p);
		if (!ch_p) break;
                status = db_enable(ch_p, TRUE);

                /* log an event if successful and inform advertiser */        
                if (status == DNS_SUCCESS)
		{
		    
		    if ((thstatus = dthread_lock(&(ch_p->ch_mutex))) < 0)
		    {
		      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
		    }
		
		    /* Beginning of the 'protected code' */
		    ch_p->ch_state = dns_On;

		    if ((thstatus = dthread_broadcast(&(ch_p->ch_state_changed))) < 0)
		    {
		      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, 
				     (long)thstatus);
		    }

		    /* End of the 'protected code' */

		    if ((thstatus = dthread_unlock(&(ch_p->ch_mutex))) < 0)
		    {
		      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
		    }


		} /* end if (status == DNS_SUCCESS) */

	    } /* end while */

	    /* 
	     * Tell advertiser about server and any clearinghouses
	     * currently enabled 
	     */
	    adver_signal_new_info(ns_p->ns_advertiser);

	}
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(ns_p->ns_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
    
leave_dns_enable:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <dns_enable(%s) return(%d)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        status));

    return(status);
}

/*
 * Allocate/free clearinghouse entry structure
 *
 * NOTE: assumes parent namespace mutex is locked!
 */
nsgbl_ch_t 
*nsgbl_ch_malloc (nsgbl_ns_t *ns_p)
{
    register nsgbl_ch_t	*ch_p;
    int thstatus;

    ch_p = (nsgbl_ch_t *)dns_malloc(sizeof(*ch_p));
    if (ch_p)
	{
	ZERO_bytes(ch_p, sizeof(*ch_p));
	ch_p->ch_state = dns_Off;
	COPY_SimpleName(NullSimpleName, &ch_p->ch_nsnickname);
	COPY_FullName(NullFullName, &ch_p->ch_name);
	COPY_FullName(NullFullName, &ch_p->ch_cellname);
        init_list(&ch_p->ch_alt_chcts);

	if (0 <= dthread_create_mutex(&ch_p->ch_mutex))
	    {
	    if (0 <= dthread_create_condition(&ch_p->ch_state_changed))
		{
		int myLock = dthread_trylock(&ns_p->ns_mutex);

		insert_list(&ns_p->ns_clearinghouses,&ch_p->ch_clearinghouses);
		ns_p->ns_clearinghouse_count++;

		if (myLock)
		{
		    if ((thstatus = dthread_unlock(&(ns_p->ns_mutex))) < 0)
		    {
		      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
		    }
		}
		ch_p->ch_nameserver = ns_p;
		return(ch_p);
		}

	    if ((thstatus = dthread_delete_mutex(&ch_p->ch_mutex)) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
	    }
	}
	dns_free((char *)ch_p);
    }
    return(NULL);
}

void 
nsgbl_ch_free (nsgbl_ch_t *ch_p)
{
    nsgbl_ns_t *const ns_p = ch_p->ch_nameserver;
    alt_chcts_list_t *alt_chcts_p;
    int thstatus;

    if (ns_p)
    {
	int myLock = dthread_trylock(&ns_p->ns_mutex);

	remove_list(&ch_p->ch_clearinghouses);
	if (--ns_p->ns_clearinghouse_count == 0)
	{
	    if ((thstatus = dthread_broadcast(&(ns_p->ns_state_changed))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	    }
	}
	
	if (myLock)
	    if ((thstatus = dthread_unlock(&(ns_p->ns_mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	    }
    }
    
	
    if ((thstatus = dthread_delete_mutex(&ch_p->ch_mutex)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_delete_condition(&ch_p->ch_state_changed)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
    }

    /* Get rid of any alternate CHCTS's that we have been using */
    alt_chcts_p = (alt_chcts_list_t *)LIST_FIRST(&ch_p->ch_alt_chcts); 

    while (alt_chcts_p)
        {
        alt_chcts_list_t *save_alt_chcts_p = alt_chcts_p;
        alt_chcts_p = (alt_chcts_list_t *)LIST_NEXT(&ch_p->ch_alt_chcts, alt_chcts_p);
        dns_free((char *)save_alt_chcts_p);
        }

    if (ch_p->ch_cellname_p) free(ch_p->ch_cellname_p);

    dns_free((char *)ch_p);

}

/*
 * Get next clearinghouse; use as follows:
 *
 *	nsgbl_ch_t *ch_p;
 *
 *	SERVER_LOCK(ns_p->ns_mutex) {
 *	    for (ch_p = NULL; ch_p = nsgbl_ch_next(ns_p, ch_p); )
 *		{do things for this clearinghouse}
 *	} SERVER_END_LOCK(ns_p->ns_mutex);
 *
 * NOTE: assumes parent namespace mutex is locked!
 */
nsgbl_ch_t 
*nsgbl_ch_next (nsgbl_ns_t  *ns_p,
                nsgbl_ch_t  *ch_p)
{
    if (!ch_p)
	ch_p = (nsgbl_ch_t *)ns_p->ns_clearinghouses.next_p;
    else
	ch_p = (nsgbl_ch_t *)ch_p->ch_clearinghouses.next_p;

    if (ch_p == (nsgbl_ch_t *)&ns_p->ns_clearinghouses)
	ch_p = NULL;

    return(ch_p);
}

static int 
user_setup (user_descriptor_t  *user_p,
            nsgbl_ch_t         *ch_p)
{
    dthread_assign(dthread_null, &user_p->ud_mutex);
    user_p->ud_ch_p = ch_p;
    user_p->ud_ns_p = ch_p->ch_nameserver;
    user_p->ud_chuid_p = (ObjUID_t *)&ch_p->ch_uid;
    user_p->ud_protocol_version = MAJORVER;

    return (DNS_SUCCESS);
}

/*
 * probe_root_dir
 *
 * This function attempts to read the CDS_CTS attribute from the
 * root replica.  If it fails, several more attempts are made (with
 * an appropriate pause in between) and if these still fail, an
 * error is returned.
 * Inputs:  nameserver nsgbl_ch structure
 * Returns: dns_status_t
*/
#define NUM_ATTEMPTS 15   /* number of attempts to read root    */
#define PROBE_INTERVAL 1  /* interval, in secs., between reads  */

static dns_status_t 
probe_root_dir (nsgbl_ch_t *ch_p)
{
    DEB_ASCII_BUF_nsgbl_ch(chBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_handle handle;
    dns_status_t status = DNS_NONSRESOURCES;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >probe_root_dir(%s)",
        deb_ascii_nsgbl_ch(chBuf,ch_p)));

    handle = dnsGetHandle();
    if (handle) {
	register int c = NUM_ATTEMPTS;

        while (TRUE) {
	    DEB_ASCII_BUF_AttributeName(aBuf)
	    const AttributeName_t *const an_p = dns_AttributeName(DNS_CTS);
	    dns_attrvalue_t *value;
	    dnsFlagStat_t flags;

          DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_server,
              svc_c_debug5,
              " >probe_root_dir(%s) attempt #%d to read %s from root",
              deb_ascii_nsgbl_ch(chBuf,ch_p),
              NUM_ATTEMPTS+1-c,
              deb_ascii_AttributeName(aBuf,an_p)));

	    ZERO_bytes (&flags, sizeof (flags));
	    flags.fsNameType = dnsOpqDNA;
	    status = dnsReadAttrValue ((dns_full_name_t *)&ch_p->ch_cellname,
					  handle,
					  (dns_attr_name_t *)an_p,
					  cdsDir,
					  (dns_full_name_t *)&ch_p->ch_name,
					  &value,
					  (dns_cts **)NULL,
					  (void *)NULL,
					  (int *)0,
					  &flags);

	    if (status == DNS_SUCCESS)
		break;

	    if (0 < --c)
		(void)dthread_delay((dthread_interval_t)PROBE_INTERVAL);
	    else {
		status = flags.fsLocStat;
		break;
	    }
	}
	dnsFreeHandle(handle);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <probe_root_dir(%s) return(%s)",
        deb_ascii_nsgbl_ch(chBuf,ch_p),
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
}



