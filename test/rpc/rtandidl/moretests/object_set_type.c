/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: object_set_type.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:32  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:47  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:07:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:08  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:06:14  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:41:42  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:37:34  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:10:21  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:36:04  devrcs
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
#include <tsh_object.h>


#define ARGS_REQUIRED        4          /* This routine should have 4 args */
#define rpc_c_max_objects   64          /* From: runtime source file comobj.h */

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock      *ctx;
ndr_long_int  argc;
unsigned_char_p_t argv[];
{
uuid_p_t        ObjectUUID,             /* Object UUID pointer */
                TypeUUID;               /* Type UUID pointer */
uuid_t          TestUUID;               /* a temp UUID and the NIL UUID */
ndr_long_int    SubCase;                /* SubCAse for this test modulee */
ndr_long_int    LoopIndex;              /* generic loop counter */
error_status_t  status;                 /* status from rpc_* and uuid_* calls */
error_status_t  EStatus;                /* expected status, compared against status */


#ifdef DEBUG
printf("-------------------------------\nTestFunction:\nInput args are:\n");
PRINT_ARGS(argc, argv);
printf("    Program Name is %s\n", ctx->ProgramName);
#endif


/*  Check the number of arguments passed in.  They should be exactly the required
 *  number when you come into this routine.
 */ 
if(argc != ARGS_REQUIRED)
    {
    TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d.\n",
        ARGS_REQUIRED, argc);
    return(TSH_BAD_ARG_COUNT);
    }


/*  atol() the subcase and expected return value to numbers.
 */
SubCase = atol(argv[0]);
EStatus = atol(argv[1]);


/*  Assign the correct UUID to the ObjectUUID.
 */
switch(atol(argv[2]))
    {
    case Valid_UUID:
        ObjectUUID = &tsh_c_valid_UUID;

        break; 

    case Invalid_UUID:
        ObjectUUID = &tsh_c_invalid_UUID;
        break; 

    case NULL_UUID:
        ObjectUUID = (uuid_t *)NULL;
        break;

    case NIL_UUID:
        ObjectUUID = &tsh_c_nil_UUID;
        break;
    }


/*  Now do the same sort of thing for the type UUID.
 */
switch(atol(argv[3]))
    {
    case Valid_UUID:
        TypeUUID = &tsh_c_valid_UUID;
        break; 

    case Invalid_UUID:
        TypeUUID = &tsh_c_invalid_UUID;
        break; 

    case NULL_UUID:
        TypeUUID = (uuid_t *)NULL;
        break;

    case NIL_UUID:
        TypeUUID = &tsh_c_nil_UUID;
        break;
    }


/*  Just to be on the safe side, unregister this object UUID.  This will make sure
 *  that we have a stable starting point for all of these tests.
 */
for(LoopIndex=0, status = rpc_s_ok; LoopIndex < rpc_c_max_objects; LoopIndex++)
    {
    rpc_object_set_type(ObjectUUID, NULL, &status);
    if (status != rpc_s_ok)
        break;

    TSHMessage(ctx, MOINFO, "Unregister object UUID before starting tests\n");
    }


/*  Perform the appropriate Sub-Test in this module.  REPEATE_NORMAL falls through  
 * into normal so that the test is repeated.
 */
