/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_reserved.h,v $
 * Revision 1.1.12.3  1996/02/18  22:59:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:10  marty]
 *
 * Revision 1.1.12.2  1995/12/08  17:27:36  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:02 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:34 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Add current sw version to signature of
 * 	rs_resv_gen_uninit_db().
 * 	[1995/03/06  14:48 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Pass current software version to rs_resv_gen_uninit_db().
 * 	[1995/03/06  14:26 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	add  prototype for rs_resv_create_uninit_db().
 * 	[1995/03/03  21:30 UTC  greg  /main/SEC_migrate/1]
 * 
 * Revision 1.1.8.2  1994/08/25  16:29:59  hanfei
 * 	dce1.1 rgy database backward compatibility work
 * 	[1994/08/25  14:46:17  hanfei]
 * 
 * Revision 1.1.8.1  1994/07/15  14:59:39  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:54  mdf]
 * 
 * Revision 1.1.5.2  1993/07/08  11:16:06  root
 * 	Initial King Kong branch
 * 	[1993/07/08  11:15:33  root]
 * 
 * Revision 1.1.3.2  1993/05/19  17:25:43  cuti
 * 	Change foreign cell name to "zzzzzzz", so make sure foreign cell name always the last to print.
 * 	[1993/08/05  18:51:11  cuti]
 * 
 * Revision 1.1.6.2  1993/06/24  20:19:46  hinman
 * 	[hinman] - Save merged-in SNI version (these files have been freed of nasty code)
 * 	[1993/06/17  14:26:44  hinman]
 * 
 * Revision 1.1.2.6  1993/03/31  21:01:11  cuti
 * 	Add sec_salvage_db test's variables
 * 	[1993/03/25  21:38:51  cuti]
 * 
 * Revision 1.1.2.5  1993/02/01  20:36:03  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  16:14:28  hinman]
 * 
 * 	[hinman] - Check in merged SNI version
 * 	[1993/01/11  16:11:56  hinman]
 * 
 * Revision 9.5.1.8  93/01/05  16:17:54  meg
 * 	[meg] folded in osf copyright
 * 
 * Revision 9.5.6.2  93/01/05  16:16:08  meg
 * 	[meg] folded in osf copyright
 * 
 * Revision 9.5.1.7  92/11/18  13:06:50  meg
 * 	changed SNI_PRIVATE to SNI_PROPRIETARY
 * 
 * Revision 9.5.5.2  92/11/18  13:05:38  meg
 * 	changed SNI_PRIVATE to SNI_PROPRIETARY
 * 
 * Revision 9.5.1.6  92/11/17  15:23:30  meg
 * 	Resubmit login related changes.
 * 
 * Revision 9.5.4.2  92/11/17  15:21:56  meg
 * 	Resubmit login related changes.
 * 
 * Revision 9.5.1.4  92/11/10  15:10:24  meg
 * 	[meg] incorporates S. Tikku's mods to uxid and gid definitions for product.
 * 
 * Revision 9.5.2.2  92/11/10  13:36:01  meg
 * 	[meg] incorporates S. Tikku's mods to uxid and gid definitions for product.
 * 
 * Revision 9.5.1.3  92/10/20  17:03:21  hinman
 * 	Acceptance of OSF rev 1.1.2.3
 * 
 * Revision 9.12.1.2  92/10/19  15:23:11  hinman
 * 	Auto checkin of OSF rev 1.1.2.3
 * 
 * Revision 1.1.2.3  1992/10/07  20:27:30  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:29  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:41:56  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:19:23  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:57:10  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - Reserved names and Acount Mangement
 */

#ifndef rs_reserved__included
#define rs_reserved__included

#include <dce/lbase.h>
#include <dce/rgynbase.h>
#include <dce/id_base.h>


    /* reserved unix numbers */
/* Persons */
#define rgyc_uxid_nobody    4294967294
#define rgyc_uxid_root      0
#define rgyc_uxid_daemon    1

#ifdef SNI_PROPRIETARY
#else
#define rgyc_uxid_sys       2
#define rgyc_uxid_bin       3
#define rgyc_uxid_uucp      4
#define rgyc_uxid_who       5
#endif /* SNI_PROPRIETARY */

