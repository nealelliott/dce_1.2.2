/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: adver_cds_event.c,v $
 * Revision 1.1.4.2  1996/02/18  19:23:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:09:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:09  root]
 * 
 * Revision 1.1.2.13  1994/08/25  19:30:33  proulx
 * 	Support for V1.1 delegation and some cleanup.
 * 	[1994/08/25  19:06:35  proulx]
 * 
 * Revision 1.1.2.12  1994/08/23  19:57:51  mccann
 * 	add annotation strings OT:11766
 * 	[1994/08/23  19:35:43  mccann]
 * 
 * Revision 1.1.2.11  1994/08/11  20:35:08  peckham
 * 	Use rpc_ss_client_free().
 * 	[1994/08/11  04:45:04  peckham]
 * 
 * Revision 1.1.2.10  1994/07/25  15:12:11  proulx
 * 	Use common ACL manager for subcomponent mgmt ACLs
 * 	[1994/07/25  14:03:00  proulx]
 * 
 * Revision 1.1.2.9  1994/07/08  17:13:23  peckham
 * 	CR#10088 - coredump on machine with 5 interfaces
 * 	Okay, bite the bullet and use TRY/CATCH around cds_Advertise().
 * 	The core dump is caused by an uncaught rpc_s_in_args_too_big
 * 	exception. The fancy way around this is pop off towers in an
 * 	intelligent manner until things fit.
 * 	[1994/07/08  15:54:28  peckham]
 * 
 * Revision 1.1.2.8  1994/06/30  19:14:59  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:34:48  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.2.7  1994/06/09  16:06:45  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:17  devsrc]
 * 
 * Revision 1.1.2.6  1994/05/25  22:22:13  peckham
 * 	Reorganize rpc termination to allow "disable clerk" to work.
 * 	Use rpc_mgmt_stop_server_listening() to shutdown listener,
 * 	and remove thread_cancel() of listener thread.
 * 	The listener thread will complete when all outstanding
 * 	requests (e. g. "disable clerk") complete.
 * 	Disable clerk will use adver_rpc_rundown() to determine
 * 	when to complete.
 * 	Remove unsupported srv_rpc_binding.
 * 	[1994/05/25  22:17:32  peckham]
 * 
 * Revision 1.1.2.5  1994/05/12  21:09:58  peckham
 * 	Thread locking routine no longer necessary.
 * 	Add TerminateRPC() to unregister endpoints.
 * 	cds_insert_cell() goes to adver_cache.c
 * 	Use new routines where appropriate.
 * 	[1994/05/12  19:11:38  peckham]
 * 
 * Revision 1.1.2.4  1994/04/19  17:05:05  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:22:47  mccann]
 * 
 * Revision 1.1.2.3  1994/03/22  19:59:50  griffin
 * 	HCell BL3 support: Modifies concept of local cell so that multiple cell
 * 	names are accepted for a cell name (uses cell UID as the key).   This is
 * 	essentially an extension of the cell name advertisement filtering
 * 	already in place.
 * 	[1994/03/22  16:48:49  griffin]
 * 
 * Revision 1.1.2.2  1994/03/19  18:34:47  jd
 * 	Don't have cds_SolicitServer_s debug print out cellname and uuid arguments
 * 	on the was in. They are outbut parameters and are uninitialized on
 * 	input. In particular the name length for the cellname may be huge.
 * 	[1994/03/19  16:46:27  jd]
 * 
 * Revision 1.1.2.1  1994/03/12  21:54:42  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  20:01:22  peckham]
 * 
 * $EndLog$
 */
/*
 * 	Module: adver_cds_event.c
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
 */

#include <adver.h>
#include <pwd.h>
#include <sys/wait.h>
#include <dce/rpc.h>
#include <dce/rpcexc.h>
#include <dce/rgynbase.h>
#include <cache.h>
#include <uc_child.h>
#include <dns_cfg.h>
#include <rpcserver.h>
#include <rpc_cvt.h>
#include <cprpc_clerk.h>
#ifdef DCE_SEC
# include <dce/rdaclifv0.h>
# include <dce/rdaclif.h>
# include <dce/dce_cf.h>
#endif

#define CH_NONE 0

extern char stand_alone;
extern char promiscuous;


#ifdef DCE_SEC

sec_rgy_name_t Clerk_Principal;

#endif
/*
 * advertiser servicibility object uuid
 */
static   char *clerk_mgmt_dacl_string = "dc8c6fc0-6143-11ca-b4b9-08002b1bb4f5";
uuid_t 	       clerk_mgmt_dacl_uuid;
extern   serviceability_v1_0_epv_t dce_svc_ep;


/*
 * RPC support in the advertiser.
 *
 * The main bulk of RPC support is included here
 *
 * We receive a set of advertisements from the CDS Server
 * through dnsAdvertise(). This incarnation only propagates
 * V2 packets, caching them in adv_pc.pc_list through
 * L_to_rpcList_AdvertiseMessage_V2(), and broadcasting them
 * when cds_SendAdvertisements() is called. The adv_pc.pc_list cache
 * is flushed through cds_clear_rpcList(). Incoming advertisements
 * are fielded by cds_Advertise_s(), which updates the Clerk cache.
 * Timer and requests call cds_SendSolicit() to request advertisements.
 * The solicit request is fielded by cds_Solicit_s(), which
 * calls cds_SendAdvertisements(). Simple!
 *
 * The associated support routines are:
 *	adver_rpc_start()	Start the RPC server
 *	cds_solicit_kill()	Kill the RPC server
 * Local support routines:
 *	cds_solicit_thread()	The initial RPC server thread
 *	cds_Solicit_s()		Service Solicit requests
 *	cds_Advertise_s()	Service Advertise notifications
 *	cds_SolicitServer_s()	Service manual Solicit request
 *	cds_SendAdvertisements() Send Advertise notification
 *	cds_SendSolicit()	Send Solicit request
 *	adver_cds_bind()	Bind RPC handle for request
 *	cds_clear_rpcList()	Clear advertisement list
 *	L_to_rpcList_AdvertiseMessage_V2() Add advertisement from DNS Server
 *	L_to_R_CH_V2()		Create clearinghouse list for RPC
 *	R_CH_Free()		Free clearinghouse list for RPC
 */

/*
 * RPC-specific defines, data structures, etc.
 */

/*
 * Maximum number of server threads RPC can spawn 
 */
#define ADV_QUEUELENGTH rpc_c_protseq_max_calls_default
/*
 * Local cell for cache lockdown
 */
static char *adv_cache_cellname_p;
static int adv_cache_cellname_len;
static uuid_t adv_cache_cell_diruid;   /* Cell's root directory UUID */

/*
 * Local server's clearinghouses to advertise.
 */
typedef struct rpcList {
    struct rpcList      *rpc_next_p;    /* queue link */
    cds_FullName_t      rpc_cellname;   /* cell directory name */
    uuid_t              rpc_diruid;     /* cell directory ID */
    cds_CH_t            rpc_nscle;      /* list of ReplicaPointers */
                                        /*  -> variable in length */
} rpcList_t;

