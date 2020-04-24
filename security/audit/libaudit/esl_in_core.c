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
 * $Log: esl_in_core.c,v $
 * Revision 1.1.6.1  1996/07/08  18:31:03  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:26 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:16  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  00:06:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:43  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:41:30  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/25  13:42 UTC  bartf
 * 	merge hash_uuid_failed msg enhance
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_wc.10/3  1995/09/21  14:16 UTC  bartf
 * 	remove comments
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_wc.10/2  1995/09/19  19:58 UTC  bartf
 * 	fix uuid_string in print
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_wc.10/1  1995/09/18  17:40 UTC  bartf
 * 	add more info to uuid_hash_failed msg
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/14  18:42 UTC  jrr
 * 	Change dce_printf() to dce_svc_printf().
 * 	[1995/07/14  18:35 UTC  jrr  /main/jrr_mothra_pure/1]
 * 
 * Revision 1.1.2.4  1994/03/11  22:48:37  luan
 * 	CR 9775 9776: Changed debug messages to use dce_svc_printf() or DCE_SVC_DEBUG.
 * 	[1994/03/11  22:20:32  luan]
 * 
 * Revision 1.1.2.3  1994/02/22  16:47:29  luan
 * 	CR 9775 9776 9860:  Defined DCE_AUD_DEBUG in audit.h.  Removed
 * 	debug_mode and PRE_BETA_DEBUG or replaced it with DCE_DEBUG.
 * 	[1994/02/22  16:40:41  luan]
 * 
 * Revision 1.1.2.2  1994/01/23  20:42:18  luan
 * 	OT# 9796: Changed to allow multiple-guide audfilter add
 * 	and audfilter remove operations.
 * 	[1994/01/23  20:41:45  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:25  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:37  zeliff]
 * 
 * $EndLog$
 */

/*
** esl_in_core.c
** Routines for maintaining the in-core filter tables.
*/

#include <dce/dce.h>
#include <dce/dce_error.h>
#include <dce/dce_msg.h>
#include <dce/audit.h>
#include <stdarg.h>
#include <time.h>
#include <dce/nbase.h>
#include <sys/stat.h>
#include <dce/idlbase.h>
#include <dce/nbase.h>
#include <dce/stubbase.h>
#include <dce/uuid.h>
#include <error_statusp.h>
#include <eslp.h>
#include <esl_filep.h>
#include <dce/pgo.h>
#include <dce/dceaudmsg.h>
#include <dce/dceaudmac.h>
#include <dce/dceaudsvc.h>

pthread_mutex_t esl_mutex;

static void
                aud__esl_dup_ec_list
                (
		                 aud_esl_evt_classes_p_t src_list,
		                 aud_esl_evt_classes_p_t * dest_list
) {
    aud_esl_evt_classes_p_t ec_ptr1, ec_ptr2;

    if (src_list == NULL) {
	*dest_list = NULL;
	return;
    }
    *dest_list = (aud_esl_evt_classes_p_t)
	malloc(sizeof(aud_esl_evt_classes_t));
    (*dest_list)->evt_class = src_list->evt_class;
    (*dest_list)->next = NULL;
    ec_ptr2 = *dest_list;
    src_list = src_list->next;

    while (src_list != NULL) {
	ec_ptr1 = (aud_esl_evt_classes_p_t)
	    malloc(sizeof(aud_esl_evt_classes_t));
	ec_ptr1->evt_class = src_list->evt_class;
	ec_ptr2->next = ec_ptr1;
	ec_ptr1->next = NULL;
	ec_ptr2 = ec_ptr1;
	src_list = src_list->next;
    }
    return;
}

