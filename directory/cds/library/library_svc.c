/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: library_svc.c,v $
 * Revision 1.1.4.2  1996/02/18  19:35:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:27:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:25  root]
 * 
 * Revision 1.1.2.3  1994/06/30  19:17:50  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:04:10  mccann]
 * 
 * Revision 1.1.2.2  1994/06/09  18:42:26  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:38  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/12  22:08:35  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  20:03:34  peckham]
 * 
 * $EndLog$
 */
/* MODULE cds_library_svc.c
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

/* 
 * This module declares the Cell Directory Service (CDS) handle and 
 * and registers the table with a call to the OSF supplied macro
 * DCE_SVC_DEFINE_HANDLE.
 */
  
#include <dce/dce.h>
#include <dcecdssvc.h>

DCE_SVC_DEFINE_HANDLE(cds__svc_handle,	/* The CDS handle (to the CDS table) */
		      cds__svc_table,	/* The CDS table itself */
		      "cds")
