/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_attr.c,v $
 * Revision 1.1.10.1  1996/10/03  15:12:39  arvind
 * 	Add lookup1_by_uuid to allow "easy" fetches by
 * 	attribute UUID.
 * 	[1996/09/16  23:24 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.8.3  1996/02/18  00:21:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:54  marty]
 * 
 * Revision 1.1.8.2  1995/12/08  17:58:34  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/01  18:38 UTC  mdf
 * 	Resolve CHFts15067
 * 	[1995/04/28  13:22 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/3]
 * 
 * 	Resolve CHFts14561
 * 	[1995/04/13  21:33 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/2]
 * 
 * 	Changes for CHFts14568
 * 	[1995/04/10  15:05 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/1]
 * 
 * 	HP revision /main/HPDCE02/mdf_mothra_defects_3/4  1995/05/01  17:25 UTC  mdf
 * 	Fix defect.
 * 
 * 	HP revision /main/HPDCE02/mdf_mothra_defects_3/3  1995/04/28  13:22 UTC  mdf
 * 	Resolve CHFts15067
 * 
 * 	HP revision /main/HPDCE02/mdf_mothra_defects_3/2  1995/04/13  21:33 UTC  mdf
 * 	Resolve CHFts14561
 * 
 * 	HP revision /main/HPDCE02/mdf_mothra_defects_3/1  1995/04/10  15:05 UTC  mdf
 * 	Changes for CHFts14568
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/02  17:01 UTC  mullan_s
 * 
 * 	HP revision /main/mullan_mothra_attrlist_fix/1  1994/12/02  16:38 UTC  mullan_s
 * 	rsdb_attr_list_is_member: set free_list_id to last good entry of attr_list_id if
 * 	end of attr list is reached. Prevents secd from crashing if list.count
 * 	> RSDB_MAX_ATTR_LIST_SIZE.
 * 	[1995/12/08  17:22:03  root]
 * 
 * Revision 1.1.2.12  1994/09/09  17:29:35  mdf
 * 	Added status checking
 * 	[1994/09/09  17:08:36  mdf]
 * 
 * 	Updates for CR 12099
 * 	[1994/09/09  14:53:39  mdf]
 * 
 * 	Cleaning up after review.
 * 	[1994/09/07  10:50:44  mdf]
 * 
 * Revision 1.1.2.11  1994/08/25  21:40:01  sekhar
 * 	Put back SVC messages for error_st() and fatal_st() calls.
 * 	[1994/08/25  21:39:12  sekhar]
 * 
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 
 * Revision 1.1.3.3  94/08/25  10:13:51  aha
 * 	OT11755: 64 bit porting changes
 * 
 * Revision 1.1.2.10  1994/08/22  19:52:22  mdf
 * 	Added NULL just to be safe
 * 	[1994/08/22  19:21:39  mdf]
 * 
 * 	CR 11616 Added functionality to calculate the total number of
 * 	of entires ofr an initial cursor in the rsdb_attr_lookup_by_uuid
 * 	routine.
 * 	[1994/08/22  19:15:16  mdf]
 * 
 * Revision 1.1.2.9  1994/08/16  10:36:50  mdf
 * 	Bug fixes from Hewlett Packard
 * 	[1994/08/15  19:51:45  mdf]
 * 
 * Revision 1.1.2.8  1994/08/08  14:23:44  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/04  19:23:59  mdf]
 * 
 * 	fix core dump
 * 	[1994/08/01  18:55:21  mdf]
 * 
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:13:12  mdf]
 * 
 * Revision 1.1.2.6  1994/07/15  15:02:40  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:23:01  mdf]
 * 
 * Revision 1.1.2.5  1994/06/17  18:42:52  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:19:55  devsrc]
 * 
 * Revision 1.1.2.4  1994/06/02  21:20:49  mdf
 * 	Fixed memory leak.
 * 	[1994/05/31  12:35:24  mdf]
 * 
 * Revision 1.1.2.3  1994/05/11  19:32:34  ahop
 * 	hp_sec_to_osf_2 drop
 * 	 Support for logging attribute updates
 * 	[1994/04/29  21:47:40  ahop]
 * 
 * $EndLog$
 */

#include <dce/dce.h>
#include <dce/rpc.h>
#include <dce/uuid.h>
#include <dce/sec_attr_util.h>
#include <dce/sec_attr_tools.h>

#include <stdio.h>
#include <rsdb.h>
#include <rsdb_util.h>
#include <rsdb_attr.h>		/* Prototypes */
#include <rsdb_attr_pvt.h>	/* macros shared with rsdb_attr_schema */
#include <rs_util.h>
#include <rs_state.h>

#include <sec_svc.h>

/* Macros */

/* 
 * ATTR_ALLOC: allocate P of length L.
 * If successful, memset buffer to zero.
 * Calling routine should set "allocator" to rpc_ss_allocate
 * if the memory must be freed automatically by the server stub code.
 *
 * P is a pointer to be allocated
 * L is the length to be allocated
 * STP is a pointer to an error_status_t
 */
#define ATTR_ALLOC(P,L,STP)	{	\
    if(!((P) = RSDB_ATTR_ALLOCATE(allocator,(L)))) {	\
	SET_STATUS(STP, sec_attr_no_memory);	\
    } else {					\
	memset((P), NULL, (L));			\
    } }

/* 
 * ATTR_ITEM_ALLOC: allocate P of length L.
 * If successful, memset buffer to zero.
 * Else, free all previously allocated ptrs in S and return.
 * Calling routine should set "allocator" to rpc_ss_allocate
 * if the memory must be freed automatically by the server stub code.
 * To be used by routines that build attribute instances.
 *
 * S is a pointer to a sec_attr_t
 * P is a pointer to be allocated
 * L is the length to be allocated
 * STP is a pointer to an error_status_t
 */
#define ATTR_ITEM_ALLOC(S,P,L,STP)	{	\
    if(!((P) = RSDB_ATTR_ALLOCATE(allocator,(L)))) {	\
	SET_STATUS(STP, sec_attr_no_memory);	\
	if(!(allocator)) {			\
	    sec_attr_util_inst_free_ptrs((S));	\
	}					\
	return;					\
    } else {					\
	memset((P), NULL, (L));			\
    } }
	
/* ATTR_SEQ_LIST_TAIL macros.
 * H is a pointer to an rsdb_attr_inst_t that is
 * the head of an attribute instance sequential list.
 */
#define ATTR_SEQ_LIST_TAIL_PREV(H)	((H)->threads.prev)

/* ATTR_TYPE_ID - return the attr_type_id (same as the
 * seq_id of the sch_entry for the attr_inst) stored in this attr_inst.
 * A is a pointer to an rsdb_attr_inst_t.
 */
#define ATTR_TYPE_ID(A)	((A)->threads.alias)


/*
 * PRIVATE OPERATIONS
 */

/*
 * r s d b _ a t t r _ o b j _ u p d _ n u m _ a t t r s
 *
 * Update the stored "current number of attribute instances"
 * for the object identified by domain and obj_id.
 * Add num to the current num_attrs.  (To subtract, num should be negative.)
 * Save the object where this info is stored.
 */
PRIVATE void rsdb_attr_obj_upd_num_attrs(
    sec_rgy_domain_t		domain,		/* [in] */
    rsdb_pvt_id_t		obj_id,		/* [in] */
    signed32			num,		/* [in] */
    error_status_t		*st_p)
{
    rsdb_named_item_t		object;

    CLEAR_STATUS(st_p);

    /* Check special case object ids first */
    if(obj_id == RSDB_POLICY_OBJ_ID) {
	rsdb_policy_upd_num_attrs(obj_id, num);
        return;
    } else if(obj_id == RSDB_REPLIST_OBJ_ID) {
        SET_STATUS(st_p, sec_attr_not_implemented);
        return;
    } else if(obj_id == RSDB_ATTR_SCHEMA_OBJ_ID) {
        SET_STATUS(st_p, sec_attr_not_implemented);
        return;
    }

    /* It must be a PGO or DIR object */
    if (!rsdb_util_get_item_by_id(domain, obj_id, &object)) {
        SET_STATUS(st_p, sec_attr_rgy_obj_not_found);
        return;
    }
    /* Polymorphic objects default to PGO wrt ERAs */
    if (RSDB_IS_PGO(&object)) {
	if ((RSDB_PGO(&object).num_attrs += num) < 0)
            dce_svc_printf(SEC_ATTR_NUM_ATTR_LTZERO_MSG);
    }
    else {
	if ((RSDB_DIR(&object).num_attrs += num) < 0)
            dce_svc_printf(SEC_ATTR_NUM_ATTR_LTZERO_MSG);
    }
    rsdb_util_store_item_by_id(domain, obj_id, &object);
}


/*
 * r s d b _ a t t r _ o b j _ g e t _ n u m _ a t t r s
 *
 * Retrieve the stored "current number of attribute instances"
 * for the object identified by rgy_obj_info->obj_id.
 */
PRIVATE void rsdb_attr_obj_get_num_attrs(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,  /* [in] */
    signed32			*num,		/* [out] */
    error_status_t		*st_p)
{
    rsdb_named_item_t		object;

    CLEAR_STATUS(st_p);

    /* Check special case object ids first */
    if(rgy_obj_info->obj_id == RSDB_POLICY_OBJ_ID) {
	rsdb_policy_get_num_attrs(rgy_obj_info->obj_id, num);
	return;
    } else if(rgy_obj_info->obj_id == RSDB_REPLIST_OBJ_ID) {
	SET_STATUS(st_p, sec_attr_not_implemented);
	return;
    } else if(rgy_obj_info->obj_id == RSDB_ATTR_SCHEMA_OBJ_ID) {
	SET_STATUS(st_p, sec_attr_not_implemented);
	return;
    }
    /* It must be a PGO or DIR object */
    if (!rsdb_util_get_item_by_id(rgy_obj_info->domain, 
	rgy_obj_info->obj_id, &object)) {
        SET_STATUS(st_p, sec_attr_rgy_obj_not_found);
        return;
    }
    /* Polymorphic objects default to PGO wrt ERAs */
    *num = RSDB_IS_PGO(&object) ?
		RSDB_PGO(&object).num_attrs :
		RSDB_DIR(&object).num_attrs;
}


/*
 * r s d b _ a t t r _ o b j _ s e t _ a t t r _ l i s t _ i d
 * 
 * Set the attr_list_id for the object identified by
 * rgy_obj_info->obj_id to rgy_obj_info->attr_list_id.
 * Save the object where this info is stored.
 */
PRIVATE void rsdb_attr_obj_set_attr_list_id(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,  /* [in] */
    error_status_t		*st_p
)
{
    rsdb_named_item_t		object;

    CLEAR_STATUS(st_p);

    /* Check special case object ids first */
    if(rgy_obj_info->obj_id == RSDB_POLICY_OBJ_ID) {
	rsdb_policy_set_attr_list_id(rgy_obj_info->obj_id, 
	    rgy_obj_info->attr_list_id);
        return;
    } else if(rgy_obj_info->obj_id == RSDB_REPLIST_OBJ_ID) {
        SET_STATUS(st_p, sec_attr_not_implemented);
        return;
    } else if(rgy_obj_info->obj_id == RSDB_ATTR_SCHEMA_OBJ_ID) {
        SET_STATUS(st_p, sec_attr_not_implemented);
        return;
    }

    /* It must be a PGO or DIR object */
    if (!rsdb_util_get_item_by_id(rgy_obj_info->domain, 
	rgy_obj_info->obj_id, &object)) {
        SET_STATUS(st_p, sec_attr_rgy_obj_not_found);
        return;
    }
    /* Polymorphic objects default to PGO wrt ERAs */
    if (RSDB_IS_PGO(&object))
	RSDB_PGO(&object).attr_list_id = rgy_obj_info->attr_list_id;
    else
	RSDB_DIR(&object).attr_list_id = rgy_obj_info->attr_list_id;

    rsdb_util_store_item_by_id(rgy_obj_info->domain, rgy_obj_info->obj_id,
	&object);
}


/*
 * r s d b _ a t t r _ o b j _ g e t _ a t t r _ l i s t _ i d
 * 
 * Get the attr_list_id for the object identified by obj_id.
 */
PRIVATE void rsdb_attr_obj_get_attr_list_id(
    sec_rgy_domain_t		domain,		/* [in] */
    rsdb_pvt_id_t		obj_id,		/* [in] */
    rsdb_pvt_id_t		*attr_list_id,	/* [out] */
    error_status_t		*st_p
)
{
    rsdb_named_item_t		object;

    CLEAR_STATUS(st_p);

    /* Check special case object ids first */
    if(obj_id == RSDB_POLICY_OBJ_ID) {
	rsdb_policy_get_attr_list_id(obj_id, attr_list_id);
        return;
    } else if(obj_id == RSDB_REPLIST_OBJ_ID) {
        SET_STATUS(st_p, sec_attr_not_implemented);
        return;
    } else if(obj_id == RSDB_ATTR_SCHEMA_OBJ_ID) {
        SET_STATUS(st_p, sec_attr_not_implemented);
        return;
    }

    /* It must be a PGO or DIR object */
    if (!rsdb_util_get_item_by_id(domain, obj_id, &object)) {
        SET_STATUS(st_p, sec_attr_rgy_obj_not_found);
        return;
    }
    /* Polymorphic objects default to PGO wrt ERAs */
    if (RSDB_IS_PGO(&object))
	*attr_list_id = RSDB_PGO(&object).attr_list_id;
    else
	*attr_list_id = RSDB_DIR(&object).attr_list_id;
}


/*
 * r s d b _ a t t r _ p u r g e
 *
 * Delete the attribute instance record 
 * identified by attr_inst_id from the 
 * attributes database.
 *
 */
PRIVATE void rsdb_attr_purge (
    rsdb_pvt_id_t	attr_inst_id,
    error_status_t	*st_p
)
{
    rsdb_sequential_key_t       sequential_key;

    CLEAR_STATUS(st_p);
    memset((Pointer) &sequential_key, NULL, sizeof(rsdb_sequential_key_t));

    sequential_key.record_type  = SEQUENTIAL_TYPE;
    sequential_key.id           = attr_inst_id;

    rsdb_delete(rsdb_attr_db, (Pointer) &sequential_key,
        sizeof(rsdb_sequential_key_t), st_p);
}


/*
 * r s d b _ a t t r _ l i s t _ s t o r e
 *
 * Store the input rsdb_attr_list_t by sequential id
 * in the ATTR database.
 * This routine either succeeds or fails with fatal error.
 */
PRIVATE void rsdb_attr_list_store(
    rsdb_pvt_id_t   		attr_list_id,
    rsdb_max_attr_list_t	*attr_list)
{
    rsdb_sequential_key_t	sequential_key;
    error_status_t		st, *st_p = &st;

    memset((Pointer) &sequential_key, NULL, sizeof(rsdb_sequential_key_t));

    sequential_key.record_type	= SEQUENTIAL_TYPE;
    sequential_key.id 		= attr_list_id;

    rsdb_replace(rsdb_attr_db, (Pointer) &sequential_key,
	sizeof(rsdb_sequential_key_t), (Pointer) attr_list, 
	RSDB_ATTR_LIST_SIZE(attr_list), st_p);
}


