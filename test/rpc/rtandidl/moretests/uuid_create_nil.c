/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: uuid_create_nil.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:49  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:30  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:12  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:30:11  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:47:15  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:50:22  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:17:19  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:04  devrcs
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
#include <math.h>
#include <tsh_uuid.h>


#define ARGS_REQUIRED        2          /* This routine should have 4 args */

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock      *ctx;
ndr_long_int  argc;
unsigned_char_p_t argv[];
{
uuid_t          NewUUID;                /* a temp UUID and the NIL UUID */
ndr_long_int    SubCase;                /* SubCAse for this test modulee */
ndr_long_int    LoopIndex;              /* generic loop counter */
error_status_t  status;                 /* status from rpc_* and uuid_* calls */
error_status_t  EStatus;                /* expected status, compared against status */
char            *mem_ptr;		/* pointer to memory space */
int             count = 0;		/* number of iterations for memory test */
boolean32       UUIDEqual;		/* generated UUID equal to nil UUID? */


/*  Check the number of arguments passed in.  They should be exactly the required
 *  number when you come into this routine.
 */ 
if(argc != ARGS_REQUIRED)
    {
    TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d.\n",
        ARGS_REQUIRED, argc);
    return(TSH_BAD_ARG_COUNT);
    }


/*  Init the UUID by set up the correct rpc mutex
 */
UUID_INIT


/*  atol() the subcase and expected return value to numbers.
 */
SubCase = atol(argv[0]);
EStatus = atol(argv[1]);


/*  Perform the appropriate Sub-Test in this module.  
 */
switch(SubCase)
    {
    case CREATE_NIL:
        TSHMessage(ctx, MOINFO, "Running subcase CREATE_NIL\n");

        uuid_create_nil(&NewUUID, &status);
        if(TSHStatus(ctx, TEST, "uuid_create_nil() failed", status, EStatus))
            return(status);

        UUIDEqual = uuid_equal(&NewUUID, &tsh_c_nil_UUID, &status);
        if(TSHStatus(ctx, CLNUP, "uuid_equal() failed", status, rpc_s_ok))
            return(status);
        if(TSHStatus(ctx, CLNUP, "generated uuid not nil uuid", UUIDEqual, TRUE))
            return(TSH_DATA_MISSMATCH);
 
        break;

    case MEMORY_ERROR:
        TSHMessage(ctx, MOINFO, "Running subcase MEMORY_ERROR\n");

        mem_ptr = (char *)calloc(1, 1000000);

        for (;;)
            {
            uuid_create_nil(&NewUUID, &status);
            if(status != rpc_s_ok)
                break;

            UUIDEqual = uuid_equal(&NewUUID, &tsh_c_nil_UUID, &status);
            if(TSHStatus(ctx, CLNUP, "uuid_equal() failed", status, rpc_s_ok))
                return(status);
            if(TSHStatus(ctx, CLNUP, "generated uuid not nil uuid", UUIDEqual, TRUE))
                return(TSH_DATA_MISSMATCH);
 
            if(++count%100000 == 0)
                TSHMessage(ctx, MOINFO, "created %d uuids\n", count);
            }

        cfree(mem_ptr);

        TSHMessage(ctx, MOINFO, "!!!!!\n");
        if(TSHStatus(ctx, TEST, "uuid_create_nil() failed", status, EStatus))
                return(status);

        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
        return(TSH_UNKNOWN_SUBCASE);
    }


return(TSH_SUCCESS);
}

