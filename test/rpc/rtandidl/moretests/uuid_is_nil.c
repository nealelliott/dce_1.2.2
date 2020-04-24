/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: uuid_is_nil.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:55  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:35  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:16  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:30:23  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:47:35  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:50:42  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:17:43  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:08  devrcs
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


#define ARGS_REQUIRED        3          /* This routine should have 3 args */

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock      *ctx;
ndr_long_int  argc;
unsigned_char_p_t argv[];
{
ndr_long_int    SubCase;                /* SubCAse for this test modulee */
error_status_t  status;                 /* status from rpc_* and uuid_* calls */
error_status_t  EStatus;                /* expected status, compared against status */
uuid_t          *TestUUID;              /* UUID to test against nil */
boolean32       EUUIDIsNil;             /* Expected status of nilness for above UUID */
boolean32       UUIDIsNil;              /* Actual status of nilness for above UUID */


/*  Check the number of arguments passed in.  They should be exactly the required
 *  number when you come into this routine.
 */ 
if(argc != ARGS_REQUIRED)
    {
    TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d.\n",
        ARGS_REQUIRED, argc);
    return(TSH_BAD_ARG_COUNT);
    }


/*  Init the uuid tests.
 */
UUID_INIT


/*  atol() the subcase and expected return value to numbers.
 */
SubCase = atol(argv[0]);
EStatus = atol(argv[1]);


/*  Assign the two uuid pointers to the right type of uuids.  In the end,
 *  determine if they are to be equal.  This allows one subcase to handle
 *  the different input combinations.
 */
switch(atol(argv[2]))
    {
    case Valid_UUID:
        TestUUID = &tsh_c_valid_UUID;
        EUUIDIsNil = FALSE;

        TSHMessage(ctx, MOINFO, "Using a valid UUID\n");
        break; 

    case Invalid_UUID:
        TestUUID = &tsh_c_invalid_UUID;
        EUUIDIsNil = FALSE;

        TSHMessage(ctx, MOINFO, "Using an invalid UUID\n");
        break; 

    case NULL_UUID:
        TestUUID = (uuid_t *)NULL;
        EUUIDIsNil = FALSE;

        TSHMessage(ctx, MOINFO, "Using NULL for a UUID\n");
        break;

    case NIL_UUID:
        TestUUID = &tsh_c_nil_UUID;
        EUUIDIsNil = TRUE;

        TSHMessage(ctx, MOINFO, "Using a nil UUID\n");
        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Invalid entry for UUID type field\n");
        return(TSH_UNKNOWN_ARGUMENT);
    }


/*  Perform the appropriate Sub-Test in this module.  
 */
switch(SubCase)
    {
    case IS_NIL:
        TSHMessage(ctx, MOINFO, "Running subcase IS_NIL\n");

        UUIDIsNil = uuid_is_nil(TestUUID, &status);
        if(TSHStatus(ctx, TEST, "uuid_is_nil() failed", status, EStatus))
            return(status);

        if(TSHStatus(ctx, TEST, "uuid_is_nil() returned incorrect result", 
            UUIDIsNil, EUUIDIsNil))
            return(TSH_DATA_MISSMATCH);
    
        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
        return(TSH_UNKNOWN_SUBCASE);
    }


return(TSH_SUCCESS);
}







