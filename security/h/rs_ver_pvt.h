/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1995, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_ver_pvt.h,v $
 * Revision 1.1.6.2  1996/08/09  12:04:49  arvind
 * 	Merge Registry migration functionality: database reformat and enable sec_passwd_genprivkey
 * 	[1996/07/18  18:45 UTC  aha  /main/DCE_1.2.2/aha_pk7/1]
 *
 * 	Merge registry migration work from aha_pk6
 * 	[1996/07/18  15:37 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
 *
 * 	Add rs_c_sw_feature_ constant for support of registry storage of
 * 	KDC private key
 * 	[1996/05/24  23:25 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:49 UTC  hanfei  /main/DCE_1.2/2]
 *
 * 	define rs_c_software_vers_1_2_2 and rs_c_sw_feature_global_group.
 * 	[1996/03/14  18:53 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:21  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/05/15  21:35 UTC  greg  /main/HPDCE02/1]
 *
 * 	security migration merge
 * 	[1995/12/08  16:53:38  root]
 *
 * Revision 1.1.6.1  1996/05/10  13:16:19  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:49 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/14  18:53 UTC  hanfei
 * 	define rs_c_software_vers_1_2_2 and rs_c_sw_feature_global_group.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:50 UTC  psn
 * 	security migration merge
 * 	[1995/05/15  21:35 UTC  greg  /main/HPDCE02/1]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Software Versioning declaration
 * 
 */

#ifndef rs_version_pvt__h
#define rs_version_pvt__h
 
/*
 * Be sure to look at rs_ver.h.  It contains things that
 * would have been declared/defined here if not for the
 * fact that we needed them available in the client modules
 * that deal with software versioning in order to 
 * rationalize what a new client sees in terms of
 * software version information when talking to an old
 * server.  They really should be in this module, but, that's
 * life ...
 *
 * rs_ver.h must always be #included before rs_ver_pvt.h
 * wherever rs_ver_pvt.h is used.
 */

#define rs_c_min_software_vers  rs_c_software_vers_1_0_2   
#define rs_c_max_software_vers  rs_c_software_vers_1_2_2

#define rs_c_default_min_sw_vers rs_c_software_vers_1_0_2
#define rs_c_default_max_sw_vers rs_c_software_vers_1_2_2

/*
 *  Feature id constants
 */
#define rs_c_sw_feature_unknown     0 /* just to make sure zero is reserved */
#define rs_c_sw_feature_cell_alias  1
#define rs_c_sw_feature_polymorph   2
#define rs_c_sw_feature_passwd_exp  3
#define rs_c_sw_feature_era         4
#define rs_c_sw_feature_global_group 5
#define rs_c_sw_feature_kdcprivkey   6

/* 
 * Total number of feature ids (including "unknown").  Must
 * be adjusted whenever new feature ids are added
 */
#define rs_c_sw_feature_num_ids     7

/*
 * Functions
 */

/*
 * This function returns a pointer to statically allocated
 * storage, so look, but don't touch.
 */
extern unsigned_char_p_t rs_sw_vers_get_printstring (
    unsigned32   sw_vers
);

/*
 * Determine wwhether or not a particular feature 
 * feature is supported at the current sw_rev.
 *
 * Returns "true" if specified feature is supported
 * false if it is not.  
 * 
 * If the feature is not supported, the no_support_status 
 * contains the "no support" failure status configured for
 * the specified feature.
 */
extern boolean32 rs_sw_feature_test_support (
    unsigned32       feature_id,        /* [in]  */
    error_status_t  *no_support_status  /* [out] */
);

/*
 * A faster version  of rs_sw_feature_test_support()
 * for callers that already have the current sw version
 * in hand for other purposes.
 */
extern boolean32 rs_sw_feature_test_version (
    unsigned32       feature_id,         /* [in]  */
    unsigned32       current_sw_version, /* [in]  */
    error_status_t  *no_support_status   /* [out] */
);

#endif
