/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * MODULE:       basic_mq.c
 * 
 * DESCRIPTION:  Illustrates use of the four basic DCE MOS calls -
 *               attach, enqueue, dequeue, done
 *               This program will: 
 *                   -Attach one queue by name
 *                   -Send ten messages to the attached queue 
 *                   -Retrieve the messages from the attached queue 
 *                   -Exit MOS
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
 * $Log: basic_mq.c,v $
 * Revision 1.1  1996/11/20  16:57:21  salamone
 * Initial version
 *
 * $EndLog$
 * ______________________________________________________________________
 */
#include <stdio.h>
#include <string.h>
#include <dce/dce.h>
#include <dcemosmsg.h>
#include <dcemosmac.h>
#include <dcemossvc.h>
#include "../../api/mosif.h"

/*
 * DECLARATIONS
 */
#define GOOD_STATUS(stp)      ((stp)==NULL || (*stp) == error_status_ok)

/*
 * INTERNAL VARIABLES
 */
static unsigned_char_t  *que_name = (unsigned_char_t *)"/.:/subsys/Q/q2/jas";
static unsigned_char_t  *sec_name = (unsigned_char_t *)"test_queues";

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

void main()
{
   int                          i;
   error_status_t               status;
   mos_handle_t                 mbox_qh;
   mos_datatype_bytearray_t     in_msg_buf;  /* Default datatype */
   mos_msg_buf_t                out_msg_buf; /* Allocated by runtime */
   uuid_t                       msg_id;
 
   printf("\nDCE-MOS basic example started...\n");

   /*
    * Find all resources required for the application
    */
   mos_rsrc_attach(que_name,
                   sec_name,   /* security group */
                   &mbox_qh,
                   &status);

   /*
    * Enqueue all messages to the specified queue
    */
   for (i = 0; (GOOD_STATUS(&status) && i < 10); i++) {

      in_msg_buf.size = (strlen(msgList[i]) + 1);
      in_msg_buf.data = (unsigned_char_t *)msgList[i];
      mos_msg_enqueue(mbox_qh,
                      mos_c_enqmode_async,
                      NULL,          /* Use default message attributes */
                      &in_msg_buf,
                      &msg_id,       
                      &status);
      if (GOOD_STATUS(&status)) { 
         printf("\nSent msg successfully: %s\n", in_msg_buf.data);
      } else {
         printf("\nError sending msg: %d\n", status);
      }
   } /* End of loop thru all messages */

   /*
    * Dequeue all messages from the specified queue
    */
   while (GOOD_STATUS(&status)) {

      mos_msg_dequeue(mbox_qh,
                      mos_c_deqmode_nowait,
                      NULL,           /* No selection criteria */
                      NULL,           /* Message attributes not returned */
                      &out_msg_buf,   
                      &status);
      if (GOOD_STATUS(&status)) {
        printf("\nRetrieved msg successfully: %s\n", out_msg_buf->data);
        mos_msg_buf_free(&out_msg_buf, &status);
      } else if (status == mos_empty_queue) {
        printf("\nEmpty queue; No msgs to retrieve\n");
      } else {
        printf("\nError retrieving msg: %d\n", status);
      }
   } /* End of loop thru all messages */
       
   /* 
    * Exit from DCE-MOS 
    */
   mos_done(&status);

   printf("\nDCE-MOS basic example complete...\n");

} /* End of main() */
