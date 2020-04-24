/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * MODULE:       telcom_oneq_mq.c
 * 
 * DESCRIPTION:  Illustrates using MOS-API calls -
 *               This program will:
 *                   -Create or attach one queue by name
 *                   -Set messsage attributes 
 *                   -Provide data encryption for authenticated communications
 *                   -Provide data marshalling
 *                   -Provide delivery notification
 *                   -Enqueue five "charge" messages
 *                   -Enqueue five "mwi" messages
 *                   -Dequeue all "charge" messages by selection filter
 *                   -Dequeue all "mwi" messages by selection filter
 *                   -Dequeue all ACKs by selection filter
 *                   -Exit MOS
 *
 * ADDED NOTE:
 *               Different datatypes are enqueued to one queue.
 *               The messages are then dequeued based on a selection 
 *               filter of message datatypes or message types.
 * ______________________________________________________________________
 *
 * $Log: telcom_oneq_mq.c,v $
 * Revision 1.1  1996/11/20  17:13:56  salamone
 * Initial version
 *
 * $EndLog$
 * ______________________________________________________________________
 */
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dce/dce.h>
#include <dce/utc.h>

#include <dcemosmsg.h>
#include <dcemosmac.h>
#include <dcemossvc.h>
#include <telcom_convert.h>
#include "../api/mosif.h"

/*
 * DECLARATIONS
 */
#define GOOD_STATUS(stp)      ((stp)==NULL || (*stp) == error_status_ok)

/*
 * INTERNAL VARIABLES
 */
static unsigned_char_t  *telcom_que_name = (unsigned_char_t *)"telcom";
static unsigned_char_t  *sec_group = (unsigned_char_t *)"test_queues";

/*
 * Phone number list
 */
static char *phoneList[] = {
                          "111-1111",
                          "222-2222",
                          "333-3333",
                          "444-4444",
                          "555-5555",
                          };
/* 
 * Datatype for charge messeages in charge_base.idl:
 *    fd1c7fd6-3975-11d0-94b7-0800095fae3a 
 */
static uuid_t charge_id = {
                            0xfd1c7fd6, 
                            0x3975, 
                            0x11d0, 
                            0x94, 
                            0xb7, 
                            {0x08,0x00,0x09,0x5f,0xae,0x3a} 
                          };
/* 
 * Datatype for mwi messeages in mwi_base.idl:
 *   7a0cd0fa-3975-11d0-9ef4-0800095fae3a 
 */
static uuid_t mwi_id = {
                         0x7a0cd0fa, 
                         0x3975, 
                         0x11d0, 
                         0x9e, 
                         0xf4, 
                         {0x08,0x00,0x09,0x5f,0xae,0x3a} 
                        };

/*
 * Resource handles to the Q-mgr and telcom queues
 */
mos_handle_t     dflt_qmgrh;
mos_handle_t     telcom_qh;

/*
 * Message attributes for charge and mwi messages
 */
mos_msg_attr_t   charge_msg_attr;
mos_msg_attr_t   mwi_msg_attr;

/*
 * INTERNAL PROTOTYPES
 */
void telcom__register_datatypes(error_status_t *);
void telcom__find_resources(error_status_t *);
void telcom__set_msg_attributes(error_status_t *);
void telcom__enq_charge_msgs(error_status_t *);
void telcom__enq_mwi_msgs(error_status_t *);
void telcom__set_charge_msg_attr(error_status_t *);
void telcom__set_mwi_msg_attr(error_status_t *);
void telcom__deq_acks(error_status_t *);
void telcom__deq_charge_msgs(error_status_t *);
void telcom__deq_mwi_msgs(error_status_t *);
void telcom__done(error_status_t *);


/*
 * main driver
 */
