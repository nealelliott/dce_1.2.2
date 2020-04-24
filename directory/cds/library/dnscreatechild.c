/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscreatechild.c,v $
 * Revision 1.1.7.2  1996/02/18  19:33:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:33  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:22:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:35  root]
 * 
 * Revision 1.1.4.5  1994/08/16  20:03:03  jd
 * 	11673 Fromn shu@hp. Fix typo in dnsCreateChild.
 * 	[1994/08/13  19:21:51  jd]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:43  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:59:39  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:15  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:27  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:52:42  peckham
 * 	Remove unnecessary conditionals.
 * 	[1994/04/29  14:17:00  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:13  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:26:56  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:54:56  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:08:54  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnscreatechild.c
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
 *
 */
  
#include <dce/dce.h>
#include <dcecdssvc.h>
#include <dce/cdsclerk.h>
#include <uc_parse.h>
#include <uc_clerk.h>
#include <dce/id_base.h>


/* ----------------------------------------------------------------------
 *  Create a child pointer entry
 * ----------------------------------------------------------------------
 */
int 
cdsCreateChild (cds_full_name_t     *ChildName_p,
                sec_id_foreign_t    *OnBehalfOf_p,
                cds_id_t            *ChildID_p,
                cds_full_name_t     *ChildReplicas_p,
                int                 ChildReplicasLen,
                cds_id_t            *ParentID_p,
                cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsCreateChild()"));

    PRM_build(OP_CreateChildEntry, Flags_p);
      PRM_IN_FullName(ChildName_p);
      /* AuthenticationInfo record */
      PRM_begin();
      if (OnBehalfOf_p != (sec_id_foreign_t *)NULL) {
        PRM_bytes(&OnBehalfOf_p->id.uuid, sizeof(OnBehalfOf_p->id.uuid),
		 sizeof(OnBehalfOf_p->id.uuid));
	if (OnBehalfOf_p->id.name) {
	  PRM_bytes((char *)OnBehalfOf_p->id.name,
		  strlen((char *)OnBehalfOf_p->id.name)+1, CDS_STR_FNAME_MAX);
	} else
	  PRM_byte(0);

        PRM_bytes(&OnBehalfOf_p->realm.uuid, sizeof(OnBehalfOf_p->realm.uuid),
		 sizeof(OnBehalfOf_p->realm.uuid));
	if (OnBehalfOf_p->realm.name) {
	    PRM_bytes((char *)OnBehalfOf_p->realm.name, 
	       strlen((char *)OnBehalfOf_p->realm.name)+1, CDS_STR_FNAME_MAX);
	} else
	  PRM_byte(0);
      }
      PRM_end();
      PRM_IN_ObjUID(ChildID_p);
      /* Set of ReplicaPointer */
      PRM_IN_bytes(ChildReplicas_p, ChildReplicasLen, 4000);
      PRM_OUT_ObjUID(ParentID_p);
    PRM_send(status);

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsCreateChild() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}


