/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_adver.c,v $
 * Revision 1.1.9.2  1996/02/18  19:36:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:26  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:32:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:57  root]
 * 
 * Revision 1.1.7.9  1994/08/24  20:14:59  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:09:31  mccann]
 * 
 * Revision 1.1.7.8  1994/08/03  19:04:41  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:28:26  mccann]
 * 
 * Revision 1.1.7.7  1994/06/09  18:43:55  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:28  devsrc]
 * 
 * Revision 1.1.7.6  1994/05/25  22:22:53  peckham
 * 	Clean up debugging.
 * 	Fix main loop so that a socket error will try to re-open
 * 	the socket to the advertiser.
 * 	[1994/05/25  22:20:42  peckham]
 * 
 * Revision 1.1.7.5  1994/04/29  15:56:16  peckham
 * 	Use dcelocal_path in file names.
 * 	Use dcecdsmac.h macros for serviceability.
 * 	[1994/04/29  14:37:36  peckham]
 * 
 * Revision 1.1.7.4  1994/04/19  17:06:25  mccann
 * 	Merge and try again
 * 	[1994/04/19  16:53:16  mccann]
 * 
 * 	More code cleanup
 * 
 * Revision 1.1.7.3  1994/04/14  14:51:33  peckham
 * 	Restore dnsadverdef.h include
 * 	[1994/04/14  14:50:09  peckham]
 * 
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:22:21  peckham]
 * 
 * Revision 1.1.7.2  1994/03/22  20:00:35  griffin
 * 	HCell BL3 support: Modified advertising mechanism to emit advertisements
 * 	for each cell name (alias) that is active for the cell.  This
 * 	change corresponds to a change in the advertiser which understands
 * 	this condition and keeps them in its cache.
 * 	[1994/03/22  18:16:08  griffin]
 * 
 * Revision 1.1.7.1  1994/03/12  22:12:39  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:45:51  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:23:34  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:24:10  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/14  20:52:31  mfox
 * 	Fixes to allow CDS to compile cleanly with RIOS ANSI C compiler
 * 	[1992/04/14  19:44:10  mfox]
 * 
 * Revision 1.1  1992/01/19  15:24:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module ta_adver.c
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
 *
 * Module Description
 *
 * This module builds the messages containing the set of clearinghouses
 * and namespaces that are active on the nameserver and sends them to
 * the advertiser to put over the ethernet and 802.3.
 *
 *      One message is built for each namespace served, and
 *      if all CH's in that namespace won't fit, multiple messages
 *      for each namespace might be built.
 *
 */



/*
 *  Include files
 */

#include <server.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <dbdef.h>
#include <sys_mbx.h>
#include <dns_strings.h>
#include <replica.h>
#include <hcell.h>
#include <names.h>
#include <adver_ipc.h>

/*
 * Mutex lock/unlock
 */
#ifndef DNS_LOCK
# define DNS_LOCK(m) { const int thstatus = dthread_lock(m); \
	if (thstatus < 0) dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus); }
#define DNS_END_LOCK(m) { const int thstatus = dthread_unlock(m); \
	if (thstatus < 0) dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus); }
#endif

/*
 * Bugcheck handling
 */
#define MULTIPLE_ADVER_THREADS -601

/*
 * linked list of messages
 */
typedef struct ta_adver_msg
    {
    struct ta_adver_msg    *next_p;
    AdvertiseRequest_t      adver_msg;
    }ta_adver_msg_t;

/*
 * build datastructure to hold mutex declaration
 */
typedef enum {
    is_dead = 0,
    is_alive,
    is_aborting
} ping_state_t;

typedef struct adver_ctrl
    {
    dthread_t		adver_thread;
    dthread_t		adver_monitor_thread;
    dthread_mutex_t	adver_mutex;
    dthread_cond_t	adver_c_var;
    dthread_cond_t	adver_change_advertised;
    sys_mailbox		adver_mbx;
    nsgbl_ns_t		*adver_ns_p;
    ping_state_t	adver_state;
    diag_state_t	adver_message_flow;
    int			adver_sweep_interval;
    boolean		adver_new_info;
    boolean		adver_monitor_on;
    boolean		adver_mbx_connected;
    } adver_ctrl_t;

/*
 * advertiser message format
 */
typedef struct
    {
    bytes_u                 g_majorVersion[1];
    bytes_u                 g_minorVersion[1];
    bytes_u                 g_userMajor[1];
    bytes_u                 g_userMinor[1];
    MessageType_u           g_message_type;
    ObjUID_u                g_ns_uid;
    SimpleName_u            g_ns_nickname;
    } g_message_t;

/* simple name followed by a 16 bit integer count, and then
 * that many replica pointers
 */


/*
 * Local data
 */
static diag_state_t message_flow_template = diag_normal;
static dthread_attr_t monitor_advertiser_attr = {"ta_monitor_advertiser"};

