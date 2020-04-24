/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsdelchild.c,v $
 * Revision 1.1.6.2  1996/02/18  19:33:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:49  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:23:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:59  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:55  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:26  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:27  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:40  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:53:13  peckham
 * 	Remove unnecessary conditionals.
 * 	[1994/04/29  14:17:50  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:34  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:28:11  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:55:45  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:10:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsdelchild.c
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
#include <dce/cdsclerk.h>
#include <dcecdssvc.h>
#include <uc_parse.h>
#include <uc_clerk.h>     

/* ----------------------------------------------------------------------
 *  Delete an child pointer entry
 * ----------------------------------------------------------------------
 */
int 
cdsDelChild (cds_full_name_t     *Name_p,
             cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsDelChild()"));

    PRM_build(OP_DeleteChildEntry, Flags_p);
      PRM_IN_FullName(Name_p);
    PRM_send(status);

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsDelChild() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
