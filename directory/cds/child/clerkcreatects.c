/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkcreatects.c,v $
 * Revision 1.1.6.2  1996/02/18  19:26:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:14  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:04:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:12  root]
 * 
 * Revision 1.1.4.4  1994/08/03  18:59:31  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:13:58  mccann]
 * 
 * Revision 1.1.4.3  1994/06/23  18:28:58  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:54:45  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  16:07:45  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:28  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:56:32  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:51:47  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:41:28  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:27:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:26:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkcreatects.c
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
 *	Clerk.CreateCTS command processor.
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
 * CreateCTS arguments (passed in the thread block)
 * First parameter was the sub-code that got us here.
 */
#define	CTS_P	clerkArgs[1]


/*
 *	c l e r k C r e a t e C T S
 *
 * This routine creates a cts.
 *
 * Outputs:
 *	CTS_P	        CTS
 *
 * Returns:
 */
void 
clerkCreateCTS (thread_t *th_p)
{
  DEB_ASCII_BUF_prm_buffer(ctsPrm)
  DEB_ASCII_BUF_Timestamp(ctsBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  Timestamp_u newcts;
  long status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug4,
      " >clerkCreateCTS(%s)",
      deb_ascii_prm_buffer(ctsPrm,th_p->CTS_P)));

  status = ts_new((Timestamp_t *)newcts);
  th_p->clerkStatus = status;
  if (status == DNS_SUCCESS) 
  {
    prm_begin(th_p);
    prm_copy(th_p, th_p->CTS_P, newcts, LEN_Timestamp(newcts));
    prm_end(th_p);
  }
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug4,
      " <clerkCreateCTS(%s) status(%s)",
      deb_ascii_Timestamp(ctsBuf, prm_out_value(th_p,th_p->CTS_P)),
      deb_ascii_dns_status(statusBuf,status)));
}
