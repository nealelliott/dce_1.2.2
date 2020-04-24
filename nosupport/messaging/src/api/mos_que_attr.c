/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 *  MODULE:
 *  m o s _ q u e _ a t t r . c
 *
 *  DESCRIPTION:
 *  Client interfaces for Queue Attribute Utilities to support the 
 *  Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 *
 * $Log: mos_que_attr.c,v $
 * Revision 1.8  1996/11/20  18:06:01  salamone
 * Update mos_que_attr_alias_reset() desc
 *
 * Revision 1.7  96/11/20  14:47:53  14:47:53  salamone (Julie Salamone)
 * Fix header file declarations
 * 
 * Revision 1.6  96/10/25  16:51:14  16:51:14  salamone (Julie Salamone)
 * Clean-up I/O params on _set() calls
 * 
 * Revision 1.5  96/10/25  14:43:15  14:43:15  salamone (Julie Salamone)
 * Add rsrc protectlvl support
 * 
 * Revision 1.4  96/10/18  11:41:43  11:41:43  salamone (Julie Salamone)
 * Validate array input
 * 
 * Revision 1.3  96/10/16  14:44:21  14:44:21  salamone (Julie Salamone)
 * Added msg and que attr codes
 * 
 * Revision 1.2  96/10/15  17:41:38  17:41:38  salamone (Julie Salamone)
 * Added function descriptions
 * 
 * Revision 1.1  96/10/14  16:51:58  16:51:58  salamone (Julie Salamone)
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

static mos_qattr_t       null_que_attr;  /* NIL queue attributes structure */
static utc_t             null_utc;       /* NIL UTC */

/*
 * INTERNAL PROTOTYPES
 */
void mos__dflt_idletimeout_set(utc_t *);
boolean mos__is_quepersistence_valid(mos_que_attr_persistence_t, error_status_t *);
boolean mos__is_boolean_valid(boolean, error_status_t *);


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ a l l o c
 *
 *  OVERVIEW:
 *  Allocates memory within MOS and returns an opaque pointer to 
 *  a queue attributes structure with defaults set.
 *
 *  It is the responsibilty of the caller to release memory
 *  via mos_que_attr_free().
 *
 *  OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to default queue attributes structure.
 *                  Use the other mos_que_attr_X_set() APIs to modify any
 *                  attributes.
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_alloc(
                   mos_que_attr_t *         que_attributes,
                   error_status_t *         status
                  )
{
   mos_qattr_t *                que_attr_p;
   unsigned32                   seconds;

   /*
    * Initialize output parameters
    */
   CLEAR_STATUS(status);
   *que_attributes = NULL;

   /*
    * Allocate space
    */
   que_attr_p = (mos_qattr_t *)malloc(sizeof(mos_qattr_t));
   if (que_attr_p == NULL) {
      SET_STATUS(status, mos_s_no_memory);  
      return;
   }

   /*
    * Set initially to NIL structure
    */
   *que_attr_p = null_que_attr;

   /*
    * Set queue attribute defaults
    */
   que_attr_p->max_qlen         = mos_c_que_attr_dflt_quemaxsize;
   que_attr_p->max_msgsize      = mos_c_que_attr_dflt_msgmaxsize;
   que_attr_p->persistence      = mos_c_que_attr_msgpersistence;
   que_attr_p->enq_enabled      = TRUE;
   que_attr_p->deq_enabled      = TRUE;

   /*
    * Set the default empty queue idle timeout
    */
   que_attr_p->flags |= mos_c_use_empty_timeout;
   mos__dflt_idletimeout_set(&que_attr_p->empty_timeout);

   /*
    * Return queue attribute structure
    */
   *que_attributes = (mos_que_attr_t)que_attr_p;
   return;

} /* End of mos_que_attr_alloc() routine */


/*
 *  FUNCTION:
 *  m o s _ _ d f l t _ i d l e t i m e o u t _ s e t
 *
 *  OVERVIEW:
 *  Determine the default empty queue idle timeout in relative time.
 *
 *  OUTPUTS:
 *  idletimeout:
 *                Indicates the maximum amount of time where no activity
 *                occurs on an empty queue. Once this timeout is reached,
 *                its queue manager is free to implicitly delete the empty
 *                queue at any time thereafter. If this value is not equal
 *                to NULL, the queue manager starts the idle timer when a
 *                queue is empty.
 *                Specified in relative time only.
 *
 *                The default queue idle timeout is 24 hours. 
 *
 *  RETURNS:       None
 */
