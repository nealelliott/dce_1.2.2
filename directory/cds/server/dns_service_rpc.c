/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dns_service_rpc.c,v $
 * Revision 1.1.13.1  1996/08/09  12:00:33  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/08/06  14:57 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/2]
 *
 * 	Incorporate fix for OTs 12812, 12802 (hang in dcecp clearinghouse initiate).
 * 	Cleanup FORCEBACKCMD and FORCECHECKCMD if multiple requests occur.
 * 	[1996/02/18  19:36:10  marty  1.1.11.2]
 *
 * Revision 1.1.11.2  1996/02/18  19:36:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:42  marty]
 * 
 * Revision 1.1.11.1  1995/12/08  15:30:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:15  root]
 * 
 * Revision 1.1.8.11  1994/09/06  17:37:10  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:30:47  proulx]
 * 
 * Revision 1.1.8.10  1994/08/25  19:30:52  proulx
 * 	Delegation support. Minor changes.
 * 	[1994/08/25  19:22:48  proulx]
 * 
 * Revision 1.1.8.8  1994/08/19  21:10:52  zee
 * 	   Added repair timestamps and get repair status for badclock fixer.
 * 	   [1994/08/19  18:09:27  zee]
 * 
 * Revision 1.1.8.9  1994/08/24  20:14:30  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:08:42  mccann]
 * 
 * Revision 1.1.8.7  1994/08/03  19:04:12  mccann
 * 	final sams drop
 * 	[1994/08/02  14:09:28  mccann]
 * 
 * Revision 1.1.8.6  1994/08/01  15:07:36  zee
 * 	     Added srv_force_clh and force_clh commands to initiate background
 * 	     and checkpoint threads.
 * 	[1994/07/29  21:04:36  zee]
 * 
 * Revision 1.1.8.5  1994/07/25  15:14:56  proulx
 * 	Use common ACL manager for subcomponent mgmt ACLs
 * 	[1994/07/25  13:57:34  proulx]
 * 
 * Revision 1.1.8.4  1994/06/09  18:43:25  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:57  devsrc]
 * 
 * Revision 1.1.8.3  1994/05/12  21:12:22  peckham
 * 	Use INIT_time_quad() and COPY_time_quad_to_hyper() macros.
 * 	Also use EXPORT_MGMT_COUNTER() in places appropriate.
 * 	[1994/05/12  19:18:26  peckham]
 * 
 * Revision 1.1.8.2  1994/04/25  19:57:23  ohara
 * 	add stdio.h
 * 	[1994/04/20  19:46:40  ohara]
 * 
 * Revision 1.1.8.1  1994/03/12  22:11:26  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:42:48  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  14:21:47  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:22:01  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  19:15:40  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:16:56  weisman]
 * 
 * Revision 1.1.2.5  1992/06/01  15:24:41  mfox
 * 	Replace reference of BSD strings.h with ANSI string.h
 * 	[1992/06/01  11:28:31  mfox]
 * 
 * Revision 1.1.2.4  1992/05/13  12:32:57  keegan
 * 	Function 'user_setup' now checks for errors from 'net_LocalName' and
 * 	returns an error itself.  'DNS_NAMESERVER error returned if user_setup
 * 	fails.
 * 	[1992/05/12  20:18:34  keegan]
 * 
 * Revision 1.1.2.3  1992/03/22  22:31:05  weisman
 * 	Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:35:15  weisman]
 * 
 * Revision 1.1.2.2  1992/01/22  23:02:12  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:17:50  melman]
 * 
 * $EndLog$
 */
/*
** Module: dns_service_rpc.c
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
 *
*/

/*
 * Include any special system/dns related include files
 */

#include <server.h>
#include <netdb.h>
#include <dpeacl.h>
#include <dce/server_ncl_defs.h>
#include <dce/cds_clerkserver.h>
#include <dce/dnsmanpar.h>
#include <dce/rpc_cvt.h>
#include <dce/uuid.h>
#include <stdio.h> 
#include <dce/dce_cf.h>
#include <dce/utc.h>
#include <dce/cprpc_server.h>
#include <dce/cprpcp.h>

#if defined(DCE_SEC)
#include <dce/rgybase.h>
#include <dce/aclbase.h>
#include <security.h>
#endif


/*
 *****************************************************************************
 * External Global Declarations
 */
extern nsgbl_ns_t dns_nameserver;
#if defined(DCE_SEC)
extern boolean server_acl_ready;
#endif

/*
 *****************************************************************************
 * Miscellaneous Macros and Definitions
 */


/*
 *****************************************************************************
 * Forward declarations and prototypes
 */

INTERNAL dns_status_t
srv_show_server (
    nsgbl_ns_t         *,
    cprpc_attr_list_t  *,
    clh_info_list_p_t  *,
    handle_t            );

INTERNAL dns_status_t
srv_disable_server (
    handle_t        ,
    nsgbl_ns_t	    *);

INTERNAL dns_status_t
srv_create_clh (
    handle_t          ,
    nsgbl_ns_t        *,
    clh_info_list_p_t *);

INTERNAL dns_status_t
srv_show_clh (
    handle_t          ,
    nsgbl_ns_t        *,
    clh_info_list_p_t *);

INTERNAL dns_status_t
srv_clear_clh (
    handle_t          ,
    nsgbl_ns_t        *,
    clh_info_list_p_t *);

INTERNAL dns_status_t
srv_delete_clh (
    handle_t          ,
    nsgbl_ns_t        *,
    clh_info_list_p_t *);

INTERNAL dns_status_t 
srv_force_clh (handle_t            ,
	       nsgbl_ns_t         *,
	       clh_info_list_p_t  *,
	       signed32            );

INTERNAL dns_status_t 
force_clh (user_descriptor_t *,
	   uuid_t            *,
	   signed32           );

INTERNAL dns_status_t
show_clh (
    struct nsgbl_ch  *,
    clh_info_t      **);

INTERNAL dns_status_t
user_setup (
    user_descriptor_t *,
    nsgbl_ch_t	      *,
    SimpleName_t      *,
    handle_t           );

INTERNAL dns_status_t
known_clearinghouses (
    nsgbl_ns_t        *,
    cprpc_attr_list_t *,
    clh_info_list_p_t *);

INTERNAL dns_status_t
my_cdsImport_FullName (
    cds_FullName_t *,
    FullName_t     *);

INTERNAL void
func_clh_read_access_count (
    cprpc_attr_t    *,
    nsgbl_ch_t      *);

INTERNAL void
func_clh_write_access_count (
    cprpc_attr_t    *,
    nsgbl_ch_t      *);

INTERNAL void
func_clh_references_returned_count (
    cprpc_attr_t    *,
    nsgbl_ch_t      *);

INTERNAL void
func_clh_root_not_reachable_count (
    cprpc_attr_t    *,
    nsgbl_ch_t      *);

INTERNAL void
func_clh_data_corruption_count (
    cprpc_attr_t    *,
    nsgbl_ch_t      *);

INTERNAL void
func_clh_skulk_failures_count (
    cprpc_attr_t   *,
    nsgbl_ch_t     *);

INTERNAL void
func_clh_entry_missing_count (
    cprpc_attr_t *,
    nsgbl_ch_t   *);

INTERNAL void
func_clh_upgrades_failed_count (
    cprpc_attr_t *,
    nsgbl_ch_t   *);

INTERNAL void
func_clh_enable_count (
    cprpc_attr_t *,
    nsgbl_ch_t   *);