/*
 * r s d b _ a t t r _ l i s t _ p u r g e
 *
 * Delete the attribute list record 
 * from the attributes database.
 *
 */
PRIVATE void rsdb_attr_list_purge (
    rsdb_pvt_id_t   		attr_list_id	/* [in] */
)
{
    error_status_t		st;

    rsdb_sequential_key_t       sequential_key;

    memset((Pointer) &sequential_key, NULL, sizeof(rsdb_sequential_key_t));

    sequential_key.record_type  = SEQUENTIAL_TYPE;
    sequential_key.id           = attr_list_id;

    rsdb_delete(rsdb_attr_db, (Pointer) &sequential_key,
        sizeof(rsdb_sequential_key_t), &st);
}


/*
 * r s d b _ a t t r _ l i s t _ a d d _ i n s t
 *
 * Add the attr_inst_id of attr_p into the attr_list stored 
 * for its object.  Create an attr_list
 * if one doesn't already exist for the obj_id.
 *
 */
PRIVATE void rsdb_attr_list_add_inst(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,  /* [in] */
    rsdb_attr_inst_t		*attr_p)	/* [in] */
{
    rsdb_sequential_key_t	sequential_key;
    rsdb_max_attr_list_t	attr_list;
    rsdb_pvt_id_t               attr_inst_id;
    rsdb_pvt_id_t               attr_list_id;
    long                        i;
    long                        slot;
    error_status_t		st, *st_p=&st;

    attr_inst_id = attr_p->threads.id;
    attr_list_id = rgy_obj_info->attr_list_id;

    /* If list doesn't exist for this object, create it */
    if(attr_list_id == END_OF_LIST) {
	/* create attr_list */
	rgy_obj_info->attr_list_id	= rsdb_new_sequential_id();
	attr_list_id			= rgy_obj_info->attr_list_id;
	attr_list.next			= END_OF_LIST;
	attr_list.total_count		= 0;
	attr_list.count			= 0;

	/* Since this is the first attr_list created for this object
	 * store the attr_list_id with the object.
	 */
	rsdb_attr_obj_set_attr_list_id(rgy_obj_info, st_p);
	if(BAD_STATUS(st_p)) {
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_RSDB_ATTR_SET_ID_MSG, *st_p);
	}
    } else {
	if(rsdb_attr_list_is_member(attr_list_id, attr_inst_id, 
	    &attr_list_id)) {
	    return;
	}
	if(!(rsdb_attr_list_get_by_id(attr_list_id, &attr_list))) {
            dce_svc_printf(SEC_ATTR_CANT_GET_ATTRLIST_MSG);
	}
	if(attr_list.count == RSDB_MAX_ATTR_LIST_SIZE) {
            /*
             * attr_list is full, add another list 
             */
            if (attr_list.next != END_OF_LIST) {
                dce_svc_printf(SEC_RSDB_LIST_NOT_TERMINATED_MSG);
            }
            attr_list.next = rsdb_new_sequential_id();
            rsdb_attr_list_store(attr_list_id, &attr_list);

            attr_list_id = attr_list.next;
            attr_list.next = END_OF_LIST;
            attr_list.total_count = 0;
            attr_list.count = 0;
        }
    }
    /*
     * locate the next available slot in the attribute list
     */
    if (attr_list.count == attr_list.total_count) {
        /*
         * the attr_list contains no deleted elements 
	 * (possibly because we just created it).
         */
        slot = attr_list.total_count++;
    } else {
        /*
         * the attr_list contains deleted elements
         */
        slot = -1;
        for (i = 0; i < attr_list.total_count; i++) {
            if (attr_list.attr[i].inst_id.deleted) {
                slot = i;
                break;
            }
        }
        if (slot == -1)
            dce_svc_printf(SEC_RSDB_NO_OPEN_SLOT_MSG);
    }

    attr_list.attr[slot].type_id = ATTR_TYPE_ID(attr_p);
    attr_list.attr[slot].inst_id.id = attr_inst_id;
    attr_list.attr[slot].inst_id.deleted = 0;
    attr_list.count++;
    rsdb_attr_list_store(attr_list_id, &attr_list);
}


/*
 * r s d b _ a t t r _ l i s t _ d e l e t e _ i n s t
 *
 * Delete the attr_inst_id from the attr_list identified
 * by attr_list_head.
 *
 */
PRIVATE boolean32 rsdb_attr_list_delete_inst(
    rsdb_pvt_id_t               attr_inst_id,	/* [in] */
    rsdb_pvt_id_t               attr_list_head	/* [in] */
)
{
    rsdb_max_attr_list_t	attr_list;
    long                        i;
    rsdb_pvt_id_t               attr_list_id;

    attr_list_id = attr_list_head;

    while(attr_list_id != END_OF_LIST) {
	if(!(rsdb_attr_list_get_by_id(attr_list_id, &attr_list))) {
            dce_svc_printf(SEC_ATTR_CANT_GET_ATTRLIST_MSG);
	}
        for (i = 0; i < attr_list.total_count; i++) {
            if (!attr_list.attr[i].inst_id.deleted && 
		attr_list.attr[i].inst_id.id == attr_inst_id) {
                attr_list.attr[i].inst_id.deleted = 1;
                attr_list.count--;
                rsdb_attr_list_store(attr_list_id, &attr_list);
                return true;
            }
        }
        if (attr_list.next == END_OF_LIST)
            break;
        attr_list_id = attr_list.next;
    }
    return false;
}


/*
 * r s d b _ a t t r _ s e q _ l i s t _ a d d _ i n s t
 *
 * Thread the input attribute instance (attr_p) on
 * the sequential list for the schema_entry of its type.
 *
 */
PRIVATE void rsdb_attr_seq_list_add_inst(
    rsdb_attr_inst_t		*attr_p)	/* [in] */
{
    rsdb_attr_inst_t		*head;
    rsdb_attr_inst_t		*prev;
    error_status_t		st, *st_p=&st;

    /* look up the head of the seq list using attr_type_id (alias) */
    if(!(rsdb_attr_get_by_id(ATTR_TYPE_ID(attr_p), &head, st_p))) {
        dce_svc_printf(SEC_ATTR_CANT_GET_INSTANCE_MSG);
    }
    attr_p->threads.next = END_OF_LIST;
    attr_p->threads.prev = ATTR_SEQ_LIST_TAIL_PREV(head);
    ATTR_SEQ_LIST_TAIL_PREV(head) = attr_p->threads.id;

    if(attr_p->threads.prev != head->threads.id) {
	/* look up the last attr_inst in the seq list using tail.prev */
	if(!(rsdb_attr_get_by_id(attr_p->threads.prev, &prev, st_p))) {
            dce_svc_printf(SEC_ATTR_CANT_GET_INSTANCE_MSG);
	}
	prev->threads.next = attr_p->threads.id;
	rsdb_attr_store_by_id(prev->threads.id, prev);
	free(prev);
    } else {
	head->threads.next = attr_p->threads.id;
    }
    rsdb_attr_store_by_id(head->threads.id, head);
    free(head);
}


/*
 * r s d b _ a t t r _ s e q _ l i s t _ d e l e t e _ i n s t
 *
 * Remove the input attribute instance (attr_p) from
 * the sequential list for the schema_entry of its type.
 *
 */
PRIVATE void rsdb_attr_seq_list_delete_inst(
    rsdb_attr_inst_t		*attr_p		/* [in] */
)
{
    rsdb_attr_inst_t		*prev;
    rsdb_attr_inst_t		*next;
    rsdb_pvt_id_t		next_id;
    boolean32			last_inst = false;
    error_status_t		st, *st_p=&st;

    /* look up the prev instance in the seq list */
    if(!(rsdb_attr_get_by_id(attr_p->threads.prev, &prev, st_p))) {
        dce_svc_printf(SEC_ATTR_CANT_GET_INSTANCE_MSG);
    }
    /* special case deleting last attr_inst on list */
    if( attr_p->threads.prev == ATTR_TYPE_ID(attr_p) &&
	attr_p->threads.next == END_OF_LIST ) {
	/* prev is the head/tail; set tail.prev */
	ATTR_SEQ_LIST_TAIL_PREV(prev) = attr_p->threads.prev;
	last_inst = true;
    }
    prev->threads.next = attr_p->threads.next;
    rsdb_attr_store_by_id(prev->threads.id, prev);
    free(prev);

    if(last_inst) {
	/* prev and next are the same object, so we're done */
	return;
    }

    /* if attr_p->threads.next is END_OF_LIST, retrieve the "head/tail",
     * otherwise, retrieve threads.next
     */
    if(attr_p->threads.next == END_OF_LIST) {
	next_id = ATTR_TYPE_ID(attr_p);
    } else {
	next_id = attr_p->threads.next;
    }

    /* look up the next instance in the seq list */
    if(!(rsdb_attr_get_by_id(next_id, &next, st_p))) {
        dce_svc_printf(SEC_ATTR_CANT_GET_INSTANCE_MSG);
    }
    next->threads.prev = attr_p->threads.prev;
    rsdb_attr_store_by_id(next->threads.id, next);
    free(next);
}


/*
 * r s d b _ a t t r _ d e l e t e _ i n s t
 *
 * Delete the attr instance, identified by attr_inst_id, 
 * from the attr database and remove it from any lists 
 * that reference it.
 *
 * Decrement the num_attrs field of the object to which it is attached.
 *
 * Status Codes:
 *	sec_attr_inst_not_found	if attr not found with attr_id.
 *	error_status_ok		otherwise.
 */
PRIVATE void rsdb_attr_delete_inst(
    rsdb_util_rgy_obj_info_t	*rgy_obj_info,  /* [in] */
    rsdb_pvt_id_t		attr_inst_id,	/* [in] */
    error_status_t		*st_p
)
{
    rsdb_attr_inst_t	*attr_p;

    CLEAR_STATUS(st_p);

    if (!(rsdb_attr_get_by_id(attr_inst_id, &attr_p, st_p))) {
	return;
    }
    /* Remove this attr_inst from the seq_list for its attr_type */
    rsdb_attr_seq_list_delete_inst(attr_p);

    /* Remove the attr_inst seq_id from the attr_list for
     * this object and attr_type.
     */
    rsdb_attr_list_delete_inst(attr_p->threads.id, rgy_obj_info->attr_list_id);

    /* Decrement the num_attrs field on the object to
     * which this attribute was attached.
     */
    rsdb_attr_obj_upd_num_attrs(rgy_obj_info->domain, 
	rgy_obj_info->obj_id, -1, st_p);

    /* delete the attr_inst record */
    rsdb_attr_purge(attr_p->threads.id, st_p);

    free(attr_p);
}


/* r s d b _ a t t r _ a l l o c _ d b _ i n s t
 *
 * Malloc an rsdb_attr_inst_t buffer large enough to 
 * hold the fixed and variable length parts of the input 
 * sec_attr_t, pointed to by sec_attr_p.
 * Set the attribute item length field in db_attr_p.
 * Buffer allocated for db_attr_p must be freed by caller.
 *
 * INPUT:
 *	sec_attr_p - points to a sec_attr_t to
 *		be "measured for size."  It is assumed that this
 *		attribute has already been checked for
 *		semantic correctness.
 * OUTPUT:
 *	db_attr_p - an allocated buffer large enough to
 *		contain the input attribute in rsdb format.
 *
 * Status Codes:
 *      sec_attr_no_memory    if malloc fails.
 *      error_status_ok                 otherwise.
 */
PRIVATE void rsdb_attr_alloc_db_inst
    (
    sec_attr_t			*sec_attr_p,	/* [in] */
    rsdb_attr_inst_t		**db_attr_p,	/* [out] */
    error_status_t		*st_p
    )
{
    int                 i, j;
    signed32		attr_inst_len;
    unsigned32		length;
    Pointer		new_buf;

    CLEAR_STATUS(st_p);
    *db_attr_p = (rsdb_attr_inst_t *)NULL;

    rsdb_attr_calc_size_value(sec_attr_p, &attr_inst_len, st_p);

    /* If value has size > 0, subtract 1 since one char
     * is already accounted for in the buf[VARYING]
     * field of rsdb_attr_inst_t
     */
    if(attr_inst_len > 0) {
        attr_inst_len -= 1;
    }

    /* Add size of full structure */
    attr_inst_len += sizeof(rsdb_attr_inst_t);

    if(!(new_buf = malloc(attr_inst_len))) {
        SET_STATUS(st_p, sec_attr_no_memory);
        return;
    }
    *db_attr_p = (rsdb_attr_inst_t *) new_buf;
    memset((Pointer)(*db_attr_p), NULL, attr_inst_len);
    /* store total length of db_attr_p buffer. */
    (*db_attr_p)->attr_inst_len = attr_inst_len;

    return;
}


/* r s d b _ a t t r _ c a l c _ s i z e _ v a l u e
 *
 * Calculate the size required to store the attribute value
 * data (not including encoding type) from the
 * input sec_attr_t in an rsdb_attr_inst_t.
 *
 * INPUT:
 *	sec_attr_p - points to a sec_attr_t to
 *		be "measured for size."  It is assumed that this
 *		attribute has already been checked for
 *		semantic correctness.
 * OUTPUT:
 *	size - number of bytes required for a buffer to
 *		store the attribute value portion of
 *		an rsdb_attr_inst_t containing the
 *		data from sec_attr_p.
 *
 */
