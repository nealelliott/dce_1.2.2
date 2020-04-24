/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: free.h,v $
 * Revision 1.1.6.2  1996/02/18  23:02:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:58  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:42:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:03  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:18  sommerfeld
 * 	Add krb5_xfree.
 * 	[1994/08/26  15:51:11  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/12/29  13:59:19  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:52:32  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  free.h V=4 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/free.h,v $
 * $Author: marty $
 * $Id: free.h,v 1.1.6.2 1996/02/18 23:02:27 marty Exp $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * definitions for free routines
 */


#ifndef KRB5_FREE__
#define KRB5_FREE__

/* to keep lint happy */
#define xfree(val) free((char *)(val))
#define krb5_xfree(val) free((char *)(val))

#define krb5_free_data(val) { xfree((val)->data); xfree(val);}

#endif /* KRB5_FREE__ */
