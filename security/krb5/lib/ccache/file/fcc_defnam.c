/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_defnam.c,v $
 * Revision 1.1.6.2  1996/02/18  00:09:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:36  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:46:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:49  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:23  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:51:52  sommerfeld]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
**
*/
/*
 * Ooops.  This file is completely unncessesary, I think.  <sigh>
 *
 * Barr3y
 */

/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_defnam.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
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
 * This file contains the source code for krb5_fcc_default_name.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_defnam_c[] =
"$Id: fcc_defnam.c,v 1.1.6.2 1996/02/18 00:09:13 marty Exp $";
#endif /* !lint && !SABER */

#include "fcc.h"


static char krb5_default_name_string[KRB5_FCC_MAXLEN] = "";

/*
 * Effects:
 * 
 * Forms the default credential cache name for the current user, which
 * is defined in the following way.  If it is set, the environment
 * variable "KRB5CCACHE" will be used (up to the maximum number of
 * characters of a legal operating system defined path).  Otherwise
 * TKT_ROOT (from fcc.h) and the user's uid are concatenated to
 * produce the ticket file name (e.g., "/tmp/tkt123").  The pointer
 * returned is to static storage; the name must be copied elsewhere.
 */

char *
krb5_fcc_default_name ()
{
     char *krb5ccache, *getenv();
     int len;

     /* Is the environment variable defined? */
     krb5ccache = getenv("KRB5CCACHE");
     if (krb5ccache != NULL) {
	  len = strlen(krb5ccache);
	  len = (len < sizeof(krb5_default_name_string) ? len :
		 sizeof(krb5_default_name_string));
	  
	  strncpy(krb5_default_name_string, krb5ccache, len);
	  krb5_default_name_string[len] = '\0';
     }

     /* No.  Use TKT_ROOT and uid */
     else {
	  /* It'd be nice if we didn't have to pull in printf */
	  sprintf(krb5_default_name_string, "%s%d", TKT_ROOT, getuid());
     }

     return krb5_default_name_string;
}