/*
 * List of advertisements to make on behalf of CDS Server
 */
typedef struct adv_pc {
    dthread_mutex_t     pc_mutex;       /* protect this data */
    dthread_cond_t      pc_cond;        /* following has changed */
    int                 pc_readers;     /* can't modify the list until == 0 */
    int                 pc_locked;      /* -1: mod reqd, 1: modifying,else 0 */
    rpcList_t           *pc_list;       /* queue of rpcList entries */
} adv_pc_t;

/*
 * Server control block.
 *
 * Provides a handle on the server thread. Thread exists (is not detached)
 * as long as this database exists.
 */
typedef struct srv_data {
    dthread_mutex_t     srv_mutex;      /* protect this data */
    dthread_t           srv_thread;     /* server thread handle */
    enum {                              /* state of server: */
        SRV_init,                       /*  initializing */
        SRV_on,                         /*  running */
        SRV_rundown,                    /*  shutting down */
        SRV_off                         /*  killed or init failed */
    }                   srv_state;
    dthread_cond_t      srv_cond;       /* wait for state change */
    void		*cache_p;	/* cache service handle */
} srv_data_t;

static srv_data_t *adv_srv = NULL;

/*
 * All this to protect adv_pc.pc_list
 *
 * We allow many readers, but give priority to writers. Count of active
 * readers is kept in pc_readers, so writer cannot procede until
 * readers are at bay. Readers wait when pc_locked is non-zero.
 * Since the possibility of more than one writer, they must request access
 * when pc_locked is zero, and grab when no readers.
 */
static dthread_once_t rpc_once = dthread_once_init;
static adv_pc_t adv_pc;

/*
 * Prototypes for local routines
 */

static void
adver_cleanup_mutex (
    dthread_mutex_t	*);

static void
cds_bind_cleanup (
    handle_t		*);

static void
cds_Advertise_s (
    handle_t		,
    cds_FullName_t	*,
    uuid_t              ,
    cds_CH_t		*);

static void
cds_Solicit_s (
    handle_t		);

static error_status_t
cds_SolicitServer_s (
    handle_t		,
    cds_FullName_t	*,
    uuid_t		*,
    cds_CHP_t		*);

static void
make_addressHint_timeless (
    ReplicaPointer_t	*);

static int
trim_towers (
    rpcList_t		*);

static dthread_address_t
adver_rpc_thread (
    srv_data_t          *);

static void
adver_cleanup_readers (
    adv_pc_t            *);

static void
cds_advertisement (
    cds_FullName_t      *,
    uuid_t              *,
    cds_CH_t            *,
    cds_Name_t          *);

static byte_t *
L_to_R_CH_V2 (
    byte_t              *,
    cds_CH_t            *);

static void
R_CH_Free (
    cds_CH_t            *);


/*
 * advertise/solicit entry point vector
 */ 
static cds_solicit_v1_0_epv_t epv_s = {cds_Solicit_s,
                                       cds_Advertise_s,
                                       cds_SolicitServer_s};


/*
 * Utility macros and routines
 */

#define COPY_cds_FullName(s,d) \
    COPY_bytes((s), (d), (&(s)->fn_name[(s)->fn_length] - ((idl_char *)(s))))

#define COPY_cds_ReplicaPointer(s,d) \
    COPY_bytes((s), (d), (&(s)->rp_towers[(s)->rp_length] - ((idl_byte *)(s))))

/* Allocateing memory to be freed by stubs, use rpc_ss */
static cds_ReplicaPointer_t *
CLONE_cds_ReplicaPointer (cds_ReplicaPointer_t *oldrp_p)
{
    register cds_ReplicaPointer_t *const newrp_p = (cds_ReplicaPointer_t *)
        rpc_ss_allocate(sizeof(*oldrp_p)
                        - sizeof(oldrp_p->rp_towers)
                        + oldrp_p->rp_length);

    if (newrp_p)
        COPY_cds_ReplicaPointer(oldrp_p, newrp_p);

    return(newrp_p);
}

/* Allocateing memory to be freed by stubs, use rpc_ss */
static cds_CH_t *
CLONE_cds_CH (cds_CH_t *oldcle_p)
{
    register cds_CH_t *const newcle_p = (cds_CH_t *)
                rpc_ss_allocate(sizeof(*oldcle_p)
                                - sizeof(oldcle_p->ch_members)
                                + oldcle_p->ch_length
                                  *sizeof(oldcle_p->ch_members[0]));

    if (newcle_p) {
        register int i;

        newcle_p->ch_length = oldcle_p->ch_length;
        for (i = 0; i < oldcle_p->ch_length; i++)
            newcle_p->ch_members[i] =
                        CLONE_cds_ReplicaPointer(oldcle_p->ch_members[i]);
    }
    return(newcle_p);
}


static void 
adver_cleanup_mutex (dthread_mutex_t *mutex_p)
{
    (void)dthread_unlock(mutex_p);
}

static void 
adver_cleanup_readers (adv_pc_t *pc_p)
{
    int thstatus;


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug9,
		   " >adver_cleanup_readers()"));
    /*
     * we are no longer using the list 
     */
    CDS_LOCK(pc_p->pc_mutex)

    if (--pc_p->pc_readers == 0) {
      if ((thstatus = dthread_signal(&(pc_p->pc_cond))) < 0) {
	dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)thstatus);
      }
    }

    CDS_END_LOCK(pc_p->pc_mutex)

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug9,
		   " <adver_cleanup_readers()"));
}

/*
 * Once-only initialization for above static data.
 * Usage:
 *      dthread_once(&rpc_once, rpc_init);
 */
static void 
rpc_init (void)
{
    adv_pc_t *const pc_p = &adv_pc;
    int thstatus;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " >rpc_init()"));

    if ((thstatus = dthread_create_mutex(&pc_p->pc_mutex)) < 0) {
      dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_create_condition(&pc_p->pc_cond)) < 0) {
      dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
    }

    pc_p->pc_list = NULL;
    pc_p->pc_readers = 0;
    pc_p->pc_locked = 0;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " <rpc_init()"));
}
/*
 *++
 *  InitializeRPC ()
 *
 *  Functional Description:
 *	Initializes the RPC interfaces for CDS.
 *      Checks to see if cdsadv is already running.
 *
 *  Inputs:
 *      myname_p = pointer to name of this program
 *	cache_p = cache handler handle.
 *      debug_mode = 1, if shouldn't fork.
 *
 *--
 */
void *
InitializeRPC (char      *myname_p,
               int       debug_mode)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    unsigned int     mode;
    rpc_if_descriptor_t if_descriptors[5];
    srv_data_t       *srv_p;
    unsigned int     temp_status;
    error_status_t   rpc_status, st;
    dns_status_t     status;
    int              thstatus;
    uuid_vector_t    uuids;
    rpc_binding_vector_p_t server_binding_vector;
    int   if_count = 0;
    char  servername[]="cds-clerk";
    int i;


