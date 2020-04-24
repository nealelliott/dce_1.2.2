/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: adver_service_rpc.c,v $
 * Revision 1.1.8.2  1996/02/18  19:23:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:47  marty]
 *
 * Revision 1.1.8.1  1995/12/08  00:10:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:15  root]
 * 
 * Revision 1.1.5.10  1994/09/06  17:36:53  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:28:32  proulx]
 * 
 * 	fix timestamps
 * 
 * 	fix timestamps
 * 
 * 	fix timestamps
 * 
 * Revision 1.1.5.9  1994/08/16  17:27:07  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Replace CALockGlobal() by CALockData(),
 * 	CAUnlockGlobal() by CAUnlockData().
 * 	[1994/08/16  15:29:30  peckham]
 * 
 * Revision 1.1.5.8  1994/08/03  18:58:53  mccann
 * 	include cleanup
 * 	[1994/08/01  18:54:59  mccann]
 * 
 * Revision 1.1.5.7  1994/07/25  15:12:15  proulx
 * 	Use common ACL manager for subcomponent mgmt ACLs
 * 	[1994/07/25  14:03:39  proulx]
 * 
 * Revision 1.1.5.6  1994/06/30  19:15:09  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:35:28  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.5.5  1994/06/09  16:06:52  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:24  devsrc]
 * 
 * Revision 1.1.5.4  1994/05/25  22:22:20  peckham
 * 	In adv_disable_clerk() wait for rpc rundown before exiting.
 * 	[1994/05/25  22:17:55  peckham]
 * 
 * Revision 1.1.5.3  1994/05/12  21:10:19  peckham
 * 	Remove call to dnsCleanup().
 * 	[1994/05/12  19:12:09  peckham]
 * 
 * Revision 1.1.5.2  1994/04/19  17:05:16  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:24:10  mccann]
 * 
 * Revision 1.1.5.1  1994/03/12  21:54:56  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:45:08  peckham]
 * 
 * Revision 1.1.2.4  1993/01/07  21:58:28  keegan
 * 	Fixed 'clear cached server' command so that the clearinghouse is removed
 * 	from the LRU list instead of marked 'unsticky'
 * 	[1993/01/06  18:51:45  keegan]
 * 
 * Revision 1.1.2.3  1992/12/30  12:35:09  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:17:41  zeliff]
 * 
 * Revision 1.1.2.2  1992/10/15  19:46:08  m_sawyer
 * 	return new cached server error status codes
 * 	[1992/10/15  19:43:09  m_sawyer]
 * 
 * Revision 1.1  1992/01/19  15:26:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
** Module: adver_service_rpc.c
**
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

#include <adver.h>
#include <dce/rpc_cvt.h>
#include <dce/rpc.h>
#include <dce/rdaclifv0.h>
#include <dce/rdaclif.h>
#include <dce/cprpc_clerk.h>
#include <cache.h>
#include <adver_ncl_defs.h>
#include <dce/dnsmanpar.h>

#if defined(DCE_SEC)
#include <dpeacl.h>
#endif

#define COPY_cds_ReplicaPointer(s,d) \
    COPY_bytes((s), (d), (&(s)->rp_towers[(s)->rp_length] - ((idl_byte *)(s))))

#define cdsReplicaPointerSize(tp) \
    (sizeof(*(tp)) - sizeof((tp)->rp_towers) + (tp)->rp_length)

/*
 * External Global Declarations
 */
extern char promiscuous;              /* boolean TRUE means we should */
                                      /* assume we are performing */
                                      /* configuration duties */
extern uuid_t clerk_mgmt_dacl_uuid;
extern int sharedMemId;

/*
 * Forward declarations/prototypes for internal (static) functions 
*/
static command_status_t 
adv_show_clerk (
   cprpc_attr_list_t        *, 
   handle_t                  );

static command_status_t 
adv_disable_clerk (
   handle_t                  );

static command_status_t 
adv_show_cached_ch (
   clh_info_list_t         **, 
   handle_t                  );

static command_status_t 
show_cached_ch (
   CleEntry_t               *,
   clh_info_t              **);

static command_status_t 
adv_define_cached_server (
   ns_info_list_t          **, 
   handle_t                  );

static command_status_t 
adv_show_cached_server (
   ns_info_list_t          **,
   handle_t                  );

static command_status_t 
adv_clear_cached_server (
   ns_info_list_t          **, 
   handle_t                  );

static command_status_t 
adv_get_clh_list (
   clh_info_list_t         **, 
   handle_t                  );

static void 
func_reads (
   cprpc_attr_t             *,
   CleEntry_t               *);

static void 
func_writes (
   cprpc_attr_t             *,
   CleEntry_t               *);

static void 
func_misc (
   cprpc_attr_t             *,
   CleEntry_t               *);

static void 
func_create_time (
   cprpc_attr_t             *,
   CleEntry_t               *);

static CleEntry_t *
first_cs_entry (
   void                      );

static CleEntry_t *
next_cs_entry (
   void                      );

