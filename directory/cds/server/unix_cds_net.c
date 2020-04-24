/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: unix_cds_net.c,v $
 * Revision 1.1.13.3  1996/02/18  19:37:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:00  marty]
 *
 * Revision 1.1.13.2  1995/12/08  15:34:12  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/DCE1_1_WP/2  1995/05/08  14:07 UTC  jrr
 * 	Merge fix for handling new cds.conf file.
 * 
 * 	HP revision /main/DCE1_1_WP/jrr_wp/1  1995/05/08  13:57 UTC  jrr
 * 	Reject old server_princ_names containing cellname to avoid trouble with changed cellnames (CHFts15155).
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/04/13  16:16 UTC  jrr
 * 	Initial version for Warranty Patch
 * 
 * 	HP revision /main/HPDCE02/MOTHRA_panic/3  1995/04/11  20:23 UTC  rps
 * 	fix null srv_p workaround to return bad status per Dean Shu
 * 
 * 	HP revision /main/HPDCE02/MOTHRA_panic/1  1995/03/31  19:24 UTC  sommerfeld
 * 	Handle case where srv_p is NULL during startup in srv_pool_picker()
 * 	[1995/12/08  14:50:35  root]
 * 
 * Revision 1.1.9.1  1994/10/25  16:17:48  proulx
 * 	     Add ept_s_not_registered to list of acceptable
 * 	     returns from rpc_ns_binding_unexport() in
 * 	     net_re_export_ch. Also, don't need to export
 * 	     hcell interface.
 * 	[1994/10/25  16:15:42  proulx]
 * 
 * Revision 1.1.2.15  1994/09/13  20:21:58  mccann
 * 	fix annotation string OT:12143
 * 	[1994/09/13  20:21:43  mccann]
 * 
 * Revision 1.1.2.14  1994/08/26  21:55:39  zee
 * 	Make registering cprpc_hcell interface similar to cds_clerkserver (per
 * 	clearinghouse).
 * 	[1994/08/26  21:50:12  zee]
 * 
 * Revision 1.1.2.13  1994/08/24  20:15:04  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:09:42  mccann]
 * 
 * Revision 1.1.2.12  1994/08/03  19:05:08  mccann
 * 	final sams cleanup drop
 * 	[1994/08/01  20:01:00  mccann]
 * 
 * Revision 1.1.2.11  1994/07/27  19:57:45  jd
 * 	Removed extern of evtblk. It no longer exists.
 * 	Cast last argument of call to export_ns_new.
 * 	[1994/07/27  17:29:02  jd]
 * 
 * Revision 1.1.2.10  1994/07/25  15:14:58  proulx
 * 	Use common ACL manager for subcomponent mgmt ACLs
 * 	[1994/07/25  13:57:57  proulx]
 * 
 * Revision 1.1.2.9  1994/06/30  19:18:22  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:49:33  mccann]
 * 
 * Revision 1.1.2.8  1994/06/23  18:30:55  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:51:48  mccann]
 * 
 * Revision 1.1.2.7  1994/06/09  18:44:18  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:57  devsrc]
 * 
 * Revision 1.1.2.6  1994/05/09  18:55:12  zee
 * 	     Remove temporary authenticated workaround for broken intercell.
 * 	[1994/05/09  15:41:35  zee]
 * 
 * Revision 1.1.2.5  1994/05/06  16:04:32  zee
 * 	     HCell BL4 support: further pool support for hcell interface.
 * 	[1994/05/05  21:21:35  zee]
 * 
 * Revision 1.1.2.4  1994/04/14  14:51:38  peckham
 * 	Remove trailing ; on DEB_ASCII_BUF
 * 	[1994/04/12  15:59:00  peckham]
 * 
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:22:54  peckham]
 * 
 * Revision 1.1.2.3  1994/04/04  20:06:29  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:01:37  peckham]
 * 
 * Revision 1.1.2.2  1994/03/22  20:00:38  griffin
 * 	HCell BL3 support: Endpoint registration for add/remove cellname
 * 	interface.
 * 	[1994/03/22  18:17:41  griffin]
 * 
 * Revision 1.1.2.1  1994/03/12  22:13:53  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  20:03:49  peckham]
 * 
 * $EndLog$
 */
/*
 * Module unix_cds_net.c
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

#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <dce/assert.h>
#include <fcntl.h>
#include <sys/uio.h>
#ifndef SOCK_STREAM		/* socket.h not idempotent in BSD ULTRIX */
# include <sys/socket.h>
#endif
#include <errno.h>
#include <netdb.h>

#include <address.h>
#include <events.h>
#include <names.h>
#include <security.h>
#include <dns_malloc.h>
#include <dns_cfg.h>

/*
 * Determine protocol support selections
 */
#include <stddef.h>
#include <dce/mgmt.h>
#include <dce/rpc.h>
#include <dce/rpcpvt.h>
#include <rpcserver.h>
#include <netinet/in.h>
#include <back.h>
#include <dbdef.h>
#include <dce/cds_clerkserver.h>
#include <dce/cprpc_hcell.h>
#include <tower.h>
#include <dce/cprpc_server.h>
#ifdef DCE_SEC
# include <dce/rdaclifv0.h>
# include <dce/rdaclif.h>
# include <dce/dce_cf.h>
#endif

#define ts_new(a)       dnsCreateCTS((dns_cts *)a)

/* local structures */
#define RPC_INIT 1
#define RPC_ON   2
#define RPC_OFF  3

extern char *myname_p;
extern int debug_mode;
extern struct nsgbl_ns dns_nameserver;
#ifdef DCE_SEC
# include <dce/binding.h>
  boolean32 orig_sec_nsi_state; /* original nsi state for security bindings
      * (formally 'BIND_PE_SITE' */
#endif
static char *server_mgmt_dacl_string = "faf2e540-58b8-11ca-a04a-08002b12a70d";
uuid_t server_mgmt_dacl_uuid;
static rpc_if_id_t clerkserver_if_id;
static rpc_if_id_t hcell_s_if_id;
static rpc_if_id_t mgmt_s_if_id;
static void srv_pool_picker(uuid_p_t, rpc_if_id_p_t, unsigned32, rpc_thread_pool_handle_t *, unsigned32 *);
extern serviceability_v1_0_epv_t dce_svc_ep;



