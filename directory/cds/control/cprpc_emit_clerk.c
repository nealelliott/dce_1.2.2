/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cprpc_emit_clerk.c,v $
 * Revision 1.1.8.2  1996/02/18  19:27:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:04  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:07:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:36  root]
 * 
 * Revision 1.1.6.5  1994/08/03  19:00:20  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:38:02  mccann]
 * 
 * Revision 1.1.6.4  1994/06/09  18:36:22  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:02  devsrc]
 * 
 * Revision 1.1.6.3  1994/04/19  17:05:26  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:30:31  mccann]
 * 
 * Revision 1.1.6.2  1994/04/04  20:05:56  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:05:12  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:57:47  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:56:57  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:01:46  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:31:28  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/26  07:10:13  jim
 * 	Added include msgblk.h. This is not getting included on AIX 3.2
 * 	for some reason.
 * 	[1992/05/29  18:46:27  jim]
 * 
 * Revision 1.1.2.2  1992/03/22  21:42:12  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:20:00  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: cprpc_emit_clerk.c
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
 *
 * This module contains the (many) clerk emit routines that are used to
 * display clerk management information.  This module is part of the
 * control program executable.
 *
 */

/*
 * Include any special system/dns related include files
 */

#include <cdscp.h>

#include <dns_malloc.h>
#include <dce/rpc_cvt.h>
#include <adver_ncl_defs.h>
#include <dce/cprpc.h>
#include <dce/cprpcp.h>


/*
 *****************************************************************************
 * Forward declarations for internal (static) functions
 */
INTERNAL void emit_cached_clh_name
    (cprpc_emit_parms_p_t);

INTERNAL void emit_cached_clh_towerset
    (cprpc_emit_parms_p_t);

INTERNAL void emit_time
    (struct op_blk *,
     unsigned,
     char *,
     cprpc_attr_t *);

/***********************************************************************/
void
emit_clerk_create_time (
cprpc_emit_parms_p_t emitp)
{
   emit_time (emitp->outbp,
	      CDSCP_CLERKCTS, "Creation Time",
	      emitp->attrp->attr_members[0]);
}

/***********************************************************************/
void
emit_clerk_auth_count (
cprpc_emit_parms_p_t emitp)
{
    emit_attr_number(emitp->outbp,
	CDSCP_CLERKAF, "Authentication Failures",
	emitp->attrp->attr_members[0]->value.av_val.av_counter);
}

/***********************************************************************/
void
emit_clerk_read_ops_count (
cprpc_emit_parms_p_t emitp)
{
    emit_attr_number(emitp->outbp,
        CDSCP_CLERKRO, "Read Operations",
        emitp->attrp->attr_members[0]->value.av_val.av_counter);
}

/***********************************************************************/
void
emit_clerk_cache_hits_count (
cprpc_emit_parms_p_t emitp)
{
    emit_attr_number(emitp->outbp,
	CDSCP_CLERKCH, "Cache Hits",
	emitp->attrp->attr_members[0]->value.av_val.av_counter);
}

/***********************************************************************/
void
emit_clerk_cache_bypass_count (
cprpc_emit_parms_p_t emitp)
{
    emit_attr_number(emitp->outbp,
	CDSCP_CLERKCB, "Cache Bypasses",
	emitp->attrp->attr_members[0]->value.av_val.av_counter);
}

/***********************************************************************/
void
emit_clerk_write_ops_count (
cprpc_emit_parms_p_t emitp)
{
    emit_attr_number(emitp->outbp,
	CDSCP_CLERKWO, "Write Operations",
	emitp->attrp->attr_members[0]->value.av_val.av_counter);
}

/***********************************************************************/
void
emit_clerk_misc_ops_count (
cprpc_emit_parms_p_t emitp)
{
    emit_attr_number(emitp->outbp,
	CDSCP_CLERKMO, "Miscellaneous Operations",
	emitp->attrp->attr_members[0]->value.av_val.av_counter);
}

/***********************************************************************/
INTERNAL void emit_cached_clh_name (
cprpc_emit_parms_p_t emitp)
{
    int status;
    char buf[256];
    int buflen=256;
    FullName_t Lname;

    R_to_L_FullName (&emitp->clhp->clh_members[0]->clh_replica_p->rp_chname,
                     &Lname,
                     0);    /* 0 -=> !freemem */
    status = dnsCvtCDSFullToStr ((dns_full_name_t *)&Lname,
                                 (unsigned char *)buf,
                                 &buflen);
    if (status != DNS_SUCCESS)
    {
        mgmt_error(status);
        return;
    }
                                   
    cprpc_emit_show_header (emitp->outbp,
	CDSCP_REFHDR_ENTLINE_CACLH, "CACHED CLEARINGHOUSE", buf);
}

/***********************************************************************/
INTERNAL void emit_cached_clh_towerset (
cprpc_emit_parms_p_t emitp)
{
    Set_t *const towerset_p = (Set_t *) dns_malloc (MAXATTRIBUTE);

    if (!towerset_p)
    {
	dce_svc_printf(CDSCP_NOMEM_MSG);
    }
    else
    {
	RPTowerUnpack (emitp->clhp->clh_members[0]->clh_replica_p->rp_towers,
		       towerset_p);

	emit_towerset_lines(emitp->outbp, towerset_p);

	dns_free (towerset_p);
    }
}

