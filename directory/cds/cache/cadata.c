/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cadata.c,v $
 * Revision 1.1.6.2  1996/02/18  19:23:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:58  marty]
 *
 * Revision 1.1.6.1  1995/12/08  00:10:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:27  root]
 * 
 * Revision 1.1.4.4  1994/06/23  18:28:18  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:15:09  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  16:07:02  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:36  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:37:52  peckham
 * 	Remove unix conditional and duplicate header files.
 * 	[1994/04/11  17:33:03  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  21:55:32  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:46:54  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:36:23  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:19:18  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module cadata.c
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

#include <cache.h>
#include <sys/time.h>

struct CacheFixed *CAfixed_p;