void
main()
{
   error_status_t   status;
 
   printf("\nDCE-MOS telcom example started...\n");

   /*
    * Register all application-specific datatypes with the MOS
    */
   telcom__register_datatypes(&status);

   /*
    * Find all resources required for the application
    */
   if (GOOD_STATUS(&status)) {
       telcom__find_resources(&status);
   }

   /*
    * Allocate and set message attributes associated with messages 
    */
   if (GOOD_STATUS(&status)) {
       telcom__set_msg_attributes(&status);
   }

   /*
    * Enqueue all charge messages to queue
    */
   if (GOOD_STATUS(&status)) {
       telcom__enq_charge_msgs(&status);
   }

   /*
    * Enqueue all mwi messages to queue
    */
   if (GOOD_STATUS(&status)) {
       telcom__enq_mwi_msgs(&status);
   }

   /*
    * Dequeue all charge messages from queue
    */
   if (GOOD_STATUS(&status)) {
       telcom__deq_charge_msgs(&status);
   }

   /*
    * Dequeue all mwi messages from queue
    */
   if (GOOD_STATUS(&status)) {
       telcom__deq_mwi_msgs(&status);
   }

   /*
    * Dequeue ACKs/NAKs on "charge" messages.
    */
   if (GOOD_STATUS(&status)) {
       telcom__deq_acks(&status);
   }

   /* 
    * Exit from DCE-MOS 
    */
   telcom__done(&status);

   printf("\nDCE-MOS telcom example complete...\n");

} /* End of main() */


void
telcom__register_datatypes(error_status_t *      status)
{
   /*
    * Register "charge" messages to be marshalled.
    */
   mos_api_datatype_register(&charge_id, 
                             (mos_msg_attr_convfn_t)charge_conversion_func, 
                             status);

   /*
    * Register "mwi" messages to be marshalled.
    */
   if (GOOD_STATUS(status)) {
      mos_api_datatype_register(&mwi_id,
                                (mos_msg_attr_convfn_t)mwi_conversion_func,
                                status);
   }

} /* End of telcom__register_datatypes() routine */


void
telcom__find_resources(error_status_t *      status)
{
   mos_rsrc_name_t         dflt_qmgr_name;

   mos_rsrc_dflt_quemgr_get(sec_group,
                            dflt_qmgr_name,
                            &dflt_qmgrh,
                            status);

   if (GOOD_STATUS(status)) {
       mos_que_mgmt_create(dflt_qmgrh,     
                           telcom_que_name,  
                           (void *)NULL,     /* Use default queue attributes */
                           &telcom_qh,
                           status);
       if (*status == mos_queue_already_exists) {
           mos_rsrc_attach(telcom_que_name,
                           sec_group,        /* security group */
                           &telcom_qh,
                           status);
      }
   }

} /* End of telcom__find_resources() */


void
telcom__set_msg_attributes(error_status_t *      status)
{

   /*
    * Allocate and set message attributes for "charge" messages
    */
   telcom__set_charge_msg_attr(status);

   /*
    * Allocate and set message attributes for "mwi" messages
    */
   if (GOOD_STATUS(status)) {
       telcom__set_mwi_msg_attr(status);
   }

} /* End of telcom__set_msg_attributes() */



void
telcom__set_charge_msg_attr(error_status_t *      status)
{
   unsigned32 notice_op = 0;

   /*
    * Allocate message attributes for "charge" messages
    */
   mos_msg_attr_alloc(&charge_msg_attr,
                      status);

   /*
    * Set "charge" messages to be sent.
    */
   if (GOOD_STATUS(status)) {
       mos_msg_attr_datatype_set(&charge_id,
                                 charge_msg_attr,
                                 status);
   }

   /*
    * Set sender's End-to-End protection level to 
    * "privacy" for "charge" messages.
    */
   if (GOOD_STATUS(status)) {
      mos_msg_attr_protectlvl_set(mos_c_protect_level_privacy,
                                  charge_msg_attr,
                                  status);
   }

   /*
    * Ensure persistence on "charge" messages
    */
   if (GOOD_STATUS(status)) {
       mos_msg_attr_persistence_set(mos_c_msg_attr_persistent,
                                    charge_msg_attr,
                                    status);
   }

   /*
    * Set notice options and destination of ACK/NAKs on "charge" messages.
    */
   if (GOOD_STATUS(status)) {
      notice_op = mos_c_msg_attr_notice_enqueue|mos_c_msg_attr_notice_dequeue;
      mos_msg_attr_notice_set(notice_op,
                              telcom_que_name, 
                              sec_group,
                              charge_msg_attr,
                              (mos_handle_t *)NULL,  /* Use hdl from create */
                              status);
   }

} /* End of telcom__set_charge_msg_attr() */


