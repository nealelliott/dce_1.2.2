/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cadumpmain.c,v $
 * Revision 1.1.8.2  1996/02/18  19:24:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:02  marty]
 *
 * Revision 1.1.8.1  1995/12/08  00:10:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:31  root]
 * 
 * Revision 1.1.6.6  1994/08/03  18:59:01  mccann
 * 	fix assert
 * 	[1994/07/21  16:50:40  mccann]
 * 
 * Revision 1.1.6.5  1994/06/23  18:28:21  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:38:00  mccann]
 * 
 * Revision 1.1.6.4  1994/06/09  16:07:05  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:40  devsrc]
 * 
 * Revision 1.1.6.3  1994/06/08  20:17:49  peckham
 * 	Initialize serviceability and message catalogs.
 * 	[1994/05/31  18:55:54  peckham]
 * 
 * Revision 1.1.6.2  1994/04/14  14:37:56  peckham
 * 	Remove VMS conditional code.
 * 	[1994/04/11  17:35:36  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:55:39  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:47:21  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  12:36:37  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:19:42  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/15  15:54:28  tom
 * 	Bug 6409 - add setlocale call for i18n.
 * 	[1992/12/15  15:16:41  tom]
 * 
 * Revision 1.1.2.2  1992/03/22  21:12:26  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:04:58  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: cadumpmain.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
 */

#include <cache.h>
#include <dce/assert.h>
#include <threads_lib.h>
#include <locale.h>

/*
 * prototype declarations of procedures in this module
 */
int
main (
    int              ,
    char            **);

/*
 * Dump the caches to a file
 */
int 
main (int   argc,
      char  *argv[])
{
  char svcname[128];
  char *myname_p;
  error_status_t st;

  /*
   * Get our filename, stripped of directories
   */
  if (myname_p = strrchr(argv[0], '/'))
    myname_p++;
  else
    myname_p = argv[0];

  setlocale(LC_ALL, "");

  (void)sprintf(svcname, "%s(%ld)", myname_p, (long)getpid());
  dce_svc_set_progname(svcname, &st);
  dce_assert(cds__svc_handle, (st == svc_s_ok));

  dce_msg_define_msg_table(cds_msg_table,
                           sizeof(cds_msg_table) / sizeof(cds_msg_table[0]),
                           &st);
  dce_assert(cds__svc_handle, (st == msg_s_ok));

  dthread_init();

  return(cadump(argc,argv,1) == DNS_SUCCESS ? 0 : 1);
}
