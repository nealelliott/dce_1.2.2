/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 *  m o s _ a p i . c
 *
 *  Client interfaces for API control to support the 
 *  Message Oriented Service Application Programming Interface(MOS-API).
 * ______________________________________________________________________
 *
 * $Log: mos_api.c,v $
 * Revision 1.6  1996/11/20  14:47:02  salamone
 * Fix header file declarations
 *
 * Revision 1.5  96/11/12  15:08:26  15:08:26  salamone (Julie Salamone)
 * Validate conversion function
 * 
 * Revision 1.4  96/10/29  15:36:42  15:36:42  salamone (Julie Salamone)
 * Add function descriptions
 * 
 * Revision 1.3  96/10/16  16:07:03  16:07:03  salamone (Julie Salamone)
 * Added datatype error codes
 * 
 * Revision 1.2  96/10/15  16:15:51  16:15:51  salamone (Julie Salamone)
 * Added mos_msg_peek()/mos_msg_delete() support
 * 
 * Revision 1.1  96/10/14  16:50:38  16:50:38  salamone (Julie Salamone)
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
 * VARIABLES
 */
mos_datatype_info_t *datatype_list_head = NULL; /* head item in datatype list */
mos_datatype_info_t *datatype_list_tail = NULL; /* tail item in datatype list */
mos_datatype_info_t null_datatype_item = { 0 }; /* NULL datatype item */

/*
 * INTERNAL PROTOTYPES
 */
void    mos__datatype_info_append(mos_datatype_info_t *);
boolean mos__is_convfn_valid(mos_msg_attr_convfn_t, error_status_t *);


/*
 *  FUNCTION:
 *  m o s _ d o n e
 *
 *  OVERVIEW:
 *  Releases all resources used by a MOS application.
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
mos_done(
         error_status_t *       status
        )
{
   CLEAR_STATUS(status);

   /* 
    * Release all server and queue resources used 
    */
   while (rsrc_list_head) { 
       mos__rsrc_info_remove(rsrc_list_head,
                             status);
       if (BAD_STATUS(status)) {
           break;
       }
   }

   /* 
    * Release list of registered datatypes 
    */
   while (datatype_list_head) { 
       mos_api_datatype_unregister(&datatype_list_head->datatype_id,
                                   status);
       if (BAD_STATUS(status)) {
           break;
       }
   }

   /* XXX Release list of group resources used */

   return;

} /* End of mos_done() routine */



/*
 *  FUNCTION:
 *  m o s _ a p i _ d a t a t y p e _ r e g i s t e r
 *
 *  OVERVIEW:
 *  Register one application-specific data type and its associated 
 *  conversion function with MOS to encode/decode the data.
 * 
 *  Marshalling is achieved on a per message basis. If an application
 *  requires multiple data types for messages, each data type and
 *  its conversion function must be registered before performing any
 *  send and/or retrieve operations. An application-specific data type
 *  on a message should be set before a send operation. On the
 *  retrieving end, the data type can be obtained.
 *
 *  Note that the registered data type and its conversion function is
 *  maintained by MOS in order to properly encode/decode messages
 *  sent/retrieved with a particular data type message attribute. MOS
 *  provides no conversion for the default data type, a NIL UUID, which
 *  refers to an array of bytes.
 *
 *  INPUTS:
 *  datatype_id:
 *                  Application-specific data type represented as a UUID.
 *
 *  conv_fn:
 *                  Conversion function to encode/decode a message with
 *                  the specified data type.
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
mos_api_datatype_register(
                          uuid_t *                      datatype_id,
                          mos_msg_attr_convfn_t         conv_fn,
                          error_status_t *              status
                         )

{
    mos_datatype_info_t *        datatype_item_p;

    /*
     * Initialize output parameters
     */
    CLEAR_STATUS(status);

    /*
     * Validate the datatype to be registered
     */
    if (! mos__is_datatype_valid(TRUE, datatype_id, status)) {
        return;
    }

    /*
     * Validate the conversion function to be registered
     */
    if (! mos__is_convfn_valid(conv_fn, status)) {
        return;
    }

    /*
     * Allocate space for a datatype
     */
    datatype_item_p = 
                (mos_datatype_info_t *)malloc(sizeof(mos_datatype_info_t));
    if (datatype_item_p == NULL) {
       SET_STATUS(status, mos_s_no_memory);    
       return;
    }

    /*
     * Initialize and set datatype info
     */
    *datatype_item_p = null_datatype_item;   
    datatype_item_p->datatype_id = *datatype_id;
    datatype_item_p->conv_fn = conv_fn;
    
    /*
     * Add new datatype to list of registered datatypes 
     */
    mos__datatype_info_append(datatype_item_p);
    return;

} /* End of mos_api_datatype_register() routine */