static void
                aud__esl_remove_ecs
                (
		                 aud_esl_evt_classes_p_t to_be_removed,
		                 boolean32 save_removed_ec,
		                 aud_esl_evt_classes_p_t * current,
		                 aud_esl_evt_classes_p_t * removed
) {
    aud_esl_evt_classes_p_t x_ec_pt1, x_ec_pt2, existing_ec_pt1, existing_ec_pt2;

    x_ec_pt1 = x_ec_pt2 = to_be_removed;
    if (save_removed_ec)
	*removed = NULL;

    while (x_ec_pt1 != NULL) {
	existing_ec_pt1 = existing_ec_pt2
	    = *current;

next_existing_class:
	while (existing_ec_pt1 != NULL) {

	    if (x_ec_pt1->evt_class == existing_ec_pt1->evt_class) {

		if (existing_ec_pt1 == existing_ec_pt2 &&
		    existing_ec_pt1->next == NULL) {
		    /* removing the last existing ec */
		    *current = NULL;

#define REMOVE_EC(x)	\
if (save_removed_ec) { \
    if (*removed == NULL) { \
        x->next = NULL; \
    } else { \
        x->next = *removed; \
    } \
    *removed = x; \
} else { \
    free(x); \
}
		    REMOVE_EC(existing_ec_pt1);
		    return;
		} else {	/* not removing the last existing ec */

		    if (existing_ec_pt1 == *current) {
			*current = existing_ec_pt1->next;
#ifdef DCE_AUD_DEBUG
			DCE_SVC_DEBUG((aud_svc_handle, aud_s_esl, 
			    svc_c_debug1, "aud__esl_remove_ecs: Removing event class %d\n", existing_ec_pt1->evt_class));
#endif 
			REMOVE_EC(existing_ec_pt1);
			existing_ec_pt1 = existing_ec_pt2
			    = *current;
		    } else {
			existing_ec_pt2->next = existing_ec_pt1->next;
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_esl, svc_c_debug1,
	    "aud__esl_remove_ecs: Removing event class %d\n", 
	    existing_ec_pt1->evt_class));
#endif 
			REMOVE_EC(existing_ec_pt1);
			existing_ec_pt1 = existing_ec_pt2->next;
		    }
		    goto next_existing_class;
		}		/* end of last existing ec "if" */
	    } else {		/* not a matching event class */

		/* simply move to the next new event class */
		existing_ec_pt2 = existing_ec_pt1;
		existing_ec_pt1 = existing_ec_pt1->next;

	    }			/* end of the event class matching "if" */

	}			/* end of the existing event classes "while"
				 * loop */

	x_ec_pt2 = x_ec_pt1;
	x_ec_pt1 = x_ec_pt1->next;

    }				/* end of the new event classes "while" */

}


/*
 * Free the memory of a single guide.
 */

void
                aud_esl_free_guide
                (
		                 aud_esl_guides_p_t guide
) {
    aud_esl_evt_classes_p_t ec_ptr1, ec_ptr2;

    if (guide->ec_list != NULL) {
	ec_ptr1 = ec_ptr2 = guide->ec_list;

	while (ec_ptr1->next != NULL) {
	    ec_ptr2 = ec_ptr1->next;
	    free(ec_ptr1);
	    ec_ptr1 = ec_ptr2;
	}
	free(ec_ptr1);
    }
    free(guide);
    return;
}

/*
 * Free the memory of a linked list of guides.
 */

void
                aud_esl_free_guides
                (
		                 aud_esl_guides_p_t guides
) {
    aud_esl_guides_p_t guides_to_be_freed, current_guide;

    if (guides == NULL) {
	return;
    }

    guides_to_be_freed = guides;

    while (guides_to_be_freed != NULL) {
	current_guide = guides_to_be_freed;
	guides_to_be_freed = guides_to_be_freed->next;
	aud_esl_free_guide(current_guide);
    }
    return;
}


void
                aud_esl_copy_guides
                (
		                 aud_esl_guides_p_t src_guides,
		                 aud_esl_guides_p_t * dest_guides
) {
    aud_esl_guides_p_t      src_guides_ptr, dest_guides_ptr1, dest_guides_ptr2;
    aud_esl_evt_classes_p_t src_ec_ptr, dest_ec_ptr1, dest_ec_ptr2;

    if (src_guides == NULL || dest_guides == NULL)
	return;

    src_guides_ptr = src_guides;
    dest_guides_ptr1 = dest_guides_ptr2 = NULL;

    while (src_guides_ptr != NULL) {
	dest_guides_ptr1 = (aud_esl_guides_p_t)
	    malloc(sizeof(aud_esl_guides_t));
	dest_guides_ptr1->next = dest_guides_ptr2;
	dest_guides_ptr1->audit_condition = src_guides_ptr->audit_condition;
	dest_guides_ptr1->audit_action = src_guides_ptr->audit_action;
	src_ec_ptr = src_guides_ptr->ec_list;
	dest_ec_ptr1 = dest_ec_ptr2 = NULL;
	while (src_ec_ptr != NULL) {
	    dest_ec_ptr1 = (aud_esl_evt_classes_p_t)
		malloc(sizeof(aud_esl_evt_classes_t));
	    dest_ec_ptr1->next = dest_ec_ptr2;
	    dest_ec_ptr1->evt_class = src_ec_ptr->evt_class;
	    dest_ec_ptr2 = dest_ec_ptr1;
	    src_ec_ptr = src_ec_ptr->next;
	}
	dest_guides_ptr1->ec_list = dest_ec_ptr1;
	dest_guides_ptr2 = dest_guides_ptr1;
	src_guides_ptr = src_guides_ptr->next;
    }
    *dest_guides = dest_guides_ptr1;
    return;
}

