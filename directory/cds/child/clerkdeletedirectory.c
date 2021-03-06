/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkdeletedirectory.c,v $
 * Revision 1.1.6.2  1996/02/18  19:26:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:19  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:04:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:22  root]
 * 
 * Revision 1.1.4.4  1994/08/03  18:59:36  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:14:12  mccann]
 * 
 * Revision 1.1.4.3  1994/06/23  18:29:02  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:55:03  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  16:07:49  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:33  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:56:39  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:52:14  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:41:49  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:27:38  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:26:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkdeletedirectory.c
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
 *	Clerk.DeleteDirectory command processor.
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
 * DeleteDirectory arguments (passed in the thread block)
 */
#define	DIRECTORY_P	clerkArgs[0]

/*
 * Local Prototype declarations.
 */
static int
DeleteDirectoryRPC (thread_t  *,
                    Set_t     *,
                    ObjUID_t  *);

int 
buildDeleteDirectoryRequest (thread_t *);

static void 
parseDeleteDirectoryResponse (thread_t *);

void 
clerkDeleteDirectory (thread_t *);


/*
 * Routine to call RPC stub for DeleteDirectory request
 */
static int
DeleteDirectoryRPC (thread_t  *th_p,
                    Set_t     *tower_p,
                    ObjUID_t  *uuid_p)
{  
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >DeleteDirectoryRPC"));

    return(clerkProgressRPC(th_p, tower_p, uuid_p, cds_DeleteDirectory));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <DeleteDirectoryRPC"));
}

/*
 *	b u i l d D e l e t e D i r e c t o r y R e q u e s t
 *
 * Routine to build a DeleteDirectory request message and
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
buildDeleteDirectoryRequest (thread_t *th_p)
{
    int length = 0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildDeleteDirectoryRequest"));

    /*
     * DeleteDirectoryRequest record:
     *	progress: Progress
     */
    length += buildProgress(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildDeleteDirectoryRequest: return(length=%d)",
	length));

    return(length);
}

/*
 *	p a r s e D e l e t e D i r e c t o r y R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseDeleteDirectoryResponse (thread_t *th_p)
{
  static EntryType_u type = {ET_dirOrObj};

  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " >parseDeleteDirectoryResponse"));

    /*
     * DeleteDirectoryResponse record:
     *	progress: Progress
     */
    ta_begin(th_p);
    ta_Progress(th_p);
    ta_end(th_p);

    if (th_p->taRspMsg_p) {
      Progress_t *const pr_p = (Progress_t *)th_p->taRspMsg_p;

      /* Use resolved name */
      CADeleteName(
	     (FullName_t *)SKIP_FullName(pr_p->pr_unresolvedName),
	     (FullName_t *)th_p->DIRECTORY_P, (EntryType_t *)type);
    }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " <parseDeleteDirectoryResponse"));
}

/*
 *	c l e r k D e l e t e D i r e c t o r y
 *
 * This routine deletes a directory.
 *
 * Inputs:
 *	DIRECTORY_P	FullName of directory to be deleted
 */
void 
clerkDeleteDirectory (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(directoryBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
 
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkDeleteDirectory(%s)",
        deb_ascii_FullName(directoryBuf,th_p->DIRECTORY_P)));

    th_p->taBuildReq = buildDeleteDirectoryRequest;
    th_p->taParseRsp = parseDeleteDirectoryResponse;
    th_p->taRPC = DeleteDirectoryRPC;

    th_p->cache_the_dir = 1;

    sendToWalkTree(th_p, th_p->DIRECTORY_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkDeleteDirectory(%s) status(%s)",
        deb_ascii_FullName(directoryBuf,th_p->DIRECTORY_P),
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}