/*
 *  FUNCTION:
 *  m o s _ a p i _ d a t a t y p e _ u n r e g i s t e r
 *
 *  OVERVIEW:
 *  Unregister one application-specific data type and its associated 
 *  conversion function with MOS.
 * 
 *  INPUTS:
 *  datatype_id:
 *                  Application-specific data type represented as a UUID.
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
mos_api_datatype_unregister(
                            uuid_t *                datatype_id,
                            error_status_t *        status
                           )
{
   mos_datatype_info_t *     datatype_item_p;
   mos_datatype_info_t *     prev_item_p;

   /*
    * Initialize output
    */
   CLEAR_STATUS(status);

   /*
    * Validate the datatype to be removed
    */
   if (! mos__is_uuid_valid(datatype_id, status)) {
       return;
   }

   /*
    * Obtain item from the list
    */
   prev_item_p = NULL;
   for (datatype_item_p = datatype_list_head;
        datatype_item_p;
        datatype_item_p = datatype_item_p->next) {

     if (uuid_equal(&datatype_item_p->datatype_id, datatype_id, status)) {
        break;
     }
     prev_item_p = datatype_item_p;    /* Save previous datatype item */

   } /* End of loop thru all datatypes */

   if (datatype_item_p == NULL) {
       SET_STATUS(status, mos_s_no_datatype_reg);  
       return;
   }

   /*
    * Remove item from list
    */
   if (datatype_item_p == datatype_list_head)
       datatype_list_head = datatype_item_p->next;
   if (prev_item_p)
       prev_item_p->next = datatype_item_p->next;
   if (datatype_item_p == datatype_list_tail)
       datatype_list_tail = prev_item_p;

   /*
    * Release memory for the datatype
    */
   free((void *)datatype_item_p);
   return;

} /* End of mos_api_datatype_unregister() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ d a t a t y p e _ v a l i d
 *
 *  OVERVIEW:
 *  Validate a datatype.
 *
 *  INPUTS:
 *  add_it:
 *                  If TRUE, determine that datatype is unique among 
 *                  all registered datatypes.
 *                  If FALSE, determine that datatype exists.
 *
 *  datatype_id:
 *                  Application-specific data type represented as a UUID.
 *
 *  OUTPUTS:
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:
 *                  TRUE  indicates validation was successful.
 *                  FALSE indicates validation was not successful.
 */
boolean
mos__is_datatype_valid(
                       boolean            add_it,
                       uuid_t *           datatype_id,
                       error_status_t *   status
                      )
{
   mos_datatype_info_t *        datatype_info_p;

   /*
    * Initialize output parameters
    */
   CLEAR_STATUS(status);

   /*
    * Determine if datatype is NIL which is reserved for an array of bytes
    */
   if (! mos__is_uuid_valid(datatype_id, status)) {
       return(FALSE);
   }

   /*
    * Determine if datatype exists
    */
   if (add_it) {
       /*
        * If caller wants to add a new datatype but the datatype is 
        * already registered with API, then error.
        */
       if (mos__is_datatype_found(datatype_id, &datatype_info_p)) {
           SET_STATUS(status, mos_s_datatype_already_reg);   
           return(FALSE);
       }
   } else {
       /*
        * If caller wants to find/remove an existing datatype but the 
        * datatype is not found, then error.
        */
       if (! mos__is_datatype_found(datatype_id, &datatype_info_p)) {
           SET_STATUS(status, mos_s_no_datatype_reg);    
           return(FALSE);
       }
   }

   return(TRUE);

} /* End of mos__is_datatype_valid() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ u u i d _ v a l i d
 *
 *  OVERVIEW:
 *  Validate a generic UUID.
 *
 *  INPUTS:
 *  id:
 *                  A pointer to a UUID.
 *
 *  OUTPUTS:
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:
 *                  TRUE  indicates validation was successful.
 *                  FALSE indicates validation was not successful.
 */
