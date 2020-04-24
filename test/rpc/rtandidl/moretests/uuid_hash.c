/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: uuid_hash.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:53  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:34  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:15  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:30:21  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:47:29  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:50:37  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:17:33  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:07  devrcs
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
ndr_long_int    SubCase;                /* argv[0]: SubCAse for this test modulee */
error_status_t  EStatus;                /* argv[1]: expected status, compared to status */
error_status_t  status;                 /* status from rpc_* and uuid_* calls */
uuid_p_t        TestUUID;               /* a temp UUID used for verification */
unsigned16      TestHash;               /* expected hash value from above UUID */
unsigned16      UUIDHash;               /* actual hash value from above UUID */


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


/*  Load the expected values for the UUID and its hash values.
 */
switch(atol(argv[2]))
    {
    case Valid_UUID:
        TestUUID = &tsh_c_valid_UUID;
        TestHash = tsh_c_valid_UUID_hash;

        TSHMessage(ctx, MOINFO, "Hash of a valid UUID\n");
        break; 

    case Invalid_UUID:
        TestUUID = &tsh_c_invalid_UUID;;
        TestHash = tsh_c_invalid_UUID_hash;

        TSHMessage(ctx, MOINFO, "Hash of an invalid UUID\n");
        break; 

    case NULL_UUID:
        TestUUID = (uuid_t *)NULL;
        TestHash = tsh_c_NULL_UUID_hash;

        TSHMessage(ctx, MOINFO, "Hash of NULL UUID\n");
        break;

    case NIL_UUID:
        TestUUID = &tsh_c_nil_UUID;
        TestHash = tsh_c_nil_UUID_hash;

        TSHMessage(ctx, MOINFO, "Hash of a nil UUID\n");
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
    case HASH:
        TSHMessage(ctx, MOINFO, "Running subcase HASH\n");

        UUIDHash = uuid_hash(TestUUID, &status);
        if(TSHStatus(ctx, TEST, "uuid_hash() failed", status, EStatus))
            return(status);

        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
        return(TSH_UNKNOWN_SUBCASE);
    }


return(TSH_SUCCESS);
}







