/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsskulkdir.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:47  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:26:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:45  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:36  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:03:03  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:15  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:26  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:55:00  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	Fix DCE_SVC_DEBUG() call to designate library, not server.
 * 	[1994/04/29  14:26:53  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:41  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:32:43  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:15:46  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:14:59  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsskulkdir.c
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
 *  Initiate a skulk process on the given directory
 * ----------------------------------------------------------------------
 */
int 
cdsSkulkDir (cds_full_name_t     *DirName_p,
             cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    cds_full_name_t name;
    int status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >cdsSkulkDir()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE)
	if ((DirName_p = cds_fulltoopq(DirName_p, &name, Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsSkulkDir;
	}
    
    PRM_build(OP_Skulk, Flags_p);
    PRM_IN_FullName(DirName_p);
    PRM_send(status);

leave_cdsSkulkDir:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <cdsSkulkDir() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
