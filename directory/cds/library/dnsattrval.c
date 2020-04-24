/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsattrval.c,v $
 * Revision 1.1.9.2  1996/02/18  19:32:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:21  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:21:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:10  root]
 * 
 * Revision 1.1.7.4  1994/06/30  19:16:34  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:59:03  mccann]
 * 
 * Revision 1.1.7.3  1994/06/09  18:40:58  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:18  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/29  15:52:15  peckham
 * 	Rename dns*() to cds*().
 * 	[1994/04/29  14:02:25  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:05:59  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:25:51  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  13:54:13  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:07:54  zeliff]
 * 
 * Revision 1.1.5.2  1992/12/08  15:09:55  mfox
 * 	Byte comparison assumed signed char -- doesn't work on AIX
 * 	[1992/12/08  15:09:14  mfox]
 * 
 * Revision 1.1  1992/01/19  15:17:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsattrval.c
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
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/cdsclerk.h>
#include <uc_parse.h>
#include <uc_clerk.h>     
/*
 * Name:	dns_AttrVal2TLV
 *
 * Purpose:	Extracts the type, len and value from a cds_attrvalue_t
 *
 * In:		v - an attribute value, ie a discriminated union
 *
 * Out:		type - the union discriminator
 *		len - the byte length of the value
 *		ptr - ptr to the value
 *
 * Return:	none
 */

int 
dns_AttrVal2TLV (cds_attrvalue_t  *attr,
                 unsigned char    *type,
                 int              *len,
                 void             **ptr)
{
  cds_attrvalue_t v;
  static int lengths [] = {
    0,		/* cds_none */
    sizeof(v.val_u.val_long),
    sizeof(v.val_u.val_short),
    sizeof(v.val_u.val_small),
    sizeof(v.val_u.val_uuid),
    sizeof(v.val_u.val_timestamp),
    sizeof(v.val_u.val_timeout),
    sizeof(v.val_u.val_version)
    };

  if ((*type = dns_validate_Syntax(attr->val_syntax)) == (unsigned char) -1)
    return(CDS_INVALIDARGUMENT);

  switch (attr->val_syntax) {
  case cds_char:
    *len = attr->val_u.val_char.length;
    *ptr = attr->val_u.val_char.char_p;
    break;
  case cds_FullName:
  case cds_byte:
  case cds_GroupMember:
  case cds_ReplicaPointer:
  case cds_CHDirectory:
  case cds_ASN1:
  case cds_ParentPointer:
  case cds_gdaPointer:
    *len = attr->val_u.val_byte.length;
    *ptr = attr->val_u.val_byte.byte_p;
    break;
  default:
    *len = lengths[attr->val_syntax];
    *ptr = &attr->val_u;
  }

  return(CDS_SUCCESS);
}
/*
 * Name:	dns_TLV2AttrVal
 *
 * Purpose:	Composes a cds_attrvalue_t from its type, len, and value
 *
 * In:		type - the union discriminator
 *		len - the byte length of the value
 *		ptr - ptr to the value
 *
 * Out:		v - an attribute value, ie a discriminated union
 *
 * Return:	none
 */

void 
dns_TLV2AttrVal (int              type,
                 int              len,
                 void             *ptr,
                 cds_attrvalue_t  *attr)
{
  switch (type) {
  case VT_long:
    attr->val_syntax = cds_long;
    attr->val_u.val_long = EXT32(ptr);
    break;
  case VT_short:
    attr->val_syntax = cds_short;
    attr->val_u.val_short = EXT16(ptr);
    break;
  case VT_small:
    attr->val_syntax = cds_small;
    attr->val_u.val_small = EXT8(ptr);
    break;
  case VT_uuid:
    attr->val_syntax = cds_uuid;
    COPY_bytes(ptr, &attr->val_u.val_uuid, 
	       sizeof(attr->val_u.val_uuid));
    break;
  case VT_Timestamp:
    attr->val_syntax = cds_Timestamp;
    COPY_Timestamp(ptr, &attr->val_u.val_timestamp);
    break;
  case VT_Timeout:
    attr->val_syntax = cds_Timeout;
    COPY_Timeout(ptr, &attr->val_u.val_timeout);
    break;
  case VT_Version:
    attr->val_syntax = cds_Version;
    COPY_VersionNumber(ptr, &attr->val_u.val_version);
    break;
  case VT_char:
    attr->val_syntax = cds_char;
    attr->val_u.val_char.length = len;
    attr->val_u.val_char.char_p = (char *)ptr;
    break;
  case VT_ReplicaPointer:
    attr->val_syntax = cds_ReplicaPointer;
    attr->val_u.val_byte.length = len;
    attr->val_u.val_byte.byte_p = (char *)ptr;
    break;
  case VT_GroupMember:
    attr->val_syntax = cds_GroupMember;
    attr->val_u.val_byte.length = len;
    attr->val_u.val_byte.byte_p = (char *)ptr;
    break;
  case VT_ParentPointer:
    attr->val_syntax = cds_ParentPointer;
    attr->val_u.val_byte.length = len;
    attr->val_u.val_byte.byte_p = (char *)ptr;
    break;
  case VT_gdaPointer:
    attr->val_syntax = cds_gdaPointer;
    attr->val_u.val_byte.length = len;
    attr->val_u.val_byte.byte_p = (char *)ptr;
    break;
  case VT_FullName:
    attr->val_syntax = cds_FullName;
    attr->val_u.val_byte.length = len;
    attr->val_u.val_byte.byte_p = (char *)ptr;
    break;
  case VT_CHDirectory:
    attr->val_syntax = cds_CHDirectory;
    attr->val_u.val_byte.length = len;
    attr->val_u.val_byte.byte_p = (char *)ptr;
    break;
  case VT_ASN1:
    attr->val_syntax = cds_ASN1;
    attr->val_u.val_byte.length = len;
    attr->val_u.val_byte.byte_p = (char *)ptr;
    break;
  case VT_byte:
  default:
    attr->val_syntax = cds_byte;
    attr->val_u.val_byte.length = len;
    attr->val_u.val_byte.byte_p = ptr;
    break;
  }
}


