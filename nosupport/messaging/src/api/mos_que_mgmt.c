/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 *  m o s _ q u e _ m g m t. c
 *
 *  Client interfaces for Queue Management to support the 
 *  Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 *
 * $Log: mos_que_mgmt.c,v $
 * Revision 1.12  1996/12/02  16:17:29  salamone
 * Fix que move comparison
 *
 * Revision 1.11  96/11/20  14:48:05  14:48:05  salamone (Julie Salamone)
 * Fix header file declarations
 * 
 * Revision 1.10  96/11/12  10:41:08  10:41:08  salamone (Julie Salamone)
 * Removed cursor parameter in mos_que_mgmt_msgid_list()
 * 
 * Revision 1.9  96/11/07  18:03:55  18:03:55  salamone (Julie Salamone)
 * Add msg move and que move support
 * 
 * Revision 1.8  96/10/31  15:25:45  15:25:45  salamone (Julie Salamone)
 * Added selection filter to mos_q_get_id_list() call
 * 
 * Revision 1.7  96/10/25  14:43:01  14:43:01  salamone (Julie Salamone)
 * Add rsrc protectlvl support
 * 
 * Revision 1.6  96/10/23  16:14:33  16:14:33  salamone (Julie Salamone)
 * Add selection filter support
 * 
 * Revision 1.5  96/10/16  17:04:56  17:04:56  salamone (Julie Salamone)
 * Added resource error codes
 * 
 * Revision 1.4  96/10/16  15:00:22  15:00:22  salamone (Julie Salamone)
 * On list() APIs if empty list, return with no error
 * 
 * Revision 1.3  96/10/15  17:29:36  17:29:36  salamone (Julie Salamone)
 * Spell mos_c_delop_preserve correctly
 * 
 * Revision 1.2  96/10/15  17:04:23  17:04:23  salamone (Julie Salamone)
 * Validate queue deletion options
 * 
 * Revision 1.1  96/10/14  16:51:38  16:51:38  salamone (Julie Salamone)
 * Initial revision
 * 
 * $EndLog$
 * ______________________________________________________________________
 */


/* 
 * INCLUDE FILES  
 */
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dce_svc.h>
#include <dce/nbase.h>
#include <dce/assert.h>
#include <dce/aclif.h>
#include <dce/utc.h>
#include <dce/id_epac.h>

#include <mosif.h>
#include <mos_api.h>
#include <mos_base.h>
#include <mos_q.h>
#include <dcemosmsg.h>
#include <dcemosmac.h>
#include <dcemossvc.h>

/*
 * INTERNAL PROTOTYPES
 */
boolean mos__is_delop_valid(mos_que_delete_op_t, error_status_t *);
boolean mos__is_mvop_valid(unsigned32, error_status_t *);
void mos__que_move_on_same_qmgr(mos_rsrc_name_t, unsigned32, mos_handle_t *, error_status_t *);
void mos__que_move_to_diff_qmgr(mos_handle_t, mos_rsrc_name_t, unsigned32, mos_handle_t *, error_status_t *);
void mos__que_attr_enqdeq_commit(mos_handle_t, boolean, boolean);


