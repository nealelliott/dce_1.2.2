/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: uuid_compare.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:29  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:09  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:29:59  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:47:02  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:50:07  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:17:08  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:00  devrcs
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


#define ARGS_REQUIRED        5          /* This routine should have 5 args */

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock      *ctx;
ndr_long_int  argc;
unsigned_char_p_t argv[];
{
uuid_t          *FirstUUID, *SecondUUID;  /* a temp UUID and the NIL UUID */
ndr_long_int    SubCase;                /* SubCAse for this test modulee */
error_status_t  status;                 /* status from rpc_* and uuid_* calls */
error_status_t  EStatus;                /* expected status, compared against status */
signed32        position, EPosition;    /* UUID positions */
uuid_t          VariantUUID;            /* Used to compare UUIDs */
ndr_long_int    LoopTimes;              /* generic loop index counter */
boolean32       Iterations = TRUE;      /* assume we willgo through iterations */


/*  Check the number of arguments passed in.  They should be exactly the required
 *  number when you come into this routine.
 */ 
if(argc != ARGS_REQUIRED)
    {
    TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d.\n",
        ARGS_REQUIRED, argc);
    return(TSH_BAD_ARG_COUNT);
    }


/*  Init the UUID tests so the proper rrpc mutexes get set.
 */
UUID_INIT


/*  atol() the subcase and expected return value to numbers.
 */
SubCase   = atol(argv[0]);
EStatus   = atol(argv[1]);
EPosition = atol(argv[2]);


/*  determine if they are to be equal.  This allows one subcase to handle
 *  the different input combinations.
 */
switch(atol(argv[3]))
    {
    case Valid_UUID:
        FirstUUID = &tsh_c_valid_UUID;

	TSHMessage(ctx, MOINFO, "First UUID is a valid UUID\n");
        break; 

    case Invalid_UUID:
        FirstUUID = &tsh_c_invalid_UUID;
        Iterations = FALSE;

        TSHMessage(ctx, MOINFO, "First UUID is an invalid UUID\n");

        break; 

    case NULL_UUID:
        FirstUUID = (uuid_t *)NULL;
        Iterations = FALSE;

        TSHMessage(ctx, MOINFO, "First UUID is NULL\n");
        break;

    case NIL_UUID:
        FirstUUID = &tsh_c_nil_UUID;
        Iterations = FALSE;

        TSHMessage(ctx, MOINFO, "First UUID is a nil UUID\n");
        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Invalid entry for UUID type field\n");
        return(TSH_UNKNOWN_ARGUMENT);
    }


/*  Now do the same sort of thing for the type UUID.
 */
switch(atol(argv[4]))
    {
    case Valid_UUID:
        SecondUUID = &tsh_c_valid_UUID;

        TSHMessage(ctx, MOINFO, "Second UUID is a valid UUID\n");
        break; 

    case Invalid_UUID:
        SecondUUID = &tsh_c_invalid_UUID;
        Iterations = FALSE;

        TSHMessage(ctx, MOINFO, "Second UUID is an invalid UUID\n");
        break; 

    case NULL_UUID:
        SecondUUID = (uuid_t *)NULL;
        Iterations = FALSE;

        TSHMessage(ctx, MOINFO, "Second UUID is NULL\n");
        break;

    case NIL_UUID:
        SecondUUID = &tsh_c_nil_UUID;
        Iterations = FALSE;

        TSHMessage(ctx, MOINFO, "Second UUID is a nil UUID\n");
        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Invalid entry for UUID type field\n");
        return(TSH_UNKNOWN_ARGUMENT);
    }


/*  We are not allowing actual lexical compares of the two UUIDs if either
 *  of the UUID types are not valid UUIDs.  This way, only valid UUIDs can be 
 *  compared since that is the only way meaningful lexical info can be
 *  acquired.  Let the user know via a WARNING that a non-fatal operand 
 *  missmatch occurred.
 */
if(!Iterations && EPosition)
    TSHMessage(ctx, MOWARNING, 
        "Both UUIDs have to be valid (non-nil), Ignoring lexical compare\n");


/*  Create a variant UUID that will be used to compare against the original UUID.
 *  Then copy the contents of the valid UUID to the variant UUID.  Do this only
 *  if we are doing Iterations.
 */
