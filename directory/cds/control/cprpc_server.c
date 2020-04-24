/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cprpc_server.c,v $
 * Revision 1.1.9.2  1996/02/18  19:27:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:07  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:07:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:42  root]
 * 
 * Revision 1.1.7.5  1994/08/03  19:00:22  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:38:27  mccann]
 * 
 * Revision 1.1.7.4  1994/06/09  18:36:24  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:05  devsrc]
 * 
 * Revision 1.1.7.3  1994/04/19  17:05:28  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:30:16  mccann]
 * 
 * Revision 1.1.7.2  1994/04/04  20:05:59  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:05:33  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  21:57:50  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:57:14  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:02:01  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:31:49  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:43:30  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:20:24  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: cprpc_server.c
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
 * CDS Control Program Server Remote Procedures
 *
 * This module is part of the control program executable.
 * It handles commands destined for the CDS SERVER.
 *
 */
#include <cdscp.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <dce/server_ncl_defs.h> 
#include <dce/cprpc_server.h>
#include <dce/cprpcp.h>
#include <dce/rpc_cvt.h>
#include <dns_malloc.h>


/*
 *****************************************************************************
 * S E R V E R _ A T T R S [ ]
 *
 * This table identifies all the SERVER attributes and the name of
 * an output routine to display their values.
 *
 * NOTE: table is searched with 'bsearch', so it MUST BE SORTED.
 */

struct SERVER_ATTRS
{
    int attrid;			       /* the attribute ID (from
				        * 'dns_ncl_definitions.h') */
    void (*outrtn)(cprpc_emit_parms_p_t); /* the output display routine */
};


static struct SERVER_ATTRS ServerAttrs[] =
{
    /* attributes */
    {DNS_CREATE_TIME,            emit_server_create_time},
    {DNS_FUTURE_SKEW,            emit_server_future_skew},
    /* counters */
    {DNS_READ_ACCESS_COUNT,      emit_server_read_access_count},
    {DNS_WRITE_ACCESS_COUNT,     emit_server_write_access_count},
    {DNS_SKULKS_INITIATED_COUNT, emit_server_skulks_initiated_count},
    {DNS_SKULKS_COMPLETED_COUNT, emit_server_skulks_completed_count},
    {DNS_TREE_BROKEN_COUNT,      emit_server_tree_broken_count},
    {DNS_CRUCIAL_REPLICA_COUNT,  emit_server_crucial_replica_count},
    {DNS_CHILD_POINTER_COUNT,    emit_server_child_pointer_count},
    {DNS_SECURITY_COUNT,         emit_server_security_count},
    {DNS_KNOWN_CLEARINGHOUSES,   emit_server_known_clearinghouses}
};
#define NUM_SERVER_ATTRS (sizeof(ServerAttrs)/sizeof(struct SERVER_ATTRS))

/*
 *****************************************************************************
 * Forward declarations for internal (static) functions
 */
INTERNAL void EnumServerAttrs
    (cprpc_attr_t *,
     int *);

INTERNAL int id_cmp
    (void *,
     void *);

INTERNAL int get_parent
    (FullName_t *,
     FullName_t *,
     int        *);

INTERNAL int cp_show_server
    (cprpc_cmd_parm_blk_p_t);

INTERNAL int cp_disable_server
    (cprpc_cmd_parm_blk_p_t);

INTERNAL int cp_create_clh
    (cprpc_cmd_parm_blk_p_t);

INTERNAL int cp_clrdel_clh
    (cprpc_cmd_parm_blk_p_t);

INTERNAL int cp_show_clh
    (cprpc_cmd_parm_blk_p_t);


