/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 *  MODULE:
 *  m o s _ m s g _ s e l f i l t e r . c
 *
 *  DESCRIPTION:
 *  Client interfaces for Message Selection Filter Utilities to support 
 *  the Message Oriented Service Application Programming Interface(MOS-API).
 *
 * ______________________________________________________________________
 *
 * $Log: mos_msg_selfilter.c,v $
 * Revision 1.4  1996/11/20  14:47:39  salamone
 * Fix header file declarations
 *
 * Revision 1.3  96/10/25  16:51:00  16:51:00  salamone (Julie Salamone)
 * Clean-up I/O params on _set() calls
 * 
 * Revision 1.2  96/10/23  16:12:45  16:12:45  salamone (Julie Salamone)
 * Add selection filter support
 * 
 * Revision 1.1  96/10/14  17:07:56  17:07:56  salamone (Julie Salamone)
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
#include <dce/dce_cf.h>
#include <dce/dce_svc.h>
#include <dce/nbase.h>
#include <dce/assert.h>
#include <dce/aclif.h>
#include <dce/utc.h>
#include <dce/id_epac.h>
#include <dce/pgo.h>
#include <dce/sec_login.h>

#include <mosif.h>
#include <mos_api.h>
#include <mos_base.h>
#include <mos_q.h>
#include <dcemosmsg.h>
#include <dcemosmac.h>
#include <dcemossvc.h>

static mos_mask_list_t    null_mask_list; /* NULL selection filter structure */

/*
 * INTERNAL PROTOTYPES
 */
boolean mos__is_sender_valid(void *, uuid_t *, error_status_t *);
boolean mos__is_maskvalue_valid(mos_msg_selmask_type_t, mos_msg_selmask_value_t, mos_msg_selmask_value_t *, error_status_t *);
boolean mos__is_maskop_valid(mos_msg_selmask_type_t, mos_msg_selmask_op_t, error_status_t *);


/*
 *  FUNCTION:
 *  m o s _ m s g _ s e l f i l t e r _ a l l o c
 *
 *  OVERVIEW:
 *  Allocates memory within MOS and returns an opaque pointer to a 
 *  selection filter structure.
 *  The selection filter is comprised of one or more masks that follow 
 *  the rules of the logical AND operator among all specified criteria 
 *  when retrieving messages.
 *
 *  OUTPUTS:
 *  selection_filter:
 *                  Opaque pointer to selection filter large enough
 *                  to hold the specified number of mask specifications.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_selfilter_alloc(
                        mos_msg_selfilter_t *   selection_filter,
                        error_status_t *        status
                       )
{
    mos_mask_list_t *       mask_list_p;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);
    *selection_filter = NULL;

    /*
     * Allocate space
     */
    mask_list_p = (mos_mask_list_t *)malloc(sizeof(mos_mask_list_t));
    if (mask_list_p == NULL) {
       SET_STATUS(status, mos_s_no_memory);
       return;
    }

    /*
     * Set initially to NIL structure
     */
    *mask_list_p = null_mask_list;

    /*
     * Return selection filter structure
     */
    *selection_filter = (mos_msg_selfilter_t)mask_list_p;
    return;

} /* End of mos_msg_selfilter_alloc() routine */