/* 
 * Remove duplicated event class numbers per guide in a list
 * of guides.  For example, 
 *    
 * Guide {{ec1, ec2, ec1}, success, log} would be compacted
 * to {{ec1. ec2}, success, log}.
 *
 * Being paranoid that some remote filter-control program might not
 * check duplicated event class numbers.
 */

void
                aud_esl_compact_ec_list
                (
		                 aud_esl_guides_p_t guides
) {
    aud_esl_guides_p_t      guide;
    aud_esl_evt_classes_p_t current_ec_pt, ec_pt1, ec_pt2;

    if ((guide = guides) == NULL) {
	return;
    }

    while (guide != NULL) {
        current_ec_pt = guide->ec_list;
        while (current_ec_pt != NULL) {
	    ec_pt2 = current_ec_pt;
	    ec_pt1 = current_ec_pt->next;
	    while (ec_pt1 != NULL) {
	        if (ec_pt1->evt_class == current_ec_pt->evt_class) {
		    ec_pt2->next = ec_pt1->next;
		    free(ec_pt1);
		    ec_pt1 = ec_pt2->next;
	        } else {
		    ec_pt2 = ec_pt1;
		    ec_pt1 = ec_pt1->next;
	        }
	    }
	    current_ec_pt = current_ec_pt->next;
        }
	guide = guide->next;
    }
    return;
}

static void
                aud__esl_purge_guide
                (
		                 aud_esl_guides_p_t * existing_guides,
		                 aud_esl_guides_p_t * guide_pt1,
		                 aud_esl_guides_p_t * guide_pt2
) {
    aud_esl_guides_p_t guide_to_be_freed = *guide_pt1;


    if (*guide_pt1 == *guide_pt2) {

	/* removing the first guide on the chain */

	*existing_guides = *guide_pt1 = *guide_pt2
	    = ((*guide_pt1)->next == NULL)
	/* Is this the also the last? */
	    ?
	    NULL : guide_to_be_freed->next;

    } else {			/* not removing the first filter */

	(*guide_pt2)->next = (*guide_pt1)->next;
	*guide_pt1 = (*guide_pt1)->next;

    }

#ifdef DCE_AUD_DEBUG 
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_esl, svc_c_debug1,
	    "aud__esl_purge_guide: Removing guide (condition %lx action %lx)\n",
	    (guide_to_be_freed)->audit_condition,
	    (guide_to_be_freed)->audit_action));
#endif
    aud_esl_free_guide(guide_to_be_freed);
    return;
}


void
                aud_esl_add_guides
                (
		                 aud_esl_guides_p_t * existing_guides,
		                 aud_esl_guides_p_t guides_to_be_added
) {
    aud_esl_guides_p_t      existing_guides_pt1, existing_guides_pt2;
    aud_esl_evt_classes_p_t new_ec_pt;
    aud_esl_guides_p_t      new_guides_pt = guides_to_be_added;
    aud_esl_guides_p_t      tmp_guides_pt;

next_new_guide:
    while (new_guides_pt != NULL) {
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_esl, svc_c_debug1,
	    "aud_esl_add_guides: Adding a new guide action %lx, condition %lx. \n",
	    new_guides_pt->audit_action, new_guides_pt->audit_condition));
#endif
	if ((*existing_guides) == NULL) {
	/* There is no existing guide. */
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_esl, svc_c_debug1,
	    "aud_esl_add_guides: No existing guide.  New guide Action %lx Condition %lx is being inserted in the beginning\n",
	    new_guides_pt->audit_action,
	    new_guides_pt->audit_condition));
#endif
	    /*
	     * Insert the current new guide in the beginning. 
	     */
	    *existing_guides = new_guides_pt;
	    tmp_guides_pt = new_guides_pt->next;
	    (*existing_guides)->next = NULL;
	} else {
	    /* guide(s) exist(s) */

	    existing_guides_pt1 = existing_guides_pt2
		= *existing_guides;

	    while (existing_guides_pt1 != NULL) {

		if (new_guides_pt->audit_action ==
		    existing_guides_pt1->audit_action &&
		    new_guides_pt->audit_condition ==
		    existing_guides_pt1->audit_condition) {

		    /*
		     * This is the right combination of audit condition and
		     * action, insert the new event class(es) here. 
		     */

		    new_ec_pt = new_guides_pt->ec_list;
		    while (new_ec_pt->next != NULL)
			new_ec_pt = new_ec_pt->next;
		    new_ec_pt->next = existing_guides_pt1->ec_list;
		    existing_guides_pt1->ec_list = new_guides_pt->ec_list;

		    /*
		     * We are done with the new guide. Its allocated space
		     * will be freed below. 
		     */

		    /* freeing the new guide */
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_esl, svc_c_debug1,
	    "aud_esl_add_guides: Removing new guide Action %d Condition %d\n",
	    new_guides_pt->audit_action, new_guides_pt->audit_condition));
#endif
		    tmp_guides_pt = new_guides_pt->next;
		    free(new_guides_pt);
		    break;
		}
		if (existing_guides_pt1->audit_action <
		    new_guides_pt->audit_action ||
		    (existing_guides_pt1->audit_action ==
		     new_guides_pt->audit_action &&
		     existing_guides_pt1->audit_condition <
		     new_guides_pt->audit_condition)
		    ) {

#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_esl, svc_c_debug1,
	    "aud_esl_add_guides: Inserting Action %lx Condition %lx before Action %lx Condition %lx \n",
	    new_guides_pt->audit_action, new_guides_pt->audit_condition,
	    existing_guides_pt1->audit_action,
	    existing_guides_pt1->audit_condition));