INTERNAL void
func_clh_disable_count (
    cprpc_attr_t *,
    nsgbl_ch_t   *);

PUBLIC error_status_t 
dnscp_server_Noop (
    handle_t     );





/*
 *
 * S E R V E R _ C L H _ C O U N T E R S [ ]
 *
 * This table identifies all the SERVER CLEARINGHOUSE counters
 * and the name of a function to access the counter.
 */

struct SERVER_CLH_COUNTERS
{
    int ctr_id;			       /* the counter ID (from
				        * 'dns_ncl_definitions.h') */
    void (*ctr_func) (cprpc_attr_t  *,
                      nsgbl_ch_t    *);/* the counter access routine */
};

static struct SERVER_CLH_COUNTERS ServerClhCounters[] =
{
    /* counters */
    {CH_READ_ACCESS_COUNT,         func_clh_read_access_count},
    {CH_WRITE_ACCESS_COUNT,        func_clh_write_access_count},
    {CH_REFERENCES_RETURNED_COUNT, func_clh_references_returned_count},
    {CH_ROOT_NOT_REACHABLE_COUNT,  func_clh_root_not_reachable_count},
    {CH_DATA_CORRUPTION_COUNT,     func_clh_data_corruption_count},
    {CH_SKULK_FAILURES_COUNT,      func_clh_skulk_failures_count},
    {CH_ENTRY_MISSING_COUNT,       func_clh_entry_missing_count},
    {CH_UPGRADES_FAILED_COUNT,     func_clh_upgrades_failed_count},
    {CH_ENABLE_COUNT,              func_clh_enable_count},
    {CH_DISABLE_COUNT,             func_clh_disable_count}
};
#define NUM_SERVER_CLH_COUNTERS \
    (sizeof(ServerClhCounters)/sizeof(struct SERVER_CLH_COUNTERS))

/*
 *
 * N S G B L _ P
 *
 * This is a pointer to the nameserver data structure.  It is
 * initialized at startup time by a call to cdscp_server_init().
 */
INTERNAL nsgbl_ns_t *nsgbl_p;
/*
 *
 *  DTSS UTC base time is October 15, 1582 (Gregorian).
 *  CDS base time is November 17, 1858 (Smithsonian).
 */
/* Difference between smithsonian zero and gregorian zero */
static time_quad_t base = INIT_time_quad(0x0135886Au, 0xC7960000u);
static time_quad_t nulltime = INIT_time_quad(0, 0);

PUBLIC error_status_t 
dnscp_server_Noop (handle_t h)
{
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >dnscp_server_Noop"));

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <dnscp_server_Noop: return (rpc_s_ok)"));

  return (rpc_s_ok);
}

/*
 *
 *
 *  ROUTINE NAME:       dnscp_server
 *
 *  SCOPE:              PUBLIC - declared in cprpc_server.idl
 *
 *  DESCRIPTION:
 *
 *  This routine is responsible for dispatching CDS server commands from
 *  the CDS Control Program.  These commands are received via the RPC
 *  mechanism.
 *
 *  INPUTS:
 *      h               handle_t for this routine
 *      func            function code
 *      caller_id       caller's user id
 *      attr_list_p     pointer to server attribute list
 *      clh_info_pp     pointer to pointer to clearinghouse list
 *  OUTPUT:
 *      command_status  pointer to return status
 *
 *  IMPLICIT INPUTS:
 *      nsgbl_p         pointer to nameserver data
 *
 *  IMPLICIT OUTPUTS:   none
 *
 *  FUNCTION VALUE:
 *      return          success/failure status code
 *                      0 = success
 *                       other = failure
 *
 */

PUBLIC error_status_t 
dnscp_server (handle_t            h,
              signed32            func,
              signed32            caller_id,
              cprpc_attr_list_t   *attr_list_p,
              clh_info_list_p_t   *clh_info_pp,
              command_status_p_t  command_status)
{
    command_status_t cmd_status;
    int              oldcancel;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >dnscp_server(%p,%ld,%ld,%p,%p)",
        h,
        func,
        caller_id,
        attr_list_p,
        clh_info_pp));

    oldcancel = dthread_setcancel(CANCEL_OFF);
    /*
     * vector to the appropriate function handler
     */
    switch (func)
    {
        /*
         * Server commands
         */
        case SHOWSRV:
        {
            cmd_status = srv_show_server (nsgbl_p, attr_list_p,
                                          clh_info_pp, h);
            break;
        }

        case DISSRV:
        {
            cmd_status = srv_disable_server (h, nsgbl_p);
            break;
        }

        /*
         * Clearinghouse commands
         */
        case CRECLECMD:
        {
            cmd_status = srv_create_clh (h, nsgbl_p, clh_info_pp);
            break;
        }

        case SHOWSRVCLE:
        {
            cmd_status = srv_show_clh (h, nsgbl_p, clh_info_pp);
            break;
        }

        case CLEARCLE:
        {
            cmd_status = srv_clear_clh (h, nsgbl_p, clh_info_pp);
            break;
        }

        case DELCLECMD:
        {
            cmd_status = srv_delete_clh (h, nsgbl_p, clh_info_pp);
            break;
        }

        case FORCEBACKCMD:
        case FORCECHECKCMD:
        case REPAIRTIMECMD:
        case REPAIRSTATUSCMD:
        {
            cmd_status = srv_force_clh (h, nsgbl_p, clh_info_pp, func);
            break;
        }

        default:
        {
            cmd_status = DNS_ERROR;
            break;
        }
    }

    /*
     * return with status
     */
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <dnscp_server(%p,%d,%d,0x%0lx,0x%0lx) cmd_status=%d return(rpc_s_ok)",
        h,
        func,
        caller_id,
        attr_list_p,
        clh_info_pp,
        cmd_status));

    *command_status = cmd_status;

    (void)dthread_setcancel(oldcancel);

    return (rpc_s_ok);
}


/*
 *  cdscp_server_init
 *    This routine is responsible for initializing the management
 *     interface of the CDS server.
 *
 *  INPUTS:
 *      nsgbl_init_p    pointer to nameserver data structure
 *
 *  IMPLICIT OUTPUTS:
 */
void 
cdscp_server_init (nsgbl_ns_t *nsgbl_init_p)
{
    nsgbl_p = nsgbl_init_p;
}

/*
 *
 *  srv_show_server
 *      Display server attribute(s).
 *
 *  INPUTS:
 *      ns_p            pointer to server data
 *      attr_list_p     pointer to attribute list
 *      clh_info_pp     pointer to pointer to clearinghouse list
 *
 *  FUNCTION VALUE:     return status
 */

INTERNAL dns_status_t 
srv_show_server (nsgbl_ns_t         *ns_p,
                 cprpc_attr_list_t  *attr_list_p,
                 clh_info_list_p_t  *clh_info_pp,
                 handle_t           h)
{
    dns_status_t cmd_status;
    cprpc_attr_t *attrp;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >srv_show_server(0x%lx,0x%0lx,attr_id=%d,0x%0lx)",
        ns_p,
        attr_list_p,
        attr_list_p->attr_members[0]->attr_id,
        clh_info_pp));

#if defined(DCE_SEC)
  /* check client for read access to server */
    if (!(TestServerUserAccess(h, sec_acl_perm_read)))
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug3,
          " <srv_show_server: return(DNS_ACCESSDENIED)"));

	return(DNS_ACCESSDENIED);
    }
    