#ifdef DCE_SEC
    char *handle;
    unsigned32 authn_svc = rpc_c_authn_none;
    char *hostprinc;
#endif

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " >InitializeRPC(%s,%d)",
		   myname_p, debug_mode));

    /*
     * Become a daemon, if debug mode not set 
     */
    mode = (debug_mode) ? (rpc_c_server_mode_debug | rpc_c_server_mode_verify): 0;
    
    if (debug_mode == 0) {
      if (cds_server_fork (myname_p, mode, &rpc_status) != 0) {
	goto leave_InitializeRPC;
      }
    }

    /*
     * Get our cell name for later reference.
     *
     * If we are unable to get our cellname and we are in
     * 'promiscuous' mode, we ASSuME that we are being run as part
     * of CDS configuration and we should then 'fudge' a cellname.
     */
    if (!adv_cache_cellname_p) {
      dce_cf_get_cell_name(&adv_cache_cellname_p, &st);
      if (!promiscuous) {
	dce_assert(cds__svc_handle,(st == dce_cf_st_ok));
      }

      if (!adv_cache_cellname_p) 
	adv_cache_cellname_p = "\0";
	
      adv_cache_cellname_len = strlen(adv_cache_cellname_p);
      ZERO_ObjUID(&adv_cache_cell_diruid);
    }

    /*
     * Allocate the advertiser's 'handle' 
     */
    if ((srv_p = (srv_data_t *)dns_malloc(sizeof(*srv_p))) != NULL) {
      ZERO_bytes(srv_p, sizeof(*srv_p));

      if ((thstatus = dthread_create_mutex(&srv_p->srv_mutex)) < 0)
      {
          dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
      }

      if ((thstatus = dthread_create_condition(&srv_p->srv_cond)) < 0)
      {
          dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
      }
      srv_p->srv_state = SRV_init;
      adv_srv = srv_p;
    }

    /*	
     * Call common server initialization routine to register all interfaces
     */
    if_descriptors[if_count].if_handle  = DNScpClerk_v1_0_s_ifspec;
    if_descriptors[if_count].if_epv     = NULL;
    if_descriptors[if_count].object_ids = NULL;
    /* default = processname [pid] */
    if_descriptors[if_count].annotation = 
	(char *)dce_msg_get_msg(cds_s_adver_mgmtif, &rpc_status);
    ++if_count;

    if_descriptors[if_count].if_handle = serviceability_v1_0_s_ifspec;
    if_descriptors[if_count].if_epv    = (rpc_mgr_epv_t *)&dce_svc_ep;
    /*
     * Set up the management Object UUIDs for the advertiser to 
     * export to the RPC end point map.
     */
     uuid_from_string ((unsigned char *)clerk_mgmt_dacl_string,
			&clerk_mgmt_dacl_uuid, &rpc_status);

     if (rpc_status != uuid_s_ok)
     {
         uuid_create_nil(&clerk_mgmt_dacl_uuid, &rpc_status);
     }
	
    uuids.count = 1;
    uuids.uuid[0] = &clerk_mgmt_dacl_uuid;
    if_descriptors[if_count].object_ids = &uuids;
    /* default = processname [pid] */
    if_descriptors[if_count].annotation = 
	(char *)dce_msg_get_msg(cds_s_adver_svcif, &rpc_status);
    ++if_count;

    /*
     * register the solicit/advertiser interface
     */
    if_descriptors[if_count].if_handle  = cds_solicit_v1_0_s_ifspec;
    if_descriptors[if_count].if_epv     = (rpc_mgr_epv_t *)&epv_s;
    if_descriptors[if_count].object_ids = NULL;
    /* default = processname [pid] */
    if_descriptors[if_count].annotation = 
	(char *)dce_msg_get_msg(cds_s_adver_solicitif, &rpc_status);
    ++if_count;
    
#ifdef DCE_SEC
    {
      extern rdaclif_v0_0_epv_t dce_acl_v0_0_epv; 
      extern rdaclif_v1_0_epv_t dce_acl_v1_0_epv; 

      if_descriptors[if_count].if_handle = rdaclif_v0_0_s_ifspec;
      if_descriptors[if_count].if_epv    = (rpc_mgr_epv_t *)&dce_acl_v0_0_epv; 
      if_descriptors[if_count].object_ids = &uuids;
      if_descriptors[if_count].annotation = 
	  (char *)dce_msg_get_msg(cds_s_adver_aclifv0, &rpc_status);
      ++if_count;

      if_descriptors[if_count].if_handle = rdaclif_v1_0_s_ifspec;
      if_descriptors[if_count].if_epv    = (rpc_mgr_epv_t *)&dce_acl_v1_0_epv; 
      if_descriptors[if_count].object_ids = &uuids;
      /* default = processname [pid] */
      if_descriptors[if_count].annotation = 
	  (char *)dce_msg_get_msg(cds_s_adver_aclifv1, &rpc_status);
      ++if_count;
    }     
#endif	/* DCE_SEC */


    if (cds_server_init( myname_p, NULL, if_count, if_descriptors, mode,
			&server_binding_vector, &rpc_status) != 0) {
      goto leave_InitializeRPC;
    }

    /*
     * Correct the entry in the namespace if we have had our host
     * IP address changed since configuration.  export_ns_new uses 
     * the server_binding_vector, so don't free it here.  It doesn't
     * take much space anyway.
     *
     * Exporting the serviceability interface
     */
    export_ns_new (myname_p, servername, serviceability_v1_0_s_ifspec,
		   server_binding_vector, &clerk_mgmt_dacl_uuid, &temp_status);
    if (temp_status != error_status_ok) {
       goto leave_InitializeRPC;
    }

    /*
     * Now lock our global structure and take ourselves to the
     * on state.
     */
    CDS_LOCK(srv_p->srv_mutex)

    srv_p->srv_state=SRV_on;

    if ((thstatus = dthread_signal(&(srv_p->srv_cond))) < 0) {
      dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)thstatus);
    }

    CDS_END_LOCK(srv_p->srv_mutex)

#ifdef DCE_SEC

    /*
     * Register authentication information with rpc.
     * Get host principal name from cds.conf, if any, otherwise get it
     * from dce_cf.db
     */

    handle = dns_cfgopen(NULL, NULL);
    if ((hostprinc = dns_cfgtostr(handle, "security.host_princ_name", NULL)) == NULL) {
      dce_cf_prin_name_from_host (NULL,&hostprinc,&st);
      if (st == dce_cf_st_ok) {
	(void)strcpy((char *)Clerk_Principal, hostprinc);
	free(hostprinc);
      }
    } else {
      /*
       * use principal name from cds.conf 
       */
      (void)strcpy ((char *)Clerk_Principal, hostprinc);
    }

    /*
     * Determine authentication service to use 
     */
    authn_svc = dns_cfgtol(handle, "security.authn_svc", 
			   rpc_c_authn_dce_secret);

    dns_cfgclose (handle);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug2,
		   " Registering auth info"));

    rpc_server_register_auth_info (Clerk_Principal, authn_svc,  NULL,
				     NULL, &st);
      
    if (st != error_status_ok) {
      dce_svc_printf(CDS_S_RPCREGISTERAUTHINFO_MSG, (long)st);
      goto leave_InitializeRPC;
    }
    
    /*
     * Initialize the clerk management DACL
     */
    
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug2,
		   " Initializing ACL"));

    if (clerk_mgmt_acl_init () != error_status_ok) {
      dce_svc_printf(CDS_S_ADVER_ACLINIT_MSG);
      goto leave_InitializeRPC;
    }
    