/*
 * Per server object
 */
typedef struct srv_data {
    short               srv_rpc_state;  /* init, on, off */
    dthread_mutex_t     srv_rpc_mutex;  /* need for synchronizing */
    dthread_cond_t	srv_rpc_cond;	/* signal when srv_rpc_state set */
    rpc_binding_vector_t *srv_rpc_binding;
    DBSet_t             *srv_rpc_towerset;
    nsgbl_ns_t		*srv_ns_p;	/* server mgmt database */
    dthread_t		srv_rpc;	/* if RPC listener active */
    rpc_thread_pool_handle_t srv_atomic_pool;
    rpc_thread_pool_handle_t srv_nsop_pool;
} srv_data_t;

#define DEB_ASCII_LEN_SRV_DATA DEB_ASCII_LEN_ptr
#define DEB_ASCII_BUF_srv_data(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_SRV_DATA)

#ifdef DCE_CDS_DEBUG

static char 
*deb_ascii_srv_data (char *const        buf,
                     const void *const  ptr)
{
    const srv_data_t *const srv_p = (const srv_data_t *)ptr;


    if (srv_p) {
	(void)sprintf(buf, DEB_ASCII_FMT_ptr, srv_p);

	return(buf);
    } 
    else
    {
	return("NULL");
    }
    
}

#endif

static error_status_t 
InitializeRPC (
    char		*,
    srv_data_t		*,
    int			);

static DBSet_t *
net_dacl_towers (
    rpc_binding_vector_t *bind_p);

#ifdef DCE_SEC

static dthread_address_t
sec_init_thread (void);

#endif


/* server_rpc_thread: listen for rpc requests 
*/
static dthread_address_t 
server_rpc_thread (srv_data_t *srv_p)
{
    DEB_ASCII_BUF_srv_data(srvBuf)
    uuid_vector_t uuids;
    rpc_binding_vector_t *bind_p;
    error_status_t rpc_status;
    dthread_address_t status = 0;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >server_rpc_thread(%s)",
        deb_ascii_srv_data(srvBuf,srv_p)));

/* Try to increase stack size */

#ifdef DCE_CDS_DEBUG
    rpc_mgmt_set_server_stack_size(64*1024, &rpc_status);
#endif

    /* create thread pools to avoid deadlocks */
    rpc_server_create_thread_pool(1, &srv_p->srv_atomic_pool, &rpc_status);
    dce_assert(cds__svc_handle, rpc_status == rpc_s_ok);

    rpc_server_set_thread_pool_qlen(srv_p->srv_atomic_pool,
		rpc_c_listen_max_calls_default + 1, &rpc_status);
    dce_assert(cds__svc_handle, rpc_status == rpc_s_ok);

    rpc_server_create_thread_pool(1, &srv_p->srv_nsop_pool, &rpc_status);
    dce_assert(cds__svc_handle, rpc_status == rpc_s_ok);

    rpc_server_set_thread_pool_qlen(srv_p->srv_nsop_pool,
		rpc_c_listen_max_calls_default + 1, &rpc_status);
    dce_assert(cds__svc_handle, rpc_status == rpc_s_ok);

    rpc_server_set_thread_pool_fn(&srv_pool_picker, &rpc_status);
    dce_assert(cds__svc_handle, rpc_status == rpc_s_ok);

    rpc_server_listen(rpc_c_listen_max_calls_default, &rpc_status);
    dce_assert(cds__svc_handle, rpc_status == rpc_s_ok);

    rpc_server_set_thread_pool_fn(NULL, &rpc_status);

    /* release the thread pools */
    rpc_server_free_thread_pool(&srv_p->srv_nsop_pool, TRUE, &rpc_status);
    rpc_server_free_thread_pool(&srv_p->srv_atomic_pool, TRUE, &rpc_status);

    /* Unregister interfaces  - clearinghouses unregisted as disabled*/
    bind_p = srv_p->srv_rpc_binding;
    uuids.count = 1;
    uuids.uuid[0] = &server_mgmt_dacl_uuid;
    rpc_ep_unregister (serviceability_v1_0_s_ifspec, bind_p,
		       &uuids, &rpc_status);
    rpc_server_unregister_if(serviceability_v1_0_s_ifspec,
			     (uuid_t *)0, &rpc_status);
#ifdef DCE_SEC
    rpc_ep_unregister (rdaclif_v0_0_s_ifspec, bind_p,
		       &uuids, &rpc_status);
    rpc_server_unregister_if(rdaclif_v0_0_s_ifspec,
			     (uuid_t *)0, &rpc_status);
    rpc_ep_unregister (rdaclif_v1_0_s_ifspec, bind_p,
		       &uuids, &rpc_status);
    rpc_server_unregister_if(rdaclif_v1_0_s_ifspec,
			     (uuid_t *)0, &rpc_status);
#endif
    rpc_ep_unregister (DNScpServer_v1_0_s_ifspec, bind_p,
		       (uuid_vector_p_t)0, &rpc_status);

    rpc_server_unregister_if(DNScpServer_v1_0_s_ifspec,
			     (uuid_t *)0, &rpc_status);

    rpc_ep_unregister (cprpc_hcell_v1_0_s_ifspec, bind_p,
		       (uuid_vector_p_t)0, &rpc_status);

    rpc_server_unregister_if(cprpc_hcell_v1_0_s_ifspec,
			     (uuid_t *)0, &rpc_status);


    SERVER_LOCK(srv_p->srv_rpc_mutex)
     /* Beginning of the 'protected code' */

    srv_p->srv_rpc_state = RPC_OFF;
    rpc_binding_vector_free(&srv_p->srv_rpc_binding, &rpc_status);
    if (srv_p->srv_rpc_towerset)
	dns_free((char *)srv_p->srv_rpc_towerset);
    srv_p->srv_rpc_towerset = NULL;

    if ((thstatus = dthread_signal(&(srv_p->srv_rpc_cond))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)thstatus);
    }

    /* End of the 'protected code' */


   SERVER_END_LOCK(srv_p->srv_rpc_mutex)

    if (rpc_status != rpc_s_ok)
	status = (dthread_address_t)-1;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >server_rpc_thread(%s) return(%p)",
        deb_ascii_srv_data(srvBuf,srv_p),
        status));

    return(status);
}

