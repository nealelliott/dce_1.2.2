/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkskulk.c,v $
 * Revision 1.1.6.2  1996/02/18  19:26:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:36  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:05:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:48  root]
 * 
 * Revision 1.1.4.4  1994/08/03  18:59:52  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:17:36  mccann]
 * 
 * Revision 1.1.4.3  1994/06/23  18:29:16  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:56:12  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  16:08:04  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:49  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:57:03  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:53:56  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:42:53  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:28:59  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:27:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkskulk.c
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
 * MODULE DESCIPTION:
 *
 *	Clerk.Skulk command processor.
 *
 * Networks & Communications Software Engineering
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>
#include <uc_clerk.h>

/*
 * Skulk arguments (passed in the thread block)
 */
#define	DIRECTORY_P	clerkArgs[0]

/*
 * Local Prototype declarations.
 */

static int 
SkulkRPC (thread_t  *,
          Set_t     *,
          ObjUID_t  *);

int 
buildSkulkRequest (thread_t *);

static void 
parseSkulkResponse (thread_t *);

void 
clerkSkulk (thread_t *);



/*
 * Routine to call RPC stub for Skulk request
 */
static int 
SkulkRPC (thread_t  *th_p,
          Set_t     *tower_p,
          ObjUID_t  *uuid_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >SkulkRPC"));

     return(clerkProgressRPC(th_p, tower_p, uuid_p, cds_Skulk));
}


/*
 *	b u i l d S k u l k R e q u e s t
 *
 * Routine to build an Skulk request message and
 * return length of data
 *
 * Inputs:
 *	th_p->taReqMsgEnd_p	buffer to receive message arguments
 *				if zero, only figure length.
 *
 * Returns:
 *	total length of message arguments.
 */
int 
buildSkulkRequest (thread_t *th_p)
{
    int length = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildSkulkRequest"));

    /*
     * SkulkRequest record:
     *	progress: Progress
     */
    length += buildProgress(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildSkulkRequest: return(length=%d)",
        length));

    return(length);
}

/*
 *	p a r s e S k u l k R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseSkulkResponse (thread_t *th_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseSkulkResponse"));

    /*
     * SkulkResponse record:
     *	progress: Progress
     */
    ta_begin(th_p);
    ta_Progress(th_p);
    ta_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseSkulkResponse"));
}

/*
 *	c l e r k S k u l k
 *
 * This routine deletes a directory, object or soft link.
 *
 * Inputs:
 *	DIRECTORY_P	FullName of directory to be updated
 */
void 
clerkSkulk (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(directoryBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkSkulk(%s)",
        deb_ascii_FullName(directoryBuf,th_p->DIRECTORY_P)));

    th_p->cache_the_dir = 1;
    th_p->taBuildReq = buildSkulkRequest;
    th_p->taParseRsp = parseSkulkResponse;
    th_p->taRPC = SkulkRPC;

    sendToWalkTree(th_p, th_p->DIRECTORY_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkSkulk(%s) status(%s)",
        deb_ascii_FullName(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