/*
 *  FUNCTION:
 *  m o s _ q u e _ m g m t _ c r e a t e
 *
 *  OVERVIEW:
 *  Create one queue and commit its queue attributes to be managed by 
 *  one queue manager. 
 *
 *  INPUTS:
 *  qmgr_handle:
 *                  A context handle to a queue manager.
 *                  To speed look-ups, a context handle to an NSI group
 *                  or profile is prohibited.
 *                  If NULL, the default queue manager is used.
 *
 *  name:
 *                  Specifies queue name to create.
 *                  If NULL, the name is generated from the queue
 *                  manager name concatenated with the queue UUID.
 *                  If a simple name, the simple name is created as
 *                  a junction from the queue manager name.
 *
 *  queue_attributes:
 *                  Specifies the queue attributes when creating a queue.
 *                  Use mos_que_attr_alloc() to allocate a buffer and use
 *                  the other mos_que_attr_X_set() APIs to modify any
 *                  attributes.
 *
 *                  If NULL is specified, the API allocates the queue
 *                  attributes and sets the defaults. Otherwise, it is
 *                  the responsibility of the caller to release the
 *                  memory for the returned queue attributes structure
 *                  via mos_que_attr_free().
 *
 *  OUTPUTS:
 *  que_handle:
 *                  A context handle to the new queue.
 *
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_mgmt_create(
                    mos_handle_t          qmgr_handle,
                    mos_rsrc_name_t       name,
                    mos_que_attr_t        que_attributes,
                    mos_handle_t *        que_handle,
                    error_status_t *      status
                   )
{
    error_status_t                 temp_status;
    boolean                        api_que_attr_alloc = FALSE;
    mos_rsrc_name_t                dflt_qmgr_name;
    mos_handle_t                   dflt_qmgr_handle;
    mos_rsrc_info_t *              rsrc_item_p;
    mos_rsrc_info_t *              temp_rsrc_item_p;

    /*
     * Initialize output parameters
     */
    *que_handle = NULL;
    CLEAR_STATUS(status);

    /*
     * Validate handle
     */
    if (qmgr_handle == NULL){
        mos_rsrc_dflt_quemgr_get((void *)NULL,
                                 dflt_qmgr_name,
                                 &dflt_qmgr_handle,
                                 status);
        if (BAD_STATUS(status)) {
           return;
        }
        temp_rsrc_item_p = dflt_qmgr_handle;
    } else {
        if (mos__is_rsrc_handle_valid(FALSE, qmgr_handle, status)) {
           temp_rsrc_item_p = qmgr_handle;
        } else {
           return;
        }
    }

    /*
     * Determine the authentication level to use
     */
    mos__auth_info_set(temp_rsrc_item_p, temp_rsrc_item_p->protectlvl, status);
    if (BAD_STATUS(status)) {
       return;
    }

    /*
     * Determine if "que_attributes" is NULL.
     * If NULL is specified, API automatically allocates and sets all
     * defaults for the queue attributes.
     * Otherwise, it is the caller's responsibility to release
     * the memory allocated for the queue attributes structure
     * via mos_que_attr_free().
     */
    if (que_attributes == NULL) {
       api_que_attr_alloc = TRUE;
       mos_que_attr_alloc(&que_attributes, status);
       if (BAD_STATUS(status)) {
          return;
       }
    } 

    /*
     * If specified, set name in queue attributes 
     */
    if (name) {
       que_attributes->name = (idl_char *)dce_strdup((char *)name);     
    }

    /*
     * Talk to queue manager and attempt to create queue
     */
    mos_mgmt_create_queue(temp_rsrc_item_p->server_handle, 
                          que_attributes, 
                          status);

    if (GOOD_STATUS(status)) {
        /*
         * Allocate space for a resource
         */
        mos__rsrc_info_create(&rsrc_item_p, status);
        if (GOOD_STATUS(status)) {
            /*
             * Copy appropriate resource information to new resource
             */
            rsrc_item_p->server_handle = temp_rsrc_item_p->server_handle;
            strcpy((char *)rsrc_item_p->security_name, 
                               (char *)temp_rsrc_item_p->security_name);
            strcpy((char *)rsrc_item_p->server_name,   
                               (char *)temp_rsrc_item_p->server_name);
            strcpy((char *)rsrc_item_p->server_prin_name, 
                               (char *)temp_rsrc_item_p->server_prin_name);
            rsrc_item_p->que_id = que_attributes->id;
            strcpy((char *)rsrc_item_p->resource_name, 
                               (char *)que_attributes->name);
            /* 
             * Here, we use protection level of default queue
             * manager or already attached resource and not 
             * necessarily mos_c_protect_level_default.
             */
            rsrc_item_p->protectlvl    = temp_rsrc_item_p->protectlvl;
        }
    }

    /*
     * If we allocated the queue attributes structure, then we'll free it
     * whether good or bad status.
     */
    if (api_que_attr_alloc) {
       mos_que_attr_free(&que_attributes, &temp_status);
    }

    /*
     * Add new resource to list of resources and
     * return resource handle to caller
     */
    if (GOOD_STATUS(status)) {
       mos__rsrc_info_append(rsrc_item_p);
       *que_handle = (mos_handle_t)rsrc_item_p;
    }
    return;

} /* End of mos_que_mgmt_create() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ m g m t _ d e l e t e
 *
 *  OVERVIEW:
 *  Delete one queue managed by the specified queue manager.
 *
 *  Since a named link to a queue manager may still be required
 *  to access another queue, the API does NOT delete all named
 *  links on each queue alias. It is the responsibility of the
 *  caller to delete any named link that was created implicitly
 *  via mos_que_mgmt_create()/mos_que_mgmt_commit() or explicitly
 *  via dcecp link create command. If a named link is no longer
 *  required; use mos_que_mgmt_commit() in conjuction with
 *  mos_que_attr_alias_remove() or use dcecp link delete command. 
 *
 *  INPUTS:
 *  flags:
 *                  Indicates what to do with queues that are non-empty.
 *                  The default is mos_c_delop_preserve.
 *
 *  INPUTS/OUTPUTS:
 *  resource_handle:
 *                  A context handle to the resource obtained via
 *                  mos_que_mgmt_create() or mos_rsrc_attach().
 *                  If the delete operation succeeds, this parameter
 *                  returns the value NULL.
 *                  Note that the caller must have the access rights
 *                  to delete the queue.
 *                  To speed look-ups, a context handle to an NSI group
 *                  or profile is prohibited.
 *
 *  OUTPUTS:
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_mgmt_delete(
                    mos_que_delete_op_t   flags,
                    mos_handle_t *        resource_handle,
                    error_status_t *      status
                   )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate handle
     */
    if (resource_handle == NULL) {
       SET_STATUS(status, mos_s_rsrc_handle_inv);  
       return;
    }
    if (! mos__is_rsrc_handle_valid(TRUE, *resource_handle, status)) {
       return;
    }

    /*
     * Validate queue deletion options
     */
    if (! mos__is_delop_valid(flags, status)) {
       return;
    }

    /*
     * Determine the authentication level to use
     */
    mos__auth_info_set(*resource_handle,
                       (*resource_handle)->protectlvl, 
                       status);

    /*
     * Request to delete queue
     */
    if (GOOD_STATUS(status)) {
        mos_q_delete_queue((*resource_handle)->server_handle,
                           (*resource_handle)->resource_name,
                           flags,
                           status);
    }

    /*
     * Remove resource from list of resources 
     */
    if (GOOD_STATUS(status)) {
        mos__rsrc_info_remove(*resource_handle, 
                              status);
        *resource_handle = NULL;
    }
    return;

} /* End of mos_que_mgmt_delete() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ d e l o p _ v a l i d
 *
 *  OVERVIEW:
 *  Validate queue deletion options.
 *
 *  INPUTS:
 *  delete_option:
 *                  Indicates what to do with queues that are non-empty.
 *                  The default is mos_c_delop_preserve.
 *
 *  OUTPUTS:
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        TRUE  indicates validation was successful.
 *                  FALSE indicates validation was not successful.
 */