/*
 * Forward declarations and prototypes
 */

char *
ping_state_ascii (ping_state_t);

static dthread_address_t 
ta_ping_advertiser (adver_ctrl_t *adv_p);

static dthread_address_t 
ta_monitor_advertiser (adver_ctrl_t *adv_p);

static void
cleanup_adver_msg (
    ta_adver_msg_t      *);

static void
build_adver_msg (
    FullName_t          *,        /* in   - Clearinghouse name      */
    byte_t              *,        /* in   - Clearinghouse towers    */
    ObjUID_t            *,        /* in   - Clearinghouse UID       */
    SimpleName_t        *,        /* in   - Cell Name               */
    ObjUID_t            *,        /* in   - Cell UID                */
    AdvertiseRequest_t  *);       /* out  - Formatted advertisement */

static dns_status_t
send_adver_msg (
    adver_ctrl_t	*);

static dns_status_t
send_reset_msg (
    adver_ctrl_t	*);

static dns_status_t
send_ping_msg (
    adver_ctrl_t	*);

static dns_status_t
send_quit_msg (
    adver_ctrl_t	*);

static void
adver_cell_uid (
    nsgbl_ch_t		*,
    ObjUID_t		*);

static void
cleanup_mutex (
    void		*);

/* -------------------------------------------------------------------------- */

/*
 * Ascii interpretation of ping_state_t variables.
 */

char *
ping_state_ascii (ping_state_t state)
{
    switch (state)
        {
        case is_dead:     return "Dead     "; break;
        case is_alive:    return "Alive    "; break;
        case is_aborting: return "Aborting "; break;
        default:          return "*INVALID*"; break;
        }
}



/* -------------------------------------------------------------------------- */

/*
 * adver_cell_uid
 *      Get a stream to the ch_cellname directory
 */

static void
adver_cell_uid (
    nsgbl_ch_t          *ch_p,
    ObjUID_t            *uid_p)
{
    DEB_ASCII_BUF_nsgbl_ch(chBuf)
    DEB_ASCII_BUF_ObjUID(uidBuf)
    db_stream_t         *stream_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >adver_cell_uid(%s,)\n",
        deb_ascii_nsgbl_ch(chBuf,ch_p)));

    if (db_open_stream(&ch_p->ch_uid, &stream_p, OP_ReadAttribute)
                == DNS_SUCCESS) {
        COPY_FullName(&ch_p->ch_cellname, stream_p->db_key);

        if (db_dir_read(stream_p) == DNS_SUCCESS) {
            COPY_ObjUID(stream_p->db_dir.data_uid, uid_p);
        } else {
            COPY_ObjUID(NullFullName, uid_p);
        }

        db_close_stream(stream_p);
    } else {
        COPY_ObjUID(NullFullName, uid_p);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <adver_cell_uid(%s,%s)\n",
        deb_ascii_nsgbl_ch(chBuf,ch_p), deb_ascii_ObjUID(uidBuf,uid_p)));
}


/* -------------------------------------------------------------------------- */

/*
 * ta_ping_advertiser
 *     control program for thread that formats and sends multicast messages to
 *     advertiser, send only a ping, if the data hasn't changed
 *
 * Input:
 *     uses management area to get namespace/clearinghouse combinations
 *
 * Output:
 *     writes to mailbox or socket
 *     re-sets timer to awaken self later
 *
 * Value:
 */
