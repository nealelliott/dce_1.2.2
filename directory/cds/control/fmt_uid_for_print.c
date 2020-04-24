/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: fmt_uid_for_print.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:00  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:09:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:32  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:53  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:49:49  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:54  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:40  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:59:08  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:01:27  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:05:40  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:36:36  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:21:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
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

#include <cdscp.h>
#include <dce/rpc.h>

void
fmt_uid_for_print (
	unsigned char  	*in,		/* pointer to a uid */
	char		*out)		/* ptr to where to put the result */
{
    unsigned32 status;
    unsigned_char_t *string_p;
    uuid_t uid;

    memcpy(&uid, in, sizeof(uid));
    uuid_to_string(&uid, &string_p, &status);
    if (status == uuid_s_ok) {
      strcpy(out, (char *)string_p);
      rpc_string_free(&string_p, &status);
    } else
      *out = '0';
}
