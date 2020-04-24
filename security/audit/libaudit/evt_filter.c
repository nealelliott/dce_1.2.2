/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: evt_filter.c,v $
 * Revision 1.1.10.1  1996/07/08  18:31:38  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:26 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:24  marty  1.1.8.3]
 *
 * Revision 1.1.8.3  1996/02/18  00:06:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:46  marty]
 * 
 * Revision 1.1.8.2  1995/12/08  17:41:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:12  root]
 * 
 * Revision 1.1.4.1  1994/10/13  19:37:58  blazin
 * 	For CR 12493, added dce_aud_set_local_cell_uuid, a new api which will be called
 * 	when secd starts.
 * 	[1994/10/13  19:37:42  blazin]
 * 
 * Revision 1.1.2.4  1994/03/11  22:48:41  luan
 * 	CR 9775 9776: Changed debug messages to use dce_svc_printf() or DCE_SVC_DEBUG.
 * 	[1994/03/11  22:27:43  luan]
 * 
 * Revision 1.1.2.3  1994/02/22  16:50:54  luan
 * 	CR 9775 9776 9860:  Defined DCE_AUD_DEBUG in audit.h.  Removed
 * 	debug_mode and PRE_BETA_DEBUG or replaced it with DCE_DEBUG.
 * 	[1994/02/22  16:36:24  luan]
 * 
 * Revision 1.1.2.2  1994/01/23  20:42:19  luan
 * 	OT# 9796: Changed to allow multiple-guide audfilter add
 * 	and audfilter remove operations.
 * 	[1994/01/23  20:41:59  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:31  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:41  zeliff]
 * 
 * $EndLog$
 */

/*
** evt_filter.c
** Determines whether/how an event should be audited given its subject 
** identity, and event outcome.
*/

#include <evtp.h>

#define APPLY \
        aud_esl_lookup_guides(guides, evt, outcome, \
				 &applicable_guides); \
	if (applicable_guides != NULL) { \
	    if (cell_overridable_guides != NULL) { \
	        aud_esl_override(applicable_guides, \
					&cell_overridable_guides); \
	    } \
	    if (world_overridable_guides != NULL) { \
	        aud_esl_override(applicable_guides, \
					&world_overridable_guides); \
	    } \
 	    aud_esl_take_hwm(applicable_guides, outcome, as); \
	    aud_esl_free_guides(applicable_guides); \
        }

void           dce_aud_set_local_cell_uuid
               (
                   uuid_t         cell_uuid,
                   error_status_t * stp
               )
{ 
    properties.realm_uuid = cell_uuid;
    cell_uuid_known = TRUE;
    *stp = aud_s_ok;
    return;
} 


boolean32       aud_esl_match_ecs
                (
		                 aud_esl_evt_classes_p_t guide_ecs,
		                 unsigned32 evt,
		                 aud_esl_evt_classes_p_t * ecs_out
) {
    unsigned32             *evt_classes;
    int                     evt_class_cnt;
    aud_esl_evt_classes_p_t ecs_ptr;
    aud_esl_evt_classes_p_t matched_ec;
    boolean32               matched = FALSE;

    evt_classes =
	evt_table[evt - aud_g_cli_first_evt_number].evt_classes;
    evt_class_cnt = evt_table
	[evt - aud_g_cli_first_evt_number].evt_class_cnt;

    *ecs_out = NULL;

    ecs_ptr = guide_ecs;

    for (; evt_class_cnt > 0; evt_class_cnt--) {
	while (ecs_ptr != NULL) {
	    if (ecs_ptr->evt_class == evt_classes[evt_class_cnt - 1]) {
		matched = TRUE;
		matched_ec = (aud_esl_evt_classes_p_t)
		    malloc(sizeof(aud_esl_evt_classes_t));
		matched_ec->next = *ecs_out;
		matched_ec->evt_class = ecs_ptr->evt_class;
		*ecs_out = matched_ec;
	    }
	    ecs_ptr = ecs_ptr->next;
	}
    }
    return (matched);
}



