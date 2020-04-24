/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsgethandle.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:09  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:24:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:15  root]
 * 
 * Revision 1.1.4.4  1994/06/30  19:17:09  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:01:13  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:41:45  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:56  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:53:50  peckham
 * 	Use malloc() instead of dns_malloc().
 * 	[1994/04/29  14:19:59  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:06:58  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:29:44  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:56:57  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:11:54  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsgethandle.c
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
 *  Allocate memory buffers for use by read/enumerate
 * ----------------------------------------------------------------------
 */
cds_handle_t
cdsGetHandle (void)
{
    struct dns_ReadHandle *const rh_p = (struct dns_ReadHandle *)
		malloc(sizeof(struct dns_ReadHandle) + MAX_MSG_SIZE);

    if (rh_p) {
	rh_p->dns_ReadBuf_p = (unsigned char *)&rh_p[1];
	cdsClearHandle((cds_handle_t)rh_p);
    }
    return((cds_handle_t)rh_p);
}




