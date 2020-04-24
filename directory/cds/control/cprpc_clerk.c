/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cprpc_clerk.c,v $
 * Revision 1.1.15.2  1996/02/18  19:27:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:02  marty]
 *
 * Revision 1.1.15.1  1995/12/08  15:07:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:32  root]
 * 
 * Revision 1.1.12.7  1994/08/22  16:21:24  mccann
 * 	fix invalid binding problem: OT 10837
 * 	[1994/08/22  16:20:54  mccann]
 * 
 * Revision 1.1.12.6  1994/08/03  19:00:18  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:37:46  mccann]
 * 
 * Revision 1.1.12.5  1994/06/30  19:15:17  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:36:13  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.12.4  1994/06/09  18:36:20  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:00  devsrc]
 * 
 * Revision 1.1.12.3  1994/04/19  17:05:25  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:25:49  mccann]
 * 
 * Revision 1.1.12.2  1994/04/04  20:05:55  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:04:59  peckham]
 * 
 * Revision 1.1.12.1  1994/03/12  21:57:44  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:56:48  peckham]
 * 
 * Revision 1.1.10.1  1993/09/24  17:02:40  peckham
 * 	OT#8405: string copy can sometimes overwrite end of allocated buffer
 * 	    Fix string copy to not include trailing null.
 * 	[1993/09/24  17:02:11  peckham]
 * 
 * Revision 1.1.7.2  1993/08/12  22:34:26  jd
 * 	(CR7584)
 * 	Removed the call to DceError on bad command status immediately after
 * 	the call to dnscp_clerk. Later, in the if(!found_ns) block the correct
 * 	error message will be queued if the requested server wasn't specified
 * 	as a wild card. All show commands allow wildcard requests to fail
 * 	without declaring an error. On failure of dnscp_clerk the list of name
 * 	servers will be empty so no further real effort will be expended and
 * 	found_ns will remain false. The value of command_status set by
 * 	cprpc_clerk, if bad, will override the default of CDS_CSTAGUNKNOWN
 * 	set in the if(!found_ns) loop.
 * 	[1993/08/12  22:28:10  jd]
 * 
 * Revision 1.1.4.3  1992/12/30  13:01:39  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:31:21  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/15  20:10:40  m_sawyer
 * 	add new error status (CDS_CSTAGUNKNOWN) to cp_show_cached_server(),
 * 	cprpc_cmp_wildsimple() now returns a boolean and change
 * 	cp_show_cached_clearinghouse() to call cprpc_cmp_wildfull()
 * 	[1992/10/15  19:57:05  m_sawyer]
 * 
 * Revision 1.1.2.3  1992/04/30  20:37:55  m_sawyer
 * 	fix 'with' processing
 * 	[1992/04/30  20:28:52  m_sawyer]
 * 
 * Revision 1.1.2.2  1992/03/22  21:41:21  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:19:49  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: cprpc_clerk.c
 *
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
 *
 * ABSTRACT:
 *
 * CDS Control Program Clerk Remote Procedures
 *
 * This module is part of the control program executable.
 * It handles commands destined for the CDS CLERK.
 *
 */

#include <cdscp.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dns_malloc.h>
#include <dce/rpc_cvt.h>

#ifdef DCE_SEC
#include <dce/sec_login.h>
extern int Authentication;
extern sec_login_handle_t login_context_h;
#endif

/*
 * Include our own include file(s)
 */

#include <dce/cprpc_clerk.h>
#include <dce/cprpcp.h>
#include <adver_ncl_defs.h>


/*
 *****************************************************************************
 * C L E R K A T T R S [ ]
 *
 * This table identifies all the CLERK attributes, the group to which
 * they belong, and the name of an output routine to display their
 * values.
 *
 * NOTE: table is searched with 'bsearch', so it MUST BE SORTED.
 */

struct CLERK_ATTRS
{
    int attrid;			       /* the attribute ID (from
				        * 'dtss_mgmt_support.h') */
    void (*outrtn) (cprpc_emit_parms_p_t); /* the output display routine */
};

