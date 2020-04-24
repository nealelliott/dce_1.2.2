/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ctime_nonl.h,v $
 * Revision 1.1.8.2  1996/02/18  23:07:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:29  marty]
 *
 * Revision 1.1.8.1  1995/12/11  22:01:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  21:00:00  root]
 * 
 * Revision 1.1.6.1  1994/06/10  20:49:19  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:11  devsrc]
 * 
 * Revision 1.1.4.1  1993/10/20  19:36:46  ohara
 * 	eliminate a control char at end of file
 * 	[1993/10/17  23:37:00  ohara]
 * 
 * Revision 1.1.2.2  1993/03/12  04:25:32  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:31:23  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* ctime_nonl.h - macro to get ctime without a newline on the end
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
 *---*    Define a macro to get an ascii representation of the time without
 *---*    a newline.
 *---* 
 *---* CALLED BY
 *---*    N.A.
 *---* 
 *---* CALLS INTO 
 *---*    Nothing
 *---*
 *---* PREREQUISITES
 *---*    None.
 *---*/

/*---*
 *---* Lets be idempotent!
 *---*/
#if !defined(_SYSTEST_CTIME_NO_NL_H)

#   define _SYSTEST_CTIME_NO_NL_H

    /*---*
     *---* macro to null the newline on the end of strings returned from ctime.
     *---* returns with cp pointed at the buffer.
     *---* NOTE: requires include of time.h.
     *---*/
#   define CTIME_NO_NL(cp, tbuf) \
            {\
               cp = (char *)ctime((time_t *)(tbuf));\
               *(cp + (strlen(cp)-1)) = '\0';\
            }

#endif  /*---* !defined(_SYSTEST_CTIME_NO_NL_H *---*/