#endif

    /*
     * Initialization
     */
    cmd_status = DNS_SUCCESS;
    attrp = attr_list_p->attr_members[0];
    ZERO_bytes (&attrp->value.av_val, sizeof (attrp->value.av_val));
    attrp->value.av_valuetype = CP_none;

    /*
     * lock the memory section
     */
    SERVER_LOCK(ns_p->ns_mutex)
     /* Beginning of the 'protected code' */
    {
        switch (attrp->attr_id)
        {
            case DNS_CREATE_TIME:
            {
                attrp->value.av_valuetype = CP_utctime;
		memcpy((char *)attrp->value.av_val.av_utctime,
		       (char *)&ns_p->ns_create_time, sizeof(utc_t));
                break;
            }
            case DNS_FUTURE_SKEW:
            {
                attrp->value.av_valuetype = CP_time_quad;
		COPY_time_quad_to_hyper(&ns_p->ns_future_skew,
					&attrp->value.av_val.av_time_quad);
                break;
            }
            case DNS_READ_ACCESS_COUNT:
            {
		EXPORT_MGMT_COUNTER(&ns_p->ns_ReadAccesses, &attrp->value);
                break;
            }
            case DNS_WRITE_ACCESS_COUNT:
            {
		EXPORT_MGMT_COUNTER(&ns_p->ns_WriteAccesses, &attrp->value);
                break;
            }
            case DNS_SKULKS_INITIATED_COUNT:
            {
		EXPORT_MGMT_COUNTER(&ns_p->ns_SkulksInitiated, &attrp->value);
                break;
            }
            case DNS_SKULKS_COMPLETED_COUNT:
            {
		EXPORT_MGMT_COUNTER(&ns_p->ns_SkulksCompleted, &attrp->value);
                break;
            }
            case DNS_TREE_BROKEN_COUNT:
            {
		EXPORT_MGMT_COUNTER(&ns_p->ns_TimesLookupPathBroken,
				    &attrp->value);
                break;
            }
            case DNS_CRUCIAL_REPLICA_COUNT:
            {
		EXPORT_MGMT_COUNTER(&ns_p->ns_CrucialReplicaRemovalsBackedOut,
				    &attrp->value);
                break;
            }
            case DNS_CHILD_POINTER_COUNT:
            {
		EXPORT_MGMT_COUNTER(&ns_p->ns_ChildPointerUpdateFailures,
				    &attrp->value);
                break;
            }
            case DNS_SECURITY_COUNT:
            {
		EXPORT_MGMT_COUNTER(&ns_p->ns_SecurityFailures, &attrp->value);
                break;
            }
            case DNS_KNOWN_CLEARINGHOUSES:
            {
                cmd_status = known_clearinghouses (ns_p, attr_list_p, clh_info_pp);
                break;
            }

            default:
            {
                cmd_status = DNS_ERROR;
                break;
            }
        }

    }	/* End of the 'protected code' */
    SERVER_END_LOCK(ns_p->ns_mutex)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <srv_show_server(0x%lx,0x%0lx,attr_id=%d,0x%0lx) return(%d)",
        ns_p,
        attr_list_p,
        attr_list_p->attr_members[0]->attr_id,
        clh_info_pp,
        cmd_status));

    return (cmd_status);
}

/*
 *
 *  srv_create_clh
 *       Create the clearinghouse entity; do not overwrite if the
 *       clearinghouse already exists.
 *
 *  INPUTS:
 *
 *      h               binding handle
 *      ns_p            address of nameserver mgmt data 
 *      clh_info_pp     pointer to pointer to clearinghouse information list
 *
 *  FUNCTION VALUE:     return status
 */

INTERNAL dns_status_t 
srv_create_clh (handle_t           h,
                nsgbl_ns_t         *ns_p,
                clh_info_list_p_t  *clh_info_pp)
{
    FullName_t          cle_name;
    FullName_t          initial_replica;
    cds_Version_t *     newdir_version_p;
    int                 replica_type;
    char *              file_spec_p;
    clh_info_t *        clhp;
    int thstatus;
    LOG_ASCII_BUF_FullName(fullnameBuf)


    char work_buf[MAX_FILE_SIZE];
    struct nsgbl_ch *ch_p;
    struct buf
    {
	user_descriptor_t b_user;
	FullName_u b_client;
	FullName_u b_node;
	FullName_u b_initial_replica;
	version_t b_newdir_version;
    }  *buf_p;
    dns_status_t status, matched_ch_file;

    DEB_ASCII_BUF_FullName (cle_name_buf)
    DEB_ASCII_BUF_FullName (initial_replica_buf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >srv_create_clh(0x%lx)",
        ns_p));

    /*
     * check for priv's
     */
#if defined(DCE_SEC)
  /* check client for write access to server */
    if (!(TestServerUserAccess(h, sec_acl_perm_write)))
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug3,
          " <srv_create_clh: return(DNS_ACCESSDENIED)"));

	return(DNS_ACCESSDENIED);
    }
    