/*
 * Initalize RPC:  this is called at startup time (main->server_watchdog->
 * server_auto->dns_enable->net_open->InitializeRPC. It is part of the
 * main thread.  It calls rpc_server_init to check to see if the
 * daemon is already running and to register all the rpc interfaces
 * (clerk-server, control program management, remote DACL)
 * If this succeeds, a thread is started to listen for rpcs.
 */
static error_status_t 
InitializeRPC (char        *mynam_p,
               srv_data_t  *srv_p,
               int         deb_mode)
{
    DEB_ASCII_BUF_srv_data(srvBuf)
    extern dthread_attr_t rpc_server_listen_attr;
    int if_count = 0;
    int i;
    unsigned int mode;
    rpc_binding_vector_t *bind_p;
    rpc_if_descriptor_t if_descriptors[4];
    error_status_t rpc_status, temp_status;
    uuid_vector_t uuids;
    error_status_t status = CDS_SUCCESS;
    uuid_vector_t mgmt_uuids;
    int thstatus;
    char                servername[]="cds-server";


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >InitializeRPC(%s,%s,%d)",
	mynam_p,
        deb_ascii_srv_data(srvBuf,srv_p), 
        deb_mode));

    
    /*
     * Fork a child, if we're not running in debug mode 
     */
    mode = (deb_mode) ? (rpc_c_server_mode_debug | rpc_c_server_mode_verify) : 0;
    if (deb_mode == 0) 
    {
      if (cds_server_fork (mynam_p, mode, &rpc_status) != 0) 
      {
	/* Beginning of the 'protected code' */

	srv_p->srv_rpc_state = RPC_OFF;

	/* End of the 'protected code' */

	SERVER_END_LOCK(srv_p->srv_rpc_mutex)
	status = CDS_ERROR;
	goto leave_InitializeRPC;
      }
    }

    rpc_server_register_if(cds_clerkserver_v1_0_s_ifspec, NULL, NULL, 
			   &rpc_status);

    rpc_server_register_if(cprpc_hcell_v1_0_s_ifspec, NULL, NULL, 
			   &rpc_status);

    /* Call rpc_server_init to check for server already running,
     * and register our interfaces/endpoints.
     */
    ZERO_bytes (if_descriptors, sizeof (if_descriptors));

    /* cdscp - server management interface */
    if_descriptors[if_count].if_handle = DNScpServer_v1_0_s_ifspec;
    if_descriptors[if_count].if_epv    = NULL;
    if_descriptors[if_count].annotation = 
	(char *)dce_msg_get_msg(cds_s_server_mgmtif, &rpc_status);
    if_count++;

    /* 
     * Register the server management DACL and SVC interface endpoint(s).
     */
    uuid_from_string ((unsigned char *)server_mgmt_dacl_string,
					&server_mgmt_dacl_uuid, &rpc_status);
    if (rpc_status != uuid_s_ok) {
	status = rpc_status;
	goto leave_InitializeRPC;
    }

    if_descriptors[if_count].if_handle = serviceability_v1_0_s_ifspec;
    if_descriptors[if_count].if_epv    = (rpc_mgr_epv_t *)&dce_svc_ep;
    if_descriptors[if_count].annotation =
	(char *)dce_msg_get_msg(cds_s_server_svcif, &rpc_status);
    mgmt_uuids.count = 1;
    mgmt_uuids.uuid[0] = &server_mgmt_dacl_uuid;
    if_descriptors[if_count].object_ids = &mgmt_uuids;
    if_count++;

#ifdef DCE_SEC
    {
       extern rdaclif_v0_0_epv_t server_acl_v0_epv; 
       extern rdaclif_v1_0_epv_t server_acl_v1_epv; 

       /* Remote DACL interface for entry ACLs V0.0 */
       if_descriptors[if_count].if_handle = rdaclif_v0_0_s_ifspec;
       if_descriptors[if_count].if_epv    = (rpc_mgr_epv_t *)&server_acl_v0_epv;
       if_descriptors[if_count].annotation =
	   (char *)dce_msg_get_msg(cds_s_server_aclmgmtifv0, &rpc_status);
       if_descriptors[if_count].object_ids = &mgmt_uuids;
       if_count++;

       /* Remote DACL interface for entry ACLs V1.0 */
       if_descriptors[if_count].if_handle = rdaclif_v1_0_s_ifspec;
       if_descriptors[if_count].if_epv    = (rpc_mgr_epv_t *)&server_acl_v1_epv;
       if_descriptors[if_count].annotation =
	   (char *)dce_msg_get_msg(cds_s_server_aclmgmtifv1, &rpc_status);
       if_descriptors[if_count].object_ids = &mgmt_uuids;
       if_count++;
    }