static dthread_address_t 
ta_ping_advertiser (adver_ctrl_t *adv_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    time_local_t expiration_time;
    char filename[_DNS_MAXPATHLEN+1];
    dns_status_t status;
    int flow;
    int thstatus;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >ta_ping_advertiser(%p)", adv_p));

    /* already running */
    if (adv_p->adver_state != is_dead)
      return(NULL);

    /*
     * initialize the resources needed by this thread to run
     * this thread basically runs 'forever' or until the
     * server is disabled
     */
    (void)sprintf(filename, CDS_ADVER_SOCK, dcelocal_path);


    DNS_LOCK(&adv_p->adver_mutex);

    /* initialize the rest of the adver control datastructure */
    adv_p->adver_state = is_alive;
    /* Initialize diagnostic state */
    flow = server_diag_check_setting(&adv_p->adver_message_flow);

    DNS_END_LOCK(&adv_p->adver_mutex);

    sys_time_from_interval(&expiration_time, 0, adv_p->adver_sweep_interval,0);

    while (TRUE) {
	dthread_date_time_t timeout;
	boolean changed;

	/*
	 * Wait for either the sweep timer to expire or someone signals
	 * that we should die.
	 */
	dthread_get_expiration(&expiration_time, &timeout);

	DNS_LOCK(&adv_p->adver_mutex);

	dthread_cleanup_push(cleanup_mutex, &adv_p->adver_mutex);

	while (TRUE) {
	    int errnum;

	    if (adv_p->adver_state == is_aborting)
		adv_p->adver_state = is_dead;

	    if (adv_p->adver_state == is_dead)
		break;	  /* quitting time */
	    
	    if (adv_p->adver_new_info) {
		break;              /* new data to advertise */
	    }

	    if ((thstatus = dthread_timed_wait(&adv_p->adver_c_var,
				    &adv_p->adver_mutex, &timeout)) < 0) {
		if ((errnum = dthread_get_errno()) == EAGAIN) {
		    break;          /* timeout */
		}

		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
	    }

	    flow = server_diag_check_setting(&adv_p->adver_message_flow);
	    if (flow) {
		break;              /* Diagnostics are active */
	    }

	}

	/*
	 * check for changes, message flow inside lock and carry outside
	 */
	changed = adv_p->adver_new_info;
	adv_p->adver_new_info = FALSE;

	dthread_cleanup_pop(1);

	if (adv_p->adver_state == is_dead)
	    break;

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_server,
	    svc_c_debug4,
	    "  ta_ping_advertiser(%p) pinging advertiser", adv_p));

	if (!adv_p->adver_mbx_connected) {
	    /* create the mbx or socket connection to advertiser */
	    status = sys_openmbx ((unsigned char *)filename,
		sizeof(struct AdvertiseRequest_t),
		NULL,
		0,      /* clear block mode */
		dthread_attr_default,
		&adv_p->adver_mbx);

	    if (status != DNS_SUCCESS) {
		/*
		 * Speed up checking
		 */
		sys_time_from_interval(&expiration_time, 0, 0, 2);
		continue;
	    }

	    adv_p->adver_mbx_connected = TRUE;
	    /*
	     * Back to regular sweep interval
	     */
	    sys_time_from_interval(&expiration_time, 0,
				   adv_p->adver_sweep_interval, 0);
	    /*
	     * Start monitoring process
	     */
	    if (0 <= dthread_create(&adv_p->adver_monitor_thread,
				    &monitor_advertiser_attr,
				    (dthread_start_routine)ta_monitor_advertiser,
				    (dthread_address_t)adv_p)) {
		adv_p->adver_monitor_on = TRUE;
	    }

	    changed = TRUE;
	}

	/*
	 * Advertise based on diagnostic flow setting
	 */

	if (flow == 0) {			/* Normal processing */
	    if (changed) {
		status = send_adver_msg(adv_p);
	    } else {
		status = send_ping_msg(adv_p);
	    }
	} else if (flow > 0) {		/* Force advertisements */
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_server,
		svc_c_debug4,
		"  ta_ping_advertiser(%p) - forcing advertisements", adv_p));

	    status = send_adver_msg(adv_p);
	} else if (flow < 0) {		/* Inhibit advertisements */
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_server,
		svc_c_debug4,
		"  ta_ping_advertiser(%p) - advertisements are inhibited", adv_p));
	}

	/*
	 * If the advertiser has dropped out,
	 * close the mailbox.
	 */
	if (!adv_p->adver_mbx_connected) {
	    if (adv_p->adver_monitor_on) {
		dthread_address_t join_status;

		dthread_cancel(adv_p->adver_monitor_thread);
		dthread_join(adv_p->adver_monitor_thread, &join_status);
		dthread_detach(&adv_p->adver_monitor_thread);
		adv_p->adver_monitor_on = FALSE;
	    } else
		status = sys_closembx (&adv_p->adver_mbx);

	    /*
	     * Speed up checking
	     */
	    sys_time_from_interval(&expiration_time, 0,
				   1, 0);
	}
    }

    if (adv_p->adver_mbx_connected) {

	if (adv_p->adver_monitor_on) {
	    dthread_address_t join_status;

	    /*
	     * send a stop advertising message
	     */
	    (void)send_quit_msg(adv_p);
	
	    dthread_cancel(adv_p->adver_monitor_thread);
	    dthread_join(adv_p->adver_monitor_thread, &join_status);
	    dthread_detach(&adv_p->adver_monitor_thread);
	    adv_p->adver_monitor_on = FALSE;
	} else
	    (void)sys_closembx (&adv_p->adver_mbx);

	adv_p->adver_mbx_connected = FALSE;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <ta_ping_advertiser(%p) return(NULL)", adv_p));

    /*
     * signal that we're exiting
     */
    return(NULL);
} /* end of routine */

/*
 * Monitor the mailbox to the advertiser and report
 * if the link goes down.
 */