#endif

    /*
     * initialization
     */
    clhp             = (*clh_info_pp)->clh_members[0];
    replica_type     = clhp->replica_type;
    newdir_version_p = clhp->dir_version_p;
    file_spec_p      = NULL;

    /*
     * do the remote to local conversion(s)
     */
    status = my_cdsImport_FullName (clhp->clh_name_p,
                                    &cle_name);
    if (status != DNS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug3,
          " <srv_create_clh: (1) return(status=%d)",status));

	return (status);
    }
    

    status = my_cdsImport_FullName (clhp->replica_name_p,
                                    &initial_replica);
    if (status != DNS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug3,
          " <srv_create_clh: (2) return(status=%d)",status));

	return (status);
    }
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        "  srv_create_clh(%s,%s,0x%0lx,%d,%s)",
        deb_ascii_FullName(cle_name_buf,&cle_name),
        deb_ascii_FullName(initial_replica_buf,&initial_replica),
        newdir_version_p,
        replica_type,
        file_spec_p));

    /*
     * get a buffer to work with
     */
    buf_p = (struct buf *) dns_malloc (sizeof (struct buf));
    if (!buf_p)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug3,
          " <srv_create_clh: return(DNS_NONSRESOURCES)"));

	return (DNS_NONSRESOURCES);
    }
    
    ZERO_bytes (buf_p, sizeof(struct buf));

    /*
     * lock the server mgmt data memory section
     */

    SERVER_LOCK(ns_p->ns_mutex)
    {     /* Beginning of the 'protected code' */

      /*
       * See if clearinghouse by this name exists
       */
      ch_p = mgmt_find_chname (ns_p, &cle_name);
      if (ch_p) {
	dns_free ((char *) buf_p);
	    
	SERVER_END_LOCK(ns_p->ns_mutex)

	DCE_SVC_DEBUG((
		       cds__svc_handle,
		       cds_svc_server,
		       svc_c_debug3,
		       " <srv_create_clh: return(DNS_ENTRYEXISTS)"));

	return (DNS_ENTRYEXISTS);
      }

      /*
       * If we weren't given a filename, create one.
       */
      if (!file_spec_p)
	  {
	    file_spec_p = work_buf;
	    (void)sys_bld_filename (&cle_name, file_spec_p);
	  }

      /*
       * If clearinghouse exists in memory, we have an error.  If
       * clearinghouse exists on disk, use that data.  Otherwise, create the
       * clearinghouse from scratch.
       */
      matched_ch_file = mgmt_find_chdata (ns_p, file_spec_p);
      if (matched_ch_file == DNS_ERROR) {
	dns_free ((char *) buf_p);

	SERVER_END_LOCK(ns_p->ns_mutex)

	DCE_SVC_DEBUG((
               cds__svc_handle,
               cds_svc_server,
               svc_c_debug3,
               " <srv_create_clh: (1) return(DNS_NAMESERVERBUG)"));

	return (DNS_NAMESERVERBUG);
      }

      /*
       * if we didn't find CH data,  begin creating a new CH
       */
      if (matched_ch_file == DNS_NOTFND) {
	if (!newdir_version_p)
	    {
	      get_directory_version (
			     (VersionNumber_t *) &buf_p->b_newdir_version);
	    }
	else
	    {
	      INS8(buf_p->b_newdir_version.vr_major, 
		   newdir_version_p->ver_major);
	      INS8(buf_p->b_newdir_version.vr_minor, 
		   newdir_version_p->ver_minor);
	    }

	/*
	 * default replica type is READONLY
	 */
	if (replica_type < 0)
            {
	      replica_type = RT_readOnly;
            }
      } /* End creating new ch  */

      /*
       * Allocate a CH entry in the NS mgmt data structure.  If
       * the allocate fails, set status and don't fill in the CH
       * data.
       */
      ch_p = nsgbl_ch_malloc (ns_p);
      if (!ch_p)
	  {
	    status = DNS_NONSRESOURCES;
	  }
      else {
	/*
	 * fill in the user context
	 */
	ZERO_bytes (&buf_p->b_user, sizeof (buf_p->b_user));
	status = user_setup (&buf_p->b_user, ch_p, NULL, h);
	if (status == error_status_ok) {
	  /*
	   * If have no previous clearinghouse data, create everything 
	   * from scratch.  Otherwise, setup and enable previous 
	   * clearinghouse data.
	   */
	  (void)strcpy (ch_p->ch_filename, file_spec_p);
	  if (matched_ch_file == DNS_NOTFND) {
	    status = mgmt_create_cle (ns_p,
				      &buf_p->b_user,
				      ch_p,
				      &cle_name,
				      &initial_replica,
				      replica_type,
				      &buf_p->b_newdir_version);
	  }
	  else {
	    /*
	     * Begin setup and enable old CH here
	     */
	    if ((status = db_setup (ch_p)) == DNS_SUCCESS) {
	      /*
	       * Begin successfully setup CH.  Log an event if
	       * successful and inform advertiser.
	       */
	      if ((status = db_enable (ch_p, TRUE)) == DNS_SUCCESS) {
		int thstatus;

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

		adver_signal_new_info (ns_p->ns_advertiser);
	      }
	    }		       /* End successfully setup CH */
	  }		       /* End setup and enable old CH here */
	} /* end got pac */

	if (status == DNS_SUCCESS) {
	  /*
	   * Add file name of new clearinghouse to the system
	   * management file.
	   */
	  status = sys_mgmt_file_add (ns_p, ch_p->ch_filename);

	  /*
	   * Create default file so that V1.x servers can join
	   * with this server and send info about new
	   * clearinghouse to advertiser.
	   */
	  if (status == DNS_SUCCESS)
	      {
		adver_signal_new_info (ns_p->ns_advertiser);
	      }
	}
	else
	    {
	      /*
	       * The mgmt_create_cle call has failed.  Remove the
	       * clearinghouse entry from the nameserver's CH list.
	       */
	      nsgbl_ch_free (ch_p);
	    }
      }
      
    } /* End of the 'protected code' */

    SERVER_END_LOCK(ns_p->ns_mutex)
    dns_free ((char *) buf_p);
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <srv_create_clh(0x%lx) return(%d)",
        ns_p,
        status));

    return (status);
}

/*
 *   srv_show_clh
 *      Show the clearinghouse entities known to a nameserver.
 *
 *  INPUTS:
 *
 *      h               binding handle
 *      ns_p            address of nameserver mgmt data
 *      clh_info_pp     pointer to pointer to clearinghouse information list
 *
 */
INTERNAL dns_status_t 
srv_show_clh (handle_t           h,
              nsgbl_ns_t         *ns_p,
              clh_info_list_p_t  *clh_info_pp)
{
    command_status_t    status;
    FullName_t          cle_name;
    clh_info_t         *clhp;
    clh_info_list_t    *clh_info_list_p;
    struct nsgbl_ch    *ch_p;
    struct buf
    {
        user_descriptor_t b_user;
        FullName_u	b_client;
        FullName_u	b_node;
    } *buf_p;
    int thstatus;
    DEB_ASCII_BUF_FullName (cle_name_buf)


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >srv_show_clh(0x%lx)",
        ns_p));

    /*
     * Initialization
     */
    status = DNS_SUCCESS;
    clh_info_list_p = *clh_info_pp;
    clhp = clh_info_list_p->clh_members[0];

    /*
     * check for priv's
     */
#if defined(DCE_SEC)
    /* check client for write access to server */
    if (!(TestServerUserAccess (h, sec_acl_perm_read)))
    {
	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_server,
           svc_c_debug3,
           " <srv_show_clh: return (DNS_ACCESSDENIED)"));

	return (DNS_ACCESSDENIED);
    }
    
#endif

    /*
     * do the remote to local conversion(s)
     */
    status = my_cdsImport_FullName (clhp->clh_name_p, &cle_name);
    if (status != DNS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_server,
           svc_c_debug3,
           " <srv_show_clh: return (status=%d)",status));

	return (status);
    }
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        "  srv_show_clh(%s)",
        deb_ascii_FullName(cle_name_buf,&cle_name)));

    /*
     * get a user buffer to work with
     */
    buf_p = (struct buf *) dns_malloc (sizeof(struct buf));
    if (!buf_p) 
    {
	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_server,
           svc_c_debug3,
           " <srv_show_clh: return (DNS_NONSRESOURCES)"));

	return (DNS_NONSRESOURCES);
    }
    
    ZERO_bytes (buf_p, sizeof(struct buf));

    /*
     * lock the server mgmt data memory section
     */

    SERVER_LOCK(ns_p->ns_mutex)
    {     /* Beginning of the 'protected code' */

      /*
       * Find the clearinghouse
       */
      ch_p = mgmt_find_chname (ns_p, &cle_name);
      if (!ch_p)
	  {
	    dns_free ((char *) buf_p);
	    
	    SERVER_END_LOCK(ns_p->ns_mutex)

	    DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_server,
              svc_c_debug3,
              " <srv_show_clh: return (DNS_UNKNOWNCLEARINGHOUSE)"));

            return (DNS_UNKNOWNCLEARINGHOUSE);
        }

        /*
         * fill in the user context
         */
        ZERO_bytes(&buf_p->b_user, sizeof(buf_p->b_user));
        status = user_setup (&buf_p->b_user, ch_p, NULL, h);

        /*
         * If successful then 'show' the clearinghouse information
         */
        if (status == error_status_ok)
        {
            status =
                show_clh (ch_p, &clh_info_list_p->clh_members[0]);
        }
        else
        {
            clh_info_list_p->clh_count = 0;
            clh_info_list_p->clh_members[0] = NULL;
        }

    } /* End of the 'protected code' */
    SERVER_END_LOCK(ns_p->ns_mutex)

    /*
     * free up our user buffer
     */
    dns_free ((char *)buf_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <srv_show_clh(0x%lx) return(%d)",
        ns_p,
        status));

    return (status);
}