void
mos__dflt_idletimeout_set(
                          utc_t *        idletimeout
                         )
{
    timespec_t          tzero;
    reltimespec_t       t_ttl;

    /*
     * Construct a timestamp that represents 
     * "mos_c_que_attr_dflt_idletime" hours
     */
    t_ttl.tv_sec  = (3600 * mos_c_que_attr_dflt_idletime);
    t_ttl.tv_nsec = 0;
    tzero.tv_sec  = 0;
    tzero.tv_nsec = 0;
    utc_mkbinreltime(idletimeout,
                     &t_ttl,
                     &tzero);

} /* End of mos__dflt_idletimeout_set() routine */

 
/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ p e e k
 *
 *  OVERVIEW:
 *  Returns an opaque pointer to the queue attributes on an
 *  existing specified queue.
 *
 *  Use the mos_que_attr_X_get() APIs to view attributes.
 *  Use the mos_que_attr_X_set() APIs to modify attributes
 *  and mos_que_attr_commit() to commit modifications.
 *
 *  It is the responsibilty of the caller to release memory
 *  via mos_que_attr_free().
 *
 *  INPUTS:
 *  resource_handle:
 *                  A context handle to the resource obtained via
 *                  mos_rsrc_attach() or mos_que_mgmt_create().
 *
 *  OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to queue attributes on existing
 *                  specified queue.
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_peek(
                  mos_handle_t          resource_handle,
                  mos_que_attr_t *      que_attributes,
                  error_status_t *      status
                 )
{
   mos_qattr_t *                que_attr_p;

   /*
    * Initialize output parameters
    */
   *que_attributes = NULL;
   CLEAR_STATUS(status);

   /*
    * Validate handle to queue
    */
   if (! mos__is_rsrc_handle_valid(TRUE, resource_handle, status)) {
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
    * Allocate space for queue attributes
    */
   que_attr_p = (mos_qattr_t *)malloc(sizeof(mos_qattr_t));
   if (que_attr_p == NULL) {
      SET_STATUS(status, mos_s_no_memory);  
      return;
   }

   /*
    * Request to peek at queue attributes
    */
   mos_q_get_qattr_by_name(resource_handle->server_handle,
                           resource_handle->resource_name,
                           que_attr_p,
                           status);

   if (GOOD_STATUS(status)) {
       *que_attributes = (mos_que_attr_t)que_attr_p;
   } else {
       free(que_attr_p);
   }

   return;

} /* End of mos_que_attr_peek() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ f r e e
 *
 *  OVERVIEW:
 *  Releases memory used for queue attributes.
 *  It is the responsibility of the caller to release the memory 
 *  if allocated by mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  INPUTS\OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to queue attributes structure to
 *                  be freed.
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
mos_que_attr_free(
                  mos_que_attr_t *         que_attributes,
                  error_status_t *         status
                 )
{
   int                          i;

   /*
    * Initialize output parameters
    */
   CLEAR_STATUS(status);

   /*
    * Validate handle to queue attributes
    */
   if ((que_attributes == NULL) || (*que_attributes == NULL)) {
       SET_STATUS(status, mos_s_que_attr_inv);  
       return;
   } 

   /*
    * Release queue name 
    */
    if ((*que_attributes)->name) {
        free((*que_attributes)->name);
        (*que_attributes)->name = NULL;
    }

   /*
    * Release queue annotation
    */
   if ((*que_attributes)->annotation) {
       free((*que_attributes)->annotation);
       (*que_attributes)->annotation = NULL;
   }

   /*
    * Discard all queue aliases
    */
   mos_string_list_free_ptrs(&(*que_attributes)->aliases);

   /*
    * Release memory and return NULL pointer
    */
   free(*que_attributes);
   *que_attributes = NULL;
   return;

} /* End of mos_que_attr_free() routine */

 
/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ c o m m i t
 *
 *  OVERVIEW:
 *  Commit queue attribute modifications on an existing queue.
 *
 *  INPUTS:
 *  resource_handle:
 *                  A context handle to the resource obtained via
 *                  mos_que_mgmt_create() or mos_rsrc_attach().
 *
 *                  To speed look-ups, a context handle to an NSI group
 *                  or profile is prohibited.
 *
 *  que_attributes:
 *                  Opaque pointer to queue attributes to be modified
 *                  and commited to the specified queue manager.
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
mos_que_attr_commit(
                    mos_handle_t          resource_handle,
                    mos_que_attr_t        que_attributes,
                    error_status_t *      status
                   )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate handle to queue
     */
    if (! mos__is_rsrc_handle_valid(TRUE, resource_handle, status)) {
        return;
    }

    /*
     * Validate handle to queue attributes
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    /*
     * Determine the authentication level to use
     */
    mos__auth_info_set(resource_handle, resource_handle->protectlvl, status);

    /*
     * Request to commit queue attributes
     */
    if (GOOD_STATUS(status)) {
        mos_q_set_qattr_by_name(resource_handle->server_handle,
                                resource_handle->resource_name,
                                (mos_qattr_t *)que_attributes,
                                status);
    }

    return;

} /* End of mos_que_attr_commit() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ q u e i d _ g e t
 *
 *  OVERVIEW:
 *  Obtain the queue ID from the queue attributes structure. 
 *  Queue ID is a read-only attribute.
 *
 *  INPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  OUTPUTS:
 *  que_id:
 *                  Unique queue ID generated by a queue manager.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_queid_get(
                       mos_que_attr_t                 que_attributes,
                       uuid_t *                       que_id,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    *que_id = que_attributes->id;

    return;

} /* End of mos_que_attr_queid_get() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ q u e c u r s i z e _ g e t
 *
 *  OVERVIEW:
 *  Obtain the current queue size from the queue attributes structure. 
 *  Queue current size is a read-only attribute.
 *
 *  INPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  OUTPUTS:
 *  que_cur_size:
 *                 Indicates the current number of messages on the queue.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_quecursize_get(
                       mos_que_attr_t                 que_attributes,
                       unsigned32 *                   que_cur_size,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv); 
        return;
    } 

    *que_cur_size = que_attributes->len;

    return;

} /* End of mos_que_attr_quecursize_get() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ c r e a t e t i m e _ g e t
 *
 *  OVERVIEW:
 *  Obtain the queue creation time stamp in absolute time from the 
 *  queue attributes structure. 
 *  Queue creation time stamp is a read-only attribute.
 *
 *  INPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  OUTPUTS:
 *  que_create_time:
 *                 Indicates the time and date the queue was created.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_createtime_get(
                       mos_que_attr_t                 que_attributes,
                       utc_t *                        que_create_time,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    *que_create_time = que_attributes->created;

    return;

} /* End of mos_que_attr_createtime_get() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ a c t i v e  t i m e _ g e t
 *
 *  OVERVIEW:
 *  Obtain the queue last activity time stamp in absolute time from the 
 *  queue attributes structure. 
 *  Queue last activity time stamp is a read-only attribute.
 *
 *  INPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  OUTPUTS:
 *  que_active_time:
 *                Indicates the time and date in absolute time that the
 *                last activity was performed on any messages within a
 *                queue. Activities include peeking, enqueuing, and
 *                dequeuing messages on a queue. It does not include
 *                peeking and changing queue attributes.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_activetime_get(
                       mos_que_attr_t                 que_attributes,
                       utc_t *                        que_active_time,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv); 
        return;
    } 

    *que_active_time = que_attributes->last_activity;

    return;

} /* End of mos_que_attr_activetime_get() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ a l i a s _ l i s t
 *
 *  OVERVIEW:
 *  Return the list of queue aliases by name from queue attributes structure.
 *  The first entry in the list is the queue name itself. 
 *  To modify/rename/move the queue name, use mos_que_mgmt_move().
 *
 *  INPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  space_avail:
 *                  Specifies the size of the "que_alias_list" array.
 *                  The caller is responsible for allocating the space
 *                  for "space_avail" queue alias names.
 *
 *
 *  OUTPUTS:
 *  num_returned:
 *                  The number of queue aliases for a queue returned in
 *                  the "que_alias_list".
 *
 *  que_alias_list:
 *                  Array of queue aliases returned.
 *                  The size of the array is determined by "space_avail"
 *                  and the length by "num_returned".
 *
 *  num_left:
 *                  Number of queue aliases that were found but could not
 *                  be returned because of space constraints of
 *                  "que_alias_list" array. Note that this number may be
 *                  inaccurate if queue aliases have been created/deleted
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
mos_que_attr_alias_list(
                        mos_que_attr_t          que_attributes,
                        unsigned32              space_avail,
                        unsigned32 *            num_returned,
                        mos_rsrc_name_t         que_alias_list[],
                        unsigned32 *            num_left,
                        error_status_t *        status
                         )
{
    int                          i, j;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);
    *num_returned = 0;
    *num_left = 0;
    if (space_avail) {
        for (i = 0; i < space_avail; i++) {   
            *que_alias_list[i] = '\0';
        }
    } else {
        SET_STATUS(status, mos_s_space_avail_req);  
        return;
    }

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    /*
     * First entry in the list is the queue name itself.
     */
    strcpy((char *)que_alias_list[0], (char *)que_attributes->name);
    (*num_returned)++;

    /*
     * Return only what was allocated
     */
    for (i = 1, j = 0; j < que_attributes->aliases.count; i++, j++){
        if (i < space_avail) {
           strcpy((char *)que_alias_list[i], 
                  (char *)que_attributes->aliases.list[j]);
           (*num_returned)++;
        } else {
           (*num_left)++; 
        }
    } /* End of loop thru all queue aliases */

    return;

} /* End of mos_que_attr_alias_list() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ a l i a s _ a d d
 *
 *  OVERVIEW:
 *  Adds one queue alias to the list of queue aliases in the queue 
 *  attributes structure. If the link name parameter is specified and 
 *  a named link does not already exist, a named link is implicitly 
 *  created to the respective queue manager. It does not create the 
 *  queue itself. To create a queue, use mos_que_mgmt_create().
 *
 *  Note that the queue manager verifies that new queue aliases
 *  are unique amongst all queue names during a mos_que_mgmt_create()
 *  or mos_que_attr_commit() operation.
 *
 *  INPUTS:
 *  link_name:
 *              Indicates that the name is a named link to the queue
 *              manager in question. If NULL, a named link is not
 *              implicitly created or deleted. Otherwise, it is.
 *
 *  que_alias_name:
 *              Provides a mechanism for an application to access a queue
 *              by one or more names.
 *
 *  INPUTS/OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
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
mos_que_attr_alias_add(
                       mos_rsrc_name_t                link_name,
                       mos_rsrc_name_t                que_alias_name,
                       mos_que_attr_t                 que_attributes,
                       error_status_t *               status
                       )
{
    int                       i;
    int                       found = FALSE;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
   if (que_attributes == NULL) {
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    /*
     * Determine if a named link needs to be created XXX try to bind server
     * Concatentate link name to alias name? XXX
     */

    /*
     * Validate queue alias 
     */
    if ((que_alias_name == NULL) || (que_alias_name[0] == '\0')) {
        SET_STATUS(status, mos_s_quename_null); 
        return;
    } 

    /*
     * Determine if specified alias already exists in the list
     */
    for (i = 0; i < que_attributes->aliases.count; i++) {
        if (strcmp((char *)que_alias_name, 
                   (char *)que_attributes->aliases.list[i]) == 0) {
            found = TRUE;
            break;
        }
    }
    if (found) {
        SET_STATUS(status, mos_s_quename_already_exists);  
        return;
    }

    /*
     * Allocate space for one more alias
     */
    que_attributes->aliases.list = 
               (mos_string_t *)realloc(que_attributes->aliases.list, 
                                       sizeof(idl_char *) *
                                       (que_attributes->aliases.count + 1));
    if (que_attributes->aliases.list == NULL) {
       SET_STATUS(status, mos_s_no_memory);  
       return;
    }

    /*
     * Add new alias
     */
    que_attributes->aliases.list[que_attributes->aliases.count] = 
                    (idl_char *)dce_strdup((char *)que_alias_name);

    /*
     * Increment counter in the queue attributes strucuture
     */
    que_attributes->aliases.count++;

    return;

} /* End of mos_que_attr_alias_add() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ a l i a s _ r e m o v e
 *
 *  OVERVIEW:
 *  Rremoves one queue alias from the list of queue aliases in the 
 *  queue attributes structure. If the link name parameter is specified 
 *  and a named link exists, a named link is implicitly removed from the 
 *  respective queue manager. It does not delete the queue itself. To delete 
 *  a queue, use mos_que_mgmt_delete().
 *
 *  INPUTS:
 *  link_name:
 *              Indicates that the name is a named link to the queue
 *              manager in question. If NULL, a named link is not
 *              implicitly created or deleted. Otherwise, it is.
 *
 *  que_alias_name:
 *              Provides a mechanism for an application to access a queue
 *              by one or more names.
 *
 *  INPUTS/OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
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
mos_que_attr_alias_remove(
                       mos_rsrc_name_t                link_name,
                       mos_rsrc_name_t                que_alias_name,
                       mos_que_attr_t                 que_attributes,
                       error_status_t *               status
                       )
{
    int                       i,j;
    int                       found = FALSE;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL) {
        SET_STATUS(status, mos_s_que_attr_inv); 
        return;
    } 

    /*
     * Determine if a named link needs to be removed XXX try to bind server
     * Concatentate link name to alias name? XXX
     */

    /*
     * Validate queue alias 
     */
    if ((que_alias_name == NULL) || (que_alias_name[0] == '\0')) {
        SET_STATUS(status, mos_s_quename_null);  
        return;
    } 

    /*
     * Determine if specified alias exists
     */
    for (i = 0; i < que_attributes->aliases.count; i++) {

        if (strcmp((char *)que_alias_name, 
                   (char *)que_attributes->aliases.list[i]) == 0) {
            found = TRUE;
            /*
             * Remove alias
             */
            free(que_attributes->aliases.list[i]);  
            /*
             * Shift any remaining aliases down the list
             */
            for ( j = i; j < que_attributes->aliases.count-1; j++) {
                que_attributes->aliases.list[j] =
                           que_attributes->aliases.list[j+1];
            }
            if (j) {
                que_attributes->aliases.list[j] = NULL;       
            } else {
                que_attributes->aliases.list = NULL;       
            }
            /*
             * Decrement counter in queue attributes structure
             */
            que_attributes->aliases.count--;
            break;
        }
    } /* End of loop thru alias list */

    if (! found) {
        SET_STATUS(status, mos_s_no_quename_exists);  
        return;
    }

    return;

} /* End of mos_que_attr_alias_remove() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ a l i a s _ r e s e t
 *
 *  OVERVIEW:
 *  Sets the queue alias list to NULL in the queue attributes structure. 
 *  Note that the queue name itself is not reset, since internally the 
 *  queue name is stored separately from the queue alias list. Also
 *  named links or the queue itself are not removed; use 
 *  mos_que_attr_alias_remove() and mos_que_mgmt_delete() respectively.
 *
 *  INPUTS/OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
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
mos_que_attr_alias_reset(
                       mos_que_attr_t                 que_attributes,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL) {
        SET_STATUS(status, mos_s_que_attr_inv); 
        return;
    } 

    /*
     * Discard all aliases
     */
    mos_string_list_free_ptrs(&que_attributes->aliases);

    return;

} /* End of mos_que_attr_alias_reset() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ a n n o t a t i o n _ g e t
 *
 *  OVERVIEW:
 *  Obtain the queue annotation from the queue attributes structure. 
 *  Queue annotation is a read/write attribute.
 *
 *  INPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  OUTPUTS:
 *  annotation:
 *                 Defines a character string comment about the queue.
 *                 The string is used by applications for informational
 *                 purposes only. The string can be up to 64 characters
 *                 long, including the NULL terminating character. A
 *                 NULL value indicates no annotation string.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_annotation_get(
                       mos_que_attr_t                 que_attributes,
                       mos_que_attr_annotation_t      annotation,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    if (que_attributes->annotation) {
        strcpy((char *)annotation, (char *)que_attributes->annotation);
    } else {
        *annotation = NULL;  
    }

    return;

} /* End of mos_que_attr_annotation_get() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ a n n o t a t i o n _ s e t
 *
 *  OVERVIEW:
 *  Modify the queue annotation from the queue attributes structure. 
 *  Queue annotation is a read/write attribute.
 *
 *  INPUTS:
 *  annotation:
 *                 Defines a character string comment about the queue.
 *                 The string is used by applications for informational
 *                 purposes only. The string can be up to 64 characters
 *                 long, including the NULL terminating character. A
 *                 NULL value indicates no annotation string.
 *
 *  INPUTS/OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
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
mos_que_attr_annotation_set(
                       mos_que_attr_annotation_t      annotation,
                       mos_que_attr_t                 que_attributes,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL) {
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    /*
     * Release old annotation
     */
    if (que_attributes->annotation) {
        free(que_attributes->annotation);
        que_attributes->annotation = NULL;
    }

    /*
     * Set new queue annotation 
     */
    if ((annotation != NULL) && (annotation[0] != '\0')) {
        que_attributes->annotation = 
                              (idl_char *)dce_strdup((char *)annotation);
    }

    return;

} /* End of mos_que_attr_annotation_set() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ q u e m a x s i z e _ g e t
 *
 *  OVERVIEW:
 *  Obtain the maximum queue size from the queue attributes structure. 
 *  Queue maximum size a read/write attribute.
 *
 *  To protect a queue manager's memory and disk resources, the
 *  maximum amount of space required for a queue is the maximum
 *  queue size multiplied by the maximum message size. If unlimited
 *  queue and message sizes are requested or if message persistence 
 *  dicates, then the maximum amount of space required for a queue
 *  is uncertain.
 *
 *  INPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  OUTPUTS:
 *  que_maxsize:
 *                Maximum queue size allowed.
 *                A default queue size of zero indicates that the number
 *                of queue elements is unlimited but subject to the
 *                availability of system resources.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_quemaxsize_get(
                       mos_que_attr_t                 que_attributes,
                       unsigned32 *                   que_maxsize,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    *que_maxsize = que_attributes->max_qlen;

    return;

} /* End of mos_que_attr_quemaxsize_get() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ q u e m a x s i z e _ s e t
 *
 *  OVERVIEW:
 *  Modify the maximum queue size from the queue attributes structure. 
 *  Queue maximum size a read/write attribute.
 *
 *  To protect a queue manager's memory and disk resources, the
 *  maximum amount of space required for a queue is the maximum
 *  queue size multiplied by the maximum message size. If unlimited
 *  queue and message sizes are requested or if message persistence 
 *  dicates, then the maximum amount of space required for a queue
 *  is uncertain.
 *
 *  INPUTS:
 *  que_maxsize:
 *                Maximum queue size allowed.
 *                A default queue size of zero indicates that the number
 *                of queue elements is unlimited but subject to the
 *                availability of system resources.
 *
 *  INPUTS/OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
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
mos_que_attr_quemaxsize_set(
                       unsigned32                     que_maxsize,
                       mos_que_attr_t                 que_attributes,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL) {
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    /*
     * Set queue maximum size 
     */
    que_attributes->max_qlen = que_maxsize;

    return;

} /* End of mos_que_attr_quemaxsize_set() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ m s g m a x s i z e _ g e t
 *
 *  OVERVIEW:
 *  Obtain the maximum message size allowed on a queue from the queue 
 *  attributes structure. 
 *  Message maximum size a read/write attribute.
 *
 *  To protect a queue manager's memory and disk resources, the
 *  maximum amount of space required for a queue is the maximum
 *  queue size multiplied by the maximum message size. If unlimited
 *  queue and message sizes are requested or if message persistence 
 *  dicates, then the maximum amount of space required for a queue
 *  is uncertain.
 *
 *  INPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  OUTPUTS:
 *  msg_maxsize:
 *                Maximum message size allowed on a queue.
 *                A default message size of zero indicates that a message
 *                can be of any size but subject to the availability of
 *                system resources.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_msgmaxsize_get(
                       mos_que_attr_t                 que_attributes,
                       unsigned32 *                   msg_maxsize,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    *msg_maxsize = que_attributes->max_msgsize;

    return;

} /* End of mos_que_attr_msgmaxsize_get() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ m s g m a x s i z e _ s e t
 *
 *  OVERVIEW:
 *  Modify the maximum message size allowed on a queue from the queue 
 *  attributes structure. 
 *  Message maximum size a read/write attribute.
 *
 *  To protect a queue manager's memory and disk resources, the
 *  maximum amount of space required for a queue is the maximum
 *  queue size multiplied by the maximum message size. If unlimited
 *  queue and message sizes are requested or if message persistence 
 *  dicates, then the maximum amount of space required for a queue
 *  is uncertain.
 *
 *  INPUTS:
 *  msg_maxsize:
 *                Maximum message size allowed on a queue.
 *                A default message size of zero indicates that a message
 *                can be of any size but subject to the availability of
 *                system resources.
 *
 *  INPUTS/OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
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
mos_que_attr_msgmaxsize_set(
                       unsigned32                     msg_maxsize,
                       mos_que_attr_t                 que_attributes,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL) {
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    /*
     * Set message maximum size 
     */
    que_attributes->max_msgsize = msg_maxsize;

    return;

} /* End of mos_que_attr_msgmaxsize_set() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ p e r s i s t e n c e _ g e t
 *
 *  OVERVIEW:
 *  Obtain the queue persistence from the queue attributes structure. 
 *  Queue persistence is a read/write attribute.
 *
 *  INPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  OUTPUTS:
 *  persistence:
 *                  Indicates whether the queue is in memory or on disk.
 *                  The default queue persistence is 
 *                  mos_c_que_attr_msgpersistence.
 *
 *                  Note that there is a persistence attribute on messages
 *                  as well as queues. If the message persistence is
 *                  "persistent" and the queue persistence is "volatile",
 *                  the queue manager stores the message in memory only and
 *                  returns a warning message to the caller of the send
 *                  operation. If the queue persistence is "msgpersistence",
 *                  then the queue manager stores the message where the message
 *                  persistence dicates. Note that if message persistence
 *                  dicates, then messages may be enqueued in memory as well
 *                  as on disk. The following table describes how a
 *                  message is stored based on the persistence attribute of
 *                  both the queue and the message itself.
 *
 *                            | Use Msg     | Volatile   | Persistent
 *                            | Persistence | Queue      | Queue
 *                  -------------------------------------------------
 *                  Volatile  |             |            |
 *                  Message   | Volatile    | Volatile   | Persistent
 *                            |             |            |
 *                  -------------------------------------------------
 *                  Persisent |             |            |
 *                  Message   | Persistent  | Volatile   | Persistent
 *                            |             | w/warning  |
 *
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_persistence_get(
                             mos_que_attr_t                 que_attributes,
                             mos_que_attr_persistence_t *   persistence,
                             error_status_t *               status
                            )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    *persistence = que_attributes->persistence;

    return;

} /* End of mos_que_attr_persistence_get() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ p e r s i s t e n c e _ s e t
 *
 *  OVERVIEW:
 *  Modify the queue persistence from the queue attributes structure. 
 *  Queue persistence is a read/write attribute.
 *
 *  INPUTS:
 *  persistence:
 *                  See above
 *
 *  INPUTS/OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
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
mos_que_attr_persistence_set(
                             mos_que_attr_persistence_t     persistence,
                             mos_que_attr_t                 que_attributes,
                             error_status_t *               status
                            )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL) {
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    /*
     * If valid, set queue persistence
     */
    if (mos__is_quepersistence_valid(persistence, status)) {
        que_attributes->persistence = persistence;
    }

    return;

} /* End of mos_que_attr_persistence_set() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ q u e p e r s i s t e n c e _ v a l i d
 *
 *  OVERVIEW:
 *  Validate queue persistence.
 *
 *  INPUTS:
 *  persistence:
 *                  See above
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
mos__is_quepersistence_valid(
                             mos_que_attr_persistence_t  persistence,
                             error_status_t *            status
                            )
{
    switch (persistence) {
       case (mos_c_que_attr_msgpersistence):
       case (mos_c_que_attr_volatile):
       case (mos_c_que_attr_persistent):
           return(TRUE);
       default:
           SET_STATUS(status, mos_s_que_attr_persistence_inv);   
           return(FALSE);
    } /* End of switch on queue persistence */

} /* End of mos__is_quepersistence_valid() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ i d l e t i m e o u t _ g e t
 *
 *  OVERVIEW:
 *  Obtain the empty queue idle timeout in relative time from the 
 *  queue attributes structure. 
 *  Empty queue idle timeout is a read/write attribute.
 *
 *  INPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  OUTPUTS:
 *  que_idle_timeout:
 *                Indicates the maximum amount of time where no activity
 *                occurs on an empty queue. Once this timeout is reached,
 *                its queue manager is free to implicitly delete the empty
 *                queue at any time thereafter. If this value is not equal
 *                to NULL, the queue manager starts the idle timer when a
 *                queue is empty.
 *                Specified in relative time only.
 *
 *                The default queue idle timeout is 24 hours. An empty
 *                queue idle timeout of NULL indicates that a queue
 *                remains indefinitely until explicitly deleted via
 *                mos_que_mgmt_delete(). 
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_idletimeout_get(
                       mos_que_attr_t                 que_attributes,
                       utc_t *                        que_idle_timeout,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    if (que_attributes->flags & mos_c_use_empty_timeout) {
        *que_idle_timeout = que_attributes->empty_timeout;
    } else {
        *que_idle_timeout = null_utc;
        SET_STATUS(status, mos_s_que_attr_idletimeout_null);  
    }

    return;

} /* End of mos_que_attr_idletimeout_get() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ i d l e t i m e o u t _ s e t
 *
 *  OVERVIEW:
 *  Modify the empty queue idle timeout in relative time from the 
 *  queue attributes structure. 
 *  Empty queue idle timeout is a read/write attribute.
 *
 *  INPUTS:
 *  que_idle_timeout:
 *                Indicates the maximum amount of time where no activity
 *                occurs on an empty queue. Once this timeout is reached,
 *                its queue manager is free to implicitly delete the empty
 *                queue at any time thereafter. If this value is not equal
 *                to NULL, the queue manager starts the idle timer when a
 *                queue is empty.
 *                Specified in relative time only.
 *
 *                The default queue idle timeout is 24 hours. An empty
 *                queue idle timeout of NULL indicates that a queue
 *                remains indefinitely until explicitly deleted via
 *                mos_que_mgmt_delete(). 
 *
 *  INPUTS/OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
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
mos_que_attr_idletimeout_set(
                       utc_t *                        que_idle_timeout,
                       mos_que_attr_t                 que_attributes,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL) {
        SET_STATUS(status, mos_s_que_attr_inv); 
        return;
    } 

    /*
     * Validate and set queue idle timeout 
     * Assume relative time specified.
     */
    if (que_idle_timeout == NULL) {
        que_attributes->flags &= ~mos_c_use_empty_timeout;
        que_attributes->empty_timeout = null_utc;
    } else {
        que_attributes->flags |= mos_c_use_empty_timeout;
        que_attributes->empty_timeout = *que_idle_timeout;
    }

    return;

} /* End of mos_que_attr_idletimeout_set() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ a l l o w e n q _ g e t
 *
 *  OVERVIEW:
 *  Obtain whether enqueuing is allowed or not from
 *  the queue attributes structure.
 *  Queue enqueue service is a read/write attribute.
 *
 *  INPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  OUTPUTS:
 *  allowenq:
 *                 Indicates whether enqueuing is allowed or not.
 *                 The default is TRUE.
 *                 TRUE indicates allow enqueuing.
 *                 FALSE indicates inhibit enqueuing.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_allowenq_get(
                       mos_que_attr_t                 que_attributes,
                       boolean *                      allow_enq,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    *allow_enq = que_attributes->enq_enabled;

    return;

} /* End of mos_que_attr_allowenq_get() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ a l l o w e n q _ s e t
 *
 *  OVERVIEW:
 *  Modify whether enqueuing is allowed or not from
 *  the queue attributes structure.
 *  Queue enqueue service is a read/write attribute.
 *
 *  INPUTS:
 *  allowenq:
 *                 Indicates whether enqueuing is allowed or not.
 *                 The default is TRUE.
 *                 TRUE indicates allow enqueuing.
 *                 FALSE indicates inhibit enqueuing.
 *
 *  INPUTS/OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
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
mos_que_attr_allowenq_set(
                       boolean                        allow_enq,
                       mos_que_attr_t                 que_attributes,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL) {
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    /*
     * If valid, set queue enqueue service 
     */
    if (mos__is_boolean_valid(allow_enq, status)) {
        que_attributes->enq_enabled = allow_enq;
    }

    return;

} /* End of mos_que_attr_allowenq_set() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ a l l o w d e q _ g e t
 *
 *  OVERVIEW:
 *  Obtain whether dequeuing is allowed or not from
 *  the queue attributes structure.
 *  Queue dequeue service is a read/write attribute.
 *
 *  INPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
 *
 *  OUTPUTS:
 *  allowdeq:
 *                 Indicates whether dequeuing is allowed or not.
 *                 The default is TRUE.
 *                 TRUE indicates allow dequeuing.
 *                 FALSE indicates inhibit dequeuing.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_que_attr_allowdeq_get(
                       mos_que_attr_t                 que_attributes,
                       boolean *                      allow_deq,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL){
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    *allow_deq = que_attributes->deq_enabled;

    return;

} /* End of mos_que_attr_allowdeq_get() routine */