#endif

		    if (existing_guides_pt1 == existing_guides_pt2)
			*existing_guides = new_guides_pt;
		    else
			existing_guides_pt2->next = new_guides_pt;

		    tmp_guides_pt = new_guides_pt->next;
		    new_guides_pt->next = existing_guides_pt1;
		    break;

		} else {
		    if (existing_guides_pt1->next == NULL) {

			/*
			 * Append the new guide to the end. 
			 */
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_esl, svc_c_debug1,
	    "aud_esl_add_guides: Appending Action %lx Condition %lx after Action %lx Level %lx \n",
	    new_guides_pt->audit_action, new_guides_pt->audit_condition,
	    existing_guides_pt1->audit_action,
	    existing_guides_pt1->audit_condition));
#endif

			existing_guides_pt1->next = new_guides_pt;
			tmp_guides_pt = new_guides_pt->next;
			new_guides_pt->next = NULL;

			/* out of existing filters "while" loop */
			break;
		    } else {
			existing_guides_pt2 = existing_guides_pt1;
			existing_guides_pt1 = existing_guides_pt1->next;
		    }
		}
	    }
	}			/* end of the existing guides "while" loop */
	new_guides_pt = tmp_guides_pt;
    }				/* end of the new guides "while" loop */
}


/*
 * Update existing list of guides with a new guide.  The memory of the
 * the new guide is used/destructed!  Save a copy of the new guide if
 * before calling this function, if it is needed later!!! 
 */

void
                aud_esl_update_guides_with_one_guide
                (
		                 aud_esl_guides_p_t * existing_guides,
		                 aud_esl_guides_p_t new_guide_pt
) {
    aud_esl_guides_p_t      existing_guides_pt1, existing_guides_pt2;
    aud_esl_evt_classes_p_t existing_ec_pt1, existing_ec_pt2, new_ec_pt1, new_ec_pt2;
    aud_esl_evt_classes_p_t removed_ecs;
    aud_esl_guides_p_t      guides_to_be_added = NULL;
    aud_esl_guides_p_t      modified_guide;

    if (new_guide_pt == NULL) {
	return;
    }

    existing_guides_pt1 = existing_guides_pt2 = *existing_guides;

    while (existing_guides_pt1 != NULL) {

	/*
	 * Begin to scan through the event class list of an existing filter.
	 * Delete the matching event classes from the existing filter if the
	 * audit action and condition of the existing filter do not match the
	 * audit action and condition of the new filter. 
	 */


	/*
	 * The NO ACTION "0" bit vector condition has been treated
	 * separately.  
	 */

	if ((new_guide_pt->audit_action ==
	     existing_guides_pt1->audit_action
	     ||
	     (new_guide_pt->audit_action != 0
	      &&
	      (~existing_guides_pt1->audit_action &
	       new_guide_pt->audit_action
	       ) == 0
	      )
	     )
	    &&
	    (~existing_guides_pt1->audit_condition &
	     new_guide_pt->audit_condition
	     ) == 0
	    ) {

	    aud__esl_remove_ecs(existing_guides_pt1->ec_list,
				   FALSE, &new_guide_pt->ec_list,
				   NULL);
	    if (new_guide_pt->ec_list == NULL) {

		/* freeing the new filter */
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_esl, svc_c_debug1,
            "aud_esl_update_guides_with_one_guide: Removing Action %lx Condition %lx from new guide\n",
	    new_guide_pt->audit_action,
	    new_guide_pt->audit_condition));
#endif
		free(new_guide_pt);

		/*
		 * There could not have been modified filters in this case,
		 * so we can safely return here. Don't need to check if there
		 * are modified filters to be added back. 
		 */
		return;
	    } else {
		existing_guides_pt2 = existing_guides_pt1;
		existing_guides_pt1 = existing_guides_pt1->next;
	    }

	} else {

	    aud__esl_remove_ecs(new_guide_pt->ec_list, TRUE,
			       &existing_guides_pt1->ec_list, &removed_ecs);
	    if (removed_ecs != NULL) {
		if ((existing_guides_pt1->audit_condition &
		     ~new_guide_pt->audit_condition) != 0) {
		    modified_guide = (aud_esl_guides_p_t)
			malloc(sizeof(aud_esl_guides_t));
		    modified_guide->audit_action =
			existing_guides_pt1->audit_action;
		    modified_guide->audit_condition =
			existing_guides_pt1->audit_condition &
			~new_guide_pt->audit_condition;
		    modified_guide->ec_list = removed_ecs;
#define LINK_MODIFIED_GUIDES \
		    if (guides_to_be_added == NULL) { \
			guides_to_be_added = modified_guide; \
			guides_to_be_added->next = NULL; \
		    } else { \
			modified_guide->next = guides_to_be_added; \
			guides_to_be_added = modified_guide; \
		    }
LINK_MODIFIED_GUIDES;
		}
		if (existing_guides_pt1->ec_list == NULL) {
		    aud__esl_purge_guide(existing_guides,
					    &existing_guides_pt1,
					    &existing_guides_pt2);
		} else {
		    existing_guides_pt2 = existing_guides_pt1;
		    existing_guides_pt1 = existing_guides_pt1->next;
		}
	    } else {
		existing_guides_pt2 = existing_guides_pt1;
		existing_guides_pt1 = existing_guides_pt1->next;
	    }
	}

    }				/* finished looking at one existing filter */

    /*
     * We are here only if there are still event classes in the new filter
     * left to be added. 
     */

    if (guides_to_be_added == NULL) {
	guides_to_be_added = new_guide_pt;
	guides_to_be_added->next = NULL;
    } else {
	new_guide_pt->next = guides_to_be_added;
	guides_to_be_added = new_guide_pt;
    }
    aud_esl_add_guides(existing_guides, guides_to_be_added);
    /* end of the "left-to-be-done" new filters "if" */
    return;
}


