/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: uuid_to_string.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:56  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:36  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:18  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:30:28  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:47:41  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:50:48  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:17:48  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:09  devrcs
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


#define ARGS_REQUIRED        3          /* This routine should have 4 args */

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock      *ctx;
ndr_long_int  argc;
unsigned_char_p_t argv[];
{
ndr_long_int    SubCase;                /* argv[0]: SubCAse for this test modulee */
error_status_t  EStatus;                /* argv[1]: expected status, compared to status */
unsigned_char_t *TestString;            /* argv[2]: UUID string to convert to UUID */
unsigned_char_p_t UUIDString;          /* generated from the converted UUID */
error_status_t  status;                 /* status from rpc_* and uuid_* calls */
uuid_t          NewUUID;                /* UUID structure derived from string */
uuid_p_t        TestUUID;               /* a temp UUID used for verification */
ndr_long_int    StringEqual;            /* used to check if the string is correct */


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


/*  Figure out what type of UUID string we want to convert over to a real
 *  UUID structure.  Also set up TestUUID so we know what we can compare the
 *  new UUID to.
 */
switch(atol(argv[2]))
    {
    case Valid_UUID:
        TestString = (unsigned_char_t *)tsh_c_valid_UUID_string;
        TestUUID   = &tsh_c_valid_UUID;

        TSHMessage(ctx, MOINFO, "UUID is a valid UUID\n");
        break; 

    case Invalid_UUID:
        TestString = (unsigned_char_t *)tsh_c_invalid_UUID_string;
        TestUUID   = &tsh_c_invalid_UUID;

        TSHMessage(ctx, MOINFO, "UUID is an invalid UUID\n");
        break; 

    case NULL_UUID:
        TestString = (unsigned_char_t *)NULL;
        TestUUID   = (uuid_t *)NULL;

        TSHMessage(ctx, MOINFO, "UUID is NULL\n");
        break;

    case NIL_UUID:
        TestString = (unsigned_char_t *)tsh_c_nil_UUID_string;
        TestUUID   = &tsh_c_nil_UUID;

        TSHMessage(ctx, MOINFO, "UUID is a nil UUID\n");
        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Invalid entry for UUID type field\n");
        return(TSH_UNKNOWN_ARGUMENT);
    }


/*  Perform the appropriate Sub-Test in this module.  REPEATE_NORMAL falls through  
 * into normal so that the test is repeated.
 */
switch(SubCase)
    {
    case TO_STRING:
        TSHMessage(ctx, MOINFO, "Running subcase TO_STRING\n");

        uuid_to_string(TestUUID, &UUIDString, &status);
        if(TSHStatus(ctx, TEST, "uuid_to_string() failed", status, EStatus))
            return(status);

   
        /*  Only continue if we are supposed to.  If we are checking for bad data
         *  then we shouldn't compare the data since it dosen't make sense to.
         */
        if(EStatus == rpc_s_ok)
            {
            TSHMessage(ctx, MOINFO, "Expected UUID String: %s\n",  TestString);
            TSHMessage(ctx, MOINFO, "Received UUID String: %s\n",  UUIDString); 

            StringEqual = strcmp(UUIDString, TestString);
            if(TSHStatus(ctx, TEST, "strcmp() returned incorrect result", 
                StringEqual, 0))
                return(TSH_DATA_MISSMATCH);
            }

        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
        return(TSH_UNKNOWN_SUBCASE);
    }


return(TSH_SUCCESS);
}