#endif /* DCE_SEC */

    if (cds_server_init ( mynam_p,
			  NULL,
			  if_count,
			  if_descriptors,
			  mode,
			  &dns_nameserver.ns_bind_vector,
			  &rpc_status) != 0) 
    {
      /*
       * Set rpc state = off (does this really matter, anymore?)
       */
      SERVER_LOCK(srv_p->srv_rpc_mutex)
      /* Beginning of the 'protected code' */

	  srv_p->srv_rpc_state = RPC_OFF;

      /* End of the 'protected code' */

      SERVER_END_LOCK(srv_p->srv_rpc_mutex)
      status = CDS_ERROR;
      goto leave_InitializeRPC;
    }

    /* Get all sequences back to register with EP */
    rpc_server_inq_bindings(&bind_p, &rpc_status);
    if (rpc_status != rpc_s_ok) {
	status = rpc_status;
	goto leave_InitializeRPC;
    }

    /* set up interface uuids for thread pool checks */
    rpc_if_inq_id(cds_clerkserver_v1_0_s_ifspec, &clerkserver_if_id, &rpc_status);
    dce_assert(cds__svc_handle, rpc_status == rpc_s_ok);

    rpc_if_inq_id(cprpc_hcell_v1_0_s_ifspec, &hcell_s_if_id, &rpc_status);
    dce_assert(cds__svc_handle, rpc_status == rpc_s_ok);

    /* Catch the internal management interface calls also */
    rpc_if_inq_id(mgmt_v1_0_s_ifspec, &mgmt_s_if_id, &rpc_status);
    dce_assert(cds__svc_handle, rpc_status == rpc_s_ok);

    /*
     * Correct the entry in the namespace if we have had our host
     * IP address changed since configuration.  export_ns_new uses 
     * the server_binding_vector, so don't free it here.  It doesn't
     * take much space anyway.
     *
     * Exporting the serviceability interface
     */
    export_ns_new (myname_p, servername, serviceability_v1_0_s_ifspec,
		   dns_nameserver.ns_bind_vector, &server_mgmt_dacl_uuid, 
		   (unsigned int *)&temp_status);
    if (temp_status != error_status_ok) {
       status = temp_status;
       goto leave_InitializeRPC;
    }

    /*
     * start the listener thread 
     */
    if ((thstatus = dthread_create(&srv_p->srv_rpc, 
				   &rpc_server_listen_attr,
				   (dthread_start_routine)server_rpc_thread, 
				   (dthread_address_t)srv_p)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
    }


    /* Set rpc state = on (does this really matter, anymore?)
     */
    SERVER_LOCK(srv_p->srv_rpc_mutex)

   /* Beginning of the 'protected code' */

       srv_p->srv_rpc_state = RPC_ON;
       /* Dacl interface only interested in security handles */
       srv_p->srv_rpc_binding = bind_p;
       srv_p->srv_rpc_towerset = 
       net_dacl_towers(srv_p->srv_rpc_binding);

       /* End of the 'protected code' */

    SERVER_END_LOCK(srv_p->srv_rpc_mutex)

leave_InitializeRPC:

    for (i=0; i<if_count; ++i)
	free(if_descriptors[i].annotation);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <InitializeRPC(%s,%s,%d) return(%lu)", 
        mynam_p,
        deb_ascii_srv_data(srvBuf,srv_p), 
        deb_mode, 
        status));

    return(status);
}

/*
 * Choose which thread pool for each request
 */
static void
srv_pool_picker(
    uuid_p_t obj_uuid,
    rpc_if_id_p_t if_id,
    unsigned32 opnum,
    rpc_thread_pool_handle_t *phandle,
    unsigned32 *status)
{
    srv_data_t *const srv_p = (srv_data_t *)dns_nameserver.ns_ta_id;
    const int op_cds_DeleteChild =
	offsetof(cds_clerkserver_v1_0_epv_t, cds_DeleteChild)/sizeof(void *);
    const int op_cds_LinkReplica =
	offsetof(cds_clerkserver_v1_0_epv_t, cds_LinkReplica)/sizeof(void *);
    const int op_cds_ModifyAttribute =
	offsetof(cds_clerkserver_v1_0_epv_t, cds_ModifyAttribute)/sizeof(void *);
    const int op_cds_ModifyReplica =
	offsetof(cds_clerkserver_v1_0_epv_t, cds_ModifyReplica)/sizeof(void *);
    const int op_cds_ReadAttribute =
	offsetof(cds_clerkserver_v1_0_epv_t, cds_ReadAttribute)/sizeof(void *);
    const int op_cds_ResolveName =
	offsetof(cds_clerkserver_v1_0_epv_t, cds_ResolveName)/sizeof(void *);
    const int op_cds_Skulk =
	offsetof(cds_clerkserver_v1_0_epv_t, cds_Skulk)/sizeof(void *);
    const int op_cds_TestAttribute =
	offsetof(cds_clerkserver_v1_0_epv_t, cds_TestAttribute)/sizeof(void *);
    const int op_cds_cds_TestGroup =
	offsetof(cds_clerkserver_v1_0_epv_t, cds_TestGroup)/sizeof(void *);

    /*
     * Temporary fix to address race condition:  cdsd starts
     * a thread to initialize the rpc, database and other stuff.  By the
     * time it gets to srv_pool_picker() triggered by a client request,
     * the database should have done its initialization or nothing will work.
     * ...
     * A non-zero status has to be
     * returned so that rpc runtime knows there is something wrong in
     * the server thread pool lookup function, and treat it as
     * "server not available".
     * 
     * An improvement could be to
     * Call pthread_cond_timedwait() to wait for "a while".  This "a while"
     * should be long enough to guarantee the database initialization is
     * done.  If srv_p becomes ready, we'll gladly continue, otherwise
     * we should abort cdsd by its internal bugcheck mechanism.
     */
    if (srv_p == NULL) {
	*status = rpc_s_cthread_no_such_pool;
	return;
    }
    else if (EQ_bytes(&if_id->uuid, &clerkserver_if_id.uuid,
		      sizeof(if_id->uuid)))
	if ((opnum == op_cds_ReadAttribute) ||
	    (opnum == op_cds_ResolveName) ||
            (opnum == op_cds_TestAttribute) ||
	    (opnum == op_cds_ModifyAttribute) ||
	    (opnum == op_cds_ModifyReplica) ||
	    (opnum == op_cds_cds_TestGroup))
	    *phandle = srv_p->srv_atomic_pool;
	else if ((opnum == op_cds_DeleteChild) ||
		 (opnum == op_cds_LinkReplica) ||
		 (opnum == op_cds_Skulk))
	    *phandle = srv_p->srv_nsop_pool;
	else
	    *phandle = NULL;
    else if (EQ_bytes(&if_id->uuid, &mgmt_s_if_id.uuid, sizeof(if_id->uuid)))
	*phandle = srv_p->srv_atomic_pool;
    else if (EQ_bytes(&if_id->uuid, &hcell_s_if_id.uuid, sizeof(if_id->uuid)))
	*phandle = srv_p->srv_atomic_pool;
    else
	*phandle = NULL;

    *status = rpc_s_ok;
}