boolean
mos__is_delop_valid(
                    mos_que_delete_op_t         delete_option,
                    error_status_t *            status
                   )
{
    CLEAR_STATUS(status);

    switch (delete_option) {
       case (mos_c_delop_preserve):
       case (mos_c_delop_force):
           return(TRUE);
       default:
           SET_STATUS(status, mos_s_que_delop_inv);  
           return(FALSE);
    } /* End of switch on queue delete options */

} /* End of mos__is_delop_valid() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ m g m t _ q u e n a m e _ l i s t
 *
 *  OVERVIEW:
 *  Return list of queues by name managed by specified Q-mgr.
 *
 *  INPUTS:
 *  resource_handle:
 *                  A context handle to the resource obtained via
 *                  mos_que_mgmt_create() or mos_rsrc_attach().
 *
 *                  To speed look-ups, a context handle to an NSI group
 *                  or profile is prohibited.
 *
 *  space_avail:
 *                  Specifies the size of the "que_name_list" array.
 *                  The caller is responsible for allocating the space
 *                  for "space_avail" queue names.
 *
 *
 *  OUTPUTS:
 *  num_returned:
 *                  The number of queues managed by the specified Q-mgr
 *                  returned in the "que_name_list".
 *
 *  que_name_list:
 *                  Array of queue names returned.
 *                  The size of the array is determined by "space_avail"
 *                  and the length by "num_returned".
 *
 *  num_left:
 *                  Number of queues that were found but could not
 *                  be returned because of space constraints of
 *                  "que_name_list" array. Note that this number may be
 *                  inaccurate if queues have been created/deleted
 *                  bewteen successive queries.
 *
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_mgmt_quename_list(
                          mos_handle_t            resource_handle,
                          unsigned32              space_avail,
                          unsigned32 *            num_returned,
                          mos_rsrc_name_t         que_name_list[],
                          unsigned32 *            num_left,
                          error_status_t *        status
                         )
{
    mos_string_list_t            names;
    int                          i;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);
    *num_returned = 0;
    *num_left = 0;
    if (space_avail) {
        for (i = 0; i < space_avail; i++) {   
            *que_name_list[i] = '\0';
        }
    } else {
        SET_STATUS(status, mos_s_space_avail_req);  
        return;
    }

    /*
     * Validate handle
     */
    if (! mos__is_rsrc_handle_valid(FALSE, resource_handle, status)) {
       return;
    }

    /*
     * Determine the authentication level to use
     */
    mos__auth_info_set(resource_handle, resource_handle->protectlvl, status);
    if (BAD_STATUS(status)) {
        return;
    } 

    /*
     * Request to get list of queue names from Qmgr
     */
    mos_mgmt_get_name_list(resource_handle->server_handle,
                           &names,
                           status);
    
    if ((BAD_STATUS(status)) ||
        (EMPTY_LIST(&names))) {
        return;
    } 

    /*
     * Return only what was allocated
     */
    for (i = 0; i < names.count; i++){
        if (i < space_avail) {
           strcpy((char *)que_name_list[i], (char *)names.list[i]);
           (*num_returned)++;
        } else {
           (*num_left)++; 
        }
    } /* End of loop thru all queue names */

    /* 
     * Release list of queue names 
     */
    mos_string_list_free_ptrs(&names);

    return;

} /* End of mos_que_mgmt_quename_list() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ m g m t _ m s g i d _ l i s t
 *
 *  OVERVIEW:
 *  Return list of messages by UUID from the specified queue that
 *  satisfy the selection criteria.
 *
 *  INPUTS:
 *  resource_handle:
 *                  A context handle to the resource obtained via
 *                  mos_que_mgmt_create() or mos_rsrc_attach().
 *
 *                  To speed look-ups, a context handle to an NSI group
 *                  or profile is prohibited.
 *
 *  msg_selection_filter:
 *                  Opaque pointer to the selection filter buffer.
 *                  Use the mos_msg_selfilter_alloc(), mos_msg_selfilter_free(),
 *                  and mos_msg_selmask_add() APIs to allocate, deallocate and
 *                  define one or more selection criteria. A message ID is
 *                  successfully retrieved following the rules of the logical
 *                  AND operator among all specified criteria.
 *                  A NULL value indicates no selection criteria.
 *
 *  space_avail:
 *                  Specifies the size of the "msg_id_list" array.
 *                  The caller is responsible for allocating the space
 *                  for "space_avail" queue names.
 *
 *
 *  OUTPUTS:
 *  num_returned:
 *                  The number of message IDs returned in the "msg_id_list".
 *
 *  msg_id_list:
 *                  Array of message IDs returned.
 *                  The size of the array is determined by "space_avail"
 *                  and the length by "num_returned".
 *
 *  num_left:
 *                  Number of messages not necessarily matching any
 *                  selection criteria that were found but could not
 *                  be returned because of space constraints of
 *                  "msg_id_list" array. Note that this number may be
 *                  inaccurate if the queue has enqueued/dequeued messages
 *                  bewteen successive queries.
 *
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_mgmt_msgid_list(
                        mos_handle_t            resource_handle,
                        mos_msg_selfilter_t     msg_selection_filter,
                        unsigned32              space_avail,
                        unsigned32 *            num_returned,
                        uuid_t                  msg_id_list[],
                        unsigned32 *            num_left,
                        error_status_t *        status
                        )
{
    mos_uuid_list_t              ids;
    int                          i;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);
    *num_returned = 0;
    *num_left = 0;

    /*
     * Validate handle
     */
    if (! mos__is_rsrc_handle_valid(TRUE, resource_handle, status)) {
       return;
    }

    /*
     * Validate available space parameter
     */
    if (space_avail == 0) {
        SET_STATUS(status, mos_s_space_avail_req);  
        return;
    }

    /*
     * Determine the authentication level to use
     */
    mos__auth_info_set(resource_handle, resource_handle->protectlvl, status);
    if (BAD_STATUS(status)) {
        return;
    } 

    /*
     * Request to get list of queue names from Qmgr
     */
    mos_q_get_id_list(resource_handle->server_handle,
                      resource_handle->resource_name,
                      msg_selection_filter,
                      &ids,
                      status);
    
    if ((BAD_STATUS(status)) ||
        (EMPTY_LIST(&ids))) {
        return;
    } 

    /*
     * Return only what was allocated
     */
    for (i = 0; i < ids.count; i++){
        if (i < space_avail) {
           msg_id_list[i] = ids.list[i];
           (*num_returned)++;
        } else {
           (*num_left)++; 
        }
    } /* End of loop thru all IDs */

    /* 
     * Release msg ID list
     */
    mos_uuid_list_free_ptrs(&ids);

    return;

} /* End of mos_que_mgmt_msgid_list() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ m g m t _ m o v e
 *
 *  OVERVIEW:
 *  Move or rename one queue within a Q-mgr or from one Q-mgr to
 *  another. The caller must have the proper access rights to
 *  create and/or delete queues as well as enqueuing and dequeuing
 *  from the specified queues.
 *
 *  INPUTS:
 *  dest_qmgr_handle:
 *                  A context handle to the destination queue manager.
 *                  To speed look-ups, a context handle to an NSI group
 *                  or profile is prohibited.
 *                  A NULL value indicates that the destination queue
 *                  manager is the same as the source queue manager.
 *
 *  new_que_name:
 *                  Specifies the new queue name.
 *                  The name specified must be a simple name.
 *
 *  flags:
 *                  Indicates the action to be performed on a queue
 *                  move or rename. The default is "mos_c_que_mv_none".
 *
 *  INPUTS/OUTPUTS:
 *  move_que_handle:
 *                  A context handle to an attached source queue resource.
 *                  If the move operation succeeds, this parameter
 *                  returns a handle to the destination queue resource.
 *
 *  OUTPUTS:
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_mgmt_move(
                  mos_handle_t          dest_qmgr_handle,
                  mos_rsrc_name_t       new_que_name,
                  unsigned32            flags,
                  mos_handle_t *        move_que_handle,  
                  error_status_t *      status
                 )

{
    unsigned_char_t *            expanded_name = (unsigned_char_t *)NULL;
    error_status_t               temp_status;
    sec_rgy_name_t               hold_security_name;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate source handle as a queue
     */
    if (move_que_handle == NULL) {
       SET_STATUS(status, mos_s_rsrc_handle_inv);
       return;
    }
    if (! mos__is_rsrc_handle_valid(TRUE, *move_que_handle, status)) {
       return;
    }

    /*
     * Validate queue move/rename options
     */
    if (! mos__is_mvop_valid(flags, status)) {
       return;
    }

    /*
     * Validate destination queue name
     */
    if ((new_que_name == NULL) || (new_que_name[0] == '\0')) {
        SET_STATUS(status, mos_s_quename_null);  
        return;
    }

    /*
     * Determine if destination queue name is a simple name.
     */
    rpc_ns_entry_expand_name(mos_c_dflt_name_syntax,
                             new_que_name,
                             &expanded_name,
                             status);
    rpc_string_free(&expanded_name, &temp_status);
    if (GOOD_STATUS(status)) {     
        SET_STATUS(status, mos_s_quename_not_simple);  
        return;
    } 
    CLEAR_STATUS(status);

    /*
     * If source and destination queue managers are the same, then
     * merely rename or perform an *intra* queue move.
     * Otherwise, perform a Peek/Enqueue/Purge queue move.
     */
     if ((dest_qmgr_handle == NULL) ||
         (dest_qmgr_handle->server_handle ==   
                           (*move_que_handle)->server_handle)) {
         if (strcmp((char *)new_que_name, 
                    (char *)(*move_que_handle)->resource_name) == 0) {
            SET_STATUS(status, mos_s_quename_already_exists);  
            return;
         }
         strcpy((char *)hold_security_name,
                (char *)(*move_que_handle)->security_name);
         mos__que_move_on_same_qmgr(new_que_name, 
                                    flags,
                                    move_que_handle, 
                                    status);
     } else {
         strcpy((char *)hold_security_name,
                (char *)dest_qmgr_handle->security_name);
         mos__que_move_to_diff_qmgr(dest_qmgr_handle, 
                                    new_que_name, 
                                    flags,
                                    move_que_handle, 
                                    status);
     }

    if (BAD_STATUS(status)) {
        return;
    }

    /*
     * If the source queue was deleted, then the destination queue
     * needs to be obtained. 
     */
    if (*move_que_handle == NULL) {
        mos_rsrc_attach(new_que_name,
                        hold_security_name,
                        move_que_handle,
                        status);
    }
    return;

} /* End of mos_que_mgmt_move() routine */