static CleEntry_t *
find_cs_entry (
   cds_Name_t               *);


/*
 *****************************************************************************
 * C D S _ C L E R K
 *
 * CDS CLERK entity structure
 */
extern struct nsgbl_clerk cds_clerk;

/*
 *****************************************************************************
 * C L H _ A T T R S [ ]
 *
 * ClearingHouse attributes structure array
 */
static struct CLH_ATTRS
{
    int attr_id;
    void (*attr_func) (cprpc_attr_t *,
		       CleEntry_t   *);
}         clh_attrs[] =

{
    { REMOTE_CH_CREATE_TIME,     func_create_time },
    { REMOTE_CH_READ_OPS_COUNT,  func_reads },
    { REMOTE_CH_WRITE_OPS_COUNT, func_writes },
    { REMOTE_CH_MISC_OPS_COUNT,  func_misc }
};
#define NUM_CLE_ATTRS (sizeof(clh_attrs)/sizeof(struct CLH_ATTRS))

/*
**++
**
**  ROUTINE NAME:       dnscp_clerk
**
**  SCOPE:              PUBLIC - declared in cprpc_clerk.idl
**
**  DESCRIPTION:
**      
**  This routine is responsible for dispatching CDS clerk commands from
**  the CDS Control Program.  These commands are received via the RPC
**  mechanism.
**
**  INPUTS:
**
**      h               handle_t for this routine
**      func            function code
**      caller_id       caller's user id
**      attr_list_p     pointer to clerk attribute list
**      clh_info_pp     pointer to pointer to clearinghouse list
**      ns_info_pp      pointer to pointer to nameserver list
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUT:
**
**      command_status  pointer to return status
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      return          success/failure status code
**                      0 = success
**                      other = failure
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC error_status_t 
dnscp_clerk (handle_t            h,
             signed32            func,
             signed32            caller_id,
             cprpc_attr_list_t   *attr_list_p,
             clh_info_list_t     **clh_info_pp,
             ns_info_list_t      **ns_info_pp,
             command_status_p_t  command_status)
{
    command_status_t cmd_status;
    int old_cancel;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   " >dnscp_clerk()"));

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug2,
		   "  Incoming Management command %ld", (long)func));

    /*
     * vector to the appropriate function handler
     */
    old_cancel = dthread_setcancel (CANCEL_OFF);
    switch (func)
    {
        /*
	 * clerk commands
	 */
        case SHOWCLK:
        {
            cmd_status = adv_show_clerk (attr_list_p, h);
            break;
        }

        case DISCLK:
        {
            cmd_status = adv_disable_clerk (h);
            break;
        }

	/*
	 * clerk remote clearinghouse
	 */
        case SHOWCLKCLE:
        {
            cmd_status = adv_show_cached_ch (clh_info_pp, h);
            break;
        }
        
        /*
         * cached server commands
         */
        case DEFCLKSRV:
        {
            cmd_status = adv_define_cached_server (ns_info_pp, h);
            break;
        }

        case SHOWCLKSRV:
        {
            cmd_status = adv_show_cached_server (ns_info_pp, h);
            break;
        }

        case CLEARCLKSRV:
        {
            cmd_status = adv_clear_cached_server (ns_info_pp, h);
            break;
        }

        /*
         * special commands
         */
        case GETCLHLIST:
        {
            cmd_status = adv_get_clh_list (clh_info_pp, h);
            break;
        }

        default:
        {
            cmd_status = DNS_ERROR;
            break;
        }
    }
    dthread_setcancel (old_cancel);

    /*
     * return with status
     */
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver,  svc_c_debug4,
		   " <dnscp_clerk() return(%lx)", (long)cmd_status));

    *command_status = cmd_status;
    return (rpc_s_ok);
}


/*
**++
**
**  ROUTINE NAME:       adv_show_clerk
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      This routine handles the SHOW CLERK command.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      attr_list_p     pointer to an attribute list
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     return status
**
**  SIDE EFFECTS:       none
**
**--
*/

static command_status_t 
adv_show_clerk (cprpc_attr_list_t  *attr_list_p,
                handle_t           h)
{
    command_status_t command_status;   /* our return status */
    ClerkStates off_state;
    cprpc_attr_t *attrp;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " >adv_show_clerk()"));
#if defined(DCE_SEC)
  /* check client for read access to clerk */
    if (!(TestAdverUserAccess(h, sec_acl_perm_read)))
	return(DNS_ACCESSDENIED);