void            aud_esl_lookup_guides
                (
		                 aud_esl_guides_p_t input_guides,
		                 unsigned32 evt,
		                 aud_esl_cond_t outcome,
		                 aud_esl_guides_p_t * output_guides
) {
    aud_esl_guides_p_t      guides = input_guides;
    aud_esl_evt_classes_p_t matched_ecs;
    aud_esl_guides_p_t      matched_guide;

    *output_guides = NULL;

    while (guides != NULL) {
	if ((guides->audit_condition & outcome) != 0) {
	    if (aud_esl_match_ecs(guides->ec_list, evt, &matched_ecs)) {
		matched_guide = (aud_esl_guides_p_t)
		    malloc(sizeof(aud_esl_guides_t));
		matched_guide->ec_list = matched_ecs;
		matched_guide->audit_condition = guides->audit_condition;
		matched_guide->audit_action = guides->audit_action;
		matched_guide->next = *output_guides;
		*output_guides = matched_guide;
	    }
	}
	guides = guides->next;
    }
    return;
}

void            aud_esl_override
                (
		                 aud_esl_guides_p_t overriding,
		                 aud_esl_guides_p_t * overriden
) {
    aud_esl_guides_p_t x = overriding;
    aud_esl_act_t saved_action;

    while (x != NULL && *overriden != NULL) {
	saved_action = x->audit_action;
	x->audit_action = aud_c_esl_act_all;
	aud_esl_remove_guides(overriden, x);

	/*
	 * "No action" is a special case, need to take care of it separately.
	 * This is ugly, should have a better way of handling this 
	 */

	x->audit_action = 0;
	aud_esl_remove_guides(overriden, x);
	x->audit_action = saved_action;

	x = x->next;
    }
    return;
}

/* Take high water mark with a new list of guides. */

void            aud_esl_take_hwm
                (
		                 aud_esl_guides_p_t new_guides,
		                 aud_esl_cond_t outcome,
		                 aud_esl_act_t ** hwm
) {
    aud_esl_guides_p_t x = new_guides;
    int                i, j;

    while (x != NULL && x->audit_action != 0) {
	x->audit_condition &= outcome;
	if (x->audit_condition != 0) {
	    for (i = 0; i < aud_c_esl_cond_number; i++) {
		if ((x->audit_condition & 0x1) != 0) {
		    if (*hwm == NULL) {
			(*hwm) = (aud_esl_act_t *) malloc
			    (aud_c_esl_cond_number * sizeof(aud_esl_act_t));
			for (j = 0; j < aud_c_esl_cond_number; j++) {
			    (*hwm)[j] = 0;
			}
		    }
		    (*hwm)[i] |= x->audit_action;
		}
		x->audit_condition >>= 1;
		if (x->audit_condition == 0) {
		    break;
		}
	    }
	}
	x = x->next;
    }
    return;
}


