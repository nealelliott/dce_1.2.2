/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dumpclerkcachecmd.c,v $
 * Revision 1.1.12.2  1996/02/18  19:28:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:30  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:08:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:06  root]
 * 
 * Revision 1.1.10.5  1994/08/03  19:00:35  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:41:54  mccann]
 * 
 * Revision 1.1.10.4  1994/07/08  15:32:20  peckham
 * 	cr10935 - Ugly error message for non-root 'cdscp dump clerk cache'
 * 	The bad error message came from an uninitialized return status.
 * 	Do cleanup: remove vms conditionals.
 * 	Use dce_priv_proc_is_appropriate() to check for privs.
 * 	Move dthread_init() to main().
 * 	[1994/06/29  19:35:16  peckham]
 * 
 * Revision 1.1.10.3  1994/06/09  18:36:38  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:20  devsrc]
 * 
 * Revision 1.1.10.2  1994/04/22  17:04:25  jd
 * 	Put call to dthread_init before call to cadump. cadump results in
 * 	a dthread_delay that was having problems without it.
 * 	[1994/04/22  13:53:10  jd]
 * 
 * Revision 1.1.10.1  1994/03/12  21:58:19  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:58:36  peckham]
 * 
 * Revision 1.1.8.1  1993/09/28  21:58:57  pwang
 * 	Removed the longjmp signal handler because it caused  cache_mutex
 * 	is held when it restored the stack context saved by setjmp().
 * 	[OT#7520]
 * 
 * Revision 1.1.5.3  1992/12/30  13:03:07  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:33:18  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  19:10:24  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:11:01  weisman]
 * 
 * Revision 1.1.2.2  1992/03/22  21:46:37  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:21:18  weisman]
 * 
 * Revision 1.1  1992/01/19  15:22:28  devrcs
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
 *
 *
 * Module Description:
 *
 *	Mgmt operation to dump the clerk cache.  This is just a temporary
 *	expedient, we really should use ncl.  The parser accepts the command
 *	as documented, except only for node 0.  The standalone program cadump
 *	is invoked to do the work. 
 *
 *	WARNINGS:  Cadump always claims U*ix success (returns 0), so this
 *		   routine does also.
 *
 *
 */

#include <cdscp.h>
#include <threads_lib.h>
#include <dce/dce_priv_proc.h>

 
int
dumpclerkcachecmd (void)
{
    static char *argv[] = {"cadump"};
    static int argc = (sizeof(argv)/sizeof(argv[0]));
    int		status;

    if (!dce_priv_proc_is_appropriate()) /* check for privs */
    {
	status = CDS_ACCESSVIOLATION;
    }
    else				/* maybe it will work */
    {   				/* cadump has other checks */
	status =                   	/* do the dumping */
                cadump(argc, argv, 0);  /* tell cadump() not to */
                                        /* display usage message */
    }
    return(status);     		/* return status */
}					/* end of dumpclerkcachecmd */
