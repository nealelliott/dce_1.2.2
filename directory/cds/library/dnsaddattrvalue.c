/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsaddattrvalue.c,v $
 * Revision 1.1.7.2  1996/02/18  19:32:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:16  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:21:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:00  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:31  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:58:52  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:40:52  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:14  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:52:06  peckham
 * 	Remove VMS conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  13:59:59  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:05:53  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:25:25  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:53:54  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:07:21  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsaddattrvalue.c
 */
/*
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
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/cdsclerk.h>
#include <uc_parse.h>
#include <uc_clerk.h>     

/* ----------------------------------------------------------------------
 *  Add a value to an attribute
 * ----------------------------------------------------------------------
 */
int
cdsAddAttrValue (cds_full_name_t     *Name_p,
                 cds_attr_name_t     *AttrName_p,
                 cdsAttrType_t       AttrType,
                 cdsEntries_t        EntryType,
                 cds_attrvalue_t     *Value_p,
                 cds_cts_t           *CTS_p,
                 cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    cds_full_name_t entryname;
    cds_attr_name_t attrname;
    void	    *vptr;
    int		    vlen;
    int             status;
    unsigned char   vtype = cds_byte;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsAddAttrValue()"));

    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE) {
	Name_p = cds_fulltoopq(Name_p, &entryname, Flags_p);
	AttrName_p = cds_attrtoopq(AttrName_p, &attrname, Flags_p);

	if (!Name_p || !AttrName_p) {
	    status = CDS_ERROR;
	    goto leave_cdsAddAttrValue;
	}
    }

    status = dns_AttrVal2TLV(Value_p, &vtype, &vlen, &vptr);

    if (status != CDS_SUCCESS) {
	status = cds_status_clerk(Flags_p, status);
	goto leave_cdsAddAttrValue;
    }

    status = cdsIntAddAttrValue(Name_p, AttrName_p, AttrType, EntryType,
			vtype, (unsigned char *)vptr, vlen, CTS_p, Flags_p);

leave_cdsAddAttrValue:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsAddAttrValue() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * This internal interface is for the server who wants to pass opaque names and
 * attribute values as TLV triples with VT_* value types.
 */
int
cdsIntAddAttrValue (cds_full_name_t     *Name_p,
                    cds_attr_name_t     *AttrName_p,
                    cdsAttrType_t       AttrType,
                    cdsEntries_t        EntryType,
                    int                 ValueType,
                    unsigned char       *Value_p,
                    int                 ValueLen,
                    cds_cts_t           *CTS_p,
                    cdsFlagStat_t       *Flags_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    int status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" >cdsIntAddAttrValue()"));

    if (!CTS_p)
	CTS_p = (cds_cts_t *)NullTimestamp;

    /* Add an empty set */
    if (AttrType == cdsEmptySet)
	if (Value_p) {
	    status = cds_status_clerk(Flags_p, CDS_INVALIDARGUMENT);
	    goto leave_cdsIntAddAttrValue;
	} else
	    AttrType = cdsSet;

    PRM_build(OP_ModifyAttribute, Flags_p);
      PRM_IN_FullName(Name_p);
      PRM_IN_EntryType(EntryType);
      /* Update record */
      PRM_begin();
	PRM_byte(UD_present);	/* present */
	PRM_Timestamp(CTS_p);
	PRM_AttributeSpecifier(AttrType, AttrName_p);
        if (Value_p) {
           PRM_BOOLEAN(BL_true);
	   PRM_AtomicValue(ValueType, Value_p, ValueLen);
        } else 
           PRM_BOOLEAN(BL_false);
        PRM_end();
    PRM_send(status);

leave_cdsIntAddAttrValue:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsIntAddAttrValue() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