#endif

    /*
     * Initialization
     */
    command_status = DNS_SUCCESS;
    off_state = Off;
    attrp = attr_list_p->attr_members[0];
    ZERO_bytes (&attrp->value.av_val, sizeof (attrp->value.av_val));
    attrp->value.av_valuetype = -1;

    /*
     * Lock the cache info structure while we access it
     */
    CAHeaderLock ();

    /*
     * switch on the particular attribute id that was requested
     */
    switch (attrp->attr_id)
    {
	case CDS_CLERK_CREATE_TIME:
	    {
		attrp->value.av_valuetype = CP_utctime;
		memcpy((char *)attrp->value.av_val.av_utctime,
		       (char *)&cds_clerk.cl_create_time, sizeof(utc_t));
		break;
	    }

	    /* counters */
	case CDS_CLERK_PROTOCOL_ERROR_COUNT:
	    {
		if (!CAfixed_p) break;
		attrp->value.av_valuetype = CP_counter;
		attrp->value.av_val.av_counter = CAfixed_p->ca_protocolerrors;
		break;
	    }
	case CDS_CLERK_AUTHENTICATION_COUNT:
	    {
		if (!CAfixed_p) break;
		attrp->value.av_valuetype = CP_counter;
		attrp->value.av_val.av_counter = CAfixed_p->ca_authenticationfailure;
		break;
	    }
	case CDS_CLERK_READ_OPS_COUNT:
	    {
		if (!CAfixed_p) break;
		attrp->value.av_valuetype = CP_counter;
		attrp->value.av_val.av_counter = CAfixed_p->ca_readoperations;
		break;
	    }
	case CDS_CLERK_CACHE_HITS_COUNT:
	    {
		if (!CAfixed_p) break;
		attrp->value.av_valuetype = CP_counter;
		attrp->value.av_val.av_counter = CAfixed_p->ca_hits;
		break;
	    }
	case CDS_CLERK_CACHE_BYPASS_COUNT:
	    {
		if (!CAfixed_p) break;
		attrp->value.av_valuetype = CP_long;
		attrp->value.av_val.av_counter = CAfixed_p->ca_bypasses;
		break;
	    }
	case CDS_CLERK_WRITE_OPS_COUNT:
	    {
		if (!CAfixed_p) break;
		attrp->value.av_valuetype = CP_counter;
		attrp->value.av_val.av_counter = CAfixed_p->ca_writeoperations;
		break;
	    }
	case CDS_CLERK_MISC_OPS_COUNT:
	    {
		if (!CAfixed_p) break;
		attrp->value.av_valuetype = CP_counter;
		attrp->value.av_val.av_counter = CAfixed_p->ca_miscoperations;
		break;
	    }
	default:
            {
                command_status = DNS_ERROR;
                break;
            }
    }

    /*
     * Check for a failure to get the correct attribute
     */
    if (attrp->value.av_valuetype == -1)
    {
	command_status = DNS_ERROR;
    }

    /*
     * Unlock cache  info structure and return
     */
    CAHeaderUnlock ();

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " <adv_show_clerk() return(%lx)", (long)command_status));
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       adv_disable_clerk
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      This routine handles the DISABLE CLERK command.  It starts
**      up a thread that disables and shuts down the advertiser.
**
**  INPUTS:
**
**      h               handle_t for this routine
**
**--
*/

static command_status_t 
adv_disable_clerk (handle_t h)
{
    command_status_t command_status = DNS_SUCCESS;
    static dthread_t    thread_id;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " >adv_disable_clerk()"));

    /*
     * check for priv's
     */
#if defined(DCE_SEC)
  /* check client for write access to clerk */
    if (!(TestAdverUserAccess(h, sec_acl_perm_write)))
	return(DNS_ACCESSDENIED);
#endif

    /*
     * kick the main loop through a signal
     *
     * I hope we get out of here before the building collapses.
     */
    kill(getpid(), SIGUSR1);

    /*
     * Wait for final collapse
     */
    adver_rpc_rundown();

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " <adv_disable_clerk() return(%lx)", (long)command_status));
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       adv_show_cached_ch
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      This routine handles the SHOW CACHED CLEARINGHOUSE command.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      clh_info_pp     pointer to a pointer to clearinghouse info
**
**              IN:     name of clearinghouse to show or wildcard
**              OUT:    list of clearinghouses that match input name
**                      and their attributes.
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     return status
**
**  SIDE EFFECTS:       none
**
**--
*/

static command_status_t 
adv_show_cached_ch (clh_info_list_t  **clh_info_list_pp,
                    handle_t         h)
{
    command_status_t command_status;	/* our return status */
    CleEntry_t *cle_p;
    int clh_count, cle_handle, index;
    clh_info_t *clh_info_p;
    clh_info_list_t *clh_info_list_p;
    int num_bytes;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " >adv_show_cached_ch()"));

#if defined(DCE_SEC)
  /* check client for read access to clerk */
    if (!(TestAdverUserAccess(h, sec_acl_perm_read)))
	return(DNS_ACCESSDENIED);
