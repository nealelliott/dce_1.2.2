/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_svg_rec.h,v $
 * Revision 1.1.6.2  1996/02/18  23:01:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:48  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:29:51  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/07/11  18:14 UTC  jrr
 * 	add parameter do_acl to indicate first/second pass
 * 	[1995/06/23  13:20 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:03 UTC  greg
 * 	merge
 * 	[1995/05/01  19:13 UTC  hanfei  /main/SEC_migrate_merge/1]
 * 
 * 	add construct_version_check()
 * 	[1995/05/01  19:05 UTC  hanfei  /main/hanfei_migrate_bl2/1]
 * 
 * Revision 1.1.4.2  1994/06/17  18:42:12  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:51  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/07  15:06:28  hanfei
 * 	[OT 8067] add entry sec_svg_exist_prtfiles().
 * 	[1994/03/04  19:27:49  hanfei]
 * 
 * Revision 1.1.2.3  1993/03/23  23:07:45  hanfei
 * 	add codes for salvager reconstruct
 * 	[1993/03/23  23:00:47  hanfei]
 * 
 * Revision 1.1.2.2  1993/03/05  18:29:35  frisco
 * 	initial salvager functionality
 * 	[1993/03/02  21:12:28  frisco]
 * 
 * $EndLog$
 */

/*  
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
*/

/*
 *      Registry Database Salvager Reconstruct Interface Include file
 */

#ifndef sec_salvage_rec__included
#define sec_salvage_rec__included

/* Public Function Prototypes */

void construct_version_check (
#ifdef __STDC__
    error_status_t *status
#endif
);

void construct_init (
#ifdef __STDC__
    error_status_t *status
#endif
);

void construct_acls (
#ifdef __STDC__
    error_status_t *status
#endif
);

void construct_accts (
#ifdef __STDC__
    error_status_t *status
#endif
);

void construct_member (
#ifdef __STDC__
    error_status_t *status
#endif
);

void construct_save (
#ifdef __STDC__
    error_status_t *status
#endif
);

void construct_pgo_domain (
#ifdef __STDC__
    sec_rgy_domain_t domain,
    boolean32      do_acl,
    error_status_t *status
#endif
);

void construct_mkey (
#ifdef __STDC__
    error_status_t *status
#endif
);

void construct_policies (
#ifdef __STDC__
    boolean32      do_acl,
    error_status_t *status
#endif
);

void construct_rgy_state (
#ifdef __STDC__
    error_status_t *status
#endif
);

void construct_replicas (
#ifdef __STDC__
    boolean32      do_acl,
    error_status_t *status
#endif
);

boolean32 sec_svg_exist_prtfiles (
#ifdef __STDC__
#endif
);

#endif
