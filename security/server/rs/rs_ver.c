/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1995, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: rs_ver.c,v $
 * Revision 1.1.6.2  1996/08/09  12:10:43  arvind
 * 	Merge Registry migration functionality: database reformat and enable sec_passwd_genprivkey
 * 	[1996/07/18  18:45 UTC  aha  /main/DCE_1.2.2/aha_pk7/1]
 *
 * 	Merge registry migration work from aha_pk6
 * 	[1996/07/18  15:36 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
 *
 * 	Changes for Public Key Login; add feature for support of KDC private key storage
 * 	in registry
 * 	[1996/05/24  23:25 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:49 UTC  hanfei  /main/DCE_1.2/2]
 *
 * 	add rs_sw_feature_info entry for rs_c_sw_feature_global_group
 * 	[1996/03/15  15:25 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:29  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/05/17  23:58 UTC  rps  /main/HPDCE02/2]
 *
 * 	WP merge
 * 	HP 	[1995/05/08  17:20 UTC  rps  /main/DCE1_1_WP/2]
 *
 * 	SEC migration Warranty Patch merge.
 * 	HP 	[1995/05/15  21:35 UTC  greg  /main/HPDCE02/1]
 *
 * 	security migration merge
 * 	[1995/12/08  17:21:47  root]
 *
 * Revision 1.1.6.1  1996/05/10  13:23:53  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:49 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/15  15:25 UTC  hanfei
 * 	add rs_sw_feature_info entry for rs_c_sw_feature_global_group
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:50 UTC  psn
 * 	WP merge
 * 	[1995/05/17  23:58 UTC  rps  /main/HPDCE02/2]
 * 
 * 	migration: real status for cell alias
 * 	[1995/05/08  17:13 UTC  rps  /main/DCE1_1_WP/rps_wp/1]
 * 
 * 	SEC migration Warranty Patch merge.
 * 	[1995/04/26  19:48 UTC  jrr  /main/DCE1_1_WP/1]
 * 
 * 	security migration merge
 * 	[1995/05/15  21:35 UTC  greg  /main/HPDCE02/1]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1995
 * Unpublished work. All Rights Reserved.
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_ver.c,v 1.1.6.2 1996/08/09 12:10:43 arvind Exp $";
#endif

#include <dce/lbase.h>
#include <dce/rplbase.h>
#include <dce/assert.h>
#include <rs_ver.h>
#include <rs_ver_pvt.h>
#include <rs_base.h>
#include <sec_svc.h>

typedef struct {
    unsigned32         sw_vers;
    unsigned_char_p_t  sw_vers_str;
} rs_sw_vers_info_t;

typedef struct {
    unsigned32         feature;
    unsigned32         min_sw_vers;
    unsigned32         max_sw_vers;
    error_status_t     no_support_status;
} rs_sw_feature_support_t;

static rs_sw_version_t rs_version_id_unknown = "secd.dce.<unknown>";

static rs_sw_vers_info_t rs_sw_vers_info[rs_c_num_software_vers] = {
    {
	rs_c_software_vers_unknown,
        &rs_version_id_unknown[0]
    },
    {
	rs_c_software_vers_1_0_2,
        &rs_1_0_2_version_id[0]
    },
    {
	rs_c_software_vers_1_1,
        &rs_1_1_version_id[0]
    },    
    {
	rs_c_software_vers_1_2_2,
        &rs_1_2_2_version_id[0]
    }    
};

static rs_sw_feature_support_t rs_sw_feature_info[rs_c_sw_feature_num_ids] = {
    /* zeroth element is for internal use only */
    {
	rs_c_sw_feature_unknown,
        rs_c_software_vers_unknown,
        rs_c_software_vers_unknown,
        sec_s_pgmerr
    },
    {
	rs_c_sw_feature_cell_alias,
        rs_c_software_vers_1_1,
        rs_c_default_max_sw_vers,
	sec_rgy_alias_not_allowed
    },
    {
	rs_c_sw_feature_polymorph,
        rs_c_software_vers_1_1,
        rs_c_default_max_sw_vers,
	sec_rgy_name_exists
    },
    {
	rs_c_sw_feature_passwd_exp,
        rs_c_software_vers_1_1,
        rs_c_default_max_sw_vers,
        sec_s_v1_1_no_support
    },
    {
	/*
	 * Though we shut off "standard" client era operations
         * by not registering the relevant interfaces, we still
	 * need to know whether or not a server is supporting
	 * ERAs so that we can take selective action at both
         * master and slave during initialization.  The no_support_status
         * isn't really meaningful, but we set it to rpc_s_unknown_if
         * anyway, 'cause that's what an ordinary client sees.
	 */
	rs_c_sw_feature_era,
        rs_c_software_vers_1_1,
        rs_c_default_max_sw_vers,
        rpc_s_unknown_if
    },
    {
	rs_c_sw_feature_global_group,
        rs_c_software_vers_1_2_2,
        rs_c_default_max_sw_vers,
        sec_rgy_object_not_found
    },
    {
	rs_c_sw_feature_kdcprivkey,
        rs_c_software_vers_1_2_2,
        rs_c_default_max_sw_vers,
        sec_rgy_key_bad_type
    }
};


/* 
 * PUBLIC functions
 */

/*
 * Convert a numeric software version specification to
 * a printstring.
 *
 * This function returns a pointer to statically allocated
 * storage, so look, but don't touch.
 */
unsigned_char_p_t rs_sw_vers_get_printstring (
    unsigned32   sw_vers
)
{
    unsigned_char_p_t vers_string;
    

    if ((sw_vers < rs_c_min_software_vers) 
    || (sw_vers  > rs_c_max_software_vers)) {
	sw_vers = rs_c_software_vers_unknown;
    } 

    vers_string = &rs_sw_vers_info[sw_vers].sw_vers_str[0];
	
    return vers_string;
}

/*
 * A faster version of rs_sw_feature_test_support
 * for caller's that already have the current software
 * version in hand fot other reasons.
 */
boolean32 rs_sw_feature_test_version (
    unsigned32      feature_id,
    unsigned32      current_sw_version, 
    error_status_t  *no_support_status
)
{
    boolean32      supported = false;
    error_status_t lst;

    if (feature_id > rs_c_sw_feature_num_ids) {
	feature_id = rs_c_sw_feature_unknown;
    } 
    dce_assert(sec_svc_handle, (feature_id != rs_c_sw_feature_unknown));
   
    if ((current_sw_version < rs_sw_feature_info[feature_id].min_sw_vers)
    || (current_sw_version > rs_sw_feature_info[feature_id].max_sw_vers)) {
	*no_support_status = rs_sw_feature_info[feature_id].no_support_status;
    } else {
        supported = true;
    }

    return supported;
}

/*
 * Determine whether or not a particular feature 
 * is supported at the current sw_rev.
 */
boolean32 rs_sw_feature_test_support (
    unsigned32      feature_id,
    error_status_t  *no_support_status
)
{
    boolean32      supported = false;
    unsigned32     cur_sw_rev;
    error_status_t lst;

    rs_master_info_get_sw_rev(&cur_sw_rev, &lst);
    
    if (BAD_STATUS(&lst)) {
	/*
	 * If we can't determine our current sw rev,
	 * we're in deep, deep, trouble, so just
         * log a fatal exit through the serviceability
         * module.
	 */
	SEC_SVC_PRINT_MSG_ST(SEC_REP_CANT_GET_SW_REV_FATAL_MSG, lst);
    }

    if (feature_id > rs_c_sw_feature_num_ids) {
	feature_id = rs_c_sw_feature_unknown;
    } 
    dce_assert(sec_svc_handle, (feature_id != rs_c_sw_feature_unknown));
   
    if ((cur_sw_rev < rs_sw_feature_info[feature_id].min_sw_vers)
    || (cur_sw_rev > rs_sw_feature_info[feature_id].max_sw_vers)) {
	*no_support_status = rs_sw_feature_info[feature_id].no_support_status;
    } else {
        supported = true;
    }

    return supported;
}