/*
**++
**
**  ROUTINE NAME:       cp_server_cmd
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**
**      Called by 'exec_mgmt_cmd', this is the entry point for all
**      DNS SERVER commands.
**
**  INPUTS:             none
**
**      cmd             the DNS SERVER command
**      entbp           ptr to head of entity list
**      atrbp           ptr to head of attribute list
**      envbp		ptr to environment block
**      msgbp           ptr to message control block
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

int cp_server_cmd (
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
         * server commands
         */
        case SHOWSRV:
        {
	    command_status = cp_show_server (&cmd_parm_blk);
            break;
        }

        case DISSRV:
        {
            command_status = cp_disable_server (&cmd_parm_blk);
            break;
        }

        /*
         * server clearinghouse commands
         */
        case CRECLECMD:
        {
	    command_status = cp_create_clh (&cmd_parm_blk);
            break;
        }
        
        case DELCLECMD:
        case CLEARCLE:
        {
	    command_status = cp_clrdel_clh (&cmd_parm_blk);
            break;
        }
        
        case SHOWSRVCLE:
        {
	    command_status = cp_show_clh (&cmd_parm_blk);
            break;
        }

        default:
        {
            command_status = DNS_ERROR;
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
**  ROUTINE NAME:       cp_show_server
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called by 'cp_server_cmd', this routine handles SHOW DNS
**      SERVER commands.
**
**      Server attributes are requested one at a time (i.e. one rpc
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

INTERNAL int cp_show_server (
cprpc_cmd_parm_blk_p_t cpbp)
{
    static handle_t rpc_binding;
    error_status_t rpc_status;
    command_status_t command_status;
    cprpc_attr_t cprpc_attrs[NUM_SERVER_ATTRS];
    cprpc_emit_parms_t cprpc_emit_parms;
    cprpc_attr_list_p_t cprpc_attr_list_p;
    int attr_id, index, attr_cnt;
    struct SERVER_ATTRS *serverattr_p;
    clh_info_list_t  clh_list, *clh_list_p;

    /*
     * init our clearinghouse list and its pointer
     */
    ZERO_bytes (&clh_list, sizeof (clh_list));
    clh_list_p = NULL; /* &clh_list; */

    /*
     * Enumerate the specified attributes and convert them to
     * canonical form for our RPC.
     */
    ZERO_bytes (cprpc_attrs, sizeof (cprpc_attrs));
    EnumServerAttrs (cprpc_attrs, &attr_cnt);

    /*
     * get a binding handle if we don't already have one
     */
    if (rpc_binding == NULL)
    {
	rpc_binding =
            cprpc_get_binding_handle (1, (cds_FullName_t *)0, &rpc_status);

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
	CDSCP_REFHDR_ENTLINE_SERVER, "SERVER", cpbp->nodename);

    /*
     * For each attribute, call the singular server managment RPC
     * stub.
     */
    for (index=0; index<attr_cnt; index++)
    {
	/*
         * allocate attribute list for RPC call
         */
	cprpc_attr_list_p =
            (cprpc_attr_list_t *) dns_malloc (sizeof (cprpc_attr_list_t)
                                          + sizeof (cprpc_attr_list_p->attr_members[0]));

	cprpc_attr_list_p->attr_members[0] = &cprpc_attrs[index];
	cprpc_attr_list_p->attr_len = 1;
        rpc_status =
            dnscp_server (rpc_binding,       /* our binding handle */
                          cpbp->cmd,	     /* requested function */
                          cpbp->caller_id,   /* caller's id */
                          cprpc_attr_list_p, /* attribute list */
                          &clh_list_p,       /* clearinghouse list */
                          &command_status);  /* command status */
        /*
         * Check for RPC error
         */
	if (rpc_status != rpc_s_ok)
	{
	    mgmt_error(rpc_status);
	    return (rpc_status);
	}
	if (command_status != DNS_SUCCESS)
	{
	    mgmt_error(command_status);
	    return (command_status);
	}

        /*
         * Get the output routine for this attribute
         */
	attr_id = cprpc_attrs[index].attr_id;
	serverattr_p = bsearch ((char *) &attr_id,
				(char *) ServerAttrs,
				NUM_SERVER_ATTRS,
				sizeof (struct SERVER_ATTRS),
				(int (*)(const void *,const void *)) id_cmp);
	if (!serverattr_p)
	{
	    dce_svc_printf(CDSCP_UNK_SVR_ATT_MSG, attr_id);
	}
	else if (!serverattr_p->outrtn)
	{
	    dce_svc_printf(CDSCP_UNIMPL_OUT_FNC_MSG, attr_id);
	}
	else
	{
	    /*
	     * Call the output routine
	     */
	    cprpc_emit_parms.attrp = cprpc_attr_list_p;
	    cprpc_emit_parms.clhp  = clh_list_p;
	    cprpc_emit_parms.outbp = cpbp->outbp;
	    (*serverattr_p->outrtn) (&cprpc_emit_parms);
	}

        /*
         * release this attribute structure
         */
	dns_free ((char *) cprpc_attr_list_p);	/* free RPC buffer */
    }   /* end of for() */

    /*
     * return with good status
     */
    return (DNS_SUCCESS);
}


/*
**++
**
**  ROUTINE NAME:       cp_disable_server
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Called by 'cp_server_cmd', this routine handles DISABLE
**      SERVER commands.
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

INTERNAL int cp_disable_server (
cprpc_cmd_parm_blk_p_t cpbp)
{
    static handle_t rpc_binding;
    error_status_t rpc_status;
    command_status_t command_status;

    /*
     * get an authenticated binding handle if we don't already have one
     */
    if (rpc_binding == NULL)
    {
	rpc_binding =
            cprpc_get_binding_handle (1, (cds_FullName_t *)0, &rpc_status);

	if (rpc_status != rpc_s_ok)
	{
	    mgmt_error( rpc_status);
            return (rpc_status);
	}
    }

    /*
     * Make the call to the server
     */
    rpc_status =
        dnscp_server (rpc_binding,       /* our binding handle */
                      cpbp->cmd,	 /* requested function */
                      cpbp->caller_id,	 /* caller's id */
                      NULL,              /* attribute list */
                      NULL,              /* clearinghouse list */
                      &command_status);	 /* command status */

    /*
     * Check for RPC error
     */
    if (rpc_status != rpc_s_ok)
    {
        mgmt_error(rpc_status);
        return (rpc_status);
    }
    if (command_status != DNS_SUCCESS)
    {
        mgmt_error(command_status);
        return (command_status);
    }

    /*
     * return with good status
     */
    return (DNS_SUCCESS);
}


/*
**++
**
**  ROUTINE NAME:       cp_create_clh
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**
**      Called by 'cp_server_cmd', this routine handles CREATE
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

INTERNAL int cp_create_clh (
cprpc_cmd_parm_blk_p_t cpbp)
{
    static handle_t rpc_binding;
    error_status_t rpc_status;
    command_status_t command_status;

    FullName_t          clh_parent_name;
    int                 clh_parent_name_len;
    FullName_t *        clh_name_p;
    FullName_t *        replica_name_p;
    cds_FullName_t      clh_name;
    cds_FullName_t      replica_name;
    clh_info_t          clh_info;
    clh_info_list_t     clh_info_list;
    clh_info_list_p_t   clh_info_list_p;
    struct attrblk *    dir_version_attr_p;

    /*
     * some sanity checking  of the input arguments and subsequent
     * error handling would be nice
     */
    if (cpbp->entbp->nxt == NULL)
    {
        return (DNS_ERROR); /* I know we can do better than this */
    }

    /*
     * clear out some stuff on the stack
     */
    ZERO_bytes (&clh_info, sizeof (clh_info));
    ZERO_bytes (&clh_name, sizeof (clh_name));
    ZERO_bytes (&replica_name, sizeof (replica_name));

    /*
     * init some hardwired stuff
     */
    clh_info.replica_type = RT_readOnly;
    clh_info.clh_name_p = &clh_name;
    clh_info.replica_name_p = &replica_name;

    /*
     * get the directory version if one was supplied
     *
     * note: we look for the directory version in dns_classversion
     *       (not dns_directoryversion) since that's where JanetM
     *       put it :-)
     */
    dir_version_attr_p = find_attr(cpbp->atrbp,
			(dns_opq_sname *)dns_AttributeName (DNS_CLASSVERSION));
    if (dir_version_attr_p)
    {
        clh_info.dir_version_p =
            (cds_Version_t *)dir_version_attr_p->av->vp->av_value;
    }

    /*
     * get the clearinghouse name from the entity list
     */
    clh_name_p = (FullName_t *)cpbp->entbp->nxt->name_p;

    /*
     * get the replica name from the entity list if one was
     * supplied
     */
    replica_name_p = NULL;
    if (cpbp->entbp->nxt->nxt)
    {
        if (cpbp->entbp->nxt->nxt->name_p)
        {
            replica_name_p = (FullName_t *)cpbp->entbp->nxt->nxt->name_p;
        }
    }

    /*
     * if a replica name was not supplied, then use the parent of
     * the clearinghouse
     */
    if (replica_name_p == NULL)
    {
        clh_parent_name_len = sizeof (clh_parent_name);
        command_status = get_parent ((FullName_t *)cpbp->entbp->nxt->name_p,
                                     &clh_parent_name,
                                     &clh_parent_name_len);
        if (command_status != DNS_SUCCESS)
        {
            mgmt_error(command_status);
            return (command_status);
        }
        replica_name_p = &clh_parent_name;
    }

    /*
     * convert the clearinghouse and replica names for sending over
     * the wire via RPC
     */
    cdsExport_FullName (NULL, clh_name_p, clh_info.clh_name_p);
    cdsExport_FullName (NULL, replica_name_p, clh_info.replica_name_p);

    /*
     * get authenticated binding handle if we don't already have one
     */
    if (rpc_binding == NULL)
    {
	rpc_binding =
            cprpc_get_binding_handle (1, (cds_FullName_t *)0, &rpc_status);

	if (rpc_status != rpc_s_ok)
	{
	    mgmt_error(rpc_status);
            return (rpc_status);
	}
    }

    /*
     * allocate/init the clearinghouse list (and its attributes)
     */
    clh_info.attr_list_p = NULL;
    clh_info_list.clh_count = 1;
    clh_info_list.clh_members[0] = &clh_info;
    clh_info_list_p = &clh_info_list;

    /*
     * make the create clearinghouse call to the server
     */
    rpc_status =
        dnscp_server (rpc_binding,	/* our binding handle */
                      cpbp->cmd,        /* requested function */
                      cpbp->caller_id,	/* caller's id */
                      NULL,             /* attribute list */
                      &clh_info_list_p, /* clearinghouse list */
                      &command_status);	/* command status */

    /*
     * Check for RPC error
     */
    if (rpc_status != rpc_s_ok)
    {
        mgmt_error(rpc_status);
        return (rpc_status);
    }
    if (command_status != DNS_SUCCESS)
    {
        mgmt_error(command_status);
        return (command_status);
    }

    /*
     * return with good status
     */
    return (DNS_SUCCESS);
}