void
telcom__set_mwi_msg_attr(error_status_t *      status)
{
   utc_t                        generic_time;
   char                         gmTime[UTC_MAX_STR_LEN];

   /*
    * Allocate message attributes for "mwi" messages
    */
   mos_msg_attr_alloc(&mwi_msg_attr,
                      status);

   /*
    * Set "mwi" messages to be sent.
    */
   if (GOOD_STATUS(status)) {
      mos_msg_attr_datatype_set(&mwi_id,
                                mwi_msg_attr,
                                status);
   }

   /*
    * Set sender's End-to-End protection level to 
    * "integrity" for "mwi" messages.
    */
   if (GOOD_STATUS(status)) {
      mos_msg_attr_protectlvl_set(mos_c_protect_level_integ,
                                  mwi_msg_attr,
                                  status);
   }

   /*
    * Set expiration date on "mwi" messages
    */
   if (GOOD_STATUS(status)) {
      strcpy((char *)gmTime, "1999-10-17");
      utc_mkasctime(&generic_time, gmTime);
      mos_msg_attr_ttl_set(&generic_time,
                           mwi_msg_attr,
                           status);
   }

} /* End of telcom__set_mwi_msg_attr() */


void
telcom__enq_charge_msgs(error_status_t *      status)
{
   int                         i, j;
   uuid_t                      msg_id;
   unsigned_char_t *           msg_id_string;
   telcom_datatype_charge_t    charge_msg_buf;
   error_status_t              tmp_status;

   /*
    * Enqueue five charge messages 
    */
   for (i = 0; i < 5; i++) {

      /*
       * Build charge message
       */
      charge_msg_buf.calling_party = (idl_char *)strdup("999-9999");  
      charge_msg_buf.called_party = (idl_char *)strdup(phoneList[i]);
      charge_msg_buf.duration = 1800;  /* 30 minutes */
      charge_msg_buf.cost = 10;        /* 10 dollars */
      utc_gettime(&charge_msg_buf.timestamp);

      /*
       * Send charge message
       */
      mos_msg_enqueue(telcom_qh,
                      mos_c_enqmode_async,
                      charge_msg_attr,
                      &charge_msg_buf,
                      &msg_id,
                      status);

      if GOOD_STATUS(status) {
         uuid_to_string(&msg_id, &msg_id_string, &tmp_status);
         printf("Sent charge msg %s for calling party %s to called party %s\n", 
                       msg_id_string,
                       charge_msg_buf.calling_party,
                       charge_msg_buf.called_party);
         free(charge_msg_buf.calling_party);
         free(charge_msg_buf.called_party);
         rpc_string_free(&msg_id_string, &tmp_status);
      } else {
         printf("Error sending msg: %d:%s\n", *status,
                                              charge_msg_buf.called_party);
         free(charge_msg_buf.calling_party);
         free(charge_msg_buf.called_party);
         break;
      }

   } /* End of loop thru all messages */

} /* End of telcom__enq_charge_msgs() */


void
telcom__enq_mwi_msgs(error_status_t *      status)
{
   int                         i, j;
   uuid_t                      msg_id;
   unsigned_char_t *           msg_id_string;
   telcom_datatype_mwi_t       mwi_msg_buf;
   error_status_t              tmp_status;

   /*
    * Enqueue five mwi messages 
    */
   for (i = 0; i < 5; i++) {

      /*
       * Build mwi message
       */
      mwi_msg_buf.phone_number = (idl_char *)strdup(phoneList[i]);
      mwi_msg_buf.mwi_status = telcom_c_mwi_on;

      /*
       * Send mwi message
       */
      mos_msg_enqueue(telcom_qh,
                      mos_c_enqmode_async,
                      mwi_msg_attr,
                      &mwi_msg_buf,
                      &msg_id,
                      status);

      if GOOD_STATUS(status) {
         uuid_to_string(&msg_id, &msg_id_string, &tmp_status);
         printf("Sent MWI msg %s with indicator [%d] for phone number %s\n", 
                       msg_id_string,
                       mwi_msg_buf.mwi_status,
                       mwi_msg_buf.phone_number);
         free(mwi_msg_buf.phone_number);
         rpc_string_free(&msg_id_string, &tmp_status);
      } else {
         printf("Error sending msg: %d:%s\n", *status, 
                                              mwi_msg_buf.phone_number);
         free(mwi_msg_buf.phone_number);
         break;
      }
       
   } /* End of loop thru all messages */

} /* End of telcom__enq_mwi_msgs() */