#endif

    /*
     * Initialization
     */
    command_status = DNS_SUCCESS;

    /*
     * Lock the global part of the cache while we read clearinghouse
     * info from it
     */
    CALockData ();

    /*
     * Get all clearinghouses
     * First count no. clearinghouses in the cache
     */
    for (cle_p=NULL, cle_handle=0, clh_count=0;
         cle_p = CANextCle (cle_p, &cle_handle);
         clh_count++)
	;

    /*
     * allocate and init clearinghouse info list
     */
    num_bytes = sizeof(clh_info_list_t) + ((clh_count-1) *
                                           sizeof(clh_info_p_t));
    *clh_info_list_pp = clh_info_list_p = 
        (clh_info_list_t *)rpc_ss_allocate (num_bytes);
    if (clh_info_list_p == NULL)
    {
        CAUnlockData ();
        return (DNS_NONSRESOURCES);
    }
    ZERO_bytes (clh_info_list_p, num_bytes);
    clh_info_list_p->clh_count = clh_count;  /* no. of clearinghouses */

    /*
     * Now go back and get info from cache
     */
    for (cle_p=NULL, cle_handle=0, index=0;
         (cle_p = CANextCle (cle_p, &cle_handle)) &&
         (command_status == DNS_SUCCESS);
         index++)
    {
        /*
         * Get info for this clearinghouse
         */
	command_status = show_cached_ch (cle_p, &clh_info_p);

        /*
         * Append info to clearinghouse info list
         */
	clh_info_list_p->clh_members[index] = clh_info_p;
    }

    /*
     * unlock cache and return status
     */
    CAUnlockData ();
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " >adv_show_cached_ch() return(%lx)", (long)command_status));
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       show_cached_ch
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      TBS
**
**  INPUTS:
**
**      cle_p           clearinghouse cache entry
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      clh_info_pp     ptr to ptr clearinghouse info structure (malloc'd)
**
**
**--
*/

static command_status_t 
show_cached_ch (CleEntry_t  *cle_p,
                clh_info_t  **clh_info_pp)
{
    int                    index;
    cprpc_attr_t          *attr_p;
    clh_info_t            *clh_info_p;
    command_status_t       command_status;
    int                    num_bytes;
    Set_t                 *tower_set_p;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " >show_cached_ch()"));

    /*
     * Initialize
     */
    command_status = DNS_NONSRESOURCES; /* assume the worst */
    tower_set_p = (Set_t *) SKIP_FullName (cle_p->ch_name);

    /*
     * Allocate a clearinghouse info structure
     */
    num_bytes = sizeof (clh_info_t);
    *clh_info_pp = clh_info_p = (clh_info_t *) rpc_ss_allocate (num_bytes);
    if (clh_info_p == NULL)
        goto checkout;
    ZERO_bytes (clh_info_p, num_bytes);

    /*
     * Allocate an RPC clearinghouse replica structure
     */
    num_bytes = sizeof (cds_ReplicaPointer_t) + LEN_Set (tower_set_p);
    clh_info_p->clh_replica_p =
        (cds_ReplicaPointer_t *) rpc_ss_allocate (num_bytes);
    if (clh_info_p->clh_replica_p == NULL)
        goto checkout;
    ZERO_bytes (clh_info_p->clh_replica_p, num_bytes);

    /*
     * Convert clearinghouse name, cts and towerset to RPC structure
     */
    L_to_R_FullName ((FullName_t *)cle_p->ch_name,
                     &clh_info_p->clh_replica_p->rp_chname);
    COPY_ObjUID (cle_p->ch_id, &clh_info_p->clh_replica_p->rp_chid);
    RPTowerPack (tower_set_p, clh_info_p->clh_replica_p);

    /*
     * Allocate/init an attribute list
     */
    num_bytes = sizeof(cprpc_attr_list_t)+(NUM_CLE_ATTRS * sizeof(cprpc_attr_p_t));
    clh_info_p->attr_list_p = (cprpc_attr_list_t *) rpc_ss_allocate (num_bytes);
    if (clh_info_p->attr_list_p == NULL)
        goto checkout;
    ZERO_bytes (clh_info_p->attr_list_p, num_bytes);
    clh_info_p->attr_list_p->attr_len = NUM_CLE_ATTRS;

    /*
     * For each of the attributes, allocate an attribute structure,
     * fill in its attribute id and value, and add pointer to attribute list.
     */
    for (index=0; index < NUM_CLE_ATTRS; index++)
    {
        num_bytes = sizeof (cprpc_attr_t);
	attr_p = (cprpc_attr_t *) rpc_ss_allocate (num_bytes);
        if (attr_p == NULL)
            goto checkout;
        ZERO_bytes (attr_p, num_bytes);

	attr_p->attr_id = clh_attrs[index].attr_id;
	(clh_attrs[index].attr_func) (attr_p, cle_p); /* call func for value */
	clh_info_p->attr_list_p->attr_members[index] = attr_p;
    }

    /*
     * if we made it this far, we return success
     */
    command_status = DNS_SUCCESS;

    /*
     * if our return status is anything other than success, we free
     * up all of our structures and return in failure
     */
checkout:
    if (command_status != DNS_SUCCESS)
    {
        if (clh_info_p != NULL)
        {
            if (clh_info_p->attr_list_p != NULL)
            {
                for (index=0; index < NUM_CLE_ATTRS; index++)
                {
                    if (clh_info_p->attr_list_p->attr_members[index] != NULL)
                        rpc_ss_free ((idl_void_p_t)clh_info_p->attr_list_p->attr_members[index]);
                }
                rpc_ss_free ((idl_void_p_t)clh_info_p->attr_list_p);
            }
            rpc_ss_free ((idl_void_p_t)clh_info_p);
        }
    }

    /*
     * return status
     */
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " <show_cached_ch() return(%lx)", (long)command_status));
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       adv_define_cached_server
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called from 'dnscp_clerk()', this routine adds cached
**      nameserver information.
**
**  INPUTS:
**
**      ns_info_pp      ptr to ptr to nameserver information list
**
**
**--
*/