/*
 * Update existing list of guides with a list of new guides.
 */

void
                aud_esl_update_guides
                (
		                 aud_esl_guides_p_t * existing_guides,
		                 aud_esl_guides_p_t new_guides_pt
) {
    aud_esl_guides_p_t new_guide_pt, tmp_guide_pt;

    if (new_guides_pt == NULL) {
	return;
    }

    tmp_guide_pt = new_guides_pt;

    while (tmp_guide_pt != NULL) {
	new_guide_pt = tmp_guide_pt;
	/* The structure pointed by new_guide_pt will be used/destructed 
	 * by aud_esl_update_guides_with_one_guide.
	 */

	tmp_guide_pt = tmp_guide_pt->next;
	aud_esl_update_guides_with_one_guide(existing_guides, new_guide_pt);
    }
}


/*
 * Find a filter entry. 
 */

void           *
                aud_esl_entry_lookup
                (
		                 aud_esl_type_t esl_type,
		                 void ***tables,
		                 uuid_t subject_uuid,
		                 uuid_t cell_uuid,
		                 unsigned16 * probe
) {
    unsigned32          st;
    int                 i;
    unsigned32          string_st;
    unsigned_char_t    *uuid_string;
    dce_error_string_t  error_string;
    int                 print_status;


    *probe = uuid_hash(&subject_uuid, &st) % HASH_TABLE_SIZE;
    if (st != uuid_s_ok) {
        dce_error_inq_text(st, error_string, &print_status);
        uuid_to_string(&subject_uuid, &uuid_string, & string_st );
        if ( string_st != uuid_s_ok ) {
           /* uuid bad from uuid_to string,
              so print unknown instead of uuid garbage */
           strcpy((char *)uuid_string, "?");
        }
        dce_svc_printf(AUD_S_UUID_HASH_FAILED_MSG, (char *)uuid_string, error_string );
        rpc_string_free(&uuid_string, & string_st );
        return(NULL);
    };

    if (FOREIGN_TYPE(esl_type)) {

	/*
	 * Find a foreign esl entry. 
	 */

	aud_esl_foreign_entry_p_t entry_ptr;
	aud_esl_foreign_entry_p_t *table =
	(aud_esl_foreign_entry_p_t *) tables[esl_type];

	entry_ptr = table[*probe];
	while (entry_ptr != NULL) {
	    if (uuid_equal(&entry_ptr->subject_uuid, &subject_uuid, &st) &&
		uuid_equal(&entry_ptr->cell_uuid, &cell_uuid, &st)) {
		return ((void *) entry_ptr);
	    } else {
		entry_ptr = entry_ptr->next;
	    }
	}
    } else {

	aud_esl_entry_p_t entry_ptr;
	aud_esl_entry_p_t *table =
	(aud_esl_entry_p_t *) tables[esl_type];

	entry_ptr = table[*probe];
	while (entry_ptr != NULL) {
	    if (uuid_equal(&entry_ptr->subject_uuid, &subject_uuid, &st)) {
		return ((void *) entry_ptr);
	    } else {
		entry_ptr = entry_ptr->next;
	    }
	}
    }

    /*
     * We are here only if matching entry not found. 
     */

    return (NULL);
}

