/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cb_test.c,v $
 * Revision 1.1.11.1  1996/10/02  17:50:41  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:49  damon]
 *
 * Revision 1.1.6.4  1994/09/28  21:22:02  sasala
 * 	bos output incorrect -missing output, OT 12280, S12Y
 * 	[1994/09/28  21:20:16  sasala]
 * 
 * Revision 1.1.6.3  1994/08/03  16:09:55  ruby
 * 	Fixed RCS problem
 * 	[1994/08/03  16:06:30  ruby]
 * 
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  16:55:30  ruby]
 * 
 * Revision 1.1.4.2  1994/06/29  11:38:08  devsrc
 * 	merged with bl-10
 * 
 * Revision 1.1.4.1  1994/04/28  17:36:13  mbs
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:30  mbs]
 * 
 * Revision 1.1.6.2  1994/07/13  22:28:27  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:38:08  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:30  mbs]
 * 
 * Revision 1.1.6.1  1994/06/09  14:09:43  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:22  annie]
 * 
 * Revision 1.1.2.2  1993/01/21  19:38:38  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  19:52:41  zeliff]
 * 
 * Revision 1.1  1992/01/19  02:41:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * (C) Copyright Transarc Corporation 1989
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

/*------------------------------------------------------------------------
 * cb_test: A test of the gator text circular buffer package.
 *
 *	Ed. Zayas
 *	Transarc Corporation
 *------------------------------------------------------------------------*/

#include <gtxtextcb.h>	/*Module interface*/

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsgtxmac.h>
#include <dfsgtxsvc.h>
#include <dfsgtxmsg.h>


dce_svc_handle_t   gtx_svc_handle;

