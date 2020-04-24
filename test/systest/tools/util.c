/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util.c,v $
 * Revision 1.1.19.2  1996/02/17  23:29:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:11  marty]
 *
 * Revision 1.1.19.1  1995/12/11  22:51:37  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/05  19:33 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1995/12/11  22:23:50  root]
 * 
 * Revision 1.1.16.1  1994/02/23  21:44:12  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:42:39  pellis]
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  19:08  bissen
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.9.2  1993/07/09  16:35:53  root
 * 	Initial King Kong branch
 * 	[1993/07/09  16:35:24  root]
 * 
 * Revision 1.1.7.2  1993/04/23  16:19:21  sekhar
 * 	Merged Hp_dce1_0_1 changes
 * 	[1993/04/16  15:02:48  sekhar]
 * 
 * Revision 1.1.1.7  1993/03/11  22:16:53  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.4.3  1993/02/05  15:29:39  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:50:23  cjd]
 * 
 * Revision 1.1.4.2  1992/09/04  16:08:10  bolinger
 * 	Fix OT defect 5339:  track EXCEPTION union member name changes
 * 	introduced by new threads source base.  (These changes do not
 * 	alter code generated to access members involved.)
 * 	[1992/09/04  14:59:27  bolinger]
 * 
 * Revision 1.1.2.3  1992/04/15  16:57:39  rps
 * 	rios needs netinet/in.h explicitly
 * 	[1992/04/15  15:20:19  rps]
 * 
 * 	Created from merge of rpc/*_util.c
 * 	Fix addr_from_name to use inet_ntoa
 * 	[1992/04/09  17:28:15  rps]
 * 
 * $EndLog$
 */
/*
 *   	FILE_NAME: util.c
 *
 *   	COMPONENT_NAME: dce.systest
 *
 *   	FUNCTIONS: make_header
 *		   check_status
 *		   check_exit_code
 *		   string_binding_to_name
 *		   name_from_addr
 *		   addr_from_name
 *		   close_sf
 *		   close_lf
 *		   print_exception
 *
 *   	ORIGINS: 27
 *
 *   	(C) COPYRIGHT International Business Machines Corp. 1990
 *   	All Rights Reserved
 *
 *   	USAGE: n/a
 *
 *   	DESCRIPTION:
 *
 *		This file contains functions used by clients and servers.
 *
 *		Function make_header constructs a header to be logged with
 *		all messages.  The header has the form of hostname, program
 *		name and process id number.  This header aids in determining
 *		which program logged which message.
 *
 *              Function check_status receives a status code, the expected
 *              value of the status, the message to log if the status code
 *              does not match the expected value, an opcode, a boolean value
 *              and whether the error originated from an RPC call or not.  If
 *              the values do not satisfy the opcode, an error message is
 *              constructed by appending the message passed to the header.  If
 *              the status code resulted from an RPC call (as specified by the
 *              mod parameter), dce_error_inq_text is called to convert the
 *              status code into an error string.  If the status did not origi-
 *              nate from an RPC call, the status value is appended to the
 *              error message.   If the boolean value is true, the workstation
 *              will log the message via xx_severe and exit.  If the boolean
 *              value is false, the workstation will log the message via
 *              xx_error and continue.
 *
 *              Function check_exit_code simply calls xx_severe or xx_error
 *              with the error message passed to it, depending on the boolean
 *              exit status.  If the status is true, the workstation exits.
 *              If false, the testcase continues.
 *
 *              Function string_binding_to_name parses the host address and
 *              port number out of an rpc string.  The address is then con-
 *              verted to its string name counterpart via a call to
 *              name_from_addr().
 *
 *              Function name_from_addr calls gethostbyaddr() to return the
 *              host string name of the address it was passed.
 *
 *              Function addr_from_name calls gethostbyname() to obtain the
 *              network address of the machine name it was passed.
 *
 *		Function close_sf tests if two short floating point numbers
 *		are within the required tolerance.
 *
 *		Function close_lf tests if two long floating point numbers
 *		are within the required tolerance.
 *
 *		Function print_exception takes an exception and turns it into
 *		a readable form for the tester.
 *
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dce/cma.h>
#include <dce/rpc.h>
#include <dce/rpcexc.h>
#include <dce/dce_error.h>
#include <dce/dce.h>
#include <systest.h>
#include <util.h>

char header[512], buf[128];

/*
 * make_header create a string that contains the hostname, program name and
 * process id number of the process.  This header is used for logging pur-
 * poses.
 */