/*
 *
 *
 *   show_clh
 *      Gets nameserver clearinghouse information.
 *
 *  INPUTS:
 *
 *      ch_p            pointer to nameserver clearinghouse structure 
 *
 *  OUTPUTS:
 * 
 *      clh_info_pp     ptr to ptr clearinghouse info structure (malloc'd)
 *
 */
INTERNAL dns_status_t 
show_clh (struct nsgbl_ch  *ch_p,
          clh_info_t       **clh_info_pp)
{
    command_status_t status;
    int              num_bytes;
    int              index;
    clh_info_t      *clh_info_p;
    cprpc_attr_t    *attr_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >show_clh(0x%0lx)",
        ch_p));

    /*
     * Initialize
     */
    status = DNS_NONSRESOURCES; /* assume the worst */

    /*
     * Allocate a clearinghouse info structure
     */
    num_bytes = sizeof (clh_info_t);
    *clh_info_pp = clh_info_p = (clh_info_t *) rpc_ss_allocate (num_bytes);
    if (clh_info_p == NULL)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug2,
          "  show_clh: (1) rpc_ss_allocate failed"));
      
        goto checkout;
    }
    
    ZERO_bytes (clh_info_p, num_bytes);

    /*
     * Allocate/init an attribute list
     */
    num_bytes = sizeof(cprpc_attr_list_t) +
                (NUM_SERVER_CLH_COUNTERS * sizeof(cprpc_attr_p_t));

    clh_info_p->attr_list_p = (cprpc_attr_list_t *) rpc_ss_allocate (num_bytes);
    if (clh_info_p->attr_list_p == NULL)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug2,
          "  show_clh: (2) rpc_ss_allocate failed"));

        goto checkout;
    }
    
    ZERO_bytes (clh_info_p->attr_list_p, num_bytes);
    clh_info_p->attr_list_p->attr_len = NUM_SERVER_CLH_COUNTERS;

    /*
     * For each of the counters, allocate an attribute structure,
     * fill in its attribute id and value, and add pointer to attribute list.
     */
    for (index=0; index < NUM_SERVER_CLH_COUNTERS; index++)
    {
        num_bytes = sizeof (cprpc_attr_t);
	attr_p = (cprpc_attr_t *) rpc_ss_allocate (num_bytes);
        if (attr_p == NULL)
	{
	    DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_server,
              svc_c_debug2,
              "  show_clh: (3) rpc_ss_allocate failed"));

            goto checkout;
	}
	
        ZERO_bytes (attr_p, num_bytes);

	attr_p->attr_id = ServerClhCounters[index].ctr_id;
	(ServerClhCounters[index].ctr_func) (attr_p, ch_p);	/* call func for value */
	clh_info_p->attr_list_p->attr_members[index] = attr_p;
    }

    /*
     * if we made it this far, we return success
     */
    status = DNS_SUCCESS;

    /*
     * if our return status is anything other than success, we free
     * up all of our structures and return in failure
     */
checkout:
    if (status != DNS_SUCCESS)
    {
        if (clh_info_p != NULL)
        {
            if (clh_info_p->attr_list_p != NULL)
            {
                for (index=0; index < NUM_SERVER_CLH_COUNTERS; index++)
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
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <show_clh() return(%d)",
        status));

    return (status);
}


/*
 *  srv_clear_clh
 *      Clear the clearinghouse entity.
 *
 * INPUTS:
 *      h               binding handle
 *      ns_p            address of nameserver mgmt data
 *      clh_info_pp     pointer to pointer to clearinghouse information list
 *
 */

INTERNAL dns_status_t 
srv_clear_clh (handle_t           h,
               nsgbl_ns_t         *ns_p,
               clh_info_list_p_t  *clh_info_pp)
{
    FullName_t          cle_name;
    struct nsgbl_ch    *ch_p;
    clh_info_t         *clhp;
    struct buf
    {
	user_descriptor_t b_user;
	FullName_u	b_client;
	FullName_u	b_node;
    } *buf_p;
    dns_status_t        status;
    int thstatus;
    DEB_ASCII_BUF_FullName (cle_name_buf)
    LOG_ASCII_BUF_FullName(fullnameBuf)


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >srv_clear_clh(0x%lx)",
        ns_p));

    /*
     * check for priv's
     */
#if defined(DCE_SEC)
    /* check client for write access to server */
    if (!(TestServerUserAccess(h, sec_acl_perm_write)))
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug3,
          " <srv_clear_clh: return (DNS_ACCESSDENIED)"));

	return (DNS_ACCESSDENIED);
    }
    
#endif

    /*
     * initialization
     */
    clhp = (*clh_info_pp)->clh_members[0];

    /*
     * do the remote to local conversion(s)
     */
    status = my_cdsImport_FullName (clhp->clh_name_p, &cle_name);
    if (status != DNS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug3,
          " <srv_clear_clh: return (status=%d)",status));

	return (status);
    }
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        "  srv_clear_clh(%s)",
        deb_ascii_FullName(cle_name_buf,&cle_name)));

    /*
     * get a buffer to work with
     */
    buf_p = (struct buf *) dns_malloc (sizeof(struct buf));
    if (!buf_p) 
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug3,
          " <srv_clear_clh: return (DNS_NONSRESOURCES)"));

	return(DNS_NONSRESOURCES);
    }
    
    ZERO_bytes (buf_p, sizeof(struct buf));

    /*
     * lock the server mgmt data memory section
     */

    SERVER_LOCK(ns_p->ns_mutex)
    {/* Beginning of the 'protected code' */ 

        /*
         * Find clearinghouse
         */
        ch_p = mgmt_find_chname (ns_p, &cle_name);
        if (!ch_p)
        {
	    dns_free ((char *) buf_p);

	    SERVER_END_LOCK(ns_p->ns_mutex)

	    DCE_SVC_DEBUG((
             cds__svc_handle,
             cds_svc_server,
             svc_c_debug3,
             " <srv_clear_clh: return (DNS_UNKNOWNCLEARINGHOUSE)"));

            return (DNS_UNKNOWNCLEARINGHOUSE);
        }

	/*
         * fill in the user context
         */
	ZERO_bytes (&buf_p->b_user, sizeof(buf_p->b_user));
	status = user_setup (&buf_p->b_user, ch_p, NULL, h);
	if (status == error_status_ok)
        {
            /*
             * Now, remove this clearinghouse from known list and clear
             * clearinghouse object.
             */
            status = mgmt_clear_ch (&buf_p->b_user, ch_p);
            if (status == DNS_SUCCESS)
            {
                adver_signal_new_info (ns_p->ns_advertiser);
                nsgbl_ch_free (ch_p);
                status = sys_mgmt_create_new_file (ns_p);
            }
        }
    } /* End of the 'protected code' */

    SERVER_END_LOCK(ns_p->ns_mutex)

    dns_free ((char *)buf_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <srv_clear_clh(0x%lx) return(%d)",
        ns_p,
        status));

    return (status);
}

/*
 *  srv_delete_clh
 *      Delete the clearinghouse entity.
 *
 *  INPUTS:
 *      h               binding handle
 *      ns_p            address of nameserver mgmt data
 *      clh_info_pp     pointer to pointer to clearinghouse information list
 */

