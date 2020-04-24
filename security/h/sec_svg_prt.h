/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_svg_prt.h,v $
 * Revision 1.1.7.2  1996/02/18  23:01:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:47  marty]
 *
 * Revision 1.1.7.1  1995/12/08  17:29:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:27  root]
 * 
 * Revision 1.1.5.2  1994/09/20  19:18:42  hanfei
 * 	add print_open_info_file() and print_close_info_file().
 * 	[1994/09/19  14:23:08  hanfei]
 * 
 * Revision 1.1.5.1  1994/06/17  18:42:11  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:50  devsrc]
 * 
 * Revision 1.1.3.3  1993/03/23  23:07:42  hanfei
 * 	add codes for salvager reconstruct
 * 	[1993/03/23  22:51:01  hanfei]
 * 
 * Revision 1.1.3.2  1993/03/05  18:29:16  frisco
 * 	initial salvager functionality
 * 	[1993/03/02  21:07:11  frisco]
 * 
 * Revision 1.1.1.2  1992/11/25  21:09:41  frisco
 * 	Initial Revision
 * 
 * $EndLog$
 */

/*  
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
*/

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: sec_svg_prt.h,v 1.1.7.2 1996/02/18 23:01:24 marty Exp $";
#endif

/*
 *      Registry Database Salvager Print Interface Include file
 */

#ifndef sec_salvage_prt__included
#define sec_salvage_prt__included

/* Public Function Prototypes */

void print_init (
    error_status_t *status
);

void print_open_info_file (
);

void print_close_info_file (
);

void print_pgo_domain (
    sec_rgy_domain_t domain,
    error_status_t *status
);

void print_mkey (
    error_status_t *status
);

void print_policies (
    error_status_t *status
);

void print_rgy_state (
    error_status_t *status
);

void print_replicas (
    error_status_t *status
);

#endif