/*
**++
**
**  ROUTINE NAME:       cp_clrdel_clh
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**
**      Called by 'cp_server_cmd', this routine handles CLEAR
**      and DELETE CLEARINGHOUSE commands.
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

INTERNAL int cp_clrdel_clh (
cprpc_cmd_parm_blk_p_t cpbp)
{
    static handle_t rpc_binding;
    error_status_t rpc_status;
    command_status_t command_status;

    FullName_t *        clh_name_p;
    cds_FullName_t      clh_name;

    clh_info_t          clh_info;
    clh_info_list_t     clh_info_list;
    clh_info_list_p_t   clh_info_list_p;

    /*
     * some sanity checking  of the input arguments and subsequent
     * error handling would be nice
     */
    if (cpbp->entbp->nxt == NULL)
    {
        return (DNS_ERROR); /* I know we can do better than this */
    }

    /*
     * clear out some stuff on the stack
     */
    ZERO_bytes (&clh_name, sizeof (clh_name));
    ZERO_bytes (&clh_info, sizeof (clh_info));

    /*
     * allocate/init the clearinghouse list (and its attributes)
     */
    clh_info.attr_list_p = NULL;
    clh_info.clh_name_p = &clh_name;
    clh_info_list.clh_count = 1;
    clh_info_list.clh_members[0] = &clh_info;
    clh_info_list_p = &clh_info_list;

    /*
     * get the clearinghouse name from the entity list and
     * convert it for sending over the wire via RPC
     */
    clh_name_p = (FullName_t *)cpbp->entbp->nxt->name_p;
    cdsExport_FullName (NULL, clh_name_p, clh_info.clh_name_p);

    /*
     * get authenticated binding handle if we don't already have one
     */
    if (rpc_binding == NULL)
    {
	rpc_binding =
            cprpc_get_binding_handle (1, (cds_FullName_t *)0, &rpc_status);

	if (rpc_status != rpc_s_ok)
	{
	    mgmt_error(rpc_status);
            return (rpc_status);
	}
    }

    /*
     * make the clear/delete clearinghouse call to the server
     */
    rpc_status =
        dnscp_server (rpc_binding,	/* our binding handle */
                      cpbp->cmd,        /* requested function */
                      cpbp->caller_id,	/* caller's id */
                      NULL,             /* attribute list */
                      &clh_info_list_p, /* clearinghouse list */
                      &command_status);	/* command status */

    /*
     * Check for RPC error
     */
    if (rpc_status != rpc_s_ok)
    {
        mgmt_error( rpc_status);
        return (rpc_status);
    }
    if (command_status != DNS_SUCCESS)
    {
        mgmt_error(command_status);
        return (command_status);
    }

    /*
     * return with good status
     */
    return (DNS_SUCCESS);
}


