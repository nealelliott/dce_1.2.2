/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 *  MODULE:
 *  m o s _ m s g _ a t t r . c
 *
 *  DESCRIPTION:
 *  Client interfaces for Message Attribute Utilities to support the 
 *  Message Oriented Service Application Programming Interface(MOS-API).
 *
 * ______________________________________________________________________
 *
 * $Log: mos_msg_attr.c,v $
 * Revision 1.12  1996/11/20  18:27:01  salamone
 * Update flags desc
 *
 * Revision 1.11  96/11/20  14:47:17  14:47:17  salamone (Julie Salamone)
 * Fix header file declarations
 * 
 * Revision 1.10  96/11/20  14:20:43  14:20:43  salamone (Julie Salamone)
 * Use API to free attributes
 * 
 * Revision 1.9  96/11/13  18:20:47  18:20:47  salamone (Julie Salamone)
 * Allow apps to use mos_datatype_ack
 * 
 * Revision 1.8  96/11/13  15:02:18  15:02:18  salamone (Julie Salamone)
 * In notice_set(), implicitly register the ACK datatype for a notice message
 * 
 * Revision 1.7  96/10/25  16:50:50  16:50:50  salamone (Julie Salamone)
 * Clean-up I/O params on _set() calls
 * 
 * Revision 1.6  96/10/25  14:43:28  14:43:28  salamone (Julie Salamone)
 * Add rsrc protectlvl support
 * 
 * Revision 1.5  96/10/23  16:13:10  16:13:10  salamone (Julie Salamone)
 * Add selection filter support
 * 
 * Revision 1.4  96/10/18  11:00:15  11:00:15  salamone (Julie Salamone)
 * Added more notice opts/dest validation support
 * 
 * Revision 1.3  96/10/16  14:44:12  14:44:12  salamone (Julie Salamone)
 * Added msg and que attr codes
 * 
 * Revision 1.2  96/10/15  17:24:45  17:24:45  salamone (Julie Salamone)
 * Added function descriptions
 * 
 * Revision 1.1  96/10/14  16:51:23  16:51:23  salamone (Julie Salamone)
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
#include <mos_ack.h>
#include <mos_base.h>
#include <mos_q.h>
#include <dcemosmsg.h>
#include <dcemosmac.h>
#include <dcemossvc.h>

static mos_mattr_t       null_msg_attr;  /* NULL message attributes structure */
static utc_t             null_utc;       /* NULL UTC */


/*
 * INTERNAL PROTOTYPES
 */
void mos__dflt_ttl_set(utc_t *);


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ a l l o c
 *
 *  OVERVIEW:
 *  Allocates memory within MOS and returns an opaque pointer to 
 *  a message attributes structure with defaults set.
 *
 *  It is the responsibilty of the caller to release memory
 *  via mos_msg_attr_free().
 *
 *  OUTPUTS:
 *  msg_attributes:
 *                  Opaque pointer to default message attributes structure.
 *                  Use the other mos_msg_attr_X_set() APIs to modify any
 *                  attributes.
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_alloc(
                   mos_msg_attr_t *         msg_attributes,
                   error_status_t *         status
                  )
{
    mos_mattr_t *       msg_attr_p;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);
    *msg_attributes = NULL;

    /*
     * Allocate space
     */
    msg_attr_p = (mos_mattr_t *)malloc(sizeof(mos_mattr_t));
    if (msg_attr_p == NULL) {
       SET_STATUS(status, mos_s_no_memory);   
       return;
    }

    /*
     * Set initially to NIL structure
     */
    *msg_attr_p = null_msg_attr; 

    /*
     * Set message attribute defaults
     */
    msg_attr_p->msgtype  =         mos_c_msg_attr_msgtype_data;
    msg_attr_p->priority =         mos_c_msg_attr_dflt_priority;
    msg_attr_p->persistence =      mos_c_msg_attr_volatile;  
    msg_attr_p->notice_opts =      mos_c_msg_attr_notice_none;
    msg_attr_p->protectlvl =       mos_c_protect_level_default;

    /* 
     * Data type used for marshalling purposes 
     * Set default NIL data type to indicate an array of bytes 
     * and no marshalling.
     */
    uuid_create_nil(&msg_attr_p->datatype, status);

    /* 
     * Set default message expiration time only.
     * The message ttr time is not used by default and the message enqueue
     * time stamp is placed on the message by the queue manager. 
     */
    msg_attr_p->flags |= mos_c_use_expire_time;  
    mos__dflt_ttl_set(&msg_attr_p->expire_time);

    /*
     * Return message attribute structure
     */
    *msg_attributes = (mos_msg_attr_t)msg_attr_p;
    return;

} /* End of mos_msg_attr_alloc() routine */


