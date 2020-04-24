/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: chk_stat.c,v $
 * Revision 1.1.7.2  1996/02/17  23:27:17  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:46  marty]
 *
 * Revision 1.1.7.1  1995/12/11  22:01:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:59:59  root]
 * 
 * Revision 1.1.5.2  1994/06/10  20:49:18  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:09  devsrc]
 * 
 * Revision 1.1.5.1  1993/12/22  20:51:10  mhickey
 * 	no_changes
 * 	[1993/12/22  20:50:01  mhickey]
 * 
 * 	Fixed odd logic that called dce_inq_err_text twice, and
 * 	added an if so that if the status passed in is less
 * 	that the constant CHK_STAT_DCE_LOW_ERR, then the
 * 	string that normally contains the name of the
 * 	operation that generated the status is used as the
 * 	error message.  This allows callers to pass in
 * 	their own messages and get them tracked.
 * 	[1993/12/14  16:44:14  mhickey]
 * 
 * Revision 1.1.2.2  1993/03/12  04:25:27  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:31:18  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* History from before name change
 *---*
 * Revision 1.1.1.2  1993/01/29  20:32:13  mhickey
 * 	Initial check in for the rpc.cds.3 system test.
 *
 * Revision 1.1.3.3  1993/01/21  16:59:35  mhickey
 * 	Added a call to rpcsec2_reg_gen after the NOTICE of the
 * 	first occurance of an error so that we can see what the
 * 	state is when the error occurs.
 * 
 * Revision 1.1.2.3  1993/01/14  18:11:53  mhickey
 * 	Fixed type mismatch
 * 	[1993/01/14  16:40:09  mhickey]
 * 
 * Revision 1.1.2.2  1992/12/03  19:53:49  mhickey
 * 	Inital check-in.
 * 
 * 	Defect 5887.
 * 	[1992/12/03  17:57:39  mhickey]
 * 
 * 	Initial checkin of base for rpc.security.2
 * 	[1992/10/06  17:26:22  mhickey]
 *---*
 *---*/

/*---*
 *---* rep_timer.c - uses pthread_cond_timedwait to time report generation.
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
 *---*    Takes a status variable containing the status returned by a DCE
 *---*    API call, a pointer to an array of error stats, a count of the
 *---*    the number of error stats being tracked, a debug variable that
 *---*    indicates how much output to generate, and a nowrite variable that
 *---*    indicates if all output is to be avoided.  If debug is true, and the
 *---*    status variable contains a DCE error status value, then the DCE 
 *---*    error value and message are printed.  If debug is false, nowrite is
 *---*    is false, and the status variable contains an error value, then just
 *---*    the error code is printed.   If nowrite is true and debug is false,
 *---*    then nothing is printed.  If the status variable contains rpc_s_ok
 *---*    and debug is true, then the string "done" and a newline are printed
 *---*    (which indicates the intended use of this call, which is something
 *---*    like:
 *---*       printf("Doing a DCE operation...");
 *---*       dce_op(arg1, arg2, &status);
 *---*       rpcsec2_check_stat(status, ...);
 *---*
 *---*    In any case, if the status variable contains a DCE error value, the
 *---*    error stats array is searched for an entry referring to that value.
 *---*    If it is found, then the count for that enty is incremented.  If no
 *---*    entry for the value is found, then a new entry is created and 
 *---*    it's fields are initialized with the value, the text indicating the
 *---*    error, and the time that it was encountered.  Note that the first 
 *---*    each error is encountered, message is printed, and a report generated
 *---*    using the function pointed to by the rep_gen_func_p pointer in order
 *---*    to show where the test was at when the failure ocurred.
 *---*
 *---* CALLED BY
 *---*    Everything.
 *---* 
 *---* CALLS INTO 
 *---*    'C' library, DCE library (dce_error_inq_text())
 *---*
 *---* PREREQUISITES
 *---*    Assumes the external definition of a pointer rep_gen_func_p to a
 *---*    routine  that takes an integer as an arguement.
 *---*/

#include <stdio.h>
#include <errno.h>
#include <dce/dce_error.h>
#include <dce/idlbase.h>
#include <dce/rpc.h>
#include <dce/nbase.h>