void make_header(char *prog)
{
	int i = 0, pid_num = getpid();
	char host[30];

	gethostname(host, 30);
	sprintf(header, "host:%s\tprogram:%s\tpid:%d\n", host, prog, pid_num);
}

/*
 * check_status checks the status code passed with the expected results. If
 * they do not satisfy the opcode, a message is composed of the header string,
 * the message passed and either a string from dce_error_inq_text() or the
 * value of the status.  That message is logged as an error.
 */
int check_status(error_status_t st, error_status_t expected_status, int op_code,
		 char *message, boolean32 exit_status, int mod)
{
	int check_exit_code(boolean32, char *);
	char error_message[1536];
	char dce_text_string[512];
	int rc = 0;
	error_status_t status;

	switch(mod)
	{
		case RPC:
			dce_error_inq_text(st, dce_text_string, (int *)&status);
			if (status == rpc_s_unknown_status_code)
				sprintf(dce_text_string,"unknown status %d\n",
					st);
			break;
		default:
			sprintf(dce_text_string, "status = %ld\n", st);
			break;
	}
	
	sprintf(error_message, "%s%s%s", header, message, dce_text_string);
	switch(op_code)
	{
		case NOT_EQUAL:
			if (st != expected_status)	
				rc = check_exit_code(exit_status, 
						     error_message);
			break;
		case EQUAL:
			if (st == expected_status)	
				rc = check_exit_code(exit_status, 
						     error_message);
			break;
		case GR_THAN:
			if (st > expected_status)	
				rc = check_exit_code(exit_status, 
						     error_message);
			break;
		case LS_THAN:
			if (st < expected_status)	
				rc = check_exit_code(exit_status, 
						     error_message);
			break;
	}
	return(rc);
}

/*
 * check_exit_code determines whether or not to exit the test with a severe
 * error message or just to log an error message and continue based upon
 * the boolean value passed.
 */
int check_exit_code(boolean32 st, char *message)
{
	switch(st)
	{
		case TRUE:
			xx_severe(message);
			exit(1);
			break;
		case FALSE:
			xx_error(message);
			return(1);
	}
}

/*
 * string_binding_to_name takes a string representation of an rpc binding 
 * and parses out the host address and port number.  The host address is
 * converted to the string name of the host via a call to name_from_addr().
 * This function aids in showing what server a client is bound to.
 */
void string_binding_to_name(unsigned_char_p_t binding, char *hostname, 
 			    unsigned_char_p_t *port, 
			    unsigned_char_p_t *prot_sequence)
{
	unsigned_char_p_t host_address;
	char *name_from_addr(char *);
	error_status_t st;

	rpc_string_binding_parse(binding,
				 NULL,
				 prot_sequence,
				 &host_address,
				 port,
				 NULL,
			  	 &st);
	sprintf(hostname, "%s", name_from_addr(host_address));
}

/*
 * name_from_addr calls gethostbyaddr() to return the hostname of the
 * address passed.
 */
char *name_from_addr(char *host_addr)
{
	struct hostent *hp;
	unsigned32 long_host_addr;

	long_host_addr = inet_addr(host_addr);
	hp = gethostbyaddr(&long_host_addr, sizeof(unsigned32), AF_INET);
	return(hp->h_name);
}

/*
 * addr_from _name calls gethostbyname() to obtain the associated inet
 * address of the hostname passed.
 */
char *addr_from_name(char *hostname)
{
	struct hostent *hp;

	hp = gethostbyname(hostname);
	if (hp == NULL)
	{
		printf("gethostbyname failed\n");
		exit(1);
	}

	return (strdup(inet_ntoa(*(struct in_addr *)hp->h_addr)));
}	

/*
 * Tests for closeness of two short floating values
 */
idl_boolean close_sf (idl_short_float f1, idl_short_float f2)
{
    idl_short_float ratio;

    if (-0.0001 < f2 && 0.0001 > f2)
        return (-0.0001 < f1 && 0.0001 > f1);
    else {
        ratio = f1 / f2;
        return (ratio > 0.9999 && ratio < 1.0001);
    }
}

/*
 * Tests for closeness of two short floating values
 */
idl_boolean close_lf (idl_long_float f1, idl_long_float f2)
{
    idl_long_float ratio;

    if (-0.0001 < f2 && 0.0001 > f2)
        return (-0.0001 < f1 && 0.0001 > f1);
    else {
        ratio = f1 / f2;
        return (ratio > 0.9999 && ratio < 1.0001);
    }
}