static dthread_address_t 
ta_monitor_advertiser (adver_ctrl_t *adv_p)
{
    byte_t buffer[2];
    int length = sizeof(buffer);
    int thstatus;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug8,
        " >ta_monitor_advertiser(%p)", adv_p));

    /*
     * This read should never succeed.
     *
     * If it is unblocked by a failure,
     * then the link must be down.
     */
    (void)sys_mbx_read(adv_p->adver_mbx, buffer, &length);

    DNS_LOCK(&adv_p->adver_mutex);

    /* No longer connected */
    adv_p->adver_mbx_connected = FALSE;
    adv_p->adver_new_info = TRUE;

    DNS_END_LOCK(&adv_p->adver_mutex);

    (void)sys_closembx (&adv_p->adver_mbx);

    /* Poke the outer loop */
    if ((thstatus = dthread_signal(&adv_p->adver_c_var)) < 0) {
	dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug8,
        " <ta_monitor_advertiser(%p) return(%p)", adv_p, NULL));

    return(NULL);
}


/* -------------------------------------------------------------------------- */


/*
 * send_adver_msg
 *     Formats and sends multicast messages to advertiser.
 *
 * Input:
 *     Uses management area to get namespace/clearinghouse combinations
 *
 * Output:
 *     writes to mailbox or socket
 */
static dns_status_t 
send_adver_msg (adver_ctrl_t *adv_p)
{
    DEB_ASCII_BUF_nsgbl_ns(nsBuf)
    DEB_ASCII_BUF_SimpleName(AliasBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    nsgbl_ns_t *const ns_p = adv_p->adver_ns_p;
    ta_adver_msg_t        *begin_p = NULL;
    ta_adver_msg_t        *prev_p;
    ta_adver_msg_t        *cur_p = NULL;
    nsgbl_ch_t            *ch_p;
    bytes_u                tower[READ_BUF_SIZE];
    ObjUID_t               cell_uid;
    SimpleName_t           cell_name;
    SimpleName_t           alias_name;
    Set_t                 *userset_p;
    int                    num_mem;
    int                    tower_len;
    int                    alias_count;
    int                    alias_index;
    dns_status_t           status = DNS_SUCCESS;
    int thstatus;

    file_info_t           *file_p;
    dir_hash_entry_t      *root_dir_p;
    DBSet_t               *set_p;
    Timestamp_t           *allupto_p;
    FullName_u             tmog_ch_name;   /* Holds transmogrified ch names */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
	" >send_adver_msg(%p)\n",
	adv_p));

    /*
     * get the local address in both forms
     */

    tower_len = towerset_build(tower);
    userset_p = (Set_t *)tower;
    num_mem = NUM_Set(userset_p);

    /*
     * obtain lock on dns_nameserver data structure
     */

    DNS_LOCK(&ns_p->ns_mutex);

    dthread_cleanup_push(cleanup_mutex, &ns_p->ns_mutex);

    /*
     * Loop thru all local clearinghouses
     */
    for (ch_p = (nsgbl_ch_t *)ns_p->ns_clearinghouses.next_p;
	 ch_p != (nsgbl_ch_t *)&ns_p->ns_clearinghouses;
	 ch_p = (nsgbl_ch_t *)ch_p->ch_clearinghouses.next_p) {
	/*
	 * check to see if ch still on
	 */
	if (ch_p->ch_state != dns_On) continue;

	/*
	 * Try to lock onto any aliases that might exist.
	 */

	alias_count = 0;

	file_p = db_find_file(&ch_p->ch_uid);
	if (file_p) {
	    DNS_LOCK(&file_p->file_mutex);

	    {       /* Beginning of the 'protected code' */
		if (file_p->file_root_dir_p) {
		    root_dir_p = file_p->file_root_dir_p;
		}
	    }      /* End of the 'protected code' */

	    DNS_END_LOCK(&file_p->file_mutex);
	}

	if (root_dir_p) {
	    DNS_LOCK(&root_dir_p->dir_mutex);

	    dthread_cleanup_push(cleanup_mutex, &root_dir_p->dir_mutex);
	    {       /* Beginning of the 'protected code' */
		/* Get the cell aliases for this cell */
		set_p = root_dir_p->dir_cellalias_set_p;
		allupto_p = root_dir_p->dir_allupto_p;
		/* ***!!!*** We need to snapshot it to set_p */
		if (set_p != (DBSet_t *) NULL) {
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_server,
			svc_c_debug6,
			"  send_adver_msg(%p) Aliases retrieved.\n",
			adv_p));
		    alias_count = NUM_DBSet(set_p);
		}
	    }      /* End of the protected region */
	    dthread_cleanup_pop(1);
	}

	/*
	 * Now we generate messages for the clearinghouse: one for the
	 * cell name and one for each alias.
	 */

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_server,
	    svc_c_debug6,
	    "  send_adver_msg(%p) Alias count: %d\n",
	    adv_p, alias_count));

	

	for (alias_index = -1; alias_index < alias_count; alias_index++) {

	    if (alias_index < 0) {
		COPY_SimpleName(&ch_p->ch_nsnickname, &cell_name);
	    } else {
		DBSetMember_t *member_p;
		FullName_t    *nameptr_p;

		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_server,
		    svc_c_debug6,
		    "  send_adver_msg(%p) Processing alias %d\n",
		    adv_p, alias_index));

		member_p = (DBSetMember_t *) INDEX_DBSet(set_p,
							 alias_index);
		if (!EXT8(member_p->dm_valid) ||
		    !EXT8(member_p->dm_value_present)) {
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_server,
			svc_c_debug6,
			"  send_adver_msg(%p) Discard %d - inactive.\n",
			adv_p, alias_index));
		    continue;
		}

		/* ***!!! Verify compare is in correct direction */

		if (dnsCmpCTS((dns_cts *)allupto_p,
			      (dns_cts *)member_p->dm_ts) < 0) {
		    DCE_SVC_DEBUG((
			cds__svc_handle,
			cds_svc_server,
			svc_c_debug6,
			"  send_adver_msg(%p) Discard %d - not safe.\n",
			adv_p, alias_index));
		    continue;
		}

		/* Skip over preferred byte */
		nameptr_p = (FullName_t *)SKIP_byte(member_p->dm_data);

		status = cell_full_to_simple(nameptr_p, &alias_name);
		if (status != DNS_SUCCESS) {
		    continue;
		}

		/*
		 * If the alias matches the current cell name (which
		 * we've already emitted, skip it.
		 */

		if (EQ_SimpleName(&ch_p->ch_nsnickname, &alias_name)) {
		    continue;
		}

                    COPY_SimpleName(&alias_name, &cell_name);

		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_server,
		    svc_c_debug6,
		    "  send_adver_msg(%p) Alias cell name is: %s\n",
		    adv_p, deb_ascii_SimpleName(AliasBuf, &alias_name)));

	    }  /* end if */

	    /*
	     * save ptr from previous loop
	     */
	    prev_p = cur_p;

	    /*
	     * first malloc some space to hold the msg
	     */
	    cur_p = (ta_adver_msg_t *)dns_malloc(sizeof(ta_adver_msg_t));
	    if (!cur_p) {
		status = DNS_NONSRESOURCES;
		break;
	    }
	    cur_p->next_p = NULL;
	    if (!begin_p) begin_p = cur_p;       /* catch the first one */
	    if (prev_p) prev_p->next_p = cur_p;  /* link in next buffer */

	    /*
	     * Now build the message
	     * and "transmogrify" the clearinghouse name on the way
	     * out the door.
	     */

	    adver_cell_uid(ch_p, &cell_uid);

	    cell_simple_to_full(&cell_name, (FullName_t *)tmog_ch_name);
	    names_append(names_pointlast(&ch_p->ch_name), 
			 (FullName_t *)tmog_ch_name);
	    
	    build_adver_msg((FullName_t *)tmog_ch_name,
			    &tower[0],
			    &ch_p->ch_uid,
			    &cell_name,
			    &cell_uid,
			    &cur_p->adver_msg);


	}  /* end of alias for-loop */

    } /* end of clearinghouse for-loop */

    dthread_cleanup_pop(1);

    if (status == DNS_SUCCESS) {
        /*
         * send a reset message
         */
        status = send_reset_msg (adv_p);
    }

    /*
     * send the messages
     */

    if ((status == DNS_SUCCESS) && begin_p) {
	ta_adver_msg_t *this_p = begin_p;

	do {
	    status = sys_mbx_write (adv_p->adver_mbx,
				    (unsigned char *)&this_p->adver_msg,
				    this_p->adver_msg.msgLen);

	    if (status != DNS_SUCCESS) {
		break;
	    }
	} while ((this_p = this_p->next_p) != NULL);
    }

    /*
     * free the space used
     */

    cleanup_adver_msg(begin_p);

    if ((thstatus = dthread_broadcast(&adv_p->adver_change_advertised)) < 0) {
	dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
	" <send_adver_msg(%p) return(%s)\n",
	adv_p, deb_ascii_dns_status(statusBuf,status)));

    return (status);
}