static command_status_t 
adv_define_cached_server (ns_info_list_t  **ns_info_pp,
                          handle_t        h)
{
    command_status_t     command_status;
    ns_info_t           *ns_info_p;
    cds_OpenChar_t      *tower_string_valp;
    cds_Name_t          *ns_tag_p;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " >adv_define_cached_server() "));

#if defined(DCE_SEC)
  /* check client for write access to clerk */
    if (!(TestAdverUserAccess(h, sec_acl_perm_write)))
	return(DNS_ACCESSDENIED);
#endif


    /*
     * Get the nameserver information
     */
    ns_info_p = (*ns_info_pp)->ns_members[0];
    ns_tag_p  = ns_info_p->ns_tag_p;

    /*
     * see if the cached server already exists
     */
    if (find_cs_entry (ns_tag_p))
        return (DNS_CSTAGEXISTS);

    /*
     * Get the tower information
     */
    tower_string_valp =
        (*ns_info_pp)->ns_members[0]->attr_list_p->attr_members[0]->value.av_val.av_char_p;

    /*
     * Call the solicit interface, specify an prot_seq and address
     */
    command_status = 
	(command_status_t)cds_SendSolicitServer(tower_string_valp->op_array, 
						ns_tag_p);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " <adv_define_cached_server() return(%lx)",
		   (long)command_status));
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       adv_show_cached_server
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called from 'dnscp_clerk()', this routine displays cached
**      nameserver information.
**
**  INPUTS:
**
**      ns_info_pp      ptr to ptr to nameserver information list
**
**--
*/

#define CS_FNAME_ATTR      0
#define CS_UUID_ATTR       1
#define CS_REPLICA_ATTR    2
#define CS_NUM_ATTRS       3

static command_status_t 
adv_show_cached_server (ns_info_list_t  **ns_info_pp,
                        handle_t        h)
{
    command_status_t    command_status;
    ns_info_list_p_t    ns_info_list_p;
    int                 cs_count;
    int                 need_ns_bytes, need_ap_bytes, need_rp_bytes;
    CleEntry_t         *cle_p;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " >adv_show_cached_server()"));

#if defined(DCE_SEC)
  /* check client for read access to clerk */
    if (!(TestAdverUserAccess(h, sec_acl_perm_read)))
	return(DNS_ACCESSDENIED);
