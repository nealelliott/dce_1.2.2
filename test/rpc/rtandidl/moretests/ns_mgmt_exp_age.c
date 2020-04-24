/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ns_mgmt_exp_age.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:23  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:35  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:07:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:00  root]
 * 
 * Revision 1.1.2.2  1993/02/03  18:20:58  cuti
 * 	Create rpc_ns_mgmt_inq_exp_age() and rpc_ns_mgmt_set_exp_age() test
 * 	[1993/02/01  22:40:21  cuti]
 * 
 * $EndLog$
 */

/* Include files */

#include <tsh_common.h>
#include <tsh_ns.h>
#include <ns_mgmt_exp_age.h>
#include <dce_error.h>


/*  Args required by the test program.  */

#define ARGS_REQUIRED 3

ndr_long_int TestFunction 
(ctx, argc, argv)
   CtxBlock          *ctx;
   ndr_long_int      argc;
   unsigned_char_p_t argv[];
{
   	ndr_long_int      subcase;		/* argv[0], subcase to run */
   	error_status_t    estatus;		/* argv[1], expected return from test */
   	error_status_t    tstatus;		/* status value from rpc calls... */
   	error_status_t    cstatus;		/* status value from dce_error_inq_text()... */
   	unsigned32		  inq_exp_age;		/* inq expiration age */
   	unsigned32		  exp_age;		/* expiration age */
	unsigned32		  type_inq;
	unsigned32		  type_set;
	unsigned32		  estatus_inq;
	unsigned32		  estatus_set;
    dce_error_string_t	errorbuf;

                                   
/* Check the number of arguments passed in.  They should be exactly the   
   required number when you come into this routine. Looking for a subcase,
   a bogus estatus, and a hostname. */

   if (argc != ARGS_REQUIRED)
     {
       TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
                  ARGS_REQUIRED, argc);
       return(TSH_BAD_ARG_COUNT);
     }

/* Obtain the input arguments from the .tsh command line. */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]);
   exp_age = atol(argv[2]);

             
          
	switch(subcase) {
		case INQ_EXP_AGE:
			type_inq = TEST;
			type_set = SETUP;

			estatus_inq = estatus;
			estatus_set = rpc_s_ok;
			break;
        	
		case SET_EXP_AGE:	
			type_inq = SETUP;
			type_set = TEST;

			estatus_inq = rpc_s_ok;
			estatus_set = estatus;
			break;

	}

	rpc_ns_mgmt_inq_exp_age(&inq_exp_age, &tstatus);
	dce_error_inq_text(tstatus, errorbuf, &cstatus);
	TSHMessage(ctx, MOINFO, "rpc_ns_mgmt_inq_exp_age status: %s\n", errorbuf);
    TSHStatus(ctx, type_inq, "rpc_ns_mgmt_inq_exp_age failed", tstatus, estatus_inq);
                                
	switch(exp_age) {
		case dont_care:
		case default_value:
			exp_age = rpc_c_ns_default_exp_age;	
			break;
	}

	rpc_ns_mgmt_set_exp_age(exp_age, &tstatus);
	dce_error_inq_text(tstatus, errorbuf, &cstatus);
	TSHMessage(ctx, MOINFO, "rpc_ns_mgmt_set_exp_age status: %s\n", errorbuf);
    TSHStatus(ctx, type_set, "rpc_ns_mgmt_set_exp_age failed", tstatus, estatus_set);

	rpc_ns_mgmt_inq_exp_age(&inq_exp_age, &tstatus);
	dce_error_inq_text(tstatus, errorbuf, &cstatus);
	TSHMessage(ctx, MOINFO, "rpc_ns_mgmt_inq_exp_age status: %s\n", errorbuf);
	TSHMessage(ctx, MOINFO, "rpc_ns_mgmt_inq_exp_age set value: %d(-1 is a default), inq value: %d: \n", exp_age, inq_exp_age);  
	if (exp_age != -1) {
    	TSHStatus(ctx, type_inq, "rpc_ns_mgmt_inq_exp_age failed", tstatus, estatus_inq);
    	if (TSHStatus(ctx, type_inq, "rpc_ns_mgmt_inq_exp_age failed: Data mismatch", inq_exp_age, exp_age))
			tstatus = TSH_DATA_MISSMATCH; 
	}
     
	return (tstatus);

}