/* -------------------------------------------------------------------------- */

/*
 * cleanup_adver_msg
 *     cleans up linked list of adver messages on failures and completion.
 *
 * Input:
 *     begin_p pointer to first message
 *
 * Output:
 *     frees memory
 *
 * Value:
 */

static void
cleanup_adver_msg (ta_adver_msg_t *begin_p)
    
{
    while (begin_p) {
	ta_adver_msg_t *const this_p = begin_p;
        begin_p = this_p->next_p;

        dns_free((char *)this_p);
    }
}



/* -------------------------------------------------------------------------- */

/*
 * build_adver_msg
 *     Builds an advertisement message with one clearinghouse in it.
 *
 * Input:
 *     tower_p    pointer to tower set to use
 *     msg_p      pointer to place to put message
 *     tower_p    pointer to tower set to use
 *
 * Output:
 *     frees memory
 *
 */

static void
build_adver_msg (
    FullName_t          *ch_name_p,      /* in   - Clearinghouse name      */
    byte_t              *tower_p,        /* in   - Clearinghouse towers    */
    ObjUID_t            *ch_uid_p,       /* in   - Clearinghouse UID       */
    SimpleName_t        *ns_nickname_p,  /* in   - Cell Name               */
    ObjUID_t            *ns_uid_p,       /* in   - Cell UID                */
    AdvertiseRequest_t  *msg_p)          /* out  - Formatted advertisement */
{
    DEB_ASCII_BUF_FullName(chNameBuf)
    DEB_ASCII_BUF_ObjUID(chuidBuf)
    DEB_ASCII_BUF_SimpleName(nsNickBuf)
    DEB_ASCII_BUF_ObjUID(nsuidBuf)
    g_message_t             *arch_msg_p;
    byte_t                  *count_p;
    byte_t                  *replica_p;
    int                      replica_len;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >build_adver_msg(%s,,%s,%s,%s,)",
	deb_ascii_FullName(chNameBuf,ch_name_p),
	deb_ascii_ObjUID(chuidBuf,ch_uid_p),
	deb_ascii_SimpleName(nsNickBuf,ns_nickname_p),
	deb_ascii_ObjUID(nsuidBuf,ns_uid_p)));

    msg_p->version = CurrentIPCversion;
    msg_p->msgType = AdvReq_t;
    msg_p->advType = an802Msg;

    arch_msg_p = (g_message_t *)&msg_p->msg[0];
    INS8(&arch_msg_p->g_majorVersion[0],MAJORVER); /* Major version of this SW */
    INS8(&arch_msg_p->g_minorVersion[0],MINORVER); /* Minor version of this SW */
    INS8(&arch_msg_p->g_userMajor[0],0);
    INS8(&arch_msg_p->g_userMinor[0],0);
    INS8(&arch_msg_p->g_message_type[0],MT_Advertise);

    COPY_ObjUID(ns_uid_p, &arch_msg_p->g_ns_uid[0]);
    COPY_SimpleName (ns_nickname_p, &arch_msg_p->g_ns_nickname[0]);

    count_p = (byte_t *)SKIP_SimpleName(&arch_msg_p->g_ns_nickname[0]);
    INS8 (count_p,1);

    replica_p = (byte_t *)SKIP_byte(count_p);
    replica_len = replica_build((ReplicaPointer_t *)replica_p, 
                                ch_uid_p, 
                                (Set_t *)tower_p,
                                RT_readOnly, 
                                ch_name_p);

    /* size of whole message */
    msg_p->msgLen = SKIP_bytes(replica_p, replica_len) - (byte_t *)msg_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <build_adver_msg(%s,,%s,%s,%s,)",
	deb_ascii_FullName(chNameBuf,ch_name_p),
	deb_ascii_ObjUID(chuidBuf,ch_uid_p),
	deb_ascii_SimpleName(nsNickBuf,ns_nickname_p),
	deb_ascii_ObjUID(nsuidBuf,ns_uid_p)));
}