/*
 * print_exception attempts to match an exception and compose a read form of
 * the exception.
 */
void print_exception(EXCEPTION *e)
{
      char *t = NULL;

/* DG and common errors */
      if (exc_matches(e,&rpc_x_cancel_timeout)) t = "rpc_x_cancel_timeout";

      else if (exc_matches(e,&rpc_x_coding_error)) t = "rpc_x_coding_error";

      else if (exc_matches(e,&rpc_x_comm_failure)) t = "rpc_x_comm_failure";

      else if (exc_matches(e,&rpc_x_invalid_binding)) t = "rpc_x_invalid_binding";

      else if (exc_matches(e,&rpc_x_invalid_tag)) t = "rpc_x_invalid_tag";

      else if (exc_matches(e,&rpc_x_invalid_bound)) t = "rpc_x_invalid_bound";

      else if (exc_matches(e,&rpc_x_manager_not_entered)) t = "rpc_x_manager_not_entered";

      else if (exc_matches(e,&rpc_x_no_memory)) t = "rpc_x_no_memory";

      else if (exc_matches(e,&rpc_x_op_rng_error)) t = "rpc_x_op_rng_error";

      else if (exc_matches(e,&rpc_x_protocol_error)) t = "rpc_x_protocol_error";

      else if (exc_matches(e,&rpc_x_unknown_if)) t = "rpc_x_unknown_if";

      else if (exc_matches(e,&rpc_x_unknown_error)) t = "rpc_x_unknown_error";

      else if (exc_matches(e,&rpc_x_unknown_reject)) t = "rpc_x_unknown_reject";

      else if (exc_matches(e,&rpc_x_unknown_remote_fault)) t = "rpc_x_unknown_remote_fault";

      else if (exc_matches(e,&rpc_x_unsupported_type)) t = "rpc_x_unsupported_type";

      else if (exc_matches(e,&rpc_x_who_are_you_failed)) t = "rpc_x_who_are_you_failed";

      else if (exc_matches(e,&rpc_x_wrong_boot_time)) t = "rpc_x_wrong_boot_time";


/* CN errors */
      if (exc_matches(e,&rpc_x_access_control_info_inv)) t = "rpc_x_access_control_info_inv";


      else if (exc_matches(e,&rpc_x_assoc_shutdown)) t = "rpc_x_assoc_shutdown";

      else if (exc_matches(e,&rpc_x_cannot_accept)) t = "rpc_x_cannot_accept";

      else if (exc_matches(e,&rpc_x_connect_closed_by_rem)) t = "rpc_x_connect_closed_by_rem";

      else if (exc_matches(e,&rpc_x_connect_no_resources)) t = "rpc_x_connect_no_resources";

      else if (exc_matches(e,&rpc_x_connect_rejected)) t = "rpc_x_connect_rejected";

      else if (exc_matches(e,&rpc_x_connect_timed_out)) t = "rpc_x_connect_timed_out";

      else if (exc_matches(e,&rpc_x_connection_aborted)) t = "rpc_x_connection_aborted";

      else if (exc_matches(e,&rpc_x_connection_closed)) t = "rpc_x_connection_closed";

      else if (exc_matches(e,&rpc_x_host_unreachable)) t = "rpc_x_host_unreachable";

      else if (exc_matches(e,&rpc_x_invalid_call_opt)) t = "rpc_x_invalid_call_opt";

      else if (exc_matches(e,&rpc_x_invalid_endpoint_format)) t = "rpc_x_invalid_endpoint_format";

      else if (exc_matches(e,&rpc_x_loc_connect_aborted)) t = "rpc_x_loc_connect_aborted";

      else if (exc_matches(e,&rpc_x_network_unreachable)) t = "rpc_x_network_unreachable";

      else if (exc_matches(e,&rpc_x_no_rem_endpoint)) t = "rpc_x_no_rem_endpoint";

      else if (exc_matches(e,&rpc_x_no_fault)) t = "rpc_x_no_fault";

      else if (exc_matches(e,&rpc_x_rem_host_crashed)) t = "rpc_x_rem_host_crashed";

      else if (exc_matches(e,&rpc_x_rem_host_down)) t = "rpc_x_rem_host_down";

      else if (exc_matches(e,&rpc_x_rem_network_shutdown)) t = "rpc_x_rem_network_shutdown";

      else if (exc_matches(e,&rpc_x_too_many_rem_connects)) t = "rpc_x_too_many_rem_connects";

      else if (exc_matches(e,&rpc_x_tsyntaxes_unsupported)) t = "rpc_x_tsyntaxes_unsupported";


/* NS import routine errors */
      if (exc_matches(e,&rpc_x_binding_vector_full)) t = "rpc_x_binding_vector_full";

      else if (exc_matches(e,&rpc_x_entry_not_found)) t = "rpc_x_entry_not_found";

      else if (exc_matches(e,&rpc_x_group_not_found)) t = "rpc_x_group_not_found";

      else if (exc_matches(e,&rpc_x_incomplete_name)) t = "rpc_x_incomplete_name";

      else if (exc_matches(e,&rpc_x_invalid_import_context)) t = "rpc_x_invalid_import_context";

      else if (exc_matches(e,&rpc_x_invalid_inquiry_context)) t = "rpc_x_invalid_inquiry_context";

      else if (exc_matches(e,&rpc_x_invalid_inquiry_type)) t = "rpc_x_invalid_inquiry_type";

      else if (exc_matches(e,&rpc_x_invalid_lookup_context)) t = "rpc_x_invalid_lookup_context";

      else if (exc_matches(e,&rpc_x_invalid_name_syntax)) t = "rpc_x_invalid_name_syntax";

      else if (exc_matches(e,&rpc_x_invalid_object)) t = "rpc_x_invalid_object";

      else if (exc_matches(e,&rpc_x_invalid_rpc_protseq)) t = "rpc_x_invalid_rpc_protseq";

      else if (exc_matches(e,&rpc_x_invalid_vers_option)) t = "rpc_x_invalid_vers_option";

      else if (exc_matches(e,&rpc_x_no_more_bindings)) t = "rpc_x_no_more_bindings";

      else if (exc_matches(e,&rpc_x_no_more_elements)) t = "rpc_x_no_more_elements";

      else if (exc_matches(e,&rpc_x_not_found)) t = "rpc_x_not_found";

      else if (exc_matches(e,&rpc_x_not_rpc_entry)) t = "rpc_x_not_rpc_entry";

      else if (exc_matches(e,&rpc_x_profile_not_found)) t = "rpc_x_profile_not_found";

      else if (exc_matches(e,&rpc_x_unsupported_name_syntax)) t = "rpc_x_unsupported_name_syntax";


/* Stub support errors */
      if (exc_matches(e,&rpc_x_ss_char_trans_open_fail)) t = "rpc_x_ss_char_trans_open_fail";

      else if (exc_matches(e,&rpc_x_ss_char_trans_short_file)) t = "rpc_x_ss_char_trans_short_file";

      else if (exc_matches(e,&rpc_x_ss_context_damaged)) t = "rpc_x_ss_context_damaged";

      else if (exc_matches(e,&rpc_x_ss_context_mismatch)) t = "rpc_x_ss_context_mismatch";

      else if (exc_matches(e,&rpc_x_ss_in_null_context)) t = "rpc_x_ss_in_null_context";

      else if (exc_matches(e,&rpc_x_ss_pipe_closed)) t = "rpc_x_ss_pipe_closed";

      else if (exc_matches(e,&rpc_x_ss_pipe_comm_error)) t = "rpc_x_ss_pipe_comm_error";

      else if (exc_matches(e,&rpc_x_ss_pipe_discipline_error)) t = "rpc_x_ss_pipe_discipline_error";

      else if (exc_matches(e,&rpc_x_ss_pipe_empty)) t = "rpc_x_ss_pipe_empty";

      else if (exc_matches(e,&rpc_x_ss_pipe_memory)) t = "rpc_x_ss_pipe_memory";

      else if (exc_matches(e,&rpc_x_ss_pipe_order)) t = "rpc_x_ss_pipe_order";

      else if (exc_matches(e,&rpc_x_ss_remote_comm_failure)) t = "rpc_x_ss_remote_comm_failure";

      else if (exc_matches(e,&rpc_x_ss_remote_no_memory)) t = "rpc_x_ss_remote_no_memory";


      /* CMA exceptions */
      if (exc_matches(e,&exc_e_uninitexc)) t = "exc_e_uninitexc";
	else if (exc_matches(e,&exc_e_illaddr)) t = "exc_e_illaddr";
	else if (exc_matches(e,&exc_e_exquota)) t = "exc_e_exquota";
	else if (exc_matches(e,&exc_e_insfmem)) t = "exc_e_insfmem";
	else if (exc_matches(e,&exc_e_nopriv)) t = "exc_e_nopriv";
	else if (exc_matches(e,&exc_e_illinstr)) t = "exc_e_illinstr";
	else if (exc_matches(e,&exc_e_resaddr)) t = "exc_e_resaddr";
	else if (exc_matches(e,&exc_e_privinst)) t = "exc_e_privinst";
	else if (exc_matches(e,&exc_e_resoper)) t = "exc_e_resoper";
	else if (exc_matches(e,&exc_e_SIGTRAP)) t = "exc_e_SIGTRAP";
	else if (exc_matches(e,&exc_e_SIGIOT)) t = "exc_e_SIGIOT";
	else if (exc_matches(e,&exc_e_SIGEMT)) t = "exc_e_SIGEMT";
	else if (exc_matches(e,&exc_e_aritherr)) t = "exc_e_aritherr";
	else if (exc_matches(e,&exc_e_SIGSYS)) t = "exc_e_SIGSYS";
	else if (exc_matches(e,&exc_e_SIGPIPE)) t = "exc_e_SIGPIPE";
	else if (exc_matches(e,&exc_e_excpu)) t = "exc_e_excpu";
	else if (exc_matches(e,&exc_e_exfilsiz)) t = "exc_e_exfilsiz";
	else if (exc_matches(e,&exc_e_intovf)) t = "exc_e_intovf";
	else if (exc_matches(e,&exc_e_intdiv)) t = "exc_e_intdiv";
	else if (exc_matches(e,&exc_e_fltovf)) t = "exc_e_fltovf";
	else if (exc_matches(e,&exc_e_fltdiv)) t = "exc_e_fltdiv";
	else if (exc_matches(e,&exc_e_fltund)) t = "exc_e_fltund";
	else if (exc_matches(e,&exc_e_decovf)) t = "exc_e_decovf";
	else if (exc_matches(e,&exc_e_subrng)) t = "exc_e_subrng";
	else if (exc_matches(e,&cma_e_alerted)) t = "cma_e_alerted";
	else if (exc_matches(e,&cma_e_assertion)) t = "cma_e_assertion";
	else if (exc_matches(e,&cma_e_badparam)) t = "cma_e_badparam";
	else if (exc_matches(e,&cma_e_bugcheck)) t = "cma_e_bugcheck";
	else if (exc_matches(e,&cma_e_exit_thread)) t = "cma_e_exit_thread";
	else if (exc_matches(e,&cma_e_existence)) t = "cma_e_existence";
	else if (exc_matches(e,&cma_e_in_use)) t = "cma_e_in_use";
	else if (exc_matches(e,&cma_e_use_error)) t = "cma_e_use_error";
	else if (exc_matches(e,&cma_e_wrongmutex)) t = "cma_e_wrongmutex";
	else if (exc_matches(e,&cma_e_stackovf)) t = "cma_e_stackovf";
	else if (exc_matches(e,&cma_e_nostackmem)) t = "cma_e_nostackmem";
	else if (exc_matches(e,&cma_e_notcmastack)) t = "cma_e_notcmastack";
	else if (exc_matches(e,&cma_e_unimp)) t = "cma_e_unimp";
	else if (exc_matches(e,&cma_e_inialrpro)) t = "cma_e_inialrpro";
	else if (exc_matches(e,&cma_e_defer_q_full)) t = "cma_e_defer_q_full";
	else if (exc_matches(e,&cma_e_signal_q_full)) t = "cma_e_signal_q_full";
	else if (exc_matches(e,&cma_e_alert_nesting)) t = "cma_e_alert_nesting";


      /* 
       *  If we found a named exception that matches print it, otherwise check
       *  if it is a status and display that if possible.
       */
      if (t != NULL)
          printf("***FAILURE***: unexpected CMA exception %s\n",t);
      else
      {
#ifdef __OSF1__
          printf("***FAILURE***: unexpected CMA exception 0X%X\n",e);
#else
          if (e->kind ==exc_kind_status_c)
          {
#ifndef VMS
              /* need to use a macro here - sekhar REVISIT THIS ***/
              printf("***FAILURE***: CMA status exception %d (%s)\n",e->status.status, strerror(e->status.status));
#else
              printf("***FAILURE***: CMA status exception %d (%s)\n",e->status.status, strerror(EVMSERR,e->status.status));
#endif      
          }
          else if (e->kind == exc_kind_address_c)
              /**  need to use a macro here. sekhar REVISIT THIS AGAIN ****/
              printf("***FAILURE***: unexpected user-defined CMA exception address 0x%x\n",e->address.address);
          else
              printf("***FAILURE***: corrupted CMA exception\n",t);
#endif
      }
}
