/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: uuid_equal.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:50  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:32  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:13  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:30:15  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:47:20  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:50:28  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:17:24  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:02  devrcs
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


#define ARGS_REQUIRED        4          /* This routine should have 4 args */

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock      *ctx;
ndr_long_int  argc;
unsigned_char_p_t argv[];
{
uuid_t          *FirstUUID, *SecondUUID;  /* a temp UUID and the NIL UUID */
ndr_long_int    SubCase;                /* SubCAse for this test modulee */
error_status_t  status;                 /* status from rpc_* and uuid_* calls */
error_status_t  EStatus;                /* expected status, compared against status */
boolean32       UUIDEqual, EUUIDEqual;  /* boolean return values from uuid_equal() */


/*  Check the number of arguments passed in.  They should be exactly the required
 *  number when you come into this routine.
 */ 
if(argc != ARGS_REQUIRED)
    {
    TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d.\n",
        ARGS_REQUIRED, argc);
    return(TSH_BAD_ARG_COUNT);
    }


/*  Init the UUID tests
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
        FirstUUID = &tsh_c_valid_UUID;

        TSHMessage(ctx, MOINFO, "First UUID is a valid UUID\n");
        break; 

    case Invalid_UUID:
        FirstUUID = &tsh_c_invalid_UUID;

        TSHMessage(ctx, MOINFO, "First UUID is an invalid UUID\n");
        break; 

    case NULL_UUID:
        FirstUUID = (uuid_t *)NULL;

        TSHMessage(ctx, MOINFO, "First UUID is NULL\n");
        break;

    case NIL_UUID:
        FirstUUID = &tsh_c_nil_UUID;

        TSHMessage(ctx, MOINFO, "First UUID is a nil UUID\n");
        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Invalid entry for UUID type field\n");
        return(TSH_UNKNOWN_ARGUMENT);
    }


/*  Now do the same sort of thing for the type UUID.
 */
switch(atol(argv[3]))
    {
    case Valid_UUID:
        SecondUUID = &tsh_c_valid_UUID;

        TSHMessage(ctx, MOINFO, "Second UUID is a valid UUID\n");
        break; 

    case Invalid_UUID:
        SecondUUID = &tsh_c_invalid_UUID;

        TSHMessage(ctx, MOINFO, "Second UUID is an invalid UUID\n");
        break; 

    case NULL_UUID:
        SecondUUID = (uuid_t *)NULL;

        TSHMessage(ctx, MOINFO, "Second UUID is NULL\n");
        break;

    case NIL_UUID:
        SecondUUID = &tsh_c_nil_UUID;

        TSHMessage(ctx, MOINFO, "Second UUID is a nil UUID\n");
        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Invalid entry for UUID type field\n");
        return(TSH_UNKNOWN_ARGUMENT);
    }


/*  Are they supposed to be equal?
 */
if(FirstUUID == SecondUUID)
    EUUIDEqual = TRUE;
else
    EUUIDEqual = FALSE;


TSHMessage(ctx, MOINFO, "First and second UUID should%s be equal\n",
    (EUUIDEqual)? "": " NOT");



/*  Perform the appropriate Sub-Test in this module.  REPEATE_NORMAL falls through  
 * into normal so that the test is repeated.
 */
switch(SubCase)
    {
    case EQUAL:
        TSHMessage(ctx, MOINFO, "Running subcase EQUAL\n");

        UUIDEqual = uuid_equal(FirstUUID, SecondUUID, &status);
        if(TSHStatus(ctx, TEST, "uuid_equal() failed", status, EStatus))
            return(status);

        TSHMessage(ctx, MOINFO, "First and second UUID are%s equal\n",
            (UUIDEqual)? "": " NOT");
        if(TSHStatus(ctx, TEST, "uuid_equal() returned incorrect result", 
            UUIDEqual, EUUIDEqual))
            return(TSH_DATA_MISSMATCH);
    
        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
        return(TSH_UNKNOWN_SUBCASE);
    }


return(TSH_SUCCESS);
}