/* -------------------------------------------------------------------------- */

/*
 * send_reset_msg
 *     sends message to advertiser to clear stash of messages, new ones
 *     coming
 *
 * Input:
 *
 * Output:
 *     writes to mailbox or socket
 *
 * Value:
 */
static dns_status_t 
send_reset_msg (adver_ctrl_t *adv_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    struct {            /* must be aligned */
        int             temp1;
        bytes_u         buf[AdReqHdrSize];
        int             temp2;
    }                      msg;
    AdvertiseRequest_t    *const msg_p = (AdvertiseRequest_t *)msg.buf;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >send_reset_msg(%p)", adv_p));
    
    /*
     * stuff the message 
     */
    msg_p->version = CurrentIPCversion;
    msg_p->msgType = AdvReq_t;
    msg_p->advType = ResetMsg;
    msg_p->msgLen = sizeof(msg.buf);

    status = sys_mbx_write (adv_p->adver_mbx, msg.buf, msg_p->msgLen);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <send_reset_msg(%p) return(%s)", adv_p,
        deb_ascii_dns_status(statusBuf,status)));

    return (status);
}

/* -------------------------------------------------------------------------- */

/*
 * send_ping_msg
 *     sends a ping message - no data - tells advertiser to send out
 *     existing stashed messages
 *
 * Input:
 *     Advertiser control data
 *
 * Output:
 *     writes to mailbox or socket
 *
 * Value:
 */
static dns_status_t 
send_ping_msg (adver_ctrl_t *adv_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    struct {            /* must be aligned */
        int             temp1;
        bytes_u         buf[AdReqHdrSize];
        int             temp2;
    }                      msg;
    AdvertiseRequest_t    *const msg_p = (AdvertiseRequest_t *)msg.buf;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >send_ping_msg(%p)", adv_p));
    
    /*
     * stuff the message 
     */
    msg_p->version = CurrentIPCversion;
    msg_p->msgType = AdvReq_t;
    msg_p->advType = PingMsg;
    msg_p->msgLen = sizeof(msg.buf);

    status = sys_mbx_write (adv_p->adver_mbx, msg.buf, msg_p->msgLen);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <send_ping_msg(%p) return(%s)", adv_p,
        deb_ascii_dns_status(statusBuf,status)));

    return (status);
}