INTERNAL dns_status_t 
srv_delete_clh (handle_t           h,
                nsgbl_ns_t         *ns_p,
                clh_info_list_p_t  *clh_info_pp)
{
    FullName_t          cle_name;
    struct nsgbl_ch    *ch_p;
    clh_info_t         *clhp;
    struct buf
    {
	user_descriptor_t b_user;
	FullName_u	b_client;
	FullName_u	b_node;
    } *buf_p;
    dns_status_t        status;
    int thstatus;
    DEB_ASCII_BUF_FullName (cle_name_buf)
    LOG_ASCII_BUF_FullName(fullnameBuf)


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >srv_delete_clh(0x%lx)",
        ns_p));

    /*
     * check for priv's
     */
#if defined(DCE_SEC)
    /* check client for write access to server */
    if (!(TestServerUserAccess (h, sec_acl_perm_write)))
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug2,
          " <srv_delete_clh: return (DNS_ACCESSDENIED)"));

	return (DNS_ACCESSDENIED);
    }
    
#endif

    /*
     * initialization
     */
    clhp = (*clh_info_pp)->clh_members[0];

    /*
     * do the remote to local conversion(s)
     */
    status = my_cdsImport_FullName (clhp->clh_name_p, &cle_name);
    if (status != DNS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug3,
          " <srv_delete_clh: return (status=%d)",status));

	return (status);
    }
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        "  srv_delete_clh(%s)",
        deb_ascii_FullName(cle_name_buf,&cle_name)));

    /*
     * get a buffer to work with
     */
    buf_p = (struct buf *) dns_malloc (sizeof(struct buf));
    if (!buf_p) 
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug3,
          "  srv_delete_clh: return (DNS_NONSRESOURCES)"));

	return (DNS_NONSRESOURCES);
    }
    
    ZERO_bytes (buf_p, sizeof(struct buf));

    /*
     * lock the server mgmt data memory section
     */

    SERVER_LOCK(ns_p->ns_mutex)
     /* Beginning of the 'protected code' */
    {
        /*
         * Find clearinghouse
         */
        ch_p = mgmt_find_chname (ns_p, &cle_name);
        if (!ch_p)
        {
	    dns_free ((char *)buf_p);

	    SERVER_END_LOCK(ns_p->ns_mutex)

	    DCE_SVC_DEBUG((
             cds__svc_handle,
             cds_svc_server,
             svc_c_debug3,
             "  srv_delete_clh: return (DNS_UNKNOWNCLEARINGHOUSE)"));

            return (DNS_UNKNOWNCLEARINGHOUSE);
        }

	/*
         * fill in the user context
         */
	ZERO_bytes(&buf_p->b_user, sizeof(buf_p->b_user));
        status = user_setup (&buf_p->b_user, ch_p, NULL, h);
	if (status == error_status_ok)
            status = mgmt_delete_ch (ns_p, &buf_p->b_user, ch_p);

        /*
         * If successful, remove this clearinghouse from known list,
         * delete cle object, create new text file and log an event
         */
        if (status == DNS_SUCCESS)
        {
            status = clerk_delete_object (&ch_p->ch_name);
            if (status == DNS_SUCCESS)
            {
	      deb_ascii_FullName(fullnameBuf, &cle_name);

	      dce_svc_printf(CDS_S_SERVER_CLEARINGHOUSEDELETED_MSG,
			     fullnameBuf);
	      adver_signal_new_info (ns_p->ns_advertiser);
	      nsgbl_ch_free (ch_p);
	      status = sys_mgmt_create_new_file (ns_p);
            }
        }
    } /* End of the 'protected code' */

    SERVER_END_LOCK(ns_p->ns_mutex)

    dns_free ((char *)buf_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <srv_delete_clh(0x%lx) return(%d)",
        ns_p,
        status));

    return (status);
}

/*
 *
 *
 *  srv_disable_server
 *      Starts up a thread that disables and shuts down the server.
 *
 *  INPUTS:
 *      h               binding handle
 *      ns_p            pointer to server data
 */
INTERNAL dns_status_t 
srv_disable_server (handle_t    h,
                    nsgbl_ns_t  *ns_p)
{

  error_status_t thstatus;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug3,
        " >srv_disable_server(0x%lx)", ns_p));

    /*
     * check for priv's
     */
#if defined(DCE_SEC)
    /* check client for write access to server */
    if (!(TestServerUserAccess (h, sec_acl_perm_write)))
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug3,
          " <srv_disable_server: return (DNS_ACCESSDENIED)"));

        return (DNS_ACCESSDENIED);
    }
    
#endif

  
    /*
     * kick the main loop through a signal
     *
     * I hope we get out of here before the building collapses.
     */
    kill(getpid(), SIGUSR1);

  /*
   * lock the memory section
   */
  SERVER_LOCK(dns_nameserver.ns_mutex) 
      
    while  (ns_p->ns_state == dns_On) {
      if ((thstatus = dthread_wait(&dns_nameserver.ns_state_changed,
				   &dns_nameserver.ns_mutex)) < 0)
	  dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
    }

  SERVER_END_LOCK(dns_nameserver.ns_mutex) 

  DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug3,
          " <srv_disable_server: return (DNS_SUCCESS)"));

    return (DNS_SUCCESS);
}

/*
 * user_setup
 *
 * Sets up our private user structure.
 *
 * Success returns error_status_ok
 */
INTERNAL dns_status_t 
user_setup (user_descriptor_t  *user_p,
            nsgbl_ch_t         *ch_p,
            SimpleName_t       *user_name_p,
            handle_t           h)
{
    dthread_assign(dthread_null, &user_p->ud_mutex);
    user_p->ud_ch_p = ch_p;
    user_p->ud_ns_p = ch_p->ch_nameserver;
    user_p->ud_chuid_p = &ch_p->ch_uid;
    user_p->ud_protocol_version = MAJORVER;

    return(get_user_creds(h, user_p));
}

/*
**++
**
**  ROUTINE NAME:       known_clearinghouses
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      This routine builds a list of the known clearinghouses.
**
**  INPUTS:
**
**      ns_p            pointer to server data
**      attr_list_p     pointer to attribute list
**      clh_info_pp     pointer to pointer to clearinghouse list
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
*/

INTERNAL dns_status_t 
known_clearinghouses (nsgbl_ns_t         *ns_p,
                      cprpc_attr_list_t  *attr_list_p,
                      clh_info_list_p_t  *clh_info_pp)
{
    int clh_num;
    int list_bytes, info_bytes;
    nsgbl_ch_t *ch_p;
    clh_info_list_t *list_p;
    clh_info_t *info_p;
    int index;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >known_clearinghouses"));

    /*
     * initialization
     */
    clh_num = ns_p->ns_clearinghouse_count;

    /*
     * allocate heap for the clearinghouse information list
     */
    list_bytes = sizeof(clh_info_list_t) +
                 ((clh_num-1)*sizeof(clh_info_p_t));
    list_p = (clh_info_list_p_t) rpc_ss_allocate (list_bytes);
    ZERO_bytes (list_p, list_bytes);

    /*
     * allocate heap for the clearinghouse information
     */
    info_bytes = clh_num*sizeof(clh_info_t);
    info_p = (clh_info_p_t) rpc_ss_allocate (info_bytes);
    ZERO_bytes (info_p, info_bytes);

    /*
     * did we get all the heap we requested?
     */
    if (!list_p || !info_p)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug2,
          " <known_clearinghouses: (1) return (DNS_NONSRESOURCES)"));

        return (DNS_NONSRESOURCES);
    }

    /*
     * run down the list of all the known clearinghouses and return
     * the fullname of each clearinghouse that this server knows
     * about
     */
    list_p->clh_count = 0;
    ch_p = NULL;
    for (index=0, ch_p=nsgbl_ch_next (ns_p, ch_p);
         index < clh_num;
         index++, ch_p=nsgbl_ch_next (ns_p, ch_p))
    {
        info_p[index].clh_name_p =
            (cds_FullName_t *) rpc_ss_allocate (sizeof(cds_FullName_t));
        if (!info_p[index].clh_name_p)
	{
	    DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_server,
              svc_c_debug2,
              " <known_clearinghouses: (2) return (DNS_NONSRESOURCES)"));

            return (DNS_NONSRESOURCES);
	}

        (void)L_to_R_FullName (&ch_p->ch_name, info_p[index].clh_name_p);
        list_p->clh_members[index] = &info_p[index];
        list_p->clh_count++;
    }

    /*
     * set our output value and return triumphant
     */
    *clh_info_pp = list_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <known_clearinghouses: return (DNS_SUCCESS)"));

    return (DNS_SUCCESS);
}