#define rgyc_uxid_mail      6
#define rgyc_uxid_tcb       9
#define rgyc_uxid_dce_ptgt 20
#define rgyc_uxid_dce_rgy  21
#define rgyc_uxid_mkey     22
#define rgyc_uxid_creator 100
#define rgyc_uxid_krbtgt    sec_rgy_uxid_unknown     /* dynamically assigned */
#define rgyc_uxid_host      sec_rgy_uxid_unknown     /* dynamically assigned */

/* Groups */
#define rgyc_gid_nogroup   4294967294

#ifdef SNI_PROPRIETARY
#else
#define rgyc_gid_system     0
#define rgyc_gid_daemon     1
#define rgyc_gid_uucp       2
#define rgyc_gid_bin        3
#define rgyc_gid_kmem       4
#endif /* SNI_PROPRIETARY */

#define rgyc_gid_mail       6
#define rgyc_gid_tty        7
#define rgyc_gid_none      12
#define rgyc_gid_tcb       18

/* Orgs */
#define rgyc_oid_none      12


/* sec_salvage_db test data */
#define foreign_krbtgt "krbtgt/zzzzzzz"
#define n_test1p  "test1pd/test2pd/test3pd/test1p" /* not using default dir acl */
#define n_test2p  "test1pd/test2pd/test2p"
#define n_test3p  "test1pd/test3p"
#define n_test4p  "test4p"
#define n_test5p  "test5p"  /* No account */
#define n_test1g  "test1gd/test2gd/test3gd/test1g"
#define n_test2g  "test1gd/test2gd/test2g"
#define n_test3g  "test1gd/test3g"
#define n_test4g  "test4g"
#define n_test1o  "test1od/test2od/test3od/test1o"
#define n_test2o  "test1od/test2od/test2o"
#define n_test3o  "test1od/test3o"
#define n_test4o  "test4o"

#define rgyc_gid_test1g  sec_rgy_uxid_unknown
#define rgyc_gid_test2g  sec_rgy_uxid_unknown
#define rgyc_gid_test3g  sec_rgy_uxid_unknown
#define rgyc_gid_test4g  sec_rgy_uxid_unknown

#define rgyc_oid_test1o  sec_rgy_uxid_unknown
#define rgyc_oid_test2o  sec_rgy_uxid_unknown
#define rgyc_oid_test3o  sec_rgy_uxid_unknown
#define rgyc_oid_test4o  sec_rgy_uxid_unknown

#define rgyc_uxid_test1p  sec_rgy_uxid_unknown
#define rgyc_uxid_test2p  sec_rgy_uxid_unknown
#define rgyc_uxid_test3p  sec_rgy_uxid_unknown
#define rgyc_uxid_test4p  sec_rgy_uxid_unknown
#define rgyc_uxid_test5p  sec_rgy_uxid_unknown
#define rgyc_uxid_foreign_krbtgt sec_rgy_uxid_unknown




/* If these items not supplied on command line to rgy_create,
 * use these default initial values.
 */
#define rs_default_creator              "root"
#define rs_default_password             "-dce-"

void db_create (
#ifdef __STDC__
    sec_id_t                *rgy_local_cell,
    sec_id_t                *rgy_creator,
    long                    rgy_creator_unix_id,
    unsigned_char_p_t       rgy_hostname,
    unsigned_char_p_t       rgy_password,
    error_status_t          *st
#endif
);

void rs_setup_creator (
#ifdef __STDC__
    sec_id_t            *rgy_creator,
    long                rgy_creator_unix_id,
    error_status_t      *st
#endif
);

void rs_resv_add_attr_sch_entries (
    error_status_t      *st_p
);

/*
 * Use this routine to selectively create 
 * reserved entries appropriate to the 
 * current software version whenever the
 * the database is (re)initialized.
 */
void rs_resv_gen_uninit_db (
    unsigned32      current_sw_rev,
    error_status_t  *st_p
);

#endif /* rs_reserved__included */

