/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * MODULE:       basic_all_mq.c
 * 
 * DESCRIPTION:  Illustrates most of the MOS-APIs.
 *
 * ADDED NOTE:
 *               The "mbox" queue must already exist.
 *               The sender uses the default protection level(encryption) for
 *               authenticated communciations with the Q-mgr.
 *               All application-specific messages are the same data type; 
 *                    an array of bytes 
 *               Same data type within a queue
 *               No data marshalling for an array of bytes.
 *               Default message attributes are used:
 *                   message type        = mos_c_msg_attr_msgtype_data
 *                   priority            = mos_c_msg_attr_dflt_priority
 *                   time-to-live        = mos_c_msg_attr_dflt_ttl
 *                   time-to-receive     = mos_c_msg_attr_dflt_ttr
 *                   persistence         = mos_c_msg_attr_volatile
 *                   notice options      = mos_c_msg_attr_notice_none
 *                   data type           = NIL-UUID for an array of bytes
 *                   reply-to queue name = NULL
 *                   sender's protection_level 
 *                                       = mos_c_protect_level_default
 * ______________________________________________________________________
 *
 * $Log: basic_all_mq.c,v $
 * Revision 1.2  1996/11/21  14:44:54  salamone
 * Move msg within same Qmgr now fixed
 *
 * Revision 1.1  96/11/20  16:58:29  16:58:29  salamone (Julie Salamone)
 * Initial version
 * 
 * $EndLog$
 * ______________________________________________________________________
 */

/*
 * INCLUDE FILES
 */
#include <stdio.h>
#include <string.h>

#include <dce/dce.h>
#include <dce/utc.h>
#include <dce/dce_msg.h>
#include <dce/dce_svc.h>
#include "../../api/mosif.h"
#include <dcemosmsg.h>
#include <dcemosmac.h>
#include <dcemossvc.h>

/*
 * DECLARATIONS
 */
#define GOOD_STATUS(stp)      ((stp)==NULL || (*stp) == error_status_ok)

/*
 * INTERNAL VARIABLES
 */
dce_svc_handle_t        mos_svc_handle;

/*
 * All messages are the same data type; an array of bytes
 */
static char *msgList[] = {
                          "First message",
                          "Second message",
                          "Third message",
                          "Fourth message",
                          "Fifth message",
                          "Sixth message",
                          "Seventh message",
                          "Eighth message",
                          "Ninth message",
                          "Tenth message"
                          };