#endif

    /*
     * Initialize
     */
    command_status = DNS_NONSRESOURCES; /* assume the worst */

    /*
     *****************************************************************
     * until proven to be very inefficient, the simplest thing to do *
     * is to just return the entire list of cached servers           *
     *****************************************************************
     */

    /*
     * count the number of entries in the list and the total number
     * of clearinghouse entries
     */
    cs_count=0;
    for (cle_p=first_cs_entry(); cle_p; cle_p=next_cs_entry())
            cs_count++;
    if (cs_count == 0)
    {
        command_status = DNS_CSTAGUNKNOWN;
        goto checkout;
    }

    /*
     * allocate enough memory for the ns_info_list
     */
    need_ns_bytes = sizeof (ns_info_list_t) + sizeof (ns_info_p_t)*(cs_count-1);
    ns_info_list_p = (ns_info_list_p_t) rpc_ss_allocate (need_ns_bytes);
    if (!ns_info_list_p) goto checkout;
    ZERO_bytes (ns_info_list_p, need_ns_bytes);

    /*
     * now work our way down the cached server list and add each
     * entry to the ns_info_list
     */
    for (cle_p=first_cs_entry(); cs_count--; cle_p=next_cs_entry())
    {
        ns_info_p_t             ns_info_p;
        cds_Name_t              *cs_tag_p;
        cds_FullName_t          *cs_fname_p;
        cprpc_attr_t            *attr_p;
        cprpc_attr_list_p_t     attr_list_p;
        cds_ReplicaPointer_t    *replica_p;

        SimpleName_t            *cle_sname_p;
        FullName_t              *cle_fname_p;
        Set_t                   *cle_set_p;

        cle_fname_p = (FullName_t *) cle_p->ch_name;
        cle_set_p   = (Set_t *)SKIP_FullName (cle_fname_p);
        cle_sname_p = (SimpleName_t *)SKIP_Set (cle_set_p);

        /*
         * allocate/initialize any required memory
         */
        ns_info_p     =            (ns_info_t *) rpc_ss_allocate (sizeof (ns_info_t));
        cs_tag_p      =           (cds_Name_t *) rpc_ss_allocate (sizeof (cds_Name_t));
        cs_fname_p    =       (cds_FullName_t *) rpc_ss_allocate (sizeof (cds_FullName_t));

        need_rp_bytes = sizeof (cds_ReplicaPointer_t) + LEN_Set (cle_set_p);
        replica_p     = rpc_ss_allocate (need_rp_bytes);

        need_ap_bytes = sizeof (cprpc_attr_list_t) + ((CS_NUM_ATTRS-1) * sizeof (cprpc_attr_p_t));
        attr_list_p   =    (cprpc_attr_list_t *) rpc_ss_allocate (need_ap_bytes);
        attr_p        =         (cprpc_attr_t *) rpc_ss_allocate (CS_NUM_ATTRS * sizeof (cprpc_attr_t));

        /*
         * sanity check that we successfully allocated enough memory
         * and then clear it out to be on the safe side
         */
        if (!ns_info_p || !cs_fname_p || !attr_list_p || !attr_p || !replica_p) goto checkout;
        ZERO_bytes (ns_info_p,   sizeof (ns_info_t));
        ZERO_bytes (cs_tag_p,    sizeof (cds_Name_t));
        ZERO_bytes (cs_fname_p,  sizeof (cds_FullName_t));
        ZERO_bytes (replica_p,   need_rp_bytes);
        ZERO_bytes (attr_list_p, need_ap_bytes);
        ZERO_bytes (attr_p,      CS_NUM_ATTRS * sizeof (cprpc_attr_t));

        /*
         * process the cached server 'tag'
         */
        cdsExport_SimpleName (NULL, cle_sname_p, cs_tag_p);

        /*
         * process the cached server fullname attribute
         */
        cdsExport_FullName (NULL, cle_fname_p, cs_fname_p);
        attr_p[CS_FNAME_ATTR].attr_id = CP_FullName;
        attr_p[CS_FNAME_ATTR].value.av_val.av_fullname_p = cs_fname_p;
        attr_p[CS_FNAME_ATTR].value.av_valuetype = CP_FullName;

        /*
         * process the cached server directory uuid attribute
         */
        COPY_ObjUID (cle_p->ch_id, &attr_p[CS_UUID_ATTR].value.av_val.av_uuid);
        attr_p[CS_UUID_ATTR].attr_id = CP_uuid;
        attr_p[CS_UUID_ATTR].value.av_valuetype = CP_uuid;

        /*
         * process the tower information attribute
         */
        RPTowerPack (cle_set_p, replica_p);
        attr_p[CS_REPLICA_ATTR].attr_id = CP_ReplicaPointer;
        attr_p[CS_REPLICA_ATTR].value.av_val.av_rp_p = replica_p;
        attr_p[CS_REPLICA_ATTR].value.av_valuetype = CP_ReplicaPointer;

        /*
         * fill in the attribute list
         */
        attr_list_p->attr_len = CS_NUM_ATTRS;
        attr_list_p->attr_members[CS_FNAME_ATTR]   = &attr_p[CS_FNAME_ATTR];
        attr_list_p->attr_members[CS_UUID_ATTR]    = &attr_p[CS_UUID_ATTR];
        attr_list_p->attr_members[CS_REPLICA_ATTR] = &attr_p[CS_REPLICA_ATTR];

        /*
         * add the 'tag' and attribute list to the current ns_info, bump the
         * output array counter and enter the info into the output array
         */
        ns_info_p->ns_tag_p = cs_tag_p;
        ns_info_p->attr_list_p = attr_list_p;

        ns_info_list_p->ns_members[ns_info_list_p->ns_count++] = ns_info_p;
    }

    /*
     * if we made it this far, return the pointer to the information
     * we have gathered and good status
     */
    *ns_info_pp = ns_info_list_p;
    command_status = DNS_SUCCESS;

checkout:

    /*
     * return status
     */
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " <adv_show_cached_server() return(%lx)",
		   (long)command_status));
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       adv_clear_cached_server
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called from 'dnscp_clerk()', this routine removes cached
**      nameserver information.
**
**  INPUTS:
**
**      ns_info_pp      ptr to ptr to nameserver information list
**
**--
*/

static command_status_t 
adv_clear_cached_server (ns_info_list_t  **ns_info_pp,
                         handle_t        h)
{
    command_status_t     command_status;
    ns_info_p_t          ns_info_p;
    cds_Name_t          *ns_tag_p;
    CleEntry_t          *cle_p;


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " >adv_clear_cached_server()"));

#if defined(DCE_SEC)
  /* check client for write access to clerk */
    if (!(TestAdverUserAccess(h, sec_acl_perm_write)))
	return(DNS_ACCESSDENIED);
#endif

    /*
     * Initialize
     */
    command_status = DNS_SUCCESS;       /* be optimistic */

    /*
     * find the requested cached server information and the
     * associated clearinghouse information
     */
    ns_info_p = (*ns_info_pp)->ns_members[0];
    ns_tag_p = ns_info_p->ns_tag_p;
    cle_p = find_cs_entry (ns_tag_p);
    if (!cle_p)
        return (DNS_CSTAGUNKNOWN);

    /*
     * clear the sticky bit
     */
    CARemoveClearinghouse ((FullName_t *)cle_p->ch_name, FALSE);

    /*
     * return status
     */
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " <adv_clear_cached_server() return(%lx)",
		   (long)command_status));
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       adv_get_clh_list
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      This routine returns a list of all clearinghouses in the cache.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      clh_info_pp     pointer to a pointer to clearinghouse info
**
**              OUT:    list of clearinghouses
**
**
**--
*/

