/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cprpc_emit_server.c,v $
 * Revision 1.1.8.2  1996/02/18  19:27:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:06  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:07:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:38  root]
 * 
 * Revision 1.1.6.5  1994/08/03  19:00:21  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:38:19  mccann]
 * 
 * Revision 1.1.6.4  1994/06/09  18:36:23  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:03  devsrc]
 * 
 * Revision 1.1.6.3  1994/04/19  17:05:27  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:30:24  mccann]
 * 
 * Revision 1.1.6.2  1994/04/04  20:05:58  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:05:20  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:57:48  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:57:04  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:01:53  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:31:43  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/26  07:10:17  jim
 * 	Added include msgblk.h. This is not getting included on AIX 3.2
 * 	for some reason.
 * 	[1992/05/29  18:48:26  jim]
 * 
 * Revision 1.1.2.2  1992/03/22  21:42:51  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:20:12  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: cprpc_emit_server.c
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
 * This module contains the (many) server emit routines that are used to
 * display server management information.  This module is part of the
 * control program executable.
 */
#include <cdscp.h>
#include <sys/time.h>
#include <dce/utc.h>
#include <dce/rpc_cvt.h>
#include <dce/server_ncl_defs.h>
#include <dce/cprpcp.h>
#include <dce/sys_time.h>

/*
 *****************************************************************************
 * Forward declarations for internal (static) functions
 */
INTERNAL void emit_counter64
    (struct op_blk *,
     unsigned,
     char          *,
     cprpc_attr_t  *);

INTERNAL void emit_time
    (struct op_blk *,
     unsigned,
     char          *,
     cprpc_attr_t  *);

INTERNAL void emit_time_quad
    (struct op_blk *,
     unsigned,
     char          *,
     cprpc_attr_t  *);

/***********************************************************************/
void emit_server_create_time (
cprpc_emit_parms_p_t emitp)
{
    emit_time (emitp->outbp,
	CDSCP_SERVERCTS, "Creation Time", emitp->attrp->attr_members[0]);
}

/***********************************************************************/
void emit_server_future_skew (
cprpc_emit_parms_p_t emitp)
{
    emit_time_quad (emitp->outbp,
	CDSCP_SERVERFST, "Future Skew Time", emitp->attrp->attr_members[0]);
}

/***********************************************************************/
void emit_server_known_clearinghouses (
cprpc_emit_parms_p_t emitp)
{
    int i;
    int clh_count;
    char buf[256];
    int buflen;
    int status;
    FullName_t Lname;

    clh_count = emitp->clhp->clh_count;
    if (!clh_count) return;

    for (i=0; i<clh_count; i++)
    {
        R_to_L_FullName (emitp->clhp->clh_members[i]->clh_name_p,
                         &Lname,
                         0);    /* 0 -=> !freemem */
        buflen = sizeof (buf);
        status =
            dnsCvtCDSFullToStr ((dns_full_name_t *)&Lname,
                                (unsigned char *)buf,
                                &buflen);
        if (status != DNS_SUCCESS)
        {
            mgmt_error(status);
            continue;
        }

	if (i)
		emit_name_string(emitp->outbp, "", buf);
	else
		emit_attr_string(emitp->outbp,
			CDSCP_SERVERCLH_0, "Known Clearinghouses", buf);
    }
}

/***********************************************************************/
void emit_server_read_access_count (
cprpc_emit_parms_p_t emitp)
{
    emit_counter64 (emitp->outbp,
	CDSCP_SERVERRO, "Read Operations",
	emitp->attrp->attr_members[0]);
}

/***********************************************************************/
void emit_server_write_access_count (
cprpc_emit_parms_p_t emitp)
{
    emit_counter64 (emitp->outbp,
	CDSCP_SERVERWO, "Write Operations",
	emitp->attrp->attr_members[0]);
}

/***********************************************************************/
void emit_server_skulks_initiated_count (
cprpc_emit_parms_p_t emitp)
{
    emit_counter64 (emitp->outbp,
	CDSCP_SERVERSKI, "Skulks Initiated",
	emitp->attrp->attr_members[0]);
}

/***********************************************************************/
void emit_server_skulks_completed_count (
cprpc_emit_parms_p_t emitp)
{
    emit_counter64 (emitp->outbp,
	CDSCP_SERVERSKC, "Skulks Completed",
	emitp->attrp->attr_members[0]);
}

/***********************************************************************/
void emit_server_tree_broken_count (
cprpc_emit_parms_p_t emitp)
{
    emit_counter64 (emitp->outbp,
	CDSCP_SERVERTB, "Times Lookup Paths Broken",
	emitp->attrp->attr_members[0]);
}

/***********************************************************************/
void emit_server_crucial_replica_count (
cprpc_emit_parms_p_t emitp)
{
    emit_counter64 (emitp->outbp,
	CDSCP_SERVERCR, "Crucial Replicas",
	emitp->attrp->attr_members[0]);
}

