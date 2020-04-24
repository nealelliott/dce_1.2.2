/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk_socket.c,v $
 * Revision 1.1.4.2  1996/02/18  19:25:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:48  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:03:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:37:32  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:30  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:15:47  annie]
 * 
 * 	expand copyright with OSF copyright text
 * 
 * 	expand copyright with OSF copyright text
 * 
 * Revision 1.1.2.1  1994/08/03  18:59:08  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:08:48  mccann]
 * 
 * $EndLog$
 */
/*
 * Module clerk_socket.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1994. ALL RIGHTS RESERVED.
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
 * Clerk library-child socket handling
 *
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>

#ifndef SOCK_STREAM             /* socket.h not idempotent in BSD ULTRIX */
#include <sys/socket.h>
#include <sys/un.h>
#endif
#include <sys/un.h>
#include <stdlib.h>
#include <unistd.h>
#include <uc_clerk.h>

/*
 *              c l e r k _ s o c k e t _ o p e n
 *
 * Opens a UNIX-domain socket, accepts connections, and passes
 * the new file descriptor to a newly-created client thread.
 *
 * Inputs:
 *      none
 *
 * Returns:
 *      none
 */
int
clerk_socket_open (const char *name_p)
{
    struct sockaddr_un addr;
    int         cfd;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug2,
        " >clerk_socket_open(%s)",
        name_p));
  
    /*
     * Create Unix domain socket and listen for connections
     * (only if socket specified on command line; otherwise use stdin)
     */

    ZERO_bytes(&addr, sizeof (addr));
    addr.sun_family = AF_UNIX;
    (void)strcpy(addr.sun_path, name_p);

    cfd = dthread_socket(AF_UNIX, SOCK_STREAM, PF_UNSPEC);

    if (cfd >= 0) 
    {
	int bind_status;

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_child,
	    svc_c_debug5,
	    "  clerk_socket_open %d=socket()",
	    cfd));

	bind_status = dthread_bind(cfd, (struct sockaddr *)&addr, sizeof (addr));
	if (bind_status >= 0) 
	{
	    int listen_status;

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_child,
		svc_c_debug5,
		"  clerk_socket_open %d=bind()",
		bind_status));

	    (void)chmod(addr.sun_path, 0666);
	    listen_status = dthread_listen(cfd, 5);
	    if (listen_status < 0) 
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_child,
		    svc_c_debug5,
		    "  clerk_socket_open %d=listen() failed, closing %d",
		    listen_status,
		    cfd));

		(void)unlink(addr.sun_path);
		(void)dthread_close(cfd);
		cfd = -1;
	    } 
	    else 
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_child,
		    svc_c_debug5,
		    "  clerk_socket_open %d=listen()",
		    listen_status));
	    }
	} 
	else 
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_child,
		svc_c_debug5,
		"  clerk_socket_open %d=bind() failed, closing %d",
		bind_status,
		cfd));

	    (void)dthread_close(cfd);
	    cfd = -1;
	}
    } 
    else 
    {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_child,
	    svc_c_debug5,
	    "  clerk_socket_open %d=socket() failed",
	    cfd));
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug2,
        " <clerk_socket_open(%s) return(%d)",
        name_p, cfd));

    return(cfd);
}

/*
 *              c l e r k _ l i s t e n e r
 *
 * Opens a UNIX-domain socket, accepts connections, and passes
 * the new file descriptor to a newly-created client thread.
 *
 * Inputs:
 *      none
 *
 * Returns:
 *      none
 */
int
clerk_socket_accept (int cfd)
{
    struct sockaddr_un addr;
    int addrlen = sizeof(addr);
    int nfd;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug2,
        " >clerk_socket_accept(%d)",
        cfd));
  
    nfd = dthread_accept(cfd, (struct sockaddr *)&addr, &addrlen);
  
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug2,
        " <clerk_socket_accept(%d) return(%d)",
        cfd, nfd));

    return(nfd);

}