static command_status_t 
adv_get_clh_list (clh_info_list_t  **clh_info_list_pp,
                  handle_t         h)
{
    command_status_t command_status;	/* our return status */
    CleEntry_t *cle_p;
    int clh_count, cle_handle, index;
    clh_info_t *clh_info_p;
    clh_info_list_t *clh_info_list_p;
    int num_bytes;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " >adv_get_clh_list()"));

#if defined(DCE_SEC)
    /* check client for read access to clerk */
    if (!promiscuous)
        if (!(TestAdverUserAccess(h, sec_acl_perm_read)))
	    return(DNS_ACCESSDENIED);
#endif

    /*
     * Initialization
     */
    command_status = DNS_SUCCESS;

    /*
     * Lock the global part of the cache while we read clearinghouse
     * info from it
     */
    CALockData ();

    /*
     * First count no. clearinghouses in the cache
     */
    for (cle_p=NULL, cle_handle=0, clh_count=0;
         cle_p = CANextCle (cle_p, &cle_handle);
         clh_count++)
	;
    if (!clh_count)
    { 
        CAUnlockData ();
        return (DNS_ERROR);
    }
    /*
     * allocate and init clearinghouse info list
     */
    num_bytes = sizeof(clh_info_list_t) +
                ((clh_count-1) * sizeof(clh_info_p_t));
    *clh_info_list_pp = clh_info_list_p = 
        (clh_info_list_t *)rpc_ss_allocate (num_bytes);
    if (clh_info_list_p == NULL)
    {
        CAUnlockData ();
        return (DNS_NONSRESOURCES);
    }
    ZERO_bytes (clh_info_list_p, num_bytes);

    /*
     * Now go back and get info from cache
     */
    for (cle_p=NULL, cle_handle=0, index=0;
         (index < clh_count) && (command_status == DNS_SUCCESS);
         index++)
    {
        /*
         * Get a pointer to the next clearinghouse
         */
        cle_p = CANextCle (cle_p, &cle_handle);
        if (!cle_p)
        {
            CAUnlockData ();
            return (DNS_ERROR);
        }

        /*
         * Allocate a clearinghouse info structure
         */
        num_bytes = sizeof (clh_info_t);
        clh_info_p = (clh_info_t *) rpc_ss_allocate (num_bytes);
        if (clh_info_p == NULL) 
        {
            CAUnlockData ();
            return (DNS_NONSRESOURCES);
        }
        ZERO_bytes (clh_info_p, num_bytes);

        /*
         * Get info for this clearinghouse
         */
        num_bytes = LEN_FullName (cle_p->ch_name);
        clh_info_p->clh_name_p = (cds_FullName_t *) rpc_ss_allocate (num_bytes);
        if (clh_info_p->clh_name_p == NULL)
        {
            CAUnlockData ();
            return (DNS_NONSRESOURCES);
        }
        ZERO_bytes (clh_info_p->clh_name_p, num_bytes);

        /*
         * Export the clearinghouse fullname
         */
        cdsExport_FullName (NULL,
                            (FullName_t *) cle_p->ch_name,
                            (cds_FullName_t *) clh_info_p->clh_name_p);

        /*
         * Append info to clearinghouse info list
         * and add it to the total count
         */
	clh_info_list_p->clh_members[index] = clh_info_p;
        clh_info_list_p->clh_count++;
    }

    /*
     * unlock cache and return status
     */
    CAUnlockData ();
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug7,
		   " >adv_get_clh_list() return(%lx)", (long)command_status));
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       func_reads
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Copy clearinghouse READS counter to RPC attribute structure.
**
**  INPUTS:             none
**
**      attr_p          ptr to RPC attributes list
**      cle_p           ptr to clearinghouse structure
**
**--
*/

static void 
func_reads (cprpc_attr_t  *attr_p,
            CleEntry_t    *cle_p)
{
    attr_p->value.av_valuetype = CP_counter;
    attr_p->value.av_val.av_counter = cle_p->ch_reads;
}


/*
**++
**
**  ROUTINE NAME:       func_writes
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Copy clearinghouse WRITES counter to RPC attribute structure.
**
**  INPUTS:             none
**
**      attr_p          ptr to RPC attributes list
**      cle_p           ptr to clearinghouse structure
**
**--
*/

static void 
func_writes (cprpc_attr_t  *attr_p,
             CleEntry_t    *cle_p)
{
    attr_p->value.av_valuetype = CP_counter;
    attr_p->value.av_val.av_counter = cle_p->ch_writes;
}


