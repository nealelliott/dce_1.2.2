/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prvb_att_002.c,v $
 * Revision 1.1.35.2  1996/02/17  23:31:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:14  marty]
 *
 * Revision 1.1.35.1  1995/12/11  23:03:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:29  root]
 * 
 * Revision 1.1.33.1  1993/10/12  19:37:38  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:31:02  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:54:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:12:24  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:19:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1989, 1990 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and copied
 *  only in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any other
 *  copies thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without notice
 *  and should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	PTHREAD Test Programs
 *
 *  FILENAME:
 *
 *	PRVB_ATT_002
 *
 *  TEST TOPICS:
 *
 *	ATT  -  Attr Objects
 *
 *  ABSTRACT:
 *
 *	Test deferred delete for attr objects which have
 *	"live" objects attached to them.  This'll be tough to really
 *	validate automatically; it's really intended to go along with
 *	manual human observation through the debugger.  However, in
 *	an automatic test system, it will PROBABLY accvio/seg. fault
 *	if things aren't being done right.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	25 October 1989
 *
 *  MODIFICATION HISTORY:
 * 
 * 	001	Paul Curtin	30 May 1990
 *		Converted test program to use pthread interface.
 *	002	Dave Butenhof	2 July 1990
 *		Fix to use condattr for condition, not mutexattr.
 */

/*
 *  INCLUDE FILES
 */

#include <pthread.h>
#include <dce/cts_test.h>


main ()
    {
    pthread_mutexattr_t	att1;
    pthread_condattr_t	att2;
    pthread_mutex_t	mutex1, mutex2;
    pthread_cond_t	cv1, cv2;
    int			status;

    cts_test ("PRVB_ATT_002", "Validate cache sequencing and reclamation");

    /*
     * Create an attr object and a "child" attr object which
     * refers to the first.
     */
    status = pthread_mutexattr_create (&att1);
    check(status,"pthread_mutexattr_create");
    status = pthread_condattr_create (&att2);
    check(status,"pthread_condattr_create");

    /*
     * Create some objects off the second one
     */
    status = pthread_mutex_init (&mutex1, att1);
    check(status,"pthread_mutex_init");
    status = pthread_mutex_init (&mutex2, att1);
    check(status,"pthread_mutex_init");
    status = pthread_cond_init (&cv1, att2);
    check(status,"pthread_cond_init");
    status = pthread_cond_init (&cv2, att2);
    check(status,"pthread_cond_init");

    /*
     * Delete some to put them on the cache
     */
    status = pthread_mutex_destroy (&mutex1);
    check(status,"pthread_mutex_destroy");
    status = pthread_cond_destroy (&cv1);
    check(status,"pthread_cond_destroy");

    /*
     * Now, delete the second attr object.  This should flush the cache
     * and free the attr object to it's parent's cache
     */
    status = pthread_mutexattr_delete(&att2);
    check(status,"pthread_mutexattr_delete");

    /*
     * Now, delete the attr objects and create new ones. This will set delete
     * pending, since they still has live children.
     */
    status = pthread_mutexattr_delete(&att1);
    check(status,"pthread_mutexattr_delete");
    status = pthread_mutexattr_create(&att1);
    check(status,"pthread_mutexattr_create");
    status = pthread_condattr_delete(&att2);
    check(status,"pthread_condattr_delete");
    status = pthread_condattr_create(&att2);
    check(status,"pthread_condattr_create");

    /*
     * Now, delete the other child objects... they should be destroyed since
     * delete-pending is set on their parent.  Deletion of the last object
     * should also result in the parent attr object being destroyed.
     */
    status = pthread_mutex_destroy (&mutex2);
    check(status,"pthread_mutex_destroy");
    status = pthread_cond_destroy (&cv2);
    check(status,"pthread_cond_destroy");

    cts_result ();
    }
