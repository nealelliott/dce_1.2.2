/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY 
 * $Log: check_status.c,v $
 * Revision 1.1.4.2  1996/02/17  23:34:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:52  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:09:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:15  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:51:54  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:44  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/21  22:05:25  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:46:57  cbrooks]
 * 
 * $EndLog$
 */

/*
 * dts_dtscp_check_status - check return code value
 * and print error message to stderr if no rpc_s_ok.
 * if the action requested is to abort, then call exit
 * we assume that any cleanup activity has been registered
 * as an atexit() function
 */

#include <dce/dce.h>
#include <dce/dce_error.h>
#include <check_status.h>
#include <stdio.h>
#include <stdlib.h>

static int            error_stat;
static unsigned char  error_string[ dce_c_error_string_len ];

void dts_dtscp_check_status( 
   	const unsigned32 input_status,
   	const char * comment_id,
	const int action) 
{
    register unsigned char * p ;
    
    if(input_status != rpc_s_ok) 
    { 
	p = (unsigned char *)comment_id ; /* p = dce_sprintf(comment_id) ;*/
	dce_error_inq_text(input_status, error_string, &error_stat); 
	
	fprintf(stderr, "%s %s\n", p, error_string); 
	if(action == dts_e_dtscp_abort) 
	{
	    abort();
	}
	
   } 
}

