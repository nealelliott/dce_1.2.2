/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY
 * $Log: dump_errs.c,v $
 * Revision 1.1.6.2  1996/02/17  23:27:21  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:48  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:01:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  21:00:04  root]
 * 
 * Revision 1.1.4.1  1994/06/10  20:49:23  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:13  devsrc]
 * 
 * Revision 1.1.2.2  1993/03/12  04:25:42  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:32:19  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* History from before the name change
 * Revision 1.1.1.2  1993/01/29  20:32:42  mhickey
 * 	Initial check in for the rpc.cds.3 system test.
 *
 * Revision 1.1.1.2  1992/12/03  18:00:43  mhickey
 * 	Inital check-in.
 * 
 * 	Defect 5887.
 *
 *---*
 *---*/

/*---*
 *---* dump_err.c - dump the stored info on errors we are tracking
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
 *---*    Takes an array of pointers to error tracking structs and the size of
 *---*    the array, and dumps the error structs.
 *---*
 *---* CALLED BY
 *---*    Reporting functions
 *---* 
 *---* CALLS INTO 
 *---*    'C' library
 *---*
 *---* PREREQUISITES
 *---*/

/*---*
 *---* Includes
 *---*/
#include <stdio.h>
#include <time.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>

#include <err_track.h>
#include <ctime_nonl.h>

/*---*
 *---* rpcsec2_dump_errs(err_stat_array, nerr_stats)
 *---*
 *---*    print the array of errors encountered.  The array is of pointers
 *---*    to type err_stat_t (defined in err_track.h), and is loaded
 *---*    by rpcsec2_check_stat when errors are encountered.
 *---*/
void
dump_errs(int id_num, char *caller_id, err_stat_p_t err_stat_array[], 
                  int nerr_stats)
{
   int i;
   char *gcp;
 
   /*---* if any errors *---*/
   if (nerr_stats)
   {
      printf("%s(%d): ERRORS ENCOUNTERED\n", caller_id, id_num);
   
      /*---* loop through the array... *---*/
      for (i=0; i < nerr_stats; i++)
      {
         /*---* take the newline off of the time string *---*/
         CTIME_NO_NL(gcp, &(err_stat_array[i]->first_tm))
         /*---* print struct *---*/
         printf(
            "%s(%d):   ERROR %d: code: %d count: %d time: %s string: %s\n",
            caller_id,id_num, i, err_stat_array[i]->err_code, 
            err_stat_array[i]->err_count, gcp, err_stat_array[i]->message);
      }
   }
}