void initialize_svc()
{
  error_status_t st;

  gtx_svc_handle = dce_svc_register(gtx_svc_table,  (idl_char *)"gtx",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dce_msg_define_msg_table(gtx__table,
        sizeof gtx__table / sizeof gtx__table[0], &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Warning Cannot register gtx incore message table 0x%lx\n", st);
}



main(argc, argv)
    int argc;
    char **argv;

{ /*main*/

    register long code;				/*Return code*/
    struct gator_textcb_hdr *newCB;		/*Ptr to new CB hdr*/
    char buf[1024];				/*Text buffer*/
    int do_debugging;				/*Print out debugging info?*/
    int i, j, k;				/*Loop variables*/
    struct gator_textcb_entry *curr_ent;	/*Ptr to text entry*/
    char *curr_char;				/*Current char to print*/


    initialize_svc();


    dce_svc_printf(GTX_S_TEST_GATOR_CIRCULAR_BUFFER_MSG);
    dce_svc_printf(GTX_S_TURN_DEBUGGING_ON_MSG);
    scanf("%d", &do_debugging);
    dce_svc_printf(GTX_S_INIT_CB_PACKAGE_MSG, (do_debugging ? "YES" : "NO"));
    code = gator_textcb_Init(do_debugging);
    if (code) {
      dce_svc_printf(GTX_S_INIT_CB_PACKAGE_FAILED_MSG);
      exit(-1);
    }
    dce_svc_printf(GTX_S_CREATE_A_CB_MSG);
    newCB = gator_textcb_Create(100, 80);
    if (newCB == (struct gator_textcb_hdr *)0) {
      dce_svc_printf(GTX_S_CANT_CREATE_NEW_CB_MSG);
      exit(-1);
    }

    /*
      * Start writing stuff to this circ buff.
      */
    dce_svc_printf(GTX_S_FIRST_WRITE_TO_CB_MSG);
    sprintf(buf, "%s", "0123456789");
    code = gator_textcb_Write(newCB, buf, strlen(buf), 0, 0);
    if (code)
      dce_svc_printf(GTX_S_FIRST_WRITE_TO_CB_FAILED_MSG, code);

    dce_svc_printf(GTX_S_SECOND_WRITE_TO_CB_MSG);
    code = gator_textcb_Write(newCB, buf, 10, 1, 0);
    if (code)
      dce_svc_printf(GTX_S_SECOND_WRITE_TO_CB_FAILED_MSG, code);

    dce_svc_printf(GTX_S_THIRD_WRITE_TO_CB_MSG);
    sprintf(buf, "%s", "Now is the time for all good men to come to the aid of their country");
    code = gator_textcb_Write(newCB, buf, strlen(buf), 0, 1);
    if (code)
      dce_svc_printf(GTX_S_THIRD_WRITE_TO_CB_FAILED_MSG, code);

    dce_svc_printf(GTX_S_WRITE_BLANKS_TO_CB_MSG);
    code = gator_textcb_BlankLine(newCB, 3);
    if (code)
      dce_svc_printf(GTX_S_WRITE_BLANKS_TO_CB_FAILED_MSG, code);

    /*
      * Print out the CB status.
      */
    dce_svc_printf(GTX_S_STATUS_OF_CB_AFTER_WRITES_MSG);
    dce_svc_printf(GTX_S_MAX_ENTRIES_IN_CB_MSG, newCB->maxEntriesStored);
    dce_svc_printf(GTX_S_CHARS_PER_ENTRY_IN_CB_MSG, newCB->maxCharsPerEntry);
    dce_svc_printf(GTX_S_CURRENT_ENTRY_IN_CB_MSG, newCB->currEnt);
    dce_svc_printf(GTX_S_CURRENT_ENTRY_ID_IN_CB_MSG, newCB->currEntIdx);
    dce_svc_printf(GTX_S_DESTINATION_ENTRY_IN_CB_MSG, newCB->oldestEnt);
    dce_svc_printf(GTX_S_DESTINATION_ENTRY_INDEX_IN_CB_MSG, newCB->oldestEntIdx);

    dce_svc_printf(GTX_S_CONTINUE_CB_TEST_MSG);
    scanf("%d", &i);

    curr_ent = newCB->entry + newCB->oldestEntIdx;
    for (j = 0, i = newCB->oldestEntIdx; j < 5; j++) {
      dce_svc_printf(GTX_S_CB_ENTRY_MSG, curr_ent->ID, i);
      dce_svc_printf(GTX_S_CB_HIGHLIGHT_MSG, curr_ent->highlight);
      dce_svc_printf(GTX_S_CB_INVERSIONS_MSG, curr_ent->numInversions);
      dce_svc_printf(GTX_S_CB_INVERSIONS2_MSG);
      for (k=0; k < GATOR_TEXTCB_MAXINVERSIONS; k++)
	dce_svc_printf(GTX_S_CB_CURRENT_INVERSION_MSG, curr_ent->inversion[k]);
      dce_svc_printf(GTX_S_GTX_NEWLINE_MSG);
      dce_svc_printf(GTX_S_CB_CHARS_USED_MSG, curr_ent->charsUsed);
      dce_svc_printf(GTX_S_CB_EXTP_MSG);
      curr_char = curr_ent->textp;
      for (k=0; k < curr_ent->charsUsed; k++)
	dce_svc_printf(GTX_S_CB_CURRENT_CHARACTER_MSG, *curr_char++);
      dce_svc_printf(GTX_S_GTX_NEWLINE_MSG);	
      if (i == newCB->maxEntriesStored - 1) {
	i = 0;
	curr_ent = newCB->entry;
      }
      else {
	i++;
	curr_ent++;
      }
    } /*for loop*/

    dce_svc_printf(GTX_S_CB_SMALL_WRITES_MSG);
    dce_svc_printf(GTX_S_CONTINUE_CB_TEST_MSG);
    scanf("%d", &i);
    sprintf(buf, "%s", "a");
    for (i=0; i < 100; i++) {
      dce_svc_printf(GTX_S_CB_POUND_MSG);
      code = gator_textcb_Write(newCB, buf, 1, 0, 1 /*skip*/);
      if (code)
	dce_svc_printf(GTX_S_CB_SMALL_WRITES_FAILED_MSG, i, code);
    }
   dce_svc_printf(GTX_S_GTX_NEWLINE_MSG);

    /*
      * Print out the CB status.
      */
    dce_svc_printf(GTX_S_CB_STATUS_AFTER_WRITES_MSG);
    dce_svc_printf(GTX_S_MAX_ENTRIES_IN_CB_MSG, newCB->maxEntriesStored);
    dce_svc_printf(GTX_S_CHARS_PER_ENTRY_IN_CB_MSG, newCB->maxCharsPerEntry);
    dce_svc_printf(GTX_S_CURRENT_ENTRY_IN_CB_MSG, newCB->currEnt);
    dce_svc_printf(GTX_S_CURRENT_ENTRY_ID_IN_CB_MSG, newCB->currEntIdx);
    dce_svc_printf(GTX_S_DESTINATION_ENTRY_IN_CB_MSG, newCB->oldestEnt);
    dce_svc_printf(GTX_S_DESTINATION_ENTRY_INDEX_IN_CB_MSG, newCB->oldestEntIdx);

    dce_svc_printf(GTX_S_CONTINUE_CB_TEST_MSG);
    scanf("%d", &i);

    curr_ent = newCB->entry + newCB->oldestEntIdx;
    for (j = 0, i = newCB->oldestEntIdx; j < 100; j++) {

      dce_svc_printf(GTX_S_CB_ENTRY_MSG, curr_ent->ID, i);
      dce_svc_printf(GTX_S_CB_HIGHLIGHT_MSG, curr_ent->highlight);
      dce_svc_printf(GTX_S_CB_INVERSIONS_MSG, curr_ent->numInversions);
      dce_svc_printf(GTX_S_CB_INVERSIONS2_MSG);

      for (k=0; k < GATOR_TEXTCB_MAXINVERSIONS; k++)
	dce_svc_printf(GTX_S_CB_CURRENT_INVERSION_MSG, curr_ent->inversion[k]);

      dce_svc_printf(GTX_S_GTX_NEWLINE_MSG);
      dce_svc_printf(GTX_S_CB_CHARS_USED_MSG, curr_ent->charsUsed);
      dce_svc_printf(GTX_S_CB_EXTP_MSG);

      curr_char = curr_ent->textp;
      for (k=0; k < curr_ent->charsUsed; k++)
	dce_svc_printf(GTX_S_CB_CURRENT_CHARACTER_MSG, *curr_char++);
      dce_svc_printf(GTX_S_GTX_NEWLINE_MSG);
      if (i == newCB->maxEntriesStored - 1) {
	i = 0;
	curr_ent = newCB->entry;
      }
      else {
	i++;
	curr_ent++;
      }
    } /*for loop*/

    /*
      * Clean-up time!
      */
    code = gator_textcb_Delete(newCB);
    if (code) {
      dce_svc_printf(GTX_S_CB_DELETION_FAILED_MSG, code);
      exit(-1);
    }

    /*
      * It worked, mon!  Goombay!!
      */
    exit(0);

} /*main*/