/*
 * Delete all guides for the specified subject (principal/group/cell). 
 */

void
                aud_esl_entry_delete_filter
                (
		                 aud_esl_type_t esl_type,
		                 void ***tables,
		                 uuid_t subject_uuid,
		                 uuid_t cell_uuid
) {

    void              *entry_ptr;
    unsigned16         probe;
    aud_esl_guides_p_t guides_ptr;

    entry_ptr = aud_esl_entry_lookup(esl_type, tables, subject_uuid,
					cell_uuid, &probe);

    if (entry_ptr != NULL) {
	if (FOREIGN_TYPE(esl_type)) {
	    guides_ptr =
		((aud_esl_foreign_entry_p_t) entry_ptr)->guides;
	    ((aud_esl_foreign_entry_p_t) entry_ptr)->guides = NULL;
	} else {
	    guides_ptr = ((aud_esl_entry_p_t) entry_ptr)->guides;
	    ((aud_esl_entry_p_t) entry_ptr)->guides = NULL;
	}
	aud_esl_free_guides(guides_ptr);
    }
    return;
}


/*
 * Add new filters for the specified local subject (principal or group) or
 * cell to an esl table.  
 */

void
                aud_esl_entry_add_guides
                (
		                 void ***tables,
		                 aud_esl_type_t esl_type,
		                 uuid_t subject_uuid,
		                 uuid_t cell_uuid,
		                 aud_esl_guides_p_t * new_guides,
		                 error_status_t * stp
) {
    unsigned16      probe;

    CLEAR_STATUS(stp);
    if (FOREIGN_TYPE(esl_type)) {
	/*
	 * Add filters for foreign subject to a foreign esl table.  
	 */

	aud_esl_foreign_entry_p_t entry_ptr;
	aud_esl_foreign_entry_p_t *table =
	(aud_esl_foreign_entry_p_t *) tables[esl_type];


	aud_esl_foreign_entry_p_t matching_entry =
	(aud_esl_foreign_entry_p_t)
	aud_esl_entry_lookup(esl_type, tables, subject_uuid,
				cell_uuid, &probe);
	if (matching_entry == NULL) {
	    entry_ptr = (aud_esl_foreign_entry_p_t)
		malloc(sizeof(aud_esl_foreign_entry_t));
	    if (entry_ptr == NULL) {
		*stp = aud_s_cannot_allocate_memory;
		dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, 
				sizeof(aud_esl_foreign_entry_t));
		return;
	    };

	    entry_ptr->subject_uuid = subject_uuid;
	    entry_ptr->cell_uuid = cell_uuid;
	    entry_ptr->guides = NULL;
	    aud_esl_add_guides(&entry_ptr->guides, *new_guides);
	    entry_ptr->next = table[probe];
	    table[probe] = entry_ptr;
	} else {
	    aud_esl_update_guides(&matching_entry->guides, *new_guides);
	}
	return;

    } else {

	aud_esl_entry_p_t entry_ptr;
	aud_esl_entry_p_t *table;

	aud_esl_entry_p_t matching_entry = (aud_esl_entry_p_t)
	aud_esl_entry_lookup(esl_type, tables, subject_uuid,
				cell_uuid, &probe);

	if (matching_entry == NULL) {
	    entry_ptr = (aud_esl_entry_p_t)
		malloc(sizeof(aud_esl_entry_t));
	    if (entry_ptr == NULL) {
		*stp = aud_s_cannot_allocate_memory;
		dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG,
				sizeof(aud_esl_entry_t));
	    };

	    entry_ptr->subject_uuid = subject_uuid;
	    entry_ptr->guides = NULL;
	    aud_esl_add_guides(&entry_ptr->guides, *new_guides);
	    table = (aud_esl_entry_p_t *) tables[esl_type];
	    if (table[probe] == NULL)
		entry_ptr->next = NULL;
	    else
		entry_ptr->next = table[probe]->next;

	    table[probe] = entry_ptr;
	} else {
	    aud_esl_update_guides(&matching_entry->guides, *new_guides);
	}
	return;
    }
}