PUBLIC void rsdb_attr_calc_size_value
    (
    sec_attr_t			*sec_attr_p,	/* [in] */
    signed32			*size,		/* [out] */
    error_status_t		*st_p
    )
{
    int                 i, j;
    long		attr_inst_len = 0;
    unsigned32		length;

    CLEAR_STATUS(st_p);
    *size = 0;

    switch(SA_ATTR_ENCODING(sec_attr_p)) {
    case sec_attr_enc_any:
    case sec_attr_enc_void:
	break;
    case sec_attr_enc_integer:
	attr_inst_len += sizeof(SA_ATTR_INTEGER(sec_attr_p));
	break;
    case sec_attr_enc_printstring:
	attr_inst_len += sizeof(length); /* store string length */
	attr_inst_len += u_strlen(SA_ATTR_PRINTSTRING_P(sec_attr_p));
	break;
    case sec_attr_enc_printstring_array:
	if(SA_ATTR_STR_ARRAY_P(sec_attr_p)) {
	    attr_inst_len += sizeof(length); /* store number of strings */
	    for(i = 0; i < SA_ATTR_STR_ARRAY_NUM(sec_attr_p); i++) {
	        attr_inst_len += sizeof(length); /* store string length */
	        attr_inst_len += 
		    u_strlen(SA_ATTR_STR_ARRAY_ELT_P(sec_attr_p, i));
	    }
	}
	break;
    case sec_attr_enc_bytes:
    case sec_attr_enc_confidential_bytes:
	if(SA_ATTR_BYTES_P(sec_attr_p)) {
	    attr_inst_len += sizeof(length); /* store byte array length */
	    attr_inst_len += SA_ATTR_BYTES_LEN(sec_attr_p);
	}
	break;
    case sec_attr_enc_i18n_data:
	if(SA_ATTR_IDATA_P(sec_attr_p)) {
	    attr_inst_len += sizeof(SA_ATTR_IDATA_CODESET(sec_attr_p));
	    attr_inst_len += sizeof(length); /* store byte array length */
	    attr_inst_len += SA_ATTR_IDATA_LEN(sec_attr_p);
	}
	break;
    case sec_attr_enc_uuid:
	attr_inst_len += sizeof(SA_ATTR_UUID(sec_attr_p));
	break;
    case sec_attr_enc_attr_set:
	if(SA_ATTR_SET_P(sec_attr_p)) {
	    attr_inst_len += sizeof(length); /* store number of members */
	    attr_inst_len += ( SA_ATTR_SET_NUM(sec_attr_p) * 
		sizeof(SA_ATTR_SET_MEMBERS(sec_attr_p, 0)) );
	}
	break;
    case sec_attr_enc_binding:
	if(SA_ATTR_BND_INFO_P(sec_attr_p)) {
	    attr_inst_len += sizeof(SA_ATTR_BND_AUTH_INFO_TYPE(sec_attr_p));
	    if(SA_ATTR_BND_AUTH_INFO_TYPE(sec_attr_p) == 
		sec_attr_bind_auth_dce) {
	        attr_inst_len += sizeof(length); /* store svr_princ name len */
	        attr_inst_len += 
		    u_strlen(SA_ATTR_BND_AUTH_SVR_PNAME_P(sec_attr_p));
	        attr_inst_len += 
		    sizeof(SA_ATTR_BND_AUTH_PROT_LEVEL(sec_attr_p));
	        attr_inst_len += 
		    sizeof(SA_ATTR_BND_AUTH_AUTHN_SVC(sec_attr_p));
	        attr_inst_len += 
		    sizeof(SA_ATTR_BND_AUTH_AUTHZ_SVC(sec_attr_p));
	    }
	    attr_inst_len += sizeof(length); /* store number of bindings */
	    for(i = 0; i < SA_ATTR_BND_NUM(sec_attr_p); i++) {
	        attr_inst_len += sizeof(SA_ATTR_BND_TYPE(sec_attr_p, i));
	        switch(SA_ATTR_BND_TYPE(sec_attr_p, i)) {
	        case sec_attr_bind_type_string:
	            attr_inst_len += sizeof(length); /* store string length */
	            attr_inst_len += 
			u_strlen(SA_ATTR_BND_STRING_P(sec_attr_p, i));
	            break;
	        case sec_attr_bind_type_twrs:
		    if(SA_ATTR_BND_TWRSET_P(sec_attr_p, i)) {
			/* store count of towers */
	                attr_inst_len += sizeof(length); 
	                for(j = 0; j < SA_ATTR_BND_TWRSET_COUNT(sec_attr_p, i); 
			    j++) {
		            attr_inst_len += sizeof(length); /* store twr_len */
		            attr_inst_len += 
				SA_ATTR_BND_TWR_LEN(sec_attr_p, i, j);
	                }
		    }
	            break;
	        case sec_attr_bind_type_svrname:
		    if(SA_ATTR_BND_SVRNAME_P(sec_attr_p, i)) {
			attr_inst_len += 
			    sizeof(SA_ATTR_BND_SVRNAME_SYNTAX(sec_attr_p, i));
			attr_inst_len += sizeof(length); /* store name length */
			attr_inst_len += 
			    u_strlen(SA_ATTR_BND_SVRNAME_NAME_P(sec_attr_p, i));
		    }
		    break;
	        } /* end bind_type switch */
	    }
	} /* end if bind_info present */
	break;
    default:
	break;
    } /* end enc_type switch */

    *size = attr_inst_len;
    return;
}



/*
 * r s d b _ a t t r _ s e q _ l i s t _ c r e a t e 
 *
 * rsdb_attr_create is called by rsdb_attr_sch_store_new to create
 * the head of a sequential list of attribute instances matching
 * the attribute type of the new schema entry.
 * A sequential list of attribute instances will exist for 
 * each schema entry (instead of one seq. list for the entire database).
 *
 * Since the head itself is stored in the attributes database with
 * a sequential key equal to the sequential_key of the schema entry,
 * we don't have another unique, "easy to find" seq_id for a "tail,"
 * therefore, we store tail information (like prev) in
 * the head structure.
 * Conceptually, head.prev will always be END_OF_LIST;
 * therefor, we don't store head.prev -- we let the tail.prev use the field.
 * Likewise, tail.next is always START_OF_LISTS; therefor we don't
 * store it; only head.next is stored.
 * As new attribute instances of this type are added,
 * they are inserted into the chain. 
 *
 */
PUBLIC void rsdb_attr_seq_list_create(
    rsdb_pvt_id_t               attr_type_id,    /* [in] */
    error_status_t 		*st_p)
{
    rsdb_attr_inst_t	head;
    signed32		length;

    length = sizeof(head);
    memset((Pointer) &head, NULL, length);
    head.threads.id = attr_type_id;
    head.threads.next = END_OF_LIST;
    ATTR_SEQ_LIST_TAIL_PREV(&head) = head.threads.id;
    head.attr_inst_len = length;
    rsdb_attr_store_by_id(head.threads.id, &head);
}


/*
 * r s d b _ a t t r _ s e q _ l i s t _ d e l e t e
 *
 * For the sequential list identified by attr_type_id, delete
 * every attribute instance on the sequential list by removing
 * each attribute from its object's attr_list, updating the
 * object's num_attrs, and purging the attribute instance record.
 * Purge the seq_list head record.
 *
 */
PUBLIC void rsdb_attr_seq_list_delete(
    rsdb_pvt_id_t		attr_type_id	/* [in] */
)
{
    rsdb_attr_inst_t            *head;
    rsdb_attr_inst_t            *prev;
    error_status_t              st, *st_p=&st;
    rsdb_attr_inst_t		*attr_p;
    rsdb_pvt_id_t		attr_inst_id;
    rsdb_pvt_id_t		attr_list_id;

    /* look up the head of the seq list using attr_type_id */
    if(!(rsdb_attr_get_by_id(attr_type_id, &head, st_p))) {
        dce_svc_printf(SEC_ATTR_CANT_GET_INSTANCE_MSG);
    }
    attr_inst_id = head->threads.next;
    while( attr_inst_id != END_OF_LIST ) {
	if(!(rsdb_attr_get_by_id(attr_inst_id, &attr_p, st_p))) {
            dce_svc_printf(SEC_ATTR_CANT_GET_INSTANCE_MSG);
	}
	/* Get the attr_list_id of the object to which this
	 * attribute is attached. 
	 */
	rsdb_attr_obj_get_attr_list_id(attr_p->domain, attr_p->object_id,
	    &attr_list_id, st_p);

	/* Remove this attribute's id from its object's attr_list */
	rsdb_attr_list_delete_inst(attr_p->threads.id, attr_list_id);

	/* Decrement the num_attrs field on the object to
	 * which this attribute was attached.
	 */
	rsdb_attr_obj_upd_num_attrs(attr_p->domain, 
	    attr_p->object_id, -1, st_p);

	/* delete the attr_inst record */
	rsdb_attr_purge(attr_p->threads.id, st_p);

	attr_inst_id = attr_p->threads.next;
	free(attr_p);
    }

    /* delete the seq_list head record */
    rsdb_attr_purge(head->threads.id, st_p);

    free(head);
}


/*
 * r s d b _ a t t r _ s t o r e _ b y _ i d 
 *
 * Store the input rsdb_attr_inst_t by sequential id
 * in the ATTR database.
 * This routine either succeeds or fails with fatal error.
 *
 * Caller must set attr->attr_inst_len.
 */
PUBLIC void rsdb_attr_store_by_id(
    rsdb_pvt_id_t   		attr_id,
    rsdb_attr_inst_t		*attr)
{
    rsdb_sequential_key_t	sequential_key;
    error_status_t		st, *st_p = &st;

    memset((Pointer) &sequential_key, NULL, sizeof(rsdb_sequential_key_t));

    sequential_key.record_type	= SEQUENTIAL_TYPE;
    sequential_key.id 		= attr_id;

    rsdb_replace(rsdb_attr_db, (Pointer) &sequential_key,
	sizeof(rsdb_sequential_key_t), (Pointer) attr, 
	attr->attr_inst_len, st_p);
}


/*
 * r s d b _ a t t r _ g e t _ b y _ i d
 *
 * Retrieve the rsdb attr instance using the input attr_inst_id as the key.
 * Return true if attr found, false otherwise.
 * Buffer allocated for attr must be freed by caller.
 *
 * Status Codes:
 *	sec_attr_inst_not_found	 if attr not found with attr_inst_id.
 *	sec_attr_no_memory      if malloc fails.
 *	error_status_ok		 otherwise.
 */
