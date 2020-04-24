/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnstestattrvalue.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:52  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:26:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:58  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:40  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:03:15  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:20  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:30  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:55:10  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:28:00  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:56  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:33:13  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:16:11  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:15:25  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnstestattrvalue.c
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
 *  Check a value of an attribute
 * ----------------------------------------------------------------------
 */
int 
cdsTestAttrValue (cds_full_name_t     *Name_p,
                  cds_attr_name_t     *AttrName_p,
                  cdsEntries_t        EntryType,
                  cds_attrvalue_t     *Value_p,
                  cdsFlagStat_t       *Flags_p)
{
    void           *vptr;
    int            vlen;
    unsigned char  vtype = dns_byte;
    int		   status;
    cds_full_name_t  name;
    cds_attr_name_t  attr;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >cdsTestAttrValue"));

    status = dns_AttrVal2TLV(Value_p, &vtype, &vlen, &vptr);
    if (status != CDS_SUCCESS)
    {
	DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_library,
		svc_c_debug2,
		" <cdsTestAttrValue: returning return value from cds_status_clerk"));

	return(cds_status_clerk(Flags_p, status));
    }
    

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE) 
    {
	if ((Name_p = cds_fulltoopq(Name_p, &name, Flags_p)) == NULL)
	{  
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_library,
		svc_c_debug2,
		" <cdsTestAttrValue: (1) return(CDS_ERROR)"));
	    
	    return(CDS_ERROR);
	}
	
	if ((AttrName_p = cds_attrtoopq(AttrName_p, &attr, Flags_p)) == NULL)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_library,
		svc_c_debug2,
		" <cdsTestAttrValue: (2) return(CDS_ERROR)"));

	    return(CDS_ERROR);
	}
	
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <cdsTestAttrValue: returning return value from cdsIntTestAttrValue"));

    return (cdsIntTestAttrValue (Name_p, AttrName_p, EntryType,
    				 vtype, (unsigned char *)vptr, vlen, Flags_p));
}

/*
 * This internal interface is for the server who wants to pass
 * attribute values as TLV triples with VT_* value types.
 */
int 
cdsIntTestAttrValue (cds_full_name_t     *Name_p,
                     cds_attr_name_t     *AttrName_p,
                     cdsEntries_t        EntryType,
                     int                 ValueType,
                     unsigned char       *Value_p,
                     int                 ValueLen,
                     cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    int status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >cdsIntTestAttrValue()"));

    PRM_build(OP_TestAttribute, Flags_p);
      PRM_IN_FullName(Name_p);
      PRM_IN_EntryType(EntryType);
      PRM_IN_AttributeName(AttrName_p);
      PRM_IN_AtomicValue(ValueType, Value_p, ValueLen);
      PRM_RET_BOOLEAN(0);
    PRM_send(status);

   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <cdsIntTestAttrValue() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
