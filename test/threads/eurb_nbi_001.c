/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eurb_nbi_001.c,v $
 * Revision 1.1.37.1  1996/06/04  22:14:42  arvind
 * 	pthread_join[0] should be [i]
 * 	[1996/03/08  22:18 UTC  bissen  /main/DCE_1.2/1]
 *
 * Revision 1.1.35.2  1996/02/17  23:31:21  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:47:53  marty]
 * 
 * Revision 1.1.35.1  1995/12/11  23:02:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:36:09  root]
 * 
 * Revision 1.1.33.1  1994/04/01  20:11:17  jd
 * 	First code cleanup drop
 * 	[1994/03/28  21:17:57  jd]
 * 
 * Revision 1.1.31.1  1993/10/12  19:36:58  rhw
 * 	Fix for OT8201 - prepend dce to header files
 * 	[1993/10/08  20:30:43  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:52:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/30  16:10:52  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:22:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1991 by
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
 *	CMA Test Programs
 *
 *  FILENAME:
 *
 *	EURB_NBI_001
 *
 *  TEST TOPICS:
 *
 *	NBI  -  Non-Blocking (Unix) I/O
 *
 *  ABSTRACT:
 *
 *	This addresses a problem reported by Dale Tonogai of HP, where the
 *	DECthreads operation masks can get out of sync if multiple threads
 *	are performing concurrent I/O on the same file.
 *
 *  AUTHORS:
 *
 *	Dave Butenhof
 *
 *  CREATION DATE:
 *
 *	18 October 1991
 *
 *  MODIFICATION HISTORY:
 *
 */

/*
 *  INCLUDE FILES
 */

#include <pthread_exc.h>
#include <dce/cts_test.h>

#define NSERVERS        2
#define MSGLEN          10

int tube[2];

int server();
int client();

int
main	(int argc, char *argv[], char *envp[])
    {
    int i;
    int result;
    pthread_t server_id[NSERVERS], client_id;


    cts_test ("EURB_NBI_001", "Test that concurrent I/O on same file works");

    if (pipe (tube) == -1)
	cts_failed ("system call failure: pipe(tube)");

    for (i = 0; i < NSERVERS; i++) {
	pthread_create (
		&server_id[i],
		pthread_attr_default,
		(pthread_startroutine_t) server,
		(pthread_addr_t) i);
	}

    pthread_create (
	    &client_id,
	    pthread_attr_default,
	    (pthread_startroutine_t) client,
	    (pthread_addr_t) 0);

    for (i = 0; i < NSERVERS; i++) {
	pthread_join (server_id[i], (pthread_addr_t *) &result);
	}

    pthread_join (client_id, (pthread_addr_t *) &result);

    if (close (tube[0]) == -1)
	cts_failed ("error closing tube 0");

    if (close (tube[1]) == -1)
	cts_failed ("error closing tube 1");

    cts_result ();
    }

server (id)
int id;
    {
    int i;
    char inmsg[80];

    if (read (tube[0], inmsg, MSGLEN) != MSGLEN)
	cts_failed ("error in server read");

    }

client()
    {
    int i;
    char outmsg[80];
    struct timespec delay;

    /*
     * Make sure servers get to run first
     */
    delay.tv_sec = 1;
    delay.tv_nsec = 0;
    pthread_delay_np(&delay);

    for (i = 0; i < NSERVERS; i++) {
	if (write (
		tube[1],
		outmsg,
		MSGLEN) != MSGLEN)
	    cts_failed ("client write failed");
        }

    }
/*  DEC/CMS REPLACEMENT HISTORY, Element EURB_NBI_001.C */
/*  *2     7-NOV-1991 08:25:43 BUTENHOF "Fix name" */
/*  *1    18-OCT-1991 12:11:00 BUTENHOF "" */
/*  DEC/CMS REPLACEMENT HISTORY, Element EURB_NBI_001.C */
