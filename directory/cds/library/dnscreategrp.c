/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscreategrp.c,v $
 * Revision 1.1.8.2  1996/02/18  19:33:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:37  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:22:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:41  root]
 * 
 * Revision 1.1.6.4  1994/06/30  19:16:45  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:59:50  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:41:19  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:30  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/29  15:52:52  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:17:12  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:06:18  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:27:13  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:55:10  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:09:16  zeliff]
 * 
 * Revision 1.1.2.3  1992/07/06  20:14:57  mfox
 * 	Limit classnames to 31 characters to prevent server crash
 * 	[1992/07/06  20:11:47  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:18:57  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:31:15  weisman]
 * 
 * Revision 1.1  1992/01/19  15:17:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnscreategrp.c
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
#include <uc_clerk.h>
#include <uc_parse.h>

static byte_t class[] = {"\10\11CDS_Group"};
static unsigned char classVersion[2] = {2, 0};

/* ----------------------------------------------------------------------
 *  Create a group entry in the namespace
 * ----------------------------------------------------------------------
 */
int 
cdsCreateGrp (cds_full_name_t     *Name_p,
              cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    cds_full_name_t name;
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsCreateGrp()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE)
	if ((Name_p = cds_fulltoopq(Name_p, &name, Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsCreateGrp;
	}

    PRM_build(OP_CreateObject, Flags_p);
      PRM_IN_FullName(Name_p);
      PRM_IN_ClassName(class);
      PRM_IN_ClassVersion(classVersion);
      PRM_IN_OPT_ObjUID(NULL);
      PRM_RET_Timestamp(NULL);
    PRM_send(status);

leave_cdsCreateGrp:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsCreateGrp() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
