/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_skulk.c,v $
 * Revision 1.1.6.2  1996/02/18  19:37:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:53  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:33:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:24  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:05:01  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:32:21  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:44:13  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:51  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:13:35  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:48:11  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:24:56  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:25:48  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:25:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module ta_skulk.c
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

#include <server.h>
#include <dbdef.h>
#include <security.h>
#include <back.h>


/*
 * ta_skulk
 *     This routine sends a mailbox message to the background process
 * requesting a skulk.  It waits for results.
 *
 * Input:
 *    user_p = address of user descriptor
 *    progress_p = address of progress record
 *
 * Value: 
 *    status
 */
dns_status_t 
ta_skulk (user_descriptor_t  *user_p,
          Progress_t         *progress_p)
{
    db_stream_t		*stream_p;
    dns_status_t	status;

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ModifyAttribute);
    if (status != DNS_SUCCESS)
        return(status);

    /* find the directory */
    status = walktree(user_p, ET_directory, WALKTREE_SKULK_OP, progress_p, stream_p);
    if (status != DNS_SUCCESS) return(status);

    status = security_check(user_p, stream_p, Skulk_check, ET_directory,
			    NULL); 
    if (status == DNS_SUCCESS) 
        /* Start the skulk and wait for results */
        status = skulk_sched(stream_p);
    else if (status == DNS_UNKNOWNENTRY)
        /* Rebuild resolved name for exception */
        resolved_null(progress_p);

    db_close_stream(stream_p);
    return(status);
}
