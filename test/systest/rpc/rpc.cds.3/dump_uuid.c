/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dump_uuid.c,v $
 * Revision 1.1.6.2  1996/02/17  23:27:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:49  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:42:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:19:39  root]
 * 
 * Revision 1.1.4.1  1994/06/10  20:49:27  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:20  devsrc]
 * 
 * Revision 1.1.2.2  1993/03/12  04:25:52  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:32:28  mhickey]
 * 
 * $EndLog$
 */

/*---* 
 *---* dump_uuid.c - converts a passed uuid to a string and prints it
 *---*
 *---* ASSOCIATION
 *---*    This file is part of the rpc.cds.3 DCE System Test
 *---*
 *---*    The rpc.cds.3 system test corresponds to test rpc.cds.3 in the 
 *---*    DCE1.0.2 Core Component System Test plan.  For more information on 
 *---*    the operation and goals of the rpc.cds.3 system test, see the test 
 *---*    plan, or the README file in this directory.
 *---* 
 *---* FUNCTION
 *---*    Takes a passed pointer to a uuid, converts it to a string, and 
 *---*    prints it.  To keep the routine simple and loosely coupled, no
 *---*    errors encountered by this routine are tracked, and any errors
 *---*    are reported by status code only.
 *---*
 *---*/

/*-------------------------------------------*
 *---*  Includes
 *-------------------------------------------*/
/*---*
 *---* Global include files
 *---*/
#include <stdio.h>
#include <pthread.h>
#include <dce/id_base.h>
#include <stdlib.h>
#include <dce/rpc.h>

void 
dump_uuid(uuid_t *uuid_p)
{
   unsigned_char_t *str_p;
   unsigned32 loc_stat;

   /*---* convert it *---*/
   uuid_to_string(uuid_p, &str_p, &loc_stat);
   if (loc_stat != uuid_s_ok)
      printf("(dump_uuid): Error converting uuid (stat = %u)!\n", loc_stat);
   else      
   {
      printf("(dump_uuid): UUID at 0x%x is %s\n", uuid_p, str_p);
     
      /*---* free the string *---*/
      rpc_string_free(&str_p, &loc_stat);
   
      if (loc_stat != rpc_s_ok)
      { 
         printf("(dump_uuid):error freeing uuid string(stat = %u)!\n");
      } 
   }
}