#include <err_track.h>
#include <ctime_nonl.h>

void (*rep_gen_func_p)();

/*---*
 *---* this name should be changed, but we don't have the time to fix
 *---* the MANY occurances in the code right now
 *---*/

int rpcsec2_check_stat(unsigned32 status, char *op_string, 
               err_stat_p_t err_stat_array[],
               int *nerr_stats, int debug, int no_print)
{
   int   retval = 0;
   int   stat2 = 0, i;
   unsigned char  msgbuff[dce_c_error_string_len];
   int pid = getpid();
   

   if (status != rpc_s_ok)
   {
       if (debug)
          printf("\n\t\tError(%d) %s...checking error code...", pid,op_string);
       if (status > CHK_STAT_LOW_DCE_ERR)
          dce_error_inq_text((unsigned long)status, msgbuff, &stat2);
       else
       {
          stat2=0; strcpy(msgbuff, op_string);
       }
       if (stat2 && debug)
       {
          printf("Error(%d): Couldn't get message for error %d,\n",pid,status);
          strcpy(msgbuff, "Couldn't get message");
       }
       else if (debug)
       {
         printf("done.\n\n\t\tError code(%d): %d\n\t\tError Message(%d): %s\n",
                                                              pid, status, pid,
                                                              msgbuff);
       }
       else if (!no_print)
       {
             printf("Error(%d)\n",status);
       }
       
       retval=1;
      
       /*---* 
        *---* update the err_stat_array note that because the array is small,
        *---* we just use linear searches
        *---*/
       for (i=0; i < *nerr_stats; i++)
       {
          /*---* if this pointer is nil... *---*/
          if (!err_stat_array[i])
	  { 
             /*---* error, if in debug mode *---*
             if (debug)
                printf("Error(%d): err_stat count is messed. Resetting...\n",
                         pid);
             /*---* 
              *---* set to i since we will update the count later when we add
              *---* the new error struct.
              *---*/
             *nerr_stats = i;
	  }
          /*---* else if we find the code in the array already *---*/
          else if (err_stat_array[i]->err_code == status)
          { 
             /*---* update the count *---*/
             err_stat_array[i]->err_count++;
             break;
	  }
       }
       
       /*---* 
        *---* if we get here and i = nerr_stats then we have a new error.
        *---* if we are not already tracking the maximum number of errors,
        *---*/
       if (i == *nerr_stats)
       {
          if (*nerr_stats < CHK_STAT_MAX_ERR)
          {
            /*---* try to get space for a new struct, if trouble *---*/
            if (!(err_stat_array[i] =(err_stat_p_t)malloc(sizeof(err_stat_t))))
	    {
               /*---* if in debug mode, report *---*/
               if (debug)
                  printf("Error(%d): Can't get space for new error code.\n",
                                                                          pid);
            }
            else
            {
               char *gcp;   /*---* for printing initial notice of error *---*/

               /*---* got the space, load the struct *---*/
               /*---* error code *---*/
               err_stat_array[i]->err_code = status;
               /*---* count *---*/
               err_stat_array[i]->err_count = 1;
               /*---* time first encountered *---*/
               (void)time((time_t *)&(err_stat_array[i]->first_tm));
               /*---* 
                *---* copy the value from the call to dce_inq_err_text above
                *---*/
               strcpy((unsigned char *)(err_stat_array[i]->message), msgbuff);
               (*nerr_stats)++;

               /*---* take the newline off of the time string *---*/
               CTIME_NO_NL(gcp, &(err_stat_array[i]->first_tm))
               /*---* print struct *---*/
               printf(
     "NOTICE(%d): JUST GOT ERROR %d: code: %d count: %d time: %s string: %s\n",
        pid, i, err_stat_array[i]->err_code, err_stat_array[i]->err_count, gcp,
                                                   err_stat_array[i]->message);
               rep_gen_func_p(0);

	    }
          }         
          else
          {
             if (debug)
                printf("Error(%d): Too many errors to track.\n",pid);
          }
       }
       else
       {
          if (debug)
             printf("Matched error.\n");
       }
   }
   else if (!no_print || debug)
          printf("done.\n\n");

   return(retval);
}   