#endif

leave_InitializeRPC:

    for (i=0; i<if_count; ++i)
	free(if_descriptors[i].annotation);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " <InitializeRPC(%s,%d) return(%p)",
		   myname_p, debug_mode, srv_p));
    return(srv_p);
}
/*
 *++
 *  adver_rpc_start ()
 *
 *  Functional Description:
 *      Starts up a thread to listen for RPCs
 *
 *--
 */
void
adver_rpc_start (void *adver_handle,
		 void *cache_p)
{
    static dthread_attr_t rpc_attr = {"adver_rpc_thread"};
    srv_data_t *srv_p = (srv_data_t *)adver_handle;
    int thstatus;


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " >adver_rpc_start(%p)", adver_handle));

    CDS_LOCK(srv_p->srv_mutex)

    srv_p->cache_p = cache_p;


    if ((thstatus = dthread_create(&srv_p->srv_thread,
				   &rpc_attr,
				   (dthread_start_routine)adver_rpc_thread,
				   (dthread_address_t)srv_p)) < 0) {
      dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
    }

    while (srv_p->srv_state == SRV_init) 
	{
	  pthread_cleanup_push(adver_cleanup_mutex, &srv_p->srv_mutex);
	  
	  if ((thstatus = dthread_wait(&(srv_p->srv_cond),
				       &(srv_p->srv_mutex))) < 0)
	      {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
	      }
	  pthread_cleanup_pop(0);
	}
    
    CDS_END_LOCK(srv_p->srv_mutex)

    /*
     * 	Make sure that adver_rpc_thread() is listening 
     */
    while (srv_p->srv_state == SRV_on) {
      unsigned32 rpc_status;

      if (rpc_mgmt_is_server_listening(NULL, &rpc_status)) 
	break;

      /*
       * no failure modes for a local query 
       */
      dce_assert(cds__svc_handle,(rpc_status == rpc_s_ok));

      (void)dthread_yield();

    } /* end of while */
    

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " <adver_rpc_start(%p)", adver_handle));
}

/*
 * Advertiser rpc listener thread
 */
static dthread_address_t 
adver_rpc_thread (srv_data_t *srv_p)
{
    error_status_t rpc_status;
    dthread_address_t exit_status = 0;
    int thstatus;


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " >adver_rpc_thread(%p)", srv_p));

    /*
     * 	set adver state = "on", signal our creator 
     */
    CDS_LOCK(srv_p->srv_mutex)
    
    srv_p->srv_state = SRV_on;

    if ((thstatus = dthread_signal(&(srv_p->srv_cond))) < 0) {
      dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)thstatus);
    }

    CDS_END_LOCK(srv_p->srv_mutex)

    rpc_server_listen(rpc_c_listen_max_calls_default, &rpc_status);

    if (rpc_status != rpc_s_ok) {
      dce_svc_printf(CDS_S_RPCSERVERLISTEN_MSG, (long)rpc_status);

      exit_status = (dthread_address_t)-1;
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " <adver_rpc_thread(%p) return(%p)",
		   srv_p, exit_status));

    return(exit_status);
}

/*
 * cds_solicit_kill
 *	Kill the server thread.
 */
void 
cds_solicit_kill (void *handle)
{
    srv_data_t *const srv_p = (srv_data_t *)handle;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " >cds_solicit_kill(%p)", handle));

    if (srv_p) {
      rpc_binding_vector_t *bind_p;

      CDS_LOCK(srv_p->srv_mutex)

      if (srv_p->srv_state == SRV_on) {
	error_status_t rpc_status;

	rpc_mgmt_stop_server_listening(NULL, &rpc_status);

	/*
	 * unregister our i/f's with the ep-mapper
	 */
	rpc_server_inq_bindings (&bind_p, &rpc_status);
	if (rpc_status == rpc_s_ok)
	{
	  uuid_vector_t         uuids;

	  uuids.count = 1;
	  uuids.uuid[0] = &clerk_mgmt_dacl_uuid;

	  rpc_ep_unregister (cds_solicit_v1_0_s_ifspec,
			     bind_p, NULL, &rpc_status);
	  rpc_server_unregister_if(cds_solicit_v1_0_s_ifspec,
				   (uuid_t *)0, &rpc_status);

	  rpc_ep_unregister (DNScpClerk_v1_0_s_ifspec,
			     bind_p, NULL, &rpc_status);
	  rpc_server_unregister_if(DNScpClerk_v1_0_s_ifspec,
			     (uuid_t *)0, &rpc_status);

	  rpc_ep_unregister (serviceability_v1_0_s_ifspec,
			     bind_p, &uuids, &rpc_status);
	  rpc_server_unregister_if(serviceability_v1_0_s_ifspec,
			     (uuid_t *)0, &rpc_status);

	  rpc_ep_unregister (rdaclif_v0_0_s_ifspec,
			     bind_p, &uuids, &rpc_status);
	  rpc_server_unregister_if(rdaclif_v0_0_s_ifspec,
			     (uuid_t *)0, &rpc_status);

	  rpc_ep_unregister (rdaclif_v1_0_s_ifspec,
			     bind_p, &uuids, &rpc_status);
	  rpc_server_unregister_if(rdaclif_v1_0_s_ifspec,
			     (uuid_t *)0, &rpc_status);

	  rpc_binding_vector_free(&bind_p, &rpc_status);

	}
      }

      CDS_END_LOCK(srv_p->srv_mutex)
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " <cds_solicit_kill(%p)", handle));
}

/*
 * adver_rpc_rundown
 * wait for rpc service to leave the on state
 */
void
adver_rpc_rundown (void)
{
    srv_data_t *const srv_p = adv_srv;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " >adver_rpc_rundown()"));

    CDS_LOCK(srv_p->srv_mutex)

    while (srv_p->srv_state == SRV_on)
	{
	  int thstatus;

	  pthread_cleanup_push(adver_cleanup_mutex, &srv_p->srv_mutex);
	  
	  if ((thstatus = dthread_wait(&(srv_p->srv_cond),
				       &(srv_p->srv_mutex))) < 0)
	      {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
	      }
	  pthread_cleanup_pop(0);
	}
    
    CDS_END_LOCK(srv_p->srv_mutex)

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " <adver_rpc_rundown()"));
}

/* TerminateRPC()
 *	Unregsiter our bindings
 */