PUBLIC boolean32 rsdb_attr_get_by_id(
    rsdb_pvt_id_t   		attr_inst_id,
    rsdb_attr_inst_t		**attr,		/* output */
    error_status_t  		*st_p)
{
    rsdb_sequential_key_t	sequential_key;
    Pointer		 	data;
    int				data_len;

    CLEAR_STATUS(st_p);
    memset((Pointer) &sequential_key, NULL, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = attr_inst_id;

    rsdb_fetch(rsdb_attr_db, (Pointer) &sequential_key,
		sizeof(sequential_key), &data, &data_len, st_p);

    if (BAD_STATUS(st_p)) {
	SET_STATUS(st_p, sec_attr_inst_not_found);
	*attr = (rsdb_attr_inst_t *)NULL;
	return false;
    }
    if(!(*attr = (rsdb_attr_inst_t *)malloc(data_len))) {
	SET_STATUS(st_p, sec_attr_no_memory);
	return false;
    }
    memcpy((Pointer)(*attr), data, data_len);
    return true;
}


/*
 * r s d b _ a t t r _ g e t _ b y _ i d _ a n d _ e x p o r t
 *
 * Return a sec_attr_t for the attribute instance identified
 * by attr_inst_id.  This call saves the malloc and memcpy of
 * the rsdb_attr_inst_t that is done in rsdb_attr_get_by_id.
 *
 * First retrieve the rsdb attr using the input attr_inst_id as the key.
 * Export the data into sec_attr_p.
 * Return true if attr found, false otherwise.
 * If allocator is not rpc_ss_allocate, caller 
 * must free allocated buffers by calling
 * sec_attr_util_inst_free_ptrs(sec_attr_p) when done.
 *
 * INPUT:
 *	attr_inst_id - sequential id of the attribute instance
 *		to be retrieved.
 *	allocator - if NULL, use malloc for sec_attr_p buffers and
 *		caller must free buffers.
 *		Set allocator to rpc_ss_allocate when calling from
 *		manager code called from a server stub so that 
 *		memory is automatically freed.
 * OUTPUT
 *	sec_attr_p - pointer to a sec_attr_t.  On output, internal
 *		pointers have been allocated and data copied in.
 *
 * Status Codes:
 *	sec_attr_inst_not_found	 if attr not found with attr_inst_id.
 *	sec_attr_no_memory      if malloc fails.
 *	error_status_ok		 otherwise.
 */
PUBLIC boolean32 rsdb_attr_get_by_id_and_export(
    rsdb_pvt_id_t   		attr_inst_id,			/* [in] */
    void                        *(*allocator)(unsigned32),      /* [in] */
    sec_attr_t			*sec_attr_p,			/* [out] */
    error_status_t  		*st_p
)
{
    rsdb_sequential_key_t	sequential_key;
    Pointer		 	data;
    int				data_len;
    rsdb_attr_inst_t		*db_attr_p;

    CLEAR_STATUS(st_p);

    memset((Pointer) &sequential_key, NULL, sizeof(rsdb_sequential_key_t));
    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = attr_inst_id;

    rsdb_fetch(rsdb_attr_db, (Pointer) &sequential_key,
		sizeof(rsdb_sequential_key_t), &data, &data_len, st_p);
    if (BAD_STATUS(st_p)) {
	SET_STATUS(st_p, sec_attr_inst_not_found);
	return false;
    }
    db_attr_p = (rsdb_attr_inst_t *)data;
    rsdb_attr_export(db_attr_p, allocator, sec_attr_p, st_p);
    if (BAD_STATUS(st_p)) {
	return false;
    }
    return true;
}


/*
 * r s d b _ a t t r _ l i s t _ g e t _ b y _ i d
 *
 * Retrieve the attr_list using the input attr_list_id as the key.
 * Return true if attr_list found, false otherwise.
 *
 * Status Codes:
 *	sec_attr_inst_not_found	 if attr_list not found with attr_list_id.
 *	error_status_ok		 otherwise.
 */
PUBLIC boolean32 rsdb_attr_list_get_by_id(
    rsdb_pvt_id_t   		attr_list_id,	/* [in] */
    rsdb_max_attr_list_t	*attr_list)	/* [out] */
{
    rsdb_sequential_key_t	sequential_key;
    Pointer		 	data;
    int				data_len;
    error_status_t  		st, *st_p = &st;

    CLEAR_STATUS(st_p);
    memset((Pointer) &sequential_key, NULL, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = attr_list_id;

    rsdb_fetch(rsdb_attr_db, (Pointer) &sequential_key,
		sizeof(sequential_key), &data, &data_len, st_p);

    if (BAD_STATUS(st_p)) {
	return false;
    }
    memcpy((Pointer)(attr_list), data, data_len);
    return true;
}


/*
 * r s d b _ a t t r _ l i s t _ i s _ m e m b e r
 *
 * Return true if the member appears
 * in the attribute list identified by attr_list_id.
 * Return the id (in free_list_id) of the attr_list with available 
 * slots for adding members.
 */
PUBLIC boolean32 rsdb_attr_list_is_member(
    rsdb_pvt_id_t      attr_list_id,		/* [in] */
    rsdb_pvt_id_t      member,			/* [in] */
    rsdb_pvt_id_t      *free_list_id		/* [out] */
)
{
    long                        i;
    rsdb_max_attr_list_t	list;
    rsdb_pvt_id_t		space_id = END_OF_LIST;
    rsdb_pvt_id_t		last_attr_list_id;

    if (attr_list_id == END_OF_LIST) {
        if (free_list_id)
            *free_list_id = attr_list_id;
        return false;
    }

    while (attr_list_id != END_OF_LIST) {
	if(!(rsdb_attr_list_get_by_id(attr_list_id, &list))) {
            dce_svc_printf(SEC_ATTR_CANT_GET_ATTRLIST_MSG);
	}
        for (i = 0; i < list.total_count; i++) {
            if(!list.attr[i].inst_id.deleted && 
		list.attr[i].inst_id.id == member) {
                return true;
            }
        }
        if (list.count < RSDB_MAX_ATTR_LIST_SIZE && space_id == END_OF_LIST) {
            space_id = attr_list_id;
        }
        /* 
         * save the value of the last good attr_list_id in case we
         * need it for the free_list_id
         */
        last_attr_list_id = attr_list_id;
        attr_list_id = list.next;
    }
    if (free_list_id) {
        *free_list_id = (space_id != END_OF_LIST) ? space_id : last_attr_list_id;
    }
    return false;        
}

/*
 * r s d b _ a t t r _ l i s t _ g e t _ i n s t _ b y _ t y p e
 *
 * Return true if an instance of type attr_type_id exists
 * in the attribute list identified by attr_list_id.
 * Return the list_id (in attr_list_id) and index where 
 * the first instance of the requested type is found.
 * Return the attr_inst_id of the instance that was found.
 */
PUBLIC boolean32 rsdb_attr_list_get_inst_by_type(
    rsdb_pvt_id_t	attr_type_id,		/* [in] */
    rsdb_pvt_id_t	*attr_list_id,		/* [in, out] */
    signed32		*index,			/* [out] */
    rsdb_pvt_id_t	*attr_inst_id		/* [out] */
)
{
    long                        i;
    rsdb_max_attr_list_t	list;

    if (*attr_list_id == END_OF_LIST) {
        return false;
    }

    while (*attr_list_id != END_OF_LIST) {
	if(!(rsdb_attr_list_get_by_id(*attr_list_id, &list))) {
            dce_svc_printf(SEC_ATTR_CANT_GET_ATTRLIST_MSG);
	}
        for (i = 0; i < list.total_count; i++) {
            if(!list.attr[i].inst_id.deleted && 
		list.attr[i].type_id == attr_type_id) {
		*index = i;
		*attr_inst_id = list.attr[i].inst_id.id;
                return true;
            }
        }
        *attr_list_id = list.next;
    }
    return false;        
}


/*
 * r s d b _ a t t r _ l i s t _ d e l e t e
 *
 * For the attribute list whose head is attr_list_head, delete
 * every attribute instance on the attribute list by removing
 * each attribute from its seq_list and purging the 
 * attribute instance record.  Purge each attr_list record.
 *
 * This operation does NOT update the num_attrs or attr_list_id
 * fields on the object.
 */
PUBLIC void rsdb_attr_list_delete(
    rsdb_pvt_id_t               attr_list_head	/* [in] */
)
{
    rsdb_max_attr_list_t	attr_list;
    long                        i;
    rsdb_pvt_id_t               attr_inst_id;
    rsdb_pvt_id_t               attr_list_id;
    rsdb_pvt_id_t               prev_attr_list_id;
    rsdb_attr_inst_t		*attr_p;
    error_status_t		st, *st_p=&st;

    attr_list_id = attr_list_head;

    while(attr_list_id != END_OF_LIST) {
	if(!(rsdb_attr_list_get_by_id(attr_list_id, &attr_list))) {
            dce_svc_printf(SEC_ATTR_CANT_GET_ATTRLIST_MSG);
	}
        for (i = 0; i < attr_list.total_count; i++) {
            if (!attr_list.attr[i].inst_id.deleted) {
		attr_inst_id = attr_list.attr[i].inst_id.id;

		if(!(rsdb_attr_get_by_id(attr_inst_id, &attr_p, st_p))) {
                    dce_svc_printf(SEC_ATTR_CANT_GET_INSTANCE_MSG);
		}
		/* Remove this attr_inst from the seq_list 
		 * for its attr_type.
		 */
		rsdb_attr_seq_list_delete_inst(attr_p);

		/* Delete the attribute instance record */
		rsdb_attr_purge(attr_p->threads.id, st_p);

		free(attr_p);
            }
        }
	prev_attr_list_id = attr_list_id;
        attr_list_id = attr_list.next;

	/* Delete the attribute list record we've finished processing */
	rsdb_attr_list_purge(prev_attr_list_id);
    }
}


/*
 * r s d b _ a t t r _ s t o r e _ n e w
 *
 * Store the input rsdb_attr_inst_t in the ATTRIBUTE database.
 * This call expects attr_p->threads.alias to be set to the
 * sequential id (attr_type_id) of the corresponding sch_entry.
 * The output value of attr_inst_id will contain the sequential key,
 * for the data that was stored.
 *
 * Store the attribute by sequential id and insert into the
 * sequential list stored on the schema_entry for attrs of this type.
 * Add the attr_inst_id to the attr_list for this object.
 *
 */
PUBLIC void rsdb_attr_store_new(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,  /* [in] */
    rsdb_attr_inst_t		*attr_p,	/* [in] */
    rsdb_pvt_id_t		*attr_inst_id,	/* [out] */
    error_status_t		*st_p
)
{
    rsdb_sequential_key_t	sequential_key;

    memset((Pointer) &sequential_key, NULL, sizeof(sequential_key));

    sequential_key.record_type 	= SEQUENTIAL_TYPE;
    sequential_key.id 		= rsdb_new_sequential_id();
    attr_p->threads.id	 	= sequential_key.id;
    *attr_inst_id 		= sequential_key.id;

    /*
     *  Thread the new attribute instance on the 
     *  sch_entry's sequential list of attribute instances.
     *  This sets the prev and next fields of attr_p->threads.
     */
    rsdb_attr_seq_list_add_inst(attr_p);

    /* store the attr_inst by seq id */
    rsdb_store(rsdb_attr_db, (Pointer) &sequential_key,
    	sizeof(sequential_key), (Pointer) attr_p, 
    	attr_p->attr_inst_len, st_p);

    /* Add the attr_inst seq_id to the attr_list for
     * this object.
     */
    rsdb_attr_list_add_inst(rgy_obj_info, attr_p);
}


/*
 * r s d b _ a t t r _ i m p o r t
 *
 * Allocate in db_attr_p enough space for the fixed and
 * variable length data in sec_attr_p.
 * Copy data from the sec_attr_p structure and place it into db_attr_p.
 * db_attr_p must be FREED BY CALLER.
 * This routine does NOT fill in the threads and object_id fields
 * of db_attr_p.
 *
 * INPUT
 *      sec_attr_p - points to semantically-checked sec_attr_t
 *              whose data is to be copied into db_attr_p for storage.
 * OUTPUT
 *      db_attr_p - points to an rsdb_attr_inst_t buffer
 *              allocated by this function into which sec_attr data is copied.
 *
 * Status Codes:
 */
PUBLIC void rsdb_attr_import(
    sec_attr_t		*sec_attr_p,
    rsdb_attr_inst_t	**db_attr_p,
    error_status_t 	*st_p)
{
    long		i,j;
    unsigned32		length;
    Pointer		bufp;

    CLEAR_STATUS(st_p);
    *db_attr_p = NULL;

    /* Allocate the db_attr_p buffer large enough to hold
     * the data in sec_attr_p.  db_attr_p is memset to NULL
     * except for the attr_inst_len field which is set to the size
     * of the allocated buffer.
     */
    rsdb_attr_alloc_db_inst(sec_attr_p, db_attr_p, st_p);
    if(BAD_STATUS(st_p)) {
	return;
    }

    (*db_attr_p)->encoding = SA_ATTR_ENCODING(sec_attr_p);
    
    bufp = (Pointer)(*db_attr_p)->buf;
    rsdb_attr_import_value(sec_attr_p, bufp, st_p);
}


/*
 * r s d b _ a t t r _ i m p o r t _ v a l u e
 *
 * Copy data from the sec_attr_p structure into bufp.
 * bufp must point to an allocated buffer large enough to hold
 * all the attribute value data (not including encoding type)
 * in sec_attr_p.  
 * This routine does NOT fill in the threads and object_id fields
 * of db_attr_p.
 *
 * INPUT
 *      sec_attr_p - points to semantically-checked sec_attr_t
 *              whose data is to be copied into db_attr_p for storage.
 * OUTPUT
 *      bufp - points to an allocated char buffer
 *              into which sec_attr value data is copied.
 *
 * Status Codes:
 */
PUBLIC void rsdb_attr_import_value(
    sec_attr_t		*sec_attr_p,
    Pointer		bufp,
    error_status_t 	*st_p)
{
    long		i,j;
    unsigned32		length;

    CLEAR_STATUS(st_p);

    switch(SA_ATTR_ENCODING(sec_attr_p)) {
    case sec_attr_enc_any:
    case sec_attr_enc_void:
	break;
    case sec_attr_enc_integer:
	RSDB_ATTR_COPY_ADV_TGT(bufp, &SA_ATTR_INTEGER(sec_attr_p),
		sizeof(SA_ATTR_INTEGER(sec_attr_p)));
	break;
    case sec_attr_enc_printstring:
	length = u_strlen(SA_ATTR_PRINTSTRING_P(sec_attr_p));
	RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
	RSDB_ATTR_COPY_ADV_TGT(bufp, SA_ATTR_PRINTSTRING_P(sec_attr_p), length);
	break;
    case sec_attr_enc_printstring_array:
	length = SA_ATTR_STR_ARRAY_NUM(sec_attr_p);
	RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
	for(i = 0; i < SA_ATTR_STR_ARRAY_NUM(sec_attr_p); i++) {
	    length = u_strlen(SA_ATTR_STR_ARRAY_ELT_P(sec_attr_p, i));
	    RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
	    RSDB_ATTR_COPY_ADV_TGT(bufp, SA_ATTR_STR_ARRAY_ELT_P(sec_attr_p, i), 
		length);
	}
	break;
    case sec_attr_enc_bytes:
    case sec_attr_enc_confidential_bytes:
	length = SA_ATTR_BYTES_LEN(sec_attr_p);
	RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
	RSDB_ATTR_COPY_ADV_TGT(bufp, &SA_ATTR_BYTES_DATA(sec_attr_p, 0), 
		length);
	break;
    case sec_attr_enc_i18n_data:
	/* NOTE: length is stored *before* codeset so that
	 * the export routine may retrieve it first in order to
	 * determine the size to malloc for the idata structure.
	 */
	length = SA_ATTR_IDATA_LEN(sec_attr_p);
	RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
	RSDB_ATTR_COPY_ADV_TGT(bufp, &SA_ATTR_IDATA_CODESET(sec_attr_p), 
		sizeof(SA_ATTR_IDATA_CODESET(sec_attr_p)));
	RSDB_ATTR_COPY_ADV_TGT(bufp, &SA_ATTR_IDATA_DATA(sec_attr_p, 0), 
		length);
	break;
    case sec_attr_enc_uuid:
	RSDB_ATTR_COPY_ADV_TGT(bufp, &SA_ATTR_UUID(sec_attr_p), 
		sizeof(SA_ATTR_UUID(sec_attr_p)));
	break;
    case sec_attr_enc_attr_set:
	length = SA_ATTR_SET_NUM(sec_attr_p);
	RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
        for(i = 0; i < SA_ATTR_SET_NUM(sec_attr_p); i++) {
	    RSDB_ATTR_COPY_ADV_TGT(bufp, &SA_ATTR_SET_MEMBERS(sec_attr_p, i), 
		sizeof(SA_ATTR_SET_MEMBERS(sec_attr_p, i)));
	}
	break;
    case sec_attr_enc_binding:
	/* NOTE: num_bindings is stored *before* auth_info so that
	 * the export routine may retrieve it first in order to
	 * determine the size to malloc for the bind_info structure.
	 */
	length = SA_ATTR_BND_NUM(sec_attr_p);
	RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
	/* store auth_info_type */
	RSDB_ATTR_COPY_ADV_TGT(bufp, &SA_ATTR_BND_AUTH_INFO_TYPE(sec_attr_p),
		sizeof(SA_ATTR_BND_AUTH_INFO_TYPE(sec_attr_p)));
	/* store auth_info */
	if(SA_ATTR_BND_AUTH_INFO_TYPE(sec_attr_p) == sec_attr_bind_auth_dce) {
	    /* store svr_pname length, then name */
	    length = u_strlen(SA_ATTR_BND_AUTH_SVR_PNAME_P(sec_attr_p));
	    RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
	    RSDB_ATTR_COPY_ADV_TGT(bufp, 
		SA_ATTR_BND_AUTH_SVR_PNAME_P(sec_attr_p), length);
	    /* copy the last three fields of DCE_INFO */
	    RSDB_ATTR_COPY_ADV_TGT(bufp, 
		&SA_ATTR_BND_AUTH_PROT_LEVEL(sec_attr_p),
		sizeof(SA_ATTR_BND_AUTH_PROT_LEVEL(sec_attr_p)));
	    RSDB_ATTR_COPY_ADV_TGT(bufp, 
		&SA_ATTR_BND_AUTH_AUTHN_SVC(sec_attr_p),
		sizeof(SA_ATTR_BND_AUTH_AUTHN_SVC(sec_attr_p)));
	    RSDB_ATTR_COPY_ADV_TGT(bufp, 
		&SA_ATTR_BND_AUTH_AUTHZ_SVC(sec_attr_p),
		sizeof(SA_ATTR_BND_AUTH_AUTHZ_SVC(sec_attr_p)));
	}
	for(i = 0; i < SA_ATTR_BND_NUM(sec_attr_p); i++) {
	    RSDB_ATTR_COPY_ADV_TGT(bufp, &SA_ATTR_BND_TYPE(sec_attr_p, i),
		sizeof(SA_ATTR_BND_TYPE(sec_attr_p, i)));
	    switch(SA_ATTR_BND_TYPE(sec_attr_p, i)) {
	    case sec_attr_bind_type_string:
		length =  u_strlen(SA_ATTR_BND_STRING_P(sec_attr_p, i));
		RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
		RSDB_ATTR_COPY_ADV_TGT(bufp, 
		    SA_ATTR_BND_STRING_P(sec_attr_p, i), length);
	        break;
	    case sec_attr_bind_type_twrs:
		length = SA_ATTR_BND_TWRSET_COUNT(sec_attr_p, i);
		RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
	        for(j = 0; j < SA_ATTR_BND_TWRSET_COUNT(sec_attr_p, i); j++) {
		    length = SA_ATTR_BND_TWR_LEN(sec_attr_p, i, j);
		    RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
		    RSDB_ATTR_COPY_ADV_TGT(bufp, 
			SA_ATTR_BND_TWR_OCTETS(sec_attr_p, i, j),
			length);
	        }
	        break;
	    case sec_attr_bind_type_svrname:
		if(SA_ATTR_BND_SVRNAME_P(sec_attr_p, i)) {
		    RSDB_ATTR_COPY_ADV_TGT(bufp, 
		        &(SA_ATTR_BND_SVRNAME_SYNTAX(sec_attr_p, i)), 
		        sizeof(SA_ATTR_BND_SVRNAME_SYNTAX(sec_attr_p, i)));
		    length = 
			u_strlen(SA_ATTR_BND_SVRNAME_NAME_P(sec_attr_p, i));
		    RSDB_ATTR_COPY_ADV_TGT(bufp, &length, sizeof(length));
		    RSDB_ATTR_COPY_ADV_TGT(bufp, 
		        SA_ATTR_BND_SVRNAME_NAME_P(sec_attr_p, i), length);
		}
		break;
	    default:
		break;
	    } /* end bind_type switch */
	}
    default:
	break;
    } /* end encoding type switch */
}



/*
 * r s d b _ a t t r _ e x p o r t
 *
 * Unpack data from db_attr_p into sec_attr_p buffer.
 * Allocate space for variable length fields in sec_attr_p.
 * If allocator is not rpc_ss_allocate, caller 
 * must free allocated buffers by calling
 * sec_attr_util_inst_free_ptrs(sec_attr_p) when done.
 *
 * INPUT:
 *	db_attr_p - points to an rsdb_attr_inst_t containing
 *		data to be exported into sec_attr_p.
 *	allocator - if NULL, use malloc for sec_attr_p buffers and
 *		caller must free buffers.
 *		Set allocator to rpc_ss_allocate when calling from
 *		manager code called from a server stub so that 
 *		memory is automatically freed.
 * OUTPUT:
 *	sec_attr_p - pointer to a sec_attr_t.  On output, internal
 *		pointers have been allocated and data copied in.
 *
 * ERRORS:
 *	sec_attr_cant_allocate_memory - no more memory.
 *
 * 
 * WARNING: CALLING PROGRAM MUST HANDLE SETTING THE UUID!!! 
 *
 */
PUBLIC void rsdb_attr_export(
    rsdb_attr_inst_t		*db_attr_p,			/* [in] */
    void			*(*allocator)(unsigned32),	/* [in] */
    sec_attr_t			*sec_attr_p,			/* [out] */
    error_status_t  		*st_p)
{
    Pointer		bufp;	/* Pointer to variable length buffer */

    CLEAR_STATUS(st_p);

    /* Note that "allocator" is called in ATTR_ITEM_ALLOC macro */

    SA_ATTR_ENCODING(sec_attr_p) = db_attr_p->encoding;

    /*
     * Set bufp to point to the beginning of the variable
     * length data in db_attr_p.  As data is copied,
     * bufp is updated to point at the unread data.
     */
    bufp = (Pointer)db_attr_p->buf;
    rsdb_attr_export_value(sec_attr_p, allocator, bufp, st_p);
}

/*
 * r s d b _ a t t r _ e x p o r t _ v a l u e
 *
 * Copy data from the bufp structure into sec_attr_p.
 * bufp must point to an allocated buffer large enough to hold
 * all the attribute value data (not including encoding type)
 * in sec_attr_p.
 * This routine does NOT fill in the threads and object_id fields
 * of db_attr_p.
 *
 * OUTPUT
 *      sec_attr_p - points to semantically-checked sec_attr_t
 *              whose data is to be copied into db_attr_p for storage.
 * INPUT
 *      bufp - points to an allocated char buffer
 *              into which sec_attr value data is copied.
 *
 * Status Codes:
 */

PUBLIC void rsdb_attr_export_value(
    sec_attr_t          *sec_attr_p,
    void		*(*allocator)(unsigned32),	/* [in] */
    Pointer             bufp,
    error_status_t      *st_p)
{
    long                i,j;
    unsigned32          length;

    CLEAR_STATUS(st_p);

    switch(SA_ATTR_ENCODING(sec_attr_p)) {
    case sec_attr_enc_any:
    case sec_attr_enc_void:
	break;
    case sec_attr_enc_integer:
	RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_INTEGER(sec_attr_p), bufp,
		sizeof(SA_ATTR_INTEGER(sec_attr_p)));
	break;
    case sec_attr_enc_printstring:
	/* get string length */
	RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
	ATTR_ITEM_ALLOC(sec_attr_p, SA_ATTR_PRINTSTRING_P(sec_attr_p),
		length + 1, st_p);
	RSDB_ATTR_COPY_NULL_ADV_SRC(SA_ATTR_PRINTSTRING_P(sec_attr_p),
		bufp, length);
	break;
    case sec_attr_enc_printstring_array:
	/* get number of strings */
	RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
	ATTR_ITEM_ALLOC(sec_attr_p, SA_ATTR_STR_ARRAY_P(sec_attr_p),
		SA_ATTR_STR_ARRAY_SIZE(length), st_p);
	SA_ATTR_STR_ARRAY_NUM(sec_attr_p) = length;
	for(i = 0; i < SA_ATTR_STR_ARRAY_NUM(sec_attr_p); i++) {
	    /* get string length */
	    RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
	    ATTR_ITEM_ALLOC(sec_attr_p, SA_ATTR_STR_ARRAY_ELT_P(sec_attr_p, i),
		length + 1, st_p);
	    RSDB_ATTR_COPY_NULL_ADV_SRC(SA_ATTR_STR_ARRAY_ELT_P(sec_attr_p, i),
		bufp, length);
	}
	break;
    case sec_attr_enc_bytes:
    case sec_attr_enc_confidential_bytes:
	/* get byte string length */
	RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
	ATTR_ITEM_ALLOC(sec_attr_p, SA_ATTR_BYTES_P(sec_attr_p),
		SA_ATTR_BYTES_SIZE(length), st_p);
	SA_ATTR_BYTES_LEN(sec_attr_p) = length;
	RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_BYTES_DATA(sec_attr_p, 0), 
		bufp, length);
	break;
    case sec_attr_enc_i18n_data:
	/* NOTE: the import routine stored the length *before* the codeset
	 */
	/* get byte string length */
	RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
	ATTR_ITEM_ALLOC(sec_attr_p, SA_ATTR_IDATA_P(sec_attr_p),
		SA_ATTR_IDATA_SIZE(length), st_p);
	/* get codeset */
	RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_IDATA_CODESET(sec_attr_p), 
		bufp, sizeof(SA_ATTR_IDATA_CODESET(sec_attr_p)));
	SA_ATTR_IDATA_LEN(sec_attr_p) = length;
	RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_IDATA_DATA(sec_attr_p, 0), 
		bufp, length);
	break;
    case sec_attr_enc_uuid:
	RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_UUID(sec_attr_p), bufp,
		sizeof(SA_ATTR_UUID(sec_attr_p)));
	break;
    case sec_attr_enc_attr_set:
	/* get number of members */
	RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
	ATTR_ITEM_ALLOC(sec_attr_p, SA_ATTR_SET_P(sec_attr_p),
		SA_ATTR_SET_SIZE(length), st_p);
	SA_ATTR_SET_NUM(sec_attr_p) = length;
	for(i = 0; i < SA_ATTR_SET_NUM(sec_attr_p); i++) {
	    RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_SET_MEMBERS(sec_attr_p, i), 
		bufp, sizeof(SA_ATTR_SET_MEMBERS(sec_attr_p, i)));
	}
	break;
    case sec_attr_enc_binding:
	/* NOTE: the import routine stored num_bindings *before* auth_info
	 */
	/* get num_bindings to calculate size for malloc */
	RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
	ATTR_ITEM_ALLOC(sec_attr_p, SA_ATTR_BND_INFO_P(sec_attr_p),
		SA_BND_INFO_SIZE(length), st_p);
	SA_ATTR_BND_NUM(sec_attr_p) = length;

	/* Copy the auth_info_type */
	RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_BND_AUTH_INFO_TYPE(sec_attr_p), 
		bufp, sizeof(SA_ATTR_BND_AUTH_INFO_TYPE(sec_attr_p)));

	/* copy auth_info if it was stored */
	if(SA_ATTR_BND_AUTH_INFO_TYPE(sec_attr_p) == sec_attr_bind_auth_dce) {
	    /* get svr_pname string length, malloc and copy */
	    RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
	    ATTR_ITEM_ALLOC(sec_attr_p, 
		SA_ATTR_BND_AUTH_SVR_PNAME_P(sec_attr_p),
		length + 1, st_p);
	    RSDB_ATTR_COPY_NULL_ADV_SRC(
		SA_ATTR_BND_AUTH_SVR_PNAME_P(sec_attr_p),
		bufp, length);
	    /* copy the last three fields of DCE_INFO */
	    RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_BND_AUTH_PROT_LEVEL(sec_attr_p), 
		bufp, sizeof(SA_ATTR_BND_AUTH_PROT_LEVEL(sec_attr_p)));
	    RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_BND_AUTH_AUTHN_SVC(sec_attr_p), 
		bufp, sizeof(SA_ATTR_BND_AUTH_AUTHN_SVC(sec_attr_p)));
	    RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_BND_AUTH_AUTHZ_SVC(sec_attr_p), 
		bufp, sizeof(SA_ATTR_BND_AUTH_AUTHZ_SVC(sec_attr_p)));
	}
	for(i = 0; i < SA_ATTR_BND_NUM(sec_attr_p); i++) {
	    RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_BND_TYPE(sec_attr_p, i), 
		bufp, sizeof(SA_ATTR_BND_TYPE(sec_attr_p, i)));

	    switch(SA_ATTR_BND_TYPE(sec_attr_p, i)) {
	    case sec_attr_bind_type_string:
		/* get string_binding length, malloc and copy */
		RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
		ATTR_ITEM_ALLOC(sec_attr_p, 
		    SA_ATTR_BND_STRING_P(sec_attr_p, i),
		    length + 1, st_p);
		RSDB_ATTR_COPY_NULL_ADV_SRC(SA_ATTR_BND_STRING_P(sec_attr_p, i),
		    bufp, length);
	        break;
	    case sec_attr_bind_type_twrs:
		/* get count of towers */
		RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
		ATTR_ITEM_ALLOC(sec_attr_p, 
		    SA_ATTR_BND_TWRSET_P(sec_attr_p, i),
		    SA_TWR_SET_SIZE(length), st_p);
		SA_ATTR_BND_TWRSET_COUNT(sec_attr_p, i) = length;
	        for(j = 0; j < SA_ATTR_BND_TWRSET_COUNT(sec_attr_p, i); j++) {
		    /* get octet string length, malloc twr_t, copy */
		    RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
		    ATTR_ITEM_ALLOC(sec_attr_p, 
			SA_ATTR_BND_TWR_P(sec_attr_p, i, j),
			SA_TWR_SIZE(length), st_p);
		    SA_ATTR_BND_TWR_LEN(sec_attr_p, i, j) = length;
		    RSDB_ATTR_COPY_ADV_SRC(
			SA_ATTR_BND_TWR_OCTETS(sec_attr_p, i, j),
			bufp, length);
		}
	        break;
	    case sec_attr_bind_type_svrname:
		/* alloc the sec_attr_bind_svrname structure */
		ATTR_ITEM_ALLOC(sec_attr_p, 
		    SA_ATTR_BND_SVRNAME_P(sec_attr_p, i),
		    sizeof(*(SA_ATTR_BND_SVRNAME_P(sec_attr_p, i))), st_p);
		/* copy the name_syntax */
		RSDB_ATTR_COPY_ADV_SRC(
		    &(SA_ATTR_BND_SVRNAME_SYNTAX(sec_attr_p, i)), 
		    bufp, sizeof(SA_ATTR_BND_SVRNAME_SYNTAX(sec_attr_p, i)));
		/* get svrname length, malloc and copy */
		RSDB_ATTR_COPY_ADV_SRC(&length, bufp, sizeof(length));
		ATTR_ITEM_ALLOC(sec_attr_p, 
		    SA_ATTR_BND_SVRNAME_NAME_P(sec_attr_p, i),
		    length + 1, st_p);
		RSDB_ATTR_COPY_NULL_ADV_SRC(
		    SA_ATTR_BND_SVRNAME_NAME_P(sec_attr_p, i),
		    bufp, length);
		break;
	    default:
		break;
	    } /* end bind_type switch */
	}
    default:
	break;
    } /* end encoding type switch */
}

