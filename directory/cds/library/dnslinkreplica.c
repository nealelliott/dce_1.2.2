/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnslinkreplica.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:20  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:24:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:26  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:19  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:01:50  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:55  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:05  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:15  peckham
 * 	Remove unnecessary conditionals.
 * 	[1994/04/29  14:24:07  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:11  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:30:44  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:57:46  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:13:02  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnslinkreplica.c
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
#include <uc_clerk.h>
#include <uc_parse.h>

/* ----------------------------------------------------------------------
 *  Link up a new replica with the other replicas of a directory
 * ----------------------------------------------------------------------
 */
int 
cdsLinkReplica (cds_full_name_t     *DirName_p,
                cds_full_name_t     *onBehalfOf_p,
                cds_id_t            *DirID_p,
                unsigned char       *Replica_p,
                int                 ReplicaLen,
                cds_id_t            *Epoch_p,
                cds_id_t            *NextRingID_p,
                cdsVersion_t        *ReplicaVerNbr_p,
                cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsLinkReplica()"));

    PRM_build(OP_LinkReplica, Flags_p);
      /* AuthenticationInfo record */
      PRM_begin();
      PRM_end();
      PRM_IN_FullName(DirName_p);
      PRM_IN_ObjUID(DirID_p);
      /* ReplicaPointer record */
      PRM_IN_bytes(Replica_p, ReplicaLen, 4000);
      PRM_OUT_ObjUID(Epoch_p);
      PRM_OUT_ObjUID(NextRingID_p);
      PRM_OUT_VersionNumber(ReplicaVerNbr_p);
    PRM_send(status);

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsLinkReplica() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