/*
 *  FUNCTION:
 *  m o s _ m s g _ s e l f i l t e r _ f r e e
 *
 *  OVERVIEW:
 *  Releases memory used for the selection filter when retrieving messages.
 *  It is the responsibility of the caller to release the memory if 
 *  allocated by mos_msg_selfilter_alloc().
 *
 *  INPUTS/OUTPUTS:
 *  selection_filter:
 *                  Opaque pointer to selection filter large enough
 *                  to hold the specified number of mask specifications.
 *                  If the operation succeeds, this parameter
 *                  returns the value NULL.
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
mos_msg_selfilter_free(
                       mos_msg_selfilter_t *   selection_filter,
                       error_status_t *        status
                      )
{
    int i;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate handle to selection filter
     */
    if ((selection_filter == NULL) || (*selection_filter == NULL)) {
        SET_STATUS(status, mos_s_msg_selfilter_inv);  
        return;
    }

    /*
     * Release memory for each mask and the selection filter itself.
     * Return NULL pointer
     */
    for (i = 0; i < (*selection_filter)->count; i++) {
        free((*selection_filter)->list[i]);
    }
    free(*selection_filter);
    *selection_filter = NULL;
    return;

} /* End of mos_msg_selfilter_free() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ s e l m a s k _ a d d
 *
 *  OVERVIEW:
 *  Adds one mask to the list of masks in the selection filter structure. 
 *  A mask is comprised of a selection criteria type, a selection
 *  criteria value, and the relational operator to be performed.
 *
 *  For DCE, only the "priority" selection criteria is allowed
 *  a relational operator other than "equal" or "not equal". This
 *  is due to the fact that some selection criteria are UUIDs,
 *  messaging system guarantees, or that they simply do not apply.
 *
 *  INPUTS:
 *  mask_type:
 *                   Type of selection criteria for specified mask.
 *  mask_value:
 *                   Opaque pointer to the value of selection criteria
 *                   for specified mask.
 *
 *                   mos_c_mask_type_msgid and mos_c_mask_type_datatype
 *                   are UUIDs.
 *
 *                   mos_c_mask_type_msgtype, mos_c_mask_type_priority and
 *                   mos_c_mask_type_protectlvl are unsigned32.
 *
 *                    mos_c_mask_type_sender_prin is a pointer to an
 *                   unsigned_char_t.  Note that the API finds the
 *                   UUID for the sender's principal name.
 *
 *                   mos_c_mask_type_ttr takes no mask value or operation.
 *                   The caller must have the proper access rights to
 *                   request these messages.
 *
 *  mask_op:
 *                   Relational operator to be performed on the "mask_type"
 *                   using the "mask_value".
 *
 *  INPUT/OUTPUT:
 *  selection_filter:
 *                   Opaque pointer to selection filter structure allocated
 *                   via mos_msg_selfilter_alloc().
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
mos_msg_selmask_add(
                    mos_msg_selmask_type_t   mask_type,
                    mos_msg_selmask_value_t  mask_value,
                    mos_msg_selmask_op_t     mask_op,
                    mos_msg_selfilter_t      selection_filter,
                    error_status_t *         status
                   )
{
    mos_msg_selmask_value_t    mask_result;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate handle to selection filter
     */
    if (selection_filter == NULL) {
        SET_STATUS(status, mos_s_msg_selfilter_inv); 
        return;
    }

    /*
     * Validate mask type and mask value based on mask type
     * Use "mask_result" as the new mask value.
     */
    if (! mos__is_maskvalue_valid(mask_type, mask_value, &mask_result, status)){
        return;
    }

    /*
     * Validate mask operations based on mask type
     */
    if (! mos__is_maskop_valid(mask_type, mask_op, status)) {
        return;
    }

    /*
     * Allocate space for one more mask
     */
    selection_filter->list =
                 (mos_mask_t *)realloc(selection_filter->list,
                                       sizeof(mos_mask_t) * 
                                       (selection_filter->count + 1));
    if (selection_filter->list == NULL) {
       SET_STATUS(status, mos_s_no_memory);
       return;
    }

    /*
     * Add new mask 
     */
    selection_filter->list[selection_filter->count].mask_info.mask_type =
                             mask_type;
    selection_filter->list[selection_filter->count].relation =
                             mask_op;
    switch (mask_type) {
       case (mos_c_mask_type_msgid):
           selection_filter->list[selection_filter->count].mask_info.tagged_union.id = *(uuid_t *)mask_result;
           break;
       case (mos_c_mask_type_msgtype):
           selection_filter->list[selection_filter->count].mask_info.tagged_union.msgtype = *(mos_msg_attr_msgtype_t *)mask_result; 
           break;
       case (mos_c_mask_type_priority):
           selection_filter->list[selection_filter->count].mask_info.tagged_union.priority = *(unsigned32 *)mask_result; 
           break;
       case (mos_c_mask_type_datatype):
           selection_filter->list[selection_filter->count].mask_info.tagged_union.datatype = *(uuid_t *)mask_result; 
           break;
       case (mos_c_mask_type_protectlvl):
           selection_filter->list[selection_filter->count].mask_info.tagged_union.protectlvl = *(unsigned32 *)mask_result; 
           break;
       case (mos_c_mask_type_sender_prin):
           selection_filter->list[selection_filter->count].mask_info.tagged_union.sender = *(uuid_t *)mask_result; 
           free(mask_result);    /* free UUID */
           break;
       case (mos_c_mask_type_ttr):
       default:
           break;
    } /* End of set mask value */

    /*
     * Increment counter in the selection filter strucuture
     */
    selection_filter->count++;

    return;

} /* End of mos_msg_selmask_add() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ m a s k v a l u e _ v a l i d
 *
 *  OVERVIEW:
 *  Validate mask type and mask value based on mask type. 
 *
 *  INPUTS:
 *  mask_type:
 *                   Type of selection criteria for specified mask.
 *
 *  mask_value:
 *                   Opaque pointer to the value of selection criteria
 *                   for specified mask.
 *
 *  OUTPUTS:
 *  mask_result:
 *                   Opaque pointer to the NEW value of selection criteria
 *                   for specified mask. Note that it is the caller's
 *                   responsibility to free any memory allocated here.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        TRUE  indicates validation was successful.
 *                  FALSE indicates validation was not successful.
 */
