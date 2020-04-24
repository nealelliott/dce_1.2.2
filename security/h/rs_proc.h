/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_proc.h,v $
 * Revision 1.1.4.2  1996/02/18  22:59:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:51  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:25:32  root
 * 	Submit
 * 	[1995/12/11  15:14:56  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:08:35  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:01  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_proc.h V=1 05/10/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - mainline process control - external defs
 * 
 *  Deal with process creation and backgrounding.  This code is isolated from
 *  the server mainline to allow it to avoid conflicts with cma system call
 *  wrappers.  We need to perform the initial process fork outside the CMA
 *  context.
 */

#ifndef _rs_proc_h_included_
#define _rs_proc_h_included_

boolean32 rs_process_make_server (
#ifdef __STDC__
    int     *argc,
    char    *argv[]
#endif
);

void rs_process_ready_to_listen (
#ifdef __STDC__
    void
#endif
);

#endif  /* _rs_proc_h_included_ */