/*
 * build a set of towers representing this server.  Used when
 * reading CDS_towers from a directory and when exporting clearinghouse
 * objects.
 */
static DBSet_t 
*net_dacl_towers (rpc_binding_vector_t *bind_p)
{
    rpc_tower_vector_p_t vector = NULL;
    struct cds_tower_vector {
	int   total;
        rpc_tower_vector_p_t vector[1];
    } *cds_p = NULL;

    unsigned32           status;
    int                  datalength = 0;
    int                  pos;
    int                  i,j,k;
    DBSet_t              *set_p = NULL;
    DBSetMember_t        *value_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >net_dacl_towers"));

    if (bind_p->count == 0)
	goto leave_net_dacl_towers;

#ifdef DCE_SEC
    for (i=0,pos=0; i<bind_p->count; ++i) {
	rpc_tower_vector_from_binding(rdaclif_v0_0_s_ifspec,
				      bind_p->binding_h[i], &vector, &status);
	if (status == rpc_s_ok) {
	    /* Assume each vector will have the same number of entires */
	    if (!cds_p) {
		cds_p =
		  (struct cds_tower_vector *)dns_malloc(sizeof(*cds_p) +
		vector->count*bind_p->count*sizeof(rpc_tower_vector_p_t));
		cds_p->total = 0;
	    }
	    if (cds_p) {
		cds_p->total += vector->count;
		cds_p->vector[pos] = vector;
		for (j=0; j<vector->count; ++j)
		    datalength += vector->tower[j]->tower_length;
		++pos;
	    }
	} /* End success */
    } /* end for loop */

    /* Now build a set of towers for this server */

    /* Get space for the set */
    set_p = (DBSet_t *)dns_malloc(sizeof(DBSet_overhead) +
	       (cds_p->total * (sizeof(DBSetMember_overhead) +
				sizeof(set_p->ds_offp[0]) +
				ALIGN_SIZE)) + datalength);

    if (set_p) {
	/* Build the set */
	INS8(set_p->ds_flag, DS_present);
	INS8(set_p->ds_type, AT_set);
	ZERO_Timestamp(set_p->ds_ts);
	INS32(set_p->ds_casemask, 0);
	set_p->ds_hcnt = 0;			/* no delete holders */
	set_p->ds_ocnt = cds_p->total;

	value_p = (DBSetMember_t *)ALIGN_PTR(DATA_DBSet(set_p));
	/* Loop through vectory of rpc_tower_vector_p_t */
	for (i=0, j=0; i<cds_p->total; ++j) {
	    for (k=0; k < cds_p->vector[j]->count; ++k, ++i) {
		/* build value header */
		set_p->ds_offp[i] = DIFF_bytes(value_p, set_p);
		INS8(value_p->dm_flag, DM_present);
		INS8(value_p->dm_valid, 1);
		INS8(value_p->dm_type, AT_set);
		INS8(value_p->dm_value_present, 1);
		(void)ts_new((Timestamp_t *)value_p->dm_ts);
		INS8(value_p->dm_valuetype, VT_byte);
		value_p->dm_length =
		  cds_p->vector[j]->tower[k]->tower_length;
		COPY_bytes(cds_p->vector[j]->tower[k]->tower_octet_string,
		       value_p->dm_data, value_p->dm_length);
		value_p =
		  (DBSetMember_t *)ALIGN_PTR(SKIP_DBSetMember(value_p));

	    } /* End inner loop */
	    rpc_tower_vector_free(&cds_p->vector[j], &status);
	} /* End outer loop */
	set_p->ds_setl = DIFF_bytes(value_p, set_p);
    }
    dns_free((char *)cds_p);
#endif /* defined DCE_SEC */

leave_net_dacl_towers:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <net_dacl_towers"));

    return(set_p);
}

/*
 * net_get_rpc_towers
 *   return the tower set for this server
 */
DBSet_t 
*net_get_rpc_towers (void *handle)
{
    register srv_data_t	*const srv_p = (srv_data_t *)handle;
    DBSet_t             *set_p = NULL;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >net_get_rpc_towers"));

    if (srv_p)
      set_p = srv_p->srv_rpc_towerset;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <net_get_rpc_towers"));

    return(set_p);
}
/*
 * Export the clearinghouse and hcell interfaces.  RPC will then generate
 * the attributes necessary for the acl_edit program to use the cle object
 * to contact the clearinghouse itself.
 */
int 
net_export_ch (nsgbl_ch_t *ch_p)
{
     DEB_ASCII_BUF_nsgbl_ch(chBuf)
     unsigned_char_t      chname[DNS_STR_FNAME_MAX];
     int                  len = sizeof(chname);
     uuid_vector_t        uuids;
     unsigned32           rpcstatus = rpc_s_incomplete_name;
     int		  status = FALSE;


     DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_server,
         svc_c_debug3,
         " >net_export_ch(%s)",
         deb_ascii_nsgbl_ch(chBuf,ch_p)));

     /* Set up uuid vector */
     uuids.count = 1;
     uuids.uuid[0] = &ch_p->ch_uid;

     /* Convert name to string format */
     if (dnsCvtCDSFullToStr((dns_full_name_t *)&ch_p->ch_name, chname, &len) ==
	 DNS_SUCCESS) {
	 register srv_data_t *const srv_p =
				(srv_data_t *)ch_p->ch_nameserver->ns_ta_id;

	 if (srv_p)  
	 {
	     /*
	      * Export the handle 
	      */

	     rpc_ns_binding_export(rpc_c_ns_syntax_default,  chname,
				   cds_clerkserver_v1_0_s_ifspec,
				   srv_p->srv_rpc_binding, &uuids, 
				   &rpcstatus);

	     if (rpcstatus != rpc_s_ok)
	     {
	       dce_svc_printf(CDS_S_RPCBINDINGEXPORT_MSG, (long)rpcstatus);
	     }
	 }	
     }
     if (rpcstatus == rpc_s_ok)
     {
	 status = TRUE;
     }

     DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_server,
         svc_c_debug3,
         " <net_export_ch(%s) return(%d)",
         deb_ascii_nsgbl_ch(chBuf,ch_p),
         status));

     return(status);
}
/*
 * Rexport the clearinghouse and rdacl interface.  RPC will then generate
 * the attributes necessary for the acl_edit program to use the cle object
 * to contact the clearinghouse itself.
 * First unexport the interface which remove the existing towers,
 * then add the new ones.
 * No need to unexport the objectuuid since it hasn't changed but will
 * reexport it in case it was removed.
 */