/* -------------------------------------------------------------------------- */

/*
 * send_quit_msg
 *     sends message to advertiser to stop advertising
 *
 * Input:
 *
 * Output:
 *     writes to mailbox or socket
 *
 * Value:
 */
static dns_status_t 
send_quit_msg (adver_ctrl_t *adv_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    struct {            /* must be aligned */
        int             temp1;
        bytes_u         buf[AdReqHdrSize];
        int             temp2;
    }                      msg;
    AdvertiseRequest_t    *const msg_p = (AdvertiseRequest_t *)msg.buf;
    dns_status_t           status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >send_quit_msg(%p)", adv_p));

    /* stuff the message */
    msg_p->version = CurrentIPCversion;
    msg_p->msgType = AdvReq_t;
    msg_p->advType = QuitMsg;
    msg_p->msgLen = sizeof(msg.buf);

    status = sys_mbx_write (adv_p->adver_mbx, msg.buf, msg_p->msgLen);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <send_quit_msg(%p) -> return(%s)", adv_p,
        deb_ascii_dns_status(statusBuf,status)));

    return (status);
}

/* -------------------------------------------------------------------------- */

/*
 * adver_start
 *     wakes up adver thread with a signal, and sets the new info flag
 *
 * Input:
 *
 * Output:
 *     signals adver thread, and sets the new info flag
 *
 * Value:
 */
void *
adver_start (nsgbl_ns_t *ns_p)
{
    DEB_ASCII_BUF_nsgbl_ns(nsBuf)
    register adver_ctrl_t *adv_p ;
    int thstatus;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
	" >adver_start(%s)\n",
	deb_ascii_nsgbl_ns(nsBuf,ns_p)));

    if (adv_p = (adver_ctrl_t *)dns_malloc(sizeof(*adv_p))) {
        extern dthread_attr_t ta_ping_advertiser_attr;

	ZERO_bytes(adv_p, sizeof(*adv_p));
        adv_p->adver_ns_p = ns_p;
        adv_p->adver_state = is_dead;
	adv_p->adver_message_flow = message_flow_template;
        adv_p->adver_sweep_interval = 10;

        /* Assume new information */
        adv_p->adver_new_info = TRUE;

        if ((thstatus = dthread_create_mutex(&adv_p->adver_mutex)) < 0) {
            dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
        }

        if ((thstatus = dthread_create_condition(&adv_p->adver_c_var)) < 0) {
            dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
        }

        if ((thstatus = dthread_create_condition(&adv_p->adver_change_advertised)) < 0) {
            dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
        }

        /* Start our advertiser thread */
        if (dthread_create(&adv_p->adver_thread, &ta_ping_advertiser_attr,
                        (dthread_start_routine)ta_ping_advertiser, (dthread_address_t)adv_p) < 0) {
            if ((thstatus = dthread_delete_condition(&adv_p->adver_c_var)) < 0) {
                dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
            }
            if ((thstatus = dthread_delete_condition(&adv_p->adver_change_advertised)) < 0) {
                dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
            }

            if ((thstatus = dthread_delete_mutex(&adv_p->adver_mutex)) < 0) {
                dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
            }
            dns_free((char *)adv_p);
            adv_p = 0;
        }
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
	" <adver_start(%s) return(%p)\n",
	deb_ascii_nsgbl_ns(nsBuf,ns_p), adv_p));

    return((void *)adv_p);
}

/* -------------------------------------------------------------------------- */

/*
 * adver_signal_new_info
 *     wakes up adver thread with a signal, and sets the new info flag
 *
 * Input:
 *
 * Output:
 *     signals adver thread, and sets the new info flag
 *
 * Value:
 */
void 
adver_signal_new_info (void *ptr)
{
    register adver_ctrl_t *const adv_p = (adver_ctrl_t *)ptr;
    int thstatus;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >adver_signal_new_info(%p)", ptr));

    if (adv_p) {

        DNS_LOCK(&adv_p->adver_mutex);

	adv_p->adver_new_info = TRUE;

	if ((thstatus = dthread_signal(&adv_p->adver_c_var)) < 0) {
	    dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)thstatus);
	}

        DNS_END_LOCK(&adv_p->adver_mutex);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <adver_signal_new_info(%p)", ptr));
}

/* -------------------------------------------------------------------------- */

/*
 * adver_wait_for_advertisement
 *
 * Stalls thread until advertisements have left the server.
 *
 */
