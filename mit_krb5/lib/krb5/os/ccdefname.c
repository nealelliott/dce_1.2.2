/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: ccdefname.c,v $
 * Revision 1.1.2.1  1996/06/05  21:08:36  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:04:50  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/os/ccdefname.c
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
 * Return default cred. cache name.
 */

#define NEED_WINDOWS
#include "k5-int.h"
#include <stdio.h>

char *
krb5_cc_default_name(context)
    krb5_context context;
{
    char *name = getenv(KRB5_ENV_CCNAME);
    static char name_buf[160];
    
    if (name == 0) {

#ifdef HAVE_MACSOCK_H
	strcpy (name_buf, "STDIO:krb5cc");
#else
#ifdef _WINDOWS
        {
            char defname[160];                  /* Default value */

            GetWindowsDirectory (defname, sizeof(defname)-7);
            strcat (defname, "\\krb5cc");
            strcpy (name_buf, "FILE:");
            GetPrivateProfileString(INI_FILES, INI_KRB_CCACHE, defname,
                name_buf+5, sizeof(name_buf)-5, KERBEROS_INI);
        }
#else
	sprintf(name_buf, "FILE:/tmp/krb5cc_%d", getuid());
#endif
#endif
	name = name_buf;
    }
    return name;
}
    
