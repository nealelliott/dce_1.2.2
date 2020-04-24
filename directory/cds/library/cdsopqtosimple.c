/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdsopqtosimple.c,v $
 * Revision 1.1.6.2  1996/02/18  19:32:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:06  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:20:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:40  root]
 * 
 * Revision 1.1.4.5  1994/08/03  19:03:02  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:09:31  mccann]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:24  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:58:27  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:40:33  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:04  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:51:40  peckham
 * 	Remove DOSorOS2 conditional.
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	Don't use dnstables.
 * 	[1994/04/29  13:59:21  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:05:38  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:24:25  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:53:24  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:06:23  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * MODULE: cdsopqtosimple.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
#include <string.h>
#include <ctype.h>
#include <dce/cdsclerk.h>
#include <uc_clerk.h>

byte_t *
oid2str (
    dns_ToSimple_t	*data_p
);


static const char to_hex[] = "0123456789ABCDEF";

/*
 * cds_opqToSimple
 *	Internal routine to append simple name to string
 *
 * Input:
 *	data_p->snm_p = pointer to internal simple name
 *	data_p->chr_p = pointer to external name buffer
 *	data_p->length = remaining bytes in external name buffer
 *
 * Output:
 *	data_p->chr_p = points past inserted external name
 *	data_p->length = counted down for bytes used
 *
 * Value:
 *	(CDS_SUCCESS, CDS_INVALIDNAME)
 */
int 
cds_opqToSimple (dns_ToSimple_t *data_p)
{
    register byte_t *out_p = data_p->chr_p;
    register byte_t *sname_ptr = data_p->snm_p->sn_name;
    register int sname_length = EXT8(data_p->snm_p->sn_length);

    switch ((int)(data_p->snType = EXT8(data_p->snm_p->sn_flag))) {
    case SN_null:
	if (sname_length != 0)
	    return(CDS_INVALIDNAME);

	break;
    case SN_cds:
    case SN_cdswildcard:
	if (sname_length == 0)
	    return(CDS_INVALIDNAME);

	if (0 <= (data_p->length -= sname_length)) {
	    COPY_bytes(sname_ptr, out_p, sname_length);
	    out_p += sname_length;
	}
	break;
    case SN_binary:
	if (sname_length == 0)
	    return(CDS_INVALIDNAME);

	if (0 <= (data_p->length -= (2 + (sname_length<<1)))) {
	    *out_p++ = '%';
	    *out_p++ = (data_p->snType == SN_binary) ? 'X' : 'T';

	    if (sname_length)
		do {
		    *out_p++ = to_hex[(*sname_ptr>>4)&0x0F];
		    *out_p++ = to_hex[*sname_ptr++&0x0F];
		} while (--sname_length);
	}
	break;
    default:
	return(CDS_INVALIDNAME);
    }
    data_p->chr_p = out_p;
    data_p->fnLength += LEN_SimpleName(data_p->snm_p);
    data_p->snm_p = (SimpleName_t *)SKIP_SimpleName(data_p->snm_p);
    return(CDS_SUCCESS);
}

/*
 * The simple name contains an unescaped '='. Convert the preceding dotted-
 * decimal object id to a local symbol. Return NULL if no converstion took
 * place, otherwise return updated data_p->chr_p.
 */
byte_t 
*oid2str (dns_ToSimple_t *data_p)
{
    register byte_t *out_p = data_p->chr_p;
    register byte_t *sname_ptr = data_p->snm_p->sn_name;
    register int sname_length = EXT8(data_p->snm_p->sn_length);
    byte_t oid[CDS_STR_ANAME_MAX];
    byte_t *eq_p, *end_p, *oid_p;
    int status, olen;

    if (!isdigit(*sname_ptr))
	return(NULL);		/* 1st char is non-numeric - don't convert */

    eq_p = (byte_t *)strchr((char *)sname_ptr, '=');
    *eq_p = '\0';
    oid_p = oid;
    olen = sizeof(oid);
    status = cdsCvtStrToObjID(sname_ptr, (byte_t *)NULL, oid_p, &olen,
			(cds_attr_name_t *)NULL, (int *)NULL, &end_p);
    if (status != CDS_SUCCESS) {
	*eq_p = '=';
	return(NULL);		/* probably don't have a local string name */
    }
    /*
     * decrement input length by no. of chars in object id
     * and copy the digits and dots, etc, up to the equal sign
     */
    sname_length -= end_p - data_p->snm_p->sn_name;
    while (--olen > 0 && --data_p->length >= 0)
	*out_p++ = *oid_p++;
    for (sname_ptr = end_p; end_p < eq_p; end_p++) {
	if (--data_p->length >= 0)
	    *out_p++ = *sname_ptr;
	    sname_ptr++;
	    sname_length++;
    }
    /*
     * Now copy everything else from the '=' on
     */
    *sname_ptr = '=';
    do {
	if (--data_p->length >= 0)
	*out_p++ = *sname_ptr++;
    } while (--sname_length);

    return(out_p);
}
