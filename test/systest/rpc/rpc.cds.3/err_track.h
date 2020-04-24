/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: err_track.h,v $
 * Revision 1.1.13.2  1996/02/18  23:07:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:32  marty]
 *
 * Revision 1.1.13.1  1995/12/11  22:42:21  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/08  20:06 UTC  bissen
 * 	merge from HPDCE01
 * 	[1995/12/11  22:19:41  root]
 * 
 * Revision 1.1.11.1  1993/12/22  20:51:12  mhickey
 * 	no_changes
 * 	[1993/12/22  20:50:06  mhickey]
 * 
 * 	Added a constant CHK_STAT_DCE_LOW_ERR to allow
 * 	chk_stat to diferentialte between DCE errors and
 * 	application errors.
 * 	[1993/12/14  16:45:10  mhickey]
 * 
 * Revision 1.1.9.1  1993/09/14  15:01:03  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/14  15:00:27  cmckeen]
 * 
 * Revision 1.1.5.3  1993/08/24  14:00:38  eheller
 * 	Include utime.h
 * 	[1993/08/24  13:56:14  eheller]
 * 
 * Revision 1.1.5.2  1993/08/19  12:18:34  eheller
 * 	Modified for TET.
 * 	[1993/07/23  18:06:16  eheller]
 * 
 * Revision 1.1.2.2  1993/03/12  04:26:01  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:38:26  mhickey]
 * 
 * $EndLog$
 */

/*
 * err_track.h - values, data structures, etc. for the error tracking
 *               package used by some system tests.
 *
 * ASSOCIATION
 *    This file is part of the DCE 1.0.2 System Tests
 *
 *    The DCE 1.0.2 system tests corresponds to the
 *    DCE1.0.2 Core Component System Test plan.  For more information on 
 *    the operation and goals of the DCE 1.0.2 system tests, see the test 
 *    plan. 
 *
 * FUNCTION
 *    Define values and data structures, for the error tracking
 *    package (implemented by rpcsec2_check_stat()) used by some of the
 *    system tests.
 * 
 * CALLED BY
 *    N.A.
 * 
 * CALLS INTO 
 *    Nothing
 *
 * PREREQUISITES
 *    None.
 */
#include <sys/time.h>
/*
 * Lets be idempotent!
 */
/* need to include this for hp-ux*/
#include <utime.h>
#if !defined(_SYSTEST_ERR_TRACK_H)

#  define _SYSTEST_ERR_TRACK_H

   /*-------------------------------------
    *    Error Tracking Package
    *-------------------------------------
   /*
    * Max number of errors we will track
    */
#  if !defined(CHK_STAT_MAX_ERR)   
#     define CHK_STAT_MAX_ERR 10 
#  endif  /* !defined(CHK_STAT_MAX_ERR) */

   /*---* 
    *---* if the status value is not 0, and less than 1000, then assume the
    *---* passed string is the error message.  This is a hack until check_stat
    *---* interface can be changed.
    *---*/
#  define CHK_STAT_LOW_DCE_ERR 1000

   
   /*
    * Error tracking struct. This is modified by check_stat, and printed 
    * using rpcsec2_dump_errs()
    */
   typedef struct {
      int err_code,
          err_count;
      time_t first_tm;
      char message[dce_c_error_string_len];
   } err_stat_t, *err_stat_p_t;

#endif   /* !defined(_SYSTEST_ERR_TRACK_H) */