/*
 *  FUNCTION:
 *  m o s _ q u e _ a t t r _ a l l o w d e q _ s e t
 *
 *  OVERVIEW:
 *  Modify whether dequeuing is allowed or not from
 *  the queue attributes structure.
 *  Queue dequeue service is a read/write attribute.
 *
 *  INPUTS:
 *  allowdeq:
 *                 Indicates whether dequeuing is allowed or not.
 *                 The default is TRUE.
 *                 TRUE indicates allow dequeuing.
 *                 FALSE indicates inhibit dequeuing.
 *
 *  INPUTS/OUTPUTS:
 *  que_attributes:
 *                  Opaque pointer to the queue attributes structure obtained
 *                  via mos_que_attr_alloc() or mos_que_attr_peek().
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
mos_que_attr_allowdeq_set(
                       boolean                        allow_deq,
                       mos_que_attr_t                 que_attributes,
                       error_status_t *               status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate queue attributes structure
     */
    if (que_attributes == NULL) {
        SET_STATUS(status, mos_s_que_attr_inv);  
        return;
    } 

    /*
     * If valid, set queue dequeue service 
     */
    if (mos__is_boolean_valid(allow_deq, status)) {
        que_attributes->deq_enabled = allow_deq;
    }

    return;

} /* End of mos_que_attr_allowdeq_set() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ b o o l e a n _ v a l i d
 *
 *  OVERVIEW:
 *  Validate boolean.
 *
 *  INPUTS:
 *  value:          A boolean value.
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
mos__is_boolean_valid(
                      boolean                     value,
                      error_status_t *            status
                      )
{
    switch (value) {
       case (FALSE):
       case (TRUE):
           return(TRUE);
       default:
           SET_STATUS(status, mos_s_boolean_inv);   
           return(FALSE);
    } /* End of switch on boolean value */

} /* End of mos__is_boolean_valid() routine */