/*
 *  FUNCTION:
 *  m o s _ _ d f l t _ t t l _ s e t
 *
 *  OVERVIEW:
 *  Determine the default message expiration(time-to-live) in absolute time.
 *
 *  OUTPUTS:
 *  expire_time:
 *                 Indicates the lifetime of the message if not retrieved.
 *                 Specified in absolute time.
 *
 *  RETURNS:       None
 */
void
mos__dflt_ttl_set(
                   utc_t *        expire_time
                  )
{
    utc_t               now, ttl;
    timespec_t          tzero;
    reltimespec_t       t_ttl;

    /*
     * Construct a timestamp that represents "mos_c_msg_attr_dflt_ttl" hours
     */
    t_ttl.tv_sec = (3600 * mos_c_msg_attr_dflt_ttl);
    t_ttl.tv_nsec = 0;
    tzero.tv_sec = 0;
    tzero.tv_nsec = 0;
    utc_mkbinreltime(&ttl,
                     &t_ttl,
                     &tzero);
    /*
     * Get current time
     */
    utc_gettime(&now);

    /* 
     * Add "mos_c_msg_attr_dflt_ttl" hours to current time
     * to set the message expiration time
     */
    utc_addtime(expire_time,
                &now,                /* Use current time */
                &ttl);               /* Default ttl time */

} /* End of mos__dflt_ttl_set() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ p e e k
 *
 *  OVERVIEW:
 *  Returns an opaque pointer to the message attributes on an 
 *  existing message in the specified queue. If no selection
 *  criteria is specified, then the first message's attributes
 *  in the queue is returned.
 *  Applies to Message Queuing model only.
 *
 *  It is the responsibilty of the caller to release memory
 *  via mos_msg_attr_free().
 *
 *  INPUTS:
 *  resource_handle:
 *                  A context handle to a resource.
 *  flags:
 *                  Indicates the message retrieval mode. Only one option
 *                  is allowed to be specified.
 *                  The default is mos_c_mode_nowait_deq.
 *
 *  msg_selection_filter:
 *                  Opaque pointer to the selection filter structure.
 *                  Use the mos_msg_selfilter_alloc(), mos_msg_selfilter_free(),
 *                  and mos_msg_selmask_add() APIs to allocate, deallocate and
 *                  define one or more selection criteria. A message is
 *                  successfully retrieved following the rules of the logical
 *                  AND operator among all specified criteria.
 *                  A NULL value indicates no selection criteria and the
 *                  message's attributes at the top of the specified queue
 *                  is returned.
 * 
 *  OUTPUTS:
 *  msg_attributes:
 *                  Use the utilities to view the message attributes.
 *                  It is the caller's responsibility to release the
 *                  memory allocated for the message attributes buffer
 *                  via mos_msg_attr_free().
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_peek(
                  mos_handle_t             resource_handle,
                  unsigned32               flags,
                  mos_msg_selfilter_t      msg_selection_filter,
                  mos_msg_attr_t *         msg_attributes,
                  error_status_t *         status
                 )
{
   mos_mattr_t *                msg_attr_p;
   error_status_t               temp_status;

   /*
    * Initialize output parameters
    */
   *msg_attributes = NULL;
   CLEAR_STATUS(status);

   /*
    * Validate resource handle
    */
   if (! mos__is_rsrc_handle_valid(TRUE, resource_handle, status)) {
      return;
   }

   /*
    * Validate dequeue mode flags.
    */
   if (! mos__is_deqmode_valid(flags, status)) {
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
    * Allocate space for message attributes
    */
   msg_attr_p = (mos_mattr_t *)malloc(sizeof(mos_mattr_t));
   if (msg_attr_p == NULL) {
      SET_STATUS(status, mos_s_no_memory);  
      return;
   }
  
   /*
    * Request to peek at message attributes
    */
   mos_q_retrieve_mattr(resource_handle->server_handle,
                        resource_handle->resource_name,
                        msg_selection_filter,
                        flags,
                        msg_attr_p,
                        status);

   if (GOOD_STATUS(status)) {
       *msg_attributes = (mos_msg_attr_t)msg_attr_p;
   } else {
       mos_msg_attr_free(&msg_attr_p, &temp_status);
   }
   return;

} /* End of mos_msg_attr_peek() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ f r e e
 *
 *  OVERVIEW:
 *  Releases memory used for message attributes.
 *  It is the responsibility of the caller to release the memory 
 *  if allocated by mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *  INPUTS/OUTPUTS:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure to
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
mos_msg_attr_free(
                  mos_msg_attr_t *         msg_attributes,
                  error_status_t *         status
                 )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate handle to message attributes
     */
    if ((msg_attributes == NULL) || (*msg_attributes == NULL)) {
        SET_STATUS(status, mos_s_msg_attr_inv); 
        return;
    }

    /*
     * Release notice destination
     */
    if ((*msg_attributes)->notice_dest) {
        free((*msg_attributes)->notice_dest);
        (*msg_attributes)->notice_dest = NULL;
    }

    /*
     * Release notice security name
     */
    if ((*msg_attributes)->notice_sec_name) {
        free((*msg_attributes)->notice_sec_name);
        (*msg_attributes)->notice_sec_name = NULL;
    }

    /*
     * Release memory and return NULL pointer
     */
    free(*msg_attributes);
    *msg_attributes = NULL;
    return;

} /* End of mos_msg_attr_free() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ m s g i d _ g e t
 *
 *  OVERVIEW:
 *  Return message ID from a message attributes structure.
 *  Message ID is a read-only attribute.
 *
 *  INPUTS:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *  OUTPUTS:
 *  msg_id:
 *                  Unique message ID generated by the send API operations.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_msgid_get(
                       mos_msg_attr_t            msg_attributes,
                       uuid_t *                  msg_id,
                       error_status_t *          status
                      )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL){
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    *msg_id = msg_attributes->id;

    return;

} /* End of mos_msg_attr_msgid_get() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ e n q t i m e _ g e t
 *
 *  OVERVIEW:
 *  Return message enqueue time stamp in absolute time from a message 
 *  attributes structure.
 *  Message enqueue time stamp is a read-only attribute.
 *
 *  INPUTS:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *  OUTPUTS:
 *  msg_enq_time:
 *                 Indicates when a message was enqueued successfully
 *                 by the messaging system.
 *                 Specified in absolute time only.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_enqtime_get(
                         mos_msg_attr_t          msg_attributes,
                         utc_t *                 msg_enq_time,
                         error_status_t *        status
                        )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL){
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    *msg_enq_time = msg_attributes->time_enqueued;

    return;

} /* End of mos_msg_attr_enqtime_get() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ m s g t y p e _ g e t
 *
 *  OVERVIEW:
 *  Return message type from a message attributes structure.
 *
 *  INPUTS:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *  OUTPUTS:
 *  msg_type:
 *                 Describes the message type for messaging system purposes.
 *                 The default is mos_c_msg_attr_msgtype_data.
 *                 A message type of "mos_c_msg_attr_msgtype_notice" is
 *                 only sent by a queue manager.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_msgtype_get(
                         mos_msg_attr_t           msg_attributes,
                         mos_msg_attr_msgtype_t * msg_type,
                         error_status_t *         status
                        )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL){
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    *msg_type = msg_attributes->msgtype;

    return;

} /* End of mos_msg_attr_msgtype_get() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ m s g t y p e _ s e t
 *
 *  OVERVIEW:
 *  Set message type in a message attributes structure.
 *
 *  INPUTS:
 *  msg_type:
 *                 Describes the message type for messaging system purposes.
 *                 The default is mos_c_msg_attr_msgtype_data.
 *                 A message type of "mos_c_msg_attr_msgtype_notice" is
 *                 only sent by a queue manager.
 *
 *  INPUT/OUTPUT:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *  OUTPUTS:
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_msgtype_set(
                         mos_msg_attr_msgtype_t  msg_type,
                         mos_msg_attr_t          msg_attributes,
                         error_status_t *        status
                        )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL) {
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    /*
     * If valid, set message type.  
     */
    if (mos__is_msgtype_valid(msg_type, status)) {
        msg_attributes->msgtype = msg_type;
    }

    return;

} /* End of mos_msg_attr_msgtype_set() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ m s g t y p e _ v a l i d
 *
 *  OVERVIEW:
 *  Validate message type.
 *
 *  INPUTS:
 *  msg_type:
 *                 Describes the message type for messaging system purposes.
 *                 The default is mos_c_msg_attr_msgtype_data.
 *                 A message type of "mos_c_msg_attr_msgtype_notice" is
 *                 only sent by a queue manager.
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
mos__is_msgtype_valid(
                      mos_msg_attr_msgtype_t  msg_type,
                      error_status_t *        status
                     )
{
    CLEAR_STATUS(status);

    switch (msg_type) {
       case (mos_c_msg_attr_msgtype_data):
       case (mos_c_msg_attr_msgtype_notice):
           return(TRUE);
       default:
           SET_STATUS(status, mos_s_msg_attr_msgtype_inv);   
           return(FALSE);
    } /* End of switch on message types */

} /* End of mos__is_msgtype_valid() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ p r i o r i t y _ g e t
 *
 *  OVERVIEW:
 *  Return priority from a message attributes structure.
 *
 *  INPUTS:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *  OUTPUTS:
 *  priority:
 *                  A number from 0 thru (2 to the power of 32-1),
 *                  where 0 indicates highest priority.
 *                  The default is mos_c_msg_attr_dflt_priority.
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_priority_get(
                          mos_msg_attr_t          msg_attributes,
                          unsigned32 *            priority,
                          error_status_t *       status
                         )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL){
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    *priority = msg_attributes->priority;

    return;

} /* End of mos_msg_attr_priority_get() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ p r i o r i t y _ s e t
 *
 *  OVERVIEW:
 *  Set priority in a message attributes structure.
 *
 *  INPUTS:
 *  priority:
 *                  A number from 0 thru (2 to the power of 32-1),
 *                  where 0 indicates highest priority.
 *                  The default is mos_c_msg_attr_dflt_priority.
 *  INPUT/OUTPUT:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *  OUTPUTS:
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_priority_set(
                          unsigned32              priority,
                          mos_msg_attr_t          msg_attributes,
                          error_status_t *        status
                         )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL) {
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    /*
     * Set message priority 
     */
    msg_attributes->priority = priority;

    return;

} /* End of mos_msg_attr_priority_set() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ t t l _ g e t
 *
 *  OVERVIEW:
 *  Return message expiration(time-to-live) in absolute time from a 
 *  message attributes structure.
 *
 *  INPUTS:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *  OUTPUTS:
 *  ttl:
 *                 Indicates the lifetime of the message if not retrieved.
 *                 Once a message has expired, its queue manager is free
 *                 to implicitly delete it at any time thereafter.
 *                 Specified in absolute time.
 *
 *                 The default message ttl is 24 hours.
 *                 A message ttl of NULL indicates that a message remains
 *                 indefinitely until explicitly dequeued via
 *                 mos_msg_dequeue() or mos_msg_delete().
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_ttl_get(
                     mos_msg_attr_t           msg_attributes,
                     utc_t *                  ttl,
                     error_status_t *         status
                    )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL){
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    if (msg_attributes->flags & mos_c_use_expire_time) {
        *ttl = msg_attributes->expire_time;
    } else {
        *ttl = null_utc;
        SET_STATUS(status, mos_s_msg_attr_ttl_null);  
    }

    return;

} /* End of mos_msg_attr_ttl_get() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ t t l _ s e t
 *
 *  OVERVIEW:
 *  Set message expiration(time-to-live) in absolute time from a message 
 *  attributes structure.
 *
 *  INPUTS:
 *  ttl:
 *                 Indicates the lifetime of the message if not retrieved.
 *                 Once a message has expired, its queue manager is free
 *                 to implicitly delete it at any time thereafter.
 *                 Specified in absolute time.
 *
 *                 The default message ttl is 24 hours.
 *                 A message ttl of NULL indicates that a message remains
 *                 indefinitely until explicitly dequeued via
 *                 mos_msg_dequeue() or mos_msg_delete().
 *
 *  INPUT/OUTPUT:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *  OUTPUTS:
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_ttl_set(
                     utc_t *                  ttl,
                     mos_msg_attr_t           msg_attributes,
                     error_status_t *         status
                    )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL) {
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    /*
     * Validate and set message expiration time 
     * Assume absolute time specified.
     */
    if (ttl == NULL) {
        msg_attributes->flags &= ~mos_c_use_expire_time;
        msg_attributes->expire_time = null_utc;
    } else {
        msg_attributes->flags |= mos_c_use_expire_time;
        msg_attributes->expire_time = *ttl;
    }

    return;

} /* End of mos_msg_attr_ttl_set() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ t t r _ g e t
 *
 *  OVERVIEW:
 *  Return time-to-receive in absolute time from a message attributes structure.
 *
 *  INPUTS:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *  OUTPUTS:
 *  ttr:
 *                  Indicates when a message is allowed to be retrieved.
 *                  Specified in absolute time.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_ttr_get(
                     mos_msg_attr_t           msg_attributes,
                     utc_t *                  ttr,
                     error_status_t *         status
                    )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL){
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    if (msg_attributes->flags & mos_c_use_valid_time) {
        *ttr = msg_attributes->valid_time;
    } else {
        *ttr = null_utc;
        SET_STATUS(status, mos_s_msg_attr_ttr_null);  
    }

    return;

} /* End of mos_msg_attr_ttr_get() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ t t r _ s e t
 *
 *  OVERVIEW:
 *  Set time-to-receive in absolute time from a message attributes structure.
 *
 *  INPUTS:
 *  ttr:
 *                  Indicates when a message is allowed to be retrieved.
 *                  Specified in absolute time.
 *  INPUT/OUTPUT:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *  OUTPUTS:
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_ttr_set(
                     utc_t *                  ttr,
                     mos_msg_attr_t           msg_attributes,
                     error_status_t *         status
                    )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL) {
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    /*
     * Validate and set message time-to_receive 
     * Assume absolute time specified.
     */
    if (ttr == NULL) {
        msg_attributes->flags &= ~mos_c_use_valid_time;
        msg_attributes->valid_time = null_utc;
    } else {
        msg_attributes->flags |= mos_c_use_valid_time;
        msg_attributes->valid_time = *ttr;
    }

    return;

} /* End of mos_msg_attr_ttr_set() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ p e r s i s t e n c e _ g e t
 *
 *  OVERVIEW:
 *  Return persistence from a message attributes structure.
 *
 *  INPUTS:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *  OUTPUTS:
 *  persistence:
 *                  Indicates whether the message is stored in memory or
 *                  on disk. The default message persistence is
 *                  mos_c_msg_attr_volatile.
 *                  Note that there is a persistence attribute on queues
 *                  as well as messages.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_persistence_get(
                             mos_msg_attr_t                  msg_attributes,
                             mos_msg_attr_persistence_t *    persistence,
                             error_status_t *                status
                            )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL){
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    *persistence = msg_attributes->persistence;

    return;

} /* End of mos_msg_attr_persistence_get() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ p e r s i s t e n c e _ s e t
 *
 *  OVERVIEW:
 *  Set persistence in a message attributes structure.
 *
 *  INPUTS:
 *  persistence:
 *                    Indicates whether the message is stored in memory or
 *                    on disk. The default message persistence is
 *                    mos_c_msg_attr_volatile.
 *                    Note that there is a persistence attribute on queues
 *                    as well as messages.
 *  INPUT/OUTPUT:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *  OUTPUTS:
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_persistence_set(
                             mos_msg_attr_persistence_t      persistence,
                             mos_msg_attr_t                  msg_attributes,
                             error_status_t *                status
                            )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL) {
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    /*
     * If valid, set message persisitence  
     */
    if (mos__is_msgpersistence_valid(persistence, status)) {
        msg_attributes->persistence = persistence;
    }

    return;

} /* End of mos_msg_attr_persistence_set() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ m s g p e r s i s t e n c e _ v a l i d
 *
 *  OVERVIEW:
 *  Validate message persistence.
 *
 *  INPUTS:
 *  persistence:
 *                  Indicates whether the message is stored in memory or
 *                  on disk. The default message persistence is
 *                  mos_c_msg_attr_volatile.
 *                  Note that there is a persistence attribute on queues
 *                  as well as messages.
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
mos__is_msgpersistence_valid(
                             mos_msg_attr_persistence_t  persistence,
                             error_status_t *            status
                            )
{
    CLEAR_STATUS(status);

    switch (persistence) {
       case (mos_c_msg_attr_volatile):
       case (mos_c_msg_attr_persistent):
           return(TRUE);
       default:
           SET_STATUS(status, mos_s_msg_attr_persistence_inv);  
           return(FALSE);
    } /* End of switch on message persistence */

} /* End of mos__is_msgpersistence_valid() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ n o t i c e _ g e t
 *
 *  OVERVIEW:
 *  Return notice options from a message attributes structure.
 *
 *  INPUTS:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *  OUTPUTS:
 *  notice_flags:
 *                  Informs messaging system when to notify the sender
 *                  of message delivery activity.
 *                  The default is mos_c_msg_attr_notice_none.
 *
 *  notice_destination:
 *                  This is the notice destination location for
 *                  a notice sent by the messaging system. In the MQ
 *                  model, the notice destination is a queue name managed
 *                  by the same queue manager or a different one. In the MP
 *                  model, this may be a destination peer application.
 *
 *  notice_security_name:
 *                  Specifies either a principal name or the security group
 *                  name where the named resource is a member.
 *
 *                  In a message queuing model, only a security group name
 *                  is allowed. Since a "notice_destination" may be to a
 *                  different queue manager, the Q-mgr sending the notice
 *                  message to the "notice_destination" can verify the
 *                  security group.
 *
 *                  In a message passing model, a sender knows what recipient
 *                  to trust and only has to specify a principal name, if
 *                  desired.
 *
 *                  Specify NULL if same Q-mgr receives notice messages.
 *                  Otherwise, NULL may be specified, but caller trusts CDS.
 *                  Not recommended.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_notice_get(
                        mos_msg_attr_t           msg_attributes,
                        unsigned32 *             notice_flags,
                        mos_rsrc_name_t          notice_destination,
                        sec_rgy_name_t           notice_security_name,
                        error_status_t *         status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL){
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    /*
     * Return notice options
     */
    *notice_flags = msg_attributes->notice_opts;
    if (msg_attributes->notice_dest) {
        strcpy((char *)notice_destination, 
               (char *)msg_attributes->notice_dest);
    } else {
        *notice_destination = NULL;
    }
    if (msg_attributes->notice_sec_name) {
        strcpy((char *)notice_security_name, 
               (char *)msg_attributes->notice_sec_name);
    } else {
        *notice_security_name = NULL;
    }

    return;

} /* End of mos_msg_attr_notice_get() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ n o t i c e _ s e t
 *
 *  OVERVIEW:
 *  Set notice options in a message attributes structure.
 *
 *  INPUTS:
 *  notice_flags:
 *                  Informs messaging system when to notify the sender
 *                  of message delivery activity.
 *                  The default is mos_c_msg_attr_notice_none.
 *
 *  notice_destination:
 *                  This is the notice destination location for
 *                  a notice sent by the messaging system. In the MQ
 *                  model, the notice destination is a queue name managed
 *                  by the same queue manager or a different one. In the MP
 *                  model, this may be a destination peer application.
 *
 *  notice_security_name:
 *                  See above in mos_msg_attr_notice_get()
 *
 *  INPUT/OUTPUT:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *
 *  OUTPUTS:
 *  resource_handle:
 *                  A context handle to the attached resource.
 *
 *                  If NULL is specified, the resource handle is not
 *                  returned. Otherwise, it is the caller's responsibility
 *                  to release the resource and any allocated memory
 *                  within MOS via mos_rsrc_detach(). For example, the
 *                  caller may already have a resource handle when the
 *                  resource was created.
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_notice_set(
                        unsigned32               notice_flags,
                        mos_rsrc_name_t          notice_destination,
                        sec_rgy_name_t           notice_security_name,
                        mos_msg_attr_t           msg_attributes,
                        mos_handle_t *           resource_handle,
                        error_status_t *         status
                       )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL) {
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    /*
     * If notice options are not set, the notice destination should be NULL.
     */
    if ((notice_flags == mos_c_msg_attr_notice_none) &&
        ((notice_destination != NULL) && (notice_destination[0] != '\0'))) {
        SET_STATUS(status, mos_s_msg_attr_noticedest_noopts); 
        return;
    }

    /*
     * If notice options are set, a notice destination is required.
     * Notice security name is optional.
     */
    if ((notice_flags != mos_c_msg_attr_notice_none) &&
        ((notice_destination == NULL) || (notice_destination[0] == '\0'))) {
        SET_STATUS(status, mos_s_msg_attr_noticedest_opts);   
        return;
    }

    /*
     * If a resource handle is requested, a notice destination is required.
     * Notice security name is optional.
     */
    if ((resource_handle) &&
        ((notice_destination == NULL) || (notice_destination[0] == '\0'))) {
        SET_STATUS(status, mos_s_msg_attr_noticedest_handle);  
        return;
    }

    /*
     * If requested, attach to notice destination
     */
    if (resource_handle) {
        mos_rsrc_attach(notice_destination,
                        notice_security_name,
                        resource_handle,
                        status);
        if (BAD_STATUS(status)) {
            return;
        }
    }

    /*
     * Implicitly register the ACK datatype for a notice message. 
     * If already registered, continue.
     */
    mos_api_datatype_register(&mos_datatype_ack,
                              (mos_msg_attr_convfn_t)mos_ack_convert,
                              status);
    if (STATUS_EQUAL(status, mos_s_datatype_already_reg)) {
        CLEAR_STATUS(status);
    } else if (BAD_STATUS(status)) {
        return;
    }

    /*
     * Release old notice destination
     */
    if (msg_attributes->notice_dest) {
        free(msg_attributes->notice_dest);
        msg_attributes->notice_dest = NULL;
    }

    /*
     * Release old notice security name
     */
    if (msg_attributes->notice_sec_name) {
        free(msg_attributes->notice_sec_name);
        msg_attributes->notice_sec_name = NULL;
    }

    /*
     * Set new notice options
     * Note that the new options may be no notice options and a NULL 
     * notice destination.
     */
    msg_attributes->notice_opts = notice_flags;
    if ((notice_destination != NULL) && (notice_destination[0] != '\0')) {
       msg_attributes->notice_dest =
                    (idl_char *)dce_strdup((char *)notice_destination);
    }
    if ((notice_security_name != NULL) && (notice_security_name[0] != '\0')) {
       msg_attributes->notice_sec_name =
                    (idl_char *)dce_strdup((char *)notice_security_name);
    }

    return;

} /* End of mos_msg_attr_notice_set() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ d a t a t y p e _ g e t 
 *
 *  OVERVIEW:
 *  Return application-specific data type from message 
 *  attributes structure.
 *
 *  INPUTS:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *  OUTPUTS:
 *  datatype_id:
 *                 Application-specific data type represented as a UUID.
 *                 This data type must be registered via
 *                 mos_api_datatype_register().
 *
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        None
 */