switch(SubCase)
    {
    case REPEAT_REGISTER:
        TSHMessage(ctx, MOINFO, "Running subcase REPEAT_REGISTER\n");

        rpc_object_set_type(ObjectUUID, TypeUUID, &status);
        if(TSHStatus(ctx, SETUP, "rpc_object_set_type() register error",status,rpc_s_ok))
            return(status);

    case REGISTER:
        TSHMessage(ctx, MOINFO, "Running subcase REGISTER\n");

        rpc_object_set_type(ObjectUUID, TypeUUID, &status);
        if(TSHStatus(ctx,TEST,"rpc_object_set_type() register failed",status,EStatus))
             return(status);
        /* If I get here and the expected status is not ok don't attempt the unregister */
        if (EStatus != rpc_s_ok)
             break;

        /*  Unregister the object just registered.
         */
        rpc_object_set_type(ObjectUUID, NULL, &status);
        TSHStatus(ctx ,CLNUP, "rpc_object_set_type() unregister error", 
            status, rpc_s_ok);

        break;

    case UNREGISTER_WITH_NULL:
        TSHMessage(ctx, MOINFO, "Running subcase UNREGISTER_WITH_NULL\n");

        rpc_object_set_type(ObjectUUID, &tsh_c_valid_UUID, &status);
        if(TSHStatus(ctx, SETUP, "rpc_object_set_type() register error",
            status, rpc_s_ok))
            return(status);

        rpc_object_set_type(ObjectUUID, TypeUUID, &status);
        if(TSHStatus(ctx, TEST, "rpc_object_set_type() unregister error",status,EStatus))
            return(status);

        break;

    case UNREGISTER_WITH_NIL:
        TSHMessage(ctx, MOINFO, "Running subcase UNREGISTER_WITH_NIL\n");

        rpc_object_set_type(ObjectUUID, &tsh_c_valid_UUID, &status);
        if(TSHStatus(ctx, SETUP, "rpc_object_set_type() register error",status,rpc_s_ok))
            return(status);

        rpc_object_set_type(ObjectUUID, &tsh_c_nil_UUID, &status);
        if(TSHStatus(ctx, TEST, "rpc_object_set_type() unregister error",status,EStatus))
            return(status);

        break;

    case REPEAT_UNREGISTER:
        TSHMessage(ctx, MOINFO, "Running subcase REPEAT_UNREGISTER\n");

        rpc_object_set_type(ObjectUUID, &tsh_c_valid_UUID, &status);
        if(TSHStatus(ctx, SETUP, "rpc_object_set_type() register error",status,rpc_s_ok))
            return(status);

        /*  Both of these are unregisters since the TypeUUID should be NILL UUIDs.
         */
        rpc_object_set_type(ObjectUUID, TypeUUID, &status);
        if(TSHStatus(ctx, SETUP, "rpc_object_set_type() unregister error",
            status, rpc_s_ok))
            return(status);

        rpc_object_set_type(ObjectUUID, TypeUUID, &status);
        if(TSHStatus(ctx, TEST, "rpc_object_set_type() unregister error",
            status, EStatus))
            return(status);

        break;

    case MAXIMUM_REGISTER:
        TSHMessage(ctx, MOINFO, "Running subcase MAXIMUM_REGISTER\n");

        for (LoopIndex=0; LoopIndex <= rpc_c_max_objects; LoopIndex++)
            { 
            uuid_create(&TestUUID, &status);
            if(TSHStatus(ctx, SETUP, "uuid_create() error", status, rpc_s_ok))
                return(status);

            rpc_object_set_type(ObjectUUID, &TestUUID, &status);
            if(TSHStatus(ctx, SETUP, "rpc_object_set_type() register error",
                status, rpc_s_ok))
                return(status);
            }

        uuid_create(&TestUUID, &status);
        if(TSHStatus(ctx, SETUP, "uuid_create() error", status, rpc_s_ok))
            return(status);

        rpc_object_set_type(ObjectUUID, &TestUUID, &status);
        if(TSHStatus(ctx, TEST, "rpc_object_set_type() register error",
            status, EStatus))
            return(status);

        break;

    case MAXIMUM_UNREGISTER:
        TSHMessage(ctx, MOINFO, "Running subcase MAXIMUM_UNREGISTER\n");

        for (LoopIndex=0; LoopIndex < rpc_c_max_objects; LoopIndex++)
            { 
            uuid_create(&TestUUID, &status);
            if(TSHStatus(ctx, SETUP, "uuid_create() error", status, rpc_s_ok))
                return(status);

            rpc_object_set_type(ObjectUUID, &TestUUID, &status);
            if(TSHStatus(ctx, SETUP, "rpc_object_set_type() register error",
                status, rpc_s_ok))
                return(status);
            }

        rpc_object_set_type(ObjectUUID, &tsh_c_nil_UUID, &status);
        if(TSHStatus(ctx, TEST, "rpc_object_set_type() unregister error", 
            status, EStatus))
            return(status);

        /*  Unregister the object just registered.
         */
        rpc_object_set_type(ObjectUUID, NULL, &status);
        TSHStatus(ctx,CLNUP,"rpc_object_set_type() unregister error",
            status, rpc_s_ok);

        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
        return(TSH_UNKNOWN_SUBCASE);
    }


return(rpc_s_ok);
}

