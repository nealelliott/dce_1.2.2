/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: string_free.c,v $
 * Revision 1.1.4.3  1996/02/17  23:22:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:17  marty]
 *
 * Revision 1.1.4.2  1995/12/13  21:19:06  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	Revision /main/jrr_1.2_mothra/1  1995/12/12  22:44 UTC  dat
 * 	Don't include tsh_uuid.h.  Contains redefinitions of values defined in tsh_string.h
 * 	[1995/12/13  21:18:17  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:28:31  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:44:57  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:41:17  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:14:39  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:17  devrcs
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



#include <math.h>
#include <tsh_string.h>
#include <dce/dce_error.h>

/*  Args required by the test program...
 */
#define ARGS_REQUIRED 2


/*  Generic test progrm entry point
 */
ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
ndr_long_int      SubCase;		/* argv[0], subcase to run */
ndr_long_int      EStatus;		/* argv[1], ex[ected status from test */
unsigned_char_p_t temp_string;
uuid_p_t          test_uuid;
error_status_t    status;


/*  check the number of arguments passed in.  They should be exactly the   
 *  required number when you come into this routine. 
 */
if(argc != ARGS_REQUIRED)
    {
    TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
        ARGS_REQUIRED, argc);
    return(TSH_BAD_ARG_COUNT);
    }


/*  atol() the input args...
 */
SubCase = atol(argv[0]);
EStatus = atol(argv[1]);


/*
 *  Set up a temp string by having a function malloc the memory.  Several
 *  of the RPC functions could do this, but use rpc_error_inq_text() since
 *  it is the most straight forward.
 */
/* dce_error_inq_text(rpc_s_ok, temp_string, &status); */

    test_uuid   = &tsh_c_valid_UUID;
	uuid_to_string(test_uuid, &temp_string, &status);

TSHMessage(ctx, MOTRACE, "string before being freed: \"%s\"\n", temp_string);


/*  run the requested SubCase
 */
switch(SubCase)
    {
    case FREE:
        TSHMessage(ctx, MOINFO, "Running subcase FREE\n");


        rpc_string_free(&temp_string, &status);
        if(TSHStatus(ctx, TEST, "rpc_string_free() failed", status, EStatus))
            return(status);

        TSHMessage(ctx, MOTRACE, "string after being freed: \"%s\"\n", temp_string);

        if(TSHStatus(ctx, TEST, "freed string not NULL", temp_string, 0))
            return(TSH_DATA_MISSMATCH);

        break;

    case FREE_AGAIN:
        TSHMessage(ctx, MOINFO, "Running subcase FREE_AGAIN\n");


        /*
         *  Free the string and check it like above, but this part is only the SETUP
         *  since the TEST will follow this with another rpc_string_free()
         */
        rpc_string_free(&temp_string, &status);
        if(TSHStatus(ctx, TEST, "rpc_string_free() failed", status, EStatus))
            return(status);

        TSHMessage(ctx, MOTRACE, "string after being freed the first time: \"%s\"\n", 
            temp_string);

        if(TSHStatus(ctx, TEST, "freed string not NULL", temp_string, 0))
            return(TSH_DATA_MISSMATCH);

        if(TSHStatus(ctx, TEST, "freed string length not 0", strlen(temp_string), 0))
            return(TSH_DATA_MISSMATCH);


        /* 
         *  Now try to free the string again...
         */
        rpc_string_free(&temp_string, &status);
        if(TSHStatus(ctx, TEST, "rpc_string_free() failed", status, EStatus))
            return(status);

        TSHMessage(ctx, MOTRACE, "string after being freed the second time: \"%s\"\n", 
            temp_string);

        if(TSHStatus(ctx, TEST, "freed string not NULL", temp_string, 0))
            return(TSH_DATA_MISSMATCH);

        if(TSHStatus(ctx, TEST, "freed string length not 0", strlen(temp_string), 0))
            return(TSH_DATA_MISSMATCH);

        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
        return(TSH_UNKNOWN_SUBCASE);
    }


return(TSH_SUCCESS);
}


