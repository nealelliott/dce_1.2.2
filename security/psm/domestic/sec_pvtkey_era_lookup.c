/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: sec_pvtkey_era_lookup.c,v $
 * Revision 1.1.2.2  1996/10/15  20:53:36  arvind
 * 	CHFts20150: bad call to sec_id_global_parse_name
 * 	CHFts20149: uses client-only free
 * 	CHFts20143: goes remote when it shouldn't
 * 	plus fixes from AHA and simplifications.
 * 	[1996/10/04  21:44 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 *
 * Revision 1.1.2.1  1996/10/04  14:26:21  arvind
 * 	     OSF DCE 1.2.2 Drop 4
 * 	     [1996/10/04  14:00:17  arvind]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:32 UTC  arvind
 * 	     PKSS drop (2) from DEC (DCE1.2.2)
 * 	     [1996/09/04  14:16 UTC  arvind  /main/arvind_pkss/2]
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 * Module:
 *
 * Purpose:
 *
 */

#include <stddef.h>
#include <u_str.h> 
#include <dce/sec_attr_base.h>
#include <dce/uuid.h>
#include <dce/binding.h>
#include <dce/sec_rgy_attr.h>
#include <dce/sec_pvtkey_base.h>

#include "sec_pvtkey_switch.h"

static unsigned char *bypass_cell;
static sec_pvtkey_get_bypass_fn_t bypass_get;

void sec_pvtkey__era_fetch_bypass(unsigned char *cell,
				sec_pvtkey_get_bypass_fn_t get)
{
    if (bypass_cell != NULL)
	return;
    bypass_cell = (unsigned char *)strdup((char *)cell);
    bypass_get = get;
}



unsigned32 sec_pvtkey__get_uuid_era_by_uuid(sec_rgy_name_t global_rgy_name,
                                            const uuid_t * uuid,
                                            uuid_t * era_value) {
    sec_rgy_handle_t		rgy_context;
    sec_rgy_bind_auth_info_t	auth_info;
    sec_attr_cursor_t		attr_cursor;
    sec_attr_t			attr_keys[1];
    sec_attr_t			out_attrs[1];
    unsigned32			num_returned = 0;
    unsigned32			num_left = 0;
    sec_rgy_name_t		canon_rgy_cell;
    sec_rgy_name_t		canon_rgy_principal;
    unsigned32			st = 0;
    unsigned32			lst;

    memset(&auth_info, 0, sizeof(auth_info));
    auth_info.info_type = sec_rgy_bind_auth_none;

    sec_id_global_parse_name(&auth_info,
                             global_rgy_name,
                             canon_rgy_cell,
                             NULL,
                             canon_rgy_principal,
                             NULL,
                             &st);

    if (st) return st;

    if (bypass_cell != NULL) {
	if (u_strcmp(bypass_cell, canon_rgy_cell) == 0) {
	    st = (*bypass_get)(canon_rgy_principal, uuid, era_value);
	    return st;
	}
    }

    sec_rgy_site_bind_query(canon_rgy_cell,
                            &auth_info,
                            &rgy_context,
                            &st);
    if (st) return st;


    attr_keys[0].attr_id = *uuid;
    attr_keys[0].attr_value.attr_encoding = sec_attr_enc_void;

    sec_rgy_attr_cursor_alloc(&attr_cursor, &st);
    if (st) {
	sec_rgy_site_close(rgy_context, &lst);
	return st;
    };

    sec_rgy_attr_lookup_by_id(rgy_context,
                              sec_rgy_domain_person,
                              canon_rgy_principal,
                              &attr_cursor,
                              (unsigned32)1,
                              (unsigned32)1,
                              attr_keys,
			      &num_returned,
			      out_attrs,
                              &num_left,
                              &st);

    sec_rgy_site_close(rgy_context, &lst);
    
    sec_rgy_attr_cursor_release(&attr_cursor, &lst);

    if (st) {
        return st;
    } else if ((num_returned != 1) ||
               !uuid_equal(&attr_keys[0].attr_id, &out_attrs[0].attr_id, &lst) ||
	       (out_attrs[0].attr_value.attr_encoding != sec_attr_enc_uuid))
    {
        if (num_returned > 1) {
	    sec_attr_util_free(rpc_ss_client_free, out_attrs[0]);
	}
        return sec_pk_e_key_attr_read_failed;
    } else {
	/* Copy UUID to parameter */
	*era_value = out_attrs[0].attr_value.tagged_union.uuid;
	sec_attr_util_free(rpc_ss_client_free, out_attrs[0]);	
	return error_status_ok;        
    };

}