int 
net_re_export_ch (nsgbl_ch_t *ch_p)
{
     DEB_ASCII_BUF_nsgbl_ch(chBuf)
     unsigned_char_t      chname[DNS_STR_FNAME_MAX];
     int                  len = sizeof(chname);
     uuid_vector_t        uuids;
     unsigned32           rpcstatus = rpc_s_incomplete_name;
     int		  status = FALSE;


     DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_server,
         svc_c_debug3,
         " >net_re_export_ch(%s)",
         deb_ascii_nsgbl_ch(chBuf,ch_p)));

     /* Set up uuid vector */
     uuids.count = 1;
     uuids.uuid[0] = &ch_p->ch_uid;

     /* Convert name to string format */
     if (dnsCvtCDSFullToStr((dns_full_name_t *)&ch_p->ch_name, chname, &len) ==
	 DNS_SUCCESS) {
	 register srv_data_t  *const srv_p =
				(srv_data_t *)ch_p->ch_nameserver->ns_ta_id;

	 if (srv_p)  
	 {
	     rpc_ns_binding_unexport(rpc_c_ns_syntax_default,  chname,
				     cds_clerkserver_v1_0_s_ifspec,
				     (uuid_vector_t *)NULL, &rpcstatus);
	     /*
	      * Now export both the interface and uuid 
	      */
	     if ((rpcstatus == rpc_s_ok) ||
		 (rpcstatus == rpc_s_interface_not_found) ||
		 (rpcstatus == rpc_s_not_rpc_entry) ||
		 (rpcstatus == ept_s_not_registered) ||
		 (rpcstatus == rpc_s_entry_not_found))
	     {
	         rpc_ns_binding_export(rpc_c_ns_syntax_default,  chname,
			    	       cds_clerkserver_v1_0_s_ifspec,
				       srv_p->srv_rpc_binding, &uuids,
				       &rpcstatus);
		 if (rpcstatus != rpc_s_ok)
		     {
		       dce_svc_printf(CDS_S_RPCBINDINGEXPORT_MSG, (long)rpcstatus);
		     }
	     }
	     else 
	     {
	       dce_svc_printf(CDS_S_RPCBINDINGUNEXPORT_MSG, (long)rpcstatus);
	     }
	   }
       }

     if (rpcstatus == rpc_s_ok)
	status = TRUE;

     DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_server,
         svc_c_debug3,
         " >net_re_export_ch(%s) return(%d)",
         deb_ascii_nsgbl_ch(chBuf,ch_p),
         status));

     return(status);
}

/*
 * Register a clearinghouse uuid with the endpoint mapper
 */
void 
rpc_register_ch (void	*handle,
                 nsgbl_ch_t *ch_p)
{
    DEB_ASCII_BUF_ObjUID(chBuf)
    register srv_data_t	*const srv_p = (srv_data_t *)handle;
    unsigned32          rpc_status;
    uuid_vector_t       uuids;
    int thstatus;


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug3,
        " >rpc_register_ch(%p,%p)", handle, ch_p));

    if (srv_p) {

      SERVER_LOCK(srv_p->srv_rpc_mutex)

       /* Beginning of the 'protected code' */

      if (srv_p->srv_rpc_state == RPC_ON) {
	unsigned char *clerkserver_string,*rdacl0_string,*rdacl1_string;
	unsigned char *hcell_string;
	unsigned_char_t      chname[DNS_STR_FNAME_MAX];
	int                  len = sizeof(chname);

	chname[0] = '\0';
	dnsCvtCDSFullToStr((dns_full_name_t *)&ch_p->ch_name, chname, &len);
	
	clerkserver_string = dce_sprintf(cds_s_server_clerkserverif, chname);
	rdacl0_string = dce_sprintf(cds_s_server_aclifv0, chname);
	rdacl1_string = dce_sprintf(cds_s_server_aclifv1, chname);
	hcell_string =  dce_sprintf(cds_s_server_hcellif, chname);

	/* Set up vector */
	uuids.count = 1;
	uuids.uuid[0] = &ch_p->ch_uid;

	/*
	 * clerkserver interface 
	 */
	rpc_ep_register(cds_clerkserver_v1_0_s_ifspec, srv_p->srv_rpc_binding,
		&uuids, clerkserver_string, &rpc_status);
	if (rpc_status != rpc_s_ok) 
	{
	  dce_svc_printf(CDS_S_RPCREGISTEREP_MSG, (long)rpc_status);
	}
	/*
	 * hcell interface
	 */
	rpc_ep_register(cprpc_hcell_v1_0_s_ifspec, srv_p->srv_rpc_binding,
		&uuids, hcell_string, &rpc_status);
	if (rpc_status != rpc_s_ok) 
	{
	  dce_svc_printf(CDS_S_RPCREGISTEREP_MSG, (long)rpc_status);
	}
#ifdef DCE_SEC

	/*
	 * dacl interface 
	 */
	rpc_ep_register(rdaclif_v0_0_s_ifspec, srv_p->srv_rpc_binding,
			&uuids, rdacl0_string, &rpc_status);

	if (rpc_status != rpc_s_ok) 
	{
	  dce_svc_printf(CDS_S_RPCREGISTEREP_MSG, (long)rpc_status);
	}
#endif

      free(clerkserver_string);
      free(hcell_string);
      free(rdacl0_string);
      free(rdacl1_string);
    }
      /* End of the 'protected code' */

      SERVER_END_LOCK(srv_p->srv_rpc_mutex)

    };
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <rpc_register_ch(%p,%s)",
        handle,
        deb_ascii_ObjUID(chBuf,ch_p)));
}
/*
 * UnRegister a clearinghouse uuid with the endpoint mapper
 */