/*
 * r s d b _ a t t r _ e x p o r t _ t r i g _ b i n d i n g
 *
 * Copy the trigger binding information from the input
 * schema entry into the value field of the input/output
 * attribute, setting the encoding type to trig_binding.
 * The input attribute must have enc_type void (which is
 * how query trigger instances are stored) and the
 * input attribute type id is assumed to correspond to
 * the input schema entry.
 *
 * If the schema entry contains NULL trig_binding info,
 * the output attr value will also contain NULL trig_binding info
 * (no error will occur).
 *
 * INPUT
 *	sch_entry_p - points to the schema_entry (for the input
 *		sec_attr_p) containing the trigger binding
 *		to be copied into sec_attr_p.
 *      allocator - if NULL, use malloc for internal buffers
 *              in sec_attr_p; caller must free buffers.
 *              If allocator is set to rpc_ss_allocate (do this
 *		when calling from manager code called from
 *              a server stub), memory is automatically freed. 
 * INPUT/OUTPUT:
 *      sec_attr_p - [in] points to a sec_attr_t (with enc_type void)
 *		whose attr type corresponds to the input sch_entry_p.
 *		[out] The void value has been replaced by enc_type
 *		sec_attr_enc_trig_binding with the value copied
 *		from the trig_binding info in sch_entry_p.
 * Status Codes:
 *	sec_attr_no_memory
 *	sec_attr_bad_encoding_type - input sec_attr_p didn't have 
 *		encoding_type void.
 */