/*
 * Print a list of guides 
 */

void
                aud_esl_list_guides
                (
		                 aud_esl_guides_p_t guides
) {
    aud_esl_guides_p_t      guides_ptr = guides;
    aud_esl_evt_classes_p_t ec_ptr;

    while (guides_ptr != NULL) {
	dce_svc_printf(AUD_S_ACTION_BIT_SET_MSG, guides_ptr->audit_action);
	dce_svc_printf(AUD_S_CONDITION_BIT_SET_MSG, guides_ptr->audit_condition);
	dce_svc_printf(AUD_S_EVT_CLASSES_MSG);
	if ((ec_ptr = guides_ptr->ec_list) == NULL) {
	    printf("\n"); 
	    return;
	}
	while (ec_ptr != NULL) {
	    printf("%ld ", ec_ptr->evt_class); 
	    ec_ptr = ec_ptr->next;
	}
	printf("\n");
	guides_ptr = guides_ptr->next;
    }
    return;
}

void            aud_esl_entry_list_guides
                (
		                 void ***tables,
		                 aud_esl_type_t esl_type,
		                 uuid_t subject_uuid,
		                 uuid_t cell_uuid
) {
    unsigned16              probe;
    void                   *entry_ptr;
    aud_esl_guides_p_t      guides_ptr;
    aud_esl_evt_classes_p_t ec_ptr;
    unsigned32              st;

    entry_ptr = aud_esl_entry_lookup(esl_type, tables,
					subject_uuid,
					cell_uuid, &probe);
    if (entry_ptr != NULL) {
	if (FOREIGN_TYPE(esl_type)) {
	    guides_ptr =
		((aud_esl_foreign_entry_p_t) entry_ptr)->guides;
	} else {
	    guides_ptr = ((aud_esl_entry_p_t) entry_ptr)->guides;
	}
	aud_esl_list_guides(guides_ptr);
    } else {
	dce_svc_printf(AUD_S_NO_FILTER_MSG);
    }
    return;
}

/* Remove (a2, c2, e2) from (a1, c1, e1): */
/* (a, c, e) is a vector of (audit actions, audit conditions, event classes) */
/* if ((a2 /\ a1 != 0 || (a2 \/ a1 == 0)) && c1 /\ c2 != 0) */
/* Results = (a1, c1, e1 - e2) +                        */
/* [if ((a1 \/ a2 != 0) && (a1 - a2) != 0)]             */
/* (a1 - a2, c1 /\ c2, e1 /\ e2) +                      */
/* [if c1 - c2 != 0 ] (a1, c1 - c2, e1 /\ e2)           */

/*
 * Remove one guide from a list of guides.
 * The result is pointed by the existing_guides parameter.
 * The guide to be removed (pointed by guide_pt) is altered
 * and should not be used after return.
 */

void
                aud_esl_remove_guide
                (
		                 aud_esl_guides_p_t * existing_guides,
		                 aud_esl_guides_p_t guide_pt
) {
    aud_esl_guides_p_t      existing_guides_pt1, existing_guides_pt2;
    aud_esl_evt_classes_p_t removed_ecs = NULL;
    aud_esl_evt_classes_p_t dup_ecs = NULL;
    aud_esl_guides_p_t      guides_to_be_added = NULL;
    aud_esl_guides_p_t      modified_guide;

    if ((*existing_guides) == NULL) {
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_esl, svc_c_debug1,
	    "aud_esl_remove_guide: There is no existing guide.\n"));
#endif
	return;
    }

    existing_guides_pt1 = existing_guides_pt2
	= *existing_guides;