/*
**++
**
**  ROUTINE NAME:       cp_show_clh
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**
**      Called by 'cp_server_cmd', this routine handles SHOW
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

INTERNAL int cp_show_clh (
cprpc_cmd_parm_blk_p_t cpbp)
{
    handle_t             rpc_binding;
    error_status_t       rpc_status;
    command_status_t     command_status;
    cprpc_emit_parms_t   cprpc_emit_parms;

    FullName_t *         clh_name_p;
    cds_FullName_t       clh_name;

    clh_info_t           clh_info;
    clh_info_list_t      clh_info_list;
    clh_info_list_p_t    clh_info_list_p;

    /*
     * some sanity checking  of the input arguments and subsequent
     * error handling would be nice
     */
    if (cpbp->entbp->name_p == NULL)
    {
        return (DNS_ERROR); /* I know we can do better than this */
    }

    /*
     * clear out some stuff on the stack
     */
    ZERO_bytes (&clh_name, sizeof (clh_name));
    ZERO_bytes (&clh_info, sizeof (clh_info));

    /*
     * allocate/init the clearinghouse list (and its attributes)
     */
    clh_info.attr_list_p         = NULL;
    clh_info.clh_name_p          = &clh_name;
    clh_info_list.clh_count      = 1;
    clh_info_list.clh_members[0] = &clh_info;
    clh_info_list_p              = &clh_info_list;

    /*
     * get the clearinghouse name from the entity list and
     * convert it for sending over the wire via RPC
     */
    clh_name_p = (FullName_t *)cpbp->entbp->name_p;
    cdsExport_FullName (NULL, clh_name_p, clh_info.clh_name_p);

    /*
     * get authenticated binding handle
     */
    rpc_binding =
        cprpc_get_binding_handle (1, &clh_name, &rpc_status);
    
    if (rpc_status != rpc_s_ok)
    {
        mgmt_error(rpc_status);
        return (rpc_status);
    }

    /*
     * make the server call and get the clearinghouse information
     */
    rpc_status =
        dnscp_server (rpc_binding,      /* our binding handle */
                      cpbp->cmd,	/* requested function */
                      cpbp->caller_id,  /* caller's id */
                      NULL,             /* attribute list */
                      &clh_info_list_p, /* clearinghouse list */
                      &command_status); /* command status */
    /*
     * Check for RPC error
     */
    if (rpc_status != rpc_s_ok)
    {
        mgmt_error(rpc_status);
        return (rpc_status);
    }
    if (command_status != DNS_SUCCESS)
    {
        mgmt_error(command_status);
        return (command_status);
    }

    /*
     * Setup our emit parameter block
     */
    cprpc_emit_parms.nsp   = NULL;
    cprpc_emit_parms.clhp  = clh_info_list_p;
    cprpc_emit_parms.attrp = NULL;
    cprpc_emit_parms.outbp = cpbp->outbp;

    /*
     * display clearinghouse information
     */
    emit_clearinghouse_counters (&cprpc_emit_parms);

    /*
     * cleanup
     */
    rpc_binding_free (&rpc_binding, &rpc_status);
    if (rpc_status != rpc_s_ok)
    {
        mgmt_error( rpc_status);
        return (rpc_status);
    }

    /*
     * return with good status
     */
    return (DNS_SUCCESS);
}