PUBLIC void rsdb_attr_export_trig_binding(
    sec_attr_schema_entry_t     *sch_entry_p,			/* [in] */
    void                        *(*allocator)(unsigned32),      /* [in] */
    sec_attr_t                  *sec_attr_p,    		/* [in,out] */
    error_status_t              *st_p
)
{
    int i, j;

    if(SA_ATTR_ENCODING(sec_attr_p) == sec_attr_enc_void) {

	SA_ATTR_ENCODING(sec_attr_p) = sec_attr_enc_trig_binding;

	/* Allocate the binding storage */

        ATTR_ALLOC(SA_ATTR_BND_INFO_P(sec_attr_p),
		SA_BND_INFO_SIZE(SA_TRG_BND_NUM(sch_entry_p)), st_p);

	if (BAD_STATUS(st_p))
		return;

        SA_ATTR_BND_NUM(sec_attr_p) = SA_TRG_BND_NUM(sch_entry_p);
	SA_ATTR_BND_AUTH_INFO_TYPE(sec_attr_p) = 
		SA_TRG_BND_AUTH_INFO_TYPE(sch_entry_p);

	switch (SA_TRG_BND_AUTH_INFO_TYPE(sch_entry_p)) {
        case sec_attr_bind_auth_none:
            break;
        case sec_attr_bind_auth_dce:

            SA_ATTR_BND_AUTH_PROT_LEVEL(sec_attr_p) =
			 SA_TRG_BND_AUTH_PROT_LEVEL(sch_entry_p);
            SA_ATTR_BND_AUTH_AUTHN_SVC(sec_attr_p) =
			 SA_TRG_BND_AUTH_AUTHN_SVC(sch_entry_p);
            SA_ATTR_BND_AUTH_AUTHZ_SVC(sec_attr_p) =
			 SA_TRG_BND_AUTH_AUTHZ_SVC(sch_entry_p);

            ATTR_ALLOC(SA_ATTR_BND_AUTH_SVR_PNAME_P(sec_attr_p),
		u_strlen(SA_TRG_BND_AUTH_SVR_PNAME_P(sch_entry_p)) + 1, st_p);
	    if (BAD_STATUS(st_p))
		return;

            u_strcpy(SA_ATTR_BND_AUTH_SVR_PNAME_P(sec_attr_p),
		SA_TRG_BND_AUTH_SVR_PNAME_P(sch_entry_p));
        }

        /* Copy each binding */
        for (i=0; i < SA_ATTR_BND_NUM(sec_attr_p); i++) {
            SA_ATTR_BND_TYPE(sec_attr_p,i) = SA_TRG_BND_TYPE(sch_entry_p,i);
	    switch (SA_ATTR_BND_TYPE(sec_attr_p,i)) {
	    case sec_attr_bind_type_string:
		ATTR_ALLOC(SA_ATTR_BND_STRING_P(sec_attr_p,i),
	            u_strlen(SA_TRG_BND_STRING_P(sch_entry_p,i)) + 1, st_p);
	        if (BAD_STATUS(st_p))
		    return;

		u_strcpy(SA_ATTR_BND_STRING_P(sec_attr_p,i), 
			SA_TRG_BND_STRING_P(sch_entry_p,i));
		break;
	    case sec_attr_bind_type_twrs:
                ATTR_ALLOC(SA_ATTR_BND_TWRSET_P(sec_attr_p, i),
                    SA_TWR_SET_SIZE(SA_TRG_BND_TWRSET_COUNT(sch_entry_p, i)),
                    st_p);
	        if (BAD_STATUS(st_p))
		    return;
                SA_ATTR_BND_TWRSET_COUNT(sec_attr_p, i) = 
		    SA_TRG_BND_TWRSET_COUNT(sch_entry_p, i);
                for(j = 0; j < SA_ATTR_BND_TWRSET_COUNT(sec_attr_p, i); j++) {
                    /* get octet string length, malloc twr_t, copy */
                    ATTR_ALLOC(SA_ATTR_BND_TWR_P(sec_attr_p, i, j),  
                        SA_TWR_SIZE(SA_TRG_BND_TWR_LEN(sch_entry_p, i, j)),
                        st_p);
	            if (BAD_STATUS(st_p))
		        return;
                    SA_ATTR_BND_TWR_LEN(sec_attr_p, i, j) = 
			SA_TRG_BND_TWR_LEN(sch_entry_p, i, j);
		    /* tower octets are not null-terminated */
		    memcpy((Pointer)SA_ATTR_BND_TWR_OCTETS(sec_attr_p, i, j),
			(Pointer)SA_TRG_BND_TWR_OCTETS(sch_entry_p, i, j),
			SA_ATTR_BND_TWR_LEN(sec_attr_p, i, j));
                }
                break;
	    case sec_attr_bind_type_svrname:
		ATTR_ALLOC(SA_ATTR_BND_SVRNAME_P(sec_attr_p,i),
		    sizeof(sec_attr_bind_svrname), st_p);

	        if (BAD_STATUS(st_p))
		    return;

		SA_ATTR_BND_SVRNAME_SYNTAX(sec_attr_p,i) =
		    SA_TRG_BND_SVRNAME_SYNTAX(sch_entry_p,i);

		ATTR_ALLOC(SA_ATTR_BND_SVRNAME_NAME_P(sec_attr_p,i), 
		    u_strlen(SA_TRG_BND_SVRNAME_NAME_P(sch_entry_p,i)) + 1,
		    st_p);

	        if (BAD_STATUS(st_p))
		    return;

		u_strcpy(SA_ATTR_BND_SVRNAME_NAME_P(sec_attr_p,i),
		    SA_TRG_BND_SVRNAME_NAME_P(sch_entry_p,i));
		break;
	    default:
		break;
	    }
	}
    }
}



/*
 * r s d b _ a t t r _ c u r s o r _ i n i t
 *
 * Initialize the cursor and return the current number
 * of attribute instances attached to the object identified
 * by rgy_obj_info->obj_id.
 *
 * INPUT:
 *	rgy_obj_info - contains obj_id, domain, obj_type, and
 *		attr_list_id of object whose attrs are to be retrieved
 *
 * OUTPUT:
 *	cur_num_attrs - number of attribute instances on object
 *
 *	cursor - initialized for attr_lookup operations on this
 *		object at this server
 *
 * ERRORS:
 *	sec_attr_bad_cursor
 *
 */
PUBLIC void rsdb_attr_cursor_init(
    rsdb_util_rgy_obj_info_t	*rgy_obj_info,		/* [in] */
    unsigned32                  *cur_num_attrs,		/* [out] */
    rs_attr_cursor_t            *cursor,                /* [out] */
    error_status_t  		*st_p)
{
    signed32		num;

    CLEAR_STATUS(st_p);

    if(!cursor) {
        SET_STATUS(st_p, sec_attr_bad_cursor);
    } else {
        cursor->source = rs_state.rep_id;

        /* attr cursor object_handle identifies object
	 * whose attrs are to be retrieved
         */
        cursor->object = rgy_obj_info->obj_id;

	/* set list to the first object attr_list_id */
        cursor->list =rgy_obj_info->attr_list_id;
        cursor->entry = 0;
	rsdb_attr_obj_get_num_attrs(rgy_obj_info, &num, st_p);
	cursor->num_entries_left = (unsigned32)num;
        cursor->valid = TRUE;
        if(cur_num_attrs) {
            *cur_num_attrs = cursor->num_entries_left;
        }
    }
}


/*
 * r s d b _ a t t r _ c u r s o r _ c h e c k
 *
 * If cursor is valid, make sure it originated at this server.
 * If not valid, initialize the cursor.
 * rs_attr_* operations should call this before making
 * an rsdb_attr_* query with the cursor.
 *
 * INPUT:
 *	rgy_obj_info - contains obj_id, domain, and obj_type of
 *		object whose attrs are to be retrieved
 *
 * OUTPUT:
 *	cursor - initialized for attr_schema_scan operation at
 *		this server.
 *
 * ERRORS:
 *	sec_attr_bad_cursor
 *
 */
PUBLIC void rsdb_attr_cursor_check(
    rsdb_util_rgy_obj_info_t	*rgy_obj_info,		/* [in] */
    rs_attr_cursor_t            *cursor,                /* [in, out] */
    error_status_t  		*st_p)
{
    unsigned32                  cur_num_attrs;

    CLEAR_STATUS(st_p);

    if(!cursor) {
	SET_STATUS(st_p, sec_attr_bad_cursor);
	return;
    }
    if(cursor->valid) {
	/* Error if cursor not initialized by this replica */
	if(!(uuid_equal(&(cursor->source), &(rs_state.rep_id), st_p))) {
	    SET_STATUS(st_p, sec_attr_bad_cursor);
	    return;
	}
    } else {
	rsdb_attr_cursor_init(rgy_obj_info, &cur_num_attrs, cursor, st_p);
    }
}


/*
 * r s d b _ a t t r _ s e m a n t i c _ c h e c k
 *
 * Evaluate the input attribute instances in attrs[] 
 * for semantic correctness according to their
 * corresponding schema entries provided in sch_list.
 * If an expected data field is missing, also report a failure.
 *
 * If the evaluation fails and can be attributed to a problem with
 * an input attribute, set failure_index to indicate which
 * element in attrs[] caused the failure.
 * Set failure_index to -1 if failure can't be attributed
 * to a given attribute.
 *
 * INPUT
 *      rgy_obj_info - identifies object with which attrs are associated
 *      num_attrs - number of attribute instances in attrs[] array
 *      attrs - array of sec_attr_t attribute instances
 *      sch_list - pointer to rsdb_attr_sch_list_t.
 *              Count is set to actual number of schema entries
 *              populated in entries[] subarray.
 *      xref -  an rsdb_attr_sch_attr_xref_t
 *              with number of elements equal to input "num_attrs."
 *              Array is populated with indices cross referencing
 *              the attrs[] and sch_list->entries[] arrays such that
 *		if attrs[i] is of type sch_list->entries[j], then
 *		xref[i] = j.
 *
 * OUTPUT:
 *      failure_index - on failure, set to the position of
 *              the problematic attribute in attrs[].
 *
 */
PUBLIC void rsdb_attr_semantic_check (
    rsdb_util_rgy_obj_info_t	*rgy_obj_info,		/* [in] */
    unsigned32                  num_attrs,              /* [in] */
    sec_attr_t                  attrs[],                /* [in] */
    rsdb_attr_sch_list_t        *sch_list,              /* [in] */
    rsdb_attr_sch_attr_xref_t   xref,                   /* [in] */
    signed32                    *failure_index,         /* [out] */
    error_status_t		*st_p
)
{
    signed32			i, j, k;
    signed32			c;
    sec_attr_schema_entry_t	*sch_entry_p;
    sec_attr_t			*sec_attr_p;

    CLEAR_STATUS(st_p);

    /* Evaluate each attribute instance in attrs[] for 
     * semantic correctness according to the rules in
     * the corresponding schema entry.
     */
    for(i = 0; i < num_attrs; i++) {
	sec_attr_p = &attrs[i];
	*failure_index = i;
	/* set j to identify the schema_entry for this attr */
	for(j = 0; j < sch_list->count; j++) {
	    if(xref[i] == j) {
		sch_entry_p = &(sch_list->entries[j].sch_entry);
		break;
	    }
	}
	/* detect failure to assign sch_entry_p */
	if(j >= sch_list->count) {
	    SET_STATUS(st_p, sec_s_pgmerr);
	    return;
	}
	/* Multi-value Check: return error if more than one attribute
	 * instance for a given type is passed in (indicated by 
	 * sch_list->entries[i].mv_update == true) and the 
	 * type is not multi-instanced.
	 */
	if( sch_list->entries[j].flags.mv_update &&
	    !(SA_SCH_FLAG_IS_SET_MULTI_INST(sch_entry_p)) ) {
	    SET_STATUS(st_p, sec_attr_not_multi_valued);
	    return;
	}
	
	if (SA_ATTR_ENCODING(sec_attr_p) == sec_attr_enc_void &&
	    sch_list->entries[j].flags.mv_update) {
	    SET_STATUS(st_p, sec_attr_multi_inst_disallowed);
	    return;
	}
		
	/* Encoding Type Check: return error if the attribute instance
	 * encoding type does not match the encoding type stored in
	 * the schema entry (unless schema_entry specifies enc_type_any).  
	 * Disallow an attribute instance with encoding_type_any.
	 * Disallow any attributes with sec_attr_enc_trig_binding.
	 */

	if (!SA_SCH_TRIG_FLAG_IS_QUERY(sch_entry_p)) {
	    if( (SA_ATTR_ENCODING(sec_attr_p) != sch_entry_p->attr_encoding &&
	        sch_entry_p->attr_encoding != sec_attr_enc_any)	||
	        SA_ATTR_ENCODING(sec_attr_p) == sec_attr_enc_any ||
		SA_ATTR_ENCODING(sec_attr_p) == sec_attr_enc_trig_binding) {
	        SET_STATUS(st_p, sec_attr_bad_encoding_type);
	        return;
	    }
	} else {
	    if( (SA_ATTR_ENCODING(sec_attr_p) != sec_attr_enc_void) ) {
	        SET_STATUS(st_p, sec_attr_qry_trig_bad_enc_type);
	        return;
	    }
	}
	/* Uniqueness Check: not yet implemented */
	/* Scope Check: not yet implemented, perhaps belongs elsewhere */
	/* Trigger Check: not yet implemented */

	/* Verify data format */
        switch(SA_ATTR_ENCODING(sec_attr_p)) {
        case sec_attr_enc_void:
        case sec_attr_enc_integer:
        case sec_attr_enc_uuid:
	    break;
        case sec_attr_enc_printstring:
	    if(!(RSDB_ATTR_PRINTSTRING_OK(SA_ATTR_PRINTSTRING_P(sec_attr_p)))) {
		SET_STATUS(st_p, sec_attr_val_printstring_bad);
		return;
	    }
	    break;
        case sec_attr_enc_printstring_array:
	    if(!(SA_ATTR_STR_ARRAY_P(sec_attr_p))) {
		SET_STATUS(st_p, sec_attr_val_string_array_bad);
		return;
	    }
	    for(c = 0; c < SA_ATTR_STR_ARRAY_NUM(sec_attr_p); c++) {
	        if(!(RSDB_ATTR_PRINTSTRING_OK(
		    SA_ATTR_STR_ARRAY_ELT_P(sec_attr_p, c)))) {
		    SET_STATUS(st_p, sec_attr_val_string_array_bad);
		    return;
	        }
	    }
	    break;
        case sec_attr_enc_bytes:
        case sec_attr_enc_confidential_bytes:
	    if( !(SA_ATTR_BYTES_P(sec_attr_p))) {
		SET_STATUS(st_p, sec_attr_val_bytes_bad);
		return;
	    }
	    break;
        case sec_attr_enc_i18n_data:
	    if( !(SA_ATTR_IDATA_P(sec_attr_p))) { 
		SET_STATUS(st_p, sec_attr_val_bytes_bad);
		return;
	    }
	    break;
        case sec_attr_enc_attr_set:
	    if(!(SA_ATTR_SET_P(sec_attr_p))) {
		SET_STATUS(st_p, sec_attr_val_bytes_bad);
		return;
	    }
	    break;
        case sec_attr_enc_binding:
	    if( !(SA_ATTR_BND_INFO_P(sec_attr_p)) ||
	        !(SA_ATTR_BND_AUTH_INFO_TYPE_VALID(sec_attr_p))) {
                SET_STATUS(st_p, sec_attr_bad_bind_info);
                return;
            }
	    for(k = 0; k < SA_ATTR_BND_NUM(sec_attr_p); k++) {
		switch(SA_ATTR_BND_TYPE(sec_attr_p, k)) {
		case sec_attr_bind_type_string:
		    if(!(SA_ATTR_BND_STRING_P(sec_attr_p, k))) {
			SET_STATUS(st_p, sec_attr_bad_bind_info);
			return;
		    }
		    break;
		case sec_attr_bind_type_twrs:
		    if(!(SA_ATTR_BND_TWRSET_P(sec_attr_p, k)) ||
			SA_ATTR_BND_TWRSET_COUNT(sec_attr_p, k) < 1) {
			SET_STATUS(st_p, sec_attr_bad_bind_info);
			return;
		    }
	    	    for(j = 0; j < SA_ATTR_BND_TWRSET_COUNT(sec_attr_p, k);
			j++) {
			if(!(SA_ATTR_BND_TWR_P(sec_attr_p, k, j))) {
			    SET_STATUS(st_p, sec_attr_bad_bind_info);
			    return;
			}
		    }
		    break;
		case sec_attr_bind_type_svrname:
		    if( !(SA_ATTR_BND_SVRNAME_P(sec_attr_p, k)) ||
			!(SA_ATTR_BND_SVRNAME_NAME_P(sec_attr_p, k))) {
			SET_STATUS(st_p, sec_attr_bad_bind_info);
			return;
		    }
		    break;
		} /* end switch on bind_type */
	    } /* end for each binding */
	    break;
        default:
	    SET_STATUS(st_p, sec_attr_bad_encoding_type);
	    return;
        } /* end enc_type switch */
    }

    /* We're here if no problems found */
    *failure_index = -1;
    return;
}


