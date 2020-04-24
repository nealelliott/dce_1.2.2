/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: object_inq_type.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:46  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:07:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:07  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:06:12  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:40:49  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:37:28  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:10:16  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:23  devrcs
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

#define ARGS_REQUIRED 4
#define rpc_c_max_objects	64		/* from runtime source file comobj.h	*/


void inquiry_fn(object_uuid, type_uuid, status)
uuid_t   *object_uuid;
uuid_t   *type_uuid;
unsigned32   *status;
{
  /* return the type UUID of a given object UUID */
  unsigned32 stat;

  if (uuid_equal(&tsh_c_valid_UUID, object_uuid, &stat)) {
    memcpy(type_uuid, &tsh_c_valid_UUID, sizeof(uuid_t));
    *status = rpc_s_ok;
  }
  else {
    memcpy(type_uuid, &tsh_c_nil_UUID, sizeof(uuid_t));
    *status = rpc_s_object_not_found;
  }
}


ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock            *ctx;
ndr_long_int        argc;
unsigned_char_p_t   argv[];
{
  uuid_p_t               ObjectUUID,		/* Object that we want to register	*/
                         TypeUUID;		/* Type of the object			*/
  uuid_t                 TestUUID;		/* temporary UUID			*/
  ndr_long_int           SubCase;		/* argv[0], subcase to run		*/
  error_status_t         EStatus;		/* argv[1], expected return from test	*/
  error_status_t         status;		/* generic status variable		*/
  int                    index;			/* generic index value			*/


  /**************************************************************************/
  /* Check the number of arguments passed in.  They should be exactly the   */
  /* required number when you come into this routine. Looking for a subcase,*/
  /* a bogus expected_status, and a hostname.                               */
  /**************************************************************************/

  if (argc != ARGS_REQUIRED) {
    TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
               ARGS_REQUIRED, argc);
    return(TSH_BAD_ARG_COUNT);
  }


  SubCase = atol(argv[0]);
  EStatus = atol(argv[1]);


  /* Assign the appropriate UUID to the ObjectUUID */

  switch(atol(argv[2])) {
    case Valid_UUID:
      ObjectUUID = &tsh_c_valid_UUID;
      break;
    case Invalid_UUID:
      ObjectUUID = &tsh_c_invalid_UUID;
      break;
    case NULL_UUID:
      ObjectUUID = (uuid_t *) NULL;
      break;
  }

  /* do the same sort of thing for the TypeUUID */

  switch(atol(argv[3])) {
    case Valid_UUID:
      TypeUUID = &tsh_c_valid_UUID;
      break;
    case Invalid_UUID:
      TypeUUID = &tsh_c_invalid_UUID;
      break;
    case NULL_UUID:
      TypeUUID = (uuid_t *) NULL;
      break;
  }

  /*********************************************************************************/
  /* Just to be on the safe side, unregister this object UUID. This will make sure */
  /* that we have a stable and well known starting point for all of these tests.   */
  /*********************************************************************************/

  for (index = 0, status = rpc_s_ok; index < rpc_c_max_objects; index++) {
    rpc_object_set_type(ObjectUUID, NULL, &status);
    if (status != rpc_s_ok)
      break;
    TSHMessage(ctx, MOINFO, "Unregister object UUID before starting tests.\n");
  }


  /* Perform the actual test according to the Subcase */

  switch (SubCase) {
    case UNREGISTERED:
      TSHMessage(ctx, MOINFO, "Running subcase UNREGISTERED\n");

      rpc_object_inq_type(ObjectUUID, &TestUUID, &status);
      TSHMessage(ctx, MOTRACE, "rpc_object_inq_type status: %d\n", status);
      if (TSHStatus(ctx, TEST, "rpc_object_inq_type() failed", status, EStatus))
        return (status);

      /* check if the returned type is equal to the NULL UUID */
      if (!uuid_equal(&tsh_c_nil_UUID, &TestUUID, &status)) {
        TSHMessage(ctx, CLNUP, "Type UUID have different value!\n");
        return (TSH_TEST_FAILED);
      }
      break;

    case REG_BY_RUNTIME:
      TSHMessage(ctx, MOINFO, "Running subcase REG_BY_RUNTIME\n");

      rpc_object_set_type(ObjectUUID, TypeUUID, &status);
      if (TSHStatus(ctx, SETUP, "rpc_object_set_type() register failed", status, rpc_s_ok))
        return (status);

      rpc_object_inq_type(ObjectUUID, &TestUUID, &status);
      TSHMessage(ctx, MOTRACE, "rpc_object_inq_type status: %d\n", status);
      if (TSHStatus(ctx, TEST, "rpc_object_inq_type() failed", status, EStatus))
        return (status);

      /* check if the returned type is equal to the original type set */
      if (!uuid_equal(TypeUUID, &TestUUID, &status)) {
        TSHMessage(ctx, CLNUP, "Type UUID have different value!\n");
        return (TSH_TEST_FAILED);
      }

      /* unregister the stuff before leaving */
      rpc_object_set_type(ObjectUUID, NULL, &status);
      TSHStatus(ctx, CLNUP, "rpc_object_set_type unregister failed\n", status, rpc_s_ok);
      break;

    case REG_BY_APPLICATION:
      TSHMessage(ctx, MOINFO, "Running subcase REG_BY_APPLICATION\n");

      rpc_object_set_inq_fn(inquiry_fn, &status);
      TSHMessage(ctx, MOTRACE, "rpc_object_set_inq_fn status: %d\n", status);
      if (TSHStatus(ctx, SETUP, "rpc_object_set_inq_fn() failed", status, EStatus))
        return(status);

      rpc_object_inq_type(ObjectUUID, &TestUUID, &status);
      TSHMessage(ctx, MOTRACE, "rpc_object_inq_type status: %d\n", status);
      if (TSHStatus(ctx, TEST, "rpc_object_inq_type() failed", status, EStatus))
        return (status);

      /* check if the returned type is equal to the original type set */
      if (!uuid_equal(TypeUUID, &TestUUID, &status)) {
        TSHMessage(ctx, CLNUP, "Type UUID have different value!\n");
        return (TSH_TEST_FAILED);
      }

      /* do we have to un-set the inquiry fn before exiting? How? */
      rpc_object_set_inq_fn(NULL, &status);
      TSHMessage(ctx, MOTRACE, "rpc_object_set_inq_fn status: %d\n", status);
      if (TSHStatus(ctx, CLNUP, "rpc_object_set_inq_fn() failed", status, rpc_s_ok))
        return(status);
      break;

    case SET_INQ_FN:
      TSHMessage(ctx, MOINFO, "Running subcase SET_INQ_FN\n");

      rpc_object_set_inq_fn(inquiry_fn, &status);
      TSHMessage(ctx, MOTRACE, "rpc_object_set_inq_fn status: %d\n", status);
      if (TSHStatus(ctx, TEST, "rpc_object_set_inq_fn() failed", status, EStatus))
        return(status);

      /* do we have to un-set the inquiry fn before exiting? How? */
      rpc_object_set_inq_fn(NULL, &status);
      TSHMessage(ctx, MOTRACE, "rpc_object_set_inq_fn status: %d\n", status);
      if (TSHStatus(ctx, CLNUP, "rpc_object_set_inq_fn() failed", status, rpc_s_ok))
        return(status);
      break;

    default:
      TSHMessage(ctx, MOFAILURE, "Unrecognized subcase: %d\n", SubCase);
      return (TSH_UNKNOWN_SUBCASE);
  }

  return(TSH_SUCCESS);
}
