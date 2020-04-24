/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdsstrings.c,v $
 * Revision 1.1.6.2  1996/02/18  19:32:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:08  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:20:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:42  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:16:25  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:58:31  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:40:37  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:05  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:51:42  peckham
 * 	Use cds*() names instead of dns*().
 * 	[1994/04/29  13:59:25  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:05:40  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:24:30  peckham]
 * 
 * Revision 1.1.2.3  1992/12/30  13:53:26  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:06:35  zeliff]
 * 
 * Revision 1.1.2.2  1992/12/04  20:15:40  keegan
 * 	cds_fulltoopq() and cds_simpletoopq() new set flags.fsLocStat to
 * 	CDS_INVALIDNAME.
 * 	[1992/12/04  20:07:23  keegan]
 * 
 * Revision 1.1  1992/01/19  15:17:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE cdsstrings.c
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
 *
 */
 
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/cdsclerk.h>
#include <uc_clerk.h>

/*
 * Convert a CDS string full name to opaque. Assumes that opqname points to a
 * buffer CDS_OPQ_FNAME_MAX bytes long. Return a pointer to the opaque name
 * or a null pointer (and set local status) if the conversion fails.
 */

cds_full_name_t *
cds_fulltoopq (cds_full_name_t  *string,
               cds_full_name_t  *opqname,
               cdsFlagStat_t    *flags)
{
	int len, status;

	if (string == NULL) {
		dns_status_clerk(flags, CDS_INVALIDNAME);
		return(string);
	}
	len = CDS_OPQ_FNAME_MAX;
	status = cdsCvtFullToOpq((unsigned char *)string, opqname, &len);
	if (status != CDS_SUCCESS) {
		dns_status_clerk(flags, status);
		opqname = NULL;
	}
	return(opqname);
}



/*
 * Convert a CDS string simple name to opaque. Assumes that opqname points to a
 * buffer CDS_OPQ_SNAME_MAX bytes long. Return a pointer to the opaque name
 * or a null pointer (and set local status) if the conversion fails.
 */

cds_simple_name_t *
cds_simpletoopq (cds_simple_name_t  *string,
                 int                iswild,
                 cds_simple_name_t  *opqname,
                 cdsFlagStat_t      *flags)
{
	int len, status;

	if (string == NULL) {
		dns_status_clerk(flags, CDS_INVALIDNAME);
		return(string);
	}
	len = CDS_OPQ_SNAME_MAX;
	status = cdsCvtSimpleToOpq((unsigned char *)string, iswild,
								opqname, &len);
	if (status != CDS_SUCCESS) {
		dns_status_clerk(flags, status);
		opqname = NULL;
	}
	return(opqname);
}


/*
 * Convert a CDS string attr. name to opaque. Assumes that opqname points to a
 * buffer CDS_OPQ_ANAME_MAX bytes long. Return a pointer to the opaque name
 * or a null pointer (and set local status) if the conversion fails.
 */

cds_attr_name_t *
cds_attrtoopq (cds_attr_name_t  *string,
               cds_attr_name_t  *opqname,
               cdsFlagStat_t    *flags)
{
  int len;
  int status;

  len = CDS_OPQ_ANAME_MAX;
  status = cdsCvtStrToObjID((unsigned char *)string, (byte_t *)NULL,
	(unsigned char *) NULL, (int *) NULL, opqname, &len,
	(unsigned char **) NULL);
  if (status != CDS_SUCCESS) {
    dns_status_clerk(flags, status);
    opqname = NULL;
  }
  return(opqname);

}