/*
 *  FUNCTION:
 *  m o s _ _ q u e _ m o v e _ o n _ s a m e _ q m g r
 *
 *  OVERVIEW:
 *  Move or rename one queue within the same Q-mgr.
 *  If the destination queue does not exist, merely rename queue.
 *  If the destination queue does exist, append messages from source 
 *  to destination queue.
 *
 *  INPUTS:
 *  new_que_name:
 *                  Specifies new queue name.
 *                  The name specified must be a simple name.
 *
 *  flags:
 *                  Indicates the action to be performed on a queue
 *                  move or rename. The default is "mos_c_que_mv_none".
 *
 *  INPUTS/OUTPUTS:
 *  move_que_handle:
 *                  A context handle to an attached source queue resource.
 *                  If the rename operation succeeds, this parameter
 *                  returns a handle to the queue resource.
 *                  If the move operation succeeds, this parameter returns
 *                  a NULL handle.
 *
 *  OUTPUTS:
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos__que_move_on_same_qmgr(
                          mos_rsrc_name_t       new_que_name,
                          unsigned32            flags,
                          mos_handle_t *        move_que_handle,  
                          error_status_t *      status
                         )
{
    /*
     * Determine the authentication level to use and rename queue
     */
    mos__auth_info_set(*move_que_handle,
                       (*move_que_handle)->protectlvl,
                       status);
    if (BAD_STATUS(status)) {
        return;
    }

    /*
     * If the destination queue does not exist, merely
     * rename queue.
     */
    mos_mgmt_rename((*move_que_handle)->server_handle,
                    (*move_que_handle)->resource_name,
                    new_que_name,
                    status);
    if (GOOD_STATUS(status)) {
       strcpy((char *)(*move_que_handle)->resource_name,
              (char *)new_que_name);
       return;
    }
    if (STATUS_EQUAL(status, mos_queue_already_exists)) {
       /*
        * If the destination queue does exist, append
        * messages from source to destination queue.
        */
       mos_mgmt_move((*move_que_handle)->server_handle,
                     (*move_que_handle)->resource_name,
                     new_que_name,
                     status);
        if (GOOD_STATUS(status)) {
           /*
            * Delete source queue, which should be empty. 
            */
           mos_que_mgmt_delete(mos_c_delop_preserve,
                               move_que_handle,
                               status);
        }
    } 

    return;

} /* End of mos__que_move_on_same_qmgr() routine */