void
mos_msg_attr_datatype_get(
                          mos_msg_attr_t           msg_attributes,
                          uuid_t *                 datatype_id,
                          error_status_t *         status
                         )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL){
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    *datatype_id = msg_attributes->datatype;

    return;

} /* End of mos_msg_attr_datatype_get() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ d a t a t y p e _ s e t 
 *
 *  OVERVIEW:
 *  Set application-specific data type from message 
 *  attributes structure.
 *
 *  INPUTS:
 *  datatype_id:
 *                 Application-specific data type represented as a UUID.
 *                 This data type must be registered via
 *                 mos_api_datatype_register().
 *
 *  INPUT/OUTPUT:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
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
mos_msg_attr_datatype_set(
                          uuid_t *                 datatype_id,
                          mos_msg_attr_t           msg_attributes,
                          error_status_t *         status
                         )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL) {
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    /*
     * If valid, set the datatype.
     */
    if (mos__is_datatype_valid(FALSE, datatype_id, status)) {
        msg_attributes->datatype = *datatype_id;
    }

    return;

} /* End of mos_msg_attr_datatype_set() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ p r o t e c t l v l _ g e t 
 *
 *  OVERVIEW:
 *  Return the end-to-end protection level of initial sender 
 *  from the message attribute structure.
 *  Applies to Message Queuing model only.
 *
 *  If a recipient must manually select another protection level
 *  in order to retrieve a message, obtain the message attributes
 *  on a message via "mos_msg_attr_peek()", obtain the sender's
 *  end-to-end protection level via "mos_msg_attr_protectlvl_get()"
 *  and retrieve the message again using the same protection level
 *  as a selection criteria.
 *
 *  INPUTS:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *  OUTPUTS:
 *  protect_level:
 *                  Specifies how much of the information in messages
 *                  exchanged by principals is encrypted.
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 * 
 *  RETURNS:        None
 */ 
