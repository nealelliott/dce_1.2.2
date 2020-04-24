/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: error_inq_text.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:29  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:39:13  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:04:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:11  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:04:17  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:35:47  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  22:13:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:05:48  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:35:01  devrcs
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
#include <tsh_error.h>
#include <dce/dce_error.h>


/*  Args required by the test program...
 */
#define ARGS_REQUIRED 3


/*  Generic test progrm entry point
 */
ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock          *ctx;
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
ndr_long_int      SubCase;		/* argv[0], subcase to run */
ndr_long_int      StartValue;		/* argv[1], start index */
ndr_long_int      StopValue;		/* argv[2], stop index */
error_status_t    status;		/* actual status from test */
dce_error_string_t ErrorString;		/* String returned from test */
unsigned32        index;		/* index value for status values */


/*  check the number of arguments passed in.  They should be exactly the   
 *  required number when you come into this routine. Looking for a subcase,
 *  a bogus expected_status, and a hostname.
 */
if(argc != ARGS_REQUIRED)
    {
    TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
        ARGS_REQUIRED, argc);
    return(TSH_BAD_ARG_COUNT);
    }


/*  atol() the input args...
 */
SubCase    = atol(argv[0]);
StartValue = atol(argv[1]);
StopValue  = atol(argv[2]);


TSHMessage(ctx, MOINFO, "range of error values: %ld (0x%lx) to %ld (0x%lx)\n",
    StartValue, StartValue, StopValue, StopValue);


/*
 * This is for some sanity checking... Also warn the user that this
 * may be a long test...
 */
if(StartValue > StopValue) 
    {
    TSHMessage(ctx, MOFAILURE, "start index greater than stop index\n");
    return TSH_INVALID_ARGUMENT;
    }
if((StopValue - StartValue) > 100000L)
    TSHMessage(ctx, MOWARNING, "this test will take a long time\n");


/*  run the requested SubCase
 */
switch(SubCase)
    {
    case INQ_TEXT:
        TSHMessage(ctx, MOINFO, "Running subcase INQ_TEXT\n");

        for (index = StartValue; index <= StopValue; index++)
        {
            dce_error_inq_text(index, ErrorString, (int *)&status);
            switch(status)
            {     
			/* For rpc4, the return status is not correct.  
               Net Mishkin told me to ignore it from maile 5/15/91 */
	
                 /* case rpc_s_ok:   */
				default:
                     if (strlen(ErrorString))
                     {
                         TSHMessage(ctx, MOTRACE, 
                           "error value %lx has string \"%s\"\n",
                             index, ErrorString);

                         continue;
                     }
                     else
                     {
                         TSHMessage(ctx, MOFAILURE, 
                             "dce_error_inq_text() returned 0 length string\n");

                         return(TSH_TEST_FAILED);
                     }
                     break;

            /* 
                case rpc_s_unknown_status_code:
                     if(!strcmp((char *)ErrorString, (char *)"unknown status code"))
                     {
                         if((index % 0x010000) == 0)
                         {
                             TSHMessage(ctx, MOTRACE, "processing at error: %lx\n",
                                 index);
                         }

                         continue;
                     }
                     else
                     {
                         TSHMessage(ctx, MOFAILURE, 
                           "dce_error_inq_text() returned unexpected string: %s\n",
                             ErrorString);
                         return(TSH_TEST_FAILED);
                     }
                     break;  

                default:
                    TSHStatus(ctx, TEST, "dce_error_inq_text() error", status,
                        rpc_s_ok);
                    return status;   
				*/      
            }
        } 
        break; 
	

    default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
        return(TSH_UNKNOWN_SUBCASE);
    }


return(TSH_SUCCESS);
}