void
telcom__deq_acks(error_status_t *      status)
{
   unsigned_char_t *      msg_id_string;
   mos_datatype_ack_t     ack_msg_buf;
   mos_msg_buf_t          out_msg_buf;  /* Allocated by runtime */
   mos_msg_selfilter_t    ack_sel_filter;
   unsigned32             protectlvl_value = mos_c_protect_level_privacy;
   unsigned32             msg_type = mos_c_msg_attr_msgtype_notice;
   error_status_t         tmp_status;

   /*
    * Allocate selection filter 
    */
   mos_msg_selfilter_alloc(&ack_sel_filter, 
                           status);  
   /*
    * Use same protection level as sender of "charge" messages.
    */
   if (GOOD_STATUS(status)) {
       mos_msg_selmask_add(mos_c_mask_type_msgtype,
                           &msg_type,
                           mos_c_mask_op_equal,
                           ack_sel_filter,
                           status);
       mos_msg_selmask_add(mos_c_mask_type_protectlvl,
                           &protectlvl_value,
                           mos_c_mask_op_equal,
                           ack_sel_filter,
                           status);
   }

   while (GOOD_STATUS(status)) {

      /*
       * Retrieve ACK/NAK on "charge" messages.
       */
      mos_msg_dequeue(telcom_qh,
                      mos_c_deqmode_nowait,
                      ack_sel_filter,     /* selection criteria specified */ 
                      NULL,               /* message attributes not returned */
                      &out_msg_buf,
                      status);

      if (GOOD_STATUS(status)) {
          mos_msg_datatype_decode(&mos_datatype_ack,
                                  out_msg_buf,
                                  &ack_msg_buf,
                                  status);
          if (GOOD_STATUS(status)) {
             uuid_to_string(&ack_msg_buf.msg_id, &msg_id_string, &tmp_status);
             printf("\nRetrieved ACK [%d] for msg %s with status [%d]\n", 
                                 ack_msg_buf.notice_option,
                                 msg_id_string,
                                 ack_msg_buf.status);
             rpc_string_free(&msg_id_string, &tmp_status);
             mos_msg_buf_free(&out_msg_buf, &tmp_status);
          }
      } 
      else if (*status == mos_empty_queue) {
         printf("\nEmpty queue; No msgs to retrieve\n");
         *status = 0;
         break;
      } else if (*status == mos_item_not_found) {
         printf("\nNo more msgs to retrieve\n");
         *status = 0;
         break;
      } else {
         printf("\nError retrieving ACK msg: %d\n", *status);
      }

   } /* End of loop thru all messages */

   mos_msg_selfilter_free(&ack_sel_filter, &tmp_status);  /* Release filter */

} /* End of telcom__deq_acks() */


