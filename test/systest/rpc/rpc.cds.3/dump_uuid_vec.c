/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dump_uuid_vec.c,v $
 * Revision 1.1.6.2  1996/02/17  23:27:25  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:50  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:42:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:19:40  root]
 * 
 * Revision 1.1.4.1  1994/06/10  20:49:28  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:20  devsrc]
 * 
 * Revision 1.1.2.2  1993/03/12  04:25:57  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:38:21  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* dump_uuid_vec.c - dumps a vector of uuids 
 *---*
 *---* ASSOCIATION
 *---*    This file is part of the DCE 1.0.2 System Tests
 *---*
 *---*    The DCE 1.0.2 system tests corresponds to the 
 *---*    DCE1.0.2 Core Component System Test plan.  For more information on 
 *---*    the operation and goals of the DCE 1.0.2 system tests, see the test 
 *---*    plan.
 *---* 
 *---* FUNCTION
 *---*    Dumps the vector of uuids in human readable format.
 *---*
 *---* CALLED BY
 *---*    Anybody
 *---* 
 *---* CALLS INTO 
 *---*    'C' library
 *---*
 *---* PREREQUISITES
 *---*    None.
 *---*/

#include <stdio.h>
#include <dce/rpc.h>

void
dump_uuid_v(uuid_vector_p_t uuid_v, unsigned32 *status)
{
   int i;
   unsigned_char_t *str_p;
   unsigned32 loc_stat;

   printf("\tUUID vector at 0x%o contains %d UUIDS:\n",&uuid_v, uuid_v->count);
   *status = 0;

   /*---* print the list of the uuids we got *---*/
   for (i=0; i< uuid_v->count; i++)
   {
      /*---* convert it *---*/
      uuid_to_string(uuid_v->uuid[i], &str_p, &loc_stat);
      if (loc_stat != uuid_s_ok)
      {
         printf("error converting uuid\n");
         *status = 1;
      }

      printf("             %s\n",str_p);

      /*---* free the string *---*/
      rpc_string_free(&str_p, &loc_stat);

      if (loc_stat != rpc_s_ok)
      {
         printf("error freeing uuid string\n");
      }
   }
}