void aud_esl_get_actions
                (
		                 unsigned32 evt,
		                 sec_id_pac_t * privs,
		                 aud_esl_cond_t outcome,
/* returned action specifiers */
		                 aud_esl_act_t ** as,
		                 error_status_t * stp
) {
    int                i, j;
    uuid_t             subject_uuid, cell_uuid;
    unsigned16         probe;
    boolean32          local;
    aud_esl_guides_p_t guides;
    aud_esl_guides_p_t applicable_guides = NULL;
    aud_esl_guides_p_t cell_guides = NULL;
    aud_esl_guides_p_t cell_overridable_guides = NULL;
    aud_esl_guides_p_t world_guides = NULL;
    aud_esl_guides_p_t world_overridable_guides = NULL;
    aud_esl_entry_p_t  entry_ptr;

    *as = NULL;

    if (!cell_uuid_known) {
	sec_rgy_site_open(NULL, &rgy_context, stp);
	if (BAD_STATUS(stp)) {
	    return;
	}
	sec_rgy_properties_get_info(rgy_context, &properties, stp);
	if (BAD_STATUS(stp)) {
	    return;
	}
	cell_uuid_known = TRUE;
    }
    local = uuid_equal(&privs->realm.uuid, &properties.realm_uuid, stp);

    /* Get applicable cell overridable guides */

    subject_uuid = privs->realm.uuid;
    entry_ptr = (aud_esl_entry_p_t) aud_esl_entry_lookup
	(aud_e_esl_cell_overridable, aud_g_cli_esl,
	 subject_uuid, cell_uuid, &probe);
    if (entry_ptr != NULL) {
	guides = entry_ptr->guides;
	aud_esl_lookup_guides(guides, evt, outcome,
				 &cell_overridable_guides);
    } else {
	cell_overridable_guides = NULL;
    }

    /* Get applicable world and world overridable guides */

    if (aud_g_cli_world_overridable_guides != NULL) {
	aud_esl_lookup_guides(aud_g_cli_world_overridable_guides,
				 evt, outcome,
				 &world_overridable_guides);
    } else {
	world_overridable_guides = NULL;
    }

    if (aud_g_cli_world_guides != NULL) {
	aud_esl_lookup_guides(aud_g_cli_world_guides,
				 evt, outcome,
				 &world_guides);
    } else {
	world_guides = NULL;
    }

    /* Applying overriding rules */

    if (world_overridable_guides != NULL &&
	cell_overridable_guides != NULL) {
	aud_esl_override(cell_overridable_guides,
			    &world_overridable_guides);
    }
    if (world_guides != NULL && world_overridable_guides != NULL) {
	aud_esl_override(world_guides, &world_overridable_guides);
    }
    /* Get cell filter and apply the override rule. */

    {
	aud_esl_entry_p_t entry_ptr;
	subject_uuid = privs->realm.uuid;
	entry_ptr = (aud_esl_entry_p_t) aud_esl_entry_lookup
	    (aud_e_esl_cell, aud_g_cli_esl,
	     subject_uuid, cell_uuid, &probe);
	if (entry_ptr != NULL) {
	    guides = entry_ptr->guides;
	    APPLY;
	}
    }


    /* Get (foreign) princ filter and apply the override rule. */

    if (local) {
	aud_esl_entry_p_t entry_ptr;
	subject_uuid = privs->principal.uuid;
	cell_uuid = privs->realm.uuid;
	entry_ptr = (aud_esl_entry_p_t) aud_esl_entry_lookup
	    (aud_e_esl_princ, aud_g_cli_esl,
	     subject_uuid, cell_uuid, &probe);
	if (entry_ptr != NULL) {
	    guides = entry_ptr->guides;
	    APPLY;
	}
    } else {
	aud_esl_foreign_entry_p_t entry_ptr;
	subject_uuid = privs->principal.uuid;
	cell_uuid = privs->realm.uuid;
	entry_ptr = (aud_esl_foreign_entry_p_t) aud_esl_entry_lookup
	    (aud_e_esl_foreign_princ, aud_g_cli_esl,
	     subject_uuid, cell_uuid, &probe);
	if (entry_ptr != NULL) {
	    guides = entry_ptr->guides;
	    APPLY;
	}
    }

    /* Get group guides and apply the override rule. */

    if (local) {
	aud_esl_entry_p_t entry_ptr;
	entry_ptr = (aud_esl_entry_p_t) aud_esl_entry_lookup
	    (aud_e_esl_group, aud_g_cli_esl,
	     privs->group.uuid, cell_uuid, &probe);
	if (entry_ptr != NULL) {
	    guides = entry_ptr->guides;
	    APPLY;
	}
    } else {
	aud_esl_foreign_entry_p_t entry_ptr;
	entry_ptr = (aud_esl_foreign_entry_p_t) aud_esl_entry_lookup
	    (aud_e_esl_foreign_group, aud_g_cli_esl,
	     privs->group.uuid, privs->realm.uuid, &probe);
	if (entry_ptr != NULL) {
	    guides = entry_ptr->guides;
	    APPLY;
	}
    }

    for (j = 0; j < privs->num_groups; j++) {
	if (local) {
	    aud_esl_entry_p_t entry_ptr;
	    entry_ptr = (aud_esl_entry_p_t) aud_esl_entry_lookup
		(aud_e_esl_group, aud_g_cli_esl,
		 privs->groups[j].uuid, cell_uuid, &probe);
	    if (entry_ptr != NULL) {
		guides = entry_ptr->guides;
		APPLY;
	    }
	} else {
	    aud_esl_foreign_entry_p_t entry_ptr;
	    entry_ptr = (aud_esl_foreign_entry_p_t) aud_esl_entry_lookup
		(aud_e_esl_foreign_group, aud_g_cli_esl,
		 privs->groups[j].uuid, privs->realm.uuid, &probe);
	    if (entry_ptr != NULL) {
		guides = entry_ptr->guides;
		APPLY;
	    }
	}
    }

    /* Apply the high-water mark rule. */
    if (cell_overridable_guides != NULL) {
	aud_esl_take_hwm(cell_overridable_guides, outcome, as);
	aud_esl_free_guides(cell_overridable_guides);
    }
    if (world_guides != NULL) {
	aud_esl_take_hwm(world_guides, outcome, as);
	aud_esl_free_guides(world_guides);
    }
    if (world_overridable_guides != NULL) {
	aud_esl_take_hwm(world_overridable_guides, outcome, as);
	aud_esl_free_guides(world_overridable_guides);
    }
    return;
}