/*
 * r s d b _ a t t r _ u p d a t e
 *
 * Attach the input attribute instances in attrs[] to
 * the object identified by rgy_obj_info.
 * The caller is assumed to have checked the input attrs for
 * semantic correctness prior to calling rsdb_attr_update.
 * All pre-existing attributes of the same type as
 * an input attribute are deleted prior to adding the
 * new attribute(s).
 * Update the current number of attribute instances for the object.
 *
 * This is an atomic operation; either the entire update
 * completes successfully, or no updates take place.
 *
 * If the update fails and can be attributed to a problem with
 * an input attribute, set failure_index to indicate which
 * element in attrs[] caused the failure.
 * Set failure_index to -1 if failure can't be attributed
 * to a given attribute.
 *
 * INPUT
 *      rgy_obj_info - identifies object on which attrs are to be updated.
 *      num_attrs - number of attribute instances in attrs[] array
 *      attrs - array of sec_attr_t attribute instances
 *      sch_list - pointer to rsdb_attr_sch_list_t.
 *              Count is set to actual number of schema entries
 *              populated in entries[] subarray.
 *      xref -  an rsdb_attr_sch_attr_xref_t
 *              with number of elements equal to input "num_attrs."
 *              Array is populated with indices cross referencing
 *              the attrs[] and sch_list->entries[] arrays such that
 *		if attrs[i] is of type sch_list->entries[j], then
 *		xref[i] = j.
 *
 * OUTPUT:
 *      failure_index - on failure, set to the position of
 *              the problematic attribute in attrs[].
 *
 */
PUBLIC void rsdb_attr_update (
    rsdb_util_rgy_obj_info_t	*rgy_obj_info,		/* [in] */
    unsigned32                  num_attrs,              /* [in] */
    sec_attr_t                  attrs[],                /* [in] */
    rsdb_attr_sch_list_t        *sch_list,              /* [in] */
    rsdb_attr_sch_attr_xref_t   xref,                   /* [in] */
    signed32                    *failure_index,         /* [out] */
    error_status_t		*st_p
)
{
    rsdb_pvt_id_t		attr_type_id;
    rsdb_pvt_id_t		attr_inst_id;
    rsdb_attr_inst_t		*db_attr_p;
    sec_attr_t			*sec_attr_p;
    signed32			i;
    signed32			num_deleted = 0;


    /* At this point, we assume the updates will succeed
     * (barring lack of memory problems), so delete all
     * pre-existing attribute instances of the types being
     * updated.  The num_attrs field of this object is
     * updated by the rsdb_attr_delete_type_from_obj call.
     */
    for(i = 0; i < sch_list->count; i++) {
	rsdb_attr_delete_type_from_obj(rgy_obj_info, 
	    sch_list->entries[i].sch_entry_id, &num_deleted, st_p);
    }

    /* XXX Need to add code to ensure mv attr instance ids
     * are stored in consecutive slots in the attr_list
     */

    /* For each element in attrs[], import and store the data */
    for(i = 0; i < num_attrs; i++) {
	sec_attr_p = &attrs[i];
        rsdb_attr_import(sec_attr_p, &db_attr_p, st_p);
        if(BAD_STATUS(st_p))
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_RSDB_ATTR_IMPORT_MSG, *st_p);

        /* store seq_id of corresponding sch_entry (attr_type_id)
         * in threads.alias before calling store_new.
         */
        ATTR_TYPE_ID(db_attr_p) = sch_list->entries[xref[i]].sch_entry_id;

        /* store domain and seq_id of object to which this attr is attached */
        db_attr_p->domain = rgy_obj_info->domain;
        db_attr_p->object_id = rgy_obj_info->obj_id;
    
        rsdb_attr_store_new(rgy_obj_info, db_attr_p, &attr_inst_id, st_p);
        free(db_attr_p);
    }

    /* Increment the num_attrs field on the object to
     * which these attributes are being attached.
     */
    rsdb_attr_obj_upd_num_attrs(rgy_obj_info->domain, 
	rgy_obj_info->obj_id, num_attrs, st_p);

    return;
}



/*
 * r s d b _ a t t r _ d e l e t e
 *
 * Delete the input attribute instances in attrs[] from
 * the object identified by rgy_obj_info.
 * If an instance in attrs[] contains only a type uuid with
 * no value (encoding_type == void), delete all instances
 * of that type.  
 * Future Behavior (Not Yet Implemented): If an instance 
 * in attrs[] contains a type uuid and a value, then delete
 * the first instance found (if any) of that type and with that
 * exact value.
 * Update the current number of attribute instances for the object.
 *
 * This is an atomic operation; either the entire delete operation
 * completes successfully, or no deletes take place.
 *
 * If the delete fails and can be attributed to a problem with
 * an input attribute, set failure_index to indicate which
 * element in attrs[] caused the failure.
 * Set failure_index to -1 if failure can't be attributed
 * to a given attribute.
 *
 * INPUT
 *      rgy_obj_info - identifies object from which attrs are to be deleted.
 *      num_attrs - number of attribute instances in attrs[] array
 *      attrs - array of sec_attr_t attribute instances identifying the
 *		attribute types to be deleted
 *      sch_list - pointer to rsdb_attr_sch_list_t.
 *              Count is set to actual number of schema entries
 *              populated in entries[] subarray.
 *      xref -  an rsdb_attr_sch_attr_xref_t
 *              with number of elements equal to input "num_attrs."
 *              Array is populated with indices cross referencing
 *              the attrs[] and sch_list->entries[] arrays such that
 *		if attrs[i] is of type sch_list->entries[j], then
 *		xref[i] = j.
 *
 * OUTPUT:
 *      failure_index - on failure, set to the position of
 *              the problematic attribute in attrs[].
 *
 */
PUBLIC void rsdb_attr_delete (
    rsdb_util_rgy_obj_info_t	*rgy_obj_info,		/* [in] */
    unsigned32                  num_attrs,              /* [in] */
    sec_attr_t                  attrs[],                /* [in] */
    rsdb_attr_sch_list_t        *sch_list,              /* [in] */
    rsdb_attr_sch_attr_xref_t   xref,                   /* [in] */
    signed32                    *failure_index,         /* [out] */
    error_status_t		*st_p
)
{
    rsdb_pvt_id_t		attr_type_id;
    rsdb_pvt_id_t		attr_inst_id;
    rsdb_attr_inst_t		*db_attr_p;
    sec_attr_t			*sec_attr_p;
    signed32			i;
    signed32			num_deleted;

    CLEAR_STATUS(st_p);

    /* 
     * Delete all attribute instances of the types 
     * represented in sch_list.
     */
    /* 
     * Performance Note: This operation could be made more
     * efficient if the following call to rsdb_attr_delete_type_from_obj
     * were replaced with code that made updates to the seq_list,
     * obj_attr_list, and object itself such that these items only
     * had to be retrieved and stored once each instead of once
     * per deleted attr instance.
     */
    for(i = 0; i < sch_list->count; i++) {
	rsdb_attr_delete_type_from_obj(rgy_obj_info, 
	    sch_list->entries[i].sch_entry_id, &num_deleted, st_p);
    }

    /* In a future version, add code that determines whether
     * an instance in attrs[] contains a value and, if so,
     * deletes only the instance with a matching value, instead
     * of all instances of that type.
     */

    return;
}

/*
 * r s d b _ a t t r _ d e l e t e _ t y p e _ f r o m _ o b j
 *
 * Delete all attr instances of the input type (attr_type_id)
 * from the object represented by rgy_obj_info. 
 * The object's num_attrs field is updated as a result of
 * this call.
 * If none of the input type are found, return success.
 * Return the number of instances deleted in num_deleted.
 *
 * Status Codes:
 *	sec_attr_inst_not_found	if attr not found with attr_id.
 *	error_status_ok		otherwise.
 */
PUBLIC void rsdb_attr_delete_type_from_obj(
    rsdb_util_rgy_obj_info_t	*rgy_obj_info,  /* [in] */
    rsdb_pvt_id_t		attr_type_id,	/* [in] */
    signed32			*num_deleted,	/* [out] */
    error_status_t		*st_p
)
{
    rsdb_pvt_id_t       	attr_inst_id;
    rsdb_pvt_id_t       	attr_list_id;
    rsdb_max_attr_list_t        list;
    boolean32			type_found = false;
    signed32			i;

    CLEAR_STATUS(st_p);
    *num_deleted = 0;

    attr_list_id = rgy_obj_info->attr_list_id;

    if (attr_list_id == END_OF_LIST) {
        return ;
    }

    while (attr_list_id != END_OF_LIST) {
	if(!(rsdb_attr_list_get_by_id(attr_list_id, &list))) {
            dce_svc_printf(SEC_ATTR_CANT_GET_ATTRLIST_MSG);
	}
        for (i = 0; i < list.total_count; i++) {
            if(!list.attr[i].inst_id.deleted) {
		if(list.attr[i].type_id == attr_type_id) {
		    type_found = true;
		    attr_inst_id = list.attr[i].inst_id.id;
		    rsdb_attr_delete_inst(rgy_obj_info, attr_inst_id, st_p);
		    if(BAD_STATUS(st_p))
        	        SEC_SVC_PRINT_MSG_ST(SEC_RGY_RSDB_ATTR_DELETE_MSG, *st_p);
		    (*num_deleted)++;

/* Put this back in when instances of the same
 * type are stored sequentially
 *
		} else if(type_found) {
		    * If type was found, but we're no longer
		     * matching on type_id, there are no more
		     * instances of this type, so return.
		     *
		    return;
*/
		}
            }
        }
        if (list.next == END_OF_LIST)
            break;
        attr_list_id = list.next;
    }
    return;
}


/*
 * r s d b _ a t t r _ l o o k u p _ b y _ u u i d
 *
 * Allocate internal buffers for and retrieve the attribute instance(s)
 * whose types are represented in sch_list->entries[].  
 * Return instances in attrs[].
 * If some attributes could not be returned due to space constraints,
 * in the attrs[] buffer, num_left will be set to the number remaining
 * at the time of the call.  If num_left is greater than zero, a
 * subsequent call to this routine -- with the *same* sch_list->entries array
 * and the output cursor from the previous call -- will return
 * the next batch of attributes beginning where the previous call
 * left off.  
 * Updates to this object's attributes between 
 * lookup calls may result in inconsistent results.
 *
 * Cursor is assumed to have been checked with
 * rsdb_attr_cursor_check or initialized with
 * rsdb_attr_cursor_init before calling this operation.
 *
 * If "allocator" is not rpc_ss_allocate, caller 
 * must free allocated buffers by calling, for each sec_attr_t
 * returned in attrs[]:
 * sec_attr_util_inst_free_ptrs(&attrs[i]) when done.
 *
 * INPUT
 *	rgy_obj_info - contains obj_id, domain, and obj_type of
 *		object whose attrs are to be retrieved
 *	space_avail - size of the output attrs[] array.
 *      sch_list - pointer to rsdb_attr_sch_list_t.
 *              Count is set to actual number of schema entries
 *              populated in entries[] subarray.
 *	allocator - if NULL, use malloc for sec_attr_t internal buffers 
 *		in the attrs[] array and caller must free buffers.
 *		Set allocator to rpc_ss_allocate when calling from
 *		manager code called from a server stub so that 
 *		memory is automatically freed.
 * INPUT/OUTPUT:
 *	cursor - [in] identifies the next attribute instance(s)
 *		on this object to return in attrs[].
 *		[out] points to the next instance(s) to be returned
 *		in a subsequent call.
 * OUTPUT:
 *	num_returned - specifies the number of attribute instances
 *		returned in the attrs[] array.
 *	attrs - array of attribute instances retrieved by id.
 *	num_left - hints at the number of attributes matching the
 *		input attr_keys that could not be returned due
 *		to space constraints in the attrs buffer.
 *
 * Status Codes:
 *	sec_attr_not_all_avail	warning: some of the requested 
 *				attributes do not exist on the object
 */
PUBLIC void rsdb_attr_lookup_by_uuid(
    rsdb_util_rgy_obj_info_t	*rgy_obj_info,	/* [in] */
    rs_attr_cursor_t            *cursor,        /* [in, out] */
    unsigned32                  space_avail,    /* [in] */
    rsdb_attr_sch_list_t	*sch_list,      /* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    unsigned32                  *num_returned,  /* [out] */
    sec_attr_t                  attrs[],        /* [out] */
    unsigned32                  *num_left,      /* [out] */
    error_status_t		*st_p
)
{
    rsdb_pvt_id_t       	attr_inst_id;
    rsdb_max_attr_list_t        list;
    boolean32			type_found = false;
    boolean32			count_entries = false;
    signed32			i, j, inital_num_entries_left = NULL;
    sec_attr_schema_entry_t     *sch_entry_p;

    CLEAR_STATUS(st_p);
    *num_returned = 0;
    *num_left = 0;

    /* if cursor->list is the object's first attr_list AND
     * cursor->entry == 0, this is the first lookup on an
     * initialized cursor.  This means we need to calculate 
     * and set cursor->num_entries_left to the total number 
     * of entries of the types specified in sch_list.
     */
    if(cursor->entry == 0 && cursor->list == rgy_obj_info->attr_list_id) {
	count_entries = true;
    }

    if (cursor->list == END_OF_LIST) {
	SET_STATUS(st_p, sec_attr_no_more_entries);
        return ;
    }

    while (cursor->list != END_OF_LIST) {
	if(!(rsdb_attr_list_get_by_id(cursor->list, &list))) {
            dce_svc_printf(SEC_ATTR_CANT_GET_ATTRLIST_MSG);
	}
        for(i = cursor->entry; i < list.total_count; i++) {
	    if(*num_returned < space_avail) {
                if(!list.attr[i].inst_id.deleted) {
		    for(j = 0; j < sch_list->count; j++) {
		        if(list.attr[i].type_id == 
			    sch_list->entries[j].sch_entry_id) {
		            attr_inst_id = list.attr[i].inst_id.id;
			    rsdb_attr_get_by_id_and_export(attr_inst_id,
			        allocator, &attrs[*num_returned], st_p);
		            if(BAD_STATUS(st_p)) {
                                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                                           sec_s_rs_rsdb,
                                           svc_c_sev_fatal | svc_c_action_abort,
                                           *st_p);
		            }
			    sch_entry_p = &(sch_list->entries[j].sch_entry);

			    if (SA_SCH_TRIG_FLAG_IS_QUERY(sch_entry_p)) {
			        rsdb_attr_export_trig_binding(sch_entry_p,
				    allocator,  &attrs[*num_returned], st_p);
		                if(BAD_STATUS(st_p)) {
                                    dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                                           sec_s_rs_rsdb,
                                           svc_c_sev_fatal | svc_c_action_abort,
                                           *st_p);
		                }
			    }
			    SA_ATTR_ID(&attrs[*num_returned]) = 
			        sch_list->entries[j].sch_entry.attr_id;
			        (*num_returned)++;
			    break;
		        }
		           /* Put in optimization when instances of the same
 			    * type are stored sequentially
  			    * If type was found, but we're no longer
   			    * matching on type_id, there are no more
  			    * instances of this type,
			    */
		    }
                }
	    } else if(count_entries) {
                if(!list.attr[i].inst_id.deleted) {
		    for(j = 0; j < sch_list->count; j++) {
		        if(list.attr[i].type_id == 
			    sch_list->entries[j].sch_entry_id) 
    				inital_num_entries_left++;
		    }
		}	
	    }
        }
	if(*num_returned == space_avail) {
	    /* if we filled the space_avail and also walked
	     * through the entire list, set cursor to next list.
	     * Otherwise, just set cursor->entry to next index.
	     */
	    if(i >= list.total_count) {
        	cursor->list = list.next;
		cursor->entry = 0;
	    } else {
	        cursor->entry = i;
	    }
	    break;
	}
        cursor->list = list.next;
	cursor->entry = 0;
    }
    if(count_entries) {
	*num_left = cursor->num_entries_left = inital_num_entries_left;
    } else {
        cursor->num_entries_left -= *num_returned;
        *num_left = cursor->num_entries_left;
    }
    if (*num_returned == 0) {
	SET_STATUS(st_p, sec_attr_no_more_entries);
    }
    return;
}