/* 
 *  FUNCTION:
 *  m o s _ _ q u e _ m o v e _ o n _ d i f f _ q m g r
 *
 *  OVERVIEW:
 *  Move one queue from one Q-mgr to another. 
 *  The caller must have the proper access rights to create and/or delete
 *  queues as well as enqueuing and dequeuing from the specified queues.
 *
 *  INPUTS:
 *  dest_qmgr_handle:
 *                  A context handle to the destination queue manager.
 *                  To speed look-ups, a context handle to an NSI group
 *                  or profile is prohibited.
 *  
 *  new_que_name:
 *                  Specifies new queue name.
 *                  The name specified must be a simple name.
 *
 *  flags:
 *                  Indicates the action to be performed on a queue
 *                  move or rename. The default is "mos_c_que_mv_none".
 *
 *  INPUTS/OUTPUTS:
 *  move_que_handle:
 *                  A context handle to an attached source queue resource.
 *                  If the move operation succeeds, this parameter returns
 *                  a NULL handle.
 *
 *  OUTPUTS:
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos__que_move_to_diff_qmgr(
                          mos_handle_t          dest_qmgr_handle,
                          mos_rsrc_name_t       new_que_name,
                          unsigned32            flags,
                          mos_handle_t *        move_que_handle,  
                          error_status_t *      status
                         )
{
    mos_mattr_t                  msg_attr;
    mos_msg_attr_t               msg_attr_p;
    unsigned32                   save_protectlvl = mos_c_protect_level_default;
    mos_datatype_bytearray_t     msg_body;
    uuid_t                       msg_id;
    mos_que_attr_t               new_que_attr;
    mos_handle_t                 dest_que_handle;
    error_status_t               temp_status;

    /*
     * Create destination queue with the same queue attributes of 
     * the source queue. Otherwise, just attach to queue.
     */
    mos_que_attr_peek(*move_que_handle,
                      &new_que_attr,
                      status);
    if (BAD_STATUS(status)) {
       return;
    }
    /*
     * Note that the source queue aliases are not copied to the 
     * destination queue. This is due to the fact that aliases may 
     * have been created with named links to queue managers.
     *
     * Disable dequeuing on destination queue, if allowed. 
     */
    mos_que_attr_alias_reset(new_que_attr,  
                             &temp_status);
    new_que_attr->deq_enabled = FALSE;     
    mos_que_mgmt_create(dest_qmgr_handle,
                        new_que_name,
                        new_que_attr, 
                        &dest_que_handle,
                        status);
    mos_que_attr_free(&new_que_attr,
                      &temp_status);
    if (STATUS_EQUAL(status, mos_queue_already_exists)) {
        mos_rsrc_attach(new_que_name,
                        dest_qmgr_handle->security_name,
                        &dest_que_handle,
                        status);
        /* Disable dequeuing on destination queue, if allowed. */
        mos__que_attr_enqdeq_commit(dest_que_handle, FALSE, FALSE);
    } 
    if (BAD_STATUS(status)) {
       return;
    }

    /* Disable enqueuing on source queue, if allowed. */
    mos__que_attr_enqdeq_commit(*move_que_handle, TRUE, FALSE);
         
    /*
     * Peek/Enqueue/Purge all messages from the appropriate source
     * queue to the destination queue.
     */
    while (GOOD_STATUS(status)) {

       mos__msg_pep(*move_que_handle,
                    dest_que_handle,
                    (void *)NULL,
                    status);

    } /* End of loop thru all messages in source queue */
    if (STATUS_EQUAL(status, mos_empty_queue)) {
       CLEAR_STATUS(status);
    }

    /*
     * Delete source queue, which should be empty.
     */
    if (GOOD_STATUS(status)) {
        mos_que_mgmt_delete(mos_c_delop_preserve,
                            move_que_handle,
                            status);
    }

    /*
     * Enable dequeuing on destination queue, if allowed.
     */
    mos__que_attr_enqdeq_commit(dest_que_handle, FALSE, TRUE);

    return;

} /* End of mos__que_move_to_diff_mgr() routine */