void 
rpc_unregister_ch (void	*handle,
                   ObjUID_t  *ch_p)
{
    DEB_ASCII_BUF_ObjUID(chBuf)
    register srv_data_t	*const srv_p = (srv_data_t *)handle;
    unsigned32          rpc_status;
    uuid_vector_t       uuids;
    int thstatus;
    dce_error_string_t  error_str;
    int                 inq_status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >rpc_unregister_ch(%p,%s)",
        handle,
        deb_ascii_ObjUID(chBuf,ch_p)));

    if (srv_p) {

      SERVER_LOCK(srv_p->srv_rpc_mutex)

       /* Beginning of the 'protected code' */
      if (srv_p->srv_rpc_state == RPC_ON) {
	/* Set up vector */
	uuids.count = 1;
	uuids.uuid[0] = ch_p;

	/* clerkserver interface */
	rpc_ep_unregister(cds_clerkserver_v1_0_s_ifspec,
			  srv_p->srv_rpc_binding, &uuids, &rpc_status);
	if (rpc_status != rpc_s_ok) 
	{
	  dce_svc_printf(CDS_S_RPCUNREGISTEREP_MSG, (long)rpc_status);
	}
	/* hcell interface */
	rpc_ep_unregister(cprpc_hcell_v1_0_s_ifspec,
			  srv_p->srv_rpc_binding, &uuids, &rpc_status);
	if (rpc_status != rpc_s_ok) 
	{
	  dce_svc_printf(CDS_S_RPCUNREGISTEREP_MSG, (long)rpc_status);
	}
#ifdef DCE_SEC

	/*
	 * dacl interface 
	 */
	rpc_ep_unregister(rdaclif_v0_0_s_ifspec, srv_p->srv_rpc_binding,
			&uuids, &rpc_status);
	if (rpc_status != rpc_s_ok) 
	  dce_svc_printf(CDS_S_RPCUNREGISTEREP_MSG, (long)rpc_status);
#endif

      }

      /* End of the 'protected code' */

      SERVER_END_LOCK(srv_p->srv_rpc_mutex)
  }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <rpc_unregister_ch(%p,%s)",
        handle,
        deb_ascii_ObjUID(chBuf,ch_p)));
}

/*
 * net_open
 *    This routine is called to bind an object number to this
 * image and issue the first asynchronous read on the network mailbox.
 * 
 * For CDS: the RPC interface is initialized, and a child process is
 * forked. If successful, the child process detaches from the parent,
 * here (if -d switch not present)

 *
 * Inputs:
 *     ns_p = nameserver database
 *     handle = address to return id
 *
 * Returns:
 *
 */
dns_status_t 
net_open (nsgbl_ns_t      *ns_p,
          void            **handle)
{
    DEB_ASCII_BUF_nsgbl_ns(nsBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    register srv_data_t	*srv_p;
    dns_status_t	status = DNS_NONSRESOURCES;
    int thstatus;
    dthread_t sec_th;
    extern dthread_attr_t sec_init_attr;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >net_open(%s)",
       deb_ascii_nsgbl_ns(nsBuf,ns_p)));

    srv_p = (srv_data_t *)dns_malloc_temp(sizeof(*srv_p));
    if (srv_p) {
	ZERO_bytes(srv_p, sizeof(*srv_p));
	srv_p->srv_ns_p = ns_p;

	srv_p->srv_rpc_state = RPC_INIT;

	if ((thstatus = dthread_create_mutex(&srv_p->srv_rpc_mutex)) < 0)
	    dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);

	if ((thstatus = dthread_create_condition(&srv_p->srv_rpc_cond)) < 0)
	    dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);

	if (InitializeRPC (myname_p, srv_p, debug_mode) != CDS_SUCCESS) {
	  dthread_term();

	  dce_svc_printf(CDS_S_SERVER_RPC_EB_MSG);
	}
	else { 
#ifdef DCE_SEC

    /* 
     * Initialize security.
     * Disable the use of NSI by security binding operations, otherwise
     * we end up in a chicken & egg situation with security trying to access
     * the namespace before it exists and/or is enabled. 
     * Restore the nsi binding state after the namespace is enabled.
     * Note: the bulk of the initialization occurs in a separate thread,
     * so that call-backs into cdsd will work.
     */
	  orig_sec_nsi_state = sec_rgy_enable_nsi (0); /* disable */

	  if ((thstatus = dthread_create(&sec_th, 
				 &sec_init_attr,
				 (dthread_start_routine)sec_init_thread,
				 NULL)) < 0)
	      {
		dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
	      }
	  dns_nameserver.Authenticated = 1;
		
#endif
	  /* allow rpc_allocate of memory */
	  
	  rpc_ss_enable_allocate();
	}
	*handle = srv_p;
	status = DNS_SUCCESS;
      }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <net_open(%s) return(%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * net_close
 *     Close the network.
 *
 * Input:
 *     net_p = address of network control block.
 *
 */
dns_status_t 
net_close (void *handle)
{
    DEB_ASCII_BUF_srv_data(srvBuf)
    register srv_data_t	*const srv_p = (srv_data_t *)handle;
    error_status_t rpc_status;
    dthread_address_t join_status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >net_close(%s)",
        deb_ascii_srv_data(srvBuf,srv_p)));

    if (srv_p) {
	  SERVER_LOCK(srv_p->srv_rpc_mutex)

             /* Beginning of the 'protected code' */

            if (srv_p->srv_rpc_state != RPC_OFF)  {
                rpc_mgmt_stop_server_listening(NULL, &rpc_status);
            }

	    /* End of the 'protected code' */

	  SERVER_END_LOCK(srv_p->srv_rpc_mutex)

	  (void)dthread_join(srv_p->srv_rpc, &join_status);
	  (void)dthread_detach(&srv_p->srv_rpc);

	  (void)dthread_delete_condition(&srv_p->srv_rpc_cond);
	  (void)dthread_delete_mutex(&srv_p->srv_rpc_mutex);
	  dns_free((char *)srv_p);
    }
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <net_close: return(DNS_SUCCESS)"));

    return(DNS_SUCCESS);
}