/*
 * r s d b _ a t t r _ g e t _ e f f e c t i v e 
 *
 * Allocate the "attrs" array for the return of all attribute instance(s)
 * whose types are represented in sch_list->entries[].  
 *
 * If "allocator" is not rpc_ss_allocate, caller 
 * must free allocated internal buffers by calling, for each sec_attr_t
 * returned in "attrs":
 * sec_attr_util_inst_free_ptrs(&attrs[i]) 
 * and then must free attrs itself.
 *
 * INPUT
 *	rgy_obj_info - contains obj_id, domain, and obj_type of
 *		object whose attrs are to be retrieved
 *      sch_list - pointer to rsdb_attr_sch_list_t.
 *              Count is set to actual number of schema entries
 *              populated in entries[] subarray.
 *	allocator - if NULL, use malloc for sec_attr_t internal buffers 
 *		in the attrs array and for the attrs array itself
 *		(caller must free attrs buffers).  Caller should
 *		set allocator to rpc_ss_allocate when calling from
 *		manager code called from a server stub so that 
 *		memory gets automatically freed.
 * OUTPUT:
 *	num_returned - specifies the number of attribute instances
 *		returned in the attrs array.
 *	attrs - array of attribute instances retrieved by id.
 *
 * Status Codes:
 *	sec_attr_not_all_avail	warning: some of the requested 
 *				attributes do not exist on the object
 */
PUBLIC void rsdb_attr_get_effective(
    rsdb_util_rgy_obj_info_t	*rgy_obj_info,	/* [in] */
    rsdb_attr_sch_list_t	*sch_list,      /* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    unsigned32                  *num_returned,  /* [out] */
    sec_attr_t                  **attrs,        /* [out] */
    error_status_t		*st_p
)
{
    rsdb_pvt_id_t       	attr_inst_id;
    rsdb_pvt_id_t       	attr_list_id;
    rsdb_max_attr_list_t        list;
    sec_attr_t			*attr_p;
    signed32			i, j;
    unsigned32			num_attrs;
    unsigned32			num_lists;
    sec_attr_schema_entry_t     *sch_entry_p;

    CLEAR_STATUS(st_p);
    *attrs = NULL;
    *num_returned = 0;
    num_attrs = 0;
    num_lists = 0; 	/* keep track for optimization */

    /* First, count the number of attribute instances
     * to determine size required for attrs array.
     */
    attr_list_id = rgy_obj_info->attr_list_id;
    while(attr_list_id != END_OF_LIST) {
	if(!(rsdb_attr_list_get_by_id(attr_list_id, &list))) {
            dce_svc_printf(SEC_ATTR_CANT_GET_ATTRLIST_MSG);
	}
	num_lists++;
        for(i = 0; i < list.total_count; i++) {
            if(!list.attr[i].inst_id.deleted) {
		for(j = 0; j < sch_list->count; j++) {
		    if(sch_list->entries[j].flags.deleted) {
			continue;
		    }
		    if(list.attr[i].type_id == 
			sch_list->entries[j].sch_entry_id) {
			num_attrs++;
		    }
		       /* Put in optimization when instances of the same
 			* type are stored sequentially
  			* If type was found, but we're no longer
   			* matching on type_id, there are no more
  			* instances of this type,
			*/
		}
            }
        }
	attr_list_id = list.next;
    }

    if(num_attrs < 1) {
	SET_STATUS(st_p, sec_attr_no_more_entries);
	return;
    }

    /* "allocator" is used by this macro */
    ATTR_ALLOC(*attrs, num_attrs * sizeof(sec_attr_t), st_p);
    if(BAD_STATUS(st_p)) {
	return;
    }

    /* Now, retrieve the requested attribute instances
     * and put them in attrs.
     */
    attr_list_id = rgy_obj_info->attr_list_id;
    attr_p = *attrs;
    while(attr_list_id != END_OF_LIST) {
	/* If only one attr_list exists, optimize by not
	 * retrieving it again.  Otherwise, retrieve the first.
	 */
	if(num_lists > 1) {
	    if(!(rsdb_attr_list_get_by_id(attr_list_id, &list))) {
                dce_svc_printf(SEC_ATTR_CANT_GET_ATTRLIST_MSG);
	    }
	}
        for(i = 0; i < list.total_count; i++) {
            if(!list.attr[i].inst_id.deleted) {
		for(j = 0; j < sch_list->count; j++) {
		    if(sch_list->entries[j].flags.deleted) {
			continue;
		    }
		    if(list.attr[i].type_id == 
			sch_list->entries[j].sch_entry_id) {
		        attr_inst_id = list.attr[i].inst_id.id;
			rsdb_attr_get_by_id_and_export(attr_inst_id,
			    allocator, attr_p, st_p);
		        if(BAD_STATUS(st_p)) {
                            SEC_SVC_PRINT_MSG_ST(SEC_RGY_RSDB_ATTR_EXPORT_MSG, *st_p);
		        }
			sch_entry_p = &(sch_list->entries[j].sch_entry);
			if (SA_SCH_TRIG_FLAG_IS_QUERY(sch_entry_p)) {
			    rsdb_attr_export_trig_binding(sch_entry_p, allocator,
				attr_p, st_p);
		            if(BAD_STATUS(st_p)) {
                                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                                           sec_s_rs_rsdb,
                                           svc_c_sev_fatal | svc_c_action_abort,
                                           *st_p);
		            }
			}
			SA_ATTR_ID(attr_p) = 
			    sch_list->entries[j].sch_entry.attr_id;
			(*num_returned)++;
			attr_p++;
			break;
		    }
		       /* Put in optimization when instances of the same
 			* type are stored sequentially
  			* If type was found, but we're no longer
   			* matching on type_id, there are no more
  			* instances of this type,
			*/
		}
            }
        }
	attr_list_id = list.next;
    }
    return;
}


/*
 * r s d b _ a t t r _ l o o k u p _ b y _ n a m e
 *
 * Allocate internal buffers for and retrieve the first attr instance
 * whose type is identified by attr_name.
 * If allocator is not rpc_ss_allocate, caller 
 * must free allocated buffers by calling
 * sec_attr_util_inst_free_ptrs(sec_attr_p) when done.
 *
 * INPUT
 *	rgy_obj_info - contains obj_id, domain, and obj_type of
 *		object whose attrs are to be retrieved
 *	attr_name - attribute name identifying the type
 *		of the attribute instances to be retrieved
 *	allocator - if NULL, use malloc for sec_attr_p buffers and
 *		caller must free buffers.
 *		Set allocator to rpc_ss_allocate when calling from
 *		manager code called from a server stub so that 
 *		memory is automatically freed.
 * OUTPUT:
 *	sec_attr_p - pointer to a sec_attr_t.  On output, internal
 *		pointers have been allocated and data copied in.
 *
 * Status Codes:
 *	sec_attr_inst_not_found	attribute instance not found 
 *	sec_attr_bad_type	attribute type "attr_name" not found
 *	error_status_ok		otherwise.
 */
PUBLIC void rsdb_attr_lookup_by_name(
    rsdb_util_rgy_obj_info_t	*rgy_obj_info,			/* [in] */
    char			*attr_name,			/* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    sec_attr_t			*sec_attr_p,			/* [out] */
    error_status_t		*st_p
)
{
    rsdb_pvt_id_t		attr_type_id;
    rsdb_pvt_id_t		attr_list_id;
    rsdb_pvt_id_t		attr_inst_id;
    rsdb_attr_schema_entry_t    *db_sch_entry_p;
    sec_attr_schema_entry_t     sch_entry;
    rsdb_max_attr_list_t	attr_list;
    signed32			index;

    CLEAR_STATUS(st_p);

    /* get the attr_type_id from the attr_name */
    if(!(rsdb_attr_sch_get_by_name(attr_name, &db_sch_entry_p, st_p))) {
	SET_STATUS(st_p, sec_attr_bad_type);
	return;
    }

    attr_type_id = db_sch_entry_p->threads.id;

    /* look for the first attribute instance of requested type
     * on the attr_list for this object
     */
    attr_list_id = rgy_obj_info->attr_list_id;
    if(!(rsdb_attr_list_get_inst_by_type(attr_type_id, &attr_list_id,
	&index, &attr_inst_id))) {
        free(db_sch_entry_p);
	SET_STATUS(st_p, sec_attr_inst_not_found);
	return;
    }

    /* get attr instance */
    rsdb_attr_get_by_id_and_export(attr_inst_id, allocator, 
	sec_attr_p, st_p);

    if (SA_SCH_TRIG_FLAG_IS_QUERY(db_sch_entry_p)) {
        rsdb_attr_sch_export(db_sch_entry_p, allocator, &sch_entry, st_p);
        if(BAD_STATUS(st_p)) {
            free(db_sch_entry_p);
            return;
        }
        rsdb_attr_export_trig_binding(&sch_entry, allocator, sec_attr_p, st_p);
        if(BAD_STATUS(st_p)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                sec_s_rs_rsdb,
                svc_c_sev_fatal | svc_c_action_abort,
                *st_p);
        }
        if(!allocator) {
            sec_attr_util_sch_ent_free_ptrs(&sch_entry);
        }
    }

    SA_ATTR_ID(sec_attr_p) = db_sch_entry_p->attr_id;
    free(db_sch_entry_p);
    return;
}

/*
 * r s d b _ a t t r _ l o o k u p 1 _ b y _ u u i d
 *
 * Allocate internal buffers for and retrieve the first attr instance
 * whose type is identified by attr_uuid.
 * If allocator is not rpc_ss_allocate, caller 
 * must free allocated buffers by calling
 * sec_attr_util_inst_free_ptrs(sec_attr_p) when done.
 *
 * INPUT
 *	rgy_obj_info - contains obj_id, domain, and obj_type of
 *		object whose attrs are to be retrieved
 *	attr_uuid - attribute name identifying the type
 *		of the attribute instances to be retrieved
 *	allocator - if NULL, use malloc for sec_attr_p buffers and
 *		caller must free buffers.
 *		Set allocator to rpc_ss_allocate when calling from
 *		manager code called from a server stub so that 
 *		memory is automatically freed.
 * OUTPUT:
 *	sec_attr_p - pointer to a sec_attr_t.  On output, internal
 *		pointers have been allocated and data copied in.
 *
 * Status Codes:
 *	sec_attr_inst_not_found	attribute instance not found 
 *	sec_attr_bad_type	attribute type "attr_name" not found
 *	error_status_ok		otherwise.
 */
PUBLIC void rsdb_attr_lookup1_by_uuid(
    rsdb_util_rgy_obj_info_t	*rgy_obj_info,			/* [in] */
    uuid_t			*attr_uuid,			/* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    sec_attr_t			*sec_attr_p,			/* [out] */
    error_status_t		*st_p
)
{
    rsdb_pvt_id_t		attr_type_id;
    rsdb_pvt_id_t		attr_list_id;
    rsdb_pvt_id_t		attr_inst_id;
    rsdb_attr_schema_entry_t    *db_sch_entry_p;
    sec_attr_schema_entry_t     sch_entry;
    rsdb_max_attr_list_t	attr_list;
    signed32			index;

    CLEAR_STATUS(st_p);

    /* get the attr_type_id from the attr_name */
    if(!(rsdb_attr_sch_get_by_uuid(attr_uuid, &db_sch_entry_p, st_p))) {
	SET_STATUS(st_p, sec_attr_bad_type);
	return;
    }

    attr_type_id = db_sch_entry_p->threads.id;

    /* look for the first attribute instance of requested type
     * on the attr_list for this object
     */
    attr_list_id = rgy_obj_info->attr_list_id;
    if(!(rsdb_attr_list_get_inst_by_type(attr_type_id, &attr_list_id,
	&index, &attr_inst_id))) {
        free(db_sch_entry_p);
	SET_STATUS(st_p, sec_attr_inst_not_found);
	return;
    }

    /* get attr instance */
    rsdb_attr_get_by_id_and_export(attr_inst_id, allocator, 
	sec_attr_p, st_p);

    if (SA_SCH_TRIG_FLAG_IS_QUERY(db_sch_entry_p)) {
        rsdb_attr_sch_export(db_sch_entry_p, allocator, &sch_entry, st_p);
        if(BAD_STATUS(st_p)) {
            free(db_sch_entry_p);
            return;
        }
        rsdb_attr_export_trig_binding(&sch_entry, allocator, sec_attr_p, st_p);
        if(BAD_STATUS(st_p)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                sec_s_rs_rsdb,
                svc_c_sev_fatal | svc_c_action_abort,
                *st_p);
        }
        if(!allocator) {
            sec_attr_util_sch_ent_free_ptrs(&sch_entry);
        }
    }

    SA_ATTR_ID(sec_attr_p) = db_sch_entry_p->attr_id;
    free(db_sch_entry_p);
    return;
}