/***********************************************************************/
void
emit_cached_clh_info (
cprpc_emit_parms_p_t emitp)
{
    struct op_blk *const outbp = emitp->outbp;
    cprpc_attr_list_p_t attr_list_p;
    cprpc_attr_p_t      attr_p;
    int                 attr_id;
    int                 attr_num;
    int                 i;

    /*
     * sanity check and initialization
     */
    if (emitp->clhp->clh_count == 0) return;

    /*
     * emit the name and towerset for this clearinghouse
     */
    emit_cached_clh_name (emitp);
    emit_cached_clh_towerset (emitp);

    /*
     * emit the attributes for this cached clearinghouse
     */
    attr_list_p = emitp->clhp->clh_members[0]->attr_list_p;
    attr_num = attr_list_p->attr_len;
    for (i=0; i<attr_num; i++)
    {
        attr_p  = attr_list_p->attr_members[i];
        attr_id = attr_list_p->attr_members[i]->attr_id;
        switch (attr_id)
        {
            case REMOTE_CH_READ_OPS_COUNT:
            {
		emit_attr_number(emitp->outbp,
		    CDSCP_CACLHRO, "Read Operations",
		    attr_p->value.av_val.av_counter);
		break;
            }
            case REMOTE_CH_WRITE_OPS_COUNT:
            {
		emit_attr_number(emitp->outbp,
		    CDSCP_CACLHWO, "Write Operations",
		    attr_p->value.av_val.av_counter);
		break;
            }
            case REMOTE_CH_MISC_OPS_COUNT:
            {
		emit_attr_number(emitp->outbp,
		    CDSCP_CACLHMO, "Miscellaneous Operations",
		    attr_p->value.av_val.av_counter);
		break;
            }
            case REMOTE_CH_CREATE_TIME:
            {
		emit_time (outbp,
		    CDSCP_CACLHCT, "Creation Time", attr_p);
		break;
            }
            default:
            {
		dce_svc_printf(CDSCP_UNK_CA_CLE_ATT_MSG, attr_id);
                break;
            }
        }
    }
}

/***********************************************************************/
void
emit_cached_ns_info (
cprpc_emit_parms_p_t emitp)
{
    int status;
    char buf[256];
    int buflen;
    cds_visa_t *cds_visa_p = (cds_visa_t *)NULL;
    FullName_t Fname;
    int i;
    cprpc_attr_list_p_t attr_list_p;
    int attr_num, attr_id;
    attr_AtomicValue_t *attr_valp;

    /*
     * sanity check
     */
    if (emitp->nsp->ns_count == 0) return;

    /*
     * emit the attributes for this cached nameserver
     */
    attr_list_p = emitp->nsp->ns_members[0]->attr_list_p;
    attr_num = attr_list_p->attr_len;
    for (i=0; i<attr_num; i++)
    {
        attr_id   = attr_list_p->attr_members[i]->attr_id;
        attr_valp = &attr_list_p->attr_members[i]->value;
        switch (attr_id)
        {
            case CP_uuid:
            {
                /*
                 * emit the cached nameserver uuid
                 */
                break;
            }
            case CP_FullName:
            {
                /*
                 * emit the cached nameserver fullname
                 */
                cdsImport_FullName (cds_visa_p,
                                    attr_valp->av_val.av_fullname_p,
                                    &Fname,
                                    0);    /* 0 -=> !freemem */
                buflen = sizeof (buf);
                status = dnsCvtCDSFullToStr ((dns_full_name_t *)&Fname,
                                             (unsigned char *)buf,
                                             &buflen);
                if (status != DNS_SUCCESS)
                {
                    mgmt_error(status);
                    return;
                }
		emit_attr_string(emitp->outbp,
			CDSCP_CANSNAME, "Name", buf);
                break;
            }
            case CP_ReplicaPointer:
            {
                Set_t *const towerset_p = (Set_t *)dns_malloc(MAXATTRIBUTE);

                if (!towerset_p)
                {
                    dce_svc_printf(CDSCP_NOMEM_MSG);
                }
                else
                {
		    RPTowerUnpack (attr_valp->av_val.av_rp_p->rp_towers,
				   towerset_p);

		    /*
		     * emit the cached nameserver tower
		     */
		    emit_towerset_lines (emitp->outbp, towerset_p);

		    dns_free (towerset_p);
                }
                break;
            }
            default:
            {
		dce_svc_printf(CDSCP_UNK_CA_CLE_ATT_MSG, attr_id);
                break;
            }
        }
    }
}

/*
 *****************************************************************
 * Output a time
 */
INTERNAL void emit_time (
struct op_blk *outbp,	/* ptr to output block */
unsigned code,
char *txt,
cprpc_attr_t *attrp)	/* ptr to the (RPC) attribute */
{
    char valuebuf[100];

    format_utcabs_for_print(valuebuf,
			    (utc_t *)attrp->value.av_val.av_utctime);
    emit_attr_string(outbp, code, txt, valuebuf);
}