void main(int argc, char **argv)
{
   int                          i;
   error_status_t               status;
   mos_handle_t                 mbox_qh;
   mos_handle_t                 notice_qh;
   mos_handle_t                 mbox_qh2;
   mos_handle_t                 new_qh;
   mos_handle_t                 reply_qh;
   mos_handle_t                 dflt_quemgr_h1;
   mos_handle_t                 dflt_quemgr_h;
   mos_datatype_bytearray_t     in_msg_buf;     /* Default data type */
   mos_msg_buf_t                out_msg_buf;    /* Allocated by runtime */
   uuid_t                       msg_id;
   mos_msg_attr_t               msg_attr;
   mos_que_attr_t               que_attr;
   mos_msg_selfilter_t          sel_filter;
   mos_rsrc_name_t              que_name;
   mos_rsrc_name_t              new_que_name;
   mos_rsrc_name_t              dflt_quemgr_name;
   sec_rgy_name_t               sec_name;
   unsigned32                   num_returned;
   unsigned32                   num_left;
   unsigned32                   space_avail = 20;
   mos_rsrc_name_t              que_name_list[20];
   uuid_t                       msg_id_list[20];
   unsigned char *              uuid_string;
   mos_que_attr_persistence_t   persistence;
   utc_t                        generic_time;
   char                         buf[100];
   char                         gmTime[UTC_MAX_STR_LEN];
   timespec_t                   tzero;
   reltimespec_t                t_ttl;
   mos_que_attr_annotation_t    annotation;
   unsigned32                   notice_opts;
   mos_rsrc_name_t              notice_dest;
   sec_rgy_name_t               notice_sec_name;
   uuid_t                       new_dt;
   uuid_t                       new_dt2;
   uuid_t                       new_dt3;
   mos_msg_attr_msgtype_t       msg_type;
   unsigned32                   protectlvl;
 
   if (argc == 1) {
      printf("Enter an existing queue name(cell-relative): ");
      scanf("%s", que_name);
      printf("Enter a security name: ");
      scanf("%s", sec_name);
      printf("Enter a queue name to create(simple): ");
      scanf("%s", new_que_name);
   } 
   if ((argc >= 2) && (argc <= 4)) {
      strcpy((char *)que_name, argv[1]);
   }
   if ((argc == 3) || (argc == 4)) {
      strcpy((char *)sec_name, argv[2]);
   }
   if (argc == 4) {
      strcpy((char *)new_que_name, argv[3]);
   }
   if (argc > 4) {
      printf("Usage: %s [queue name] [security group]\n", argv[0]);
      return;
   }

   mos_svc_handle = dce_svc_register(mos_svc_table, (idl_char *)"mos", &status);
   dce_svc_set_progname(argv[0], &status);
   dce_msg_define_msg_table(mos_svc_g_table,
        sizeof mos_svc_g_table / sizeof mos_svc_g_table[0], &status);

   printf("\nDCE-MOS basic example started...\n");

   mos_rsrc_dflt_quemgr_get(sec_name,
                            dflt_quemgr_name,
                            &dflt_quemgr_h1,
                            &status);
   if (GOOD_STATUS(&status)){
       printf("Default queue manager name: %s\n", dflt_quemgr_name);
   } 
   mos_rsrc_dflt_quemgr_set((unsigned_char_t *)"/.:/subsys/Q/q2",
                            sec_name,
                            &dflt_quemgr_h,
                            &status);
   if (GOOD_STATUS(&status)){
       mos_rsrc_dflt_quemgr_get(sec_name,
                                dflt_quemgr_name,
                                NULL,
                                &status);
       printf("Default queue manager name: %s\n", dflt_quemgr_name);
   } 
   if (! GOOD_STATUS(&status)){
       printf("Get dflt quemgr failed: %d\n", status);
       return;
   }
   mos_rsrc_protectlvl_get(dflt_quemgr_h,
                           &protectlvl,
                           &status);
   if (GOOD_STATUS(&status))
       printf("Org resource protection level: %d\n", protectlvl);
   /*
    * Qmgr on same host, don't need all that protection.
    */
   mos_rsrc_protectlvl_set(mos_c_protect_level_auth, 
                           dflt_quemgr_h,
                           &status);
   mos_rsrc_protectlvl_get(dflt_quemgr_h,
                           &protectlvl,
                           &status);
   if (GOOD_STATUS(&status))
       printf("New resource protection level: %d\n", protectlvl);

   uuid_create(&new_dt, &status);
   uuid_create(&new_dt2, &status);
   uuid_create(&new_dt3, &status);
   mos_api_datatype_register(&new_dt,
                             (mos_msg_attr_convfn_t)NULL,
                             &status);
   mos_api_datatype_register(&new_dt2,
                             (mos_msg_attr_convfn_t)NULL,
                             &status);
   mos_api_datatype_register(&new_dt3,
                             (mos_msg_attr_convfn_t)NULL,
                             &status);
   mos_api_datatype_unregister(&new_dt2,
                               &status);

   /*
    * Find all resources required for the application
    */
   mos_rsrc_attach(que_name,   
                   sec_name,  
                   &mbox_qh,
                   &status);

   if (! GOOD_STATUS(&status)){
      return;
   }
   /* 
    * Also, tested the following:
    * mos_que_mgmt_create(mbox_qh, 
    * mos_que_mgmt_create(dflt_quemgr_h, 
    */
   mos_que_mgmt_create((void *)NULL,
                       new_que_name,
                       (void *)NULL,
                       &new_qh,
                       &status);

   if (GOOD_STATUS(&status)) {
      mos_que_mgmt_quename_list(new_qh,
                                space_avail,
                                &num_returned,
                                que_name_list,
                                &num_left,
                                &status);
      printf("%d queues returned: %d left\n", num_returned, num_left);
      for(i = 0; i < num_returned; i++) {
          printf("%s\n", que_name_list[i]);
      }

      /*
       * Change and commit queue persistence
      mos_que_attr_alloc(&que_attr,
                         &status);
       */
      mos_que_attr_peek(new_qh,
                        &que_attr,
                        &status);
      if (GOOD_STATUS(&status)) {
         mos_que_attr_persistence_get(que_attr,
                                      &persistence,
                                      &status);
         printf("Initial queue persistence: %d\n", persistence);
         mos_que_attr_persistence_set(mos_c_que_attr_persistent, 
                                      que_attr,
                                      &status);
         mos_que_attr_annotation_set((unsigned_char_t *)"personal q", 
                                      que_attr,
                                      &status);
         mos_que_attr_alias_add((unsigned char *)NULL,
                                (unsigned char *)"juju5", 
                                que_attr,
                                &status);
         mos_que_attr_alias_add((unsigned char *)NULL,
                                (unsigned char *)"juju6", 
                                que_attr,
                                &status);
         mos_que_attr_alias_add((unsigned char *)NULL,
                                (unsigned char *)"juju7", 
                                que_attr,
                                &status);
         mos_que_attr_alias_add((unsigned char *)NULL,
                                (unsigned char *)"juju8", 
                                que_attr,
                                &status);
         mos_que_attr_alias_list(que_attr,
                                 space_avail,
                                 &num_returned,
                                 que_name_list,
                                 &num_left,
                                 &status);
         printf("%d aliases returned: %d left\n", num_returned, num_left);
         for(i = 0; i < num_returned; i++) {
             printf("%s\n", que_name_list[i]);
         }
         mos_que_attr_alias_remove((unsigned char *)NULL,
                                   (unsigned char *)"juju8", 
                                   que_attr,
                                   &status);
/*
         mos_que_attr_alias_reset(que_attr,
                                  &status);
*/
         mos_que_attr_alias_list(que_attr,
                                 space_avail,
                                 &num_returned,
                                 que_name_list,
                                 &num_left,
                                 &status);
         printf("%d aliases returned: %d left\n", num_returned, num_left);
         for(i = 0; i < num_returned; i++) {
             printf("%s\n", que_name_list[i]);
         }
        /*
         mos_que_attr_allowdeq_set(0,
                                   que_attr,
                                   &status);
         */

         /*
          * Show empty queue idle time
          */
         gmTime[0] = '\0';
         generic_time.char_array[0] = '\0';
         mos_que_attr_idletimeout_get(que_attr,
                                      &generic_time,
                                      &status);
         utc_ascreltime(gmTime,
                        UTC_MAX_STR_LEN,
                        &generic_time);
         printf("Empty queue idle time: %s\n", gmTime);
         /*
          * Construct a timestamp that represents 48 hours
          */
         t_ttl.tv_sec  = (3600 * 48);
         t_ttl.tv_nsec = 0;
         tzero.tv_sec  = 0;
         tzero.tv_nsec = 0;
         utc_mkbinreltime(&generic_time,
                          &t_ttl,
                          &tzero);
         /* mos_que_attr_idletimeout_set((utc_t *)NULL, test NULL time */
         mos_que_attr_idletimeout_set(&generic_time, 
                                      que_attr,
                                      &status);
         gmTime[0] = '\0';
         generic_time.char_array[0] = '\0';
         mos_que_attr_idletimeout_get(que_attr,
                                      &generic_time,
                                      &status);
         utc_ascreltime(gmTime,
                        UTC_MAX_STR_LEN,
                        &generic_time);
         printf("Empty queue idle time: %s\n", gmTime);

         if (GOOD_STATUS(&status)) {
             mos_que_attr_commit(new_qh,
                                 que_attr,
                                 &status);
             /* 
              * Also tested mos_que_attr_free((mos_que_attr_t *)NULL,
              */
             mos_que_attr_free(&que_attr, 
                               &status);
         }
         mos_que_attr_peek(new_qh,
                           &que_attr,
                           &status);
         mos_que_attr_persistence_get(que_attr,
                                      &persistence,
                                      &status);
         printf("New queue persistence: %d\n", persistence);
         mos_que_attr_annotation_get(que_attr,
                                     annotation,
                                     &status);
         printf("New queue annotation: %s\n", annotation);

      }
      mos_que_attr_free(&que_attr,
                        &status);
          
   }

   /*
    * Enqueue all messages to the specified queue
    */
   for (i = 0; (GOOD_STATUS(&status) && i < 10); i++) {

      in_msg_buf.size = (strlen(msgList[i]) + 1);
      in_msg_buf.data = (unsigned_char_t *)msgList[i];
      mos_msg_enqueue(mbox_qh,
                      mos_c_enqmode_async,
                      NULL,   /* Use default message attributes */
                      &in_msg_buf,
                      &msg_id,       
                      &status);

      if (GOOD_STATUS(&status)) { 
         printf("\nSent msg successfully: %s\n", in_msg_buf.data);
      } 

      mos_msg_enqueue(new_qh,
                      mos_c_enqmode_async,
                      NULL,   /* Use default message attributes */
                      &in_msg_buf,
                      &msg_id,       
                      &status);

      if (GOOD_STATUS(&status)) { 
         printf("\nSent msg successfully: %s\n", in_msg_buf.data);
      } else {
         printf("\nError sending msg: %d\n", status);
      }
   } /* End of loop thru all messages */


   /* test for non-queues
      mos_msg_enqueue(dflt_quemgr_h,
                      mos_c_enqmode_async,
                      NULL,   
                      &in_msg_buf,
                      &msg_id,       
                      &status);

      if (GOOD_STATUS(&status)) { 
         printf("\nSent msg successfully: %s\n", in_msg_buf.data);
      } else {
         printf("\nError sending msg to non-queue: %d\n", status);
      }
    */

   if (GOOD_STATUS(&status)) {
       mos_que_mgmt_msgid_list(new_qh,
                               (void *)NULL,
                               space_avail,
                               &num_returned,
                               msg_id_list,
                               &num_left,
                               &status);
       printf("%d msgs returned: %d left\n", num_returned, num_left);
       for(i = 0; i < num_returned; i++) {
           uuid_to_string(&msg_id_list[i], &uuid_string, &status);
           printf("%d Msg ID: %s\n", i, uuid_string);
       }
   }
   mos_msg_peek(new_qh,
                mos_c_deqmode_nowait,
                NULL,   	/* No selection criteria */
                &msg_attr,  
                &out_msg_buf,
                &status);
   if (GOOD_STATUS(&status)) {
      printf("\nRetrieved msg successfully: %s\n", out_msg_buf->data);
      mos_msg_attr_msgid_get(msg_attr,
                             &msg_id,
                             &status);
      uuid_to_string(&msg_id, &uuid_string, &status);
      printf("Peeked at Message ID: %s\n", uuid_string);
      mos_msg_attr_free(&msg_attr,
                        &status);
      mos_msg_buf_free(&out_msg_buf,
                       &status);
   }
   mos_msg_delete(new_qh,
                  &msg_id_list[5], 
                  &status);
   if (GOOD_STATUS(&status)) {
      uuid_to_string(&msg_id_list[5], &uuid_string, &status);
      printf("Deleted Message ID: %s\n", uuid_string);
   }
   if (GOOD_STATUS(&status)) {
       mos_que_mgmt_msgid_list(new_qh,
                               (void *)NULL,
                               space_avail,
                               &num_returned,
                               msg_id_list,
                               &num_left,
                               &status);
       printf("%d msgs returned: %d left\n", num_returned, num_left);
       for(i = 0; i < num_returned; i++) {
           uuid_to_string(&msg_id_list[i], &uuid_string, &status);
           printf("%d Msg ID: %s\n", i, uuid_string);
       }
   }

   /*
    * JAS Test selection filter
   msg_type = mos_c_msg_attr_msgtype_data;
   mos_msg_selmask_add(mos_c_mask_type_msgtype,
                       &msg_type,
                       mos_c_mask_op_equal,
                       sel_filter,
                       &status);
    */
   mos_msg_selfilter_alloc(&sel_filter,
                           &status);
   mos_msg_selmask_add(mos_c_mask_type_msgid,
                       &msg_id_list[5],
                       mos_c_mask_op_equal,
                       sel_filter,
                       &status);
   protectlvl = mos_c_protect_level_default;
   mos_msg_selmask_add(mos_c_mask_type_protectlvl,
                       &protectlvl,
                       mos_c_mask_op_equal, 
                       sel_filter,
                       &status);
   mos_msg_selmask_add(mos_c_mask_type_sender_prin,
                       "salamone",
                       mos_c_mask_op_equal,
                       sel_filter,
                       &status);
/*
   mos_msg_attr_peek(new_qh,
                     0,            
                     sel_filter,
                     &msg_attr,
                     &status);
*/
   mos_msg_dequeue(new_qh,
                   mos_c_deqmode_nowait,
                   sel_filter,   
                   &msg_attr,  
                   &out_msg_buf,
                   &status);
   if (GOOD_STATUS(&status)) {
      printf("\nRetrieved msg successfully: %s\n", out_msg_buf->data);
      mos_msg_attr_msgid_get(msg_attr,
                             &msg_id,
                             &status);
      uuid_to_string(&msg_id, &uuid_string, &status);
      printf("Dequeued Message ID via selection filter: %s\n", uuid_string);
      mos_msg_attr_free(&msg_attr,
                        &status);
      mos_msg_buf_free(&out_msg_buf,
                       &status);
   }
   mos_msg_selfilter_free(&sel_filter,
                          &status);

   mos_msg_move(new_qh,   
                mbox_qh,
                &msg_id_list[2],
                &status);
   if (GOOD_STATUS(&status)) {
      uuid_to_string(&msg_id_list[2], &uuid_string, &status);
      printf("Moved Message ID from new queue to jas: %s\n", uuid_string);
   }

   /*
    * Dequeue all messages from the specified queue
    */
   while (GOOD_STATUS(&status)) {

      mos_msg_dequeue(mbox_qh,
                      mos_c_deqmode_nowait,
                      NULL,   
                      &msg_attr,  
                      &out_msg_buf,
                      &status);

      if (GOOD_STATUS(&status)) {
         printf("\nRetrieved msg successfully: %s\n", out_msg_buf->data);
         mos_msg_attr_msgid_get(msg_attr,
                                &msg_id,
                                &status);
         uuid_to_string(&msg_id, &uuid_string, &status);
         printf("Message ID: %s\n", uuid_string);
         mos_msg_attr_free(&msg_attr,
                           &status);
         mos_msg_buf_free(&out_msg_buf,
                          &status);
      } else if (status == mos_empty_queue) {
         printf("\nEmpty queue; No msgs to retrieve\n");
         status = 0;
         break;
      } else if (status == mos_item_not_found) {
         printf("\nNo more msgs to retrieve\n"); 
         status = 0;
         break;
      } else {
         printf("\nError retrieving msg: %d\n", status);
      }

   } /* End of loop thru all messages in queue */
       
   if (GOOD_STATUS(&status)) {
       mos_msg_attr_peek(new_qh,
                         0,            /* no-wait deq */
                         (void *)NULL, /* No selection filter */
                         &msg_attr,
                         &status);
       /*
        * Show message ID
        */
       mos_msg_attr_msgid_get(msg_attr,
                              &msg_id,
                              &status);
       uuid_to_string(&msg_id, &uuid_string, &status);
       printf("Message ID: %s\n", uuid_string);

       /*
        * Set and get notice options
       mos_msg_attr_notice_set(mos_c_msg_attr_notice_none,
        */
       strcpy((char *)notice_dest, (char *)"jas");
       mos_msg_attr_notice_set(mos_c_msg_attr_notice_enqueue,
                               notice_dest,
                               notice_sec_name,
                               msg_attr,
                               &notice_qh,
                               &status);
       if (GOOD_STATUS(&status)) {
           mos_msg_attr_notice_get(msg_attr,
                                   &notice_opts,
                                   notice_dest,
                                   notice_sec_name,
                                   &status);
           printf("Notice options: %d\n", notice_opts);
           printf("Notice destination: %s\n", notice_dest);
           printf("Notice security name: %s\n", notice_sec_name);
       }

       /*
        * Show message enq time
        */
       gmTime[0] = '\0';
       mos_msg_attr_enqtime_get(msg_attr,
                                &generic_time,
                                &status);
       utc_ascgmtime(gmTime,
                     UTC_MAX_STR_LEN,
                     &generic_time);
       printf("Message enq time: %s\n", gmTime);

       /*
        * Show message expiration time
        */
       gmTime[0] = '\0';
       mos_msg_attr_ttl_get(msg_attr,
                            &generic_time,
                            &status);
       utc_ascgmtime(gmTime,
                     UTC_MAX_STR_LEN,
                     &generic_time);
       printf("Message expiration time: %s\n", gmTime);

       /*
        * Set and Get message expiration time
        */
       strcpy((char *)gmTime, "1996-10-17");
       utc_mkasctime(&generic_time, gmTime);
       mos_msg_attr_ttl_set(&generic_time,
                            msg_attr,
                            &status);

       gmTime[0] = '\0';
       mos_msg_attr_ttl_get(msg_attr,
                            &generic_time,
                            &status);
       utc_ascgmtime(gmTime,
                     UTC_MAX_STR_LEN,
                     &generic_time);
       printf("New Message expiration time: %s\n", gmTime);

       /*
        * Show message time-to-receive 
        */
       gmTime[0] = '\0';
       mos_msg_attr_ttr_get(msg_attr,
                            &generic_time,
                            &status);
       utc_ascgmtime(gmTime,
                     UTC_MAX_STR_LEN,
                     &generic_time);
       printf("Message ttr time: %s\n", gmTime);

       /*
        * Set and Get message ttr time
        */
       strcpy((char *)gmTime, "1996-12-25");
       utc_mkasctime(&generic_time, gmTime);
       mos_msg_attr_ttr_set(&generic_time,
                            msg_attr,
                            &status);

       gmTime[0] = '\0';
       mos_msg_attr_ttr_get(msg_attr,
                            &generic_time,
                            &status);
       utc_ascgmtime(gmTime,
                     UTC_MAX_STR_LEN,
                     &generic_time);
       printf("New Message ttr time: %s\n", gmTime);

       /*
        * Free msg attr
        */
       mos_msg_attr_free(&msg_attr,
                         &status);
   }
   if (GOOD_STATUS(&status)) {
       /* mos_que_mgmt_move(NULL,    * rename queue */
       mos_que_mgmt_move(new_qh,
                         (unsigned_char_t *)"helloq",
                         mos_c_que_mv_none,
                         &new_qh,
                         &status);

       if (GOOD_STATUS(&status)) {
          mos_que_attr_peek(new_qh,
                            &que_attr,
                            &status);
          if (GOOD_STATUS(&status)) 
             mos_que_attr_alias_list(que_attr,
                                     1,
                                     &num_returned,
                                     que_name_list,
                                     &num_left,
                                     &status);
          if (GOOD_STATUS(&status)) 
             printf("Queue renamed successfully: %s\n", que_name_list[0]);
          mos_que_attr_free(&que_attr,
                            &status);
       } else {
          printf("Queue rename error: %d\n", status);
       }
       mos_que_mgmt_delete(mos_c_delop_force,
                           &new_qh, 
                           &status);

       mos_que_mgmt_quename_list(mbox_qh,
                                 space_avail,
                                 &num_returned,
                                 que_name_list,
                                 &num_left,
                                 &status);
       printf("%d queues returned: %d left\n", num_returned, num_left);
       for(i = 0; i < num_returned; i++) {
           printf("%s\n", que_name_list[i]);
       }
   }

   /* 
    * Also tested detaching a qmgr
   mos_rsrc_detach(&dflt_quemgr_h, &status);
   if (GOOD_STATUS(&status)) {
     printf("\nDetached %s\n", dflt_quemgr_name);
   }
    */

   /* 
    * Also tested for NULL mos_rsrc_detach((mos_handle_t *)NULL, &status);
    */
   mos_rsrc_detach(&mbox_qh, &status);
   if (GOOD_STATUS(&status)) {
     printf("\nDetached %s\n", que_name);
   } else {
     printf("\nDetached error %d\n", status);
   }
   mos_done(&status);

   printf("\nDCE-MOS basic example complete...\n");

} /* End of main() */
