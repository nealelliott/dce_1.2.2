/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsreadattrset.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:32  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:25:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:35  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:26  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:02:28  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:02  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:15  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:36  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:25:41  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:24  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:31:38  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:58:25  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:13:54  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:19:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsreadattrset.c
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
 */
  
#include <dce/dce.h>
#include <dce/cdsclerk.h>
#include <dcecdssvc.h>
#include <uc_parse.h>
#include <uc_clerk.h>     


/* ----------------------------------------------------------------------
 *  Read attribute value from object, group, etc.
 * ----------------------------------------------------------------------
 */
int 
cdsReadAttrSet (cds_full_name_t     *Name_p,
                unsigned char       *LastSet_p,
                cds_simple_name_t   *AttrName_p,
                cdsEntries_t        EntryType,
                cds_full_name_t     *Clearinghouse_p,
                unsigned char       *ValueBuf_p,
                int                 ValueBufLen,
                unsigned char       *More_p,
                cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    Timestamp_t *context_p;
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsReadAttrSet()"));

    if (LastSet_p) {
	register byte_t *next_p = LastSet_p;
	register int num_offsets = 0;

	switch (*next_p++) {		/* check AttributeType */
	case AT_set:
	    /*
	     * Insure that attributes are present
	     *
	     *
	     * Pick up number of entries in the set
	     * and make sure non-empty
	     */
	    if (FLAG_CLEAR(((Set_t *)next_p)->st_flag, ST_present))
		goto set_empty;
	    if ((num_offsets = NUM_Set(next_p)) == 0)
		goto set_empty;

	    /*
	     * index to the last SetMember
	     */
	    --num_offsets;
	    next_p = INDEX_Set(next_p, num_offsets);

	    /* fall through with next_p = SetMember */
	case AT_single:
	    context_p = (Timestamp_t *)((MemberHeader_t *)next_p)->mh_timestamp;
	    break;
	case AT_null:

set_empty:
	    status = cds_status_clerk(Flags_p, CDS_EMPTY);
	    goto leave_cdsReadAttrSet;
	default:
	    status = cds_status_clerk(Flags_p, CDS_CLERKBUG);
	    goto leave_cdsReadAttrSet;
	}
    } else
	context_p = NullTimestamp;

    if (!Clearinghouse_p)
	Clearinghouse_p = (cds_full_name_t *)NullFullName;

    PRM_build(OP_ReadAttribute, Flags_p);
      PRM_IN_FullName(Name_p);
      PRM_IN_EntryType(EntryType);
      PRM_IN_SimpleName(AttrName_p);
      PRM_IN_fsMaybeMore(Flags_p->fsMaybeMore);
      PRM_IN_Timestamp(context_p);
      PRM_IN_FullName_Null(Clearinghouse_p);
      PRM_OUT_Set(ValueBuf_p, ValueBufLen);
      PRM_RET_BOOLEAN(More_p);
    PRM_send(status);

leave_cdsReadAttrSet:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsReadAttrSet() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}