/*
 ********************************************************************
 * Convert a cdsfullname
 */
INTERNAL dns_status_t 
my_cdsImport_FullName (cds_FullName_t  *rnamep,
                       FullName_t      *lnamep)
{
    error_status_t status;
    cds_visa_t visa;
    char *cell_name;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >my_cdsImport_FullName"));

    dce_cf_get_cell_name (&cell_name, &status);
    if (status != dce_cf_st_ok)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug2,
          " <my_cdsImport_FullName: return (DNS_ERROR)"));

        return (DNS_ERROR);
    }
    (void)cds_visa_init (&visa, cell_name);
    (void)cdsImport_FullName (&visa, rnamep, lnamep, 0);
    cds_visa_destroy (&visa);

    /*
     * dce_cf_get_cell_name() uses malloc() so we use free()
     */
    free (cell_name);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <my_cdsImport_FullName: return (DNS_SUCCESS)"));

    return (DNS_SUCCESS);
}

#if defined(DCE_SEC)
extern uuid_t server_mgmt_dacl_uuid;

/* For DCE security, determine if the client identified in the
 * binding handle has the specified permission(s) to the server..
 * NOTE: as of 9/25/91, commands which are not 'priveleged' don't
 * use an authenticated binding handle, but the default ACL
 * gives unauthenticated users 'read' access to the server.
*/
int 
TestServerUserAccess (handle_t           h_p,
                      sec_acl_permset_t  permset)
{
  error_status_t st;
  int auth_status = 0;
  sec_id_pac_t *pac_p;
  int thstatus;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " >TestServerUserAccess (%p,%p)",
      h_p,
      permset));

  if (server_acl_ready) {
     dce_acl_is_client_authorized ( h_p,
				   &server_mgmt_dacl_uuid,
				   &server_mgmt_dacl_uuid,
				   (uuid_t *)0,
				   (uuid_t *)0,
				   permset,
				   &auth_status,
				   &st);
     if (auth_status == FALSE) {
       sec_id_pac_t pac;
       
       st = server_get_client_creds(h_p, NULL, &pac, NULL);
       if (st == error_status_ok)
	 auth_status = security_is_locksmith(&pac, &dns_nameserver);
     }
  } 
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_server,
      svc_c_debug2,
      " <TestServerUserAccess (%p,%p) return(%d)",
      h_p,
      permset,
      auth_status));

  return (auth_status);
}
#endif

/*
**++
**
**  ROUTINE NAME:       func_clh_mumble_count
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      The next set of routines are all similar in structure, each
**      routine returning a unique counter value from a nameserver
**      global clearinghouse structure.
**
**  INPUTS:
**
**      attr_p          pointer to an rpc attribute
**      nsgbl_p         pointer to a clearinghouse structure
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
*/

INTERNAL void 
func_clh_read_access_count (cprpc_attr_t  *attr_p,
                            nsgbl_ch_t    *nsgbl_ch_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >func_clh_read_access_count"));

    EXPORT_MGMT_COUNTER(&nsgbl_ch_p->ch_ReadAccesses, &attr_p->value);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <func_clh_read_access_count"));
}

INTERNAL void 
func_clh_write_access_count (cprpc_attr_t  *attr_p,
                             nsgbl_ch_t    *nsgbl_ch_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >func_clh_write_access_count"));

    EXPORT_MGMT_COUNTER(&nsgbl_ch_p->ch_WriteAccesses, &attr_p->value);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <func_clh_write_access_count"));
}

INTERNAL void 
func_clh_references_returned_count (cprpc_attr_t  *attr_p,
                                    nsgbl_ch_t    *nsgbl_ch_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >func_clh_references_returned_count"));

    EXPORT_MGMT_COUNTER(&nsgbl_ch_p->ch_ReferencesReturned, &attr_p->value);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <func_clh_references_returned_count"));
}

INTERNAL void 
func_clh_root_not_reachable_count (cprpc_attr_t  *attr_p,
                                   nsgbl_ch_t    *nsgbl_ch_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >func_clh_root_not_reachable_count"));

    EXPORT_MGMT_COUNTER(&nsgbl_ch_p->ch_TimesRootNotReachable, &attr_p->value);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <func_clh_root_not_reachable_count"));
}

INTERNAL void 
func_clh_data_corruption_count (cprpc_attr_t  *attr_p,
                                nsgbl_ch_t    *nsgbl_ch_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >func_clh_data_corruption_count"));

    EXPORT_MGMT_COUNTER(&nsgbl_ch_p->ch_DataCorruptions, &attr_p->value);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <func_clh_data_corruption_count"));
}

INTERNAL void 
func_clh_skulk_failures_count (cprpc_attr_t  *attr_p,
                               nsgbl_ch_t    *nsgbl_ch_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >func_clh_skulk_failures_count"));

    EXPORT_MGMT_COUNTER(&nsgbl_ch_p->ch_SkulkFailures, &attr_p->value);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <func_clh_skulk_failures_count"));
}

INTERNAL void 
func_clh_entry_missing_count (cprpc_attr_t  *attr_p,
                              nsgbl_ch_t    *nsgbl_ch_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >func_clh_entry_missing_count"));

    EXPORT_MGMT_COUNTER(&nsgbl_ch_p->ch_TimesClearinghouseEntryMissing,
			&attr_p->value);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <func_clh_entry_missing_count"));
}

INTERNAL void 
func_clh_upgrades_failed_count (cprpc_attr_t  *attr_p,
                                nsgbl_ch_t    *nsgbl_ch_p)
{
   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >func_clh_upgrades_failed_count"));

    EXPORT_MGMT_COUNTER(&nsgbl_ch_p->ch_UpgradesNotPossible, &attr_p->value);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <func_clh_upgrades_failed_count"));
}

INTERNAL void 
func_clh_enable_count (cprpc_attr_t  *attr_p,
                       nsgbl_ch_t    *nsgbl_ch_p)
{
   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >func_clh_enable_count"));

    EXPORT_MGMT_COUNTER(&nsgbl_ch_p->ch_EnableCounts, &attr_p->value);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <func_clh_enable_count"));
}

INTERNAL void 
func_clh_disable_count (cprpc_attr_t  *attr_p,
                        nsgbl_ch_t    *nsgbl_ch_p)
{ 
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >func_clh_disable_count"));

    EXPORT_MGMT_COUNTER(&nsgbl_ch_p->ch_DisableCounts, &attr_p->value);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <func_clh_disable_count"));
}

