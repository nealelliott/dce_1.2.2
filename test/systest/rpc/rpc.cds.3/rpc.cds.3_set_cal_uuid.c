/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc.cds.3_set_cal_uuid.c,v $
 * Revision 1.1.78.2  1996/02/17  23:27:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:01  marty]
 *
 * Revision 1.1.78.1  1995/12/11  22:43:02  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/08  19:42 UTC  bissen
 * 	merge from HPDCE01
 * 	[1995/12/11  22:20:07  root]
 * 
 * Revision 1.1.76.1  1994/02/23  21:43:55  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:39:50  pellis]
 * 
 * Revision 1.1.74.2  1993/10/14  17:37:09  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:24  cmckeen]
 * 
 * Revision 1.1.74.1  1993/09/14  15:01:13  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/14  15:00:33  cmckeen]
 * 
 * Revision 1.1.6.2  1993/08/24  14:56:25  bissen
 * 	Included tools/systest.h for TET
 * 	[1993/08/24  14:54:11  bissen]
 * 
 * Revision 1.1.2.2  1993/03/12  04:28:21  mhickey
 * 	Last checkin before initial submit to dce1.0.2.
 * 	[1993/03/12  04:19:50  mhickey]
 * 
 * 	Initial check -in for the rpc.cds.3 system test.
 * 	[1993/01/29  20:28:15  mhickey]
 * 
 * 	Initial checkin of rpc.cds.3.
 * 	[1992/12/22  04:48:31  mhickey]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <errno.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <dce/dce.h>
#include <dce/id_base.h>

/*
 * added for TET 
 */
#include "../../tools/systest.h"

#include <rpc.cds.3_defs.h>