void
TerminateRPC (void *handle)
{
    srv_data_t *const srv_p = (srv_data_t *)handle;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " >TerminateRPC(%p)", handle));

    if (srv_p) {
	dthread_address_t join_status;
	int thstatus;

	/*
	 * 	Change the state to rundown.
	 */
	CDS_LOCK(srv_p->srv_mutex)
	
	srv_p->srv_state = SRV_rundown;

	if ((thstatus = dthread_signal(&(srv_p->srv_cond))) < 0) {
	  dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)thstatus);
	}

	CDS_END_LOCK(srv_p->srv_mutex)

	(void)dthread_join(srv_p->srv_thread, &join_status);
	(void)dthread_detach(&srv_p->srv_thread);

	dthread_delete_condition(&srv_p->srv_cond);
	dthread_delete_mutex(&srv_p->srv_mutex);
	dns_free((char *)srv_p);
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " <TerminateRPC(%p)", handle));
}

/*
 * adver_cds_bind
 *	Create an RPC binding handle.
 */
int 
adver_cds_bind (handle_t         *client_h_p,
                unsigned_char_t  *addr_p,
                error_status_t   *status_p)
{
    char *disp_p = addr_p ? (char *)addr_p : "";
    byte_t *string_binding_p;
    static char *protseq_str = "ncadg_ip_udp";
    int mystatus = FALSE;


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " >adver_cds_bind(%p,%s,%p)", client_h_p,
		   disp_p, status_p));

    (void)dthread_once(&rpc_once, rpc_init);

    /*
     * Create a binding handle
     *
     * NB: we hardwire the use of 'ncadg_ip_udp' because it is the
     *     only protocol sequence that supports "broadcast maybe"
     *     semantics and that is what we require for the
     *     solicit/advertisement process
     */
    rpc_string_binding_compose(NULL, (unsigned char *)protseq_str,
			       addr_p, NULL, NULL,
			       &string_binding_p, status_p);
    if (*status_p == rpc_s_ok) 
    {
      error_status_t free_status;

      rpc_binding_from_string_binding(string_binding_p,
				      client_h_p, status_p);
      rpc_string_free(&string_binding_p, &free_status);
      
      if (*status_p == rpc_s_ok) 
	  mystatus = TRUE;
      else {

	dce_svc_printf(CDS_S_RPCBINDINGFROMSTRING_MSG, (long)*status_p);

      }
    } else {
      dce_svc_printf(CDS_S_RPCSTRINGBINDINGCOMPOSE_MSG, (long)*status_p);

    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " <adver_cds_bind(%p,%s,%p) return(%lx)",
		   client_h_p, disp_p, status_p, (long)mystatus));
    return(mystatus);
}

static void 
cds_bind_cleanup (handle_t *client_h_p)
{
    error_status_t commStatus;

    rpc_binding_free(client_h_p, &commStatus);
}

static int
cds_advertisement_cellmatch (
    cds_FullName_t      *cellname_p,
    uuid_t              *cell_diruid_p)
{
    DEB_ASCII_BUF_cds_FullName(cellnameBuf)
    DEB_ASCII_BUF_ObjUID(diruidBuf)
    register int cell_name_match; /* Cell matching flags */
    register int cell_uuid_match;
    register int cell_match;


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver,  svc_c_debug7,
		   " >cds_Advertise_cellmatch(%s,%s)",
		   deb_ascii_cds_FullName(cellnameBuf,cellname_p),
		   deb_ascii_ObjUID(diruidBuf,cell_diruid_p)));


    /*
     *  Rules for accepting an advertisement:
     *
     *    If the cell name matches, accept.  (On the side, grab it's uuid).
     *    Else see if the uuid advertised matches the configured cell's
     *         uuid.  If it does, accept (this is a cell alias).
     */

    cell_name_match =
        (adv_cache_cellname_p
         && (cellname_p->fn_length == adv_cache_cellname_len)
         && EQ_bytes(cellname_p->fn_name, adv_cache_cellname_p,
                                         adv_cache_cellname_len));

    if (!cell_name_match) {
        cell_uuid_match = EQ_ObjUID(cell_diruid_p, &adv_cache_cell_diruid);
    } else if (ObjUID_is_nil(&adv_cache_cell_diruid)) {
        cell_uuid_match = FALSE;
        COPY_ObjUID(cell_diruid_p, &adv_cache_cell_diruid);
    }

    cell_match = (cell_name_match || cell_uuid_match);

    if (!stand_alone && cell_match) {
      DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		     " >cds_Advertise_cellmatch(%s,%s) return(TRUE)",
		     deb_ascii_cds_FullName(cellnameBuf,cellname_p),
		     deb_ascii_ObjUID(diruidBuf,cell_diruid_p)));
      return (TRUE);
    } else {
      DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		     " >cds_Advertise_cellmatch(%s,%s) return(FALSE)",
		     deb_ascii_cds_FullName(cellnameBuf,cellname_p),
		     deb_ascii_ObjUID(diruidBuf,cell_diruid_p)));
      return (FALSE);
    }
}

/*
 * Process incoming advertisement
 */