static struct CLERK_ATTRS ClerkAttrs[] =
{
    {CDS_CLERK_CREATE_TIME,          emit_clerk_create_time},
    /* counters */
    {CDS_CLERK_AUTHENTICATION_COUNT, emit_clerk_auth_count},
    {CDS_CLERK_READ_OPS_COUNT,       emit_clerk_read_ops_count},
    {CDS_CLERK_CACHE_HITS_COUNT,     emit_clerk_cache_hits_count},
    {CDS_CLERK_CACHE_BYPASS_COUNT,   emit_clerk_cache_bypass_count},
    {CDS_CLERK_WRITE_OPS_COUNT,      emit_clerk_write_ops_count},
    {CDS_CLERK_MISC_OPS_COUNT,       emit_clerk_misc_ops_count}
};
#define NUM_CLERK_ATTRS (sizeof(ClerkAttrs)/sizeof(struct CLERK_ATTRS))


/*
 *****************************************************************************
 * Forward declarations for internal (static) functions
 */

INTERNAL int cp_show_clerk
    (cprpc_cmd_parm_blk_p_t);

INTERNAL int cp_disable_clerk
    (cprpc_cmd_parm_blk_p_t);

INTERNAL int cp_show_cached_clearinghouse
    (cprpc_cmd_parm_blk_p_t);

INTERNAL void show_cached_clearinghouse
    (cprpc_cmd_parm_blk_p_t,
     clh_info_p_t);

INTERNAL int cp_define_cached_server
    (cprpc_cmd_parm_blk_p_t);

INTERNAL int cp_show_cached_server
    (cprpc_cmd_parm_blk_p_t);

INTERNAL void show_cached_server
    (cprpc_cmd_parm_blk_p_t,
     ns_info_p_t,
     SimpleName_t *);

INTERNAL int cp_clear_cached_server
    (cprpc_cmd_parm_blk_p_t);

INTERNAL void EnumClerkAttrs
    (cprpc_attr_t *,
     int *);

INTERNAL int id_cmp
    (void *,
     void *);

/*
**++
**
**  ROUTINE NAME:       cp_clerk_cmd
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**
**      Called by 'exec_mgmt_cmd', this is the entry point for all
**      DNS CLERK commands.
**
**      This function enumerates the clerk attribute groups, if
**      necessary, adding them to the attribute list (BTW, who frees
**      this list???).  It then calls cprpc_cmd() which loops thru
**      the attribute list and makes an RPC call to aquire the value
**      from the clerk (advertiser).
**
**  INPUTS:             none
**
**      cmd             the DNS CLERK command
**      entbp           ptr to head of entity list
**      atrbp           ptr to head of attribute list
**      envbp		ptr to environment block
**      outbp		ptr to head of output list
**
**  INPUTS/OUTPUTS:     none
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

int
cp_clerk_cmd (
int cmd,
struct entblk  *entbp,
struct attrblk *atrbp,
struct envblk  *envbp,
struct op_blk  *outbp)
{
    command_status_t command_status;
    cprpc_cmd_parm_blk_t cmd_parm_blk;

    /*
     * local initialization
     */
    cmd_parm_blk.cmd = (long) cmd;
    cmd_parm_blk.caller_id = (long) geteuid();
    cmd_parm_blk.nodename = "";
    cmd_parm_blk.entbp = entbp;
    cmd_parm_blk.atrbp = atrbp;
    cmd_parm_blk.envbp = envbp;
    cmd_parm_blk.outbp = outbp;

    /*
     * Switch on the given command
     */
    switch (cmd)
    {
        /*
         * clerk commands
         */
        case SHOWCLK:
        {
            command_status = cp_show_clerk (&cmd_parm_blk);
            break;
        }

        case DISCLK:
        {
            command_status = cp_disable_clerk (&cmd_parm_blk);
            break;
        }

        /*
         * clerk clearinghouse commands
         */
        case SHOWCLKCLE:
        {
            command_status = cp_show_cached_clearinghouse (&cmd_parm_blk);
            break;
        }

        /*
         * cached server commands
         */
        case DEFCLKSRV:
        {
            command_status = cp_define_cached_server (&cmd_parm_blk);
            break;
        }

        case SHOWCLKSRV:
        {
            command_status = cp_show_cached_server (&cmd_parm_blk);
            break;
        }

        case CLEARCLKSRV:
        {
            command_status = cp_clear_cached_server (&cmd_parm_blk);
            break;
        }

        default:
        {
            command_status = CDS_ERROR;
            break;
        }
      }

    /*
     * return with status
     */
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       cp_show_clerk
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called by 'cp_clerk_cmd', this routine handles SHOW DNS
**      CLERK commands.
**
**      Note: corresponding function with same name occurs in
**            server.
**
**      Clerk attributes are requested one at a time (i.e. one rpc
**      call per attribute). If this incurs too much of a
**      performance penalty for requests for multiple (re: all)
**      attributes, the IDL allows us to request multiple attributes
**      per request.
**
**  INPUTS:
**
**      cpbp            ptr to our command parameter block
**
**  INPUTS/OUTPUTS:     none
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