void
mos_msg_attr_protectlvl_get(
                            mos_msg_attr_t           msg_attributes,
                            unsigned32 *             protect_level,
                            error_status_t *         status
                           )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL){
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    *protect_level = msg_attributes->protectlvl;

    return;

} /* End of mos_msg_attr_protectlvl_get() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ p r o t e c t l v l _ s e t 
 *
 *  OVERVIEW:
 *  Sets the end-to-end protection level of initial sender 
 *  from the message attribute structure.
 *  Applies to Message Queuing model only.
 *
 *  INPUTS:
 *  protect_level:
 *                    Specifies how much of the information in messages
 *                    exchanged by principals is encrypted.
 *
 *  INPUT/OUTPUT:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
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
mos_msg_attr_protectlvl_set(
                            unsigned32               protect_level,
                            mos_msg_attr_t           msg_attributes,
                            error_status_t *         status
                           )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL) {
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    /*
     * If valid, set message protection level 
     */
    if (mos__is_protectlvl_valid(protect_level, status)) {
        msg_attributes->protectlvl = protect_level;
    }

    return;

} /* End of mos_msg_attr_protectlvl_set() routine */
 

/*
 *  FUNCTION:
 *  m o s _ _ i s _ p r o t e c t l v l _ v a l i d
 *
 *  OVERVIEW:
 *  Validate message protection level.
 *
 *  INPUTS:
 *  protectlvl:
 *                  Specifies how much of the information in messages
 *                  exchanged by principals is encrypted.
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
mos__is_protectlvl_valid(
                         unsigned32                  protectlvl,
                         error_status_t *            status
                         )
{
    CLEAR_STATUS(status);

    switch (protectlvl) {
       case (mos_c_protect_level_default):
       case (mos_c_protect_level_none):
       case (mos_c_protect_level_auth):
       case (mos_c_protect_level_integ):
       case (mos_c_protect_level_privacy):
           return(TRUE);
       default:
           SET_STATUS(status, mos_s_msg_attr_protectlvl_inv);   
           return(FALSE);
    } /* End of switch on message protection levels */

} /* End of mos__is_protectlvl_valid() routine */


