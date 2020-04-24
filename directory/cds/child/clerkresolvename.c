/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkresolvename.c,v $
 * Revision 1.1.8.2  1996/02/18  19:26:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:35  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:05:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:47  root]
 * 
 * Revision 1.1.6.4  1994/08/03  18:59:50  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:17:39  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:29:15  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:56:08  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  16:08:03  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:48  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:57:01  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:53:49  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  12:42:49  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:28:54  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/11  14:41:40  keegan
 * 	Plug a memory leak in 'resolvename'. Clearinghouse list added to the
 * 	'cloned' thread_t structure is never released.
 * 	[1992/12/11  14:41:01  keegan]
 * 
 * Revision 1.1.2.2  1992/06/17  18:34:34  keegan
 * 	In the function 'clerk_ResolveName', the new thread should inherit the
 * 	level count from the existing thread to avoid infinite recursion on
 * 	'clerk_find_ch' from treewalk's ResolveName call.
 * 	[1992/06/17  17:43:15  keegan]
 * 
 * Revision 1.1  1992/01/19  15:27:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: clerkresolvename.c
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
 *	Clerk.ResolveName command processor.
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
 * ResolveName arguments (passed in the thread block)
 */
#define	LINKNAME_P	clerkArgs[0]
#define OUTLEN_P        clerkArgs[1]
#define	RET_P		clerkArgs[2]

/*
 * Local Prototype declarations.
 */

static int 
ResolveNameRPC (thread_t  *,
                Set_t     *,
                ObjUID_t  *);

static int 
buildResolveNameRequest (thread_t *);

static void 
parseResolveNameResponse (thread_t *);


/*
 * Routine to call RPC stub for ResolveName request
 */
static int 
ResolveNameRPC (thread_t  *th_p,
                Set_t     *tower_p,
                ObjUID_t  *uuid_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >ResolveNameRPC, cds_ResolveNam()=0x%lx",cds_ResolveName));

    return(clerkProgressRPC(th_p, tower_p, uuid_p, cds_ResolveName));
}

/*
 *	b u i l d R e s o l v e N a m e R e q u e s t
 *
 * Routine to build an ResolveName request message and
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
buildResolveNameRequest (thread_t *th_p)
{
    int length = 0;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
	" >buildResolveNameRequest"));
    
    /*
     * ResolveNameRequest record:
     *	progress: Progress
     */
    length += buildProgress(th_p);


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
	" <buildResolveNameRequest: return(length=%d)",
        length));

    return(length);
}

/*
 *	p a r s e R e s o l v e N a m e R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseResolveNameResponse (thread_t *th_p)
{
    byte_t *arg_p;
    int len;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
	" >parseResolveNameResponse"));
    /*
     * ResolveNameResponse record:
     *	progress: Progress
     */
    ta_begin(th_p);
    arg_p = ta_Progress_resolvedName(th_p, th_p->RET_P);

    /* Compute resolvedname length */
    len = LEN_FullName(arg_p);
    prm_out_Integer(th_p, (prm_out_t *)th_p->OUTLEN_P, (byte_t *)&len);

    ta_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
	" <parseResolveNameResponse"));
}

/*
 *	c l e r k R e s o l v e N a m e
 *
 * Follow any soft links and return the target FullName
 *
 * Inputs:
 *	LINKNAME_P	FullName of link to be traced
 *
 * Returns:
 *	RET_P		FullName of entry pointed to by link
 */
void 
clerkResolveName (thread_t *th_p)
{
    DEB_ASCII_BUF_FullName(linkNameBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkResolveName(%s,%p)",
        deb_ascii_FullName(linkNameBuf,th_p->LINKNAME_P),
        th_p->RET_P));

    th_p->taBuildReq = buildResolveNameRequest;
    th_p->taParseRsp = parseResolveNameResponse;
    th_p->taRPC = ResolveNameRPC;

    sendToWalkTree(th_p, th_p->LINKNAME_P);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkResolveName(%s,%p) status(%s)",
        deb_ascii_FullName(linkNameBuf,th_p->LINKNAME_P),
        th_p->RET_P,
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}

/*
 * Internal Resolvename Setup.  Used to assist high confidence processing
 */
int 
clerk_ResolveName (thread_t    *th_p,
                   FullName_t  *inname_p,
                   FullName_t  *outname_p)
{
  DEB_ASCII_BUF_FullName(innameBuf)
  DEB_ASCII_BUF_FullName(outnameBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  thread_t *newth_p;
  dns_status_t status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug5,
      " >clerk_ResolveName(%s,%p)",
      deb_ascii_FullName(innameBuf,inname_p),
      outname_p));

  /* Create a new thread to resolve the name */
  newth_p = alloc_child(th_p->link_p, th_p);
  if (newth_p == NULL) 
  {
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_child,
          svc_c_debug5,
	  " <clerk_ResolveName: return(DNS_NONSRESOURCES)"));
      
      return(DNS_NONSRESOURCES);
  }
  
  newth_p->taOperation = OP_ResolveName;
  newth_p->taObj = 44;

  /* Set up response length for deterimining max_size */
  newth_p->clerkRspBufLen = MAX_MSG_SIZE;

  /* Don't pass other paramters */
  newth_p->LINKNAME_P = (prm_in_t *)inname_p;

  newth_p->clerkConf = CO_medium;
  newth_p->clerkWait = 0;
  newth_p->clerkStatus = DNS_SUCCESS;
  newth_p->level = th_p->level;
  newth_p->OPtype = CH_read;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug5,
      "  clerk_ResolveName(%s,%p) level(%d)",
      deb_ascii_FullName(innameBuf,inname_p),
      outname_p,
      newth_p->level));

  /* Don't using any caching */
  ++newth_p->skipResolveNameCache;

  clerkResolveName(newth_p);

  status = newth_p->clerkStatus;
  if (status == DNS_SUCCESS) 
  {
    Progress_t *const pr_p = (Progress_t *)newth_p->taRspMsg_p;
    COPY_FullName(SKIP_FullName(pr_p->pr_unresolvedName), outname_p);
  }

#if defined(DCE_CDS_DEBUG)

    else
      COPY_FullName(NullFullName, outname_p);

#endif

  if (newth_p->cle_p != NULL)
  {
      CAClearinghousesUsed(newth_p->cle_p);
  }
  
  free_child(newth_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug5,
      " <clerk_ResolveName(%s,%p->%s) status(%s)",
      deb_ascii_FullName(innameBuf,inname_p),
      outname_p,
      deb_ascii_FullName(outnameBuf,outname_p),
      deb_ascii_dns_status(statusBuf,status)));

  return(status);
}
