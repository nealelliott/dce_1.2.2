/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsreadreplica.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:37  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:25:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:38  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:29  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:02:35  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:05  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:18  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:41  peckham
 * 	Remove unnecessary conditionals.
 * 	[1994/04/29  14:25:55  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:28  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:31:56  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:58:38  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:14:15  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsreadreplica.c
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
 *  Read Replica
 * ----------------------------------------------------------------------
 */
int 
cdsReadReplica (cds_full_name_t     *Name_p,
                cds_id_t            *Dirid_p,
                cds_handle_t        Handle_p,
                cds_attr_name_t     *AttrName_p,
                cds_full_name_t     *Clearinghouse_p,
                unsigned char       *ValueType_p,
                unsigned char       **Value_pp,
                int                 *ValueLen_p,
                cds_cts_t           **CTS_pp,
                unsigned char       *AddrBuf_p,
                int                 *AddrBufLen_p,
                cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsReadReplica()"));

    if ((status = cds_handleReplCon(Handle_p, ValueType_p, Value_pp,
		ValueLen_p, CTS_pp, Flags_p)) != CDS_PENDING)
	goto leave_cdsReadReplica;

    /* 
     * This packet must look like a readattribute.  Any differences
     * should be at the end.  This supports the v2.0 readattribute
     * which does the read-replica call .
     */
    PRM_build(OP_ReadReplica, Flags_p);
      PRM_handle(Handle_p);
      PRM_IN_FullName(Name_p);
      PRM_IN_EntryType(cdsDir);
      PRM_IN_AttributeName(AttrName_p);
      PRM_IN_fsMaybeMore(Flags_p->fsMaybeMore);
      PRM_IN_Timestamp(rh_p->dns_Context_p);
      PRM_IN_FullName(Clearinghouse_p);
      PRM_OUT_Set(rh_p->dns_ReadBuf_p, MAX_MSG_SIZE);
      PRM_RET_BOOLEAN(&rh_p->dns_done);
      PRM_IN_ObjUID(Dirid_p);
      if (AddrBuf_p) {
	PRM_OUT_Set(AddrBuf_p, *AddrBufLen_p);
      } else {
	PRM_OUT_Set(AddrBuf_p, 0);
      }
      PRM_OUT_bytes(AddrBufLen_p, sizeof(int));
    PRM_send(status);

leave_cdsReadReplica:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsReadReplica() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