/*
**++
**
**  ROUTINE NAME:       func_misc
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Copy clearinghouse MISC OPS counter to RPC attribute structure.
**
**  INPUTS:             none
**
**      attr_p          ptr to RPC attributes list
**      cle_p           ptr to clearinghouse structure
**
**
**--
*/

static void 
func_misc (cprpc_attr_t  *attr_p,
           CleEntry_t    *cle_p)
{
    attr_p->value.av_valuetype = CP_counter;
    attr_p->value.av_val.av_counter = cle_p->ch_miscl;
}


/*
**++
**
**  ROUTINE NAME:       func_create_time
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Copy clearinghouse CREATE TIME value to RPC attribute structure.
**
**  INPUTS:             none
**
**      attr_p          ptr to RPC attributes list
**      cle_p           ptr to clearinghouse structure
**
**
**--
*/

static void 
func_create_time (cprpc_attr_t  *attr_p,
                  CleEntry_t    *cle_p)
{
    attr_p->value.av_valuetype = CP_utctime;
    COPY_bytes (&cle_p->ch_createtime,
                attr_p->value.av_val.av_utctime,
                sizeof (utc_t));
}


#if defined(DCE_SEC)

extern boolean clerk_acl_ready;

/* For DCE security, determine if the client identified in the
 * binding handle has the specified permission(s) to the clerk.
 * NOTE: as of 9/25/91, commands which are not 'priveleged' don't
 * use an authenticated binding handle, but the default ACL
 * gives unauthenticated users 'read' access to the clerk.
*/
int 
TestAdverUserAccess (handle_t           h_p,
                     sec_acl_permset_t  permset)
{
  error_status_t st;
  int auth_status = 0;
  int thstatus;


  /*
   * If security is not running on this system, return TRUE 
   */
  DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver,  svc_c_debug6,
		 " >TestAdverUserAccess (%lx)", (long)permset));

  if(clerk_acl_ready)
    dce_acl_is_client_authorized (h_p,
				  &clerk_mgmt_dacl_uuid,
				  &clerk_mgmt_dacl_uuid,
				  (uuid_t *)0,
				  (uuid_t *)0,
				  permset,
				  &auth_status,
				  &st);
  
  DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		 " <TestAdverUserAccess (%lx) = %lx",
		 (long)permset, (long)auth_status));
  return (auth_status);
}

#endif


/*
**++
**
**  ROUTINE NAME:       first_cs_entry
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      This routine looks up the first cached server entry in the
**      cache.
**
**
**  FUNCTION VALUE:
**
**      Returns a pointer to the information, returns NULL if the
**      information was not found.
**
**  SIDE EFFECTS:       none
**
**--
*/

static CleEntry_t  *_cle_p=NULL;
static int          _cle_handle=0;

static CleEntry_t 
*first_cs_entry (void)
{
    /*
     * initialize
     */
    _cle_p=NULL;
    _cle_handle=0;

    /*
     * run through the cache looking for the requested information
     */
    while (_cle_p = CANextCle (_cle_p, &_cle_handle))
        if (_cle_p->ch_flags.status.flags.sticky)
            return (_cle_p);

    /*
     * return whatever we have found, note that his may be nothing
     */
    return (_cle_p);
}

/*
**++
**
**  ROUTINE NAME:       next_cs_entry
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      This routine looks up the next cached server entry in the
**      cache.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      Returns a pointer to the information, returns NULL if the
**      information was not found.
**
**  SIDE EFFECTS:       none
**
**--
*/

static CleEntry_t 
*next_cs_entry (void)
{
    /*
     * run through the cache looking for the requested information
     */
    while (_cle_p = CANextCle (_cle_p, &_cle_handle))
        if (_cle_p->ch_flags.status.flags.sticky)
            return (_cle_p);

    /*
     * return whatever we have found, note that his may be nothing
     */
    return (_cle_p);
}

/*
**++
**
**  ROUTINE NAME:       find_cs_entry
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      This routine looks up the cached server information for a
**      given cds (simple) name.
**
**  INPUTS:
**
**      mgmt_namep      pointer to the cached server name
**
**  FUNCTION VALUE:
**
**      Returns a pointer to the information, returns NULL if the
**      information was not found.
**
**  SIDE EFFECTS:       none
**
**--
*/

static CleEntry_t 
*find_cs_entry (cds_Name_t *mgmt_namep)
{
    CleEntry_t *cle_p;
    SimpleName_t mgmt_sname;

    cdsImport_SimpleName (NULL, mgmt_namep, &mgmt_sname, 0);

    /*
     * run through the cache looking for the requested information
     */
    for (cle_p=first_cs_entry(); cle_p; cle_p=next_cs_entry())
    {
        Set_t        *set_p;
        SimpleName_t *sname_p;

        set_p = (Set_t *)SKIP_FullName (cle_p->ch_name);
        sname_p = (SimpleName_t *)SKIP_Set (set_p);

        if (EQ_SimpleName (&mgmt_sname, sname_p))
            return (cle_p);
    }

    /*
     * return whatever we have found, note that his may be nothing
     */
    return (cle_p);
}