static void 
cds_advertisement (cds_FullName_t  *cellname_p,
                   uuid_t          *diruid_p,
                   cds_CH_t        *nscle_p,
                   cds_Name_t      *tag_p)
{
    DEB_ASCII_BUF_cds_FullName(cellnameBuf)
    DEB_ASCII_BUF_cds_Name(tagnameBuf)
    DEB_ASCII_BUF_ObjUID(diruidBuf)
    byte_t		rpset[MAXATTRIBUTE];
    FullName_t		cellname;
    SimpleName_t        tag;
    struct CleFlags	flags;
    void *const		cache_p = adv_srv->cache_p;
    Set_t *const	rpset_p = (Set_t *)rpset;
    MemberValue_t	*mv_p;
    int			chCount, i;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " >cds_advertisement"));
    /*
     * initialize the clearinghouse flags ...
     *
     * if we have not been given a 'tag' then this is not a cached
     * server advertisement and therefore must be 'onLAN'
     *
     * likewise, if we *have* been given a 'tag' then this *is* a
     * cached server advertisement and we should therefore set the
     * 'sticky' bit
     */
    ZERO_bytes(&flags, sizeof(flags));
    flags.status.flags.ok = 1;
    flags.status.flags.addressused = 1;
    if (tag_p)
        flags.status.flags.sticky = 1;
    else
        flags.status.flags.onLAN = 1;

    /*
     * 	Get local versions of arguments 
     */
    (void)R_to_L_FullName(cellname_p, &cellname, FALSE);
    ZERO_bytes(&tag, sizeof(tag));
    if (tag_p)
    {
        (void)R_to_L_SimpleName(tag_p, &tag, FALSE);
    }
    chCount = nscle_p->ch_length;
    INS8(rpset_p->st_flag, ST_present);
    INS16(rpset_p->st_offsetLength, chCount*sizeof(word_u));
    mv_p = (MemberValue_t *)DATA_Set(rpset_p);

    /*
     * If clerk state = initializing, cancel the dns_solicit holdown timer
     */
    adver_cache_set_state(cache_p, On);

    /*
     * now look at the clearinghouse (or hice, as the case may be)
     */
    for (i = 0; i < chCount; i++) {
	cds_ReplicaPointer_t *const crp_p = nscle_p->ch_members[i];

	/*
	 * 	Fiddle with Set 
	 */
	INS16(OFFSET_Set(rpset_p, i), DIFF_bytes(mv_p, rpset_p));
	INS8(mv_p->mv_flag, MV_present);
	ZERO_Timestamp(mv_p->mv_timestamp);
	INS8(mv_p->mv_valuetype, VT_ReplicaPointer);

	if (crp_p) {
	    byte_t *msg_p = mv_p->mv_value;
	    Set_t *addressHint_p;
	    FullName_t *CHName_p;

	    /*
	     * 	Create ReplicaPointer 
	     */
	    COPY_ObjUID(&crp_p->rp_chid, msg_p);
	    addressHint_p = (Set_t *)SKIP_ObjUID(msg_p);
	    msg_p = RPTowerUnpack(crp_p->rp_towers, addressHint_p);
	    INS8(msg_p, crp_p->rp_type);
	    CHName_p = (FullName_t *)SKIP_ReplicaType(msg_p);
	    (void)R_to_L_FullName(&crp_p->rp_chname, CHName_p, FALSE);

	    /*
	     * 	add clearinghouse to cache 
	     */
	    adver_cache_add_cle(cache_p, CHName_p, &crp_p->rp_chid,
				addressHint_p, &flags, &tag);

	    INS16(mv_p->mv_length, LEN_ReplicaPointer(mv_p->mv_value));
	} else {
	    CLEAR_FLAG(mv_p->mv_flag, MV_present);
	    INS16(mv_p->mv_length, 0);
	}
	mv_p = (MemberValue_t *)SKIP_MemberValue(mv_p);
    }
    INS16(rpset_p->st_valueLength, DIFF_bytes(mv_p, DATA_Set(rpset_p)));

    if (!tag_p || cds_advertisement_cellmatch(cellname_p, diruid_p)) 
	adver_cache_add_rep(cache_p, &cellname, diruid_p, rpset_p, TRUE);
    else
	adver_cache_add_rep(cache_p, &cellname, diruid_p, rpset_p, FALSE);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " <cds_advertisement"));
}

/*
 * cds_Solicit_s
 *	Solicit message received from RPC.
 */
static void 
cds_Solicit_s (handle_t h)
{
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   " >cds_Solicit_s()"));

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug2,
		   "  Incoming cds_Solicit_s"));
    if (!stand_alone)
        cds_SendAdvertisements();

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   " <cds_Solicit_s()"));

  }

/*
 * cds_Advertise_s
 *	Advertise message received from RPC.
 */
static void 
cds_Advertise_s (handle_t        h,
                 cds_FullName_t  *cellname_p,
                 uuid_t          cell_diruid,
                 cds_CH_t        *nscle_p)
{
    DEB_ASCII_BUF_cds_FullName(cellnameBuf)
    DEB_ASCII_BUF_ObjUID(diruidBuf)

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   " >cds_Advertise_s(,%s,%s,{%d})",
		   deb_ascii_cds_FullName(cellnameBuf,cellname_p),
		   deb_ascii_ObjUID(diruidBuf,&cell_diruid),
		   nscle_p->ch_length));


    /* Only process advertisements from my cell */
    if (promiscuous || (!stand_alone && 
           cds_advertisement_cellmatch(cellname_p, &cell_diruid)) ) {
      DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug2,
		     "  Incoming cell cds_Advertise_s"));
      cds_advertisement(cellname_p, &cell_diruid, nscle_p, NULL);
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   " <cds_Advertise_s(,%s,%s,{%d})",
		   deb_ascii_cds_FullName(cellnameBuf,cellname_p),
		   deb_ascii_ObjUID(diruidBuf,&cell_diruid),
		   nscle_p->ch_length));
}

/*
 * cds_SolicitServer_s
 *	Solicit message received from RPC.
 */
static error_status_t 
cds_SolicitServer_s (handle_t        server_h,
                     cds_FullName_t  *cellname_p,
                     uuid_t          *cell_diruid_p,
                     cds_CHP_t       *nscle_pp)
{
    DEB_ASCII_BUF_cds_FullName(cellnameBuf)
    DEB_ASCII_BUF_ObjUID(diruidBuf)
    adv_pc_t *pc_p = &adv_pc;
    int length = 0;
    int thstatus;


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   "  >cds_SolicitServer_s(,,,)"));

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug2,
		   "  Incoming cds_SolicitServer_s"));

    (void)dthread_once(&rpc_once, rpc_init);

    /*
     * 	make sure the list is not being modified 
     */

    CDS_LOCK(pc_p->pc_mutex)
    
    while(pc_p->pc_locked) {
      pthread_cleanup_push(adver_cleanup_mutex,&pc_p->pc_mutex);

      if ((thstatus = dthread_wait(&(pc_p->pc_cond),
				   &(pc_p->pc_mutex))) < 0) {
	dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
      }
      pthread_cleanup_pop(0);
    }
    pc_p->pc_readers++;

    CDS_END_LOCK(pc_p->pc_mutex)
    
    /*
     * No cancellation points follow, but we use the common routines anyway.
     */
    pthread_cleanup_push(adver_cleanup_readers, pc_p);
    
    /*
     * Return the first clearinghouse set that we have.
     * Copy into caller buffers (stub)
     */
    if (pc_p->pc_list != NULL) {
      rpcList_t *rpc_p;

      rpc_p = pc_p->pc_list;
      COPY_cds_FullName(&rpc_p->rpc_cellname, cellname_p);
      COPY_ObjUID(&rpc_p->rpc_diruid, cell_diruid_p);
      if ((*nscle_pp = CLONE_cds_CH(&rpc_p->rpc_nscle)) != (cds_CHP_t)0)
	  length = (*nscle_pp)->ch_length;
    } else {
      unsigned32 status;

      L_to_R_FullName((FullName_t *)NullFullName, cellname_p);
      uuid_create_nil(cell_diruid_p, &status);
      nscle_pp = NULL;
    }

    pthread_cleanup_pop(1);		/* we are no longer a reader */

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   " <cds_SolicitServer_s(,%s,%s,)",
		   deb_ascii_cds_FullName(cellnameBuf,cellname_p),
		   deb_ascii_ObjUID(diruidBuf,cell_diruid_p)));

    return(rpc_s_ok);
}

/* 
 * cds_SendSolicit
 *   Send a solicit RPC message 
 */
void 
cds_SendSolicit (void)
{
    handle_t client_h;
    error_status_t commStatus;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug5,
		   " >cds_SendSolicit()")); 

    if (adver_cds_bind(&client_h, NULL, &commStatus)) 
    {
      pthread_cleanup_push(cds_bind_cleanup, &client_h);
      DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
		     "  Outgoing cds_Solicit")); 
      cds_Solicit(client_h);
      pthread_cleanup_pop(1);
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug5,
		   " <cds_SendSolicit()"));
}