next_existing_filter:
    while (existing_guides_pt1 != NULL) {
	if (((guide_pt->audit_action | existing_guides_pt1->audit_action) == 0
	     ||
	  (guide_pt->audit_action & existing_guides_pt1->audit_action) != 0)
	    &&
	    (guide_pt->audit_condition &
	     existing_guides_pt1->audit_condition) != 0) {

	    aud__esl_remove_ecs(guide_pt->ec_list, TRUE,
				   &existing_guides_pt1->ec_list,
				   &removed_ecs);
	    if (removed_ecs != NULL) {
		/* if !(a1 == 0 && a2 == 0) and (a1 -a2) != 0 */
		if ((existing_guides_pt1->audit_action |
		     guide_pt->audit_action) != 0
		    &&
		    (existing_guides_pt1->audit_action &
		     ~guide_pt->audit_action) != 0) {
		    modified_guide = (aud_esl_guides_p_t)
			malloc(sizeof(aud_esl_guides_t));
		    modified_guide->audit_action =
			existing_guides_pt1->audit_action &
			~guide_pt->audit_action;
		    modified_guide->audit_condition =
			existing_guides_pt1->audit_condition &
			guide_pt->audit_condition;
		    modified_guide->ec_list = removed_ecs;
		    /*
		     * We have used the physical copy of the removed_ecs.
		     * Duplicate it now so it can be used again. 
		     */
		    LINK_MODIFIED_GUIDES;
		}
		/* if c1 - c2 != 0 */
		if ((existing_guides_pt1->audit_condition &
		     ~guide_pt->audit_condition) != 0) {
		    aud__esl_dup_ec_list(removed_ecs, &dup_ecs);
		    modified_guide = (aud_esl_guides_p_t)
			malloc(sizeof(aud_esl_guides_t));
		    modified_guide->audit_action =
			existing_guides_pt1->audit_action;
		    modified_guide->audit_condition =
			existing_guides_pt1->audit_condition &
			~guide_pt->audit_condition;
		    modified_guide->ec_list = dup_ecs;
		    LINK_MODIFIED_GUIDES;
		}
		if (existing_guides_pt1->ec_list == NULL) {
		    aud__esl_purge_guide(existing_guides,
					    &existing_guides_pt1,
					    &existing_guides_pt2);
		    goto next_existing_filter;
		}
	    }
	}
	existing_guides_pt2 = existing_guides_pt1;
	existing_guides_pt1 = existing_guides_pt1->next;
    }				/* end of the existing filters "while" loop */

    aud_esl_add_guides(existing_guides, guides_to_be_added);
    return;
}

/*
 * Remove a list of guides from an existing list of guides.
 * The result is pointed by the existing_guides parameter.
 * The list of guides to be removed (pointed by guide_pt) is altered
 * and should not be used after return.
 */

void
                aud_esl_remove_guides
                (
		                 aud_esl_guides_p_t * existing_guides,
		                 aud_esl_guides_p_t guides_pt
) {
    aud_esl_guides_p_t guide_pt, tmp_guide_pt;

    guide_pt = guides_pt;

    while (*existing_guides != NULL && guide_pt != NULL) {
	tmp_guide_pt = guide_pt->next;
	aud_esl_remove_guide(existing_guides, guide_pt);
	guide_pt = tmp_guide_pt;
    }
    return;
}
    

/*
 * Remove a filter.
 */

void            aud_esl_entry_remove_guides
                (
		                 void ***tables,
		                 aud_esl_type_t esl_type,
		                 uuid_t subject_uuid,
		                 uuid_t cell_uuid,
		                 aud_esl_guides_p_t guides
) {
    unsigned16      probe;
    void           *entry_ptr;

    entry_ptr = aud_esl_entry_lookup(esl_type, tables,
					subject_uuid,
					cell_uuid, &probe);

    if (entry_ptr != NULL) {
	if (FOREIGN_TYPE(esl_type)) {
	    aud_esl_remove_guides(
			&((aud_esl_foreign_entry_p_t) entry_ptr)->guides,
				     guides);
	} else {
	    aud_esl_remove_guides(
		       &((aud_esl_entry_p_t) entry_ptr)->guides, guides);
	}
    } else {
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_esl, svc_c_debug1,
	    "aud_esl_entry_remove_guides: Entry not found\n"));
#endif
    }
    return;
}

void            aud_esl_purge_entries
                (
		                 aud_esl_type_t esl_type,
		                 void *entry
) {
    if              (FOREIGN_TYPE(esl_type)) {
	aud_esl_foreign_entry_p_t entry_ptr_next;
	aud_esl_foreign_entry_p_t entry_ptr = (aud_esl_foreign_entry_p_t) entry;
	while (entry_ptr != NULL) {
	    aud_esl_free_guides(entry_ptr->guides);
	    entry_ptr_next = entry_ptr->next;
	    free(entry_ptr);
	    entry_ptr = entry_ptr_next;
	}
    } else {
	aud_esl_entry_p_t entry_ptr_next;
	aud_esl_entry_p_t entry_ptr = (aud_esl_entry_p_t) entry;
	while (entry_ptr != NULL) {
	    aud_esl_free_guides(entry_ptr->guides);
	    entry_ptr_next = entry_ptr->next;
	    free(entry_ptr);
	    entry_ptr = entry_ptr_next;
	}
    }
    return;
}


void            aud_esl_clean_table
                (
		                 void ***tables,
		                 aud_esl_type_t esl_type
) {
    int             i;
    void          **table = tables[esl_type];

    for (i = 0; i < HASH_TABLE_SIZE; i++) {
	if (table[i] != NULL) {
	    aud_esl_purge_entries(esl_type, table[i]);
	    table[i] = NULL;
	}
    }
    return;
}