/*
 *
 *
 *  ROUTINE NAME:       srv_force_clh
 *
 *  SCOPE:              INTERNAL
 *
 *   DESCRIPTION:
 *
 *      Force some thread to run
 *
 *  INPUTS:
 *
 *      h               binding handle
 *      ns_p            address of nameserver mgmt data
 *      clh_info_pp     pointer to pointer to clearinghouse information list
 *      func            specifies which thread you wish to start
 *
 *  INPUTS/OUTPUTS:     none
 *
 *  OUTPUTS:            none
 *
 *  IMPLICIT INPUTS:    none
 *
 *  IMPLICIT OUTPUTS:   none
 *
 *  FUNCTION VALUE:     return status
 *
 *  SIDE EFFECTS:       none
 *
 *
 */

INTERNAL dns_status_t 
srv_force_clh (handle_t            h,
	       nsgbl_ns_t         *ns_p,
	       clh_info_list_p_t  *clh_info_pp,
	       signed32            func)
{
    FullName_t          cle_name;
    struct nsgbl_ch    *ch_p;
    clh_info_t         *clhp;
    struct buf
    {
	user_descriptor_t b_user;
	FullName_u	b_client;
	FullName_u	b_node;
    } *buf_p;
    dns_status_t        status;
    int thstatus;
    DEB_ASCII_BUF_FullName (cle_name_buf)
    LOG_ASCII_BUF_FullName(fullnameBuf)


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >srv_force_clh(0x%lx)",
        ns_p));

    /*
     * check for priv's
     */
#if defined(DCE_SEC)
    /* check client for write access to server */
    if (!(TestServerUserAccess (h, sec_acl_perm_write)))
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug2,
          " <srv_force_clh: return (DNS_ACCESSDENIED)"));

	return (DNS_ACCESSDENIED);
    }
    
#endif

    /*
     * initialization
     */
    clhp = (*clh_info_pp)->clh_members[0];

    /*
     * do the remote to local conversion(s)
     */
    status = my_cdsImport_FullName (clhp->clh_name_p, &cle_name);
    if (status != DNS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug2,
          " <srv_force_clh: return (status=%d)",status));

	return (status);
    }
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        "  srv_force_clh(%s)",
        deb_ascii_FullName(cle_name_buf,&cle_name)));

    /*
     * get a buffer to work with
     */
    buf_p = (struct buf *) dns_malloc (sizeof(struct buf));
    if (!buf_p) 
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug2,
          "  srv_force_clh: return (DNS_NONSRESOURCES)"));

	return (DNS_NONSRESOURCES);
    }
    
    ZERO_bytes (buf_p, sizeof(struct buf));

    /*
     * lock the server mgmt data memory section
     */

    SERVER_LOCK(ns_p->ns_mutex)
     /* Beginning of the 'protected code' */
    {
        /*
         * Find clearinghouse
         */
        ch_p = mgmt_find_chname (ns_p, &cle_name);
        if (!ch_p)
        {
	    dns_free ((char *)buf_p);

	    SERVER_END_LOCK(ns_p->ns_mutex)

	    DCE_SVC_DEBUG((
             cds__svc_handle,
             cds_svc_server,
             svc_c_debug2,
             "  srv_force_clh: return (DNS_UNKNOWNCLEARINGHOUSE)"));

            return (DNS_UNKNOWNCLEARINGHOUSE);
        }

	/*
         * fill in the user context
         */
	ZERO_bytes(&buf_p->b_user, sizeof(buf_p->b_user));
        status = user_setup (&buf_p->b_user, ch_p, NULL, h);
        if (status == error_status_ok) {
	    /*
	     *  Force action on clearinghouse
	     */
            status = force_clh(&buf_p->b_user, &ch_p->ch_uid, func);
            if (status == DNS_SUCCESS) {
/****
		dce_svc_printf(
		    DCE_SVC(cds__svc_handle,"%s"),
		    cds_svc_server,
		    svc_c_sev_notice,
		    cds_s_server_XXX,
		    deb_ascii_FullName(fullnameBuf,&ch_p->ch_name));

		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_server,
		    svc_c_debug5,
		    "  srv_force_clh: Event: Scheduled %d for clearinghouse %s",
		    func, deb_ascii_FullName(fullnameBuf,&ch_p->ch_name)));
****/
            }
        }
    } /* End of the 'protected code' */

    SERVER_END_LOCK(ns_p->ns_mutex)

    dns_free ((char *)buf_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <srv_force_clh(0x%lx) return(%d)",
        ns_p,
        status));

    return (status);
}

/*
 *
 *
 *  ROUTINE NAME:       force_clh
 *
 *  SCOPE:              INTERNAL
 *
 *  DESCRIPTION:
 *
 *      Dynamically force any of the following threads to run:
 *		Checkpoint
 *		Background
 *
 *  INPUTS:
 *	user_p	user_descriptor of the user
 *	chid_p	pointer to nameserver clearinghouse structure
 *	func	which thread to run
 *
 *  INPUTS/OUTPUTS:     none
 *
 *  OUTPUTS:
 *	CDS_SUCCESS
 *
 *  IMPLICIT INPUTS:    none
 *
 *  IMPLICIT OUTPUTS:   none
 *
 *  FUNCTION VALUE:     dns_status_t
 *
 *  SIDE EFFECTS:       none
 *
 *
 */

INTERNAL dns_status_t 
force_clh (user_descriptor_t *user_p,
	   uuid_t            *chid_p,
	   signed32           func)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_ObjUID(chidBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    int              thstatus;
    file_info_t     *file_p = NULL;
    db_stream_t     *stream_p = NULL;
    dns_status_t     status = CDS_SUCCESS;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >force_clh(%s, %s, %d)\n",
	deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ObjUID(chidBuf,chid_p), func));

    status = db_open_stream(chid_p, &stream_p, OP_ModifyAttribute);
    if (status != CDS_SUCCESS) {
        goto exit_force_clh;
    }
    file_p = stream_p->db_file_p;

    switch (func) {
      case FORCEBACKCMD:
	if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0) {
	    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	}
	file_p->file_back_force = 1;
	if ((thstatus = dthread_broadcast(&(file_p->file_state_changed))) < 0) {
	    dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	}

	while (file_p->file_back_force == 1) {
	    if ((thstatus = dthread_wait(&(file_p->file_back_done),
					 &(file_p->file_mutex))) < 0) {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
	    }
	}
	if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0) {
	    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}
 	break;

      case FORCECHECKCMD:
	db_checkpoint_schedule(file_p);
	if (stream_p)  db_close_stream(stream_p), stream_p = NULL;

	if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0) {
	    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	}

	while (file_p->file_ckpt_force == 1) {
	    if ((thstatus = dthread_wait(&(file_p->file_ckpt_done),
					 &(file_p->file_mutex))) < 0) {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
	    }
	}
	if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0) {
	    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}
	break;

      case REPAIRTIMECMD:
	status = RepairTimestamps_schedule(file_p);
	break;

      case REPAIRSTATUSCMD:
	status = GetRepairStatus();
	break;

      default:
	status = CDS_NOTIMPLEMENTED;
	break;
    }  /* end switch */


  exit_force_clh:
    if (stream_p)  db_close_stream(stream_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <force_clh(%s), returns %s\n",
	deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_dns_status(statusBuf,status)));

    return status;
}