boolean
mos__is_uuid_valid(
                   uuid_t *           id,
                   error_status_t *   status
                  )
{
   /*
    * Initialize output parameters
    */
   CLEAR_STATUS(status);

   /*
    * Good UUID?
    */
   if (id == NULL) {
       SET_STATUS(status, mos_s_no_uuid);    
       return(FALSE);
   }

   /*
    * Determine if UUID is NIL 
    */
   if (uuid_is_nil(id, status)) {
       SET_STATUS(status, mos_s_nil_uuid);  
       return(FALSE);
   }

   return(TRUE);

} /* End of mos__is_uuid_valid() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ c o n v f n _ v a l i d
 *
 *  OVERVIEW:
 *  Validate the specified conversion function.
 *
 *  INPUTS:
 *  conv_fn:
 *                  Conversion function to encode/decode a message with
 *                  the specified data type.
 *
 *  OUTPUTS:
 *  status:
 *                  A pointer to the completion status. On successful
 *                  completion, the routine returns error_status_ok.
 *                  Otherwise, it returns an error.
 *
 *  RETURNS:        TRUE  indicates conversion function is valid.
 *                  FALSE indicates conversion function is not valid.
 */
boolean
mos__is_convfn_valid(
                     mos_msg_attr_convfn_t         conv_fn,
                     error_status_t *              status
                    )
{
   mos_datatype_info_t *     datatype_item_p;

   if (conv_fn == NULL) {
      SET_STATUS(status, mos_s_no_convfn);
      return(FALSE);
   }

   /*
    * Determine if the specified conversion function has already been 
    * registered from the list of registered datatypes.
    */
   for (datatype_item_p = datatype_list_head;
        datatype_item_p;
        datatype_item_p = datatype_item_p->next) {

     if (conv_fn == datatype_item_p->conv_fn) {
         SET_STATUS(status, mos_s_convfn_already_reg);
         return(FALSE);
     }
   } /* End of loop thru all datatypes */

   return(TRUE);

} /* End of mos__is_convfn_valid() routine */


/*
 *  FUNCTION:
 *  m o s _ _ i s _ d a t a t y p e _ f o u n d
 *
 *  OVERVIEW:
 *  Find a datatype from the list of registered datatypes.
 *  If so, return the datatype handle. Otherwise, return a NULL handle.
 *
 *  INPUTS:
 *  datatype_id:
 *                  Application-specific data type represented as a UUID.
 *
 *  OUTPUTS:
 *  datatype_info:
 *                  A pointer to a registered datatype information structure.
 *
 *
 *  RETURNS:        TRUE  indicates datatype was found.
 *                  FALSE indicates datatype was not found.
 */
boolean
mos__is_datatype_found(
                       uuid_t *                   datatype_id,
                       mos_datatype_info_t **     datatype_info
                      )
{
   mos_datatype_info_t *     datatype_item_p;
   error_status_t            status;

   for (datatype_item_p = datatype_list_head;
        datatype_item_p;
        datatype_item_p = datatype_item_p->next) {

     if (uuid_equal(&datatype_item_p->datatype_id, datatype_id, &status)) {
         *datatype_info = datatype_item_p;
         return(TRUE);
     }
   } /* End of loop thru all datatypes */

   *datatype_info = NULL;
   return(FALSE);

} /* End of mos__is_datatype_found() routine */


/*
 *  FUNCTION:
 *  m o s _ _ d a t a t y p e _ i n f o _ a p p e n d
 *
 *  OVERVIEW:
 *  Add a new datatype at the tail of the list of registered datatypes.
 *
 *  INPUTS:
 *  datatype_item_p:
 *                  A pointer to a new datatype information structure.
 *
 *  RETURNS:        None
 */
void
mos__datatype_info_append(
                          mos_datatype_info_t *   datatype_item_p
                         )
{
   /*
    * Append to list
    */
   if (datatype_list_head == NULL) {
       datatype_list_head = datatype_list_tail = datatype_item_p;
   } else {
       datatype_list_tail->next = datatype_item_p;
       datatype_list_tail = datatype_item_p;
   }

} /* End of mos__datatype_info_append() routine */
