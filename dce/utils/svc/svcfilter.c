/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Filtering
 */

/*
 * HISTORY
 * $Log: svcfilter.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:22  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:38:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:10:00  root]
 * 
 * Revision 1.1.4.1  1994/06/09  16:06:26  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:54  devsrc]
 * 
 * Revision 1.1.2.2  1993/08/16  18:09:48  rsalz
 * 	Initial release
 * 	[1993/08/16  18:04:32  rsalz]
 * 
 * $EndLog$
 */

#include <svcimpl.h>


/*
**  Attach a filter and a filter-control to a component.
*/
void
dce_svc_define_filter(
    dce_svc_handle_t		handle,
    dce_svc_filter_proc_t	filter_function,
    dce_svc_filterctl_proc_t	filter_ctl_function,
    error_status_t		*status
)
{
    handle->filter = (int (*)(void))filter_function;
    handle->filterctl = (int (*)(void))filter_ctl_function;
    *status = svc_s_ok;
}
