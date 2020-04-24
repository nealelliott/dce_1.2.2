/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsaddreplica.c,v $
 * Revision 1.1.6.2  1996/02/18  19:32:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:18  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:21:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:05  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:33  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:58:59  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:40:55  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:16  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:52:11  peckham
 * 	Remove VMS conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:00:07  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:05:56  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:25:38  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:54:03  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:07:30  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsaddreplica.c
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
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/cdsclerk.h>
#include <uc_parse.h>
#include <uc_clerk.h>


/* ----------------------------------------------------------------------
 *  Add a copy of a directory to a specified clearinghouse
 * ----------------------------------------------------------------------
 */
int
cdsAddReplica (cds_full_name_t      *DirName_p,
               cds_full_name_t      *ClearinghouseName_p,
               cdsReplicaType_t     ReplicaType,
               cdsFlagStat_t        *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    cds_full_name_t dirname;
    cds_full_name_t chname;
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsAddReplica()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE) {
	DirName_p = cds_fulltoopq(DirName_p, &dirname, Flags_p);
	ClearinghouseName_p = cds_fulltoopq(ClearinghouseName_p, &chname, Flags_p);

	if (!DirName_p || !ClearinghouseName_p) {
	    status = CDS_ERROR;
	    goto leave_cdsAddReplica;
	}
    }

    PRM_build(OP_AddReplica, Flags_p);
      PRM_IN_FullName(DirName_p);
      PRM_IN_FullName(ClearinghouseName_p);
      PRM_IN_ReplicaType(ReplicaType);
    PRM_send(status);

leave_cdsAddReplica:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsAddReplica() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}