/*
**++
**
**  ROUTINE NAME:       EnumServerAttrs
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      Enumerate server attributes.
**
**  INPUTS:
**
**      cprpc_attrs_p   ptr to rpc attribute list
**      attr_cnt_p      ptr to count of rpc attributes
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

INTERNAL void EnumServerAttrs (
cprpc_attr_t *cprpc_attrs_p,
int *attr_cnt_p)
{
    int count;
    struct SERVER_ATTRS *server_attrs_p;

    /*
     * just show all attributes
     */
    for (count = 0, server_attrs_p = ServerAttrs;
	 count < NUM_SERVER_ATTRS;
	 count++, cprpc_attrs_p++, server_attrs_p++)
    {
	cprpc_attrs_p->attr_id = server_attrs_p->attrid;
    }

    *attr_cnt_p = count;
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
**      searching the server attributes table.
**
**  INPUTS:
**
**      key_p           ptr to sought after table item (key)
**      tableitem       ptr to current table item
**
**	ANSI C dictates that these be void *'s as defined in stdlib.h
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
INTERNAL int id_cmp (
void * k_p,
void * tblitm)
{
    int *key_p = (int *)k_p;
    struct SERVER_ATTRS *tableitem = (struct SERVER_ATTRS *)tblitm;
    int e1 = *key_p;
    int e2 = tableitem->attrid;

    return ((e1 < e2) ? -1 : (e1 > e2));
}


/*
**++
**
**  ROUTINE NAME:       get_parent
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**      This routine returns the parent directory of a fullname.
**
**  INPUTS:
**
**      fullname_p      ptr to fullname
**      parent_p        ptr to parent
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

INTERNAL int get_parent (
FullName_t *fullname_p,
FullName_t *parent_p,
int        *parent_len_p)
{
    int snum;
    int status;

    /*
     * pick off all but the last simple name from the given fullname
     */
    snum = dnsCountSimple ((dns_opq_fname *)fullname_p);
    
    status = dnsScatter ((dns_opq_fname *)fullname_p, snum-1,
                         (dns_opq_fname *)parent_p, parent_len_p,
                         NULL,NULL,NULL,NULL);
    return (status);
}
