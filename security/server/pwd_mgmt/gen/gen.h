/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gen.h,v $
 * Revision 1.1.4.2  1996/02/18  23:03:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:13  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:47:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:22  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:22:02  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:20  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  15:00:52  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:18:28  mdf]
 * 
 * 	/main/ODESSA_2/1  1994/06/15  17:48 UTC  mullan_s
 * 	Merge -from mullan_el_bl19 -to ODESSA_2
 * 
 * 	/main/mullan_el_bl19/1  1994/05/19  16:53 UTC  mullan_s
 * 	Header file for generation public API.
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

#include <dce/nbase.h>
#include <dce/sec_attr_base.h>

void generate_pwd (
    unsigned32     plcy_args,   /* [in]  */
    sec_attr_t     plcy[],      /* [in]  */
    unsigned char  **gen_pwd,   /* [out] */
    error_status_t *status      /* [out] */
);
