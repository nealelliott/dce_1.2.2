/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsenumobj.c,v $
 * Revision 1.1.6.2  1996/02/18  19:33:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:01  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:23:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:09  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:03  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:53  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:39  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:50  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:53:37  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:18:31  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:49  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:29:08  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:56:28  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:11:14  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsenumobj.c
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

static byte_t allNames[] = {"\11\1*"};
static byte_t allClasses[] = {"\11\1*"};

/* ----------------------------------------------------------------------
 *  Return the next object for a given directory
 * ----------------------------------------------------------------------
 */
int 
cdsEnumObj (cds_full_name_t     *DirName_p,
            cds_handle_t        Handle_p,
            cds_simple_name_t   *ClassFilter_p,
            cds_simple_name_t   *NameFilter_p,
            int                 *ReturnClass_p,
            cds_simple_name_t   **Name_pp,
            unsigned char       *NameBuf_p,
            int                 *NameBufLen_p,
            cds_simple_name_t   **ClassName_pp,
            unsigned char       *ClassNameBuf_p,
            int                 *ClassNameBufLen_p,
            cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)Handle_p;
    cds_full_name_t name;
    cds_simple_name_t cfilter;
    cds_simple_name_t nfilter;
    int ReturnClass = BL_false;
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsEnumObj()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE) {
	if ((DirName_p = cds_fulltoopq(DirName_p, &name, Flags_p)) == NULL) {
	    status = CDS_ERROR;
	    goto leave_cdsEnumObj;
	}
	if (ClassFilter_p
	    && ((ClassFilter_p = cds_simpletoopq(ClassFilter_p, 1, &cfilter,
							Flags_p)) == NULL)) {
	    status = CDS_ERROR;
	    goto leave_cdsEnumObj;
	}
	if (NameFilter_p
	    && ((NameFilter_p = cds_simpletoopq(NameFilter_p, 1, &nfilter,
							Flags_p)) == NULL)) {
	    status = CDS_ERROR;
	    goto leave_cdsEnumObj;
	}
    }

    if ((status = cds_handleSetNamCls(Handle_p,
		Name_pp, NameBuf_p, NameBufLen_p,
		ReturnClass_p, ClassName_pp, ClassNameBuf_p, ClassNameBufLen_p,
		Flags_p)) != CDS_PENDING)
	goto leave_cdsEnumObj;

    if (!ClassFilter_p)
	ClassFilter_p = (cds_simple_name_t *)allClasses;

    if (!NameFilter_p)
	NameFilter_p = (cds_simple_name_t *)allNames;

    if (!ReturnClass_p)
	ReturnClass_p = &ReturnClass;
    else if (*ReturnClass_p)
	*ReturnClass_p = BL_true;

    PRM_build(OP_EnumerateObjects, Flags_p);
      PRM_handle(Handle_p);
      PRM_IN_FullName(DirName_p);
      PRM_IN_SimpleName(NameFilter_p);
      PRM_IN_AttributeName(ClassFilter_p);
      PRM_IN_SimpleName(rh_p->dns_Context_p);
      PRM_INOUT_BOOLEAN(*ReturnClass_p, &rh_p->dns_ReturnClass);
      PRM_OUT_Set(rh_p->dns_ReadBuf_p, MAX_MSG_SIZE);
      PRM_RET_BOOLEAN(&rh_p->dns_done);
    PRM_send(status);

leave_cdsEnumObj:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsEnumObj() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