/*
 *  FUNCTION:
 *  m o s _ _ q u e _ a t t r _ e n q d e q _ c o m m i t
 *
 *  OVERVIEW:
 *  This routine obtains the existing queue attributes, sets the 
 *  "allow enqueue" or "allow dequeue" queue attribute accordingly, 
 *  and commits the change. Note that errors are ignored.
 *
 *  INPUTS:
 *  que_handle:
 *                 A context handle to a queue resource.
 * 
 *  enqdeq:
 *                 Indicates queue attribute to modify.
 *                 TRUE indicates enqueuing.
 *                 FALSE indicates dequeuing.
 *  enable:
 *                 Indicates whether enqueuing/dequeuing is allowed or not.
 *                 TRUE indicates allow enqueuing/dequeuing.
 *                 FALSE indicates inhibit enqueuing/dequeuing.
 *
 *  OUTPUTS:
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos__que_attr_enqdeq_commit(
                            mos_handle_t                que_handle,
                            boolean                     enqdeq,
                            boolean                     enable
                           )
{
    mos_que_attr_t               que_attr;
    error_status_t               temp_status;

    /*
     * Obtain existing queue attributes
     */
    mos_que_attr_peek(que_handle,
                      &que_attr,
                      &temp_status);
    if (BAD_STATUS(&temp_status)) {
       return;
    }

    /*
     * Enable/Disable enqueuing/dequeuing on queue, if allowed.
     */
    if (enqdeq) {
        if (enable) {
           que_attr->enq_enabled = TRUE;
        } else {
           que_attr->enq_enabled = FALSE;
        }
    } else {
        if (enable) {
           que_attr->deq_enabled = TRUE;
        } else {
           que_attr->deq_enabled = FALSE;
        }
    }
    
    /*
     * If commit fails, still continue
     */
    mos_que_attr_commit(que_handle,
                        que_attr,
                        &temp_status);

    /*
     * Clean-up queue attributes
     */
    mos_que_attr_free(&que_attr,
                      &temp_status);

} /* End of mos__que_attr_enqdeq_commit() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ m v o p _ v a l i d
 *
 *  OVERVIEW:
 *  Validate queue move/rename options.
 *
 *  INPUTS:
 *  mv_option:
 *                  Indicates the action to be performed on a queue
 *                  move or rename. The default is "mos_c_que_mv_none".
 *
 *  OUTPUTS:
 *  status:         
 *                  A pointer to the completion status. On successful 
 *                  completion, the routine returns error_status_ok. 
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        TRUE  indicates validation was successful.
 *                  FALSE indicates validation was not successful.
 */
boolean
mos__is_mvop_valid(
                    unsigned32                  mv_option,
                    error_status_t *            status
                   )
{
    CLEAR_STATUS(status);

    switch (mv_option) {
       case (mos_c_que_mv_none):
       case (mos_c_que_mv_referral):
           return(TRUE);
       default:
           SET_STATUS(status, mos_s_que_mvop_inv);  
           return(FALSE);
    } /* End of switch on queue move/rename options */

} /* End of mos__is_mvop_valid() routine */
