/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dump_binding.c,v $
 * Revision 1.1.6.2  1996/02/17  23:27:19  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:47  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:01:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  21:00:02  root]
 * 
 * Revision 1.1.4.1  1994/06/10  20:49:22  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:12  devsrc]
 * 
 * Revision 1.1.2.2  1993/03/12  04:25:37  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:31:28  mhickey]
 * 
 * $EndLog$
 */

/*---* 
 *---* dump_binding.c - converts a passed binding to a string and prints it
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
 *---*    Takes a passed pointer to a binding, converts it to a string, and 
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
dump_binding(rpc_binding_handle_t binding)
{
   unsigned_char_t *str_p;
   unsigned32 stat;

   rpc_binding_to_string_binding(binding, &str_p, &stat);
   if (stat != rpc_s_ok)
      printf("(dump_binding): Error converting binding (stat = %u)!\n", stat);
   else
   {
      printf("(dump_binding): binding is %s\n", str_p);

      /*---* free the memory *---*/            
      rpc_string_free(&str_p, &stat);
  
      if (stat != rpc_s_ok)
         printf("(dump_binding): Leaking memory :-{ (stat = %u)\n", stat);
   }
}
