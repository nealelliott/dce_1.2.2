/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkdeletelink.c,v $
 * Revision 1.1.6.2  1996/02/18  19:26:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:20  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:04:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:24  root]
 * 
 * Revision 1.1.4.4  1994/08/03  18:59:37  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:22:10  mccann]
 * 
 * Revision 1.1.4.3  1994/06/23  18:29:03  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:55:07  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  16:07:51  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:34  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:56:40  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:52:20  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:41:53  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:27:42  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:26:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkdeletelink.c
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
 *	Clerk.DeleteLink command processor.
 *
 * Networks & Communications Software Engineering
 *
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>
#include <uc_clerk.h>

/*
 * DeleteLink arguments (passed in the thread block)
 */
#define	LINKNAME_P	clerkArgs[0]

/*
 * Local Prototype declarations.
 */

static int 
DeleteSoftLinkRPC (thread_t  *,
                   Set_t     *,
                   ObjUID_t  *);

int 
buildDeleteLinkRequest (thread_t *);

static void 
parseDeleteLinkResponse (thread_t *);

void 
clerkDeleteLink (thread_t *);


/*
 * Routine to call RPC stub for DeleteSoftLink request
 */
static int
DeleteSoftLinkRPC (thread_t  *th_p,
                   Set_t     *tower_p,
                   ObjUID_t  *uuid_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >DeleteSoftLinkRPC"));

     return(clerkProgressRPC(th_p, tower_p, uuid_p, cds_DeleteSoftLink));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <DeleteSoftLinkRPC"));
}

/*
 *      b u i l d D e l e t e L i n k R e q u e s t
 *
 * Routine to build a DeleteLink request message and
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
buildDeleteLinkRequest (thread_t *th_p)
{
    int length = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildDeleteLinkRequest"));

    /*
     * DeleteLinkRequest record:
     *	progress: Progress
     */
    length += buildProgress(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildDeleteLinkRequest: return(length=%d)",
        length));

    return(length);
}

/*
 *	p a r s e D e l e t e L i n k R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseDeleteLinkResponse (thread_t *th_p)
{
    static EntryType_u type = {ET_softlink};


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseDeleteLinkResponse"));

    /*
     * DeleteLinkResponse record:
     *	progress: Progress
     */
    ta_begin(th_p);
    ta_Progress(th_p);
    ta_end(th_p);

    /* Clear out the cache */
    if (th_p->taRspMsg_p) {
      Progress_t *const pr_p = (Progress_t *)th_p->taRspMsg_p;

      /* Use resolved name */
      CADeleteName(
             (FullName_t *)SKIP_FullName(pr_p->pr_unresolvedName),
	     (FullName_t *)th_p->LINKNAME_P, (EntryType_t *)type);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseDeleteLinkResponse"));
}

/*
 *	c l e r k D e l e t e L i n k
 *
 * This routine deletes a soft link.
 *
 * Inputs:
 *	LINKNAME_P	FullName of soft link to be deleted
 */
void 
clerkDeleteLink (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(linkNameBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkDeleteLink(%s)",
        deb_ascii_FullName(linkNameBuf,th_p->LINKNAME_P)));

    th_p->taBuildReq = buildDeleteLinkRequest;
    th_p->taParseRsp = parseDeleteLinkResponse;
    th_p->taRPC = DeleteSoftLinkRPC;
    ++th_p->skipResolveNameCache;

    sendToWalkTree(th_p, th_p->LINKNAME_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkDeleteLink(%s) status(%s)",
        deb_ascii_FullName(linkNameBuf,th_p->LINKNAME_P),
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
