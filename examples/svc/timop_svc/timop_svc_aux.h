/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: timop_svc_aux.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:23  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:55:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:11  root]
 * 
 * Revision 1.1.2.1  1994/06/14  19:57:36  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:15:15  weir]
 * 
 * $EndLog$
 */
/*
 * The Example code is provided to you as "Pass-through Code".
 * Pass-through Code and its related Documentation are not warrented,
 * supported, or indemnified by OSF, and are provided "AS IS".
 * You can use, sublicense, or distribute the same at your own risk.
 */
/*
 * Copyright (c) 1990, 1991  
 * Open Software Foundation, Inc. 
 *  
 * Permission is hereby granted to use, copy, modify and freely distribute 
 * the software in this file and its documentation for any purpose without 
 * fee, provided that the above copyright notice appears in all copies and 
 * that both the copyright notice and this permission notice appear in 
 * supporting documentation.  Further, provided that the name of Open 
 * Software Foundation, Inc. ("OSF") not be used in advertising or 
 * publicity pertaining to distribution of the software without prior 
 * written permission from OSF.  OSF makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty. 
 */
/*
 *  OSF DCE Version 1.0 
 */

/*
**	timop_svc_aux.h
**
**      Auxiliary info for timop_svc example.
**	There are other ways to do these things, but we're just
**	illustrating the basics here.
*/

/* Principal names for this example application. */

/* NEW CODE */
#define CLIENT_PRINC_NAME	(unsigned_char_t *)"tsclient"
#define SERVER_PRINC_NAME	(unsigned_char_t *)"tsserver"
#define SERVER_KEYFILE		(unsigned_char_t *)"/tmp/tskeyfile"
/* NEW CODE */

#ifdef _HPUX_SOURCE
#define TRUE 1
#define FALSE 0
#endif



#define NO_FLAG 0
/* ...return code from process_args() => errorless command line scanned       */

#define OBJECT_AMOUNT 2
/* ...number of objects allowed. Note that this number should be equal to     */
/*  the number of object UUIDs given directly below. Both timop_svc_client.c  */
/*  and timop_svc_server.c have an array whose contents are initialized with  */
/*  these.                                                                    */

/* Well-known object uuids... */
#define OBJ_UUID_1      (unsigned_char_t *)"2541af56-43a2-11ca-a9f5-02608c0ffe69"
#define OBJ_UUID_2      (unsigned_char_t *)"2541af56-43a2-11ca-a9f5-02608c0ffe79"

