/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsrmattrvalue.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:41  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:25:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:40  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:31  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:02:45  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:11  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:21  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:54:47  peckham
 * 	Remove unnecessary conditionals.
 * 	Rename dns*() -> cds*().
 * 	[1994/04/29  14:26:08  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:07:32  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:32:13  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:58:51  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:14:29  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsrmattrvalue.c
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
 *  Remove a value from a set-valued attribute
 * ----------------------------------------------------------------------
 */
int 
cdsRmAttrValue (cds_full_name_t     *Name_p,
                cds_attr_name_t     *AttrName_p,
                cdsEntries_t        EntryType,
                cds_attrvalue_t     *Value_p,
                cds_cts_t           *CTS_p,
                cdsFlagStat_t       *Flags_p)
{
    void           *vptr;
    int            vlen;
    int		   status;
    cds_full_name_t  name;
    cds_attr_name_t  attr;
    unsigned char  vtype = dns_byte;
    if (dns_validate_NameType(Flags_p->fsNameType) == cdsStrDCE) {
	if ((Name_p = cds_fulltoopq(Name_p, &name, Flags_p)) == NULL)
	    return(CDS_ERROR);
	if ((AttrName_p = cds_attrtoopq(AttrName_p, &attr, Flags_p)) == NULL)
	    return(CDS_ERROR);
    }
    status = dns_AttrVal2TLV(Value_p, &vtype, &vlen, &vptr);
    if (status != CDS_SUCCESS)
	return(status);

    return (cdsIntRmAttrValue(Name_p, AttrName_p, EntryType,
			vtype, (unsigned char *)vptr, vlen, CTS_p, Flags_p));
}

/*
 * Internal call for the server who calls with attribute
 * values as explicit TLV triples with VT_* value types
 */
int 
cdsIntRmAttrValue (cds_full_name_t     *Name_p,
                   cds_attr_name_t     *AttrName_p,
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
	" >cdsIntRmAttrValue()"));

    if (!CTS_p)
	CTS_p = (cds_cts_t *)NullTimestamp;

    PRM_build(OP_ModifyAttribute, Flags_p);
      PRM_IN_FullName(Name_p);
      PRM_IN_EntryType(EntryType);
      /* Update record */
      PRM_begin();
	PRM_byte(UD_absent);	/* absent */
	PRM_Timestamp(CTS_p);
	PRM_AttributeSpecifier(cdsSet, AttrName_p);
	PRM_BOOLEAN(BL_true);
	PRM_AtomicValue(ValueType, Value_p, ValueLen);
	PRM_end();
    PRM_send(status);

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug3,
	" <cdsIntRmAttrValue() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}