/***********************************************************************/
void emit_server_child_pointer_count (
cprpc_emit_parms_p_t emitp)
{
    emit_counter64 (emitp->outbp,
	CDSCP_SERVERCP, "Child Update Failures",
	emitp->attrp->attr_members[0]);
}

/***********************************************************************/
void emit_server_security_count (
cprpc_emit_parms_p_t emitp)
{
    emit_counter64 (emitp->outbp,
	CDSCP_SERVERSC, "Security Failures",
	emitp->attrp->attr_members[0]);
}

/*
 *****************************************************************
 * Output a time
 */
INTERNAL void emit_time (
struct op_blk *outbp,
unsigned      code,
char          *txt,
cprpc_attr_t  *attrp)
{
    char valuebuf[100];

    format_utcabs_for_print(valuebuf,
			    (utc_t *)attrp->value.av_val.av_utctime);
    emit_attr_string(outbp, code, txt, valuebuf);
}

/*******************************************************************/
void emit_clearinghouse_counters (
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
     * emit the counters for this clearinghouse
     */
    attr_list_p = emitp->clhp->clh_members[0]->attr_list_p;
    attr_num = attr_list_p->attr_len;
    for (i=0; i<attr_num; i++)
    {
        attr_p  = attr_list_p->attr_members[i];
        attr_id = attr_list_p->attr_members[i]->attr_id;
        switch (attr_id)
        {
            case CH_READ_ACCESS_COUNT:
            {
                emit_counter64 (outbp,
			CDSCP_SERVERCLHRC, "Read Accesses", attr_p);
                break;
            }
            case CH_WRITE_ACCESS_COUNT:
            {
                emit_counter64 (outbp,
			CDSCP_SERVERCLHWC, "Write Accesses", attr_p);
                break;
            }
            case CH_REFERENCES_RETURNED_COUNT:
            {
                emit_counter64 (outbp,
			CDSCP_SERVERCLHRRC, "References Returned", attr_p);
                break;
            }
            case CH_ROOT_NOT_REACHABLE_COUNT:
            {
                emit_counter64 (outbp,
			CDSCP_SERVERCLHRNR, "Root Not Reachable Count", attr_p);
                break;
            }
            case CH_DATA_CORRUPTION_COUNT:
            {
                emit_counter64 (outbp,
			CDSCP_SERVERCLHDCC, "Data Corruption Count", attr_p);
                break;
            }
            case CH_SKULK_FAILURES_COUNT:
            {
                emit_counter64 (outbp,
			CDSCP_SERVERCLHSFC, "Skulk Failures", attr_p);
                break;
            }
            case CH_ENTRY_MISSING_COUNT:
            {
                emit_counter64 (outbp,
			CDSCP_SERVERCLHEMC, "Entry Missing Count", attr_p);
                break;
            }
            case CH_UPGRADES_FAILED_COUNT:
            {
                emit_counter64 (outbp,
			CDSCP_SERVERCLHUFC, "Upgrades Failed Count", attr_p);
                break;
            }
            case CH_ENABLE_COUNT:
            {
                emit_counter64 (outbp,
			CDSCP_SERVERCLHEC, "Enables\n", attr_p);
                break;
            }
            case CH_DISABLE_COUNT:
            {
                emit_counter64 (outbp,
			CDSCP_SERVERCLHDC, "Disables", attr_p);
                break;
            }
            default:
            {
		dce_svc_printf(CDSCP_UNK_CLE_CTR_MSG, attr_id);
                break;
            }
        }
    }
}

/*
 ********************************************************************
 * Output a 64-bit server counter
 *
 * Note: I'm not really pleased with this implementation but ...
 *       ... for now, since I don't have available to me a 64-bit
 *       arithmetic package or a 64-bit integer display routine,
 *       I'm going to display the counter in decimal if the hi-word
 *       is zero, otherwise, I'll display the whole thing in hex.
 */
INTERNAL void emit_counter64 (
struct op_blk *outbp,
unsigned      code,
char          *txt,
cprpc_attr_t  *attrp)
{
    char valuebuf[64];

#if (LONG_BIT == 64)
    sprintf (valuebuf, "%ld", attrp->value.av_val.av_counter64);
#else
    if (attrp->value.av_val.av_counter64.high == 0)
    {
        sprintf (valuebuf, "%d",
                 attrp->value.av_val.av_counter64.low);
    }
    else
    {
        sprintf (valuebuf, "0x%0lx%0lx",
                 attrp->value.av_val.av_counter64.high,
                 attrp->value.av_val.av_counter64.low);
    }
#endif
    emit_attr_string(outbp, code, txt, valuebuf);
}

/*
 ********************************************************************
 * Output a server time_quad
 */
INTERNAL void emit_time_quad (
struct op_blk *outbp,
unsigned      code,
char          *txt,
cprpc_attr_t  *attrp)
{
    static int ticks_per_second = 10*1000*1000;
    int seconds, nsecs;

    dns_ediv (&ticks_per_second,
              (time_quad_t *)&attrp->value.av_val.av_time_quad,
              &seconds, &nsecs);

    emit_attr_number(outbp, code, txt, seconds);
}