void
rpccds3_set_cal_uuid(char *cal_cds_name, uuid_t *cal_uuid, int *uuid_from, 
                     uuid_vector_t **uuid_v_pp, err_stat_p_t err_stats,
                     int *nerr_stats,int debug_sw, int *stat)
{
   unsigned32 status;
   rpc_ns_handle_t inq_ctxt;
   uuid_t cds_uuid;

   *stat = rpccds3_set_cal_uuid_ok;

   /*-----------------------------------------------------*
    *---* check for presence of current namespace
    *---* entry for this calendar
    *-----------------------------------------------------*
    *---*  Since we are operating in the model of an object oriented
    *---*  namespace with group reference back to a server containing the
    *---*  bindings, we need to find out if there is already a namespace 
    *---*  entry for each calendar, and if there is, get the uuid out of it
    *---*  and see if we want to use it.  The handling is like this:
    *---*    cal_uuid      cds entry    cds uuid    uuid_from   uuid_v_p
    *---*    -----------   -----------  ---------   ----------  ------------
    *---*      null         none         n.a.        new_uuid    null 
    *---*      null         exists       none        new_uuid    null
    *---*      null         exists       exists      cds_uuid    null
    *---*      exists       none         n.a         cal_uuid    null
    *---*      exists       exists       none        cal_uuid    null
    *---*      exists       exists       exists/     both_uuid   null
    *---*                                matches 
    *---*                                cal uuid
    *---*      exists       exists       exists/     cal_uuid    cds uuid
    *---*                                differs
    *---*                                from cal
    *---*                                uuid
    *---*
    *---*/ 
   
   /*---* setup to look for the object in the namnespace *---*/
   printf(" Server: Looking for existing namespace entry for calendar %s...",
                                                        cal_cds_name);
   rpc_ns_entry_object_inq_begin(rpc_c_ns_syntax_dce, 
                                 (unsigned_char_t *)cal_cds_name,
                                 &inq_ctxt, &status);
   
   /*---* if we can't do the setup, set return variable to an error *---*/
   if (rpcsec2_check_stat(status, "looking for existing name space entry",
                              err_stats, nerr_stats,  debug_sw,0))
   {
      if (debug_sw)
         printf("Bad name for object lookup?\n");
      uuid_create_nil(cal_uuid, &status);
      *stat = rpccds3_set_cal_uuid_bad_name;
   }
   else
   {
      /*---* setup okay, look for the entry and get a uuid *---*/
      if (debug_sw)
         printf("    getting next uuid...");
      rpc_ns_entry_object_inq_next(inq_ctxt, &cds_uuid, &status);
      
      /*---* if no entry, or no members (one of case 1,2,4,5 above) *---*/
      if (status == rpc_s_entry_not_found || status == rpc_s_no_more_members)
      {
         unsigned32 locstat;

         printf("\n   name space entry for %s %s\n", cal_cds_name,
                      status == rpc_s_entry_not_found? "does not exist":
                                                            "has no uuid");
         /*---* if no cal_uuid (case 1 or 2 above )... *---*/
         if (uuid_is_nil(cal_uuid, &status) == TRUE)
	 {
            /*---* create a new uuid *---*/
            printf("    creating new uuid for calendar %s...", cal_cds_name);
            uuid_create(cal_uuid, &locstat);
 
            /*---* if a problem, return an error status *---*/
            if (rpcsec2_check_stat(locstat, "creating obj uuid",
                                    err_stats, nerr_stats,  debug_sw, 0))
            {
                *stat = rpccds3_set_cal_uuid_create;
	    }
            /*---* else no problem, set indicator of new uuid *---*/
            else
	    {
               *uuid_from = uuid_from_uuidgen;
            }
	 }
         /*---* else we have a cal uuid (case 4 or 5 above)... *---*/
         else
	 {
            /*---* indicate that caller should use cal uuid *---*/
            *uuid_from = uuid_from_cal;
	 }
      }
      /*---* else if other error from obj search *---*/
      else if (rpcsec2_check_stat(status, "getting uuid", err_stats, 
                                                   nerr_stats,  debug_sw, 0))
      {
         /*---* return error status *---*/
         *stat = rpccds3_set_cal_uuid_ns_err;
      }
      /*---* else we got a object and a uuid (one of cases 3,6,7 above *---*/
      else
      {
         /*---* if no cal uuid passed (case 3 above)  *---*/
         if (uuid_is_nil(cal_uuid, &status) == TRUE)
         {
            /*---* set cal uuid to be the uuid from the cds entry *---*/
            *cal_uuid = cds_uuid;
            *uuid_from = uuid_from_cds;
         }
         /*---* 
          *---* else we have a cal uuid.  If it matches the cds uuid 
          *---* (case 6 above)
          *---*/
         else if (uuid_equal(cal_uuid, &cds_uuid, &status) == TRUE )
         {
            /*---* return both match status *---*/
            *uuid_from = uuid_from_both;
         }
         /*---* old uuid in cds entry (case 7 above ) *---*/
         else
         {
            /*---* get space for a uuid_vector and a uuid *----*/
            if (
              !(*uuid_v_pp=(uuid_vector_t *)malloc(sizeof(uuid_vector_t))) ||
              !(((*uuid_v_pp)->uuid)[0] = (uuid_t *)malloc(sizeof(uuid_t))) )
            {
               /*---* report error *---*/
               *stat = rpccds3_set_cal_uuid_no_spc;
            }
            /*---* got the space, copy in the old uuid and set the count *---*/
            else
            {
               *((*uuid_v_pp)->uuid)[0] = cds_uuid;
               (*uuid_v_pp)->count = 1;
               /*---* set return value *---*/
               *uuid_from = uuid_from_cal;
            }
         }
      } 
   }
   /*---* free the context for the inquiry *---*/
   printf(" Server: freeing inquiry context...");
   rpc_ns_entry_object_inq_done(&inq_ctxt, &status);

   if (rpcsec2_check_stat(status, "freeing inquiry context",
                                     err_stats, nerr_stats, debug_sw, 0))
      printf("    leaking memory :-(...\n");
   
   /*---* we are done *---*/
   return;
} 
           




