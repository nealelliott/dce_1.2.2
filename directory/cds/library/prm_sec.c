/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: prm_sec.c,v $
 * Revision 1.1.6.2  1996/02/18  19:35:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:08  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:27:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:27  root]
 * 
 * Revision 1.1.4.5  1994/08/03  19:03:11  mccann
 * 	includes file cleanup
 * 	[1994/07/21  13:00:38  mccann]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:51  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:04:12  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:27  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:39  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:55:28  peckham
 * 	Change dns* to cds*
 * 	[1994/04/29  14:34:16  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:08:37  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:34:34  peckham]
 * 
 * Revision 1.1.2.3  1993/02/10  20:15:04  keegan
 * 	If sec_login_export_context returns any error except 'sec_login_s_no_memory',
 * 	return an error.
 * 	[1993/02/10  19:51:41  keegan]
 * 
 * Revision 1.1.2.2  1992/12/30  14:17:03  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:16:47  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 * MODULE: prm_sec.c 
 * FUNCTION: Used only for DCE with security defined via 'DCE_SEC'.
 * called by macro 'PRM_build'
 * 
 */

#if defined(DCE_SEC)

#include <dce/dce.h>
#include <dce/dcecdssvc.h>
#include <dce/cdsclerk.h>
#include <dce/assert.h>
#include <dce/sec_login.h>
#include <dce/rpc.h>
#include <dce/dns_record.h>
#include <uc_parse.h>

#define IN
#define OUT
#define INOUT


static int 
dns_status_dce (
    cdsFlagStat_t       *,
    error_status_t      );


/* Put login context, if not done already.
 * Returns length of context data.
 */
int 
prm_login_context (sec_login_handle_t  *login_context_h_p,
                   cdsFlagStat_t       *flgs,
                   byte_t              *ucp_ptr,
                   int                 export_length)
{ 
    if (!flgs->fsUnAuth) {
	unsigned32 len_used, len_needed; 
	error_status_t sec_status;

	if (!ucp_ptr) {
	    /* 
	     * If authenticated access is requested,
	     * get/export the current context.
	     */
	    sec_login_get_current_context(login_context_h_p, &sec_status);
	    if (sec_status != rpc_s_ok) {
		dns_status_dce(flgs, sec_status);
		return(-1);
	    }
	}
	/* If we authenticated access
	 *  is requested, insert login context @ucp_ptr.
	 */
	sec_login_export_context(*login_context_h_p,
				 export_length,
				 ucp_ptr,
				 &len_used,
				 &len_needed,
				 &sec_status);
	if (ucp_ptr) {
	    if (sec_status != rpc_s_ok) {
		dns_status_dce(flgs, sec_status);
		return(-1);
	    }
	    dce_assert(cds__svc_handle,len_used == len_needed);
	    dce_assert(cds__svc_handle,len_needed == export_length);
	} else if (sec_status != sec_login_s_no_memory) {
	  /*
	   * We expect an error status of 'sec_login_s_no_memory'.
	   * If it isn't,
	   * punt now lest we waste our time marshalling or get confused
	   * by a bogus len_needed field.
	   * (courtesy of: 8 Feb 1993 sommerfeld@apollo.hp.com)
	   */
	  dns_status_dce(flgs, sec_status);
	  return(-1);
	}
	return(len_needed);
    } else
	return(0);
}

static int 
dns_status_dce (cdsFlagStat_t       *Flags_p,
                error_status_t      status)
{
    if (Flags_p) {
        Flags_p->fsCDSStat = CDS_NOCOMMUNICATION - CDS_NAMESERVERERRORS;
        Flags_p->fsLocStat = status;
    }
    return(CDS_ERROR);
}

#endif