void 
adver_wait_for_advertisement (void *ptr)
{
    register adver_ctrl_t *const adv_p = (adver_ctrl_t *)ptr;
    int thstatus;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >adver_wait_for_advertisement(%p)", ptr));

    if (adv_p) { /* i */
        DNS_LOCK(&adv_p->adver_mutex);

	dthread_cleanup_push(cleanup_mutex, &adv_p->adver_mutex);

	if (server_diag_check_setting(&adv_p->adver_message_flow) >= 0) {
	    while (adv_p->adver_new_info && 
		   adv_p->adver_state == is_alive) {
		
		if ((thstatus = dthread_wait(&adv_p->adver_change_advertised,
					     &adv_p->adver_mutex)) < 0) {
		    dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
		}
	    }
	} else {
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_server,
		svc_c_debug6,
		"  adver_wait_for_advertisement(%p) - inhibit behavior", ptr));
	}

        dthread_cleanup_pop(1);
    }
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <adver_wait_for_advertisement(%p)", ptr));
}

/* -------------------------------------------------------------------------- */

/*
 * adver_kill
 *     wakes up adver thread with a signal, and sets the new info flag
 *
 * Input:
 *
 * Output:
 *     signals adver thread, and sets the new info flag
 *
 * Value:
 */
void 
adver_kill (void *ptr)
{
    register adver_ctrl_t *const adv_p = (adver_ctrl_t *)ptr;
    int thstatus;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >adver_kill(%p)", ptr));

    if (adv_p) {
        dthread_address_t join_status;

        DNS_LOCK(&adv_p->adver_mutex);

	adv_p->adver_state = is_aborting;

	if ((thstatus = dthread_signal(&adv_p->adver_c_var)) < 0) {
	    dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)thstatus);
	}
               
	if ((thstatus = dthread_broadcast(&adv_p->adver_change_advertised)) < 0) {
	    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	}

        DNS_END_LOCK(&adv_p->adver_mutex);

        dthread_join(adv_p->adver_thread, &join_status);
        dthread_detach(&adv_p->adver_thread);

        if ((thstatus = dthread_delete_condition(&adv_p->adver_c_var)) < 0) {
            dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
        }

        if ((thstatus = dthread_delete_condition(&adv_p->adver_change_advertised)) < 0) {
            dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
        }

        if ((thstatus = dthread_delete_mutex(&adv_p->adver_mutex)) < 0) { 
             dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
        }

        dns_free((char *)adv_p);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <adver_kill(%p)", ptr));
}


/* -------------------------------------------------------------------------- */

int 
adver_diag_dump_control_block (int                 detail_level,
                               void *           adv_handle,
                               int (*emit_routine  )(char *, void *),
                               void *           emit_arg)
{
#define EMIT if ((*emit_routine)(out,emit_arg) < 0) return(1)
    char out[255];        /* Output buffer */
    adver_ctrl_t *adv_p = (adver_ctrl_t *)adv_handle;

    if (adv_p == NULL) {
        sprintf(out, "** Invalid advertiser control block"); EMIT;
        return (-1);
    }

    sprintf(out, "___ Adver Control Block: 0x%x _____________________________",
            adv_p); EMIT;

    sprintf(out, "Thread:   0x%x      NSB: 0x%x", 
        adv_p->adver_thread, adv_p->adver_ns_p); EMIT;
    sprintf(out, "Mutex:    0x%x      State changed cv: 0x%x",
        adv_p->adver_mutex, adv_p->adver_c_var); EMIT;
    sprintf(out, "State:    %s       Message flow: %s",
        ping_state_ascii(adv_p->adver_state), 
        server_diag_setting_ascii(adv_p->adver_message_flow)); EMIT;
    sprintf(out, "New Info: %d               Advertised change cv: 0x%x",
        adv_p->adver_new_info, adv_p->adver_change_advertised); EMIT;
    sprintf(out, "Sweep Interval: %d minutes", adv_p->adver_sweep_interval);
    EMIT;

    return (1);
}


/* -------------------------------------------------------------------------- */

/*
 * Modify the advertiser message flow diagnostic switch in a control
 * block, or modify the template from which new advertiser threads
 * are created (allows inhibiting prior to advertiser thread startup).
 */

int 
adver_diag_set_message_flow (void *     adv_handle,
                             diag_state_t  new_state,
                             diag_state_t  *old_state)
{
    adver_ctrl_t  *adv_p = (adver_ctrl_t *)adv_handle;
    int thstatus;


    if (adv_p == NULL) {
        *old_state = message_flow_template;
        message_flow_template = new_state;
    } else {
        DNS_LOCK(&adv_p->adver_mutex);

	*old_state = adv_p->adver_message_flow;
	adv_p->adver_message_flow = new_state;

        DNS_END_LOCK(&adv_p->adver_mutex);
    }

    return (DNS_SUCCESS);
}

static void
cleanup_mutex (void *arg)
{
    dthread_mutex_t *const mutex_p = arg;
    int thstatus;

    DNS_END_LOCK(mutex_p);
}