/*
 * cds_SendAdvertisements
 *	Send advertisements via RPC
 */
void 
cds_SendAdvertisements (void)
{
    adv_pc_t *pc_p = &adv_pc;
    int thstatus;


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug5,
		   " >cds_SendAdvertisements()"));

    if (!stand_alone) {
      (void)dthread_once(&rpc_once, rpc_init);

      /*
       * make sure the list is not being modified 
       */
      
      CDS_LOCK(pc_p->pc_mutex)

      while(pc_p->pc_locked) {
	pthread_cleanup_push(adver_cleanup_mutex,&pc_p->pc_mutex);
	      
	if ((thstatus = dthread_wait(&(pc_p->pc_cond),
				     &(pc_p->pc_mutex))) < 0) {
	  dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
	}
	pthread_cleanup_pop(0);
      }
      pc_p->pc_readers++;

      CDS_END_LOCK(pc_p->pc_mutex)

      pthread_cleanup_push(adver_cleanup_readers, pc_p);

      if (pc_p->pc_list != NULL) {
	handle_t client_h;
	error_status_t commStatus;
	
	if (adver_cds_bind(&client_h, NULL, &commStatus)) {
	  rpcList_t *rpc_p = pc_p->pc_list;
	  
	  pthread_cleanup_push(cds_bind_cleanup, &client_h);
	  
	  while (rpc_p != NULL) {

	    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
			   "  Outgoing cds_Advertise"));
	    TRY {
	      cds_Advertise(client_h, &rpc_p->rpc_cellname,
			    rpc_p->rpc_diruid, &rpc_p->rpc_nscle);
	    
	      /*
	       * Kludge: if we send multiple advertisements (because
	       * we have multiple CH's on the local server), we need
	       * the following delay. This is because RPC currently
	       * checks to see if an RPC is on its call queue (the
	       * prior broadcast) and if so it will flush that one
	       * and will drop the current one.
	       */
	      dthread_delay(1.0);
	      rpc_p = rpc_p->rpc_next_p;

	    } CATCH (rpc_x_in_args_too_big) {

	      /*
	       * In the case of too many towers, try to choose a
	       * subset which will still allow clients to get here.
	       */
	      if (!trim_towers(rpc_p))
		rpc_p = rpc_p->rpc_next_p;

	    } ENDTRY
	  }

	  pthread_cleanup_pop(1);
	}
      }
      pthread_cleanup_pop(1);		/* we are no longer a reader */
    }
    
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug5,
		   " <cds_SendAdvertisements()"));
}

/*
 * cds_SendSolicitServer
 *	Client side of cds_SolicitServer.
 *      Call directed to a specific server.
 *      
 */
error_status_t 
cds_SendSolicitServer (string_binding_p, tag_p)
    unsigned_char_t *string_binding_p;
    cds_Name_t       *tag_p;
{
    DEB_ASCII_BUF_cds_Name(tagBuf)
    handle_t client_h;
    error_status_t     status=DNS_SUCCESS;
    cds_FullName_t cellname;
    uuid_t diruid;
    cds_CH_t *nscle_p = (cds_CH_t *)NULL;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug5,
		   " >cds_SendSolicitServer(%s,%s)",
		   string_binding_p, deb_ascii_cds_Name(tagBuf,tag_p)));

    /*
     * only do the solicit if we successfully got a binding handle
     */
    rpc_binding_from_string_binding(string_binding_p,
				    &client_h, &status);
    if (status == rpc_s_ok) {
      pthread_cleanup_push(cds_bind_cleanup, &client_h);
      /*
       * solicit using "must_complete" form of the
       * solicit function call
       */

      DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
		   "  Outgoing cds_SolicitServer"));
      status = 
	  cds_SolicitServer(client_h, &cellname, &diruid, &nscle_p);

      /*
       * process the advertisement if the solicit was
       * successful; if we don't get a clearinghouse back yet
       * the rpc call was successful, then we assume that the
       * clearinghouse was down
       */
      if (status == rpc_s_ok) {
	if (!nscle_p) {
	  status  = DNS_CLEARINGHOUSEDOWN;
	} else {
	  FullName_t fname;
	  CleEntry_t *cle_p;
	  
	  (void)cdsImport_FullName (NULL,
				    &nscle_p->ch_members[0]->rp_chname,
				    &fname,
				    FALSE);
	  cle_p = CALookupClearinghouse (&fname);
	  if (!cle_p) 
	    /* Process return info as received advertisement */
	    cds_advertisement(&cellname, &diruid, nscle_p, tag_p);
	  else {
	    status = DNS_CSCLHEXISTS;
	    dns_free (cle_p);
	  }
	}
      }

      /*
       * Release memory
       */
      if (nscle_p) {
	R_CH_Free(nscle_p);
	rpc_ss_client_free((char *)nscle_p);
      }

      /*
       * cleanup
       */
      pthread_cleanup_pop(1);
    } /* End got a binding */

    /*
     * return status to caller
     */
    if (status == rpc_s_ok)
	status = DNS_SUCCESS;
    
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug5,
		   " <cds_SendSolicitServer(%s,%s, return(%lx)",
		    string_binding_p, deb_ascii_cds_Name(tagBuf,tag_p), 
		    (long)status));
    return(status);
}

/*
 * cds_clear_rpcList
 *	Frees all rpcList structures from queue header.
 */
void 
cds_clear_rpcList (void)
{
    adv_pc_t *pc_p = &adv_pc;
    rpcList_t *rpc_p;
    int thstatus;


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " >cds_clear_rpcList()"));

    (void)dthread_once(&rpc_once, rpc_init);
    
    /*
     * 	get exclusive control of the list 
     */

    CDS_LOCK(pc_p->pc_mutex)

    while(pc_p->pc_locked) {
      pthread_cleanup_push(adver_cleanup_mutex,&pc_p->pc_mutex);

      if ((thstatus = dthread_wait(&(pc_p->pc_cond),
				   &(pc_p->pc_mutex))) < 0) {
	dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
      }
      pthread_cleanup_pop(0);
    }
    pc_p->pc_locked = -1;

    while (pc_p->pc_readers)  {
      pthread_cleanup_push(adver_cleanup_mutex, &pc_p->pc_mutex);

      if ((thstatus = dthread_wait(&(pc_p->pc_cond),
				   &(pc_p->pc_mutex))) < 0) {
	dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
      }
      pthread_cleanup_pop(0);
    }
    pc_p->pc_locked = 1;

    CDS_END_LOCK(pc_p->pc_mutex)

    for (rpc_p = pc_p->pc_list; rpc_p != NULL; rpc_p = pc_p->pc_list) {
      register int i;

      pc_p->pc_list = rpc_p->rpc_next_p;
      for (i = 0; i < rpc_p->rpc_nscle.ch_length; i++) {
	cds_ReplicaPointer_t *const rp_p = rpc_p->rpc_nscle.ch_members[i];

	if (rp_p)
	    dns_free((char *)rp_p);
      }
      dns_free((char *)rpc_p);
    } 

    /*
     * 	release the list 
     */
    CDS_LOCK(pc_p->pc_mutex)

    pc_p->pc_locked = 0;

    if ((thstatus = dthread_broadcast(&(pc_p->pc_cond))) < 0) {
      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
    }

    CDS_END_LOCK(pc_p->pc_mutex)

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " <cds_clear_rpcList()"));
}