boolean
mos__is_maskvalue_valid(
                        mos_msg_selmask_type_t      mask_type,
                        mos_msg_selmask_value_t     mask_value,
                        mos_msg_selmask_value_t *   mask_result,
                        error_status_t *            status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);
    *mask_result = mask_value;

    switch (mask_type) {
       case (mos_c_mask_type_msgid):
           mos__is_uuid_valid((uuid_t *)mask_value, status);
           break;
       case (mos_c_mask_type_msgtype):
           mos__is_msgtype_valid(*(mos_msg_attr_msgtype_t *)mask_value, 
                                 status);
           break;
       case (mos_c_mask_type_priority):
           /* Assume integer */
           break;
       case (mos_c_mask_type_datatype):
           mos__is_datatype_valid(FALSE, (uuid_t *)mask_value, status);
           break;
       case (mos_c_mask_type_protectlvl):
           mos__is_protectlvl_valid(*(mos_msg_attr_msgtype_t *)mask_value, 
                                    status);
           break;
       case (mos_c_mask_type_sender_prin):
           *mask_result = (uuid_t *)malloc(sizeof(uuid_t));
           if (*mask_result == NULL) {
               SET_STATUS(status, mos_s_no_memory);
           } else {
               mos__is_sender_valid(mask_value, *mask_result, status);
           }
           break;
       case (mos_c_mask_type_ttr):
           /*
            * No mask value on ttr selection criteria.
            */
           if (mask_value != NULL) {
               SET_STATUS(status, mos_s_mask_ttr_value); 
           }
           break;
       default:
           SET_STATUS(status, mos_s_mask_type_inv); 
           break;
    } /* End of determine valid mask type and value */

    if (GOOD_STATUS(status)) {
        return(TRUE);
    } else {
        *mask_result = NULL;
        return(FALSE);
    }

} /* End of mos__is_maskvalue_valid() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ m a s k o p _ v a l i d
 *
 *  OVERVIEW:
 *  Validate mask operation based on mask type.
 *
 *  INPUTS:
 *  mask_type:
 *                  Type of selection criteria for specified mask.
 *
 *  mask_op:
 *                  Defines a relational operator to be performed on
 *                  the "mask_type" using the "mask_value".
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
mos__is_maskop_valid(
                     mos_msg_selmask_type_t      mask_type,
                     mos_msg_selmask_op_t        mask_op,
                     error_status_t *            status
                    )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    switch (mask_op) {
       case (mos_c_mask_op_none):
           /*
            * Only ttr selection criteria is allowed 
            */
           if (mask_type != mos_c_mask_type_ttr) {
              SET_STATUS(status, mos_s_mask_op_none);  
           }
           break;
       case (mos_c_mask_op_equal):
           /*
            * All selection criteria, except ttr, is allowed.
            */
           if (mask_type == mos_c_mask_type_ttr) {
              SET_STATUS(status, mos_s_mask_op_equal);  
           }
           break;
       case (mos_c_mask_op_not_equal):
           /*
            * All selection criteria, except protectlvl and ttr, is allowed.
            */
           if ((mask_type == mos_c_mask_type_protectlvl) ||
               (mask_type == mos_c_mask_type_ttr)) {
              SET_STATUS(status, mos_s_mask_op_not_equal);  
           }
           break;
       case (mos_c_mask_op_less_than):
       case (mos_c_mask_op_eq_less_than):
       case (mos_c_mask_op_greater_than):
           /*
            * Only priority selection criteria is allowed 
            */
           if (mask_type != mos_c_mask_type_priority) {
              SET_STATUS(status, mos_s_mask_op_priority);  
           }
           break;
       case (mos_c_mask_op_eq_greater_than):
           /*
            * Only priority and protectlvl selection criteria is allowed 
            */
           if (! ((mask_type == mos_c_mask_type_priority) ||
                  (mask_type == mos_c_mask_type_protectlvl))) {
              SET_STATUS(status, mos_s_mask_op_eq_greater_than);  
           }
           break;
       default:
           SET_STATUS(status, mos_s_mask_op_inv);  
           break;
    } /* End of switch on mask operation */

    if (GOOD_STATUS(status)) {
        return(TRUE);
    } else {
        return(FALSE);
    }

} /* End of mos__is_maskop_valid() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ s e n d e r _ v a l i d
 *
 *  OVERVIEW:
 *  Validate name of principal(sender).
 *
 *  INPUTS:
 *  mask_value:
 *                  Specifies principal name.
 *
 *  OUTPUTS:
 *  sender_id:
 *                  UUID of principal(sender).
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        TRUE  indicates validation was successful.
 *                  FALSE indicates validation was not successful.
 */
boolean
mos__is_sender_valid(
                     void *                      mask_value,
                     uuid_t *                    sender_id,
                     error_status_t *            status
                    )
{
    sec_rgy_handle_t     rgy_handle;
    char *               cell_name;        /* Local cell name */

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Good value?
     */
    if (mask_value == NULL) {
        SET_STATUS(status, mos_s_mask_sender_null);  
        return(FALSE);
    }

    /*
     * Obtain local cell name
     */
    dce_cf_get_cell_name(&cell_name, status);
    if (*status != dce_cf_st_ok){
       return(FALSE);
    }

    /*
     * Bind to local cell registry
     */
    sec_rgy_site_open(cell_name, &rgy_handle, status);
    if (BAD_STATUS(status)){
       free(cell_name);
       return(FALSE);
    }
    free(cell_name);    /* Got handle, free cell name */

    sec_rgy_pgo_name_to_id(rgy_handle,
                           sec_rgy_domain_person,
                           (unsigned_char_t *)mask_value, /* name of princ */
                           sender_id,   /* UUID of principal (sender) */
                           status);

    if (GOOD_STATUS(status)) {
       return(TRUE);
    } else {
       return(FALSE);
    }

} /* End of mos__is_sender_valid() routine */