/*
 *  FUNCTION:
 *  m o s _ m s g _ a t t r _ s e c i d _ g e t
 *
 *  OVERVIEW:
 *  Return the security identity of initial sender of a message 
 *  from the message attribute structure.
 *
 *  Security identity is obtained on a per message basis. Only the
 *  trusted Q-mgr will place the initial sender's authentication
 *  information in the message attributes area of a message.
 *
 *  In the message queuing model, performing authorization is left up
 *  to the recipient. The DCE security API can not help here.
 *  In the message passing model, this API does not apply.
 *
 *  INPUTS:
 *  msg_attributes:
 *                  Opaque pointer to message attributes structure obtained
 *                  via mos_msg_attr_alloc() or mos_msg_attr_peek().
 *
 *  OUTPUTS:
 *  sec_id:
 *                 Specifies the sender's identity as privilege
 *                 attributes. The recipient may or may not perform
 *                 authorization.
 * 
 *  RETURNS:        None
 */
void
mos_msg_attr_secid_get(
                       mos_msg_attr_t           msg_attributes,
                       sec_id_pa_t *            sec_id,
                       error_status_t *         status
                      )
{
    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate message attributes structure
     */
    if (msg_attributes == NULL){
        SET_STATUS(status, mos_s_msg_attr_inv);  
        return;
    } 
    
    *sec_id = msg_attributes->sender;

    return;

} /* End of mos_msg_attr_secid_get() routine */
