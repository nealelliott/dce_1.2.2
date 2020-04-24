/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: narrow.h,v $
 * Revision 1.1.4.2  1996/02/18  23:02:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:17  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:01:11  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:53:31  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  narrow.h V=2 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/narrow.h,v $
 * $Author: marty $
 * $Id: narrow.h,v 1.1.4.2 1996/02/18 23:02:52 marty Exp $
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
 * definitions to un-widen prototype types...see <krb5/widen.h>
 * and <krb5/base-defs.h>
 */

#ifndef NARROW_PROTOTYPES

/* only needed if not narrow, i.e. wide */

#undef krb5_boolean
#undef krb5_msgtype
#undef krb5_kvno

#undef krb5_addrtype
#undef krb5_keytype
#undef krb5_enctype
#undef krb5_cksumtype
#undef krb5_authdatatype

#endif /* not NARROW_PROTOTYPES */
