/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscreatects.c,v $
 * Revision 1.1.6.2  1996/02/18  19:33:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:34  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:22:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:37  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:44  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:59:43  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:17  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:28  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:52:45  peckham
 * 	Remove unnecessary conditionals.
 * 	[1994/04/29  14:17:04  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:14  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:27:02  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:55:01  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:09:06  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnscreatects.c
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
#include <dce/dce.h>
#include <dcecdssvc.h>
#include <dce/cdsclerk.h>
#include <uc_parse.h>
#include <uc_clerk.h>     

/* ----------------------------------------------------------------------
 *  Create a timestamp
 * ----------------------------------------------------------------------
 */
int 
cdsCreateCTS (cds_cts_t *CTS_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    cdsFlagStat_t flags;
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsCreateCTS()"));

    /* Required by macros */
    ZERO_bytes(&flags, sizeof(struct cdsFlagStat));

    PRM_build(OP_Local, &flags);
      PRM_IN_byte(op_creatects);
      PRM_RET_Timestamp(CTS_p);
    PRM_send(status);

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsCreateCTS() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