/*
 * L_to_rpcList_AdvertiseMessage_V2
 *	Marshall a V2 AdvertiseMessage into a new rpcList structure
 *
 * AdvertiseMessage message (V2):
 *  header: SAHeader =
 *	version: ProtocolVersion
 *	messageType: MessageType
 *  namespaceUID: ObjUID
 *  namespaceNickname: SimpleName
 *  chCount: CARDINAL
 *  ch: ARRAY[0..chCount-1] OF ReplicaPointer
 */
void 
L_to_rpcList_AdvertiseMessage_V2 (byte_t *msg_p)
{
    adv_pc_t *pc_p = &adv_pc;
    rpcList_t *rpc_p, rpcBuf;
    SimpleName_t *name_p;
    int chCount;
    int thstatus;


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " >L_to_rpcList_AdvertiseMessage_V2(%p)", msg_p));

    /*
     * Parse up to chCount to find out how big this thing is...
     */
    msg_p = SKIP_protocolVersion(msg_p);
    msg_p = SKIP_MessageType(msg_p);
    COPY_ObjUID(msg_p, &rpcBuf.rpc_diruid);
    name_p = (SimpleName_t *)SKIP_ObjUID(msg_p);

    /*
     * Make sure there is a cellname
     */
    if (name_p->sn_name[0] == '/') {
	COPY_ObjUID(cdsWildRoot, &rpcBuf.rpc_cellname.fn_root);
	rpcBuf.rpc_cellname.fn_length = EXT8(name_p->sn_length);
	COPY_bytes(name_p->sn_name, rpcBuf.rpc_cellname.fn_name,
	    rpcBuf.rpc_cellname.fn_length);
	msg_p = SKIP_SimpleName(name_p);
	chCount = EXT8(msg_p);

	/*
	 * 	Allocate enough room for all clearinghouses 
	 */
	rpc_p = (rpcList_t *)dns_malloc(sizeof(*rpc_p)
			- sizeof(rpc_p->rpc_nscle.ch_members)
			+ chCount*sizeof(rpc_p->rpc_nscle.ch_members[0]));
	if (rpc_p) {
	    COPY_bytes(&rpcBuf.rpc_diruid, &rpc_p->rpc_diruid, sizeof(rpc_p->rpc_diruid));
	    COPY_bytes(&rpcBuf.rpc_cellname, &rpc_p->rpc_cellname, sizeof(rpc_p->rpc_cellname));
	    msg_p = L_to_R_CH_V2(msg_p, &rpc_p->rpc_nscle);

	    /*
	     * 	FIX: what to do on failure? 
	     */

	    (void)dthread_once(&rpc_once, rpc_init);

	    /*
	     * 	get exclusive control of the list 
	     */

	    CDS_LOCK(pc_p->pc_mutex)

	    while(pc_p->pc_locked) {
	      pthread_cleanup_push(adver_cleanup_mutex,&pc_p->pc_mutex);

	      if ((thstatus = dthread_wait(&(pc_p->pc_cond),
					   &(pc_p->pc_mutex))) < 0) {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
	      }
	      pthread_cleanup_pop(0);
	    }
	    pc_p->pc_locked = -1;

	    while (pc_p->pc_readers)  {
	      pthread_cleanup_push(adver_cleanup_mutex, &pc_p->pc_mutex);

	      if ((thstatus = dthread_wait(&(pc_p->pc_cond),
					   &(pc_p->pc_mutex))) < 0) {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
	      }
	      pthread_cleanup_pop(0);
	    }
	    pc_p->pc_locked = 1;

	    CDS_END_LOCK(pc_p->pc_mutex)

	    rpc_p->rpc_next_p = pc_p->pc_list;
	    pc_p->pc_list = rpc_p;

	    /*
	     * 	release the list 
	     */
	    CDS_LOCK(pc_p->pc_mutex)
	    pc_p->pc_locked=0;

	    if ((thstatus = dthread_broadcast(&(pc_p->pc_cond))) < 0) {
	      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	    }

	    CDS_END_LOCK(pc_p->pc_mutex)
	  }
      }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " <L_to_rpcList_AdvertiseMessage_V2(%p)", msg_p));
}

/*
 * L_to_R_CH_V2
 *	Marshal a V2 wire-format ReplicaPointers into an RPC structure.
 */
static byte_t 
*L_to_R_CH_V2 (byte_t    *msg_p,
               cds_CH_t  *ch_p)
{
    register int i;

    ch_p->ch_length = GET8(msg_p);

    for (i = 0; i < ch_p->ch_length; i++) {
	cds_ReplicaPointer_t *rp_p = (cds_ReplicaPointer_t *)dns_malloc(
		sizeof(cds_ReplicaPointer_t) + LEN_Set(SKIP_ObjUID(msg_p)));

	if (rp_p) {
	    COPY_ObjUID(msg_p, &rp_p->rp_chid);
 	    msg_p = SKIP_ObjUID(msg_p);
	    msg_p = RPTowerPack((Set_t *)msg_p, rp_p);
	    rp_p->rp_type = GET8(msg_p);
	    msg_p = L_to_R_FullName((FullName_t *)msg_p, &rp_p->rp_chname);
	}
	/*
	 * 	FIX: what to do on failure? 
	 */
	ch_p->ch_members[i] = rp_p;
    }

    return(msg_p);
}

/*
 * R_CH_Free
 *	Release memory associated with ReplicaPointers
 */
static void 
R_CH_Free (cds_CH_t *ch_p)
{
    register int i;

    for (i = 0; i < ch_p->ch_length; i++) {
	cds_ReplicaPointer_t *rp_p;

	if ((rp_p = ch_p->ch_members[i]) != (cds_ReplicaPointer_t *)0) {
	    rpc_ss_client_free((char *)rp_p);
	    ch_p->ch_members[i] = (cds_ReplicaPointer_t *)0;
	}
    }
}

static void 
make_addressHint_timeless (ReplicaPointer_t *rp_p)
{
    Set_t *set_p = (Set_t *)rp_p->rp_addressHint;
    register MemberValue_t *mv_p = (MemberValue_t *)DATA_Set(set_p);
    register int num_set;

    if ((num_set = NUM_Set(set_p)) != 0)
	do {
	    ZERO_Timestamp(mv_p->mv_timestamp);
	    mv_p = (MemberValue_t *)SKIP_MemberValue(mv_p);
	} while (--num_set);
}

/*
 * The towerset was too big for an atomic broadcast message.
 * Trim the set of towers without (hopefully) compromising connectivity.
 * For instance, we can drop tcp towers as long as udp is there.
 */
static int
trim_towers (
    rpcList_t *rpc_p)
{
    /* someone else fill this in */

    return(FALSE);
}
