/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsvalidate.c,v $
 * Revision 1.1.8.2  1996/02/18  19:34:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:56  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:26:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:06  root]
 * 
 * Revision 1.1.6.4  1994/06/30  19:17:43  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:03:25  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:42:22  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:33  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/29  15:55:14  peckham
 * 	Change dns* to cds*
 * 	[1994/04/29  14:29:55  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:08:08  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:33:32  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:16:35  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:15:53  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/06  20:15:17  mfox
 * 	Limit classnames to 31 characters to prevent server crash
 * 	[1992/07/06  20:12:20  mfox]
 * 
 * Revision 1.1  1992/01/19  15:19:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * MODULE: dnsvalidate.c
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
#include <dce/assert.h>
#include <uc_clerk.h>
#include <uc_parse.h>

/* ----------------------------------------------------------------------
 *  Validate confidence value
 * ----------------------------------------------------------------------
 */
int 
dns_validate_fsConf (cdsConf_t conf)
{
    if (!conf || (conf == cdsConfLow))
    {
	return(CO_low);
    }
    
    if (conf == cdsConfMed)
    {
	return(CO_medium);
    }
    
    if (conf == cdsConfHi)
    {
	return(CO_high);
    }

    return(-1);
}

/* ----------------------------------------------------------------------
 *  Validate timeout value
 * ----------------------------------------------------------------------
 */
int 
dns_validate_fsWait (int wait)
{
    if (0 <= wait)
    {
	return(wait);
    }

    return(-1);
}

/* ----------------------------------------------------------------------
 *  Validate maybeMore value
 * ----------------------------------------------------------------------
 */
int 
dns_validate_fsMaybeMore (int more)
{
    if (more == FALSE)
    {
	return(BL_false);
    }
    
    if (more = TRUE)
    {
	return(BL_true);
    }

    return(-1);
}

/* ----------------------------------------------------------------------
 *  Validate cdsAttrType value
 * ----------------------------------------------------------------------
 */
int 
dns_validate_AttrType (cdsAttrType_t type)
{
    if (type == cdsEmptySet)
    {
	return(AT_null);
    }
    
    if (type == cdsSingle)
    {
	return(AT_single);
    }
    
    if (type == cdsSet)
    {
	return(AT_set);
    }

    return(-1);
}

/* ----------------------------------------------------------------------
 *  Validate name type
 * ----------------------------------------------------------------------
 */
int 
dns_validate_NameType (cdsNameType_t type)
{	
    switch (type) {
    case cdsStrDCE:
    case cdsOpqDNA:
    case cdsStrDNA:
	return(type);

    case cdsStrXDS:		/* Make look like cdsStrDCE */
    case 0:	
	return(cdsStrDCE);

    default:
	return(-1);
    }
}

/* ----------------------------------------------------------------------
 *  Validate Attr Syntax value
 * ----------------------------------------------------------------------
 */
int 
dns_validate_Syntax (cds_syntax_t syntax)
{
    switch (syntax) {
    case cds_none:
	dce_assert(cds__svc_handle,cds_none == VT_none);
	return((int)syntax);
    case cds_short:
	dce_assert(cds__svc_handle,cds_short == VT_short);
	return((int)syntax);
    case cds_long:
	dce_assert(cds__svc_handle,cds_long == VT_long);
	return((int)syntax);
    case cds_small:
	dce_assert(cds__svc_handle,cds_small == VT_small);
	return((int)syntax);
    case cds_uuid:
	dce_assert(cds__svc_handle,cds_uuid == VT_uuid);
	return((int)syntax);
    case cds_char:
	dce_assert(cds__svc_handle,cds_char == VT_char);
	return((int)syntax);
    case cds_byte:
	dce_assert(cds__svc_handle,cds_byte == VT_byte);
	return((int)syntax);
    case cds_FullName:
	dce_assert(cds__svc_handle,cds_FullName == VT_FullName);
	return((int)syntax);
    case cds_Timeout:
	dce_assert(cds__svc_handle,cds_Timeout == VT_Timeout);
	return((int)syntax);
    case cds_Timestamp:
	dce_assert(cds__svc_handle,cds_Timestamp == VT_Timestamp);
	return((int)syntax);
    case cds_Version:
	dce_assert(cds__svc_handle,cds_Version == VT_Version);
	return((int)syntax);
    case cds_ReplicaPointer:
	dce_assert(cds__svc_handle,cds_ReplicaPointer == VT_ReplicaPointer);
	return((int)syntax);
    case cds_GroupMember:
	dce_assert(cds__svc_handle,cds_GroupMember == VT_GroupMember);
	return((int)syntax);
    case cds_ParentPointer:
	dce_assert(cds__svc_handle,cds_ParentPointer == VT_ParentPointer);
	return((int)syntax);
    case cds_gdaPointer:
	dce_assert(cds__svc_handle,cds_gdaPointer == VT_gdaPointer);
	return((int)syntax);
    case cds_CHDirectory:
	dce_assert(cds__svc_handle,cds_CHDirectory == VT_CHDirectory);
	return((int)syntax);
    case cds_ASN1:
	dce_assert(cds__svc_handle,cds_ASN1 == VT_ASN1);
	return((int)syntax);
    }

    return(-1);
}

/* ----------------------------------------------------------------------
 *  Validate VT_Attr type value
 * ----------------------------------------------------------------------
 */
int 
dns_validate_VT_type (int type)
{
    switch (type)
    {
        case VT_none:
	    dce_assert(cds__svc_handle,VT_none == cds_none);
	    return (type);
        case VT_short:
	    dce_assert(cds__svc_handle,VT_short == cds_short);
	    return (type);
        case VT_small:
	    dce_assert(cds__svc_handle,VT_small == cds_small);
	    return (type);
        case VT_long:
	    dce_assert(cds__svc_handle,VT_long == cds_long);
	    return (type);
        case VT_uuid:
	    dce_assert(cds__svc_handle,VT_uuid == cds_uuid);
	    return (type);
        case VT_char:
	    dce_assert(cds__svc_handle,VT_char == cds_char);
	    return (type);
        case VT_byte:
	    dce_assert(cds__svc_handle,VT_byte == cds_byte);
	    return (type);
        case VT_FullName:
	    dce_assert(cds__svc_handle,VT_FullName == cds_FullName);
	    return (type);
        case VT_Timeout:
	    dce_assert(cds__svc_handle,VT_Timeout == cds_Timeout);
	    return (type);
        case VT_Timestamp:
	    dce_assert(cds__svc_handle,VT_Timestamp == cds_Timestamp);
	    return (type);
        case VT_Version:
	    dce_assert(cds__svc_handle,VT_Version == cds_Version);
	    return (type);
        case VT_ReplicaPointer:
	    dce_assert(cds__svc_handle,VT_ReplicaPointer == cds_ReplicaPointer);
	    return (type);
        case VT_GroupMember:
	    dce_assert(cds__svc_handle,VT_GroupMember == cds_GroupMember);
	    return (type);
        case VT_ParentPointer:
	    dce_assert(cds__svc_handle,VT_ParentPointer == cds_ParentPointer);
	    return (type);
        case VT_gdaPointer:
	    dce_assert(cds__svc_handle,VT_gdaPointer == cds_gdaPointer);
	    return (type);
        case VT_CHDirectory:
	    dce_assert(cds__svc_handle,VT_CHDirectory == cds_CHDirectory);
	    return (type);
        case VT_ASN1:
	    dce_assert(cds__svc_handle,VT_ASN1 == cds_ASN1);
	    return (type);
    }

    return(-1);
}

/* ----------------------------------------------------------------------
 *  Validate cdsEntries value
 * ----------------------------------------------------------------------
 */
int 
dns_validate_Entries (cdsEntries_t type)
{
    if (type == cdsObj)
	return(ET_object);
    if (type == cdsDir)
	return(ET_directory);
    if (type == cdsDirOrObj)
	return(ET_dirOrObj);
    if (type == cdsSLink)
	return(ET_softlink);
    if (type == cdsChildPtr)
	return(ET_childPointer);
    if (type == cdsCH)
	return(ET_clearinghouse);
    if (type == cdsAnyEntries)
	return(ET_anyDirectName);

    return(-1);
}

/* ----------------------------------------------------------------------
 *  Validate cdsGrpMbrType value
 * ----------------------------------------------------------------------
 */
int 
dns_validate_GrpMbrType (cdsGrpMbrType_t type)
{
    if (type == cdsPrincipal)
    {
	return(BL_false);
    }
    
    if (type == cdsGroup)
    {
	return(BL_true);
    }

    return(-1);
}

/* ----------------------------------------------------------------------
 *  Validate cdsReplicaType value
 * ----------------------------------------------------------------------
 */
int 
dns_validate_ReplicaType (cdsReplicaType_t type)
{
    if (type == cdsMaster)
    {    
	return(RT_master);
    }
    
    if (type == cdsSecondary)
    {
	return(RT_secondary);
    }
    
    if (type == cdsReadOnly)
    {
	return(RT_readOnly);
    }

    return(-1);
}

/* ----------------------------------------------------------------------
 *  Validate SimpleName value
 * ----------------------------------------------------------------------
 */
unsigned char *
dns_validate_sname (void *name_p)
{
    register SimpleName_t *this_p = (SimpleName_t *)name_p;

    switch (EXT8(this_p->sn_flag)) {
    case SN_null:
    case SN_ellipsis:
	if (EXT8(this_p->sn_length))
	    break;
    case SN_normal:
    case SN_objectid:
    case SN_quoted:
    case SN_binary:
    case SN_typed:
    case SN_wildcard:
    case SN_cds:
    case SN_cdswildcard:
	return((unsigned char *)name_p);
    }

    return((unsigned char *)0);
}

/* ----------------------------------------------------------------------
 *  Validate ClassName value
 * ----------------------------------------------------------------------
 */
unsigned char *
dns_validate_cname (void *name_p)
{
    register SimpleName_t *this_p = (SimpleName_t *)name_p;
    int len = EXT8(this_p->sn_length);
    unsigned char *ret = NULL;

    switch (EXT8(this_p->sn_flag)) {
    case SN_null:
    case SN_ellipsis:
	if (len)
	    break;
    case SN_normal:
    case SN_objectid:
    case SN_quoted:
    case SN_binary:
    case SN_typed:
    case SN_wildcard:
    case SN_cds:
    case SN_cdswildcard:
	if (len <= CLASSSIZE)
	    ret = (unsigned char *)name_p;
    }

    return(ret);
}

/* ----------------------------------------------------------------------
 *  Validate FullName value
 * ----------------------------------------------------------------------
 */
unsigned char *
dns_validate_fname (void *name_p,
                     int  nullokay)
{
    register FullName_t *const fn_p = (FullName_t *)name_p;
    register int length;

    /* Reject fullnames with null cts, even though convert
     * will generate them.
     */
    if ((!nullokay) && (ObjUID_is_nil(fn_p->fn_root)))
    {
	return((unsigned char *)0);
    }
    

    if ((length = EXT16(fn_p->fn_length)) <= FULLNAMEMAX) {
	byte_t *this_p = fn_p->fn_name;
	byte_t *end_p = &fn_p->fn_name[length];

	while (this_p < end_p) {
	    SimpleName_t *sn_p = (SimpleName_t *)this_p;

	    this_p = SKIP_SimpleName(this_p);

	    if (!dns_validate_sname(sn_p))
		break;

	    if (EXT8(sn_p->sn_flag) == SN_null)
		if (this_p == end_p)
		{
		    return((unsigned char *)name_p);
		}
		else
		    break;
	}
    }

    return((unsigned char *)0);
}