#ifdef DCE_SEC
/*
 * ROUTINE - sec_init_thread
 *
 * Security initialization. 
 * This code runs in a separate thread because it makes security 
 * calls which may either a) fail or b) result in calls to the cds
 * clerk.  If a failure occurs, the thread sleeps for a while and
 * retries.  If the security calls make calls to the cds api, this
 * thread may block until the cds_clerkserver interface has been
 * registered and exported - in the 'server_watchdog' thread.
 * This thread does the following:
 * 1) Does secure login using principal name from cds.conf file,
 *    and key from local keystore (or clear text password, if supplied
 *    in cds.conf)
 * 2) Initializes the default server management DACL, if necessary.
 * 3) Starts up a thread to refresh the server's identity periodically.
 * 4) Starts up a thread to change the server's key periodically.
 * 5) Registers RPC auth info.
*/

static dthread_address_t 
sec_init_thread (void)
{
  char passwd[100], *p, *cellname, *hostname;
  char *handle;
  error_status_t st;
  sec_rgy_handle_t context;
  sec_rgy_bind_auth_info_t auth;
  dthread_address_t status = (dthread_address_t)FALSE;
  int thstatus;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug3,
      " >sec_init_thread()"));

  handle = dns_cfgopen(NULL, NULL);

  /* Check cds.conf for presence of principal name. If absent, generate:
   * <cellname>/hosts/<hostname>/cds-server 
   */
  /* reject old server_princ_names containing cellname to avoid trouble with changed cellnames CHFts15155 */
  if ((p = dns_cfgtostr(handle, "security.server_princ_name", NULL)) == NULL || !strncmp(p, "/.../", 5)) {
    dce_cf_get_cell_name(&cellname, &st);
    if (st != dce_cf_st_ok) 
    {
      dce_svc_printf(CDS_S_CFGETCELLNAME_MSG, (long)st);
      goto leave_sec_init_thread;
    }

    dce_cf_get_host_name(&hostname, &st);
    if (st != dce_cf_st_ok) 
    {
      dce_svc_printf(CDS_S_CFGETHOSTNAME_MSG, (long)st);
      free(cellname);
      goto leave_sec_init_thread;
    }

    (void)sprintf((char *)dns_nameserver.ns_Principal_Global,
		  "%s/%s/cds-server", cellname, hostname);
    free(cellname);
    free(hostname);
  }
  else
    (void)strcpy ((char *)dns_nameserver.ns_Principal_Global,p);

  if ((p = dns_cfgtostr(handle, "security.server_passwd", NULL)) == NULL)
      *passwd = '\0';
  else
      (void)strcpy (passwd, p);
  dns_cfgclose (handle);

  /* Do secure login, register auth info, start refresh id and 
   * manage server key threads.
   */

  /* lock security mutex while we get cell uuid */

  SERVER_LOCK(dns_nameserver.ns_sec_mutex)

   /* Beginning of the 'protected code' */

  if (!server_sec_init (dns_nameserver.ns_Principal_Global,
		       passwd,
		       1,		/* retry */
		       (unsigned char *)dns_nameserver.ns_Realm,
		       &dns_nameserver.ns_Realm_uuid,
		       (unsigned char *)dns_nameserver.ns_Principal,
		       &dns_nameserver.ns_Principal_uuid)) {
    dthread_term();
    dce_svc_printf(CDS_S_SERVER_SEC_EB_MSG);
  }

  /* Get locksmith uuids */
  dns_nameserver.ns_Locksmith_enabled = FALSE;
  if (strlen((char *)dns_nameserver.ns_Locksmith) != 0) {
    auth.info_type = sec_rgy_bind_auth_dce;
    auth.tagged_union.dce_info.authn_level = rpc_c_authn_level_default;
    auth.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_private;
    auth.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth.tagged_union.dce_info.identity = dns_nameserver.ns_login_context;

    sec_rgy_site_bind((unsigned_char_t *)"", &auth, &context, &st);
    if (st != error_status_ok) {
      dce_svc_printf(CDS_S_SECRGYCELLBIND_MSG, (long)st);
      dthread_term();
      dce_svc_printf(CDS_S_SERVER_SEC_EB_MSG);
    } else {
      sec_id_parse_name(context, dns_nameserver.ns_Locksmith, NULL,
                        &dns_nameserver.ns_Locksmith_realm, NULL,
                        &dns_nameserver.ns_Locksmith_princ, &st);
      if (st != error_status_ok) {
	dce_svc_printf(CDS_S_SECIDPARSENAME_MSG, (long)st);
	dthread_term();
	dce_svc_printf(CDS_S_SERVER_SEC_EB_MSG);
    } else
        dns_nameserver.ns_Locksmith_enabled = TRUE;
      sec_rgy_site_close(context, &st);
    }
  }
  /* End of the 'protected code' */

  SERVER_END_LOCK(dns_nameserver.ns_sec_mutex)

  /*
   * Initialize the server managment DACL and its mutex 
   */
  if (server_mgmt_acl_init () != error_status_ok)
  {
      dthread_term();
      dce_svc_printf(CDS_S_SERVER_DACL_EB_MSG);
  }
    
  /*
   * Let other threads know that security initialization is complete.
   */
  SERVER_LOCK(dns_nameserver.ns_sec_mutex)

   /* Beginning of the 'protected code' */

  dns_nameserver.ns_sec_state = cds_sec_On;
  {
      if ((thstatus = dthread_broadcast(&(dns_nameserver.ns_sec_state_changed))) < 0)
      {
	dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
      }
  }

  /* End of the 'protected code' */

  SERVER_END_LOCK(dns_nameserver.ns_sec_mutex)

  status = (dthread_address_t)TRUE;

leave_sec_init_thread:
 
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug3,
      " <sec_init_thread() return(status=%p)",
      status));
  
  return(status);
}

#endif /* DCE_SEC */
