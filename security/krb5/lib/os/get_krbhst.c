/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: get_krbhst.c,v $
 * Revision 1.1.5.2  1996/02/18  00:15:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:41  marty]
 *
 * Revision 1.1.5.1  1995/12/08  18:03:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:34  root]
 * 
 * Revision 1.1.3.3  1992/12/29  16:30:06  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:17:43  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:42:37  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:34:10  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:50:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  get_krbhst.c V=4 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/get_krbhst.c,v $
 * $Author: marty $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * krb5_get_krbhst() function.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_get_krbhst_c [] =
"$Id: get_krbhst.c,v 1.1.5.2 1996/02/18 00:15:20 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <stdio.h>
#include <ctype.h>

/*
 Figures out the Kerberos server names for the given realm, filling in a
 pointer to an argv[] style list of names, terminated with a null pointer.
 
 If the realm is unknown, the filled-in pointer is set to NULL.

 The pointer array and strings pointed to are all in allocated storage,
 and should be freed by the caller when finished.

 returns system errors
*/

/*
 * Implementation:  the server names for given realms are stored in a
 * configuration file, 
 * named by krb5_config_file;  the first token (on the first line) in
 * this file is taken as the default local realm name.
 * 
 * Each succeeding line has a realm name as the first token, and a server name
 * as a second token.  Additional tokens may be present on the line, but
 * are ignored by this function.
 *
 * All lines which begin with the desired realm name will have the
 * hostname added to the list returned.
 */

extern char *krb5_config_file;		/* extern so can be set at
					   load/runtime */

krb5_error_code
krb5_get_krbhst(realm, hostlist)
krb5_data *realm;
char ***hostlist;
{
    FILE *config_file;
    char filebuf[BUFSIZ];
    krb5_error_code retval;
    char *cp, *cp2;
    register char **rethlist = 0;
    int hlindex = 0;
    int hlsize = 2;                     /* Always have to null terminate
                                         * host list, so be sure there
                                         * is enough room in the common
                                         * case.
                                         */

    if (!(config_file = fopen(krb5_config_file, "r")))
	/* can't open */
	return KRB5_CONFIG_CANTOPEN;

    if (fgets(filebuf, sizeof(filebuf), config_file) == NULL)
	retval = KRB5_CONFIG_BADFORMAT;
    else {
	retval = 0;
	rethlist = (char **)calloc(hlsize, sizeof (*rethlist));
	for (;;) {
	    if (fgets(filebuf, sizeof(filebuf), config_file) == NULL)
		break;
	    if (strncmp(filebuf, realm->data, realm->length))
		continue;		/* no match */

	    /* +1 to get beyond trailing space */
	    if (strlen(filebuf) < (size_t) realm->length + 1) {
		/* no hostname on config line */
		retval = KRB5_CONFIG_BADFORMAT;
		break;
	    }

            if (!isspace(filebuf[realm->length])) {
                continue;               /* no match */
            }

            /* Throw away any whitespace between tokens */
            for (cp = &filebuf[realm->length + 1]; isspace(*cp); cp++);
            if (! *cp) {
		/* no hostname on config line */
		retval = KRB5_CONFIG_BADFORMAT;
		break;
	    }

            /* Throw away any trailing whitespace or tokens */
            for (cp2 = cp+1; *cp2 && !isspace(*cp2); cp2++);
            *cp2 = '\0';

	    rethlist[hlindex] = strdup(cp);
	    if (!rethlist[hlindex]) {
		for (--hlindex; hlindex >= 0; hlindex--)
		    free(rethlist[hlindex]);
		xfree(rethlist);
		rethlist = 0;
		retval = ENOMEM;
		break;
	    }

	    if (++hlindex >= hlsize) {
		/* need larger pointer array */
		hlsize *= 2;
		rethlist = (char **)realloc((char *)rethlist,
					    hlsize * sizeof(*rethlist));
		if (!rethlist) {
		    /* XXX clean up the old saved strings?
		       realloc might possibly trash them if it fails? */
		    retval = ENOMEM;
		    break;
		}
	    }
	    rethlist[hlindex] = 0;
	}
    }
    (void) fclose(config_file);

    if (hlindex == 0) {
	xfree(rethlist);
	rethlist = 0;
	retval = KRB5_REALM_UNKNOWN;
    }
    *hostlist = rethlist;

    return retval;
}

