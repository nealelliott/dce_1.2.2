/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: createuid.c,v $
 * Revision 1.1.6.2  1996/02/18  19:27:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:41  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:05:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:54  root]
 * 
 * Revision 1.1.4.4  1994/06/23  18:29:19  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:56:26  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  16:08:07  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:52  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:41:18  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:36:49  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  21:57:10  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:54:18  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:43:10  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:29:21  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:27:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module createuid.c
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
 * MODULE DESCRIPTION:
 *
 *	Timestamp handling for DNS Clerk
 *
 * Networks & Communications Software Engineering
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <cache.h>

/*
 * Local Prototype declarations.
 */

int 
ts_new (Timestamp_t *);



/*
 * This routine gets a uid, it copies a local string variable
 * into the place the pointer argument points to.
 *
 */
int 
ts_new (Timestamp_t *ts_p)
{
    int status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ts_new()"));

    status = CACreateTimestamp(ts_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ts_new() return(%d)", status));

    return(status);
}