if(Iterations == TRUE)
    {
    uuid_create(&VariantUUID, &status);
    if(TSHStatus(ctx, SETUP, "uuid_create() failed", status, rpc_s_ok))
        return(status);
    VariantUUID.time_low                  = tsh_c_valid_UUID.time_low;
    VariantUUID.time_mid                  = tsh_c_valid_UUID.time_mid;
    VariantUUID.time_hi_and_version       = tsh_c_valid_UUID.time_hi_and_version;
    VariantUUID.clock_seq_hi_and_reserved = tsh_c_valid_UUID.clock_seq_hi_and_reserved;
    VariantUUID.clock_seq_low             = tsh_c_valid_UUID.clock_seq_low;
    for (LoopTimes = 0; LoopTimes < 6; LoopTimes++ )
        VariantUUID.node[LoopTimes]       = tsh_c_valid_UUID.node[LoopTimes];


    /*  Are they supposed to be equal? Print this info and then set up for the big
     *  compare.  This involves getting another UUID structure so we can vary its
     *  elements.
     */
    switch(EPosition)
        {
        case -1:
            TSHMessage(ctx,MOINFO, "First UUID should be lexically AFTER the second\n");
            FirstUUID = &VariantUUID;

            break;

        case 0:
            TSHMessage(ctx,MOINFO, "First UUID should be lexically EQUAL to the second\n");
            break;

        case 1:
            TSHMessage(ctx,MOINFO, "First UUID should be lexically BEFORE the second\n");
            SecondUUID = &VariantUUID;

            break;

        default:
            TSHMessage(ctx, MOFAILURE, "Invalid entry for UUID type field\n");
            return(TSH_UNKNOWN_ARGUMENT);
        }
    } 	/* Do Iterations? */


/*  Perform the appropriate Sub-Test in this module.  
 */
switch(SubCase)
    {
    case COMPARE:
        TSHMessage(ctx, MOINFO, "Running subcase COMPARE\n");


        /*  If this is supposed to be equal, loop only once, otherwise, loop 11
         *  times (number of members of UUID structures to vary)
         */
        if(EPosition == 0 || !Iterations)
            LoopTimes = 1;
        else
            LoopTimes = 11;

        TSHMessage(ctx, MOINFO, "Loop through %d iterations of UUID comparisons\n",
            LoopTimes);


        while(LoopTimes--)
            {
            /*  Each iteration of the loop, decrement one of the structure members
             *  in the variant UUID.  If the previous iteration decremented the  
             *  previous member, then increment it again.
             *
             *  This is really gross but it will force execution through all of the
             *  paths of  uuid_compare().
             */
            switch(LoopTimes)
                {
                case 10:  VariantUUID.time_low--;
                          break;
                case  9:  VariantUUID.time_low++;
                          VariantUUID.time_mid--;
                          break;
                case  8:  VariantUUID.time_mid++;
                          VariantUUID.time_hi_and_version--;
                          break;
                case  7:  VariantUUID.time_hi_and_version++;
                          VariantUUID.clock_seq_hi_and_reserved--;
                          break;
                case  6:  VariantUUID.clock_seq_hi_and_reserved++;
                          VariantUUID.clock_seq_low--;
                          break;
                case  5:  VariantUUID.clock_seq_low++;
                          VariantUUID.node[0]--;
                          break;
                case  4:  VariantUUID.node[0]++;
                          VariantUUID.node[1]--;
                          break;
                case  3:  VariantUUID.node[1]++;
                          VariantUUID.node[2]--;
                          break;
                case  2:  VariantUUID.node[2]++;
                          VariantUUID.node[3]--;
                          break;
                case  1:  VariantUUID.node[3]++;
                          VariantUUID.node[4]--;
                          break;
                case  0:  if(EPosition) 
                              {
                              VariantUUID.node[4]++;
                              VariantUUID.node[5]--;
                              }
                          break;
                }


            position = uuid_compare(FirstUUID, SecondUUID, &status);
            if(TSHStatus(ctx, TEST, "uuid_compare() failed", status, EStatus))
                return(status);

           
            /*  Again if we are doing the iterations, check the data returned
             *  If we are only doing the input data tests, we cannot expect
             *  valid position info from the compare.
             */
            if(Iterations)
                {
                TSHMessage(ctx, MOINFO, 
                    "Loop %2d: First UUID is lexically %s the second UUID\n",
                     LoopTimes,
                     (position)?((position == 1)? "BEFORE": "AFTER" ): "EQUAL to");
                if(TSHStatus(ctx, TEST, "uuid_equal() returned incorrect result", 
                    position, EPosition))
                    return(TSH_DATA_MISSMATCH);
                }
            }
 
        break;

    default:
        TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
        return(TSH_UNKNOWN_SUBCASE);
    }


return(TSH_SUCCESS);
}