INTERNAL int cp_show_clerk (
cprpc_cmd_parm_blk_p_t cpbp)
{
    static handle_t rpc_binding = NULL;
    error_status_t rpc_status;
    command_status_t command_status;
    cprpc_attr_t cprpc_attrs[NUM_CLERK_ATTRS];
    cprpc_emit_parms_t cprpc_emit_parms;
    cprpc_attr_list_t *cprpc_attr_list_p;
    int attr_id, index, attr_cnt;
    struct CLERK_ATTRS *clerkattr_p;

    /*
     * Enumerate the specified attributes and convert them to
     * canonical form for our RPC.
     */
    ZERO_bytes (cprpc_attrs, sizeof (cprpc_attrs));
    EnumClerkAttrs (cprpc_attrs, &attr_cnt);

    /*
     * get a binding handle if we don't already have one
     */
    if (rpc_binding == NULL)
    {
	rpc_binding = cprpc_get_binding_handle (0,
                                                (cds_FullName_t *)0,
                                                &rpc_status);
	if (rpc_status != rpc_s_ok)
	{
	  mgmt_error(rpc_status);
	  return (rpc_status);
	}
    }

    /*
     * Show the header
     */
    cprpc_emit_show_header (cpbp->outbp,
	CDSCP_REFHDR_ENTLINE_CLERK, "CLERK", cpbp->nodename);

    /*
     * For each attribute, call the singular clerk managment RPC stub.
     */
    for (index=0; index<attr_cnt; index++)
    {
	/*
         * allocate attribute list for RPC call
         */
	cprpc_attr_list_p =
            (cprpc_attr_list_t *) dns_malloc (sizeof (cprpc_attr_list_t) +
                                              sizeof (cprpc_attr_list_p->attr_members[0]));
        if (!cprpc_attr_list_p)
        {
	  mgmt_error(CDS_NONSRESOURCES);
	  return(CDS_NONSRESOURCES);
        }

        /*
         * put one attribute in the attribute list
         */
	cprpc_attr_list_p->attr_members[0] = &cprpc_attrs[index];
	cprpc_attr_list_p->attr_len = 1;

        /*
         * make the request
         */
	rpc_status =
            dnscp_clerk (rpc_binding,	        /* our binding handle */
                         cpbp->cmd,             /* requested function */
                         cpbp->caller_id,       /* caller's id */
                         cprpc_attr_list_p,     /* attribute list */
                         NULL,                  /* clearinghouse list */
                         NULL,                  /* nameserver list */
                         &command_status);	/* command status */

        /*
         * Check for RPC error
         */
	if (rpc_status != rpc_s_ok)
	{
	  mgmt_error(rpc_status);
	  break;
	}
	if (command_status != CDS_SUCCESS)
	{
	  mgmt_error(command_status);
	  break;
	}

        /*
         * Get the output routine for this attribute
         */
	attr_id = cprpc_attrs[index].attr_id;
	clerkattr_p = bsearch ((char *) &attr_id,
                               (char *) ClerkAttrs,
                               NUM_CLERK_ATTRS,
                               sizeof (struct CLERK_ATTRS),
                               (int (*)(const void *,const void *)) id_cmp);
	if (!clerkattr_p)
	{
	    dce_svc_printf(CDSCP_UNK_CLRK_ATT_MSG, attr_id);
	}
	else if (!clerkattr_p->outrtn)
	{
	    dce_svc_printf(CDSCP_UNIMPL_OUT_FNC_MSG, attr_id);
	}
	else
	{
	    /*
	     * Call the output routine
	     */
	    cprpc_emit_parms.attrp = cprpc_attr_list_p;
	    cprpc_emit_parms.outbp = cpbp->outbp;
	    (*clerkattr_p->outrtn) (&cprpc_emit_parms);
	}

        /*
         * release this attribute structure
         */
	dns_free ((char *) cprpc_attr_list_p);	/* free RPC buffer */
    }   /* end of for() */

    /*
     * return with status (rpc_status or command_status)
     */
    if (rpc_status != rpc_s_ok)
    {
        command_status = rpc_status;
    }
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       cp_show_cached_clearinghouse
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called by 'cp_clerk_cmd', this routine handles SHOW CACHED
**      CLEARINGHOUSE commands.
**
**  INPUTS:
**
**      cpbp            ptr to our command parameter block
**
**  INPUTS/OUTPUTS:     none
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

INTERNAL int cp_show_cached_clearinghouse (
cprpc_cmd_parm_blk_p_t cpbp)
{
    static handle_t rpc_binding = NULL;
    error_status_t rpc_status;
    command_status_t command_status;
    int clh_index, clh_count;
    clh_info_list_t clh_info_list;
    clh_info_list_p_t clh_info_list_p;
    FullName_t *desired_clh_p;
    FullName_t  desired_clh_wild;
    int         desired_clh_wild_len;
    FullName_t  possible_clh;
    int need_nl = FALSE;

    /*
     * get a binding handle if we don't already have one
     */
    if (rpc_binding == NULL)
    {
	rpc_binding = cprpc_get_binding_handle (0,
                                                (cds_FullName_t *)0,
                                                &rpc_status);
	if (rpc_status != rpc_s_ok)
	{
	  mgmt_error(rpc_status);
	  return (rpc_status);
	}
    }

    /*
     * init our in/out parameter
     */
    clh_info_list.clh_count      = 0;
    clh_info_list.clh_members[0] = NULL;
    clh_info_list_p              = &clh_info_list;

    /*
     * make the clerk call and get the clearinghouse information
     */
    rpc_status =
        dnscp_clerk (rpc_binding,	/* our binding handle */
                     cpbp->cmd,         /* requested function */
                     cpbp->caller_id,   /* caller's id */
                     NULL,              /* attribute list */
                     &clh_info_list_p,  /* clearinghouse list */
                     NULL,              /* nameserver list */
                     &command_status);	/* command status */
    /*
     * Check for RPC error
     */
    if (rpc_status != rpc_s_ok)
    {
      mgmt_error(rpc_status);
    }
    if (command_status != CDS_SUCCESS)
    {
      mgmt_error(command_status);
    }

    /*
     * Point to the desired clearinghouse
     */
    if (cpbp->entbp->nxt->wild)
    {
        desired_clh_wild_len = sizeof(desired_clh_wild);
        command_status =
            cdsGather ((cds_full_name_t *)cpbp->entbp->nxt->name_p,
                       (cds_simple_name_t *)&cpbp->entbp->nxt->wildname,
                       (cds_full_name_t *)NULL,
                       (cds_full_name_t *)&desired_clh_wild,
                       &desired_clh_wild_len);
        if (command_status != CDS_SUCCESS)
        {
            mgmt_error(command_status);
        }
        desired_clh_p = &desired_clh_wild;
    }
    else
    {
        desired_clh_p = (FullName_t *)cpbp->entbp->nxt->name_p;
    }

    /*
     * For each clearinghouse in the list that matches the requested
     * clearinghouse, display all attributes
     */
    clh_count = clh_info_list_p->clh_count;
    for (clh_index=0; clh_index<clh_count; clh_index++)
    {
        /*
         * Import the next clearinghouse
         */
        ZERO_bytes (&possible_clh, sizeof (possible_clh));
        cdsImport_FullName (NULL,
                            &clh_info_list_p->clh_members[clh_index]->clh_replica_p->rp_chname,
                            &possible_clh,
                            0);
        /*
         * Display the clearinghouse information if the fullnames match
         */
        if (cprpc_cmp_wildfull (&possible_clh, desired_clh_p))
        {
            if (need_nl) emit_line(cpbp->outbp, "\n");
            show_cached_clearinghouse (cpbp, clh_info_list_p->clh_members[clh_index]);
            need_nl = TRUE;
        }
    }

    /*
     * return with status (rpc_status or command_status)
     */
    if (rpc_status != rpc_s_ok)
    {
        command_status = rpc_status;
    }
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       show_cached_clearinghouse
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called by 'cp_show_cached_clearinghouse', this routine
**      displays the information for a clearinghouse.
**
**  INPUTS:
**
**      cpbp            ptr to our command parameter block
**      clh_info_p      ptr to clearinghouse information
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

INTERNAL void show_cached_clearinghouse (
cprpc_cmd_parm_blk_p_t cpbp,
clh_info_p_t           clh_info_p)
{
    clh_info_list_t     clh_info_list;
    cprpc_emit_parms_t  cprpc_emit_parms;

    /*
     * Create a list of one clearinghouse
     */
    clh_info_list.clh_count = 1;
    clh_info_list.clh_members[0] = clh_info_p;

    /*
     * Setup our emit parameter block
     */
    cprpc_emit_parms.nsp   = NULL;
    cprpc_emit_parms.clhp  = &clh_info_list;
    cprpc_emit_parms.attrp = NULL;
    cprpc_emit_parms.outbp = cpbp->outbp;

    /*
     * Display the cached clearinghouse information
     */
    emit_cached_clh_info (&cprpc_emit_parms);
}


/*
**++
**
**  ROUTINE NAME:       cp_disable_clerk
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called by 'cp_clerk_cmd', this routine handles DISABLE
**      CLERK commands.
**
**  INPUTS:
**
**      cpbp            ptr to our command parameter block
**
**  INPUTS/OUTPUTS:     none
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

INTERNAL int cp_disable_clerk (
cprpc_cmd_parm_blk_p_t cpbp)
{
    static handle_t rpc_binding;
    error_status_t rpc_status;
    command_status_t command_status;

    /*
     * get authenticated binding handle if we don't already have one
     */
    if (rpc_binding == NULL)
    {
	rpc_binding = cprpc_get_binding_handle (0,
                                                (cds_FullName_t *)0,
                                                &rpc_status);
	if (rpc_status != rpc_s_ok)
	{
	    mgmt_error(rpc_status);
            return (rpc_status);
	}
    }

    /*
     * Make the call to the clerk
     */
    rpc_status =
        dnscp_clerk (rpc_binding,       /* our binding handle */
                     cpbp->cmd,	        /* requested function */
                     cpbp->caller_id,	/* caller's id */
                     NULL,              /* attribute list */
                     NULL,              /* clearinghouse list */
                     NULL,              /* nameserver list */
                     &command_status);	/* command status */

    /*
     * Check for RPC error
     */
    if (rpc_status != rpc_s_ok)
    {
        mgmt_error(rpc_status);
    }
    if (command_status != CDS_SUCCESS)
    {
        mgmt_error(command_status);
    }

    /*
     * return with status (rpc_status or command_status)
     */
    if (rpc_status != rpc_s_ok)
    {
        command_status = rpc_status;
    }
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       cp_define_cached_server
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called by 'cp_clerk_cmd()', this routine adds a cached
**      server.
**
**  INPUTS:
**
**      cpbp            ptr to our command parameter block
**
**  INPUTS/OUTPUTS:     none
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

INTERNAL int cp_define_cached_server (
cprpc_cmd_parm_blk_p_t cpbp)
{
    static handle_t rpc_binding = NULL;
    error_status_t rpc_status;
    command_status_t command_status;
    ns_info_t ns_info;
    ns_info_list_p_t ns_info_list_p;
    cprpc_attr_t ns_attr;
    cprpc_attr_list_p_t ns_attr_list_p;
    cds_visa_t *ns_visa_p;
    cds_Name_t ns_tag;
    cds_OpenChar_t *tower_string_valp;
    int tower_string_len;

    /*
     * get authenticated binding handle if we don't already have one
     */
    if (rpc_binding == NULL)
    {
	rpc_binding = cprpc_get_binding_handle (0,
                                                (cds_FullName_t *)0,
                                                &rpc_status);
	if (rpc_status != rpc_s_ok)
	{
	    mgmt_error(rpc_status);
            return (rpc_status);
	}
    }

    /*
     * export the cached server 'tag'
     */
    ZERO_bytes (&ns_tag, sizeof (ns_tag));
    ns_visa_p = (cds_visa_t *)NULL;
    cdsExport_SimpleName (ns_visa_p,
                          (SimpleName_t *)sname_pointer (),
                          &ns_tag);

    /*
     * create the cds 'openchar' structure
     */
    tower_string_len = EXT16(cpbp->atrbp->fwd->av->vp->av_length);
    tower_string_valp =
        (cds_OpenChar_t *) dns_malloc (sizeof(cds_OpenChar_t) +
                                       tower_string_len + sizeof(char));
    if (!tower_string_valp)
    {
        mgmt_error(CDS_NONSRESOURCES);
        return(CDS_NONSRESOURCES);
    }
    tower_string_valp->op_length = tower_string_len + sizeof(char);
    strncpy ((char *)(tower_string_valp->op_array),
            (char *)cpbp->atrbp->fwd->av->vp->av_value, tower_string_len);
    /* add null terminator */
    tower_string_valp->op_array[tower_string_len] = '\0';

    /*
     * convert the tower string to an attribute
     */
    ns_attr.attr_id = CP_char;
    ns_attr.value.av_valuetype = CP_char;
    ns_attr.value.av_val.av_char_p = tower_string_valp;

    /*
     * allocate and initialize an attribute list
     */
    ns_attr_list_p =
        (cprpc_attr_list_t *) dns_malloc (sizeof (cprpc_attr_list_t) +
                                          sizeof (cprpc_attr_list_p_t));
    if (!ns_attr_list_p)
    {
        dns_free (tower_string_valp);
        mgmt_error(CDS_NONSRESOURCES);
        return(CDS_NONSRESOURCES);
    }
    ns_attr_list_p->attr_members[0] = &ns_attr;
    ns_attr_list_p->attr_len = 1;

    /*
     * init our ns 'info' structure
     */
    ns_info.ns_tag_p    = &ns_tag;
    ns_info.attr_list_p = ns_attr_list_p;

    /*
     * init our in/out list parameter
     */
    ns_info_list_p =
        (ns_info_list_p_t) dns_malloc (sizeof (ns_info_list_t) +
                                       sizeof (ns_info_list_p_t));
    if (!ns_info_list_p)
    {
        dns_free (tower_string_valp);
        mgmt_error(CDS_NONSRESOURCES);
        return(CDS_NONSRESOURCES);
    }
    ns_info_list_p->ns_members[0] = &ns_info;
    ns_info_list_p->ns_count = 1;

    /*
     * Make the call to the clerk
     */
    rpc_status =
        dnscp_clerk (rpc_binding,       /* our binding handle */
                     cpbp->cmd,	        /* requested function */
                     cpbp->caller_id,	/* caller's id */
                     NULL,              /* attribute list */
                     NULL,              /* clearinghouse list */
                     &ns_info_list_p,   /* nameserver list */
                     &command_status);	/* command status */

    /*
     * Check for RPC error
     */
    if (rpc_status != rpc_s_ok)
    {
        mgmt_error(rpc_status);
	
    }
    if (command_status != CDS_SUCCESS)
    {
        printf("%s\n", tower_string_valp->op_array);
        mgmt_error(command_status);
    }

    /*
     * cleanup after ourselves
     */
    dns_free (tower_string_valp);

    /*
     * return with status (rpc_status or command_status)
     */
    if (rpc_status != rpc_s_ok)
    {
        command_status = rpc_status;
    }
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       cp_show_cached_server
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called by 'cp_clerk_cmd()', this routine displays cached
**      server information.
**
**  INPUTS:
**
**      cpbp            ptr to our command parameter block
**
**  INPUTS/OUTPUTS:     none
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

INTERNAL int cp_show_cached_server (
cprpc_cmd_parm_blk_p_t cpbp)
{
    static handle_t rpc_binding = NULL;
    error_status_t rpc_status;
    command_status_t command_status = CDS_SUCCESS;
    int ns_index, ns_count;
    ns_info_list_t ns_info_list;
    ns_info_list_p_t ns_info_list_p;
    SimpleName_t possible_ns;
    SimpleName_t *desired_ns_p;
    int need_nl = FALSE;
    int found_ns = FALSE;

    /*
     * get a binding handle if we don't already have one
     */
    if (rpc_binding == NULL)
    {
	rpc_binding = cprpc_get_binding_handle (0,
                                                (cds_FullName_t *)0,
                                                &rpc_status);
	if (rpc_status != rpc_s_ok)
	{
	    mgmt_error(rpc_status);
	    return (rpc_status);
	}
    }

    /*
     * init our in/out parameter
     */
    ns_info_list.ns_count = 0;
    ns_info_list.ns_members[0] = NULL;

    ns_info_list_p = &ns_info_list;

    /*
     * make the clerk call and get the nameserver information
     */
    rpc_status =
        dnscp_clerk (rpc_binding,	/* our binding handle */
                     cpbp->cmd,         /* requested function */
                     cpbp->caller_id,   /* caller's id */
                     NULL,              /* attribute list */
                     NULL,              /* clearinghouse list */
                     &ns_info_list_p,   /* nameserver list */
                     &command_status);	/* command status */
    /*
     * Check for RPC error
     */
    if (rpc_status != rpc_s_ok)
    {
        mgmt_error(rpc_status);
	return(rpc_status);
    }

    if (command_status != CDS_SUCCESS)
    {
        mgmt_error(command_status);
	return(command_status);
    }

    /*
     * Point to the desired nameserver
     */
    desired_ns_p = (SimpleName_t *)wild_sname_pointer ();

    /*
     * Display the information for each nameserver in the list that matches
     * the requested nameserver.
     */
    ns_count = ns_info_list_p->ns_count;
    for (ns_index=0; ns_index<ns_count; ns_index++)
    {
        cds_visa_t *cds_visa_p;

        /*
         * Import the next nameserver
         */
        cds_visa_p = (cds_visa_t *)NULL;
        ZERO_bytes (&possible_ns, sizeof (possible_ns));
        cdsImport_SimpleName (cds_visa_p,
                              ns_info_list_p->ns_members[ns_index]->ns_tag_p,
                              &possible_ns,
                              FALSE);

        /*
         * Display the nameserver information if the simplenames
         * match
         */
        if (cprpc_cmp_wildsimple (&possible_ns, desired_ns_p))
        {
            if (need_nl) emit_line(cpbp->outbp, "\n");
            show_cached_server (cpbp,
                                ns_info_list_p->ns_members[ns_index],
                                &possible_ns);
            need_nl = TRUE;
            found_ns = TRUE;
        }
    }

    /*
     * See if we successfully found and displayed a nameserver
     */
    if (!found_ns)
    {
        int flag;

        /*
         * We treat the case where we were asked to find a cached
         * server with a wildcard differently.  Searches for cached
         * server tags with wildcards are allowed to "fail" without
         * declaring an error.  This is consistant with all of the
         * other "SHOW" commands.
         */
        flag = EXT8(desired_ns_p->sn_flag);
        if ((flag != SN_cdswildcard) && (flag != SN_wildcard))
        {
	    /*
	     * If wasn't set to something other than success by
	     * cprpc_clerk set it now to tag unknown.
	     */
	    if(command_status == CDS_SUCCESS)
		command_status = CDS_CSTAGUNKNOWN;
            mgmt_error(command_status);
        }
    }

    /*
     * return with status (rpc_status or command_status)
     */
    if (rpc_status != rpc_s_ok)
    {
        command_status = rpc_status;
    }
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       show_cached_server
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called by 'cp_show_cached_nameserver', this routine
**      displays the information for a cached server.
**
**  INPUTS:
**
**      cpbp            ptr to our command parameter block
**      ns_info_p       ptr to nameserver information
**      name_tag_p      ptr to nameserver 'tag'
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

INTERNAL void show_cached_server (
cprpc_cmd_parm_blk_p_t cpbp,
ns_info_p_t            ns_info_p,
SimpleName_t           *name_tag_p)
{
    cprpc_emit_parms_t  cprpc_emit_parms;
    ns_info_list_t ns_info_list;
    char name_str[256];
    int name_len = sizeof(name_str);
    error_status_t status;

    /*
     * display the header with the cached server name 'tag'
     */
    status = cdsCvtSimpleToStr ((cds_simple_name_t *)name_tag_p,
                                   (unsigned char *)name_str,
                                   &name_len);
    if (status != CDS_SUCCESS)
    {
        mgmt_error(status);
    }

    cprpc_emit_show_header (cpbp->outbp,
		CDSCP_REFHDR_ENTLINE_CANS, "CACHED NAMESERVER", name_str);

    /*
     * Create a list of one nameserver
     */
    ns_info_list.ns_count = 1;
    ns_info_list.ns_members[0] = ns_info_p;

    /*
     * Setup our emit parameter block
     */
    cprpc_emit_parms.nsp   = &ns_info_list;
    cprpc_emit_parms.clhp  = NULL;
    cprpc_emit_parms.attrp = NULL;
    cprpc_emit_parms.outbp = cpbp->outbp;

    /*
     * Display the cached server information
     */
    emit_cached_ns_info (&cprpc_emit_parms);
}


/*
**++
**
**  ROUTINE NAME:       cp_clear_cached_server
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called by 'cp_clerk_cmd()', this routine removes cached
**      server information.
**
**  INPUTS:
**
**      cpbp            ptr to our command parameter block
**
**  INPUTS/OUTPUTS:     none
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

INTERNAL int cp_clear_cached_server (
cprpc_cmd_parm_blk_p_t cpbp)
{
    static handle_t rpc_binding = NULL;
    error_status_t rpc_status;
    command_status_t command_status;
    ns_info_t ns_info;
    ns_info_list_t ns_info_list;
    ns_info_list_p_t ns_info_list_p;
    cds_visa_t *ns_visa_p;
    cds_Name_t ns_tag;

    /*
     * export the cached server 'tag'
     */
    ZERO_bytes (&ns_tag, sizeof (ns_tag));
    ns_visa_p = (cds_visa_t *)NULL;
    cdsExport_SimpleName (ns_visa_p,
                          (SimpleName_t *)sname_pointer (),
                          &ns_tag);

    /*
     * init our 'info' structure
     */
    ns_info.ns_tag_p = &ns_tag;
    ns_info.attr_list_p = (cprpc_attr_list_t *)NULL;

    /*
     * init our in/out list parameter
     */
    ns_info_list.ns_count = 1;
    ns_info_list.ns_members[0] = &ns_info;
    ns_info_list_p = &ns_info_list;

    /*
     * get authenticated binding handle if we don't already have one
     */
    if (rpc_binding == NULL)
    {
	rpc_binding = cprpc_get_binding_handle (0,
                                                (cds_FullName_t *)0,
                                                &rpc_status);
	if (rpc_status != rpc_s_ok)
	{
	    mgmt_error(rpc_status);
            return (rpc_status);
	}
    }

    /*
     * Make the call to the clerk
     */
    rpc_status =
        dnscp_clerk (rpc_binding,       /* our binding handle */
                     cpbp->cmd,	        /* requested function */
                     cpbp->caller_id,	/* caller's id */
                     NULL,              /* attribute list */
                     NULL,              /* clearinghouse list */
                     &ns_info_list_p,   /* nameserver list */
                     &command_status);	/* command status */

    /*
     * Check for RPC error
     */
    if (rpc_status != rpc_s_ok)
    {
        mgmt_error(rpc_status);
    }
    if (command_status != CDS_SUCCESS)
    {
        mgmt_error(command_status);
    }

    /*
     * return with status (rpc_status or command_status)
     */
    if (rpc_status != rpc_s_ok)
    {
        command_status = rpc_status;
    }
    return (command_status);
}


/*
**++
**
**  ROUTINE NAME:       EnumClerkAttrs
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Enumerate clerk attributes.
**
**  INPUTS:
**
**      cprpc_attrs     ptr to rpc attribute list
**      attr_cnt        ptr to count of rpc attributes
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

INTERNAL void EnumClerkAttrs (
cprpc_attr_t *cprpc_attrs,
int *attr_cnt)
{
    int index;
    struct CLERK_ATTRS *clerkattrs;

    /*
     * for now, just show all attributes, whether or not all were
     * requested or not
     */
    for (index = 0, clerkattrs = ClerkAttrs;
	 index < NUM_CLERK_ATTRS;
	 index++)
    {
	cprpc_attrs[index].attr_id = clerkattrs[index].attrid;
    }

    *attr_cnt = index;

}

/*
**++
**
**  ROUTINE NAME:       id_cmp
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      This routine is the compare routine used by 'bsearch' when
**      searching the clerk attributes table.
**
**  INPUTS:
**
**      key_p           ptr to sought after table item (key)
**      tableitem       ptr to current table item
**
**	ANSI C requires the signature of this function to be as defined
**	by stdlib.h; thus the inputs are both void *'s.
**
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

INTERNAL
int id_cmp (
void * k_p,
void * tblitm)
{
    int *key_p = (int *)k_p;
    struct CLERK_ATTRS *tableitem = (struct CLERK_ATTRS *)tblitm;
    int e1 = *key_p;
    int e2 = tableitem->attrid;

    return ((e1 < e2) ? -1 : (e1 > e2));
}