void
telcom__deq_charge_msgs(error_status_t *      status)
{
   unsigned_char_t *         msg_id_string;
   mos_msg_selfilter_t       sel_filter;
   unsigned32                protectlvl_value = mos_c_protect_level_privacy;
   error_status_t            tmp_status;
   telcom_datatype_charge_t  charge_msg_buf;
   mos_msg_buf_t             out_msg_buf;  /* Allocated by runtime */

   /*
    * Allocate selection filter 
    */
   mos_msg_selfilter_alloc(&sel_filter, 
                           status);  
   /*
    * Use same protection level as sender of "charge" messages.
    */
   if (GOOD_STATUS(status)) {
       mos_msg_selmask_add(mos_c_mask_type_datatype,
                           &charge_id,
                           mos_c_mask_op_equal,
                           sel_filter,
                           status);
       mos_msg_selmask_add(mos_c_mask_type_protectlvl,
                           &protectlvl_value,
                           mos_c_mask_op_equal,
                           sel_filter,
                           status);
   }

   while (GOOD_STATUS(status)) {

      mos_msg_dequeue(telcom_qh,
                      mos_c_deqmode_nowait,
                      sel_filter,     /* selection criteria specified */ 
                      NULL,           /* message attributes not returned */
                      &out_msg_buf,
                      status);

      if (GOOD_STATUS(status)) {
          mos_msg_datatype_decode(&charge_id,
                                  out_msg_buf,
                                  &charge_msg_buf,
                                  status);
          if (GOOD_STATUS(status)) {
             printf("\nDequeued charge msg for calling party %s"
                    " to called party %s\n",                     
                    charge_msg_buf.calling_party,
                    charge_msg_buf.called_party);
             mos_msg_buf_free(&out_msg_buf, &tmp_status);
             /* Release allocated parts of charge message */
             free(charge_msg_buf.calling_party);
             free(charge_msg_buf.called_party);
          }
      } 
      else if (*status == mos_empty_queue) {
         printf("\nEmpty queue; No msgs to retrieve\n");
         *status = 0;
         break;
      } else if (*status == mos_item_not_found) {
         printf("\nNo more msgs to retrieve\n");
         *status = 0;
         break;
      } else {
         printf("\nError retrieving charge msg: %d\n", *status);
      }

   } /* End of loop thru all messages */

   mos_msg_selfilter_free(&sel_filter, &tmp_status);  /* Release filter */

} /* End of telcom__deq_charge_msgs() */


void
telcom__deq_mwi_msgs(error_status_t *      status)
{
   unsigned_char_t *         msg_id_string;
   mos_msg_selfilter_t       sel_filter;
   unsigned32                protectlvl_value = mos_c_protect_level_integ;
   error_status_t            tmp_status;
   telcom_datatype_mwi_t     mwi_msg_buf;
   mos_msg_buf_t             out_msg_buf;  /* Allocated by runtime */

   /*
    * Allocate selection filter 
    */
   mos_msg_selfilter_alloc(&sel_filter, 
                           status);  
   /*
    * Use same protection level as sender of "mwi" messages.
    */
   if (GOOD_STATUS(status)) {
       mos_msg_selmask_add(mos_c_mask_type_datatype,
                           &mwi_id,
                           mos_c_mask_op_equal,
                           sel_filter,
                           status);
       mos_msg_selmask_add(mos_c_mask_type_protectlvl,
                           &protectlvl_value,
                           mos_c_mask_op_equal,
                           sel_filter,
                           status);
   }

   while (GOOD_STATUS(status)) {

      mos_msg_dequeue(telcom_qh,
                      mos_c_deqmode_nowait,
                      sel_filter,     /* selection criteria specified */ 
                      NULL,           /* message attributes not returned */
                      &out_msg_buf,
                      status);

      if (GOOD_STATUS(status)) {
          mos_msg_datatype_decode(&mwi_id,
                                  out_msg_buf,
                                  &mwi_msg_buf,
                                  status);
          if (GOOD_STATUS(status)) {
             printf("\nDequeued mwi msg: %s\n", 
                                 mwi_msg_buf.phone_number);
             mos_msg_buf_free(&out_msg_buf, &tmp_status);
             /* Release allocated parts of mwi message */
             free(mwi_msg_buf.phone_number);
          }
      } 
      else if (*status == mos_empty_queue) {
         printf("\nEmpty queue; No msgs to retrieve\n");
         *status = 0;
         break;
      } else if (*status == mos_item_not_found) {
         printf("\nNo more msgs to retrieve\n");
         *status = 0;
         break;
      } else {
         printf("\nError retrieving mwi msg: %d\n", *status);
      }

   } /* End of loop thru all messages */

   mos_msg_selfilter_free(&sel_filter, &tmp_status);  /* Release filter */

} /* End of telcom__deq_mwi_msgs() */


void
telcom__done(error_status_t *      status)
{

   /*
    * Free message attributes 
    */
   mos_msg_attr_free(&charge_msg_attr,
                     status);
   mos_msg_attr_free(&mwi_msg_attr,
                     status);

   /*
    * Delete telcom queue
    */
   mos_que_mgmt_delete(mos_c_delop_force,
                       &telcom_qh,
                       status);

   /*   
    * Exit MOS
    */
   mos_done(status);

} /* End of telcom__done() */
