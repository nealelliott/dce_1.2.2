/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: rpc_server.c,v $
 * Revision 1.1.5.2  1996/02/17  23:21:59  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:01  marty]
 *
 * Revision 1.1.5.1  1995/12/11  20:09:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:47  root]
 * 
 * Revision 1.1.3.5  1993/01/25  19:27:41  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:43:52  bbelch]
 * 
 * Revision 1.1.3.4  1993/01/11  23:39:37  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:12:56  bbelch]
 * 
 * Revision 1.1.3.3  1992/10/12  17:26:56  cuti
 * 	use "rpc_server_use_all_protseqs" instead of specific protocol.
 * 	[1992/10/09  21:18:12  cuti]
 * 
 * Revision 1.1.3.2  1992/09/29  21:22:46  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:47:55  weisman]
 * 
 * Revision 1.1  1992/01/19  03:38:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



#include <stdio.h>
#include <signal.h>
#include <dce/rpc.h>
#include <tsh_common.h>
#include <tsh_binding.h>
#include <binding_server.h>
#include <dce/dce_error.h>


struct if_reg {
  rpc_if_handle_t *s_ifspec;
  char *name;
} 

If[] = {
  { &binding_server_v1_0_s_ifspec, "Runtime_Tests" },
  {0,0}
};

unsigned long MaxCalls;


/*****************************************************************************/
/*  Register the interfaces.  Don't have to specify manager epvs, use the    */
/*  NIDL generated defaults.  Also, don't have to specify the uuid_type      */
/*  because only a single implementation of each manager is being used.      */
/*****************************************************************************/

register_ifs()
{
  error_status_t      status;
  unsigned long       interface;
  struct if_reg       *if_reg;
  dce_error_string_t  errorbuf;
  rpc_binding_vector_t  *bindings; 

  for (if_reg = If; if_reg->s_ifspec; if_reg++) {

    rpc_server_use_all_protseqs(1, &status);

    if(status != rpc_s_ok) {
      dce_error_inq_text(status, errorbuf, (int *) &status);
      fprintf(stderr, "Can't use protocols sequence for %s: %s\n",
              if_reg->name, errorbuf);
    } 
	else
      fprintf(stderr, "Register protocol sequence.\n"); 

     rpc_server_register_if(*if_reg->s_ifspec, (uuid_p_t) 0,
                           (rpc_mgr_epv_t) 0, &status);

    if (status != rpc_s_ok) {
      dce_error_inq_text(status, errorbuf, (int *) &status);
      fprintf(stderr, "Can't register %s test manager - %s\n", 
              if_reg->name, errorbuf);
    } 
	else
      fprintf(stderr, "Register interface.\n");
		
    rpc_server_inq_bindings(&bindings, &status);
    if ( status != rpc_s_ok) 
        {
            dce_error_inq_text(status, errorbuf, (int *) &status);
            fprintf(stderr,"Can't inquire binding for %s test manager - %s\n", 
                if_reg->name, errorbuf);
        }

	rpc_ep_register( *if_reg->s_ifspec, bindings, (uuid_vector_t *) NULL, 
                          (unsigned_char_p_t) if_reg->name, &status);
    if ( status != rpc_s_ok) 
        {
            dce_error_inq_text(status, errorbuf, (int *) &status);
            fprintf(stderr,"Can't register %s end point - %s\n", 
                if_reg->name, errorbuf);
        }


  }
}



static void usage()
{
    fprintf(stderr, "usage: server [-dv] <max calls> <family name> ... \n");
    fprintf(stderr, "  -d: Enables NCK debugging trace output\n");
    fprintf(stderr, "  -v: Turns on verbose mode\n");
}
   
 
void binding_server_op1(handle, subcase, status)
handle_t         handle;
long             subcase;
error_status_t   *status;
{
  handle_t   server_binding;

  switch (subcase) {
    case VALID_HANDLE:
      rpc_binding_server_to_client(handle, &server_binding, status);
      break;
    case INVALID_HANDLE:
      rpc_binding_server_to_client(NULL, &server_binding, status);
      break;
    case TERMINATE:
    default:
      fprintf(stderr, "rpc_server: exiting...\n");
      rpc_mgmt_stop_server_listening(NULL, status);
      fprintf(stderr, "rpc_server: rpc_mgmt_stop_server_listening status: %d\n", *status);
      break;
  }
}
  


/*
 * M A I N
 *
 * Main program.
 */

main(argc, argv)
int argc;
char *argv[];
{
  error_status_t       status;
  int                  c; 
  int                  arg_count, i, j;
  unsigned long        interface;
  struct if_reg        *if_reg;
  dce_error_string_t   errorbuf;

  arg_count = argc;
  for (i = 1, j = 1; i < arg_count; i++) {
    argv[j] = argv[i];
    if (strncmp(argv[i], "-d", 2) == 0) {
      if (*(argv[i]+2) == '\0')
        rpc__dbg_set_switches("0-4.10", &status);
      else
        rpc__dbg_set_switches(argv[i]+2, &status);
      argc--; }
    else if (strcmp(argv[i], "-help") == 0) {
      usage();
      argc--; 
      exit(0); }
    else
      j++;
  }


        

/***************************************************************************/
/*  Using known ports, don't have to export interfaces to the name server. */
/*  Later, maybe export bindings so that we can test the name server.      */
/***************************************************************************/

    register_ifs();

    rpc_server_listen(1, &status);

/*****************************************************************************/
/*  Maybe figure out when to unregister_ifs.  Or simply wait to be killed?   */
/*  That should be ok in this case cause we haven't registered with the name */
/*  server and the runtime will disappear when we kill the server.           */
/*****************************************************************************/
}

